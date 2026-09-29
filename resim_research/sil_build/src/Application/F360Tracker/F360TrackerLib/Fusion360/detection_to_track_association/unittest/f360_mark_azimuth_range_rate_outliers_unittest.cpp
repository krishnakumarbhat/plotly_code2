/** \file
 * This file contains unit tests for content of f360_mark_azimuth_range_rate_outliers.cpp file
 */

#include "f360_mark_azimuth_range_rate_outliers.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_mark_azimuth_range_rate_outliers
 *  @{
 */

/** \brief
 * Test group containing common data for all tests. The parameters are tweaked for
 * the individual test cases.
 */
TEST_GROUP(f360_mark_azimuth_range_rate_outliers)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T obj;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Calibrations_T calibs;
   float32_t host_rear_axle;
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   
   /** \setup
    * Set up an object with three detections associated.
    * Set vcs azimuth of the three detections to 0. 
    * Set compensated range rate of second detection to match object velocity profile. 
    * Set compensated range rate of first and third such that they do not match object velocity profile.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      host_rear_axle = 0.0F;

      obj.vcs_position.x = F360_Sqrtf(calibs.k_az_rdot_max_sq_dist) - 0.5F;
      obj.vcs_position.y = 0.0F;
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = {F360_Sqrtf(calibs.k_az_rdot_max_sq_dist) - 0.5F,0.0F};
      obj.bbox.Set_Center(center);
      obj.speed = 10.0F;
      obj.f_moving = true;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.ndets = 3;
      obj.detids[0] = 1;
      obj.detids[1] = 2;
      obj.detids[2] = 3;

      raw_detection_list.detections[0].processed.vcs_az = 0.0F;
      det_props[0].range_rate_compensated = obj.speed - 1.0F;
      raw_detection_list.detections[1].processed.vcs_az = F360_DEG2RAD(0.0F);
      det_props[1].range_rate_compensated = obj.speed;
      raw_detection_list.detections[2].processed.vcs_az = F360_DEG2RAD(0.0F);
      det_props[2].range_rate_compensated = obj.speed - 1.0F;
   }
   
};

/** \purpose  
 * Purpose is to verify that no detections are flagged as outliers when only 1 detection is associated
 * \req
 * NA
 */
TEST(f360_mark_azimuth_range_rate_outliers, Mark_Azimuth_Range_Rate_Outliers__One_Detection_Associated)
{
   /** \precond
    * Set number of associated detections to 1
    */
   obj.ndets = 1;

   /** \action
    * Call Mark_Azimuth_Range_Rate_Outliers()
    */
   Mark_Azimuth_Range_Rate_Outliers(obj, calibs, host_rear_axle, raw_detection_list, det_props);

   /** \result
    * Verify that the one detection is not flagged as a range rate outlier
    */
   CHECK_FALSE(det_props[0].f_azimuth_rdot_outlier);
}

/** \purpose
 * Purpose is to verify that no detections are flagged as outliers when object is not flagged as moving
 * \req
 * NA
 */
TEST(f360_mark_azimuth_range_rate_outliers, Mark_Azimuth_Range_Rate_Outliers__Not_Moving_Object)
{
   /** \precond
    * Set object moving flag to false
    */
   obj.f_moving = false;

   /** \action
    * Call Mark_Azimuth_Range_Rate_Outliers()
    */
   Mark_Azimuth_Range_Rate_Outliers(obj, calibs, host_rear_axle, raw_detection_list, det_props);

   /** \result
    * Verify that no detections are flagged as a range rate outlier
    */
   CHECK_FALSE(det_props[0].f_azimuth_rdot_outlier);
   CHECK_FALSE(det_props[1].f_azimuth_rdot_outlier);
   CHECK_FALSE(det_props[2].f_azimuth_rdot_outlier);
}

/** \purpose
 * Purpose is to verify that no detections are flagged when object is outside zone of interest
 * \req
 * NA
 */
TEST(f360_mark_azimuth_range_rate_outliers, Mark_Azimuth_Range_Rate_Outliers__Object_Too_Far_Away)
{
   /** \precond
    * Set position of object outside zone of interest
    */
   obj.vcs_position.x = F360_Sqrtf(calibs.k_az_rdot_max_sq_dist) + 5.0F;
   Point center = {F360_Sqrtf(calibs.k_az_rdot_max_sq_dist) + 5.0F,0.0F};
   obj.bbox.Set_Center(center);

   /** \action
    * Call Mark_Azimuth_Range_Rate_Outliers()
    */
   Mark_Azimuth_Range_Rate_Outliers(obj, calibs, host_rear_axle, raw_detection_list, det_props);

   /** \result
    * Verify that the no detections are not flagged as a range rate outlier
    */
   CHECK_FALSE(det_props[0].f_azimuth_rdot_outlier);
   CHECK_FALSE(det_props[1].f_azimuth_rdot_outlier);
   CHECK_FALSE(det_props[2].f_azimuth_rdot_outlier);
}

/** \purpose
 * Purpose is to verify that correct detections are flagged as outliers when second detection
 * fits best to velocity profile
 * \req
 * NA
 */
TEST(f360_mark_azimuth_range_rate_outliers, Mark_Azimuth_Range_Rate_Outliers__Second_Detection_Is_Valid)
{
   /** \precond
    * All three detections have the same vcs aligned azimuth
    * Detection 1 and 3 deviates from the predicted velocity profile above calibration threshold
    */

   /** \action
    * Call Mark_Azimuth_Range_Rate_Outliers()
    */
   Mark_Azimuth_Range_Rate_Outliers(obj, calibs, host_rear_axle, raw_detection_list, det_props);

   /** \result
    * Verify that the second detections is not flagged as a range rate outlier
    * while the other two are
    */
   CHECK_TRUE(det_props[0].f_azimuth_rdot_outlier);
   CHECK_FALSE(det_props[1].f_azimuth_rdot_outlier);
   CHECK_TRUE(det_props[2].f_azimuth_rdot_outlier);
}

/** \purpose
 * Purpose is to verify that correct detections are flagged as outliers when first detection
 * is too far away from the other two in azimuth
 * \req
 * NA
 */
TEST(f360_mark_azimuth_range_rate_outliers, Mark_Azimuth_Range_Rate_Outliers__First_Detection_Too_Far_Away_In_Azimuth)
{
   /** \precond
    * Set first detection azimuth far away from the other two
    */
   raw_detection_list.detections[0].processed.vcs_az = F360_DEG2RAD(-1.0F) - calibs.k_az_rdot_max_az_diff;

   /** \action
    * Call Mark_Azimuth_Range_Rate_Outliers()
    */
   Mark_Azimuth_Range_Rate_Outliers(obj, calibs, host_rear_axle, raw_detection_list, det_props);

   /** \result
    * Verify that the first and second detection is not flagged as a range rate outlier
    */
   CHECK_FALSE(det_props[0].f_azimuth_rdot_outlier);
   CHECK_FALSE(det_props[1].f_azimuth_rdot_outlier);
   CHECK_TRUE(det_props[2].f_azimuth_rdot_outlier);
}

/** \purpose
 * Purpose is to verify that no detections are flagged as outliers when third detection
 * fits best to velocity profile. Detection range rates are ordered such that the detection
 * that deviates the most from the velocity profile is the first detection but still below
 * calibration threshold. The second deviates less than the first.
 * \req
 * NA
 */
TEST(f360_mark_azimuth_range_rate_outliers, Mark_Azimuth_Range_Rate_Outliers__First_Detection_Fits_Best)
{
   /** \precond
    * Arrange range rates so that third detection fits best
    */
   det_props[0].range_rate_compensated = obj.speed + calibs.k_az_rdot_min_rdot_diff - 0.05;
   det_props[1].range_rate_compensated = obj.speed + calibs.k_az_rdot_min_rdot_diff - 0.1F;
   det_props[2].range_rate_compensated = obj.speed;

   /** \action
    * Call Mark_Azimuth_Range_Rate_Outliers()
    */
   Mark_Azimuth_Range_Rate_Outliers(obj, calibs, host_rear_axle, raw_detection_list, det_props);

   /** \result
    * Verify that no detection is flagged as a range rate outlier
    */
   CHECK_FALSE(det_props[0].f_azimuth_rdot_outlier);
   CHECK_FALSE(det_props[1].f_azimuth_rdot_outlier);
   CHECK_FALSE(det_props[2].f_azimuth_rdot_outlier);
}

/** @}*/
