"""HDF parser that transforms raw HDF payloads into scan-index keyed storage.

Purpose: parse a CAN radar HDF5 file into per-sensor KPI storage objects with
a parse report (parsed/skipped sensors, warnings, errors).
Inputs : HDF5 file path; optional allowed detection signal prefixes.
Outputs: dict {sensor_id: {"storage": ..., "scan_index": ..., "time_ns": ...}}.
"""

import logging
from typing import Any, Dict, Iterable, List, Optional, Set

import numpy as np

from can_inplot._01_ingest.hdf_reader import HdfAttrReader
from can_inplot._01_ingest.schema import DETECTION_SIGNALS
from can_inplot._02_kpi.storage import KPI_DataModelStorage

logger = logging.getLogger(__name__)


class KpiHdfParser:
    """Parse CAN KPI HDF files and store values in ``KPI_DataModelStorage``."""

    def __init__(
        self,
        reader: Optional[HdfAttrReader] = None,
        required_detection_signals: Optional[Iterable[str]] = None,
    ) -> None:
        """Purpose: configure the parser.
        Inputs : optional reader and required detection signal prefixes.
        Outputs: parser instance."""
        self._reader = reader or HdfAttrReader()
        self._required_detection_signals = list(
            required_detection_signals or DETECTION_SIGNALS
        )
        self._required_detection_set: Set[str] = set(
            self._required_detection_signals
        )
        self._last_parse_report: Dict[str, Any] = self._new_parse_report("")

    def get_last_parse_report(self) -> Dict[str, Any]:
        """Purpose: report from the most recent parse.
        Inputs : none.
        Outputs: parse report dict."""
        report = self._last_parse_report or self._new_parse_report("")
        return {
            "path": report.get("path", ""),
            "status": report.get("status", "ok"),
            "parsed_sensors": list(report.get("parsed_sensors", [])),
            "skipped_sensors": list(report.get("skipped_sensors", [])),
            "warnings": list(report.get("warnings", [])),
            "errors": list(report.get("errors", [])),
            "sensor_scan_counts": dict(report.get("sensor_scan_counts", {})),
        }

    def parse_file(self, hdf_path: str) -> Dict[str, Any]:
        """Purpose: parse one HDF file into sensor-wise scan-index structures.
        Inputs : HDF5 path.
        Outputs: dict of sensor id -> payload with storage and scan metadata."""
        report = self._new_parse_report(hdf_path)
        if not hdf_path:
            report["status"] = "error"
            report["errors"].append("HDF was unable to parse: path was not provided.")
            self._last_parse_report = report
            return {}

        try:
            raw = self._reader.read_hdf_attrs(hdf_path)
        except Exception as exc:
            logger.exception("Failed to read HDF file %s: %s", hdf_path, exc)
            report["status"] = "error"
            report["errors"].append(f"HDF was unable to parse: {exc}")
            self._last_parse_report = report
            return {}

        parsed: Dict[str, Any] = {}
        if not raw:
            report["status"] = "error"
            report["errors"].append(
                "HDF was unable to parse: no sensor groups were found in this file."
            )
            self._last_parse_report = report
            return parsed

        for sensor_id, sensor_data in raw.items():
            try:
                scan_index = self._reader.get_scan_index(sensor_data)
                if not isinstance(scan_index, np.ndarray) or scan_index.size == 0:
                    msg = (
                        "skipped because no HED_SCAN_INDEX/HED_LOOK_INDEX and no "
                        "real detection arrays were found"
                    )
                    logger.warning("Skip sensor %s in %s: %s", sensor_id, hdf_path, msg)
                    report["warnings"].append(f"{sensor_id}: {msg}.")
                    self._append_unique(report["skipped_sensors"], sensor_id)
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
                valid_cnt = self._extract_valid_detection_count(
                    header, len(scan_index)
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
                    "storage": storage,
                    "header": header,
                    "alignment": alignment,
                    "detection": detection,
                }
                self._append_unique(report["parsed_sensors"], sensor_id)
                report["sensor_scan_counts"][sensor_id] = int(len(scan_index))
            except Exception as exc:
                logger.exception(
                    "Failed parsing sensor %s in %s: %s", sensor_id, hdf_path, exc
                )
                report["errors"].append(
                    f"{sensor_id}: HDF was unable to parse sensor payload: {exc}"
                )
                self._append_unique(report["skipped_sensors"], sensor_id)

        if not parsed:
            report["status"] = "error"
            if not report["errors"]:
                report["errors"].append(
                    "HDF was unable to parse: no supported sensor payloads were extracted."
                )
        elif report["warnings"] or report["errors"]:
            report["status"] = "partial"

        self._last_parse_report = report
        return parsed

    def extract_storages(self, parsed: Dict[str, Dict[str, Any]]) -> Dict[str, Any]:
        """Purpose: pull storage objects out of parsed payloads.
        Inputs : parsed payload dict from parse_file.
        Outputs: dict of sensor id -> KPI_DataModelStorage."""
        out: Dict[str, Any] = {}
        for sensor_id, payload in (parsed or {}).items():
            if isinstance(payload, dict) and payload.get("storage") is not None:
                out[sensor_id] = payload["storage"]
        return out

    def align_storage_rows_by_scanindex(
        self,
        input_storage: KPI_DataModelStorage,
        output_storage: KPI_DataModelStorage,
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        """Purpose: align two storages by equal scan index values.
        Inputs : input and output storage objects.
        Outputs: (common scan values, input rows, output rows)."""
        in_scan = input_storage.get_scan_index()
        out_scan = output_storage.get_scan_index()
        if len(in_scan) == 0 or len(out_scan) == 0:
            empty = np.array([], dtype=np.int64)
            return empty, empty, empty
        return self._align_scan_only(in_scan, out_scan)

    def _align_scan_only(
        self, in_scan: np.ndarray, out_scan: np.ndarray
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        """Purpose: align two scan arrays by matching ids (duplicates allowed).
        Inputs : two int64 scan arrays.
        Outputs: (common values, input rows, output rows)."""
        out_positions: Dict[int, List[int]] = {}
        for out_idx, scan in enumerate(out_scan):
            out_positions.setdefault(int(scan), []).append(out_idx)
        common_vals: List[int] = []
        in_rows: List[int] = []
        out_rows: List[int] = []
        for in_idx, scan in enumerate(in_scan):
            pos_list = out_positions.get(int(scan))
            if not pos_list:
                continue
            out_rows.append(pos_list.pop(0))
            common_vals.append(int(scan))
            in_rows.append(in_idx)
        return (
            np.asarray(common_vals, dtype=np.int64),
            np.asarray(in_rows, dtype=np.int64),
            np.asarray(out_rows, dtype=np.int64),
        )

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
        """Purpose: assemble one KPI storage from parsed signal maps.
        Inputs : per-sensor parsed structures.
        Outputs: KPI_DataModelStorage with stream parents populated."""
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

    def _build_detection_dataset(
        self,
        det_idx_map: Dict[int, np.ndarray],
        row_count: int,
        valid_detection_count: np.ndarray,
    ) -> List[np.ndarray]:
        """Purpose: convert per-detection-index arrays into per-scan rows.
        Inputs : {det_idx: per-scan array}, number of rows, valid counts.
        Outputs: list of per-scan float arrays (variable length)."""
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
        """Purpose: placeholder rows for absent signals.
        Inputs : row count.
        Outputs: list of None rows."""
        return [None for _ in range(row_count)]

    def _ordered_detection_signals(
        self, detection_signals: Dict[str, Dict[int, np.ndarray]]
    ) -> List[str]:
        """Purpose: deterministic detection signal ordering.
        Inputs : detection signal map.
        Outputs: ordered signal names (configured first, extras sorted)."""
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
        """Purpose: per-scan valid detection counts from header.
        Inputs : header signal map, expected scan count.
        Outputs: clipped int64 counts."""
        values = header_signals.get("HED_NUM_OF_VALID_DETECTIONS")
        if isinstance(values, np.ndarray) and len(values) >= scan_count:
            out = np.rint(values[:scan_count]).astype(np.int64)
            return np.clip(out, 0, None)
        return np.zeros(scan_count, dtype=np.int64)

    def _is_per_scan(self, arr: Any, scan_count: int) -> bool:
        """Purpose: test whether an array is per-scan length.
        Inputs : candidate array, scan count.
        Outputs: True when the array covers all scans."""
        return isinstance(arr, np.ndarray) and len(arr) >= scan_count

    def _new_parse_report(self, hdf_path: str) -> Dict[str, Any]:
        """Purpose: fresh parse report template.
        Inputs : HDF path.
        Outputs: report dict."""
        return {
            "path": hdf_path or "",
            "status": "ok",
            "parsed_sensors": [],
            "skipped_sensors": [],
            "warnings": [],
            "errors": [],
            "sensor_scan_counts": {},
        }

    def _append_unique(self, values: List[str], value: str) -> None:
        """Purpose: append value once.
        Inputs : list, value.
        Outputs: None."""
        if value not in values:
            values.append(value)