import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

import pytest
import json
import tempfile


@pytest.fixture
def sample_xml_config():
    return """<?xml version="1.0" encoding="UTF-8"?>
    <Configuration>
        <FileType>HDF5</FileType>
    </Configuration>"""


@pytest.fixture
def sample_json_config():
    return {
        "inputs": {"sensor1": "path/to/sensor1", "sensor2": "path/to/sensor2"},
        "outputs": {"plot1": "path/to/plot1"},
    }


@pytest.fixture
def temp_xml_file(sample_xml_config):
    with tempfile.NamedTemporaryFile(mode="w", delete=False, suffix=".xml") as f:
        f.write(sample_xml_config)
    yield f.name
    os.unlink(f.name)


@pytest.fixture
def allsensor_json_config():
    return {
        "inputs": {"sensor1": "path/to/sensor1", "sensor2": "path/to/sensor2"},
        "outputs": {"plot1": "path/to/plot1"},
        "allsensor_specific_key": "value"
    }


@pytest.fixture
def persensor_json_config():
    return {
        "inputs": {"sensor_A": "path/to/sensorA", "sensor_B": "path/to/sensorB"},
        "outputs": {"plot_A": "path/to/plotA"},
        "persensor_specific_key": "value"
    }


@pytest.fixture
def paired_json_config():
    return {
        "inputs": {"pair1": ["path/to/file1", "path/to/file2"]},
        "outputs": {"paired_plot": "path/to/paired_plot"},
        "paired_specific_key": "value"
    }


@pytest.fixture
def temp_json_file(sample_json_config):
    with tempfile.NamedTemporaryFile(mode="w", delete=False, suffix=".json") as f:
        json.dump(sample_json_config, f)
    yield f.name
    os.unlink(f.name)


@pytest.fixture
def temp_allsensor_json_file(allsensor_json_config):
    with tempfile.NamedTemporaryFile(mode="w", delete=False, suffix=".json") as f:
        json.dump(allsensor_json_config, f)
    yield f.name
    os.unlink(f.name)


@pytest.fixture
def temp_persensor_json_file(persensor_json_config):
    with tempfile.NamedTemporaryFile(mode="w", delete=False, suffix=".json") as f:
        json.dump(persensor_json_config, f)
    yield f.name
    os.unlink(f.name)


@pytest.fixture
def temp_paired_json_file(paired_json_config):
    with tempfile.NamedTemporaryFile(mode="w", delete=False, suffix=".json") as f:
        json.dump(paired_json_config, f)
    yield f.name
    os.unlink(f.name)
