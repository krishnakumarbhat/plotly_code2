"""Scan alignment helpers.

Purpose: align input/output storages by scan index or absolute time.
Inputs : two KPI_DataModelStorage objects (or scan arrays).
Outputs: (common scan values, input rows, output rows).
"""

from typing import Dict, List

import numpy as np

from can_inplot._02_kpi.storage import KPI_DataModelStorage


def align_storage_rows_by_scanindex(
    input_storage: KPI_DataModelStorage,
    output_storage: KPI_DataModelStorage,
) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Purpose: align two storages by equal scan index values.
    Inputs : input and output storage objects.
    Outputs: (common values, input rows, output rows)."""
    in_scan = input_storage.get_scan_index()
    out_scan = output_storage.get_scan_index()
    if len(in_scan) == 0 or len(out_scan) == 0:
        empty = np.array([], dtype=np.int64)
        return empty, empty, empty
    return _align_scan_only(in_scan, out_scan)


def align_storage_rows_by_time(
    input_storage: KPI_DataModelStorage,
    output_storage: KPI_DataModelStorage,
    time_tolerance_ns: int = 2_000_000,
) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Purpose: align storages by absolute time within a tolerance window.
    Inputs : storages and tolerance in nanoseconds.
    Outputs: (common values, input rows, output rows); empty when no time data."""
    in_scan = input_storage.get_scan_index()
    out_scan = output_storage.get_scan_index()
    in_time = input_storage.get_time_ns()
    out_time = output_storage.get_time_ns()
    if len(in_scan) == 0 or len(out_scan) == 0:
        empty = np.array([], dtype=np.int64)
        return empty, empty, empty
    if len(in_time) == 0 or len(out_time) == 0:
        return _align_scan_only(in_scan, out_scan)
    return _align_by_time(
        in_scan, out_scan, in_time, out_time, int(time_tolerance_ns)
    )


def _align_scan_only(
    in_scan: np.ndarray, out_scan: np.ndarray
) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Purpose: align scan arrays by matching ids (duplicates consumed once).
    Inputs : two int64 scan arrays.
    Outputs: (common values, input rows, output rows)."""
    out_positions: Dict[int, List[int]] = {}
    for out_idx, scan in enumerate(out_scan):
        out_positions.setdefault(int(scan), []).append(out_idx)
    common_vals: List[int] = []
    in_rows: List[int] = []
    out_rows: List[int] = []
    for in_idx, scan in enumerate(in_scan):
        pos_list = out_positions.get(int(scan))
        if not pos_list:
            continue
        out_rows.append(pos_list.pop(0))
        common_vals.append(int(scan))
        in_rows.append(in_idx)
    return (
        np.asarray(common_vals, dtype=np.int64),
        np.asarray(in_rows, dtype=np.int64),
        np.asarray(out_rows, dtype=np.int64),
    )


def _align_by_time(
    in_scan: np.ndarray,
    out_scan: np.ndarray,
    in_time: np.ndarray,
    out_time: np.ndarray,
    tol_ns: int,
) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Purpose: merge-scan alignment on absolute timestamps.
    Inputs : scan arrays, time arrays, tolerance.
    Outputs: (common values, input rows, output rows)."""
    n_in = min(len(in_scan), len(in_time))
    n_out = min(len(out_scan), len(out_time))
    common_vals: List[int] = []
    in_rows: List[int] = []
    out_rows: List[int] = []
    i = 0
    j = 0
    while i < n_in and j < n_out:
        dt = int(in_time[i]) - int(out_time[j])
        if abs(dt) <= tol_ns:
            common_vals.append(int(in_scan[i]))
            in_rows.append(i)
            out_rows.append(j)
            i += 1
            j += 1
        elif dt < 0:
            i += 1
        else:
            j += 1
    return (
        np.asarray(common_vals, dtype=np.int64),
        np.asarray(in_rows, dtype=np.int64),
        np.asarray(out_rows, dtype=np.int64),
    )