"""This file contains test implementations for the source file ct_parser."""
import os
import re
import numpy as np
import pytest
from typing import List
import xml.etree.ElementTree as Et

import python_src.ct_parser as ct_p
import python_src.ct_shared_resources as ct_sr


def test_parse_calibration_xml__parsing_of_xml_files_with_two_customers():
    """
    Test parsing of provided xml files. In case of customer_b it is expected that no customer specific
    mapping is created, since entries of this customer specific file are erroneous.
    """
    # \arrange Set up path to main xml for parsing.
    calibration_path = os.path.join(os.path.dirname(__file__), "..", "example_files")
    path_to_main_xml = os.path.join(calibration_path, "Core", "cool_feature_cal.xml")
    # \action Call customer specific file creation
    calibrations, _, generic_calibration_info = ct_p.parse_calibration_xml(path_to_main_xml, calibration_path)
    # \assert expect that files are parsed correctly.
    # Check that customer specific entries for customer_b are rejected except for cal_1, since this change is permitted.
    permitted_changes: List[str] = ct_sr.cal_header_order + ["cal_1"]
    result_list = [not bool("Customer_B" in value.customer_specific_values and key not in permitted_changes) for key,
                   value in calibrations.items()]
    assert all(res for res in result_list)
    # Check that basic information is parsed correctly
    assert "CF_CORE_CALIBRATION_SIZE" == generic_calibration_info.get_cal_size_macro_name(generic_calibration_info.core_structure_name)
    assert (np.array(generic_calibration_info.customers) == np.array(["Customer_A", "Customer_B"])).all()
    assert "Cf_Core_Calibration_T" == generic_calibration_info.core_structure_name
    assert "Cf_Core_Calibration_T" == generic_calibration_info.core_structure_name
    assert "cf_core_calibration" == generic_calibration_info.get_deployment_file_name(generic_calibration_info.core_structure_name)
    # Check that customer specific values for Customer_A are parsed correctly
    assert calibrations["cal_float_array_1"].customer_specific_values["Customer_A"] == "[150.0f 100.0f]"
    assert calibrations["cal_5"].customer_specific_values["Customer_A"] == "25u"
    assert calibrations["cal_3"].customer_specific_values["Customer_A"] == "80u"
    # Check that two padding bytes are added due to 4 byte alignment
    assert set(["k_unused_padding_byte_0", "k_unused_padding_byte_1"]).issubset(calibrations)
    # Check that incomplete data is not added to the calibration dictionary
    assert "cal_i_am_an_incomplete_cal" not in calibrations
    # Check that h3_hdr_cals are overwritten accordingly
    assert calibrations["Section_Size"].default_value == "76"
    assert calibrations["Cal_Chk_Sum"].customer_specific_values["Customer_A"] == 2628
    assert calibrations["Cal_Chk_Sum"].customer_specific_values["Customer_B"] == 2609

def parsed_xml() -> Et:
    """
    Parse the input .xml file containing the calibrations data and return it as a string
    """
    # Set up path to main xml for parsing.
    calibration_path = os.path.join(os.path.dirname(__file__), "..", "example_files")
    path_to_main_xml = os.path.join(calibration_path, "Core", "cool_feature_cal.xml")
    # Parse input xml
    tree = Et.parse(path_to_main_xml)
    tree = tree.getroot()
    # Convert to parsed .xml file to string
    text = Et.tostring(tree, encoding="unicode", method="xml")
    return text, calibration_path

def test_parse_calibration_xml__calibration_generic_info_header_missing(ct_fixture_setup_path_for_new_sample_xml, caplog):
    """
    Test parsing of provided xml files. If CALIBRATION_GENERIC_INFO is missing it lead to an error.
    Checks are expected to catch this.
    """
    # \arrange Set up path to xml for parsing.
    text, calibration_path = parsed_xml()
    # Remove the CALIBRATION_GENERIC_INFO header and all associated data
    text = re.sub("<CALIBRATION_GENERIC_INFO>?(.*?)</CALIBRATION_GENERIC_INFO>", "", text, flags=re.DOTALL)
    # Open a new temporary .xml file for calibration
    with open(ct_fixture_setup_path_for_new_sample_xml, "w", encoding="utf8") as f:
        # Save the new calibration data to file
        f.write(text)
    with pytest.raises(Exception):
        # \action Call customer specific file creation
        ct_p.parse_calibration_xml(ct_fixture_setup_path_for_new_sample_xml, calibration_path)
    # \assert expect that checks work correctly and clean up.
    assert "CALIBRATION_HEADER section is missing" in caplog.text

def test_parse_calibration_xml__calibration_component_missing(ct_fixture_setup_path_for_new_sample_xml, caplog):
    """
    Test parsing of provided xml files. If CALIBRATION_COMPONENT is missing it lead to an error.
    Checks are expected to catch this.
    """
    # \arrange Set up path to xml for parsing.
    text, calibration_path = parsed_xml()
    # Remove the CALIBRATION_COMPONENT header and all associated data
    text = re.sub("<CALIBRATION_COMPONENT>?(.*?)</CALIBRATION_COMPONENT>", "", text, flags=re.DOTALL)
    # Open a new temporary .xml file for calibration
    with open(ct_fixture_setup_path_for_new_sample_xml, "w", encoding="utf8") as f:
        # Save the new calibration data to file
        f.write(text)
    # \action Call customer specific file creation
    ct_p.parse_calibration_xml(ct_fixture_setup_path_for_new_sample_xml, calibration_path)
    # \assert expect that checks work correctly.
    assert "CALIBRATION_COMPONENT section is missing" in caplog.text

def test_parse_calibration_xml__record_header_name_with_typo(ct_fixture_setup_path_for_new_sample_xml, caplog):
    """
    Test parsing of provided xml files. A typo in header lead to an error.
    Exception is expected to catch this.
    """
    # \arrange Set up path to xml for parsing.
    text, calibration_path = parsed_xml()
    # Replace the header name with an inconsistent one
    text = text.replace("<record>", "<sadge>", 1)
    # Open a new temporary .xml file for calibration
    with open(ct_fixture_setup_path_for_new_sample_xml, "w", encoding="utf8") as f:
        # Save the new calibration data to file
        f.write(text)
    with pytest.raises(Exception):
        # \action Call customer specific file creation
        ct_p.parse_calibration_xml(ct_fixture_setup_path_for_new_sample_xml, calibration_path)
    # \assert expect that exception works correctly.
    assert "mismatched tag:" in caplog.text

def test_parse_calibration_xml__missing_brackets(ct_fixture_setup_path_for_new_sample_xml, caplog):
    """
    Test parsing of provided xml files. Missing brackets lead to an error.
    Exception is expected to catch this.
    """
    # \arrange Set up path to xml for parsing.
    text, calibration_path = parsed_xml()
    # Replace the value with an inconsistent one
    text = text.replace("[5.0f 10.0f]", "5.0f 10.0f", 1) # cal_float_array_1
    # Open a new temporary .xml file for calibration
    with open(ct_fixture_setup_path_for_new_sample_xml, "w", encoding="utf8") as f:
        # Save the new calibration data to file
        f.write(text)
    with pytest.raises(Exception):
        # \action Call customer specific file creation
        ct_p.parse_calibration_xml(ct_fixture_setup_path_for_new_sample_xml, calibration_path)
    # \assert expect that exception works correctly.
    assert "does not have properly set brackets denoting matrix" in caplog.text

def test_parse_calibration_xml__comma_in_matrix(ct_fixture_setup_path_for_new_sample_xml, caplog):
    """
    Test parsing of provided xml files. Comma in matrix lead to an error.
    Exception is expected to catch this.
    """
    # \arrange Set up path to xml for parsing.
    text, calibration_path = parsed_xml()
    # Replace the value with an inconsistent one
    text = text.replace("[5.0f 10.0f]", "[5.0f, 10.0f]", 1) # cal_float_array_1
    # Open a new temporary .xml file for calibration
    with open(ct_fixture_setup_path_for_new_sample_xml, "w", encoding="utf8") as f:
        # Save the new calibration data to file
        f.write(text)
    with pytest.raises(Exception):
        # \action Call customer specific file creation
        ct_p.parse_calibration_xml(ct_fixture_setup_path_for_new_sample_xml, calibration_path)
    # \assert expect that exception works correctly.
    assert "contains a comma" in caplog.text

def test_parse_calibration_xml__no_decimal_number(ct_fixture_setup_path_for_new_sample_xml, caplog):
    """
    Test parsing of provided xml files. Invalid range or default number specified - is not a decimal number
    Exception is expected to catch this.
    """
    # \arrange Set up path to xml for parsing.
    text, calibration_path = parsed_xml()
    # Replace the value with an inconsistent one
    text = text.replace("0u", "u", 1) # cal_1
    # Open a new temporary .xml file for calibration
    with open(ct_fixture_setup_path_for_new_sample_xml, "w", encoding="utf8") as f:
        # Save the new calibration data to file
        f.write(text)
    # \action Call customer specific file creation
    ct_p.parse_calibration_xml(ct_fixture_setup_path_for_new_sample_xml, calibration_path)
    # \assert expect that exception works correctly.
    assert "not contain a decimal value" in caplog.text
