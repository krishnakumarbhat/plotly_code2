"""
data_model.py
=============
Loads an MF4 file, decodes SRRL/SRRR CAN messages, and provides a clean
interface for the GUI:

  - Ordered list of scan indices (from Header messages)
  - Per-scan-index list of ObjectRecord (both sides)
  - Full decoded DataFrames accessible for the signal tree / plot
"""

from __future__ import annotations

import re
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional, Tuple

import numpy as np
import pandas as pd

# Allow running from the gui/ directory or project root.
_SRC = Path(__file__).resolve().parent.parent / "src"
if str(_SRC) not in sys.path:
    sys.path.insert(0, str(_SRC))

from vcan_reader import read_vcan_from_mf4, detect_mf4_type, load_dbc

# Regex to strip _L_01 / _R_07 style slot suffixes from DBC signal names.
_SLOT_SUFFIX_RE = re.compile(r'_[LR]_\d{2}$')

# ---------------------------------------------------------------------------
# ReferencePoint enum → (dLon, dLat) offsets as fractions of (Length, Width)
# The offset converts the reference corner/edge to the box CENTRE.
# VCS (signal convention): Lon = forward (+), Lat = LEFT (+)  [right is negative]
# Box corners in object-local frame (before heading rotation) relative to centre:
#   Front-left  = (+L/2, +W/2)   Front-right = (+L/2, -W/2)
#   Rear-left   = (-L/2, +W/2)   Rear-right  = (-L/2, -W/2)
# To go from ref-point back to centre we negate these offsets.
# Values 8 (Unknown), 14/0xE (Error), 15/0xF (Not Available) → treat as centre.
# ---------------------------------------------------------------------------
_REF_OFFSET: Dict[int, Tuple[float, float]] = {
    0:  (-0.5, -0.5),   # Front-left
    1:  (-0.5,  0.0),   # Front
    2:  (-0.5,  0.5),   # Front-right
    3:  ( 0.0,  0.5),   # Right
    4:  ( 0.5,  0.5),   # Rear-right
    5:  ( 0.5,  0.0),   # Rear
    6:  ( 0.5, -0.5),   # Rear-left
    7:  ( 0.0, -0.5),   # Left
    8:  ( 0.0,  0.0),   # Unknown  → use raw pos as centre
}


@dataclass
class ObjectRecord:
    """One detected object for a single radar scan cycle."""
    side: str            # "L" or "R"
    obj_id: int          # 1-based object number (1-20 per side)
    tracking_id: int
    lon: float           # longitudinal position of CENTRE [m]
    lat: float           # lateral position of CENTRE [m]
    length: float        # [m]
    width: float         # [m]
    heading: float       # [rad] in VCS (0 = forward)
    existence_prob: float
    dyn_property: int
    timestamp: float     # MF4 reception timestamp [s]
    # Reference point position in VCS (from signal, before centre offset)
    ref_lon: float = 0.0
    ref_lat: float = 0.0
    ref_pt: int = 8      # raw ReferencePoint enum value
    # Ground velocity components [m/s] in VCS
    lon_vel: float = 0.0
    lat_vel: float = 0.0
    # ClassMostProb: 0=Unknown 1=Car 2=Motorbike 3=Truck 4=Bicycle 5=Pedestrian 6=Error 7=N/A
    class_id: int = 0
    raw_signals: dict = field(default_factory=dict)  # all decoded signals for this slot


# Signals kept from each object slot for quick access.
_OBJ_SIGNALS = [
    "LonPosition", "LatPosition", "Width", "Length",
    "HeadingAngle", "ReferencePoint", "TrackingId",
    "ExistenceProbability", "DynamicProperty",
]


def _side_of(msg_name: str) -> str:
    return "L" if msg_name.startswith("SRRL_") else "R"


def _suffix_of(side: str) -> str:
    return "_L_" if side == "L" else "_R_"


class DataModel:
    """
    Central data store for one loaded MF4 file.

    Attributes
    ----------
    scan_indices : list[int]
        Sorted list of Scan_Index values from the Header messages.
    frames : dict[str, pd.DataFrame]
        Raw decoded DataFrames keyed by message name (for signal tree/plot).
    mf4_path : Path
    """

    def __init__(self) -> None:
        self.scan_indices: List[int] = []
        self.frames: Dict[str, pd.DataFrame] = {}
        self.mf4_path: Optional[Path] = None
        self.mf4_format: str = "unknown"  # "man_autera" | "aptiv_orcas" | "scania_orcas" | "resim_canoe" | "unknown"
        self._objects_by_scan: Dict[int, List[ObjectRecord]] = {}
        self._header_ts_to_scan: Optional[pd.Series] = None  # index=recv_ts -> scan_index (for object matching)
        self._scan_to_obj_ts:   Dict[int, float] = {}   # scan_index -> sensor obj_ts (for display)
        self._scan_to_recv_ts:  Dict[int, float] = {}   # scan_index -> CAN recv_ts (for CAN-indexed DFs)
        # Reverse mapping: canonical SRRL scan_index → original SRRR scan_index.
        # Populated only when do_remap=True (non-overlapping SRRL/SRRR ranges).
        self._canonical_to_srrr: Dict[int, int] = {}
        # stripped_signal_name → unit string (from DBC); only non-enum signals with a unit.
        self.signal_units: Dict[str, str] = {}

    # ------------------------------------------------------------------
    # Loading
    # ------------------------------------------------------------------

    def load(self, mf4_path: str | Path, dbc_path: str | Path,
             progress_cb=None) -> None:
        """Decode the MF4 file and build internal indexes.

        Parameters
        ----------
        progress_cb : callable(pct: int, message: str) | None
            Optional callback invoked with (0-100, status string) at each
            loading stage so the UI can update a progress bar.
        """
        def _progress(pct: int, msg: str) -> None:
            if progress_cb is not None:
                progress_cb(pct, msg)

        _progress(2, "Detecting file format…")
        self.mf4_path   = Path(mf4_path)
        self.mf4_format = detect_mf4_type(mf4_path)

        _progress(4, "Loading DBC…")
        _prefixes = ("SRRL_", "SRRR_", "SRR2_")
        try:
            _db = load_dbc(dbc_path, prefixes=_prefixes)
        except Exception:
            _db = None

        _progress(5, "Reading and decoding CAN frames…")
        self.frames = read_vcan_from_mf4(
            mf4_path, dbc_path,
            prefixes=_prefixes,
            fmt=self.mf4_format,   # reuse already-known format; skips 2nd MDF open
            progress_cb=progress_cb,
            base_pct=5,
            span_pct=80,
        )

        _progress(86, "Building signal unit map…")
        self.signal_units = self._build_signal_units(dbc_path, db=_db)

        _progress(88, "Building scan index…")
        self._build_scan_index()

        _progress(90, "Building object index…")
        self._build_object_index(progress_cb=progress_cb, base_pct=90, span_pct=9)

        _progress(100, "Done.")

    # ------------------------------------------------------------------
    # Signal unit map
    # ------------------------------------------------------------------

    def _build_signal_units(self, dbc_path: str | Path,
                             db=None) -> Dict[str, str]:
        """Return {stripped_signal_name: unit} for all non-enum DBC signals.

        Pass an already-loaded ``db`` to skip re-parsing the DBC file.
        """
        try:
            if db is None:
                db = load_dbc(dbc_path, prefixes=("SRRL_", "SRRR_", "SRR2_"))
        except Exception:
            return {}

        units: Dict[str, str] = {}
        for msg in db.messages:
            for sig in msg.signals:
                unit = (sig.unit or "").strip()
                # "-" is the DBC placeholder meaning "no physical unit".
                # Enum/counter signals always carry "-"; physics signals carry
                # their real unit (m, m/s, rad, s, %, …).
                if not unit or unit == "-":
                    continue
                stripped = _SLOT_SUFFIX_RE.sub("", sig.name)
                if stripped not in units:
                    units[stripped] = unit
        return units

    # ------------------------------------------------------------------
    # Scan index building
    # ------------------------------------------------------------------

    def _build_scan_index(self) -> None:
        """
        Collect all scan indices from both SRRL and SRRR Header messages.
        Each entry in the combined header is (timestamp, scan_index).

        Normal case — shared counter
        ----------------------------
        Both sensors increment the same scan counter, so SRRL and SRRR always
        emit the same scan_index for the same cycle.  Combining rows and
        deduplicating by scan_index yields one entry per cycle.

        Offset case — independent counters (scan-index gap bug)
        -------------------------------------------------------
        Some Aptiv recordings have SRRL and SRRR counters that run
        independently (e.g. after a sensor reset).  The two value ranges are
        completely disjoint (e.g. SRRR 15869–18268, SRRL 30992–33391).
        Without special handling, ``drop_duplicates`` finds no duplicates →
        ~2× frames appear, each containing only one sensor's objects.
        Consequences:
          - PlanView shows 2× the expected number of frames (flickering).
          - Object-track scan-index gaps go non-monotonic, fragmenting
            continuous detections into dozens of spurious short tracks.

        Fix: detect disjoint ranges, then use SRRL scan indices as the
        canonical frame key.  Every SRRR cycle is remapped to the
        temporally nearest SRRL cycle.  ``_header_ts_to_scan`` is built to
        map CAN recv_ts from *both* sensors to the canonical SRRL scan_index,
        so ``_build_object_index`` always stores objects under a unified key.
        """
        # ---- collect per-side header rows --------------------------------
        side_rows: Dict[str, List[Tuple[float, int, float]]] = {
            "SRRL": [], "SRRR": [],
        }

        for side in ("SRRL", "SRRR"):
            key = f"{side}_Header_SRR2"
            if key not in self.frames:
                continue
            df = self.frames[key]
            ts_col = f"ObjectTimestampSeconds_{side[3]}"    # e.g. ObjectTimestampSeconds_L
            ns_col = f"ObjectTimestampNanoseconds_{side[3]}"
            si_col = f"Scan_Index_{side[3]}"

            if ts_col not in df.columns or si_col not in df.columns:
                continue

            rows = side_rows[side]
            for recv_ts, row in df.iterrows():
                obj_ts = float(row[ts_col]) + float(row.get(ns_col, 0)) * 1e-9
                rows.append((obj_ts, int(row[si_col]), float(recv_ts)))

        srrl_rows = side_rows["SRRL"]
        srrr_rows = side_rows["SRRR"]
        header_rows: List[Tuple[float, int, float]] = srrl_rows + srrr_rows

        if not header_rows:
            return

        # ---- detect scan-index offset between sides ----------------------
        # If both sensors are present and their scan-index value ranges are
        # completely disjoint, the counters are independent.  Remap SRRR
        # scan indices onto the nearest SRRL scan index (by sensor time).
        do_remap = False
        if srrl_rows and srrr_rows:
            srrl_min = min(r[1] for r in srrl_rows)
            srrl_max = max(r[1] for r in srrl_rows)
            srrr_min = min(r[1] for r in srrr_rows)
            srrr_max = max(r[1] for r in srrr_rows)
            do_remap = (srrl_max < srrr_min) or (srrr_max < srrl_min)

        if do_remap:
            # ---- offset path: time-based pairing -----------------------
            # Build per-side DataFrames, deduplicated by scan_index.
            srrl_df = (
                pd.DataFrame(srrl_rows, columns=["obj_ts", "scan_index", "recv_ts"])
                .sort_values("obj_ts")
                .drop_duplicates("scan_index", keep="first")
            )
            srrr_df = (
                pd.DataFrame(srrr_rows, columns=["obj_ts", "scan_index", "recv_ts"])
                .sort_values("obj_ts")
                .drop_duplicates("scan_index", keep="first")
            )

            # For each SRRR cycle, find the temporally nearest SRRL cycle.
            srrl_times = srrl_df["obj_ts"].to_numpy()
            srrl_scans = srrl_df["scan_index"].to_numpy()
            srrr_times = srrr_df["obj_ts"].to_numpy()

            ri    = np.searchsorted(srrl_times, srrr_times)
            ri_lo = (ri - 1).clip(0, len(srrl_times) - 1)
            ri_hi = ri.clip(0, len(srrl_times) - 1)
            best  = np.where(
                np.abs(srrl_times[ri_lo] - srrr_times)
                <= np.abs(srrl_times[ri_hi] - srrr_times),
                ri_lo, ri_hi,
            )
            # SRRR scan_index → canonical SRRL scan_index
            srrr_to_srrl: Dict[int, int] = dict(
                zip(srrr_df["scan_index"].tolist(), srrl_scans[best].tolist())
            )

            # Reverse mapping for display only: pair by position (frame N of SRRL
            # ↔ frame N of SRRR, sorted by sensor time).
            # Inverting srrr_to_srrl is avoided because it is many-to-one — two
            # adjacent SRRR scans can round to the same SRRL canonical, leaving
            # some canonicals without a SRRR entry and forcing a fallback to the
            # SRRL index.  Position pairing is collision-free and display-accurate.
            srrl_scans_sorted = srrl_df.sort_values("obj_ts")["scan_index"].tolist()
            srrr_scans_sorted = srrr_df.sort_values("obj_ts")["scan_index"].tolist()
            self._canonical_to_srrr = dict(zip(srrl_scans_sorted, srrr_scans_sorted))

            # Canonical frame list and timestamp lookups use SRRL only.
            hdr_df = srrl_df
            self._scan_to_obj_ts  = dict(zip(hdr_df["scan_index"], hdr_df["obj_ts"]))
            self._scan_to_recv_ts = dict(zip(hdr_df["scan_index"], hdr_df["recv_ts"]))
            self.scan_indices = hdr_df.sort_values("obj_ts")["scan_index"].tolist()

            # Build recv_ts → canonical scan_index lookup that covers CAN frames
            # from both sensors.  Using the raw (non-deduped) rows gives the
            # most accurate nearest-neighbour match in _build_object_index.
            remap_rows: List[Tuple[float, int]] = []
            for _ots, si, rts in srrl_rows:
                remap_rows.append((rts, si))
            for _ots, si, rts in srrr_rows:
                remap_rows.append((rts, srrr_to_srrl.get(si, si)))

            remap_df = (
                pd.DataFrame(remap_rows, columns=["recv_ts", "scan_index"])
                .sort_values("recv_ts")
            )
            self._header_ts_to_scan = pd.Series(
                remap_df["scan_index"].values,
                index=remap_df["recv_ts"].values,
                name="scan_index",
            )

        else:
            # ---- normal path: combine and deduplicate -------------------
            # Combine and deduplicate by scan_index (keep earliest obj_ts per index).
            hdr_df = pd.DataFrame(header_rows, columns=["obj_ts", "scan_index", "recv_ts"])
            hdr_df = hdr_df.sort_values("obj_ts").drop_duplicates("scan_index", keep="first")

            # Authoritative sensor-time lookup: scan_index → obj_ts.
            # Used for display timestamps and ObjectRecord.timestamp — never recv_ts.
            self._scan_to_obj_ts  = dict(zip(hdr_df["scan_index"], hdr_df["obj_ts"]))
            self._scan_to_recv_ts = dict(zip(hdr_df["scan_index"], hdr_df["recv_ts"]))

            # Frame order: sort by sensor obj_ts so the navigation slider is
            # always chronological in sensor time even when CAN recv_ts is anomalous.
            self.scan_indices = hdr_df.sort_values("obj_ts")["scan_index"].tolist()

            # Build a CAN-time lookup: recv_ts → scan_index (needed to match object
            # messages to their scan cycle in _build_object_index).
            hdr_df_recv = hdr_df.sort_values("recv_ts")
            self._header_ts_to_scan = pd.Series(
                hdr_df_recv["scan_index"].values,
                index=hdr_df_recv["recv_ts"].values,
                name="scan_index",
            )

    # ------------------------------------------------------------------
    # Object index building
    # ------------------------------------------------------------------

    def _build_object_index(self, progress_cb=None,
                             base_pct: int = 65, span_pct: int = 34) -> None:
        """
        For every Object message frame, resolve its scan_index by nearest
        Header timestamp, then store ObjectRecord in _objects_by_scan.
        """
        if self._header_ts_to_scan is None or len(self._header_ts_to_scan) == 0:
            return

        header_timestamps = self._header_ts_to_scan.index.to_numpy(dtype=float)
        header_scan_vals  = self._header_ts_to_scan.values

        # Max allowed time gap: 1 full scan period (≈ 50 ms @ 20 Hz)
        _MAX_GAP = 0.1  # seconds

        obj_msg_re = re.compile(r"^(SRRL|SRRR)_Objects_(\d+)_SRR2$")

        obj_msgs = [(n, df) for n, df in self.frames.items() if obj_msg_re.match(n)]
        total_msgs = max(len(obj_msgs), 1)

        for msg_i, (msg_name, df) in enumerate(obj_msgs):
            if progress_cb is not None:
                pct = base_pct + int(span_pct * msg_i / total_msgs)
                progress_cb(pct, f"Indexing objects: {msg_name}…")

            m = obj_msg_re.match(msg_name)
            side    = "L" if m.group(1) == "SRRL" else "R"
            msg_num = int(m.group(2))
            sfx     = _suffix_of(side)

            slot_a = (msg_num - 1) * 2 + 1
            slot_b = slot_a + 1

            # --- Vectorised scan-index resolution for the whole message DF ---
            recv_ts_arr = df.index.to_numpy(dtype=float)  # shape (n,)

            # searchsorted gives the insertion point; check both neighbours.
            ri        = np.searchsorted(header_timestamps, recv_ts_arr)
            ri_lo     = (ri - 1).clip(0, len(header_timestamps) - 1)
            ri_hi     = ri.clip(0, len(header_timestamps) - 1)
            diff_lo   = np.abs(header_timestamps[ri_lo] - recv_ts_arr)
            diff_hi   = np.abs(header_timestamps[ri_hi] - recv_ts_arr)
            best      = np.where(diff_lo < diff_hi, ri_lo, ri_hi)
            best_gap  = np.minimum(diff_lo, diff_hi)
            valid_mask = best_gap <= _MAX_GAP          # rows within gap tolerance
            scan_idx_arr = header_scan_vals[best]      # scan index per row

            for slot in (slot_a, slot_b):
                lon_col  = f"LonPosition{sfx}{slot:02d}"
                lat_col  = f"LatPosition{sfx}{slot:02d}"
                w_col    = f"Width{sfx}{slot:02d}"
                l_col    = f"Length{sfx}{slot:02d}"
                hd_col   = f"HeadingAngle{sfx}{slot:02d}"
                rp_col   = f"ReferencePoint{sfx}{slot:02d}"
                tid_col  = f"TrackingId{sfx}{slot:02d}"
                ep_col   = f"ExistenceProbability{sfx}{slot:02d}"
                dp_col   = f"DynamicProperty{sfx}{slot:02d}"
                lv_col   = f"LonGndVel{sfx}{slot:02d}"
                latv_col = f"LatGndVel{sfx}{slot:02d}"
                cls_col  = f"ClassMostProb{sfx}{slot:02d}"

                if lon_col not in df.columns:
                    continue

                # Extract columns as numpy arrays — vastly faster than row access.
                lon_raw_arr = df[lon_col].to_numpy(dtype=float)
                lat_raw_arr = df[lat_col].to_numpy(dtype=float)    if lat_col  in df.columns else np.zeros(len(df))
                length_arr  = df[l_col].to_numpy(dtype=float)      if l_col    in df.columns else np.zeros(len(df))
                width_arr   = df[w_col].to_numpy(dtype=float)      if w_col    in df.columns else np.zeros(len(df))
                heading_arr = df[hd_col].to_numpy(dtype=float)     if hd_col   in df.columns else np.zeros(len(df))
                ref_pt_arr  = df[rp_col].to_numpy(dtype=int)       if rp_col   in df.columns else np.full(len(df), 8)
                tid_arr     = df[tid_col].to_numpy(dtype=int)      if tid_col  in df.columns else np.zeros(len(df), dtype=int)
                ep_arr      = df[ep_col].to_numpy(dtype=float)     if ep_col   in df.columns else np.zeros(len(df))
                dp_arr      = df[dp_col].to_numpy(dtype=int)       if dp_col   in df.columns else np.zeros(len(df), dtype=int)
                lv_arr      = df[lv_col].to_numpy(dtype=float)     if lv_col   in df.columns else np.zeros(len(df))
                latv_arr    = df[latv_col].to_numpy(dtype=float)   if latv_col in df.columns else np.zeros(len(df))
                cls_arr     = df[cls_col].to_numpy(dtype=int)      if cls_col  in df.columns else np.zeros(len(df), dtype=int)

                # Per-row filter: valid gap + non-NaN positions + meaningful TID.
                nan_mask    = ~(np.isnan(lon_raw_arr) | np.isnan(lat_raw_arr))
                tid_mask    = ~np.isin(tid_arr, [0, 255])
                row_mask    = valid_mask & nan_mask & tid_mask

                # Collect raw signal columns for this slot (for detail window).
                slot_tag   = f"_{slot:02d}"
                slot_cols  = [c for c in df.columns if c.endswith(slot_tag)]
                # Pre-extract as numpy arrays — avoids per-row iloc overhead.
                slot_arrays = {c: df[c].to_numpy() for c in slot_cols}
                side_str   = "Left" if side == "L" else "Right"

                for i in np.where(row_mask)[0]:
                    rp       = int(ref_pt_arr[i])
                    heading  = float(heading_arr[i])
                    lon_raw  = float(lon_raw_arr[i])
                    lat_raw  = float(lat_raw_arr[i])
                    length   = float(length_arr[i])
                    width    = float(width_arr[i])

                    dlon_frac, dlat_frac = _REF_OFFSET.get(rp, (0.0, 0.0))
                    c_h, s_h = np.cos(heading), np.sin(heading)
                    dlon_vcs = c_h * dlon_frac * length  - s_h * dlat_frac * width
                    dlat_vcs = s_h * dlon_frac * length  + c_h * dlat_frac * width

                    scan_idx = int(scan_idx_arr[i])
                    recv_ts  = float(recv_ts_arr[i])
                    obj_ts   = self._scan_to_obj_ts.get(scan_idx, recv_ts)

                    raw = {}
                    for c, arr in slot_arrays.items():
                        v = arr[i]
                        raw[c] = v.item() if hasattr(v, 'item') else v
                    raw["__timestamp__"] = obj_ts
                    raw["__side__"]      = side_str

                    self._objects_by_scan.setdefault(scan_idx, []).append(
                        ObjectRecord(
                            side=side,
                            obj_id=slot,
                            tracking_id=int(tid_arr[i]),
                            lon=lon_raw + dlon_vcs,
                            lat=lat_raw + dlat_vcs,
                            length=length,
                            width=width,
                            heading=heading,
                            existence_prob=float(ep_arr[i]),
                            dyn_property=int(dp_arr[i]),
                            timestamp=obj_ts,
                            ref_lon=lon_raw,
                            ref_lat=lat_raw,
                            ref_pt=rp,
                            lon_vel=float(lv_arr[i]),
                            lat_vel=float(latv_arr[i]),
                            class_id=int(cls_arr[i]),
                            raw_signals=raw,
                        )
                    )

    # ------------------------------------------------------------------
    # Public query interface
    # ------------------------------------------------------------------

    def get_objects(self, scan_idx: int) -> List[ObjectRecord]:
        """Return all ObjectRecords for the given scan index."""
        return self._objects_by_scan.get(scan_idx, [])

    def get_header_timestamp(self, scan_idx: int) -> Optional[float]:
        """Return the sensor object timestamp (obj_ts) for this scan index.

        Uses the ObjectTimestampSeconds/Nanoseconds field from the Header
        message — never the CAN reception timestamp, which can be anomalous.
        """
        return self._scan_to_obj_ts.get(scan_idx)

    def get_recv_timestamp(self, scan_idx: int) -> Optional[float]:
        """Return the CAN reception timestamp (recv_ts) for this scan index.

        Used internally for interpolating CAN-indexed DataFrames such as
        SRR2_SensorInput_K whose index is CAN recv_ts, not sensor obj_ts.
        """
        v = self._scan_to_recv_ts.get(scan_idx)
        return float(v) if v is not None else None

    def get_host_data(self, scan_idx: int) -> Optional[Dict[str, float]]:
        """
        Return host vehicle data (VehicleSpeed, YawRate) for the given scan
        index, interpolated to the header timestamp.

        Returns ``None`` if ``SRR2_SensorInput_K`` frames are unavailable.
        """
        df = self.frames.get("SRR2_SensorInput_K")
        if df is None or df.empty:
            return None
        # SRR2_SensorInput_K is indexed by CAN recv_ts; use recv_ts for lookup
        # so that the interpolation stays in the correct time domain.
        ts = self.get_recv_timestamp(scan_idx)
        if ts is None:
            return None

        idx_arr = df.index.to_numpy(dtype=float)
        i = int(np.searchsorted(idx_arr, ts))
        if i >= len(idx_arr):
            i = len(idx_arr) - 1
        elif i > 0 and abs(idx_arr[i - 1] - ts) < abs(idx_arr[i] - ts):
            i -= 1

        row = df.iloc[i]
        def _f(col: str) -> float:
            v = row.get(col, 0.0)
            try:
                return float(v)
            except (TypeError, ValueError):
                return 0.0
        return {
            "VehicleSpeed":            _f("VehicleSpeed"),
            "TrailerConnection":       _f("TrailerConnection"),
            "SteeringWheelAngle":      _f("SteeringWheelAngle"),
            "YawRate":                 _f("YawRate"),
            "LongitudinalAcceleration": _f("LongitudinalAcceleration"),
            "LateralAcceleration":     _f("LateralAcceleration"),
        }

    def frame_count(self) -> int:
        return len(self.scan_indices)

    def scan_index_info(self) -> dict:
        """Return a dict with scan-index counts and ranges per side (R / L / combined).

        Keys: 'total', 'r_count', 'r_min', 'r_max', 'l_count', 'l_min', 'l_max'
        Missing sides have count=0 and min/max=None.
        """
        info: dict = {
            "total":   len(self.scan_indices),
            "r_count": 0, "r_min": None, "r_max": None,
            "l_count": 0, "l_min": None, "l_max": None,
        }
        for side_prefix, key_char in (("SRRR", "R"), ("SRRL", "L")):
            key = f"{side_prefix}_Header_SRR2"
            if key not in self.frames:
                continue
            df = self.frames[key]
            si_col = f"Scan_Index_{key_char}"
            if si_col not in df.columns:
                continue
            vals = df[si_col].dropna().astype(int)
            if vals.empty:
                continue
            k = key_char.lower()
            info[f"{k}_count"] = len(vals)
            info[f"{k}_min"]   = int(vals.min())
            info[f"{k}_max"]   = int(vals.max())
        return info

    def display_scan(self, canonical_scan: int, side: str) -> int:
        """Return the sensor-native scan index for display.

        For Left (SRRL) objects the canonical scan_index already IS the
        SRRL sensor index, so it is returned unchanged.

        For Right (SRRR) objects, when the two sensors had non-overlapping
        scan-index counters (counter reset detected during load), the
        original SRRR scan_index that was paired to this canonical SRRL
        frame is returned.  Falls back to canonical_scan when no mapping
        exists (normal overlapping-ranges case or single-side files).
        """
        if side == "R" and canonical_scan in self._canonical_to_srrr:
            return self._canonical_to_srrr[canonical_scan]
        return canonical_scan
