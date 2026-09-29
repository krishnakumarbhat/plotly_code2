"""This file contains test implementations for the source file ct_export_customer_cals."""
import os

import python_src.ct_parser as ct_p
import python_src.ct_export_update_calibration as ct_euc

expected_h_file = """\
/**
* @file update_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in cf_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/
#ifndef UPDATE_CALIBRATION_H
#define UPDATE_CALIBRATION_H

/**************************************************
 * Includes
 **************************************************/

#include "reuse.h"
#include "cf_core_calibration_t.h"
#include "cf_public_calibration_t.h"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief This function updates common part of Cf_Core_Calibration_T and Cf_Public_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Test_Update_Core_Calibration_By_Public(Cf_Core_Calibration_T* cal_dst, const Cf_Public_Calibration_T* cal_src);
/**
 * @brief This function updates common part of Cf_Core_Calibration_T and Cf_Core_Calibration_T to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T Test_Update_Core_Calibration_By_Core(Cf_Core_Calibration_T* cal_dst, const Cf_Core_Calibration_T* cal_src);"""
expected_c_file = """\
/**
* @file update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in cf_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "reuse.h"
#include "cf_core_calibration_check.h"
#include "cf_core_calibration_t.h"
#include "cf_public_calibration_check.h"
#include "cf_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Test_Update_Core_Calibration_By_Public(Cf_Core_Calibration_T* cal_dst, const Cf_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Cf_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->cal_float_array_1[0] = cal_src->cal_float_array_1[0];
        cal_dst->cal_float_array_1[1] = cal_src->cal_float_array_1[1];
        cal_dst->cal_i_am_a_2d_array[0][0] =         cal_src->cal_i_am_a_2d_array[0][0];
        cal_dst->cal_i_am_a_2d_array[0][1] =         cal_src->cal_i_am_a_2d_array[0][1];
        cal_dst->cal_i_am_a_2d_array[0][2] =         cal_src->cal_i_am_a_2d_array[0][2];
        cal_dst->cal_i_am_a_2d_array[1][0] =         cal_src->cal_i_am_a_2d_array[1][0];
        cal_dst->cal_i_am_a_2d_array[1][1] =         cal_src->cal_i_am_a_2d_array[1][1];
        cal_dst->cal_i_am_a_2d_array[1][2] =         cal_src->cal_i_am_a_2d_array[1][2];
        cal_dst->cal_i_am_a_2d_array[2][0] =         cal_src->cal_i_am_a_2d_array[2][0];
        cal_dst->cal_i_am_a_2d_array[2][1] =         cal_src->cal_i_am_a_2d_array[2][1];
        cal_dst->cal_i_am_a_2d_array[2][2] =         cal_src->cal_i_am_a_2d_array[2][2];
        cal_dst->cal_5 = cal_src->cal_5;
        cal_dst->cal_3 = cal_src->cal_3;
        cal_dst->cal_4 = cal_src->cal_4;
        cal_dst->cal_1 = cal_src->cal_1;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Test_Update_Core_Calibration_By_Core(Cf_Core_Calibration_T* cal_dst, const Cf_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Cf_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->cal_float_array_2[0] = cal_src->cal_float_array_2[0];
        cal_dst->cal_float_array_2[1] = cal_src->cal_float_array_2[1];
        cal_dst->cal_float_array_1[0] = cal_src->cal_float_array_1[0];
        cal_dst->cal_float_array_1[1] = cal_src->cal_float_array_1[1];
        cal_dst->cal_i_am_a_2d_array[0][0] =         cal_src->cal_i_am_a_2d_array[0][0];
        cal_dst->cal_i_am_a_2d_array[0][1] =         cal_src->cal_i_am_a_2d_array[0][1];
        cal_dst->cal_i_am_a_2d_array[0][2] =         cal_src->cal_i_am_a_2d_array[0][2];
        cal_dst->cal_i_am_a_2d_array[1][0] =         cal_src->cal_i_am_a_2d_array[1][0];
        cal_dst->cal_i_am_a_2d_array[1][1] =         cal_src->cal_i_am_a_2d_array[1][1];
        cal_dst->cal_i_am_a_2d_array[1][2] =         cal_src->cal_i_am_a_2d_array[1][2];
        cal_dst->cal_i_am_a_2d_array[2][0] =         cal_src->cal_i_am_a_2d_array[2][0];
        cal_dst->cal_i_am_a_2d_array[2][1] =         cal_src->cal_i_am_a_2d_array[2][1];
        cal_dst->cal_i_am_a_2d_array[2][2] =         cal_src->cal_i_am_a_2d_array[2][2];
        cal_dst->cal_5 = cal_src->cal_5;
        cal_dst->cal_3 = cal_src->cal_3;
        cal_dst->cal_4 = cal_src->cal_4;
        cal_dst->cal_2 = cal_src->cal_2;
        cal_dst->cal_1 = cal_src->cal_1;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;
        cal_dst->k_unused_padding_byte_1 = cal_src->k_unused_padding_byte_1;

        f_result = (boolean_T) 1;
    }
    return f_result;
}"""

def test_create_update_calibration_files__create_example_c_file(tmp_path):
    """
    Test customer file extraction based on an example dictionary without any customer specific values.
    """
    # \arrange Set up calibration dictionary and modify it, such that customer specific values are given.
    calibration_path = os.path.join(os.path.dirname(__file__), "..", "example_files")
    path_to_main_xml = os.path.join(calibration_path, "Core", "cool_feature_cal.xml")
    # \action Call customer specific file creation
    core_cal_dict, _, generic_cal_info = ct_p.parse_calibration_xml(path_to_main_xml, calibration_path)

    customer_calibration_path = tmp_path/"Customer_A"
    customer_calibration_path.mkdir()
    file_name = "update_calibration"

    core_structure_name = generic_cal_info.core_structure_name
    public_structure_name = generic_cal_info.public_structure_name

    public_cal_dict = {k: v for k, v in core_cal_dict.items() if not v.f_is_constant}
    update_calibration_methods = {
            "Test_Update_Core_Calibration_By_Public" : (core_structure_name, public_structure_name),
            "Test_Update_Core_Calibration_By_Core" : (core_structure_name, core_structure_name),
        }
    calibrations_dict_of_dicts = {
            core_structure_name: core_cal_dict,
            public_structure_name: public_cal_dict,
        }

    # \action Call customer specific file creation
    ct_euc.create_update_calibration_files(generic_cal_info, customer_calibration_path, file_name,
        update_calibration_methods, calibrations_dict_of_dicts)

    # \assert expect that specific properties are set and that 2d array format is fulfilled in row-major-order
    file_path = os.path.join(customer_calibration_path, file_name + ".h")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        assert "Test_Update_Core_Calibration_By_Public" in content
        assert expected_h_file in content

    file_path = os.path.join(customer_calibration_path, file_name + ".c")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        assert "Test_Update_Core_Calibration_By_Public" in content
        assert expected_c_file in content
