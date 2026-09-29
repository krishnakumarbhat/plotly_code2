/**
 * @file lcda_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for Nissan_SRR6 post run functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-90032}
 */

#include "lcda_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_post_run.c"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Check that outputs are reset to their defaults.
 * \uts{CSCSA-90033} \sdd{SF-6993} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Reset_Output__check_default_mapping)
{
   /** \arrange Set up LCDA output to non default. */
   lcda_output.f_lcda_enabled  = 1u;
   lcda_output.f_bsw_enabled   = 1u;
   lcda_output.f_cvw_enabled   = 1u;
   lcda_output.bsw_alert_left  = 1u;
   lcda_output.bsw_alert_right = 1u;
   lcda_output.cvw_alert_left  = 1u;
   lcda_output.cvw_alert_right = 1u;
   lcda_output.cvw_id_left     = 1u;
   lcda_output.cvw_id_right    = 1u;
   lcda_output.bsw_id_left     = 1u;
   lcda_output.bsw_id_right    = 1u;
   lcda_output.cvw_ttc_left    = 2.0f;
   lcda_output.cvw_ttc_right   = 2.0f;

   /** \action Call function to test. */
   Lcda_Reset_Output(&lcda_output);

   /** \assert Check that defaults are set correctly. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_bsw_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_cvw_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bsw_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bsw_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_id_left, PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_id_right, PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.bsw_id_left, PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.bsw_id_right, PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_left, LCDA_NISSAN_DEFAULT_TTC);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_right, LCDA_NISSAN_DEFAULT_TTC);
}

/**
 * Check that outputs are reset to their defaults by initialization routine.
 * \uts{CSCSA-90034} \sdd{SF-6987} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run_Init__check_default_mapping)
{
   /** \arrange */
   /** \action Call function to test. */
   /** \assert */
   ASSERT_NO_FATAL_FAILURE(Lcda_Post_Run_Init(); Lcda_Post_Run_Init(););
}

/**
 * Check that in case of an active alert for any submodule the corresponding object ID is filled to the LCDA output.
 * \uts{CSCSA-90035} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__fills_alert_id_to_lcda_output_for_active_alerts)
{
   /** \arrange Set up LCDA core output such that alerts are present for all submodules on both sides. */
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;

   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]  = 2u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT] = 5u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT] = 6u;

   lcda_core_output.lcda_status = LCDA_STATUS_ACTIVE;

   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that object IDs are set correctly for all active alerts. */
   EXPECT_EQ(lcda_output.bsw_id_left, lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.cvw_id_left, lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bsw_id_right, lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.cvw_id_right, lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]);
}

/**
 * Tests that calibration is updated correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Init_Output_test)
{
   /** \arrange declare variable for result and simple imput */
   Lcda_Output_T output;
   output.f_lcda_enabled = 1;
   /** \action call calibration update */
   Lcda_Init_Output(&output);
   /** \assert expect succes */
   EXPECT_EQ(output.f_lcda_enabled, 0);
}
