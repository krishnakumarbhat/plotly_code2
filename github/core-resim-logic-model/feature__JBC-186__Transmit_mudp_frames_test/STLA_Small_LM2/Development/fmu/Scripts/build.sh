#!/usr/bin/env bash
# =============================================================================
#  build.sh  —  Compile and package LogicModel2.fmu  (Linux)
# =============================================================================
#  Project:  STLA Small LM2 — SIL/ReSim FMU for LRCF radar algorithm
#  FMU:      LogicModel2.fmu  (FMI 2.0 CS, linux64)
#
#  Build steps performed by this script:
#    1. (Optional) Generate CAN signal glue code from DBC via dbc_to_fmi_generator.py
#    2. CMake configure + compile  →  LogicModel2.so
#    3. Stage binaries: LogicModel2.so + ExternalLibs/Libs/*.so
#       (includes libDPH_RR_ADAS_SIL.so — the SIL algorithm library)
#    4. Patch RUNPATH=$ORIGIN on all staged .so files via patchelf
#       (so the FMU resolves libDPH_RR_ADAS_SIL.so from its own binaries/ dir)
#    5. Zip staging area  →  Build/LogicModel2.fmu
#    6. (Optional) Validate the FMU via validate_lrcf_fmu.py
#
#  Runtime notes:
#    - LogicModel2.so loads libDPH_RR_ADAS_SIL.so at runtime via dlopen from
#      its own directory (no hard-coded path, no LD_LIBRARY_PATH required).
#    - ETH TX output from dph_sil_adcam_udp_frame_process is queued internally
#      (g_eth_tx_queue) and drained one frame per DoStep.
#    - Use run_fmu_udp.py to test the built FMU with live or dummy UDP traffic.
#
#  Prerequisites:
#    - CMake >= 3.14, g++ (C++14), patchelf
#    - Python 3 with fmpy  (for --validate and run_fmu_udp.py)
#
#  Usage:
#    ./build.sh                                              # compile + package
#    ./build.sh --generate ../Code/JOB3_LRCF_FD_CAN15.dbc  # regenerate from DBC, then build
#    ./build.sh --node <name>            # DBC node name (default: LRCF, only with --generate)
#    ./build.sh --validate               # also run validate_lrcf_fmu.py after build
#    ./build.sh --clean                  # remove build outputs (not generated sources)
#    ./build.sh --clean --generate <..>  # remove everything, regenerate and build
#    ./build.sh --build-type Release     # skip interactive prompt (Release or Debug)
#
#  If --build-type is not supplied the script prompts interactively.
#
#  Generated files (generated_signal_map.cpp, generated_structs.h,
#  modelDescription.xml) are NOT regenerated on every build.
#  Run with --generate only when JOB3_LRCF_FD_CAN15.dbc changes.
# =============================================================================
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CODE_DIR="${SCRIPT_DIR}/../Code"
BUILD_DIR="${SCRIPT_DIR}/../Build"
cd "$SCRIPT_DIR"

# ── Argument parsing ──────────────────────────────────────────────────────────
DBC_FILE=""
FMU_NODE="LRCF"
DO_GENERATE=0
DO_VALIDATE=0
DO_CLEAN=0
BUILD_TYPE=""   # filled by prompt or --build-type flag

while [[ $# -gt 0 ]]; do
    case $1 in
        --generate)    DO_GENERATE=1; DBC_FILE="$2"; shift 2 ;;
        --node)        FMU_NODE="$2"; shift 2 ;;
        --validate)    DO_VALIDATE=1; shift ;;
        --clean)       DO_CLEAN=1;    shift ;;
        --build-type)  BUILD_TYPE="$2"; shift 2 ;;
        *)             echo "Unknown option: $1"; exit 1 ;;
    esac
done

# ── Build-type selection ──────────────────────────────────────────────────────
if [[ -z "$BUILD_TYPE" ]]; then
    echo ""
    echo "Select build type:"
    echo "  1) Release  (optimised, no debug symbols)"
    echo "  2) Debug    (no optimisation, full debug symbols)"
    read -rp "Enter choice [1/2] (default: 1): " _bt_choice
    case "${_bt_choice}" in
        2|[Dd][Ee][Bb][Uu][Gg])   BUILD_TYPE="Debug"   ;;
        *)                        BUILD_TYPE="Release" ;;
    esac
fi

# Normalise to CMake capitalisation (Release / Debug)
case "${BUILD_TYPE,,}" in
    debug)   BUILD_TYPE="Debug"   ;;
    release) BUILD_TYPE="Release" ;;
    *)
        echo "ERROR: --build-type must be Release or Debug (got '${BUILD_TYPE}')"
        exit 1
        ;;
esac

echo "[config] Build type: ${BUILD_TYPE}"

FMU_NAME="LogicModel2"
CMAKE_BUILD_DIR="${BUILD_DIR}/cmake/${BUILD_TYPE}"
FMU_FILE="${BUILD_DIR}/${FMU_NAME}.fmu"
XML_FILE="${CODE_DIR}/modelDescription.xml"
GEN_CPP="${CODE_DIR}/generated_signal_map.cpp"
GEN_H="${CODE_DIR}/generated_structs.h"

# ── Clean ─────────────────────────────────────────────────────────────────────
if [[ $DO_CLEAN -eq 1 ]]; then
    echo "[clean] removing build outputs…"
    # Remove the type-specific cmake dir; if BUILD_TYPE is already set via
    # --build-type only that tree is removed, otherwise the whole cmake/ dir.
    if [[ -n "$BUILD_TYPE" ]]; then
        rm -rf "${CMAKE_BUILD_DIR}"
    else
        rm -rf "${BUILD_DIR}/cmake"
    fi
    rm -f "${FMU_FILE}"
    if [[ $DO_GENERATE -eq 1 ]]; then
        echo "[clean] removing generated sources…"
        rm -f "${GEN_CPP}" "${GEN_H}" "${XML_FILE}"
    fi
    echo "[clean] done"
fi

# ── Step 1 (optional): Generate sources from DBC ─────────────────────────────
if [[ $DO_GENERATE -eq 1 ]]; then
    if [[ -z "$DBC_FILE" ]]; then
        echo "ERROR: --generate requires a DBC path: --generate <path/to/file.dbc>"
        exit 1
    fi
    if [[ ! -f "$DBC_FILE" ]]; then
        echo "ERROR: DBC file not found: $DBC_FILE"
        exit 1
    fi
    echo ""
    echo "════════════════════════════════════════════════════════"
    echo "  Generate  —  $(basename "$DBC_FILE")  [node: ${FMU_NODE}]"
    echo "════════════════════════════════════════════════════════"
    python3 dbc_to_fmi_generator.py "$DBC_FILE" "$FMU_NODE"
fi

# ── Check generated files exist before compiling ─────────────────────────────
for f in "${GEN_CPP}" "${GEN_H}" "${XML_FILE}"; do
    if [[ ! -f "$f" ]]; then
        echo "ERROR: Missing generated file: $(basename "$f")"
        echo "       Run with --generate <dbc_path> to produce it first."
        exit 1
    fi
done

# ── Step 2 & 3: CMake configure, compile, package ────────────────────────────
echo ""
echo "════════════════════════════════════════════════════════"
echo "  CMake configure  →  ${CMAKE_BUILD_DIR}  [${BUILD_TYPE}]"
echo "════════════════════════════════════════════════════════"
cmake -S "${CODE_DIR}" -B "${CMAKE_BUILD_DIR}" -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

echo ""
echo "════════════════════════════════════════════════════════"
echo "  CMake build (compile + package FMU)  [${BUILD_TYPE}]"
echo "════════════════════════════════════════════════════════"
cmake --build "${CMAKE_BUILD_DIR}" --config "${BUILD_TYPE}"

SIZE_KB=$(du -k "${FMU_FILE}" | cut -f1)
echo "  Packaged OK  —  ${FMU_FILE}  (${SIZE_KB} KB)"

# ── Optional: Validate ────────────────────────────────────────────────────────
if [[ $DO_VALIDATE -eq 1 ]]; then
    echo ""
    echo "════════════════════════════════════════════════════════"
    echo "  Validation"
    echo "════════════════════════════════════════════════════════"
    python3 validate_lrcf_fmu.py "${FMU_FILE}"
fi

echo ""
echo "✅  Build complete  →  ${FMU_FILE}"
