/**
 * @file ta_state_machine_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for ta_state_machine.c functions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 * \uts_heading_wi{WI-18621}
 */

/* Root work item under which all workitems within this file are created
 * \
 */

#include "ta_state_machine_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>


extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_types.h"
}

/**
 * Check whether the Transition_From_Not_Available_To_Inactive is as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Not_Available_To_Inactive)
{
   /** \arrange data required to test transition from not available to inactive state*/
   ta_current_state                             = TA_STATE_NOT_AVAILABLE;
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_InActive_To_Not Available is as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_InActive_To_Not_Available)
{
   /** \arrange data required to test transition from inactive to not available state*/
   ta_current_state                             = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_FALSE;
   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_NOT_AVAILABLE);
}


/**
 * Check whether the Transition_From_Inactive_To_Active
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Active)
{
   /** \arrange data required to test transition from inactive to active state*/

   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_current_state                                       = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Active*/
   EXPECT_EQ(ta_current_state, TA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active_velocity_upper
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Active_vel_upper)
{
   /** \arrange data required to test transition from inactive to active state*/

   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_current_state                                       = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 11.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 2.5f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Active*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active_velocity_lower
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Active_vel_lower)
{
   /** \arrange data required to test transition from inactive state*/

   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_current_state                                       = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 50.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Active*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active_velocity_lower
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Active_driving_direction)
{
   /** \arrange data required to test transition from inactive state*/

   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_current_state                                       = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_BACKWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Active*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active_velocity_lower
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Active_end_of_line)
{
   /** \arrange data required to test transition from not inactive to active state*/

   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_current_state                                       = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Active*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active_velocity_lower
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Active_function_error)
{
   /** \arrange data required to test transition from not inactive to active state*/

   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_current_state                                       = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_TRUE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Active*/
   EXPECT_NE(ta_current_state, TA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active_velocity_upper
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Active_pwf_state)
{
   /** \arrange data required to test transition from not inactive to active state*/

   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_current_state                                       = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_WOHNEN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Active*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Error
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Error)
{
   /** \arrange data required to test transition from not inactive to error state*/

   ta_current_state                             = TA_STATE_INACTIVE;
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_input.ta_input_signals.ta_function_error  = FBK_TRUE;
   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Error*/
   EXPECT_EQ(ta_current_state, TA_STATE_ERROR);
}


/**
 * Check whether the Transition_From_Inactive_To_Not_Available
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Inactive_To_Not_Available)
{
   /** \arrange data required to test transition from not inactive to not available state*/

   ta_current_state = TA_STATE_INACTIVE;

   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_FALSE;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available*/
   EXPECT_EQ(ta_current_state, TA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Active_To_Inactive_Speed_Condition_Failed)
{
   /** \arrange data required to test transition from active to inactive state*/
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_current_state                             = TA_STATE_ACTIVE;

   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 0.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Active_To_Inactive_Driving_Condition_Failed)
{
   /** \arrange data required to test transition from active to inactive state*/
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_current_state                             = TA_STATE_ACTIVE;

   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_WOHNEN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Active_To_Inactive_Driving_Direction_Condition_Failed)
{
   /** \arrange data required to test transition from active to inactive state*/
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_current_state                             = TA_STATE_ACTIVE;

   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_BACKWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Active_To_Inactive_Error_Condition_Failed)
{
   /** \arrange data required to test transition from active to inactive state*/
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_current_state                             = TA_STATE_ACTIVE;

   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_BACKWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_TRUE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_NE(ta_current_state, TA_STATE_INACTIVE);
   EXPECT_EQ(ta_current_state, TA_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Active_To_Inactive_Dynamometer_Condition_Failed)
{
   /** \arrange data required to test transition from active to inactive state*/
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_current_state                             = TA_STATE_ACTIVE;

   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = FRONT_AXLE_ON_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Active_To_Inactive_EOL_Condition_Failed)
{
   /** \arrange data required to test transition from active to inactive state*/
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_current_state                             = TA_STATE_ACTIVE;

   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive_max_velocity_upper
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Active_To_Inactive_max_velocity_upper)
{
   /** \arrange data required to test transition from active to inactive state*/
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_current_state                             = TA_STATE_ACTIVE;

   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 10.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 15.0f;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Not_Available
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Active_To_Not_Available)
{
   /** \arrange data required to test transition from active to not available state*/

   ta_current_state = TA_STATE_ACTIVE;

   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_FALSE;
   ta_current_state                             = TA_STATE_ACTIVE;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available*/
   EXPECT_EQ(ta_current_state, TA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Error_To_Inactive
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Transition_From_Error_To_Inactive)
{
   /** \arrange data required to test transition from error to inactive state*/

   ta_current_state = TA_STATE_ERROR;

   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_TRUE;
   ta_input.ta_input_signals.ta_function_error  = FBK_FALSE;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive*/
   EXPECT_EQ(ta_current_state, TA_STATE_INACTIVE);
}

/**
 * check whether the Trasition_From_Error_To_Not_Available
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test, Ta_Set_Current_Ta_Functional_State__Trasition_From_Error_To_Not_Available)
{
   /** \arrange data required to test transition from error to not available state*/
   ta_current_state                             = TA_STATE_ERROR;
   ta_input.ta_coding_parameters.c_f_ta_enabled = FBK_FALSE;
   ta_input.ta_input_signals.ta_function_error  = FBK_TRUE;

   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available*/
   EXPECT_EQ(ta_current_state, TA_STATE_NOT_AVAILABLE);
}

/**
 * check whether the Check Subfunctions are Disabled or not
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(TA_State_Machine_Test,
       Ta_Set_Current_Ta_Functional_State__Transition_From_Not_Available_To_Active_Based_on_Initial_Default_Value_Provided_In_Pre_Run_Init)
{
   /** \arrange data required to test transition from not available to active state*/
   ta_current_state                                       = TA_STATE_NOT_AVAILABLE;
   ta_input.ta_coding_parameters.c_f_ta_enabled           = FBK_TRUE;
   ta_input.ta_coding_parameters.c_ta_max_vel_upper_limit = 50.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_upper_limit = 0.0f;
   ta_input.ta_coding_parameters.c_ta_max_vel_lower_limit = 45.0f;
   ta_input.ta_coding_parameters.c_ta_min_vel_lower_limit = 0.0f;
   p_vehicle_data->host_speed                             = 10.0f;
   ta_input.ta_input_signals.vehicle_driving_direction    = TA_VEHICLE_MOVES_FORWARD;
   ta_input.ta_input_signals.pwf_state                    = TA_FAHREN;
   ta_input.ta_input_signals.status_dynamometer_mode      = NO_DYNAMOMETER;
   ta_input.ta_input_signals.status_end_of_line_mode      = TA_END_OF_LINE_MODE_NOT_SET;
   ta_input.ta_input_signals.ta_function_error            = FBK_FALSE;
   /** \action Set_Current_Ta_Functional_State function*/
   Ta_Set_Current_Ta_Functional_State(&ta_input, &ta_current_state, p_vehicle_data);

   /** \assert Verify current State is Active*/
   EXPECT_EQ(ta_current_state, TA_STATE_ACTIVE);
}