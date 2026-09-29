/** \file
 * This file contains unit tests for content of f360_identify_objects_that_are_immediatly_behind_another_object.cpp file
 */

#include "f360_identify_objects_that_are_immediatly_behind_another_object.h"
#include "f360_calibrations.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_identify_objects_that_are_immediatly_behind_another_object
 *  @{
 */

/** \brief
 * This test groups checks the functionality of f360_identify_objects_that_are_immediatly_behind_another_object()
 * f360_identify_objects_that_are_immediatly_behind_another_object() parses over all objects, in order to find object pairs, 
 * which are below 80m from host and are similar in speed (less than 1m/s diff),
 * heading (less than 10 deg diff), and are either ongoing (hdg ~= 0 deg)
 * r oncoming (i.e hdg ~= 180 deg).
 *  
 * For found pairs, the occlusion check is done by extending the bbox of rear object 
 * laterally on both sides and longitudinally towards the front, then it checks
 * if any of the trailing closest edge reference point is inside the extended bbox.
 * If so, then the object behind is considered to be occluded by its pair object and
 * f_object_is_behind_another_object_array[obj2_idx] is set.
 * 
 * f_object_is_behind_another_object_array[obj2_idx] is used in Update_Object_Reference_Point(),
 * to only allow corner reference points as valid ref point choices for that object.
 * 
 */
TEST_GROUP(f360_identify_objects_that_are_immediatly_behind_another_object)
{
   //Initialize common variables used within all tests in this test group.
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calib;
   bool f_object_is_behind_another_object_array[NUMBER_OF_OBJECT_TRACKS];
   bool expected_f_object_is_behind_another_object_array[NUMBER_OF_OBJECT_TRACKS];
   
   // Initialze object indexes variables
   uint32_t obj_idx_1;
   uint32_t obj_idx_2; 
   uint32_t obj_idx_3;
   uint32_t obj_idx_4;
   uint32_t obj_idx_5;
   uint32_t obj_idx_6;

   /** \setup
    * Initialize calibrations, common tracker_info and objects properties.
    * The setup includes 4 objects, that are moving
    * The objects are deliberatly setup longitudinaly in the following order:
    * -> Longitudinal position: Obj1 < Obj2 < Obj3 < Obj4 < Obj5 < Obj6
    * Obj1 and Obj3 are setup as onoging (hdg = 0 degrees) and positive longitudinal velocities
    * Obj1 is setup to occlude Ojbj3
    * Obj1 and Obj3 have similar speed (diff should be less than 1m/s)
    * Obj1 and Obj3 have similar heading (diff should be less than 10 degrees)
    * Obj2 and Obj4 are setup as oncoming (hdg = 180 degrees) and negative longitudinal velocities
    * Obj2 is setup to occlude Ojbj4
    * Obj2 and Obj4 have similar speed (diff should be less than 1m/s)
    * Obj2 and Obj4 have similar heading (diff should be less than 10 degrees)
    * Obj5 is set to be a stationary object, with low speed and f_moving = false
    * Obj6 is set to be longitudinally further from OBj1 by distance greater than 40m
    * Tracker info sorted list is set to correspiond to the above setup
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      tracker_info.num_active_objs = 0;

      std::fill(cmn::begin(f_object_is_behind_another_object_array), cmn::end(f_object_is_behind_another_object_array), false);
      std::fill(cmn::begin(expected_f_object_is_behind_another_object_array), cmn::end(expected_f_object_is_behind_another_object_array), false);

      // Object 1
      obj_idx_1 = 0U;
      object_tracks[obj_idx_1].id = obj_idx_1 + 1U;
      object_tracks[obj_idx_1].vcs_position.x = 38.0F;
      object_tracks[obj_idx_1].vcs_position.y = -2.5F;
      object_tracks[obj_idx_1].reference_point = F360_REFERENCE_POINT_REAR;
      object_tracks[obj_idx_1].Update_Bbox_Size(7.0F, 1.85F);
      object_tracks[obj_idx_1].vcs_heading.Value(0.0F);
      object_tracks[obj_idx_1].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[obj_idx_1].Update_Bbox_Center();
      object_tracks[obj_idx_1].speed = 17.0F;
      object_tracks[obj_idx_1].vcs_velocity.longitudinal = 17.0F;
      object_tracks[obj_idx_1].f_moving = true;
      tracker_info.num_active_objs++;

      // Object 3
      obj_idx_3 = 2U;
      object_tracks[obj_idx_3].id = obj_idx_3 + 1U;
      object_tracks[obj_idx_3].vcs_position.x = 49.0F;
      object_tracks[obj_idx_3].vcs_position.y = -2.5F;
      object_tracks[obj_idx_3].reference_point = F360_REFERENCE_POINT_REAR;
      object_tracks[obj_idx_3].Update_Bbox_Size(5.0F, 1.85F);
      object_tracks[obj_idx_3].vcs_heading.Value(0.0F);
      object_tracks[obj_idx_3].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[obj_idx_3].Update_Bbox_Center();
      object_tracks[obj_idx_3].speed = 17.5F;
      object_tracks[obj_idx_3].vcs_velocity.longitudinal = 17.5F;
      object_tracks[obj_idx_3].f_moving = true;
      tracker_info.num_active_objs++;

      // Object 2
      obj_idx_2 = 1U;
      object_tracks[obj_idx_2].id = obj_idx_2 + 1U;
      object_tracks[obj_idx_2].vcs_position.x = 46.0F;
      object_tracks[obj_idx_2].vcs_position.y = 10.0F;
      object_tracks[obj_idx_2].reference_point = F360_REFERENCE_POINT_FRONT;
      object_tracks[obj_idx_2].Update_Bbox_Size(5.0F, 1.85F);
      object_tracks[obj_idx_2].vcs_heading.Value(F360_PI);
      object_tracks[obj_idx_2].Set_Bbox_Orientation(Angle{ F360_PI });
      object_tracks[obj_idx_2].Update_Bbox_Center();
      object_tracks[obj_idx_2].speed = 12.0F;
      object_tracks[obj_idx_2].vcs_velocity.longitudinal = -12.0F;
      object_tracks[obj_idx_2].f_moving = true;
      tracker_info.num_active_objs++;

      // Object 4
      obj_idx_4 = 3U;
      object_tracks[obj_idx_4].id = obj_idx_4 + 1U;
      object_tracks[obj_idx_4].vcs_position.x = 52.0F;
      object_tracks[obj_idx_4].vcs_position.y = 11.0F;
      object_tracks[obj_idx_4].reference_point = F360_REFERENCE_POINT_FRONT;
      object_tracks[obj_idx_4].Update_Bbox_Size(5.0F, 1.8F);
      object_tracks[obj_idx_4].vcs_heading.Value(F360_PI);
      object_tracks[obj_idx_4].Set_Bbox_Orientation(Angle{ F360_PI });
      object_tracks[obj_idx_4].Update_Bbox_Center();
      object_tracks[obj_idx_4].speed = 11.5F;
      object_tracks[obj_idx_4].vcs_velocity.longitudinal = -11.5F;
      object_tracks[obj_idx_4].f_moving = true;
      tracker_info.num_active_objs++;

      // Object 5
      obj_idx_5 = 4U;
      object_tracks[obj_idx_5].id = obj_idx_4 + 1U;
      object_tracks[obj_idx_5].vcs_position.x = 54.0F;
      object_tracks[obj_idx_5].vcs_position.y = 15.0F;
      object_tracks[obj_idx_5].reference_point = F360_REFERENCE_POINT_CENTER;
      object_tracks[obj_idx_5].Update_Bbox_Size(0.5F, 0.5F);
      object_tracks[obj_idx_5].vcs_heading.Value(F360_PI);
      object_tracks[obj_idx_5].Set_Bbox_Orientation(Angle{ F360_PI });
      object_tracks[obj_idx_5].Update_Bbox_Center();
      object_tracks[obj_idx_5].speed = 0.1F;
      object_tracks[obj_idx_5].vcs_velocity.longitudinal = -0.1F;
      object_tracks[obj_idx_5].f_moving = false;
      tracker_info.num_active_objs++;

      // Object 6
      obj_idx_6 = 5U;
      object_tracks[obj_idx_6].id = obj_idx_4 + 1U;
      object_tracks[obj_idx_6].vcs_position.x = 79.0F;
      object_tracks[obj_idx_6].vcs_position.y = 11.0F;
      object_tracks[obj_idx_6].reference_point = F360_REFERENCE_POINT_REAR;
      object_tracks[obj_idx_6].Update_Bbox_Size(5.0F, 1.8F);
      object_tracks[obj_idx_6].vcs_heading.Value(0.0F);
      object_tracks[obj_idx_6].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[obj_idx_6].Update_Bbox_Center();
      object_tracks[obj_idx_6].speed = 11.5F;
      object_tracks[obj_idx_6].vcs_velocity.longitudinal = 11.5F;
      object_tracks[obj_idx_6].f_moving = true;
      tracker_info.num_active_objs++;

      tracker_info.vcslong_sorted_start = &(object_tracks[obj_idx_1]);
      tracker_info.vcslong_sorted_prev_track[obj_idx_1] = NULL;
      tracker_info.vcslong_sorted_next_track[obj_idx_1] = &(object_tracks[obj_idx_2]);

      tracker_info.vcslong_sorted_prev_track[obj_idx_2] = &(object_tracks[obj_idx_1]);
      tracker_info.vcslong_sorted_next_track[obj_idx_2] = &(object_tracks[obj_idx_3]);

      tracker_info.vcslong_sorted_prev_track[obj_idx_3] = &(object_tracks[obj_idx_2]);
      tracker_info.vcslong_sorted_next_track[obj_idx_3] = &(object_tracks[obj_idx_4]);

      tracker_info.vcslong_sorted_prev_track[obj_idx_4] = &(object_tracks[obj_idx_3]);
      tracker_info.vcslong_sorted_next_track[obj_idx_4] = &(object_tracks[obj_idx_5]);

      tracker_info.vcslong_sorted_prev_track[obj_idx_5] = &(object_tracks[obj_idx_4]);
      tracker_info.vcslong_sorted_next_track[obj_idx_5] = &(object_tracks[obj_idx_5]);

      tracker_info.vcslong_sorted_prev_track[obj_idx_6] = &(object_tracks[obj_idx_4]);
      tracker_info.vcslong_sorted_next_track[obj_idx_6] = NULL;

      expected_f_object_is_behind_another_object_array[obj_idx_3] = true;
      expected_f_object_is_behind_another_object_array[obj_idx_4] = true;
   }
};


/** \purpose  
 * This test checks if an object points to a NULL pointer, then this logic i.e identify_objects_that_are_immediatly_behind_another_object is not applicable
 */
TEST(f360_identify_objects_that_are_immediatly_behind_another_object, Check_if_object_is_not_occluded_due_to_NULL_pointer)
{
   /** \precond
    * First object in sorted list has vcslong_sorted_next_track = NULL
    * f_object_is_behind_another_object_array is expected to be false for all elements 
    */

   tracker_info.vcslong_sorted_next_track[obj_idx_1] = NULL;

   expected_f_object_is_behind_another_object_array[obj_idx_3] = false;
   expected_f_object_is_behind_another_object_array[obj_idx_4] = false;
   
   /** \action
    * call Identify_Objects_That_Are_Immediatly_Behind_Another_Object().
    */
   Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      calib.k_far_away_object_dist_sq_thr,
      tracker_info,
      object_tracks,
      f_object_is_behind_another_object_array);

   /** \result
    * check that the output match expected data.
    */
   for (int32_t i = 0; i < (tracker_info.num_active_objs); i++)
   {
      CHECK_EQUAL_TEXT(expected_f_object_is_behind_another_object_array[i], f_object_is_behind_another_object_array[i], "f_object_is_behind_another_object_array is not set as expected");
   }
}

/** \purpose  
 * This test checks if an object long pos is negative, then this logic i.e identify_objects_that_are_immediatly_behind_another_object is not applicable
 */
TEST(f360_identify_objects_that_are_immediatly_behind_another_object, Check_if_object_is_not_occluded_if_occluding_object_x_pos_is_negative)
{
   /** \precond
    * This test generally uses the same default setup from the test group
    * Obj1 is set with negative x position 
    * Obj3 is set to be behind Obj1
    * Obj3 is not expected to not be considered as behind another object
    */

   // Object 1
   object_tracks[obj_idx_1].vcs_position.x = -0.1F;
   object_tracks[obj_idx_1].vcs_position.y = -2.5F;
   object_tracks[obj_idx_1].reference_point = F360_REFERENCE_POINT_FRONT;
   object_tracks[obj_idx_1].Update_Bbox_Size(7.0F, 1.85F);
   object_tracks[obj_idx_1].vcs_heading.Value(0.0F);
   object_tracks[obj_idx_1].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[obj_idx_1].Update_Bbox_Center();
   // Object 3
   object_tracks[obj_idx_3].vcs_position.x =  8.0F;
   object_tracks[obj_idx_3].vcs_position.y = -2.5F;
   object_tracks[obj_idx_3].reference_point = F360_REFERENCE_POINT_REAR;
   object_tracks[obj_idx_3].Update_Bbox_Size(5.0F, 1.85F);
   object_tracks[obj_idx_3].vcs_heading.Value(0.0F);
   object_tracks[obj_idx_3].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[obj_idx_3].Update_Bbox_Center();

   expected_f_object_is_behind_another_object_array[obj_idx_3] = false;
   expected_f_object_is_behind_another_object_array[obj_idx_4] = true;
   
   /** \action
    * call Identify_Objects_That_Are_Immediatly_Behind_Another_Object().
    */
   Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      calib.k_far_away_object_dist_sq_thr,
      tracker_info,
      object_tracks,
      f_object_is_behind_another_object_array);

   /** \result
    * check that the output match expected data.
    */
   for (int32_t i = 0; i < (tracker_info.num_active_objs); i++)
   {
      CHECK_EQUAL_TEXT(expected_f_object_is_behind_another_object_array[i], f_object_is_behind_another_object_array[i], "f_object_is_behind_another_object_array is not set as expected");
   }
}

/** \purpose  
 * This test checks if an object is beyond threshold distance from host, then this logic i.e identify_objects_that_are_immediatly_behind_another_object is not applicable
 */
TEST(f360_identify_objects_that_are_immediatly_behind_another_object, Check_if_object_is_not_occluded_if_distance_from_host_is_beyond_threshold)
{
   /** \precond
    * This test generally uses the same default setup from the test group
    * Obj3 is set to be further away (greater than 80m) from host, but the rest of the properties are ok for it to be considered occuluded
    * Obj3 is not expected to not be considered as behind another object
    */

   // Object 3
   object_tracks[obj_idx_3].vcs_position.x = 81.0F;
   object_tracks[obj_idx_3].vcs_position.y = -2.5F;
   object_tracks[obj_idx_3].reference_point = F360_REFERENCE_POINT_REAR;
   object_tracks[obj_idx_3].Update_Bbox_Size(5.0F, 1.85F);
   object_tracks[obj_idx_3].vcs_heading.Value(0.0F);
   object_tracks[obj_idx_3].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[obj_idx_3].Update_Bbox_Center();

   expected_f_object_is_behind_another_object_array[obj_idx_3] = false;
   expected_f_object_is_behind_another_object_array[obj_idx_4] = true;
   
   /** \action
    * call Identify_Objects_That_Are_Immediatly_Behind_Another_Object().
    */
   Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      calib.k_far_away_object_dist_sq_thr,
      tracker_info,
      object_tracks,
      f_object_is_behind_another_object_array);

   /** \result
    * check that the output match expected data.
    */
   for (int32_t i = 0; i < (tracker_info.num_active_objs); i++)
   {
      CHECK_EQUAL_TEXT(expected_f_object_is_behind_another_object_array[i], f_object_is_behind_another_object_array[i], "f_object_is_behind_another_object_array is not set as expected");
   }
}

/** \purpose  
 * This test checks if an object, that is behind another object and has similar speed and heading to that object, is not considered to be occluded
 * because its distance from front object is beyond threshold
 */
TEST(f360_identify_objects_that_are_immediatly_behind_another_object, Check_if_object_is_not_occluded_if_distance_between_objects_is_beyond_threshold)
{
   /** \precond
    * This test generally uses the same default setup from the test group
    * Obj3 is set to be further away (greater than 30m) from Obj1, but the rest of the properties are ok for it to be considered occuluded
    * Obj3 is not expected to not be considered as behind another object
    */

   // Object 3
   object_tracks[obj_idx_3].vcs_position.x = 75.0F;
   object_tracks[obj_idx_3].vcs_position.y = -2.5F;
   object_tracks[obj_idx_3].reference_point = F360_REFERENCE_POINT_REAR;
   object_tracks[obj_idx_3].Update_Bbox_Size(5.0F, 1.85F);
   object_tracks[obj_idx_3].vcs_heading.Value(0.0F);
   object_tracks[obj_idx_3].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[obj_idx_3].Update_Bbox_Center();

   expected_f_object_is_behind_another_object_array[obj_idx_3] = false;
   expected_f_object_is_behind_another_object_array[obj_idx_4] = true;
   
   /** \action
    * call Identify_Objects_That_Are_Immediatly_Behind_Another_Object().
    */
   Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      calib.k_far_away_object_dist_sq_thr,
      tracker_info,
      object_tracks,
      f_object_is_behind_another_object_array);

   /** \result
    * check that the output match expected data.
    */
   for (int32_t i = 0; i < (tracker_info.num_active_objs); i++)
   {
      CHECK_EQUAL_TEXT(expected_f_object_is_behind_another_object_array[i], f_object_is_behind_another_object_array[i], "f_object_is_behind_another_object_array is not set as expected");
   }
}

/** \purpose  
 * This test checks if an object, that is immediatly behind another object, not considered to be occluded because its heading is beyond ongoing threshsold
 * This test checks if an object, that is immediatly behind another object, not considered to be occluded because its heading is beyond oncoming threshsold
 */
TEST(f360_identify_objects_that_are_immediatly_behind_another_object, Check_if_objects_are_not_occluded_if_heading_is_beyond_oncoming_or_ongoing_threshold)
{
   /** \precond
    * This test generally uses the same default setup from the test group
    * Obj3 and Obj4 are occluded by Obj1 and Obj2 respectively, but their heading is set to be beyond ongoing or oncoming threshold
    * Obj3 and Obj4 are expected to not be considered as behind another object
    */
   const float32_t k_additive_angle_to_not_pass_ongoing_or_oncoming_threshold = F360_DEG2RAD(4.1F);
   // Object 2
   object_tracks[obj_idx_3].vcs_heading.Value((0.0F + k_additive_angle_to_not_pass_ongoing_or_oncoming_threshold));

   // Object 4
   object_tracks[obj_idx_4].vcs_heading.Value((F360_PI - k_additive_angle_to_not_pass_ongoing_or_oncoming_threshold));

   expected_f_object_is_behind_another_object_array[obj_idx_3] = false;
   expected_f_object_is_behind_another_object_array[obj_idx_4] = false;
   
   /** \action
    * call Identify_Objects_That_Are_Immediatly_Behind_Another_Object().
    */
   Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      calib.k_far_away_object_dist_sq_thr,
      tracker_info,
      object_tracks,
      f_object_is_behind_another_object_array);

   /** \result
    * check that the output match expected data.
    */
   for (int32_t i = 0; i < (tracker_info.num_active_objs); i++)
   {
      CHECK_EQUAL_TEXT(expected_f_object_is_behind_another_object_array[i], f_object_is_behind_another_object_array[i], "f_object_is_behind_another_object_array is not set as expected");
   }
}

/** \purpose
 * This test checks if the object is not considered to be occluded i.e behind another object, if its edge corners are outside the extended bbox of occluding object
 */
TEST(f360_identify_objects_that_are_immediatly_behind_another_object, Check_object_is_not_occluded_if_no_corner_in_occluding_objects_ext_bbox)
{
   /** \precond
    * This test uses the same default setup from the test group
    * Obj3 and Obj4 are laterally offset from Obj1 and Obj2 respectively
    * Obj3 and Obj4 are occluded by Obj1 and Obj2 respectively
    */

      // Object 3
   obj_idx_3 = 2U;
   object_tracks[obj_idx_3].id = obj_idx_3 + 1U;
   object_tracks[obj_idx_3].vcs_position.x = 49.0F;
   object_tracks[obj_idx_3].vcs_position.y = -6.0F;
   object_tracks[obj_idx_3].reference_point = F360_REFERENCE_POINT_REAR;
   object_tracks[obj_idx_3].Update_Bbox_Size(5.0F, 1.85F);
   object_tracks[obj_idx_3].vcs_heading.Value(0.0F);
   object_tracks[obj_idx_3].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[obj_idx_3].Update_Bbox_Center();

	expected_f_object_is_behind_another_object_array[obj_idx_3] = false;
   expected_f_object_is_behind_another_object_array[obj_idx_4] = true;

   /** \action
    * call Identify_Objects_That_Are_Immediatly_Behind_Another_Object().
    */
   Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      calib.k_far_away_object_dist_sq_thr,
      tracker_info,
      object_tracks,
      f_object_is_behind_another_object_array);

   /** \result
    * check that the output match expected data.
    */
   for (int32_t i = 0; i < (tracker_info.num_active_objs); i++)
   {
      CHECK_EQUAL_TEXT(expected_f_object_is_behind_another_object_array[i], f_object_is_behind_another_object_array[i], "f_object_is_behind_another_object_array is not set as expected");
   }
}

/** \purpose
 * This test checks if the object is considered to be occluded i.e behind another object, if there is a slight lateral offset in objects
 */
TEST(f360_identify_objects_that_are_immediatly_behind_another_object, Check_oncoming_and_ongoing_occlusion_with_lateral_offset)
{
   /** \precond
    * This test uses the same default setup from the test group
    * Obj3 and Obj4 are laterally offset from Obj1 and Obj2 respectively
    * Obj3 and Obj4 are occluded by Obj1 and Obj2 respectively
    */

      // Object 3
   obj_idx_3 = 2U;
   object_tracks[obj_idx_3].id = obj_idx_3 + 1U;
   object_tracks[obj_idx_3].vcs_position.x = 49.0F;
   object_tracks[obj_idx_3].vcs_position.y = -4.0F;
   object_tracks[obj_idx_3].reference_point = F360_REFERENCE_POINT_REAR;
   object_tracks[obj_idx_3].Update_Bbox_Size(5.0F, 1.85F);
   object_tracks[obj_idx_3].vcs_heading.Value(0.0F);
   object_tracks[obj_idx_3].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[obj_idx_3].Update_Bbox_Center();

   // Object 4
   obj_idx_4 = 3U;
   object_tracks[obj_idx_4].id = obj_idx_4 + 1U;
   object_tracks[obj_idx_4].vcs_position.x = 52.0F;
   object_tracks[obj_idx_4].vcs_position.y = 12.0F;
   object_tracks[obj_idx_4].reference_point = F360_REFERENCE_POINT_FRONT;
   object_tracks[obj_idx_4].Update_Bbox_Size(5.0F, 1.8F);
   object_tracks[obj_idx_4].vcs_heading.Value(F360_PI);
   object_tracks[obj_idx_4].Set_Bbox_Orientation(Angle{ F360_PI });
   object_tracks[obj_idx_4].Update_Bbox_Center();

   expected_f_object_is_behind_another_object_array[obj_idx_3] = true;
   expected_f_object_is_behind_another_object_array[obj_idx_4] = true;
   
   /** \action
    * call Identify_Objects_That_Are_Immediatly_Behind_Another_Object().
    */
   Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      calib.k_far_away_object_dist_sq_thr,
      tracker_info,
      object_tracks,
      f_object_is_behind_another_object_array);

   /** \result
    * check that the output match expected data.
    */
   for (int32_t i = 0; i < (tracker_info.num_active_objs); i++)
   {
      CHECK_EQUAL_TEXT(expected_f_object_is_behind_another_object_array[i], f_object_is_behind_another_object_array[i], "f_object_is_behind_another_object_array is not set as expected");
   }
}

/** \purpose
 * This test checks the following:
 * Given two ongoing objects, where one of the object occludes the other, then this test checks if the occluded object is identified and f_object_is_behind_another_object_array is set accordingly
 * Given two oncoming objects, where one of the object occludes the other, then this test checks if the occluded object is identified and f_object_is_behind_another_object_array is set accordingly
 */
TEST(f360_identify_objects_that_are_immediatly_behind_another_object, Check_oncoming_and_ongoing_occlusion)
{
   /** \precond
    * This test uses the same default setup from the test group
    * Obj3 and Obj4 are occluded by Obj1 and Obj2 respectively
    */

   expected_f_object_is_behind_another_object_array[obj_idx_3] = true;
   expected_f_object_is_behind_another_object_array[obj_idx_4] = true;
   
   /** \action
    * call Identify_Objects_That_Are_Immediatly_Behind_Another_Object().
    */
   Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      calib.k_far_away_object_dist_sq_thr,
      tracker_info,
      object_tracks,
      f_object_is_behind_another_object_array);

   /** \result
    * check that the output match expected data.
    */
   for (int32_t i = 0; i < (tracker_info.num_active_objs); i++)
   {
      CHECK_EQUAL_TEXT(expected_f_object_is_behind_another_object_array[i], f_object_is_behind_another_object_array[i], "f_object_is_behind_another_object_array is not set as expected");
   }
}
/** @}*/
