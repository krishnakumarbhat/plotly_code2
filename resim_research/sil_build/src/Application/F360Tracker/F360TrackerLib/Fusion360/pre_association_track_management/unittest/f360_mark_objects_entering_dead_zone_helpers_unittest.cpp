/** \file
 * This file contains unit tests for content of f360_mark_objects_entering_dead_zone_helpers.cpp file
 */

#include "f360_mark_objects_entering_dead_zone_helpers.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  is_in_dead_zone
 *  @{
 */

 /** \brief
  * Purpose of this test is to verify whether it is properly analysed whether object centroid is inside dead zone
  */
TEST_GROUP(is_in_dead_zone)
{
   float32_t centroid_long_pos{};
   Interval<float32_t> zone{};
   /** \setup
    * Set up zone limits
    */
   TEST_SETUP()
   {
      zone.lower = -5.0F;
      zone.upper = 0.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when centroid longitudinal position is in dead zone function returns true.
 * \req
 * NA.
 */
TEST(is_in_dead_zone, Is_In_Dead_Zone__Is_Inside)
{
   /** \precond
    * Set up object centroid position to be inside dead zone
    */
   centroid_long_pos = -2.5F;

   /** \action
    * Call tested function
    */
   const bool result = Is_Center_In_Dead_Zone(centroid_long_pos, zone);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Purpose of this test is to verify whether when centroid longitudinal position is not in dead zone function returns false.
 * \req
 * NA.
 */
TEST(is_in_dead_zone, Is_In_Dead_Zone__Is_Outside)
{
   /** \precond
    * Set up object centroid position to be outside of dead zone
    */
   centroid_long_pos = 5.0F;

   /** \action
    * Call tested function
    */
   const bool result = Is_Center_In_Dead_Zone(centroid_long_pos, zone);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(result);
}
/** @}*/

/** \defgroup  is_rear_in_zone
 *  @{
 */

 /** \brief
  * Purpose of this test group is to verify whether function properly determines whether object rear side of bbox is inside dead zone
  */
TEST_GROUP(is_rear_in_zone)
{
   BboxCorners bbox{};
   Interval<float32_t> zone{};
   /** \setup
    * Set up zone limits
    */
   TEST_SETUP()
   {
      zone.lower = -5.0F;
      zone.upper = 0.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when left rear corner is in zone function returns true.
 * \req
 * NA.
 */
TEST(is_rear_in_zone, Is_Rear_In_Zone__Left_Rear_Corner_In_Zone)
{
   /** \precond
    * Set up object rear left corner to be inside zone
    * Set up object rear right corner to be outside zone
    */
   bbox.Rear_Left().x = -2.5F;
   bbox.Rear_Right().x = 5.0F;

   /** \action
    * Call tested funciton
    */
   const bool result = Is_Rear_In_Zone(bbox, zone);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Purpose of this test is to verify whether when left rear corner is in zone function returns true.
 * \req
 * NA.
 */
TEST(is_rear_in_zone, Is_Rear_In_Zone__Right_Rear_Corner_In_Zone)
{
   /** \precond
    * Set up object rear left corner to be outside zone
    * Set up object rear right corner to be inside zone
    */
   bbox.Rear_Left().x = 5.0F;
   bbox.Rear_Right().x = -2.5F;

   /** \action
    * Call tested funciton
    */
   const bool result = Is_Rear_In_Zone(bbox, zone);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Purpose of this test is to verify whether when rear corners are not inside zone function returns false
 * \req
 * NA.
 */
TEST(is_rear_in_zone, Is_Rear_In_Zone__Rear_Corners_Not_In_Zone)
{
   /** \precond
    * Set up object rear left corner to be outside zone
    * Set up object rear right corner to be outside zone
    */
   bbox.Rear_Left().x = 5.0F;
   bbox.Rear_Right().x = 5.0F;

   /** \action
    * Call tested funciton
    */
   const bool result = Is_Rear_In_Zone(bbox, zone);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(result);
}
/** @}*/



/** \defgroup  is_front_in_zone
 *  @{
 */

 /** \brief
  * Purpose of this test group is to verify whether function properly determines whether object front side of bbox is inside dead zone
  */
TEST_GROUP(is_front_in_zone)
{
   BboxCorners bbox{};
   Interval<float32_t> zone{};
   /** \setup
    * Set up zone limits
    */
   TEST_SETUP()
   {
      zone.lower = -5.0F;
      zone.upper = 0.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when front left corner is in zone function returns true.
 * \req
 * NA.
 */
TEST(is_front_in_zone, Is_Front_In_Zone__Left_Front_Corner_In_Zone)
{
   /** \precond
    * Set up object front left corner to be inside zone
    * Set up object front right corner to be outside zone
    */
   bbox.Front_Left().x = -2.5F;
   bbox.Front_Right().x = 5.0F;

   /** \action
    * Call tested funciton
    */
   const bool result = Is_Front_In_Zone(bbox, zone);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Purpose of this test is to verify whether when front right corner is in zone function returns true.
 * \req
 * NA.
 */
TEST(is_front_in_zone, Is_Front_In_Zone__Right_Front_Corner_In_Zone)
{
   /** \precond
    * Set up object front left corner to be outside zone
    * Set up object front right corner to be inside zone
    */
   bbox.Front_Left().x = 5.0F;
   bbox.Front_Right().x = -2.5F;

   /** \action
    * Call tested funciton
    */
   const bool result = Is_Front_In_Zone(bbox, zone);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Purpose of this test is to verify whether when front corners are not inside zone function returns false
 * \req
 * NA.
 */
TEST(is_front_in_zone, Is_Front_In_Zone__Front_Corners_Not_In_Zone)
{
   /** \precond
    * Set up object front left corner to be outside zone
    * Set up object front right corner to be outside zone
    */
   bbox.Front_Left().x = 5.0F;
   bbox.Front_Right().x = 5.0F;

   /** \action
    * Call tested funciton
    */
   const bool result = Is_Front_In_Zone(bbox, zone);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(result);
}
/** @}*/

/** \defgroup  determine_object_dead_zone_status
 *  @{
 */

 /** \brief
  * Test group of Determine_Object_Dead_Zone_Status function. Tests verify whether dead zone status
  * of track is properly updated.
  */
TEST_GROUP(determine_object_dead_zone_status)
{
   Dead_Zone_T dead_zone{};
   F360_Calibrations_T calib{};
   F360_Object_Track_T object{};

   /** \setup
    * Set up zone limits
    * Initialize tracker calibrations
    * Set up base object position and size
    * Set up host speed
    */
   TEST_SETUP()
   {
      dead_zone.basic.upper = 0.0F;
      dead_zone.extended.upper = 4.0F;

      dead_zone.basic.lower = -5.0F;
      dead_zone.extended.lower = -9.0F;

      Initialize_Tracker_Calibrations(calib);

      object.vcs_position.y = 2.0F;
      object.vcs_position.x = -2.5;
      object.bbox.Set_Length(2.0F);

      object.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = object.vcs_position;
      object.bbox.Set_Center(center);
      object.bbox.Set_Length(2.0F);
      object.Set_Bbox_Orientation(Angle{ 0.0F });
   }
};

/** \purpose
 * Purpose of this test is to verify whether when object centroid is inside zone function returns F360_Dead_Zone_Status_T::INSIDE
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status__Meets_Conditions_Is_In_Center)
{
   /** \precond
    * All is set in TEST_SETUP
    */


    /** \action
     * Call tested function
     */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::INSIDE
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::INSIDE, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object front bumper inside simplified zone function returns F360_Dead_Zone_Status_T::IN_REAR
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status__Front_Bumper_Inside_Simplified_Dead_Zone)
{
   /** \precond
    * Set object vcs position to make its front bumper be in simplified dead zone
    */
   object.vcs_position.x = -5.5F;
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);

    /** \action
     * Call tested function
     */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::IN_REAR
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::IN_REAR, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object front bumper inside extended zone function returns F360_Dead_Zone_Status_T::ENTERING_REAR
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status__Front_Bumper_Inside_Extended_Dead_Zone)
{
   /** \precond
    * Set object vcs position to make its front bumper be in extended dead zone
    */
   object.vcs_position.x = -9.5F;
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);

   /** \action
    * Call tested function
    */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::ENTERING_REAR
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::ENTERING_REAR, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object inside posterior extended zone, function returns F360_Dead_Zone_Status_T::ENTERING_REAR
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status__Object_Inside_Posterior_Extended_Dead_Zone)
{
   /** \precond
    * Set object vcs position in posterior extended dead zone (front and rear bumper inside posterior extended zone)
    */
   object.vcs_position.x = -7.0F;
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);

   /** \action
    * Call tested function
    */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::ENTERING_REAR
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::ENTERING_REAR, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object front bumper inside simplified zone function returns F360_Dead_Zone_Status_T::IN_FRONT
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status_Rear_Bumper_Inside_Simplified_Dead_Zone)
{
   /** \precond
    * Set object vcs position to make its rear bumper be in simplified dead zone
    */
   object.vcs_position.x = 0.5F;
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);

   /** \action
    * Call tested function
    */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::IN_FRONT
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::IN_FRONT, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object front bumper inside extended zone function returns F360_Dead_Zone_Status_T::ENTERING_FRONT
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status_Rear_Bumper_Inside_Extended_Dead_Zone)
{
   /** \precond
    * Set object vcs position to make its rear bumper be in extended dead zone
    */
   object.vcs_position.x = 4.5F;
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);

   /** \action
    * Call tested function
    */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::ENTERING_FRONT
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::ENTERING_FRONT, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object inside frontal extended zone, function returns F360_Dead_Zone_Status_T::ENTERING_FRONT
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status_Object_Inside_Frontal_Extended_Dead_Zone)
{
   /** \precond
    * Set object vcs position in frontal extended dead zone (front and rear bumper inside frontal extended zone)
    */
   object.vcs_position.x = 2.0F;
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);

   /** \action
    * Call tested function
    */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::ENTERING_FRONT
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::ENTERING_FRONT, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object is not in zone function returns F360_Dead_Zone_Status_T::OUTSIDE
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status__Object_Not_In_Zone)
{
   /** \precond
    * All is set in TEST_SETUP
    * Set up object position to make it not in zone
    */
   object.vcs_position.x = 10.0F;
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);

   /** \action
    * Call tested function
    */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::OUTSIDE
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::OUTSIDE, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object lateral position is too high function returns F360_Dead_Zone_Status_T::OUTSIDE
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status__Object_Lateral_Position_Too_High)
{
   /** \precond
    * All is set in TEST_SETUP
    * Set up object lateral position to make it too high
    */
   object.vcs_position.y = calib.k_dead_zone_max_obj_vcs_lat_pos + 10.0F;
   object.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = object.vcs_position;
   object.bbox.Set_Center(center);

   /** \action
    * Call tested function
    */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::OUTSIDE
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::OUTSIDE, result);
}

/** \purpose
 * Purpose of this test is to verify whether when object vcs heading is too high function returns F360_Dead_Zone_Status_T::OUTSIDE
 * \req
 * NA.
 */
TEST(determine_object_dead_zone_status, Determine_Object_Dead_Zone_Status__Object_VCS_Heading_Too_High)
{
   /** \precond
    * All is set in TEST_SETUP
    * Set up object vcs heading to make it too high
    */
   object.vcs_heading = Angle{ calib.k_dead_zone_max_obj_vcs_heading + 0.1F };

   /** \action
    * Call tested function
    */
   const F360_Dead_Zone_Status_T result = Determine_Object_Dead_Zone_Status(dead_zone, object, calib.k_dead_zone_max_obj_vcs_lat_pos, calib.k_dead_zone_max_obj_vcs_heading);

   /** \result
    * Check whether returned value is equal to F360_Dead_Zone_Status_T::OUTSIDE
    */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::OUTSIDE, result);
}
/** @}*/
