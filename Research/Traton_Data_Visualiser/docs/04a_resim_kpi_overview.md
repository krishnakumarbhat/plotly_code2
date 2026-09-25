# Resim Comparison — High Level Overview

## What Is the Resim Comparison?

The **Resim Comparison** measures how faithfully a reprocessed ("resim") dataset
reproduces the original radar output.  Three independent sub-scores assess
different failure modes of the reprocessing algorithm:

| Score | Engineering Question |
|-------|----------------------|
| **TP Events** | Did the reprocessing reproduce all object events that are present in the source file? |
| **FP Events** | Did the reprocessing avoid generating object events that do not exist in the source file? |
| **Accuracy** | For matched object pairs, are the reported kinematic signal values close enough? |

These three scores are combined into a single **RESIM Score**:

```
RESIM Score = 0.40 × TP Events + 0.40 × FP Events + 0.20 × Accuracy
```

TP Events and FP Events are weighted equally because failing to reproduce
an existing object and generating a spurious object are considered equally
critical defects. Accuracy carries a lower weight as a secondary fidelity
measure.

---

## The Three Scores

### TP Events — True Positive Events Score (0–100 %)

**Measures the fraction of object events from the source file that are
successfully recreated in the reprocessed output.**

```
TP Events = matched / total_original × 100
```

- 100 % → every original object event was reproduced in resim
- 0 % → no original object events were reproduced

An object event is considered matched only when a corresponding resim object
has sufficient spatial overlap (IoU ≥ 1 %, or centroid distance ≤ 0.6 m for
geometrically small objects where both dimensions are < 0.6 m).  Tracking IDs
are deliberately ignored — the tracker may reassign different IDs after
reprocessing without affecting this score.

A low TP Events score indicates the reprocessing algorithm is failing to detect
or maintain objects that are present in the original sensor output.

### FP Events — False Positive Events Score (0–100 %)

**Measures the extent to which the reprocessed output is free from object
events that have no correspondence in the source file.**

```
FP Events = (1 − spurious / total_resim) × 100
```

- 100 % → every object in the reprocessed output corresponds to a real object
  in the original recording
- Low value → the reprocessor is generating objects that do not exist in the
  source file (false detections, hallucinated tracks)

A resim object is classified as spurious — and penalises FP Events — only when
it has **no spatial overlap** with *any* original object in the relevant scan
or track lifetime.  A resim object that spatially overlaps a real original
object but was not selected by the assignment algorithm (lost the competition
to a closer pair) is classified as **"Outcompeted"** and does **not** penalise
FP Events.

A low FP Events score indicates the reprocessing algorithm is producing
erroneous object reports without a physical counterpart in the measurement.

### Accuracy (0–100 %)

**Measures the kinematic signal fidelity of matched object pairs.**

Accuracy is computed **only for matched pairs where both objects are Moving
(`DynamicProperty == 3`)**.  Stopped and Stationary matches contribute to
TP/FP Events scores but are excluded from Accuracy to avoid contaminating
kinematic quality metrics with stationary-object data.

Accuracy is based on the 95th-percentile absolute error (P95) for **four
kinematic signals** (position and velocity):

| Signal | Tolerance | Scored |
|--------|-----------|--------|
| Longitudinal position | ±0.5 m | Yes |
| Lateral position | ±0.5 m | Yes |
| Longitudinal ground velocity | ±1.0 m/s | Yes |
| Lateral ground velocity | ±1.0 m/s | Yes |
| Heading angle | ±15 ° | **Informational only** |

HeadingAngle P95 error is computed and shown in the signal statistics table
for diagnostic purposes, but it does **not** contribute to the Accuracy score.
This avoids penalising reprocessing runs where heading estimation differs
slightly from the original without affecting position or velocity quality.

For each scored signal:

```
signal_score = clamp(0, 100,  (4 × tolerance − P95) / (3 × tolerance) × 100)
```

| P95 | Signal score |
|-----|-------------|
| 0 (perfect) | 100 % |
| = tolerance | 100 % |
| = 2 × tolerance | 67 % |
| = 4 × tolerance | 0 % |
| > 4 × tolerance | 0 % |

The final Accuracy score is the **mean across the four scored signals**.

---

## Two Analysis Modes

The comparison is computed in two complementary modes:

### Frame-to-Frame (F2F)

- Unit of comparison: **individual scan observations**
- Orig baseline: **Moving objects only** (`DynamicProperty == 3`)
- Resim match pool: Moving (ghost-eligible) + Stopped/Stationary (match candidates; not counted as ghosts if unmatched)
- Matching: spatial only (IoU ≥ 1 % or centroid distance ≤ 0.6 m for small objects), no use of tracking IDs
- Covers every scan frame where both original and resim data are present
- Gives a broad picture of per-frame detection fidelity across the entire recording
- Signal accuracy (Accuracy score) uses only Moving-Moving matched pairs
- Each object-frame contributes its **ROI multiplier** to score accumulators; objects outside all ROIs are excluded from scoring

### Track-to-Track (T2T)

- Unit of comparison: **complete object lifecycles**
- Object scope: Moving (3), Stopped (2), Stationary (1); **stationary-only tracks** are used for matching but hidden from all UI lists
- Matching: cumulative spatial score over the full track duration
- Requires object tracks to be computed first (done automatically on file load)
- Reveals whether resim tracks objects consistently over time, not just in isolated frames
- TP Events and FP Events scores are **ROI-weighted** — each track contributes its best (highest)
  ROI multiplier achieved anywhere in its lifetime (*"closest range" rule*):
  - Track passed through ROI 1 at any point → 1.0× weight for entire track
  - Track never reached ROI 1 but reached ROI 2 → 0.5× weight
  - Track only reached ROI 3 → 0.25× weight
  - Track always outside all ROIs → excluded from scoring
- Signal accuracy uses only scans where both matched records are Moving

---

## Region of Interest (ROI)

Only objects and tracks located within a defined **Region of Interest** are
included in TP/FP/Accuracy score computations.  This ensures that score
components reflect detection quality in the areas that matter operationally,
not across the full sensor field of view.

Three nested ROI zones are defined (longitudinal = forward axis, lateral = left/right):

| Zone | Longitudinal | Lateral | Score multiplier |
|------|-------------|---------|------------------|
| **ROI 1** | −30 m … +10 m | −6 m … +6 m | **1.00×** |
| **ROI 2** | −50 m … +20 m | −9 m … +9 m | **0.75×** |
| **ROI 3** | −80 m … +25 m | −12 m … +12 m | **0.50×** |
| Outside all | — | — | **0.00 (excluded)** |

Zones are tested in priority order (ROI 1 first); the first match determines
the multiplier.

### F2F ROI Application

Each object-frame contributes its multiplier `_roi_mult_pos(lon, lat)` to the
weighted accumulators (total, matched, total_resim, ghost).  An object at
lon = −40 m, lat = 2 m falls in ROI 2 → contributes 0.75 to all counts.

### T2T ROI Application

For T2T the **"closest range" rule** applies: the multiplier assigned to a
track is the **best (highest) multiplier it ever achieved** across any scan in
its lifetime.  A track that briefly enters ROI 1 at some point receives 1.0×
weight even if it spends most of its life in ROI 2 or ROI 3.

This approach rewards the reprocessing system for correctly tracking objects
at close range and avoids penalising tracks that extend far beyond the
operationally important zone.

---


| RESIM Score | Colour | Interpretation |
|-------------|--------|----------------|
| ≥ 90 % | Green | Excellent reproduction |
| 70–89 % | Amber | Acceptable with notable differences |
| < 70 % | Red | Significant deviations; investigation recommended |

Each component score is also shown individually to allow rapid root cause
identification:

- **Low TP Events** → reprocessing is missing objects present in the original recording
- **Low FP Events** → reprocessing is generating spurious objects absent from the original
- **Low Accuracy** → matched objects have kinematic signal values outside tolerance

---

## Track Classification (Track-to-Track)

Each track is classified into one of five categories:

| Category | Description | Affects score? |
|----------|-------------|----------------|
| **Matched** | Original and resim track aligned by the assignment algorithm | Contributes to TP Events and Accuracy |
| **Orig only** | Original track with no resim counterpart (missed by reprocessor) | Reduces TP Events |
| **Resim only** | Resim track with no spatial overlap with any original (spurious detection) | Reduces FP Events |
| **Outcompeted** | Resim track overlapping a real original object but not selected by assignment | Does **not** affect any score |
| **Short event** | Track shorter than 200 ms (below minimum lifecycle threshold) | Excluded from all scoring |
| **Stationary-only** | Track whose every frame has DynamicProperty == 1 | Counted for TP/FP only; **hidden from UI** |

For detailed algorithmic descriptions see **Resim Comparison — Detailed**.
