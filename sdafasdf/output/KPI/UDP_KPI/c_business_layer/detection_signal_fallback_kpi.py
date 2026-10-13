import logging
from itertools import product
from typing import Dict, Iterable, List

import numpy as np

from UDP_KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
from UDP_KPI.c_business_layer.detection_matching_kpi import DetectionMappingKPIHDF

logger = logging.getLogger(__name__)


class DetectionSignalFallbackKPIHDF(DetectionMappingKPIHDF):
    """Fallback detection KPI that matches detections directly on AF signals."""

    MATCH_SIGNALS = ("ran", "vel", "theta", "phi")

    def __init__(self, data: Dict[str, KPI_DataModelStorage], sensor_id: str):
        super().__init__(data, sensor_id)
        self._match_offsets = sorted(
            list(product([-1, 0, 1], repeat=4)),
            key=lambda off: abs(off[0]) + abs(off[1]) + abs(off[2]) + abs(off[3]),
        )
        self.kpi_results["matching_mode"] = "Signal-only fallback"
        self.kpi_results["denominator_label"] = "Detections Used"

    def process_signal_matching(self) -> bool:
        detection_stream = self.data.get("DETECTION_STREAM") if isinstance(self.data, dict) else None
        if not isinstance(detection_stream, dict):
            logger.warning("DETECTION_STREAM is unavailable for fallback detection KPI")
            return False

        input_storage = detection_stream.get("input")
        output_storage = detection_stream.get("output")
        if input_storage is None or output_storage is None:
            logger.warning("Fallback detection KPI requires both input and output detection storage")
            return False

        payloads = self._load_signal_payloads(input_storage, output_storage)
        missing_signals = [
            signal for signal in self.MATCH_SIGNALS if not isinstance(payloads.get(signal), dict)
        ]
        if missing_signals:
            logger.warning(
                "Fallback detection KPI missing required detection signals for %s: %s",
                self.sensor_id,
                missing_signals,
            )
            return False

        scan_indices = self._collect_scan_indices(payloads.values())
        if not scan_indices:
            logger.warning("Fallback detection KPI found no comparable scan indices for %s", self.sensor_id)
            return False

        input_counts = self._extract_declared_counts(payloads.get("num_af_det"), side="input")
        output_counts = self._extract_declared_counts(payloads.get("num_af_det"), side="output")

        total_matches = 0
        total_detections = 0
        per_scan_accuracy: List[float | None] = []
        per_scan_scanindex: List[int] = []
        per_scan_matches: List[int] = []
        per_scan_den: List[int] = []
        af_det_counts: List[int] = []
        af_det_scanindex: List[int] = []

        for scan_idx in scan_indices:
            veh_points, veh_requested = self._build_scan_points(
                payloads, scan_idx, side="input", declared_count=input_counts.get(scan_idx)
            )
            sim_points, sim_requested = self._build_scan_points(
                payloads, scan_idx, side="output", declared_count=output_counts.get(scan_idx)
            )

            denominator = max(veh_requested, sim_requested, len(veh_points), len(sim_points))
            matches = self._match_points(veh_points, sim_points)
            accuracy = (matches / denominator) * 100.0 if denominator > 0 else None

            total_matches += matches
            total_detections += denominator
            per_scan_accuracy.append(accuracy)
            per_scan_scanindex.append(scan_idx)
            per_scan_matches.append(int(matches))
            per_scan_den.append(int(denominator))
            af_det_counts.append(int(veh_requested))
            af_det_scanindex.append(scan_idx)

            logger.debug(
                "Fallback scan %s for %s: input_points=%s output_points=%s denominator=%s matches=%s",
                scan_idx,
                self.sensor_id,
                len(veh_points),
                len(sim_points),
                denominator,
                matches,
            )

        valid_acc_values = [value for value in per_scan_accuracy if value is not None]
        detection_accuracy = (
            (total_matches / total_detections) * 100.0 if total_detections > 0 else None
        )

        self.kpi_results["per_scan_accuracy"] = per_scan_accuracy
        self.kpi_results["per_scan_scanindex"] = per_scan_scanindex
        self.kpi_results["per_scan_matches"] = per_scan_matches
        self.kpi_results["per_scan_den"] = per_scan_den
        self.kpi_results["af_det_counts"] = af_det_counts
        self.kpi_results["af_det_scanindex"] = af_det_scanindex
        self.kpi_results["veh_si_count"] = int(len(input_counts) or len(per_scan_scanindex))
        self.kpi_results["sim_si_count"] = int(len(output_counts) or len(per_scan_scanindex))
        self.kpi_results["scans_processed"] = int(len(per_scan_scanindex))
        self.kpi_results["scans_with_matches"] = int(sum(1 for value in per_scan_matches if value > 0))
        self.kpi_results["min_accuracy"] = min(valid_acc_values) if valid_acc_values else None
        self.kpi_results["max_accuracy"] = max(valid_acc_values) if valid_acc_values else None
        self.kpi_results["matching_accuracy"] = {
            "matches": int(total_matches),
            "total_detections": int(total_detections),
            "detection_accuracy_percentage": detection_accuracy,
        }

        logger.info(
            "Fallback detection KPI completed for %s: matches=%s total=%s accuracy=%s",
            self.sensor_id,
            total_matches,
            total_detections,
            "N/A" if detection_accuracy is None else f"{detection_accuracy:.2f}%",
        )

        self.generate_html_report()
        return True

    def _load_signal_payloads(
        self,
        input_storage: KPI_DataModelStorage,
        output_storage: KPI_DataModelStorage,
    ) -> Dict[str, dict]:
        payloads: Dict[str, dict] = {}
        for signal_name in (*self.MATCH_SIGNALS, "num_af_det"):
            payload = KPI_DataModelStorage.get_data(input_storage, output_storage, signal_name)
            if isinstance(payload, dict):
                payloads[signal_name] = payload
            else:
                logger.debug(
                    "Fallback detection KPI signal %s unavailable for %s: %s",
                    signal_name,
                    self.sensor_id,
                    payload,
                )
        return payloads

    def _collect_scan_indices(self, payloads: Iterable[dict]) -> List[int]:
        scan_indices = set()
        for payload in payloads:
            scan_indices.update(payload.get("scan_input_counts", {}).keys())
            scan_indices.update(payload.get("scan_output_counts", {}).keys())
        return sorted(int(scan_idx) for scan_idx in scan_indices)

    def _extract_declared_counts(self, payload: dict | None, side: str) -> Dict[int, int]:
        if not isinstance(payload, dict):
            return {}

        values_key = "scan_input_values" if side == "input" else "scan_output_values"
        counts: Dict[int, int] = {}
        for scan_idx, values in payload.get(values_key, {}).items():
            declared = 0
            if values:
                try:
                    declared = int(max(0, round(float(np.asarray(values).ravel()[0]))))
                except Exception:
                    declared = 0
            counts[int(scan_idx)] = declared
        return counts

    def _build_scan_points(
        self,
        payloads: Dict[str, dict],
        scan_idx: int,
        side: str,
        declared_count: int | None,
    ) -> tuple[np.ndarray, int]:
        values_key = "scan_input_values" if side == "input" else "scan_output_values"
        signal_arrays: Dict[str, np.ndarray] = {}

        for signal_name in self.MATCH_SIGNALS:
            raw_values = payloads[signal_name].get(values_key, {}).get(scan_idx, [])
            signal_arrays[signal_name] = np.asarray(raw_values, dtype=np.float64).ravel()

        available_lengths = {name: int(values.size) for name, values in signal_arrays.items()}
        non_zero_lengths = [length for length in available_lengths.values() if length > 0]
        if not non_zero_lengths:
            return np.empty((0, len(self.MATCH_SIGNALS)), dtype=np.float64), 0

        max_valid_length = min(non_zero_lengths)
        requested_count = int(declared_count) if declared_count is not None else max_valid_length
        requested_count = max(0, min(requested_count, max_valid_length))

        if len(set(available_lengths.values())) > 1:
            logger.debug(
                "Fallback %s scan %s signal lengths differ for %s: %s",
                side,
                scan_idx,
                self.sensor_id,
                available_lengths,
            )

        if requested_count == 0:
            return np.empty((0, len(self.MATCH_SIGNALS)), dtype=np.float64), 0

        points = np.column_stack(
            [signal_arrays[signal_name][:requested_count] for signal_name in self.MATCH_SIGNALS]
        )
        valid_mask = np.isfinite(points).all(axis=1)
        if not np.all(valid_mask):
            logger.debug(
                "Fallback %s scan %s filtered %s invalid detections for %s",
                side,
                scan_idx,
                int((~valid_mask).sum()),
                self.sensor_id,
            )
        return points[valid_mask], requested_count

    def _match_points(self, veh_points: np.ndarray, sim_points: np.ndarray) -> int:
        if veh_points.size == 0 or sim_points.size == 0:
            return 0

        out_map: Dict[tuple[int, int, int, int], int] = {}
        for point in sim_points:
            key = self._build_match_key(point)
            out_map[key] = out_map.get(key, 0) + 1

        matches = 0
        for point in veh_points:
            base_key = self._build_match_key(point)
            for offset in self._match_offsets:
                probe_key = (
                    base_key[0] + offset[0],
                    base_key[1] + offset[1],
                    base_key[2] + offset[2],
                    base_key[3] + offset[3],
                )
                available = out_map.get(probe_key, 0)
                if available <= 0:
                    continue
                matches += 1
                if available == 1:
                    del out_map[probe_key]
                else:
                    out_map[probe_key] = available - 1
                break

        return matches

    def _build_match_key(self, point: np.ndarray) -> tuple[int, int, int, int]:
        thresholds = (
            max(float(self.RAN_THRESHOLD), 1e-9),
            max(float(self.VEL_THRESHOLD), 1e-9),
            max(float(self.THETA_THRESHOLD), 1e-9),
            max(float(self.PHI_THRESHOLD), 1e-9),
        )
        return tuple(
            int(round(float(value) / threshold))
            for value, threshold in zip(point.tolist(), thresholds)
        )