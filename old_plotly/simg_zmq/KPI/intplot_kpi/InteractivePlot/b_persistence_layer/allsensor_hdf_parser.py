import h5py
import os
import logging
import time
import re
import math
import shutil
import tempfile
from pathlib import Path
from typing import Dict, List
import plotly.graph_objects as go
from InteractivePlot.b_persistence_layer import hdf_parser
from InteractivePlot.d_business_layer.data_prep import DataPrep
from InteractivePlot.b_persistence_layer.Persensor_hdf_parser import PersensorHdfParser
from InteractivePlot.b_persistence_layer.prerun_hdf_parser import PreRun
from InteractivePlot.c_data_storage.data_model_storage import DataModelStorage
from InteractivePlot.c_data_storage.config_loader import get_plot_config
from InteractivePlot.d_business_layer.utils import time_taken
from InteractivePlot.kpi_client.kpi_integration import kpiIntegration

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
        Uses sequential processing to avoid serialization issues.

        Returns:
            List[DataPrep]: List of DataPrep objects for each sensor and stream
        """

        for input_file, output_file in self.address_map.items():
            global base_name
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

            # Start KPI service early and notify with required metadata (non-blocking)

            total_combinations = len(sensor_list) * len(streams)
            processed = 0
            global hdf_file_in, hdf_file_out

            try:
                hdf_file_in = h5py.File(input_file, "r")
            except Exception as e:
                print(f"\nError processing input file {str(e)}")

            try:
                hdf_file_out = h5py.File(output_file, "r")
            except Exception as e:
                print(f"\nError processing output file {str(e)}")

            for sensor in sensor_list:
                if os.environ.get('INTERACTIVE_PLOT_ENABLE_KPI', '0') == '1':
                    try:
                        kpi_client = kpiIntegration(
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
                plot_config = get_plot_config()

                for stream in streams:
                    if stream in plot_config:
                        processed += 1
                        progress = (processed / total_combinations) * 100
                        print(f"\rProcessing...... [{progress:.1f}%]", end="")

                        # Create storage instances for this combination
                        input_data = DataModelStorage()
                        output_data = DataModelStorage()

                        input_data.init_parent(stream)
                        output_data.init_parent(stream)

                        # Process input file
                        sensor_stream_path = f"{sensor}/{stream}"

                        if sensor_stream_path in hdf_file_in:
                            data_group = hdf_file_in[sensor_stream_path]

                            # Find header using next() with generator expression
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
                                (
                                    variant
                                    for variant in header_variants
                                    if variant in data_group
                                ),
                                None,
                            )

                            if header_path:
                                scan_index = data_group[f"{header_path}/scan_index"][()]
                                input_data.initialize(scan_index, sensor, stream)
                                a = time.time()
                                input_data = hdf_parser.HDF5Parser.parse(
                                    data_group, input_data, scan_index, header_variants
                                )
                                b = time.time()
                                logging.debug(
                                    f"Time taken by input parsing {b - a} in {stream}"
                                )

                        # Process output file
                        sensor_stream_path = f"{sensor}/{stream}"

                        if sensor_stream_path in hdf_file_out:
                            data_group = hdf_file_out[sensor_stream_path]

                            # Find header using next() with generator expression
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
                                (
                                    variant
                                    for variant in header_variants
                                    if variant in data_group
                                ),
                                None,
                            )

                            if header_path:
                                scan_index = data_group[f"{header_path}/scan_index"][()]
                                output_data.initialize(scan_index, sensor, stream)
                                a = time.time()
                                output_data = hdf_parser.HDF5Parser.parse(
                                    data_group, output_data, scan_index, header_variants
                                )
                                b = time.time()
                                logging.debug(
                                    f"Time taken by output parsing {b - a} in {stream}"
                                )

                        # Create data container if we have data
                        if input_data._data_container or output_data._data_container:
                            a = time.time()
                            # try:
                            DataPrep(
                                input_data,
                                output_data,
                                html_name,
                                sensor,
                                stream,
                                base_name,
                                base_name_out,
                                self.output_dir,
                                generate_html=True,
                            )
                            b = time.time()
                            logging.debug(
                                f"Time taken by dataprep parsing {b - a} in {stream}"
                            )

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
                xaxis_title="Scan Index",
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
        base_folder = Path(self.output_dir) / base_name
        base_folder.mkdir(parents=True, exist_ok=True)

        enable_kpi = os.environ.get('INTERACTIVE_PLOT_ENABLE_KPI', '0') == '1'
        enable_rag = os.environ.get('INTERACTIVE_PLOT_ENABLE_RAG_TEXT', '0') == '1'
        if not enable_kpi and not enable_rag:
            return

        try:
            from InteractivePlot.kpi_client import sil_radar_validation as sil_kpi
            from InteractivePlot.kpi_client import sil_log_narrative as sil_rag

            pair = sil_kpi.FilePair(
                sensor="UNKNOWN",
                base_key=base_name,
                veh_path=Path(input_file),
                resim_path=Path(output_file),
            )

            if enable_kpi:
                tmp_kpi_dir = Path(tempfile.mkdtemp(prefix="sil_kpi_", dir=str(base_folder)))
                kpi_paths = sil_kpi.process_pair(
                    pair,
                    output_dir=tmp_kpi_dir,
                    gate=1.0,
                    metric="euclidean",
                    max_sensors=4,
                )
                logging.info("SIL KPI HTML generated: %s", [str(p) for p in kpi_paths])
                for src in kpi_paths:
                    sensor = self._sensor_from_sil_name(src) or "UNKNOWN"
                    target_dir = self._sensor_kpi_dir(base_folder, sensor)
                    self._stage_artifact(target_dir, src)
                    self._write_f1_summary(target_dir, [target_dir / src.name])
                shutil.rmtree(tmp_kpi_dir, ignore_errors=True)

            if enable_rag:
                try:
                    tmp_rag_dir = Path(tempfile.mkdtemp(prefix="sil_rag_", dir=str(base_folder)))
                    rag_path = sil_rag.process_pair(
                        pair,
                        output_dir=tmp_rag_dir,
                        gate=1.0,
                        metric="euclidean",
                        max_sensors=4,
                    )
                    logging.info("SIL narrative HTML generated: %s", rag_path)
                    rag_sensor = self._sensor_from_sil_name(Path(rag_path)) or "UNKNOWN"
                    self._stage_artifact(self._sensor_kpi_dir(base_folder, rag_sensor), Path(rag_path))
                    shutil.rmtree(tmp_rag_dir, ignore_errors=True)
                except Exception:
                    logging.exception("Global SIL narrative generation failed")

                try:
                    veh_sensors = sil_kpi.sensors_in_file(Path(input_file))
                    resim_sensors = sil_kpi.sensors_in_file(Path(output_file))
                    sensors = sorted(set(veh_sensors) & set(resim_sensors))[:4]
                    for sensor in sensors:
                        veh = sil_kpi.load_radar_hdf(Path(input_file), sensor=sensor, build_frame=True)
                        resim = sil_kpi.load_radar_hdf(Path(output_file), sensor=sensor, build_frame=True)
                        narrative = sil_rag._build_sensor_narrative(sensor, veh, resim, gate=1.0, metric="euclidean")
                        narrative_text = sil_rag._build_narrative_text(base_name, [narrative])
                        per_sensor_rag = self._sensor_kpi_dir(base_folder, sensor) / f"{base_name}_{sensor}_sil_narrative.html"
                        sil_rag._write_html(per_sensor_rag, f"SIL Narrative - {base_name} - {sensor}", narrative_text)
                except Exception:
                    logging.exception("Per-sensor SIL narrative generation failed")
        except Exception as exc:
            logging.exception("SIL artifact generation failed for %s: %s", base_name, exc)

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
