/**
 * @file pt_reset_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_reset.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44006}
 */

#include "pt_reset_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_const_macros.h"
#include "pt_common_functions.h"
#include "pt_reset.c"
#include "pt_types.h"
}


/**
 * Tests the overall reset of path tracking output and also the reset of all pt_persistent.paths. Here those are arranged at first
 * with non default values, so that it can be checked, whether those attributes are reset correctly. \uts{CSCSA-44007}
 * \sdd{SF-7547} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Reset_Test, Pt_Reset_All_Paths__check_that_all_paths_and_pt_output_is_cleared)
{
   /** \arrange Fill both structures with non default values. */
   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_MATURE;
   }
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      path_output.path_obj_pair_output[idx].path_direction = PATH_DIRECTION_LONG_FORWARD;
   }
   /** \action Call reset of output and all pt_persistent.paths */
   Pt_Reset_All_Paths(pt_persistent.paths, &path_output, best_path_object_pairs);

   /** \assert Check if output and pt_persistent.paths are set to default values. */
   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      EXPECT_EQ(pt_persistent.paths[idx].path_state, PATH_STATUS_DEFAULT);
   }
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(path_output.path_obj_pair_output[idx].path_direction, PATH_DIRECTION_NONE);
   }
}


/**
 * Test the path tracking reset functionality by filling the output with non-default values, calling Pt_Reset_Path_Output, and
 * verifying that the output contains default values only. \uts{CSCSA-44008} \sdd{SF-7642} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Reset_Test, Pt_Reset_Path_Output__all_attributes_reset_correctly)
{
   /** \arrange Fill the output with non-default values. */
   Pt_Output_T output_test;
   uint8_t idx;

   for (idx = 0u; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      output_test.path_obj_pair_output[idx].track_match                   = 5;
      output_test.path_obj_pair_output[idx].track_match_last_cycle        = 5;
      output_test.path_obj_pair_output[idx].track_match_age               = 5;
      output_test.path_obj_pair_output[idx].range_at_zero                 = 1.2f;
      output_test.path_obj_pair_output[idx].range_at_host_edge            = 1.2f;
      output_test.path_obj_pair_output[idx].length_of_trajectory          = 1.2f;
      output_test.path_obj_pair_output[idx].path_heading                  = 1.2f;
      output_test.path_obj_pair_output[idx].path_direction                = PATH_DIRECTION_LAT_LEFT;
      output_test.nearest_path_output[idx].range_vcs_proj_to_path_segment = 10.0f;
      output_test.nearest_path_output[idx].segment_heading_diff           = PI;
      output_test.nearest_path_output[idx].track_idx_nearest_path         = 2u;
   }
   /** \action Call Pt_Reset_Path_Output */
   Pt_Reset_Path_Output(&output_test);

   /** \assert Check that the complete Pt output is set to its corresponding default. */
   for (idx = 0u; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(output_test.path_obj_pair_output[idx].track_match, PT_DEFAULT_MATCH_INDEX);
      EXPECT_EQ(output_test.path_obj_pair_output[idx].track_match_last_cycle, PT_DEFAULT_MATCH_INDEX);
      EXPECT_EQ(output_test.path_obj_pair_output[idx].track_match_age, 0);
      EXPECT_FLOAT_EQ(output_test.path_obj_pair_output[idx].range_at_zero, PT_HIGH_DISTANCE_DEFAULT_VAL);
      EXPECT_FLOAT_EQ(output_test.path_obj_pair_output[idx].range_at_host_edge, PT_HIGH_DISTANCE_DEFAULT_VAL);
      EXPECT_FLOAT_EQ(output_test.path_obj_pair_output[idx].length_of_trajectory, -1.0f);
      EXPECT_FLOAT_EQ(output_test.path_obj_pair_output[idx].path_heading, 0.0f);
      EXPECT_EQ(output_test.path_obj_pair_output[idx].path_direction, PATH_DIRECTION_NONE);
      EXPECT_FLOAT_EQ(output_test.nearest_path_output[idx].range_vcs_proj_to_path_segment, PT_HIGH_DISTANCE_DEFAULT_VAL);
      EXPECT_FLOAT_EQ(output_test.nearest_path_output[idx].segment_heading_diff, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(output_test.nearest_path_output[idx].track_idx_nearest_path, PT_DEFAULT_MATCH_INDEX);
   }
}


/**
 * Test the path tracking reset functionality by filling the output with non-default values, calling Pt_Reset_Path_Output, and
 * verifying that the output contains default values only. \uts{CSCSA-44009} \sdd{SF-7549} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Reset_Test, Pt_Reset_Single_Path_Output__all_attributes_reset_correctly)
{
   /** \arrange Fill the output with non-default values. */
   Pt_Output_T output_test;

   output_test.path_obj_pair_output[0].track_match            = 5;
   output_test.path_obj_pair_output[0].track_match_last_cycle = 5;
   output_test.path_obj_pair_output[0].track_match_age        = 5;
   output_test.path_obj_pair_output[0].range_at_zero          = 1.2f;
   output_test.path_obj_pair_output[0].range_at_host_edge     = 1.2f;
   output_test.path_obj_pair_output[0].length_of_trajectory   = 1.2f;
   output_test.path_obj_pair_output[0].path_heading           = 1.2f;
   output_test.path_obj_pair_output[0].path_direction         = PATH_DIRECTION_LAT_LEFT;

   /** \action Call Pt_Reset_Single_Path_Output */
   Pt_Reset_Single_Path_Output(&output_test.path_obj_pair_output[0], &output_test.nearest_path_output[0]);

   /** \assert Check if output is set to default values. */
   EXPECT_EQ(output_test.path_obj_pair_output[0].track_match, PT_DEFAULT_MATCH_INDEX);
   EXPECT_EQ(output_test.path_obj_pair_output[0].track_match_last_cycle, PT_DEFAULT_MATCH_INDEX);
   EXPECT_EQ(output_test.path_obj_pair_output[0].track_match_age, 0);
   EXPECT_FLOAT_EQ(output_test.path_obj_pair_output[0].range_at_zero, PT_HIGH_DISTANCE_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(output_test.path_obj_pair_output[0].range_at_host_edge, PT_HIGH_DISTANCE_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(output_test.path_obj_pair_output[0].length_of_trajectory, -1.0f);
   EXPECT_FLOAT_EQ(output_test.path_obj_pair_output[0].path_heading, 0.0f);
   EXPECT_EQ(output_test.path_obj_pair_output[0].path_direction, PATH_DIRECTION_NONE);
}

/**
 * Tests the constructor of a single path. Check that all attributes are reset correctly.
 * \uts{CSCSA-44010} \sdd{SF-7543} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Reset_Test, Pt_Reset_Path__reset_all_path_attributes_correctly)
{
   /** \arrange Initialize path with non default values. */
   Pt_Init_Path_Linearly(&single_path, p_grid_array, PATH_DIRECTION_LONG_BACKWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         Create_2d_Vector_Coordinates(p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 3.0f, 1.0f), 0.0f, -1.0f);

   /** \action Call reset of the path function */
   Pt_Reset_Path(&single_path);

   /** \assert Check whether path is reset to its default. */
   EXPECT_EQ(single_path.obj_curr_used_for_path_build.id, 0u);
   EXPECT_EQ(single_path.obj_curr_used_for_path_build.age, 0u);
   EXPECT_EQ(single_path.first_p, PT_DEFAULT_DISCR_BORDER);
   EXPECT_EQ(single_path.last_p, PT_DEFAULT_DISCR_BORDER);
   EXPECT_EQ(single_path.path_state, PATH_STATUS_DEFAULT);
   EXPECT_EQ(single_path.direction, PATH_DIRECTION_NONE);
   EXPECT_FLOAT_EQ(single_path.max_speed, 0.0f);
   EXPECT_EQ(single_path.path_age, 0u);
   EXPECT_EQ(single_path.num_groupings, 0u);
   EXPECT_FLOAT_EQ(single_path.first.x, 0.0f);
   EXPECT_FLOAT_EQ(single_path.first.y, 0.0f);
   EXPECT_FLOAT_EQ(single_path.last_mat.x, 0.0f);
   EXPECT_FLOAT_EQ(single_path.last_mat.y, 0.0f);
   EXPECT_EQ(single_path.new_path_point_status, PATH_POINT_NEW_FIRST);
   EXPECT_EQ(single_path.path_border_status, PATH_BOTH_BORDERS_NEW);

   for (uint8_t idx = 0u; idx < PT_NUM_GRID_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(single_path.path_points[idx], PT_PATH_POINTS_DEFAULT_VAL);
   }
}


/**
 * Test the path association reset functionality by filling the associations with non-default values, calling
 * Pt_Reset_Path_Associations, and verifying that the associated pt_persistent.paths contain only default values. \uts{CSCSA-44011}
 * \sdd{SF-7544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Reset_Test, Pt_Reset_Path_Associations__all_attributes_reset_correctly)
{
   /** \arrange Set all pt_persistent.paths to longitudinal. Fill the output with non-default values. */
   uint8_t idx;

   for (idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      best_path_object_pairs[idx].path_index        = 5;
      best_path_object_pairs[idx].confidence_factor = 3.0f;
   }

   /** \action Call Pt_Reset_Path_Associations */
   Pt_Reset_Path_Associations(&(pt_persistent.paths[5]), best_path_object_pairs);

   /** \assert Check if path outputs are set to default values. */
   for (idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(best_path_object_pairs[idx].path_index, PT_DEFAULT_MATCH_INDEX);
      EXPECT_FLOAT_EQ(best_path_object_pairs[idx].confidence_factor, 0.0f);
   }
}


/**
 * Tests Reset of path and any associations to it. Here no reason is given and thus no reset shall occure.
 * \uts{CSCSA-44012} \sdd{SF-7548} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Reset_Test, Pt_Reset_Path_And_Associations_To_It__no_reset_reason_is_given_thus_no_reset)
{
   /** \arrange values on the thresholds. */
   uint8_t idx;

   /** \action call function to test. */
   Pt_Reset_Path_And_Associations_To_It(&(pt_persistent.paths[5]), best_path_object_pairs, PATH_RESET_NO_RESET);

   /** \assert Expect speed to be saturated. */
   for (idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[5].path_points[idx], 3);
   }
}