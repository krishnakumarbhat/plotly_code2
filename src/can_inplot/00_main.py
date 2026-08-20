"""can_inplot — unified CAN KPI + interactive plot pipeline entry point.

Purpose: orchestrate the 5-layer pipeline. When triggered, verifies the
preprocessed index cache; when missing/stale, automatically runs the KPI
index generator first, then streams per-sensor KPI work through the ZMQ
transport and compiles interactive HTML output.

Usage
-----
    python -m can_inplot.00_main <config.json> [output_dir] [--zmq PORT] [--report]
    python -m can_inplot.00_main --verify <input.hdf> <output.hdf>
    python -m can_inplot.00_main --research-report

Modes
-----
    default      : batch KPI + interactive plot synthesis (with cache resolution)
    --zmq PORT   : run the KPI ZMQ server on the given port
    --verify     : verify pipeline + algorithms on one HDF pair
    --research-report : compile overnight_research_report.html
"""

import argparse
import logging
import os
from pathlib import Path
from typing import Any, Dict, List, Optional

import numpy as np

from can_inplot._02_kpi.index_cache import verify_index_cache, IndexCache
from can_inplot._02_kpi.kpi_business import KpiBusiness
from can_inplot._05_visual.html_gen import CanKpiEngine, KpiHtmlGen

logging.basicConfig(
    level=os.environ.get("CAN_INPLOT_LOG_LEVEL", "INFO").upper(),
    format="%(asctime)s  %(levelname)-8s  %(name)s  %(message)s",
)
logger = logging.getLogger("can_inplot.main")


class CanInplotPipeline:
    """Top-level orchestrator wiring 01_ingest → 02_kpi → 03_transport → 05_visual."""

    def __init__(self) -> None:
        """Purpose: build pipeline components.
        Inputs : none.
        Outputs: pipeline instance."""
        self.business = KpiBusiness()
        self.engine = CanKpiEngine(business=self.business)
        self.html = KpiHtmlGen()

    def resolve_index_cache(
        self, input_hdf: str, output_hdf: str, cache_root: Path, force: bool = False
    ) -> IndexCache:
        """Purpose: verify/regenerate the KPI index cache (dependency resolution).
        Inputs : HDF pair paths, cache root, force flag.
        Outputs: IndexCache verdict.

        Execution dependency: when ``can_inplot`` is triggered and the index
        cache/preprocessed data is missing, this method automatically runs the
        KPI index generator (per-sensor HTML + index page) before proceeding.
        """
        cache_root = Path(cache_root)

        def _generator() -> None:
            logger.info("Auto-running CAN KPI index generator (dependency resolution)...")
            sensors = self.engine.discover_sensors(input_hdf, output_hdf)
            if not sensors:
                raise RuntimeError("No sensors discovered; cannot build index cache")
            base_name = Path(input_hdf).stem
            for sensor in sensors:
                self.engine.generate_sensor_report(
                    sensor_id=sensor,
                    input_hdf=input_hdf,
                    output_hdf=output_hdf,
                    output_dir=str(cache_root),
                    base_name=base_name,
                    kpi_subdir="KPI",
                )
            logger.info("Index cache generated for %d sensors.", len(sensors))

        return verify_index_cache(
            input_hdf, output_hdf, cache_root, generator=_generator, force=force
        )

    def run_pair(
        self,
        input_hdf: str,
        output_hdf: str,
        output_dir: str = "out_html",
        cache_root: Optional[str] = None,
    ) -> Dict[str, Any]:
        """Purpose: end-to-end run for one HDF pair.
        Inputs : HDF paths, output dir, optional cache root.
        Outputs: dict with report path, per-sensor metrics, cache verdict."""
        out_dir = Path(output_dir).expanduser().resolve()
        out_dir.mkdir(parents=True, exist_ok=True)
        cache_path = Path(cache_root) if cache_root else out_dir / "index_cache"
        cache = self.resolve_index_cache(input_hdf, output_hdf, cache_path)

        in_parsed = self.business._hdf.parse_file(input_hdf)
        out_parsed = self.business._hdf.parse_file(output_hdf)
        in_stores = self.business._hdf.extract_storages(in_parsed)
        out_stores = self.business._hdf.extract_storages(out_parsed)
        all_sensors = sorted(set(in_stores) | set(out_stores))

        per_sensor: Dict[str, Dict[str, Any]] = {}
        radar_plot_data: Dict[str, tuple] = {}
        for sensor_id in all_sensors:
            in_store = in_stores.get(sensor_id)
            out_store = out_stores.get(sensor_id)
            result = self.business.compute_match_per_sensor(
                in_store, out_store, sensor_id
            )
            latency = (
                {}
                if in_store is None or out_store is None
                else self.business.compute_latency_kpis(in_store, out_store)
            )
            per_sensor[sensor_id] = {"result": result, "latency": latency}
            radar_plot_data[sensor_id] = (
                result.get("scan", np.array([], dtype=np.int64)),
                result.get("overall", np.array([], dtype=np.float16)),
            )

        # Multi-sensor summary plot (interactive)
        plot_html = ""
        if radar_plot_data:
            plot_html = self.html.match_all_radars_plot(
                radar_plot_data, "Overall Match % Across Sensors"
            )

        summary_headers, summary_rows = self.business.build_summary_tables(
            in_stores, out_stores, all_sensors
        )
        summary_html = self.html.stats_table(
            "Overview — All Sensors", summary_headers, summary_rows
        )

        tabs: Dict[str, str] = {}
        for sensor_id, payload in per_sensor.items():
            result = payload["result"]
            latency = payload["latency"]
            latency_rows = [[k, f"{v:.2f}"] for k, v in latency.items()]
            tabs[sensor_id] = "\n".join(
                [
                    self.html.build_sensor_tab(
                        sensor_id,
                        result.get("scan", np.array([], dtype=np.int64)),
                        {
                            "Overall": result.get("overall", np.array([], dtype=np.float16)),
                            "Precision": result.get("precision", np.array([], dtype=np.float16)),
                            "Recall": result.get("recall", np.array([], dtype=np.float16)),
                            "F1": result.get("f1", np.array([], dtype=np.float16)),
                            "Accuracy": result.get("accuracy", np.array([], dtype=np.float16)),
                        },
                        result.get("per_signal", {}),
                    ),
                    self.html.stats_table("Latency KPIs", ["KPI", "Value (ms)"], latency_rows),
                ]
            )
        tabs["Summary Plot"] = plot_html if plot_html else "<p>No plot data.</p>"

        stem = Path(input_hdf).stem
        title = f"CAN Inplot — {stem}"
        page = self.html.build_tabbed_html(tabs, title, summary_html)
        report_path = out_dir / f"{stem}_inplot.html"
        report_path.write_text(page, encoding="utf-8")
        logger.info("Wrote interactive report: %s", report_path)

        return {
            "report_path": str(report_path),
            "sensors": all_sensors,
            "per_sensor": per_sensor,
            "cache": {
                "root": str(cache_root),
                "fresh": cache.is_valid(cache.fingerprint),
                "fingerprint": cache.fingerprint,
            },
        }


def _run_batch(args: argparse.Namespace) -> int:
    """Purpose: batch mode driver.
    Inputs : parsed args.
    Outputs: exit code."""
    pipeline = CanInplotPipeline()
    input_paths: List[str] = []
    output_paths: List[str] = []
    if args.verify:
        input_paths = [args.verify[0]]
        output_paths = [args.verify[1]]
    else:
        import json

        with open(args.config, "r", encoding="utf-8") as fp:
            cfg = json.load(fp)
        input_paths = cfg.get("INPUT_HDF", []) or []
        output_paths = cfg.get("OUTPUT_HDF", []) or []
        if not input_paths:
            logger.error("No INPUT_HDF entries in config")
            return 2

    results = []
    for i, (in_path, out_path) in enumerate(
        zip(input_paths, output_paths if output_paths else input_paths)
    ):
        try:
            result = pipeline.run_pair(
                in_path,
                out_path,
                output_dir=args.output_dir,
                cache_root=args.cache_root,
            )
            results.append(result)
            logger.info(
                "Pair %d: report=%s cache_fresh=%s",
                i + 1,
                result["report_path"],
                result["cache"]["fresh"],
            )
        except Exception as exc:
            logger.exception("Pair %d failed: %s", i + 1, exc)
    if not results:
        return 1
    return 0


def main(argv: Optional[List[str]] = None) -> int:
    """Purpose: CLI entry point.
    Inputs : argv list (defaults to sys.argv[1:]).
    Outputs: exit code."""
    parser = argparse.ArgumentParser(prog="can_inplot")
    parser.add_argument("config", nargs="?", help="kpi.json with INPUT_HDF/OUTPUT_HDF")
    parser.add_argument("output_dir", nargs="?", default="out_html")
    parser.add_argument("--cache-root", default=None, help="index cache directory")
    parser.add_argument("--zmq", type=int, default=None, metavar="PORT", help="run ZMQ server")
    parser.add_argument("--verify", nargs=2, metavar=("IN.hdf", "OUT.hdf"), help="verify on one pair")
    parser.add_argument("--research-report", action="store_true", help="compile overnight report")
    parser.add_argument("--force-cache", action="store_true", help="regenerate index cache")
    args = parser.parse_args(argv)

    if args.research_report:
        from can_inplot._05_visual.report import build_overnight_report

        path = build_overnight_report("overnight_research_report.html")
        print(f"Report written: {path}")
        return 0

    if args.zmq is not None:
        from can_inplot._03_transport.server import run_server

        logger.info("Starting CAN KPI ZMQ server on port %d", args.zmq)
        engine = CanKpiEngine()
        run_server(args.zmq, engine=engine)
        return 0

    if args.verify or args.config:
        return _run_batch(args)

    parser.print_help()
    return 2


if __name__ == "__main__":
    raise SystemExit(main())