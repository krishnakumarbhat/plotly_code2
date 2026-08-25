import h5py
import os
import logging
import time
import re
import math
import shutil
import tempfile
import numpy as np
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
from typing import Dict, List
import plotly.graph_objects as go
from InteractivePlot.b_persistence_layer import hdf_parser
from InteractivePlot.d_business_layer.data_prep import DataPrep
from InteractivePlot.b_persistence_layer.Persensor_hdf_parser import PersensorHdfParser
from InteractivePlot.b_persistence_layer.prerun_hdf_parser import PreRun
from InteractivePlot.c_data_storage.data_model_storage import DataModelStorage
from InteractivePlot.c_data_storage.config_loader import get_stream_plot_config
from InteractivePlot.d_business_layer.utils import time_taken
from InteractivePlot.kpi_client.can_kpi_integration import CanKpiIntegration

class AllsensorHdfParser(PersensorHdfParser):
    """Parser for HDF5 files based on an address map and customer type."""

    def __init__(self, address_map: Dict[str, str], output_dir=None):
        """
        Initialize the AllsensorHdfParser.

        Args:
            address_map: Dictionary mapping input file paths to output file paths
            output_dir: Directory to save HTML reports
        """
        super().__init__(address_map, output_dir)
        self.start_time_parsing = time.time()
        
    @time_taken
    def parse(self) -> List[DataPrep]:
        """
        Parse input and output HDF5 files based on the address map.
        (sensor, stream) combinations are processed in parallel with a
        thread pool; h5py/numpy/plotly release the GIL, so threads avoid
        the serialization issues of process pools while still parallelizing.

        Returns:
            List[DataPrep]: List of DataPrep objects for each sensor and stream
        """

        for input_file, output_file in self.address_map.items():
            global base_name, base_name_out, hdf_file_in, hdf_file_out
            base_name = os.path.basename(input_file).split(".")[0]
            base_name_out = os.path.basename(output_file).split(".")[0]

            logging.info(f"\nProcessing input file: {os.path.basename(input_file)}")
            logging.info(f"Processing output file: {os.path.basename(output_file)} \n")
            prerun_result = PreRun(input_file, output_file)
            missing_data = prerun_result.missing_data
            sensor_list = prerun_result.sensor_list
            streams = prerun_result.streams

            if getattr(prerun_result, "read_error", None):
                logging.warning(
                    "Skipping file pair due to unreadable HDF (%s -> %s): %s",
                    os.path.basename(input_file),
                    os.path.basename(output_file),
                    prerun_result.read_error,
                )
                self._write_read_error_placeholder_report(
                    base_name=base_name,
                    input_file=input_file,
                    output_file=output_file,
                    read_error=str(prerun_result.read_error),
                )
                continue

            if missing_data:
                logging.info(
                    f"Warning: Missing data if in input not in output or viceversa : {missing_data}"
                )

            logging.info(f"Found {len(sensor_list)} sensors: {', '.join(sensor_list)}")
            logging.info(f"Found {len(streams)} streams: {', '.join(streams)}")

            self._run_sil_artifacts_once(input_file, output_file, base_name)

            # Actual HDF group names for sensors and streams can differ from the
            # canonical keys (e.g. canonical "CEER_FL/DETECTION_001_004" maps to
            # "MCIP_FL/SRR_FL_DETECTION_001_004").  Keep the PreRun maps so
            # _process_stream can resolve real group paths.
            self.stream_map_in = getattr(prerun_result, "stream_map_in", {}) or {}
            self.stream_map_out = getattr(prerun_result, "stream_map_out", {}) or {}

            # Start KPI service early and notify with required metadata (non-blocking)

            total_combinations = len(sensor_list) * len(streams)
            processed = 0

            try:
                hdf_file_in = h5py.File(input_file, "r")
            except Exception as e:
                print(f"\nError processing input file {str(e)}")

            try:
                hdf_file_out = h5py.File(output_file, "r")
            except Exception as e:
                print(f"\nError processing output file {str(e)}")

            max_workers = min(12, os.cpu_count() or 1)
            logging.info(
                "Processing %d sensor/stream combinations with %d worker threads",
                total_combinations,
                max_workers,
            )
            with ThreadPoolExecutor(
                max_workers=max_workers,
                thread_name_prefix="stream-worker",
            ) as executor:
                for sensor in sensor_list:
                    if os.environ.get('INTERACTIVE_PLOT_ENABLE_KPI', '0') == '1':
                        try:
                            kpi_client = CanKpiIntegration(
                                base_name=base_name,
                                sensor=sensor,
                                input_file=input_file,
                                output_file=output_file,
                                output_dir=self.output_dir,
                            )
                            reply = getattr(kpi_client, "last_reply", None)
                            logging.info(
                                "KPI request sent for sensor=%s status=%s message=%s",
                                sensor,
                                getattr(reply, "status", "sent"),
                                getattr(reply, "message", ""),
                            )
                        except Exception as exc:
                            logging.warning("Failed to send KPI request for sensor %s: %s", sensor, exc)

                    html_name = f"{base_name}_{sensor}.html"
                    # Actual HDF group names can differ between input and output
                    # files (e.g. input "CEER_ FL" vs output "CEER_FL").
                    sensor_in = getattr(prerun_result, "sensor_map_in", {}).get(sensor, sensor)
                    sensor_out = getattr(prerun_result, "sensor_map_out", {}).get(sensor, sensor)

                    sensor_futures = []
                    for stream in streams:
                        if get_stream_plot_config(stream):
                            processed += 1
                            progress = (processed / total_combinations) * 100
                            print(f"\rProcessing...... [{progress:.1f}%]", end="")
                            sensor_futures.append(
                                executor.submit(
                                    self._process_stream,
                                    sensor,
                                    sensor_in,
                                    sensor_out,
                                    stream,
                                )
                            )

                    stream_results = []
                    for future in sensor_futures:
                        result = future.result()
                        if result is not None:
                            stream_results.append(result)

                    # Build one merged plot set per sensor: all streams combined
                    # so the scan index x-axis spans the whole measurement.
                    streams_data = [
                        (stream, in_s, out_s)
                        for stream, in_s, out_s in stream_results
                        if (in_s and in_s._data_container)
                        or (out_s and out_s._data_container)
                    ]
                    if streams_data:
                        a = time.time()
                        # Synthetic stream name: all streams of a sensor are
                        # merged under one DETECTION_STREAM plot set (like intplot).
                        DataPrep(
                            None,
                            None,
                            html_name,
                            sensor,
                            "DETECTION_STREAM",
                            base_name,
                            base_name_out,
                            self.output_dir,
                            generate_html=True,
                            streams_data=streams_data,
                        )
                        b = time.time()
                        logging.info(
                            f"Merged {len(streams_data)} streams into one plot set"
                            f" for sensor /{sensor}/ in {b - a:.1f}s"
                        )
                        self._write_sensor_sync_sidecar(base_name, sensor, streams_data)

                    logging.info(
                        f"Generated HTML report for sensor /{sensor}/ with /{len(streams)}/ streams"
                    )
                    self._normalize_sensor_kpi_layout(Path(self.output_dir) / base_name, sensor)
            print("\nCompleted processing all sensor/stream combinations")
            # Create a per-base index like html/<base>/<base>.html
            try:
                from InteractivePlot.e_presentation_layer.html_generator import HtmlGenerator
                index_path = HtmlGenerator.create_base_index(self.output_dir, base_name)
                logging.info(f"Per-base index created: {index_path}")
            except Exception:
                logging.exception("Failed to create per-base index")

    def _process_stream(self, sensor, sensor_in, sensor_out, stream):
        """Parse input/output HDF data for one (sensor, stream) combination and
        return (stream, input_data, output_data). DataPrep is invoked once per
        sensor (merged across all streams) by the caller. Runs in worker threads;
        reads the module-level globals hdf_file_in, hdf_file_out, base_name,
        base_name_out which are set once per file pair and only read while
        workers are active. Raises on error; the caller re-raises via
        future.result()."""
        input_data = DataModelStorage()
        output_data = DataModelStorage()

        input_data.init_parent(stream)
        output_data.init_parent(stream)

        # Canonical stream keys (e.g. "DETECTION_001_004") may differ from the
        # actual HDF group names (e.g. "SRR_FL_DETECTION_001_004").  Resolve
        # the real group name per file via the PreRun maps before parsing.
        stream_in = self.stream_map_in.get(sensor, {}).get(stream, stream)
        stream_out = self.stream_map_out.get(sensor, {}).get(stream, stream)

        self._parse_stream(
            hdf_file_in,
            sensor_in,
            stream_in,
            input_data,
            "input",
        )
        self._parse_stream(
            hdf_file_out,
            sensor_out,
            stream_out,
            output_data,
            "output",
        )

        # Return storages; nothing to plot if both are empty.
        if not input_data._data_container and not output_data._data_container:
            return None
        return (stream, input_data, output_data)

    @staticmethod
    def _parse_stream(hdf_file, sensor, stream, storage, source_name):
        sensor_stream_path = f"{sensor}/{stream}"
        if sensor_stream_path not in hdf_file:
            return

        data_group = hdf_file[sensor_stream_path]
        header_variants = [
            "Stream_Hdr",
            "stream_hdr",
            "StreamHdr",
            "STREAM_HDR",
            "streamheader",
            "stream_header",
            "HEADER_STREAM",
        ]
        header_path = next(
            (variant for variant in header_variants if variant in data_group),
            None,
        )
        if header_path:
            scan_index = data_group[f"{header_path}/scan_index"][()]
        else:
            scan_index = (
                AllsensorHdfParser._header_scan_index(data_group)
                or AllsensorHdfParser._infer_row_index(data_group)
            )

        if scan_index is None:
            return

        storage.initialize(scan_index, sensor, stream)
        start_time = time.time()
        hdf_parser.HDF5Parser.parse(
            data_group, storage, scan_index, header_variants
        )
        logging.debug(
            "Time taken by %s parsing %.4f in %s",
            source_name,
            time.time() - start_time,
            stream,
        )

    @staticmethod
    def _header_scan_index(data_group):
        """Mirror the CAN KPI reader (HdfAttrReader.get_scan_index): prefer the
        header group's HED_LOOK_INDEX / HED_SCAN_INDEX attributes when no
        Stream_Hdr/scan_index dataset is present (CAN edge-case HDFs).

        HED_LOOK_INDEX is preferred because some producers write
        HED_SCAN_INDEX = HED_LOOK_INDEX - 1 (off-by-one) while the detection
        payloads are indexed by the look index; aligning on HED_SCAN_INDEX then
        shifts input vs output by one scan."""
        parent = getattr(data_group, "parent", None)
        if parent is None:
            return None
        for gname in sorted(parent.keys()):
            if "HEADER" not in str(gname).upper():
                continue
            sibling = parent[gname]
            if not isinstance(sibling, h5py.Group):
                continue
            for key in ("HED_LOOK_INDEX", "HED_SCAN_INDEX"):
                if key in sibling.attrs:
                    arr = np.asarray(sibling.attrs[key])
                    if arr.ndim == 1 and arr.size > 0:
                        return list(arr.astype(int))
        return None

    @staticmethod
    def _infer_row_index(data_group):
        for name, item in data_group.items():
            if not isinstance(item, h5py.Dataset):
                continue
            if name.startswith("id_") or name.startswith("timestamp_"):
                continue
            if len(item.shape) == 1:
                return list(range(1, len(item) + 1))
        # Attribute-based fallback: CAN edge-case HDFs store payloads as group
        # attributes (mirror HdfAttrReader.get_scan_index fallback).
        for name, value in data_group.attrs.items():
            if name.startswith("id_") or name.startswith("timestamp_"):
                continue
            arr = np.asarray(value)
            if arr.ndim == 1 and arr.size > 0:
                return list(range(1, arr.size + 1))
        return None


    def _write_sensor_sync_sidecar(self, base_name, sensor, streams_data):
        """Write a small sidecar JSON per sensor recording the input/output
        scan-index ranges so the HTML index pages can warn the user when the
        input and output are not in sync (offset / missing scans)."""
        try:
            import json
            in_keys = set()
            out_keys = set()
            for _stream, in_s, out_s in streams_data:
                if in_s is not None and getattr(in_s, "_data_container", None):
                    in_keys.update(int(k) for k in in_s._data_container.keys())
                if out_s is not None and getattr(out_s, "_data_container", None):
                    out_keys.update(int(k) for k in out_s._data_container.keys())
            if not in_keys and not out_keys:
                return
            def _stats(keys):
                return {
                    "count": len(keys),
                    "min": min(keys) if keys else None,
                    "max": max(keys) if keys else None,
                }
            in_stats = _stats(in_keys)
            out_stats = _stats(out_keys)
            matched = sorted(in_keys & out_keys)
            payload = {
                "sensor": str(sensor),
                "input": in_stats,
                "output": out_stats,
                "matched_scan_count": len(matched),
                "offset": (
                    (out_stats["min"] - in_stats["min"])
                    if in_stats["min"] is not None and out_stats["min"] is not None
                    else None
                ),
            }
            sensor_dir = Path(self.output_dir) / base_name / str(sensor)
            sensor_dir.mkdir(parents=True, exist_ok=True)
            sidecar = sensor_dir / f"{base_name}_{sensor}_sync.json"
            sidecar.write_text(
                json.dumps(payload, indent=2), encoding="utf-8"
            )
            logging.info("Wrote scan-sync sidecar: %s", sidecar)
        except Exception:
            logging.exception("Failed to write scan-sync sidecar for sensor %s", sensor)

    def _write_read_error_placeholder_report(self, base_name: str, input_file: str, output_file: str, read_error: str) -> None:
        try:
            report_dir = Path(self.output_dir) / base_name / "sensors" / "UNKNOWN" / "READ_ERROR"
            report_dir.mkdir(parents=True, exist_ok=True)
            report_path = report_dir / f"{base_name}_UNKNOWN_read_error_scatter.html"

            fig = go.Figure()
            fig.add_trace(
                go.Scatter(
                    x=[0],
                    y=[0],
                    mode="markers",
                    name="INPUT",
                    marker=dict(color="red", size=8),
                )
            )
            fig.add_trace(
                go.Scatter(
                    x=[0],
                    y=[0],
                    mode="markers",
                    name="output mismatch",
                    marker=dict(color="blue", size=8),
                )
            )
            fig.update_layout(
                title="Read-error fallback plot (0 placeholder)",
                xaxis_title="ScanIndex",
                yaxis_title="Value",
            )

            meta = (
                f"<p><b>Input:</b> {os.path.basename(input_file)}</p>"
                f"<p><b>Output:</b> {os.path.basename(output_file)}</p>"
                f"<p><b>Reason:</b> {read_error}</p>"
                "<p><b>Note:</b> Source HDF could not be read, so a zero placeholder plot is generated.</p>"
            )
            html = (
                "<!doctype html><html><head><meta charset='utf-8'><title>Read Error Placeholder</title></head><body>"
                "<h1>Read Error Placeholder Report</h1>"
                f"{meta}"
                f"{fig.to_html(full_html=False, include_plotlyjs='inline')}"
                "</body></html>"
            )
            report_path.write_text(html, encoding="utf-8")
            logging.info("Created read-error placeholder report: %s", report_path)

            try:
                from InteractivePlot.e_presentation_layer.html_generator import HtmlGenerator

                index_path = HtmlGenerator.create_base_index(self.output_dir, base_name)
                logging.info("Per-base index updated after read-error fallback: %s", index_path)
            except Exception:
                logging.exception("Failed to refresh base index after read-error fallback")
        except Exception:
            logging.exception("Failed to create read-error placeholder report")

    def _run_sil_artifacts_once(self, input_file: str, output_file: str, base_name: str) -> None:
        """Create the base output folder for this file pair.

        The CAN KPI HTML is generated by the CAN KPI server (can_kpi_server.py)
        over ZMQ, so no local SIL artifact generation is performed here.
        """
        _ = (input_file, output_file)
        base_folder = Path(self.output_dir) / base_name
        base_folder.mkdir(parents=True, exist_ok=True)

    def _sensor_kpi_dir(self, base_folder: Path, sensor: str) -> Path:
        sensor_dir = base_folder / str(sensor).upper() / "KPI"
        sensor_dir.mkdir(parents=True, exist_ok=True)
        return sensor_dir

    def _stage_artifact(self, target_dir: Path, source_path: Path) -> None:
        try:
            target = target_dir / source_path.name
            shutil.copy2(source_path, target)
        except Exception:
            logging.exception("Failed staging artifact %s -> %s", source_path, target_dir)

    def _normalize_sensor_kpi_layout(self, base_folder: Path, sensor: str) -> None:
        """Ensure all KPI HTML files for a sensor are kept under <sensor>/KPI."""
        sensor_root = base_folder / str(sensor).upper()
        kpi_dir = self._sensor_kpi_dir(base_folder, sensor)
        if not sensor_root.exists():
            return
        for kpi_html in sensor_root.glob("*_kpi.html"):
            try:
                target = kpi_dir / kpi_html.name
                shutil.move(str(kpi_html), str(target))
            except Exception:
                logging.exception("Failed moving KPI file %s -> %s", kpi_html, kpi_dir)

    def _sensor_from_sil_name(self, path: Path) -> str:
        m = re.search(r"_([A-Z]{2,4})_sil_validation_report$", path.stem)
        if m:
            return m.group(1)
        return ""

    def _stage_sensor_artifact(self, base_folder: Path, sensor: str, source_path: Path) -> None:
        sensor_stream_dir = base_folder / sensor / "DETECTION_STREAM"
        sensor_stream_dir.mkdir(parents=True, exist_ok=True)
        target = sensor_stream_dir / source_path.name
        try:
            shutil.copy2(source_path, target)
        except Exception:
            logging.exception("Failed staging SIL artifact %s -> %s", source_path, target)

    def _write_f1_summary(self, base_folder: Path, kpi_paths: List[Path]) -> None:
        rows = []
        pat = re.compile(r"<tr><td>f1_score</td><td>([^<]+)</td></tr>", re.IGNORECASE)
        numeric_f1 = []
        for p in kpi_paths:
            try:
                txt = p.read_text(encoding="utf-8", errors="ignore")
                m = pat.search(txt)
                f1 = m.group(1).strip() if m else "NA"
                sensor = self._sensor_from_sil_name(p) or p.stem
                try:
                    numeric_f1.append(float(f1))
                except Exception:
                    pass
                rows.append((sensor, f1, p.name))
            except Exception:
                rows.append((p.stem, "NA", p.name))

        if not rows:
            return

        body_rows = "".join(
            f"<tr><td>{sensor}</td><td>{f1}</td><td><a href=\"{fname}\">{fname}</a></td></tr>"
            for sensor, f1, fname in rows
        )
        avg_f1 = (sum(numeric_f1) / len(numeric_f1)) if numeric_f1 else float("nan")
        avg_txt = f"{avg_f1:.4f}" if not math.isnan(avg_f1) else "NA"
        html = f"""<!doctype html>
<html lang=\"en\"><head><meta charset=\"utf-8\"/>
<title>KPI F1 Summary</title>
<style>body{{font-family:Arial,sans-serif;background:#0f172a;color:#e2e8f0;margin:24px}}table{{border-collapse:collapse;width:100%}}th,td{{border:1px solid #334155;padding:8px}}th{{background:#1f2937}}</style>
</head><body>
    <h1>KPI Accuracy Summary</h1>
    <p><strong>Average Accuracy (F1):</strong> {avg_txt}</p>
    <table><thead><tr><th>Sensor</th><th>Accuracy (F1)</th><th>KPI HTML</th></tr></thead><tbody>{body_rows}</tbody></table>
</body></html>"""
        (base_folder / "kpi_f1_summary.html").write_text(html, encoding="utf-8")
