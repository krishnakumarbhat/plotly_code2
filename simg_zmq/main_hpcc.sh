#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUNDLE_SRC_DIR="$SCRIPT_DIR/bundle_src"
DEFAULT_PROJECT_ROOT="$SCRIPT_DIR"
if [[ -d "$BUNDLE_SRC_DIR" ]]; then
    DEFAULT_PROJECT_ROOT="$BUNDLE_SRC_DIR"
elif [[ -d "$ROOT_DIR/main_html" ]]; then
    DEFAULT_PROJECT_ROOT="$ROOT_DIR"
elif [[ -d "$ROOT_DIR/KPI" ]]; then
    DEFAULT_PROJECT_ROOT="$ROOT_DIR"
elif [[ -d "$ROOT_DIR/rag" ]]; then
    DEFAULT_PROJECT_ROOT="$ROOT_DIR"
fi

export HPCC_BUNDLE_ROOT="${HPCC_BUNDLE_ROOT:-$SCRIPT_DIR}"
export BUNDLE_ROOT="$HPCC_BUNDLE_ROOT"
if [[ -f "$SCRIPT_DIR/bundle_common.sh" ]]; then
    # shellcheck disable=SC1091
    source "$SCRIPT_DIR/bundle_common.sh"
fi

RUNTIME_CONFIG_FILE="${HPCC_RUNTIME_CONFIG:-$SCRIPT_DIR/hpcc_runtime.env}"
if [[ -f "$RUNTIME_CONFIG_FILE" ]]; then
    # shellcheck disable=SC1090
    source "$RUNTIME_CONFIG_FILE"
fi

select_fast_runtime_root() {
    local candidate
    for candidate in "${HPCC_RUNTIME_LOCAL_ROOT:-}" /local/hpc_tools /var/tmp/hpc_tools /tmp/hpc_tools "$SCRIPT_DIR/runtime_local"; do
        [[ -n "$candidate" ]] || continue
        if mkdir -p "$candidate" >/dev/null 2>&1 && [[ -w "$candidate" ]]; then
            printf '%s' "$candidate"
            return 0
        fi
    done
    return 1
}

FAST_RUNTIME_ROOT="$(select_fast_runtime_root)"
export HPCC_RUNTIME_LOCAL_ROOT="$FAST_RUNTIME_ROOT"

LOG_DIR="$SCRIPT_DIR/logs"
RUNTIME_STATE_DIR="$SCRIPT_DIR/runtime_state"
MAIN_HTML_STATE_DIR="$RUNTIME_STATE_DIR/main_html"
MAIN_HTML_CACHE_DIR="${CACHE_HTML_DIR:-$FAST_RUNTIME_ROOT/cache_html}"
mkdir -p "$LOG_DIR" "$MAIN_HTML_CACHE_DIR/html" "$MAIN_HTML_CACHE_DIR/video" "$MAIN_HTML_CACHE_DIR/vlm_cache"

rotate_log() {
    local log_path="$1"
    if [[ -f "$log_path" ]]; then
        local stamp
        stamp="$(date +%Y%m%d_%H%M%S 2>/dev/null || printf '%s' now)"
        mv "$log_path" "${log_path}.${stamp}.prev" >/dev/null 2>&1 || true
    fi
}

rotate_log "$LOG_DIR/hpcc_broker.log"
rotate_log "$LOG_DIR/main_html.log"
rotate_log "$LOG_DIR/rag.log"

export HPCC_PROJECT_ROOT="${HPCC_PROJECT_ROOT:-$DEFAULT_PROJECT_ROOT}"
if [[ -z "${HPCC_RUNTIME_DB:-}" ]]; then
    runtime_db_root="$FAST_RUNTIME_ROOT/db"
    mkdir -p "$runtime_db_root"
    export HPCC_RUNTIME_DB="$runtime_db_root/hpc_tools_dev.db"
else
    export HPCC_RUNTIME_DB
fi
export HPCC_BUNDLE_VERSION="${HPCC_BUNDLE_VERSION:-2026-04-13-hpcc-standalone}"
export HPCC_PUBLIC_HOST="${HPCC_PUBLIC_HOST:-}"
export HPCC_BROKER_HOST="${HPCC_BROKER_HOST:-0.0.0.0}"
export HPCC_BROKER_PORT="${HPCC_BROKER_PORT:-9100}"
export PORT="${PORT:-5005}"
export HOST_SIMG_PATH="$SCRIPT_DIR"
export HPCC_AUTO_START_RAG="${HPCC_AUTO_START_RAG:-0}"
export HPCC_PORT_CONFLICT_POLICY="${HPCC_PORT_CONFLICT_POLICY:-shift}"
export HPCC_REQUIRE_SLURM_FOR_KPI="${HPCC_REQUIRE_SLURM_FOR_KPI:-1}"
export HPCC_ALLOW_LOCAL_KPI="${HPCC_ALLOW_LOCAL_KPI:-0}"
export HPCC_SLURM_IMMEDIATE_SECONDS="${HPCC_SLURM_IMMEDIATE_SECONDS:-}"
export HPCC_HYPERLINK_SESSION_TTL_SECONDS="${HPCC_HYPERLINK_SESSION_TTL_SECONDS:-1800}"
export WORKERS="${WORKERS:-3}"
export THREADS="${THREADS:-4}"
export KEEPALIVE="${KEEPALIVE:-5}"
export TIMEOUT="${TIMEOUT:-240}"

broker_python() {
    local candidate
    for candidate in "${HPCC_BROKER_PYTHON:-}" python3.12 python3.11 python3.10 python3.9 python3.8 python3.7 python3; do
        [[ -n "$candidate" ]] || continue
        if command -v "$candidate" >/dev/null 2>&1; then
            printf '%s' "$candidate"
            return 0
        fi
    done
    echo 'Python 3.7+ is required for hpcc_main.pyz.' >&2
    return 1
}

BROKER_PYTHON="$(broker_python)"
export BROKER_PYTHON

BROKER_ARTIFACT=""
BROKER_CMD=()
if [[ -f "$SCRIPT_DIR/hpcc_main.pyz" ]]; then
    BROKER_ARTIFACT="$SCRIPT_DIR/hpcc_main.pyz"
    BROKER_CMD=("$BROKER_PYTHON" "$BROKER_ARTIFACT")
elif [[ -f "$SCRIPT_DIR/hpcc_main.py" ]]; then
    BROKER_ARTIFACT="$SCRIPT_DIR/hpcc_main.py"
    BROKER_CMD=("$BROKER_PYTHON" "$BROKER_ARTIFACT")
elif [[ -f "$ROOT_DIR/hpcc_main.py" ]]; then
    BROKER_ARTIFACT="$ROOT_DIR/hpcc_main.py"
    BROKER_CMD=("$BROKER_PYTHON" "$BROKER_ARTIFACT")
fi
if [[ -z "$BROKER_ARTIFACT" ]]; then
    echo "Missing bundled broker artifact. Expected $SCRIPT_DIR/hpcc_main.pyz or $SCRIPT_DIR/hpcc_main.py." >&2
    exit 1
fi

python_path_parts=("$SCRIPT_DIR")
if [[ -d "$HPCC_PROJECT_ROOT" ]]; then
    python_path_parts+=("$HPCC_PROJECT_ROOT")
fi
export PYTHONPATH="$(IFS=:; printf '%s' "${python_path_parts[*]}")${PYTHONPATH:+:$PYTHONPATH}"

MAIN_HTML_IMAGE="$SCRIPT_DIR/main_html.simg"
if [[ ! -f "$MAIN_HTML_IMAGE" ]]; then
    echo "Missing $MAIN_HTML_IMAGE. Build the standalone bundle first." >&2
    exit 1
fi

bundle_load_runtime_module
if ! RUNTIME_BIN="$(bundle_runtime_bin)"; then
    echo 'Apptainer/Singularity is required to launch the HPCC bundle.' >&2
    exit 1
fi

QWEN_MODEL_DIR="${QWEN_MODEL_DIR:-}"
if [[ -z "$QWEN_MODEL_DIR" ]]; then
    for candidate in "$SCRIPT_DIR/rag/model" "$ROOT_DIR/rag/model" "$ROOT_DIR/rag/qwn_kk_fine_model"; do
        if [[ -d "$candidate" ]]; then
            QWEN_MODEL_DIR="$candidate"
            break
        fi
    done
fi

MAIN_HTML_SOURCE_DIR=''
if [[ -d "$SCRIPT_DIR/bundle_src/main_html" ]]; then
    MAIN_HTML_SOURCE_DIR="$SCRIPT_DIR/bundle_src/main_html"
elif [[ -d "$HPCC_PROJECT_ROOT/main_html" ]]; then
    MAIN_HTML_SOURCE_DIR="$HPCC_PROJECT_ROOT/main_html"
fi

HYPERLINK_SOURCE_DIR=''
if [[ -d "$SCRIPT_DIR/bundle_src/Hyperlink_tool" ]]; then
    HYPERLINK_SOURCE_DIR="$SCRIPT_DIR/bundle_src/Hyperlink_tool"
elif [[ -d "$HPCC_PROJECT_ROOT/Hyperlink_tool" ]]; then
    HYPERLINK_SOURCE_DIR="$HPCC_PROJECT_ROOT/Hyperlink_tool"
fi

bind_args=()
if [[ -d "$HPCC_PROJECT_ROOT" ]]; then
    bind_args+=(--bind "$HPCC_PROJECT_ROOT:$HPCC_PROJECT_ROOT")
fi
if [[ -d "$SCRIPT_DIR" && "$SCRIPT_DIR" != "$HPCC_PROJECT_ROOT" ]]; then
    bind_args+=(--bind "$SCRIPT_DIR:$SCRIPT_DIR")
fi
if [[ -d "$ROOT_DIR" && "$ROOT_DIR" != "$HPCC_PROJECT_ROOT" && "$ROOT_DIR" != "$SCRIPT_DIR" ]]; then
    bind_args+=(--bind "$ROOT_DIR:$ROOT_DIR")
fi
for host_path in /net /scratch /mnt; do
    if [[ -d "$host_path" ]]; then
        bind_args+=(--bind "$host_path:$host_path")
    fi
done
if [[ -n "$MAIN_HTML_SOURCE_DIR" ]]; then
    bind_args+=(--bind "$MAIN_HTML_SOURCE_DIR:/app/main_html")
fi
if [[ -n "$HYPERLINK_SOURCE_DIR" ]]; then
    bind_args+=(--bind "$HYPERLINK_SOURCE_DIR:/app/Hyperlink_tool")
fi
if [[ -n "$QWEN_MODEL_DIR" && -d "$QWEN_MODEL_DIR" ]]; then
    bind_args+=(--bind "$QWEN_MODEL_DIR:$QWEN_MODEL_DIR")
fi

port_is_available() {
    local port="$1"
    python3 - "$port" <<'PY'
import socket
import sys

port = int(sys.argv[1])
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        sock.bind(('0.0.0.0', port))
    except OSError:
        raise SystemExit(1)
raise SystemExit(0)
PY
}

find_listen_pids() {
    local port="$1"
    if command -v lsof >/dev/null 2>&1; then
        lsof -tiTCP:"$port" -sTCP:LISTEN 2>/dev/null || true
        return 0
    fi
    if command -v ss >/dev/null 2>&1; then
        ss -ltnp "sport = :$port" 2>/dev/null | sed -n 's/.*pid=\([0-9][0-9]*\).*/\1/p' || true
        return 0
    fi
    return 1
}

resolve_requested_port() {
    local label="$1"
    local requested_port="$2"
    local policy="$3"
    local max_shift="${4:-50}"
    local current_user
    current_user="$(id -un 2>/dev/null || printf '%s' unknown)"

    if port_is_available "$requested_port"; then
        printf '%s\n' "$requested_port"
        return 0
    fi

    if [[ "$policy" == 'kill' ]]; then
        local pid owner killed_any=0
        while IFS= read -r pid; do
            [[ -n "$pid" ]] || continue
            owner="$(ps -o user= -p "$pid" 2>/dev/null | tr -d '[:space:]')"
            if [[ "$owner" != "$current_user" ]]; then
                continue
            fi
            kill "$pid" >/dev/null 2>&1 || true
            sleep 1
            kill -9 "$pid" >/dev/null 2>&1 || true
            killed_any=1
        done < <(find_listen_pids "$requested_port")
        if (( killed_any )) && port_is_available "$requested_port"; then
            printf '%s\n' "$requested_port"
            return 0
        fi
    elif [[ "$policy" == 'fail' ]]; then
        echo "$label port $requested_port is busy and automatic recovery is disabled." >&2
        return 1
    fi

    local candidate
    for candidate in $(seq $((requested_port + 1)) $((requested_port + max_shift))); do
        if port_is_available "$candidate"; then
            echo "$label port shifted from $requested_port to $candidate." >&2
            printf '%s\n' "$candidate"
            return 0
        fi
    done

    echo "Unable to find a free port for $label." >&2
    return 1
}

wait_for_tcp() {
    bundle_wait_for_port "$1" "$2" "${3:-60}" 1
}

wait_for_http() {
    local host="$1"
    local port="$2"
    local path="$3"
    local retries="${4:-120}"
    local attempt
    for attempt in $(seq 1 "$retries"); do
        if python3 - "$host" "$port" "$path" <<'PY'
import http.client
import sys

host = sys.argv[1]
port = int(sys.argv[2])
path = sys.argv[3]
try:
    conn = http.client.HTTPConnection(host, port, timeout=2)
    conn.request('GET', path)
    response = conn.getresponse()
    if 200 <= response.status < 500:
        raise SystemExit(0)
except Exception:
    raise SystemExit(1)
raise SystemExit(1)
PY
        then
            return 0
        fi
        sleep 1
    done
    return 1
}

detect_public_host() {
    if [[ -n "$HPCC_PUBLIC_HOST" ]]; then
        printf '%s\n' "$HPCC_PUBLIC_HOST"
        return 0
    fi
    if [[ -d /net/8k3 ]]; then
        printf '%s\n' '10.214.45.45'
        return 0
    fi
    if [[ -d /mnt/usmidet ]]; then
        printf '%s\n' '10.192.224.131'
        return 0
    fi

    python3 - <<'PY'
import socket

candidate = ''
try:
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as connection:
        connection.connect(('8.8.8.8', 80))
        candidate = connection.getsockname()[0]
except OSError:
    candidate = ''

if not candidate or candidate.startswith('127.'):
    try:
        values = socket.gethostbyname_ex(socket.gethostname())[2]
    except OSError:
        values = []
    for value in values:
        if value and not value.startswith('127.'):
            candidate = value
            break

print(candidate)
PY
}

HPCC_BROKER_PORT="$(resolve_requested_port 'Broker' "$HPCC_BROKER_PORT" "$HPCC_PORT_CONFLICT_POLICY")"
export HPCC_BROKER_PORT
PORT="$(resolve_requested_port 'Dashboard' "$PORT" "$HPCC_PORT_CONFLICT_POLICY")"
export PORT
RAG_PORT="${RAG_PORT:-5100}"
RAG_PORT="$(resolve_requested_port 'RAG' "$RAG_PORT" "$HPCC_PORT_CONFLICT_POLICY")"
while [[ "$RAG_PORT" == "$PORT" ]]; do
    RAG_PORT="$(resolve_requested_port 'RAG' "$((RAG_PORT + 1))" shift)"
done
export RAG_PORT

PUBLIC_HOST="$(detect_public_host)"
RAG_SERVICE_URL=''
if [[ "$HPCC_AUTO_START_RAG" =~ ^(1|true|yes|y)$ ]]; then
    RAG_SERVICE_URL="http://${PUBLIC_HOST:-127.0.0.1}:$RAG_PORT"
fi

ui_cmd=(
    "$RUNTIME_BIN" run
    "${bind_args[@]}"
    --env "HPCC_BUNDLE_ROOT=$HPCC_BUNDLE_ROOT"
    --env "HPCC_PROJECT_ROOT=$HPCC_PROJECT_ROOT"
    --env "HPCC_BROKER_HOST=127.0.0.1"
    --env "HPCC_BROKER_PORT=$HPCC_BROKER_PORT"
    --env "HPC_TOOLS_AUTH_MODE=cluster"
    --env "HPC_TOOLS_DISABLE_DEFAULT_ADMIN=1"
    --env "HOST=0.0.0.0"
    --env "PORT=$PORT"
    --env "WORKERS=$WORKERS"
    --env "THREADS=$THREADS"
    --env "KEEPALIVE=$KEEPALIVE"
    --env "TIMEOUT=$TIMEOUT"
    --env "CACHE_HTML_DIR=$MAIN_HTML_CACHE_DIR"
    --env "DATABASE_URL=sqlite:///$HPCC_RUNTIME_DB"
    --env "CHROMADB_PATH=$MAIN_HTML_CACHE_DIR/chromadb_data"
    --env "RAG_SERVICE_URL=$RAG_SERVICE_URL"
    --env "HF_HUB_OFFLINE=1"
    --env "TRANSFORMERS_OFFLINE=1"
    --env "HPCC_REQUIRE_SLURM_FOR_KPI=$HPCC_REQUIRE_SLURM_FOR_KPI"
    --env "HPCC_ALLOW_LOCAL_KPI=$HPCC_ALLOW_LOCAL_KPI"
    --env "HPCC_SLURM_IMMEDIATE_SECONDS=$HPCC_SLURM_IMMEDIATE_SECONDS"
    --env "HPCC_HYPERLINK_SESSION_TTL_SECONDS=$HPCC_HYPERLINK_SESSION_TTL_SECONDS"
    "$MAIN_HTML_IMAGE"
)
if [[ -n "$QWEN_MODEL_DIR" ]]; then
    ui_cmd=(
        "$RUNTIME_BIN" run
        "${bind_args[@]}"
        --env "HPCC_BUNDLE_ROOT=$HPCC_BUNDLE_ROOT"
        --env "HPCC_PROJECT_ROOT=$HPCC_PROJECT_ROOT"
        --env "HPCC_BROKER_HOST=127.0.0.1"
        --env "HPCC_BROKER_PORT=$HPCC_BROKER_PORT"
        --env "HPC_TOOLS_AUTH_MODE=cluster"
        --env "HPC_TOOLS_DISABLE_DEFAULT_ADMIN=1"
        --env "HOST=0.0.0.0"
        --env "PORT=$PORT"
        --env "WORKERS=$WORKERS"
        --env "THREADS=$THREADS"
        --env "KEEPALIVE=$KEEPALIVE"
        --env "TIMEOUT=$TIMEOUT"
        --env "CACHE_HTML_DIR=$MAIN_HTML_CACHE_DIR"
        --env "DATABASE_URL=sqlite:///$HPCC_RUNTIME_DB"
        --env "CHROMADB_PATH=$MAIN_HTML_CACHE_DIR/chromadb_data"
        --env "RAG_SERVICE_URL=$RAG_SERVICE_URL"
        --env "HF_HUB_OFFLINE=1"
        --env "TRANSFORMERS_OFFLINE=1"
        --env "HYPERLINK_VLM_MODEL_DIR=$QWEN_MODEL_DIR"
        --env "HPCC_HYPERLINK_VLM_MODEL_DIR=$QWEN_MODEL_DIR"
        --env "HPCC_REQUIRE_SLURM_FOR_KPI=$HPCC_REQUIRE_SLURM_FOR_KPI"
        --env "HPCC_ALLOW_LOCAL_KPI=$HPCC_ALLOW_LOCAL_KPI"
        --env "HPCC_SLURM_IMMEDIATE_SECONDS=$HPCC_SLURM_IMMEDIATE_SECONDS"
        --env "HPCC_HYPERLINK_SESSION_TTL_SECONDS=$HPCC_HYPERLINK_SESSION_TTL_SECONDS"
        "$MAIN_HTML_IMAGE"
    )
fi

BROKER_LOG="$LOG_DIR/hpcc_broker.log"
UI_LOG="$LOG_DIR/main_html.log"
RAG_LOG="$LOG_DIR/rag.log"
BROKER_PID=''
UI_PID=''
RAG_PID=''

cleanup() {
    local pid
    for pid in "$UI_PID" "$BROKER_PID" "$RAG_PID"; do
        [[ -n "$pid" ]] || continue
        kill "$pid" >/dev/null 2>&1 || true
    done
}
trap cleanup EXIT INT TERM

printf 'HPCC bundle root: %s\n' "$SCRIPT_DIR"
printf 'Project root: %s\n' "$HPCC_PROJECT_ROOT"
printf 'Dashboard URL: http://%s:%s/html\n' "${PUBLIC_HOST:-127.0.0.1}" "$PORT"
printf 'Broker port: %s\n' "$HPCC_BROKER_PORT"

"${BROKER_CMD[@]}" --host "$HPCC_BROKER_HOST" --port "$HPCC_BROKER_PORT" --broker-only >> "$BROKER_LOG" 2>&1 &
BROKER_PID="$!"

if ! wait_for_tcp 127.0.0.1 "$HPCC_BROKER_PORT" 60; then
    echo 'Broker did not become ready.' >&2
    exit 1
fi

if [[ "$HPCC_AUTO_START_RAG" =~ ^(1|true|yes|y)$ ]]; then
    if [[ ! -x "$SCRIPT_DIR/rag/run_rag.sh" ]]; then
        echo 'RAG auto-start is enabled but rag/run_rag.sh is missing or not executable.' >&2
        exit 1
    fi
    FLASK_PORT="$RAG_PORT" "$SCRIPT_DIR/rag/run_rag.sh" --talk >> "$RAG_LOG" 2>&1 &
    RAG_PID="$!"
    if ! wait_for_http 127.0.0.1 "$RAG_PORT" /health 120; then
        echo 'WARNING: rag did not become ready (continuing without rag).' >&2
        RAG_PID=''
    fi
fi

"${ui_cmd[@]}" >> "$UI_LOG" 2>&1 &
UI_PID="$!"

if ! wait_for_http 127.0.0.1 "$PORT" /html 120; then
    echo 'main_html did not become ready.' >&2
    exit 1
fi

while true; do
    if ! kill -0 "$BROKER_PID" >/dev/null 2>&1; then
        wait "$BROKER_PID" || true
        echo 'Broker exited.' >&2
        exit 1
    fi
    if ! kill -0 "$UI_PID" >/dev/null 2>&1; then
        wait "$UI_PID"
        exit $?
    fi
    if [[ -n "$RAG_PID" ]] && ! kill -0 "$RAG_PID" >/dev/null 2>&1; then
        wait "$RAG_PID" || true
        RAG_PID=''
    fi
    sleep 2
done