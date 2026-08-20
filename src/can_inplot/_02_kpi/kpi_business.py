"""Business orchestration — per-sensor KPI computation.

Purpose: drive parse + match + summary for a CAN log pair, producing the metric
arrays consumed by the visual layer.
Inputs : parsed input/output storage maps (from 01_ingest).
Outputs: per-sensor match metric dicts + summary tables.
"""

import logging
import math
from typing import Any, Dict, List, Optional

import numpy as np

from can_inplot._01_ingest.hdf_parser import KpiHdfParser
from can_inplot._02_kpi.storage import KPI_DataModelStorage
from can_inplot._02_kpi.align import align_storage_rows_by_scanindex
from can_inplot._02_kpi.match import DetectionMatcher, MATCH_SIGNALS
from can_inplot._01_ingest.schema import SENSOR_ORDER, FRIENDLY

logger = logging.getLogger(__name__)


class KpiBusiness:
    """Computes per-sensor match KPIs between input and output HDF payloads."""

    def __init__(self, hdf_parser: Optional[KpiHdfParser] = None) -> None:
        """Purpose: build the business layer.
        Inputs : optional preconfigured HDF parser.
        Outputs: business instance."""
        required_signals = sorted(set(MATCH_SIGNALS))
        self._hdf = hdf_parser or KpiHdfParser(
            required_detection_signals=required_signals
        )
        self._matcher = DetectionMatcher()
        self.SENSOR_ORDER = SENSOR_ORDER
        self.FRIENDLY = FRIENDLY

    def compute_match_per_sensor(
        self,
        in_store: Optional[KPI_DataModelStorage],
        out_store: Optional[KPI_DataModelStorage],
        sensor_id: str,
    ) -> Dict[str, Any]:
        """Purpose: compute per-scan match metrics for one sensor.
        Inputs : input/output storages (may be None) and sensor id.
        Outputs: dict with scan/overall/precision/recall/f1/accuracy/per_signal."""
        empty = np.array([], dtype=np.float16)
        empty_params = {s: empty for s in MATCH_SIGNALS}
        if in_store is None or out_store is None:
            return {
                "scan": np.array([], dtype=np.int64),
                "overall": empty,
                "precision": empty,
                "recall": empty,
                "f1": empty,
                "accuracy": empty,
                "per_signal": empty_params,
            }

        common_scan, in_rows, out_rows = align_storage_rows_by_scanindex(
            in_store, out_store
        )
        n = len(common_scan)
        if n == 0:
            logger.warning("%s: no common scanindex rows found", sensor_id)
            return {
                "scan": common_scan,
                "overall": empty,
                "precision": empty,
                "recall": empty,
                "f1": empty,
                "accuracy": empty,
                "per_signal": empty_params,
            }

        in_cnt_all = in_store.get_valid_detection_counts()
        out_cnt_all = out_store.get_valid_detection_counts()
        in_cnt = self._safe_take(in_cnt_all, in_rows)
        out_cnt = self._safe_take(out_cnt_all, out_rows)
        denom_cnt = np.maximum(in_cnt, out_cnt).astype(np.int32)

        in_total_scans = len(in_store.get_scan_index())
        out_total_scans = len(out_store.get_scan_index())
        diff_cnt_scans = int(np.sum(in_cnt != out_cnt))
        if diff_cnt_scans > 0:
            logger.warning(
                "%s: HED_NUM_OF_VALID_DETECTIONS differs between input/output for "
                "%d scans; using max(in,out)",
                sensor_id,
                diff_cnt_scans,
            )

        overall = np.zeros(n, dtype=np.float16)
        precision = np.zeros(n, dtype=np.float16)
        recall = np.zeros(n, dtype=np.float16)
        f1 = np.zeros(n, dtype=np.float16)
        accuracy = np.zeros(n, dtype=np.float16)
        per_param = {sig: np.zeros(n, dtype=np.float16) for sig in MATCH_SIGNALS}

        for idx in range(n):
            n_det = int(denom_cnt[idx])
            in_n_det = int(in_cnt[idx])
            out_n_det = int(out_cnt[idx])
            in_candidates = in_store.get_scan_detections(
                MATCH_SIGNALS, int(in_rows[idx]), max(in_n_det, 0)
            )
            out_candidates = out_store.get_scan_detections(
                MATCH_SIGNALS, int(out_rows[idx]), max(out_n_det, 0)
            )

            if n_det <= 0 and (not in_candidates and not out_candidates):
                overall[idx] = np.float16(100.0)
                precision[idx] = np.float16(100.0)
                recall[idx] = np.float16(100.0)
                f1[idx] = np.float16(100.0)
                accuracy[idx] = np.float16(100.0)
                for sig in MATCH_SIGNALS:
                    per_param[sig][idx] = np.float16(100.0)
                continue

            stats = self._matcher.match_scan(in_candidates, out_candidates)
            denom = max(n_det, len(in_candidates), len(out_candidates))
            if denom <= 0:
                continue

            overall[idx] = np.float16(round(100.0 * stats["tp"] / float(denom), 2))
            precision[idx] = np.float16(round(100.0 * stats["precision"], 2))
            recall[idx] = np.float16(round(100.0 * stats["recall"], 2))
            f1[idx] = np.float16(round(100.0 * stats["f1"], 2))
            accuracy[idx] = np.float16(round(100.0 * stats["accuracy"], 2))

            for sig in MATCH_SIGNALS:
                in_vals = [row[sig] for row in in_candidates]
                out_vals = [row[sig] for row in out_candidates]
                tp_sig = self._matcher.match_signal_1d(in_vals, out_vals)
                per_param[sig][idx] = np.float16(
                    round(100.0 * tp_sig / float(denom), 2)
                )

        logger.info(
            "%s: common_scans=%d, input_scans=%d, output_scans=%d, "
            "count_mismatch_scans=%d, avg_overall=%.2f",
            sensor_id,
            n,
            in_total_scans,
            out_total_scans,
            diff_cnt_scans,
            self._avg(overall),
        )
        return {
            "scan": common_scan,
            "overall": overall,
            "precision": precision,
            "recall": recall,
            "f1": f1,
            "accuracy": accuracy,
            "per_signal": per_param,
        }

    def build_summary_tables(
        self,
        in_stores: Dict[str, KPI_DataModelStorage],
        out_stores: Dict[str, KPI_DataModelStorage],
        all_sensors: List[str],
    ) -> tuple[List[str], List[List[str]]]:
        """Purpose: per-sensor overview table.
        Inputs : storage maps and sensor list.
        Outputs: (headers, rows)."""
        headers = [
            "Sensor",
            "Scans (Input)",
            "Scans (Output)",
            "Common Scans",
            "Scan Match %",
            "Det Signals (Input)",
            "Det Signals (Output)",
        ]
        rows: List[List[str]] = []
        for sensor_id in all_sensors:
            friendly = self.FRIENDLY.get(sensor_id, sensor_id)
            in_store = in_stores.get(sensor_id)
            out_store = out_stores.get(sensor_id)
            in_scan = (
                in_store.get_scan_index() if in_store else np.array([], dtype=np.int64)
            )
            out_scan = (
                out_store.get_scan_index() if out_store else np.array([], dtype=np.int64)
            )
            if in_store is not None and out_store is not None:
                common, _, _ = align_storage_rows_by_scanindex(in_store, out_store)
            else:
                common = np.array([], dtype=np.int64)
            pct = self._pct(len(common), max(len(in_scan), len(out_scan)))
            in_sigs = len(in_store.get_detection_signal_names()) if in_store else 0
            out_sigs = len(out_store.get_detection_signal_names()) if out_store else 0
            rows.append(
                [
                    friendly,
                    str(len(in_scan)),
                    str(len(out_scan)),
                    str(len(common)),
                    f"{pct:.2f}" if not math.isnan(pct) else "NA",
                    str(in_sigs),
                    str(out_sigs),
                ]
            )
        return headers, rows

    def compute_latency_kpis(
        self, in_store: KPI_DataModelStorage, out_store: KPI_DataModelStorage
    ) -> Dict[str, float]:
        """Purpose: latency KPIs between input and output timestamps.
        Inputs : input/output storages.
        Outputs: dict with mean/p95/p99 latency in ms and scan count."""
        in_time = in_store.get_time_ns()
        out_time = out_store.get_time_ns()
        if len(in_time) == 0 or len(out_time) == 0:
            return {"mean_ms": 0.0, "p95_ms": 0.0, "p99_ms": 0.0, "scans": 0}
        _, in_rows, out_rows = align_storage_rows_by_scanindex(in_store, out_store)
        if len(in_rows) == 0:
            return {"mean_ms": 0.0, "p95_ms": 0.0, "p99_ms": 0.0, "scans": 0}
        n = min(len(in_rows), len(in_time), len(out_time))
        dt_ms = (
            out_time[out_rows[:n]] - in_time[in_rows[:n]]
        ).astype(np.float64) / 1e6
        return {
            "mean_ms": float(np.mean(dt_ms)),
            "p95_ms": float(np.percentile(dt_ms, 95)),
            "p99_ms": float(np.percentile(dt_ms, 99)),
            "scans": int(n),
        }

    def _safe_take(self, arr: np.ndarray, rows: np.ndarray) -> np.ndarray:
        """Purpose: indexed take with bounds clipping.
        Inputs : source array and row indices.
        Outputs: clipped int32 values."""
        if not isinstance(arr, np.ndarray) or len(arr) == 0 or len(rows) == 0:
            return np.zeros(len(rows), dtype=np.int32)
        out = np.zeros(len(rows), dtype=np.int32)
        valid = rows < len(arr)
        if np.any(valid):
            out[valid] = np.clip(np.rint(arr[rows[valid]]).astype(np.int32), 0, None)
        return out

    def _avg(self, arr: np.ndarray) -> float:
        """Purpose: NaN-safe mean.
        Inputs : metric array.
        Outputs: mean value."""
        if not isinstance(arr, np.ndarray) or len(arr) == 0:
            return 0.0
        return float(np.nanmean(arr.astype(float)))

    def _pct(self, num: int, den: int) -> float:
        """Purpose: percentage helper.
        Inputs : numerator, denominator.
        Outputs: percentage or NaN."""
        if den <= 0:
            return float("nan")
        return 100.0 * float(num) / float(den)