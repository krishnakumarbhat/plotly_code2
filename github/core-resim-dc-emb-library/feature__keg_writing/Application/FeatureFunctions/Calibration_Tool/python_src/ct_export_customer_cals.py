"""This file contains functions for creation of customer specific calibration files. Several string replacements
are applied here e.g. replacement of calibration update routine."""
import os
import re
import logging
from typing import Dict

import python_src.ct_shared_resources as ct_sr
from python_src.file_skeletons.ct_customer_specific_cal_skeleton import get_customer_specific_cal_file

#########################################################################
# Public functions
#########################################################################
def create_customer_specific_c_file(cal_dict: dict, calibration_path: str,
                                    generic_cal_info: ct_sr.Generic_Calibration_Info, customer, structure_name:str) -> None:
    """
    Main function that generates the customer specific calibration .c-files.

    Args:
        cal_dict (dict) : A dictionary containing calibration names as keys and Calibration objects as values.
        calibration_path (str) : Path pointing to the Calibration folder where the customer sub folders are located.
        generic_cal_info (ct_sr.Generic_Calibration_Info) : Generic information of the calibration scheme

    Returns:
        Generates customer specific .c-files for each given customer.
     """

    #########################################################################
    # Private functions
    #########################################################################
    def convert_si_to_eng_unit(unit: str, value: str) -> str:
        """
        Private function that generates the customer specific calibration .c-files.

        Args:
            unit (str): unit which can be converted if SI and supported
            value (str): value to be supported
        Returns:
            String which is giving the conversion as a string.
        """
        si_to_engineering_conversion = {
            "rad": {"factor": 57.2957795131, "eng_unit": "deg"},
            "rad/s": {"factor": 57.2957795131, "eng_unit": "deg/s"},
            "m/s": {"factor": 3.6, "eng_unit": "km/h"}}
        # Remove type information
        value = value.replace("f","").replace("u","")
        if unit in si_to_engineering_conversion:
            # Convert unit from SI to engineering unit
            value_casted = float(value)
            eng_unit = si_to_engineering_conversion[unit]["eng_unit"]
            converted_cal = round(value_casted * si_to_engineering_conversion[unit]["factor"], 2)
            return_str = f" /**< {value} {unit} | {str(converted_cal)} {eng_unit} */"
        elif not unit in ["None", "none", "-", "", None]:
            # Add unit information when a non default value is given.
            return_str = f" /**< {value} {unit} */"
        else:
            return_str = ""

        return return_str

    def get_cal_default_check(cals: Dict[str, ct_sr.Calibration], _customer: str) -> str:
        """
        Formats string for the so called Asil check routine which is used when flashing a calibration structure to an
        embedded hardware.
        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            _customer (str) : Customer for which the updating is generated. Needed for customer specific
        Returns:
            Formatted string of the Asil check routine
        """
        output = "   /* coverity[misra_c_2012_rule_14_3_violation][Keeping always true condition to avoid " + \
                 "unnecessary complicated auto-generation] */\n"
        for key, cal in cals.items():
            customer_specific_cal = cal.get_value_of_cal_for_customer(_customer)
            if key in ct_sr.cal_header_order:
                # assumption: generic information of cal tool does not contain any arrays
                output += f"   f_check_ok = (boolean_T) (f_check_ok && (p_cals->Header.{key} == " \
                          f"(({cal.data_type}) {customer_specific_cal})));\n"
            else:
                # case distinction for arrays and simple types.
                if cal.f_is_array:
                    matches = ct_sr.convert_array_string_to_list(customer_specific_cal)
                    if len(cal.dimensions) == 2:
                        temp_list = []
                        # For two dimensional arrays loop through dimensions and set the indices at first.
                        for idx0 in range(cal.dimensions[0]):
                            for idx1 in range(cal.dimensions[1]):
                                temp_list.append( f"   f_check_ok = (boolean_T) (f_check_ok && (p_cals->{key}[{idx0}][{idx1}] ==" \
                                        f" (({cal.data_type}) <<<REPLACE_WITH_MATCH_HERE>>>)));\n")
                        # The second loop is replacing the value information of the calibration value
                        for match, temp_str in zip(matches, temp_list):
                            output += temp_str.replace("<<<REPLACE_WITH_MATCH_HERE>>>",str(match))
                    else:
                        # For one dimensional arrays use single index.
                        for idx, match in enumerate(matches):
                            output += f"   f_check_ok = (boolean_T) (f_check_ok && (p_cals->{key}[{idx}] ==" \
                                    f" (({cal.data_type}) {match})));\n"
                else:
                    output += f"   f_check_ok = (boolean_T) (f_check_ok && (p_cals->{key} ==" \
                              f" (({cal.data_type}) {customer_specific_cal})));\n"
        return output

    def get_array_reversing(cals: Dict[str, ct_sr.Calibration], _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Formats string for the reversing of arrays on embedded hardware. This is used when the endianness of the
        embedded hardware differs from the development endianness, such that the order of the member of an array is
        reversed according to the endianness of the target hardware.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : Generic information of the calibration scheme
        Returns:
            Formatted string of the array reversing routine without trailing newline.
        """
        cal_dst_arg_name = "cal_dst"
        output = ""
        for cal in cals:
            # Only add calibrations to this formatted string which are arrays.
            if cals[cal].f_is_array:
                output += f"   Ct_Reverse_Array((uint8_t*)&{cal_dst_arg_name}->{cals[cal].name}[0], " \
                          f"sizeof({cal_dst_arg_name}->{cals[cal].name}), " \
                          f"{ct_sr.endian_swap_satisfier[ct_sr.type_to_byte_dict[cals[cal].data_type]]});\n"
        # Return output without newline
        return output[:-1]

    def get_header_member_variables(cals: Dict[str, ct_sr.Calibration], _customer: str) -> str:
        """
        Formats string for the calibration tool internal datatype. This string is used for direct initialization of
        the targets calibration structure. Here the types of the type are ordered from big to small types (according to
        the target hardware)
        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            _customer (str) : Name of the customer for which the cals shall be generated.
        Returns:
            Formatted string of calibration tool internal type. Here with trailing newline, since the order may be
            reversed by consumer functions
        """
        output = ""
        # gather calibration header datatype member
        cal_header = ct_sr.get_cal_header_in_specific_order(cals)
        cal_header = ct_sr.change_order_of_dict_by_name(cal_header, ct_sr.cal_header_order)

        for key, header_member in cal_header.items():
            # Since cal check sum is customer specific, we need to get the customer specific value here.
            customer_specific_val = header_member.get_value_of_cal_for_customer(_customer)
            output += f"   /**<{key}*/ ({header_member.data_type}){customer_specific_val},\n"
        return output

    def get_type_casting_and_units_adder(cal):
        def add_type_casting_and_units_to_calibration_value(x):
            return "(" + cal.data_type + ")" + x + convert_si_to_eng_unit(cal.unit, x)
        return add_type_casting_and_units_to_calibration_value
    def get_calibrations_little_endian(cals: Dict[str, ct_sr.Calibration], _customer: str) -> str:
        """
        Formats string for direct initialization of the targets calibration structure. Here only the calibrations
        of the module are considered (not the calibration tool internal type) and a case distinction for arrays is
        available.
        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            _customer (str) : Name of the customer for which the cals shall be generated.
        Returns:
            Formatted string of direct initialization of module internal calibrations.
        """
        output = ""
        cals_ordered = ct_sr.sort_dict_from_big_to_small_types(cals)
        for key, cal in cals_ordered.items():
            if key not in ct_sr.cal_header_order:
                customer_specific_value = cal.get_value_of_cal_for_customer(_customer)
                if cal.f_is_array:
                    # add datatype as prefix for any digit (negative or positive and fixed point or floating
                    # point format) and add some string replacements such that casting of the
                    # underlying datatype is available in the value assignment
                    customer_specific_value = customer_specific_value.replace("[", "{").replace("]", "}").replace(" "\
                        ,",").replace("}{","},{")
                    cut_customer_specific_value = re.split(r"([-\d\.]+f?u?)", customer_specific_value)
                    values = cut_customer_specific_value[1::2]
                    add_type_casting_and_units_to_calibration_value = get_type_casting_and_units_adder(cal)
                    decorated_values = list(map(add_type_casting_and_units_to_calibration_value, values))
                    cut_customer_specific_value[1::2] = decorated_values
                    customer_specific_value = "".join(cut_customer_specific_value)
                    output += f"   /**<{key}*/ {customer_specific_value},\n"
                else:
                    output += f"   /**<{key}*/ ({cal.data_type}) {customer_specific_value}{convert_si_to_eng_unit(cal.unit, customer_specific_value)},\n"
        # Return output without trailing comma and newline
        return output[:-2]

    def create_customer_specific_c_file_implementation(cals: Dict[str, ct_sr.Calibration], path: str, _customer: str,
                                      _generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str) -> None:
        """
        Private function which calls string replacement sub functions and writes the formatted string output to a
        deployment path.
        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            path (str) : Deployment path of the .c-file.
            _customer (str) : Name of the customer for which the cals shall be generated.
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : Generic information of the calibration scheme
        Returns:
            None but generates a customer specific .c-file
        """
        deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
        target_deployment_filename = deployment_file_name + ".c"
        logging.info("Now generating %s for %s_%s", target_deployment_filename, _generic_cal_info.component_name, _customer)
        file_path = os.path.join(path, target_deployment_filename)

        header_member_little_endian: str = get_header_member_variables(cals, _customer)
        header_member_list_reversed = header_member_little_endian.split("\n")
        header_member_list_reversed.reverse()
        header_member_big_endian: str = "\n".join(header_member_list_reversed)
        # Remove trailing comma of h3 member lists
        header_member_little_endian = header_member_little_endian[:-2]
        header_member_big_endian = header_member_big_endian[1:-1]

        calibration_string_little_endian = get_calibrations_little_endian(cals, _customer)
        calibration_string_big_endian = ""
        if calibration_string_little_endian != "":
            calibration_list_initialized_reversed = (calibration_string_little_endian + ",").split("\n")
            calibration_list_initialized_reversed.reverse()
            calibration_string_big_endian: str = "\n".join(calibration_list_initialized_reversed)

        output = get_customer_specific_cal_file(_customer, _generic_cal_info, ct_sr.are_arrays_used_in_cals(cals), structure_name)

        output = output.replace("<<<INSERT_DEFAULT_VALUE_CHECK>>>", get_cal_default_check(cals, _customer))
        output = output.replace("<<<INSERT_REVERSING_OF_CAL_ARRAYS_HERE>>>",
                                get_array_reversing(cals, _generic_cal_info))
        output = output.replace("<<<INSERT_H3_HDR_MEMBER_VARIABLES_BIG_ENDIAN_HERE>>>", header_member_big_endian)
        output = output.replace("<<<INSERT_H3_HDR_MEMBER_VARIABLES_LITTLE_ENDIAN_HERE>>>", header_member_little_endian)
        output = output.replace("<<<INSERT_CALIBRATIONS_BIG_ENDIAN_HERE>>>",
                                calibration_string_big_endian)

        output = output.replace("<<<INSERT_CALIBRATIONS_LITTLE_ENDIAN_HERE>>>", calibration_string_little_endian)

        with open(file_path, "w", encoding="utf-8") as f:
            f.write(output)

    #########################################################################
    # Start of public functions code
    #########################################################################
    create_customer_specific_c_file_implementation(cal_dict, calibration_path, customer, generic_cal_info, structure_name)
