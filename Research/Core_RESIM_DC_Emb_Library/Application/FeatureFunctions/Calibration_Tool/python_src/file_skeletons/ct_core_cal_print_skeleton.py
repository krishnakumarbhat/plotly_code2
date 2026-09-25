"""This file defines the file skeleton for calibration printing. Those f-strings are formatted in a way,
that formatting via clang-format is not necessarily required.
Additional formatting is done in the functions for generation of the strings to be replaced. That's why the string
replacement indicators are not formatted here."""
from datetime import date
from python_src.ct_shared_resources import Generic_Calibration_Info, cal_tool_version


def get_core_cal_printing_file_skeleton(generic_cal_info: Generic_Calibration_Info,
                                        f_array_includes: bool, structure_name:str) -> str:
    """
    Public function that returns a file skeleton for the core calibration printing file and adapts module name
    related replacements.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema
        f_array_includes (bool): Flag indicating whether any array is given in the calibrations dictionary.

    Returns:
        Returns the file skeleton as formatted string with replacements
    """

    # Define file skeletons inside for encapsulation
    core_cal_printing_file_skeleton = """\
/**
* @file <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in <<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_cal.xml.
* This file is auto-generated with SFL calibration tool v<<<INSERT_CT_VERSION_HERE>>> and shall not be edited manually.
*
* @copyright Copyright (C) <<<INSERT_YEAR_HERE>>> Aptiv. All rights reserved.
*/

#include "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>_t.h" // IWYU pragma: keep
#include "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
<<<INSERT_MODIFIED_BASIC_TYPE_INCLUDE_HERE>>>
#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void <<<METHOD_NAME_PREFIX>>>_Print(FILE* c_file_ptr, const <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
<<<INSERT_PRINTING_COMMANDS_HERE>>>
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
"""

    # Helper string
    type_include_file_skeleton = """\
#include "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>"
"""

    deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
    method_name_prefix = generic_cal_info.get_method_name_prefix(structure_name)

    # Search and replace basic information of the file skeleton
    output = core_cal_printing_file_skeleton.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>", structure_name)
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>",
                            deployment_file_name)
    output = output.replace("<<<INSERT_COMPONENT_NAME_HERE>>>",
                            generic_cal_info.component_name.capitalize())
    output = output.replace("<<<INSERT_YEAR_HERE>>>", str(date.today().year))
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)
    assert structure_name[-2:] == "_T"
    output = output.replace("<<<METHOD_NAME_PREFIX>>>", method_name_prefix)

    modified_type_include_string: str = type_include_file_skeleton
    modified_type_include_string = modified_type_include_string.replace("<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>",
                                                                        generic_cal_info.type_include_file)
    if not f_array_includes:
        modified_type_include_string = ""

    output = output.replace("<<<INSERT_MODIFIED_BASIC_TYPE_INCLUDE_HERE>>>", modified_type_include_string)

    return output
