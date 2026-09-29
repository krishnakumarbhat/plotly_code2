"""This file contains test implementations for the source file ct_export_cal_printing."""
from typing import Dict
import os

import python_src.ct_export_cal_printing as ct_cp
from python_src.ct_shared_resources import Calibration


def test_create_cal_printing__check_that_printing_commands_are_replaced(tmp_path, ct_fixture_setup_header_calibrations,
                                                                        ct_fixture_setup_array_fixed_point_calibrations,
                                                                        ct_fixture_setup_5_fixed_point_cals,
                                                                        ct_fixture_setup_floating_point_calibrations,
                                                                        ct_fixture_setup_floating_point_2d_array_calibration,
                                                                        ct_fixture_setup_generic_calibration_info):
    """
    Test cal printing export unit. For this a calibration dictionary is created consisting of several
    sub dictionaries.
    """
    # \arrange Set up dictionary.
    combined_dict: Dict[str: Calibration] = {**ct_fixture_setup_header_calibrations, **ct_fixture_setup_floating_point_calibrations,
                                             **ct_fixture_setup_array_fixed_point_calibrations, **ct_fixture_setup_5_fixed_point_cals,
                                             **ct_fixture_setup_floating_point_2d_array_calibration}

    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action Call calibration printing export unit
    ct_cp.create_cal_printing(combined_dict, tmp_path, ct_fixture_setup_generic_calibration_info, structure_name)

    # \assert expect that the tag id is replaced and that arrays as well as floating and fixed point calibrations
    # are returned.
    file_path = os.path.join(tmp_path,
                             ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) + "_print_functions.c")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        assert "<<<INSERT_PRINTING_COMMANDS_HERE>>>" not in content
        assert "p_cals.cal_array_5._4_,%d" in content
        assert "p_cals->cal_array_5[4]" in content
        assert "p_cals.cal_float_5,%f" in content
        assert "p_cals->cal_float_5" in content
