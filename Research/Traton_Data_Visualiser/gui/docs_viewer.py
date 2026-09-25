"""
docs_viewer.py
==============
In-app documentation browser dialog.

Opens a resizable QDialog with:
  - Left sidebar: clickable list of documentation topics
  - Right pane:   QTextBrowser rendering Markdown source

The Markdown files live in  <project_root>/docs/  and are resolved
relative to this file at runtime, so the dialog works both in the
development tree and when bundled with PyInstaller.
"""

from __future__ import annotations

import sys
from pathlib import Path
from typing import List, Tuple

from PyQt6.QtCore import Qt, QSize
from PyQt6.QtGui import QFont, QKeySequence, QShortcut
from PyQt6.QtWidgets import (
    QDialog,
    QHBoxLayout,
    QListWidget,
    QListWidgetItem,
    QSplitter,
    QTextBrowser,
    QVBoxLayout,
    QWidget,
)


# ---------------------------------------------------------------------------
# Docs catalogue
# filename (relative to docs/) and display title
# ---------------------------------------------------------------------------

_DOCS: List[Tuple[str, str]] = [
    ("00_how_to_use.md",          "How to Use"),
    ("01_overview.md",            "Application Overview"),
    ("02_data_model.md",          "Data Model & MF4 Loading"),
    ("03_object_tracks.md",       "Object Track Creation"),
    ("04a_resim_kpi_overview.md", "Resim Comparison \u2014 High Level"),
    ("04_resim_kpi.md",           "Resim Comparison \u2014 Detailed"),
]

# Resolve docs directory: works both in the dev tree and when frozen by
# PyInstaller (sys._MEIPASS points to the _internal/ extraction folder).
_DOCS_DIR = Path(getattr(sys, "_MEIPASS", Path(__file__).resolve().parent.parent)) / "docs"


# ---------------------------------------------------------------------------
# Stylesheets
# ---------------------------------------------------------------------------

_DARK_CSS = """
QDialog, QWidget {
    background: #12122a;
    color: #ccccff;
}
QListWidget {
    background: #1a1a3a;
    color: #aaaadd;
    border: 1px solid #333366;
    font-size: 11px;
}
QListWidget::item:selected {
    background: #2a2a6e;
    color: #ffffff;
}
QListWidget::item:hover {
    background: #22224e;
}
QTextBrowser {
    background: #0e0e20;
    color: #ddddff;
    border: 1px solid #333366;
    font-size: 12px;
    font-family: Segoe UI, Arial, sans-serif;
    padding: 12px;
    selection-background-color: #3a3a7e;
}
QScrollBar:vertical {
    background: #1a1a3a;
    width: 10px;
}
QScrollBar::handle:vertical {
    background: #444488;
    border-radius: 4px;
}
"""

_LIGHT_CSS = """
QDialog, QWidget {
    background: #f5f5f5;
    color: #111111;
}
QListWidget {
    background: #ffffff;
    color: #333333;
    border: 1px solid #aaaaaa;
    font-size: 11px;
}
QListWidget::item:selected {
    background: #d0d8ff;
    color: #000000;
}
QListWidget::item:hover {
    background: #e8ecff;
}
QTextBrowser {
    background: #ffffff;
    color: #111111;
    border: 1px solid #aaaaaa;
    font-size: 12px;
    font-family: Segoe UI, Arial, sans-serif;
    padding: 12px;
    selection-background-color: #c0c8ff;
}
"""


# ---------------------------------------------------------------------------
# Widget
# ---------------------------------------------------------------------------

class DocsViewerDialog(QDialog):
    """
    Modeless documentation browser.

    Parameters
    ----------
    theme : str
        "dark" or "light" — matches the application theme.
    start_page : int
        0-based index into _DOCS to open initially.
    parent : QWidget | None
    """

    def __init__(self, theme: str = "dark", start_page: int = 0,
                 parent=None):
        super().__init__(parent)
        self.setWindowTitle("SRR6T Visualiser — Documentation")
        self.resize(980, 700)
        self.setMinimumSize(QSize(600, 400))
        # Keep window on top of the main window but don't block it
        self.setWindowModality(Qt.WindowModality.NonModal)

        self._build_ui()
        self.set_theme(theme)

        # Load initial page
        idx = max(0, min(start_page, len(_DOCS) - 1))
        self._list.setCurrentRow(idx)
        self._load_page(idx)

        # Esc closes
        close_shortcut = QShortcut(QKeySequence("Escape"), self)
        close_shortcut.activated.connect(self.close)

    # ------------------------------------------------------------------
    # Public API
    # ------------------------------------------------------------------

    def set_theme(self, theme: str) -> None:
        self.setStyleSheet(_DARK_CSS if theme == "dark" else _LIGHT_CSS)
        # Adjust heading colours inside the QTextBrowser via document stylesheet
        if theme == "dark":
            self._browser.document().setDefaultStyleSheet(
                "h1 { color: #8888ff; border-bottom: 1px solid #444488; "
                "     padding-bottom: 4px; }"
                "h2 { color: #7777dd; }"
                "h3 { color: #6666cc; }"
                "code { background: #1e1e3e; color: #aaffaa; "
                "       font-family: Consolas, monospace; "
                "       padding: 1px 4px; border-radius: 3px; }"
                "pre  { background: #1e1e3e; color: #aaffaa; "
                "       font-family: Consolas, monospace; "
                "       padding: 8px; border-radius: 4px; }"
                "table { border-collapse: collapse; width: 100%; }"
                "th    { background: #2a2a5e; color: #aaaacc; "
                "        padding: 4px 8px; }"
                "td    { padding: 4px 8px; border-bottom: 1px solid #333366; }"
                "tr:nth-child(even) td { background: #16162e; }"
                "a  { color: #88aaff; }"
            )
        else:
            self._browser.document().setDefaultStyleSheet(
                "h1 { color: #2233aa; border-bottom: 1px solid #aabbdd; "
                "     padding-bottom: 4px; }"
                "h2 { color: #3344bb; }"
                "h3 { color: #4455cc; }"
                "code { background: #eef0ff; color: #224422; "
                "       font-family: Consolas, monospace; "
                "       padding: 1px 4px; border-radius: 3px; }"
                "pre  { background: #eef0ff; color: #224422; "
                "       font-family: Consolas, monospace; "
                "       padding: 8px; border-radius: 4px; }"
                "table { border-collapse: collapse; width: 100%; }"
                "th    { background: #dde8ff; color: #222244; "
                "        padding: 4px 8px; }"
                "td    { padding: 4px 8px; border-bottom: 1px solid #cccccc; }"
                "tr:nth-child(even) td { background: #f5f5ff; }"
                "a  { color: #2244cc; }"
            )
        # Re-render current page so styles apply
        row = self._list.currentRow()
        if row >= 0:
            self._load_page(row)

    # ------------------------------------------------------------------
    # UI construction
    # ------------------------------------------------------------------

    def _build_ui(self) -> None:
        root = QVBoxLayout(self)
        root.setContentsMargins(6, 6, 6, 6)
        root.setSpacing(0)

        splitter = QSplitter(Qt.Orientation.Horizontal)
        root.addWidget(splitter)

        # Left: topic list
        left = QWidget()
        left_lay = QVBoxLayout(left)
        left_lay.setContentsMargins(0, 0, 4, 0)
        left_lay.setSpacing(0)

        self._list = QListWidget()
        self._list.setFixedWidth(200)
        font = QFont()
        font.setPointSize(10)
        self._list.setFont(font)

        for _, title in _DOCS:
            item = QListWidgetItem(title)
            item.setTextAlignment(Qt.AlignmentFlag.AlignLeft |
                                  Qt.AlignmentFlag.AlignVCenter)
            self._list.addItem(item)

        self._list.currentRowChanged.connect(self._load_page)
        left_lay.addWidget(self._list)
        splitter.addWidget(left)

        # Right: markdown browser
        self._browser = QTextBrowser()
        self._browser.setOpenExternalLinks(True)
        self._browser.setReadOnly(True)
        splitter.addWidget(self._browser)

        splitter.setStretchFactor(0, 0)
        splitter.setStretchFactor(1, 1)
        splitter.setSizes([200, 760])

    # ------------------------------------------------------------------
    # Page loading
    # ------------------------------------------------------------------

    def _load_page(self, index: int) -> None:
        if index < 0 or index >= len(_DOCS):
            return
        filename, title = _DOCS[index]
        path = _DOCS_DIR / filename
        try:
            text = path.read_text(encoding="utf-8")
        except FileNotFoundError:
            text = f"# {title}\n\n*Documentation file not found:*\n\n`{path}`"
        except Exception as exc:
            text = f"# {title}\n\n*Error reading file:* {exc}"

        # QTextBrowser inherits QTextEdit which has setMarkdown() in Qt 6
        self._browser.setMarkdown(text)
        self._browser.verticalScrollBar().setValue(0)
