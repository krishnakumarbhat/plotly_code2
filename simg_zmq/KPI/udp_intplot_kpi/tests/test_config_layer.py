import pytest
from InteractivePlot.a_config_layer.xml_config_parser import XmlConfigParser
from InteractivePlot.a_config_layer.json_parser_factory import JSONParserFactory


def test_xml_config_parser(temp_xml_file):
    parser = XmlConfigParser(temp_xml_file)
    file_type = parser.parse()
    assert file_type == "HDF5"


def test_json_parser_factory_allsensor(temp_allsensor_json_file):
    parser = JSONParserFactory.create_parser("HDF_WITH_ALLSENSOR", temp_allsensor_json_file)
    io_map = parser.get_input_output_map()
    assert isinstance(io_map, dict)
    assert "inputs" in io_map
    assert "outputs" in io_map
    assert "allsensor_specific_key" in io_map


def test_json_parser_factory_persensor(temp_persensor_json_file):
    parser = JSONParserFactory.create_parser("HDF_PER_SENSOR", temp_persensor_json_file)
    io_map = parser.get_input_output_map()
    assert isinstance(io_map, dict)
    assert "inputs" in io_map
    assert "outputs" in io_map
    assert "persensor_specific_key" in io_map


def test_json_parser_factory_paired(temp_paired_json_file):
    parser = JSONParserFactory.create_parser("HDF_PAIRED_FILES", temp_paired_json_file)
    io_map = parser.get_input_output_map()
    assert isinstance(io_map, dict)
    assert "inputs" in io_map
    assert "outputs" in io_map
    assert "paired_specific_key" in io_map


def test_json_parser_factory_invalid_type(temp_json_file):
    with pytest.raises(ValueError):
        JSONParserFactory.create_parser("INVALID_HDF_TYPE", temp_json_file)


def test_invalid_xml_file():
    with pytest.raises(Exception):
        parser = XmlConfigParser("nonexistent.xml")
        parser.parse()


def test_invalid_json_file():
    with pytest.raises(Exception):
        # Assuming HDF_WITH_ALLSENSOR is a valid type for this test, 
        # the error should come from the non-existent file
        parser = JSONParserFactory.create_parser("HDF_WITH_ALLSENSOR", "nonexistent.json")
        parser.get_input_output_map()
