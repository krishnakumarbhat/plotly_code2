import h5py
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
from UDP_KPI.b_data_storage.kpi_config_storage import KPI_ALIGNMENT_CONFIG, KPI_DETECTION_CONFIG, KPI_TRACKER_CONFIG
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

        for stream_idx, stream in enumerate(streams):
            logger.info(f"Processing stream [{stream_idx}] {stream}")
            group_path = f"{sensor}/{stream}"

            # # Skip if OD stream is missing
            if hdf_in is not None and group_path not in hdf_in:
                logger.warning(f"Skipping stream {stream} - OD data not found")
                continue
            # scan_index = ['scan_index','Stream_Hdr_scan_index']
            # # For each KPI stream, attempt to parse from input and output
            # scan_index = None
            # if hdf_in is not None and streams:
            #     data_group_in = hdf_in[group_path]
            #     header_path_in = next((v for v in self.header_variants if v in data_group_in), None)
            #     if header_path_in and f"{header_path_in}/scan_index" in data_group_in:
            #         scan_index = data_group_in[f"{header_path_in}/scan_index"][()]
            
            # Check for both scan_index variants: keep input and output scan indices separate
            scan_index_in = None
            scan_index_out = None

            if hdf_in is not None and streams:
                data_group_in = hdf_in[group_path]
                header_path_in = next((v for v in self.header_variants if v in data_group_in), None)
                if header_path_in:
                    # Try 'scan_index' first, then 'Stream_Hdr_scan_index'
                    for idx_name in ['scan_index', 'Stream_Hdr_scan_index']:
                        idx_path = f"{header_path_in}/{idx_name}"
                        if idx_path in data_group_in:
                            scan_index_in = data_group_in[idx_path][()]
                            break
                if scan_index_in is None:
                    # Fallback: search recursively under stream group
                    scan_index_in = self._find_scan_index_fallback(data_group_in)
                # Also try direct dataset at stream level (some PCAN dumps store scan_index directly)
                if scan_index_in is None and "scan_index" in data_group_in:
                    try:
                        scan_index_in = data_group_in["scan_index"][()]
                    except Exception:
                        pass

            if hdf_out is not None and streams:
                data_group_out = hdf_out[group_path]
                header_path_out = next((v for v in self.header_variants if v in data_group_out), None)
                if header_path_out:
                    for idx_name in ['scan_index', 'Stream_Hdr_scan_index']:
                        idx_path = f"{header_path_out}/{idx_name}"
                        if idx_path in data_group_out:
                            scan_index_out = data_group_out[idx_path][()]
                            break
                if scan_index_out is None:
                    scan_index_out = self._find_scan_index_fallback(data_group_out)
                if scan_index_out is None and "scan_index" in data_group_out:
                    try:
                        scan_index_out = data_group_out["scan_index"][()]
                    except Exception:
                        pass

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
                data_group_out_peek = hdf_out[group_path] if hdf_out is not None and group_path in hdf_out else None
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
            if hdf_in is not None and hdf_out is not None and group_path in hdf_in and group_path in hdf_out:
                # Parse input stream
                data_group_in = hdf_in[group_path]
                header_path_in = next((v for v in self.header_variants if v in data_group_in), None)
                if header_path_in:
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
                data_group_out = hdf_out[group_path]
                header_path_out = next((v for v in self.header_variants if v in data_group_out), None)
                if header_path_out:
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
