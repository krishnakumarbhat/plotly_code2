/** \file
 * This file contains unit tests for content of f360_reflector_selector.cpp file
 */

#include "f360_reflector_selector.h"
#include <CppUTest/TestHarness.h>
#include "f360_sorted_tracks_mgmt.h"

 // Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup Get_Reflection_Point
 *  @{
 */

 /** \brief
  * This test group contains tests for Object_Track_Reflector_Selector class by using its interface function Get_Reflection_Point()
  */
TEST_GROUP(Get_Reflection_Point)
{
   // Common variables used within all tests in this test group.
   F360_Calibrations_T calibs{};
   F360_Tracker_Info_T tracker_info{};
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS]{};
   

   TEST_SETUP()
   {
      const float half_object_length = 1.0F;
      const float half_object_width = 0.5F;
      const int max_number_of_test_objects = 4;

      tracker_info.num_active_objs = 0;

      // Reset sorted vcslong list
      tracker_info.vcslong_sorted_start = NULL;
      for (uint32_t i = 0; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = NULL;
         tracker_info.vcslong_sorted_prev_track[i] = NULL;
      }

      for (int i = 0; i < max_number_of_test_objects; i++)
      {
         objects[i].id = i + 1;
         objects[i].bbox.Set_Length(2*half_object_length);
         objects[i].bbox.Set_Width(2*half_object_width);
         objects[i].Set_Bbox_Orientation(Angle{ 0.0F });
         objects[i].f_moving = false;
         objects[i].reference_point = F360_REFERENCE_POINT_CENTER;
         const Point center = {static_cast<float>(i), 0.0F};
         objects[i].bbox.Set_Center(center);
         objects[i].vcs_position = center;
      }
      objects[0].average_rcs = 1.0F;
      objects[1].average_rcs = 1.0F;
      Initialize_Tracker_Calibrations(calibs);
      calibs.k_mp_object_reflector_size_extension = 0.0F;
   }

};

/** \purpose
 * This test checks whether outside zone is defined properly and objects in front of host inside this zone are selected.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_outside_zone_is_defined_properly_and_objs_inside_are_selected_at_the_front_of_host)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - inside outer zone, very near zone border,
    *                                                   object 2 - outside outer zone, very near zone border
    * Set necessary tracker data.
    */
   Point center = {9.9F,9.9F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {10.1F,10.1F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));


   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 10.0F, 10.0F };

   std::pair<bool, Point> expected_intersection = { true, {8.9F, 8.9F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks whether outside zone is defined properly and objects at the rear of host inside this zone are selected.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_outside_zone_is_defined_properly_and_objs_inside_are_selected_at_the_rear_of_host)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - inside outer zone, at the rear of host, very near zone border,
    *                                                   object 2 - outside outer zone, at the rear of host, very near zone border
    * Set necessary tracker data.
    */
   Point center = {-9.9F,-9.9F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {-10.1F,-10.1F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ -10.0F, -10.0F };

   std::pair<bool, Point> expected_intersection = { true, {-8.9F, -8.9F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks whether outside zone is defined properly and objects laying exactly at its border are selected.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_outside_zone_is_defined_properly_and_objs_laying_on_its_border_are_selected_at_the_front_of_host)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - at the border of outer zone, at the front of host,
    *                                                   object 2 - outside outer zone, at the front of host, very near zone border
    * Set necessary tracker data.
    */

   Point center = {10.0F,10.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {10.1F,10.1F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 10.0F, 10.0F };

   std::pair<bool, Point> expected_intersection = { true, {9.0F, 9.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks whether object closer to sensor is selected as reflector when their rcs are ok.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, object_closer_to_sensor_is_selected_as_reflector)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor with acceptable RCS,
    *                                                   object 2 - further from sensor with acceptable RCS
    * Set necessary tracker data.
    */
   Point center = {2.0F,2.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   objects[0].average_rcs = -11.9F; // Set acceptable RCS value for object 1
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {3.0F,3.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   objects[1].average_rcs = -11.9F; // Set acceptable RCS value for object 2
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));


   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 10.0F, 10.0F };

   std::pair<bool, Point> expected_intersection = { true, {1.0F, 1.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks that if one object near host has too low RCS, it will not be considered as a reflector even if it is closer than the farther object with acceptable RCS.
 * Objects are on right host side so they have positive vcs lateral position
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, in_near_region_too_low_rcs_object_will_not_be_selected_as_reflector_but_farther_object_with_acceptable_rcs_will)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor with too low RCS
    *                                                   object 2 - further from sensor with acceptable RCS
    * Set necessary tracker data.
    */
   Point center = {2.0F, 2.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   objects[0].average_rcs = -12.1F; // Set too low RCS value for object 1
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {3.0F, 3.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   objects[1].average_rcs = -11.9F; // Set higher RCS value for object 2
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 10.0F, 10.0F };

   std::pair<bool, Point> expected_intersection = { true, {2.0F, 2.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 2 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks that if all objects in near region have too low RCS, they will not be considered as reflectors. No intersection point should be returned in this case.
 * Objects are on right host side so they have positive vcs lateral position
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, too_low_rcs_objects_will_not_be_selected_as_reflectors_in_near_region)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor with too low RCS
    *                                                   object 2 - further from sensor with too low RCS
    * Set necessary tracker data.
    */
   Point center = {2.0F, 2.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   objects[0].average_rcs = -12.1F; // Set too low RCS value for object 1
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {3.0F, 3.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   objects[1].average_rcs = -12.1F; // Set too low RCS value for object 2
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 10.0F, 10.0F };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * No intersection point should be returned since both objects have too low RCS
    */
   CHECK_FALSE(result.first);
}

/** \purpose
 * This test checks that if an object in far region has too low RCS, it will not be considered as a reflector even if it is closer than a farther object with acceptable RCS.
 * Objects are on right host side so they have positive vcs lateral position
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, in_far_region_too_low_rcs_object_will_not_be_selected_as_reflector_but_farther_object_with_acceptable_rcs_will)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor with too low RCS
    *                                                   object 2 - further from sensor with acceptable RCS
    * Set necessary tracker data.
    */
   Point center = {0.0F, 11.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   objects[0].average_rcs = 0.0F; // Set too low RCS value for object 1
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {0.0F, 12.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   objects[1].average_rcs = 0.1F; // Set higher RCS value for object 2
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(13.0F, 13.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 0.0F, 20.0F };

   std::pair<bool, Point> expected_intersection = { true, {0.0F, 11.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 2 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks that when RCS values are ok for both objects in far region, the closest object will be selected as reflectors.
 * Objects are on left host side so they have negative vcs lateral position
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, closest_object_will_be_selected_as_reflector_in_far_region_if_both_with_good_rcs)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor with acceptable RCS
    *                                                   object 2 - further from sensor with acceptable RCS
    * Set necessary tracker data.
    */
   Point center = {-10.0F, -10.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   objects[0].average_rcs = 0.1F; // Set acceptable RCS value for object 1
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {-15.0F, -15.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   objects[1].average_rcs = 0.1F; // Set acceptable RCS value for object 2
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(13.0F, 13.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ -50.0F, -50.0F };

   std::pair<bool, Point> expected_intersection = { true, {-9.0F, -9.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks that when RCS values are too low, objects in the far region will not be selected as reflector.
 * Objects are on left host side so they have negative vcs lateral position
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, too_low_rcs_objects_will_not_be_selected_in_far_region)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor with too low RCS
    *                                                   object 2 - further from sensor with too low RCS
    * Set necessary tracker data.
    */
   Point center = {-10.0F, -10.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   objects[0].average_rcs = 0.0F; // Set too low RCS value for object 1
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {-15.0F, -15.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   objects[1].average_rcs = 0.0F; // Set too low RCS value for object 2
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(13.0F, 13.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ -50.0F, -50.0F };


   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * No intersection point should be returned since RCS values are too low
    */
   CHECK_FALSE(result.first);
}


/** \purpose
 * This test checks whether object closer to sensor is selected as reflector at the rear of host.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_object_closer_to_sensor_is_selected_as_reflector_at_the_rear_of_host)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor,
    *                                                   object 2 - further from sensor
    * Set necessary tracker data.
    */

   Point center = {-2.0F,-2.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   objects[0].average_rcs = 0.1F; // Set acceptable RCS value for object 1
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {-3.0F,-3.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   objects[1].average_rcs = 0.1F; // Set acceptable RCS value for object 2
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ -10.0F, -10.0F };

   std::pair<bool, Point> expected_intersection = { true, {-1.0F, -1.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}


/** \purpose
 * Checks if reflector selector reset array with objects inside inner zone between calls.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, reseting_array_of_objects_which_are_inside_inner_zone)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - inside object 2, further from sensor "metal to metal",
    *                                                   object 2 - closer to sensor "metal to metal"
    * Set necessary tracker data.
    */
   Point center = {4.0F,6.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].bbox.Set_Width(2);
   objects[0].bbox.Set_Length(2);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {6.0F,6.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].bbox.Set_Width(2);
   objects[1].bbox.Set_Length(2);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined items position
    * Call Get_Reflection_Point() for first item
    */
   const Point sensor_posn{ -4.0F, 6.0F };
   const Point first_item_posn{ 12.0F, 6.0F };  // both objects are inside inner zone
   const Point second_item_posn{ 5.0F, 6.0F };  // only object 1 is inside inner zone (but object 2 is closer to the sensor "metal to metal")
   refl_selector.Get_Reflection_Point(sensor_posn, first_item_posn, objects);

   /** \action
    * Call Get_Reflection_Point() for second item
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, second_item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   const std::pair<bool, Point> expected_intersection = { true, { 2.0F, 6.0F} };
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks whether closer object is selected when its lateral position is equal to sensor lat pos and tested item lat pos. All have the same lateral position.
 * Objects are at front of host so they have positive longitudinal vcs position.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_obj_selected_when_its_lat_pos_is_equal_to_sensor_lat_pos_and_tested_item_lat_pos_at_host_front)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor, same lateral position as sensor
    *                                                   object 2 - further from sensor, same lateral position as sensor
    * Set necessary tracker data.
    */

   Point center = {5.0F,0.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {6.0F,0.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 10.0F, 0.0F };

   std::pair<bool, Point> expected_intersection = { true, {3.0F, 0.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks whether closer object is selected when its lateral position is equal to sensor lat pos and tested item lat pos. All have the same lateral position.
 * Objects are at rear of host so they have negative longitudinal vcs position.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_obj_selected_when_its_lat_pos_is_equal_to_sensor_lat_pos_and_tested_item_lat_pos_at_host_rear)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor, same lateral position as sensor
    *                                                   object 2 - further from sensor, same lateral position as sensor
    * Set necessary tracker data.
    */

   Point center = {-5.0F,0.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {-6.0F,0.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ -10.0F, 0.0F };

   std::pair<bool, Point> expected_intersection = { true, {-3.0F, 0.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks whether closer object is selected when its longitudinal position is equal to sensor long pos and tested item llong pos. All have the same longitudinal position.
 * Objects are on right host side so they have positive vcs lateral position
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_object_selected_when_its_long_pos_is_equal_to_sensor_long_pos_and_tested_item_long_pos_right_host_side)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor, same longitudinal position as sensor
    *                                                   object 2 - further from sensor, same longitudinal position as sensor
    * Set necessary tracker data.
    */
   Point center = {0.0F,5.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {0.0F,6.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 0.0F, 10.0F };

   std::pair<bool, Point> expected_intersection = { true, {0.0F, 4.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * This test checks whether closer object is selected when its longitudinal position is equal to sensor long pos and tested item long pos. All have the same longitudinal position.
 * Objects are on left host side so they have negative vcs lateral position
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_object_selected_when_its_long_pos_is_equal_to_sensor_long_pos_and_tested_item_long_pos_Left_host_side)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor, same longitudinal position as sensor
    *                                                   object 2 - further from sensor, same longitudinal position as sensor
    * Set necessary tracker data.
    */
   Point center = {0.0F,-5.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {0.0F,-6.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 0.0F, -10.0F };

   std::pair<bool, Point> expected_intersection = { true, {0.0F, -4.0F} };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Intersection point with object 1 should be returned
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(expected_intersection.second.y, result.second.y, F360_EPSILON);
   DOUBLES_EQUAL(expected_intersection.second.x, result.second.x, F360_EPSILON);
}

/** \purpose
 * Check if reflection point is reseted between calls
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, reset_output_between_calls)
{
   /** \precond 1
    * Create two tracker objects with required params : object 1 - closer to sensor, same longitudinal position as sensor
    *                                                   object 2 - further from sensor, same longitudinal position as sensor
    * Set necessary tracker data.
    */
   Point center = {0.0F,-5.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {0.0F,-6.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 0.0F, -10.0F };

   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);
   CHECK_TRUE(result.first); // Check for sure that before test call algorithm found reflection point

   item_posn = { 100.0F, -10.0F };

   /** \action
    * Call Get_Reflection_Point()
    */
   result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * Reflection point is reseted
    */
   CHECK_FALSE(result.first);
}

/** \purpose
 * This test checks whether object inside inner zone that have no intersection point is not selected as reflector
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_object_is_not_selected_as_reflector_when_do_not_has_intersection_point)
{
   /** \precond 1
    * Create tracker object inside inner zone that there is no intersection point with line sensor_pos-item_pos
    */
   Point center = {2.0F,7.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   objects[0].average_rcs = -0.5F;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 10.0F, 10.0F };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * No reflection/intersection point.
    */
   CHECK_FALSE(result.first);
}

/** \purpose
 * This test checks whether object outside inner zone but inside outer zone is not selected as reflector
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_object_outside_inner_zone_but_inside_outer_zone_is_not_selected_as_reflector)
{
   /** \precond 1
    * Create tracker object inside outer zone but ooutside inner zone.
    */
   Point center = {2.0F,7.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));


   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 3.0F, 3.0F };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * No reflection/intersection point
    */
   CHECK_FALSE(result.first);
}

/** \purpose
 * This test checks whether object inside inner zone with flag f_moving == true is not raported as reflector
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_f_moving_object_is_not_selected_as_reflector)
{
   /** \precond 1
    * Create tracker object inside outer zone but ooutside inner zone.
    */
   Point center = {5.0F,5.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   objects[0].f_moving = true;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));


   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 10.0F, 10.0F };

   /** \action
    * Call Get_Reflection_Point()
    */
   std::pair<bool, Point> result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result
    * No reflection/intersection point
    */
   CHECK_FALSE(result.first);
}

/** \purpose
 * This test checks whether outer zone is deefined properly and no outlaying objects selected.
 * \req
 * NA.
 */
TEST(Get_Reflection_Point, if_object_not_selected_when_close_to_outer_zone_outside)
{
   /** \precond 1
    * Create four tracker objects with required params : object 1 - out of zone, near border, in fron of host, same lateral position as sensor
    *                                                    object 2 - out of zone, near border, at rear of host, same lateral position as sensor
    *                                                    object 3 - out of zone, near border, at host left side, same longitudinal position as sensor
    *                                                    object 4 - out of zone, near border, at host right side, same longitudinal position as sensor
    * Set necessary tracker data.
    */
   Point center = {10.1F,10.0F};
   objects[0].bbox.Set_Center(center);
   objects[0].vcs_position = center;
   tracker_info.num_active_objs = 1;
   Sorted_Tracks_Insert(tracker_info, &(objects[0]));

   center = {-10.1F,0.0F};
   objects[1].bbox.Set_Center(center);
   objects[1].vcs_position = center;
   tracker_info.num_active_objs = 2;
   Sorted_Tracks_Insert(tracker_info, &(objects[1]));

   center = {0.0F,-10.1F};
   objects[2].bbox.Set_Center(center);
   objects[2].vcs_position = center;
   tracker_info.num_active_objs = 3;
   Sorted_Tracks_Insert(tracker_info, &(objects[2]));

   center = {0.0F,10.1F};
   objects[3].bbox.Set_Center(center);
   objects[3].vcs_position = center;
   tracker_info.num_active_objs = 4;
   Sorted_Tracks_Insert(tracker_info, &(objects[3]));

   /** \precond 2
    * Create object Object_Track_Reflector_Selector by paramerized constructor, with outer zone half length and width = 10.0
    */
   Object_Track_Reflector_Selector refl_selector = Object_Track_Reflector_Selector(10.0F, 10.0F, calibs.k_mp_object_reflector_size_extension, tracker_info);

   /** \precond 3
    * Set sensor position and examined item position
    */
   Point sensor_posn{ 0.0F, 0.0F };
   Point item_posn{ 0.0F, 0.0F };

   std::pair<bool, Point> result{ true, {} };

   /** \action 1
    * Call Get_Reflection_Point() with object 1 perspective, item_posn [10.2, 0.0]
    */
   item_posn = { 10.2F, 0.0F };
   result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result 1
    * Intersection point should not be returned
    */
   CHECK_FALSE(result.first);

   /** \action 2
    * Call Get_Reflection_Point() with object 2 perspective, item_posn [-10.2, 0.0]
    */
   item_posn = { -10.2F, 0.0F };
   result = { true, {} };     // Reset value
   result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result 2
    * Intersection point should not be returned
    */
   CHECK_FALSE(result.first);

   /** \action 3
    * Call Get_Reflection_Point() with object 3 perspective, item_posn [0.0, -10.2]
    */
   item_posn = { 0.0F, -10.2F };
   result = { true, {} };     // Reset value
   result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result 3
    * Intersection point should not be returned
    */
   CHECK_FALSE(result.first);

   /** \action 4
    * Call Get_Reflection_Point() with object 4 perspective, item_posn [0.0, 10.2]
    */
   item_posn = { 0.0F, 10.2F };
   result = { true, {} };     // Reset value
   result = refl_selector.Get_Reflection_Point(sensor_posn, item_posn, objects);

   /** \result 4
    * Intersection point should not be returned
    */
   CHECK_FALSE(result.first);
}

/** @}*/
