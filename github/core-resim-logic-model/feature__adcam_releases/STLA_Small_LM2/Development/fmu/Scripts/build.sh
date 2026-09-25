#!/usr/bin/env bash
# =============================================================================
#  build.sh  —  Compile and package LogicModel2.fmu  (Linux)
# =============================================================================
#  Usage:
#    ./build.sh                          # compile + package (generated files must exist)
#    ./build.sh --generate <dbc_path>    # generate from DBC, then compile + package
#    ./build.sh --node <name>            # DBC node name (default: LRCF, only with --generate)
#    ./build.sh --validate               # also run validate_lrcf_fmu.py after build
#    ./build.sh --clean                  # remove build outputs (not generated sources)
#    ./build.sh --clean --generate <..>  # remove everything, regenerate and build
#
#  Generated files (generated_signal_map.cpp, generated_structs.h,
#  modelDescription.xml) are NOT regenerated automatically on every build.
#  Run with --generate only when the DBC changes.
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

while [[ $# -gt 0 ]]; do
    case $1 in
        --generate) DO_GENERATE=1; DBC_FILE="$2"; shift 2 ;;
        --node)     FMU_NODE="$2"; shift 2 ;;
        --validate) DO_VALIDATE=1; shift ;;
        --clean)    DO_CLEAN=1;    shift ;;
        *)          echo "Unknown option: $1"; exit 1 ;;
    esac
done

FMU_NAME="LogicModel2"
SO_FILE="${BUILD_DIR}/${FMU_NAME}.so"
FMU_FILE="${BUILD_DIR}/${FMU_NAME}.fmu"
XML_FILE="${CODE_DIR}/modelDescription.xml"
GEN_CPP="${CODE_DIR}/generated_signal_map.cpp"
GEN_H="${CODE_DIR}/generated_structs.h"

# ── Clean ─────────────────────────────────────────────────────────────────────
if [[ $DO_CLEAN -eq 1 ]]; then
    echo "[clean] removing build outputs…"
    rm -f "${SO_FILE}" "${FMU_FILE}"
    if [[ $DO_GENERATE -eq 1 ]]; then
        echo "[clean] removing generated sources…"
        rm -f "${GEN_CPP}" "${GEN_H}" "${XML_FILE}"
    fi
    rm -rf staging/
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

# ── Step 2: Compile shared library ───────────────────────────────────────────
echo ""
echo "════════════════════════════════════════════════════════"
echo "  Compile  →  ${SO_FILE}"
echo "════════════════════════════════════════════════════════"
g++ -std=c++14 -fPIC -shared \
    -Wall -Wno-unused-parameter \
    -O2 \
    -I"${CODE_DIR}" \
    -o "${SO_FILE}" \
    "${GEN_CPP}" \
    "${CODE_DIR}/LogicModel2.cpp"

EXPORT_COUNT=$(nm -D "${SO_FILE}" | grep -c "fmi2")
echo "  Compiled OK  —  ${EXPORT_COUNT} fmi2* symbols exported"

# ── Step 3: Package FMU zip ───────────────────────────────────────────────────
echo ""
echo "════════════════════════════════════════════════════════"
echo "  Package  →  ${FMU_FILE}"
echo "════════════════════════════════════════════════════════"
rm -f "${FMU_FILE}"
mkdir -p staging/binaries/linux64

cp "${XML_FILE}" staging/modelDescription.xml
cp "${SO_FILE}"  staging/binaries/linux64/

cd staging
zip -r "${FMU_FILE}" . -q
cd "${SCRIPT_DIR}"
rm -rf staging/

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
