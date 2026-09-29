/**
 * @file pt_modify_grid_cals_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_modify_grid_cals.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43693}
 */

#include "pt_modify_grid_cals_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_input_t.h"
#include "pt_modify_grid_cals.c"
}

/**
 * Test the depending cals functionality by filling them with non-default values, calling the function, and verifying that the
 * calls contain the default cal values. \uts{CSCSA-43695} \sdd{SF-7437} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Modify_Grid_Cals_Test, Pt_Set_Num_Grid_Pts_Dependend_Cals__default_case)
{
   /** \arrange Fill the local call structure with non-zero values. */
   float32_T grid_array[PT_NUM_GRID_POINTS]{};
   Pt_Num_Grid_Pts_Dep_Cals_T num_grid_points_dep_cals{};

   pt_cals.k_pt_default_range_of_tracking_zone = 0;

   /** \action Call setter function of grid dependent calibrations. */
   Pt_Set_Num_Grid_Pts_Dependend_Cals(&pt_cals, grid_array, &num_grid_points_dep_cals);

   /** \assert Check if depending cal values are set to cal values. */
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_find_max_diff_path_points, pt_cals.k_pt_find_max_diff_path_points);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_find_min_diff_path_points, pt_cals.k_pt_find_min_diff_path_points);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_kill_lane_change_min_diff_path_point,
                   pt_cals.k_pt_kill_lane_change_min_diff_path_point);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_max_diff_num_path_point, pt_cals.k_pt_max_diff_num_path_point);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_min_path_length_after_rot, pt_cals.k_pt_min_path_length_after_rot);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_min_path_length_proc_lane_change, pt_cals.k_pt_min_path_length_proc_lane_change);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_group_path_min_overlap_count, pt_cals.k_pt_group_path_min_overlap_count);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_group_path_min_overlap_count_ad, pt_cals.k_pt_group_path_min_overlap_count_ad);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_min_diff_num_path_points, pt_cals.k_pt_min_diff_num_path_points);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_find_max_long_posn, pt_cals.k_pt_find_max_long_posn);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_find_max_lat_posn, pt_cals.k_pt_find_max_lat_posn);
}


/**
 * Test the depending cals functionality by filling them with non-default values, calling the function, and verifying that the
 * calls contain the default cal values. In this special case, the cals are modified depending on the grid array. \uts{CSCSA-43696}
 * \sdd{SF-7437} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Modify_Grid_Cals_Test, Pt_Set_Num_Grid_Pts_Dependend_Cals__grid_points_modified_cals)
{
   /** \arrange Fill the local call structure with non-zero values. Choose required grid values such that the resulting cals look
    * nice. */
   float32_T grid_array[PT_NUM_GRID_POINTS]{};
   Pt_Num_Grid_Pts_Dep_Cals_T num_grid_points_dep_cals{};

   grid_array[PT_LOWEST_GRID_POINT_INDEX]                               = 0;
   pt_cals.k_pt_default_range_of_tracking_zone                          = 1;
   grid_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = 2;
   grid_array[PT_HIGHEST_GRID_POINT_INDEX]                              = 3;

   pt_cals.k_pt_find_max_diff_path_points            = 0;
   pt_cals.k_pt_find_min_diff_path_points            = 0;
   pt_cals.k_pt_kill_lane_change_min_diff_path_point = 0;
   pt_cals.k_pt_max_diff_num_path_point              = 0;
   pt_cals.k_pt_min_path_length_after_rot            = 0;
   pt_cals.k_pt_min_path_length_proc_lane_change     = 0;
   pt_cals.k_pt_group_path_min_overlap_count         = 0;
   pt_cals.k_pt_group_path_min_overlap_count_ad      = 0;
   pt_cals.k_pt_min_diff_num_path_points             = 0;

   /** \action Call Pt_Set_Num_Grid_Pts_Dependend_Cals */
   Pt_Set_Num_Grid_Pts_Dependend_Cals(&pt_cals, grid_array, &num_grid_points_dep_cals);

   /** \assert Check if depending cal values are set to cal values. */
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_find_max_diff_path_points, 1);
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_find_min_diff_path_points, 1);
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_kill_lane_change_min_diff_path_point, 1);
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_max_diff_num_path_point, 1);
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_min_path_length_after_rot, 1);
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_min_path_length_proc_lane_change, 1);
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_group_path_min_overlap_count, 1);
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_group_path_min_overlap_count_ad, 1);
   EXPECT_EQ(num_grid_points_dep_cals.k_pt_min_diff_num_path_points, 1);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_find_max_long_posn, 3.0f);
   EXPECT_FLOAT_EQ(num_grid_points_dep_cals.k_pt_find_max_lat_posn, 3.0f);
}
