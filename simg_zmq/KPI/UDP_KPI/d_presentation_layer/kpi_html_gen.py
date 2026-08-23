import os
from pathlib import Path
from typing import List


def _find_kpi_files(base_output: Path, base_name: str) -> List[Path]:
    candidates: List[Path] = []
    root = base_output / base_name if (base_output / base_name).exists() else base_output
    if not root.exists():
        return []

    for sensor_dir in root.iterdir():
        if sensor_dir.is_dir():
            for html in sensor_dir.rglob(f"{base_name}_*_kpi.html"):
                candidates.append(html)
    return sorted(candidates)


def generate_kpi_index(output_dir: str, base_name: str) -> str:
    out_path = Path(output_dir)
    files = _find_kpi_files(out_path, base_name)

    index_root = out_path / base_name if (out_path / base_name).exists() else out_path
    index_root.mkdir(parents=True, exist_ok=True)
    index_file = index_root / f"{base_name}_kpi.html"

    buttons = []
    for f in files:
        sensor = f.parent.name
        label = f"{sensor}: {f.stem.replace(base_name + '_', '')}"
        rel = os.path.relpath(f, index_root)
        buttons.append(f'<a class="btn" href="{rel}" target="_blank">{label}</a>')

    html = f"""
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>{base_name} KPI Index</title>
  <style>
    body {{ font-family: Arial, sans-serif; margin: 24px; background: #f7f9fc; }}
    h1 {{ color: #2c3e50; }}
    .grid {{ display: grid; grid-template-columns: repeat(auto-fill, minmax(280px, 1fr)); gap: 16px; margin-top: 20px; }}
    .btn {{
      display: inline-block; padding: 14px 16px; background: #4f46e5; color: #fff; text-decoration: none;
      border-radius: 8px; box-shadow: 0 4px 10px rgba(0,0,0,0.08); font-weight: 600; letter-spacing: .2px;
      transition: transform .05s ease, box-shadow .2s ease, background .2s ease;
    }}
    .btn:hover {{ transform: translateY(-1px); background: #4338ca; box-shadow: 0 6px 14px rgba(0,0,0,0.12); }}
    .empty {{ color: #6b7280; margin-top: 12px; }}
  </style>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;600&display=swap" rel="stylesheet">
  <style> body {{ font-family: 'Inter', Arial, sans-serif; }} </style>
</head>
<body>
  <h1>{base_name} KPI Reports</h1>
  <div class="grid">
    {''.join(buttons) if buttons else '<div class="empty">No KPI HTML files found.</div>'}
  </div>
</body>
</html>
"""

    with open(index_file, "w", encoding="utf-8") as fp:
        fp.write(html)

    return str(index_file)


def record_timeline_series(output_dir: str, base_name: str, sensor_id: str, sections: List[dict]) -> str:
    """Persist one sensor's compact per-scan series for the Timeline Overview.

    Called once per sensor right after its KPI HTML files are written; the
    overview page is rebuilt from all recorded sensors each time so the last
    call yields the complete page.
    """
    import json
    series_root = Path(output_dir) / base_name / "timeline_series"
    series_root.mkdir(parents=True, exist_ok=True)
    payload = {"sensor": sensor_id, "alignment": {}, "detection": {}}
    for item in sections or []:
        kpi_type = (item or {}).get("type")
        series = (item or {}).get("series") or {}
        if kpi_type == "alignment" and series:
            payload["alignment"] = {
                "az_est_diff": series.get("az_est_diff") or [],
                "el_est_diff": series.get("el_est_diff") or [],
            }
        elif kpi_type == "detection" and series:
            payload["detection"] = {
                "scan_index": series.get("scan_index") or [],
                "accuracy": series.get("accuracy") or [],
                "matches": series.get("matches") or [],
            }
    out_file = series_root / f"{sensor_id}.json"
    with open(out_file, "w", encoding="utf-8") as fp:
        json.dump(payload, fp)
    return str(out_file)


def generate_timeline_overview(output_dir: str, base_name: str) -> str:
    """Build <base>_timeline_overview.html: per-sensor signal line plots
    (misalignment + detection accuracy vs scan index) for this log."""
    import json

    try:
        import plotly.graph_objects as go
    except Exception:
        return ""

    out_path = Path(output_dir)
    series_root = out_path / base_name / "timeline_series"
    if not series_root.exists():
        return ""
    series_files = sorted(series_root.glob("*.json"))
    if not series_files:
        return ""

    index_root = out_path / base_name if (out_path / base_name).exists() else out_path
    sections_html = []
    first = True
    for sf in series_files:
        try:
            with open(sf, "r", encoding="utf-8") as fp:
                payload = json.load(fp)
        except Exception:
            continue
        sensor = payload.get("sensor") or sf.stem
        fig = go.Figure()
        has_trace = False

        align = payload.get("alignment") or {}
        az = align.get("az_est_diff") or []
        el = align.get("el_est_diff") or []
        if az:
            x = list(range(len(az)))
            fig.add_trace(go.Scattergl(x=x, y=az[:len(x)], mode="lines", name="AZ misalign diff (deg)",
                                       line=dict(color="#d62728")))
            has_trace = True
        if el:
            x = list(range(len(el)))
            fig.add_trace(go.Scattergl(x=x, y=el[:len(x)], mode="lines", name="EL misalign diff (deg)",
                                       line=dict(color="#ff7f0e")))
            has_trace = True

        det = payload.get("detection") or {}
        acc_x = det.get("scan_index") or []
        acc = det.get("accuracy") or []
        n = min(len(acc_x), len(acc))
        if n:
            fig.add_trace(go.Scattergl(x=acc_x[:n], y=acc[:n], mode="lines+markers", name="Detection accuracy",
                                       yaxis="y2", line=dict(color="#1f77b4")))
            has_trace = True

        if not has_trace:
            continue

        fig.update_layout(
            title=f"{sensor} — signal timeline",
            xaxis_title="Scan Index",
            yaxis_title="Misalignment diff (deg)",
            yaxis=dict(title="Misalignment diff (deg)"),
            yaxis2=dict(title="Accuracy", overlaying="y", side="right", range=[0, 105]),
            hovermode="closest",
            margin=dict(l=40, r=50, t=50, b=40),
        )
        sections_html.append(
            "<section class=\"ov-plot\"><h3>" + sensor + "</h3>"
            + fig.to_html(full_html=False, include_plotlyjs="cdn" if first else False,
                          config={"displaylogo": False, "responsive": True})
            + "</section>"
        )
        first = False

    # Links grid to per-sensor KPI reports
    buttons = []
    for f in _find_kpi_files(out_path, base_name):
        sensor = f.parent.name
        label = f"{sensor}: {f.stem.replace(base_name + '_', '')}"
        rel = os.path.relpath(f, index_root)
        buttons.append(f'<a class="btn" href="{rel}" target="_blank">{label}</a>')

    html = f"""
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>{base_name} Timeline Overview</title>
  <style>
    body {{ font-family: 'Inter', Arial, sans-serif; margin: 24px; background: #eef3f7; color: #17324a; }}
    h1 {{ color: #17324a; }}
    .ov-plot {{ background: #fff; border: 1px solid #d9e3ec; border-radius: 14px; padding: 10px 14px; margin-bottom: 14px; }}
    .grid {{ display: grid; grid-template-columns: repeat(auto-fill, minmax(280px, 1fr)); gap: 16px; margin-top: 20px; }}
    .btn {{
      display: inline-block; padding: 12px 14px; background: #175f7b; color: #fff; text-decoration: none;
      border-radius: 8px; font-weight: 600;
    }}
    .btn:hover {{ background: #0f4760; }}
  </style>
</head>
<body>
  <h1>{base_name} — Timeline Overview</h1>
  {''.join(sections_html) or '<p>No series recorded yet.</p>'}
  <h2>KPI Reports</h2>
  <div class="grid">{''.join(buttons)}</div>
</body>
</html>
"""
    overview_file = index_root / f"{base_name}_timeline_overview.html"
    with open(overview_file, "w", encoding="utf-8") as fp:
        fp.write(html)
    return str(overview_file)


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Aggregate KPI HTML links into an index page")
    parser.add_argument("--output-dir", required=True, help="Output directory root")
    parser.add_argument("--base-name", required=True, help="Base name for this run")
    args = parser.parse_args()

    path = generate_kpi_index(args.output_dir, args.base_name)
    print(path)
