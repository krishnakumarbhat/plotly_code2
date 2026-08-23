"""Tests for config-driven axes and scan-continuity KPI additions."""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from can_inplot.d_business_layer.data_cal import DataCalculations


def _make_dc():
    return DataCalculations()


def test_x_axis_defaults_to_scan_index():
    dc = _make_dc()
    si = [1, 2, 3]
    dd = {"SI": si, "I": [1.0, 2.0, 3.0], "O": [1.0, 2.0, 3.0]}
    _, fig = dc.scatter_plot("ran", dict(dd), None, None)
    assert fig.layout.xaxis.title.text == "ScanIndex"
    assert list(fig.data[0].x) == si


def test_x_axis_override_uses_stashed_signal():
    dc = _make_dc()
    si = [1, 2, 3]
    dd = {
        "SI": si,
        "I": [1.0, 2.0, 3.0],
        "O": [1.0, 2.0, 3.0],
        "XI": [0.1, 0.2, 0.3],
    }
    dc.set_signal_config({"x_axis": "timestamp", "x_label": "Time (s)"})
    _, fig = dc.scatter_plot("ran", dict(dd), None, None)
    assert fig.layout.xaxis.title.text == "Time (s)"
    assert list(fig.data[0].x)[:3] == [0.1, 0.2, 0.3]


def test_x_axis_falls_back_when_missing():
    dc = _make_dc()
    dd = {"SI": [1, 2], "I": [1.0, 2.0], "O": [1.0, 2.0]}
    dc.set_signal_config({"x_axis": "timestamp"})
    _, fig = dc.scatter_plot("ran", dict(dd), None, None)
    assert fig.layout.xaxis.title.text == "ScanIndex"


def test_scan_continuity_counts_gaps_and_duplicates():
    dc = _make_dc()
    si = [1, 2, 3, 5, 5, 8, 9]  # dropped: 3->5 (1), 5->8 (2) => 3
    dd = {"SI": si, "I": [float(x) for x in si], "O": list(map(float, si))}
    _, fig = dc.scan_continuity_plot("ran", dd, None, None)
    text = fig.layout.annotations[0].text
    assert "dropped: 3" in text
    assert "duplicates: 1" in text
    assert "out-of-order: 0" in text


def test_scan_continuity_handles_numpy_si():
    import numpy as np

    dc = _make_dc()
    dd = {"SI": np.array([10.0, 12.0]), "I": np.zeros(2), "O": np.zeros(2)}
    fid, fig = dc.scan_continuity_plot("ran", dd, None, None)
    assert "dropped: 1" in fig.layout.annotations[0].text
