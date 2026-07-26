#!/usr/bin/env python3
from __future__ import annotations

import argparse
import logging
import gzip
import io
import math
import re
import time
from collections import defaultdict
from dataclasses import dataclass, field
from itertools import product
from pathlib import Path
from typing import Dict, Iterable, List, Mapping, Optional, Sequence, Tuple

import h5py
import numpy as np
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots
from scipy.spatial.distance import cdist

from InteractivePlot.b_persistence_layer import hdf_parser
from InteractivePlot.c_data_storage.data_model_storage import DataModelStorage


SENSOR_TOKENS = {"FL", "FR", "RL", "RR", "FC", "RC", "MRR", "SRR"}
MATCH_REQUIRED_COLUMNS = ("ran", "vel", "azimuth", "elevation")
EXPONENTIAL_MATCH_THRESHOLD = 1e5
MATCH_QUANTIZATION_EPSILON = 10.0
MATCH_OFFSETS_4D = sorted(
    list(product([-1, 0, 1], repeat=4)),
    key=lambda off: abs(off[0]) + abs(off[1]) + abs(off[2]) + abs(off[3]),
)
REQUIRED_SIGNAL_KEYS = ("ran", "vel", "theta", "phi")
OPTIONAL_SIGNAL_KEYS = (
    "snr",
    "rcs",
    "f_superres_target",
    "f_ci_det",
    "f_bistatic",
    "vacs_boresight_az_estimated",
    "hdrTimestamp_Sec",
    "hdrTimestamp_fractionalSec",
)
HEADER_VARIANTS = (
    "Stream_Hdr",
    "stream_hdr",
    "StreamHdr",
    "STREAM_HDR",
    "streamheader",
    "stream_header",
    "HEADER_STREAM",
)

logger = logging.getLogger(__name__)


@dataclass(frozen=True)
class FilePair:
    sensor: str
    base_key: str
    veh_path: Path
    resim_path: Path


@dataclass
class LoadedRadar:
    sensor: str
    storage: DataModelStorage
    metadata: Dict[str, str]
    dataset_map: Dict[str, str]
    scan_index: np.ndarray = field(
        default_factory=lambda: np.array([], dtype=np.int64)
    )
    frame: Optional[pd.DataFrame] = None


@dataclass
class MatchOutput:
    matched_veh_idx: np.ndarray
    matched_resim_idx: np.ndarray
    orphan_veh_idx: np.ndarray
    orphan_resim_idx: np.ndarray
    dropped_scan_indices: np.ndarray
    distance_values: np.ndarray
    common_scan_indices: np.ndarray = field(
        default_factory=lambda: np.array([], dtype=np.int64)
    )
    input_only_scan_indices: np.ndarray = field(
        default_factory=lambda: np.array([], dtype=np.int64)
    )
    output_only_scan_indices: np.ndarray = field(
        default_factory=lambda: np.array([], dtype=np.int64)
    )
    scan_match_percentages: np.ndarray = field(
        default_factory=lambda: np.array([], dtype=np.float64)
    )
    total_veh_points: int = 0
    total_resim_points: int = 0


@dataclass
class RadarSideBundle:
    scan_index: np.ndarray
    points: np.ndarray
    optional: Dict[str, np.ndarray]
    scan_to_indices: Dict[int, np.ndarray]
    valid_mask: np.ndarray


@dataclass
class RadarComparisonBundle:
    veh: RadarSideBundle
    resim: RadarSideBundle
    common_scan_indices: np.ndarray
    input_only_scan_indices: np.ndarray
    output_only_scan_indices: np.ndarray
    scan_input_counts: Dict[int, int]
    scan_output_counts: Dict[int, int]


def list_hdf_files(input_dir: Path) -> List[Path]:
    files = sorted(
        p
        for p in input_dir.iterdir()
        if p.is_file() and (p.name.lower().endswith(".h5") or p.name.lower().endswith(".h5.gz"))
    )
    return files


def filename_stem_without_ext(path: Path) -> str:
    name = path.name
    if name.lower().endswith(".h5.gz"):
        return name[:-6]
    if name.lower().endswith(".h5"):
        return name[:-3]
    return path.stem


def infer_sensor(stem: str) -> str:
    upper = stem.upper()
    parts = re.split(r"[^A-Z0-9]+", upper)
    for part in parts:
        if part in SENSOR_TOKENS:
            return part
    return "UNKNOWN"


def is_resim_filename(stem: str) -> bool:
    upper = stem.upper()
    return (
        bool(re.search(r"(^|[-_])R(R)?[0-9]+($|[-_])", upper))
        or bool(re.search(r"(^|[-_])OUTPUT($|[-_])", upper))
        or bool(re.search(r"(^|[-_])CDC($|[-_])", upper))
    )


def normalize_pair_key(stem: str) -> str:
    key = re.sub(r"(?i)(^|[-_])r[0-9a-z]*($|[-_])", r"\1", stem)
    key = re.sub(r"(?i)(^|[-_])(input|output)($|[-_])", r"\1", key)
    key = re.sub(r"(?i)(^|[-_])cdc($|[-_])", r"\1", key)
    key = re.sub(r"[-_]{2,}", "_", key).strip("_-")
    return key


def auto_pair_files(input_dir: Path) -> List[FilePair]:
    files = list_hdf_files(input_dir)
    buckets: Dict[Tuple[str, str], Dict[str, List[Path]]] = {}

    for file_path in files:
        stem = filename_stem_without_ext(file_path)
        sensor = infer_sensor(stem)
        base_key = normalize_pair_key(stem)
        key = (sensor, base_key)
        group = buckets.setdefault(key, {"veh": [], "resim": []})
        if is_resim_filename(stem):
            group["resim"].append(file_path)
        else:
            group["veh"].append(file_path)

    pairs: List[FilePair] = []
    for (sensor, base_key), group in sorted(buckets.items(), key=lambda x: (x[0][0], x[0][1])):
        if not group["veh"] or not group["resim"]:
            continue
        veh_file = sorted(group["veh"])[0]
        resim_file = sorted(group["resim"])[0]
        pairs.append(FilePair(sensor=sensor, base_key=base_key, veh_path=veh_file, resim_path=resim_file))

    return pairs


def _open_hdf(path: Path) -> h5py.File:
    if path.name.lower().endswith(".gz"):
        with gzip.open(path, "rb") as gz_file:
            payload = gz_file.read()
        bio = io.BytesIO(payload)
        return h5py.File(bio, "r")
    return h5py.File(path, "r")


def _dataset_paths(handle: h5py.File) -> List[str]:
    names: List[str] = []

    def visit(name: str, obj: h5py.Dataset) -> None:
        if isinstance(obj, h5py.Dataset):
            names.append(name)

    handle.visititems(visit)
    return names


def _leaf_name(path: str) -> str:
    return path.split("/")[-1]


def _choose_dataset_path(all_paths: Sequence[str], preferred_leaf_names: Sequence[str]) -> Optional[str]:
    leaf_to_paths: Dict[str, List[str]] = {}
    for p in all_paths:
        leaf_to_paths.setdefault(_leaf_name(p).lower(), []).append(p)

    for pref in preferred_leaf_names:
        key = pref.lower()
        if key in leaf_to_paths:
            return sorted(leaf_to_paths[key], key=lambda s: (s.count("/"), len(s)))[0]
    return None


def _candidate_dataset_paths(all_paths: Sequence[str], preferred_leaf_names: Sequence[str]) -> List[str]:
    leaf_to_paths: Dict[str, List[str]] = {}
    for p in all_paths:
        leaf_to_paths.setdefault(_leaf_name(p).lower(), []).append(p)

    candidates: List[str] = []
    for pref in preferred_leaf_names:
        key = pref.lower()
        if key in leaf_to_paths:
            candidates.extend(sorted(leaf_to_paths[key], key=lambda s: (s.count("/"), len(s))))
    return candidates


def _to_1d_numeric(data: np.ndarray) -> np.ndarray:
    if data.ndim == 0:
        return np.asarray([data.item()])
    if data.ndim > 1:
        return np.ravel(data)
    return data


def _to_numeric_ndarray(data: np.ndarray) -> np.ndarray:
    if data.ndim == 0:
        return np.asarray([data.item()])
    return np.asarray(data)


def _to_bool(arr: np.ndarray) -> np.ndarray:
    if arr.dtype == np.bool_:
        return arr
    return np.nan_to_num(arr.astype(np.float64), nan=0.0) > 0.5


def _safe_read_dataset(handle: h5py.File, path: str) -> Optional[np.ndarray]:
    dset = handle[path]
    try:
        data = np.asarray(dset[...])
    except Exception:
        try:
            data = np.asarray(dset.astype("f8")[...])
        except Exception:
            return None
    return _to_numeric_ndarray(data)


def _select_existing_path(handle: h5py.File, paths: Sequence[str]) -> Optional[str]:
    for p in paths:
        if p in handle:
            return p
    return None


def _safe_read_first(handle: h5py.File, paths: Sequence[str]) -> Tuple[Optional[np.ndarray], Optional[str]]:
    for p in paths:
        if p not in handle:
            continue
        arr = _safe_read_dataset(handle, p)
        if arr is not None:
            return arr, p
    return None, None


def _safe_read_first_excluding(
    handle: h5py.File,
    paths: Sequence[str],
    excluded_path: Optional[str],
) -> Tuple[Optional[np.ndarray], Optional[str]]:
    for p in paths:
        if excluded_path is not None and p == excluded_path:
            continue
        if p not in handle:
            continue
        arr = _safe_read_dataset(handle, p)
        if arr is not None:
            return arr, p
    return None, None


def _as_scan_indices(arr: np.ndarray, n_scans: int) -> np.ndarray:
    one_d = np.ravel(arr).astype(np.float64)
    if one_d.size < n_scans:
        pad = np.arange(one_d.size, n_scans, dtype=np.float64)
        one_d = np.concatenate([one_d, pad])
    return np.nan_to_num(one_d[:n_scans], nan=-1).astype(np.int64)


def _matrix_to_long(
    scan_index: np.ndarray,
    ran: np.ndarray,
    vel: np.ndarray,
    azimuth: np.ndarray,
    optional_cols: Mapping[str, np.ndarray],
) -> pd.DataFrame:
    ran2 = np.asarray(ran)
    vel2 = np.asarray(vel)
    az2 = np.asarray(azimuth)
    if ran2.ndim != 2 or vel2.ndim != 2 or az2.ndim != 2:
        raise ValueError("matrix_to_long expects 2D ran/vel/azimuth")

    n_scans = min(ran2.shape[0], vel2.shape[0], az2.shape[0], scan_index.shape[0])
    n_cols = min(ran2.shape[1], vel2.shape[1], az2.shape[1])
    ran2 = ran2[:n_scans, :n_cols].astype(np.float64)
    vel2 = vel2[:n_scans, :n_cols].astype(np.float64)
    az2 = az2[:n_scans, :n_cols].astype(np.float64)
    scan_idx = scan_index[:n_scans]

    mask = np.isfinite(ran2) & np.isfinite(vel2) & np.isfinite(az2)
    if not np.any(mask):
        return pd.DataFrame(columns=["scan_index", "ran", "vel", "azimuth"])

    scan_repeat = np.repeat(scan_idx[:, None], n_cols, axis=1)
    base = {
        "scan_index": scan_repeat[mask].astype(np.int64),
        "ran": ran2[mask],
        "vel": vel2[mask],
        "azimuth": az2[mask],
    }
    df = pd.DataFrame(base)

    for key, arr in optional_cols.items():
        a = np.asarray(arr)
        if a.ndim == 2:
            if a.shape[0] >= n_scans and a.shape[1] >= n_cols:
                cut = a[:n_scans, :n_cols]
                df[key] = cut[mask]
        elif a.ndim == 1 and a.shape[0] >= n_scans:
            per_scan = np.repeat(a[:n_scans], n_cols)
            df[key] = per_scan[mask.ravel()]

    return df.reset_index(drop=True)


def _build_from_af_det_with_rdd_fallback(handle: h5py.File, sensor: str) -> Optional[LoadedRadar]:
    af_prefix = f"{sensor}/DETECTION_STREAM/AF_Det"
    if af_prefix not in handle:
        return None

    dataset_map: Dict[str, str] = {}

    scan_candidates = (
        f"{sensor}/DETECTION_STREAM/Stream_Hdr/scan_index",
        f"{sensor}/RDD_STREAM/Look_Data/scan_index",
        f"{sensor}/RDD_STREAM/Stream_Hdr/scan_index",
    )
    scan_raw, scan_path = _safe_read_first(handle, scan_candidates)
    if scan_raw is None:
        return None

    num_af_det, num_path = _safe_read_first(handle, (f"{af_prefix}/num_af_det",))
    if num_af_det is not None and num_path:
        dataset_map["num_af_det"] = num_path

    ran_raw, ran_path = _safe_read_first(handle, (f"{af_prefix}/ran", f"{af_prefix}/range"))
    vel_raw, vel_path = _safe_read_first(handle, (f"{af_prefix}/vel", f"{af_prefix}/velocity", f"{af_prefix}/doppler"))
    az_raw, az_path = _safe_read_first(handle, (f"{af_prefix}/azimuth", f"{af_prefix}/phi", f"{af_prefix}/theta"))
    el_raw, el_path = _safe_read_first_excluding(
        handle,
        (f"{af_prefix}/elevation", f"{af_prefix}/el", f"{af_prefix}/ele", f"{af_prefix}/theta", f"{af_prefix}/phi"),
        az_path,
    )

    if ran_raw is not None and ran_path:
        dataset_map["ran"] = ran_path
    if vel_raw is not None and vel_path:
        dataset_map["vel"] = vel_path
    if az_raw is not None and az_path:
        dataset_map["azimuth"] = az_path
    if el_raw is not None and el_path:
        dataset_map["elevation"] = el_path

    rdd_idx, rdd_idx_path = _safe_read_first(handle, (f"{af_prefix}/rdd_idx",))
    if rdd_idx is not None and rdd_idx_path:
        dataset_map["rdd_idx"] = rdd_idx_path

    if (ran_raw is None or vel_raw is None) and rdd_idx is not None:
        r2_range, r2_range_path = _safe_read_first(handle, (f"{sensor}/RDD_STREAM/RDD_Data/rdd2_range",))
        r2_rate, r2_rate_path = _safe_read_first(handle, (f"{sensor}/RDD_STREAM/RDD_Data/rdd2_range_rate",))
        if r2_range is not None and r2_rate is not None:
            if r2_range_path:
                dataset_map["rdd2_range"] = r2_range_path
            if r2_rate_path:
                dataset_map["rdd2_range_rate"] = r2_rate_path
            ridx = np.rint(np.asarray(rdd_idx)).astype(np.int64)
            r2r = np.asarray(r2_range)
            r2v = np.asarray(r2_rate)
            if ridx.ndim == 2 and r2r.ndim == 2 and r2v.ndim == 2:
                ns = min(ridx.shape[0], r2r.shape[0], r2v.shape[0])
                nc = ridx.shape[1]
                ridx = ridx[:ns, :nc]
                max_idx = min(r2r.shape[1], r2v.shape[1])
                valid = (ridx >= 0) & (ridx < max_idx)
                rr = np.full_like(ridx, np.nan, dtype=np.float64)
                rv = np.full_like(ridx, np.nan, dtype=np.float64)
                row_ids = np.broadcast_to(np.arange(ns, dtype=np.int64)[:, None], ridx.shape)
                rr[valid] = r2r[:ns, :][row_ids[valid], ridx[valid]]
                rv[valid] = r2v[:ns, :][row_ids[valid], ridx[valid]]
                if ran_raw is None:
                    ran_raw = rr
                if vel_raw is None:
                    vel_raw = rv

    if az_raw is None and rdd_idx is not None:
        az_raw = np.asarray(rdd_idx).astype(np.float64)
        dataset_map["azimuth"] = dataset_map.get("rdd_idx", f"{af_prefix}/rdd_idx")

    if ran_raw is None or vel_raw is None or az_raw is None:
        return None

    optional_paths = {
        "f_superres_target": (f"{af_prefix}/f_superres_target",),
        "f_ci_det": (f"{af_prefix}/f_ci_det",),
        "f_bistatic": (f"{af_prefix}/f_bistatic",),
        "snr": (f"{af_prefix}/snr", f"{sensor}/RDD_STREAM/RDD_Data/rdd2_snr"),
        "rcs": (f"{af_prefix}/rcs",),
        "vacs_boresight_az_estimated": (
            f"{sensor}/DYNAMIC_ALIGNMENT_STREAM/DRA_Core_Log_Data_T/estimated_yawrate",
            f"{sensor}/HEADER_STREAM/Dynamic_Alignment_Data_Log/AACurrentAzimuth",
        ),
        "hdrTimestamp_Sec": (f"{sensor}/RDD_STREAM/Look_Data/mid_dwell_sec",),
        "hdrTimestamp_fractionalSec": (f"{sensor}/RDD_STREAM/Look_Data/mid_dwell_nanosec",),
    }

    optional_data: Dict[str, np.ndarray] = {}
    for key, paths in optional_paths.items():
        arr, pth = _safe_read_first(handle, paths)
        if arr is not None:
            optional_data[key] = arr
            if pth:
                dataset_map[key] = pth
    if el_raw is not None:
        optional_data["elevation"] = np.asarray(el_raw)

    if ran_raw.ndim == 2 and vel_raw.ndim == 2:
        n_scans = min(ran_raw.shape[0], vel_raw.shape[0], az_raw.shape[0] if az_raw.ndim == 2 else scan_raw.shape[0])
        scan_index = _as_scan_indices(np.asarray(scan_raw), n_scans)
        if az_raw.ndim == 1:
            az_raw = np.repeat(az_raw[:n_scans, None], ran_raw.shape[1], axis=1)

        if num_af_det is not None and num_af_det.ndim == 1:
            m = np.rint(np.asarray(num_af_det[:n_scans])).astype(np.int64)
            m = np.clip(m, 0, ran_raw.shape[1])
            valid = np.arange(ran_raw.shape[1])[None, :] < m[:, None]
            ran2 = np.asarray(ran_raw[:n_scans, :], dtype=np.float64)
            vel2 = np.asarray(vel_raw[:n_scans, :], dtype=np.float64)
            az2 = np.asarray(az_raw[:n_scans, :], dtype=np.float64)
            ran2 = np.where(valid, ran2, np.nan)
            vel2 = np.where(valid, vel2, np.nan)
            az2 = np.where(valid, az2, np.nan)
        else:
            ran2 = np.asarray(ran_raw[:n_scans, :], dtype=np.float64)
            vel2 = np.asarray(vel_raw[:n_scans, :], dtype=np.float64)
            az2 = np.asarray(az_raw[:n_scans, :], dtype=np.float64)

        df = _matrix_to_long(scan_index=scan_index, ran=ran2, vel=vel2, azimuth=az2, optional_cols=optional_data)
    else:
        required = [np.ravel(np.asarray(scan_raw)), np.ravel(np.asarray(ran_raw)), np.ravel(np.asarray(vel_raw)), np.ravel(np.asarray(az_raw))]
        target_length = min(x.shape[0] for x in required)
        df = pd.DataFrame(
            {
                "scan_index": np.nan_to_num(required[0][:target_length], nan=-1).astype(np.int64),
                "ran": required[1][:target_length].astype(np.float64),
                "vel": required[2][:target_length].astype(np.float64),
                "azimuth": required[3][:target_length].astype(np.float64),
            }
        )
        if el_raw is not None:
            df["elevation"] = np.ravel(np.asarray(el_raw))[:target_length].astype(np.float64)

    for flag in ("f_superres_target", "f_ci_det", "f_bistatic"):
        if flag in df.columns:
            df[flag] = _to_bool(np.asarray(df[flag]))

    if "timestamp" not in df.columns and {"hdrTimestamp_Sec", "hdrTimestamp_fractionalSec"}.issubset(df.columns):
        frac = df["hdrTimestamp_fractionalSec"].astype(np.float64)
        if np.nanmax(np.abs(frac.to_numpy())) > 1e4:
            frac = frac / 1e9
        df["timestamp"] = df["hdrTimestamp_Sec"].astype(np.float64) + frac

    df = df.replace([np.inf, -np.inf], np.nan).dropna(subset=["scan_index", "ran", "vel", "azimuth"]).reset_index(drop=True)
    if df.empty:
        return None

    metadata = {"sensor": sensor}
    return LoadedRadar(sensor=sensor, frame=df, metadata=metadata, dataset_map=dataset_map)


def _read_metadata(handle: h5py.File, dset_paths: Sequence[str]) -> Dict[str, str]:
    metadata_candidates: Mapping[str, Sequence[str]] = {
        "DC_version": ("DC_version",),
        "Tracker_version": ("Tracker_version",),
        "OCG_version": ("OCG_version",),
        "OLP_version": ("OLP_version",),
        "SFL_version": ("SFL_version",),
        "Created_datetime": ("Created_datetime",),
    }
    metadata: Dict[str, str] = {}
    for key, candidates in metadata_candidates.items():
        value = None
        for candidate in candidates:
            if "/" in candidate and candidate in handle:
                value = _safe_read_dataset(handle, candidate)
            else:
                for path in _candidate_dataset_paths(dset_paths, (candidate,)):
                    value = _safe_read_dataset(handle, path)
                    if value is not None:
                        break
            if value is not None:
                break
        if value is not None and value.size > 0:
            metadata[key] = str(value[0])
    return metadata


def _scan_seed(arr: np.ndarray) -> np.ndarray:
    raw = np.asarray(arr)
    if raw.ndim == 0:
        return np.asarray([raw.item()], dtype=np.float64)
    if raw.ndim == 1:
        return raw.astype(np.float64, copy=False)
    return raw.reshape(raw.shape[0], -1)[:, 0].astype(np.float64, copy=False)


def _storage_signal_available(storage: DataModelStorage, signal_name: str) -> bool:
    signal_map = getattr(storage, "_signal_to_value", None)
    if not signal_map:
        return False
    if signal_name in signal_map:
        return True
    normalized = "".join(ch for ch in signal_name.lower() if ch.isalnum())
    for existing in signal_map.keys():
        existing_normalized = "".join(ch for ch in str(existing).lower() if ch.isalnum())
        if existing_normalized == normalized:
            return True
    return False


def _load_radar_storage(
    handle: h5py.File,
    sensor: str,
) -> tuple[DataModelStorage, Dict[str, str], Dict[str, str]]:
    scan_candidates = (
        f"{sensor}/DETECTION_STREAM/Stream_Hdr/scan_index",
        f"{sensor}/RDD_STREAM/Look_Data/scan_index",
        f"{sensor}/RDD_STREAM/Stream_Hdr/scan_index",
    )
    scan_raw, scan_path = _safe_read_first(handle, scan_candidates)
    if scan_raw is None:
        raise ValueError(f"{sensor}: missing scan_index dataset")

    scan_seed = _scan_seed(scan_raw)
    scan_index = _as_scan_indices(scan_seed, scan_seed.shape[0])
    dataset_map: Dict[str, str] = {}
    if scan_path:
        dataset_map["scan_index"] = scan_path

    storage = DataModelStorage()
    storage.initialize(scan_index.tolist(), sensor, "DETECTION_STREAM")

    for stream_name in ("DETECTION_STREAM", "RDD_STREAM"):
        stream_path = f"{sensor}/{stream_name}"
        if stream_path not in handle:
            continue
        storage.init_parent(stream_name)
        hdf_parser.HDF5Parser.parse(
            handle[stream_path],
            storage,
            scan_index.tolist(),
            HEADER_VARIANTS,
        )

    aux_candidates: Mapping[str, Sequence[str]] = {
        "vacs_boresight_az_estimated": (
            f"{sensor}/DYNAMIC_ALIGNMENT_STREAM/DRA_Core_Log_Data_T/estimated_yawrate",
            f"{sensor}/HEADER_STREAM/Dynamic_Alignment_Data_Log/AACurrentAzimuth",
        ),
        "hdrTimestamp_Sec": (f"{sensor}/RDD_STREAM/Look_Data/mid_dwell_sec",),
        "hdrTimestamp_fractionalSec": (f"{sensor}/RDD_STREAM/Look_Data/mid_dwell_nanosec",),
    }
    aux_started = False
    for signal_name, paths in aux_candidates.items():
        arr, found_path = _safe_read_first(handle, paths)
        if arr is None:
            continue
        raw = np.asarray(arr)
        if raw.ndim > 1:
            raw = raw.reshape(raw.shape[0], -1)
            if raw.shape[1] != 1:
                continue
            raw = raw[:, 0]
        if raw.shape[0] < scan_index.shape[0]:
            continue
        if not aux_started:
            storage.init_parent("AUX_STREAM")
            aux_started = True
        storage.set_value(raw[: scan_index.shape[0]], signal_name, "AUX_STREAM")
        if found_path:
            dataset_map[signal_name] = found_path

    missing_required = [
        signal_name
        for signal_name in REQUIRED_SIGNAL_KEYS
        if not _storage_signal_available(storage, signal_name)
    ]
    if missing_required:
        raise ValueError(f"{sensor}: missing required signals {missing_required}")

    metadata = _read_metadata(handle, _dataset_paths(handle))
    metadata.setdefault("sensor", sensor)
    return storage, metadata, dataset_map


def _storage_to_frame(storage: DataModelStorage) -> pd.DataFrame:
    signal_aliases = {
        "ran": "ran",
        "vel": "vel",
        "theta": "azimuth",
        "phi": "elevation",
        "snr": "snr",
        "rcs": "rcs",
        "f_superres_target": "f_superres_target",
        "f_ci_det": "f_ci_det",
        "f_bistatic": "f_bistatic",
        "vacs_boresight_az_estimated": "vacs_boresight_az_estimated",
        "hdrTimestamp_Sec": "hdrTimestamp_Sec",
        "hdrTimestamp_fractionalSec": "hdrTimestamp_fractionalSec",
    }
    payloads: Dict[str, Dict[str, np.ndarray]] = {}
    for signal_name in signal_aliases.keys():
        result = DataModelStorage.get_data(storage, storage, signal_name)
        if not isinstance(result, dict) or result.get("SI") is None:
            continue
        payloads[signal_name] = result

    if not all(signal in payloads for signal in REQUIRED_SIGNAL_KEYS):
        return pd.DataFrame(columns=["scan_index", "ran", "vel", "azimuth", "elevation"])

    length = min(len(payloads[signal]["I"]) for signal in REQUIRED_SIGNAL_KEYS)
    if length <= 0:
        return pd.DataFrame(columns=["scan_index", "ran", "vel", "azimuth", "elevation"])

    frame_data: Dict[str, np.ndarray] = {
        "scan_index": np.asarray(payloads["ran"]["SI"][:length], dtype=np.int64),
    }
    for source_name, target_name in signal_aliases.items():
        payload = payloads.get(source_name)
        if payload is None:
            continue
        frame_data[target_name] = np.asarray(payload["I"][:length], dtype=np.float64)

    if "hdrTimestamp_Sec" in frame_data and "hdrTimestamp_fractionalSec" in frame_data:
        frac = frame_data["hdrTimestamp_fractionalSec"].astype(np.float64)
        if np.nanmax(np.abs(frac)) > 1e4:
            frac = frac / 1e9
        frame_data["timestamp"] = frame_data["hdrTimestamp_Sec"].astype(np.float64) + frac

    return pd.DataFrame(frame_data)


def load_radar_hdf(path: Path, sensor: str, build_frame: bool = False) -> LoadedRadar:
    with _open_hdf(path) as handle:
        storage, metadata, dataset_map = _load_radar_storage(handle, sensor)

    frame = _storage_to_frame(storage) if build_frame else None
    scan_index = np.asarray(sorted(storage._data_container.keys()), dtype=np.int64)
    return LoadedRadar(
        sensor=sensor,
        storage=storage,
        metadata=metadata,
        dataset_map=dataset_map,
        scan_index=scan_index,
        frame=frame,
    )


def sensors_in_file(path: Path) -> List[str]:
    with _open_hdf(path) as handle:
        sensors: List[str] = []
        for key in handle.keys():
            k = str(key).upper()
            if k in SENSOR_TOKENS:
                sensors.append(k)
        return sorted(set(sensors))


def _distance_metric(veh_points: np.ndarray, resim_points: np.ndarray, metric: str) -> np.ndarray:
    if metric == "mahalanobis":
        merged = np.vstack([veh_points, resim_points])
        cov = np.cov(merged.T)
        if np.linalg.matrix_rank(cov) < cov.shape[0]:
            cov = cov + np.eye(cov.shape[0]) * 1e-6
        inv_cov = np.linalg.pinv(cov)
        return cdist(veh_points, resim_points, metric="mahalanobis", VI=inv_cov)
    return cdist(veh_points, resim_points, metric="euclidean")


def _prepare_match_dataframe(df: pd.DataFrame) -> Tuple[pd.DataFrame, np.ndarray]:
    working_df = df.copy()
    for col in MATCH_REQUIRED_COLUMNS:
        if col not in working_df.columns:
            working_df[col] = np.nan

    numeric = working_df[list(MATCH_REQUIRED_COLUMNS)].apply(pd.to_numeric, errors="coerce")
    values = numeric.to_numpy(dtype=np.float64)
    finite_mask = np.isfinite(values).all(axis=1)
    bounded_mask = (np.abs(values) <= EXPONENTIAL_MATCH_THRESHOLD).all(axis=1)
    valid_mask = finite_mask & bounded_mask

    excluded_idx = working_df.index.to_numpy(dtype=np.int64)[~valid_mask]
    return working_df.loc[valid_mask], excluded_idx


def _quantize_match_value(value: float) -> int:
    return int(round(float(value) / MATCH_QUANTIZATION_EPSILON))


def _build_match_key(values: np.ndarray) -> Tuple[int, int, int, int]:
    return tuple(_quantize_match_value(v) for v in values.tolist())


def _row_distance(
    veh_row: np.ndarray,
    resim_row: np.ndarray,
    metric: str,
) -> float:
    return float(
        _distance_metric(
            np.asarray(veh_row, dtype=np.float64)[None, :],
            np.asarray(resim_row, dtype=np.float64)[None, :],
            metric,
        )[0, 0]
    )


def match_points(
    veh_df: pd.DataFrame,
    resim_df: pd.DataFrame,
    gate_threshold: float,
    metric: str,
) -> MatchOutput:
    _ = gate_threshold
    veh_match_df, veh_excluded_idx = _prepare_match_dataframe(veh_df)
    resim_match_df, resim_excluded_idx = _prepare_match_dataframe(resim_df)

    veh_groups = veh_match_df.groupby("scan_index").indices
    resim_groups = resim_match_df.groupby("scan_index").indices

    veh_scans = set(veh_groups.keys())
    resim_scans = set(resim_groups.keys())
    common_scans = np.array(sorted(veh_scans & resim_scans), dtype=np.int64)
    input_only_scans = np.array(sorted(veh_scans - resim_scans), dtype=np.int64)
    output_only_scans = np.array(sorted(resim_scans - veh_scans), dtype=np.int64)

    matched_veh_idx_parts: List[np.ndarray] = []
    matched_resim_idx_parts: List[np.ndarray] = []
    orphan_veh_idx_parts: List[np.ndarray] = []
    orphan_resim_idx_parts: List[np.ndarray] = []
    dist_parts: List[np.ndarray] = []
    scan_match_pct: List[float] = []

    veh_xyz = veh_match_df[["ran", "vel", "azimuth", "elevation"]].to_numpy(dtype=np.float64, copy=False)
    resim_xyz = resim_match_df[["ran", "vel", "azimuth", "elevation"]].to_numpy(dtype=np.float64, copy=False)

    for scan in common_scans:
        veh_idx = np.asarray(veh_groups[scan], dtype=np.int64)
        resim_idx = np.asarray(resim_groups[scan], dtype=np.int64)
        if veh_idx.size == 0 or resim_idx.size == 0:
            continue

        out_map: Dict[Tuple[int, int, int, int], List[int]] = {}
        for resim_row_idx in resim_idx:
            out_map.setdefault(_build_match_key(resim_xyz[resim_row_idx]), []).append(
                int(resim_row_idx)
            )

        matched_veh_scan: List[int] = []
        matched_resim_scan: List[int] = []
        matched_distances: List[float] = []

        for veh_row_idx in veh_idx:
            veh_row = veh_xyz[veh_row_idx]
            base = _build_match_key(veh_row)
            matched_resim_row_idx: Optional[int] = None

            for off in MATCH_OFFSETS_4D:
                key = (
                    base[0] + off[0],
                    base[1] + off[1],
                    base[2] + off[2],
                    base[3] + off[3],
                )
                bucket = out_map.get(key)
                if not bucket:
                    continue
                matched_resim_row_idx = bucket.pop(0)
                if not bucket:
                    del out_map[key]
                break

            if matched_resim_row_idx is None:
                continue

            matched_veh_scan.append(int(veh_row_idx))
            matched_resim_scan.append(int(matched_resim_row_idx))
            matched_distances.append(
                _row_distance(veh_row, resim_xyz[matched_resim_row_idx], metric)
            )

        matched_veh_arr = np.asarray(matched_veh_scan, dtype=np.int64)
        matched_resim_arr = np.asarray(matched_resim_scan, dtype=np.int64)
        matched_veh_idx_parts.append(matched_veh_arr)
        matched_resim_idx_parts.append(matched_resim_arr)
        if matched_distances:
            dist_parts.append(np.asarray(matched_distances, dtype=np.float64))

        orphan_veh_idx_parts.append(
            np.setdiff1d(veh_idx, matched_veh_arr, assume_unique=False).astype(np.int64)
        )
        orphan_resim_idx_parts.append(
            np.setdiff1d(resim_idx, matched_resim_arr, assume_unique=False).astype(np.int64)
        )

        denom = max(int(veh_idx.size), int(resim_idx.size))
        scan_match_pct.append(
            100.0 * float(matched_veh_arr.size) / float(denom) if denom > 0 else 100.0
        )

    def _concat(parts: List[np.ndarray]) -> np.ndarray:
        if not parts:
            return np.array([], dtype=np.int64)
        non_empty = [p for p in parts if p.size > 0]
        if not non_empty:
            return np.array([], dtype=np.int64)
        return np.concatenate(non_empty)

    def _concat_float(parts: List[np.ndarray]) -> np.ndarray:
        if not parts:
            return np.array([], dtype=np.float64)
        non_empty = [p for p in parts if p.size > 0]
        if not non_empty:
            return np.array([], dtype=np.float64)
        return np.concatenate(non_empty).astype(np.float64)

    orphan_veh_idx = _concat(orphan_veh_idx_parts)
    orphan_resim_idx = _concat(orphan_resim_idx_parts)
    if veh_excluded_idx.size > 0:
        orphan_veh_idx = np.concatenate([orphan_veh_idx, veh_excluded_idx]) if orphan_veh_idx.size > 0 else veh_excluded_idx
    if resim_excluded_idx.size > 0:
        orphan_resim_idx = np.concatenate([orphan_resim_idx, resim_excluded_idx]) if orphan_resim_idx.size > 0 else resim_excluded_idx

    return MatchOutput(
        matched_veh_idx=_concat(matched_veh_idx_parts),
        matched_resim_idx=_concat(matched_resim_idx_parts),
        orphan_veh_idx=orphan_veh_idx.astype(np.int64),
        orphan_resim_idx=orphan_resim_idx.astype(np.int64),
        dropped_scan_indices=input_only_scans,
        distance_values=_concat_float(dist_parts),
        common_scan_indices=common_scans,
        input_only_scan_indices=input_only_scans,
        output_only_scan_indices=output_only_scans,
        scan_match_percentages=np.asarray(scan_match_pct, dtype=np.float64),
        total_veh_points=int(len(veh_df)),
        total_resim_points=int(len(resim_df)),
    )


def _safe_percentile(values: np.ndarray, q: float) -> float:
    if values.size == 0:
        return float("nan")
    return float(np.nanpercentile(values, q))


def _timestamp_jitter(df: pd.DataFrame) -> float:
    if "timestamp" not in df.columns:
        return float("nan")
    ts = df.groupby("scan_index", sort=True)["timestamp"].mean().to_numpy(dtype=np.float64)
    if ts.size < 3:
        return float("nan")
    delta = np.diff(ts)
    if delta.size == 0:
        return float("nan")
    return float(np.nanstd(delta))


def _scan_index_match_pct(match: MatchOutput) -> float:
    input_scan_total = float(
        match.common_scan_indices.size + match.input_only_scan_indices.size
    )
    if input_scan_total <= 0:
        return float("nan")
    return 100.0 * float(match.common_scan_indices.size) / input_scan_total


def compute_kpis(veh_df: pd.DataFrame, resim_df: pd.DataFrame, match: MatchOutput) -> Dict[str, float]:
    total_veh = float(len(veh_df))
    total_resim = float(len(resim_df))
    matched = float(match.matched_veh_idx.size)

    # Use only points from common scan indices for recall/precision/F1
    common_set = set(match.common_scan_indices.tolist())
    if common_set and len(veh_df) > 0 and "scan_index" in veh_df.columns:
        common_veh = float(veh_df["scan_index"].isin(common_set).sum())
    else:
        common_veh = total_veh
    if common_set and len(resim_df) > 0 and "scan_index" in resim_df.columns:
        common_resim = float(resim_df["scan_index"].isin(common_set).sum())
    else:
        common_resim = total_resim

    if matched > 0:
        delta_ran = veh_df.loc[match.matched_veh_idx, "ran"].to_numpy() - resim_df.loc[match.matched_resim_idx, "ran"].to_numpy()
        delta_vel = veh_df.loc[match.matched_veh_idx, "vel"].to_numpy() - resim_df.loc[match.matched_resim_idx, "vel"].to_numpy()
    else:
        delta_ran = np.array([], dtype=np.float64)
        delta_vel = np.array([], dtype=np.float64)

    abs_dran = np.abs(delta_ran)
    abs_dvel = np.abs(delta_vel)
    strict_mask = (abs_dran < 0.1) & (abs_dvel < 0.1)
    strict_count = float(np.count_nonzero(strict_mask))

    p95_dran = _safe_percentile(abs_dran, 95)
    p99_dvel = _safe_percentile(abs_dvel, 99)
    recall = matched / common_veh if common_veh > 0 else float("nan")
    precision = matched / common_resim if common_resim > 0 else float("nan")
    if not math.isnan(recall) and not math.isnan(precision) and (recall + precision) > 0:
        f1_score = 2.0 * recall * precision / (recall + precision)
    else:
        f1_score = float("nan")
    scan_index_match_pct = _scan_index_match_pct(match)

    return {
        "total_veh_points": total_veh,
        "total_resim_points": total_resim,
        "matched_points": matched,
        "unmatched_veh_points": float(match.orphan_veh_idx.size),
        "unmatched_resim_points": float(match.orphan_resim_idx.size),
        "matched_scan_index_count": float(match.common_scan_indices.size),
        "common_scan_count": float(match.common_scan_indices.size),
        "input_only_scan_count": float(match.input_only_scan_indices.size),
        "output_only_scan_count": float(match.output_only_scan_indices.size),
        "avg_scan_match_pct": scan_index_match_pct,
        "resim_recall": recall,
        "resim_precision": precision,
        "f1_score": f1_score,
        "p95_abs_dran": p95_dran,
        "p99_abs_dvel": p99_dvel,
        "range_bound_flag": float(p95_dran > 0.1) if not math.isnan(p95_dran) else float("nan"),
        "vel_bound_flag": float(p99_dvel > 0.05) if not math.isnan(p99_dvel) else float("nan"),
        "veh_timestamp_jitter": _timestamp_jitter(veh_df),
        "resim_timestamp_jitter": _timestamp_jitter(resim_df),
        "pcfs": strict_count / total_veh if total_veh > 0 else float("nan"),
        "dropped_frames": float(match.dropped_scan_indices.size),
        "mean_gate_distance": float(np.nanmean(match.distance_values)) if match.distance_values.size > 0 else float("nan"),
    }


def _build_side_bundle(
    signal_payloads: Dict[str, Dict[str, object]],
    side_key: str,
) -> RadarSideBundle:
    scan_value_key = "scan_input_values" if side_key == "I" else "scan_output_values"
    scan_indices: List[int] = sorted(
        {
            int(scan_idx)
            for payload in signal_payloads.values()
            for scan_idx in payload.get(scan_value_key, {}).keys()
        }
    )

    flat_scan: List[np.ndarray] = []
    flat_points: List[np.ndarray] = []
    optional_data: Dict[str, List[np.ndarray]] = defaultdict(list)
    scan_to_indices: Dict[int, np.ndarray] = {}
    offset = 0

    for scan_idx in scan_indices:
        required_vectors: List[np.ndarray] = []
        for signal_name in REQUIRED_SIGNAL_KEYS:
            payload = signal_payloads.get(signal_name, {})
            raw_values = payload.get(scan_value_key, {}).get(scan_idx, [])
            values = np.asarray(raw_values, dtype=np.float64)
            required_vectors.append(values)

        if not required_vectors or any(values.size == 0 for values in required_vectors):
            continue

        row_count = min(values.size for values in required_vectors)
        if row_count <= 0:
            continue

        points = np.column_stack([values[:row_count] for values in required_vectors]).astype(
            np.float64,
            copy=False,
        )
        flat_points.append(points)
        flat_scan.append(np.full(row_count, scan_idx, dtype=np.int64))
        scan_to_indices[scan_idx] = np.arange(offset, offset + row_count, dtype=np.int64)
        offset += row_count

        for signal_name in OPTIONAL_SIGNAL_KEYS:
            payload = signal_payloads.get(signal_name)
            values = [] if payload is None else payload.get(scan_value_key, {}).get(scan_idx, [])
            out = np.full(row_count, np.nan, dtype=np.float64)
            if len(values) > 0:
                numeric = np.asarray(values, dtype=np.float64)
                take = min(row_count, numeric.size)
                out[:take] = numeric[:take]
            optional_data[signal_name].append(out)

    if flat_points:
        scan_index = np.concatenate(flat_scan).astype(np.int64)
        points = np.concatenate(flat_points).astype(np.float64)
    else:
        scan_index = np.array([], dtype=np.int64)
        points = np.empty((0, len(REQUIRED_SIGNAL_KEYS)), dtype=np.float64)

    optional_arrays = {
        signal_name: (
            np.concatenate(chunks).astype(np.float64) if chunks else np.array([], dtype=np.float64)
        )
        for signal_name, chunks in optional_data.items()
    }

    valid_mask = (
        np.isfinite(points).all(axis=1)
        & (np.abs(points) <= EXPONENTIAL_MATCH_THRESHOLD).all(axis=1)
        if points.size > 0
        else np.array([], dtype=bool)
    )

    return RadarSideBundle(
        scan_index=scan_index,
        points=points,
        optional=optional_arrays,
        scan_to_indices=scan_to_indices,
        valid_mask=valid_mask,
    )


def _build_comparison_bundle(veh: LoadedRadar, resim: LoadedRadar) -> RadarComparisonBundle:
    signal_names = REQUIRED_SIGNAL_KEYS + OPTIONAL_SIGNAL_KEYS
    signal_payloads: Dict[str, Dict[str, object]] = {}
    for signal_name in signal_names:
        result = DataModelStorage.get_data(veh.storage, resim.storage, signal_name)
        if isinstance(result, dict):
            signal_payloads[signal_name] = result

    primary = signal_payloads.get("ran")
    if primary is None:
        raise ValueError(f"{veh.sensor}: unable to prepare range signal bundle")

    return RadarComparisonBundle(
        veh=_build_side_bundle(signal_payloads, "I"),
        resim=_build_side_bundle(signal_payloads, "O"),
        common_scan_indices=np.asarray(primary.get("common_scan_indices", []), dtype=np.int64),
        input_only_scan_indices=np.asarray(primary.get("input_only_scan_indices", []), dtype=np.int64),
        output_only_scan_indices=np.asarray(primary.get("output_only_scan_indices", []), dtype=np.int64),
        scan_input_counts={
            int(key): int(value)
            for key, value in primary.get("scan_input_counts", {}).items()
        },
        scan_output_counts={
            int(key): int(value)
            for key, value in primary.get("scan_output_counts", {}).items()
        },
    )


def match_loaded_radars(bundle: RadarComparisonBundle, metric: str) -> MatchOutput:
    matched_veh_idx_parts: List[np.ndarray] = []
    matched_resim_idx_parts: List[np.ndarray] = []
    orphan_veh_idx_parts: List[np.ndarray] = []
    orphan_resim_idx_parts: List[np.ndarray] = []
    dist_parts: List[np.ndarray] = []
    scan_match_pct: List[float] = []

    veh_scan_lookup = bundle.veh.scan_to_indices
    resim_scan_lookup = bundle.resim.scan_to_indices

    for scan_idx in bundle.common_scan_indices:
        scan_int = int(scan_idx)
        veh_all = veh_scan_lookup.get(scan_int, np.array([], dtype=np.int64))
        resim_all = resim_scan_lookup.get(scan_int, np.array([], dtype=np.int64))

        if veh_all.size > 0:
            orphan_veh_idx_parts.append(veh_all[~bundle.veh.valid_mask[veh_all]])
        if resim_all.size > 0:
            orphan_resim_idx_parts.append(resim_all[~bundle.resim.valid_mask[resim_all]])

        veh_idx = veh_all[bundle.veh.valid_mask[veh_all]] if veh_all.size > 0 else np.array([], dtype=np.int64)
        resim_idx = resim_all[bundle.resim.valid_mask[resim_all]] if resim_all.size > 0 else np.array([], dtype=np.int64)

        out_map: Dict[Tuple[int, int, int, int], List[int]] = {}
        for resim_row_idx in resim_idx:
            out_map.setdefault(_build_match_key(bundle.resim.points[resim_row_idx]), []).append(int(resim_row_idx))

        matched_veh_scan: List[int] = []
        matched_resim_scan: List[int] = []
        matched_distances: List[float] = []

        for veh_row_idx in veh_idx:
            veh_row = bundle.veh.points[veh_row_idx]
            base = _build_match_key(veh_row)
            matched_resim_row_idx: Optional[int] = None
            for off in MATCH_OFFSETS_4D:
                key = (
                    base[0] + off[0],
                    base[1] + off[1],
                    base[2] + off[2],
                    base[3] + off[3],
                )
                bucket = out_map.get(key)
                if not bucket:
                    continue
                matched_resim_row_idx = bucket.pop(0)
                if not bucket:
                    del out_map[key]
                break

            if matched_resim_row_idx is None:
                continue

            matched_veh_scan.append(int(veh_row_idx))
            matched_resim_scan.append(int(matched_resim_row_idx))
            matched_distances.append(
                _row_distance(veh_row, bundle.resim.points[matched_resim_row_idx], metric)
            )

        matched_veh_arr = np.asarray(matched_veh_scan, dtype=np.int64)
        matched_resim_arr = np.asarray(matched_resim_scan, dtype=np.int64)
        matched_veh_idx_parts.append(matched_veh_arr)
        matched_resim_idx_parts.append(matched_resim_arr)

        if matched_distances:
            dist_parts.append(np.asarray(matched_distances, dtype=np.float64))

        orphan_veh_idx_parts.append(np.setdiff1d(veh_idx, matched_veh_arr, assume_unique=False).astype(np.int64))
        orphan_resim_idx_parts.append(np.setdiff1d(resim_idx, matched_resim_arr, assume_unique=False).astype(np.int64))

        denom = max(bundle.scan_input_counts.get(scan_int, 0), bundle.scan_output_counts.get(scan_int, 0))
        scan_match_pct.append(100.0 * float(matched_veh_arr.size) / float(denom) if denom > 0 else 100.0)

    for scan_idx in bundle.input_only_scan_indices:
        orphan_veh_idx_parts.append(veh_scan_lookup.get(int(scan_idx), np.array([], dtype=np.int64)))
    for scan_idx in bundle.output_only_scan_indices:
        orphan_resim_idx_parts.append(resim_scan_lookup.get(int(scan_idx), np.array([], dtype=np.int64)))

    def _concat(parts: List[np.ndarray], dtype: np.dtype) -> np.ndarray:
        non_empty = [part.astype(dtype, copy=False) for part in parts if part.size > 0]
        if not non_empty:
            return np.array([], dtype=dtype)
        return np.concatenate(non_empty).astype(dtype, copy=False)

    return MatchOutput(
        matched_veh_idx=_concat(matched_veh_idx_parts, np.int64),
        matched_resim_idx=_concat(matched_resim_idx_parts, np.int64),
        orphan_veh_idx=_concat(orphan_veh_idx_parts, np.int64),
        orphan_resim_idx=_concat(orphan_resim_idx_parts, np.int64),
        dropped_scan_indices=bundle.input_only_scan_indices.astype(np.int64),
        distance_values=_concat(dist_parts, np.float64),
        common_scan_indices=bundle.common_scan_indices.astype(np.int64),
        input_only_scan_indices=bundle.input_only_scan_indices.astype(np.int64),
        output_only_scan_indices=bundle.output_only_scan_indices.astype(np.int64),
        scan_match_percentages=np.asarray(scan_match_pct, dtype=np.float64),
        total_veh_points=int(bundle.veh.points.shape[0]),
        total_resim_points=int(bundle.resim.points.shape[0]),
    )


def _timestamp_jitter_from_arrays(scan_index: np.ndarray, timestamps: np.ndarray) -> float:
    if scan_index.size == 0 or timestamps.size == 0:
        return float("nan")
    valid = np.isfinite(timestamps)
    if not np.any(valid):
        return float("nan")
    scans = scan_index[valid]
    times = timestamps[valid].astype(np.float64, copy=False)
    unique, inverse = np.unique(scans, return_inverse=True)
    if unique.size < 3:
        return float("nan")
    sums = np.bincount(inverse, weights=times)
    counts = np.bincount(inverse)
    means = np.divide(sums, counts, out=np.full_like(sums, np.nan, dtype=np.float64), where=counts > 0)
    delta = np.diff(means)
    if delta.size == 0:
        return float("nan")
    return float(np.nanstd(delta))


def compute_loaded_kpis(bundle: RadarComparisonBundle, match: MatchOutput) -> Dict[str, float]:
    def _combine_seconds_fractional(seconds: np.ndarray, fractional: np.ndarray) -> np.ndarray:
        if seconds.size == 0 or fractional.size == 0:
            return np.array([], dtype=np.float64)
        if not np.isfinite(fractional).any():
            return np.array([], dtype=np.float64)
        frac = fractional / 1e9 if np.nanmax(np.abs(fractional[np.isfinite(fractional)])) > 1e4 else fractional
        return seconds + frac

    total_veh = float(match.total_veh_points)
    total_resim = float(match.total_resim_points)
    matched = float(match.matched_veh_idx.size)

    # Use only points from common scan indices for recall/precision/F1
    if match.common_scan_indices.size > 0:
        common_veh = float(sum(
            bundle.scan_input_counts.get(int(s), 0) for s in match.common_scan_indices
        ))
        common_resim = float(sum(
            bundle.scan_output_counts.get(int(s), 0) for s in match.common_scan_indices
        ))
    else:
        common_veh = total_veh
        common_resim = total_resim

    if match.matched_veh_idx.size > 0:
        veh_points = bundle.veh.points[match.matched_veh_idx]
        resim_points = bundle.resim.points[match.matched_resim_idx]
        delta_ran = veh_points[:, 0] - resim_points[:, 0]
        delta_vel = veh_points[:, 1] - resim_points[:, 1]
    else:
        delta_ran = np.array([], dtype=np.float64)
        delta_vel = np.array([], dtype=np.float64)

    abs_dran = np.abs(delta_ran)
    abs_dvel = np.abs(delta_vel)
    strict_mask = (abs_dran < 0.1) & (abs_dvel < 0.1)
    strict_count = float(np.count_nonzero(strict_mask))

    p95_dran = _safe_percentile(abs_dran, 95)
    p99_dvel = _safe_percentile(abs_dvel, 99)
    recall = matched / common_veh if common_veh > 0 else float("nan")
    precision = matched / common_resim if common_resim > 0 else float("nan")
    if not math.isnan(recall) and not math.isnan(precision) and (recall + precision) > 0:
        f1_score = 2.0 * recall * precision / (recall + precision)
    else:
        f1_score = float("nan")
    scan_index_match_pct = _scan_index_match_pct(match)

    veh_timestamps = bundle.veh.optional.get("hdrTimestamp_Sec", np.array([], dtype=np.float64))
    veh_frac = bundle.veh.optional.get("hdrTimestamp_fractionalSec", np.array([], dtype=np.float64))
    resim_timestamps = bundle.resim.optional.get("hdrTimestamp_Sec", np.array([], dtype=np.float64))
    resim_frac = bundle.resim.optional.get("hdrTimestamp_fractionalSec", np.array([], dtype=np.float64))
    veh_time = _combine_seconds_fractional(veh_timestamps, veh_frac)
    resim_time = _combine_seconds_fractional(resim_timestamps, resim_frac)

    return {
        "total_veh_points": total_veh,
        "total_resim_points": total_resim,
        "matched_points": matched,
        "unmatched_veh_points": float(match.orphan_veh_idx.size),
        "unmatched_resim_points": float(match.orphan_resim_idx.size),
        "matched_scan_index_count": float(match.common_scan_indices.size),
        "common_scan_count": float(match.common_scan_indices.size),
        "input_only_scan_count": float(match.input_only_scan_indices.size),
        "output_only_scan_count": float(match.output_only_scan_indices.size),
        "avg_scan_match_pct": scan_index_match_pct,
        "resim_recall": recall,
        "resim_precision": precision,
        "f1_score": f1_score,
        "p95_abs_dran": p95_dran,
        "p99_abs_dvel": p99_dvel,
        "range_bound_flag": float(p95_dran > 0.1) if not math.isnan(p95_dran) else float("nan"),
        "vel_bound_flag": float(p99_dvel > 0.05) if not math.isnan(p99_dvel) else float("nan"),
        "veh_timestamp_jitter": _timestamp_jitter_from_arrays(bundle.veh.scan_index, veh_time),
        "resim_timestamp_jitter": _timestamp_jitter_from_arrays(bundle.resim.scan_index, resim_time),
        "pcfs": strict_count / total_veh if total_veh > 0 else float("nan"),
        "dropped_frames": float(match.dropped_scan_indices.size),
        "mean_gate_distance": float(np.nanmean(match.distance_values)) if match.distance_values.size > 0 else float("nan"),
    }


def _scan_mean(scan_index: np.ndarray, values: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
    if scan_index.size == 0 or values.size == 0:
        return np.array([], dtype=np.int64), np.array([], dtype=np.float64)
    valid = np.isfinite(values)
    if not np.any(valid):
        return np.array([], dtype=np.int64), np.array([], dtype=np.float64)
    scans = scan_index[valid]
    vals = values[valid].astype(np.float64, copy=False)
    unique, inverse = np.unique(scans, return_inverse=True)
    sums = np.bincount(inverse, weights=vals)
    counts = np.bincount(inverse)
    means = np.divide(sums, counts, out=np.full_like(sums, np.nan, dtype=np.float64), where=counts > 0)
    return unique.astype(np.int64), means.astype(np.float64)


def _make_orphan_histogram_bundle(bundle: RadarComparisonBundle, match: MatchOutput) -> go.Figure:
    fig = make_subplots(rows=1, cols=2, subplot_titles=("Orphan SNR", "Orphan RCS"))
    veh_snr = bundle.veh.optional.get("snr", np.array([], dtype=np.float64))
    resim_snr = bundle.resim.optional.get("snr", np.array([], dtype=np.float64))
    veh_rcs = bundle.veh.optional.get("rcs", np.array([], dtype=np.float64))
    resim_rcs = bundle.resim.optional.get("rcs", np.array([], dtype=np.float64))
    if veh_snr.size > 0 and match.orphan_veh_idx.size > 0:
        fig.add_trace(go.Histogram(x=veh_snr[match.orphan_veh_idx], name="VEH orphan", opacity=0.6), row=1, col=1)
    if resim_snr.size > 0 and match.orphan_resim_idx.size > 0:
        fig.add_trace(go.Histogram(x=resim_snr[match.orphan_resim_idx], name="RESIM orphan", opacity=0.6), row=1, col=1)
    if veh_rcs.size > 0 and match.orphan_veh_idx.size > 0:
        fig.add_trace(go.Histogram(x=veh_rcs[match.orphan_veh_idx], name="VEH orphan", opacity=0.6, showlegend=False), row=1, col=2)
    if resim_rcs.size > 0 and match.orphan_resim_idx.size > 0:
        fig.add_trace(go.Histogram(x=resim_rcs[match.orphan_resim_idx], name="RESIM orphan", opacity=0.6, showlegend=False), row=1, col=2)
    fig.update_layout(title="Orphan Analysis", barmode="overlay")
    return fig


def _make_feature_confusion_figure_bundle(bundle: RadarComparisonBundle, match: MatchOutput) -> go.Figure:
    if match.matched_veh_idx.size == 0:
        fig = go.Figure()
        fig.update_layout(title="Additional Feature Confusion Matrices")
        return fig

    veh_m = bundle.veh.points[match.matched_veh_idx]
    res_m = bundle.resim.points[match.matched_resim_idx]
    features: List[Tuple[str, np.ndarray, np.ndarray, List[str]]] = []

    range_edges = _quantile_edges(np.concatenate([veh_m[:, 0], res_m[:, 0]]), [0.0, 0.33, 0.66, 1.0])
    if range_edges is not None:
        features.append(("Range Bins", _digitize_bins(veh_m[:, 0], range_edges), _digitize_bins(res_m[:, 0], range_edges), _class_labels("R", 3)))

    az_edges = _quantile_edges(np.concatenate([veh_m[:, 2], res_m[:, 2]]), [0.0, 0.33, 0.66, 1.0])
    if az_edges is not None:
        features.append(("Azimuth Bins", _digitize_bins(veh_m[:, 2], az_edges), _digitize_bins(res_m[:, 2], az_edges), _class_labels("Az", 3)))

    veh_vel_cat = np.full(veh_m.shape[0], 1, dtype=np.int64)
    res_vel_cat = np.full(res_m.shape[0], 1, dtype=np.int64)
    veh_vel_cat[veh_m[:, 1] < -0.1] = 0
    veh_vel_cat[veh_m[:, 1] > 0.1] = 2
    res_vel_cat[res_m[:, 1] < -0.1] = 0
    res_vel_cat[res_m[:, 1] > 0.1] = 2
    features.append(("Velocity State", veh_vel_cat, res_vel_cat, ["Approach", "Static", "Recede"]))

    veh_snr = bundle.veh.optional.get("snr", np.array([], dtype=np.float64))
    resim_snr = bundle.resim.optional.get("snr", np.array([], dtype=np.float64))
    if veh_snr.size > 0 and resim_snr.size > 0:
        veh_snr_m = veh_snr[match.matched_veh_idx]
        resim_snr_m = resim_snr[match.matched_resim_idx]
        snr_edges = _quantile_edges(np.concatenate([veh_snr_m, resim_snr_m]), [0.0, 0.33, 0.66, 1.0])
        if snr_edges is not None:
            features.append(("SNR Bins", _digitize_bins(veh_snr_m, snr_edges), _digitize_bins(resim_snr_m, snr_edges), _class_labels("SNR", 3)))

    veh_rcs = bundle.veh.optional.get("rcs", np.array([], dtype=np.float64))
    resim_rcs = bundle.resim.optional.get("rcs", np.array([], dtype=np.float64))
    if veh_rcs.size > 0 and resim_rcs.size > 0:
        veh_rcs_m = veh_rcs[match.matched_veh_idx]
        resim_rcs_m = resim_rcs[match.matched_resim_idx]
        rcs_edges = _quantile_edges(np.concatenate([veh_rcs_m, resim_rcs_m]), [0.0, 0.33, 0.66, 1.0])
        if rcs_edges is not None:
            features.append(("RCS Bins", _digitize_bins(veh_rcs_m, rcs_edges), _digitize_bins(resim_rcs_m, rcs_edges), _class_labels("RCS", 3)))

    if not features:
        fig = go.Figure()
        fig.update_layout(title="Additional Feature Confusion Matrices")
        return fig

    cols = 3
    rows = math.ceil(len(features) / cols)
    fig = make_subplots(rows=rows, cols=cols, subplot_titles=[item[0] for item in features])
    for idx, (title, veh_cat, resim_cat, labels) in enumerate(features, start=1):
        row = (idx - 1) // cols + 1
        col = (idx - 1) % cols + 1
        z = _confusion_from_categories(veh_cat, resim_cat, len(labels))
        fig.add_trace(
            go.Heatmap(
                z=z,
                x=[f"RESIM {label}" for label in labels],
                y=[f"VEH {label}" for label in labels],
                text=z,
                texttemplate="%{text}",
                showscale=False,
                colorscale="Blues",
            ),
            row=row,
            col=col,
        )
    fig.update_layout(title="Additional Feature Confusion Matrices")
    return fig


def _make_flag_divergence_figure_bundle(bundle: RadarComparisonBundle, match: MatchOutput) -> go.Figure:
    flags = ["f_superres_target", "f_ci_det", "f_bistatic"]
    fig = make_subplots(rows=1, cols=3, subplot_titles=flags)
    for idx, flag in enumerate(flags, start=1):
        veh_flag = bundle.veh.optional.get(flag, np.array([], dtype=np.float64))
        resim_flag = bundle.resim.optional.get(flag, np.array([], dtype=np.float64))
        if veh_flag.size > 0 and resim_flag.size > 0 and match.matched_veh_idx.size > 0:
            matrix = _flag_confusion(veh_flag[match.matched_veh_idx], resim_flag[match.matched_resim_idx])
            fig.add_trace(
                go.Heatmap(
                    z=matrix,
                    x=["RESIM False", "RESIM True"],
                    y=["VEH False", "VEH True"],
                    showscale=False,
                    text=matrix,
                    texttemplate="%{text}",
                    colorscale="Blues",
                ),
                row=1,
                col=idx,
            )
        else:
            fig.add_trace(go.Heatmap(z=[[0, 0], [0, 0]], showscale=False, colorscale="Blues"), row=1, col=idx)
    fig.update_layout(title="Feature Flag Divergence Matrices")
    return fig


def _make_boresight_figure_bundle(bundle: RadarComparisonBundle) -> go.Figure:
    fig = go.Figure()
    veh_bore = bundle.veh.optional.get("vacs_boresight_az_estimated", np.array([], dtype=np.float64))
    resim_bore = bundle.resim.optional.get("vacs_boresight_az_estimated", np.array([], dtype=np.float64))
    veh_scan, veh_mean = _scan_mean(bundle.veh.scan_index, veh_bore)
    resim_scan, resim_mean = _scan_mean(bundle.resim.scan_index, resim_bore)
    if veh_scan.size > 0:
        fig.add_trace(go.Scatter(x=veh_scan, y=veh_mean, mode="lines", name="VEH boresight az"))
    if resim_scan.size > 0:
        fig.add_trace(go.Scatter(x=resim_scan, y=resim_mean, mode="lines", name="RESIM boresight az"))
    fig.update_layout(title="Boresight Alignment Tracking", xaxis_title="scan_index", yaxis_title="vacs_boresight_az_estimated")
    return fig


def _make_spatial_error_heatmap(
    azimuth: np.ndarray,
    ran: np.ndarray,
    value: np.ndarray,
    title: str,
    colorbar: str,
    nbins_az: int = 48,
    nbins_ran: int = 48,
) -> go.Figure:
    if azimuth.size == 0:
        fig = go.Figure()
        fig.update_layout(title=title)
        return fig

    az_min, az_max = np.nanmin(azimuth), np.nanmax(azimuth)
    r_min, r_max = np.nanmin(ran), np.nanmax(ran)

    az_edges = np.linspace(az_min, az_max, nbins_az + 1)
    r_edges = np.linspace(r_min, r_max, nbins_ran + 1)
    az_bin = np.clip(np.digitize(azimuth, az_edges) - 1, 0, nbins_az - 1)
    r_bin = np.clip(np.digitize(ran, r_edges) - 1, 0, nbins_ran - 1)
    flat = az_bin * nbins_ran + r_bin

    sum_vals = np.bincount(flat, weights=value, minlength=nbins_az * nbins_ran)
    cnt_vals = np.bincount(flat, minlength=nbins_az * nbins_ran)
    mean_vals = np.divide(sum_vals, cnt_vals, out=np.full_like(sum_vals, np.nan, dtype=np.float64), where=cnt_vals > 0)
    z = mean_vals.reshape((nbins_az, nbins_ran)).T

    az_centers = (az_edges[:-1] + az_edges[1:]) / 2.0
    r_centers = (r_edges[:-1] + r_edges[1:]) / 2.0
    fig = go.Figure(
        data=go.Heatmap(
            x=az_centers,
            y=r_centers,
            z=z,
            colorscale="Turbo",
            colorbar={"title": colorbar},
        )
    )
    fig.update_layout(title=title, xaxis_title="azimuth", yaxis_title="ran")
    return fig


def _make_cdf_figure(abs_dran: np.ndarray, abs_dvel: np.ndarray) -> go.Figure:
    def _cdf(vals: np.ndarray) -> Tuple[np.ndarray, np.ndarray]:
        if vals.size == 0:
            return np.array([]), np.array([])
        xs = np.sort(vals)
        ys = np.arange(1, xs.size + 1, dtype=np.float64) / xs.size
        return xs, ys

    x_r, y_r = _cdf(abs_dran)
    x_v, y_v = _cdf(abs_dvel)
    fig = go.Figure()
    fig.add_trace(go.Scatter(x=x_r, y=y_r, mode="lines", name="|Δran| CDF"))
    fig.add_trace(go.Scatter(x=x_v, y=y_v, mode="lines", name="|Δvel| CDF"))
    fig.update_layout(title="Error CDF", xaxis_title="Absolute Error", yaxis_title="Cumulative Probability")
    return fig


def _make_orphan_histogram(veh_df: pd.DataFrame, resim_df: pd.DataFrame, match: MatchOutput) -> go.Figure:
    fig = make_subplots(rows=1, cols=2, subplot_titles=("Orphan SNR", "Orphan RCS"))

    veh_orphan = veh_df.iloc[match.orphan_veh_idx] if match.orphan_veh_idx.size else pd.DataFrame()
    resim_orphan = resim_df.iloc[match.orphan_resim_idx] if match.orphan_resim_idx.size else pd.DataFrame()

    if "snr" in veh_orphan.columns:
        fig.add_trace(go.Histogram(x=veh_orphan["snr"], name="VEH orphan", opacity=0.6), row=1, col=1)
    if "snr" in resim_orphan.columns:
        fig.add_trace(go.Histogram(x=resim_orphan["snr"], name="RESIM orphan", opacity=0.6), row=1, col=1)
    if "rcs" in veh_orphan.columns:
        fig.add_trace(go.Histogram(x=veh_orphan["rcs"], name="VEH orphan", opacity=0.6, showlegend=False), row=1, col=2)
    if "rcs" in resim_orphan.columns:
        fig.add_trace(go.Histogram(x=resim_orphan["rcs"], name="RESIM orphan", opacity=0.6, showlegend=False), row=1, col=2)

    fig.update_layout(title="Orphan Analysis", barmode="overlay")
    return fig


def _flag_confusion(veh_flags: np.ndarray, resim_flags: np.ndarray) -> np.ndarray:
    mat = np.zeros((2, 2), dtype=np.int64)
    veh_b = veh_flags.astype(bool)
    res_b = resim_flags.astype(bool)
    mat[0, 0] = np.count_nonzero((~veh_b) & (~res_b))
    mat[0, 1] = np.count_nonzero((~veh_b) & res_b)
    mat[1, 0] = np.count_nonzero(veh_b & (~res_b))
    mat[1, 1] = np.count_nonzero(veh_b & res_b)
    return mat


def _quantile_edges(values: np.ndarray, q: Sequence[float]) -> Optional[np.ndarray]:
    vals = np.asarray(values, dtype=np.float64)
    vals = vals[np.isfinite(vals)]
    if vals.size < 3:
        return None
    edges = np.quantile(vals, q)
    if np.unique(edges).size < 4:
        vmin = float(np.nanmin(vals))
        vmax = float(np.nanmax(vals))
        if not np.isfinite(vmin) or not np.isfinite(vmax) or vmax <= vmin:
            return None
        return np.array([vmin, vmin + (vmax - vmin) / 3.0, vmin + 2.0 * (vmax - vmin) / 3.0, vmax])
    return np.asarray(edges, dtype=np.float64)


def _digitize_bins(values: np.ndarray, edges: np.ndarray) -> np.ndarray:
    v = np.asarray(values, dtype=np.float64)
    bins = np.digitize(v, edges[1:-1], right=False)
    return np.clip(bins, 0, len(edges) - 2).astype(np.int64)


def _confusion_from_categories(veh_cat: np.ndarray, resim_cat: np.ndarray, n_classes: int) -> np.ndarray:
    mat = np.zeros((n_classes, n_classes), dtype=np.int64)
    for i in range(n_classes):
        for j in range(n_classes):
            mat[i, j] = np.count_nonzero((veh_cat == i) & (resim_cat == j))
    return mat


def _class_labels(prefix: str, n_classes: int) -> List[str]:
    names = ["Low", "Mid", "High", "Very High", "Top"]
    if n_classes <= len(names):
        return [f"{prefix} {names[i]}" for i in range(n_classes)]
    return [f"{prefix} C{i}" for i in range(n_classes)]


def _make_feature_confusion_figure(veh_df: pd.DataFrame, resim_df: pd.DataFrame, match: MatchOutput) -> go.Figure:
    if match.matched_veh_idx.size == 0:
        fig = go.Figure()
        fig.update_layout(title="Additional Feature Confusion Matrices")
        return fig

    veh_m = veh_df.iloc[match.matched_veh_idx].reset_index(drop=True)
    res_m = resim_df.iloc[match.matched_resim_idx].reset_index(drop=True)

    features: List[Tuple[str, str, Optional[np.ndarray], Optional[np.ndarray], Optional[List[str]]]] = []

    range_edges = _quantile_edges(np.concatenate([veh_m["ran"].to_numpy(), res_m["ran"].to_numpy()]), [0.0, 0.33, 0.66, 1.0])
    if range_edges is not None:
        features.append((
            "Range Bins",
            "ran",
            _digitize_bins(veh_m["ran"].to_numpy(), range_edges),
            _digitize_bins(res_m["ran"].to_numpy(), range_edges),
            _class_labels("R", 3),
        ))

    az_edges = _quantile_edges(np.concatenate([veh_m["azimuth"].to_numpy(), res_m["azimuth"].to_numpy()]), [0.0, 0.33, 0.66, 1.0])
    if az_edges is not None:
        features.append((
            "Azimuth Bins",
            "azimuth",
            _digitize_bins(veh_m["azimuth"].to_numpy(), az_edges),
            _digitize_bins(res_m["azimuth"].to_numpy(), az_edges),
            _class_labels("Az", 3),
        ))

    vel = veh_m["vel"].to_numpy(dtype=np.float64)
    rvel = res_m["vel"].to_numpy(dtype=np.float64)
    veh_vel_cat = np.full(vel.shape[0], 1, dtype=np.int64)
    res_vel_cat = np.full(rvel.shape[0], 1, dtype=np.int64)
    veh_vel_cat[vel < -0.1] = 0
    veh_vel_cat[vel > 0.1] = 2
    res_vel_cat[rvel < -0.1] = 0
    res_vel_cat[rvel > 0.1] = 2
    features.append(("Velocity State", "vel", veh_vel_cat, res_vel_cat, ["Approach", "Static", "Recede"]))

    if "snr" in veh_m.columns and "snr" in res_m.columns:
        snr_edges = _quantile_edges(np.concatenate([veh_m["snr"].to_numpy(), res_m["snr"].to_numpy()]), [0.0, 0.33, 0.66, 1.0])
        if snr_edges is not None:
            features.append((
                "SNR Bins",
                "snr",
                _digitize_bins(veh_m["snr"].to_numpy(), snr_edges),
                _digitize_bins(res_m["snr"].to_numpy(), snr_edges),
                _class_labels("SNR", 3),
            ))

    if "rcs" in veh_m.columns and "rcs" in res_m.columns:
        rcs_edges = _quantile_edges(np.concatenate([veh_m["rcs"].to_numpy(), res_m["rcs"].to_numpy()]), [0.0, 0.33, 0.66, 1.0])
        if rcs_edges is not None:
            features.append((
                "RCS Bins",
                "rcs",
                _digitize_bins(veh_m["rcs"].to_numpy(), rcs_edges),
                _digitize_bins(res_m["rcs"].to_numpy(), rcs_edges),
                _class_labels("RCS", 3),
            ))

        # Out-of-RCS (tail) confusion: values outside [q05, q95] treated as outliers.
        rcs_all = np.concatenate([veh_m["rcs"].to_numpy(dtype=np.float64), res_m["rcs"].to_numpy(dtype=np.float64)])
        rcs_all = rcs_all[np.isfinite(rcs_all)]
        if rcs_all.size > 3:
            q05 = float(np.quantile(rcs_all, 0.05))
            q95 = float(np.quantile(rcs_all, 0.95))
            veh_out = ((veh_m["rcs"].to_numpy(dtype=np.float64) < q05) | (veh_m["rcs"].to_numpy(dtype=np.float64) > q95)).astype(np.int64)
            res_out = ((res_m["rcs"].to_numpy(dtype=np.float64) < q05) | (res_m["rcs"].to_numpy(dtype=np.float64) > q95)).astype(np.int64)
            features.append(("RCS Out-of-Band", "rcs_out", veh_out, res_out, ["In-Band", "Out-of-Band"]))

    if not features:
        fig = go.Figure()
        fig.update_layout(title="Additional Feature Confusion Matrices")
        return fig

    cols = 3
    rows = math.ceil(len(features) / cols)
    subplot_titles = [f[0] for f in features]
    fig = make_subplots(rows=rows, cols=cols, subplot_titles=subplot_titles)

    for idx, (title, _name, veh_cat, res_cat, labels) in enumerate(features, start=1):
        row = (idx - 1) // cols + 1
        col = (idx - 1) % cols + 1
        if veh_cat is None or res_cat is None or labels is None:
            z = np.zeros((2, 2), dtype=np.int64)
            labels = ["Class0", "Class1"]
        else:
            n_classes = len(labels)
            z = _confusion_from_categories(veh_cat, res_cat, n_classes)

        fig.add_trace(
            go.Heatmap(
                z=z,
                x=[f"RESIM {x}" for x in labels],
                y=[f"VEH {x}" for x in labels],
                text=z,
                texttemplate="%{text}",
                showscale=False,
                colorscale="Blues",
            ),
            row=row,
            col=col,
        )

    fig.update_layout(title="Additional Feature Confusion Matrices")
    return fig


def _make_flag_divergence_figure(veh_df: pd.DataFrame, resim_df: pd.DataFrame, match: MatchOutput) -> go.Figure:
    flags = ["f_superres_target", "f_ci_det", "f_bistatic"]
    fig = make_subplots(rows=1, cols=3, subplot_titles=flags)
    for i, flag in enumerate(flags, start=1):
        if flag in veh_df.columns and flag in resim_df.columns and match.matched_veh_idx.size > 0:
            veh_vals = veh_df.loc[match.matched_veh_idx, flag].to_numpy()
            res_vals = resim_df.loc[match.matched_resim_idx, flag].to_numpy()
            matrix = _flag_confusion(veh_vals, res_vals)
            fig.add_trace(
                go.Heatmap(
                    z=matrix,
                    x=["RESIM False", "RESIM True"],
                    y=["VEH False", "VEH True"],
                    showscale=False,
                    text=matrix,
                    texttemplate="%{text}",
                    colorscale="Blues",
                ),
                row=1,
                col=i,
            )
        else:
            fig.add_trace(go.Heatmap(z=[[0, 0], [0, 0]], showscale=False, colorscale="Blues"), row=1, col=i)
    fig.update_layout(title="Feature Flag Divergence Matrices")
    return fig


def _make_boresight_figure(veh_df: pd.DataFrame, resim_df: pd.DataFrame) -> go.Figure:
    fig = go.Figure()
    if "vacs_boresight_az_estimated" in veh_df.columns:
        veh_line = veh_df.groupby("scan_index", sort=True)["vacs_boresight_az_estimated"].mean().reset_index()
        fig.add_trace(
            go.Scatter(
                x=veh_line["scan_index"],
                y=veh_line["vacs_boresight_az_estimated"],
                mode="lines",
                name="VEH boresight az",
            )
        )
    if "vacs_boresight_az_estimated" in resim_df.columns:
        resim_line = resim_df.groupby("scan_index", sort=True)["vacs_boresight_az_estimated"].mean().reset_index()
        fig.add_trace(
            go.Scatter(
                x=resim_line["scan_index"],
                y=resim_line["vacs_boresight_az_estimated"],
                mode="lines",
                name="RESIM boresight az",
            )
        )
    fig.update_layout(title="Boresight Alignment Tracking", xaxis_title="scan_index", yaxis_title="vacs_boresight_az_estimated")
    return fig


def build_report_html(
    pair: FilePair,
    veh: LoadedRadar,
    resim: LoadedRadar,
    bundle: RadarComparisonBundle,
    match: MatchOutput,
    kpis: Dict[str, float],
) -> str:
    if match.matched_veh_idx.size > 0:
        veh_matched = bundle.veh.points[match.matched_veh_idx]
        resim_matched = bundle.resim.points[match.matched_resim_idx]
        delta_ran = veh_matched[:, 0] - resim_matched[:, 0]
        delta_vel = veh_matched[:, 1] - resim_matched[:, 1]
        az = veh_matched[:, 2]
        ran = veh_matched[:, 0]
    else:
        delta_ran = np.array([], dtype=np.float64)
        delta_vel = np.array([], dtype=np.float64)
        az = np.array([], dtype=np.float64)
        ran = np.array([], dtype=np.float64)

    fig_hex_ran = _make_spatial_error_heatmap(az, ran, np.abs(delta_ran), "Spatial Error Sectors (mean |Δran|)", "mean |Δran|")
    fig_hex_vel = _make_spatial_error_heatmap(az, ran, np.abs(delta_vel), "Spatial Error Sectors (mean |Δvel|)", "mean |Δvel|")
    fig_cdf = _make_cdf_figure(np.abs(delta_ran), np.abs(delta_vel))
    fig_orphan = _make_orphan_histogram_bundle(bundle, match)
    fig_flag = _make_flag_divergence_figure_bundle(bundle, match)
    fig_boresight = _make_boresight_figure_bundle(bundle)
    fig_feature_conf = _make_feature_confusion_figure_bundle(bundle, match)

    fig_blocks = [
        fig_hex_ran,
        fig_hex_vel,
        fig_cdf,
        fig_orphan,
        fig_flag,
        fig_boresight,
        fig_feature_conf,
    ]

    divs: List[str] = []
    for idx, figure in enumerate(fig_blocks):
        divs.append(
            figure.to_html(
                full_html=False,
                include_plotlyjs="cdn" if idx == 0 else False,
                config={
                    "responsive": True,
                    "displayModeBar": False,
                    "scrollZoom": False,
                },
            )
        )

    metadata_rows = []
    all_meta = {
        "VEH_DC_version": veh.metadata.get("DC_version", "N/A"),
        "VEH_Tracker_version": veh.metadata.get("Tracker_version", "N/A"),
        "RESIM_DC_version": resim.metadata.get("DC_version", "N/A"),
        "RESIM_Tracker_version": resim.metadata.get("Tracker_version", "N/A"),
    }
    for key, value in all_meta.items():
        metadata_rows.append(f"<tr><td>{key}</td><td>{value}</td></tr>")

    kpi_rows = []
    for key, value in kpis.items():
        if isinstance(value, float) and not math.isnan(value):
            display = f"{value:.6f}"
        else:
            display = str(value)
        kpi_rows.append(f"<tr><td>{key}</td><td>{display}</td></tr>")

    summary_cards = [
        ("VEH Total", f"{int(kpis.get('total_veh_points', 0))}"),
        ("RESIM Total", f"{int(kpis.get('total_resim_points', 0))}"),
        ("Matched", f"{int(kpis.get('matched_points', 0))}"),
        ("VEH Unmatched", f"{int(kpis.get('unmatched_veh_points', 0))}"),
        ("RESIM Unmatched", f"{int(kpis.get('unmatched_resim_points', 0))}"),
        ("Scanindex Match", f"{kpis.get('avg_scan_match_pct', float('nan')):.2f}%" if not math.isnan(kpis.get('avg_scan_match_pct', float('nan'))) else "NA"),
        ("Matched Scan Indices", f"{int(kpis.get('matched_scan_index_count', 0))}"),
        ("Input-only Scans", f"{int(kpis.get('input_only_scan_count', 0))}"),
        ("Output-only Scans", f"{int(kpis.get('output_only_scan_count', 0))}"),
    ]
    summary_cards_html = "".join(
        f'<div class="stat-card"><div class="stat-label">{label}</div><div class="stat-value">{value}</div></div>'
        for label, value in summary_cards
    )

    return f"""
<!doctype html>
<html lang=\"en\">
<head>
  <meta charset=\"utf-8\" />
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />
  <title>SIL Radar Validation Report - {pair.sensor}</title>
  <style>
        :root {{
            color-scheme: light;
            --page-bg: #eef4f8;
            --panel-bg: rgba(255, 255, 255, 0.92);
            --panel-border: #d8e2ec;
            --text-main: #153047;
            --text-muted: #5f7286;
            --accent: #126b90;
            --accent-soft: #d9edf5;
            --shadow: 0 10px 24px rgba(24, 55, 83, 0.08);
        }}
        * {{ box-sizing: border-box; }}
        body {{
            font-family: "Segoe UI", Arial, sans-serif;
            margin: 0;
            padding: 24px;
            background: linear-gradient(180deg, #f7fbfd 0%, var(--page-bg) 100%);
            color: var(--text-main);
        }}
        h1, h2, h3 {{ color: var(--text-main); margin-top: 0; }}
        p {{ margin: 0 0 8px; color: var(--text-muted); }}
        .page {{ max-width: 1440px; margin: 0 auto; }}
        .hero {{ margin-bottom: 18px; }}
        .hero h1 {{ margin-bottom: 6px; font-size: 2rem; }}
        .hero p {{ font-size: 1rem; }}
        .panel {{
            background: var(--panel-bg);
            border: 1px solid var(--panel-border);
            border-radius: 18px;
            padding: 18px;
            margin-bottom: 18px;
            box-shadow: var(--shadow);
            content-visibility: auto;
            contain-intrinsic-size: 480px;
        }}
        .panel-head {{ display: flex; justify-content: space-between; gap: 12px; align-items: baseline; margin-bottom: 10px; }}
        .stat-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 12px;
            margin-bottom: 18px;
        }}
        .stat-card {{
            padding: 14px;
            border-radius: 14px;
            background: linear-gradient(180deg, #ffffff 0%, #f5fbff 100%);
            border: 1px solid #d9e6ef;
            min-height: 94px;
            animation: rise-in 180ms ease-out both;
        }}
        .stat-label {{ font-size: 0.8rem; font-weight: 700; letter-spacing: 0.04em; text-transform: uppercase; color: var(--text-muted); }}
        .stat-value {{ margin-top: 8px; font-size: 1.55rem; font-weight: 700; color: var(--accent); }}
        .meta-grid {{ display: grid; grid-template-columns: 1.1fr 0.9fr; gap: 14px; }}
        table {{ border-collapse: collapse; width: 100%; background: #fff; border-radius: 12px; overflow: hidden; }}
        th, td {{ border: 1px solid #dce5ee; padding: 8px 10px; text-align: left; }}
        th {{ background: #edf5fa; color: var(--text-main); }}
        .plot-panel {{ padding-top: 14px; }}
        .plot-panel .js-plotly-plot {{ width: 100% !important; }}
        @media (max-width: 900px) {{
            body {{ padding: 14px; }}
            .meta-grid {{ grid-template-columns: 1fr; }}
        }}
        @keyframes rise-in {{
            from {{ opacity: 0; transform: translateY(6px); }}
            to {{ opacity: 1; transform: translateY(0); }}
        }}
  </style>
</head>
<body>
    <div class=\"page\">
        <div class=\"hero\">
            <h1>SIL Radar Validation Report</h1>
            <p>{pair.sensor} · {pair.base_key}</p>
        </div>

        <div class=\"panel\">
            <div class=\"panel-head\">
                <h2>Run Summary</h2>
                <span>{pair.veh_path.name} vs {pair.resim_path.name}</span>
            </div>
            <div class=\"stat-grid\">{summary_cards_html}</div>
        </div>

        <div class=\"meta-grid\">
            <div class=\"panel\">
                <h2>KPI Dashboard</h2>
                <table><tbody>{''.join(kpi_rows)}</tbody></table>
            </div>
            <div class=\"panel\">
                <h2>Metadata</h2>
                <table><tbody>{''.join(metadata_rows)}</tbody></table>
            </div>
        </div>

        <div class=\"panel plot-panel\"><h2>Spatial Error Hexbins</h2>{divs[0]}{divs[1]}</div>
        <div class=\"panel plot-panel\"><h2>Error CDFs</h2>{divs[2]}</div>
        <div class=\"panel plot-panel\"><h2>Orphan Analysis</h2>{divs[3]}</div>
        <div class=\"panel plot-panel\"><h2>Feature Flag Divergence</h2>{divs[4]}</div>
        <div class=\"panel plot-panel\"><h2>Boresight Alignment Tracking</h2>{divs[5]}</div>
        <div class=\"panel plot-panel\"><h2>Additional Feature Confusion Matrices</h2>{divs[6]}</div>
    </div>
</body>
</html>
"""


def process_pair(pair: FilePair, output_dir: Path, gate: float, metric: str, max_sensors: int = 0) -> List[Path]:
    veh_sensors = sensors_in_file(pair.veh_path)
    resim_sensors = sensors_in_file(pair.resim_path)
    sensors = sorted(set(veh_sensors) & set(resim_sensors))
    if not sensors:
        sensors = [pair.sensor] if pair.sensor != "UNKNOWN" else ["UNKNOWN"]

    if max_sensors > 0:
        sensors = sensors[:max_sensors]

    output_dir.mkdir(parents=True, exist_ok=True)
    outputs: List[Path] = []

    for sensor in sensors:
        phase_start = time.perf_counter()
        veh = load_radar_hdf(pair.veh_path, sensor=sensor, build_frame=False)
        resim = load_radar_hdf(pair.resim_path, sensor=sensor, build_frame=False)
        load_elapsed = time.perf_counter() - phase_start

        phase_start = time.perf_counter()
        bundle = _build_comparison_bundle(veh, resim)
        match = match_loaded_radars(bundle, metric=metric)
        kpis = compute_loaded_kpis(bundle, match)
        compare_elapsed = time.perf_counter() - phase_start

        sensor_pair = FilePair(sensor=sensor, base_key=pair.base_key, veh_path=pair.veh_path, resim_path=pair.resim_path)
        phase_start = time.perf_counter()
        report_html = build_report_html(sensor_pair, veh, resim, bundle, match, kpis)
        html_elapsed = time.perf_counter() - phase_start

        out_name = f"{pair.base_key}_{sensor}_sil_validation_report.html".replace(" ", "_")
        out_path = output_dir / out_name
        out_path.write_text(report_html, encoding="utf-8")
        outputs.append(out_path)
        logger.info(
            "%s: load=%.2fs compare=%.2fs html=%.2fs matched=%d common_scans=%d output=%s",
            sensor,
            load_elapsed,
            compare_elapsed,
            html_elapsed,
            match.matched_veh_idx.size,
            match.common_scan_indices.size,
            out_path.name,
        )

    if not outputs:
        raise ValueError(f"{pair.base_key}: no common sensor groups found in pair files")
    return outputs


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="SIL Radar VEH vs RESIM validation pipeline.")
    parser.add_argument("input_dir", type=Path, help="Directory containing .h5 or .h5.gz files")
    parser.add_argument("--output-dir", type=Path, default=Path("sil_reports"), help="Directory to write HTML reports")
    parser.add_argument("--gate", type=float, default=1.0, help="Gating threshold for assignment distance")
    parser.add_argument(
        "--metric",
        type=str,
        default="euclidean",
        choices=["euclidean", "mahalanobis"],
        help="Distance metric for assignment",
    )
    parser.add_argument("--max-pairs", type=int, default=0, help="Optional cap for processed pairs (0 = all)")
    parser.add_argument("--max-sensors", type=int, default=0, help="Optional cap for sensors per pair (0 = all)")
    return parser.parse_args()


def main() -> int:
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s | %(levelname)s | %(message)s",
    )
    args = parse_args()
    start = time.time()

    if not args.input_dir.exists() or not args.input_dir.is_dir():
        raise FileNotFoundError(f"Input directory does not exist: {args.input_dir}")

    pairs = auto_pair_files(args.input_dir)
    if not pairs:
        print("No valid VEH/RESIM file pairs found.")
        return 1

    if args.max_pairs > 0:
        pairs = pairs[: args.max_pairs]

    print(f"Found {len(pairs)} VEH/RESIM pairs. Running SIL validation...")
    success_count = 0
    for i, pair in enumerate(pairs, start=1):
        try:
            print(f"[{i}/{len(pairs)}] {pair.sensor} | {pair.veh_path.name} -> {pair.resim_path.name}")
            out_paths = process_pair(pair, args.output_dir, gate=args.gate, metric=args.metric, max_sensors=args.max_sensors)
            for out_path in out_paths:
                print(f"  Report: {out_path}")
            success_count += 1
        except Exception as exc:
            logger.exception("Failed processing pair %s", pair.base_key)
            print(f"  FAILED: {exc}")

    elapsed = time.time() - start
    print(f"Completed. Success: {success_count}/{len(pairs)} pairs in {elapsed:.2f}s")
    return 0 if success_count > 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
