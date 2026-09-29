/**
 * @file ta_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 TA pre run tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-45214}
 */

#include "ta_pre_run_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_reuse.h"
#include "ta_bmw_enums.h"
#include "ta_pre_run.c"
}


/**
 * Checks, if address of static object is provided.
 * \uts{CSCSA-45215} \sdd{SF-8597} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Get_Ta_Bmw_Boardnet_Ptr__returns_correct_pointer_address)
{
   /** \arrange Declare pointer of type Ta_BMW_Boardnet_T. */
   Ta_BMW_Boardnet_T *p_test_address;

   /** \action Get boardnet data pointer */
   p_test_address = Ta_Get_Ta_Bmw_Boardnet_Ptr();

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_EQ(p_test_address, &Boardnet_Signals);
}

/**
 * Checks, if TA pre run is intialized correctly for ECU mounting position.
 * \uts{CSCSA-45216} \sdd{SF-8551} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Init_Input__all_parameters_set_correctly)
{
   /** \arrange */


   /** \action Call function Ta_Init_Input with parameter ta_input and central mounting position. */
   Ta_Init_Input(&ta_input, radar_position);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_TRUE(ta_input.bmw_boardnet_signals != NULL);

   EXPECT_EQ(ta_input.f_fta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.f_rta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.f_rta_enable_dynamic_area, FBK_TRUE);
   EXPECT_EQ(ta_input.f_rta_enable_turning_area, FBK_TRUE);

   EXPECT_EQ(ta_input.fta_steering_angle_max_left, FBK_ZERO_INT);
   EXPECT_EQ(ta_input.fta_steering_angle_max_right, FBK_ZERO_INT);

   EXPECT_FLOAT_EQ(ta_input.fta_obj_offset_x_positive, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_input.fta_obj_offset_x_negative, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_input.fta_obj_offset_y_positive, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_input.fta_obj_offset_y_negative, FBK_ZERO_F);
}

/**
 * Checks, if TA pre run is intialized correctly for rear application
 * \uts{CSCSA-45217} \sdd{SF-8551} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Init_Input__all_parameters_set_correctly_mounting_position)
{
   /** \arrange */
   radar_position = FRONT_RIGHT;

   /** \action Call function Ta_Init_Input with parameter ta_input and central mounting position. */
   Ta_Init_Input(&ta_input, radar_position);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_TRUE(ta_input.bmw_boardnet_signals != NULL);

   EXPECT_EQ(ta_input.f_fta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.f_rta_enable, FBK_ZERO_UINT);
   EXPECT_EQ(ta_input.f_rta_enable_dynamic_area, FBK_ZERO_UINT);
   EXPECT_EQ(ta_input.f_rta_enable_turning_area, FBK_ZERO_UINT);

   EXPECT_EQ(ta_input.fta_steering_angle_max_left, FBK_ZERO_UINT);
   EXPECT_EQ(ta_input.fta_steering_angle_max_right, FBK_ZERO_UINT);

   EXPECT_FLOAT_EQ(ta_input.fta_obj_offset_x_positive, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_input.fta_obj_offset_x_negative, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_input.fta_obj_offset_y_positive, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ta_input.fta_obj_offset_y_negative, FBK_ZERO_F);
}

/**
 * Checks, if TA pre run is intialized correctly for rear application
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Init_Input__set_up_rear_radar_mounting_pos)
{
   /** \arrange */


   /** \action Call function Ta_Init_Input with parameter ta_input and rear right mounting position. */
   Ta_Init_Input(&ta_input, REAR_RIGHT);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_TRUE(ta_input.bmw_boardnet_signals != NULL);

   EXPECT_EQ(ta_input.f_fta_enable, FBK_FALSE);
   EXPECT_EQ(ta_input.f_rta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.f_rta_enable_dynamic_area, FBK_TRUE);
   EXPECT_EQ(ta_input.f_rta_enable_turning_area, FBK_TRUE);
}

#ifndef NDEBUG
/**
 * Checks, if Ta_Pre_Run throws exception, when input pointer is NULL.
 * \uts{CSCSA-45218} \sdd{SF-8550} \testtype{negative}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__p_ta_instance_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Pre_Run throws exception, when p_ta_core_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Pre_Run(NULL, &ta_input, &fbk_output); }, ".*p_ta_instance.*");
}

/**
 * Checks, if Ta_Pre_Run throws exception, when input pointer is NULL.
 * \uts{CSCSA-45219} \sdd{SF-8550} \testtype{negative}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__ta_input_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Pre_Run throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Pre_Run(&ta_instance, NULL, &fbk_output); }, ".*p_ta_input.*");
}

/**
 * Checks, if Ta_Pre_Run throws exception, when input pointer is NULL.
 * \uts{CSCSA-45220} \sdd{SF-8550} \testtype{negative}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__p_fbk_output_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Pre_Run throws exception, when &ta_cal is NULL pointer. */
   EXPECT_DEATH({ Ta_Pre_Run(&ta_instance, &ta_input, NULL); }, ".*p_fbk_output.*");
}
#endif // !NDEBUG

/**
 * Checks, if TA pre run works as expected, if debug mode is disabled.
 * \uts{CSCSA-45221} \sdd{SF-8550} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__all_parameters_set_correctly)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. Disable debug mode. */
   ta_input.f_fta_enable                        = FBK_FALSE;
   ta_input.f_rta_enable                        = FBK_FALSE;
   ta_core_input.debug_mode_obj_pos_lat_offset  = 0.0f;
   ta_core_input.debug_mode_obj_pos_long_offset = 0.0f;

   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cal.k_f_ta_enable_debug_mode = 0u;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Lat
   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = -50.0f;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Long
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = -50.0f;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_EQ(ta_core_input.f_enable_debug_mode, FBK_FALSE);
   EXPECT_FLOAT_EQ(ta_core_input.debug_mode_obj_pos_lat_offset, 0.0f);
   EXPECT_FLOAT_EQ(ta_core_input.debug_mode_obj_pos_long_offset, 0.0f);
   EXPECT_EQ(ta_core_input.f_ta_enable, FBK_FALSE);
}

/**
 * Checks, if TA pre run works as expected, if debug mode is enabled.
 * \uts{CSCSA-45222} \sdd{SF-8550} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__all_parameters_set_correctly_debug_mode)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. Enable debug mode. */
   ta_input.f_fta_enable                        = FBK_TRUE;
   ta_input.f_rta_enable                        = FBK_FALSE;
   ta_core_input.debug_mode_obj_pos_lat_offset  = 0.0f;
   ta_core_input.debug_mode_obj_pos_long_offset = 0.0f;

   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_FALSE;
   ta_input.ta_coding_parameters.c_f_dynamic_ta_enabled   = FBK_FALSE;
   ta_input.ta_coding_parameters.c_f_static_ta_enabled    = FBK_FALSE;
   ta_input.ta_coding_parameters.c_f_cross_ta_enabled     = FBK_FALSE;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = TA_BMW_DEFAULT_MIN_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = TA_BMW_DEFAULT_MIN_VEL_UPPER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = TA_BMW_DEFAULT_MAX_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = TA_BMW_DEFAULT_MAX_VEL_UPPER_LIMIT;

   ta_input.ta_input_signals.vehicle_driving_direction = TA_SIGNAL_UNFILLED;
   ta_input.ta_input_signals.pwf_state                 = TA_PARKENBN_NIO;
   ta_input.ta_input_signals.status_dynamometer_mode   = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode   = TA_END_OF_LINE_MODE_NOT_SET;
   ta_input.ta_input_signals.ta_function_error         = FBK_FALSE;
   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cal.k_f_ta_enable_debug_mode = 1u;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Lat
   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = -50.0f;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Long
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = -50.0f;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_EQ(ta_core_input.f_enable_debug_mode, FBK_TRUE);
   EXPECT_FLOAT_EQ(ta_core_input.debug_mode_obj_pos_lat_offset,
                   -0.01f * (ta_input.fta_obj_offset_y_positive + ta_input.fta_obj_offset_y_negative));
   EXPECT_FLOAT_EQ(ta_core_input.debug_mode_obj_pos_long_offset,
                   0.01f * (ta_input.fta_obj_offset_x_positive + ta_input.fta_obj_offset_x_negative));
   EXPECT_EQ(ta_core_input.f_ta_enable, FBK_TRUE);
}

/**
 * Checks, if TA pre run works as expected, if debug mode is enabled and FTA is disabled.
 * \uts{CSCSA-45223} \sdd{SF-8550} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__all_parameters_set_correctly_debug_mode_rta_enable)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. Enable debug mode and disable FTA. */
   ta_input.f_fta_enable                                  = FBK_FALSE;
   ta_input.f_rta_enable                                  = FBK_FALSE;
   ta_core_input.debug_mode_obj_pos_lat_offset            = 0.0f;
   ta_core_input.debug_mode_obj_pos_long_offset           = 0.0f;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_FALSE;
   ta_input.ta_coding_parameters.c_f_dynamic_ta_enabled   = FBK_FALSE;
   ta_input.ta_coding_parameters.c_f_static_ta_enabled    = FBK_FALSE;
   ta_input.ta_coding_parameters.c_f_cross_ta_enabled     = FBK_FALSE;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = TA_BMW_DEFAULT_MIN_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = TA_BMW_DEFAULT_MIN_VEL_UPPER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = TA_BMW_DEFAULT_MAX_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = TA_BMW_DEFAULT_MAX_VEL_UPPER_LIMIT;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.pwf_state                    = TA_PARKENBN_NIO;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cal.k_f_ta_enable_debug_mode = 1u;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Lat
   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = -50.0f;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Long
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = -50.0f;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_EQ(ta_core_input.f_enable_debug_mode, FBK_FALSE);
   EXPECT_FLOAT_EQ(ta_core_input.debug_mode_obj_pos_lat_offset, 0.0f);
   EXPECT_FLOAT_EQ(ta_core_input.debug_mode_obj_pos_long_offset, 0.0f);
   EXPECT_EQ(ta_core_input.f_ta_enable, FBK_FALSE);
}

/**
 * Checks, if TA pre run works as expected, if State is not active f_ta_enable is false.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__check_rta_disabled_when_not_in_active_state)
{
   /** \arrange Set up TA input and calibration values with valid values. */
   ta_input.f_fta_enable                                  = FBK_FALSE;
   ta_input.f_rta_enable                                  = FBK_TRUE;
   ta_core_input.debug_mode_obj_pos_lat_offset            = 0.0f;
   ta_core_input.debug_mode_obj_pos_long_offset           = 0.0f;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_dynamic_ta_enabled   = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_static_ta_enabled    = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_cross_ta_enabled     = FBK_TRUE;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = TA_BMW_DEFAULT_MIN_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = TA_BMW_DEFAULT_MIN_VEL_UPPER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = TA_BMW_DEFAULT_MAX_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = TA_BMW_DEFAULT_MAX_VEL_UPPER_LIMIT;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.status_dynamometer_mode      = TWO_AXLE_ON_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cal.k_f_ta_enable_debug_mode = 1u;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Lat
   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = -50.0f;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Long
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = -50.0f;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if returned flag f_ta_enable is false. */
   EXPECT_EQ(ta_core_input.f_ta_enable, FBK_FALSE);
}

/**
 * Checks, if TA pre run works as expected, if State is active f_ta_enable TRUE
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__check_ta_enabled_when_in_active_state)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. */
   ta_input.f_fta_enable                                  = FBK_FALSE;
   ta_input.f_rta_enable                                  = FBK_TRUE;
   ta_core_input.debug_mode_obj_pos_lat_offset            = 0.0f;
   ta_core_input.debug_mode_obj_pos_long_offset           = 0.0f;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_dynamic_ta_enabled   = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_static_ta_enabled    = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_cross_ta_enabled     = FBK_TRUE;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = TA_BMW_DEFAULT_MIN_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = TA_BMW_DEFAULT_MIN_VEL_UPPER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = TA_BMW_DEFAULT_MAX_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = TA_BMW_DEFAULT_MAX_VEL_UPPER_LIMIT;

   p_vehicle_data->host_speed                          = 5.0f;
   ta_input.ta_input_signals.vehicle_driving_direction = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.pwf_state                 = TA_FAHREN;
   ta_input.ta_input_signals.status_dynamometer_mode   = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode   = TA_END_OF_LINE_MODE_NOT_SET;
   ta_input.ta_input_signals.ta_function_error         = FBK_FALSE;
   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cal.k_f_ta_enable_debug_mode = 1u;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Lat
   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = -50.0f;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Long
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = -50.0f;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if returned flag f_ta_enable is false. */
   EXPECT_EQ(ta_core_input.f_ta_enable, FBK_TRUE);
}

/**
 * Checks, if TA pre run works as expected, if State is active f_ta_enable TRUE
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__check_ta_enabled_when_in_active_state_fta_enabled)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. */
   ta_input.f_fta_enable                                  = FBK_TRUE;
   ta_input.f_rta_enable                                  = FBK_FALSE;
   ta_core_input.debug_mode_obj_pos_lat_offset            = 0.0f;
   ta_core_input.debug_mode_obj_pos_long_offset           = 0.0f;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_dynamic_ta_enabled   = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_static_ta_enabled    = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_cross_ta_enabled     = FBK_TRUE;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = TA_BMW_DEFAULT_MIN_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = TA_BMW_DEFAULT_MIN_VEL_UPPER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = TA_BMW_DEFAULT_MAX_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = TA_BMW_DEFAULT_MAX_VEL_UPPER_LIMIT;

   p_vehicle_data->host_speed                          = 5.0f;
   ta_input.ta_input_signals.vehicle_driving_direction = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.pwf_state                 = TA_FAHREN;
   ta_input.ta_input_signals.status_dynamometer_mode   = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode   = TA_END_OF_LINE_MODE_NOT_SET;
   ta_input.ta_input_signals.ta_function_error         = FBK_FALSE;
   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cal.k_f_ta_enable_debug_mode = 1u;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Lat
   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = -50.0f;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Long
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = -50.0f;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if returned flag f_ta_enable is false. */
   EXPECT_EQ(ta_core_input.f_ta_enable, FBK_TRUE);
}

/**
 * Checks, if TA pre run works as expected
 * if State is Active But f_fta_enable and f_rta_enable is false
 * Then f_ta_enable is false.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__check_rta_disabled_when_in_active_state_but_fta_and_rta_disbled)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. */
   ta_input.f_fta_enable                                  = FBK_FALSE;
   ta_input.f_rta_enable                                  = FBK_FALSE;
   ta_core_input.debug_mode_obj_pos_lat_offset            = 0.0f;
   ta_core_input.debug_mode_obj_pos_long_offset           = 0.0f;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_dynamic_ta_enabled   = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_static_ta_enabled    = FBK_TRUE;
   ta_input.ta_coding_parameters.c_f_cross_ta_enabled     = FBK_TRUE;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = TA_BMW_DEFAULT_MIN_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = TA_BMW_DEFAULT_MIN_VEL_UPPER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = TA_BMW_DEFAULT_MAX_VEL_LOWER_LIMIT;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = TA_BMW_DEFAULT_MAX_VEL_UPPER_LIMIT;

   p_vehicle_data->host_speed                          = 10.0f;
   ta_input.ta_input_signals.vehicle_driving_direction = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.pwf_state                 = TA_FAHREN;
   ta_input.ta_input_signals.status_dynamometer_mode   = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode   = TA_END_OF_LINE_MODE_NOT_SET;
   ta_input.ta_input_signals.ta_function_error         = FBK_FALSE;
   // Input parameters for subfunction Ta_Is_Diagnostic_Mode_Enabled
   ta_cal.k_f_ta_enable_debug_mode = 1u;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Lat
   ta_input.fta_obj_offset_y_positive = 100.0f;
   ta_input.fta_obj_offset_y_negative = -50.0f;

   // Input parameters for subfunction Ta_Get_Target_Shift_Offset_Long
   ta_input.fta_obj_offset_x_positive = 100.0f;
   ta_input.fta_obj_offset_x_negative = -50.0f;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if returned flag f_ta_enable is false. */
   EXPECT_EQ(ta_core_input.f_ta_enable, FBK_FALSE);
}

/**
 * Checks, if TA pre run works as expected, if host vehicle speed is in between k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max
 * and k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max. \uts{CSCSA-45224} \sdd{SF-8550} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__alert_ttp_threshold_alert_trigger_normal)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. */

   // set ego speed to expect TA_ALERT_TRIGGER_NORMAL to be set
   p_vehicle_data->host_speed = ta_cal.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max - EPSILON;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if alert_ttp_threshold is set correctly. */
   EXPECT_FLOAT_EQ(ta_core_input.alert_ttp_threshold, ta_cal.k_ta_alert_lvl_1_ttp_threshold[TA_ALERT_TRIGGER_NORMAL]);
}

/**
 * Checks, if TA pre run works as expected, if host vehicle speed is above k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max.
 * \uts{CSCSA-45225} \sdd{SF-8550} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__alert_ttp_threshold_alert_trigger_early)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. */

   // set ego speed to expect TA_ALERT_TRIGGER_EARLY to be set
   p_vehicle_data->host_speed = ta_cal.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max + EPSILON;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if alert_ttp_threshold is set correctly. */
   EXPECT_FLOAT_EQ(ta_core_input.alert_ttp_threshold, ta_cal.k_ta_alert_lvl_1_ttp_threshold[TA_ALERT_TRIGGER_EARLY]);
}

/**
 * Checks, if TA pre run works as expected, if host vehicle speed is below k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max.
 * \uts{CSCSA-45226} \sdd{SF-8550} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__alert_ttp_threshold_alert_trigger_late)
{
   /** \arrange Set up TA input and calibration values with valid values. */

   // set ego speed to expect TA_ALERT_TRIGGER_EARLY to be set
   p_vehicle_data->host_speed = ta_cal.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max - EPSILON;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if alert_ttp_threshold is set correctly. */
   EXPECT_FLOAT_EQ(ta_core_input.alert_ttp_threshold, ta_cal.k_ta_alert_lvl_1_ttp_threshold[TA_ALERT_TRIGGER_LATE]);
}
