"""This file defines the file skeleton for customer specific calibration c-files. Those f-strings are formatted in a way,
that formatting via clang-format is not necessarily required.
Additional formatting is done in the functions for generation of the strings to be replaced. That's why the string
replacement indicators are not formatted here."""
from datetime import date
from python_src.ct_shared_resources import Generic_Calibration_Info, cal_tool_version


def get_customer_specific_cal_file(customer: str, generic_cal_info: Generic_Calibration_Info, f_array_includes: bool, structure_name:str) -> str:
    """
    Public function that returns a file skeleton for the customer specific calibration .c-file and adapts module and
    customer name related replacements.

    Args:
          customer (str) : Name of the customer to indicate in the file header to which customer the file belongs to.
          generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema.
          f_array_includes (bool): Flag indicating whether any array is given in the calibrations dictionary.
    Returns:
       Returns the file skeleton as formatted string with replacements
    """

    # Define the file skeleton for the customer specific calibration files
    customer_specific_cal_file = """\

/**
* @file <<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the <<<CUSTOMER>>> specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in <<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_cal.xml.
* This file is auto-generated with SFL calibration tool v<<<INSERT_CT_VERSION_HERE>>> and shall not be edited manually.
*
* @copyright Copyright (C) <<<INSERT_YEAR_HERE>>> Aptiv. All rights reserved.
*/

#include "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>_t.h"
#include "<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>.h"
#include <string.h>
<<<INSERT_ENDIAN_SWAP_INCLUDE_HERE>>>
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void <<<METHOD_NAME_PREFIX>>>_Update_Defaults(<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>* cal_dst)
{
    <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>> default_calibration= 
    <<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_INITIALIZATION_HERE>>>
/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void <<<METHOD_NAME_PREFIX>>>_Reverse_Array_<<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_NAME_HERE>>>(<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>* cal_dst)
{
<<<INSERT_REVERSING_OF_CAL_ARRAYS_HERE>>>
}
#endif /* CT_BIG_ENDIAN */

<<<INSERT_ASIL_CHECK_DEFINITION_SKELETON_HERE>>>
"""

    # Helper strings
    # Asil checker related include skeletons
    asil_check_definition_skeleton = """\
/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
enum <<<METHOD_NAME_PREFIX>>>_Asil_Status <<<METHOD_NAME_PREFIX>>>_Evaluate_Asil_Cal_Status(const <<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>* p_cals)
{
   enum <<<METHOD_NAME_PREFIX>>>_Asil_Status asil_check_status = <<<INSERT_COMPONENT_NAME_CAPS_HERE>>>_ASIL_STATUS_FAIL;

   boolean_T f_check_ok = (boolean_T) 1;

<<<INSERT_DEFAULT_VALUE_CHECK>>>

   if(f_check_ok)
   {
      asil_check_status = <<<INSERT_COMPONENT_NAME_CAPS_HERE>>>_ASIL_STATUS_SUCCESS;
   }

   return asil_check_status;
}
"""

    endian_swap_include_skeleton = """\

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */
"""

    generic_info_structure_obj_initialization_skeleton = """\
{
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
<<<INSERT_H3_HDR_MEMBER_VARIABLES_LITTLE_ENDIAN_HERE>>>
   },
   /* Component Calibration */
<<<INSERT_CALIBRATIONS_LITTLE_ENDIAN_HERE>>>
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
<<<INSERT_CALIBRATIONS_BIG_ENDIAN_HERE>>>
   /* Ct_Header_T */
   {
<<<INSERT_H3_HDR_MEMBER_VARIABLES_BIG_ENDIAN_HERE>>>
   }
#endif /* CT_BIG_ENDIAN */
};
"""

    deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
    method_name_prefix = generic_cal_info.get_method_name_prefix(structure_name)

    # Operate on the file skeleton
    output = customer_specific_cal_file
    # At first replace potentially control flow dependent skeletons
    if generic_cal_info.export_asil_check_flag:
        output = output.replace("<<<INSERT_ASIL_CHECK_DEFINITION_SKELETON_HERE>>>", asil_check_definition_skeleton)
    else:
        output = output.replace("<<<INSERT_ASIL_CHECK_DEFINITION_SKELETON_HERE>>>", "")

    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_INITIALIZATION_HERE>>>", generic_info_structure_obj_initialization_skeleton)

    # Rest of string replacements
    output = output.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<INSERT_COMPONENT_NAME_CAPS_HERE>>>", generic_cal_info.component_name.upper())
    output = output.replace("<<<INSERT_BASIC_TYPE_INCLUDE_FILE_HERE>>>", generic_cal_info.type_include_file)
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_TYPE_HERE>>>", structure_name)
    output = output.replace("<<<INSERT_GENERIC_INFO_DEPLOYMENT_FILE_NAME_HERE>>>", deployment_file_name)
    output = output.replace("<<<INSERT_GENERIC_INFO_STRUCTURE_OBJ_NAME_HERE>>>", generic_cal_info.component_name.capitalize() +"_Cal")
    assert structure_name[-2:] == "_T"
    output = output.replace("<<<METHOD_NAME_PREFIX>>>", method_name_prefix)
    output = output.replace("<<<CUSTOMER>>>", customer)
    output = output.replace("<<<INSERT_YEAR_HERE>>>", str(date.today().year))
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)

    modified_endian_swap_include_string = endian_swap_include_skeleton
    if not f_array_includes:
        modified_endian_swap_include_string = ""

    output = output.replace("<<<INSERT_ENDIAN_SWAP_INCLUDE_HERE>>>", modified_endian_swap_include_string)

    return output
