# Running Tools on HPC Cluster (SLURM)

This guide explains how to run KPI, Interactive Plot, and DC HTML tools on HPC clusters using SLURM job scheduler.

---

## Overview

All three tools are available as Singularity containers:

| Tool | Image File | Purpose |
|------|-----------|---------|
| **KPI** | `simg/kpi.simg` | Generate KPI HTML reports |
| **Interactive Plot** | `simg/interactive_plot.simg` | Generate interactive plot HTML reports |
| **DC HTML** | `simg/dc_html.simg` | Generate DC (Data Collect) HTML reports |
| **All Services** | `simg/all_services.simg` | Main UI web application |

---

## Building Images

### Build All 4 Images at Once

```bash
# In WSL, from all_services directory:
bash scripts/wsl_build_all_images.sh
```

### Build Options

```bash
# Build only KPI image
bash scripts/wsl_build_all_images.sh --kpi-only

# Build only Interactive Plot
bash scripts/wsl_build_all_images.sh --interactive-only

# Build only DC HTML
bash scripts/wsl_build_all_images.sh --dc-html-only

# Build and sync to cluster
bash scripts/wsl_build_all_images.sh --sync user@cluster:/path/to/simg/
```

---

## Running on SLURM (Cluster)

### KPI Tool

```bash
# Submit JSON batch mode job
./simg/run_kpi.sh json KPIPlot.json /output/html_db

# Submit HDF pair mode job
./simg/run_kpi.sh hdf input.h5 output.h5 /output/html_db
```

### Interactive Plot

```bash
# Submit JSON mode job
./simg/run_interactive_plot.sh config.xml inputs.json /output/html

# Submit HDF pair mode job
./simg/run_interactive_plot.sh --hdf input.h5 output.h5 /output/html
```

### DC HTML

```bash
# Submit job
./simg/run_dc_html.sh HTMLConfig.xml Inputs.json /output/dc_html
```

---

## Running Locally with Progress Tracking

All run scripts support `--local` mode which runs directly on your machine with real-time progress display.

### KPI - Local Mode

```bash
# JSON batch mode with progress
./simg/run_kpi.sh --local json KPIPlot.json /output/html_db

# HDF pair mode
./simg/run_kpi.sh --local hdf input.h5 output.h5 /output/html_db
```

### Interactive Plot - Local Mode

```bash
./simg/run_interactive_plot.sh --local config.xml inputs.json /output/html
```

### DC HTML - Local Mode

```bash
./simg/run_dc_html.sh --local HTMLConfig.xml Inputs.json /output/dc_html
```

---

## Progress Tracking

### What You'll See

When running in local mode, you'll see real-time progress:

```
=== KPI Tool - JSON Batch Mode ===
JSON File: KPIPlot.json
HTML Dir:  /output/html_db
Total Pairs: 5

Running in LOCAL mode with progress tracking...
Started at: 2026-01-13 10:30:00

Progress: [████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░] 40% (2/5) | Elapsed: 3m 20s | ETA: 5m 0s
```

### SLURM Mode Progress

When running on SLURM, progress is logged to job output files:

```bash
# Monitor job progress
tail -f /scratch/logs/kpi_12345.out

# Check job status
squeue -j 12345
```

---

## Resource Allocation

Default resource allocations for each tool:

| Tool | Memory | CPUs | Time Limit | Partition |
|------|--------|------|------------|-----------|
| KPI | 32G | 8 | 2 hours | plcyf-com |
| Interactive Plot | 64G | 8 | 4 hours | plcyf-com |
| DC HTML | 32G | 4 | 2 hours | plcyf-com |

---

## Input File Formats

### KPIPlot.json (KPI Tool)

```json
[
  {
    "input_path": "/path/to/input.h5",
    "output_path": "/path/to/output.h5"
  }
]
```

### Inputs.json (Interactive Plot / DC HTML)

```json
{
  "InputHDF": [
    "/path/to/input1.h5",
    "/path/to/input2.h5"
  ],
  "OutputHDF": [
    "/path/to/output1.h5",
    "/path/to/output2.h5"
  ]
}
```

### HTMLConfig.xml (DC HTML)

```xml
<DATA_SOURCE_SELECTION>UDPDC</DATA_SOURCE_SELECTION>
```

Supported data sources:
- `UDP` - HDF Files from MUDP with UDP Data
- `UDPDC` - HDF Files from DC with UDP Data
- `MCIP_CAN` - HDF Files from Bordnet with MCIP_CAN Data
- `CEER_CAN` - HDF Files from Bordnet with CEER_CAN Data

---

## Running on Windows PC (Local Development)

You can run the DC HTML tool directly from Windows without containers:

```powershell
# Navigate to dc_html folder
cd C:\git\Core_RESIM_KPI\all_services\tools\dc_html

# Run using Python module
python -m IPS.ResimHTMLReport HTMLConfig.xml Inputs.json /output
```

Or with full paths:

```powershell
python -m IPS.ResimHTMLReport `
    C:\git\Core_RESIM_KPI\all_services\tools\dc_html\IPS\HTMLConfig.xml `
    C:\git\Core_RESIM_KPI\all_services\tools\dc_html\IPS\Inputs.json `
    /out
```

---

## Troubleshooting

### Image Not Found

```
Error: Singularity image not found: simg/kpi.simg
```

**Solution:** Build the images first:
```bash
bash scripts/wsl_build_all_images.sh
```

### Job Submission Failed

**Solution:** Check SLURM status and partition availability:
```bash
sinfo -p plcyf-com
squeue -u $USER
```

### Container Build Fails

**Solution:** Ensure Docker is running for Docker-based builds:
```bash
docker info
# Then retry build
```

---

## Python Progress Tracker (For Developers)

A Python module is available for adding progress tracking to your own scripts:

```python
from tools.progress_tracker import ProgressTracker, BatchProgressTracker

# Simple progress tracking
tracker = ProgressTracker(total_items=10, task_name="Processing HDF Pairs")
for i in range(1, 11):
    tracker.update(current=i, message=f"Processing pair {i}")
    # ... do work ...
tracker.complete()

# Batch progress tracking
batch = BatchProgressTracker(total_batches=5, batch_name="HDF Pair")
for i in range(1, 6):
    batch.start_batch(i, f"input_{i}.h5 → output_{i}.h5")
    # ... do work ...
    batch.end_batch(i, success=True)
batch.summary()
```
