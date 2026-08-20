"""Layer 05_visual — interactive plotting and HTML canvas generation.

Purpose: generate interactive Plotly/WebGL figures (point clouds, range-Doppler
maps, tracklet confidence, latency profiles) and assemble HTML reports.
Inputs : metric arrays, detection streams, tracking results.
Outputs: standalone HTML fragments and full report pages.
"""

from can_inplot._05_visual.plots import (
    make_point_cloud_fig,
    make_range_doppler_fig,
    make_tracklet_confidence_fig,
    make_latency_profile_fig,
    make_divergence_fig,
)
from can_inplot._05_visual.html_gen import (
    KpiHtmlGen,
    CanKpiEngine,
    html_from_fig,
)
from can_inplot._05_visual.report import build_overnight_report

__all__ = [
    "make_point_cloud_fig",
    "make_range_doppler_fig",
    "make_tracklet_confidence_fig",
    "make_latency_profile_fig",
    "make_divergence_fig",
    "KpiHtmlGen",
    "CanKpiEngine",
    "html_from_fig",
    "build_overnight_report",
]