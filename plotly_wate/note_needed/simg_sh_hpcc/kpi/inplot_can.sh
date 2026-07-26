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

if [[ $# -lt 2 || $# -gt 4 ]]; then
    echo 'Usage: inplot_can.sh [--wait] <config.xml> <inputs.json> [output_dir] [plot_config.json]' >&2
    exit 1
fi

CONFIG_XML="$(bundle_abs_path "$1")"
INPUT_JSON="$(bundle_abs_path "$2")"
OUTPUT_DIR=''
PLOT_CONFIG=''

if [[ $# -ge 3 && -n "${3:-}" ]]; then
    OUTPUT_DIR="$(bundle_abs_path "$3")"
fi
if [[ $# -ge 4 && -n "${4:-}" ]]; then
    PLOT_CONFIG="$(bundle_abs_path "$4")"
fi

bundle_ensure_file "$CONFIG_XML"
bundle_ensure_file "$INPUT_JSON"
if [[ -n "$PLOT_CONFIG" ]]; then
    bundle_ensure_file "$PLOT_CONFIG"
fi

if [[ -z "$OUTPUT_DIR" ]]; then
    OUTPUT_DIR="$(bundle_default_output_dir "$BUNDLE_ROOT" can_interactive)"
else
    mkdir -p "$OUTPUT_DIR"
fi

bundle_require_tmux
RUN_DIR="$(bundle_user_run_dir can_interactive)"
SESSION_NAME="$(id -un 2>/dev/null || printf '%s' user)_can_intplot_$(bundle_timestamp)"
MAIN_EXIT="$RUN_DIR/can_intplot.exit"
MAIN_LOG="$RUN_DIR/can_intplot.log"
MAIN_SCRIPT="$RUN_DIR/can_intplot_window.sh"

cat > "$MAIN_SCRIPT" <<EOF
#!/usr/bin/env bash
set -uo pipefail
exec > >(tee -a "$MAIN_LOG") 2>&1
echo '[inplot_can] experimental sequential CAN + Interactive Plot flow'
if [[ -n "$PLOT_CONFIG" ]]; then
  "$BUNDLE_ROOT/kpi_runtime_launcher.sh" --target can_kpi --source-target can_kpi --interactive-mode enabled --input-mode json --json-path "$INPUT_JSON" --config-xml "$CONFIG_XML" --optional-config "$PLOT_CONFIG" --output-dir "$OUTPUT_DIR"
else
  "$BUNDLE_ROOT/kpi_runtime_launcher.sh" --target can_kpi --source-target can_kpi --interactive-mode enabled --input-mode json --json-path "$INPUT_JSON" --config-xml "$CONFIG_XML" --output-dir "$OUTPUT_DIR"
fi
status=\$?
printf '%s' "\$status" > "$MAIN_EXIT"
exit "\$status"
EOF

chmod +x "$MAIN_SCRIPT"
rm -f "$MAIN_EXIT"
tmux kill-session -t "$SESSION_NAME" >/dev/null 2>&1 || true
tmux new-session -d -s "$SESSION_NAME" -n main "bash '$MAIN_SCRIPT'"

echo "tmux_session=$SESSION_NAME"
echo "run_dir=$RUN_DIR"
echo "log=$MAIN_LOG"
echo "attach=tmux attach -t $SESSION_NAME"

if (( WAIT_FOR_COMPLETION == 0 )); then
    exit 0
fi

while [[ ! -f "$MAIN_EXIT" ]]; do
    sleep 2
done
exit "$(cat "$MAIN_EXIT")"