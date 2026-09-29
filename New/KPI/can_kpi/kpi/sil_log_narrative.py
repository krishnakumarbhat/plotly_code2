#!/usr/bin/env python3
from __future__ import annotations

import argparse
import html
import math
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Sequence, Tuple

import numpy as np
import pandas as pd
from scipy.optimize import linear_sum_assignment
from scipy.spatial.distance import cdist

try:
    from . import sil_radar_validation as sil
except Exception:
    import sil_radar_validation as sil


@dataclass
class SensorNarrative:
    sensor: str
    total_scans: int
    mean_recall: float
    mean_precision: float
    p95_dran: float
    p99_dvel: float
    weak_windows: List[Tuple[int, int, float]]
    orphan_spike_windows: List[Tuple[int, int, float]]
    scenario_lines: List[str]


def _safe_float(v: float) -> str:
    if v is None or (isinstance(v, float) and math.isnan(v)):
        return "NA"
    return f"{v:.4f}"


def _distance_metric(veh_points: np.ndarray, resim_points: np.ndarray, metric: str) -> np.ndarray:
    if metric == "mahalanobis":
        merged = np.vstack([veh_points, resim_points])
        cov = np.cov(merged.T)
        if np.linalg.matrix_rank(cov) < cov.shape[0]:
            cov = cov + np.eye(cov.shape[0]) * 1e-6
        inv_cov = np.linalg.pinv(cov)
        return cdist(veh_points, resim_points, metric="mahalanobis", VI=inv_cov)
    return cdist(veh_points, resim_points, metric="euclidean")


def _scan_level_match_stats(
    veh_df: pd.DataFrame,
    resim_df: pd.DataFrame,
    gate: float,
    metric: str,
) -> pd.DataFrame:
    veh_groups = veh_df.groupby("scan_index").indices
    resim_groups = resim_df.groupby("scan_index").indices
    common_scans = sorted(set(veh_groups.keys()) & set(resim_groups.keys()))

    veh_xyz = veh_df[["ran", "vel", "azimuth"]].to_numpy(dtype=np.float64, copy=False)
    resim_xyz = resim_df[["ran", "vel", "azimuth"]].to_numpy(dtype=np.float64, copy=False)

    rows: List[Dict[str, float]] = []
    for scan in common_scans:
        veh_idx = np.asarray(veh_groups[scan], dtype=np.int64)
        resim_idx = np.asarray(resim_groups[scan], dtype=np.int64)
        if veh_idx.size == 0 or resim_idx.size == 0:
            continue

        dmat = _distance_metric(veh_xyz[veh_idx], resim_xyz[resim_idx], metric)
        row_pos, col_pos = linear_sum_assignment(dmat)
        assigned = dmat[row_pos, col_pos]
        ok = assigned <= gate
        matched = int(np.count_nonzero(ok))

        rows.append(
            {
                "scan_index": int(scan),
                "veh_count": int(veh_idx.size),
                "resim_count": int(resim_idx.size),
                "matched_count": matched,
                "recall": matched / max(1, int(veh_idx.size)),
                "precision": matched / max(1, int(resim_idx.size)),
                "orphan_rate_veh": 1.0 - (matched / max(1, int(veh_idx.size))),
                "orphan_rate_resim": 1.0 - (matched / max(1, int(resim_idx.size))),
                "mean_gate_distance": float(np.nanmean(assigned[ok])) if np.any(ok) else float("nan"),
            }
        )

    if not rows:
        return pd.DataFrame(
            columns=[
                "scan_index",
                "veh_count",
                "resim_count",
                "matched_count",
                "recall",
                "precision",
                "orphan_rate_veh",
                "orphan_rate_resim",
                "mean_gate_distance",
            ]
        )

    return pd.DataFrame(rows).sort_values("scan_index").reset_index(drop=True)


def _find_windows(scan_df: pd.DataFrame, metric_col: str, threshold: float, min_len: int = 4) -> List[Tuple[int, int, float]]:
    if scan_df.empty or metric_col not in scan_df.columns:
        return []

    bad = scan_df[metric_col].to_numpy(dtype=np.float64) >= threshold
    scans = scan_df["scan_index"].to_numpy(dtype=np.int64)

    windows: List[Tuple[int, int, float]] = []
    start = None
    for i, flag in enumerate(bad):
        if flag and start is None:
            start = i
        if (not flag or i == len(bad) - 1) and start is not None:
            end_i = i if flag and i == len(bad) - 1 else i - 1
            if end_i - start + 1 >= min_len:
                block = scan_df.iloc[start : end_i + 1]
                windows.append((int(scans[start]), int(scans[end_i]), float(block[metric_col].mean())))
            start = None
    return windows


def _object_presence_hint(frame: pd.DataFrame) -> List[str]:
    if frame.empty:
        return ["No valid detections available for object-type inference."]

    ran = frame["ran"].to_numpy(dtype=np.float64)
    vel = np.abs(frame["vel"].to_numpy(dtype=np.float64))
    has_rcs = "rcs" in frame.columns
    rcs = frame["rcs"].to_numpy(dtype=np.float64) if has_rcs else np.full_like(ran, np.nan)

    notes: List[str] = []

    if has_rcs and np.isfinite(rcs).any():
        q75 = float(np.nanpercentile(rcs, 75))
        truck_like = int(np.count_nonzero((ran > 10) & (ran < 120) & (vel < 8) & (rcs >= q75)))
        if truck_like > 30:
            notes.append(f"Truck-like signatures are likely present (high-RCS cluster count ~{truck_like}).")

        ped_like = int(np.count_nonzero((ran < 35) & (vel < 3) & (rcs < q75)))
        if ped_like > 20:
            notes.append(f"Pedestrian-like signatures may be present in near range (count ~{ped_like}).")

        bike_like = int(np.count_nonzero((ran >= 15) & (ran <= 70) & (vel >= 2) & (vel <= 12) & (rcs < q75)))
        if bike_like > 20:
            notes.append(f"Bike-like signatures may be present (mid-range, moderate relative speed, count ~{bike_like}).")
    else:
        near_slow = int(np.count_nonzero((ran < 40) & (vel < 4)))
        if near_slow > 40:
            notes.append("Near-range slow-moving signatures suggest pedestrian/cyclist traffic in parts of this log.")
        dense_mid = int(np.count_nonzero((ran >= 20) & (ran <= 120)))
        if dense_mid > 100:
            notes.append("Dense mid-range returns indicate at least one larger object stream (possible truck/large vehicle).")

    if not notes:
        notes.append("Object-class evidence is weak in this file; no strong truck/pedestrian/bike signature was isolated.")
    return notes


def _host_motion_hint(frame: pd.DataFrame) -> str:
    vel_abs = np.abs(frame["vel"].to_numpy(dtype=np.float64)) if "vel" in frame.columns else np.array([])
    if vel_abs.size == 0:
        return "Host-motion inference unavailable (no velocity channel)."

    med = float(np.nanmedian(vel_abs))
    p90 = float(np.nanpercentile(vel_abs, 90))
    if med < 1.0 and p90 < 3.0:
        return "This log is consistent with a mostly stationary or very low-speed host phase (inferred from low relative velocity spread)."
    if med < 2.0:
        return "This log suggests low-speed host behavior for substantial intervals (inferred from relative velocity statistics)."
    return "This log appears to include moderate host or traffic-relative motion; not a purely stationary scenario."


def _road_context_hint(scan_df: pd.DataFrame, frame: pd.DataFrame) -> List[str]:
    notes: List[str] = []
    if scan_df.empty:
        return ["Road-context inference unavailable due to missing scan-level stats."]

    if "azimuth" in frame.columns:
        per_scan_az_std = frame.groupby("scan_index")["azimuth"].std().replace([np.inf, -np.inf], np.nan).dropna()
        if not per_scan_az_std.empty and float(per_scan_az_std.median()) > 25.0:
            notes.append("Wide azimuth spread indicates possible curve/turn geometry in parts of the sequence.")

    orphan_mix = (scan_df["orphan_rate_veh"] + scan_df["orphan_rate_resim"]) / 2.0
    spikes = _find_windows(scan_df.assign(orphan_mix=orphan_mix), "orphan_mix", threshold=0.55, min_len=5)
    if spikes:
        s0, s1, _ = spikes[0]
        notes.append(f"Potential infrastructure clutter (e.g., overhead structure/bridge-like scene) around scan_index {s0}-{s1} due to sustained orphan spikes.")
    else:
        notes.append("No strong sustained clutter spike detected for overhead-structure inference.")

    return notes


def _build_sensor_narrative(sensor: str, veh: sil.LoadedRadar, resim: sil.LoadedRadar, gate: float, metric: str) -> SensorNarrative:
    match = sil.match_points(veh.frame, resim.frame, gate_threshold=gate, metric=metric)
    kpis = sil.compute_kpis(veh.frame, resim.frame, match)
    scan_df = _scan_level_match_stats(veh.frame, resim.frame, gate=gate, metric=metric)

    weak_windows = _find_windows(scan_df, metric_col="orphan_rate_resim", threshold=0.45, min_len=5)
    orphan_spike_windows = _find_windows(scan_df, metric_col="orphan_rate_veh", threshold=0.55, min_len=5)

    scenario_lines = [
        _host_motion_hint(veh.frame),
        *_object_presence_hint(veh.frame),
        *_road_context_hint(scan_df, veh.frame),
    ]

    return SensorNarrative(
        sensor=sensor,
        total_scans=int(scan_df.shape[0]),
        mean_recall=float(scan_df["recall"].mean()) if not scan_df.empty else float("nan"),
        mean_precision=float(scan_df["precision"].mean()) if not scan_df.empty else float("nan"),
        p95_dran=float(kpis.get("p95_abs_dran", float("nan"))),
        p99_dvel=float(kpis.get("p99_abs_dvel", float("nan"))),
        weak_windows=weak_windows,
        orphan_spike_windows=orphan_spike_windows,
        scenario_lines=scenario_lines,
    )


def _format_windows(name: str, windows: Sequence[Tuple[int, int, float]]) -> str:
    if not windows:
        return f"- {name}: no sustained problematic window detected."
    top = windows[:3]
    lines = [f"- {name}:"]
    for s0, s1, score in top:
        lines.append(f"  - scan_index {s0}-{s1}, severity={score:.3f}")
    return "\n".join(lines)


def _build_narrative_text(log_name: str, sensor_reports: Sequence[SensorNarrative]) -> str:
    lines: List[str] = []
    lines.append(f"Log: {log_name}")
    lines.append("")
    lines.append("Overall implication:")

    if not sensor_reports:
        lines.append("No sensor-level data could be extracted, so RESIM-vs-VEH behavior cannot be concluded.")
        return "\n".join(lines)

    mean_recall_all = np.nanmean([s.mean_recall for s in sensor_reports])
    if mean_recall_all >= 0.8:
        lines.append("RESIM performance is broadly aligned with VEH in most sections, with localized misses.")
    elif mean_recall_all >= 0.6:
        lines.append("RESIM is partially aligned but has multiple stability gaps across sensors and scan windows.")
    else:
        lines.append("RESIM underperforms notably in this log with frequent misses against VEH detections.")

    lines.append("")
    for s in sensor_reports:
        lines.append(f"[{s.sensor}] summary")
        lines.append(f"- mean_recall={_safe_float(s.mean_recall)}, mean_precision={_safe_float(s.mean_precision)}, scans={s.total_scans}")
        lines.append(f"- p95|Δrange|={_safe_float(s.p95_dran)}, p99|Δvel|={_safe_float(s.p99_dvel)}")
        lines.append(_format_windows("RESIM weak windows", s.weak_windows))
        lines.append(_format_windows("VEH orphan-heavy windows", s.orphan_spike_windows))
        for sc in s.scenario_lines:
            lines.append(f"- {sc}")
        lines.append("")

    lines.append("Actionable note:")
    lines.append("Use weak-window scan ranges above as anchors for scenario replay and model tuning (association gate, clutter rejection, and per-sensor calibration).")
    return "\n".join(lines)


def _write_html(output_path: Path, title: str, text_body: str) -> None:
    output_path.parent.mkdir(parents=True, exist_ok=True)
    escaped = html.escape(text_body)
    html_body = f"""<!doctype html>
<html lang=\"en\">
<head>
  <meta charset=\"utf-8\" />
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />
  <title>{html.escape(title)}</title>
  <style>
    body {{ font-family: Arial, sans-serif; background: #0f172a; color: #e2e8f0; margin: 24px; }}
    .panel {{ background: #111827; border: 1px solid #334155; border-radius: 10px; padding: 16px; }}
    h1 {{ margin-top: 0; }}
    pre {{ white-space: pre-wrap; word-break: break-word; line-height: 1.45; font-size: 14px; }}
  </style>
</head>
<body>
  <div class=\"panel\">
    <h1>{html.escape(title)}</h1>
    <pre>{escaped}</pre>
  </div>
</body>
</html>
"""
    output_path.write_text(html_body, encoding="utf-8")


def process_pair(pair: sil.FilePair, output_dir: Path, gate: float, metric: str, max_sensors: int = 4) -> Path:
    veh_sensors = sil.sensors_in_file(pair.veh_path)
    resim_sensors = sil.sensors_in_file(pair.resim_path)
    sensors = sorted(set(veh_sensors) & set(resim_sensors))

    if not sensors and pair.sensor != "UNKNOWN":
        sensors = [pair.sensor]
    if max_sensors > 0:
        sensors = sensors[:max_sensors]

    sensor_reports: List[SensorNarrative] = []
    for sensor in sensors:
        try:
            veh = sil.load_radar_hdf(pair.veh_path, sensor=sensor)
            resim = sil.load_radar_hdf(pair.resim_path, sensor=sensor)
            sensor_reports.append(_build_sensor_narrative(sensor, veh, resim, gate=gate, metric=metric))
        except Exception:
            continue

    narrative_text = _build_narrative_text(pair.base_key, sensor_reports)
    out_path = output_dir / f"detailed_on_{pair.base_key}.html"
    _write_html(out_path, title=f"Detailed Log Narrative - {pair.base_key}", text_body=narrative_text)
    return out_path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate narrative HTML summaries per VEH/RESIM log pair.")
    parser.add_argument("input_dir", type=Path, help="Directory containing .h5/.h5.gz files")
    parser.add_argument("--output-dir", type=Path, default=None, help="Output directory (default: same as input_dir)")
    parser.add_argument("--gate", type=float, default=1.0, help="Gate threshold for assignment distance")
    parser.add_argument("--metric", type=str, default="euclidean", choices=["euclidean", "mahalanobis"], help="Distance metric")
    parser.add_argument("--max-pairs", type=int, default=1, help="How many pairs to process (default 1)")
    parser.add_argument("--max-sensors", type=int, default=4, help="Sensors per pair to summarize")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    input_dir = args.input_dir
    output_dir = args.output_dir or input_dir

    if not input_dir.exists() or not input_dir.is_dir():
        raise FileNotFoundError(f"Input directory does not exist: {input_dir}")

    pairs = sil.auto_pair_files(input_dir)
    if not pairs:
        print("No valid VEH/RESIM file pairs found.")
        return 1

    if args.max_pairs > 0:
        pairs = pairs[: args.max_pairs]

    print(f"Found {len(pairs)} pairs. Generating detailed narrative HTML...")
    success = 0
    for i, pair in enumerate(pairs, start=1):
        try:
            out = process_pair(pair, output_dir=output_dir, gate=args.gate, metric=args.metric, max_sensors=args.max_sensors)
            print(f"[{i}/{len(pairs)}] {pair.base_key} -> {out}")
            success += 1
        except Exception as exc:
            print(f"[{i}/{len(pairs)}] {pair.base_key} FAILED: {exc}")

    print(f"Completed. Success: {success}/{len(pairs)}")
    return 0 if success > 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
