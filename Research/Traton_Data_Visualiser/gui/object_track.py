"""
object_track.py
===============
ObjectTrack container and build_object_tracks() function.

Definitions
-----------
An *object track* is the complete lifetime of one real-world moving object
as seen by one radar side, identified by a stable TrackingID.

Rules
-----
- DynamicProperty 1 (Stationary), 2 (Stopped), and 3 (Moving) frames are
  considered during track building.
- Stationary frames keep an existing track alive and allow Stopped↔Stationary
  cross-matching at Track-to-Track KPI level.
- Tracks whose every frame is Stationary (is_stationary_only == True) are
  discarded and never returned — they would inflate denominators without
  representing meaningful detection events.
- Gap detection: scan_idx gap > 4 (≈ 200 ms at 20 Hz) → new track segment.
- Every track is assigned a UUID on creation.
"""

from __future__ import annotations

import re
import uuid
from typing import Dict, List, Optional, Tuple

import numpy as np


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

_SLOT_RE = re.compile(r'_[LR]_\d{2}$')


def strip_slot_suffix(name: str) -> str:
    """Remove trailing _L_01 / _R_07 style slot identifiers for display."""
    return _SLOT_RE.sub('', name)


# ---------------------------------------------------------------------------
# Data containers
# ---------------------------------------------------------------------------

class TrackRecord:
    """One frame-observation of a tracked object."""

    __slots__ = ("scan_idx", "timestamp", "raw_signals", "dyn_property")

    def __init__(self, scan_idx: int, timestamp: float, raw_signals: dict,
                 dyn_property: int = 0):
        self.scan_idx     = scan_idx
        self.timestamp    = timestamp
        self.raw_signals  = raw_signals
        self.dyn_property = dyn_property


class SignalStats:
    """Pre-computed statistics for one signal across an object's lifetime."""

    __slots__ = ("min", "max", "avg", "median", "initial", "last")

    def __init__(self, min_: float, max_: float, avg: float, median: float,
                 initial: float = float("nan"), last: float = float("nan")):
        self.min     = min_
        self.max     = max_
        self.avg     = avg
        self.median  = median
        self.initial = initial   # first observed raw value
        self.last    = last      # last observed raw value


class ObjectTrack:
    """Full lifetime of one real-world moving object on one radar side."""

    def __init__(self, side: str, tracking_id: int,
                 data_source: str = "Original"):
        self.uuid        = str(uuid.uuid4())
        self.side        = side           # "L" or "R"
        self.tracking_id = tracking_id
        self.data_source = data_source   # "Original" or "Resim"

        self._records: List[TrackRecord] = []

        # Populated by finalize()
        self.start_scan: int   = -1
        self.end_scan:   int   = -1
        self.start_ts:  float  = 0.0
        self.end_ts:    float  = 0.0
        self.duration:  float  = 0.0
        self.stats: Dict[str, SignalStats] = {}   # stripped_signal_name → stats
        # ID drop gaps: list of (scan_before, scan_after) tuples
        self.id_drops: List[Tuple[int, int]] = []
        # True when every record in this track has DynamicProperty == 1.
        # Such tracks are used for KPI matching but hidden from all UI lists.
        self.is_stationary_only: bool = False

    # ------------------------------------------------------------------

    def add_record(self, scan_idx: int, timestamp: float,
                   raw_signals: dict, dyn_property: int = 0) -> None:
        self._records.append(TrackRecord(scan_idx, timestamp, raw_signals,
                                         dyn_property))

    @property
    def records(self) -> List[TrackRecord]:
        return self._records

    def get_record_at(self, scan_idx: int) -> Optional[TrackRecord]:
        """Return the TrackRecord for scan_idx, or None if absent (gap frame)."""
        for rec in self._records:
            if rec.scan_idx == scan_idx:
                return rec
        return None

    def finalize(self) -> None:
        """Sort records, detect ID drops, compute per-signal statistics."""
        if not self._records:
            return
        self._records.sort(key=lambda r: r.scan_idx)
        self.start_scan = self._records[0].scan_idx
        self.end_scan   = self._records[-1].scan_idx
        self.start_ts   = self._records[0].timestamp
        self.end_ts     = self._records[-1].timestamp
        # Duration is computed from scan indices (one scan = 50 ms).
        # This is robust against non-monotonic or broken MF4 timestamps.
        self.duration   = (self.end_scan - self.start_scan) * _SCAN_PERIOD_S
        # True when every observed frame was Stationary (DynamicProperty == 1).
        self.is_stationary_only = all(r.dyn_property == 1 for r in self._records)

        # Detect within-track gaps: consecutive records with a scan_idx gap > 1
        # (i.e. one or more scans were missing for this object, e.g. ID drop).
        self.id_drops = []
        for i in range(1, len(self._records)):
            dscan = self._records[i].scan_idx - self._records[i - 1].scan_idx
            if dscan > 1:
                self.id_drops.append(
                    (self._records[i - 1].scan_idx, self._records[i].scan_idx)
                )

        # Collect numeric values per STRIPPED signal name across all records.
        # Using the stripped name means the same physical signal that appears
        # in different object slots across frames is merged into one entry.
        sig_vals:  Dict[str, list]  = {}
        sig_first: Dict[str, float] = {}   # first observed value per signal
        sig_last:  Dict[str, float] = {}   # last  observed value per signal
        for rec in self._records:
            for k, v in rec.raw_signals.items():
                if k.startswith("__"):
                    continue
                display_key = strip_slot_suffix(k)
                try:
                    fval = float(v)
                except (TypeError, ValueError):
                    continue
                sig_vals.setdefault(display_key, []).append(fval)
                if display_key not in sig_first:
                    sig_first[display_key] = fval
                sig_last[display_key] = fval

        for sig, vals in sig_vals.items():
            arr = np.array(vals, dtype=float)
            valid = arr[~np.isnan(arr)]
            if len(valid) == 0:
                continue
            self.stats[sig] = SignalStats(
                min_    = float(np.min(valid)),
                max_    = float(np.max(valid)),
                avg     = float(np.mean(valid)),
                median  = float(np.median(valid)),
                initial = sig_first.get(sig, float("nan")),
                last    = sig_last.get(sig, float("nan")),
            )


# ---------------------------------------------------------------------------
# Track builder
# ---------------------------------------------------------------------------

_SCAN_PERIOD_S      = 0.050  # nominal scan cycle period (50 ms @ 20 Hz)
_GAP_SCAN_TOLERANCE = 4     # max scan-index gap to continue the same track
                             # (4 × 50 ms = 200 ms)
_POS_JUMP_M         = 5.0   # max lon/lat displacement [m] within one scan step;
                             # a larger jump means a different physical object


def build_object_tracks(model, progress_cb=None,
                        data_source: str = "Original") -> List[ObjectTrack]:
    """
    Build all ObjectTrack instances from a loaded DataModel.

    Parameters
    ----------
    model : DataModel
        Fully loaded model (load() already called).
    progress_cb : callable(pct: int, msg: str) | None
        Optional progress callback (0-100).
    data_source : str
        Label stored on every produced ObjectTrack ("Original" or "Resim").

    Returns
    -------
    List[ObjectTrack]
        All finished tracks, sorted by start_scan ascending.
    """

    def _cb(pct: int, msg: str) -> None:
        if progress_cb:
            progress_cb(pct, msg)

    _cb(0, "Collecting Moving objects…")

    # active[(side, tid)] = (last_scan_idx, last_lon, last_lat, ObjectTrack)
    active: Dict[Tuple[str, int], Tuple[int, float, float, ObjectTrack]] = {}
    finished: List[ObjectTrack] = []

    total = max(len(model.scan_indices), 1)
    for i, scan_idx in enumerate(model.scan_indices):
        if i % 100 == 0:
            _cb(int(95 * i / total), f"Processing scan {scan_idx} ({i + 1}/{total})…")

        for obj in model.get_objects(scan_idx):
            # Include Moving (3), Stopped (2), and Stationary (1) objects.
            # Stationary is included so that an object classified as Stopped
            # in the original can be matched against a Stationary counterpart
            # in resim (and vice versa).  Invalid (0) is still excluded.
            if obj.dyn_property not in (1, 2, 3):
                continue
            key = (obj.side, obj.tracking_id)

            if key in active:
                last_scan, last_lon, last_lat, track = active[key]
                dscan   = scan_idx - last_scan
                dlon    = abs(obj.lon - last_lon)
                dlat    = abs(obj.lat - last_lat)
                pos_ok  = dlon <= _POS_JUMP_M and dlat <= _POS_JUMP_M
                if dscan == 0:
                    # Duplicate scan index in MF4 data (sensor counter did not
                    # increment between two CAN frames).  Keep the track alive
                    # but do NOT add a second record for the same scan index.
                    active[key] = (scan_idx, obj.lon, obj.lat, track)
                elif 0 < dscan <= _GAP_SCAN_TOLERANCE and pos_ok:
                    # Normal forward step — continue track
                    track.add_record(scan_idx, obj.timestamp, obj.raw_signals,
                                     obj.dyn_property)
                    active[key] = (scan_idx, obj.lon, obj.lat, track)
                else:
                    # Either scan gap too large / non-monotonic, OR position
                    # jumped > 5 m → this is a different physical object with
                    # a reused tracking ID.  Close the old track, start fresh.
                    track.finalize()
                    if not track.is_stationary_only:
                        finished.append(track)
                    track = ObjectTrack(obj.side, obj.tracking_id,
                                        data_source=data_source)
                    track.add_record(scan_idx, obj.timestamp, obj.raw_signals,
                                     obj.dyn_property)
                    active[key] = (scan_idx, obj.lon, obj.lat, track)
            else:
                track = ObjectTrack(obj.side, obj.tracking_id,
                                    data_source=data_source)
                track.add_record(scan_idx, obj.timestamp, obj.raw_signals,
                                 obj.dyn_property)
                active[key] = (scan_idx, obj.lon, obj.lat, track)

    # Finalise all still-active tracks.
    # Stationary-only tracks are discarded — they are not built at all,
    # keeping memory usage low and the UI free of clutter.
    for _, (_, _lon, _lat, track) in active.items():
        track.finalize()
        if not track.is_stationary_only:
            finished.append(track)

    # Sort by start scan index (scan indices are reliable; timestamps are not)
    finished.sort(key=lambda t: t.start_scan)

    _cb(100, f"Done — {len(finished)} object track(s) found.")
    return finished
