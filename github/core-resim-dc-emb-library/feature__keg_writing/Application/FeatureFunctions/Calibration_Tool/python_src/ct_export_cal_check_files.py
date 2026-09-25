"""This file contains functions for creation of the core cal check files. Here the string for the boundary checks is replaced."""
import ast
import os
import logging
from typing import List, Dict

import python_src.file_skeletons.ct_core_cal_check_skeleton as ct_cccs
import python_src.ct_shared_resources as ct_sr

#########################################################################
# Public functions
#########################################################################
def create_cal_check_files(cal_dict: Dict[str, ct_sr.Calibration], cal_core_path: str,\
    generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str) -> None:
    """
    Public function for generation of core cal check files

    Args:
        cal_dict (Dict[str, ct_sr.Calibration]) : calibration dictionary providing information about all given cals.
        cal_core_path (str) : path to which the files shall be generated
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
     """

    def write_h_file(_cal_core_path: str, _generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str):
        """
        Private function for generation of cal check header file.

        Args:
            _cal_core_path (str) : path to which the files shall be generated
            _generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

        Returns:
            Returns the file skeleton as formatted string with replacements
        """
        deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
        file_name: str = deployment_file_name + "_check.h"
        output_path = os.path.join(_cal_core_path, file_name)
        logging.info("Now generating %s for %s", file_name, _generic_cal_info.component_name)

        output = ct_cccs.get_core_cal_check_h_file_skeleton(_generic_cal_info, structure_name)
        with open(output_path, "w", encoding="utf-8") as f:
            f.write(output)

    def sort_dictionary(input_dict: Dict[str, ct_sr.Calibration])\
         -> Dict[str, Dict[str, ct_sr.Calibration]]:
        """
        Private function for sorting dictionary by key valie.

        Args:
            input_dict (Dict[str, ct_sr.Calibration]) : calibration dictionary providing information about all given calibrations.

        Returns:
            Returns dictionary sorted by key value.
        """
        sorted_keys = sorted(input_dict.keys())
        sorted_input_dict = {key: input_dict[key] for key in sorted_keys}

        return sorted_input_dict

    def get_cals_sorted_by_dimensions(_cal_dict: Dict[str, ct_sr.Calibration])\
         -> Dict[str, Dict[str, ct_sr.Calibration]]:
        """
        Private function to get the calibration restructured in a multidimensional dict summarizing similar calibrations
        respective to their dimensions.

        Args:
            _cal_dict (Dict[str, ct_sr.Calibration]) : calibration dictionary providing information about all given calibrations.

        Returns:
            Returns calibrations sorted by their dimensions as a multidimensional dictionary.
        """
        # Reformat the calibration dictionary such that the calibrations are sorted in a dict of dict by their dimension
        # for that get the unique dimensions as keys for the dict of dict
        unique_dimensions: List[str] = list(set({str(value.dimensions) for value in _cal_dict.values()}))
        cals_by_dimensions: Dict[str, Dict[str, ct_sr.Calibration]] = {key:{} for key in unique_dimensions}
        for key, value in _cal_dict.items():
            cals_by_dimensions[str(value.dimensions)].update({key: value})

        # sorting outer dict
        cals_sorted_by_dimensions = sort_dictionary(cals_by_dimensions)

        # sorting outer dicts
        for key in cals_sorted_by_dimensions.keys():
            cals_sorted_by_dimensions[key] = sort_dictionary(cals_by_dimensions[key])

        return cals_sorted_by_dimensions

    def get_dimension_loop_suffix(_cal:ct_sr.Calibration) -> str:
        """
        Private function to get array access as a substring for loop boundary checks.

        Args:
            _cal (ct_sr.Calibration) : information of a single calibration.

        Returns:
            Returns a string containing the access to array elements dependent on the dimension of those.
        """
        output = ""

        if len(_cal.dimensions) == 2:
            output += f"[{ct_cccs.loop_variables[0]}][{ct_cccs.loop_variables[1]}]"
        elif len(_cal.dimensions) == 1:
            output += f"[{ct_cccs.loop_variables[0]}]"

        return output

    def get_boundary_check_definition_of_calibration(_cal: ct_sr.Calibration, _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to get a calibration check string for a single calibration.

        Args:
            _cal (ct_sr.Calibration) : information of a single calibration.
            _generic_cal_info (ct_sr.Generic_Calibration_Info) : generic string replacements.

        Returns:
            Returns calibrations sorted by their dimensions as a multidimensional dictionary.
        """
        range_min_adapted = _cal.range_min.replace("u","").replace("f","")
        range_max_adapted = _cal.range_max.replace("u","").replace("f","")
        boundary_check_skeleton = ""
        # Only add a boundary check for calibrations which dont have their xml ranges set to the datatype ranges to prevent the creation of
        # compiler warnings.
        if range_min_adapted != str(ct_sr.type_ranges[_cal.data_type]["min"]) or\
            range_max_adapted != str(ct_sr.type_ranges[_cal.data_type]["max"]) or\
            _cal.data_type == "boolean_T":

            boundary_check_skeleton = ct_cccs.get_boundary_check_skeleton(_generic_cal_info)
            cal_with_dim_suffix = f"{_cal.name}{get_dimension_loop_suffix(_cal)}"
            boundary_check = ""
            if _cal.data_type == "boolean_T":
                # In case of boolean always check for equality to lower and upper boundary
                boundary_check += f"Ct_Is_Bool_In_Bondaries(&f_{_generic_cal_info.component_name.lower()}_calibration_in_boundaries, "
                boundary_check += f"{_cal.get_minimum_value_macro_by_cal(_generic_cal_info.component_name)}, "
                boundary_check += f"p_calibration->{cal_with_dim_suffix}, "
                boundary_check += f"{_cal.get_maximum_value_macro_by_cal(_generic_cal_info.component_name)})"
            else:
                lower_boundary = ""
                upper_boundary = ""

                # Check whether a check is not necessary due to underlying type limitations
                if range_min_adapted != str(ct_sr.type_ranges[_cal.data_type]["min"]):
                    lower_boundary = f"{_cal.get_minimum_value_macro_by_cal(_generic_cal_info.component_name)}"
                else:
                    logging.debug("Lower boundary check condition for %s is refused. The lower xml range is representing a "\
                        "type limit of %s", _cal.name, _cal.data_type)
                    lower_boundary = f"{range_min_adapted}"
                if range_max_adapted != str(ct_sr.type_ranges[_cal.data_type]["max"]):
                    upper_boundary = f"{_cal.get_maximum_value_macro_by_cal(_generic_cal_info.component_name)}"
                else:
                    logging.debug("Upper boundary check condition for %s is refused. The lower xml range is representing a "\
                        "type limit of %s", _cal.name, _cal.data_type)
                    upper_boundary = f"{range_max_adapted}"

                #Combine both checks depending on whether a check is necessary
                if lower_boundary == "" or upper_boundary == "":
                    boundary_check += f"({lower_boundary}{upper_boundary})"
                else:
                    if _cal.data_type == "float32_T":
                        boundary_check += "Ct_Is_Float_In_Bondaries"
                    elif _cal.data_type == "uint8_t":
                        boundary_check += "Ct_Is_Uint8_In_Bondaries"
                    elif _cal.data_type == "uint16_t":
                        boundary_check += "Ct_Is_Uint16_In_Bondaries"
                    elif _cal.data_type == "uint32_t":
                        boundary_check += "Ct_Is_Uint32_In_Bondaries"
                    elif _cal.data_type == "int8_t":
                        boundary_check += "Ct_Is_Int8_In_Bondaries"
                    elif _cal.data_type == "int16_t":
                        boundary_check += "Ct_Is_Int16_In_Bondaries"
                    elif _cal.data_type == "int32_t":
                        boundary_check += "Ct_Is_Int32_In_Bondaries"
                    else:
                        boundary_check +="//"
                    boundary_check += f"(&f_{_generic_cal_info.component_name.lower()}_calibration_in_boundaries, "
                    boundary_check += f"{lower_boundary}, p_calibration->{cal_with_dim_suffix}, {upper_boundary})"

            boundary_check_skeleton = boundary_check_skeleton.replace("<<<INSERT_BOUNDARY_CHECK_HERE>>>", boundary_check)
        else:
            logging.debug("Boundary check for calibration %s is refused since both ranges given in xml are presenting "
                "the type limits of %s", _cal.name, _cal.data_type)

        return boundary_check_skeleton


    def get_2d_boundary_checks(_cals_sorted_by_dimensions: Dict[str, Dict[str, ct_sr.Calibration]],\
        _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to get boundary checks for two dimensional arrays.

        Args:
            _cals_sorted_by_dimensions (Dict[str, Dict[str, ct_sr.Calibration]]) : calibrations sorted by their dimensions in
                a multidimensional array.
            _generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

        Returns:
            Returns output string containing all boundary checks for two dimensional arrays.
        """
        output = ""

        # Loop through all calibrations assigned to their respective dimension such that loops can be summarized.
        for dim_key, sub_dict in _cals_sorted_by_dimensions.items():
            # Check whether a two dimensional array is contained
            if len(ast.literal_eval(dim_key)) == 2:
                # Build a loop based on arbitrarily chosen dimensions of the subdict (variables of the subdict all have
                # the same dimension)
                any_cal_of_subdict: ct_sr.Calibration = next(iter(sub_dict.values()))
                string_to_attach: str = ct_cccs.get_2d_loop_placeholder_skeleton(
                    any_cal_of_subdict.get_dimension_name_by_cal(_generic_cal_info.component_name, "0"),
                    any_cal_of_subdict.get_dimension_name_by_cal(_generic_cal_info.component_name, "1"))

                #Get boundary checks
                boundary_checks = ""
                for _cal in sub_dict.values():
                    boundary_checks += get_boundary_check_definition_of_calibration(_cal, _generic_cal_info)
                string_to_attach = string_to_attach.replace("<<<INSERT_CONTENT_HERE>>>", boundary_checks)

                output += string_to_attach
        return output

    def get_1d_boundary_checks(_cals_sorted_by_dimensions: Dict[str, Dict[str, ct_sr.Calibration]],\
        _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to get boundary checks for one dimensional arrays.

        Args:
            _cals_sorted_by_dimensions (Dict[str, Dict[str, ct_sr.Calibration]]) : calibrations sorted by their dimensions
                in a multidimensional array.
            _generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

        Returns:
            Returns output string containing all boundary checks for one dimensional arrays.
        """
        output = ""

        # Loop through all calibrations assigned to their respective dimension such that loops can be summarized.
        for dim_key, sub_dict in _cals_sorted_by_dimensions.items():
            # Check whether a one dimensional array is contained
            if len(ast.literal_eval(dim_key)) == 1:
                # Build a loop based on arbitrarily chosen dimensions of the subdict (variables of the subdict all have
                # the same dimension)
                any_cal_of_subdict: ct_sr.Calibration = next(iter(sub_dict.values()))
                string_to_attach: str = ct_cccs.get_1d_loop_placeholder_skeleton(
                    any_cal_of_subdict.get_dimension_name_by_cal(_generic_cal_info.component_name, "0"))

                #Get boundary checks
                boundary_checks = ""
                for _cal in sub_dict.values():
                    boundary_checks += get_boundary_check_definition_of_calibration(_cal, _generic_cal_info)
                string_to_attach = string_to_attach.replace("<<<INSERT_CONTENT_HERE>>>", boundary_checks)

                output += string_to_attach
        return output

    def get_constant_boundary_checks(_cals_sorted_by_dimensions: Dict[str, Dict[str, ct_sr.Calibration]],\
        _generic_cal_info: ct_sr.Generic_Calibration_Info) -> str:
        """
        Private function to get boundary checks for one dimensional arrays.

        Args:
            _cals_sorted_by_dimensions (Dict[str, Dict[str, ct_sr.Calibration]]) : calibrations sorted by
                their dimensions in a multidimensional array.
            _generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

        Returns:
            Returns output string containing all boundary checks for one dimensional arrays.
        """
        output = ""
        # Loop through all calibrations assigned to their respective dimension such that loops can be summarized.
        for dim_key, sub_dict in _cals_sorted_by_dimensions.items():
            # Check whether a constant is contained
            if len(ast.literal_eval(dim_key)) == 0:
                string_to_attach: str = ""

                #Attach boundary checks
                for _cal in sub_dict.values():
                    if _cal.name not in ct_sr.cal_header_order:
                        string_to_attach += get_boundary_check_definition_of_calibration(_cal, _generic_cal_info)

                output += string_to_attach
        return output

    def write_c_file(cal_dict: Dict[str, ct_sr.Calibration], cal_core_path: str, generic_cal_info: ct_sr.Generic_Calibration_Info, structure_name:str):
        """
        Private function to export the boundary check .c-file. Here boundary checks are supported for 2D-, 1D-arrays as well as constant
        calibrations.

        Args:
            cal_dict (Dict[str, ct_sr.Calibration]) : calibrations sorted by their dimensions in a multidimensional array.
            _generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

        Returns:
            Returns output string containing all boundary checks for one dimensional arrays.
        """
        deployment_file_name = generic_cal_info.get_deployment_file_name(structure_name)
        file_name: str = deployment_file_name + "_check.c"
        output_path = os.path.join(cal_core_path, file_name)

        logging.info("Now generating %s for %s", file_name, generic_cal_info.component_name)

        cals_sorted_by_dimensions: Dict[str, Dict[str, ct_sr.Calibration]]= get_cals_sorted_by_dimensions(cal_dict)

        output = ct_cccs.get_core_cal_check_c_file_skeleton(generic_cal_info, structure_name)
        output = output.replace("<<<INSERT_2D_ARRAY_BOUNDARY_CHECKS_HERE>>>", get_2d_boundary_checks(cals_sorted_by_dimensions, generic_cal_info))
        output = output.replace("<<<INSERT_1D_ARRAY_BOUNDARY_CHECKS_HERE>>>", get_1d_boundary_checks(cals_sorted_by_dimensions, generic_cal_info))
        output = output.replace("<<<INSERT_CONSTANT_BOUNDARY_CHECKS_HERE>>>", get_constant_boundary_checks(cals_sorted_by_dimensions, generic_cal_info))
        with open(output_path, "w", encoding="utf-8") as f:
            f.write(output)

    #########################################################################
    # Start of public functions code
    #########################################################################
    write_h_file(cal_core_path, generic_cal_info, structure_name)
    write_c_file(cal_dict, cal_core_path, generic_cal_info, structure_name)
