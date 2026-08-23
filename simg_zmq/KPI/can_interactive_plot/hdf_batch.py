"""Batch driver: run the merged plot pipeline AND the CAN KPI report for one
HDF input/output pair, then refresh the master index.

Usage: python hdf_batch.py <input.h5> <output.h5> [output_dir]
"""

import argparse
import asyncio
import logging
import os
import sys

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s %(levelname)s %(message)s",
)


def run_pipeline(input_hdf: str, output_hdf: str, output_dir: str) -> None:
    from can_inplot.b_persistence_layer.hdf_processor_factory import (
        HdfProcessorFactory,
    )

    pair = {input_hdf: output_hdf}
    factory = HdfProcessorFactory(pair, "HDF_WITH_ALLSENSOR", output_dir)
    asyncio.run(factory.process_async())


def run_can_kpi(input_hdf: str, output_hdf: str, output_dir: str) -> None:
    import can_kpi_server

    can_kpi_server.run_hdf_mode(input_hdf, output_hdf, output_dir)


def refresh_indexes(output_dir: str) -> None:
    try:
        from can_inplot.e_presentation_layer.html_generator import HtmlGenerator

        HtmlGenerator.create_timeline_overview(output_dir)
    except Exception:
        logging.exception("Failed to refresh timeline overview")


def main() -> None:
    parser = argparse.ArgumentParser(description="Run plot + CAN KPI for an HDF pair")
    parser.add_argument("input_hdf", help="Input HDF file")
    parser.add_argument("output_hdf", help="Output HDF file")
    parser.add_argument(
        "--outdir", default=None, help="Output directory (defaults next to output HDF)"
    )
    args = parser.parse_args()

    input_hdf = os.path.abspath(args.input_hdf)
    output_hdf = os.path.abspath(args.output_hdf)
    if not os.path.exists(input_hdf):
        logging.error("Input HDF not found: %s", input_hdf)
        sys.exit(2)
    if not os.path.exists(output_hdf):
        logging.error("Output HDF not found: %s", output_hdf)
        sys.exit(2)
    output_dir = os.path.abspath(args.outdir or os.path.dirname(output_hdf))
    os.makedirs(output_dir, exist_ok=True)

    logging.info("=== Step 1/3: merged plot pipeline ===")
    run_pipeline(input_hdf, output_hdf, output_dir)

    logging.info("=== Step 2/3: CAN KPI reports ===")
    run_can_kpi(input_hdf, output_hdf, output_dir)

    logging.info("=== Step 3/3: refresh indexes ===")
    refresh_indexes(output_dir)

    logging.info("DONE: %s", output_dir)


if __name__ == "__main__":
    main()
