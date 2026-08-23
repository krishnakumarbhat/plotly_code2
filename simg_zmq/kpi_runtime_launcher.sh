#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export BUNDLE_ROOT="${HPCC_BUNDLE_ROOT:-$SCRIPT_DIR}"
export HPCC_PROJECT_ROOT="${HPCC_PROJECT_ROOT:-$SCRIPT_DIR/bundle_src}"
# shellcheck disable=SC1091
source "$SCRIPT_DIR/bundle_common.sh"

usage() {
    cat <<'EOF'
Usage: kpi_runtime_launcher.sh --target <can_kpi|udp_kpi|interactive_plot> \
  [--source-target <can_kpi|udp_kpi>] \
  [--interactive-mode <disabled|enabled|only>] \
  [--input-mode <json|hdf>] \
  [--json-path <file.json>] \
  [--input-hdf <input.h5> --output-hdf <output.h5>] \
  [--config-xml <ConfigInteractivePlots.xml>] \
  [--optional-config <plot_config.json>] \
  [--output-dir <dir>] \
  [--port <zmq_port>] \
  [--detached]
EOF
}

TARGET=''
SOURCE_TARGET='udp_kpi'
INTERACTIVE_MODE='disabled'
INPUT_MODE='json'
JSON_PATH=''
INPUT_HDF=''
OUTPUT_HDF=''
OUTPUT_DIR=''
CONFIG_XML=''
OPTIONAL_CONFIG=''
PORT="${KPI_SERVER_PORT:-5560}"
DETACHED=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        --target)
            TARGET="$2"
            shift 2
            ;;
        --source-target)
            SOURCE_TARGET="$2"
            shift 2
            ;;
        --interactive-mode)
            INTERACTIVE_MODE="$2"
            shift 2
            ;;
        --input-mode)
            INPUT_MODE="$2"
            shift 2
            ;;
        --json-path)
            JSON_PATH="$(bundle_abs_path "$2")"
            shift 2
            ;;
        --input-hdf)
            INPUT_HDF="$(bundle_abs_path "$2")"
            shift 2
            ;;
        --output-hdf)
            OUTPUT_HDF="$(bundle_abs_path "$2")"
            shift 2
            ;;
        --output-dir)
            OUTPUT_DIR="$(bundle_abs_path "$2")"
            shift 2
            ;;
        --config-xml)
            CONFIG_XML="$(bundle_abs_path "$2")"
            shift 2
            ;;
        --optional-config)
            OPTIONAL_CONFIG="$(bundle_abs_path "$2")"
            shift 2
            ;;
        --port)
            PORT="$2"
            shift 2
            ;;
        --detached)
            DETACHED=1
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown argument: $1" >&2
            usage >&2
            exit 1
            ;;
    esac
done

if [[ -z "$TARGET" ]]; then
    usage >&2
    exit 1
fi

if [[ "$SOURCE_TARGET" != 'can_kpi' ]]; then
    SOURCE_TARGET='udp_kpi'
fi
if [[ "$INTERACTIVE_MODE" != 'enabled' && "$INTERACTIVE_MODE" != 'only' ]]; then
    INTERACTIVE_MODE='disabled'
fi
if [[ "$TARGET" == 'interactive_plot' ]]; then
    INTERACTIVE_MODE='only'
fi
if [[ "$INPUT_MODE" != 'hdf' ]]; then
    INPUT_MODE='json'
fi

if [[ -z "$OUTPUT_DIR" ]]; then
    OUTPUT_DIR="$(bundle_default_output_dir "$BUNDLE_ROOT" "$TARGET")"
else
    mkdir -p "$OUTPUT_DIR"
fi

if [[ -n "$OPTIONAL_CONFIG" ]]; then
    if [[ -d "$OPTIONAL_CONFIG" ]]; then
        echo "Optional config path is a directory; ignoring it: $OPTIONAL_CONFIG" >&2
        OPTIONAL_CONFIG=''
    else
        bundle_ensure_file "$OPTIONAL_CONFIG"
    fi
fi

if [[ "$INPUT_MODE" == 'json' ]]; then
    bundle_ensure_file "$JSON_PATH"
else
    bundle_ensure_file "$INPUT_HDF"
    bundle_ensure_file "$OUTPUT_HDF"
fi

if [[ "$INTERACTIVE_MODE" != 'disabled' ]]; then
    if [[ -z "$CONFIG_XML" ]]; then
        CONFIG_XML="$(bundle_default_config_xml "$BUNDLE_ROOT" "$SOURCE_TARGET")"
    fi
    bundle_ensure_file "$CONFIG_XML"
fi

TMP_FILES=()
UDP_PID=''

cleanup() {
    local file_path
    if [[ -n "$UDP_PID" ]]; then
        kill "$UDP_PID" >/dev/null 2>&1 || true
        wait "$UDP_PID" >/dev/null 2>&1 || true
    fi
    for file_path in "${TMP_FILES[@]:-}"; do
        rm -f "$file_path" >/dev/null 2>&1 || true
    done
}
trap cleanup EXIT INT TERM

pair_json_for_interactive() {
    if [[ "$INPUT_MODE" == 'json' ]]; then
        printf '%s\n' "$JSON_PATH"
        return 0
    fi

    local temp_json
    temp_json="$(bundle_make_pair_json "$INPUT_HDF" "$OUTPUT_HDF")"
    TMP_FILES+=("$temp_json")
    printf '%s\n' "$temp_json"
}

run_interactive_plot() {
    local config_path="$1"
    local inputs_json="$2"
    local target_output="$3"
    local -a command
    command=("$BUNDLE_ROOT/kpi/int_plot/run_intplot.sh" "$config_path" "$inputs_json" "$target_output")
    if [[ -n "$OPTIONAL_CONFIG" ]]; then
        command+=("$OPTIONAL_CONFIG")
    fi
    "${command[@]}"
}

wait_for_udp_port_or_exit() {
    local attempts="${HPCC_PORT_WAIT_ATTEMPTS:-90}"
    local delay="${HPCC_PORT_WAIT_DELAY_SECONDS:-2}"
    local attempt
    local status

    for attempt in $(seq 1 "$attempts"); do
        if bundle_wait_for_port 127.0.0.1 "$PORT" 1 0; then
            return 0
        fi
        if ! kill -0 "$UDP_PID" >/dev/null 2>&1; then
            if wait "$UDP_PID"; then
                status=0
            else
                status=$?
            fi
            echo "UDP KPI server exited before port $PORT became ready (status $status)." >&2
            if [[ "$status" == '0' ]]; then
                return 1
            fi
            return "$status"
        fi
        sleep "$delay"
    done

    return 1
}

case "$TARGET" in
    can_kpi)
        if [[ "$INTERACTIVE_MODE" == 'enabled' ]]; then
            local_json="$(pair_json_for_interactive)"
            # inplot_can.sh prefers the combined CAN KPI + Interactive Plot image
            # (kpi/can_intplot/canudp_intplot_kpi.simg) and falls back to the sequential
            # run_can.sh + run_intplot.sh flow when it is not present.
            command=("$BUNDLE_ROOT/kpi/inplot_can.sh" --wait "$CONFIG_XML" "$local_json" "$OUTPUT_DIR")
            if [[ -n "$OPTIONAL_CONFIG" ]]; then
                command+=("$OPTIONAL_CONFIG")
            fi
            "${command[@]}"
            exit 0
        fi

        if [[ "$INPUT_MODE" == 'hdf' ]]; then
            "$BUNDLE_ROOT/kpi/can/run_can.sh" --hdf "$INPUT_HDF" "$OUTPUT_HDF" "$OUTPUT_DIR"
        else
            "$BUNDLE_ROOT/kpi/can/run_can.sh" "$JSON_PATH" "$OUTPUT_DIR"
        fi
        ;;
    udp_kpi)
        if [[ "$INTERACTIVE_MODE" == 'enabled' ]]; then
            local_json="$(pair_json_for_interactive)"
            if (( DETACHED )); then
                command=("$BUNDLE_ROOT/kpi/inplot_udp.sh" "$CONFIG_XML" "$local_json" "$OUTPUT_DIR" "$PORT")
                if [[ -n "$OPTIONAL_CONFIG" ]]; then
                    command+=("$OPTIONAL_CONFIG")
                fi
                "${command[@]}"
                exit 0
            fi
            "$BUNDLE_ROOT/kpi/udp/run_udp.sh" zmq "$PORT" &
            UDP_PID="$!"
            if ! wait_for_udp_port_or_exit; then
                echo "Timed out waiting for UDP KPI server on port $PORT." >&2
                exit 1
            fi
            bundle_set_container_env INTERACTIVE_PLOT_ENABLE_KPI 1
            bundle_set_container_env KPI_SERVER_HOST 127.0.0.1
            bundle_set_container_env KPI_SERVER_PORT "$PORT"
            run_interactive_plot "$CONFIG_XML" "$local_json" "$OUTPUT_DIR"
            exit 0
        fi

        if [[ "$INPUT_MODE" == 'hdf' ]]; then
            "$BUNDLE_ROOT/kpi/udp/run_udp.sh" --hdf "$INPUT_HDF" "$OUTPUT_HDF" "$OUTPUT_DIR"
        else
            "$BUNDLE_ROOT/kpi/udp/run_udp.sh" "$JSON_PATH" "$OUTPUT_DIR"
        fi
        ;;
    interactive_plot)
        local_json="$(pair_json_for_interactive)"
        run_interactive_plot "$CONFIG_XML" "$local_json" "$OUTPUT_DIR"
        ;;
    *)
        echo "Unsupported target: $TARGET" >&2
        exit 1
        ;;
esac