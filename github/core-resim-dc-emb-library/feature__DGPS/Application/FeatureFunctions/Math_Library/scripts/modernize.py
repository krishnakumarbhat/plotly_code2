# Recursivly edits all *.hpp and *.h file from within the directory its been
# started from.
# Changes all calls to items and macros to new names
# Removes all includes to legacy headers
# adds includes for new headers as needed

import os
import pathlib

# List of headers to no longer include
legacy_headers = [
    'Angle_Range.h',
    'Assert_Macros.h',
    'Basic_Macros.h',
    'Basic_Math.h',
    'Basic_Math_Factory.h',
    'Basic_Math_Structs.h',
    'Basic_Saturated_Math.h',
    'Compiler_Warning_Macros.h',
    'fastMath.h',
    'Geometric_2d_Factory.h',
    'Geometric_2d_Functions.h',
    'Geometric_2d_Structs.h',
    'line_hesse.h',
    'line_hesse_type.h',
    'LookupTable.h',
    'Math_Infinity.h',
    'math_infinity_silent.h',
    'Math_Macros.h',
    'Math_Selector.h',
    'matrix.h',
    'matrix_2x2_type.h',
    'matrix_3x3_type.h',
    'Moving_Average.h',
    'Runtime_Parameters.h',
    'Shared_Toolbox_All.h',
    'Shared_Toolbox_All_Structs.h',
    'shared_toolbox_content.h',
    'shared_toolbox_serial_buffer_t.h',
    'shared_toolbox_serialization_error_t.h',
    'shared_toolbox_version.h',
    'Shared_Toolbox_Version_Check.h',
    'SHARED_TOOLBOX_VERSION_CONTENT.h',
    'SHARED_TOOLBOX_VERSION_DATE.h',
    'Sieve.h',
    'st_angle.h',
    'st_angle_range.h',
    'st_angle_range_fuse_state_t.h',
    'st_angle_range_t.h',
    'st_angle_t.h',
    'st_bool.h',
    'st_checked_rounding.h',
    'st_checksum.h',
    'st_compiler_warning.h',
    'st_exp.h',
    'st_fast_math_table_macros.h',
    'st_float_range_t.h',
    'st_int_range_t.h',
    'st_interval.h',
    'st_line.h',
    'st_line_hesse.h',
    'st_line_hesse_t.h',
    'st_line_parameter.h',
    'st_line_parameter_t.h',
    'st_line_segment.h',
    'st_line_segment_t.h',
    'st_lookup_table_2d.h',
    'st_macros.h',
    'st_math.h',
    'st_math_infinity.h',
    'st_math_infinity_silent.h',
    'st_matrix.h',
    'st_matrix_2x2_t.h',
    'st_matrix_3x3_t.h',
    'st_max_sieve_t.h',
    'st_min_max_sieve_t.h',
    'st_min_sieve_t.h',
    'st_moving_average.h',
    'st_moving_average_filter_instance_t.h',
    'st_overlapping_angle_range_t.h',
    'st_polygon.h',
    'st_runtime_parameter_boolean_t.h',
    'st_runtime_parameter_error_t.h',
    'st_runtime_parameter_float32_t.h',
    'st_runtime_parameter_int16_t.h',
    'st_runtime_parameter_int32_t.h',
    'st_runtime_parameter_int8_t.h',
    'st_runtime_parameter_legal_state_t.h',
    'st_runtime_parameter_uint16_t.h',
    'st_runtime_parameter_uint32_t.h',
    'st_runtime_parameter_uint8_t.h',
    'st_runtime_parameters.h',
    'st_saturated_math.h',
    'st_set_trigonometric_table.h',
    'st_sieve.h',
    'st_trigonometry.h',
    'st_unit_conversion.h',
    'st_vector_2d.h',
    'st_vector_2d_angle.h',
    'st_vector_2d_t.h',
    'st_version_number_check.h',
    'st_version_number_t.h',
    'Vector_2d.h',
    'Vector_2d_Algebra.h',
    'Vector_2d_Factory.h',
    'VERSION_NUMBER_T.h'
]

# List of headers and the items they define
item_headers = [
    ['ml_angle.h',
        [
            'Create_Angle',
            'Normalize_Angle_Struct',
            'Normalize_Angle',
            'Angle_Diff',
            'Angle_Mean']],
    ['ml_bool.h',
        [
            'Is_True',
            'Is_False',
            'TRUE',
            'FALSE']],
    ['ml_angle_range.h',
        [
            'Is_Angle_Contained_In_Angle_Range',
            'Create_Angle_Range',
            'Create_Angle_Range_From_Float',
            'Create_Angle_Range_Reference_Inside',
            'Create_Angle_Range_Reference_Outside',
            'Angle_Range_Fuse',
            'Does_Angle_Range_Overlap_Angle_Range',
            'Initialize_Overlapping_Angle_Range',
            'Get_Overlapping_Angle_Range',
            'Get_Angle_Range_Width_Float',
            'Get_Angle_Range_Width_Angle',
            'Swap_Angle_Range_Start_End',
            'Get_Angle_Range_Start_Angle',
            'Get_Angle_Range_End_Angle',
            'Get_Angle_Range_Start',
            'Get_Angle_Range_End',
            'Get_Angle_Range_Center']],
    ['ml_checked_rounding.h',
        [
            'Roundf_Checked_Uint8',
            'Roundf_Checked_Int8',
            'Roundf_Checked_Uint16',
            'Roundf_Checked_Int16',
            'Roundf_Checked_Uint32',
            'Roundf_Checked_Int32']],
    ['ml_checksum.h',
        [
            'Calc_Checksum_U8',
            'Calc_Checksum_U16',
            'Calc_Checksum_U32']],
    ['ml_compiler_warning.h',
        [
            'As_Compiler_Warning',
            'Msvs_Disable_Warning',
            'Msvs_Enable_Warning']],
    ['ml_exp.h',
        [
            'Compute_Exp_Table',
            'Serialize_Exp_Table',
            'Deserialize_Exp_Table']],
    ['ml_interval.h',
        [
            'Create_Float_Range',
            'Create_Int_Range',
            'Init_Float_Range',
            'Init_Int_Range',
            'Is_Float_Contained_In_Float_Range',
            'Is_Int_Contained_In_Float_Range',
            'Is_Float_Contained_In_Int_Range',
            'Is_Int_Contained_In_Int_Range',
            'Does_Int_Range_Overlap_Int_Range',
            'Does_Float_Range_Overlap_Int_Range',
            'Does_Float_Range_Overlap_Float_Range',
            'Is_Int_Interval_Subset_Of_Int_Interval',
            'Is_Float_Interval_Subset_Of_Float_Interval',
            'Extend_Float_Range',
            'Extend_Int_Range',
            'Enforce_Range',
            'Enforce_Nonzero',
            'Is_Float_Within_Tolerance',
            'Is_Int_Within_Tolerance']],
    ['ml_line.h',
        [
            'Get_Slope_Of_Line',
            'Get_Y_Value_From_Line_Defined_By_2_Points',
            'Get_Y_Value_From_Line_By_Coordinates']],
    ['ml_line_hesse.h',
        [
            'Line_Hesse_Create',
            'Line_Hesse_Create_Using_Point_And_Unit_Vector',
            'Line_Hesse_Create_Using_Point_And_Normal_Vector',
            'Line_Hesse_Create_Using_Two_Points',
            'Line_Hesse_Create_Using_Line_Segment',
            'Line_Hesse_Create_Using_Line_Parameter',
            'Line_Hesse_Get_Distance_Of_Point',
            'Line_Hesse_Update_Line_Normal_Direction',
            'Line_Hesse_Get_Side_of_Point',
            'Line_Hesse_Create_Using_Azimuth_And_Point',
            'Line_Hesse_Get_Norm_Vector',
            'Line_Hesse_Get_Unit_Vector',
            'Line_Hesse_Get_Distance_To_Origin']],
    ['ml_line_parameter.h',
        [
            'Create_Line_Parameter_Form',
            'Get_Y_Value_From_Line']],
    ['ml_line_segment.h',
        ['Create_Line_Segment_Fom_Points']],
    ['ml_lookup_table_2d.h',
        [
            'Get_Value_From_2d_Lookup_Table',
            'Interpolate_To_Zero']],
    ['ml_macros.h',
        [
            'Swap',
            'NULL']],
    ['ml_math_infinity_silent.h',
        ['AS_TOOLBOX_INFINITY']],
    ['ml_math.h',
        [
            'Fast_Sqrt',
            'Sign',
            'PI',
            'Abs',
            'Max',
            'Min',
            'EPSILON',
            'THRESHOLD_IS_ZERO',
            'Quotient_Ceiled',
            'Is_Not_Nan']],
    ['ml_matrix.h',
        [
            'Matrix_2x2_Create_Identity_Matrix',
            'Matrix_2x2_Create_Zero_Matrix',
            'Matrix_2x2_Mul_Matrix_2x2',
            'Matrix_2x2_Add_Matrix_2x2',
            'Matrix_2x2_Mul_Scalar',
            'Matrix_3x3_Create_Identity_Matrix',
            'Matrix_3x3_Create_Zero_Matrix',
            'Matrix_3x3_Mul_Matrix_3x3',
            'Matrix_3x3_Add_Matrix_3x3',
            'Matrix_3x3_Mul_Scalar',
            'Matrix_2x2_Mul_Vector_2d',
            'Matrix_2x2_Determinant',
            'Matrix_3x3_Determinant',
            'Matrix_2x2_Inverse_Given_Determinant',
            'Matrix_2x2_Inverse',
            'Matrix_2x2_Transpose',
            'Matrix_3x3_Transpose']],
    ['ml_moving_average.h',
        [
            'Moving_Average_Init',
            'Moving_Average_Reset',
            'Moving_Average_Reset_Ringbuffer',
            'Moving_Average_Run']],
    ['ml_polygon.h',
        [
            'Is_Point_In_Polygon',
            'Is_Point_In_Convex_Polygon_Ray_Casting_Method']],
    ['ml_runtime_parameters.h',
        [
            'Generate_Runtime_Value_Boolean',
            'Generate_Runtime_Value_Uint8',
            'Generate_Runtime_Value_Uint16',
            'Generate_Runtime_Value_Uint32',
            'Generate_Runtime_Value_Int8',
            'Generate_Runtime_Value_Int16',
            'Generate_Runtime_Value_Int32',
            'Generate_Runtime_Value_Float32',
            'Init_Runtime_Param_Error',
            'Map_Cal_Settings_To_Runtime_Parameter_Legal_State',
            'Test_Runtime_Value_Uint8',
            'Test_Runtime_Value_Int8',
            'Test_Runtime_Value_Uint16',
            'Test_Runtime_Value_Int16',
            'Test_Runtime_Value_Uint32',
            'Test_Runtime_Value_Int32',
            'Test_Runtime_Value_Float32',
            'Set_Runtime_Parameter_Extern_Boolean',
            'Set_Runtime_Parameter_Extern_Uint8',
            'Set_Runtime_Parameter_Extern_Uint16',
            'Set_Runtime_Parameter_Extern_Uint32',
            'Set_Runtime_Parameter_Extern_Int8',
            'Set_Runtime_Parameter_Extern_Int16',
            'Set_Runtime_Parameter_Extern_Int32',
            'Set_Runtime_Parameter_Extern_Float32']],
    ['ml_saturated_math.h',
        [
            'Sat_Inc_Uint8',
            'Sat_Inc_Int8',
            'Sat_Inc_Uint16',
            'Sat_Inc_Int16',
            'Sat_Inc_Uint32',
            'Sat_Inc_Int32',
            'Sat_Dec_Uint8',
            'Sat_Dec_Int8',
            'Sat_Dec_Uint16',
            'Sat_Dec_Int16',
            'Sat_Dec_Uint32',
            'Sat_Dec_Int32',
            'Sat_Add_Uint8',
            'Sat_Add_Int8',
            'Sat_Add_Uint16',
            'Sat_Add_Int16',
            'Sat_Add_Uint32',
            'Sat_Add_Int32',
            'Sat_Sub_Uint8',
            'Sat_Sub_Int8',
            'Sat_Sub_Uint16',
            'Sat_Sub_Int16',
            'Sat_Sub_Uint32',
            'Sat_Sub_Int32']],
    ['ml_set_trigonometric_table.h',
        ['Set_Trig_Table_By_Checksum']],
    ['ml_sieve.h',
        [
            'Init_Max_Sieve',
            'Max_Sieve',
            'Init_Max_Sieve_Set',
            'Max_Sieve_Set',
            'Get_Max_Sieve_Content',
            'Init_Min_Sieve',
            'Min_Sieve',
            'Init_Min_Sieve_Set',
            'Min_Sieve_Set',
            'Get_Min_Sieve_Content',
            'Get_Max_From_Max_Sieve_Set',
            'Get_Max_Sieve_Set_Content_At_Index',
            'Get_Min_Sieve_Set_Content_At_Index',
            'Get_Min_From_Min_Sieve_Set',
            'Init_Min_Max_Sieve',
            'Min_Max_Sieve',
            'Init_Min_Max_Sieve_Set',
            'Min_Max_Sieve_Set',
            'Get_Min_Max_Sieve_Min_Content',
            'Get_Min_Max_Sieve_Max_Content',
            'Is_Sieved_Value_Valid']],
    ['ml_trigonometry.h',
        [
            'Compute_Trig_Tables',
            'Serialize_Trig_Table',
            'Serialize_Trig_Table_Checksum',
            'Deserialize_Trig_Table',
            'Fast_Cos',
            'Fast_Sin',
            'Fast_Acos',
            'Fast_Asin',
            'Fast_Tan',
            'Fast_Atan',
            'Fast_Atan2',
            'Fast_Hypot']],
    ['ml_vector_2d.h',
        [
            'Create_2d_Vector_Coordinates',
            'Create_2d_Vector_Origin',
            'Create_2d_Vector_X_Normal',
            'Create_2d_Vector_Y_Normal',
            'Vector_2d_Alg_Multiply_Scalar',
            'Vector_2d_Alg_Add',
            'Vector_2d_Alg_Middle',
            'Vector_2d_Alg_Scalar_Product',
            'Vector_2d_Alg_Distance',
            'Vector_2d_Alg_Rotate_Half_Pi',
            'Vector_2d_Alg_Normalize_Vector',
            'Vector_2d_Alg_Abs',
            'Vector_2d_Alg_Abs_Squared',
            'Vector_2d_Alg_Diff',
            'Vector_2d_Alg_Abs_Component_Wise',
            'Vector_2d_Alg_Sqrt_Component_Wise',
            'Vector_2d_Alg_Calculate_Cos_Between_Two_Vec',
            'Vector_2d_Alg_Limit_Vector']],
    ['ml_vector_2d_angle.h',
        [
            'Vector_2d_Alg_Rotate',
            'Vector_2d_Alg_Angle_From_Vector',
            'Vector_2d_Alg_Project_On_Angle',
            'Vector_2d_Alg_Project_On_Rotated_X_Axis',
            'Vector_2d_Alg_Rotate_Negative',
            'Vector_2d_Alg_Scalar_Product_With_Angle',
            'Vector_2d_Alg_Angle_To_Vector',
            'Vector_2d_Alg_Angle_To_Perpendicular_Vector']],
    ['ml_version.h',
        [
            'Get_Ml_Math_Library_Version',
            'Ml_Math_Library_Version_Equals',
            'Ml_Math_Library_Version_Insufficient']],
    ['ml_version_number_check.h',
        [
            'Ml_Compute_Version_Integer_From_Date',
            'Ml_Compute_Version_Integer_From_Name',
            'Ml_Version_Insufficient_Check',
            'Ml_Version_Equals_Check']],
    ['ml_angle_range_fuse_state_t.h',
        ['Angle_Range_Fuse_State_T']],
    ['ml_angle_range_t.h',
        ['Angle_Range_T']],
    ['ml_angle_t.h',
        ['Angle_T']],
    ['ml_float_range_t.h',
        ['Float_Range_T']],
    ['ml_int_range_t.h',
        ['Int_Range_T']],
    ['ml_line_hesse_t.h',
        ['Line_Hesse_T']],
    ['ml_line_parameter_t.h',
        ['Line_Parameter_T']],
    ['ml_line_segment_t.h',
        ['Line_Segment_T']],
    ['ml_matrix_2x2_t.h',
        ['Matrix_2X2_T']],
    ['ml_matrix_3x3_t.h',
        ['Matrix_3X3_T']],
    ['ml_max_sieve_t.h',
        ['Max_Sieve_T']],
    ['ml_min_max_sieve_t.h',
        ['Min_Max_Sieve_T']],
    ['ml_min_sieve_t.h',
        ['Min_Sieve_T']],
    ['ml_moving_average_filter_instance_t.h',
        ['Moving_Average_Filter_Instance_T']],
    ['ml_overlapping_angle_range_t.h',
        ['Overlapping_Angle_Range_T']],
    ['ml_runtime_parameter_boolean_t.h',
        ['Runtime_Parameter_Boolean_T']],
    ['ml_runtime_parameter_error_t.h',
        ['Runtime_Parameter_Error_T']],
    ['ml_runtime_parameter_float32_t.h',
        ['Runtime_Parameter_Float32_T']],
    ['ml_runtime_parameter_int16_t.h',
        ['Runtime_Parameter_Int16_T']],
    ['ml_runtime_parameter_int32_t.h',
        ['Runtime_Parameter_Int32_T']],
    ['ml_runtime_parameter_int8_t.h',
        ['Runtime_Parameter_Int8_T']],
    ['ml_runtime_parameter_legal_state_t.h',
        ['Runtime_Parameter_Legal_State_T']],
    ['ml_runtime_parameter_uint16_t.h',
        ['Runtime_Parameter_Uint16_T']],
    ['ml_runtime_parameter_uint32_t.h',
        ['Runtime_Parameter_Uint32_T']],
    ['ml_runtime_parameter_uint8_t.h',
        ['Runtime_Parameter_Uint8_T']],
    ['ml_serial_buffer_t.h',
        ['Serial_Buffer_T']],
    ['ml_serialization_error_t.h',
        ['Serialization_Error_T']],
    ['ml_vector_2d_t.h',
        ['Vector_2d_T']],
    ['ml_version_number_t.h',
        ['Version_Number_T']],
    ['ml_serial_buffer_t.h',
        ['Shared_Toolbox_Serial_Buffer_T']]
]


# Deprecated item names and their new names
replacements = [
    ['ANGLE_RANGE_T', 'Angle_Range_T'],
    ['OVERLAPPING_ANGLE_RANGE_T', 'Overlapping_Angle_Range_T'],
    ['SIGN(', 'Sign('],
    ['ABS(', 'Abs('],
    ['FAST_ABSF(', 'Abs('],
    ['fast_absf(', 'Abs('],
    ['FAST_ABS(', 'Abs('],
    ['fast_abs(', 'Abs('],
    ['SWAP(', 'Swap('],
    ['IS_TRUE', 'Is_True'],
    ['IS_FALSE', 'Is_False'],
    ['MAX(', 'Max('],
    ['max(', 'Max('],
    ['MIN(', 'Min('],
    ['min(', 'Min('],
    ['ForceRange', 'Enforce_Range'],
    ['NormalizeAngleStruct', 'Normalize_Angle_Struct'],
    ['NormalizeAngle', 'Normalize_Angle'],
    ['Angle_diff', 'Angle_Diff'],
    ['AS_ROUNDF', 'Ml_Roundf'],
    ['As_Roundf', 'Ml_Roundf'],
    ['MSVS_DISABLE_WARNING', 'Msvs_Disable_Warning'],
    ['MSVS_ENABLE_WARNING', 'Msvs_Enable_Warning'],
    ['AS_COMPILER_WARNING', 'As_Compiler_Warning'],
    ['Create_Line_Segment(', 'Create_Line_Segment_Fom_Points('],
    ['Is_Point_In_Polygon_Ray_Casting_Method',
        'Is_Point_In_Convex_Polygon_Ray_Casting_Method'],
    ['GetValueFrom2dLookuptable', 'Get_Value_From_2d_Lookup_Table'],
    ['QUOTIENT_CEILED', 'Quotient_Ceiled'],
    ['FAST_SQRT', 'Fast_Sqrt'],
    ['fast_sqrt', 'Fast_Sqrt'],
    ['FAST_HYPOT', 'Fast_Hypot'],
    ['fast_hypot', 'Fast_Hypot'],
    ['FAST_COS', 'Fast_Cos'],
    ['fast_cos', 'Fast_Cos'],
    ['FAST_ACOS', 'Fast_Acos'],
    ['fast_acos', 'Fast_Acos'],
    ['FAST_SIN', 'Fast_Sin'],
    ['fast_sin', 'Fast_Sin'],
    ['FAST_ASIN', 'Fast_Asin'],
    ['fast_asin', 'Fast_Asin'],
    ['FAST_TAN', 'Fast_Tan'],
    ['fast_tan', 'Fast_Tan'],
    ['FAST_ATAN', 'Fast_Atan'],
    ['fast_atan', 'Fast_Atan'],
    ['FAST_ATAN2', 'Fast_Atan2'],
    ['fast_atan2', 'Fast_Atan2'],
    ['FAST_EXP', 'Fast_Exp'],
    ['fast_exp', 'Fast_Exp'],
    ['generate_runtime_value_boolean_T', 'Generate_Runtime_Value_Boolean'],
    ['generate_runtime_value_uint8_T', 'Generate_Runtime_Value_Uint8'],
    ['generate_runtime_value_uint16_T', 'Generate_Runtime_Value_Uint16'],
    ['generate_runtime_value_uint32_T', 'Generate_Runtime_Value_Uint32'],
    ['generate_runtime_value_int8_T', 'Generate_Runtime_Value_Int8'],
    ['generate_runtime_value_int16_T', 'Generate_Runtime_Value_Int16'],
    ['generate_runtime_value_int32_T', 'Generate_Runtime_Value_Int32'],
    ['generate_runtime_value_float32_T', 'Generate_Runtime_Value_Float32'],
    ['init_runtime_param_error', 'Init_Runtime_Param_Error'],
    ['map_cal_settings_to_runtime_parameter_legal_state',
        'Map_Cal_Settings_To_Runtime_Parameter_Legal_State'],
    ['test_runtime_value_uint8_T', 'Test_Runtime_Value_Uint8'],
    ['test_runtime_value_uint16_T', 'Test_Runtime_Value_Uint16'],
    ['test_runtime_value_uint32_T', 'Test_Runtime_Value_Uint32'],
    ['test_runtime_value_int8_T', 'Test_Runtime_Value_Int8'],
    ['test_runtime_value_int16_T', 'Test_Runtime_Value_Int16'],
    ['test_runtime_value_int32_T', 'Test_Runtime_Value_Int32'],
    ['test_runtime_value_float32_T', 'Test_Runtime_Value_Float32'],
    ['set_runtime_parameter_extern_boolean_T',
        'Set_Runtime_Parameter_Extern_Boolean'],
    ['set_runtime_parameter_extern_uint8_T',
        'Set_Runtime_Parameter_Extern_Uint8'],
    ['set_runtime_parameter_extern_uint16_T',
        'Set_Runtime_Parameter_Extern_Uint16'],
    ['set_runtime_parameter_extern_uint32_T',
        'Set_Runtime_Parameter_Extern_Uint32'],
    ['set_runtime_parameter_extern_int8_T',
        'Set_Runtime_Parameter_Extern_int8'],
    ['set_runtime_parameter_extern_int16_T',
        'Set_Runtime_Parameter_Extern_int16'],
    ['set_runtime_parameter_extern_int32_T',
        'Set_Runtime_Parameter_Extern_int32'],
    ['set_runtime_parameter_extern_float32_T',
        'Set_Runtime_Parameter_Extern_Float32'],
    ['RUNTIME_PARAMETER_BOOLEAN_T', 'Runtime_Parameter_Boolean_T'],
    ['RUNTIME_PARAMETER_ERROR_T', 'Runtime_Parameter_Error_T'],
    ['RUNTIME_PARAMETER_FLOAT32_T', 'Runtime_Parameter_Float32_T'],
    ['RUNTIME_PARAMETER_INT8_T', 'Runtime_Parameter_Int8_T'],
    ['RUNTIME_PARAMETER_INT16_T', 'Runtime_Parameter_Int16_T'],
    ['RUNTIME_PARAMETER_INT32_T', 'Runtime_Parameter_Int32_T'],
    ['RUNTIME_PARAMETER_UINT8_T', 'Runtime_Parameter_Uint8_T'],
    ['RUNTIME_PARAMETER_UINT16_T', 'Runtime_Parameter_Uint16_T'],
    ['RUNTIME_PARAMETER_UINT32_T', 'Runtime_Parameter_Uint32_T'],
    ['RUNTIME_PARAMETER_LEGAL_STATE_T',
        'Runtime_Parameter_Legal_State_T'],
    ['SHARED_TOOLBOX_COMPUTE_VERSION_INTEGER_FROM_DATE',
        'Ml_Compute_Version_Integer_From_Date'],
    ['SHARED_TOOLBOX_COMPUTE_VERSION_INTEGER_FROM_NAME',
        'Ml_Compute_Version_Integer_From_Name'],
    ['SHARED_TOOLBOX_VERSION_INSUFFICIENT_CHECK',
        'Ml_Version_Insufficient_Check'],
    ['SHARED_TOOLBOX_VERSION_EQUALS_CHECK',
        'Ml_Version_Equals_Check'],
    ['SHARED_TOOLBOX_VERSION_INSUFFICIENT',
        'Ml_Math_Library_Version_Insufficient'],
    ['SHARED_TOOLBOX_VERSION_EQUALS', 'Ml_Math_Library_Version_Equals'],
    ['SHARED_TOOLBOX_VERSION_YEAR', 'ML_MATH_LIBRARY_VERSION_YEAR'],
    ['SHARED_TOOLBOX_VERSION_MONTH', 'ML_MATH_LIBRARY_VERSION_MONTH'],
    ['SHARED_TOOLBOX_VERSION_DAY', 'ML_MATH_LIBRARY_VERSION_DAY'],
    ['SHARED_TOOLBOX_VERSION_ITERATION', 'ML_MATH_LIBRARY_VERSION_ITERATION'],
    ['Get_Shared_Toolbox_Version', 'Get_Ml_Math_Library_Version'],
    ['init_max_sieve', 'Init_Max_Sieve'],
    ['init_min_sieve', 'Init_Min_Sieve'],
    ['init_min_max_sieve', 'Init_Min_Max_Sieve'],
    ['init_max_sieve_set', 'Init_Max_Sieve_Set'],
    ['init_min_sieve_set', 'Init_Min_Sieve_Set'],
    ['init_min_max_sieve_set', 'Init_Min_Max_Sieve_Set'],
    ['max_sieve', 'Max_Sieve'],
    ['min_sieve', 'Min_Sieve'],
    ['min_max_sieve', 'Min_Max_Sieve'],
    ['min_max_sieve_set', 'Min_Max_Sieve_Set'],
    ['max_sieve_set', 'Max_Sieve_Set'],
    ['min_sieve_set', 'Min_Sieve_Set'],
    ['get_max_sieve_content', 'Get_Max_Sieve_Content'],
    ['get_min_sieve_content', 'Get_Min_Sieve_Content'],
    ['get_min_max_sieve_min_content', 'Get_Min_Max_Sieve_Min_Content'],
    ['get_min_max_sieve_max_content', 'Get_Min_Max_Sieve_Max_Content'],
    ['get_max_from_max_sieve_set', 'Get_Max_From_Max_Sieve_Set'],
    ['get_min_from_min_sieve_set', 'Get_Min_From_Min_Sieve_Set'],
    ['get_max_sieve_set_content_at_index',
        'Get_Max_Sieve_Set_Content_At_Index'],
    ['get_min_sieve_set_content_at_index',
        'Get_Min_Sieve_Set_Content_At_Index'],
    ['is_sieved_value_valid', 'Is_Sieved_Value_Valid'],
    ['MAX_SIEVE_T', 'Max_Sieve_T'],
    ['MIN_MAX_SIEVE_T', 'Min_Max_Sieve_T'],
    ['MIN_SIEVE_T', 'Min_Sieve_T'],
    ['Vector_2d_Alg_Project_on_Angle', 'Vector_2d_Alg_Project_On_Angle'],
    ['Vector_2d_Alg_Project_on_rotated_xAxis',
        'Vector_2d_Alg_Project_On_Rotated_X_Axis'],
    ['Vector_2d_Alg_Abs_squared', 'Vector_2d_Alg_Abs_Squared'],
    ['Vector_2d_Alg_RotateNegative', 'Vector_2d_Alg_Rotate_Negative'],
    ['Vector_2d_Alg_Abs_ComponentWise', 'Vector_2d_Alg_Abs_Component_Wise'],
    ['Vector_2d_Alg_Sqrt_ComponentWise', 'Vector_2d_Alg_Sqrt_Component_Wise'],
    ['Vector_2d_Alg_Scalar_Product_with_angle',
        'Vector_2d_Alg_Scalar_Product_With_Angle'],
    ['Vector_2d_Alg_CalculateCosBetweenTwoVec',
        'Vector_2d_Alg_Calculate_Cos_Between_Two_Vec'],
    ['Vector_2d_Alg_LimitVector', 'Vector_2d_Alg_Limit_Vector'],
    ['VERSION_NUMBER_T', 'Version_Number_T'],
    ['Shared_Toolbox_Version_Insufficient',
        'Ml_Math_Library_Version_Insufficient'],
    ['Shared_Toolbox_Version_Equals', 'Ml_Math_Library_Version_Equals']
]


def remove_legacy_include(file_content):
    """Remove include statements for legacy headers

    Args:
        file_content (list[str]): The file to alter

    Returns:
        list[str]: File content without the legacy includes
    """
    ret_file: list[str] = []
    for line in file_content:
        if line.find('#include') >= 0:
            legacy_found = False
            for legacy_header in legacy_headers:
                if line.find(legacy_header) >= 0:
                    legacy_found = True
                    break
            if not legacy_found:
                ret_file.append(line)
        else:
            ret_file.append(line)
    return ret_file


def apply_replacements(file_content):
    """Find items to rename and rename them

    Args:
        file_content (list[str]): File to rename items in

    Returns:
        list[str]: file with items renamed
    """
    ret = []
    for line in file_content:
        # the header names are too similar to the struct names
        if line.find('#include') == -1:
            for replacement in replacements:
                line = line.replace(replacement[0], replacement[1])
        ret.append(line)
    return ret


def find_existing_ml_headers(file_content):
    """In order to not add duplicate includes for headers
    find existing ml_* headers

    Args:
        file_content (list[str]): File to search in

    Returns:
        list[str]: List of existing headers
    """
    headers = []
    for line in file_content:
        if line.find('#include') == 0:
            quote_pos = line.find('"')
            header = ''
            if quote_pos >= 0:
                header = line[quote_pos + 1:]
                quote_pos = header.find('"')
                header = header[:quote_pos]
            else:
                quote_pos = line.find('<')
                if quote_pos >= 0:
                    header = line[quote_pos + 1:]
                    quote_pos = header.find('>')
                    header = header[:quote_pos]
            if header and header.find('ml_') == 0:
                headers.append(header)
    return headers


def does_line_contain_item(
        line,
        item
        ):
    """Returns True if given line contains given item

    Args:
        line (str): line to search in
        item (str): item to search

    Returns:
        bool: True if item was found
    """
    does_contain = False
    item_pos = line.find(item)
    if item_pos >= 0:
        does_contain = True
        if item_pos > 0:
            char_before = line[item_pos - 1]
            if char_before.isdigit() or\
                    char_before.isalpha() or\
                    char_before == '_':
                does_contain = False
            char_after = line[item_pos + len(item)]
            if char_after.isdigit() or\
                    char_after.isalpha() or\
                    char_after == '_':
                does_contain = False
        if does_contain:
            cpp_comment_pos = line.find('//')
            c_comment_pos = line.find('/*')
            does_contain = False
            if cpp_comment_pos < 0 or item_pos < cpp_comment_pos:
                does_contain = True
            if c_comment_pos < 0 or item_pos < c_comment_pos:
                does_contain = True
    return does_contain


def find_needed_headers(file_content):
    """Generate a list of headers needed by given file_content

    Args:
        file_content (list[str]): File to search in

    Returns:
        list[str]: List of needed header files
    """
    needed_headers = []
    for item_header in item_headers:
        for item in item_header[1]:
            header_found = False
            within_comment = False
            for line in file_content:
                if line.find('/*') >= 0:
                    within_comment = True
                    if line.find('*/') >= 0:
                        within_comment = False
                if not within_comment:
                    add_header = does_line_contain_item(line, item)
                    if add_header:
                        print(
                            'adding header ' +
                            item_header[0] +
                            ' because of ' +
                            item)
                        needed_headers.append(item_header[0])
                        header_found = True
                        break
                if line.find('*/') >= 0:
                    within_comment = False
            if header_found:
                break
    existing_headers = find_existing_ml_headers(file_content)
    for existing_header in existing_headers:
        if existing_header in needed_headers:
            needed_headers.remove(existing_header)
    return needed_headers


def add_needed_headers(file_content, needed_headers):
    """Adds given needed_headers to given file_content

    Args:
        file_content (list[str]): File to add include statements to
        needed_headers (list[str]): List of include files to be added

    Returns:
        list[str]: Given file_content extended with the includes given
                   in needed_headers
    """
    ret_content = []
    index = 0
    last_inc_line = -1
    for line in file_content:
        if line.find('#include') >= 0:
            last_inc_line = index
        index = index + 1
    if last_inc_line == -1:
        # Try to find another good place. Search for the end of a
        # copyright comment
        index = 0
        for line in file_content:
            if line.find(
                    '\\*================================================' +
                    '===========================*/'
                    ) >= 0:
                last_inc_line = index
            index = index + 1
    index = 0
    for line in file_content:
        ret_content.append(line)
        if index == last_inc_line:
            for needed_header in needed_headers:
                new_line = '#include <' + needed_header + '>\n'
                ret_content.append(new_line)
        index = index + 1
    return ret_content


directory = os.getcwd()
for root, dirs, files in os.walk(directory):
    for filename in files:
        file_path = os.path.join(root, filename)
        file_suffix = pathlib.Path(filename).suffix
        if file_suffix in ['.c', '.h', '.cpp', '.hpp']:
            print(file_path)
            with open(file_path, 'r', encoding="utf8") as file_content:
                file_content = remove_legacy_include(file_content)
                file_content = apply_replacements(file_content)
                needed_headers = find_needed_headers(file_content)
                if filename in needed_headers:
                    needed_headers.remove(filename)
                if needed_headers:
                    file_content = add_needed_headers(
                        file_content,
                        needed_headers)
                with open(file_path, 'w', encoding="utf8") as the_file:
                    for line in file_content:
                        the_file.write(line)
