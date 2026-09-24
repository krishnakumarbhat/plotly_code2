import sys
from pathlib import Path

import numpy as np

KPI_ROOT = Path(__file__).resolve().parents[2]
if str(KPI_ROOT) not in sys.path:
    sys.path.insert(0, str(KPI_ROOT))

from UDP_KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
from UDP_KPI.c_business_layer.detection_matching_kpi import process_detection_kpi

SENSOR = "FC_TEST"


def _build_det_storage(scan_payloads, grp_name, signal_names):
    """signal_names: dict canonical -> stored name, e.g. {"ran": "af_dets_ran", ...}."""
    scan_indices = list(scan_payloads.keys())
    storage = KPI_DataModelStorage()
    storage.initialize(scan_indices, SENSOR)
    storage.init_parent("DETECTION_STREAM")
    counts = [len(scan_payloads[s][signal_names["ran"]]) for s in scan_indices]
    storage.set_value(counts, "num_af_det", grp_name)
    for key in ("ran", "vel", "theta", "phi", "rdd_idx"):
        if key not in signal_names:
            continue
        sig = signal_names[key]
        dataset = [
            np.asarray(scan_payloads[s][sig], dtype=np.float64)
            for s in scan_indices
        ]
        storage.set_value(dataset, sig, grp_name)
    return storage


CLASSIC = {"ran": "ran", "vel": "vel", "theta": "theta", "phi": "phi",
           "rdd_idx": "rdd_idx"}
AFDETS = {"ran": "af_dets_ran", "vel": "af_dets_vel", "theta": "af_dets_theta",
          "phi": "af_dets_phi", "rdd_idx": "af_dets_rdd_idx"}


def _af_payloads(signal_names, ran_out, vel_out, theta_out, phi_out):
    return {
        100: {signal_names["ran"]: [10.0, 20.0],
              signal_names["vel"]: [1.0, 2.0],
              signal_names["theta"]: [0.10, 0.20],
              signal_names["phi"]: [0.01, 0.02],
              signal_names["rdd_idx"]: [0, 1]},
        200: {signal_names["ran"]: [30.0],
              signal_names["vel"]: [3.0],
              signal_names["theta"]: [0.30],
              signal_names["phi"]: [0.03],
              signal_names["rdd_idx"]: [0]},
    }, {
        100: {signal_names["ran"]: ran_out[0],
              signal_names["vel"]: vel_out[0],
              signal_names["theta"]: theta_out[0],
              signal_names["phi"]: phi_out[0],
              signal_names["rdd_idx"]: [0, 1]},
        200: {signal_names["ran"]: ran_out[1],
              signal_names["vel"]: vel_out[1],
              signal_names["theta"]: theta_out[1],
              signal_names["phi"]: phi_out[1],
              signal_names["rdd_idx"]: [0]},
    }


def test_af_dets_layout_end_to_end_via_fallback():
    inp_pay, out_pay = _af_payloads(
        AFDETS,
        ran_out=[[10.1, 20.1], [30.1]],
        vel_out=[[1.1, 2.1], [3.1]],
        theta_out=[[0.11, 0.21], [0.31]],
        phi_out=[[0.015, 0.025], [0.035]],
    )
    inp = _build_det_storage(inp_pay, "AF_Str_Detection", AFDETS)
    out = _build_det_storage(out_pay, "AF_Str_Detection", AFDETS)
    res = process_detection_kpi(
        {"DETECTION_STREAM": {"input": inp, "output": out}}, SENSOR
    )
    assert res["success"] is True
    assert res["kpi_results"]["matching_accuracy"]["matches"] > 0


def test_mixed_layouts_classic_in_af_out():
    inp_pay, _ = _af_payloads(
        CLASSIC,
        ran_out=[[10.1, 20.1], [30.1]],
        vel_out=[[1.1, 2.1], [3.1]],
        theta_out=[[0.11, 0.21], [0.31]],
        phi_out=[[0.015, 0.025], [0.035]],
    )
    _, out_pay = _af_payloads(
        AFDETS,
        ran_out=[[10.1, 20.1], [30.1]],
        vel_out=[[1.1, 2.1], [3.1]],
        theta_out=[[0.11, 0.21], [0.31]],
        phi_out=[[0.015, 0.025], [0.035]],
    )
    inp = _build_det_storage(inp_pay, "Detection_Stream", CLASSIC)
    out = _build_det_storage(out_pay, "AF_Str_Detection", AFDETS)
    res = process_detection_kpi(
        {"DETECTION_STREAM": {"input": inp, "output": out}}, SENSOR
    )
    assert res["success"] is True
    assert res["kpi_results"]["matching_accuracy"]["matches"] > 0


def test_get_value_canonical_resolution_af_alias():
    storage = KPI_DataModelStorage()
    storage.initialize([100], SENSOR)
    storage.init_parent("DETECTION_STREAM")
    storage.set_value(
        [np.asarray([10.0, 20.0], dtype=np.float64)],
        "af_dets_ran",
        "AF_Str_Detection",
    )
    values, status = KPI_DataModelStorage.get_value(storage, "ran")
    assert status == "success"
    assert np.asarray(values).size > 0
