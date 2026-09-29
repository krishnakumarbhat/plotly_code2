/**
 * @file pt_object_matching_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_object_matching.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43697}
 */

#include "pt_object_matching_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_shared_types.h"
#include "pt_common_functions.h"
#include "pt_object_matching.c"
#include "pt_reset.h"
}


/**
 * Tests superordinate function for matching. Here all objects are in status invalid and thus a default output shall be generated
 * for each of them. \uts{CSCSA-43698} \sdd{SF-7478} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Objects_To_Path_Matching__all_objects_are_in_state_invalid_thus_default_output_is_expected)
{
   /** \arrange set all object status to invalid */
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      object_data[idx].status     = PA_OBJ_STATUS_INVALID;
      object_data[idx].f_moveable = FBK_TRUE;
   }

   /** \action Call matching routine for all objects. */
   Pt_Objects_To_Path_Matching(&pt_persistent, &path_output, &cals, &pt_input, p_vehicle_data);

   /** \assert expect that every output is assigned to an uninitialized path. */
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(path_output.path_obj_pair_output[idx].track_match, PT_DEFAULT_MATCH_INDEX);
   }
}

/**
 * Tests superordinate function for matching. Here all objects are in status new and thus a default output shall be generated for
 * each of them. \uts{CSCSA-43699} \sdd{SF-7478} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Objects_To_Path_Matching__all_objects_are_in_state_new_thus_default_output_is_expected)
{
   /** \arrange set all object status to invalid */
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      object_data[idx].status                           = PA_OBJ_STATUS_NEW;
      object_data[idx].f_moveable                       = FBK_TRUE;
      path_output.path_obj_pair_output[idx].track_match = idx;
   }

   /** \action Call matching routine for all objects. */
   Pt_Objects_To_Path_Matching(&pt_persistent, &path_output, &cals, &pt_input, p_vehicle_data);

   /** \assert expect that every output is assigned to an uninitialized path. */
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(path_output.path_obj_pair_output[idx].track_match, PT_DEFAULT_MATCH_INDEX);
   }
}


/**
 * Tests module for matching a path to a given object.tracker_data. Here the object is invalid and thus no path match shall be
 * given. \uts{CSCSA-43700} \sdd{SF-7473} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Match_Object_To_Path__object_is_invalid_due_to_its_status)
{
   /** \arrange set objects status to invalid */
   object.tracker_data.status                      = PA_OBJ_STATUS_INVALID;
   path_output.path_obj_pair_output[0].track_match = PT_DEFAULT_MATCH_INDEX;

   /** \action Call matching routine for a given object.tracker_data. */
   Pt_Objects_To_Path_Matching(&pt_persistent, &path_output, &cals, &pt_input, p_vehicle_data);

   /** \assert expect that the tracked path is kept on its default value. */
   EXPECT_EQ(path_output.path_obj_pair_output[0].track_match, PT_DEFAULT_MATCH_INDEX);
}


/**
 * Tests module for matching a path to a given object.tracker_data. Here the object is valid but and the next point index is also
 * valid (invalid next point index is never reachable due to control flow). \uts{CSCSA-43701} \sdd{SF-7473}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Match_Object_To_Path__next_point_index_is_valid)
{
   /** \arrange create valid object in a range where the next point index is not valid but create a path set where only default
    * pt_persistent.paths are available. */
   object.tracker_data.status      = PA_OBJ_STATUS_MATURE;
   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(5.0f, p_grid_array[PT_LOWEST_GRID_POINT_INDEX] + 0.5f);
   object.tracker_data.vcs_heading = 0.5 * PI;
   object.tracker_data.speed       = 1.1f * cals.k_pt_find_min_speed;
   object.tracker_data.index       = 1u;
   path_output.path_obj_pair_output[0].track_match       = PT_DEFAULT_MATCH_INDEX;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_lat_posn = 90.0f;

   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_DEFAULT;
   }

   /** \action Call matching routine for a given object.tracker_data. */
   Pt_Match_Object_To_Path(&pt_persistent, &path_output, &cals, &pt_input, &object, p_vehicle_data);

   /** \assert expect that the path tracking output for the given input object consists of default values. */
   EXPECT_FLOAT_EQ(path_output.path_obj_pair_output[object.tracker_data.index].range_at_zero, PT_HIGH_DISTANCE_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(path_output.path_obj_pair_output[object.tracker_data.index].range_to_current_path_part,
                   PT_HIGH_DISTANCE_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(path_output.path_obj_pair_output[object.tracker_data.index].range_at_host_edge, PT_HIGH_DISTANCE_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(path_output.path_obj_pair_output[object.tracker_data.index].length_of_trajectory, -FBK_ONE_F);
   EXPECT_FLOAT_EQ(path_output.path_obj_pair_output[object.tracker_data.index].path_heading, FBK_ZERO_F);
   EXPECT_EQ(path_output.path_obj_pair_output[object.tracker_data.index].path_direction, PATH_DIRECTION_NONE);
}


/**
 * Tests module for finding of best matching path. Here no valid pt_persistent.paths are given so that no valid path object pair
 * can be created. \uts{CSCSA-43702} \sdd{SF-7457} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Find_Best_Matching_Path__no_valid_path_available_for_matching)
{
   /** \arrange set all pt_persistent.paths to default state */
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   next_point_idx.next_point_idx_lat_path  = PT_MID_GRID_POINT_INDEX;
   next_point_idx.next_point_idx_long_path = PT_MID_GRID_POINT_INDEX;
   uint8_t i;
   for (i = 0u; i < PT_NUMBER_OF_PATHS; i++)
   {
      pt_persistent.paths[i].path_state = PATH_STATUS_DEFAULT;
   }

   /** \action Call function for path object pair creation */
   Pt_Find_Best_Matching_Path(&pt_persistent, &path_obj_info, &path_obj_pair_consumer_info, &pt_input, &cals, obj_move_dir,
                              &next_point_idx, &object.tracker_data);

   /** \assert expect matching pairs to be empty */
   for (i = 0u; i <= PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.best_path_obj_pairs[i].confidence_factor, 0.0f);
   }
}


/**
 * Tests module for finding of best matching path. Here one valid path is given so that one valid path object pair can be created.
 * Additionally the position of the object is chosen in a way that the positive fast abs branch is hit. \uts{CSCSA-43703}
 * \sdd{SF-7457} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Find_Best_Matching_Path__valid_path_shall_be_set_to_best_matching_one_positive_fast_abs_case)
{
   /** \arrange Set up an object moving with the given path. The object is directly moving on that path such that it is expected to
    * be matched to that path. */
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   next_point_idx.next_point_idx_lat_path  = PT_MID_GRID_POINT_INDEX;
   next_point_idx.next_point_idx_long_path = PT_MID_GRID_POINT_INDEX;
   float32_T path_val                      = 1.0f;
   Vector_2d_T first_path_point = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f);
   Vector_2d_T last_path_point  = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f);

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], p_grid_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 1u, first_path_point, last_path_point, 0.0f, path_val);

   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;

   obj_move_dir                    = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(0.9f * path_val, p_grid_array[PT_MID_GRID_POINT_INDEX] - 1.0f);
   object.tracker_data.vcs_heading = 0.5f * PI;
   object.tracker_data.index       = 0;
   object.tracker_data.id          = 1;

   path_obj_pair_consumer_info.distance_to_path = 90.0f;
   /** \action Call function for path object pair creation */
   Pt_Find_Best_Matching_Path(&pt_persistent, &path_obj_info, &path_obj_pair_consumer_info, &pt_input, &cals, obj_move_dir,
                              &next_point_idx, &object.tracker_data);

   /** \assert expect path to be matched to the given object */
   EXPECT_EQ(pt_persistent.best_path_obj_pairs[0].path_index, 0);
}


/**
 * Tests module for finding of best matching path. Here one valid path is given so that one valid path object pair can be created.
 * Additionally the position of the object is chosen in a way that the negative fast abs branch is hit. \uts{CSCSA-43704}
 * \sdd{SF-7457} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Find_Best_Matching_Path__valid_path_shall_be_set_to_best_matching_one_negative_fast_abs_case)
{
   /** \arrange Set up an object moving with the given path. The object is directly moving on that path such that it is expected to
    * be matched to that path. */
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   next_point_idx.next_point_idx_lat_path  = PT_MID_GRID_POINT_INDEX;
   next_point_idx.next_point_idx_long_path = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   float32_T path_val                      = 1.0f;
   Vector_2d_T first_path_point = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f);
   Vector_2d_T last_path_point  = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f);

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], p_grid_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 1u, first_path_point, last_path_point, 0.0f, path_val);

   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;

   obj_move_dir                    = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(1.1f * path_val, p_grid_array[PT_MID_GRID_POINT_INDEX] - 1.0f);
   object.tracker_data.vcs_heading = 0.5f * PI;
   object.tracker_data.index       = 0;
   object.tracker_data.id          = 1;

   path_obj_pair_consumer_info.distance_to_path = 90.0f;
   /** \action Call function for path object pair creation */
   Pt_Find_Best_Matching_Path(&pt_persistent, &path_obj_info, &path_obj_pair_consumer_info, &pt_input, &cals, obj_move_dir,
                              &next_point_idx, &object.tracker_data);

   /** \assert expect path to be matched to the given object */
   EXPECT_EQ(pt_persistent.best_path_obj_pairs[0].path_index, 0);
}


/**
 * Tests module for finding of best matching path. Here two pt_persistent.paths are put into the function. The first one shall be
 * taken, since it is a better match compared to the second one. \uts{CSCSA-43705} \sdd{SF-7457} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Find_Best_Matching_Path__take_first_valid_path_instead_of_second_one)
{
   /** \arrange Set up multiple pt_persistent.paths. The first one shall be a better match for the object, since it is nearer to
    * it. */
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   next_point_idx.next_point_idx_lat_path = PT_MID_GRID_POINT_INDEX;
   next_point_idx.next_point_idx_lat_path = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   float32_T path_val                     = 1.0f;
   Vector_2d_T first_path_point = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f);
   Vector_2d_T last_path_point  = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f);

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], p_grid_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 1u, first_path_point, last_path_point, 0.0f, path_val);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], p_grid_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 1u, first_path_point, last_path_point, 0.0f, path_val + 1.0f);

   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[0].path_index = 0;
   pt_persistent.paths[1].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[1].path_index = 1;

   obj_move_dir                    = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_MID_GRID_POINT_INDEX] - 1.0f);
   object.tracker_data.vcs_heading = 0.5f * PI;
   object.tracker_data.index       = 0;
   object.tracker_data.id          = 1;

   path_obj_pair_consumer_info.distance_to_path = 90.0f;

   /** \action Call function for path object pair creation */
   Pt_Find_Best_Matching_Path(&pt_persistent, &path_obj_info, &path_obj_pair_consumer_info, &pt_input, &cals, obj_move_dir,
                              &next_point_idx, &object.tracker_data);

   /** \assert expect path to be matched to the given object */
   EXPECT_EQ(pt_persistent.best_path_obj_pairs[0].path_index, 0);
}

/**
 * Tests whether the initialization of pair confidence is done correctly.
 * \uts{CSCSA-43706} \sdd{SF-7460} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Pt_Init_Path_Obj_Pair_Confidence__Initialize_Structure)
{
   /** \arrange Set up an instance of the pair confidence via fixture class. */
   /** \action call the initialization step. */
   Pt_Pt_Init_Path_Obj_Pair_Confidence(&path_obj_pair_confidence);
   /** \assert expect default values to be set */
   EXPECT_FLOAT_EQ(path_obj_pair_confidence.confidence_factor_total, 0.0f);
   EXPECT_FLOAT_EQ(path_obj_pair_confidence.dist_border_to_isect_confidence, 0.0f);
   EXPECT_FLOAT_EQ(path_obj_pair_confidence.dist_obj_to_border_confidence, 0.0f);
   EXPECT_FLOAT_EQ(path_obj_pair_confidence.dist_obj_to_path_confidence, 0.0f);
   EXPECT_FLOAT_EQ(path_obj_pair_confidence.similarity_trail_path_confidence, 0.0f);
   EXPECT_FLOAT_EQ(path_obj_pair_confidence.heading_difference_confidence, 0.0f);
}

/**
 * Tests whether the object is out of path tracking range. Here it is outside of the range on a longitudinal perspective, thus true
 * is expected. \uts{CSCSA-43707} \sdd{SF-7467} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Pos_Out_Of_Path_Tracking_Range__Long_Obj_Out_Of_Path_Tracking_Range)
{
   /** \arrange Set up an object outside of the path tracking range. */
   boolean_T res;
   Vector_2d_T obj_pos = Create_2d_Vector_Coordinates(pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_long_posn + 1.0f, 0.0f);
   obj_move_dir        = PT_OBJECT_MOV_DIR_LONG_FORWARD;

   /** \action Call range check */
   res = Pt_Is_Obj_Pos_Out_Of_Path_Tracking_Range(&(obj_pos), obj_move_dir, &pt_input);
   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Tests whether the object is out of path tracking range. Here it is outside of the range on a lateral perspective, thus true is
 * expected. \uts{CSCSA-43708} \sdd{SF-7467} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Pos_Out_Of_Path_Tracking_Range__Lat_Obj_Out_Of_Path_Tracking_Range)
{
   /** \arrange Set up an object outside of the path tracking range. */
   boolean_T res;
   Vector_2d_T obj_pos = Create_2d_Vector_Coordinates(0.0f, pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_lat_posn + 1.0f);
   obj_move_dir        = PT_OBJECT_MOV_DIR_LAT_LEFT;

   /** \action Call range check */
   res = Pt_Is_Obj_Pos_Out_Of_Path_Tracking_Range(&(obj_pos), obj_move_dir, &pt_input);
   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Checks whether an object is invalid to be matched. Here it is invalid due to its speed. Thus true is expected.
 * \uts{CSCSA-43709} \sdd{SF-7469} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path__Object_Not_Valid_For_Matching_Speed_Cond)
{
   /** \arrange set up an object which is too slow to be valid. */
   boolean_T res;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(0.0f, (pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_lat_posn / 2.0f));
   obj_move_dir                = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.speed   = cals.k_pt_find_min_speed - EPSILON;
   object.tracker_data.status  = PA_OBJ_STATUS_MATURE;

   /** \action Call validity check for an object to be matched to a path. */
   res = Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path(obj_move_dir, &cals, &pt_input, &object.tracker_data);

   /** \assert expect true. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether an object is invalid to be matched. Here it is invalid since no movement direction is given. Thus true is
 * expected. \uts{CSCSA-43710} \sdd{SF-7469} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path__Object_No_Defined_Mov_Dir)
{
   /** \arrange set up an object with an invalid movement direction. */
   boolean_T res;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(0.0f, (pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_lat_posn / 2.0f));
   obj_move_dir                = PT_OBJECT_MOV_DIR_NONE;
   object.tracker_data.speed   = cals.k_pt_find_min_speed - EPSILON;
   object.tracker_data.status  = PA_OBJ_STATUS_MATURE;

   /** \action Call validity check for an object to be matched to a path. */
   res = Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path(obj_move_dir, &cals, &pt_input, &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Checks whether an object is invalid to be matched. Here it is invalid since its status is invalid. Thus true is expected.
 * \uts{CSCSA-43711} \sdd{SF-7469} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path__Object_Status_Invalid)
{
   /** \arrange set up an object with status invalid. */
   boolean_T res;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(0.0f, (pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_lat_posn / 2.0f));
   obj_move_dir                = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.speed   = cals.k_pt_find_min_speed - EPSILON;
   object.tracker_data.status  = PA_OBJ_STATUS_INVALID;

   /** \action Call validity check for an object to be matched to a path. */
   res = Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path(obj_move_dir, &cals, &pt_input, &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Checks whether an object is invalid to be matched. Here it is valid. Thus false is expected.
 * \uts{CSCSA-43712} \sdd{SF-7469} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path__Object_Valid)
{
   /** \arrange set up a valid object.tracker_data. */
   boolean_T res;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(0.0f, (pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_lat_posn / 2.0f));
   obj_move_dir                = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.speed   = cals.k_pt_find_min_speed + EPSILON;
   object.tracker_data.status  = PA_OBJ_STATUS_MATURE;

   /** \action Call validity check for an object to be matched to a path. */
   res = Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path(obj_move_dir, &cals, &pt_input, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Checks whether the object orientation is matching with the path direction. Here a longitudinal moving object and a longitudinal
 * path is given. Thus true is returned. \uts{CSCSA-43713} \sdd{SF-7456} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit__Both_Longitudinal)
{
   /** \arrange Set up a longitudinal moving object and a longitudinal path. */
   boolean_T res;
   path.direction = PATH_DIRECTION_LONG_FORWARD;
   obj_move_dir   = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   /** \action Call check whether object movement direction fits with the path direction. */
   res = Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit(&path, obj_move_dir);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Checks whether the object orientation is matching with the path direction. Here a lateral moving object and a lateral path is
 * given. Thus true is returned. \uts{CSCSA-43714} \sdd{SF-7456} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit__Both_Lateral)
{
   /** \arrange Set up a lateral moving object and a lateral path. */
   boolean_T res;
   path.direction = PATH_DIRECTION_LAT_LEFT;
   obj_move_dir   = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action Call check whether object movement direction fits with the path direction. */
   res = Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit(&path, obj_move_dir);

   /** \assert expect true */
   EXPECT_TRUE(res);
}


/**
 * Checks whether the object orientation is matching with the path direction. Here a longitudinal moving object and a lateral path
 * is given. Thus false is returned. \uts{CSCSA-43715} \sdd{SF-7456} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit__Directions_Do_Not_Match)
{
   /** \arrange Set up a longitudinal moving object and a lateral path. */
   boolean_T res;
   path.direction = PATH_DIRECTION_LAT_LEFT;
   obj_move_dir   = PT_OBJECT_MOV_DIR_LONG_FORWARD;

   /** \action Call check whether object movement direction fits with the path direction. */
   res = Pt_Do_Object_Movement_Direction_And_Path_Direction_Fit(&path, obj_move_dir);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Checks whether the path is valid for object matching. Here the path is too short. Thus false is returned.
 * \uts{CSCSA-43716} \sdd{SF-7472} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Valid_For_Object_Matching__path_is_too_short)
{
   /**\arrange Set up a path which is too short for matching. */
   boolean_T res;
   path.path_state = PATH_STATUS_DEFAULT;
   path.first_p    = PT_LOWEST_GRID_POINT_INDEX;
   path.last_p     = PT_LOWEST_GRID_POINT_INDEX + cals.k_pt_find_min_diff_path_points - PT_SINGLE_GRID_POINT_OFFSET;

   /*Conditions for falsification of test*/
   path.obj_curr_used_for_path_build.id = 1;
   object.tracker_data.id               = 1;

   /** \action Call check whether path is valid for object matching. */
   res = Pt_Is_Path_Valid_For_Object_Matching(&path, &object.tracker_data, &cals);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Checks whether the path is valid for object matching. Here the path state is default. Thus false is returned.
 * \uts{CSCSA-43717} \sdd{SF-7472} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Valid_For_Object_Matching__path_state_is_default)
{
   /** \arrange set up a path in default state. */
   boolean_T res;
   path.path_state = PATH_STATUS_DEFAULT;

   /** \action Call check whether path is valid for object matching. */
   res = Pt_Is_Path_Valid_For_Object_Matching(&path, &object.tracker_data, &cals);

   /** \assert expect false */
   EXPECT_FALSE(res);
}


/**
 * Checks whether the path is valid for object matching. Here the path shall be valid and thus true is returned.
 * \uts{CSCSA-43718} \sdd{SF-7472} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Valid_For_Object_Matching__Path_Is_Valid_For_Object_Matching)
{
   /** \arrange Set up a valid path for object matching. */
   boolean_T res;
   path.path_state                      = PATH_STATUS_CREATION;
   path.obj_curr_used_for_path_build.id = 1;
   object.tracker_data.id               = 2;

   // TODO: Ask Marvin why he set path.first_p = next_point_index-cals.k_pt_find_min_diff_path_points;
   path.first_p = next_point_index; //   -cals.k_pt_find_min_diff_path_points;
   path.last_p  = next_point_index + cals.k_pt_find_min_diff_path_points + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check whether path is valid for object matching. */
   res = Pt_Is_Path_Valid_For_Object_Matching(&path, &object.tracker_data, &cals);

   /** \assert expect true */
   EXPECT_TRUE(res);
}


/**
 * Set up a path in creation phase and use a index which is contained in the path. Thus true is expected.
 * \uts{CSCSA-43719} \sdd{SF-7586} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Next_Point_Contained_In_Valid_Path_Borders__point_is_contained_in_path)
{
   /** \arrange Set up a path in build up process where nearest path index is valid. */
   boolean_T res;
   object.tracker_data.id = 2;
   next_point_index       = PT_MID_GRID_POINT_INDEX;
   path.first_p           = next_point_index - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p            = next_point_index + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check whether path is valid for object matching. */
   res = Pt_Is_Next_Point_Contained_In_Valid_Path_Borders(next_point_index, &path, &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect true */
   EXPECT_TRUE(res);
}


/**
 * Set up a path in creation phase and use a index which is not contained in the path. Thus false is expected.
 * \uts{CSCSA-43720} \sdd{SF-7586} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Next_Point_Contained_In_Valid_Path_Borders__point_is_not_contained_in_path)
{
   /** \arrange Set up a path in build up process where nearest path index is valid. */
   boolean_T res;
   path.obj_curr_used_for_path_build.id = 2;
   next_point_index                     = PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET;
   path.first_p                         = next_point_index + PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                          = next_point_index + 5 * PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check whether path is valid for object matching. */
   res = Pt_Is_Next_Point_Contained_In_Valid_Path_Borders(next_point_index, &path, &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect false */
   EXPECT_FALSE(res);
}


/**
 * Checks whether a point index is in the valid extrapolated area of a path. Here it is in the path, thus true is expected.
 * \uts{CSCSA-43721} \sdd{SF-7464} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Idx_In_Valid_Extrapol_Pt_Part__Index_Is_In_Valid_Path_Part)
{

   /** \arrange set up index in the valid path part */
   boolean_T res;
   path.obj_curr_used_for_path_build.id = FBK_ZERO_UINT;

   next_point_index = PT_MID_GRID_POINT_INDEX;

   path.first_p = next_point_index - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p  = next_point_index + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check for containment of index in extrapolated path part. */
   res = Pt_Is_Idx_In_Valid_Extrapol_Pt_Part(next_point_index, &path, &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Checks whether a point index is in the valid extrapolated area of a path. Here it is in the valid area around the path, thus
 * true is expected. \uts{CSCSA-43722} \sdd{SF-7464} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Idx_In_Valid_Extrapol_Pt_Part__Index_Is_Valid_Area_Around_Path_Part)
{

   /** \arrange set up index in valid extrapolated path part. */
   boolean_T res;
   path.obj_curr_used_for_path_build.id = FBK_ZERO_UINT;

   path.first_p = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p  = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;

   next_point_index = path.last_p + cals.k_pt_find_max_diff_path_points - PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check for containment of index in extrapolated path part. */
   res = Pt_Is_Idx_In_Valid_Extrapol_Pt_Part(next_point_index, &path, &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Checks whether a point index is in the valid extrapolated area of a path. Here it is outside of the valid area around the path,
 * thus false is expected. \uts{CSCSA-43723} \sdd{SF-7464} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Idx_In_Valid_Extrapol_Pt_Part__Index_Is_Out_Of_Valid_Area_Around_Path_Bounds)
{
   /** \arrange set up index out of the valid area of a path */
   boolean_T res;
   path.obj_curr_used_for_path_build.id = FBK_ZERO_UINT;

   path.first_p = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p  = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;

   next_point_index = path.last_p + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_diff_path_points + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check for containment of index in extrapolated path part. */
   res = Pt_Is_Idx_In_Valid_Extrapol_Pt_Part(next_point_index, &path, &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Checks whether a point index is in the valid extrapolated area of a path. Here the path is not out of its creation phase, thus
 * false shall be returned. \uts{CSCSA-43724} \sdd{SF-7464} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Idx_In_Valid_Extrapol_Pt_Part__Obj_Building_Up_This_Path_Not_Vanished_Yet_So_Disable_This_Check)
{

   /** \arrange set up a path in creation process */
   boolean_T res;
   path.obj_curr_used_for_path_build.id = 1;

   next_point_index = PT_MID_GRID_POINT_INDEX;
   path.first_p     = next_point_index - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p      = next_point_index + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check for containment of index in extrapolated path part. */
   res = Pt_Is_Idx_In_Valid_Extrapol_Pt_Part(next_point_index, &path, &pt_input.Num_Grid_Pts_Dep_Cals);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Checks whether a point index is in the real tracked part of a path. Here it is included in the path, thus true is expected.
 * \uts{CSCSA-43725} \sdd{SF-7463} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Idx_In_Path_Borders__idx_is_in_path_borders)
{

   /** \arrange set up index in the real tracked path part. */
   boolean_T res;
   path.obj_curr_used_for_path_build.id = 1;
   next_point_index                     = PT_MID_GRID_POINT_INDEX;
   path.first_p                         = next_point_index - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                          = next_point_index + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check for containment of index in real tracked path part. */
   res = Pt_Is_Idx_In_Path_Borders(next_point_index, &path);

   /** \assert expect true */
   EXPECT_TRUE(res);
}

/**
 * Checks whether a point index is in the real tracked part of a path. Here it is not included in the path, thus false is expected.
 * \uts{CSCSA-43726} \sdd{SF-7463} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Idx_In_Path_Borders__idx_is_not_in_path_borders)
{
   /** \arrange Set up index outside of the real tracked path borders */
   boolean_T res;
   path.obj_curr_used_for_path_build.id = 1;
   path.first_p                         = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                          = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;

   next_point_index = path.last_p + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check for containment of index in real tracked path part. */
   res = Pt_Is_Idx_In_Path_Borders(next_point_index, &path);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Checks whether a point index is in the real tracked part of a path. Here a path is created which is has finished its creation
 * phase. Thus false is expected. \uts{CSCSA-43727} \sdd{SF-7463} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Idx_In_Path_Borders__Disable_This_Check_Since_Obj_Building_Up_The_Path_Has_Vanished)
{

   /** \arrange Set up a path which has finished its creation phase */
   boolean_T res;
   path.obj_curr_used_for_path_build.id = FBK_ZERO_UINT;

   next_point_index = PT_MID_GRID_POINT_INDEX;
   path.first_p     = next_point_index - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p      = next_point_index + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check for containment of index in real tracked path part. */
   res = Pt_Is_Idx_In_Path_Borders(next_point_index, &path);

   /** \assert expect false */
   EXPECT_FALSE(res);
}

/**
 * Tests whether the next point index for an object is returned correctly. Here the object moves aligned with vcs, so that the
 * offset shall be applied within the function and index 1 is returned. \uts{CSCSA-43728} \sdd{SF-7459}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Next_Point_For_Object_To_Pass__obj_moving_longitudinal_aligned_vcs)
{

   /** \arrange setup object aligned with vcs without heading increment */
   object.tracker_data.vcs_pos.x   = p_grid_array[PT_LOWEST_GRID_POINT_INDEX] + 1.0f;
   object.tracker_data.vcs_pos.y   = p_grid_array[PT_MID_GRID_POINT_INDEX] + 1.0f;
   object.tracker_data.vcs_heading = PI;
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   /** \action Call function to check */
   Pt_Get_Next_Point_For_Object_To_Pass(&next_point_idx, &object.tracker_data, p_grid_array);
   /** \assert expect long index to be 1 and lat index to be mid+1 */
   EXPECT_EQ(next_point_idx.next_point_idx_long_path, PT_LOWEST_GRID_POINT_INDEX);
   EXPECT_EQ(next_point_idx.next_point_idx_lat_path, PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET);
}

/**
 * Tests whether the next point index for an object is returned correctly. Here the object moves aligned with vcs, so that the
 * offset shall be applied within the function and index 0 is returned. \uts{CSCSA-43729} \sdd{SF-7459}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Next_Point_For_Object_To_Pass__obj_moving_longitudinal_aligned_vcs_at_idx_0_lowest_border)
{
   /** \arrange setup object aligned with vcs */
   object.tracker_data.vcs_pos.x   = p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f;
   object.tracker_data.vcs_pos.y   = p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f;
   object.tracker_data.vcs_heading = 0.25f * PI;
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   next_point_idx.next_point_idx_lat_path  = PT_INVALID_GRID_POINT_INDEX;
   next_point_idx.next_point_idx_long_path = PT_INVALID_GRID_POINT_INDEX;
   /** \action Call function to check */
   Pt_Get_Next_Point_For_Object_To_Pass(&next_point_idx, &object.tracker_data, p_grid_array);
   /** \assert expect long index to be 0 and lat point to be greater than maximum */
   EXPECT_EQ(next_point_idx.next_point_idx_long_path, PT_LOWEST_GRID_POINT_INDEX);
   EXPECT_EQ(next_point_idx.next_point_idx_lat_path, PT_HIGHEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET);
}

/**
 * Tests whether the next point index for an object is returned correctly. Here the object moves against vcs, so that the offset
 * shall not be applied within the function and index 0 is returned. \uts{CSCSA-43730} \sdd{SF-7459}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Next_Point_For_Object_To_Pass__Obj_Moving_Lateral_aligned_against_vcs_highest_border)
{

   /** \arrange setup object moving against with vcs */
   object.tracker_data.vcs_pos.y   = p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f;
   object.tracker_data.vcs_pos.x   = p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f;
   object.tracker_data.vcs_heading = -0.75f * PI;
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   next_point_idx.next_point_idx_lat_path  = PT_INVALID_GRID_POINT_INDEX;
   next_point_idx.next_point_idx_long_path = PT_INVALID_GRID_POINT_INDEX;
   /** \action Call function to check */
   Pt_Get_Next_Point_For_Object_To_Pass(&next_point_idx, &object.tracker_data, p_grid_array);
   /** \assert expect lat index to be the highest and long index to be the lowest possible. */
   EXPECT_EQ(next_point_idx.next_point_idx_lat_path, PT_HIGHEST_GRID_POINT_INDEX);
   EXPECT_EQ(next_point_idx.next_point_idx_long_path, PT_INVALID_GRID_POINT_INDEX);
}

/**
 * Tests whether object movement direction determination is done correctly. Here the object moves lateral right indicated by its
 * heading. \uts{CSCSA-43731} \sdd{SF-7455} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Determine_Object_Movement_Direction__Obj_Move_Dir_Lateral_Right)
{
   /** \arrange set up heading for objects classified as lateral right moving */
   float32_T heading = cals.k_pt_lower_lim_obj_orient_lat + EPSILON;
   /** \action Call object movement direction determination function. */
   obj_move_dir = Pt_Determine_Object_Movement_Direction(heading, &cals);
   /** \assert expect lateral right */
   EXPECT_EQ(obj_move_dir, PT_OBJECT_MOV_DIR_LAT_RIGHT);
}

/**
 * Tests whether object movement direction determination is done correctly. Here the object moves lateral left indicated by its
 * heading. \uts{CSCSA-43732} \sdd{SF-7455} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Determine_Object_Movement_Direction__Obj_Move_Dir_Lateral_left)
{
   /** \arrange set up heading for objects classified as lateral left moving */
   float32_T heading = -cals.k_pt_lower_lim_obj_orient_lat - EPSILON;
   /** \action Call object movement direction determination function. */
   obj_move_dir = Pt_Determine_Object_Movement_Direction(heading, &cals);
   /** \assert expect lateral left */
   EXPECT_EQ(obj_move_dir, PT_OBJECT_MOV_DIR_LAT_LEFT);
}

/**
 * Tests whether object movement direction determination is done correctly. Here the object moves longitudinal forward indicated by
 * its heading. \uts{CSCSA-43733} \sdd{SF-7455} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Determine_Object_Movement_Direction__Obj_Move_Dir_Long_Forward)
{
   /** \arrange set up heading for objects classified as longitudinal forward moving */
   float32_T heading = cals.k_pt_lower_lim_obj_orient_lat - EPSILON;
   /** \action Call object movement direction determination function. */
   obj_move_dir = Pt_Determine_Object_Movement_Direction(heading, &cals);
   /** \assert expect longitudinal forward */
   EXPECT_EQ(obj_move_dir, PT_OBJECT_MOV_DIR_LONG_FORWARD);
}

/**
 * Tests whether object movement direction determination is done correctly. Here the object moves longitudinal backward indicated
 * by its heading. \uts{CSCSA-43734} \sdd{SF-7455} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Determine_Object_Movement_Direction__Obj_Move_Dir_Long_Backward)
{
   /** \arrange set up heading for objects classified as longitudinal backward moving */
   float32_T heading = cals.k_pt_upper_lim_obj_orient_lat + EPSILON;
   /** \action Call object movement direction determination function. */
   obj_move_dir = Pt_Determine_Object_Movement_Direction(heading, &cals);
   /** \assert expect longitudinal backward */
   EXPECT_EQ(obj_move_dir, PT_OBJECT_MOV_DIR_LONG_BACKWARD);
}

/**
 * Tests whether object movement direction determination is done correctly. Here the object moves longitudinal backward indicated
 * by its heading. \uts{CSCSA-43735} \sdd{SF-7455} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Determine_Object_Movement_Direction__Obj_Move_Dir_Long_Backward_2)
{
   /** \arrange set up heading for objects classified as longitudinal backward moving */
   float32_T heading = PI + EPSILON;
   /** \action Call object movement direction determination function. */
   obj_move_dir = Pt_Determine_Object_Movement_Direction(heading, &cals);
   /** \assert expect longitudinal backward */
   EXPECT_EQ(obj_move_dir, PT_OBJECT_MOV_DIR_LONG_BACKWARD);
}

/**
 * Tests whether object movement direction determination is done correctly. Here the object moves longitudinal forward indicated by
 * its heading. \uts{CSCSA-43736} \sdd{SF-7455} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Determine_Object_Movement_Direction__Obj_Move_Dir_Long_Forward_2)
{
   /** \arrange set up heading for objects classified as longitudinal forward moving */
   float32_T heading = -EPSILON;
   /** \action Call object movement direction determination function. */
   obj_move_dir = Pt_Determine_Object_Movement_Direction(heading, &cals);
   /** \assert expect longitudinal forward */
   EXPECT_EQ(obj_move_dir, PT_OBJECT_MOV_DIR_LONG_FORWARD);
}


/**
 * Checks whether the distances to relevant path points are calculated correctly. Here the object is moving against vcs, is in the
 * path and only one point away from mid_grid_pt_idx. \uts{CSCSA-43737} \sdd{SF-7474} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Set_Path_Obj_Pair_Border_Info__Obj_Moves_Against_Vcs_Is_In_Path_And_Path_One_Step_Next_To_Mid)
{
   /** \arrange Set up object moving against vcs with a distance of 1 to mid. */
   path.first_p = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p  = PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;

   obj_move_dir     = PT_OBJECT_MOV_DIR_LAT_LEFT;
   next_point_index = path.first_p + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call Path object pair constructor */
   Pt_Set_Path_Obj_Pair_Border_Info(&path_obj_info, &path, next_point_index, obj_move_dir);

   /** \assert expect dist to border to be 0 and distance to mid to be 1 */
   ASSERT_EQ(path_obj_info.border_info.dist_border_to_mid, PT_SINGLE_GRID_POINT_OFFSET);
   ASSERT_EQ(path_obj_info.border_info.dist_border_to_obj, 0);
}


/**
 * Checks whether the distances to relevant path points are calculated correctly. Here the object is moving against vcs and next
 * point index is equal to the upper boundary of the path. \uts{CSCSA-43738} \sdd{SF-7474} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Set_Path_Obj_Pair_Border_Info__obj_moves_against_vcs_and_next_point_equals_upper_boundary)
{
   /** \arrange Set up object moving against vcs with next_point equal to the upper boundary of the path. */
   path.first_p = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p  = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;

   obj_move_dir     = PT_OBJECT_MOV_DIR_LAT_LEFT;
   next_point_index = path.last_p;

   /** \action Call Path object pair constructor */
   Pt_Set_Path_Obj_Pair_Border_Info(&path_obj_info, &path, next_point_index, obj_move_dir);

   /** \assert expect dist to border to be 0. */
   EXPECT_EQ(path_obj_info.border_info.dist_border_to_obj, 0);
}


/**
 * Checks whether the distances to relevant path points are calculated correctly. Here the object is moving aligned with vcs and
 * next point index is equal to the lower boundary of the path. \uts{CSCSA-43739} \sdd{SF-7474} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Set_Path_Obj_Pair_Border_Info__obj_moves_aligned_with_vcs_and_next_point_equals_lower_boundary)
{
   /** \arrange Set up object moving against vcs with next_point equal to the lower boundary of the path. */
   path.first_p = PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p  = PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET;

   obj_move_dir     = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   next_point_index = path.first_p;

   /** \action Call Path object pair constructor */
   Pt_Set_Path_Obj_Pair_Border_Info(&path_obj_info, &path, next_point_index, obj_move_dir);

   /** \assert expect dist to border to be 0. */
   EXPECT_EQ(path_obj_info.border_info.dist_border_to_obj, 0);
}

/**
 * Checks whether the distances to relevant path points are calculated correctly. Here the object is moving aligned with vcs, is in
 * the path and only one point away from mid_grid_pt_idx. \uts{CSCSA-43740} \sdd{SF-7474} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Set_Path_Obj_Pair_Border_Info__Obj_Moves_Aligned_With_Vcs_Is_In_Path_And_Path_One_Step_From_Mid)
{
   /** \arrange set uo */
   path.first_p = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p  = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;

   obj_move_dir     = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   next_point_index = path.last_p + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call Path object pair constructor */
   Pt_Set_Path_Obj_Pair_Border_Info(&path_obj_info, &path, next_point_index, obj_move_dir);

   /** \assert expect dist to border to be 0 and distance to mid to be 1 */
   ASSERT_EQ(path_obj_info.border_info.dist_border_to_mid, PT_SINGLE_GRID_POINT_OFFSET);
   ASSERT_EQ(path_obj_info.border_info.dist_border_to_obj, 0);
}

/**
 * Checks whether the distances to relevant path points are calculated correctly. Here the object has an unknown movement
 * direction. Thus default values shall be returned. \uts{CSCSA-43741} \sdd{SF-7474} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Set_Path_Obj_Pair_Border_Info__Obj_Unknow_Moving_Direction)
{
   /** \arrange set up an object without a movement direction. */
   obj_move_dir                                 = PT_OBJECT_MOV_DIR_NONE;
   path_obj_info.border_info.dist_border_to_mid = PT_INVALID_GRID_POINT_INDEX;
   path_obj_info.border_info.dist_border_to_obj = PT_INVALID_GRID_POINT_INDEX;

   /** \action Call Path object pair constructor */
   Pt_Set_Path_Obj_Pair_Border_Info(&path_obj_info, &path, next_point_index, obj_move_dir);

   /** \assert expect default values */
   ASSERT_EQ(path_obj_info.border_info.dist_border_to_mid, PT_DEFAULT_DISCR_BORDER);
   ASSERT_EQ(path_obj_info.border_info.dist_border_to_obj, PT_DEFAULT_DISCR_BORDER);
}

/**
 * Tests the confidence calculation for a current path object pair. Here each metric is set to its corresponding lookuptable first
 * element. Thus it is expected, that the total confidence factor is 1. \uts{CSCSA-43742} \sdd{SF-7448}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Confidence_Of_Current_Pair__Get_Confidence_Of_One)
{
   /** \arrange set up metrics to their corresponding first values of lookuptables. */
   path_obj_info.border_info.dist_border_to_obj = cals.k_pt_dist_obj_to_border_lut[0];
   path_obj_info.border_info.dist_border_to_mid = cals.k_pt_dist_border_to_isect_lut[0];
   path_obj_info.relevant_dist_comp_matching    = cals.k_pt_dist_obj_to_path_lut[0];
   path_obj_info.weighted_mean_diff_last_points = cals.k_pt_similarity_trail_path_lut[0];
   for (uint8_t idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      path_obj_info.heading_diff_pt_segment[idx] = cals.k_pt_heading_diff_lut[0];
   }

   /** \action execute the confidence calculation of the current path object pair */
   Pt_Calc_Confidence_Of_Current_Pair(&path_obj_pair_confidence, &path_obj_info, &cals, object.tracker_data.index, 0u);

   /** \assert expect confidence of 1 */
   EXPECT_FLOAT_EQ(path_obj_pair_confidence.confidence_factor_total, 1.0f);
}

/**
 * Tests the confidence calculation for a current path object pair. Here each metric is set to its last entry of the respective
 * lookuptable. Thus it is expected, that the total confidence factor is equal to the possible minimum. \uts{CSCSA-43743}
 * \sdd{SF-7448} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Confidence_Of_Current_Pair__Get_Lowest_Possible_Confidence)
{
   /** \arrange set up confidences to the last elements of their lookuptables. */
   path_obj_info.border_info.dist_border_to_obj = cals.k_pt_dist_obj_to_border_lut[PT_K_PT_DIST_OBJ_TO_BORDER_LUT_ARRAY_SIZE_DIM0 - 1];
   path_obj_info.border_info.dist_border_to_mid =
      cals.k_pt_dist_border_to_isect_lut[PT_K_PT_DIST_BORDER_TO_ISECT_LUT_ARRAY_SIZE_DIM0 - 1];
   path_obj_info.relevant_dist_comp_matching = cals.k_pt_dist_obj_to_path_lut[PT_K_PT_DIST_OBJ_TO_PATH_LUT_ARRAY_SIZE_DIM0 - 1];
   path_obj_info.weighted_mean_diff_last_points =
      cals.k_pt_similarity_trail_path_lut[PT_K_PT_SIMILARITY_TRAIL_PATH_LUT_ARRAY_SIZE_DIM0 - 1];
   for (uint8_t idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      path_obj_info.heading_diff_pt_segment[idx] = cals.k_pt_heading_diff_lut[PT_K_PT_HEADING_DIFF_LUT_ARRAY_SIZE_DIM0 - 1];
   }

   /** \action execute the confidence calculation of the current path object pair */
   Pt_Calc_Confidence_Of_Current_Pair(&path_obj_pair_confidence, &path_obj_info, &cals, object.tracker_data.index, 0u);

   /** \assert expect the minimum confidence value */
   EXPECT_FLOAT_EQ(path_obj_pair_confidence.confidence_factor_total,
                   cals.k_pt_dist_obj_to_border_conf_lut[PT_K_PT_DIST_OBJ_TO_BORDER_LUT_ARRAY_SIZE_DIM0 - 1]
                      * cals.k_pt_dist_border_to_isect_conf_lut[PT_K_PT_DIST_BORDER_TO_ISECT_LUT_ARRAY_SIZE_DIM0 - 1]
                      * cals.k_pt_dist_obj_to_path_conf_lut[PT_K_PT_DIST_OBJ_TO_PATH_LUT_ARRAY_SIZE_DIM0 - 1]
                      * cals.k_pt_similarity_trail_path_conf_lut[PT_K_PT_SIMILARITY_TRAIL_PATH_LUT_ARRAY_SIZE_DIM0 - 1]
                      * cals.k_pt_heading_diff_conf_lut[PT_K_PT_HEADING_DIFF_LUT_ARRAY_SIZE_DIM0 - 1]);
}


/**
 * Tests calculation of two last path point of an objects trail. Here the path is against vcs and for the mid_grid_point_idx as
 * first_p an increment is expected. \uts{CSCSA-43744} \sdd{SF-7458} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Last_Two_Point_Idx_Of_Trail__Path_Against_Vcs)
{
   /** \arrange set up input the mid_grid_point_idx and path against vcs */
   Pt_Trail_Path_Comp_Indices_T indices_to_compare;
   path.direction = PATH_DIRECTION_LAT_LEFT;
   path.first_p   = PT_MID_GRID_POINT_INDEX;

   /** \action Call function to determine the last two points in respect to the input index. */
   Pt_Get_Last_Two_Point_Idx_Of_Trail(&indices_to_compare, &path);

   /** \assert expect first_p and its increment to be returned. */
   ASSERT_EQ(indices_to_compare.path_point_index_higher_prio, path.first_p);
   ASSERT_EQ(indices_to_compare.path_point_index_lower_prio, path.first_p + PT_SINGLE_GRID_POINT_OFFSET);
}

/**
 * Tests calculation of two last path point of an objects trail. Here the path is aligned with vcs and for the mid_grid_point_idx
 * as last_p an decrement is expected. \uts{CSCSA-43745} \sdd{SF-7458} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Last_Two_Point_Idx_Of_Trail__Path_Aligned_With_Vcs)
{
   /** \arrange set up input the mid_grid_point_idx and path aligned with vcs */
   Pt_Trail_Path_Comp_Indices_T indices_to_compare;
   path.direction = PATH_DIRECTION_LAT_RIGHT;
   path.last_p    = PT_MID_GRID_POINT_INDEX;

   /** \action Call function to determine the last two points in respect to the input index. */
   Pt_Get_Last_Two_Point_Idx_Of_Trail(&indices_to_compare, &path);

   /** \assert expect last_p and its decrement to be returned. */
   ASSERT_EQ(indices_to_compare.path_point_index_higher_prio, path.last_p);
   ASSERT_EQ(indices_to_compare.path_point_index_lower_prio, path.last_p - PT_SINGLE_GRID_POINT_OFFSET);
}

/**
 * Tests calculation weighted distance between trail of object and path. Here the object has not created a trail yet, thus the
 * weighted mean shall be set to its default value. \uts{CSCSA-43746} \sdd{SF-7450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Weighted_Dist_Of_Path_Pt__Object_Did_Not_Create_Path_By_Itself_Yet)
{
   /** \arrange set up a scenario where no path is build up by the given object id. */
   object.tracker_data.id = 1;
   Pt_Reset_All_Paths(pt_persistent.paths, &path_output, pt_persistent.best_path_obj_pairs);

   /** \action Call calculation of weighted mean distance between trail and object.tracker_data. */
   Pt_Calc_Weighted_Dist_Of_Path_Pt(&path_obj_info, pt_persistent.paths, &pt_persistent.paths[0], &(object.tracker_data), &cals);

   /** \assert expect weighted distance of zero. */
   EXPECT_FLOAT_EQ(path_obj_info.weighted_mean_diff_last_points, 0.0f);
}

/**
 * Tests calculation weighted distance between trail of object and path. Here the object has created a trail,but the path is too
 * short. \uts{CSCSA-43747} \sdd{SF-7450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Weighted_Dist_Of_Path_Pt__Object_Did_Create_Path_But_Length_Is_Too_Short)
{
   /** \arrange set up an object which creates a path which is too short for a non default weighted mean distance calculation. */
   object.tracker_data.id = 1;
   Pt_Reset_All_Paths(pt_persistent.paths, &path_output, pt_persistent.best_path_obj_pairs);
   pt_persistent.paths[0].obj_curr_used_for_path_build.id = 1;
   pt_persistent.paths[0].first_p                         = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p                          = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call calculation of weighted mean distance between trail and object.tracker_data. */
   Pt_Calc_Weighted_Dist_Of_Path_Pt(&path_obj_info, pt_persistent.paths, &pt_persistent.paths[1], &(object.tracker_data), &cals);

   /** \assert expect weighted distance of zero. */
   EXPECT_FLOAT_EQ(path_obj_info.weighted_mean_diff_last_points, 0.0f);
}

/**
 * Tests calculation weighted distance between trail of object and path. Here the object enters the non extrapolated path part for
 * the first time. \uts{CSCSA-43748} \sdd{SF-7450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Weighted_Dist_Of_Path_Pt__Object_Enters_Non_Extrapolated_Path_Part_For_First_Time)
{
   /** \arrange set up an object which has entered the extrapolated path part for the first time. */
   object.tracker_data.id = 1;
   Pt_Reset_All_Paths(pt_persistent.paths, &path_output, pt_persistent.best_path_obj_pairs);
   pt_persistent.paths[0].obj_curr_used_for_path_build.id = 1;
   pt_persistent.paths[0].first_p                         = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p                          = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;

   pt_persistent.paths[1].path_state                                                         = PATH_STATUS_CREATION;
   pt_persistent.paths[1].path_points[PT_MID_GRID_POINT_INDEX]                               = PT_PATH_POINTS_DEFAULT_VAL;
   pt_persistent.paths[1].path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = PT_PATH_POINTS_DEFAULT_VAL;

   /** \action Call calculation of weighted mean distance between trail and object.tracker_data. */
   Pt_Calc_Weighted_Dist_Of_Path_Pt(&path_obj_info, pt_persistent.paths, &pt_persistent.paths[1], &(object.tracker_data), &cals);

   /** \assert expect weighted distance of zero. */
   EXPECT_FLOAT_EQ(path_obj_info.weighted_mean_diff_last_points, 0.0f);
}


/**
 * Tests calculation weighted distance between trail of object and path. Here the path length is not sufficiently enough. Thus
 * default values are expected to be returned by this routine. \uts{CSCSA-43749} \sdd{SF-7450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Weighted_Dist_Of_Path_Pt__path_length_of_objects_created_path_is_not_sufficient)
{
   /** \arrange set up an object which has only added one path point to its trail. */
   object.tracker_data.id = 1;
   Pt_Reset_All_Paths(pt_persistent.paths, &path_output, pt_persistent.best_path_obj_pairs);
   cals.k_pt_min_path_length_obj_trail                    = 2u;
   pt_persistent.paths[0].obj_curr_used_for_path_build.id = 1;
   pt_persistent.paths[0].first_p                         = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p                          = PT_MID_GRID_POINT_INDEX;

   pt_persistent.paths[1].path_state                                                         = PATH_STATUS_CREATION;
   pt_persistent.paths[1].path_points[PT_MID_GRID_POINT_INDEX]                               = PT_PATH_POINTS_DEFAULT_VAL;
   pt_persistent.paths[1].path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = PT_PATH_POINTS_DEFAULT_VAL;

   /** \action Call calculation of weighted mean distance between trail and object.tracker_data. */
   Pt_Calc_Weighted_Dist_Of_Path_Pt(&path_obj_info, pt_persistent.paths, &pt_persistent.paths[1], &(object.tracker_data), &cals);

   /** \assert expect weighted distance of zero. */
   EXPECT_FLOAT_EQ(path_obj_info.weighted_mean_diff_last_points, 0.0f);
}


/**
 * Tests calculation weighted distance between trail of object and path. Here the object has created a trail and thus a weighted
 * mean diff is calculated. \uts{CSCSA-43750} \sdd{SF-7450} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Weighted_Dist_Of_Path_Pt__Calculate_A_Valid_Weighted_Mean_Diff)
{
   /** \arrange Set up a scenario which creates a non default weighted mean difference. */
   object.tracker_data.id = 1;
   Pt_Reset_All_Paths(pt_persistent.paths, &path_output, pt_persistent.best_path_obj_pairs);

   pt_persistent.paths[0].obj_curr_used_for_path_build.id = 1;
   pt_persistent.paths[0].first_p                         = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[0].last_p                          = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[0].direction                       = PATH_DIRECTION_LAT_LEFT;

   pt_persistent.paths[1].path_state                                                         = PATH_STATUS_CREATION;
   pt_persistent.paths[1].path_points[PT_MID_GRID_POINT_INDEX]                               = 5.0f;
   pt_persistent.paths[1].path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = 2.0f;
   pt_persistent.paths[1].first_p = pt_persistent.paths[0].first_p - PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[1].last_p  = pt_persistent.paths[0].last_p + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call calculation of weighted mean distance between trail and object.tracker_data. */
   Pt_Calc_Weighted_Dist_Of_Path_Pt(&path_obj_info, pt_persistent.paths, &pt_persistent.paths[1], &(object.tracker_data), &cals);

   /** \assert expect a weighted mean distance unequal to zero. */
   EXPECT_FLOAT_EQ(path_obj_info.weighted_mean_diff_last_points,
                   cals.k_pt_weight_of_last_trail_point * pt_persistent.paths[1].path_points[PT_MID_GRID_POINT_INDEX]
                      + cals.k_pt_weight_of_sec_last_trail_point
                           * pt_persistent.paths[1].path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]);
}


/**
 * Tests the calculation of relevant matching distance component. Here the object is moving lateral against vcs and the path is
 * oriented lateral left. \uts{CSCSA-43751} \sdd{SF-7449} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Relevant_Match_Dist_Comp__Obj_Moving_Lateral_Path_Against_Vcs)
{
   /** \arrange Set up an object which is moving lateral left as well as a path with direction of lateral left. */
   path.direction                                                          = PATH_DIRECTION_LAT_LEFT;
   obj_move_dir                                                            = PT_OBJECT_MOV_DIR_LAT_LEFT;
   next_point_index                                                        = PT_MID_GRID_POINT_INDEX;
   path.path_points[PT_MID_GRID_POINT_INDEX]                               = 5.0f;
   path.path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = 10.0f;
   object.tracker_data.vcs_pos                                             = Create_2d_Vector_Coordinates(
                                                  0.0f, p_grid_array[PT_MID_GRID_POINT_INDEX] + 0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]);

   /** \action Calculate releveant matching distance component */
   Pt_Calc_Relevant_Match_Dist_Comp(&path_obj_info, &path, &object.tracker_data, p_grid_array, next_point_index);

   /** \assert expect the returned distance to be equal to the precalculated one. */
   EXPECT_FLOAT_EQ(
      path_obj_info.relevant_dist_comp_matching,
      0.5f * (path.path_points[PT_MID_GRID_POINT_INDEX] + path.path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]));
}

/**
 * Tests the calculation of relevant matching distance component. Here the object is moving longitudinad aligned with vcs and the
 * path is oriented longitudinal forward \uts{CSCSA-43752} \sdd{SF-7449} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calc_Relevant_Match_Dist_Comp__Obj_Moving_Long_Path_Aligned_With_Vcs)
{
   /** \arrange Set up an object which is moving longitudinal forward as well as a path with direction of longitudinal forward. */
   path.direction                                                          = PATH_DIRECTION_LONG_FORWARD;
   obj_move_dir                                                            = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   next_point_index                                                        = PT_MID_GRID_POINT_INDEX;
   path.path_points[PT_MID_GRID_POINT_INDEX]                               = 5.0f;
   path.path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] = 10.0f;
   object.tracker_data.vcs_pos                                             = Create_2d_Vector_Coordinates(
                                                  p_grid_array[PT_MID_GRID_POINT_INDEX] + 0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET], 0.0f);

   /** \action Calculate releveant matching distance component */
   Pt_Calc_Relevant_Match_Dist_Comp(&path_obj_info, &path, &object.tracker_data, p_grid_array, next_point_index);

   /** \assert expect the returned distance to be equal to the precalculated one. */
   EXPECT_FLOAT_EQ(
      path_obj_info.relevant_dist_comp_matching,
      0.5f * (path.path_points[PT_MID_GRID_POINT_INDEX] + path.path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]));
}


/**
 * Create a path and an object which moves aligned with vcs for a path which is also aligned with vcs.
 * \uts{CSCSA-43753} \sdd{SF-7453} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Heading_Diffs__obj_moves_right_and_starts_at_lowest_point_for_a_path_which_is_lat_right)
{

   /** \arrange setup path and object aligned in the same direction */
   uint8_t next_point_idx = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Path_Creation_Props_T path_properties;

   Pt_Init_Creation_Prop(&path_properties, PT_LOWEST_GRID_POINT_INDEX, PT_MID_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_RIGHT, PATH_STATUS_MATURE);
   Pt_Create_Path(&path, &path_properties, p_grid_array);

   path_obj_info.closest_to_successive_pt_heading = 0.5f * PI;
   object.tracker_data.vcs_heading                = path_obj_info.closest_to_successive_pt_heading - 0.1f;
   obj_move_dir                                   = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action Calculate heading differences */
   Pt_Calculate_Heading_Diffs(&path_obj_info, &pt_input, &object.tracker_data, &path, obj_move_dir, next_point_idx);

   /** \assert Check that each difference is filled. */
   for (uint8_t idx = 1; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      EXPECT_NEAR(path_obj_info.heading_diff_pt_segment[idx], 0.1f, EPSILON);
   }
}


/**
 * Create a path and an object which moves disaligned with vcs for a path which is also aligned with vcs.
 * \uts{CSCSA-43754} \sdd{SF-7453} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Calculate_Heading_Diffs__obj_moves_left_and_starts_between_point_1_and_2_for_a_path_which_is_lat_right)
{

   /** \arrange setup path and object aligned in different directions. */
   uint8_t next_point_idx = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Path_Creation_Props_T path_properties;

   Pt_Init_Creation_Prop(&path_properties, PT_LOWEST_GRID_POINT_INDEX, PT_MID_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_RIGHT, PATH_STATUS_MATURE);
   Pt_Create_Path(&path, &path_properties, p_grid_array);

   path_obj_info.closest_to_successive_pt_heading = -0.5f * PI;
   object.tracker_data.vcs_heading                = path_obj_info.closest_to_successive_pt_heading + 0.1f;
   obj_move_dir                                   = PT_OBJECT_MOV_DIR_LAT_LEFT;

   /** \action Calculate heading differences. */
   Pt_Calculate_Heading_Diffs(&path_obj_info, &pt_input, &object.tracker_data, &path, obj_move_dir, next_point_idx);

   /** \assert Check that each difference is filled. */
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[1], 0.0f);
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[2], 0.0f);
}


/**
 * Create a path in creation phase which is not covering the whole grid area. Only the first heading_difference is expected to be
 * filled. \uts{CSCSA-43755} \sdd{SF-7453} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Heading_Diffs__obj_moves_left_and_moves_at_left_border_for_a_path_which_is_lat_left)
{

   /** \arrange setup path and object aligned in same directions. */
   uint8_t next_point_idx = PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Path_Creation_Props_T path_properties;

   Pt_Init_Creation_Prop(&path_properties, PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET,
                         PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_CREATION);
   Pt_Create_Path(&path, &path_properties, p_grid_array);

   path_obj_info.closest_to_successive_pt_heading = -0.5f * PI;
   object.tracker_data.vcs_heading                = path_obj_info.closest_to_successive_pt_heading + 0.1f;
   obj_move_dir                                   = PT_OBJECT_MOV_DIR_LAT_LEFT;

   /** \action Calculate heading differences. */
   Pt_Calculate_Heading_Diffs(&path_obj_info, &pt_input, &object.tracker_data, &path, obj_move_dir, next_point_idx);

   /** \assert Check that only the first difference is filled. */
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[1], 0.0f);
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[2], 0.0f);
}


/**
 * Create a path in creation phase which is not covering the whole grid area. The object is moving aligned with coordinate system.
 * Only the first heading_difference is expected to be filled. filled. \uts{CSCSA-43756} \sdd{SF-7453}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Heading_Diffs__obj_moves_right_and_moves_at_right_border_for_a_path_which_is_lat_left)
{

   /** \arrange setup path and object aligned in different directions. */
   uint8_t next_point_idx = PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Path_Creation_Props_T path_properties;

   Pt_Init_Creation_Prop(&path_properties, PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET,
                         PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_CREATION);
   Pt_Create_Path(&path, &path_properties, p_grid_array);

   path_obj_info.closest_to_successive_pt_heading = 0.5f * PI;
   object.tracker_data.vcs_heading                = path_obj_info.closest_to_successive_pt_heading - 0.1f;
   obj_move_dir                                   = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action Calculate heading differences. */
   Pt_Calculate_Heading_Diffs(&path_obj_info, &pt_input, &object.tracker_data, &path, obj_move_dir, next_point_idx);

   /** \assert Check that only the first difference is filled. */
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[1], 0.0f);
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[2], 0.0f);
}


/**
 * Create a path in extrapolated phase. The object is moving disaligned with coordinate system. All heading_differences are
 * expected to be filled. filled. \uts{CSCSA-43757} \sdd{SF-7453} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Heading_Diffs__obj_moves_left_and_inside_of_the_right_border_for_a_path_which_is_lat_left)
{

   /** \arrange setup extrapolated path and object aligned in same direction. */
   uint8_t next_point_idx =
      PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_diff_path_points;
   Pt_Path_Creation_Props_T path_properties;

   Pt_Init_Creation_Prop(&path_properties, PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET,
                         PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Create_Path(&path, &path_properties, p_grid_array);
   Pt_Extrapolate_Path(&path, &cals, p_grid_array);

   path_obj_info.closest_to_successive_pt_heading = -0.5f * PI;
   object.tracker_data.vcs_heading                = path_obj_info.closest_to_successive_pt_heading + 0.1f;
   obj_move_dir                                   = PT_OBJECT_MOV_DIR_LAT_LEFT;

   /** \action Calculate heading differences. */
   Pt_Calculate_Heading_Diffs(&path_obj_info, &pt_input, &object.tracker_data, &path, obj_move_dir, next_point_idx);

   /** \assert Check that each difference is filled. */
   EXPECT_NEAR(path_obj_info.heading_diff_pt_segment[1], 0.1f, EPSILON);
   EXPECT_NEAR(path_obj_info.heading_diff_pt_segment[2], 0.1f, EPSILON);
}


/**
 * Create a path in extrapolated phase. The object is moving aligned with coordinate system. All heading differences are expected
 * to be filled. filled. \uts{CSCSA-43758} \sdd{SF-7453} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Heading_Diffs__obj_moves_right_and_inside_of_the_right_border_for_a_path_which_is_lat_left)
{

   /** \arrange setup extrapolated path and object aligned in same direction. */
   uint8_t next_point_idx =
      PT_MID_GRID_POINT_INDEX - 2 * PT_SINGLE_GRID_POINT_OFFSET - pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_diff_path_points;
   Pt_Path_Creation_Props_T path_properties;

   Pt_Init_Creation_Prop(&path_properties, PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET,
                         PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Create_Path(&path, &path_properties, p_grid_array);
   Pt_Extrapolate_Path(&path, &cals, p_grid_array);

   path_obj_info.closest_to_successive_pt_heading = 0.5f * PI;
   object.tracker_data.vcs_heading                = path_obj_info.closest_to_successive_pt_heading - 0.1f;
   obj_move_dir                                   = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action Calculate heading differences. */
   Pt_Calculate_Heading_Diffs(&path_obj_info, &pt_input, &object.tracker_data, &path, obj_move_dir, next_point_idx);

   /** \assert Check that each difference is filled. */
   EXPECT_NEAR(path_obj_info.heading_diff_pt_segment[1], 0.1f, EPSILON);
   EXPECT_NEAR(path_obj_info.heading_diff_pt_segment[2], 0.1f, EPSILON);
}


/**
 * Create a path in extrapolated phase. The object is moving aligned with coordinate system. Since object is nearly outside of the
 * extended path border, not all heading differences are expected to be filled. Only one heading is expected to be filled. filled.
 * \uts{CSCSA-43759} \sdd{SF-7453} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Calculate_Heading_Diffs__obj_moves_right_and_nearly_outside_of_the_of_the_right_border_for_a_path_which_is_lat_left)
{

   /** \arrange setup extrapolated path and object aligned in same direction. */
   uint8_t next_point_idx =
      PT_MID_GRID_POINT_INDEX + 2 * PT_SINGLE_GRID_POINT_OFFSET + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_diff_path_points;
   Pt_Path_Creation_Props_T path_properties;

   Pt_Init_Creation_Prop(&path_properties, PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET,
                         PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Create_Path(&path, &path_properties, p_grid_array);
   Pt_Extrapolate_Path(&path, &cals, p_grid_array);

   path_obj_info.closest_to_successive_pt_heading = 0.5f * PI;
   object.tracker_data.vcs_heading                = path_obj_info.closest_to_successive_pt_heading - 0.1f;
   obj_move_dir                                   = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action Calculate heading differences. */
   Pt_Calculate_Heading_Diffs(&path_obj_info, &pt_input, &object.tracker_data, &path, obj_move_dir, next_point_idx);

   /** \assert Check that only the first heading is filled. */
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[1], 0.0f);
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[2], 0.0f);
}


/**
 * Create a path in extrapolated phase. The object is moving aligned with coordinate system. Since object is nearly outside of the
 * extended path border, not all heading differences are expected to be filled. The first two headings are expected to be filled.
 * \uts{CSCSA-43760} \sdd{SF-7453} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Calculate_Heading_Diffs__obj_moves_right_and_is_2_points_away_from_extended_border_for_a_path_which_is_lat_left)
{

   /** \arrange setup extrapolated path and object aligned in different direction. */
   uint8_t next_point_idx =
      PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_diff_path_points;
   Pt_Path_Creation_Props_T path_properties;

   Pt_Init_Creation_Prop(&path_properties, PT_MID_GRID_POINT_INDEX - 3 * PT_SINGLE_GRID_POINT_OFFSET,
                         PT_MID_GRID_POINT_INDEX + 3 * PT_SINGLE_GRID_POINT_OFFSET, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Create_Path(&path, &path_properties, p_grid_array);
   Pt_Extrapolate_Path(&path, &cals, p_grid_array);

   path_obj_info.closest_to_successive_pt_heading = 0.5f * PI;
   object.tracker_data.vcs_heading                = path_obj_info.closest_to_successive_pt_heading - 0.1f;
   obj_move_dir                                   = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action Calculate heading differences. */
   Pt_Calculate_Heading_Diffs(&path_obj_info, &pt_input, &object.tracker_data, &path, obj_move_dir, next_point_idx);

   /** \assert Check that the first two heading differences are filled. */
   EXPECT_NEAR(path_obj_info.heading_diff_pt_segment[1], 0.1f, EPSILON);
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[2], 0.0f);
}


/**
 * All heading differences are filled so that no correction of weights shall be applied. Here the heading differences are equal to
 * the calibrations within the lookuptable. A weighted confidence value of all heading differences is expected. \uts{CSCSA-43761}
 * \sdd{SF-7452} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Heading_Diff_Confidence__all_heading_differences_are_filled_by_cals)
{

   /** \arrange Initialize heading differences. */
   float32_T denumerator = 0.0f;
   float32_T confidences[PT_NUM_HEADING_SEGMENTS];
   float32_T weights[PT_NUM_HEADING_SEGMENTS];
   float32_T exp_heading_diff_confidence = 0.0f;
   float32_T returned_heading_diff_confidence;

   for (uint8_t idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      path_obj_info.heading_diff_pt_segment[idx] = cals.k_pt_heading_diff_lut[idx];
      confidences[idx]                           = cals.k_pt_heading_diff_conf_lut[idx];
      weights[idx]                               = cals.k_pt_weight_heading_diff_confidence[idx];
      denumerator += weights[idx];
   }

   /** \action Calculate heading differences. */
   returned_heading_diff_confidence = Pt_Calculate_Heading_Diff_Confidence(&path_obj_info, &cals, object.tracker_data.index, 0u);

   /** \assert Check that the first two heading differences are filled. */
   for (uint8_t idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      exp_heading_diff_confidence += (weights[idx] / denumerator) * confidences[idx];
   }
   EXPECT_FLOAT_EQ(returned_heading_diff_confidence, exp_heading_diff_confidence);
}


/**
 * Only the next two heading differences shall be filled. Thus an adaption of weights shall be applied. Check that only the first
 * two heading differences are used for calculation of heading difference confidence. \uts{CSCSA-43762} \sdd{SF-7452}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Heading_Diff_Confidence__next_two_heading_differences_shall_be_used)
{

   /** \arrange Initialize heading differences. */
   float32_T denumerator                          = 0.0f;
   float32_T confidences[PT_NUM_HEADING_SEGMENTS] = {0.0f};
   float32_T weights[PT_NUM_HEADING_SEGMENTS]     = {0.0f};
   float32_T exp_heading_diff_confidence          = 0.0f;
   float32_T returned_heading_diff_confidence;

   for (uint8_t idx = 0; idx < PT_NUM_HEADING_SEGMENTS - 1; idx++)
   {
      path_obj_info.heading_diff_pt_segment[idx] = -cals.k_pt_heading_diff_lut[idx];
      confidences[idx]                           = cals.k_pt_heading_diff_conf_lut[idx];
      weights[idx]                               = cals.k_pt_weight_heading_diff_confidence[idx];
      denumerator += weights[idx];
   }

   /** \action Calculate heading difference confidence. */
   returned_heading_diff_confidence = Pt_Calculate_Heading_Diff_Confidence(&path_obj_info, &cals, object.tracker_data.index, 0u);

   /** \assert Check that only two headings are used. */
   for (uint8_t idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      exp_heading_diff_confidence += (weights[idx] / denumerator) * confidences[idx];
   }
   EXPECT_FLOAT_EQ(returned_heading_diff_confidence, exp_heading_diff_confidence);
}


/**
 * No heading difference was measured thus a default value for the confidence shall be returned.
 * \uts{CSCSA-43763} \sdd{SF-7452} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Calculate_Heading_Diff_Confidence__no_heading_differences_were_measured_thus_confidence_for_this_metric_shall_be_one)
{
   /** \arrange Initialize heading differences. */
   float32_T returned_heading_diff_confidence;

   for (uint8_t idx = 0; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      path_obj_info.heading_diff_pt_segment[idx] = 0.0f;
   }

   /** \action Calculate heading difference confidence. */
   returned_heading_diff_confidence = Pt_Calculate_Heading_Diff_Confidence(&path_obj_info, &cals, object.tracker_data.index, 0u);

   /** \assert Check that default value is returned */
   EXPECT_FLOAT_EQ(returned_heading_diff_confidence, 1.0f);
}


/**
 * No heading difference was measured thus a default value for the confidence shall be returned.
 * \uts{CSCSA-43764} \sdd{SF-7454} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Path_Segment_Heading_Diff__calculate_a_path_heading_segment)
{
   /** \arrange Initialize inputs */
   Pt_Path_Creation_Props_T path_properties;
   uint8_t segment_idx = PT_SINGLE_GRID_POINT_OFFSET;
   uint8_t idx_to_map  = 0u;

   Pt_Init_Creation_Prop(&path_properties, PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, -10.0f,
                         PATH_DIRECTION_LAT_RIGHT, PATH_STATUS_MATURE);
   Pt_Create_Path(&path, &path_properties, p_grid_array);

   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   /** \action Calculate path heading segment. */
   Pt_Calculate_Path_Segment_Heading_Diff(&path_obj_info, &path, p_grid_array, &object.tracker_data, segment_idx, idx_to_map,
                                          obj_move_dir);

   /** \assert Check that default value is returned */
   EXPECT_FLOAT_EQ(path_obj_info.heading_diff_pt_segment[0], 0.5f * PI);
}


/**
 * Checks functionality where two path matches are compared an where it is decided whether the current given path pair is better
 * than the previously best one. Here no match was previously given. Thus the minimum threshold for the confidence is exceeded.
 * \uts{CSCSA-43765} \sdd{SF-7471} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Obj_Pair_Better_Match__minimum_needed_confidence_is_exceeded_no_pair_was_given_before)
{
   /** \arrange Initialize inputs */
   boolean_T returned_res;

   path_obj_pair_confidence.confidence_factor_total = cals.k_pt_min_confidence_valid_match;
   best_path_object_pair.path_index                 = PT_DEFAULT_MATCH_INDEX;

   /** \action Check whether path is a better match. */
   returned_res = Pt_Is_Path_Obj_Pair_Better_Match(&path_obj_pair_confidence, &path_obj_info, &best_path_object_pair,
                                                   pt_persistent.paths, &object.tracker_data, &cals, 1u, PT_OBJECT_MOV_DIR_NONE);

   /** \assert Expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Checks functionality where two path matches are compared an where it is decided whether the current given path pair is better
 * than the previously best one. Previously a path has been selected as best one, so that now it shall update its confidence value.
 * \uts{CSCSA-43766} \sdd{SF-7471} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Obj_Pair_Better_Match__path_is_still_the_best_and_updates_its_confidence)
{
   /** \arrange Initialize inputs */
   boolean_T returned_res;

   path_obj_pair_confidence.confidence_factor_total                 = cals.k_pt_min_confidence_valid_match;
   best_path_object_pair.confidence_factor                          = 1.0f;
   best_path_object_pair.path_index                                 = 1;
   pt_persistent.paths[best_path_object_pair.path_index].path_index = 1;


   /** \action Check whether path is a better match. */
   returned_res = Pt_Is_Path_Obj_Pair_Better_Match(&path_obj_pair_confidence, &path_obj_info, &best_path_object_pair,
                                                   pt_persistent.paths, &object.tracker_data, &cals,
                                                   best_path_object_pair.path_index, PT_OBJECT_MOV_DIR_NONE);

   /** \assert Expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Checks functionality where two path matches are compared an where it is decided whether the current given path pair is better
 * than the previously best one. Here the path candidate is nearer to host and thus a path change based on nearer host is expected.
 * \uts{CSCSA-43767} \sdd{SF-7471} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Obj_Pair_Better_Match__path_candidate_is_nearer_compared_to_the_previous_best_match)
{
   /** \arrange Initialize pt_persistent.paths in such a way that path candidate 2 is nearer to the host and thus a better match */
   boolean_T returned_res;
   uint8_t path_candidate_idx = 2u;

   /*Set object properties*/
   obj_move_dir                = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(1.0f, -30.0f);

   /*Set properties of the nearer path.*/
   pt_persistent.paths[path_candidate_idx].first_p                                   = PT_MID_GRID_POINT_INDEX - 5u;
   pt_persistent.paths[path_candidate_idx].last_p                                    = PT_MID_GRID_POINT_INDEX + 5u;
   pt_persistent.paths[path_candidate_idx].direction                                 = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[path_candidate_idx].direction                                 = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[path_candidate_idx].path_state                                = PATH_STATUS_MATURE;
   pt_persistent.paths[path_candidate_idx].path_points[PT_MID_GRID_POINT_INDEX]      = 0.8f;
   pt_persistent.paths[path_candidate_idx].path_points[PT_MID_GRID_POINT_INDEX - 1u] = 0.8f;
   pt_persistent.paths[path_candidate_idx].path_points[PT_MID_GRID_POINT_INDEX - 2u] = 0.8f;
   path_obj_info.relevant_dist_comp_matching                                         = 2.0f;
   path_obj_pair_confidence.confidence_factor_total                                  = 1.1f * cals.k_pt_min_confidence_valid_match;

   /*Set properties for the currently best match choice.*/
   best_path_object_pair.path_index                                 = 1u;
   pt_persistent.paths[best_path_object_pair.path_index].first_p    = PT_MID_GRID_POINT_INDEX - 5u;
   pt_persistent.paths[best_path_object_pair.path_index].last_p     = PT_MID_GRID_POINT_INDEX + 5u;
   pt_persistent.paths[best_path_object_pair.path_index].direction  = PATH_DIRECTION_LAT_RIGHT;
   pt_persistent.paths[best_path_object_pair.path_index].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[best_path_object_pair.path_index].path_points[PT_MID_GRID_POINT_INDEX] =
      pt_persistent.paths[path_candidate_idx].path_points[PT_MID_GRID_POINT_INDEX]
      + 0.9f * cals.k_pt_dist_betw_paths_similarity_matching;
   pt_persistent.paths[best_path_object_pair.path_index].path_points[PT_MID_GRID_POINT_INDEX - 1u] =
      pt_persistent.paths[path_candidate_idx].path_points[PT_MID_GRID_POINT_INDEX - 1u]
      + 0.9f * cals.k_pt_dist_betw_paths_similarity_matching;
   pt_persistent.paths[best_path_object_pair.path_index].path_points[PT_MID_GRID_POINT_INDEX - 2u] =
      pt_persistent.paths[path_candidate_idx].path_points[PT_MID_GRID_POINT_INDEX - 2u]
      + 0.9f * cals.k_pt_dist_betw_paths_similarity_matching;
   best_path_object_pair.relevant_dist_comp_matching =
      path_obj_info.relevant_dist_comp_matching + 0.9f * cals.k_pt_dist_betw_paths_similarity_matching;

   /** \action Check whether path is a better match. */
   returned_res = Pt_Is_Path_Obj_Pair_Better_Match(&path_obj_pair_confidence, &path_obj_info, &best_path_object_pair,
                                                   pt_persistent.paths, &object.tracker_data, &cals, path_candidate_idx,
                                                   obj_move_dir);

   /** \assert Expect that path 2 is indeed a better match */
   EXPECT_TRUE(returned_res);
}


/**
 * Checks functionality where two path matches are compared an where it is decided whether the current given path pair is better
 * than the previously best one. Previously a path has been selected as best one, another path supersedes the previous match e
 * \uts{CSCSA-43768} \sdd{SF-7471} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Is_Path_Obj_Pair_Better_Match__another_path_has_a_greater_confidence_and_triggers_the_path_change_hysteresis)
{
   /** \arrange Initialize inputs */
   boolean_T returned_res;

   best_path_object_pair.confidence_factor = cals.k_pt_min_confidence_valid_match;
   path_obj_pair_confidence.confidence_factor_total =
      best_path_object_pair.confidence_factor + cals.k_pt_path_change_match_hyst_default;
   best_path_object_pair.path_index = 1;
   path.path_index                  = 2;


   /** \action Check whether path is a better match. */
   returned_res = Pt_Is_Path_Obj_Pair_Better_Match(&path_obj_pair_confidence, &path_obj_info, &best_path_object_pair,
                                                   pt_persistent.paths, &object.tracker_data, &cals, path.path_index,
                                                   PT_OBJECT_MOV_DIR_NONE);

   /** \assert Expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Checks functionality where two path matches are compared an where it is decided whether the current given path pair is better
 * than the previously best one. Another path object pair does not have a minimum needed confidence metric. Thus the pair is not
 * better and false is expected. \uts{CSCSA-43769} \sdd{SF-7471} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Is_Path_Obj_Pair_Better_Match__another_path_has_a_confidence_which_is_not_huge_enough_to_supersede_previous_match)
{
   /** \arrange Initialize inputs */
   boolean_T returned_res;

   best_path_object_pair.confidence_factor          = cals.k_pt_min_confidence_valid_match;
   path_obj_pair_confidence.confidence_factor_total = cals.k_pt_min_confidence_valid_match - EPSILON;
   best_path_object_pair.path_index                 = 1;
   path.path_index                                  = 2;


   /** \action Check whether path is a better match. */
   returned_res = Pt_Is_Path_Obj_Pair_Better_Match(&path_obj_pair_confidence, &path_obj_info, &best_path_object_pair,
                                                   pt_persistent.paths, &object.tracker_data, &cals, path.path_index,
                                                   PT_OBJECT_MOV_DIR_NONE);

   /** \assert Expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Checks functionality where two path matches are compared an where it is decided whether the current given path pair is better
 * than the previously best one. Another path object pair has the minimum needed confidence metric. However it is not exceeding the
 * previously best match. Thus false is expected. \uts{CSCSA-43771} \sdd{SF-7471} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Is_Path_Obj_Pair_Better_Match__path_has_minimum_confidence_but_does_not_update_or_exceed_previous_confidence_metric)
{
   /** \arrange Initialize inputs */
   boolean_T returned_res;

   best_path_object_pair.confidence_factor          = 1.0f;
   path_obj_pair_confidence.confidence_factor_total = cals.k_pt_min_confidence_valid_match;
   best_path_object_pair.path_index                 = 1;
   path.path_index                                  = 2;


   /** \action Check whether path is a better match. */
   returned_res = Pt_Is_Path_Obj_Pair_Better_Match(&path_obj_pair_confidence, &path_obj_info, &best_path_object_pair,
                                                   pt_persistent.paths, &object.tracker_data, &cals, path.path_index,
                                                   PT_OBJECT_MOV_DIR_NONE);

   /** \assert Expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Tests path object pair consumer information constructor. Check that it is correctly setup.
 * \uts{CSCSA-43778} \sdd{SF-7461} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Pt_Init_Path_Obj_Pair_Consumer_Info__constructor_is_tested)
{
   /** \arrange Initialize inputs */
   Pt_Path_Obj_Pair_Consumer_Info_T consumer_info;

   /** \action Call constructor */
   Pt_Pt_Init_Path_Obj_Pair_Consumer_Info(&consumer_info);

   /** \assert Expect that it is initialized correctly */
   EXPECT_EQ(consumer_info.path_index, PT_DEFAULT_MATCH_INDEX);
   EXPECT_FLOAT_EQ(consumer_info.distance_to_path, 90.0f);
   EXPECT_FLOAT_EQ(consumer_info.segment_heading_diff, 0.0f);
}


/**
 * Tests path object pair information constructor. Check that it is correctly setup.
 * \uts{CSCSA-43789} \sdd{SF-7462} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Pt_Init_Path_Obj_Pair_Info__constructor_is_tested)
{
   /** \arrange Inputs are already initialzed in test fixture class */

   /** \action Call constructor */
   Pt_Pt_Init_Path_Obj_Pair_Info(&path_obj_info);

   /** \assert Expect that it is initialized correctly */
   EXPECT_FLOAT_EQ(path_obj_info.closest_to_successive_pt_heading, 0.0f);
   EXPECT_EQ(path_obj_info.border_info.dist_border_to_mid, PT_DEFAULT_DISCR_BORDER);
   EXPECT_EQ(path_obj_info.border_info.dist_border_to_obj, PT_DEFAULT_DISCR_BORDER);
}


/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Here the current best match is
 * performing a lane change to the 2nd lane on host level. Point at PT_MID_GRID_POINT_INDEX - 2 PT_SINGLE_GRID_POINT_OFFSET is
 * fulfilling the similarity condition. Since also the mid grid point is within the each path, and the distance between object an
 * both pt_persistent.paths is lower than a threshold, true is expected. \uts{CSCSA-43792} \sdd{SF-7447}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Are_Paths_Similar_At_Host_And_Object_Level__checked_for_rcta_cases_current_best_match_does_a_lane_change_on_height_of_host_obj_moving_aligned_with_vcs)
{
   /** \arrange Setup two pt_persistent.paths which are similar based on the analysis of function to be called */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T path_properties;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;

   Pt_Init_Creation_Prop(&path_properties, PT_LOWEST_GRID_POINT_INDEX, PT_MID_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, -8.0f,
                         PATH_DIRECTION_LAT_RIGHT, PATH_STATUS_MATURE);

   /*Setup object properties moving lateral right*/
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.vcs_pos.y = -EPSILON;

   /*Create the currently best match does a lane change at host level on 2nd lane*/
   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair.path_index], &path_properties, p_grid_array);
   pt_persistent.paths[best_path_object_pair.path_index].path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] -= 1.0f;
   pt_persistent.paths[best_path_object_pair.path_index].path_points[PT_MID_GRID_POINT_INDEX] -= 2.0f;

   /*Create candidate path*/
   path_properties.factor_const += (cals.k_pt_dist_betw_paths_similarity_matching - EPSILON);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &path_properties, p_grid_array);

   path_obj_info.relevant_dist_comp_matching = 0.4f;
   best_path_object_pair.relevant_dist_comp_matching =
      path_obj_info.relevant_dist_comp_matching + (cals.k_pt_dist_betw_paths_similarity_matching - EPSILON);

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Here the current best match is
 * performing a lane change to the 2nd lane on host level. Point at PT_MID_GRID_POINT_INDEX - 2 PT_SINGLE_GRID_POINT_OFFSET is
 * fulfilling the similarity condition. Since also the mid grid point is within the each path, and the distance between object an
 * both pt_persistent.paths is lower than a threshold, true is expected. This tests the function for fcta cases and lateral left
 * path. \uts{CSCSA-43793} \sdd{SF-7447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Are_Paths_Similar_At_Host_And_Object_Level__checked_for_fcta_cases_current_best_match_does_a_lane_change_on_height_of_host_obj_moving_against_vcs)
{
   /** \arrange Setup two pt_persistent.paths which are similar based on the analysis of function to be called */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T path_properties;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;

   Pt_Init_Creation_Prop(&path_properties, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   /*Setup object properties moving lateral left*/
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = EPSILON;

   /*Create the currently best match does a lane change at host level on 2nd lane*/
   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair.path_index], &path_properties, p_grid_array);
   pt_persistent.paths[best_path_object_pair.path_index].path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] += 1.0f;
   pt_persistent.paths[best_path_object_pair.path_index].path_points[PT_MID_GRID_POINT_INDEX] += 2.0f;

   /*Create candidate path*/
   path_properties.factor_const -= (cals.k_pt_dist_betw_paths_similarity_matching - EPSILON);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &path_properties, p_grid_array);

   path_obj_info.relevant_dist_comp_matching = 0.4f;
   best_path_object_pair.relevant_dist_comp_matching =
      path_obj_info.relevant_dist_comp_matching + (cals.k_pt_dist_betw_paths_similarity_matching - EPSILON);

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Here no valid object moving
 * direction is given, thus false is expected. \uts{CSCSA-43794} \sdd{SF-7447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Similar_At_Host_And_Object_Level__object_properties_are_invalid)
{
   /** \arrange Setup uninitialized object moving direction */
   boolean_T returned_res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;
   obj_move_dir                     = PT_OBJECT_MOV_DIR_NONE;

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Here the mid grid point is not
 * included within path candidates boundaries, thus false is expected. \uts{CSCSA-43795} \sdd{SF-7447}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Similar_At_Host_And_Object_Level__candidate_path_lower_border_gt_mid_grid_pt_idx)
{
   /** \arrange setup path candidates boundaries to invalid parameters */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;

   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET, PT_HIGHEST_GRID_POINT_INDEX,
                         0.0f, 0.0f, 0.0f, 8.0f, PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   /*Setup object properties moving lateral left*/
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = EPSILON;

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair.path_index], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}

/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Here the mid grid point is not
 * included within path candidates boundaries, thus false is expected. \uts{CSCSA-43796} \sdd{SF-7447}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Similar_At_Host_And_Object_Level__candidate_path_upper_border_lt_mid_grid_pt_idx)
{
   /** \arrange setup path candidates boundaries to invalid parameters */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;

   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET,
                         PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET, 0.0f, 0.0f, 0.0f, 8.0f, PATH_DIRECTION_LAT_LEFT,
                         PATH_STATUS_MATURE);

   /*Setup object properties moving lateral left*/
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = EPSILON;

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair.path_index], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Here the mid grid point is not
 * included within best path boundaries, thus false is expected. \uts{CSCSA-43797} \sdd{SF-7447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Similar_At_Host_And_Object_Level__best_path_lower_border_gt_mid_grid_pt_idx)
{
   /** \arrange setup best path boundaries to invalid parameters */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;

   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET, PT_HIGHEST_GRID_POINT_INDEX,
                         0.0f, 0.0f, 0.0f, 8.0f, PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   /*Setup object properties moving lateral left*/
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = EPSILON;

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair.path_index], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Here the mid grid point is not
 * included within best path boundaries, thus false is expected. \uts{CSCSA-43798} \sdd{SF-7447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Similar_At_Host_And_Object_Level__best_path_upper_border_lt_mid_grid_pt_idx)
{
   /** \arrange setup best path boundaries to invalid parameters */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;

   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET,
                         PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET, 0.0f, 0.0f, 0.0f, 8.0f, PATH_DIRECTION_LAT_LEFT,
                         PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   /*Setup object properties moving lateral left*/
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = EPSILON;

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair.path_index], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Paths are not similar at object
 * level. Thus false is expected. \uts{CSCSA-43799} \sdd{SF-7447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Similar_At_Host_And_Object_Level__paths_are_not_similar_at_object_level)
{
   /** \arrange setup distance to pt_persistent.paths at object level to invalid parameters */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;

   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f,
                         candidate_path_props.factor_const - cals.k_pt_dist_betw_paths_similarity_matching,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   /*Setup object properties moving lateral left*/
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = EPSILON;

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair.path_index], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   path_obj_info.relevant_dist_comp_matching = 0.4f;
   best_path_object_pair.relevant_dist_comp_matching =
      path_obj_info.relevant_dist_comp_matching - (cals.k_pt_dist_betw_paths_similarity_matching + EPSILON);

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Test the check for similarity of pt_persistent.paths when it comes to the matching procedure. Paths are not similar on host
 * level. \uts{CSCSA-43800} \sdd{SF-7447} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Similar_At_Host_And_Object_Level__paths_are_not_similar_at_host_level)
{
   /** \arrange setup path points to invalid values */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2;

   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f,
                         candidate_path_props.factor_const - (cals.k_pt_dist_betw_paths_similarity_matching + EPSILON),
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   /*Setup object properties moving lateral left*/
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = EPSILON;

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair.path_index], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   path_obj_info.relevant_dist_comp_matching = 0.4f;
   best_path_object_pair.relevant_dist_comp_matching =
      path_obj_info.relevant_dist_comp_matching + (cals.k_pt_dist_betw_paths_similarity_matching - EPSILON);

   /** \action Call function to check similarity of pt_persistent.paths */
   returned_res = Pt_Are_Paths_Similar_At_Host_And_Object_Level(&path_obj_info, &best_path_object_pair, pt_persistent.paths, &cals,
                                                                path_candidate_idx, obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Tests whether the given scenario is valid for the nearest path match. Thus true is expected.
 * \uts{CSCSA-43801} \sdd{SF-7572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Valid_For_Nearest_Path_Match__scenario_valid)
{
   /** \arrange setup pt_persistent.paths to valid state and object valid for a path similarity check. */
   boolean_T res;
   uint8_t path_candidate_idx       = 1u;
   obj_move_dir                     = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   best_path_object_pair.path_index = 2u;
   object.tracker_data.vcs_pos.x    = -EPSILON;

   pt_persistent.paths[path_candidate_idx].path_state               = PATH_STATUS_MATURE;
   pt_persistent.paths[best_path_object_pair.path_index].path_state = PATH_STATUS_MATURE;

   /** \action Call function to check whether pt_persistent.paths are valid for nearest path match. */
   res = Pt_Are_Paths_Valid_For_Nearest_Path_Match(&best_path_object_pair, pt_persistent.paths, &object.tracker_data,
                                                   path_candidate_idx, obj_move_dir);

   /** \assert expect true */
   EXPECT_TRUE(res);
}


/**
 * Tests whether the given scenario is invalid for the nearest path match due to the currently best path to be set to creation
 * state. Thus false is expected. \uts{CSCSA-43802} \sdd{SF-7572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Valid_For_Nearest_Path_Match__scenario_invalid_since_best_path_is_in_creation)
{
   /** \arrange setup pt_persistent.paths to invalid state and object valid for a path similarity check. */
   boolean_T res;
   uint8_t path_candidate_idx       = 1u;
   obj_move_dir                     = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   best_path_object_pair.path_index = 2u;
   object.tracker_data.vcs_pos.x    = -EPSILON;

   pt_persistent.paths[path_candidate_idx].path_state               = PATH_STATUS_MATURE;
   pt_persistent.paths[best_path_object_pair.path_index].path_state = PATH_STATUS_CREATION;

   /** \action Call function to check whether pt_persistent.paths are valid for nearest path match. */
   res = Pt_Are_Paths_Valid_For_Nearest_Path_Match(&best_path_object_pair, pt_persistent.paths, &object.tracker_data,
                                                   path_candidate_idx, obj_move_dir);

   /** \assert expect false */
   EXPECT_FALSE(res);
}


/**
 * Tests whether the given scenario is invalid for the nearest path match due no currently given best path. Thus false is expected.
 * \uts{CSCSA-43803} \sdd{SF-7572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Are_Paths_Valid_For_Nearest_Path_Match__scenario_invalid_since_best_path_is_not_given_yet)
{
   /** \arrange setup pt_persistent.paths best path to uninitialized path index. */
   boolean_T res;
   uint8_t path_candidate_idx       = 1u;
   obj_move_dir                     = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   best_path_object_pair.path_index = PT_DEFAULT_MATCH_INDEX;
   object.tracker_data.vcs_pos.x    = -EPSILON;

   pt_persistent.paths[path_candidate_idx].path_state = PATH_STATUS_MATURE;

   /** \action Call function to check whether pt_persistent.paths are valid for nearest path match. */
   res = Pt_Are_Paths_Valid_For_Nearest_Path_Match(&best_path_object_pair, pt_persistent.paths, &object.tracker_data,
                                                   path_candidate_idx, obj_move_dir);

   /** \assert expect false */
   EXPECT_FALSE(res);
}


/**
 * Tests whether the correct hysteresis for path match changes is returned. Here the currently best path has an higher amount of
 * grouping. and thus a positive hysteresis is expected to be returned. \uts{CSCSA-43804} \sdd{SF-7571}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Return_Confidence_Hysteresis_For_Path_Change__best_path_groupings_gt_input_paths_groupings)
{
   /** \arrange setup pt_persistent.paths such that a positive hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 2u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_GROUPED;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = pt_persistent.paths[path_candidate_idx].num_groupings + 1u;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_GROUPED;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a positive hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, cals.k_pt_path_change_match_hyst_more_established);
}


/**
 * Tests whether the correct hysteresis for path match changes is returned. Path candidate is in creation phase, thus the most
 * strict hysteresis shall be returned. \uts{CSCSA-43805} \sdd{SF-7571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Return_Confidence_Hysteresis_For_Path_Change__path_candidate_is_in_creation_phase)
{
   /** \arrange setup pt_persistent.paths such that a positive hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 0u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_CREATION;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = 2u;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_GROUPED;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a positive hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, cals.k_pt_path_change_one_grouped_one_creation);
}

/**
 * Tests whether the correct hysteresis for path match changes is returned. Path candidate is in mature phase, thus the most strict
 * hysteresis shall be returned. \uts{CSCSA-43806} \sdd{SF-7571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Return_Confidence_Hysteresis_For_Path_Change__path_candidate_is_in_mature_phase)
{
   /** \arrange setup pt_persistent.paths such that a positive hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 0u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_MATURE;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = 2u;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_GROUPED;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a positive hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, cals.k_pt_path_change_one_grouped_one_mature);
}


/**
 * Tests whether the correct hysteresis for path match changes is returned. Here the currently best path has a lower amount of
 * grouping. and thus a negative hysteresis is expected to be returned. \uts{CSCSA-43807} \sdd{SF-7571}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Return_Confidence_Hysteresis_For_Path_Change__best_path_groupings_lt_input_paths_groupings)
{
   /** \arrange setup pt_persistent.paths such that a negative hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 2u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_GROUPED;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = pt_persistent.paths[path_candidate_idx].num_groupings - 1u;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_GROUPED;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a negative hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, -cals.k_pt_path_change_match_hyst_more_established);
}


/**
 * Tests whether the correct hysteresis for path match changes is returned. Here the currently best path is in creation phase, and
 * thus a negative hysteresis is expected to be returned. \uts{CSCSA-43808} \sdd{SF-7571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Return_Confidence_Hysteresis_For_Path_Change__best_path_in_creation_phase)
{
   /** \arrange setup pt_persistent.paths such that a negative hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 2u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_GROUPED;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = 0u;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_CREATION;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a negative hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, -cals.k_pt_path_change_one_grouped_one_creation);
}


/**
 * Tests whether the correct hysteresis for path match changes is returned. Here the currently best path is in mature state, and
 * thus a negative hysteresis is expected to be returned. \uts{CSCSA-43809} \sdd{SF-7571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Return_Confidence_Hysteresis_For_Path_Change__best_path_in_mature_state)
{
   /** \arrange setup pt_persistent.paths such that a negative hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 2u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_GROUPED;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = 0u;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_MATURE;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a negative hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, -cals.k_pt_path_change_one_grouped_one_mature);
}

/**
 * Tests whether the correct hysteresis for path match changes is returned. Here both pt_persistent.paths have the same amount of
 * groupings. Thus a default hysteresis shall be returned. \uts{CSCSA-43810} \sdd{SF-7571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Return_Confidence_Hysteresis_For_Path_Change__return_default_hysteresis_due_to_equally_grouping_amount)
{
   /** \arrange setup pt_persistent.paths such that a default hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 2u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_GROUPED;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = pt_persistent.paths[path_candidate_idx].num_groupings;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_GROUPED;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a default hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, cals.k_pt_path_change_match_hyst_default);
}


/**
 * Tests whether the correct hysteresis for path match changes is returned. Here both pt_persistent.paths have the same amount of
 * groupings, and the current best path is in creation mode while the path candidate is in mature state. Thus a negative hysteresis
 * is expected. \uts{CSCSA-43811} \sdd{SF-7571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Return_Confidence_Hysteresis_For_Path_Change__return_negative_hysteresis_for_equally_established_paths_with_differing_states)
{
   /** \arrange setup pt_persistent.paths such that a default hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 0u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_MATURE;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = pt_persistent.paths[path_candidate_idx].num_groupings;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_CREATION;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a negative default hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, -cals.k_pt_path_change_differing_states_hyst_default);
}


/**
 * Tests whether the correct hysteresis for path match changes is returned. Here both pt_persistent.paths have the same amount of
 * groupings, and the current best path is in mature mode while the path candidate is in creation state. Thus a positive hysteresis
 * is expected. \uts{CSCSA-43812} \sdd{SF-7571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Return_Confidence_Hysteresis_For_Path_Change__return_positive_hysteresis_for_equally_established_paths_with_differing_states)
{
   /** \arrange setup pt_persistent.paths such that a default hysteresis is expected. */
   float32_T res;
   uint8_t path_candidate_idx       = 1u;
   best_path_object_pair.path_index = 2u;

   pt_persistent.paths[path_candidate_idx].num_groupings               = 0u;
   pt_persistent.paths[path_candidate_idx].path_state                  = PATH_STATUS_CREATION;
   pt_persistent.paths[best_path_object_pair.path_index].num_groupings = pt_persistent.paths[path_candidate_idx].num_groupings;
   pt_persistent.paths[best_path_object_pair.path_index].path_state    = PATH_STATUS_MATURE;

   /** \action Call function for confidence hysteresis calculation. */
   res = Pt_Return_Confidence_Hysteresis_For_Path_Change(&best_path_object_pair, pt_persistent.paths, &cals, path_candidate_idx);

   /** \assert expect a positive default hysteresis to be returned. */
   EXPECT_FLOAT_EQ(res, cals.k_pt_path_change_differing_states_hyst_default);
}

/**
 * Tests whether other matching method based on path similiarities is applicable for given object.tracker_data. Object moves
 * lateral left and has not crossed axis yet. Thus true is expected. \uts{CSCSA-43813} \sdd{SF-7468}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Valid_For_Path_Similarity_Check__lateral_left_not_crossed_axis)
{
   /** \arrange object moving lateral left before crossig vcs axis */
   boolean_T returned_res;
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = EPSILON;
   /** \action Call function to check whether object is valid for path similarity check */
   returned_res = Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Tests whether other matching method based on path similiarities is applicable for given object.tracker_data. Object moves
 * lateral right and has not crossed axis yet. Thus true is expected. \uts{CSCSA-43814} \sdd{SF-7468}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Valid_For_Path_Similarity_Check__lateral_right_not_crossed_axis)
{
   /** \arrange object moving lateral right before crossig vcs axis */
   boolean_T returned_res;
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.vcs_pos.y = -EPSILON;
   /** \action Call function to check whether object is valid for path similarity check */
   returned_res = Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Tests whether other matching method based on path similiarities is applicable for given object.tracker_data. Object moves
 * longitudinal forward and has not crossed axis yet. Thus true is expected. \uts{CSCSA-43815} \sdd{SF-7468}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Valid_For_Path_Similarity_Check__longidutinal_forward_not_crossed_axis)
{
   /** \arrange object moving longitudinal forward before crossig vcs axis */
   boolean_T returned_res;
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   object.tracker_data.vcs_pos.x = -EPSILON;
   /** \action Call function to check whether object is valid for path similarity check */
   returned_res = Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Tests whether other matching method based on path similiarities is applicable for given object.tracker_data. Object moves
 * longitudinal backward and has not crossed axis yet. Thus true is expected. \uts{CSCSA-43816} \sdd{SF-7468}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Valid_For_Path_Similarity_Check__longidutinal_backward_not_crossed_axis)
{
   /** \arrange object moving longitudinal backward before crossig vcs axis */
   boolean_T returned_res;
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LONG_BACKWARD;
   object.tracker_data.vcs_pos.x = EPSILON;
   /** \action Call function to check whether object is valid for path similarity check */
   returned_res = Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, &object.tracker_data);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Tests whether other matching method based on path similiarities is applicable for given object.tracker_data. Object moves
 * lateral left and has crossed axis. Thus false is expected. \uts{CSCSA-43817} \sdd{SF-7468} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Valid_For_Path_Similarity_Check__lateral_left_crossed_axis)
{
   /** \arrange object moving lateral left has crossed vcs axis */
   boolean_T returned_res;
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_LEFT;
   object.tracker_data.vcs_pos.y = -EPSILON;
   /** \action Call function to check whether object is valid for path similarity check */
   returned_res = Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Tests whether other matching method based on path similiarities is applicable for given object.tracker_data. Object moves
 * lateral right and has crossed axis. Thus false is expected. \uts{CSCSA-43818} \sdd{SF-7468} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Valid_For_Path_Similarity_Check__lateral_right_crossed_axis)
{
   /** \arrange object moving lateral right has crossed vcs axis */
   boolean_T returned_res;
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.vcs_pos.y = EPSILON;
   /** \action Call function to check whether object is valid for path similarity check */
   returned_res = Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Tests whether other matching method based on path similiarities is applicable for given object.tracker_data. Object moves
 * longitudinal forward and has crossed axis. Thus false is expected. \uts{CSCSA-43819} \sdd{SF-7468}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Valid_For_Path_Similarity_Check__longidutinal_forward_crossed_axis)
{
   /** \arrange object moving longitudinal forward has crossed vcs axis */
   boolean_T returned_res;
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   object.tracker_data.vcs_pos.x = EPSILON;
   /** \action Call function to check whether object is valid for path similarity check */
   returned_res = Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Tests whether other matching method based on path similiarities is applicable for given object.tracker_data. Object moves
 * longitudinal backward and has crossed axis. Thus false is expected. \uts{CSCSA-43820} \sdd{SF-7468}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Obj_Valid_For_Path_Similarity_Check__longidutinal_backward_crossed_axis)
{
   /** \arrange object moving longitudinal backward has crossed vcs axis */
   boolean_T returned_res;
   obj_move_dir                  = PT_OBJECT_MOV_DIR_LONG_BACKWARD;
   object.tracker_data.vcs_pos.x = -EPSILON;
   /** \action Call function to check whether object is valid for path similarity check */
   returned_res = Pt_Is_Obj_Valid_For_Path_Similarity_Check(obj_move_dir, &object.tracker_data);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Tests whether path candidate shall be taken, since it is nearer to the host. Object is moving against vcs. Here this is the case
 * and true is expected. \uts{CSCSA-43821} \sdd{SF-7470} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Candidate_Nearer_To_Host__obj_moves_against_vcs_path_candidate_is_nearer_to_host)
{
   /** \arrange object moving against vcs and path */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx        = 1u;
   uint8_t best_path_object_pair_idx = 2u;

   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;

   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f,
                         candidate_path_props.factor_const + 1.0f, PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair_idx], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check whether path candidate is closer to host */
   returned_res =
      Pt_Is_Path_Candidate_Nearer_To_Host(pt_persistent.paths, obj_move_dir, path_candidate_idx, best_path_object_pair_idx);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Tests whether path candidate shall be taken, since it is nearer to the host. Object is moving with vcs. Here this is the case
 * and true is expected. \uts{CSCSA-43822} \sdd{SF-7470} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Candidate_Nearer_To_Host__obj_moves_aligned_with_vcs_path_candidate_is_nearer_to_host)
{
   /** \arrange object moving with vcs and path */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx        = 1u;
   uint8_t best_path_object_pair_idx = 2u;

   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f,
                         candidate_path_props.factor_const + 1.0f, PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair_idx], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check whether path candidate is closer to host */
   returned_res =
      Pt_Is_Path_Candidate_Nearer_To_Host(pt_persistent.paths, obj_move_dir, path_candidate_idx, best_path_object_pair_idx);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Tests whether path candidate shall be taken, since it is nearer to the host. Object is moving against vcs. Here this is the case
 * and true is expected. \uts{CSCSA-43823} \sdd{SF-7470} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Is_Path_Candidate_Nearer_To_Host__obj_moves_against_vcs_path_candidate_is_nearer_to_host_negative_path_points)
{
   /** \arrange object moving against vcs and path */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx        = 1u;
   uint8_t best_path_object_pair_idx = 2u;

   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_LEFT;

   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, -8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f,
                         candidate_path_props.factor_const - 1.0f, PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair_idx], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check whether path candidate is closer to host */
   returned_res =
      Pt_Is_Path_Candidate_Nearer_To_Host(pt_persistent.paths, obj_move_dir, path_candidate_idx, best_path_object_pair_idx);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Tests whether path candidate shall be taken, since it is nearer to the host. Object is moving with vcs. Here this is the case
 * and true is expected. \uts{CSCSA-43824} \sdd{SF-7470} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Is_Path_Candidate_Nearer_To_Host__obj_moves_aligned_with_vcs_path_candidate_is_nearer_to_host_negative_path_points)
{
   /** \arrange object moving with vcs and path */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx        = 1u;
   uint8_t best_path_object_pair_idx = 2u;

   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, -8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f,
                         candidate_path_props.factor_const - 1.0f, PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair_idx], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check whether path candidate is closer to host */
   returned_res =
      Pt_Is_Path_Candidate_Nearer_To_Host(pt_persistent.paths, obj_move_dir, path_candidate_idx, best_path_object_pair_idx);

   /** \assert expect true */
   EXPECT_TRUE(returned_res);
}


/**
 * Tests whether path candidate shall be taken, since it is nearer to the host. Object is moving with vcs. Here the best path is
 * nearer to the host than the current path candidate. Thus false is expected \uts{CSCSA-43825} \sdd{SF-7470}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Path_Candidate_Nearer_To_Host__path_candidate_is_farer_away_from_host)
{
   /** \arrange object moving with vcs and path */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx        = 1u;
   uint8_t best_path_object_pair_idx = 2u;

   obj_move_dir = PT_OBJECT_MOV_DIR_LAT_RIGHT;

   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f,
                         candidate_path_props.factor_const - 1.0f, PATH_DIRECTION_LAT_LEFT, PATH_STATUS_MATURE);

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair_idx], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check whether path candidate is closer to host */
   returned_res =
      Pt_Is_Path_Candidate_Nearer_To_Host(pt_persistent.paths, obj_move_dir, path_candidate_idx, best_path_object_pair_idx);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Tests whether path candidate shall be taken, since it is nearer to the host. Object is moving against vcs longitudinally. Here
 * the best path is nearer to the host than the current path candidate. Thus false is expected \uts{CSCSA-43826} \sdd{SF-7470}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Is_Path_Candidate_Nearer_To_Host__path_candidate_is_farer_away_from_host_for_obj_moving_long_backwards)
{
   /** \arrange object moving against vcs and path */
   boolean_T returned_res;
   Pt_Path_Creation_Props_T best_path_props, candidate_path_props;
   uint8_t path_candidate_idx        = 1u;
   uint8_t best_path_object_pair_idx = 2u;

   obj_move_dir = PT_OBJECT_MOV_DIR_LONG_BACKWARD;

   Pt_Init_Creation_Prop(&candidate_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f, 8.0f,
                         PATH_DIRECTION_LONG_BACKWARD, PATH_STATUS_MATURE);
   Pt_Init_Creation_Prop(&best_path_props, PT_MID_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0.0f, 0.0f, 0.0f,
                         candidate_path_props.factor_const - 1.0f, PATH_DIRECTION_LONG_BACKWARD, PATH_STATUS_MATURE);

   Pt_Create_Path(&pt_persistent.paths[best_path_object_pair_idx], &best_path_props, p_grid_array);
   Pt_Create_Path(&pt_persistent.paths[path_candidate_idx], &candidate_path_props, p_grid_array);

   /** \action Call function to check whether path candidate is closer to host */
   returned_res =
      Pt_Is_Path_Candidate_Nearer_To_Host(pt_persistent.paths, obj_move_dir, path_candidate_idx, best_path_object_pair_idx);

   /** \assert expect false */
   EXPECT_FALSE(returned_res);
}


/**
 * Tests boundary functionality of the object matching module. Here the given index is within the internal boundaries of the
 * function. Thus true is expected. \uts{CSCSA-43827} \sdd{SF-7466} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Next_Point_Idx_Valid__point_idx_in_boundary)
{
   /** \arrange Setup the input indices */
   Pt_Point_Indices_Dir_Indep_Obj_T point_idx;
   point_idx.next_point_idx_lat_path  = PT_MID_GRID_POINT_INDEX;
   point_idx.next_point_idx_long_path = PT_MID_GRID_POINT_INDEX;
   boolean_T result;

   /** \action Call function to check whether index is in allowed boundaries */
   result = Pt_Is_Next_Point_Idx_Valid(&point_idx);

   /** \assert expect true */
   EXPECT_TRUE(result);
}


/**
 * Tests boundary functionality of the object matching module. Here the given index is equal to the lower boundary. Thus false is
 * expected. \uts{CSCSA-43828} \sdd{SF-7466} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Next_Point_Idx_Valid__point_idx_eq_lower_boundary)
{
   /** \arrange Setup the input indices */
   Pt_Point_Indices_Dir_Indep_Obj_T point_idx;
   point_idx.next_point_idx_lat_path  = PT_LOWEST_GRID_POINT_INDEX;
   point_idx.next_point_idx_long_path = PT_LOWEST_GRID_POINT_INDEX;
   boolean_T result;

   /** \action Call function to check whether index is in allowed boundaries */
   result = Pt_Is_Next_Point_Idx_Valid(&point_idx);

   /** \assert expect false */
   EXPECT_FALSE(result);
}

/**
 * Tests boundary functionality of the object matching module. Here the given index is equal to the upper boundary. Thus false is
 * expected. \uts{CSCSA-43829} \sdd{SF-7466} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Next_Point_Idx_Valid__point_idx_eq_upper_boundary)
{
   /** \arrange Setup the input index */
   Pt_Point_Indices_Dir_Indep_Obj_T point_idx;
   point_idx.next_point_idx_lat_path  = PT_HIGHEST_GRID_POINT_INDEX;
   point_idx.next_point_idx_long_path = PT_HIGHEST_GRID_POINT_INDEX;
   boolean_T result;

   /** \action Call function to check whether index is in allowed boundaries */
   result = Pt_Is_Next_Point_Idx_Valid(&point_idx);

   /** \assert expect false */
   EXPECT_FALSE(result);
}

/**
 * Tests distance calculation of matching module. Here the object is moving longitudinal and thus the formula for the longitudinal
 * calculation is expected to be used. \uts{CSCSA-43830} \sdd{SF-7451} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Distance_Between_Object_And_Path__object_is_moving_longitudinal)
{
   /** \arrange Setup scenario with longitudinal path and longitudinal moving object */
   float32_T path_val     = 1.0f;
   uint8_t next_point_idx = PT_MID_GRID_POINT_INDEX;
   obj_move_dir           = PT_OBJECT_MOV_DIR_LONG_FORWARD;
   object.tracker_data.vcs_pos =
      Create_2d_Vector_Coordinates(p_grid_array[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]
                                      + 0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET],
                                   path_val);
   path_obj_info.closest_to_successive_pt_heading = 0.0f;
   path.path_points[next_point_idx]               = path_val;

   /** \action Call function for radial distance calculation */
   Pt_Calculate_Distance_Between_Object_And_Path(&path_obj_info, &path, &object.tracker_data, next_point_idx, obj_move_dir,
                                                 p_grid_array);

   /** \assert expect radial distance to consist of the difference between lateral position component only and path point */
   EXPECT_FLOAT_EQ(path_obj_info.rad_object_to_path_dist, object.tracker_data.vcs_pos.y - path.path_points[next_point_idx]);
}


/**
 * Tests distance calculation of matching module. Here the object is moving lateral and thus the formula for the lateral
 * calculation is expected to be used. \uts{CSCSA-43831} \sdd{SF-7451} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Calculate_Distance_Between_Object_And_Path__object_is_moving_lateral)
{
   /** \arrange Setup scenario with longitudinal path and longitudinal moving object */
   float32_T path_val     = 1.0f;
   uint8_t next_point_idx = PT_MID_GRID_POINT_INDEX;
   obj_move_dir           = PT_OBJECT_MOV_DIR_LAT_RIGHT;
   object.tracker_data.vcs_pos =
      Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET]
                                                + 0.5f * p_grid_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET]);
   path_obj_info.closest_to_successive_pt_heading = 0.5f * PI;
   path.path_points[next_point_idx]               = path_val;

   /** \action Call function for radial distance calculation */
   Pt_Calculate_Distance_Between_Object_And_Path(&path_obj_info, &path, &object.tracker_data, next_point_idx, obj_move_dir,
                                                 p_grid_array);

   /** \assert expect radial distance to consist of the difference between longitudinal position component only and path point */
   EXPECT_FLOAT_EQ(path_obj_info.rad_object_to_path_dist, object.tracker_data.vcs_pos.x - path.path_points[next_point_idx]);
}


/**
 * Tests whether the mid grid point index is in both pt_persistent.paths. Here the mid grid point index is included there.
 * \uts{CSCSA-43832} \sdd{SF-7465} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Mid_Point_Included_In_Paths__mid_grid_point_index_is_in_paths)
{
   /** \arrange Setup pt_persistent.paths with sufficient range for check */
   Pt_Path_T path_a{};
   Pt_Path_T path_b{};
   boolean_T res;

   path_a.first_p = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path_b.first_p = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path_a.last_p  = PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   path_b.last_p  = PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call mid grid point index validity check function */
   res = Pt_Is_Mid_Point_Included_In_Paths(&path_a, &path_b);

   /** \assert expect true since point is included */
   EXPECT_TRUE(res);
}


/**
 * Tests whether the mid grid point index is in both pt_persistent.paths. Here the mid grid point index is not included there.
 * \uts{CSCSA-43833} \sdd{SF-7465} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Mid_Point_Included_In_Paths__mid_grid_point_index_is_not_in_paths)
{
   /** \arrange Setup pt_persistent.paths with insufficient range for check */
   Pt_Path_T path_a{};
   Pt_Path_T path_b{};
   boolean_T res;

   path_a.first_p = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path_b.first_p = PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path_a.last_p  = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   path_b.last_p  = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call mid grid point index validity check function */
   res = Pt_Is_Mid_Point_Included_In_Paths(&path_a, &path_b);

   /** \assert expect false since point is outside of path */
   EXPECT_FALSE(res);
}


/**
 * Tests whether weighted mean distance can be calculated. Here the path is finished and thus the weighted mean distance shall be
 * calculated. \uts{CSCSA-43834} \sdd{SF-7475} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated__path_is_finished)
{
   /** \arrange set up finished path */
   boolean_T res;
   Pt_Trail_Path_Comp_Indices_T point_to_compare_idx{};
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;
   pt_persistent.paths[0].first_p    = PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[0].last_p     = PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check whether weighted mean distance chall be calculated. */
   res = Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated(&point_to_compare_idx, &pt_persistent.paths[0]);

   /** \assert expect true */
   EXPECT_TRUE(res);
}


/**
 * Tests whether weighted mean distance can be calculated. Here the path is in creation mode and the boundaries are sufficient.
 * Thus true is expected. \uts{CSCSA-43835} \sdd{SF-7475} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated__path_is_in_creation_mode_and_boundaries_are_sufficient)
{
   /** \arrange set up a path in creation mode and with sufficient boundaries. */
   boolean_T res;
   Pt_Trail_Path_Comp_Indices_T point_to_compare_idx{};
   pt_persistent.paths[0].path_state                 = PATH_STATUS_MATURE;
   pt_persistent.paths[0].first_p                    = PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[0].last_p                     = PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   point_to_compare_idx.path_point_index_higher_prio = PT_MID_GRID_POINT_INDEX;
   point_to_compare_idx.path_point_index_lower_prio  = PT_MID_GRID_POINT_INDEX;

   /** \action Call check whether weighted mean distance chall be calculated. */
   res = Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated(&point_to_compare_idx, &pt_persistent.paths[0]);

   /** \assert expect true */
   EXPECT_TRUE(res);
}


/**
 * Tests whether weighted mean distance can be calculated. Here the path is in creation mode and the boundaries are not sufficient.
 * Thus true is expected. \uts{CSCSA-43836} \sdd{SF-7475} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test,
       Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated__path_is_in_creation_and_boundaries_are_not_sufficient)
{
   /** \arrange set up a path in creation mode and with insufficient boundaries. */
   boolean_T res;
   Pt_Trail_Path_Comp_Indices_T point_to_compare_idx{};
   pt_persistent.paths[0].path_state                 = PATH_STATUS_CREATION;
   pt_persistent.paths[0].first_p                    = PT_SINGLE_GRID_POINT_OFFSET;
   pt_persistent.paths[0].last_p                     = PT_MID_GRID_POINT_INDEX;
   point_to_compare_idx.path_point_index_higher_prio = pt_persistent.paths[0].last_p;
   point_to_compare_idx.path_point_index_lower_prio  = pt_persistent.paths[0].last_p + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call check whether weighted mean distance chall be calculated. */
   res = Pt_Shall_Non_Default_Weighted_Dist_Mean_Be_Calculated(&point_to_compare_idx, &pt_persistent.paths[0]);

   /** \assert expect false */
   EXPECT_FALSE(res);
}


/**
 * Tests the heading calculation by position near the object.tracker_data. Here heading is expected to be PI.
 * \uts{CSCSA-43837} \sdd{SF-7584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos__longitudinal_backward_path_obj_moving_aligned)
{
   /** \arrange set up a longitudinal backward path. */
   uint8_t next_point_idx;
   float32_T path_val = 1.0f;
   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_BACKWARD, PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX,
                         1u, Create_2d_Vector_Coordinates(p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f, path_val),
                         Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f, path_val), 0.0f, path_val);

   object.tracker_data.vcs_pos     = Create_2d_Vector_Coordinates(p_grid_array[PT_MID_GRID_POINT_INDEX] - 1.0f, path_val);
   object.tracker_data.vcs_heading = -0.5f * PI;
   next_point_idx                  = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call heading calculation routine by position. */
   Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos(&path_obj_info, next_point_idx, &path, &object.tracker_data, p_grid_array);

   /** \assert expect heading to be equal to PI */
   EXPECT_FLOAT_EQ(path_obj_info.closest_to_successive_pt_heading, PI);
}


/**
 * Tests the heading calculation by position near the object.tracker_data. Here heading is expected to be 0.
 * \uts{CSCSA-43838} \sdd{SF-7584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos__longitudinal_forward_path_obj_moving_aligned)
{
   /** \arrange set up a longitudinal forward path. */
   uint8_t next_point_idx;
   float32_T path_val = 1.0f;
   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX,
                         1u, Create_2d_Vector_Coordinates(p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f, path_val),
                         Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f, path_val), 0.0f, path_val);

   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(p_grid_array[PT_MID_GRID_POINT_INDEX] + 1.0f, path_val);
   next_point_idx              = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call heading calculation routine by position. */
   Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos(&path_obj_info, next_point_idx, &path, &object.tracker_data, p_grid_array);

   /** \assert expect heading to be equal to near 0 */
   EXPECT_FLOAT_EQ(path_obj_info.closest_to_successive_pt_heading, 0.0f);
}


/**
 * Tests the heading calculation by position near the object.tracker_data. Here heading is expected to be -0.5 PI.
 * \uts{CSCSA-43839} \sdd{SF-7584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos__lateral_left_path_obj_moving_left_obj_moving_aligned)
{
   /** \arrange set up a longitudinal forward path. */
   uint8_t next_point_idx;
   float32_T path_val = 1.0f;
   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LAT_LEFT, PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 1u,
                         Create_2d_Vector_Coordinates(p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f, path_val),
                         Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f, path_val), 0.0f, path_val);

   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_MID_GRID_POINT_INDEX] - 1.0f);
   next_point_idx              = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call heading calculation routine by position. */
   Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos(&path_obj_info, next_point_idx, &path, &object.tracker_data, p_grid_array);

   /** \assert expect heading to be equal to -0.5f PI */
   EXPECT_FLOAT_EQ(path_obj_info.closest_to_successive_pt_heading, -0.5f * PI);
}

/**
 * Tests the heading calculation by position near the object.tracker_data. Here heading is expected to be 0.5 PI.
 * \uts{CSCSA-43840} \sdd{SF-7584} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos__lateral_right_path_obj_moving_left_obj_moving_aligned)
{
   /** \arrange set up a longitudinal forward path. */
   uint8_t next_point_idx;
   float32_T path_val = 1.0f;
   Pt_Init_Path_Linearly(&path, p_grid_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX,
                         1u, Create_2d_Vector_Coordinates(p_grid_array[PT_LOWEST_GRID_POINT_INDEX] - 1.0f, path_val),
                         Create_2d_Vector_Coordinates(p_grid_array[PT_HIGHEST_GRID_POINT_INDEX] + 1.0f, path_val), 0.0f, path_val);

   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(path_val, p_grid_array[PT_MID_GRID_POINT_INDEX] + 1.0f);
   next_point_idx              = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;

   /** \action Call heading calculation routine by position. */
   Pt_Get_Heading_Diff_Near_Obj_By_Obj_Pos(&path_obj_info, next_point_idx, &path, &object.tracker_data, p_grid_array);

   /** \assert expect heading to be equal to -0.5f PI */
   EXPECT_FLOAT_EQ(path_obj_info.closest_to_successive_pt_heading, 0.5f * PI);
}


/**
 * Check that next point indices are initialized correctly.
 * \uts{CSCSA-43841} \sdd{SF-7583} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Init_Next_Point_Indices__initialize_values)
{
   /** \arrange set up non default values for attributes of next point indices. */
   Pt_Point_Indices_Dir_Indep_Obj_T next_point_idx;
   next_point_idx.next_point_idx_lat_path  = 50;
   next_point_idx.next_point_idx_long_path = 50;

   /** \action Call constructor. */
   Pt_Init_Next_Point_Indices(&next_point_idx);

   /** \assert expect attributes to be initialized correctly */
   EXPECT_EQ(next_point_idx.next_point_idx_lat_path, PT_INVALID_GRID_POINT_INDEX);
   EXPECT_EQ(next_point_idx.next_point_idx_long_path, PT_INVALID_GRID_POINT_INDEX);
}


/**
 * Check the return of path index for a given object id. Here a path is in creation process by the given object id. Thus a non
 * default value is expected. \uts{CSCSA-43842} \sdd{SF-7342} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Path_Index__object_id_is_building_up_a_path)
{
   /** \arrange set up the path in creation process attached to the id. */
   uint8_t returned_idx;
   uint8_t idx_to_return                                              = PT_NUMBER_OF_PATHS - 5u;
   object.tracker_data.id                                             = 1;
   pt_persistent.paths[idx_to_return].obj_curr_used_for_path_build.id = object.tracker_data.id;

   /** \action Call routine for path index returning. */
   returned_idx = Pt_Get_Path_Index(pt_persistent.paths, &object.tracker_data);

   /** \assert Expect equality of the two indices. */
   EXPECT_EQ(returned_idx, idx_to_return);
}


/**
 * Check the return of path index for a given object id. A default object id is passed here. Thus a default value for the path
 * index is expected. \uts{CSCSA-43843} \sdd{SF-7342} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Path_Index__object_id_is_default_and_thus_default_value_shall_be_returned)
{
   /** \arrange set up the path in creation process attached to the id. */
   uint8_t returned_idx;
   object.tracker_data.id = FBK_ZERO_UINT;

   /** \action Call routine for path index returning. */
   returned_idx = Pt_Get_Path_Index(pt_persistent.paths, &object.tracker_data);

   /** \assert Expect default value for path index. */
   EXPECT_EQ(returned_idx, PT_DEFAULT_MATCH_INDEX);
}


/**
 * Check the return of path index for a given object id. A valid object id is passed here. However it has not started to build a
 * path yet. Thus a default value for the path index is expected. \uts{CSCSA-43844} \sdd{SF-7342} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Get_Path_Index__object_id_valid_and_thus_default_value_shall_be_returned)
{
   /** \arrange set up a valid id. */
   uint8_t returned_idx;
   object.tracker_data.id = 1;

   /** \action Call routine for path index returning. */
   returned_idx = Pt_Get_Path_Index(pt_persistent.paths, &object.tracker_data);

   /** \assert Expect default value for path index. */
   EXPECT_EQ(returned_idx, PT_DEFAULT_MATCH_INDEX);
}

/**
 * Checks whether an object is invalid to be matched. True is expected.
 * \uts{CSCSA-43845} \sdd{SF-7469} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Object_Matching_Test, Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path__none_direction)
{
   /** \arrange set up a valid object.tracker_data. */
   boolean_T res;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(0.0f, (pt_input.Num_Grid_Pts_Dep_Cals.k_pt_find_max_lat_posn / 2.0f));
   obj_move_dir                = PT_OBJECT_MOV_DIR_NONE;
   object.tracker_data.speed   = cals.k_pt_find_min_speed + EPSILON;
   object.tracker_data.status  = PA_OBJ_STATUS_MATURE;

   /** \action Call validity check for an object to be matched to a path. */
   res = Pt_Is_Object_Invalid_For_Being_Matched_To_A_Path(obj_move_dir, &cals, &pt_input, &object.tracker_data);

   /** \assert expect false */
   EXPECT_TRUE(res);
}