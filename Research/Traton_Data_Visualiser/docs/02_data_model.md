# Data Model — MF4 Loading and Signal Decoding

## Overview

`data_model.py` is responsible for reading an MF4 file, decoding all CAN messages
using the DBC definition, and providing a clean, indexed interface that the GUI
consumes frame-by-frame.

---

## Loading Pipeline

```
MF4 file
   │
   ▼
read_vcan_from_mf4()          ← asammdf + cantools (vcan_reader.py)
   │  Reads all CAN channels, decodes each frame against the DBC
   │  Returns: dict[msg_name → pd.DataFrame(timestamp, signal_cols)]
   │
   ▼
DataModel.load()
   │  1. Builds scan_indices  — sorted list of unique Header scan counters
   │  2. Builds _frame_index  — scan_idx → {msg_name → row index}
   │  3. Builds ObjectRecord  — one record per object slot per scan cycle
   └─ Result: fully indexed DataModel ready for GUI consumption
```

---

## Scan Index

A **scan index** is the `ScanIndex` field from the SRRL/SRRR Header message.
It acts as the primary frame identifier; both radar sides produce the same
scan indices in a well-formed recording.  The slider in the GUI maps directly
to positions in the ordered `scan_indices` list.

When two files (original + resim) are loaded, the merged scan index list is
the union of both sets, enabling frame-by-frame alignment even if the resim
recording started or ended at a slightly different time.

---

## Object Slots

Each scan cycle carries up to **20 object slots per radar side**.  Slot signals
are suffixed `_L_01` … `_L_20` (SRRL) or `_R_01` … `_R_20` (SRRR).

The following signals are decoded for each slot:

| Signal | Meaning |
|--------|---------|
| `TrackingId` | Sensor-assigned track ID (0 = unoccupied) |
| `DynamicProperty` | Motion state (see table below) |
| `LonPosition` | Reference-point longitudinal position [m] |
| `LatPosition` | Reference-point lateral position [m] |
| `Length` | Object length [m] |
| `Width` | Object width [m] |
| `HeadingAngle` | Heading relative to VCS forward [rad] |
| `ReferencePoint` | Which corner/edge `LonPosition`/`LatPosition` describes |
| `ExistenceProbability` | Detection confidence [0–1] |
| `LonGndVel` | Longitudinal ground velocity [m/s] |
| `LatGndVel` | Lateral ground velocity [m/s] |
| `ClassMostProb` | Object classification |

### DynamicProperty Values

| Value | Name | Included in tracks/KPI? |
|-------|------|------------------------|
| 0 | Unknown | No |
| 1 | Stationary | No |
| 2 | **Stopped** | **Yes** |
| 3 | **Moving** | **Yes** |
| 4 | Oncoming | No |
| 5 | Crossing | No |
| 6 | Invalid | No |
| 7 | N/A | No |

---

## Reference Point and Bounding Box Centre

The `LonPosition` / `LatPosition` signals report the position of a specific
**reference point** on the object's bounding box, not necessarily the centre.
The `ReferencePoint` signal encodes which corner or edge is being reported:

```
         Lon+ (forward)
              ▲
      0─────1─────2
      │  Front    │
Lat+  7   CENTRE  3  Lat−
(left)│   OBJECT  │  (right)
      6─────5─────4
         Lon− (rear)
```

| Code | Position |
|------|----------|
| 0 | Front-left corner |
| 1 | Front edge centre |
| 2 | Front-right corner |
| 3 | Right edge centre |
| 4 | Rear-right corner |
| 5 | Rear edge centre |
| 6 | Rear-left corner |
| 7 | Left edge centre |
| 8 | Unknown — raw position treated as centre |

The `_REF_OFFSET` dictionary maps each code to a `(dLon, dLat)` fractional
offset (as a fraction of `Length` and `Width` respectively).  Adding
`dLon × Length` and `dLat × Width` to the raw position yields the **bounding
box centre**:

```python
_REF_OFFSET = {
    0: (-0.5, -0.5),   # Front-left
    1: (-0.5,  0.0),   # Front
    2: (-0.5,  0.5),   # Front-right
    3: ( 0.0,  0.5),   # Right
    4: ( 0.5,  0.5),   # Rear-right
    5: ( 0.5,  0.0),   # Rear
    6: ( 0.5, -0.5),   # Rear-left
    7: ( 0.0, -0.5),   # Left
    8: ( 0.0,  0.0),   # Unknown → centre
}
```

### ObjectRecord fields

| Field | Content |
|-------|---------|
| `ref_lon`, `ref_lat` | Raw DBC signal value (reference-point position) |
| `lon`, `lat` | Computed **bounding-box centre** (after applying `_REF_OFFSET`) |
| `ref_pt` | Raw `ReferencePoint` enum |

The Plan View draws bounding boxes **centred on `ref_lon`/`ref_lat`** (the raw
signal position) and shows the reference-point dot on the correct box edge using
the inverted offset.

---

## DataModel API

```python
model.scan_indices            # list[int]  — ordered scan frame IDs
model.get_objects(scan_idx)   # list[ObjectRecord] — all objects in that frame
model.get_header_timestamp(scan_idx)  # float | None — absolute timestamp [s]
model.frames                  # dict[msg_name → pd.DataFrame] — all decoded signals
model.scan_index_info()       # dict with first/last scan and count
```
