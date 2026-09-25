#!/usr/bin/env bash
# Combined launcher: replaces main_hpcc.sh + run_hpcc_stack.sh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_DIR="$SCRIPT_DIR/store/logs"
RUNTIME_STATE_DIR="$SCRIPT_DIR/store/db"
MAIN_HTML_CACHE_DIR="$RUNTIME_STATE_DIR/.cache_html"

mkdir -p "$LOG_DIR" "$MAIN_HTML_CACHE_DIR/html" "$MAIN_HTML_CACHE_DIR/video"
mkdir -p "$MAIN_HTML_CACHE_DIR/vlm_cache" "$MAIN_HTML_CACHE_DIR/chromadb_data"

[ -f "$SCRIPT_DIR/bundle_common.sh" ] && source "$SCRIPT_DIR/bundle_common.sh"

export HPCC_BUNDLE_ROOT="$SCRIPT_DIR"
export HPCC_PROJECT_ROOT="${HPCC_PROJECT_ROOT:-$SCRIPT_DIR/bundle_src}"
export HPCC_BROKER_HOST="${HPCC_BROKER_HOST:-0.0.0.0}"
export HPCC_BROKER_PORT="${HPCC_BROKER_PORT:-9100}"
export PORT="${PORT:-5002}"
export WORKERS="${WORKERS:-3}"
export THREADS="${THREADS:-8}"
export TIMEOUT="${TIMEOUT:-240}"
export RAG_PORT="${RAG_PORT:-5100}"
export JIRA_PORT="${JIRA_PORT:-5200}"
export HPCC_AUTO_START_RAG="${HPCC_AUTO_START_RAG:-1}"
export HPCC_PORT_CONFLICT_POLICY="${HPCC_PORT_CONFLICT_POLICY:-shift}"
export HOST_SIMG_PATH="$SCRIPT_DIR"
export HPCC_REQUIRE_SLURM_FOR_KPI="${HPCC_REQUIRE_SLURM_FOR_KPI:-1}"
export HPCC_ALLOW_LOCAL_KPI="${HPCC_ALLOW_LOCAL_KPI:-0}"

# Log rotation
rotate_log() { local f="$1"; [ -f "$f" ] && mv "$f" "${f}.$(date +%Y%m%d_%H%M%S).prev" 2>/dev/null || true; }
rotate_log "$LOG_DIR/broker.log"
rotate_log "$LOG_DIR/main_html.log"
rotate_log "$LOG_DIR/rag.log"
rotate_log "$LOG_DIR/jira.log"

# Runtime DB
if [ -z "${HPCC_RUNTIME_DB:-}" ]; then
    if [ -d /net/8k3 ] || [ -d /mnt/usmidet ]; then
        runtime_db_user="$(id -un 2>/dev/null || echo hpcc)"
        export HPCC_RUNTIME_DB="/tmp/hpcc_runtime_db_${runtime_db_user}/hpc_tools_dev.db"
    else
        export HPCC_RUNTIME_DB="$MAIN_HTML_CACHE_DIR/hpc_tools_dev.db"
    fi
fi

# Broker
BROKER_CMD=()
if [ -f "$SCRIPT_DIR/hpcc_main.pyz" ]; then
    BROKER_CMD=(python3 "$SCRIPT_DIR/hpcc_main.pyz")
elif [ -f "$SCRIPT_DIR/hpcc_main.py" ]; then
    BROKER_CMD=(python3 "$SCRIPT_DIR/hpcc_main.py")
elif [ -f "$SCRIPT_DIR/../hpcc_main.py" ]; then
    BROKER_CMD=(python3 "$SCRIPT_DIR/../hpcc_main.py")
else
    echo "Missing hpcc_main.py or hpcc_main.pyz" >&2; exit 1
fi

export PYTHONPATH="${PYTHONPATH:+$PYTHONPATH:}$SCRIPT_DIR:$SCRIPT_DIR/bundle_src"

# Model dir
QWEN_MODEL_DIR="${QWEN_MODEL_DIR:-}"
[ -z "$QWEN_MODEL_DIR" ] && QWEN_MODEL_DIR="$(find "$SCRIPT_DIR/bundle_src/rag" -name '*.gguf' -exec dirname {} \; 2>/dev/null | head -1)" || true

# Bind mounts
bind_args=()
for d in "$SCRIPT_DIR" "$HPCC_PROJECT_ROOT" /net /scratch /mnt; do
    [ -d "$d" ] && bind_args+=(--bind "$d:$d")
done
[ -n "$QWEN_MODEL_DIR" ] && [ -d "$QWEN_MODEL_DIR" ] && bind_args+=(--bind "$QWEN_MODEL_DIR:$QWEN_MODEL_DIR")

# Port resolution
port_avail() { python3 -c "import socket; s=socket.socket(); s.settimeout(1); s.bind(('0.0.0.0',$1)); s.close()" 2>/dev/null; }
resolve_port() {
    local p="$1" m="${2:-50}"
    port_avail "$p" && echo "$p" && return 0
    for c in $(seq $((p+1)) $((p+m))); do
        port_avail "$c" && echo "$c" && return 0
    done
    echo "$p"
}

HPCC_BROKER_PORT=$(resolve_port "$HPCC_BROKER_PORT" 20)
PORT=$(resolve_port "$PORT" 20)
RAG_PORT=$(resolve_port "$RAG_PORT" 20)
export HPCC_BROKER_PORT PORT RAG_SERVICE_URL="http://127.0.0.1:$RAG_PORT"

detect_host() {
    if [ -d /net/8k3 ]; then echo '10.214.45.45'; return 0; fi
    if [ -d /mnt/usmidet ]; then echo '10.192.224.131'; return 0; fi
    hostname -I 2>/dev/null | awk '{print $1}' || echo '127.0.0.1'
}
PUBLIC_HOST=$(detect_host)

ui_cmd=(
    singularity run "${bind_args[@]}"
    --env "HPCC_BUNDLE_ROOT=$HPCC_BUNDLE_ROOT"
    --env "HPCC_PROJECT_ROOT=$HPCC_PROJECT_ROOT"
    --env "HPCC_BROKER_HOST=127.0.0.1"
    --env "HPCC_BROKER_PORT=$HPCC_BROKER_PORT"
    --env "HOST=0.0.0.0" --env "PORT=$PORT"
    --env "WORKERS=$WORKERS" --env "THREADS=$THREADS" --env "TIMEOUT=$TIMEOUT"
    --env "CACHE_HTML_DIR=$MAIN_HTML_CACHE_DIR"
    --env "DATABASE_URL=sqlite:///$HPCC_RUNTIME_DB"
    --env "CHROMADB_PATH=$MAIN_HTML_CACHE_DIR/chromadb_data"
    --env "RAG_SERVICE_URL=$RAG_SERVICE_URL"
    --env "HPCC_AUTO_START_RAG=1"
    "$SCRIPT_DIR/main_html.simg"
)
[ -n "$QWEN_MODEL_DIR" ] && ui_cmd+=(--env "HYPERLINK_VLM_MODEL_DIR=$QWEN_MODEL_DIR" --env "HPCC_HYPERLINK_VLM_MODEL_DIR=$QWEN_MODEL_DIR")

cleanup() { for p in "$UI_PID" "$BROKER_PID" "$RAG_PID"; do [ -n "$p" ] && kill "$p" 2>/dev/null || true; done; }
trap cleanup EXIT INT TERM

echo "Dashboard: http://$PUBLIC_HOST:$PORT/html"
echo "Broker:    port $HPCC_BROKER_PORT"
echo "RAG:       port $RAG_PORT"

"${BROKER_CMD[@]}" --host "$HPCC_BROKER_HOST" --port "$HPCC_BROKER_PORT" --broker-only >> "$LOG_DIR/broker.log" 2>&1 &
BROKER_PID="$!"
sleep 3

if [ -x "$SCRIPT_DIR/rag/run_rag.sh" ]; then
    FLASK_PORT="$RAG_PORT" bash "$SCRIPT_DIR/rag/run_rag.sh" --talk >> "$LOG_DIR/rag.log" 2>&1 &
    RAG_PID="$!"
    echo "RAG started (PID $RAG_PID)"
fi

"${ui_cmd[@]}" >> "$LOG_DIR/main_html.log" 2>&1 &
UI_PID="$!"

echo "All services started. Logs in $LOG_DIR"
while true; do
    for p in "$BROKER_PID" "$UI_PID"; do
        if ! kill -0 "$p" 2>/dev/null; then wait "$p" || true; echo "Process $p exited" >&2; exit 1; fi
    done
    sleep 5
done
