"""
ScanIndex match metrics - separate pure function for InPlot / UDP-KPI HTML.

Keeps original alignment logic (`_build_aligned_scan_plan`) untouched.
This module computes unbiased match percentages from raw input/output
scan_index arrays without mutating container alignment.

Use this for display only; do NOT wire into storage initialization.
"""

from typing import Dict, Any, List, Optional
import math


def _norm_int_list(arr) -> List[int]:
    if arr is None:
        return []
    try:
        # h5py datasets, numpy arrays, lists -> flat int list
        if hasattr(arr, "tolist"):
            arr = arr.tolist()
        # handle scalar
        if not hasattr(arr, "__iter__") or isinstance(arr, (bytes, str)):
            return [int(arr)]
        out = []
        for v in arr:
            try:
                out.append(int(v))
            except Exception:
                # nested arrays (e.g. [[677]]) -> flatten one level
                try:
                    out.append(int(v[0]))
                except Exception:
                    continue
        return out
    except Exception:
        return []


def calculate_scanindex_match_metrics(
    scan_index_in,
    scan_index_out,
    *,
    exclude_zero: bool = True,
    exclude_negative: bool = False,
) -> Dict[str, Any]:
    """
    Pure diff/match calculator for scan_index. No side-effects on storage.

    Returns detailed match report for HTML:
      - row counts (len including duplicates)
      - unique counts (deduped)
      - common / input-only / output-only (unique)
      - input_match_pct, output_match_pct, jaccard_pct, overlap_pct
      - if exclude_zero: 0 is treated as invalid padding (common in ORCAS HDFs)

    Original code mixing `len(common_unique)/len(rows)` -> ~40-50% for identical
    files. This function keeps both views separate so caller can choose.

    Args:
        scan_index_in, scan_index_out: array-like scan_index from HDF header.
        exclude_zero: if True, drop scan_index == 0 before set math.
        exclude_negative: if True, drop negative scan_index.

    Returns:
        dict with keys: input_total_rows, output_total_rows,
        input_unique, output_unique, common_unique, input_only_unique,
        output_only_unique, union_unique,
        input_match_pct, output_match_pct, jaccard_pct, union_match_pct,
        common_count, input_total, output_total, scan_match_pct (legacy)
        plus filtered lists for debug.
    """
    in_rows = _norm_int_list(scan_index_in)
    out_rows = _norm_int_list(scan_index_out)

    in_total_rows = len(in_rows)
    out_total_rows = len(out_rows)

    def _filter(vals: List[int]) -> List[int]:
        if exclude_zero:
            vals = [v for v in vals if v != 0]
        if exclude_negative:
            vals = [v for v in vals if v >= 0]
        return vals

    in_filt = _filter(in_rows)
    out_filt = _filter(out_rows)

    # unique sets (order preserved for common list)
    in_unique_set = set(in_filt)
    out_unique_set = set(out_filt)

    # preserve order of first appearance in input (like _build_aligned_scan_plan)
    seen = set()
    common_ordered: List[int] = []
    for v in in_filt:
        if v in seen:
            continue
        if v in out_unique_set:
            common_ordered.append(v)
        seen.add(v)

    common_unique = len(common_ordered)
    input_only_unique = len(in_unique_set - out_unique_set)
    output_only_unique = len(out_unique_set - in_unique_set)
    union_unique = len(in_unique_set | out_unique_set)

    # row-level dup counts for diagnostics
    input_unique = len(in_unique_set)
    output_unique = len(out_unique_set)

    def _pct(num, den) -> float:
        if den == 0 or num == 0:
            # 0/0 -> nan, else 0%
            return float("nan") if den == 0 else 0.0
        return 100.0 * float(num) / float(den)

    # unbiased percentages (unique / unique)
    input_match_pct = _pct(common_unique, input_unique)
    output_match_pct = _pct(common_unique, output_unique)
    jaccard_pct = _pct(common_unique, union_unique)  # IoU
    # legacy mixed metric for backward compat (unique/rows) - kept but not for display
    legacy_scan_match_pct = _pct(common_unique, in_total_rows)

    return {
        # row counts (raw, duplicates kept)
        "input_total_rows": int(in_total_rows),
        "output_total_rows": int(out_total_rows),
        # legacy aliases expected by old HTML
        "input_total": int(in_total_rows),
        "output_total": int(out_total_rows),
        # unique views
        "input_unique": int(input_unique),
        "output_unique": int(output_unique),
        "common_unique": int(common_unique),
        "common_count": int(common_unique),
        "common_scan_count": float(common_unique),
        "input_only_unique": int(input_only_unique),
        "input_only_scan_count": float(input_only_unique),
        "output_only_unique": int(output_only_unique),
        "output_only_scan_count": float(output_only_unique),
        "union_unique": int(union_unique),
        # percentages
        "input_match_pct": float(input_match_pct),
        "output_match_pct": float(output_match_pct),
        "jaccard_pct": float(jaccard_pct),
        # legacy field (unique/rows) kept for old consumers
        "scan_match_pct": float(legacy_scan_match_pct),
        # aliases for HTML that expects scan_match_pct to be unique/unique
        "scan_match_pct_unique": float(input_match_pct),
        "avg_scan_match_pct_raw": float(input_match_pct),
        # debug lists
        "common_scan_indices": list(common_ordered),
        "input_only_scan_indices": sorted(list(in_unique_set - out_unique_set)),
        "output_only_scan_indices": sorted(list(out_unique_set - in_unique_set)),
    }


# Convenience alias for caller that wants single pct value
def calculate_scanindex_match_percentage(
    scan_index_in,
    scan_index_out,
    *,
    mode: str = "input",  # input | output | jaccard
    exclude_zero: bool = True,
) -> float:
    """
    Return single percentage for quick HTML injection.
    mode=input  -> common/input_unique (what user expects: 100% for copy)
    mode=output -> common/output_unique
    mode=jaccard -> common/union
    """
    m = calculate_scanindex_match_metrics(
        scan_index_in, scan_index_out, exclude_zero=exclude_zero
    )
    if mode == "output":
        return m["output_match_pct"]
    if mode == "jaccard":
        return m["jaccard_pct"]
    return m["input_match_pct"]
