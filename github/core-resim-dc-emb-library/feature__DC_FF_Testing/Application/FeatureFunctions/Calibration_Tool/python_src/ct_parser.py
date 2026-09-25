"""This file contains the parsing of xml data as well as adding of padding bytes and basic manipulation and validation
of parsed content."""
import os
import re
import xml.etree.ElementTree as Et
from typing import Tuple, Dict, List
import logging
from copy import deepcopy

import python_src.ct_shared_resources as ct_sr
from python_src.ct_export_customer_data_stream import get_hex_by_type
from python_src.ct_header_type import initialize_cal_header


#########################################################################
# Public functions
#########################################################################


def parse_calibration_xml(xml_path: str, calibration_path: str) \
        -> Tuple[Dict[str, ct_sr.Calibration], Dict[str, Dict[str, ct_sr.Calibration]], ct_sr.Generic_Calibration_Info]:
    """
    Main function for parsing the input core .xml-file as well as the customer specific calibration files, such that
    all Calibration information is gathered in a Calibration dictionary.

    Args:
        xml_path (str) : Path pointing to the main .xml- file.
        calibration_path (str) : Path pointing to the Calibration folder where the customer sub folders are located.

    Returns:
        A dictionary of cals containing the calibration names as keys and Calibration objects as values and also
        general information about the calibration like customers or data type related properties.
    """

    #########################################################################
    # Private functions
    #########################################################################
    def parse_xml(path: str):
        """Private function to parse a .xml-file.

        Args:
            path (str) : Path pointing to an input .xml-file.
        Returns:
            Returns the parsed .xml-file as Element type.
        """
        try:
            # Parse input xml
            tree = Et.parse(path)
            data_set = tree.getroot()
        except Et.ParseError as e:
            # Catch the wrong syntax of xml
            logging.critical("File %s is invalid", path)
            with open(path, "r", encoding="utf8") as xml_file:
                logging.critical("%s %s", e, xml_file.readlines()[e.position[0]-1])
        return data_set

    def are_xml_records_decimal(range_min: str, range_max: str, value: str) -> bool:
        """
        Check whether the ranges specified in the xml record as well as the given value contains a decimal number.
        Function checks initial conditions and indicates whether it is possible to create an object containing a calibration record.
        It must be called before subsequent validations and the start of data conversion and assignment, otherwise an error occurs.

        Args:
            range_min (str) : Minimum boundary of the xml record additionally to the type limits.
            range_max (str) : Maximum boundary of the xml record additionally to the type limits.
            value (str) : Value of the xml record.

        Returns:
            True when the xml input data contains decimal number.
        """
        regex_any_decimal_without_suffix = r"[-\d\.]+"

        if not all(re.findall(regex_any_decimal_without_suffix, list_element) for list_element in [range_max, range_min, value]):
            return False
        return True

    def are_xml_record_ranges_and_value_consistent(data_type: str, range_min: str, range_max: str, value: str) -> bool:
        """
        Check whether the ranges caused by the underlying data type, the extra ranges specified in the xml record as well as
        the given value are meaningful.

        Args:
            data_type (str) : Data type of the xml record to be checked.
            range_min (str) : Minimum boundary of the xml record additionally to the type limits.
            range_max (str) : Maximum boundary of the xml record additionally to the type limits.
            value (str) : Value of the xml record.

        Returns:
            True when the xml input data is meaningful.
        """

        value = value.replace("U", "").replace("F", "").replace("u", "").replace("f", "")
        range_max = range_max.replace("U", "").replace("F", "").replace("u", "").replace("f", "")
        range_min = range_min.replace("U", "").replace("F", "").replace("u", "").replace("f", "")

        #Operate on matches since value could also depict arrays
        matches = re.findall(r"([-\d\.]+)", value)
        if data_type == "float32_T":
            values_casted = [float(match) for match in matches]
            range_max_casted = float(range_max)
            range_min_casted = float(range_min)
        elif data_type in ct_sr.types_order_big_to_small:
            values_casted = [int(match) for match in matches]
            range_max_casted = int(range_max)
            range_min_casted = int(range_min)

        # Check whether the xml record range is contained in the supported data type range
        f_are_type_ranges_in_xml_ranges: bool = ct_sr.type_ranges[data_type]["min"] <= range_min_casted <= ct_sr.type_ranges[data_type]["max"] and \
            ct_sr.type_ranges[data_type]["min"] <= range_max_casted <= ct_sr.type_ranges[data_type]["max"]

        #Check whether the given value is contained in the xml record range
        f_value_in_ranges: bool = all(range_min_casted <= value_casted <= range_max_casted
            for value_casted in values_casted)
        return f_are_type_ranges_in_xml_ranges and f_value_in_ranges


    def get_check_sum(cals: Dict[str, ct_sr.Calibration], customer: str) -> str:
        """
        Private function to calculate the checksum such that the calibration tool type Cal_Chk_Sum can be
        overwritten.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            customer (str) : Customer for which the checksum needs to be evaluated.

        Returns:
            Returns the checksum as a string.
         """
        checksum = 0
        split_into_single_bytes = 2
        hex_representation = 16
        for cal in cals:
            # filter the h3_hdr_t member, since those are not content of the checksum.
            if cal not in ct_sr.cal_header_order:
                customer_specific_cal = cals[cal].get_value_of_cal_for_customer(customer)
                customer_specific_cal = customer_specific_cal.replace("u", "").replace("f", "")
                # case distinction for arrays, since content of those needs to add up
                if cals[cal].f_is_array:
                    matches = ct_sr.convert_array_string_to_list(customer_specific_cal)
                    # get the byte representation of each value of the array and add it up for each byte
                    for match in matches:
                        value_hex = get_hex_by_type(match, cals[cal].data_type)
                        byte_wise_string = [value_hex[i:i + split_into_single_bytes] for i in
                                            range(0, len(value_hex), split_into_single_bytes)]
                        checksum += sum((int(b, hex_representation) for b in byte_wise_string))
                else:
                    # get the byte representation of the value and add it up for each byte
                    value_hex = get_hex_by_type(customer_specific_cal, cals[cal].data_type)
                    byte_wise_string = [value_hex[i:i + split_into_single_bytes] for i in
                                        range(0, len(value_hex), split_into_single_bytes)]
                    checksum += sum((int(b, hex_representation) for b in byte_wise_string))
        return str(checksum)

    def parse_generic_cal(data_set: Et) -> ct_sr.Generic_Calibration_Info:
        """
        Constructs the generic information.

        Args:
            data_set (Et) : Parsed calibration file

        Returns:
            Returns generic information about the calibration scheme like customer list or data related properties
        """
        _generic_cal_info: ct_sr.Generic_Calibration_Info

        # Check whether the xml contain calibration generic info header.
        if not data_set.findall(".//CALIBRATION_GENERIC_INFO"):
            logging.error("CALIBRATION_HEADER section is missing. Have a look at README.md")

        for record in data_set.findall("CALIBRATION_GENERIC_INFO"):
            type_include_file = record.find("BASIC_TYPE_INCLUDE_FILE")
            asil_check_flag = record.find("EXPORT_ASIL_CHECK")
            component_name = record.find("COMPONENT_NAME")
            customers = record.find("CUSTOMERS")
            if None in [type_include_file, asil_check_flag, component_name, customers]:
                logging.error("Generic info of calibration is not complete and thus will not be added here. Have a look at README.md")
            else:
                customers = customers.text.replace("[", "").replace("]", "").split(" ")
                asil_check_flag_bool: bool = asil_check_flag.text.lower() in ["true", "1", "yes", "t", "y"]
                component_type_prefix = component_name.text.capitalize()
                _generic_cal_info = ct_sr.Generic_Calibration_Info(type_include_file = type_include_file.text,
                public_structure_name = component_type_prefix + "_Public_Calibration_T",
                customer_structure_name = component_type_prefix + "_Customer_Calibration_T",
                core_structure_name = component_type_prefix + "_Core_Calibration_T",
                component_name = component_name.text, export_asil_check_flag = asil_check_flag_bool,
                customers = customers)
        return _generic_cal_info

    def parse_cal_header(data_set: Et, cal_header: Dict[str, ct_sr.Calibration]) -> Dict[str, ct_sr.Calibration]:
        """
        Constructs the calibration dictionary.
        Args:
            data_set (Et) : Parsed calibration file
            cal_header (dict) : Information about calibration tool internals.
        Returns:
            Returns the updated calibration header.
        """

        # Check whether that version is set beforehand
        if not any(True for data in data_set.findall(".//CALIBRATION_HEADER/record/NAME") if "version" == data.text):
            logging.error("record version is missing in the CALIBRATION_HEADER section. Have a look at README.md")

        for record in data_set.findall("./CALIBRATION_HEADER/record"):
            name = record.find("NAME")
            value = record.find("VALUE")
            # Check whether the minimum required set of information for the current calibration is available.
            if None in [name, value]:
                if name is None:
                    logging.error("Name of calibration record is not complete and thus calibration will be dismissed.")
                else:
                    logging.error("Calibration record of %s is not complete and thus will not be added here.",
                                  name.text)
            else:
                cal_header[name.text.rstrip()].default_value = value.text.replace("U", "u").replace("F", "f")
        return cal_header

    def parse_cal_single_record(record):
        name = record.find("NAME")
        description = record.find("DESCRIPTION")
        unit = "" if None is record.find("UNIT") else record.find("UNIT").text
        data_type = record.find("DATA_TYPE")
        data_resolution = record.find("DATA_RESOLUTION")
        range_min = record.find("RANGE_MIN")
        range_max = record.find("RANGE_MAX")
        default_value = record.find("DEFAULT_VALUE")
        constant = record.find("CONSTANT")

        if constant is not None and constant.text.lower() not in ["true", "false"]:
            logging.error("CONSTANT attribute must be boolean but is %s", constant.text)
        # Check whether the minimum required set of information for the current calibration is available.
        elif None in [name, data_type, data_resolution, range_min, range_max, default_value]:
            if name is None:
                logging.error("Name of calibration record is not complete and thus calibration will be dismissed.")
            else:
                logging.error("Calibration record of %s is not complete and thus will not be added here. Take a look at README.md",
                              name.text)
        elif data_type.text not in ct_sr.types_order_big_to_small:
            logging.error("Datatype %s of calibration %s does not match the supported types %s calibration will be dismissed", \
                data_type.text , name.text, str(ct_sr.types_order_big_to_small))
        elif not are_xml_records_decimal(range_min.text, range_max.text, default_value.text):
            logging.error("Range values [%s,%s] or default value [%s] are empty or do not contain a decimal value in %s calibration", \
                range_min.text, range_max.text, default_value.text, name.text)
        elif not are_xml_record_ranges_and_value_consistent(data_type.text, range_min.text, range_max.text, default_value.text):
            output_str:str = f"Calibration {name.text} is dismissed due to inconsistent ranges caused by either data type "
            output_str += f"{data_type.text} given range [{range_min.text},{range_max.text}] or default value {default_value.text}"
            logging.error(output_str)
        else:
            # Check if calibration value is an array
            default_value = default_value.text.replace("U", "u").replace("F", "f")
            range_max = range_max.text.replace("U", "u").replace("F", "f")
            range_min = range_min.text.replace("U", "u").replace("F", "f")
            f_is_array = False
            if "[" in default_value:
                f_is_array = True
                if "]" not in default_value:
                    logging.error("%s not contains a closing bracket in default value: %s", name.text, default_value)
                elif "," in default_value:
                    logging.error("%s contains a comma in default value: %s", name.text, default_value)
            else:
                # Check if the default value has multiple values, but they are not in brackets
                default_value_list = re.findall(r"[-\d\w\.]+", default_value)
                if "]" not in default_value and len(default_value_list) > 1:
                    logging.error("Calbration %s does not have properly set brackets denoting matrix. Take a look at README.md",\
                        name.text)
            is_constant = False if constant is None else {"false":False, "true":True}[constant.text.lower()]

            cal = ct_sr.Calibration(name = name.text, data_type = data_type.text, data_resolution = data_resolution.text,
                                    range_min = range_min, range_max = range_max, default_value = default_value,
                                    f_is_array = f_is_array, description = description.text, unit = unit, f_is_constant = is_constant)
            return cal
        return None
    def parse_single_customer_record_and_update_cal(cals, customer, record):
        cal_name = record.find("NAME").text.rstrip()
        cal_value = record.find("VALUE").text.replace("U", "u").replace("F", "f")
        if cal_name in cals:
            if "[" in cal_value and not cals[cal_name].f_is_array:
                # Array was specified while cal is not an array
                logging.warning("%s of customer specific cal xml for %s is not supposed to be an array and "
                                "will be ignored!", cal_name, customer)
            elif cals[cal_name].f_is_array and not all(list_element in cal_value for list_element in ["[","]"]):
                # Non-array was specified while cal is an array
                logging.warning("%s of customer specific cal xml for %s is expected to be an array "
                                "and will be ignored!", cal_name, customer)
            elif cals[cal_name].f_is_array and "," in cal_value:
                logging.warning("%s of customer specific cal xml for %s contains a comma in value: %s",\
                    cal_name, customer, cal_value)
            elif cals[cal_name].f_is_array and not cals[cal_name].dimensions == cals[cal_name].extract_dimension(cal_value):
                # Dimension mismatch is detected
                logging.warning("%s of customer specific cal xml for %s is expected to have dimension %s, but %s was given!",\
                    cal_name, customer, str(cals[cal_name].dimensions), str(cals[cal_name].extract_dimension(cal_value)))
            elif customer in cals[cal_name].customer_specific_values:
                warning_string = f"{cal_name} was already specified to {cals[cal_name].get_value_of_cal_for_customer(customer)} "
                warning_string += f"at least once before in customer specific cal xml of {customer}. "
                warning_string += f"Setting to {cal_value} is suppressed!"
                logging.warning(warning_string)
            elif not are_xml_records_decimal(cals[cal_name].range_min,cals[cal_name].range_max, cal_value):
                logging.error("Range values [%s,%s] or value [%s] are empty or do not contain a decimal value in %s calibration of %s", \
                    cals[cal_name].range_min, cals[cal_name].range_max, cal_value, cal_name, customer)
            elif not are_xml_record_ranges_and_value_consistent(cals[cal_name].data_type, cals[cal_name].range_min,\
                cals[cal_name].range_max, cal_value):
                output_str:str = f"Calibration {cal_name} of {customer} is dismissed due to inconsistent ranges caused by either data type "
                output_str += f"{cals[cal_name].data_type} given range [{cals[cal_name].range_min},{cals[cal_name].range_max}]"
                output_str += f" or default value {cal_value}"
                logging.error(output_str)
            else:
                cals[cal_name].customer_specific_values.update({customer: cal_value})
        else:
            logging.warning("%s of customer specific cal xml for %s is not part of the cal "
                            "set anymore and will be ignored!", cal_name, customer)
    def parse_cal_component(data_set: Et) -> Dict[str, ct_sr.Calibration]:
        """Constructs the calibration dictionary.
         Args:
             data_set (Et) : Parsed calibration file
         Returns:
             Returns the Calibration dictionary of the component specific calibrations.
         """
        cals = {}

        # Check whether the xml contain calibration component header.
        if not data_set.findall(".//CALIBRATION_COMPONENT"):
            logging.error("CALIBRATION_COMPONENT section is missing. Have a look at README.md")

        for record in data_set.findall("./CALIBRATION_COMPONENT/record"):
            cal = parse_cal_single_record(record)
            if cal is not None:
                cals.update({cal.name: cal})

        return cals


    def parse_customer_specific_configs_xml(core_cal_dict: Dict[str, ct_sr.Calibration], cal_path: str, customers: List[str]) -> dict:
        """
        Parses the customer specific .xml-files and updates customer specific calibrations.

        Args:
            core_cal_dict (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            cal_path (str) : Path pointing to the path which is containing customers as sub folders.
            customers (List[str]) : Customer list for which the customer dependent values need to be gathered.

        Returns:
            Returns the Calibration dictionary with updated customer specific values in case there are any.
        """
        # Loop over all customers given from the main xml file and parse all customer specific configurations
        # (if available)
        customers_specific_cals = {}
        for customer in customers:
            customer_xml_path = os.path.join(cal_path, customer, "Customer_Specific_Cal.xml")
            data_set = parse_xml(customer_xml_path)
            for record in data_set.findall("./record"):
                parse_single_customer_record_and_update_cal(core_cal_dict, customer, record)
            customer_cals = {}
            for record in data_set.findall("./CUSTOMER_SPECIFIC_CALIBRATION_COMPONENT/record"):
                cal = parse_cal_single_record(record)
                if cal is not None:
                    if cal.name in core_cal_dict:
                        logging.error("Custom specific calibration can't have the same name as core calibration %s", cal.name)
                    else:
                        customer_cals.update({cal.name: cal})
            customers_specific_cals[customer] = customer_cals
        return core_cal_dict, customers_specific_cals

    def add_padding_bytes_to_cals(cals: Dict[str, ct_sr.Calibration]) -> dict:
        """
        Loops through the calibration dictionary and check whether padding bytes need to be added. In case 4 byte
        packing is not fulfilled with the given calibrations, padding bytes shall be added.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : Calibration dictionary which is sorted from big to small types.

        Returns:
            Calibration dictionary with padding bytes added.
        """
        remainder = 0
        byte_packing = 4
        # Always calculate the remainder. Since our cals are sorted from big to little we are expecting
        # to only need padding bytes to be appended.
        for cal in cals:
            if cals[cal].name not in ct_sr.cal_header_order:
                # Also take care of arrays and check how many entries are available.
                num_val_occurrences = len(ct_sr.convert_array_string_to_list(cals[cal].default_value))
                remainder += num_val_occurrences * ct_sr.type_to_byte_dict[cals[cal].data_type]
                remainder = remainder % byte_packing
        if remainder > 0:
            # Add padding bytes
            for idx in range(byte_packing - remainder):
                name = "k_unused_padding_byte_" + str(idx)
                cals.update(
                    {name: ct_sr.Calibration(name = name, data_type = "uint8_t", data_resolution = "1",
                                             range_min = "0", range_max = "255", default_value = "0",
                                             f_is_array = False, description = "Padded byte for byte packing of 4",
                                             unit = "none", f_is_constant=True)})
        return cals

    def overwrite_cal_tool_specific_variables(cals: Dict[str, ct_sr.Calibration], customers: List[str]) -> dict:
        """
        Update calibration tool specific values. Some of them are customer dependent.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : Calibration dictionary which is sorted from big to small types.
            customers (List[str]) : Customer List for which the customer dependent calibration tool type needs to be
            updated.
        Returns:
            Calibration dictionary with updated calibration tool internal type.
         """
        cals[ct_sr.cal_header_order[0]].default_value = ct_sr.get_cal_size(cals)
        uint16_num_values = 65536
        for customer in customers:
            checksum = get_check_sum(cals, customer)
            # Check whether checksum is exceeding the maximum of the given datatype.
            # This is introduced due to limitations of the current calibration tool. It is allowing overflows in
            # uint16_t and is adding values from 0 again.
            checksum_int: int = int(checksum) % uint16_num_values
            cals[ct_sr.cal_header_order[3]].customer_specific_values.update({customer: checksum_int})
        return cals

    #########################################################################
    # Start of public functions code
    #########################################################################
    # Parse all calibrations and their corresponding customer specific values
    cal_header_dict = initialize_cal_header()
    data_set = parse_xml(xml_path)
    generic_cal_info = parse_generic_cal(data_set)
    cal_header_dict = parse_cal_header(data_set, cal_header_dict)
    core_cal_dict = parse_cal_component(data_set)
    core_cal_dict, customers_cal_dicts = parse_customer_specific_configs_xml(core_cal_dict, calibration_path, generic_cal_info.customers)

    for cal_dict in [core_cal_dict, *customers_cal_dicts.values()]:
        # Sort content of cals dictionary with respect to the type (from big to little)
        tmp = ct_sr.sort_dict_from_big_to_small_types(cal_dict)

        # Combine dictionaries such that calibration header is in front of the rest of the component calibrations
        tmp = {**deepcopy(cal_header_dict), **tmp}

        # Add padding bytes to cals for a packing of 4 bytes
        tmp = add_padding_bytes_to_cals(tmp)

        # Modify parsed results due to internal assumptions of the cal tool
        tmp = overwrite_cal_tool_specific_variables(tmp, generic_cal_info.customers)

        cal_dict.clear()
        cal_dict.update(tmp)

    return core_cal_dict, customers_cal_dicts, generic_cal_info
