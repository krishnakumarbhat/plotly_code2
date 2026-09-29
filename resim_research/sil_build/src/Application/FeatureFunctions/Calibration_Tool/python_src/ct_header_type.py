"""This file contains the header type of the calibration tool. This type provides basic information of internals of the calibration tool.
"""
from typing import Dict

import python_src.ct_shared_resources as ct_sr

CAL_TYPE_DEFAULT_VALUE = "3u"
CHK_SUM_VERSION_DEFAULT_VALUE = "1u"
SECTION_COMPATIBILITY_DEFAULT_VALUE = "3u"
DEFAULT_VALUE_TO_OVERWRITE = "0u"

#########################################################################
# Public functions
#########################################################################

def initialize_cal_header() -> Dict[str, ct_sr.Calibration]:
    """
    Public function to calculate initialize

        Returns:
            Returns the header specific calibrations as a dictionary. The order is sorted from big to small types by default.
    """
    cals: Dict[str, ct_sr.Calibration] = {}
    data_resolution: str = "1u"

    cals.update({ct_sr.cal_header_order[0]: ct_sr.Calibration(name = ct_sr.cal_header_order[0], data_type="uint32_t", data_resolution = data_resolution,
    range_min = str(ct_sr.type_ranges["uint32_t"]["min"]), range_max = str(ct_sr.type_ranges["uint32_t"]["max"]), default_value = DEFAULT_VALUE_TO_OVERWRITE,
    f_is_array = False, description = "Size of the section based on the types. This also includes datatypes of the header.", unit = "None")})

    cals.update({ct_sr.cal_header_order[1]: ct_sr.Calibration(name = ct_sr.cal_header_order[1], data_type = "uint16_t", data_resolution = data_resolution,
    range_min = str(ct_sr.type_ranges["uint16_t"]["min"]), range_max = str(ct_sr.type_ranges["uint16_t"]["max"]), default_value = DEFAULT_VALUE_TO_OVERWRITE,
    f_is_array = False, description =  "Version of the byte-structure based on the types. This needs to be adapted when a type is changed and" +
     "a cal is removed or added. Expected to be provided by the user.", unit =  "None")})

    cals.update({ct_sr.cal_header_order[2]: ct_sr.Calibration(name = ct_sr.cal_header_order[2], data_type = "uint16_t", data_resolution = data_resolution,
    range_min = str(ct_sr.type_ranges["uint16_t"]["min"]), range_max = str(ct_sr.type_ranges["uint16_t"]["max"]),
    default_value = SECTION_COMPATIBILITY_DEFAULT_VALUE, f_is_array = False, description = "Information for the section compatibility.", unit =  "None")})

    cals.update({ct_sr.cal_header_order[3]: ct_sr.Calibration(name = ct_sr.cal_header_order[3], data_type = "uint16_t", data_resolution = data_resolution,
    range_min = str(ct_sr.type_ranges["uint16_t"]["min"]), range_max = str(ct_sr.type_ranges["uint16_t"]["max"]), default_value = DEFAULT_VALUE_TO_OVERWRITE,
    f_is_array = False, description = "Contains the checksum based on the customer specific calibrations without header type values.", unit =  "None")})

    cals.update({ct_sr.cal_header_order[4]: ct_sr.Calibration(name = ct_sr.cal_header_order[4], data_type = "uint8_t", data_resolution = data_resolution,
    range_min = str(ct_sr.type_ranges["uint8_t"]["min"]), range_max = str(ct_sr.type_ranges["uint8_t"]["max"]), default_value = CHK_SUM_VERSION_DEFAULT_VALUE,
    f_is_array = False, description = "Holds information about the check sum version.", unit =  "None")})

    cals.update({ct_sr.cal_header_order[5]: ct_sr.Calibration(name = ct_sr.cal_header_order[5], data_type = "uint8_t", data_resolution = data_resolution,
    range_min = str(ct_sr.type_ranges["uint8_t"]["min"]), range_max = str(ct_sr.type_ranges["uint8_t"]["max"]),  default_value = CAL_TYPE_DEFAULT_VALUE,
    f_is_array = False, description = "Cal Type which distinguishes between different types e.g. tracker or feature", unit = "None")})

    return cals
