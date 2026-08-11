"""Plotly HTML generation with tabbed sensor layout."""

from typing import Dict, List

import numpy as np
import plotly.graph_objects as go
from plotly.offline import plot


class KpiHtmlGen:
    """Generates Plotly scatter plots organized by sensor tabs."""

    COLORS_INPUT = "rgba(31,119,180,0.45)"
    COLORS_OUTPUT = "rgba(255,127,14,0.45)"

    def __init__(self, template: str | None = None):
        self._template = template

    def detection_scatter(
        self,
        scan_index: np.ndarray,
        signal_2d: np.ndarray,
        label: str,
        color: str = "rgba(31,119,180,0.4)",
    ) -> go.Scattergl:
        n_scans, n_dets = signal_2d.shape
        xs = np.repeat(scan_index, n_dets)
        ys = signal_2d.ravel()
        mask = ~np.isnan(ys) & (ys != 0)
        xs, ys = xs[mask], ys[mask]
        return go.Scattergl(
            x=xs.tolist(),
            y=ys.tolist(),
            mode="markers",
            marker=dict(size=2, color=color, opacity=0.5),
            name=label,
        )

    def input_output_scatter(
        self,
        in_scan: np.ndarray,
        in_2d: np.ndarray,
        out_scan: np.ndarray,
        out_2d: np.ndarray,
        signal_name: str,
        sensor_name: str,
    ) -> str:
        fig = go.Figure()
        if in_2d.size > 0:
            fig.add_trace(
                self.detection_scatter(in_scan, in_2d, "Input", self.COLORS_INPUT)
            )
        if out_2d.size > 0:
            fig.add_trace(
                self.detection_scatter(out_scan, out_2d, "Output", self.COLORS_OUTPUT)
            )
        fig.update_layout(
            title=f"{sensor_name} — {signal_name}",
            xaxis_title="Scan Index",
            yaxis_title=signal_name,
            template=self._template,
            hovermode="closest",
            legend=dict(
                orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1
            ),
            margin=dict(l=50, r=20, t=60, b=45),
            height=400,
        )
        return self._div(fig)

    def match_line_plot(
        self,
        scan_index: np.ndarray,
        pct_arrays: Dict[str, np.ndarray],
        title: str,
    ) -> str:
        """Line plot: X = scan index, Y = match % for each signal."""
        colors = {
            "Overall": "rgba(231,76,60,1)",
            "DET_RANGE": "rgba(31,119,180,1)",
            "DET_RANGE_VELOCITY": "rgba(255,127,14,1)",
            "DET_AZIMUTH": "rgba(44,160,44,1)",
            "DET_ELEVATION": "rgba(148,103,189,1)",
        }
        friendly = {
            "Overall": "All Params",
            "DET_RANGE": "Range",
            "DET_RANGE_VELOCITY": "Range Velocity",
            "DET_AZIMUTH": "Azimuth",
            "DET_ELEVATION": "Elevation",
        }
        fig = go.Figure()
        x = scan_index.tolist()
        for key, pct in pct_arrays.items():
            fig.add_trace(
                go.Scattergl(
                    x=x,
                    y=pct.astype(float).tolist(),
                    mode="lines+markers",
                    marker=dict(size=3, color=colors.get(key, "gray")),
                    line=dict(color=colors.get(key, "gray"), width=1.5),
                    name=friendly.get(key, key),
                )
            )
        fig.update_layout(
            title=title,
            xaxis_title="Scan Index",
            yaxis_title="Match %",
            yaxis=dict(range=[0, 105]),
            template=self._template,
            hovermode="closest",
            legend=dict(
                orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1
            ),
            margin=dict(l=50, r=20, t=60, b=45),
            height=400,
        )
        return self._div(fig)

    def kpi_metrics_plot(
        self,
        scan_index: np.ndarray,
        metric_arrays: Dict[str, np.ndarray],
        title: str,
    ) -> str:
        colors = {
            "Overall": "rgba(231,76,60,1)",
            "Precision": "rgba(52,152,219,1)",
            "Recall": "rgba(46,204,113,1)",
            "F1": "rgba(155,89,182,1)",
            "Accuracy": "rgba(241,196,15,1)",
        }
        fig = go.Figure()
        x = scan_index.tolist()
        for key, pct in metric_arrays.items():
            fig.add_trace(
                go.Scattergl(
                    x=x,
                    y=pct.astype(float).tolist(),
                    mode="lines+markers",
                    marker=dict(size=3, color=colors.get(key, "gray")),
                    line=dict(color=colors.get(key, "gray"), width=1.5),
                    name=key,
                )
            )
        fig.update_layout(
            title=title,
            xaxis_title="Aligned Scan Index",
            yaxis_title="Score %",
            yaxis=dict(range=[0, 105]),
            template=self._template,
            hovermode="closest",
            legend=dict(
                orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1
            ),
            margin=dict(l=50, r=20, t=60, b=45),
            height=420,
        )
        return self._div(fig)

    def build_sensor_tab(
        self,
        sensor_name: str,
        scan_index: np.ndarray,
        metrics: Dict[str, np.ndarray],
        per_signal: Dict[str, np.ndarray],
    ) -> str:
        if len(scan_index) == 0:
            return f"<h2>{sensor_name}</h2><p><em>No aligned scans available.</em></p>"

        parts: List[str] = [f"<h2>{sensor_name} — KPI Metrics</h2>"]
        parts.append(
            f'<div class="plot-card">{self.kpi_metrics_plot(scan_index, metrics, f"{sensor_name} — Overall KPIs")}</div>'
        )

        if per_signal:
            parts.append(
                f'<div class="plot-card">{self.match_line_plot(scan_index, per_signal, f"{sensor_name} — Signal Match %")}</div>'
            )

        return "\n".join(parts)

    def build_status_tab(
        self,
        sensor_name: str,
        messages: List[str],
        tone: str = "warning",
    ) -> str:
        parts: List[str] = [f"<h2>{sensor_name}</h2>"]
        parts.append(self.notice_block(f"{sensor_name} Status", messages, tone=tone))
        return "\n".join(parts)

    def build_kpi_tab(
        self,
        summary_table_html: str,
        overall_plot_html: str,
    ) -> str:
        parts = ["<h2>KPI Summary</h2>"]
        if overall_plot_html:
            parts.append(f'<div class="plot-card">{overall_plot_html}</div>')
        if summary_table_html:
            parts.append(summary_table_html)
        return "\n".join(parts)

    def match_all_radars_plot(
        self,
        radar_data: Dict[str, tuple],
        title: str,
    ) -> str:
        """Combined line plot of overall match % for all radars on one chart."""
        color_map = {
            "SRR / FL": "green",
            "FLR": "red",
            "SRR / FR": "blue",
            "SRR / RL": "violet",
            "SRR / RR": "black",
        }
        fig = go.Figure()
        for sensor_name, (scan_idx, pct) in radar_data.items():
            if len(scan_idx) == 0:
                continue
            fig.add_trace(
                go.Scattergl(
                    x=scan_idx.tolist(),
                    y=pct.astype(float).tolist(),
                    mode="lines+markers",
                    marker=dict(size=4, color=color_map.get(sensor_name, "gray")),
                    line=dict(color=color_map.get(sensor_name, "gray"), width=1.5),
                    name=sensor_name,
                )
            )
        fig.update_layout(
            title=title,
            xaxis_title="Scan Index",
            yaxis_title="Match %",
            yaxis=dict(range=[0, 105]),
            template=self._template,
            hovermode="closest",
            legend=dict(
                orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1
            ),
            margin=dict(l=50, r=20, t=60, b=45),
            height=400,
        )
        return self._div(fig)

    def index_accuracy_plot(
        self,
        log_names: List[str],
        avg_accuracy: List[float],
        title: str,
    ) -> str:
        if not log_names or not avg_accuracy:
            return ""

        def extract_short_name(name: str) -> str:
            import re
            match = re.search(r"_(\d{4})_(?:HDF|PreDetList)", name)
            if match:
                return match.group(1)
            parts = name.split("_")
            if len(parts) >= 2 and parts[-2].isdigit() and len(parts[-2]) == 4:
                return parts[-2]
            return name[-4:] if len(name) >= 4 else name

        paired = list(zip(log_names, avg_accuracy))
        paired.sort(key=lambda x: extract_short_name(x[0]))

        short_names = [extract_short_name(name) for name, _ in paired]
        sorted_accuracy = [acc for _, acc in paired]

        fig = go.Figure()
        fig.add_trace(
            go.Scatter(
                x=short_names,
                y=sorted_accuracy,
                mode="lines+markers",
                line=dict(color="rgba(13,106,139,1)", width=2),
                marker=dict(size=8, color="rgba(13,106,139,1)", symbol="circle"),
                text=[f"{value:.2f}%" for value in sorted_accuracy],
                textposition="top center",
                hovertemplate="Log: %{x}<br>Avg Accuracy: %{y:.2f}%<extra></extra>",
                name="Avg Accuracy",
            )
        )
        fig.update_layout(
            title=title,
            xaxis_title="Log ID (last 4 digits)",
            yaxis_title="Average Accuracy %",
            yaxis=dict(range=[0, 105]),
            template=self._template,
            hovermode="closest",
            margin=dict(l=50, r=20, t=60, b=60),
            height=420,
        )
        return self._div(fig)

    def stats_table(self, title: str, headers: List[str], rows: List[List[str]]) -> str:
        th = "".join(f"<th>{h}</th>" for h in headers)
        trs = []
        for r in rows:
            tds = "".join(f"<td>{c}</td>" for c in r)
            trs.append(f"<tr>{tds}</tr>")
        return (
            f"<h3>{title}</h3>"
            f'<div class="scroll"><table><thead><tr>{th}</tr></thead>'
            f"<tbody>{''.join(trs)}</tbody></table></div>"
        )

    def notice_block(
        self,
        title: str,
        messages: List[str],
        tone: str = "warning",
    ) -> str:
        if not messages:
            return ""

        tone_cls = tone if tone in {"warning", "error", "info"} else "info"
        items = "".join(f"<li>{message}</li>" for message in messages)
        return (
            f'<section class="notice-block {tone_cls}">' 
            f"<h3>{title}</h3>"
            f"<ul>{items}</ul>"
            "</section>"
        )

    def build_tabbed_html(
        self,
        sensor_tabs: Dict[str, str],
        title: str = "CAN Detection KPI",
        summary_html: str = "",
    ) -> str:
        tab_buttons: List[str] = []
        tab_contents: List[str] = []
        for i, (tab_name, content) in enumerate(sensor_tabs.items()):
            active_cls = " active" if i == 0 else ""
            display = "block" if i == 0 else "none"
            safe_id = tab_name.replace("/", "_").replace(" ", "_")
            tab_buttons.append(
                f'<button class="tab-btn{active_cls}" '
                f"onclick=\"switchTab('{safe_id}', this)\">{tab_name}</button>"
            )
            tab_contents.append(
                f'<div id="tab_{safe_id}" class="tab-content" style="display:{display}">'
                f"{content}</div>"
            )

        parts = [
            "<!DOCTYPE html>",
            "<html><head>",
            '<meta charset="utf-8"/>',
            '<meta name="viewport" content="width=device-width, initial-scale=1"/>',
            f"<title>{title}</title>",
            '<link rel="preconnect" href="https://cdn.plot.ly">',
            '<script src="https://cdn.plot.ly/plotly-3.0.1.min.js"></script>',
            "<style>",
            self._css(),
            "</style>",
            "</head><body>",
            f"<h1>{title}</h1>",
        ]

        if summary_html:
            parts.append('<div class="summary-section">')
            parts.append(summary_html)
            parts.append("</div>")

        parts.append('<div class="tab-bar">')
        parts.extend(tab_buttons)
        parts.append("</div>")
        parts.extend(tab_contents)

        parts.append("<script>")
        parts.append(self._tab_js())
        parts.append("</script>")
        parts.append("</body></html>")
        return "\n".join(parts)

    def _css(self) -> str:
        return """
:root {
    --page-bg: #f2f7fb;
    --panel-bg: rgba(255,255,255,0.94);
    --panel-border: #d9e5ee;
    --text-main: #17324a;
    --text-muted: #627789;
    --accent: #0d6a8b;
    --accent-soft: #dceef5;
    --shadow: 0 10px 22px rgba(18, 47, 72, 0.08);
}
html, body { width: 100%; max-width: 100%; overflow-x: hidden; }
* { box-sizing: border-box; }
body {
    font-family: Segoe UI, Arial, sans-serif;
    margin: 0;
    padding: 18px;
    background: linear-gradient(180deg, #f8fbfd 0%, var(--page-bg) 100%);
    color: var(--text-main);
}
h1 { margin: 0 0 12px 0; color: var(--text-main); }
h2 { color: var(--text-main); margin: 18px 0 8px 0; }
h3 { color: var(--text-main); margin: 12px 0 6px 0; }

.tab-bar {
    display: flex; flex-wrap: wrap; gap: 0; border-bottom: 2px solid #b9d7e6;
    margin-bottom: 16px; background: rgba(255,255,255,0.92); border-radius: 14px 14px 0 0;
    box-shadow: var(--shadow);
}
.tab-btn {
    padding: 10px 22px; border: none; background: #edf5f9; cursor: pointer;
    font-size: 14px; font-weight: 600; color: var(--text-muted); transition: background 0.16s ease, color 0.16s ease;
    border-radius: 14px 14px 0 0; margin-right: 2px;
}
.tab-btn:hover { background: #dcecf4; color: var(--text-main); }
.tab-btn.active { background: var(--accent); color: #fff; }

.tab-content { display: none; }

.plot-card {
    background: var(--panel-bg); border-radius: 16px; padding: 12px; margin: 10px 0;
    box-shadow: var(--shadow); border: 1px solid var(--panel-border);
    content-visibility: auto; contain-intrinsic-size: 420px;
}
.plots-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 12px; }
@media (max-width: 1200px) { .plots-grid { grid-template-columns: 1fr; } }

.summary-section {
    background: var(--panel-bg); border-radius: 16px; padding: 16px; margin-bottom: 16px;
    box-shadow: var(--shadow); border: 1px solid var(--panel-border);
}
.notice-block {
    border-radius: 14px; padding: 12px 14px; margin: 0 0 12px 0;
    border: 1px solid transparent;
}
.notice-block h3 { margin: 0 0 8px 0; }
.notice-block ul { margin: 0; padding-left: 18px; }
.notice-block li { margin: 4px 0; }
.notice-block.warning { background: #fff6e5; border-color: #f0cf82; color: #7a5412; }
.notice-block.error { background: #fff0ef; border-color: #e8b2ac; color: #8b3127; }
.notice-block.info { background: #eef7ff; border-color: #bfd6ea; color: #214968; }
.tables-container { display: flex; flex-wrap: wrap; gap: 12px; }
.table-wrapper { flex: 1 1 300px; max-width: 48%; }
@media (max-width: 900px) {
    body { padding: 10px; }
    .table-wrapper { max-width: 100%; flex-basis: 100%; }
}
.scroll { max-height: 300px; overflow: auto; }
table { border-collapse: collapse; width: 100%; font-size: 12px; background: #fff; }
th, td { border: 1px solid #d7e1e9; padding: 6px 8px; text-align: right; }
th { background: #edf5fa; position: sticky; top: 0; color: var(--text-main); font-weight: 700; }
tr:nth-child(even) { background: #f8fbfd; }
tr:hover { background: #eef6fa; }
"""

    def _tab_js(self) -> str:
        return """
function switchTab(tabId, btn) {
    document.querySelectorAll('.tab-content').forEach(c => c.style.display = 'none');
    document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
    document.getElementById('tab_' + tabId).style.display = 'block';
    btn.classList.add('active');
    var plots = document.getElementById('tab_' + tabId).querySelectorAll('.js-plotly-plot');
    plots.forEach(function(p) { Plotly.Plots.resize(p); });
}
"""

    def _div(self, fig) -> str:
        fig.update_layout(template=self._template)
        config = {"responsive": True, "displayModeBar": False, "scrollZoom": False}
        return plot(fig, include_plotlyjs=False, output_type="div", config=config)
