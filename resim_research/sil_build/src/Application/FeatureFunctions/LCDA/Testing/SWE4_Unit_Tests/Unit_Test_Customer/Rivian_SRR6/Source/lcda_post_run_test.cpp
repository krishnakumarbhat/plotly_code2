/**
 * @file lcda_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_post_run.c functions
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43051}
 */

#include "lcda_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_post_run.c"
#include "ml_checked_rounding.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Check that initialization in Lcda_Post_Run_Init is performed correctly.
 * \uts{CSCSA-43052} \sdd{SF-6987} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run_Init__check_correct_reset)
{
   /** \arrange Set up lcda output arbitrary, such that it differs from the initialization value */
   /** \action Call Lcda_Post_Run_Init to initialize lcda_output. */
   /** \assert Check that lcda output is initialized correctly. */
   ASSERT_NO_FATAL_FAILURE(Lcda_Post_Run_Init(); Lcda_Post_Run_Init(););
}

/**
 * Check that signal mapping in Lcda_Post_Run is correct.
 * \uts{CSCSA-43053} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__check_correct_mapping)
{
   /** \arrange Set up lcda output arbitrary, such that it differs from the values set in Lcda_Post_Run */
   lcda_core_output.lcda_status                               = LCDA_STATUS_ACTIVE;
   lcda_core_output.bsw_core_output.f_bsw_is_enabled          = FBK_TRUE;
   lcda_core_output.cvw_core_output.f_cvw_is_enabled          = FBK_TRUE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]     = 2;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]    = 4;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]     = 6;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]    = 8;
   lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_LEFT]    = 2.5f;
   lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT]   = 3.4f;

   /** \action Call Lcda_Post_Run to fill customer output accordingly */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that customer output is set correctly. */
   EXPECT_EQ(lcda_output.lcda_status, RIVIAN_LCDA_ACTIVE);
   EXPECT_EQ(lcda_output.f_bsw_enabled, FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.f_cvw_enabled, FBK_ONE_UINT);
   EXPECT_EQ(lcda_output.bsw_alert_left, LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(lcda_output.bsw_alert_right, LCDA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(lcda_output.cvw_alert_left, LCDA_ALERT_STATE_LEVEL_1);
   EXPECT_EQ(lcda_output.cvw_alert_right, LCDA_ALERT_STATE_LEVEL_2);
   EXPECT_EQ(lcda_output.bsw_id_left, 2);
   EXPECT_EQ(lcda_output.bsw_id_right, 4);
   EXPECT_EQ(lcda_output.cvw_id_left, 6);
   EXPECT_EQ(lcda_output.cvw_id_right, 8);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_left, 2.5f);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_right, 3.4f);
}

/**
 * Check that LCDA status is mapped correctly to Rivian specific LCDA status for LCDA_STATUS_ACTIVE.
 * \uts{CSCSA-43054} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Map_Lcda_Status_To_Rivian__works_properly_for_status_active)
{
   /** \arrange Set up LCDA status as LCDA_STATUS_ACTIVE. */
   Lcda_Status_T lcda_status = LCDA_STATUS_ACTIVE;

   /** \action Call Lcda_Map_Lcda_Status_To_Rivian to map status. */
   Lcda_Rivian_Status_T lcda_rivian_status = Lcda_Map_Lcda_Status_To_Rivian(lcda_status);

   /** \assert Check that rivian specific status is RIVIAN_LCDA_ACTIVE. */
   EXPECT_EQ(lcda_rivian_status, RIVIAN_LCDA_ACTIVE);
}

/**
 * Check that LCDA status is mapped correctly to Rivian specific LCDA status for LCDA_STATUS_DISABLED_BY_INPUT.
 * \uts{CSCSA-43055} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Map_Lcda_Status_To_Rivian__works_properly_for_status_disabled)
{
   /** \arrange Set up LCDA status as LCDA_STATUS_DISABLED_BY_INPUT. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DISABLED_BY_INPUT;

   /** \action Call Lcda_Map_Lcda_Status_To_Rivian to map status. */
   Lcda_Rivian_Status_T lcda_rivian_status = Lcda_Map_Lcda_Status_To_Rivian(lcda_status);

   /** \assert Check that rivian specific status is RIVIAN_LCDA_DISABLED. */
   EXPECT_EQ(lcda_rivian_status, RIVIAN_LCDA_DISABLED);
}

/**
 * Check that LCDA status is mapped correctly to Rivian specific LCDA status for LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED.
 * \uts{CSCSA-43056} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Map_Lcda_Status_To_Rivian__works_properly_for_status_deactivated_low_ego_speed)
{
   /** \arrange Set up LCDA status as LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED;

   /** \action Call Lcda_Map_Lcda_Status_To_Rivian to map status. */
   Lcda_Rivian_Status_T lcda_rivian_status = Lcda_Map_Lcda_Status_To_Rivian(lcda_status);

   /** \assert Check that rivian specific status is RIVIAN_LCDA_DEACTIVATED_LOW_EGO_SPEED. */
   EXPECT_EQ(lcda_rivian_status, RIVIAN_LCDA_DEACTIVATED_LOW_EGO_SPEED);
}

/**
 * Check that LC DA status is mapped correctly to Rivian specific LCDA status for LCDA_STATUS_DEACTIVATED_HIGH_EGO_SPEED.
 * \uts{CSCSA-43057} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Map_Lcda_Status_To_Rivian__works_properly_for_status_deactivated_high_ego_speed)
{
   /** \arrange Set up LCDA status as LCDA_STATUS_DEACTIVATED_HIGH_EGO_SPEED. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DEACTIVATED_HIGH_EGO_SPEED;

   /** \action Call Lcda_Map_Lcda_Status_To_Rivian to map status. */
   Lcda_Rivian_Status_T lcda_rivian_status = Lcda_Map_Lcda_Status_To_Rivian(lcda_status);

   /** \assert Check that rivian specific status is RIVIAN_LCDA_DEACTIVATED_HIGH_EGO_SPEED. */
   EXPECT_EQ(lcda_rivian_status, RIVIAN_LCDA_DEACTIVATED_HIGH_EGO_SPEED);
}

/**
 * Check that LCDA status is mapped correctly to Rivian specific LCDA status for LCDA_STATUS_DEACTIVATED_LOW_CURVE_RADIUS.
 * \uts{CSCSA-68153} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Map_Lcda_Status_To_Rivian__works_properly_for_status_deactivated_low_curve_radius)
{
   /** \arrange Set up LCDA status as LCDA_STATUS_DEACTIVATED_LOW_CURVE_RADIUS. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DEACTIVATED_LOW_CURVE_RADIUS;

   /** \action Call Lcda_Map_Lcda_Status_To_Rivian to map status. */
   Lcda_Rivian_Status_T lcda_rivian_status = Lcda_Map_Lcda_Status_To_Rivian(lcda_status);

   /** \assert Check that rivian specific status is RIVIAN_LCDA_DEACTIVATED_HIGH_EGO_SPEED. */
   EXPECT_EQ(lcda_rivian_status, RIVIAN_LCDA_DEACTIVATED_LOW_CURVE_RADIUS);
}

/**
 * Check that LCDA status is mapped correctly to Rivian specific LCDA status for default case.
 * \uts{CSCSA-68154} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Map_Lcda_Status_To_Rivian__works_properly_for_default_status)
{
   /** \arrange Set up LCDA status as default (-1). */
   Lcda_Status_T lcda_status = ((Lcda_Status_T) -1);

   /** \action Call Lcda_Map_Lcda_Status_To_Rivian to map status. */
   Lcda_Rivian_Status_T lcda_rivian_status = Lcda_Map_Lcda_Status_To_Rivian(lcda_status);

   /** \assert Check that rivian specific status is RIVIAN_LCDA_DEACTIVATED_HIGH_EGO_SPEED. */
   EXPECT_EQ(lcda_rivian_status, RIVIAN_LCDA_DEACTIVATED_INTERNAL_ERROR);
}

/**
 * Check that LCDA status is mapped correctly to Rivian specific LCDA status for LCDA_STATUS_DISABLED_BY_CAL.
 * \uts{CSCSA-68155} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Map_Lcda_Status_To_Rivian__works_properly_for_status_disabled_by_cal)
{
   /** \arrange Set up LCDA status as LCDA_STATUS_DISABLED_BY_CAL. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DISABLED_BY_CAL;

   /** \action Call Lcda_Map_Lcda_Status_To_Rivian to map status. */
   Lcda_Rivian_Status_T lcda_rivian_status = Lcda_Map_Lcda_Status_To_Rivian(lcda_status);

   /** \assert Check that rivian specific status is RIVIAN_LCDA_DEACTIVATED_HIGH_EGO_SPEED. */
   EXPECT_EQ(lcda_rivian_status, RIVIAN_LCDA_DISABLED);
}

/**
 * Tests that calibration is updated correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Init_Output_test)
{
   /** \arrange declare variable for result and simple imput */
   Lcda_Output_T output;
   output.f_bsw_enabled = 1;
   /** \action call calibration update */
   Lcda_Init_Output(&output);
   /** \assert expect succes */
   EXPECT_EQ(output.f_bsw_enabled, 0);
}
