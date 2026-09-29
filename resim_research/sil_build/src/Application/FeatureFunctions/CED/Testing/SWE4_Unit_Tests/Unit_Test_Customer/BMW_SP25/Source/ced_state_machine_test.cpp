/**
 * @file ced_state_machine_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW Sp25 CED State Machine
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-70414}
 */

#include "ced_state_machine_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_core_calibration_t.h"
#include "ced_state_machine.c"
#include "fbk_macros.h"
}

/**
 * Check whether the Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_Front_axle.
 * \uts{CSCSA-70415} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_Front_axle)
{
   /** \arrange ced input bus signal values */
   boolean_T test_mode                                           = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;

   /** \action execute check Test Mode */
   test_mode = Ced_Is_Ced_Working_In_Test_Mode(&ced_input);

   /** \assert expect Test Mode is true */
   EXPECT_TRUE(test_mode);
}

/**
 * Check whether the Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_Back_axle.
 * \uts{CSCSA-70416} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_Back_axle)
{
   /** \arrange ced input bus signal values */
   boolean_T test_mode                                           = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_BACK_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;

   /** \action execute check Test Mode */
   test_mode = Ced_Is_Ced_Working_In_Test_Mode(&ced_input);

   /** \assert expect Test Mode is true */
   EXPECT_TRUE(test_mode);
}

/**
 * Check whether the Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_Two_axle.
 * \uts{CSCSA-70417} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_Two_axle)
{
   /** \arrange ced input bus signal values */
   boolean_T test_mode                                           = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_TWO_AXLE_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;

   /** \action execute check Test Mode */
   test_mode = Ced_Is_Ced_Working_In_Test_Mode(&ced_input);

   /** \assert expect Test Mode is true */
   EXPECT_TRUE(test_mode);
}

/**
 * Check whether the Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_Two_axle.
 * \uts{CSCSA-70418} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Is_Ced_Working_In_Test_Mode__end_line)
{
   /** \arrange ced input bus signal values */
   boolean_T test_mode                                           = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;

   /** \action execute check Test Mode */
   test_mode = Ced_Is_Ced_Working_In_Test_Mode(&ced_input);

   /** \assert expect Test Mode is true */
   EXPECT_TRUE(test_mode);
}

/**
 * Check whether the host speed is below treshold with hysteresis.
 * \uts{CSCSA-112891} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Is_Speed_Less_Than_Threshold__spped_is_below_treshold)
{
   /** \arrange host speed set below threshold */
   boolean_T result;
   p_vehicle_data->host_speed = p_vehicle_data->host_speed - EPSILON;

   /** \action execute check speed */
   result = Ced_Is_Speed_Less_Than_Threshold(p_vehicle_data, &ced_cal);

   /** \assert expect speed is lower then threshold */
   EXPECT_TRUE(result);
}

/**
 * Check whether the host speed is greater than treshold with hysteresis.
 * \uts{CSCSA-112892} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Is_Speed_Greater_Than_Threshold_With_Hysteresis__spped_is_greater_treshold)
{
   /** \arrange host speed set greater threshold */
   boolean_T result;
   p_vehicle_data->host_speed = ced_cal.k_ced_ego_abs_speed_max + ced_cal.k_bmw_ced_speed_max_hysteresis + 1.0f;

   /** \action execute check speed */
   result = Ced_Is_Speed_Greater_Than_Threshold_With_Hysteresis(p_vehicle_data, &ced_cal);

   /** \assert expect speed is greater then threshold with hysteresis */
   EXPECT_TRUE(result);
}

/**
 * Check whether the host speed is lower than treshold with hysteresis.
 * \uts{CSCSA-112893} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Is_Speed_Greater_Than_Threshold_With_Hysteresis__spped_is_lower_treshold)
{
   /** \arrange host speed set lower threshold */
   boolean_T result;
   p_vehicle_data->host_speed = -ced_cal.k_ced_ego_abs_speed_max - ced_cal.k_bmw_ced_speed_max_hysteresis;

   /** \action execute check speed */
   result = Ced_Is_Speed_Greater_Than_Threshold_With_Hysteresis(p_vehicle_data, &ced_cal);

   /** \assert expect speed is lower then threshold with hysteresis */
   EXPECT_FALSE(result);
}


/**
 * Check whether the Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_Two_axle.
 * \uts{CSCSA-70419} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Is_Ced_Working_In_Test_Mode__roller_dynamoter_false)
{
   /** \arrange ced input bus signal values */
   boolean_T test_mode                                           = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;

   /** \action execute check Test Mode */
   test_mode = Ced_Is_Ced_Working_In_Test_Mode(&ced_input);

   /** \assert expect Test Mode is true */
   EXPECT_FALSE(test_mode);
}

/**
 * Check whether the Transition_From_Not_Available_To_Ready when Function enable to True .
 * \uts{CSCSA-70420} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Not_Available_To_Ready)
{
   /** \arrange current state value and set function enable as True */
   ced_current_state                                             = CED_STATE_NOTAVAILABLE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;

   /** \action execute change the current state from not available to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Not_Available_To_not_Ready when Function enable to True .
 * \uts{CSCSA-70421} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Not_Available_To_Ready_sfe_function)
{
   /** \arrange current state value and set function enable as True */
   ced_current_state                                             = CED_STATE_NOTAVAILABLE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;

   /** \action execute change the current state from not available to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Not_Available_To_not_Ready when Function enable to True .
 * \uts{CSCSA-70422} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Not_Available_To_Ready_Test_Mode)
{
   /** \arrange current state value and set function enable as True */
   ced_current_state                                             = CED_STATE_NOTAVAILABLE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;

   /** \action execute change the current state from not available to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Not_Available_To_not_Ready when Function enable to True .
 * \uts{CSCSA-70423} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Not_Available_To_Ready_Test_Mode_enabled)
{
   /** \arrange current state value and set function enable as True */
   ced_current_state                                             = CED_STATE_NOTAVAILABLE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;

   /** \action execute change the current state from not available to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_NOTAVAILABLE);
}

/**
 * Check whether the Transition_From_Ready_To_Not_Available when Function enable to false .
 * \uts{CSCSA-70424} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Not_Available)
{
   /** \arrange current state value and set function enable as false */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_FALSE;

   /** \action execute change the current state from ready to Not Available */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Not_Available */
   EXPECT_EQ(ced_current_state, CED_STATE_NOTAVAILABLE);
}

/**
 * Check whether the Transition_From_Ready_To_Not_Available when Function enable to false .
 * \uts{CSCSA-70425} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Not_Available_function_enabled)
{
   /** \arrange current state value and set function enable as false */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;

   /** \action execute change the current state from ready to Not Available */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Not_Available */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Ready_To_Not_Available when Function enable to false .
 * \uts{CSCSA-70426} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Not_Available_Test_mode)
{
   /** \arrange current state value and set function enable as false */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_FALSE;

   /** \action execute change the current state from ready to Not Available */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Not_Available */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}


/**
 * Check whether the Transition_From_Ready_To_Active when velocity lessthan ego speed is as expected
 * \uts{CSCSA-70427} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Active)
{
   /** \arrange ced input value and set ego speed greatherthan Vehicle velocity */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = 2.5f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.5f;

   /** \action execute change the current state from ready to Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Active */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Ready_To_Active when velocity lessthan ego speed is as expected
 * \uts{CSCSA-70428} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Active_function_activation)
{
   /** \arrange ced input value and set ego speed greatherthan Vehicle velocity */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = 9.5f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.5f;

   /** \action execute change the current state from ready to Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Active */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Ready_To_Active when velocity lessthan ego speed is as expected
 * \uts{CSCSA-70429} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Active_Test_mode)
{
   /** \arrange ced input value and set ego speed greatherthan Vehicle velocity */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = 9.5f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.5f;

   /** \action execute change the current state from ready to Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Active */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Ready_To_Active when velocity lessthan ego speed is as expected
 * \uts{CSCSA-70430} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Active_speed)
{
   /** \arrange ced input value and set ego speed greatherthan Vehicle velocity */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = 15.5f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.5f;

   /** \action execute change the current state from ready to Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Active */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Ready_To_Active when velocity lessthan ego speed is as expected
 * \uts{CSCSA-70431} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test,
       Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_not_Active_when_velocity_greatherthan_ego_speed)
{
   /** \arrange ced input value and set ego speed less than Vehicle velocity */
   ced_current_state                                         = CED_STATE_READY;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled  = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation = FBK_TRUE;
   p_vehicle_data->host_speed                                = 0.9f;
   ced_cal.k_ced_ego_abs_speed_max                           = 3.5f;

   /** \action execute change the current state from ready to not Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be states other than Active */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Ready_To_Active when velocity lessthan ego speed is as expected
 * \uts{CSCSA-70432} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_not_Active_when_function_not_activated)
{
   /** \arrange ced input value and set ego speed lessthan Vehicle velocity and not Activated */
   ced_current_state                                         = CED_STATE_READY;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled  = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation = FBK_FALSE;
   p_vehicle_data->host_speed                                = 0.5f;

   /** \action execute change the current state from ready to not Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be states otherthan Active */
   EXPECT_NE(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Ready_To_Error is as expected function not activated
 * \uts{CSCSA-70433} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Error)
{
   /** \arrange current state value and ced input which will transform Ready state to Error */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 10.9f;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from ready to Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Error */
   EXPECT_EQ(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Ready_To_Error is as expected function activated
 * \uts{CSCSA-70434} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Error_fault_state)
{
   /** \arrange current state value and ced input which will transform Ready state to Error */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 10.9f;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from ready to someother states otherthan Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be states otherthan Error */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Ready_To_Error is as expected function not activated and with less velocity value
 * \uts{CSCSA-70435} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Error_function_activated)
{
   /** \arrange current state value and ced input which will transform Ready state to Error */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 10.9f;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from ready to Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Error */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Ready_To_Error is as expected function not activated
 * \uts{CSCSA-70436} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Ready_to_Error_Test_mode)
{
   /** \arrange current state value and ced input which will transform Ready state to Error */
   ced_current_state                                             = CED_STATE_READY;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 10.9f;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from ready to Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Error */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Active_To_Ready is as expected when Speed Greater than or less than threshold with hysteresis
 * and function activated expected \uts{CSCSA-70437} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Ready_function_activated)
{

   /** \arrange ced input bus signal values and set Vehicle velocity */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 10.9f;

   /** \action execute change the current state from Active to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}


/**
 * Check whether the Transition_From_Active_To_Ready is as expected when Speed Greater than or less than threshold with hysteresis
 * and function not activated expected \uts{CSCSA-70438} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test,
       Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Ready_function_not_activated_fault_state_true)
{
   /** \arrange ced input bus signal values and set Vehicle velocity */
   ced_current_state                                           = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state  = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation   = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation = FBK_FALSE;
   p_vehicle_data->host_speed                                  = 0.5f;

   /** \action execute change the current state from Active to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Ready */
   EXPECT_NE(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Active_To_Ready is as expected when Speed Greater than or less than threshold with hysteresis
 * and function not activated expected \uts{CSCSA-70439} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Ready_Hysteresis)
{
   /** \arrange ced input bus signal values and set Vehicle velocity */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 30.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 3.5f;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   /** \action execute change the current state from Active to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Active_To_Ready is as expected when Speed Greater than or less than threshold with hysteresis
 * and function activated expected \uts{CSCSA-70440} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Ready_Test_mode)
{

   /** \arrange ced input bus signal values and set Vehicle velocity */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 10.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 3.5f;

   /** \action execute change the current state from Active to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Active_To_Ready is as expected when Speed Greater than or less than threshold with hysteresis
 * and function activated expected \uts{CSCSA-70441} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_false_function_activation)
{

   /** \arrange ced input bus signal values and set Vehicle velocity */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 5.9f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 3.5f;
   /** \action execute change the current state from Active to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Ready is as expected when Speed Greater than or less than threshold with hysteresis
 * and function activated expected \uts{CSCSA-70442} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_false_hystersis)
{

   /** \arrange ced input bus signal values and set Vehicle velocity */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 10.9f;
   ced_cal.k_ced_ego_abs_speed_max                               = 5.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 3.5f;

   /** \action execute change the current state from Active to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Ready is as expected when Speed Greater than or less than threshold with hysteresis
 * and function activated expected \uts{CSCSA-70443} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_false_roller_dynamometer)
{

   /** \arrange ced input bus signal values and set Vehicle velocity */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_FALSE;
   p_vehicle_data->host_speed                                    = 0.5f;

   /** \action execute change the current state from Active to ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Error_function_fault_state_true is false is as expected.
 * \uts{CSCSA-70444} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Error_function_fault_state_true)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from Active to Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Error */
   EXPECT_EQ(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Active_To_Error_function_fault_state is false is as expected.
 * \uts{CSCSA-70445} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Error_function_fault_state_false)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;

   /** \action execute change the current state from Active to not Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Error */
   EXPECT_NE(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Active_To_Error__function_activation_disabled .
 * \uts{CSCSA-70446} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Error_function_activation_disabled)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;

   /** \action execute change the current state from Active to not Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Error */
   EXPECT_NE(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Active_To_Error_function_activation_disabled_fault_state_enabled.
 * \uts{CSCSA-70447} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test,
       Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Error_function_activation_disabled_fault_state_enabled)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;

   /** \action execute change the current state from Active to not Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Error */
   EXPECT_NE(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Active_To_Error_function_fault_state_true is false is as expected.
 * \uts{CSCSA-70448} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Error_Test_mode)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from Active to Error */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Error */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Degraded.
 * \uts{CSCSA-70449} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Degraded)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = 0.5f;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;


   /** \action execute change the current state from Active to Degraded */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Degraded */
   EXPECT_EQ(ced_current_state, CED_STATE_DEGRADED);
}

/**
 * Check whether the Transition_From_Active_To_Degraded_fault_state_disabled.
 * \uts{CSCSA-70450} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Degraded_Function_Degraded)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = 0.5f;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from Active to not Degraded */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be degraded */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Degraded_fault_state_enabled.
 * \uts{CSCSA-70451} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Degraded_fault_state_enabled)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = 0.5f;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from Active to Degraded */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Degraded */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Degraded_fault_state_enabled_activation_enabled.
 * \uts{CSCSA-70452} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Active_To_Degraded_Test_mode)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   p_vehicle_data->host_speed                                    = 0.5f;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;

   /** \action execute change the current state from Active to not Degraded */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be degraded */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}


/**
 * Check whether the Transition_From_Degraded_To_Ready is as expected
 * \uts{CSCSA-70453} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Ready)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   p_vehicle_data->host_speed                                    = 30.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 3.5f;

   /** \action execute change the current state from degraded to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Degraded_To_Ready_activation_disabled.
 * \uts{CSCSA-70454} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Ready_activation_disabled)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   p_vehicle_data->host_speed                                    = 30.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 3.5f;

   /** \action execute change the current state from degraded to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Degraded_To_Ready_activation_disabled_greater_velocity.
 * \uts{CSCSA-70455} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test,
       Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Ready_activation_disabled_greater_velocity)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   p_vehicle_data->host_speed                                    = 30.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 5.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 4.5f;

   /** \action execute change the current state from degraded to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Degraded_To_Ready_activation_enabled_greater_velocity.
 * \uts{CSCSA-70456} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Ready_fault_state)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 30.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 3.5f;

   /** \action execute change the current state from degraded to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Degraded_To_Ready is as expected
 * \uts{CSCSA-70457} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Ready_Test_mode)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   p_vehicle_data->host_speed                                    = 30.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 3.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 3.5f;

   /** \action execute change the current state from degraded to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be ready */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Degraded_To_Active is as expected
 * \uts{CSCSA-70458} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Active)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 5.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 10.0f;

   /** \action execute change the current state from degraded to not Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Active */
   EXPECT_EQ(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Degraded_To_Active_Function_Activation.
 * \uts{CSCSA-70459} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Active_Function_Activation)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 5.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 10.0f;

   /** \action execute change the current state from degraded to not Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Active */
   EXPECT_NE(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Degraded_To_Active_speed.
 * \uts{CSCSA-70460} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Active_speed)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 8.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 5.0f;
   ced_cal.k_bmw_ced_speed_max_hysteresis                        = 4.8f;

   /** \action execute change the current state from degraded to not Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Active */
   EXPECT_EQ(ced_current_state, CED_STATE_DEGRADED);
}

/**
 * Check whether the Transition_From_Degraded_To_Active_Fault_State.
 * \uts{CSCSA-70461} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Active_Fault_State)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 5.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 10.0f;

   /** \action execute change the current state from degraded to not Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Active */
   EXPECT_NE(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Degraded_To_Active_Function_Degraded.
 * \uts{CSCSA-70462} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Active_Function_Degraded)
{

   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 5.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 10.0f;

   /** \action execute change the current state from degraded to not Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Active */
   EXPECT_NE(ced_current_state, CED_STATE_ACTIVE);
}
/**
 * Check whether the Transition_From_Degraded_To_Active_Test_Mode is as expected
 * \uts{CSCSA-70463} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Active_Test_Mode)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_function_activation   = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 5.0f;
   ced_cal.k_ced_ego_abs_speed_max                               = 10.0f;

   /** \action execute change the current state from degraded to not Active */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE not to be Active */
   EXPECT_NE(ced_current_state, CED_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Degraded_To_Error is as expected when activation_and_fault_state_true.
 * \uts{CSCSA-70464} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Error)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;

   /** \action execute change the current state from degraded to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Error */
   EXPECT_EQ(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Degraded_To_Error is as expected when activation_false_and_fault_state_true
 * \uts{CSCSA-70465} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Error_fault_state_true)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;

   /** \action execute change the current state from degraded to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Error */
   EXPECT_NE(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Degraded_To_Error is as expected when activation_true_and_fault_state_false
 * \uts{CSCSA-70466} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Error_Function_Activation)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;

   /** \action execute change the current state from degraded to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be not an Error state */
   EXPECT_NE(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Degraded_To_Error is as expected when activation_and_fault_state_false
 * \uts{CSCSA-70467} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Degraded_To_Error_Test_Mode)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_DEGRADED;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;

   /** \action execute change the current state from degraded to not Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be not an Error state */
   EXPECT_NE(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Error_To_Ready is as expected when activation_and_fault_state_false
 * \uts{CSCSA-70468} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Error_To_Ready)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ERROR;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;

   /** \action execute change the current state from Error to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready state */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Error_To_Ready is as expected when activation_true
 * \uts{CSCSA-70469} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Error_To_Ready_Fault_State)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ERROR;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_FALSE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;

   /** \action execute change the current state from Error to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready state */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Error_To_Ready is as expected when fault_state_true
 * \uts{CSCSA-70470} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Error_To_Ready_Function_Activation)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ERROR;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_SET;

   /** \action execute change the current state from Error to Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be Ready state */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}

/**
 * Check whether the Transition_From_Error_To_Ready is as expected when Test mode false
 * \uts{CSCSA-70471} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Error_To_Ready_Test_mode)
{
   /** \arrange ced input bus signal values */
   ced_current_state                                             = CED_STATE_ERROR;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;

   /** \action execute change the current state from Error to otherthan Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be not Ready state */
   EXPECT_EQ(ced_current_state, CED_STATE_READY);
}


/**
 * Check whether the Transition From Error to not Ready is as expected when activation_and_fault_state_true
 * \uts{CSCSA-70472} \sdd{} \testtype{negative}
 */
TEST_F(Ced_State_Machine_Test, Ced_Set_Current_Ced_Functional_State__Transition_From_Error_To_Ready_All_Fault)
{
   /** \arrange ced input bus signal values to check the transition from Error */
   ced_current_state                                             = CED_STATE_ERROR;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;

   /** \action execute change the current state from Error to otherthan Ready */
   Ced_Set_Current_Ced_Functional_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE to be not Ready state */
   EXPECT_EQ(ced_current_state, CED_STATE_ERROR);
}

/**
 * Check whether the Transition is as expected when speed is below threshold and activation and enabled state astrue on
 * dynamometer. \uts{CSCSA-112894} \sdd{} \testtype{positive}
 */
TEST_F(Ced_State_Machine_Test, Ced_State_Machine_Transitions_From_Ready_State__default_state_after_transition)
{
   /** \arrange ced input bus signal values to check the transition from Error */
   p_vehicle_data->host_speed                                    = ced_cal.k_ced_ego_abs_speed_max - EPSILON;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_FRONT_AXLE_ON_DYNAMOMETER;


   /** \action execute change the current state */
   Ced_State_Machine_Transitions_From_Ready_State(&ced_input, &ced_cal, &ced_current_state, p_vehicle_data);

   /** \assert expect current state of SFE is not changed */
   EXPECT_EQ(ced_current_state, CED_STATE_NOTAVAILABLE);
}
