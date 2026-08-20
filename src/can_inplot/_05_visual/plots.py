"""Interactive Plotly figure builders.

Purpose: create self-contained interactive figures for radar telemetry:
detection point clouds, range-Doppler maps, tracklet confidence curves and
latency profiles.
Inputs : numpy arrays of detections/tracks/latencies.
Outputs: plotly Figure objects (rendered to HTML by callers).
"""

import logging
from typing import Any, Optional

import numpy as np

logger = logging.getLogger(__name__)

try:
    import plotly.graph_objects as go
    import plotly.io as pio

    _HAS_PLOTLY = True
except Exception as exc:  # pragma: no cover
    _HAS_PLOTLY = False
    logger.warning("plotly unavailable: %s", exc)
    go = None
    pio = None

_TEMPLATE = "plotly_white"


def _fig(go_module) -> Any:
    return go_module.Figure()


def make_point_cloud_fig(
    ranges: np.ndarray,
    azimuths: np.ndarray,
    elevations: Optional[np.ndarray] = None,
    colors: Optional[np.ndarray] = None,
    title: str = "Detection Point Cloud",
) -> Any:
    """Purpose: 2D/3D detection point cloud.
    Inputs : range, azimuth, elevation, optional color values.
    Outputs: plotly figure."""
    if not _HAS_PLOTLY:
        return None
    x = ranges * np.cos(azimuths)
    y = ranges * np.sin(azimuths)
    if colors is None:
        colors = ranges
    if elevations is not None:
        fig = go.Figure(
            go.Scatter3d(
                x=x, y=y, z=elevations, mode="markers",
                marker=dict(size=3, color=colors, colorscale="Viridis"),
                name="detections",
            )
        )
        fig.update_layout(scene=dict(xaxis_title="X (m)", yaxis_title="Y (m)", zaxis_title="El (rad)"))
    else:
        fig = go.Figure(
            go.Scatter(
                x=x, y=y, mode="markers",
                marker=dict(size=5, color=colors, colorscale="Viridis", opacity=0.8),
                name="detections",
            )
        )
        fig.update_layout(xaxis_title="X (m)", yaxis_title="Y (m)")
    fig.update_layout(title=title, template=_TEMPLATE, height=520)
    return fig


def make_range_doppler_fig(
    ranges: np.ndarray,
    velocities: np.ndarray,
    colors: Optional[np.ndarray] = None,
    title: str = "Range-Doppler Map",
) -> Any:
    """Purpose: range-velocity scatter (range-Doppler map).
    Inputs : range and velocity arrays, optional color values.
    Outputs: plotly figure."""
    if not _HAS_PLOTLY:
        return None
    if colors is None:
        colors = velocities
    fig = go.Figure(
        go.Scatter(
            x=ranges, y=velocities, mode="markers",
            marker=dict(size=6, color=colors, colorscale="Turbo", opacity=0.85),
            name="cells",
        )
    )
    fig.update_layout(
        title=title, template=_TEMPLATE, height=480,
        xaxis_title="Range (m)", yaxis_title="Range rate (m/s)",
    )
    return fig


def make_tracklet_confidence_fig(
    scan_ids: np.ndarray,
    confidences: np.ndarray,
    title: str = "Tracklet Confidence",
) -> Any:
    """Purpose: confidence evolution per track over scans.
    Inputs : scan id array and confidence array.
    Outputs: plotly figure."""
    if not _HAS_PLOTLY:
        return None
    fig = go.Figure(
        go.Scatter(
            x=scan_ids, y=confidences, mode="lines+markers",
            line=dict(color="#1f77b4", width=2), name="confidence",
        )
    )
    fig.update_layout(
        title=title, template=_TEMPLATE, height=380,
        xaxis_title="Scan", yaxis_title="Confidence", yaxis=dict(range=[0, 1]),
    )
    return fig


def make_latency_profile_fig(
    scan_ids: np.ndarray,
    latencies_ms: np.ndarray,
    title: str = "Latency Profile (Input → Output)",
) -> Any:
    """Purpose: latency over scans with p95/p99 bands.
    Inputs : scan ids and per-scan latencies in ms.
    Outputs: plotly figure."""
    if not _HAS_PLOTLY:
        return None
    fig = go.Figure()
    fig.add_trace(
        go.Scatter(
            x=scan_ids, y=latencies_ms, mode="lines",
            line=dict(color="#2ca02c", width=1.5), name="latency",
        )
    )
    p95 = float(np.percentile(latencies_ms, 95)) if len(latencies_ms) else 0.0
    p99 = float(np.percentile(latencies_ms, 99)) if len(latencies_ms) else 0.0
    fig.add_hline(y=p95, line_dash="dash", line_color="orange", annotation_text=f"p95={p95:.1f} ms")
    fig.add_hline(y=p99, line_dash="dash", line_color="red", annotation_text=f"p99={p99:.1f} ms")
    fig.update_layout(
        title=title, template=_TEMPLATE, height=380,
        xaxis_title="Scan", yaxis_title="Latency (ms)",
    )
    return fig


def make_divergence_fig(
    scan_ids: np.ndarray,
    js_values: np.ndarray,
    threshold: float = 0.20,
    title: str = "JS Divergence Gate over Scans",
) -> Any:
    """Purpose: JS divergence gate activity over scans (false-alarm flagging).
    Inputs : scan ids, JS values, gate threshold.
    Outputs: plotly figure."""
    if not _HAS_PLOTLY:
        return None
    fig = go.Figure()
    fig.add_trace(
        go.Scatter(
            x=scan_ids, y=js_values, mode="lines+markers",
            line=dict(color="#9467bd", width=2), name="JS divergence",
        )
    )
    fig.add_hline(
        y=threshold, line_dash="dot", line_color="crimson",
        annotation_text=f"gate τ={threshold}",
    )
    fig.update_layout(
        title=title, template=_TEMPLATE, height=380,
        xaxis_title="Scan", yaxis_title="D_JS",
    )
    return fig