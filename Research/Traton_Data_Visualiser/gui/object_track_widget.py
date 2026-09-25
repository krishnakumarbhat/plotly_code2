"""
object_track_widget.py
======================
Widget displaying full statistics for a single ObjectTrack.

States
------
1. No model loaded     → placeholder text, Select button disabled
2. Model loaded        → tracks computed automatically, progress dialog shown
3. Computed, no click  → status message with track counts
4. Track selected      → header (UUID / scan range / duration) + signal table
"""

from __future__ import annotations

from typing import Dict, List, Optional, Tuple

from PyQt6.QtCore import Qt, pyqtSignal
from PyQt6.QtGui import QColor, QPainter
from PyQt6.QtWidgets import (
    QAbstractItemView,
    QApplication,
    QDialog,
    QDialogButtonBox,
    QFrame,
    QHBoxLayout,
    QHeaderView,
    QLabel,
    QProgressDialog,
    QPushButton,
    QSizePolicy,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
)

import pandas as pd

from object_track import ObjectTrack, build_object_tracks, strip_slot_suffix
from resim_kpi import _roi_mult_track


# ---------------------------------------------------------------------------
# Enum signal definitions (decoded from DBC VAL_ tables)
# ---------------------------------------------------------------------------

_ENUM_SIGNALS = {
    "ClassMostProb", "ClassSecMostProb",
    "DynamicProperty", "MaintenanceState", "Occlusion",
}

_ENUM_LABELS: Dict[str, Dict[int, str]] = {
    "DynamicProperty": {
        0: "Invalid object", 1: "Stationary", 2: "Stopped",
        3: "Moving", 6: "Error", 7: "Not available",
    },
    "ClassMostProb": {
        0: "Unknown", 1: "Car", 2: "Motorbike", 3: "Truck",
        4: "Bicycle", 5: "Pedestrian", 6: "Error", 7: "Not available",
    },
    "ClassSecMostProb": {
        0: "Unknown", 1: "Car", 2: "Motorbike", 3: "Truck",
        4: "Bicycle", 5: "Pedestrian", 6: "Error", 7: "Not available",
    },
    "MaintenanceState": {
        0: "Not active", 1: "Newly created", 2: "Measured",
        3: "Predicted", 6: "Error", 7: "Not available",
    },
    "Occlusion": {
        0: "Undefined", 1: "Occluded", 2: "On edge",
        3: "Visible", 6: "Error", 7: "Not available",
    },
}


def _decode_enum(sig: str, raw_val: float) -> str:
    """Return '3 (Moving)' style string for an enum signal value."""
    labels = _ENUM_LABELS.get(sig, {})
    try:
        key = int(round(raw_val))
    except (TypeError, ValueError):
        return str(raw_val)
    label = labels.get(key, "?")
    return f"{key} ({label})"


def _roi_label(track: ObjectTrack) -> str:
    """Return '1', '2', '3', or 'None' for the best ROI zone the track reached."""
    mult = _roi_mult_track(track)
    if mult >= 1.0:
        return "1"
    if mult >= 0.5:
        return "2"
    if mult > 0.0:
        return "3"
    return "None"


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

class _ElidedLabel(QLabel):
    """QLabel that right-elides its text instead of enforcing a minimum width."""

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setMinimumWidth(0)
        self.setSizePolicy(QSizePolicy.Policy.Expanding,
                           QSizePolicy.Policy.Preferred)

    def paintEvent(self, event):          # noqa: N802
        painter = QPainter(self)
        fm = self.fontMetrics()
        elided = fm.elidedText(
            self.text(), Qt.TextElideMode.ElideRight, self.width())
        painter.drawText(self.rect(), int(self.alignment()), elided)


# ---------------------------------------------------------------------------
# Stylesheets
# ---------------------------------------------------------------------------

_DARK_STYLE = """
    QWidget          { background: #12122a; color: #ffffff; }
    QTableWidget     { background: #1a1a2e; color: #ffffff;
                       gridline-color: #333366; font-size: 11px; }
    QTableWidget::item            { color: #ffffff; background: #1a1a2e; }
    QTableWidget::item:alternate  { color: #ffffff; background: #16162a; }
    QTableWidget::item:selected   { color: #ffffff; background: #3a3a7e; }
    QHeaderView::section { background: #12122a; color: #aaaacc;
                       border: none; padding: 3px; font-size: 11px; }
    QLabel           { color: #ccccff; font-size: 11px; }
    QPushButton      { background: #2a2a5e; color: #ffffff;
                       border: 1px solid #555588; border-radius: 3px;
                       padding: 4px 14px; }
    QPushButton:hover    { background: #3a3a7e; }
    QPushButton:disabled { color: #555577; border-color: #333355; }
    QFrame           { color: #444466; }
"""

_LIGHT_STYLE = """
    QWidget          { background: #f5f5f5; color: #111111; }
    QTableWidget     { background: #ffffff; color: #111111;
                       gridline-color: #cccccc; font-size: 11px; }
    QHeaderView::section { background: #eeeeee; color: #333333;
                       border: none; padding: 3px; font-size: 11px; }
    QLabel           { color: #333333; font-size: 11px; }
    QPushButton      { background: #e0e0e0; color: #111111;
                       border: 1px solid #aaaaaa; border-radius: 3px;
                       padding: 4px 14px; }
    QPushButton:hover    { background: #cccccc; }
    QPushButton:disabled { color: #aaaaaa; border-color: #cccccc; }
    QFrame           { color: #aaaaaa; }
"""


# ---------------------------------------------------------------------------
# Track list dialog
# ---------------------------------------------------------------------------

_TRACK_LIST_STYLE = (
    "QDialog,QWidget { background:#12122a; color:#ccccff; }"
    "QTableWidget { background:#1a1a3e; color:#ffffff; gridline-color:#333366;"
    " font-size: 11px; selection-background-color:#3a3a7e; }"
    "QHeaderView::section { background:#2a2a5e; color:#ccccff;"
    " border:none; padding:3px; font-size:11px; }"
    "QPushButton { background:#2a2a5e; color:#ccccff; border:1px solid #555588;"
    " border-radius:3px; padding:4px 14px; }"
    "QPushButton:hover { background:#3a3a7e; }"
    "QPushButton:disabled { color:#555577; border-color:#333355; }"
    "QLabel { color:#aaaacc; }"
)


class _NumericItem(QTableWidgetItem):
    """QTableWidgetItem that sorts by a stored numeric value."""
    def __init__(self, display: str, sort_value: float):
        super().__init__(display)
        self._sort_value = sort_value

    def __lt__(self, other) -> bool:
        if isinstance(other, _NumericItem):
            return self._sort_value < other._sort_value
        return super().__lt__(other)


class _TrackListDialog(QDialog):
    """Modal dialog listing all computed ObjectTracks."""

    # Emits the selected ObjectTrack when user confirms
    track_selected = pyqtSignal(object)

    _COLS = ["UUID", "Source", "Side", "Tracking ID", "Scan Start", "Scan End", "Duration", "Drops", "ROI"]
    _COL_SCAN_START = 4   # default sort column

    def __init__(self, tracks: List[ObjectTrack], model, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Select Object Track")
        self.resize(980, 480)
        self.setStyleSheet(_TRACK_LIST_STYLE)

        lay = QVBoxLayout(self)
        lay.setSpacing(8)

        lbl = QLabel(f"{len(tracks)} object track{'s' if len(tracks) != 1 else ''} available.  "
                     "Double-click a row or select and press OK.")
        lay.addWidget(lbl)

        self._table = QTableWidget(len(tracks), len(self._COLS))
        self._table.setHorizontalHeaderLabels(self._COLS)
        self._table.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        self._table.setSelectionBehavior(QAbstractItemView.SelectionBehavior.SelectRows)
        self._table.setSelectionMode(QAbstractItemView.SelectionMode.SingleSelection)
        self._table.verticalHeader().setVisible(False)
        self._table.setAlternatingRowColors(True)
        self._table.setSortingEnabled(True)
        hdr = self._table.horizontalHeader()
        hdr.setSectionResizeMode(0, QHeaderView.ResizeMode.Stretch)  # UUID
        for c in range(1, len(self._COLS)):
            hdr.setSectionResizeMode(c, QHeaderView.ResizeMode.ResizeToContents)

        # Disable sorting while populating to avoid mid-insert reordering
        self._table.setSortingEnabled(False)
        _CTR = Qt.AlignmentFlag.AlignCenter
        _LFT = Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter
        for row, t in enumerate(tracks):
            side_str = "Left (SRRL)" if t.side == "L" else "Right (SRRR)"

            uuid_item = QTableWidgetItem(t.uuid)
            uuid_item.setTextAlignment(_LFT)
            # Store the track reference so lookup survives sorting
            uuid_item.setData(Qt.ItemDataRole.UserRole, t)
            self._table.setItem(row, 0, uuid_item)

            src_item = QTableWidgetItem(t.data_source)
            src_item.setTextAlignment(_CTR)
            # Colour-code: Original = default, Resim = cyan tint
            if t.data_source == "Resim":
                src_item.setForeground(Qt.GlobalColor.cyan)
            self._table.setItem(row, 1, src_item)

            side_item = QTableWidgetItem(side_str)
            side_item.setTextAlignment(_CTR)
            self._table.setItem(row, 2, side_item)

            start_disp = model.display_scan(t.start_scan, t.side) if model else t.start_scan
            end_disp   = model.display_scan(t.end_scan,   t.side) if model else t.end_scan
            for col, (display, num) in enumerate([
                (str(t.tracking_id),          float(t.tracking_id)),
                (str(start_disp),             float(start_disp)),
                (str(end_disp),               float(end_disp)),
                (f"{t.duration:.3f} s",       t.duration),
                (str(len(t.id_drops)),        float(len(t.id_drops))),
            ], start=3):
                item = _NumericItem(display, num)
                item.setTextAlignment(_CTR)
                self._table.setItem(row, col, item)

            roi_txt  = _roi_label(t)
            roi_item = _NumericItem(
                roi_txt,
                {"1": 1.0, "2": 2.0, "3": 3.0}.get(roi_txt, 4.0),
            )
            roi_item.setTextAlignment(_CTR)
            _ROI_COLS = {"1": QColor(60, 160, 80), "2": QColor(200, 140, 30),
                         "3": QColor(120, 120, 140), "None": QColor(200, 60, 60)}
            roi_item.setForeground(_ROI_COLS.get(roi_txt, QColor(200, 60, 60)))
            self._table.setItem(row, 8, roi_item)

        self._table.setSortingEnabled(True)
        self._table.sortItems(self._COL_SCAN_START, Qt.SortOrder.AscendingOrder)

        self._table.itemDoubleClicked.connect(self._on_double_click)
        self._table.itemSelectionChanged.connect(self._on_selection_changed)
        lay.addWidget(self._table, stretch=1)

        btns = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel
        )
        self._ok_btn = btns.button(QDialogButtonBox.StandardButton.Ok)
        self._ok_btn.setEnabled(False)
        btns.accepted.connect(self._on_ok)
        btns.rejected.connect(self.reject)
        lay.addWidget(btns)

    # ------------------------------------------------------------------

    def _selected_track(self) -> Optional[ObjectTrack]:
        rows = self._table.selectionModel().selectedRows()
        if not rows:
            return None
        return self._table.item(rows[0].row(), 0).data(Qt.ItemDataRole.UserRole)

    def _on_selection_changed(self) -> None:
        self._ok_btn.setEnabled(self._selected_track() is not None)

    def _on_double_click(self, item) -> None:
        track = self._table.item(item.row(), 0).data(Qt.ItemDataRole.UserRole)
        self.track_selected.emit(track)
        self.accept()

    def _on_ok(self) -> None:
        track = self._selected_track()
        if track is not None:
            self.track_selected.emit(track)
        self.accept()


# ---------------------------------------------------------------------------
# Widget
# ---------------------------------------------------------------------------

class ObjectTrackWidget(QWidget):
    """
    Displays full statistics for the ObjectTrack that corresponds to
    the object most recently clicked in the Plan View.
    """

    # Emitted when the user clicks "Plot Signals"; carries a list of
    # (label, sig_name, pd.DataFrame) tuples ready for SignalPlotWidget.
    plot_signals_requested = pyqtSignal(list)

    # Emitted when the user clicks "Add to Plot"; same payload.
    add_signals_requested = pyqtSignal(list)

    # Emitted when the user clicks "Jump to start frame"; carries scan_idx.
    jump_to_scan_requested = pyqtSignal(int)

    # Emitted after tracks are computed; carries the full unified track list.
    tracks_computed = pyqtSignal(list)

    def __init__(self, parent=None):
        super().__init__(parent)
        self._model = None
        self._resim_model = None
        # Unified track list: contains both original + resim tracks once computed
        self._tracks: List[ObjectTrack] = []
        # Lookup: (side, tid, scan_idx, data_source) → ObjectTrack
        self._lookup: Dict[Tuple[str, int, int, str], ObjectTrack] = {}
        self._current_track: Optional[ObjectTrack] = None
        self._current_scan:  Optional[int]          = None
        self._sig_names:     List[str]               = []   # sorted signal keys
        self._signal_units:  Dict[str, str]          = {}   # stripped_name → unit

        self._build_ui()

    # ------------------------------------------------------------------
    # UI construction
    # ------------------------------------------------------------------

    def _build_ui(self) -> None:
        root = QVBoxLayout(self)
        root.setContentsMargins(6, 6, 6, 6)
        root.setSpacing(6)

        # ── Compute row ───────────────────────────────────────────────
        cr = QWidget()
        cr_lay = QHBoxLayout(cr)
        cr_lay.setContentsMargins(0, 0, 0, 0)
        cr_lay.setSpacing(8)

        self._select_btn = QPushButton("Select Object Track…")
        self._select_btn.setEnabled(False)
        self._select_btn.setToolTip("Browse all computed tracks and open one directly")
        self._select_btn.setFixedWidth(170)
        self._select_btn.clicked.connect(self._on_select_track)
        cr_lay.addWidget(self._select_btn)

        self._status_lbl = QLabel("Load an MF4 file first.")
        self._status_lbl.setWordWrap(True)
        cr_lay.addWidget(self._status_lbl, stretch=1)

        root.addWidget(cr)

        # ── Separator ─────────────────────────────────────────────────
        sep1 = QFrame()
        sep1.setFrameShape(QFrame.Shape.HLine)
        sep1.setFrameShadow(QFrame.Shadow.Sunken)
        root.addWidget(sep1)

        # ── Track header ──────────────────────────────────────────────
        self._header_widget = QWidget()
        h_lay = QVBoxLayout(self._header_widget)
        h_lay.setContentsMargins(0, 0, 0, 4)
        h_lay.setSpacing(2)

        self._lbl_uuid = QLabel()
        self._lbl_uuid.setTextInteractionFlags(
            Qt.TextInteractionFlag.TextSelectableByMouse)
        self._lbl_uuid.setStyleSheet(
            "font-family: monospace; font-size: 10px; color: #888899;")
        h_lay.addWidget(self._lbl_uuid)

        self._lbl_info = QLabel()
        h_lay.addWidget(self._lbl_info)

        self._lbl_drops = _ElidedLabel()
        self._lbl_drops.setStyleSheet("font-size: 10px; color: #aaaacc;")
        self._lbl_drops.setVisible(False)
        h_lay.addWidget(self._lbl_drops)

        self._header_widget.setVisible(False)
        root.addWidget(self._header_widget)

        # ── Separator (only visible when track shown) ─────────────────
        self._sep2 = QFrame()
        self._sep2.setFrameShape(QFrame.Shape.HLine)
        self._sep2.setFrameShadow(QFrame.Shadow.Sunken)
        self._sep2.setVisible(False)
        root.addWidget(self._sep2)

        # ── Signal statistics table ───────────────────────────────────
        self._table = QTableWidget()
        self._table.setColumnCount(6)
        self._table.setHorizontalHeaderLabels(
            ["Signal", "Current", "Min / Initial", "Max / Last", "Avg", "Median"])
        hdr = self._table.horizontalHeader()
        hdr.setSectionResizeMode(0, QHeaderView.ResizeMode.Stretch)
        for col in range(1, 6):
            hdr.setSectionResizeMode(col, QHeaderView.ResizeMode.ResizeToContents)
        self._table.verticalHeader().setVisible(False)
        self._table.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        self._table.setAlternatingRowColors(True)
        self._table.setVisible(False)
        self._table.itemSelectionChanged.connect(self._on_selection_changed)
        root.addWidget(self._table, stretch=1)

        # ── Plot button row ───────────────────────────────────────────
        btn_row = QWidget()
        btn_lay = QHBoxLayout(btn_row)
        btn_lay.setContentsMargins(0, 2, 0, 0)
        btn_lay.setSpacing(6)
        self._plot_btn = QPushButton("Plot Selected Signals")
        self._plot_btn.setEnabled(False)
        self._plot_btn.setToolTip(
            "Select one or more rows in the table above, then click to plot"
            " the corresponding time series in the Signal Plot tab.")
        self._plot_btn.clicked.connect(self._on_plot_clicked)
        btn_lay.addStretch()
        btn_lay.addWidget(self._plot_btn)

        self._add_to_plot_btn = QPushButton("Add to Plot")
        self._add_to_plot_btn.setEnabled(False)
        self._add_to_plot_btn.setToolTip(
            "Append selected signals to the current Signal Plot without clearing it.")
        self._add_to_plot_btn.clicked.connect(self._on_add_to_plot_clicked)
        btn_lay.addWidget(self._add_to_plot_btn)

        self._jump_btn = QPushButton("Jump to start frame")
        self._jump_btn.setEnabled(False)
        self._jump_btn.setToolTip("Navigate the playback position to this track's first scan")
        self._jump_btn.clicked.connect(self._on_jump_clicked)
        btn_lay.addWidget(self._jump_btn)
        root.addWidget(btn_row)

        self.setStyleSheet(_DARK_STYLE)
        self._jump_btn.setEnabled(False)
        self._jump_btn.setToolTip("Navigate the playback position to this track's first scan")
        self._jump_btn.clicked.connect(self._on_jump_clicked)
        btn_lay.addWidget(self._jump_btn)
        root.addWidget(btn_row)

        self.setStyleSheet(_DARK_STYLE)

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def set_model(self, model) -> None:
        """Called after a new MF4 file is loaded."""
        self._model = model
        self._resim_model = None
        self._signal_units = dict(getattr(model, "signal_units", {}) or {})
        self._tracks  = []
        self._lookup  = {}
        self._current_track = None
        self._current_scan  = None
        self._sig_names     = []
        self._header_widget.setVisible(False)
        self._sep2.setVisible(False)
        self._table.setVisible(False)
        self._select_btn.setEnabled(False)
        if model is not None:
            self._status_lbl.setText("Computing tracks\u2026")
            QApplication.processEvents()
            self._on_compute()
        else:
            self._status_lbl.setText("Load an MF4 file first.")

    def set_resim_model(self, model) -> None:
        """Called when a reprocessed MF4 is loaded or unloaded (model=None)."""
        self._resim_model = model
        if model is not None:
            self._signal_units.update(getattr(model, "signal_units", {}) or {})
        # Reset all tracks — recompute from both sources
        self._tracks  = []
        self._lookup  = {}
        self._current_track = None
        self._header_widget.setVisible(False)
        self._sep2.setVisible(False)
        self._table.setVisible(False)
        self._select_btn.setEnabled(False)
        if self._model is not None:
            self._status_lbl.setText("Computing tracks\u2026")
            QApplication.processEvents()
            self._on_compute()
        else:
            has_resim = model is not None
            self._status_lbl.setText(
                "Reprocessed data" + (" loaded." if has_resim else " unloaded.") +
                "  Load an original MF4 first."
            )

    def show_for_object(self, obj, scan_idx: int) -> None:
        """Show the track for the object clicked in Plan View."""
        if not self._tracks:
            self._status_lbl.setText(
                "Tracks not yet computed."
            )
            return

        data_source = getattr(obj, '_data_source', 'Original')
        key = (obj.side, obj.tracking_id, scan_idx, data_source)
        track = self._lookup.get(key)
        if track is None:
            side_str = "L" if obj.side == "L" else "R"
            self._status_lbl.setText(
                f"No {data_source.lower()} track found for {side_str}:TID {obj.tracking_id} "
                f"at scan {scan_idx}.  "
                f"(Object may not be Moving or Stopped at this frame.)"
            )
            self._header_widget.setVisible(False)
            self._sep2.setVisible(False)
            self._table.setVisible(False)
            return

        self._current_track = track
        self._current_scan  = scan_idx

        # Order: TrackingId first, then sorted non-enum, then sorted enum
        all_sigs = sorted(track.stats.keys())
        tid_sigs   = [s for s in all_sigs if s == "TrackingId"]
        non_enum   = [s for s in all_sigs if s not in _ENUM_SIGNALS
                      and s != "TrackingId"]
        enum_sigs  = [s for s in all_sigs if s in _ENUM_SIGNALS]
        self._sig_names = tid_sigs + sorted(non_enum) + sorted(enum_sigs)

        self._populate_header(track)
        self._populate_table(track, scan_idx)
        self._header_widget.setVisible(True)
        self._sep2.setVisible(True)
        self._table.setVisible(True)
        self._jump_btn.setEnabled(True)

    def update_scan(self, scan_idx: int) -> None:
        """Refresh the Current column when the active frame changes."""
        if self._current_track is None:
            return
        self._current_scan = scan_idx
        self._update_current_column(self._current_track, scan_idx)

    def clear_selection(self) -> None:
        """Deselect the current track (called when user clicks empty plan-view space)."""
        self._current_track = None
        self._current_scan  = None
        self._sig_names     = []
        self._table.setRowCount(0)
        self._table.setVisible(False)
        self._header_widget.setVisible(False)
        self._sep2.setVisible(False)
        self._plot_btn.setEnabled(False)
        self._add_to_plot_btn.setEnabled(False)
        self._jump_btn.setEnabled(False)
        if self._tracks:
            n_orig  = sum(1 for t in self._tracks if t.data_source == "Original")
            n_resim = sum(1 for t in self._tracks if t.data_source == "Resim")
            parts = [f"{n_orig} original"]
            if n_resim:
                parts.append(f"{n_resim} resim")
            self._status_lbl.setText(
                f"{', '.join(parts)} track{'s' if len(self._tracks) != 1 else ''} found.  "
                "Click a Moving or Stopped object in Plan View to inspect its track."
            )

    def set_theme(self, theme: str) -> None:
        self.setStyleSheet(_LIGHT_STYLE if theme == "light" else _DARK_STYLE)

    # ------------------------------------------------------------------
    # Private helpers
    # ------------------------------------------------------------------

    def _on_select_track(self) -> None:
        """Open the track-list dialog and display the chosen track."""
        if not self._tracks:
            return
        # Stationary-only tracks are kept for KPI matching but hidden from UI.
        visible = [t for t in self._tracks if not t.is_stationary_only]
        dlg = _TrackListDialog(visible, self._model, self)
        dlg.track_selected.connect(self._show_track_directly)
        dlg.exec()

    def _show_track_directly(self, track: ObjectTrack) -> None:
        """Display a track chosen from the track-list dialog."""
        self._current_track = track
        self._current_scan  = track.start_scan

        all_sigs = sorted(track.stats.keys())
        tid_sigs  = [s for s in all_sigs if s == "TrackingId"]
        non_enum  = [s for s in all_sigs if s not in _ENUM_SIGNALS and s != "TrackingId"]
        enum_sigs = [s for s in all_sigs if s in _ENUM_SIGNALS]
        self._sig_names = tid_sigs + sorted(non_enum) + sorted(enum_sigs)

        self._populate_header(track)
        self._populate_table(track, track.start_scan)
        self._header_widget.setVisible(True)
        self._sep2.setVisible(True)
        self._table.setVisible(True)
        self._jump_btn.setEnabled(True)

    def _on_jump_clicked(self) -> None:
        """Emit jump_to_scan_requested with the track's start_scan."""
        if self._current_track is not None:
            self.jump_to_scan_requested.emit(self._current_track.start_scan)

    def _on_compute(self) -> None:
        if self._model is None:
            return

        self._current_track = None
        self._header_widget.setVisible(False)
        self._sep2.setVisible(False)
        self._table.setVisible(False)
        has_resim = self._resim_model is not None
        self._status_lbl.setText(
            "Computing tracks from Original" +
            (" + Resim…" if has_resim else "…")
        )
        QApplication.processEvents()

        prog = QProgressDialog(
            "Computing object tracks…", None, 0, 100, self)
        prog.setWindowModality(Qt.WindowModality.WindowModal)
        prog.setWindowTitle("Object Tracks")
        prog.setMinimumDuration(0)
        prog.setValue(0)
        prog.show()
        QApplication.processEvents()

        def _cb(pct: int, msg: str) -> None:
            prog.setLabelText(msg)
            prog.setValue(pct)
            QApplication.processEvents()

        # When both sources are loaded split the bar 50/50; otherwise use full range.
        if has_resim:
            def _cb_orig(pct: int, msg: str) -> None:
                _cb(pct // 2, f"[Original] {msg}")

            orig_tracks = build_object_tracks(
                self._model, progress_cb=_cb_orig, data_source="Original")

            def _cb_resim(pct: int, msg: str) -> None:
                _cb(50 + pct // 2, f"[Resim] {msg}")
            resim_tracks = build_object_tracks(
                self._resim_model, progress_cb=_cb_resim, data_source="Resim")
        else:
            orig_tracks = build_object_tracks(
                self._model, progress_cb=_cb, data_source="Original")
            resim_tracks = []

        all_tracks = orig_tracks + resim_tracks

        # Build unified lookup: (side, tid, scan_idx, data_source) → track
        lookup: Dict[Tuple[str, int, int, str], ObjectTrack] = {}
        for track in all_tracks:
            for rec in track.records:
                lookup[(track.side, track.tracking_id,
                        rec.scan_idx, track.data_source)] = track

        self._tracks = all_tracks
        self._lookup = lookup

        prog.close()

        n_orig  = len(orig_tracks)
        n_resim = len(resim_tracks)
        # Stationary-only tracks are invisible to the user but kept for KPI.
        n_orig_vis  = sum(1 for t in orig_tracks  if not t.is_stationary_only)
        n_resim_vis = sum(1 for t in resim_tracks if not t.is_stationary_only)
        parts = [f"{n_orig_vis} original"]
        if has_resim:
            parts.append(f"{n_resim_vis} resim")
        n_vis = n_orig_vis + (n_resim_vis if has_resim else 0)
        self._status_lbl.setText(
            f"{', '.join(parts)} track{'s' if n_vis != 1 else ''} found.  "
            "Click a Moving or Stopped object in Plan View to inspect its track."
        )
        self._select_btn.setEnabled(True)
        self.tracks_computed.emit(all_tracks)

    def _populate_header(self, track: ObjectTrack) -> None:
        side_str = "Left (SRRL)" if track.side == "L" else "Right (SRRR)"
        src_str = track.data_source   # "Original" or "Resim"

        def _ds(si: int) -> int:
            return self._model.display_scan(si, track.side) if self._model else si

        self._lbl_uuid.setText(f"UUID: {track.uuid}  \u2502  Source: {src_str}")
        self._lbl_info.setText(
            f"Side: {side_str}  \u2502  TID: {track.tracking_id}  \u2502  "
            f"Scans: {_ds(track.start_scan)} \u2192 {_ds(track.end_scan)}  \u2502  "
            f"Duration: {track.duration:.3f} s  \u2502  "
            f"Frames observed: {len(track.records)}"
        )
        # ID drops on a separate elided line
        if track.id_drops:
            all_strs = [f"{_ds(a)}\u2192{_ds(b)}" for a, b in track.id_drops]
            _MAX_SHOWN = 5
            if len(all_strs) > _MAX_SHOWN:
                shown = ", ".join(all_strs[:_MAX_SHOWN])
                label_str = (f"ID drops: {len(track.id_drops)}  "
                             f"(between scans: {shown} … +{len(all_strs) - _MAX_SHOWN} more)")
                self._lbl_drops.setToolTip(
                    "All ID drops (between scans):\n" +
                    ", ".join(all_strs)
                )
            else:
                label_str = (f"ID drops: {len(track.id_drops)}  "
                             f"(between scans: {', '.join(all_strs)})")
                self._lbl_drops.setToolTip("")
            self._lbl_drops.setText(label_str)
            self._lbl_drops.setVisible(True)
        else:
            self._lbl_drops.setToolTip("")
            self._lbl_drops.setVisible(False)

    def _populate_table(self, track: ObjectTrack, scan_idx: int) -> None:
        rec = track.get_record_at(scan_idx)
        # Build a stripped-name → raw_signal_value lookup for the current record
        cur_vals = {}
        if rec is not None:
            for k, v in rec.raw_signals.items():
                cur_vals[strip_slot_suffix(k)] = v
        self._table.setRowCount(len(self._sig_names))
        for row, sig in enumerate(self._sig_names):
            stats = track.stats[sig]
            is_enum = sig in _ENUM_SIGNALS
            # Signal name + unit in column 0
            unit = self._signal_units.get(sig, "")
            display_name = f"{sig} [{unit}]" if (unit and not is_enum) else sig
            self._table.setItem(row, 0, QTableWidgetItem(display_name))

            if is_enum:
                # Col 2: Initial State (decoded)
                init_str = (_decode_enum(sig, stats.initial)
                            if stats.initial == stats.initial  # not NaN
                            else "—")
                self._table.setItem(row, 2, QTableWidgetItem(init_str))
                # Col 3: Last State (decoded)
                last_str = (_decode_enum(sig, stats.last)
                            if stats.last == stats.last
                            else "—")
                self._table.setItem(row, 3, QTableWidgetItem(last_str))
                # Col 4: Avg → not meaningful for enum
                avg_item = QTableWidgetItem("—")
                avg_item.setForeground(Qt.GlobalColor.gray)
                self._table.setItem(row, 4, avg_item)
                # Col 5: Median (decoded)
                med_str = (_decode_enum(sig, stats.median)
                           if stats.median == stats.median
                           else "—")
                self._table.setItem(row, 5, QTableWidgetItem(med_str))
                # Col 1: Current (decoded)
                if sig in cur_vals:
                    try:
                        cur_str = _decode_enum(sig, float(cur_vals[sig]))
                    except (TypeError, ValueError):
                        cur_str = str(cur_vals[sig])
                    item = QTableWidgetItem(cur_str)
                else:
                    item = QTableWidgetItem("—")
                    item.setForeground(Qt.GlobalColor.gray)
            else:
                self._table.setItem(row, 2, QTableWidgetItem(f"{stats.min:.4g}"))
                self._table.setItem(row, 3, QTableWidgetItem(f"{stats.max:.4g}"))
                self._table.setItem(row, 4, QTableWidgetItem(f"{stats.avg:.4g}"))
                self._table.setItem(row, 5, QTableWidgetItem(f"{stats.median:.4g}"))
                item = QTableWidgetItem(self._current_value_str(cur_vals, sig))
                if sig not in cur_vals:
                    item.setForeground(Qt.GlobalColor.gray)

            self._table.setItem(row, 1, item)

    def _update_current_column(self, track: ObjectTrack,
                                scan_idx: int) -> None:
        rec = track.get_record_at(scan_idx)
        cur_vals = {}
        if rec is not None:
            for k, v in rec.raw_signals.items():
                cur_vals[strip_slot_suffix(k)] = v
        for row, sig in enumerate(self._sig_names):
            if sig in _ENUM_SIGNALS:
                if sig in cur_vals:
                    try:
                        cur_str = _decode_enum(sig, float(cur_vals[sig]))
                    except (TypeError, ValueError):
                        cur_str = str(cur_vals[sig])
                    item = QTableWidgetItem(cur_str)
                else:
                    item = QTableWidgetItem("—")
                    item.setForeground(Qt.GlobalColor.gray)
            else:
                item = QTableWidgetItem(self._current_value_str(cur_vals, sig))
                if sig not in cur_vals:
                    item.setForeground(Qt.GlobalColor.gray)
            self._table.setItem(row, 1, item)

    def _on_selection_changed(self) -> None:
        """Enable Plot / Add to Plot buttons whenever at least one row is selected."""
        has_sel = bool(self._table.selectedItems()) and self._current_track is not None
        self._plot_btn.setEnabled(has_sel)
        self._add_to_plot_btn.setEnabled(has_sel)

    def _on_plot_clicked(self) -> None:
        """Build DataFrames for selected signals and emit plot_signals_requested."""
        signals_list = self._build_signals_list()
        if signals_list:
            self.plot_signals_requested.emit(signals_list)

    def _on_add_to_plot_clicked(self) -> None:
        """Build DataFrames for selected signals and emit add_signals_requested."""
        signals_list = self._build_signals_list()
        if signals_list:
            self.add_signals_requested.emit(signals_list)

    def _build_signals_list(self) -> list:
        """Return [(track_label, sig_name, df)] for the currently selected rows."""
        if self._current_track is None:
            return []
        track = self._current_track

        selected_sigs: List[str] = []
        seen: set = set()
        for item in self._table.selectedItems():
            row = item.row()
            # Use _sig_names (raw name) not column 0 text (which may include unit)
            sig = self._sig_names[row] if row < len(self._sig_names) else ""
            if sig and sig not in seen:
                seen.add(sig)
                selected_sigs.append(sig)

        if not selected_sigs:
            return []

        timestamps = [r.timestamp for r in track.records]
        rows = []
        for rec in track.records:
            cur_vals: dict = {}
            for k, v in rec.raw_signals.items():
                cur_vals[strip_slot_suffix(k)] = v
            rows.append(cur_vals)
        full_df = pd.DataFrame(rows, index=timestamps)
        full_df.index.name = "obj_ts"     # signals the plot widget to use obj_ts-space lookup
        full_df.sort_index(inplace=True)  # guarantee monotonic order (safety net)

        track_label = f"Track {track.side}{track.tracking_id}"
        result = []
        for sig in selected_sigs:
            if sig not in full_df.columns:
                continue
            df_sig = pd.to_numeric(full_df[sig], errors="coerce").to_frame()
            result.append((track_label, sig, df_sig))
        return result

    @staticmethod
    def _current_value_str(cur_vals: dict, sig: str) -> str:
        """cur_vals is a stripped_name → raw_value dict."""
        if sig not in cur_vals:
            return "—"
        v = cur_vals[sig]
        try:
            return f"{float(v):.4g}"
        except (TypeError, ValueError):
            return str(v)
