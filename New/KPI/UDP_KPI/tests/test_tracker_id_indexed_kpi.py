import sys
from pathlib import Path

import numpy as np

KPI_ROOT = Path(__file__).resolve().parents[2]
if str(KPI_ROOT) not in sys.path:
    sys.path.insert(0, str(KPI_ROOT))

from UDP_KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
from UDP_KPI.c_business_layer.tracker_matching_kpi import process_tracker_kpi

GRP = "F360_Object_Log"
SENSOR = "TEST_SENSOR"
STREAM = "TRACKER_STREAM"


def _build_tracker_storage(scan_payloads, grp_name=GRP):
    """scan_payloads: {scan_idx: {signal_name: [per-slot values]}}."""
    scan_indices = list(scan_payloads.keys())
    storage = KPI_DataModelStorage()
    storage.initialize(scan_indices, SENSOR)
    storage.init_parent(STREAM)
    # Preserve insertion order of first scan for parent/child discipline.
    signal_names = list(scan_payloads[scan_indices[0]].keys())
    for sig in signal_names:
        dataset = [
            np.asarray(scan_payloads[s][sig], dtype=np.float64)
            for s in scan_indices
        ]
        storage.set_value(dataset, sig, grp_name)
    return storage


def test_classic_layout_match():
    scans = [1, 2]
    inp = {
        1: {"trkID": [1, 2], "vcs_xposn": [10.0, 20.0], "vcs_yposn": [5.0, 6.0],
            "vcs_xvel": [1.0, 2.0], "vcs_yvel": [0.5, 0.6], "f_moving": [1, 0]},
        2: {"trkID": [1, 2], "vcs_xposn": [11.0, 21.0], "vcs_yposn": [5.1, 6.1],
            "vcs_xvel": [1.1, 2.1], "vcs_yvel": [0.5, 0.6], "f_moving": [1, 0]},
    }
    out = {
        1: {"trkID": [1, 2], "vcs_xposn": [10.005, 20.005], "vcs_yposn": [5.005, 6.005],
            "vcs_xvel": [1.005, 2.005], "vcs_yvel": [0.505, 0.605], "f_moving": [1, 0]},
        2: {"trkID": [1, 2], "vcs_xposn": [11.005, 21.005], "vcs_yposn": [5.105, 6.105],
            "vcs_xvel": [1.105, 2.105], "vcs_yvel": [0.505, 0.605], "f_moving": [1, 0]},
    }
    res = process_tracker_kpi(
        _build_tracker_storage(inp), _build_tracker_storage(out), SENSOR, STREAM
    )
    assert res["success"] is True
    kpi = res["kpi_results"]
    assert kpi["numerator"] >= 1  # tp
    assert kpi["accuracy"] > 0
    assert "Tracker Accuracy" in res["html_content"]


def test_matrix_fail_zero_percent_still_success():
    inp = {
        1: {"trkID": [1], "vcs_xposn": [10.0], "vcs_yposn": [5.0],
            "vcs_xvel": [1.0], "vcs_yvel": [0.5], "f_moving": [1]},
    }
    out = {
        1: {"trkID": [1], "vcs_xposn": [15.0], "vcs_yposn": [5.0],
            "vcs_xvel": [1.0], "vcs_yvel": [0.5], "f_moving": [1]},
    }
    res = process_tracker_kpi(
        _build_tracker_storage(inp), _build_tracker_storage(out), SENSOR, STREAM
    )
    assert res["success"] is True
    kpi = res["kpi_results"]
    assert kpi["numerator"] == 0  # tp
    assert kpi["total_fn"] > 0
    assert kpi["accuracy"] == 0.0


def test_id_absent_one_side_fp_fn():
    inp = {
        1: {"trkID": [1, 2], "vcs_xposn": [10.0, 20.0], "vcs_yposn": [5.0, 6.0],
            "vcs_xvel": [1.0, 2.0], "vcs_yvel": [0.5, 0.6], "f_moving": [1, 1]},
    }
    out = {
        1: {"trkID": [2, 3], "vcs_xposn": [20.005, 30.0], "vcs_yposn": [6.005, 7.0],
            "vcs_xvel": [2.005, 3.0], "vcs_yvel": [0.605, 0.7], "f_moving": [1, 1]},
    }
    res = process_tracker_kpi(
        _build_tracker_storage(inp), _build_tracker_storage(out), SENSOR, STREAM
    )
    assert res["success"] is True
    kpi = res["kpi_results"]
    assert kpi["numerator"] == 1  # only ID 2 co-occurs and passes
    assert kpi["total_fn"] == 1  # input-only ID 1
    assert kpi["total_fp"] == 1  # output-only ID 3


def test_fmoving_disagree_not_tp():
    inp = {
        1: {"trkID": [1], "vcs_xposn": [10.0], "vcs_yposn": [5.0],
            "vcs_xvel": [1.0], "vcs_yvel": [0.5], "f_moving": [1]},
    }
    out = {
        1: {"trkID": [1], "vcs_xposn": [10.005], "vcs_yposn": [5.005],
            "vcs_xvel": [1.005], "vcs_yvel": [0.505], "f_moving": [0]},
    }
    res = process_tracker_kpi(
        _build_tracker_storage(inp), _build_tracker_storage(out), SENSOR, STREAM
    )
    assert res["success"] is True
    assert res["kpi_results"]["numerator"] == 0


def test_al_layout_padding_ignored():
    inp = {
        1: {"object_trkID": [5, 0, 0], "object_xposn": [10.0, 0.0, 0.0],
            "object_yposn": [20.0, 0.0, 0.0], "object_xvel": [1.0, 0.0, 0.0],
            "object_yvel": [2.0, 0.0, 0.0], "object_f_moving": [1, 0, 0]},
    }
    out = {
        1: {"object_trkID": [5, 0, 0], "object_xposn": [10.005, 0.0, 0.0],
            "object_yposn": [20.005, 0.0, 0.0], "object_xvel": [1.005, 0.0, 0.0],
            "object_yvel": [2.005, 0.0, 0.0], "object_f_moving": [1, 0, 0]},
    }
    res = process_tracker_kpi(
        _build_tracker_storage(inp), _build_tracker_storage(out), SENSOR, STREAM
    )
    assert res["success"] is True
    kpi = res["kpi_results"]
    assert kpi["numerator"] == 1
    assert kpi["denominator"] == 1  # padding slots excluded


def test_duplicate_ids_multiset_consume():
    inp = {
        1: {"trkID": [7, 7], "vcs_xposn": [10.0, 10.0], "vcs_yposn": [20.0, 20.0],
            "vcs_xvel": [1.0, 1.0], "vcs_yvel": [2.0, 2.0], "f_moving": [1, 1]},
    }
    out = {
        1: {"trkID": [7, 7], "vcs_xposn": [10.005, 10.005], "vcs_yposn": [20.005, 20.005],
            "vcs_xvel": [1.005, 1.005], "vcs_yvel": [2.005, 2.005], "f_moving": [1, 1]},
    }
    res = process_tracker_kpi(
        _build_tracker_storage(inp), _build_tracker_storage(out), SENSOR, STREAM
    )
    assert res["success"] is True
    tp = res["kpi_results"]["numerator"]
    assert tp == 2  # bounded by min(2, 2), not 2*2=4


def test_missing_velocity_still_matches():
    inp = {
        1: {"trkID": [1], "vcs_xposn": [10.0], "vcs_yposn": [5.0], "f_moving": [1]},
    }
    out = {
        1: {"trkID": [1], "vcs_xposn": [10.005], "vcs_yposn": [5.005], "f_moving": [1]},
    }
    res = process_tracker_kpi(
        _build_tracker_storage(inp), _build_tracker_storage(out), SENSOR, STREAM
    )
    assert res["success"] is True
    assert res["kpi_results"]["numerator"] >= 1


def test_missing_core_signal_fails():
    inp = {
        1: {"trkID": [1], "vcs_xposn": [10.0]},
    }
    out = {
        1: {"trkID": [1], "vcs_xposn": [10.005]},
    }
    res = process_tracker_kpi(
        _build_tracker_storage(inp), _build_tracker_storage(out), SENSOR, STREAM
    )
    assert res["success"] is False
    assert "Failed to process tracker KPIs" in res["html_content"]


def test_zero_empty_storages_fail_no_exception():
    empty_in = KPI_DataModelStorage()
    empty_in.initialize([1, 2], SENSOR)
    empty_in.init_parent(STREAM)
    empty_out = KPI_DataModelStorage()
    empty_out.initialize([1, 2], SENSOR)
    empty_out.init_parent(STREAM)
    res = process_tracker_kpi(empty_in, empty_out, SENSOR, STREAM)
    assert res["success"] is False  # no exception raised
