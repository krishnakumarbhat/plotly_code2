import logging
import subprocess
import sys
import os
import time
from dataclasses import dataclass
from typing import Optional
import zmq
import platform
from can_inplot.d_business_layer.path_utils import normalize_fs_path, set_default_umask

# Compatibility for older generated *_pb2.py against newer protobuf runtimes.
os.environ.setdefault('PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION', 'python')

# Setup logger
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

# Import protobuf messages (optional)
# Some deployments intentionally omit protobuf; KPI integration should then be disabled.
try:
    import can_inplot.kpi_client.hdf_add_pb2 as hdf_add_pb2
except Exception:
    hdf_add_pb2 = None
    logger.debug("KPI protobuf messages unavailable; KPI integration disabled", exc_info=True)

# Get CAN KPI server connection settings from environment variables (for Docker/Singularity)
CAN_KPI_SERVER_HOST = os.environ.get('CAN_KPI_SERVER_HOST', '127.0.0.1')
CAN_KPI_SERVER_PORT = int(os.environ.get('CAN_KPI_SERVER_PORT', '6000'))
CAN_KPI_SERVER_RESPONSE_TIMEOUT_MS = max(
    1000,
    int(os.environ.get('CAN_KPI_SERVER_RESPONSE_TIMEOUT_MS', '180000')),
)

# -------------------------------
# Dataclasses for messaging (kept for backward compatibility)
# -------------------------------
@dataclass
class RequestMessage:
    """Message sent to CAN KPI server requesting processing."""
    sensor_id: str
    hdf_file_path: str
    output_dir: str
    base_name: str
    kpi_subdir: str = "KPI"
    output_hdf_path: Optional[str] = None

@dataclass
class ReplyMessage:
    """Reply received from CAN KPI server with results or errors."""
    html_file_path: Optional[str] = None
    status: str = "unknown"
    message: Optional[str] = None

# -------------------------------
# CAN KPI Integration Class
# -------------------------------
class CanKpiIntegration:
    def __init__(
        self,
        base_name: str,
        sensor: str,
        input_file: str,
        output_file: str,
        output_dir: Optional[str] = None,
        server_host: str = None,
        server_port: int = None
    ):
        set_default_umask()
        self.base_name = base_name
        self.sensor = sensor
        self.input_file = normalize_fs_path(input_file)
        self.output_file = normalize_fs_path(output_file)
        self.output_dir = normalize_fs_path(output_dir) if output_dir else None
        # Use environment variables as defaults for Docker/Singularity compatibility
        self.server_host = server_host or CAN_KPI_SERVER_HOST
        self.server_port = server_port or CAN_KPI_SERVER_PORT
        self._zmq_con = None
        self.last_reply: Optional[ReplyMessage] = None

        logger.debug(
            f"CanKpiIntegration initialized with base_name={base_name}, "
            f"sensor={sensor}, input_file={input_file}, "
            f"output_file={output_file}, output_dir={output_dir}, "
            f"server_host={self.server_host}, server_port={self.server_port}"
        )

        # Attempt to ensure the CAN KPI server is running and send data
        if self._ensure_server_process():
            self._send_data_after_initialization()

    # -------------------------------
    # Private helpers
    # -------------------------------
    def _get_socket(self):
        """Create and return a ZeroMQ REQ socket."""
        context = zmq.Context.instance()
        socket = context.socket(zmq.REQ)
        socket.setsockopt(zmq.LINGER, 0)
        socket.connect(f"tcp://{self.server_host}:{self.server_port}")
        return socket

    def _is_server_responding(self) -> bool:
        """Check if the CAN KPI server responds to a ping message using protobuf."""
        if hdf_add_pb2 is None:
            return False
        try:
            sock = self._get_socket()

            # Create and send protobuf ping message
            ping_message = hdf_add_pb2.PingMessage(message_type="ping")
            sock.send(ping_message.SerializeToString())

            if sock.poll(1000) == 0:  # Timeout: 1 second
                sock.close()
                return False

            # Receive protobuf response
            response_bytes = sock.recv()
            pong_response = hdf_add_pb2.PongMessage()
            pong_response.ParseFromString(response_bytes)

            sock.close()
            # Check for the correct ping response format
            return pong_response.status == "pong"

        except Exception as e:
            logger.debug(f"Ping check failed: {e}")
            return False

    def _ensure_server_process(self, timeout: int = 5) -> bool:
        """Start CAN KPI server if not responding."""
        if self._is_server_responding():
            logger.info("CAN KPI server already responding")
            return True

        # The server lives in the CAN KPI tool folder: <KPI>/can_interactive_plot/can_kpi_server.py
        kpi_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        candidates = [
            os.path.join(kpi_root, "can_interactive_plot", "can_kpi_server.py"),
            os.path.join(kpi_root, "can_kpi_server.py"),
        ]
        can_kpi_server_path = next((p for p in candidates if os.path.exists(p)), candidates[0])
        if not os.path.exists(can_kpi_server_path):
            logger.error(f"CAN KPI server not found at {can_kpi_server_path}")
            return False

        try:
            # Never bind-spawn onto a busy port: pick the next free one so
            # simultaneous users each get their own CAN KPI server.
            from can_inplot.kpi_client.port_utils import find_free_port
            self.server_port = find_free_port(self.server_port)
            if platform.system() == 'Windows':
                # Launch silently (no popup console window).
                try:
                    creationflags = subprocess.CREATE_NO_WINDOW
                except AttributeError:
                    creationflags = 0
                subprocess.Popen(
                    [sys.executable, can_kpi_server_path, 'zmq', str(self.server_port)],
                    creationflags=creationflags,
                    close_fds=True,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL,
                )
            elif platform.system() == 'Linux':
                subprocess.Popen(
                    [sys.executable, can_kpi_server_path, 'zmq', str(self.server_port)],
                    stdin=subprocess.DEVNULL,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL,
                    close_fds=True,
                    start_new_session=True,
                )
            else:
                logger.error('Unsupported OS for launching CAN KPI server')
                return False

            # Wait for server to start
            for _ in range(timeout * 2):
                if self._is_server_responding():
                    logger.info("CAN KPI server started successfully")
                    return True
                time.sleep(0.5)

            logger.warning(f"CAN KPI server not ready after {timeout} seconds")
            return False
        except Exception as e:
            logger.error(f"Failed to start CAN KPI server: {e}")
            return False

    def _send_data_after_initialization(self) -> None:
        """Automatically send data to CAN KPI server after initialization."""
        if hdf_add_pb2 is None:
            return
        request = RequestMessage(
            sensor_id=self.sensor,
            hdf_file_path=self.input_file,
            output_dir=self.output_dir or "",
            base_name=self.base_name,
            kpi_subdir="KPI",
            output_hdf_path=self.output_file
        )
        self.last_reply = self.send_data_to_kpi_server(request)

    # -------------------------------
    # Public API
    # -------------------------------
    def send_data_to_kpi_server(self, request: RequestMessage) -> ReplyMessage:
        """Send sensor data processing request to the CAN KPI server."""
        if hdf_add_pb2 is None:
            return ReplyMessage(status="disabled", message="KPI protobuf integration not available")
        sock = None
        try:
            sock = self._get_socket()
            protobuf_message = hdf_add_pb2.RequestMessage(
                sensor_id=request.sensor_id,
                hdf_file_path=normalize_fs_path(request.hdf_file_path),
                output_dir=normalize_fs_path(request.output_dir),
                base_name=request.base_name,
                kpi_subdir=request.kpi_subdir,
                output_hdf_path=normalize_fs_path(request.output_hdf_path or ""),
                input_file=self.input_file,
                sensor=self.sensor,
                server_port=self.server_port
            )

            logger.info(f"Sending CAN KPI request for sensor {request.sensor_id}")
            sock.send(protobuf_message.SerializeToString())
            if sock.poll(CAN_KPI_SERVER_RESPONSE_TIMEOUT_MS) == 0:
                logger.warning(
                    "Timed out waiting %s ms for CAN KPI response sensor=%s base=%s",
                    CAN_KPI_SERVER_RESPONSE_TIMEOUT_MS,
                    request.sensor_id,
                    request.base_name,
                )
                return ReplyMessage(status="error", message="Timeout: No response from CAN KPI server")

            response_bytes = sock.recv()
            protobuf_response = hdf_add_pb2.ReplyMessage()
            protobuf_response.ParseFromString(response_bytes)
            logger.info(f"CAN KPI response: {protobuf_response.status}")
            return ReplyMessage(
                html_file_path=protobuf_response.html_file_path,
                status=protobuf_response.status,
                message=protobuf_response.message
            )
        except Exception as e:
            logger.error(f"Error sending CAN KPI request: {e}")
            return ReplyMessage(status="error", message=str(e))
        finally:
            if sock:
                try:
                    sock.close()
                except Exception:
                    pass

    @staticmethod
    def receive_html_path_from_can_kpi_server(server_host: str = None, server_port: int = None) -> Optional[str]:
        """Request latest generated CAN KPI HTML path from server."""
        if hdf_add_pb2 is None:
            return None
        host = server_host or CAN_KPI_SERVER_HOST
        port = server_port or CAN_KPI_SERVER_PORT
        try:
            context = zmq.Context.instance()
            sock = context.socket(zmq.REQ)
            sock.connect(f"tcp://{host}:{port}")

            ping = hdf_add_pb2.PingMessage(message_type="request_html")
            sock.send(ping.SerializeToString())

            if sock.poll(2000) == 0:
                sock.close()
                return None

            response_bytes = sock.recv()
            reply = hdf_add_pb2.ReplyMessage()
            reply.ParseFromString(response_bytes)
            sock.close()

            if reply.status == "success" and reply.html_file_path:
                return reply.html_file_path
            return None
        except Exception as e:
            logger.debug(f"CAN KPI server request failed: {e}")
            return None
