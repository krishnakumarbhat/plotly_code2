"""This file contains test implementations for the source file ct_export_cal_check_files."""
from typing import Dict
import os

import python_src.ct_shared_resources as ct_sr
import python_src.ct_export_cal_check_files as ct_eccf
def test_create_cal_check_files__create_a_boundary_check_source_file(tmp_path, ct_fixture_setup_header_calibrations,
    ct_fixture_setup_floating_point_2d_array_calibration, ct_fixture_setup_5_fixed_point_cals, ct_fixture_setup_bool_cals , ct_fixture_setup_int_cals,
    ct_fixture_setup_floating_point_array_calibration, ct_fixture_setup_generic_calibration_info):
    """
    Test boundary calibration check for a given set of calibrations.
    """
    # \arrange Set up calibration dictionary and other inputs
    input_dict: Dict[str, ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations,
                                            **ct_fixture_setup_floating_point_2d_array_calibration, **ct_fixture_setup_bool_cals, **ct_fixture_setup_int_cals,
                                            **ct_fixture_setup_5_fixed_point_cals, **ct_fixture_setup_floating_point_array_calibration}
    input_dict = ct_sr.sort_dict_from_big_to_small_types(input_dict)
    # \action Call boundary check creation
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    ct_eccf.create_cal_check_files(input_dict, tmp_path, ct_fixture_setup_generic_calibration_info, structure_name)
    # \assert expect that specific properties are set.
    file_path = os.path.join(tmp_path, ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) + "_check.c")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        # Check that the correct formatting for 2 dimensional arrays is available.
        assert "Ct_Is_Float_In_Bondaries(&f_cf_calibration_in_boundaries, CF_MIN_CAL_FLOAT_ARRAY_2D_2," in content
        assert "p_calibration->cal_float_array_2d_2[x][y], CF_MAX_CAL_FLOAT_ARRAY_2D_2);" in content
        # Check that the correct formatting for 1 dimensional arrays is available.
        assert "Ct_Is_Float_In_Bondaries(&f_cf_calibration_in_boundaries, CF_MIN_CAL_FLOAT_ARRAY_1," in content
        assert "p_calibration->cal_float_array_1[x], CF_MAX_CAL_FLOAT_ARRAY_1);" in content
        # Check that the correct formatting for constants is available.
        assert "Ct_Is_Uint32_In_Bondaries(&f_cf_calibration_in_boundaries, 0, p_calibration->cal_5, CF_MAX_CAL_5);" in content


def test_create_cal_check_files__create_a_boundary_check_header_file(tmp_path, ct_fixture_setup_header_calibrations,
    ct_fixture_setup_floating_point_2d_array_calibration, ct_fixture_setup_generic_calibration_info):
    """
    Test boundary check declaration creation.
    """
    # \arrange Set up calibration dictionary and other inputs
    input_dict: Dict[str, ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations,
        **ct_fixture_setup_floating_point_2d_array_calibration}
    input_dict = ct_sr.sort_dict_from_big_to_small_types(input_dict)

    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name

    # \action Call boundary check creation
    ct_eccf.create_cal_check_files(input_dict, tmp_path, ct_fixture_setup_generic_calibration_info, structure_name)

    # \assert expect that specific properties are set.
    file_path = os.path.join(tmp_path, ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) + "_check.h")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        #Check that the correct function declaration is given.
        assert "boolean_T Cf_Core_Cal_In_Boundary(const Cf_Core_Calibration_T *p_calibration)" in content
