/**
 * @file ced_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Ford_DAT2_1 CED post run
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41669}
 */

#include "ced_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_post_run.c"
#include "ced_types.h"
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_reuse.h"
}


/**
 * Check that ced output is reset to its default.
 * \uts{CSCSA-41671} \sdd{SF-3474} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Reset_Output__resets_ced_forddat21_output)
{
   /** \arrange Set up output to non default. */
   for (uint8_t side_idx = 0u; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      ced_output.ced_alert[side_idx]          = (uint8_t) CED_ALERT_ACTIVE_LEVEL_1;
      ced_output.ced_id[side_idx]             = 1u;
      ced_output.ced_ttc[side_idx]            = 1.0f;
      ced_output.ced_ttp[side_idx]            = 1.0f;
      ced_output.ced_object_speed[side_idx]   = 1.0f;
      ced_output.ced_object_heading[side_idx] = 1.0f;
      ced_output.ced_object_length[side_idx]  = 1.0f;
      ced_output.ced_object_width[side_idx]   = 1.0f;
   }
   /** \action Call function to test */
   Ced_Reset_Output(&ced_output);

   /** \assert Verify that output is reset accordingly. */
   for (uint8_t side_idx = 0u; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      EXPECT_EQ(ced_output.ced_alert[side_idx], (uint8_t) CED_NO_ALERT);
      EXPECT_EQ(ced_output.ced_id[side_idx], FBK_ZERO_UINT);
      EXPECT_FLOAT_EQ(ced_output.ced_ttc[side_idx], CED_FORD_INVALID_TTC);
      EXPECT_FLOAT_EQ(ced_output.ced_ttp[side_idx], CED_FORD_INVALID_TTP);
      EXPECT_FLOAT_EQ(ced_output.ced_object_speed[side_idx], FBK_ZERO_F);
      EXPECT_FLOAT_EQ(ced_output.ced_object_heading[side_idx], FBK_ZERO_F);
      EXPECT_FLOAT_EQ(ced_output.ced_object_length[side_idx], FBK_ZERO_F);
      EXPECT_FLOAT_EQ(ced_output.ced_object_width[side_idx], FBK_ZERO_F);
   }
}

/**
 * Check that ced output is set for the front left approach.
 * \uts{CSCSA-41672} \sdd{SF-3503} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Output__sets_ced_forddat21_output_front_left)
{
   uint8_t side_index                      = FBK_SIDE_LEFT;
   uint8_t id                              = FBK_ONE_UINT;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_FRONT_LEFT;
   Ced_Alert_T alert_level                 = CED_ALERT_ACTIVE_LEVEL_1;
   uint8_t approach_index                  = (uint8_t) approach_type;

   /** \arrange Set up core output to front left alert object. */
   ced_instance.core_output.ced_alert[side_index] = alert_level;
   ced_instance.core_output.ced_id[side_index]    = id;

   /** \action Call function to test */
   Ced_Set_Output(&ced_output, &(ced_instance.core_input), &(ced_instance.core_output), approach_type, side_index);

   /** \assert Verify that output is set accordingly. */
   EXPECT_EQ(ced_output.ced_alert[approach_index], (uint8_t) alert_level);
   EXPECT_EQ(ced_output.ced_id[approach_index], id);
}

/**
 * Check that ced output is set for the front right approach.
 * \uts{CSCSA-41673} \sdd{SF-3503} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Output__sets_ced_forddat21_output_front_right)
{
   uint8_t side_index                      = FBK_SIDE_RIGHT;
   uint8_t id                              = FBK_ONE_UINT;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_FRONT_RIGHT;
   Ced_Alert_T alert_level                 = CED_ALERT_ACTIVE_LEVEL_1;
   uint8_t approach_index                  = (uint8_t) approach_type;

   /** \arrange Set up core output to front right alert object. */
   ced_instance.core_output.ced_alert[side_index] = alert_level;
   ced_instance.core_output.ced_id[side_index]    = id;

   /** \action Call function to test */
   Ced_Set_Output(&ced_output, &(ced_instance.core_input), &(ced_instance.core_output), approach_type, side_index);

   /** \assert Verify that output is set accordingly. */
   EXPECT_EQ(ced_output.ced_alert[approach_index], (uint8_t) alert_level);
   EXPECT_EQ(ced_output.ced_id[approach_index], id);
}

/**
 * Check that ced output is set for the rear left approach.
 * \uts{CSCSA-41674} \sdd{SF-3503} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Output__sets_ced_forddat21_output_rear_left)
{
   uint8_t side_index                      = FBK_SIDE_LEFT;
   uint8_t id                              = FBK_ONE_UINT;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_REAR_LEFT;
   Ced_Alert_T alert_level                 = CED_ALERT_ACTIVE_LEVEL_1;
   uint8_t approach_index                  = (uint8_t) approach_type;

   /** \arrange Set up core output to rear left alert object. */
   ced_instance.core_output.ced_alert[side_index] = alert_level;
   ced_instance.core_output.ced_id[side_index]    = id;

   /** \action Call function to test */
   Ced_Set_Output(&ced_output, &(ced_instance.core_input), &(ced_instance.core_output), approach_type, side_index);

   /** \assert Verify that output is set accordingly. */
   EXPECT_EQ(ced_output.ced_alert[approach_index], (uint8_t) alert_level);
   EXPECT_EQ(ced_output.ced_id[approach_index], id);
}

/**
 * Check that ced output is set for the rear right approach.
 * \uts{CSCSA-41675} \sdd{SF-3503} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Set_Output__sets_ced_forddat21_output_rear_right)
{
   uint8_t side_index                      = FBK_SIDE_RIGHT;
   uint8_t id                              = FBK_ONE_UINT;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_REAR_RIGHT;
   Ced_Alert_T alert_level                 = CED_ALERT_ACTIVE_LEVEL_1;
   uint8_t approach_index                  = (uint8_t) approach_type;

   /** \arrange Set up core output to rear right alert object. */
   ced_instance.core_output.ced_alert[side_index] = alert_level;
   ced_instance.core_output.ced_id[side_index]    = id;

   /** \action Call function to test */
   Ced_Set_Output(&ced_output, &(ced_instance.core_input), &(ced_instance.core_output), approach_type, side_index);

   /** \assert Verify that output is set accordingly. */
   EXPECT_EQ(ced_output.ced_alert[approach_index], (uint8_t) alert_level);
   EXPECT_EQ(ced_output.ced_id[approach_index], id);
}


/**
 * Check that heading convertion works properly for objects with approach type front right and heading towards ego.
 * \uts{CSCSA-41676} \sdd{SF-3475} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Convert_Vcs_Heading_To_Ford_Heading__works_properly_front_right_heading_towards_ego)
{
   /** \arrange Set up heading and approach type. */
   float32_T ford_heading;
   float32_T vcs_heading                   = -PI * 0.9f;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_FRONT_RIGHT;

   /** \action Run function to convert VCS heading to Ford heading */
   ford_heading = Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, vcs_heading);

   /** \assert Verify that correct heading is returned by function. */
   EXPECT_NEAR(ford_heading, PI * 0.1f, EPSILON);
}

/**
 * Check that heading convertion works properly for objects with approach type front right and heading away from ego.
 * \uts{CSCSA-41677} \sdd{SF-3475} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Convert_Vcs_Heading_To_Ford_Heading__works_properly_front_right_heading_away_from_ego)
{
   /** \arrange Set up heading and approach type. */
   float32_T ford_heading;
   float32_T vcs_heading                   = PI * 0.9f;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_FRONT_RIGHT;

   /** \action Run function to convert VCS heading to Ford heading */
   ford_heading = Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, vcs_heading);

   /** \assert Verify that correct heading is returned by function. */
   EXPECT_NEAR(ford_heading, -PI * 0.1f, EPSILON);
}

/**
 * Check that heading convertion works properly for objects with approach type front left and heading towards ego.
 * \uts{CSCSA-41678} \sdd{SF-3475} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Convert_Vcs_Heading_To_Ford_Heading__works_properly_front_left_heading_towards_ego)
{
   /** \arrange Set up heading and approach type. */
   float32_T ford_heading;
   float32_T vcs_heading                   = PI * 0.9f;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_FRONT_LEFT;

   /** \action Run function to convert VCS heading to Ford heading */
   ford_heading = Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, vcs_heading);

   /** \assert Verify that correct heading is returned by function. */
   EXPECT_NEAR(ford_heading, PI * 0.1f, EPSILON);
}

/**
 * Check that heading convertion works properly for objects with approach type front left and heading away from ego.
 * \uts{CSCSA-41679} \sdd{SF-3475} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Convert_Vcs_Heading_To_Ford_Heading__works_properly_front_left_heading_away_from_ego)
{
   /** \arrange Set up heading and approach type. */
   float32_T ford_heading;
   float32_T vcs_heading                   = -PI * 0.9f;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_FRONT_LEFT;

   /** \action Run function to convert VCS heading to Ford heading */
   ford_heading = Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, vcs_heading);

   /** \assert Verify that correct heading is returned by function. */
   EXPECT_NEAR(ford_heading, -PI * 0.1f, EPSILON);
}

/**
 * Check that heading convertion works properly for objects with approach type rear left and heading towards ego.
 * \uts{CSCSA-41680} \sdd{SF-3475} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Convert_Vcs_Heading_To_Ford_Heading__works_properly_rear_left_heading_towards_ego)
{
   /** \arrange Set up heading and approach type. */
   float32_T ford_heading;
   float32_T vcs_heading                   = PI * 0.1f;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_REAR_LEFT;

   /** \action Run function to convert VCS heading to Ford heading */
   ford_heading = Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, vcs_heading);

   /** \assert Verify that correct heading is returned by function. */
   EXPECT_NEAR(ford_heading, PI * 0.1f, EPSILON);
}

/**
 * Check that heading convertion works properly for objects with approach type rear left and heading away from ego.
 * \uts{CSCSA-41681} \sdd{SF-3475} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Convert_Vcs_Heading_To_Ford_Heading__works_properly_rear_left_heading_away_from_ego)
{
   /** \arrange Set up heading and approach type. */
   float32_T ford_heading;
   float32_T vcs_heading                   = -PI * 0.1f;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_REAR_LEFT;

   /** \action Run function to convert VCS heading to Ford heading */
   ford_heading = Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, vcs_heading);

   /** \assert Verify that correct heading is returned by function. */
   EXPECT_NEAR(ford_heading, -PI * 0.1f, EPSILON);
}

/**
 * Check that heading convertion works properly for objects with approach type rear right and heading towards ego.
 * \uts{CSCSA-41682} \sdd{SF-3475} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Convert_Vcs_Heading_To_Ford_Heading__works_properly_rear_right_heading_towards_ego)
{
   /** \arrange Set up heading and approach type. */
   float32_T ford_heading;
   float32_T vcs_heading                   = -PI * 0.1f;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_REAR_RIGHT;

   /** \action Run function to convert VCS heading to Ford heading */
   ford_heading = Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, vcs_heading);

   /** \assert Verify that correct heading is returned by function. */
   EXPECT_NEAR(ford_heading, PI * 0.1f, EPSILON);
}

/**
 * Check that heading convertion works properly for objects with approach type rear right and heading away from ego.
 * \uts{CSCSA-41683} \sdd{SF-3475} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Convert_Vcs_Heading_To_Ford_Heading__works_properly_rear_right_heading_away_from_ego)
{
   /** \arrange Set up heading and approach type. */
   float32_T ford_heading;
   float32_T vcs_heading                   = PI * 0.1f;
   Ced_Ford_Approach_Types_T approach_type = CED_FORD_APPROACH_REAR_RIGHT;

   /** \action Run function to convert VCS heading to Ford heading */
   ford_heading = Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, vcs_heading);

   /** \assert Verify that correct heading is returned by function. */
   EXPECT_NEAR(ford_heading, -PI * 0.1f, EPSILON);
}
