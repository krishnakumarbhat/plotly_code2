"""This file defines the file skeleton for calibration update. Those f-strings are formatted in a way,
that formatting via clang-format is not necessarily required.
Additional formatting is done in the functions for generation of the strings to be replaced. That's why the string
replacement indicators are not formatted here."""
from datetime import date
from python_src.ct_shared_resources import Generic_Calibration_Info, cal_tool_version

def get_update_calibration_method_declaration_skeleton():
    return """\
/**
 * @brief This function updates common part of <<<DST_TYPE>>> and <<<SRC_TYPE>>> to their value from cal_src.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
boolean_T <<<METHOD_NAME>>>(<<<DST_TYPE>>>* cal_dst, const <<<SRC_TYPE>>>* cal_src);
"""

def get_update_calibration_h_file_skeleton(generic_cal_info: Generic_Calibration_Info, file_name:str) -> str:
    """
    Public function that returns a file skeleton for the update_calibration header file.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
    """
    # Header file skeleton
    update_calibration_header_skeleton = """\
/**
* @file <<<THIS_FILE_NAME>>>.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides update_calibration methods for the calibrations defined in <<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_cal.xml.
* This file is auto-generated with SFL calibration tool v<<<INSERT_CT_VERSION_HERE>>> and shall not be edited manually.
*
* @copyright Copyright (C) <<<INSERT_YEAR_HERE>>> Aptiv. All rights reserved.
*/
#ifndef <<<THIS_FILE_NAME_UPPERCASE>>>_H
#define <<<THIS_FILE_NAME_UPPERCASE>>>_H

/**************************************************
 * Includes
 **************************************************/

#include "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>"
<<<INSERT_INCLUDES_FOR_CALIBRATION_DEFINITION_HERE>>>
/**************************************************
 * Global function declaration
 **************************************************/

<<<INSERT_UPDATE_CALIBRATION_METHOD_DECLARATION_HERE>>>

#endif /* <<<THIS_FILE_NAME_UPPERCASE>>>_H */

"""
    output = update_calibration_header_skeleton
    output = output.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<THIS_FILE_NAME>>>",
         file_name)
    output = output.replace("<<<THIS_FILE_NAME_UPPERCASE>>>",
         file_name.upper())
    output = output.replace("<<<INSERT_YEAR_HERE>>>", str(date.today().year))
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)
    output = output.replace("<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>", generic_cal_info.type_include_file)
    return output

def get_update_calibration_method_definition_skeleton():
    return """
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T <<<METHOD_NAME>>>(<<<DST_TYPE>>>* cal_dst, const <<<SRC_TYPE>>>* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!<<<SRC_METHOD_NAME_PREFIX>>>_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
<<<INSERT_CAL_UPDATING_WITH_CUSTOM_VALUES_HERE>>>
        f_result = (boolean_T) 1;
    }
    return f_result;
}
"""

def get_update_calibration_c_file_skeleton(generic_cal_info: Generic_Calibration_Info, file_name:str) -> str:
    """
    Public function that returns a file skeleton for the calibration update source file.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
    """
    # Header file skeleton
    update_calibration_source_skeleton = """\
/**
* @file <<<THIS_FILE_NAME>>>.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in <<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_cal.xml.
* This file is auto-generated with SFL calibration tool v<<<INSERT_CT_VERSION_HERE>>> and shall not be edited manually.
*
* @copyright Copyright (C) <<<INSERT_YEAR_HERE>>> Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "<<<THIS_FILE_NAME>>>.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>"
<<<INSERT_INCLUDES_FOR_CALIBRATION_DEFINITION_HERE>>>

/**************************************************
 * Global function definition
 **************************************************/

<<<INSERT_UPDATE_CALIBRATION_METHOD_DEFINITION_HERE>>>

"""

    output = update_calibration_source_skeleton
    output = output.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<INSERT_YEAR_HERE>>>", str(date.today().year))
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)
    output = output.replace("<<<THIS_FILE_NAME>>>", file_name)
    output = output.replace("<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>", generic_cal_info.type_include_file)
    return output
