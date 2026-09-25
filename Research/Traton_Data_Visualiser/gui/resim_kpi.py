"""
resim_kpi.py
============
Pure-computation module for Resim KPI analysis.  No Qt dependencies.

Two analysis modes
------------------
Frame-to-Frame (F2F)
    Each scan-index present in both datasets is processed independently.
    Orig baseline = Moving objects (DynamicProperty == 3) only.
    Resim match pool = Moving (ghost-eligible) + Stopped/Stationary (match
    candidates only — not counted as ghosts if unmatched).
    Matching uses the Hungarian algorithm on AABB-IoU scores.
    A match is accepted when IoU >= 1 %.  For objects where both length and
    width are < 0.6 m, IoU is unreliable; these are instead matched by
    centroid distance <= 0.6 m (small-object distance fallback).
    Tracking IDs are deliberately ignored because the tracker may assign
    different IDs after reprocessing.
    Each object-frame contributes its ROI multiplier (0.0–1.0) to the score
    accumulators; objects outside all ROIs are excluded from scoring.

Track-to-Track (T2T)
    Pre-computed ObjectTrack lists (Moving, Stopped, and Stationary objects)
    are used.  Each original track is paired with the resim track whose
    cumulative per-scan match score is highest (Hungarian algorithm).
    Unmatched resim tracks are ghost tracks.
    Each track contributes its ROI multiplier (best zone it ever entered —
    "closest range" rule) to the score accumulators.

RESIM Score
-----------
A composite 0–100 score is computed per radar side:
    RESIM Score = 0.40 × TP Events + 0.40 × FP Events + 0.20 × Accuracy

Accuracy is based on P95 signal errors vs configured tolerances:
    LonPosition ±0.5 m, LatPosition ±0.5 m,
    LonGndVel ±1.0 m/s, LatGndVel ±1.0 m/s.
    HeadingAngle (±15 °) is computed and displayed for information only —
    it does NOT contribute to the Accuracy score.

Region of Interest (ROI)
------------------------
Only objects/tracks within defined ROI zones are counted toward any score.
Three nested zones, checked in priority order (first match wins):
    ROI 1: lon [−30, +10] m, lat [−6, +6] m   → weight 1.00×
    ROI 2: lon [−50, +20] m, lat [−9, +9] m   → weight 0.75×
    ROI 3: lon [−80, +25] m, lat [−12, +12] m → weight 0.50×
    Outside: weight 0.00 (excluded from all score accumulators).
For T2T, the best (highest) ROI multiplier achieved anywhere in the track
lifetime is used for the entire track ("closest range" rule).
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Callable, Dict, List, Optional, Tuple

import numpy as np
from scipy.optimize import linear_sum_assignment

from object_track import ObjectTrack, strip_slot_suffix


# ---------------------------------------------------------------------------
# KPI signal definitions  (name as it appears in raw_signals / ObjectRecord)
# ---------------------------------------------------------------------------

_KPI_SIGNALS: List[Tuple[str, str]] = [
    ("LonPosition",  "m"),
    ("LatPosition",  "m"),
    ("LonGndVel",    "m/s"),
    ("LatGndVel",    "m/s"),
    ("HeadingAngle", "°"),   # stored in rad, converted to degrees in deltas
]
_HEADING_SIG = "HeadingAngle"

# ---------------------------------------------------------------------------
# Matching / accuracy thresholds
# ---------------------------------------------------------------------------

_IOU_THRESHOLD   = 0.01   # minimum IoU to accept a match
_SMALL_OBJ_M     = 0.6    # both L and W below this → distance fallback instead of IoU
_DIST_FALLBACK_M = 0.6    # max centroid distance (m) allowed for small-object match
_NO_MATCH_COST   = 1e9    # sentinel cost value meaning "no valid match"
_GHOST_FRAME_COV_THRESHOLD = 0.50  # min frame coverage to call outcompeted track a non-ghost
_MIN_TRACK_DURATION_S      = 0.200 # tracks shorter than this are excluded from scoring

# Tolerance thresholds for accuracy score (P95 error must be ≤ tolerance for 100 %)
# HeadingAngle is retained in _TOLERANCES so it appears in the signal table,
# but it is deliberately excluded from the composite accuracy score calculation.
_TOLERANCES: Dict[str, float] = {
    "LonPosition":  0.5,    # m
    "LatPosition":  0.5,    # m
    "LonGndVel":    1.0,    # m/s
    "LatGndVel":    1.0,    # m/s
    "HeadingAngle": 15.0,   # degrees — informational only, not scored
}

# ---------------------------------------------------------------------------
# Region of Interest (ROI) — gates which objects/tracks influence TP/FP scores
# ---------------------------------------------------------------------------
# Convention: lon = longitudinal (positive = forward), lat = lateral (positive = left).
# Zones are checked in priority order; first match wins.
# Multiplier 0.0 means the object/track does NOT count toward any score.
_ROI_ZONES = (
    # (lon_min, lon_max, lat_min, lat_max, multiplier)
    (-30.0, +10.0,  -6.0,  +6.0, 1.00),   # ROI 1 — highest weight
    (-50.0, +20.0,  -9.0,  +9.0, 0.75),   # ROI 2
    (-80.0, +25.0, -12.0, +12.0, 0.50),   # ROI 3
)


def _roi_mult_pos(lon: float, lat: float) -> float:
    """ROI weight multiplier for a single (lon, lat) position."""
    for lon_min, lon_max, lat_min, lat_max, mult in _ROI_ZONES:
        if lon_min <= lon <= lon_max and lat_min <= lat <= lat_max:
            return mult
    return 0.0   # outside all ROIs → excluded from scoring


def _roi_mult_track(track: ObjectTrack) -> float:
    """Best (highest) ROI multiplier the track ever achieved across its scan records.

    For T2T scoring we use the 'closest range' rule: a track that passed
    through ROI 1 at any point carries 1× weight for its entire lifetime.
    """
    best = 0.0
    for rec in track.records:
        lon = _get_sig(rec.raw_signals, "LonPosition")
        lat = _get_sig(rec.raw_signals, "LatPosition")
        if lon is None or lat is None:
            continue
        m = _roi_mult_pos(lon, lat)
        if m > best:
            best = m
        if best >= 1.0:   # can't improve further
            break
    return best


# ---------------------------------------------------------------------------
# Result data-classes
# ---------------------------------------------------------------------------

@dataclass
class DeltaStats:
    """Error statistics for one signal over a set of observations."""
    n:       int
    mean:    float       # signed mean delta  (orig − resim)
    rmse:    float       # sqrt(mean² + std²) — combined bias + scatter
    std:     float       # standard deviation of delta
    median:  float       # median of delta
    p95:     float       # 95th-percentile of |delta|
    max_abs: float       # worst-case |delta|
    unit:    str = ""


@dataclass
class SideFrameKpi:
    side:             str
    total:            int    # orig Moving object-frames considered
    matched:          int    # object-frames with an accepted resim match
    total_resim:      int    # resim Moving object-frames considered
    ghost:            int    # resim object-frames with no orig match
    availability_pct: float  # matched / total × 100
    ghost_pct:        float  # ghost / total_resim × 100
    mean_iou:         float  # mean IoU over matched pairs
    tp_score:     float  # 0-100  True Positive Events (matched / orig)
    fp_score:  float  # 0-100  False Positive Events (1 - spurious/resim)
    accuracy_score:   float  # 0-100  (P95 errors vs tolerances)
    resim_score:      float  # composite 0-100
    signal_stats:     Dict[str, DeltaStats] = field(default_factory=dict)


@dataclass
class FrameKpiResult:
    left:  SideFrameKpi
    right: SideFrameKpi


@dataclass
class TrackMatch:
    orig_track:         Optional[ObjectTrack]   # None → unmatched resim-only row
    resim_track:        Optional[ObjectTrack]   # None → unmatched orig-only row
    coverage_pct:       float = 0.0             # orig-side: common/orig_scans × 100
    resim_coverage_pct: float = 0.0             # resim-side: common/resim_scans × 100
    mean_iou:           float = 0.0             # mean per-scan IoU over common scans
    is_ghost:           bool  = True             # False = outcompeted but overlapping orig
    is_short:           bool  = False            # True = track < _MIN_TRACK_DURATION_S
    raw_deltas:         Dict[str, List[float]] = field(default_factory=dict)
    signal_stats:       Dict[str, DeltaStats]  = field(default_factory=dict)


@dataclass
class SideTrackKpi:
    side:                   str
    n_orig:                 int    # total original tracks on this side
    n_matched:              int    # orig tracks assigned a resim match
    n_resim:                int    # total resim tracks on this side
    n_resim_matched:        int    # resim tracks assigned an orig match (== n_matched)
    avg_coverage_pct:       float  # avg orig-side coverage across matched pairs
    avg_resim_coverage_pct: float  # avg resim-side coverage across matched pairs
    track_count_ratio:      float  # n_resim / max(n_orig, 1)
    mean_iou:               float  # mean per-scan IoU over all matched pairs
    tp_score:           float  # 0-100  True Positive Events (matched / orig)
    fp_score:           float  # 0-100  False Positive Events (1 - spurious/resim)
    accuracy_score:         float  # 0-100  (P95 errors vs tolerances)
    resim_score:            float  # composite 0-100
    signal_stats:           Dict[str, DeltaStats] = field(default_factory=dict)
    matches:                List[TrackMatch]       = field(default_factory=list)


@dataclass
class TrackKpiResult:
    left:  SideTrackKpi
    right: SideTrackKpi


# ---------------------------------------------------------------------------
# Internal helpers
# ---------------------------------------------------------------------------

def _get_sig(raw_signals: dict, name: str) -> Optional[float]:
    """Return float value of stripped signal *name* from raw_signals, or None."""
    for k, v in raw_signals.items():
        if k.startswith("__"):
            continue
        if strip_slot_suffix(k) == name:
            try:
                return float(v)
            except (TypeError, ValueError):
                return None
    return None


def _aabb_iou_obj(o, r) -> float:
    """AABB IoU between two ObjectRecords (uses ref_lon/ref_lat/length/width)."""
    lon1, lat1 = o.ref_lon, o.ref_lat
    lon2, lat2 = r.ref_lon, r.ref_lat
    l1, w1 = max(float(o.length), 0.3), max(float(o.width), 0.3)
    l2, w2 = max(float(r.length), 0.3), max(float(r.width), 0.3)
    ol = max(0.0, min(lon1 + l1/2, lon2 + l2/2) - max(lon1 - l1/2, lon2 - l2/2))
    ow = max(0.0, min(lat1 + w1/2, lat2 + w2/2) - max(lat1 - w1/2, lat2 - w2/2))
    inter = ol * ow
    if inter == 0.0:
        return 0.0
    return inter / max(l1*w1 + l2*w2 - inter, 1e-9)


def _aabb_iou_sig(o_raw: dict, r_raw: dict) -> float:
    """AABB IoU using raw_signals dicts (for TrackRecord comparison)."""
    lon1 = _get_sig(o_raw, "LonPosition")
    lat1 = _get_sig(o_raw, "LatPosition")
    l1   = _get_sig(o_raw, "Length") or 0.3
    w1   = _get_sig(o_raw, "Width")  or 0.3
    lon2 = _get_sig(r_raw, "LonPosition")
    lat2 = _get_sig(r_raw, "LatPosition")
    l2   = _get_sig(r_raw, "Length") or 0.3
    w2   = _get_sig(r_raw, "Width")  or 0.3
    if None in (lon1, lat1, lon2, lat2):
        return 0.0
    l1, w1 = max(l1, 0.3), max(w1, 0.3)
    l2, w2 = max(l2, 0.3), max(w2, 0.3)
    ol = max(0.0, min(lon1 + l1/2, lon2 + l2/2) - max(lon1 - l1/2, lon2 - l2/2))
    ow = max(0.0, min(lat1 + w1/2, lat2 + w2/2) - max(lat1 - w1/2, lat2 - w2/2))
    inter = ol * ow
    if inter == 0.0:
        return 0.0
    return inter / max(l1*w1 + l2*w2 - inter, 1e-9)


def _heading_delta_deg(orig_rad: float, resim_rad: float) -> float:
    """Wrap-corrected heading difference in degrees (orig − resim)."""
    diff = orig_rad - resim_rad
    diff = (diff + np.pi) % (2.0 * np.pi) - np.pi
    return float(np.degrees(diff))


def _delta_stats(deltas: List[float], unit: str = "") -> DeltaStats:
    if not deltas:
        return DeltaStats(0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, unit)
    arr = np.array(deltas, dtype=float)
    mean = float(np.mean(arr))
    std  = float(np.std(arr))
    return DeltaStats(
        n       = len(arr),
        mean    = mean,
        rmse    = float(np.sqrt(mean**2 + std**2)),
        std     = std,
        median  = float(np.median(arr)),
        p95     = float(np.percentile(np.abs(arr), 95)),
        max_abs = float(np.max(np.abs(arr))),
        unit    = unit,
    )


def _obj_kpi_values(obj) -> Dict[str, float]:
    """Extract KPI signal values from an ObjectRecord using direct fields."""
    return {
        "LonPosition":  float(obj.ref_lon),
        "LatPosition":  float(obj.ref_lat),
        "LonGndVel":    float(obj.lon_vel),
        "LatGndVel":    float(obj.lat_vel),
        "HeadingAngle": float(obj.heading),
    }


def _rec_kpi_values(raw: dict) -> Dict[str, Optional[float]]:
    """Extract KPI signal values from a TrackRecord.raw_signals dict."""
    return {sig: _get_sig(raw, sig) for sig, _ in _KPI_SIGNALS}


# ---------------------------------------------------------------------------
# Small-object and distance helpers
# ---------------------------------------------------------------------------

def _is_small_obj_frame(o) -> bool:
    """True when an ObjectRecord's bounding box is below the small-object threshold
    (both length AND width < _SMALL_OBJ_M)."""
    return abs(o.length) < _SMALL_OBJ_M and abs(o.width) < _SMALL_OBJ_M


def _is_small_obj_sig(raw: dict) -> bool:
    """True when raw_signals indicate a small object (both L and W < _SMALL_OBJ_M)."""
    l = _get_sig(raw, "Length") or _SMALL_OBJ_M
    w = _get_sig(raw, "Width")  or _SMALL_OBJ_M
    return abs(l) < _SMALL_OBJ_M and abs(w) < _SMALL_OBJ_M


def _centroid_dist(lon1: float, lat1: float, lon2: float, lat2: float) -> float:
    return float(np.sqrt((lon1 - lon2) ** 2 + (lat1 - lat2) ** 2))


# ---------------------------------------------------------------------------
# RESIM score helpers
# ---------------------------------------------------------------------------

def _accuracy_score(signal_stats: Dict[str, DeltaStats]) -> float:
    """
    0-100 accuracy score based on P95 errors vs configured tolerances.
    Each signal scores 100 % when P95 <= tolerance, falling linearly to 0 %
    at 4 × tolerance.  The final score is the mean across all scored signals.

    HeadingAngle is excluded from scoring (kept in signal_stats for display).
    Accuracy is based solely on position (LonPosition, LatPosition) and
    velocity (LonGndVel, LatGndVel).
    """
    scores = []
    for sig, tol in _TOLERANCES.items():
        if sig == _HEADING_SIG:
            continue   # informational only — not included in composite score
        if sig in signal_stats and signal_stats[sig].n > 0:
            p95   = signal_stats[sig].p95
            score = min(100.0, max(0.0, (4.0 * tol - p95) / (3.0 * tol)) * 100.0)
            scores.append(score)
    return float(sum(scores) / len(scores)) if scores else 100.0


def _resim_scores(
        matched_orig:  int,
        total_orig:    int,
        ghost:         int,
        total_resim:   int,
        signal_stats:  Dict[str, DeltaStats],
) -> Tuple[float, float, float, float]:
    """
    Return (tp_score, fp_score, accuracy_score, composite_score)
    all in the range 0-100.

    tp  = matched_orig / total_orig × 100          (True Positive Events)
    fp  = (1 - spurious / total_resim) × 100       (False Positive Events: higher = fewer FPs)
    accuracy  = P95-based score across tolerance thresholds
    composite = 0.40 × tp + 0.40 × fp + 0.20 × accuracy
    """
    tp        = 100.0 * matched_orig / max(total_orig,  1)
    fp        = max(0.0, 100.0 - 100.0 * ghost / max(total_resim, 1))
    accuracy  = _accuracy_score(signal_stats)
    composite = 0.4 * tp + 0.4 * fp + 0.2 * accuracy
    return tp, fp, accuracy, composite


# ---------------------------------------------------------------------------
# Per-scan match score (IoU with small-object distance fallback)
# ---------------------------------------------------------------------------

def _scan_score_sig(o_raw: dict, r_raw: dict) -> Tuple[float, float]:
    """
    Match score and raw IoU for one scan pair.
    Returns (match_score, iou) where match_score may use distance fallback
    for small objects while iou is always the pure bounding-box overlap.
    """
    iou = _aabb_iou_sig(o_raw, r_raw)
    if iou >= _IOU_THRESHOLD:
        return iou, iou
    # Small-object distance fallback
    if _is_small_obj_sig(o_raw) or _is_small_obj_sig(r_raw):
        lon1 = _get_sig(o_raw, "LonPosition")
        lat1 = _get_sig(o_raw, "LatPosition")
        lon2 = _get_sig(r_raw, "LonPosition")
        lat2 = _get_sig(r_raw, "LatPosition")
        if None not in (lon1, lat1, lon2, lat2):
            d = _centroid_dist(lon1, lat1, lon2, lat2)
            if d <= _DIST_FALLBACK_M:
                # Score in [0, 0.5] so IoU-based matches are always preferred
                return 0.5 * (1.0 - d / _DIST_FALLBACK_M), iou
    return 0.0, iou


# ---------------------------------------------------------------------------
# Frame-to-Frame KPI
# ---------------------------------------------------------------------------

def compute_frame_kpi(orig_model, resim_model,
                      progress_cb: Optional[Callable] = None) -> FrameKpiResult:
    """
    Compute frame-to-frame comparison KPI.

    Only Moving (DynamicProperty == 3) and Stopped (DynamicProperty == 2)
    objects are considered for F2F counting.
    Stationary (1) objects are excluded from F2F to avoid inflating
    denominators; Stopped↔Stationary cross-matching is handled at Track-to-Track level.
    one-to-one assignment between original and resim objects on each radar
    A pair is accepted when IoU >= _IOU_THRESHOLD (1 %) or, for
    objects where both L and W < _SMALL_OBJ_M (0.6 m), when the centroid
    distance is <= _DIST_FALLBACK_M (0.6 m).
    A resim object is a ghost only when it has NO valid overlap/distance
    match with ANY orig object in that scan — i.e. it was not just
    outcompeted by a better pairing but is genuinely undetectable in orig.
    """
    def _cb(pct: int, msg: str) -> None:
        if progress_cb:
            progress_cb(pct, msg)

    _cb(0, "Frame KPI: finding common scan indices…")
    common_scans = sorted(
        set(orig_model.scan_indices) & set(resim_model.scan_indices)
    )

    # Accumulators per side
    counts: Dict[str, Dict[str, int]] = {
        "L": {"total": 0, "matched": 0, "total_resim": 0, "ghost": 0,
              "iou_count": 0},
        "R": {"total": 0, "matched": 0, "total_resim": 0, "ghost": 0,
              "iou_count": 0},
    }
    iou_sum: Dict[str, float] = {"L": 0.0, "R": 0.0}
    sig_deltas: Dict[str, Dict[str, List[float]]] = {
        "L": {s: [] for s, _ in _KPI_SIGNALS},
        "R": {s: [] for s, _ in _KPI_SIGNALS},
    }

    n = max(len(common_scans), 1)
    for i, scan in enumerate(common_scans):
        if i % 100 == 0:
            _cb(int(i / n * 88), f"Frame KPI: scan {scan} ({i}/{n})…")

        # Baseline = Moving objects (dyn_property == 3) in the original data.
        # TP/FP denominator uses Moving objects only.
        #
        # Resim match pool = Moving (ghost-eligible) + Stopped/Stationary
        # (match candidates only).  This lets a Moving orig be credited as a
        # True Positive when the resim classified it as Stopped or Stationary.
        # Non-Moving resim objects that have NO Moving orig counterpart are NOT
        # counted as ghosts — they are infrastructure/background detections
        # that are simply out of scope for this baseline.
        #
        # Implementation: put Moving resim objects FIRST so indices 0..n_r_move-1
        # are the ghost pool; Stopped/Stationary follow at n_r_move..n_r-1.
        orig_objs         = [o for o in orig_model.get_objects(scan)
                             if o.dyn_property == 3]
        resim_objs_move   = [o for o in resim_model.get_objects(scan)
                             if o.dyn_property == 3]
        resim_objs_other  = [o for o in resim_model.get_objects(scan)
                             if o.dyn_property in (1, 2)]

        for side in ("L", "R"):
            o_list    = [o for o in orig_objs        if o.side == side]
            r_move    = [o for o in resim_objs_move  if o.side == side]
            r_extra   = [o for o in resim_objs_other if o.side == side]
            r_list    = r_move + r_extra   # Moving first — ghost pool = [0, n_r_move)
            n_o       = len(o_list)
            n_r_move  = len(r_move)        # FP/ghost denominator
            n_r       = len(r_list)        # full match pool

            # ROI-weighted accumulation: each object contributes its ROI
            # multiplier instead of a flat count of 1.
            for o in o_list:
                counts[side]["total"] += _roi_mult_pos(o.ref_lon, o.ref_lat)
            for r in r_move:
                counts[side]["total_resim"] += _roi_mult_pos(r.ref_lon, r.ref_lat)

            if n_o == 0:
                # No Moving orig → all Moving resim become ghosts.
                # Non-Moving resim are irrelevant to this baseline.
                for r in r_move:
                    counts[side]["ghost"] += _roi_mult_pos(r.ref_lon, r.ref_lat)
                continue

            if n_r == 0:
                # No resim objects at all → orig unmatched (hurts TP, not FP).
                continue

            # ── Build cost matrix (Hungarian: minimise cost) ───────────────
            cost     = np.full((n_o, n_r), _NO_MATCH_COST)
            iou_mat  = np.zeros((n_o, n_r))

            for oi, o in enumerate(o_list):
                for ri, r in enumerate(r_list):
                    iou = _aabb_iou_obj(o, r)
                    iou_mat[oi, ri] = iou
                    if iou >= _IOU_THRESHOLD:
                        cost[oi, ri] = 1.0 - iou   # maximise IoU == minimise 1-IoU
                    elif _is_small_obj_frame(o) or _is_small_obj_frame(r):
                        d = _centroid_dist(o.ref_lon, o.ref_lat,
                                           r.ref_lon, r.ref_lat)
                        if d <= _DIST_FALLBACK_M:
                            cost[oi, ri] = d / _DIST_FALLBACK_M

            row_ind, col_ind = linear_sum_assignment(cost)

            assigned_r: set = set()
            for oi, ri in zip(row_ind, col_ind):
                if cost[oi, ri] >= _NO_MATCH_COST * 0.5:
                    continue   # no valid match available
                # Weight matched count by the orig object's ROI multiplier
                counts[side]["matched"] += _roi_mult_pos(
                    o_list[oi].ref_lon, o_list[oi].ref_lat)
                assigned_r.add(ri)
                iou_sum[side]           += iou_mat[oi, ri]
                counts[side]["iou_count"] += 1

                ov = _obj_kpi_values(o_list[oi])
                rv = _obj_kpi_values(r_list[ri])
                # Accuracy: only Moving↔Moving pairs.
                # orig is always Moving (filtered above); check resim side only.
                if r_list[ri].dyn_property == 3:
                    for sig, _ in _KPI_SIGNALS:
                        if sig == _HEADING_SIG:
                            sig_deltas[side][sig].append(
                                _heading_delta_deg(ov[sig], rv[sig]))
                        else:
                            sig_deltas[side][sig].append(ov[sig] - rv[sig])

            # Ghost = Moving resim object (indices 0..n_r_move-1) with no
            # valid match candidate.  Non-Moving resim (indices >= n_r_move)
            # are NEVER counted as ghosts regardless of match status.
            for ri in range(n_r_move):
                if ri not in assigned_r:
                    if np.all(cost[:, ri] >= _NO_MATCH_COST * 0.5):
                        counts[side]["ghost"] += _roi_mult_pos(
                            r_list[ri].ref_lon, r_list[ri].ref_lat)

    _cb(93, "Frame KPI: computing statistics…")

    def _side_kpi(side: str) -> SideFrameKpi:
        c       = counts[side]
        total   = c["total"]
        matched = c["matched"]
        t_resim = c["total_resim"]
        ghost   = c["ghost"]
        avail   = 100.0 * matched / max(total,   1)
        ghostp  = 100.0 * ghost   / max(t_resim, 1)
        m_iou   = iou_sum[side]   / max(c["iou_count"], 1)

        stats = {
            sig: _delta_stats(sig_deltas[side][sig], unit)
            for sig, unit in _KPI_SIGNALS
            if sig_deltas[side][sig]
        }
        det, gfree, acc, comp = _resim_scores(matched, total, ghost, t_resim, stats)

        return SideFrameKpi(
            side=side, total=total, matched=matched,
            total_resim=t_resim, ghost=ghost,
            availability_pct=avail, ghost_pct=ghostp, mean_iou=m_iou,
            tp_score=det, fp_score=gfree,
            accuracy_score=acc, resim_score=comp,
            signal_stats=stats,
        )

    _cb(100, "Frame KPI: done.")
    return FrameKpiResult(left=_side_kpi("L"), right=_side_kpi("R"))


# ---------------------------------------------------------------------------
# Track-to-Track KPI
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Track-to-Track KPI
# ---------------------------------------------------------------------------

def _track_match_score(ot: ObjectTrack, rt: ObjectTrack) -> Tuple[float, float]:
    """
    Cumulative match score and mean per-scan IoU across common scans.

    The match score uses IoU where available, with the small-object distance
    fallback for tiny objects.  Returns (cumulative_score, mean_iou).
    """
    o_by_scan = {rec.scan_idx: rec for rec in ot.records}
    r_by_scan = {rec.scan_idx: rec for rec in rt.records}
    common    = set(o_by_scan) & set(r_by_scan)
    if not common:
        return 0.0, 0.0
    total_score = 0.0
    total_iou   = 0.0
    for scan in common:
        score, iou = _scan_score_sig(o_by_scan[scan].raw_signals,
                                     r_by_scan[scan].raw_signals)
        total_score += score
        total_iou   += iou
    n = len(common)
    return total_score, total_iou / n


def _resim_track_frame_coverage(rt: ObjectTrack,
                                o_list: List[ObjectTrack]) -> float:
    """
    Fraction of resim track's scans where at least one orig track has a
    valid per-scan match (IoU >= threshold or small-object distance fallback).

    Used to distinguish a genuinely absent object (ghost) from a resim track
    that was simply outcompeted in the Hungarian assignment even though it
    overlaps real objects on >50 % of its frames.
    """
    o_by_scan: Dict[int, list] = {}
    for ot in o_list:
        for rec in ot.records:
            o_by_scan.setdefault(rec.scan_idx, []).append(rec)

    matched_scans = 0
    for rec in rt.records:
        for o_rec in o_by_scan.get(rec.scan_idx, []):
            score, _ = _scan_score_sig(o_rec.raw_signals, rec.raw_signals)
            if score > 0.0:
                matched_scans += 1
                break   # one valid orig match per scan is enough
    return matched_scans / max(len(rt.records), 1)


def _match_tracks_side(orig_tracks: List[ObjectTrack],
                       resim_tracks: List[ObjectTrack],
                       side: str) -> List[TrackMatch]:
    """
    Optimal (Hungarian) assignment for one radar side.

    Returns a list of TrackMatch objects covering all original and unmatched
    resim tracks.
    """
    all_o = [t for t in orig_tracks  if t.side == side]
    all_r = [t for t in resim_tracks if t.side == side]

    # Separate short events — they are listed in results but never scored
    o_short = [t for t in all_o if t.duration < _MIN_TRACK_DURATION_S]
    r_short = [t for t in all_r if t.duration < _MIN_TRACK_DURATION_S]
    o_list  = [t for t in all_o if t.duration >= _MIN_TRACK_DURATION_S]
    r_list  = [t for t in all_r if t.duration >= _MIN_TRACK_DURATION_S]

    # Build short-event TrackMatch entries (excluded from scoring)
    short_matches: List[TrackMatch] = [
        TrackMatch(orig_track=t,    resim_track=None, is_short=True, is_ghost=False)
        for t in o_short
    ] + [
        TrackMatch(orig_track=None, resim_track=t,    is_short=True, is_ghost=False)
        for t in r_short
    ]

    if not o_list:
        return short_matches + [
            TrackMatch(orig_track=None, resim_track=t, mean_iou=0.0)
            for t in r_list
        ]
    if not r_list:
        return short_matches + [
            TrackMatch(orig_track=t, resim_track=None, mean_iou=0.0)
            for t in o_list
        ]

    # ── Build score matrix ─────────────────────────────────────────────────
    n_o, n_r = len(o_list), len(r_list)
    score_mat   = np.zeros((n_o, n_r))
    mean_iou_mat = np.zeros((n_o, n_r))

    for oi, ot in enumerate(o_list):
        o_scans = {rec.scan_idx for rec in ot.records}
        for ri, rt in enumerate(r_list):
            r_scans = {rec.scan_idx for rec in rt.records}
            if not (o_scans & r_scans):
                continue
            score, m_iou = _track_match_score(ot, rt)
            score_mat[oi, ri]    = score
            mean_iou_mat[oi, ri] = m_iou

    # Hungarian minimises cost → negate score matrix
    row_ind, col_ind = linear_sum_assignment(-score_mat)

    used_r: set = set()
    assignment: Dict[int, Tuple[int, float]] = {}  # oi → (ri, mean_iou)
    for oi, ri in zip(row_ind, col_ind):
        if score_mat[oi, ri] > 0.0:
            assignment[oi] = (ri, mean_iou_mat[oi, ri])
            used_r.add(ri)

    # ── Build TrackMatch list ──────────────────────────────────────────────
    matches: List[TrackMatch] = []
    for oi, ot in enumerate(o_list):
        assign = assignment.get(oi)
        if assign is None:
            matches.append(TrackMatch(orig_track=ot, resim_track=None, mean_iou=0.0))
            continue

        ri, m_iou = assign
        rt = r_list[ri]
        o_by_scan = {rec.scan_idx: rec for rec in ot.records}
        r_by_scan = {rec.scan_idx: rec for rec in rt.records}
        common    = set(o_by_scan) & set(r_by_scan)
        coverage       = 100.0 * len(common) / max(len(o_by_scan), 1)
        resim_coverage = 100.0 * len(common) / max(len(r_by_scan), 1)

        raw_deltas: Dict[str, List[float]] = {s: [] for s, _ in _KPI_SIGNALS}
        for scan in sorted(common):
            ov = _rec_kpi_values(o_by_scan[scan].raw_signals)
            rv = _rec_kpi_values(r_by_scan[scan].raw_signals)
            # Accuracy is only computed for Moving observations (dyn_property == 3).
            # Stopped and Stationary scans contribute to TP/FP coverage but
            # not to signal deltas.
            if not (o_by_scan[scan].dyn_property == 3
                    and r_by_scan[scan].dyn_property == 3):
                continue
            for sig, _ in _KPI_SIGNALS:
                if ov[sig] is None or rv[sig] is None:
                    continue
                if sig == _HEADING_SIG:
                    raw_deltas[sig].append(
                        _heading_delta_deg(ov[sig], rv[sig]))
                else:
                    raw_deltas[sig].append(ov[sig] - rv[sig])

        sig_stats = {
            sig: _delta_stats(raw_deltas[sig], unit)
            for sig, unit in _KPI_SIGNALS
            if raw_deltas[sig]
        }

        matches.append(TrackMatch(
            orig_track=ot, resim_track=rt,
            mean_iou=m_iou,
            coverage_pct=coverage,
            resim_coverage_pct=resim_coverage,
            raw_deltas=raw_deltas,
            signal_stats=sig_stats,
        ))

    # Unmatched resim tracks → ghost or outcompeted-but-not-ghost
    for ri, rt in enumerate(r_list):
        if ri not in used_r:
            # True ghost: score_mat column is all zeros → never had any valid
            # per-scan overlap with any orig track on this side.
            if np.all(score_mat[:, ri] == 0.0):
                is_ghost = True
            else:
                # Had valid candidate(s) but was outcompeted by a better pair.
                # Check per-frame coverage: if >50 % of its scans can match
                # ANY orig track it is not a ghost, just unlucky in assignment.
                frame_cov = _resim_track_frame_coverage(rt, o_list)
                is_ghost  = frame_cov <= _GHOST_FRAME_COV_THRESHOLD
            matches.append(TrackMatch(
                orig_track=None, resim_track=rt,
                mean_iou=0.0, coverage_pct=0.0, resim_coverage_pct=0.0,
                is_ghost=is_ghost,
            ))

    return matches + short_matches


def compute_track_kpi(orig_tracks: List[ObjectTrack],
                      resim_tracks: List[ObjectTrack],
                      progress_cb: Optional[Callable] = None) -> TrackKpiResult:
    """
    Compute track-to-track comparison KPI (Moving, Stopped, and Stationary objects).

    Uses pre-computed ObjectTrack lists from ObjectTrackWidget.  The
    Hungarian algorithm finds the globally-optimal one-to-one assignment
    between original and resim tracks per radar side.
    """
    def _cb(pct: int, msg: str) -> None:
        if progress_cb:
            progress_cb(pct, msg)

    _cb(0,  "Track KPI: matching Left-side tracks…")
    left_matches  = _match_tracks_side(orig_tracks, resim_tracks, "L")
    _cb(50, "Track KPI: matching Right-side tracks…")
    right_matches = _match_tracks_side(orig_tracks, resim_tracks, "R")
    _cb(90, "Track KPI: aggregating statistics…")

    def _side_kpi(side: str, matches: List[TrackMatch]) -> SideTrackKpi:
        # Short events are excluded from all scoring — only listed in the table
        scored = [m for m in matches if not m.is_short]

        paired    = [m for m in scored
                     if m.orig_track is not None and m.resim_track is not None]
        n_orig    = sum(1 for m in scored if m.orig_track  is not None)
        n_resim   = sum(1 for m in scored if m.resim_track is not None)
        n_matched = len(paired)
        ghost     = sum(1 for m in scored
                        if m.resim_track is not None and m.orig_track is None
                        and m.is_ghost)
        ratio     = n_resim / max(n_orig, 1)

        # ── ROI-weighted frame counts for recall and precision scores ──
        # Each track contributes its scan count × its ROI multiplier.
        # The ROI multiplier is the best (highest) zone the track ever entered
        # ('closest range' rule for T2T).  Tracks outside all ROIs get 0.0
        # and are excluded from scoring entirely.
        orig_frames    = sum(
            len(m.orig_track.records)  * _roi_mult_track(m.orig_track)
            for m in scored if m.orig_track  is not None
        )
        resim_frames   = sum(
            len(m.resim_track.records) * _roi_mult_track(m.resim_track)
            for m in scored if m.resim_track is not None
        )
        # Count only the common (overlapping) scans per matched pair, weighted
        # by the orig track's ROI multiplier.
        matched_frames = sum(
            (m.coverage_pct / 100.0 * len(m.orig_track.records))
            * _roi_mult_track(m.orig_track)
            for m in paired
        )
        # Only true ghosts (is_ghost=True) count against the precision score.
        # Outcompeted-but-overlapping tracks are excluded.
        ghost_frames   = sum(
            len(m.resim_track.records) * _roi_mult_track(m.resim_track)
            for m in scored
            if m.orig_track is None and m.resim_track is not None
            and m.is_ghost
        )

        # Duration-weighted average coverage and mean IoU
        pair_weights  = [len(m.orig_track.records) for m in paired]
        total_pw      = max(sum(pair_weights), 1)
        avg_cov       = sum(m.coverage_pct       * w
                            for m, w in zip(paired, pair_weights)) / total_pw
        avg_resim_cov = sum(m.resim_coverage_pct * w
                            for m, w in zip(paired, pair_weights)) / total_pw
        m_iou         = sum(m.mean_iou           * w
                            for m, w in zip(paired, pair_weights)) / total_pw

        # Aggregate all raw deltas (naturally duration-weighted: longer tracks
        # contribute more samples)
        all_deltas: Dict[str, List[float]] = {s: [] for s, _ in _KPI_SIGNALS}
        for m in paired:
            for sig, dl in m.raw_deltas.items():
                all_deltas[sig].extend(dl)

        agg_stats = {
            sig: _delta_stats(all_deltas[sig], unit)
            for sig, unit in _KPI_SIGNALS
            if all_deltas[sig]
        }

        # Use frame-weighted counts: detection = matched_frames/orig_frames,
        # ghost = ghost_frames/resim_frames
        det, gfree, acc, comp = _resim_scores(matched_frames, orig_frames,
                                              ghost_frames,   resim_frames,
                                              agg_stats)

        return SideTrackKpi(
            side=side,
            n_orig=n_orig,
            n_matched=n_matched,
            n_resim=n_resim,
            n_resim_matched=n_matched,
            avg_coverage_pct=avg_cov,
            avg_resim_coverage_pct=avg_resim_cov,
            track_count_ratio=ratio,
            mean_iou=m_iou,
            tp_score=det, fp_score=gfree,
            accuracy_score=acc,
            resim_score=comp,
            signal_stats=agg_stats,
            matches=matches,
        )

    _cb(100, "Track KPI: done.")
    return TrackKpiResult(
        left=_side_kpi("L", left_matches),
        right=_side_kpi("R", right_matches),
    )
