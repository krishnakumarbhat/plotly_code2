/**
 * @file ta_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic TA post run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-125717}
 */

#include "ta_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "pa_shared_types.h"
#include "ta_post_run.c"

#include "fbk_macros.h"
}


/**
 * Check that post run initialization routine resets the generic output accordingly.
 * \uts{CSCSA-125718} \sdd{SF-8650} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run_Init__check_no_fatal_failure)
{
   /** \arrange */
   /** \action Run function to test */
   /** \assert Verify output is set to default */
   EXPECT_NO_FATAL_FAILURE(Ta_Post_Run_Init(););
}

/**
 * Verify if copying variables from core output and tracker is valid.
 * \uts{CSCSA-125719} \sdd{SF-8548} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Post_Run_Test, Ta_Post_Run__math_from_core_and_tracker)
{
   uint8_t side_index;

   /** \arrange set output to non default */
   ta_input.f_ta_enable = FBK_TRUE;

   ta_core_output.ta_f_vehicle_state_relevant = FBK_TRUE;
   ta_core_output.ta_most_critical_side       = 1u;
   ta_core_output.ta_n_valid_objects          = 1u;
   ta_core_output.ta_n_relevant_objects       = 1u;
   ta_core_output.ta_n_critical_objects       = 1u;

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      ta_core_output.ta_waypoint_at_collision[side_index].x = 3.0f;
      ta_core_output.ta_waypoint_at_collision[side_index].y = 3.0f;
      ta_core_output.ta_ttc[side_index]                     = 50.0f;
      ta_core_output.ta_ttp[side_index]                     = 50.0f;
      ta_core_output.ta_ttb[side_index]                     = 50.0f;
      ta_core_output.ta_decel_estimate[side_index]          = 1.0f;
      ta_core_output.ta_distance[side_index]                = 50.0f;

      ta_core_output.ta_alert_level[side_index] = TA_ALERT_STATE_LEVEL_1;
      ta_core_output.ta_id[side_index]          = 1u;
      ta_core_output.ta_index[side_index]       = 1u;

      ta_core_output.ta_f_obj_in_danger_zone[side_index] = FBK_TRUE;
      ta_core_output.ta_f_obj_in_info_zone[side_index]   = FBK_TRUE;
      ta_core_output.ta_f_obj_in_wing_zone[side_index]   = FBK_TRUE;
   }

   /** \action Run Ta_Post_Run for mapping wit core output and tracker data. */
   Ta_Post_Run(&ta_instance, &ta_input, &ta_output);


   /** \assert Verify output is set to default */
   EXPECT_TRUE(ta_output.f_ta_enable);

   EXPECT_TRUE(ta_output.ta_f_vehicle_state_relevant);
   EXPECT_EQ(ta_output.ta_most_critical_side, FBK_SIDE_RIGHT);
   EXPECT_EQ(ta_output.ta_n_valid_objects, 1u);
   EXPECT_EQ(ta_output.ta_n_relevant_objects, 1u);
   EXPECT_EQ(ta_output.ta_n_critical_objects, 1u);

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      EXPECT_FLOAT_EQ(ta_output.ta_object[side_index].ta_waypoint_at_collision_m.x, 3.0f);
      EXPECT_FLOAT_EQ(ta_output.ta_object[side_index].ta_waypoint_at_collision_m.y, 3.0f);
      EXPECT_FLOAT_EQ(ta_output.ta_object[side_index].ta_ttc_s, 50.0f);
      EXPECT_FLOAT_EQ(ta_output.ta_object[side_index].ta_ttp_s, 50.0f);
      EXPECT_FLOAT_EQ(ta_output.ta_object[side_index].ta_ttb_s, 50.0f);
      EXPECT_FLOAT_EQ(ta_output.ta_object[side_index].ta_decel_estimate_mps2, 1.0f);
      EXPECT_FLOAT_EQ(ta_output.ta_object[side_index].ta_distance_m, 50.0f);

      EXPECT_EQ(ta_output.ta_alert_level[side_index], TA_ALERT_STATE_LEVEL_1);
      EXPECT_EQ(ta_output.ta_object[side_index].ta_id, 1u);
      EXPECT_EQ(ta_output.ta_object[side_index].ta_index, 1u);

      EXPECT_TRUE(ta_output.ta_object[side_index].ta_f_obj_in_danger_zone);
      EXPECT_TRUE(ta_output.ta_object[side_index].ta_f_obj_in_info_zone);
      EXPECT_TRUE(ta_output.ta_object[side_index].ta_f_obj_in_wing_zone);
   }
}
