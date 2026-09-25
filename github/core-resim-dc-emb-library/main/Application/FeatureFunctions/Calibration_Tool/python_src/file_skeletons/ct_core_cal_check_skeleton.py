"""This file defines the file skeleton for calibration boundary checks. Those f-strings are formatted in a way,
that formatting via clang-format is not necessarily required.
Additional formatting is done in the functions for generation of the strings to be replaced. That's why the string
replacement indicators are not formatted here."""
from datetime import date
from python_src.ct_shared_resources import Generic_Calibration_Info, cal_tool_version

loop_variables = ["x","y"]

def get_1d_loop_placeholder_skeleton(dimension: str) -> str:
    """Public function that returns a skeleton for 1D loops.
     Args:
         dimension (str) : Name of the respective dimension used for looping
     Returns:
         Returns the loop skeleton with basic string replacements
     """
    core_cal_1d_loop_skeleton = """\
{
    uint8_t <<<INSERT_LOOP_VARIABLE_HERE>>>;
    for (<<<INSERT_LOOP_VARIABLE_HERE>>> = 0u; <<<INSERT_LOOP_VARIABLE_HERE>>> < <<<INSERT_DIM_HERE>>>; <<<INSERT_LOOP_VARIABLE_HERE>>>++)
        {
        <<<INSERT_CONTENT_HERE>>>
        }
}
"""
    output = core_cal_1d_loop_skeleton
    output = output.replace("<<<INSERT_LOOP_VARIABLE_HERE>>>", loop_variables[0])
    output = output.replace("<<<INSERT_DIM_HERE>>>", dimension)
    return output

def get_2d_loop_placeholder_skeleton(dim0: str, dim1: str) -> str:
    """
    Public function that returns a skeleton for 2D loops.

    Args:
        dim0 (str) : Name of the first dimension used for looping
        dim1 (str) : Name of the second dimension used for looping

    Returns:
        Returns the loop skeleton with basic string replacements
    """
    core_cal_2d_loop_skeleton = """\
{
    uint8_t <<<INSERT_LOOP_VARIABLE_0_HERE>>>;
    for (<<<INSERT_LOOP_VARIABLE_0_HERE>>> = 0u; <<<INSERT_LOOP_VARIABLE_0_HERE>>> < <<<INSERT_DIM_0_HERE>>>; <<<INSERT_LOOP_VARIABLE_0_HERE>>>++)
    {
        uint8_t <<<INSERT_LOOP_VARIABLE_1_HERE>>>;
        for (<<<INSERT_LOOP_VARIABLE_1_HERE>>> = 0u; <<<INSERT_LOOP_VARIABLE_1_HERE>>> < <<<INSERT_DIM_1_HERE>>>; <<<INSERT_LOOP_VARIABLE_1_HERE>>>++)
        {
            <<<INSERT_CONTENT_HERE>>>
        }
    }
}
"""
    output = core_cal_2d_loop_skeleton
    output = output.replace("<<<INSERT_LOOP_VARIABLE_0_HERE>>>", loop_variables[0])
    output = output.replace("<<<INSERT_LOOP_VARIABLE_1_HERE>>>", loop_variables[1])
    output = output.replace("<<<INSERT_DIM_0_HERE>>>", dim0)
    output = output.replace("<<<INSERT_DIM_1_HERE>>>", dim1)
    return output

def get_boundary_check_skeleton(_generic_cal_info: Generic_Calibration_Info) -> str:
    """
    Public function that returns a boundary check skeleton for a single calibration.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
    """
    boundary_check_skeleton = """\
<<<INSERT_BOUNDARY_CHECK_HERE>>>;
"""
    output = boundary_check_skeleton
    output = output.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", _generic_cal_info.component_name.lower())
    return output

def get_core_cal_check_h_file_skeleton(generic_cal_info: Generic_Calibration_Info, structure_name:str) -> str:
    """
    Public function that returns a file skeleton for the calibration boundary check header file.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
    """
    sdd_dict = {
                "Bsis":{
                    "Bsis_Core_Calibration_T":"CSCSA-216460"
                },
                "Ced":{
                    "Ced_Core_Calibration_T":"CSCSA-186577"
                },
                "Cta":{
                    "Cta_Core_Calibration_T":"CSCSA-186506"
                },
                "Esa":{
                    "Esa_Core_Calibration_T":"CSCSA-216546"
                },
                "Lcda":{
                    "Lcda_Core_Calibration_T":"CSCSA-186587"
                },
                "Ltb":{
                    "Ltb_Core_Calibration_T":"CSCSA-216629"
                },
                "Mois":{
                    "Mois_Core_Calibration_T":"CSCSA-216668"
                },
                "Pt":{
                    "Pt_Core_Calibration_T":"CSCSA-216698"
                },
                "Rdd":{
                    "Rdd_Core_Calibration_T":"CSCSA-216735"
                },
                "Recw":{
                    "Recw_Core_Calibration_T":"CSCSA-186520"
                },
                "Scw":{
                    "Scw_Core_Calibration_T":"CSCSA-218590"
                },
                "Ta":{
                    "Ta_Core_Calibration_T":"CSCSA-216777"
                },
                "Tods":{
                    "Tods_Core_Calibration_T":"CSCSA-216759"
                },
                "Fbk":{},
                "Cf":{}
            }
    sdd_numbers = sdd_dict[generic_cal_info.component_name].get(structure_name, "n/a")
    sdd_number = sdd_numbers if isinstance(sdd_numbers, str) else sdd_numbers[-1]
    sdd_number = f"{{{sdd_number}}}"
    # Header file skeleton
    core_cal_check_header_skeleton = f"""\
/**
* @file <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>_check.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides boundary checks for the calibrations defined in <<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_cal.xml.
* This file is auto-generated with SFL calibration tool v<<<INSERT_CT_VERSION_HERE>>> and shall not be edited manually.
*
* @copyright Copyright (C) <<<INSERT_YEAR_HERE>>> Aptiv. All rights reserved.
*/
#ifndef <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>_CAL_CHECK_H
#define <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>_CAL_CHECK_H

/**************************************************
 * Includes
 **************************************************/

#include "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>_t.h"
#include "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>"

/**************************************************
 * Global function declaration
 **************************************************/

/**
 * @brief Checks whether all calibrations of <<<INSERT_COMPONENT_NAME_CAPITALIZED_HERE>>> are in given boundaries
 *
 * @return True when containing calibrations are within their boundaries
 *
 * @SRS{{n/a}}
 * @SAE{{n/a}}
 * @SDD{sdd_number}
 * @verification{"{Create a superordinate test to check whether all calibrations are in given boundaries}"}
 **/
boolean_T <<<METHOD_NAME_PREFIX>>>_In_Boundary(const <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>> *p_calibration);

#endif /*<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>_CAL_CHECK_H*/

"""
    deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
    method_name_prefix = generic_cal_info.get_method_name_prefix(structure_name)

    output = core_cal_check_header_skeleton
    output = output.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<INSERT_COMPONENT_NAME_CAPITALIZED_HERE>>>", generic_cal_info.component_name.capitalize())
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>",
         deployment_file_name.upper())
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>",
         deployment_file_name)
    assert structure_name[-2:] == "_T"
    output = output.replace("<<<METHOD_NAME_PREFIX>>>", method_name_prefix)
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>", structure_name)
    output = output.replace("<<<INSERT_YEAR_HERE>>>", str(date.today().year))
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)
    output = output.replace("<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>", generic_cal_info.type_include_file)
    return output


def get_core_cal_check_c_file_skeleton(generic_cal_info: Generic_Calibration_Info, structure_name:str) -> str:
    """
    Public function that returns a file skeleton for the calibration boundary check source file.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
    """
    # Header file skeleton
    core_cal_check_source_skeleton = """\
/**
* @file <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in <<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_cal.xml.
* This file is auto-generated with SFL calibration tool v<<<INSERT_CT_VERSION_HERE>>> and shall not be edited manually.
*
* @copyright Copyright (C) <<<INSERT_YEAR_HERE>>> Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>_check.h" // IWYU pragma: keep
#include "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T <<<METHOD_NAME_PREFIX>>>_In_Boundary(const <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>> *p_calibration)
{
   boolean_T f_<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   <<<INSERT_2D_ARRAY_BOUNDARY_CHECKS_HERE>>>
   <<<INSERT_1D_ARRAY_BOUNDARY_CHECKS_HERE>>>
   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   <<<INSERT_CONSTANT_BOUNDARY_CHECKS_HERE>>>

   return f_<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_calibration_in_boundaries;
}

"""
    deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
    method_name_prefix = generic_cal_info.get_method_name_prefix(structure_name)

    output = core_cal_check_source_skeleton
    output = output.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<INSERT_COMPONENT_NAME_CAPITALIZED_HERE>>>", generic_cal_info.component_name.capitalize())
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>",
         deployment_file_name)
    output = output.replace("<<<METHOD_NAME_PREFIX>>>", method_name_prefix)
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>", structure_name)
    output = output.replace("<<<INSERT_YEAR_HERE>>>", str(date.today().year))
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)
    return output
