/**
 * @file pt_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44013}
 */


#include "pt_test.hpp"
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
#include "pt.c"
#include "pt_output_t.h"
}

/**
 * Checks the main recording of pt_persistent.paths routine. Here all input objects are in status invalid and thus no
 * pt_persistent.paths shall be created. \uts{CSCSA-44014} \sdd{SF-7318} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Record_Paths__all_objects_are_invalid_and_thus_no_paths_are_created)
{
   /** \arrange Set all objects to invalid state and set all pt_persistent.paths to their defaults. */
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      object_data[idx].status = PA_OBJ_STATUS_INVALID;
      object_data[idx].id     = 0u;
   }

   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state                      = PATH_STATUS_DEFAULT;
      pt_persistent.paths[idx].obj_curr_used_for_path_build.id = 0u;
   }

   /** \action call main function for recording. */
   Pt_Record_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Expect no pt_persistent.paths to be recorded. */
   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      EXPECT_EQ(pt_persistent.paths[idx].path_state, PATH_STATUS_DEFAULT);
   }
}


/**
 * Checks the main recording of pt_persistent.paths routine. Here one object is getting invalid and has build up a path.
 * \uts{CSCSA-44015} \sdd{SF-7318} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Record_Paths__object_turns_invalid)
{
   /** \arrange Set up one object with status invalid which has created a path. */
   object_data[2u].status     = PA_OBJ_STATUS_INVALID;
   object_data[2u].id         = 1u;
   object_data[2u].age        = 160u;
   object_data[2u].f_moveable = FBK_TRUE;


   float32_T path_val = 1.0f;
   Pt_Init_Path_Linearly(&pt_persistent.paths[5u], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_MID_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f),
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 0.5f), 0.0f,
                         path_val);
   pt_persistent.paths[5u].path_state                       = PATH_STATUS_CREATION;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.id  = 1u;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.age = object_data[2u].age - 1u;

   /** \action call main function for recording. */
   Pt_Record_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Expect that the path has been finished. */
   EXPECT_EQ(pt_persistent.paths[5u].path_state, PATH_STATUS_MATURE);
   EXPECT_EQ(pt_persistent.paths[5u].obj_curr_used_for_path_build.id, 0u);
   EXPECT_EQ(pt_persistent.paths[5u].obj_curr_used_for_path_build.age, 0u);
}


/**
 * Checks the main recording of pt_persistent.paths routine. Here one object is getting coasted and has build up a path.
 * \uts{CSCSA-44016} \sdd{SF-7318} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Record_Paths__object_turns_coasted)
{
   /** \arrange Set up one object with status coasted which has created a path. */
   object_data[2u].status     = PA_OBJ_STATUS_COASTED;
   object_data[2u].id         = 1u;
   object_data[2u].age        = 160u;
   object_data[2u].speed      = 0.9f * cals.k_pt_min_obj_speed;
   object_data[2u].f_moveable = FBK_TRUE;


   float32_T path_val = 1.0f;
   Pt_Init_Path_Linearly(&pt_persistent.paths[5u], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_MID_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f),
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 0.5f), 0.0f,
                         path_val);
   pt_persistent.paths[5u].path_state                       = PATH_STATUS_CREATION;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.id  = 1u;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.age = object_data[2u].age - 1u;

   /** \action call main function for recording. */
   Pt_Record_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Expect that the path has been finished. */
   EXPECT_EQ(pt_persistent.paths[5u].path_state, PATH_STATUS_MATURE);
   EXPECT_EQ(pt_persistent.paths[5u].obj_curr_used_for_path_build.id, 0u);
   EXPECT_EQ(pt_persistent.paths[5u].obj_curr_used_for_path_build.age, 0u);
}


/**
 * Checks the main recording of pt_persistent.paths routine. Here one object is mature but needs to finish its path since it is not
 * further allowed for path creation. \uts{CSCSA-44017} \sdd{SF-7318} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Record_Paths__object_is_mature_but_gets_invalid_for_path_creation_due_to_its_speed)
{
   /** \arrange Set up one object with status coasted which has created a path. */
   object_data[2u].status     = PA_OBJ_STATUS_MATURE;
   object_data[2u].id         = 1u;
   object_data[2u].age        = 160u;
   object_data[2u].speed      = 0.9f * cals.k_pt_min_obj_speed;
   object_data[2u].f_moveable = FBK_TRUE;

   float32_T path_val = 1.0f;
   Pt_Init_Path_Linearly(&pt_persistent.paths[5u], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_MID_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f),
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 0.5f), 0.0f,
                         path_val);
   pt_persistent.paths[5u].path_state                       = PATH_STATUS_CREATION;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.id  = 1u;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.age = object_data[2u].age - 1u;

   /** \action call main function for recording. */
   Pt_Record_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Expect that the path has been finished. */
   EXPECT_EQ(pt_persistent.paths[5u].path_state, PATH_STATUS_MATURE);
   EXPECT_EQ(pt_persistent.paths[5u].obj_curr_used_for_path_build.id, 0u);
   EXPECT_EQ(pt_persistent.paths[5u].obj_curr_used_for_path_build.age, 0u);
}


/**
 * Checks the main recording of pt_persistent.paths routine. Here one object is mature but has not yet added any path information.
 * \uts{CSCSA-44018} \sdd{SF-7318} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Record_Paths__object_is_mature_but_has_not_yet_created_a_path)
{
   /** \arrange Set up one object with status mature which has created a path and needs to finish it. */
   object_data[2u].status                = PA_OBJ_STATUS_MATURE;
   object_data[2u].id                    = 1u;
   object_data[2u].age                   = 5u;
   object_data[2u].speed                 = 1.1f * cals.k_pt_min_obj_speed;
   object_data[2u].f_reflection          = FBK_FALSE;
   object_data[2u].existence_probability = 1.1f * cals.k_pt_min_exist_prob_to_be_valid;
   object_data[2u].vcs_pos.y             = 0.8f * cals.k_pt_path_track_long_range_limit;
   object_data[2u].vcs_pos.x             = 0.8f * cals.k_pt_path_track_long_range_limit;
   object_data[2u].f_moveable            = FBK_TRUE;

   /** \action call main function for recording. */
   Pt_Record_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Expect that the path has been finished. */
   EXPECT_EQ(pt_persistent.paths[0u].path_state, PATH_STATUS_CREATION);
   EXPECT_EQ(pt_persistent.paths[0u].obj_curr_used_for_path_build.id, 1u);
   EXPECT_EQ(pt_persistent.paths[0u].obj_curr_used_for_path_build.age, 5u);
}


/**
 * Checks the main recording of pt_persistent.paths routine. Here one object is mature and needs to update its path.
 * \uts{CSCSA-44019} \sdd{SF-7318} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Record_Paths__object_is_mature_and_needs_to_update_its_path)
{
   /** \arrange Set up one object with status mature which needs to update its path. */
   float32_T path_val                    = 1.0f;
   object_data[2u].status                = PA_OBJ_STATUS_MATURE;
   object_data[2u].id                    = 1u;
   object_data[2u].age                   = 5u;
   object_data[2u].speed                 = 1.1f * cals.k_pt_min_obj_speed;
   object_data[2u].f_reflection          = FBK_FALSE;
   object_data[2u].existence_probability = 1.1f * cals.k_pt_min_exist_prob_to_be_valid;
   object_data[2u].vcs_pos.y             = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 0.7f;
   object_data[2u].vcs_pos.x             = path_val;
   object_data[2u].vcs_heading           = 0.5 * PI;
   object_data[2u].f_moveable            = FBK_TRUE;


   Pt_Init_Path_Linearly(&pt_persistent.paths[5u], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_MID_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f),
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 0.5f), 0.0f,
                         path_val);
   pt_persistent.paths[5u].path_state                       = PATH_STATUS_CREATION;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.id  = 1u;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.age = object_data[2u].age - 1u;


   /** \action call main function for recording. */
   Pt_Record_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Expect that the path was updated. */
   EXPECT_EQ(pt_persistent.paths[5u].last_mat.y, object_data[2u].vcs_pos.y);
   EXPECT_EQ(pt_persistent.paths[5u].new_path_point_status, PATH_POINT_NEW_MATURE);
}

/**
 * Checks the main recording of pt_persistent.paths routine. Here one object is mature but is not moveable.
 * \uts{CSCSA-138947} \sdd{SF-7318} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Record_Paths__object_is_mature_but_is_not_moveable)
{
   /** \arrange Set up one object with status mature which needs to update its path. */
   float32_T path_val                    = 1.0f;
   object_data[2u].status                = PA_OBJ_STATUS_MATURE;
   object_data[2u].id                    = 1u;
   object_data[2u].age                   = 5u;
   object_data[2u].speed                 = 1.1f * cals.k_pt_min_obj_speed;
   object_data[2u].f_reflection          = FBK_FALSE;
   object_data[2u].existence_probability = 1.1f * cals.k_pt_min_exist_prob_to_be_valid;
   object_data[2u].vcs_pos.y             = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 0.7f;
   object_data[2u].vcs_pos.x             = path_val;
   object_data[2u].vcs_heading           = 0.5 * PI;
   object_data[2u].f_moveable            = FBK_FALSE;


   Pt_Init_Path_Linearly(&pt_persistent.paths[5u], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX,
                         PT_MID_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f),
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 0.5f), 0.0f,
                         path_val);
   pt_persistent.paths[5u].path_state                       = PATH_STATUS_CREATION;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.id  = 1u;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.age = object_data[2u].age - 1u;


   /** \action call main function for recording. */
   Pt_Record_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Expect that the path was updated. */
   EXPECT_EQ(pt_persistent.paths[5u].new_path_point_status, PATH_POINT_NEW_FIRST);
}


/**
 * Checks whether the object is valid for path creation. Here each condition is fulfilled so that the object is valid for path
 * creation. \uts{CSCSA-44020} \sdd{SF-7303} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Obj_Valid_For_Path_Creation__object_is_valid)
{
   /** \arrange Setup a valid object for path creation. */
   boolean_T result;
   object.tracker_data.f_reflection          = FBK_FALSE;
   object.tracker_data.speed                 = cals.k_pt_min_obj_speed;
   object.tracker_data.existence_probability = cals.k_pt_min_exist_prob_to_be_valid;

   /** \action call function to test. */
   result = Pt_Is_Obj_Valid_For_Path_Creation(&object.tracker_data, &cals);
   /** \assert Expect true. */
   EXPECT_TRUE(result);
}

/**
 * Checks whether the object is valid for path creation. Here the object is a reflection. Thus false is expected.
 * \uts{CSCSA-44021} \sdd{SF-7303} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Obj_Valid_For_Path_Creation__object_is_a_reflection)
{
   /** \arrange Setup an object classified as reflection. */
   boolean_T result;
   object.tracker_data.f_reflection = FBK_TRUE;

   /** \action call function to test. */
   result = Pt_Is_Obj_Valid_For_Path_Creation(&object.tracker_data, &cals);
   /** \assert Expect false. */
   EXPECT_FALSE(result);
}

/**
 * Checks whether the object is valid for path creation. Here the objects existence probability is not sufficient. Thus false is
 * expected. \uts{CSCSA-44022} \sdd{SF-7303} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Obj_Valid_For_Path_Creation__existence_prob_is_too_low)
{
   /** \arrange Setup an object with low existence probability. */
   boolean_T result;
   object.tracker_data.f_reflection          = FBK_FALSE;
   object.tracker_data.existence_probability = cals.k_pt_min_exist_prob_to_be_valid - EPSILON;
   object.tracker_data.speed                 = cals.k_pt_min_obj_speed;

   /** \action call function to test. */
   result = Pt_Is_Obj_Valid_For_Path_Creation(&object.tracker_data, &cals);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}

/**
 * Checks whether the object is valid for path creation. Here the objects speed is too low. Thus false is expected.
 * \uts{CSCSA-44023} \sdd{SF-7303} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Obj_Valid_For_Path_Creation__speed_is_too_low)
{
   /** \arrange Setup an object with low speed. */
   boolean_T result;
   object.tracker_data.f_reflection          = FBK_FALSE;
   object.tracker_data.existence_probability = cals.k_pt_min_exist_prob_to_be_valid;
   object.tracker_data.speed                 = cals.k_pt_min_obj_speed - EPSILON;

   /** \action call function to test. */
   result = Pt_Is_Obj_Valid_For_Path_Creation(&object.tracker_data, &cals);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}

/**
 * Checks whether a coasted object has become invalid in path creation process. Here the object has build a path and is just
 * coasting in the current cycle. It is not out of the path tracking range nor does it came to standstill or has a drop in its
 * existence probability. Thus false is expected. \uts{CSCSA-44024} \sdd{SF-7302} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Coasted_Obj_Invalid_For_Further_Creation__coasted_object_is_valid)
{
   /** \arrange Arrange valid coasted object. */
   boolean_T result;
   object.tracker_data.f_reflection          = FBK_FALSE;
   object.tracker_data.vcs_pos.x             = cals.k_pt_path_track_long_range_limit - EPSILON;
   object.tracker_data.speed                 = cals.k_pt_min_obj_speed;
   object.tracker_data.f_reflection          = FBK_FALSE;
   object.tracker_data.existence_probability = cals.k_pt_min_exist_prob_to_be_valid;
   path.direction                            = PATH_DIRECTION_LONG_FORWARD;

   /** \action call function to test. */
   result = Pt_Is_Coasted_Obj_Invalid_For_Further_Creation(&object.tracker_data, &cals);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}

/**
 * Checks whether a coasted object has become invalid in path creation process. Here the object is out of the path tracking range.
 * Thus true is expected. \uts{CSCSA-44025} \sdd{SF-7302} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Coasted_Obj_Invalid_For_Further_Creation__coasted_obj_out_of_pt_range_thus_true_expected)
{
   /** \arrange Arrange valid coasted object. */
   boolean_T result;
   object.tracker_data.f_reflection = FBK_FALSE;
   object.tracker_data.vcs_pos.x    = cals.k_pt_path_track_long_range_limit + EPSILON;
   path.direction                   = PATH_DIRECTION_LONG_FORWARD;

   /** \action call function to test. */
   result = Pt_Is_Coasted_Obj_Invalid_For_Further_Creation(&object.tracker_data, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}

/**
 * Checks whether a coasted object has become invalid in path creation process. Here the object is invalid since it is classified
 * as a reflection. Thus true is expected \uts{CSCSA-44026} \sdd{SF-7302} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Coasted_Obj_Invalid_For_Further_Creation__object_is_classified_as_reflection)
{
   /** \arrange Arrange valid coasted object. */
   boolean_T result;
   object.tracker_data.f_reflection = FBK_FALSE;
   object.tracker_data.vcs_pos.x    = cals.k_pt_path_track_long_range_limit - EPSILON;
   object.tracker_data.f_reflection = FBK_TRUE;
   path.direction                   = PATH_DIRECTION_LONG_FORWARD;

   /** \action call function to test. */
   result = Pt_Is_Coasted_Obj_Invalid_For_Further_Creation(&object.tracker_data, &cals);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Checks whether a path object pair has finished its creation phase. Here the path object pair is not unjustified by the functions
 * subconditions. Thus false is expected. \uts{CSCSA-44027} \sdd{SF-7298} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Has_Path_Obj_Pair_Prep_Finished_Creation__path_obj_pair_is_still_in_creation_process)
{
   /** \arrange Arrange valid path object pair in creation process. */
   boolean_T result;

   /* Plausible age check criteria */
   path.obj_curr_used_for_path_build.age     = 2u;
   object.tracker_data.age                   = path.obj_curr_used_for_path_build.age + 1u;
   cals.k_pt_f_check_object_age_plausibility = FBK_TRUE;
   /* In range check */
   path.direction                = PATH_DIRECTION_LONG_FORWARD;
   object.tracker_data.vcs_pos.x = cals.k_pt_path_track_long_range_limit - EPSILON;
   /* direction check*/
   object_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;

   /** \action call function to test. */
   result = Pt_Has_Path_Obj_Pair_Prep_Finished_Creation(&path, &pt_persistent, &pt_input, &object_orientation,
                                                        &object.tracker_data, &cals, p_vehicle_data);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}


/**
 * Checks whether a path object pair has finished its creation phase. No path direction is given here, thus false is expected and
 * the creation process shall not be aborted. \uts{CSCSA-44028} \sdd{SF-7298} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Has_Path_Obj_Pair_Prep_Finished_Creation__path_obj_pair_started_creation_process_thus_no_direction_given)
{
   /** \arrange Arrange valid path object pair which started the creation process. Thus no direction is expected */
   boolean_T result;

   /* Plausible age check criteria */
   path.obj_curr_used_for_path_build.age     = 2u;
   object.tracker_data.age                   = path.obj_curr_used_for_path_build.age + 1u;
   cals.k_pt_f_check_object_age_plausibility = FBK_TRUE;
   /* In range check */
   path.direction                = PATH_DIRECTION_NONE;
   object.tracker_data.vcs_pos.x = cals.k_pt_path_track_long_range_limit - EPSILON;
   /* direction check*/
   object_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;

   /** \action call function to test. */
   result = Pt_Has_Path_Obj_Pair_Prep_Finished_Creation(&path, &pt_persistent, &pt_input, &object_orientation,
                                                        &object.tracker_data, &cals, p_vehicle_data);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}


/**
 * Checks whether a path object pair has finished its creation phase. The object age has an undefined behaviour, thus the creation
 * process shall be aborted and true is expected. \uts{CSCSA-44029} \sdd{SF-7298} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Has_Path_Obj_Pair_Prep_Finished_Creation__age_is_implausible)
{
   /** \arrange Create a path object pair without */
   boolean_T result;

   /* Plausible age check criteria */
   path.obj_curr_used_for_path_build.age     = 2u;
   object.tracker_data.age                   = path.obj_curr_used_for_path_build.age - 1u;
   cals.k_pt_f_check_object_age_plausibility = FBK_TRUE;

   /** \action call function to test. */
   result = Pt_Has_Path_Obj_Pair_Prep_Finished_Creation(&path, &pt_persistent, &pt_input, &object_orientation,
                                                        &object.tracker_data, &cals, p_vehicle_data);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Checks whether a path object pair has finished its creation phase. Object is out of path tracking range. Thus true is expected
 * and the creation process shall be aborted. \uts{CSCSA-44030} \sdd{SF-7298} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Has_Path_Obj_Pair_Prep_Finished_Creation__object_is_out_of_path_tracking_range)
{
   /** \arrange Arrange path object pair where the creation process is ending. Thus object drives out of the path tracking range */
   boolean_T result;

   /* Plausible age check criteria */
   path.obj_curr_used_for_path_build.age     = 2u;
   object.tracker_data.age                   = path.obj_curr_used_for_path_build.age + 1u;
   cals.k_pt_f_check_object_age_plausibility = FBK_TRUE;
   /* In range check */
   path.direction                = PATH_DIRECTION_LONG_FORWARD;
   object.tracker_data.vcs_pos.x = cals.k_pt_path_track_long_range_limit + EPSILON;
   /* direction check*/
   object_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;

   /** \action call function to test. */
   result = Pt_Has_Path_Obj_Pair_Prep_Finished_Creation(&path, &pt_persistent, &pt_input, &object_orientation,
                                                        &object.tracker_data, &cals, p_vehicle_data);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Checks whether a path object pair has finished its creation phase. Here the orientation between object and path are differing.
 * Thus true is expected. \uts{CSCSA-44031} \sdd{SF-7298} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Has_Path_Obj_Pair_Prep_Finished_Creation__orientation_between_object_and_path_is_differing)
{
   /** \arrange Arrange a path object pair where the orientation is differing. */
   boolean_T result;

   /* Plausible age check criteria */
   path.obj_curr_used_for_path_build.age     = 2u;
   object.tracker_data.age                   = path.obj_curr_used_for_path_build.age + 1u;
   cals.k_pt_f_check_object_age_plausibility = FBK_TRUE;
   /* In range check */
   path.direction                = PATH_DIRECTION_LONG_FORWARD;
   object.tracker_data.vcs_pos.x = cals.k_pt_path_track_long_range_limit - EPSILON;
   /* direction check*/
   object_orientation = PT_OBJECT_ORIENTATION_LATERAL;

   /** \action call function to test. */
   result = Pt_Has_Path_Obj_Pair_Prep_Finished_Creation(&path, &pt_persistent, &pt_input, &object_orientation,
                                                        &object.tracker_data, &cals, p_vehicle_data);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Checks whether a path point shall be added to an existing path. Here the lastly added boundary and the relevant object position
 * are surrounding the grid array while the object is moving against vcs. Thus true is expected. \uts{CSCSA-44032} \sdd{SF-7310}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Shall_Path_Point_Be_Added__object_is_moving_against_vcs_and_surrounds_the_grid_array)
{
   /** \arrange Arrange boundaries so that they are surrounding the grid component. */
   boolean_T result;
   float32_T relevant_obj_pos   = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] - EPSILON;
   float32_T last_path_boundary = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + EPSILON;

   /** \action call function to test. */
   result = Pt_Shall_Path_Point_Be_Added(&pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], &relevant_obj_pos, &last_path_boundary);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Checks whether a path point shall be added to an existing path. Here the lastly added boundary and the relevant object position
 * are surrounding the grid array while the object is moving with vcs. Thus true is expected. \uts{CSCSA-44033} \sdd{SF-7310}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Shall_Path_Point_Be_Added__object_is_moving_with_vcs_and_surrounds_the_grid_array)
{
   /** \arrange Arrange boundaries so that they are surrounding the grid component. */
   boolean_T result;
   float32_T relevant_obj_pos   = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + EPSILON;
   float32_T last_path_boundary = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] - EPSILON;

   /** \action call function to test. */
   result = Pt_Shall_Path_Point_Be_Added(&pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], &relevant_obj_pos, &last_path_boundary);

   /** \assert Expect true. */
   EXPECT_TRUE(result);
}


/**
 * Checks whether a path point shall be added to an existing path. Here a grid value to check is given which is right to both
 * boundaries. Thus false is expected. \uts{CSCSA-44034} \sdd{SF-7310} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Shall_Path_Point_Be_Added__boundaries_are_located_left_to_the_grid_component)
{
   /** \arrange Arrange boundaries so that they are not surrounding the grid component. */
   boolean_T result;
   float32_T relevant_obj_pos   = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] + EPSILON;
   float32_T last_path_boundary = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] - EPSILON;

   /** \action call function to test. */
   result = Pt_Shall_Path_Point_Be_Added(&pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], &relevant_obj_pos, &last_path_boundary);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}


/**
 * Checks whether a path point shall be added to an existing path. Here a grid value to check is given which is left to both
 * boundaries. Thus false is expected. \uts{CSCSA-44035} \sdd{SF-7310} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Shall_Path_Point_Be_Added__boundaries_are_located_right_to_the_grid_component)
{
   /** \arrange Arrange boundaries so that they are not surrounding the grid component. */
   boolean_T result;
   float32_T relevant_obj_pos   = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] + EPSILON;
   float32_T last_path_boundary = pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - EPSILON;

   /** \action call function to test. */
   result = Pt_Shall_Path_Point_Be_Added(&pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], &relevant_obj_pos, &last_path_boundary);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}


/**
 * Adds path points to a path in case that a path point shall be added to that based on subconditions. In this test no path point
 * shall be added. Thus the input path shall remain the same \uts{CSCSA-44036} \sdd{SF-7315} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Path_Points__no_path_points_shall_be_added)
{
   /** \arrange Initialize a input path. */
   float32_T path_val     = 5.0f;
   uint8_t upper_boundary = PT_LOWEST_GRID_POINT_INDEX + 5u;
   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX, upper_boundary, 0,
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f),
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[upper_boundary] + 0.5f), 0.0f, path_val);

   object_orientation            = PT_OBJECT_ORIENTATION_LATERAL;
   object.tracker_data.vcs_pos.y = path.last_mat.y + EPSILON;
   /** \action call function to test. */
   Pt_Update_Path_Points(&path, &object.tracker_data, object_orientation, &pt_input, p_vehicle_data);

   /** \assert Expect that path no path point is added. */
   EXPECT_FLOAT_EQ(path.path_points[path.last_p + 1u], 0.0f);
   EXPECT_EQ(path.last_p, upper_boundary);
}


/**
 * Adds path points to a path in case that a path point shall be added to that based on subconditions. In this test a path point
 * shall be added to the lateral path. Thus the input path increases its size. \uts{CSCSA-44037} \sdd{SF-7315}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Path_Points__point_shall_be_added_to_lateral_path)
{
   /** \arrange Initialize a input path. */
   float32_T path_val     = 5.0f;
   uint8_t upper_boundary = PT_LOWEST_GRID_POINT_INDEX + 5u;
   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX, upper_boundary, 0,
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f),
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[upper_boundary]
                                                                   + (pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 1u] - EPSILON)),
                         0.0f, path_val);

   object_orientation          = PT_OBJECT_ORIENTATION_LATERAL;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(
      path_val, pt_input.grid_pt_array[upper_boundary] + (pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 1u] + EPSILON));
   /** \action call function to test. */
   Pt_Update_Path_Points(&path, &object.tracker_data, object_orientation, &pt_input, p_vehicle_data);

   /** \assert Expect that a path point is added to the right. */
   EXPECT_FLOAT_EQ(path.path_points[upper_boundary + 1u], path_val);
   EXPECT_EQ(path.last_p, upper_boundary + 1u);
}


/**
 * Adds path points to a path in case that a path point shall be added to that based on subconditions. In this test a path point
 * shall be added to the longitudinal path. Thus the input path increases its size. \uts{CSCSA-44038} \sdd{SF-7315}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Path_Points__point_shall_be_added_to_longitudinal_path)
{
   /** \arrange Initialize a input path. */
   float32_T path_val     = 5.0f;
   uint8_t upper_boundary = PT_LOWEST_GRID_POINT_INDEX + 5u;
   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX, upper_boundary, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f, path_val),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_boundary]
                                                         + (pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 1u] - EPSILON),
                                                      path_val),
                         0.0f, path_val);

   object_orientation          = PT_OBJECT_ORIENTATION_LONGITUDINAL;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(
      pt_input.grid_pt_array[upper_boundary] + (pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 1u] + EPSILON), path_val);

   /** \action call function to test. */
   Pt_Update_Path_Points(&path, &object.tracker_data, object_orientation, &pt_input, p_vehicle_data);

   /** \assert Expect that path point is added to the right. */
   EXPECT_FLOAT_EQ(path.path_points[upper_boundary + 1u], path_val);
   EXPECT_EQ(path.last_p, upper_boundary + 1u);
}


/**
 * Adds path points to a path in case that a path point shall be added to that based on subconditions. In this test a path point
 * shall be added to a path while this one does not have a given direction. The adding shall be based on the object orientation.
 * Thus the input path increases its size. \uts{CSCSA-44039} \sdd{SF-7315} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Path_Points__point_shall_be_added_to_path_based_on_lateral_object_orientation)
{
   /** \arrange Initialize a input path. */
   float32_T path_val     = 5.0f;
   uint8_t upper_boundary = PT_LOWEST_GRID_POINT_INDEX;
   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_NONE, PT_LOWEST_GRID_POINT_INDEX, upper_boundary, 0,
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f),
                         Create_2d_Vector_Coordinates(path_val, pt_input.grid_pt_array[upper_boundary]
                                                                   + (pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 1u] - EPSILON)),
                         0.0f, path_val);

   object_orientation          = PT_OBJECT_ORIENTATION_LATERAL;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(
      path_val, pt_input.grid_pt_array[upper_boundary] + (pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 1u] + EPSILON));
   /** \action call function to test. */
   Pt_Update_Path_Points(&path, &object.tracker_data, object_orientation, &pt_input, p_vehicle_data);

   /** \assert Expect that a path point is added to the right. */
   EXPECT_FLOAT_EQ(path.path_points[upper_boundary + 1u], path_val);
   EXPECT_EQ(path.last_p, upper_boundary + 1u);
}


/**
 * Adds path points to a path in case that a path point shall be added to that based on subconditions. In this test a path point
 * shall be added to the path while this one does not have a given direction. The adding shall be based on the object orientation.
 * Thus the input path increases its size. \uts{CSCSA-44040} \sdd{SF-7315} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Path_Points__point_shall_be_added_to_longitudinal_path_based_on_longitudinal_object_orientation)
{
   /** \arrange Initialize a input path. */
   float32_T path_val     = 5.0f;
   uint8_t upper_boundary = PT_LOWEST_GRID_POINT_INDEX;
   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_NONE, PT_LOWEST_GRID_POINT_INDEX, upper_boundary, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f, path_val),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_boundary]
                                                         + (pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 1u] - EPSILON),
                                                      path_val),
                         0.0f, path_val);

   object_orientation          = PT_OBJECT_ORIENTATION_LONGITUDINAL;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(
      pt_input.grid_pt_array[upper_boundary] + (pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX + 1u] + EPSILON), path_val);

   /** \action call function to test. */
   Pt_Update_Path_Points(&path, &object.tracker_data, object_orientation, &pt_input, p_vehicle_data);

   /** \assert Expect that path point is added to the right. */
   EXPECT_FLOAT_EQ(path.path_points[upper_boundary + 1u], path_val);
   EXPECT_EQ(path.last_p, upper_boundary + 1u);
}

/**
 * Tests the update of age which is attached to the path for its creation phase. Here a valid id is given and thus an update is
 * expected. \uts{CSCSA-44041} \sdd{SF-7313} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Object_Used_For_Path_Age__valid_object_id)
{
   /** \arrange set up a default age and a valid object id such that an update is expected */
   uint8_t obj_index                     = 1u;
   path.obj_curr_used_for_path_build.id  = 1u;
   path.obj_curr_used_for_path_build.age = 1u;

   object_data[obj_index].id  = 1u;
   object_data[obj_index].age = 3u;

   /** \action Call update routine for the path object age */
   Pt_Update_Object_Used_For_Path_Age(&path, &pt_input, obj_index);

   /** \assert expect object age to be updated */
   EXPECT_EQ(path.obj_curr_used_for_path_build.age, object_data[obj_index].age);
}

/**
 * Tests the update of age which is attached to the path for its creation phase. Here an invalid id is given and thus no update is
 * expected. \uts{CSCSA-44042} \sdd{SF-7313} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Object_Used_For_Path_Age__invalid_object_id)
{
   /** \arrange set up a default age and an invalid object id */
   uint8_t obj_index                     = 1u;
   path.obj_curr_used_for_path_build.age = 1u;
   object_data[obj_index].id             = PA_INVALID_OBJ_ID;

   /** \action Call update routine for the path object age */
   Pt_Update_Object_Used_For_Path_Age(&path, &pt_input, obj_index);

   /** \assert expect object age to remain its arranged initialization */
   EXPECT_EQ(path.obj_curr_used_for_path_build.age, 1u);
}


/**
 * Tests the update of age which is attached to the path for its creation phase. Here valid ids are given however the attached id
 * to the path differs from the current object id. Thus no update shall be applied \uts{CSCSA-44043} \sdd{SF-7313}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Object_Used_For_Path_Age__ids_are_differing_due_to_control_logic_of_path_index_and_invalid_obj)
{
   /** \arrange set up a default age and differing ids */
   uint8_t obj_index                     = 1u;
   path.obj_curr_used_for_path_build.id  = 1u;
   path.obj_curr_used_for_path_build.age = 2u;

   object_data[obj_index].id  = 3u;
   object_data[obj_index].age = 1u;

   /** \action Call update routine for the path object age */
   Pt_Update_Object_Used_For_Path_Age(&path, &pt_input, obj_index);
   /** \assert expect age to remain its arranged valued */
   EXPECT_EQ(path.obj_curr_used_for_path_build.age, 2u);
}

/**
 * Tests the routine of path constructor. Here it is expected that the path corresponding to the valid path index is initialized
 * correctly. \uts{CSCSA-44044} \sdd{SF-7299} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Init_Path__initialize_a_path_with_object_properties_for_creation_phase)
{
   /** \arrange set up basic object properties and a valid path index. */
   uint8_t path_idx = 1;

   object.tracker_data.id  = 3;
   object.tracker_data.age = 5;

   /** \action Call the path constructor */
   Pt_Init_Path(path_idx, &path, &object.tracker_data);

   /** \assert Verify that the object properties are correctly mapped to the path as well as that the status is updated to creation
    * state. */
   EXPECT_EQ(path.path_state, PATH_STATUS_CREATION);
   EXPECT_EQ(path.obj_curr_used_for_path_build.id, object.tracker_data.id);
   EXPECT_EQ(path.obj_curr_used_for_path_build.age, object.tracker_data.age);
}

/**
 * Tests the routine of path constructor. Here the path index is uninitialized. Thus no path initialization shall occure.
 * \uts{CSCSA-44045} \sdd{SF-7299} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Init_Path__path_index_is_uninitialized)
{
   /** \arrange set up a default path index. */
   uint8_t path_idx = PT_DEFAULT_MATCH_INDEX;

   /** \action Call the path constructor */
   Pt_Init_Path(path_idx, &path, &object.tracker_data);

   /** \assert Verify that the path state remains default. */
   EXPECT_EQ(path.path_state, PATH_STATUS_DEFAULT);
}

/**
 * Tests the routine of path constructor. Here the path index is greater than an allowed threshold. Thus no path initialization
 * shall occure. \uts{CSCSA-44046} \sdd{SF-7299} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Init_Path__path_index_greater_than_highest_possible_index)
{
   /** \arrange set up a path index greater than the highest allowed threshold */
   uint8_t path_idx = PT_NUMBER_OF_PATHS;

   /** \action Call the path constructor. */
   Pt_Init_Path(path_idx, &path, &object.tracker_data);

   /** \assert Verify that the path state remains default. */
   EXPECT_EQ(path.path_state, PATH_STATUS_DEFAULT);
}

/**
 * Tests the maximum path speed update functionality. Here the maximum speed across a path shall be updated.
 * \uts{CSCSA-44047} \sdd{SF-7312} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Maximum_Path_Speed__obj_speed_gt_thres)
{
   /** \arrange set up an object with an higher speed than the speed attached to the path. */
   float32_T max_speed       = 10.0f;
   path.max_speed            = max_speed;
   object.tracker_data.speed = path.max_speed + EPSILON;

   /** \action Call function to update maximum path speed. */
   Pt_Update_Maximum_Path_Speed(&path, &object.tracker_data);

   /** \assert Expect maximum speed to be updated. */
   EXPECT_FLOAT_EQ(path.max_speed, max_speed + EPSILON);
}

/**
 * Tests the maximum path speed update functionality. Here the maximum speed across a path shall be kept.
 * \uts{CSCSA-44048} \sdd{SF-7312} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Maximum_Path_Speed__obj_speed_lt_thres)
{
   /** \arrange set up an object with a lower speed than the speed attached to the path. */
   float32_T max_speed       = 10.0f;
   path.max_speed            = max_speed;
   object.tracker_data.speed = max_speed - EPSILON;

   /** \action Call function to update maximum path speed. */
   Pt_Update_Maximum_Path_Speed(&path, &object.tracker_data);

   /** \assert expect speed attached to the path to remain */
   EXPECT_FLOAT_EQ(path.max_speed, max_speed);
}

/**
 * Tests the path border update functionality. Here the input path is against the vcs axis. Thus the border declared as first shall
 * be updated. \uts{CSCSA-44049} \sdd{SF-7314} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Path_Borders__path_against_vcs_axis_first_is_updated)
{
   /** \arrange Set up path against vcs. */
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(-2.0f, 2.0f);
   path.direction              = PATH_DIRECTION_LAT_LEFT;

   /** \action Call border update function. */
   Pt_Update_Path_Borders(&path, &object.tracker_data);

   /** \assert expect border declared as first to be updated. */
   EXPECT_EQ(path.path_border_status, PATH_BORDER_NEW_FIRST);
   EXPECT_FLOAT_EQ(path.first.x, object.tracker_data.vcs_pos.x);
   EXPECT_FLOAT_EQ(path.first.y, object.tracker_data.vcs_pos.y);
}

/**
 * Tests the path border update functionality. Here the input path is against the vcs axis. Thus the border declared as last shall
 * be updated. \uts{CSCSA-44050} \sdd{SF-7314} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Update_Path_Borders__path_aligned_with_vcs_axis_first_is_updated)
{
   /** \arrange Set up path aligned with vcs. */
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(-2.0f, 2.0f);
   path.direction              = PATH_DIRECTION_LAT_RIGHT;

   /** \action Call border update function. */
   Pt_Update_Path_Borders(&path, &object.tracker_data);

   /** \assert expect border declared as last to be updated. */
   EXPECT_EQ(path.path_border_status, PATH_BORDER_NEW_LAST);
   EXPECT_FLOAT_EQ(path.last_mat.x, object.tracker_data.vcs_pos.x);
   EXPECT_FLOAT_EQ(path.last_mat.y, object.tracker_data.vcs_pos.y);
}

/**
 * Tests whether the right position component is returned based on the object orientation. Here the orientation is longitudinal and
 * thus the longitudinal component is expected to be returned. \uts{CSCSA-44051} \sdd{SF-7297} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Path_Relevant_Obj_Pos_Component__object_orientation_is_longitudinal)
{
   /** \arrange Set up an object which is moving longitudinal. */
   float32_T result;
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;

   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(-2.0f, 2.0f);

   /** \action Call function to return relevant position. */
   result = Pt_Get_Path_Relevant_Obj_Pos_Component(&object.tracker_data, obj_orientation);

   /** \assert expect longitudinal component to be returned. */
   EXPECT_EQ(result, object.tracker_data.vcs_pos.x);
}

/**
 * Tests whether the right position component is returned based on the object orientation. Here the orientation is lateral and thus
 * the lateral component is expected to be returned. \uts{CSCSA-44052} \sdd{SF-7297} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Path_Relevant_Obj_Pos_Component__object_orientation_is_lateral)
{
   /** \arrange Set up an object which is moving lateral. */
   float32_T result;
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LATERAL;

   object.tracker_data.vcs_pos.x = -2.0f;
   object.tracker_data.vcs_pos.y = 2.0f;

   /** \action Call function to return relevant position. */
   result = Pt_Get_Path_Relevant_Obj_Pos_Component(&object.tracker_data, obj_orientation);

   /** \assert expect lateral component to be returned. */
   EXPECT_EQ(result, object.tracker_data.vcs_pos.y);
}

/**
 * Check whether the correct object position attached to a path is returned. Here a longitudinal backward path is given thus the
 * first longitudinal component is expected to be returned. \uts{CSCSA-44053} \sdd{SF-7296} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Last_Path_Point__path_direction_is_longitudinal_backward)
{
   /** \arrange Set up object dependent path borders and longitudinal path. */
   float32_T result;
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;

   path.first     = Create_2d_Vector_Coordinates(2.0f, -2.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(3.0f, -3.0f);
   path.direction = PATH_DIRECTION_LONG_BACKWARD;

   /** \action Call function to return the last vehicle dependent path border */
   result = Pt_Get_Last_Path_Point(&path, obj_orientation);

   /** \assert expect that the first longitudinal component is returned */
   EXPECT_EQ(result, path.first.x);
}


/**
 * Check whether the correct object position attached to a path is returned. Here a longitudinal forward path is given thus the
 * last longitudinal component is expected to be returned. \uts{CSCSA-44054} \sdd{SF-7296} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Last_Path_Point__path_direction_is_longitudinal_forward)
{
   /** \arrange Set up object dependent path borders and longitudinal path. */
   float32_T result;
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;

   path.first     = Create_2d_Vector_Coordinates(2.0f, -2.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(3.0f, -3.0f);
   path.direction = PATH_DIRECTION_LONG_FORWARD;

   /** \action Call function to return the last vehicle dependent path border */
   result = Pt_Get_Last_Path_Point(&path, obj_orientation);

   /** \assert expect that the last longitudinal component is returned */
   EXPECT_EQ(result, path.last_mat.x);
}

/**
 * Check whether the correct object position attached to a path is returned. Here a lateral left path is given thus the first
 * lateral component is expected to be returned. \uts{CSCSA-44055} \sdd{SF-7296} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Last_Path_Point__path_direction_is_lateral_left)
{
   /** \arrange Set up object dependent path borders and a lateral left path. */
   float32_T result;
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LATERAL;

   path.first     = Create_2d_Vector_Coordinates(2.0f, -2.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(3.0f, -3.0f);
   path.direction = PATH_DIRECTION_LAT_LEFT;

   /** \action Call function to return the last vehicle dependent path border. */
   result = Pt_Get_Last_Path_Point(&path, obj_orientation);

   /** \assert expect that the first lateral component is returned. */
   EXPECT_EQ(result, path.first.y);
}

/**
 * Check whether the correct object position attached to a path is returned. Here a lateral right path is given thus the last
 * lateral component is expected to be returned. \uts{CSCSA-44056} \sdd{SF-7296} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Last_Path_Point__path_direction_is_lateral_right)
{
   /** \arrange Set up object dependent path borders and a lateral right path. */
   float32_T result;
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LATERAL;

   path.first     = Create_2d_Vector_Coordinates(2.0f, -2.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(3.0f, -3.0f);
   path.direction = PATH_DIRECTION_LAT_RIGHT;

   /** \action Call function to return the last vehicle dependent path border. */
   result = Pt_Get_Last_Path_Point(&path, obj_orientation);

   /** \assert expect that the last lateral component is returned. */
   EXPECT_EQ(result, path.last_mat.y);
}


/**
 * Check whether the correct object position attached to a path is returned. Here a path with an unknown direction is given and
 * thus the point shall be returned based on the object orientation. The object orientation here is lateral. Thus the last lateral
 * component is expected to be returned. \uts{CSCSA-44057} \sdd{SF-7296} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Last_Path_Point__no_path_direction_given_but_object_moves_longitudinal)
{
   /** \arrange Set up a path without a given direction but an object which moves lateral. */
   float32_T result;
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LATERAL;

   path.first     = Create_2d_Vector_Coordinates(2.0f, -2.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(3.0f, -3.0f);
   path.direction = PATH_DIRECTION_NONE;

   /** \action Call function to return the last vehicle dependent path border. */
   result = Pt_Get_Last_Path_Point(&path, obj_orientation);

   /** \assert expect that the last lateral component is returned. */
   EXPECT_EQ(result, path.last_mat.y);
}


/**
 * Check whether the correct object position attached to a path is returned. Here a path with an unknown direction is given and
 * thus the point shall be returned based on the object orientation. The object orientation here is longitudinal. Thus the last
 * longitudinal component is expected to be returned. \uts{CSCSA-44058} \sdd{SF-7296} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Last_Path_Point__no_path_direction_given_but_object_moves_lateral)
{
   /** \arrange Set up a path without a given direction but an object which moves longitudinal. */
   float32_T result;
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;

   path.first     = Create_2d_Vector_Coordinates(2.0f, -2.0f);
   path.last_mat  = Create_2d_Vector_Coordinates(3.0f, -3.0f);
   path.direction = PATH_DIRECTION_NONE;

   /** \action Call function to return the last vehicle dependent path border. */
   result = Pt_Get_Last_Path_Point(&path, obj_orientation);

   /** \assert expect that the last longitudinal component is returned. */
   EXPECT_EQ(result, path.last_mat.x);
}


/**
 * Check whether an obj is implausible by its age increment. Here the check is enabled and the age is incremented. Thus false is
 * expected. \uts{CSCSA-44059} \sdd{SF-7301} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Associated_Obj_Implausible__check_enabled_age_incremented)
{
   /** \arrange Set up an object which has incremented its age accordingly. */
   boolean_T result;
   path.obj_curr_used_for_path_build.age     = 5u;
   object.tracker_data.age                   = path.obj_curr_used_for_path_build.age + 1u;
   cals.k_pt_f_check_object_age_plausibility = FBK_TRUE;

   /** \action Check whether the associated object has increased its age. */
   result = Pt_Is_Associated_Obj_Implausible(&object.tracker_data, &path, &cals);

   /** \assert expect false */
   EXPECT_FALSE(result);
}

/**
 * Check whether an obj is implausible by its age increment. Here the check is enabled and the age has not incremented. Thus true
 * is expected. \uts{CSCSA-44060} \sdd{SF-7301} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Associated_Obj_Implausible__check_enabled_age_has_not_incremented)
{
   /** \arrange Set up an object which has a lower age than the one attached to the path. */
   boolean_T result;
   path.obj_curr_used_for_path_build.age     = 5u;
   object.tracker_data.age                   = path.obj_curr_used_for_path_build.age - 1u;
   cals.k_pt_f_check_object_age_plausibility = FBK_TRUE;

   /** \action Check whether the associated object has increased its age. */
   result = Pt_Is_Associated_Obj_Implausible(&object.tracker_data, &path, &cals);

   /** \assert expect true */
   EXPECT_TRUE(result);
}


/**
 * Check whether an obj is implausible by its age increment. Here the check is disabled. Even though the age is not incremented,
 * false is expected. \uts{CSCSA-44061} \sdd{SF-7301} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Associated_Obj_Implausible__check_is_disabled_but_age_is_not_incremented_correctly)
{
   /** \arrange Set up an object which has a lower age than the one attached to the path. Disable the check */
   boolean_T result;
   path.obj_curr_used_for_path_build.age     = 5u;
   object.tracker_data.age                   = path.obj_curr_used_for_path_build.age - 1u;
   cals.k_pt_f_check_object_age_plausibility = FBK_FALSE;

   /** \action Check whether the associated object has increased its age. */
   result = Pt_Is_Associated_Obj_Implausible(&object.tracker_data, &path, &cals);

   /** \assert expect false */
   EXPECT_FALSE(result);
}

/**
 * Test whether the correct path direction is returned. Here the path direction is already set and thus the direction shall remain.
 * \uts{CSCSA-44062} \sdd{SF-7293} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Calculate_Path_Dir__direction_is_already_set)
{
   /** \arrange Set up a path with an already given direction */
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LATERAL;

   path.direction = PATH_DIRECTION_LONG_FORWARD;
   path.first_p   = 1;
   path.last_p    = path.last_p + 1;

   /** \action call the path direction calculation routine */
   Pt_Calculate_Path_Dir(&path, &obj_orientation);

   /** \assert expect direction to be kept even though the object orientation is different */
   EXPECT_EQ(path.direction, PATH_DIRECTION_LONG_FORWARD);
}


/**
 * Test whether the correct path direction is returned. Here the object is moving longitudinal and the first point index is less
 * than last point. \uts{CSCSA-44063} \sdd{SF-7293} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Calculate_Path_Dir__object_long_first_lt_last)
{
   /** \arrange Set up a longitudinal moving object and first border less than last border. */
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;

   path.direction = PATH_DIRECTION_NONE;
   path.first_p   = 1;
   path.last_p    = path.first_p + 1;

   /** \action call the path direction calculation routine. */
   Pt_Calculate_Path_Dir(&path, &obj_orientation);

   /** \assert expect a direction of longitudinal forward */
   EXPECT_EQ(path.direction, PATH_DIRECTION_LONG_FORWARD);
}

/**
 * Test whether the correct path direction is returned. Here the object is moving lateral and the first point index is less than
 * last point. \uts{CSCSA-44064} \sdd{SF-7293} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Calculate_Path_Dir__object_lat_first_lt_last)
{
   /** \arrange Set up a lateral moving object and first border less than last border. */
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LATERAL;

   path.direction = PATH_DIRECTION_NONE;
   path.first_p   = 1;
   path.last_p    = path.first_p + 1;

   /** \action call the path direction calculation routine. */
   Pt_Calculate_Path_Dir(&path, &obj_orientation);

   /** \assert expect a direction of lateral right to be set. */
   EXPECT_EQ(path.direction, PATH_DIRECTION_LAT_RIGHT);
}

/**
 * Test whether the correct path direction is returned. Here the object is moving longitudinal and the first point index is greater
 * than last point. \uts{CSCSA-44065} \sdd{SF-7293} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Calculate_Path_Dir__object_long_first_gt_last)
{
   /** \arrange Set up a longitudinal moving object and first border greater than last border. */
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LONGITUDINAL;
   path.direction                          = PATH_DIRECTION_NONE;
   path.first_p                            = 2;
   path.last_p                             = path.first_p - 1;

   /** \action call the path direction calculation routine. */
   Pt_Calculate_Path_Dir(&path, &obj_orientation);

   /** \assert expect a direction of longitudinal backward and the borders to be swapped. */
   EXPECT_EQ(path.direction, PATH_DIRECTION_LONG_BACKWARD);
   EXPECT_EQ(path.first_p, 1);
   EXPECT_EQ(path.last_p, 2);
}


/**
 * Test whether the correct path direction is returned. Here the object is moving lateral and the first point index is greater than
 * last point. \uts{CSCSA-44066} \sdd{SF-7293} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Calculate_Path_Dir__object_lat_first_gt_last)
{
   /** \arrange Set up a lateral moving object and first border greater than last border. */
   Pt_Object_Orientation_T obj_orientation = PT_OBJECT_ORIENTATION_LATERAL;

   path.direction = PATH_DIRECTION_NONE;
   path.first_p   = 2;
   path.last_p    = path.first_p - 1;

   /** \action call the path direction calculation routine. */
   Pt_Calculate_Path_Dir(&path, &obj_orientation);

   /** \assert expect a direction of lateral left and the borders to be swapped. */
   EXPECT_EQ(path.direction, PATH_DIRECTION_LAT_LEFT);
   EXPECT_EQ(path.first_p, 1);
   EXPECT_EQ(path.last_p, 2);
}

/**
 * Checks whether the direction matches to the rest of the path properties. Here the first border is greater than the last border.
 * Thus a swap is expected. \uts{CSCSA-44067} \sdd{SF-7294} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Check_Direction_Integrity__swaps_last_border_and_first_border)
{
   /** \arrange Set up a path without a direction and borders which need to be swapped. */
   path.direction = PATH_DIRECTION_NONE;
   path.first_p   = 2;
   path.last_p    = path.first_p - 1;

   /** \action call direction integrity function. */
   Pt_Check_Direction_Integrity(&path);

   /** \assert expect borders to be swapped. */
   EXPECT_GT(path.last_p, path.first_p);
}

/**
 * Checks whether the direction matches to the rest of the path properties. Here the first border is less than the last border.
 * Thus no swap is expected. \uts{CSCSA-44068} \sdd{SF-7294} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Check_Direction_Integrity__dont_swap_last_border_and_first_border_since_already_correct_aligned)
{
   /** \arrange Set up a path without a direction and borders which do not need to be swapped. */
   path.direction = PATH_DIRECTION_NONE;
   path.first_p   = 1;
   path.last_p    = path.first_p + 1;

   /** \action call direction integrity function. */
   Pt_Check_Direction_Integrity(&path);

   /** \assert expect borders to be unchanged. */
   EXPECT_GT(path.last_p, path.first_p);
}

/**
 * Checks whether the direction matches to the rest of the path properties. Here the input path is lateral right but the object
 * dependent borders are mismatching. Thus a direction switch is expected. \uts{CSCSA-44069} \sdd{SF-7294}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Check_Direction_Integrity__switch_lateral_direction_since_object_borders_are_mismatching)
{
   /** \arrange set up a lateral right path with mismatching object borders */
   path.direction  = PATH_DIRECTION_LAT_RIGHT;
   path.last_mat.y = -2.0f;
   path.first.y    = path.last_mat.y + EPSILON;

   /** \action call direction integrity function. */
   Pt_Check_Direction_Integrity(&path);

   /** \assert expect a switch for the direction and a swap for the object borders. */
   EXPECT_GT(path.last_mat.y, path.first.y);
   EXPECT_EQ(path.direction, PATH_DIRECTION_LAT_LEFT);
}

/**
 * Checks whether the direction matches to the rest of the path properties. Here the input path is lateral right and the object
 * dependent borders are matching. Thus no direction switch is expected. \uts{CSCSA-44070} \sdd{SF-7294}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Check_Direction_Integrity__dont_switch_direction_since_lateral_direction_and_borders_are_correct)
{
   /** \arrange set up a correct lateral right path */
   path.direction  = PATH_DIRECTION_LAT_RIGHT;
   path.last_mat.y = 2.0f;
   path.first.y    = path.last_mat.y - EPSILON;

   /** \action call direction integrity function. */
   Pt_Check_Direction_Integrity(&path);

   /** \assert expect path properties to remain. */
   EXPECT_GT(path.last_mat.y, path.first.y);
   EXPECT_EQ(path.direction, PATH_DIRECTION_LAT_RIGHT);
}

/**
 * Checks whether the direction matches to the rest of the path properties. Here the input path is longitudinal forward but the
 * object dependent borders are mismatching. Thus a direction switch is expected. \uts{CSCSA-44071} \sdd{SF-7294}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Check_Direction_Integrity__switch_long_direction_since_object_borders_are_mismatching)
{
   /** \arrange set up a longitudinal path with mismatching path properties */
   path.direction  = PATH_DIRECTION_LONG_FORWARD;
   path.last_mat.x = -2.0f;
   path.first.x    = path.last_mat.x + EPSILON;

   /** \action call direction integrity function. */
   Pt_Check_Direction_Integrity(&path);

   /** \assert expect a switch for the direction and a swap for the object borders. */
   EXPECT_GT(path.last_mat.x, path.first.x);
   EXPECT_EQ(path.direction, PATH_DIRECTION_LONG_BACKWARD);
}


/**
 * Checks whether the direction matches to the rest of the path properties. Here the input path is longitudinal forward but the
 * object dependent borders are mismatching. Thus no direction switch is expected. \uts{CSCSA-44072} \sdd{SF-7294}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Check_Direction_Integrity__dont_switch_direction_since_long_direction_and_borders_are_correct)
{
   /** \arrange set up a longitudinal path without mismatching properties */
   path.direction  = PATH_DIRECTION_LONG_FORWARD;
   path.last_mat.x = 2.0f;
   path.first.x    = path.last_mat.x - EPSILON;

   /** \action call direction integrity function. */
   Pt_Check_Direction_Integrity(&path);

   /** \assert expect the path properties to remain */
   EXPECT_GT(path.last_mat.x, path.first.x);
   EXPECT_EQ(path.direction, PATH_DIRECTION_LONG_FORWARD);
}

/**
 * Tests funtionality of implausibility check for pt_persistent.paths. Here a path is created with a length of 4 points. However
 * these points are all set to zero. Thus the path shall be invalidated. \uts{CSCSA-44073} \sdd{SF-7308}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Reset_Implausible_Path__Invalidate_path_and_associations_to_it)
{
   /** \arrange path to be invalidated */
   path.obj_curr_used_for_path_build.id = 1;
   path.direction                       = PATH_DIRECTION_LONG_FORWARD;
   path.first_p                         = 1;
   path.last_p                          = 5;

   /** \action call function to test */
   Pt_Reset_Implausible_Path(&path, pt_persistent.best_path_obj_pairs, &cals);

   /** \assert expect default values for path */
   EXPECT_EQ(path.first_p, PT_DEFAULT_DISCR_BORDER);
   EXPECT_EQ(path.last_p, PT_DEFAULT_DISCR_BORDER);
}


/**
 * Tests funtionality of implausibility check for pt_persistent.paths. Here a the path does not yet have a defined path direction.
 * Thus the check shall not be executed and the initialized path values shall remain. \uts{CSCSA-44074} \sdd{SF-7308}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Reset_Implausible_Path__path_direction_none_no_implausibility_check)
{
   /** \arrange set up a path without a direction */
   path.obj_curr_used_for_path_build.id = 1;
   path.direction                       = PATH_DIRECTION_NONE;
   path.first_p                         = 1;
   path.last_p                          = 1;

   /** \action call function to test */
   Pt_Reset_Implausible_Path(&path, pt_persistent.best_path_obj_pairs, &cals);

   /** \assert expect path to remain */
   EXPECT_EQ(path.first_p, 1);
   EXPECT_EQ(path.last_p, 1);
}


/**
 * Tests funtionality of implausibility check for pt_persistent.paths. Here the path shall be valid.
 * \uts{CSCSA-44075} \sdd{SF-7308} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Reset_Implausible_Path__path_is_valid)
{
   /** \arrange set up a valid path */
   path.obj_curr_used_for_path_build.id = 1;
   path.direction                       = PATH_DIRECTION_LAT_LEFT;
   path.first_p                         = 1;
   path.last_p                          = 5;

   for (uint8_t i = path.first_p; i <= path.last_p; i++)
   {
      path.path_points[i] = 1.0f;
   }

   /** \action call implausibility check */
   Pt_Reset_Implausible_Path(&path, pt_persistent.best_path_obj_pairs, &cals);

   /** \assert expect initialized values for path */
   EXPECT_EQ(path.first_p, 1);
   EXPECT_EQ(path.last_p, 5);
}

/**
 * Tests funtionality of implausibility check for pt_persistent.paths. Here the path shall be invalid since more non zero path
 * points exist. \uts{CSCSA-44076} \sdd{SF-7308} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Reset_Implausible_Path__path_is_invalid_due_to_too_many_nonzero_path_points)
{
   /** \arrange path to be invalidated */
   path.obj_curr_used_for_path_build.id = 1;
   path.direction                       = PATH_DIRECTION_LAT_LEFT;
   path.first_p                         = 1u;
   path.last_p                          = 5u;

   for (uint8_t i = path.first_p; i <= path.last_p + 3u * PT_SINGLE_GRID_POINT_OFFSET; i++)
   {
      path.path_points[i] = 1.0f;
   }

   /** \action call function to test */
   Pt_Reset_Implausible_Path(&path, pt_persistent.best_path_obj_pairs, &cals);

   /** \assert expect initialized values for path */
   EXPECT_EQ(path.first_p, PT_DEFAULT_DISCR_BORDER);
   EXPECT_EQ(path.last_p, PT_DEFAULT_DISCR_BORDER);
}


/**
 * Tests funtionality of invalidation of far field pt_persistent.paths. Here the path shall be reset since it is too far away from
 * the host. \uts{CSCSA-44077} \sdd{SF-7307} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Reset_Far_Field_Path__path_shall_be_reset)
{
   /** \arrange path to be invalidated */
   path.obj_curr_used_for_path_build.id                                    = FBK_ZERO_UINT;
   path.path_state                                                         = PATH_STATUS_CREATION;
   path.path_points[PT_MID_GRID_POINT_INDEX]                               = 1.1f * cals.k_pt_kill_path_exceed_dist_thres;
   path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] = 1.0f;
   path.path_points[PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] =
      path.path_points[PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET] + 0.9f * cals.k_pt_kill_path_max_diff_posn;

   path.first_p = 1;
   path.last_p  = 3;

   /** \action call function to test */
   Pt_Reset_Far_Field_Path(&path, pt_persistent.best_path_obj_pairs, &cals);

   /** \assert expect initialized values for path */
   EXPECT_EQ(path.first_p, PT_DEFAULT_DISCR_BORDER);
   EXPECT_EQ(path.last_p, PT_DEFAULT_DISCR_BORDER);
}

/**
 * Tests funtionality of invalidation of objects so that they are not able to build up a path. Here the object is too far away
 * laterally. \uts{CSCSA-44078} \sdd{SF-7304} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Object_Out_Of_Tracking_Range__object_is_out_of_range_in_lateral_perspective)
{
   /** \arrange set up an object which is out of range */
   boolean_T res;
   path.direction                = PATH_DIRECTION_LAT_LEFT;
   object.tracker_data.vcs_pos.y = 1.1f * cals.k_pt_path_track_lat_range_limit;

   /** \action call function to test */
   res = Pt_Is_Object_Out_Of_Tracking_Range(&object.tracker_data, &cals);

   /** \assert expect true since object is out of range */
   EXPECT_TRUE(res);
}

/**
 * Tests funtionality of invalidation of objects so that they are not able to build up a path. Here the object is too far away
 * longitudinally. \uts{CSCSA-44079} \sdd{SF-7304} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Object_Out_Of_Tracking_Range__object_is_out_of_range_in_longitudinal_perspective)
{

   /** \arrange object out of range */
   boolean_T res;

   path.direction                = PATH_DIRECTION_LONG_BACKWARD;
   object.tracker_data.vcs_pos.x = 1.1f * cals.k_pt_path_track_long_range_limit;
   /** \action call function to test */
   res = Pt_Is_Object_Out_Of_Tracking_Range(&object.tracker_data, &cals);

   /** \assert expect true since object is out of range */
   EXPECT_TRUE(res);
}


/**
 * Tests funtionality of invalidation of objects so that they are not able to build up a path. Here the object is in range from
 * lateral perspective. \uts{CSCSA-44080} \sdd{SF-7304} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Object_Out_Of_Tracking_Range__object_is_in_range_in_lateral_perspective)
{
   /** \arrange object in range */
   boolean_T res;
   path.direction                = PATH_DIRECTION_LAT_LEFT;
   object.tracker_data.vcs_pos.y = -0.9f * cals.k_pt_path_track_lat_range_limit;

   /** \action call function to test */
   res = Pt_Is_Object_Out_Of_Tracking_Range(&object.tracker_data, &cals);

   /** \assert expect false since object is in range */
   EXPECT_FALSE(res);
}


/**
 * Tests funtionality of invalidation of objects so that they are not able to build up a path. Here the object is in range
 * longitudinally. \uts{CSCSA-44081} \sdd{SF-7304} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Object_Out_Of_Tracking_Range__object_is_in_range_in_longitudinal_perspective)
{
   /** \arrange object in range */
   boolean_T res;
   path.direction                = PATH_DIRECTION_LONG_BACKWARD;
   object.tracker_data.vcs_pos.x = -0.9f * cals.k_pt_path_track_long_range_limit;

   /** \action call function to test */
   res = Pt_Is_Object_Out_Of_Tracking_Range(&object.tracker_data, &cals);

   /** \assert expect false since object is in range */
   EXPECT_FALSE(res);
}


/**
 * Tests funtionality of invalidation of objects so that they are not able to build up a path. Here the object is in range but the
 * path direction is not set yet. \uts{CSCSA-44082} \sdd{SF-7304} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Object_Out_Of_Tracking_Range__path_does_not_have_a_direction)
{

   /** \arrange object in range but unset direction of path */
   boolean_T res;

   path.direction                = PATH_DIRECTION_NONE;
   object.tracker_data.vcs_pos.x = 0.9f * cals.k_pt_path_track_long_range_limit;
   /** \action call function to test */
   res = Pt_Is_Object_Out_Of_Tracking_Range(&object.tracker_data, &cals);

   /** \assert expect false path attribute is not set */
   EXPECT_FALSE(res);
}


/**
 * Tests functionality of path point adding. Here a longitudinal path against vcs is considered. The path is setup in such a way,
 * that the property first of the path and the objects position are surrounding a grid point. Thus it is expected that the path is
 * extending by one point. \uts{CSCSA-44083} \sdd{SF-7311} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Track_Path_Wrapper__long_path_against_vcs_add_a_point)
{
   /** \arrange path where an additional point shall be added */

   uint8_t path_grid_idx = PT_HIGHEST_GRID_POINT_INDEX - 6u * PT_SINGLE_GRID_POINT_OFFSET;
   float32_T path_val    = -10.0f;

   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD,
                         PT_HIGHEST_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET, PT_HIGHEST_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] + 0.5f, path_val),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX], path_val), 0.0f, path_val);

   /*Overwrite border so that a point is added */
   path.first_p                = PT_HIGHEST_GRID_POINT_INDEX - 6 * PT_SINGLE_GRID_POINT_OFFSET;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] - 0.5f, path_val);

   p_vehicle_data->host_speed = 1.0f;

   /** \action call function to test */
   Pt_Track_Path_Wrapper(&path, path_grid_idx, &(path.first.x), &(path.first.y), &(path.last_mat.x), &(path.last_mat.y),
                         &(object.tracker_data.vcs_pos.x), &(object.tracker_data.vcs_pos.y), &pt_input, p_vehicle_data);

   /** \assert expect adding of a point */
   EXPECT_EQ(path.path_points[path_grid_idx], path_val);
   EXPECT_EQ(path.new_path_point_status, PATH_POINT_NEW_FIRST);
}


/**
 * Tests functionality of path point adding. Here a longitudinal path against vcs is considered. The object which build up the path
 * coasted for 2 grid points. Thus missing path information shall be added and also the discrete border shall be adapted.
 * \uts{CSCSA-44084} \sdd{SF-7311} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Track_Path_Wrapper__long_path_against_vcs_object_coasted_for_two_grid_points)
{
   /** \arrange path where an additional point shall be added */
   uint8_t path_grid_idx = PT_HIGHEST_GRID_POINT_INDEX - 6u * PT_SINGLE_GRID_POINT_OFFSET;
   float32_T path_val    = -10.0f;
   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD,
                         PT_HIGHEST_GRID_POINT_INDEX - 5 * PT_SINGLE_GRID_POINT_OFFSET, PT_HIGHEST_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] + THRESHOLD_IS_ZERO, path_val),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX], path_val), 0.0f, path_val);
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] - THRESHOLD_IS_ZERO, path_val);

   p_vehicle_data->host_speed = -0.00001f;

   /** \action call function to test */
   Pt_Track_Path_Wrapper(&path, path_grid_idx, &(path.first.x), &(path.first.y), &(path.last_mat.x), &(path.last_mat.y),
                         &(object.tracker_data.vcs_pos.x), &(object.tracker_data.vcs_pos.y), &pt_input, p_vehicle_data);

   /** \assert expect adding of just one point and adapt discrete border */
   EXPECT_EQ(path.path_points[path_grid_idx], path_val);
   EXPECT_EQ(path.first_p, path_grid_idx);
}


/**
 * Tests functionality of path point adding.Here a longitudinal path against vcs is considered. The first point will be added to
 * the path here.Thus an extension as well as an initialization of the border is expected. \uts{CSCSA-44085} \sdd{SF-7311}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Track_Path_Wrapper__long_path_against_vcs_started_to_build_up)
{
   /** \arrange path where an additional point shall be added */

   uint8_t path_grid_idx       = PT_HIGHEST_GRID_POINT_INDEX;
   float32_T path_val          = -10.0f;
   path.direction              = PATH_DIRECTION_LONG_BACKWARD;
   path.first                  = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] - THRESHOLD_IS_ZERO, path_val);
   path.first_p                = PT_DEFAULT_DISCR_BORDER;
   path.last_mat               = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] + 2.5f, path_val);
   path.last_p                 = PT_DEFAULT_DISCR_BORDER;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] + THRESHOLD_IS_ZERO, path_val);

   p_vehicle_data->host_speed = 0.0f;

   /** \action call function to test */
   Pt_Track_Path_Wrapper(&path, path_grid_idx, &(path.first.x), &(path.first.y), &(path.last_mat.x), &(path.last_mat.y),
                         &(object.tracker_data.vcs_pos.x), &(object.tracker_data.vcs_pos.y), &pt_input, p_vehicle_data);

   /** \assert expect adding of just one point and adapt discrete border */
   EXPECT_EQ(path.path_points[path_grid_idx], path_val);
   EXPECT_EQ(path.last_p, path_grid_idx);
}


/**
 * Tests functionality of path point adding. Here a longitudinal path aligned with vcs is considered. The path is setup in such a
 * way, that the property first of the path and the objects position are surrounding a grid point.Thus it is expected that the path
 * is extending by one point. \uts{CSCSA-44086} \sdd{SF-7311} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Track_Path_Wrapper__long_path_aligned_with_vcs_add_a_point)
{
   /** \arrange path where an additional point shall be added */

   uint8_t path_grid_idx = PT_LOWEST_GRID_POINT_INDEX + 6u * PT_SINGLE_GRID_POINT_OFFSET;
   float32_T path_val    = -10.0f;

   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_LOWEST_GRID_POINT_INDEX + 5 * PT_SINGLE_GRID_POINT_OFFSET, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX], path_val),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] + 0.5f, path_val), 0.0f, path_val);
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] - 0.5f, path_val);
   /* Override for a point adding*/
   path.last_p = PT_LOWEST_GRID_POINT_INDEX + 6 * PT_SINGLE_GRID_POINT_OFFSET;

   p_vehicle_data->host_speed = 1.0f;

   /** \action call function to test */
   Pt_Track_Path_Wrapper(&path, path_grid_idx, &(path.first.x), &(path.first.y), &(path.last_mat.x), &(path.last_mat.y),
                         &(object.tracker_data.vcs_pos.x), &(object.tracker_data.vcs_pos.y), &pt_input, p_vehicle_data);

   /** \assert expect adding of a point */
   EXPECT_EQ(path.path_points[path_grid_idx], path_val);
   EXPECT_EQ(path.new_path_point_status, PATH_POINT_NEW_LAST);
}


/**
 * Tests functionality of path point adding. Here a longitudinal path aligned with vcs is considered. The object which build up the
 * path coasted for 2 grid points. Thus missing path information shall be added and also the discrete border shall be adapted.
 * \uts{CSCSA-44087} \sdd{SF-7311} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Track_Path_Wrapper__long_path_aligned_with_vcs_object_coasted_for_two_grid_points)
{
   /** \arrange path where an additional point shall be added */

   uint8_t path_grid_idx = PT_LOWEST_GRID_POINT_INDEX + 6u * PT_SINGLE_GRID_POINT_OFFSET;
   float32_T path_val    = -10.0f;
   Pt_Init_Path_Linearly(&path, pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_DEFAULT_DISCR_BORDER, PT_DEFAULT_DISCR_BORDER,
                         0, Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX], path_val),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] + THRESHOLD_IS_ZERO, path_val), 0.0f,
                         path_val);

   path.direction              = PATH_DIRECTION_LONG_FORWARD;
   object.tracker_data.vcs_pos = Create_2d_Vector_Coordinates(pt_input.grid_pt_array[path_grid_idx] - THRESHOLD_IS_ZERO, path_val);

   p_vehicle_data->host_speed = -0.00001f;

   /** \action call function to test */
   Pt_Track_Path_Wrapper(&path, path_grid_idx, &(path.first.x), &(path.first.y), &(path.last_mat.x), &(path.last_mat.y),
                         &(object.tracker_data.vcs_pos.x), &(object.tracker_data.vcs_pos.y), &pt_input, p_vehicle_data);

   /** \assert expect adding of just one point and adapt discrete border */
   EXPECT_EQ(path.path_points[path_grid_idx], path_val);
   EXPECT_EQ(path.first_p, path_grid_idx);
}


/**
 * Tests functionality of path tracking where pt_persistent.paths are removed when all slots are occupied. Here the path slot of
 * the path which is farer away shall be removed. \uts{CSCSA-44088} \sdd{SF-7305} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Make_Path_Room__return_slot_of_path_which_is_farer_away_negative_path_value)
{
   /** \arrange two pt_persistent.paths with the same amount of grouping and same length but different distance to host */
   uint8_t idx_returned;
   float32_T path_val_nearer_path       = -5.0f;
   float32_T path_val_further_away_path = -10.0f;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]
                                                         - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_nearer_path),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                         + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_nearer_path),
                         0.0f, path_val_nearer_path);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]
                                                         - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_further_away_path),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                         + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_further_away_path),
                         0.0f, path_val_further_away_path);


   /** \action call function to test */
   idx_returned = Pt_Make_Path_Room(&pt_persistent);

   /** \assert expect index 1 */
   EXPECT_EQ(idx_returned, 1);
}


/**
 * Tests functionality of path tracking where pt_persistent.paths are removed when all slots are occupied. Here the path slot 0
 * shall be returned, since path at slot 1 is nearer. \uts{CSCSA-44089} \sdd{SF-7305} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Make_Path_Room__return_slot_of_path_0_since_path_at_1_is_nearer)
{
   /** \arrange two pt_persistent.paths with the same amount of grouping and same length but different distance to host */
   uint8_t idx_returned;
   float32_T path_val_further_away_path = 10.0f;
   float32_T path_val_nearer_path       = 5.0f;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]
                                                         - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_further_away_path),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                         + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_further_away_path),
                         0.0f, path_val_further_away_path);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]
                                                         - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_nearer_path),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                         + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_nearer_path),
                         0.0f, path_val_nearer_path);


   /** \action call function to test */
   idx_returned = Pt_Make_Path_Room(&pt_persistent);

   /** \assert expect index 0 */
   EXPECT_EQ(idx_returned, 0);
}


/**
 * Tests functionality of path tracking where pt_persistent.paths are removed when all slots are occupied. Here the path slot of
 * the path which is shorter shall be removed. \uts{CSCSA-44090} \sdd{SF-7305} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Make_Path_Room__return_slot_of_path_which_is_shorter)
{
   /** \arrange two pt_persistent.paths with the same amount of grouping and same length but different distance to host */
   uint8_t idx_returned;
   float32_T path_val = -5.0f;

   Pt_Init_Path_Linearly(
      &pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
      PT_HIGHEST_GRID_POINT_INDEX, 0,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      0.0f, path_val);

   Pt_Init_Path_Linearly(
      &pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
      PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET, 0,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      0.0f, path_val);


   /** \action call function to test */
   idx_returned = Pt_Make_Path_Room(&pt_persistent);

   /** \assert expect index 1 */
   EXPECT_EQ(idx_returned, 1);
}

/**
 * Tests functionality of path tracking where pt_persistent.paths are removed when all slots are occupied. Here the path slot 0
 * shall be removed since path at slot 1 is longer than the one at 0. \uts{CSCSA-44091} \sdd{SF-7305}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Make_Path_Room__return_slot_zero_since_path_at_1_is_longer)
{
   /** \arrange two pt_persistent.paths with the same amount of grouping and same length but different distance to host */
   uint8_t idx_returned;
   float32_T path_val = -5.0f;

   Pt_Init_Path_Linearly(
      &pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_SINGLE_GRID_POINT_OFFSET,
      PT_HIGHEST_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET, 0,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      0.0f, path_val);

   Pt_Init_Path_Linearly(
      &pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
      PT_HIGHEST_GRID_POINT_INDEX, 0,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      0.0f, path_val);


   /** \action call function to test */
   idx_returned = Pt_Make_Path_Room(&pt_persistent);

   /** \assert expect index 0 */
   EXPECT_EQ(idx_returned, 0);
}

/**
 * Tests functionality of path tracking where pt_persistent.paths are removed when all slots are occupied. Here the path slot of
 * the path which was grouped the least shall be returned. \uts{CSCSA-44092} \sdd{SF-7305} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Make_Path_Room__return_slot_of_path_which_has_been_grouped_least)
{
   /** \arrange two pt_persistent.paths with the same amount of grouping and same length but different distance to host */
   uint8_t idx_returned;
   float32_T path_val = -5.0f;

   Pt_Init_Path_Linearly(
      &pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
      PT_HIGHEST_GRID_POINT_INDEX, 5u,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      0.0f, path_val);

   Pt_Init_Path_Linearly(
      &pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
      PT_HIGHEST_GRID_POINT_INDEX, 1u,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      0.0f, path_val);


   /** \action call function to test */
   idx_returned = Pt_Make_Path_Room(&pt_persistent);

   /** \assert expect index 1 */
   EXPECT_EQ(idx_returned, 1);
}

/**
 * Tests functionality of path tracking where pt_persistent.paths are removed when all slots are occupied. Here the second path has
 * a greater amount of grouping and thus the first path slot shall be returned. \uts{CSCSA-44093} \sdd{SF-7305}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Make_Path_Room__return_slot_of_first_path_since_second_path_has_greater_grouping_amount)
{
   /** \arrange two pt_persistent.paths with the same amount of grouping and same length but different distance to host */
   uint8_t idx_returned;
   float32_T path_val_nearer_path       = -5.0f;
   float32_T path_val_further_away_path = -10.0f;

   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 0,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]
                                                         - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_nearer_path),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                         + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_nearer_path),
                         0.0f, path_val_nearer_path);
   Pt_Init_Path_Linearly(&pt_persistent.paths[1], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
                         PT_HIGHEST_GRID_POINT_INDEX, 3,
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]
                                                         - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_further_away_path),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                         + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                      path_val_further_away_path),
                         0.0f, path_val_further_away_path);


   /** \action call function to test */
   idx_returned = Pt_Make_Path_Room(&pt_persistent);

   /** \assert expect index 0 */
   EXPECT_EQ(idx_returned, 0);
}

/**
 * Tests functionality of path tracking where pt_persistent.paths are removed when all slots are occupied. Here all available path
 * slots are in build up process and thus are not allowed to be deleted. A default value is expected to be returned.
 * \uts{CSCSA-44094} \sdd{SF-7305} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Make_Path_Room__no_path_shall_be_deleted_since_all_are_in_build_process)
{
   /** \arrange two pt_persistent.paths with the same amount of grouping and same length but different distance to host */
   uint8_t idx_returned;
   float32_T path_val_nearer_path = -5.0f;

   for (uint8_t i = 0; i < PT_NUMBER_OF_PATHS; i++)
   {
      Pt_Init_Path_Linearly(&pt_persistent.paths[i], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD,
                            PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 0,
                            Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX]
                                                            - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                         path_val_nearer_path),
                            Create_2d_Vector_Coordinates(pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX]
                                                            + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                                         path_val_nearer_path),
                            0.0f, path_val_nearer_path);
      pt_persistent.paths[i].path_state                      = PATH_STATUS_CREATION;
      pt_persistent.paths[i].obj_curr_used_for_path_build.id = i + 1u;
   }

   /** \action call function to test */
   idx_returned = Pt_Make_Path_Room(&pt_persistent);

   /** \assert expect an uninitialized path index */
   EXPECT_EQ(idx_returned, PT_DEFAULT_MATCH_INDEX);
}


/**
 * Check whether path priorities are initialized correctly.
 * \uts{CSCSA-44095} \sdd{SF-7300} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Pt_Init_Path_Priority_Metric__initialization_set_up_correctly)
{
   /** \arrange set up path priority metric */
   Pt_Path_Priority_Metrics_T path_priority_metric;

   /** \action call function to test */
   Pt_Pt_Init_Path_Priority_Metric(&path_priority_metric);

   /** \assert expect correctly initialized metrics */
   EXPECT_EQ(path_priority_metric.path_slot_to_liberate, PT_DEFAULT_MATCH_INDEX);
   EXPECT_EQ(path_priority_metric.temp_amount_path_grouping, PT_TEMP_GROUPING_AMOUNT_PATH_PRIORITY);
   EXPECT_EQ(path_priority_metric.temp_dist_to_host, PT_TEMP_DISTANCE_TO_HOST_PATH_PRIORITY);
   EXPECT_EQ(path_priority_metric.temp_path_length, PT_TEMP_LENGTH_OF_PATH_HOST_PATH_PRIORITY);
}


/**
 * Check whether path priorities are initialized correctly.
 * \uts{CSCSA-44096} \sdd{SF-7309} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Set_Path_Priority_Metric__setter_check)
{
   /** \arrange set up path priority metric */
   Pt_Path_Priority_Metrics_T path_priority_metric;
   path.path_points[PT_MID_GRID_POINT_INDEX] = 5.0f;
   path.first_p                              = PT_MID_GRID_POINT_INDEX - PT_SINGLE_GRID_POINT_OFFSET;
   path.last_p                               = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   path.num_groupings                        = 3;


   /** \action call function to test */
   Pt_Set_Path_Priority_Metric(&path_priority_metric, &path, 1);

   /** \assert expect correctly set path priority metric */
   EXPECT_EQ(path_priority_metric.path_slot_to_liberate, 1);
   EXPECT_EQ(path_priority_metric.temp_amount_path_grouping, 3);
   EXPECT_EQ(path_priority_metric.temp_dist_to_host, path.path_points[PT_MID_GRID_POINT_INDEX]);
   EXPECT_EQ(path_priority_metric.temp_path_length, 3);
}


/**
 * Searches for an available path slot for path creation. Here empty path slots are available. Thus the first one shall be used.
 * \uts{CSCSA-44097} \sdd{SF-7295} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Find_Available_Path_Slot__use_the_first_free_slot)
{
   /** \arrange use empty path array */
   uint8_t idx_returned;

   /** \action call function to test */
   idx_returned = Pt_Find_Available_Path_Slot(&pt_persistent);

   /** \assert expect index 0 since this is the first free slot */
   EXPECT_EQ(idx_returned, 0);
}

/**
 * Searches for an available path slot for path creation. Some of them are filled. Thus the first empty slot shall be used for
 * further path creation. \uts{CSCSA-44098} \sdd{SF-7295} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Find_Available_Path_Slot__use_slot_5_since_the_first_4_are_occupied)
{
   /** \arrange use empty path array */
   uint8_t idx_returned;
   float32_T path_val = -5.0f;
   for (uint8_t i = 0; i < 5u; i++)
   {
      Pt_Init_Path_Linearly(
         &pt_persistent.paths[i], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
         PT_HIGHEST_GRID_POINT_INDEX, 5,
         Create_2d_Vector_Coordinates(
            pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
         Create_2d_Vector_Coordinates(
            pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
         0.0f, path_val);
   }
   /** \action call function to test */
   idx_returned = Pt_Find_Available_Path_Slot(&pt_persistent);

   /** \assert expect an uninitialized path index */
   EXPECT_EQ(idx_returned, 5);
}

/**
 * Searches for an available path slot for path creation. Here each slot is blocked. Thus the one path needs to be deleted (the
 * last one, since it has the lowest grouping). \uts{CSCSA-44099} \sdd{SF-7295} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Find_Available_Path_Slot__use_the_last_slot_since_this_is_freed)
{
   /** \arrange use empty path array */
   uint8_t idx_returned;
   float32_T path_val = -5.0f;

   for (uint8_t i = 0; i < PT_NUMBER_OF_PATHS - 1u; i++)
   {
      Pt_Init_Path_Linearly(
         &pt_persistent.paths[i], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
         PT_HIGHEST_GRID_POINT_INDEX, 5,
         Create_2d_Vector_Coordinates(
            pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
         Create_2d_Vector_Coordinates(
            pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
         0.0f, path_val);
   }

   Pt_Init_Path_Linearly(
      &pt_persistent.paths[PT_NUMBER_OF_PATHS - 1u], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD,
      PT_LOWEST_GRID_POINT_INDEX, PT_HIGHEST_GRID_POINT_INDEX, 2,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
      0.0f, path_val);

   /** \action call function to test */
   idx_returned = Pt_Find_Available_Path_Slot(&pt_persistent);

   /** \assert expect an uninitialized path index */
   EXPECT_EQ(idx_returned, PT_NUMBER_OF_PATHS - 1u);
}


/**
 * Call the book keeping function for all pt_persistent.paths. Create two identical pt_persistent.paths as input and expect them to
 * be grouped. \uts{CSCSA-44100} \sdd{SF-7292} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Book_Keep_Paths__call_the_grouping_as_superordinate_function)
{
   /** \arrange create two identical pt_persistent.paths */
   float32_T path_val = 1.0f;
   for (uint8_t i = 0; i < 2; i++)
   {
      Pt_Init_Path_Linearly(
         &pt_persistent.paths[i], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
         PT_HIGHEST_GRID_POINT_INDEX, 2,
         Create_2d_Vector_Coordinates(
            pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
         Create_2d_Vector_Coordinates(
            pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], path_val),
         0.0f, path_val);
      pt_persistent.paths[i].path_state = PATH_STATUS_MATURE;
      pt_persistent.paths[i].path_index = i;
      pt_persistent.paths[i].path_age   = i;
   }

   /** \action call book keeping of pt_persistent.paths */
   Pt_Book_Keep_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert expect pt_persistent.paths to be grouped */
   EXPECT_EQ(pt_persistent.paths[0].path_age, 2u);
   EXPECT_EQ(pt_persistent.paths[0].path_state, PATH_STATUS_GROUPED);
   EXPECT_EQ(pt_persistent.paths[1].path_age, 0u);
   EXPECT_EQ(pt_persistent.paths[1].path_state, PATH_STATUS_DEFAULT);
}

/**
 * Tests overall path rotation module. Here a path consisting of two points is given. Due to the shift, the path will shrink to a
 * not allowed amount of path points. Thus a reset is expected \uts{CSCSA-44101} \sdd{SF-7292} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Book_Keep_Paths__rotate_a_single_path)
{
   /** \arrange Set up a single path which has valid path information and a single shift of 1 meter. */
   for (uint8_t idx = 0u; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_DEFAULT;
   }

   pt_persistent.paths[1u].path_state                            = PATH_STATUS_MATURE;
   pt_persistent.paths[1u].first_p                               = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[1u].last_p                                = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1u].new_path_point_status                 = PATH_POINT_NEW_MATURE;
   pt_persistent.paths[1u].path_border_status                    = PATH_BORDER_POINTS_MATURE;
   pt_persistent.paths[1u].direction                             = PATH_DIRECTION_LAT_RIGHT;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_path_length_after_rot = 2u;

   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_MID_GRID_POINT_INDEX; idx++)
   {
      pt_persistent.paths[1u].path_points[idx] = 5.0f;
   }
   pt_persistent.paths[1u].first = Create_2d_Vector_Coordinates(5.0f, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 2.5f);
   pt_persistent.paths[1u].last_mat = Create_2d_Vector_Coordinates(5.0f, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 2.5f);
   /*Set up a shift.*/
   p_vehicle_data->yawrate      = 0.0f;
   p_vehicle_data->host_speed   = 20.0f;
   data.time_diff_to_last_cycle = 0.05f;

   /** \action call book keeping function. */
   Pt_Book_Keep_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Check that a shift has been applied. */
   for (uint8_t point_idx = PT_LOWEST_GRID_POINT_INDEX; point_idx <= PT_MID_GRID_POINT_INDEX; point_idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[point_idx], 4.0f);
   }
}


/**
 * Call Path completion function. Here a path is given, which is too short. Thus a reset is expected.
 * \uts{CSCSA-44102} \sdd{SF-7306} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Process_Incomplete_Path__call_the_grouping_as_superordinate_function)
{
   /** \arrange set up a short path candidate. */
   uint8_t start_point                                         = PT_MID_GRID_POINT_INDEX;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_max_diff_num_path_point = 4;
   uint8_t end_point = PT_MID_GRID_POINT_INDEX + pt_input.Num_Grid_Pts_Dep_Cals.k_pt_max_diff_num_path_point - 1u;

   Pt_Init_Path_Linearly(
      &pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, start_point, end_point, 2,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], 1.0f),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], 1.0f),
      0.0f, 1.0f);
   pt_persistent.paths[0].path_state = PATH_STATUS_CREATION;

   /** \action call completition of pt_persistent.paths function. */
   Pt_Process_Incomplete_Path(&pt_persistent.paths[0], &pt_persistent, &pt_input, &cals, p_vehicle_data);

   /** \assert expect the path to be reset */
   EXPECT_EQ(pt_persistent.paths[0].path_state, PATH_STATUS_DEFAULT);
}


/**
 * Tests the implausibility check of extrapolated pt_persistent.paths. Here the longitudinal path is defined on the complete grid
 * and thus false is expected. \uts{CSCSA-44103} \sdd{SF-7585} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Extrapolated_Path_Part_Implausible__long_path_is_completely_defined)
{
   /** \arrange set up path which is defined on the whole grid. */
   boolean_T res;
   cals.k_pt_range_nearest_border_impl_path = 4u;
   Pt_Init_Path_Linearly(
      &pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX,
      PT_HIGHEST_GRID_POINT_INDEX, 0,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], 1.0f),
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_HIGHEST_GRID_POINT_INDEX] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], 1.0f),
      0.0f, cals.k_pt_host_implausibilty_range + EPSILON);
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;

   /** \action call extrapolated path plausibility check. */
   res = Pt_Is_Extrapolated_Path_Part_Implausible(&pt_persistent.paths[0], &cals, p_vehicle_data);

   /** \assert expect false. */
   EXPECT_FALSE(res);
}


/**
 * Tests the implausibility check of extrapolated pt_persistent.paths. Here the longitudinal path is plausible.
 * \uts{CSCSA-44104} \sdd{SF-7585} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Extrapolated_Path_Part_Implausible__long_path_is_implausible)
{
   /** \arrange set up path which is defined on the whole grid. */
   boolean_T res;
   cals.k_pt_range_nearest_border_impl_path = 4u;
   uint8_t upper_boundary = PT_LOWEST_GRID_POINT_INDEX + cals.k_pt_range_nearest_border_impl_path - PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Init_Path_Linearly(
      &pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_FORWARD, PT_LOWEST_GRID_POINT_INDEX, upper_boundary, 0,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], 1.0f),
      Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_boundary] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                   1.0f),
      0.0f, cals.k_pt_host_implausibilty_range - EPSILON);
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;

   /** \action call extrapolated path plausibility check. */
   res = Pt_Is_Extrapolated_Path_Part_Implausible(&pt_persistent.paths[0], &cals, p_vehicle_data);

   /** \assert expect true. */
   EXPECT_TRUE(res);
}


/**
 * Tests the implausibility check of extrapolated pt_persistent.paths. Here the lateral path is implausible.
 * \uts{CSCSA-44105} \sdd{SF-7585} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Extrapolated_Path_Part_Implausible__lateral_path_is_implausible)
{
   /** \arrange set up path which is defined on the whole grid. */
   boolean_T res;
   cals.k_pt_range_nearest_border_impl_path = 4u;
   uint8_t upper_boundary = PT_LOWEST_GRID_POINT_INDEX + cals.k_pt_range_nearest_border_impl_path - PT_SINGLE_GRID_POINT_OFFSET;
   Pt_Init_Path_Linearly(
      &pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LAT_RIGHT, PT_LOWEST_GRID_POINT_INDEX, upper_boundary, 0,
      Create_2d_Vector_Coordinates(
         pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX], 1.0f),
      Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_boundary] + 0.5f * pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX],
                                   1.0f),
      0.0f, cals.k_pt_host_implausibilty_range - EPSILON);
   pt_persistent.paths[0].path_state = PATH_STATUS_MATURE;

   /** \action call extrapolated path plausibility check. */
   res = Pt_Is_Extrapolated_Path_Part_Implausible(&pt_persistent.paths[0], &cals, p_vehicle_data);

   /** \assert expect true. */
   EXPECT_TRUE(res);
}


/**
 * Tests the implausibility check of extrapolated pt_persistent.paths. Longitudinal path from front is implausible.
 * \uts{CSCSA-44106} \sdd{SF-7585} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Extrapolated_Path_Part_Implausible__longitudinal_path_from_front)
{
   /** \arrange set up path which is defined on the whole grid. */
   boolean_T res;
   uint8_t upper_boundary                   = PT_HIGHEST_GRID_POINT_INDEX;
   uint8_t lower_boundary                   = PT_MID_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET;
   cals.k_pt_host_implausibilty_range       = 0.0f;
   cals.k_pt_range_nearest_border_impl_path = 1u;
   Pt_Init_Path_Linearly(&pt_persistent.paths[0], pt_input.grid_pt_array, PATH_DIRECTION_LONG_BACKWARD, lower_boundary,
                         upper_boundary, 0, Create_2d_Vector_Coordinates(pt_input.grid_pt_array[lower_boundary] - 0.5f, 0.0f),
                         Create_2d_Vector_Coordinates(pt_input.grid_pt_array[upper_boundary] + 0.5f, 0.0f), 0.0f, 0.0f);

   /** \action call extrapolated path plausibility check. */
   res = Pt_Is_Extrapolated_Path_Part_Implausible(&pt_persistent.paths[0], &cals, p_vehicle_data);

   /** \assert expect true. */
   EXPECT_TRUE(res);
}


/**
 * Tests whether the object to path association finding works directly on platform abstraction data. Here an association is
 * expected. \uts{CSCSA-44107} \sdd{SF-7641} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Path_By_Abstraction_Layer__object_has_path)
{
   /** \arrange set up association. */
   uint8_t obj_index = 1u;
   uint8_t obj_id    = 23u;
   uint8_t path_idx  = 5u;
   Pt_Path_T *p_path;

   data.object_data[obj_index].id                          = obj_id;
   pt_persistent.paths[5u].path_index                      = path_idx;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.id = obj_id;

   /** \action function to test. */
   p_path = Pt_Get_Path_By_Abstraction_Layer(&pt_persistent, &pt_input, obj_index);

   /** \assert expect association of object id 23 to path index 5. */
   EXPECT_EQ(p_path->path_index, path_idx);
}


/**
 * Tests whether the object to path association finding works directly on platform abstraction data. Here the object became invalid
 * and thus no association is expected. \uts{CSCSA-44108} \sdd{SF-7641} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Path_By_Abstraction_Layer__object_is_invalid)
{
   /** \arrange set up invalid object and an association with invalid data. */
   uint8_t obj_index = 1u;
   uint8_t obj_id    = PA_INVALID_OBJ_ID;
   uint8_t path_idx  = 5u;
   Pt_Path_T *p_path;

   data.object_data[obj_index].id                          = obj_id;
   pt_persistent.paths[5u].path_index                      = path_idx;
   pt_persistent.paths[5u].obj_curr_used_for_path_build.id = obj_id;

   /** \action function to test. */
   p_path = Pt_Get_Path_By_Abstraction_Layer(&pt_persistent, &pt_input, obj_index);

   /** \assert expect nullptr. */
   EXPECT_EQ(p_path, nullptr);
}


/**
 * Tests whether the object to path association finding works directly on platform abstraction data. Here an association is not
 * expected. \uts{CSCSA-44109} \sdd{SF-7641} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Get_Path_By_Abstraction_Layer__object_has_not_yet_a_path)
{
   /** \arrange set up object without any path creation. */
   uint8_t obj_index = 1u;
   uint8_t obj_id    = 23u;
   Pt_Path_T *p_path;

   data.object_data[obj_index].id = obj_id;

   /** \action function to test. */
   p_path = Pt_Get_Path_By_Abstraction_Layer(&pt_persistent, &pt_input, obj_index);

   /** \assert expect null pointer. */
   EXPECT_EQ(p_path, nullptr);
}

/**
 * Test the main path tracking function and verify that default output is given in case that only invalid objects are passed.
 * \uts{CSCSA-44110} \sdd{SF-7406} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Run_Path_Tracking__verify_that_default_input_creates_default_output)
{
   /* \arrange Set all objects to invalid state. */
   Pt_Output_T pt_output{};

   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      object_data[idx].status     = PA_OBJ_STATUS_INVALID;
      object_data[idx].f_moveable = FBK_TRUE;
   }

   /* \action Call main function of path tracking. */
   Pt_Run_Path_Tracking(&pt_output, &pt_persistent, &pt_input, &cals, p_vehicle_data);

   /* \assert Verify that no match is existing. */
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(pt_output.path_obj_pair_output[idx].track_match, PT_DEFAULT_MATCH_INDEX);
   }
}


/**
 * Tests pt core run routine. Here the Host vehicle is too fast. Thus Pt gets reinitialized.
 * \uts{CSCSA-44111} \sdd{SF-7643} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Core_Run__pt_was_running_before_but_host_is_too_fast)
{
   /* \arrange Set pt state to executed and host speed to a high value. */
   Pt_Output_T pt_output{};

   pt_persistent.f_was_pt_executed = FBK_TRUE;
   p_vehicle_data->host_speed      = 1.1f * cals.k_pt_en_algo_max_val_active;

   /* \action Call core run routine of Pt. */
   Pt_Core_Run(&pt_output, &pt_persistent, &pt_input, &cals);

   /* \assert Expect that the persistent flag is set to false again. */
   EXPECT_FALSE(pt_persistent.f_was_pt_executed);
}

/**
 * Checks whether the object is valid for path creation. Here the objects existence probability is not sufficient. Thus false is
 * expected. \uts{CSCSA-44112} \sdd{SF-7303} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Is_Obj_Valid_For_Path_Creation__existence_prob_is_too_low_with_reflection)
{
   /** \arrange Setup an object with low existence probability. */
   boolean_T result;
   object.tracker_data.f_reflection          = FBK_TRUE;
   object.tracker_data.existence_probability = cals.k_pt_min_exist_prob_to_be_valid - EPSILON;
   object.tracker_data.speed                 = cals.k_pt_min_obj_speed;

   /** \action call function to test. */
   result = Pt_Is_Obj_Valid_For_Path_Creation(&object.tracker_data, &cals);

   /** \assert Expect false. */
   EXPECT_FALSE(result);
}

/**
 * Tests overall path rotation module. Here a path consisting of two points is given. Due to the shift, the path will shrink to a
 * not allowed amount of path points. Thus a reset is expected \uts{CSCSA-44113} \sdd{SF-7292} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Book_Keep_Paths__rotate_a_single_path_disable_cal)
{
   /** \arrange Set up a single path which has valid path information and a single shift of 1 meter. */
   for (uint8_t idx = 0u; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_DEFAULT;
   }

   pt_persistent.paths[1u].path_state                            = PATH_STATUS_MATURE;
   pt_persistent.paths[1u].first_p                               = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[1u].last_p                                = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1u].new_path_point_status                 = PATH_POINT_NEW_MATURE;
   pt_persistent.paths[1u].path_border_status                    = PATH_BORDER_POINTS_MATURE;
   pt_persistent.paths[1u].direction                             = PATH_DIRECTION_LAT_RIGHT;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_path_length_after_rot = 2u;

   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_MID_GRID_POINT_INDEX; idx++)
   {
      pt_persistent.paths[1u].path_points[idx] = 5.0f;
   }
   pt_persistent.paths[1u].first = Create_2d_Vector_Coordinates(5.0f, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 2.5f);
   pt_persistent.paths[1u].last_mat = Create_2d_Vector_Coordinates(5.0f, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 2.5f);
   /*Set up a shift.*/
   p_vehicle_data->yawrate      = 0.0f;
   p_vehicle_data->host_speed   = 20.0f;
   data.time_diff_to_last_cycle = 0.05f;
   cals.k_pt_f_apply_move_point = false;

   /** \action call book keeping function. */
   Pt_Book_Keep_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Check that a shift has been applied. */
   for (uint8_t point_idx = PT_LOWEST_GRID_POINT_INDEX; point_idx <= PT_MID_GRID_POINT_INDEX; point_idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[point_idx], 5.0f);
   }
}

/**
 * Tests overall path rotation module. Here a path consisting of two points is given. Due to the shift, the path will shrink to a
 * not allowed amount of path points. Thus a reset is expected \uts{CSCSA-44114} \sdd{SF-7292} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Book_Keep_Paths__rotate_a_single_path_negative_speed)
{
   /** \arrange Set up a single path which has valid path information and a single shift of 1 meter. */
   for (uint8_t idx = 0u; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_DEFAULT;
   }

   pt_persistent.paths[1u].path_state                            = PATH_STATUS_MATURE;
   pt_persistent.paths[1u].first_p                               = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[1u].last_p                                = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1u].new_path_point_status                 = PATH_POINT_NEW_MATURE;
   pt_persistent.paths[1u].path_border_status                    = PATH_BORDER_POINTS_MATURE;
   pt_persistent.paths[1u].direction                             = PATH_DIRECTION_LAT_RIGHT;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_path_length_after_rot = 2u;

   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_MID_GRID_POINT_INDEX; idx++)
   {
      pt_persistent.paths[1u].path_points[idx] = 5.0f;
   }
   pt_persistent.paths[1u].first = Create_2d_Vector_Coordinates(5.0f, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 2.5f);
   pt_persistent.paths[1u].last_mat = Create_2d_Vector_Coordinates(5.0f, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 2.5f);
   /*Set up a shift.*/
   p_vehicle_data->yawrate      = 0.0f;
   p_vehicle_data->host_speed   = -20.0f;
   data.time_diff_to_last_cycle = 0.05f;

   /** \action call book keeping function. */
   Pt_Book_Keep_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Check that a shift has been applied. */
   for (uint8_t point_idx = PT_LOWEST_GRID_POINT_INDEX; point_idx <= PT_MID_GRID_POINT_INDEX; point_idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[point_idx], 6.0f);
   }
}

/**
 * Tests overall path rotation module. Here a path consisting of two points is given. Due to the shift, the path will shrink to a
 * not allowed amount of path points. Thus a reset is expected \uts{CSCSA-44115} \sdd{SF-7292} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Book_Keep_Paths__rotate_a_single_path_negative_yawrate)
{
   /** \arrange Set up a single path which has valid path information and a single shift of 1 meter. */
   for (uint8_t idx = 0u; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state = PATH_STATUS_DEFAULT;
   }

   pt_persistent.paths[1u].path_state                            = PATH_STATUS_MATURE;
   pt_persistent.paths[1u].first_p                               = PT_LOWEST_GRID_POINT_INDEX;
   pt_persistent.paths[1u].last_p                                = PT_MID_GRID_POINT_INDEX;
   pt_persistent.paths[1u].new_path_point_status                 = PATH_POINT_NEW_MATURE;
   pt_persistent.paths[1u].path_border_status                    = PATH_BORDER_POINTS_MATURE;
   pt_persistent.paths[1u].direction                             = PATH_DIRECTION_LAT_RIGHT;
   pt_input.Num_Grid_Pts_Dep_Cals.k_pt_min_path_length_after_rot = 2u;

   for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_MID_GRID_POINT_INDEX; idx++)
   {
      pt_persistent.paths[1u].path_points[idx] = 5.0f;
   }
   pt_persistent.paths[1u].first = Create_2d_Vector_Coordinates(5.0f, pt_input.grid_pt_array[PT_LOWEST_GRID_POINT_INDEX] - 2.5f);
   pt_persistent.paths[1u].last_mat = Create_2d_Vector_Coordinates(5.0f, pt_input.grid_pt_array[PT_MID_GRID_POINT_INDEX] + 2.5f);
   /*Set up a shift.*/
   p_vehicle_data->yawrate      = -Fbk_Half(cals.k_pt_apply_move_point_min_yaw_rate);
   p_vehicle_data->host_speed   = 0.0f;
   data.time_diff_to_last_cycle = 0.05f;
   cals.k_pt_f_apply_move_point = false;

   /** \action call book keeping function. */
   Pt_Book_Keep_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data);

   /** \assert Check that a shift has been applied. */
   for (uint8_t point_idx = PT_LOWEST_GRID_POINT_INDEX; point_idx <= PT_MID_GRID_POINT_INDEX; point_idx++)
   {
      EXPECT_FLOAT_EQ(pt_persistent.paths[1u].path_points[point_idx], 5.0f);
   }
}

/**
 * Checks the main recording of pt_persistent.paths routine. Error should be thrown due to wrong object status.
 * \uts{CSCSA-44116} \sdd{SF-7318} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Test, Pt_Record_Paths__error)
{
   /** \arrange Set all objects to invalid state and set all pt_persistent.paths to their defaults. */
   for (uint8_t idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      object_data[idx].status     = (Pa_Obj_Status_T) 10u;
      object_data[idx].id         = 0u;
      object_data[idx].f_moveable = FBK_TRUE;
   }

   for (uint8_t idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      pt_persistent.paths[idx].path_state                      = PATH_STATUS_DEFAULT;
      pt_persistent.paths[idx].obj_curr_used_for_path_build.id = 0u;
   }

   /** \action call main function for recording. */

   /** \assert Expect no pt_persistent.paths to be recorded. */
   EXPECT_DEBUG_DEATH({ Pt_Record_Paths(&pt_persistent, &cals, &pt_input, p_vehicle_data); }, "");
}