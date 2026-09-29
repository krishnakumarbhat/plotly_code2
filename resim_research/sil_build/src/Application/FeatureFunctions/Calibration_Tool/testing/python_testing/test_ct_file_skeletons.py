"""This file contains test implementations for the source file ct_file_skeletons."""
import python_src.ct_shared_resources as ct_sr
import python_src.file_skeletons.ct_core_cal_print_skeleton as ct_ccps
import python_src.file_skeletons.ct_core_header_skeleton as ct_chs
import python_src.file_skeletons.ct_customer_data_stream_skeleton as ct_cdss
import python_src.file_skeletons.ct_customer_specific_cal_skeleton as ct_cscs
import python_src.file_skeletons.ct_core_cal_check_skeleton as ct_cccs


def test_get_customer_data_stream_xml_file__check_that_generics_are_replaced(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of generic calibration information in datastream file skeleton.
    """
    # \arrange set up inputs via test fixture
    # \action call file skeleton getter
    output: str = ct_cdss.get_customer_data_stream_xml_file(ct_fixture_setup_generic_calibration_info)
    # \assert Check that generics are replaced in the file skeleton
    assert "<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>" not in output
    assert "<<<INSERT_CT_VERSION_HERE>>>" not in output
    assert ct_fixture_setup_generic_calibration_info.component_name.lower() in output
    assert ct_sr.cal_tool_version in output

def test_get_customer_data_stream_xml_file__check_that_generics_are_replaced_no_asil(ct_fixture_setup_generic_calibration_info_no_asil):
    """
    Test setting routine of generic calibration information in datastream file skeleton.
    """
    # \arrange set up inputs via test fixture
    # \action call file skeleton getter
    output: str = ct_cdss.get_customer_data_stream_xml_file(ct_fixture_setup_generic_calibration_info_no_asil)
    # \assert Check that generics are replaced in the file skeleton
    assert "<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>" not in output
    assert "<<<INSERT_CT_VERSION_HERE>>>" not in output
    assert ct_fixture_setup_generic_calibration_info_no_asil.component_name.lower() in output
    assert ct_sr.cal_tool_version in output


def test_get_customer_specific_cal_file__check_that_generics_no_arrays_given(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of generic calibration information in customer specific cal file.
    Here Endian Swap shall not be included since no arrays are given.
    """
    # \arrange set up inputs
    customer: str = "Customer_A"
    f_any_array_given: bool = False
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action call file skeleton getter
    output: str = ct_cscs.get_customer_specific_cal_file(customer,
                                                         ct_fixture_setup_generic_calibration_info, f_any_array_given,
                                                         structure_name)
    # \assert Check that generics are replaced in the file skeleton
    assert "<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_NAME_HERE>>>" not in output
    assert "<<<CUSTOMER>>>" not in output
    assert "<<<INSERT_YEAR_HERE>>>" not in output
    assert "<<<INSERT_CT_VERSION_HERE>>>" not in output
    assert "<<<INSERT_ENDIAN_SWAP_INCLUDE_HERE>>>" not in output
    assert "#include \"ct_endianness_switch.h\"" not in output
    assert ct_fixture_setup_generic_calibration_info.component_name.lower() in output
    assert structure_name in output
    assert ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) in output
    assert ct_fixture_setup_generic_calibration_info.core_structure_name in output
    assert ct_sr.cal_tool_version in output


def test_get_customer_specific_cal_file__check_that_generics_are_replaced_arrays_are_given(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of generic calibration information in customer specific cal file.
    Here Endian Swap shall be included since arrays are given.
    """
    # \arrange set up inputs
    customer: str = "Customer_A"
    f_any_array_given: bool = True
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action call file skeleton getter
    output: str = ct_cscs.get_customer_specific_cal_file(customer,
                                                         ct_fixture_setup_generic_calibration_info, f_any_array_given, structure_name)
    # \assert Check that endian swap include is introduced
    assert "<<<INSERT_ENDIAN_SWAP_INCLUDE_HERE>>>" not in output
    assert "#include \"ct_endianness_switch.h\"" in output

def test_get_customer_specific_cal_file__check_that_asil_check_is_removed_if_not_needed(
    ct_fixture_setup_generic_calibration_info,
    ct_fixture_setup_generic_calibration_info_no_asil):
    """
    Test if _Evaluate_Asil_Cal_Status method is implemented
    """
    # \arrange set up inputs
    customer: str = "Customer_A"
    f_any_array_given: bool = True
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name

    #case 1: with asil
    output: str = ct_cscs.get_customer_specific_cal_file(customer,
                                                         ct_fixture_setup_generic_calibration_info, f_any_array_given, structure_name)
    assert "_Evaluate_Asil_Cal_Status" in output

    #case 2: with no asil
    output: str = ct_cscs.get_customer_specific_cal_file(customer,
                                                         ct_fixture_setup_generic_calibration_info_no_asil, f_any_array_given, structure_name)
    assert "_Evaluate_Asil_Cal_Status" not in output


def test_get_core_cal_printing_file_skeleton__check_that_generics_are_replaced_arrays_given(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of generic calibration information in calibration printing file. Here arrays are given and thus
    reuse.h is expected to be included.
    """
    # \arrange set up inputs
    f_any_array_given: bool = True
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action call file skeleton getter
    output: str = ct_ccps.get_core_cal_printing_file_skeleton(ct_fixture_setup_generic_calibration_info, f_any_array_given, structure_name)
    # \assert Check that reuse.h include is introduced
    assert "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>" not in output
    assert "#include \"" + ct_fixture_setup_generic_calibration_info.type_include_file + "\"" in output


def test_get_core_cal_printing_file_skeleton__check_that_generics_are_replaced_no_arrays_given(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of generic calibration information in calibration printing file. Here no arrays are given and thus
    reuse.h is not expected to be included.
    """
    # \arrange set up inputs.
    f_any_array_given: bool = False
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action call file skeleton getter.
    output: str = ct_ccps.get_core_cal_printing_file_skeleton(ct_fixture_setup_generic_calibration_info, f_any_array_given, structure_name)

    # \assert Check that reuse.h include is not given while other generics are replaced.
    assert "<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>" not in output
    assert "<<<INSERT_CT_VERSION_HERE>>>" not in output
    assert "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>" not in output
    assert ct_fixture_setup_generic_calibration_info.type_include_file not in output
    assert ct_fixture_setup_generic_calibration_info.component_name.lower() in output
    assert structure_name in output
    assert ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) in output
    assert ct_sr.cal_tool_version in output


def test_get_core_header_file_skeleton__check_that_generics_are_replaced(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of generic calibration information in header file creation.
    """
    # \arrange set up inputs via test fixture
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action call file skeleton getter
    output: str = ct_chs.get_core_header_file_skeleton(ct_fixture_setup_generic_calibration_info, structure_name)

    # \assert Check that reuse.h include is introduced
    assert "<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_CAL_SIZE_MACRO_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_NAME_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>" not in output

def test_get_core_header_file_skeleton__check_that_generics_are_replaced_no_asil(ct_fixture_setup_generic_calibration_info_no_asil):
    """
    Test setting routine of generic calibration information in header file creation.
    """
    # \arrange set up inputs via test fixture
    structure_name = ct_fixture_setup_generic_calibration_info_no_asil.core_structure_name
    # \action call file skeleton getter
    output: str = ct_chs.get_core_header_file_skeleton(ct_fixture_setup_generic_calibration_info_no_asil, structure_name)

    # \assert Check that reuse.h include is introduced
    assert "<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_CAL_SIZE_MACRO_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_NAME_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>" not in output

def test_get_1d_loop_placeholder_skeleton__check_that_replacement_tags_are_replaced():
    """
    Test setting routine of a dimension for a 1d loop placeholder skeleton.
    """
    # \arrange no inputs needed for this skeleton
    # \action call file skeleton getter
    output: str = ct_cccs.get_1d_loop_placeholder_skeleton("EXAMPLE_DIM")

    # \assert Check that replacement tags are replaced
    assert "<<<INSERT_LOOP_VARIABLE_HERE>>>" not in output
    assert "<<<INSERT_DIM_HERE>>>" not in output

def test_get_2d_loop_placeholder_skeleton__check_that_replacement_tags_are_replaced():
    """
    Test setting routine of a dimension for a 2d loop placeholder skeleton.
    """
    # \arrange no inputs needed for this skeleton
    # \action call file skeleton getter
    output: str = ct_cccs.get_2d_loop_placeholder_skeleton("EXAMPLE_DIM1", "EXAMPLE_DIM2")

    # \assert Check that replacement tags are replaced
    assert "<<<INSERT_LOOP_VARIABLE_0_HERE>>>" not in output
    assert "<<<INSERT_LOOP_VARIABLE_1_HERE>>>" not in output
    assert "<<<INSERT_DIM_0_HERE>>>" not in output
    assert "<<<INSERT_DIM_1_HERE>>>" not in output

def test_get_boundary_check_skeleton__check_that_replacement_tags_are_replaced(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of a dimension for a boundary check skeleton.
    """
    # \arrange set up input via test fixture
    # \action call file skeleton getter
    output: str = ct_cccs.get_boundary_check_skeleton(ct_fixture_setup_generic_calibration_info)

    # \assert Check that replacement tags are replaced
    assert "<<<INSERT_BOUNDARY_CHECK_HERE>>>" in output

def test_get_core_cal_check_h_file_skeleton__check_that_replacement_tags_are_replaced(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of a boundary check header file skeleton.
    """
    # \arrange set up input via test fixture
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action call file skeleton getter
    output: str = ct_cccs.get_core_cal_check_h_file_skeleton(ct_fixture_setup_generic_calibration_info, structure_name)

    # \assert Check that replacement tags are replaced
    assert "<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>" not in output
    assert "<<<INSERT_COMPONENT_NAME_CAPITALIZED_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>" not in output
    assert "<<<INSERT_YEAR_HERE>>>" not in output
    assert "<<<INSERT_CT_VERSION_HERE>>>" not in output

def test_get_core_cal_check_c_file_skeleton__check_that_replacement_tags_are_replaced(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of a boundary check source file skeleton.
    """
    # \arrange set up input via test fixture
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action call file skeleton getter
    output: str = ct_cccs.get_core_cal_check_c_file_skeleton(ct_fixture_setup_generic_calibration_info, structure_name)
    # \assert Check that replacement tags are replaced
    assert "<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>" not in output
    assert "<<<INSERT_COMPONENT_NAME_CAPITALIZED_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>" not in output
    assert "<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>" not in output
    assert "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>" not in output
    assert "<<<INSERT_YEAR_HERE>>>" not in output
    assert "<<<INSERT_CT_VERSION_HERE>>>" not in output
    assert "<<<INSERT_2D_ARRAY_BOUNDARY_CHECKS_HERE>>>" in output
    assert "<<<INSERT_1D_ARRAY_BOUNDARY_CHECKS_HERE>>>" in output
    assert "<<<INSERT_CONSTANT_BOUNDARY_CHECKS_HERE>>>" in output
