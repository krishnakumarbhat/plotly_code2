/**
 * @file ltb_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic RECW post run tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-123115}
 */

#include "ltb_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ltb_post_run.c"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Test that RECW output is filled from core output.
 * \uts{CSCSA-123116} \sdd{CSCSA-54001} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Post_Run_Test, Ltb_Post_Run__is_filled_properly)
{
   /** \arrange Set core output with level 1 alert and tracker data. */

   /** \action Call Ltb_Output to update output of RECW. */
   Ltb_Post_Run(&ltb_instance, &ltb_output, &ltb_input);

   /** \assert Verify that RECW output is filled correctly with tracker data and alert. */
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_LEFT].ltb_id, ltb_core_output.ltb_id[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_RIGHT].ltb_id, ltb_core_output.ltb_id[FBK_SIDE_RIGHT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_LEFT].ltb_ttc_s, ltb_core_output.ltb_ttc[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_RIGHT].ltb_ttc_s, ltb_core_output.ltb_ttc[FBK_SIDE_RIGHT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_LEFT].ltb_ttb_s, ltb_core_output.ltb_ttb[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_RIGHT].ltb_ttb_s, ltb_core_output.ltb_ttb[FBK_SIDE_RIGHT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_LEFT].ltb_decel_estimate_mps2, ltb_core_output.ltb_decel_estimate[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_RIGHT].ltb_decel_estimate_mps2, ltb_core_output.ltb_decel_estimate[FBK_SIDE_RIGHT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_LEFT].ltb_distance_m, ltb_core_output.ltb_distance[FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(ltb_output.ltb_object[FBK_SIDE_RIGHT].ltb_distance_m, ltb_core_output.ltb_distance[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ltb_output.ltb_alert_level[FBK_SIDE_LEFT], ltb_core_output.ltb_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(ltb_output.ltb_alert_level[FBK_SIDE_RIGHT], ltb_core_output.ltb_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(ltb_output.ltb_most_critical_side, ltb_core_output.ltb_most_critical_side);
}
