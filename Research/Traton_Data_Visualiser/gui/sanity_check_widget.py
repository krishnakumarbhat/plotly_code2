"""
sanity_check_widget.py
======================
Data Sanity Check dialog.

Shows a table of every expected scan index in the loaded range and marks
each one as present (True) or missing (False/red) for both radar sides
(L and R).  When a resim model is also loaded, four additional columns
are shown for the resim data.

Columns (orig only):
    Scan Index L | Presence | Scan Index R | Presence

Columns (orig + resim):
    Scan Index L | Presence | Scan Index R | Presence |
    Resim: Scan Index L | Presence | Resim: Scan Index R | Presence
"""

from __future__ import annotations

from typing import Optional, Set

from PyQt6.QtCore import Qt
from PyQt6.QtGui import QColor, QFont
from PyQt6.QtWidgets import (
    QDialog,
    QDialogButtonBox,
    QHBoxLayout,
    QLabel,
    QSizePolicy,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QHeaderView,
)

from data_model import DataModel


# ---------------------------------------------------------------------------
# Colours
# ---------------------------------------------------------------------------
_GREEN = QColor(40, 160, 60)
_RED   = QColor(200, 60, 60)


def _scan_set_for_side(model: DataModel, side: str) -> Set[int]:
    """Return the set of scan indices actually present for the given side."""
    prefix = "SRRL" if side == "L" else "SRRR"
    key    = f"{prefix}_Header_SRR2"
    si_col = f"Scan_Index_{side}"
    if key not in model.frames:
        return set()
    df = model.frames[key]
    if si_col not in df.columns:
        return set()
    return set(df[si_col].dropna().astype(int).tolist())


def _presence_item(present: bool) -> QTableWidgetItem:
    it = QTableWidgetItem("True" if present else "False")
    it.setTextAlignment(Qt.AlignmentFlag.AlignCenter)
    it.setForeground(_GREEN if present else _RED)
    if not present:
        it.setBackground(QColor(80, 0, 0))
    return it


def _index_item(value: int, present: bool) -> QTableWidgetItem:
    it = QTableWidgetItem(str(value))
    it.setTextAlignment(Qt.AlignmentFlag.AlignCenter)
    it.setData(Qt.ItemDataRole.UserRole, value)   # for numeric sort
    if not present:
        it.setForeground(_RED)
        it.setBackground(QColor(80, 0, 0))
    return it


class SanityCheckDialog(QDialog):
    """Modal dialog showing per-scan-index presence for both sides."""

    def __init__(self,
                 orig_model: DataModel,
                 resim_model: Optional[DataModel] = None,
                 parent=None):
        super().__init__(parent)
        self.setWindowTitle("Frame Table — Scan Index Presence")
        self.resize(900, 650)
        self.setModal(True)

        self._orig  = orig_model
        self._resim = resim_model
        self._build_ui()

    # ------------------------------------------------------------------
    # UI
    # ------------------------------------------------------------------
    def _build_ui(self) -> None:
        lay = QVBoxLayout(self)
        lay.setSpacing(8)

        # ── Info bar ──────────────────────────────────────────────────
        self._info_lbl = QLabel()
        self._info_lbl.setWordWrap(True)
        lay.addWidget(self._info_lbl)

        # ── Table ─────────────────────────────────────────────────────
        has_resim = self._resim is not None
        col_headers = [
            "Scan Index L", "Presence",
            "Scan Index R", "Presence",
        ]
        if has_resim:
            col_headers += [
                "Resim: Scan Index L", "Presence",
                "Resim: Scan Index R", "Presence",
            ]

        self._table = QTableWidget(0, len(col_headers))
        self._table.setHorizontalHeaderLabels(col_headers)
        hdr = self._table.horizontalHeader()
        for c in range(len(col_headers)):
            hdr.setSectionResizeMode(c, QHeaderView.ResizeMode.ResizeToContents)
        self._table.verticalHeader().setVisible(False)
        self._table.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        self._table.setAlternatingRowColors(True)
        self._table.setSortingEnabled(True)
        self._table.setSelectionBehavior(
            QTableWidget.SelectionBehavior.SelectRows)
        bold = QFont()
        bold.setBold(True)
        self._table.horizontalHeader().setFont(bold)
        lay.addWidget(self._table, stretch=1)

        # ── Summary row ───────────────────────────────────────────────
        self._summary_lbl = QLabel()
        self._summary_lbl.setSizePolicy(
            QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        lay.addWidget(self._summary_lbl)

        # ── Close button ──────────────────────────────────────────────
        btn = QDialogButtonBox(QDialogButtonBox.StandardButton.Close)
        btn.rejected.connect(self.reject)
        lay.addWidget(btn)

        self._populate()

    # ------------------------------------------------------------------
    # Data population
    # ------------------------------------------------------------------
    def _populate(self) -> None:
        has_resim = self._resim is not None

        # Collect per-side scan sets for original
        orig_l = _scan_set_for_side(self._orig, "L")
        orig_r = _scan_set_for_side(self._orig, "R")

        # Collect per-side scan sets for resim (if loaded)
        resim_l: Set[int] = set()
        resim_r: Set[int] = set()
        if has_resim:
            resim_l = _scan_set_for_side(self._resim, "L")
            resim_r = _scan_set_for_side(self._resim, "R")

        # Overall scan range: union of all present indices, then fill the
        # contiguous range from global min to global max.
        all_present = orig_l | orig_r | resim_l | resim_r
        if not all_present:
            self._info_lbl.setText("No scan index data found in loaded file(s).")
            return

        global_min = min(all_present)
        global_max = max(all_present)
        full_range = range(global_min, global_max + 1)

        self._info_lbl.setText(
            f"Scan range: <b>{global_min}</b> – <b>{global_max}</b> "
            f"({global_max - global_min + 1} expected indices)"
            + (f"  |  Orig L: {len(orig_l)} present"
               f"  |  Orig R: {len(orig_r)} present"
               + (f"  |  Resim L: {len(resim_l)} present"
                  f"  |  Resim R: {len(resim_r)} present"
                  if has_resim else ""))
        )

        self._table.setSortingEnabled(False)
        self._table.setRowCount(len(full_range))

        missing_orig_l = missing_orig_r = 0
        missing_resim_l = missing_resim_r = 0

        for row, idx in enumerate(full_range):
            pres_ol = idx in orig_l
            pres_or = idx in orig_r
            if not pres_ol: missing_orig_l += 1
            if not pres_or: missing_orig_r += 1

            self._table.setItem(row, 0, _index_item(idx, pres_ol))
            self._table.setItem(row, 1, _presence_item(pres_ol))
            self._table.setItem(row, 2, _index_item(idx, pres_or))
            self._table.setItem(row, 3, _presence_item(pres_or))

            if has_resim:
                pres_rl = idx in resim_l
                pres_rr = idx in resim_r
                if not pres_rl: missing_resim_l += 1
                if not pres_rr: missing_resim_r += 1
                self._table.setItem(row, 4, _index_item(idx, pres_rl))
                self._table.setItem(row, 5, _presence_item(pres_rl))
                self._table.setItem(row, 6, _index_item(idx, pres_rr))
                self._table.setItem(row, 7, _presence_item(pres_rr))

        self._table.setSortingEnabled(True)

        # Summary
        total = len(full_range)
        parts = [
            f"Orig L missing: <b style='color:#e05050'>{missing_orig_l}</b> / {total}",
            f"Orig R missing: <b style='color:#e05050'>{missing_orig_r}</b> / {total}",
        ]
        if has_resim:
            parts += [
                f"Resim L missing: <b style='color:#e05050'>{missing_resim_l}</b> / {total}",
                f"Resim R missing: <b style='color:#e05050'>{missing_resim_r}</b> / {total}",
            ]
        self._summary_lbl.setText("  |  ".join(parts))
