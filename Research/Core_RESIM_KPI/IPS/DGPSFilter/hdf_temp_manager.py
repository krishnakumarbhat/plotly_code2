"""Helper functions to create and cleanup DGPS-filtered temporary HDF files.
"""
import os
import tempfile
import shutil
from typing import List, Tuple
import h5py
import numpy as np


def create_filtered_temp_hdfs(report_folder: str, in_f: str, out_f: str, dgps_filter) -> Tuple[str, str, List[str]]:
    """Create filtered HDF copies under report_folder/temp_hdf and return
    (in_use, out_use, temp_files).
    
    selects GT source:
    - First tries to load GT from in_f (file1)
    - If in_f has no valid GT data (zero/empty), falls back to out_f (file2)
    """
    temp_dir = os.path.join(report_folder, "temp_hdf")
    os.makedirs(temp_dir, exist_ok=True)

    # Create readable, deterministic temp filenames based on source names.
    def _make_temp_path(src_path: str, tag: str) -> str:
        base = os.path.splitext(os.path.basename(src_path))[0]
        candidate = os.path.join(temp_dir, f"{base}_{tag}.dgps_filtered.h5")
        idx = 0
        while os.path.exists(candidate):
            idx += 1
            candidate = os.path.join(temp_dir, f"{base}_{tag}.dgps_filtered_{idx}.h5")
        # create empty placeholder file
        open(candidate, 'wb').close()
        return candidate

    tmp_in = _make_temp_path(in_f, 'in')
    tmp_out = _make_temp_path(out_f, 'out')

    temp_files: List[str] = []

    with h5py.File(in_f, 'r') as in_hdf, h5py.File(out_f, 'r') as out_hdf:
        input_scans = np.array(in_hdf['01_Scan_Index/scan_index'][()])
        output_scans = np.array(out_hdf['01_Scan_Index/scan_index'][()])
    common_scans = np.intersect1d(input_scans, output_scans)
    dgps_filter.set_common_scan_indices(common_scans)
    print(f"  Common scan count between files: {len(common_scans)}")
 
    
    # Determine GT source: try in_f first, fallback to out_f if no valid GT
    gt_source = in_f
    print(f"  Checking GT in file1: {os.path.basename(in_f)}")
    if dgps_filter.load_gt_from_hdf(in_f) and dgps_filter.has_valid_gt():
        print(f"  ✓ Valid GT found in file1, using as GT source")
        gt_source = in_f
    else:
        print(f"  ⚠ No valid GT in file1, checking file2: {os.path.basename(out_f)}")
        if dgps_filter.load_gt_from_hdf(out_f) and dgps_filter.has_valid_gt():
            print(f"  ✓ Valid GT found in file2, using as GT source")
            gt_source = out_f
        else:
            print(f"  ❌ No valid GT found in either file")
            # cleanup and return originals
            for p in [tmp_in, tmp_out]:
                try:
                    os.remove(p)
                except Exception:
                    pass
            return in_f, out_f, []
    
    # Now filter both files using the determined GT source
    ok1 = dgps_filter.create_filtered_hdf_copy(in_f, tmp_in, gt_hdf=gt_source)
    ok2 = dgps_filter.create_filtered_hdf_copy(out_f, tmp_out, gt_hdf=gt_source)
    if ok1 and ok2:
        in_use = tmp_in
        out_use = tmp_out
        temp_files = [tmp_in, tmp_out]
        return in_use, out_use, temp_files

    # failure path: remove created temp files and fallback to originals
    for p in [tmp_in, tmp_out]:
        try:
            os.remove(p)
        except Exception:
            pass
    return in_f, out_f, []


def cleanup_filtered_temp_hdfs(temp_files: List[str], report_folder: str) -> None:
    """Delete temp files and the temp_hdf directory created under report_folder."""
    for p in temp_files:
        try:
            os.remove(p)
            print(f"  Deleted temp HDF: {p}")
        except Exception as e:
            print(f"  Failed to delete temp HDF {p}: {e}")

    temp_dir = os.path.join(report_folder, "temp_hdf")
    try:
        if os.path.isdir(temp_dir):
            try:
                os.rmdir(temp_dir)
                print(f"  Deleted temp_hdf directory: {temp_dir}")
            except Exception:
                shutil.rmtree(temp_dir, ignore_errors=True)
                print(f"  Removed temp_hdf directory and contents: {temp_dir}")
    except Exception as e:
        print(f"  Failed removing temp_hdf directory: {e}")

    # (DGPS_PLOTS_ENABLED toggles removed - standalone DGPS plotting retired)
