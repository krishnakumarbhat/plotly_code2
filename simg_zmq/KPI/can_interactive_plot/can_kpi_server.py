"""
CAN KPI Integration Server.

Basic ZMQ-based bridge in the style of UDP_KPI/kpi_server.py: receives per-sensor
notifications from the interactive plot pipeline, runs the CAN KPI pipeline
(a_persistence_layer/b_data_storage parsing + c_business_layer matching +
d_presentation_layer HTML), and returns the KPI HTML path for the plot pipeline
to embed as a KPI tab.

Usage
-----
    python can_kpi_server.py zmq [PORT]
    python can_kpi_server.py CONFIG.json [HTML_DIR]
    python can_kpi_server.py INPUT.hdf OUTPUT.hdf [HTML_DIR]
"""

import logging
import os
import json
import sys
from pathlib import Path
from typing import Dict, Any, Optional
import zmq

# Compatibility for older generated *_pb2.py against newer protobuf runtimes.
os.environ.setdefault('PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION', 'python')

from a_persistence_layer.can_kpi_wrapper import parse_for_can_kpi, get_engine

# Lazily import protobuf definitions only when running ZMQ mode
hdf_add_pb2 = None

def _ensure_pb():
    global hdf_add_pb2
    if hdf_add_pb2 is None:
        import sys
        here = Path(__file__).resolve().parent
        candidates = [
            here,
            here / "kpi_proto",
            here.parent / "can_inplot" / "kpi_client",
        ]
        for candidate in candidates:
            candidate_str = str(candidate)
            if candidate.exists() and candidate_str not in sys.path:
                sys.path.insert(0, candidate_str)
        try:
            from kpi_proto import hdf_add_pb2 as _pb
        except ImportError:
            import hdf_add_pb2 as _pb
        hdf_add_pb2 = _pb

logger = logging.getLogger(__name__)

class CanKPIZMQServer:
    """ZMQ server for handling CAN KPI processing requests using protobuf."""

    def __init__(self, port: int = 6000):
        self.port = port
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self._running = False
        # Stores the most recently generated KPI HTML file path
        self._latest_html_path: Optional[str] = None

    def start(self):
        """Start the ZMQ server."""
        try:
            # Ensure protobuf is available when running server
            _ensure_pb()
            from port_utils import bind_with_retry
            self.port = bind_with_retry(self.socket, self.port)
            self._running = True
            logger.info(f"CAN KPI ZMQ server started on port {self.port}")

            while self._running:
                try:
                    # Receive protobuf message
                    message_bytes = self.socket.recv()
                    logger.debug(f"Received protobuf message of size: {len(message_bytes)}")

                    # Process message
                    response_bytes = self._process_protobuf_message(message_bytes)

                    # Send protobuf response
                    self.socket.send(response_bytes)

                except zmq.ZMQError as e:
                    if self._running:
                        logger.error(f"ZMQ error: {e}")
                    break
                except Exception as e:
                    logger.error(f"Error processing message: {e}")
                    # Send error response as protobuf
                    error_response = hdf_add_pb2.ReplyMessage(
                        status="error",
                        message=str(e)
                    )
                    self.socket.send(error_response.SerializeToString())

        except Exception as e:
            logger.error(f"Failed to start server: {e}")
        finally:
            self.stop()

    def stop(self):
        """Stop the ZMQ server."""
        self._running = False
        if hasattr(self, 'socket'):
            self.socket.close()
        if hasattr(self, 'context'):
            self.context.term()
        logger.info("CAN KPI ZMQ server stopped")

    def _process_protobuf_message(self, message_bytes: bytes) -> bytes:
        """Process incoming protobuf message and return protobuf response."""
        try:
            # Handle raw ping message - return server status
            if message_bytes == b'\n\x04ping':
                pong_response = hdf_add_pb2.PongMessage(
                    status="pong",
                    message="Server is Running"
                )
                return pong_response.SerializeToString()
            elif message_bytes == b'\n\x0crequest_html':
                # Handle raw HTML request - return latest HTML path
                if self._latest_html_path and os.path.exists(self._latest_html_path):
                    protobuf_response = hdf_add_pb2.ReplyMessage(
                        html_file_path=self._latest_html_path,
                        status="success",
                        message=""
                    )
                else:
                    protobuf_response = hdf_add_pb2.ReplyMessage(
                        status="error",
                        message="No HTML available yet"
                    )
                return protobuf_response.SerializeToString()

            # Try to parse as RequestMessage first
            try:
                request = hdf_add_pb2.RequestMessage()
                request.ParseFromString(message_bytes)
                logger.debug(f"Received RequestMessage: sensor_id={request.sensor_id}, base_name={request.base_name}")
                # Generate HTML immediately and store the path
                html_path = self._handle_sensor_data_ready_generate_only(request)
                self._latest_html_path = html_path
                return hdf_add_pb2.ReplyMessage(
                    html_file_path=html_path,
                    status="success",
                    message="CAN KPI HTML generated",
                ).SerializeToString()

            except Exception as e:
                logger.info(f"Not a RequestMessage: {e}")

            # Try to parse as PingMessage
            try:
                ping = hdf_add_pb2.PingMessage()
                ping.ParseFromString(message_bytes)
                logger.debug(f"Received PingMessage: {getattr(ping, 'message_type', 'ping')}")
                # If requester asks for latest HTML path
                if getattr(ping, "message_type", "ping") == "request_html":
                    if self._latest_html_path and os.path.exists(self._latest_html_path):
                        protobuf_response = hdf_add_pb2.ReplyMessage(
                            html_file_path=self._latest_html_path,
                            status="success",
                            message=""
                        )
                    else:
                        protobuf_response = hdf_add_pb2.ReplyMessage(
                            status="error",
                            message="No HTML available yet"
                        )
                    return protobuf_response.SerializeToString()
                else:
                    # Regular ping response
                    pong_response = hdf_add_pb2.PongMessage(
                        status="pong",
                        message="Server is Running"
                    )
                    return pong_response.SerializeToString()
            except Exception as e:
                logger.debug(f"Not a PingMessage: {e}")

            # If neither protobuf format works, try JSON fallback for backward compatibility
            try:
                message_str = message_bytes.decode('utf-8')
                import json
                message = json.loads(message_str)
                logger.debug(f"Falling back to JSON: {message}")
                return self._process_json_message(message)
            except Exception as e:
                logger.debug(f"Not a JSON message: {e}")

            # If all parsing fails, return error
            error_response = hdf_add_pb2.ReplyMessage(
                status="error",
                message="Failed to parse message format"
            )
            return error_response.SerializeToString()

        except Exception as e:
            logger.error(f"Error processing protobuf message: {e}")
            error_response = hdf_add_pb2.ReplyMessage(
                status="error",
                message=str(e)
            )
            return error_response.SerializeToString()

    def _process_json_message(self, message: Dict[str, Any]) -> bytes:
        """Process JSON message for backward compatibility."""
        try:
            message_type_str = message.get("message_type")
            if not message_type_str:
                return hdf_add_pb2.ReplyMessage(status="error", message="Missing message_type").SerializeToString()

            if message_type_str == "ping":
                # Simple JSON ping
                pong = hdf_add_pb2.PongMessage(status="pong", message="Server is running")
                return pong.SerializeToString()

            if message_type_str == "sensor_data_ready":
                sensor_id = message.get("sensor_id")
                data_path = message.get("data_path")
                output_dir = message.get("output_dir")
                base_name = message.get("base_name")
                kpi_subdir = message.get("kpi_subdir", "KPI")

                if not all([sensor_id, data_path, output_dir, base_name]):
                    return hdf_add_pb2.ReplyMessage(
                        status="error",
                        message="Missing required fields: sensor_id, data_path, output_dir, base_name"
                    ).SerializeToString()

                logger.info(f"Processing CAN KPI for sensor {sensor_id}, base {base_name}")
                html_report_path = self._generate_kpi_html(sensor_id, output_dir, base_name, kpi_subdir, data_path)
                return hdf_add_pb2.ReplyMessage(
                    html_file_path=html_report_path,
                    status="success",
                    message=""
                ).SerializeToString()

            return hdf_add_pb2.ReplyMessage(status="error", message=f"Unknown message type: {message_type_str}").SerializeToString()

        except Exception as e:
            logger.error(f"Error processing JSON message: {e}")
            return hdf_add_pb2.ReplyMessage(status="error", message=str(e)).SerializeToString()

    def _handle_sensor_data_ready_generate_only(self, request: Any) -> str:
        """Handle sensor data ready message using protobuf: generate HTML and return its path."""
        sensor_id = request.sensor_id
        input_file_path = request.hdf_file_path
        output_dir = request.output_dir
        base_name = request.base_name
        kpi_subdir = request.kpi_subdir
        output_file_path = request.output_hdf_path

        if not all([sensor_id, input_file_path, output_dir, base_name, kpi_subdir, output_file_path]):
            raise ValueError("Missing required fields: sensor_id, hdf_file_path, output_dir, base_name, kpi_subdir, output_hdf_path")

        logger.info(f"Processing CAN KPI for sensor {sensor_id}, base {base_name}")
        html_report_path = parse_for_can_kpi(
            sensor_id, input_file_path, output_dir, base_name, kpi_subdir, output_file_path
        )
        return html_report_path

    def _generate_kpi_html(self, sensor_id: str, output_dir: str, base_name: str, kpi_subdir: str, data_path: str) -> str:
        """Generate KPI HTML from a JSON-mode request (output file unknown)."""
        if not all([sensor_id, output_dir, base_name, data_path]):
            raise ValueError("Missing required fields: sensor_id, data_path, output_dir, base_name")
        return get_engine().generate_sensor_report(
            sensor_id=sensor_id,
            input_hdf=data_path,
            output_hdf=data_path,
            output_dir=output_dir,
            base_name=base_name,
            kpi_subdir=kpi_subdir,
        )

    def _get_timestamp(self) -> str:
        """Get current timestamp string."""
        from datetime import datetime
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def run_server(port: int):
    server = CanKPIZMQServer(port)
    server.start()


def _write_can_kpi_index(output_dir: str, base_name: str, reports: Dict[str, str]) -> Optional[str]:
    """Write a small index page linking all generated per-sensor CAN KPI pages."""
    if not reports:
        return None
    rows = "".join(
        f'<tr><td>{sensor}</td><td><a href="{Path(rel).as_posix()}">{Path(rel).name}</a></td></tr>'
        for sensor, rel in reports.items()
    )
    html = "\n".join(
        [
            "<!DOCTYPE html>",
            "<html><head>",
            '<meta charset="utf-8"/>',
            '<meta name="viewport" content="width=device-width, initial-scale=1"/>',
            f"<title>CAN KPI — {base_name}</title>",
            "<style>",
            "body{font-family:Segoe UI,Arial,sans-serif;background:#f5f6fa;color:#2c3e50;margin:0;padding:20px;}",
            "h1{color:#2c3e50;}",
            "table{border-collapse:collapse;width:100%;background:#fff;}",
            "th,td{border:1px solid #e8ecef;padding:8px 10px;text-align:left;font-size:13px;}",
            "th{background:#f8fbff;color:#2f4358;}",
            "a{color:#3498db;text-decoration:none;}",
            "</style>",
            "</head><body>",
            f"<h1>CAN KPI Reports — {base_name}</h1>",
            '<table><thead><tr><th>Sensor</th><th>KPI HTML</th></tr></thead>',
            f"<tbody>{rows}</tbody></table>",
            "</body></html>",
        ]
    )
    index_path = Path(output_dir) / base_name / "can_kpi_index.html"
    index_path.write_text("\n".join(html), encoding="utf-8")
    logger.info("Wrote CAN KPI index to %s", index_path)
    return str(index_path)


def run_json_mode(json_path: str, html_output_dir: Optional[str] = None):
    json_path = os.path.abspath(json_path)
    if not os.path.exists(json_path):
        logger.error(f"JSON configuration not found: {json_path}")
        return

    logger.info(f"Processing CAN KPI batches from JSON: {json_path}")
    try:
        with open(json_path, "r", encoding="utf-8") as fp:
            config = json.load(fp)
    except Exception as exc:
        logger.error(f"Failed to read JSON configuration: {exc}")
        return

    inputs = config.get("INPUT_HDF", []) or []
    outputs = config.get("OUTPUT_HDF", []) or []

    if not inputs or not outputs:
        logger.error("JSON configuration must contain non-empty INPUT_HDF and OUTPUT_HDF lists")
        return

    if len(inputs) != len(outputs):
        logger.warning("INPUT_HDF and OUTPUT_HDF lengths differ; processing up to the shortest length")

    total_pairs = min(len(inputs), len(outputs))
    if total_pairs == 0:
        logger.error("No valid input/output pairs to process")
        return

    engine = get_engine()
    for index in range(total_pairs):
        input_path = inputs[index]
        output_path = outputs[index]

        if not os.path.exists(input_path):
            logger.error(f"Input HDF not found: {input_path}")
            continue
        if not os.path.exists(output_path):
            logger.error(f"Output HDF not found: {output_path}")
            continue

        logger.info(f"Processing pair {index + 1}/{total_pairs}")
        try:
            sensors = engine.discover_sensors(input_path, output_path)
        except Exception as exc:
            logger.error(f"Failed to analyze HDF pair: {exc}")
            continue

        if not sensors:
            logger.warning("No sensors discovered for current HDF pair; skipping")
            continue

        base_name = Path(input_path).stem
        output_dir = html_output_dir or os.path.dirname(output_path)
        reports: Dict[str, str] = {}

        for sensor in sensors:
            try:
                html_path = engine.generate_sensor_report(
                    sensor_id=sensor,
                    input_hdf=input_path,
                    output_hdf=output_path,
                    output_dir=output_dir,
                    base_name=base_name,
                    kpi_subdir="KPI",
                )
                if html_path:
                    rel = os.path.relpath(html_path, os.path.join(output_dir, base_name))
                    reports[sensor] = rel
                    logger.info(f"Generated CAN KPI report for sensor {sensor}: {html_path}")
                else:
                    logger.warning(f"CAN KPI report generation returned empty path for sensor {sensor}")
            except Exception as exc:
                logger.error(f"Failed processing sensor {sensor}: {exc}")

        try:
            _write_can_kpi_index(output_dir, base_name, reports)
        except Exception as exc:
            logger.warning(f"Failed writing CAN KPI index: {exc}")

    logger.info("CAN KPI JSON batch processing completed")


def run_hdf_mode(input_hdf: str, output_hdf: str, html_output_dir: Optional[str] = None):
    input_hdf = os.path.abspath(input_hdf)
    output_hdf = os.path.abspath(output_hdf)
    if not os.path.exists(input_hdf):
        logger.error(f"Input HDF not found: {input_hdf}")
        return
    if not os.path.exists(output_hdf):
        logger.error(f"Output HDF not found: {output_hdf}")
        return

    engine = get_engine()
    try:
        sensors = engine.discover_sensors(input_hdf, output_hdf)
    except Exception as exc:
        logger.error(f"Failed to analyze HDF pair: {exc}")
        return

    if not sensors:
        logger.warning("No sensors discovered for HDF pair; skipping")
        return

    base_name = Path(input_hdf).stem
    out_dir = html_output_dir or os.path.dirname(output_hdf)
    reports: Dict[str, str] = {}

    for sensor in sensors:
        try:
            html_path = engine.generate_sensor_report(
                sensor_id=sensor,
                input_hdf=input_hdf,
                output_hdf=output_hdf,
                output_dir=out_dir,
                base_name=base_name,
                kpi_subdir="KPI",
            )
            if html_path:
                rel = os.path.relpath(html_path, os.path.join(out_dir, base_name))
                reports[sensor] = rel
                logger.info(f"Generated CAN KPI report for sensor {sensor}: {html_path}")
            else:
                logger.warning(f"CAN KPI report generation returned empty path for sensor {sensor}")
        except Exception as exc:
            logger.error(f"Failed processing sensor {sensor}: {exc}")

    try:
        _write_can_kpi_index(out_dir, base_name, reports)
    except Exception as exc:
        logger.warning(f"Failed writing CAN KPI index: {exc}")

    logger.info("CAN KPI HDF processing completed")


if __name__ == "__main__":
    argv = sys.argv[1:]

    # Setup logging (allow override via flag/env)
    log_level_name = os.environ.get("CAN_KPI_LOG_LEVEL", "INFO").upper()
    if "--debug" in argv:
        log_level_name = "DEBUG"
        argv = [a for a in argv if a != "--debug"]
    log_level = getattr(logging, log_level_name, logging.INFO)
    logging.basicConfig(
        level=log_level,
        format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
        force=True,
    )
    if not argv:
        logger.info(
            "Usage: can_kpi_server.py zmq [PORT] | can_kpi_server.py CONFIG.json [HTML_DIR] | can_kpi_server.py INPUT.hdf OUTPUT.hdf [HTML_DIR]"
        )
        raise SystemExit(1)

    # ### ZMQ MODE
    if argv[0].lower() == 'zmq':
        port = int(argv[1]) if len(argv) > 1 else 6000
        logger.info(f"Starting CAN KPI ZMQ server on port {port}...")
        try:
            run_server(port)
        except KeyboardInterrupt:
            logger.info("CAN KPI server shutting down.")
        except Exception as e:
            logger.error(f"Server error: {e}")
        raise SystemExit(0)

    # ### JSON MODE
    if argv[0].lower().endswith('.json'):
        json_path = argv[0]
        html_dir = argv[1] if len(argv) > 1 else None
        run_json_mode(json_path, html_output_dir=html_dir)
        raise SystemExit(0)

    # ### HDF MODE
    if len(argv) >= 2:
        input_hdf, output_hdf = argv[0], argv[1]
        html_dir = argv[2] if len(argv) > 2 else None
        run_hdf_mode(input_hdf, output_hdf, html_output_dir=html_dir)
        raise SystemExit(0)

    logger.error("Invalid arguments")
    raise SystemExit(2)
