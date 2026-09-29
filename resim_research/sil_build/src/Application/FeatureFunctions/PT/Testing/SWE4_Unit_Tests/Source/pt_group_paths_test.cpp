/**
 * @file pt_group_paths_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_group_paths.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43556}
 */

#include "pt_group_paths_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pt_group_paths.c"
}


/**
 * Check the superordinate function for overlap grouping. Here too many pt_persistent.paths are similar to the path to check. Thus
 * the path to check is expected to be reset, since it is too redundant to its surrounding pt_persistent.paths. \uts{CSCSA-43557}
 * \sdd{SF-7386} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Overlap_Paths__reset_path_zero_since_its_redundant_to_its_surrounding)
{
   /** \arrange Setup redundant pt_persistent.paths. */
   for (uint8_t idx = 0; idx < PT_MAX_ALLOWED_PATHS_WITH_PATH_POINT_OVERLAPS + 2; idx++)
   {
      Pt_Init_Path_Linearly(&pt_persistent.paths[idx], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD,
                            PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0u,
                            Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 3.0f, 1.0f),
                            Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 3.0f, 1.0f), 0.0f,
                            -1.0f);
      pt_persistent.paths[idx].path_index = idx;
      pt_persistent.paths[idx].path_state = PATH_STATUS_MATURE;
   }

   /** \action call overlap grouping function. */
   Pt_Group_Overlap_Paths(&pt_persistent, &pt_persistent.paths[0], &cals, &pt_input);

   /** \assert expect path at slot 0 to be reset. */
   EXPECT_EQ(pt_persistent.paths[0].path_state, PATH_STATUS_DEFAULT);
}


/**
 * Check the superordinate function for overlap grouping. Here the less established path is reset .
 * \uts{CSCSA-43558} \sdd{SF-7386} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Overlap_Paths__reset_less_established_path)
{
   /** \arrange Setup two pt_persistent.paths which are not equally established. */
   Pt_Init_Path_Linearly(&pt_persistent.paths[0u], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD,
                         PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 3.0f, 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[0u].path_index = 0u;
   pt_persistent.paths[0u].path_state = PATH_STATUS_MATURE;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1u], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD,
                         PT_LOWEST_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET, PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 3.0f, 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[1u].path_index = 1u;
   pt_persistent.paths[1u].path_state = PATH_STATUS_MATURE;

   /*calibrations*/
   cals.k_pt_overlap_max_match_value                                   = 30.0f;
   cals.k_pt_start_of_lane_change_processing                           = 0u;
   cals.k_pt_end_of_lane_change_processing                             = PT_HIGHEST_GRID_POINT_INDEX;
   cals.k_pt_group_overlap_paths_min_diff                              = 3.5f;
   cals.k_pt_group_max_match_value_ad                                  = 2.0f;
   cals.k_pt_group_dir_max_diff_value                                  = 1.5f;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_diff_num_path_points        = 0u;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_group_path_min_overlap_count    = 2u;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_group_path_min_overlap_count_ad = 4u;

   pt_persistent.paths[1].path_points[PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] =
      cals.k_pt_group_overlap_paths_min_diff + 1.0f;
   pt_persistent.paths[1].path_points[PT_HIGHEST_GRID_POINT_INDEX] = cals.k_pt_group_overlap_paths_min_diff + 1.0f;

   pt_persistent.paths[0].num_groupings = 5u;
   pt_persistent.paths[1].num_groupings = 6u;
   /** \action call overlap grouping function. */
   Pt_Group_Overlap_Paths(&pt_persistent, &pt_persistent.paths[0], &cals, &pt_input);

   /** \assert expect path at slot 0 to be reset. */
   EXPECT_EQ(pt_persistent.paths[0].path_state, PATH_STATUS_DEFAULT);
   EXPECT_EQ(pt_persistent.paths[1].path_state, PATH_STATUS_MATURE);
   EXPECT_EQ(pt_persistent.paths[1].first_p, PT_LOWEST_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET);
   EXPECT_EQ(pt_persistent.paths[1].last_p, PT_HIGHEST_GRID_POINT_INDEX);
}


/**
 * Checks the functionality of grouping weight calculation. Here the extrapolation path is shorter than the path to kill so that a
 * weighting factor of 1.0f - cals.weight is expected. \uts{CSCSA-43559} \sdd{SF-7384} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Get_Grouping_Weight_Outside_Path_Borders__path_extrapolate_shorter_than_path_to_kill)
{
   /** \arrange Setup boundaries of both pt_persistent.paths. */
   float32_T result;
   uint8_t point_idx;
   pt_persistent.paths[0].last_p = PT_MID_GRID_POINT_INDEX;
   point_idx                     = pt_persistent.paths[0].last_p + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p = pt_persistent.paths[0].last_p + cals.k_pt_point_diff_grouping_borders_lut[1];

   /** \action call function to test. */
   result = Pt_Get_Grouping_Weight_Outside_Path_Borders(&pt_persistent.paths[0], &pt_persistent.paths[1], &cals, point_idx);

   /** \assert Expect_value to be set to 1 minus difference weight factor lut at second. */
   EXPECT_FLOAT_EQ(result, 1.0f - cals.k_pt_point_diff_weighting_factor_lut[1]);
}

/**
 * Checks the functionality of grouping weight calculation. Here the extrapolation path is shorter than the path to kill so that a
 * weighting factor of cals.weight is expected. \uts{CSCSA-43560} \sdd{SF-7384} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Get_Grouping_Weight_Outside_Path_Borders__path_extrapolate_longer_than_path_to_kill)
{
   /** \arrange Setup boundaries of both pt_persistent.paths. */
   float32_T result;
   uint8_t point_idx;
   pt_persistent.paths[0].last_p = PT_MID_GRID_POINT_INDEX;
   point_idx                     = pt_persistent.paths[0].last_p + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p = pt_persistent.paths[0].last_p + cals.k_pt_point_diff_grouping_borders_lut[1];

   /** \action call function to test. */
   result = Pt_Get_Grouping_Weight_Outside_Path_Borders(&pt_persistent.paths[1], &pt_persistent.paths[0], &cals, point_idx);

   /** \assert Expect_value to be set on last point. */
   EXPECT_FLOAT_EQ(result, cals.k_pt_point_diff_weighting_factor_lut[1]);
}


/**
 * Checks the functionality of grouping weight calculation. Here the point idx is located below lower border
 * \uts{CSCSA-43561} \sdd{SF-7384} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Get_Grouping_Weight_Outside_Path_Borders__points_idx_for_grouping_weights)
{
   /** \arrange Setup boundaries of both pt_persistent.paths. */
   float32_T result;
   uint8_t point_idx;

   pt_persistent.paths[0].last_p = PT_HIGHEST_GRID_POINT_INDEX;
   pt_persistent.paths[1].last_p = PT_HIGHEST_GRID_POINT_INDEX;

   pt_persistent.paths[0].first_p = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].first_p = pt_persistent.paths[0].first_p + cals.k_pt_point_diff_grouping_borders_lut[1];
   point_idx                      = PT_LOWEST_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call function to test. */
   result = Pt_Get_Grouping_Weight_Outside_Path_Borders(&pt_persistent.paths[1], &pt_persistent.paths[0], &cals, point_idx);

   /** \assert Expect_value to be set on last point. */
   EXPECT_FLOAT_EQ(result, 1.0f - cals.k_pt_point_diff_weighting_factor_lut[1]);
}


/**
 * Checks the functionality of grouping weight calculation. Here the lower border of the path for termination is inside the path
 * which is allowed to live longer. \uts{CSCSA-43562} \sdd{SF-7384} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Get_Grouping_Weight_Outside_Path_Borders__path_to_kill_lower_border_gt_other_paths_lower_border)
{
   /** \arrange Setup boundaries of both pt_persistent.paths. */
   float32_T result;
   uint8_t point_idx;

   pt_persistent.paths[0].last_p = PT_HIGHEST_GRID_POINT_INDEX - 1u;
   pt_persistent.paths[1].last_p = PT_HIGHEST_GRID_POINT_INDEX;

   pt_persistent.paths[0].first_p = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1].first_p = PT_MID_GRID_POINT_INDEX + 2u;
   point_idx                      = PT_MID_GRID_POINT_INDEX - 3u;

   /** \action call function to test. */
   result = Pt_Get_Grouping_Weight_Outside_Path_Borders(&pt_persistent.paths[0], &pt_persistent.paths[1], &cals, point_idx);

   /** \assert Expect_value to be set on last point. */
   EXPECT_FLOAT_EQ(result, cals.k_pt_point_diff_weighting_factor_lut[2]);
}


/**
 * Test the merging of path borders for longitudinally oriented pt_persistent.paths. Here the longitudinal component shall be
 * overwritten. \uts{CSCSA-43563} \sdd{SF-7392} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Merge_Path_Borders_With_Extrapol__longitudinal_overwrite_boundaries)
{
   /** \arrange Setup two pt_persistent.paths which shall be merged. the path at 0 shall be the path to reset and the path 1 shall
    * be the slot of merging. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 3.0f, 1.0f), 0.0f, -1.0f);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 1.5f, 1.0f), 0.0f, -1.0f);


   /** \action call function for path border merging. */
   Pt_Merge_Path_Borders_With_Extrapol(&pt_persistent.paths[1], &pt_persistent.paths[0], pt_input.grid_pt_array);

   /** \assert expect borders to be set to the outermost border of both input pt_persistent.paths. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].first.x, pt_input.grid_pt_array[lower_idx] - 3.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].last_mat.x, pt_input.grid_pt_array[upper_idx] + 3.0f);
}


/**
 * Test the merging of path borders for laterally oriented pt_persistent.paths. Here the lateral component shall be overwritten.
 * \uts{CSCSA-43564} \sdd{SF-7392} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Merge_Path_Borders_With_Extrapol__lateral_overwrite_boundaries)
{
   /** \arrange Setup two pt_persistent.paths which shall be merged. the path at 0 shall be the path to reset and the path 1 shall
    * be the slot of merging. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LAT_LEFT, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[lower_idx] - 3.0f),
                         Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[upper_idx] + 3.0f), 0.0f, -1.0f);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LAT_LEFT, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[lower_idx] - 1.5f),
                         Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[upper_idx] + 1.5f), 0.0f, -1.0f);


   /** \action call function for path border merging. */
   Pt_Merge_Path_Borders_With_Extrapol(&pt_persistent.paths[1], &pt_persistent.paths[0], pt_input.grid_pt_array);

   /** \assert expect borders to be set to the outermost border of both input pt_persistent.paths. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].first.y, pt_input.grid_pt_array[lower_idx] - 3.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].last_mat.y, pt_input.grid_pt_array[upper_idx] + 3.0f);
}


/**
 * Test the merging of path borders for longitudinally oriented pt_persistent.paths. Here the longitudinal component shall be
 * overwritten. No interpoation is applied here thus only the vectors are set up. \uts{CSCSA-43565} \sdd{SF-7391}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Merge_Path_Borders__longitudinal_overwrite_boundaries)
{
   /** \arrange Setup two pt_persistent.paths which shall be merged. the path at 0 shall be the path to reset and the path 1 shall
    * be the slot of merging. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;

   pt_persistent.paths[0].direction = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[0].first     = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 3.0f, 1.0f);
   pt_persistent.paths[0].last_mat  = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 3.0f, 1.0f);
   pt_persistent.paths[1].direction = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[1].first     = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 1.5f, 1.0f);
   pt_persistent.paths[1].last_mat  = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 1.5f, 1.0f);


   /** \action call function for path border merging without extrapolation. */
   Pt_Merge_Path_Borders(&pt_persistent.paths[1], &pt_persistent.paths[0]);

   /** \assert expect borders to be set to the outermost border of both input pt_persistent.paths. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].first.x, pt_input.grid_pt_array[lower_idx] - 3.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].last_mat.x, pt_input.grid_pt_array[upper_idx] + 3.0f);
}


/**
 * Test the merging of path borders for laterally oriented pt_persistent.paths. Here the lateral component shall be overwritten. No
 * interpoation is applied here thus only the vectors are set up. \uts{CSCSA-43566} \sdd{SF-7391} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Merge_Path_Borders__lateral_overwrite_boundaries)
{
   /** \arrange Setup two pt_persistent.paths which shall be merged. the path at 0 shall be the path to reset and the path 1 shall
    * be the slot of merging. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;

   pt_persistent.paths[0].direction = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].first     = Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[lower_idx] - 3.0f);
   pt_persistent.paths[0].last_mat  = Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[upper_idx] + 3.0f);
   pt_persistent.paths[1].direction = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[1].first     = Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[lower_idx] - 1.5f);
   pt_persistent.paths[1].last_mat  = Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[upper_idx] + 1.5f);


   /** \action call function for path border merging. */
   Pt_Merge_Path_Borders(&pt_persistent.paths[1], &pt_persistent.paths[0]);

   /** \assert expect borders to be set to the outermost border of both input pt_persistent.paths. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].first.y, pt_input.grid_pt_array[lower_idx] - 3.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].last_mat.y, pt_input.grid_pt_array[upper_idx] + 3.0f);
}


/**
 * Tests whether a given path pair is valid for cross grouping. Here the pair is valid for cross grouping, thus true is expected.
 * \uts{CSCSA-43567} \sdd{SF-7389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Cross_Grouping__path_pair_is_valid_for_cross_grouping)
{
   /** \arrange set up two pt_persistent.paths which are valid for cross grouping. */
   boolean_T ret;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[0].direction  = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].first_p    = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p     = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].path_state = PATH_STATUS_GROUPED;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index + 1u;
   pt_persistent.paths[1].direction  = pt_persistent.paths[0].direction;
   pt_persistent.paths[1].first_p    = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p     = PT_HIGHEST_GRID_POINT_INDEX;
   pt_persistent.paths[1].path_state = pt_persistent.paths[0].path_state;

   /** \action call function for validity check of path pair for cross grouping. */
   ret = Pt_Is_Path_Pair_Valid_For_Cross_Grouping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect true. */
   EXPECT_TRUE(ret);
}


/**
 * Tests whether a given path pair is valid for cross grouping. Here the pair is invalid for cross grouping, thus false is
 * expected. \uts{CSCSA-43568} \sdd{SF-7389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Cross_Grouping__path_pair_is_invalid_since_different_directions_are_given)
{
   /** \arrange set up two pt_persistent.paths which are invalid for cross grouping. */
   boolean_T ret;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[0].direction  = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].first_p    = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p     = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].path_state = PATH_STATUS_GROUPED;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index + 1u;
   pt_persistent.paths[1].direction  = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[1].first_p    = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p     = PT_HIGHEST_GRID_POINT_INDEX;
   pt_persistent.paths[1].path_state = pt_persistent.paths[0].path_state;

   /** \action call function for validity check of path pair for cross grouping. */
   ret = Pt_Is_Path_Pair_Valid_For_Cross_Grouping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect false due to different directions given. */
   EXPECT_FALSE(ret);
}


/**
 * Tests whether a given path pair is valid for cross grouping. Here the pair is invalid for cross grouping, thus false is
 * expected. \uts{CSCSA-43569} \sdd{SF-7389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Cross_Grouping__path_pair_is_invalid_since_same_path_index_is_given)
{
   /** \arrange set up two pt_persistent.paths which are invalid for cross grouping. */
   boolean_T ret;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[0].first_p    = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p     = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].path_state = PATH_STATUS_GROUPED;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index;
   pt_persistent.paths[1].first_p    = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p     = PT_HIGHEST_GRID_POINT_INDEX;
   pt_persistent.paths[1].path_state = pt_persistent.paths[0].path_state;

   /** \action call function for validity check of path pair for cross grouping. */
   ret = Pt_Is_Path_Pair_Valid_For_Cross_Grouping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect false due to the same path index. */
   EXPECT_FALSE(ret);
}


/**
 * Tests whether a given path pair is valid for cross grouping. Here the pair is invalid for cross grouping, thus false is
 * expected. \uts{CSCSA-43570} \sdd{SF-7389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Cross_Grouping__path_pair_is_invalid_since_paths1_object_is_still_assigned)
{
   /** \arrange set up two pt_persistent.paths which are invalid for cross grouping. */
   boolean_T ret;
   pt_persistent.paths[0].first_p                         = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p                          = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].path_state                      = PATH_STATUS_GROUPED;
   pt_persistent.paths[1].first_p                         = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].obj_curr_used_for_path_build.id = 1u;
   pt_persistent.paths[1].last_p                          = PT_HIGHEST_GRID_POINT_INDEX;
   pt_persistent.paths[1].path_state                      = PATH_STATUS_CREATION;

   /** \action call function for validity check of path pair for cross grouping. */
   ret = Pt_Is_Path_Pair_Valid_For_Cross_Grouping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect false due to assignment of object. */
   EXPECT_FALSE(ret);
}


/**
 * Tests whether a given path pair is valid for cross grouping. Here the pair is invalid for cross grouping, thus false is
 * expected. \uts{CSCSA-43571} \sdd{SF-7389} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Cross_Grouping__path_pair_is_invalid_since_intervals_are_overlapping)
{
   /** \arrange set up two pt_persistent.paths which are invalid for cross grouping. */
   boolean_T ret;
   pt_persistent.paths[0].first_p = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p  = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].first_p = pt_persistent.paths[0].first_p - PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p  = pt_persistent.paths[1].last_p + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call function for validity check of path pair for cross grouping. */
   ret = Pt_Is_Path_Pair_Valid_For_Cross_Grouping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect false due to overlapping intervals. */
   EXPECT_FALSE(ret);
}


/**
 * Tests whether a given path pair is better for cross grouping. Here the path pair is better for grouping since no path pair is
 * given yet and the intervals are nested. \uts{CSCSA-43572} \sdd{SF-7388} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Better_For_Cross_Grouping__no_cross_is_given_but_intervals_are_nested)
{
   /** \arrange set up conditions for a better cross grouping criteria pair. */
   boolean_T ret;
   float32_T min_diff_in_comp = 0.0f;
   float32_T min_diff         = 0.0f;
   boolean_T f_valid_path_intervals_nested;
   boolean_T f_have_one_cross;

   f_have_one_cross              = FBK_FALSE;
   f_valid_path_intervals_nested = FBK_TRUE;
   /** \action call function to check whether a better cross grouping pair is given. */
   ret = Pt_Is_Path_Pair_Better_For_Cross_Grouping(min_diff_in_comp, min_diff, f_valid_path_intervals_nested, f_have_one_cross);

   /** \assert expect true. */
   EXPECT_TRUE(ret);
}


/**
 * Tests whether a given path pair is better for cross grouping. Here the path pair is better for grouping since the current pairs
 * intervals are nested and the minimum difference is lower. \uts{CSCSA-43573} \sdd{SF-7388} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test,
       Pt_Is_Path_Pair_Better_For_Cross_Grouping__cross_is_given_but_intervals_are_nested_and_min_diff_is_lower_than_thres)
{
   /** \arrange set up conditions for a better cross grouping criteria pair. */
   boolean_T ret;
   float32_T min_diff_in_comp = 0.5f;
   float32_T min_diff         = 1.0f;
   boolean_T f_valid_path_intervals_nested;
   boolean_T f_have_one_cross;

   f_have_one_cross              = FBK_TRUE;
   f_valid_path_intervals_nested = FBK_TRUE;

   /** \action call function to check whether a better cross grouping pair is given. */
   ret = Pt_Is_Path_Pair_Better_For_Cross_Grouping(min_diff_in_comp, min_diff, f_valid_path_intervals_nested, f_have_one_cross);

   /** \assert expect true. */
   EXPECT_TRUE(ret);
}


/**
 * Tests whether a given path pair is better for cross grouping. Here the path pair is better for grouping since no cross is given
 * yet but the minimum difference is lower than the previous stored one. \uts{CSCSA-43574} \sdd{SF-7388}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Better_For_Cross_Grouping__no_cross_is_given_and_intervals_are_not_nested_but_diff_is_lower)
{
   /** \arrange set up conditions for a better cross grouping criteria pair. */
   boolean_T ret;
   float32_T min_diff_in_comp = 0.5f;
   float32_T min_diff         = 1.0f;
   boolean_T f_valid_path_intervals_nested;
   boolean_T f_have_one_cross;

   f_have_one_cross              = FBK_FALSE;
   f_valid_path_intervals_nested = FBK_TRUE;

   /** \action call function to check whether a better cross grouping pair is given. */
   ret = Pt_Is_Path_Pair_Better_For_Cross_Grouping(min_diff_in_comp, min_diff, f_valid_path_intervals_nested, f_have_one_cross);

   /** \assert expect true. */
   EXPECT_TRUE(ret);
}

/**
 * Tests whether a given path pair is better for cross grouping.Here the path pair is not better for cross grouping, since the
 * minimum diff is lower on the one hand but a cross is given and the intervals are not nested. \uts{CSCSA-43575} \sdd{SF-7388}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test,
       Pt_Is_Path_Pair_Better_For_Cross_Grouping__min_diff_is_better_but_a_cross_is_given_and_the_intervals_are_not_nested)
{
   /** \arrange set up conditions for a for a worse cross grouping pair. */
   boolean_T ret;
   float32_T min_diff_in_comp = 0.5f;
   float32_T min_diff         = 1.0f;
   boolean_T f_valid_path_intervals_nested;
   boolean_T f_have_one_cross;

   f_have_one_cross              = FBK_TRUE;
   f_valid_path_intervals_nested = FBK_FALSE;

   /** \action call function to check whether a better cross grouping pair is given. */
   ret = Pt_Is_Path_Pair_Better_For_Cross_Grouping(min_diff_in_comp, min_diff, f_valid_path_intervals_nested, f_have_one_cross);

   /** \assert expect false. */
   EXPECT_FALSE(ret);
}


/**
 * Tests whether a given path pair is better for cross grouping.Here the path pair is not better for cross grouping, since the
 * minimum difference is worse and a cross is already given. \uts{CSCSA-43576} \sdd{SF-7388} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Better_For_Cross_Grouping__min_diff_is_worse_a_cross_is_given_and_intervals_are_not_nested)
{
   /** \arrange set up conditions for a for a worse cross grouping pair. */
   boolean_T ret;
   float32_T min_diff         = 1.0f;
   float32_T min_diff_in_comp = 1.1f * min_diff;
   boolean_T f_valid_path_intervals_nested;
   boolean_T f_have_one_cross;

   f_have_one_cross              = FBK_TRUE;
   f_valid_path_intervals_nested = FBK_FALSE;

   /** \action call function to check whether a better cross grouping pair is given. */
   ret = Pt_Is_Path_Pair_Better_For_Cross_Grouping(min_diff_in_comp, min_diff, f_valid_path_intervals_nested, f_have_one_cross);

   /** \assert expect false. */
   EXPECT_FALSE(ret);
}


/**
 * Check whether basic grouping criteria are fulfilled. Here the basic conditions are fulfilled
 * \uts{CSCSA-43577} \sdd{SF-7380} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Are_Basic_Conditions_For_Grouping_Fulfilled__basic_conditions_are_fulfilled)
{
   /** \arrange set up grouping criteria such that conditions are fulfilled. */
   boolean_T ret;
   Pt_Local_Grouping_Criteria_T local_group_crit{};
   Pt_Global_Grouping_Criteria_T global_group_crit{};

   local_group_crit.match_value      = cals.k_pt_group_dir_max_avg_diff_value - EPSILON;
   global_group_crit.min_match_value = cals.k_pt_group_dir_max_avg_diff_value;

   /** \action call function to check whether basic conditions for grouping are fulfilled. */
   ret = Pt_Are_Basic_Conditions_For_Grouping_Fulfilled(&local_group_crit, &global_group_crit, &cals);

   /** \assert expect true. */
   EXPECT_TRUE(ret);
}

/**
 * Check whether basic grouping criteria are fulfilled. Here the basic conditions are not fulfilled since local groupings match
 * value is exceeds the globals match value. \uts{CSCSA-43578} \sdd{SF-7380} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test,
       Pt_Are_Basic_Conditions_For_Grouping_Fulfilled__basic_conditions_are_not_fulfilled_inner_loop_lt_global_props)
{
   /** \arrange set up grouping criteria such that conditions are not fulfilled. */
   boolean_T ret;
   Pt_Local_Grouping_Criteria_T local_group_crit{};
   Pt_Global_Grouping_Criteria_T global_group_crit{};

   local_group_crit.match_value      = cals.k_pt_group_dir_max_avg_diff_value - EPSILON;
   global_group_crit.min_match_value = local_group_crit.match_value - EPSILON;

   /** \action call function to check whether basic conditions for grouping are fulfilled. */
   ret = Pt_Are_Basic_Conditions_For_Grouping_Fulfilled(&local_group_crit, &global_group_crit, &cals);

   /** \assert expect false. */
   EXPECT_FALSE(ret);
}


/**
 * Check whether basic grouping criteria are fulfilled. Here the basic conditions are not fulfilled since local groupings match
 * value is exceeds the calibration difference. \uts{CSCSA-43579} \sdd{SF-7380} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Are_Basic_Conditions_For_Grouping_Fulfilled__basic_conditions_are_not_fulfilled_inner_loop_lt_cals)
{
   /** \arrange set up grouping criteria such that conditions are not fulfilled. */
   boolean_T ret;
   Pt_Local_Grouping_Criteria_T local_group_crit{};
   Pt_Global_Grouping_Criteria_T global_group_crit{};

   local_group_crit.match_value      = cals.k_pt_group_dir_max_avg_diff_value - EPSILON;
   global_group_crit.min_match_value = local_group_crit.match_value - EPSILON;

   /** \action call function to check whether basic conditions for grouping are fulfilled. */
   ret = Pt_Are_Basic_Conditions_For_Grouping_Fulfilled(&local_group_crit, &global_group_crit, &cals);

   /** \assert expect false. */
   EXPECT_FALSE(ret);
}


/**
 * Check whether overlapping pt_persistent.paths criteria are updated correctly. Here the criteria shall be updated since every
 * condition for the branch with min overlap count is fulfilled. \uts{CSCSA-43580} \sdd{SF-7398} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Update_Overlapping_Paths_Criteria__gt_min_overlap_count_and_lt_min_overlap_count_ad)
{
   /** \arrange set up conditions such that overlapping path criteria are updated. */
   Pt_Local_Grouping_Criteria_T local_group_crit{};
   Pt_Global_Grouping_Criteria_T global_group_crit{};

   local_group_crit.match_value               = cals.k_pt_group_dir_max_avg_diff_value - EPSILON;
   local_group_crit.f_intervals_overlapping   = FBK_TRUE;
   local_group_crit.count_path_points_overlap = pt_input.Num_Grid_Pts_Dep_Cals.k_pt_group_path_min_overlap_count + 1u;

   global_group_crit.min_match_value = cals.k_pt_group_dir_max_avg_diff_value;

   /** \action call function for grouping criteria update. */
   Pt_Update_Overlapping_Paths_Criteria(&global_group_crit, &local_group_crit, &cals, &pt_input.Num_Grid_Pts_Dep_Cals, 2u);

   /** \assert criteria to be mapped correctly. */
   EXPECT_EQ(global_group_crit.best_match_overlapping_paths, 2u);
   EXPECT_FLOAT_EQ(global_group_crit.min_match_value, local_group_crit.match_value);
}


/**
 * Check whether overlapping pt_persistent.paths criteria are updated correctly. Here the criteria shall be updated since every
 * condition for the branch with min overlap count ad is fulfilled. \uts{CSCSA-43581} \sdd{SF-7398}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Update_Overlapping_Paths_Criteria__gt_min_overlap_count_and_gt_min_overlap_count_ad)
{
   /** \arrange set up conditions such that overlapping path criteria are updated. */
   Pt_Local_Grouping_Criteria_T local_group_crit{};
   Pt_Global_Grouping_Criteria_T global_group_crit{};

   local_group_crit.match_value               = cals.k_pt_group_dir_max_avg_diff_value - EPSILON;
   local_group_crit.f_intervals_overlapping   = FBK_FALSE;
   local_group_crit.count_path_points_overlap = pt_input.Num_Grid_Pts_Dep_Cals.k_pt_group_path_min_overlap_count_ad + 1u;

   global_group_crit.min_match_value = cals.k_pt_group_dir_max_avg_diff_value;

   /** \action call function for grouping criteria update. */
   Pt_Update_Overlapping_Paths_Criteria(&global_group_crit, &local_group_crit, &cals, &pt_input.Num_Grid_Pts_Dep_Cals, 2u);

   /** \assert criteria to be mapped correctly. */
   EXPECT_EQ(global_group_crit.best_match_non_overlapping_paths, 2u);
   EXPECT_FLOAT_EQ(global_group_crit.min_match_value, local_group_crit.match_value);
}


/**
 * Check whether overlapping pt_persistent.paths criteria are updated correctly. Here the criteria shall not be updated since no
 * intervals are overlapping and the overlap count is less than the ad threshold. \uts{CSCSA-43582} \sdd{SF-7398}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Update_Overlapping_Paths_Criteria__no_overlaps_given_and_count_is_less_than_ad_threshold)
{
   /** \arrange set up conditions such that overlapping path criteria are not updated. */
   Pt_Local_Grouping_Criteria_T local_group_crit{};
   Pt_Global_Grouping_Criteria_T global_group_crit{};

   local_group_crit.match_value               = cals.k_pt_group_dir_max_avg_diff_value - EPSILON;
   local_group_crit.f_intervals_overlapping   = FBK_FALSE;
   local_group_crit.count_path_points_overlap = pt_input.Num_Grid_Pts_Dep_Cals.k_pt_group_path_min_overlap_count_ad - 1u;

   global_group_crit.min_match_value = cals.k_pt_group_dir_max_avg_diff_value;

   /** \action call function for grouping criteria update. */
   Pt_Update_Overlapping_Paths_Criteria(&global_group_crit, &local_group_crit, &cals, &pt_input.Num_Grid_Pts_Dep_Cals, 2u);

   /** \assert criteria to be not mapped. */
   EXPECT_EQ(global_group_crit.best_match_non_overlapping_paths, 0u);
   EXPECT_EQ(global_group_crit.best_match_overlapping_paths, 0u);
   EXPECT_FLOAT_EQ(global_group_crit.min_match_value, cals.k_pt_group_dir_max_avg_diff_value);
}


/**
 * Test whether the less established path is reset correctly. Here the second path is reset, since it is not as established as the
 * first one. \uts{CSCSA-43583} \sdd{SF-7395} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Reset_Less_Established_Path__second_path_is_reset)
{
   /** \arrange set up conditions such that the second input path is reset. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx], 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx], 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[0].num_groupings = 5u;
   pt_persistent.paths[0].path_state    = PATH_STATUS_MATURE;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx,
                         upper_idx + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_diff_num_path_points + 1u, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 1.5f, 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[1].num_groupings = pt_persistent.paths[0].num_groupings - 1u;
   pt_persistent.paths[1].path_state    = pt_persistent.paths[0].path_state;

   /** \action call function for reset of less established pt_persistent.paths. */
   Pt_Reset_Less_Established_Path(&pt_persistent.paths[0], &pt_persistent.paths[1], best_path_obj_pairs,
                                  &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect second path to be reset. */
   EXPECT_EQ(PATH_STATUS_DEFAULT, pt_persistent.paths[1].path_state);
}


/**
 * Test whether the less established path is reset correctly. Here the first path is reset, since it is not as established as the
 * second one. \uts{CSCSA-43584} \sdd{SF-7395} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Reset_Less_Established_Path__first_path_is_reset)
{
   /** \arrange set up conditions such that the first input path is reset. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx], 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx], 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[0].num_groupings = 5u;
   pt_persistent.paths[0].path_state    = PATH_STATUS_MATURE;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx,
                         upper_idx + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_diff_num_path_points + 1u, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 1.5f, 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[1].num_groupings = pt_persistent.paths[0].num_groupings + 1u;
   pt_persistent.paths[1].path_state    = pt_persistent.paths[0].path_state;

   /** \action call function for reset of less established pt_persistent.paths. */
   Pt_Reset_Less_Established_Path(&pt_persistent.paths[0], &pt_persistent.paths[1], best_path_obj_pairs,
                                  &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect first path to be reset. */
   EXPECT_EQ(PATH_STATUS_DEFAULT, pt_persistent.paths[0].path_state);
}


/**
 * Test whether the less established path is reset correctly. Here both pt_persistent.paths are equally established, thus both
 * pt_persistent.paths shall be kept. \uts{CSCSA-43585} \sdd{SF-7395} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Reset_Less_Established_Path__both_paths_are_equally_established)
{
   /** \arrange set up conditions such that the both pt_persistent.paths are kept. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx], 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx], 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[0].num_groupings = 5u;
   pt_persistent.paths[0].path_state    = PATH_STATUS_MATURE;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx,
                         upper_idx + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_diff_num_path_points + 1u, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 1.5f, 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[1].num_groupings = pt_persistent.paths[0].num_groupings;
   pt_persistent.paths[1].path_state    = pt_persistent.paths[0].path_state;

   /** \action call function for reset of less established pt_persistent.paths. */
   Pt_Reset_Less_Established_Path(&pt_persistent.paths[0], &pt_persistent.paths[1], best_path_obj_pairs,
                                  &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect both pt_persistent.paths to be kept. */
   EXPECT_EQ(PATH_STATUS_MATURE, pt_persistent.paths[0].path_state);
   EXPECT_EQ(PATH_STATUS_MATURE, pt_persistent.paths[1].path_state);
}


/**
 * Test whether the less established path is reset correctly. Here both pt_persistent.paths are equally long, so that the reset of
 * less established pt_persistent.paths shall not execute. \uts{CSCSA-43586} \sdd{SF-7395} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Reset_Less_Established_Path__both_paths_same_length_and_shall_be_kept)
{
   /** \arrange set up conditions such that the both pt_persistent.paths are kept. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx], 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx], 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[0].num_groupings = 5u;
   pt_persistent.paths[0].path_state    = PATH_STATUS_MATURE;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 1.5f, 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[1].num_groupings = pt_persistent.paths[0].num_groupings;
   pt_persistent.paths[1].path_state    = pt_persistent.paths[0].path_state;

   /** \action call function for reset of less established pt_persistent.paths. */
   Pt_Reset_Less_Established_Path(&pt_persistent.paths[0], &pt_persistent.paths[1], best_path_obj_pairs,
                                  &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect both pt_persistent.paths to be kept. */
   EXPECT_EQ(PATH_STATUS_MATURE, pt_persistent.paths[0].path_state);
   EXPECT_EQ(PATH_STATUS_MATURE, pt_persistent.paths[1].path_state);
}


/**
 * Check whether the path pair is valid for group overlapping. Here the path pair is valid for group overlapping, thus true is
 * expected. \uts{CSCSA-43587} \sdd{SF-7390} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Group_Overlapping__path_pair_is_valid_for_group_overlapping)
{
   /** \arrange set up a path pair which is valid for the group overlapping. */
   boolean_T ret;
   pt_persistent.paths[0].direction  = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[1].direction  = pt_persistent.paths[0].direction;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index + 1u;
   pt_persistent.paths[1].path_state = pt_persistent.paths[0].path_state;


   /** \action call validity check of group overlapping. */
   ret = Pt_Is_Path_Pair_Valid_For_Group_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect path pair to be valid. */
   EXPECT_TRUE(ret);
}


/**
 * Check whether the path pair is valid for group overlapping. Here the second path was already grouped in this cycle. Thus false
 * is expected. \uts{CSCSA-43588} \sdd{SF-7390} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Group_Overlapping__second_path_was_already_grouped_in_this_cylce)
{
   /** \arrange set up a path pair which is invalid for the group overlapping. */
   boolean_T ret;
   pt_persistent.paths[0].direction  = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[1].direction  = pt_persistent.paths[0].direction;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index + 1u;
   pt_persistent.paths[1].path_state = PATH_STATUS_GROUPED_IN_CURRENT_CYCLE;

   /** \action call validity check of group overlapping. */
   ret = Pt_Is_Path_Pair_Valid_For_Group_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect path pair to be invalid. */
   EXPECT_FALSE(ret);
}


/**
 * Check whether the path pair is valid for group overlapping. Here the second path is longitudinal. Thus false is expected.
 * \uts{CSCSA-43589} \sdd{SF-7390} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Group_Overlapping__second_path_is_longitudinal)
{
   /** \arrange set up a path pair which is invalid for the group overlapping. */
   boolean_T ret;
   pt_persistent.paths[0].direction  = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[1].direction  = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index + 1u;

   /** \action call validity check of group overlapping. */
   ret = Pt_Is_Path_Pair_Valid_For_Group_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect path pair to be invalid. */
   EXPECT_FALSE(ret);
}

/**
 * Check whether the path pair is valid for group overlapping. Here the second has the same index as the first. Thus false is
 * expected. \uts{CSCSA-43590} \sdd{SF-7390} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Group_Overlapping__second_path_has_equal_index_compared_to_first_one)
{
   /** \arrange set up a path pair which is invalid for the group overlapping. */
   boolean_T ret;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index;

   /** \action call validity check of group overlapping. */
   ret = Pt_Is_Path_Pair_Valid_For_Group_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect path pair to be invalid. */
   EXPECT_FALSE(ret);
}


/**
 * Check whether the path pair is valid for group overlapping. Here the second path has still assigned the object to it. Thus false
 * is expected. \uts{CSCSA-43591} \sdd{SF-7390} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Group_Overlapping__object_is_still_assigned_to_second_path)
{
   /** \arrange set up a path pair which is invalid for the group overlapping. */
   boolean_T ret;
   pt_persistent.paths[1].obj_curr_used_for_path_build.id = 1u;

   /** \action call validity check of group overlapping. */
   ret = Pt_Is_Path_Pair_Valid_For_Group_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect path pair to be invalid. */
   EXPECT_FALSE(ret);
}


/**
 * Check whether the local grouping criteria is reset correctly.
 * \uts{CSCSA-43592} \sdd{SF-7396} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Reset_Local_Grouping_Criteria__check_the_initialization)
{
   /** \arrange set up a non default input. */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   inner_loop_crit.count_path_points_overlap   = 1;
   inner_loop_crit.index_to_path_overlap_count = 1;
   inner_loop_crit.path_point_diff             = 1.0f;
   inner_loop_crit.match_value                 = 1.0f;
   inner_loop_crit.count_path_points_overlap   = 1u;
   inner_loop_crit.f_intervals_overlapping     = FBK_TRUE;

   /** \action call constructor of local grouping criteria . */
   Pt_Reset_Local_Grouping_Criteria(&inner_loop_crit);

   /** \assert expect default values to be set. */
   EXPECT_FLOAT_EQ(inner_loop_crit.path_point_diff, 0.0f);
   EXPECT_EQ(inner_loop_crit.count_path_points_overlap, 0);
   EXPECT_EQ(inner_loop_crit.index_to_path_overlap_count, 0);
   EXPECT_FLOAT_EQ(inner_loop_crit.match_value, 0.0f);
   EXPECT_FLOAT_EQ(inner_loop_crit.max_path_point_diff, 0.0f);
   EXPECT_FALSE(inner_loop_crit.f_intervals_overlapping);
}


/**
 * Check whether the global grouping criteria is reset correctly.
 * \uts{CSCSA-43593} \sdd{SF-7394} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Reset_Global_Grouping_Properties__check_the_initialization)
{
   /** \arrange set up a non default input. */
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   outer_loop_properties.best_match_overlapping_paths               = 1;
   outer_loop_properties.best_match_overlapping_paths               = 1;
   outer_loop_properties.count_path_point_overlaps_across_all_paths = 1;
   outer_loop_properties.min_match_value                            = 2.0f;


   /** \action call constructor of global grouping criteria . */
   Pt_Reset_Global_Grouping_Properties(&outer_loop_properties, &cals);

   /** \assert expect default values to be set. */
   EXPECT_EQ(outer_loop_properties.best_match_overlapping_paths, PT_DEFAULT_MATCH_INDEX);
   EXPECT_EQ(outer_loop_properties.best_match_non_overlapping_paths, PT_DEFAULT_MATCH_INDEX);
   EXPECT_EQ(outer_loop_properties.count_path_point_overlaps_across_all_paths, 0);
   EXPECT_FLOAT_EQ(outer_loop_properties.min_match_value, cals.k_pt_overlap_max_match_value);
}


/**
 * Check whether path points are overlapping. Here the two path points are overlapping
 * \uts{CSCSA-43594} \sdd{SF-7381} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Are_Path_Points_Overlapping__points_are_strongly_overlapping)
{
   /** \arrange set nested pt_persistent.paths with a low path point difference. */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   boolean_T ret;
   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].path_points[path_point_index]                               = 1.0f;
   pt_persistent.paths[0].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.5f;
   pt_persistent.paths[1].path_points[path_point_index]                               = 1.2f;
   pt_persistent.paths[1].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.4f;
   inner_loop_crit.path_point_diff = cals.k_pt_group_max_match_value_ad - EPSILON;

   /** \action call overlapping check. */
   ret = Pt_Are_Path_Points_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1], path_point_index, &inner_loop_crit, &cals);

   /** \assert expect true. */
   EXPECT_TRUE(ret);
}


/**
 * Check whether path points are overlapping. Here the two path points are overlapping. The path point difference is gt the thres
 * but the points are nested. \uts{CSCSA-43595} \sdd{SF-7381} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Are_Path_Points_Overlapping__points_are_strongly_overlapping_since_they_are_nested)
{
   /** \arrange set nested pt_persistent.paths with a low path point difference. */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   boolean_T ret;
   uint8_t path_point_index = 16;

   pt_persistent.paths[0].path_points[path_point_index]                               = 1.0f;
   pt_persistent.paths[0].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.5f;
   pt_persistent.paths[1].path_points[path_point_index]                               = 1.2f;
   pt_persistent.paths[1].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.4f;
   inner_loop_crit.path_point_diff = cals.k_pt_group_max_match_value_ad + EPSILON;

   /** \action call overlapping check. */
   ret = Pt_Are_Path_Points_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1], path_point_index, &inner_loop_crit, &cals);

   /** \assert expect true. */
   EXPECT_TRUE(ret);
}


/**
 * Check whether path points are overlapping. Here the path point difference exceeds the boundary and the pt_persistent.paths are
 * not nested. Thus false is expected. \uts{CSCSA-43596} \sdd{SF-7381} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Are_Path_Points_Overlapping__path_point_diff_exceeds_boundary_and_unnested_paths)
{
   /** \arrange set unnested pt_persistent.paths with a low path point difference and path point difference gt threshold. */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   boolean_T ret;
   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].path_points[path_point_index]                               = 1.0f;
   pt_persistent.paths[0].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.5f;
   pt_persistent.paths[1].path_points[path_point_index]                               = 1.6f;
   pt_persistent.paths[1].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.8f;
   inner_loop_crit.path_point_diff = cals.k_pt_group_max_match_value_ad + EPSILON;

   /** \action call overlapping check. */
   ret = Pt_Are_Path_Points_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1], path_point_index, &inner_loop_crit, &cals);

   /** \assert expect false. */
   EXPECT_FALSE(ret);
}


/**
 * Check whether path points are overlapping. Here the path index to the is out of area to check and the path point diff is gt
 * threshold. Thus false is expected. \uts{CSCSA-43597} \sdd{SF-7381} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Are_Path_Points_Overlapping__path_point_diff_gt_thres_and_path_index_gt_thres)
{
   /** \arrange set unnested pt_persistent.paths with a low path point difference and path point difference gt threshold. */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   boolean_T ret;
   uint8_t path_point_index        = cals.k_pt_end_of_lane_change_processing;
   inner_loop_crit.path_point_diff = cals.k_pt_group_max_match_value_ad + EPSILON;

   /** \action call overlapping check. */
   ret = Pt_Are_Path_Points_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1], path_point_index, &inner_loop_crit, &cals);

   /** \assert expect false. */
   EXPECT_FALSE(ret);
}


/**
 * Checks the grouping criteria update routine. Here the path points are overlapping and thus the intervals are nested. It is
 * expected that the counters are increased based on the given overlapping. \uts{CSCSA-43598} \sdd{SF-7397}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Update_Grouping_Crit__points_are_overlapping_thus_counter_shall_be_increased)
{
   /** \arrange set set up nested pt_persistent.paths and an uninitialized inner loop criteria . */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T point_quality = POINT_IN_ONLY_ONE_PATH;

   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].path_points[path_point_index]                               = 1.0f;
   pt_persistent.paths[0].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.5f;
   pt_persistent.paths[1].path_points[path_point_index]                               = 1.2f;
   pt_persistent.paths[1].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.4f;

   inner_loop_crit.count_path_points_overlap = PT_INITIAL_PATH_POINT_OVERLAP_COUNT_CURRENT_PATH;
   inner_loop_crit.f_intervals_overlapping   = FBK_TRUE;

   /** \action call grouping criteria update routine. */
   Pt_Update_Grouping_Crit(&inner_loop_crit, &outer_loop_properties, &pt_persistent.paths[0], &pt_persistent.paths[1], &cals,
                           path_point_index, point_quality);

   /** \assert expect counter for overlap as well as counter for overlaps across all pt_persistent.paths to be increased. */
   EXPECT_EQ(inner_loop_crit.count_path_points_overlap, 1);
   EXPECT_EQ(outer_loop_properties.count_path_point_overlaps_across_all_paths, 1);
}


/**
 * Checks the grouping criteria update routine. Here the path points are overlapping and thus the intervals are nested. However the
 * global properties have been increased in this cycle already. Thus it is expected that only the counter of path points
 * overlapping is increased. overlapping. \uts{CSCSA-43599} \sdd{SF-7397} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test,
       Pt_Update_Grouping_Crit__points_are_overlapping_global_crit_is_already_updated_only_path_pointer_shall_increment)
{
   /** \arrange set set up nested pt_persistent.paths and an already increased outer loop criteria. */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T point_quality = POINT_IN_ONLY_ONE_PATH;

   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].path_points[path_point_index]                               = 1.0f;
   pt_persistent.paths[0].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.5f;
   pt_persistent.paths[1].path_points[path_point_index]                               = 1.2f;
   pt_persistent.paths[1].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.4f;

   outer_loop_properties.count_path_point_overlaps_across_all_paths = 1;
   inner_loop_crit.count_path_points_overlap                        = 1;
   inner_loop_crit.f_intervals_overlapping                          = FBK_TRUE;

   /** \action call grouping criteria update routine. */
   Pt_Update_Grouping_Crit(&inner_loop_crit, &outer_loop_properties, &pt_persistent.paths[0], &pt_persistent.paths[1], &cals,
                           path_point_index, point_quality);

   /** \assert expect counter for overlap as well to be increased but the counter for overlaps across all pt_persistent.paths to
    * remain. */
   EXPECT_EQ(inner_loop_crit.count_path_points_overlap, 2);
   EXPECT_EQ(outer_loop_properties.count_path_point_overlaps_across_all_paths, 1);
}


/**
 * Checks the grouping criteria update routine. Here the path point is contained in both pt_persistent.paths and the path point
 * diff is less than the given thresholds \uts{CSCSA-43600} \sdd{SF-7397} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test,
       Pt_Update_Grouping_Crit__intervals_are_not_nested_but_point_is_contained_in_both_paths_while_path_point_diff_lt_thresholds)
{
   /** \arrange set set up non nested pt_persistent.paths with a path point index contained in both pt_persistent.paths. */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T point_quality = POINT_IN_BOTH_PATHS;

   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].path_points[path_point_index]                               = 1.0f;
   pt_persistent.paths[0].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.2f;
   pt_persistent.paths[1].path_points[path_point_index]                               = 1.3f;
   pt_persistent.paths[1].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.5f;

   inner_loop_crit.f_intervals_overlapping = FBK_TRUE;

   /** \action call grouping criteria update routine. */
   Pt_Update_Grouping_Crit(&inner_loop_crit, &outer_loop_properties, &pt_persistent.paths[0], &pt_persistent.paths[1], &cals,
                           path_point_index, point_quality);

   /** \assert expect the counter for containment of a point in both intervals to be increased. */
   EXPECT_EQ(inner_loop_crit.index_to_path_overlap_count, 1);
}


/**
 * Checks the grouping criteria update routine. Here the path is nested given by the inner loop criteria. But the current point is
 * not contained in both pt_persistent.paths. However the path point difference is exceeding both thresholds. Thus it is expected
 * that the match value is increased as well as the maximum path point difference. \uts{CSCSA-43601} \sdd{SF-7397}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Update_Grouping_Crit__paths_are_nested_somewhere_and_path_point_diff_is_updated_since_gt_thresholds)
{
   /** \arrange set set up non nested pt_persistent.paths with a path point index contained in only one path. However the intervals
    * of the pt_persistent.paths are somewhere overlapping */
   Pt_Local_Grouping_Criteria_T inner_loop_crit{};
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T point_quality = POINT_IN_ONLY_ONE_PATH;

   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].path_points[path_point_index]                               = 1.0f;
   pt_persistent.paths[0].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.2f;
   pt_persistent.paths[1].path_points[path_point_index]                               = 1.3f;
   pt_persistent.paths[1].path_points[path_point_index + PT_SINGLE_GRID_POINT_OFFSET] = 1.5f;

   inner_loop_crit.f_intervals_overlapping = FBK_FALSE;
   inner_loop_crit.path_point_diff = Max(inner_loop_crit.max_path_point_diff, cals.k_pt_group_dir_max_diff_value) + EPSILON;

   /** \action call grouping criteria update routine. */
   Pt_Update_Grouping_Crit(&inner_loop_crit, &outer_loop_properties, &pt_persistent.paths[0], &pt_persistent.paths[1], &cals,
                           path_point_index, point_quality);

   /** \assert expect match value and maximum path point diff to be updated. */
   EXPECT_FLOAT_EQ(inner_loop_crit.match_value, inner_loop_crit.path_point_diff);
   EXPECT_FLOAT_EQ(inner_loop_crit.max_path_point_diff, inner_loop_crit.path_point_diff);
}


/**
 * Checks whether a given index is contained in both pt_persistent.paths. Here it is contained in both pt_persistent.paths
 * \uts{CSCSA-43602} \sdd{SF-7382} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Determine_If_Index_Is_Contained_In_Paths__point_is_contained_in_both_paths)
{
   /** \arrange set up an index which contained in both input pt_persistent.paths */
   Pt_Overlap_Quality_T point_quality;
   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].last_p  = ((uint8_t) path_point_index) + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[0].first_p = ((uint8_t) path_point_index) - PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p  = ((uint8_t) path_point_index) + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].first_p = ((uint8_t) path_point_index) - PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call point quality determination. */
   point_quality = Pt_Determine_If_Index_Is_Contained_In_Paths(&pt_persistent.paths[0], &pt_persistent.paths[1], path_point_index);

   /** \assert expect point to be contained in both pt_persistent.paths. */
   EXPECT_EQ(point_quality, POINT_IN_BOTH_PATHS);
}


/**
 * Checks whether a given index is contained in both pt_persistent.paths. Here it is contained in first path but not in the second.
 * \uts{CSCSA-43603} \sdd{SF-7382} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Determine_If_Index_Is_Contained_In_Paths__point_is_contained_in_first_path_but_not_in_second)
{
   /** \arrange set up an index which contained in only one input path */
   Pt_Overlap_Quality_T point_quality;
   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].last_p  = ((uint8_t) path_point_index) + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[0].first_p = ((uint8_t) path_point_index) - PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p  = ((uint8_t) path_point_index) - PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].first_p = ((uint8_t) path_point_index) - 2 * PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call point quality determination. */
   point_quality = Pt_Determine_If_Index_Is_Contained_In_Paths(&pt_persistent.paths[0], &pt_persistent.paths[1], path_point_index);

   /** \assert expect point to be contained in only one path. */
   EXPECT_EQ(point_quality, POINT_IN_ONLY_ONE_PATH);
}


/**
 * Checks whether a given index is contained in both pt_persistent.paths. Here it is contained second path but not in the first.
 * \uts{CSCSA-43604} \sdd{SF-7382} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Determine_If_Index_Is_Contained_In_Paths__point_is_contained_in_second_path_but_not_in_first)
{
   /** \arrange set up an index which contained in only one input path */
   Pt_Overlap_Quality_T point_quality;
   uint8_t path_point_index = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[0].last_p  = ((uint8_t) path_point_index) - PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[0].first_p = ((uint8_t) path_point_index) - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p  = ((uint8_t) path_point_index) + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].first_p = ((uint8_t) path_point_index) - PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call point quality determination. */
   point_quality = Pt_Determine_If_Index_Is_Contained_In_Paths(&pt_persistent.paths[0], &pt_persistent.paths[1], path_point_index);

   /** \assert expect point to be contained in only one path. */
   EXPECT_EQ(point_quality, POINT_IN_ONLY_ONE_PATH);
}

/**
 * Checks whether a given index is contained in both pt_persistent.paths. Here it not contained in either path a or path b.
 * \uts{CSCSA-43605} \sdd{SF-7382} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Determine_If_Index_Is_Contained_In_Paths__point_is_not_contained_anywhere)
{
   /** \arrange set up an index which is not contained anywhere. */
   Pt_Overlap_Quality_T point_quality;
   uint8_t path_point_index = 1u;

   pt_persistent.paths[0].last_p  = (PT_MID_GRID_POINT_INDEX) -PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[0].first_p = (PT_MID_GRID_POINT_INDEX) -2 * PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p  = (PT_MID_GRID_POINT_INDEX) + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].first_p = (PT_MID_GRID_POINT_INDEX) -PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call point quality determination. */
   point_quality = Pt_Determine_If_Index_Is_Contained_In_Paths(&pt_persistent.paths[0], &pt_persistent.paths[1], path_point_index);

   /** \assert expect point to be not contained by any path. */
   EXPECT_EQ(point_quality, POINT_NOT_IN_PATH_PAIR);
}


/**
 * Test the functionality of path grouping. Here both inputs pt_persistent.paths have already been grouped previously. and they are
 * covering the whole grid area. \uts{CSCSA-43606} \sdd{SF-7387} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Weight_Path__already_grouped_paths_covering_the_whole_grid_array)
{
   /** \arrange set up two pt_persistent.paths which are in status grouped and which are covering the whole grid array range */
   Pt_Overlap_Quality_T overlaps[PT_NUM_GRID_POINTS];
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      overlaps[idx] = POINT_IN_BOTH_PATHS;
   }

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 5u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 3.0f, 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[0].path_state = PATH_STATUS_GROUPED;
   pt_persistent.paths[0].path_age   = 50u;
   pt_persistent.paths[0].max_speed  = 25.0f;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 5u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.5f, 1.0f), 0.0f, -0.8f);
   pt_persistent.paths[1].path_state = PATH_STATUS_GROUPED;
   pt_persistent.paths[1].path_age   = 150u;
   pt_persistent.paths[1].max_speed  = 25.0f;

   /** \action call function to weight path points of different pt_persistent.paths. */
   Pt_Group_Weight_Path(&pt_persistent.paths[0], &pt_persistent.paths[1], pt_persistent.best_path_obj_pairs, &cals,
                        pt_input.grid_pt_array, overlaps);

   /** \assert expect path points to be the mean of the input path points and also check that the mapping is done correctly. */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[0].path_points[idx], -0.9f);
   }
   EXPECT_EQ(pt_persistent.paths[0].path_age, 150u);
   EXPECT_EQ(pt_persistent.paths[0].max_speed, 25.0f);
   EXPECT_EQ(pt_persistent.paths[0].path_state, PATH_STATUS_GROUPED_IN_CURRENT_CYCLE);
   EXPECT_EQ(pt_persistent.paths[0].num_groupings, 6u);
}


/**
 * Test the functionality of path grouping. Here the input pt_persistent.paths are not covering the whole grid area and the second
 * path is covering a bigger range. \uts{CSCSA-43607} \sdd{SF-7387} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Weight_Path__mature_paths_where_points_are_partly_contained_in_only_one_path)
{
   /** \arrange set up two pt_persistent.paths which are in status mature and the second path extends the range of the first and
    * set up the overlaps accordingly. */

   /*Set up the overlaps*/
   Pt_Overlap_Quality_T overlaps[PT_NUM_GRID_POINTS];
   uint8_t lower_border_first_path = PT_MID_GRID_POINT_INDEX;
   uint8_t upper_border_first_path = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t lower_border_sec_path   = lower_border_first_path - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_border_sec_path   = upper_border_first_path;
   uint8_t min_border_total        = Min(lower_border_sec_path, lower_border_first_path);
   uint8_t max_border_total        = Min(upper_border_first_path, upper_border_sec_path);
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      if (idx < min_border_total || idx > max_border_total)
      {
         overlaps[idx] = POINT_NOT_IN_PATH_PAIR;
      }
      else if (lower_border_sec_path <= idx && idx < lower_border_first_path)
      {
         overlaps[idx] = POINT_IN_ONLY_ONE_PATH;
      }
      else
      {
         overlaps[idx] = POINT_IN_BOTH_PATHS;
      }
   }


   /*Set up the extrapolated versions of pt_persistent.paths*/
   float32_T path_val_first_path = -1.0f;
   float32_T path_val_sec_path   = -0.5f;
   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_border_first_path,
                         upper_border_first_path, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_border_first_path] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_border_first_path] + 3.0f, 1.0f), 0.0f,
                         path_val_first_path);
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[0].path_age   = 50u;
   pt_persistent.paths[0].max_speed  = 25.0f;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_border_sec_path,
                         upper_border_sec_path, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_border_sec_path] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_border_sec_path] + 1.5f, 1.0f), 0.0f,
                         path_val_sec_path);
   pt_persistent.paths[1].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[1].path_age   = 150u;
   pt_persistent.paths[1].max_speed  = 25.0f;

   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      pt_persistent.paths[0].path_points[idx] = path_val_first_path;
      pt_persistent.paths[1].path_points[idx] = path_val_sec_path;
   }

   /** \action call function to weight path points of different pt_persistent.paths. */
   Pt_Group_Weight_Path(&pt_persistent.paths[0], &pt_persistent.paths[1], pt_persistent.best_path_obj_pairs, &cals,
                        pt_input.grid_pt_array, overlaps);

   /** \assert expect path points to be the mean of the input path points at the grid points where points are contained in both
    * pt_persistent.paths and expect that the rest of the points are determined via a weighted mean procedure . */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      if (POINT_IN_BOTH_PATHS == overlaps[idx])
      {
         EXPECT_FLOAT_EQ(pt_persistent.paths[0].path_points[idx], -0.75f);
      }
      else if (POINT_IN_ONLY_ONE_PATH == overlaps[idx])
      {
         EXPECT_FLOAT_EQ(pt_persistent.paths[0].path_points[idx], -0.5f);
      }
      else
      {
      }
   }
   EXPECT_EQ(pt_persistent.paths[0].path_age, 150u);
   EXPECT_EQ(pt_persistent.paths[0].max_speed, 25.0f);
   EXPECT_EQ(pt_persistent.paths[0].path_state, PATH_STATUS_GROUPED_IN_CURRENT_CYCLE);
   EXPECT_EQ(pt_persistent.paths[0].num_groupings, 1u);
}


/**
 * Evaluates the path criteria.Here too many redundant pt_persistent.paths are given and thus the input path shall be reset.
 * \uts{CSCSA-43608} \sdd{SF-7383} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Evaluate_Overlapping_Paths_Criteria__too_many_overlaps_across_paths_are_given_path_shall_be_reset)
{
   /** \arrange set up two many overlaps */
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T overlaps[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS]{};

   outer_loop_properties.count_path_point_overlaps_across_all_paths = PT_MAX_ALLOWED_PATHS_WITH_PATH_POINT_OVERLAPS + 1;
   pt_persistent.paths[1].path_state                                = PATH_STATUS_MATURE;

   /** \action evaluation of overlapping criteria function. */
   Pt_Evaluate_Overlapping_Paths_Criteria(&pt_persistent, &pt_persistent.paths[1], overlaps, &outer_loop_properties, &cals,
                                          pt_input.grid_pt_array);

   /** \assert expect first path index to be reset. */
   EXPECT_EQ(pt_persistent.paths[1].path_state, PATH_STATUS_DEFAULT);
}


/**
 * Evaluates the path criteria. Here two input pt_persistent.paths are set up and shall be grouped based on the best match of
 * overlapping pt_persistent.paths index. \uts{CSCSA-43609} \sdd{SF-7383} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Evaluate_Overlapping_Paths_Criteria__weight_path_zero_and_one_based_on_best_overlapping_path_match)
{
   /** \arrange set up two pt_persistent.paths which are overlapping */
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T overlaps[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS]{};

   outer_loop_properties.count_path_point_overlaps_across_all_paths = PT_MAX_ALLOWED_PATHS_WITH_PATH_POINT_OVERLAPS;
   outer_loop_properties.best_match_overlapping_paths               = 0;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 3.0f, 1.0f), 0.0f, 0.5f);
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[0].path_age   = 50u;
   pt_persistent.paths[0].max_speed  = 25.0f;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.5f, 1.0f), 0.0f, 1.0f);
   pt_persistent.paths[1].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[1].path_age   = 150u;
   pt_persistent.paths[1].max_speed  = 25.0f;


   /** \action evaluation of overlapping criteria function. */
   Pt_Evaluate_Overlapping_Paths_Criteria(&pt_persistent, &pt_persistent.paths[1], overlaps, &outer_loop_properties, &cals,
                                          pt_input.grid_pt_array);

   /** \assert expect pt_persistent.paths to be grouped. */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[1].path_points[idx], 0.75f);
   }
   EXPECT_EQ(pt_persistent.paths[1].path_age, 150u);
   EXPECT_EQ(pt_persistent.paths[1].max_speed, 25.0f);
   EXPECT_EQ(pt_persistent.paths[1].path_state, PATH_STATUS_GROUPED_IN_CURRENT_CYCLE);
   EXPECT_EQ(pt_persistent.paths[1].num_groupings, 1u);
}


/**
 * Evaluates the path criteria. Here two input pt_persistent.paths are set up and shall be grouped based on the best match of non
 * overlapping pt_persistent.paths index. \uts{CSCSA-43610} \sdd{SF-7383} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Evaluate_Overlapping_Paths_Criteria__weight_path_zero_and_one_based_on_best_non_overlapping_path_match)
{
   /** \arrange set up two pt_persistent.paths which are not overlapping but shall still be weighted. */
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T overlaps[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS]{};

   outer_loop_properties.count_path_point_overlaps_across_all_paths = PT_MAX_ALLOWED_PATHS_WITH_PATH_POINT_OVERLAPS;
   outer_loop_properties.best_match_overlapping_paths               = PT_DEFAULT_MATCH_INDEX;
   outer_loop_properties.best_match_non_overlapping_paths           = 0;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 3.0f, 1.0f), 0.0f, 0.5f);
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[0].path_age   = 50u;
   pt_persistent.paths[0].max_speed  = 25.0f;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.5f, 1.0f), 0.0f, 1.0f);
   pt_persistent.paths[1].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[1].path_age   = 150u;
   pt_persistent.paths[1].max_speed  = 25.0f;


   /** \action evaluation of overlapping criteria function. */
   Pt_Evaluate_Overlapping_Paths_Criteria(&pt_persistent, &pt_persistent.paths[1], overlaps, &outer_loop_properties, &cals,
                                          pt_input.grid_pt_array);

   /** \assert expect pt_persistent.paths to be grouped. */
   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[1].path_points[idx], 0.75f);
   }
   EXPECT_EQ(pt_persistent.paths[1].path_age, 150u);
   EXPECT_EQ(pt_persistent.paths[1].max_speed, 25.0f);
   EXPECT_EQ(pt_persistent.paths[1].path_state, PATH_STATUS_GROUPED_IN_CURRENT_CYCLE);
   EXPECT_EQ(pt_persistent.paths[1].num_groupings, 1u);
}


/**
 * Evaluates the path criteria. Here no input match is given and thus no grouping shall occure.
 * \uts{CSCSA-43611} \sdd{SF-7383} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Evaluate_Overlapping_Paths_Criteria__no_best_match_found)
{
   /** \arrange set up inputs such that no best match can be found. */
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T overlaps[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS]{};

   outer_loop_properties.count_path_point_overlaps_across_all_paths = PT_MAX_ALLOWED_PATHS_WITH_PATH_POINT_OVERLAPS;
   outer_loop_properties.best_match_overlapping_paths               = PT_DEFAULT_MATCH_INDEX;
   outer_loop_properties.best_match_non_overlapping_paths           = PT_DEFAULT_MATCH_INDEX;

   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_MATURE;
   }

   /** \action evaluation of overlapping criteria function. */
   Pt_Evaluate_Overlapping_Paths_Criteria(&pt_persistent, &pt_persistent.paths[1], overlaps, &outer_loop_properties, &cals,
                                          pt_input.grid_pt_array);

   /** \assert expect pt_persistent.paths to remain. */
   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      EXPECT_EQ(pt_persistent.paths[idx].path_state, PATH_STATUS_MATURE);
   }
}


/**
 * Evaluates the path criteria. Here a input match is given but the age condition is not fulfilled.
 * \uts{CSCSA-43612} \sdd{SF-7383} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Evaluate_Overlapping_Paths_Criteria__match_is_found_but_age_condition_is_not_fulfilled)
{
   /** \arrange set up inputs such that no grouping occures. */
   Pt_Global_Grouping_Criteria_T outer_loop_properties{};
   Pt_Overlap_Quality_T overlaps[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS]{};

   outer_loop_properties.count_path_point_overlaps_across_all_paths = PT_MAX_ALLOWED_PATHS_WITH_PATH_POINT_OVERLAPS;
   outer_loop_properties.best_match_overlapping_paths               = PT_DEFAULT_MATCH_INDEX;
   outer_loop_properties.best_match_non_overlapping_paths           = 0;

   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_MATURE;
   }
   pt_persistent.paths[outer_loop_properties.best_match_non_overlapping_paths].path_age = 0;

   /** \action evaluation of overlapping criteria function. */
   Pt_Evaluate_Overlapping_Paths_Criteria(&pt_persistent, &pt_persistent.paths[1], overlaps, &outer_loop_properties, &cals,
                                          pt_input.grid_pt_array);

   /** \assert expect pt_persistent.paths to remain. */
   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      EXPECT_EQ(pt_persistent.paths[idx].path_state, PATH_STATUS_MATURE);
   }
}

/**
 * Call the cross grouping routine but here all pt_persistent.paths are in creation mode.
 * \uts{CSCSA-43613} \sdd{SF-7385} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Cross_Paths__no_valid_cross_grouping_path_is_given)
{
   /** \arrange set up inputs such that no cross grouping occures. */

   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state                      = PATH_STATUS_CREATION;
      pt_persistent.paths[idx].obj_curr_used_for_path_build.id = idx;
   }

   /** \action call cross grouping function. */
   Pt_Group_Cross_Paths(&pt_persistent, &pt_persistent.paths[1], &cals, pt_input.grid_pt_array);

   /** \assert expect pt_persistent.paths to remain. */
   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      EXPECT_EQ(pt_persistent.paths[idx].path_state, PATH_STATUS_CREATION);
   }
}

/**
 * Call the cross grouping routine and expect that two given pt_persistent.paths are grouped.
 * \uts{CSCSA-43614} \sdd{SF-7385} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Cross_Paths__cross_grouping_for_two_lateral_paths)
{
   /** \arrange set up two input pt_persistent.paths. */
   uint8_t start_first  = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t end_first    = PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t start_second = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t end_second   = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, start_first, end_first, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[start_first] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[end_first] + 3.0f, 1.0f), 0.0f, 1.0f);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, start_second, end_second, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[start_second] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[end_second] + 1.5f, 1.0f), 0.0f, 1.0f);
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[1].path_state = PATH_STATUS_MATURE;

   /** \action call cross grouping function. */
   Pt_Group_Cross_Paths(&pt_persistent, &pt_persistent.paths[1], &cals, pt_input.grid_pt_array);

   /** \assert expect pt_persistent.paths to be merged. */
   EXPECT_EQ(pt_persistent.paths[0u].path_state, PATH_STATUS_DEFAULT);
   EXPECT_EQ(pt_persistent.paths[1u].path_state, PATH_STATUS_MATURE);
   EXPECT_EQ(pt_persistent.paths[1u].first_p, start_first);
   EXPECT_EQ(pt_persistent.paths[1u].last_p, end_second);
}


/**
 * Call the cross grouping routine and expect that two given pt_persistent.paths are given but they have a cross and are merged.
 * \uts{CSCSA-43615} \sdd{SF-7385} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Cross_Paths__paths_have_a_cross_and_are_merged)
{
   /** \arrange set up two input pt_persistent.paths. */
   uint8_t start_first  = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t end_first    = PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t start_second = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t end_second   = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, start_first, end_first, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[start_first] - 3.0f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[end_first] + 3.0f, 1.0f), 0.0f, 1.0f);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, start_second, end_second, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[start_second] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[end_second] + 1.5f, 1.0f), 0.0f, 1.0f);
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[1].path_state = PATH_STATUS_MATURE;

   for (uint8_t idx = end_first; idx <= start_second; idx++)
   {
      pt_persistent.paths[0].path_points[idx] = 1.0f;
      pt_persistent.paths[1].path_points[idx] = 1.0f;
   }

   /** \action call cross grouping function. */
   Pt_Group_Cross_Paths(&pt_persistent, &pt_persistent.paths[1], &cals, pt_input.grid_pt_array);

   /** \assert expect pt_persistent.paths to be merged. */
   EXPECT_EQ(pt_persistent.paths[0u].path_state, PATH_STATUS_DEFAULT);
   EXPECT_EQ(pt_persistent.paths[1u].path_state, PATH_STATUS_MATURE);
   EXPECT_EQ(pt_persistent.paths[1u].first_p, start_first);
   EXPECT_EQ(pt_persistent.paths[1u].last_p, end_second);
}


/**
 * Call the main module of grouping routines. Here the path which shall be checked has is in default state and thus invalid for
 * grouping procedure. \uts{CSCSA-43616} \sdd{SF-7401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Paths__input_path_is_in_default_state_and_thus_not_valid)
{
   /** \arrange set up inputs such that no grouping occures. */
   pt_persistent.paths[0].path_state = PATH_STATUS_DEFAULT;
   for (uint8_t idx = 1; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_MATURE;
   }

   /** \action call grouping function. */
   Pt_Group_Paths(&pt_persistent, &pt_persistent.paths[0], &cals, &pt_input);

   /** \assert expect pt_persistent.paths to remain. */
   for (uint8_t idx = 1; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      EXPECT_EQ(pt_persistent.paths[idx].path_state, PATH_STATUS_MATURE);
   }
}


/**
 * Call the main module of grouping routines. Here the path to check is implausible and shall thus be reset.
 * \uts{CSCSA-43617} \sdd{SF-7401} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Group_Paths__reset_implausible_path)
{
   /** \arrange set up input path such that it is reset afterwards. */
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[0].first_p    = 8u;
   pt_persistent.paths[0].last_p     = 8u;
   pt_persistent.paths[0].direction  = PATH_DIRECTION_LONG_BACKWARD;

   /** \action call grouping function. */
   Pt_Group_Paths(&pt_persistent, &pt_persistent.paths[0], &cals, &pt_input);

   /** \assert expect path to be reset. */
   EXPECT_EQ(pt_persistent.paths[0].path_state, PATH_STATUS_DEFAULT);
}

/**
 * Call the processing of cross path function. Here two pt_persistent.paths are given as an input which are not overlapping and are
 * merged to a cross path. \uts{CSCSA-43618} \sdd{SF-7393} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Process_Cross_Path__calculate_cross_path)
{
   /** \arrange set up two pt_persistent.paths which are not overlapping . */
   Pt_Init_Path_Linearly(
      &pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD,
      PT_MID_GRID_POINT_INDEX - 7 * PT_SINGLE_GRID_POINT_OFFSET, PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET, 0u,
      Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX - 7 * PT_SINGLE_GRID_POINT_OFFSET] - 3.0f, 1.0f),
      Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET] + 3.0f, 1.0f),
      0.0f, -1.0f);
   Pt_Init_Path_Linearly(
      &pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, PT_MID_GRID_POINT_INDEX,
      PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET, 0u,
      Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] - 1.5f, 1.0f),
      Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET] + 1.5f, 1.0f),
      0.0f, -0.5f);


   /** \action call processing of cross path function. */
   Pt_Process_Cross_Path(&pt_persistent.paths[1], &pt_persistent.paths[0], best_path_obj_pairs, &cals, pt_input.grid_pt_array);

   /** \assert expect pt_persistent.paths to contain the value of path0 in the first segment, then interpolated values and then the
    * value of the second initialized path segment. */
   for (uint8_t idx = PT_MID_GRID_POINT_INDEX - 7 * PT_SINGLE_GRID_POINT_OFFSET;
        idx <= PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET; idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[0].path_points[idx], -1.0f);
   }

   EXPECT_FLOAT_EQ(pt_persistent.paths[0].path_points[PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET], -5.0f / 6.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0].path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET], -2.0f / 3.0f);

   for (uint8_t idx = PT_MID_GRID_POINT_INDEX; idx <= PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET; idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[0].path_points[idx], -0.5f);
   }
}

/**
 * Check whether the path pair is valid for group overlapping.
 * \uts{CSCSA-43619} \sdd{SF-7390} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Group_Overlapping__path_pair_is_invalid_host_trail)
{
   /** \arrange set up a path pair which is valid for the group overlapping. */
   boolean_T ret;
   pt_persistent.paths[0].direction  = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[0].path_state = PATH_STATUS_HOST_TRAIL;
   pt_persistent.paths[1].direction  = pt_persistent.paths[0].direction;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index + 1u;
   pt_persistent.paths[1].path_state = pt_persistent.paths[0].path_state;


   /** \action call validity check of group overlapping. */
   ret = Pt_Is_Path_Pair_Valid_For_Group_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect path pair to be valid. */
   EXPECT_FALSE(ret);
}

/**
 * Check whether the path pair is valid for group overlapping.
 * \uts{CSCSA-43620} \sdd{SF-7390} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Is_Path_Pair_Valid_For_Group_Overlapping__same_index)
{
   /** \arrange set up a path pair which is valid for the group overlapping. */
   boolean_T ret;
   pt_persistent.paths[0].direction  = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].path_index = 0u;
   pt_persistent.paths[0].path_state = PATH_STATUS_HOST_TRAIL;
   pt_persistent.paths[1].direction  = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[1].path_index = pt_persistent.paths[0].path_index + 1;
   pt_persistent.paths[1].path_state = pt_persistent.paths[0].path_state;


   /** \action call validity check of group overlapping. */
   ret = Pt_Is_Path_Pair_Valid_For_Group_Overlapping(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect path pair to be valid. */
   EXPECT_FALSE(ret);
}

/**
 * Test whether the less established path is reset correctly. Here both pt_persistent.paths are equally established, thus both
 * pt_persistent.paths shall be kept. \uts{CSCSA-43621} \sdd{SF-7395} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Reset_Less_Established_Path__high_diff_number_points)
{
   /** \arrange set up conditions such that the both pt_persistent.paths are kept. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx], 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx], 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[0].num_groupings = 5u;
   pt_persistent.paths[0].path_state    = PATH_STATUS_MATURE;

   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_idx,
                         upper_idx + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_diff_num_path_points + 1u, 0u,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 1.5f, 1.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 1.5f, 1.0f), 0.0f, -1.0f);
   pt_persistent.paths[1].num_groupings = pt_persistent.paths[0].num_groupings;
   pt_persistent.paths[1].path_state    = pt_persistent.paths[0].path_state;
   pt_persistent.paths[1].first_p       = PT_DEFAULT_DISCR_BORDER;
   pt_persistent.paths[1].last_p        = PT_DEFAULT_DISCR_BORDER;

   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_diff_num_path_points = 1u;

   /** \action call function for reset of less established pt_persistent.paths. */
   Pt_Reset_Less_Established_Path(&pt_persistent.paths[0], &pt_persistent.paths[1], best_path_obj_pairs,
                                  &pt_input.Num_Grid_Pts_Dep_Cals);


   /** \assert expect both pt_persistent.paths to be kept. */
   EXPECT_EQ(PATH_STATUS_MATURE, pt_persistent.paths[0].path_state);
   EXPECT_EQ(PATH_STATUS_MATURE, pt_persistent.paths[1].path_state);
}

/**
 * Test the merging of path borders for laterally oriented pt_persistent.paths. Here the lateral component shall be overwritten.
 * \uts{CSCSA-43622} \sdd{SF-7392} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Merge_Path_Borders_With_Extrapol__lateral_overwrite_boundaries_reversed_kill_extrapolate)
{
   /** \arrange Setup two pt_persistent.paths which shall be merged. the path at 0 shall be the path to reset and the path 1 shall
    * be the slot of merging. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LAT_LEFT, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[lower_idx] - 3.0f),
                         Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[upper_idx] + 3.0f), 0.0f, -1.0f);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LAT_LEFT, lower_idx, upper_idx, 0u,
                         Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[lower_idx] - 1.5f),
                         Create_2d_Vector_Coordinates(1.0f, pt_input.grid_pt_array[upper_idx] + 1.5f), 0.0f, -1.0f);


   /** \action call function for path border merging. */
   Pt_Merge_Path_Borders_With_Extrapol(&pt_persistent.paths[0], &pt_persistent.paths[1], pt_input.grid_pt_array);

   /** \assert expect borders to be set to the outermost border of both input pt_persistent.paths. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].first.y, pt_input.grid_pt_array[lower_idx] - 1.5f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].last_mat.y, pt_input.grid_pt_array[upper_idx] + 1.5f);
}

/**
 * Test the merging of path borders for longitudinally oriented pt_persistent.paths. Here the longitudinal component shall be
 * overwritten. No interpoation is applied here thus only the vectors are set up. \uts{CSCSA-43623} \sdd{SF-7391}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Group_Paths_Test, Pt_Merge_Path_Borders__longitudinal_overwrite_boundaries_reversed)
{
   /** \arrange Setup two pt_persistent.paths which shall be merged. the path at 0 shall be the path to reset and the path 1 shall
    * be the slot of merging. */
   uint8_t lower_idx = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t upper_idx = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;

   pt_persistent.paths[0].direction = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[0].first     = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 3.0f, 1.0f);
   pt_persistent.paths[0].last_mat  = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 3.0f, 1.0f);
   pt_persistent.paths[1].direction = PATH_DIRECTION_LONG_BACKWARD;
   pt_persistent.paths[1].first     = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_idx] - 1.5f, 1.0f);
   pt_persistent.paths[1].last_mat  = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_idx] + 1.5f, 1.0f);


   /** \action call function for path border merging without extrapolation. */
   Pt_Merge_Path_Borders(&pt_persistent.paths[0], &pt_persistent.paths[1]);

   /** \assert expect borders to be set to the outermost border of both input pt_persistent.paths. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].first.x, pt_input.grid_pt_array[lower_idx] - 1.5f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1].last_mat.x, pt_input.grid_pt_array[upper_idx] + 1.5f);
}