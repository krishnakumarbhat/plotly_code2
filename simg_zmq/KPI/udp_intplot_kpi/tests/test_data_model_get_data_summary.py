from unittest.mock import MagicMock

import numpy as np

from InteractivePlot.c_data_storage.data_model_storage import DataModelStorage


def test_get_data_tracks_common_and_one_sided_scans():
    input_data = MagicMock()
    output_data = MagicMock()

    input_data._signal_to_value = {"ran": "0_0"}
    output_data._signal_to_value = {"ran": "0_0"}

    input_data._data_container = {
        100: [[np.array([10.0, 20.0])]],
        200: [[np.array([30.0])]],
    }
    output_data._data_container = {
        100: [[np.array([11.0])]],
        300: [[np.array([40.0, 50.0])]],
    }

    result = DataModelStorage.get_data(input_data, output_data, "ran")

    assert result["scan_input_counts"] == {100: 2, 200: 1, 300: 0}
    assert result["scan_output_counts"] == {100: 1, 200: 0, 300: 2}
    assert result["common_scan_indices"] == [100]
    assert result["input_only_scan_indices"] == [200]
    assert result["output_only_scan_indices"] == [300]
    assert result["input_point_total"] == 3
    assert result["output_point_total"] == 3
    np.testing.assert_array_equal(result["SI"], np.array([100, 200, 300, 300]))