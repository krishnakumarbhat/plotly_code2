"""Multi-user safe ZMQ port allocation.

Concurrent CAN/UDP KPI runs on one host must never fight over a fixed
port. Strategy: dedicated base ranges plus incremental probing --
UDP KPI starts at 5555, CAN KPI starts at 6000; both walk upward only
within their own range (default attempts=200 keeps UDP <= 5755), so an
incrementing UDP server can never collide with a CAN one.
"""
import logging
import socket

import zmq

logger = logging.getLogger(__name__)


def port_in_use(port: int, host: str = "127.0.0.1") -> bool:
    """Return True if a TCP server is already accepting on host:port."""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.settimeout(0.2)
        return sock.connect_ex((host, int(port))) == 0


def find_free_port(start: int = 5555, attempts: int = 200, host: str = "127.0.0.1") -> int:
    """Return the first port >= start with no listener, else raise RuntimeError."""
    port = int(start)
    for _ in range(attempts):
        if not port_in_use(port, host):
            return port
        port += 1
    raise RuntimeError(f"No free port found from {start} after {attempts} attempts")


def bind_with_retry(socket: "zmq.Socket", start_port: int, attempts: int = 200) -> int:
    """Bind a ZMQ socket to tcp://*:<port>, incrementing the port on
    AddressInUse so simultaneous users never collide. Returns bound port."""
    port = int(start_port)
    for attempt in range(attempts):
        try:
            socket.bind(f"tcp://*:{port}")
            if port != int(start_port):
                logger.warning(
                    "ZMQ port %s busy (attempt %s); bound to incremented port %s",
                    start_port, attempt + 1, port,
                )
            return port
        except zmq.ZMQError as exc:
            if exc.errno == zmq.EADDRINUSE:
                port += 1
                continue
            raise
    raise RuntimeError(f"No bindable ZMQ port found from {start_port} after {attempts} attempts")
