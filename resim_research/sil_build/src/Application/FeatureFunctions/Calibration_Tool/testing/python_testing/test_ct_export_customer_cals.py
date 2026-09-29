"""This file contains test implementations for the source file ct_export_customer_cals."""
from typing import Dict
import os

import python_src.ct_export_customer_cals as ct_ecc
import python_src.ct_shared_resources as ct_sr

expected_cal_update_content = """\
   /**<cal_float_array_1*/ {(float32_T)0.0,(float32_T)100.0},
   /**<cal_float_array_2*/ {(float32_T)-12.5,(float32_T)-25.75},
"""

expected_order_big_to_small_datatypes = """\
   {
   /**<Section_Size*/ (uint32_t)250,
   /**<version*/ (uint16_t)80,
   /**<Section_Compatibility*/ (uint16_t)3,
   /**<Cal_Chk_Sum*/ (uint16_t)120,
   /**<Chk_sum_Version*/ (uint8_t)1,
   /**<Cal_Type*/ (uint8_t)3
   },
   /* Component Calibration */
   /**<cal_float_array_2d_1*/ {{(float32_T)5.0f /**< 5.0 m/s | 18.0 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */,(float32_T)15.0f /**< 15.0 m/s | 54.0 km/h */},{(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */,(float32_T)25.0f /**< 25.0 m/s | 90.0 km/h */,(float32_T)30.0f /**< 30.0 m/s | 108.0 km/h */}},
   /**<cal_float_array_2d_2*/ {{(float32_T)50.0 /**< 50.0 m */,(float32_T)55.0 /**< 55.0 m */},{(float32_T)60.0 /**< 60.0 m */,(float32_T)65.0 /**< 65.0 m */},{(float32_T)70.0 /**< 70.0 m */,(float32_T)75.0 /**< 75.0 m */}},
   /**<cal_float_array_1*/ {(float32_T)0.0,(float32_T)100.0},
   /**<cal_float_array_2*/ {(float32_T)-12.5,(float32_T)-25.75},
   /**<cal_5*/ (uint32_t) 50,
   /**<cal_3*/ (uint16_t) 30,
   /**<cal_4*/ (uint16_t) 40,
   /**<cal_1*/ (uint8_t) 10,
   /**<cal_2*/ (uint8_t) 20
"""

expected_row_major_order_first_cal = """\
   /**<cal_float_array_2d_1*/ {{(float32_T)5.0f /**< 5.0 m/s | 18.0 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */,(float32_T)15.0f /**< 15.0 m/s | 54.0 km/h */},{(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */,(float32_T)25.0f /**< 25.0 m/s | 90.0 km/h */,(float32_T)30.0f /**< 30.0 m/s | 108.0 km/h */}},
   /**<cal_float_array_2d_2*/ {{(float32_T)50.0 /**< 50.0 m */,(float32_T)55.0 /**< 55.0 m */},{(float32_T)60.0 /**< 60.0 m */,(float32_T)65.0 /**< 65.0 m */},{(float32_T)70.0 /**< 70.0 m */,(float32_T)75.0 /**< 75.0 m */}},
   """

expected_row_major_order_second_cal = """\
   /**<cal_float_array_2d_2*/ {{(float32_T)50.0 /**< 50.0 m */,(float32_T)55.0 /**< 55.0 m */},{(float32_T)60.0 /**< 60.0 m */,(float32_T)65.0 /**< 65.0 m */},{(float32_T)70.0 /**< 70.0 m */,(float32_T)75.0 /**< 75.0 m */}},
   /**<cal_float_array_2d_1*/ {{(float32_T)5.0f /**< 5.0 m/s | 18.0 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */,(float32_T)15.0f /**< 15.0 m/s | 54.0 km/h */},{(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */,(float32_T)25.0f /**< 25.0 m/s | 90.0 km/h */,(float32_T)30.0f /**< 30.0 m/s | 108.0 km/h */}},
   """

def test_create_customer_specific_c_file__create_example_c_file(tmp_path, ct_fixture_setup_floating_point_2d_array_calibration,
                                                                ct_fixture_setup_header_calibrations,
                                                                ct_fixture_setup_5_fixed_point_cals,
                                                                ct_fixture_setup_floating_point_array_calibration,
                                                                ct_fixture_setup_generic_calibration_info):
    """
    Test customer file extraction based on an example dictionary without any customer specific values.
    """
    # \arrange Set up calibration dictionary and modify it, such that customer specific values are given.
    input_dict: Dict[str, ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations, **ct_fixture_setup_5_fixed_point_cals,
      **ct_fixture_setup_floating_point_2d_array_calibration, **ct_fixture_setup_floating_point_array_calibration}
    input_dict = ct_sr.sort_dict_from_big_to_small_types(input_dict)
    input_dict["cal_float_array_1"].customer_specific_values = {"Customer_A": "[0.0 100.0]"}
    input_dict["cal_float_array_2"].customer_specific_values = {"Customer_A": "[-12.5 -25.75]"}
    ct_fixture_setup_generic_calibration_info.customers = ["Customer_A"]
    sub_path = tmp_path/"Customer_A"
    sub_path.mkdir()
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action Call customer specific file creation
    ct_ecc.create_customer_specific_c_file(input_dict, sub_path, ct_fixture_setup_generic_calibration_info, "Customer_A", structure_name)
    # \assert expect that specific properties are set and that 2d array format is fulfilled in row-major-order
    file_path = os.path.join(tmp_path, "Customer_A", ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) + ".c")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        assert expected_row_major_order_first_cal in content
        assert expected_row_major_order_second_cal in content
        assert expected_cal_update_content in content
        assert expected_order_big_to_small_datatypes in content
