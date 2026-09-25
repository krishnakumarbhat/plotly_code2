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
  run_udp.sh <kpi.json> [output_dir]
  run_udp.sh --hdf <input.h5> <output.h5> [output_dir]
  run_udp.sh zmq [port]
EOF
}

IMAGE_PATH="$SCRIPT_DIR/udp_kpi.simg"
bundle_ensure_file "$IMAGE_PATH"

case "${1:-}" in
    -h|--help|'')
        usage
        exit 0
        ;;
    zmq)
        PORT="${2:-5560}"
        echo "[run_udp] zmq_port=$PORT"
        bundle_run_image "$IMAGE_PATH" zmq "$PORT"
        ;;
    --hdf)
        INPUT_HDF="$(bundle_abs_path "$2")"
        OUTPUT_HDF="$(bundle_abs_path "$3")"
        OUTPUT_DIR="${4:-}"
        if [[ -z "$OUTPUT_DIR" ]]; then
            OUTPUT_DIR="$(bundle_default_output_dir "$BUNDLE_ROOT" udp_kpi)"
        else
            OUTPUT_DIR="$(bundle_abs_path "$OUTPUT_DIR")"
            mkdir -p "$OUTPUT_DIR"
        fi
        bundle_ensure_file "$INPUT_HDF"
        bundle_ensure_file "$OUTPUT_HDF"
        echo "[run_udp] input=$INPUT_HDF"
        echo "[run_udp] output=$OUTPUT_HDF"
        echo "[run_udp] html_dir=$OUTPUT_DIR"
        bundle_run_image "$IMAGE_PATH" hdf "$INPUT_HDF" "$OUTPUT_HDF" "$OUTPUT_DIR"
        ;;
    *)
        INPUT_JSON="$(bundle_abs_path "$1")"
        OUTPUT_DIR="${2:-}"
        if [[ -z "$OUTPUT_DIR" ]]; then
            OUTPUT_DIR="$(bundle_default_output_dir "$BUNDLE_ROOT" udp_kpi)"
        else
            OUTPUT_DIR="$(bundle_abs_path "$OUTPUT_DIR")"
            mkdir -p "$OUTPUT_DIR"
        fi
        bundle_ensure_file "$INPUT_JSON"
        echo "[run_udp] json=$INPUT_JSON"
        echo "[run_udp] html_dir=$OUTPUT_DIR"
        bundle_run_image "$IMAGE_PATH" json "$INPUT_JSON" "$OUTPUT_DIR"
        ;;
esac