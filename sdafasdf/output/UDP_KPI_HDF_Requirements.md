# UDP KPI — HDF5 File Requirements

## Overview

This document describes the exact HDF5 structure that MUDP extractor (or any tool generating HDF files) must produce so that the UDP KPI pipeline can successfully generate detection, alignment, and tracker KPI reports with plots and accuracy metrics.

---

## 1. Top-Level Structure

Each HDF5 file must contain one group per radar sensor. Expected sensor names:

```
FC    — Front Center
FL    — Front Left
FR    — Front Right
RL    — Rear Left
RR    — Rear Right
```

Each sensor group must contain one or more **stream groups** listed below.

---

## 2. Required Stream Groups & Datasets

### 2.1 Detection KPI — `DETECTION_STREAM`

```
Sensor/DETECTION_STREAM/
├── <Stream_Hdr> /              # One of: Stream_Hdr, stream_hdr, StreamHdr,
│   └── scan_index              #   STREAM_HDR, streamheader, stream_header,
│                               #   HEADER_STREAM, DRA_Stream_Hdr_T
├── scan_index                  # (optional, at stream root)
├── num_af_det                  # Number of AF detections per scan
├── rdd_idx                     # RDD index for each detection
├── ran                         # Range [m]
├── vel                         # Velocity [m/s]
├── theta                       # Azimuth [rad]
├── phi                         # Elevation [rad]
├── f_single_target             # Single-target flag
├── f_superres_target           # Super-resolution target flag
└── f_bistatic                  # Bistatic flag
```

**Critical requirement**: The `scan_index` dataset must be present — either directly inside the stream group, or inside a recognized header sub-group. Without `scan_index`, the stream is skipped entirely.

### 2.2 Detection KPI — `RDD_STREAM`

```
Sensor/RDD_STREAM/
├── <Stream_Hdr> /
│   └── scan_index
├── scan_index
├── rdd1_num_detect             # Number of RDD detections
├── rdd1_rindx                  # Range index
├── rdd1_dindx                  # Doppler index
├── rdd2_range                  # Range value [m]
└── rdd2_range_rate             # Range rate [m/s]
```

This is used for **RDD-assisted detection matching**, which produces full plots with per-scan accuracy. Requires `DETECTION_STREAM` data to also be available.

### 2.3 Detection — `VSE_STREAM`

```
Sensor/VSE_STREAM/
└── veh_speed                   # Vehicle speed [m/s]
```

Used for context/validation.

### 2.4 Detection — `CDC_STREAM`

```
Sensor/CDC_STREAM/
└── num_cdc_records             # Number of CDC records
```

### 2.5 Alignment KPI — `DYNAMIC_ALIGNMENT_STREAM`

```
Sensor/DYNAMIC_ALIGNMENT_STREAM/
├── <Stream_Hdr> /
│   └── scan_index
├── scan_index
├── vacs_boresight_az_nominal_V       # Nominal azimuth [deg]
├── vacs_boresight_az_estimated        # Estimated azimuth [deg]
├── vacs_boresight_az_kf_internal_V    # Kalman filter azimuth [deg]
├── vacs_boresight_el_nominal          # Nominal elevation [deg]
├── vacs_boresight_el_estimated        # Estimated elevation [deg]
└── vacs_boresight_el_kf_internal      # Kalman filter elevation [deg]
```

**Note**: This stream was absent in all test files. Without it, alignment KPI always reports "Failed".

### 2.6 Tracker KPI — `TRACKER_STREAM`

```
Sensor/TRACKER_STREAM/
├── <Stream_Hdr> /
│   └── scan_index
├── scan_index
├── trkID                         # Track ID
├── vcs_xposn                     # X position [m]
├── vcs_yposn                     # Y position [m]
├── vcs_xvel                      # X velocity [m/s]
├── vcs_yvel                      # Y velocity [m/s]
├── vcs_heading                   # Heading [rad]
├── len1                          # Length 1 [m]
├── len2                          # Length 2 [m]
├── wid1                          # Width 1 [m]
├── wid2                          # Width 2 [m]
└── f_moving                      # Moving flag
```

---

## 3. The `scan_index` Requirement (Root Cause of Most Failures)

The hdf_wrapper (`hdf_wrapper.py:153-198`) checks each stream for a valid `scan_index` before parsing. The lookup order:

1. Check for header sub-group matching: `Stream_Hdr`, `stream_hdr`, `StreamHdr`, `STREAM_HDR`, `streamheader`, `stream_header`, `HEADER_STREAM`, `DRA_Stream_Hdr_T`
2. Inside that header group, look for dataset named `scan_index` or `Stream_Hdr_scan_index`
3. If neither exists → `scan_index = None` → **stream is skipped entirely**

If a stream group exists in the HDF but lacks `scan_index`, it is **silently dropped** from KPI processing. The KPI code never sees it, and both RDD-assisted and fallback detection matching fail.

### Visual check

```python
import h5py
h = h5py.File("file.h5", "r")
for sensor in ["FC","FL","FR","RL","RR"]:
    path = f"{sensor}/DETECTION_STREAM"
    if path in h:
        grp = h[path]
        # Check for header
        headers = [k for k in grp.keys() if "header" in k.lower() or "hdr" in k.lower()]
        print(sensor, "headers:", headers)
        for hdr in headers:
            print("  scan_index in header:", "scan_index" in grp[hdr])
    else:
        print(sensor, "NO DETECTION_STREAM")
```

---

## 4. Why Files Failed — Trace

### Detection KPI flow (`detection_matching_kpi.py:594-634`)

```
process_detection_kpi(data, sensor_id)
  │
  ├─ _can_use_rdd_matching(data)
  │   ├─ DETECTION_STREAM in data?    ← FAILS if stream was skipped (no scan_index)
  │   ├─ RDD_STREAM in data?
  │   └─ rdd1_rindx / rdd1_dindx have values?
  │
  ├─ if YES → process_rdd_matching()  → PLOTS + ACCURACY ✅
  │
  └─ if NO  → DetectionSignalFallbackKPIHDF
      └─ process_signal_matching()
          ├─ DETECTION_STREAM in data? ← FAILS again
          └─ ran, vel, theta, phi available?
              → "Failed to process detection KPIs"
```

### Alignment KPI flow (`alignment_matching_kpi.py:34-49`)

```
process_alignment_matching()
  │
  └─ data.get("DYNAMIC_ALIGNMENT_STREAM") exists?
      → NO → "Skipping alignment matching KPI - DYNAMIC_ALIGNMENT_STREAM not found"
      → "Failed to process alignment KPIs"
```

### Results summary

| Pair | Input | Output | Detection | Alignment | Why |
|------|-------|--------|-----------|-----------|-----|
| 01-12 | CCA | any | ❌ 0/5 | ❌ 0/5 | CCA lacks scan_index in DETECTION_STREAM; no DYNAMIC_ALIGNMENT_STREAM in any file |
| 13-15 | IFV7XX | CCA | ❌ 2/5 (FR,RL only) | ❌ 0/5 | Output CCA lacks DETECTION_STREAM (FC) or scan_index (others) |
| 16 (4×4) / 25 (5×5) | IFV7XX | IFV7XX | ⚠️ 5/5 ran, **0% accuracy** | ❌ 0/5 | RDD matching ran but all hash keys mismatched due to numpy type bug (see §7) |

---

## 5. Checklist for HDF Generation

For each sensor (FC, FL, FR, RL, RR), ensure:

### Mandatory for ANY KPI to work

- [ ] Stream group has a recognized header sub-group containing `scan_index`
- [ ] OR: `scan_index` dataset exists directly inside the stream group

### For Detection KPI (RDD-assisted — best quality)

- [ ] `DETECTION_STREAM` with scan_index + signals: `num_af_det`, `rdd_idx`, `ran`, `vel`, `theta`, `phi`, `f_single_target`, `f_superres_target`, `f_bistatic`
- [ ] `RDD_STREAM` with scan_index + signals: `rdd1_num_detect`, `rdd1_rindx`, `rdd1_dindx`, `rdd2_range`, `rdd2_range_rate`

### For Detection KPI (signal-fallback fallback)

- [ ] `DETECTION_STREAM` with scan_index + at minimum: `num_af_det`, `ran`, `vel`, `theta`, `phi`

### For Alignment KPI

- [ ] `DYNAMIC_ALIGNMENT_STREAM` with scan_index + signals: `vacs_boresight_az_nominal_V`, `vacs_boresight_az_estimated`, `vacs_boresight_az_kf_internal_V`, `vacs_boresight_el_nominal`, `vacs_boresight_el_estimated`, `vacs_boresight_el_kf_internal`

### For Tracker KPI

- [ ] `TRACKER_STREAM` with scan_index + signals: `trkID`, `vcs_xposn`, `vcs_yposn`, `vcs_xvel`, `vcs_yvel`, `vcs_heading`, `len1`, `len2`, `wid1`, `wid2`, `f_moving`

---

## 6. What a Valid `scan_index` Looks Like

A valid `scan_index` is a 1-D dataset of integer scan frame IDs, present in **both** input and output HDF files for the same sensor+stream. Example from working IFV7XX file:

```
FC/DETECTION_STREAM/Stream_Hdr/scan_index  → [0, 1, 2, 3, ..., 215]  (216 scans)
FC/RDD_STREAM/Stream_Hdr/scan_index        → [0, 1, 2, 3, ..., 215]  (same indices)
```

The KPI pipeline uses these indices to align input and output data scan-by-scan before computing matching accuracy.

---

## 7. Known KPI Code Bug — 0% Accuracy on Self-Pairs (Data Type Mismatch)

Even when `DETECTION_STREAM` + `RDD_STREAM` are present with valid `scan_index` and `rdd1_rindx`/`rdd1_dindx`, the RDD-assisted matching produces **0% accuracy** even for self-pairs (same file as both input and output).

### Root Cause

`detection_matching_kpi.py:176-214` builds hash keys from HDF5 values without type casting:

```python
key = (veh_rdd['rdd1_rindx'][i][j], veh_rdd['rdd1_dindx'][i][j])
```

Values read from HDF5 are `numpy` scalar types (e.g. `numpy.uint16`, `numpy.float64`), but Python dict key lookup is type-sensitive. When the same file is used as both input and output, the identical values may produce keys of different numpy subtypes → hash lookup fails → **zero matches** → 0% accuracy.

### Fix

Cast to Python native `int()` before building hash keys:

```python
key = (int(veh_rdd['rdd1_rindx'][i][j]), int(veh_rdd['rdd1_dindx'][i][j]))
```

Same for the `sim_key` on line ~204. With this fix, a self-pair is expected to produce ~100% accuracy.

### Impact

- Affects **all pairs**, not just self-pairs
- The RDD matching pipeline "runs" (no crash, plot is generated) but all per-scan accuracy values are 0.0
- The signal-fallback path (`DetectionSignalFallbackKPIHDF`) is not affected — it uses different comparison logic
- Until this bug is fixed, **every pair** will report 0% detection accuracy regardless of HDF data quality
