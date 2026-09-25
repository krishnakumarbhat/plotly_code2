"""
signal_plot_widget.py
=====================
Time-series signal plot with:
  - NavigationToolbar (zoom / pan / home)
  - Vertical cursor showing current scan-frame timestamp
  - Side panel showing signal values at cursor (toggle-able)
  - Markers: Ctrl+click to place / remove a vertical marker line;
             marker values shown in the side panel
  - Click on plot: move cursor + show value
"""

from __future__ import annotations

from typing import List, Optional, Tuple

import numpy as np
import pandas as pd
from matplotlib.backends.backend_qtagg import (
    FigureCanvasQTAgg as FigureCanvas,
    NavigationToolbar2QT,
)
from matplotlib.figure import Figure
from view_settings import SETTINGS

# ---------------------------------------------------------------------------
# Theme palettes for matplotlib axes / table
# ---------------------------------------------------------------------------
_PLOT_THEME_DARK = dict(
    ax_bg="#1a1a2e", fig_bg="#12122a",
    tick="white", spine="#444466", grid="#2a2a4a", title="white",
    legend_fc="#1a1a2e", legend_ec="#444466", legend_lc="white",
    table_bg="#1a1a2e", table_fg="#ccccff",
    table_hdr_bg="#12122a", table_hdr_fg="#8888cc",
    toolbar=(
        "QToolBar{background:#1a1a3e;border:none;}"
        "QToolButton{background:#2a2a5e;color:#ccccff;border:none;padding:2px;}"
        "QToolButton:hover{background:#3a3a7e;}"
    ),
)
_PLOT_THEME_LIGHT = dict(
    ax_bg="#ffffff", fig_bg="#f5f5f5",
    tick="#111111", spine="#aaaaaa", grid="#dddddd", title="#111111",
    legend_fc="#ffffff", legend_ec="#aaaaaa", legend_lc="#111111",
    table_bg="#ffffff", table_fg="#111111",
    table_hdr_bg="#eeeeee", table_hdr_fg="#333333",
    toolbar=(
        "QToolBar{background:#e0e0e0;border:none;}"
        "QToolButton{background:#cccccc;color:#111111;border:none;padding:2px;}"
        "QToolButton:hover{background:#bbbbbb;}"
    ),
)

from PyQt6.QtCore import Qt
from PyQt6.QtWidgets import (
    QComboBox,
    QHBoxLayout,
    QHeaderView,
    QLabel,
    QPushButton,
    QSplitter,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
)

# Colours for signal lines
_LINE_COLOURS = [
    "#4fc3f7", "#ff9800", "#4caf50", "#e91e63",
    "#ce93d8", "#ffeb3b", "#26c6da", "#ef5350",
]
# Colours for marker lines
_MARKER_COLOURS = ["#ffeb3b", "#ff9800", "#ab47bc", "#26c6da"]


# X-axis mode labels and internal keys
_X_MODES = [
    ("Sensor time [s]",                 "can_ts"),
    ("Scan Index L",                     "scan_idx_l"),
    ("Scan Index R",                     "scan_idx_r"),
    ("Object timestamp L [s]",           "obj_ts_l"),
    ("Object timestamp R [s]",           "obj_ts_r"),
]


def _fmt_val(v: float) -> str:
    """Format *v* as a human-readable decimal string (no scientific notation).

    Rules:
    - If the value is an integer (or very close to one) show it without decimals.
    - Otherwise use up to 4 significant digits but always in decimal form.
    """
    if v != v:          # NaN
        return "nan"
    if v == 0:
        return "0"
    abs_v = abs(v)
    # Show as integer when there is no fractional part
    if abs_v >= 1 and abs_v < 1e15 and v == int(v):
        return f"{int(v):,}".replace(",", "\u202f")   # thin-space thousands separator
    # Choose decimal places to give ≈4 significant digits, clamped to [0, 6]
    if abs_v >= 1:
        decimals = max(0, 4 - len(str(int(abs_v))))
    else:
        import math
        decimals = min(6, 4 - int(math.floor(math.log10(abs_v))))
    return f"{v:.{decimals}f}"


class SignalPlotWidget(QWidget):
    """Embeddable time-series signal plot with values panel and markers."""

    def __init__(self, parent=None):
        super().__init__(parent)
        layout = QVBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(2)

        # --- Matplotlib canvas ---
        self._fig = Figure(figsize=(8, 3), tight_layout=True)
        self._canvas = FigureCanvas(self._fig)
        self._ax = self._fig.add_subplot(111)

        # --- Toolbar ---
        self._nav_toolbar = NavigationToolbar2QT(self._canvas, self)
        self._nav_toolbar.setStyleSheet(
            "QToolBar { background:#1a1a3e; border:none; }"
            "QToolButton { background:#2a2a5e; color:#ccccff; border:none; padding:2px; }"
            "QToolButton:hover { background:#3a3a7e; }"
        )

        # --- Toggle button ---
        self._toggle_btn = QPushButton("Hide values")
        self._toggle_btn.setFixedWidth(90)
        self._toggle_btn.setCheckable(True)
        self._toggle_btn.setChecked(True)
        self._toggle_btn.toggled.connect(self._on_toggle_panel)

        # --- Values table ---
        self._values_table = QTableWidget()
        self._values_table.setColumnCount(3)
        self._values_table.setHorizontalHeaderLabels(["Signal", "Value", ""])
        self._values_table.horizontalHeader().setSectionResizeMode(
            0, QHeaderView.ResizeMode.Stretch
        )
        self._values_table.horizontalHeader().setSectionResizeMode(
            1, QHeaderView.ResizeMode.ResizeToContents
        )
        self._values_table.horizontalHeader().setSectionResizeMode(
            2, QHeaderView.ResizeMode.Fixed
        )
        self._values_table.setColumnWidth(2, 22)
        self._values_table.verticalHeader().setVisible(False)
        self._values_table.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        self._values_table.setMinimumWidth(200)
        self._values_table.setMaximumWidth(340)
        self._values_table.setStyleSheet(
            "QTableWidget { background:#1a1a2e; color:#ccccff; "
            "gridline-color:#333366; font-size:10px; }"
            "QHeaderView::section { background:#12122a; color:#8888cc; "
            "border:none; padding:3px; font-size:10px; }"
        )

        # --- Layout ---
        # Toolbar + toggle button row
        toolbar_row = QWidget()
        tr_layout = QHBoxLayout(toolbar_row)
        tr_layout.setContentsMargins(0, 0, 0, 0)
        tr_layout.setSpacing(4)
        tr_layout.addWidget(self._nav_toolbar, stretch=1)

        tr_layout.addWidget(QLabel("X axis:"))
        self._x_combo = QComboBox()
        for label, _ in _X_MODES:
            self._x_combo.addItem(label)
        self._x_combo.setFixedWidth(180)
        self._x_combo.currentIndexChanged.connect(self._on_x_mode_changed)
        tr_layout.addWidget(self._x_combo)

        self._clear_btn = QPushButton("Clear Plot")
        self._clear_btn.setFixedWidth(80)
        self._clear_btn.setToolTip("Remove all plotted signals")
        self._clear_btn.clicked.connect(self.clear_plot)
        tr_layout.addWidget(self._clear_btn)

        tr_layout.addWidget(self._toggle_btn)
        layout.addWidget(toolbar_row)

        # Canvas + values panel splitter
        self._splitter = QSplitter(Qt.Orientation.Horizontal)
        self._splitter.addWidget(self._canvas)
        self._splitter.addWidget(self._values_table)
        self._splitter.setStretchFactor(0, 5)
        self._splitter.setStretchFactor(1, 1)
        layout.addWidget(self._splitter, stretch=1)

        # --- Internal state ---
        self._cursor_line = None
        self._current_ts: Optional[float] = None   # sensor obj_ts [s]
        self._current_x:  Optional[float] = None   # X-axis position (current mode)
        self._last_signals: list = []
        self._last_x_arrays: list = []   # x_arr per signal (current X mode)
        self._last_can_arrays: list = [] # can_ts per signal (always seconds)
        self._model = None   # set via set_model()
        # Each marker: (can_ts, x_pos, line_artist, label_str)
        self._markers: List[Tuple[float, float, object, str]] = []

        # Connect mouse click on canvas
        self._fig.canvas.mpl_connect("button_press_event", self._on_click)

        self._setup_axes()

    # ------------------------------------------------------------------
    # Axes style
    # ------------------------------------------------------------------

    def _setup_axes(self) -> None:
        t = _PLOT_THEME_LIGHT if SETTINGS.theme == "light" else _PLOT_THEME_DARK
        ax = self._ax
        ax.set_facecolor(t["ax_bg"])
        self._fig.patch.set_facecolor(t["fig_bg"])
        ax.tick_params(colors=t["tick"], labelsize=8)
        ax.xaxis.label.set_color(t["tick"])
        ax.yaxis.label.set_color(t["tick"])
        ax.title.set_color(t["title"])
        for spine in ax.spines.values():
            spine.set_edgecolor(t["spine"])
        ax.grid(True, color=t["grid"], linewidth=0.5, linestyle="--")

    def _apply_table_theme(self) -> None:
        t = _PLOT_THEME_LIGHT if SETTINGS.theme == "light" else _PLOT_THEME_DARK
        self._values_table.setStyleSheet(
            f"QTableWidget{{background:{t['table_bg']};color:{t['table_fg']};"
            f"gridline-color:#333366;font-size:10px;}}"
            f"QHeaderView::section{{background:{t['table_hdr_bg']};"
            f"color:{t['table_hdr_fg']};border:none;padding:3px;font-size:10px;}}"
        )
        self._nav_toolbar.setStyleSheet(t["toolbar"])

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def set_model(self, model) -> None:
        """Provide a DataModel so X-axis alternatives can be resolved."""
        self._model = model

    def set_theme(self, theme: str) -> None:
        """Switch matplotlib colour theme and redraw."""
        self._setup_axes()
        self._apply_table_theme()
        self._canvas.draw()

    def plot_signals(self, signals: list) -> None:
        """
        Plot one or more signals.

        Parameters
        ----------
        signals : list of (msg_name, signal_name, pd.DataFrame)
        """
        ax = self._ax
        ax.cla()
        self._setup_axes()
        self._cursor_line = None
        self._markers = []

        plotted = 0
        x_mode = _X_MODES[self._x_combo.currentIndex()][1]
        x_label = _X_MODES[self._x_combo.currentIndex()][0]
        new_x_arrays = []
        new_can_arrays = []
        for i, (msg_name, sig_name, df) in enumerate(signals):
            if sig_name not in df.columns:
                new_x_arrays.append(None)
                new_can_arrays.append(None)
                continue
            can_ts     = df.index.to_numpy(dtype=float)
            is_obj_ts  = (getattr(df.index, 'name', '') == 'obj_ts')
            x_arr      = self._build_x_array(can_ts, x_mode, is_obj_ts=is_obj_ts)
            new_x_arrays.append(x_arr)
            new_can_arrays.append(can_ts)
            vals   = pd.to_numeric(df[sig_name], errors="coerce").to_numpy(dtype=float)
            colour = _LINE_COLOURS[i % len(_LINE_COLOURS)]
            ax.plot(x_arr, vals, color=colour, linewidth=0.5,
                    linestyle="solid", marker=".", markersize=1.0,
                    zorder=3, label=f"{msg_name}  \u203a  {sig_name}")
            plotted += 1

        self._last_x_arrays  = new_x_arrays
        self._last_can_arrays = new_can_arrays

        if plotted:
            ax.set_xlabel(x_label, fontsize=8)
            t = _PLOT_THEME_LIGHT if SETTINGS.theme == "light" else _PLOT_THEME_DARK
            title = signals[0][1] if len(signals) == 1 else f"{plotted} signals"
            ax.set_title(title, color=t["title"], fontsize=9)
            if len(signals) > 1:
                ax.legend(facecolor=t["legend_fc"], edgecolor=t["legend_ec"],
                          labelcolor=t["legend_lc"], fontsize=7, loc="upper right")

        if self._current_ts is not None:
            # _current_ts is always sensor obj_ts (set via set_cursor)
            self._draw_cursor_at_can_ts(self._current_ts, is_obj_ts=True)

        self._last_signals = list(signals)
        self._canvas.draw()
        self._update_values_table()

    def add_signals(self, signals: list) -> None:
        """
        Append *signals* to the current plot without clearing it.
        Signals already present (same msg_name + signal_name) are skipped.

        Parameters
        ----------
        signals : list of (msg_name, signal_name, pd.DataFrame)
        """
        existing_keys = {(m, s) for m, s, _ in self._last_signals}
        new = [sig for sig in signals
               if (sig[0], sig[1]) not in existing_keys]
        if not new:
            return
        self.plot_signals(self._last_signals + new)

    # Backward-compat alias
    def plot_signal(self, df, msg_name: str, signal_name: str) -> None:
        self.plot_signals([(msg_name, signal_name, df)])

    def set_cursor(self, timestamp: float) -> None:
        """Move the vertical cursor to *timestamp* (sensor obj_ts [s])."""
        self._current_ts = timestamp
        self._current_x  = float(self._build_x_array(
            np.array([timestamp]),
            self._x_mode_key(),
            is_obj_ts=True,   # cursor timestamp is always obj_ts
        )[0])
        if self._cursor_line is not None:
            try:
                self._cursor_line.remove()
            except Exception:
                pass
            self._cursor_line = None
        self._draw_cursor_at_can_ts(timestamp, is_obj_ts=True)
        self._canvas.draw()
        self._update_values_table()

    def clear_plot(self) -> None:
        self._ax.cla()
        self._setup_axes()
        self._cursor_line = None
        self._markers = []
        self._last_x_arrays = []
        self._last_can_arrays = []
        self._canvas.draw()
        self._values_table.setRowCount(0)

    # ------------------------------------------------------------------
    # Internal helpers
    # ------------------------------------------------------------------

    def _x_mode_key(self) -> str:
        return _X_MODES[self._x_combo.currentIndex()][1]

    def _build_x_array(self, can_ts: np.ndarray, mode: str,
                        is_obj_ts: bool = False) -> np.ndarray:
        """Convert timestamps to the requested X axis.

        Parameters
        ----------
        can_ts     : input timestamps.  For Signals-tab DataFrames this is
                     CAN recv_ts; for track-signal DataFrames (index.name ==
                     'obj_ts') and for cursor placement this is sensor obj_ts.
        mode       : one of the _X_MODES internal keys.
        is_obj_ts  : True when can_ts values are sensor obj_ts (track plots
                     and cursor).  Enables obj_ts-space lookup which is exact
                     for track DataFrames and avoids recv_ts anomalies.
        """
        if mode == "can_ts" or self._model is None:
            return can_ts

        side = "L" if mode.endswith("_l") else "R"

        # Object-timestamp mode: input is already (approximately) obj_ts.
        # Return as-is — no header lookup needed.  For signal-tab DataFrames
        # recv_ts ≈ obj_ts within a few ms, so this is accurate enough.
        if mode in ("obj_ts_l", "obj_ts_r"):
            return can_ts

        # Scan-index mode: map timestamps → scan_index.
        # When can_ts is obj_ts (track plots, cursor) use the model's
        # authoritative obj_ts→scan_index mapping for exact results.
        # When can_ts is recv_ts (signal-tab DataFrames) use the header
        # DataFrame lookup (recv_ts space) so anomalous recv_ts frames are
        # still placed at the correct scan_index.
        if is_obj_ts and hasattr(self._model, '_scan_to_obj_ts'):
            scan_to_ts = self._model._scan_to_obj_ts
            if scan_to_ts:
                items      = sorted(scan_to_ts.items(), key=lambda kv: kv[1])
                si_arr     = np.array([k for k, _ in items], dtype=float)
                ts_arr     = np.array([v for _, v in items], dtype=float)
                idxs       = np.searchsorted(ts_arr, can_ts).clip(0, len(ts_arr) - 1)
                idxs_prev  = (idxs - 1).clip(0, len(ts_arr) - 1)
                closer     = np.abs(ts_arr[idxs_prev] - can_ts) < np.abs(ts_arr[idxs] - can_ts)
                idxs[closer] = idxs_prev[closer]
                return si_arr[idxs]

        # Fallback: recv_ts-space lookup using the header DataFrame.
        hdr_key = f"SRR{side}_Header_SRR2"
        hdr_df  = self._model.frames.get(hdr_key)
        if hdr_df is None:
            return can_ts

        hdr_ts   = hdr_df.index.to_numpy(dtype=float)
        si_col   = f"Scan_Index_{side}"
        if si_col not in hdr_df.columns:
            return can_ts
        hdr_vals = hdr_df[si_col].to_numpy(dtype=float)

        idxs      = np.searchsorted(hdr_ts, can_ts).clip(0, len(hdr_ts) - 1)
        idxs_prev = (idxs - 1).clip(0, len(hdr_ts) - 1)
        closer    = np.abs(hdr_ts[idxs_prev] - can_ts) < np.abs(hdr_ts[idxs] - can_ts)
        idxs[closer] = idxs_prev[closer]
        return hdr_vals[idxs]

    def _on_x_mode_changed(self) -> None:
        if self._last_signals:
            self.plot_signals(self._last_signals)

    def _x_to_can_ts(self, x_pos: float) -> float:
        """Reverse-map an X-axis position to the nearest CAN timestamp [s]."""
        # In CAN-ts mode x_pos IS the CAN timestamp
        if self._x_mode_key() == "can_ts":
            return x_pos
        # Use the first available signal's arrays
        for x_arr, can_arr in zip(self._last_x_arrays, self._last_can_arrays):
            if x_arr is None or can_arr is None or len(x_arr) == 0:
                continue
            i = int(np.argmin(np.abs(x_arr - x_pos)))
            return float(can_arr[i])
        return x_pos  # fallback

    def _draw_cursor_at_can_ts(self, can_ts: float,
                               is_obj_ts: bool = False) -> None:
        """Draw cursor at the X position corresponding to *can_ts*."""
        x_pos = float(self._build_x_array(
            np.array([can_ts]), self._x_mode_key(),
            is_obj_ts=is_obj_ts)[0])
        self._cursor_line = self._ax.axvline(
            x=x_pos, color="#ff6b6b", linewidth=1.2, linestyle="--", zorder=5
        )

    def _draw_cursor(self, ts: float) -> None:
        """Draw cursor directly at *ts* (already in current X units)."""
        self._cursor_line = self._ax.axvline(
            x=ts, color="#ff6b6b", linewidth=1.2, linestyle="--", zorder=5
        )

    def _interp_value(self, df: pd.DataFrame, sig_name: str, ts: float) -> Optional[float]:
        """Return the signal value nearest to *ts*."""
        if sig_name not in df.columns:
            return None
        idx_arr = df.index.to_numpy(dtype=float)
        vals = pd.to_numeric(df[sig_name], errors="coerce").to_numpy(dtype=float)
        if len(idx_arr) == 0:
            return None
        i = int(np.searchsorted(idx_arr, ts))
        if i >= len(idx_arr):
            i = len(idx_arr) - 1
        elif i > 0 and abs(idx_arr[i - 1] - ts) < abs(idx_arr[i] - ts):
            i -= 1
        return float(vals[i])

    def _remove_signal(self, idx: int) -> None:
        """Remove the signal at *idx* from the plot and redraw."""
        if 0 <= idx < len(self._last_signals):
            self._last_signals.pop(idx)
            if self._last_signals:
                self.plot_signals(self._last_signals)
            else:
                self.clear_plot()

    def _update_values_table(self) -> None:
        """Populate the side panel with cursor and marker values."""
        # Each entry: ('header', label) | ('signal', label, val_str, sig_idx)
        row_data = []
        x_label = _X_MODES[self._x_combo.currentIndex()][0]

        # Cursor values
        if self._current_ts is not None:
            x_disp = self._current_x if self._current_x is not None else self._current_ts
            row_data.append(('header', f"{x_label} = {_fmt_val(x_disp)}"))
            for i, (msg_name, sig_name, df) in enumerate(self._last_signals):
                v = self._interp_value(df, sig_name, self._current_ts)
                label = f"  {msg_name} \u203a {sig_name}"
                val_str = _fmt_val(v) if v is not None else "n/a"
                row_data.append(('signal', label, val_str, i))

        # Marker values
        for m_can_ts, m_x, _, m_lbl in self._markers:
            row_data.append(('header', f"Marker {m_lbl}  {x_label} = {_fmt_val(m_x)}"))
            for i, (msg_name, sig_name, df) in enumerate(self._last_signals):
                v = self._interp_value(df, sig_name, m_can_ts)
                label = f"  {msg_name} \u203a {sig_name}"
                val_str = _fmt_val(v) if v is not None else "n/a"
                row_data.append(('signal', label, val_str, i))

        self._values_table.setRowCount(len(row_data))
        for r, rd in enumerate(row_data):
            if rd[0] == 'header':
                self._values_table.setItem(r, 0, QTableWidgetItem(rd[1]))
                self._values_table.setItem(r, 1, QTableWidgetItem(""))
                self._values_table.removeCellWidget(r, 2)
                self._values_table.setItem(r, 2, QTableWidgetItem(""))
            else:
                _, label, val_str, sig_idx = rd
                self._values_table.setItem(r, 0, QTableWidgetItem(label))
                self._values_table.setItem(r, 1, QTableWidgetItem(val_str))
                btn = QPushButton("\u00d7")
                btn.setFixedSize(18, 18)
                btn.setToolTip("Remove this signal from the plot")
                btn.setStyleSheet(
                    "QPushButton{background:#3a1a1a;color:#ff8888;"
                    "border:1px solid #664444;border-radius:2px;font-size:11px;padding:0;}"
                    "QPushButton:hover{background:#5a2a2a;}")
                btn.clicked.connect(
                    lambda _checked, idx=sig_idx: self._remove_signal(idx))
                self._values_table.setCellWidget(r, 2, btn)

    def _on_toggle_panel(self, checked: bool) -> None:
        self._values_table.setVisible(checked)
        self._toggle_btn.setText("Hide values" if checked else "Show values")

    # ------------------------------------------------------------------
    # Mouse events
    # ------------------------------------------------------------------

    def _toolbar_active(self) -> bool:
        return bool(self._nav_toolbar.mode)

    def _on_click(self, event) -> None:
        if event.inaxes is not self._ax or event.xdata is None:
            return
        if self._toolbar_active():
            return

        x_pos = event.xdata

        if event.button == 1:
            if event.key == "control":
                self._toggle_marker(x_pos)
            else:
                # Reverse-map X position to CAN timestamp
                can_ts = self._x_to_can_ts(x_pos)
                self._current_ts = can_ts
                self._current_x  = x_pos
                if self._cursor_line is not None:
                    try:
                        self._cursor_line.remove()
                    except Exception:
                        pass
                    self._cursor_line = None
                self._draw_cursor(x_pos)
                self._canvas.draw()
                self._update_values_table()

    def _toggle_marker(self, x_pos: float) -> None:
        """Add a new marker or remove the nearest existing one."""
        xlim = self._ax.get_xlim()
        threshold = (xlim[1] - xlim[0]) * 0.01 if xlim[1] != xlim[0] else 0.1

        for i, (m_can_ts, m_x, m_line, _) in enumerate(self._markers):
            if abs(m_x - x_pos) < threshold:
                try:
                    m_line.remove()
                except Exception:
                    pass
                self._markers.pop(i)
                self._canvas.draw()
                self._update_values_table()
                return

        # Add new marker
        n = len(self._markers) + 1
        col = _MARKER_COLOURS[(n - 1) % len(_MARKER_COLOURS)]
        line = self._ax.axvline(x=x_pos, color=col, linewidth=1.0,
                                linestyle=":", zorder=4)
        can_ts = self._x_to_can_ts(x_pos)
        self._markers.append((can_ts, x_pos, line, str(n)))
        self._canvas.draw()
        self._update_values_table()
