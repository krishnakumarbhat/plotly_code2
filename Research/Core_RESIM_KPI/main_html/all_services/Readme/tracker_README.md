# Tracker scripts (gen7v2_resim_kpi_scripts)

This document describes the four `tracker_*.py` scripts in the `gen7v2_resim_kpi_scripts` folder, explains their inputs/outputs, shows how they work at a high level, and gives quick usage and troubleshooting tips.

Files covered
- [gen7v2_resim_kpi_scripts/tracker_matching_kpi_script.py](gen7v2_resim_kpi_scripts/tracker_matching_kpi_script.py)
- [gen7v2_resim_kpi_scripts/tracker_processed_det_kpi_script.py](gen7v2_resim_kpi_scripts/tracker_processed_det_kpi_script.py)
- [gen7v2_resim_kpi_scripts/tracker_vehicle_info_kpi_script.py](gen7v2_resim_kpi_scripts/tracker_vehicle_info_kpi_script.py)
- [gen7v2_resim_kpi_scripts/tracker_info_kpi_script.py](gen7v2_resim_kpi_scripts/tracker_info_kpi_script.py)

Overview
--------
All four scripts follow the same pattern and purpose: compare vehicle (recorded) CSV streams with simulation (re-simulated) CSV streams, compute KPIs and time-series metrics, render Plotly plots, and write an HTML report for each processed log. Each script targets a different CSV stream / set of columns (object stream, processed detections, vehicle state, tracker info).

Common flow (shared by all scripts)
- CLI check: each script expects exactly three command-line args: `log_path.txt meta_data.txt output_folder`.
- Read `meta_data.txt` into a `metadata_dict` (simple whitespace-split per line).
- Read `log_path.txt` (first line is the directory containing CSV files).
- `find_data_files(directory)` scans files in the directory for a script-specific `file_suffix` and builds two lists: `input` (vehicle) and `output` (simulation). Files with `_r0` in the base filename are treated as outputs; others as inputs.
- `process_logs(data_files)` iterates pairs of input/output CSVs (by index), resets globals, calls `main(input_csv, output_csv)`, then `plot_stats()` and writes HTML reports into `output_folder`.

Important common variables
- `max_num_of_si_to_process` — user-modifiable row limit (0 => no limit).
- `output_folder` — where HTML reports are written.
- Each script uses a small set of globals (lists for timeseries, counters) that are reset per-log inside `process_logs()` before `main()` is called.

Per-script details
------------------

1) tracker_matching_kpi_script.py
- Purpose: compute track-level matching KPIs between vehicle object stream and simulation object stream.
- Input suffix: `_UDP_GEN7_ROT_OBJECT_STREAM.csv`.
- Key steps:
  - Read trkID, position (`vcs_xposn_*`, `vcs_yposn_*`), velocity (`vcs_xvel_*`, `vcs_yvel_*`) and moving flags (`f_moving_*`) for a configurable number of tracks per scan (`max_number_of_data`, default 64).
  - Merge vehicle and sim frames on `scan_index`.
  - Build boolean masks for valid tracks (trkID>0 and within `max_valid_distance`).
  - For each vehicle track, iterate simulation tracks to find the first matching sim track where position and velocity diffs are within thresholds. Thresholds scale by distance: `vcs_xposn_threshold`, `vcs_yposn_threshold`, `vcs_xvel_threshold`, `vcs_yvel_threshold` (scaled by 1×,2×,3× for 10–20m, 20–30m, ...).
  - Count matches (all tracks and moving-only), compute per-scan matching percentages, and aggregate KPIs such as accuracy and counts.
  - Plot accuracy vs `scan_index` and write `tracker_kpi_report_*.html`.
- User-tunable variables inside the script: `use_sample_data`, `max_num_of_si_to_process`, `max_number_of_data`, `max_valid_distance`, and thresholds.

2) tracker_processed_det_kpi_script.py
- Purpose: plot and compare many per-scan processed-detection variables (positions, velocities, boresight angles, FOV flags, etc.).
- Input suffix: `_UDP_GEN7_ROT_PROCESSED_DETECTION_STREAM.csv`.
- Key steps:
  - Read a long list of `vcs_*` columns (e.g. `vcs_long_posn_0`, `vcs_lat_posn_0`, `vcs_long_vel_0`, etc.) for a single-index set (index 0 in the script).
  - Merge vehicle and sim frames on `scan_index`.
  - Produce a large grid of Plotly subplots comparing vehicle vs sim time series for those vcs variables.
  - Write `tracker_processed_det_kpi_report_*.html`.

3) tracker_vehicle_info_kpi_script.py
- Purpose: compare vehicle-state time-series (world coordinates, speed, heading, accelerations, steering, PRNDL flags, etc.).
- Input suffix: `_UDP_GEN7_ROT_VEHICLE_INFO.csv`.
- Key steps:
  - Read vehicle info columns (e.g. `world_x`, `world_y`, `speed`, `heading`, `acceleration`, `steering_angle_rad`,gdsr tracker ,  etc.).
  - Merge on `scan_index` and generate multi-row subplot grids showing vehicle vs sim traces.
  - Write `tracker_vehicle_info_kpi_report_*.html`. 

4) tracker_info_kpi_script.py
- Purpose: report higher-level tracker statistics per scan such as `elapsed_time_s`, `reduced_num_active_objs`, `num_active_clusters`, and `number_of_historic_detections`.
- Input suffix: `_UDP_GEN7_ROT_TRACKER_INFO.csv`.
- Key steps:
  - Read small set of columns per scan, merge on `scan_index` and plot 4 line-plots comparing vehicle vs sim.
  - Write `tracker_info_kpi_report_*.html`.

Input files and naming
---------------------
- `log_path.txt`: a plain text file where the first line is the path to the directory that contains CSV files to process. Example content:

  C:\path\to\csv_directory

- `meta_data.txt`: simple key/value lines separated by whitespace (the scripts read each line and call `key, value = line.strip().split(maxsplit=1)`). Expected keys used in HTML header include `SiL_Engine`, `SW`, `RSP_SiL`, `Tracker`, `Mode`.

- CSV file pairing: each script looks for files whose filename contains the script-specific `file_suffix`. The code builds a set of base names by removing the suffix, sorts them and then treats base names that include `_r0` as simulation `output` files and the others as vehicle `input` files. The lists are zipped by index. If file counts or naming are inconsistent this can lead to mis-pairing — see Troubleshooting.

Execution (example)
-------------------
1. Install dependencies (from the workspace `requirements.txt`):

```powershell
pip install -r requirements.txt
```

2. Create `log_path.txt` with the directory containing CSVs, create a `meta_data.txt` with metadata key/value lines, and run a script, for example:

```powershell
python gen7v2_resim_kpi_scripts/tracker_matching_kpi_script.py log_path.txt meta_data.txt C:\temp\tracker_reports
```

Notes and troubleshooting
-------------------------
- Column names must match exactly what each script expects. If a `pd.read_csv(..., usecols=cols_of_interest)` fails, the CSV does not contain the expected headers.
- The pairing logic in `find_data_files()` assumes base filenames (prefix before the suffix) are unique and that presence of `_r0` denotes simulation outputs. If your files don't follow this convention, adjust `find_data_files()`.
- Large CSVs can consume memory. Use `max_num_of_si_to_process` to limit rows during development.
- The scripts use globals extensively; refactoring to return values from `main()` and pass them explicitly to `plot_stats()` would improve testability and reduce side effects.

Suggested improvements
----------------------
- Use `argparse` for clearer CLI parsing and better help messages.
- Emit a CSV or JSON KPI summary per log in addition to HTML, to make programmatic consumption easier.
- Replace global-state approach with functions that return data objects. This simplifies testing and parallel processing.
- Improve pairing robustness by matching inputs and outputs by a stronger naming convention or by using timestamps inside files.

.dot flow (high-level)
---------------------
Below is a small Graphviz DOT fragment describing the common flow. Save it as `tracker_flow.dot` and render with `dot -Tpng tracker_flow.dot -o tracker_flow.png` if you want a visual.

```dot
digraph TrackerScripts {
  rankdir=LR;
  node [shape=box];
  ReadLogPath -> FindDataFiles;
  FindDataFiles -> ProcessLogs;
  ProcessLogs -> Main;
  Main -> ComputeMetrics;
  ComputeMetrics -> PlotStats;
  PlotStats -> WriteHTML;
}
```

Where to find the scripts
-------------------------
- The scripts are in `gen7v2_resim_kpi_scripts/` in this workspace. See the files listed at the top for direct links.

If you'd like, I can:
- add the `.dot` file to the repository at `gen7v2_resim_kpi_scripts/tracker_flow.dot`, or
- generate a rendered PNG/SVG of the flow and add it to the repo, or
- refactor one script to remove globals and demonstrate a more testable structure.

---
Generated by a repository analysis of the four `tracker_*.py` scripts.
