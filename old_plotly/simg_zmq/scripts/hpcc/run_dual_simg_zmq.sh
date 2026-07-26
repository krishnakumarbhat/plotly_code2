#!/usr/bin/env bash
set -euo pipefail

# Run both SIMG containers on one node:
# 1) UDP_KPI SIMG as ZMQ server
# 2) InteractivePlot SIMG as client

usage() {
  cat <<'EOF'
Usage:
  run_dual_simg_zmq.sh \
    --interactive-simg /path/interactiveplot.simg \
    --kpi-simg /path/kpi.simg \
    --config-xml /path/ConfigInteractivePlots.xml \
    --input-json /path/InputsInteractivePlot.json \
    --int-output /path/output_dir \
    [--port 5560] \
    [--log-dir /path/logs] \
    [--bind /path1:/path1] [--bind /path2:/path2] ...

Notes:
- Keep both containers on the same node because ZMQ uses localhost.
- In this connected mode, KPI input is received from InteractivePlot JSON paths.
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

require_dir() {
  local p="$1"
  local name="$2"
  if [[ ! -d "$p" ]]; then
    echo "ERROR: $name not found: $p" >&2
    exit 1
  fi
}

INTERACTIVE_SIMG=""
KPI_SIMG=""
CONFIG_XML=""
INPUT_JSON=""
INT_OUTPUT=""
PORT="5560"
LOG_DIR=""

BIND_OPTS=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    --interactive-simg) INTERACTIVE_SIMG="$2"; shift 2 ;;
    --kpi-simg) KPI_SIMG="$2"; shift 2 ;;
    --config-xml) CONFIG_XML="$2"; shift 2 ;;
    --input-json) INPUT_JSON="$2"; shift 2 ;;
    --int-output) INT_OUTPUT="$2"; shift 2 ;;
    --port) PORT="$2"; shift 2 ;;
    --log-dir) LOG_DIR="$2"; shift 2 ;;
    --bind) BIND_OPTS+=("-B" "$2"); shift 2 ;;
    -h|--help) usage; exit 0 ;;
    *)
      echo "ERROR: Unknown argument: $1" >&2
      usage
      exit 1
      ;;
  esac
done

if [[ -z "$INTERACTIVE_SIMG" || -z "$KPI_SIMG" || -z "$CONFIG_XML" || -z "$INPUT_JSON" || -z "$INT_OUTPUT" ]]; then
  usage
  exit 1
fi

require_file "$INTERACTIVE_SIMG" "InteractivePlot SIMG"
require_file "$KPI_SIMG" "KPI SIMG"
require_file "$CONFIG_XML" "Config XML"
require_file "$INPUT_JSON" "Input JSON"

if command -v python3 >/dev/null 2>&1; then
  echo "[precheck] Validating INPUT_HDF/OUTPUT_HDF paths in $INPUT_JSON"
  python3 - "$INPUT_JSON" <<'PY'
import json
import os
import sys

cfg = sys.argv[1]
with open(cfg, "r", encoding="utf-8") as fp:
  data = json.load(fp)

inputs_raw = data.get("INPUT_HDF") or []
outputs_raw = data.get("OUTPUT_HDF") or []
if not isinstance(inputs_raw, list) or not isinstance(outputs_raw, list):
  print("ERROR: INPUT_HDF and OUTPUT_HDF must be JSON arrays", file=sys.stderr)
  sys.exit(2)

if len(inputs_raw) != len(outputs_raw):
  print(
    f"ERROR: INPUT_HDF/OUTPUT_HDF length mismatch: {len(inputs_raw)} != {len(outputs_raw)}",
    file=sys.stderr,
  )
  sys.exit(2)

dup_count = len(inputs_raw) - len(set([p for p in inputs_raw if isinstance(p, str)]))
print(f"[precheck] Pair count in JSON: {len(inputs_raw)}")
print(f"[precheck] Duplicate INPUT_HDF entries: {dup_count}")

missing = []
for key in ("INPUT_HDF", "OUTPUT_HDF"):
  values = data.get(key) or []
  if not isinstance(values, list):
    missing.append((key, "<not-a-list>"))
    continue
  for p in values:
    if not isinstance(p, str) or not p.strip():
      missing.append((key, str(p)))
      continue
    if not os.path.exists(p):
      missing.append((key, p))

if missing:
  print("ERROR: Some HDF paths from JSON do not exist on this node:", file=sys.stderr)
  for k, p in missing:
    print(f"  - {k}: {p}", file=sys.stderr)
  sys.exit(2)

print("[precheck] JSON HDF paths look valid")
PY
else
  echo "[precheck] WARNING: python3 not found; skipping JSON HDF path validation"
fi

mkdir -p "$INT_OUTPUT"
if [[ -z "$LOG_DIR" ]]; then
  LOG_DIR="$INT_OUTPUT/simg_logs"
fi
mkdir -p "$LOG_DIR"

umask 022

if command -v apptainer >/dev/null 2>&1; then
  CTR_CMD="apptainer"
elif command -v singularity >/dev/null 2>&1; then
  CTR_CMD="singularity"
else
  echo "ERROR: Neither apptainer nor singularity is available." >&2
  exit 1
fi

KPI_LOG="$LOG_DIR/kpi_server.log"
INT_LOG="$LOG_DIR/interactiveplot.log"

KPI_PID=""
cleanup() {
  if [[ -n "$KPI_PID" ]] && kill -0 "$KPI_PID" >/dev/null 2>&1; then
    kill "$KPI_PID" >/dev/null 2>&1 || true
    wait "$KPI_PID" >/dev/null 2>&1 || true
  fi
}
trap cleanup EXIT

echo "[1/3] Starting KPI ZMQ server SIMG on port $PORT"
"$CTR_CMD" run "${BIND_OPTS[@]}" "$KPI_SIMG" zmq "$PORT" >"$KPI_LOG" 2>&1 &
KPI_PID=$!

# Wait until server says it started.
READY=0
for _ in $(seq 1 60); do
  if ! kill -0 "$KPI_PID" >/dev/null 2>&1; then
    echo "ERROR: KPI server exited early. Check $KPI_LOG" >&2
    tail -n 80 "$KPI_LOG" >&2 || true
    exit 1
  fi
  if grep -q "KPI ZMQ server started on port" "$KPI_LOG"; then
    READY=1
    break
  fi
  sleep 1
done

if [[ "$READY" -ne 1 ]]; then
  echo "ERROR: KPI server did not become ready in time. Check $KPI_LOG" >&2
  tail -n 80 "$KPI_LOG" >&2 || true
  exit 1
fi

echo "[2/3] Running InteractivePlot SIMG with ZMQ target 127.0.0.1:$PORT"
"$CTR_CMD" run "${BIND_OPTS[@]}" \
  --env "KPI_SERVER_HOST=127.0.0.1,KPI_SERVER_PORT=$PORT" \
  "$INTERACTIVE_SIMG" "$CONFIG_XML" "$INPUT_JSON" "$INT_OUTPUT" >"$INT_LOG" 2>&1

if ! grep -q "Report Generation Completed" "$INT_LOG"; then
  echo "ERROR: InteractivePlot did not report completion. Check $INT_LOG" >&2
  tail -n 120 "$INT_LOG" >&2 || true
  exit 1
fi

if grep -E -q "Input HDF not found|Output HDF not found|JSON configuration must contain|Failed to analyze HDF pair|Config file not found|JSON file not found|Traceback" "$INT_LOG"; then
  echo "ERROR: Interactive log contains fatal path/config errors. Check $INT_LOG" >&2
  tail -n 120 "$INT_LOG" >&2 || true
  exit 1
fi

MASTER_INDEX="$INT_OUTPUT/master_index.html"
if [[ ! -f "$MASTER_INDEX" ]]; then
  echo "ERROR: Expected output file not found: $MASTER_INDEX" >&2
  tail -n 120 "$INT_LOG" >&2 || true
  exit 1
fi

HTML_COUNT=$(find "$INT_OUTPUT" -type f -name "*.html" | wc -l | tr -d ' ')
if [[ "${HTML_COUNT:-0}" -lt 2 ]]; then
  echo "ERROR: HTML output seems empty (count=$HTML_COUNT). Check $INT_LOG" >&2
  tail -n 120 "$INT_LOG" >&2 || true
  exit 1
fi

if command -v python3 >/dev/null 2>&1; then
  JSON_PAIR_COUNT=$(python3 - "$INPUT_JSON" <<'PY'
import json
import sys
with open(sys.argv[1], "r", encoding="utf-8") as fp:
  d = json.load(fp)
ins = d.get("INPUT_HDF") if isinstance(d, dict) else []
outs = d.get("OUTPUT_HDF") if isinstance(d, dict) else []
if not isinstance(ins, list) or not isinstance(outs, list):
  print(0)
else:
  print(min(len(ins), len(outs)))
PY
)
  BASE_DIR_COUNT=$(find "$INT_OUTPUT" -mindepth 1 -maxdepth 1 -type d ! -name "simg_logs" | wc -l | tr -d ' ')
  if [[ "${BASE_DIR_COUNT:-0}" -lt "${JSON_PAIR_COUNT:-0}" ]]; then
    echo "ERROR: Generated base folders ($BASE_DIR_COUNT) are fewer than JSON pairs ($JSON_PAIR_COUNT)." >&2
    echo "This usually means filename collisions (overwrites) or skipped pairs. Check $INT_LOG" >&2
    tail -n 120 "$INT_LOG" >&2 || true
    exit 1
  fi
fi

echo "[3/3] Completed"
echo "Interactive output: $INT_OUTPUT"
echo "KPI log:            $KPI_LOG"
echo "Interactive log:    $INT_LOG"
echo "Master index:       $MASTER_INDEX"
echo "HTML count:         $HTML_COUNT"
