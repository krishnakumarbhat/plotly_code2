#!/bin/sh
set -eu

APP_DIR=/app/can_interactive_plot
MODE="${1:-help}"
shift || true

start_server() {
    port="${CAN_KPI_SERVER_PORT:-5556}"
    log_path="${CAN_KPI_SERVER_LOG:-/tmp/can_kpi_server.log}"
    python "$APP_DIR/can_kpi_server.py" zmq "$port" >"$log_path" 2>&1 &
    server_pid=$!

    cleanup_server() {
        kill "$server_pid" 2>/dev/null || true
        wait "$server_pid" 2>/dev/null || true
    }
    trap cleanup_server EXIT INT TERM

    attempt=1
    while [ "$attempt" -le 30 ]; do
        if ! kill -0 "$server_pid" 2>/dev/null; then
            cat "$log_path" >&2 || true
            echo "CAN KPI server exited before becoming ready" >&2
            exit 1
        fi
        if python - "$port" <<'PY'
import sys
import zmq
from InteractivePlot.kpi_client import hdf_add_pb2

port = int(sys.argv[1])
context = zmq.Context.instance()
socket = context.socket(zmq.REQ)
socket.setsockopt(zmq.LINGER, 0)
socket.connect(f"tcp://127.0.0.1:{port}")
socket.send(hdf_add_pb2.PingMessage(message_type="ping").SerializeToString())
ready = socket.poll(1000) != 0
if ready:
    response = hdf_add_pb2.PongMessage()
    response.ParseFromString(socket.recv())
    ready = response.status == "pong"
socket.close(0)
raise SystemExit(0 if ready else 1)
PY
        then
            return 0
        fi
        sleep 1
        attempt=$((attempt + 1))
    done

    echo "CAN KPI server did not become ready on port $port" >&2
    cat "$log_path" >&2 || true
    exit 1
}

case "$MODE" in
    server)
        exec python "$APP_DIR/can_kpi_server.py" zmq "${1:-${CAN_KPI_SERVER_PORT:-5556}}"
        ;;
    kpi)
        exec python "$APP_DIR/can_kpi_main.py" "$@"
        ;;
    plot)
        exec python "$APP_DIR/can_intplot_main.py" "$@"
        ;;
    both)
        if [ "$#" -lt 2 ]; then
            echo "Usage: docker run IMAGE both <config.xml> <inputs.json> [output_dir] [plot_config.json]" >&2
            exit 2
        fi
        start_server
        set +e
        python "$APP_DIR/can_intplot_main.py" "$@"
        rc=$?
        set -e
        cleanup_server
        trap - EXIT INT TERM
        exit "$rc"
        ;;
    help|--help|-h)
        cat <<'USAGE'
Combined CAN KPI and Interactive Plot container.

Modes:
  server [port]
  kpi <kpi.json> [html_dir]
  plot <config.xml> <inputs.json> [output_dir] [plot_config.json]
  both <config.xml> <inputs.json> [output_dir] [plot_config.json]
USAGE
        ;;
    *)
        echo "Unknown mode: $MODE" >&2
        exit 2
        ;;
esac
