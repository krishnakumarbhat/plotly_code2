#################################
# Tracker Matching KPI HDF Implementation
#################################
import sys
import os
import re
import numpy as np
import pandas as pd
import plotly.graph_objs as go
import plotly.subplots as sp
import plotly.io as pio
import json
import logging
from collections import deque
from typing import Dict, List, Tuple, Any

from UDP_KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
from UDP_KPI.b_data_storage.kpi_config_storage import KPI_VALIDATION_RULES, KPI_TRACKER_CONFIG

logger = logging.getLogger(__name__)

# Canonical tracker signals with backward-compatible HDF spellings.
# Classic layout: trkID / vcs_xposn / ... ; AshokLeyland layout: object_trkID /
# object_xposn / ... under OBJECT_LIST_STREAM/F360_Object_Log.
TRACKER_SIGNAL_ALIASES = {
    "track_id": ["trkID", "track_id", "object_trkID"],
    "x_position": ["vcs_xposn", "x_position", "object_xposn"],
    "y_position": ["vcs_yposn", "y_position", "object_yposn"],
    "x_velocity": ["vcs_xvel", "x_velocity", "object_xvel"],
    "y_velocity": ["vcs_yvel", "y_velocity", "object_yvel"],
    "heading": ["vcs_heading", "heading", "object_heading"],
    "length": ["len1", "len2", "length", "object_length"],
    "width": ["wid1", "wid2", "width", "object_width"],
    "f_moving": ["f_moving", "object_f_moving", "object_f_moveable"],
}

# Cap on matched pairs kept for interactive plots (stats always use all pairs).
_TRACKER_PLOT_PAIR_CAP = 2000


def _first_available_signal(storage: KPI_DataModelStorage, aliases):
    """Return (values, status, matched_alias) for the first usable alias."""
    for alias in aliases:
        try:
            values, status = KPI_DataModelStorage.get_value(storage, alias)
        except Exception:
            continue
        if status == "success" and values is not None and np.asarray(values).size > 0:
            return np.asarray(values), status, alias
    return np.empty((0, 0)), "not_found", None


class TrackerMappingKPIHDF:
    """Tracker KPI analysis using HDF data from self.input_data and self.output_data"""

    def __init__(self, input_data: KPI_DataModelStorage, output_data: KPI_DataModelStorage,
                 sensor_id: str, stream_name: str):
        self.input_data = input_data
        self.output_data = output_data
        self.sensor_id = sensor_id
        self.stream_name = stream_name

        # Thresholds from KPI_VALIDATION_RULES["tracker_thresholds"]:
        # vcs_xposn_threshold / vcs_yposn_threshold / vcs_xvel_threshold /
        # vcs_yvel_threshold (per-axis kinematics gates) + max_valid_distance
        # (range gate on hypot(x, y)). The legacy single position_gate_m is
        # retained harmlessly unused for backward compatibility.
        self.thresholds = KPI_VALIDATION_RULES.get("tracker_thresholds", {})
        self.tracker_config = KPI_TRACKER_CONFIG
        self.tx = float(self.thresholds.get("vcs_xposn_threshold", 0.01))
        self.ty = float(self.thresholds.get("vcs_yposn_threshold", 0.01))
        self.tvx = float(self.thresholds.get("vcs_xvel_threshold", 0.02))
        self.tvy = float(self.thresholds.get("vcs_yvel_threshold", 0.02))
        self.max_valid_distance_m = float(self.thresholds.get("max_valid_distance", 160.0))
        self.position_gate_m = float(np.hypot(self.tx, self.ty))  # legacy, unused

        # Initialize result variables
        self.html_content = ""
        self.kpi_results = {}
        self.matched_tracks_df = None
        self.summary_stats = {}
        self._per_scan = []
        self._total_in_valid = 0
        self._total_out_valid = 0
        self._total_tp = 0
        self._total_fp = 0
        self._total_fn = 0
        self._fmoving_agree = 0
        self._fmoving_cooccur = 0
        self._scans_with_matches = 0
        self._n_scans = 0
        self._id_overlap = {}
        
    def extract_tracker_data_from_hdf(self, data_model: KPI_DataModelStorage):
        """Extract canonical tracker arrays from storage via signal aliases.

        Returns dict canonical_name -> 2D np.ndarray (n_scans, n_slots),
        trying every known HDF spelling so classic TRACKER_STREAM and
        AshokLeyland OBJECT_LIST_STREAM layouts both work.
        """
        try:
            tracker_data = {}
            for canonical, aliases in TRACKER_SIGNAL_ALIASES.items():
                values, status, hit = _first_available_signal(data_model, aliases)
                if status == "success":
                    tracker_data[canonical] = np.asarray(values, dtype=float)
                    logger.debug(
                        "Tracker signal %s resolved as '%s' shape=%s",
                        canonical, hit, tracker_data[canonical].shape,
                    )
            return tracker_data

        except Exception as e:
            logger.error(f"Error extracting tracker data: {e}")
            return {}

    @staticmethod
    def _valid_mask(track_ids, xs, ys):
        """Valid object slots: real track id, finite position.

        trkID 0 marks unused slots (AshokLeyland resim pads with zeros).
        """
        track_ids = np.asarray(track_ids, dtype=float)
        xs = np.asarray(xs, dtype=float)
        ys = np.asarray(ys, dtype=float)
        valid = (
            np.isfinite(track_ids) & (track_ids != 0)
            & np.isfinite(xs) & np.isfinite(ys)
        )
        return valid

    def process_tracker_matching(self):
        """Main processing function for tracker matching KPIs"""
        try:
            # Extract data from HDF (alias-aware, both log layouts)
            input_tracker_data = self.extract_tracker_data_from_hdf(self.input_data)
            output_tracker_data = self.extract_tracker_data_from_hdf(self.output_data)

            required = ["track_id", "x_position", "y_position"]
            missing_in = [k for k in required if k not in input_tracker_data]
            missing_out = [k for k in required if k not in output_tracker_data]
            if missing_in or missing_out:
                logger.warning(
                    "Insufficient tracker data for KPI analysis "
                    f"(input missing={missing_in}, output missing={missing_out})"
                )
                return False

            # Perform per-scan tracker matching: composite-key (scan, trkID)
            # hashmap with per-axis kinematics matrix (CAN-KPI discipline).
            matching_results = self.match_tracker_data(
                input_tracker_data, output_tracker_data
            )

            # Calculate KPIs (zero-match still yields a report, not a failure)
            self.calculate_tracker_kpis(matching_results or {})

            # Generate HTML report
            self.generate_html_report()

            return True

        except Exception as e:
            logger.error(f"Error in tracker matching process: {e}")
            return False
            
    def match_tracker_data(self, input_data: Dict[str, np.ndarray], output_data: Dict[str, np.ndarray]):
        """Match tracker objects per scan by composite key (scan, trkID).

        CAN-KPI discipline: per-scan hashmaps keyed on integer trkID with
        one-to-one consume (one output dequeued per input, in slot order).
        A co-occurring ID is a true positive only if the kinematics matrix
        passes: |dx|<=tx and |dy|<=ty and |dvx|<=tvx and |dvy|<=tvy (a
        velocity-axis gate is skipped when either side is NaN, recorded on
        the pair) and f_moving agrees (NaN/unset flag is a mismatch).
        Co-occurring IDs that fail the matrix are FN; unpaired input IDs
        are FN; unpaired output IDs are FP. Storage rows are scan-aligned
        by the wrapper (common scan_index order), so row i of input and
        output correspond to the same scan. O(scans*slots).
        """
        try:
            in_ids = np.asarray(input_data['track_id'], dtype=float)
            in_x = np.asarray(input_data['x_position'], dtype=float)
            in_y = np.asarray(input_data['y_position'], dtype=float)
            out_ids = np.asarray(output_data['track_id'], dtype=float)
            out_x = np.asarray(output_data['x_position'], dtype=float)
            out_y = np.asarray(output_data['y_position'], dtype=float)
            in_vx = np.asarray(input_data.get('x_velocity', np.full_like(in_x, np.nan)), dtype=float)
            in_vy = np.asarray(input_data.get('y_velocity', np.full_like(in_y, np.nan)), dtype=float)
            out_vx = np.asarray(output_data.get('x_velocity', np.full_like(out_x, np.nan)), dtype=float)
            out_vy = np.asarray(output_data.get('y_velocity', np.full_like(out_y, np.nan)), dtype=float)
            in_fm = np.asarray(input_data.get('f_moving', np.full_like(in_x, np.nan)), dtype=float)
            out_fm = np.asarray(output_data.get('f_moving', np.full_like(out_x, np.nan)), dtype=float)

            # Stacked arrays are (n_scans, n_slots); normalize 1-D corner cases.
            def _as_2d(a):
                a = np.asarray(a, dtype=float)
                if a.ndim == 1:
                    a = a.reshape(-1, 1)
                return a

            in_ids, in_x, in_y = _as_2d(in_ids), _as_2d(in_x), _as_2d(in_y)
            out_ids, out_x, out_y = _as_2d(out_ids), _as_2d(out_x), _as_2d(out_y)
            in_vx, in_vy = _as_2d(in_vx), _as_2d(in_vy)
            out_vx, out_vy = _as_2d(out_vx), _as_2d(out_vy)
            in_fm, out_fm = _as_2d(in_fm), _as_2d(out_fm)

            n_scans = int(min(in_ids.shape[0], out_ids.shape[0]))
            tx = float(getattr(self, 'tx', float(self.thresholds.get('vcs_xposn_threshold', 0.01))))
            ty = float(getattr(self, 'ty', float(self.thresholds.get('vcs_yposn_threshold', 0.01))))
            tvx = float(getattr(self, 'tvx', float(self.thresholds.get('vcs_xvel_threshold', 0.02))))
            tvy = float(getattr(self, 'tvy', float(self.thresholds.get('vcs_yvel_threshold', 0.02))))
            max_range = float(self.max_valid_distance_m)
            range_on = bool(np.isfinite(max_range) and max_range > 0)

            try:
                in_keys = list(getattr(self.input_data, '_data_container', {}).keys())
            except Exception:
                in_keys = []
            try:
                out_keys = list(getattr(self.output_data, '_data_container', {}).keys())
            except Exception:
                out_keys = []

            def _at(a, i, j):
                try:
                    if i < a.shape[0] and j < a.shape[1]:
                        return float(a[i, j])
                except Exception:
                    pass
                return float('nan')

            matched_tracks = []
            position_errors = []
            velocity_errors = []
            per_scan = []
            total_in_valid = 0
            total_out_valid = 0
            total_tp = 0
            total_fp = 0
            total_fn = 0
            fmoving_agree = 0
            fmoving_cooccur = 0
            scans_with_matches = 0
            in_id_set = set()
            out_id_set = set()

            for i in range(n_scans):
                if i < len(in_keys):
                    scan_value = in_keys[i]
                elif i < len(out_keys):
                    scan_value = out_keys[i]
                else:
                    scan_value = i

                in_valid = self._valid_mask(in_ids[i], in_x[i], in_y[i])
                out_valid = self._valid_mask(out_ids[i], out_x[i], out_y[i])
                # Range gate on hypot(x, y) when configured.
                if range_on:
                    in_valid = in_valid & (np.hypot(in_x[i], in_y[i]) <= max_range)
                    out_valid = out_valid & (np.hypot(out_x[i], out_y[i]) <= max_range)

                in_idx = np.flatnonzero(in_valid)
                out_idx = np.flatnonzero(out_valid)
                scan_n_in = int(in_idx.size)
                scan_n_out = int(out_idx.size)
                total_in_valid += scan_n_in
                total_out_valid += scan_n_out

                # Composite-key hashmaps: trkID (int, finite only) -> deque of
                # payloads (x, y, vx, vy, fmoving, slot), built in slot order.
                in_map: Dict[int, deque] = {}
                in_order: List[Tuple[int, tuple]] = []
                for gj in in_idx:
                    j = int(gj)
                    tid_raw = float(in_ids[i, j])
                    if not np.isfinite(tid_raw):
                        continue
                    tid = int(tid_raw)
                    in_id_set.add(tid)
                    pay = (float(in_x[i, j]), float(in_y[i, j]),
                           _at(in_vx, i, j), _at(in_vy, i, j),
                           _at(in_fm, i, j), j)
                    dq = in_map.get(tid)
                    if dq is None:
                        dq = deque()
                        in_map[tid] = dq
                    dq.append(pay)
                    in_order.append((tid, pay))

                out_map: Dict[int, deque] = {}
                for gj in out_idx:
                    j = int(gj)
                    tid_raw = float(out_ids[i, j])
                    if not np.isfinite(tid_raw):
                        continue
                    tid = int(tid_raw)
                    out_id_set.add(tid)
                    pay = (float(out_x[i, j]), float(out_y[i, j]),
                           _at(out_vx, i, j), _at(out_vy, i, j),
                           _at(out_fm, i, j), j)
                    dq = out_map.get(tid)
                    if dq is None:
                        dq = deque()
                        out_map[tid] = dq
                    dq.append(pay)

                # Greedy in-slot-order pairing: pop one output per input ID.
                scan_tp = 0
                scan_fn_paired = 0
                scan_fn_unpaired = 0
                for tid, in_pay in in_order:
                    out_dq = out_map.get(tid)
                    if out_dq is None or len(out_dq) == 0:
                        scan_fn_unpaired += 1
                        continue
                    out_pay = out_dq.popleft()
                    ix, iy, ivx, ivy, ifm, _js = in_pay
                    ox, oy, ovx, ovy, ofm, _ks = out_pay
                    dx = abs(ix - ox)
                    dy = abs(iy - oy)
                    pos_ok = (dx <= tx) and (dy <= ty)
                    if (not np.isfinite(ivx)) or (not np.isfinite(ovx)):
                        vx_ok = True
                        vx_skipped = True
                        dvx = float('nan')
                    else:
                        dvx = abs(ivx - ovx)
                        vx_ok = bool(dvx <= tvx)
                        vx_skipped = False
                    if (not np.isfinite(ivy)) or (not np.isfinite(ovy)):
                        vy_ok = True
                        vy_skipped = True
                        dvy = float('nan')
                    else:
                        dvy = abs(ivy - ovy)
                        vy_ok = bool(dvy <= tvy)
                        vy_skipped = False
                    if (not np.isfinite(ifm)) or (not np.isfinite(ofm)):
                        fm_ok = False
                    else:
                        fm_ok = bool(float(ifm) == float(ofm))
                    fmoving_cooccur += 1
                    if fm_ok:
                        fmoving_agree += 1
                    if pos_ok and vx_ok and vy_ok and fm_ok:
                        scan_tp += 1
                        spat = float(np.hypot(ix - ox, iy - oy))
                        pair = {
                            'scan_row': int(i),
                            'scan_value': scan_value,
                            'input_track_id': int(tid),
                            'output_track_id': int(tid),
                            'input_pos': np.array([ix, iy], dtype=float),
                            'output_pos': np.array([ox, oy], dtype=float),
                            'spatial_distance': spat,
                            'input_vel': np.array([ivx, ivy], dtype=float),
                            'output_vel': np.array([ovx, ovy], dtype=float),
                            'fmoving_agree': bool(fm_ok),
                            'vx_gate_skipped': bool(vx_skipped),
                            'vy_gate_skipped': bool(vy_skipped),
                        }
                        matched_tracks.append(pair)
                        position_errors.append(spat)
                        try:
                            verr = float(np.linalg.norm(
                                pair['input_vel'] - pair['output_vel']))
                        except Exception:
                            verr = float('nan')
                        if np.isfinite(verr):
                            velocity_errors.append(verr)
                    else:
                        scan_fn_paired += 1

                scan_fp = 0
                for dq in out_map.values():
                    scan_fp += int(len(dq))
                scan_fn = int(scan_fn_paired + scan_fn_unpaired)
                scan_den = int(scan_tp + scan_fn)
                if scan_den > 0:
                    scan_acc = 100.0 * float(scan_tp) / float(scan_den)
                else:
                    scan_acc = 100.0 if (scan_tp + scan_fp + scan_fn) == 0 else 0.0
                total_tp += int(scan_tp)
                total_fp += int(scan_fp)
                total_fn += int(scan_fn)
                if scan_tp > 0:
                    scans_with_matches += 1
                per_scan.append({
                    'scan_row': int(i),
                    'scan_value': scan_value,
                    'tp': int(scan_tp),
                    'fp': int(scan_fp),
                    'fn': int(scan_fn),
                    'matches': int(scan_tp),
                    'den': int(scan_den),
                    'acc_pct': float(scan_acc),
                    'n_in': int(scan_n_in),
                    'n_out': int(scan_n_out),
                })

            common_ids = in_id_set & out_id_set
            union_ids = in_id_set | out_id_set
            id_overlap = {
                'common': int(len(common_ids)),
                'union': int(len(union_ids)),
                'input_only': int(len(in_id_set - out_id_set)),
                'output_only': int(len(out_id_set - in_id_set)),
                'n_input_ids': int(len(in_id_set)),
                'n_output_ids': int(len(out_id_set)),
            }

            self._total_in_valid = int(total_in_valid)
            self._total_out_valid = int(total_out_valid)
            self._total_tp = int(total_tp)
            self._total_fp = int(total_fp)
            self._total_fn = int(total_fn)
            self._fmoving_agree = int(fmoving_agree)
            self._fmoving_cooccur = int(fmoving_cooccur)
            self._scans_with_matches = int(scans_with_matches)
            self._n_scans = int(n_scans)
            self._per_scan = per_scan
            self._id_overlap = dict(id_overlap)

            if matched_tracks:
                plot_pairs = matched_tracks[:_TRACKER_PLOT_PAIR_CAP]
                self.matched_tracks_df = pd.DataFrame(plot_pairs)
            else:
                self.matched_tracks_df = pd.DataFrame()
            return {
                'matched_tracks': matched_tracks,
                'total_matches': int(total_tp),
                'total_fp': int(total_fp),
                'total_fn': int(total_fn),
                'total_in_valid': int(total_in_valid),
                'total_out_valid': int(total_out_valid),
                'scans_compared': int(n_scans),
                'scans_with_matches': int(scans_with_matches),
                'fmoving_agree': int(fmoving_agree),
                'fmoving_cooccur': int(fmoving_cooccur),
                'position_errors': [float(v) for v in position_errors],
                'velocity_errors': [float(v) for v in velocity_errors],
                'per_scan': per_scan,
                'id_overlap': dict(id_overlap),
            }

        except Exception as e:
            logger.error(f"Error matching tracker data: {e}")
            return {}

    def calculate_tracker_kpis(self, matching_results: Dict):
        """Calculate tracker KPIs from composite-key (scan, trkID) matching.

        Hero accuracy = TP / valid input tracks (recall); overall/Jaccard =
        TP / (TP + FP + FN); precision = TP / valid output tracks; F1 is the
        harmonic mean of precision and recall. Zero matches still yield a
        valid 0% result (never an empty kpi_results on extracted data).
        """
        try:
            mr = matching_results or {}
            matched_tracks = mr.get('matched_tracks', []) or []
            tp = int(mr.get('total_matches', len(matched_tracks)))
            total_fp = int(mr.get('total_fp', getattr(self, '_total_fp', 0)))
            total_fn = int(mr.get('total_fn', getattr(self, '_total_fn', 0)))
            total_in = int(mr.get('total_in_valid', getattr(self, '_total_in_valid', 0)))
            total_out = int(mr.get('total_out_valid', getattr(self, '_total_out_valid', 0)))
            scans_compared = int(mr.get('scans_compared', getattr(self, '_n_scans', 0)))
            scans_with_matches = int(mr.get('scans_with_matches',
                                            getattr(self, '_scans_with_matches', 0)))
            fm_agree = int(mr.get('fmoving_agree', getattr(self, '_fmoving_agree', 0)))
            fm_cooccur = int(mr.get('fmoving_cooccur', getattr(self, '_fmoving_cooccur', 0)))
            per_scan = mr.get('per_scan', getattr(self, '_per_scan', [])) or []
            id_overlap = dict(mr.get('id_overlap', getattr(self, '_id_overlap', {})) or {})

            pos_list = mr.get('position_errors', None)
            if pos_list is None:
                pos_list = []
                for m in matched_tracks:
                    try:
                        pos_list.append(float(m['spatial_distance']))
                    except Exception:
                        continue
            vel_list = mr.get('velocity_errors', None)
            if vel_list is None:
                vel_list = []
                for m in matched_tracks:
                    try:
                        v = float(np.linalg.norm(m['input_vel'] - m['output_vel']))
                    except Exception:
                        continue
                    if np.isfinite(v):
                        vel_list.append(v)
            pos_arr = np.array([float(v) for v in pos_list
                                if np.isfinite(float(v))], dtype=float)
            vel_arr = np.array([float(v) for v in vel_list
                                if np.isfinite(float(v))], dtype=float)

            hero = (float(tp) / float(total_in)) if total_in > 0 else 0.0
            prec = (float(tp) / float(total_out)) if total_out > 0 else 0.0
            jden = int(tp + total_fp + total_fn)
            jacc = (float(tp) / float(jden)) if jden > 0 else 0.0
            f1 = (2.0 * prec * hero / (prec + hero)) if (prec + hero) > 0 else 0.0
            accuracy_percentage = 100.0 * hero
            fm_pct = (100.0 * float(fm_agree) / float(fm_cooccur)) if fm_cooccur > 0 else 0.0

            if pos_arr.size:
                pos_stats = {
                    'position_error_mean': float(np.mean(pos_arr)),
                    'position_error_std': float(np.std(pos_arr)),
                    'position_error_max': float(np.max(pos_arr)),
                    'position_error_min': float(np.min(pos_arr)),
                }
            else:
                pos_stats = {
                    'position_error_mean': 0.0,
                    'position_error_std': 0.0,
                    'position_error_max': 0.0,
                    'position_error_min': 0.0,
                }
            if vel_arr.size:
                vel_stats = {
                    'velocity_error_mean': float(np.mean(vel_arr)),
                    'velocity_error_std': float(np.std(vel_arr)),
                    'velocity_error_max': float(np.max(vel_arr)),
                    'velocity_error_min': float(np.min(vel_arr)),
                }
            else:
                vel_stats = {
                    'velocity_error_mean': float('nan'),
                    'velocity_error_std': float('nan'),
                    'velocity_error_max': float('nan'),
                    'velocity_error_min': float('nan'),
                }

            self.summary_stats = {
                'total_input_tracks': int(total_in),
                'total_output_tracks': int(total_out),
                'total_matches': int(tp),
                'accurate_matches': int(tp),
                'total_fp': int(total_fp),
                'total_fn': int(total_fn),
                'scans_compared': int(scans_compared),
                'scans_with_matches': int(scans_with_matches),
                'precision': round(float(100.0 * prec), 2),
                'recall': round(float(accuracy_percentage), 2),
                'f1': round(float(100.0 * f1), 2),
                'jaccard': round(float(100.0 * jacc), 2),
                'overall': round(float(100.0 * jacc), 2),
                'fmoving_agree': int(fm_agree),
                'fmoving_cooccur': int(fm_cooccur),
                'fmoving_agreement_pct': round(float(fm_pct), 2),
                'id_common': int(id_overlap.get('common', 0)),
                'id_union': int(id_overlap.get('union', 0)),
                'id_input_only': int(id_overlap.get('input_only', 0)),
                'id_output_only': int(id_overlap.get('output_only', 0)),
                'per_scan': per_scan,
            }
            self.summary_stats.update(pos_stats)
            self.summary_stats.update(vel_stats)

            tx = float(getattr(self, 'tx', float(self.thresholds.get('vcs_xposn_threshold', 0.01))))
            ty = float(getattr(self, 'ty', float(self.thresholds.get('vcs_yposn_threshold', 0.01))))
            tvx = float(getattr(self, 'tvx', float(self.thresholds.get('vcs_xvel_threshold', 0.02))))
            tvy = float(getattr(self, 'tvy', float(self.thresholds.get('vcs_yvel_threshold', 0.02))))
            # Store KPI results (numerator/denominator + compat threshold-key
            # naming kept for HTML/sidecar-regex compat)
            self.kpi_results = {
                'accuracy': round(float(accuracy_percentage), 2),
                'numerator': int(tp),
                'denominator': int(total_in),
                'precision': round(float(100.0 * prec), 2),
                'recall': round(float(accuracy_percentage), 2),
                'f1': round(float(100.0 * f1), 2),
                'jaccard': round(float(100.0 * jacc), 2),
                'overall': round(float(100.0 * jacc), 2),
                'total_fp': int(total_fp),
                'total_fn': int(total_fn),
                'fmoving_agree': int(fm_agree),
                'fmoving_cooccur': int(fm_cooccur),
                'per_scan': per_scan,
                'summary_stats': self.summary_stats,
                'matching_mode': 'id-indexed (scan,trkID) + kinematics matrix',
                'thresholds': {
                    'position_threshold': float(tx),
                    'time_threshold': 0.0,
                    'accuracy_threshold': float(ty),
                    'vcs_xposn_threshold': float(tx),
                    'vcs_yposn_threshold': float(ty),
                    'vcs_xvel_threshold': float(tvx),
                    'vcs_yvel_threshold': float(tvy),
                    'max_valid_distance': float(self.max_valid_distance_m),
                    'f_moving_rule': 'f_moving flags must agree; NaN/unset flag counts as mismatch',
                    'matching_mode': 'id-indexed (scan,trkID) + kinematics matrix',
                }
            }

        except Exception as e:
            logger.error(f"Error calculating tracker KPIs: {e}")
            try:
                total_in = int(getattr(self, '_total_in_valid', 0))
            except Exception:
                total_in = 0
            try:
                total_out = int(getattr(self, '_total_out_valid', 0))
            except Exception:
                total_out = 0
            self.summary_stats = {
                'total_input_tracks': total_in,
                'total_output_tracks': total_out,
                'total_matches': 0,
                'accurate_matches': 0,
                'total_fp': int(getattr(self, '_total_fp', 0)),
                'total_fn': int(getattr(self, '_total_fn', 0)),
                'scans_compared': int(getattr(self, '_n_scans', 0)),
                'scans_with_matches': 0,
                'precision': 0.0,
                'recall': 0.0,
                'f1': 0.0,
                'jaccard': 0.0,
                'overall': 0.0,
                'position_error_mean': 0.0,
                'position_error_std': 0.0,
                'position_error_max': 0.0,
                'position_error_min': 0.0,
                'velocity_error_mean': float('nan'),
                'velocity_error_std': float('nan'),
                'velocity_error_max': float('nan'),
                'velocity_error_min': float('nan'),
                'fmoving_agree': 0,
                'fmoving_cooccur': 0,
                'fmoving_agreement_pct': 0.0,
                'per_scan': list(getattr(self, '_per_scan', []) or []),
            }
            self.kpi_results = {
                'accuracy': 0.0,
                'numerator': 0,
                'denominator': total_in,
                'precision': 0.0,
                'recall': 0.0,
                'f1': 0.0,
                'jaccard': 0.0,
                'overall': 0.0,
                'summary_stats': self.summary_stats,
                'matching_mode': 'id-indexed (scan,trkID) + kinematics matrix',
                'thresholds': {
                    'position_threshold': float(getattr(self, 'tx', 0.01)),
                    'time_threshold': 0.0,
                    'accuracy_threshold': float(getattr(self, 'ty', 0.01)),
                    'vcs_xposn_threshold': float(getattr(self, 'tx', 0.01)),
                    'vcs_yposn_threshold': float(getattr(self, 'ty', 0.01)),
                    'vcs_xvel_threshold': float(getattr(self, 'tvx', 0.02)),
                    'vcs_yvel_threshold': float(getattr(self, 'tvy', 0.02)),
                    'f_moving_rule': 'f_moving flags must agree; NaN/unset flag counts as mismatch',
                    'matching_mode': 'id-indexed (scan,trkID) + kinematics matrix',
                }
            }
            
    def generate_plots(self):
        """Generate interactive plots for tracker data (2D x/y, no z required)"""
        try:
            if self.matched_tracks_df is None or self.matched_tracks_df.empty:
                return "", ""

            def _xy(series, idx):
                vals = []
                for pos in series:
                    try:
                        vals.append(float(np.asarray(pos).ravel()[idx]))
                    except Exception:
                        vals.append(float('nan'))
                return vals

            in_x, in_y = _xy(self.matched_tracks_df['input_pos'], 0), _xy(self.matched_tracks_df['input_pos'], 1)
            out_x, out_y = _xy(self.matched_tracks_df['output_pos'], 0), _xy(self.matched_tracks_df['output_pos'], 1)

            # Position comparison plot
            fig_pos = sp.make_subplots(rows=2, cols=2,
                                      subplot_titles=('X Position Comparison', 'Y Position Comparison',
                                                    'Bird-eye Matched Positions', 'Position Error Distribution'),
                                      specs=[[{"secondary_y": False}, {"secondary_y": False}],
                                            [{"secondary_y": False}, {"secondary_y": False}]])

            # X position
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=in_x,
                                       mode='lines+markers', name='Input X', line=dict(color='blue')),
                            row=1, col=1)
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=out_x,
                                       mode='lines+markers', name='Output X', line=dict(color='red')),
                            row=1, col=1)

            # Y position
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=in_y,
                                       mode='lines+markers', name='Input Y', line=dict(color='blue')),
                            row=1, col=2)
            fig_pos.add_trace(go.Scatter(x=self.matched_tracks_df.index,
                                       y=out_y,
                                       mode='lines+markers', name='Output Y', line=dict(color='red')),
                            row=1, col=2)

            # Bird-eye scatter
            fig_pos.add_trace(go.Scatter(x=in_x, y=in_y,
                                       mode='markers', name='Input X-Y',
                                       marker=dict(color='blue', size=4)),
                            row=2, col=1)
            fig_pos.add_trace(go.Scatter(x=out_x, y=out_y,
                                       mode='markers', name='Output X-Y',
                                       marker=dict(color='red', size=4)),
                            row=2, col=1)

            # Position error histogram
            fig_pos.add_trace(go.Histogram(x=self.matched_tracks_df['spatial_distance'],
                                         name='Position Error', nbinsx=20),
                            row=2, col=2)

            fig_pos.update_layout(height=800, width=1200, title_text="Tracker Position Analysis")
            pos_plot_html = pio.to_html(fig_pos, full_html=False, include_plotlyjs='cdn')

            # 2D bird-eye plot of matched tracks
            fig_2d = go.Figure()

            fig_2d.add_trace(go.Scatter(x=in_x, y=in_y,
                                        mode='markers',
                                        name='Input Tracks',
                                        marker=dict(size=5, color='blue')))

            fig_2d.add_trace(go.Scatter(x=out_x, y=out_y,
                                        mode='markers',
                                        name='Output Tracks',
                                        marker=dict(size=5, color='red')))

            fig_2d.update_layout(title='Bird-eye Track Positions',
                                 xaxis_title='X Position (m)',
                                 yaxis_title='Y Position (m)',
                                 yaxis=dict(scaleanchor='x', scaleratio=1))

            scatter_2d_html = pio.to_html(fig_2d, full_html=False, include_plotlyjs='cdn')

            return pos_plot_html, scatter_2d_html

        except Exception as e:
            logger.error(f"Error generating tracker plots: {e}")
            return "", ""
            
    def generate_html_report(self):
        """Generate HTML report for tracker KPIs (composite-key matching)"""
        try:
            # Generate plots
            pos_plot_html, scatter_3d_html = self.generate_plots()

            summary = self.summary_stats or {}
            kpi = self.kpi_results or {}
            thr = kpi.get('thresholds', {}) or {}
            per_scan = (kpi.get('per_scan', None)
                        if isinstance(kpi, dict) else None)
            if per_scan is None:
                per_scan = summary.get('per_scan', None) if isinstance(summary, dict) else None
            if per_scan is None:
                per_scan = getattr(self, '_per_scan', [])
            per_scan = per_scan or []

            rows = []
            for ps in per_scan:
                try:
                    scan_label = ps.get('scan_value', ps.get('scan_row', ''))
                    rows.append(
                        "<tr><td>{}</td><td>{}</td><td>{}</td><td>{}</td><td>{:.2f}%</td></tr>".format(
                            scan_label, int(ps.get('tp', 0)),
                            int(ps.get('fp', 0)), int(ps.get('fn', 0)),
                            float(ps.get('acc_pct', 0.0)),
                        )
                    )
                except Exception:
                    continue
            if rows:
                scan_table = (
                    "<table border='1' cellpadding='4' cellspacing='0'>"
                    "<thead><tr><th>Scan</th><th>TP</th><th>FP</th>"
                    "<th>FN</th><th>Acc%</th></tr></thead>"
                    "<tbody>" + "".join(rows) + "</tbody></table>"
                )
            else:
                scan_table = "<p>No common scans compared.</p>"

            def _fmt3(v):
                try:
                    f = float(v)
                except Exception:
                    return "n/a"
                return "{:.3f}".format(f) if np.isfinite(f) else "n/a"

            vel_mean_s = _fmt3(summary.get('velocity_error_mean', float('nan')))
            vel_std_s = _fmt3(summary.get('velocity_error_std', float('nan')))
            id_common = summary.get('id_common', 0)
            id_union = summary.get('id_union', 0)
            fm_rule = thr.get('f_moving_rule',
                              'f_moving flags must agree; NaN/unset flag counts as mismatch')

            self.html_content = f"""
            <div class="kpi-section">
                <h3>Tracker Matching KPI Results - {self.sensor_id}</h3>

                <div class="kpi-summary">
                    <p><strong>Tracker Accuracy:</strong> ({kpi.get('numerator', 0)}/{kpi.get('denominator', 0)}) → <strong>{kpi.get('accuracy', 0):.2f}%</strong></p>
                    <p><strong>Total Matched Tracks:</strong> {summary.get('total_matches', 0)}</p>
                    <p><strong>Position Error Mean:</strong> {summary.get('position_error_mean', 0):.3f} m</p>
                    <p><strong>Position Error Std:</strong> {summary.get('position_error_std', 0):.3f} m</p>
                    <p><strong>Precision:</strong> {kpi.get('precision', 0):.2f}% &nbsp; <strong>Recall:</strong> {kpi.get('recall', 0):.2f}% &nbsp; <strong>F1:</strong> {kpi.get('f1', 0):.2f}% &nbsp; <strong>Overall/Jaccard:</strong> {kpi.get('jaccard', kpi.get('overall', 0)):.2f}%</p>
                    <p><strong>ID overlap:</strong> common-IDs {id_common}/{id_union} (input-only {summary.get('id_input_only', 0)}, output-only {summary.get('id_output_only', 0)}) &nbsp; <strong>f_moving agreement:</strong> {summary.get('fmoving_agree', 0)}/{summary.get('fmoving_cooccur', 0)} ({summary.get('fmoving_agreement_pct', 0):.2f}%)</p>
                </div>

                <details open>
                    <summary><strong>How to read this report</strong></summary>
                    <ul>
                        <li>Hero <strong>Tracker Accuracy</strong> = TP / valid input tracks (same as recall). <strong>Overall/Jaccard</strong> = TP / (TP + FP + FN); <strong>Precision</strong> = TP / valid output tracks; <strong>F1</strong> is the harmonic mean of precision and recall.</li>
                        <li><strong>ID-overlap diagnostic common-IDs {id_common}/{id_union}</strong>: distinct track IDs seen in both input and output (union {id_union}). Low overlap means misses come from ID presence (input-only / output-only), not from the kinematics matrix.</li>
                        <li><strong>Flag rule:</strong> {fm_rule}. A velocity-axis gate is skipped for a pair when either side is NaN (recorded on the pair; non-finite velocity errors are excluded from velocity stats).</li>
                        <li>Per-scan <strong>Acc%</strong> = TP / (TP + FN) of that scan (100% when the scan is empty, 0% when only spurious outputs are present).</li>
                    </ul>
                </details>

                <details>
                    <summary><strong>Detailed Results</strong></summary>

                    <h4>Track Statistics</h4>
                    <ul>
                        <li>Total Input Tracks: {summary.get('total_input_tracks', 0)}</li>
                        <li>Total Output Tracks: {summary.get('total_output_tracks', 0)}</li>
                        <li>Successfully Matched: {summary.get('total_matches', 0)}</li>
                        <li>Accurate Matches: {summary.get('accurate_matches', 0)}</li>
                        <li>False Positives (unpaired output IDs): {summary.get('total_fp', kpi.get('total_fp', 0))}</li>
                        <li>False Negatives (unpaired or matrix-failed input IDs): {summary.get('total_fn', kpi.get('total_fn', 0))}</li>
                        <li>Scans Compared: {summary.get('scans_compared', 0)}; Scans With Matches: {summary.get('scans_with_matches', 0)}</li>
                    </ul>

                    <h4>Per-scan Matches</h4>
                    {scan_table}

                    <h4>Position Error Statistics</h4>
                    <ul>
                        <li>Mean Error: {summary.get('position_error_mean', 0):.3f} m</li>
                        <li>Std Deviation: {summary.get('position_error_std', 0):.3f} m</li>
                        <li>Maximum Error: {summary.get('position_error_max', 0):.3f} m</li>
                        <li>Minimum Error: {summary.get('position_error_min', 0):.3f} m</li>
                        <li>Velocity Mean Error: {vel_mean_s} m/s; Std: {vel_std_s} m/s; Max: {_fmt3(summary.get('velocity_error_max', float('nan')))} m/s; Min: {_fmt3(summary.get('velocity_error_min', float('nan')))} m/s</li>
                    </ul>

                    <h4>Threshold Configuration</h4>
                    <ul>
                        <li>Position matching gate: {kpi.get('thresholds', {}).get('position_threshold', 0):.4f} m</li>
                        <li>Accuracy gate: {kpi.get('thresholds', {}).get('accuracy_threshold', 0):.4f} m</li>
                        <li>Time gate: {kpi.get('thresholds', {}).get('time_threshold', 0)} s (no time gate in this KPI)</li>
                        <li>vcs_xposn gate: {thr.get('vcs_xposn_threshold', 0):.4f} m; vcs_yposn gate: {thr.get('vcs_yposn_threshold', 0):.4f} m; vcs_xvel gate: {thr.get('vcs_xvel_threshold', 0):.4f} m/s; vcs_yvel gate: {thr.get('vcs_yvel_threshold', 0):.4f} m/s</li>
                        <li>f_moving rule: {fm_rule}</li>
                        <li>Matching mode: {kpi.get('matching_mode', 'id-indexed (scan,trkID) + kinematics matrix')}</li>
                    </ul>

                    <h4>Analysis Description</h4>
                    <p>This KPI measures tracker accuracy by matching tracks between input (vehicle) and output (simulation)
                    per scan on the composite key (scan, trkID) with one-to-one consume, mirroring CAN-KPI hashmap discipline.
                    A co-occurring ID counts as a true positive only when the kinematics matrix passes (|dx| &lt;= tx,
                    |dy| &lt;= ty, |dvx| &lt;= tvx, |dvy| &lt;= tvy with a velocity axis skipped when either side is NaN)
                    and the f_moving flags agree. Hero accuracy = TP / valid input tracks; overall/Jaccard = TP / (TP + FP + FN).</p>

                    <h4>Interactive Plots</h4>
                    <details>
                        <summary><strong>Plot A: Position Comparison and Error Distribution</strong></summary>
                        {pos_plot_html}
                    </details>

                    <details>
                        <summary><strong>Plot B: Bird-eye Track Position Visualization</strong></summary>
                        {scatter_3d_html}
                    </details>
                </details>
            </div>
            <hr>
            """

        except Exception as e:
            logger.error(f"Error generating HTML report: {e}")
            self.html_content = f"<p>Error generating tracker KPI report: {e}</p>"

    def get_results(self):
        """Return KPI results and HTML content"""
        return {
            'kpi_results': self.kpi_results,
            'html_content': self.html_content,
            'success': bool(self.kpi_results),
            'sensor_id': self.sensor_id,
        }

# Main processing function to be called by KPI factory
def process_tracker_kpi(input_data: KPI_DataModelStorage, output_data: KPI_DataModelStorage, 
                       sensor_id: str, stream_name: str) -> dict:
    """
    Main function to process tracker KPIs from HDF data
    
    Args:
        input_data: KPI_DataModelStorage with input/vehicle data
        output_data: KPI_DataModelStorage with output/simulation data  
        sensor_id: Sensor identifier
        stream_name: Stream name being processed
        
    Returns:
        dict: KPI results and HTML content
    """
    try:
        processor = TrackerMappingKPIHDF(input_data, output_data, sensor_id, stream_name)
        success = processor.process_tracker_matching()
        
        if success:
            return processor.get_results()
        else:
            return {
                'kpi_results': {},
                'html_content': f"<p>Failed to process tracker KPIs for {sensor_id}</p>",
                'success': False
            }
            
    except Exception as e:
        logger.error(f"Error in process_tracker_kpi: {e}")
        return {
            'kpi_results': {},
            'html_content': f"<p>Error processing tracker KPIs: {e}</p>",
            'success': False
        }
