"""HDF parser that transforms raw HDF attributes into scan-index keyed storage."""

import logging
from typing import Any, Dict, Iterable, List, Optional, Set

import numpy as np

from a_persistence_layer.hdf_wrapper import HdfAttrReader
from b_data_storage.can_kpi_data_model_storage import KPI_DataModelStorage

logger = logging.getLogger(__name__)


class KpiHdfParser:
    """Parse CAN KPI HDF files and store values in ``KPI_DataModelStorage``.

    Output format per sensor:
    {
            "scan_index": ndarray,
            "scan_dict": {scan_idx: {"header": {...}, "alignment": {...}, "detection": {...}}},
            "storage": KPI_DataModelStorage,
            "header": {...},
            "alignment": {...},
            "detection": {signal_prefix: {det_index: ndarray}}
    }
    """

    def __init__(
        self,
        reader: Optional[HdfAttrReader] = None,
        required_detection_signals: Optional[Iterable[str]] = None,
    ):
        self._reader = reader or HdfAttrReader()
        default_signals = [
            "DET_RANGE",
            "DET_RANGE_VELOCITY",
            "DET_AZIMUTH",
            "DET_ELEVATION",
            "DET_RCS",
            "DET_SNR",
        ]
        self._required_detection_signals = list(
            required_detection_signals or default_signals
        )
        self._required_detection_set: Set[str] = set(self._required_detection_signals)

    def parse_file(self, hdf_path: str) -> Dict[str, Any]:
        """Parse one HDF file into sensor-wise scan-index keyed structures."""
        if not hdf_path:
            return {}

        try:
            raw = self._reader.read_hdf_attrs(hdf_path)
        except Exception as exc:
            logger.exception(f"Failed to read HDF file {hdf_path}: {exc}")
            return {}

        parsed: Dict[str, Any] = {}

        for sensor_id, sensor_data in raw.items():
            try:
                scan_index = self._reader.get_scan_index(sensor_data)
                if not isinstance(scan_index, np.ndarray) or scan_index.size == 0:
                    logger.debug(f"Skip sensor {sensor_id}: no scan index")
                    continue

                scan_index = np.rint(scan_index).astype(np.int64)
                header = self._reader.extract_header_signals(sensor_data)
                alignment = self._reader.extract_alignment_signals(sensor_data)
                detection = self._reader.extract_detection_signals(
                    sensor_data, allowed_prefixes=self._required_detection_set
                )
                time_ns = self._reader.get_absolute_time_ns(
                    sensor_data=sensor_data,
                    header_signals=header,
                    scan_count=len(scan_index),
                )

                valid_cnt = self._extract_valid_detection_count(header, len(scan_index))
                scan_dict = self._build_scan_dict(
                    scan_index=scan_index,
                    header_signals=header,
                    alignment_signals=alignment,
                    detection_signals=detection,
                    valid_detection_count=valid_cnt,
                )

                storage = self._build_storage(
                    sensor_id=sensor_id,
                    scan_index=scan_index,
                    time_ns=time_ns,
                    header_signals=header,
                    alignment_signals=alignment,
                    detection_signals=detection,
                    valid_detection_count=valid_cnt,
                )

                parsed[sensor_id] = {
                    "friendly_name": sensor_data.get("friendly_name", sensor_id),
                    "scan_index": scan_index,
                    "time_ns": time_ns,
                    "scan_dict": scan_dict,
                    "storage": storage,
                    "header": header,
                    "alignment": alignment,
                    "detection": detection,
                }
            except Exception as exc:
                logger.exception(
                    f"Failed parsing sensor {sensor_id} in {hdf_path}: {exc}"
                )

        return parsed

    def extract_storages(self, parsed: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
        return self._reader._extract_storages(parsed)

    def _build_storage(
        self,
        sensor_id: str,
        scan_index: np.ndarray,
        time_ns: np.ndarray,
        header_signals: Dict[str, np.ndarray],
        alignment_signals: Dict[str, np.ndarray],
        detection_signals: Dict[str, Dict[int, np.ndarray]],
        valid_detection_count: np.ndarray,
    ) -> KPI_DataModelStorage:
        storage = KPI_DataModelStorage()
        storage.initialize(scan_index.tolist(), sensor_id)
        storage.set_time_ns(time_ns)

        if header_signals:
            storage.init_parent("HEADER_STREAM")
            for name, arr in header_signals.items():
                if self._is_per_scan(arr, len(scan_index)):
                    storage.set_value(arr, name, "HEADER_STREAM")

        if alignment_signals:
            storage.init_parent("ALIGNMENT_STREAM")
            for name, arr in alignment_signals.items():
                if self._is_per_scan(arr, len(scan_index)):
                    storage.set_value(arr, name, "ALIGNMENT_STREAM")

        storage.init_parent("DETECTION_STREAM")
        for signal_name in self._ordered_detection_signals(detection_signals):
            det_idx_map = detection_signals.get(signal_name)
            if isinstance(det_idx_map, dict) and det_idx_map:
                dataset = self._build_detection_dataset(
                    det_idx_map=det_idx_map,
                    row_count=len(scan_index),
                    valid_detection_count=valid_detection_count,
                )
            else:
                dataset = self._build_missing_dataset(len(scan_index))
            storage.set_value(dataset, signal_name, "DETECTION_STREAM")

        return storage

    def align_storage_rows(
        self,
        input_storage: KPI_DataModelStorage,
        output_storage: KPI_DataModelStorage,
        time_tolerance_ns: int = 2_000_000,
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        in_scan = input_storage.get_scan_index()
        out_scan = output_storage.get_scan_index()
        in_time = input_storage.get_time_ns()
        out_time = output_storage.get_time_ns()

        if len(in_scan) == 0 or len(out_scan) == 0:
            empty = np.array([], dtype=np.int64)
            return empty, empty, empty

        if len(in_time) == 0 or len(out_time) == 0:
            return self._align_scan_only(in_scan, out_scan)

        n_in = min(len(in_scan), len(in_time))
        n_out = min(len(out_scan), len(out_time))
        in_scan = in_scan[:n_in]
        out_scan = out_scan[:n_out]
        in_time = in_time[:n_in]
        out_time = out_time[:n_out]

        common_vals: List[int] = []
        in_rows: List[int] = []
        out_rows: List[int] = []

        i = 0
        j = 0
        tol = int(time_tolerance_ns)
        while i < n_in and j < n_out:
            dt = int(in_time[i]) - int(out_time[j])
            if abs(dt) <= tol:
                common_vals.append(int(in_scan[i]))
                in_rows.append(i)
                out_rows.append(j)
                i += 1
                j += 1
            elif dt < 0:
                i += 1
            else:
                j += 1

        return (
            np.asarray(common_vals, dtype=np.int64),
            np.asarray(in_rows, dtype=np.int64),
            np.asarray(out_rows, dtype=np.int64),
        )

    def align_storage_rows_by_scanindex(
        self,
        input_storage: KPI_DataModelStorage,
        output_storage: KPI_DataModelStorage,
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        in_scan = input_storage.get_scan_index()
        out_scan = output_storage.get_scan_index()

        if len(in_scan) == 0 or len(out_scan) == 0:
            empty = np.array([], dtype=np.int64)
            return empty, empty, empty

        return self._align_scan_only(in_scan, out_scan)

    def _align_scan_only(
        self, in_scan: np.ndarray, out_scan: np.ndarray
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        out_positions: Dict[int, List[int]] = {}
        for out_idx, scan in enumerate(out_scan):
            key = int(scan)
            out_positions.setdefault(key, []).append(out_idx)

        common_vals: List[int] = []
        in_rows: List[int] = []
        out_rows: List[int] = []

        for in_idx, scan in enumerate(in_scan):
            key = int(scan)
            pos_list = out_positions.get(key)
            if not pos_list:
                continue
            out_idx = pos_list.pop(0)
            common_vals.append(key)
            in_rows.append(in_idx)
            out_rows.append(out_idx)

        return (
            np.asarray(common_vals, dtype=np.int64),
            np.asarray(in_rows, dtype=np.int64),
            np.asarray(out_rows, dtype=np.int64),
        )

    def _build_scan_dict(
        self,
        scan_index: np.ndarray,
        header_signals: Dict[str, np.ndarray],
        alignment_signals: Dict[str, np.ndarray],
        detection_signals: Dict[str, Dict[int, np.ndarray]],
        valid_detection_count: np.ndarray,
    ) -> Dict[int, Dict[str, Any]]:
        scan_dict: Dict[int, Dict[str, Any]] = {}

        for row, raw_scan in enumerate(scan_index):
            scan = int(raw_scan)
            scan_dict[scan] = {
                "header": self._pick_row_values(header_signals, row),
                "alignment": self._pick_row_values(alignment_signals, row),
                "detection": {},
            }

        for signal_name in self._ordered_detection_signals(detection_signals):
            det_idx_map = detection_signals.get(signal_name)
            if isinstance(det_idx_map, dict) and det_idx_map:
                rows = self._build_detection_dataset(
                    det_idx_map=det_idx_map,
                    row_count=len(scan_index),
                    valid_detection_count=valid_detection_count,
                )
            else:
                rows = self._build_missing_dataset(len(scan_index))
            for row, raw_scan in enumerate(scan_index):
                scan = int(raw_scan)
                scan_dict[scan]["detection"][signal_name] = rows[row]

        return scan_dict

    def _build_detection_dataset(
        self,
        det_idx_map: Dict[int, np.ndarray],
        row_count: int,
        valid_detection_count: np.ndarray,
    ) -> List[np.ndarray]:
        if row_count <= 0:
            return []

        max_det = max(det_idx_map.keys(), default=0)
        rows: List[np.ndarray] = []

        for row in range(row_count):
            n_valid = (
                int(valid_detection_count[row])
                if row < len(valid_detection_count)
                else max_det
            )
            n_valid = max(0, min(n_valid, max_det))
            values: List[float] = []

            for det_idx in range(1, n_valid + 1):
                arr = det_idx_map.get(det_idx)
                if arr is None or row >= len(arr):
                    continue
                values.append(float(arr[row]))

            rows.append(np.asarray(values, dtype=np.float64))

        return rows

    def _build_missing_dataset(self, row_count: int) -> List[Any]:
        return [None for _ in range(row_count)]

    def _ordered_detection_signals(
        self, detection_signals: Dict[str, Dict[int, np.ndarray]]
    ) -> List[str]:
        out: List[str] = []
        for sig in self._required_detection_signals:
            out.append(sig)
        for sig in sorted(detection_signals.keys()):
            if sig not in out:
                out.append(sig)
        return out

    def _extract_valid_detection_count(
        self, header_signals: Dict[str, np.ndarray], scan_count: int
    ) -> np.ndarray:
        values = header_signals.get("HED_NUM_OF_VALID_DETECTIONS")
        if isinstance(values, np.ndarray) and len(values) >= scan_count:
            out = np.rint(values[:scan_count]).astype(np.int64)
            return np.clip(out, 0, None)
        return np.zeros(scan_count, dtype=np.int64)

    def _pick_row_values(
        self, signals: Dict[str, np.ndarray], row: int
    ) -> Dict[str, Any]:
        row_data: Dict[str, Any] = {}
        for key, arr in signals.items():
            if not isinstance(arr, np.ndarray) or row >= len(arr):
                continue
            val = arr[row]
            if isinstance(val, np.ndarray):
                row_data[key] = val
            elif np.isscalar(val):
                row_data[key] = (
                    float(val) if np.issubdtype(type(val), np.number) else val
                )
            else:
                row_data[key] = val
        return row_data

    def _is_per_scan(self, arr: Any, scan_count: int) -> bool:
        return isinstance(arr, np.ndarray) and len(arr) >= scan_count
