"""This file contains test implementations for the source file ct_shared_resources."""
from typing import List, Dict
import numpy as np

import python_src.ct_shared_resources as ct_sr


def test_generic_calibration_info_init__check_the_correct_mapping(ct_fixture_setup_generic_calibration_info):
    """
    Test setting routine of generic calibration information.
    """
    # \arrange set up a single generic calibration information via initialization routine
    # \action call initialization routine via test fixture
    core_structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name

    # \assert Check that the correct mapping is applied

    assert ct_fixture_setup_generic_calibration_info.core_structure_name == "Cf_Core_Calibration_T"
    assert ct_fixture_setup_generic_calibration_info.get_deployment_file_name(core_structure_name) == "cf_core_calibration"
    assert ct_fixture_setup_generic_calibration_info.get_cal_size_macro_name(core_structure_name) == "CF_CORE_CALIBRATION_SIZE"
    assert (np.array(ct_fixture_setup_generic_calibration_info.customers) == np.array(["Customer_A", "Customer_B"])).all()

def test_update_customer_specific_calibration_dict__update_calibration_dictionary():
    """
    Test update routine for the customer specific calibration dictionary. Check that the
    correct mapping is applied.
    """
    # \arrange set up a single Calibration customer dictionary
    test_cal = ct_sr.Calibration("test_cal", "uint8_t", "1", "0", "100", "50", False, None, None)
    customer_dict: Dict[str, str] = {"Customer_A": "10", "Customer_B": "25"}
    # \action call update function
    test_cal.update_customer_specific_calibration_dict(customer_dict)
    # \assert Check that the correct mapping is applied
    assert test_cal.customer_specific_values["Customer_A"] == customer_dict["Customer_A"]
    assert test_cal.customer_specific_values["Customer_B"] == customer_dict["Customer_B"]


def test_get_value_of_cal_for_customer__customer_mapping_is_applied():
    """
    Test getter function of customer specific calibration. Here a customer dictionary is given
    in the calibration, thus the customer specific value shall be returned.
    """
    # \arrange set up a single Calibration customer dictionary
    test_cal = ct_sr.Calibration("test_cal", "uint8_t", "1", "0", "100", "50", False, None, None)
    test_cal.customer_specific_values = {"Customer_A": "10", "Customer_B": "25"}
    # \action call getter function for customer specific value
    output_value: str = test_cal.get_value_of_cal_for_customer("Customer_A")
    # \assert Check that the customer specific value is returned
    assert output_value == "10"


def test_get_value_of_cal_for_customer__no_customer_specific_values_are_given():
    """
    Test getter function of customer specific calibration. Here the customer specific dictionary
    is empty and thus the default value shall be used as customer specific value.
    """
    # \arrange set up a single Calibration without any customer configuration
    test_cal = ct_sr.Calibration("test_cal", "uint8_t", "1", "0", "100", "50", False, None, None)
    # \action call getter function for customer specific value
    output_value: str = test_cal.get_value_of_cal_for_customer("Customer_A")
    # \assert Check that the default value is used for the given customer
    assert output_value == test_cal.default_value


def test_convert_array_string_to_list__pass_negative_floating_point_string():
    """
    Test conversion function whether it is able to return a list of negative
    floating point values. Expect that a list of length 5 is returned.
    """
    # \arrange set up a string of 5 fixed point values
    input_list: str = "[-0.05359f, -0.0459f, -0.325f, -1553.0f, -20.1f]"
    # \action call the conversion function
    output_list: List[str] = ct_sr.convert_array_string_to_list(input_list)
    # \assert Check whether the correct parsed list is returned
    assert (np.array(output_list) == np.array(["-0.05359f", "-0.0459f", "-0.325f", "-1553.0f", "-20.1f"])).all()
    assert len(output_list) == 5


def test_convert_array_string_to_list__pass_mixed_fix_point_string():
    """
    Test conversion function whether it is able to return a list of fixed point values.
    Expect that a list of length 5 is returned.
    """
    # \arrange set up a string of 5 fixed point values
    input_list: str = "[10, 30, -1, -15, -20]"
    # \action call the conversion function
    output_list: List[str] = ct_sr.convert_array_string_to_list(input_list)
    # \assert Check whether the correct parsed list is returned
    assert (np.array(output_list) == np.array(["10", "30", "-1", "-15", "-20"])).all()
    assert len(output_list) == 5


def test_convert_array_string_to_list__pass_mixed_unsigned_fix_point_string():
    """
    Test conversion function whether it is able to return a list of unsigned fixed point values.
    Expect that a list of length 5 is returned.
    """
    # \arrange set up a string of 5 fixed point values
    input_list: str = "[10u, 30u, 1u, 15u, 20u]"
    # \action call the conversion function
    output_list: List[str] = ct_sr.convert_array_string_to_list(input_list)
    # \assert Check whether the correct parsed list is returned
    assert (np.array(output_list) == np.array(["10u", "30u", "1u", "15u", "20u"])).all()
    assert len(output_list) == 5

def test_sort_dict_from_big_to_small_types__test_sorting_of_integer_types(ct_fixture_setup_5_fixed_point_cals):
    """
    Test sorting function and expect that the calibrations are sorted according to their types in the order of
    big type to small type.
    """
    # \arrange set up a dictionary with help of the test fixture class
    # \action call ordering function
    sorted_calibrations = ct_sr.sort_dict_from_big_to_small_types(ct_fixture_setup_5_fixed_point_cals)
    # \assert Check whether the correct order is returned
    assert (np.array(list(sorted_calibrations.keys())) == np.array(["cal_5", "cal_3", "cal_4", "cal_1", "cal_2"])).all()


def test_are_arrays_used_in_cals__check_that_false_is_returned_when_no_array_is_given(ct_fixture_setup_5_fixed_point_cals):
    """
    Test check whether arrays are used in the calibration dictionary. In the test fixture class there is no array entry
    given and thus false is expected.
    """
    # \arrange Set up a dictionary with help of the test fixture class
    # \action Call array check routine
    f_result = ct_sr.are_arrays_used_in_cals(ct_fixture_setup_5_fixed_point_cals)
    # \assert Check whether false is returned
    assert f_result is False


def test_are_arrays_used_in_cals__check_that_true_is_returned_when_an_array_is_given(ct_fixture_setup_array_fixed_point_calibrations):
    """
    Test check whether arrays are used in the calibration dictionary. Return value of test fixture class modified and thus
    True is expected.
    """
    # \arrange Set up a dictionary with help of the test fixture class which already contains arrays
    # \action Call array check routine
    f_result = ct_sr.are_arrays_used_in_cals(ct_fixture_setup_array_fixed_point_calibrations)
    # \assert Check whether true is returned
    assert f_result is True


def test_change_order_of_dict_by_name__sort_dictionary_by_name_list(ct_fixture_setup_5_fixed_point_cals):
    """
    Test sorting of calibrations with a given name list. Expect that the dictionary is sorted according to the input
    name list.
    """
    # \arrange Set up a dictionary with help of the test fixture class and set up an arbitrary list for sorting
    name_list: List[str] = ["cal_3", "cal_1", "cal_5", "cal_2", "cal_4"]
    # \action Call ordering function
    sorted_calibrations = ct_sr.change_order_of_dict_by_name(ct_fixture_setup_5_fixed_point_cals, name_list)
    # \assert Check whether true is returned
    assert (np.array(list(sorted_calibrations.keys())) == np.array(name_list)).all()


def test_get_cal_size__test_cal_size_calculation_with_fixed_point_values(ct_fixture_setup_5_fixed_point_cals):
    """
    Test whether the correct calibration size is returned. For this a test fixture dictionary is used.
    """
    # \arrange Set up a dictionary with help of the test fixture class.
    # \action Call array size calculation
    cal_size: str = ct_sr.get_cal_size(ct_fixture_setup_5_fixed_point_cals)
    # \assert Check cal size is equal to 10 due to the underlying types.
    assert cal_size == "10"


def test_get_cal_size__test_cal_size_calculation_arrays(ct_fixture_setup_array_fixed_point_calibrations):
    """
    Test whether the correct calibration size is returned. For this a test fixture dictionary is used
    which is containing arrays.
    """
    # \arrange Set up a dictionary which is holding arrays with help of a test fixture class.
    # \action Call array size calculation
    cal_size: str = ct_sr.get_cal_size(ct_fixture_setup_array_fixed_point_calibrations)
    # \assert Check cal size is equal to 27 due to the underlying types.
    assert cal_size == "27"


def test_get_cal_header_in_specific_order__test_little_order(ct_fixture_setup_header_calibrations, ct_fixture_setup_5_fixed_point_cals):
    """
    Test whether the correct order of h3_hdr calibrations is returned. For this several fixtures are used to check,
    that only a subset is returned in the little order.
    """
    # \arrange Set up dictionaries which are holding some calibrations and fuse them.
    combined_dict: Dict[str:ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations, **ct_fixture_setup_5_fixed_point_cals}
    # \action Call h3_hdr extraction in specific order
    output_dict: Dict[str:ct_sr.Calibration] = ct_sr.get_cal_header_in_specific_order(combined_dict, "little")
    # \assert Check that the output calibrations are a subset of the input in a specific order.
    assert (np.array(list(output_dict.keys())) ==
            np.array(["Section_Size", "version", "Section_Compatibility", "Cal_Chk_Sum", "Chk_sum_Version",
                      "Cal_Type"])).all()


def test_get_cal_header_in_specific_order__test_big_order(ct_fixture_setup_header_calibrations, ct_fixture_setup_5_fixed_point_cals):
    """
    Test whether the correct order of h3_hdr calibrations is returned. For this several fixtures are used to check,
    that only a subset is returned in the big order.
    """
    # \arrange Set up dictionaries which are holding some calibrations and fuse them.
    combined_dict: Dict[str:ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations, **ct_fixture_setup_5_fixed_point_cals}
    # \action Call h3_hdr extraction in specific order
    output_dict: Dict[str:ct_sr.Calibration] = ct_sr.get_cal_header_in_specific_order(combined_dict, "big")
    # \assert Check that the output calibrations are a subset of the input in a specific order.
    assert (np.array(list(output_dict.keys())) ==
            np.array(["Cal_Type", "Chk_sum_Version", "Cal_Chk_Sum", "Section_Compatibility", "version",
                      "Section_Size"])).all()


def test_sort_cal_dict_from_big_to_small__check_that_correct_order_is_returned(ct_fixture_setup_header_calibrations,
                                                                               ct_fixture_setup_5_fixed_point_cals,
                                                                               ct_fixture_setup_array_fixed_point_calibrations):
    """
    Tests the sorting of the overall calibrations struct in the order of hdr to small data.
    """
    # \arrange Set up dictionaries which are holding some calibrations and fuse them.
    combined_dict: Dict[str:ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations,
                                                  **ct_fixture_setup_array_fixed_point_calibrations, **ct_fixture_setup_5_fixed_point_cals}
    # \action Call sorting function
    output_dict: Dict[str:ct_sr.Calibration] = ct_sr.sort_cal_dict_from_big_to_small(combined_dict)
    # \assert Check that the output calibrations are a subset of the input in a specific order.
    expected_output_order = ["cal_array_1", "cal_5", "cal_array_2", "cal_array_3",
                             "cal_3", "cal_4", "cal_array_4", "cal_array_5", "cal_1", "cal_2"]
    assert (np.array(list(output_dict.keys())) == np.array(expected_output_order)).all()



def test_extract_dimension__first_dim_lt_second_dim(ct_fixture_setup_floating_point_2d_array_calibration):
    """
    Tests the extraction of dimensions for a nxm matrix (or array) with n<m.
    """
    # \arrange Set up dictionaries via test fixture.
    # \action Call dimension extraction
    output = ct_fixture_setup_floating_point_2d_array_calibration["cal_float_array_2d_1"].extract_dimension\
        (ct_fixture_setup_floating_point_2d_array_calibration["cal_float_array_2d_1"].default_value)
    # \assert Check dimensions are set accordingly.
    assert output[0] == 2
    assert output[1] == 3


def test_extract_dimension__first_dim_gt_second_dim(ct_fixture_setup_floating_point_2d_array_calibration):
    """
    Tests the extraction of dimensions for a nxm matrix (or array) with n>m.
    """
    # \arrange Set up dictionaries via test fixture.
    # \action Call dimension extraction
    output = ct_fixture_setup_floating_point_2d_array_calibration["cal_float_array_2d_2"].extract_dimension\
        (ct_fixture_setup_floating_point_2d_array_calibration["cal_float_array_2d_2"].default_value)
    # \assert Check dimensions are set accordingly.
    assert output[0] == 3
    assert output[1] == 2
