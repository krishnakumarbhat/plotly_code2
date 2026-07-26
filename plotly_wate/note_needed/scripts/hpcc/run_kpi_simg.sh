#!/usr/bin/env bash
set -euo pipefail

# Run UDP_KPI SIMG in one of three modes: zmq, json, hdf.

usage() {
  cat <<'EOF'
Usage:
  # ZMQ server mode (for InteractivePlot integration)
  run_kpi_simg.sh --simg /path/kpi.simg --mode zmq [--port 5560] [--bind /path:/path]...

  # JSON batch mode (standalone KPI processing)
  run_kpi_simg.sh --simg /path/kpi.simg --mode json \
    --kpi-json /path/kpi_or_interactive_json \
    --output-dir /path/kpi_output \
    [--bind /path:/path]...

  # HDF pair mode (standalone KPI processing)
  run_kpi_simg.sh --simg /path/kpi.simg --mode hdf \
    --input-hdf /path/input.h5 --output-hdf /path/output.h5 \
    --output-dir /path/kpi_output \
    [--bind /path:/path]...
EOF
}

require_file() {
  local p="$1"
  local name="$2"
  if [[ ! -f "$p" ]]; then
    echo "ERROR: $name not found: $p" >&2
    exit 1
  fi
}

SIMG=""
MODE=""
PORT="5560"
KPI_JSON=""
INPUT_HDF=""
OUTPUT_HDF=""
OUTPUT_DIR=""
BIND_OPTS=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    --simg) SIMG="$2"; shift 2 ;;
    --mode) MODE="$2"; shift 2 ;;
    --port) PORT="$2"; shift 2 ;;
    --kpi-json) KPI_JSON="$2"; shift 2 ;;
    --input-hdf) INPUT_HDF="$2"; shift 2 ;;
    --output-hdf) OUTPUT_HDF="$2"; shift 2 ;;
    --output-dir) OUTPUT_DIR="$2"; shift 2 ;;
    --bind) BIND_OPTS+=("-B" "$2"); shift 2 ;;
    -h|--help) usage; exit 0 ;;
    *)
      echo "ERROR: Unknown argument: $1" >&2
      usage
      exit 1
      ;;
  esac
done

if [[ -z "$SIMG" || -z "$MODE" ]]; then
  usage
  exit 1
fi

require_file "$SIMG" "KPI SIMG"
umask 022

if command -v apptainer >/dev/null 2>&1; then
  CTR_CMD="apptainer"
elif command -v singularity >/dev/null 2>&1; then
  CTR_CMD="singularity"
else
  echo "ERROR: Neither apptainer nor singularity is available." >&2
  exit 1
fi

case "$MODE" in
  zmq)
    exec "$CTR_CMD" run "${BIND_OPTS[@]}" "$SIMG" zmq "$PORT"
    ;;
  json)
    if [[ -z "$KPI_JSON" || -z "$OUTPUT_DIR" ]]; then
      echo "ERROR: --kpi-json and --output-dir are required for mode=json" >&2
      exit 1
    fi
    require_file "$KPI_JSON" "KPI JSON"
    mkdir -p "$OUTPUT_DIR"
    exec "$CTR_CMD" run "${BIND_OPTS[@]}" "$SIMG" json "$KPI_JSON" "$OUTPUT_DIR"
    ;;
  hdf)
    if [[ -z "$INPUT_HDF" || -z "$OUTPUT_HDF" || -z "$OUTPUT_DIR" ]]; then
      echo "ERROR: --input-hdf --output-hdf --output-dir are required for mode=hdf" >&2
      exit 1
    fi
    require_file "$INPUT_HDF" "Input HDF"
    require_file "$OUTPUT_HDF" "Output HDF"
    mkdir -p "$OUTPUT_DIR"
    exec "$CTR_CMD" run "${BIND_OPTS[@]}" "$SIMG" hdf "$INPUT_HDF" "$OUTPUT_HDF" "$OUTPUT_DIR"
    ;;
  *)
    echo "ERROR: Invalid mode: $MODE (expected: zmq|json|hdf)" >&2
    exit 1
    ;;
esac
