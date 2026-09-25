"""
main_window.py
==============
Main application window.  Layout:

  ┌─ Menu bar ──────────────────────────────────────────────────────┐
  ├─ Left panel ──────────┬─ Right panel (tabs) ────────────────────┤
  │  Signal tree          │  Tab 1 : Plan View                      │
  │                       │  Tab 2 : Signal Plot                    │
  │  [Plot Signal]        │                                         │
  ├───────────────────────┴─────────────────────────────────────────┤
  │  [|◄] [◄]  ══ slider ══  [►] [►|]    Side: [L/R/Both]           │
  │  Scan Index: XXXXX    Frame N / M    Timestamp: XX.XXX s        │
  └─────────────────────────────────────────────────────────────────┘
"""

from __future__ import annotations

import sys
import time
from pathlib import Path
from typing import List, Optional

from PyQt6.QtCore import Qt, QThread, QTimer, QSettings, pyqtSignal, QObject
from PyQt6.QtWidgets import (
    QApplication,
    QCheckBox,
    QComboBox,
    QDialog,
    QDialogButtonBox,
    QDoubleSpinBox,
    QFileDialog,
    QGroupBox,
    QHBoxLayout,
    QHeaderView,
    QLabel,
    QMainWindow,
    QMenu,
    QMessageBox,
    QProgressDialog,
    QPushButton,
    QSlider,
    QSplitter,
    QStatusBar,
    QScrollArea,
    QTabBar,
    QTabWidget,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
)
from PyQt6.QtGui import QAction, QKeySequence, QShortcut, QCursor, QColor
from PyQt6.QtWidgets import QColorDialog
from matplotlib.backends.backend_qtagg import NavigationToolbar2QT

# Ensure gui/ and src/ are on the path.
_GUI = Path(__file__).resolve().parent
_SRC = _GUI.parent / "src"
for _p in (_GUI, _SRC):
    if str(_p) not in sys.path:
        sys.path.insert(0, str(_p))

from data_model import DataModel, ObjectRecord
from plan_view_widget import PlanViewWidget
from signal_tree_widget import SignalTreeWidget
from signal_plot_widget import SignalPlotWidget
from object_track_widget import ObjectTrackWidget
from resim_comparison_widget import ResimComparisonWidget
from video_widget import VideoWidget
from docs_viewer import DocsViewerDialog
from sanity_check_widget import SanityCheckDialog
from can_frame_count_widget import CanFrameCountDialog
from view_settings import (
    SETTINGS, ViewSettings,
    _DYN_LABELS, CLASS_LABELS,
)


def _resource_path(relative: str) -> Path:
    """Return absolute path to a bundled resource (works both frozen and dev)."""
    base = Path(getattr(sys, "_MEIPASS", Path(__file__).resolve().parent.parent))
    return base / relative


_DBC_DEFAULT = _resource_path("dbc/VCAN_SRR6pT_V25.dbc")

APP_VERSION = "1.2.0"
_APP_TITLE   = f"TRATON SRR6+T VCAN VISUALISER  —  v{APP_VERSION}"


# ---------------------------------------------------------------------------
# Detachable tab widget
# ---------------------------------------------------------------------------

class _DetachableTabBar(QTabBar):
    """QTabBar that emits detach_tab(index) when:
      - dragged outside the QTabWidget boundary, OR
      - double-clicked, OR
      - "Detach to window" is chosen from the right-click context menu.
    """

    detach_tab = pyqtSignal(int)
    _THRESHOLD = 25   # px outside QTabWidget rect before drag-detach fires

    def __init__(self, parent=None):
        super().__init__(parent)
        self._pressed_tab: int = -1
        self.setToolTip(
            "Double-click or right-click a tab to detach it into a floating window."
        )

    def mousePressEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            self._pressed_tab = self.tabAt(event.pos())
        super().mousePressEvent(event)

    def mouseDoubleClickEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            idx = self.tabAt(event.pos())
            if idx >= 0:
                self._pressed_tab = -1
                self.detach_tab.emit(idx)
                return
        super().mouseDoubleClickEvent(event)

    def mouseMoveEvent(self, event):
        if self._pressed_tab >= 0 and (event.buttons() & Qt.MouseButton.LeftButton):
            tw = self.parent()  # the QTabWidget
            if tw is not None:
                local = tw.mapFromGlobal(event.globalPosition().toPoint())
                r = tw.rect()
                t = self._THRESHOLD
                if (local.x() < -t or local.x() > r.width()  + t
                        or local.y() < -t or local.y() > r.height() + t):
                    idx = self._pressed_tab
                    self._pressed_tab = -1
                    self.detach_tab.emit(idx)
                    return
        super().mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        self._pressed_tab = -1
        super().mouseReleaseEvent(event)

    def contextMenuEvent(self, event):
        idx = self.tabAt(event.pos())
        if idx < 0:
            return
        menu = QMenu(self)
        detach_act = menu.addAction("Detach to window")
        action = menu.exec(event.globalPos())
        if action is detach_act:
            self.detach_tab.emit(idx)


class _FloatingPanel(QDialog):
    """Floating window that holds a tab widget panel while it is detached."""

    reattach = pyqtSignal(str, QWidget)   # (title, widget)

    def __init__(self, widget: QWidget, title: str, orig_index: int,
                 pos, stylesheet: str, parent=None):
        super().__init__(parent, Qt.WindowType.Window)
        self.setWindowTitle(title)
        self.setStyleSheet(stylesheet)
        self.orig_index = orig_index
        self._title = title
        self._widget = widget
        lay = QVBoxLayout(self)
        lay.setContentsMargins(0, 0, 0, 0)
        lay.addWidget(widget)
        widget.show()          # removeTab() hides the widget; must re-show it
        self.resize(900, 700)
        self.move(pos)
        self.show()

    def closeEvent(self, event):
        self.layout().removeWidget(self._widget)
        self.reattach.emit(self._title, self._widget)
        event.accept()


class DetachableTabWidget(QTabWidget):
    """QTabWidget whose tabs can be detached into floating windows by dragging
    them outside the widget boundary.  Close the floating window to re-dock."""

    def __init__(self, parent=None):
        super().__init__(parent)
        self._bar = _DetachableTabBar(self)
        self.setTabBar(self._bar)
        self._bar.detach_tab.connect(self._on_detach)
        self._floating: dict[str, _FloatingPanel] = {}

    def _on_detach(self, index: int) -> None:
        if self.count() <= 1:
            return   # never leave the tab widget completely empty
        title  = self.tabText(index)
        widget = self.widget(index)
        if title in self._floating:
            return   # already floating
        self.removeTab(index)
        fp = _FloatingPanel(
            widget, title, index, QCursor.pos(),
            self.window().styleSheet(), self.window(),
        )
        fp.reattach.connect(self._on_reattach)
        self._floating[title] = fp

    def _on_reattach(self, title: str, widget: QWidget) -> None:
        fp  = self._floating.pop(title, None)
        orig = fp.orig_index if fp is not None else self.count()
        idx  = min(orig, self.count())
        self.insertTab(idx, widget, title)
        self.setCurrentIndex(idx)
        if fp is not None:
            fp.deleteLater()

    def show_panel(self, widget: QWidget) -> None:
        """Switch to *widget*'s tab, or raise its floating window if detached."""
        idx = self.indexOf(widget)
        if idx >= 0:
            self.setCurrentIndex(idx)
        else:
            for fp in self._floating.values():
                if fp._widget is widget:
                    fp.raise_()
                    fp.activateWindow()
                    break

    # ------------------------------------------------------------------
    # Layout persistence
    # ------------------------------------------------------------------

    def save_layout(self, settings: QSettings, key_prefix: str = "tabs") -> None:
        """Persist which tabs are floating and their window geometry."""
        floating_data = []
        for title, fp in self._floating.items():
            geom = fp.saveGeometry()
            floating_data.append((title, fp.orig_index, geom))
        settings.setValue(f"{key_prefix}/floating", floating_data)
        settings.setValue(f"{key_prefix}/current", self.currentIndex())

    def restore_layout(self, settings: QSettings, key_prefix: str = "tabs") -> None:
        """Re-detach tabs that were floating when the app was last closed."""
        floating_data = settings.value(f"{key_prefix}/floating", [])
        if not floating_data:
            return
        for entry in floating_data:
            try:
                title, orig_index, geom = entry
            except (TypeError, ValueError):
                continue
            # Find the tab with this title and detach it
            for i in range(self.count()):
                if self.tabText(i) == title:
                    if self.count() <= 1:
                        break   # never leave empty
                    widget = self.widget(i)
                    self.removeTab(i)
                    fp = _FloatingPanel(
                        widget, title, orig_index,
                        QCursor.pos(),
                        self.window().styleSheet(),
                        self.window(),
                    )
                    fp.reattach.connect(self._on_reattach)
                    if geom:
                        fp.restoreGeometry(geom)
                    self._floating[title] = fp
                    break
        saved_idx = settings.value(f"{key_prefix}/current", 0)
        try:
            self.setCurrentIndex(int(saved_idx))
        except (TypeError, ValueError):
            pass


# ---------------------------------------------------------------------------
# Settings dialogs
# ---------------------------------------------------------------------------

_DIALOG_STYLE = (
    "QDialog,QWidget { background:#12122a; color:#ccccff; }"
    "QLabel { color:#aaaacc; }"
    "QPushButton { background:#2a2a5e; color:#ccccff; border:1px solid #555588;"
    " border-radius:3px; padding:3px 10px; }"
    "QPushButton:hover { background:#3a3a7e; }"
    "QDoubleSpinBox,QComboBox { background:#2a2a5e; color:#ccccff;"
    " border:1px solid #555588; border-radius:3px; padding:2px 4px; }"
    "QGroupBox { color:#8888cc; border:1px solid #444466; margin-top:6px;"
    " padding:6px; }"
    "QGroupBox::title { subcontrol-origin:margin; left:6px; color:#8888cc; }"
    "QCheckBox { color:#ccccff; }"
    "QScrollArea { border:none; }"
)
_DIALOG_STYLE_LIGHT = (
    "QDialog,QWidget { background:#f5f5f5; color:#111111; }"
    "QLabel { color:#222222; }"
    "QPushButton { background:#e0e0e0; color:#111111; border:1px solid #aaaaaa;"
    " border-radius:3px; padding:3px 10px; }"
    "QPushButton:hover { background:#cccccc; }"
    "QDoubleSpinBox,QComboBox { background:#ffffff; color:#111111;"
    " border:1px solid #aaaaaa; border-radius:3px; padding:2px 4px; }"
    "QGroupBox { color:#444444; border:1px solid #aaaaaa; margin-top:6px;"
    " padding:6px; }"
    "QGroupBox::title { subcontrol-origin:margin; left:6px; color:#444444; }"
    "QCheckBox { color:#111111; }"
    "QScrollArea { border:none; }"
)


def _colour_button(colour_hex: str, parent=None) -> QPushButton:
    """Small square button showing a colour swatch."""
    btn = QPushButton(parent)
    btn.setFixedSize(28, 22)
    btn.setToolTip(colour_hex)
    _set_colour_button(btn, colour_hex)
    return btn


def _set_colour_button(btn: QPushButton, colour_hex: str) -> None:
    btn.setToolTip(colour_hex)
    btn.setStyleSheet(
        f"background:{colour_hex}; border:1px solid #888; border-radius:2px;"
    )


def _pick_colour(current: str, parent=None) -> str | None:
    """Open a QColorDialog; return new hex or None if cancelled."""
    from PyQt6.QtWidgets import QColorDialog
    c = QColorDialog.getColor(QColor(current), parent, "Pick colour")
    if c.isValid():
        return c.name()
    return None


def _alpha_spin(value: float) -> QDoubleSpinBox:
    s = QDoubleSpinBox()
    s.setRange(0.0, 1.0)
    s.setSingleStep(0.05)
    s.setDecimals(2)
    s.setValue(value)
    s.setFixedWidth(68)
    return s


class _StyleRow(QWidget):
    """One row: colour swatch button + alpha spinbox, bound to a mutable slot."""

    def __init__(self, label: str, colour: str, alpha: float, parent=None):
        super().__init__(parent)
        lay = QHBoxLayout(self)
        lay.setContentsMargins(0, 2, 0, 2)
        lay.setSpacing(6)
        self._colour = colour
        lbl = QLabel(label)
        lbl.setMinimumWidth(130)
        lay.addWidget(lbl)
        self._btn = _colour_button(colour, self)
        self._btn.clicked.connect(self._on_pick)
        lay.addWidget(self._btn)
        lay.addWidget(QLabel("alpha:"))
        self._alpha = _alpha_spin(alpha)
        lay.addWidget(self._alpha)
        lay.addStretch()

    def _on_pick(self):
        c = _pick_colour(self._colour, self)
        if c:
            self._colour = c
            _set_colour_button(self._btn, c)

    def get_colour(self) -> str:
        return self._colour

    def get_alpha(self) -> float:
        return self._alpha.value()

    def lw_spin(self) -> QDoubleSpinBox | None:
        return None


class _StyleRowWithLW(_StyleRow):
    """Style row with an extra linewidth spinbox."""

    def __init__(self, label: str, colour: str, lw: float, alpha: float, parent=None):
        super().__init__(label, colour, alpha, parent)
        self.layout().insertWidget(4, QLabel("lw:"))
        self._lw = QDoubleSpinBox()
        self._lw.setRange(0.5, 5.0)
        self._lw.setSingleStep(0.5)
        self._lw.setDecimals(1)
        self._lw.setValue(lw)
        self._lw.setFixedWidth(58)
        self.layout().insertWidget(5, self._lw)

    def get_lw(self) -> float:
        return self._lw.value()


class _PlanViewSettingsDialog(QDialog):
    """Modal dialog to edit all plan-view colour / style settings."""

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Plan View Settings")
        self.resize(560, 680)
        self.setStyleSheet(_DIALOG_STYLE)

        root = QVBoxLayout(self)

        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        inner = QWidget()
        lay = QVBoxLayout(inner)
        lay.setSpacing(10)
        scroll.setWidget(inner)
        root.addWidget(scroll, stretch=1)

        # ── DynProperty styles ────────────────────────────────────────
        grp_dyn = QGroupBox("Bounding Box – Dynamic Property colours")
        g_dyn = QVBoxLayout(grp_dyn)
        self._dyn_rows: dict[int, _StyleRowWithLW] = {}
        for key, label in _DYN_LABELS.items():
            s = SETTINGS.dyn_styles.get(key, SETTINGS.dyn_fallback)
            row = _StyleRowWithLW(label, s[0], s[1], s[2], grp_dyn)
            g_dyn.addWidget(row)
            self._dyn_rows[key] = row
        lay.addWidget(grp_dyn)

        # ── Class colour mode ─────────────────────────────────────────
        grp_cls = QGroupBox("Bounding Box – Class colour mode")
        g_cls = QVBoxLayout(grp_cls)
        self._class_mode_cb = QCheckBox("Enable class colour mode (overrides DynProperty colours)")
        self._class_mode_cb.setChecked(SETTINGS.class_color_mode)
        g_cls.addWidget(self._class_mode_cb)
        self._class_rows: dict[int, _StyleRowWithLW] = {}
        for key, label in CLASS_LABELS.items():
            s = SETTINGS.class_styles.get(key, SETTINGS.dyn_fallback)
            row = _StyleRowWithLW(label, s[0], s[1], s[2], grp_cls)
            g_cls.addWidget(row)
            self._class_rows[key] = row
        lay.addWidget(grp_cls)

        # ── Velocity vector ───────────────────────────────────────────
        grp_vel = QGroupBox("Velocity vector")
        g_vel = QVBoxLayout(grp_vel)
        self._vel_moving = _StyleRow(
            "Moving / Stopped",
            SETTINGS.vel_moving_colour, SETTINGS.vel_moving_alpha, grp_vel)
        self._vel_stat = _StyleRow(
            "Stationary",
            SETTINGS.vel_stationary_colour, SETTINGS.vel_stationary_alpha, grp_vel)
        g_vel.addWidget(self._vel_moving)
        g_vel.addWidget(self._vel_stat)
        lay.addWidget(grp_vel)

        # ── ID label ──────────────────────────────────────────────────
        grp_id = QGroupBox("ID label")
        g_id = QVBoxLayout(grp_id)
        self._id_row = _StyleRow(
            "ID text", SETTINGS.id_colour, SETTINGS.id_alpha, grp_id)
        g_id.addWidget(self._id_row)
        lay.addWidget(grp_id)

        # ── Buttons ───────────────────────────────────────────────────
        btn_row = QWidget()
        b_lay = QHBoxLayout(btn_row)
        b_lay.setContentsMargins(0, 0, 0, 0)
        reset_btn = QPushButton("Reset to defaults")
        reset_btn.clicked.connect(self._reset)
        b_lay.addWidget(reset_btn)
        b_lay.addStretch()
        bbox = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok |
            QDialogButtonBox.StandardButton.Cancel
        )
        bbox.accepted.connect(self.accept)
        bbox.rejected.connect(self.reject)
        b_lay.addWidget(bbox)
        root.addWidget(btn_row)

    def _reset(self):
        """Reset SETTINGS to defaults and re-populate all rows."""
        SETTINGS.reset()
        # Re-populate dyn rows
        for key, row in self._dyn_rows.items():
            s = SETTINGS.dyn_styles.get(key, SETTINGS.dyn_fallback)
            row._colour = s[0]; _set_colour_button(row._btn, s[0])
            row._lw.setValue(s[1]); row._alpha.setValue(s[2])
        # Re-populate class rows
        self._class_mode_cb.setChecked(False)
        for key, row in self._class_rows.items():
            s = SETTINGS.class_styles.get(key, SETTINGS.dyn_fallback)
            row._colour = s[0]; _set_colour_button(row._btn, s[0])
            row._lw.setValue(s[1]); row._alpha.setValue(s[2])
        # Velocity
        self._vel_moving._colour = SETTINGS.vel_moving_colour
        _set_colour_button(self._vel_moving._btn, SETTINGS.vel_moving_colour)
        self._vel_moving._alpha.setValue(SETTINGS.vel_moving_alpha)
        self._vel_stat._colour = SETTINGS.vel_stationary_colour
        _set_colour_button(self._vel_stat._btn, SETTINGS.vel_stationary_colour)
        self._vel_stat._alpha.setValue(SETTINGS.vel_stationary_alpha)
        # ID
        self._id_row._colour = SETTINGS.id_colour
        _set_colour_button(self._id_row._btn, SETTINGS.id_colour)
        self._id_row._alpha.setValue(SETTINGS.id_alpha)

    def apply(self):
        """Write dialog values into SETTINGS."""
        for key, row in self._dyn_rows.items():
            SETTINGS.dyn_styles[key] = (row.get_colour(), row.get_lw(), row.get_alpha())
        SETTINGS.class_color_mode = self._class_mode_cb.isChecked()
        for key, row in self._class_rows.items():
            SETTINGS.class_styles[key] = (row.get_colour(), row.get_lw(), row.get_alpha())
        SETTINGS.vel_moving_colour     = self._vel_moving.get_colour()
        SETTINGS.vel_moving_alpha      = self._vel_moving.get_alpha()
        SETTINGS.vel_stationary_colour = self._vel_stat.get_colour()
        SETTINGS.vel_stationary_alpha  = self._vel_stat.get_alpha()
        SETTINGS.id_colour = self._id_row.get_colour()
        SETTINGS.id_alpha  = self._id_row.get_alpha()


# ---------------------------------------------------------------------------
# Background loader thread
# ---------------------------------------------------------------------------

class _LoadWorker(QObject):
    finished = pyqtSignal(object)   # DataModel
    error    = pyqtSignal(str)
    progress = pyqtSignal(int, str) # (pct 0-100, status message)

    def __init__(self, mf4_path: str, dbc_path: str):
        super().__init__()
        self.mf4_path = mf4_path
        self.dbc_path = dbc_path

    def run(self) -> None:
        try:
            model = DataModel()
            model.load(self.mf4_path, self.dbc_path,
                       progress_cb=lambda p, m: self.progress.emit(p, m))
            self.finished.emit(model)
        except Exception as exc:
            self.error.emit(str(exc))


# ---------------------------------------------------------------------------
# Main window
# ---------------------------------------------------------------------------

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle(_APP_TITLE)
        self.resize(1400, 900)

        self._model: Optional[DataModel] = None
        self._resim_model: Optional[DataModel] = None
        self._merged_scan_indices: List[int] = []
        self._frame_idx: int = 0
        self._load_thread: Optional[QThread] = None
        self._load_worker: Optional[_LoadWorker] = None

        # Playback
        self._play_timer = QTimer(self)
        self._play_timer.setTimerType(Qt.TimerType.PreciseTimer)
        self._play_timer.timeout.connect(self._on_play_tick)
        self._is_playing = False
        self._play_start_wall: float = 0.0    # wall time when play was started
        self._play_start_frame: int  = 0      # frame index when play was started

        # Plan view axis limits (managed by window-size dialog)
        self._plan_lat_min = -40.0
        self._plan_lat_max =  40.0
        self._plan_lon_min = -60.0
        self._plan_lon_max =  60.0
        self._plan_keep_ratio = True

        self._build_ui()
        self._setup_shortcuts()
        self._apply_dark_style()
        self._restore_window_state()

    # ------------------------------------------------------------------
    # UI construction
    # ------------------------------------------------------------------

    def _build_ui(self) -> None:
        # ── Menu ──────────────────────────────────────────────────────
        menu = self.menuBar()
        file_menu = menu.addMenu("File")

        open_act = QAction("Load original .MF4...", self)
        open_act.setShortcut(QKeySequence("Ctrl+O"))
        open_act.triggered.connect(self._open_file)
        file_menu.addAction(open_act)

        self._load_resim_act = QAction("Load reprocessed .MF4…", self)
        self._load_resim_act.setEnabled(False)
        self._load_resim_act.triggered.connect(self._open_resim_file)
        file_menu.addAction(self._load_resim_act)

        self._unload_resim_act = QAction("Unload reprocessed data", self)
        self._unload_resim_act.setEnabled(False)
        self._unload_resim_act.triggered.connect(self._unload_resim)
        file_menu.addAction(self._unload_resim_act)

        # Create resim source combo early – referenced by Plan View controls row
        self._resim_source_combo = QComboBox()
        self._resim_source_combo.addItems(["Both", "Original only", "Resim only"])
        self._resim_source_combo.setFixedWidth(120)
        self._resim_source_combo.setToolTip("Which data source to show in Plan View")
        self._resim_source_combo.setVisible(False)
        self._resim_source_combo.currentIndexChanged.connect(self._refresh_plan_view)

        file_menu.addSeparator()
        quit_act = QAction("Quit", self)
        quit_act.setShortcut(QKeySequence("Ctrl+Q"))
        quit_act.triggered.connect(QApplication.quit)
        file_menu.addAction(quit_act)

        # ── Tools menu ────────────────────────────────────────────────
        tools_menu = menu.addMenu("Tools")

        self._sanity_act = QAction("Frame Table…", self)
        self._sanity_act.setEnabled(False)
        self._sanity_act.triggered.connect(self._open_sanity_check)
        tools_menu.addAction(self._sanity_act)

        self._frame_count_act = QAction("CAN Frame Count…", self)
        self._frame_count_act.setEnabled(False)
        self._frame_count_act.triggered.connect(self._open_frame_count)
        tools_menu.addAction(self._frame_count_act)

        # ── Settings menu ─────────────────────────────────────────────
        settings_menu = menu.addMenu("Settings")

        theme_menu = settings_menu.addMenu("Theme")
        dark_act = QAction("Dark mode", self)
        dark_act.triggered.connect(lambda: self._apply_theme("dark"))
        theme_menu.addAction(dark_act)
        light_act = QAction("Light mode", self)
        light_act.triggered.connect(lambda: self._apply_theme("light"))
        theme_menu.addAction(light_act)

        pv_act = QAction("Plan View Settings…", self)
        pv_act.triggered.connect(self._open_plan_view_settings)
        settings_menu.addAction(pv_act)

        # ── About menu ────────────────────────────────────────────────
        about_menu = menu.addMenu("About")

        version_act = QAction("Version…", self)
        version_act.triggered.connect(self._show_version_dialog)
        about_menu.addAction(version_act)
        about_menu.addSeparator()

        how_to_use_act = QAction("How to use…", self)
        how_to_use_act.triggered.connect(lambda: self._open_docs(page=0))
        about_menu.addAction(how_to_use_act)

        docs_act = QAction("Documentation…", self)
        docs_act.setShortcut(QKeySequence("F1"))
        docs_act.triggered.connect(self._open_docs)
        about_menu.addAction(docs_act)

        _DOC_PAGES = [
            ("How to use",                    0),
            ("Application Overview",          1),
            ("Data Model \u0026 MF4 Loading",      2),
            ("Object Track Creation",          3),
            ("Resim Comparison \u2014 High Level",         4),
            ("Resim Comparison \u2014 Detailed",   5),
        ]
        about_menu.addSeparator()
        topics_menu = about_menu.addMenu("Jump to topic")
        for label, page_idx in _DOC_PAGES:
            act = QAction(label, self)
            act.triggered.connect(
                lambda checked, p=page_idx: self._open_docs(page=p))
            topics_menu.addAction(act)

        # ── Central widget ────────────────────────────────────────────
        central = QWidget()
        self.setCentralWidget(central)
        root_layout = QVBoxLayout(central)
        root_layout.setContentsMargins(4, 4, 4, 4)
        root_layout.setSpacing(4)

        # ── Tabs fill the full window width ───────────────────────────
        self._tabs = DetachableTabWidget()
        self._tabs.setToolTip("Drag a tab outside this area to detach it into a separate window")
        root_layout.addWidget(self._tabs, stretch=1)

        # ── Signals tab (was left panel) ──────────────────────────────
        signals_tab = QWidget()
        signals_layout = QVBoxLayout(signals_tab)
        signals_layout.setContentsMargins(4, 4, 4, 4)
        signals_layout.setSpacing(4)

        self._tree = SignalTreeWidget()
        signals_layout.addWidget(self._tree, stretch=1)

        btn_row_sig = QWidget()
        btn_row_sig_lay = QHBoxLayout(btn_row_sig)
        btn_row_sig_lay.setContentsMargins(0, 0, 0, 0)
        btn_row_sig_lay.setSpacing(6)

        self._plot_signal_btn = QPushButton("Plot Selected Signals")
        self._plot_signal_btn.setEnabled(False)
        self._plot_signal_btn.clicked.connect(self._plot_selected_signals)
        btn_row_sig_lay.addWidget(self._plot_signal_btn)

        self._add_to_plot_btn = QPushButton("Add to Plot")
        self._add_to_plot_btn.setEnabled(False)
        self._add_to_plot_btn.setToolTip(
            "Append selected signals to the current Signal Plot without clearing it.")
        self._add_to_plot_btn.clicked.connect(self._add_selected_signals)
        btn_row_sig_lay.addWidget(self._add_to_plot_btn)

        signals_layout.addWidget(btn_row_sig)

        # ── Plan View tab (canvas + toolbar + controls) ───────────────
        plan_tab = QWidget()
        plan_tab_layout = QVBoxLayout(plan_tab)
        plan_tab_layout.setContentsMargins(2, 2, 2, 2)
        plan_tab_layout.setSpacing(3)

        self._plan_view = PlanViewWidget()

        # Matplotlib navigation toolbar (zoom / pan / home)
        self._nav_toolbar = NavigationToolbar2QT(self._plan_view, plan_tab)
        self._plan_view._nav_toolbar = self._nav_toolbar   # for mode detection
        plan_tab_layout.addWidget(self._nav_toolbar)

        # Controls row: show IDs, show velocity, window size button
        ctrl_row = QWidget()
        ctrl_layout = QHBoxLayout(ctrl_row)
        ctrl_layout.setContentsMargins(2, 0, 2, 0)
        ctrl_layout.setSpacing(8)

        self._show_ids_cb = QCheckBox("Show IDs")
        self._show_ids_cb.setChecked(False)
        self._show_ids_cb.stateChanged.connect(self._on_show_ids_changed)
        ctrl_layout.addWidget(self._show_ids_cb)

        self._show_velocity_cb = QCheckBox("Show Velocity")
        self._show_velocity_cb.setChecked(True)
        self._show_velocity_cb.stateChanged.connect(self._on_show_velocity_changed)
        ctrl_layout.addWidget(self._show_velocity_cb)

        self._show_heading_cb = QCheckBox("Show Heading Arrow")
        self._show_heading_cb.setChecked(False)
        self._show_heading_cb.stateChanged.connect(self._on_show_heading_changed)
        ctrl_layout.addWidget(self._show_heading_cb)

        self._zoom_mode_btn = QPushButton("Zoom Mode")
        self._zoom_mode_btn.setCheckable(True)
        self._zoom_mode_btn.setChecked(False)
        self._zoom_mode_btn.setFixedWidth(90)
        self._zoom_mode_btn.setToolTip(
            "Enable to zoom with scroll wheel or left-click drag")
        self._zoom_mode_btn.toggled.connect(self._on_zoom_mode_toggled)
        ctrl_layout.addWidget(self._zoom_mode_btn)

        win_size_btn = QPushButton("Window Size...")
        win_size_btn.setFixedWidth(110)
        win_size_btn.clicked.connect(self._open_window_size_dialog)
        ctrl_layout.addWidget(win_size_btn)

        reset_close_btn = QPushButton("Reset View (close)")
        reset_close_btn.setFixedWidth(130)
        reset_close_btn.setToolTip("Reset plan view: lon ±20 m, lat ±20 m")
        reset_close_btn.clicked.connect(self._reset_axis_limits_close)
        ctrl_layout.addWidget(reset_close_btn)

        reset_far_btn = QPushButton("Reset View (far)")
        reset_far_btn.setFixedWidth(115)
        reset_far_btn.setToolTip("Reset plan view: lon +40 / −80 m, lat ±50 m")
        reset_far_btn.clicked.connect(self._reset_axis_limits_far)
        ctrl_layout.addWidget(reset_far_btn)

        # Inline Plan View source selector – only visible when resim is loaded
        self._pv_source_lbl = QLabel("Show:")
        self._pv_source_lbl.setVisible(False)
        ctrl_layout.addWidget(self._pv_source_lbl)
        ctrl_layout.addWidget(self._resim_source_combo)

        ctrl_layout.addStretch()

        plan_tab_layout.addWidget(ctrl_row)

        # Sensor input bar — shows host vehicle data synchronized to current frame
        self._sensor_bar = QLabel("—")
        self._sensor_bar.setStyleSheet(
            "QLabel { background: #1a1a2e; color: #ccccee; font-size: 11px; "
            "padding: 3px 8px; border-bottom: 1px solid #333355; }"
        )
        self._sensor_bar.setAlignment(Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter)
        self._sensor_bar.setFixedHeight(24)
        plan_tab_layout.addWidget(self._sensor_bar)

        plan_tab_layout.addWidget(self._plan_view, stretch=1)

        self._tabs.addTab(plan_tab, "Plan View")

        # Wire double-click on object
        self._plan_view.object_double_clicked.connect(self._on_object_double_clicked)
        # Wire single-click on object → object track view
        self._plan_view.object_clicked.connect(self._on_object_single_click)
        # Wire empty-space click → deselect
        self._plan_view.object_selection_cleared.connect(self._on_object_selection_cleared)

        self._track_widget = ObjectTrackWidget()
        self._tabs.addTab(self._track_widget, "Object Track")
        self._track_widget.plot_signals_requested.connect(self._on_track_plot_signals)
        self._track_widget.add_signals_requested.connect(self._on_track_add_signals)
        self._track_widget.jump_to_scan_requested.connect(self._on_jump_to_scan)
        self._track_widget.tracks_computed.connect(self._on_tracks_computed)

        self._comparison_widget = ResimComparisonWidget()
        self._comparison_widget.jump_requested.connect(self._on_jump_from_resim_kpi)
        self._tabs.addTab(self._comparison_widget, "Resim")

        self._video_widget = VideoWidget()
        self._tabs.addTab(self._video_widget, "Video")

        self._tabs.addTab(signals_tab, "Signals")

        self._sig_plot = SignalPlotWidget()
        self._tabs.addTab(self._sig_plot, "Signal Plot")

        # ── Navigation bar ────────────────────────────────────────────
        nav_widget = QWidget()
        nav_vlay = QVBoxLayout(nav_widget)
        nav_vlay.setContentsMargins(4, 2, 4, 2)
        nav_vlay.setSpacing(2)

        # ── Row 1: buttons + full-width slider ────────────────────────
        nav_row1 = QWidget()
        nav_layout = QHBoxLayout(nav_row1)
        nav_layout.setContentsMargins(0, 0, 0, 0)
        nav_layout.setSpacing(6)

        self._btn_first = QPushButton("|◄\n(Home)")
        self._btn_prev  = QPushButton("◄\n(a)")
        self._btn_next  = QPushButton("►\n(d)")
        self._btn_last  = QPushButton("►|\n(End)")
        self._btn_play  = QPushButton("▶  Play\n(Space)")
        self._btn_play.setFixedWidth(80)
        for btn in (self._btn_first, self._btn_prev, self._btn_next,
                    self._btn_last, self._btn_play):
            btn.setFixedWidth(btn.sizeHint().width() if btn is self._btn_play else 52)
            btn.setEnabled(False)

        self._slider = QSlider(Qt.Orientation.Horizontal)
        self._slider.setEnabled(False)
        self._slider.setMinimum(0)
        self._slider.setMaximum(0)

        nav_layout.addWidget(self._btn_first)
        nav_layout.addWidget(self._btn_prev)
        nav_layout.addWidget(self._slider, stretch=1)
        nav_layout.addWidget(self._btn_next)
        nav_layout.addWidget(self._btn_last)
        nav_layout.addWidget(self._btn_play)
        nav_vlay.addWidget(nav_row1)

        # ── Row 2: secondary controls + info labels ───────────────────
        nav_row2 = QWidget()
        nav_layout2 = QHBoxLayout(nav_row2)
        nav_layout2.setContentsMargins(0, 0, 0, 0)
        nav_layout2.setSpacing(8)

        self._side_combo = QComboBox()
        self._side_combo.addItems(["Both sides", "Left (SRRL)", "Right (SRRR)"])
        self._side_combo.setFixedWidth(120)
        self._side_combo.currentIndexChanged.connect(self._refresh_plan_view)

        self._speed_combo = QComboBox()
        for _lbl in ("0.1x", "0.25x", "0.5x", "1x", "2x", "4x"):
            self._speed_combo.addItem(_lbl)
        self._speed_combo.setCurrentText("1x")
        self._speed_combo.setFixedWidth(68)
        self._speed_combo.setToolTip("Replay speed  (1x = real-time, 20 Hz)")

        self._lbl_scan   = QLabel("Scan  L: —  R: —")
        self._lbl_frame  = QLabel("Frame: — / —")
        self._lbl_ts     = QLabel("Timestamp: — s")

        nav_layout2.addWidget(QLabel("Side:"))
        nav_layout2.addWidget(self._side_combo)
        nav_layout2.addWidget(QLabel("Speed:"))
        nav_layout2.addWidget(self._speed_combo)
        nav_layout2.addStretch()
        nav_layout2.addWidget(self._lbl_scan)
        nav_layout2.addWidget(self._lbl_frame)
        nav_layout2.addWidget(self._lbl_ts)
        nav_vlay.addWidget(nav_row2)

        # ── Row 3: persistent file labels (stacked vertically) ───────────────
        nav_row3 = QWidget()
        nav_layout3 = QVBoxLayout(nav_row3)
        nav_layout3.setContentsMargins(2, 0, 2, 0)
        nav_layout3.setSpacing(1)

        self._lbl_orig_file = QLabel("Original: —")
        self._lbl_orig_file.setStyleSheet(
            "font-size: 10px; color: #8888aa;")
        self._lbl_orig_file.setToolTip("Currently loaded original MF4 file")

        self._lbl_resim_file = QLabel()
        self._lbl_resim_file.setStyleSheet(
            "font-size: 10px; color: #4dd0e1;")
        self._lbl_resim_file.setToolTip("Currently loaded reprocessed MF4 file")
        self._lbl_resim_file.setVisible(False)

        nav_layout3.addWidget(self._lbl_orig_file)
        nav_layout3.addWidget(self._lbl_resim_file)
        nav_vlay.addWidget(nav_row3)

        root_layout.addWidget(nav_widget)

        # Wire navigation
        self._btn_first.clicked.connect(self._go_first)
        self._btn_prev.clicked.connect(self._go_prev)
        self._btn_next.clicked.connect(self._go_next)
        self._btn_last.clicked.connect(self._go_last)
        self._btn_play.clicked.connect(self._on_play_toggled)
        self._slider.valueChanged.connect(self._on_slider)

        # Wire tree
        self._tree.signal_selected.connect(self._on_signal_selected)

        # ── Status bar ────────────────────────────────────────────────
        self.setStatusBar(QStatusBar())
        self.statusBar().showMessage("Ready — open an MF4 file to begin.")

    def _update_file_labels(self) -> None:
        """Refresh the persistent file-name labels under the navigation bar."""
        def _file_text(model) -> str:
            name = Path(model.mf4_path).name
            info = model.scan_index_info()
            parts = [name, f"  |  {info['total']} frames"]
            if info["r_count"]:
                parts.append(
                    f"  |  Scan_Index_R: {info['r_min']} – {info['r_max']}"
                    f" ({info['r_count']})"
                )
            if info["l_count"]:
                parts.append(
                    f"  |  Scan_Index_L: {info['l_min']} – {info['l_max']}"
                    f" ({info['l_count']})"
                )
            return "".join(parts)

        if self._model is not None:
            self._lbl_orig_file.setText(f"Original:  {_file_text(self._model)}")
        else:
            self._lbl_orig_file.setText("Original: —")

        if self._resim_model is not None:
            self._lbl_resim_file.setText(f"Resim:  {_file_text(self._resim_model)}")
            self._lbl_resim_file.setVisible(True)
        else:
            self._lbl_resim_file.setVisible(False)

    # ------------------------------------------------------------------
    # Dark style sheet
    # ------------------------------------------------------------------

    def _apply_dark_style(self) -> None:
        self.setStyleSheet("""
            QMainWindow, QWidget {
                background-color: #12122a;
                color: #ccccff;
            }
            QMenuBar {
                background-color: #1a1a3e;
                color: #ccccff;
            }
            QMenuBar::item:selected { background-color: #2a2a6e; }
            QMenu {
                background-color: #1a1a3e;
                color: #ccccff;
                border: 1px solid #444466;
            }
            QMenu::item:selected { background-color: #2a2a6e; }
            QPushButton {
                background-color: #2a2a5e;
                color: #ccccff;
                border: 1px solid #555588;
                border-radius: 3px;
                padding: 3px 8px;
            }
            QPushButton:hover  { background-color: #3a3a7e; }
            QPushButton:pressed { background-color: #1a1a4e; }
            QPushButton:disabled { color: #555577; border-color: #333355; }
            QSlider::groove:horizontal {
                background: #2a2a5e;
                height: 6px;
                border-radius: 3px;
            }
            QSlider::handle:horizontal {
                background: #8888cc;
                width: 14px; height: 14px;
                border-radius: 7px;
                margin: -4px 0;
            }
            QSlider::sub-page:horizontal { background: #5555aa; border-radius: 3px; }
            QTabWidget::pane {
                border: 1px solid #444466;
                background: #12122a;
            }
            QTabBar::tab {
                background: #1a1a3e;
                color: #8888cc;
                padding: 5px 16px;
                border: 1px solid #444466;
                border-bottom: none;
            }
            QTabBar::tab:selected { background: #2a2a6e; color: white; }
            QComboBox {
                background-color: #2a2a5e;
                color: #ccccff;
                border: 1px solid #555588;
                border-radius: 3px;
                padding: 2px 6px;
            }
            QComboBox QAbstractItemView {
                background-color: #1a1a3e;
                color: #ccccff;
                selection-background-color: #2a2a6e;
            }
            QLabel { color: #aaaacc; }
            QStatusBar { background: #0e0e22; color: #8888aa; }
            QSplitter::handle { background: #333366; }
        """)

    # ------------------------------------------------------------------
    # File loading
    # ------------------------------------------------------------------

    def _open_file(self) -> None:
        settings = QSettings("TRATON", "SRR6T_VCAN_Visualiser")
        last_dir = settings.value("last_open_orig_dir", str(Path.home()))
        path, _ = QFileDialog.getOpenFileName(
            self, "Load original .MF4", last_dir,
            "MF4 files (*.mf4);;All files (*.*)"
        )
        if not path:
            return
        settings.setValue("last_open_orig_dir", str(Path(path).parent))
        self._open_file_path(path)

    def _open_file_path(self, path: str) -> None:
        """Start background loading of *path* (called from dialog or CLI)."""
        self._progress = QProgressDialog(
            "Decoding MF4 file…", None, 0, 100, self)
        self._progress.setWindowModality(Qt.WindowModality.WindowModal)
        self._progress.setWindowTitle("Loading")
        self._progress.setMinimumDuration(0)
        self._progress.setValue(0)
        self._progress.show()
        QApplication.processEvents()

        dbc_path = str(_DBC_DEFAULT)

        # Run decoder in a background thread so UI stays responsive.
        self._load_thread = QThread()
        self._load_worker = _LoadWorker(path, dbc_path)
        self._load_worker.moveToThread(self._load_thread)
        self._load_thread.started.connect(self._load_worker.run)
        self._load_worker.finished.connect(self._on_load_finished)
        self._load_worker.error.connect(self._on_load_error)
        self._load_worker.finished.connect(self._load_thread.quit)
        self._load_worker.error.connect(self._load_thread.quit)
        self._load_worker.progress.connect(self._on_load_progress)
        self._load_thread.start()

    def _on_load_progress(self, pct: int, message: str) -> None:
        self._progress.setLabelText(message)
        self._progress.setValue(pct)
        QApplication.processEvents()

    def _on_load_finished(self, model: DataModel) -> None:
        self._progress.close()
        self._model = model
        # Loading new original file clears any previous resim
        self._resim_model = None
        self._resim_source_combo.setVisible(False)
        self._pv_source_lbl.setVisible(False)
        self._unload_resim_act.setEnabled(False)
        n = model.frame_count()

        if n == 0:
            QMessageBox.warning(self, "No data",
                "No SRRL/SRRR scan frames found in this file.")
            return

        self._merged_scan_indices = list(model.scan_indices)

        # Update slider
        self._slider.setMaximum(n - 1)
        self._slider.setValue(0)
        self._slider.setEnabled(True)
        for btn in (self._btn_first, self._btn_prev, self._btn_next,
                    self._btn_last, self._btn_play):
            btn.setEnabled(True)

        # Populate signal tree (single-source mode)
        self._tree.populate(model.frames)
        self._auto_size_tree_panel()

        # Set model on plan view
        self._plan_view.set_model(model)
        self._sig_plot.set_model(model)
        self._track_widget.set_model(model)

        # Clear stale comparison results — they belong to the previous file.
        # Also drops the resim model reference inside the widget and disables
        # "Compare Resim" until a new reprocessed file is loaded.
        self._comparison_widget.clear()
        self._comparison_widget.set_models(model, None)

        # Enable load-resim and sanity-check now that original is loaded
        self._load_resim_act.setEnabled(True)
        self._sanity_act.setEnabled(True)
        self._frame_count_act.setEnabled(True)

        # Update file label
        self._update_file_labels()

        # Notify video widget about the new file
        self._video_widget.set_mf4_path(str(model.mf4_path))

        # Show first frame
        self._frame_idx = 0
        self._show_frame(0)

        name = Path(model.mf4_path).name
        fmt_label = {
            "man_autera":   "MAN-Autera",
            "aptiv_orcas":  "APTIV-Orcas",
            "scania_orcas": "SCANIA-Orcas",
            "resim_canoe":  "RESIM-Canoe",
        }.get(model.mf4_format, model.mf4_format)
        self.statusBar().showMessage(
            f"Loaded: {name}  |  {n} scan frames  |  "
            f"{len(model.frames)} message types  |  Format: {fmt_label}"
        )

    def _on_load_error(self, msg: str) -> None:
        self._progress.close()
        QMessageBox.critical(self, "Load error", f"Failed to load file:\n{msg}")

    # ------------------------------------------------------------------
    # Resim loading
    # ------------------------------------------------------------------

    def _open_resim_file(self) -> None:
        settings = QSettings("TRATON", "SRR6T_VCAN_Visualiser")
        last_dir = settings.value("last_open_resim_dir", str(Path.home()))
        path, _ = QFileDialog.getOpenFileName(
            self, "Load reprocessed .MF4", last_dir,
            "MF4 files (*.mf4);;All files (*.*)"
        )
        if not path:
            return
        settings.setValue("last_open_resim_dir", str(Path(path).parent))

        self._progress = QProgressDialog(
            "Decoding reprocessed MF4…", None, 0, 100, self)
        self._progress.setWindowModality(Qt.WindowModality.WindowModal)
        self._progress.setWindowTitle("Loading Resim")
        self._progress.setMinimumDuration(0)
        self._progress.setValue(0)
        self._progress.show()
        QApplication.processEvents()

        self._load_thread = QThread()
        self._load_worker = _LoadWorker(path, str(_DBC_DEFAULT))
        self._load_worker.moveToThread(self._load_thread)
        self._load_thread.started.connect(self._load_worker.run)
        self._load_worker.finished.connect(self._on_resim_load_finished)
        self._load_worker.error.connect(self._on_load_error)
        self._load_worker.finished.connect(self._load_thread.quit)
        self._load_worker.error.connect(self._load_thread.quit)
        self._load_worker.progress.connect(self._on_load_progress)
        self._load_thread.start()

    def _on_resim_load_finished(self, model: DataModel) -> None:
        self._progress.close()

        if model.frame_count() == 0:
            QMessageBox.warning(self, "No data",
                "No SRRL/SRRR scan frames found in the reprocessed file.")
            return

        # Validate overlap using SRRR scan indices as reference
        def _srrr_set(m: DataModel) -> set:
            return {s for s in m.scan_indices}

        orig_set  = _srrr_set(self._model)
        resim_set = _srrr_set(model)
        overlap = orig_set & resim_set
        overlap_pct = 100.0 * len(overlap) / max(len(orig_set), len(resim_set), 1)
        if overlap_pct < 10.0:
            QMessageBox.critical(
                self, "Session mismatch",
                f"The reprocessed file overlaps only {overlap_pct:.1f}% of scan indices "
                f"with the original log (minimum required: 10%).\n"
                "These files may not be from the same session.\n\n"
                f"Original scan range:  {min(orig_set)} – {max(orig_set)}\n"
                f"Resim scan range:     {min(resim_set)} – {max(resim_set)}\n"
                f"Overlapping frames:   {len(overlap)} / {max(len(orig_set), len(resim_set))}"
            )
            return

        self._resim_model = model

        # Merged indices: union of both sets, sorted
        self._merged_scan_indices = sorted(orig_set | resim_set)
        n = len(self._merged_scan_indices)
        self._slider.setMaximum(n - 1)

        # Switch signal tree to grouped mode
        self._tree.clear()
        self._tree.populate(self._model.frames,  group_label="Original")
        self._tree.populate(self._resim_model.frames, group_label="Resim")
        self._auto_size_tree_panel()

        # Show resim source combo
        self._resim_source_combo.setVisible(True)
        self._pv_source_lbl.setVisible(True)
        self._plan_view.set_dual_mode(True)

        # Enable unload
        self._unload_resim_act.setEnabled(True)

        # Update file labels
        self._update_file_labels()

        # Clear stale KPI results before the new computation starts
        self._comparison_widget.clear()

        # Notify object track widget about resim model
        self._track_widget.set_resim_model(model)

        # Notify comparison widget
        self._comparison_widget.set_models(self._model, model)

        # Refresh current frame
        self._show_frame(self._frame_idx)

        name = Path(model.mf4_path).name
        self.statusBar().showMessage(
            f"Resim loaded: {name}  |  "
            f"{len(self._merged_scan_indices)} merged scan frames"
        )

    def _unload_resim(self) -> None:
        """Remove reprocessed data and return to single-source mode."""
        self._resim_model = None
        self._merged_scan_indices = list(self._model.scan_indices)
        n = len(self._merged_scan_indices)
        self._slider.setMaximum(n - 1)

        # Restore single-source signal tree
        self._tree.populate(self._model.frames)
        self._auto_size_tree_panel()

        # Hide resim source combo
        self._resim_source_combo.setVisible(False)
        self._pv_source_lbl.setVisible(False)
        self._unload_resim_act.setEnabled(False)
        self._plan_view.set_dual_mode(False)

        self._track_widget.set_resim_model(None)

        # Reset comparison widget
        self._comparison_widget.set_models(None, None)

        frame_idx = min(self._frame_idx, n - 1)
        self._show_frame(frame_idx)
        self.statusBar().showMessage("Reprocessed data unloaded.")

    # ------------------------------------------------------------------
    # Navigation
    # ------------------------------------------------------------------

    def _show_frame(self, frame_idx: int) -> None:
        if self._model is None:
            return
        n = len(self._merged_scan_indices)
        frame_idx = max(0, min(frame_idx, n - 1))
        self._frame_idx = frame_idx

        scan_idx = self._merged_scan_indices[frame_idx]
        ts = self._model.get_header_timestamp(scan_idx) or 0.0

        scan_l = self._model.display_scan(scan_idx, "L")
        scan_r = self._model.display_scan(scan_idx, "R")
        self._lbl_scan.setText(f"Scan  L: {scan_l}  R: {scan_r}")
        self._lbl_frame.setText(f"Frame: {frame_idx + 1} / {n}")
        self._lbl_ts.setText(f"Timestamp: {ts:.3f} s")

        # Block slider signals while updating to avoid recursion
        self._slider.blockSignals(True)
        self._slider.setValue(frame_idx)
        self._slider.blockSignals(False)

        self._refresh_plan_view()

        # Update cursor in signal plot only when its tab is visible
        sig_plot_visible = (
            self._tabs.indexOf(self._sig_plot) == self._tabs.currentIndex()
            or self._sig_plot in [
                fp._widget for fp in self._tabs._floating.values()
            ]
        )
        if sig_plot_visible or not self._is_playing:
            self._sig_plot.set_cursor(ts)

        # Update object-track current column only when its tab is visible
        track_visible = (
            self._tabs.indexOf(self._track_widget) == self._tabs.currentIndex()
            or self._track_widget in [
                fp._widget for fp in self._tabs._floating.values()
            ]
        )
        if track_visible or not self._is_playing:
            self._track_widget.update_scan(scan_idx)

        # Update video widget (O(1) dict lookup; no-op if video not loaded)
        self._video_widget.show_frame(scan_idx)

    def _refresh_plan_view(self) -> None:
        if self._model is None:
            return
        scan_idx = self._merged_scan_indices[self._frame_idx]
        objects = self._model.get_objects(scan_idx)

        side_filter = self._side_combo.currentIndex()
        if side_filter == 1:
            objects = [o for o in objects if o.side == "L"]
        elif side_filter == 2:
            objects = [o for o in objects if o.side == "R"]

        # Resim objects
        resim_objects: List[ObjectRecord] = []
        if self._resim_model is not None:
            resim_src = self._resim_source_combo.currentIndex()   # 0=Both,1=Orig,2=Resim
            if resim_src != 2:   # not "Resim only" → show original
                pass             # objects already populated above
            else:
                objects = []     # hide original
            if resim_src != 1:   # not "Original only" → show resim
                resim_objects = self._resim_model.get_objects(scan_idx)
                if side_filter == 1:
                    resim_objects = [o for o in resim_objects if o.side == "L"]
                elif side_filter == 2:
                    resim_objects = [o for o in resim_objects if o.side == "R"]

        host_data = self._model.get_host_data(scan_idx)
        self._update_sensor_bar(host_data)
        self._plan_view.update_frame_with_objects(scan_idx, objects, host_data,
                                                   resim_objects=resim_objects)

    _TRAILER_LABELS = {0: "None", 1: "Connected", 14: "Error", 15: "N/A"}

    def _update_sensor_bar(self, host_data: Optional[dict]) -> None:
        if host_data is None:
            self._sensor_bar.setText("SensorInput: no data")
            return
        spd   = host_data.get("VehicleSpeed", 0.0)
        trl   = int(host_data.get("TrailerConnection", 0))
        steer = host_data.get("SteeringWheelAngle", 0.0)
        yaw   = host_data.get("YawRate", 0.0)
        lon_a = host_data.get("LongitudinalAcceleration", 0.0)
        lat_a = host_data.get("LateralAcceleration", 0.0)
        trl_s = self._TRAILER_LABELS.get(trl, str(trl))
        self._sensor_bar.setText(
            f"Speed: {spd:.1f} km/h  │  Trailer: {trl_s}  │  "
            f"SteerAngle: {steer:.1f}°  │  YawRate: {yaw:.3f} rad/s  │  "
            f"LonAcc: {lon_a:.2f} m/s²  │  LatAcc: {lat_a:.2f} m/s²"
        )

    def _go_first(self) -> None: self._show_frame(0)
    def _go_last(self)  -> None: self._show_frame(self._model.frame_count() - 1)

    def _go_prev(self) -> None:
        self._show_frame(self._frame_idx - 1)

    def _go_next(self) -> None:
        if self._model and self._frame_idx >= self._model.frame_count() - 1:
            self._stop_play()
            return
        self._show_frame(self._frame_idx + 1)

    def _on_slider(self, value: int) -> None:
        self._show_frame(value)

    # ------------------------------------------------------------------
    # Playback
    # ------------------------------------------------------------------

    def _on_play_toggled(self) -> None:
        if self._is_playing:
            self._stop_play()
        else:
            self._start_play()

    def _start_play(self) -> None:
        if self._model is None:
            return
        self._is_playing = True
        self._btn_play.setText("⏸  Pause")
        _speed_ms = {"0.1x": 500, "0.25x": 200, "0.5x": 100, "1x": 50, "2x": 25, "4x": 13}
        interval = _speed_ms.get(self._speed_combo.currentText(), 50)
        # Record wall-clock reference so the tick handler can catch up if
        # rendering is slower than the requested speed.
        self._play_start_wall  = time.monotonic()
        self._play_start_frame = self._frame_idx
        self._play_timer.start(interval)

    def _stop_play(self) -> None:
        self._is_playing = False
        self._btn_play.setText("▶  Play")
        self._play_timer.stop()

    def _on_play_tick(self) -> None:
        if self._model is None:
            self._stop_play()
            return
        if self._frame_idx >= self._model.frame_count() - 1:
            self._stop_play()
            return
        # Compute how many frames should have elapsed based on wall-clock time
        # so we maintain the requested speed even if rendering is slow.
        _speed_factors = {"0.1x": 0.1, "0.25x": 0.25, "0.5x": 0.5,
                          "1x": 1.0, "2x": 2.0, "4x": 4.0}
        spd = _speed_factors.get(self._speed_combo.currentText(), 1.0)
        elapsed_s = time.monotonic() - self._play_start_wall
        target_frame = self._play_start_frame + int(elapsed_s / 0.050 * spd)
        # Advance to target (at least 1, at most 8 to avoid huge visual jumps
        # when coming back from a tab switch or system hiccup).
        next_frame = max(self._frame_idx + 1,
                         min(target_frame, self._frame_idx + 8))
        self._show_frame(next_frame)

    # ------------------------------------------------------------------
    # Plan view axis / display controls
    # ------------------------------------------------------------------

    def _auto_size_tree_panel(self) -> None:
        """Switch to Signals tab so the tree is visible after loading."""
        self._tabs.show_panel(self._tree.parent().parent())

    def _apply_axis_limits(self) -> None:
        self._plan_view.set_axis_limits(
            lat_min=self._plan_lat_min,
            lat_max=self._plan_lat_max,
            lon_min=self._plan_lon_min,
            lon_max=self._plan_lon_max,
        )
        self._plan_view.set_keep_ratio(self._plan_keep_ratio)
        self._refresh_plan_view()

    def _reset_axis_limits_close(self) -> None:
        """Reset plan view to a close-range window: lon ±20 m, lat ±20 m."""
        self._plan_lat_min = -20.0
        self._plan_lat_max =  20.0
        self._plan_lon_min = -20.0
        self._plan_lon_max =  20.0
        self._plan_keep_ratio = True
        self._apply_axis_limits()

    def _reset_axis_limits_far(self) -> None:
        """Reset plan view to a far-range window: lon +40/−80 m, lat ±50 m."""
        self._plan_lat_min = -50.0
        self._plan_lat_max =  50.0
        self._plan_lon_min = -80.0
        self._plan_lon_max =  40.0
        self._plan_keep_ratio = True
        self._apply_axis_limits()

    def _on_show_ids_changed(self) -> None:
        self._plan_view.set_show_ids(self._show_ids_cb.isChecked())
        self._refresh_plan_view()

    def _on_show_velocity_changed(self) -> None:
        self._plan_view.set_show_velocity(self._show_velocity_cb.isChecked())
        self._refresh_plan_view()

    def _on_show_heading_changed(self) -> None:
        self._plan_view.set_show_heading(self._show_heading_cb.isChecked())
        self._refresh_plan_view()

    def _on_zoom_mode_toggled(self, checked: bool) -> None:
        self._plan_view.set_zoom_mode(checked)
        self._zoom_mode_btn.setStyleSheet(
            "background:#5555aa; color:white;" if checked else ""
        )

    def _open_window_size_dialog(self) -> None:
        dlg = _WindowSizeDialog(
            self._plan_lat_min, self._plan_lat_max,
            self._plan_lon_min, self._plan_lon_max,
            self._plan_keep_ratio,
            self,
        )
        if dlg.exec():
            lat_min, lat_max, lon_min, lon_max, keep_ratio = dlg.get_values()
            self._plan_lat_min = lat_min
            self._plan_lat_max = lat_max
            self._plan_lon_min = lon_min
            self._plan_lon_max = lon_max
            self._plan_keep_ratio = keep_ratio
            self._apply_axis_limits()

    # ------------------------------------------------------------------
    # Theme & plan-view settings
    # ------------------------------------------------------------------

    def _apply_theme(self, theme: str) -> None:
        SETTINGS.theme = theme
        self._current_theme = theme
        if theme == "dark":
            self._apply_dark_style()
        else:
            self._apply_light_style()
        # Propagate to embedded matplotlib widgets and tree
        self._plan_view.set_theme(theme)
        self._sig_plot.set_theme(theme)
        self._tree.set_theme(theme)
        self._track_widget.set_theme(theme)
        self._comparison_widget.set_theme(theme)
        # Propagate to docs dialog if open
        if hasattr(self, "_docs_dialog") and self._docs_dialog is not None:
            self._docs_dialog.set_theme(theme)
        self._refresh_plan_view()

    def _apply_light_style(self) -> None:
        self.setStyleSheet("""
            QMainWindow, QWidget {
                background-color: #f5f5f5;
                color: #111111;
            }
            QMenuBar {
                background-color: #e0e0e0;
                color: #111111;
            }
            QMenuBar::item:selected { background-color: #c8c8c8; }
            QMenu {
                background-color: #f0f0f0;
                color: #111111;
                border: 1px solid #aaaaaa;
            }
            QMenu::item:selected { background-color: #c8c8c8; }
            QPushButton {
                background-color: #e0e0e0;
                color: #111111;
                border: 1px solid #aaaaaa;
                border-radius: 3px;
                padding: 3px 8px;
            }
            QPushButton:hover  { background-color: #cccccc; }
            QPushButton:pressed { background-color: #b8b8b8; }
            QPushButton:disabled { color: #aaaaaa; border-color: #cccccc; }
            QSlider::groove:horizontal {
                background: #dddddd;
                height: 6px;
                border-radius: 3px;
            }
            QSlider::handle:horizontal {
                background: #888888;
                width: 14px; height: 14px;
                border-radius: 7px;
                margin: -4px 0;
            }
            QSlider::sub-page:horizontal { background: #8888cc; border-radius: 3px; }
            QTabWidget::pane {
                border: 1px solid #aaaaaa;
                background: #f5f5f5;
            }
            QTabBar::tab {
                background: #e0e0e0;
                color: #444444;
                padding: 5px 16px;
                border: 1px solid #aaaaaa;
                border-bottom: none;
            }
            QTabBar::tab:selected { background: #f5f5f5; color: #111111; }
            QComboBox {
                background-color: #ffffff;
                color: #111111;
                border: 1px solid #aaaaaa;
                border-radius: 3px;
                padding: 2px 6px;
            }
            QComboBox QAbstractItemView {
                background-color: #ffffff;
                color: #111111;
                selection-background-color: #c8c8e8;
            }
            QLabel { color: #333333; }
            QStatusBar { background: #e8e8e8; color: #444444; }
            QSplitter::handle { background: #cccccc; }
        """)

    def _open_plan_view_settings(self, reset: bool = False) -> None:
        dlg = _PlanViewSettingsDialog(self)
        if dlg.exec():
            dlg.apply()
            self._plan_view.invalidate_background()
            self._refresh_plan_view()

    def _show_version_dialog(self) -> None:
        """Show a modal dialog with version and build information."""
        import platform
        from PyQt6.QtCore import PYQT_VERSION_STR, QT_VERSION_STR
        try:
            import asammdf; asammdf_ver = asammdf.__version__
        except Exception:
            asammdf_ver = "n/a"
        try:
            import scipy; scipy_ver = scipy.__version__
        except Exception:
            scipy_ver = "n/a"
        try:
            import numpy; numpy_ver = numpy.__version__
        except Exception:
            numpy_ver = "n/a"

        dlg = QDialog(self)
        dlg.setWindowTitle("About — Version Information")
        dlg.setMinimumWidth(420)
        dlg.setModal(True)
        lay = QVBoxLayout(dlg)
        lay.setSpacing(10)

        def _row(label: str, value: str) -> QWidget:
            w = QWidget()
            h = QHBoxLayout(w)
            h.setContentsMargins(0, 0, 0, 0)
            lbl = QLabel(f"<b>{label}</b>")
            lbl.setFixedWidth(160)
            val = QLabel(value)
            val.setTextInteractionFlags(
                Qt.TextInteractionFlag.TextSelectableByMouse)
            h.addWidget(lbl)
            h.addWidget(val, stretch=1)
            return w

        lay.addWidget(QLabel("<h2>TRATON SRR6+T VCAN VISUALISER</h2>"))
        lay.addWidget(_row("Application version:", f"v{APP_VERSION}"))
        lay.addWidget(_row("Python:", platform.python_version()))
        lay.addWidget(_row("PyQt6 / Qt:", f"{PYQT_VERSION_STR} / {QT_VERSION_STR}"))
        lay.addWidget(_row("asammdf:", asammdf_ver))
        lay.addWidget(_row("scipy:", scipy_ver))
        lay.addWidget(_row("numpy:", numpy_ver))
        lay.addWidget(_row("Platform:", platform.platform(terse=True)))

        sep = QLabel("<hr/>")
        lay.addWidget(sep)
        contact = QLabel("Developer contact: "
                         "<a href='mailto:szymon.bruzda@aptiv.com'>"
                         "szymon.bruzda@aptiv.com</a>")
        contact.setOpenExternalLinks(True)
        contact.setTextInteractionFlags(
            Qt.TextInteractionFlag.TextBrowserInteraction)
        lay.addWidget(contact)

        btn = QDialogButtonBox(QDialogButtonBox.StandardButton.Ok)
        btn.accepted.connect(dlg.accept)
        lay.addWidget(btn)
        dlg.exec()

    def _open_sanity_check(self) -> None:
        """Open the Frame Table dialog."""
        if self._model is None:
            return
        dlg = SanityCheckDialog(
            orig_model=self._model,
            resim_model=self._resim_model,
            parent=self,
        )
        dlg.exec()

    def _open_frame_count(self) -> None:
        """Open the CAN Frame Count dialog."""
        if self._model is None:
            return
        dlg = CanFrameCountDialog(
            orig_model=self._model,
            dbc_path=_DBC_DEFAULT,
            prefixes=("SRRL_", "SRRR_", "SRR2_"),
            resim_model=self._resim_model,
            parent=self,
        )
        dlg.exec()

    def _open_docs(self, page: int = 0) -> None:
        """Open the documentation viewer (modeless, reuses existing window)."""
        if not hasattr(self, "_docs_dialog") or self._docs_dialog is None:
            theme = getattr(self, "_current_theme", "dark")
            self._docs_dialog = DocsViewerDialog(theme=theme, start_page=page,
                                                  parent=self)
            self._docs_dialog.destroyed.connect(
                lambda: setattr(self, "_docs_dialog", None))
        else:
            # Bring existing window to front and navigate to the requested page
            self._docs_dialog._list.setCurrentRow(page)
        self._docs_dialog.show()
        self._docs_dialog.raise_()
        self._docs_dialog.activateWindow()

    # ------------------------------------------------------------------
    # Object detail dialog (double-click on plan view)
    # ------------------------------------------------------------------

    def _on_object_single_click(self, obj: ObjectRecord) -> None:
        """Single-click on plan view: show corresponding object track."""
        if self._model is None:
            return
        scan_idx = self._merged_scan_indices[self._frame_idx]
        data_source = getattr(obj, '_data_source', 'Original')
        self._plan_view.set_selected_object(obj.side, obj.tracking_id, data_source)
        self._refresh_plan_view()
        self._track_widget.show_for_object(obj, scan_idx)
        self._tabs.show_panel(self._track_widget)

    def _on_object_selection_cleared(self) -> None:
        """Click on empty plan-view space: clear highlight and track selection."""
        self._plan_view.clear_selected_object()
        self._refresh_plan_view()
        self._track_widget.clear_selection()

    def _on_track_plot_signals(self, signals: list) -> None:
        """Plot object-track signals in the Signal Plot tab."""
        self._sig_plot.plot_signals(signals)
        self._tabs.show_panel(self._sig_plot)

    def _on_tracks_computed(self, tracks: list) -> None:
        """Forward computed tracks to the Resim Comparison widget."""
        self._comparison_widget.set_tracks(tracks)

    def _on_track_add_signals(self, signals: list) -> None:
        """Append object-track signals to the current Signal Plot."""
        self._sig_plot.add_signals(signals)
        self._tabs.show_panel(self._sig_plot)

    def _on_jump_to_scan(self, scan_idx: int) -> None:
        """Jump playback to the frame corresponding to scan_idx."""
        if self._model is None:
            return
        try:
            frame_idx = self._merged_scan_indices.index(scan_idx)
        except ValueError:
            return
        self._show_frame(frame_idx)

    def _on_jump_from_resim_kpi(self, scan_idx: int, side: str,
                                 orig_tid: int, resim_tid: int,
                                 scan_end: int) -> None:
        """Jump to scan_idx and highlight the matched track pair in Plan View."""
        self._on_jump_to_scan(scan_idx)
        keys = set()
        if orig_tid >= 0:
            keys.add((side, orig_tid, "Original", scan_idx, scan_end))
        if resim_tid >= 0:
            keys.add((side, resim_tid, "Resim", scan_idx, scan_end))
        self._plan_view.set_jump_highlights(keys)
        self._tabs.setCurrentIndex(0)   # switch to Plan View tab

    def _on_object_double_clicked(self, obj: ObjectRecord) -> None:
        dlg = _ObjectDetailDialog(obj, self)
        dlg.show()

    # ------------------------------------------------------------------
    # Signal tree / plot
    # ------------------------------------------------------------------

    def _on_signal_selected(self, msg_name: str, sig_name: str) -> None:
        self._plot_signal_btn.setEnabled(True)
        self._add_to_plot_btn.setEnabled(True)
        self.statusBar().showMessage(f"Selected: {msg_name}  ›  {sig_name}")

    def _plot_selected_signals(self) -> None:
        if self._model is None:
            return
        selected = self._tree.get_selected_signals()
        if not selected:
            return
        signals = []
        for msg_name, sig_name in selected:
            df = self._model.frames.get(msg_name)
            if df is not None and sig_name in df.columns:
                signals.append((msg_name, sig_name, df))
        if not signals:
            return
        self._sig_plot.plot_signals(signals)
        self._tabs.show_panel(self._sig_plot)
        if self._model and self._model.scan_indices:
            scan_idx = self._model.scan_indices[self._frame_idx]
            ts = self._model.get_header_timestamp(scan_idx) or 0.0
            self._sig_plot.set_cursor(ts)

    def _add_selected_signals(self) -> None:
        """Append selected tree signals to the current Signal Plot."""
        if self._model is None:
            return
        selected = self._tree.get_selected_signals()
        if not selected:
            return
        signals = []
        for msg_name, sig_name in selected:
            df = self._model.frames.get(msg_name)
            if df is not None and sig_name in df.columns:
                signals.append((msg_name, sig_name, df))
        if not signals:
            return
        self._sig_plot.add_signals(signals)
        self._tabs.show_panel(self._sig_plot)
        if self._model and self._model.scan_indices:
            scan_idx = self._model.scan_indices[self._frame_idx]
            ts = self._model.get_header_timestamp(scan_idx) or 0.0
            self._sig_plot.set_cursor(ts)

    # ------------------------------------------------------------------
    # Keyboard shortcuts
    # ------------------------------------------------------------------

    def closeEvent(self, event) -> None:
        reply = QMessageBox.question(
            self,
            "Confirm exit",
            "Are you sure you want to close?",
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No,
            QMessageBox.StandardButton.No,
        )
        if reply == QMessageBox.StandardButton.Yes:
            self._save_window_state()
            event.accept()
        else:
            event.ignore()

    def _save_window_state(self) -> None:
        """Persist window geometry and floating tab positions."""
        s = QSettings("TRATON", "SRR6T_VCAN_Visualiser")
        s.setValue("window/geometry", self.saveGeometry())
        self._tabs.save_layout(s, "window/tabs")

    def _restore_window_state(self) -> None:
        """Restore window geometry and floating tab positions."""
        s = QSettings("TRATON", "SRR6T_VCAN_Visualiser")
        geom = s.value("window/geometry")
        if geom:
            self.restoreGeometry(geom)
        self._tabs.restore_layout(s, "window/tabs")

    def _setup_shortcuts(self) -> None:
        """Register application-level shortcuts that fire regardless of focus."""
        _bindings = [
            (Qt.Key.Key_Space, self._on_play_toggled),
            (Qt.Key.Key_Left,  self._go_prev),
            (Qt.Key.Key_Right, self._go_next),
            (Qt.Key.Key_Home,  self._go_first),
            (Qt.Key.Key_End,   self._go_last),
            (Qt.Key.Key_A,     self._go_prev),
            (Qt.Key.Key_D,     self._go_next),
        ]
        for key, slot in _bindings:
            sc = QShortcut(QKeySequence(key), self)
            sc.setContext(Qt.ShortcutContext.WindowShortcut)
            sc.activated.connect(slot)


# ---------------------------------------------------------------------------
# Object detail dialog
# ---------------------------------------------------------------------------

class _ObjectDetailDialog(QDialog):
    """Non-modal window showing all decoded signals for one object."""

    def __init__(self, obj: ObjectRecord, parent=None):
        super().__init__(parent)
        side_label = "Left (SRRL)" if obj.side == "L" else "Right (SRRR)"
        self.setWindowTitle(
            f"Object #{obj.obj_id:02d}  |  {side_label}  |  "
            f"TrackingId: {obj.tracking_id}"
        )
        self.resize(480, 520)
        self.setWindowFlags(
            self.windowFlags() | Qt.WindowType.Window
        )
        layout = QVBoxLayout(self)

        # Summary header
        summary = (
            f"Side: {side_label}    Slot: #{obj.obj_id:02d}    "
            f"TrackingId: {obj.tracking_id}\n"
            f"Position (centre):  Lon = {obj.lon:.2f} m   Lat = {obj.lat:.2f} m\n"
            f"Size:  Length = {obj.length:.2f} m   Width = {obj.width:.2f} m\n"
            f"Heading: {obj.heading:.4f} rad    "
            f"DynProperty: {obj.dyn_property}    "
            f"ExistProb: {obj.existence_prob:.1f}%\n"
            f"Timestamp: {obj.timestamp:.4f} s"
        )
        lbl = QLabel(summary)
        lbl.setStyleSheet("color:#ccccff; font-family:monospace; font-size:11px;")
        layout.addWidget(lbl)

        # Signal table
        raw = obj.raw_signals
        table = QTableWidget(len(raw), 2)
        table.setHorizontalHeaderLabels(["Signal", "Value"])
        table.horizontalHeader().setSectionResizeMode(
            0, QHeaderView.ResizeMode.Stretch
        )
        table.horizontalHeader().setSectionResizeMode(
            1, QHeaderView.ResizeMode.ResizeToContents
        )
        table.verticalHeader().setVisible(False)
        table.setEditTriggers(QTableWidget.EditTrigger.NoEditTriggers)
        table.setStyleSheet(
            "QTableWidget { background:#1a1a2e; color:#ccccff; "
            "gridline-color:#333366; font-size:11px; }"
            "QHeaderView::section { background:#12122a; color:#8888cc; "
            "border:none; padding:3px; }"
        )

        for row, (k, v) in enumerate(sorted(raw.items())):
            table.setItem(row, 0, QTableWidgetItem(str(k)))
            val_str = f"{v:.4f}" if isinstance(v, float) else str(v)
            table.setItem(row, 1, QTableWidgetItem(val_str))

        layout.addWidget(table, stretch=1)

        btn_box = QDialogButtonBox(QDialogButtonBox.StandardButton.Close)
        btn_box.rejected.connect(self.close)
        layout.addWidget(btn_box)

        self.setStyleSheet(
            "QDialog { background:#12122a; color:#ccccff; }"
            "QLabel { color:#ccccff; }"
            "QPushButton { background:#2a2a5e; color:#ccccff; "
            "border:1px solid #555588; border-radius:3px; padding:3px 10px; }"
            "QPushButton:hover { background:#3a3a7e; }"
        )


# ---------------------------------------------------------------------------
# Window size dialog
# ---------------------------------------------------------------------------

class _WindowSizeDialog(QDialog):
    """Modal dialog to set plan-view axis limits."""

    _STYLE = (
        "QDialog { background:#12122a; color:#ccccff; }"
        "QLabel  { color:#aaaacc; }"
        "QDoubleSpinBox { background:#2a2a5e; color:#ccccff; "
        "border:1px solid #555588; border-radius:3px; padding:2px 4px; }"
        "QPushButton { background:#2a2a5e; color:#ccccff; "
        "border:1px solid #555588; border-radius:3px; padding:3px 10px; }"
        "QPushButton:hover { background:#3a3a7e; }"
    )

    def __init__(self, lat_min, lat_max, lon_min, lon_max, keep_ratio=True, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Plan View Window Size")
        self.setFixedWidth(300)
        self.setStyleSheet(self._STYLE)

        layout = QVBoxLayout(self)
        layout.setSpacing(8)

        def _spin(val, lo=-500, hi=500):
            s = QDoubleSpinBox()
            s.setRange(lo, hi)
            s.setValue(val)
            s.setDecimals(1)
            s.setFixedWidth(90)
            return s

        def _row(label_txt, spin):
            row = QWidget()
            rl = QHBoxLayout(row)
            rl.setContentsMargins(0, 0, 0, 0)
            rl.addWidget(QLabel(label_txt))
            rl.addStretch()
            rl.addWidget(spin)
            layout.addWidget(row)
            return spin

        layout.addWidget(QLabel("Longitudinal limits (forward/backward):"))
        self._lon_min = _row("  Min [m]:", _spin(lon_min))
        self._lon_max = _row("  Max [m]:", _spin(lon_max))
        layout.addWidget(QLabel("Lateral limits (right/left):"))
        self._lat_min = _row("  Min [m]:", _spin(lat_min))
        self._lat_max = _row("  Max [m]:", _spin(lat_max))

        self._keep_ratio_cb = QCheckBox("Keep fixed ratio (1:1)")
        self._keep_ratio_cb.setChecked(keep_ratio)
        self._keep_ratio_cb.setStyleSheet("QCheckBox { color:#ccccff; }")
        layout.addWidget(self._keep_ratio_cb)

        # Buttons
        btn_box = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok |
            QDialogButtonBox.StandardButton.Cancel
        )
        btn_box.accepted.connect(self.accept)
        btn_box.rejected.connect(self.reject)
        layout.addWidget(btn_box)

    def _reset(self) -> None:
        self._lon_min.setValue(-50.0)
        self._lon_max.setValue(50.0)
        self._lat_min.setValue(-50.0)
        self._lat_max.setValue(50.0)

    def get_values(self) -> tuple:
        return (
            self._lat_min.value(),
            self._lat_max.value(),
            self._lon_min.value(),
            self._lon_max.value(),
            self._keep_ratio_cb.isChecked(),
        )
