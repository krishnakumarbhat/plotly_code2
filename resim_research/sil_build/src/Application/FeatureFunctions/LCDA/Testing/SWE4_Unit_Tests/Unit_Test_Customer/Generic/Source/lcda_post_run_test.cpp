/**
 * @file lcda_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic LCDA post run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-122581}
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
 * Check that outputs are passed in post run correctly from core output.
 * \uts{CSCSA-127634} \sdd{SF-6838} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__check_output_creation)
{
   /** \arrange Set up LCDA core output to non default values. */
   lcda_core_output.lcda_status                      = LCDA_STATUS_ACTIVE;
   lcda_core_output.bsw_core_output.f_bsw_is_enabled = FBK_TRUE;
   lcda_core_output.cvw_core_output.f_cvw_is_enabled = FBK_TRUE;
   lcda_core_output.slc_core_output.f_slc_is_enabled = FBK_TRUE;

   for (uint8_t side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      lcda_core_output.bsw_core_output.bsw_alert[side_index]     = LCDA_ALERT_STATE_LEVEL_2;
      lcda_core_output.bsw_core_output.bsw_id[side_index]        = 2u;
      lcda_core_output.bsw_core_output.bsw_unique_id[side_index] = 2u;

      lcda_core_output.cvw_core_output.cvw_alert[side_index]     = LCDA_ALERT_STATE_LEVEL_2;
      lcda_core_output.cvw_core_output.cvw_id[side_index]        = 3u;
      lcda_core_output.cvw_core_output.cvw_unique_id[side_index] = 3u;
      lcda_core_output.cvw_core_output.cvw_ttc[side_index]       = 1.0f;

      lcda_core_output.slc_core_output.slc_alert[side_index]            = FBK_TRUE;
      lcda_core_output.slc_core_output.slc_id[side_index]               = 4u;
      lcda_core_output.slc_core_output.slc_unique_id[side_index]        = 4u;
      lcda_core_output.slc_core_output.slc_lat_ttc[side_index]          = 1.0f;
      lcda_core_output.slc_core_output.slc_lane_change_prob[side_index] = 0.5f;
   }

   /** \action Call function to test. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that LCDA outputs are set correctly. */
   EXPECT_EQ(lcda_output.lcda_status, LCDA_STATUS_ACTIVE);
   EXPECT_TRUE(lcda_output.f_bsw_enabled);
   EXPECT_TRUE(lcda_output.f_cvw_enabled);
   EXPECT_TRUE(lcda_output.f_slc_enabled);

   for (uint8_t side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      EXPECT_EQ(lcda_output.bsw_alert[side_index], LCDA_ALERT_STATE_LEVEL_2);
      EXPECT_EQ(lcda_output.bsw_id[side_index], 2u);
      EXPECT_EQ(lcda_output.bsw_unique_id[side_index], 2u);

      EXPECT_EQ(lcda_output.cvw_alert[side_index], LCDA_ALERT_STATE_LEVEL_2);
      EXPECT_EQ(lcda_output.cvw_id[side_index], 3u);
      EXPECT_EQ(lcda_output.cvw_unique_id[side_index], 3u);
      EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_s[side_index], 1.0f);

      EXPECT_EQ(lcda_output.slc_alert[side_index], FBK_TRUE);
      EXPECT_EQ(lcda_output.slc_id[side_index], 4u);
      EXPECT_EQ(lcda_output.slc_unique_id[side_index], 4u);
      EXPECT_FLOAT_EQ(lcda_output.slc_ttc_s[side_index], 1.0f);
      EXPECT_FLOAT_EQ(lcda_output.slc_lane_change_probability[side_index], 0.5f);
   }
}

/**
 * Tests that output is initialized correctly.
 * \uts{CSCSA-185799} \sdd{CSCSA-196458} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Init_Output__test)
{
   /** \arrange declare variable for result and simple imput */
   Lcda_Output_T output;
   output.lcda_status = LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR;
   /** \action call output initialization */
   Lcda_Init_Output(&output);
   /** \assert expect succes */
   EXPECT_EQ(output.lcda_status, LCDA_STATUS_DISABLED_BY_INPUT);
}