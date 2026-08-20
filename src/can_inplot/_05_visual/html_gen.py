"""HTML report generation for CAN KPI.

Purpose: assemble tabbed HTML reports (tables + interactive Plotly figures)
per sensor and provide the KPI engine used by the ZMQ server.
Inputs : metric results from 02_kpi, figures from plots.py.
Outputs: HTML strings and report file paths.
"""

import logging
import re
from pathlib import Path
from typing import Dict, List, Optional

import numpy as np

from can_inplot._02_kpi.kpi_business import KpiBusiness

logger = logging.getLogger(__name__)

PLOTLY_CDN = "https://cdn.plot.ly/plotly-3.0.1.min.js"


def html_from_fig(fig) -> str:
    """Purpose: render a plotly figure to a standalone HTML div.
    Inputs : plotly Figure or None.
    Outputs: HTML string (empty when figure missing)."""
    if fig is None:
        return ""
    try:
        import plotly.io as pio

        return pio.to_html(
            fig, full_html=False, include_plotlyjs=False, config={"scrollZoom": True}
        )
    except Exception as exc:
        logger.warning("Figure render failed: %s", exc)
        return ""


class KpiHtmlGen:
    """Builds HTML fragments and full tabbed KPI reports."""

    def stats_table(self, title: str, headers: List[str], rows: List[List[str]]) -> str:
        """Purpose: styled stats table.
        Inputs : title, headers, rows.
        Outputs: HTML table string."""
        head = "".join(f"<th>{h}</th>" for h in headers)
        body = "".join(
            "<tr>" + "".join(f"<td>{c}</td>" for c in row) + "</tr>" for row in rows
        )
        return (
            f"<h3>{title}</h3>"
            '<div class="table-wrap"><table class="kpi-table">'
            f"<thead><tr>{head}</tr></thead><tbody>{body}</tbody></table></div>"
        )

    def notice_block(self, title: str, messages: List[str], tone: str = "warning") -> str:
        """Purpose: warning/error notice block.
        Inputs : title, message list, tone.
        Outputs: HTML block."""
        items = "".join(f"<li>{m}</li>" for m in messages)
        cls = "notice-error" if tone == "error" else "notice-warning"
        return (
            f'<div class="{cls}"><h4>{title}</h4><ul>{items}</ul></div>'
        )

    def build_tabbed_html(
        self, tabs: Dict[str, str], title: str, summary_html: str = ""
    ) -> str:
        """Purpose: assemble the full tabbed report page.
        Inputs : tab name -> HTML body, page title, summary HTML.
        Outputs: standalone HTML document."""
        tab_buttons = "".join(
            f'<button class="tab-btn" data-tab="tab-{i}">{name}</button>'
            for i, name in enumerate(tabs.keys())
        )
        tab_panes = "".join(
            f'<div class="tab-pane" id="tab-{i}">{body}</div>'
            for i, body in enumerate(tabs.values())
        )
        return "\n".join(
            [
                "<!DOCTYPE html>",
                "<html><head>",
                '<meta charset="utf-8"/>',
                '<meta name="viewport" content="width=device-width, initial-scale=1"/>',
                f"<title>{title}</title>",
                f'<script src="{PLOTLY_CDN}"></script>',
                "<style>",
                "*{box-sizing:border-box;}",
                "body{font-family:Segoe UI,Arial,sans-serif;margin:0;padding:20px;background:#f5f6fa;color:#2c3e50;}",
                ".page{max-width:1300px;margin:0 auto;}",
                "h1{color:#2c3e50;margin:0 0 6px 0;}",
                ".sub{color:#5b6b7b;font-size:14px;margin:0 0 16px 0;}",
                ".summary{background:#fff;border:1px solid #e8ecef;border-radius:12px;padding:14px;margin:0 0 16px 0;}",
                ".tabbar{display:flex;flex-wrap:wrap;gap:6px;margin:0 0 14px 0;}",
                ".tab-btn{background:#fff;border:1px solid #d5dbe2;border-radius:8px;padding:8px 14px;cursor:pointer;font-size:13px;color:#2c3e50;}",
                ".tab-btn.active{background:#3498db;color:#fff;border-color:#3498db;}",
                ".tab-pane{display:none;background:#fff;border:1px solid #e8ecef;border-radius:12px;padding:16px;box-shadow:0 4px 14px rgba(0,0,0,.06);}",
                ".tab-pane.active{display:block;}",
                ".table-wrap{overflow:auto;border-radius:8px;border:1px solid #eef1f4;margin:0 0 14px 0;}",
                ".kpi-table{width:100%;border-collapse:collapse;min-width:480px;}",
                ".kpi-table th,.kpi-table td{padding:8px 10px;border-bottom:1px solid #edf1f5;text-align:left;font-size:13px;}",
                ".kpi-table th{background:#f8fbff;color:#2f4358;position:sticky;top:0;}",
                ".kpi-table tbody tr:hover{background:#f7fbff;}",
                ".notice-warning{border:1px solid #f5d9a0;background:#fff8ec;border-radius:10px;padding:10px 14px;margin:0 0 12px 0;}",
                ".notice-error{border:1px solid #f0a0a0;background:#fdf0f0;border-radius:10px;padding:10px 14px;margin:0 0 12px 0;}",
                ".notice-warning h4,.notice-error h4{margin:0 0 6px 0;}",
                "@media (max-width:640px){body{padding:12px;}}",
                "</style>",
                "</head><body>",
                '<main class="page">',
                f"<h1>{title}</h1>",
                '<p class="sub">CAN radar KPI report — unified can_inplot pipeline.</p>',
                f'<section class="summary">{summary_html}</section>' if summary_html else "",
                '<nav class="tabbar">',
                f"{tab_buttons}</nav>",
                f"{tab_panes}",
                "<script>",
                "document.querySelectorAll('.tab-btn').forEach(function(btn,i){btn.addEventListener('click',function(){",
                "document.querySelectorAll('.tab-btn').forEach(function(b){b.classList.remove('active')});",
                "document.querySelectorAll('.tab-pane').forEach(function(p){p.classList.remove('active')});",
                "btn.classList.add('active');document.getElementById('tab-'+i).classList.add('active');});});",
                "document.querySelector('.tab-btn').classList.add('active');",
                "document.querySelector('.tab-pane').classList.add('active');",
                "</script>",
                "</main></body></html>",
            ]
        )

    def build_sensor_tab(
        self,
        label: str,
        scan: np.ndarray,
        metrics: Dict[str, np.ndarray],
        per_signal: Dict[str, np.ndarray],
    ) -> str:
        """Purpose: per-sensor metrics tab with plots.
        Inputs : label, scan array, metric arrays, per-signal arrays.
        Outputs: HTML tab body."""
        parts: List[str] = [f"<h3>{label}</h3>"]
        rows = []
        n = len(scan)
        names = list(metrics.keys())
        for i in range(n):
            rows.append(
                [
                    str(int(scan[i])),
                    *[f"{metrics[k][i]:.2f}" for k in names],
                ]
            )
        parts.append(self.stats_table(f"Per-scan metrics ({n} scans)", ["Scan", *names], rows))
        sig_rows = [
            [sig, f"{per_signal[sig].mean():.2f}"] if len(per_signal[sig]) else [sig, "NA"]
            for sig in per_signal
        ]
        parts.append(self.stats_table("Per-signal average match %", ["Signal", "Avg %"], sig_rows))
        return "\n".join(parts)

    def build_kpi_tab(self, kpi_table: str, kpi_plot: str) -> str:
        """Purpose: aggregate KPI tab.
        Inputs : KPI table HTML and plot HTML.
        Outputs: tab body."""
        return f"{kpi_table}{kpi_plot}"

    def match_all_radars_plot(self, radar_plot_data: Dict[str, tuple], title: str) -> str:
        """Purpose: multi-sensor overall-match figure.
        Inputs : radar name -> (scan, overall) tuples.
        Outputs: plotly HTML div."""
        try:
            import plotly.graph_objects as go
            import plotly.io as pio
        except Exception:
            return ""
        fig = go.Figure()
        for name, (scan, overall) in radar_plot_data.items():
            fig.add_trace(
                go.Scatter(
                    x=scan,
                    y=overall.astype(float) if isinstance(overall, np.ndarray) else overall,
                    mode="lines",
                    name=name,
                )
            )
        fig.update_layout(
            title=title,
            template="plotly_white",
            height=420,
            xaxis_title="Scan",
            yaxis_title="Overall match %",
        )
        return pio.to_html(fig, full_html=False, include_plotlyjs=False)


class CanKpiEngine:
    """Engine implementing the ZMQ server contract (discover/generate)."""

    def __init__(self, business: Optional[KpiBusiness] = None) -> None:
        """Purpose: build the engine.
        Inputs : optional business layer.
        Outputs: engine instance."""
        self._business = business or KpiBusiness()
        self._html = KpiHtmlGen()

    def discover_sensors(self, input_hdf: str, output_hdf: str) -> List[str]:
        """Purpose: sensor list for an HDF pair (fast pre-run scan).
        Inputs : HDF paths.
        Outputs: sorted sensor ids."""
        try:
            import h5py
        except Exception:
            return []
        sensors: List[str] = []
        for path in (input_hdf, output_hdf):
            with h5py.File(path, "r") as f:
                sensors.extend(f.keys())
        seen: Dict[str, int] = {}
        out: List[str] = []
        for s in sensors:
            seen[s] = seen.get(s, 0) + 1
            if seen[s] == 1:
                out.append(s)
        return sorted(out)

    def generate_sensor_report(
        self,
        sensor_id: str,
        input_hdf: str,
        output_hdf: str,
        output_dir: str,
        base_name: str,
        kpi_subdir: str = "KPI",
    ) -> str:
        """Purpose: generate one sensor KPI HTML report.
        Inputs : request fields.
        Outputs: HTML file path."""
        in_parsed = self._business._hdf.parse_file(input_hdf) if input_hdf else {}
        out_parsed = self._business._hdf.parse_file(output_hdf) if output_hdf else {}
        in_stores = self._business._hdf.extract_storages(in_parsed)
        out_stores = self._business._hdf.extract_storages(out_parsed)
        in_store = in_stores.get(sensor_id)
        out_store = out_stores.get(sensor_id)
        result = self._business.compute_match_per_sensor(in_store, out_store, sensor_id)
        latency = (
            {}
            if in_store is None or out_store is None
            else self._business.compute_latency_kpis(in_store, out_store)
        )
        scan = result.get("scan", np.array([], dtype=np.int64))
        metrics = {
            "Overall": result.get("overall", np.array([], dtype=np.float16)),
            "Precision": result.get("precision", np.array([], dtype=np.float16)),
            "Recall": result.get("recall", np.array([], dtype=np.float16)),
            "F1": result.get("f1", np.array([], dtype=np.float16)),
            "Accuracy": result.get("accuracy", np.array([], dtype=np.float16)),
        }
        parts: List[str] = []
        rows = [[k, f"{v:.2f}"] for k, v in latency.items()]
        parts.append(self._html.stats_table("Latency KPIs", ["KPI", "Value (ms)"], rows))
        parts.append(
            self._html.build_sensor_tab(
                sensor_id, scan, metrics, result.get("per_signal", {})
            )
        )
        body = "\n".join(parts)
        page = self._html.build_tabbed_html(
            {sensor_id: body, "Latency": self._html.stats_table("Latency KPIs", ["KPI", "Value"], rows)},
            f"CAN KPI — {base_name} / {sensor_id}",
            summary_html="",
        )
        out_root = Path(output_dir) / base_name / kpi_subdir
        out_root.mkdir(parents=True, exist_ok=True)
        safe = re.sub(r"[^A-Za-z0-9_.-]", "_", sensor_id)
        report_path = out_root / f"{safe}_kpi.html"
        report_path.write_text(page, encoding="utf-8")
        logger.info("Wrote %s", report_path)
        return str(report_path)