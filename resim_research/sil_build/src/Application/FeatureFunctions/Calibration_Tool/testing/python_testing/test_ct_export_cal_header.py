"""This file contains test implementations for the source file ct_export_cal_header."""
from typing import Dict
import os

import python_src.ct_export_cal_header as ct_ech
import python_src.ct_shared_resources as ct_sr

helper_expected_file_content_1 = """\
#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t cal_2; /**<None*/
   uint8_t cal_1; /**<None*/
   uint16_t cal_4; /**<None*/
   uint16_t cal_3; /**<None*/
   uint32_t cal_5; /**<None*/
   float32_T cal_float_array_2[CF_CAL_FLOAT_ARRAY_2_ARRAY_SIZE_DIM0]; /**<None*/
   float32_T cal_float_array_1[CF_CAL_FLOAT_ARRAY_1_ARRAY_SIZE_DIM0]; /**<None*/
   float32_T cal_float_array_2d_2[CF_CAL_FLOAT_ARRAY_2D_2_ARRAY_SIZE_DIM0][CF_CAL_FLOAT_ARRAY_2D_2_ARRAY_SIZE_DIM1]; /**<None*/
   float32_T cal_float_array_2d_1[CF_CAL_FLOAT_ARRAY_2D_1_ARRAY_SIZE_DIM0][CF_CAL_FLOAT_ARRAY_2D_1_ARRAY_SIZE_DIM1]; /**<None*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Cf_Core_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T cal_float_array_2d_1[CF_CAL_FLOAT_ARRAY_2D_1_ARRAY_SIZE_DIM0][CF_CAL_FLOAT_ARRAY_2D_1_ARRAY_SIZE_DIM1]; /**<None*/
   float32_T cal_float_array_2d_2[CF_CAL_FLOAT_ARRAY_2D_2_ARRAY_SIZE_DIM0][CF_CAL_FLOAT_ARRAY_2D_2_ARRAY_SIZE_DIM1]; /**<None*/
   float32_T cal_float_array_1[CF_CAL_FLOAT_ARRAY_1_ARRAY_SIZE_DIM0]; /**<None*/
   float32_T cal_float_array_2[CF_CAL_FLOAT_ARRAY_2_ARRAY_SIZE_DIM0]; /**<None*/
   uint32_t cal_5; /**<None*/
   uint16_t cal_3; /**<None*/
   uint16_t cal_4; /**<None*/
   uint8_t cal_1; /**<None*/
   uint8_t cal_2; /**<None*/
} Cf_Core_Calibration_T;
#endif /* CT_BIG_ENDIAN */"""

def test_calibration_header_file__create_an_example_header_t_file(tmp_path, ct_fixture_setup_header_calibrations,
                                                                ct_fixture_setup_floating_point_2d_array_calibration,
                                                                ct_fixture_setup_5_fixed_point_cals,
                                                                ct_fixture_setup_floating_point_array_calibration,
                                                                ct_fixture_setup_generic_calibration_info):
    """
    Test header file extraction based on an example dictionary without any customer specific values.
    """
    # \arrange Set up calibration dictionary and other inputs
    input_dict: Dict[str, ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations,
                                                **ct_fixture_setup_floating_point_2d_array_calibration,
                                                **ct_fixture_setup_5_fixed_point_cals, **ct_fixture_setup_floating_point_array_calibration}
    input_dict = ct_sr.sort_dict_from_big_to_small_types(input_dict)
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action Call header file creation
    ct_ech.create_header(input_dict, tmp_path,
                         ct_fixture_setup_generic_calibration_info, structure_name)
    # \assert expect that specific properties are set.
    file_path = os.path.join(tmp_path, ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) + "_t.h")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        assert f"#define {ct_fixture_setup_generic_calibration_info.get_cal_size_macro_name(structure_name)} (86u)" in content
        assert "#define CF_CAL_FLOAT_ARRAY_1_ARRAY_SIZE_DIM0 (2u)" in content
        assert helper_expected_file_content_1 in content



helper_expected_file_content_2 = """\
/*===========================================================================*\\
* Global function declarations
\\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
/**
 * @brief Reverses arrays with variable length of 1, 2 or 4 bytes. Dependent on whether arrays are given.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
void Cf_Core_Cal_Reverse_Array_Cf_Cal(Cf_Core_Calibration_T* cal_dst);
#endif /*CT_BIG_ENDIAN*/

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * @brief Prints values of calibrations.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
void Cf_Core_Cal_Print(FILE* c_file_ptr, const Cf_Core_Calibration_T* p_cals);

#endif /*CT_ACTIVATE_CAL_PRINT*/

/**
 * @brief This function updates all calibrations of the component to their respective defaults given by the customer specific xml sheet.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
void Cf_Core_Cal_Update_Defaults(Cf_Core_Calibration_T* cal_dst);

/**
 * @brief This function checks whether the initial values set as calibrations of component Cf_Core_Calibration_T,
 * have the correct values indicated by the core xml as well as the customer specific calibration xml file.
 *
 * @return Enums state indicating whether initial calibration value check was successful. 
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
enum Cf_Core_Cal_Asil_Status Cf_Core_Cal_Evaluate_Asil_Cal_Status(const Cf_Core_Calibration_T* p_cals);
"""

def test_calibration_header_file__create_an_example_header_file(tmp_path, ct_fixture_setup_header_calibrations,
                                                                ct_fixture_setup_floating_point_2d_array_calibration,
                                                                ct_fixture_setup_5_fixed_point_cals,
                                                                ct_fixture_setup_floating_point_array_calibration,
                                                                ct_fixture_setup_generic_calibration_info):
    """
    Test header file extraction based on an example dictionary without any customer specific values.
    """
    # \arrange Set up calibration dictionary and other inputs
    input_dict: Dict[str, ct_sr.Calibration] = {**ct_fixture_setup_header_calibrations,
                                                **ct_fixture_setup_floating_point_2d_array_calibration,
                                                **ct_fixture_setup_5_fixed_point_cals, **ct_fixture_setup_floating_point_array_calibration}
    input_dict = ct_sr.sort_dict_from_big_to_small_types(input_dict)
    structure_name = ct_fixture_setup_generic_calibration_info.core_structure_name
    # \action Call header file creation
    ct_ech.create_header(input_dict, tmp_path,
                         ct_fixture_setup_generic_calibration_info, structure_name)
    # \assert expect that specific properties are set.
    file_path = os.path.join(tmp_path, ct_fixture_setup_generic_calibration_info.get_deployment_file_name(structure_name) + ".h")
    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()
        assert helper_expected_file_content_2 in content
        assert "#define CF_MIN_CAL_FLOAT_ARRAY_1 ((float32_T)(0.0f))" in content
        assert "#define CF_MAX_CAL_5 ((uint32_t)(100u))" in content
