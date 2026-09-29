/**
 * @file pt_path_rotation_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_path_rotation.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43907}
 */

#include "pt_path_rotation_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pt_path_rotation.c"
#include "pt_types.h"
#include <math.h>
}


/**
 * Tests overall path rotation module. Here no path information is given and thus no single rotation shall occure.
 * \uts{CSCSA-43908} \sdd{SF-7527} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Recorded_Paths__no_path_information_is_given)
{
   /** \arrange Set up all path slots to default state. */
   for (uint8_t idx = 0u; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_DEFAULT;
   }

   /** \action rotation function. */
   Pt_Rotate_Recorded_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert that nothing has changed. */
   for (uint8_t idx = 0u; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      EXPECT_EQ(pt_persistent.paths[idx].path_state, PATH_STATUS_DEFAULT);
      EXPECT_FLOAT_EQ(pt_persistent.paths[idx].first.x, 0.0f);
      EXPECT_FLOAT_EQ(pt_persistent.paths[idx].first.y, 0.0f);
      EXPECT_FLOAT_EQ(pt_persistent.paths[idx].last_mat.x, 0.0f);
      EXPECT_FLOAT_EQ(pt_persistent.paths[idx].last_mat.y, 0.0f);

      for (uint8_t point_idx = 0u; point_idx < PT_NUM_GRID_POINTS; point_idx++)
      {
         EXPECT_FLOAT_EQ(pt_persistent.paths[idx].path_points[point_idx], 0.0f);
      }
   }
}


/**
 * Tests overall path rotation module. Here a path is shifted by a given amount of positional shift.
 * \uts{CSCSA-43909} \sdd{SF-7527} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Recorded_Paths__rotate_a_single_path)
{
   /** \arrange Set up a single path which has valid path information and a single shift of 1 meter. */
   for (uint8_t idx = 0u; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_DEFAULT;
   }

   pt_persistent.paths[1u].path_state            = PATH_STATUS_MATURE;
   pt_persistent.paths[1u].first_p               = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[1u].last_p                = PT_HIGHEST_GRID_POINT_INDEX;
   pt_persistent.paths[1u].new_path_point_status = PATH_POINT_NEW_MATURE;
   pt_persistent.paths[1u].direction             = PATH_DIRECTION_LAT_RIGHT;
   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx < PT_NUM_GRID_POINTS; idx++)
   {
      pt_persistent.paths[1u].path_points[idx] = 5.0f;
   }
   pt_persistent.paths[1u].first    = Create_2d_Vector_Coordinates(5.0f, p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 2.5f);
   pt_persistent.paths[1u].last_mat = Create_2d_Vector_Coordinates(5.0f, p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 2.5f);
   /*Set up a shift.*/
   p_vehicle_data->yawrate                                       = 0.0f;
   p_vehicle_data->host_speed                                    = 20.0f;
   data.time_diff_to_last_cycle                                  = 0.05f;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_path_length_after_rot = 2u;
   /** \action call rotation function. */
   Pt_Rotate_Recorded_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Check that a shift has been applied. */
   for (uint8_t point_idx = PT_LOWEST_GRID_POINT_INDEX; point_idx < PT_NUM_GRID_POINTS; point_idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[point_idx], 4.0f);
   }
}


/**
 * Tests overall path rotation module. Here a path consisting of two points is given. Due to the shift, the path will shrink to a
 * not allowed amount of path points. Thus a reset is expected \uts{CSCSA-43910} \sdd{SF-7527} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Recorded_Paths__reset_a_path_due_to_shrinking)
{
   /** \arrange Set up a single path which has valid path information and a single shift of 1 meter. */
   for (uint8_t idx = 0u; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_DEFAULT;
   }

   pt_persistent.paths[1u].path_state                            = PATH_STATUS_MATURE;
   pt_persistent.paths[1u].first_p                               = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[1u].last_p                                = PT_LOWEST_GRID_POINT_INDEX + 1u;
   pt_persistent.paths[1u].new_path_point_status                 = PATH_POINT_NEW_MATURE;
   pt_persistent.paths[1u].path_border_status                    = PATH_BORDER_POINTS_MATURE;
   pt_persistent.paths[1u].direction                             = PATH_DIRECTION_LONG_FORWARD;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_path_length_after_rot = 2u;

   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx < PT_LOWEST_GRID_POINT_INDEX + 1u; idx++)
   {
      pt_persistent.paths[1u].path_points[idx] = 5.0f;
   }
   pt_persistent.paths[1u].first    = Create_2d_Vector_Coordinates(p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 2.5f, 5.0f);
   pt_persistent.paths[1u].last_mat = Create_2d_Vector_Coordinates(p_grid_array[PT_LOWEST_GRID_POINT_INDEX + 1u] + 0.9f, 5.0f);
   /*Set up a shift.*/
   p_vehicle_data->yawrate      = 0.0f;
   p_vehicle_data->host_speed   = 20.0f;
   data.time_diff_to_last_cycle = 0.05f;

   /** \action call rotation function. */
   Pt_Rotate_Recorded_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Check that a shift has been applied and that the path is reset. */
   EXPECT_EQ(pt_persistent.paths[1u].path_state, PATH_STATUS_DEFAULT);
}


/**
 * Checks whether path boundaries are exceeding limits after rotation. Here no path direction is given, thus false is expected.
 * \uts{CSCSA-43911} \sdd{SF-7513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Do_Path_Boundary_Exceed_Limits__no_path_direction_is_given)
{
   /** \arrange Setup a default path direction. */
   boolean_T result;
   path.direction = PATH_DIRECTION_NONE;

   /** \action call function to test. */
   result = Pt_Do_Path_Boundary_Exceed_Limits(&path, &cals);

   /** \assert Expect_value to be set on last point. */
   EXPECT_FALSE(result);
}

/**
 * Checks whether path boundaries are exceeding limits after rotation. Lateral path is in the range and both fast abs are negative.
 * Thus false is expected. \uts{CSCSA-43912} \sdd{SF-7513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Do_Path_Boundary_Exceed_Limits__lateral_path_is_inside_the_range_both_fast_abs_negative)
{
   /** \arrange lower and upper boundary are negative below the given threshold. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   path.first.x    = -cals.k_pt_move_max_value + EPSILON;
   path.last_mat.x = -cals.k_pt_move_max_value + EPSILON;

   /** \action call function to test. */
   result = Pt_Do_Path_Boundary_Exceed_Limits(&path, &cals);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}

/**
 * Checks whether path boundaries are exceeding limits after rotation. Lateral pt_persistent.paths upper border outside the range
 * and both fast abs are positive. Thus true is expected. \uts{CSCSA-43913} \sdd{SF-7513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Do_Path_Boundary_Exceed_Limits__lateral_paths_upper_border_outside_range_both_fast_abs_positive)
{
   /** \arrange setup upper border outside of range. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   path.first.x    = cals.k_pt_move_max_value - EPSILON;
   path.last_mat.x = cals.k_pt_move_max_value + EPSILON;

   /** \action call function to test. */
   result = Pt_Do_Path_Boundary_Exceed_Limits(&path, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Checks whether path boundaries are exceeding limits after rotation. Lateral pt_persistent.paths lower border outside the range
 * and both fast abs are positive. Thus true is expected. \uts{CSCSA-43914} \sdd{SF-7513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Do_Path_Boundary_Exceed_Limits__lateral_paths_lower_border_outside_range_both_fast_abs_positive)
{
   /** \arrange setup lower border outside of range. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LAT_LEFT;

   path.first.x    = cals.k_pt_move_max_value + EPSILON;
   path.last_mat.x = cals.k_pt_move_max_value - EPSILON;

   /** \action call function to test. */
   result = Pt_Do_Path_Boundary_Exceed_Limits(&path, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Checks whether path boundaries are exceeding limits after rotation. Longitudinal path is in the range and both fast abs are
 * negative. Thus false is expected. \uts{CSCSA-43915} \sdd{SF-7513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Do_Path_Boundary_Exceed_Limits__longitudinal_path_is_inside_the_range_both_fast_abs_negative)
{
   /** \arrange lower and upper boundary are negative below the given threshold. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_BACKWARD;

   path.first.y    = -cals.k_pt_move_max_value + EPSILON;
   path.last_mat.y = -cals.k_pt_move_max_value + EPSILON;

   /** \action call function to test. */
   result = Pt_Do_Path_Boundary_Exceed_Limits(&path, &cals);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}

/**
 * Checks whether path boundaries are exceeding limits after rotation. Longitudinal pt_persistent.paths upper border outside the
 * range and both fast abs are positive. Thus true is expected. \uts{CSCSA-43916} \sdd{SF-7513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Do_Path_Boundary_Exceed_Limits__longitudinal_paths_upper_border_outside_range_both_fast_abs_positive)
{
   /** \arrange setup upper border outside of range. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_BACKWARD;

   path.first.y    = cals.k_pt_move_max_value - EPSILON;
   path.last_mat.y = cals.k_pt_move_max_value + EPSILON;

   /** \action call function to test. */
   result = Pt_Do_Path_Boundary_Exceed_Limits(&path, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Tests the calculation of the transformation vector of host between the current and previous cycle. Here the rotation angle shall
 * be small. Thus a small angle approximation shall be used. \uts{CSCSA-43917} \sdd{SF-7512} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Determine_Ego_Shifting_Vector__small_angle_approximation_shall_be_used)
{
   /** \arrange setup small angle and vehicle properties. */
   Vector_2d_T ego_shift_vector, expected_shift;
   Angle_T rot_angle;
   rot_angle.angle              = -(0.9f * cals.k_pt_move_point_yaw_rate_thres_calc_ego_shift);
   data.time_diff_to_last_cycle = 0.05f;
   p_vehicle_data->host_speed   = EPSILON;
   p_vehicle_data->yawrate      = 0.07f;

   expected_shift = Create_2d_Vector_Coordinates(data.time_diff_to_last_cycle * p_vehicle_data->host_speed,
                                                 0.5f * p_vehicle_data->host_speed * p_vehicle_data->yawrate
                                                    * powf(data.time_diff_to_last_cycle, 2));

   /** \action call function to test. */
   Pt_Determine_Ego_Shifting_Vector(&ego_shift_vector, &data, &cals, &rot_angle, p_vehicle_data);

   /** \assert Expect pre calculation to be equal to the returned one. */
   EXPECT_FLOAT_EQ(ego_shift_vector.x, expected_shift.x);
   EXPECT_FLOAT_EQ(ego_shift_vector.y, expected_shift.y);
}

/**
 * Tests the calculation of the transformation vector of host between the current and previous cycle. Here the usual calculation
 * method shall be used. \uts{CSCSA-43918} \sdd{SF-7512} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Determine_Ego_Shifting_Vector__no_approximation_shall_be_used)
{
   /** \arrange setup vehicle properties. */
   Vector_2d_T ego_shift_vector, expected_shift;
   Angle_T rot_angle;
   float32_T radius;
   rot_angle                    = Create_Angle(0.0174f);
   data.time_diff_to_last_cycle = 0.05f;
   p_vehicle_data->host_speed   = EPSILON;
   p_vehicle_data->yawrate      = 0.1f;

   radius         = p_vehicle_data->host_speed / p_vehicle_data->yawrate;
   expected_shift = Create_2d_Vector_Coordinates(radius * rot_angle.sin, radius * (1.0f - rot_angle.cos));

   /** \action call function to test. */
   Pt_Determine_Ego_Shifting_Vector(&ego_shift_vector, &data, &cals, &rot_angle, p_vehicle_data);

   /** \assert Expect pre calculation to be equal to the returned one. */
   EXPECT_FLOAT_EQ(ego_shift_vector.x, expected_shift.x);
   EXPECT_FLOAT_EQ(ego_shift_vector.y, expected_shift.y);
}


/**
 * Checks whether path boundaries are exceeding limits after rotation. Longitudinal pt_persistent.paths lower border outside the
 * range and both fast abs are positive. Thus true is expected. \uts{CSCSA-43919} \sdd{SF-7513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Do_Path_Boundary_Exceed_Limits__longitudinal_paths_lower_border_outside_range_both_fast_abs_positive)
{
   /** \arrange setup lower border outside of range. */
   boolean_T result;
   path.direction = PATH_DIRECTION_LONG_BACKWARD;

   path.first.y    = cals.k_pt_move_max_value + EPSILON;
   path.last_mat.y = cals.k_pt_move_max_value - EPSILON;

   /** \action call function to test. */
   result = Pt_Do_Path_Boundary_Exceed_Limits(&path, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}

/**
 * Tests searching for a point which is unequal to zero, when iterating through the path point array from the right side. Return
 * default value in this case. \uts{CSCSA-43920} \sdd{SF-7515} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Last_Point_Unequal_Zero__last_p_is_returned)
{
   /** \arrange setup path border */
   uint8_t index_first_unequal_zero;
   path.last_p                               = PT_MID_GRID_POINT_INDEX;
   path.path_points[PT_MID_GRID_POINT_INDEX] = 1.0f;

   /** \action call function to test. */
   index_first_unequal_zero = Pt_Get_Last_Point_Unequal_Zero(&path);

   /** \assert Expect_value to be set on last point. */
   EXPECT_EQ(index_first_unequal_zero, path.last_p);
}

/**
 * Tests searching for a point which is unequal to zero, when iterating through the path point array from the right side. Here it
 * is iterated once. \uts{CSCSA-43921} \sdd{SF-7515} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Last_Point_Unequal_Zero__last_p_zero_so_that_next_slot_is_returned)
{
   /** \arrange setup path border */
   uint8_t index_first_unequal_zero;
   path.last_p                                                             = PT_MID_GRID_POINT_INDEX;
   path.first_p                                                            = PT_LOWEST_GRID_POINT_INDEX;
   path.path_points[PT_MID_GRID_POINT_INDEX]                               = -0.9f * THRESHOLD_IS_ZERO;
   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] = -1.1f * THRESHOLD_IS_ZERO;

   /** \action call function to test. */
   index_first_unequal_zero = Pt_Get_Last_Point_Unequal_Zero(&path);

   /** \assert Expect_value to be set on last point - 1. */
   EXPECT_EQ(index_first_unequal_zero, path.last_p - PT_SINGLE_GRID_POINT_OFFSET);
}

/**
 * Tests searching for a point which is unequal to zero, when iterating through the path point array from the right side. Here it
 * is iterated twice. \uts{CSCSA-43922} \sdd{SF-7515} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Last_Point_Unequal_Zero__last_p_zero_so_that_slot_in_dist_2_is_returned)
{
   /** \arrange setup path border */
   uint8_t index_first_unequal_zero;
   path.last_p                                                                 = PT_MID_GRID_POINT_INDEX;
   path.first_p                                                                = PT_LOWEST_GRID_POINT_INDEX;
   path.path_points[PT_MID_GRID_POINT_INDEX]                                   = -0.9f * THRESHOLD_IS_ZERO;
   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]     = 0.9f * THRESHOLD_IS_ZERO;
   path.path_points[PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET] = 1.1f * THRESHOLD_IS_ZERO;

   /** \action call function to test. */
   index_first_unequal_zero = Pt_Get_Last_Point_Unequal_Zero(&path);

   /** \assert Expect_value to be set on last point - 2. */
   EXPECT_EQ(index_first_unequal_zero, path.last_p - 2 * PT_SINGLE_GRID_POINT_OFFSET);
}


/**
 * Tests searching for a point which is unequal to zero, when iterating through the path point array from the right side. Return
 * default value in this case. \uts{CSCSA-43923} \sdd{SF-7514} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_First_Point_Unequal_Zero__first_p_is_returned)
{
   /** \arrange setup path border */
   uint8_t index_first_unequal_zero;
   path.first_p                              = PT_MID_GRID_POINT_INDEX;
   path.path_points[PT_MID_GRID_POINT_INDEX] = 1.0f;

   /** \action call function to test. */
   index_first_unequal_zero = Pt_Get_First_Point_Unequal_Zero(&path);

   /** \assert Expect_value to be set on first point. */
   EXPECT_EQ(index_first_unequal_zero, path.first_p);
}

/**
 * Tests searching for a point which is unequal to zero, when iterating through the path point array from the right side. Here it
 * is iterated once. \uts{CSCSA-43924} \sdd{SF-7514} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_First_Point_Unequal_Zero__first_p_zero_so_that_next_slot_is_returned)
{
   /** \arrange setup path border */
   uint8_t index_first_unequal_zero;
   path.last_p                                                             = PT_HIGHEST_GRID_POINT_INDEX;
   path.first_p                                                            = PT_MID_GRID_POINT_INDEX;
   path.path_points[PT_MID_GRID_POINT_INDEX]                               = -0.9f * THRESHOLD_IS_ZERO;
   path.path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = -1.1f * THRESHOLD_IS_ZERO;

   /** \action call function to test. */
   index_first_unequal_zero = Pt_Get_First_Point_Unequal_Zero(&path);

   /** \assert Expect_value to be set on first point - 1. */
   EXPECT_EQ(index_first_unequal_zero, path.first_p + PT_SINGLE_GRID_POINT_OFFSET);
}

/**
 * Tests searching for a point which is unequal to zero, when iterating through the path point array from the right side. Here it
 * is iterated twice. \uts{CSCSA-43925} \sdd{SF-7514} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_First_Point_Unequal_Zero__first_p_zero_so_that_slot_in_dist_2_is_returned)
{
   /** \arrange setup path border */
   uint8_t index_first_unequal_zero;
   path.last_p                                                                 = PT_HIGHEST_GRID_POINT_INDEX;
   path.first_p                                                                = PT_MID_GRID_POINT_INDEX;
   path.path_points[PT_MID_GRID_POINT_INDEX]                                   = -0.9f * THRESHOLD_IS_ZERO;
   path.path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]     = 0.9f * THRESHOLD_IS_ZERO;
   path.path_points[PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET] = 1.1f * THRESHOLD_IS_ZERO;

   /** \action call function to test. */
   index_first_unequal_zero = Pt_Get_First_Point_Unequal_Zero(&path);

   /** \assert Expect_value to be set on last point - 2. */
   EXPECT_EQ(index_first_unequal_zero, path.first_p + 2 * PT_SINGLE_GRID_POINT_OFFSET);
}


/**
 * Check whether the mapping is done correctly for the border adaption routine. Here a longitudinal path is given and first long
 * component is gt the last component. \uts{CSCSA-43926} \sdd{SF-7516} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Relev_Pos_For_Border_Adaption__longitudinal_path_first_gt_last)
{
   /** \arrange setup a longitudinal path with first long component greater than the last */
   Pt_Obj_Borders_Of_Path_T path_borders;
   path.last_mat.x = 10.0f;
   path.last_mat.y = -5.0f;
   path.first.x    = 20.0f;
   path.first.y    = -6.0f;
   path.direction  = PATH_DIRECTION_LONG_BACKWARD;

   /** \action call position determination function for border adaption. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \assert Expect mapping to be done correctly for longitudinal backward path. */
   EXPECT_TRUE(
      (path_borders.first_obj_pos_grid_component == path.last_mat.x) && (path_borders.first_obj_pos_val_component == path.last_mat.y)
      && (path_borders.last_obj_pos_grid_component == path.first.x) && (path_borders.last_obj_pos_val_component == path.first.y));
}

/**
 * Check whether the mapping is done correctly for the border adaption routine. Here a longitudinal path is given and first long
 * component is less than the last component. \uts{CSCSA-43927} \sdd{SF-7516} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Relev_Pos_For_Border_Adaption__longitudinal_path_first_smaller_than_last)
{
   /** \arrange setup a longitudinal path with first long component less than the last */
   Pt_Obj_Borders_Of_Path_T path_borders;
   path.last_mat.x = 10.0f;
   path.last_mat.y = -5.0f;
   path.first.x    = -10.0f;
   path.first.y    = -6.0f;
   path.direction  = PATH_DIRECTION_LONG_FORWARD;

   /** \action call position determination function for border adaption. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \assert Expect mapping to be done correctly for longitudinal forward path. */
   EXPECT_TRUE((path_borders.first_obj_pos_grid_component == path.first.x)
               && (path_borders.first_obj_pos_val_component == path.first.y)
               && (path_borders.last_obj_pos_grid_component == path.last_mat.x)
               && (path_borders.last_obj_pos_val_component == path.last_mat.y));
}

/**
 * Check whether the mapping is done correctly for the border adaption routine. Here a lateral path is given and first lateral
 * component is greater than the last component. \uts{CSCSA-43928} \sdd{SF-7516} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Relev_Pos_For_Border_Adaption__lateral_path_lateral_first_gt_last)
{
   /** \arrange setup a lateral right path with first long component greater than the last */
   Pt_Obj_Borders_Of_Path_T path_borders;
   path.last_mat.x = 10.0f;
   path.last_mat.y = -50.0f;
   path.first.x    = 4.0f;
   path.first.y    = 15.0f;
   path.direction  = PATH_DIRECTION_LAT_RIGHT;

   /** \action call position determination function for border adaption. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \assert Expect mapping to be done correctly for lateral right path. */
   EXPECT_TRUE(
      (path_borders.first_obj_pos_grid_component == path.last_mat.y) && (path_borders.first_obj_pos_val_component == path.last_mat.x)
      && (path_borders.last_obj_pos_grid_component == path.first.y) && (path_borders.last_obj_pos_val_component == path.first.x));
}

/**
 * Check whether the mapping is done correctly for the border adaption routine. Here a lateral path is given and first lateral
 * component is less than the last component. \uts{CSCSA-43929} \sdd{SF-7516} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Relev_Pos_For_Border_Adaption__lateral_path_first_lt_last)
{
   /** \arrange setup a lateral right path with first long component less than the last */
   Pt_Obj_Borders_Of_Path_T path_borders;
   path.last_mat.x = 10.0f;
   path.last_mat.y = 10.0f;
   path.first.x    = 4.0f;
   path.first.y    = -35.0f;
   path.direction  = PATH_DIRECTION_LAT_LEFT;

   /** \action call position determination function for border adaption. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \assert Expect mapping to be done correctly for lateral left path. */
   EXPECT_TRUE((path_borders.first_obj_pos_grid_component == path.first.y)
               && (path_borders.first_obj_pos_val_component == path.first.x)
               && (path_borders.last_obj_pos_grid_component == path.last_mat.y)
               && (path_borders.last_obj_pos_val_component == path.last_mat.x));
}

/**
 * Check whether the mapping is done correctly for the border adaption routine. Here a path without a direction but with
 * longitudinal tendencies is given. \uts{CSCSA-43930} \sdd{SF-7516} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Relev_Pos_For_Border_Adaption__approximation_of_longitudinal_path)
{
   /** \arrange setup a path with longitudinal tendencies */
   Pt_Obj_Borders_Of_Path_T path_borders;
   path.last_mat.x = -20.0f;
   path.last_mat.y = -5.0f;
   path.first.x    = -15.0f;
   path.first.y    = -6.0f;
   path.direction  = PATH_DIRECTION_NONE;

   /** \action call position determination function for border adaption. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \assert Expect mapping to be done correctly for path with longitudinal tendency. */
   EXPECT_TRUE(
      (path_borders.first_obj_pos_grid_component == path.last_mat.x) && (path_borders.first_obj_pos_val_component == path.last_mat.y)
      && (path_borders.last_obj_pos_grid_component == path.first.x) && (path_borders.last_obj_pos_val_component == path.first.y));
}

/**
 * Check whether the mapping is done correctly for the border adaption routine. Here a path without a direction but with lateral
 * tendencies is given. \uts{CSCSA-43931} \sdd{SF-7516} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Relev_Pos_For_Border_Adaption__approximation_of_lateral_path)
{
   /** \arrange setup a path with lateral tendencies */
   Pt_Obj_Borders_Of_Path_T path_borders;
   path.last_mat.x = 10.0f;
   path.last_mat.y = -50.0f;
   path.first.x    = 6.0f;
   path.first.y    = -43.0f;
   path.direction  = PATH_DIRECTION_NONE;

   /** \action call position determination function for border adaption. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \assert Expect mapping to be done correctly for path with lateral tendency. */
   EXPECT_TRUE(
      (path_borders.first_obj_pos_grid_component == path.last_mat.y) && (path_borders.first_obj_pos_val_component == path.last_mat.x)
      && (path_borders.last_obj_pos_grid_component == path.first.y) && (path_borders.last_obj_pos_val_component == path.first.x));
}

/**
 * Test the constructor for temporary path points. The boundaries are covering the whole range of grid points and thus a temporary
 * path across the whole grid range is expected. \uts{CSCSA-43932} \sdd{SF-7518} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Init_Temp_Path_Struct__init_the_complete_interval)
{
   /** \arrange set up boundaries across whole range */
   Pt_Discr_Borders_Of_Path_T temp_borders;
   Pt_Temporary_Path_T temp_path;
   temp_borders.temp_discr_first_boundary = PT_LOWEST_GRID_POINT_INDEX;
   temp_borders.temp_discr_last_boundary  = PT_HIGHEST_GRID_POINT_INDEX;

   /** \action call path temporary struct construction. */
   Pt_Init_Temp_Path_Struct(&temp_path, &temp_borders, &path_for_temp_path_init, p_grid_array);

   /** \assert Expect path accross whole range. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(temp_path.path_point_array_transform[i], path_for_temp_path_init.path_points[i]);
   }
}

/**
 * Test the constructor for temporary path points. The boundaries are not defined of grid points and thus a temporary path filled
 * with zero is expected. \uts{CSCSA-43933} \sdd{SF-7518} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Init_Temp_Path_Struct__boundaries_not_defined)
{
   /** \arrange set up undefined boundaries */
   Pt_Discr_Borders_Of_Path_T temp_borders;
   Pt_Temporary_Path_T temp_path;
   temp_borders.temp_discr_first_boundary = PT_INVALID_GRID_POINT_INDEX;
   temp_borders.temp_discr_last_boundary  = PT_INVALID_GRID_POINT_INDEX;

   /** \action call path temporary struct construction. */
   Pt_Init_Temp_Path_Struct(&(temp_path), &(temp_borders), &(path_for_temp_path_init), p_grid_array);

   /** \assert Expect default value of zero filled in the path struct.. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(temp_path.path_point_array_transform[i], PT_PATH_POINTS_DEFAULT_VAL);
   }
}

/**
 * Test the constructor for temporary path points. The boundaries are not covering the whole grid range. Thus a partly filled path
 * is expected \uts{CSCSA-43934} \sdd{SF-7518} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Init_Temp_Path_Struct__borders_are_not_covering_the_whole_grid_array_range)
{
   /** \arrange set up boundaries which are not covering the whole area. */
   Pt_Discr_Borders_Of_Path_T temp_borders;
   Pt_Temporary_Path_T temp_path;
   temp_borders.temp_discr_first_boundary = 5u;
   temp_borders.temp_discr_last_boundary  = 13u;

   /** \action call path temporary struct construction. */
   Pt_Init_Temp_Path_Struct(&(temp_path), &(temp_borders), &(path_for_temp_path_init), p_grid_array);

   /** \assert Expect parts of structure which are contained in the boundaries, to be updated to non default values. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      if (i >= temp_borders.temp_discr_first_boundary && i <= temp_borders.temp_discr_last_boundary)
      {
         EXPECT_FLOAT_EQ(temp_path.path_point_array_transform[i], path_for_temp_path_init.path_points[i]);
      }
      else
      {
         EXPECT_FLOAT_EQ(temp_path.path_point_array_transform[i], PT_PATH_POINTS_DEFAULT_VAL);
      }
   }
}

/**
 * Tests transformation of a point to via rotation and translation. Here only a translation is given.
 * \uts{CSCSA-43935} \sdd{SF-7523} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Transform_Path_Point_To_New_Vcs_Pos__shift_a_point_pure_translation_is_given)
{
   /** \arrange Set up a pure translation. */
   Vector_2d_T point_to_transform, ego_vehicle_shift_vector, expected_vector;
   Angle_T rot_angle;
   point_to_transform       = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   ego_vehicle_shift_vector = Create_2d_Vector_Coordinates(-1.0f, 0.0f);
   rot_angle                = Create_Angle(0.0f);
   expected_vector          = Vector_2d_Alg_Diff(&point_to_transform, &ego_vehicle_shift_vector);

   /** \action call rotation routine for a single point. */
   Pt_Transform_Path_Point_To_New_Vcs_Pos(&point_to_transform, &ego_vehicle_shift_vector, &rot_angle, p_vehicle_data);

   /** \assert Expect that the calculated modified point is equal to the precalculated one. */
   EXPECT_NEAR(point_to_transform.x, expected_vector.x, EPSILON);
   EXPECT_NEAR(point_to_transform.y, expected_vector.y, EPSILON);
}

/**
 * Tests transformation of a point to via rotation and translation. Here only a rotation is given.
 * \uts{CSCSA-43936} \sdd{SF-7523} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Transform_Path_Point_To_New_Vcs_Pos__rotate_a_point_pure_rotation_is_given)
{
   /** \arrange Set up a pure rotation. */
   Vector_2d_T point_to_transform, ego_vehicle_shift_vector, expected_vector;
   Angle_T rot_angle;
   point_to_transform       = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   ego_vehicle_shift_vector = Create_2d_Vector_Coordinates(0.0f, 0.0f);
   rot_angle                = Create_Angle(0.5 * PI);
   expected_vector          = Create_2d_Vector_Coordinates(0.0f, -1.0f);

   /** \action call rotation routine for a single point. */
   Pt_Transform_Path_Point_To_New_Vcs_Pos(&point_to_transform, &ego_vehicle_shift_vector, &rot_angle, p_vehicle_data);

   /** \assert Expect that the calculated modified point is equal to the precalculated one. */
   EXPECT_NEAR(point_to_transform.x, expected_vector.x, EPSILON);
   EXPECT_NEAR(point_to_transform.y, expected_vector.y, EPSILON);
}

/**
 * Tests transformation of a point to via rotation and translation. Here a rotation and translation is combined.
 * \uts{CSCSA-43937} \sdd{SF-7523} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Transform_Path_Point_To_New_Vcs_Pos__rotate_and_shift_a_point)
{
   /** \arrange Set up a combined translation and rotation. */
   Vector_2d_T point_to_transform, ego_vehicle_shift_vector, expected_vector;
   Angle_T rot_angle;

   point_to_transform       = Create_2d_Vector_Coordinates(1.0f, 0.0f);
   ego_vehicle_shift_vector = Create_2d_Vector_Coordinates(-1.0f, 0.0f);
   rot_angle                = Create_Angle(0.5 * PI);
   expected_vector          = Create_2d_Vector_Coordinates(0.0f, -2.0f);

   /** \action call rotation routine for a single point. */
   Pt_Transform_Path_Point_To_New_Vcs_Pos(&point_to_transform, &ego_vehicle_shift_vector, &rot_angle, p_vehicle_data);

   /** \assert Expect that the calculated modified point is equal to the precalculated one. */
   EXPECT_NEAR(point_to_transform.x, expected_vector.x, EPSILON);
   EXPECT_NEAR(point_to_transform.y, expected_vector.y, EPSILON);
}


/**
 * Tests the functionality of rotating path points by a given shift vector. Here a longitudinal path shall be rotated.
 * \uts{CSCSA-43938} \sdd{SF-7524} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Transform_Path_Points_And_Grid_Idx__longitudinal_path_to_lat)
{
   /** \arrange setup shifting vector and input path. */
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   Pt_Discr_Borders_Of_Path_T init_borders;
   Pt_Temporary_Path_T temp_path;

   float32_T shift_x = -3.0f;
   float32_T shift_y = -1.0f;

   init_borders.temp_discr_first_boundary = PT_LOWEST_GRID_POINT_INDEX;
   init_borders.temp_discr_last_boundary  = PT_HIGHEST_GRID_POINT_INDEX;

   rot_angle                         = Create_Angle(-0.5f * PI);
   ego_shift                         = Create_2d_Vector_Coordinates(-3.0f, -1.0f);
   path_for_temp_path_init.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action call function to test. */
   Pt_Init_Temp_Path_Struct(&(temp_path), &(init_borders), &(path_for_temp_path_init), p_grid_array);
   Pt_Transform_Path_Points_And_Grid_Idx(&temp_path, &init_borders, &path_for_temp_path_init, &ego_shift, &rot_angle, p_vehicle_data);

   /** \assert Expect path points and grid to be transformed by the shifting vector. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(temp_path.grid_point_table_transform[i], (-path_for_temp_path_init.path_points[i] + shift_y));
      EXPECT_FLOAT_EQ(temp_path.path_point_array_transform[i], (p_grid_array[i] - shift_x));
   }
}

/**
 * Tests the functionality of rotating path points by a given shift vector. Here a lateral path shall be rotated.
 * \uts{CSCSA-43939} \sdd{SF-7524} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Transform_Path_Points_And_Grid_Idx__lateral_path_to_long)
{
   /** \arrange setup shifting vector and input path. */
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   Pt_Discr_Borders_Of_Path_T init_borders;
   Pt_Temporary_Path_T temp_path;

   float32_T shift_x = -3.0f;
   float32_T shift_y = -1.0f;

   init_borders.temp_discr_first_boundary = PT_LOWEST_GRID_POINT_INDEX;
   init_borders.temp_discr_last_boundary  = PT_HIGHEST_GRID_POINT_INDEX;

   rot_angle                         = Create_Angle(-0.5f * PI);
   ego_shift                         = Create_2d_Vector_Coordinates(-3.0f, -1.0f);
   path_for_temp_path_init.direction = PATH_DIRECTION_LAT_LEFT;

   /** \action call function to test. */
   Pt_Init_Temp_Path_Struct(&(temp_path), &(init_borders), &(path_for_temp_path_init), p_grid_array);
   Pt_Transform_Path_Points_And_Grid_Idx(&temp_path, &init_borders, &path_for_temp_path_init, &ego_shift, &rot_angle, p_vehicle_data);

   /** \assert Expect path points and grid to be transformed by the shifting vector. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(temp_path.grid_point_table_transform[i], (+path_for_temp_path_init.path_points[i] - shift_x));
      EXPECT_FLOAT_EQ(temp_path.path_point_array_transform[i], (-p_grid_array[i] + shift_y));
   }
}


/**
 * Tests the functionality of rotating path points by a given shift vector. Here a path shall be rotated which has longitudinal
 * tendencies. \uts{CSCSA-43940} \sdd{SF-7524} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Transform_Path_Points_And_Grid_Idx__path_has_longitudinal_tendency)
{
   /** \arrange setup shifting vector and input path. */
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   Pt_Discr_Borders_Of_Path_T init_borders;
   Pt_Temporary_Path_T temp_path;

   float32_T shift_x = -3.0f;
   float32_T shift_y = -1.0f;

   init_borders.temp_discr_first_boundary = PT_LOWEST_GRID_POINT_INDEX;
   init_borders.temp_discr_last_boundary  = PT_HIGHEST_GRID_POINT_INDEX;

   rot_angle                         = Create_Angle(0.0f);
   ego_shift                         = Create_2d_Vector_Coordinates(shift_x, shift_y);
   path_for_temp_path_init.direction = PATH_DIRECTION_NONE;
   path_for_temp_path_init.first     = Create_2d_Vector_Coordinates(5.0f, 1.0f);
   path_for_temp_path_init.last_mat  = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   /** \action call function to test. */
   Pt_Init_Temp_Path_Struct(&(temp_path), &(init_borders), &(path_for_temp_path_init), p_grid_array);
   Pt_Transform_Path_Points_And_Grid_Idx(&temp_path, &init_borders, &path_for_temp_path_init, &ego_shift, &rot_angle, p_vehicle_data);

   /** \assert Expect path points and grid to be transformed by the shifting vector. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(temp_path.path_point_array_transform[i], (path_for_temp_path_init.path_points[i] - shift_y));
      EXPECT_FLOAT_EQ(temp_path.grid_point_table_transform[i], (p_grid_array[i] - shift_x));
   }
}

/**
 * Tests the functionality of rotating path points by a given shift vector. Here a path shall be rotated which has lateral
 * tendencies. \uts{CSCSA-43941} \sdd{SF-7524} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Transform_Path_Points_And_Grid_Idx__path_has_lateral_tendency)
{
   /** \arrange setup shifting vector and input path. */
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   Pt_Discr_Borders_Of_Path_T init_borders;
   Pt_Temporary_Path_T temp_path;

   float32_T shift_x = -3.0f;
   float32_T shift_y = -1.0f;

   init_borders.temp_discr_first_boundary = PT_LOWEST_GRID_POINT_INDEX;
   init_borders.temp_discr_last_boundary  = PT_HIGHEST_GRID_POINT_INDEX;

   rot_angle                         = Create_Angle(0.0f);
   ego_shift                         = Create_2d_Vector_Coordinates(shift_x, shift_y);
   path_for_temp_path_init.direction = PATH_DIRECTION_NONE;
   path_for_temp_path_init.first     = Create_2d_Vector_Coordinates(1.0f, 5.0f);
   path_for_temp_path_init.last_mat  = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   /** \action call function to test. */
   Pt_Init_Temp_Path_Struct(&(temp_path), &(init_borders), &(path_for_temp_path_init), p_grid_array);
   Pt_Transform_Path_Points_And_Grid_Idx(&temp_path, &init_borders, &path_for_temp_path_init, &ego_shift, &rot_angle, p_vehicle_data);

   /** \assert Expect path points and grid to be transformed by the shifting vector. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(temp_path.path_point_array_transform[i], (path_for_temp_path_init.path_points[i] - shift_x));
      EXPECT_FLOAT_EQ(temp_path.grid_point_table_transform[i], (p_grid_array[i] - shift_y));
   }
}

/**
 * Checks the shrinkage functionality of path tracking rotation. Here the given path has a reduction of path borders on both sides.
 * \uts{CSCSA-43942} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__lateral_path_has_shrunk_at_both_borders)
{
   /** \arrange Setup a path such that shrinkage occures on both path sides. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;

   val_first     = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   val_last      = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(3.0f, p_grid_array[val_first] + EPSILON);
   path.last_mat = Create_2d_Vector_Coordinates(-3.0f, p_grid_array[val_last] - EPSILON);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, default_slope, default_offset, p_grid_array);
   path.direction = PATH_DIRECTION_LAT_RIGHT;

   /** \action call function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect the borders to be shrunk. */
   EXPECT_EQ(path.first_p, val_first + ((uint8_t) PT_SINGLE_GRID_POINT_OFFSET));
   EXPECT_EQ(path.last_p, val_last - ((uint8_t) PT_SINGLE_GRID_POINT_OFFSET));
}

/**
 * Checks the shrinkage functionality of path tracking rotation. Here the given path has a reduction of path borders on both sides
 * by multiple points. \uts{CSCSA-43943} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__both_borders_has_shrunk_by_three_points)
{
   /** \arrange Setup a path such that shrinkage occures on both path sides by multiple points. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;

   val_first     = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   val_last      = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(3.0f, p_grid_array[val_first + 2 * PT_SINGLE_GRID_POINT_OFFSET] + EPSILON);
   path.last_mat = Create_2d_Vector_Coordinates(-3.0f, p_grid_array[val_last - 2 * PT_SINGLE_GRID_POINT_OFFSET] - EPSILON);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, default_slope, default_offset, p_grid_array);
   path.direction = PATH_DIRECTION_LAT_RIGHT;
   /** \action call function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect the borders to be shrunk. */
   EXPECT_EQ(path.first_p, val_first + ((uint8_t) 3u * PT_SINGLE_GRID_POINT_OFFSET));
   EXPECT_EQ(path.last_p, val_last - ((uint8_t) 3u * PT_SINGLE_GRID_POINT_OFFSET));
}


/**
 * Checks the shrinkage functionality of path tracking rotation. Here the given path is fading out of the grid array on the right
 * side with its lower border. \uts{CSCSA-43944} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__first_point_index_is_fading_out_to_the_positive_vcs_axis)
{
   /** \arrange Setup a path such that shrinkage occures on maximum possible path index. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;

   val_first     = PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   val_last      = PT_HIGHEST_GRID_POINT_INDEX;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(3.0f, p_grid_array[val_first] + EPSILON);
   path.last_mat = Create_2d_Vector_Coordinates(-3.0f, p_grid_array[val_last] + EPSILON);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, default_slope, default_offset, p_grid_array);
   path.direction = PATH_DIRECTION_LAT_RIGHT;

   /** \action call function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect the lower border to be shrunk and saturated. */
   EXPECT_FLOAT_EQ(path.first_p, PT_HIGHEST_GRID_POINT_INDEX);
   EXPECT_FLOAT_EQ(path.last_p, val_last);
}


/**
 * Checks the shrinkage functionality of path tracking rotation. Here the given path is fading out of the grid array on the left
 * side with its upper border. \uts{CSCSA-43945} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__last_point_index_is_fading_out_to_the_negative_vcs_axis)
{
   /** \arrange Setup a path such that shrinkage occures on the lowest possible path point index. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;

   val_first     = PT_LOWEST_GRID_POINT_INDEX;
   val_last      = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(3.0f, p_grid_array[val_first] - EPSILON);
   path.last_mat = Create_2d_Vector_Coordinates(-3.0f, p_grid_array[val_last] - EPSILON);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, default_slope, default_offset, p_grid_array);
   path.direction = PATH_DIRECTION_LAT_RIGHT;

   /** \action call function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect the upper border to be shrunk and saturated. */
   EXPECT_FLOAT_EQ(path.first_p, val_first);
   EXPECT_FLOAT_EQ(path.last_p, PT_LOWEST_GRID_POINT_INDEX);
}

/**
 * Checks the shrinkage functionality of path tracking rotation. Here no shrinkage occured.
 * \uts{CSCSA-43946} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__no_skrinking_occured)
{
   /** \arrange Setup a path without any shrinkage. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;

   val_first     = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   val_last      = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(3.0f, p_grid_array[val_first] - EPSILON);
   path.last_mat = Create_2d_Vector_Coordinates(3.0f, p_grid_array[val_last] + EPSILON);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, default_slope, default_offset, p_grid_array);

   path.direction = PATH_DIRECTION_LAT_LEFT;

   /** \action call function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect border to be kept. */
   EXPECT_EQ(path.first_p, val_first);
   EXPECT_EQ(path.last_p, val_last);
}

/**
 * Checks the shrinkage functionality of path tracking rotation. Here a shrinkage is applied on both sides of the path. Thus a
 * reduction of path length by two is expected. \uts{CSCSA-43947} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__shrinking_to_both_sides_occured)
{
   /** \arrange Setup a path and parameters such that a shrinkage on both path sides is occuring. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;

   val_first     = 5;
   val_last      = 12;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(0.5f * (p_grid_array[val_first] + p_grid_array[val_first + 1]), -3.0f);
   path.last_mat = Create_2d_Vector_Coordinates(0.5f * (p_grid_array[val_last - 1] + p_grid_array[val_last]), -5.0f);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, default_slope, default_offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_FORWARD;
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \action call shrinkage function. */
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect a size reduction on both path sides. */
   EXPECT_EQ(path.first_p, val_first + 1);
   EXPECT_EQ(path.last_p, val_last - 1);
}


/**
 * Checks the shrinkage functionality of path tracking rotation. Here a path in creation phase is tested which consists of only one
 * point at the negative outhermost path tracking zone. Check whether the default value is set correctly for this
 * pt_persistent.paths borders. \uts{CSCSA-43948} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__one_point_path_shrinks_at_lowest_border_lesson_learned)
{
   /** \arrange Setup a single point path such that shrinkage occures due to last_p. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;
   float32_T offset = 2.0f;
   float32_T slope  = 0.0f;

   val_first                                    = PT_LOWEST_GRID_POINT_INDEX;
   val_last                                     = PT_LOWEST_GRID_POINT_INDEX;
   path.first_p                                 = val_first;
   path.last_p                                  = val_last;
   path.first                                   = Create_2d_Vector_Coordinates(p_grid_array[val_first] - 2.5f, offset);
   path.last_mat                                = Create_2d_Vector_Coordinates(p_grid_array[val_first] - EPSILON, offset);
   path.path_points[PT_LOWEST_GRID_POINT_INDEX] = offset;

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope, offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_FORWARD;
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \action call shrinkage function. */
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect a default size. */
   EXPECT_EQ(path.first_p, PT_DEFAULT_DISCR_BORDER);
   EXPECT_EQ(path.last_p, PT_DEFAULT_DISCR_BORDER);
}


/**
 * Checks the shrinkage functionality of path tracking rotation. Here a path in creation phase is tested which consists of two
 * points at the negative outhermost path tracking zone. Check whether both borders are equal after the call of this unit.
 * \uts{CSCSA-43949} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__two_point_path_shrinks_at_second_lowest_border_lesson_learned)
{
   /** \arrange Setup a path with two points at the outermist negative path tracking zone. Set up paramters in a way that a
    * shrinkage is occuring due to the last_p border. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;
   float32_T offset = 2.0f;
   float32_T slope  = 0.0f;

   val_first                                    = PT_LOWEST_GRID_POINT_INDEX;
   val_last                                     = PT_LOWEST_GRID_POINT_INDEX + 1u;
   path.first_p                                 = val_first;
   path.last_p                                  = val_last;
   path.first                                   = Create_2d_Vector_Coordinates(p_grid_array[val_first] - 2.5f, offset);
   path.last_mat                                = Create_2d_Vector_Coordinates(p_grid_array[val_last] - EPSILON, offset);
   path.path_points[PT_LOWEST_GRID_POINT_INDEX] = offset;

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope, offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_FORWARD;
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \action call shrinkage function. */
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect borders to equal. */
   EXPECT_EQ(path.first_p, path.last_p);
}

/**
 * Checks the shrinkage functionality of path tracking rotation. Here a path in creation phase is tested which consists of only one
 * point at the positive outhermost path tracking zone. Check whether the default value is set correctly for this
 * pt_persistent.paths borders. \uts{CSCSA-43950} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__one_point_path_shrinks_at_highest_border_lesson_learned)
{
   /** \arrange Setup a single point path such that a shrinkage occurs due to first_p. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;
   float32_T offset = 2.0f;
   float32_T slope  = 0.0f;

   val_first                                     = PT_HIGHEST_GRID_POINT_INDEX;
   val_last                                      = PT_HIGHEST_GRID_POINT_INDEX;
   path.first_p                                  = val_first;
   path.last_p                                   = val_last;
   path.first                                    = Create_2d_Vector_Coordinates(p_grid_array[val_last] + EPSILON, offset);
   path.last_mat                                 = Create_2d_Vector_Coordinates(p_grid_array[val_last] + 2.5f, offset);
   path.path_points[PT_HIGHEST_GRID_POINT_INDEX] = offset;

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope, offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_BACKWARD;
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \action call shrinkage function. */
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect a default size. */
   EXPECT_EQ(path.first_p, PT_DEFAULT_DISCR_BORDER);
   EXPECT_EQ(path.last_p, PT_DEFAULT_DISCR_BORDER);
}

/**
 * Checks the shrinkage functionality of path tracking rotation. Here a path in creation phase is tested which consists of two
 * points at the positive outhermost path tracking zone. Here shrinkage occures at first_p thus first_p=last_p is expected.
 * \uts{CSCSA-43951} \sdd{SF-7507} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Shrinkage__two_point_path_shrinks_at_second_highest_border_lesson_learned)
{
   /** \arrange Setup a path consisting of two points such that a shrinkage is occuring at first_p. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   uint8_t val_first, val_last;
   float32_T offset = 2.0f;
   float32_T slope  = 0.0f;

   val_first                   = PT_HIGHEST_GRID_POINT_INDEX - 1u;
   val_last                    = PT_HIGHEST_GRID_POINT_INDEX;
   path.first_p                = val_first;
   path.last_p                 = val_last;
   path.first                  = Create_2d_Vector_Coordinates(p_grid_array[val_first] + EPSILON, offset);
   path.last_mat               = Create_2d_Vector_Coordinates(p_grid_array[val_last] + 2.5f, offset);
   path.path_points[val_first] = offset;
   path.path_points[val_last]  = offset;

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope, offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_BACKWARD;
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);

   /** \action call shrinkage function. */
   Pt_Adapt_Path_For_Shrinkage(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect borders to be equal. */
   EXPECT_EQ(path.first_p, path.last_p);
}


/**
 * Tests the extension functionality of rotation module in path tracking. Here the path shall be extended at the lower path border.
 * \uts{CSCSA-43952} \sdd{SF-7506} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Extension__path_extension_occured_at_lower_path_border)
{
   /** \arrange Setup the rotated grid and path array so that an extension is available at the lower border. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   uint8_t val_first, val_last;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   float32_T slope_of_path = 0.0f;

   val_first    = 5;
   val_last     = 12;
   path.first_p = val_first;
   path.last_p  = val_last;
   path.first   = Create_2d_Vector_Coordinates(0.5f * (p_grid_array[val_first - 2] + p_grid_array[val_first - 1]), default_offset);
   path.last_mat = Create_2d_Vector_Coordinates(0.5f * (p_grid_array[val_last] + p_grid_array[val_last + 1]), default_offset);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope_of_path, default_offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action executes function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Extension(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect the border to be extended. */
   EXPECT_EQ(path.first_p, val_first - 1);

   EXPECT_EQ(path.last_p, val_last);
   EXPECT_EQ(temp_path_to_transforms.path_point_array_transform[path.first_p], default_offset);
}

/**
 * Tests the extension functionality of rotation module in path tracking. Here the path shall be extended at the upper path border.
 * \uts{CSCSA-43953} \sdd{SF-7506} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Extension__path_extension_occured_at_upper_path_border)
{
   /** \arrange Setup the rotated grid and path array so that an extension is available at the upper border. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   uint8_t val_first, val_last;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   float32_T slope_of_path = 0.0f;

   val_first     = 5u;
   val_last      = 12u;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(0.5f * (p_grid_array[val_first - 1] + p_grid_array[val_first]), default_offset);
   path.last_mat = Create_2d_Vector_Coordinates(0.5f * (p_grid_array[val_last + 1] + p_grid_array[val_last + 2]), default_offset);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope_of_path, default_offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action executes function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Extension(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect the border to be extended. */
   EXPECT_EQ(path.first_p, val_first);
   EXPECT_EQ(path.last_p, val_last + 1);
   EXPECT_EQ(temp_path_to_transforms.path_point_array_transform[path.last_p], default_offset);
}

/**
 * Tests the extension functionality of rotation module in path tracking. Here the path shall be extended at the maximum possible
 * borders (lower and under). This overflow shall be catched from function side. \uts{CSCSA-43954} \sdd{SF-7506}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Extension__path_extension_occured_in_both_parts_at_the_maximum_possible_borders)
{
   /** \arrange Setup the rotated grid and path array so that an extension is available at both border. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   uint8_t val_first, val_last;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   float32_T slope_of_path = 0.0f;

   val_first     = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   val_last      = PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(p_grid_array[PT_LOWEST_GRID_POINT_INDEX]
                                                   - (0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]),
                                                default_offset);
   path.last_mat = Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                   + (0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]),
                                                default_offset);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope_of_path, default_offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_FORWARD;
   /** \action executes function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Extension(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect borders to be extended. */
   EXPECT_EQ(path.first_p, PT_LOWEST_GRID_POINT_INDEX);
   EXPECT_EQ(path.last_p, PT_HIGHEST_GRID_POINT_INDEX);
   EXPECT_EQ(temp_path_to_transforms.path_point_array_transform[path.last_p], default_offset);
   EXPECT_EQ(temp_path_to_transforms.path_point_array_transform[path.first_p], default_offset);
}

/**
 * Tests the extension functionality of rotation module in path tracking. Catch the case when multiple border indices are extended.
 * \uts{CSCSA-43955} \sdd{SF-7506} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Extension__path_extension_by_multiple_points_occured_in_both_parts)
{
   /** \arrange Setup the rotated grid and path array so that multiple extensions are available at both borders. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   uint8_t val_first, val_last;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   float32_T slope_of_path = 0.0f;

   val_first     = 5u;
   val_last      = 12u;
   path.first_p  = val_first;
   path.last_p   = val_last;
   path.first    = Create_2d_Vector_Coordinates(p_grid_array[val_first - 3 * PT_SINGLE_GRID_POINT_OFFSET], default_offset);
   path.last_mat = Create_2d_Vector_Coordinates(p_grid_array[val_last + 3 * PT_SINGLE_GRID_POINT_OFFSET], default_offset);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope_of_path, default_offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action executes function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Extension(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect borders to be extended and all path points inbetween to be filled. */
   EXPECT_EQ(path.first_p, val_first - 3 * PT_SINGLE_GRID_POINT_OFFSET);
   EXPECT_EQ(path.last_p, val_last + 3 * PT_SINGLE_GRID_POINT_OFFSET);
   for (uint8_t idx = path.first_p; idx <= path.last_p; idx++)
   {
      EXPECT_EQ(temp_path_to_transforms.path_point_array_transform[idx], default_offset);
      EXPECT_EQ(temp_path_to_transforms.path_point_array_transform[idx], default_offset);
   }
}

/**
 * Tests the extension functionality of rotation module in path tracking. Here no path extension is given, thus border shall be
 * kept and False as flags shall be returned \uts{CSCSA-43956} \sdd{SF-7506} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Path_For_Extension__no_path_extension_is_given_borders_shall_be_kept)
{
   /** \arrange Setup the rotated grid and path array so that no single extension is given. */
   Pt_Obj_Borders_Of_Path_T path_borders;
   Pt_Temporary_Path_T temp_path_to_transforms;
   uint8_t val_first, val_last;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   float32_T slope_of_path = 0.0f;

   val_first    = 5u;
   val_last     = 12u;
   path.first_p = val_first;
   path.last_p  = val_last;
   path.first   = Create_2d_Vector_Coordinates(
        p_grid_array[val_first] - 0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET], default_offset);
   path.last_mat = Create_2d_Vector_Coordinates(
      p_grid_array[val_last] + 0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET], default_offset);

   Pt_Init_Path_Points_Linearly(temp_path_to_transforms.path_point_array_transform, temp_path_to_transforms.grid_point_table_transform,
                                path.first_p, path.last_p, slope_of_path, default_offset, p_grid_array);

   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action executes function to test. */
   Pt_Get_Relev_Pos_For_Border_Adaption(&path, &path_borders);
   Pt_Adapt_Path_For_Extension(&path, &temp_path_to_transforms, &path_borders, &discr_borders_of_path, p_grid_array);

   /** \assert Expect borders to be extended and all path points inbetween to be filled. */
   EXPECT_EQ(path.first_p, val_first);
   EXPECT_EQ(path.last_p, val_last);
}

/**
 * Tests the extension check of path rotation module. Here a longidutinal path is extended at both borders.
 * \uts{CSCSA-43957} \sdd{SF-7510} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Extended__longitudinal_path_at_both_borders_extendable_and_also_extended)
{
   /** \arrange setup path conditions so that the ability to extend as well as the extension is given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LONG_FORWARD;
   path.first_p                                             = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first.x                                             = p_grid_array[path.first_p - PT_SINGLE_GRID_POINT_OFFSET] - EPSILON;
   path.last_mat.x                                          = p_grid_array[path.last_p + PT_SINGLE_GRID_POINT_OFFSET] + EPSILON;

   /** \action call function to check whether path has extended. */
   Pt_Check_Whether_Path_Has_Extended(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect both borders of longitudinal path to be extended. */
   EXPECT_TRUE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_TRUE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}

/**
 * Tests the extension check of path rotation module. Here a longidutinal path is extendable but no extension is given.
 * \uts{CSCSA-43958} \sdd{SF-7510} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Extended__longitudinal_path_at_both_borders_extendable_but_no_extension_given)
{
   /** \arrange setup path conditions so that the ability to extend but no extension is given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LONG_FORWARD;
   path.first_p                                             = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first.x                                             = p_grid_array[path.first_p - PT_SINGLE_GRID_POINT_OFFSET] + EPSILON;
   path.last_mat.x                                          = p_grid_array[path.last_p + PT_SINGLE_GRID_POINT_OFFSET] - EPSILON;

   /** \action call function to check whether path has extended. */
   Pt_Check_Whether_Path_Has_Extended(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect that path borders are not extended. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}


/**
 * Tests the extension check of path rotation module. Here a longidutinal path is neither extendable nor an extension is given.
 * \uts{CSCSA-43959} \sdd{SF-7510} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Extended__longitudinal_path_is_not_extendable)
{
   /** \arrange setup path conditions so that the ability to extend and extension is not given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LONG_FORWARD;
   path.first_p                                             = PT_DEFAULT_MATCH_INDEX;
   path.last_p                                              = PT_NUM_GRID_POINTS;

   /** \action call function to check whether path has extended. */
   Pt_Check_Whether_Path_Has_Extended(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect that path borders are not extended. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}


/**
 * Tests the extension check of path rotation module. Here a lateral path is extended at both borders.
 * \uts{CSCSA-43960} \sdd{SF-7510} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Extended__lateral_path_at_both_borders_extendable_and_also_extended)
{
   /** \arrange setup path conditions so that the ability to extend as well as the extension is given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LAT_LEFT;
   path.first_p                                             = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first.y                                             = p_grid_array[path.first_p - PT_SINGLE_GRID_POINT_OFFSET] - EPSILON;
   path.last_mat.y                                          = p_grid_array[path.last_p + PT_SINGLE_GRID_POINT_OFFSET] + EPSILON;

   /** \action call function to check whether path has extended. */
   Pt_Check_Whether_Path_Has_Extended(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect both borders of lateral path to be extended. */
   EXPECT_TRUE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_TRUE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}

/**
 * Tests the extension check of path rotation module. Here a lateral path is extendable but no extension is given.
 * \uts{CSCSA-43961} \sdd{SF-7510} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Extended__lateral_path_at_both_borders_extendable_but_no_extension_given)
{
   /** \arrange setup path conditions so that the ability to extend but no extension is given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LAT_LEFT;
   path.first_p                                             = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first.y                                             = p_grid_array[path.first_p - PT_SINGLE_GRID_POINT_OFFSET] + EPSILON;
   path.last_mat.y                                          = p_grid_array[path.last_p + PT_SINGLE_GRID_POINT_OFFSET] - EPSILON;

   /** \action call function to check whether path has extended. */
   Pt_Check_Whether_Path_Has_Extended(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect that path borders are not extended. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}


/**
 * Tests the extension check of path rotation module. Here a lateral path is neither extendable nor an extension is given.
 * \uts{CSCSA-43962} \sdd{SF-7510} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Extended__lateral_path_is_not_extendable)
{
   /** \arrange setup path conditions so that the ability to extend and extension is not given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LAT_LEFT;
   path.first_p                                             = PT_HIGHEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call function to check whether path has extended. */
   Pt_Check_Whether_Path_Has_Extended(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect that path borders are not extended. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}

/**
 * Tests the extension check of path rotation module. Here a path without any direction is given. Thus no extension check shall be
 * given \uts{CSCSA-43963} \sdd{SF-7510} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Extended__no_path_direction_given_thus_no_extension_check)
{
   /** \arrange setup path conditions so that the ability to extend and extension is not given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_NONE;
   path.first_p                                             = PT_DEFAULT_MATCH_INDEX;
   path.last_p                                              = PT_DEFAULT_MATCH_INDEX;

   /** \action call function to check whether path has extended. */
   Pt_Check_Whether_Path_Has_Extended(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect no extension since no direction is given. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}

/**
 * Tests the shrinkage check of path rotation module. Here a longitudinal path with shrinkage at both borders is given. Thus true
 * is expected in both cases \uts{CSCSA-43964} \sdd{SF-7511} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Shrunk__longitudinal_path_at_both_borders_shrinkage_is_given)
{
   /** \arrange setup path conditions so that shrinkage is given at both sides */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LONG_BACKWARD;
   path.first_p                                             = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first.x                                             = p_grid_array[path.first_p] + EPSILON;
   path.last_mat.x                                          = p_grid_array[path.last_p] - EPSILON;

   /** \action call function to check whether path has shrunk. */
   Pt_Check_Whether_Path_Has_Shrunk(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect shrinkage at both sides. */
   EXPECT_TRUE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_TRUE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}

/**
 * Tests the shrinkage check of path rotation module. Here a longitudinal path without any shrinkage is given. Thus false is
 * expected in both cases \uts{CSCSA-43965} \sdd{SF-7511} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Shrunk__longitudinal_path_at_both_borders_no_shrinkage_is_given)
{
   /** \arrange setup path conditions so that no shrinkage is given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LONG_BACKWARD;
   path.first_p                                             = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first.x                                             = p_grid_array[path.first_p] - EPSILON;
   path.last_mat.x                                          = p_grid_array[path.last_p] + EPSILON;

   /** \action call function to check whether path has shrunk. */
   Pt_Check_Whether_Path_Has_Shrunk(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect no shrinkage. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}


/**
 * Tests the shrinkage check of path rotation module. Here a lateral path with shrinkage at both borders is given. Thus true is
 * expected in both cases \uts{CSCSA-43966} \sdd{SF-7511} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Shrunk__lateral_path_at_both_borders_shrinkage_is_given)
{
   /** \arrange setup path conditions so that shrinkage is given at both sides */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LAT_LEFT;
   path.first_p                                             = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first.y                                             = p_grid_array[path.first_p] + EPSILON;
   path.last_mat.y                                          = p_grid_array[path.last_p] - EPSILON;

   /** \action call function to check whether path has shrunk. */
   Pt_Check_Whether_Path_Has_Shrunk(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect shrinkage at both sides. */
   EXPECT_TRUE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_TRUE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}

/**
 * Tests the shrinkage check of path rotation module. Here a lateral path without any shrinkage is given. Thus false is expected in
 * both cases \uts{CSCSA-43967} \sdd{SF-7511} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Shrunk__lateral_path_at_both_borders_no_shrinkage_is_given)
{
   /** \arrange setup path conditions so that no shrinkage is given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_LAT_LEFT;
   path.first_p                                             = 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                                              = PT_HIGHEST_GRID_POINT_INDEX - 4 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first.y                                             = p_grid_array[path.first_p] - EPSILON;
   path.last_mat.y                                          = p_grid_array[path.last_p] + EPSILON;

   /** \action call function to check whether path has shrunk. */
   Pt_Check_Whether_Path_Has_Shrunk(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect no shrinkage. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}


/**
 * Tests the shrinkage check of path rotation module. Here a path without a direction is given. Thus false shall be returned
 * \uts{CSCSA-43968} \sdd{SF-7511} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Check_Whether_Path_Has_Shrunk__no_path_direction_given_thus_shrinkage_not_checked)
{
   /** \arrange setup path conditions so that no shrinkage is given */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;
   pt_size_change_flags.f_path_changed_size_at_lower_border = FBK_FALSE;
   pt_size_change_flags.f_path_changed_size_at_upper_border = FBK_FALSE;
   path.direction                                           = PATH_DIRECTION_NONE;

   /** \action call function to check whether path has shrunk. */
   Pt_Check_Whether_Path_Has_Shrunk(&pt_size_change_flags, &path, p_grid_array);

   /** \assert Expect no shrinkage. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}

/**
 * Tests constructor for flags to verify, that flags are initialized correctly
 * \uts{CSCSA-43969} \sdd{SF-7517} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Pt_Init_Path_Size_Change_Flags__initialize_flags)
{
   /** \arrange setup instance of structure to be initialized */
   Pt_Path_Size_Change_Flags_T pt_size_change_flags;

   /** \action call constructor. */
   Pt_Pt_Init_Path_Size_Change_Flags(&pt_size_change_flags);

   /** \assert Expect flags set to false. */
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_lower_border);
   EXPECT_FALSE(pt_size_change_flags.f_path_changed_size_at_upper_border);
}

/**
 * Tests the extrapolation of the rotation module of path tracking. Here no extrapolation shall be given, since grid components are
 * close to each other for fast abs negative case \uts{CSCSA-43970} \sdd{SF-7509} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper__grid_values_are_near_to_each_other_fast_abs_negative)
{
   /** \arrange setup point values close to each other */
   float32_T path_pt_previous_index    = 2.0f;
   float32_T path_pt_current_index     = 2.0f;
   float32_T grid_pt_previous_index    = -80.0f;
   float32_T grid_pt_current_index     = grid_pt_previous_index + 0.9f * EPSILON;
   float32_T grid_pt_to_extrapolate_to = -80.0f;

   float32_T res;
   /** \action extrapolation wrapper. */
   res = Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(path_pt_previous_index, path_pt_current_index, grid_pt_previous_index,
                                                            grid_pt_current_index, grid_pt_to_extrapolate_to);

   /** \assert Expect result to be equal to point value of current index. */
   EXPECT_FLOAT_EQ(res, path_pt_current_index);
}


/**
 * Tests the extrapolation of the rotation module of path tracking. Here no extrapolation shall be given, since grid components are
 * close to each other for fast abs positive case \uts{CSCSA-43971} \sdd{SF-7509} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper__grid_values_are_near_to_each_other_fast_abs_positive)
{
   /** \arrange setup point values close to each other */
   float32_T path_pt_previous_index    = 2.0f;
   float32_T path_pt_current_index     = 2.0f;
   float32_T grid_pt_previous_index    = -80.0f;
   float32_T grid_pt_current_index     = grid_pt_previous_index - 0.9f * EPSILON;
   float32_T grid_pt_to_extrapolate_to = -80.0f;

   float32_T res;
   /** \action extrapolation wrapper. */
   res = Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(path_pt_previous_index, path_pt_current_index, grid_pt_previous_index,
                                                            grid_pt_current_index, grid_pt_to_extrapolate_to);

   /** \assert Expect result to be equal to point value of current index. */
   EXPECT_FLOAT_EQ(res, path_pt_current_index);
}


/**
 * Tests the extrapolation of the rotation module of path tracking. Here the extrapolation shall be used.
 * \uts{CSCSA-43972} \sdd{SF-7509} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper__extrapolation_shall_be_used)
{
   /** \arrange setup point values */
   float32_T path_pt_previous_index    = 2.0f;
   float32_T path_pt_current_index     = 2.0f;
   float32_T grid_pt_previous_index    = -82.0f;
   float32_T grid_pt_current_index     = -79.0f;
   float32_T grid_pt_to_extrapolate_to = -80.0f;

   float32_T res;
   /** \action extrapolation wrapper. */
   res = Pt_Adaption_On_Original_Grid_Extrapolation_Wrapper(path_pt_previous_index, path_pt_current_index, grid_pt_previous_index,
                                                            grid_pt_current_index, grid_pt_to_extrapolate_to);

   /** \assert Expect result to be equal to point value of current index, since path is constant. */
   EXPECT_FLOAT_EQ(res, path_pt_current_index);
}


/**
 * Tests boundary rotation of path tracking rotation module. Here no rotation of boundaries shall occure, since those boundaries
 * have been created in the current cycle. \uts{CSCSA-43973} \sdd{SF-7519} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test,
       Pt_Rotate_Path_Boundary_Points__both_boundaries_were_created_in_the_current_cycle_thus_no_rotation_is_expected)
{
   /** \arrange setup already rotated boundaries */
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   float32_T original_float_val = 1.0f;
   rot_angle                    = Create_Angle(0.01f);
   ego_shift                    = Create_2d_Vector_Coordinates(1.0f, -1.0f);
   path.path_border_status      = PATH_BOTH_BORDERS_NEW;
   path.first                   = Create_2d_Vector_Coordinates(original_float_val, original_float_val);
   path.last_mat                = Create_2d_Vector_Coordinates(original_float_val, original_float_val);
   /** \action call path boundary rotation function. */
   Pt_Rotate_Path_Boundary_Points(&path, &rot_angle, &ego_shift, p_vehicle_data);

   /** \assert Expect boundaries to stay the same. */
   EXPECT_FLOAT_EQ(path.first.x, original_float_val);
   EXPECT_FLOAT_EQ(path.first.y, original_float_val);
   EXPECT_FLOAT_EQ(path.last_mat.x, original_float_val);
   EXPECT_FLOAT_EQ(path.last_mat.y, original_float_val);
}

/**
 * Tests boundary rotation of path tracking rotation module. Here the first boundary has been updated thus only the last boundary
 * is updated. \uts{CSCSA-43974} \sdd{SF-7519} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Path_Boundary_Points__rotate_upper_boundary)
{
   /** \arrange setup already rotated lower boundary. However the upper boundary needs to be rotated */
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   float32_T original_float_val = 1.0f;
   rot_angle                    = Create_Angle(0.0f);
   ego_shift                    = Create_2d_Vector_Coordinates(2.0f, -2.5f);
   path.path_border_status      = PATH_BORDER_NEW_FIRST;
   path.first                   = Create_2d_Vector_Coordinates(original_float_val, original_float_val);
   path.last_mat                = Create_2d_Vector_Coordinates(original_float_val, original_float_val);
   /** \action call path boundary rotation function. */
   Pt_Rotate_Path_Boundary_Points(&path, &rot_angle, &ego_shift, p_vehicle_data);

   /** \assert Expect first boundary to remain and upper boundary to be adapted. */
   EXPECT_FLOAT_EQ(path.first.x, original_float_val);
   EXPECT_FLOAT_EQ(path.first.y, original_float_val);
   EXPECT_FLOAT_EQ(path.last_mat.x, original_float_val - ego_shift.x);
   EXPECT_FLOAT_EQ(path.last_mat.y, original_float_val - ego_shift.y);
}

/**
 * Tests boundary rotation of path tracking rotation module. Here the upper boundary has been updated thus only the firstboundary
 * is updated. \uts{CSCSA-43975} \sdd{SF-7519} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Path_Boundary_Points__rotate_lower_boundary)
{
   /** \arrange setup already rotated upper boundary. However the lowerboundary needs to be rotated */
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   float32_T original_float_val = 1.0f;
   rot_angle                    = Create_Angle(0.0f);
   ego_shift                    = Create_2d_Vector_Coordinates(2.0f, -2.5f);
   path.path_border_status      = PATH_BORDER_NEW_LAST;
   path.first                   = Create_2d_Vector_Coordinates(original_float_val, original_float_val);
   path.last_mat                = Create_2d_Vector_Coordinates(original_float_val, original_float_val);

   /** \action call path boundary rotation function. */
   Pt_Rotate_Path_Boundary_Points(&path, &rot_angle, &ego_shift, p_vehicle_data);

   /** \assert Expect upper boundary to remain and lower boundary to be adapted. */
   EXPECT_FLOAT_EQ(path.first.x, original_float_val - ego_shift.x);
   EXPECT_FLOAT_EQ(path.first.y, original_float_val - ego_shift.y);
   EXPECT_FLOAT_EQ(path.last_mat.x, original_float_val);
   EXPECT_FLOAT_EQ(path.last_mat.y, original_float_val);
}

/**
 * Tests functionality adapting rotated path points to the grid. Here no extension has occured. Thus the adapted path only needs to
 * be adapted onto the grid. \uts{CSCSA-43976} \sdd{SF-7508} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Rotated_Paths_Points_To_Grid__adaption_onto_new_grid_check_without_any_extension)
{
   /** \arrange setup already rotated upper boundary. However the lowerboundary needs to be rotated */
   Pt_Discr_Borders_Of_Path_T temp_borders;
   Pt_Temporary_Path_T temp_path;
   temp_borders.temp_discr_first_boundary = PT_LOWEST_GRID_POINT_INDEX;
   temp_borders.temp_discr_last_boundary  = PT_HIGHEST_GRID_POINT_INDEX;
   path.first_p                           = PT_LOWEST_GRID_POINT_INDEX;
   path.last_p                            = PT_HIGHEST_GRID_POINT_INDEX;
   path.first                             = Create_2d_Vector_Coordinates(62.5f, 3.0f);
   path.last_mat                          = Create_2d_Vector_Coordinates(-62.5f, 3.0f);
   path.direction                         = PATH_DIRECTION_LONG_FORWARD;

   Pt_Init_Path_Points_Linearly(temp_path.path_point_array_transform, temp_path.grid_point_table_transform, path.first_p,
                                path.last_p, 0.1f, 7.0f, p_grid_array);

   /*Offset of both grid and path coordinate*/
   for (uint8_t idx = path.first_p; idx <= path.last_p; idx++)
   {
      temp_path.grid_point_table_transform[idx] = temp_path.grid_point_table_transform[idx] - 0.2f;
      temp_path.path_point_array_transform[idx] = temp_path.path_point_array_transform[idx] - 0.2f;
      path.path_points[idx]                     = temp_path.path_point_array_transform[idx]; /*temporary*/
   }

   /** \action call function to test. */
   Pt_Adapt_Rotated_Paths_Points_To_Grid(&path, &temp_path, &temp_borders, p_grid_array);

   /** \assert Expect that the path is adapted onto the grid and thus differs from the input path. */
   EXPECT_NE(temp_path.path_point_array_transform, path.path_points);
}

/**
 * Tests functionality adapting rotated path points to the grid. Here an extension at the lower border has occured for the path
 * previously, so that it shall be interpolated starting from the upper border down to the lower border. \uts{CSCSA-43977}
 * \sdd{SF-7508} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Rotated_Paths_Points_To_Grid__extension_at_the_lower_border_has_occured_previously)
{
   /** \arrange setup temporary pt_persistent.paths and so on so that an extension at the lower border is detected. */
   Pt_Discr_Borders_Of_Path_T temp_borders;
   Pt_Temporary_Path_T temp_path;
   temp_borders.temp_discr_first_boundary = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   temp_borders.temp_discr_last_boundary  = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first_p                           = temp_borders.temp_discr_first_boundary;
   path.last_p                            = temp_borders.temp_discr_last_boundary;
   path.first     = Create_2d_Vector_Coordinates(p_grid_array[path.first_p - PT_SINGLE_GRID_POINT_OFFSET] - EPSILON, 3.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(p_grid_array[path.last_p], 3.0f);
   path.direction = PATH_DIRECTION_LONG_FORWARD;

   Pt_Init_Path_Points_Linearly(temp_path.path_point_array_transform, temp_path.grid_point_table_transform, path.first_p,
                                path.last_p, 0.1f, 7.0f, p_grid_array);

   /*Offset of both grid and path coordinate*/
   for (uint8_t idx = path.first_p; idx <= path.last_p; idx++)
   {
      temp_path.grid_point_table_transform[idx] = temp_path.grid_point_table_transform[idx] - 0.2f;
      temp_path.path_point_array_transform[idx] = temp_path.path_point_array_transform[idx] - 0.2f;
      path.path_points[idx]                     = temp_path.path_point_array_transform[idx]; /*temporary*/
   }

   /** \action call function to test. */
   Pt_Adapt_Rotated_Paths_Points_To_Grid(&path, &temp_path, &temp_borders, p_grid_array);

   /** \assert Expect an extension at the lower border and thus adaption to be iterated from upper to lower border. */
   // EXPECT_EQ(path.path_extension, PATH_EXTENSION_LOWER_BORDER);
}

/**
 * Tests functionality adapting rotated path points to the grid. Here an extension at the upper border has occured for the path
 * previously, so that it shall be interpolated starting from the lower border up to the upper border. \uts{CSCSA-43978}
 * \sdd{SF-7508} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Adapt_Rotated_Paths_Points_To_Grid__extension_at_the_upper_border_has_occured_previously)
{
   /** \arrange setup temporary pt_persistent.paths and so on so that an extension at the upper border is detected. */
   Pt_Discr_Borders_Of_Path_T temp_borders;
   Pt_Temporary_Path_T temp_path;
   temp_borders.temp_discr_first_boundary = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   temp_borders.temp_discr_last_boundary  = PT_MID_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first_p                           = temp_borders.temp_discr_first_boundary;
   path.last_p                            = temp_borders.temp_discr_last_boundary;
   path.first                             = Create_2d_Vector_Coordinates(p_grid_array[path.first_p], 3.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(p_grid_array[path.last_p + PT_SINGLE_GRID_POINT_OFFSET] + EPSILON, 3.0f);
   path.direction = PATH_DIRECTION_LONG_FORWARD;

   Pt_Init_Path_Points_Linearly(temp_path.path_point_array_transform, temp_path.grid_point_table_transform, path.first_p,
                                path.last_p, 0.1f, 7.0f, p_grid_array);

   /*Offset of both grid and path coordinate*/
   for (uint8_t idx = path.first_p; idx <= path.last_p; idx++)
   {
      temp_path.grid_point_table_transform[idx] = temp_path.grid_point_table_transform[idx] - 0.2f;
      temp_path.path_point_array_transform[idx] = temp_path.path_point_array_transform[idx] - 0.2f;
      path.path_points[idx]                     = temp_path.path_point_array_transform[idx]; /*temporary*/
   }

   /** \action call function to test. */
   Pt_Adapt_Rotated_Paths_Points_To_Grid(&path, &temp_path, &temp_borders, p_grid_array);

   /** \assert Expect an extension at the upper border and thus adaption to be iterated from lower to upper border. */
   EXPECT_EQ(path.last_p, PT_MID_GRID_POINT_INDEX + 6 * PT_SINGLE_GRID_POINT_OFFSET);
}


/**
 * Tests the overall path rotation module. Here it is expected, that the rotation is executed and thus the input path differs from
 * the output path. \uts{CSCSA-43979} \sdd{SF-7520} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Path_Points__rotate_given_path)
{
   /** \arrange setup a rotation and a path which is qualified to be rotated. */
   float32_T angle;
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   angle     = 0.349066f;
   rot_angle = Create_Angle(angle);
   ego_shift = Create_2d_Vector_Coordinates(0.5f, 1.0f);

   path_for_temp_path_init.new_path_point_status = PATH_POINT_NEW_MATURE;
   path_for_temp_path_init.first_p               = PT_LOWEST_GRID_POINT_INDEX;
   path_for_temp_path_init.last_p                = PT_HIGHEST_GRID_POINT_INDEX;


   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
   {
      path.path_points[idx] = path_for_temp_path_init.path_points[idx];
   }

   /** \action call path point rotation function. */
   Pt_Rotate_Path_Points(&path_for_temp_path_init, &rot_angle, &ego_shift, &pt_input, p_vehicle_data);

   /** \assert Expect that a rotation has occured. */
   EXPECT_NE(path_for_temp_path_init.path_points, path.path_points);
}

/**
 * Set up an implausible path input and expect that this path is not rotated.
 * \uts{CSCSA-43980} \sdd{SF-7520} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Path_Points__last_point_lt_first_point)
{
   /** \arrange setup an implausible path input. */
   float32_T angle;
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   angle     = 0.349066f;
   rot_angle = Create_Angle(angle);
   ego_shift = Create_2d_Vector_Coordinates(0.5f, 1.0f);

   path_for_temp_path_init.new_path_point_status = PATH_POINT_NEW_MATURE;
   path_for_temp_path_init.first_p               = 2;
   path_for_temp_path_init.last_p                = 1;


   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
   {
      path.path_points[idx] = path_for_temp_path_init.path_points[idx];
   }

   /** \action call path point rotation function. */
   Pt_Rotate_Path_Points(&path_for_temp_path_init, &rot_angle, &ego_shift, &pt_input, p_vehicle_data);

   /** \assert Expect that no rotation has occured. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path_for_temp_path_init.path_points[i], path.path_points[i]);
   }
}


/**
 * Tests the functionality of path point rotation in path tracking. Here invalid data is given to the submodule, so that no
 * rotation shall occure. \uts{CSCSA-43981} \sdd{SF-7520} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Path_Points__first_point_is_not_initialized)
{
   /** \arrange setup invalid path boundaries. */
   float32_T angle;
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   angle     = 0.349066f;
   rot_angle = Create_Angle(angle);
   ego_shift = Create_2d_Vector_Coordinates(0.5f, 1.0f);

   path_for_temp_path_init.new_path_point_status = PATH_POINT_NEW_MATURE;
   path_for_temp_path_init.first_p               = PT_DEFAULT_MATCH_INDEX;
   path_for_temp_path_init.last_p                = PT_DEFAULT_MATCH_INDEX;


   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
   {
      path.path_points[idx] = path_for_temp_path_init.path_points[idx];
   }

   /** \action call path point rotation function. */
   Pt_Rotate_Path_Points(&path_for_temp_path_init, &rot_angle, &ego_shift, &pt_input, p_vehicle_data);

   /** \assert Expect no rotation occured. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path_for_temp_path_init.path_points[i], path.path_points[i]);
   }
}


/**
 * Tests the functionality of path point rotation in path tracking. Here a shift of the host is given. Thus all path points shall
 * be shifted with exception to the newly added path point at the lower boundary position. \uts{CSCSA-43982} \sdd{SF-7520}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Path_Points__point_first_new_only_transform_other_points_than_first)
{
   /** \arrange setup a path with a newly added point at the lower position and a pure shift. */
   float32_T angle;
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   float32_T exp_res[PT_NUM_GRID_POINTS]{};

   /*Set up Host properties*/
   p_vehicle_data->host_speed = 3.0f;
   p_vehicle_data->yawrate    = 0.3f;

   angle     = 0.0f;
   rot_angle = Create_Angle(angle);
   ego_shift = Create_2d_Vector_Coordinates(0.5f, 1.0f);

   /*Set up Path properties*/
   path_for_temp_path_init.new_path_point_status = PATH_POINT_NEW_FIRST;
   path_for_temp_path_init.first_p               = 6;
   path_for_temp_path_init.last_p                = 20;
   path_for_temp_path_init.direction             = PATH_DIRECTION_LAT_RIGHT;

   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
   {
      if (idx < path_for_temp_path_init.first_p || idx > path_for_temp_path_init.last_p)
      {
         path_for_temp_path_init.path_points[idx] = PT_PATH_POINTS_DEFAULT_VAL;
      }
   }

   path_for_temp_path_init.first.x = 14.5f;
   path_for_temp_path_init.first.y =
      p_grid_array[path_for_temp_path_init.first_p]
      - Fbk_Half(p_grid_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - p_grid_array[PT_LOWEST_GRID_POINT_INDEX]);
   path_for_temp_path_init.last_mat.x = 14.5f;
   path_for_temp_path_init.last_mat.y =
      p_grid_array[path_for_temp_path_init.last_p]
      + Fbk_Half(p_grid_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - p_grid_array[PT_LOWEST_GRID_POINT_INDEX]);

   /*Set up expected result*/
   exp_res[path_for_temp_path_init.first_p] = path_for_temp_path_init.path_points[path_for_temp_path_init.first_p];
   for (uint8_t idx = path_for_temp_path_init.first_p + PT_SINGLE_GRID_POINT_OFFSET; idx <= path_for_temp_path_init.last_p; idx++)
   {
      exp_res[idx] = path_for_temp_path_init.path_points[idx] - ego_shift.x;
   }

   /** \action call path point rotation function. */
   Pt_Rotate_Path_Points(&path_for_temp_path_init, &rot_angle, &ego_shift, &pt_input, p_vehicle_data);

   /** \assert Expect a pure shift applied to everything except the newly added point at the lower position. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path_for_temp_path_init.path_points[i], exp_res[i]);
   }
}

/**
 * Tests the functionality of path point rotation in path tracking. Here a shift of the host is given. Thus all path points shall
 * be shifted with exception to the newly added path point at the upper boundary position. \uts{CSCSA-43983} \sdd{SF-7520}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Path_Points__point_last_new_only_transform_other_points_than_last)
{
   /** \arrange setup a path at the upper position and a pure shift. */
   float32_T angle;
   Angle_T rot_angle;
   Vector_2d_T ego_shift;
   float32_T exp_res[PT_NUM_GRID_POINTS] = {0.0f};

   p_vehicle_data->host_speed = 3.0f;
   p_vehicle_data->yawrate    = 0.3f;

   angle     = 0.0f;
   rot_angle = Create_Angle(angle);
   ego_shift = Create_2d_Vector_Coordinates(0.5f, 1.0f);

   path_for_temp_path_init.new_path_point_status = PATH_POINT_NEW_LAST;
   path_for_temp_path_init.first_p               = 6;
   path_for_temp_path_init.last_p                = 20;
   path_for_temp_path_init.direction             = PATH_DIRECTION_LAT_RIGHT;


   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
   {
      if (idx < path_for_temp_path_init.first_p || idx > path_for_temp_path_init.last_p)
      {
         path_for_temp_path_init.path_points[idx] = PT_PATH_POINTS_DEFAULT_VAL;
      }
   }

   path_for_temp_path_init.first.x = 14.5f;
   path_for_temp_path_init.first.y =
      p_grid_array[path_for_temp_path_init.first_p]
      - Fbk_Half(p_grid_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - p_grid_array[PT_LOWEST_GRID_POINT_INDEX]);
   path_for_temp_path_init.last_mat.x = 14.5f;
   path_for_temp_path_init.last_mat.y =
      p_grid_array[path_for_temp_path_init.last_p]
      + Fbk_Half(p_grid_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - p_grid_array[PT_LOWEST_GRID_POINT_INDEX]);

   /*Setup expected result*/
   exp_res[path_for_temp_path_init.last_p] = path_for_temp_path_init.path_points[path_for_temp_path_init.last_p];
   for (uint8_t idx = path_for_temp_path_init.first_p; idx <= path_for_temp_path_init.last_p - PT_SINGLE_GRID_POINT_OFFSET; idx++)
   {
      exp_res[idx] = path_for_temp_path_init.path_points[idx] - ego_shift.x;
   }

   /** \action call path point rotation function. */
   Pt_Rotate_Path_Points(&path_for_temp_path_init, &rot_angle, &ego_shift, &pt_input, p_vehicle_data);

   /** \assert Expect a pure shift applied to everything except the newly added point at the upper position. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path_for_temp_path_init.path_points[i], exp_res[i]);
   }
}


/**
 * Tests superordinate function for path rotation. Here the boundaries of a path are rotated in a way, so that the boundaries are
 * out of the path tracking limits. Thus this path shall be reset. \uts{CSCSA-43984} \sdd{SF-7527} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Rotate_Recorded_Paths__rotate_path_out_of_path_tracking_boundaries)
{
   /** \arrange setup path boundaries near the reset boundaries. */
   p_vehicle_data->yawrate    = 0.0f;
   p_vehicle_data->host_speed = 10.0f;

   for (uint8_t idx = 0; idx < PT_NUM_GRID_POINTS; idx++)
   {
      pt_persistent.paths[0].path_points[idx] = 5.0f;
   }
   pt_persistent.paths[0].direction          = PATH_DIRECTION_LAT_LEFT;
   pt_persistent.paths[0].path_state         = PATH_STATUS_MATURE;
   pt_persistent.paths[0].path_border_status = PATH_BORDER_POINTS_MATURE;
   pt_persistent.paths[0].first              = Create_2d_Vector_Coordinates(-cals.k_pt_move_max_value, 5.0f);
   pt_persistent.paths[0].last_mat           = Create_2d_Vector_Coordinates(cals.k_pt_move_max_value, 5.0f);

   /** \action call function to test. */
   Pt_Rotate_Recorded_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Expect path to be reset. */
   EXPECT_EQ(pt_persistent.paths[0].path_state, PATH_STATUS_DEFAULT);
}

/**
 * Tests functionality of copying path points to the respective temporary path structure. Here the lower threshold shall be copied.
 * \uts{CSCSA-43985} \sdd{SF-7522} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Set_Temporary_Path_Points__extension_at_lower_border_shall_be_copied)
{
   /** \arrange setup temporary path and extension at lower border. */
   Pt_Temporary_Path_T temp_path_to_transform{};
   path.new_path_point_status     = PATH_POINT_NEW_FIRST;
   path.first_p                   = 5;
   path.path_points[path.first_p] = 1.0f;

   /** \action call function to test. */
   Pt_Set_Temporary_Path_Points(&temp_path_to_transform, &path);

   /** \assert Expect first_p to be set. */
   EXPECT_FLOAT_EQ(temp_path_to_transform.path_point_array_transform[path.first_p], path.path_points[path.first_p]);
   EXPECT_EQ(path.new_path_point_status, PATH_POINT_NEW_MATURE);
}

/**
 * Tests functionality of copying path points to the respective temporary path structure. Here the upper threshold shall be copied.
 * \uts{CSCSA-43986} \sdd{SF-7522} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Set_Temporary_Path_Points__extension_at_upper_border_shall_be_copied)
{
   /** \arrange setup temporary path and extension at upper border. */
   Pt_Temporary_Path_T temp_path_to_transform{};
   path.new_path_point_status    = PATH_POINT_NEW_LAST;
   path.last_p                   = PT_MID_GRID_POINT_INDEX;
   path.path_points[path.last_p] = 1.0f;

   /** \action call function to test. */
   Pt_Set_Temporary_Path_Points(&temp_path_to_transform, &path);

   /** \assert Expect last_p to be set. */
   EXPECT_FLOAT_EQ(temp_path_to_transform.path_point_array_transform[path.last_p], path.path_points[path.last_p]);
   EXPECT_EQ(path.new_path_point_status, PATH_POINT_NEW_MATURE);
}


/**
 * Tests functionality of copying path points to the respective temporary path structure. Here no border shall be copied.
 * \uts{CSCSA-43987} \sdd{SF-7522} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Set_Temporary_Path_Points__no_extension_given)
{
   /** \arrange setup temporary path. */
   Pt_Temporary_Path_T temp_path_to_transform{};
   path.new_path_point_status    = PATH_POINT_NEW_MATURE;
   path.last_p                   = PT_MID_GRID_POINT_INDEX;
   path.path_points[path.last_p] = 1.0f;

   /** \action call function to test. */
   Pt_Set_Temporary_Path_Points(&temp_path_to_transform, &path);

   /** \assert Expect last_p to be set. */
   EXPECT_FLOAT_EQ(temp_path_to_transform.path_point_array_transform[path.last_p], 0.0f);
}


/**
 * Tests functionality of specifying the borders in which pt_persistent.paths shall be rotated.
 * \uts{CSCSA-43988} \sdd{SF-7521} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Set_Temporary_Path_Borders__lower_border_is_new_and_shall_not_be_rotated)
{
   /** \arrange setup path point status to new first. */
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path{};
   path.new_path_point_status = PATH_POINT_NEW_FIRST;
   path.last_p                = PT_MID_GRID_POINT_INDEX;
   path.first_p               = PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call function to test. */
   Pt_Set_Temporary_Path_Borders(&discr_borders_of_path, &path);

   /** \assert Expect first_p to be excluded. */
   EXPECT_EQ(discr_borders_of_path.temp_discr_first_boundary, path.first_p + PT_SINGLE_GRID_POINT_OFFSET);
   EXPECT_EQ(discr_borders_of_path.temp_discr_last_boundary, path.last_p);
}


/**
 * Tests functionality of specifying the borders in which pt_persistent.paths shall be rotated.
 * \uts{CSCSA-43989} \sdd{SF-7521} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Set_Temporary_Path_Borders__expect_upper_boundary_to_be_reset)
{
   /** \arrange setup path point status to new last. */
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path{};
   path.new_path_point_status = PATH_POINT_NEW_LAST;
   path.last_p                = PT_LOWEST_GRID_POINT_INDEX;
   path.first_p               = PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call function to test. */
   Pt_Set_Temporary_Path_Borders(&discr_borders_of_path, &path);

   /** \assert Expect last_p to be reset. */
   EXPECT_EQ(discr_borders_of_path.temp_discr_last_boundary, PT_DEFAULT_DISCR_BORDER);
}


/**
 * Tests functionality of specifying the borders in which pt_persistent.paths shall be rotated. Here the uppder border is new and
 * shall not be rotated. \uts{CSCSA-43990} \sdd{SF-7521} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Set_Temporary_Path_Borders__upper_border_is_new_and_shall_not_be_rotated)
{
   /** \arrange setup path point status to new last. */
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path{};
   path.new_path_point_status = PATH_POINT_NEW_LAST;
   path.last_p                = PT_MID_GRID_POINT_INDEX;
   path.first_p               = PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call function to test. */
   Pt_Set_Temporary_Path_Borders(&discr_borders_of_path, &path);

   /** \assert Expect last_p to be excluded. */
   EXPECT_EQ(discr_borders_of_path.temp_discr_first_boundary, path.first_p);
   EXPECT_EQ(discr_borders_of_path.temp_discr_last_boundary, path.last_p - PT_SINGLE_GRID_POINT_OFFSET);
}

/**
 * Tests functionality of specifying the borders in which pt_persistent.paths shall be rotated. Here all borders are mature so that
 * the whole path interval can be taken. \uts{CSCSA-43991} \sdd{SF-7521} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Set_Temporary_Path_Borders__mature_borders_so_that_whole_intervals_can_be_taken)
{
   /** \arrange setup path point status to mature. */
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path{};
   path.new_path_point_status = PATH_POINT_NEW_MATURE;
   path.last_p                = PT_MID_GRID_POINT_INDEX;
   path.first_p               = PT_SINGLE_GRID_POINT_OFFSET;

   /** \action call function to test. */
   Pt_Set_Temporary_Path_Borders(&discr_borders_of_path, &path);

   /** \assert Expect borders to remain. */
   EXPECT_EQ(discr_borders_of_path.temp_discr_first_boundary, path.first_p);
   EXPECT_EQ(discr_borders_of_path.temp_discr_last_boundary, path.last_p);
}


/**
 * Tests whether path points shall be exclusively used for interpolation of rotated path points. Here parameters are set such that
 * the positive case is expected. \uts{CSCSA-43992} \sdd{SF-7614} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Shall_Points_Be_Used_For_Adaption__expect_interpolation_occurence_between_points)
{
   /** \arrange setup path rotated path and discrete borders such that interpolation between points occures. */
   boolean_T res;
   uint8_t grid_index = 5u;
   Pt_Temporary_Path_T temp_path_to_transform;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;

   discr_borders_of_path.temp_discr_first_boundary = 5u;
   discr_borders_of_path.temp_discr_last_boundary  = 6u;

   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_first_boundary] = p_grid_array[grid_index] - 0.2f;
   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_last_boundary] = p_grid_array[grid_index] + 0.2f;

   /** \action call function to test. */
   res = Pt_Shall_Points_Be_Used_For_Adaption(&temp_path_to_transform, &discr_borders_of_path, p_grid_array, grid_index);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Tests whether path points shall be exclusively used for interpolation of rotated path points. Here parameters are set such that
 * the negative case is expected. \uts{CSCSA-43993} \sdd{SF-7614} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Shall_Points_Be_Used_For_Adaption__no_lower_point_border_shall_be_found)
{
   /** \arrange setup path rotated path and discrete borders such no lower point can be found. */
   boolean_T res;
   uint8_t grid_index = 5u;
   Pt_Temporary_Path_T temp_path_to_transform;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;

   discr_borders_of_path.temp_discr_first_boundary = 5u;
   discr_borders_of_path.temp_discr_last_boundary  = 6u;

   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_first_boundary] = p_grid_array[grid_index] + 0.2f;
   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_last_boundary] = p_grid_array[grid_index] + 5.2f;

   /** \action call function to test. */
   res = Pt_Shall_Points_Be_Used_For_Adaption(&temp_path_to_transform, &discr_borders_of_path, p_grid_array, grid_index);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Tests whether path points shall be exclusively used for interpolation of rotated path points. Here parameters are set such that
 * the negative case is expected because no upper border can be found. \uts{CSCSA-43994} \sdd{SF-7614}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Shall_Points_Be_Used_For_Adaption__no_upper_point_border_shall_be_found)
{
   /** \arrange setup path rotated path and discrete borders such no upper point can be found. */
   boolean_T res;
   uint8_t grid_index = 5u;
   Pt_Temporary_Path_T temp_path_to_transform;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;

   discr_borders_of_path.temp_discr_first_boundary = 5u;
   discr_borders_of_path.temp_discr_last_boundary  = 6u;

   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_first_boundary] = p_grid_array[grid_index] - 5.2f;
   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_last_boundary] = p_grid_array[grid_index] - 0.2f;

   /** \action call function to test. */
   res = Pt_Shall_Points_Be_Used_For_Adaption(&temp_path_to_transform, &discr_borders_of_path, p_grid_array, grid_index);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Tests the border interpolation of points onto their respective grid entry. Here the lower target path border shall be used such
 * that true is expected. \uts{CSCSA-43995} \sdd{SF-7611} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Shall_Lower_Tgt_Border_Be_Used_For_Interpol__use_lower_target_border)
{
   /** \arrange Set up rotated path entities such that the lower target shall be used for adaption */
   boolean_T res;
   uint8_t grid_index = 5u;
   Pt_Temporary_Path_T temp_path_to_transform;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   Pt_Obj_Borders_Of_Path_T path_borders;

   discr_borders_of_path.temp_discr_first_boundary = 5u;
   path_borders.first_obj_pos_grid_component       = p_grid_array[grid_index] - 0.2f;
   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_first_boundary] = p_grid_array[grid_index] + 0.2f;

   /** \action call function to test. */
   res = Pt_Shall_Lower_Tgt_Border_Be_Used_For_Interpol(&temp_path_to_transform, &discr_borders_of_path, &path_borders,
                                                        p_grid_array, grid_index);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Tests the border interpolation of points onto their respective grid entry. Here an interpolation between two points shall be
 * applied, thus false is expected. \uts{CSCSA-43996} \sdd{SF-7611} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Shall_Lower_Tgt_Border_Be_Used_For_Interpol__dont_use_lower_target_border)
{
   /** \arrange Set up rotated path entities such that the lower target shall be used for adaption */
   boolean_T res;
   uint8_t grid_index = 5u;
   Pt_Temporary_Path_T temp_path_to_transform;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   Pt_Obj_Borders_Of_Path_T path_borders;

   discr_borders_of_path.temp_discr_first_boundary = 5u;
   path_borders.first_obj_pos_grid_component       = p_grid_array[grid_index] - 3.5f;
   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_first_boundary] = p_grid_array[grid_index] - 2.5f;

   /** \action call function to test. */
   res = Pt_Shall_Lower_Tgt_Border_Be_Used_For_Interpol(&temp_path_to_transform, &discr_borders_of_path, &path_borders,
                                                        p_grid_array, grid_index);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Tests the border interpolation of points onto their respective grid entry. Here the upper target path border shall be used such
 * that true is expected. \uts{CSCSA-43997} \sdd{SF-7610} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Shall_Upper_Tgt_Border_Be_Used_For_Interpol__use_upper_target_border)
{
   /** \arrange Set up rotated path entities such that the upper target shall be used for adaption */
   boolean_T res;
   uint8_t grid_index = 12u;
   Pt_Temporary_Path_T temp_path_to_transform;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   Pt_Obj_Borders_Of_Path_T path_borders;

   discr_borders_of_path.temp_discr_last_boundary = 12u;
   path_borders.last_obj_pos_grid_component       = p_grid_array[grid_index] + 0.2f;
   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_last_boundary] = p_grid_array[grid_index] - 0.2f;

   /** \action call function to test. */
   res = Pt_Shall_Upper_Tgt_Border_Be_Used_For_Interpol(&temp_path_to_transform, &discr_borders_of_path, &path_borders,
                                                        p_grid_array, grid_index);

   /** \assert Expect true. */
   EXPECT_TRUE(res);
}


/**
 * Tests the border interpolation of points onto their respective grid entry. Here an interpolation between multiple path points
 * shall occure, thus false is expected. \uts{CSCSA-43998} \sdd{SF-7610} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Shall_Upper_Tgt_Border_Be_Used_For_Interpol__dont_use_upper_target_border)
{
   /** \arrange Set up rotated path entities such that the upper target is not needed to be used in this case */
   boolean_T res;
   uint8_t grid_index = 12u;
   Pt_Temporary_Path_T temp_path_to_transform;
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path;
   Pt_Obj_Borders_Of_Path_T path_borders;

   discr_borders_of_path.temp_discr_last_boundary = 12u;
   path_borders.last_obj_pos_grid_component       = p_grid_array[grid_index] + 2.2f;
   temp_path_to_transform.grid_point_table_transform[discr_borders_of_path.temp_discr_last_boundary] = p_grid_array[grid_index] + 0.2f;

   /** \action call function to test. */
   res = Pt_Shall_Upper_Tgt_Border_Be_Used_For_Interpol(&temp_path_to_transform, &discr_borders_of_path, &path_borders,
                                                        p_grid_array, grid_index);

   /** \assert Expect false. */
   EXPECT_FALSE(res);
}


/**
 * Apply a rotated path entity structure and check whether the right lower point is returned for the given grid index.
 * \uts{CSCSA-43999} \sdd{SF-7612} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Lower_Surrounding_Point__get_left_point)
{
   /** \arrange set up rotated path array and expect that */
   uint8_t res;
   uint8_t grid_index = 9u;
   Pt_Temporary_Path_T temp_path_to_transform{};
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path{};

   discr_borders_of_path.temp_discr_last_boundary  = 12u;
   discr_borders_of_path.temp_discr_first_boundary = 6u;

   for (uint8_t idx = discr_borders_of_path.temp_discr_first_boundary; idx <= discr_borders_of_path.temp_discr_last_boundary; idx++)
   {
      temp_path_to_transform.grid_point_table_transform[idx] = p_grid_array[idx] - 0.2f;
   }

   /** \action call function to test. */
   res = Pt_Get_Lower_Surrounding_Point(&temp_path_to_transform, &discr_borders_of_path, p_grid_array, grid_index);

   /** \assert Expect equality between grid index and result. */
   EXPECT_EQ(res, grid_index);
}


/**
 * Apply a rotated path entity structure and check whether the right upper point is returned for the given grid index.
 * \uts{CSCSA-44000} \sdd{SF-7613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Path_Rotation_Test, Pt_Get_Upper_Surrounding_Point__get_right_point)
{
   /** \arrange set up rotated path array and expect that */
   uint8_t res;
   uint8_t grid_index = 9u;
   Pt_Temporary_Path_T temp_path_to_transform{};
   Pt_Discr_Borders_Of_Path_T discr_borders_of_path{};

   discr_borders_of_path.temp_discr_last_boundary  = 12u;
   discr_borders_of_path.temp_discr_first_boundary = 6u;

   for (uint8_t idx = discr_borders_of_path.temp_discr_first_boundary; idx <= discr_borders_of_path.temp_discr_last_boundary; idx++)
   {
      temp_path_to_transform.grid_point_table_transform[idx] = p_grid_array[idx] - 0.2f;
   }

   /** \action call function to test. */
   res = Pt_Get_Upper_Surrounding_Point(&temp_path_to_transform, &discr_borders_of_path, p_grid_array, grid_index);

   /** \assert Expect equality between grid index plus 1 and result. */
   EXPECT_EQ(res, grid_index + 1u);
}