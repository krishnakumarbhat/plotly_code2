"""
plan_view_widget.py
===================
Matplotlib canvas showing a top-down (plan) view of detected radar objects
in the Vehicle Coordinate System (VCS).

VCS convention:
  - X axis = Longitudinal (forward = positive)
  - Y axis = Lateral      (right   = positive)
  - Origin = front bumper centre
  - Plot: lateral → X-axis, longitudinal → Y-axis

Mouse controls:
  - Left-click drag (no toolbar active) : rubber-band zoom
  - Scroll wheel                         : zoom in / out centred on cursor
  - Double-click on object               : open detail dialog (emits signal)
"""

from __future__ import annotations

from typing import Dict, List, Optional

import numpy as np
from matplotlib.backends.backend_qtagg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.patches import Polygon, Rectangle as MplRectangle, Circle
from matplotlib.lines import Line2D
from PyQt6.QtCore import pyqtSignal

from data_model import DataModel, ObjectRecord, _REF_OFFSET
from view_settings import SETTINGS, CLASS_LABELS, _DYN_LABELS
_THEME_DARK = dict(
    ax_bg="#1a1a2e", fig_bg="#12122a",
    tick="white", spine="#444466",
    grid="#2a2a4a", title="white",
    legend_fc="#1a1a2e", legend_ec="#444466", legend_lc="white",
)
_THEME_LIGHT = dict(
    ax_bg="#ffffff", fig_bg="#f5f5f5",
    tick="#111111", spine="#aaaaaa",
    grid="#dddddd", title="#111111",
    legend_fc="#ffffff", legend_ec="#aaaaaa", legend_lc="#111111",
)

# Truck body in VCS  (length 8 m, width 2.6 m, rear at -8 m, front bumper at 0)
_TRUCK_LON = [-8.0, 0.0]
_TRUCK_WIDTH = 2.6

# Default axis limits
_DEF_LAT_MIN = -40.0
_DEF_LAT_MAX =  40.0
_DEF_LON_MIN = -60.0
_DEF_LON_MAX =  60.0

# Velocity vector scale: 1 m/s → 1 m arrow (1-second lookahead)
_VEL_SCALE = 1.0


# Ref-point values that indicate unknown/error position
_REF_PT_INVALID = {8, 14, 15}

# ---------------------------------------------------------------------------
# Dual-source colour palettes
# ---------------------------------------------------------------------------
# When BOTH original and resim data are loaded the two sources get distinct
# hue families so they are unmistakable at a glance:
#   Original  →  greens
#   Resim     →  reds
# Unknown / Error / N/A use grey tones for both sources.
# When only original data is loaded the default SETTINGS palette is used.
# ---------------------------------------------------------------------------

# --- Original (dual-source mode) – greens for active states, greys for inactive ---
_ORIG_DUAL_DYN: Dict[int, str] = {
    0: "#9e9e9e",   # Invalid    → medium grey
    1: "#bdbdbd",   # Stationary → light grey
    2: "#66bb6a",   # Stopped    → medium green
    3: "#2e7d32",   # Moving     → dark green
    6: "#757575",   # Error      → dark grey
    7: "#e0e0e0",   # N/A        → very light grey
}
_ORIG_DUAL_DYN_FALLBACK = "#66bb6a"

# Ordering: a (lightest/most transparent) → f (darkest/least transparent)
# a) Unknown  b) Truck  c) Car  d) Motorbike  e) Bicycle  f) Pedestrian
# Error (6) and N/A (7) are always grey in class color mode.
_ORIG_DUAL_CLASS: Dict[int, str] = {
    0: "#c8e6c9",   # Unknown    → green 100 (lightest)
    3: "#81c784",   # Truck      → green 300
    1: "#4caf50",   # Car        → green 500
    2: "#388e3c",   # Motorbike  → green 700
    4: "#2e7d32",   # Bicycle    → green 800
    5: "#1b5e20",   # Pedestrian → green 900 (darkest)
    6: "#757575",   # Error      → dark grey
    7: "#e0e0e0",   # N/A        → light grey
}
_ORIG_DUAL_CLASS_FALLBACK = "#4caf50"

# --- Resim (dual-source mode) – reds for active states, greys for inactive ---
_RESIM_DYN: Dict[int, str] = {
    0: "#9e9e9e",   # Invalid    → medium grey
    1: "#bdbdbd",   # Stationary → light grey
    2: "#ef5350",   # Stopped    → medium red
    3: "#b71c1c",   # Moving     → dark red
    6: "#757575",   # Error      → dark grey
    7: "#e0e0e0",   # N/A        → very light grey
}
_RESIM_DYN_FALLBACK = "#ef5350"

_RESIM_CLASS: Dict[int, str] = {
    0: "#ffcdd2",   # Unknown    → red 100 (lightest)
    3: "#ef9a9a",   # Truck      → red 200
    1: "#e57373",   # Car        → red 300
    2: "#ef5350",   # Motorbike  → red 400
    4: "#e53935",   # Bicycle    → red 600
    5: "#b71c1c",   # Pedestrian → red 800 (darkest)
    6: "#757575",   # Error      → dark grey
    7: "#e0e0e0",   # N/A        → light grey
}
_RESIM_CLASS_FALLBACK = "#e57373"

_RESIM_LW_EXTRA = 0.8   # extra linewidth for resim boxes
_RESIM_LINESTYLE = (0, (4, 3))  # dotted pattern: 4 pt on, 3 pt off


class PlanViewWidget(FigureCanvas):
    """Embeddable matplotlib plan-view canvas."""

    object_double_clicked    = pyqtSignal(object)   # ObjectRecord
    object_clicked           = pyqtSignal(object)   # ObjectRecord (single click)
    object_selection_cleared = pyqtSignal()          # click on empty space

    def __init__(self, parent=None):
        self._fig = Figure(figsize=(6, 9))
        self._fig.subplots_adjust(left=0.07, right=0.98, top=0.96, bottom=0.04)
        super().__init__(self._fig)
        self.setParent(parent)

        self._ax = self._fig.add_subplot(111)

        # Stored state
        self._objects: List[ObjectRecord] = []             # original
        self._resim_objects: List[ObjectRecord] = []       # resim
        self._current_scan: Optional[int] = None
        self._current_host_data: Optional[Dict[str, float]] = None

        # Axis limits
        self._lat_min = _DEF_LAT_MIN
        self._lat_max = _DEF_LAT_MAX
        self._lon_min = _DEF_LON_MIN
        self._lon_max = _DEF_LON_MAX

        # Display toggles
        self._show_ids = False
        self._show_velocity = True
        self._show_heading = False
        self._keep_ratio = True  # enforce 1:1 aspect (equal increments on both axes)

        # Zoom mode: rubber-band and scroll only active when enabled
        self._zoom_mode = False

        # Reference to matplotlib NavigationToolbar (set by container)
        self._nav_toolbar = None

        self._in_managed_draw = False   # True only while _rebuild_background draws
        self._fig.canvas.mpl_connect("draw_event", self._on_canvas_drawn)

        # Rubber-band zoom state
        self._rb_start: Optional[tuple] = None
        self._rb_patch: Optional[MplRectangle] = None
        # Single-click tracking (non-zoom mode)
        self._click_start: Optional[tuple] = None
        # Selected object highlight (side, tracking_id)
        self._selected_key: Optional[tuple] = None
        # Jump-to-frame highlights: frozenset of (side, tracking_id, data_source)
        self._jump_highlights: frozenset = frozenset()

        # True when both original and resim files are loaded; set via
        # set_dual_mode() at file-load/unload time.  Governs color palette
        # for the entire session — never toggled per-frame.
        self._dual_mode: bool = False

        # Blit background cache – invalidated whenever the static scene changes
        self._bg_cache = None   # None means "needs rebuild"

        # Artists added in the blit layer; removed at the start of the next frame
        # to prevent unbounded accumulation in ax.patches / ax.lines / ax.texts.
        self._dynamic_artists: List = []

        # Connect events
        self._fig.canvas.mpl_connect("button_press_event",   self._on_press)
        self._fig.canvas.mpl_connect("button_release_event", self._on_release)
        self._fig.canvas.mpl_connect("motion_notify_event",  self._on_motion)
        self._fig.canvas.mpl_connect("scroll_event",         self._on_scroll)

        self._setup_axes()
        self._draw_vehicle()
        self._ax.set_xlim(self._lat_max, self._lat_min)
        self._ax.set_ylim(self._lon_min, self._lon_max)
        self._ax.autoscale(False)
        self._ax.set_aspect("equal", adjustable="box")
        self.draw()

    def resizeEvent(self, event):   # noqa: N802
        """Invalidate blit cache on canvas resize (layout change)."""
        self._bg_cache = None
        super().resizeEvent(event)

    # ------------------------------------------------------------------
    # Configuration API
    # ------------------------------------------------------------------

    def set_axis_limits(self, lat_min: float, lat_max: float,
                        lon_min: float, lon_max: float) -> None:
        self._lat_min = lat_min
        self._lat_max = lat_max
        self._lon_min = lon_min
        self._lon_max = lon_max
        self._bg_cache = None   # axis limits changed → rebuild background

    def set_show_ids(self, show: bool) -> None:
        self._show_ids = show
        self._bg_cache = None

    def set_show_velocity(self, show: bool) -> None:
        self._show_velocity = show
        self._bg_cache = None

    def set_show_heading(self, show: bool) -> None:
        self._show_heading = show
        self._bg_cache = None

    def set_keep_ratio(self, keep: bool) -> None:
        self._keep_ratio = keep
        self._bg_cache = None

    def set_zoom_mode(self, enabled: bool) -> None:
        self._zoom_mode = enabled

    def set_selected_object(self, side: str, tracking_id: int,
                             data_source: str = "Original") -> None:
        """Highlight the object with the given (side, tracking_id, data_source)."""
        self._selected_key = (side, tracking_id, data_source)
        # selection only changes the dynamic layer – no bg rebuild needed

    def clear_selected_object(self) -> None:
        self._selected_key = None

    def set_jump_highlights(self, keys) -> None:
        """Highlight a set of objects with a cyan halo (jump-to-frame).
        keys: iterable of (side, tracking_id, data_source, scan_start, scan_end) tuples."""
        self._jump_highlights = frozenset(keys)

    def clear_jump_highlights(self) -> None:
        self._jump_highlights = frozenset()

    def set_theme(self, theme: str) -> None:
        """Switch matplotlib colour theme ('dark' or 'light') and redraw."""
        SETTINGS.theme = theme
        self._bg_cache = None
        self._setup_axes()
        self._draw_vehicle()
        self.draw()

    def set_model(self, model: DataModel) -> None:
        self._bg_cache = None
        self.clear_view()

    # ------------------------------------------------------------------
    # Axes setup
    # ------------------------------------------------------------------

    def _setup_axes(self) -> None:
        t = _THEME_LIGHT if SETTINGS.theme == "light" else _THEME_DARK
        ax = self._ax
        ax.set_facecolor(t["ax_bg"])
        self._fig.patch.set_facecolor(t["fig_bg"])
        ax.tick_params(colors=t["tick"])
        ax.xaxis.label.set_color(t["tick"])
        ax.yaxis.label.set_color(t["tick"])
        ax.title.set_color(t["title"])
        for spine in ax.spines.values():
            spine.set_edgecolor(t["spine"])
        ax.grid(True, color=t["grid"], linewidth=0.5, linestyle="--")

    # ------------------------------------------------------------------
    # Public drawing API
    # ------------------------------------------------------------------

    def set_dual_mode(self, enabled: bool) -> None:
        """Switch between single-source and dual-source (original + resim) color
        palettes.  Call when a resim file is loaded or unloaded.  Invalidates
        the background cache so the legend is redrawn on the next frame."""
        if self._dual_mode != enabled:
            self._dual_mode = enabled
            self._bg_cache = None

    def invalidate_background(self) -> None:
        """Force a full background rebuild on the next frame render."""
        self._bg_cache = None

    def _rebuild_background(self, scan_idx: int) -> None:
        """Full redraw of the static layer; saves pixel buffer for blitting."""
        ax = self._ax
        ax.cla()
        self._setup_axes()
        self._draw_vehicle()

        ax.set_xlim(self._lat_max, self._lat_min)
        ax.set_ylim(self._lon_min, self._lon_max)
        ax.autoscale(False)
        if self._keep_ratio:
            ax.set_aspect("equal", adjustable="box")

        t = _THEME_LIGHT if SETTINGS.theme == "light" else _THEME_DARK
        dual = self._dual_mode   # True when both source files are loaded

        if SETTINGS.class_color_mode:
            if dual:
                # Original: blues
                legend_entries = [
                    Line2D([0], [0],
                           color=_ORIG_DUAL_CLASS.get(k, _ORIG_DUAL_CLASS_FALLBACK),
                           lw=SETTINGS.class_styles.get(k, SETTINGS.dyn_fallback)[1],
                           label=label)
                    for k, label in CLASS_LABELS.items()
                ]
            else:
                legend_entries = [
                    Line2D([0], [0],
                           color=SETTINGS.class_styles.get(k, SETTINGS.dyn_fallback)[0],
                           lw=SETTINGS.class_styles.get(k, SETTINGS.dyn_fallback)[1],
                           label=label)
                    for k, label in CLASS_LABELS.items()
                ]
        else:
            if dual:
                # Original: blues
                legend_entries = [
                    Line2D([0], [0],
                           color=_ORIG_DUAL_DYN.get(k, _ORIG_DUAL_DYN_FALLBACK),
                           lw=SETTINGS.dyn_styles.get(k, SETTINGS.dyn_fallback)[1],
                           label=label)
                    for k, label in _DYN_LABELS.items()
                ]
            else:
                legend_entries = [
                    Line2D([0], [0],
                           color=SETTINGS.dyn_styles.get(k, SETTINGS.dyn_fallback)[0],
                           lw=SETTINGS.dyn_styles.get(k, SETTINGS.dyn_fallback)[1],
                           label=label)
                    for k, label in _DYN_LABELS.items()
                ]
        # Resim legend section (greens) when both sources are loaded
        if dual:
            legend_entries.append(
                Line2D([0], [0], color="none", label="── Resim ──")
            )
            if SETTINGS.class_color_mode:
                for k, label in CLASS_LABELS.items():
                    legend_entries.append(
                        Line2D([0], [0],
                               color=_RESIM_CLASS.get(k, _RESIM_CLASS_FALLBACK),
                               lw=SETTINGS.class_styles.get(k, SETTINGS.dyn_fallback)[1] + _RESIM_LW_EXTRA,
                               label=f"{label} (R)"))
            else:
                for k, label in _DYN_LABELS.items():
                    legend_entries.append(
                        Line2D([0], [0],
                               color=_RESIM_DYN.get(k, _RESIM_DYN_FALLBACK),
                               lw=SETTINGS.dyn_styles.get(k, SETTINGS.dyn_fallback)[1] + _RESIM_LW_EXTRA,
                               label=f"{label} (R)"))
        ax.legend(handles=legend_entries, loc="upper right",
                  facecolor=t["legend_fc"], edgecolor=t["legend_ec"],
                  labelcolor=t["legend_lc"], fontsize=7)

        # Render to Qt and capture the clean pixel buffer
        self._in_managed_draw = True
        try:
            self.draw()
        finally:
            self._in_managed_draw = False
        self._bg_cache = self.copy_from_bbox(ax.bbox)

    def update_frame_with_objects(
        self,
        scan_idx: int,
        objects: List[ObjectRecord],
        host_data: Optional[Dict[str, float]] = None,
        resim_objects: Optional[List[ObjectRecord]] = None,
    ) -> None:
        self._current_scan = scan_idx
        self._objects = list(objects)
        self._resim_objects = list(resim_objects) if resim_objects else []
        self._current_host_data = host_data

        # Rebuild static background when cache is invalid
        if self._bg_cache is None:
            self._rebuild_background(scan_idx)

        # --- fast blit path -------------------------------------------
        ax = self._ax

        # Remove artists left over from the previous frame so ax.patches /
        # ax.lines / ax.texts don't grow unboundedly during playback.
        for art in self._dynamic_artists:
            try:
                art.remove()
            except Exception:
                pass
        self._dynamic_artists = []

        self.restore_region(self._bg_cache)

        # Draw dynamic artists directly onto the cached background
        self._draw_objects_blit(self._objects, ax, resim=False)
        if self._resim_objects:
            self._draw_objects_blit(self._resim_objects, ax, resim=True)

        self.blit(ax.bbox)

    def clear_view(self) -> None:
        self._objects = []
        self._resim_objects = []
        self._current_host_data = None
        ax = self._ax
        ax.cla()
        self._setup_axes()
        self._draw_vehicle()
        ax.set_xlim(self._lat_max, self._lat_min)
        ax.set_ylim(self._lon_min, self._lon_max)
        ax.autoscale(False)
        if self._keep_ratio:
            ax.set_aspect("equal", adjustable="box")
        self.draw()

    # ------------------------------------------------------------------
    # Drawing helpers
    # ------------------------------------------------------------------

    def _draw_vehicle(self) -> None:
        ax = self._ax
        _A  = 0.60
        _EC = "#8888cc"

        # --- Cab: tapered front, lon 0 to -2.8 m ---
        ax.add_patch(Polygon(
            [[-1.0, 0.0], [1.0, 0.0], [1.3, -2.8], [-1.3, -2.8]],
            closed=True, facecolor="#3a3a7e", edgecolor=_EC,
            linewidth=1.2, alpha=_A, zorder=3,
        ))

        # Windscreen: lighter trapezoid inset inside the cab
        ax.add_patch(Polygon(
            [[-0.7, -0.2], [0.7, -0.2], [0.9, -1.3], [-0.9, -1.3]],
            closed=True, facecolor="#6868b8", edgecolor=_EC,
            linewidth=0.6, alpha=0.55, zorder=4,
        ))

        # Left mirror stub
        ax.add_patch(Polygon(
            [[-1.0, -0.3], [-1.35, -0.3], [-1.35, -0.8], [-1.0, -0.8]],
            closed=True, facecolor="#5555aa", edgecolor=_EC,
            linewidth=0.8, alpha=_A, zorder=4,
        ))
        # Right mirror stub
        ax.add_patch(Polygon(
            [[1.0, -0.3], [1.35, -0.3], [1.35, -0.8], [1.0, -0.8]],
            closed=True, facecolor="#5555aa", edgecolor=_EC,
            linewidth=0.8, alpha=_A, zorder=4,
        ))

        # --- Chassis: lon -2.8 to -8.0 m ---
        ax.add_patch(Polygon(
            [[-1.3, -2.8], [1.3, -2.8], [1.3, -8.0], [-1.3, -8.0]],
            closed=True, facecolor="#2a2a5a", edgecolor=_EC,
            linewidth=1.2, alpha=_A, zorder=3,
        ))

        # Front bumper line at origin
        ax.plot([-1.0, 1.0], [0, 0], color=_EC, linewidth=2.0, alpha=_A, zorder=4)

        # VCS cross-hair
        ax.axhline(0, color="#444466", linewidth=0.5, linestyle=":")
        ax.axvline(0, color="#444466", linewidth=0.5, linestyle=":")
    def _draw_objects(self, objects: List[ObjectRecord]) -> None:
        ax = self._ax
        dual = bool(self._resim_objects)
        for obj in objects:
            # Skip objects entirely outside the visible window
            if (obj.ref_lat < self._lat_min or obj.ref_lat > self._lat_max
                    or obj.ref_lon < self._lon_min or obj.ref_lon > self._lon_max):
                continue
            if dual:
                if SETTINGS.class_color_mode:
                    colour = _ORIG_DUAL_CLASS.get(obj.class_id, _ORIG_DUAL_CLASS_FALLBACK)
                    _, lw, alpha = SETTINGS.box_style(obj.dyn_property, obj.class_id)
                else:
                    colour = _ORIG_DUAL_DYN.get(obj.dyn_property, _ORIG_DUAL_DYN_FALLBACK)
                    _, lw, alpha = SETTINGS.dyn_styles.get(obj.dyn_property, SETTINGS.dyn_fallback)
            else:
                colour, lw, alpha = SETTINGS.box_style(obj.dyn_property, obj.class_id)

            # Bounding box – centred on reference point (raw signal position)
            corners = _box_corners(
                obj.ref_lat, obj.ref_lon,
                max(obj.width,  0.3),
                max(obj.length, 0.3),
                obj.heading,
            )
            poly = Polygon(corners, closed=True,
                           facecolor=colour, alpha=alpha,
                           edgecolor=colour, linewidth=lw, zorder=5)
            ax.add_patch(poly)

            # Highlight selected object with yellow dashed outline
            if (self._selected_key is not None
                    and obj.side == self._selected_key[0]
                    and obj.tracking_id == self._selected_key[1]
                    and getattr(obj, '_data_source', 'Original') == self._selected_key[2]):
                outer = _box_corners(
                    obj.ref_lat, obj.ref_lon,
                    max(obj.width,  0.3) + 0.6,
                    max(obj.length, 0.3) + 0.6,
                    obj.heading,
                )
                sel_poly = Polygon(outer, closed=True,
                                   facecolor="none",
                                   edgecolor="#ffff00",
                                   linewidth=2.0,
                                   linestyle="--",
                                   zorder=10)
                ax.add_patch(sel_poly)

            # Reference point dot — only for Moving objects (dyn_property == 3)
            if obj.dyn_property == 3:
                dot_colour = "#f44336" if obj.ref_pt in _REF_PT_INVALID else colour
                dot_lat, dot_lon = _ref_point_pos(obj)
                ax.plot(dot_lat, dot_lon, "o",
                        color=dot_colour, markersize=3, zorder=8, alpha=0.9)

            # Heading arrow from reference point
            if self._show_heading:
                arrow_len = min(max(obj.length, 0.5) * 0.8, 5.0)
                dlat = arrow_len * np.sin(obj.heading)
                dlon = arrow_len * np.cos(obj.heading)
                ax.annotate("",
                            xy=(obj.ref_lat + dlat, obj.ref_lon + dlon),
                            xytext=(obj.ref_lat, obj.ref_lon),
                            arrowprops=dict(arrowstyle="->", color=colour,
                                            lw=min(lw, 1.5)),
                            zorder=6)

            # Velocity vector from reference point
            if self._show_velocity:
                spd = np.hypot(obj.lon_vel, obj.lat_vel)
                if spd > 0.1:
                    vel_colour, vel_alpha = SETTINGS.vel_style(obj.dyn_property)
                    ax.annotate(
                        "",
                        xy=(obj.ref_lat + obj.lat_vel * _VEL_SCALE,
                            obj.ref_lon + obj.lon_vel * _VEL_SCALE),
                        xytext=(obj.ref_lat, obj.ref_lon),
                        arrowprops=dict(arrowstyle="->", color=vel_colour,
                                        lw=0.8, alpha=vel_alpha),
                        zorder=7,
                    )

            # ID label (Moving / Stopped only) — anchored outside front-right corner
            if self._show_ids and obj.dyn_property in (2, 3):
                label = f"{'L' if obj.side == 'L' else 'R'}:{obj.tracking_id}"
                _hl = max(obj.length, 0.3) / 2
                _hw = max(obj.width,  0.3) / 2
                _ch, _sh = np.cos(obj.heading), np.sin(obj.heading)
                _dlat = _sh * _hl + _ch * _hw   # vector to front-right corner
                _dlon = _ch * _hl - _sh * _hw
                _nm   = max(np.hypot(_dlat, _dlon), 1e-6)
                _off  = 0.5   # metres beyond the corner
                _lbl_lat = obj.ref_lat + _dlat + _off * _dlat / _nm
                _lbl_lon = obj.ref_lon + _dlon + _off * _dlon / _nm
                ax.text(_lbl_lat, _lbl_lon, label,
                        color=SETTINGS.id_colour,
                        alpha=SETTINGS.id_alpha,
                        fontsize=7,
                        ha="center", va="center",
                        fontweight="bold", zorder=9)
    # ------------------------------------------------------------------

    def _add_dynamic(self, artist) -> None:
        """Register a blit-layer artist for cleanup at the next frame."""
        self._dynamic_artists.append(artist)

    def _draw_objects_blit(self, objects: List[ObjectRecord], ax,
                            resim: bool = False) -> None:
        """Add per-frame object artists and draw them into the blit region.

        Every artist created here is appended to ``_dynamic_artists`` so it
        can be removed at the start of the next frame (prevents unbounded
        accumulation in ax.patches / ax.lines / ax.texts during playback).
        """
        da = self._dynamic_artists   # local alias for speed
        fig_draw = self._fig.draw_artist
        dual = self._dual_mode   # True when both source files are loaded

        for obj in objects:
            if (obj.ref_lat < self._lat_min or obj.ref_lat > self._lat_max
                    or obj.ref_lon < self._lon_min or obj.ref_lon > self._lon_max):
                continue
            if resim:
                if SETTINGS.class_color_mode:
                    colour = _RESIM_CLASS.get(obj.class_id, _RESIM_CLASS_FALLBACK)
                    _, lw, alpha = SETTINGS.box_style(obj.dyn_property, obj.class_id)
                else:
                    colour = _RESIM_DYN.get(obj.dyn_property, _RESIM_DYN_FALLBACK)
                    _, lw, alpha = SETTINGS.dyn_styles.get(obj.dyn_property, SETTINGS.dyn_fallback)
                lw = lw + _RESIM_LW_EXTRA
            elif dual:
                if SETTINGS.class_color_mode:
                    colour = _ORIG_DUAL_CLASS.get(obj.class_id, _ORIG_DUAL_CLASS_FALLBACK)
                    _, lw, alpha = SETTINGS.box_style(obj.dyn_property, obj.class_id)
                else:
                    colour = _ORIG_DUAL_DYN.get(obj.dyn_property, _ORIG_DUAL_DYN_FALLBACK)
                    _, lw, alpha = SETTINGS.dyn_styles.get(obj.dyn_property, SETTINGS.dyn_fallback)
            else:
                colour, lw, alpha = SETTINGS.box_style(obj.dyn_property, obj.class_id)

            corners = _box_corners(
                obj.ref_lat, obj.ref_lon,
                max(obj.width, 0.3), max(obj.length, 0.3), obj.heading,
            )
            poly = Polygon(corners, closed=True,
                           facecolor=colour, alpha=alpha,
                           edgecolor=colour, linewidth=lw, zorder=5,
                           linestyle=_RESIM_LINESTYLE if resim else "solid",
                           animated=True)
            ax.add_patch(poly)
            da.append(poly)
            fig_draw(poly)

            # Selected-object highlight
            if (self._selected_key is not None
                    and obj.side == self._selected_key[0]
                    and obj.tracking_id == self._selected_key[1]
                    and getattr(obj, '_data_source', 'Original') == self._selected_key[2]):
                outer = _box_corners(
                    obj.ref_lat, obj.ref_lon,
                    max(obj.width, 0.3) + 0.6,
                    max(obj.length, 0.3) + 0.6,
                    obj.heading,
                )
                sel_poly = Polygon(outer, closed=True,
                                   facecolor="none",
                                   edgecolor="#ffff00",
                                   linewidth=2.0, linestyle="--",
                                   zorder=10, animated=True)
                ax.add_patch(sel_poly)
                da.append(sel_poly)
                fig_draw(sel_poly)

            # Jump-to-frame halo (cyan ring around object centre)
            if self._jump_highlights and self._current_scan is not None:
                src = "Resim" if resim else "Original"
                cur = self._current_scan
                if any(
                    h[0] == obj.side
                    and h[1] == obj.tracking_id
                    and h[2] == src
                    and h[3] <= cur <= h[4]
                    for h in self._jump_highlights
                ):
                    circ, = ax.plot(obj.ref_lat, obj.ref_lon, "o",
                                    markersize=26, markeredgewidth=2.5,
                                    markerfacecolor="none",
                                    markeredgecolor="#00e5ff",
                                    zorder=12, alpha=0.9, animated=True)
                    da.append(circ)
                    fig_draw(circ)

            # Reference-point dot (Moving only)
            if obj.dyn_property == 3:
                dot_colour = "#f44336" if obj.ref_pt in _REF_PT_INVALID else colour
                dot_lat, dot_lon = _ref_point_pos(obj)
                dot, = ax.plot(dot_lat, dot_lon, "o",
                               color=dot_colour, markersize=3,
                               zorder=8, alpha=0.9, animated=True)
                da.append(dot)
                fig_draw(dot)

            # Heading arrow — simple line+tip marker (fast; avoids FancyArrowPatch)
            if self._show_heading:
                arrow_len = min(max(obj.length, 0.5) * 0.8, 5.0)
                dlat = arrow_len * np.sin(obj.heading)
                dlon = arrow_len * np.cos(obj.heading)
                tip_lat = obj.ref_lat + dlat
                tip_lon = obj.ref_lon + dlon
                ln, = ax.plot([obj.ref_lat, tip_lat], [obj.ref_lon, tip_lon],
                              color=colour, linewidth=min(lw, 1.5),
                              zorder=6, animated=True)
                da.append(ln)
                fig_draw(ln)
                mk, = ax.plot(tip_lat, tip_lon, ">",
                              color=colour, markersize=4,
                              zorder=6, animated=True)
                da.append(mk)
                fig_draw(mk)

            # Velocity vector — simple line+tip (avoids expensive FancyArrowPatch)
            if self._show_velocity:
                spd = np.hypot(obj.lon_vel, obj.lat_vel)
                if spd > 0.1:
                    vel_colour, vel_alpha = SETTINGS.vel_style(obj.dyn_property)
                    tip_lat = obj.ref_lat + obj.lat_vel * _VEL_SCALE
                    tip_lon = obj.ref_lon + obj.lon_vel * _VEL_SCALE
                    ln, = ax.plot([obj.ref_lat, tip_lat], [obj.ref_lon, tip_lon],
                                  color=vel_colour, linewidth=0.8,
                                  alpha=vel_alpha, zorder=7, animated=True)
                    da.append(ln)
                    fig_draw(ln)

            # ID label (Moving / Stopped only) — anchored outside front-right corner
            if self._show_ids and obj.dyn_property in (2, 3):
                label = f"{'L' if obj.side == 'L' else 'R'}:{obj.tracking_id}"
                _hl = max(obj.length, 0.3) / 2
                _hw = max(obj.width,  0.3) / 2
                _ch, _sh = np.cos(obj.heading), np.sin(obj.heading)
                _dlat = _sh * _hl + _ch * _hw   # vector to front-right corner
                _dlon = _ch * _hl - _sh * _hw
                _nm   = max(np.hypot(_dlat, _dlon), 1e-6)
                _off  = 0.5   # metres beyond the corner
                _lbl_lat = obj.ref_lat + _dlat + _off * _dlat / _nm
                _lbl_lon = obj.ref_lon + _dlon + _off * _dlon / _nm
                txt = ax.text(_lbl_lat, _lbl_lon, label,
                              color=SETTINGS.id_colour,
                              alpha=SETTINGS.id_alpha,
                              fontsize=7, ha="center", va="center",
                              fontweight="bold", zorder=9, animated=True)
                da.append(txt)
                fig_draw(txt)

    def _on_canvas_drawn(self, event) -> None:
        """Fired after every canvas.draw().  Re-blit objects when the draw was
        triggered externally (e.g. toolbar zoom/pan/home) so they don't vanish."""
        if self._in_managed_draw:
            return   # we initiated this draw; _rebuild_background handles it
        if self._current_scan is None:
            return   # no data loaded yet

        # Sync stored axis limits with whatever matplotlib now has
        # (toolbar may have changed them without going through our setters)
        xlim = self._ax.get_xlim()   # inverted: (lat_max, lat_min)
        ylim = self._ax.get_ylim()   # (lon_min, lon_max)
        self._lat_max, self._lat_min = xlim[0], xlim[1]
        self._lon_min, self._lon_max = ylim[0], ylim[1]

        # Recapture the clean static background and overlay objects
        # Remove any stale animated artists first so they don't bake into
        # the captured background pixels.
        for art in self._dynamic_artists:
            try:
                art.remove()
            except Exception:
                pass
        self._dynamic_artists = []
        self._bg_cache = self.copy_from_bbox(self._ax.bbox)
        ax = self._ax
        self.restore_region(self._bg_cache)
        self._draw_objects_blit(self._objects, ax, resim=False)
        if self._resim_objects:
            self._draw_objects_blit(self._resim_objects, ax, resim=True)
        self.blit(ax.bbox)

    # ------------------------------------------------------------------
    # Mouse / scroll events
    # ------------------------------------------------------------------

    def _toolbar_active(self) -> bool:
        return (self._nav_toolbar is not None
                and bool(self._nav_toolbar.mode))

    def _on_scroll(self, event) -> None:
        if not self._zoom_mode:
            return
        if event.inaxes is not self._ax:
            return
        factor = 0.82 if event.button == "up" else 1.0 / 0.82
        cx = event.xdata if event.xdata is not None else (self._lat_min + self._lat_max) / 2
        cy = event.ydata if event.ydata is not None else (self._lon_min + self._lon_max) / 2
        self._lat_min = cx - (cx - self._lat_min) * factor
        self._lat_max = cx + (self._lat_max - cx) * factor
        self._lon_min = cy - (cy - self._lon_min) * factor
        self._lon_max = cy + (self._lon_max - cy) * factor
        self._ax.set_xlim(self._lat_max, self._lat_min)
        self._ax.set_ylim(self._lon_min, self._lon_max)
        self._bg_cache = None   # axis limits changed
        self.draw()

    def _on_press(self, event) -> None:
        if event.inaxes is not self._ax:
            return
        if event.dblclick:
            self._click_start = None   # cancel pending single-click
            self._handle_dblclick(event)
            return
        if event.button == 1:
            if event.xdata is not None and event.ydata is not None:
                self._click_start = (event.xdata, event.ydata)
            if self._zoom_mode and not self._toolbar_active():
                if event.xdata is not None and event.ydata is not None:
                    self._rb_start = (event.xdata, event.ydata)

    def _on_motion(self, event) -> None:
        if self._rb_start is None:
            return
        if event.inaxes is not self._ax or event.xdata is None:
            return
        x0, y0 = self._rb_start
        x1, y1 = event.xdata, event.ydata
        if self._rb_patch is not None:
            try:
                self._rb_patch.remove()
            except Exception:
                pass
        self._rb_patch = MplRectangle(
            (min(x0, x1), min(y0, y1)),
            abs(x1 - x0), abs(y1 - y0),
            linewidth=1.5, linestyle="--",
            edgecolor="white", facecolor="none", zorder=20,
        )
        self._ax.add_patch(self._rb_patch)
        self.draw()

    def _on_release(self, event) -> None:
        # Single-click detection (non-zoom mode only)
        if (self._click_start is not None
                and event.button == 1
                and not self._zoom_mode
                and event.inaxes is self._ax
                and event.xdata is not None
                and event.ydata is not None):
            x0, y0 = self._click_start
            if abs(event.xdata - x0) < 1.5 and abs(event.ydata - y0) < 1.5:
                self._handle_single_click(event.xdata, event.ydata)
        self._click_start = None

        if self._rb_start is None:
            return
        if self._rb_patch is not None:
            try:
                self._rb_patch.remove()
            except Exception:
                pass
            self._rb_patch = None

        x0, y0 = self._rb_start
        self._rb_start = None

        if (event.inaxes is not self._ax
                or event.xdata is None or event.ydata is None):
            self.draw()
            return

        x1, y1 = event.xdata, event.ydata
        if abs(x1 - x0) > 0.5 and abs(y1 - y0) > 0.5:
            self._lat_min = min(x0, x1)
            self._lat_max = max(x0, x1)
            self._lon_min = min(y0, y1)
            self._lon_max = max(y0, y1)
            self._ax.set_xlim(self._lat_max, self._lat_min)
            self._ax.set_ylim(self._lon_min, self._lon_max)
            self._bg_cache = None   # axis limits changed
        self.draw()

    def _handle_single_click(self, xdata: float, ydata: float) -> None:
        """Find nearest object within tolerance and emit object_clicked."""
        if self._toolbar_active():
            return
        best_obj: Optional[ObjectRecord] = None
        best_dist = float("inf")
        best_source = "Original"
        for obj in self._objects:
            if obj.dyn_property not in (1, 2, 3):   # Stationary, Stopped or Moving
                continue
            dist = np.hypot(obj.ref_lat - xdata, obj.ref_lon - ydata)
            if dist < best_dist:
                best_dist = dist
                best_obj = obj
                best_source = "Original"
        # also check resim objects
        for obj in self._resim_objects:
            if obj.dyn_property not in (1, 2, 3):
                continue
            dist = np.hypot(obj.ref_lat - xdata, obj.ref_lon - ydata)
            if dist < best_dist:
                best_dist = dist
                best_obj = obj
                best_source = "Resim"
        if best_obj is not None and best_dist < 1.0:
            best_obj._data_source = best_source
            self.object_clicked.emit(best_obj)
        else:
            self.object_selection_cleared.emit()

    def _handle_dblclick(self, event) -> None:
        if self._toolbar_active():
            return
        if event.xdata is None or event.ydata is None:
            return
        best_obj: Optional[ObjectRecord] = None
        best_dist = float("inf")
        for obj in self._objects:
            dist = np.hypot(obj.ref_lat - event.xdata, obj.ref_lon - event.ydata)
            if dist < best_dist:
                best_dist = dist
                best_obj = obj
        if best_obj is not None and best_dist < 1.0:
            self.object_double_clicked.emit(best_obj)


# ---------------------------------------------------------------------------
# Module-level geometry helper
# ---------------------------------------------------------------------------

def _ref_point_pos(obj) -> tuple:
    """Return (lat, lon) of the ReferencePoint indicator dot on the bounding box.

    The box is centred at (obj.ref_lat, obj.ref_lon).  _REF_OFFSET gives the
    fraction (dlon_frac, dlat_frac) such that:
        box_centre = raw_signal + offset
    so the raw signal is at:
        raw_signal = box_centre - offset
    which in local object coords is:
        lon_local = -dlon_frac * length
        lat_local = -dlat_frac * width
    Rotate into VCS with the same convention used by _box_corners.
    """
    dlon_frac, dlat_frac = _REF_OFFSET.get(obj.ref_pt, (0.0, 0.0))
    lon_local = -dlon_frac * obj.length
    lat_local = -dlat_frac * obj.width
    c, s = np.cos(obj.heading), np.sin(obj.heading)
    dot_lon = obj.ref_lon + c * lon_local - s * lat_local
    dot_lat = obj.ref_lat + s * lon_local + c * lat_local
    return dot_lat, dot_lon


def _box_corners(lat_centre: float, lon_centre: float,
                 width: float, length: float,
                 heading: float) -> np.ndarray:
    """Return (4, 2) array of (lat, lon) corners for a rotated bounding box."""
    half_l = length / 2
    half_w = width  / 2
    local = np.array([
        [ half_l, -half_w],
        [ half_l,  half_w],
        [-half_l,  half_w],
        [-half_l, -half_w],
    ])
    c, s = np.cos(heading), np.sin(heading)
    rot = np.array([[c, -s], [s, c]])
    rotated = (rot @ local.T).T
    return np.column_stack([
        lat_centre + rotated[:, 1],
        lon_centre + rotated[:, 0],
    ])
