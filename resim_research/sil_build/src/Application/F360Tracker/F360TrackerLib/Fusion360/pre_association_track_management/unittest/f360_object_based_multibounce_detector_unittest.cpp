/** \file
 * This file contains unit tests for content of f360_object_based_multibounce_detector.cpp file
 */

#include "f360_object_based_multibounce_detector.h"
#include <CppUTest/TestHarness.h>
#include <limits>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_based_multibounce_detector
 *  @{
 */

/** \brief
 * Test functionality of Check_Dets_Against_Multibounces() if detections are correctly
 * marked (or not) as multibounces.
 */
TEST_GROUP(f360_object_based_multibounce_detector)
{	
   F360_Object_Track_T obj_track{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   bool relevant_sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Calibrations_T calibs{};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS]{};
   
   F360_Radar_Sensor_T &front_right_sensor_cal = sensors[0];
   F360_Radar_Sensor_T &front_right_sensor = sensors[0];
   F360_Radar_Sensor_T &rear_right_sensor_cal = sensors[1];
   F360_Radar_Sensor_T &rear_right_sensor = sensors[1];
   F360_Radar_Sensor_T &front_left_sensor_cal = sensors[2];
   F360_Radar_Sensor_T &front_left_sensor = sensors[2];
   F360_Radar_Sensor_T &rear_left_sensor_cal = sensors[3];
   F360_Radar_Sensor_T &rear_left_sensor = sensors[3];

   const float tolerance = 0.0001F;

   rspp_variant_A::RSPP_Detection_T &fr_mb_det_raw = raw_detection_list.detections[0];
   F360_Detection_Props_T &fr_mb_det_prop = detection_props[0];
   rspp_variant_A::RSPP_Detection_T &rr_mb_det_raw = raw_detection_list.detections[1];
   F360_Detection_Props_T &rr_mb_det_prop = detection_props[1];

   /** \setup
    * Setup all needed data like radars, detections and object
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      //Setup radars
      front_right_sensor_cal.constant.mounting_position.vcs_position = { -0.4F, 0.9F, 0.0F };
      front_right_sensor.variable.vcs_velocity = { 20.0F, 0.0F };
      rear_right_sensor_cal.constant.mounting_position.vcs_position = { -4.4F, 0.9F, 0.0F };
      rear_right_sensor.variable.vcs_velocity = front_right_sensor.variable.vcs_velocity;
      front_left_sensor_cal.constant.mounting_position.vcs_position = { -0.4F, -0.9F, 0.0F };
      front_left_sensor.variable.vcs_velocity = { 20.0F, 0.0F };
      rear_left_sensor_cal.constant.mounting_position.vcs_position = { -4.4F, -0.9F, 0.0F };
      rear_left_sensor.variable.vcs_velocity = front_right_sensor.variable.vcs_velocity;

      relevant_sensors[0] = true;
      relevant_sensors[1] = true;
      relevant_sensors[2] = true;
      relevant_sensors[3] = true;

      //Setup object
      obj_track.vcs_velocity = { 22.0F, 0.0F };    // moves faster than host/sensors
      obj_track.reference_point = F360_REFERENCE_POINT_CENTER;
      obj_track.vcs_position = {-2.0F, 5.0F};
      obj_track.bbox.Set_Center(obj_track.vcs_position);
      obj_track.bbox.Set_Length(10.0F);
      obj_track.bbox.Set_Width(4.0F);

      //Setup two multibounces detections
      raw_detection_list.number_of_valid_detections = 2U;
      
      fr_mb_det_raw.raw.sensor_id = 1;  // belongs to front right sensor
      fr_mb_det_raw.raw.range = 7.87F;
      fr_mb_det_raw.raw.range_rate = 0.863F * 3.0F;
      fr_mb_det_raw.processed.sin_vcs_az = std::sin(F360_DEG2RAD(64.411F));
      fr_mb_det_raw.processed.cos_vcs_az = std::cos(F360_DEG2RAD(64.411F));
      fr_mb_det_prop.vcs_position = { 3.0F, 8.0F };
      fr_mb_det_prop.f_ok_to_use = true;

      rr_mb_det_raw.raw.sensor_id = 2;  // belongs to rear right sensor
      rr_mb_det_raw.raw.range = 5.64F;
      rr_mb_det_raw.raw.range_rate = 0.852F * 2.0F;
      rr_mb_det_raw.processed.sin_vcs_az = std::sin(F360_DEG2RAD(64.799F));
      rr_mb_det_raw.processed.cos_vcs_az = std::cos(F360_DEG2RAD(64.799F));
      rr_mb_det_prop.vcs_position = { -4.0F, 6.0F };
      rr_mb_det_prop.f_ok_to_use = true;
   }
};

/** \purpose  
 * Check if detections are marked as multibounces if all conditions are met.
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, both_dets_marked_as_multibounce)
{
   /** \precond
    * Same as setup.
    */
	
   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * Both dets marked as multi bounce and should be not ok to use
    */	
   CHECK_TRUE(fr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to not relevant sensor.
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_not_relevant_sensor)
{
   /** \precond
    * Set first sensor to not relevant
    */
   relevant_sensors[0] = false;

    /** \action
     * Call Check_Dets_Against_Multibounces().
     */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det in NOT marked as multibounce, second is
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to being already multibounce.
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_being_already_multibounce)
{
   /** \precond
    * Set first det to already multibounce
    */
   fr_mb_det_prop.f_double_bounce = true;

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det should be ok to use (not touched by this algorithm), second is fully marked as multibounce
    */
   CHECK_TRUE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to being to far away.
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_being_to_far_away)
{
   /** \precond
    * Set first det range just above threshold
    */
   fr_mb_det_raw.raw.range = calibs.mb_max_det_range + tolerance;

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det in NOT marked as multibounce, second is
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to being in restircted area
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_being_in_restricted_area)
{
   /** \precond
    * Set first det position to be in restricted area
    */
   fr_mb_det_prop.vcs_position = { 0.0F, 3.1F };

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det in NOT marked as multibounce, second is
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to lack of reference point
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_lack_of_rference_point)
{
   /** \precond
    * Set first det position in the way that there will be no intersection
    */
   fr_mb_det_prop.vcs_position = { 0.0F, 1.0F };

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det in NOT marked as multibounce, second is
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to low factor
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_to_low_factor)
{
   /** \precond
    * Set first det range in order to get low multibounce factor
    */
   fr_mb_det_raw.raw.range = calibs.mb_max_det_range - tolerance;

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det in NOT marked as multibounce, second is
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to high factor
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_to_high_factor)
{
   /** \precond
    * Set first det range in order to get high multibounce factor
    */
   fr_mb_det_raw.raw.range = 2.75F;

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det in NOT marked as multibounce, second is
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to not being in restricted area after range correction
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_not_being_in_area_after_correction)
{
   /** \precond
    * 
    */
   fr_mb_det_raw.raw.range = 6.75F;

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det in NOT marked as multibounce, second is
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if one of the detection is NOT marked as multibounce due to to high range rate diff
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, one_det_not_marked_due_to_to_high_range_rate_diff)
{
   /** \precond
    * Set first det range rate to high value
    */
   fr_mb_det_raw.raw.range_rate = 20.75F;

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * First det in NOT marked as multibounce, second is
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if everything is fine when all items (detections, track) are on the left side
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, left_side_scenario)
{
   /** \precond
    * Switch object and dets to left side
    */
   Point center = {obj_track.bbox.Get_Center().x,-obj_track.bbox.Get_Center().y};
   obj_track.bbox.Set_Center(center);

   fr_mb_det_raw.raw.sensor_id = 3;  // make it front left
   fr_mb_det_raw.processed.sin_vcs_az = std::sin(F360_DEG2RAD(-64.411F));
   fr_mb_det_raw.processed.cos_vcs_az = std::cos(F360_DEG2RAD(-64.411F));
   fr_mb_det_prop.vcs_position = { 3.0F, -8.0F };

   rr_mb_det_raw.raw.sensor_id = 4;  //make it rear left
   rr_mb_det_raw.processed.sin_vcs_az = std::sin(F360_DEG2RAD(-64.799F));
   rr_mb_det_raw.processed.cos_vcs_az = std::cos(F360_DEG2RAD(-64.799F));
   rr_mb_det_prop.vcs_position = { -4.0F, -6.0F };

   /** \action
    * Call Check_Dets_Against_Multibounces().
    */
   Check_Dets_Against_Multibounces(obj_track, raw_detection_list, relevant_sensors, sensors, calibs, detection_props);

   /** \result
    * Both dets marked as multi bounce and should be not ok to use
    */
   CHECK_TRUE(fr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(fr_mb_det_prop.f_ok_to_use);
   CHECK_TRUE(rr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(rr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if calibrations have not been changed
 * \req
 * NA.
 */
TEST(f360_object_based_multibounce_detector, check_calibs)
{
   /** \precond
    * Same as setup i.e. initiated calibrations
    */

   /** \action
    * Nothing.
    */

   /** \result
    * Calibs are set correctly
    */
   CHECK_EQUAL(4U, calibs.mb_max_num_bounces);
   CHECK_TRUE_TEXT(calibs.mb_max_num_bounces < std::numeric_limits<uint16_t>::max(), "mb_max_num_bounces is to high. Check Is_Det_Object_Based_Multibounce() for more details");
   DOUBLES_EQUAL(1.0F, calibs.mb_range_rate_diff_th, tolerance);
   DOUBLES_EQUAL(15.0F, calibs.mb_max_det_range, tolerance);
   DOUBLES_EQUAL(0.5F, calibs.mb_restricted_area_width, tolerance);
}
/** @}*/
