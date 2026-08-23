import pytest
from InteractivePlot.b_persistence_layer.hdf_processor_factory import (
    HdfProcessorFactory,
)
import h5py
import numpy as np
import tempfile
import os


@pytest.fixture
def sample_hdf_file():
    with tempfile.NamedTemporaryFile(suffix=".h5", delete=False) as f:
        with h5py.File(f.name, "w") as hdf:
            hdf.create_dataset("test_data", data=np.array([1, 2, 3, 4, 5]))
    yield f.name
    os.unlink(f.name)


@pytest.mark.parametrize(
    "hdf_file_type, json_config_fixture",
    [
        ("HDF_WITH_ALLSENSOR", "allsensor_json_config"),
        ("HDF_PER_SENSOR", "persensor_json_config"),
        ("HDF_PAIRED_FILES", "paired_json_config"),
    ],
)
def test_hdf_processor_factory_creation_and_process(
    hdf_file_type, json_config_fixture, request, tmp_path
):
    json_config = request.getfixturevalue(json_config_fixture)
    output_dir = tmp_path / "test_output"
    output_dir.mkdir()

    # Create dummy HDF files based on json_config inputs if they don't exist
    # This is a simplified version; real HDF files might be complex
    for sensor_path_or_list in json_config["inputs"].values():
        if isinstance(sensor_path_or_list, list):
            for p_item in sensor_path_or_list:
                dummy_file_path = tmp_path / os.path.basename(p_item)
                if not dummy_file_path.exists():
                     with h5py.File(dummy_file_path, "w") as hdf:
                        hdf.create_dataset("dummy_data", data=[1,2,3])
        else:
            dummy_file_path = tmp_path / os.path.basename(sensor_path_or_list)
            if not dummy_file_path.exists():
                with h5py.File(dummy_file_path, "w") as hdf:
                    hdf.create_dataset("dummy_data", data=[1,2,3])
    
    # Update input paths in json_config to point to tmp_path
    updated_inputs = {}
    for key, path_or_list in json_config["inputs"].items():
        if isinstance(path_or_list, list):
            updated_inputs[key] = [str(tmp_path / os.path.basename(p)) for p in path_or_list]
        else:
            updated_inputs[key] = str(tmp_path / os.path.basename(path_or_list))
    
    json_config["inputs"] = updated_inputs

    factory = HdfProcessorFactory(json_config, hdf_file_type, str(output_dir))
    assert factory is not None
    try:
        factory.process()  # This will call the respective processor's process_and_plot
        # Add assertions here to check for output files in output_dir if applicable
        # For example, check if HTML files are generated for plots
        # This depends on the specifics of your plotting logic which is not fully visible here
        # For now, we just ensure it runs without error.
    except Exception as e:
        pytest.fail(f"HdfProcessorFactory.process() raised {e} unexpectedly for {hdf_file_type}")


def test_hdf_processor_factory_invalid_type(sample_json_config, tmp_path):
    output_dir = tmp_path / "test_output"
    output_dir.mkdir()
    with pytest.raises(ValueError):
        HdfProcessorFactory(sample_json_config, "INVALID_HDF_TYPE", str(output_dir))


def test_hdf_file_reading(sample_hdf_file):
    with h5py.File(sample_hdf_file, "r") as hdf:
        assert "test_data" in hdf
        data = hdf["test_data"][:]
        expected_data = np.array([1, 2, 3, 4, 5])
        assert len(data) == len(expected_data)
        assert np.array_equal(data, expected_data)
        assert data.dtype == expected_data.dtype


def test_invalid_hdf_file():
    with pytest.raises(Exception):
        with h5py.File("nonexistent.h5", "r") as hdf:
            pass
