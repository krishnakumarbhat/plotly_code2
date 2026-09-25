# TRATON Radar ReSim KPI – Plan View & Signal Explorer

A desktop application for visualising and analysing MDF4 radar log files produced by the **APTIV SRR6** short-range radar sensors mounted on TRATON vehicles.  
Built with Python 3, PyQt6 and Matplotlib.

---

## Features

### Plan View
- Top-down (bird's-eye) view of detected radar objects around the host vehicle.
- Objects rendered as oriented bounding boxes colour-coded by:
  - **Dynamic Property** (Moving / Stopped / Stationary / Invalid …), or
  - **Object Class** (Car, Truck, Motorbike, Bicycle, Pedestrian, …) — switchable via Settings.
- Optional overlays: velocity vector, heading arrow, tracking-ID labels.
- Host-vehicle vector rendered at the origin (yellow arrow, km/h → m/s conversion, handles reverse driving).
- Configurable field-of-view window (Lat ±50 m, Lon −80 … +20 m default).
- Rubber-band zoom, scroll-wheel zoom, keep-aspect-ratio mode.
- **Single-click** a Moving, Stopped, or Stationary object → select it and jump to the Object Track tab.
- **Double-click** a Moving, Stopped, or Stationary object → detailed object info pop-up.
- Click on empty space → deselects the current track and removes the highlight.
- Selected object highlighted with a yellow dashed bounding box.
- Side filter: Both / Left (SRRL) / Right (SRRR).

### Signal Plot
- Time-series plot of any decoded CAN signal from the loaded file.
- Select signals from the tree on the left; click **Plot Selected Signals**.
- Multi-signal overlay with auto-coloured lines and a shared legend.
- Moving cursor synced to the current navigation frame.
- Value table below the plot shows interpolated signal values at cursor position.
- X-axis modes: CAN timestamp, scan index (left/right), object timestamp (left/right).
- Dark / light theme support.

### Object Track
- Displays the full lifetime statistics of a single radar object track.
- Tracks are built automatically from **Moving** (DynProperty 3), **Stopped** (2), and **Stationary** (1) objects.
- Gap tolerance: **4 consecutive missing scan indices** (≈ 200 ms at 20 Hz) — based on ScanIndex, not timestamps.
- Tracks are identified by a stable UUID; **ID drops** (any skipped scan index within a track) are reported.
- **Stationary-only** tracks (never had Moving or Stopped frames) are used internally for KPI matching but are hidden from all UI lists.
- Per-track header: Side, Tracking ID, scan range, duration, frame count, ID drop list.
- Per-signal statistics table:
  - **Continuous signals** (positions, velocities, probabilities …): Current / Min / Max / Avg / Median
  - **Enum signals** (ClassMostProb, ClassSecMostProb, DynamicProperty, MaintenanceState, Occlusion):  
    Current / Initial State / Last State / Median — raw integers decoded to human-readable DBC strings.
- Signal ordering: TrackingId first → continuous signals → enum signals.
- Select one or more signal rows → **Plot Signals** → sends them directly to the Signal Plot tab as time series.
- Tab updates the **Current** column live as you navigate frames.

### Navigation
- Frame-by-frame navigation with slider, prev/next/first/last buttons, keyboard shortcuts (A / D / ← / → / Space).
- Playback mode with configurable frame interval (10 – 5000 ms).
- Two-row navigation bar: buttons + full-width slider on top; side selector, interval, and info labels below.

### Settings & Theming
- **Dark** (default) and **Light** themes applied globally to all panels and matplotlib canvases.
- **Plan View Settings** dialog: per-DynProperty and per-Class colours, line widths, and alphas; velocity vector colour; ID label colour; Reset to defaults button.
- Class-colour mode (default ON) or DynProperty-colour mode selectable per session.
- All settings stored in-memory; reset any time via the dialog.

### Detachable Tabs
- Any tab (Plan View, Signal Plot, Object Track) can be **detached** into a floating window:
  - Right-click a tab → *Detach to window*
  - Double-click a tab
  - Drag the tab outside the tab-widget boundary
- Close the floating window to re-dock the tab at its original position.

---

## Supported File Formats

| Format | Detection key | Description |
|---|---|---|
| `mf4` (MAN-Autera) | `man_autera` | MDF4 log — standard `CAN_DataFrame` channel groups |
| `mf4` (APTIV-Orcas) | `aptiv_orcas` | MDF4 log — `MF4Frame` group, single bus channel |
| `mf4` (SCANIA-Orcas) | `scania_orcas` | MDF4 log — `MF4Frame` group, dual bus channels |
| `mf4` (RESIM-Canoe) | `resim_canoe` | CANoe resim output — VLSD_CHANNEL_GROUP with 23-byte fixed records and external SD payload |

DBC used: `dbc/VCAN_SRR6pT_V25.dbc`  
Object messages decoded: `SRRL_ObjData_*` (left radar) and `SRRR_ObjData_*` (right radar).

---

## Installation

### Prerequisites
- Python 3.11 or newer (tested on 3.14.3)
- Windows (developed and tested on Windows; should run on Linux/macOS with minor path adjustments)

### Setup

```bash
# Clone the repository
git clone <repo-url>
cd TRATON_RESIM_KPI

# Create and activate a virtual environment
python -m venv venv
venv\Scripts\activate          # Windows
# source venv/bin/activate     # Linux / macOS

# Install dependencies
pip install -r requirements.txt
```

---

## Running the Application

```bash
# From the repository root (with venv active)
python gui/main.py
```

Then use **File → Open MF4…** (or `Ctrl+O`) to load a log file.

---

## Project Structure

```
TRATON_RESIM_KPI/
├── dbc/
│   └── VCAN_SRR6pT_V25.dbc        # CAN database for SRR6 object messages
├── gui/
│   ├── main.py                    # Entry point
│   ├── main_window.py             # QMainWindow, layout, wiring, dialogs
│   ├── data_model.py              # MDF4 decoder, scan-index builder, object indexer
│   ├── plan_view_widget.py        # Matplotlib top-down radar canvas
│   ├── signal_plot_widget.py      # Time-series signal plot
│   ├── signal_tree_widget.py      # Two-level message/signal tree
│   ├── object_track.py            # ObjectTrack data model + build_object_tracks()
│   ├── object_track_widget.py     # Object Track UI (compute, stats table, plot)
│   └── view_settings.py           # Central SETTINGS singleton (colours, theme)
├── src/
│   ├── vcan_reader.py             # Low-level MDF4 / VCAN reader utilities
│   └── ...                        # Diagnostic / probe scripts
├── requirements.txt
└── README.md
```

---

## Coordinate System

| Axis | Direction | Plot axis |
|---|---|---|
| Longitudinal (Lon) | Forward = positive | Y-axis |
| Lateral (Lat) | Left = positive | X-axis (inverted) |

---

## Keyboard Shortcuts

| Key | Action |
|---|---|
| `A` / `←` | Previous frame |
| `D` / `→` | Next frame |
| `Space` | Play / Pause |
| `Home` | First frame |
| `End` | Last frame |
| `Ctrl+O` | Open MF4 file |
| `Ctrl+Q` | Quit |
