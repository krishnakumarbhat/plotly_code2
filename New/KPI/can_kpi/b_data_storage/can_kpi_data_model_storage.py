from typing import Any, Dict, List, Optional

import numpy as np


class KPI_DataModelStorage:
    """Scan-index keyed hierarchical storage used by HDF parser."""

    def __init__(self):
        self._value_to_signal: Dict[str, str] = {}
        self._signal_to_value: Dict[str, Any] = {}
        self._data_container: Dict[int, List[List[Any]]] = {}
        self._signal_values: Dict[str, Any] = {}
        self._signal_stream: Dict[str, str] = {}
        self._scan_index = np.array([], dtype=np.int64)
        self._time_ns = np.array([], dtype=np.int64)
        # CAN-bus timestamps: header (per-scan) + per-DET-group (per-scan).
        # Empty when the producer did not write timestamp_* payloads.
        self._hdr_can_t = np.array([], dtype=np.float64)
        self._det_can_t: Dict[str, np.ndarray] = {}
        self._parent_counter = -1
        self._child_counter = -1
        self.stream_name = ""

    def initialize(self, scan_index, sensor, missing_idx=None) -> None:
        _ = sensor
        _ = missing_idx
        self._scan_index = np.asarray(scan_index, dtype=np.int64)
        self._data_container = {int(scan): [] for scan in scan_index}

    def set_time_ns(self, time_ns: np.ndarray) -> None:
        if isinstance(time_ns, np.ndarray) and time_ns.size > 0:
            n = min(len(self._scan_index), len(time_ns))
            self._time_ns = np.asarray(np.rint(time_ns[:n]), dtype=np.int64)
            if n < len(self._scan_index):
                pad = np.full(len(self._scan_index) - n, -1, dtype=np.int64)
                self._time_ns = np.concatenate([self._time_ns, pad])
        else:
            self._time_ns = np.array([], dtype=np.int64)

    def init_parent(self, stream_name: str) -> None:
        self._parent_counter += 1
        self._child_counter = -1
        self.stream_name = stream_name

    def set_value(self, dataset: Any, signal_name: str, grp_name: str) -> str:
        self._signal_values[signal_name] = dataset
        self._signal_stream[signal_name] = grp_name

        dataset_len = len(dataset) if dataset is not None else 0
        if dataset_len != len(self._data_container):
            return ""

        is_new_parent = (
            grp_name not in self._signal_to_value and self._child_counter == -1
        )
        if is_new_parent:
            key_grp = f"{self._parent_counter}_None"
            self._child_counter = 0
            key = f"{self._parent_counter}_{self._child_counter}"
            for row, scanidx in zip(dataset, self._data_container):
                self._data_container[scanidx].append([self._norm_row(row)])
            self._value_to_signal[key] = signal_name
            self._value_to_signal[key_grp] = self.stream_name
            self._signal_to_value[signal_name] = key
            self._signal_to_value[self.stream_name] = key_grp
            return key

        self._child_counter += 1
        key = f"{self._parent_counter}_{self._child_counter}"
        for row, scanidx in zip(dataset, self._data_container):
            self._data_container[scanidx][-1].append(self._norm_row(row))

        self._value_to_signal[key] = signal_name
        current = self._signal_to_value.get(signal_name)
        if current is None:
            self._signal_to_value[signal_name] = [{grp_name: key}]
        elif isinstance(current, list):
            current.append({grp_name: key})
        else:
            self._signal_to_value[signal_name] = [{grp_name: key}]
        return key

    def clear(self) -> None:
        self._value_to_signal.clear()
        self._signal_to_value.clear()
        self._data_container.clear()
        self._signal_values.clear()
        self._signal_stream.clear()
        self._scan_index = np.array([], dtype=np.int64)
        self._time_ns = np.array([], dtype=np.int64)
        self._hdr_can_t = np.array([], dtype=np.float64)
        self._det_can_t = {}
        self._parent_counter = -1
        self._child_counter = -1

    def get_scan_index(self) -> np.ndarray:
        return self._scan_index.copy()

    def get_time_ns(self) -> np.ndarray:
        return self._time_ns.copy()

    def set_hdr_can_t(self, hdr_can_t: np.ndarray) -> None:
        """Per-scan header CAN timestamp (float seconds). Truncated to scans."""
        if isinstance(hdr_can_t, np.ndarray) and hdr_can_t.size > 0:
            n = min(len(self._scan_index), len(hdr_can_t))
            self._hdr_can_t = np.asarray(hdr_can_t[:n], dtype=np.float64)
            if n < len(self._scan_index):
                pad = np.full(len(self._scan_index) - n, np.nan, dtype=np.float64)
                self._hdr_can_t = np.concatenate([self._hdr_can_t, pad])
        else:
            self._hdr_can_t = np.array([], dtype=np.float64)

    def get_hdr_can_t(self) -> np.ndarray:
        return self._hdr_can_t.copy()

    def set_det_can_t(self, det_can_t: Dict[str, np.ndarray]) -> None:
        """Per-DET-group CAN timestamps keyed by group name. Truncated to scans."""
        self._det_can_t = {}
        if not isinstance(det_can_t, dict):
            return
        n_scans = len(self._scan_index)
        for grp, arr in det_can_t.items():
            if isinstance(arr, np.ndarray) and arr.size > 0:
                a = np.asarray(arr[:n_scans], dtype=np.float64)
                if len(a) < n_scans:
                    a = np.concatenate(
                        [a, np.full(n_scans - len(a), np.nan, dtype=np.float64)]
                    )
                self._det_can_t[str(grp)] = a

    def get_det_can_t(self) -> Dict[str, np.ndarray]:
        return {k: v.copy() for k, v in self._det_can_t.items()}

    def get_signal(self, signal_name: str) -> Any:
        return self._signal_values.get(signal_name)

    def get_detection_signal_names(self) -> List[str]:
        return sorted(
            [
                sig
                for sig, stream in self._signal_stream.items()
                if stream == "DETECTION_STREAM"
            ]
        )

    def get_header_signal(self, signal_name: str) -> np.ndarray:
        sig = self._signal_values.get(signal_name)
        return sig if isinstance(sig, np.ndarray) else np.array([], dtype=np.float64)

    def get_valid_detection_counts(self) -> np.ndarray:
        vals = self.get_header_signal("HED_NUM_OF_VALID_DETECTIONS")
        if vals.size == 0:
            return np.zeros(len(self._scan_index), dtype=np.int64)
        n = min(len(vals), len(self._scan_index))
        out = np.zeros(len(self._scan_index), dtype=np.int64)
        out[:n] = np.clip(np.rint(vals[:n]).astype(np.int64), 0, None)
        return out

    def get_detection_rows(self, signal_name: str) -> List[Any]:
        rows = self._signal_values.get(signal_name)
        return rows if isinstance(rows, list) else []

    def get_detection_2d(self, signal_name: str) -> tuple[np.ndarray, np.ndarray]:
        scan_index = self.get_scan_index()
        rows = self.get_detection_rows(signal_name)
        if len(scan_index) == 0 or not rows:
            return scan_index, np.empty((len(scan_index), 0), dtype=np.float64)

        values: List[np.ndarray] = []
        for idx in range(min(len(scan_index), len(rows))):
            row = rows[idx]
            if isinstance(row, np.ndarray):
                values.append(np.asarray(row, dtype=np.float64))
            else:
                values.append(np.array([], dtype=np.float64))

        width = max((len(v) for v in values), default=0)
        if width <= 0:
            return scan_index[: len(values)], np.empty(
                (len(values), 0), dtype=np.float64
            )

        out = np.full((len(values), width), np.nan, dtype=np.float64)
        for row_idx, row_vals in enumerate(values):
            if len(row_vals):
                out[row_idx, : len(row_vals)] = row_vals
        return scan_index[: len(values)], out

    def get_scan_detections(
        self,
        signal_names: List[str],
        row_idx: int,
        max_det: int,
        det_idx_shifts: Optional[Dict[int, int]] = None,
    ) -> List[Dict[str, float]]:
        """Detections for one scan row.

        ``det_idx_shifts`` maps detector-group index (``det_pos // 4``) to a
        row delta applied when reading that group's values. It compensates
        producers that pair header rows with detection payloads from a
        neighbouring scan (see ``KpiHdfParser.align_storage_rows_triple``).
        ``None``/empty preserves the legacy exact-row behaviour.
        """
        if max_det <= 0:
            return []

        rows_by_sig: Dict[str, Any] = {
            sig: self.get_detection_rows(sig) for sig in signal_names
        }
        detections: List[Dict[str, float]] = []
        shifts = det_idx_shifts or {}

        for det_pos in range(max_det):
            ridx = row_idx + int(shifts.get(det_pos // 4, 0))
            values: Dict[str, float] = {}
            valid = True
            for sig in signal_names:
                sig_rows = rows_by_sig.get(sig, [])
                if ridx < 0 or ridx >= len(sig_rows):
                    valid = False
                    break
                row = sig_rows[ridx]
                if not isinstance(row, np.ndarray) or det_pos >= len(row):
                    valid = False
                    break
                val = float(row[det_pos])
                if np.isnan(val):
                    valid = False
                    break
                values[sig] = val

            if not valid:
                continue
            if values.get("DET_RANGE", 0.0) == 0.0:
                continue
            detections.append(values)

        return detections

    def _norm_row(self, row: Any) -> Any:
        if isinstance(row, np.ndarray):
            return np.round(row.astype(float), 2)
        return row
