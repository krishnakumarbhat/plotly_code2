"""
KPI Integration Module
Basic ZMQ-based bridge that receives sensor notifications, writes a dummy KPI HTML,
and returns the HTML path for use by the HTML generator.
"""

import logging
import os
import json
import sys
from pathlib import Path
from typing import Dict, Any, Optional
import zmq

from KPI.a_persistence_layer.hdf_wrapper import parse_for_kpi 
from InteractivePlot.b_persistence_layer.prerun_hdf_parser import PreRun

# Lazily import protobuf definitions only when running ZMQ mode
hdf_add_pb2 = None

def _ensure_pb():
    global hdf_add_pb2
    if hdf_add_pb2 is None:
        import sys
        sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'InteractivePlot', 'kpi_client'))
        import InteractivePlot.kpi_client.hdf_add_pb2 as _pb
        hdf_add_pb2 = _pb

logger = logging.getLogger(__name__)

class KPIZMQServer:
    """ZMQ server for handling KPI processing requests using protobuf."""
    
    def __init__(self, port: int = 5555):
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
            self.socket.bind(f"tcp://*:{self.port}")
            self._running = True
            logger.info(f"KPI ZMQ server started on port {self.port}")
            
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
        logger.info("KPI ZMQ server stopped")
    
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
                # Do not send any payload back now; html_generator will request later
                return b""
            
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
                kpi_subdir = message.get("kpi_subdir", "kpi")

                if not all([sensor_id, data_path, output_dir, base_name]):
                    return hdf_add_pb2.ReplyMessage(
                        status="error",
                        message="Missing required fields: sensor_id, data_path, output_dir, base_name"
                    ).SerializeToString()

                logger.info(f"Processing KPI for sensor {sensor_id}, base {base_name}")
                html_report_path = self._generate_kpi_html(sensor_id, output_dir, base_name, kpi_subdir)
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
        """Handle sensor data ready message using protobuf: generate HTML and return its path.

        This method does not craft a direct response for the requester beyond ACK; it is used
        to prepare the HTML for later retrieval by html_generator.py.
        """
        sensor_id = request.sensor_id
        input_file_path = request.hdf_file_path
        output_dir = request.output_dir
        base_name = request.base_name
        kpi_subdir = request.kpi_subdir
        output_file_path = request.output_hdf_path
        
        if not all([sensor_id, input_file_path, output_dir, base_name, kpi_subdir, output_file_path]):
            raise ValueError("Missing required fields: sensor_id, hdf_file_path, output_dir, base_name, kpi_subdir, output_hdf_path")
        
        logger.info(f"Processing KPI for sensor {sensor_id}, base {base_name}")
        html_report_path = parse_for_kpi(
            sensor_id, input_file_path, output_dir, base_name, kpi_subdir, output_file_path
        )
        return html_report_path
    
    def _get_timestamp(self) -> str:
        """Get current timestamp string."""
        from datetime import datetime
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def run_server(port: int):
    server = KPIZMQServer(port)
    server.start()


def run_json_mode(json_path: str, html_output_dir: Optional[str] = None):
    json_path = os.path.abspath(json_path)
    if not os.path.exists(json_path):
        logger.error(f"JSON configuration not found: {json_path}")
        return

    logger.info(f"Processing KPI batches from JSON: {json_path}")
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
            prerun = PreRun(input_path, output_path)
            sensors = prerun.sensor_list or []
        except Exception as exc:
            logger.error(f"Failed to analyze HDF pair: {exc}")
            continue

        if not sensors:
            logger.warning("No sensors discovered for current HDF pair; skipping")
            continue

        base_name = Path(input_path).stem
        output_dir = html_output_dir or os.path.dirname(output_path)
        if output_dir:
            os.makedirs(output_dir, exist_ok=True)

        for sensor in sensors:
            try:
                html_path = parse_for_kpi(
                    sensor,
                    input_path,
                    output_dir,
                    base_name,
                    "kpi",
                    output_path,
                )
                if html_path:
                    logger.info(f"Generated KPI report for sensor {sensor}: {html_path}")
                else:
                    logger.warning(f"KPI report generation returned empty path for sensor {sensor}")
            except Exception as exc:
                logger.error(f"Failed processing sensor {sensor}: {exc}")

    logger.info("JSON batch processing completed")


def run_hdf_mode(input_hdf: str, output_hdf: str, html_output_dir: Optional[str] = None):
    input_hdf = os.path.abspath(input_hdf)
    output_hdf = os.path.abspath(output_hdf)
    if not os.path.exists(input_hdf):
        logger.error(f"Input HDF not found: {input_hdf}")
        return
    if not os.path.exists(output_hdf):
        logger.error(f"Output HDF not found: {output_hdf}")
        return

    try:
        sensors = (PreRun(input_hdf, output_hdf).sensor_list) or []
    except Exception as exc:
        logger.error(f"Failed to analyze HDF pair: {exc}")
        return

    if not sensors:
        logger.warning("No sensors discovered for HDF pair; skipping")
        return

    base_name = Path(input_hdf).stem
    out_dir = html_output_dir or os.path.dirname(output_hdf)
    if out_dir:
        os.makedirs(out_dir, exist_ok=True)

    for sensor in sensors:
        try:
            html_path = parse_for_kpi(sensor, input_hdf, out_dir, base_name, "kpi", output_hdf)
            if html_path:
                logger.info(f"Generated KPI report for sensor {sensor}: {html_path}")
            else:
                logger.warning(f"KPI report generation returned empty path for sensor {sensor}")
        except Exception as exc:
            logger.error(f"Failed processing sensor {sensor}: {exc}")

    logger.info("HDF processing completed")


if __name__ == "__main__":
    argv = sys.argv[1:]

    # Setup logging (allow override via flag/env)
    # Examples:
    #   python kpi_server.py --debug kpi.json
    #   set KPI_LOG_LEVEL=DEBUG && python kpi_server.py kpi.json
    log_level_name = os.environ.get("KPI_LOG_LEVEL", "INFO").upper()
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
            "Usage: kpi_server.py zmq [PORT] | kpi_server.py CONFIG.json [HTML_DIR] | kpi_server.py INPUT.hdf OUTPUT.hdf [HTML_DIR]"
        )
        raise SystemExit(1)

    # ### ZMQ MODE
    if argv[0].lower() == 'zmq':
        port = int(argv[1]) if len(argv) > 1 else 5555
        logger.info(f"Starting KPI ZMQ server on port {port}...")
        try:
            run_server(port)
        except KeyboardInterrupt:
            logger.info("KPI server shutting down.")
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