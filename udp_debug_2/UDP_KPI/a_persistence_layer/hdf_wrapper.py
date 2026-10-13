import gc
import h5py
import json
import os
import logging
import time
import functools
from typing import Dict, Any, Optional, List
from dataclasses import dataclass

from UDP_KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
try:
    # Optional dependency: some deployments package InteractivePlot alongside KPI.
    from InteractivePlot.d_business_layer.utils import time_taken  # type: ignore
except Exception:
    def time_taken(func):
        @functools.wraps(func)
        def _wrapped(*args, **kwargs):
            start = time.time()
            try:
                return func(*args, **kwargs)
            finally:
                elapsed = time.time() - start
                logger.info("%s took %.3fs", getattr(func, '__name__', 'func'), elapsed)

        return _wrapped
from UDP_KPI.b_data_storage.kpi_config_storage import (
    KPI_ALIGNMENT_CONFIG,
    KPI_DETECTION_CONFIG,
    KPI_TRACKER_CONFIG,
    STREAM_ALIASES,
    canonical_stream_name,
    resolve_actual_stream,
)
from UDP_KPI.a_persistence_layer.kpi_hdf_parser import KPIHDFParser
from UDP_KPI.c_business_layer.kpi_factory import KpiDataModel
from UDP_KPI.d_presentation_layer.kpi_html_gen import generate_kpi_index
from UDP_KPI.runtime_utils import normalize_fs_path, ensure_dir, set_default_umask
# Separate pure diff helper for scan_index match % (does NOT affect _build_aligned_scan_plan)
from UDP_KPI.a_persistence_layer.scan_index_metrics import calculate_scanindex_match_metrics

logger = logging.getLogger(__name__)


@dataclass
class KPIProcessingConfig:
    sensor_id: str
    input_file_path: str
    output_file_path: str
    output_dir: str
    base_name: str
    kpi_subdir: str = "kpi"


class KPIHDFWrapper:
    """Parses KPI-related streams from HDF5 input/output and forwards to KPI factory."""

    def __init__(self, config: KPIProcessingConfig):
        set_default_umask()
        self.config = config
        self.start_time_parsing = time.time()
        self.header_variants = [
            "stream_hdr",
            "stream_Hdr",
            "stream_HDR",
            "stream_header",
            "stream_Header",
            "stream_HEADER",
            "Stream_hdr",
            "Stream_Hdr",
            "Stream_HDR",
            "Stream_header",
            "Stream_Header",
            "Stream_HEADER",
            "STREAM_hdr",
            "STREAM_Hdr",
            "STREAM_HDR",
            "STREAM_header",
            "STREAM_Header",
            "STREAM_HEADER",
            "streamhdr",
            "streamHdr",
            "streamHDR",
            "streamheader",
            "streamHeader",
            "streamHEADER",
            "Streamhdr",
            "StreamHdr",
            "StreamHDR",
            "Streamheader",
            "StreamHeader",
            "StreamHEADER",
            "STREAMhdr",
            "STREAMHdr",
            "STREAMHDR",
            "STREAMheader",
            "STREAMHeader",
            "STREAMHEADER",
            "hdr_stream",
            "hdr_Stream",
            "hdr_STREAM",
            "Hdr_stream",
            "Hdr_Stream",
            "Hdr_STREAM",
            "HDR_stream",
            "HDR_Stream",
            "HDR_STREAM",
            "header_stream",
            "header_Stream",
            "header_STREAM",
            "Header_stream",
            "Header_Stream",
            "Header_STREAM",
            "HEADER_stream",
            "HEADER_Stream",
            "HEADER_STREAM",
            "DRA_Stream_Hdr_T",
        ]
        self.stream_input_model = KPI_DataModelStorage()
        self.stream_output_model = KPI_DataModelStorage()

    @staticmethod
    def _find_scan_index_fallback(group: h5py.Group):
        """Fallback recursive search for scan_index dataset when header variant not found."""
        target_leafs = {"scan_index", "stream_hdr_scan_index", "hed_scan_index", "hed_look_index"}
        found = None
        def _visit(name, obj):
            nonlocal found
            if found is not None:
                return
            if isinstance(obj, h5py.Dataset):
                leaf = name.split("/")[-1].lower()
                if leaf in target_leafs:
                    try:
                        found = obj[()]
                    except Exception:
                        pass
        try:
            group.visititems(_visit)
        except Exception:
            pass
        return found

    @staticmethod
    def _find_all_scan_index_fallbacks(group, max_matches=50, max_elements=1_000_000):
        """Collect EVERY scan_index-like dataset under a stream group.

        The old first-match-wins fallback depended on HDF visit order: for
        FR/Dyn_Align it returned the corrupt header index while the sane
        core index sat one group away. Returning all matches lets the sanity
        picker choose. Reads are bounded (skip datasets > max_elements).
        """
        target_leafs = {"scan_index", "stream_hdr_scan_index", "hed_scan_index", "hed_look_index"}
        found = []

        def _visit(name, obj):
            if len(found) >= max_matches:
                return
            if isinstance(obj, h5py.Dataset):
                leaf = name.split("/")[-1].lower()
                if leaf in target_leafs:
                    try:
                        if obj.size > max_elements:
                            return
                        found.append((name, obj[()]))
                    except Exception:
                        pass

        try:
            group.visititems(_visit)
        except Exception:
            pass
        return found

    def _borrow_scan_index_from_siblings(self, hdf_file, sensor, exclude_stream):
        """Borrow a scan_index from a sibling stream of the same sensor.

        Some logs (e.g. AshokLeyland OBJECT_LIST_STREAM) carry no header of
        their own, but a sibling stream (OLP_STREAM/PARTNER_SENSOR_STREAM/...)
        shares the same per-row cycle. Returns the first usable array or None.
        """
        try:
            if hdf_file is None or sensor not in hdf_file:
                return None, None
            sensor_group = hdf_file[sensor]
            for sibling in sorted(sensor_group.keys()):
                if sibling == exclude_stream:
                    continue
                sib_group = sensor_group[sibling]
                if not isinstance(sib_group, h5py.Group):
                    continue
                header_path = next(
                    (v for v in self.header_variants if v in sib_group), None
                )
                borrowed = None
                if header_path:
                    for idx_name in ['scan_index', 'Stream_Hdr_scan_index']:
                        idx_path = f"{header_path}/{idx_name}"
                        if idx_path in sib_group:
                            try:
                                borrowed = sib_group[idx_path][()]
                                break
                            except Exception:
                                continue
                if borrowed is None:
                    try:
                        borrowed = self._find_scan_index_fallback(sib_group)
                    except Exception:
                        borrowed = None
                if borrowed is not None:
                    return borrowed, sibling
        except Exception:
            pass
        return None, None

    @staticmethod
    def _sanitize_scan_values(arr, hard_max=1 << 24):
        """Return (int list, dropped count), dropping absurd scan values.

        Corrupt headers (e.g. Dyn_Align Stream_Hdr_scan_index with values up
        to ~3.7e9 and stale zeros) must never reach alignment: values <= 0
        or above hard_max (16M; real cycles are < 1M) are discarded.
        Pure Python, bounded by len(arr); never materializes ranges.
        """
        try:
            raw = arr.tolist() if hasattr(arr, "tolist") else list(arr)
        except Exception:
            return [], 0
        vals, dropped = [], 0
        for v in raw:
            try:
                if isinstance(v, (list, tuple)) and len(v) == 1:
                    v = v[0]
                iv = int(v)
            except Exception:
                dropped += 1
                continue
            if iv <= 0 or iv > hard_max:
                dropped += 1
                continue
            vals.append(iv)
        return vals, dropped

    def _pick_sane_scan_index(self, candidates):
        """Choose the sanest scan_index among (label, array) candidates.

        Collects every header variant + recursive fallback; sanitizes absurd
        values; returns the candidate with the smallest span. A corrupt
        header (span ~1e9 over ~200 rows) is rejected in favour of the sane
        core scan_index (span ~200). Returns (values or None, label or None).
        Span check is arithmetic only - no range materialization (that list
        previously grew to tens of GB and OOM-killed the machine).
        """
        best, best_label, best_span = None, None, None
        for label, arr in candidates:
            if arr is None:
                continue
            vals, dropped = self._sanitize_scan_values(arr)
            total = len(vals) + dropped
            if not vals:
                continue
            # Reject degenerate survivors: a corrupt header sanitized down
            # to a singleton (span 0) would otherwise beat every real cycle
            # on min-span alone (FR/Dyn_Align lesson: n=1 chosen over n=192).
            if len(vals) < 10 or (total > 0 and dropped / total > 0.9):
                logger.warning(
                    f"Rejecting scan_index candidate '{label}' "
                    f"(kept {len(vals)}/{total} after sanitization): degenerate"
                )
                continue
            span = max(vals) - min(vals)
            if span > 1_000_000 and span > 100 * len(vals):
                logger.warning(
                    f"Rejecting scan_index candidate '{label}' "
                    f"(n={len(vals)} span={span} dropped={dropped}): implausible cycle"
                )
                continue
            if dropped:
                logger.info(
                    f"Scan_index candidate '{label}': dropped {dropped} absurd values, "
                    f"kept n={len(vals)} span={span}"
                )
            if best is None or span < best_span:
                best, best_label, best_span = vals, label, span
        if best is None:
            return None, None
        return best, best_label

    @staticmethod
    def _summarize_kpi_section(kpi_type, html):
        """Extract (status, headline, detail) from a KPI HTML section.

        Presentation-only extraction for the index summary page; the KPI
        computation itself is untouched. Statuses: ok | failed | missing.
        """
        import re
        if not html:
            return ("missing", "—", "")
        if re.search(r"Failed to process \w+ KPIs", html):
            return ("failed", "failed", "")
        try:
            if kpi_type == "detection":
                m1 = re.search(r"Matched Detections:\s*<span[^>]*>([^<]+)</span>", html, re.S)
                m2 = re.search(r"Accuracy:\s*<span[^>]*>([^<%]+)%?</span>", html, re.S)
                if m1 and m2:
                    return ("ok", m2.group(1).strip() + "%", m1.group(1).strip() + " matched")
            elif kpi_type == "alignment":
                ma = re.search(r"Azimuth Accuracy:</strong>\s*<span[^>]*>([^<]+)</span>", html, re.S)
                me = re.search(r"Elevation Accuracy:</strong>\s*<span[^>]*>([^<]+)</span>", html, re.S)
                ms = re.search(r"Total Scans Processed:</strong>\s*<span[^>]*>([^<]+)</span>", html, re.S)
                if ma and me:
                    az = ma.group(1).strip().split("=")[-1].strip()
                    el = me.group(1).strip().split("=")[-1].strip()
                    scans = ms.group(1).strip() if ms else ""
                    return ("ok", f"Az {az} · El {el}", f"{scans} scans" if scans else "")
            elif kpi_type == "tracker":
                mt = re.search(r"Tracker Accuracy:</strong>\s*\(([^)]+)\)[^<]*<strong>([^<]+)</strong>", html, re.S)
                if mt:
                    return ("ok", mt.group(2).strip(), mt.group(1).strip() + " matched")
        except Exception:
            pass
        return ("failed", "failed", "")

    def _write_diagnostics_sidecar(
        self, sensor_dir, sensor, available_in, available_out,
        stream_resolution, kpi_model, saved_files,
    ):
        """Write KPI/diagnostics.json: stream inventory + per-KPI status.

        Powers the tabbed index page ("why is this KPI missing?").
        Presentation metadata only; never affects KPI computation.
        """
        by_type = {}
        try:
            for item in kpi_model.get_kpi_htmls():
                t = (item or {}).get("type")
                if t and t not in by_type:
                    by_type[t] = (item or {}).get("html_content", "")
        except Exception:
            by_type = {}
        file_by_type = {}
        for p in saved_files or []:
            b = os.path.basename(p)
            for t, suffix in (
                ("alignment", "alignment_kpi"),
                ("detection", "detection_kpi"),
                ("tracker", "tracker_kpi"),
            ):
                if suffix in b and t not in file_by_type:
                    file_by_type[t] = b
        kpis = {}
        for t in ("detection", "alignment", "tracker"):
            section = by_type.get(t)
            if section is None:
                status, headline, detail = ("missing", "—", "")
            else:
                status, headline, detail = self._summarize_kpi_section(t, section)
            kpis[t] = {
                "status": status,
                "headline": headline,
                "detail": detail,
                "file": file_by_type.get(t, ""),
            }
        diag = {
            "sensor": sensor,
            "base_name": self.config.base_name,
            "generated_at": time.strftime("%Y-%m-%d %H:%M:%S"),
            "streams_in": sorted(available_in),
            "streams_out": sorted(available_out),
            "resolution": stream_resolution,
            "kpis": kpis,
        }
        with open(os.path.join(sensor_dir, "diagnostics.json"), "w", encoding="utf-8") as fp:
            json.dump(diag, fp, indent=2)

    @staticmethod
    def _build_aligned_scan_plan(scan_index_in, scan_index_out):
        out_positions = {}
        for out_idx, scan_id in enumerate(scan_index_out):
            out_positions.setdefault(int(scan_id), []).append(out_idx)

        common_scan_index = []
        selected_in_indices = []
        selected_out_indices = []
        seen_scan_ids = set()

        for in_idx, scan_id in enumerate(scan_index_in):
            scan_id_int = int(scan_id)
            if scan_id_int in seen_scan_ids:
                continue

            out_idx_list = out_positions.get(scan_id_int)
            if not out_idx_list:
                continue

            common_scan_index.append(scan_id_int)
            selected_in_indices.append(in_idx)
            selected_out_indices.append(out_idx_list.pop(0))
            seen_scan_ids.add(scan_id_int)

        selected_in_set = set(selected_in_indices)
        selected_out_set = set(selected_out_indices)
        missing_in_indices = [idx for idx in range(len(scan_index_in)) if idx not in selected_in_set]
        missing_out_indices = [idx for idx in range(len(scan_index_out)) if idx not in selected_out_set]

        return (
            common_scan_index,
            selected_in_indices,
            selected_out_indices,
            missing_in_indices,
            missing_out_indices,
        )

    def parse(self) -> Dict[str, Any]:
        """Parse configured KPI streams from input/output HDF5 files and forward to KPI factory."""
        results: Dict[str, Any] = {
            "sensor_id": self.config.sensor_id,
            "base_name": self.config.base_name,
            "processing_time": 0.0,
            "available_streams": [],
            "input_data": {},
            "output_data": {},
            "streams_processed": {},
            "html_report_path": "",
            "saved_files": [],
        }
        # Build per-sensor KPI directory to keep all KPI artifacts grouped per sensor.
        kpi_dir_name = (self.config.kpi_subdir or "KPI").strip() or "KPI"
        sensor_dir = os.path.join(
            ensure_dir(self.config.output_dir),
            self.config.base_name,
            self.config.sensor_id,
            kpi_dir_name,
        )
        os.makedirs(sensor_dir, exist_ok=True)

        # Collect streams from KPI config
        streams: List[str] = []
        streams.extend(list(KPI_ALIGNMENT_CONFIG.keys())) if KPI_ALIGNMENT_CONFIG else None
        streams.extend(list(KPI_DETECTION_CONFIG.keys())) if KPI_DETECTION_CONFIG else None
        streams.extend(list(KPI_TRACKER_CONFIG.keys())) if KPI_TRACKER_CONFIG else None
        results["available_streams"] = streams

        sensor = self.config.sensor_id
        input_path = normalize_fs_path(self.config.input_file_path)
        output_path = normalize_fs_path(self.config.output_file_path)

        # Open HDF5 files (if present)
        hdf_in: Optional[h5py.File] = None
        hdf_out: Optional[h5py.File] = None

        if os.path.exists(input_path):
            hdf_in = h5py.File(input_path, "r")
        else:
            logger.error(f"Input HDF5 not found: {input_path}")

        if os.path.exists(output_path):
            hdf_out = h5py.File(output_path, "r")
        else:
            logger.warning(f"Output HDF5 not found: {output_path}")
        
        # Initialize stream-specific models dictionary
        self.stream_models = {}
        # Keep per-stream scan_index alignment summary for HTML reporting
        self.scan_summaries = {}

        # Resolve concrete HDF stream names per sensor (backward compatible:
        # canonical KPI names accept known log variants, e.g.
        # DYNAMIC_ALIGNMENT_STREAM <- Dyn_Align_STREAM,
        # TRACKER_STREAM <- OBJECT_LIST_STREAM).
        available_in = set(hdf_in[sensor].keys()) if hdf_in is not None and sensor in hdf_in else set()
        available_out = set(hdf_out[sensor].keys()) if hdf_out is not None and sensor in hdf_out else set()
        # Canonical -> concrete stream resolution per side, kept for the
        # diagnostics sidecar ("why is this KPI missing?").
        stream_resolution = {}

        for stream_idx, stream in enumerate(streams):
            logger.info(f"Processing stream [{stream_idx}] {stream}")
            actual_in = resolve_actual_stream(available_in, stream)
            actual_out = resolve_actual_stream(available_out, stream)
            stream_resolution[stream] = {"input": actual_in, "output": actual_out}
            if (actual_in is not None and actual_in != stream) or (
                actual_out is not None and actual_out != stream
            ):
                logger.info(
                    f"Stream {stream} resolved to input={actual_in} output={actual_out} "
                    f"(variants: {STREAM_ALIASES.get(stream, [stream])})"
                )

            # # Skip if OD stream is missing
            if hdf_in is not None and actual_in is None:
                logger.warning(f"Skipping stream {stream} - OD data not found")
                continue
            if hdf_out is not None and actual_out is None:
                logger.warning(f"Skipping stream {stream} - SIM data not found")
                continue
            group_path = f"{sensor}/{actual_in}"
            group_path_out = f"{sensor}/{actual_out}"

            # Check for both scan_index variants: keep input and output scan indices separate
            scan_index_in = None
            scan_index_out = None
            header_path_in = None
            header_path_out = None

            if hdf_in is not None and streams:
                data_group_in = hdf_in[group_path]
                header_path_in = next((v for v in self.header_variants if v in data_group_in), None)
                # Gather EVERY candidate, then pick the sane one: corrupt
                # headers (Dyn_Align garbage up to ~3.7e9) must lose to the
                # sane core scan_index instead of poisoning alignment.
                candidates_in = []
                if header_path_in:
                    # Try 'scan_index' first, then 'Stream_Hdr_scan_index'
                    for idx_name in ['scan_index', 'Stream_Hdr_scan_index']:
                        idx_path = f"{header_path_in}/{idx_name}"
                        if idx_path in data_group_in:
                            try:
                                candidates_in.append((idx_path, data_group_in[idx_path][()]))
                            except Exception:
                                pass
                try:
                    for _rel, _arr in self._find_all_scan_index_fallbacks(data_group_in):
                        candidates_in.append((f"recursive:{_rel}", _arr))
                except Exception:
                    pass
                # Also try direct dataset at stream level (some PCAN dumps store scan_index directly)
                if "scan_index" in data_group_in:
                    try:
                        candidates_in.append(("direct/scan_index", data_group_in["scan_index"][()]))
                    except Exception:
                        pass
                scan_index_in, _src_in = self._pick_sane_scan_index(candidates_in)
                if scan_index_in is not None and _src_in:
                    logger.info(f"Stream {stream} input scan_index source: {_src_in}")

            if hdf_out is not None and streams:
                data_group_out = hdf_out[group_path_out]
                header_path_out = next((v for v in self.header_variants if v in data_group_out), None)
                candidates_out = []
                if header_path_out:
                    for idx_name in ['scan_index', 'Stream_Hdr_scan_index']:
                        idx_path = f"{header_path_out}/{idx_name}"
                        if idx_path in data_group_out:
                            try:
                                candidates_out.append((idx_path, data_group_out[idx_path][()]))
                            except Exception:
                                pass
                try:
                    for _rel, _arr in self._find_all_scan_index_fallbacks(data_group_out):
                        candidates_out.append((f"recursive:{_rel}", _arr))
                except Exception:
                    pass
                if "scan_index" in data_group_out:
                    try:
                        candidates_out.append(("direct/scan_index", data_group_out["scan_index"][()]))
                    except Exception:
                        pass
                scan_index_out, _src_out = self._pick_sane_scan_index(candidates_out)
                if scan_index_out is not None and _src_out:
                    logger.info(f"Stream {stream} output scan_index source: {_src_out}")

            # Sibling fallback: ONLY for truly header-less streams
            # (e.g. OBJECT_LIST_STREAM). Borrowing a different-cycle sibling
            # for a stream that HAS headers would silently misalign rows
            # (FR/Dyn_Align lesson: 772-row DETECTION cycle over 192-row data
            # parses to empty). A skipped stream is more honest than a
            # misaligned one.
            if scan_index_in is None and header_path_in is None and hdf_in is not None:
                _borrowed_in, donor_in = self._borrow_scan_index_from_siblings(
                    hdf_in, sensor, actual_in
                )
                if _borrowed_in is not None:
                    scan_index_in, _bsrc = self._pick_sane_scan_index(
                        [(f"sibling:{sensor}/{donor_in}", _borrowed_in)]
                    )
                    if scan_index_in is not None:
                        logger.info(
                            f"Stream {stream}: borrowed input scan_index from sibling {sensor}/{donor_in}"
                        )
            if scan_index_out is None and header_path_out is None and hdf_out is not None:
                _borrowed_out, donor_out = self._borrow_scan_index_from_siblings(
                    hdf_out, sensor, actual_out
                )
                if _borrowed_out is not None:
                    scan_index_out, _bsrc = self._pick_sane_scan_index(
                        [(f"sibling:{sensor}/{donor_out}", _borrowed_out)]
                    )
                    if scan_index_out is not None:
                        logger.info(
                            f"Stream {stream}: borrowed output scan_index from sibling {sensor}/{donor_out}"
                        )

            # Skip if either scan_index is None
            if scan_index_in is None or scan_index_out is None:
                logger.warning(f"Skipping stream {stream} - No valid scan index found for input or output")
                continue

            # Initialize stream-specific models only if we have valid scan indices
            self.stream_models[stream] = {
                'input': KPI_DataModelStorage(),
                'output': KPI_DataModelStorage()
            }



            (
                common_scan_index,
                selected_in_indices,
                selected_out_indices,
                missing_in_indices,
                missing_out_indices,
            ) = self._build_aligned_scan_plan(scan_index_in, scan_index_out)

            # Log details for debugging
            logger.debug(
                f"Stream {stream}: common_count={len(common_scan_index)}, "
                f"input_rows={len(selected_in_indices)}, output_rows={len(selected_out_indices)}, "
                f"missing_in_indices={missing_in_indices}, missing_out_indices={missing_out_indices}"
            )

            # Persist scan_index match summary for downstream KPI/HTML
            # --- keep original alignment for storage (no change to _build_aligned_scan_plan) ---
            # Separate pure calculator for display % -> unbiased unique/unique metric
            try:
                input_total = int(len(scan_index_in)) if scan_index_in is not None else 0
                output_total = int(len(scan_index_out)) if scan_index_out is not None else 0
                common_count = int(len(common_scan_index))
                input_only_count = int(input_total - len(selected_in_indices))
                output_only_count = int(output_total - len(selected_out_indices))
                scan_match_pct = (100.0 * common_count / input_total) if input_total > 0 else float("nan")
                # New isolated diff metrics (does not interfere with above legacy keys)
                isolated_metrics = calculate_scanindex_match_metrics(
                    scan_index_in, scan_index_out, exclude_zero=True
                )
                # Build summary preserving legacy keys for backward compat
                self.scan_summaries[stream] = {
                    # legacy keys (rows vs unique mixed) - kept for old HTML consumers
                    "common_scan_count": float(common_count),
                    "input_only_scan_count": float(input_only_count),
                    "output_only_scan_count": float(output_only_count),
                    "input_total": float(input_total),
                    "output_total": float(output_total),
                    "common_count": float(common_count),
                    "scan_match_pct": float(scan_match_pct),
                    "common_scan_indices": list(common_scan_index),
                    # isolated unbiased metrics (unique/unique) for new display
                    "input_unique": float(isolated_metrics["input_unique"]),
                    "output_unique": float(isolated_metrics["output_unique"]),
                    "common_unique": float(isolated_metrics["common_unique"]),
                    "union_unique": float(isolated_metrics["union_unique"]),
                    "input_match_pct": float(isolated_metrics["input_match_pct"]),
                    "output_match_pct": float(isolated_metrics["output_match_pct"]),
                    "jaccard_pct": float(isolated_metrics["jaccard_pct"]),
                    # prefer unbiased for new HTML titles
                    "scan_match_pct_unique": float(isolated_metrics["input_match_pct"]),
                    "avg_scan_match_pct_raw": float(isolated_metrics["input_match_pct"]),
                    # also expose filtered common list from isolated calc for debugging
                    "common_scan_indices_isolated": list(isolated_metrics["common_scan_indices"]),
                }
                logger.info(
                    f"Stream {stream} scanindex match legacy: {common_count}/{input_total} = {scan_match_pct:.2f}% "
                    f"| isolated unbiased: {isolated_metrics['common_unique']}/{isolated_metrics['input_unique']} = {isolated_metrics['input_match_pct']:.2f}% "
                    f"(input_only_unique={isolated_metrics['input_only_unique']}, output_only_unique={isolated_metrics['output_only_unique']}, jaccard={isolated_metrics['jaccard_pct']:.2f}%)"
                )
            except Exception as e:
                logger.debug(f"Failed to build scan summary for {stream}: {e}")
                self.scan_summaries[stream] = {}

            # --- Adaptive selection for heterogeneous HDFs (CCA 283 vs R11 570) ---
            # Datasets may be stored by raw scan_index length (1150) or by unique count (570).
            # Raw selected indices (positions in 1150 array) fail when dataset is 570 (many >570).
            # Detect dataset row count and remap to unique order if needed.
            def _peek_dataset_len(group, header_path):
                # try to find a representative signal dataset length for this stream
                try:
                    # search for first dataset under stream group excluding header
                    for sub_name, sub_obj in group.items():
                        if isinstance(sub_obj, h5py.Group) and sub_name != header_path:
                            for ds_name, ds_obj in sub_obj.items():
                                if isinstance(ds_obj, h5py.Dataset):
                                    try:
                                        return int(ds_obj.shape[0]) if len(ds_obj.shape) >=1 else 0
                                    except Exception:
                                        continue
                    # fallback: scan_index length itself
                    return 0
                except Exception:
                    return 0

            try:
                data_group_in_peek = hdf_in[group_path] if hdf_in is not None and group_path in hdf_in else None
                data_group_out_peek = hdf_out[group_path_out] if hdf_out is not None and group_path_out in hdf_out else None
                header_path_in_peek = next((v for v in self.header_variants if v in data_group_in_peek), None) if data_group_in_peek is not None else None
                header_path_out_peek = next((v for v in self.header_variants if v in data_group_out_peek), None) if data_group_out_peek is not None else None
                ds_len_in = _peek_dataset_len(data_group_in_peek, header_path_in_peek) if data_group_in_peek is not None else 0
                ds_len_out = _peek_dataset_len(data_group_out_peek, header_path_out_peek) if data_group_out_peek is not None else 0
                raw_len_in = int(len(scan_index_in)) if scan_index_in is not None else 0
                raw_len_out = int(len(scan_index_out)) if scan_index_out is not None else 0
                uniq_len_in = len(set(int(x) for x in scan_index_in)) if scan_index_in is not None else 0
                uniq_len_out = len(set(int(x) for x in scan_index_out)) if scan_index_out is not None else 0

                # Build unique order maps (first appearance order) for remapping
                def _build_unique_order(scan_arr):
                    seen = set()
                    order = []
                    mp = {}
                    for v in scan_arr:
                        iv = int(v)
                        if iv not in seen:
                            mp[iv] = len(order)
                            order.append(iv)
                            seen.add(iv)
                    return order, mp

                in_unique_order, in_map = _build_unique_order(scan_index_in) if scan_index_in is not None else ([], {})
                out_unique_order, out_map = _build_unique_order(scan_index_out) if scan_index_out is not None else ([], {})

                # Decide selection indices for storage (must match dataset row count)
                # For each side, if dataset len == raw len -> keep raw selected, if == uniq len -> use unique-mapped
                sel_in = selected_in_indices
                sel_out = selected_out_indices
                if ds_len_in and ds_len_in != raw_len_in and ds_len_in == uniq_len_in:
                    # remap common -> unique indices for input
                    sel_in = [in_map[c] for c in common_scan_index if c in in_map]
                    logger.info(f"Stream {stream} input dataset {ds_len_in} == uniq {uniq_len_in} != raw {raw_len_in} -> remapped selected_in to unique indices len {len(sel_in)}")
                if ds_len_out and ds_len_out != raw_len_out and ds_len_out == uniq_len_out:
                    sel_out = [out_map[c] for c in common_scan_index if c in out_map]
                    logger.info(f"Stream {stream} output dataset {ds_len_out} == uniq {uniq_len_out} != raw {raw_len_out} -> remapped selected_out to unique indices len {len(sel_out)}")
                # If dataset len is neither, keep raw but storage will truncate gracefully
            except Exception as _e:
                logger.debug(f"Adaptive selection peek failed for {stream}: {_e}")
                sel_in = selected_in_indices
                sel_out = selected_out_indices

            # Initialize models with aligned common scan indices and per-side row selection.
            self.stream_models[stream]['input'].initialize(
                common_scan_index,
                sensor,
                missing_idx=missing_in_indices,
                selected_idx=sel_in,
            )
            self.stream_models[stream]['output'].initialize(
                common_scan_index,
                sensor,
                missing_idx=missing_out_indices,
                selected_idx=sel_out,
            )

            # Set stream-specific parent 
            self.stream_models[stream]['input'].init_parent(stream)
            self.stream_models[stream]['output'].init_parent(stream)

            # Attach scan summary to storages and stream_models for KPI layer
            summary = self.scan_summaries.get(stream, {})
            if summary:
                try:
                    self.stream_models[stream]['input']._scan_summary = summary
                    self.stream_models[stream]['output']._scan_summary = summary
                    self.stream_models[stream]['scan_summary'] = summary
                except Exception:
                    pass


            # # Expose per-stream storages on the wrapper for downstream access/debugging
            # safe_attr = stream.replace("/", "_").replace(" ", "_")
            # setattr(self, f"{safe_attr}_input_storage", stream_input_storage)
            # setattr(self, f"{safe_attr}_output_storage", stream_output_storage)
            # # Also store in a single dict for easy access by stream name
            # self.per_stream_storages[stream] = {
            #     "input": stream_input_storage,
            #     "output": stream_output_storage,
            # }

            # # Update stream processing status
            # results["streams_processed"][stream] = {
            #     "input_available": bool(scan_ind is not None and group_path in hdf_in),
            #     "output_available": bool(scan_ind is not None and group_path in hdf_out),
            #     "both_available": bool(scan_ind is not None and group_path in hdf_in and group_path in hdf_out),
            # }

            # # Set stream-specific parent with bracket notation for stream separation
            # self.stream_input_model.init_parent(stream)
            # self.stream_output_model.init_parent(stream)
            # # Also set parent on per-stream storages
            # stream_input_storage.init_parent(stream)
            # stream_output_storage.init_parent(stream)





            # Parse input and output files for this specific stream
            # NOTE: header-less streams (e.g. OBJECT_LIST_STREAM with only
            # F360_Log_Header/F360_Object_Log) are still parsed: the parser
            # stores only known signal names and ignores the rest.
            if hdf_in is not None and hdf_out is not None and group_path in hdf_in and group_path_out in hdf_out:
                # Parse input stream
                data_group_in = hdf_in[group_path]
                try:
                    a = time.time()
                    # Parse into stream-specific model
                    self.stream_models[stream]['input'] = KPIHDFParser.parse(
                        data_group_in, self.stream_models[stream]['input'], self.header_variants
                    )
                    b = time.time()
                    logger.debug(f"Parsed input stream {stream} in {b - a:.3f}s")
                except Exception as e:
                    import traceback as _tb
                    logger.error(f"Error parsing input stream {stream}: {e}\n{_tb.format_exc()}")

                # Parse output stream
                data_group_out = hdf_out[group_path_out]
                try:
                    a = time.time()
                    # Parse into stream-specific model
                    self.stream_models[stream]['output'] = KPIHDFParser.parse(
                        data_group_out, self.stream_models[stream]['output'], self.header_variants
                    )
                    b = time.time()
                    logger.debug(f"Parsed output stream {stream} in {b - a:.3f}s")
                except Exception as e:
                    import traceback as _tb
                    logger.error(f"Error parsing output stream {stream}: {e}\n{_tb.format_exc()}")




        # After parsing all streams, combine them into the global models
        self.stream_input_model = KPI_DataModelStorage()
        self.stream_output_model = KPI_DataModelStorage()
        
        for stream, models in self.stream_models.items():
            # Here you would need to implement a way to combine models
            # This is a simplified example - you'll need to adapt this to your specific needs
            if hasattr(self.stream_input_model, '_data_container') and hasattr(models['input'], '_data_container'):
                self.stream_input_model._data_container.update(models['input']._data_container)
            if hasattr(self.stream_output_model, '_data_container') and hasattr(models['output'], '_data_container'):
                self.stream_output_model._data_container.update(models['output']._data_container)


        # Also expose aggregated scan_summaries via stream_models for global access
        # e.g., detection KPI can read data['DETECTION_STREAM']['scan_summary']
        # and overall results dict keeps it for debugging
        results["scan_summaries"] = self.scan_summaries
        # Attach as attribute on overall models dict
        try:
            self.stream_models["_scan_summaries"] = self.scan_summaries
        except Exception:
            pass

        kpi_model = KpiDataModel(
            self.stream_models,
            sensor
        )
        # Save individual KPI HTML files under sensor directory using base_name
        def wrap_full_html(sensor_id: str, section_html: str) -> str:
            return f"""
            <!DOCTYPE html>
            <html lang=\"en\">
            <head>
                <meta charset=\"UTF-8\">
                <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
                <title>KPI Report - {sensor_id}</title>
                <script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>
                <style>
                    body {{ font-family: Arial, sans-serif; margin: 20px; }}
                </style>
            </head>
            <body>
                {section_html}
            </body>
            </html>
            """

        saved_files: List[str] = []
        try:
            for item in kpi_model.get_kpi_htmls():
                kpi_type = (item or {}).get('type')
                section = (item or {}).get('html_content')
                if not section or not kpi_type:
                    continue
                suffix = None
                if kpi_type == 'alignment':
                    suffix = 'alignment_kpi'
                elif kpi_type == 'detection':
                    suffix = 'detection_kpi'
                elif kpi_type == 'tracker':
                    suffix = 'tracker_kpi'
                else:
                    # Skip other KPI types for now
                    continue
                file_path = os.path.join(sensor_dir, f"{self.config.base_name}_{suffix}.html")
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(wrap_full_html(self.config.sensor_id, section))
                saved_files.append(file_path)
            results["saved_files"] = saved_files
            # For backward compatibility, point html_report_path to a saved file if available
            preferred = next((p for p in saved_files if p.endswith("alignment_kpi.html")), None)
            results["html_report_path"] = preferred or (saved_files[0] if saved_files else "")
            # Diagnostics sidecar for the tabbed index page (stream
            # inventory + per-KPI status/reasons). Best-effort, no KPI impact.
            try:
                self._write_diagnostics_sidecar(
                    sensor_dir, sensor, available_in, available_out,
                    stream_resolution, kpi_model, saved_files,
                )
            except Exception as _e:
                logger.debug(f"Diagnostics sidecar skipped: {_e}")
            # Generate base index page linking all KPI HTMLs for this run
            try:
                index_path = generate_kpi_index(self.config.output_dir, self.config.base_name)
                results["kpi_index_path"] = index_path
            except Exception as _:
                # Index generation is best-effort; do not fail parsing if it errors
                pass
        except Exception as e:
            logger.error(f"Error saving per-KPI HTML files: {e}")

        if hdf_in is not None:
            try:
                hdf_in.close()
            except Exception:
                pass
        if hdf_out is not None:
            try:
                hdf_out.close()
            except Exception:
                pass

        results["processing_time"] = time.time() - self.start_time_parsing
        logger.info(
            "KPI parsing complete for sensor=%s base=%s in %.3fs",
            self.config.sensor_id,
            self.config.base_name,
            results["processing_time"],
        )
        # Memory hygiene: storages hold rounded copies of every parsed HDF
        # dataset. Drop references and force collection so RSS does not grow
        # across sensors in one process (OOMs observed at 13-31GB on 32GB
        # machines with a tiny pagefile when runs overlapped/accumulated).
        try:
            self.stream_models.clear()
            self.scan_summaries.clear()
            self.stream_input_model = None
            self.stream_output_model = None
        except Exception:
            pass
        gc.collect()
        return results


@time_taken
def parse_for_kpi(
    sensor_id: str,
    input_file_path: str,
    output_dir: str,
    base_name: str,
    kpi_subdir: str,
    output_file_path: str,
) -> str:
    set_default_umask()
    input_file_path = normalize_fs_path(input_file_path)
    output_file_path = normalize_fs_path(output_file_path)
    output_dir = ensure_dir(output_dir)
    if not all([sensor_id, input_file_path, output_file_path, output_dir, base_name]):
        raise ValueError(
            "Missing required fields: sensor_id, input_file_path, output_file_path, output_dir, base_name"
        )

    wrapper = KPIHDFWrapper(
        KPIProcessingConfig(
            sensor_id=sensor_id,
            input_file_path=input_file_path,
            output_file_path=output_file_path,
            output_dir=output_dir,
            base_name=base_name,
            kpi_subdir=kpi_subdir,
        )
    )
    results = wrapper.parse()
    return results.get("html_report_path", "")
