import h5py
import os
import logging
import time
import functools
from typing import Dict, Any, Optional, List
from dataclasses import dataclass

from KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
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
from KPI.b_data_storage.kpi_config_storage import KPI_ALIGNMENT_CONFIG, KPI_DETECTION_CONFIG, KPI_TRACKER_CONFIG
from KPI.a_persistence_layer.kpi_hdf_parser import KPIHDFParser
from KPI.c_business_layer.kpi_factory import KpiDataModel
from KPI.d_presentation_layer.kpi_html_gen import generate_kpi_index

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
        self.config = config
        self.start_time_parsing = time.time()
        self.header_variants = [
            "Stream_Hdr",
            "stream_hdr",
            "StreamHdr",
            "STREAM_HDR",
            "streamheader",
            "stream_header",
            "HEADER_STREAM",
            "DRA_Stream_Hdr_T",
        ]
        self.stream_input_model = KPI_DataModelStorage()
        self.stream_output_model = KPI_DataModelStorage()

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
        # Build sensor directory (no 'kpi' subfolder)
        sensor_dir = os.path.join(
            self.config.output_dir,
            self.config.base_name,
            self.config.sensor_id,
        )
        os.makedirs(sensor_dir, exist_ok=True)

        # Collect streams from KPI config
        streams: List[str] = []
        streams.extend(list(KPI_ALIGNMENT_CONFIG.keys())) if KPI_ALIGNMENT_CONFIG else None
        streams.extend(list(KPI_DETECTION_CONFIG.keys())) if KPI_DETECTION_CONFIG else None
        streams.extend(list(KPI_TRACKER_CONFIG.keys())) if KPI_TRACKER_CONFIG else None
        results["available_streams"] = streams

        sensor = self.config.sensor_id
        input_path = self.config.input_file_path
        output_path = self.config.output_file_path

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

            if hdf_out is not None and streams:
                data_group_out = hdf_out[group_path]
                header_path_out = next((v for v in self.header_variants if v in data_group_out), None)
                if header_path_out:
                    for idx_name in ['scan_index', 'Stream_Hdr_scan_index']:
                        idx_path = f"{header_path_out}/{idx_name}"
                        if idx_path in data_group_out:
                            scan_index_out = data_group_out[idx_path][()]
                            break

            # Skip if either scan_index is None
            if scan_index_in is None or scan_index_out is None:
                logger.warning(f"Skipping stream {stream} - No valid scan index found for input or output")
                continue

            # Initialize stream-specific models only if we have valid scan indices
            self.stream_models[stream] = {
                'input': KPI_DataModelStorage(),
                'output': KPI_DataModelStorage()
            }



            # Convert lists to sets for comparison
            set_in = set(scan_index_in)
            set_out = set(scan_index_out)

            # Find missing and common scan indices
            missing = sorted((set_in - set_out) | (set_out - set_in))
            common_scan_index = sorted(set_in & set_out)

            # Get indices of missing scan indices in their original arrays
            missing_in_indices = [i for i, sid in enumerate(scan_index_in) if sid in missing]
            missing_out_indices = [i for i, sid in enumerate(scan_index_out) if sid in missing]

            # Log details for debugging
            logger.debug(
                f"Stream {stream}: missing_in={missing} (indices: {missing_in_indices}), "
                f"missing_out={missing} (indices: {missing_out_indices}), "
                f"common_count={len(common_scan_index)}"
            )

            # Initialize models with common scan indices and per-side missing sets
            self.stream_models[stream]['input'].initialize(common_scan_index, sensor, missing_idx=missing_in_indices)
            self.stream_models[stream]['output'].initialize(common_scan_index, sensor, missing_idx=missing_out_indices)

            # Set stream-specific parent 
            self.stream_models[stream]['input'].init_parent(stream)
            self.stream_models[stream]['output'].init_parent(stream)


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
                        logger.error(f"Error parsing input stream {stream}: {e}")
                
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
                        logger.error(f"Error parsing output stream {stream}: {e}")




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
