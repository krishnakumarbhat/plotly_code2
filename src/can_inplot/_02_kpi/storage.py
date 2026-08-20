"""Canonical per-scan data storage for CAN radar KPI.

Purpose: hold parsed radar payloads in scan-index keyed arrays with typed
stream accessors (HEADER_STREAM / ALIGNMENT_STREAM / DETECTION_STREAM).
Inputs : scan index, sensor id, per-scan signal arrays, detection datasets.
Outputs: storage object consumed by the 02_kpi layer.
"""

import logging
from typing import Any, Dict, List

import numpy as np

logger = logging.getLogger(__name__)


class KPI_DataModelStorage:
    """Scan-index keyed storage with stream parents.

    Detection streams are stored as per-scan variable-length arrays:
    ``get_scan_detections(signals, row, n)`` returns up to ``n`` detection
    records for one scan row.
    """

    def __init__(self) -> None:
        """Purpose: create empty storage container."""
        self.scan_index: np.ndarray = np.array([], dtype=np.int64)
        self.sensor_id: str = ""
        self._time_ns: np.ndarray = np.array([], dtype=np.int64)
        self._streams: Dict[str, Dict[str, np.ndarray]] = {}

    def initialize(self, scan_index: List[int], sensor_id: str) -> None:
        """Purpose: set scan index and sensor id for this storage.
        Inputs : list of scan ids; canonical sensor id.
        Outputs: None (mutates storage)."""
        self.scan_index = np.asarray(scan_index, dtype=np.int64)
        self.sensor_id = sensor_id

    def set_time_ns(self, time_ns: np.ndarray) -> None:
        """Purpose: store per-scan absolute timestamps.
        Inputs : int64 ns timestamp array.
        Outputs: None."""
        self._time_ns = np.asarray(time_ns, dtype=np.int64)

    def get_time_ns(self) -> np.ndarray:
        """Purpose: per-scan absolute timestamps.
        Inputs : none.
        Outputs: int64 ns array."""
        return self._time_ns

    def init_parent(self, stream_name: str) -> None:
        """Purpose: create an empty signal map for a stream parent.
        Inputs : stream name (HEADER_STREAM/ALIGNMENT_STREAM/DETECTION_STREAM).
        Outputs: None."""
        self._streams.setdefault(stream_name, {})

    def set_value(self, value: Any, name: str, stream_name: str) -> None:
        """Purpose: store one signal array under a stream parent.
        Inputs : array, signal name, stream name.
        Outputs: None."""
        self.init_parent(stream_name)
        self._streams[stream_name][name] = value

    def get_scan_index(self) -> np.ndarray:
        """Purpose: per-scan index array.
        Inputs : none.
        Outputs: int64 array."""
        return self.scan_index

    def get_detection_signal_names(self) -> List[str]:
        """Purpose: names of stored detection signals.
        Inputs : none.
        Outputs: sorted list of detection signal names."""
        return sorted(self._streams.get("DETECTION_STREAM", {}).keys())

    def get_signal(self, name: str, stream_name: str = "HEADER_STREAM") -> np.ndarray:
        """Purpose: fetch one signal array by name.
        Inputs : signal name, stream parent name.
        Outputs: ndarray or empty array when missing."""
        arr = self._streams.get(stream_name, {}).get(name)
        if arr is None:
            return np.array([], dtype=np.float64)
        return arr

    def get_valid_detection_counts(self) -> np.ndarray:
        """Purpose: per-scan valid detection counts (HED_NUM_OF_VALID_DETECTIONS).
        Inputs : none.
        Outputs: int64 count array."""
        arr = self.get_signal(
            "HED_NUM_OF_VALID_DETECTIONS", stream_name="HEADER_STREAM"
        )
        if arr.size == 0:
            return np.zeros(len(self.scan_index), dtype=np.int64)
        return np.clip(np.rint(arr).astype(np.int64), 0, None)

    def get_scan_detections(
        self, signals: List[str], row: int, n_det: int
    ) -> List[Dict[str, float]]:
        """Purpose: assemble detection records for one scan row.
        Inputs : signal names, scan row index, number of detections to read.
        Outputs: list of dicts {signal: float} for the scan row."""
        det_stream = self._streams.get("DETECTION_STREAM", {})
        rows: List[Dict[str, float]] = []
        n_valid = max(0, min(int(n_det), max((len(v) for v in det_stream.values() if isinstance(v, (list, tuple))), default=0)))
        for det_idx in range(n_valid):
            rec: Dict[str, float] = {}
            complete = True
            for sig in signals:
                dataset = det_stream.get(sig)
                if not isinstance(dataset, (list, tuple)) or row >= len(dataset):
                    complete = False
                    break
                cell = dataset[row]
                if cell is None or not isinstance(cell, np.ndarray) or det_idx >= len(cell):
                    complete = False
                    break
                rec[sig] = float(cell[det_idx])
            if complete and rec:
                rows.append(rec)
        return rows