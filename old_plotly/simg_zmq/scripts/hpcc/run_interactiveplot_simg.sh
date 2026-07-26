#!/usr/bin/env bash
set -euo pipefail

# Run InteractivePlot SIMG and optionally point it to a KPI ZMQ server.

usage() {
  cat <<'EOF'
Usage:
  run_interactiveplot_simg.sh \
    --simg /path/interactiveplot.simg \
    --config-xml /path/ConfigInteractivePlots.xml \
    --input-json /path/InputsInteractivePlot.json \
    --output-dir /path/int_output \
    [--kpi-host 127.0.0.1] [--kpi-port 5560] \
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
CONFIG_XML=""
INPUT_JSON=""
OUTPUT_DIR=""
KPI_HOST="127.0.0.1"
KPI_PORT="5560"
BIND_OPTS=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    --simg) SIMG="$2"; shift 2 ;;
    --config-xml) CONFIG_XML="$2"; shift 2 ;;
    --input-json) INPUT_JSON="$2"; shift 2 ;;
    --output-dir) OUTPUT_DIR="$2"; shift 2 ;;
    --kpi-host) KPI_HOST="$2"; shift 2 ;;
    --kpi-port) KPI_PORT="$2"; shift 2 ;;
    --bind) BIND_OPTS+=("-B" "$2"); shift 2 ;;
    -h|--help) usage; exit 0 ;;
    *)
      echo "ERROR: Unknown argument: $1" >&2
      usage
      exit 1
      ;;
  esac
done

if [[ -z "$SIMG" || -z "$CONFIG_XML" || -z "$INPUT_JSON" || -z "$OUTPUT_DIR" ]]; then
  usage
  exit 1
fi

require_file "$SIMG" "InteractivePlot SIMG"
require_file "$CONFIG_XML" "Config XML"
require_file "$INPUT_JSON" "Input JSON"

mkdir -p "$OUTPUT_DIR"
umask 022

if command -v apptainer >/dev/null 2>&1; then
  CTR_CMD="apptainer"
elif command -v singularity >/dev/null 2>&1; then
  CTR_CMD="singularity"
else
  echo "ERROR: Neither apptainer nor singularity is available." >&2
  exit 1
fi

exec "$CTR_CMD" run "${BIND_OPTS[@]}" \
  --env "KPI_SERVER_HOST=$KPI_HOST,KPI_SERVER_PORT=$KPI_PORT" \
  "$SIMG" "$CONFIG_XML" "$INPUT_JSON" "$OUTPUT_DIR"
