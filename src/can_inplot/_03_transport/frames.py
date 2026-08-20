"""Zero-copy message framing.

Purpose: define the wire format for pipeline frames: a JSON envelope first part
plus raw numpy payload parts. Payloads travel as ``tobytes`` views so receivers
rebuild arrays without re-encoding through the envelope.
Inputs : python dicts and numpy arrays.
Outputs: lists of bytes (multipart frames) and back.
"""

import json
import time
from dataclasses import dataclass, field
from enum import Enum
from typing import Any, Dict, List, Optional, Tuple

import numpy as np


class MessageType(str, Enum):
    """Message kinds routed over the ZMQ pipeline."""

    PING = "ping"
    PONG = "pong"
    SENSOR_READY = "sensor_data_ready"
    KPI_REQUEST = "kpi_request"
    KPI_RESULT = "kpi_result"
    TELEMETRY = "telemetry"
    LOG = "log"


@dataclass
class Frame:
    """One pipeline message: envelope + optional numpy payloads."""

    msg_type: str
    body: Dict[str, Any] = field(default_factory=dict)
    arrays: List[np.ndarray] = field(default_factory=list)
    timestamp: float = field(default_factory=time.time)


class FrameCodec:
    """Serialize/deserialize frames to ZMQ multipart byte lists."""

    @staticmethod
    def encode(frame: Frame) -> List[bytes]:
        """Purpose: convert a frame into multipart bytes.
        Inputs : Frame object.
        Outputs: list of byte parts (envelope first, arrays after)."""
        parts: List[bytes] = [
            json.dumps(
                {
                    "type": frame.msg_type,
                    "body": frame.body,
                    "timestamp": frame.timestamp,
                    "n_arrays": len(frame.arrays),
                },
                separators=(",", ":"),
            ).encode("utf-8")
        ]
        for arr in frame.arrays:
            parts.append(arr.astype(np.float64, copy=False).tobytes())
        return parts

    @staticmethod
    def decode(parts: List[bytes], shapes: Optional[List[Tuple[int, ...]]] = None) -> Frame:
        """Purpose: rebuild a frame from multipart bytes.
        Inputs : multipart byte list; optional explicit array shapes.
        Outputs: Frame object with payload arrays reconstructed."""
        envelope = json.loads(parts[0].decode("utf-8"))
        arrays: List[np.ndarray] = []
        for i, part in enumerate(parts[1:]):
            if shapes is not None and i < len(shapes):
                n = int(np.prod(shapes[i])) if shapes[i] else 0
                arrays.append(
                    np.frombuffer(part, dtype=np.float64, count=n).reshape(shapes[i])
                )
            else:
                arrays.append(np.frombuffer(part, dtype=np.float64))
        return Frame(
            msg_type=envelope["type"],
            body=envelope.get("body", {}),
            arrays=arrays,
            timestamp=envelope.get("timestamp", time.time()),
        )


def encode_frame(frame: Frame) -> List[bytes]:
    """Purpose: convenience wrapper for FrameCodec.encode.
    Inputs : Frame.
    Outputs: multipart bytes."""
    return FrameCodec.encode(frame)


def decode_frame(parts: List[bytes], shapes: Optional[List[Tuple[int, ...]]] = None) -> Frame:
    """Purpose: convenience wrapper for FrameCodec.decode.
    Inputs : multipart bytes, optional shapes.
    Outputs: Frame."""
    return FrameCodec.decode(parts, shapes=shapes)