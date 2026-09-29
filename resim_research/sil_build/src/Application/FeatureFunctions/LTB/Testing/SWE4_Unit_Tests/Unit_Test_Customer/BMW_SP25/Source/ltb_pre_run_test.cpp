/**
 * @file ltb_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 LTB pre run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-46121}
 */

#include "ltb_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ltb_pre_run.c"
#include "pa_reuse.h"
}

/**
 * Check that mapping in ltb pre run is done correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ltb_Pre_Run_Test, Ltb_Init_Input__runs_correctly)
{
   /** \arrange set ltb input to something */
   ltb_input.f_ltb_enable = FBK_FALSE;

   /** \action execute pre run */
   Ltb_Init_Input(&ltb_input);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(ltb_input.f_ltb_enable);
}

/**
 * Check that mapping in ltb pre run is done correctly.
 * \uts{CSCSA-46122} \sdd{CSCSA-54002} \testtype{positive}
 */
TEST_F(Ltb_Pre_Run_Test, Ltb_Pre_Run__check_mapping_between_input_and_core_input)
{
   /** \arrange set ltb input to something */
   ltb_input.f_ltb_enable = FBK_TRUE;

   /** \action execute pre run */
   Ltb_Pre_Run(&ltb_instance, &ltb_input, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_FALSE(p_ltb_core_input->f_ltb_enable);
}

/**
 * Check that mapping in ltb pre run is done correctly.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_Pre_Run_Test, Ltb_Pre_Run__check_mapping_between_input_and_core_input_active_state)
{
   /** \arrange set ltb input to something */
   ltb_input.f_ltb_enable                                     = FBK_TRUE;
   *p_ltb_current_state                                       = LTB_STATE_ACTIVE;
   cals.k_ltb_bmw_sp25_v_ego_max                              = 2.0f;
   cals.k_ltb_bmw_sp25_v_ego_max_hys                          = 2.5f;
   p_vehicle_data->host_speed                                 = 4.0f;
   ltb_input.ltb_vehicle_parameters.vehicle_moving_direction  = BMW_CTB_VEH_MOVING_DIR_MOVES_FORWARD;
   ltb_input.ltb_vehicle_parameters.pwf_state                 = DRIVING;
   ltb_input.ltb_vehicle_parameters.status_roller_dynamometer = BMW_LTB_NO_DYNAMOMETER;
   ltb_input.ltb_vehicle_parameters.status_end_of_lne         = LTB_END_OF_LINE_MODE_NOT_SET;

   /** \action execute pre run */
   Ltb_Pre_Run(&ltb_instance, &ltb_input, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(p_ltb_core_input->f_ltb_enable);
}
