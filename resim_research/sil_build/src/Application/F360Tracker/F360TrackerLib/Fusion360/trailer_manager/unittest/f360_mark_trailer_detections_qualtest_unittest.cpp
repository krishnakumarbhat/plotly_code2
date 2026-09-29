/** \file
   This file contains qualtest for functions in f360_mark_trailer_detections.
*/

#include "f360_mark_object_tracks_next_to_sensors.h"
#include <CppUTest/TestHarness.h>
#include "f360_constants.h"
#include "f360_sensor_type.h"
#include "f360_set_variant.h"
#include "f360_mark_trailer_detections.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"

using namespace f360_variant_A;
/** \defgroup  f360_mark_trailer_detections
 *  @{
 */
/** \brief
*  This test group contains tests for mark_trailer_detections
*  function.
**/
float32_t Get_Det_Scs_Az(
      const F360_Detection_Props_T &det_prop,
      const F360_Radar_Sensor_T& sensor)
   {
      const float32_t lon_scs = det_prop.vcs_position.x - sensor.constant.mounting_position.vcs_position.longitudinal;
      const float32_t lat_scs = det_prop.vcs_position.y - sensor.constant.mounting_position.vcs_position.lateral;
      const float32_t cos_bs_ang = F360_Cosf(sensor.constant.mounting_position.vcs_boresight_azimuth_angle);
      const float32_t sin_bs_ang = F360_Sinf(sensor.constant.mounting_position.vcs_boresight_azimuth_angle);
      const float32_t x = cos_bs_ang * lon_scs + sin_bs_ang * lat_scs;
      const float32_t y = -sin_bs_ang * lon_scs + cos_bs_ang * lat_scs;

      return F360_Atan2f(y, x);
   }
   
TEST_GROUP(f360_mark_trailer_detections)
{
   rspp_variant_A::RSPP_Detection_List_T raw_det_list{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Host_T host = {};
   F360_Calibrations_T calibs = {};
   F360_Trailer_Estimator_Output_T trailer_detector_output = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** \setup
    * Default setup will change f_ok_to_use and f_trailer_related_det after function call
    **/
   TEST_SETUP()
   {
      // Initialize host
      host.dist_rear_axle_to_vcs_m = 3.7F;
      
      // Initialize detections
      const float32_t distance_rear_axle_to_tow_hitch = 1.2F;    // distance between rear axle to to hitch (m)
      calibs.k_trailer_distance_rear_axle_to_tow_hitch = distance_rear_axle_to_tow_hitch;
      
      raw_det_list.number_of_valid_detections = 5U;
      #define LEFT 1
      #define RIGHT 2
      // sensor locations
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -2.14675498;
      sensors[0].constant.mounting_position.vcs_position.lateral = -0.779999971;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -4.59999990;
      sensors[0].constant.polarity = 1;
      sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[1].constant.mounting_position.vcs_boresight_azimuth_angle = 2.14675498;
      sensors[1].constant.mounting_position.vcs_position.lateral = 0.779999971;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -4.59999990;
      sensors[1].constant.polarity = 1;

      det_props[0].f_ok_to_use = true;
      det_props[0].f_trailer_related_det = false;
      det_props[0].vcs_position.x = -(host.dist_rear_axle_to_vcs_m + distance_rear_axle_to_tow_hitch);;
      det_props[0].vcs_position.y = 0.0F;;
      raw_det_list.detections[0].processed.vcs_position_x = det_props[0].vcs_position.x;
      raw_det_list.detections[0].processed.vcs_position_y = det_props[0].vcs_position.y;
      raw_det_list.detections[0].raw.sensor_id = LEFT;

      det_props[1].f_ok_to_use = true;
      det_props[1].f_trailer_related_det = false;
      det_props[1].vcs_position.x = -(host.dist_rear_axle_to_vcs_m + distance_rear_axle_to_tow_hitch) - 1.0F;
      det_props[1].vcs_position.y = 0.0F;
      raw_det_list.detections[1].processed.vcs_position_x = det_props[1].vcs_position.x;
      raw_det_list.detections[1].processed.vcs_position_y = det_props[1].vcs_position.y;
      raw_det_list.detections[1].raw.sensor_id = LEFT;

      det_props[2].f_ok_to_use = true;
      det_props[2].f_trailer_related_det = false;
      det_props[2].vcs_position.x = 10.0F;
      det_props[2].vcs_position.y = 0.0F;
      raw_det_list.detections[2].processed.vcs_position_x = det_props[2].vcs_position.x;
      raw_det_list.detections[2].processed.vcs_position_y = det_props[2].vcs_position.y;
      raw_det_list.detections[2].raw.sensor_id = RIGHT;

      det_props[3].f_ok_to_use = true;
      det_props[3].f_trailer_related_det = false;
      det_props[3].vcs_position.x = -20.0F;
      det_props[3].vcs_position.y = 0.0F;
      raw_det_list.detections[3].processed.vcs_position_x = det_props[3].vcs_position.x;
      raw_det_list.detections[3].processed.vcs_position_y = det_props[3].vcs_position.y;
      raw_det_list.detections[3].raw.sensor_id = LEFT;

      det_props[4].f_ok_to_use = true;
      det_props[4].f_trailer_related_det = false;
      det_props[4].vcs_position.x = -10.0F;
      det_props[4].vcs_position.y = 1.6F;
      raw_det_list.detections[4].processed.vcs_position_x = det_props[4].vcs_position.x;
      raw_det_list.detections[4].processed.vcs_position_y = det_props[4].vcs_position.y;
      raw_det_list.detections[4].raw.sensor_id = LEFT;

      for (uint8_t i = 0U; i < 5; i++)
      {
        raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(det_props[i], sensors[raw_det_list.detections[i].raw.sensor_id - 1]);
      }

      // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
      for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
      {
         raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
         raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
      }

      Sort_Detections_Vcs_Long(raw_det_list);


      trailer_detector_output.trailer_presence[0] = TRAILER_PRESENCE_STATE_DETECTED;
      trailer_detector_output.trailer_width[0] = 2.0F;
      trailer_detector_output.trailer_length[0] = 8.0F;

   }
};

/**
*\purpose
* The purpose of the test is to verify if the detections are classified to be detections on trailer 
* when the trailer condition satified. Among the cases, detection 1 is within the trailer bounding box
* \req   CPR-5451
*/
TEST(f360_mark_trailer_detections, detect_det_on_trailer)
{
   /** \precond
   * Use default settings
   **/

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_useis set to expected value.
   **/
   CHECK_TRUE_TEXT(det_props[0].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_FALSE_TEXT(det_props[0].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_TRUE_TEXT(det_props[1].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_FALSE_TEXT(det_props[1].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[2].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_TRUE_TEXT(det_props[3].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_FALSE_TEXT(det_props[3].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_TRUE_TEXT(det_props[4].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_FALSE_TEXT(det_props[4].f_ok_to_use , "f_ok_to_use not set right");

}

/**
*\purpose
* The purpose of the test is to verify the flag f_trailer_related_det and f_ok_to_use won't
* be set to false when trailer length is 0.
* \req   CPR-5451
*/
TEST(f360_mark_trailer_detections, detect_det_on_trailer_trailer_length_0)
{
   /** \precond
   * Use default settings
   **/
   trailer_detector_output.trailer_length[0] = 0.0F;

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_use is not updated
   **/
   CHECK_FALSE_TEXT(det_props[0].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[1].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[2].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[3].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[4].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[4].f_ok_to_use , "f_ok_to_use not set right");

}

/**
*\purpose
* The purpose of the test is to verify the flag f_trailer_related_det and f_ok_to_use won't
* be updated when trailer not detected.
* \req   CPR-5451
*/
TEST(f360_mark_trailer_detections, detect_det_on_trailer_trailer_not_detected)
{
   /** \precond
   * Use default settings
   **/
   trailer_detector_output.trailer_presence[0] = TRAILER_PRESENCE_STATE_NOT_DETECTED;

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_use is not updated
   **/
   CHECK_FALSE_TEXT(det_props[0].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[1].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[2].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[3].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[4].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[4].f_ok_to_use , "f_ok_to_use not set right");

}

/**
*\purpose
* The purpose of the test is to verify the flag f_trailer_related_det and f_ok_to_use won't
* be updated when trailer width is 0.
* \req   CPR-5451
*/
TEST(f360_mark_trailer_detections, detect_det_on_trailer_trailer_width_0)
{
   /** \precond
   * Use default settings
   **/
   trailer_detector_output.trailer_width[0] = 0.0F;

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_use is not updated
   **/
   CHECK_FALSE_TEXT(det_props[0].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[1].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[2].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[3].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[4].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[4].f_ok_to_use , "f_ok_to_use not set right");
}
/** @}*/

/** \defgroup  f360_mark_trailer_detections_4_detections
 *  @{
 */
/** \brief
*  This test group contains tests for Occluded_By_Trailer in Mark_Trailer_Dets
*  function.
**/
TEST_GROUP(f360_mark_trailer_detections_fov)
{
   rspp_variant_A::RSPP_Detection_List_T raw_det_list{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Host_T host = {};
   F360_Calibrations_T calibs = {};
   F360_Trailer_Estimator_Output_T trailer_detector_output = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** \setup
    * Default setup will change f_ok_to_use and f_trailer_related_det after function call
    **/
   TEST_SETUP()
   {
      // Initialize host
      host.dist_rear_axle_to_vcs_m = 3.7F;

      // Initialize detections
      const float32_t distance_rear_axle_to_tow_hitch = 1.2F;    // distance between rear axle to to hitch (m)
      calibs.k_trailer_distance_rear_axle_to_tow_hitch = distance_rear_axle_to_tow_hitch;

      trailer_detector_output.trailer_angle[0] = F360_DEG2RAD(45.0F);
      
      #define LEFT 1
      #define RIGHT 2
      // sensor locations
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -2.14675498;
      sensors[0].constant.mounting_position.vcs_position.lateral = -0.779999971;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -4.59999990;
      sensors[0].constant.polarity = 1;
      sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[1].constant.mounting_position.vcs_boresight_azimuth_angle = 2.14675498;
      sensors[1].constant.mounting_position.vcs_position.lateral = 0.779999971;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -4.59999990;
      sensors[1].constant.polarity = 1;
      
      //create 5 rear right snsr dets
      for(int8_t i = 0; i < 5; i++)
      {
         det_props[i].vcs_position.x = -20.0F;
         det_props[i].vcs_position.y = -5.0F * i;
         det_props[i].f_ok_to_use = true;
         det_props[i].f_trailer_related_det = false;
         raw_det_list.detections[i].raw.sensor_id = 2;
         raw_det_list.detections[i].processed.vcs_position_x = det_props[i].vcs_position.x;
         raw_det_list.detections[i].processed.vcs_position_y = det_props[i].vcs_position.y;
         raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(det_props[i], sensors[raw_det_list.detections[i].raw.sensor_id-1]);
         
         int8_t j = 5 + i;
         det_props[j].vcs_position.x = -20.0F;
         det_props[j].vcs_position.y = -5.0F * i;
         det_props[j].f_ok_to_use = true;
         det_props[j].f_trailer_related_det = false;
         raw_det_list.detections[j].raw.sensor_id = 1;
         raw_det_list.detections[j].processed.vcs_position_x = det_props[j].vcs_position.x;
         raw_det_list.detections[j].processed.vcs_position_y = det_props[j].vcs_position.y;
         raw_det_list.detections[j].raw.azimuth = Get_Det_Scs_Az(det_props[j], sensors[raw_det_list.detections[j].raw.sensor_id-1]);
      }

      // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
      for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
      {
         raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
         raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
      }
      raw_det_list.number_of_valid_detections = 10U;
      Sort_Detections_Vcs_Long(raw_det_list);

      trailer_detector_output.trailer_presence[0] = TRAILER_PRESENCE_STATE_DETECTED;
      trailer_detector_output.trailer_width[0] = 2.0F;
      trailer_detector_output.trailer_length[0] = 8.0F;

   }
};

/**
*\purpose
* The purpose of the test is to verify the flag f_trailer_related_det and f_ok_to_use 
* are properly set by Occluded_By_Trailer when host is doing right turn and all detections are occluded
* \req   CPR-5453
*/
TEST(f360_mark_trailer_detections_fov, detect_trailer_occluded_detection_host_right_turn)
{
   /** \precond
    * The host is turning right
    * All valids detections are occluded by trailer bounding box
    **/

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_useis set to expected value.
   **/
   for(uint8_t i=0; i>10; i++)
   {
      CHECK_TRUE_TEXT(det_props[i].f_trailer_related_det , "f_trailer_related_det not set right");
      CHECK_FALSE_TEXT(det_props[i].f_ok_to_use , "f_ok_to_use not set right");
   }
}

/**
*\purpose
* The purpose of the test is to verify the flag f_trailer_related_det and f_ok_to_use 
* are properly set by Occluded_By_Trailer when host is doing left turn and all detections are occluded
* \req   CPR-5453
*/
TEST(f360_mark_trailer_detections_fov, detect_trailer_occluded_detection_host_left_turn)
{
   /** \precond
    * The host is turning left
   * All valids detections are occluded by trailer bounding box
    **/
   trailer_detector_output.trailer_angle[0] = F360_DEG2RAD(-60.0F);

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_useis set to expected value.
   **/
   for(uint8_t i=0; i>10; i++)
   {
      CHECK_TRUE_TEXT(det_props[i].f_trailer_related_det , "f_trailer_related_det not set right");
      CHECK_FALSE_TEXT(det_props[i].f_ok_to_use , "f_ok_to_use not set right");
   }
}
/** @}*/

/** \defgroup  f360_mark_trailer_detections_for_trailer_reflection
 *  @{
 */
/** \brief
*  This test group contains tests for Trailer_Reflection in Mark_Trailer_Dets
*  function.
**/
TEST_GROUP(f360_mark_trailer_reflection_detection)
{
   rspp_variant_A::RSPP_Detection_List_T raw_det_list{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Host_T host = {};
   F360_Calibrations_T calibs = {};
   F360_Trailer_Estimator_Output_T trailer_detector_output = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** \setup
    * Default setup will change f_ok_to_use and f_trailer_related_det after function call
    **/
   TEST_SETUP()
   {
      // Initialize host
      host.dist_rear_axle_to_vcs_m = 3.7F;
      host.speed = 5.0F;

      // Initialize detections
      raw_det_list.number_of_valid_detections = 3U;

      const float32_t distance_rear_axle_to_tow_hitch = 1.2F;    // distance between rear axle to to hitch (m)
      calibs.k_trailer_distance_rear_axle_to_tow_hitch = distance_rear_axle_to_tow_hitch;

      trailer_detector_output.trailer_angle[0] = F360_DEG2RAD(0.0F);
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = F360_DEG2RAD(115.0F);
      sensors[0].constant.polarity = 1;

      // D1 detection i.e. detection on trailer that potentially bounces the chirp from the stationary environment back to the sensor
      det_props[0].vcs_position.x = -7.0F;
      det_props[0].vcs_position.y = 2.4F;
      det_props[0].f_ok_to_use = true;
      det_props[0].f_trailer_related_det = false;
      raw_det_list.detections[0].raw.sensor_id = 1;
      raw_det_list.detections[0].raw.azimuth = F360_DEG2RAD(45.0F);
      raw_det_list.detections[0].processed.vcs_position_x = det_props[0].vcs_position.x;
      raw_det_list.detections[0].processed.vcs_position_y = det_props[0].vcs_position.y;
      raw_det_list.detections[0].raw.range = 2.67;

      // stationary detection
      det_props[1].vcs_position.x = -6.1F;
      det_props[1].vcs_position.y = 5.3F;
      det_props[1].range_rate_compensated = 0.4F;
      det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
      det_props[1].f_ok_to_use = true;
      det_props[1].f_trailer_related_det = false;
      raw_det_list.detections[1].raw.sensor_id = 1;
      raw_det_list.detections[1].processed.vcs_position_x = det_props[1].vcs_position.x;
      raw_det_list.detections[1].processed.vcs_position_y = det_props[1].vcs_position.y;
      raw_det_list.detections[1].processed.range_rate_compensated = det_props[1].range_rate_compensated;
      raw_det_list.detections[1].processed.motion_status = det_props[1].motion_status;

      //D2 detection 
      det_props[2].vcs_position.x = -13.7F;
      det_props[2].vcs_position.y = 4.82F;
      det_props[2].range_rate_compensated = 10.0F;
      det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_props[2].f_ok_to_use = true;
      det_props[2].f_trailer_related_det = false;
      raw_det_list.detections[2].raw.sensor_id = 1;
      raw_det_list.detections[2].processed.vcs_position_x = det_props[2].vcs_position.x;
      raw_det_list.detections[2].processed.vcs_position_y = det_props[2].vcs_position.y;
      raw_det_list.detections[2].processed.range_rate_compensated = det_props[2].range_rate_compensated;
      raw_det_list.detections[2].processed.motion_status = det_props[2].motion_status;
      raw_det_list.detections[2].raw.azimuth = F360_DEG2RAD(45.0F);
      raw_det_list.detections[2].raw.range = 14.59;

      // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
      for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
      {
         raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
         raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
      }

      Sort_Detections_Vcs_Long(raw_det_list);

      trailer_detector_output.trailer_presence[0] = TRAILER_PRESENCE_STATE_DETECTED;
      trailer_detector_output.trailer_width[0] = 2.0F;
      trailer_detector_output.trailer_length[0] = 8.0F;

   }
};

/**
*\purpose
* The purpose of the test is to verify the flag f_trailer_related_det and f_ok_to_use 
* are properly set for a detection that fit multi-path relations with trailer bounding box and guardrail
* \req   CPR-5452
*/
TEST(f360_mark_trailer_reflection_detection, trailer_guardrail_reflection_detection_10m)
{
   /** \precond
    * Default setting
    * - Detection 0 from trailer that potentially bounces the chirp from the stationary environment back to the sensor
    * - Detection 1 from stationary environment that represents the object source, located within 10 meters from the host
    * - Detection 2 is the clutter detection due to multipath
    **/

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_use are set to expected value. In this case, only the clutter detection 2 is marked
   * as not okay to use and detection on trailer.
   **/
   CHECK_FALSE_TEXT(det_props[1].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "f_ok_to_use not set right");

   CHECK_FALSE_TEXT(det_props[0].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "f_ok_to_use not set right");

   CHECK_TRUE_TEXT(det_props[2].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_FALSE_TEXT(det_props[2].f_ok_to_use , "f_ok_to_use not set right");
}

/**
*\purpose
* The purpose of the test is to verify the flag f_trailer_related_det and f_ok_to_use 
* are properly set for a detection that fit multi-path relations with trailer bounding box and surface of the road
* \req   CPR-5456
*/
TEST(f360_mark_trailer_reflection_detection, trailer_ground_reflection)
{
   /** \precond
    * Default setting
    * - Detection 0 from the region of likely ground reflected detections and elevation is not significantly high (<0.11)
    * - Detection 1 from the region of likely ground reflected detections and elevation is significantly high (>0.11)
    * - Detection 2 from outside the region of likely ground reflected detections and elevation is significantly high (>0.11)
    * - Change the default compensated range rate setting for detection 2 to avoid being considered for trailer and guardrail multi-path
    **/
   raw_det_list.detections[0].processed.vcs_el = 0.10F;
   det_props[0].vcs_position.x = -18.0F;
   det_props[0].vcs_position.y = 6.5F;

   raw_det_list.detections[1].processed.vcs_el = 0.13F;
   det_props[1].vcs_position.x = -18.0F;
   det_props[1].vcs_position.y = 6.5F;

   raw_det_list.detections[2].processed.vcs_el = 0.13F;
   det_props[2].vcs_position.x = -18.0F;
   det_props[2].vcs_position.y = 9.5F;
   det_props[2].range_rate_compensated = 1.0F;
   

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_use are set to expected value. In this case, only the detection 1 is marked
   * as not okay to use and detection on trailer.
   **/
   CHECK_FALSE_TEXT(det_props[0].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_TRUE_TEXT(det_props[1].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_FALSE_TEXT(det_props[1].f_ok_to_use , "f_ok_to_use not set right");
   CHECK_FALSE_TEXT(det_props[2].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "f_ok_to_use not set right");
}
/** @}*/
