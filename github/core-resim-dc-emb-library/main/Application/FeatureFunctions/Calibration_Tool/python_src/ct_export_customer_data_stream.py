"""This file contains the functions for creation of customer specific data stream xml files.

Those are showing the byte stream of the calibration struct in big and little endian.
"""
import os
import struct
from typing import Dict, Tuple
import logging

import python_src.ct_shared_resources as ct_sr
from python_src.file_skeletons.ct_customer_data_stream_skeleton import get_customer_data_stream_xml_file


datatype_to_stream_length_map = {"char": 2, "short": 4, "long": 8}
""" Dict[str, int]: This is used to define the type length on the data stream.

Thus the following is expected:
char:  0x00 - 0xFF instead of 0x0
long: 0x00000000 - 0xFFFFFFFF instead of 0x1 for small values in a big data type. etc.
"""


#########################################################################
# Public functions
#########################################################################
def get_hex_by_type(value: str, datatype: str, endianness: str = ">") -> str:
    """
    Public function that returns the hexadecimal byte-wise representation of a fixed or floating point value as a
    joined output string.

    Args:
        value (str) : A value passed as string. Content of the string can be fixed or floating point.
        datatype (str) : Datatype of the underlying value which is used in the hex conversion.
        endianness (str) : Endianness of the hex conversion. Little and Big-Endian might be used here while big endian
            is the default value

    Returns:
        A list containing the paths of .c-files which were generated. And also the generated files themselves.
    """

    value = value.replace("u", "").replace("f", "")
    if datatype == "float32_T":
        value_casted = float(value)
    elif datatype in ct_sr.types_order_big_to_small:
        value_casted = int(value)
    else:
        logging.error("Datatype %s is not supported!", datatype)
        raise Exception("Invalid Datatype.")

    if datatype == "float32_T":
        bytes_packed = struct.pack(f"{endianness}f", value_casted)
    elif datatype == "int8_t":
        bytes_packed = struct.pack(f"{endianness}b", value_casted)
    elif datatype in ("uint8_t", "boolean_T"):
        bytes_packed = struct.pack(f"{endianness}B", value_casted)
    elif datatype == "int16_t":
        bytes_packed = struct.pack(f"{endianness}h", value_casted)
    elif datatype == "uint16_t":
        bytes_packed = struct.pack(f"{endianness}H", value_casted)
    elif datatype == "int32_t":
        bytes_packed = struct.pack(f"{endianness}i", value_casted)
    elif datatype == "uint32_t":
        bytes_packed = struct.pack(f"{endianness}I", value_casted)

    hex_to_add = "".join([f"{b:02x}" for b in bytes_packed])

    return hex_to_add


def create_customer_data_stream_xml(cal_dict: Dict[str, ct_sr.Calibration], calibration_path: str,
                                    generic_cal_info: ct_sr.Generic_Calibration_Info,
                                    _customer: str,
                                    structure_name:str) -> None:
    """
    Main function that generates the customer specific calibration xml data streams. Those are needed for
    flashing process of calibrations to an embedded hardware. This data stream is given as big- and little-endian.

    Args:
        cal_dict (dict) : A dictionary containing calibration names as keys and Calibration objects as values.
        calibration_path (str) : Path pointing to the Calibration folder where the customer sub folders are located.
        generic_cal_info (Generic_Calibration_Info) : General information of calibration scheme.

    Returns:
        No python internal datatype, but generates data stream files for each customer.
     """
    #########################################################################
    # Private functions
    #########################################################################
    def get_little_endian_data_stream(cals_little_endian_structure: Dict[str, ct_sr.Calibration], customer: str) -> str:
        """
        Private function which extracts the little endian hexadecimal data stream from the given cal dictionary. The
        cal dictionary will be ordered from the calibration tool internal type to small data types.
        Arrays are also considered by this routine.

        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            customer (str) : Name of the customer for which the cals shall be generated.

        Returns:
            Returns the complete little endian data stream of an internally specified order of the calibration struct.
        """
        output = ""
        # In the stream the whole content of calibration is considered (even header)
        for cal in cals_little_endian_structure.values():
            customer_specific_val = cal.get_value_of_cal_for_customer(customer)
            stream_string = ""
            if cal.f_is_array:
                if len(cal.dimensions) == 2:
                    matches = ct_sr.get_row_major_order_of_2d_array(customer_specific_val, cal.dimensions[1])
                else:
                    # Find any positive are negative digit in fixed or floating point format
                    matches = ct_sr.convert_array_string_to_list(customer_specific_val)
                # get the hex representation of each value and append it
                for match in matches:
                    stream_string += get_hex_by_type(match, cal.data_type, "<")
            else:
                stream_string += get_hex_by_type(customer_specific_val, cal.data_type, "<")
            output += f"{stream_string}"

        return output

    def get_next_n_bytes(cal: ct_sr.Calibration, data_stream: str, last_idx: int) -> Tuple[str, int]:
        """
        Private function which extracts the little endian hexadecimal data stream of a single calibration.
        Args:
            cal (Calibration) : Next calibration which shall be investigated in order to get its little endian hex
                representation.
            data_stream (str) : Calibration data stream in little endian.
            last_idx (int) : Index pointing to the point in the hexadecimal string where the previous data was located.

        Returns:
            Returns the big endian representation of a calibration by manipulating the corresponding part of the
            little endian data stream. Also updates the index and returns it to indicate where the last calibration was
            located in the little endian data stream.
        """
        split_into_single_bytes = 2
        # Use the type to identify the next n hex values which are corresponding to that type
        if cal.data_type in ["uint8_t", "int8_t", "boolean_T"]:
            hex_to_receive = 2
        if cal.data_type in ["uint16_t", "int16_t"]:
            hex_to_receive = 4
        if cal.data_type in ["uint32_t", "int32_t", "float32_T"]:
            hex_to_receive = 8

        if cal.f_is_array:
            length_array = len(ct_sr.convert_array_string_to_list(cal.default_value))
            end_idx = hex_to_receive * length_array
        else:
            # For a single cal only the native datatype is providing the number of hex values
            end_idx = hex_to_receive
        # Receive the next stream part of the next calibration variable and transform it to little endian.
        # For this the order of the bytes need to be reverted.
        byte_sub_string = data_stream[last_idx:last_idx + end_idx]
        last_idx += end_idx

        # Split sub string of currently investigated byte substring to list of bytes (2 hex-values)
        # and revert the order.
        byte_wise_string_list = [byte_sub_string[i:i + split_into_single_bytes] for i in
                                 range(0, len(byte_sub_string), split_into_single_bytes)]
        byte_wise_string_list = list(reversed(byte_wise_string_list))
        byte_wise_string = "".join(byte_wise_string_list)

        if cal.f_is_array:
            # In arrays the order needs to be reverted per value on the one hand and the order of the cal values
            # themselves in the array need to be reverted.
            # Split the string into pieces of the size of the underlying type. Revert the order and join it back.
            byte_wise_string_list = [byte_wise_string[i:i + hex_to_receive] for i in
                                     range(0, len(byte_wise_string), hex_to_receive)]
            byte_wise_string_list = list(reversed(byte_wise_string_list))
            byte_wise_string = "".join(byte_wise_string_list)

        return byte_wise_string, last_idx

    def get_big_endian_data_stream(cals_little_endian_structure: Dict[str, ct_sr.Calibration], little_endian_data_stream: str) -> str:
        """
        Private function which extracts the little endian hexadecimal data stream of the whole calibration structure.

        Args:
            cals (Calibration) : A dictionary containing calibration names as keys and Calibration objects as values.
            big_endian_data_stream (str) : Calibration data stream in big endian.
        Returns:
            Returns the little endian representation of the whole calibration structure in an iterative way.
        """
        output = ""
        # In the stream the whole content of calibration is considered (even h3_hdr)
        last_idx = 0
        for cal in cals_little_endian_structure.values():
            stream_to_add, last_idx = get_next_n_bytes(cal, little_endian_data_stream, last_idx)
            output = f"{stream_to_add}" + output
        return output

    def write_cal_data_stream_file(cals: Dict[str, ct_sr.Calibration], path: str, _customer: str,
                                   _generic_cal_info: ct_sr.Generic_Calibration_Info,
                                   structure_name:str) -> None:
        """
        Private function which calls string replacement sub functions and writes the formatted string output to a
        deployment path.
        Args:
            cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
            path (str) : Deployment path of the .c-file.
            _customer (str) : Name of the customer for which the cals shall be generated.
            _generic_cal_info (Generic_Calibration_Info) : General information of calibration scheme.
        Returns:
            None but generates the customer specific calibration data stream .xml-file.
        """
        target_deployment_filename = _generic_cal_info.get_deployment_file_name(structure_name) + "_data_stream.xml"
        logging.info("Now generating %s for %s_%s", target_deployment_filename, _generic_cal_info.component_name, _customer)

        file_path = os.path.join(path, target_deployment_filename)

        cal_header: Dict[str, ct_sr.Calibration] = ct_sr.get_cal_header_in_specific_order(cals, "little")
        ordered_cals = ct_sr.sort_cal_dict_from_big_to_small(cals)
        cals_little_endian_structure = {**cal_header, **ordered_cals}

        little_endian_data_stream = get_little_endian_data_stream(cals_little_endian_structure, _customer)

        output = get_customer_data_stream_xml_file(_generic_cal_info)
        output = output.replace("<<<INSERT_LITTLE_ENDIAN_STREAM_HERE>>>", little_endian_data_stream)
        output = output.replace("<<<INSERT_BIG_ENDIAN_STREAM_HERE>>>", get_big_endian_data_stream(cals_little_endian_structure, little_endian_data_stream))

        with open(file_path, "w", encoding="utf-8") as f:
            f.write(output)

    #########################################################################
    # Start of public functions code
    #########################################################################
    write_cal_data_stream_file(cal_dict, calibration_path, _customer, generic_cal_info, structure_name)
