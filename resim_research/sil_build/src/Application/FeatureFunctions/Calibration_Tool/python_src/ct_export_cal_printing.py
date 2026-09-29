"""This file contains functions for creation of the printing .c-file."""
import os
import logging
from typing import Dict

from python_src.file_skeletons.ct_core_cal_print_skeleton import get_core_cal_printing_file_skeleton
import python_src.ct_shared_resources as ct_sr

# Type print formatter for printf
type_print_formatter_dict = {"float32_T": "%f", "int32_t": "%d", "uint32_t": "%d", "int16_t": "%d", "uint16_t": "%d",
                             "int8_t": "%d", "uint8_t": "%d", "boolean_T": "%d"}


#########################################################################
# Public functions
#########################################################################
def create_cal_printing(cal_dict: dict, cal_core_path: str, generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str) -> None:
    """
    Main function that auto generates the core file responsible for printing of calibration values.

    Args:
        cal_dict (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
        cal_core_path (str) : Path pointing to the core subfolder in the Calibration value of a module
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        None but generates a .c-file.
    """

    #########################################################################
    # Private functions
    #########################################################################
    def get_printing_commands(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function which returns a formatted string of module calibrations (calibration tool internal types excluded)

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            _generic_cal_info (Generic_Calibration_Info) : General information of calibration scheme.
        Returns:
            Returns formatted string without trailing newline.
        """
        output = ""
        coverity_exception = "   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as " \
                    "function is only used for debug purposes] */\n"
        structure_object_name = "p_cals"
        for cal in cals:
            if cal not in ct_sr.cal_header_order:
                if cals[cal].f_is_array:
                    # Check for two dimensional arrays
                    if len(cals[cal].dimensions) == 2:
                        # Access array based on all dimensions
                        for idx0 in range(cals[cal].dimensions[0]):
                            for idx1 in range(cals[cal].dimensions[1]):
                                output += coverity_exception
                                output += f"   (void)fprintf(c_file_ptr,\"{structure_object_name}." \
                                    f"{cals[cal].name}._{idx0}_{idx1}_,{type_print_formatter_dict[cals[cal].data_type]}\\n\"," \
                                    f" p_cals->{cals[cal].name}[{idx0}][{idx1}]);\n"
                    else:
                        # In case of arrays count the amount of different digits, such that all of them can be printed.
                        num_digits = len(ct_sr.convert_array_string_to_list(cals[cal].default_value))
                        for idx in range(num_digits):
                            output += coverity_exception
                            output += f"   (void)fprintf(c_file_ptr,\"{structure_object_name}." \
                                    f"{cals[cal].name}._{idx}_,{type_print_formatter_dict[cals[cal].data_type]}\\n\"," \
                                    f" p_cals->{cals[cal].name}[{idx}]);\n"
                else:
                    output += coverity_exception
                    output += f"   (void)fprintf(c_file_ptr,\"{structure_object_name}." \
                              f"{cals[cal].name},{type_print_formatter_dict[cals[cal].data_type]}\\n\"," \
                              f" p_cals->{cals[cal].name});\n"
        # Return output without newline
        return output[:-1]

    def write_print_c_file(cals: Dict[str, ct_sr.Calibration], path: str, _generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str) -> None:
        """
        Private function which calls string replacement sub functions and writes the formatted string output to a
        deployment path.
        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            path (str) : Path of the file to be deployed.
            _generic_cal_info (Generic_Calibration_Info) : General information of calibration scheme.
        Returns:
            None but generates a .c-file.
        """
        target_deployment_filename = generic_cal_info.get_deployment_file_name(structure_name) + "_print_functions.c"
        logging.info("Now generating %s for %s", target_deployment_filename, _generic_cal_info.component_name)

        output_path = os.path.join(path, target_deployment_filename)
        output = get_core_cal_printing_file_skeleton(_generic_cal_info, ct_sr.are_arrays_used_in_cals(cals), structure_name)
        output = output.replace("<<<INSERT_PRINTING_COMMANDS_HERE>>>",
                                get_printing_commands(cals, _generic_cal_info))

        with open(output_path, "w", encoding="utf-8") as f:
            f.write(output)

    #########################################################################
    # Start of public functions code
    #########################################################################
    write_print_c_file(cal_dict, cal_core_path, generic_cal_info, structure_name)
