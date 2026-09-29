/**
 * @file ltb_state_machine_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 LTB State Machine
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "ltb_state_machine_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ltb_state_machine.c"
#include "ltb_state_machine.h"
#include "pa_reuse.h"
}

/**
 * Check whether the Ltb_Update_Vehicle_Speed_Check_Flag__Update_vehicle_speed_flag_True.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Vehicle_Speed_Check_Flag__Update_vehicle_speed_flag_True)
{
   /** \arrange set vehicle data and Calibration data to set speed check flag */
   boolean_T speed_check_flag        = FBK_FALSE;
   vehicle_data.host_speed           = 2.5f;
   ltb_cals.k_ltb_bmw_sp25_v_ego_max = 10.0f;

   /** \action execute to update speed check flag */
   speed_check_flag = Ltb_Update_Vehicle_Speed_Check_Flag(&ltb_cals, &vehicle_data);

   /** \assert expect speed check flag to TRUE */
   EXPECT_TRUE(speed_check_flag);
}

/**
 * Check whether the Ltb_Update_Vehicle_Speed_Check_Flag__Update_vehicle_speed_flag_False.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Vehicle_Speed_Check_Flag__Update_vehicle_speed_flag_False)
{
   /** \arrange set vehicle data and Calibration data to set speed check flag */
   boolean_T speed_check_flag        = FBK_TRUE;
   vehicle_data.host_speed           = 10.0f;
   ltb_cals.k_ltb_bmw_sp25_v_ego_max = 2.5f;

   /** \action execute to update speed check flag */
   speed_check_flag = Ltb_Update_Vehicle_Speed_Check_Flag(&ltb_cals, &vehicle_data);

   /** \assert expect speed check flag to FALSE */
   EXPECT_FALSE(speed_check_flag);
}

/**
 * Check whether the Ltb_Update_Vehicle_Speed_Check_Hys_Flag__Update_vehicle_speed_flag_True.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Vehicle_Speed_Check_Hys_Flag__Update_vehicle_speed_hysterese_flag_True)
{
   /** \arrange set vehicle data and Calibration data to set speed check flag */
   boolean_T speed_check_hys_flag        = FBK_FALSE;
   vehicle_data.host_speed               = 25.5f;
   ltb_cals.k_ltb_bmw_sp25_v_ego_max     = 10.0f;
   ltb_cals.k_ltb_bmw_sp25_v_ego_max_hys = 13.f;

   /** \action execute to update speed check hysterese flag */
   speed_check_hys_flag = Ltb_Update_Vehicle_Speed_Check_Hys_Flag(&ltb_cals, &vehicle_data);

   /** \assert expect speed check flag to TRUE */
   EXPECT_TRUE(speed_check_hys_flag);
}

/**
 * Check whether the Ltb_Update_Vehicle_Speed_Check_Hys_Flag__Update_vehicle_speed_flag_False.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Vehicle_Speed_Check_Hys_Flag__Update_vehicle_speed_hysterese_flag_False)
{
   /** \arrange set vehicle data and Calibration data to set speed check flag */
   boolean_T speed_check_hys_flag        = FBK_TRUE;
   vehicle_data.host_speed               = 10.0f;
   ltb_cals.k_ltb_bmw_sp25_v_ego_max     = 2.5f;
   ltb_cals.k_ltb_bmw_sp25_v_ego_max_hys = 13.f;

   /** \action execute to update speed check hysterese flag */
   speed_check_hys_flag = Ltb_Update_Vehicle_Speed_Check_Hys_Flag(&ltb_cals, &vehicle_data);

   /** \assert expect speed check flag to FALSE */
   EXPECT_FALSE(speed_check_hys_flag);
}

/**
 * Check whether the Ltb_Update_Vehicle_Moving_Forward_Check_Flag__Check_vehicle_Moving_Forward is expected.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Vehicle_Moving_Forward_Check_Flag__Check_vehicle_Moving_Forward)
{
   /** \arrange set ltb input to check moving Direction flag*/
   boolean_T direction_flag                                  = FBK_TRUE;
   ltb_input.ltb_vehicle_parameters.vehicle_moving_direction = BMW_CTB_VEH_MOVING_DIR_MOVES_FORWARD;

   /** \action execute to update moving direction flag to Forward */
   direction_flag = Ltb_Update_Vehicle_Moving_Forward_Check_Flag(&ltb_input);

   /** \assert expect moving direction flag to True */
   EXPECT_TRUE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Vehicle_Moving_Forward_Check_Flag__Check_vehicle_Moving_not_Forward is expected.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Vehicle_Moving_Forward_Check_Flag__Check_vehicle_Moving_not_Forward)
{
   /** \arrange set ltb input to check moving Direction flag*/
   boolean_T direction_flag                                  = FBK_TRUE;
   ltb_input.ltb_vehicle_parameters.vehicle_moving_direction = BMW_CTB_VEH_MOVING_DIR_MOVES_BACKWARDS;

   /** \action execute to update moving direction flag to Forward */
   direction_flag = Ltb_Update_Vehicle_Moving_Forward_Check_Flag(&ltb_input);

   /** \assert expect moving direction flag to False */
   EXPECT_FALSE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Vehicle_Moving_Forward_Check_Flag__Pwf_state_Driving is expected.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Vehicle_Moving_Forward_Check_Flag__Pwf_state_Driving)
{
   /** \arrange set ltb input to check pwf state flag*/
   boolean_T direction_flag                   = FBK_FALSE;
   ltb_input.ltb_vehicle_parameters.pwf_state = DRIVING;

   /** \action execute to check if  pwf state flag to Driving */
   direction_flag = Ltb_Update_Pwf_Check_Flag(&ltb_input);

   /** \assert expect pwf state flag to True */
   EXPECT_TRUE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Vehicle_Moving_Forward_Check_Flag__Pwf_state_not_Driving is expected.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Vehicle_Moving_Forward_Check_Flag__Pwf_not_state_Driving)
{
   /** \arrange set ltb input to check pwf state flag*/
   boolean_T direction_flag                   = FBK_TRUE;
   ltb_input.ltb_vehicle_parameters.pwf_state = DWELLING;

   /** \action execute to check if  pwf state flag is not  Driving */
   direction_flag = Ltb_Update_Pwf_Check_Flag(&ltb_input);

   /** \assert expect pwf state flag to False */
   EXPECT_FALSE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Test_Mode_Check_Flag__status_roller_dynamometer_active is expected.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Test_Mode_Check_Flag__status_roller_dynamometer_active)
{
   /** \arrange set ltb input to check status roller dynamometer flag*/
   boolean_T direction_flag                                   = FBK_FALSE;
   ltb_input.ltb_vehicle_parameters.status_roller_dynamometer = BMW_LTB_FRONT_AXLE_ON_DYNAMOMETER;

   /** \action execute to check if  status roller dynamometer flag Activated or not */
   direction_flag = Ltb_Update_Test_Mode_Check_Flag(&ltb_input);

   /** \assert expect status roller dynamometer flag to True */
   EXPECT_TRUE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Test_Mode_Check_Flag__status_roller_dynamometer_back_axle is expected.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Test_Mode_Check_Flag__status_roller_dynamometer_back_axle)
{
   /** \arrange set ltb input to check status roller dynamometer flag*/
   boolean_T direction_flag                                   = FBK_FALSE;
   ltb_input.ltb_vehicle_parameters.status_roller_dynamometer = BMW_LTB_BACK_AXLE_ON_DYNAMOMETER;

   /** \action execute to check if  status roller dynamometer flag Activated or not */
   direction_flag = Ltb_Update_Test_Mode_Check_Flag(&ltb_input);

   /** \assert expect status roller dynamometer flag to True */
   EXPECT_TRUE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Test_Mode_Check_Flag__status_roller_dynamometer_two_axle is expected.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Test_Mode_Check_Flag__status_roller_dynamometer_two_axle)
{
   /** \arrange set ltb input to check status roller dynamometer flag*/
   boolean_T direction_flag                                   = FBK_FALSE;
   ltb_input.ltb_vehicle_parameters.status_roller_dynamometer = BMW_LTB_TWO_AXLE_DYNAMOMETER;

   /** \action execute to check if  status roller dynamometer flag Activated or not */
   direction_flag = Ltb_Update_Test_Mode_Check_Flag(&ltb_input);

   /** \assert expect status roller dynamometer flag to True */
   EXPECT_TRUE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Test_Mode_Check_Flag__status_end_of_line is expected.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Test_Mode_Check_Flag__status_end_of_line)
{
   /** \arrange set ltb input to check status roller dynamometer flag*/
   boolean_T direction_flag                                   = FBK_FALSE;
   ltb_input.ltb_vehicle_parameters.status_roller_dynamometer = BMW_LTB_NO_DYNAMOMETER;
   ltb_input.ltb_vehicle_parameters.status_end_of_lne         = LTB_END_OF_LINE_MODE_SET;

   /** \action execute to check if  status roller dynamometer flag Activated or not */
   direction_flag = Ltb_Update_Test_Mode_Check_Flag(&ltb_input);

   /** \assert expect status roller dynamometer flag to True */
   EXPECT_TRUE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Test_Mode_Check_Flag__status_roller_dynamometer_not_active is expected.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Test_Mode_Check_Flag__status_roller_dynamometer_not_active)
{
   /** \arrange set ltb input to check status roller dynamometer flag*/
   boolean_T direction_flag                                   = FBK_TRUE;
   ltb_input.ltb_vehicle_parameters.status_roller_dynamometer = BMW_LTB_NO_DYNAMOMETER;

   /** \action execute to check if  status roller dynamometer flag Activated or not */
   direction_flag = Ltb_Update_Test_Mode_Check_Flag(&ltb_input);

   /** \assert expect status roller dynamometer flag to FALSE */
   EXPECT_FALSE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Error_Flag__Error_flag_Active is expected.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Error_Flag__Error_flag_Active)
{
   /** \arrange set ltb input to check error flag*/
   boolean_T direction_flag = FBK_FALSE;
   ltb_input.ltb_error      = LTB_ERROR;

   /** \action execute to check if  error flag Activated or not */
   direction_flag = Ltb_Update_Error_Flag(&ltb_input);

   /** \assert expect error to TRUE */
   EXPECT_TRUE(direction_flag);
}

/**
 * Check whether the Ltb_Update_Error_Flag__Error_flag_not_Active is expected.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_Update_Error_Flag__Error_flag_not_Active)
{
   /** \arrange set ltb input to check error flag*/
   boolean_T direction_flag = FBK_TRUE;
   ltb_input.ltb_error      = LTB_NO_ERROR;

   /** \action execute to check if  error flag Activated or not */
   direction_flag = Ltb_Update_Error_Flag(&ltb_input);

   /** \assert expect error to FALSE */
   EXPECT_FALSE(direction_flag);
}

/**
 * Check whether the Transition_From_Error_To_Ready.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Error_to_Ready)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_ERROR;
   state_flag.Ltb_Error_Check_Flag                     = FBK_FALSE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_FALSE;
   state_flag.Ltb_Speed_Check_Flag                     = FBK_FALSE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_TRUE;

   /** \action execute change the current state from ERROR to READY */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Ready */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether the Transition_From_Error_To_Ready.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Error_to_Ready_check_flag_active)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State               = LTB_STATE_ERROR;
   state_flag.Ltb_Error_Check_Flag = FBK_TRUE;
   state_flag.Ltb_Enable_Check     = FBK_TRUE;

   /** \action execute change the current state from ERROR to READY */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Ready */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Not_Available_To_Ready.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Not_Available_to_Ready)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_NOT_AVAILABLE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;
   state_flag.Ltb_Error_Check_Flag                     = FBK_FALSE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_FALSE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_FALSE;
   state_flag.Ltb_Speed_Check_Flag                     = FBK_FALSE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;

   /** \action execute change the current state from NOT_AVAILABLE to READY */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Ready */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether the Transition_From_Ready_to_Active.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Ready_to_Active)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_READY;
   state_flag.Ltb_Speed_Check_Flag                     = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_TRUE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_TRUE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;

   /** \action execute change the current state from READY to ACTIVE */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Active */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_ACTIVE);
}

/**
 * Check whether there is no Transition_From_Ready_to_Active.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Ready_to_Active_False_Driving_Direction)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_READY;
   state_flag.Ltb_Speed_Check_Flag                     = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_FALSE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_TRUE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;

   /** \action execute does not change the current state from READY to ACTIVE */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Active */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether there is no Transition_From_Ready_to_Active.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Ready_to_Active_False_Pwf)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_READY;
   state_flag.Ltb_Speed_Check_Flag                     = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_TRUE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_FALSE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;

   /** \action execute does not change the current state from READY to ACTIVE */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Active */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether there is no Transition_From_Ready_to_Active.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Ready_to_Active_False_Test_Mode)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_READY;
   state_flag.Ltb_Speed_Check_Flag                     = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_TRUE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_TRUE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_TRUE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;

   /** \action execute does not change the current state from READY to ACTIVE */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Active */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether there is no Transition_From_Ready_to_Active.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Ready_to_Active_Speed_Check_Hys)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_READY;
   state_flag.Ltb_Speed_Check_Flag                     = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_TRUE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_TRUE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_TRUE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;

   /** \action execute does not change the current state from READY to ACTIVE */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Active */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether there is no Transition_From_Ready_to_Active.
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Ready_to_Active_Speed_Check)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_READY;
   state_flag.Ltb_Speed_Check_Flag                     = FBK_FALSE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_TRUE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_TRUE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;

   /** \action execute does not change the current state from READY to ACTIVE */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Active */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether the Transition_From_Active_to_Ready.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Active_to_Ready_Speed)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_ACTIVE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_TRUE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_TRUE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;
   state_flag.Ltb_Error_Check_Flag                     = FBK_FALSE;

   /** \action execute change the current state from ACTIVE to READY */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Ready */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether there is Transition_From_Active_to_Ready.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Active_to_Ready_Driving_Direction_Fail)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_ACTIVE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_FALSE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_FALSE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_TRUE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;
   state_flag.Ltb_Error_Check_Flag                     = FBK_FALSE;

   /** \action execute change the current state from ACTIVE to READY */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Ready */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether there is Transition_From_Active_to_Ready.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Active_to_Ready_Driving_Pwf_Fail)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_ACTIVE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_FALSE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_TRUE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_FALSE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;
   state_flag.Ltb_Error_Check_Flag                     = FBK_FALSE;

   /** \action execute change the current state from ACTIVE to READY */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Ready */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether there is Transition_From_Active_to_Ready.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Active_to_Ready_Driving_Test_Mode_True)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_ACTIVE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_FALSE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_TRUE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_TRUE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_TRUE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;
   state_flag.Ltb_Error_Check_Flag                     = FBK_FALSE;

   /** \action execute change the current state from ACTIVE to READY */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Ready */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_READY);
}

/**
 * Check whether the Transition_From_Active_to_Error.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Active_to_Error)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_ACTIVE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_FALSE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_FALSE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_TRUE;
   state_flag.Ltb_Error_Check_Flag                     = FBK_TRUE;

   /** \action execute change the current state from ACTIVE to Error */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Error */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Active_to_Not_Available.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_State_Machine_Test, Ltb_State_Machine__Transition_From_Active_to_Not_Available)
{
   /** \arrange set ltb input to default */
   Ltb_Current_State                                   = LTB_STATE_ACTIVE;
   state_flag.Ltb_Speed_Check_Hys_Flag                 = FBK_TRUE;
   state_flag.Ltb_Check_Forward_Driving_Direction_Flag = FBK_FALSE;
   state_flag.Ltb_Check_Pwf_State_Driving_Flag         = FBK_FALSE;
   state_flag.Ltb_Test_Mode_Check_Flag                 = FBK_FALSE;
   state_flag.Ltb_Enable_Check                         = FBK_FALSE;
   state_flag.Ltb_Error_Check_Flag                     = FBK_TRUE;

   /** \action execute change the current state from ACTIVE to Not_Available */
   Ltb_State_Machine(&state_flag);

   /** \assert expect current state of LTB to be Not_Available */
   EXPECT_EQ(Ltb_Current_State, LTB_STATE_NOT_AVAILABLE);
}
