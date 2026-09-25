#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export BUNDLE_ROOT="${HPCC_BUNDLE_ROOT:-$(cd "$SCRIPT_DIR/../.." && pwd)}"
export HPCC_PROJECT_ROOT="${HPCC_PROJECT_ROOT:-$BUNDLE_ROOT/bundle_src}"
# shellcheck disable=SC1091
source "$BUNDLE_ROOT/bundle_common.sh"

usage() {
    cat <<'EOF'
Usage:
  run_can.sh <kpi.json> [output_dir]
  run_can.sh <config.xml> <kpi.json> [output_dir]
  run_can.sh --hdf <input.h5> <output.h5> [output_dir]

The XML argument is accepted for compatibility with older notes but is not used
by CAN KPI direct execution.
EOF
}

IMAGE_PATH="$SCRIPT_DIR/can_kpi.simg"
bundle_ensure_file "$IMAGE_PATH"

MODE='json'
INPUT_JSON=''
INPUT_HDF=''
OUTPUT_HDF=''
OUTPUT_DIR=''

case "${1:-}" in
    -h|--help|'')
        usage
        exit 0
        ;;
    --hdf)
        MODE='hdf'
        INPUT_HDF="$(bundle_abs_path "$2")"
        OUTPUT_HDF="$(bundle_abs_path "$3")"
        OUTPUT_DIR="${4:-}"
        ;;
    *)
        if [[ $# -ge 2 && "$1" == *.xml ]]; then
            bundle_ensure_file "$(bundle_abs_path "$1")"
            INPUT_JSON="$(bundle_abs_path "$2")"
            OUTPUT_DIR="${3:-}"
        else
            INPUT_JSON="$(bundle_abs_path "$1")"
            OUTPUT_DIR="${2:-}"
        fi
        ;;
esac

if [[ -z "$OUTPUT_DIR" ]]; then
    OUTPUT_DIR="$(bundle_default_output_dir "$BUNDLE_ROOT" can_kpi)"
else
    OUTPUT_DIR="$(bundle_abs_path "$OUTPUT_DIR")"
    mkdir -p "$OUTPUT_DIR"
fi

if [[ "$MODE" == 'hdf' ]]; then
    bundle_ensure_file "$INPUT_HDF"
    bundle_ensure_file "$OUTPUT_HDF"
    echo "[run_can] input=$INPUT_HDF"
    echo "[run_can] output=$OUTPUT_HDF"
    echo "[run_can] html_dir=$OUTPUT_DIR"
    bundle_run_image "$IMAGE_PATH" hdf "$INPUT_HDF" "$OUTPUT_HDF" "$OUTPUT_DIR"
    exit $?
fi

bundle_ensure_file "$INPUT_JSON"
echo "[run_can] json=$INPUT_JSON"
echo "[run_can] html_dir=$OUTPUT_DIR"
bundle_run_image "$IMAGE_PATH" json "$INPUT_JSON" "$OUTPUT_DIR"