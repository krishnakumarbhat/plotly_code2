"""
resim_comparison_widget.py
==========================
Widget that computes and displays resim vs. original KPI results.

Requirements
------------
- Both original and reprocessed DataModels must be set via set_models().
- Object Tracks must have been computed in ObjectTrackWidget and forwarded
  via set_tracks(all_tracks).
- Clicking "Compare Resim" runs both Frame-to-Frame and Track-to-Track analyses.
- Results are shown per radar side (Left / Right).
- "Export XLSX" exports all results to an Excel workbook (one sheet per radar side).
- "Export JSON" exports all results to a structured JSON file.
"""

from __future__ import annotations

import json
from typing import Dict, List, Optional

from PyQt6.QtCore import Qt, pyqtSignal
from PyQt6.QtGui import QColor
from PyQt6.QtWidgets import (
    QApplication,
    QFileDialog,
    QGroupBox,
    QHBoxLayout,
    QHeaderView,
    QLabel,
    QProgressDialog,
    QPushButton,
    QScrollArea,
    QSizePolicy,
    QSplitter,
    QTabWidget,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
)

from object_track import ObjectTrack
from resim_kpi import (
    DeltaStats,
    FrameKpiResult,
    SideFrameKpi,
    SideTrackKpi,
    TrackKpiResult,
    TrackMatch,
    _KPI_SIGNALS,
    _MIN_TRACK_DURATION_S,
    _roi_mult_track,
    compute_frame_kpi,
    compute_track_kpi,
)


# ---------------------------------------------------------------------------
# Stylesheets
# ---------------------------------------------------------------------------

_DARK = """
    QWidget          { background:#12122a; color:#ccccff; }
    QGroupBox        { color:#8888cc; border:1px solid #444466;
                       margin-top:8px; padding:6px; }
    QGroupBox::title { subcontrol-origin:margin; left:6px; color:#8888cc; }
    QLabel           { color:#ccccff; font-size:11px; }
    QPushButton      { background:#2a2a5e; color:#ccccff;
                       border:1px solid #555588; border-radius:3px;
                       padding:4px 14px; }
    QPushButton:hover    { background:#3a3a7e; }
    QPushButton:disabled { color:#555577; border-color:#333355; }
    QTabWidget::pane     { border:1px solid #444466; }
    QTabBar::tab         { background:#2a2a4e; color:#aaaacc;
                           padding:4px 12px; margin-right:2px; }
    QTabBar::tab:selected { background:#3a3a6e; color:#ffffff; }
    QTableWidget     { background:#1a1a2e; color:#ccccff;
                       gridline-color:#333366; font-size:10px; }
    QTableWidget::item            { background:#1a1a2e; }
    QTableWidget::item:alternate  { background:#16162a; }
    QTableWidget::item:selected   { background:#3a3a7e; }
    QHeaderView::section { background:#12122a; color:#aaaacc;
                           border:none; padding:3px; font-size:10px; }
    QScrollArea { border:none; }
"""

_LIGHT = """
    QWidget          { background:#f5f5f5; color:#111111; }
    QGroupBox        { color:#444444; border:1px solid #aaaaaa;
                       margin-top:8px; padding:6px; }
    QGroupBox::title { subcontrol-origin:margin; left:6px; color:#444444; }
    QLabel           { color:#333333; font-size:11px; }
    QPushButton      { background:#e0e0e0; color:#111111;
                       border:1px solid #aaaaaa; border-radius:3px;
                       padding:4px 14px; }
    QPushButton:hover    { background:#cccccc; }
    QPushButton:disabled { color:#aaaaaa; border-color:#cccccc; }
    QTabWidget::pane     { border:1px solid #aaaaaa; }
    QTabBar::tab         { background:#e8e8e8; color:#555555;
                           padding:4px 12px; margin-right:2px; }
    QTabBar::tab:selected { background:#ffffff; color:#000000; }
    QTableWidget     { background:#ffffff; color:#111111;
                       gridline-color:#cccccc; font-size:10px; }
    QHeaderView::section { background:#eeeeee; color:#333333;
                           border:none; padding:3px; font-size:10px; }
    QScrollArea { border:none; }
"""

# Signal display names and units
_SIG_LABELS = {
    "LonPosition":  ("LonPosition",  "m"),
    "LatPosition":  ("LatPosition",  "m"),
    "LonGndVel":    ("LonGndVel",    "m/s"),
    "LatGndVel":    ("LatGndVel",    "m/s"),
    "HeadingAngle": ("HeadingAngle", "°"),
}

# Tolerance thresholds for display colour-coding (must match resim_kpi._TOLERANCES)
# HeadingAngle is intentionally absent: it is informational only and not scored.
_DISPLAY_TOLERANCES: Dict[str, float] = {
    "LonPosition":  0.5,
    "LatPosition":  0.5,
    "LonGndVel":    1.0,
    "LatGndVel":    1.0,
}

_STATS_COLS   = ["Signal", "Unit", "N", "Mean Δ", "RMSE", "Median", "P95 |Δ|",
                 "Tolerance", "Quality"]
_TRACK_COLS = [
    "Source",
    "Orig TID", "Resim TID",
    "Scan Start", "Scan End", "Duration [s]",
    "ROI",
    "Orig Cov [%]", "Resim Cov [%]", "Mean IoU",
    "ΔLon mean", "ΔLat mean", "ΔLonVel mean", "ΔLatVel mean", "ΔHead mean [°]",
]


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _fmt(v: float, decimals: int = 4) -> str:
    """Format float without scientific notation."""
    if v != v:
        return "nan"
    return f"{v:.{decimals}f}"


def _fill_stats_table(table: QTableWidget,
                      signal_stats: Dict[str, DeltaStats]) -> None:
    _GREEN  = QColor(40,  160, 80)
    _AMBER  = QColor(200, 140, 30)
    _RED    = QColor(200,  60, 60)

    sigs = [s for s, _ in _KPI_SIGNALS if s in signal_stats]
    table.setRowCount(len(sigs))
    for row, sig in enumerate(sigs):
        st  = signal_stats[sig]
        tol = _DISPLAY_TOLERANCES.get(sig)
        is_heading = (sig == "HeadingAngle")

        # Quality label + colour
        if tol is not None and st.n > 0:
            if st.p95 <= tol:
                quality_txt = "PASS"
                qual_colour = _GREEN
            elif st.p95 <= 4.0 * tol:
                quality_txt = "WARN"
                qual_colour = _AMBER
            else:
                quality_txt = "FAIL"
                qual_colour = _RED
        else:
            # HeadingAngle (and any unrecognised signal): no quality rating
            quality_txt = ""
            qual_colour = None

        tol_txt = "N/A" if is_heading else (f"\u00b1{tol}" if tol is not None else "\u2014")

        values = [
            sig, st.unit,
            str(st.n),
            _fmt(st.mean),
            _fmt(st.rmse),
            _fmt(st.median),
            _fmt(st.p95),
            tol_txt,
            quality_txt,
        ]
        for col, txt in enumerate(values):
            item = QTableWidgetItem(txt)
            item.setTextAlignment(Qt.AlignmentFlag.AlignCenter
                                  if col > 0 else
                                  Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter)
            if col == len(values) - 1 and qual_colour is not None:
                item.setForeground(qual_colour)
            table.setItem(row, col, item)


def _roi_label(track) -> str:
    """Return '1', '2', '3', or 'None' for the best ROI zone the track reached."""
    mult = _roi_mult_track(track)
    if mult >= 1.0:
        return "1"
    if mult >= 0.5:
        return "2"
    if mult > 0.0:
        return "3"
    return "None"


def _make_stats_table() -> QTableWidget:
    t = QTableWidget(0, len(_STATS_COLS))
    t.setHorizontalHeaderLabels(_STATS_COLS)
    t.horizontalHeader().setSectionResizeMode(0, QHeaderView.ResizeMode.Stretch)
    for c in range(1, len(_STATS_COLS)):
        t.horizontalHeader().setSectionResizeMode(c, QHeaderView.ResizeMode.ResizeToContents)
    t.verticalHeader().setVisible(False)
    t.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
    t.setAlternatingRowColors(True)
    return t


def _make_track_table() -> QTableWidget:
    t = QTableWidget(0, len(_TRACK_COLS))
    t.setHorizontalHeaderLabels(_TRACK_COLS)
    hdr = t.horizontalHeader()
    for c in range(len(_TRACK_COLS)):
        hdr.setSectionResizeMode(c, QHeaderView.ResizeMode.ResizeToContents)
    t.verticalHeader().setVisible(False)
    t.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
    t.setAlternatingRowColors(True)
    t.setSortingEnabled(True)
    t.setSelectionBehavior(QTableWidget.SelectionBehavior.SelectRows)
    t.setSelectionMode(QTableWidget.SelectionMode.SingleSelection)
    return t


def _fill_track_table(table: QTableWidget, matches: List[TrackMatch]) -> None:
    # Stationary-only tracks are used for KPI scoring but not shown to the user.
    visible = [m for m in matches
               if not (m.orig_track  is not None and m.orig_track.is_stationary_only)
               and not (m.resim_track is not None and m.resim_track.is_stationary_only)]
    table.setSortingEnabled(False)
    table.setRowCount(len(visible))
    _CTR         = Qt.AlignmentFlag.AlignCenter
    _ORANGE      = QColor(210, 140,  40)   # ghost (resim-only, no overlap)
    _OUTCOMPETED = QColor(130, 160, 210)   # resim-only but overlapping orig
    _RED         = QColor(220,  80,  80)   # orig-only (unmatched) rows
    _GREY        = QColor(120, 120, 140)   # short events (excluded from scoring)

    for row, m in enumerate(visible):
        ot = m.orig_track
        rt = m.resim_track
        is_matched       = ot is not None and rt is not None and not m.is_short
        is_orig_only     = ot is not None and rt is None     and not m.is_short
        is_resim_only    = ot is None     and rt is not None and not m.is_short
        is_ghost         = is_resim_only and m.is_ghost
        is_outcompeted   = is_resim_only and not m.is_ghost
        is_short_evt     = m.is_short
        ref = ot if ot is not None else rt   # track used for scan meta

        def _item(txt: str, num: Optional[float] = None) -> QTableWidgetItem:
            it = QTableWidgetItem(txt)
            it.setTextAlignment(_CTR)
            if num is not None:
                it.setData(Qt.ItemDataRole.UserRole, num)
            return it

        src_lbl = ("Short event"  if is_short_evt
                   else "Matched"      if is_matched
                   else "Orig only"   if is_orig_only
                   else "Outcompeted" if is_outcompeted
                   else "Resim only")
        src_item = _item(src_lbl)
        src_item.setData(Qt.ItemDataRole.UserRole + 1, (ot or rt).side)
        table.setItem(row, 0, src_item)
        table.setItem(row, 1, _item(str(ot.tracking_id), float(ot.tracking_id)) if ot else _item("—"))
        table.setItem(row, 2, _item(str(rt.tracking_id), float(rt.tracking_id)) if rt else _item("—"))
        table.setItem(row, 3, _item(str(ref.start_scan), float(ref.start_scan)))
        table.setItem(row, 4, _item(str(ref.end_scan),   float(ref.end_scan)))
        table.setItem(row, 5, _item(_fmt(ref.duration, 3), ref.duration))

        # ROI column (col 6) — best zone the reference track ever entered
        roi_txt = _roi_label(ref)
        roi_item = _item(roi_txt)
        _ROI_COLOURS = {"1": QColor(60, 160, 80), "2": QColor(200, 140, 30),
                        "3": QColor(120, 120, 140), "None": QColor(200, 60, 60)}
        roi_item.setForeground(_ROI_COLOURS.get(roi_txt, QColor(200, 60, 60)))
        table.setItem(row, 6, roi_item)

        if is_matched:
            table.setItem(row, 7, _item(_fmt(m.coverage_pct,       1), m.coverage_pct))
            table.setItem(row, 8, _item(_fmt(m.resim_coverage_pct, 1), m.resim_coverage_pct))
            table.setItem(row, 9, _item(_fmt(m.mean_iou,           3), m.mean_iou))
        else:
            table.setItem(row, 7, _item("—"))
            table.setItem(row, 8, _item("—"))
            table.setItem(row, 9, _item("—"))

        sig_keys = ["LonPosition", "LatPosition", "LonGndVel",
                    "LatGndVel", "HeadingAngle"]
        for ci, sig in enumerate(sig_keys, start=10):
            if is_matched and sig in m.signal_stats:
                val = m.signal_stats[sig].mean
                table.setItem(row, ci, _item(_fmt(val), val))
            else:
                table.setItem(row, ci, _item("—"))

        # Colour non-matched rows — applied AFTER all items are set so
        # every column (0–14) is covered.
        fg = (_GREY        if is_short_evt  else
              _RED         if is_orig_only   else
              _OUTCOMPETED if is_outcompeted else
              _ORANGE      if is_ghost       else None)
        if fg is not None:
            for c in range(len(_TRACK_COLS)):
                item = table.item(row, c)
                if item:
                    item.setForeground(fg)

    table.setSortingEnabled(True)


# ---------------------------------------------------------------------------
# Side panel widget (used inside both tabs)
# ---------------------------------------------------------------------------

class _SidePanel(QWidget):
    """Displays KPI results for one radar side in one analysis mode."""

    jump_requested = pyqtSignal(int, str, int, int, int)  # scan_idx, side, orig_tid, resim_tid (-1 = none)

    def __init__(self, side_label: str, include_track_table: bool = False,
                 parent=None):
        super().__init__(parent)
        self._include_track_table = include_track_table

        lay = QVBoxLayout(self)
        lay.setContentsMargins(4, 4, 4, 4)
        lay.setSpacing(6)

        grp = QGroupBox(side_label)
        grp_lay = QVBoxLayout(grp)
        grp_lay.setSpacing(4)

        # -- RESIM Score row
        score_row = QWidget()
        score_row_lay = QHBoxLayout(score_row)
        score_row_lay.setContentsMargins(0, 0, 0, 0)
        score_row_lay.setSpacing(12)

        self._score_lbl = QLabel("\u2014")
        self._score_lbl.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self._score_lbl.setStyleSheet(
            "font-size:26px; font-weight:bold; padding:4px 12px; "
            "border:1px solid #555588; border-radius:4px;"
        )
        self._score_lbl.setFixedWidth(90)
        score_row_lay.addWidget(self._score_lbl)

        self._score_detail_lbl = QLabel("")
        self._score_detail_lbl.setWordWrap(True)
        score_row_lay.addWidget(self._score_detail_lbl, stretch=1)
        grp_lay.addWidget(score_row)

        self._summary_lbl = QLabel("No data.")
        self._summary_lbl.setWordWrap(True)
        grp_lay.addWidget(self._summary_lbl)

        self._stats_table = _make_stats_table()

        if include_track_table:
            grp_lay.addWidget(self._stats_table)
            detail_lbl = QLabel(
                "Per-track detail  "
                "(red\u202f=\u202fOrig only, orange\u202f=\u202fResim only, "
                "blue\u202f=\u202fOutcompeted, grey\u202f=\u202fShort event)"
            )
            grp_lay.addWidget(detail_lbl)
            self._track_table = _make_track_table()
            self._track_table.itemSelectionChanged.connect(self._on_track_row_selected)
            grp_lay.addWidget(self._track_table, stretch=1)
            self._jump_btn = QPushButton("Jump to track")
            self._jump_btn.setEnabled(False)
            self._jump_btn.clicked.connect(self._on_jump_to_track)
            grp_lay.addWidget(self._jump_btn)
        else:
            grp_lay.addWidget(self._stats_table, stretch=1)
            self._track_table = None
            self._jump_btn = None

        lay.addWidget(grp, stretch=1)

    def _on_track_row_selected(self) -> None:
        """Enable the Jump button when a row is selected in the track table."""
        if self._jump_btn is not None:
            self._jump_btn.setEnabled(bool(self._track_table.selectedItems()))

    def _on_jump_to_track(self) -> None:
        """Emit jump_requested for the currently selected track table row."""
        rows = self._track_table.selectionModel().selectedRows()
        if not rows:
            return
        row = rows[0].row()
        scan_item  = self._track_table.item(row, 3)   # Scan Start
        end_item   = self._track_table.item(row, 4)   # Scan End
        side_item  = self._track_table.item(row, 0)   # Source (stores side in UserRole+1)
        orig_item  = self._track_table.item(row, 1)   # Orig TID
        resim_item = self._track_table.item(row, 2)   # Resim TID
        if scan_item is None or side_item is None:
            return
        scan_idx  = int(scan_item.data(Qt.ItemDataRole.UserRole) or 0)
        scan_end  = int(end_item.data(Qt.ItemDataRole.UserRole) or scan_idx) if end_item else scan_idx
        side_str  = side_item.data(Qt.ItemDataRole.UserRole + 1) or ""
        orig_val  = orig_item.data(Qt.ItemDataRole.UserRole)  if orig_item  else None
        resim_val = resim_item.data(Qt.ItemDataRole.UserRole) if resim_item else None
        orig_tid  = int(orig_val)  if orig_val  is not None else -1
        resim_tid = int(resim_val) if resim_val is not None else -1
        if side_str:
            self.jump_requested.emit(scan_idx, side_str, orig_tid, resim_tid, scan_end)

    def _update_score(self, resim_score: float, detection: float,
                      ghost: float, accuracy: float) -> None:
        """Refresh the RESIM Score label with colour and breakdown text."""
        score_int = int(round(resim_score))
        if resim_score >= 90.0:
            colour = "#3cb371"   # green
        elif resim_score >= 70.0:
            colour = "#d4a017"   # amber
        else:
            colour = "#dc143c"   # red

        self._score_lbl.setText(f"{score_int}%")
        self._score_lbl.setStyleSheet(
            f"font-size:26px; font-weight:bold; padding:4px 12px; "
            f"border:1px solid {colour}; border-radius:4px; color:{colour};"
        )
        self._score_detail_lbl.setText(
            f"<b>RESIM Score</b>  "
            f"<span style='color:#7799cc;'>TP Events:</span> {detection:.1f}%  │  "
            f"<span style='color:#7799cc;'>FP Events:</span> {ghost:.1f}%  │  "
            f"<span style='color:#7799cc;'>Accuracy:</span> {accuracy:.1f}%"
        )

    def update_frame(self, kpi: SideFrameKpi) -> None:
        self._update_score(kpi.resim_score, kpi.tp_score,
                           kpi.fp_score, kpi.accuracy_score)
        self._summary_lbl.setText(
            f"Original objects: {kpi.total}  │  "
            f"Resim objects: {kpi.total_resim}  │  "
            f"Matched: {kpi.matched}  │  "
            f"Unmatched: {kpi.total - kpi.matched}  │  "
            f"Mean IoU: {kpi.mean_iou:.3f}"
        )
        _fill_stats_table(self._stats_table, kpi.signal_stats)

    def update_track(self, kpi: SideTrackKpi) -> None:
        self._update_score(kpi.resim_score, kpi.tp_score,
                           kpi.fp_score, kpi.accuracy_score)
        n_matched     = sum(1 for m in kpi.matches
                            if m.orig_track and m.resim_track and not m.is_short)
        n_orig_only   = sum(1 for m in kpi.matches
                            if m.orig_track and not m.resim_track and not m.is_short)
        n_resim_only  = sum(1 for m in kpi.matches
                            if not m.orig_track and m.resim_track
                            and not m.is_short and m.is_ghost)
        n_outcompeted = sum(1 for m in kpi.matches
                            if not m.orig_track and m.resim_track
                            and not m.is_short and not m.is_ghost)
        n_short       = sum(1 for m in kpi.matches if m.is_short)
        self._summary_lbl.setText(
            f"Original tracks: {kpi.n_orig}  │  "
            f"Reprocessed tracks: {kpi.n_resim}\n"
            f"Matched: {n_matched}  │  "
            f"Orig only: {n_orig_only}  │  "
            f"Resim only: {n_resim_only}  │  "
            f"Outcompeted: {n_outcompeted}  │  "
            f"Short event (<200\u202fms): {n_short}"
        )
        _fill_stats_table(self._stats_table, kpi.signal_stats)
        if self._track_table is not None:
            _fill_track_table(self._track_table, kpi.matches)

    def clear(self) -> None:
        self._score_lbl.setText("—")
        self._score_lbl.setStyleSheet(
            "font-size:26px; font-weight:bold; padding:4px 12px; "
            "border:1px solid #555588; border-radius:4px;"
        )
        self._score_detail_lbl.setText("")
        self._summary_lbl.setText("No data.")
        self._stats_table.setRowCount(0)
        if self._track_table is not None:
            self._track_table.setRowCount(0)


# ---------------------------------------------------------------------------
# Main widget
# ---------------------------------------------------------------------------

class ResimComparisonWidget(QWidget):
    """
    Resim Comparison panel — Frame-to-Frame and Track-to-Track KPI.

    Call set_models(orig_model, resim_model) when both files are loaded.
    Call set_tracks(all_tracks)             when tracks are computed.
    """

    jump_requested = pyqtSignal(int, str, int, int, int)  # scan_idx, side, orig_tid, resim_tid, scan_end

    def __init__(self, parent=None):
        super().__init__(parent)
        self._orig_model  = None
        self._resim_model = None
        self._orig_tracks:  List[ObjectTrack] = []
        self._resim_tracks: List[ObjectTrack] = []
        self._frame_result: Optional[FrameKpiResult] = None
        self._track_result: Optional[TrackKpiResult] = None
        self._coverage_banner = None   # created in _build_ui

        self._build_ui()
        self.setStyleSheet(_DARK)

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def set_models(self, orig_model, resim_model) -> None:
        self._orig_model  = orig_model
        self._resim_model = resim_model
        self._frame_result = None
        self._track_result = None
        self._clear_panels()
        self._update_button_state()
        self._update_coverage_banner()
        if orig_model is None or resim_model is None:
            self._status_lbl.setText(
                "Load both original and reprocessed MF4 files "
                "(File → Load reprocessed MF4…) to enable KPI analysis."
            )
        else:
            self._status_lbl.setText(
                "Both files loaded.  "
                + ("Compute Tracks in 'Object Track' tab for Track-to-Track KPI, "
                   "then click 'Compare Resim'."
                   if not (self._orig_tracks or self._resim_tracks) else
                   "Click 'Compare Resim' to run analysis.")
            )

    def set_tracks(self, all_tracks: List[ObjectTrack]) -> None:
        """Called after ObjectTrackWidget computes tracks (both sources)."""
        self._orig_tracks  = [t for t in all_tracks if t.data_source == "Original"]
        self._resim_tracks = [t for t in all_tracks if t.data_source == "Resim"]
        self._track_result = None
        self._update_button_state()
        n_o = len(self._orig_tracks)
        n_r = len(self._resim_tracks)
        if self._orig_model and self._resim_model:
            self._status_lbl.setText(
                f"Tracks ready ({n_o} original, {n_r} resim).  "
                "Click 'Compare Resim' to run analysis."
            )

    def set_theme(self, theme: str) -> None:
        self.setStyleSheet(_LIGHT if theme == "light" else _DARK)

    def clear(self) -> None:
        """Reset all KPI panels to their blank state (called on new file load).

        Also drops the resim model reference so that 'Compare Resim' stays
        disabled until a new reprocessed file is loaded and validated.
        """
        self._resim_model  = None
        self._orig_tracks  = []
        self._resim_tracks = []
        self._frame_result = None
        self._track_result = None
        self._clear_panels()
        self._update_button_state()
        self._status_lbl.setText(
            "Load both original and reprocessed MF4 files "
            "(File → Load reprocessed MF4…) to enable KPI analysis."
        )

    # ------------------------------------------------------------------
    # UI construction
    # ------------------------------------------------------------------

    def _build_ui(self) -> None:
        root = QVBoxLayout(self)
        root.setContentsMargins(6, 6, 6, 6)
        root.setSpacing(6)

        # ── Top control row ───────────────────────────────────────────
        ctrl = QWidget()
        ctrl_lay = QHBoxLayout(ctrl)
        ctrl_lay.setContentsMargins(0, 0, 0, 0)
        ctrl_lay.setSpacing(8)

        self._compute_btn = QPushButton("Compare Resim")
        self._compute_btn.setEnabled(False)
        self._compute_btn.setFixedWidth(120)
        self._compute_btn.clicked.connect(self._on_compute)
        ctrl_lay.addWidget(self._compute_btn)

        self._save_xlsx_btn = QPushButton("Export XLSX…")
        self._save_xlsx_btn.setEnabled(False)
        self._save_xlsx_btn.setFixedWidth(120)
        self._save_xlsx_btn.clicked.connect(self._on_save_xlsx)
        ctrl_lay.addWidget(self._save_xlsx_btn)

        self._save_json_btn = QPushButton("Export JSON…")
        self._save_json_btn.setEnabled(False)
        self._save_json_btn.setFixedWidth(120)
        self._save_json_btn.clicked.connect(self._on_save_json)
        ctrl_lay.addWidget(self._save_json_btn)

        self._status_lbl = QLabel("Load both original and reprocessed MF4 files first.")
        self._status_lbl.setWordWrap(True)
        ctrl_lay.addWidget(self._status_lbl, stretch=1)

        root.addWidget(ctrl)

        # ── Sub-tabs ──────────────────────────────────────────────────
        self._sub_tabs = QTabWidget()
        root.addWidget(self._sub_tabs, stretch=1)

        # Frame-to-Frame tab
        f2f_widget = QWidget()
        f2f_lay    = QVBoxLayout(f2f_widget)
        f2f_lay.setContentsMargins(4, 4, 4, 4)
        self._f2f_splitter = QSplitter(Qt.Orientation.Horizontal)
        self._f2f_left  = _SidePanel("Left Radar (SRRL)",  include_track_table=False)
        self._f2f_right = _SidePanel("Right Radar (SRRR)", include_track_table=False)
        self._f2f_splitter.addWidget(self._f2f_left)
        self._f2f_splitter.addWidget(self._f2f_right)
        self._f2f_splitter.setStretchFactor(0, 1)
        self._f2f_splitter.setStretchFactor(1, 1)
        f2f_lay.addWidget(self._f2f_splitter, stretch=1)
        self._sub_tabs.addTab(f2f_widget, "Frame-to-Frame")

        # Track-to-Track tab
        t2t_widget = QWidget()
        t2t_lay    = QVBoxLayout(t2t_widget)
        t2t_lay.setContentsMargins(4, 4, 4, 4)
        self._t2t_splitter = QSplitter(Qt.Orientation.Horizontal)
        self._t2t_left  = _SidePanel("Left Radar (SRRL)",  include_track_table=True)
        self._t2t_right = _SidePanel("Right Radar (SRRR)", include_track_table=True)
        self._t2t_splitter.addWidget(self._t2t_left)
        self._t2t_splitter.addWidget(self._t2t_right)
        self._t2t_splitter.setStretchFactor(0, 1)
        self._t2t_splitter.setStretchFactor(1, 1)
        self._t2t_left.jump_requested.connect(self.jump_requested)
        self._t2t_right.jump_requested.connect(self.jump_requested)
        t2t_lay.addWidget(self._t2t_splitter, stretch=1)
        self._sub_tabs.addTab(t2t_widget, "Track-to-Track")

    # ------------------------------------------------------------------
    # Private helpers
    # ------------------------------------------------------------------

    _SCAN_TOLERANCE = 10   # frames: resim may be shorter by this many without warning

    def _update_coverage_banner(self) -> None:
        """Show/hide the resim-length warning banner based on scan count difference."""
        if self._coverage_banner is None:
            return   # widget not yet built
        if self._orig_model is None or self._resim_model is None:
            self._coverage_banner.hide()
            return

        n_orig  = len(self._orig_model.scan_indices)
        n_resim = len(self._resim_model.scan_indices)
        missing = n_orig - n_resim   # positive = resim is shorter

        if missing <= self._SCAN_TOLERANCE:
            self._coverage_banner.hide()
            return

        pct_missing = 100.0 * missing / max(n_orig, 1)

        if pct_missing >= 20.0:
            bg, border, icon = "#7a1a1a", "#cc3333", "⚠️  CRITICAL"
        elif pct_missing >= 5.0:
            bg, border, icon = "#7a4a00", "#cc8800", "⚠️  WARNING"
        else:
            bg, border, icon = "#1a3a1a", "#448844", "ℹ️  NOTE"

        self._coverage_banner.setStyleSheet(
            f"background:{bg}; border:1px solid {border};"
            f" color:#ffffff; font-weight:bold; border-radius:4px;"
        )
        self._coverage_banner.setText(
            f"{icon}  Resim file is SHORTER than original: "
            f"{n_resim} scans vs {n_orig} scans "
            f"(−{missing} frames, −{pct_missing:.1f}%).  "
            f"KPI scores reflect only the overlapping portion of the recording."
        )
        self._coverage_banner.show()

    def _update_button_state(self) -> None:
        ready = (self._orig_model is not None and
                 self._resim_model is not None)
        self._compute_btn.setEnabled(ready)

    def _clear_panels(self) -> None:
        for panel in (self._f2f_left, self._f2f_right,
                      self._t2t_left, self._t2t_right):
            try:
                panel.clear()
            except AttributeError:
                pass  # panels not built yet during __init__

    def _on_compute(self) -> None:
        if self._orig_model is None or self._resim_model is None:
            return

        # Progress dialog
        prog = QProgressDialog("Running Resim Comparison…", None, 0, 100, self)
        prog.setWindowModality(Qt.WindowModality.WindowModal)
        prog.setWindowTitle("Resim Comparison")
        prog.setMinimumDuration(0)
        prog.setValue(0)
        prog.show()
        QApplication.processEvents()

        def _cb_frame(pct: int, msg: str) -> None:
            prog.setLabelText(msg)
            prog.setValue(int(pct * 0.55))   # frame = 0–55%
            QApplication.processEvents()

        def _cb_track(pct: int, msg: str) -> None:
            prog.setLabelText(msg)
            prog.setValue(55 + int(pct * 0.45))   # track = 55–100%
            QApplication.processEvents()

        self._status_lbl.setText("Computing Frame-to-Frame KPI…")
        QApplication.processEvents()

        try:
            self._frame_result = compute_frame_kpi(
                self._orig_model, self._resim_model, _cb_frame)
        except Exception as exc:
            prog.close()
            self._status_lbl.setText(f"Frame KPI error: {exc}")
            return

        self._f2f_left.update_frame(self._frame_result.left)
        self._f2f_right.update_frame(self._frame_result.right)

        if self._orig_tracks or self._resim_tracks:
            self._status_lbl.setText("Computing Track-to-Track KPI…")
            QApplication.processEvents()
            try:
                self._track_result = compute_track_kpi(
                    self._orig_tracks, self._resim_tracks, _cb_track)
            except Exception as exc:
                prog.close()
                self._status_lbl.setText(
                    f"Track KPI error: {exc}  (Frame-to-Frame completed)")
                self._save_xlsx_btn.setEnabled(True)
                self._save_json_btn.setEnabled(True)
                return

            self._t2t_left.update_track(self._track_result.left)
            self._t2t_right.update_track(self._track_result.right)
        else:
            prog.setValue(100)
            self._status_lbl.setText(
                "Frame-to-Frame KPI done.  "
                "Compute Tracks in 'Object Track' tab to enable Track-to-Track KPI.")

        prog.close()

        if self._frame_result and self._track_result:
            self._status_lbl.setText("KPI computation complete.")
        self._save_xlsx_btn.setEnabled(self._frame_result is not None)
        self._save_json_btn.setEnabled(self._frame_result is not None)

    # ------------------------------------------------------------------
    # XLSX export
    # ------------------------------------------------------------------

    def _on_save_xlsx(self) -> None:
        if self._frame_result is None:
            return
        path, _ = QFileDialog.getSaveFileName(
            self, "Export KPI Results to Excel", "resim_kpi.xlsx",
            "Excel files (*.xlsx);;All files (*)"
        )
        if not path:
            return
        try:
            self._build_xlsx(path)
            self._status_lbl.setText(f"Saved: {path}")
        except Exception as exc:
            self._status_lbl.setText(f"Export error: {exc}")

    def _build_xlsx(self, path: str) -> None:
        """Write a .xlsx workbook with one sheet per radar side."""
        import openpyxl
        from openpyxl.styles import Font, PatternFill, Alignment

        wb = openpyxl.Workbook()
        wb.remove(wb.active)   # remove default empty sheet

        _SIDES = [
            ("Left (SRRL)",  "L", "Left SRRL"),
            ("Right (SRRR)", "R", "Right SRRR"),
        ]

        # Shared style helpers
        _bold      = Font(bold=True)
        _hdr_fill  = PatternFill("solid", fgColor="1A1A4E")
        _hdr_font  = Font(bold=True, color="CCCCFF")
        _ctr       = Alignment(horizontal="center")
        _fill_pass = PatternFill("solid", fgColor="2E7D32")
        _fill_warn = PatternFill("solid", fgColor="F57F17")
        _fill_fail = PatternFill("solid", fgColor="B71C1C")
        _font_qlt  = Font(bold=True, color="FFFFFF")

        def _hdr_row(ws, row_idx: int, labels: List[str]) -> None:
            for ci, lbl in enumerate(labels, start=1):
                c = ws.cell(row=row_idx, column=ci, value=lbl)
                c.font      = _hdr_font
                c.fill      = _hdr_fill
                c.alignment = _ctr

        def _section_title(ws, row_idx: int, title: str) -> None:
            c = ws.cell(row=row_idx, column=1, value=title)
            c.font = Font(bold=True, size=11)

        def _quality_cell(ws, row_idx: int, col_idx: int,
                          p95: float, tol: float) -> None:
            """Write PASS/WARN/FAIL with background colour."""
            if p95 <= tol:
                label, fill = "PASS", _fill_pass
            elif p95 <= 4.0 * tol:
                label, fill = "WARN", _fill_warn
            else:
                label, fill = "FAIL", _fill_fail
            c = ws.cell(row=row_idx, column=col_idx, value=label)
            c.fill      = fill
            c.font      = _font_qlt
            c.alignment = _ctr

        for side_str, side_key, sheet_name in _SIDES:
            ws = wb.create_sheet(title=sheet_name)
            r  = 1   # current row cursor

            # ── Frame-to-Frame summary ──────────────────────────────
            _section_title(ws, r, "Frame-to-Frame KPI — Summary"); r += 1
            f2f_hdrs = [
                "Orig Objects", "Matched", "Unmatched", "TP Events [%]",
                "Resim Objects", "Spurious", "Spurious Rate [%]", "Mean IoU",
                "RESIM Score", "TP Events Score", "FP Events Score", "Accuracy Score",
            ]
            _hdr_row(ws, r, f2f_hdrs); r += 1
            if self._frame_result:
                kpi = self._frame_result.left if side_key == "L" else self._frame_result.right
                row_data = [
                    kpi.total, kpi.matched, kpi.total - kpi.matched,
                    round(kpi.availability_pct, 1),
                    kpi.total_resim, kpi.ghost, round(kpi.ghost_pct, 1),
                    round(kpi.mean_iou, 3),
                    round(kpi.resim_score, 1), round(kpi.tp_score, 1),
                    round(kpi.fp_score, 1), round(kpi.accuracy_score, 1),
                ]
                for ci, v in enumerate(row_data, start=1):
                    ws.cell(row=r, column=ci, value=v).alignment = _ctr
                r += 2

                # F2F signal accuracy
                _section_title(ws, r, "Frame-to-Frame KPI — Signal Accuracy"); r += 1
                sig_hdrs = ["Signal", "Unit", "N", "Mean Δ", "RMSE",
                            "Median", "P95 |Δ|", "Tolerance", "Quality"]
                _hdr_row(ws, r, sig_hdrs); r += 1
                for sig, _ in _KPI_SIGNALS:
                    if sig not in kpi.signal_stats:
                        continue
                    st  = kpi.signal_stats[sig]
                    tol = _DISPLAY_TOLERANCES.get(sig)
                    row_vals = [
                        sig, st.unit, st.n,
                        round(st.mean, 4), round(st.rmse, 4),
                        round(st.median, 4), round(st.p95, 4),
                        f"±{tol}" if tol else "",
                    ]
                    for ci, v in enumerate(row_vals, start=1):
                        ws.cell(row=r, column=ci, value=v)
                    if tol and st.n > 0:
                        _quality_cell(ws, r, len(row_vals) + 1, st.p95, tol)
                    r += 1
                r += 1

            # ── Track-to-Track summary ──────────────────────────────
            if self._track_result:
                tkpi = self._track_result.left if side_key == "L" else self._track_result.right

                _section_title(ws, r, "Track-to-Track KPI — Summary"); r += 1
                t_hdrs = [
                    "Orig Tracks", "Orig Matched", "Orig Unmatched", "TP Events [%]",
                    "Resim Tracks", "Spurious", "Spurious Rate [%]",
                    "Track Ratio", "Mean IoU",
                    "Avg Orig Cov [%]", "Avg Resim Cov [%]",
                    "RESIM Score", "TP Events Score", "FP Events Score", "Accuracy Score",
                ]
                _hdr_row(ws, r, t_hdrs); r += 1
                orig_avail = round(100.0 * tkpi.n_matched / max(tkpi.n_orig, 1), 1)
                ghost_ct   = sum(1 for m in tkpi.matches
                                 if m.orig_track is None and not m.is_short
                                 and getattr(m, "is_ghost", True))
                ghost_rate = round(100.0 - tkpi.fp_score, 1)
                row_data = [
                    tkpi.n_orig, tkpi.n_matched, tkpi.n_orig - tkpi.n_matched,
                    orig_avail,
                    tkpi.n_resim, ghost_ct, ghost_rate,
                    round(tkpi.track_count_ratio, 2), round(tkpi.mean_iou, 3),
                    round(tkpi.avg_coverage_pct, 1),
                    round(tkpi.avg_resim_coverage_pct, 1),
                    round(tkpi.resim_score, 1), round(tkpi.tp_score, 1),
                    round(tkpi.fp_score, 1), round(tkpi.accuracy_score, 1),
                ]
                for ci, v in enumerate(row_data, start=1):
                    ws.cell(row=r, column=ci, value=v).alignment = _ctr
                r += 2

                # Track signal accuracy
                _section_title(ws, r, "Track-to-Track KPI — Aggregated Signal Accuracy"); r += 1
                sig_hdrs = ["Signal", "Unit", "N", "Mean Δ", "RMSE",
                            "Median", "P95 |Δ|", "Tolerance", "Quality"]
                _hdr_row(ws, r, sig_hdrs); r += 1
                for sig, _ in _KPI_SIGNALS:
                    if sig not in tkpi.signal_stats:
                        continue
                    st  = tkpi.signal_stats[sig]
                    tol = _DISPLAY_TOLERANCES.get(sig)
                    row_vals = [
                        sig, st.unit, st.n,
                        round(st.mean, 4), round(st.rmse, 4),
                        round(st.median, 4), round(st.p95, 4),
                        f"±{tol}" if tol else "",
                    ]
                    for ci, v in enumerate(row_vals, start=1):
                        ws.cell(row=r, column=ci, value=v)
                    if tol and st.n > 0:
                        _quality_cell(ws, r, len(row_vals) + 1, st.p95, tol)
                    r += 1
                r += 1

                # Per-track details
                _section_title(ws, r, "Track-to-Track KPI — Per-Track Details"); r += 1
                pt_hdrs = [
                    "Source", "Orig TID", "Resim TID",
                    "Scan Start", "Scan End", "Duration [s]", "ROI",
                    "Orig Cov [%]", "Resim Cov [%]", "Mean IoU",
                    "ΔLon [m]", "ΔLat [m]", "ΔLonVel [m/s]",
                    "ΔLatVel [m/s]", "ΔHeading [°]",
                ]
                _hdr_row(ws, r, pt_hdrs); r += 1
                _src_fills = {
                    "Resim only":  PatternFill("solid", fgColor="D28C28"),
                    "Outcompeted": PatternFill("solid", fgColor="8299D2"),
                    "Orig only":   PatternFill("solid", fgColor="DC5050"),
                    "Short event": PatternFill("solid", fgColor="787888"),
                }
                _src_font = Font(bold=True, color="FFFFFF")
                for m in tkpi.matches:
                    ot  = m.orig_track
                    rt  = m.resim_track
                    ref = ot if ot is not None else rt
                    src = ("Short event"  if m.is_short
                           else "Matched"      if (ot and rt)
                           else "Orig only"   if ot
                           else "Outcompeted" if not m.is_ghost
                           else "Resim only")
                    def _ms(sig: str) -> Optional[float]:
                        if ot and rt and sig in m.signal_stats:
                            return round(m.signal_stats[sig].mean, 4)
                        return None
                    row_vals = [
                        src,
                        ot.tracking_id if ot else None,
                        rt.tracking_id if rt else None,
                        ref.start_scan, ref.end_scan,
                        round(ref.duration, 3),
                        _roi_label(ref),
                        round(m.coverage_pct,       1) if (ot and rt) else None,
                        round(m.resim_coverage_pct, 1) if (ot and rt) else None,
                        round(m.mean_iou,           3) if (ot and rt) else None,
                        _ms("LonPosition"), _ms("LatPosition"),
                        _ms("LonGndVel"),   _ms("LatGndVel"),
                        _ms("HeadingAngle"),
                    ]
                    for ci, v in enumerate(row_vals, start=1):
                        cell = ws.cell(row=r, column=ci, value=v)
                        cell.alignment = _ctr
                    if src in _src_fills:
                        src_cell = ws.cell(row=r, column=1)
                        src_cell.fill = _src_fills[src]
                        src_cell.font = _src_font
                    r += 1

            # Auto-size columns (estimate)
            for col in ws.columns:
                max_len = max((len(str(cell.value)) for cell in col
                               if cell.value is not None), default=8)
                ws.column_dimensions[col[0].column_letter].width = min(max_len + 4, 30)

        wb.save(path)

    # ------------------------------------------------------------------
    # JSON export
    # ------------------------------------------------------------------

    def _on_save_json(self) -> None:
        if self._frame_result is None:
            return
        path, _ = QFileDialog.getSaveFileName(
            self, "Export KPI Results to JSON", "resim_kpi.json",
            "JSON files (*.json);;All files (*)"
        )
        if not path:
            return
        try:
            data = self._build_json()
            with open(path, "w", encoding="utf-8") as fh:
                json.dump(data, fh, indent=2, ensure_ascii=False)
            self._status_lbl.setText(f"Saved: {path}")
        except Exception as exc:
            self._status_lbl.setText(f"Export error: {exc}")

    def _build_json(self) -> dict:
        """Build a fully-structured dict suitable for json.dump."""

        def _stats_dict(st: "DeltaStats", tol: Optional[float]) -> dict:
            q = None
            if tol is not None and st.n > 0:
                q = ("PASS" if st.p95 <= tol
                     else "WARN" if st.p95 <= 4.0 * tol
                     else "FAIL")
            return {
                "n":         st.n,
                "mean":      round(st.mean,    4),
                "rmse":      round(st.rmse,    4),
                "std":       round(st.std,     4),
                "median":    round(st.median,  4),
                "p95_abs":   round(st.p95,     4),
                "max_abs":   round(st.max_abs, 4),
                "unit":      st.unit,
                "tolerance": tol,
                "quality":   q,
            }

        def _side_frame(kpi: "SideFrameKpi") -> dict:
            return {
                "total_orig":       kpi.total,
                "matched":          kpi.matched,
                "total_resim":      kpi.total_resim,
                "ghost":            kpi.ghost,
                "availability_pct": round(kpi.availability_pct, 2),
                "ghost_pct":        round(kpi.ghost_pct,         2),
                "mean_iou":         round(kpi.mean_iou,          4),
                "tp_score":    round(kpi.tp_score,    2),
                "fp_score": round(kpi.fp_score, 2),
                "accuracy_score":   round(kpi.accuracy_score,    2),
                "resim_score":      round(kpi.resim_score,       2),
                "signal_stats": {
                    sig: _stats_dict(kpi.signal_stats[sig],
                                     _DISPLAY_TOLERANCES.get(sig))
                    for sig in kpi.signal_stats
                },
            }

        def _side_track(kpi: "SideTrackKpi") -> dict:
            tracks = []
            for m in kpi.matches:
                ot  = m.orig_track
                rt  = m.resim_track
                ref = ot if ot is not None else rt
                src = ("Short event"  if m.is_short
                           else "Matched"      if (ot and rt)
                           else "Orig only"   if ot
                           else "Outcompeted" if not m.is_ghost
                           else "Resim only")
                entry: dict = {
                    "source":     src,
                    "orig_tid":   ot.tracking_id  if ot else None,
                    "resim_tid":  rt.tracking_id  if rt else None,
                    "scan_start": ref.start_scan,
                    "scan_end":   ref.end_scan,
                    "duration_s": round(ref.duration, 3),
                    "roi":        _roi_label(ref),
                }
                if ot and rt:
                    entry["orig_coverage_pct"]  = round(m.coverage_pct,       2)
                    entry["resim_coverage_pct"] = round(m.resim_coverage_pct, 2)
                    entry["mean_iou"]           = round(m.mean_iou,           4)
                    entry["signal_stats"] = {
                        sig: _stats_dict(m.signal_stats[sig],
                                         _DISPLAY_TOLERANCES.get(sig))
                        for sig in m.signal_stats
                    }
                tracks.append(entry)

            return {
                "n_orig":                 kpi.n_orig,
                "n_matched":              kpi.n_matched,
                "n_resim":                kpi.n_resim,
                "ghost_tracks":           sum(1 for m in kpi.matches
                                             if m.orig_track is None
                                             and not m.is_short
                                             and getattr(m, "is_ghost", True)),
                "avg_orig_coverage_pct":  round(kpi.avg_coverage_pct,       2),
                "avg_resim_coverage_pct": round(kpi.avg_resim_coverage_pct, 2),
                "track_count_ratio":      round(kpi.track_count_ratio,      4),
                "mean_iou":               round(kpi.mean_iou,               4),
                "tp_score":           round(kpi.tp_score,    2),
                "fp_score":            round(kpi.fp_score, 2),
                "accuracy_score":         round(kpi.accuracy_score,         2),
                "resim_score":            round(kpi.resim_score,            2),
                "signal_stats": {
                    sig: _stats_dict(kpi.signal_stats[sig],
                                     _DISPLAY_TOLERANCES.get(sig))
                    for sig in kpi.signal_stats
                },
                "tracks": tracks,
            }

        out: dict = {}

        if self._frame_result:
            fr = self._frame_result
            out["frame_to_frame"] = {
                "left":  _side_frame(fr.left),
                "right": _side_frame(fr.right),
            }

        if self._track_result:
            tr = self._track_result
            out["track_to_track"] = {
                "left":  _side_track(tr.left),
                "right": _side_track(tr.right),
            }

        return out
