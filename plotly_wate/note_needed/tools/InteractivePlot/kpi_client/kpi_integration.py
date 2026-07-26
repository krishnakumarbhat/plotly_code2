import logging
import subprocess
import sys
import os
import time
from dataclasses import dataclass
from typing import Optional
import zmq
import platform

# Setup logger
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

# Import protobuf messages (optional)
# Some deployments intentionally omit protobuf; KPI integration should then be disabled.
try:
    import InteractivePlot.kpi_client.hdf_add_pb2 as hdf_add_pb2
except Exception:
    hdf_add_pb2 = None
    logger.debug("KPI protobuf messages unavailable; KPI integration disabled", exc_info=True)

# Get KPI server connection settings from environment variables (for Docker/Singularity)
KPI_SERVER_HOST = os.environ.get('KPI_SERVER_HOST', '127.0.0.1')
KPI_SERVER_PORT = int(os.environ.get('KPI_SERVER_PORT', '5555'))

# -------------------------------
# Dataclasses for messaging (kept for backward compatibility)
# -------------------------------
@dataclass
class RequestMessage:
    """Message sent to KPI server requesting processing."""
    sensor_id: str
    hdf_file_path: str
    output_dir: str
    base_name: str
    kpi_subdir: str = "kpi"
    output_hdf_path: Optional[str] = None

@dataclass
class ReplyMessage:
    """Reply received from KPI server with results or errors."""
    html_file_path: Optional[str] = None
    status: str = "unknown"
    message: Optional[str] = None

# -------------------------------
# KPI Integration Class
# -------------------------------
class kpiIntegration:
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
        self.base_name = base_name
        self.sensor = sensor
        self.input_file = input_file
        self.output_file = output_file
        self.output_dir = output_dir
        # Use environment variables as defaults for Docker/Singularity compatibility
        self.server_host = server_host or KPI_SERVER_HOST
        self.server_port = server_port or KPI_SERVER_PORT
        self._zmq_con = None

        logger.debug(
            f"kpiIntegration initialized with base_name={base_name}, "
            f"sensor={sensor}, input_file={input_file}, "
            f"output_file={output_file}, output_dir={output_dir}, "
            f"server_host={self.server_host}, server_port={self.server_port}"
        )

        # Attempt to ensure the server is running and send data
        if self._ensure_server_process():
            self._send_data_after_initialization()

    # -------------------------------
    # Private helpers
    # -------------------------------
    def _get_socket(self):
        """Create and return a ZeroMQ REQ socket."""
        context = zmq.Context.instance()
        socket = context.socket(zmq.REQ)
        socket.connect(f"tcp://{self.server_host}:{self.server_port}")
        return socket

    def _is_server_responding(self) -> bool:
        """Check if the KPI server responds to a ping message using protobuf."""
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
        """Start KPI server if not responding."""
        if self._is_server_responding():
            logger.info("KPI server already responding")
            return True

        kpi_server_path = os.path.join(
            os.path.dirname(os.path.dirname(os.path.dirname(__file__))),
            "KPI", "kpi_server.py"
        )
        if not os.path.exists(kpi_server_path):
            logger.error(f"KPI server not found at {kpi_server_path}")
            return False

        try:
            if False:
                logger.info("Starting KPI server...")
                if platform.system() == 'Windows':
                    command = f'{sys.executable} "{kpi_server_path}" --port {self.server_port}'
                    subprocess.Popen(f'start cmd /k "{command}"', shell=True)
                elif platform.system() == 'Linux':
                    subprocess.Popen([
                        'gnome-terminal', '--', sys.executable, kpi_server_path,
                        '--port', str(self.server_port)
                    ])
                else:
                    logger.error('Unsupported OS for launching KPI server')
                    return False

            # Wait for server to start
            for _ in range(timeout * 2):
                if self._is_server_responding():
                    logger.info("KPI server started successfully")
                    return True
                time.sleep(0.5)

            logger.warning(f"KPI server not ready after {timeout} seconds")
            return False
        except Exception as e:
            logger.error(f"Failed to start KPI server: {e}")
            return False

    def _send_data_after_initialization(self) -> None:
        """Automatically send data to KPI server after initialization."""
        if hdf_add_pb2 is None:
            return
        request = RequestMessage(
            sensor_id=self.sensor,
            hdf_file_path=self.input_file,
            output_dir=self.output_dir or "",
            base_name=self.base_name,
            kpi_subdir="kpi",
            output_hdf_path=self.output_file
        )
        self.send_data_to_kpi_server(request)

    # -------------------------------
    # Public API
    # -------------------------------
    def send_data_to_kpi_server(self, request: RequestMessage) -> ReplyMessage:
        """Send sensor data processing request to KPI server."""
        if hdf_add_pb2 is None:
            return ReplyMessage(status="disabled", message="KPI protobuf integration not available")
        sock = None
        try:
            sock = self._get_socket()
            protobuf_message = hdf_add_pb2.RequestMessage(
                sensor_id=request.sensor_id,
                hdf_file_path=request.hdf_file_path,
                output_dir=request.output_dir,
                base_name=request.base_name,
                kpi_subdir=request.kpi_subdir,
                output_hdf_path=request.output_hdf_path or "",
                input_file=self.input_file,
                sensor=self.sensor,
                server_port=self.server_port
            )

            logger.info(f"Sending KPI request for sensor {request.sensor_id}")
            sock.send(protobuf_message.SerializeToString())
            logger.info("data send to kpi")
            return "message has been send recive this at html genration"
            # if sock.poll(5000) == 0:
            #     return ReplyMessage(status="error", message="Timeout: No response")

            # response_bytes = sock.recv()
            # protobuf_response = hdf_add_pb2.ReplyMessage()
            # protobuf_response.ParseFromString(response_bytes)
            
        #     logger.info(f"KPI response: {protobuf_response.status}")
        #     return ReplyMessage(
        #         html_file_path=protobuf_response.html_file_path,
        #         status=protobuf_response.status,
        #         message=protobuf_response.message
        #     )
        except Exception as e:
            logger.error(f"Error sending KPI request: {e}")
            return ReplyMessage(status="error", message=str(e))
        # finally:
        #     if sock:
        #         try:
        #             sock.close()
        #         except:
        #             pass
        
    @staticmethod
    def receive_html_path_from_kpi_server(server_host: str = None, server_port: int = None) -> Optional[str]:
        """Request latest generated KPI HTML path from server."""
        if hdf_add_pb2 is None:
            return None
        host = server_host or KPI_SERVER_HOST
        port = server_port or KPI_SERVER_PORT
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
            logger.debug(f"KPI server request failed: {e}")
            return None