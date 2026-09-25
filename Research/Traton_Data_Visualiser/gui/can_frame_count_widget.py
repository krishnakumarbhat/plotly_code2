"""
can_frame_count_widget.py
=========================
CAN Frame Count dialog.

Shows a table with every message defined in the DBC and the actual number
of decoded frames present in the loaded model(s).

Columns:
    Signal name | Channel | Message ID | Orig count | [Resim count]

"Resim count" column is added only when a resim model is also loaded.
Rows are colour-coded:
  - green  : count > 0 (expected)
  - red    : count == 0 (missing)
"""

from __future__ import annotations

import sys
from pathlib import Path
from typing import Dict, Optional

import numpy as np
from asammdf import MDF

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

# Ensure src/ is importable.
_GUI = Path(__file__).resolve().parent
_SRC = _GUI.parent / "src"
for _p in (_GUI, _SRC):
    if str(_p) not in sys.path:
        sys.path.insert(0, str(_p))

from data_model import DataModel
from vcan_reader import load_dbc

# ---------------------------------------------------------------------------
# Colours
# ---------------------------------------------------------------------------
_GREEN  = QColor(40, 160, 60)
_RED    = QColor(200, 60, 60)
_RED_BG = QColor(80, 0, 0)


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _bus_channel_for_msg(
    mf4_path: Path,
    mf4_format: str,
    msg_frame_id: int,
    is_extended: bool,
) -> str:
    """
    Return the bus channel (as a string) on which this message was seen in the
    raw MF4 file, or "-" if the file cannot be inspected or the message was
    not found.

    Only inspects MF4Frame-based files (aptiv_orcas / scania_orcas).
    For man_autera the channel is embedded in the CAN_DataFrame group name
    and is not easily extracted here; returns "-".
    """
    if mf4_format not in ("aptiv_orcas", "scania_orcas"):
        return "-"
    key = int(np.uint32((msg_frame_id | 0x80000000) if is_extended else msg_frame_id))
    try:
        mdf = MDF(str(mf4_path))
        proto = mdf.get("MF4Frame.ProtocolType").samples
        bus   = mdf.get("MF4Frame.BusChannel").samples
        ids   = mdf.get("MF4Frame.ID").samples.astype(np.int64)
        can_mask = (proto == 4) & (ids == key)
        buses = np.unique(bus[can_mask]).tolist()
        mdf.close()
        if not buses:
            return "-"
        return ", ".join(str(int(b)) for b in sorted(buses))
    except Exception:
        return "-"


def _build_channel_map(model: DataModel) -> Dict[int, str]:
    """
    Build a {raw_id: channel_string} dict for the given model by reading the
    MF4 file once.  Returns empty dict on failure or unsupported format.
    """
    if model.mf4_path is None or model.mf4_format not in ("aptiv_orcas", "scania_orcas"):
        return {}
    try:
        mdf = MDF(str(model.mf4_path))
        proto = mdf.get("MF4Frame.ProtocolType").samples
        bus   = mdf.get("MF4Frame.BusChannel").samples
        ids   = mdf.get("MF4Frame.ID").samples.astype(np.int64)
        mdf.close()
        can_mask = (proto == 4)
        can_ids  = ids[can_mask]
        can_bus  = bus[can_mask]
        result: Dict[int, str] = {}
        for uid in np.unique(can_ids):
            buses = np.unique(can_bus[can_ids == uid]).tolist()
            result[int(uid)] = ", ".join(str(int(b)) for b in sorted(buses))
        return result
    except Exception:
        return {}


def _count_cell(count: int) -> QTableWidgetItem:
    it = QTableWidgetItem(str(count))
    it.setTextAlignment(Qt.AlignmentFlag.AlignCenter)
    it.setData(Qt.ItemDataRole.UserRole, count)
    if count == 0:
        it.setForeground(_RED)
        it.setBackground(_RED_BG)
    else:
        it.setForeground(_GREEN)
    return it


def _text_cell(text: str, align=Qt.AlignmentFlag.AlignCenter) -> QTableWidgetItem:
    it = QTableWidgetItem(text)
    it.setTextAlignment(align)
    return it


# ---------------------------------------------------------------------------
# Dialog
# ---------------------------------------------------------------------------

class CanFrameCountDialog(QDialog):
    """Modal dialog showing per-message raw frame counts."""

    def __init__(self,
                 orig_model: DataModel,
                 dbc_path: str | Path,
                 prefixes: tuple[str, ...] = ("SRRL_", "SRRR_", "SRR2_"),
                 resim_model: Optional[DataModel] = None,
                 parent=None):
        super().__init__(parent)
        self.setWindowTitle("CAN Frame Count")
        self.resize(820, 600)
        self.setModal(True)

        self._orig     = orig_model
        self._resim    = resim_model
        self._dbc_path = Path(dbc_path)
        self._prefixes = prefixes
        self._build_ui()

    # ------------------------------------------------------------------
    def _build_ui(self) -> None:
        lay = QVBoxLayout(self)
        lay.setSpacing(8)

        has_resim = self._resim is not None

        # ── Table ──────────────────────────────────────────────────────
        col_headers = ["Signal name", "Channel", "Message ID", "Orig count"]
        if has_resim:
            col_headers.append("Resim count")

        tbl = QTableWidget()
        tbl.setColumnCount(len(col_headers))
        tbl.setHorizontalHeaderLabels(col_headers)
        tbl.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        tbl.setSelectionBehavior(QTableWidget.SelectionBehavior.SelectRows)
        tbl.setSortingEnabled(True)
        hdr = tbl.horizontalHeader()
        hdr.setSectionResizeMode(0, QHeaderView.ResizeMode.Stretch)
        for c in range(1, len(col_headers)):
            hdr.setSectionResizeMode(c, QHeaderView.ResizeMode.ResizeToContents)

        # ── Data ───────────────────────────────────────────────────────
        db        = load_dbc(self._dbc_path, self._prefixes)
        ch_map    = _build_channel_map(self._orig)
        ch_map_r  = _build_channel_map(self._resim) if has_resim else {}

        messages  = sorted(db.messages, key=lambda m: m.name)
        tbl.setRowCount(len(messages))

        orig_total  = 0
        resim_total = 0

        for row, msg in enumerate(messages):
            raw_key = int(
                np.uint32((msg.frame_id | 0x80000000) if msg.is_extended_frame
                          else msg.frame_id)
            )

            # Signal name
            tbl.setItem(row, 0, _text_cell(msg.name, Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter))

            # Channel (from orig file)
            channel = ch_map.get(raw_key, "-")
            tbl.setItem(row, 1, _text_cell(channel))

            # Message ID
            id_str = f"0x{msg.frame_id:08X}" if msg.is_extended_frame else f"0x{msg.frame_id:04X}"
            tbl.setItem(row, 2, _text_cell(id_str))

            # Orig count
            orig_count = len(self._orig.frames[msg.name]) if msg.name in self._orig.frames else 0
            orig_total += orig_count
            tbl.setItem(row, 3, _count_cell(orig_count))

            # Resim count
            if has_resim:
                resim_count = len(self._resim.frames[msg.name]) if msg.name in self._resim.frames else 0
                resim_total += resim_count
                tbl.setItem(row, 4, _count_cell(resim_count))

        lay.addWidget(tbl)

        # ── Summary bar ────────────────────────────────────────────────
        summary_parts = [f"Messages in DBC: {len(messages)}"]
        orig_missing = sum(
            1 for msg in messages
            if (len(self._orig.frames[msg.name]) if msg.name in self._orig.frames else 0) == 0
        )
        summary_parts.append(f"Orig missing: {orig_missing}")
        if has_resim:
            resim_missing = sum(
                1 for msg in messages
                if (len(self._resim.frames[msg.name]) if msg.name in self._resim.frames else 0) == 0
            )
            summary_parts.append(f"Resim missing: {resim_missing}")

        summary_lbl = QLabel("  |  ".join(summary_parts))
        summary_lbl.setAlignment(Qt.AlignmentFlag.AlignLeft)
        lay.addWidget(summary_lbl)

        # ── Close button ───────────────────────────────────────────────
        btn = QDialogButtonBox(QDialogButtonBox.StandardButton.Close)
        btn.rejected.connect(self.reject)
        lay.addWidget(btn)
