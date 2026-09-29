/** \file
 * This file contains unit tests for content of f360_object_based_multibounce_detector_internals.cpp file
 */

#include "f360_object_based_multibounce_detector_internals.h"
#include <CppUTest/TestHarness.h>
#include <limits>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_based_multibounce_detector_internals__Create_Area_Of_Correct_Detections
 *  @{
 */

/** \brief
 * Test behavior of Create_Area_Of_Correct_Detections() function.
 */
TEST_GROUP(f360_object_based_multibounce_detector_internals__Create_Area_Of_Correct_Detections)
{	
   const float tolerance = 0.000001F;

   F360_Object_Track_T obj_track;     
   float32_t area_width;

   /** \setup
    * COnfigure object on the right from the host
    */
   TEST_SETUP()
   {
      area_width = 1.2F;
      
      obj_track.vcs_position = Point{ -2.0F, 6.0F };
      obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = {-2.0F, 6.0F};
      obj_track.bbox.Set_Center(center);
      obj_track.bbox.Set_Length(5.0F);
      obj_track.bbox.Set_Width(2.0F);
   }
};

/** \purpose  
 * Check calculations correctness when obejct is on the right
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Create_Area_Of_Correct_Detections, Correct_Area_Estimation_When_Object_Is_On_The_Right)
{
   /** \precond
    * Same as setup.
    */
	
   /** \action
    * Call Create_Area_Of_Correct_Detections()
    */
   const BoundingBox bbox = Create_Area_Of_Correct_Detections(obj_track, area_width);

   /** \result
    * Correct size and center position
    */
   DOUBLES_EQUAL(5.0F, bbox.Get_Length(), tolerance);
   DOUBLES_EQUAL(1.2F, bbox.Get_Width(), tolerance);
   DOUBLES_EQUAL(-2.0F, bbox.Get_Center().x, tolerance);
   DOUBLES_EQUAL(5.6F, bbox.Get_Center().y, tolerance);
}

/** \purpose
 * Check calculations correctness when obejct is on the left
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Create_Area_Of_Correct_Detections, Correct_Area_Estimation_When_Object_Is_On_The_Left)
{
   /** \precond
    * Same as setup with below change:
    * set object to be no the left side (neg lateral position)
    */
   obj_track.vcs_position.y = -obj_track.vcs_position.y;
   Point center = obj_track.vcs_position;
   obj_track.bbox.Set_Center(center);

    /** \action
     * Call Create_Area_Of_Correct_Detections()
     */
   const BoundingBox bbox = Create_Area_Of_Correct_Detections(obj_track, area_width);

   /** \result
    * Correct size and center position
    */
   DOUBLES_EQUAL(5.0F, bbox.Get_Length(), tolerance);
   DOUBLES_EQUAL(1.2F, bbox.Get_Width(), tolerance);
   DOUBLES_EQUAL(-2.0F, bbox.Get_Center().x, tolerance);
   DOUBLES_EQUAL(-5.6F, bbox.Get_Center().y, tolerance);
}

/** \purpose
 * Check calculations correctness when obejct is on the right
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Create_Area_Of_Correct_Detections, Correct_Area_Estimation_When_Object_Is_On_The_Right_Extended)
{
   /** \precond
    * set object to be on the right side (lat < 2.5F) with less than 3.0 in longi
    */
   obj_track.vcs_position = Point{ -2.0F, 2.0F };
   obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = {-2.0F, 2.0F};
   obj_track.bbox.Set_Center(center);
   obj_track.bbox.Set_Length(5.0F);
   obj_track.bbox.Set_Width(2.0F);

    /** \action
     * Call Create_Area_Of_Correct_Detections()
     */
   const BoundingBox bbox = Create_Area_Of_Correct_Detections(obj_track, area_width);

   /** \result
    * Correct size and center position
    */
   DOUBLES_EQUAL(5.0F, bbox.Get_Length(), tolerance);
   DOUBLES_EQUAL(2.2F, bbox.Get_Width(), tolerance);
   DOUBLES_EQUAL(-2.0F, bbox.Get_Center().x, tolerance);
   DOUBLES_EQUAL(2.1F, bbox.Get_Center().y, tolerance);
}

/** \purpose
 * Check calculations correctness when obejct is on the right
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Create_Area_Of_Correct_Detections, Correct_Area_Estimation_When_Object_Is_On_The_Right_Not_Extended)
{
   /** \precond
    * set object to be on the right side (neg lateral position) with more than 3.0 in longi
    */
   obj_track.vcs_position = Point{ 4.0F, 2.0F };
   obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = { 4.0F, 2.0F};
   obj_track.bbox.Set_Center(center);
   obj_track.bbox.Set_Length(5.0F);
   obj_track.bbox.Set_Width(2.0F);

    /** \action
     * Call Create_Area_Of_Correct_Detections()
     */
   const BoundingBox bbox = Create_Area_Of_Correct_Detections(obj_track, area_width);

   /** \result
    * Correct size and center position
    */
   DOUBLES_EQUAL(5.0F, bbox.Get_Length(), tolerance);
   DOUBLES_EQUAL(1.2F, bbox.Get_Width(), tolerance);
   DOUBLES_EQUAL(4.0F, bbox.Get_Center().x, tolerance);
   DOUBLES_EQUAL(1.6F, bbox.Get_Center().y, tolerance);
}

/** \purpose
 * Check calculations correctness when obejct is on the left
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Create_Area_Of_Correct_Detections, Correct_Area_Estimation_When_Object_Is_On_The_Left_Extended)
{
   /** \precond
    * Same as setup with below change:
    * set object to be no the left side (neg lateral position)
    */
   obj_track.vcs_position = Point{ -2.0F, -2.0F };
   obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = {-2.0F, -2.0F};
   obj_track.bbox.Set_Center(center);
   obj_track.bbox.Set_Length(5.0F);
   obj_track.bbox.Set_Width(2.0F);

    /** \action
     * Call Create_Area_Of_Correct_Detections()
     */
   const BoundingBox bbox = Create_Area_Of_Correct_Detections(obj_track, area_width);

   /** \result
    * Correct size and center position
    */
   DOUBLES_EQUAL(5.0F, bbox.Get_Length(), tolerance);
   DOUBLES_EQUAL(2.2F, bbox.Get_Width(), tolerance);
   DOUBLES_EQUAL(-2.0F, bbox.Get_Center().x, tolerance);
   DOUBLES_EQUAL(-1.6F, bbox.Get_Center().y, tolerance);
}

/** \purpose
 * Check calculations correctness when obejct is on the left
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Create_Area_Of_Correct_Detections, Correct_Area_Estimation_When_Object_Is_On_The_Left_Not_Extended)
{
   /** \precond
    * set object to be on the left side (neg lateral position) with more than 3.0 in longi
    */
   obj_track.vcs_position = Point{ 4.0F, -2.0F };
   obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = { 4.0F, -2.0F};
   obj_track.bbox.Set_Center(center);
   obj_track.bbox.Set_Length(5.0F);
   obj_track.bbox.Set_Width(2.0F);

    /** \action
     * Call Create_Area_Of_Correct_Detections()
     */
   const BoundingBox bbox = Create_Area_Of_Correct_Detections(obj_track, area_width);

   /** \result
    * Correct size and center position
    */
   DOUBLES_EQUAL(5.0F, bbox.Get_Length(), tolerance);
   DOUBLES_EQUAL(1.2F, bbox.Get_Width(), tolerance);
   DOUBLES_EQUAL(4.0F, bbox.Get_Center().x, tolerance);
   DOUBLES_EQUAL(-1.6F, bbox.Get_Center().y, tolerance);
}

/** \defgroup  f360_object_based_multibounce_detector_internals__Find_Bounce_Origin_Point
 *  @{
 */

 /** \brief
  * Test behavior of Find_Bounce_Origin_Point() function.
  */
TEST_GROUP(f360_object_based_multibounce_detector_internals__Find_Bounce_Origin_Point)
{
   const float tolerance = 0.000001F;

   Point detection_pos_vcs{};
   Point sensor_mount_pos_vcs{};
   BoundingBox restricted_area{};

   /** \setup
    * Configure scenario in order to get bounce point
    */
   TEST_SETUP()
   {
      detection_pos_vcs = { 1.0F, 7.0F };
      sensor_mount_pos_vcs = { -1.0F, 1.0F };

      restricted_area = BoundingBox(Point(-1.0F, 3.0F), 4.0F, 2.0F, Angle{ 0.0F });
   }
};

/** \purpose
 * Check if bounce point is correctly estimated
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Find_Bounce_Origin_Point, estimation_correctness)
{
   /** \precond
    * Same as setup.
    */

    /** \action
     * Call Find_Bounce_Origin_Point()
     */
   const Bounce_Origin result_point = Find_Bounce_Origin_Point(detection_pos_vcs, sensor_mount_pos_vcs, restricted_area);

   /** \result
    * x = -0.(3), y = 3.0F
    */
   CHECK_TRUE(result_point.f_found);
   DOUBLES_EQUAL(-0.33333333333F, result_point.position.x, tolerance);
   DOUBLES_EQUAL(3.0F, result_point.position.y, tolerance);
}

/** \purpose
 * Check if bounce point is not found if no intersection
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Find_Bounce_Origin_Point, point_not_found_due_to_no_intersection)
{
   /** \precond
    * Same as setup with below change:
    * are position (-3, 3)
    */
   restricted_area = BoundingBox(Point(-3.0F, 3.0F), 4.0F, 2.0F, Angle{ 0.0F });

    /** \action
     * Call Find_Bounce_Origin_Point()
     */
   const Bounce_Origin result_point = Find_Bounce_Origin_Point(detection_pos_vcs, sensor_mount_pos_vcs, restricted_area);

   /** \result
    * Point not found
    */
   CHECK_FALSE(result_point.f_found);
}
/** @}*/


/** \defgroup  f360_object_based_multibounce_detector_internals__Is_Det_Object_Based_Multibounce
 *  @{
 */

 /** \brief
  * Test behavior of Is_Det_Object_Based_Multibounce() function.
  */
TEST_GROUP(f360_object_based_multibounce_detector_internals__Is_Det_Object_Based_Multibounce)
{
   const float tolerance = 0.000001F;

   F360_VCS_Velocity_T sensor_velocity{};
   Point sensor_mount_pos_vcs{};
   F360_Detection_Props_T det_prop{};
   rspp_variant_A::RSPP_Detection_T det_raw{};
   F360_VCS_Velocity_T obj_velocity_vcs{};
   BoundingBox restricted_area{};
   F360_Calibrations_T calibs{};

   /** \setup
    * Configure scenario in order to get bounce point
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      det_prop.vcs_position = { 1.0F, 7.0F };
      det_raw.raw.range = 6.3246F;
      det_raw.raw.range_rate = 0.63246F * 3.0F;
      det_raw.processed.vcs_az = F360_DEG2RAD(71.56F);
      det_raw.processed.cos_vcs_az = F360_Cosf(det_raw.processed.vcs_az);
      det_raw.processed.sin_vcs_az = F360_Sinf(det_raw.processed.vcs_az);

      sensor_mount_pos_vcs = { -1.0F, 1.0F };
      sensor_velocity = { 20.0F , 0.0F };

      obj_velocity_vcs = { 22.0F, 0.0F };

      restricted_area = BoundingBox(Point(-1.0F, 3.0F), 4.0F, 2.0F, Angle{ 0.0F });
   }
};

/** \purpose
 * Check whether function returns true when all conditions are met
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Is_Det_Object_Based_Multibounce, identified_as_multibounce)
{
   /** \precond
    * Same as setup.
    */

    /** \action
     * Call Is_Det_Object_Based_Multibounce()
     */
   const bool f_multibounce = Is_Det_Object_Based_Multibounce(sensor_velocity, sensor_mount_pos_vcs, det_prop, det_raw, obj_velocity_vcs, restricted_area, calibs);

   /** \result
    * True
    */
   CHECK_TRUE(f_multibounce);
}

/** \purpose
 * Check whether function returns false when bounce point is not found
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Is_Det_Object_Based_Multibounce, not_identified_as_multibounce_due_to_lack_of_bounce_point)
{
   /** \precond
    * Same as setup with below change:
    * restriceted area center in (-3, 3)
    */
   restricted_area = BoundingBox(Point(-3.0F, 3.0F), 4.0F, 2.0F, Angle{ 0.0F });

    /** \action
     * Call Is_Det_Object_Based_Multibounce()
     */
   const bool f_multibounce = Is_Det_Object_Based_Multibounce(sensor_velocity, sensor_mount_pos_vcs, det_prop, det_raw, obj_velocity_vcs, restricted_area, calibs);

   /** \result
    * False
    */
   CHECK_FALSE(f_multibounce);
}

/** \purpose
 * Check whether function returns false when dividing factor is too low
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Is_Det_Object_Based_Multibounce, not_identified_as_multibounce_due_to_too_low_dividing_factor)
{
   /** \precond
    * Same as setup with below change:
    * set detection parameters that provide low dividing factor
    */
   det_prop.vcs_position = { -0.5F, 3.5F };
   det_raw.raw.range = 2.55F;
   det_raw.processed.vcs_az = F360_DEG2RAD(78.69F);
   det_raw.processed.cos_vcs_az = F360_Cosf(det_raw.processed.vcs_az);
   det_raw.processed.sin_vcs_az = F360_Sinf(det_raw.processed.vcs_az);

   /** \action
    * Call Is_Det_Object_Based_Multibounce()
    */
   const bool f_multibounce = Is_Det_Object_Based_Multibounce(sensor_velocity, sensor_mount_pos_vcs, det_prop, det_raw, obj_velocity_vcs, restricted_area, calibs);

   /** \result
    * False
    */
   CHECK_FALSE(f_multibounce);
}


/** \purpose
 * Check whether function returns false when dividing factor is too high
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Is_Det_Object_Based_Multibounce, not_identified_as_multibounce_due_to_too_high_dividing_factor)
{
   /** \precond
    * Same as setup with below change:
    * set detection parameters that provide high dividing factor
    */
   det_prop.vcs_position = { 6.0F, 10.5F };
   det_raw.raw.range = 11.8F;
   det_raw.processed.vcs_az = F360_DEG2RAD(56.62F);
   det_raw.processed.cos_vcs_az = F360_Cosf(det_raw.processed.vcs_az);
   det_raw.processed.sin_vcs_az = F360_Sinf(det_raw.processed.vcs_az);

   /** \action
    * Call Is_Det_Object_Based_Multibounce()
    */
   const bool f_multibounce = Is_Det_Object_Based_Multibounce(sensor_velocity, sensor_mount_pos_vcs, det_prop, det_raw, obj_velocity_vcs, restricted_area, calibs);

   /** \result
    * False
    */
   CHECK_FALSE(f_multibounce);
}

/** \purpose
 * Check whether function returns false when corrected detection (range) is not in restricted area
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Is_Det_Object_Based_Multibounce, not_identified_as_multibounce_due_to_corrected_det_not_inside_area)
{
   /** \precond
    * Same as setup with below change:
    * set detection parameters to get corrected detecion out of area
    * make restriccted area thiner
    */
   det_prop.vcs_position = { 2.0F, 5.5F };
   det_raw.raw.range = 5.408F;
   det_raw.processed.vcs_az = F360_DEG2RAD(56.31F);
   det_raw.processed.cos_vcs_az = F360_Cosf(det_raw.processed.vcs_az);
   det_raw.processed.sin_vcs_az = F360_Sinf(det_raw.processed.vcs_az);

   restricted_area = BoundingBox(Point(-1.0F, 3.0F), 4.0F, 0.4F, Angle{ 0.0F });

   /** \action
    * Call Is_Det_Object_Based_Multibounce()
    */
   const bool f_multibounce = Is_Det_Object_Based_Multibounce(sensor_velocity, sensor_mount_pos_vcs, det_prop, det_raw, obj_velocity_vcs, restricted_area, calibs);

   /** \result
    * False
    */
   CHECK_FALSE(f_multibounce);
}

/** \purpose
 * Check whether function returns false when corrected detection range rate does not fit an object
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Is_Det_Object_Based_Multibounce, not_identified_as_multibounce_due_to_corrected_range_rate_does_not_fit_object)
{
   /** \precond
    * Same as setup with below change:
    * set detection range rate to 6.5
    */
   det_raw.raw.range_rate = 6.5F;

   /** \action
    * Call Is_Det_Object_Based_Multibounce()
    */
   const bool f_multibounce = Is_Det_Object_Based_Multibounce(sensor_velocity, sensor_mount_pos_vcs, det_prop, det_raw, obj_velocity_vcs, restricted_area, calibs);

   /** \result
    * False
    */
   CHECK_FALSE(f_multibounce);
}
/** @}*/


/** \defgroup  f360_object_based_multibounce_detector_internals__Estimate_Num_Bounces
 *  @{
 */

 /** \brief
  * Test behavior of Estimate_Num_Bounces() function.
  */
TEST_GROUP(f360_object_based_multibounce_detector_internals__Estimate_Num_Bounces)
{
   const float tolerance = 0.000001F;
};

/** \purpose
 * Check calculation correctness
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Estimate_Num_Bounces, calculation_correctness)
{
   /** \precond
    * 
    */
   Point sensor_mount_pos_vcs{ 2.0F, 1.0F };
   Point bounce_origin_point{ 2.0F, 2.0F };
   float32_t det_range = 1.0F;

    /** \action
     * Call Estimate_Num_Bounces()
     */
   const uint16_t num_bounces = Estimate_Num_Bounces(sensor_mount_pos_vcs, bounce_origin_point, det_range);

   /** \result
    * 1 bounce
    */
   CHECK_EQUAL(1, num_bounces);
}

/** \purpose
 * Check rounding correctness (up)
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Estimate_Num_Bounces, rounding_correctness__up)
{
   /** \precond
    *
    */
   Point sensor_mount_pos_vcs{ 2.0F, 1.0F };
   Point bounce_origin_point{ 2.0F, 3.0F };
   float32_t det_range = 1.0F;

   /** \action
    * Call Estimate_Num_Bounces()
    */
   const uint16_t num_bounces = Estimate_Num_Bounces(sensor_mount_pos_vcs, bounce_origin_point, det_range);

   /** \result
    * 1 bounce
    */
   CHECK_EQUAL(1, num_bounces);
}


/** \purpose
 * Check rounding correctness (down)
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Estimate_Num_Bounces, rounding_correctness__down)
{
   /** \precond
    *
    */
   Point sensor_mount_pos_vcs{ 2.0F, 1.0F };
   Point bounce_origin_point{ 2.0F, 3.0F + tolerance };
   float32_t det_range = 1.0F;

   /** \action
    * Call Estimate_Num_Bounces()
    */
   const uint16_t num_bounces = Estimate_Num_Bounces(sensor_mount_pos_vcs, bounce_origin_point, det_range);

   /** \result
    * 0 bounce
    */
   CHECK_EQUAL(0, num_bounces);
}

/** \purpose
 * Check if function return uint16_t max if sensor and bounce point are the same
 * \req
 * NA
 */
TEST(f360_object_based_multibounce_detector_internals__Estimate_Num_Bounces, returns_max_if_points_are_the_same)
{
   /** \precond
    *
    */
   Point sensor_mount_pos_vcs{ 2.0F, 1.0F };
   Point bounce_origin_point{ sensor_mount_pos_vcs.x, sensor_mount_pos_vcs.y };
   float32_t det_range = 1.0F;

   /** \action
    * Call Estimate_Num_Bounces()
    */
   const uint16_t num_bounces = Estimate_Num_Bounces(sensor_mount_pos_vcs, bounce_origin_point, det_range);

   /** \result
    * std::numeric_limits<uint16_t>::max() bounce
    */
   CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), num_bounces);
}
/** @}*/
