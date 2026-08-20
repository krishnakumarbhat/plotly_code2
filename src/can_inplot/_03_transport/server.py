"""CAN KPI ZMQ server (unified transport entry).

Purpose: REP/REQ server accepting per-sensor KPI requests over ZMQ (JSON
fallback supported), plus batch modes (json / hdf) mirroring the legacy
UDP_KPI kpi_server CLI contract.
Inputs : ZMQ port or JSON config / HDF pair paths.
Outputs: KPI HTML report paths.
"""

import json
import logging
import os
import sys
from pathlib import Path
from typing import Any, Dict, Optional

import zmq

from can_inplot._03_transport.frames import Frame, MessageType, FrameCodec

logger = logging.getLogger(__name__)

DEFAULT_PORT = 5556


class CanKPIZMQServer:
    """REP socket server: parses JSON or Frame requests and runs KPI generation."""

    def __init__(self, port: int = DEFAULT_PORT, engine=None) -> None:
        """Purpose: build the server.
        Inputs : port and optional KPI engine (callable(sensor, input, output,
                outdir, base, kpi_subdir) -> html path).
        Outputs: server instance."""
        self.port = port
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self._running = False
        self._latest_html_path: Optional[str] = None
        self._engine = engine or self._default_engine

    def start(self) -> None:
        """Purpose: run the REP loop.
        Inputs : none.
        Outputs: None (blocking)."""
        try:
            self.socket.bind(f"tcp://*:{self.port}")
            self._running = True
            logger.info("CAN KPI ZMQ server started on port %s", self.port)
            while self._running:
                try:
                    parts = self.socket.recv_multipart()
                    response = self._process_frame(parts)
                    self.socket.send_multipart(FrameCodec.encode(response))
                except zmq.ZMQError as exc:
                    if self._running:
                        logger.error("ZMQ error: %s", exc)
                    break
        except Exception as exc:
            logger.error("Failed to start server: %s", exc)
        finally:
            self.stop()

    def stop(self) -> None:
        """Purpose: stop the loop and close sockets.
        Inputs : none.
        Outputs: None."""
        self._running = False
        if hasattr(self, "socket"):
            self.socket.close()
        if hasattr(self, "context"):
            self.context.term()
        logger.info("CAN KPI ZMQ server stopped")

    def _process_frame(self, parts) -> Frame:
        """Purpose: dispatch one request frame.
        Inputs : multipart parts.
        Outputs: response Frame."""
        try:
            frame = FrameCodec.decode(parts)
        except Exception:
            frame = self._fallback_json_frame(parts)
        if frame.msg_type == MessageType.PING:
            return Frame(msg_type=MessageType.PONG, body={"status": "pong"})
        if frame.msg_type == MessageType.KPI_REQUEST:
            return self._handle_kpi_request(frame.body)
        return Frame(
            msg_type=MessageType.KPI_RESULT,
            body={"status": "error", "message": f"Unknown type: {frame.msg_type}"},
        )

    def _handle_kpi_request(self, body: Dict[str, Any]) -> Frame:
        """Purpose: generate KPI HTML for a sensor request.
        Inputs : request body (sensor_id, hdf_file_path, output_hdf_path,
                output_dir, base_name, kpi_subdir).
        Outputs: result Frame with html_file_path."""
        sensor_id = body.get("sensor_id")
        input_path = body.get("hdf_file_path") or body.get("data_path")
        output_path = body.get("output_hdf_path") or input_path
        output_dir = body.get("output_dir")
        base_name = body.get("base_name")
        kpi_subdir = body.get("kpi_subdir", "KPI")
        if not all([sensor_id, input_path, output_dir, base_name]):
            return Frame(
                msg_type=MessageType.KPI_RESULT,
                body={
                    "status": "error",
                    "message": "Missing required fields: sensor_id, hdf_file_path, output_dir, base_name",
                },
            )
        try:
            html_path = self._engine(
                sensor_id, input_path, output_path, output_dir, base_name, kpi_subdir
            )
            self._latest_html_path = html_path
            return Frame(
                msg_type=MessageType.KPI_RESULT,
                body={"status": "success", "html_file_path": html_path},
            )
        except Exception as exc:
            logger.exception("KPI generation failed: %s", exc)
            return Frame(
                msg_type=MessageType.KPI_RESULT,
                body={"status": "error", "message": str(exc)},
            )

    def _fallback_json_frame(self, parts) -> Frame:
        """Purpose: decode legacy single-part JSON requests.
        Inputs : multipart parts.
        Outputs: Frame."""
        raw = parts[0].decode("utf-8")
        message = json.loads(raw)
        message_type = message.get("message_type")
        if message_type == "ping":
            return Frame(msg_type=MessageType.PING)
        if message_type == "sensor_data_ready":
            return Frame(
                msg_type=MessageType.KPI_REQUEST,
                body={
                    "sensor_id": message.get("sensor_id"),
                    "data_path": message.get("data_path"),
                    "output_dir": message.get("output_dir"),
                    "base_name": message.get("base_name"),
                    "kpi_subdir": message.get("kpi_subdir", "KPI"),
                },
            )
        return Frame(
            msg_type=MessageType.KPI_REQUEST,
            body={"status": "error", "message": f"Unknown JSON type: {message_type}"},
        )

    def _default_engine(self, sensor_id, input_path, output_path, output_dir, base_name, kpi_subdir) -> str:
        """Purpose: placeholder engine; real engine injected by 00_main.
        Inputs : request fields.
        Outputs: HTML path (raises when no engine wired)."""
        raise NotImplementedError(
            "No KPI engine wired; pass engine= to CanKPIZMQServer"
        )


def run_server(port: int, engine=None) -> None:
    """Purpose: CLI entry for ZMQ mode.
    Inputs : port, optional engine.
    Outputs: None (blocking)."""
    CanKPIZMQServer(port, engine=engine).start()


def run_json_mode(json_path: str, html_output_dir: Optional[str] = None, engine=None) -> None:
    """Purpose: batch KPI generation from a JSON config.
    Inputs : config path, optional output dir, optional engine.
    Outputs: None."""
    json_path = os.path.abspath(json_path)
    if not os.path.exists(json_path):
        logger.error("JSON configuration not found: %s", json_path)
        return
    with open(json_path, "r", encoding="utf-8") as fp:
        config = json.load(fp)
    inputs = config.get("INPUT_HDF", []) or []
    outputs = config.get("OUTPUT_HDF", []) or []
    if not inputs or not outputs:
        logger.error("JSON configuration must contain non-empty INPUT_HDF and OUTPUT_HDF lists")
        return
    total_pairs = min(len(inputs), len(outputs))
    engine = engine or _default_engine()
    for index in range(total_pairs):
        input_path, output_path = inputs[index], outputs[index]
        if not os.path.exists(input_path) or not os.path.exists(output_path):
            logger.error("HDF pair %d missing; skipping", index + 1)
            continue
        sensors = engine.discover_sensors(input_path, output_path)
        if not sensors:
            logger.warning("No sensors discovered for pair %d", index + 1)
            continue
        base_name = Path(input_path).stem
        out_dir = html_output_dir or os.path.dirname(output_path)
        for sensor in sensors:
            try:
                engine.generate_sensor_report(
                    sensor, input_path, output_path, out_dir, base_name, "KPI"
                )
            except Exception as exc:
                logger.error("Failed processing sensor %s: %s", sensor, exc)
    logger.info("CAN KPI JSON batch processing completed")


def run_hdf_mode(input_hdf: str, output_hdf: str, html_output_dir: Optional[str] = None, engine=None) -> None:
    """Purpose: batch KPI generation for a single HDF pair.
    Inputs : HDF paths, optional output dir, optional engine.
    Outputs: None."""
    if not os.path.exists(input_hdf) or not os.path.exists(output_hdf):
        logger.error("Input or output HDF not found")
        return
    engine = engine or _default_engine()
    sensors = engine.discover_sensors(input_hdf, output_hdf)
    if not sensors:
        logger.warning("No sensors discovered for HDF pair")
        return
    base_name = Path(input_hdf).stem
    out_dir = html_output_dir or os.path.dirname(output_hdf)
    for sensor in sensors:
        try:
            engine.generate_sensor_report(
                sensor, input_hdf, output_hdf, out_dir, base_name, "KPI"
            )
        except Exception as exc:
            logger.error("Failed processing sensor %s: %s", sensor, exc)
    logger.info("CAN KPI HDF processing completed")


def _default_engine():
    """Purpose: wire the real KPI engine lazily.
    Inputs : none.
    Outputs: engine object with discover_sensors/generate_sensor_report."""
    from can_inplot._05_visual.html_gen import CanKpiEngine

    return CanKpiEngine()


if __name__ == "__main__":
    argv = sys.argv[1:]
    logging.basicConfig(
        level=os.environ.get("CAN_KPI_LOG_LEVEL", "INFO").upper(),
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
        force=True,
    )
    if not argv:
        print("Usage: python -m can_inplot._03_transport.server zmq [PORT] | CONFIG.json [HTML_DIR] | INPUT.hdf OUTPUT.hdf [HTML_DIR]")
        raise SystemExit(1)
    if argv[0].lower() == "zmq":
        port = int(argv[1]) if len(argv) > 1 else DEFAULT_PORT
        run_server(port)
    elif argv[0].lower().endswith(".json"):
        run_json_mode(argv[0], argv[1] if len(argv) > 1 else None)
    elif len(argv) >= 2:
        run_hdf_mode(argv[0], argv[1], argv[2] if len(argv) > 2 else None)
    else:
        raise SystemExit(2)