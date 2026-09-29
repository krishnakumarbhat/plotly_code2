"""This file defines the file skeleton for customer specific data stream xml files for integration. Those f-strings are formatted in a way,
that formatting via clang-format is not necessarily required.
Additional formatting is done in the functions for generation of the strings to be replaced. That's why the string
replacement indicators are not formatted here."""
from python_src.ct_shared_resources import Generic_Calibration_Info, cal_tool_version


def get_customer_data_stream_xml_file(generic_cal_info: Generic_Calibration_Info) -> str:
    """
    Public function that returns a file skeleton for the customer specific data stream .xml-file and adapts module
    name related replacements.

    Args:
        generic_cal_info (Generic_Calibration_Info) : object for basic string replacements given by generic schema

    Returns:
        Returns the file skeleton as formatted string with replacements
    """

    customer_data_stream_xml_file = """\
<?xml version=1.0 encoding=UTF-8 standalone=yes?>
<CALIBRATION_XML>
<CALIB_FILE_NAME><<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>_cal.xml</CALIB_FILE_NAME>
<CALIB_TOOL_VERSION><<<INSERT_CT_VERSION_HERE>>></CALIB_TOOL_VERSION>
<CALIB_DATA_BE><<<INSERT_BIG_ENDIAN_STREAM_HERE>>></CALIB_DATA_BE>
<CALIB_DATA_LE><<<INSERT_LITTLE_ENDIAN_STREAM_HERE>>></CALIB_DATA_LE>
</CALIBRATION_XML>
"""

    output = customer_data_stream_xml_file.replace("<<<INSERT_COMPONENT_NAME_LOWER_CASE_HERE>>>", generic_cal_info.component_name.lower())
    output = output.replace("<<<INSERT_CT_VERSION_HERE>>>", cal_tool_version)
    return output
