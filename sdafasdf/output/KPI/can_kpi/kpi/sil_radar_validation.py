#!/usr/bin/env python3
from __future__ import annotations

import argparse
import gzip
import io
import math
import re
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Mapping, Optional, Sequence, Tuple

import h5py
import numpy as np
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots
from scipy.optimize import linear_sum_assignment
from scipy.spatial.distance import cdist


SENSOR_TOKENS = {"FL", "FR", "RL", "RR", "FC", "RC", "MRR", "SRR"}


@dataclass(frozen=True)
class FilePair:
    sensor: str
    base_key: str
    veh_path: Path
    resim_path: Path


@dataclass
class LoadedRadar:
    sensor: str
    frame: pd.DataFrame
    metadata: Dict[str, str]
    dataset_map: Dict[str, str]


@dataclass
class MatchOutput:
    matched_veh_idx: np.ndarray
    matched_resim_idx: np.ndarray
    orphan_veh_idx: np.ndarray
    orphan_resim_idx: np.ndarray
    dropped_scan_indices: np.ndarray
    distance_values: np.ndarray


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

    if ran_raw is not None and ran_path:
        dataset_map["ran"] = ran_path
    if vel_raw is not None and vel_path:
        dataset_map["vel"] = vel_path
    if az_raw is not None and az_path:
        dataset_map["azimuth"] = az_path

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


def load_radar_hdf(path: Path, sensor: str) -> LoadedRadar:
    with _open_hdf(path) as handle:
        fast = _build_from_af_det_with_rdd_fallback(handle, sensor)
        if fast is not None:
            return fast

    column_candidates: Mapping[str, Sequence[str]] = {
        "scan_index": (
            f"{sensor}/DETECTION_STREAM/Stream_Hdr/scan_index",
            f"{sensor}/RDD_STREAM/Look_Data/scan_index",
            "scan_index",
            "ScanIndex",
            "scanindex",
        ),
        "ran": (f"{sensor}/DETECTION_STREAM/AF_Det/ran", "ran", "range", "Range"),
        "vel": (f"{sensor}/DETECTION_STREAM/AF_Det/vel", "vel", "velocity", "doppler", "range_rate", "RangeRate"),
        "azimuth": (f"{sensor}/DETECTION_STREAM/AF_Det/phi", f"{sensor}/DETECTION_STREAM/AF_Det/theta", "azimuth", "phi", "theta"),
        "snr": ("snr",),
        "rcs": ("rcs",),
        "f_superres_target": ("f_superres_target",),
        "f_ci_det": ("f_ci_det",),
        "f_bistatic": ("f_bistatic",),
        "vacs_boresight_az_estimated": ("vacs_boresight_az_estimated",),
        "timestamp": ("timestamp",),
        "hdrTimestamp_Sec": ("hdrTimestamp_Sec",),
        "hdrTimestamp_fractionalSec": ("hdrTimestamp_fractionalSec",),
    }
    metadata_candidates: Mapping[str, Sequence[str]] = {
        "DC_version": ("DC_version",),
        "Tracker_version": ("Tracker_version",),
        "OCG_version": ("OCG_version",),
        "OLP_version": ("OLP_version",),
        "SFL_version": ("SFL_version",),
        "Created_datetime": ("Created_datetime",),
    }

    with _open_hdf(path) as handle:
        dset_paths = _dataset_paths(handle)
        selected_paths: Dict[str, str] = {}
        raw_columns: Dict[str, np.ndarray] = {}
        required_cols = {"scan_index", "ran", "vel", "azimuth"}

        for col_name, candidates in column_candidates.items():
            picked_path: Optional[str] = None
            picked_data: Optional[np.ndarray] = None
            best_score: Optional[Tuple[int, int, int]] = None
            for candidate in candidates:
                if "/" in candidate and candidate in handle:
                    data = _safe_read_dataset(handle, candidate)
                    if data is not None:
                        if col_name == "scan_index":
                            score = (1 if data.ndim == 1 else 0, int(data.shape[0]) if data.ndim > 0 else 1, -candidate.count("/"))
                        else:
                            width = int(np.prod(data.shape[1:])) if data.ndim > 1 else 1
                            score = (data.ndim, width, -candidate.count("/"))
                        if col_name in required_cols:
                            if best_score is None or score > best_score:
                                best_score = score
                                picked_path = candidate
                                picked_data = data
                        else:
                            picked_path = candidate
                            picked_data = data
                            break
                else:
                    for p in _candidate_dataset_paths(dset_paths, (candidate,)):
                        data = _safe_read_dataset(handle, p)
                        if data is not None:
                            if col_name == "scan_index":
                                score = (1 if data.ndim == 1 else 0, int(data.shape[0]) if data.ndim > 0 else 1, -p.count("/"))
                            else:
                                width = int(np.prod(data.shape[1:])) if data.ndim > 1 else 1
                                score = (data.ndim, width, -p.count("/"))
                            if col_name in required_cols:
                                if best_score is None or score > best_score:
                                    best_score = score
                                    picked_path = p
                                    picked_data = data
                            else:
                                picked_path = p
                                picked_data = data
                                break
                    if col_name not in required_cols and picked_data is not None:
                        break
                if col_name not in required_cols and picked_data is not None:
                    break
            if picked_path is None or picked_data is None:
                continue
            selected_paths[col_name] = picked_path
            raw_columns[col_name] = picked_data

        metadata: Dict[str, str] = {}
        for key, candidates in metadata_candidates.items():
            val1d = None
            for candidate in candidates:
                if "/" in candidate and candidate in handle:
                    val1d = _safe_read_dataset(handle, candidate)
                    if val1d is not None:
                        break
                else:
                    for p in _candidate_dataset_paths(dset_paths, (candidate,)):
                        val1d = _safe_read_dataset(handle, p)
                        if val1d is not None:
                            break
                if val1d is not None:
                    break
            if val1d is None:
                continue
            if val1d.size > 0:
                metadata[key] = str(val1d[0])

    required = ["scan_index", "ran", "vel", "azimuth"]
    missing_required = [r for r in required if r not in raw_columns]
    if missing_required:
        raise ValueError(f"{path.name}: missing required datasets {missing_required}")

    required_ndims = [np.asarray(raw_columns[k]).ndim for k in required]
    if any(nd > 1 for nd in required_ndims[1:]):
        scan_raw = np.asarray(raw_columns["scan_index"])
        ran_raw = np.asarray(raw_columns["ran"])
        vel_raw = np.asarray(raw_columns["vel"])
        az_raw = np.asarray(raw_columns["azimuth"])

        req_arrays = [ran_raw, vel_raw, az_raw]
        n_scans = min(
            *(a.shape[0] for a in req_arrays),
            scan_raw.shape[0] if scan_raw.ndim > 0 else ran_raw.shape[0],
        )
        matrix_cols = [int(np.prod(a.shape[1:])) for a in req_arrays if a.ndim > 1]
        n_cols = min(matrix_cols) if matrix_cols else 1

        def _as_matrix(a: np.ndarray) -> np.ndarray:
            if a.ndim == 1:
                return np.repeat(a[:n_scans, None], n_cols, axis=1)
            b = a[:n_scans]
            if b.ndim > 2:
                b = b.reshape(n_scans, -1)
            if b.ndim == 1:
                return np.repeat(b[:n_scans, None], n_cols, axis=1)
            return b[:, :n_cols]

        ran_mat = _as_matrix(ran_raw)
        vel_mat = _as_matrix(vel_raw)
        az_mat = _as_matrix(az_raw)

        if scan_raw.ndim > 1:
            scan_for_idx = scan_raw[:n_scans].reshape(n_scans, -1)[:, 0]
        else:
            scan_for_idx = scan_raw if scan_raw.ndim > 0 else np.arange(n_scans)
        scan_idx = _as_scan_indices(scan_for_idx, n_scans)

        optional_cols: Dict[str, np.ndarray] = {}
        for key, arr in raw_columns.items():
            if key in required:
                continue
            optional_cols[key] = np.asarray(arr)

        df = _matrix_to_long(
            scan_index=scan_idx,
            ran=ran_mat,
            vel=vel_mat,
            azimuth=az_mat,
            optional_cols=optional_cols,
        )

        for flag in ("f_superres_target", "f_ci_det", "f_bistatic"):
            if flag in df.columns:
                df[flag] = _to_bool(np.asarray(df[flag]))

        if "timestamp" not in df.columns and {"hdrTimestamp_Sec", "hdrTimestamp_fractionalSec"}.issubset(df.columns):
            frac = df["hdrTimestamp_fractionalSec"].astype(np.float64)
            if np.nanmax(np.abs(frac.to_numpy())) > 1e4:
                frac = frac / 1e9
            df["timestamp"] = df["hdrTimestamp_Sec"].astype(np.float64) + frac

        df = df.replace([np.inf, -np.inf], np.nan).dropna(subset=["scan_index", "ran", "vel", "azimuth"]).reset_index(drop=True)
        return LoadedRadar(sensor=sensor, frame=df, metadata=metadata, dataset_map=selected_paths)

    target_length = min(raw_columns[key].shape[0] for key in required)
    trimmed: Dict[str, np.ndarray] = {}
    for key, values in raw_columns.items():
        if values.shape[0] < target_length:
            continue
        trimmed[key] = values[:target_length]

    df = pd.DataFrame(
        {
            "scan_index": np.nan_to_num(trimmed["scan_index"].astype(np.float64), nan=-1).astype(np.int64),
            "ran": trimmed["ran"].astype(np.float64),
            "vel": trimmed["vel"].astype(np.float64),
            "azimuth": trimmed["azimuth"].astype(np.float64),
        }
    )

    optional_numeric = ["snr", "rcs", "vacs_boresight_az_estimated", "timestamp", "hdrTimestamp_Sec", "hdrTimestamp_fractionalSec"]
    for col in optional_numeric:
        if col in trimmed:
            df[col] = trimmed[col].astype(np.float64)

    for flag in ("f_superres_target", "f_ci_det", "f_bistatic"):
        if flag in trimmed:
            df[flag] = _to_bool(trimmed[flag])

    if "timestamp" not in df.columns and {"hdrTimestamp_Sec", "hdrTimestamp_fractionalSec"}.issubset(df.columns):
        df["timestamp"] = df["hdrTimestamp_Sec"] + df["hdrTimestamp_fractionalSec"]

    df = df.replace([np.inf, -np.inf], np.nan).dropna(subset=["scan_index", "ran", "vel", "azimuth"]).reset_index(drop=True)
    return LoadedRadar(sensor=sensor, frame=df, metadata=metadata, dataset_map=selected_paths)


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


def match_points(
    veh_df: pd.DataFrame,
    resim_df: pd.DataFrame,
    gate_threshold: float,
    metric: str,
) -> MatchOutput:
    veh_groups = veh_df.groupby("scan_index").indices
    resim_groups = resim_df.groupby("scan_index").indices

    veh_scans = set(veh_groups.keys())
    resim_scans = set(resim_groups.keys())
    common_scans = sorted(veh_scans & resim_scans)
    dropped_scans = np.array(sorted(veh_scans - resim_scans), dtype=np.int64)

    matched_veh_idx_parts: List[np.ndarray] = []
    matched_resim_idx_parts: List[np.ndarray] = []
    orphan_veh_idx_parts: List[np.ndarray] = []
    orphan_resim_idx_parts: List[np.ndarray] = []
    dist_parts: List[np.ndarray] = []

    veh_xyz = veh_df[["ran", "vel", "azimuth"]].to_numpy(dtype=np.float64, copy=False)
    resim_xyz = resim_df[["ran", "vel", "azimuth"]].to_numpy(dtype=np.float64, copy=False)

    for scan in common_scans:
        veh_idx = np.asarray(veh_groups[scan], dtype=np.int64)
        resim_idx = np.asarray(resim_groups[scan], dtype=np.int64)
        if veh_idx.size == 0 or resim_idx.size == 0:
            continue

        veh_points = veh_xyz[veh_idx]
        resim_points = resim_xyz[resim_idx]
        dist_matrix = _distance_metric(veh_points, resim_points, metric)

        row_pos, col_pos = linear_sum_assignment(dist_matrix)
        assigned_dist = dist_matrix[row_pos, col_pos]
        gate_mask = assigned_dist <= gate_threshold

        matched_rows = row_pos[gate_mask]
        matched_cols = col_pos[gate_mask]
        matched_veh_scan = veh_idx[matched_rows]
        matched_resim_scan = resim_idx[matched_cols]

        matched_veh_idx_parts.append(matched_veh_scan)
        matched_resim_idx_parts.append(matched_resim_scan)
        dist_parts.append(assigned_dist[gate_mask])

        veh_assigned_mask = np.zeros(veh_idx.shape[0], dtype=bool)
        resim_assigned_mask = np.zeros(resim_idx.shape[0], dtype=bool)
        veh_assigned_mask[matched_rows] = True
        resim_assigned_mask[matched_cols] = True

        orphan_veh_idx_parts.append(veh_idx[~veh_assigned_mask])
        orphan_resim_idx_parts.append(resim_idx[~resim_assigned_mask])

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

    return MatchOutput(
        matched_veh_idx=_concat(matched_veh_idx_parts),
        matched_resim_idx=_concat(matched_resim_idx_parts),
        orphan_veh_idx=_concat(orphan_veh_idx_parts),
        orphan_resim_idx=_concat(orphan_resim_idx_parts),
        dropped_scan_indices=dropped_scans,
        distance_values=_concat_float(dist_parts),
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


def compute_kpis(veh_df: pd.DataFrame, resim_df: pd.DataFrame, match: MatchOutput) -> Dict[str, float]:
    total_veh = float(len(veh_df))
    total_resim = float(len(resim_df))
    matched = float(match.matched_veh_idx.size)

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
    recall = matched / total_veh if total_veh > 0 else float("nan")
    precision = matched / total_resim if total_resim > 0 else float("nan")
    if not math.isnan(recall) and not math.isnan(precision) and (recall + precision) > 0:
        f1_score = 2.0 * recall * precision / (recall + precision)
    else:
        f1_score = float("nan")

    return {
        "total_veh_points": total_veh,
        "total_resim_points": total_resim,
        "matched_points": matched,
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
    match: MatchOutput,
    kpis: Dict[str, float],
) -> str:
    if match.matched_veh_idx.size > 0:
        delta_ran = veh.frame.loc[match.matched_veh_idx, "ran"].to_numpy() - resim.frame.loc[match.matched_resim_idx, "ran"].to_numpy()
        delta_vel = veh.frame.loc[match.matched_veh_idx, "vel"].to_numpy() - resim.frame.loc[match.matched_resim_idx, "vel"].to_numpy()
        az = veh.frame.loc[match.matched_veh_idx, "azimuth"].to_numpy()
        ran = veh.frame.loc[match.matched_veh_idx, "ran"].to_numpy()
    else:
        delta_ran = np.array([], dtype=np.float64)
        delta_vel = np.array([], dtype=np.float64)
        az = np.array([], dtype=np.float64)
        ran = np.array([], dtype=np.float64)

    fig_hex_ran = _make_spatial_error_heatmap(az, ran, np.abs(delta_ran), "Spatial Error Sectors (mean |Δran|)", "mean |Δran|")
    fig_hex_vel = _make_spatial_error_heatmap(az, ran, np.abs(delta_vel), "Spatial Error Sectors (mean |Δvel|)", "mean |Δvel|")
    fig_cdf = _make_cdf_figure(np.abs(delta_ran), np.abs(delta_vel))
    fig_orphan = _make_orphan_histogram(veh.frame, resim.frame, match)
    fig_flag = _make_flag_divergence_figure(veh.frame, resim.frame, match)
    fig_boresight = _make_boresight_figure(veh.frame, resim.frame)

    fig_blocks = [
        fig_hex_ran,
        fig_hex_vel,
        fig_cdf,
        fig_orphan,
        fig_flag,
        fig_boresight,
    ]

    divs: List[str] = []
    for idx, figure in enumerate(fig_blocks):
        divs.append(figure.to_html(full_html=False, include_plotlyjs="cdn" if idx == 0 else False))

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

    return f"""
<!doctype html>
<html lang=\"en\">
<head>
  <meta charset=\"utf-8\" />
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />
  <title>SIL Radar Validation Report - {pair.sensor}</title>
  <style>
    body {{ font-family: Arial, sans-serif; margin: 20px; background: #0f172a; color: #e2e8f0; }}
    h1, h2 {{ color: #f8fafc; }}
    .panel {{ background: #111827; border: 1px solid #334155; border-radius: 10px; padding: 14px; margin-bottom: 18px; }}
    table {{ border-collapse: collapse; width: 100%; }}
    th, td {{ border: 1px solid #334155; padding: 8px; text-align: left; }}
    th {{ background: #1f2937; }}
    .meta-grid {{ display: grid; grid-template-columns: 1fr 1fr; gap: 14px; }}
  </style>
</head>
<body>
  <h1>SIL Radar Validation Report</h1>
  <div class=\"panel\">
    <p><b>Sensor:</b> {pair.sensor}</p>
    <p><b>Base Key:</b> {pair.base_key}</p>
    <p><b>VEH file:</b> {pair.veh_path.name}</p>
    <p><b>RESIM file:</b> {pair.resim_path.name}</p>
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

  <div class=\"panel\"><h2>Spatial Error Hexbins</h2>{divs[0]}{divs[1]}</div>
  <div class=\"panel\"><h2>Error CDFs</h2>{divs[2]}</div>
  <div class=\"panel\"><h2>Orphan Analysis</h2>{divs[3]}</div>
  <div class=\"panel\"><h2>Feature Flag Divergence</h2>{divs[4]}</div>
  <div class=\"panel\"><h2>Boresight Alignment Tracking</h2>{divs[5]}</div>
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
        veh = load_radar_hdf(pair.veh_path, sensor=sensor)
        resim = load_radar_hdf(pair.resim_path, sensor=sensor)

        match = match_points(veh.frame, resim.frame, gate_threshold=gate, metric=metric)
        kpis = compute_kpis(veh.frame, resim.frame, match)
        sensor_pair = FilePair(sensor=sensor, base_key=pair.base_key, veh_path=pair.veh_path, resim_path=pair.resim_path)
        report_html = build_report_html(sensor_pair, veh, resim, match, kpis)

        out_name = f"{pair.base_key}_{sensor}_sil_validation_report.html".replace(" ", "_")
        out_path = output_dir / out_name
        out_path.write_text(report_html, encoding="utf-8")
        outputs.append(out_path)

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
            print(f"[{i}/{len(pairs)}] {pair.sensor} :: {pair.veh_path.name}  <->  {pair.resim_path.name}")
            out_paths = process_pair(pair, args.output_dir, gate=args.gate, metric=args.metric, max_sensors=args.max_sensors)
            for out_path in out_paths:
                print(f"  Report: {out_path}")
            success_count += 1
        except Exception as exc:
            print(f"  FAILED: {exc}")

    elapsed = time.time() - start
    print(f"Completed. Success: {success_count}/{len(pairs)} pairs in {elapsed:.2f}s")
    return 0 if success_count > 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
