"""This file contains test implementations for the source file ct_export_customer_data_stream."""
from typing import Dict
import pytest
import os
from hypothesis import given
from hypothesis.strategies import integers

import python_src.ct_export_customer_data_stream as ct_cds
import python_src.ct_shared_resources as ct_sr


def twos_comp_helper_function(val: int, bits: int) -> int:
    """
    Compute the 2's complement of int value val.

    Args:
        val (int) : Any integer value with 8, 16 or 32 bits
        bits (int) : Number of bits used for the function

    Returns:
        2's complement of an input integer with a given bit size
    """
    if (val & (1 << (bits - 1))) != 0:  # if sign bit is set e.g., 8bit: 128-255
        val = val - (1 << bits)        # compute negative value
    return val


@given(integers(min_value=-128, max_value=127))
def test_get_hex_by_type__hypothesis_testing_int8_dec_to_hex_to_dec(decimal: int):
    """
    Test decimal to hexadecimal conversion. Hypothesis testing for a given set of signed integers.
    Expect conversion to hexadecimal and back to decimal does not change the underlying decimal value of type
    int8_t.
    """
    # \arrange Set up inputs for hypothesis testing.
    value: str = str(decimal)
    data_type: str = "int8_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that reversing of the transformation results in the original value
    assert value == str(twos_comp_helper_function(int(value_hex, 16), 8))


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint8_max_value():
    """
    Test decimal to hexadecimal conversion. Here the maximum of uint8_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up upper boundary of uint8_t.
    value: str = "255"
    data_type: str = "uint8_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that ff is returned
    assert "ff" == value_hex


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint8_middle_of_the_road():
    """
    Test decimal to hexadecimal conversion. Here the some value in the range of uint8_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up some value in the middle of the road of uint8_t.
    value: str = "125"
    data_type: str = "uint8_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that 7d is returned
    assert "7d" == value_hex


@given(integers(min_value=0, max_value=255))
def test_get_hex_by_type__hypothesis_testing_uint8(decimal: int):
    """
    Test decimal to hexadecimal conversion. Hypothesis testing for a given set of integers.
    Expect that the length of the returned string is 2 for datatype uint8_t.
    """
    # \arrange Set up inputs for hypothesis testing.
    value: str = str(decimal)
    data_type: str = "uint8_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that a string of length 2 is returned
    assert 2 == len(value_hex)


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_boolean():
    """
    Test decimal to hexadecimal conversion. Here a boolean_t shall be converted.
    """
    # \arrange Set up a boolean conversion.
    value: str = "1"
    data_type: str = "boolean_T"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that 01 is returned
    assert "01" == value_hex


def test_get_hex_by_type__little_endian_conversion_to_hexadecimal_uint8_middle_of_the_road():
    """
    Test decimal to hexadecimal conversion. Here the some value in the range of uint8_t
    shall be converted to hexadecimal. This time the little endian representation
    shall be returned.
    """
    # \arrange Set up some value in the middle of the road of uint8_t.
    value: str = "125"
    data_type: str = "uint8_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, "<")
    # \assert expect that 7d is returned since endianness only has effects byte-wise.
    assert "7d" == value_hex


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint8_min_value():
    """
    Test decimal to hexadecimal conversion. Here the minimum of uint8_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up lower boundary of uint8_t.
    value: str = "0"
    data_type: str = "uint8_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that 00 is returned
    assert "00" == value_hex


@given(integers(min_value=-32768, max_value=32767))
def test_get_hex_by_type__hypothesis_testing_int16_dec_to_hex_to_dec(decimal: int):
    """
    Test decimal to hexadecimal conversion. Hypothesis testing for a given set of signed integers.
    Expect conversion to hexadecimal and back to decimal does not change the underlying decimal value of type
    int16_t.
    """
    # \arrange Set up inputs for hypothesis testing.
    value: str = str(decimal)
    data_type: str = "int16_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that reversing of the transformation results in the original value
    assert value == str(twos_comp_helper_function(int(value_hex, 16), 16))


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint16_max_value():
    """
    Test decimal to hexadecimal conversion. Here the maximum of uint16_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up upper boundary of uint16_t.
    value: str = "65535"
    data_type: str = "uint16_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that ffff is returned
    assert "ffff" == value_hex


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint16_middle_of_the_road():
    """
    Test decimal to hexadecimal conversion. Here an arbitrary value of uint16_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up some value in the middle of the road of uint16_t.
    value: str = "3245"
    data_type: str = "uint16_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that 0cad is returned
    assert "0cad" == value_hex


def test_get_hex_by_type__little_endian_conversion_to_hexadecimal_uint16_middle_of_the_road():
    """
    Test decimal to hexadecimal conversion. Here an arbitrary value of uint16_t
    shall be converted to hexadecimal. This time little endian shall be returned, where the byte
    order is reversed.
    """
    # \arrange Set up some value in the middle of the road of uint16_t.
    value: str = "3245"
    data_type: str = "uint16_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, "<")
    # \assert expect that 0cad is returned
    assert "ad0c" == value_hex


@given(integers(min_value=0, max_value=65535))
def test_get_hex_by_type__hypothesis_testing_uint16(decimal: int):
    """
    Test decimal to hexadecimal conversion. Hypothesis testing for a given set of integers.
    Expect that the length of the returned string is 4 for datatype uint16_t.
    """
    # \arrange Set up inputs for hypothesis testing.
    value: str = str(decimal)
    data_type: str = "uint16_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that a string of length 4 is returned
    assert 4 == len(value_hex)


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint16_min_value():
    """
    Test decimal to hexadecimal conversion. Here the minimum of uint16_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up lower boundary of uint16_t.
    value: str = "0"
    data_type: str = "uint16_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that 0000 is returned
    assert "0000" == value_hex


@given(integers(min_value=-2147483648, max_value=2147483647))
def test_get_hex_by_type__hypothesis_testing_int32_dec_to_hex_to_dec(decimal: int):
    """
    Test decimal to hexadecimal conversion. Hypothesis testing for a given set of signed integers.
    Expect conversion to hexadecimal and back to decimal does not change the underlying decimal value of type
    int32_t.
    """
    # \arrange Set up inputs for hypothesis testing.
    value: str = str(decimal)
    data_type: str = "int32_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that reversing of the transformation results in the original value
    assert value == str(twos_comp_helper_function(int(value_hex, 16), 32))


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint32_max_value():
    """
    Test decimal to hexadecimal conversion. Here the maximum of uint32_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up upper boundary of uint32_t.
    value: str = "4294967295"
    data_type: str = "uint32_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that ffffffff is returned
    assert "ffffffff" == value_hex


@given(integers(min_value=0, max_value=4294967295))
def test_get_hex_by_type__hypothesis_testing_uint32(decimal: int):
    """
    Test decimal to hexadecimal conversion. Hypothesis testing for a given set of integers.
    Expect that the length of the returned string is 8 for datatype uint32_t.
    """
    # \arrange Set up inputs for hypothesis testing.
    value: str = str(decimal)
    data_type: str = "uint32_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that a string of length 8 is returned
    assert 8 == len(value_hex)


def test_get_hex_by_type__little_endian_conversion_to_hexadecimal_uint32_middle_of_the_road():
    """
    Test decimal to hexadecimal conversion. Here an arbitrary value of uint32_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up some value in the middle of the road of uint32_t.
    value: str = "4473289"
    data_type: str = "uint32_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, "<")
    # \assert expect that c9414400 is returned
    assert "c9414400" == value_hex


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint32_middle_of_the_road():
    """
    Test decimal to hexadecimal conversion. Here an arbitrary value of uint32_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up some value in the middle of the road of uint32_t.
    value: str = "4473289"
    data_type: str = "uint32_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that 004441c9 is returned
    assert "004441c9" == value_hex


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_uint32_min_value():
    """
    Test decimal to hexadecimal conversion. Here the minimum of uint32_t
    shall be converted to hexadecimal.
    """
    # \arrange Set up lower boundary of uint32_t.
    value: str = "0"
    data_type: str = "uint32_t"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that 00000000 is returned
    assert "00000000" == value_hex


def test_get_hex_by_type__big_endian_conversion_to_hexadecimal_float32():
    """
    Test decimal to hexadecimal conversion. Here some floating point value shall be converted.
    """
    # \arrange Set up an arbitrary floating point value.
    value: str = "5.0"
    data_type: str = "float32_T"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, ">")
    # \assert expect that 40a00000 is returned
    assert "40a00000" == value_hex


def test_get_hex_by_type__little_endian_conversion_to_hexadecimal_float32():
    """
    Test decimal to hexadecimal conversion. Here some floating point value shall be converted
    to little endian hexadecimal representation.
    """
    # \arrange Set up an arbitrary floating point value.
    value: str = "5.0"
    data_type: str = "float32_T"
    # \action Call decimal to hexadecimal conversion
    value_hex: str = ct_cds.get_hex_by_type(value, data_type, "<")
    # \assert expect that 0000a040 is returned
    assert "0000a040" == value_hex


def test_get_hex_by_type__raise_exception_due_to_fantasy_type():
    """
    Test decimal to hexadecimal conversion. Here some floating point value shall be converted
    to little endian hexadecimal representation.
    """
    # \arrange Set up a non defined type.
    value: str = "5.0"
    data_type: str = "I_Am_A_Crazy_Fantasy_Type_T"
    # \action Call decimal to hexadecimal conversion
    # \assert expect that function raises an error
    with pytest.raises(Exception) as exception_info:
        ct_cds.get_hex_by_type(value, data_type, ">")
    assert "Invalid Datatype." in str(exception_info.value)


def test_create_customer_data_stream_xml__create_an_example_data_stream(tmp_path, ct_fixture_setup_floating_point_2d_array_calibration,
    ct_fixture_setup_header_calibrations, ct_fixture_setup_5_fixed_point_cals,
    ct_fixture_setup_floating_point_array_calibration, ct_fixture_setup_generic_calibration_info):
    """
    Test data stream extraction based on an example dictionary without any customer specific values.
    """
    # \arrange Set up calibration dictionary and other inputs
    input_dict: Dict[str, ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations,
                                                **ct_fixture_setup_floating_point_2d_array_calibration,
                                                **ct_fixture_setup_5_fixed_point_cals, **ct_fixture_setup_floating_point_array_calibration
                                                }
    ct_fixture_setup_generic_calibration_info.customers = ["Customer_A"]
    sub_path = tmp_path/"Customer_A"
    sub_path.mkdir()
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name

    # \action Call data stream extraction
    ct_cds.create_customer_data_stream_xml(input_dict, sub_path, ct_fixture_setup_generic_calibration_info, "Customer_A", structure_name)

    # \assert expect that specific properties are set.
    cal_2d_1_array_row_major_order_be = "40a0000041a000004120000041c800004170000041f00000"
    cal_2d_1_array_row_major_order_le = "0000a0400000a041000020410000c841000070410000f041"
    cal_2d_2_array_row_major_order_be = "4248000042700000428c0000425c00004282000042960000"
    cal_2d_2_array_row_major_order_le = "000048420000704200008c4200005c420000824200009642"
    basic_data_types_le = "0000a04000002041000078410000ca41320000001e0028000a14"
    basic_data_types_be = "140a0028001e000000324178000041ca000040a0000041200000"
    file_path = os.path.join(tmp_path, "Customer_A", ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) + "_data_stream.xml")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        assert "<<<INSERT_BIG_ENDIAN_STREAM_HERE>>>" not in content
        assert "<<<INSERT_LITTLE_ENDIAN_STREAM_HERE>>>" not in content
        assert "<CALIB_DATA_BE>"+basic_data_types_be+cal_2d_2_array_row_major_order_be+\
            cal_2d_1_array_row_major_order_be+"0301007800030050000000fa</CALIB_DATA_BE>" in content
        assert "<CALIB_DATA_LE>fa0000005000030078000103"+cal_2d_1_array_row_major_order_le+cal_2d_2_array_row_major_order_le+ basic_data_types_le+\
            "</CALIB_DATA_LE>" in content
