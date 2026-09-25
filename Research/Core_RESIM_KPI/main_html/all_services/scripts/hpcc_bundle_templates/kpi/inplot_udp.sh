#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export BUNDLE_ROOT="${HPCC_BUNDLE_ROOT:-$(cd "$SCRIPT_DIR/.." && pwd)}"
export HPCC_PROJECT_ROOT="${HPCC_PROJECT_ROOT:-$BUNDLE_ROOT/bundle_src}"
# shellcheck disable=SC1091
source "$BUNDLE_ROOT/bundle_common.sh"

WAIT_FOR_COMPLETION=0
if [[ "${1:-}" == '--wait' ]]; then
    WAIT_FOR_COMPLETION=1
    shift
fi

if [[ $# -lt 2 || $# -gt 5 ]]; then
    echo 'Usage: inplot_udp.sh [--wait] <config.xml> <inputs.json> [output_dir] [port] [plot_config.json]' >&2
    exit 1
fi

CONFIG_XML="$(bundle_abs_path "$1")"
INPUT_JSON="$(bundle_abs_path "$2")"
OUTPUT_DIR=''
PORT='5560'
PLOT_CONFIG=''

if [[ $# -ge 3 && -n "${3:-}" ]]; then
    OUTPUT_DIR="$(bundle_abs_path "$3")"
fi
if [[ $# -ge 4 && -n "${4:-}" ]]; then
    PORT="$4"
fi
if [[ $# -ge 5 && -n "${5:-}" ]]; then
    PLOT_CONFIG="$(bundle_abs_path "$5")"
fi

bundle_ensure_file "$CONFIG_XML"
bundle_ensure_file "$INPUT_JSON"
if [[ -n "$PLOT_CONFIG" ]]; then
    bundle_ensure_file "$PLOT_CONFIG"
fi

if [[ -z "$OUTPUT_DIR" ]]; then
    OUTPUT_DIR="$(bundle_default_output_dir "$BUNDLE_ROOT" udp_interactive)"
else
    mkdir -p "$OUTPUT_DIR"
fi

bundle_require_tmux
RUN_DIR="$(bundle_user_run_dir udp_interactive)"
SESSION_NAME="$(id -un 2>/dev/null || printf '%s' user)_udp_intplot_$(bundle_timestamp)"
UDP_EXIT="$RUN_DIR/udp.exit"
INT_EXIT="$RUN_DIR/intplot.exit"
UDP_LOG="$RUN_DIR/udp.log"
INT_LOG="$RUN_DIR/intplot.log"
UDP_SCRIPT="$RUN_DIR/udp_window.sh"
INT_SCRIPT="$RUN_DIR/intplot_window.sh"

cat > "$UDP_SCRIPT" <<EOF
#!/usr/bin/env bash
set -uo pipefail
exec > >(tee -a "$UDP_LOG") 2>&1
"$BUNDLE_ROOT/kpi/udp/run_udp.sh" zmq "$PORT"
status=\$?
printf '%s' "\$status" > "$UDP_EXIT"
exit "\$status"
EOF

cat > "$INT_SCRIPT" <<EOF
#!/usr/bin/env bash
set -uo pipefail
exec > >(tee -a "$INT_LOG") 2>&1
export INTERACTIVE_PLOT_ENABLE_KPI=1
export KPI_SERVER_HOST=127.0.0.1
export KPI_SERVER_PORT="$PORT"
export APPTAINERENV_INTERACTIVE_PLOT_ENABLE_KPI=1
export APPTAINERENV_KPI_SERVER_HOST=127.0.0.1
export APPTAINERENV_KPI_SERVER_PORT="$PORT"
export SINGULARITYENV_INTERACTIVE_PLOT_ENABLE_KPI=1
export SINGULARITYENV_KPI_SERVER_HOST=127.0.0.1
export SINGULARITYENV_KPI_SERVER_PORT="$PORT"
if [[ -n "$PLOT_CONFIG" ]]; then
  "$BUNDLE_ROOT/kpi/int_plot/run_intplot.sh" "$CONFIG_XML" "$INPUT_JSON" "$OUTPUT_DIR" "$PLOT_CONFIG"
else
  "$BUNDLE_ROOT/kpi/int_plot/run_intplot.sh" "$CONFIG_XML" "$INPUT_JSON" "$OUTPUT_DIR"
fi
status=\$?
printf '%s' "\$status" > "$INT_EXIT"
exit "\$status"
EOF

chmod +x "$UDP_SCRIPT" "$INT_SCRIPT"
rm -f "$UDP_EXIT" "$INT_EXIT"
tmux kill-session -t "$SESSION_NAME" >/dev/null 2>&1 || true
tmux new-session -d -s "$SESSION_NAME" -n udp "bash '$UDP_SCRIPT'"

if ! bundle_wait_for_port 127.0.0.1 "$PORT" 90 2; then
    echo "Timed out waiting for UDP KPI server on port $PORT." >&2
    exit 1
fi

tmux new-window -t "$SESSION_NAME" -n interactive "bash '$INT_SCRIPT'"

echo "tmux_session=$SESSION_NAME"
echo "run_dir=$RUN_DIR"
echo "udp_log=$UDP_LOG"
echo "intplot_log=$INT_LOG"
echo "attach=tmux attach -t $SESSION_NAME"

if (( WAIT_FOR_COMPLETION == 0 )); then
    exit 0
fi

while true; do
    if [[ -f "$INT_EXIT" ]]; then
        status="$(cat "$INT_EXIT")"
        tmux send-keys -t "$SESSION_NAME:udp" C-c >/dev/null 2>&1 || true
        exit "$status"
    fi
    if [[ -f "$UDP_EXIT" ]]; then
        status="$(cat "$UDP_EXIT")"
        if [[ "$status" != '0' ]]; then
            exit "$status"
        fi
    fi
    sleep 2
done