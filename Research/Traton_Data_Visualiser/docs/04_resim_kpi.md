# Resim Comparison — Detailed

## Purpose

The Resim Comparison module (`resim_kpi.py`) quantifies how closely a reprocessed
(resim) dataset reproduces the original radar output.  It provides two
complementary analysis modes that together cover both per-scan fidelity and
long-term tracking continuity.

| Mode | Unit of comparison | ID matching |
|------|--------------------|-------------|
| **Frame-to-Frame** | Individual scan observations | Ignored — spatial only |
| **Track-to-Track** | Complete object lifecycles | By cumulative spatial score |

Both modes operate **independently per radar side** (Left SRRL / Right SRRR).
Objects with `DynamicProperty` values **1** (Stationary), **2** (Stopped), and
**3** (Moving) are all considered.  This allows cross-matching when the two
datasets disagree on classification (e.g., Stopped in original vs. Stationary
in resim).  Invalid objects (`DynamicProperty == 0`) are excluded.

**Accuracy score** is computed exclusively from matched pairs where **both**
objects are Moving (`DynamicProperty == 3`).  Stopped and Stationary matches
contribute to TP/FP scores only.

---

## High-Level Workflow

```
Load original MF4  ──┐
                      ├─► Compute Frame-to-Frame Comparison ──► Frame scores
Load resim MF4    ──┘

Build orig tracks  ──┐
                      ├─► Compute Track-to-Track Comparison ──► Track scores
Build resim tracks ──┘
```

For each mode the output is a **RESIM Score** (0–100) per radar side, built from
three independent sub-scores:

```
RESIM Score = 0.40 × TP Events + 0.40 × FP Events + 0.20 × Accuracy
```

| Signal | Unit | Notes |
|--------|------|-------|
| `LonPosition` | m | Reference-point longitudinal position |
| `LatPosition` | m | Reference-point lateral position |
| `LonGndVel` | m/s | Longitudinal ground velocity |
| `LatGndVel` | m/s | Lateral ground velocity |
| `HeadingAngle` | ° | Converted from radians; wrap-corrected delta — **informational only, not scored** |

All deltas are computed as **original − resim**, so a positive mean indicates
the original value was higher.

---

## Bounding Box IoU (AABB-IoU)

Both matching methods rely on **Axis-Aligned Bounding Box Intersection over
Union (AABB-IoU)**.

Given two objects with centres `(lon₁, lat₁)` / `(lon₂, lat₂)` and dimensions
`(L₁, W₁)` / `(L₂, W₂)`:

```
intersection_lon = max(0, min(lon₁+L₁/2, lon₂+L₂/2) − max(lon₁−L₁/2, lon₂−L₂/2))
intersection_lat = max(0, min(lat₁+W₁/2, lat₂+W₂/2) − max(lat₁−W₁/2, lat₂−W₂/2))
intersection     = intersection_lon × intersection_lat

union = L₁×W₁ + L₂×W₂ − intersection

IoU = intersection / union        (0 = no overlap, 1 = perfect overlap)
```

Minimum dimension of **0.3 m** is enforced to avoid division-by-zero on
degenerate slots.

### Small-Object Fallback

When both dimensions of an object are below **0.6 m**, AABB-IoU is unreliable
(tiny boxes rarely overlap even for the same physical point).  In this case the
matching criterion switches to **centroid Euclidean distance**:

- If distance ≤ 0.6 m → accepted as a match; score ∈ [0, 0.5] so it is always
  ranked below any IoU-based match.
- Otherwise → no match.

---

## Frame-to-Frame Comparison

### Step-by-Step Algorithm

For each scan index present in **both** datasets:

1. **Extract objects** — collect objects from both the original and resim dataset
   for that scan, split by radar side (L / R):
   - **Orig baseline**: Moving objects (`DynamicProperty == 3`) **only** — these
     form the TP/FP denominator.
   - **Resim match pool**: Moving (ghost-eligible, front of list) — counted as
     ghosts if unmatched — plus Stopped/Stationary (back of list, match
     candidates only; never counted as ghosts).
2. **ROI pre-filter** — each object’s position `(ref_lon, ref_lat)` is looked up
   in the ROI zone table (see ROI section below).  The resulting multiplier
   (0.0 – 1.0) is accumulated instead of a flat `1`; objects outside all ROIs
   contribute `0.0` to every accumulator (effectively excluded).
3. **Build cost matrix** — for each original × resim pair on a given side,
   compute the per-scan match score:
   - If `IoU ≥ 0.01` → `cost = 1 − IoU` (lower is better)
   - Else if either object is small (both dims < 0.6 m) **and** centroid
     distance ≤ 0.6 m → `cost = distance / 0.6` (lower is better)
   - Else → `cost = 1e9` (no valid match)
4. **Hungarian assignment** (`scipy.optimize.linear_sum_assignment`) — finds
   the globally optimal one-to-one pairing that minimises total cost.  Each
   original object and each resim object is used **at most once**.
5. **Accept pairs** — any pair where `cost < 0.5 × 1e9` is a valid match.
   Accumulate IoU.  Signal deltas are computed **only when both matched objects
   have `DynamicProperty == 3`** (Moving); Stopped/Stationary matches count for
   TP/FP but not for Accuracy.
6. **Count ghosts** — for each unassigned Moving resim object, check every column of
   the cost matrix.  If **all** original objects had `cost = 1e9` (no spatial
   overlap with anything), the resim object is a **ghost**.  If it had at least
   one valid candidate but was outcompeted, it is **not** a ghost.

### Why Tracking IDs Are Ignored

After reprocessing, the tracker may assign different `TrackingId` values to the
same physical object.  Spatial position (IoU / centroid distance) is a more
reliable pairing criterion than ID.

### Accumulated Metrics Per Side

All counters are **ROI-weighted floats**, not raw integer counts.  Each
object-frame contributes its ROI multiplier (0.0, 0.5, 0.75, or 1.0) rather
than a flat `1`.

| Metric | Definition |
|--------|------------|
| **Total** | ROI-weighted sum of orig Moving object-frames across all scans |
| **Matched** | ROI-weighted sum of orig object-frames with an accepted resim match |
| **Total Resim** | ROI-weighted sum of Moving resim object-frames across all scans |
| **Ghost** | ROI-weighted sum of Moving resim object-frames with no spatial overlap with any orig object |
| **Availability [%]** | `100 × Matched / Total` |
| **Ghost [%]** | `100 × Ghost / Total_Resim` |
| **Mean IoU** | Mean AABB-IoU over all matched pairs |

Signal statistics (mean Δ, RMSE, std, median, P95 |Δ|, max |Δ|) are computed
over all matched pairs.

---

## Track-to-Track Comparison

### Prerequisites

Object tracks must be computed first in the **Object Track** tab.  The comparison
engine receives two separate lists: original tracks and resim tracks.  Tracks
include Moving, Stopped, and Stationary objects.  **Stationary-only tracks**
(every record has `DynamicProperty == 1`) participate in matching to allow
Stopped↔Stationary cross-matching, but are hidden from the event table.

### Step 1 — Build Per-Pair Score Matrix

For each candidate pair `(original track Oᵢ, resim track Rⱼ)` on the same
radar side:

1. Find the set of **common scan indices** (scans present in both tracks).
2. If no common scans exist → score = 0 (ineligible pair).
3. For each common scan, compute the per-scan match score using the same IoU /
   small-object-distance logic as Frame-to-Frame.
4. **Cumulative score** = sum of all per-scan scores.
5. **Mean IoU** = mean AABB-IoU over common scans (reported separately).

### Step 2 — Hungarian Assignment

The globally-optimal one-to-one assignment is found with `linear_sum_assignment`
(negated score matrix → minimise negative score = maximise score).  Each
original track and each resim track is paired **at most once**.

A pair is accepted only if `score > 0` (at least one valid per-scan match).

### Step 3 — Classify Unmatched Resim Tracks

For every resim track that was not assigned to any original track:

| Condition | Classification | Colour in table |
|-----------|---------------|-----------------|
| Score column = 0 for all orig tracks (no common scans with any orig) | **Resim only** (spurious) | Orange |
| Had non-zero score but outcompeted; AND ≤ 50 % of its scans can spatially match any orig track | **Resim only** (spurious) | Orange |
| Had non-zero score but outcompeted; AND > 50 % of its scans can spatially match any orig track | **Outcompeted** | Blue |

The 50 % threshold (`_GHOST_FRAME_COV_THRESHOLD = 0.50`) is configurable in
`resim_kpi.py`.  An "Outcompeted" track is a real object that simply had no
dedicated original counterpart available in the assignment — it is **not
penalised** in the FP Events score.

### Step 4 — Per-Track Metrics (matched pairs only)

| Metric | Definition |
|--------|------------|
| **Coverage [%]** | `100 × common_scans / orig_track_scans` — fraction of original track lifetime observed by the resim track |
| **Resim Coverage [%]** | `100 × common_scans / resim_track_scans` — fraction of resim track lifetime aligned with the original |
| **Mean IoU** | Mean AABB-IoU over common scans |
| **Signal stats** | Mean Δ, RMSE, std, median, P95 |Δ|, max |Δ| over common scans where **both records are Moving** (DynProp 3) |

### Step 5 — ROI-Weighted Aggregation

All side-level metrics are **ROI-weighted**: each track’s contribution to
frame counts is multiplied by its ROI multiplier.  This is the primary
weighting mechanism and also incorporates track length naturally (a longer
track generates more scan samples, which further scales its contribution).

The ROI multiplier is the **best (highest) zone the track ever entered**
(\u201cclosest range\u201d rule), determined by scanning every `TrackRecord`’s position:

| Best zone reached | Multiplier |
|-------------------|------------|
| ROI 1 (lon −30…+10 m, lat −6…+6 m) | 1.00 |
| ROI 2 (lon −50…+20 m, lat −9…+9 m) | 0.75 |
| ROI 3 (lon −80…+25 m, lat −12…+12 m) | 0.50 |
| Never inside any ROI | 0.00 — **excluded from all score accumulators** |

| Metric | How computed |
|--------|--------------|
| **TP Events score** | `100 × roi_matched_frames / roi_orig_frames` (ROI-weighted overlapping scans per pair) |
| **FP Events score** | `100 × (1 − roi_ghost_frames / roi_resim_frames)` (only true-spurious frames, ROI-weighted) |
| **Avg Coverage [%]** | Weighted mean of `coverage_pct` across matched pairs |
| **Avg Resim Coverage [%]** | Weighted mean of `resim_coverage_pct` across matched pairs |
| **Mean IoU** | Weighted mean of `mean_iou` across matched pairs |
| **Accuracy score** | P95-based (see below) — naturally duration-weighted by sample count; **Moving-only matched scans** |

---

## RESIM Score Components

### TP Events Score (0–100)

Measures the fraction of original object events successfully recreated in the
reprocessed output.

**Frame-to-Frame:**
```
TP Events = 100 × matched_object_frames / total_orig_object_frames
```

**Track-to-Track (duration-weighted):**
```
TP Events = 100 × Σ(overlapping_frames per matched pair)
                / Σ(orig_track_frames for all orig tracks)
```
Only the actual overlapping scans between a matched pair count in the
numerator, not the full original track length.

### FP Events Score (0–100)

Measures the extent to which the reprocessed output is free from spurious
object events that have no correspondence in the source file.

**Frame-to-Frame:**
```
FP Events = 100 × (1 − spurious_frames / total_resim_frames)
```
A resim object-frame is spurious only if it has **no spatial overlap** with any
original object in that scan (not just unassigned by Hungarian).

**Track-to-Track (duration-weighted):**
```
FP Events = 100 × (1 − spurious_frames / total_resim_frames)
```
Only frames belonging to **true spurious tracks** (`is_ghost = True`) count.
Outcompeted-but-overlapping tracks are excluded.

### Accuracy Score (0–100)

Measures signal fidelity for matched pairs based on P95 absolute errors.

For each signal, a per-signal score is computed:

```
score_sig = max(0, (4 × tolerance − P95_error) / (3 × tolerance)) × 100
```

The final accuracy score is the **mean across all five signals**:

| Signal | Tolerance |
|--------|-----------|
| LonPosition | 0.5 m |
| LatPosition | 0.5 m |
| LonGndVel | 1.0 m/s |
| LatGndVel | 1.0 m/s |
| HeadingAngle | 15.0 ° |

- P95 ≤ tolerance → signal score = 100 %
- P95 = 4 × tolerance → signal score = 0 %
- Linear interpolation between those points.

### Composite RESIM Score (0–100)

```
RESIM Score = 0.40 × TP Events + 0.40 × FP Events + 0.20 × Accuracy
```

All three components contribute to the final score, weighted 40 / 40 / 20.

### Signal Quality Column (in Statistics Table)

Each individual signal also receives a **PASS / WARN / FAIL** quality label
based on its P95 error versus the configured tolerance:

| Label | Condition |
|-------|-----------|
| **PASS** | P95 ≤ tolerance |
| **WARN** | tolerance < P95 ≤ 4 × tolerance |
| **FAIL** | P95 > 4 × tolerance |

---

## Region of Interest (ROI)

All TP/FP/Accuracy score accumulators use ROI-weighted counts rather than raw
object or frame counts.  The ROI system ensures that only detections in
operationally relevant zones influence the score.

### Zone Definitions

| Zone | Longitudinal (forward axis) | Lateral (left/right) | Multiplier |
|------|-----------------------------|----------------------|------------|
| **ROI 1** | −30 m … +10 m | −6 m … +6 m | **1.00×** |
| **ROI 2** | −50 m … +20 m | −9 m … +9 m | **0.75×** |
| **ROI 3** | −80 m … +25 m | −12 m … +12 m | **0.50×** |
| Outside | — | — | **0.00** (excluded) |

Zones are tested in priority order (ROI 1 first).  ROI 2 includes ROI 1; ROI 3
includes ROI 1 and ROI 2.  Because ROI 1 is tested first, a position inside
ROI 1 always gets the full 1.0× weight.

### F2F: Per-Position Lookup (`_roi_mult_pos`)

For each object-frame, the position `(ref_lon, ref_lat)` is looked up and the
corresponding multiplier is added to the weighted accumulator.  The fractional
counters flow directly into the TP/FP score formulas.

### T2T: Per-Track Best Zone (`_roi_mult_track`)

For T2T, the **best (highest) multiplier** observed across all scan records of
the track is used for the **entire track** (\u201cclosest range\u201d rule).  This is
because a track that passed through ROI 1 at any point is a high-value
detection event regardless of where it started or ended.

The early-exit optimisation (`if best >= 1.0: break`) means tracks that enter
ROI 1 are assigned weight 1.0 without scanning remaining records.

---

## Statistical Metrics

For any set of signed deltas {δ₁, δ₂, …, δₙ}:

| Metric | Formula |
|--------|---------|
| **N** | Sample count |
| **Mean Δ** | Arithmetic mean (signed bias) |
| **RMSE** | `√(mean² + std²)` — combined bias and scatter |
| **Std** | Standard deviation of signed delta |
| **Median** | 50th percentile of signed delta |
| **P95 \|Δ\|** | 95th percentile of absolute delta |
| **Max \|Δ\|** | Worst-case absolute error |

### Heading Delta

Because heading is a circular quantity (stored in radians), the delta is
wrap-corrected before converting to degrees:

```
diff      = orig_rad − resim_rad
diff      = (diff + π) mod (2π) − π      # wrap to [−π, +π]
delta_deg = degrees(diff)
```

---

## Track Table Colour Coding

| Row colour | Meaning |
|------------|---------|
| Normal (white/dark) | Matched pair — orig and resim track aligned |
| **Red** | Orig-only — original track with no resim counterpart |
| **Orange** | Resim only — resim track with no spatial relationship to any original |
| **Blue** | Outcompeted — resim track spatially overlapping orig but not assigned (> 50 % frame coverage with some orig track) |
| **Grey** | Short event — track shorter than 200 ms; excluded from all scoring |

---

## Export

The **Export XLSX…** and **Export JSON…** buttons export all computed results.

### XLSX Export

Two sheets (Left SRRL / Right SRRR), each containing:
1. **Frame-to-Frame Summary** — per-side object counts (orig / matched / unmatched / resim), TP Events %, mean IoU, and all RESIM score components
2. **Frame-to-Frame Signal Accuracy** — per-signal statistics with tolerance and PASS/WARN/FAIL quality label
3. **Track-to-Track Summary** — per-side orig/resim track counts, spurious count, TP events/FP events/accuracy/RESIM scores, coverage and mean IoU
4. **Track-to-Track Aggregated Signal Accuracy** — pooled stats per side
5. **Track-to-Track Per-Track Details** — one row per track (Matched, Orig only, Resim only, Outcompeted, Short event) with scan range, duration, coverage percentages, mean IoU, and mean signal deltas

Cells are colour-coded: green (PASS), amber (WARN), red (FAIL) for signal quality;  
Source column rows are colour-coded: red (Orig only), orange (Resim only), blue (Outcompeted), grey (Short event).

### JSON Export

Nested dict structure:
```
{
  "frame_to_frame": { "left": { ... }, "right": { ... } },
  "track_to_track":  { "left": { ... }, "right": { ... } }
}
```
Each side contains full signal stats (n, mean, rmse, std, median, p95_abs, max_abs, unit, tolerance, quality)
and, for Track-to-Track, a `tracks` list with one entry per `TrackMatch`.
