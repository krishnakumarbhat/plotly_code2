# SRR6T VCAN Visualiser — Application Overview

## Purpose

The **SRR6T VCAN Visualiser** is a desktop tool for loading, visualising, and
quantitatively comparing MF4 (MDF4) radar sensor logs produced by TRATON's SRR6T
Short-Range Radar system.  Its primary use-case is **resimulation (resim)
validation**: comparing an original sensor recording with a reprocessed version of
the same recording (e.g., after a firmware or algorithm change) to check whether
object-level behaviour is consistent.

---

## File Format

The tool reads **MDF4 (`.mf4`)** files containing decoded VCAN (Virtual CAN) data.
The DBC file `dbc/VCAN_SRR6pT_V25.dbc` defines the CAN message and signal layout.

Four source formats are supported and auto-detected on load:

| Format label | Source | Detection rule |
|---|---|---|
| `man_autera` | MAN-Autera data logger | `CAN_DataFrame` channel group with standard (non-VLSD) layout |
| `aptiv_orcas` | APTIV Orcas logger | `MF4Frame.ProtocolType` channel, single CAN bus |
| `scania_orcas` | SCANIA Orcas logger | `MF4Frame.ProtocolType` channel, dual CAN buses (3 + 4) |
| `resim_canoe` | CANoe resim output | `CAN_DataFrame` group where `channel_group.samples_byte_nr == 23` (8 B ts + 1 B flags + 1 B DLC + 1 B DataLength + 4 B CAN-ID + 8 B VLSD offset); external SD payload |

Each file contains two independent radar sides:

| Prefix | Side |
|--------|------|
| `SRRL_` | Left radar (SRRL) |
| `SRRR_` | Right radar (SRRR) |

Each side exposes up to **20 object slots** per scan cycle, named with suffixes
`_L_01` … `_L_20` (left) and `_R_01` … `_R_20` (right).

---

## Coordinate System

All positions use the **Vehicle Coordinate System (VCS)**:

| Axis | Direction | Signal |
|------|-----------|--------|
| Longitudinal (Lon) | Forward = positive | `LonPosition`, `LonGndVel` |
| Lateral (Lat) | Left = positive, Right = negative | `LatPosition`, `LatGndVel` |

In the **Plan View**, lateral is mapped to the X-axis (inverted, so left appears
on the right side of the screen matching driver perspective) and longitudinal to
the Y-axis.

---

## Application Layout

```
┌─ Menu bar: File | Settings | About ──────────────────────────────────┐
├─ Left panel ───────────┬─ Right panel (tabs) ──────────────────────┤
│  Signal Tree           │  Plan View  │  Signal Plot  │  Object Track │
│                        │             │               │  Resim KPI    │
│  [Plot Signal]         │                                              │
│  [Add to Plot]         │                                              │
├────────────────────────┴──────────────────────────────────────────────┤
│  [|◄] [◄]  ══ slider ══  [►] [►|]    Side: [L/R/Both]                │
│  Scan Index: XXXXX    Frame N / M    Timestamp: XX.XXX s              │
└───────────────────────────────────────────────────────────────────────┘
```

### Tabs

| Tab | Purpose |
|-----|---------|
| **Plan View** | Top-down Matplotlib canvas showing all detected objects per scan frame with bounding boxes, heading arrows, and reference-point dots. |
| **Signal Plot** | Time-series plots of any signal from the signal tree. Multiple signals can be overlaid; individual traces can be removed with the × button. |
| **Object Track** | Computes full object lifecycles (Moving, Stopped, Stationary) from the loaded data. Shows signal statistics per track and can plot any signal over time. |
| **Resim Comparison** | Side-by-side KPI comparison between original and resim data. Enabled only when both files are loaded. |

---

## Workflow

### Basic Playback

1. **File → Open MF4…** — load an original recording.
2. Use the slider or `◄` / `►` buttons to step through scan frames.
3. Click any bounding box in Plan View to select an object and see its signal
   values in the Signal Plot tab.

### Resim Validation

1. Load the original MF4 (step above).
2. **File → Load reprocessed MF4…** — load a second file recorded from the same
   scenario but after reprocessing.
3. The Plan View will display both datasets side-by-side using different colour
   palettes.  **Original** objects are drawn with **solid** bounding boxes;
   **Resim** objects use a **dotted** outline with a separate colour palette
   (red tones), making the two sources visually distinguishable at a glance.
4. Switch to the **Object Track** tab — tracks are computed automatically when
   files are loaded (Moving, Stopped, and Stationary objects).
5. Switch to the **Resim Comparison** tab and click **Compute KPI**.

---

## Technology Stack

| Library | Version | Role |
|---------|---------|------|
| Python | 3.14 | Runtime |
| PyQt6 | 6.11 | GUI framework |
| asammdf | 8.8 | MDF4 reader |
| matplotlib | 3.10 | Plan View and Signal Plot rendering |
| cantools | 41 | DBC decoding |
| pandas | 3.0 | Signal DataFrames |
| numpy | 2.4 | Numerical computation |
| scipy | 1.17 | Hungarian algorithm (optimal KPI matching) |
