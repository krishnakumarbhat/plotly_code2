/**
 * @file ced_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Nissan_SRR6 CED post run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41684}
 */

#include "ced_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_post_run.c"
#include "ced_types.h"
#include "fbk_macros.h"
}

/**
 * Check that post run reset routine resets the nissan_srr6 output accordingly.
 * \uts{CSCSA-41686} \sdd{SF-3468} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Reset_Output__check_initialization_routine)
{
   /** \arrange set output to non default */
   ced_output.CED_alert_right                    = OSE_ALERT_LEVEL_1;
   ced_output.CED_alert_left                     = OSE_ALERT_LEVEL_1;
   ced_output.f_ced_enable                       = 1u;
   ced_output.CED_ttc_right                      = 1.0f;
   ced_output.CED_id_right                       = 1u;
   ced_output.CED_object_predicted_lat_pos_right = 1.0f;
   ced_output.CED_ttc_left                       = 1.0f;
   ced_output.CED_id_left                        = 1u;
   ced_output.CED_object_predicted_lat_pos_left  = 1.0f;

   /** \action Run function to test */
   Ced_Reset_Output(&ced_output);

   /** \assert Verify output is set to default */
   EXPECT_EQ(ced_output.CED_alert_right, OSE_NO_ALERT);
   EXPECT_EQ(ced_output.CED_alert_left, OSE_NO_ALERT);
   EXPECT_EQ(ced_output.f_ced_enable, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.CED_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.CED_id_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.CED_object_predicted_lat_pos_right, CED_INVALID_DISTANCE);
   EXPECT_FLOAT_EQ(ced_output.CED_ttc_left, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.CED_ttc_right, CED_INVALID_TIME);
   EXPECT_FLOAT_EQ(ced_output.CED_object_predicted_lat_pos_left, CED_INVALID_DISTANCE);
}
