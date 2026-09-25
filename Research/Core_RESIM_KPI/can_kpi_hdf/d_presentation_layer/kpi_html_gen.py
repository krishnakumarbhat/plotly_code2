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
html, body { width: 100%; max-width: 100%; overflow-x: hidden; }
body { font-family: Segoe UI, Arial, sans-serif; margin: 0; padding: 16px; background: #f5f6fa; }
h1 { margin: 0 0 12px 0; color: #2c3e50; }
h2 { color: #34495e; margin: 18px 0 8px 0; }
h3 { color: #2c3e50; margin: 12px 0 6px 0; }

.tab-bar {
    display: flex; flex-wrap: wrap; gap: 0; border-bottom: 2px solid #3498db;
    margin-bottom: 16px; background: #fff; border-radius: 8px 8px 0 0;
    box-shadow: 0 1px 3px rgba(0,0,0,0.08);
}
.tab-btn {
    padding: 10px 22px; border: none; background: #ecf0f1; cursor: pointer;
    font-size: 14px; font-weight: 600; color: #7f8c8d; transition: all 0.2s;
    border-radius: 8px 8px 0 0; margin-right: 2px;
}
.tab-btn:hover { background: #d5dbdb; color: #2c3e50; }
.tab-btn.active { background: #3498db; color: #fff; }

.tab-content { display: none; }

.plot-card {
    background: #fff; border-radius: 8px; padding: 12px; margin: 10px 0;
    box-shadow: 0 1px 4px rgba(0,0,0,0.06); border: 1px solid #e8ecef;
}
.plots-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 12px; }
@media (max-width: 1200px) { .plots-grid { grid-template-columns: 1fr; } }

.summary-section {
    background: #fff; border-radius: 8px; padding: 16px; margin-bottom: 16px;
    box-shadow: 0 1px 4px rgba(0,0,0,0.06); border: 1px solid #e8ecef;
}
.tables-container { display: flex; flex-wrap: wrap; gap: 12px; }
.table-wrapper { flex: 1 1 300px; max-width: 48%; }
@media (max-width: 900px) {
    body { padding: 10px; }
    .table-wrapper { max-width: 100%; flex-basis: 100%; }
}
.scroll { max-height: 300px; overflow: auto; }
table { border-collapse: collapse; width: 100%; font-size: 12px; }
th, td { border: 1px solid #bdc3c7; padding: 5px 8px; text-align: right; }
th { background: #3498db; position: sticky; top: 0; color: #fff; font-weight: 600; }
tr:nth-child(even) { background: #f8f9fa; }
tr:hover { background: #eaf2f8; }
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
        config = {"responsive": True, "displayModeBar": True, "scrollZoom": True}
        return plot(fig, include_plotlyjs=False, output_type="div", config=config)
