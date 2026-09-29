/**
 * @file ced_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 CED pre run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41666}
 */

#include "ced_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_pre_run.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
}

/**
 * Check that mapping in ced pre run is done correctly.
 * \uts{CSCSA-41667} \sdd{SF-3402} \testtype{positive}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run__check_mapping_between_input_and_core_input_Active)
{
   /** \arrange set ced input to something */
   ced_input.f_ced_enable                                        = FBK_TRUE;
   ced_input.f_ced_front_mode                                    = FBK_TRUE;
   ced_input.f_ced_rear_mode                                     = FBK_TRUE;
   Ced_Current_State                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 3.5f;
   cals.k_ced_ego_abs_speed_max                                  = 20.0f;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = FBK_ZERO_F;
   /** \action execute pre run */
   Ced_Pre_Run(&ced_instance, &ced_input, &pt_output, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(ced_instance.core_input.f_ced_enable);
   EXPECT_TRUE(ced_instance.core_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_instance.core_input.f_ced_front_mode);
   EXPECT_TRUE(ced_instance.core_input.p_pa_data != nullptr);
}

/**
 * Check that mapping in ced pre run is done correctly.
 * \uts{CSCSA-70518} \sdd{SF-3402} \testtype{positive}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run__check_mapping_between_input_and_core_input_degraded)
{
   /** \arrange set ced input to something */
   ced_input.f_ced_enable                                        = FBK_TRUE;
   ced_input.f_ced_front_mode                                    = FBK_TRUE;
   ced_input.f_ced_rear_mode                                     = FBK_TRUE;
   Ced_Current_State                                             = CED_STATE_ACTIVE;
   ced_input.ced_input_bus_signals.f_ced_function_activation     = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_function_enabled      = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_function_fault_state    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_function_degraded       = FBK_TRUE;
   p_vehicle_data->host_speed                                    = 0.5f;
   ced_input.ced_input_bus_signals.ced_status_end_of_line        = BMW_CED_END_OF_LINE_SIGNAL_UNFILLED;
   ced_input.ced_input_bus_signals.ced_status_roller_dynamometer = BMW_CED_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   p_vehicle_data->host_speed                                    = FBK_ZERO_F;
   /** \action execute pre run */
   Ced_Pre_Run(&ced_instance, &ced_input, &pt_output, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(ced_instance.core_input.f_ced_enable);
   EXPECT_TRUE(ced_instance.core_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_instance.core_input.f_ced_front_mode);
   EXPECT_TRUE(ced_instance.core_input.p_pa_data != nullptr);
}

/**
 * Check that ced input is correctly initialized.
 * \uts{CSCSA-41668} \sdd{SF-3398} \testtype{positive}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run_Init__initialize_ced_input)
{
   /** \arrange set ced input to default */
   ced_input.f_ced_enable     = FBK_FALSE;
   ced_input.f_ced_front_mode = FBK_FALSE;
   ced_input.f_ced_rear_mode  = FBK_FALSE;

   /* Initialize customer pre run input*/
   Ced_Init_Input(&ced_input);

   /* \action execute ced pre run init  */
   Ced_Pre_Run_Init(&ced_instance);

   /** \assert expect ced input to be initialized correctly. */
   EXPECT_TRUE(ced_input.f_ced_enable);
   EXPECT_TRUE(ced_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_input.f_ced_front_mode);
   EXPECT_TRUE(object_data != nullptr);
   EXPECT_TRUE(p_vehicle_data != nullptr);
}