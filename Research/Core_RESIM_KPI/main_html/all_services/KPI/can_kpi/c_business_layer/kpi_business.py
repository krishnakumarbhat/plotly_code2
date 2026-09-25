"""Business orchestration — uses storage objects for KPI matching and plotting."""

import logging
import math
from itertools import product
from typing import Any, Dict, List, Optional

import numpy as np

from a_persistence_layer.hdf_parser import KpiHdfParser
from b_data_storage.can_kpi_data_model_storage import KPI_DataModelStorage

logger = logging.getLogger(__name__)


class KpiBusiness:
    MATCH_SIGNALS = ["DET_RANGE", "DET_RANGE_VELOCITY", "DET_AZIMUTH", "DET_ELEVATION"]
    MATCH_EPSILON = 10.0
    TIME_MATCH_TOL_NS = 2_000_000

    SENSOR_ORDER = ["CEER_FL", "CEER_FLR", "CEER_FR", "CEER_RL", "CEER_RR"]
    FRIENDLY = {
        "CEER_FL": "SRR / FL",
        "CEER_FLR": "SRR / FLR",
        "CEER_FR": "SRR / FR",
        "CEER_RL": "SRR / RL",
        "CEER_RR": "SRR / RR",
    }

    def __init__(
        self,
        hdf_parser: Optional[KpiHdfParser] = None,
    ):
        required_signals = sorted(set(self.MATCH_SIGNALS))
        self._hdf = hdf_parser or KpiHdfParser(
            required_detection_signals=required_signals
        )
        self._offsets_4d = sorted(
            list(product([-1, 0, 1], repeat=4)),
            key=lambda o: abs(o[0]) + abs(o[1]) + abs(o[2]) + abs(o[3]),
        )
        self._offsets_1d = [(-1,), (0,), (1,)]

    def _compute_match_pct(
        self,
        in_store: Optional[KPI_DataModelStorage],
        out_store: Optional[KPI_DataModelStorage],
        sensor_id: str,
    ) -> Dict[str, Any]:
        empty = np.array([], dtype=np.float16)
        empty_params = {s: empty for s in self.MATCH_SIGNALS}
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

        common_scan, in_rows, out_rows = self._hdf.align_storage_rows_by_scanindex(
            in_store, out_store
        )
        n = len(common_scan)
        if n == 0:
            logger.warning(f"{sensor_id}: no common scanindex rows found")
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
        unmatched_in_scan = max(0, in_total_scans - n)
        unmatched_out_scan = max(0, out_total_scans - n)
        diff_cnt_scans = int(np.sum(in_cnt != out_cnt))

        if diff_cnt_scans > 0:
            logger.warning(
                f"{sensor_id}: HED_NUM_OF_VALID_DETECTIONS differs between input/output "
                f"for {diff_cnt_scans} scans; using max(in,out)"
            )
        if unmatched_in_scan > 0 or unmatched_out_scan > 0:
            logger.warning(
                f"{sensor_id}: unmatched scanindex rows input_only={unmatched_in_scan}, "
                f"output_only={unmatched_out_scan}"
            )

        overall = np.zeros(n, dtype=np.float16)
        precision = np.zeros(n, dtype=np.float16)
        recall = np.zeros(n, dtype=np.float16)
        f1 = np.zeros(n, dtype=np.float16)
        accuracy = np.zeros(n, dtype=np.float16)
        per_param = {sig: np.zeros(n, dtype=np.float16) for sig in self.MATCH_SIGNALS}
        missing_input_det_total = 0
        missing_output_det_total = 0

        for idx in range(n):
            n_det = int(denom_cnt[idx])
            in_n_det = int(in_cnt[idx])
            out_n_det = int(out_cnt[idx])
            in_candidates = in_store.get_scan_detections(
                self.MATCH_SIGNALS, int(in_rows[idx]), max(in_n_det, 0)
            )
            out_candidates = out_store.get_scan_detections(
                self.MATCH_SIGNALS, int(out_rows[idx]), max(out_n_det, 0)
            )

            if n_det <= 0 and (not in_candidates and not out_candidates):
                overall[idx] = np.float16(100.0)
                precision[idx] = np.float16(100.0)
                recall[idx] = np.float16(100.0)
                f1[idx] = np.float16(100.0)
                accuracy[idx] = np.float16(100.0)
                for sig in self.MATCH_SIGNALS:
                    per_param[sig][idx] = np.float16(100.0)
                continue

            tp_all = self._match_detections_hashmap(in_candidates, out_candidates)
            in_n = len(in_candidates)
            out_n = len(out_candidates)
            denom = max(n_det, in_n, out_n)
            if denom <= 0:
                continue

            missing_input_det_total += max(0, n_det - in_n)
            missing_output_det_total += max(0, n_det - out_n)

            tp_sig: Dict[str, int] = {}
            for sig in self.MATCH_SIGNALS:
                in_vals = [row[sig] for row in in_candidates]
                out_vals = [row[sig] for row in out_candidates]
                tp_sig[sig] = self._match_1d_hashmap(in_vals, out_vals)

            fp = max(0, out_n - tp_all)
            fn = max(0, in_n - tp_all)

            prec = tp_all / (tp_all + fp) if (tp_all + fp) > 0 else 0.0
            rec = tp_all / (tp_all + fn) if (tp_all + fn) > 0 else 0.0
            f1_val = (2.0 * prec * rec / (prec + rec)) if (prec + rec) > 0 else 0.0
            acc = tp_all / (tp_all + fp + fn) if (tp_all + fp + fn) > 0 else 0.0

            overall[idx] = np.float16(round(100.0 * (tp_all / float(denom)), 2))
            precision[idx] = np.float16(round(100.0 * prec, 2))
            recall[idx] = np.float16(round(100.0 * rec, 2))
            f1[idx] = np.float16(round(100.0 * f1_val, 2))
            accuracy[idx] = np.float16(round(100.0 * acc, 2))

            for sig in self.MATCH_SIGNALS:
                per_param[sig][idx] = np.float16(
                    round(100.0 * (tp_sig[sig] / float(denom)), 2)
                )

        logger.info(
            f"{sensor_id}: common_scans={n}, input_scans={in_total_scans}, output_scans={out_total_scans}, "
            f"count_mismatch_scans={diff_cnt_scans}, missing_input_det_total={missing_input_det_total}, "
            f"missing_output_det_total={missing_output_det_total}, avg_overall={self._avg(overall):.2f}"
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

    def _quantize(self, value: float) -> int:
        return int(round(float(value) / self.MATCH_EPSILON))

    def _match_1d_hashmap(self, in_vals: List[float], out_vals: List[float]) -> int:
        out_map: Dict[tuple[int], int] = {}
        for v in out_vals:
            k = (self._quantize(v),)
            out_map[k] = out_map.get(k, 0) + 1

        matches = 0
        for v in in_vals:
            base = self._quantize(v)
            used = False
            for off in self._offsets_1d:
                key = (base + off[0],)
                cnt = out_map.get(key, 0)
                if cnt > 0:
                    matches += 1
                    if cnt == 1:
                        del out_map[key]
                    else:
                        out_map[key] = cnt - 1
                    used = True
                    break
            if used:
                continue
        return matches

    def _match_detections_hashmap(
        self,
        in_candidates: List[Dict[str, float]],
        out_candidates: List[Dict[str, float]],
    ) -> int:
        out_map: Dict[tuple[int, int, int, int], int] = {}
        for row in out_candidates:
            key = (
                self._quantize(row["DET_RANGE"]),
                self._quantize(row["DET_RANGE_VELOCITY"]),
                self._quantize(row["DET_AZIMUTH"]),
                self._quantize(row["DET_ELEVATION"]),
            )
            out_map[key] = out_map.get(key, 0) + 1

        matches = 0
        for row in in_candidates:
            base = (
                self._quantize(row["DET_RANGE"]),
                self._quantize(row["DET_RANGE_VELOCITY"]),
                self._quantize(row["DET_AZIMUTH"]),
                self._quantize(row["DET_ELEVATION"]),
            )

            for off in self._offsets_4d:
                key = (
                    base[0] + off[0],
                    base[1] + off[1],
                    base[2] + off[2],
                    base[3] + off[3],
                )
                cnt = out_map.get(key, 0)
                if cnt > 0:
                    matches += 1
                    if cnt == 1:
                        del out_map[key]
                    else:
                        out_map[key] = cnt - 1
                    break
        return matches

    def _safe_take(self, arr: np.ndarray, rows: np.ndarray) -> np.ndarray:
        if not isinstance(arr, np.ndarray) or len(arr) == 0 or len(rows) == 0:
            return np.zeros(len(rows), dtype=np.int32)
        out = np.zeros(len(rows), dtype=np.int32)
        valid = rows < len(arr)
        if np.any(valid):
            out[valid] = np.clip(np.rint(arr[rows[valid]]).astype(np.int32), 0, None)
        return out

    def _build_summary_tables(
        self,
        in_stores: Dict[str, KPI_DataModelStorage],
        out_stores: Dict[str, KPI_DataModelStorage],
        all_sensors: List[str],
    ) -> tuple[List[str], List[List[str]]]:
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
                out_store.get_scan_index()
                if out_store
                else np.array([], dtype=np.int64)
            )
            if in_store is not None and out_store is not None:
                common, _, _ = self._hdf.align_storage_rows_by_scanindex(
                    in_store, out_store
                )
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

    def _avg(self, arr: np.ndarray) -> float:
        if not isinstance(arr, np.ndarray) or len(arr) == 0:
            return 0.0
        return float(np.nanmean(arr.astype(float)))

    def _pct(self, num: int, den: int) -> float:
        if den <= 0:
            return float("nan")
        return 100.0 * float(num) / float(den)
