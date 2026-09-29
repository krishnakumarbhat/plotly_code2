"""This file contains functions for creation of the shared calibration header file. Several string replacements
are applied here e.g. generation of macros and specific orders of calibration dictionaries."""
import os
import logging
from typing import Dict

from python_src.file_skeletons.ct_core_header_skeleton import get_core_header_file_skeleton
from python_src.file_skeletons.ct_core_type_header_skeleton import get_core_type_header_file_skeleton
import python_src.ct_shared_resources as ct_sr

#########################################################################
# Public functions
#########################################################################
def create_header(cal_dict: dict, cal_core_path: str,
                  generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str) -> None:
    """
    Main function that generates the customer core calibration .h-file.

    Args:
        cal_dict (dict) : A dictionary containing calibration names as keys and Calibration objects as values.
        cal_core_path (str) : Path pointing to the module Calibration Core folder where the main cal xml is located.
        generic_cal_info (ct_sr.Generic_Calibration_Info) : Generic information of xml cal file.

    Returns:
        Generates core calibration .h-file.
    """
    #########################################################################
    # Private functions
    #########################################################################
    def get_dimension_substring_of_cal(cal: ct_sr.Calibration, _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to add formatted string for array size dimension macros. This does not return a define
        but is rather used in declaration of arrays. Example: type array[array_size]; where this function returns
        array_size.

        Args:
            cal (Calibration) : Path pointing to the main .xml- file.
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme
        Returns:
            Formatted string which is listing all array size dimensions.
        """
        output = ""
        for idx in range(len(cal.dimensions)):
            output += f"[{cal.get_dimension_name_by_cal(_generic_cal_info.component_name, idx)}]"
        return output

    def get_array_sizes_of_cals(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to add formatted string for defines of array size macros.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            Formatted string which is listing all array size defines.
        """
        output = ""
        for cal in cals.values():
            if cal.f_is_array:
                for idx, dimension in enumerate(cal.dimensions):
                    output += "/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */\n"
                    output += f"#define {cal.get_dimension_name_by_cal( _generic_cal_info.component_name, idx)}" + \
                    f" ({dimension}u)\n"
        return output

    def get_array_dimensions_of_cals(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to add formatted string for defines of array size dimension macros.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            Formatted string which is listing all array dimension size defines.
        """
        output = ""
        for key in cals:
            if cals[key].f_is_array:
                output += "/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */\n"
                output += f"#define {_generic_cal_info.component_name.upper()}_{cals[key].name.upper()}_ARRAY_DIM_SIZE" + \
                f" ({len(cals[key].dimensions)}u)\n"
        return output

    def get_type_suffix(cal: ct_sr.Calibration, range_boundary: str) -> str:
        """
        Private function to return data type dependent suffix for minimum and maximum macros.

        Args:
            cal (ct_sr.Calibration) : An object of the class Calibration.
            range_boundary (str) : A boundary which could already contain a suffix.

        Returns:
            Formatted string containing a single suffix
        """
        output = ""
        if cal.data_type == "float32_T" and "f" not in range_boundary:
            if "." in range_boundary:
                output = "f"
            else:
                output = ".f"
        elif cal.data_type in ["boolean_T", "uint8_t", "uint16_t", "uint32_t"] and "u" not in range_boundary:
            output = "u"

        return output

    def get_maximum_value_of_cals(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to add formatted string for defines of permitted maximum values of calibrations.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            Formatted string which is listing all defines for permitted maximum values of calibrations.
        """
        output = ""
        for cal in cals.values():
            if cal.name not in ct_sr.cal_header_order:
                output += "/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */\n"
                output += f"#define {cal.get_maximum_value_macro_by_cal(_generic_cal_info.component_name)}" + \
                f" (({cal.data_type})({cal.range_max}{get_type_suffix(cal, cal.range_max)})) \n"
        return output

    def get_minimum_value_of_cals(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to add formatted string for defines of permitted minimum values of calibrations.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            Formatted string which is listing all defines for permitted minimum values of calibrations.
        """
        output = ""
        for cal in cals.values():
            if cal.name not in ct_sr.cal_header_order:
                output += "/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */\n"
                output += f"#define {cal.get_minimum_value_macro_by_cal(_generic_cal_info.component_name)}" + \
                f" (({cal.data_type})({cal.range_min}{get_type_suffix(cal, cal.range_min)})) \n"
        return output

    def get_cals_in_little_endian_order(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to add formatted string of struct definition of each calibration in the order of big to
        small types.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            Formatted string of calibration struct definition (calibration tool internal type excluded).
            """
        cals_ordered = ct_sr.sort_dict_from_big_to_small_types(cals)
        return get_cals_with_given_order(cals_ordered, _generic_cal_info)

    def get_cals_in_big_endian_order(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to add formatted string of struct definition of each calibration in the reversed order
        big to small but not only related to the types. The order needs to be mirrored in comparison to the big to
        small type order due to endianness which is assumed.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            Formatted string of calibration struct definition in mirrored order
            (calibration tool internal type excluded).
        """
        # revert the order of calibrations since they are ordered from big to small datatypes
        cals_ordered = ct_sr.sort_dict_from_big_to_small_types(cals)
        cals_ordered = {cal: cals_ordered[cal] for cal in reversed(list(cals_ordered))}
        return get_cals_with_given_order(cals_ordered, _generic_cal_info)

    def get_cals_with_given_order(cals_ordered: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to add formatted string of struct definition of each calibration in given order

        Args:
            cals_ordered (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            Formatted string of calibration struct definition in mirrored order
            (calibration tool internal type excluded).
        """
        output = ""
        for key, cal in cals_ordered.items():
            if key not in ct_sr.cal_header_order:
                dimensions = get_dimension_substring_of_cal(cal, _generic_cal_info)
                constant_keyword = "" #"const " if cal.f_is_constant else ""
                output += f"   {constant_keyword}{cal.data_type} {cal.name}{dimensions}; /**<{cal.description}*/\n"
        # Return output without trailing newline
        return output[:-1]

    def get_cals_default_init_in_little_endian_order(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        cals_ordered = ct_sr.sort_dict_from_big_to_small_types(cals)
        header_init = get_cals_default_header_init_in_given_order(cals_ordered, _generic_cal_info)
        return "{" + header_init + "," + get_cals_default_init_in_given_order(cals_ordered, _generic_cal_info)+"}"

    def get_cals_default_init_in_big_endian_order(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        cals_ordered = ct_sr.sort_dict_from_big_to_small_types(cals)
        cals_ordered = {cal: cals_ordered[cal] for cal in reversed(list(cals_ordered))}
        header_init = get_cals_default_header_init_in_given_order(cals_ordered, _generic_cal_info)
        return "{" + get_cals_default_init_in_given_order(cals_ordered, _generic_cal_info) + "," + header_init + "}"

    def get_cals_default_init_for_single_calibration(cal):
        output = ""
        type_cast = f"({cal.data_type})"
        default_value = cal.default_value
        # Check if calibration is array
        if cal.f_is_array:
            # Search for any decimal (floating or fixed point)
            matches = ct_sr.convert_array_string_to_list(default_value)
            if len(cal.dimensions) == 2:
                output+= "{"
                # For two dimensional arrays loop through dimensions and set the indices at first.
                idx = 0
                for _ in range(cal.dimensions[0]):
                    output+= "{"
                    for _ in range(cal.dimensions[1]):
                        output += f"{type_cast}{matches[idx]},"
                        idx += 1
                    output = output[:-1] + "},"
                output = output[:-1] + "}"
            else:
                output+= "{"
                for _, value in enumerate(matches):
                    output += f"{type_cast}{value},"
                output = output[:-1] + "}"
        else:
            output = type_cast+default_value
        return output

    def get_cals_default_init_in_given_order(cals_ordered: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        output = ""
        for key, cal in cals_ordered.items():
            if key not in ct_sr.cal_header_order:
                output += f"{get_cals_default_init_for_single_calibration(cal)},"
        # Return output without trailing newline
        return output[:-1] + ""
    def get_cals_default_header_init_in_given_order(cals_ordered: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        output = "{"
        for key, cal in cals_ordered.items():
            if key in ct_sr.cal_header_order:
                output += f"{get_cals_default_init_for_single_calibration(cal)},"
        # Return output without trailing newline
        return output[:-1] + "}"

    def write_t_h_file(cals: Dict[str, ct_sr.Calibration], path: str, _generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str) -> None:
        """
        Private function which calls string replacement sub functions and writes the formatted string output to a
        deployment path.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            path (str) : Deployment path of the .h-file.
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            None but generates the core header calibration file.
         """
        target_deployment_filename = generic_cal_info.get_deployment_file_name(structure_name) + "_t.h"
        logging.info("Now generating %s for %s", target_deployment_filename, generic_cal_info.component_name)

        output_path = os.path.join(path, target_deployment_filename)

        output = get_core_type_header_file_skeleton(_generic_cal_info, structure_name)
        output = output.replace("<<<INSERT_ARRAY_SIZES_OF_CALS_HERE>>>", get_array_sizes_of_cals(cals, _generic_cal_info))
        output = output.replace("<<<INSERT_ARRAY_DIMENSIONS_OF_CALS_HERE>>>", get_array_dimensions_of_cals(cals, _generic_cal_info))
        output = output.replace("<<<INSERT_CALS_IN_BIG_ENDIAN_ORDER_HERE>>>", get_cals_in_big_endian_order(cals, _generic_cal_info))
        output = output.replace("<<<INSERT_CALS_IN_LITTLE_ENDIAN_ORDER_HERE>>>", get_cals_in_little_endian_order(cals, _generic_cal_info))
        output = output.replace("<<<INSERT_CAL_SIZE_HERE>>>", ct_sr.get_cal_size(cals))
        output = output.replace("<<<INSERT_CALS_DEFAULT_INIT_IN_BIG_ENDIAN_ORDER_HERE>>>", get_cals_default_init_in_big_endian_order(cals, _generic_cal_info))
        output = output.replace("<<<INSERT_CALS_DEFAULT_INIT_IN_LITTLE_ENDIAN_ORDER_HERE>>>",
                                get_cals_default_init_in_little_endian_order(cals, _generic_cal_info))


        with open(output_path, "w", encoding="utf-8") as f:
            f.write(output)

    def write_h_file(cals: Dict[str, ct_sr.Calibration], path: str, _generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str) -> None:
        """
        Private function which calls string replacement sub functions and writes the formatted string output to a
        deployment path.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            path (str) : Deployment path of the .h-file.
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : object for basic string replacements given by generic scheme

        Returns:
            None but generates the core header calibration file.
         """
        deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
        target_deployment_filename = deployment_file_name + ".h"
        logging.info("Now generating %s for %s", target_deployment_filename, generic_cal_info.component_name)

        output_path = os.path.join(path, target_deployment_filename)

        output = get_core_header_file_skeleton(_generic_cal_info, structure_name)
        output = output.replace("<<<INSERT_MAXIMUM_VALUE_OF_CAL_DEFINES_HERE>>>", get_maximum_value_of_cals(cals, _generic_cal_info))
        output = output.replace("<<<INSERT_MINIMUM_VALUE_OF_CAL_DEFINES_HERE>>>", get_minimum_value_of_cals(cals, _generic_cal_info))

        with open(output_path, "w", encoding="utf-8") as f:
            f.write(output)

    #########################################################################
    # Start of public functions code
    #########################################################################

    write_t_h_file(cal_dict, cal_core_path, generic_cal_info, structure_name)
    write_h_file(cal_dict, cal_core_path, generic_cal_info, structure_name)

