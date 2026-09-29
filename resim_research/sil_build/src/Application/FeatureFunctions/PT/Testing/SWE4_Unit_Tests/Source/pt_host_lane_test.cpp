/**
 * @file pt_host_lane_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_host_lane.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43624}
 */

#include "pt_host_lane_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>


extern "C"
{
#include "fbk_core_calibration_t.h"
#include "fbk_host_trail.c"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "pt_host_lane.c"
#include "pt_output_t.h"
}


/**
 * Tests whether the constant path part of a cta scenario can be extracted.
 * \uts{CSCSA-43625} \sdd{SF-7631} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Search_Trail_Interval_For_Path_Gen__cta_scenario)
{
   /** \arrange Set up a trail with different heading segments. */
   Fbk_Host_Lane_Interval_T interval;

   host_trail.segments[0].heading  = 0.5f * PI;
   host_trail.segments[1].heading  = 0.5f * PI;
   host_trail.segments[2].heading  = 0.5f * PI;
   host_trail.segments[3].heading  = 0.5f * PI;
   host_trail.segments[4].heading  = 0.5f * PI;
   host_trail.segments[5].heading  = 0.5f * PI;
   host_trail.segments[6].heading  = 0.5f * PI;
   host_trail.segments[7].heading  = 0.5f * PI;
   host_trail.segments[8].heading  = 0.35f * PI;
   host_trail.segments[9].heading  = 0.25f * PI;
   host_trail.segments[10].heading = 0.15f * PI;
   host_trail.segments[11].heading = 0.05f * PI;
   host_trail.segments[12].heading = 0.0f;
   host_trail.segments[13].heading = 0.0f;
   host_trail.segments[14].heading = 0.0f;
   host_trail.segments[15].heading = 0.0f;
   host_trail.segments[16].heading = 0.0f;
   host_trail.segments[17].heading = 0.0f;
   host_trail.segments[18].heading = 0.0f;
   host_trail.segments[19].heading = 0.0f;

   host_trail.oldest_trail_index  = 0;
   host_trail.trail_index         = 13;
   host_trail.f_trail_full_buffer = FBK_FALSE;

   /** \action call interval searching routine. */
   Pt_Search_Trail_Interval_For_Path_Gen(&interval, &cals, &host_trail);

   /** \assert Check that the correct interval is chosen. */
   EXPECT_EQ(interval.interval_start, 7);
   EXPECT_EQ(interval.interval_end, 0);
   EXPECT_EQ(interval.interval_range, 8);
}


/**
 * Tests whether the constant path part of a cta scenario can be extracted. Here the way how the trajectory is stored, differs from
 * other unit tests. \uts{CSCSA-43626} \sdd{SF-7631} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test,
       Pt_Search_Trail_Interval_For_Path_Gen__cta_scenario_which_has_been_stored_with_overflow_in_buffer_hit_overflow_branches)
{
   /** \arrange Set up a trail with different heading segments. */
   cals.k_pt_max_heading_diff_valid_interval = 0.05f;
   cals.k_pt_minimum_amount_of_trail_points  = 4u;
   Fbk_Host_Lane_Interval_T interval;

   host_trail.segments[0].heading  = 0.64f * PI;
   host_trail.segments[1].heading  = 0.58f * PI;
   host_trail.segments[2].heading  = 0.52f * PI;
   host_trail.segments[3].heading  = 0.46f * PI;
   host_trail.segments[4].heading  = 0.40f;
   host_trail.segments[5].heading  = 0.34f;
   host_trail.segments[6].heading  = 0.28f;
   host_trail.segments[7].heading  = 0.22f;
   host_trail.segments[8].heading  = 0.15f;
   host_trail.segments[9].heading  = 0.1f;
   host_trail.segments[10].heading = 0.0f;
   host_trail.segments[11].heading = 0.0f;
   host_trail.segments[12].heading = 0.7f * PI;
   host_trail.segments[13].heading = 0.7f * PI;
   host_trail.segments[14].heading = 0.7f * PI;
   host_trail.segments[15].heading = 0.7f * PI;
   host_trail.segments[16].heading = 0.7f * PI;
   host_trail.segments[17].heading = 0.7f * PI;
   host_trail.segments[18].heading = 0.7f * PI;
   host_trail.segments[19].heading = 0.7f * PI;

   host_trail.oldest_trail_index  = 12;
   host_trail.trail_index         = 12;
   host_trail.f_trail_full_buffer = FBK_TRUE;

   /** \action call interval searching routine. */
   Pt_Search_Trail_Interval_For_Path_Gen(&interval, &cals, &host_trail);

   /** \assert Check that the correct interval is chosen. */
   EXPECT_EQ(interval.interval_start, 19);
   EXPECT_EQ(interval.interval_end, 12);
   EXPECT_EQ(interval.interval_range, 8);
}


/**
 * Tests whether the constant path part of a cta scenario can be extracted. Trail index is set to 0 and thus it is expected to
 * prevent the underflow for newest idx. \uts{CSCSA-43627} \sdd{SF-7631} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Search_Trail_Interval_For_Path_Gen__hit_branch_for_prevention_of_underflow_of_newest_idx)
{
   /** \arrange Set up a trail with different heading segments. */
   cals.k_pt_max_heading_diff_valid_interval = 0.05f;
   cals.k_pt_minimum_amount_of_trail_points  = 5u;
   Fbk_Host_Lane_Interval_T interval;

   host_trail.segments[0].heading  = 0.7f * PI;
   host_trail.segments[1].heading  = 0.7f * PI;
   host_trail.segments[2].heading  = 0.7f * PI;
   host_trail.segments[3].heading  = 0.7f * PI;
   host_trail.segments[4].heading  = 0.7f * PI;
   host_trail.segments[5].heading  = 0.7f * PI;
   host_trail.segments[6].heading  = 0.58f * PI;
   host_trail.segments[7].heading  = 0.52f * PI;
   host_trail.segments[8].heading  = 0.46f * PI;
   host_trail.segments[9].heading  = 0.40f;
   host_trail.segments[10].heading = 0.34f;
   host_trail.segments[11].heading = 0.28f;
   host_trail.segments[12].heading = 0.22f;
   host_trail.segments[13].heading = 0.16f;
   host_trail.segments[14].heading = 0.1f;
   host_trail.segments[15].heading = 0.4f;
   host_trail.segments[16].heading = 0.0f;
   host_trail.segments[17].heading = 0.0f;
   host_trail.segments[18].heading = 0.0f;
   host_trail.segments[19].heading = 0.0f;

   host_trail.oldest_trail_index  = 0u;
   host_trail.trail_index         = 0u;
   host_trail.f_trail_full_buffer = FBK_TRUE;

   /** \action call interval searching routine. */
   Pt_Search_Trail_Interval_For_Path_Gen(&interval, &cals, &host_trail);

   /** \assert Check that the correct interval is chosen. */
   EXPECT_EQ(interval.interval_start, 5);
   EXPECT_EQ(interval.interval_end, 0);
   EXPECT_EQ(interval.interval_range, 6);
}


/**
 * Tests the routine for transformation between trail and pt_persistent.paths. Here the maximum amount of interval shall be
 * transformed. \uts{CSCSA-43628} \sdd{SF-7622} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path_Points__get_point_via_interpolation_lateral_path)
{
   /** \arrange Set up a vcs trail shifted to the grid. */
   cals.k_pt_max_heading_diff_valid_interval = 0.05f;
   cals.k_pt_minimum_amount_of_trail_points  = 4u;
   Fbk_Host_Lane_Interval_T interval;
   interval.interval_start = 19u;
   interval.interval_end   = 0u;
   interval.interval_range = 20u;

   float32_T step_width = path_tracking_input.grid_pt_array[1] - path_tracking_input.grid_pt_array[0];

   for (uint8_t idx = 0; idx < FBK_NUM_HOST_TRAIL_POINTS; idx++)
   {
      trail_vcs[idx] =
         Create_2d_Vector_Coordinates(1.0f, path_tracking_input.grid_pt_array[0] + (float32_T(idx) * step_width + 1.0f));
   }
   pt_persistent.paths[1u].direction = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[1u].first_p   = 255u;
   pt_persistent.paths[1u].last_p    = 0u;
   pt_persistent.paths[1u].direction = PATH_DIRECTION_LAT_RIGHT;
   /** \action call interval searching routine. */
   Pt_Transform_Trail_To_Path_Points(&pt_persistent.paths[1u], &interval, 2u, trail_vcs, path_tracking_input.grid_pt_array);

   /** \assert Check that path point is correctly set. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[2u], 1.0f);
}


/**
 * Tests the routine for transformation between trail and pt_persistent.paths. Here the maximum amount of interval shall be
 * transformed. \uts{CSCSA-43629} \sdd{SF-7622} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path_Points__get_point_via_interpolation_longitudinal_path)
{
   /** \arrange Set up a vcs trail shifted to the grid. */
   cals.k_pt_max_heading_diff_valid_interval = 0.05f;
   cals.k_pt_minimum_amount_of_trail_points  = 4u;
   Fbk_Host_Lane_Interval_T interval;
   interval.interval_start = 19u;
   interval.interval_end   = 0u;
   interval.interval_range = 20u;

   float32_T step_width = path_tracking_input.grid_pt_array[1] - path_tracking_input.grid_pt_array[0];

   for (uint8_t idx = 0; idx < FBK_NUM_HOST_TRAIL_POINTS; idx++)
   {
      trail_vcs[idx] = Create_2d_Vector_Coordinates(path_tracking_input.grid_pt_array[0] + (float32_T(idx) * step_width + 1.0f), 1.0f);
   }
   pt_persistent.paths[1u].direction = PATH_DIRECTION_LONG_FORWARD;
   pt_persistent.paths[1u].first_p   = 255u;
   pt_persistent.paths[1u].last_p    = 0u;
   /** \action call interval searching routine. */
   Pt_Transform_Trail_To_Path_Points(&pt_persistent.paths[1u], &interval, 2u, trail_vcs, path_tracking_input.grid_pt_array);

   /** \assert Check that path point is correctly set. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[2u], 1.0f);
}


/**
 * Tests the routine for transformation between trail and pt_persistent.paths. Here the maximum amount of interval shall be
 * transformed. \uts{CSCSA-43630} \sdd{SF-7622} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path_Points__set_point_directly_via_next_grid_component)
{
   /** \arrange Set up a vcs trail directly on the grid */
   cals.k_pt_max_heading_diff_valid_interval = 0.05f;
   cals.k_pt_minimum_amount_of_trail_points  = 4u;
   Fbk_Host_Lane_Interval_T interval;
   interval.interval_start = 19u;
   interval.interval_end   = 0u;
   interval.interval_range = 20u;

   float32_T step_width = path_tracking_input.grid_pt_array[1] - path_tracking_input.grid_pt_array[0];

   for (uint8_t idx = 0; idx < FBK_NUM_HOST_TRAIL_POINTS; idx++)
   {
      trail_vcs[idx] = Create_2d_Vector_Coordinates(1.0f, path_tracking_input.grid_pt_array[0] + (float32_T(idx) * step_width));
   }
   pt_persistent.paths[1u].direction = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[1u].first_p   = 255u;
   pt_persistent.paths[1u].last_p    = 0u;

   /** \action call transformation routine. */
   Pt_Transform_Trail_To_Path_Points(&pt_persistent.paths[1u], &interval, 2u, trail_vcs, path_tracking_input.grid_pt_array);

   /** \assert Check that path point is correctly set. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[2u], 1.0f);
}


/**
 * Tests the routine for transformation between trail and pt_persistent.paths. Here the maximum amount of interval shall be
 * transformed. \uts{CSCSA-43657} \sdd{SF-7622} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path_Points__set_point_directly_via_passed_grid_component)
{
   /** \arrange Set up a vcs trail directly on the grid */
   cals.k_pt_max_heading_diff_valid_interval = 0.05f;
   cals.k_pt_minimum_amount_of_trail_points  = 4u;
   Fbk_Host_Lane_Interval_T interval;
   interval.interval_start = 19u;
   interval.interval_end   = 1u;
   interval.interval_range = 20u;

   float32_T step_width = path_tracking_input.grid_pt_array[1] - path_tracking_input.grid_pt_array[0];

   for (uint8_t idx = 0; idx < FBK_NUM_HOST_TRAIL_POINTS; idx++)
   {
      trail_vcs[idx] = Create_2d_Vector_Coordinates(1.0f, path_tracking_input.grid_pt_array[0] + (float32_T(idx) * step_width));
   }
   pt_persistent.paths[1u].direction = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[1u].first_p   = 255u;
   pt_persistent.paths[1u].last_p    = 0u;
   /** \action call transformation routine. */
   Pt_Transform_Trail_To_Path_Points(&pt_persistent.paths[1u], &interval, 1u, trail_vcs, path_tracking_input.grid_pt_array);

   /** \assert Check that path point is correctly set. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[1u], 1.0f);
}


/**
 * Test whether a path point can be generated. Here the grid array entry is inbetween the interval and thus true is expected.
 * \uts{CSCSA-43659} \sdd{SF-7621} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Can_Path_Point_Be_Generated__first_branch_hit)
{
   /** \arrange Set up a specific interval. */
   float32_T passed_grid_comp = -60.0f;
   float32_T next_grid_comp   = -55.0f;
   float32_T grid_array_entry = -57.5f;
   boolean_T res;

   /** \action call point indices. */
   res = Pt_Can_Path_Point_Be_Generated(&passed_grid_comp, &next_grid_comp, &grid_array_entry);

   /** \assert Expect that true is returned. */
   EXPECT_TRUE(res);
}


/**
 * Test whether a path point can be generated. Here the grid array entry is inbetween the interval and thus true is expected.
 * \uts{CSCSA-43660} \sdd{SF-7621} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Can_Path_Point_Be_Generated__second_branch_hit)
{
   /** \arrange Set up a specific interval. */
   float32_T passed_grid_comp = 60.0f;
   float32_T next_grid_comp   = 55.0f;
   float32_T grid_array_entry = 57.5f;
   boolean_T res;

   /** \action call point indices. */
   res = Pt_Can_Path_Point_Be_Generated(&passed_grid_comp, &next_grid_comp, &grid_array_entry);

   /** \assert Expect that true is returned. */
   EXPECT_TRUE(res);
}


/**
 * Test whether a path point can be generated. Here the grid array component is not in the interval.
 * \uts{CSCSA-43661} \sdd{SF-7621} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Can_Path_Point_Be_Generated__entry_outside_of_interval)
{
   /** \arrange Set up a specific interval. */
   float32_T passed_grid_comp = 60.0f;
   float32_T next_grid_comp   = 55.0f;
   float32_T grid_array_entry = 65.0f;
   boolean_T res;

   /** \action call point indices. */
   res = Pt_Can_Path_Point_Be_Generated(&passed_grid_comp, &next_grid_comp, &grid_array_entry);

   /** \assert Expect that false is returned. */
   EXPECT_FALSE(res);
}


/**
 * Tests the direction calculation. Here a lateral right path is expected.
 * \uts{CSCSA-43662} \sdd{SF-7623} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Get_Trail_Based_Path_Dir__direction_is_lateral_right)
{
   /** \arrange Set up a longidutinal trail. */
   cals.k_pt_lower_lim_obj_orient_lat = 0.7f;
   cals.k_pt_upper_lim_obj_orient_lat = 2.44f;

   host_trail.oldest_trail_index = 1u;

   trail_vcs[host_trail.oldest_trail_index]      = Create_2d_Vector_Coordinates(0.0f, -1.0f);
   trail_vcs[host_trail.oldest_trail_index + 1u] = Create_2d_Vector_Coordinates(0.0f, 1.0f);

   /** \action call trail direction calculation method. */
   Pt_Get_Trail_Based_Path_Dir(&pt_persistent.paths[1u], trail_vcs, &cals, &host_trail);

   /** \assert Expect direction is correctly set. */
   EXPECT_EQ(pt_persistent.paths[1u].direction, PATH_DIRECTION_LAT_RIGHT);
}


/**
 * Tests the direction calculation. Here a longitudinal forward path is expected
 * \uts{CSCSA-43663} \sdd{SF-7623} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Get_Trail_Based_Path_Dir__direction_is_longidutinal_forward)
{
   /** \arrange Set up a longidutinal trail. */
   cals.k_pt_lower_lim_obj_orient_lat = 0.7f;
   cals.k_pt_upper_lim_obj_orient_lat = 2.44f;

   host_trail.oldest_trail_index = 1u;

   trail_vcs[host_trail.oldest_trail_index]      = Create_2d_Vector_Coordinates(-1.0f, 0.0f);
   trail_vcs[host_trail.oldest_trail_index + 1u] = Create_2d_Vector_Coordinates(1.0f, 0.0f);

   /** \action call trail direction calculation method. */
   Pt_Get_Trail_Based_Path_Dir(&pt_persistent.paths[1u], trail_vcs, &cals, &host_trail);

   /** \assert Expect direction is correctly set. */
   EXPECT_EQ(pt_persistent.paths[1u].direction, PATH_DIRECTION_LONG_FORWARD);
}


/**
 * Tests the direction calculation. Here a longitudinal forward path is expected
 * \uts{CSCSA-43664} \sdd{SF-7623} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Get_Trail_Based_Path_Dir__direction_is_longidutinal_forward_with_overflow_save_decoding_of_points)
{
   /** \arrange Set up a lateral trail. */
   cals.k_pt_lower_lim_obj_orient_lat = 0.7f;
   cals.k_pt_upper_lim_obj_orient_lat = 2.44f;

   host_trail.oldest_trail_index = 19u;

   trail_vcs[host_trail.oldest_trail_index] = Create_2d_Vector_Coordinates(-1.0f, 0.0f);
   trail_vcs[0u]                            = Create_2d_Vector_Coordinates(1.0f, 0.0f);

   /** \action call trail direction calculation method. */
   Pt_Get_Trail_Based_Path_Dir(&pt_persistent.paths[1u], trail_vcs, &cals, &host_trail);

   /** \assert Expect direction is correctly set. */
   EXPECT_EQ(pt_persistent.paths[1u].direction, PATH_DIRECTION_LONG_FORWARD);
}


/**
 * Test the data management of host trail module. Here no pt_persistent.paths exist yet and thus an empty path slot is returned.
 * \uts{CSCSA-43665} \sdd{SF-7632} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Get_Host_Trail_Path_Index__no_paths_exists_yet)
{
   /** \arrange Set up index and result variable. */
   uint8_t path_index;
   boolean_T res;

   /** \action call getter function. */
   res = Pt_Get_Host_Trail_Path_Index(pt_persistent.paths, &path_index);

   /** \assert Expect that an index is found. */
   EXPECT_TRUE(res);
   EXPECT_EQ(path_index, 0u);
}


/**
 * Test the data management of host trail module. Here a previously created host trail shall be overriden.
 * \uts{CSCSA-43666} \sdd{SF-7632} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Get_Host_Trail_Path_Index__a_host_trail_is_overriden)
{
   /** \arrange Set up a host trail. */
   uint8_t path_index;
   boolean_T res;
   pt_persistent.paths[5u].path_state = PATH_STATUS_HOST_TRAIL;

   /** \action call getter function. */
   res = Pt_Get_Host_Trail_Path_Index(pt_persistent.paths, &path_index);

   /** \assert Expect that host trail is overriden. */
   EXPECT_TRUE(res);
   EXPECT_EQ(path_index, 5u);
}


/**
 * Test the data management of host trail module. Here all pt_persistent.paths are full and thus a host trail can not be created.
 * \uts{CSCSA-43667} \sdd{SF-7632} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Get_Host_Trail_Path_Index__no_trail_can_be_created)
{
   /** \arrange Set up all possible pt_persistent.paths. */
   uint8_t path_index = 255u;
   boolean_T res;

   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_MATURE;
   }

   /** \action call getter function. */
   res = Pt_Get_Host_Trail_Path_Index(pt_persistent.paths, &path_index);

   /** \assert Expect that no trail can be created. */
   EXPECT_FALSE(res);
}


/**
 * Tests the complete transformation between trail and path information. Here a lateral trail is given and thus a lateral direction
 * is expected. \uts{CSCSA-43668} \sdd{SF-7629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path__transform_interval_to_lateral_path)
{
   /** \arrange Set up all possible pt_persistent.paths. */
   int8_t idx;
   float32_T step;
   host_trail.trail_host_position     = Create_2d_Vector_Coordinates(0.0f, 37.0f);
   host_trail.trail_host_heading      = Create_Angle(0.0f);
   p_vehicle_data->rear_axle_position = 0.0f;
   host_trail.oldest_trail_index      = 0u;
   host_trail.trail_index             = 8u;
   host_trail.trail_host_dist         = 35.0f;
   cals.k_pt_enable_host_trail        = FBK_TRUE;

   step                                                       = 5.0;
   path_tracking_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] = 0.0f;
   for (idx = (int8_t) (PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET); idx < (int8_t) PT_NUM_GRID_POINTS;
        idx += (int8_t) PT_SINGLE_GRID_POINT_OFFSET)
   {
      path_tracking_input.grid_pt_array[idx] = path_tracking_input.grid_pt_array[idx - 1] + step;
   }
   for (idx = (int8_t) (PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET); idx >= (int8_t) PT_LOWEST_GRID_POINT_INDEX;
        idx -= (int8_t) PT_SINGLE_GRID_POINT_OFFSET)
   {
      path_tracking_input.grid_pt_array[idx] = path_tracking_input.grid_pt_array[idx + 1] - step;
   }

   host_trail.segments[0].point   = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   host_trail.segments[0].heading = 0.5f * PI;
   host_trail.segments[1].point   = Create_2d_Vector_Coordinates(1.0f, 5.0f);
   host_trail.segments[1].heading = 0.5f * PI;
   host_trail.segments[2].point   = Create_2d_Vector_Coordinates(1.0f, 10.0f);
   host_trail.segments[2].heading = 0.5f * PI;
   host_trail.segments[3].point   = Create_2d_Vector_Coordinates(1.0f, 15.0f);
   host_trail.segments[3].heading = 0.5f * PI;
   host_trail.segments[4].point   = Create_2d_Vector_Coordinates(1.0f, 20.0f);
   host_trail.segments[4].heading = 0.5f * PI;
   host_trail.segments[5].point   = Create_2d_Vector_Coordinates(1.0f, 25.0f);
   host_trail.segments[5].heading = 0.5f * PI;
   host_trail.segments[6].point   = Create_2d_Vector_Coordinates(1.0f, 30.0f);
   host_trail.segments[6].heading = 0.5f * PI;
   host_trail.segments[7].point   = Create_2d_Vector_Coordinates(1.0f, 35.0f);
   host_trail.segments[7].heading = 0.5f * PI;

   host_trail.f_was_trail_point_added_this_cycle = FBK_TRUE;

   /** \action call transformation function. */
   Pt_Transform_Trail_To_Path(&pt_persistent, &cals, &path_tracking_input, p_vehicle_data, &host_trail);

   /** \assert Expect that lateral path is created. */
   uint8_t starting_idx;

   for (starting_idx = 0; starting_idx < PT_NUM_GRID_POINTS; starting_idx++)
   {
      if (path_tracking_input.grid_pt_array[starting_idx] == -35.0f)
      {
         break;
      }
   }

   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 1u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 2u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 3u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 4u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 5u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 6u], 1.0f);
   EXPECT_EQ(pt_persistent.paths[0u].direction, PATH_DIRECTION_LAT_RIGHT);
}

/**
 * Tests the transformation between trail and path information is not completed. Only one point has been mapped
 * \uts{} \sdd{SF-7629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path__not_complete_transform)
{
   /** \arrange Set up all possible parameters. */
   int8_t idx;
   float32_T step                                             = 2.0;
   path_tracking_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] = 0.0f;
   for (idx = (int8_t) (PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET); idx < (int8_t) PT_NUM_GRID_POINTS;
        idx += (int8_t) PT_SINGLE_GRID_POINT_OFFSET)
   {
      path_tracking_input.grid_pt_array[idx] = path_tracking_input.grid_pt_array[idx - 1] + step;
   }
   for (idx = (int8_t) (PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET); idx >= (int8_t) PT_LOWEST_GRID_POINT_INDEX;
        idx -= (int8_t) PT_SINGLE_GRID_POINT_OFFSET)
   {
      path_tracking_input.grid_pt_array[idx] = path_tracking_input.grid_pt_array[idx + 1] - step;
   }

   /*Initialize vehicle_data */
   p_vehicle_data->host_length        = 4.70200014f;
   p_vehicle_data->host_width         = 1.86700010f;
   p_vehicle_data->rear_axle_position = -3.69700027f;
   p_vehicle_data->host_speed         = 0.996169448f;
   p_vehicle_data->steering_angle     = 2.53072762f;
   p_vehicle_data->long_vel           = 0.996169388f;
   p_vehicle_data->long_acc           = 0.0f;
   p_vehicle_data->lat_acc            = 0.0f;
   p_vehicle_data->prndl              = PA_VEH_PRNDL_STATE_DRIVE;
   p_vehicle_data->lane_width         = 3.50000000f;
   p_vehicle_data->lane_center_offset = 0.0f;
   p_vehicle_data->turn_signal        = 0;
   p_vehicle_data->curvature          = 0.0950706229f;
   p_vehicle_data->f_reverse          = 0;
   p_vehicle_data->wheelbase          = 2.70000005f;

   /* Set up trail_host_position, trail_host_heading, etc. */
   host_trail.segments[0].point.x             = 93.3909073f;
   host_trail.segments[0].point.y             = 31.4339085f;
   host_trail.segments[0].distance_traveled   = 103.287506f;
   host_trail.segments[0].dist_between_points = 5.28988743f;
   host_trail.segments[0].heading             = 0.0809927508f;

   host_trail.segments[1].point.x             = 98.4541931f;
   host_trail.segments[1].point.y             = 31.4785576f;
   host_trail.segments[1].distance_traveled   = 108.352753f;
   host_trail.segments[1].dist_between_points = 5.06348276f;
   host_trail.segments[1].heading             = -0.0532134473f;

   host_trail.segments[2].point.x             = 103.470779f;
   host_trail.segments[2].point.y             = 31.0568542f;
   host_trail.segments[2].distance_traveled   = 113.426399f;
   host_trail.segments[2].dist_between_points = 5.03427982f;
   host_trail.segments[2].heading             = -0.0882483795f;

   host_trail.segments[3].point.x             = 15.9857435f;
   host_trail.segments[3].point.y             = 0.143383920f;
   host_trail.segments[3].distance_traveled   = 15.3037233f;
   host_trail.segments[3].dist_between_points = 5.30173397f;
   host_trail.segments[3].heading             = 0.0435516983f;

   host_trail.segments[4].point.x             = 20.7055397f;
   host_trail.segments[4].point.y             = 0.389062881f;
   host_trail.segments[4].distance_traveled   = 20.3047333f;
   host_trail.segments[4].dist_between_points = 4.72618580f;
   host_trail.segments[4].heading             = 0.0582694747f;

   host_trail.segments[5].point.x             = 25.5909691f;
   host_trail.segments[5].point.y             = 0.722803831f;
   host_trail.segments[5].distance_traveled   = 25.3608379f;
   host_trail.segments[5].dist_between_points = 4.89681578f;
   host_trail.segments[5].heading             = 0.0810382888f;

   host_trail.segments[6].point.x             = 30.2869358f;
   host_trail.segments[6].point.y             = 1.17628622f;
   host_trail.segments[6].distance_traveled   = 30.3879528f;
   host_trail.segments[6].dist_between_points = 4.71781206f;
   host_trail.segments[6].heading             = 0.110346802f;

   host_trail.segments[7].point.x             = 35.5875320f;
   host_trail.segments[7].point.y             = 1.82529163f;
   host_trail.segments[7].distance_traveled   = 35.3946037f;
   host_trail.segments[7].dist_between_points = 5.34018040f;
   host_trail.segments[7].heading             = 0.132671192f;

   host_trail.segments[8].point.x             = 40.5888824f;
   host_trail.segments[8].point.y             = 2.52448368f;
   host_trail.segments[8].distance_traveled   = 40.5302773f;
   host_trail.segments[8].dist_between_points = 5.04998779f;
   host_trail.segments[8].heading             = 0.142756701f;

   host_trail.segments[9].point.x             = 45.7339249f;
   host_trail.segments[9].point.y             = 3.26262832f;
   host_trail.segments[9].distance_traveled   = 45.7280083f;
   host_trail.segments[9].dist_between_points = 5.19772243f;
   host_trail.segments[9].heading             = 0.145f;

   host_trail.segments[10].point.x             = 50.6750298f;
   host_trail.segments[10].point.y             = 3.98424792f;
   host_trail.segments[10].distance_traveled   = 50.9147186f;
   host_trail.segments[10].dist_between_points = 4.99352121f;
   host_trail.segments[10].heading             = 0.158896431f;

   host_trail.segments[11].point.x             = 55.5720100f;
   host_trail.segments[11].point.y             = 5.05579805f;
   host_trail.segments[11].distance_traveled   = 55.9258347f;
   host_trail.segments[11].dist_between_points = 5.01284695f;
   host_trail.segments[11].heading             = 0.292134821f;

   host_trail.segments[12].point.x             = 60.3472824f;
   host_trail.segments[12].point.y             = 7.08789635f;
   host_trail.segments[12].distance_traveled   = 61.1293182f;
   host_trail.segments[12].dist_between_points = 5.18966770f;
   host_trail.segments[12].heading             = 0.523935199f;

   host_trail.segments[13].point.x             = 64.5890808f;
   host_trail.segments[13].point.y             = 10.3444033f;
   host_trail.segments[13].distance_traveled   = 66.5026093f;
   host_trail.segments[13].dist_between_points = 5.34768105f;
   host_trail.segments[13].heading             = 0.785195112f;

   host_trail.segments[14].point.x             = 67.9597626f;
   host_trail.segments[14].point.y             = 14.4237995f;
   host_trail.segments[14].distance_traveled   = 71.7857208f;
   host_trail.segments[14].dist_between_points = 5.29178333f;
   host_trail.segments[14].heading             = 0.937100530f;

   host_trail.segments[15].point.x             = 71.1950455f;
   host_trail.segments[15].point.y             = 18.8409309f;
   host_trail.segments[15].distance_traveled   = 77.1057434f;
   host_trail.segments[15].dist_between_points = 5.47522640f;
   host_trail.segments[15].heading             = 0.914994895f;

   host_trail.segments[16].point.x             = 74.6921844f;
   host_trail.segments[16].point.y             = 22.8698654f;
   host_trail.segments[16].distance_traveled   = 82.4019394f;
   host_trail.segments[16].dist_between_points = 5.33500671f;
   host_trail.segments[16].heading             = 0.783889353f;

   host_trail.segments[17].point.x             = 78.7059174f;
   host_trail.segments[17].point.y             = 26.2398663f;
   host_trail.segments[17].distance_traveled   = 87.6213989f;
   host_trail.segments[17].dist_between_points = 5.24089289f;
   host_trail.segments[17].heading             = 0.609947026f;

   host_trail.segments[18].point.x             = 83.1663971f;
   host_trail.segments[18].point.y             = 28.7965584f;
   host_trail.segments[18].distance_traveled   = 92.7234573f;
   host_trail.segments[18].dist_between_points = 5.14126015f;
   host_trail.segments[18].heading             = 0.432073176f;

   host_trail.segments[19].point.x             = 88.1721725f;
   host_trail.segments[19].point.y             = 30.5692024f;
   host_trail.segments[19].distance_traveled   = 97.9846268f;
   host_trail.segments[19].dist_between_points = 5.31037235f;
   host_trail.segments[19].heading             = 0.249949947f;

   host_trail.trail_host_position.x = 106.287086f;
   host_trail.trail_host_position.y = 30.9033718f;

   host_trail.trail_host_heading.angle = 0.0309987552f;
   host_trail.trail_host_heading.sin   = 0.0310581159f;
   host_trail.trail_host_heading.cos   = 0.999517560f;

   host_trail.trail_diff_dist                    = 2.74245262f;
   host_trail.trail_diff_heading                 = 0.119247146f;
   host_trail.trail_host_dist                    = 116.168839f;
   host_trail.trail_index                        = 3;
   host_trail.oldest_trail_index                 = 3;
   host_trail.f_trail_full_buffer                = 1;
   host_trail.f_was_trail_point_added_this_cycle = 0;

   /** \action call transformation function. */
   Pt_Transform_Trail_To_Path(&pt_persistent, &cals, &path_tracking_input, p_vehicle_data, &host_trail);

   /** \assert Expect that no trail can be created. */
   EXPECT_NE(pt_persistent.paths[0].path_state, PATH_STATUS_HOST_TRAIL);
}


/**
 * Tests the complete transformation between trail and path information. Here a longitudinal trail is given and thus a longitudinal
 * direction is expected. \uts{CSCSA-43669} \sdd{SF-7629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path__transform_interval_to_longitudinal_path)
{
   /** \arrange Set up all possible pt_persistent.paths. */
   int8_t idx;
   float32_T step;
   host_trail.trail_host_position     = Create_2d_Vector_Coordinates(37.0f, 0.0f);
   host_trail.trail_host_heading      = Create_Angle(0.0f);
   p_vehicle_data->rear_axle_position = 0.0f;
   host_trail.oldest_trail_index      = 0u;
   host_trail.trail_index             = 8u;
   host_trail.trail_host_dist         = 35.0f;
   cals.k_pt_enable_host_trail        = FBK_TRUE;

   step                                                       = 5.0;
   path_tracking_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] = 0.0f;
   for (idx = (int8_t) (PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET); idx < (int8_t) PT_NUM_GRID_POINTS;
        idx += (int8_t) PT_SINGLE_GRID_POINT_OFFSET)
   {
      path_tracking_input.grid_pt_array[idx] = path_tracking_input.grid_pt_array[idx - 1] + step;
   }
   for (idx = (int8_t) (PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET); idx >= (int8_t) PT_LOWEST_GRID_POINT_INDEX;
        idx -= (int8_t) PT_SINGLE_GRID_POINT_OFFSET)
   {
      path_tracking_input.grid_pt_array[idx] = path_tracking_input.grid_pt_array[idx + 1] - step;
   }

   host_trail.segments[0].point   = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   host_trail.segments[0].heading = 0.0f * PI;
   host_trail.segments[1].point   = Create_2d_Vector_Coordinates(5.0f, 1.0f);
   host_trail.segments[1].heading = 0.0f * PI;
   host_trail.segments[2].point   = Create_2d_Vector_Coordinates(10.0f, 1.0f);
   host_trail.segments[2].heading = 0.0f * PI;
   host_trail.segments[3].point   = Create_2d_Vector_Coordinates(15.0f, 1.0f);
   host_trail.segments[3].heading = 0.0f * PI;
   host_trail.segments[4].point   = Create_2d_Vector_Coordinates(20.0f, 1.0f);
   host_trail.segments[4].heading = 0.0f * PI;
   host_trail.segments[5].point   = Create_2d_Vector_Coordinates(25.0f, 1.0f);
   host_trail.segments[5].heading = 0.0f * PI;
   host_trail.segments[6].point   = Create_2d_Vector_Coordinates(30.0f, 1.0f);
   host_trail.segments[6].heading = 0.0f * PI;
   host_trail.segments[7].point   = Create_2d_Vector_Coordinates(35.0f, 1.0f);
   host_trail.segments[7].heading = 0.0f * PI;

   host_trail.f_was_trail_point_added_this_cycle = FBK_TRUE;

   /** \action call transformation function. */
   Pt_Transform_Trail_To_Path(&pt_persistent, &cals, &path_tracking_input, p_vehicle_data, &host_trail);

   /** \assert Expect that longitudinal path is created. */
   uint8_t starting_idx;
   for (starting_idx = 0; starting_idx < PT_NUM_GRID_POINTS; starting_idx++)
   {
      if (path_tracking_input.grid_pt_array[starting_idx] == -35.0f)
      {
         break;
      }
   }

   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 1u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 2u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 3u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 4u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 5u], 1.0f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].path_points[starting_idx + 6u], 1.0f);
   EXPECT_EQ(pt_persistent.paths[0u].direction, PATH_DIRECTION_LONG_FORWARD);
}


/**
 * Tests the complete transformation between trail and path information. Since no suitable interval is found here, not path shall
 * be created. \uts{CSCSA-43670} \sdd{SF-7629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path__no_suitable_interval_found)
{
   /** \arrange Set up all possible pt_persistent.paths. */
   host_trail.trail_host_position                = Create_2d_Vector_Coordinates(37.0f, 0.0f);
   host_trail.trail_host_heading                 = Create_Angle(0.0f);
   p_vehicle_data->rear_axle_position            = 0.0f;
   host_trail.oldest_trail_index                 = 0u;
   host_trail.trail_index                        = 2u;
   host_trail.f_was_trail_point_added_this_cycle = FBK_TRUE;
   host_trail.trail_host_dist                    = cals.k_pt_minimum_host_trail_length + EPSILON;

   host_trail.segments[0].point   = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   host_trail.segments[0].heading = 0.0f * PI;
   host_trail.segments[1].point   = Create_2d_Vector_Coordinates(5.0f, 1.0f);
   host_trail.segments[1].heading = 0.0f * PI;

   /** \action call transformation function. */
   Pt_Transform_Trail_To_Path(&pt_persistent, &cals, &path_tracking_input, p_vehicle_data, &host_trail);

   /** \assert Expect that no trail can be created. */
   EXPECT_EQ(pt_persistent.paths[1u].direction, PATH_DIRECTION_NONE);
}


/**
 * Tests the complete transformation between trail and path information. Here the rear axle is intentionally too high and thus
 * shifting trail so far, that no path points can be mapped. \uts{CSCSA-122415} \sdd{SF-7629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path__trail_could_not_be_mapped_to_path)
{
   /** \arrange Set up all possible pt_persistent.paths. */
   host_trail.trail_host_position     = Create_2d_Vector_Coordinates(37.0f, 0.0f);
   host_trail.trail_host_heading      = Create_Angle(0.0f);
   p_vehicle_data->rear_axle_position = 1000.0f;
   host_trail.oldest_trail_index      = 0u;
   host_trail.trail_index             = 8u;
   host_trail.trail_host_dist         = 35.0f;
   cals.k_pt_enable_host_trail        = FBK_TRUE;

   host_trail.segments[0].point   = Create_2d_Vector_Coordinates(0.0f, 1.0f);
   host_trail.segments[0].heading = 0.0f * PI;
   host_trail.segments[1].point   = Create_2d_Vector_Coordinates(5.0f, 1.0f);
   host_trail.segments[1].heading = 0.0f * PI;
   host_trail.segments[2].point   = Create_2d_Vector_Coordinates(10.0f, 1.0f);
   host_trail.segments[2].heading = 0.0f * PI;
   host_trail.segments[3].point   = Create_2d_Vector_Coordinates(15.0f, 1.0f);
   host_trail.segments[3].heading = 0.0f * PI;
   host_trail.segments[4].point   = Create_2d_Vector_Coordinates(20.0f, 1.0f);
   host_trail.segments[4].heading = 0.0f * PI;
   host_trail.segments[5].point   = Create_2d_Vector_Coordinates(25.0f, 1.0f);
   host_trail.segments[5].heading = 0.0f * PI;
   host_trail.segments[6].point   = Create_2d_Vector_Coordinates(30.0f, 1.0f);
   host_trail.segments[6].heading = 0.0f * PI;
   host_trail.segments[7].point   = Create_2d_Vector_Coordinates(35.0f, 1.0f);
   host_trail.segments[7].heading = 0.0f * PI;

   host_trail.f_was_trail_point_added_this_cycle = FBK_TRUE;

   /** \action call transformation function. */
   Pt_Transform_Trail_To_Path(&pt_persistent, &cals, &path_tracking_input, p_vehicle_data, &host_trail);

   /** \assert Expect that path is not created. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].first_p, 0u);
   EXPECT_FLOAT_EQ(pt_persistent.paths[0u].last_p, 0u);
   EXPECT_FALSE(pt_persistent.paths[0u].path_state == PATH_STATUS_HOST_TRAIL);
}


/**
 * Tests the complete transformation between trail and path information. Here the calibration flag to enable host trail is
 * disabled, so it is expected no trail path created. \uts{CSCSA-122416} \sdd{SF-7629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Transform_Trail_To_Path__calibration_disable)
{
   /** \arrange Set up all possible pt_persistent.paths. */
   host_trail.trail_host_position     = Create_2d_Vector_Coordinates(37.0f, 0.0f);
   host_trail.trail_host_heading      = Create_Angle(0.0f);
   p_vehicle_data->rear_axle_position = 0.0f;
   host_trail.oldest_trail_index      = 0u;
   host_trail.trail_index             = 8u;
   host_trail.trail_host_dist         = 35.0f;
   cals.k_pt_enable_host_trail        = FBK_FALSE;

   host_trail.f_was_trail_point_added_this_cycle = FBK_TRUE;

   /** \action call transformation function. */
   Pt_Transform_Trail_To_Path(&pt_persistent, &cals, &path_tracking_input, p_vehicle_data, &host_trail);

   /** \assert Expect that path is not created. */
   EXPECT_FALSE(pt_persistent.paths[0u].path_state == PATH_STATUS_HOST_TRAIL);
}


/**
 * Test whether the host trail existence check is returning true in case that a host trail path exists.
 * \uts{CSCSA-43671} \sdd{SF-7651} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Is_Host_Lane_Path_Existent__host_trail_path_exists)
{
   /** \arrange Set a path with host trail status. */
   boolean_T res;
   pt_persistent.paths[0].path_state = PATH_STATUS_HOST_TRAIL;

   /** \action call function to test. */
   res = Pt_Is_Host_Lane_Path_Existent(pt_persistent.paths);

   /** \assert Expect that true is returned due to already existing host trail path. */
   EXPECT_TRUE(res);
}


/**
 * Test whether the host trail existence check is returning false in case that a host trail path exists.
 * \uts{CSCSA-43672} \sdd{SF-7651} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Is_Host_Lane_Path_Existent__host_trail_path_not_existing)
{
   /** \arrange No host trail paths are given. */
   boolean_T res;

   /** \action call function to test. */
   res = Pt_Is_Host_Lane_Path_Existent(pt_persistent.paths);

   /** \assert Expect that false is returned due to missing host trail path. */
   EXPECT_FALSE(res);
}


/**
 * Tests the extrapolation of first and last_mat points of lateral path.
 * \uts{CSCSA-122417} \sdd{CSCSA-122370} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Extrapolate_Non_Grid_Borders_Of_The_Path__extrapolate_lateral_path)
{
   /** \arrange Set up pt_persistent.path */
   pt_persistent.paths[1u].first_p   = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1u].last_p    = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1u].direction = PATH_DIRECTION_LAT_RIGHT;

   pt_persistent.paths[1u].path_points[PT_MID_GRID_POINT_INDEX]                               = 0.0f;
   pt_persistent.paths[1u].path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = 1.0f;
   pt_persistent.paths[1u].path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] = -1.0f;

   /** \action call extrapolation function. */
   Pt_Extrapolate_Non_Grid_Borders_Of_The_Path(&pt_persistent.paths[1u], &path_tracking_input);

   /** \assert Expect proper extrapolation of last_mat and first points of path. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].first.x, -0.5f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].first.y,
                   -0.5f
                      * (path_tracking_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]
                         - path_tracking_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]));
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].last_mat.x, 0.5f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].last_mat.y,
                   0.5f
                      * (path_tracking_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]
                         - path_tracking_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]));
}


/**
 * Tests the extrapolation of first and last_mat points of longitudinal path.
 * \uts{CSCSA-122418} \sdd{CSCSA-122370} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Host_Lane_Test, Pt_Extrapolate_Non_Grid_Borders_Of_The_Path__extrapolate_longitudinal_path)
{
   /** \arrange Set up pt_persistent.path */
   pt_persistent.paths[1u].first_p   = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1u].last_p    = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1u].direction = PATH_DIRECTION_LONG_FORWARD;

   pt_persistent.paths[1u].path_points[PT_MID_GRID_POINT_INDEX]                               = 0.0f;
   pt_persistent.paths[1u].path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = 1.0f;
   pt_persistent.paths[1u].path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] = -1.0f;

   /** \action call extrapolation function. */
   Pt_Extrapolate_Non_Grid_Borders_Of_The_Path(&pt_persistent.paths[1u], &path_tracking_input);

   /** \assert Expect proper extrapolation of last_mat and first points of path. */
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].first.x,
                   -0.5f
                      * (path_tracking_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]
                         - path_tracking_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]));
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].first.y, -0.5f);
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].last_mat.x,
                   0.5f
                      * (path_tracking_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]
                         - path_tracking_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]));
   EXPECT_FLOAT_EQ(pt_persistent.paths[1u].last_mat.y, 0.5f);
}
