"""Tabbed KPI index page in the CAN-KPI visual language.

One index per run (base_name): an accuracy summary matrix on top plus
Detection / Alignment / Tracker tabs. Each tab holds a per-sensor table
(status, accuracy, detail, link to the full report) and the concrete
reason whenever a KPI is absent. Reasons come from the per-sensor
diagnostics.json sidecars written by the wrapper; when no sidecars exist
(old runs) the page falls back to a plain button grid.
"""

import html as _html
import json
import os
import re
from pathlib import Path
from typing import Dict, List, Optional

KPI_ORDER = ("detection", "alignment", "tracker")
KPI_TITLES = {
    "detection": "Detection",
    "alignment": "Alignment",
    "tracker": "Tracker",
}

# Canonical stream each KPI needs (+ accepted file variants for messages).
KPI_REQUIREMENTS = {
    "detection": {
        "need": "DETECTION_STREAM",
        "variants": ["DETECTION_STREAM"],
        "note": "RDD-assisted mode additionally uses RDD_STREAM.",
    },
    "alignment": {
        "need": "DYNAMIC_ALIGNMENT_STREAM",
        "variants": ["DYNAMIC_ALIGNMENT_STREAM", "Dyn_Align_STREAM"],
        "note": "",
    },
    "tracker": {
        "need": "TRACKER_STREAM",
        "variants": ["TRACKER_STREAM", "OBJECT_LIST_STREAM"],
        "note": "",
    },
}


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


def _discover_sensors(base_output: Path, base_name: str):
    """Return (index_root, [sensor dicts]) for a run."""
    root = base_output / base_name if (base_output / base_name).exists() else base_output
    sensors: List[Dict] = []
    if not root.exists():
        return root, sensors
    for sensor_dir in sorted(root.iterdir(), key=lambda p: p.name):
        if not sensor_dir.is_dir():
            continue
        files: Dict[str, Path] = {}
        kpi_dir = sensor_dir / "KPI"
        search = sorted(kpi_dir.glob(f"{base_name}_*_kpi.html")) if kpi_dir.is_dir() else []
        if not search:
            search = sorted(sensor_dir.rglob(f"{base_name}_*_kpi.html"))
        for f in search:
            for t, suffix in (
                ("alignment", "alignment_kpi"),
                ("detection", "detection_kpi"),
                ("tracker", "tracker_kpi"),
            ):
                if suffix in f.name and t not in files:
                    files[t] = f
        diag: Optional[Dict] = None
        # Sidecar lives next to the KPI files (<sensor>/KPI/), with a
        # fallback to <sensor>/ for forward compatibility.
        for dp in (sensor_dir / "KPI" / "diagnostics.json", sensor_dir / "diagnostics.json"):
            if dp.exists():
                try:
                    diag = json.loads(dp.read_text(encoding="utf-8"))
                    break
                except Exception:
                    diag = None
        if files or diag:
            sensors.append(
                {"name": sensor_dir.name, "dir": sensor_dir, "files": files, "diag": diag}
            )
    return root, sensors


def _summarize_html(kpi_type: str, text: str):
    """Fallback headline extraction for runs without sidecars."""
    if not text:
        return ("missing", "—", "")
    if re.search(r"Failed to process \w+ KPIs", text):
        return ("failed", "failed", "")
    try:
        if kpi_type == "detection":
            m1 = re.search(r"Matched Detections:\s*<span[^>]*>([^<]+)</span>", text, re.S)
            m2 = re.search(r"Accuracy:\s*<span[^>]*>([^<%]+)%?</span>", text, re.S)
            if m1 and m2:
                return ("ok", m2.group(1).strip() + "%", m1.group(1).strip() + " matched")
        elif kpi_type == "alignment":
            ma = re.search(r"Azimuth Accuracy:</strong>\s*<span[^>]*>([^<]+)</span>", text, re.S)
            me = re.search(r"Elevation Accuracy:</strong>\s*<span[^>]*>([^<]+)</span>", text, re.S)
            ms = re.search(r"Total Scans Processed:</strong>\s*<span[^>]*>([^<]+)</span>", text, re.S)
            if ma and me:
                az = ma.group(1).strip().split("=")[-1].strip()
                el = me.group(1).strip().split("=")[-1].strip()
                scans = ms.group(1).strip() if ms else ""
                return ("ok", f"Az {az} · El {el}", f"{scans} scans" if scans else "")
        elif kpi_type == "tracker":
            mt = re.search(
                r"Tracker Accuracy:</strong>\s*\(([^)]+)\)[^<]*<strong>([^<]+)</strong>",
                text,
                re.S,
            )
            if mt:
                return ("ok", mt.group(2).strip(), mt.group(1).strip() + " matched")
    except Exception:
        pass
    return ("failed", "failed", "")


def _abridge(names, limit: int = 8) -> str:
    names = list(names or [])
    if not names:
        return "—"
    if len(names) <= limit:
        return ", ".join(names)
    return ", ".join(names[:limit]) + f" (+{len(names) - limit} more)"


def _why_missing(kpi: str, diag: Optional[Dict]) -> str:
    """Human reason why a KPI is absent. Handles every edge case."""
    req = KPI_REQUIREMENTS[kpi]
    need = req["need"]
    if not diag:
        return "No diagnostics recorded for this sensor — regenerate its reports to see why."
    res = (diag.get("resolution") or {}).get(need, {}) or {}
    got_in, got_out = res.get("input"), res.get("output")
    streams_in = diag.get("streams_in", []) or []
    streams_out = diag.get("streams_out", []) or []
    have = sorted(set(streams_in) | set(streams_out))
    if not got_in and not got_out:
        msg = f"Needs {need}"
        if len(req["variants"]) > 1:
            msg += f" (also accepted as: {', '.join(req['variants'][1:])})"
        msg += ". It is not present in this log"
        if kpi == "detection" and "PROCESSED_DETECTION_STREAM" in have:
            msg += " — the log has PROCESSED_DETECTION_STREAM instead, which uses a different F360 schema (object positions/velocities, no RDD indices), so the RDD-assisted detection KPI cannot run on it"
        if kpi == "alignment" and not any("align" in s.lower() for s in have):
            msg += " — this log carries no alignment stream at all (no boresight signals)"
        if not have:
            msg += " (no streams discovered for this sensor)"
        else:
            msg += f". File contains: {_abridge(have)}"
        msg += "."
        if req["note"]:
            msg += " " + req["note"]
        return msg
    if not got_in or not got_out:
        missing_side = "input" if not got_in else "output"
        present_side = "output" if not got_in else "input"
        return (
            f"Needs {need}: present in the {present_side} file "
            f"but missing in the {missing_side} file."
        )
    return (
        "Streams are present but the KPI produced no result "
        "(signal/scan mismatch) — open the full report for details."
    )


def _esc(text) -> str:
    return _html.escape(str(text if text is not None else ""), quote=True)


def _chip(status: str) -> str:
    cls = {"ok": "ok", "failed": "failed"}.get(status, "missing")
    label = {"ok": "OK", "failed": "FAILED"}.get(status, "MISSING")
    return f'<span class="chip {cls}">{label}</span>'


def _cell(sensor: Dict, kpi: str, index_root: Path) -> Dict[str, str]:
    """Resolve one sensor×KPI cell: status/headline/detail/link/reason."""
    diag = sensor.get("diag")
    files = sensor.get("files", {})
    href = ""
    f = files.get(kpi)
    if f is not None:
        try:
            href = os.path.relpath(f, index_root)
        except Exception:
            href = f.name
    status = headline = detail = ""
    if diag and (diag.get("kpis") or {}).get(kpi):
        k = diag["kpis"][kpi]
        status = k.get("status", "") or ""
        headline = k.get("headline", "") or ""
        detail = k.get("detail", "") or ""
        if f is None and status == "ok":
            status = "missing"
    elif f is not None:
        try:
            text = f.read_text(encoding="utf-8", errors="replace")
        except Exception:
            text = ""
        status, headline, detail = _summarize_html(kpi, text)
    else:
        status, headline, detail = ("missing", "—", "")
    if status not in ("ok", "failed", "missing"):
        status = "missing"
    reason = _why_missing(kpi, diag) if status in ("failed", "missing") else ""
    return {
        "status": status,
        "headline": headline or "—",
        "detail": detail,
        "href": href,
        "reason": reason,
    }


def _summary_matrix(sensors: List[Dict], index_root: Path) -> str:
    cells = {s["name"]: {k: _cell(s, k, index_root) for k in KPI_ORDER} for s in sensors}
    head = "".join(f"<th>{KPI_TITLES[k]}</th>" for k in KPI_ORDER)
    rows = []
    for s in sensors:
        tds = [f"<td><strong>{_esc(s['name'])}</strong></td>"]
        for k in KPI_ORDER:
            c = cells[s["name"]][k]
            tds.append(f"<td>{_chip(c['status'])}<br/><span class='acc'>{_esc(c['headline'])}</span></td>")
        rows.append("<tr>" + "".join(tds) + "</tr>")
    return (
        "<h2>KPI Summary</h2>"
        '<div class="scroll"><table><thead><tr><th>Sensor</th>'
        + head
        + "</tr></thead><tbody>"
        + "".join(rows)
        + "</tbody></table></div>"
    )


def _kpi_tab(sensors: List[Dict], kpi: str, index_root: Path) -> str:
    rows = []
    problems: List[str] = []
    for s in sensors:
        c = _cell(s, kpi, index_root)
        link = (
            f"<a class='btn-link' href='{_esc(c['href'])}' target='_blank'>Open report</a>"
            if c["href"]
            else "<span class='muted'>no file</span>"
        )
        rows.append(
            "<tr>"
            f"<td><strong>{_esc(s['name'])}</strong></td>"
            f"<td>{_chip(c['status'])}</td>"
            f"<td>{_esc(c['headline'])}</td>"
            f"<td>{_esc(c['detail'])}</td>"
            f"<td>{link}</td>"
            "</tr>"
        )
        if c["status"] in ("failed", "missing") and c["reason"]:
            problems.append(f"<li><strong>{_esc(s['name'])}</strong>: {_esc(c['reason'])}</li>")
    table = (
        f"<h3>{KPI_TITLES[kpi]} — per-sensor accuracy</h3>"
        '<div class="scroll"><table><thead><tr>'
        "<th>Sensor</th><th>Status</th><th>Accuracy</th><th>Detail</th><th>Report</th>"
        "</tr></thead><tbody>" + "".join(rows) + "</tbody></table></div>"
    )
    notice = ""
    if problems:
        notice = (
            '<section class="notice-block warning">'
            f"<h3>Why {KPI_TITLES[kpi]} is missing / failed</h3>"
            "<ul>" + "".join(problems) + "</ul></section>"
        )
    return table + notice


def _contents_section(sensors: List[Dict]) -> str:
    parts = ["<h2>Log contents per sensor</h2>"]
    for s in sensors:
        diag = s.get("diag") or {}
        ins = diag.get("streams_in", []) or []
        outs = diag.get("streams_out", []) or []
        if not ins and not outs:
            body = "<p class='muted'>No stream inventory recorded — regenerate this sensor.</p>"
        else:
            body = (
                f"<p><strong>Input file streams:</strong> {_esc(_abridge(ins, 50))}</p>"
                f"<p><strong>Output file streams:</strong> {_esc(_abridge(outs, 50))}</p>"
            )
        parts.append(f"<details><summary><strong>{_esc(s['name'])}</strong></summary>{body}</details>")
    return "\n".join(parts)


def _legacy_grid(base_output: Path, base_name: str, index_root: Path) -> str:
    files = _find_kpi_files(base_output, base_name)
    if not files:
        return '<div class="empty">No KPI HTML files found.</div>'
    buttons = []
    for f in files:
        label = f"{f.parent.name}: {f.stem.replace(base_name + '_', '')}"
        try:
            rel = os.path.relpath(f, index_root)
        except Exception:
            rel = f.name
        buttons.append(f'<a class="btn" href="{_esc(rel)}" target="_blank">{_esc(label)}</a>')
    return (
        '<div class="grid">' + "".join(buttons) + "</div>"
        '<p class="muted">Accuracy summary unavailable for these files — '
        "regenerate the reports to enable the tabbed summary.</p>"
    )


def generate_kpi_index(output_dir: str, base_name: str) -> str:
    out_path = Path(output_dir)
    index_root = out_path / base_name if (out_path / base_name).exists() else out_path
    index_root.mkdir(parents=True, exist_ok=True)
    index_file = index_root / f"{base_name}_kpi.html"

    _, sensors = _discover_sensors(out_path, base_name)
    use_tabs = any(s.get("diag") for s in sensors)

    if use_tabs and sensors:
        tabs = {KPI_TITLES[k]: _kpi_tab(sensors, k, index_root) for k in KPI_ORDER}
        body = (
            _summary_matrix(sensors, index_root)
            + _tab_bar(tabs, title=f"{base_name} KPI Reports")
            + _contents_section(sensors)
        )
    else:
        body = f"<h1>{_esc(base_name)} KPI Reports</h1>" + _legacy_grid(
            out_path, base_name, index_root
        )

    with open(index_file, "w", encoding="utf-8") as fp:
        fp.write(_page(f"{base_name} KPI Reports", body))

    return str(index_file)


def _tab_bar(tabs: Dict[str, str], title: str) -> str:
    buttons: List[str] = []
    contents: List[str] = []
    for i, (name, content) in enumerate(tabs.items()):
        active = " active" if i == 0 else ""
        display = "block" if i == 0 else "none"
        safe = re.sub(r"[^A-Za-z0-9]+", "_", name)
        buttons.append(
            f'<button class="tab-btn{active}" '
            f"onclick=\"switchTab('{safe}', this)\">{_esc(name)}</button>"
        )
        contents.append(
            f'<div id="tab_{safe}" class="tab-content" style="display:{display}">'
            f"{content}</div>"
        )
    return (
        f"<h1>{_esc(title)}</h1>"
        '<div class="tab-bar">' + "".join(buttons) + "</div>" + "".join(contents)
    )


def _page(title: str, body: str) -> str:
    return f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>{_esc(title)}</title>
  <style>{_css()}</style>
</head>
<body>
{body}
<script>{_tab_js()}</script>
</body>
</html>
"""


def _css() -> str:
    return """
:root {
    --page-bg: #f2f7fc; --panel-bg: #ffffff; --panel-border: #d9e5ee;
    --text-main: #17324a; --text-muted: #627789; --accent: #0d6a8b;
}
* { box-sizing: border-box; }
body {
    font-family: 'Segoe UI', Arial, sans-serif; margin: 0; padding: 18px;
    background: linear-gradient(180deg, #f8fbfd 0%, var(--page-bg) 100%);
    color: var(--text-main);
}
h1 { margin: 0 0 12px 0; }
h2 { margin: 18px 0 8px 0; }
h3 { margin: 12px 0 6px 0; }
.muted { color: var(--text-muted); font-size: 0.9em; }
.tab-bar {
    display: flex; flex-wrap: wrap; border-bottom: 2px solid #b9d7e6;
    margin-bottom: 16px; background: rgba(255,255,255,0.92);
    border-radius: 14px 14px 0 0; box-shadow: 0 10px 22px rgba(18,47,72,0.08);
}
.tab-btn {
    padding: 10px 22px; border: none; background: #edf5f9; cursor: pointer;
    font-size: 14px; font-weight: 600; color: var(--text-muted);
    border-radius: 14px 14px 0 0; margin-right: 2px;
    transition: background 0.16s ease, color 0.16s ease;
}
.tab-btn:hover { background: #dcecf4; color: var(--text-main); }
.tab-btn.active { background: var(--accent); color: #fff; }
.tab-content { display: none; background: var(--panel-bg); border-radius: 0 0 16px 16px;
    padding: 16px; margin-bottom: 16px; box-shadow: 0 10px 22px rgba(18,47,72,0.08);
    border: 1px solid var(--panel-border); border-top: none; }
.scroll { max-height: 420px; overflow: auto; }
table { border-collapse: collapse; width: 100%; font-size: 13px; background: #fff; }
th, td { border: 1px solid #d7e1e9; padding: 8px 10px; text-align: left; vertical-align: top; }
th { background: #edf5fa; position: sticky; top: 0; font-weight: 700; }
tr:nth-child(even) { background: #f8fbfd; }
tr:hover { background: #eef6fa; }
.chip { display: inline-block; padding: 2px 10px; border-radius: 999px;
    font-size: 12px; font-weight: 700; letter-spacing: 0.3px; }
.chip.ok { background: #e6f4ea; color: #1e7e34; border: 1px solid #b7dfc0; }
.chip.failed { background: #fff6e5; color: #7a5412; border: 1px solid #f0cf82; }
.chip.missing { background: #eef1f4; color: #5f6b76; border: 1px solid #d4dae0; }
.acc { font-weight: 700; color: #0d6a8b; }
.notice-block { border-radius: 14px; padding: 12px 14px; margin: 12px 0 4px 0; border: 1px solid transparent; }
.notice-block h3 { margin: 0 0 8px 0; }
.notice-block ul { margin: 0; padding-left: 18px; }
.notice-block li { margin: 5px 0; }
.notice-block.warning { background: #fff6e5; border-color: #f0cf82; color: #7a5412; }
details { margin-top: 10px; background: var(--panel-bg); border: 1px solid var(--panel-border);
    border-radius: 12px; padding: 10px 14px; }
summary { cursor: pointer; font-weight: 600; }
.btn-link { display: inline-block; padding: 6px 12px; background: var(--accent); color: #fff;
    text-decoration: none; border-radius: 6px; font-size: 13px; font-weight: 600; }
.btn-link:hover { background: #0a546f; }
.grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(280px, 1fr)); gap: 16px; margin-top: 20px; }
.btn { display: inline-block; padding: 14px 16px; background: #4f46e5; color: #fff; text-decoration: none;
    border-radius: 8px; font-weight: 600; }
.empty { color: var(--text-muted); margin-top: 12px; }
@media (max-width: 900px) { body { padding: 10px; } }
"""


def _tab_js() -> str:
    return """
function switchTab(tabId, btn) {
    document.querySelectorAll('.tab-content').forEach(c => c.style.display = 'none');
    document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
    document.getElementById('tab_' + tabId).style.display = 'block';
    btn.classList.add('active');
}
"""


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Aggregate KPI HTML links into an index page")
    parser.add_argument("--output-dir", required=True, help="Output directory root")
    parser.add_argument("--base-name", required=True, help="Base name for this run")
    args = parser.parse_args()

    path = generate_kpi_index(args.output_dir, args.base_name)
    print(path)
