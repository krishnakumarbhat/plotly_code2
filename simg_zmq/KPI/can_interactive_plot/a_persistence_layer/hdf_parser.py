"""HDF parser that transforms raw HDF attributes into scan-index keyed storage."""

import logging
import re
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
        self._last_parse_report = self._new_parse_report("")

    def get_last_parse_report(self) -> Dict[str, Any]:
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
        """Parse one HDF file into sensor-wise scan-index keyed structures."""
        report = self._new_parse_report(hdf_path)
        if not hdf_path:
            report["status"] = "error"
            report["errors"].append("HDF was unable to parse: path was not provided.")
            self._last_parse_report = report
            return {}

        try:
            raw = self._reader.read_hdf_attrs(hdf_path)
        except Exception as exc:
            logger.exception(f"Failed to read HDF file {hdf_path}: {exc}")
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
                    logger.warning(f"Skip sensor {sensor_id} in {hdf_path}: {msg}")
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
                hdr_can_t = self._reader.extract_header_can_times(sensor_data)
                det_can_t = self._reader.extract_detection_can_times(sensor_data)

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
                    hdr_can_t=hdr_can_t,
                    det_can_t=det_can_t,
                    header_signals=header,
                    alignment_signals=alignment,
                    detection_signals=detection,
                    valid_detection_count=valid_cnt,
                )

                parsed[sensor_id] = {
                    "friendly_name": sensor_data.get("friendly_name", sensor_id),
                    "scan_index": scan_index,
                    "time_ns": time_ns,
                    "hdr_can_t": hdr_can_t,
                    "det_can_t": det_can_t,
                    "scan_dict": scan_dict,
                    "storage": storage,
                    "header": header,
                    "alignment": alignment,
                    "detection": detection,
                }
                self._append_unique(report["parsed_sensors"], sensor_id)
                report["sensor_scan_counts"][sensor_id] = int(len(scan_index))
            except Exception as exc:
                logger.exception(
                    f"Failed parsing sensor {sensor_id} in {hdf_path}: {exc}"
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
        hdr_can_t: Optional[np.ndarray] = None,
        det_can_t: Optional[Dict[str, np.ndarray]] = None,
    ) -> KPI_DataModelStorage:
        storage = KPI_DataModelStorage()
        storage.initialize(scan_index.tolist(), sensor_id)
        storage.set_time_ns(time_ns)
        if hdr_can_t is not None:
            storage.set_hdr_can_t(hdr_can_t)
        if det_can_t is not None:
            storage.set_det_can_t(det_can_t)

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

    # Triple-key alignment tuning (see approach_compare.py results).
    TRIPLE_HDR_TOL_NS = 2_000_000
    TRIPLE_CAN_TOL_S = 0.002
    TRIPLE_WINDOW = 5
    TRIPLE_SHIFT_MIN = -5
    TRIPLE_SHIFT_MAX = 5
    # CAN timestamps are bus-relative seconds. Old producers leave
    # timestamp_* payloads uninitialized (e.g. -1.4e91); such values must
    # never steer gating or calibration.
    TRIPLE_CAN_T_MAX_S = 1_000_000.0

    @staticmethod
    def _sane_can_t(arr: np.ndarray) -> np.ndarray:
        a = np.asarray(arr, dtype=np.float64)
        bad = ~np.isfinite(a) | (a < 0.0) | (a > KpiHdfParser.TRIPLE_CAN_T_MAX_S)
        if np.any(bad):
            a = a.copy()
            a[bad] = np.nan
        return a

    def align_storage_rows_triple(
        self,
        input_storage: KPI_DataModelStorage,
        output_storage: KPI_DataModelStorage,
        hdr_tol_ns: int = TRIPLE_HDR_TOL_NS,
        can_tol_s: float = TRIPLE_CAN_TOL_S,
        window: int = TRIPLE_WINDOW,
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray, Dict[int, int]]:
        """Align rows by SCAN + header time, with per-group DET shifts.

        Stage 1: SCAN-equality candidates (same as legacy).
        Stage 2: header gate — a pair is rejected only when BOTH header times
          (HED ``time_ns`` and header CAN ``hdr_can_t``) are present AND both
          disagree. Missing times pass (legacy behaviour, e.g. old loggers).
          Pairs where both sides hold zero valid detections (header-only
          scans, XML parity) are dropped.
        Stage 3: per-detector-group row-shift calibration from DET-group CAN
          timestamps via O(1) hash vote (see ``_calibrate_det_shifts``).
          Returns ``det_shifts`` mapping group index (``det_pos // 4``) to the
          row delta applied to the INPUT side in ``get_scan_detections``.

        Falls back to legacy results (empty shifts) when timestamp payloads
        are absent, so old logs behave exactly as before.
        """
        common, in_rows, out_rows = self.align_storage_rows_by_scanindex(
            input_storage, output_storage
        )
        if len(common) == 0:
            return common, in_rows, out_rows, {}

        in_time = input_storage.get_time_ns()
        out_time = output_storage.get_time_ns()
        in_hdr = self._sane_can_t(input_storage.get_hdr_can_t())
        out_hdr = self._sane_can_t(output_storage.get_hdr_can_t())
        in_cnt = input_storage.get_valid_detection_counts()
        out_cnt = output_storage.get_valid_detection_counts()

        in_det = input_storage.get_det_can_t()
        out_det = output_storage.get_det_can_t()
        has_det_ts = any(
            np.count_nonzero(~np.isnan(v)) > max(10, len(in_rows) // 2)
            for v in in_det.values()
        ) if in_det else False

        kept_common: List[int] = []
        kept_in: List[int] = []
        kept_out: List[int] = []
        for k in range(len(common)):
            i, j = int(in_rows[k]), int(out_rows[k])
            # Header gate: best-effort. Accept if either time source says OK,
            # or when DET timestamps exist (shift calibration will fix it).
            hed_ok: Optional[bool] = None
            if (
                i < len(in_time)
                and j < len(out_time)
                and int(in_time[i]) >= 0
                and int(out_time[j]) >= 0
            ):
                hed_ok = abs(int(in_time[i]) - int(out_time[j])) <= int(hdr_tol_ns)
            can_ok: Optional[bool] = None
            if (
                i < len(in_hdr)
                and j < len(out_hdr)
                and np.isfinite(in_hdr[i])
                and np.isfinite(out_hdr[j])
            ):
                can_ok = abs(float(in_hdr[i]) - float(out_hdr[j])) <= float(can_tol_s)
            # Reject only when both available times disagree AND no DET
            # timestamps are available to compensate via shift calibration.
            if hed_ok is False and can_ok is False and not has_det_ts:
                continue
            ic = int(in_cnt[i]) if i < len(in_cnt) else 0
            oc = int(out_cnt[j]) if j < len(out_cnt) else 0
            if max(ic, oc) <= 0:
                continue  # header-only scan on both sides (XML parity)
            kept_common.append(int(common[k]))
            kept_in.append(i)
            kept_out.append(j)

        common_a = np.asarray(kept_common, dtype=np.int64)
        in_a = np.asarray(kept_in, dtype=np.int64)
        out_a = np.asarray(kept_out, dtype=np.int64)
        det_shifts = self._calibrate_det_shifts(
            input_storage, output_storage, in_a, out_a, window=window
        )
        return common_a, in_a, out_a, det_shifts

    def _calibrate_det_shifts(
        self,
        input_storage: KPI_DataModelStorage,
        output_storage: KPI_DataModelStorage,
        in_rows: np.ndarray,
        out_rows: np.ndarray,
        window: int = TRIPLE_WINDOW,
    ) -> Dict[int, int]:
        """Vote per-detector-group INPUT row shifts from DET CAN timestamps.

        For each DET group present in both files, an O(1) hash
        (1 ms-quantized INPUT timestamp -> rows) finds, for every gated pair
        ``(i, j)``, the INPUT row holding OUT[j]'s bus time. The shift
        ``matched_in_row - i`` is voted; the winner in
        ``[TRIPLE_SHIFT_MIN, TRIPLE_SHIFT_MAX]`` becomes that group's shift
        (applied to the INPUT fetch in ``get_scan_detections``).
        Groups without timestamps or without majority support keep shift 0,
        which reproduces the legacy exact-row behaviour.
        """
        in_det = input_storage.get_det_can_t()
        out_det = output_storage.get_det_can_t()
        if not in_det or not out_det or len(in_rows) == 0:
            return {}
        shifts: Dict[int, int] = {}
        lo, hi = self.TRIPLE_SHIFT_MIN, self.TRIPLE_SHIFT_MAX
        for gname, t_out_raw in out_det.items():
            t_out = self._sane_can_t(t_out_raw)
            t_in_raw = in_det.get(gname)
            if t_in_raw is None:
                continue
            t_in = self._sane_can_t(t_in_raw)
            # Require mostly-sane coverage; uninitialized payloads vote junk.
            if (
                np.count_nonzero(~np.isnan(t_in)) < max(10, len(in_rows) // 2)
                or np.count_nonzero(~np.isnan(t_out)) < max(10, len(out_rows) // 2)
            ):
                continue
            if len(t_in) < 2 or len(t_out) < 2:
                continue
            gidx = self._det_group_index(gname)
            if gidx is None:
                continue
            in_map: Dict[int, List[int]] = {}
            for r, t in enumerate(t_in):
                if np.isfinite(t):
                    in_map.setdefault(int(round(float(t) * 1000.0)), []).append(r)
            if not in_map:
                continue
            votes: Dict[int, int] = {}
            for i, j in zip(in_rows.tolist(), out_rows.tolist()):
                i, j = int(i), int(j)
                if j >= len(t_out):
                    continue
                tj = t_out[j]
                if not np.isfinite(tj):
                    continue
                best_c: Optional[int] = None
                best_dt = float("inf")
                q = int(round(float(tj) * 1000.0))
                for dq in (-2, -1, 0, 1, 2):
                    for c in in_map.get(q + dq, ()):
                        if abs(c - i) > window:
                            continue
                        dt = abs(float(t_in[c]) - float(tj))
                        if dt < best_dt:
                            best_dt = dt
                            best_c = c
                if best_c is None:
                    continue
                d = best_c - i
                if lo <= d <= hi:
                    votes[d] = votes.get(d, 0) + 1
            if not votes:
                continue
            best = min(votes.items(), key=lambda kv: (-kv[1], abs(kv[0]), kv[0]))[0]
            if votes[best] >= max(10, len(in_rows) // 2) and best != 0:
                shifts[gidx] = int(best)
        if shifts:
            logger.info(f"Triple align DET shifts (group->rows): {shifts}")
        fallback = self._calibrate_det_shifts_signal_fallback(
            input_storage, output_storage, in_rows, out_rows, skip=shifts
        )
        shifts.update(fallback)
        return shifts

    # Tight per-signal tolerances for shift calibration. Storage rounds rows
    # to 2 decimals, so true-shift diffs stay <= ~0.01 while neighbouring
    # scans normally differ by much more. Static scenes tie -> shift 0 wins.
    _CAL_EPS = {
        "DET_RANGE": 0.05,
        "DET_RANGE_VELOCITY": 0.05,
        "DET_AZIMUTH": 0.02,
        "DET_ELEVATION": 0.02,
    }
    _CAL_SIGNALS = ("DET_RANGE", "DET_RANGE_VELOCITY", "DET_AZIMUTH", "DET_ELEVATION")
    _CAL_MAX_GROUPS = 50

    def _calibrate_det_shifts_signal_fallback(
        self,
        input_storage: KPI_DataModelStorage,
        output_storage: KPI_DataModelStorage,
        in_rows: np.ndarray,
        out_rows: np.ndarray,
        skip: Optional[Dict[int, int]] = None,
    ) -> Dict[int, int]:
        """Timestamp-free per-group shift vote from DET signal values.

        Covers producers with missing/uninitialized ``timestamp_*`` payloads
        (old loggers). For each group without a timestamp shift, every shift
        ``n`` in ``[0,-1,+1,...]`` order is scored by position-wise joint
        signal equality (tight eps) summed over pairs; the winner needs a
        strict majority margin over ``n=0``. Ties/ambiguity keep shift 0,
        i.e. legacy behaviour.
        """
        skip = skip or {}
        if len(in_rows) == 0:
            return {}
        rows_in = {s: input_storage.get_detection_rows(s) for s in self._CAL_SIGNALS}
        rows_out = {s: output_storage.get_detection_rows(s) for s in self._CAL_SIGNALS}
        if not rows_in["DET_RANGE"] or not rows_out["DET_RANGE"]:
            return {}
        n_in, n_out = len(rows_in["DET_RANGE"]), len(rows_out["DET_RANGE"])
        P = len(in_rows)
        i_idx = np.asarray(in_rows, dtype=np.int64)
        j_idx = np.asarray(out_rows, dtype=np.int64)
        order = [0, -1, 1, -2, 2, -3, 3, -4, 4, -5, 5]
        order = [n for n in order if self.TRIPLE_SHIFT_MIN <= n <= self.TRIPLE_SHIFT_MAX]
        margin = max(5, int(0.05 * P))
        found: Dict[int, int] = {}
        for g in range(self._CAL_MAX_GROUPS):
            if g in skip:
                continue
            cols = range(4 * g, 4 * g + 4)
            totals: Dict[int, int] = {}
            for n in order:
                ii = i_idx + n
                # Majority vote per-pair: if >= 2 of 4 signals match,
                # the pair votes for this shift. Handles dropped slots.
                pair_votes = np.zeros(P, dtype=np.int32)
                n_signals = 0
                for sig in self._CAL_SIGNALS:
                    rin, rout = rows_in[sig], rows_out[sig]
                    if not rin or not rout:
                        continue
                    n_signals += 1
                    eps = self._CAL_EPS[sig]
                    hit = np.zeros(P, dtype=bool)
                    for p, (i, j) in enumerate(zip(ii.tolist(), j_idx.tolist())):
                        if i < 0 or i >= n_in or j < 0 or j >= n_out:
                            continue
                        a, b = rin[i], rout[j]
                        if not isinstance(a, np.ndarray) or not isinstance(b, np.ndarray):
                            continue
                        pos_ok = True
                        compared = False
                        for c in cols:
                            va = a[c] if c < len(a) else np.nan
                            vb = b[c] if c < len(b) else np.nan
                            if sig == "DET_RANGE":
                                ea = bool(np.isfinite(va)) and va != 0.0
                                eb = bool(np.isfinite(vb)) and vb != 0.0
                            else:
                                ea, eb = bool(np.isfinite(va)), bool(np.isfinite(vb))
                            if not ea and not eb:
                                continue
                            if not ea or not eb:
                                pos_ok = False
                                break
                            compared = True
                            if abs(float(va) - float(vb)) > eps:
                                pos_ok = False
                                break
                        if pos_ok and compared:
                            hit[p] = True
                    pair_votes += hit.astype(np.int32)
                # Pair matches if >= half of available signals agree
                min_sig = max(1, n_signals // 2)
                totals[n] = int(np.sum(pair_votes >= min_sig))
            base = totals.get(0, 0)
            best_n, best_h = 0, base
            for n in order[1:]:
                if totals.get(n, 0) > best_h:
                    best_n, best_h = n, totals[n]
            # Conservative: winner needs an absolute majority of pairs AND
            # a clear margin over legacy n=0. Reprocessed logs whose values
            # genuinely differ (beyond eps) keep legacy behaviour instead of
            # a noise-voted shift.
            if (
                best_n != 0
                and best_h >= max(10, P // 2)
                and best_h - base >= margin
            ):
                found[g] = int(best_n)
        if found:
            logger.info(f"Triple align fallback DET shifts (group->rows): {found}")
        return found

    @staticmethod
    def _det_group_index(group_name: str) -> Optional[int]:
        """Detector-group index from names like ``RDR_DETECTION_005_008``."""
        m = re.search(r"(\d{3})_(\d{3})\s*$", str(group_name))
        if not m:
            return None
        try:
            start = int(m.group(1))
        except ValueError:
            return None
        if start < 1:
            return None
        return (start - 1) // 4

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

    def _new_parse_report(self, hdf_path: str) -> Dict[str, Any]:
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
        if value not in values:
            values.append(value)
