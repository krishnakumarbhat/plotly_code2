#!/usr/bin/env python3
"""
fmu_viewer.py  —  LogicModel2 FMU Browser & API Tester
======================================================
Usage:
  python3 fmu_viewer.py [path/to/LogicModel2.fmu]
"""

import sys, os, zipfile, ctypes, tempfile, shutil, math, random

FMU_FILE = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), '..', 'Build', 'LogicModel2.fmu')

from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QPushButton, QTabWidget, QTableWidget, QTableWidgetItem,
    QGroupBox, QTextEdit, QHeaderView, QLineEdit, QStatusBar,
    QComboBox, QSplitter,
)
from PyQt5.QtCore import Qt, QThread, QTimer, pyqtSignal
from PyQt5.QtGui import QFont, QColor

# ─────────────────────────────────────────────────────────────────────────────
#  Background worker for fmpy validation (avoids freezing UI)
# ─────────────────────────────────────────────────────────────────────────────
class ValidateWorker(QThread):
    done = pyqtSignal(list)

    def __init__(self, fmu_path):
        super().__init__()
        self.fmu_path = fmu_path

    def run(self):
        try:
            from fmpy.validation import validate_fmu
            issues = validate_fmu(self.fmu_path)
        except Exception as e:
            issues = [f"ERROR: {e}"]
        self.done.emit(issues)

# ─────────────────────────────────────────────────────────────────────────────
#  Main Window
# ─────────────────────────────────────────────────────────────────────────────
class FMUViewer(QMainWindow):
    def __init__(self, fmu_path):
        super().__init__()
        self.fmu_path = fmu_path
        self.md       = None
        self._tmp_dir = None
        self._lib     = None
        self._comp    = None
        self.setWindowTitle(f"LogicModel2 FMU Viewer  —  {os.path.basename(fmu_path)}")
        self.setGeometry(100, 80, 1100, 780)
        self._load_model_description()
        self._build_ui()

    def _load_model_description(self):
        try:
            from fmpy.model_description import read_model_description
            self.md = read_model_description(self.fmu_path)
        except Exception as e:
            print(f"Failed to read model description: {e}")

    # ── UI Construction ───────────────────────────────────────────────────────
    def _build_ui(self):
        central = QWidget()
        self.setCentralWidget(central)
        root = QVBoxLayout(central)
        root.setSpacing(6)

        # Title bar
        title = QLabel("LogicModel2  —  CAN FD + Ethernet UDP  FMU Browser")
        title.setFont(QFont("Arial", 13, QFont.Bold))
        title.setAlignment(Qt.AlignCenter)
        title.setStyleSheet("color: #1a3a6b; padding: 4px;")
        root.addWidget(title)

        # Info strip
        info_row = QHBoxLayout()
        if self.md:
            md = self.md
            inputs  = [v for v in md.modelVariables if v.causality == 'input']
            outputs = [v for v in md.modelVariables if v.causality == 'output']
            sz = os.path.getsize(self.fmu_path)
            bits = [
                f"FMI {md.fmiVersion}",
                f"Model: {md.modelName}",
                f"Variables: {len(md.modelVariables):,}",
                f"Inputs: {len(inputs):,}",
                f"Outputs: {len(outputs):,}",
                f"Size: {sz/1024:.0f} KB",
            ]
            for b in bits:
                lbl = QLabel(b)
                lbl.setStyleSheet(
                    "background:#e8edf5; border-radius:4px; padding:3px 8px; "
                    "font-size:11px; color:#333;")
                info_row.addWidget(lbl)
        info_row.addStretch()
        root.addLayout(info_row)

        # Tabs
        tabs = QTabWidget()
        root.addWidget(tabs, 1)
        tabs.addTab(self._tab_overview(),        "📋  Overview")
        tabs.addTab(self._tab_can(),               "📡  CAN Signals")
        tabs.addTab(self._tab_eth(),               "🔌  Ethernet Frames")
        tabs.addTab(self._tab_api_test(),          "🧪  FMI API Test")
        tabs.addTab(self._tab_live_injection(),    "🔄  Live Injection")

        # Status bar
        self.status = QStatusBar()
        self.setStatusBar(self.status)
        self.status.showMessage(f"Loaded: {self.fmu_path}")

    # ── Tab 1: Overview ───────────────────────────────────────────────────────
    def _tab_overview(self):
        w = QWidget()
        layout = QVBoxLayout(w)

        grp = QGroupBox("FMU Metadata")
        grp.setFont(QFont("Arial", 10, QFont.Bold))
        gl = QVBoxLayout()

        tbl = QTableWidget(0, 2)
        tbl.setHorizontalHeaderLabels(["Property", "Value"])
        tbl.horizontalHeader().setStretchLastSection(True)
        tbl.verticalHeader().setVisible(False)
        tbl.setEditTriggers(QTableWidget.NoEditTriggers)
        tbl.setAlternatingRowColors(True)

        rows = []
        if self.md:
            md = self.md
            inputs  = [v for v in md.modelVariables if v.causality == 'input']
            outputs = [v for v in md.modelVariables if v.causality == 'output']
            can_in  = [v for v in inputs  if v.name.startswith('CAN_')]
            can_out = [v for v in outputs if v.name.startswith('CAN_')]
            eth_rx  = [v for v in inputs  if v.name.startswith('Eth_RX')]
            eth_tx  = [v for v in outputs if v.name.startswith('Eth_TX')]
            rows = [
                ("Model Name",         md.modelName),
                ("FMI Version",        md.fmiVersion),
                ("CoSim Identifier",   md.coSimulation.modelIdentifier),
                ("GUID",               md.guid),
                ("Description",        md.description or ""),
                ("Total Variables",    f"{len(md.modelVariables):,}"),
                ("Total Inputs",       f"{len(inputs):,}"),
                ("Total Outputs",      f"{len(outputs):,}"),
                ("── CAN RX Signals (Real inputs)",   f"{len(can_in)}"),
                ("── CAN TX Signals (Real outputs)",  f"{len(can_out)}"),
                ("── ETH RX Variables (Integer in)",  f"{len(eth_rx):,}  "
                    f"(1 frame_count + {len(eth_rx)//1504} frames × 1504)"),
                ("── ETH TX Variables (Integer out)", f"{len(eth_tx):,}  "
                    f"(1 frame_count + {len(eth_tx)//1504} frames × 1504)"),
                ("FMU File",           self.fmu_path),
                ("FMU Size",           f"{os.path.getsize(self.fmu_path)/1024:.1f} KB"),
            ]

        tbl.setRowCount(len(rows))
        for i, (k, v) in enumerate(rows):
            tbl.setItem(i, 0, QTableWidgetItem(k))
            tbl.setItem(i, 1, QTableWidgetItem(str(v)))
            if k.startswith("──"):
                for col in range(2):
                    tbl.item(i, col).setBackground(QColor("#dce8f5"))

        tbl.resizeColumnToContents(0)
        gl.addWidget(tbl)
        grp.setLayout(gl)
        layout.addWidget(grp)

        # Validate button + output
        btn_val = QPushButton("▶  Run FMI 2.0 Schema Validation")
        btn_val.setStyleSheet(
            "background:#1a6b3c; color:white; padding:6px 20px; "
            "border-radius:4px; font-weight:bold;")
        btn_val.clicked.connect(self._run_validation)
        self.val_output = QTextEdit()
        self.val_output.setReadOnly(True)
        self.val_output.setMaximumHeight(100)
        self.val_output.setFont(QFont("Courier New", 9))
        self.val_output.setPlaceholderText("Validation output will appear here…")
        layout.addWidget(btn_val)
        layout.addWidget(self.val_output)
        return w

    # ── Tab 2: CAN Signals ────────────────────────────────────────────────────
    def _tab_can(self):
        w = QWidget()
        layout = QVBoxLayout(w)

        search_row = QHBoxLayout()
        search_row.addWidget(QLabel("Filter:"))
        self.can_filter = QLineEdit()
        self.can_filter.setPlaceholderText("type to filter by signal name…")
        self.can_filter.textChanged.connect(self._filter_can)
        search_row.addWidget(self.can_filter)
        layout.addLayout(search_row)

        self.can_table = QTableWidget(0, 5)
        self.can_table.setHorizontalHeaderLabels(
            ["VR", "Name", "Type", "Causality", "Description"])
        self.can_table.horizontalHeader().setStretchLastSection(True)
        self.can_table.horizontalHeader().setSectionResizeMode(1, QHeaderView.Interactive)
        self.can_table.verticalHeader().setVisible(False)
        self.can_table.setEditTriggers(QTableWidget.NoEditTriggers)
        self.can_table.setAlternatingRowColors(True)
        self.can_table.setSelectionBehavior(QTableWidget.SelectRows)
        self.can_table.setSortingEnabled(True)
        layout.addWidget(self.can_table)

        self._can_rows = []
        if self.md:
            for v in [x for x in self.md.modelVariables if x.name.startswith('CAN_')]:
                type_str = "Real" if v.type == 'Real' else "Integer"
                self._can_rows.append(
                    (str(v.valueReference), v.name, type_str,
                     v.causality, v.description or ""))
            self._populate_can_table(self._can_rows)
        return w

    def _populate_can_table(self, rows):
        self.can_table.setRowCount(len(rows))
        for i, (vr, name, typ, caus, desc) in enumerate(rows):
            self.can_table.setItem(i, 0, QTableWidgetItem(vr))
            self.can_table.setItem(i, 1, QTableWidgetItem(name))
            self.can_table.setItem(i, 2, QTableWidgetItem(typ))
            item_c = QTableWidgetItem(caus)
            item_c.setForeground(
                QColor("#1a6b3c") if caus == 'output' else QColor("#1a3a6b"))
            self.can_table.setItem(i, 3, item_c)
            self.can_table.setItem(i, 4, QTableWidgetItem(desc))
        self.can_table.resizeColumnToContents(0)
        self.can_table.resizeColumnToContents(2)
        self.can_table.resizeColumnToContents(3)

    def _filter_can(self, text):
        self._populate_can_table(
            [r for r in self._can_rows if text.lower() in r[1].lower()])

    # ── Tab 3: Ethernet Interface ─────────────────────────────────────────────
    def _tab_eth(self):
        w = QWidget()
        layout = QVBoxLayout(w)

        grp_layout = QGroupBox("VR Address Map")
        ll = QVBoxLayout()
        addr_text = QTextEdit()
        addr_text.setReadOnly(True)
        addr_text.setFont(QFont("Courier New", 9))
        addr_text.setText(
            "ETH_FRAMES     = 10   (max UDP frames per fmi2DoStep)\n"
            "ETH_PAYLOAD    = 1500 (bytes per UDP frame, Ethernet MTU)\n"
            "ETH_VARS/FRAME = 1504 (4 metadata + 1500 data)\n"
            "\n"
            "─────────────────────────────────────────────────────────────────\n"
            "  Ethernet RX  (Integer INPUTS    VR 5000 … 20040)\n"
            "─────────────────────────────────────────────────────────────────\n"
            "  VR 5000                       Eth_RX_frame_count\n"
            "  VR 5001 + f×1504 + 0          Eth_RX_frame[f]_Status\n"
            "  VR 5001 + f×1504 + 1          Eth_RX_frame[f]_Received_Bytes\n"
            "  VR 5001 + f×1504 + 2          Eth_RX_frame[f]_Source_IP\n"
            "  VR 5001 + f×1504 + 3          Eth_RX_frame[f]_Source_Port\n"
            "  VR 5001 + f×1504 + 4..1503    Eth_RX_frame[f]_Data[0..1499]\n"
            "\n"
            "  frame 0: VR 5001–6504     frame 5: VR 12525–14024\n"
            "  frame 1: VR 6505–8008     frame 6: VR 14025–15528\n"
            "  frame 2: VR 8009–9512     frame 7: VR 15529–17032\n"
            "  frame 3: VR 9513–11016    frame 8: VR 17033–18536\n"
            "  frame 4: VR 11017–12524   frame 9: VR 18537–20040\n"
            "\n"
            "─────────────────────────────────────────────────────────────────\n"
            "  Ethernet TX  (Integer OUTPUTS   VR 21000 … 36040)\n"
            "─────────────────────────────────────────────────────────────────\n"
            "  VR 21000                      Eth_TX_frame_count\n"
            "  VR 21001 + f×1504 + 0         Eth_TX_frame[f]_Status\n"
            "  VR 21001 + f×1504 + 1         Eth_TX_frame[f]_Transmit_Bytes\n"
            "  VR 21001 + f×1504 + 2         Eth_TX_frame[f]_Dest_IP\n"
            "  VR 21001 + f×1504 + 3         Eth_TX_frame[f]_Dest_Port\n"
            "  VR 21001 + f×1504 + 4..1503   Eth_TX_frame[f]_Data[0..1499]\n"
            "\n"
            "─────────────────────────────────────────────────────────────────\n"
            "  CAN FD                        VR 1000 … ~1539\n"
            "  ├─ RX signals (input)         VR 1000 … 1205  (206 Real vars)\n"
            "  └─ TX signals (output)        VR 1206 … ~1539 (333 Real vars)\n"
            "─────────────────────────────────────────────────────────────────\n"
        )
        ll.addWidget(addr_text)
        grp_layout.setLayout(ll)
        layout.addWidget(grp_layout)

        grp_frame = QGroupBox("Frame [0] Variable Preview — RX")
        fl = QVBoxLayout()
        frame_tbl = QTableWidget(0, 3)
        frame_tbl.setHorizontalHeaderLabels(["VR", "Name", "Causality"])
        frame_tbl.verticalHeader().setVisible(False)
        frame_tbl.setEditTriggers(QTableWidget.NoEditTriggers)
        frame_tbl.setAlternatingRowColors(True)
        frame_tbl.horizontalHeader().setStretchLastSection(True)

        if self.md:
            eth_vars = [v for v in self.md.modelVariables
                        if v.name.startswith('Eth_RX_frame[0]')]
            preview = eth_vars[:6] + [None] + eth_vars[-3:]
            frame_tbl.setRowCount(len(preview))
            for i, v in enumerate(preview):
                if v is None:
                    for col in range(3):
                        item = QTableWidgetItem("  ···  (1490 Data[6..1496] vars omitted)  ···")
                        item.setForeground(QColor("#888"))
                        frame_tbl.setItem(i, col, item)
                else:
                    frame_tbl.setItem(i, 0, QTableWidgetItem(str(v.valueReference)))
                    frame_tbl.setItem(i, 1, QTableWidgetItem(v.name))
                    frame_tbl.setItem(i, 2, QTableWidgetItem(v.causality))
        fl.addWidget(frame_tbl)
        grp_frame.setLayout(fl)
        layout.addWidget(grp_frame)
        return w

    # ── Tab 4: FMI API Test ───────────────────────────────────────────────────
    def _tab_api_test(self):
        w = QWidget()
        layout = QVBoxLayout(w)

        top_row = QHBoxLayout()
        btns = [
            ("① Instantiate + Init", "#1a3a6b", self._api_instantiate),
            ("② Set ETH RX Data",    "#6b3a1a", self._api_set_data),
            ("③ fmi2DoStep",         "#3a1a6b", self._api_step),
            ("④ Get ETH TX + CAN TX","#1a6b3c", self._api_get),
            ("⑤ FreeInstance",       "#6b1a1a", self._api_free),
        ]
        for label, color, slot in btns:
            btn = QPushButton(label)
            btn.setStyleSheet(
                f"background:{color}; color:white; padding:6px 14px; "
                f"border-radius:4px; font-weight:bold;")
            btn.clicked.connect(slot)
            top_row.addWidget(btn)
        layout.addLayout(top_row)

        self.api_log = QTextEdit()
        self.api_log.setReadOnly(True)
        self.api_log.setFont(QFont("Courier New", 9))
        self.api_log.setStyleSheet("background:#1e1e1e; color:#d4d4d4;")
        layout.addWidget(self.api_log, 1)
        return w

    # ── Validation ────────────────────────────────────────────────────────────
    def _run_validation(self):
        self.val_output.setPlainText("Running FMI 2.0 schema validation…")
        self._val_worker = ValidateWorker(self.fmu_path)
        self._val_worker.done.connect(self._on_validation_done)
        self._val_worker.start()
        self.status.showMessage("Validating…")

    def _on_validation_done(self, issues):
        if not issues:
            self.val_output.setPlainText("✅  No schema errors or warnings — FMU is valid.")
            self.val_output.setStyleSheet("background:#e8f5e9;")
        else:
            errors   = [i for i in issues if 'error'   in str(i).lower()]
            warnings = [i for i in issues if 'warning' in str(i).lower()]
            lines = [f"❌  {e}" for e in errors] + [f"⚠   {w}" for w in warnings]
            self.val_output.setPlainText("\n".join(lines))
            self.val_output.setStyleSheet(
                f"background:{'#fce4e4' if errors else '#fff8e1'};")
        self.status.showMessage(
            f"Validation complete — {len(issues)} issue(s) found")

    # ── FMI API helpers ───────────────────────────────────────────────────────
    def _alog(self, msg, ok=True):
        color = "#4ec9b0" if ok else "#f44747"
        icon  = "✓" if ok else "✗"
        self.api_log.append(f'<span style="color:{color};">{icon}  {msg}</span>')

    def _load_so(self):
        if self._lib:
            return self._lib
        self._tmp_dir = tempfile.mkdtemp(prefix="fmu_viewer_")
        with zipfile.ZipFile(self.fmu_path, 'r') as zf:
            zf.extractall(self._tmp_dir)
        so = os.path.join(self._tmp_dir, 'binaries', 'linux64', 'LogicModel2.so')
        self._lib = ctypes.cdll.LoadLibrary(so)
        return self._lib

    def _api_instantiate(self):
        self.api_log.clear()
        try:
            lib = self._load_so()
            self._alog("dlopen  binaries/linux64/LogicModel2.so")

            lib.fmi2Instantiate.restype = ctypes.c_void_p
            self._comp = lib.fmi2Instantiate(
                b"LogicModel2_Viewer", ctypes.c_int(1), b"test-guid", b"",
                ctypes.c_void_p(0), ctypes.c_int(0), ctypes.c_int(0))
            self._alog(f"fmi2Instantiate  →  0x{self._comp:x}", bool(self._comp))

            lib.fmi2SetupExperiment.restype  = ctypes.c_int
            lib.fmi2SetupExperiment.argtypes = [
                ctypes.c_void_p, ctypes.c_int, ctypes.c_double,
                ctypes.c_double, ctypes.c_int, ctypes.c_double,
            ]
            st = lib.fmi2SetupExperiment(self._comp, 0, 0.0, 0.0, 1, 1.0)
            self._alog(f"fmi2SetupExperiment  →  status={st}", st == 0)

            lib.fmi2EnterInitializationMode.restype = ctypes.c_int
            st = lib.fmi2EnterInitializationMode(self._comp)
            self._alog(f"fmi2EnterInitializationMode  →  status={st}", st == 0)

            lib.fmi2ExitInitializationMode.restype = ctypes.c_int
            st = lib.fmi2ExitInitializationMode(self._comp)
            self._alog(f"fmi2ExitInitializationMode  →  status={st}", st == 0)
            self.status.showMessage("FMU instantiated and initialized")
        except Exception as e:
            self._alog(f"ERROR: {e}", False)

    def _api_set_data(self):
        if not self._comp:
            self._alog("ERROR: Call ① Instantiate first", False); return
        try:
            lib = self._lib
            lib.fmi2SetInteger.restype = ctypes.c_int
            lib.fmi2SetReal.restype    = ctypes.c_int

            # ETH RX: pass size=0 (no frame this step) — lo/hi are irrelevant.
            # Passing a garbage pointer here causes a SEGFAULT in the smoke test.
            eth_sets = [
                (5000, 0,    "EthRxIn.lo   = 0 (no frame)"),
                (5001, 0,    "EthRxIn.hi   = 0 (no frame)"),
                (5002, 0,    "EthRxIn.size = 0 (no frame)"),
            ]
            for vr_v, val_v, label in eth_sets:
                vr = (ctypes.c_uint * 1)(vr_v); v = (ctypes.c_int * 1)(val_v)
                st = lib.fmi2SetInteger(self._comp, vr, 1, v)
                self._alog(f"fmi2SetInteger  {label:40s}  (VR {vr_v})  status={st}", st == 0)

            # CAN RX: write a few real input signals (Integer + Real)
            can_int_sets = [
                (1000, 0xAB, "VCAN_RX.FD15_ZCU_F_DATA_9.E2E_CRC"),
                (1164, 1,    "VCAN_RX.ZCU_CL_GLOBAL.VEHICLE_IGNITION_STATUS"),
            ]
            for vr_v, val_v, label in can_int_sets:
                vr = (ctypes.c_uint * 1)(vr_v); v = (ctypes.c_int * 1)(val_v)
                st = lib.fmi2SetInteger(self._comp, vr, 1, v)
                self._alog(f"fmi2SetInteger  {label:40s}  (VR {vr_v})  status={st}", st == 0)

            can_real_sets = [
                (1040, 14.0,  "VCAN_RX.FD15_BSM_DATA_4.VEHICLE_LONG_SPEED  [m/s]"),
                (1034, 0.05,  "VCAN_RX.FD15_BSM_DATA_4.YAW_VELOCITY_FILTERED [deg/s]"),
            ]
            for vr_v, val_v, label in can_real_sets:
                vr = (ctypes.c_uint * 1)(vr_v); v = (ctypes.c_double * 1)(val_v)
                st = lib.fmi2SetReal(self._comp, vr, 1, v)
                self._alog(f"fmi2SetReal     {label:40s}  (VR {vr_v})  status={st}", st == 0)

            self.status.showMessage("Input data written")
        except Exception as e:
            self._alog(f"ERROR: {e}", False)

    def _api_step(self):
        if not self._comp:
            self._alog("ERROR: Call ① Instantiate first", False); return
        try:
            lib = self._lib
            lib.fmi2DoStep.restype  = ctypes.c_int
            lib.fmi2DoStep.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_int]
            st = lib.fmi2DoStep(self._comp, 0.0, 0.01, 1)
            self._alog(
                f"fmi2DoStep  t=0.00  h=0.01  status={st}  "
                f"({'OK — decoder DLL hookup pending' if st==0 else 'FAILED'})",
                st == 0)
            self.status.showMessage("DoStep complete")
        except Exception as e:
            self._alog(f"ERROR: {e}", False)

    def _api_get(self):
        if not self._comp:
            self._alog("ERROR: Call ① Instantiate first", False); return
        try:
            lib = self._lib
            lib.fmi2GetInteger.restype = ctypes.c_int
            lib.fmi2GetReal.restype    = ctypes.c_int

            # ETH TX pointer-triplet (VR 5003-5005)
            for vr_v, name in [
                (5003, "EthTxOut.lo"),
                (5004, "EthTxOut.hi"),
                (5005, "EthTxOut.size"),
            ]:
                vr  = (ctypes.c_uint * 1)(vr_v)
                out = (ctypes.c_int  * 1)(0)
                st  = lib.fmi2GetInteger(self._comp, vr, 1, out)
                self._alog(
                    f"fmi2GetInteger  {name:40s}  (VR {vr_v})  = {out[0]}  status={st}",
                    st == 0)

            # CAN TX: spot-check a few Integer output signals
            for vr_v, name in [
                (1205, "VCAN_TX.FD15_LRCF_DATA_7.VHL_1_CL_SUB_TYPE"),
                (1250, "VCAN_TX.FD15_LRCF_DATA_2.E2E_ALIVE_LRCF_DATA_2"),
                (1314, "VCAN_TX.FD15_LRCF_DATA_9.E2E_ALIVE_LRCF_DATA_9"),
            ]:
                vr  = (ctypes.c_uint * 1)(vr_v)
                out = (ctypes.c_int  * 1)(0)
                st  = lib.fmi2GetInteger(self._comp, vr, 1, out)
                self._alog(
                    f"fmi2GetInteger  {name:55s}  (VR {vr_v})  = {out[0]}  status={st}",
                    st == 0)

            # CAN TX: spot-check a few Real output signals
            for vr_v, name in [
                (1254, "VCAN_TX.FD15_LRCF_DATA_2.ADAS_LONGI_ACCEL_REQUEST"),
                (1257, "VCAN_TX.FD15_LRCF_DATA_2.YAW_RATE_REQUEST_SETPOINT"),
                (1315, "VCAN_TX.FD15_LRCF_DATA_9.ACC_FRONT_TARGET_DISTANCE"),
                (1265, "VCAN_TX.FD15_LRCF_DATA_2.ACC_POTENTIAL_ACCELERATION_REQ"),
            ]:
                vr  = (ctypes.c_uint  * 1)(vr_v)
                out = (ctypes.c_double * 1)(0.0)
                st  = lib.fmi2GetReal(self._comp, vr, 1, out)
                self._alog(
                    f"fmi2GetReal     {name:55s}  (VR {vr_v})  = {out[0]:.4f}  status={st}",
                    st == 0)
            self.status.showMessage("Get complete")
        except Exception as e:
            self._alog(f"ERROR: {e}", False)

    def _api_free(self):
        if not self._comp:
            self._alog("Nothing to free", False); return
        try:
            self._lib.fmi2FreeInstance.restype = None
            self._lib.fmi2FreeInstance(self._comp)
            self._comp = None
            self._alog("fmi2FreeInstance  →  OK")
            if self._tmp_dir:
                shutil.rmtree(self._tmp_dir, ignore_errors=True)
                self._tmp_dir = None
                self._lib     = None
            self.status.showMessage("FMU instance freed")
        except Exception as e:
            self._alog(f"ERROR: {e}", False)

    # ── Tab 5: Live Signal Injection ─────────────────────────────────────────
    def _tab_live_injection(self):
        w = QWidget()
        layout = QVBoxLayout(w)

        # Control bar
        ctrl = QHBoxLayout()
        ctrl.addWidget(QLabel("Pattern:"))
        self.inj_pattern = QComboBox()
        self.inj_pattern.addItems(["Counter", "Sine Wave", "Random", "0xDEADBEEF"])
        ctrl.addWidget(self.inj_pattern)
        ctrl.addWidget(QLabel("  Interval:"))
        self.inj_interval = QComboBox()
        self.inj_interval.addItems(["50 ms", "100 ms", "200 ms", "500 ms", "1000 ms"])
        self.inj_interval.setCurrentIndex(1)
        ctrl.addWidget(self.inj_interval)
        self.inj_step_lbl = QLabel("  Step: 0  ")
        self.inj_step_lbl.setStyleSheet(
            "font-weight:bold; font-size:12px; color:#1a3a6b; padding:0 10px;")
        ctrl.addWidget(self.inj_step_lbl)
        self.inj_start_btn = QPushButton("▶  Start")
        self.inj_start_btn.setStyleSheet(
            "background:#1a6b3c; color:white; padding:6px 18px; "
            "border-radius:4px; font-weight:bold;")
        self.inj_start_btn.clicked.connect(self._live_start)
        self.inj_stop_btn = QPushButton("■  Stop")
        self.inj_stop_btn.setStyleSheet(
            "background:#6b1a1a; color:white; padding:6px 18px; "
            "border-radius:4px; font-weight:bold;")
        self.inj_stop_btn.clicked.connect(self._live_stop)
        self.inj_stop_btn.setEnabled(False)
        ctrl.addWidget(self.inj_start_btn)
        ctrl.addWidget(self.inj_stop_btn)
        ctrl.addStretch()
        layout.addLayout(ctrl)

        # Side-by-side RX / TX tables
        splitter = QSplitter(Qt.Horizontal)

        rx_grp = QGroupBox("📥  RX Inputs  (injecting into FMU)")
        rx_grp.setFont(QFont("Arial", 9, QFont.Bold))
        rx_l = QVBoxLayout()
        self.inj_rx_table = QTableWidget(0, 2)
        self.inj_rx_table.setHorizontalHeaderLabels(["Signal", "Value"])
        self.inj_rx_table.horizontalHeader().setStretchLastSection(True)
        self.inj_rx_table.verticalHeader().setVisible(False)
        self.inj_rx_table.setEditTriggers(QTableWidget.NoEditTriggers)
        self.inj_rx_table.setAlternatingRowColors(True)
        self.inj_rx_table.setFont(QFont("Courier New", 9))
        rx_l.addWidget(self.inj_rx_table)
        rx_grp.setLayout(rx_l)

        tx_grp = QGroupBox("📤  TX Outputs  (read back from FMU)")
        tx_grp.setFont(QFont("Arial", 9, QFont.Bold))
        tx_l = QVBoxLayout()
        self.inj_tx_table = QTableWidget(0, 2)
        self.inj_tx_table.setHorizontalHeaderLabels(["Signal", "Value"])
        self.inj_tx_table.horizontalHeader().setStretchLastSection(True)
        self.inj_tx_table.verticalHeader().setVisible(False)
        self.inj_tx_table.setEditTriggers(QTableWidget.NoEditTriggers)
        self.inj_tx_table.setAlternatingRowColors(True)
        self.inj_tx_table.setFont(QFont("Courier New", 9))
        tx_l.addWidget(self.inj_tx_table)
        tx_grp.setLayout(tx_l)

        splitter.addWidget(rx_grp)
        splitter.addWidget(tx_grp)
        layout.addWidget(splitter, 1)

        # Mini console
        self.inj_log = QTextEdit()
        self.inj_log.setReadOnly(True)
        self.inj_log.setFont(QFont("Courier New", 8))
        self.inj_log.setStyleSheet("background:#1e1e1e; color:#d4d4d4;")
        self.inj_log.setMaximumHeight(110)
        layout.addWidget(self.inj_log)

        self._live_step = 0
        self._live_timer = QTimer()
        self._live_timer.timeout.connect(self._live_tick)
        self._init_live_tables()
        return w

    def _init_live_tables(self):
        """Populate fixed row labels for both RX and TX panels."""
        # ── Hardcoded to LogicModel2_step() implementation ────────────────────
        # CAN RX Real inputs (fmi2SetReal): include the two passthrough sources
        self._live_can_rx = [
            (1033, "LONG_ACCEL_CORR"),   # → LONGI_ACCEL_REQUEST (passthrough)
            (1034, "YAW_VEL_FILTERED"),  # → YAW_RATE_SETPOINT   (passthrough)
            (1017, "YAW_RATE"),
            (1018, "LAT_ACCELERATION"),
            (1019, "LONG_ACCELERATION"),
        ]
        # CAN RX Integer inputs (fmi2SetInteger): alive counters & status flags
        self._live_can_rx_int = [
            (1000, "E2E_CRC_ZCU_F9"),
            (1002, "E2E_ALIVE_ZCU_F9"),
            (1012, "ACC_STATE_PWT"),
            (1013, "PRND_ENGAGED"),
            (1164, "IGNITION_STATUS"),
        ]
        # CAN TX Integer outputs (fmi2GetInteger): algo-internal rolling counters
        self._live_can_tx_int = [
            (1250, "ALIVE_DATA_2"),   # E2E 4-bit rolling counter 0→15
            (1314, "ALIVE_DATA_9"),
        ]
        # CAN TX Real outputs (fmi2GetReal): signals actually written by the algo
        self._live_can_tx = [
            (1254, "LONGI_ACCEL_REQ"),   # passthrough from LONG_ACCEL_CORR
            (1257, "YAW_RATE_SETPOINT"), # passthrough from YAW_VEL_FILTERED
            (1265, "POT_ACCEL_REQ"),     # sine ±5 m/s²
            (1315, "ACC_TARGET_DIST"),   # sawtooth 0→199 m
        ]

        def _make_table(table, eth_header_color, section_label_color, can_names, initial_dash):
            rows = (
                [("─── ETH (pointer-triplet) ───", None)] +
                [("EthRxIn.lo  / EthTxOut.lo",  initial_dash),
                 ("EthRxIn.hi  / EthTxOut.hi",  initial_dash),
                 ("EthRxIn.size / EthTxOut.size", initial_dash),
                 ("(unused)",  "—"),
                 ("(unused)",  "—"),
                 ("(unused)",  "—"),
                 ("(unused)",  "—")] +
                [("─── CAN ───", None)] +
                [(nm, initial_dash) for nm in can_names]
            )
            table.setRowCount(len(rows))
            for i, (name, val) in enumerate(rows):
                item_n = QTableWidgetItem(name)
                item_v = QTableWidgetItem(val or "")
                if val is None:   # section header row
                    for item in (item_n, item_v):
                        item.setBackground(QColor(eth_header_color))
                        item.setForeground(QColor(section_label_color))
                        f = item.font(); f.setBold(True); item.setFont(f)
                table.setItem(i, 0, item_n)
                table.setItem(i, 1, item_v)
            table.resizeColumnToContents(0)

        rx_can_names = [nm for _, nm in self._live_can_rx]
        tx_can_names = ([nm for _, nm in self._live_can_tx_int] +
                        [nm for _, nm in self._live_can_tx])
        _make_table(self.inj_rx_table, "#c0d8f0", "#1a3a6b", rx_can_names, "0")
        _make_table(self.inj_tx_table, "#c8e6c9", "#1a6b3c", tx_can_names, "—")
        # Row index references (skip section-header rows at indices 0 and 8)
        self._rx_eth_rows      = list(range(1, 8))
        self._rx_can_rows      = list(range(9, 9 + len(self._live_can_rx)))
        self._tx_eth_rows      = list(range(1, 8))
        _n_int  = len(self._live_can_tx_int)
        _n_real = len(self._live_can_tx)
        self._tx_can_int_rows  = list(range(9, 9 + _n_int))
        self._tx_can_real_rows = list(range(9 + _n_int, 9 + _n_int + _n_real))

    def _live_start(self):
        """Auto-instantiate if needed, then start the injection timer."""
        if not self._comp:
            try:
                lib = self._load_so()
                lib.fmi2Instantiate.restype = ctypes.c_void_p
                self._comp = lib.fmi2Instantiate(
                    b"LogicModel2_Live", ctypes.c_int(1), b"test-guid", b"",
                    ctypes.c_void_p(0), ctypes.c_int(0), ctypes.c_int(0))
                lib.fmi2SetupExperiment.restype  = ctypes.c_int
                lib.fmi2SetupExperiment.argtypes = [
                    ctypes.c_void_p, ctypes.c_int, ctypes.c_double,
                    ctypes.c_double, ctypes.c_int, ctypes.c_double]
                lib.fmi2SetupExperiment(self._comp, 0, 0.0, 0.0, 1, 3600.0)
                lib.fmi2EnterInitializationMode.restype = ctypes.c_int
                lib.fmi2EnterInitializationMode(self._comp)
                lib.fmi2ExitInitializationMode.restype  = ctypes.c_int
                lib.fmi2ExitInitializationMode(self._comp)
                self.inj_log.append(
                    '<span style="color:#4ec9b0;">✓  FMU auto-instantiated and initialized</span>')
            except Exception as e:
                self.inj_log.append(
                    f'<span style="color:#f44747;">✗  Instantiate failed: {e}</span>')
                return

        interval_ms = int(self.inj_interval.currentText().split()[0])
        self._live_step = 0
        self._live_timer.setInterval(interval_ms)
        self._live_timer.start()
        self.inj_start_btn.setEnabled(False)
        self.inj_stop_btn.setEnabled(True)
        self.inj_pattern.setEnabled(False)
        self.inj_interval.setEnabled(False)
        self.status.showMessage(f"Live injection running  [{interval_ms} ms / step]")
        self.inj_log.append(
            f'<span style="color:#dcdcaa;">▶  Started — pattern={self.inj_pattern.currentText()}  '
            f'interval={interval_ms} ms</span>')

    def _live_stop(self):
        self._live_timer.stop()
        self.inj_start_btn.setEnabled(True)
        self.inj_stop_btn.setEnabled(False)
        self.inj_pattern.setEnabled(True)
        self.inj_interval.setEnabled(True)
        self.status.showMessage(f"Live injection stopped at step {self._live_step}")
        self.inj_log.append(
            f'<span style="color:#dcdcaa;">■  Stopped at step {self._live_step}</span>')

    def _live_tick(self):
        if not self._comp:
            self._live_stop(); return
        lib  = self._lib
        step = self._live_step
        pat  = self.inj_pattern.currentText()
        t    = step * 0.1   # simulated time (seconds)

        # ── Compute RX byte pattern for ETH ──────────────────────────────────
        if pat == "Counter":
            b = [(step * 4 + i) & 0xFF for i in range(4)]
            frame_count   = (step % 10) + 1
            recv_bytes    = 1200
        elif pat == "Sine Wave":
            b = [int(127.5 + 127.5 * math.sin(t + i * math.pi / 2)) for i in range(4)]
            frame_count   = max(1, int(5 + 4 * math.sin(t * 0.3)))
            recv_bytes    = int(600 + 600 * abs(math.sin(t * 0.2)))
        elif pat == "Random":
            b = [random.randint(0, 255) for _ in range(4)]
            frame_count   = random.randint(1, 10)
            recv_bytes    = random.randint(64, 1500)
        else:  # 0xDEADBEEF
            b = [0xDE, 0xAD, 0xBE, 0xEF]
            frame_count   = 4
            recv_bytes    = 1492

        # ── Compute RX CAN values ─────────────────────────────────────────────
        if pat == "Sine Wave":
            can_rx_vals = [math.sin(t + i * 0.7) * (10 + i * 5) for i in range(5)]
        elif pat == "Random":
            can_rx_vals = [random.uniform(-100, 100) for _ in range(5)]
        elif pat == "Counter":
            can_rx_vals = [float(step + i) * 0.1 for i in range(5)]
        else:
            can_rx_vals = [42.0 + i * 0.5 for i in range(5)]

        try:
            lib.fmi2SetInteger.restype = ctypes.c_int
            lib.fmi2SetReal.restype    = ctypes.c_int

            # Inject ETH RX: pass size=0 (no frame this step).
            # Setting a non-zero pointer from Python is unsafe — size=0 bypasses
            # the loopback in the smoke test and prevents a SEGFAULT.
            for vr_v, val_v in [
                (5000, 0),   # EthRxIn.lo  = 0
                (5001, 0),   # EthRxIn.hi  = 0
                (5002, 0),   # EthRxIn.size = 0  ← skips memcpy in smoke test
            ]:
                vr  = (ctypes.c_uint * 1)(vr_v)
                val = (ctypes.c_int  * 1)(val_v)
                lib.fmi2SetInteger(self._comp, vr, 1, val)

            # Inject CAN RX Integer inputs (alive counters, status flags)
            for (vr_v, _), val_v in zip(self._live_can_rx_int, [step % 16, step % 256, 1, 1, 1]):
                vr  = (ctypes.c_uint * 1)(vr_v)
                val = (ctypes.c_int  * 1)(val_v)
                lib.fmi2SetInteger(self._comp, vr, 1, val)

            # Inject CAN RX Real inputs
            for (vr_v, _), val_v in zip(self._live_can_rx, can_rx_vals):
                vr  = (ctypes.c_uint   * 1)(vr_v)
                val = (ctypes.c_double * 1)(val_v)
                lib.fmi2SetReal(self._comp, vr, 1, val)

            # Step
            lib.fmi2DoStep.restype  = ctypes.c_int
            lib.fmi2DoStep.argtypes = [
                ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_int]
            lib.fmi2DoStep(self._comp, t, 0.1, 1)

            # Read ETH TX pointer-triplet (VR 5003-5005)
            lib.fmi2GetInteger.restype = ctypes.c_int
            tx_eth = []
            for vr_v in [5003, 5004, 5005]:
                vr  = (ctypes.c_uint * 1)(vr_v)
                out = (ctypes.c_int  * 1)(0)
                lib.fmi2GetInteger(self._comp, vr, 1, out)
                tx_eth.append(out[0])
            tx_eth += [0, 0, 0, 0]   # pad to 7 so the table display loop works

            # Read CAN TX Integer (E2E alive counters — increment every step)
            lib.fmi2GetInteger.restype = ctypes.c_int
            tx_int_vals = []
            for vr_v, _ in self._live_can_tx_int:
                vr  = (ctypes.c_uint * 1)(vr_v)
                out = (ctypes.c_int  * 1)(0)
                lib.fmi2GetInteger(self._comp, vr, 1, out)
                tx_int_vals.append(out[0])

            # Read CAN TX Real (algorithmic outputs)
            lib.fmi2GetReal.restype = ctypes.c_int
            tx_can = []
            for vr_v, _ in self._live_can_tx:
                vr  = (ctypes.c_uint   * 1)(vr_v)
                out = (ctypes.c_double * 1)(0.0)
                lib.fmi2GetReal(self._comp, vr, 1, out)
                tx_can.append(out[0])

        except Exception as e:
            self.inj_log.append(
                f'<span style="color:#f44747;">✗  step {step}: {e}</span>')
            self._live_stop(); return

        # ── Update RX table (yellow flash on change) ──────────────────────────
        rx_vals = [
            "0", "0", "0",   # lo=0, hi=0, size=0 (no frame injected via pointer)
            "—", "—", "—", "—",
        ]
        for row, val in zip(self._rx_eth_rows, rx_vals):
            item = self.inj_rx_table.item(row, 1)
            if item:
                item.setText(val)
                item.setBackground(QColor("#fffde7"))
        for row, val in zip(self._rx_can_rows, can_rx_vals):
            item = self.inj_rx_table.item(row, 1)
            if item:
                item.setText(f"{val:+.4f}")
                item.setBackground(QColor("#fffde7"))

        # ── Update TX table (green on readback) ───────────────────────────────
        tx_eth_strs = [
            str(tx_eth[0]), str(tx_eth[1]), str(tx_eth[2]),  # lo, hi, size
            "—", "—", "—", "—",
        ]
        for row, val in zip(self._tx_eth_rows, tx_eth_strs):
            item = self.inj_tx_table.item(row, 1)
            if item:
                item.setText(val)
                item.setBackground(QColor("#e8f5e9"))
        for row, val in zip(self._tx_can_int_rows, tx_int_vals):
            item = self.inj_tx_table.item(row, 1)
            if item:
                item.setText(str(val))
                item.setBackground(QColor("#e8f5e9"))
        for row, val in zip(self._tx_can_real_rows, tx_can):
            item = self.inj_tx_table.item(row, 1)
            if item:
                item.setText(f"{val:+.4f}")
                item.setBackground(QColor("#e8f5e9"))

        self._live_step += 1
        self.inj_step_lbl.setText(f"  Step: {self._live_step}  ")

        # Log every 10 steps
        if step % 10 == 0:
            can_rx_str  = f"{can_rx_vals[0]:+.3f}" if can_rx_vals else "n/a"
            alive_str   = str(tx_int_vals[0])       if tx_int_vals else "n/a"
            pot_accel   = f"{tx_can[2]:+.3f}"      if len(tx_can) > 2 else "n/a"
            acc_target  = f"{tx_can[3]:.1f}"        if len(tx_can) > 3 else "n/a"
            self.inj_log.append(
                f'<span style="color:#9cdcfe;">'
                f'step {step:04d}  '
                f'CAN_RX LONG_ACCEL={can_rx_str}  '
                f'ALIVE={alive_str}  POT_ACCEL={pot_accel}  ACC_TARGET={acc_target}'
                f'</span>')
            # Keep log short
            if self.inj_log.document().blockCount() > 60:
                cursor = self.inj_log.textCursor()
                cursor.movePosition(cursor.Start)
                cursor.select(cursor.LineUnderCursor)
                cursor.removeSelectedText()
                cursor.deleteChar()

    def closeEvent(self, event):
        if self._comp and self._lib:
            try:
                self._lib.fmi2FreeInstance.restype = None
                self._lib.fmi2FreeInstance(self._comp)
            except Exception:
                pass
        if self._tmp_dir:
            shutil.rmtree(self._tmp_dir, ignore_errors=True)
        event.accept()


def main():
    if not os.path.exists(FMU_FILE):
        print(f"ERROR: {FMU_FILE} not found"); sys.exit(1)
    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    viewer = FMUViewer(FMU_FILE)
    viewer.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
