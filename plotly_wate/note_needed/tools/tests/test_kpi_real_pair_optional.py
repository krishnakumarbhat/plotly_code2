from __future__ import annotations

import os
from pathlib import Path

import pytest

from KPI.kpi_server import run_hdf_mode


@pytest.mark.skipif(
    not (os.environ.get("KPI_TEST_HDF_INPUT") and os.environ.get("KPI_TEST_HDF_OUTPUT")),
    reason="Set KPI_TEST_HDF_INPUT and KPI_TEST_HDF_OUTPUT to run this test with real HDF files.",
)
def test_kpi_runs_on_real_hdf_pair(tmp_path: Path):
    input_h5 = os.environ["KPI_TEST_HDF_INPUT"]
    output_h5 = os.environ["KPI_TEST_HDF_OUTPUT"]

    assert os.path.exists(input_h5), f"Missing input HDF: {input_h5}"
    assert os.path.exists(output_h5), f"Missing output HDF: {output_h5}"

    out_dir = tmp_path / "kpi_html"
    out_dir.mkdir(parents=True, exist_ok=True)

    run_hdf_mode(input_hdf=input_h5, output_hdf=output_h5, html_output_dir=str(out_dir))

    # We can’t predict exact filenames (depends on sensors discovered),
    # but KPI should produce at least one HTML file.
    html_files = list(out_dir.rglob("*.html"))
    assert html_files, "Expected KPI to generate at least one HTML file"
