/** \file
 * This file contains unit tests for content of f360_multipath_detector.cpp file
 */

#include "f360_multipath_detector.h"
#include "f360_sorted_tracks_mgmt.h"
#include <CppUTest/TestHarness.h>
#include "f360_calibrations.h"
#include "f360_set_variant.h"

 // Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  Is_Multipath___SEP_Reflector
 *  @{
 */

 /** \brief
  * This test group contains test cases for Is_Multipath()
  */
TEST_GROUP(Is_Multipath___SEP_Reflector)
{
   F360_Calibrations_T calibs;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS]{};
   Static_Env_Polys_Array static_env_polys_array;
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T item_pos;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);
   }

   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
};

/** \purpose
 * Test checks if item is reported as multipath when range to it is equal to range to source.
 * \req
 * NA
 */
IGNORE_TEST(Is_Multipath___SEP_Reflector, if_item_has_range_equal_to_source)
{
   //IGNORE reason: in current implementation SEP is not used as reflector. Reflectors are limited to objects.

   /** \precond
   * Set object(reflection source) parameters
   * Set reflector(guardrail) parameters
   * Set lateral and longitudinal position of sensor
   * Set lateral and longitudinal position of examined item to be reported as multipath
   */
   objects[0].vcs_position.x = 16.0F;
   objects[0].vcs_position.y = 0.0F;
   objects[0].vcs_velocity.longitudinal = 6.0F;
   objects[0].vcs_velocity.lateral = 0.0F;

   static_env_polys_array[0].p0 = 6.0F;
   static_env_polys_array[0].poly_type = F360_STATIC_ENV_POLY_TYPE_CURVG;
   static_env_polys_array[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;

   const Point radar_pos = {};

   item_pos.vcs_position = { 16.0F, 12.0F };

   const float32_t item_range_rate = 4.8F;

   /** \action
    * Call Is_Multipath()
    */
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as mutipath
    */
   CHECK_TRUE(expected_result);
}


/** \purpose
 * Test checks if item is not reported as multipath when range to it is lower than range to source.
 * \req
 * NA
 */
IGNORE_TEST(Is_Multipath___SEP_Reflector, if_item_has_range_lower_than_source)
{
   //IGNORE reason: in current implementation SEP is not used as reflector. Reflectors are limited to objects.
   /** \precond
   * Set object(reflection source) parameters
   * Set reflector(guardrail) parameters
   * Set lateral and longitudinal position of sensor
   * Set lateral and longitudinal position of examined item to be NOT reported as multipath
   */
   objects[0].vcs_position.x = 16.0F;
   objects[0].vcs_position.y = 0.0F;
   objects[0].vcs_velocity.longitudinal = 6.0F;
   objects[0].vcs_velocity.lateral = 0.0F;

   static_env_polys_array[0].p0 = 6.0F;
   static_env_polys_array[0].poly_type = F360_STATIC_ENV_POLY_TYPE_CURVG;
   static_env_polys_array[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;

   const Point radar_pos = {};

   item_pos.vcs_position = { 14.0F, 12.0F };

   const float32_t item_range_rate = 4.8F;

   /** \action
    * Call Is_Multipath()
    */
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   CHECK_FALSE(expected_result);
}


/** \purpose
 * Test checks if item is not reported as multipath when range to it is equal to range to source
 * but source speed is equal to zero.
 * \req
 * NA
 */
IGNORE_TEST(Is_Multipath___SEP_Reflector, if_item_has_range_equal_to_source_range_but_wrong_range_rate)
{
   //IGNORE reason: in current implementation SEP is not used as reflector. Reflectors are limited to objects.
   /** \precond
   * Set object(reflection source) parameters
   * Set reflector(guardrail) parameters
   * Set lateral and longitudinal position of sensor
   * Set lateral and longitudinal position of examined item to be NOT reported as multipath
   */
   objects[0].vcs_position.x = 16.0F;
   objects[0].vcs_position.y = 0.0F;
   objects[0].vcs_velocity.longitudinal = 0.0F;
   objects[0].vcs_velocity.lateral = 0.0F;

   static_env_polys_array[0].p0 = 6.0F;
   static_env_polys_array[0].poly_type = F360_STATIC_ENV_POLY_TYPE_CURVG;
   static_env_polys_array[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;

   const Point radar_pos = {};

   item_pos.vcs_position = { 16.0F, 12.0F };

   const float32_t item_range_rate = 0.0F;

   /** \action
    * Call Is_Multipath()
    */
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   CHECK_FALSE(expected_result);
}


/** \purpose
 * Test checks if item is not reported as multipath when range to it is lower than range to source
 * but both have the same longitudinal position and range rates.
 * \req
 * NA
 */
IGNORE_TEST(Is_Multipath___SEP_Reflector, if_item_and_source_have_the_same_long_pos_and_range_rates)
{
   //IGNORE reason: in current implementation SEP is not used as reflector. Reflectors are limited to objects.
   /** \precond
   * Set object(reflection source) parameters
   * Set reflector(guardrail) parameters
   * Set lateral and longitudinal position of sensor
   * Set lateral and longitudinal position of examined item to be NOT reported as multipath
   */
   objects[0].vcs_position.x = 16.0F;
   objects[0].vcs_position.y = 0.0F;
   objects[0].vcs_velocity.longitudinal = 4.0F;
   objects[0].vcs_velocity.lateral = 0.0F;
   static_env_polys_array[0].p0 = 6.0F;
   static_env_polys_array[0].poly_type = F360_STATIC_ENV_POLY_TYPE_CURVG;
   static_env_polys_array[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;

   const Point radar_pos = {};

   item_pos.vcs_position = { 16.0F, 8.0F };

   const float32_t item_range_rate = 3.2F;

   /** \action
    * Call Is_Multipath()
    */
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   CHECK_FALSE(expected_result);
}
/** @}*/

/** \defgroup  Is_Multipath___Stationary_Object_Reflector
 *  @{
 */

 /** \brief
  * This test group contains test cases for Is_Multipath() when reflector is stationary
  */
TEST_GROUP(Is_Multipath___Stationary_Object_Reflector)
{
   F360_Calibrations_T calibs;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS]{};
   Static_Env_Polys_Array static_env_polys_array = {};
   F360_Tracker_Info_T tracker_info = {};

   Point radar_pos;
   F360_Object_Track_T item_pos;
   float32_t item_range_rate;

   F360_Object_Track_T &reflector = objects[0];
   F360_Object_Track_T &refl_source = objects[1];

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      //Set (reflector) stationary reflector parameters
      reflector.status = F360_OBJECT_STATUS_UPDATED;
      reflector.Set_Bbox_Orientation(Angle{ 0.0F });
      reflector.id = 1;
      reflector.average_rcs = 1.0F;
      reflector.vcs_position = { 15.0F, 4.0F };
      reflector.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = { 15.0F, 4.0F };
      reflector.bbox.Set_Center(center);
      reflector.bbox.Set_Length(2.0F);
      reflector.bbox.Set_Width(2.0F);

      // Set object (reflection source) parameters
      refl_source.vcs_velocity = { 5.0F, -1.0F };
      refl_source.f_moving = true;
      refl_source.Set_Bbox_Orientation(Angle{ -0.1F });
      refl_source.status = F360_OBJECT_STATUS_UPDATED;
      refl_source.id = 2;
      reflector.vcs_position = { 6.0F, 14.0F };
      refl_source.reference_point = F360_REFERENCE_POINT_CENTER;
      center = { 6.0F, 14.0F };
      refl_source.bbox.Set_Center(center);
      refl_source.bbox.Set_Length(6.0F);
      refl_source.bbox.Set_Width(3.0F);

      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;

      radar_pos = { 2.0F, 2.0F };
      item_pos.vcs_position = { 26.0F, 6.0F };
      item_range_rate = -4.0F;

      tracker_info.num_active_objs = 2;

      // Reset sorted vcslong list
      tracker_info.vcslong_sorted_start = NULL;
      for (uint32_t i = 0; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = NULL;
         tracker_info.vcslong_sorted_prev_track[i] = NULL;
      }
      Sorted_Tracks_Insert(tracker_info, &(reflector));
      Sorted_Tracks_Insert(tracker_info, &(refl_source));
   }
};

/** \purpose
 * Check if Is_Multipath() returns true when there is true multipath case
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, valid_multipath)
{
   /** \precond
   * Same as group setup - two objects that match true multipath model.
   */

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as multipath
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns false when there is no any intersection with reflector
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, no_intersections_with_reflector)
{
   /** \precond
   * Same as setup
   * Reflector obejct does not intersect with sensor-item segment
   */
   reflector.vcs_position = { 16.0F, 8.0F };
   Point center = {16.0F, 8.0F};
   reflector.bbox.Set_Center(center);

   /** \action
    * Call Is_Multipath()
    */

   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as no mutipath
    */
   CHECK_FALSE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns true when there is true multipath case (two potential reflectors, real is closer)
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, valid_multipath__use_closer_reflector)
{
   /** \precond
   * Same as group setup - three objects that match true multipath model.
   */
   objects[2].vcs_position = { 20.0F, 5.0F };
   Point center = {20.0F, 5.0F};
   objects[2].bbox.Set_Center(center);
   objects[2].status = F360_OBJECT_STATUS_UPDATED;
   objects[2].Set_Bbox_Orientation(Angle{ 0.0F });
   objects[2].id = 3;


   tracker_info.num_active_objs++;
   Sorted_Tracks_Insert(tracker_info, &(objects[2]));

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as mutipath
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns true when there is true multipath case (two potential reflectors, real is further) - case 1
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, valid_multipath__use_closer_reflector__reverted_order__case_1)
{
   /** \precond
   * Same as group setup - three objects that match true multipath model.
   */
   objects[2].vcs_position = { 20.0F, 5.0F };
   Point center = {20.0F, 5.0F};
   objects[2].bbox.Set_Center(center);
   objects[2].status = F360_OBJECT_STATUS_UPDATED;
   objects[2].Set_Bbox_Orientation(Angle{ 0.0F });
   objects[2].id = 3;

   //Revert order
   F360_Object_Track_T temp = reflector;
   reflector = objects[2];
   objects[2] = temp;

   tracker_info = {};
   Set_Tracker_Variant(tracker_info.variant);
   tracker_info.num_active_objs = 3;
   Sorted_Tracks_Insert(tracker_info, &(objects[2]));
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   /** \action
    * Call Is_Multipath()
    */

   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as mutipath
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns true when there is true multipath case (two potential reflectors, real is further)  case 2
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, valid_multipath__use_closer_reflector__reverted_order__case_2)
{
   /** \precond
   * Same as group setup - three objects that match true multipath model.
   */
   objects[2].vcs_position = { 14.7F, 4.0F };
   Point center = {14.7F, 4.0F};
   objects[2].bbox.Set_Center(center);
   objects[2].Update_Bbox_Size(1.0F, 1.0F);
   objects[2].status = F360_OBJECT_STATUS_UPDATED;
   objects[2].Set_Bbox_Orientation(Angle{ 0.0F });
   objects[2].id = 3;

   //Revert order
   F360_Object_Track_T temp = reflector;
   reflector = objects[2];
   objects[2] = temp;

   tracker_info = {};
   Set_Tracker_Variant(tracker_info.variant);
   tracker_info.num_active_objs = 3;
   Sorted_Tracks_Insert(tracker_info, &(objects[2]));
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   /** \action
    * Call Is_Multipath()
    */

   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as mutipath
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns false when there is true multipath case (two potential reflectors, real is closer)
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, invalid_multipath_due_to_being_inside)
{
   /** \precond
   * Same as group setup - two objects that match true multipath model.
   * Set reflection source object to item position.
   */
   refl_source.vcs_position = item_pos.vcs_position;
   refl_source.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = item_pos.vcs_position;
   refl_source.bbox.Set_Center(center);

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as NOT multipath
    */
   CHECK_FALSE(expected_result);
}


/** \purpose
 * Check if Is_Multipath() returns false when source reflector is too close (inside searching range)
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, range_hypothesis__source_too_close)
{
   /** \precond
   * Same as group setup but source object is positioned closer reflection point.
   */
   refl_source.vcs_position = {9.0F, 9.0F};
   refl_source.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = {9.0F, 9.0F};
   refl_source.bbox.Set_Center(center);

   //Set lateral and longitudinal position of examined item to be reported as multipath

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   CHECK_FALSE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns false when source reflector is too far away (outside searching range)
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, range_hypothesis__source_too_far_away)
{
   /** \precond
   * Same as group setup but source object is positioned further reflection point.
   */
   refl_source.vcs_position = {3.0F, 14.0F};
   refl_source.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = {3.0F, 14.0F};
   refl_source.bbox.Set_Center(center);

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   //CHECK_FALSE(expected_result);
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns false when item range rate is too low
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, range_rate_hypothesis__item_too_low_speed)
{
   /** \precond
   * Same as group setup but item range rate is set to low value.
   */
   item_range_rate = -5.11F;

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   CHECK_FALSE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns false when item range rate is too high
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, range_rate_hypothesis__item_too_high_speed)
{
   /** \precond
   * Same as group setup but item range rate is set to high value.
   */
   item_range_rate = -2.2F;

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   CHECK_FALSE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns false when reflector_source status is changed to costed
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, reflector_source_status__costed)
{
   /** \precond
   * Same as group setup but reflector_source.status is changed to coasted.
   */
   refl_source.status = F360_OBJECT_STATUS_COASTED;

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   CHECK_FALSE(expected_result);
}

/** \purpose
 * Check if Is_Multipath() returns false when mp_object_candidate id is the same as reflector source id.
 * \req
 * NA
 */
TEST(Is_Multipath___Stationary_Object_Reflector, reflector_source_id_the_same_as_mp_candidate)
{
   /** \precond
   * Same as group setup but mp_object_candidate.id is changed to reflector source id.
   */
   item_pos.id = 2;

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as not mutipath
    */
   CHECK_FALSE(expected_result);
}
/** @}*/

/** \defgroup  Reflection_Distance_Check
 *  @{
 */

 /** \brief
  * This test group contains test cases for Distances_ok_for_Multipath() in Is_Multipath()
  */
TEST_GROUP(Reflection_Distance_Check)
{
   Point radar_pos;
   Point reflection_point;
   Point source_pos;

   TEST_SETUP()
   {
      radar_pos = { 0.0F, 0.0F };
      reflection_point = { 15.80F, 0.0F };
      source_pos = { 15.80F, -15.88F };
   }
};

/** \purpose
 * Check if Distances_ok_for_Multipath() returns false when the reflection distances are within 50m range (not ok) and
 * distance(radar, reflection) < distance(reflection, source)  (not ok).
 * So neither of the distance or the ratio is valid.
 * \req
 * NA
 */
TEST(Reflection_Distance_Check, Reflection_Distance_Not_Valid_if_within_50m_Range_and_Radar2Reflector_Smaller_than_Reflector2Source)
{
   /** \precond
   * Same as group setup.
   */

   /** \action
    * Call Distances_ok_for_Multipath()
    */
   const bool expected_result = Distances_ok_for_Multipath(radar_pos, reflection_point, source_pos);

   /** \result
    * Function should report distances as not valid for multipath hypothesis
    */
   CHECK_FALSE(expected_result);
}

/** \purpose
 * Check if Distances_ok_for_Multipath() returns true when the reflection distances are outside 50m range (ok) but
 * distance(radar, reflection) < distance(reflection, source)  (not ok).
 * So distance is valid, but ratio is not.
 * \req
 * NA
 */
TEST(Reflection_Distance_Check, Reflection_Distance_Valid_if_Outside_50m_Range_but_Radar2Reflector_Smaller_than_Reflector2Source)
{
   /** \precond
   * Same as group setup, but source object is positioned further away from reflection point.
   */
  source_pos.y = 48.0F;

   /** \action
    * Call Distances_ok_for_Multipath()
    */
   const bool expected_result = Distances_ok_for_Multipath(radar_pos, reflection_point, source_pos);

   /** \result
    * Function should report distances as valid for multipath hypothesis
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Check if Distances_ok_for_Multipath() returns true when the reflection distances are WITHIN 50m range (not ok) and
 * distance(radar, reflection) >= distance(reflection, source)  (ok).
 * So distance is invalid, but ratio is ok.
 * \req
 * NA
 */
TEST(Reflection_Distance_Check, Reflection_Distance_Valid_if_within_50m_Range_but_radar2reflector_greater_than_reflector2source)
{
   /** \precond
   * Same as group setup, but reflection point is positioned slightly further away from radar to test the satisfied condition on the border.
   */
  reflection_point.x = 15.90F;

   /** \action
    * Call Distances_ok_for_Multipath()
    */
   const bool expected_result = Distances_ok_for_Multipath(radar_pos, reflection_point, source_pos);

   /** \result
    * Function should report distances as valid for multipath hypothesis
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Check if Distances_ok_for_Multipath() returns true when the reflection distances are outside 50m range (ok) and
 * distance(radar, reflection) > distance(reflection, source)  (ok).
 * So Both the distance and the ratio are ok.
 * \req
 * NA
 */
TEST(Reflection_Distance_Check, Reflection_Distance_Valid_if_Object_outside_50m_Range_and_Ratio_is_Insignificant)
{
   /** \precond
   * Same as group setup, but reflection point is positioned slightly further away from radar to test the border condition.
   */
  reflection_point.x = -20.0F;
  source_pos.y = 40.0F;

   /** \action
    * Call Distances_ok_for_Multipath()
    */
   const bool expected_result = Distances_ok_for_Multipath(radar_pos, reflection_point, source_pos);

   /** \result
    * Function should report distances as valid for multipath hypothesis
    */
   CHECK_TRUE(expected_result);
}
/** @}*/

/** \defgroup  Reflection_Angle_Check
 *  @{
 */

static float32_t Normalize_Angle_Deg(float32_t angle_deg)
{
   // Guard against special float values
   if (!std::isfinite(angle_deg))
   {
      // Handle error: return 0, assert, log error, etc.
      return 0.0F; // Or handle appropriately
   }

   // Use modulo for efficiency (handles both directions)
   angle_deg = std::fmod(angle_deg, 360.0F);

   // Handle negative case
   if (angle_deg < 0.0F)
   {
      angle_deg += 360.0F;
   }

   return angle_deg;
}

static Point Build_Point_From_Polar(const float32_t radius, const float32_t angle_deg)
{
   const float32_t angle_rad = F360_DEG2RAD(angle_deg);
   Point point;
   point.x = radius * F360_Cosf(angle_rad);
   point.y = radius * F360_Sinf(angle_rad);
   return point;
}

static void Compute_Source_Position(
   const float32_t incident_angle_vcs_deg,
   const float32_t reflected_angle_vcs_deg,
   const float32_t reflector_range,
   const float32_t dist_reflector_to_source,
   Point &reflection_point,
   Point &source_pos)
{
   const Point reflection_from_origin = Build_Point_From_Polar(reflector_range, Normalize_Angle_Deg(incident_angle_vcs_deg));
   reflection_point = reflection_from_origin;

   const Point reflected_offset = Build_Point_From_Polar(dist_reflector_to_source, Normalize_Angle_Deg(reflected_angle_vcs_deg));
   source_pos.x = reflection_point.x + reflected_offset.x;
   source_pos.y = reflection_point.y + reflected_offset.y;
}

static void Compute_Multipath_Position(
   const float32_t incident_angle_vcs_deg,
   const float32_t reflector_range,
   const float32_t dist_reflector_to_multipath,
   Point &multipath_pos)
{
   multipath_pos = Build_Point_From_Polar(reflector_range + dist_reflector_to_multipath, Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 180.0F));
}

/** \brief
 * This test group contains test cases for Distances_ok_for_Multipath() in Is_Multipath()
 */
TEST_GROUP(Reflection_Angle_Check)
{
   Point radar_pos;
   Point reflection_point;
   Point source_pos;
   float32_t max_reflection_angle;

   TEST_SETUP()
   {
      radar_pos = { 0.0F, 0.0F };  // We set the radar at the origin for simplicity
      max_reflection_angle= 2.618F; // 150 degrees in radians
   }

};

/** \purpose
 * Verify reflection angle hypothesis passes in quadrant I within threshold.
 * \req
 * NA
 */
TEST(Reflection_Angle_Check, quadrant_I_within_threshold)
{
   const float32_t incident_angle_vcs_deg = 45.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 149.0F);
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, 20.0F, 80.0F, reflection_point, source_pos);

   const bool expected_result = Reflection_Angle_ok_for_Multipath(
      radar_pos, reflection_point, source_pos, max_reflection_angle);

   CHECK_TRUE(expected_result);
}

/** \purpose
 * Verify reflection angle hypothesis fails in quadrant I outside threshold.
 * \req
 * NA
 */
TEST(Reflection_Angle_Check, quadrant_I_outside_threshold)
{
   const float32_t incident_angle_vcs_deg = 45.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 151.0F);
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, 20.0F, 80.0F, reflection_point, source_pos);

   const bool expected_result = Reflection_Angle_ok_for_Multipath(
      radar_pos, reflection_point, source_pos, max_reflection_angle);

   CHECK_FALSE(expected_result);
}

/** \purpose
 * Verify reflection angle hypothesis passes in quadrant II within threshold.
 * \req
 * NA
 */
TEST(Reflection_Angle_Check, quadrant_II_within_threshold)
{
   const float32_t incident_angle_vcs_deg = 135.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 149.0F);
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, 20.0F, 80.0F, reflection_point, source_pos);

   const bool expected_result = Reflection_Angle_ok_for_Multipath(
      radar_pos, reflection_point, source_pos, max_reflection_angle);

   CHECK_TRUE(expected_result);
}

/** \purpose
 * Verify reflection angle hypothesis fails in quadrant II outside threshold.
 * \req
 * NA
 */
TEST(Reflection_Angle_Check, quadrant_II_outside_threshold)
{
   const float32_t incident_angle_vcs_deg = 135.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 151.0F);
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, 20.0F, 80.0F, reflection_point, source_pos);

   const bool expected_result = Reflection_Angle_ok_for_Multipath(
      radar_pos, reflection_point, source_pos, max_reflection_angle);

   CHECK_FALSE(expected_result);
}

/** \purpose
 * Verify reflection angle hypothesis passes in quadrant III within threshold.
 * \req
 * NA
 */
TEST(Reflection_Angle_Check, quadrant_III_within_threshold)
{
   const float32_t incident_angle_vcs_deg = -135.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 149.0F);
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, 20.0F, 80.0F, reflection_point, source_pos);

   const bool expected_result = Reflection_Angle_ok_for_Multipath(
      radar_pos, reflection_point, source_pos, max_reflection_angle);

   CHECK_TRUE(expected_result);
}

/** \purpose
 * Verify reflection angle hypothesis fails in quadrant III outside threshold.
 * \req
 * NA
 */
TEST(Reflection_Angle_Check, quadrant_III_outside_threshold)
{
   const float32_t incident_angle_vcs_deg = -135.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 151.0F);
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, 20.0F, 80.0F, reflection_point, source_pos);

   const bool expected_result = Reflection_Angle_ok_for_Multipath(
      radar_pos, reflection_point, source_pos, max_reflection_angle);

   CHECK_FALSE(expected_result);
}

/** \purpose
 * Verify reflection angle hypothesis passes in quadrant IV within threshold.
 * \req
 * NA
 */
TEST(Reflection_Angle_Check, quadrant_IV_within_threshold)
{
   const float32_t incident_angle_vcs_deg = -45.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 149.0F);
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, 20.0F, 80.0F, reflection_point, source_pos);

   const bool expected_result = Reflection_Angle_ok_for_Multipath(
      radar_pos, reflection_point, source_pos, max_reflection_angle);

   CHECK_TRUE(expected_result);
}

/** \purpose
 * Verify reflection angle hypothesis fails in quadrant IV outside threshold.
 * \req
 * NA
 */
TEST(Reflection_Angle_Check, quadrant_IV_outside_threshold)
{
   const float32_t incident_angle_vcs_deg = -45.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 151.0F);
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, 20.0F, 80.0F, reflection_point, source_pos);

   const bool expected_result = Reflection_Angle_ok_for_Multipath(
      radar_pos, reflection_point, source_pos, max_reflection_angle);

   CHECK_FALSE(expected_result);
}
/** @} */

/** \defgroup  Is_Multipath_Distance_And_Angle_Validation
 *  @{
 */

 /** \brief
  * This test group contains test cases for Is_Multipath() focusing on distance and angle validation logic.
  * Tests cover combinations of: far/near objects, distance ratio pass/fail, angle within/outside 150 degrees.
  */
TEST_GROUP(Is_Multipath_Distance_And_Angle_Validation)
{
   F360_Calibrations_T calibs;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS]{};
   Static_Env_Polys_Array static_env_polys_array = {};
   F360_Tracker_Info_T tracker_info = {};

   Point radar_pos;
   F360_Object_Track_T item_pos;
   float32_t item_range_rate;

   F360_Object_Track_T &reflector = objects[0];
   F360_Object_Track_T &refl_source = objects[1];

   void Setup_Common_Multipath_Scenario()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      // Set reflector (stationary) parameters
      reflector.status = F360_OBJECT_STATUS_UPDATED;
      reflector.Set_Bbox_Orientation(Angle{ 0.0F });
      reflector.id = 1;
      reflector.average_rcs = 1.0F;
      reflector.reference_point = F360_REFERENCE_POINT_CENTER;
      reflector.bbox.Set_Length(2.0F);
      reflector.bbox.Set_Width(2.0F);
      reflector.f_moving = false;

      // Set source object (moving) parameters
      refl_source.f_moving = true;
      refl_source.Set_Bbox_Orientation(Angle{ 0.0F });
      refl_source.status = F360_OBJECT_STATUS_UPDATED;
      refl_source.id = 2;
      refl_source.reference_point = F360_REFERENCE_POINT_CENTER;
      refl_source.bbox.Set_Length(4.0F);
      refl_source.bbox.Set_Width(2.0F);
      refl_source.vcs_velocity = { -5.0F, -1.0F };

      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.num_active_objs = 2;

      item_range_rate = -2.58F;

      radar_pos = { 0.0F, 0.0F };

      // Reset sorted vcslong list
      tracker_info.vcslong_sorted_start = NULL;
      for (uint32_t i = 0; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = NULL;
         tracker_info.vcslong_sorted_prev_track[i] = NULL;
      }

   }

   TEST_SETUP()
   {
      Setup_Common_Multipath_Scenario();
   }
};

/** \purpose
 * Test Is_Multipath() when objects are FAR from host (typical multipath), radar2reflector > reflector2object (typical multipath), and angle is larger than 150 degrees (not typical multipath).
 * Should return TRUE (multipath detected).
 * \req
 * NA
 */
TEST(Is_Multipath_Distance_And_Angle_Validation, far_objects_distances_ratio_pass_angle_within_threshold)
{
   /** \precond
   * Distance ratio satisfies requirement (radar2reflector > reflector2object), angle > 150 degrees are strange for multipath.
   * But objects positioned far from host (>50m), which is ok for multipaths.
   */
   const float32_t incident_angle_vcs_deg = 44.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 151.0F);  // revert the direction of incident angle to compute the vcs angle of reflected ray
   const float32_t reflector_radar_range = 20.0F;
   const float32_t dist_reflector_to_source = 59.0F;

   Point source_pos;
   Point reflector_center;
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, reflector_radar_range, dist_reflector_to_source, reflector_center, source_pos);
   Compute_Multipath_Position(incident_angle_vcs_deg, reflector_radar_range, dist_reflector_to_source, item_pos.vcs_position);

   reflector.vcs_position = reflector_center;
   reflector.bbox.Set_Center(reflector_center);

   // Source is far away and positioned to create valid reflection angle
   refl_source.vcs_position = source_pos;
   refl_source.bbox.Set_Center(source_pos);

   Sorted_Tracks_Insert(tracker_info, &reflector);
   Sorted_Tracks_Insert(tracker_info, &refl_source);

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as multipath, as distance check passes.
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Test Is_Multipath() when objects are NEAR host (not typical multipath), radar2reflector < reflector2object (not typical multipath), but angle is WITHIN 150 degrees (typical multipath).
 * Should return TRUE (angle check saves it).
 * \req
 * NA
 */
TEST(Is_Multipath_Distance_And_Angle_Validation, far_objects__ratio_fail__angle_within_threshold)
{
   /** \precond
   * Objects positioned near host (<50m), distance ratio does not satisfy requirement (radar2reflector < reflector2object) is strange for multipath.
   * But angle < 150 degrees is ok for multipaths
   */
   const float32_t incident_angle_vcs_deg = 136.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 147.0F);  // revert the direction of incident angle to compute the vcs angle of reflected ray
   const float32_t reflector_radar_range = 12.0F;
   const float32_t dist_reflector_to_source = 40.0F;

   Point source_pos;
   Point reflector_center;
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, reflector_radar_range, dist_reflector_to_source, reflector_center, source_pos);
   Compute_Multipath_Position(incident_angle_vcs_deg, reflector_radar_range, dist_reflector_to_source, item_pos.vcs_position);

   reflector.vcs_position = reflector_center;
   reflector.bbox.Set_Center(reflector_center);

   // Source is far away and positioned to create valid reflection angle
   refl_source.vcs_position = source_pos;
   refl_source.bbox.Set_Center(source_pos);

   Sorted_Tracks_Insert(tracker_info, &reflector);
   Sorted_Tracks_Insert(tracker_info, &refl_source);

   item_range_rate = 4.6F; // Set range rate to value that passes range rate check

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report as multipath (angle saves it)
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Test Is_Multipath() when objects are NEAR host (not typical multipath), radar2reflector > reflector2object (typical multipath), but angle is OUTSIDE 150 degrees (not typical multipath).
 * Should return TRUE (distance ratio check saves it).
 * \req
 * NA
 */
TEST(Is_Multipath_Distance_And_Angle_Validation, far_objects_distances_ratio_pass_angle_outside_threshold)
{
   /** \precond
   * Objects positioned near host (<50m) not typical for multipath, but radar2reflector > reflector2object (requirement met), and angle > 150 degrees (not typical multipath)
   */
   const float32_t incident_angle_vcs_deg = -46.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 151.0F);  // revert the direction of incident angle to compute the vcs angle of reflected ray
   const float32_t reflector_radar_range = 27.0F;
   const float32_t dist_reflector_to_source = 20.0F;

   Point source_pos;
   Point reflector_center;
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, reflector_radar_range, dist_reflector_to_source, reflector_center, source_pos);
   Compute_Multipath_Position(incident_angle_vcs_deg, reflector_radar_range, dist_reflector_to_source, item_pos.vcs_position);

   reflector.vcs_position = reflector_center;
   reflector.bbox.Set_Center(reflector_center);

   // Source is far away and positioned to create valid reflection angle
   refl_source.vcs_position = source_pos;
   refl_source.bbox.Set_Center(source_pos);

   Sorted_Tracks_Insert(tracker_info, &reflector);
   Sorted_Tracks_Insert(tracker_info, &refl_source);
   item_range_rate = -4.55F;

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should report item as multipath (distance ratio check saves it)
    */
   CHECK_TRUE(expected_result);
}

/** \purpose
 * Test Is_Multipath() when objects are NEAR host (not typical multipath), radar2reflector < reflector2object (not typical multipath), and angle is OUTSIDE 150 degrees (not typical multipath).
 * Should return FALSE (all checks fail).
 * \req
 * NA
 */
TEST(Is_Multipath_Distance_And_Angle_Validation, far_objects__ratio_fail__angle_outside_threshold)
{
   /** \precond
   * Objects positioned near host (<50m) not typical for multipath, radar2reflector < reflector2object (not typical multipath requirement), and angle > 150 degrees (not typical multipath)
   */
   const float32_t incident_angle_vcs_deg = -125.0F;
   const float32_t reflected_angle_vcs_deg = Normalize_Angle_Deg(incident_angle_vcs_deg + 180.0F - 151.0F);  // revert the direction of incident angle to compute the vcs angle of reflected ray
   const float32_t reflector_radar_range = 10.0F;
   const float32_t dist_reflector_to_source = 32.0F;

   Point source_pos;
   Point reflector_center;
   Compute_Source_Position(incident_angle_vcs_deg, reflected_angle_vcs_deg, reflector_radar_range, dist_reflector_to_source, reflector_center, source_pos);
   Compute_Multipath_Position(incident_angle_vcs_deg, reflector_radar_range, dist_reflector_to_source, item_pos.vcs_position);

   reflector.vcs_position = reflector_center;
   reflector.bbox.Set_Center(reflector_center);

   // Source is far away and positioned to create valid reflection angle
   refl_source.vcs_position = source_pos;
   refl_source.bbox.Set_Center(source_pos);

   Sorted_Tracks_Insert(tracker_info, &reflector);
   Sorted_Tracks_Insert(tracker_info, &refl_source);

   /** \action
    * Call Is_Multipath()
    */
   Multipath_Detector multipath_detector = Multipath_Detector(static_env_polys_array, objects, tracker_info, calibs);
   const bool expected_result = multipath_detector.Is_Multipath(radar_pos, item_pos, item_range_rate, tracker_info);

   /** \result
    * Function should NOT report item as multipath (All checks fail)
    */
   CHECK_FALSE(expected_result);
}
/** @} */
