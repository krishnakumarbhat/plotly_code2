# Object Tracks — Creation and Lifecycle

## What Is an Object Track?

An **object track** (`ObjectTrack`) represents the complete observed lifetime of
one real-world radar object as seen by one radar side.  It is a time-ordered
sequence of per-scan observations (`TrackRecord`) belonging to the same physical
object, identified by a stable `TrackingId` value assigned by the radar's internal
tracker.

---

## Which Objects Are Tracked?

Objects with the following `DynamicProperty` values are included:

| Value | Label | Included | Visible in UI lists |
|-------|-------|----------|---------------------|
| 3 | Moving | Yes | Yes |
| 2 | Stopped | Yes | Yes |
| 1 | Stationary | Yes | **Only if track also has Moving/Stopped frames** |
| 0 | Invalid | No | — |

**Stationary-only** tracks (all records have `DynamicProperty == 1`) are built
and used internally for KPI matching (improving TP/FP scores by allowing
Stopped↔Stationary cross-matching between datasets), but are **hidden from all
UI lists** and the KPI event table.

---

## Track Building Algorithm (`build_object_tracks`)

### Step 1 — Iterate Scan Frames

Every scan index in the `DataModel` is visited in order.  Scan indices are
reliable integers from `SensorHeader`; timestamps from CAN logs are **not used**
for track lifecycle decisions (they can be non-monotonic in resim files).

### Step 2 — Key Assignment

Each object is identified by the composite key `(side, tracking_id)`:

- `side` = `"L"` (SRRL) or `"R"` (SRRR)
- `tracking_id` = sensor-assigned `TrackingId` signal value

### Step 3 — Gap Detection

An `active` dictionary maps each `(side, tracking_id)` key to `(last_scan_idx, ObjectTrack)`.
When the same key is seen again:

```
scan gap = current_scan_idx − last_scan_idx
```

| Scan gap | Action |
|----------|--------|
| **0** | **Skip** — duplicate scan index in MF4 data (sensor counter did not increment between two CAN frames).  Track stays alive; the second frame is discarded. |
| 1 – 4 (≤ 200 ms) | **Continue** existing track — append new `TrackRecord` |
| > 4 | **Close** current track, start a **new** track with a fresh UUID |
| < 0 (non-monotonic) | **Close** current track, start a **new** track with a fresh UUID |

The 4-scan threshold (4 × 50 ms = 200 ms) accommodates up to four missed frames
at 20 Hz, allowing small detection interruptions within a single object pass.
Duplicate scan indices (gap = 0) occur in some Aptiv ORCAS MF4 files where the
sensor publishes two CAN frames for the same scan cycle without incrementing the
`ScanIndex` counter; they are silently skipped to prevent false track splits.

### Step 4 — Finalisation

When a track is closed (gap exceeded or end of file), `finalize()` is called:

1. Records are sorted by `scan_idx`.
2. **Start/end** scan indices and timestamps are recorded.
3. **Duration** = `(end_scan − start_scan) × 0.050` [s] — scan-index-based,
   immune to broken timestamps.
4. **ID drops** are detected: any pair of consecutive records whose `scan_idx`
   difference is > 1 indicates a momentary ID loss within the same track lifecycle.
5. **`is_stationary_only`** flag is set to `True` when every record has
   `DynamicProperty == 1`.
6. **Per-signal statistics** are computed across all records (see below).

---

## Signal Statistics

For every signal present in the slot data, the following statistics are computed
over all frame observations within the track:

| Statistic | Description |
|-----------|-------------|
| `min` | Minimum observed value |
| `max` | Maximum observed value |
| `avg` | Arithmetic mean |
| `median` | Median value |
| `initial` | Value in the first observed frame |
| `last` | Value in the last observed frame |

Signal names are **stripped** of their slot suffix before aggregation (e.g.,
`LonPosition_L_03` → `LonPosition`), so the same physical quantity observed
across different object slots within a track is merged into one entry.

---

## TrackRecord

Each `TrackRecord` stores a snapshot of one scan observation:

| Field | Content |
|-------|---------|
| `scan_idx` | Scan index of this observation |
| `timestamp` | Reception timestamp [s] — stored for display only; not used for lifecycle logic |
| `dyn_property` | `DynamicProperty` value for this frame (1 = Stationary, 2 = Stopped, 3 = Moving) |
| `raw_signals` | Full dict of decoded slot signals for this frame |

---

## ObjectTrack Fields

| Field | Content |
|-------|---------|
| `uuid` | UUID4 string — unique even if `tracking_id` is reused |
| `side` | `"L"` or `"R"` |
| `tracking_id` | Radar-assigned ID |
| `data_source` | `"Original"` or `"Resim"` |
| `start_scan` / `end_scan` | First and last scan index |
| `start_ts` / `end_ts` | First and last reception timestamp [s] (display only) |
| `duration` | `(end_scan − start_scan) × 0.050` [s] — scan-index-based, reliable |
| `is_stationary_only` | `True` when every record has `DynamicProperty == 1` |
| `stats` | Dict of `SignalStats` per signal |
| `id_drops` | List of `(scan_before, scan_after)` tuples where `scan_after − scan_before > 1` |
| `records` | Ordered list of `TrackRecord` |

---

## Example Track Lifecycle

```
Scan:  100   101   102   103   (gap: 6 scans)  109   110   111
TID:    5     5     5     5                     5     5     5
        ├── Track A (UUID-1) ───────────────┤   ├─ Track B (UUID-2) ┤
```

In the example above, the 6-scan gap (> 4-scan threshold) splits the object
into two separate `ObjectTrack` instances even though the `TrackingId` is the
same in both segments.

---

## Multi-Source Tracks

When both an original and a resim MF4 are loaded, `build_object_tracks` is
called separately for each model.  Tracks carry a `data_source` label
(`"Original"` or `"Resim"`) and are merged into a single unified list only
for display purposes in the Object Track widget.  The KPI engine receives
the two lists separately and performs its own matching.
