"""This file defines the file skeleton for the core header file. Those f-strings are formatted in a way,
that formatting via clang-format is not necessarily required.
Additional formatting is done in the functions for generation of the strings to be replaced. That's why the string
replacement indicators are not formatted here."""
from datetime import date
from python_src.ct_shared_resources import Generic_Calibration_Info, cal_tool_version


def get_core_header_file_skeleton(generic_cal_info: Generic_Calibration_Info, structure_name:str) -> str:
    """
    Public function that returns a file skeleton for the core header file and adapts module name
    related replacements.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
    """
    sdd_dict = {
                "Bsis":{
                    "Bsis_Core_Calibration_T":["CSCSA-216481", "CSCSA-216479", "CSCSA-216480"]
                },
                "Ced":{
                    "Ced_Core_Calibration_T":["CSCSA-218554", "CSCSA-186574", "CSCSA-186573"]
                },
                "Cta":{
                    "Cta_Core_Calibration_T":["CSCSA-218557", "CSCSA-186433", "CSCSA-186433"]
                },
                "Esa":{
                    "Esa_Core_Calibration_T":["CSCSA-216530", "CSCSA-216528", "CSCSA-216529"]
                },
                "Lcda":{
                    "Lcda_Core_Calibration_T":["CSCSA-216549", "CSCSA-186582", "CSCSA-186583"]
                },
                "Ltb":{
                    "Ltb_Core_Calibration_T":["CSCSA-216641", "CSCSA-216642", "CSCSA-216639"]
                },
                "Mois":{
                    "Mois_Core_Calibration_T":["CSCSA-216676", "CSCSA-216677", "CSCSA-216674"]
                },
                "Pt":{
                    "Pt_Core_Calibration_T":["CSCSA-216703", "CSCSA-216704", "CSCSA-216702"]
                },
                "Rdd":{
                    "Rdd_Core_Calibration_T":["CSCSA-216738", "CSCSA-216741", "CSCSA-216740"]
                },
                "Recw":{
                    "Recw_Core_Calibration_T":["CSCSA-216746", "CSCSA-186530", "CSCSA-186531"]
                },
                "Scw":{
                    "Scw_Core_Calibration_T":["CSCSA-218602", "CSCSA-218603", "CSCSA-218600"]
                },
                "Ta":{
                    "Ta_Core_Calibration_T":["CSCSA-216783", "CSCSA-216784", "CSCSA-216781"]
                },
                "Tods":{
                    "Tods_Core_Calibration_T":["CSCSA-216764", "CSCSA-216766", "CSCSA-216767"]
                },
                "Fbk":{},
                "Cf":{}
            }
    sdd_numbers = sdd_dict[generic_cal_info.component_name].get(structure_name, "{n/a}")
    sdd_number_1 = sdd_number_2 = sdd_number_3 = "{n/a}"

    if not isinstance(sdd_numbers, str):
        sdd_number_1 = f"{{{sdd_numbers[0]}}}"
        sdd_number_2 = f"{{{sdd_numbers[1]}}}"
        sdd_number_3 = f"{{{sdd_numbers[2]}}}"

    # Header file skeleton
    core_header_file_skeleton = f"""\
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
#include "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>_t.h"
#include "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\\
* Defines
\\*===========================================================================*/

/* Macros for minimum range of calibrations */
<<<INSERT_MINIMUM_VALUE_OF_CAL_DEFINES_HERE>>>
/* Macros for maximum range of calibrations */
<<<INSERT_MAXIMUM_VALUE_OF_CAL_DEFINES_HERE>>>
<<<INSERT_ASIL_CHECK_ENUM_DEFINITION_HERE>>>
/*===========================================================================*\\
* Global function declarations
\\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
/**
 * @brief Reverses arrays with variable length of 1, 2 or 4 bytes. Dependent on whether arrays are given.
 *
 * @return void
 *
 * @SRS{{n/a}}
 * @SAE{{n/a}}
 * @SDD{sdd_number_1}
 * @verification{{}}
 **/
void <<<METHOD_NAME_PREFIX>>>_Reverse_Array_<<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_NAME_HERE>>>(<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>* cal_dst);
#endif /*CT_BIG_ENDIAN*/

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * @brief Prints values of calibrations.
 *
 * @return void
 *
 * @SRS{{n/a}}
 * @SAE{{n/a}}
 * @SDD{sdd_number_2}
 * @verification{"{}"}
 **/
void <<<METHOD_NAME_PREFIX>>>_Print(FILE* c_file_ptr, const <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>* p_cals);

#endif /*CT_ACTIVATE_CAL_PRINT*/

/**
 * @brief This function updates all calibrations of the component to their respective defaults given by the customer specific xml sheet.
 *
 * @return void
 *
 * @SRS{{n/a}}
 * @SAE{{n/a}}
 * @SDD{sdd_number_3}
 * @verification{"{}"}
 **/
void <<<METHOD_NAME_PREFIX>>>_Update_Defaults(<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>* cal_dst);

<<<INSERT_ASIL_CHECK_DECLARATION_SKELETON_HERE>>>

#endif /* <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>_H */
"""
    # Helper strings
    asil_check_declaration_skeleton = """\
/**
 * @brief This function checks whether the initial values set as calibrations of component <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>,
 * have the correct values indicated by the core xml as well as the customer specific calibration xml file.
 *
 * @return Enums state indicating whether initial calibration value check was successful. 
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
enum <<<METHOD_NAME_PREFIX>>>_Asil_Status <<<METHOD_NAME_PREFIX>>>_Evaluate_Asil_Cal_Status(const <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>* p_cals);
"""

    asil_check_enum_definition_skeleton = """\
/*===========================================================================*\\
* Enums
\\*===========================================================================*/

enum <<<METHOD_NAME_PREFIX>>>_Asil_Status {<<<INSERT_COMPONENT_NAME_CAPS_HERE>>>_ASIL_STATUS_SUCCESS = 0xA5A5, <<<INSERT_COMPONENT_NAME_CAPS_HERE>>>_ASIL_STATUS_FAIL = 0xA0A0};
"""
    deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
    method_name_prefix = generic_cal_info.get_method_name_prefix(structure_name)

    output = core_header_file_skeleton
    # At first replace potentially control flow dependent skeletons
    if generic_cal_info.export_asil_check_flag:
        output = output.replace("<<<INSERT_ASIL_CHECK_ENUM_DEFINITION_HERE>>>", asil_check_enum_definition_skeleton)
        output = output.replace("<<<INSERT_ASIL_CHECK_DECLARATION_SKELETON_HERE>>>", asil_check_declaration_skeleton)
    else:
        output = output.replace("<<<INSERT_ASIL_CHECK_ENUM_DEFINITION_HERE>>>", "")
        output = output.replace("<<<INSERT_ASIL_CHECK_DECLARATION_SKELETON_HERE>>>", "")
    # Rest of string replacements
    output = output.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<INSERT_COMPONENT_NAME_CAPS_HERE>>>", generic_cal_info.component_name.upper())
    output = output.replace("<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>", generic_cal_info.type_include_file)
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>", structure_name)
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_CAPS_HERE>>>", structure_name.upper())
    output = output.replace("<<<INSERT_GENERIC_INFO_CAL_SIZE_MACRO_HERE>>>", generic_cal_info.get_cal_size_macro_name(structure_name))
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_NAME_HERE>>>", generic_cal_info.component_name.capitalize() +"_Cal")
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>",
                            deployment_file_name)
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_UPPER_CASE_HERE>>>",
                            deployment_file_name.upper())
    assert structure_name[-2:] == "_T"
    output = output.replace("<<<METHOD_NAME_PREFIX>>>", method_name_prefix)
    output = output.replace("<<<INSERT_YEAR_HERE>>>", str(date.today().year))
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)
    return output
