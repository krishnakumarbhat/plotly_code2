"""This file shares test fixtures for the testing folder and possible subfolders.
Subfolders could have their own conftest.py which provides testfixtures for this exact subfolder.
The directories become their own sort of scope where fixtures that are defined in a conftest.py file
in that directory become available for that whole scope."""
import pytest
import python_src.ct_shared_resources as ct_sr
import os
from typing import List, Dict


@pytest.fixture
def ct_fixture_setup_5_fixed_point_cals() -> Dict[str, ct_sr.Calibration]:
    """
    Sets up a test fixture returning example integer constants.

    Returns:
        A calibration dictionary containing 5 unsigned integer constants.
    """
    calibrations: dict = {}
    names: List[str] = ["cal_1", "cal_2", "cal_3", "cal_4", "cal_5"]
    data_types: List[str] = ["uint8_t", "uint8_t", "uint16_t", "uint16_t", "uint32_t"]
    default_values: List[str] = ["10", "20", "30", "40", "50"]

    for name, data_type, default_value in zip(names, data_types, default_values):
        calibrations.update({name: ct_sr.Calibration(name, data_type, "1", "0", "100", default_value,
                                                     False, None, None)})

    return calibrations


@pytest.fixture
def ct_fixture_setup_array_fixed_point_calibrations() -> Dict[str, ct_sr.Calibration]:
    """
    Sets up a test fixture returning example integer arrays.

    Returns:
        A calibration dictionary containing 5 unsigned integer arrays with differing sizes.
    """
    calibrations: dict = {}
    names: List[str] = ["cal_array_1", "cal_array_2", "cal_array_3", "cal_array_4", "cal_array_5"]
    data_types: List[str] = ["uint32_t", "uint16_t", "uint16_t", "uint8_t", "uint8_t"]
    default_values: List[str] = ["[10 10]", "[20 20]", "[30 30 30]", "[40 40 40 40]", "[50 50 50 50 50]"]

    for name, data_type, default_value in zip(names, data_types, default_values):
        calibrations.update({name: ct_sr.Calibration(name, data_type, "1", "0", "100", default_value,
                                                     True, None, None)})

    return calibrations


@pytest.fixture
def ct_fixture_setup_header_calibrations() -> Dict[str, ct_sr.Calibration]:
    """
    Sets up a test fixture returning example calibration header data.

    Returns:
        A calibration dictionary containing elements of the calibration header.
    """
    calibrations: dict = {}
    names: List[str] = ["Section_Size", "version", "Section_Compatibility", "Cal_Chk_Sum",
                        "Chk_sum_Version", "Cal_Type"]
    data_types: List[str] = ["uint32_t", "uint16_t", "uint16_t", "uint16_t", "uint8_t", "uint8_t"]
    default_values: List[str] = ["250", "80", "3", "120", "1", "3"]

    for name, data_type, default_value in zip(names, data_types, default_values):
        calibrations.update({name: ct_sr.Calibration(name, data_type, "1", "0", "255", default_value,
                                                     False, None, None)})

    return calibrations


@pytest.fixture
def ct_fixture_setup_floating_point_calibrations() -> Dict[str, ct_sr.Calibration]:
    """
    Sets up a test fixture example dictionary containing some example values with floating point format.

    Returns:
        A calibration dictionary containing 5 floating point constants.
    """
    calibrations: dict = {}
    names: List[str] = ["cal_float_1", "cal_float_2", "cal_float_3", "cal_float_4", "cal_float_5"]
    data_types: List[str] = ["float32_T", "float32_T", "float32_T", "float32_T", "float32_T"]
    default_values: List[str] = ["1.0", "2.0", "3.0", "4.0", "5.0"]

    for name, data_type, default_value in zip(names, data_types, default_values):
        calibrations.update({name: ct_sr.Calibration(name, data_type, "0.1", "0.0", "100.0", default_value,
                                                     False, None, None)})

    return calibrations


@pytest.fixture
def ct_fixture_setup_floating_point_array_calibration() -> Dict[str, ct_sr.Calibration]:
    """
    Sets up a test fixture example dictionary containing some example one-dimensional arrays with floating point format.

    Returns:
        A calibration dictionary containing 2 one-dimensional floating point arrays.
    """
    calibrations: dict = {}
    names: List[str] = ["cal_float_array_1", "cal_float_array_2"]
    data_types: List[str] = ["float32_T", "float32_T"]
    default_values: List[str] = ["[5.0 10.0]", "[15.5 25.25]"]

    for name, data_type, default_value in zip(names, data_types, default_values):
        calibrations.update({name: ct_sr.Calibration(name, data_type, "0.1", "0.0", "100.0", default_value,
                                                     True, None, None)})

    return calibrations

def ct_fixture_setup_generic_calibration_info_generic(export_asil_check_flag) -> ct_sr.Generic_Calibration_Info:
    """
    Creates an object containing any generic calibration information.

    Returns:
        An object with the type of Generic_Calibration_Info with sample data relating to example_data folder.
    """

    generic_calibration_info: ct_sr.Generic_Calibration_Info = \
        ct_sr.Generic_Calibration_Info("reuse.h", "Cf", export_asil_check_flag, "Cf_Public_Calibration_T",
                                       "Cf_Customer_Calibration_T", "Cf_Core_Calibration_T",
                                       ["Customer_A", "Customer_B"])

    return generic_calibration_info

@pytest.fixture
def ct_fixture_setup_generic_calibration_info() -> ct_sr.Generic_Calibration_Info:
    return ct_fixture_setup_generic_calibration_info_generic(export_asil_check_flag=True)

@pytest.fixture
def ct_fixture_setup_generic_calibration_info_no_asil() -> ct_sr.Generic_Calibration_Info:
    return ct_fixture_setup_generic_calibration_info_generic(export_asil_check_flag=False)


@pytest.fixture
def ct_fixture_setup_floating_point_2d_array_calibration() -> Dict[str, ct_sr.Calibration]:
    """
    Sets up a test fixture example dictionary containing some example values in 2d floating point array format.

    Returns:
        A calibration dictionary containing 2 two-dimensional floating point arrays.
    """
    calibrations: dict = {}
    names: List[str] = ["cal_float_array_2d_1", "cal_float_array_2d_2"]
    data_types: List[str] = ["float32_T", "float32_T"]
    default_values: List[str] = ["[[5.0f 10.0f 15.0f][20.0f 25.0f 30.0f]]", "[[50.0 55.0][60.0 65.0][70.0 75.0]]"]
    units: List[str] = ["m/s", "m"]

    for name, data_type, default_value, unit in zip(names, data_types, default_values, units):
        calibrations.update({name: ct_sr.Calibration(name, data_type, "0.1", "0.0", "100.0", default_value,
                                                     True, False, None, unit)})

    return calibrations

@pytest.fixture
def ct_fixture_setup_path_for_new_sample_xml(tmp_path) -> str:
    """
    Sets up a temporary xml file for testing of exception throwing. This will be deleted after the testing phase.

    Args:
        tmp_path : fixture for temporary paths

    Yields:
        str: A path pointing to a xml file which is deleted in the teardown phase of this fixture
    """

    # Setup phase: Create a path and a temporary file for testing purposes.
    directory = tmp_path / "tmp"
    directory.mkdir()
    file = directory / "sample_data.xml"
    path = directory.name + file.name

    # Execution phase: Interrupt with yield and execute the test
    yield path

    # Teardown phase: Return to the test fixture and clean up the used file
    if os.path.exists(path):
        os.remove(path)


@pytest.fixture
def ct_fixture_setup_bool_cals() -> Dict[str, ct_sr.Calibration]:
    """
    Sets up a test fixture returning example boolean constants.

    Returns:
        A calibration dictionary containing 2 boolean constants.
    """
    calibrations: dict = {}
    names: List[str] = ["cal_6", "cal_7"]
    data_types: List[str] = ["boolean_T", "boolean_T"]
    default_values: List[str] = ["1", "1"]

    for name, data_type, default_value in zip(names, data_types, default_values):
        calibrations.update({name: ct_sr.Calibration(name, data_type, "1", "0", "1", default_value,
                                                     False, None, None)})

    return calibrations

@pytest.fixture
def ct_fixture_setup_int_cals() -> Dict[str, ct_sr.Calibration]:
    """
    Sets up a test fixture returning example signed integer constants.

    Returns:
        A calibration dictionary containing 3 signed integer constants.
    """
    calibrations: dict = {}
    names: List[str] = ["cal_8", "cal_9", "cal_10"]
    data_types: List[str] = ["int8_t", "int16_t", "int32_t"]
    default_values: List[str] = ["1", "2", "3"]

    for name, data_type, default_value in zip(names, data_types, default_values):
        calibrations.update({name: ct_sr.Calibration(name, data_type, "1", "0", "1", default_value,
                                                     False, None, None)})

    return calibrations
