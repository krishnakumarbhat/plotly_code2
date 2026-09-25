from pathlib import Path

import pytest

from hpcc_main import RuntimeBroker


class DummyStore:
    pass


def test_resolve_output_path_uses_managed_output_when_blank(tmp_path):
    broker = RuntimeBroker(tmp_path, DummyStore())
    run_dir = tmp_path / "runs" / "user" / "udp_kpi_20260508_000000"

    output_path, warning = broker._resolve_output_path("", run_dir, "v0bivq")

    assert output_path == str(run_dir / "output")
    assert warning == ""
    assert Path(output_path).is_dir()


def test_resolve_output_path_accepts_explicit_directory_without_broker_write_test(tmp_path):
    """Broker must NOT test writability; mkdir is deferred to the Slurm job."""
    broker = RuntimeBroker(tmp_path, DummyStore())
    run_dir = tmp_path / "runs" / "user" / "udp_kpi_20260508_000000"

    explicit_path = "/mnt/usmidet/projects/GPO-IFV7XX/8-Users/pcmzxl/out_html"
    output_path, warning = broker._resolve_output_path(explicit_path, run_dir, "pcmzxl")

    assert output_path == explicit_path
    assert warning == ""