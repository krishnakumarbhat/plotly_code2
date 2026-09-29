/** \file
 * This file contains unit tests for content of f360_object_based_radar_phenomena.cpp file
 */

#include "f360_object_based_radar_phenomena.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena__moved_from_angle_jump_detector
 *  @{
 */

 /** \brief
  * Test group for testing Check_Dets_Against_Radar_Phenomena(). Moved from angle jump detector.
  */
TEST_GROUP(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena__moved_from_angle_jump_detector)
{
   const float32_t floating_threshold = 0.0001F;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibs = {};
   F360_Tracker_Info_T tracker_info = {};

   /** \setup
   *
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      calibs.rp_max_object_lateral_distance = 7.0F;
      calibs.rp_max_abs_pointing_disagreement = F360_DEG2RAD(10.0F);
      calibs.rp_object_max_longitudinal_margin = 2.0F;

      for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
      {
         sensors[idx].variable.is_valid = false;
         sensors[idx].constant.mounting_location = F360_MOUNTING_LOCATION_UNKNOWN;
      }

      // Front right
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -0.25F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.9F;
      sensors[0].variable.is_valid = true;

      // Front left
      sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -0.25F;
      sensors[1].constant.mounting_position.vcs_position.lateral = -0.9F;
      sensors[1].variable.is_valid = true;

      // Rear right
      sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -3.9F;
      sensors[2].constant.mounting_position.vcs_position.lateral = 0.9F;
      sensors[2].variable.is_valid = true;

      // Rear left
      sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -3.9F;
      sensors[3].constant.mounting_position.vcs_position.lateral = -0.9F;
      sensors[3].variable.is_valid = true;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
   }
};

/** \purpose
 * Check_Dets_Against_Radar_Phenomena should flag one det as angle jump
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena__moved_from_angle_jump_detector, one_det_flaged_as_angle_jump)
{
   /** \precond
    * Set input in order to set only one detection
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].f_moving = true;

   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position = {0.0F, 4.0F};
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_velocity.longitudinal = 10.0F;
   object_tracks[0].vcs_velocity.lateral = 0.0F;
   object_tracks[0].vcs_heading = Angle{ F360_Atan2f(object_tracks[0].vcs_velocity.lateral, object_tracks[0].vcs_velocity.longitudinal)};
   object_tracks[0].hdg_ptng_disagmt = 0.0F;
   object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
   object_tracks[0].bbox.Set_Length(6.0F);
   object_tracks[0].bbox.Set_Width(3.0F);

   object_tracks[0].confidenceLevel = calibs.rp_min_confidence_level + floating_threshold;

   sensors[0].variable.vcs_velocity.longitudinal = 10.0F;
   sensors[0].variable.vcs_velocity.lateral = 0.0F;
   sensors[0].constant.polarity = 1.0F;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
   sensors[0].constant.v_wrapping[0] = 30.0F;
   sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
   sensors[0].variable.vacs_boresight_az_estimated = 0.0F;

   raw_detection_list.number_of_valid_detections = 2;

   raw_detection_list.detections[0].raw.sensor_id = 1;
   raw_detection_list.detections[0].raw.confid_azimuth = 1;
   raw_detection_list.detections[0].raw.range = 4.0F;
   raw_detection_list.detections[0].raw.azimuth = F360_DEG2RAD(60.0F);
   raw_detection_list.detections[0].raw.range_rate = 2.0F;

   raw_detection_list.detections[1].raw.sensor_id = 1;
   raw_detection_list.detections[1].raw.confid_azimuth = 1;
   raw_detection_list.detections[1].raw.range = 4.0F;
   raw_detection_list.detections[1].raw.azimuth = F360_DEG2RAD(60.0F);
   raw_detection_list.detections[1].raw.range_rate = 0.4F;

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, object_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Flaged one detection as angle jump
    */
   CHECK_FALSE(detection_props[0].f_object_based_angle_jump);
   CHECK_TRUE(detection_props[1].f_object_based_angle_jump);
}

/** \purpose
 * Check_Dets_Against_Radar_Phenomena should NOT flag any det as angle jump due to lack of valid object reference
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena__moved_from_angle_jump_detector, no_det_flagged_due_to_no_reference_valid_object)
{
   /** \precond
    * Set input in order to set only one detection
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].f_moving = true;
   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position = {0.0F, 40.0F};
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_velocity.longitudinal = 10.0F;
   object_tracks[0].vcs_velocity.lateral = 0.0F;
   object_tracks[0].vcs_heading = Angle{ F360_Atan2f(object_tracks[0].vcs_velocity.lateral, object_tracks[0].vcs_velocity.longitudinal) };
   object_tracks[0].hdg_ptng_disagmt = 0.0F;
   object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
   object_tracks[0].bbox.Set_Length(6.0F);
   object_tracks[0].bbox.Set_Width(3.0F);

   sensors[0].variable.vcs_velocity.longitudinal = 10.0F;
   sensors[0].variable.vcs_velocity.lateral = 0.0F;

   raw_detection_list.number_of_valid_detections = 2;

   raw_detection_list.detections[0].raw.sensor_id = 1;
   raw_detection_list.detections[0].raw.confid_azimuth = 1;
   raw_detection_list.detections[0].raw.range = 4.0F;
   raw_detection_list.detections[0].processed.vcs_az = F360_DEG2RAD(60.0F);
   detection_props[0].vcs_position.y = raw_detection_list.detections[0].raw.range * F360_Sinf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].vcs_position.x = raw_detection_list.detections[0].raw.range * F360_Cosf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].range_rate_compensated = 9.5F;

   raw_detection_list.detections[1].raw.sensor_id = 1;
   raw_detection_list.detections[1].raw.confid_azimuth = 1;
   raw_detection_list.detections[1].raw.range = 4.0F;
   raw_detection_list.detections[1].processed.vcs_az = F360_DEG2RAD(10.0F);
   detection_props[1].vcs_position.y = raw_detection_list.detections[1].raw.range * F360_Sinf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].vcs_position.x = raw_detection_list.detections[1].raw.range * F360_Cosf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].range_rate_compensated = 4.5F;

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, object_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * No single angle jump
    */
   CHECK_FALSE(detection_props[0].f_object_based_angle_jump);
   CHECK_FALSE(detection_props[1].f_object_based_angle_jump);
}


/** \purpose
 * Check_Dets_Against_Radar_Phenomena should not mark any det as angle jump if each sensor is invalid
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena__moved_from_angle_jump_detector, no_any_angle_jumps_due_to_invalid_sensors)
{
   /** \precond
    * Set input in order to set only one detection
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   sensors[0].variable.is_valid = false;
   sensors[1].variable.is_valid = false;
   sensors[2].variable.is_valid = false;
   sensors[3].variable.is_valid = false;

   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position = {0.0F, 4.0F};
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_velocity.longitudinal = 10.0F;
   object_tracks[0].vcs_velocity.lateral = 0.0F;
   object_tracks[0].vcs_heading = Angle{ F360_Atan2f(object_tracks[0].vcs_velocity.lateral , object_tracks[0].vcs_velocity.longitudinal) };
   object_tracks[0].hdg_ptng_disagmt = 0.0F;
   object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
   object_tracks[0].bbox.Set_Length(12.0F);
   object_tracks[0].bbox.Set_Width(3.0F);
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].f_moving = true;

   sensors[0].variable.vcs_velocity.longitudinal = 10.0F;
   sensors[0].variable.vcs_velocity.lateral = 0.0F;

   raw_detection_list.number_of_valid_detections = 2;

   raw_detection_list.detections[0].raw.sensor_id = 1;
   raw_detection_list.detections[0].raw.confid_azimuth = 1;
   raw_detection_list.detections[0].raw.range = 4.0F;
   raw_detection_list.detections[0].processed.vcs_az = F360_DEG2RAD(60.0F);
   detection_props[0].vcs_position.y = raw_detection_list.detections[0].raw.range * F360_Sinf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].vcs_position.x = raw_detection_list.detections[0].raw.range * F360_Cosf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].range_rate_compensated = 9.5F;

   raw_detection_list.detections[1].raw.sensor_id = 1;
   raw_detection_list.detections[1].raw.confid_azimuth = 1;
   raw_detection_list.detections[1].raw.range = 4.0F;
   raw_detection_list.detections[1].processed.vcs_az = F360_DEG2RAD(10.0F);
   detection_props[1].vcs_position.y = raw_detection_list.detections[1].raw.range * F360_Sinf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].vcs_position.x = raw_detection_list.detections[1].raw.range * F360_Cosf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].range_rate_compensated = 4.5F;

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, object_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * No single angle jump
    */
   CHECK_FALSE(detection_props[0].f_object_based_angle_jump);
   CHECK_FALSE(detection_props[1].f_object_based_angle_jump);
}

/** \purpose
 * Check_Dets_Against_Radar_Phenomena should not mark any det if corresponding sensor is invalid
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena__moved_from_angle_jump_detector, no_any_angle_jumps_due_to_already_marked_as_angle_jump)
{
   /** \precond
    * Set input in order to set only one detection
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position = {0.0F, 4.0F};
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_velocity.longitudinal = 10.0F;
   object_tracks[0].vcs_velocity.lateral = 0.0F;
   object_tracks[0].vcs_heading = Angle{ F360_Atan2f(object_tracks[0].vcs_velocity.lateral, object_tracks[0].vcs_velocity.longitudinal) };
   object_tracks[0].hdg_ptng_disagmt = 0.0F;
   object_tracks[0].bbox.Set_Orientation(object_tracks[0].vcs_heading + object_tracks[0].hdg_ptng_disagmt);
   object_tracks[0].bbox.Set_Length(6.0F);
   object_tracks[0].bbox.Set_Width(3.0F);
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].f_moving = true;

   sensors[0].variable.vcs_velocity.longitudinal = 10.0F;
   sensors[0].variable.vcs_velocity.lateral = 0.0F;

   raw_detection_list.number_of_valid_detections = 2;

   raw_detection_list.detections[0].raw.sensor_id = 1;
   raw_detection_list.detections[0].raw.confid_azimuth = 1;
   raw_detection_list.detections[0].raw.range = 4.0F;
   raw_detection_list.detections[0].processed.vcs_az = F360_DEG2RAD(60.0F);
   detection_props[0].vcs_position.y = raw_detection_list.detections[0].raw.range * F360_Sinf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].vcs_position.x = raw_detection_list.detections[0].raw.range * F360_Cosf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].range_rate_compensated = 9.5F;

   raw_detection_list.detections[1].raw.sensor_id = 1;
   raw_detection_list.detections[1].raw.confid_azimuth = 1;
   raw_detection_list.detections[1].raw.range = 4.0F;
   raw_detection_list.detections[1].processed.vcs_az = F360_DEG2RAD(10.0F);
   detection_props[1].vcs_position.y = raw_detection_list.detections[1].raw.range * F360_Sinf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].vcs_position.x = raw_detection_list.detections[1].raw.range * F360_Cosf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].range_rate_compensated = 4.5F;
   detection_props[1].f_object_based_angle_jump = true;
   detection_props[1].f_ok_to_use = false;

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, object_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Properly NOT marked first and marked second detection
    * Detection which was already marked as angle jump should have f_ok_to_use not changed (as set before run tested function i.e. true
    */
   CHECK_FALSE(detection_props[0].f_object_based_angle_jump);
   CHECK_TRUE(detection_props[1].f_object_based_angle_jump);
}

/** \purpose
 * Check_Dets_Against_Radar_Phenomena should not mark any det if corresponding sensor is invalid
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena__moved_from_angle_jump_detector, no_any_angle_jumps_due_to_invalid_corresponding_sensor)
{
   /** \precond
    * Set input in order to set only one detection
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
   object_tracks[0].vcs_position = {0.0F, 4.0F};
   object_tracks[0].bbox.Set_Center(object_tracks[0].vcs_position);
   object_tracks[0].vcs_velocity.longitudinal = 10.0F;
   object_tracks[0].vcs_velocity.lateral = 0.0F;
   object_tracks[0].vcs_heading = Angle{ F360_Atan2f(object_tracks[0].vcs_velocity.lateral, object_tracks[0].vcs_velocity.longitudinal) };
   object_tracks[0].bbox.Set_Length(6.0F);
   object_tracks[0].bbox.Set_Width(3.0F);
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].f_moving = true;

   sensors[0].variable.is_valid = false;

   sensors[0].variable.vcs_velocity.longitudinal = 10.0F;
   sensors[0].variable.vcs_velocity.lateral = 0.0F;

   raw_detection_list.number_of_valid_detections = 2;

   raw_detection_list.detections[0].raw.sensor_id = 1;
   raw_detection_list.detections[0].raw.confid_azimuth = 1;
   raw_detection_list.detections[0].raw.range = 4.0F;
   raw_detection_list.detections[0].processed.vcs_az = F360_DEG2RAD(60.0F);
   detection_props[0].vcs_position.y = raw_detection_list.detections[0].raw.range * F360_Sinf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].vcs_position.x = raw_detection_list.detections[0].raw.range * F360_Cosf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].range_rate_compensated = 9.5F;

   raw_detection_list.detections[1].raw.sensor_id = 1;
   raw_detection_list.detections[1].raw.confid_azimuth = 1;
   raw_detection_list.detections[1].raw.range = 4.0F;
   raw_detection_list.detections[1].processed.vcs_az = F360_DEG2RAD(10.0F);
   detection_props[1].vcs_position.x = raw_detection_list.detections[1].raw.range * F360_Cosf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].vcs_position.y = raw_detection_list.detections[1].raw.range * F360_Sinf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].range_rate_compensated = 4.5F;

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, object_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * No single angle jump
    */
   CHECK_FALSE(detection_props[0].f_object_based_angle_jump);
   CHECK_FALSE(detection_props[1].f_object_based_angle_jump);
}
/** @}*/



/** \defgroup  f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena
 *  @{
 */

 /** \brief
  * Test group for testing Check_Dets_Against_Radar_Phenomena().
  */
TEST_GROUP(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena)
{
   const float tolerance = 0.0001F;

   F360_Object_Track_T obj_tracks[NUMBER_OF_OBJECT_TRACKS]{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Calibrations_T calibs{};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Tracker_Info_T tracker_info{};

   F360_Radar_Sensor_T &front_right_sensor_cal = sensors[0];
   F360_Radar_Sensor_T &front_right_sensor = sensors[0];
   F360_Radar_Sensor_T &rear_right_sensor_cal = sensors[1];
   F360_Radar_Sensor_T &rear_right_sensor = sensors[1];
   F360_Radar_Sensor_T &front_left_sensor_cal = sensors[2];
   F360_Radar_Sensor_T &front_left_sensor = sensors[2];
   F360_Radar_Sensor_T &rear_left_sensor_cal = sensors[3];
   F360_Radar_Sensor_T &rear_left_sensor = sensors[3];

   F360_Radar_Sensor_T &fl7_front_sensor = sensors[4];

   rspp_variant_A::RSPP_Detection_T &multibounce_det_raw = raw_detection_list.detections[0];
   F360_Detection_Props_T &multibounce_det_prop = detection_props[0];
   rspp_variant_A::RSPP_Detection_T &angle_jump_det_raw = raw_detection_list.detections[1];
   F360_Detection_Props_T &angle_jump_det_prop = detection_props[1];

   rspp_variant_A::RSPP_Detection_T &flr7_angle_jump_det_raw = raw_detection_list.detections[2];
   F360_Detection_Props_T &flr7_angle_jump_det_prop = detection_props[2];

   F360_Object_Track_T &relevant_object = obj_tracks[0];
   F360_Object_Track_T &fl7_reference_object = obj_tracks[1];

   /** \setup
    * Setup all needed data like radars, detections and object
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      //Setup radars
      front_right_sensor_cal.variable.is_valid = true;
      front_right_sensor_cal.constant.mounting_position.vcs_position = { -0.4F, 0.9F, 0.0F};
      front_right_sensor_cal.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      front_right_sensor.variable.vcs_velocity = { 20.0F, 0.0F };
     
      rear_right_sensor_cal.variable.is_valid = true;
      rear_right_sensor_cal.constant.mounting_position.vcs_position = { -4.4F, 0.9F, 0.0F };
      rear_right_sensor_cal.constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      rear_right_sensor.variable.vcs_velocity = { 20.0F, 0.0F };

      rear_right_sensor_cal.constant.polarity = 1.0F;
      rear_right_sensor_cal.constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
      rear_right_sensor_cal.constant.v_wrapping[0] = 30.0F;
      rear_right_sensor_cal.variable.look_id = F360_DET_LOOK_ID_0;

      fl7_front_sensor.variable.is_valid = true;
      fl7_front_sensor.constant.mounting_position.vcs_position = { 0.004F, -0.3F, 0.84F };
      fl7_front_sensor.constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
      fl7_front_sensor.variable.vcs_velocity = { 21.48F, -0.02F };
      fl7_front_sensor.constant.polarity = 1.0F;
      fl7_front_sensor.constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
      fl7_front_sensor.constant.v_wrapping[0] = 28.17F;
      fl7_front_sensor.variable.look_id = F360_DET_LOOK_ID_0;

      //Setup object
      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;

      relevant_object.reference_point = F360_REFERENCE_POINT_CENTER;
      relevant_object.vcs_position = {-2.0F, 5.0F};
      relevant_object.bbox.Set_Center(relevant_object.vcs_position);
      relevant_object.vcs_velocity = { 22.0F, 0.0F };    // moves faster than host/sensors
      relevant_object.vcs_heading = Angle{ F360_Atan2f(relevant_object.vcs_velocity.lateral, relevant_object.vcs_velocity.longitudinal) };
      relevant_object.hdg_ptng_disagmt = 0.0F;
      relevant_object.bbox.Set_Orientation(relevant_object.vcs_heading + relevant_object.hdg_ptng_disagmt);
      relevant_object.bbox.Set_Length(10.0F);
      relevant_object.bbox.Set_Width(4.0F);
      relevant_object.status = F360_OBJECT_STATUS_UPDATED;
      relevant_object.f_moving = true;
      relevant_object.confidenceLevel = calibs.rp_min_confidence_level + tolerance;

      fl7_reference_object.reference_point = F360_REFERENCE_POINT_LEFT;
      fl7_reference_object.vcs_position = {6.9487F, 2.592F};
      fl7_reference_object.vcs_velocity = { 17.097F, 0.199F };
      fl7_reference_object.vcs_heading = Angle(-0.01F);
      fl7_reference_object.hdg_ptng_disagmt = 0.0F;
      fl7_reference_object.bbox.Set_Orientation(fl7_reference_object.vcs_heading + fl7_reference_object.hdg_ptng_disagmt);
      fl7_reference_object.bbox.Set_Length(16.55F);
      fl7_reference_object.bbox.Set_Width(2.49F);
      fl7_reference_object.bbox.Set_Center(Point(6.96F, 3.84F));
      fl7_reference_object.status = F360_OBJECT_STATUS_UPDATED;
      fl7_reference_object.f_moving = true;
      fl7_reference_object.confidenceLevel = calibs.rp_min_confidence_level + tolerance;

      //Setup two multibounces detections
      raw_detection_list.number_of_valid_detections = 3U;

      multibounce_det_raw.raw.sensor_id = 1;  // belongs to front right sensor
      multibounce_det_raw.raw.range = 7.872F;
      multibounce_det_raw.raw.range_rate = 0.863F * 3.0F;
      multibounce_det_raw.processed.vcs_az = F360_DEG2RAD(60.326F);
      multibounce_det_raw.processed.sin_vcs_az = std::sin(multibounce_det_raw.processed.vcs_az);
      multibounce_det_raw.processed.cos_vcs_az = std::cos(multibounce_det_raw.processed.vcs_az);
      multibounce_det_prop.vcs_position = { 3.0F, 8.0F };
      multibounce_det_prop.f_ok_to_use = true;

      angle_jump_det_raw.raw.sensor_id = 2;  // belongs to rear right sensor
      angle_jump_det_raw.raw.range = 5.35F;
      angle_jump_det_raw.raw.range_rate = -0.304;
      angle_jump_det_raw.raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;
      angle_jump_det_raw.raw.azimuth = F360_DEG2RAD(40.0F);
      angle_jump_det_prop.vcs_position = {-5.3F, 0.75F};
      angle_jump_det_prop.f_ok_to_use = true;

      flr7_angle_jump_det_raw.raw.sensor_id = 5;  // belongs to front center sensor
      flr7_angle_jump_det_raw.raw.range = 6.62F;
      flr7_angle_jump_det_raw.raw.range_rate = -3.975;
      flr7_angle_jump_det_raw.raw.azimuth = 0.21567;
      flr7_angle_jump_det_prop.vcs_position = { 6.4856F, 1.0456F };
      flr7_angle_jump_det_prop.f_ok_to_use = true;
   }
};

/** \purpose
 * Check if function mark multibounce detection
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena, multibounce_marking)
{
   /** \precond
    * Same as setup
    */

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, obj_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Detection marked as multibounce
    */
   CHECK_FALSE(multibounce_det_prop.f_ok_to_use);
   CHECK_TRUE(multibounce_det_prop.f_double_bounce);
}

/** \purpose
 * Check if function mark FLR7 angle jump detection
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena, flr7_angle_jump_marking)
{
   /** \precond
    * Same as setup
    */

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, obj_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * FLR7 Detection marked as an angle jump.
    */
   CHECK_FALSE(flr7_angle_jump_det_prop.f_ok_to_use);
   CHECK_TRUE(flr7_angle_jump_det_prop.f_object_based_angle_jump);
}

/** \purpose
 * Check if function mark angle jump detection
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena, angle_jump_marking)
{
   /** \precond
    * Same as setup
    */

    /** \action
     * Call Check_Dets_Against_Radar_Phenomena()
     */
   Check_Dets_Against_Radar_Phenomena(tracker_info, obj_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Detection marked as angle jump
    */
   CHECK_FALSE(angle_jump_det_prop.f_ok_to_use);
   CHECK_TRUE(angle_jump_det_prop.f_object_based_angle_jump);
}

/** \purpose
 * Check if function does not mark detection if there is no object
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena, not_marking_due_to_no_object)
{
   /** \precond
    * Same as setup with below change:
    * set num active objects to zero
    */
   tracker_info.num_active_objs = 0;

    /** \action
     * Call Check_Dets_Against_Radar_Phenomena()
     */
   Check_Dets_Against_Radar_Phenomena(tracker_info, obj_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Not marked detections
    */
   CHECK_TRUE(multibounce_det_prop.f_ok_to_use);
   CHECK_FALSE(multibounce_det_prop.f_double_bounce);
   CHECK_TRUE(angle_jump_det_prop.f_ok_to_use);
   CHECK_FALSE(angle_jump_det_prop.f_object_based_angle_jump);
}

/** \purpose
 * Check if function does not mark detection if object cannot be a reference (not moving)
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena, not_marking_due_to_object_cannot_be_reference)
{
   /** \precond
    * Same as setup with below change:
    * set object to be not moving
    */
   relevant_object.f_moving = false;

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, obj_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Not marked detections
    */
   CHECK_TRUE(multibounce_det_prop.f_ok_to_use);
   CHECK_FALSE(multibounce_det_prop.f_double_bounce);
   CHECK_TRUE(angle_jump_det_prop.f_ok_to_use);
   CHECK_FALSE(angle_jump_det_prop.f_object_based_angle_jump);
}

/** \purpose
 * Check if function does not mark detection if no valid sensors
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena, not_marking_due_to_no_valid_sensors)
{
   /** \precond
    * Same as setup with below change:
    * set all sensors to invalid
    */
   for (F360_Radar_Sensor_T &sen_cal : sensors)
   {
      sen_cal.variable.is_valid = false;
   }

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, obj_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Not marked detections
    */
   CHECK_TRUE(multibounce_det_prop.f_ok_to_use);
   CHECK_FALSE(multibounce_det_prop.f_double_bounce);
   CHECK_TRUE(angle_jump_det_prop.f_ok_to_use);
   CHECK_FALSE(angle_jump_det_prop.f_object_based_angle_jump);
}

/** \purpose
 * Check calibrations correctness
 * \req
 * NA
 */
TEST(f360_object_based_radar_phenomena__Check_Dets_Against_Radar_Phenomena, check_calibrations)
{
   /** \precond
    * Same as setup (initialized calibrations)
    */

   /** \action
    * Call Check_Dets_Against_Radar_Phenomena()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, obj_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Calibrations met conditions
    */
   DOUBLES_EQUAL(13.0F, calibs.rp_max_object_lateral_distance, tolerance);

   DOUBLES_EQUAL(0.174533F, calibs.rp_max_abs_pointing_disagreement, tolerance);
   DOUBLES_EQUAL(0.33F, calibs.rp_min_confidence_level, tolerance);
   CHECK_TRUE_TEXT(calibs.rp_max_abs_pointing_disagreement <= 0.20944F, "Disagreement should be small enough due to some assumptions. Check Create_Area_Of_Correct_Detections() function for details");
   CHECK_TRUE(0.0F <= calibs.rp_max_abs_pointing_disagreement);

   DOUBLES_EQUAL(2.0F, calibs.rp_object_max_longitudinal_margin, tolerance);
}

/** @}*/