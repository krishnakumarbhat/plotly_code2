/**
 * @file pt_output_factory_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_output_factory.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43846}
 */

#include "pt_output_factory_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>


extern "C"
{
#include "fbk_iface_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pt_common_functions.h"
#include "pt_directions.h"
#include "pt_output_factory.c"
#include <cstring>
}


/**
 * Sets the output of PT for a given object.tracker_data. Here no path match is established for the given object.tracker_data. Thus
 * the output shall be set to its default. \uts{CSCSA-43847} \sdd{SF-7499} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Set_Best_Matching_Path_Output__No_path_is_matched_to_object)
{
   /** \arrange set up an object and some inputs for the function call. */
   Pt_Persistent_T pt_persistent{};
   Pt_Object_Mov_Direction_T obj_move_dir;
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   Pt_Path_Obj_Pair_Consumer_Info_T path_consumer_info;

   next_point_idx.next_point_idx_lat_path  = PT_LOWEST_GRID_POINT_INDEX;
   next_point_idx.next_point_idx_long_path = PT_LOWEST_GRID_POINT_INDEX;
   obj_move_dir                            = PT_OBJECT_MOV_DIR_LAT_LEFT;

   object.tracker_data.index                                            = 0;
   object.tracker_data.id                                               = 2;
   pt_persistent.best_path_obj_pairs[object.tracker_data.id].path_index = PT_DEFAULT_MATCH_INDEX;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match = 0;
   Pt_Init_Path_Points_With_Constants(&pt_persistent.paths[0], -1.0f);

   /** \action call the output setting function of path tracking */
   Pt_Set_Best_Matching_Path_Output(&path_out, &pt_persistent, &path_consumer_info, &pt_input, &object, &cals, obj_move_dir,
                                    &next_point_idx, p_vehicle_data);

   /** \assert expect default output for the given object.tracker_data. */
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle, 0);
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match, PT_DEFAULT_MATCH_INDEX);
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match_age, 0);
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].path_direction, PATH_DIRECTION_NONE);
   EXPECT_FLOAT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].range_at_host_edge, PT_HIGH_DISTANCE_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero, PT_HIGH_DISTANCE_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].length_of_trajectory, -1.0f);
   EXPECT_FLOAT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].path_heading, 0.0f);
}


/**
 * Sets the output of PT for a given object.tracker_data. A path match is established. Thus non-default values shall be set to the
 * path tracking output. \uts{CSCSA-43848} \sdd{SF-7499} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Set_Best_Matching_Path_Output__path_is_matched_to_object_and_the_complete_path_output_struct_is_filled)
{
   /** \arrange set up a valid path-object pair for the output setting function. */
   Pt_Persistent_T pt_persistent;
   Pt_Object_Mov_Direction_T obj_move_dir;
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   Pt_Path_Obj_Pair_Consumer_Info_T path_consumer_info;

   next_point_idx.next_point_idx_lat_path  = PT_LOWEST_GRID_POINT_INDEX;
   next_point_idx.next_point_idx_long_path = PT_LOWEST_GRID_POINT_INDEX;
   obj_move_dir                            = PT_OBJECT_MOV_DIR_LAT_LEFT;

   object.tracker_data.index                                            = 0;
   object.tracker_data.id                                               = 4;
   pt_persistent.best_path_obj_pairs[object.tracker_data.id].path_index = 0;
   pt_persistent.path_index_last_cycle[object.tracker_data.id]          = 0;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match = 0;
   Pt_Init_Path_Points_With_Constants(&pt_persistent.paths[0], -1.0f);
   pt_persistent.paths[0].first_p   = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p    = PT_HIGHEST_GRID_POINT_INDEX;
   pt_persistent.paths[0].direction = PATH_DIRECTION_LAT_LEFT;
   object.tracker_data.vcs_heading  = -0.5f * PI;

   p_vehicle_data->host_width = 2.0f;

   /** \action call the output setting function of path tracking */
   Pt_Set_Best_Matching_Path_Output(&path_out, &pt_persistent, &path_consumer_info, &pt_input, &object, &cals, obj_move_dir,
                                    &next_point_idx, p_vehicle_data);

   /** \assert expect non default values set for the output. */
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle, 0);
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match, 0);
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match_age, 1);
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].path_direction, PATH_DIRECTION_LAT_LEFT);
}


/**
 * Tests the path heading calculation routine. Here an object moving lateral left parallel to a path is given and the path is
 * constant. Thus -0.5 PI is expected. \uts{CSCSA-43849} \sdd{SF-7483} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Path_Heading__Object_moves_lateral_left_parallel_to_path)
{
   /** \arrange Set up an object moving lateral left parallel to a path which is constant 1.0f. */
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;
   float32_T path_point_val               = -1.0f;
   float32_T res_heading;

   Pt_Init_Path_Points_With_Constants(&path, path_point_val);

   object.tracker_data.vcs_heading                                        = -0.5f * PI;
   object.tracker_data.vcs_pos                                            = Create_2d_Vector_Coordinates(path_point_val, 20.0f);
   object.tracker_data.index                                              = 0;
   path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero = path_point_val;
   path_out.path_obj_pair_output[object.tracker_data.index].range_to_current_path_part = 0.0f;

   /** \action Call path heading calculation routine. */
   res_heading = Pt_Calc_Path_Heading(&path_out, &object.tracker_data, obj_move_dir);

   /** \assert expect heading to be half of PI */
   EXPECT_FLOAT_EQ(res_heading, -0.5f * PI);
}


/**
 * Tests the path heading calculation routine. Here a path is set up in a way, that quarter of PI is returned.
 * \uts{CSCSA-43850} \sdd{SF-7483} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Path_Heading__object_moves_long_forward_parallel_to_path_in_direction_of_right_half_plane_in_vcs)
{
   /** \arrange Set up an object moving longitudinal forward and a path such that a heading of quarter pi is reached. */
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   float32_T res_heading;

   object.tracker_data.index                                                           = 0;
   path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero              = 0.0f;
   path_out.path_obj_pair_output[object.tracker_data.index].range_to_current_path_part = 0.0f;
   Pt_Init_Path_Points_With_Constants(&path, 0.0f);
   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]     = -5.0f;
   path.path_points[PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET] = -10.0f;

   object.tracker_data.vcs_heading = 0.0f;
   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(-10.0f, -10.0f);

   /** \action Call path heading calculation routine. */
   res_heading = Pt_Calc_Path_Heading(&path_out, &object.tracker_data, obj_move_dir);

   /** \assert expect heading to be quarter of PI */
   EXPECT_NEAR(res_heading, 0.25f * PI, EPSILON);
}


/**
 * Tests the path heading calculation routine. Here a path is set up in a way, that negative quarter of PI is returned.
 * \uts{CSCSA-43851} \sdd{SF-7483} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Path_Heading__object_moves_long_forward_parallel_to_path_in_direction_of_left_half_plane_in_vcs)
{
   /** \arrange Set up an object moving longitudinal forward and a path such that a heading of negative quarter pi is reached. */
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   float32_T res_heading;

   object.tracker_data.index                                                           = 0;
   path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero              = 0.0f;
   path_out.path_obj_pair_output[object.tracker_data.index].range_to_current_path_part = 0.0f;
   Pt_Init_Path_Points_With_Constants(&path, 0.0f);
   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]     = 5.0f;
   path.path_points[PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET] = 10.0f;

   object.tracker_data.vcs_heading = 0.0f;
   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(-10.0f, 10.0f);

   /** \action Call path heading calculation routine. */
   res_heading = Pt_Calc_Path_Heading(&path_out, &object.tracker_data, obj_move_dir);

   /** \assert expect heading to be negative quarter of PI */
   EXPECT_NEAR(res_heading, -0.25f * PI, EPSILON);
}


/**
 * Tests the path heading calculation routine. Here no object movement direction is given, thus the object heading shall be
 * returned. \uts{CSCSA-43852} \sdd{SF-7483} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Path_Heading__no_object_movement_direction_is_given)
{
   /** \arrange Set up an object moving without a specified direction. */
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_NONE;
   float32_T res_heading;

   object.tracker_data.index                                              = 0;
   path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero = 0.0f;
   Pt_Init_Path_Points_With_Constants(&path, -1.0f);

   object.tracker_data.vcs_heading = 0.0f;
   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(-10.0f, 10.0f);

   /** \action Call path heading calculation routine. */
   res_heading = Pt_Calc_Path_Heading(&path_out, &object.tracker_data, obj_move_dir);

   /** \assert expect object heading to be returned. */
   EXPECT_FLOAT_EQ(res_heading, object.tracker_data.vcs_heading);
}


/**
 * Tests the path heading calculation routine. the object is moving lateral right parallel to the constant path, thus an object
 * heading of half PI is expected. \uts{CSCSA-43853} \sdd{SF-7483} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Path_Heading__object_moves_lateral_lright_parallel_to_path)
{
   /** \arrange Set up an object moving lateral right. */
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   float32_T path_point_val               = -1.0f;
   float32_T res_heading;

   object.tracker_data.index                                                           = 0;
   path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero              = path_point_val;
   path_out.path_obj_pair_output[object.tracker_data.index].range_to_current_path_part = 0.0f;
   Pt_Init_Path_Points_With_Constants(&path, path_point_val);

   object.tracker_data.vcs_heading = 0.5f * PI;
   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(path_point_val, -20.0f);

   /** \action Call path heading calculation routine. */
   res_heading = Pt_Calc_Path_Heading(&path_out, &object.tracker_data, obj_move_dir);

   /** \assert expect heading of half PI to be returned. */
   EXPECT_FLOAT_EQ(res_heading, 0.5f * PI);
}


/**
 * Tests the trajectory length calculation of path tracking. Here the object has already crossed the intersection point with the
 * coordinate axis. Thus a default value shall be returned. \uts{CSCSA-43854} \sdd{SF-7482} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Length_Of_Trajectory_To_Zero__Object_has_passed_the_intersection_point_with_coordinate_axis)
{
   /** \arrange set up an lateral right moving object which has already passed the intersection with coordinate axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   uint8_t next_point_idx;
   float32_T res_length_trajectory;

   Pt_Init_Path_Points_With_Constants(&path, -1.0f);

   obj_move_dir                = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   next_point_idx              = PT_LOWEST_GRID_POINT_INDEX;
   object.tracker_data.vcs_pos = Create_2d_Vector_Y_Normal();

   /** \action Call trajectory length calculation function. */
   res_length_trajectory =
      Pt_Calc_Length_Of_Trajectory_To_Zero(&path, &object.tracker_data, p_grid_array, obj_move_dir, next_point_idx);

   /** \assert expect default value to be set. */
   EXPECT_NEAR(res_length_trajectory, -1.0f, EPSILON);
}

/**
 * Tests the trajectory length calculation of path tracking. Here the object has not crossed the intersection with coordinate axis
 * and thus a non default value shall be set. \uts{CSCSA-43855} \sdd{SF-7482} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test,
       Pt_Calc_Length_Of_Trajectory_To_Zero__Accumulate_path_distances_to_trajectory_length_for_path_moving_aligned_to_coordinate_system)
{
   /** \arrange set up an lateral right moving object and a path for which the length shall be calculated. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   uint8_t next_point_idx;
   float32_T res_length_trajectory;
   float32_T exp_length_trajectory;

   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   path.direction = PATH_DIRECTION_LAT_RIGHT;
   obj_move_dir   = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   next_point_idx = PT_SINGLE_GRID_POINT_OFFSET;
   object.tracker_data.vcs_pos =
      Create_2d_Vector_Coordinates(-2.5f, p_grid_array[PT_SINGLE_GRID_POINT_OFFSET]
                                             - (0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]));

   /** \action Call trajectory length calculation function. */
   res_length_trajectory =
      Pt_Calc_Length_Of_Trajectory_To_Zero(&path, &object.tracker_data, p_grid_array, obj_move_dir, next_point_idx);
   exp_length_trajectory =
      Fbk_Abs_F(((float32_T) PT_MID_GRID_POINT_INDEX - (float32_T) PT_SINGLE_GRID_POINT_OFFSET + 0.5f)
                * (p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - p_grid_array[PT_MID_GRID_POINT_INDEX]));
   /** \assert expect returned length to be equal to the precalculated one. */
   EXPECT_NEAR(res_length_trajectory, exp_length_trajectory, EPSILON);
}

/**
 * Tests the trajectory length calculation of path tracking. Here the object is moving against vcs and has not crossed the
 * intersection with coordinate axis and thus a non default value shall be set. \uts{CSCSA-43856} \sdd{SF-7482}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test,
       Pt_Calc_Length_Of_Trajectory_To_Zero__Accumulate_path_distances_to_trajectory_length_for_path_moving_against_coordinate_system_corner_case_with_highest_grid_idx_as_closest_one)
{
   /** \arrange set up an lateral left moving object and a path for which the length shall be calculated. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   uint8_t next_point_idx;
   float32_T res_length_trajectory;
   float32_T exp_length_trajectory;

   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   path.direction              = PATH_DIRECTION_LAT_RIGHT;
   obj_move_dir                = PT_OBJECT_MOV_DIR_LAT_LEFT;
   next_point_idx              = PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(
      -2.5f, p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] - 0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]);

   /** \action Call trajectory length calculation function. */
   res_length_trajectory =
      Pt_Calc_Length_Of_Trajectory_To_Zero(&path, &object.tracker_data, p_grid_array, obj_move_dir, next_point_idx);
   exp_length_trajectory =
      Fbk_Abs_F((next_point_idx - PT_MID_GRID_POINT_INDEX + 0.5f)
                * (p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - p_grid_array[PT_MID_GRID_POINT_INDEX]));
   /** \assert expect returned length to be equal to the precalculated one. */
   EXPECT_NEAR(res_length_trajectory, exp_length_trajectory, EPSILON);
}


/**
 * Tests the trajectory length calculation of path tracking. Here the object is moving against vcs and has not crossed the
 * intersection with coordinate axis and thus a non default value shall be set. \uts{CSCSA-43857} \sdd{SF-7482}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test,
       Pt_Calc_Length_Of_Trajectory_To_Zero__Accumulate_path_distances_to_trajectory_length_for_path_moving_against_coordinate_system)
{
   /** \arrange set up an lateral left moving object and a path for which the length shall be calculated. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   uint8_t next_point_idx;
   float32_T res_length_trajectory;
   float32_T exp_length_trajectory;

   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   path.direction = PATH_DIRECTION_LAT_RIGHT;
   obj_move_dir   = PT_OBJECT_MOV_DIR_LAT_LEFT;
   next_point_idx = PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   object.tracker_data.vcs_pos =
      Create_2d_Vector_Coordinates(-2.5f, p_grid_array[PT_HIGHEST_GRID_POINT_INDEX]
                                             - 0.5f
                                                  * (p_grid_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                     - p_grid_array[PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]));

   /** \action Call trajectory length calculation function. */
   res_length_trajectory =
      Pt_Calc_Length_Of_Trajectory_To_Zero(&path, &object.tracker_data, p_grid_array, obj_move_dir, next_point_idx);
   exp_length_trajectory =
      Fbk_Abs_F((PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET - PT_MID_GRID_POINT_INDEX + 0.5f)
                * (p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - p_grid_array[PT_MID_GRID_POINT_INDEX]));
   /** \assert expect returned length to be equal to the precalculated one. */
   EXPECT_NEAR(res_length_trajectory, exp_length_trajectory, EPSILON);
}


/**
 * Tests the extrapolation for output calculation module in path tracking. Here the path shall not be extrapolated in case that it
 * is covering already the points. \uts{CSCSA-43858} \sdd{SF-7489} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Extrap_For_Zero_Point_Dep_Calc__Dont_Pt_Extrapolate_Path)
{
   /** \arrange set up a path which is already covering the desired range of points. */
   Pt_Path_T expected_path;
   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   memcpy(&expected_path.path_points, &path.path_points, PT_NUM_GRID_POINTS * sizeof(path.path_points[PT_LOWEST_GRID_POINT_INDEX]));

   /** \action Call extrapolation step for output. */
   Pt_Extrap_For_Zero_Point_Dep_Calc(&path, &cals, p_grid_array);

   /** \assert expected that path is not changed by extrapolation */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path.path_points[i], expected_path.path_points[i]);
   }
}


/**
 * Tests the extrapolation for output calculation module in path tracking. Here it is expected that the path shall cover the points
 * around the mid point after extrapolation. \uts{CSCSA-43859} \sdd{SF-7489} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Extrap_For_Zero_Point_Dep_Calc__Extrapolate_a_path_until_mid_grid_index_plus_one)
{
   /** \arrange set up a path which is not covering the mid points already. */
   Pt_Path_T expected_path;
   path.first_p = PT_LOWEST_GRID_POINT_INDEX;
   path.last_p  = PT_MID_GRID_POINT_INDEX;
   Pt_Init_Path_Points_With_Constants(&path, 0.0f);
   Pt_Init_Path_Points_With_Constants(&expected_path, 0.0f);
   Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(&path, path.first_p, path.last_p, -1.0f);
   Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(&expected_path, PT_LOWEST_GRID_POINT_INDEX,
                                                         PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET, -1.0f);

   /** \action Call extrapolation step for output. */
   Pt_Extrap_For_Zero_Point_Dep_Calc(&path, &cals, p_grid_array);

   /** \assert expected that path is covering mid grid point index now */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path.path_points[i], expected_path.path_points[i]);
   }
}


/**
 * Tests the correctopn of extrapolation step function. Here the path is still in build up process and thus the points are expected
 * to be removed again. \uts{CSCSA-43860} \sdd{SF-7488} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test,
       Pt_Correct_For_Extrapolation__Reset_extrapolated_path_points_for_path_which_gets_build_up_against_coordinate_system)
{
   /** \arrange set up path against vcs which was extraploated. */
   Pt_Path_T expected_path;
   path.first_p                         = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                          = PT_HIGHEST_GRID_POINT_INDEX;
   path.obj_curr_used_for_path_build.id = FBK_ZERO_UINT + 1;

   Pt_Init_Path_Points_With_Constants(&path, 0.0f);
   Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(&path, PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET,
                                                         path.first_p - PT_SINGLE_GRID_POINT_OFFSET, -1.0f);
   Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(&path, path.first_p, PT_HIGHEST_GRID_POINT_INDEX, -1.0f);
   Pt_Init_Path_Points_With_Constants(&expected_path, -1.0f);
   Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(&expected_path, PT_LOWEST_GRID_POINT_INDEX,
                                                         path.first_p - PT_SINGLE_GRID_POINT_OFFSET, 0.0f);

   /** \action Call correction step. */
   Pt_Correct_For_Extrapolation(&path);

   /** \assert Expect that the extrapolated path points are reverted. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path.path_points[i], expected_path.path_points[i]);
   }
}


/**
 * Tests the correctopn of extrapolation step function. Here the path which is aligned with vcs is still in build up process and
 * thus the points are expected to be removed again. \uts{CSCSA-43861} \sdd{SF-7488} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test,
       Pt_Correct_For_Extrapolation__Reset_extrapolated_path_points_for_path_which_gets_build_up_aligned_with_coordinate_system)
{
   /** \arrange set up path aligned with vcs which was extraploated. */
   Pt_Path_T expected_path;
   path.first_p                         = PT_LOWEST_GRID_POINT_INDEX;
   path.last_p                          = PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   path.obj_curr_used_for_path_build.id = FBK_ZERO_UINT + 1;

   Pt_Init_Path_Points_With_Constants(&path, 0.0f);
   Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(&path, PT_LOWEST_GRID_POINT_INDEX, path.last_p, -1.0f);
   Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(&path, path.last_p + PT_SINGLE_GRID_POINT_OFFSET,
                                                         PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET, -1.0f);
   Pt_Init_Path_Points_With_Constants(&expected_path, -1.0f);
   Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(&expected_path, path.last_p + PT_SINGLE_GRID_POINT_OFFSET,
                                                         PT_HIGHEST_GRID_POINT_INDEX, 0.0f);

   /** \action Call correction step. */
   Pt_Correct_For_Extrapolation(&path);

   /** \assert Expect that the extrapolated path points are reverted. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path.path_points[i], expected_path.path_points[i]);
   }
}


/**
 * Tests the correctopn of extrapolation step function. Here the path is already finished and thus no correction step is expected.
 * \uts{CSCSA-43862} \sdd{SF-7488} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Correct_For_Extrapolation__Dont_reset_path)
{
   /** \arrange set up an already finished path. */
   Pt_Path_T expected_path;
   path.first_p                         = PT_LOWEST_GRID_POINT_INDEX;
   path.last_p                          = PT_HIGHEST_GRID_POINT_INDEX;
   path.obj_curr_used_for_path_build.id = FBK_ZERO_UINT;

   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   memcpy(&expected_path.path_points, &path.path_points, PT_NUM_GRID_POINTS * sizeof(path.path_points[PT_LOWEST_GRID_POINT_INDEX]));

   /** \action Call correction step. */
   Pt_Correct_For_Extrapolation(&path);

   /** \assert Expect that the path remains unchanged. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path.path_points[i], expected_path.path_points[i]);
   }
}


/**
 * Check whether object has already passed the intersection point. Here the object has passed the host and is driving long forward.
 * \uts{CSCSA-43863} \sdd{SF-7492} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Object_Already_Passed_The_Host__obj_has_passed_host_and_drives_in_long_forward_dir)
{
   /** \arrange Set up a longitudinal forward moving object which has already passed the axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T obj_pos;
   boolean_T res;
   obj_move_dir = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   obj_pos      = Create_2d_Vector_X_Normal();

   /** \action Call check whether object has passed the axis. */
   res = Pt_Has_Object_Already_Passed_The_Host(&obj_pos, obj_move_dir);

   /** \assert expect true. */
   EXPECT_TRUE(res);
}

/**
 * Check whether object has already passed the intersection point. Here the object has not passed the host and is driving long
 * forward. \uts{CSCSA-43864} \sdd{SF-7492} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Object_Already_Passed_The_Host__obj_has_not_passed_host_and_drives_in_long_forward_dir)
{
   /** \arrange Set up a longitudinal forward moving object which has not already passed the axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T obj_pos;
   boolean_T res;
   obj_move_dir = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   obj_pos      = Create_2d_Vector_X_Normal();
   obj_pos      = Vector_2d_Alg_Multiply_Scalar(&obj_pos, -1.0f);

   /** \action Call check whether object has passed the axis. */
   res = Pt_Has_Object_Already_Passed_The_Host(&obj_pos, obj_move_dir);

   /** \assert expect false. */
   EXPECT_FALSE(res);
}

/**
 * Check whether object has already passed the intersection point. Here the object has passed the host and is driving longitudinal
 * backward. \uts{CSCSA-43865} \sdd{SF-7492} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Object_Already_Passed_The_Host__obj_has_passed_host_and_drives_in_long_backward_dir)
{
   /** \arrange Set up a longitudinal backward moving object which has already passed the axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T obj_pos;
   boolean_T res;
   obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;
   obj_pos      = Create_2d_Vector_X_Normal();
   obj_pos      = Vector_2d_Alg_Multiply_Scalar(&obj_pos, -1.0f);

   /** \action Call check whether object has passed the axis. */
   res = Pt_Has_Object_Already_Passed_The_Host(&obj_pos, obj_move_dir);

   /** \assert expect true. */
   EXPECT_TRUE(res);
}

/**
 * Check whether object has already passed the intersection point. Here the object has not passed the host and is driving
 * longitudinal backward. \uts{CSCSA-43866} \sdd{SF-7492} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Object_Already_Passed_The_Host__obj_has_not_passed_host_and_drives_in_long_backward_dir)
{
   /** \arrange Set up a longitudinal backward moving object which has not already passed the axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T obj_pos;
   boolean_T res;
   obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;
   obj_pos      = Create_2d_Vector_X_Normal();

   /** \action Call check whether object has passed the axis. */
   res = Pt_Has_Object_Already_Passed_The_Host(&obj_pos, obj_move_dir);

   /** \assert expect false. */
   EXPECT_FALSE(res);
}


/**
 * Check whether object has already passed the intersection point. Here the object has passed the host and is driving lateral left.
 * \uts{CSCSA-43867} \sdd{SF-7492} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Object_Already_Passed_The_Host__obj_has_passed_host_and_drives_in_lat_left_dir)
{
   /** \arrange Set up a lateral left moving object which has already passed the axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T obj_pos;
   boolean_T res;
   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;
   obj_pos      = Create_2d_Vector_Y_Normal();
   obj_pos      = Vector_2d_Alg_Multiply_Scalar(&obj_pos, -1.0f);

   /** \action Call check whether object has passed the axis. */
   res = Pt_Has_Object_Already_Passed_The_Host(&obj_pos, obj_move_dir);

   /** \assert expect true. */
   EXPECT_TRUE(res);
}


/**
 * Check whether object has already passed the intersection point. Here the object has not passed the host and is driving lateral
 * left. \uts{CSCSA-43868} \sdd{SF-7492} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Object_Already_Passed_The_Host__obj_has_not_passed_host_and_drives_in_lat_left_dir)
{
   /** \arrange Set up a lateral left moving object which has not already passed the axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T obj_pos;
   boolean_T res;
   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;
   obj_pos      = Create_2d_Vector_Y_Normal();

   /** \action Call check whether object has passed the axis. */
   res = Pt_Has_Object_Already_Passed_The_Host(&obj_pos, obj_move_dir);

   /** \assert expect false. */
   EXPECT_FALSE(res);
}

/**
 * Check whether object has already passed the intersection point. Here the object has passed the host and is driving lateral
 * right. \uts{CSCSA-43869} \sdd{SF-7492} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Object_Already_Passed_The_Host__obj_has_passed_host_and_drives_in_lat_right_dir)
{
   /** \arrange Set up a lateral right moving object which has already passed the axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T obj_pos;
   boolean_T res;
   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   obj_pos      = Create_2d_Vector_Y_Normal();

   /** \action Call check whether object has passed the axis. */
   res = Pt_Has_Object_Already_Passed_The_Host(&obj_pos, obj_move_dir);

   /** \assert expect true. */
   EXPECT_TRUE(res);
}

/**
 * Check whether object has already passed the intersection point. Here the object has not passed the host and is driving lateral
 * right. \uts{CSCSA-43870} \sdd{SF-7492} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Object_Already_Passed_The_Host__obj_has_not_passed_host_and_drives_in_lat_right_dir)
{
   /** \arrange Set up a lateral right moving object which has not already passed the axis. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T obj_pos;
   boolean_T res;
   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   obj_pos      = Create_2d_Vector_Y_Normal();
   obj_pos      = Vector_2d_Alg_Multiply_Scalar(&obj_pos, -1.0f);

   /** \action Call check whether object has passed the axis. */
   res = Pt_Has_Object_Already_Passed_The_Host(&obj_pos, obj_move_dir);

   /** \assert expect false. */
   EXPECT_FALSE(res);
}


/**
 * Check whether the range to the path at a given point is calculated correctly. Here a constant path is given.
 * \uts{CSCSA-43871} \sdd{SF-7485} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Range_To_Path_At_Input_Pt__calculate_range_for_constant_path)
{
   /** \arrange Set up a constant path. */
   float32_T point_in;
   float32_T path_init_val;
   float32_T range_to_path_res;

   path_init_val = -1.0f;
   point_in      = -6.0f;

   Pt_Init_Path_Points_With_Constants(&path, path_init_val);

   /** \action Call range to path calculation */
   range_to_path_res = Pt_Calc_Range_To_Path_At_Input_Pt(&path, p_grid_array, &point_in);

   /** \assert expect initialized value to be the returned value */
   EXPECT_FLOAT_EQ(range_to_path_res, path_init_val);
}

#ifndef NDEBUG
/**
 * Check whether the range to the path at a given point is calculated correctly. Here an assert is given, since lower path point is
 * exceeded. \uts{CSCSA-43872} \sdd{SF-7485} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Range_To_Path_At_Input_Pt__calculate_range_for_constant_path_lowest_path_point_is_exceeded)
{
   /** \arrange set up point lower than lower boundary. */
   float32_T point_in;
   float32_T path_init_val;

   path_init_val = -1.0f;
   point_in      = p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET];

   Pt_Init_Path_Points_With_Constants(&path, path_init_val);

   ASSERT_DEATH(
      {
         /** \action Call range to path calculation */
         Pt_Calc_Range_To_Path_At_Input_Pt(&path, p_grid_array, &point_in);
         /** \assert expect assert since lower border is exceeded. */
      },
      ".*");
}

/**
 * Check whether the range to the path at a given point is calculated correctly. Here an assert is given, since upper path point is
 * exceeded. \uts{CSCSA-43873} \sdd{SF-7485} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Range_To_Path_At_Input_Pt__calculate_range_for_constant_path_upper_path_point_is_exceeded)
{
   /** \arrange set up point greater than upper boundary. */
   float32_T point_in;
   float32_T path_init_val;

   path_init_val = -1.0f;
   point_in      = p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET];

   Pt_Init_Path_Points_With_Constants(&path, path_init_val);

   ASSERT_DEATH(
      {
         /** \action Call range to path calculation */
         Pt_Calc_Range_To_Path_At_Input_Pt(&path, p_grid_array, &point_in);
         /** \assert expect assert since upper border is exceeded. */
      },
      ".*");
}
#endif

/**
 * Check whether the range to the path at a given point is calculated correctly. Here the left side of the host is used for
 * calculation. \uts{CSCSA-43874} \sdd{SF-7486} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Range_To_Path_Wrapper__calculate_range_at_left_side_of_host)
{
   /** \arrange use the left side for calculation. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   float32_T expected_range = -5.0f;
   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   path.direction = PATH_DIRECTION_LAT_LEFT;
   obj_move_dir   = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   path.path_points[PT_MID_GRID_POINT_INDEX]                               = expected_range;
   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] = expected_range;

   p_vehicle_data->host_width  = 2.5f;
   p_vehicle_data->host_length = 6.0f;

   /** \action Call wrapper for distance to path calculation. */
   path_out.path_obj_pair_output[0].range_at_host_edge =
      Pt_Calc_Range_To_Path_Wrapper(&path, &obj_move_dir, &pt_input, p_vehicle_data);

   /** \assert Expect that the range is equal to the pre determined one */
   EXPECT_NEAR(path_out.path_obj_pair_output[0].range_at_host_edge, expected_range, EPSILON);
}


/**
 * Check whether the range to the path at a given point is calculated correctly. Here the right side of the host is used for
 * calculation. \uts{CSCSA-43875} \sdd{SF-7486} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Range_To_Path_Wrapper__calculate_range_at_right_side_of_host)
{
   /** \arrange use the right side for calculation. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   float32_T expected_range = -5.0f;
   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   path.direction = PATH_DIRECTION_LAT_LEFT;
   obj_move_dir   = PT_OBJECT_MOV_DIR_LAT_LEFT;

   path.path_points[PT_MID_GRID_POINT_INDEX]                               = expected_range;
   path.path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = expected_range;

   p_vehicle_data->host_width  = 2.5f;
   p_vehicle_data->host_length = 6.0f;

   /** \action Call wrapper for distance to path calculation. */
   path_out.path_obj_pair_output[0].range_at_host_edge =
      Pt_Calc_Range_To_Path_Wrapper(&path, &obj_move_dir, &pt_input, p_vehicle_data);

   /** \assert Expect that the range is equal to the pre determined one */
   EXPECT_NEAR(path_out.path_obj_pair_output[0].range_at_host_edge, expected_range, EPSILON);
}

/**
 * Check whether the range to the path at a given point is calculated correctly. Here the rear of the host is used for calculation.
 * \uts{CSCSA-43876} \sdd{SF-7486} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Range_To_Path_Wrapper__calculate_range_for_rear_of_host)
{
   /** \arrange use the rear of host for calculation. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   float32_T expected_range = -5.0f;
   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   path.direction = PATH_DIRECTION_LONG_BACKWARD;
   obj_move_dir   = PT_OBJECT_MOV_DIR_LONG_FORWARD;

   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]     = expected_range;
   path.path_points[PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET] = expected_range;
   path.path_points[PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET] = expected_range;

   p_vehicle_data->host_width  = 2.5f;
   p_vehicle_data->host_length = 6.0f;

   /** \action Call wrapper for distance to path calculation. */
   path_out.path_obj_pair_output[0].range_at_host_edge =
      Pt_Calc_Range_To_Path_Wrapper(&path, &obj_move_dir, &pt_input, p_vehicle_data);

   /** \assert Expect that the range is equal to the pre determined one */
   EXPECT_NEAR(path_out.path_obj_pair_output[0].range_at_host_edge, expected_range, EPSILON);
}


/**
 * Check whether the range at zero is calculated correctly. Here the object moves longitudinal and has not passed the host.
 * \uts{CSCSA-43877} \sdd{SF-7487} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Vector_Obj_To_Range_At_Zero__Obj_moves_longitudinally_and_has_not_passed_the_host)
{
   /** \arrange Set up a longitudinal moving object.tracker_data. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T res_vec;
   Vector_2d_T origin_to_range_zero_vec;
   Vector_2d_T expected_vector;

   obj_move_dir                = PT_OBJECT_MOV_DIR_LONG_BACKWARD;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(6.0f, 5.0f);
   object.tracker_data.index   = FBK_ZERO_UINT;

   path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero              = FBK_ONE_F;
   path_out.path_obj_pair_output[object.tracker_data.index].range_to_current_path_part = FBK_ZERO_F;
   origin_to_range_zero_vec =
      Create_2d_Vector_Coordinates(0.0f, path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero);
   expected_vector = Vector_2d_Alg_Diff(&origin_to_range_zero_vec, &object.tracker_data.vcs_pos);
   /** \action Call range at zero calculation routine. */
   Pt_Calc_Vector_Obj_To_Range_At_Zero(&res_vec, &path_out, &object.tracker_data, obj_move_dir);

   /** \assert Expect vectors to be equal. */
   EXPECT_FLOAT_EQ(res_vec.x, expected_vector.x);
   EXPECT_FLOAT_EQ(res_vec.y, expected_vector.y);
}

/**
 * Check whether the range at zero is calculated correctly. Here the object moves lateral and has not passed the host.
 * \uts{CSCSA-43878} \sdd{SF-7487} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Vector_Obj_To_Range_At_Zero__Obj_moves_lateral_and_has_not_passed_the_host)
{
   /** \arrange Set up a lateral moving object which has not passed the host. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T res_vec;
   Vector_2d_T origin_to_range_zero_vec;
   Vector_2d_T expected_vec;

   obj_move_dir                = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(5.0f, 5.0f);
   object.tracker_data.index   = FBK_ZERO_UINT;

   path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero              = FBK_ONE_F;
   path_out.path_obj_pair_output[object.tracker_data.index].range_to_current_path_part = FBK_ZERO_F;

   origin_to_range_zero_vec =
      Create_2d_Vector_Coordinates(path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero, 0.0f);
   expected_vec = Vector_2d_Alg_Diff(&origin_to_range_zero_vec, &object.tracker_data.vcs_pos);
   /** \action Call range at zero calculation routine. */
   Pt_Calc_Vector_Obj_To_Range_At_Zero(&res_vec, &path_out, &object.tracker_data, obj_move_dir);

   /** \assert Expect vectors to be equal. */
   EXPECT_FLOAT_EQ(res_vec.x, expected_vec.x);
   EXPECT_FLOAT_EQ(res_vec.y, expected_vec.y);
}


/**
 * Check whether the range at zero is calculated correctly. Here the object moves lateral and has passed the host.
 * \uts{CSCSA-43879} \sdd{SF-7487} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Vector_Obj_To_Range_At_Zero__Obj_moves_lateral_and_has_passed_the_host)
{
   /** \arrange Set up a lateral moving object which has passed the host. */
   Pt_Object_Mov_Direction_T obj_move_dir;
   Vector_2d_T res_vec;
   Vector_2d_T origin_to_range_zero_vec;
   Vector_2d_T expected_res_vec;

   obj_move_dir                = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(5.0f, -5.0f);
   object.tracker_data.index   = FBK_ZERO_UINT;

   path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero              = FBK_ONE_F;
   path_out.path_obj_pair_output[object.tracker_data.index].range_to_current_path_part = FBK_ZERO_F;

   origin_to_range_zero_vec =
      Create_2d_Vector_Coordinates(path_out.path_obj_pair_output[object.tracker_data.index].range_at_zero, 0.0f);
   expected_res_vec = Vector_2d_Alg_Diff(&origin_to_range_zero_vec, &object.tracker_data.vcs_pos);
   expected_res_vec = Vector_2d_Alg_Multiply_Scalar(&expected_res_vec, -1.0f);

   /** \action Call range at zero calculation routine. */
   Pt_Calc_Vector_Obj_To_Range_At_Zero(&res_vec, &path_out, &object.tracker_data, obj_move_dir);

   /** \assert Expect vectors to be equal. */
   EXPECT_FLOAT_EQ(res_vec.x, expected_res_vec.x);
   EXPECT_FLOAT_EQ(res_vec.y, expected_res_vec.y);
}

/**
 * Tests the distance to the first path point calculation when the object moves parallel to the path. Both headings, the local path
 * heading and object heading are identical. In this corner case the lateral distance between the next path point and object
 * centrum position shall be identical to the lateral distance between orthgonally projected point and the path point which shall
 * be passed next. \uts{CSCSA-43880} \sdd{SF-7481} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Dist_To_First_Path_Point__orthogonal_projection_without_heading_difference_to_path_segment)
{
   /** \arrange Set up object moving parallel to a path with the same heading as the path segment. */
   Fbk_Point_Pair_T grid_point_interval;
   float32_T res_distance;
   Vector_2d_T vec_next, vec_passed;


   grid_point_interval.next   = PT_SINGLE_GRID_POINT_OFFSET;
   grid_point_interval.passed = 2 * PT_SINGLE_GRID_POINT_OFFSET;

   Pt_Init_Path_Points_With_Constants(&(path), -1.0f);
   path.direction = PATH_DIRECTION_LAT_LEFT;

   vec_next   = Create_2d_Vector_Coordinates(path.path_points[grid_point_interval.next], p_grid_array[grid_point_interval.next]);
   vec_passed = Create_2d_Vector_Coordinates(path.path_points[grid_point_interval.passed], p_grid_array[grid_point_interval.passed]);

   object.tracker_data.vcs_pos = Vector_2d_Alg_Middle(&vec_next, &vec_passed);

   /** \action call distance to first path point calculation routine. */
   res_distance = Pt_Calc_Dist_To_First_Path_Point(&path, &object.tracker_data, p_grid_array, &grid_point_interval);

   /** \assert Expect the returned distance to be equal to the distance between object center and desired grid array point. */
   EXPECT_NEAR(res_distance, Fbk_Abs_F(object.tracker_data.vcs_pos.y - p_grid_array[grid_point_interval.next]), EPSILON);
}


/**
 * Tests the distance to the first path point calculation when the object moves below a given path. Check whether orthogonal
 * projection is done correctly. \uts{CSCSA-43881} \sdd{SF-7481} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Dist_To_First_Path_Point__Object_moves_below_path_and_gets_orthogonally_projected)
{
   /** \arrange Set up object moving below a path. */
   Fbk_Point_Pair_T grid_point_interval;
   float32_T res_distance;
   float32_T expected_res_dist = 4.5962f;

   p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]     = 5.0f;
   p_grid_array[PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET] = 10.0f;
   grid_point_interval.next                                                = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   grid_point_interval.passed = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;

   Pt_Init_Path_Points_With_Constants(&(path), -1.0f);

   path.path_points[grid_point_interval.next]   = 5.0f;
   path.path_points[grid_point_interval.passed] = 10.0f;
   path.direction                               = PATH_DIRECTION_LAT_LEFT;
   object.tracker_data.vcs_pos                  = Create_2d_Vector_Coordinates(7.5f, 9.0f);

   /** \action call distance to first path point calculation routine. */
   res_distance = Pt_Calc_Dist_To_First_Path_Point(&path, &object.tracker_data, p_grid_array, &grid_point_interval);

   /** \assert Expect the returned distance to be equal to the pre calculated one. */
   EXPECT_NEAR(res_distance, expected_res_dist, EPSILON);
}


/**
 * Tests the distance to the first path point calculation when the object moves above a given path. Check whether orthogonal
 * projection is done correctly. \uts{CSCSA-43882} \sdd{SF-7481} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Dist_To_First_Path_Point__Object_moves_above_path_and_gets_orthogonally_projected)
{
   /** \arrange set up a lateral path and a object which moves above that path */
   Fbk_Point_Pair_T grid_point_interval;
   float32_T res_distance;
   float32_T expected_res_dist = 6.0104f;

   p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]     = 5.0f;
   p_grid_array[PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET] = 10.0f;
   grid_point_interval.next                                                = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   grid_point_interval.passed = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;

   Pt_Init_Path_Points_With_Constants(&(path), -1.0f);

   path.path_points[grid_point_interval.next]   = 5.0f;
   path.path_points[grid_point_interval.passed] = 10.0f;
   path.direction                               = PATH_DIRECTION_LAT_LEFT;
   object.tracker_data.vcs_pos                  = Create_2d_Vector_Coordinates(7.5f, 11.0f);

   /** \action call distance to first path point calculation routine. */
   res_distance = Pt_Calc_Dist_To_First_Path_Point(&path, &object.tracker_data, p_grid_array, &grid_point_interval);

   /** \assert Expect the returned distance to be equal to the pre calculated one. */
   EXPECT_NEAR(res_distance, expected_res_dist, EPSILON);
}

/**
 * Tests the mapping of nex and passed vectors. Here the input path is lateral. Check whether the grid array is set to the y
 * coordinate and the path points to the c coordinate. \uts{CSCSA-43883} \sdd{SF-7494} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Map_Path_Entities_To_Vectors__Lateral_path_is_mapped_to_vector)
{
   /** \arrange Set up a lateral path. */
   Vector_2d_T vector_next;
   Vector_2d_T vector_passed;
   Fbk_Point_Pair_T grid_point_interval;
   Vector_2d_T exp_vec_next;
   Vector_2d_T exp_vec_passed;

   Pt_Init_Path_Points_With_Constants(&(path), -1.0f);
   path.direction             = PATH_DIRECTION_LAT_LEFT;
   grid_point_interval.next   = PT_SINGLE_GRID_POINT_OFFSET;
   grid_point_interval.passed = 2 * PT_SINGLE_GRID_POINT_OFFSET;
   exp_vec_next = Create_2d_Vector_Coordinates(path.path_points[grid_point_interval.next], p_grid_array[grid_point_interval.next]);
   exp_vec_passed =
      Create_2d_Vector_Coordinates(path.path_points[grid_point_interval.passed], p_grid_array[grid_point_interval.passed]);

   /** \action Call mapping function. */
   Pt_Map_Path_Entities_To_Vectors(&vector_next, &vector_passed, &path, p_grid_array, &grid_point_interval);

   /** \assert expect that the mapping is done correctly for lateral pt_persistent.paths. */
   EXPECT_FLOAT_EQ(vector_next.x, exp_vec_next.x);
   EXPECT_FLOAT_EQ(vector_next.y, exp_vec_next.y);
   EXPECT_FLOAT_EQ(vector_passed.x, exp_vec_passed.x);
   EXPECT_FLOAT_EQ(vector_passed.y, exp_vec_passed.y);
}

/**
 * Tests the mapping of nex and passed vectors. Here the input path is longidutinal. Check whether the grid array is set to the x
 * coordinate and the path points to the y coordinate. \uts{CSCSA-43884} \sdd{SF-7494} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Map_Path_Entities_To_Vectors__Longitudinal_path_is_mapped_to_vector)
{
   /** \arrange Set up a longitudinal path and the desired indices. */
   Vector_2d_T vector_next;
   Vector_2d_T vector_passed;
   Fbk_Point_Pair_T grid_point_interval;
   Vector_2d_T exp_vec_next;
   Vector_2d_T exp_vec_passed;

   Pt_Init_Path_Points_With_Constants(&(path), -1.0f);
   path.direction             = PATH_DIRECTION_LONG_BACKWARD;
   grid_point_interval.next   = PT_SINGLE_GRID_POINT_OFFSET;
   grid_point_interval.passed = 2 * PT_SINGLE_GRID_POINT_OFFSET;
   exp_vec_next = Create_2d_Vector_Coordinates(p_grid_array[grid_point_interval.next], path.path_points[grid_point_interval.next]);
   exp_vec_passed =
      Create_2d_Vector_Coordinates(p_grid_array[grid_point_interval.passed], path.path_points[grid_point_interval.passed]);
   /** \action Call mapping function. */
   Pt_Map_Path_Entities_To_Vectors(&vector_next, &vector_passed, &path, p_grid_array, &grid_point_interval);

   /** \assert expect that the mapping is done correctly for longitudinal pt_persistent.paths. */
   EXPECT_FLOAT_EQ(vector_next.x, exp_vec_next.x);
   EXPECT_FLOAT_EQ(vector_next.y, exp_vec_next.y);
   EXPECT_FLOAT_EQ(vector_passed.x, exp_vec_passed.x);
   EXPECT_FLOAT_EQ(vector_passed.y, exp_vec_passed.y);
}


/**
 * Tests whether an object has been matched to the same path over successive cycles. HHere the path match occures firstly in this
 * cycle. Thus true is expected. \uts{CSCSA-43885} \sdd{SF-7491} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Obj_Matched_For_Succ_Cycles__Rising_flank_occures)
{
   /** \arrange set up a match in the current cycle. */
   boolean_T res;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = PT_DEFAULT_MATCH_INDEX;

   /** \action call check for same match over successive cycles. */
   res = Pt_Has_Obj_Matched_For_Succ_Cycles(&(path_out), &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Tests whether an object has been matched to the same path over successive cycles. Here the path match is kept over successive
 * cycles. \uts{CSCSA-43886} \sdd{SF-7491} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Obj_Matched_For_Succ_Cycles__Matched_to_same_path_over_successive_cycles)
{
   /** \arrange Set up a match over successive cycles. */
   boolean_T res;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = FBK_ZERO_INT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = FBK_ZERO_INT;

   /** \action call check for same match over successive cycles. */
   res = Pt_Has_Obj_Matched_For_Succ_Cycles(&(path_out), &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Tests whether an object has been matched to the same path over successive cycles. Here no path index was or is assinged. Thus
 * false is expected. \uts{CSCSA-43887} \sdd{SF-7491} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Obj_Matched_For_Succ_Cycles__Match_index_is_uninitialized)
{
   /** \arrange set up default path index for last and current cycle. */
   boolean_T res;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = PT_DEFAULT_MATCH_INDEX;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = PT_DEFAULT_MATCH_INDEX;

   /** \action call check for same match over successive cycles. */
   res = Pt_Has_Obj_Matched_For_Succ_Cycles(&(path_out), &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Tests whether an object has been matched to the same path over successive cycles. Here the the match is lost, thus false is
 * expected. \uts{CSCSA-43888} \sdd{SF-7491} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Obj_Matched_For_Succ_Cycles__Falling_flank_occures)
{
   /** \arrange set up a path match loss. */
   boolean_T res;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = PT_DEFAULT_MATCH_INDEX;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = 1;

   /** \action call check for same match over successive cycles. */
   res = Pt_Has_Obj_Matched_For_Succ_Cycles(&(path_out), &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(res);
}


/**
 * Tests whether a match assignment change has occured. Here a path match change has occured and thus true is expected.
 * \uts{CSCSA-43889} \sdd{SF-7493} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Path_Change_Occured__Path_has_changed)
{
   /** \arrange set up a path match change. */
   boolean_T res;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = 2;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = 1;

   /** \action Call path change check. */
   res = Pt_Has_Path_Change_Occured(&(path_out), &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Tests whether a match assignment change has occured. Here a path match got lost and thus true is expected.
 * \uts{CSCSA-43890} \sdd{SF-7493} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Path_Change_Occured__Path_match_gets_lost)
{
   /** \arrange set up a path match loss. */
   boolean_T res;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = PT_DEFAULT_MATCH_INDEX;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = 1;

   /** \action Call path change check. */
   res = Pt_Has_Path_Change_Occured(&(path_out), &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Tests whether a match assignment change has occured. Here no path match change has occured and thus false is expected.
 * \uts{CSCSA-43891} \sdd{SF-7493} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Path_Change_Occured__Path_was_matched_over_successive_cycles)
{
   /** \arrange Set up a match which is kept over successive cycles. */
   boolean_T res;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = 1;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = 1;

   /** \action Call path change check. */
   res = Pt_Has_Path_Change_Occured(&(path_out), &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Tests the update of the match age. Here the match is kept in this cycle and thus it is expected that the age is incremented.
 * \uts{CSCSA-43892} \sdd{SF-7496} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Update_Track_Match_Age__Age_gets_incremented_since_path_is_kept_over_successive_cycles)
{
   /** \arrange set up a match which is kept. */
   uint8_t res                                                                     = 1;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_age        = 0;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = 1;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = 1;

   /** \action call the age match update routine. */
   Pt_Update_Track_Match_Age(&(path_out), &object.tracker_data);

   /** \assert expect that the age is increased. */
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match_age, res);
}

/**
 * Tests the update of the match age. Here the match has changed for the object and thus the age shall be reset.
 * \uts{CSCSA-43893} \sdd{SF-7496} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Update_Track_Match_Age__Age_gets_reset_since_path_match_changed)
{
   /** \arrange set up a scenario where the match has changed. */
   uint8_t res                                                                     = 0;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_age        = 1;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = PT_DEFAULT_MATCH_INDEX;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = 1;

   /** \action call the age match update routine. */
   Pt_Update_Track_Match_Age(&(path_out), &object.tracker_data);

   /** \assert expect that the age reset. */
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match_age, res);
}

/**
 * Tests the update of the match age. Here the previous track match age shall be kept.
 * \uts{CSCSA-43894} \sdd{SF-7496} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Update_Track_Match_Age__Keep_previously_set_track_match_age)
{
   /** \arrange set up uninitialized path index and an age which shall be kept. */
   uint8_t res                                                                     = 5;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_age        = res;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = PT_DEFAULT_MATCH_INDEX;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = PT_DEFAULT_MATCH_INDEX;

   /** \action call the age match update routine. */
   Pt_Update_Track_Match_Age(&(path_out), &object.tracker_data);

   /** \assert expect that the initialized age is kept. */
   EXPECT_EQ(path_out.path_obj_pair_output[object.tracker_data.index].track_match_age, res);
}

/**
 * Tests the reset of extrapolated path segment. Here a given path shall be transfered to its original state.
 * \uts{CSCSA-43895} \sdd{SF-7495} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Reset_Extrapolated_Points__resets_correct_path_point)
{
   /** \arrange Set up a path which got extrapolated- */
   Pt_Path_T test_path;
   uint8_t start_idx = 6;
   uint8_t end_idx   = 7;
   Pt_Init_Path_Points_With_Constants(&(test_path), -1.0f);

   /** \action Call reset of extrapolated points */
   Pt_Reset_Extrapolated_Points(&test_path, start_idx, end_idx);

   /** \assert Expect that the path is transfered to its original state. */
   EXPECT_NE(test_path.path_points[start_idx - PT_SINGLE_GRID_POINT_OFFSET], PT_PATH_POINTS_DEFAULT_VAL);
   EXPECT_EQ(test_path.path_points[start_idx], PT_PATH_POINTS_DEFAULT_VAL);
   EXPECT_EQ(test_path.path_points[end_idx], PT_PATH_POINTS_DEFAULT_VAL);
   EXPECT_NE(test_path.path_points[end_idx + PT_SINGLE_GRID_POINT_OFFSET], PT_PATH_POINTS_DEFAULT_VAL);
}


#ifndef NDEBUG
/**
 * Tests the reset of extrapolated path segment. Here the upper boundary is exceeded and thus an assert is expected.
 * \uts{CSCSA-43896} \sdd{SF-7495} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Reset_Extrapolated_Points__Assert_test_upper_boundary)
{
   /** \arrange Set up boundary which is exceeding the upper boundary. */
   Pt_Path_T test_path;
   uint8_t start_idx = PT_INVALID_GRID_POINT_INDEX;
   uint8_t end_idx   = PT_HIGHEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Init_Path_Points_With_Constants(&(test_path), -1.0f);

   ASSERT_DEATH(
      {
         /** \action Call reset of extrapolated points */
         Pt_Reset_Extrapolated_Points(&test_path, start_idx, end_idx);
         /** \assert Expect assert due to exceeding upper boundary */
      },
      ".*for_loop_end <= PT_HIGHEST_GRID_POINT_INDEX.*");
}
#endif

/**
 * Tests whether the range of an object to the path is calculated correctly. Here a longitudinal path is given and the object is
 * moving aligned with vcs on the right side of the path. \uts{CSCSA-43897} \sdd{SF-7484} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Range_to_Path_At_Curr_Pos__Check_range_to_path_for_long_paths)
{
   /** \arrange Set up path and object in a way, such that a negative distance of 1.0f is returned */
   Pt_Object_Mov_Direction_T obj_move_dir;
   uint8_t next_point_idx;
   float32_T expected_res = -1.0f;

   next_point_idx              = PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(-9.0f, -6.0f);
   path.direction              = PATH_DIRECTION_LONG_FORWARD;
   Pt_Init_Path_Points_With_Constants(&(path), -5.0f);
   obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   /** \action Call range to path calculation */
   path_out.path_obj_pair_output[0].range_to_current_path_part =
      Pt_Calc_Range_to_Path_At_Curr_Pos(&path, &obj_move_dir, &object.tracker_data, p_grid_array, next_point_idx);

   /** \assert Expect a distance of -1.0f */
   EXPECT_FLOAT_EQ(path_out.path_obj_pair_output[0].range_to_current_path_part, expected_res);
}

/**
 * Tests whether the range of an object to the path is calculated correctly. Here a lateral path is given and the object is moving
 * aligned with vcs on the left side of the path. \uts{CSCSA-43898} \sdd{SF-7484} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Calc_Range_to_Path_At_Curr_Pos__Check_range_to_path_for_lat_paths)
{
   /** \arrange Set up path and object in a way, such that a positive distance of 1.0f is returned */
   Pt_Object_Mov_Direction_T obj_move_dir;
   uint8_t next_point_idx;
   float32_T expected_res = 1.0f;

   next_point_idx              = PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(-6.0f, -9.0f);
   path.direction              = PATH_DIRECTION_LAT_RIGHT;
   Pt_Init_Path_Points_With_Constants(&(path), -5.0f);
   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action Call range to path calculation */
   path_out.path_obj_pair_output[0].range_to_current_path_part =
      Pt_Calc_Range_to_Path_At_Curr_Pos(&path, &obj_move_dir, &object.tracker_data, p_grid_array, next_point_idx);

   /** \assert Expect a distance of 1.0f */
   EXPECT_FLOAT_EQ(path_out.path_obj_pair_output[0].range_to_current_path_part, expected_res);
}

/**
 * Tests whether the range of an object to the path is calculated correctly. Here a lateral path is given and the object is moving
 * against vcs on the right side of the path. \uts{CSCSA-43899} \sdd{SF-7484} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test,
       Pt_Calc_Range_to_Path_At_Curr_Pos__Check_range_to_path_for_lat_paths_with_obj_moving_against_vcs_on_left_side_of_path)
{
   /** \arrange Set up path and object in a way, such that a negative distance of 1.0f is returned */
   Pt_Object_Mov_Direction_T obj_move_dir;
   uint8_t next_point_idx;
   float32_T expected_res = -1.0f;

   next_point_idx              = PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(-4.0f, -9.0f);
   path.direction              = PATH_DIRECTION_LAT_RIGHT;
   Pt_Init_Path_Points_With_Constants(&(path), -5.0f);
   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;

   /** \action Call range to path calculation */
   path_out.path_obj_pair_output[0].range_to_current_path_part =
      Pt_Calc_Range_to_Path_At_Curr_Pos(&path, &obj_move_dir, &object.tracker_data, p_grid_array, next_point_idx);

   /** \assert Expect a distance of -1.0f */
   EXPECT_FLOAT_EQ(path_out.path_obj_pair_output[0].range_to_current_path_part, expected_res);
}


/**
 * Tests whether the range of an object to the path is calculated correctly. Here a longitudinal path is given and the object is
 * moving aligned with vcs on the left side of the path. \uts{CSCSA-43900} \sdd{SF-7484} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test,
       Pt_Calc_Range_to_Path_At_Curr_Pos__Check_range_to_path_for_long_paths_with_obj_moving_aligned_with_vcs_on_left_side_of_path)
{
   /** \arrange Set up path and object in a way, such that a positive distance of 1.0f is returned */
   Pt_Object_Mov_Direction_T obj_move_dir;
   uint8_t next_point_idx;
   float32_T expected_res = 1.0f;

   next_point_idx              = PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(-9.0f, -4.0f);
   path.direction              = PATH_DIRECTION_LONG_FORWARD;
   Pt_Init_Path_Points_With_Constants(&(path), -5.0f);
   obj_move_dir = PT_OBJECT_MOV_DIR_LONG_FORWARD;

   /** \action Call range to path calculation */
   path_out.path_obj_pair_output[0].range_to_current_path_part =
      Pt_Calc_Range_to_Path_At_Curr_Pos(&path, &obj_move_dir, &object.tracker_data, p_grid_array, next_point_idx);

   /** \assert Expect a distance of 1.0f */
   EXPECT_FLOAT_EQ(path_out.path_obj_pair_output[0].range_to_current_path_part, expected_res);
}


/**
 * Check whether the surrounding grid point index is returned correctly. Here the object is moving aligned with vcs. Thus the
 * passed index is lower than the next. \uts{CSCSA-43901} \sdd{SF-7490} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Get_Surrounding_Path_Points__obj_aligned_with_vcs)
{
   /** \arrange set up and object moving aligned with vcs. */
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   uint8_t next_point_idx                 = PT_MID_GRID_POINT_INDEX;
   Fbk_Point_Pair_T grid_point_interval;

   /** \action call surrounding grid point calculation routine. */
   Pt_Get_Surrounding_Path_Points(&grid_point_interval, &obj_move_dir, next_point_idx);

   /** \assert expect mapping to be done correctly */
   EXPECT_EQ(grid_point_interval.next, next_point_idx);
   EXPECT_EQ(grid_point_interval.passed, next_point_idx - 1);
}


/**
 * Check whether the surrounding grid point index is returned correctly. Here the object is moving against vcs. Thus the passed
 * index is greater than the next. \uts{CSCSA-43902} \sdd{SF-7490} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Get_Surrounding_Path_Points__obj_against_vcs)
{
   /** \arrange set up and object moving against vcs. */
   Pt_Object_Mov_Direction_T obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;
   uint8_t next_point_idx                 = PT_MID_GRID_POINT_INDEX;
   Fbk_Point_Pair_T grid_point_interval;

   /** \action call surrounding grid point calculation routine. */
   Pt_Get_Surrounding_Path_Points(&grid_point_interval, &obj_move_dir, next_point_idx);

   /** \assert expect mapping to be done correctly */
   EXPECT_EQ(grid_point_interval.next, next_point_idx);
   EXPECT_EQ(grid_point_interval.passed, next_point_idx + 1);
}

/**
 * Tests the correctopn of extrapolation step function. No correction step is expected due to wrong obj id.
 * \uts{CSCSA-43903} \sdd{SF-7488} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Correct_For_Extrapolation__Dont_reset_obj_id_zero)
{
   /** \arrange set up an already finished path. */
   Pt_Path_T expected_path;
   path.first_p                         = PT_HIGHEST_GRID_POINT_INDEX;
   path.last_p                          = PT_LOWEST_GRID_POINT_INDEX;
   path.obj_curr_used_for_path_build.id = FBK_ZERO_UINT;

   Pt_Init_Path_Points_With_Constants(&path, -1.0f);
   memcpy(&expected_path.path_points, &path.path_points, PT_NUM_GRID_POINTS * sizeof(path.path_points[PT_LOWEST_GRID_POINT_INDEX]));

   /** \action Call correction step. */
   Pt_Correct_For_Extrapolation(&path);

   /** \assert Expect that the path remains unchanged. */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path.path_points[i], expected_path.path_points[i]);
   }
}

/**
 * Tests whether a match assignment change has occure. False due to wrong track match
 * \uts{CSCSA-43904} \sdd{SF-7493} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Has_Path_Change_Occured__Path_has_not_changed_match_defualt)
{
   /** \arrange set up a path match change. */
   boolean_T res;
   object.tracker_data.index                                                       = FBK_ZERO_UINT;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match            = 2;
   path_out.path_obj_pair_output[object.tracker_data.index].track_match_last_cycle = PT_DEFAULT_MATCH_INDEX;

   /** \action Call path change check. */
   res = Pt_Has_Path_Change_Occured(&(path_out), &object.tracker_data);

   /** \assert expect true */
   EXPECT_FALSE(res);
}

/**
 * Tests the extrapolation for output calculation module in path tracking.
 * \uts{CSCSA-43905} \sdd{SF-7489} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Extrap_For_Zero_Point_Dep_Calc__Extrapolate_mid_zero)
{
   /** \arrange set up a path which is not covering the mid points already. */
   Pt_Path_T expected_path;
   path.first_p = PT_LOWEST_GRID_POINT_INDEX;
   path.last_p  = PT_MID_GRID_POINT_INDEX;
   Pt_Init_Path_Points_With_Constants(&path, 1.0f);
   Pt_Init_Path_Points_With_Constants(&expected_path, 1.0f);
   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]          = 0.0f;
   expected_path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] = 0.0f;

   /** \action Call extrapolation step for output. */
   Pt_Extrap_For_Zero_Point_Dep_Calc(&path, &cals, p_grid_array);

   /** \assert expected that path is covering mid grid point index now */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path.path_points[i], expected_path.path_points[i]);
   }
}

/**
 * Tests the extrapolation for output calculation module in path tracking.
 * \uts{CSCSA-43906} \sdd{SF-7489} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Output_Factory_Test, Pt_Extrap_For_Zero_Point_Dep_Calc__Extrapolate_mid_plus_is_zero)
{
   /** \arrange set up a path which is not covering the mid points already. */
   Pt_Path_T expected_path;
   path.first_p = PT_LOWEST_GRID_POINT_INDEX;
   path.last_p  = PT_MID_GRID_POINT_INDEX;
   Pt_Init_Path_Points_With_Constants(&path, 0.0f);
   Pt_Init_Path_Points_With_Constants(&expected_path, 0.0f);
   path.path_points[PT_MID_GRID_POINT_INDEX]                               = 0.0f;
   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] = 0.0f;

   /** \action Call extrapolation step for output. */
   Pt_Extrap_For_Zero_Point_Dep_Calc(&path, &cals, p_grid_array);

   /** \assert expected that path is covering mid grid point index now */
   for (uint8_t i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(path.path_points[i], expected_path.path_points[i]);
   }
}