"""This file defines the file skeleton for the core header file. Those f-strings are formatted in a way,
that formatting via clang-format is not necessarily required.
Additional formatting is done in the functions for generation of the strings to be replaced. That's why the string
replacement indicators are not formatted here."""
from datetime import date
from python_src.ct_shared_resources import Generic_Calibration_Info, cal_tool_version


def get_core_type_header_file_skeleton(generic_cal_info: Generic_Calibration_Info, structure_name:str) -> str:
    """
    Public function that returns a file skeleton for the core header file and adapts module name
    related replacements.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
    """

    # Header file skeleton
    core_header_file_skeleton = """\
# ifndef <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>_H
# define <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>_H

/**
* @file <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in <<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_cal.xml.
* This file is auto-generated with SFL calibration tool v<<<INSERT_CT_VERSION_HERE>>> and shall not be edited manually.
*
* @copyright Copyright (C) <<<INSERT_YEAR_HERE>>> Aptiv. All rights reserved.
*/

/*===========================================================================*\\
* Includes
\\*===========================================================================*/
#include "ct_calibration_header_t.h"
#include "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>"

/*===========================================================================*\\
* Defines
\\*===========================================================================*/

/* Macros for all calibrations */
/* Macros for array sizes for all array variables */
<<<INSERT_ARRAY_SIZES_OF_CALS_HERE>>>
/* Macros for dimension size for all array variables */
<<<INSERT_ARRAY_DIMENSIONS_OF_CALS_HERE>>>

/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define <<<INSERT_GENERIC_INFO_CAL_SIZE_MACRO_HERE>>> (<<<INSERT_CAL_SIZE_HERE>>>u)

/*===========================================================================*\\
* Typedefs
\\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
<<<INSERT_CALS_IN_BIG_ENDIAN_ORDER_HERE>>>
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
<<<INSERT_CALS_IN_LITTLE_ENDIAN_ORDER_HERE>>>
} <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>;
#endif /* CT_BIG_ENDIAN */
#endif /* <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>_H */
"""
    deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name) + "_t"

    output = core_header_file_skeleton
    # At first replace potentially control flow dependent skeletons
    # Rest of string replacements
    output = output.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<INSERT_COMPONENT_NAME_CAPITALIZED_HERE>>>", generic_cal_info.component_name.capitalize())
    output = output.replace("<<<INSERT_COMPONENT_NAME_CAPS_HERE>>>", generic_cal_info.component_name.upper())
    output = output.replace("<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>", generic_cal_info.type_include_file)
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>", structure_name)
    output = output.replace("<<<INSERT_GENERIC_INFO_CAL_SIZE_MACRO_HERE>>>", generic_cal_info.get_cal_size_macro_name(structure_name))
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>",
                            deployment_file_name.lower())
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>",
                            deployment_file_name.upper())
    output = output.replace("<<<INSERT_YEAR_HERE>>>", str(date.today().year))
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)
    return output
