"""ZMQ-facing CAN KPI wrapper.

Takes the CAN KPI parsing pipeline (a_persistence_layer + b_data_storage),
runs the CAN KPI business matching (c_business_layer) and renders the
per-sensor KPI HTML (d_presentation_layer) at
``<output_dir>/<base_name>/<SENSOR_ID>/<kpi_subdir>/<base_name>_<sensor>_kpi.html``,
mirroring the layout the interactive plot pipeline expects for its KPI tabs.
"""

import logging
import os
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

import numpy as np

from a_persistence_layer.hdf_parser import KpiHdfParser
from b_data_storage.can_kpi_data_model_storage import KPI_DataModelStorage
from c_business_layer.kpi_business import KpiBusiness
from d_presentation_layer.kpi_html_gen import KpiHtmlGen

logger = logging.getLogger(__name__)


class CanKpiEngine:
    """Shared engine: cached HDF parsing + CAN KPI business + HTML generation."""

    def __init__(self) -> None:
        self._business = KpiBusiness()
        self._parser: KpiHdfParser = self._business._hdf
        self._html = KpiHtmlGen()
        self._parse_cache: Dict[str, Tuple[Optional[Tuple[int, int]], Dict[str, Any], Dict[str, Any]]] = {}

    # -------------------------------
    # HDF parsing (layer a + layer b)
    # -------------------------------
    def _parse_cached(self, hdf_path: Optional[str]) -> Dict[str, Any]:
        """Parse one HDF file, caching the result per (mtime, size)."""
        if not hdf_path:
            return {}
        key = os.path.abspath(str(hdf_path))
        token: Optional[Tuple[int, int]] = None
        try:
            stat = os.stat(key)
            token = (stat.st_mtime_ns, stat.st_size)
        except OSError:
            pass

        cached = self._parse_cache.get(key)
        if cached is not None and cached[0] == token:
            return cached[1]

        parsed = self._parser.parse_file(key)
        report = self._parser.get_last_parse_report()
        self._parse_cache[key] = (token, parsed, report)
        return parsed

    def _get_report_cached(self, hdf_path: Optional[str]) -> Dict[str, Any]:
        if not hdf_path:
            return {}
        cached = self._parse_cache.get(os.path.abspath(str(hdf_path)))
        return cached[2] if cached else {}

    def discover_sensors(self, input_hdf: Optional[str], output_hdf: Optional[str]) -> List[str]:
        """Return the sorted union of sensors found in both files."""
        in_parsed = self._parse_cached(input_hdf)
        out_parsed = self._parse_cached(output_hdf)
        return sorted(set(in_parsed.keys()) | set(out_parsed.keys()))

    # -------------------------------
    # Per-sensor report generation
    # -------------------------------
    def generate_sensor_report(
        self,
        sensor_id: str,
        input_hdf: Optional[str],
        output_hdf: Optional[str],
        output_dir: str,
        base_name: str,
        kpi_subdir: str = "KPI",
    ) -> str:
        """Parse the CAN HDF pair and write one KPI HTML page for ``sensor_id``."""
        in_parsed = self._parse_cached(input_hdf)
        out_parsed = self._parse_cached(output_hdf)
        in_report = self._get_report_cached(input_hdf)
        out_report = self._get_report_cached(output_hdf)
        in_stores = self._parser.extract_storages(in_parsed)
        out_stores = self._parser.extract_storages(out_parsed)

        in_store = in_stores.get(sensor_id)
        out_store = out_stores.get(sensor_id)
        label = self._business.FRIENDLY.get(sensor_id, sensor_id)

        result = self._business._compute_match_pct(in_store, out_store, sensor_id)
        diag = self._business._timestamp_diagnostics(in_store, out_store, sensor_id)
        scan = result.get("scan", np.array([], dtype=np.int64))
        metrics = {
            "Overall": result.get("overall", np.array([], dtype=np.float16)),
            "Precision": result.get("precision", np.array([], dtype=np.float16)),
            "Recall": result.get("recall", np.array([], dtype=np.float16)),
            "F1": result.get("f1", np.array([], dtype=np.float16)),
            "Accuracy": result.get("accuracy", np.array([], dtype=np.float16)),
        }

        messages = self._status_messages(sensor_id, in_report, out_report, in_store, out_store)
        sync_plot_html = ""
        mismatch_messages = self._business._timestamp_mismatch_messages(diag)
        if mismatch_messages:
            warning_html = self._html.notice_block(
                f"{label} — Timestamp/ScanIndex Mismatch",
                mismatch_messages,
                tone="warning",
            )
            messages.insert(0, warning_html)
        if in_store is not None or out_store is not None:
            sync_plot_html = self._build_sync_plot_html(label, in_store, out_store)

        if messages and scan.size == 0:
            tab = self._html.build_status_tab(label, messages, tone="warning")
        else:
            tab = self._html.build_sensor_tab(
                label,
                scan,
                metrics,
                result.get("per_signal", {}),
                sync_plot_html=sync_plot_html,
            )
        if mismatch_messages and scan.size > 0:
            tab = warning_html + "\n" + tab

        kpi_dir = Path(output_dir) / base_name / str(sensor_id).upper() / (kpi_subdir or "KPI")
        kpi_dir.mkdir(parents=True, exist_ok=True)
        html_path = kpi_dir / f"{base_name}_{sensor_id}_kpi.html"
        title = f"PCAN KPI — {base_name} — {label}"
        html_path.write_text(self._html.build_tabbed_html({label: tab}, title=title), encoding="utf-8")
        self._write_stats_sidecar(
            kpi_dir,
            base_name,
            sensor_id,
            result,
            diag,
        )
        logger.info(
            "Generated CAN KPI report for sensor=%s base=%s -> %s",
            sensor_id,
            base_name,
            html_path,
        )
        return str(html_path)

    def _write_stats_sidecar(self, kpi_dir: Path, base_name: str, sensor_id: str, result: Dict[str, Any], diag: Optional[Dict[str, Any]] = None) -> None:
        """Write <base>_<sensor>_kpi_stats.json next to each KPI HTML page so the
        interactive plot index pages can show Match % / aligned-scan stats without
        parsing plotly binary data."""
        try:
            import json

            def _avg(arr) -> float:
                arr = np.asarray(arr, dtype=np.float64)
                return float(np.mean(arr)) if arr.size else float("nan")

            stats = {
                "sensor": sensor_id,
                "score": _avg(result.get("overall")),
                "overall": _avg(result.get("overall")),
                "accuracy": _avg(result.get("accuracy")),
                "precision": _avg(result.get("precision")),
                "recall": _avg(result.get("recall")),
                "f1": _avg(result.get("f1")),
                "aligned_scans": int(len(np.asarray(result.get("scan", []), dtype=np.int64))),
            }
            if diag:
                stats["timestamp_mismatch"] = bool(diag.get("mismatch", False))
                stats["offset_exceed_count"] = int(diag.get("offset_exceed_count", 0))
                stats["offset_abs_max_ns"] = int(diag.get("offset_abs_max_ns", 0))
                stats["offset_median_ns"] = int(diag.get("offset_median_ns", 0))
            path = kpi_dir / f"{base_name}_{sensor_id}_kpi_stats.json"
            path.write_text(json.dumps(stats, indent=2), encoding="utf-8")
        except Exception as exc:
            logger.warning("Failed writing KPI stats sidecar for %s: %s", sensor_id, exc)

    def _build_sync_plot_html(
        self,
        label: str,
        in_store: Optional[KPI_DataModelStorage],
        out_store: Optional[KPI_DataModelStorage],
    ) -> str:
        """Timestamp (y) vs scan index (x) for input and output traces."""
        try:
            in_scan = in_store.get_scan_index() if in_store else np.array([], dtype=np.int64)
            out_scan = out_store.get_scan_index() if out_store else np.array([], dtype=np.int64)
            in_time = in_store.get_time_ns() if in_store else np.array([], dtype=np.int64)
            out_time = out_store.get_time_ns() if out_store else np.array([], dtype=np.int64)
            if in_time.size == 0 and out_time.size == 0:
                return ""
            return self._html.timestamp_sync_plot(
                in_scan, in_time, out_scan, out_time, label
            )
        except Exception as exc:
            logger.warning("Failed building timestamp sync plot for %s: %s", label, exc)
            return ""

    def _status_messages(
        self,
        sensor_id: str,
        in_report: Dict[str, Any],
        out_report: Dict[str, Any],
        in_store: Optional[KPI_DataModelStorage],
        out_store: Optional[KPI_DataModelStorage],
    ) -> List[str]:
        if in_store is not None or out_store is not None:
            return []

        messages: List[str] = []
        for label, report in (("Input", in_report), ("Output", out_report)):
            for message in list(report.get("errors", [])) + list(report.get("warnings", [])):
                if not message.startswith(f"{sensor_id}:"):
                    continue
                detail = message.split(": ", 1)[1] if ": " in message else message
                messages.append(f"{label}: {detail}")

        if messages:
            return messages
        return ["HDF was unable to parse this sensor into KPI data for the current report."]


_ENGINE: Optional[CanKpiEngine] = None


def get_engine() -> CanKpiEngine:
    """Return the process-wide engine so parsed files are cached across requests."""
    global _ENGINE
    if _ENGINE is None:
        _ENGINE = CanKpiEngine()
    return _ENGINE


def parse_for_can_kpi(
    sensor_id: str,
    input_file_path: str,
    output_dir: str,
    base_name: str,
    kpi_subdir: str,
    output_file_path: str,
) -> str:
    """Parse a CAN HDF pair for one sensor and return the generated KPI HTML path.

    Mirrors UDP_KPI's ``parse_for_kpi`` entry point used by the ZMQ server.
    """
    if not all([sensor_id, input_file_path, output_file_path, output_dir, base_name]):
        raise ValueError(
            "Missing required fields: sensor_id, input_file_path, output_file_path, output_dir, base_name"
        )

    engine = get_engine()
    return engine.generate_sensor_report(
        sensor_id=sensor_id,
        input_hdf=input_file_path,
        output_hdf=output_file_path,
        output_dir=output_dir,
        base_name=base_name,
        kpi_subdir=kpi_subdir,
    )
