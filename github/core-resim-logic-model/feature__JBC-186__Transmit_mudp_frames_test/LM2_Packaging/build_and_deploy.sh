#!/usr/bin/env bash
# ---------------------------------------------------------------------------
# build_and_deploy.sh
# Builds an LM2 FMU for a given customer / build type and deploys it into
# the VV SIL Engine project (pre-extracts for CloudMode = true).
#
# Usage:
#   ./build_and_deploy.sh [OPTIONS]
#
# Options:
#   -c, --customer    Customer name   (default: STLA_SMALL_IFV600)
#   -b, --build-type  Release|Debug   (default: Release)
#   -d, --deploy-dir  Absolute path to the VV Engine root
#                     (default: auto-detected sibling Core_RESIM_VV_Engine)
#   -s, --sil-lib-dir Path to directory containing the real SIL .so files
#                     (e.g. /path/to/ADCAM_SIL_ENGINE_DBG_TEST_PKG_v2p1_20260609)
#                     Also readable from env var SIL_LIB_DIR.
#                     If supplied, its .so files overwrite those from FW_LIME/RADAR_DLLS.
#   -e, --emblib-artifact-dir
#                     Path containing emb_sil artifacts (`.so` and `.yaml`).
#                     (default: /home/vish225/projects/ifv600_repo/10055633_STLASmallMY26_ifv6xxcameraprj/Appl/emb_sil/build/bin/Debug)
#   --sanitize        Enable AddressSanitizer and LeakSanitizer in the Linux FMU
#   -h, --help        Show this help and exit
#
# Example:
#   ./build_and_deploy.sh --customer STLA_SMALL_IFV600 --build-type Debug \
#     --sil-lib-dir /path/to/ADCAM_SIL_ENGINE_DBG_TEST_PKG_v2p1_20260609
# ---------------------------------------------------------------------------
set -euo pipefail

# ---------- colour helpers --------------------------------------------------
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; NC='\033[0m'
info()  { echo -e "${GREEN}[INFO]${NC}  $*"; }
warn()  { echo -e "${YELLOW}[WARN]${NC}  $*"; }
error() { echo -e "${RED}[ERROR]${NC} $*" >&2; exit 1; }

# ---------- locate this script ----------------------------------------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FMU_DIR="${SCRIPT_DIR}/LM2_FMU"
DELIVERABLES_DIR="${SCRIPT_DIR}/Deliverables"

# ---------- defaults --------------------------------------------------------
CUSTOMER="STLA_SMALL_IFV600"
BUILD_TYPE="Release"
VV_ENGINE_DIR="$(cd "${SCRIPT_DIR}/../../Core_RESIM_VV_Engine" 2>/dev/null && pwd || true)"
SIL_LIB_DIR="${SIL_LIB_DIR:-}"   # may also be set via env var
EMBLIB_ARTIFACT_DIR="${EMBLIB_ARTIFACT_DIR:-${FMU_DIR}/Data/Aptiv_Libraries/IFV600_emblib}"
SANITIZE=0

# ---------- customer → Build.py menu choice ---------------------------------
# Keep in sync with Build.py handle_customer_selection()
declare -A CUSTOMER_CHOICE=( [CEER]="1" [GPO_Gen8]="2" [STLA_SMALL_IFV600]="3" )
declare -A LM2_SUBDIR=(      [CEER]="CEER" [GPO_Gen8]="CEER" [STLA_SMALL_IFV600]="STLA_Small" )
declare -A LM2_ID=(          [CEER]="2"    [GPO_Gen8]="2"    [STLA_SMALL_IFV600]="5" )

# ---------- parse args ------------------------------------------------------
while [[ $# -gt 0 ]]; do
    case "$1" in
        -c|--customer)    CUSTOMER="$2";      shift 2 ;;
        -b|--build-type)  BUILD_TYPE="$2";    shift 2 ;;
        -d|--deploy-dir)  VV_ENGINE_DIR="$2"; shift 2 ;;
        -s|--sil-lib-dir) SIL_LIB_DIR="$2";  shift 2 ;;
        -e|--emblib-artifact-dir) EMBLIB_ARTIFACT_DIR="$2"; shift 2 ;;
        --sanitize)        SANITIZE=1;       shift ;;
        -h|--help)
            sed -n '/^# Usage/,/^# -----/p' "$0" | grep -v '^# ---' | sed 's/^# //'
            exit 0 ;;
        *) error "Unknown argument: $1" ;;
    esac
done

# ---------- validate inputs -------------------------------------------------
[[ -v CUSTOMER_CHOICE[$CUSTOMER] ]] || \
    error "Unknown customer '${CUSTOMER}'. Valid: ${!CUSTOMER_CHOICE[*]}"

[[ "$BUILD_TYPE" == "Release" || "$BUILD_TYPE" == "Debug" ]] || \
    error "Build type must be 'Release' or 'Debug'"

[[ -d "$VV_ENGINE_DIR" ]] || \
    error "VV Engine directory not found: ${VV_ENGINE_DIR}"

compgen -G "${EMBLIB_ARTIFACT_DIR}/Linux/*.so" > /dev/null || \
    error "No emb_sil shared libraries found in: ${EMBLIB_ARTIFACT_DIR}"

compgen -G "${EMBLIB_ARTIFACT_DIR}/Linux/*.yaml" > /dev/null || \
    error "No emb_sil YAML files found in: ${EMBLIB_ARTIFACT_DIR}"

CUST_MENU="${CUSTOMER_CHOICE[$CUSTOMER]}"
TYPE_MENU="$( [[ "$BUILD_TYPE" == "Release" ]] && echo "1" || echo "2" )"
LM2_SUB="${LM2_SUBDIR[$CUSTOMER]}"
LM2_LOGIC_ID="${LM2_ID[$CUSTOMER]}"

FMU_NAME="SRR_Master_LogicModel2_${CUSTOMER}.fmu"
FMU_PATH="${DELIVERABLES_DIR}/${FMU_NAME}"

VV_LM2_DIR="${VV_ENGINE_DIR}/03_LOGIC_MODEL/${LM2_SUB}"
VV_EXTRACT_DIR="${VV_LM2_DIR}/LogicModelId${LM2_LOGIC_ID}"

info "===== LM2 FMU Build & Deploy ====="
info "Customer   : ${CUSTOMER} (menu choice ${CUST_MENU})"
info "Build type : ${BUILD_TYPE} (menu choice ${TYPE_MENU})"
info "VV Engine  : ${VV_ENGINE_DIR}"
info "emb_sil artifacts: ${EMBLIB_ARTIFACT_DIR}"
[[ -n "${SIL_LIB_DIR}" ]] && info "SIL lib dir: ${SIL_LIB_DIR}" || info "SIL lib dir: (default FW_LIME/RADAR_DLLS)"
[[ -z "${SIL_LIB_DIR}" || -d "${SIL_LIB_DIR}" ]] || error "SIL library directory not found: ${SIL_LIB_DIR}"
echo ""

# ---------- 1. build via Build.py (non-interactive) ------------------------
# Drive Build.py with piped stdin to guarantee identical execution path to
# the interactive run (same cwd management, cmake invocation, and packaging).
info "Step 1/3 – Building FMU via Build.py"
cd "${FMU_DIR}"
export SIL_LIB_DIR EMBLIB_ARTIFACT_DIR
BUILD_ARGS=()
[[ "${SANITIZE}" -eq 1 ]] && BUILD_ARGS+=(--sanitize)
printf '%s\n%s\n' "${CUST_MENU}" "${TYPE_MENU}" | python3 Build.py "${BUILD_ARGS[@]}"
echo ""

# ---------- 2. verify deliverable ------------------------------------------
info "Step 2/3 – Verifying deliverable"
[[ -f "$FMU_PATH" ]] || error "Expected FMU not found: ${FMU_PATH}"

DEBUG_SECTIONS=$(unzip -p "$FMU_PATH" "binaries/linux64/SRR_Master_LogicModel2.so" 2>/dev/null \
    | readelf -S - 2>/dev/null | grep -c "debug" || true)
DEBUG_SECTIONS="${DEBUG_SECTIONS//[^0-9]/}"   # strip whitespace/newlines
DEBUG_SECTIONS="${DEBUG_SECTIONS:-0}"

info "FMU        : ${FMU_PATH} ($(du -sh "$FMU_PATH" | cut -f1))"
info "Debug info : ${DEBUG_SECTIONS} DWARF section(s) in SRR_Master_LogicModel2.so"
if [[ "$BUILD_TYPE" == "Debug" && "${DEBUG_SECTIONS}" -eq 0 ]]; then
    warn "No debug sections found – FMU may have been built as Release"
fi
echo ""

# ---------- 3. deploy into VV Engine ----------------------------------------
info "Step 3/3 – Deploying to VV Engine"

mkdir -p "${VV_LM2_DIR}"
cp "${FMU_PATH}" "${VV_LM2_DIR}/"
info "Copied     : ${VV_LM2_DIR}/${FMU_NAME}"

# Re-extract for CloudMode=true (engine does not unzip when CloudMode=true)
info "Extracting : ${VV_EXTRACT_DIR}/"
rm -rf "${VV_EXTRACT_DIR}"
mkdir -p "${VV_EXTRACT_DIR}"
unzip -o "${VV_LM2_DIR}/${FMU_NAME}" -d "${VV_EXTRACT_DIR}" > /dev/null

[[ -f "${VV_EXTRACT_DIR}/modelDescription.xml" ]] || \
    error "Extraction failed – modelDescription.xml missing in ${VV_EXTRACT_DIR}"

echo ""
info "===== Done ====="
info "FMU (${BUILD_TYPE})  : ${VV_LM2_DIR}/${FMU_NAME}"
info "Extracted            : ${VV_EXTRACT_DIR}/"
info "Plugin (.so)         : ${VV_EXTRACT_DIR}/binaries/linux64/SRR_Master_LogicModel2.so"
