/**
 * @file pt_persistent_handler_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_persistent_handler.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44001}
 */

#include "pt_persistent_handler_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>


extern "C"
{
#include "fbk_macros.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
#include "pt_persistent_handler.c"
}

using ::testing::Eq;
using ::testing::Not;

/**
 * Call the update routine for the executional flag of path tracking algorithm.
 * \uts{CSCSA-44002} \sdd{SF-7537} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Persistent_Handler_Test, Pt_Update_Persistent__update_the_flag_indicating_whether_pt_is_operational)
{
   /** \arrange set PT to not operational. */
   path_tracking_persistent.f_was_pt_executed = FBK_FALSE;

   /** \action Call update routine for the operational flag. */
   Pt_Update_Persistent(&path_tracking_persistent);

   /** \assert expect flag set to true */
   EXPECT_TRUE(path_tracking_persistent.f_was_pt_executed);
}

/**
 * Test the persistent path tracking reset functionality by filling the output with non-default values, calling
 * Pt_Reset_Persistent, and verifying that the persistent data contain only default values. \uts{CSCSA-44003} \sdd{SF-7535}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Persistent_Handler_Test, Pt_Reset_Persistent__all_attributes_reset_correctly)
{
   /** \arrange Fill the output with non-default values. */
   path_tracking_persistent.f_was_pt_executed = FBK_TRUE;

   /** \action Call Pt_Reset_Persistent */
   Pt_Reset_Persistent(&path_tracking_persistent);

   /** \assert Check if persistent data are set to default values. */
   EXPECT_EQ(path_tracking_persistent.f_was_pt_executed, FBK_FALSE);
   for (uint8_t i = 0; i <= PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      EXPECT_EQ(path_tracking_persistent.path_index_last_cycle[i], PT_DEFAULT_MATCH_INDEX);
   }
}

/**
 * Test the persistent path tracking reset functionality by filling the output with non-default values, calling
 * Pt_Reset_Persistent, and verifying that the persistent data contain only default values. \uts{CSCSA-44004} \sdd{SF-7534}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Persistent_Handler_Test, Pt_Reset_All_Matching_Pairs__all_attributes_reset_correctly)
{
   Pt_Best_Path_Obj_Pair_Persistent_T matching_pairs[PT_OBJ_MAX_ARRAY_SIZE];

   /** \arrange Set all pt_persistent.paths to longitudinal. Fill the output with non-default values. */
   for (uint8_t i = 0; i <= PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      matching_pairs[i].confidence_factor = 3.0f;
      matching_pairs[i].path_index        = 12u;
   }

   /** \action Call Pt_Reset_Persistent */
   Pt_Reset_All_Matching_Pairs(matching_pairs);

   /** \assert Check if persistent data are set to default values. */
   for (uint8_t i = 0; i <= PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      EXPECT_FLOAT_EQ(matching_pairs[i].confidence_factor, 0.0f);
      EXPECT_EQ(matching_pairs[i].path_index, PT_DEFAULT_MATCH_INDEX);
   }
}


/**
 * Call the reset routine of a single matching pair.
 * \uts{CSCSA-44005} \sdd{SF-7536} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Persistent_Handler_Test, Pt_Reset_Single_Matching_Pair__fill_matching_pair_with_default_values)
{
   /** \arrange fill matching pair with non default values. */
   Pt_Best_Path_Obj_Pair_Persistent_T matching_pair{};

   /** \action Call update routine for the operational flag. */
   Pt_Reset_Single_Matching_Pair(&matching_pair);

   /** \assert expect matching pair filled with its default values */
   EXPECT_FLOAT_EQ(matching_pair.confidence_factor, 0.0f);
   EXPECT_FLOAT_EQ(matching_pair.relevant_dist_comp_matching, PT_HUGE_DISTANCE_TO_PATH);
   EXPECT_EQ(matching_pair.path_index, PT_DEFAULT_MATCH_INDEX);
}