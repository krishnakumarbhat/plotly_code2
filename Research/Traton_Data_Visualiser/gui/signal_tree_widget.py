"""
signal_tree_widget.py
=====================
QTreeWidget that displays all decoded messages and their signals in a
two-level hierarchy:

  Messages_XX_SRR2
    ├── signal_name_1
    ├── signal_name_2
    └── ...

Emits ``signal_selected(msg_name: str, signal_name: str)`` when the user
clicks on a leaf signal node.
"""

from __future__ import annotations

from typing import Dict, Optional

import pandas as pd
from PyQt6.QtCore import pyqtSignal, Qt
from PyQt6.QtWidgets import (
    QAbstractItemView,
    QTreeWidget,
    QTreeWidgetItem,
)
from PyQt6.QtGui import QFont


class SignalTreeWidget(QTreeWidget):
    """Two-level tree: message → signal."""

    signal_selected = pyqtSignal(str, str)   # (msg_name, signal_name) — single click

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setHeaderLabel("Messages / Signals")
        self.setColumnCount(1)
        self.setSelectionMode(QAbstractItemView.SelectionMode.ExtendedSelection)
        self.itemClicked.connect(self._on_item_clicked)

        # Styling
        self._apply_dark_stylesheet()

    _STYLESHEET_DARK = """
            QTreeWidget {
                background-color: #1a1a2e;
                color: #ccccff;
                border: 1px solid #444466;
                font-size: 11px;
            }
            QTreeWidget::item:selected {
                background-color: #2a2a6e;
                color: white;
            }
            QTreeWidget::item:hover {
                background-color: #22224e;
            }
            QHeaderView::section {
                background-color: #12122a;
                color: #8888cc;
                border: none;
                padding: 4px;
                font-weight: bold;
            }
        """
    _STYLESHEET_LIGHT = """
            QTreeWidget {
                background-color: #ffffff;
                color: #111111;
                border: 1px solid #aaaaaa;
                font-size: 11px;
            }
            QTreeWidget::item:selected {
                background-color: #c8c8e8;
                color: #000000;
            }
            QTreeWidget::item:hover {
                background-color: #e8e8f8;
            }
            QHeaderView::section {
                background-color: #eeeeee;
                color: #333333;
                border: none;
                padding: 4px;
                font-weight: bold;
            }
        """

    def _apply_dark_stylesheet(self):
        self.setStyleSheet(self._STYLESHEET_DARK)

    def set_theme(self, theme: str) -> None:
        if theme == "light":
            self.setStyleSheet(self._STYLESHEET_LIGHT)
        else:
            self.setStyleSheet(self._STYLESHEET_DARK)

    # ------------------------------------------------------------------
    # Population
    # ------------------------------------------------------------------

    def populate(self, frames: Dict[str, pd.DataFrame],
                 group_label: Optional[str] = None) -> None:
        """Fill the tree from a decoded frames dict.

        If *group_label* is given (e.g. "Original" / "Resim") the messages are
        nested under a top-level group node so multiple calls can coexist.
        Pass no label (default) to clear and rebuild in single-source mode.
        """
        if group_label is None:
            self.clear()

        bold = QFont()
        bold.setBold(True)

        italic_bold = QFont()
        italic_bold.setBold(True)
        italic_bold.setItalic(True)

        if group_label is not None:
            group_item = QTreeWidgetItem(self, [f"▶  {group_label}"])
            group_item.setFont(0, italic_bold)
            group_item.setData(0, 32, ("__group__", group_label))
            group_item.setExpanded(True)
            parent_of_msgs = group_item
        else:
            parent_of_msgs = self   # type: ignore[assignment]

        for msg_name in sorted(frames.keys()):
            df = frames[msg_name]
            msg_item = QTreeWidgetItem(parent_of_msgs, [msg_name])
            msg_item.setFont(0, bold)
            msg_item.setData(0, 32, ("__msg__", msg_name))

            for col in sorted(df.columns):
                child = QTreeWidgetItem(msg_item, [col])
                child.setData(0, 32, (msg_name, col))

        if group_label is None:
            self.sortItems(0, Qt.SortOrder.AscendingOrder)
        self.resizeColumnToContents(0)

    # ------------------------------------------------------------------
    # Interaction
    # ------------------------------------------------------------------

    def get_selected_signals(self) -> list:
        """
        Return list of (msg_name, signal_name) tuples for all currently
        selected leaf (signal) items.
        """
        result = []
        for item in self.selectedItems():
            data = item.data(0, 32)
            if data is None:
                continue
            msg_name, sig_name = data
            if msg_name == "__msg__":
                continue
            result.append((msg_name, sig_name))
        return result

    def _on_item_clicked(self, item: QTreeWidgetItem, _column: int) -> None:
        data = item.data(0, 32)
        if data is None:
            return
        msg_name, sig_name = data
        if msg_name in ("__msg__", "__group__"):
            return   # clicked on a group/message header, not a signal
        self.signal_selected.emit(msg_name, sig_name)
