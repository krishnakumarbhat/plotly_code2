/**
 * @file ltb_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 LTB post run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "ltb_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ltb_post_run.c"
#include "ltb_pre_run.h"
#include "ltb_types.h"
#include "pa_reuse.h"
}

/**
 * Check that mapping in ltb post run is done correctly.
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Ltb_Post_Run_Test, Ltb_Post_Run__check_mapping_between_input_and_core_input_active_state)
{
   /** \arrange set ltb input to something */
   *p_ltb_current_state                               = LTB_STATE_ACTIVE;
   p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT]  = NO_ALERT;
   p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT] = ALERT_ACTIVE_LEVEL_1;
   p_ltb_core_output->ltb_ttb[FBK_SIDE_LEFT]          = 0.0f;
   p_ltb_core_output->ltb_ttb[FBK_SIDE_RIGHT]         = 2.5f;
   p_ltb_core_output->ltb_ttc[FBK_SIDE_LEFT]          = 0.0f;
   p_ltb_core_output->ltb_ttc[FBK_SIDE_RIGHT]         = 3.0f;

   /** \action execute pre run */
   Ltb_Post_Run(&ltb_instance, &ltb_output, &ltb_input);

   /** \assert expect mapping is done correctly */
   EXPECT_EQ(ltb_output.LTB_bmw_sp25_alert_left, p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_output.LTB_bmw_sp25_alert_right, p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ltb_output.LTB_bmw_sp25_ttc_left, p_ltb_core_output->ltb_ttc[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_output.LTB_bmw_sp25_ttc_right, p_ltb_core_output->ltb_ttc[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ltb_output.LTB_bmw_sp25_ttb_left, p_ltb_core_output->ltb_ttb[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_output.LTB_bmw_sp25_ttb_right, p_ltb_core_output->ltb_ttb[FBK_SIDE_RIGHT]);
}
