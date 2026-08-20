"""ZMQ pipeline broker and telemetry publisher.

Purpose: PUSH/PULL worker pipeline with a PUB/SUB telemetry side-channel.
The broker binds a PULL frontend and a PUB telemetry socket; workers connect
via PUSH and push frames; subscribers receive frame-rate and latency telemetry.
Inputs : frames from pipeline stages.
Outputs: distribution sockets with throughput accounting.
"""

import logging
import time
from typing import Dict

import zmq

from can_inplot._03_transport.frames import Frame, FrameCodec

logger = logging.getLogger(__name__)


class PipelineBroker:
    """Front-end broker: PULL from producers, PUB telemetry to subscribers."""

    def __init__(self, pull_port: int = 5560, pub_port: int = 5561) -> None:
        """Purpose: bind broker sockets.
        Inputs : PULL port and PUB telemetry port.
        Outputs: broker instance (sockets bound)."""
        self.pull_port = pull_port
        self.pub_port = pub_port
        self.context = zmq.Context()
        self.pull = self.context.socket(zmq.PULL)
        self.pull.bind(f"tcp://*:{self.pull_port}")
        self.pub = self.context.socket(zmq.PUB)
        self.pub.bind(f"tcp://*:{self.pub_port}")
        self._running = False
        self.stats: Dict[str, float] = {
            "frames": 0.0,
            "bytes": 0.0,
            "start": time.time(),
            "last": time.time(),
        }

    def serve(self, handler=None) -> None:
        """Purpose: pump frames until stopped.
        Inputs : optional callable handler(frame) -> Frame or None.
        Outputs: None (blocking loop)."""
        self._running = True
        poller = zmq.Poller()
        poller.register(self.pull, zmq.POLLIN)
        while self._running:
            events = dict(poller.poll(250))
            if self.pull not in events:
                continue
            parts = self.pull.recv_multipart()
            frame = FrameCodec.decode(parts)
            self._record(len(parts))
            result = handler(frame) if handler else frame
            if result is not None:
                self.pub.send_multipart(
                    [b"telemetry", *FrameCodec.encode(result)]
                )

    def _record(self, part_count: int) -> None:
        """Purpose: update throughput stats.
        Inputs : number of parts received.
        Outputs: None."""
        now = time.time()
        self.stats["frames"] += 1.0
        self.stats["bytes"] += float(part_count) * 256.0
        self.stats["last"] = now

    def fps(self) -> float:
        """Purpose: measured frame rate.
        Inputs : none.
        Outputs: frames per second since start."""
        dt = time.time() - self.stats["start"]
        return self.stats["frames"] / dt if dt > 0 else 0.0

    def stop(self) -> None:
        """Purpose: tear down sockets.
        Inputs : none.
        Outputs: None."""
        self._running = False
        self.pull.close()
        self.pub.close()
        self.context.term()


class TelemetryPublisher:
    """Worker-side PUSH socket used by pipeline stages."""

    def __init__(self, pull_port: int = 5560) -> None:
        """Purpose: connect a PUSH socket to the broker PULL.
        Inputs : broker PULL port.
        Outputs: publisher instance."""
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PUSH)
        self.socket.connect(f"tcp://127.0.0.1:{pull_port}")

    def send(self, frame: Frame) -> None:
        """Purpose: push one frame to the broker.
        Inputs : Frame.
        Outputs: None."""
        self.socket.send_multipart(FrameCodec.encode(frame))

    def close(self) -> None:
        """Purpose: close socket and context.
        Inputs : none.
        Outputs: None."""
        self.socket.close()
        self.context.term()