"""CAN Radar KPI — main entry point.

Usage
-----
    python kpi_main.py [kpi.json] [output_dir]

- If kpi.json is omitted, resolves like before (local / bundled / dev).
- If output_dir is omitted, writes to current directory.
- If kpi.json contains multiple INPUT_HDF/OUTPUT_HDF entries, writes one HTML
    per pair and an index.html linking to them.
"""

import logging
import sys
from pathlib import Path
from typing import Optional, List, Dict, Any

from c_business_layer.kpi_business import KpiBusiness
from a_persistence_layer.json_parser import KpiJsonParser
from d_presentation_layer.kpi_html_gen import KpiHtmlGen
import numpy as np

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s  %(levelname)-8s  %(name)s  %(message)s",
)
logger = logging.getLogger(__name__)


class KpiMain:
    def __init__(self, business: Optional[KpiBusiness] = None):
        self._json = KpiJsonParser()
        self._business = business or KpiBusiness()
        self._html = KpiHtmlGen()

    def run(
        self, config_path: str = "kpi.json", output_dir: str | Path = "out_html"
    ) -> Path:
        config_path = str(config_path)
        out_dir = Path(output_dir).expanduser().resolve()
        out_dir.mkdir(parents=True, exist_ok=True)

        logger.info(f"Config: {config_path}")
        logger.info(f"Output dir: {out_dir}")

        cfg = self._json.parse(config_path)
        input_paths: List[str] = cfg.get("INPUT_HDF", [])
        output_paths: List[str] = cfg.get("OUTPUT_HDF", [])
        n_logs = max(len(input_paths), len(output_paths))

        if n_logs <= 0:
            raise ValueError("No INPUT_HDF/OUTPUT_HDF entries found")

        report_files: List[Dict[str, Any]] = []
        failures: List[str] = []
        for i in range(n_logs):
            in_path = input_paths[i] if i < len(input_paths) else None
            out_path = output_paths[i] if i < len(output_paths) else None

            try:
                stem = (
                    Path(in_path).stem
                    if in_path
                    else (Path(out_path).stem if out_path else f"log_{i + 1}")
                )
                safe = "".join(
                    ch if ch.isalnum() or ch in ("-", "_", ".") else "_" for ch in stem
                )
                filename = f"{safe}.html"
                used = {r["filename"] for r in report_files}
                if filename in used:
                    filename = f"{safe}_{i + 1}.html"

                title = f"PCAN KPI — {stem}"
                result = self.run_pair(in_path, out_path, title=title)
                report_path = out_dir / filename
                report_path.write_text(result["html"], encoding="utf-8")
                logger.info(f"Wrote report to {report_path}")
                report_files.append(
                    {
                        "name": stem,
                        "filename": filename,
                        "f1_rows": result.get("index_f1_rows", []),
                        "overall_f1": float(result.get("index_overall_f1", 0.0)),
                    }
                )
            except Exception as exc:
                pair_name = f"input={in_path}, output={out_path}"
                logger.exception(f"Failed report generation for {pair_name}: {exc}")
                failures.append(pair_name)

        if not report_files:
            raise RuntimeError("No report generated. Check logs for failed HDF pairs.")

        index_path = out_dir / "index.html"
        index_path.write_text(self._build_index(report_files), encoding="utf-8")
        logger.info(f"Wrote index to {index_path}")
        if failures:
            logger.warning(f"Failed pairs: {len(failures)}")
        return index_path

    def run_pair(
        self,
        input_hdf: Optional[str],
        output_hdf: Optional[str],
        title: str = "PCAN Detection KPI",
    ) -> dict:
        if not input_hdf and not output_hdf:
            raise ValueError("At least one of input_hdf/output_hdf must be provided")

        in_parsed = self._business._hdf.parse_file(input_hdf) if input_hdf else {}
        out_parsed = self._business._hdf.parse_file(output_hdf) if output_hdf else {}
        in_stores = self._business._hdf.extract_storages(in_parsed)
        out_stores = self._business._hdf.extract_storages(out_parsed)

        all_sensors = sorted(
            set(in_stores.keys()) | set(out_stores.keys()),
            key=lambda s: self._business.SENSOR_ORDER.index(s)
            if s in self._business.SENSOR_ORDER
            else 99,
        )

        match_results = {
            sensor_id: self._business._compute_match_pct(
                in_stores.get(sensor_id), out_stores.get(sensor_id), sensor_id
            )
            for sensor_id in all_sensors
        }

        summary_headers, summary_rows = self._business._build_summary_tables(
            in_stores, out_stores, all_sensors
        )
        summary_html = self._html.stats_table(
            "Overview — All Sensors", summary_headers, summary_rows
        )

        sensor_tabs = {}
        for sensor_id in all_sensors:
            label = self._business.FRIENDLY.get(sensor_id, sensor_id)
            result = match_results.get(sensor_id, {})
            sensor_tabs[label] = self._html.build_sensor_tab(
                label,
                result.get("scan", np.array([], dtype=np.int64)),
                {
                    "Overall": result.get("overall", np.array([], dtype=np.float16)),
                    "Precision": result.get(
                        "precision", np.array([], dtype=np.float16)
                    ),
                    "Recall": result.get("recall", np.array([], dtype=np.float16)),
                    "F1": result.get("f1", np.array([], dtype=np.float16)),
                    "Accuracy": result.get("accuracy", np.array([], dtype=np.float16)),
                },
                result.get("per_signal", {}),
            )

        kpi_rows: List[List[str]] = []
        radar_plot_data = {}
        index_f1_rows: List[Dict[str, Any]] = []
        f1_for_overall: List[float] = []
        for sensor_id in all_sensors:
            result = match_results.get(sensor_id, {})
            scan = result.get("scan", np.array([], dtype=np.int64))
            overall = result.get("overall", np.array([], dtype=np.float16))
            precision = result.get("precision", np.array([], dtype=np.float16))
            recall = result.get("recall", np.array([], dtype=np.float16))
            f1 = result.get("f1", np.array([], dtype=np.float16))
            accuracy = result.get("accuracy", np.array([], dtype=np.float16))

            radar_name = self._business.FRIENDLY.get(sensor_id, sensor_id)
            f1_avg = self._business._avg(f1)
            index_f1_rows.append({"sensor": radar_name, "f1": f1_avg})
            f1_for_overall.append(f1_avg)
            radar_plot_data[radar_name] = (scan, overall)
            kpi_rows.append(
                [
                    radar_name,
                    str(len(scan)),
                    f"{self._business._avg(overall):.2f}",
                    f"{self._business._avg(precision):.2f}",
                    f"{self._business._avg(recall):.2f}",
                    f"{f1_avg:.2f}",
                    f"{self._business._avg(accuracy):.2f}",
                ]
            )

        index_overall_f1 = (
            float(np.mean(np.asarray(f1_for_overall, dtype=np.float64)))
            if f1_for_overall
            else 0.0
        )

        kpi_table = self._html.stats_table(
            "KPI Summary — Sensors",
            [
                "Sensor",
                "Aligned Scans",
                "Overall",
                "Precision",
                "Recall",
                "F1",
                "Accuracy",
            ],
            kpi_rows,
        )
        kpi_plot = self._html.match_all_radars_plot(
            radar_plot_data, "Overall Match % Across Sensors"
        )
        sensor_tabs["KPI"] = self._html.build_kpi_tab(kpi_table, kpi_plot)

        html = self._html.build_tabbed_html(sensor_tabs, title, summary_html)

        signals = {}
        for sid, store in in_stores.items():
            signals[f"input/{sid}"] = store.get_detection_signal_names()
        for sid, store in out_stores.items():
            signals[f"output/{sid}"] = store.get_detection_signal_names()

        return {
            "html": html,
            "signals": signals,
            "storages": {"input": in_stores, "output": out_stores},
            "index_f1_rows": index_f1_rows,
            "index_overall_f1": index_overall_f1,
        }

    def _build_index(self, reports: List[Dict[str, Any]]) -> str:
        cards: List[str] = []
        for idx, report in enumerate(reports):
            name = report.get("name", "Unknown")
            filename = report.get("filename", "#")
            f1_rows = report.get("f1_rows", [])
            overall_f1 = float(report.get("overall_f1", 0.0))

            row_html = ""
            for row in f1_rows:
                sensor = row.get("sensor", "Unknown")
                f1_val = float(row.get("f1", 0.0))
                row_html += f"<tr><td>{sensor}</td><td>{f1_val:.2f}%</td></tr>"
            row_html += (
                f'<tr class="overall-row"><td>Overall Average (All Sensors)</td>'
                f"<td>{overall_f1:.2f}%</td></tr>"
            )

            cards.append(
                "\n".join(
                    [
                        f'<article class="report-card" style="--i:{idx}">',
                        f'<h2><a class="report-link" href="{filename}">{name}</a></h2>',
                        '<div class="table-wrap">',
                        '<table class="f1-table">',
                        "<thead><tr><th>Sensor</th><th>Accuracy</th></tr></thead>",
                        f"<tbody>{row_html}</tbody>",
                        "</table>",
                        "</div>",
                        "</article>",
                    ]
                )
            )

        return "\n".join(
            [
                "<!DOCTYPE html>",
                "<html><head>",
                '<meta charset="utf-8"/>',
                '<meta name="viewport" content="width=device-width, initial-scale=1"/>',
                "<title>PCAN KPI — Index</title>",
                "<style>",
                "*{box-sizing:border-box;}",
                "body{font-family:Segoe UI,Arial,sans-serif;margin:0;padding:20px;background:#f5f6fa;color:#2c3e50;}",
                ".page{max-width:1200px;margin:0 auto;}",
                "h1{margin:0 0 8px 0;color:#2c3e50;}",
                ".sub{margin:0 0 18px 0;color:#5b6b7b;font-size:14px;}",
                ".cards{display:grid;grid-template-columns:repeat(auto-fit,minmax(320px,1fr));gap:14px;}",
                ".report-card{background:#fff;border:1px solid #e8ecef;border-radius:12px;padding:14px;box-shadow:0 4px 14px rgba(0,0,0,.06);opacity:0;transform:translateY(10px);animation:cardIn .45s ease forwards;animation-delay:calc(var(--i)*.06s);transition:transform .2s ease, box-shadow .2s ease;}",
                ".report-card:hover{transform:translateY(-2px);box-shadow:0 8px 22px rgba(0,0,0,.10);}",
                "h2{margin:0 0 8px 0;font-size:20px;line-height:1.3;}",
                ".report-link{color:#3498db;text-decoration:none;word-break:break-word;}",
                ".report-link:hover{text-decoration:underline;}",
                ".meta{margin:0 0 10px 0;color:#5b6b7b;font-size:13px;}",
                ".table-wrap{overflow:auto;border-radius:8px;border:1px solid #eef1f4;}",
                ".f1-table{width:100%;border-collapse:collapse;min-width:280px;}",
                ".f1-table th,.f1-table td{padding:8px 10px;border-bottom:1px solid #edf1f5;text-align:left;font-size:13px;}",
                ".f1-table th:last-child,.f1-table td:last-child{text-align:right;}",
                ".f1-table th{background:#f8fbff;color:#2f4358;position:sticky;top:0;}",
                ".f1-table tbody tr:hover{background:#f7fbff;}",
                ".overall-row td{font-weight:700;background:#f2f8ff;}",
                "@keyframes cardIn{to{opacity:1;transform:translateY(0);}}",
                "@media (max-width:640px){body{padding:12px;}h2{font-size:17px;}}",
                "</style>",
                "</head><body>",
                '<main class="page">',
                "<h1>PCAN KPI Reports</h1>",
                '<p class="sub">Open a log report and review F1 by sensor plus overall average.</p>',
                f'<section class="cards">{"".join(cards)}</section>',
                "</main>",
                "</body></html>",
            ]
        )


def _resolve_config_path(cli_arg: Optional[str]) -> str:
    if cli_arg:
        return cli_arg

    local = Path("kpi.json")
    if local.exists():
        return str(local)

    # PyInstaller one-dir: data files usually live under sys._MEIPASS (e.g., dist/can_kpi/_internal)
    meipass = getattr(sys, "_MEIPASS", None)
    if meipass:
        bundled = Path(meipass) / "kpi.json"
        if bundled.exists():
            return str(bundled)

    # Dev fallback: alongside this source file.
    here = Path(__file__).resolve().parent
    dev = here / "kpi.json"
    if dev.exists():
        return str(dev)

    return "kpi.json"


def _parse_args(argv: list[str]) -> tuple[str, Path]:
    """Return (config_path, output_dir)."""
    config = _resolve_config_path(
        argv[1] if len(argv) > 1 and argv[1].lower().endswith(".json") else None
    )
    out_dir = Path("out_html")

    if len(argv) == 2 and not argv[1].lower().endswith(".json"):
        out_dir = Path(argv[1])
    elif len(argv) >= 3:
        config = argv[1]
        out_dir = Path(argv[2])

    return config, out_dir


if __name__ == "__main__":
    cfg, out_dir = _parse_args(sys.argv)
    KpiMain().run(cfg, out_dir)
