import sys
from pathlib import Path

import numpy as np


KPI_ROOT = Path(__file__).resolve().parents[2]
if str(KPI_ROOT) not in sys.path:
    sys.path.insert(0, str(KPI_ROOT))

from UDP_KPI.b_data_storage.kpi_data_model_storage import KPI_DataModelStorage
from UDP_KPI.a_persistence_layer.hdf_wrapper import KPIHDFWrapper
from UDP_KPI.c_business_layer.detection_matching_kpi import process_detection_kpi


def _build_detection_storage(scan_payloads):
    scan_indices = list(scan_payloads.keys())
    storage = KPI_DataModelStorage()
    storage.initialize(scan_indices, "FC_TEST")
    storage.init_parent("DETECTION_STREAM")

    grp_name = "AF_Det"
    counts = [len(scan_payloads[scan_idx]["ran"]) for scan_idx in scan_indices]
    storage.set_value(counts, "num_af_det", grp_name)

    for signal_name in ("ran", "vel", "theta", "phi"):
        dataset = [
            np.asarray(scan_payloads[scan_idx][signal_name], dtype=np.float64)
            for scan_idx in scan_indices
        ]
        storage.set_value(dataset, signal_name, grp_name)

    return storage


def test_get_data_tracks_scan_summaries_and_aliases():
    input_storage = _build_detection_storage(
        {
            100: {
                "ran": [10.0, 20.0],
                "vel": [1.0, 2.0],
                "theta": [0.10, 0.20],
                "phi": [0.01, 0.02],
            },
            200: {
                "ran": [30.0],
                "vel": [3.0],
                "theta": [0.30],
                "phi": [0.03],
            },
        }
    )
    output_storage = _build_detection_storage(
        {
            100: {
                "ran": [11.0],
                "vel": [1.1],
                "theta": [0.10],
                "phi": [0.01],
            },
            300: {
                "ran": [40.0, 50.0],
                "vel": [4.0, 5.0],
                "theta": [0.40, 0.50],
                "phi": [0.04, 0.05],
            },
        }
    )

    output_storage._signal_to_value["azimuth"] = output_storage._signal_to_value.pop("theta")

    result = KPI_DataModelStorage.get_data(input_storage, output_storage, "theta")

    assert result["scan_input_counts"] == {100: 2, 200: 1, 300: 0}
    assert result["scan_output_counts"] == {100: 1, 200: 0, 300: 2}
    assert result["common_scan_indices"] == [100]
    assert result["input_only_scan_indices"] == [200]
    assert result["output_only_scan_indices"] == [300]
    np.testing.assert_array_equal(result["SI"], np.array([100, 200, 300, 300]))
    assert np.isnan(result["I"][2])
    assert np.isnan(result["I"][3])


def test_process_detection_kpi_falls_back_without_rdd_stream():
    input_storage = _build_detection_storage(
        {
            100: {
                "ran": [10.0, 20.0],
                "vel": [1.0, 2.0],
                "theta": [0.10, 0.20],
                "phi": [0.01, 0.02],
            },
            200: {
                "ran": [30.0],
                "vel": [3.0],
                "theta": [0.30],
                "phi": [0.03],
            },
        }
    )
    output_storage = _build_detection_storage(
        {
            100: {
                "ran": [10.2, 45.0],
                "vel": [1.1, 4.5],
                "theta": [0.11, 0.45],
                "phi": [0.01, 0.05],
            },
            200: {
                "ran": [30.3, 60.0],
                "vel": [3.1, 6.0],
                "theta": [0.29, 0.60],
                "phi": [0.03, 0.06],
            },
        }
    )

    result = process_detection_kpi(
        {"DETECTION_STREAM": {"input": input_storage, "output": output_storage}},
        "FC_TEST",
    )

    assert result["success"] is True
    assert result["kpi_results"]["matching_mode"] == "Signal-only fallback"
    assert result["kpi_results"]["matching_accuracy"]["matches"] == 2
    assert result["kpi_results"]["matching_accuracy"]["total_detections"] == 4
    assert result["kpi_results"]["per_scan_matches"] == [1, 1]
    assert result["kpi_results"]["per_scan_den"] == [2, 2]
    assert result["html_content"]


def test_build_aligned_scan_plan_drops_duplicate_common_output_rows():
    common_scan_index, selected_in, selected_out, missing_in, missing_out = (
        KPIHDFWrapper._build_aligned_scan_plan(
            [2681, 2682, 2683, 2684],
            [478, 2682, 2683, 2683, 2684],
        )
    )

    assert common_scan_index == [2682, 2683, 2684]
    assert selected_in == [1, 2, 3]
    assert selected_out == [1, 2, 4]
    assert missing_in == [0]
    assert missing_out == [0, 3]