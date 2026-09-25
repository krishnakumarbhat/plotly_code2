"""This file contains functions for creation of the core cal check files. Here the string for the boundary checks is replaced."""
import os
import logging
from typing import Dict

import python_src.file_skeletons.ct_update_calibration_skeleton as ct_ucs
import python_src.ct_shared_resources as ct_sr

#########################################################################
# Public functions
#########################################################################
def create_update_calibration_files(generic_cal_info: ct_sr.Generic_Calibration_Info, cal_core_path: str, file_name:str,
                                    update_calibration_methods:dict, calibrations_dict_of_dicts:dict) -> None:
    """
    Public function for generation of update_calibration files

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema
        cal_core_path (str) : path to which the files shall be generated

    Returns:
        Returns the file skeleton as formatted string with replacements
     """

    def get_cal_update_with_custom_values_content(_generic_cal_info: ct_sr.Generic_Calibration_Info,
                                                  dst_cals: Dict[str, ct_sr.Calibration],
                                                  src_cals: Dict[str, ct_sr.Calibration]) -> str:
        """
        Formats string for the explicit calibration update with custom values routine. Here a case distinction is needed for calibration
        tool internal types and also for calibrations displaying an array. Arrays are identified by square brackets in
        the default value member of a Calibration.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            _customer (str) : Customer for which the updating is generated. Needed for customer specific
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : Generic information of the calibration scheme

        Returns:
            Formatted string of the cal update with custom values routine
        """
        cal_src_arg_name = "cal_src"
        cal_dst_arg_name = "cal_dst"
        output = ""
        for key, cal in dst_cals.items():
            if key in ct_sr.cal_header_order:
                continue
            if key not in src_cals:
                continue

            # Check if calibration is array
            if cal.f_is_array:
                # Search for any decimal (floating or fixed point)
                if len(cal.dimensions) == 2:
                    # For two dimensional arrays loop through dimensions and set the indices at first.
                    for idx0 in range(cal.dimensions[0]):
                        for idx1 in range(cal.dimensions[1]):
                            output += (f"        {cal_dst_arg_name}->{key}[{idx0}][{idx1}] = " \
                            f"        {cal_src_arg_name}->{key}[{idx0}][{idx1}];\n")
                else:
                    for idx in range(cal.dimensions[0]):
                        # Format output string for arrays
                        str_to_add = f"        {cal_dst_arg_name}->{key}[{idx}] = " \
                                     f"{cal_src_arg_name}->{key}[{idx}];\n"
                        output += str_to_add
            else:
                str_to_add = f"        {cal_dst_arg_name}->{key} = {cal_src_arg_name}->{key};\n"
                output += str_to_add
        return output

    def get_calibration_type_include(_generic_cal_info:ct_sr.Generic_Calibration_Info, structure_name:str):
        return "#include \"" + _generic_cal_info.get_deployment_file_name(structure_name) + "_t.h\"\n"

    def get_includes_for_calibration_definition(_generic_cal_info:ct_sr.Generic_Calibration_Info, update_calibration_methods:dict):
        includs = set()
        for out_structure_name, in_structure_name in update_calibration_methods.values():
            includs.add(get_calibration_type_include(_generic_cal_info, in_structure_name))
            includs.add(get_calibration_type_include(_generic_cal_info, out_structure_name))
        result = ""
        includs = list(includs)
        includs.sort()
        for include in includs:
            result += include
        return result

    def get_includes_for_calibration_declaration(_generic_cal_info:ct_sr.Generic_Calibration_Info, update_calibration_methods:dict):
        includs = set()
        for out_structure_name, in_structure_name in update_calibration_methods.values():
            includs.add(get_calibration_type_include(_generic_cal_info, in_structure_name))
            includs.add(get_calibration_type_include(_generic_cal_info, out_structure_name))
            check_header_file_name =  _generic_cal_info.get_deployment_file_name(in_structure_name) + "_check.h"
            includs.add("#include \"" + check_header_file_name + "\"\n")
        result = ""
        includs = list(includs)
        includs.sort()
        for include in includs:
            result += include
        return result

    def get_update_calibration_method_declarations(_generic_cal_info:ct_sr.Generic_Calibration_Info,
                                                  update_calibration_methods:dict
                                                  ):
        output = ""
        for function_name in update_calibration_methods:
            out_structure_name, in_structure_name = update_calibration_methods[function_name]

            single_declaration = ct_ucs.get_update_calibration_method_declaration_skeleton()
            single_declaration = single_declaration.replace("<<<METHOD_NAME>>>", function_name)
            single_declaration = single_declaration.replace("<<<SRC_TYPE>>>", in_structure_name)
            single_declaration = single_declaration.replace("<<<DST_TYPE>>>", out_structure_name)
            output+=single_declaration
        return output

    def get_update_calibration_method_definition(_generic_cal_info:ct_sr.Generic_Calibration_Info,
                                                  update_calibration_methods:dict,
                                                  calibrations_dict_of_dicts:dict
                                                  ):
        output = ""
        for function_name in update_calibration_methods:
            out_structure_name, in_structure_name = update_calibration_methods[function_name]
            out_dict = calibrations_dict_of_dicts[out_structure_name]
            in_dict = calibrations_dict_of_dicts[in_structure_name]

            single_declaration = ct_ucs.get_update_calibration_method_definition_skeleton()
            single_declaration = single_declaration.replace("<<<METHOD_NAME>>>", function_name)
            single_declaration = single_declaration.replace("<<<SRC_TYPE>>>", in_structure_name)
            single_declaration = single_declaration.replace("<<<DST_TYPE>>>", out_structure_name)
            in_method_name_prefix = generic_cal_info.get_method_name_prefix(in_structure_name)
            single_declaration = single_declaration.replace("<<<SRC_METHOD_NAME_PREFIX>>>", in_method_name_prefix)
            single_declaration = single_declaration.replace("<<<INSERT_CAL_UPDATING_WITH_CUSTOM_VALUES_HERE>>>",
                get_cal_update_with_custom_values_content(_generic_cal_info, out_dict, in_dict))
            output+=single_declaration
        return output

    def write_h_file(_cal_core_path: str, _generic_cal_info: ct_sr.Generic_Calibration_Info, file_name:str,
                     update_calibration_methods:dict):
        """
        Private function for generation of cal check header file.

        Args:
            _cal_core_path (str) : path to which the files shall be generated
            _generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

        Returns:
            Returns the file skeleton as formatted string with replacements
        """
        output_path = os.path.join(_cal_core_path, file_name+".h")
        logging.info("Now generating %s for %s", file_name, _generic_cal_info.component_name)

        includes_for_calibration_definition = get_includes_for_calibration_definition(_generic_cal_info, update_calibration_methods)
        update_calibration_method_declarations = get_update_calibration_method_declarations(
            _generic_cal_info, update_calibration_methods)

        output = ct_ucs.get_update_calibration_h_file_skeleton(_generic_cal_info, file_name)
        output = output.replace("<<<INSERT_INCLUDES_FOR_CALIBRATION_DEFINITION_HERE>>>",
                                includes_for_calibration_definition)
        output = output.replace("<<<INSERT_UPDATE_CALIBRATION_METHOD_DECLARATION_HERE>>>",
                                update_calibration_method_declarations)
        with open(output_path, "w", encoding="utf-8") as f:
            f.write(output)


    def write_c_file(cal_core_path: str,
                     generic_cal_info: ct_sr.Generic_Calibration_Info,
                     file_name:str,
                     update_calibration_methods:dict,
                     calibrations_dict_of_dicts:dict):
        """
        Private function to export the update_calibration .c-file.

        Args:
            cal_dict (Dict[str, ct_sr.Calibration]) : calibrations sorted by their dimensions in a multidimensional array.
            _generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

        Returns:
            Returns output string containing all boundary checks for one dimensional arrays.
        """
        output_path = os.path.join(cal_core_path, file_name+".c")

        logging.info("Now generating %s for %s", file_name, generic_cal_info.component_name)

        insert_includes_for_calibration_definition_here = get_includes_for_calibration_declaration(
            generic_cal_info, update_calibration_methods)
        insert_update_calibration_method_definition_here = get_update_calibration_method_definition(
            generic_cal_info, update_calibration_methods, calibrations_dict_of_dicts
        )

        output = ct_ucs.get_update_calibration_c_file_skeleton(generic_cal_info, file_name)
        output = output.replace("<<<INSERT_INCLUDES_FOR_CALIBRATION_DEFINITION_HERE>>>",
                                insert_includes_for_calibration_definition_here)
        output = output.replace("<<<INSERT_UPDATE_CALIBRATION_METHOD_DEFINITION_HERE>>>",
                                insert_update_calibration_method_definition_here)
        with open(output_path, "w", encoding="utf-8") as f:
            f.write(output)

    #########################################################################
    # Start of public functions code
    #########################################################################
    write_h_file(cal_core_path, generic_cal_info, file_name, update_calibration_methods)
    write_c_file(cal_core_path, generic_cal_info, file_name, update_calibration_methods, calibrations_dict_of_dicts)
