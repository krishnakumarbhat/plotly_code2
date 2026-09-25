# How to Use — SRR6T VCAN Visualiser

This guide walks through every feature of the application step by step.

---

## 1. Opening a Recording

1. Launch the application (or run `python main.py` from the `gui/` folder).
2. Click **File → Open MF4…** (or press `Ctrl+O`).
3. Select an MF4 file recorded from the SRR6T radar system.
4. The **Signals** tab is populated and the Plan View shows the first scan frame.

The title bar shows the loaded file name.  The status bar at the bottom shows the
total number of scan frames.

---

## 2. Navigating Scan Frames

The navigation bar at the bottom of the window controls playback:

| Control | Action |
|---------|--------|
| `|◄` button | Jump to first frame |
| `◄` button | Step one frame back |
| `►` button | Step one frame forward |
| `►|` button | Jump to last frame |
| **Slider** | Drag to any frame |
| **Space** | Toggle auto-play |
| **← / A** | Step back (keyboard) |
| **→ / D** | Step forward (keyboard) |
| **Home** | Jump to first frame |
| **End** | Jump to last frame |

The current **Scan Index**, **Frame N / M**, and **Timestamp** are shown to the
right of the navigation buttons.

---

## 3. Plan View Tab

The **Plan View** shows a top-down radar canvas with all detected objects for the
current scan frame.

### Sensor bar

Directly above the canvas a thin **sensor bar** shows host-vehicle data
synchronised to the current scan frame:

```
Speed: 72.3 km/h  │  Trailer: Connected  │  SteerAngle: -3.2°  │  YawRate: 0.012 rad/s  │  LonAcc: 0.15 m/s²  │  LatAcc: -0.03 m/s²
```

If `SRR2_SensorInput_K` frames are absent the bar shows `SensorInput: no data`.

### Radar side filter

The **Side** dropdown (bottom-right of the Plan View controls) switches between:

- `Both` — objects from both the left (SRRL) and right (SRRR) radar
- `Left only` — SRRL only
- `Right only` — SRRR only

### Object bounding boxes

Each detected object is drawn as a coloured bounding box with:

- A **heading arrow** pointing in the direction of travel
- A small **reference-point dot** on the edge that the radar signal reports
  (only for Moving/Stopped objects)

**Colour coding (single-file mode):**

| Colour | Meaning |
|--------|---------|
| Green | Moving |
| Yellow-green | Stopped |
| Greyed tones | Stationary / Unknown / other states |

**Colour coding (resim loaded — dual-source mode):**

| Colour | Source | State |
|--------|--------|-------|
| Green shades | Original | Moving / Stopped |
| Red shades | Resim | Moving / Stopped |
| Grey | Either | Unknown / Error / N/A / other |

### Source selector (resim mode only)

The **Show:** dropdown in the Plan View controls switches between
`Both`, `Original only`, and `Resim only`.

### Zoom and pan

- Use the mouse **scroll wheel** to zoom in/out (when Zoom Mode is on).
- Use **left-click drag** to pan (when Zoom Mode is on).
- Click **Zoom Mode** button to toggle; click **Reset View** to restore default
  limits.
- Use **Window Size…** to adjust the coordinate extent of the view.

### Selecting an object

- **Single-click** a bounding box to highlight the object and jump to its track
  in the Object Track tab.
- **Double-click** a bounding box to open a floating detail window for that object.
- Click empty space to clear the selection.

The click tolerance is **1 metre** — click close to the centre of the box.

---

## 4. Signals Tab and Signal Plot Tab

### Signals tab

The **Signals** tab lists all decoded CAN messages and their signals, grouped by
message name.  When a resim file is loaded, two group headers are shown:
`Original` (italic) and `Resim` (italic).

- **Single-click** a signal to select it.
- Hold `Ctrl` or `Shift` to select multiple signals.
- Click **Plot Selected Signals** to send selected signals to the Signal Plot tab
  (replaces the current plot).
- Click **Add to Plot** to append selected signals without clearing the current
  plot.

### Signal Plot tab

The Signal Plot displays time-series data for the selected signals as overlaid
line charts.  A values panel on the right shows the current value of each signal
at the playback cursor position.

| Control | Action |
|---------|--------|
| **Clear Plot** button | Remove all signals from the plot |
| **×** button per signal | Remove only that signal from the plot |
| Vertical line | Tracks the current scan frame timestamp |

Signal values are displayed in decimal notation (no scientific notation).
Large integer values use a thin-space thousands separator.

---

## 5. Object Track Tab

The Object Track tab computes the full lifecycles of **Moving** (DynamicProperty == 3),
**Stopped** (DynamicProperty == 2), and **Stationary** (DynamicProperty == 1) objects
from the loaded data.

**Stationary-only** tracks (objects that were never seen as Moving or Stopped) are
calculated internally for KPI cross-matching purposes but are **not shown** in the
track list or KPI event table.

Tracks are **computed automatically** whenever a file is loaded.  A progress
dialog is shown during computation.

### Track list

Each row in the track list shows:
- Radar side (L / R)
- Data source (Original / Resim)
- Tracking ID
- Start and end scan index
- Duration [s]
- Number of ID drops (brief detection interruptions within the track)

Click **Select Object Track…** to browse all computed tracks and open one directly.

### Track detail panel

Click any **Moving, Stopped, or Stationary** object in the Plan View to open its track in
the detail panel below the list:
- **Header**: UUID, source, side, TID, scan range, duration
- **Signal statistics table** (min, max, avg, median per signal)
- **Plot area** with the selected signal over time

**Jump to start** navigates the Plan View to the track's first scan frame.  
**Plot Signals** sends the track's signals to the Signal Plot tab (replaces current plot).  
**Add to Plot** appends signals to the current Signal Plot without clearing.

### Track filtering

- Type in the **search box** above the track list to filter by TID or source.
- Click a column header to sort.

---

## 6. Loading Resim Data

1. Load an original MF4 file first (Step 1).
2. Click **File → Load reprocessed MF4…** and select the resim MF4 file.
3. The application validates that the two files share at least 10% of scan
   indices — if they diverge completely a warning is shown.
4. On success:
   - The signal tree is split into `Original` and `Resim` groups.
   - The Plan View switches to dual-source palette (blue + green).
   - The `Show:` source selector appears in the Plan View controls.
   - The **Resim** tab becomes active.
5. To unload resim data: **File → Unload reprocessed data**.

---

## 7. Resim Tab

This tab quantifies how closely the resim output reproduces the original.
It is only active when both an original and a resim file are loaded.

### Before computing

Object tracks are computed automatically when files are loaded, enabling the
Track-to-Track KPI in addition to Frame-to-Frame KPI.

### Running the analysis

Click **Compute KPI**.  A progress bar runs through:

1. Frame-to-Frame matching (0–55 %)
2. Track-to-Track matching (55–100 %, only if tracks were computed)

### RESIM Score

Each radar side displays a **composite RESIM Score (0–100 %)** at the top of
its panel, coloured green (≥ 90 %), amber (70–89 %), or red (< 70 %).

The score combines three independent components:

| Component | What it measures | Weight |
|-----------|-----------------|--------|
| **TP Events %** | Fraction of original Moving/Stopped/Stationary object events successfully recreated in resim — `matched / orig × 100`; a low score indicates the reprocessor is missing detections present in the source file | 40 % |
| **FP Events %** | Extent to which resim avoids generating object events absent from the source file — `(1 − spurious / resim) × 100`; a low score indicates the reprocessor is producing false detections with no physical counterpart | 40 % |
| **Accuracy %** | Whether signal errors (P95) stay within configured tolerances for **Moving** matched pairs only | 20 % |

```
RESIM Score = 0.40 × TP Events + 0.40 × FP Events + 0.20 × Accuracy
```

### Frame-to-Frame sub-tab

Shows results for Left (SRRL) and Right (SRRR) radar side by side.

Only **Moving** (DynamicProperty == 3), **Stopped** (DynamicProperty == 2), and
**Stationary** (DynamicProperty == 1) objects are analysed.  Cross-matching
between classifications is allowed (e.g., a Stopped object in the original can
be matched to a Stationary object in resim).

**Matching rules:**
- Pairs are found by the **Hungarian (optimal) algorithm** on AABB-IoU scores —
  every original object is assigned the best available resim counterpart globally.
- A match is accepted when IoU ≥ **1 %**.
- For objects whose bounding box is smaller than **0.6 m in both dimensions**
  (sub-threshold for reliable IoU), a **distance fallback** is used: match is
  accepted when the centroid-to-centroid distance ≤ **0.6 m**.
- Signal accuracy is collected **only when both matched objects are Moving** (DynProp 3).
- Resim objects with no accepted original match are counted as **ghosts**.

**Summary line** (per side):

- `Original objects` — total Moving object-frames in the original
- `Matched` / `Unmatched`
- `Resim objects` — total Moving object-frames in resim
- `Mean IoU` — average bounding-box overlap of matched pairs

**Signal accuracy table** — for each KPI signal:

| Column | Meaning |
|--------|---------|
| N | Number of matched frame-pairs used |
| Mean Δ | Average signed error (original − resim) |
| RMSE | √(mean² + std²) — combined bias and scatter |
| Median | Median signed error |
| P95 \|Δ\| | 95th-percentile absolute error |
| Tolerance | Configured pass threshold |
| Quality | **PASS** (P95 ≤ tol) / **WARN** (tol < P95 ≤ 4×tol) / **FAIL** (P95 > 4×tol) |

Configured tolerances: position ±0.5 m, velocity ±1.0 m/s, heading ±15 °.

### Track-to-Track sub-tab

Shows results after matching full object lifecycles (Moving, Stopped, and
Stationary).  Stationary-only tracks are used for TP/FP scoring but are
**not shown** in this table.

**Matching rules:** same Hungarian algorithm, IoU ≥ 1 %, small-obj distance ≤ 0.6 m,
applied to cumulative per-scan scores across each track pair.

**Summary** (per side):

| Metric | Meaning |
|--------|---------|
| Original / Reprocessed tracks | Total track counts per source |
| Matched | Pairs accepted by Hungarian assignment |
| Orig only | Original tracks with no resim counterpart |
| Resim only | Resim tracks classified as spurious (no spatial overlap) |
| Outcompeted | Resim tracks spatially overlapping an orig track but not assigned |
| Short event | Tracks shorter than 200 ms — excluded from scoring |
| Mean IoU | Mean per-scan IoU over all matched pairs |

**Per-track detail table** — one row per track:

| Row colour | Meaning |
|------------|---------|
| Normal | Matched pair |
| Red | Orig only — original track with no resim counterpart |
| Orange | Resim only — resim ghost track |
| Blue | Outcompeted — resim track spatially overlapping orig but unassigned |
| Grey | Short event — track below 200 ms duration |

Columns include scan range, duration, coverage percentages, `Mean IoU`, and
per-signal mean Δ for matched pairs.  Click any column header to sort.  
Select a row and click **Jump to track** to navigate the Plan View to the
track's start frame and highlight the matched objects with a cyan halo.

### Saving results

Click **Export XLSX…** to export all results to an Excel workbook (one sheet per
radar side, with colour-coded quality cells).

Click **Export JSON…** to export all results to a structured JSON file.
---

## 8. Theme

**Settings → Theme → Dark mode** / **Light mode** switches the application
colour scheme.  The selection is saved across sessions.

---

## 9. Detaching Tabs

Any tab can be **detached into a floating window**:

- **Double-click** the tab label, or
- **Right-click** the tab label and choose *Detach to window*, or
- **Drag** the tab label outside the tab widget boundary.

Close the floating window to re-dock the tab.  Floating state is saved and
restored across sessions.

---

## 10. About / Documentation

**About → Documentation…** (or press `F1`) opens the in-app documentation
browser with technical reference pages:

- Application Overview
- Data Model & MF4 Loading
- Object Track Creation
- Resim KPI — Computation Methods

**About → How to use** reopens this guide.

Use **About → Jump to topic** to navigate directly to a specific page.
