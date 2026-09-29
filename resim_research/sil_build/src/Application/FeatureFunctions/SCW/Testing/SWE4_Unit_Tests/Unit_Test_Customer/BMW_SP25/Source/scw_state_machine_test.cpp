/**
 * @file scw_state_machine_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW state machine
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-101341}
 */

#include "scw_state_machine_test.hpp"
#include "scw_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "scw_state_machine.c"
#include "scw_types.h"
}

/*
 * Tests checking dynamometer status when front axle dynamometer is connected
 * \uts{CSCSA-101342} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Dynamometer_Status_Check__dynamometer_set_front)
{
   boolean_T return_flag;
   /** \arrange Dynamometer status set for Front Axle Dynamometer */
   scw_input.scw_vehicle_input.vehicle_dynamometer_status = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Dynamometer_Status_Check(&scw_input);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking dynamometer status when rear axle dynamometer is connected
 * \uts{CSCSA-101343} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Dynamometer_Status_Check__dynamometer_set_rear)
{
   boolean_T return_flag;
   /** \arrange Dynamometer status set for Rear Axle Dynamometer */
   scw_input.scw_vehicle_input.vehicle_dynamometer_status = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_BACK_AXLE_ON_DYNAMOMETER;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Dynamometer_Status_Check(&scw_input);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/**
 *
 * \uts{CSCSA-101344} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Dynamometer_Status_Check__dynamometer_set_two_axle)
{
   boolean_T return_flag;
   /** \arrange Dynamometer status set for Two Axle Dynamometer */
   scw_input.scw_vehicle_input.vehicle_dynamometer_status = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_TWO_AXLE_DYNAMOMETER;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Dynamometer_Status_Check(&scw_input);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking dynamometer status when status end of line is connected
 * \uts{CSCSA-101345} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Dynamometer_Status_Check__dynamometer_set_end_of_line)
{
   boolean_T return_flag;
   /** \arrange Dynamometer status set for Two Axle Dynamometer */
   scw_input.scw_vehicle_input.vehicle_end_of_line_status = SCW_BMW_STATUS_END_OF_LINE_MODE_SET;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Dynamometer_Status_Check(&scw_input);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking dynamometer status when dynamometer reports error
 * \uts{CSCSA-101346} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Dynamometer_Status_Check__dynamometer_reports_error)
{
   boolean_T return_flag;
   /** \arrange Dynamometer status set as ERROR */
   scw_input.scw_vehicle_input.vehicle_dynamometer_status = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_FUNCTION_REPORTS_ERROR;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Dynamometer_Status_Check(&scw_input);
   /** \assert expect the flag to be FALSE */
   EXPECT_EQ(return_flag, FBK_FALSE);
}

/*
 * Tests checking dynamometer status when unfilled
 * \uts{CSCSA-101347} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Dynamometer_Status_Check__dynamometer_unfilled)
{
   boolean_T return_flag;
   /** \arrange Dynamometer status set as unfilled */
   scw_input.scw_vehicle_input.vehicle_dynamometer_status = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Dynamometer_Status_Check(&scw_input);
   /** \assert expect the flag to be FALSE */
   EXPECT_EQ(return_flag, FBK_FALSE);
}

/*
 * Tests checking pwf status when DRIVING
 * \uts{CSCSA-101348} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Gear_In_Drive_Check___set_pwf_driving)
{
   boolean_T return_flag;
   /** \arrange Set pwf status and vehicle condition */
   scw_input.scw_vehicle_input.vehicle_condition = SCW_BMW_PWF_STATE_DRIVING;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Gear_In_Drive_Check(&scw_input);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking pwf status when not DRIVING
 * \uts{CSCSA-101349} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Gear_In_Drive_Check__pwf_not_driving)
{
   boolean_T return_flag;
   /** \arrange Set pwf status and vehicle condition */
   scw_input.scw_vehicle_input.vehicle_condition = SCW_BMW_PWF_STATE_ESTABLISH_DRIVING_AVAILABILITY;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Gear_In_Drive_Check(&scw_input);
   /** \assert expect the flag to be FALSE */
   EXPECT_EQ(return_flag, FBK_FALSE);
}

/*
 * Tests checking when vehicle movement type is forward
 * \uts{CSCSA-101350} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Ego_Move_Front_Check_moves_front)
{
   boolean_T return_flag;
   /** \arrange Set vehicle driving direction and vehicle condition */
   scw_input.scw_vehicle_input.vehicle_driving_direction = SCW_BMW_VEH_MOVING_DIR_MOVES_FORWARD;
   /** \action Call Dynamometer Status check Function. */
   return_flag = Scw_Ego_Move_Front_Check(&scw_input);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking when vehicle movement type is forward
 * \uts{CSCSA-101351} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Ego_Move_Front_Check_moves_moving)
{
   boolean_T return_flag;
   /** \arrange Set vehicle driving direction and vehicle condition */
   scw_input.scw_vehicle_input.vehicle_driving_direction = SCW_BMW_VEH_MOVING_DIR_MOVING;
   /** \action Call Ego Move Front check Function. */
   return_flag = Scw_Ego_Move_Front_Check(&scw_input);
   /** \assert expect the flag to be FALSE */
   EXPECT_EQ(return_flag, FBK_FALSE);
}

/*
 * Tests checking when speed > max vel upper limit
 * \uts{CSCSA-101352} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Max_Vel_Upper_Limit_Check_true)
{
   boolean_T return_flag;
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.scw_coding_parameters.c_scw_max_vel_upper_limit = 10.0f;
   p_vehicle_data->host_speed                                = 12.0f;
   /** \action Call Check Max Vel Upper Limit Function. */
   return_flag = Scw_Max_Vel_Upper_Limit_Check(&scw_input, p_vehicle_data);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking when speed < max vel upper limit
 * \uts{CSCSA-101353} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Max_Vel_Upper_Limit_Check_flag)
{
   boolean_T return_flag;
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.scw_coding_parameters.c_scw_max_vel_upper_limit = 12.0f;
   p_vehicle_data->host_speed                                = 3.0f;
   /** \action Call Check Max Vel Upper Limit Function. */
   return_flag = Scw_Max_Vel_Upper_Limit_Check(&scw_input, p_vehicle_data);
   /** \assert expect the flag to be FALSE */
   EXPECT_EQ(return_flag, FBK_FALSE);
}

/*
 * Tests checking when speed < min vel lower limit
 * \uts{CSCSA-101354} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Min_Vel_Lower_Limit_Check_true)
{
   boolean_T return_flag;
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.scw_coding_parameters.c_scw_min_vel_lower_limit = 14.0f;
   p_vehicle_data->host_speed                                = 3.0f;
   /** \action Call Check Min Vel Lower Limit Function. */
   return_flag = Scw_Min_Vel_Lower_Limit_Check(&scw_input, p_vehicle_data);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking when speed < min vel lower limit
 * \uts{CSCSA-101355} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Min_Vel_Lower_Limit_Check_false)
{
   boolean_T return_flag;
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.scw_coding_parameters.c_scw_min_vel_lower_limit = 10.0f;
   p_vehicle_data->host_speed                                = 12.0f;
   /** \action Call Check Min Vel Lower Limit Function. */
   return_flag = Scw_Min_Vel_Lower_Limit_Check(&scw_input, p_vehicle_data);
   /** \assert expect the flag to be FALSE */
   EXPECT_EQ(return_flag, FBK_FALSE);
}

/*
 * Tests checking when speed < max vel lower limit
 * \uts{CSCSA-101356} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Max_Vel_Lower_Limit_Check_true)
{
   boolean_T return_flag;
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.scw_coding_parameters.c_scw_max_vel_lower_limit = 14.0f;
   p_vehicle_data->host_speed                                = 2.5f;
   /** \action Call Check Max Vel Lower Limit Function. */
   return_flag = Scw_Max_Vel_Lower_Limit_Check(&scw_input, p_vehicle_data);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking when speed < max vel lower limit
 * \uts{CSCSA-101357} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Max_Vel_Lower_Limit_Check_false)
{
   boolean_T return_flag;
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.scw_coding_parameters.c_scw_max_vel_lower_limit = 10.0f;
   p_vehicle_data->host_speed                                = 12.0f;
   /** \action Call Check Max Vel Lower Limit Function. */
   return_flag = Scw_Max_Vel_Lower_Limit_Check(&scw_input, p_vehicle_data);
   /** \assert expect the flag to be FALSE */
   EXPECT_EQ(return_flag, FBK_FALSE);
}

/*
 * Tests checking when speed < min vel upper limit
 * \uts{CSCSA-101358} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Min_Vel_Upper_Limit_Check_true)
{
   boolean_T return_flag;
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.scw_coding_parameters.c_scw_min_vel_upper_limit = 10.0f;
   p_vehicle_data->host_speed                                = 12.0f;
   /** \action Call Check Min Vel Upper Limit Function. */
   return_flag = Scw_Min_Vel_Upper_Limit_Check(&scw_input, p_vehicle_data);
   /** \assert expect the flag to be TRUE */
   EXPECT_EQ(return_flag, FBK_TRUE);
}

/*
 * Tests checking when speed < min vel upper limit
 * \uts{CSCSA-101359} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Min_Vel_Upper_Limit_Check_false)
{
   boolean_T return_flag;
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.scw_coding_parameters.c_scw_min_vel_upper_limit = 20.0f;
   p_vehicle_data->host_speed                                = 4.0f;
   /** \action Call Check Min Vel Upper Limit Function. */
   return_flag = Scw_Min_Vel_Upper_Limit_Check(&scw_input, p_vehicle_data);
   /** \assert expect the flag to be False */
   EXPECT_EQ(return_flag, FBK_FALSE);
}

/*
 * Tests checking if all the flags are passed out
 * \uts{CSCSA-101360} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Update_Flags_check)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                                    = FBK_TRUE;
   scw_input.scw_coding_parameters.c_scw_min_vel_upper_limit = 10.0f;
   p_vehicle_data->host_speed                                = 12.0f;
   scw_input.scw_vehicle_input.vehicle_driving_direction     = SCW_BMW_VEH_MOVING_DIR_MOVES_FORWARD;
   scw_input.scw_vehicle_input.vehicle_condition             = SCW_BMW_PWF_STATE_DRIVING;
   scw_input.scw_vehicle_input.vehicle_dynamometer_status    = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   /** \action Call Set Update Flags Function. */
   Scw_Update_Flags(&scw_input, &scw_state_flags, p_vehicle_data);
   /** \assert expect the flags outputs as passed */
   EXPECT_FLOAT_EQ(scw_state_flags.f_scw_enable_check, FBK_TRUE);
   EXPECT_FLOAT_EQ(scw_state_flags.f_min_vel_upper_limit_check, FBK_TRUE);
   EXPECT_FLOAT_EQ(scw_state_flags.f_max_vel_lower_limit_check, FBK_FALSE);
   EXPECT_FLOAT_EQ(scw_state_flags.f_min_vel_lower_limit_check, FBK_FALSE);
   EXPECT_FLOAT_EQ(scw_state_flags.f_min_vel_upper_limit_check, FBK_TRUE);
   EXPECT_FLOAT_EQ(scw_state_flags.f_min_vel_upper_limit_check, FBK_TRUE);
   EXPECT_FLOAT_EQ(scw_state_flags.f_drive_mode_check, FBK_TRUE);
   EXPECT_FLOAT_EQ(scw_state_flags.f_move_front_check, FBK_TRUE);
   EXPECT_FLOAT_EQ(scw_state_flags.f_scw_test_mode_check, FBK_TRUE);
}

/*
 * Tests checking if all the flags are passed out
 * \uts{CSCSA-101361} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Update_Flags_check_false)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable = FBK_FALSE;
   /** \action Call Set Update Flags Function. */
   Scw_Update_Flags(&scw_input, &scw_state_flags, p_vehicle_data);
   /** \assert expect the flags outputs as passed */
   EXPECT_FLOAT_EQ(scw_state_flags.f_scw_enable_check, FBK_FALSE);
}

/*
 * Test Transition from Inactive to Active
 * \uts{CSCSA-101362} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Inactive_to_active)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_INACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_TRUE;
   scw_state_flags.f_min_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_lower_limit_check = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Inactive Function. */
   Scw_Transitions_from_Inactive(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_ACTIVE);
}

/*
 * Test Transition from Inactive to Active
 * \uts{CSCSA-101363} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Inactive_to_active_lower_limit_false)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_INACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_TRUE;
   scw_state_flags.f_min_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_lower_limit_check = FBK_FALSE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Inactive Function. */
   Scw_Transitions_from_Inactive(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Inactive to Active
 * \uts{CSCSA-101364} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Inactive_to_active_front_check)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_INACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_TRUE;
   scw_state_flags.f_min_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_lower_limit_check = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_FALSE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Inactive Function. */
   Scw_Transitions_from_Inactive(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Inactive to Active
 * \uts{CSCSA-101365} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Inactive_to_active_drive_mode_check)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_INACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_TRUE;
   scw_state_flags.f_min_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_lower_limit_check = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_FALSE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Inactive Function. */
   Scw_Transitions_from_Inactive(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Inactive to Active
 * \uts{CSCSA-101366} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Inactive_to_active_test_mode_check)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_INACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_TRUE;
   scw_state_flags.f_min_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_lower_limit_check = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_TRUE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Inactive Function. */
   Scw_Transitions_from_Inactive(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Inactive to Not available
 * \uts{CSCSA-101367} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Inactive_to_Not_available)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_FALSE;
   *p_scw_current_state                        = SCW_STATE_INACTIVE;
   scw_state_flags.f_min_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_lower_limit_check = FBK_FALSE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Inactive function. */
   Scw_Transitions_from_Inactive(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_NOT_AVAILABLE);
}

/*
 * Test Transition from Inactive to Error
 * \uts{CSCSA-101368} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Inactive_to_Error)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_INACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_TRUE;
   scw_state_flags.f_min_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_lower_limit_check = FBK_FALSE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_TRUE;
   /** \action Call Transitions from Inactive Function. */
   Scw_Transitions_from_Inactive(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_ERROR);
}

/*
 * Test Transition from Active to Inactive
 * \uts{CSCSA-101369} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Active_to_Inactive)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_TRUE;
   scw_state_flags.f_min_vel_lower_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_FALSE;
   scw_state_flags.f_drive_mode_check          = FBK_FALSE;
   scw_state_flags.f_scw_test_mode_check       = FBK_TRUE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Active Function. */
   Scw_Transitions_from_Active(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Active to Active .
 * \uts{CSCSA-101370} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Active_to_Inactive_no_change)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_FALSE;
   scw_state_flags.f_min_vel_lower_limit_check = FBK_FALSE;
   scw_state_flags.f_max_vel_upper_limit_check = FBK_FALSE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Active Function. */
   Scw_Transitions_from_Active(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_ACTIVE);
}

/*
 * Test Transition from Active to Inactive
 * \uts{CSCSA-101371} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Active_to_Inactive_lower_limit)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_FALSE;
   scw_state_flags.f_min_vel_lower_limit_check = FBK_TRUE;
   scw_state_flags.f_max_vel_upper_limit_check = FBK_FALSE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Active Function. */
   Scw_Transitions_from_Active(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Active to Inactive
 * \uts{CSCSA-101372} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Active_to_Inactive_max_upper_limit)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_FALSE;
   scw_state_flags.f_min_vel_lower_limit_check = FBK_FALSE;
   scw_state_flags.f_max_vel_upper_limit_check = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Active Function. */
   Scw_Transitions_from_Active(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Active to Inactive
 * \uts{CSCSA-101373} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Active_to_Inactive_Move_Front_Check)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_FALSE;
   scw_state_flags.f_min_vel_lower_limit_check = FBK_FALSE;
   scw_state_flags.f_max_vel_upper_limit_check = FBK_FALSE;
   scw_state_flags.f_move_front_check          = FBK_FALSE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Active Function. */
   Scw_Transitions_from_Active(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Active to Inactive
 * \uts{CSCSA-101374} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Active_to_Inactive_drive_mode)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_FALSE;
   scw_state_flags.f_min_vel_lower_limit_check = FBK_FALSE;
   scw_state_flags.f_max_vel_upper_limit_check = FBK_FALSE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_FALSE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Active Function. */
   Scw_Transitions_from_Active(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Active to Inactive
 * \uts{CSCSA-101375} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Active_to_Inactive_test_mode)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_FALSE;
   scw_state_flags.f_min_vel_lower_limit_check = FBK_FALSE;
   scw_state_flags.f_max_vel_upper_limit_check = FBK_FALSE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_TRUE;
   f_scw_error                                 = FBK_FALSE;
   /** \action Call Transitions from Active Function. */
   Scw_Transitions_from_Active(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Active to Error
 * \uts{CSCSA-101376} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Active_to_Error)
{
   /** \arrange Set coding parameter and vehicle speed */
   scw_input.f_scw_enable                      = FBK_TRUE;
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   scw_state_flags.f_scw_enable_check          = FBK_TRUE;
   scw_state_flags.f_min_vel_lower_limit_check = FBK_TRUE;
   scw_state_flags.f_drive_mode_check          = FBK_TRUE;
   scw_state_flags.f_move_front_check          = FBK_TRUE;
   scw_state_flags.f_scw_test_mode_check       = FBK_FALSE;
   f_scw_error                                 = FBK_TRUE;
   /** \action Call Transitions from Active Function. */
   Scw_Transitions_from_Active(&scw_state_flags, p_scw_current_state, &f_scw_error);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_ERROR);
}

/*
 * Test Transition from Not available to Inactive
 * \uts{CSCSA-101377} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Not_Available_to_Inactive)
{
   *p_scw_current_state = SCW_STATE_NOT_AVAILABLE;
   /** \arrange Set coding parameter and vehicle speed */
   scw_state_flags.f_scw_enable_check = FBK_TRUE;
   /** \action Call Scw State Machine Function. */
   Scw_State_Machine(&scw_state_flags, &f_scw_error, p_scw_current_state);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Not available to Inactive
 * \uts{CSCSA-101378} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Not_Available_to_Inactive_scw_disabled)
{
   /** \arrange Set coding parameter and vehicle speed */
   *p_scw_current_state               = SCW_STATE_NOT_AVAILABLE;
   scw_state_flags.f_scw_enable_check = FBK_FALSE;
   /** \action Call Scw State Machine Function. */
   Scw_State_Machine(&scw_state_flags, &f_scw_error, p_scw_current_state);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_NOT_AVAILABLE);
}

/*
 * Test Transition from Error to Inactive
 * \uts{CSCSA-101379} \sdd{} \testtype{positive}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Error_to_Inactive)
{
   *p_scw_current_state = SCW_STATE_ERROR;
   /** \arrange Set coding parameter and vehicle speed */
   scw_state_flags.f_scw_enable_check = FBK_TRUE;
   f_scw_error                        = FBK_FALSE;
   /** \action Call Scw State Machine Function. */
   Scw_State_Machine(&scw_state_flags, &f_scw_error, p_scw_current_state);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_INACTIVE);
}

/*
 * Test Transition from Error to Inactive
 * \uts{CSCSA-101380} \sdd{} \testtype{negative}
 */
TEST_F(Scw_State_Machine_Test, Scw_Transitions_from_Error_to_Inactive_scw_error)
{
   /** \arrange Set coding parameter and vehicle speed */
   *p_scw_current_state               = SCW_STATE_ERROR;
   scw_state_flags.f_scw_enable_check = FBK_TRUE;
   f_scw_error                        = FBK_TRUE;
   /** \action Call Scw State Machine Function. */
   Scw_State_Machine(&scw_state_flags, &f_scw_error, p_scw_current_state);
   /** \assert expect the flags outputs as passed */
   EXPECT_EQ(*p_scw_current_state, SCW_STATE_ERROR);
}