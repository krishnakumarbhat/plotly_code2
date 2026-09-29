"""This file provides shared functions for the calibration tool as well as class definitions for parsed data."""
import re
import logging
from typing import List, Dict
from dataclasses import dataclass, field


cal_tool_version = "5.0.3"
"""Cal tool version consisting of Major, Minor, Patch version based on semver."""

type_ranges = {
    "boolean_T": {"min": 0, "max": 1},
    "uint8_t": {"min": 0, "max": (1<<8) - 1},
    "uint16_t": {"min": 0, "max": (1<<16) - 1},
    "uint32_t": {"min": 0, "max": (1<<32) - 1},
    "int8_t": {"min": -(1<<7), "max": (1<<7) - 1},
    "int16_t": {"min": -(1<<15), "max": (1<<15) - 1},
    "int32_t": {"min": -(1<<31), "max": (1<<31) - 1},
    "float32_T": {"min": -3.402823466E+38, "max": 3.402823466E+38}
    }
"""Summarizes ranges of supported types. Numpy is not used here because it would increase the executable size by 60MB."""

cal_header_order = ["Section_Size", "version", "Section_Compatibility", "Cal_Chk_Sum", "Chk_sum_Version", "Cal_Type"]
"""Summarizes the member of calibration header. The order is chosen from big to small data types."""

types_order_big_to_small = ["double", "float32_T", "int32_t", "uint32_t", "int16_t", "uint16_t", "boolean_T",
                            "int8_t", "uint8_t"]
"""Summarizes types of export files in the order of big to small data types"""

type_to_byte_dict = {"float32_T": 4, "int32_t": 4, "uint32_t": 4, "int16_t": 2, "uint16_t": 2, "int8_t": 1,
                     "uint8_t": 1, "boolean_T": 1}
""" Dict[str, int]: Summarizes the size of basic datatypes for a reference architecture
(double does not exist here and is permitted)."""

endian_swap_satisfier = {4: "CT_FOUR_BYTE", 2: "CT_TWO_BYTE", 1: "CT_ONE_BYTE"}
""" Dict[int, str]: Used for endianness swap.

Maps byte sizes to an externally provided type definition and is dependent on C type structure located in c_src.
Limitation: Double is not covered here.
"""

regex_any_decimal_number_with_suffix = r"[-\d\w\.]+"
""" str: Shared regular expression.

Matches any (negative) decimal digit given via default_value or customer_specific_cal property. In addition
to that the type suffix is also matched.
"""

regex_dimension_check = r"\[[\d\w\-\.\s]*?\]"
"""str: Shared regular expression.

Mainly used for dimension consistency check and also matching square brackets.
"""

@dataclass
class Generic_Calibration_Info:
    """
    Class summarizing generic information of a provided cal sheet. Define attributes statically, such that
    no further attributes can be appended like in a hashed dictionary. Dataclass provides constructor automatically.

    Attributes:
        type_include_file (str) : Include file pointing to the necessary basic type definitions
        component_name (str) : Name of the component used for prefixing of functions and macros
        export_asil_check_flag (bool) : Flag indicating whether the asil check function shall be generated.
        public_structure_name (str) : Name of the calibration data type.
        customer_structure_name (str) : Name of the calibration data type.
        core_structure_name (str) : Name of the calibration data type.
        customers (List[str]) : list of customers for which calibrations shall be generated
    """

    type_include_file: str
    component_name: str
    export_asil_check_flag: bool
    public_structure_name: str
    customer_structure_name: str
    core_structure_name: str
    customers: List[str]
    def get_deployment_file_name(self, structure_name):
        assert structure_name[-2:] == "_T"
        deployment_file_name = structure_name[:-2].lower()
        return deployment_file_name

    def get_cal_size_macro_name(self, structure_name):
        assert structure_name[-2:] == "_T"
        cal_size_macro_name = structure_name[:-2].upper() + "_SIZE"
        return cal_size_macro_name

    def get_method_name_prefix(self, structure_name):
        assert structure_name[-2:] == "_T"
        method_name_prefix = structure_name[:-2].replace("Calibration", "Cal")
        return method_name_prefix

@dataclass
class Calibration:
    """
    This is a class representation of calibrations parsed from .xml or other file formats. Define attributes
    statically,such that no further attributes can be appended like in a hashed dictionary.
    Dataclass provides constructor automatically.

    Attributes:
        name (str) : Identifier of a calibration
        data_type (str) : Target data type of the calibration when exporting into .h- or .c-files.
        data_resolution (str) : Quantization of the given calibration.
        range_min (str) : Minimum allowed value of the given calibration.
        range_max (str) : Maximum allowed value of the given calibration.
        default_value (str) : Default value of calibration value.
        f_is_array (bool) : Flag indicating if calibration value is of array-type.
        f_is_constant (bool): Flag indicating that default value cannot be modified or override by client
        dimensions (List[int]) : Dimensions of an array. In case that an 2D-Array is given, it contains two entries.
        description (str) : Meta information for the given calibration.
        unit (str) : physical unit of the given calibration (e.g. m, m/s).
        customer_specific_values (Dict[str, str]) : Dictionary of customer specific calibration values with customer name as key
            (string) and calibration value as dictionary value (string)

    """
    name: str
    data_type: str
    data_resolution: str
    range_min: str
    range_max: str
    default_value: str
    f_is_array: bool
    f_is_constant: bool = False
    description: str = None
    unit: str = None
    dimensions: List[int] = field(default_factory = list)
    customer_specific_values: Dict[str, str] = field(default_factory = dict)

    def __post_init__(self) -> None:
        """
        Method called after initialization routine, which is automatically setting dimensions based on the given
        input of standard constructor.

        Args:
            default_value (str) : Default value given in initialization routine with squared brackets inclusively.

        """
        self.dimensions: list[int] = self.extract_dimension(self.default_value)


    def get_dimension_name_by_cal(self, component_name: str, index_string: str) -> str:
        """
        Class function returning the dimension macro of the calibration for consistent naming in calibration tool.

        Args:
            component_name (str) : Name of the component
            index_string (str) : index as string used as macro suffix

        Returns:
            String representation of the dimension macro to be defined.
        """
        output = f"{component_name.upper()}_{self.name.upper()}_ARRAY_SIZE_DIM{index_string}"
        return output

    def get_minimum_value_macro_by_cal(self, component_name: str) -> str:
        """
        Class function returning the minimum macro of the calibration for consistent naming in calibration tool.

        Args:
            component_name (str) : Name of the component

        Returns:
            String representation of the minimum macro to be defined.
        """
        output = f"{component_name.upper()}_MIN_{self.name.upper()}"
        return output

    def get_maximum_value_macro_by_cal(self, component_name: str) -> str:
        """
        Class function returning the maximum macro of the calibration for consistent naming in calibration tool.

        Args:
            component_name (str) : Name of the component

        Returns:
            String representation of the maximum macro to be defined.
        """
        output = f"{component_name.upper()}_MAX_{self.name.upper()}"
        return output

    def update_customer_specific_calibration_dict(self, customer_specific_cal_dict: dict) -> None:
        """
        Class function for updating of customer_specific_values member.

        Args:
            customer_specific_cal_dict (dict) : Dictionary with customers as key values (string) and customer specific

        Returns:
            None but updates member variable of a Calibration object.
        """
        self.customer_specific_values = customer_specific_cal_dict

    def get_value_of_cal_for_customer(self, customer: str) -> str:
        """
        Class function which returns the calibration value used by a given customer.

        Args:
            customer (str) : Customer as a string for which the calibration value shall be returned.

        Returns:
            Customer specific calibration value if the calibration is overwritten, or the default value as default.
        """
        output = ""
        if customer in self.customer_specific_values:
            output += f"{self.customer_specific_values[customer]}"
        else:
            output += f"{self.default_value}"
        return output

    def extract_dimension(self, value: str) -> List[int]:
        """
        Class function for extraction of array dimensions of an input string value.

        Args:
            value (str) : default_value or customer specific value.

        Returns:
            List of integers (array dimensions)
        """
        dimensions=[]
        if self.f_is_array:
            if "[[[" in value:
                # Prepare string in case of three dimensions
                temp_list = value.split("]][[")
                dimensions.append(len(temp_list))
                string_test = temp_list[0] + "]"
            else:
                # Pass default value directly to string to be processed for lower dimensions
                string_test = value
            matches = re.findall(regex_dimension_check, string_test)
            if "[[[" in value:
                # Sanity check for the first dimension of 3D array.
                if not all(len(matches) == len(re.findall(regex_dimension_check, "["+string+"]")) \
                    for string in value.split("]][[")):
                    logging.error("Dimensions of cal %s are inconsistent.", self.name)
            if "[[" in value:
                dimensions.append(len(matches))
                if not all(len(matches[0].split(" ")) == len(match.split(" ")) for match in matches):
                    # Sanity check for the 2D- or 3D-arrays inner dimensions.
                    logging.error("Dimensions of cal %s are inconsistent.", self.name)
            if "[" in value:
                dimensions.append(len(matches[0].split(" ")))

        return dimensions

def get_row_major_order_of_2d_array(value: str, dimension: int) -> List[str]:
    """
    Public function for row major order string generation of a given 2d array.

    Args:
        value (str) : A string containing values of a given array e.g. in this format
            ([[a11 a12 a13][a21 a22 a23][a31 a32 a33][a41 a42 a43]]).
        dimension (int) : length of the most inner dimension. In the above example this would be 4.

    Returns:
        List of string (1d representation of 2d-array) containing values of the input
        array in row major order (like [a11, a21, a31, a12, a22, a32, a13, a23, a33] with the above example)
    """
    matches_old = re.findall(regex_any_decimal_number_with_suffix, value)
    list_temp=[]
    for y in range(0, dimension):
        sublist=[]
        for x in range(y, len(matches_old), dimension):
            sublist.append(matches_old[x])
        list_temp.append(sublist)
    # Transpose the list of lists for row major order.
    list_temp_t = str(list(zip(list_temp)))
    matches_new = re.findall(regex_any_decimal_number_with_suffix, list_temp_t)
    return matches_new

def sort_dict_from_big_to_small_types(cals: Dict[str, Calibration]) -> Dict[str, Calibration]:
    """
    Public function to sort a dict with respect to types.

    Args:
        cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.

    Returns:
        Sorted list which starts with big types of target language (c/c++) and ends with small types.
    """
    output: Dict[str, Calibration] = {}
    for data_type in types_order_big_to_small:
        for cal in cals:
            if not cals[cal].f_is_constant:
                continue
            if data_type == cals[cal].data_type:
                output.update({cal: cals[cal]})
        for cal in cals:
            if cals[cal].f_is_constant:
                continue
            if data_type == cals[cal].data_type:
                output.update({cal: cals[cal]})
    return output


def are_arrays_used_in_cals(cals: Dict[str, Calibration]) -> bool:
    """
    Public function to check if any arrays are part of calibration set.

    Args:
        cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.

    Returns:
        Bool flag indicating if arrays are used.
    """
    f_array_used = False
    for cal in cals:
        if cals[cal].f_is_array:
            f_array_used = True

    return f_array_used


def convert_array_string_to_list(array_as_string: str) -> List[str]:
    """
    Public function to get the values of an array as list. Here the type suffix is also matched.

    Args:
        array_as_string (str) : String version of the array values

    Returns:
        Array values as list of strings with type suffix
    """
    array_as_list = re.findall(regex_any_decimal_number_with_suffix, array_as_string)

    return array_as_list


def change_order_of_dict_by_name(cals: Dict[str, Calibration], name_list: list) -> Dict[str, Calibration]:
    """
    Public function to extract a sub dictionary with a specific order specified by the input name_list.

    Args:
        cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
        name_list (list) : A list containing strings which are giving the output order of the sub dictionary.

    Returns:
        Sub dictionary in a specific order specified by the input name list.
    """
    output: Dict[str, Calibration] = {}
    for data_type in name_list:
        for cal in cals:
            if data_type == cals[cal].name:
                output.update({cal: cals[cal]})
    return output


def get_cal_size(cals: Dict[str, Calibration]) -> str:
    """
    Public function to calculate the byte size of the whole calibration structure to be generated. This is done via
    an internal map from target data type to byte size.

    Args:
        cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.

    Returns:
        Size of the target calibration structure as string in Bytes
    """
    cal_size = 0
    for cal in cals:
        byte_per_type = type_to_byte_dict[cals[cal].data_type]
        if cals[cal].f_is_array:
            num_variables_of_cal = 1
            for dimension in cals[cal].dimensions:
                num_variables_of_cal *= dimension
        else:
            num_variables_of_cal = len(cals[cal].default_value.split(" "))
        cal_size += byte_per_type * num_variables_of_cal
    return f"{str(cal_size)}"


def get_cal_header_in_specific_order(cals: Dict[str, Calibration], endianness: str = "little") -> Dict[str, Calibration]:
    """
    This function sorts the calibration tool internal type assumption in a specific order.

    Args:
        cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.
        endianness (str) : Endianness which is specifying in which order the calibrations shall be sorted with
            respect to their types.

    Returns:
        Calibration tool internal type in a specific order.
    """
    h3_member = {}

    # switch between different modes for h3_hdr type
    if endianness == "little":
        cal_header_hdr_order = cal_header_order
    else:
        cal_header_hdr_order = reversed(cal_header_order)

    # assume that the rest of the cals are already sorted from big to small datatypes.
    for h3_member_str in cal_header_hdr_order:
        for cal in cals:
            if cals[cal].name == h3_member_str:
                h3_member.update({cal: cals[cal]})
    return h3_member


def sort_cal_dict_from_big_to_small(cals: Dict[str, Calibration]) -> Dict[str, Calibration]:
    """
    This function sorts the cal struct in an order such that h3_member variables are in front and the components
    calibration are following with an order from big to small data types.

    Args:
        cals (Dict[str, ct_sr.Calibration]) : A dictionary containing calibration names as keys and Calibration objects as values.

    Returns:
        Calibration dictionary with expected target file order.
    """
    cals_without_header: Dict[str, Calibration] = {}
    for data_type in types_order_big_to_small:
        for cal_name, cal in cals.items():
            if cal_name not in cal_header_order and data_type == cal.data_type:
                cals_without_header.update({cal_name: cal})
    return cals_without_header
