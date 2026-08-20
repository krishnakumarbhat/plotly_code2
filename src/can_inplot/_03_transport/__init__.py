"""Layer 03_transport — ZeroMQ message framing and distribution.

Purpose: high-throughput IPC transport for the pipeline: JSON envelope frames
with raw numpy payload parts (zero-copy ``tobytes``), PUSH/PULL pipelines and
PUB/SUB telemetry broadcast.
Inputs : frame dicts and numpy arrays from pipeline stages.
Outputs: serialized multipart ZMQ messages and sockets.
"""

from can_inplot._03_transport.frames import (
    Frame,
    FrameCodec,
    MessageType,
    encode_frame,
    decode_frame,
)
from can_inplot._03_transport.broker import PipelineBroker, TelemetryPublisher
from can_inplot._03_transport.server import run_server, CanKPIZMQServer

__all__ = [
    "Frame",
    "FrameCodec",
    "MessageType",
    "encode_frame",
    "decode_frame",
    "PipelineBroker",
    "TelemetryPublisher",
    "run_server",
    "CanKPIZMQServer",
]