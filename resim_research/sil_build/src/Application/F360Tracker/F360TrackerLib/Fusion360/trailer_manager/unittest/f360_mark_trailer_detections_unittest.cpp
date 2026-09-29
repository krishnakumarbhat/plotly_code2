/** \file
   This file contains unit test for functions in f360_mark_trailer_detections.
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
      const rspp_variant_A::RSPP_Detection_T& raw_det,
      const F360_Radar_Sensor_T& sensor)
   {
      const float32_t lon_scs = raw_det.processed.vcs_position_x - sensor.constant.mounting_position.vcs_position.longitudinal;
      const float32_t lat_scs = raw_det.processed.vcs_position_y - sensor.constant.mounting_position.vcs_position.lateral;
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

      // Initialize calibrations

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

      raw_det_list.detections[0].processed.vcs_position_x = -(host.dist_rear_axle_to_vcs_m + distance_rear_axle_to_tow_hitch);
      raw_det_list.detections[0].processed.vcs_position_y = 0.0F;
      raw_det_list.detections[0].raw.sensor_id = LEFT;
      det_props[0].f_ok_to_use = true;
      det_props[0].f_trailer_related_det = false;
      det_props[0].vcs_position.x = -(host.dist_rear_axle_to_vcs_m + distance_rear_axle_to_tow_hitch);
      det_props[0].vcs_position.y = 0.0F;

      raw_det_list.detections[1].processed.vcs_position_x = -(host.dist_rear_axle_to_vcs_m + distance_rear_axle_to_tow_hitch) - 1.0F;
      raw_det_list.detections[1].raw.sensor_id = LEFT;
      det_props[1].f_ok_to_use = true;
      det_props[1].f_trailer_related_det = false;
      det_props[1].vcs_position.x = -(host.dist_rear_axle_to_vcs_m + distance_rear_axle_to_tow_hitch) - 1.0F;
      det_props[1].vcs_position.y = 0.0F;

      raw_det_list.detections[2].processed.vcs_position_x = 10.0F;
      raw_det_list.detections[2].raw.sensor_id = RIGHT;
      det_props[2].f_ok_to_use = true;
      det_props[2].f_trailer_related_det = false;
      det_props[2].vcs_position.x = 10.0F;
      det_props[2].vcs_position.y = 0.0F;

      raw_det_list.detections[3].processed.vcs_position_x = -20.0F;
      raw_det_list.detections[3].raw.sensor_id = LEFT;
      det_props[3].f_ok_to_use = true;
      det_props[3].f_trailer_related_det = false;
      det_props[3].vcs_position.x = -20.0F;
      det_props[3].vcs_position.y = 0.0F;

      raw_det_list.detections[4].processed.vcs_position_x = -10.0F;
      raw_det_list.detections[4].raw.sensor_id = LEFT;
      det_props[4].f_ok_to_use = true;
      det_props[4].f_trailer_related_det = false;
      det_props[4].vcs_position.x = -10.0F;
      det_props[4].vcs_position.y = 1.6F;

      for(uint8_t i=0U; i<5;i++) raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);


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
      trailer_detector_output.f_reversing_countermeasures_active = false;

   }
};

/**
*\purpose
* The purpose of the test is to verify the computation of the lines made by trailer edges.
* This local function is similar to the existing Line class
*/
TEST(f360_mark_trailer_detections, compute_line_parameters_test)
{
   /** \precond
   * Use default settings
   **/
  // on line y = 2*x + 3
   const float32_t rear_point[2]{-3.0F, -3.0F};
   const float32_t front_point[2]{1.0F, 5.0F};
   Trailer_line line_param;
   const Trailer_line expected_line_param{8.0F, -4.0F, 12.0F};

   /** \action
   * Execute the function
   **/
   Compute_Line_Parameters(rear_point, front_point, line_param);

   /** \result
   * Check that the computed line parameters are correct.
   **/
   DOUBLES_EQUAL_TEXT(expected_line_param.a, line_param.a, 0.1F, "the first line parameter is wrong" );
   DOUBLES_EQUAL_TEXT(expected_line_param.b, line_param.b, 0.1F, "the second line parameter is wrong");
   DOUBLES_EQUAL_TEXT(expected_line_param.c, line_param.c, 0.1F, "the third line parameter, is wrong");
}

/**
*\purpose
* The purpose of the test is to check the inside_trailer detection logic when the host is slow. 
* Trailer is at left, i.e, angle > 0, and 2 detections inside the trailer.
* 2 detection are slightly off the trailer but within the buffer and 2 detections are clearly outside the trailer.
* among the pairs, one of them is with high raw range rate, while the other is with low raw range rate.
* The test checks that only the detections inside area and with low enough raw range rate are marked as not ok to use
*/
TEST(f360_mark_trailer_detections, trailer_dets_are_determined_correctly_based_on_location_and_raw_range_rates_trailer_stationary)
{
   /** \precond
   * Three pairs of detections with same locations, and only one of them has higher raw range rates. 
   * The trailer partly blocks the left sensor FOV
   **/
   const int8_t trailer_idx = 0;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   trailer_detector_output.trailer_angle[trailer_idx] = F360_DEG2RAD(30.0F);
   trailer_detector_output.trailer_width[trailer_idx] = 2.0F;
   trailer_detector_output.trailer_length[trailer_idx] = 10.0F;

   trailer_detector_output.joint_position_vcs_long[trailer_idx] = -7.0F;
   trailer_detector_output.joint_position_vcs_lat[trailer_idx] = 0.0F;

   sensors[0].variable.is_valid = true;
   sensors[1].variable.is_valid = true;

   // det1 inside the trailer, low range rate
   raw_det_list.detections[0].processed.vcs_position_x = -8.0F;
   raw_det_list.detections[0].processed.vcs_position_y = -1.0F;
   raw_det_list.detections[0].raw.sensor_id = LEFT;
   raw_det_list.detections[0].raw.range_rate = 0.39F;
   det_props[0].f_ok_to_use = true;
   det_props[0].f_trailer_related_det = false;
   det_props[0].vcs_position.x = -8.0F;
   det_props[0].vcs_position.y = -1.0F;

   // det2 inside the trailer, higher range rate
   raw_det_list.detections[1].processed.vcs_position_x = -8.0F;
   raw_det_list.detections[1].processed.vcs_position_y = -1.0F;
   raw_det_list.detections[1].raw.sensor_id = LEFT;
   raw_det_list.detections[1].raw.range_rate = 0.41F;
   det_props[1].f_ok_to_use = true;
   det_props[1].f_trailer_related_det = false;
   det_props[1].vcs_position.x = -8.0F;
   det_props[1].vcs_position.y = -1.0F;

   // det3 outside the trailer but in buffer, lower range rate
   raw_det_list.detections[2].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[2].processed.vcs_position_y = -2.0F;
   raw_det_list.detections[2].raw.sensor_id = LEFT;
   raw_det_list.detections[2].raw.range_rate = 0.39F;
   det_props[2].f_ok_to_use = true;
   det_props[2].f_trailer_related_det = false;
   det_props[2].vcs_position.x = -7.0F;
   det_props[2].vcs_position.y = -2.0F;

   // det4 outside the trailer but in buffer, higher range rate
   raw_det_list.detections[3].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[3].processed.vcs_position_y = -2.0F;
   raw_det_list.detections[3].raw.sensor_id = LEFT;
   raw_det_list.detections[3].raw.range_rate = 0.41F;

   det_props[3].f_ok_to_use = true;
   det_props[3].f_trailer_related_det = false;
   det_props[3].vcs_position.x = -7.0F;
   det_props[3].vcs_position.y = -2.0F;

   // det5 outside the trailer, lower range rate
   raw_det_list.detections[4].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[4].processed.vcs_position_y = -5.0F;
   raw_det_list.detections[4].raw.sensor_id = LEFT;
   raw_det_list.detections[4].raw.range_rate = 0.39F;
   det_props[4].f_ok_to_use = true;
   det_props[4].f_trailer_related_det = false;
   det_props[4].vcs_position.x = -7.0F;
   det_props[4].vcs_position.y = -5.0F;

   // det4 outside the trailer but in buffer, higher range rate
   raw_det_list.detections[5].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[5].processed.vcs_position_y = -5.0F;
   raw_det_list.detections[5].raw.sensor_id = LEFT;
   raw_det_list.detections[5].raw.range_rate = 0.41F;
   det_props[5].f_ok_to_use = true;
   det_props[5].f_trailer_related_det = false;
   det_props[5].vcs_position.x = -7.0F;
   det_props[5].vcs_position.y = -5.0F;

   host.vcs_speed = 0.0F; // slow host

   for(uint8_t i=0U; i<6;i++){
      raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);
      raw_det_list.detections[i].raw.range = F360_Get_Hypotenuse(raw_det_list.detections[i].processed.vcs_position_x, raw_det_list.detections[i].processed.vcs_position_y);
   }



   // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
   }

   Sort_Detections_Vcs_Long(raw_det_list);

   /** \action
   * Execute the function
   **/
  
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_ok_to_use mark results are correct
   **/
   CHECK_FALSE_TEXT(det_props[0].f_ok_to_use , "inside trailer det with low speed set to OK to use");
   CHECK_FALSE_TEXT(det_props[1].f_ok_to_use , "inside trailer det with higher speed set to OK to use");
   CHECK_FALSE_TEXT(det_props[2].f_ok_to_use , "near trailer det with low speed set to OK to use");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "near trailer det with higher speed not set to OK to use");
   CHECK_TRUE_TEXT(det_props[4].f_ok_to_use , "outside trailer det with low speed not set to OK to use");
   CHECK_TRUE_TEXT(det_props[5].f_ok_to_use , "outside trailer det with higher speed not set to OK to use");
}

/**
*\purpose
* The purpose of the test is to check the mark trailer det logic when the reversing countermeasures are active. 
* Trailer is at left, i.e, angle > 0, and 2 detections inside the trailer.
* 2 detection are slightly off the trailer but within the buffer and 2 detections are clearly outside the trailer.
* among the pairs, one of them is with high raw range rate, while the other is with low raw range rate.
* The test checks that only the detections from rear are marked as not ok to use when reversing countermeasures are active
*/
TEST(f360_mark_trailer_detections, all_dets_from_rear_are_marked_correctly_when_reversing_countermeasures_active)
{
   /** \precond
   * Three pairs of detections with same locations, and only one of them has higher raw range rates. 
   * The trailer partly blocks the left sensor FOV
   **/
   const int8_t trailer_idx = 0;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   trailer_detector_output.trailer_angle[trailer_idx] = F360_DEG2RAD(30.0F);
   trailer_detector_output.trailer_width[trailer_idx] = 2.0F;
   trailer_detector_output.trailer_length[trailer_idx] = 10.0F;
   trailer_detector_output.f_reversing_countermeasures_active = true;  // To test the reversing countermeasures active case

   trailer_detector_output.joint_position_vcs_long[trailer_idx] = -7.0F;
   trailer_detector_output.joint_position_vcs_lat[trailer_idx] = 0.0F;

   sensors[0].variable.is_valid = true;
   sensors[1].variable.is_valid = true;

   raw_det_list.number_of_valid_detections = 10U;

   // det1 inside the trailer, low range rate
   raw_det_list.detections[0].processed.vcs_position_x = -8.0F;
   raw_det_list.detections[0].processed.vcs_position_y = -1.0F;
   raw_det_list.detections[0].raw.sensor_id = LEFT;
   raw_det_list.detections[0].raw.range_rate = 0.39F;
   det_props[0].f_ok_to_use = true;
   det_props[0].f_trailer_related_det = false;
   det_props[0].vcs_position.x = -8.0F;
   det_props[0].vcs_position.y = -1.0F;

   // det2 inside the trailer, higher range rate
   raw_det_list.detections[1].processed.vcs_position_x = -8.0F;
   raw_det_list.detections[1].processed.vcs_position_y = -1.0F;
   raw_det_list.detections[1].raw.sensor_id = LEFT;
   raw_det_list.detections[1].raw.range_rate = 0.41F;
   det_props[1].f_ok_to_use = true;
   det_props[1].f_trailer_related_det = false;
   det_props[1].vcs_position.x = -8.0F;
   det_props[1].vcs_position.y = -1.0F;

   // det3 outside the trailer but in buffer, lower range rate
   raw_det_list.detections[2].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[2].processed.vcs_position_y = -2.0F;
   raw_det_list.detections[2].raw.sensor_id = LEFT;
   raw_det_list.detections[2].raw.range_rate = 0.39F;
   det_props[2].f_ok_to_use = true;
   det_props[2].f_trailer_related_det = false;
   det_props[2].vcs_position.x = -7.0F;
   det_props[2].vcs_position.y = -2.0F;

   // det4 outside the trailer but in buffer, higher range rate
   raw_det_list.detections[3].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[3].processed.vcs_position_y = -2.0F;
   raw_det_list.detections[3].raw.sensor_id = LEFT;
   raw_det_list.detections[3].raw.range_rate = 0.41F;

   det_props[3].f_ok_to_use = true;
   det_props[3].f_trailer_related_det = false;
   det_props[3].vcs_position.x = -7.0F;
   det_props[3].vcs_position.y = -2.0F;

   // det5 outside the trailer, lower range rate
   raw_det_list.detections[4].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[4].processed.vcs_position_y = -5.0F;
   raw_det_list.detections[4].raw.sensor_id = LEFT;
   raw_det_list.detections[4].raw.range_rate = 0.39F;
   det_props[4].f_ok_to_use = true;
   det_props[4].f_trailer_related_det = false;
   det_props[4].vcs_position.x = -7.0F;
   det_props[4].vcs_position.y = -5.0F;

   // det6 outside the trailer but in buffer, higher range rate
   raw_det_list.detections[5].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[5].processed.vcs_position_y = -5.0F;
   raw_det_list.detections[5].raw.sensor_id = LEFT;
   raw_det_list.detections[5].raw.range_rate = 0.41F;
   det_props[5].f_ok_to_use = true;
   det_props[5].f_trailer_related_det = false;
   det_props[5].vcs_position.x = -7.0F;
   det_props[5].vcs_position.y = -5.0F;

   // det7 outside the trailer and at front
   raw_det_list.detections[6].processed.vcs_position_x = 7.0F;
   raw_det_list.detections[6].processed.vcs_position_y = -5.0F;
   raw_det_list.detections[6].raw.sensor_id = LEFT;
   raw_det_list.detections[6].raw.range_rate = 0.41F;
   det_props[6].f_ok_to_use = true;
   det_props[6].f_trailer_related_det = false;
   det_props[6].vcs_position.x = 7.0F;
   det_props[6].vcs_position.y = -5.0F;

   // det8 outside the trailer and at front
   raw_det_list.detections[7].processed.vcs_position_x = 7.0F;
   raw_det_list.detections[7].processed.vcs_position_y = 5.0F;
   raw_det_list.detections[7].raw.sensor_id = LEFT;
   raw_det_list.detections[7].raw.range_rate = 0.41F;
   det_props[7].f_ok_to_use = true;
   det_props[7].f_trailer_related_det = false;
   det_props[7].vcs_position.x = 7.0F;
   det_props[7].vcs_position.y = 5.0F;
   
   // det7 outside the trailer and at front
   raw_det_list.detections[8].processed.vcs_position_x = -30.0F;
   raw_det_list.detections[8].processed.vcs_position_y = -5.0F;
   raw_det_list.detections[8].raw.sensor_id = LEFT;
   raw_det_list.detections[8].raw.range_rate = 0.41F;
   det_props[8].f_ok_to_use = true;
   det_props[8].f_trailer_related_det = false;
   det_props[8].vcs_position.x = -30.0F;
   det_props[8].vcs_position.y = -5.0F;

   // det8 outside the trailer and at front
   raw_det_list.detections[9].processed.vcs_position_x = -30.0F;
   raw_det_list.detections[9].processed.vcs_position_y = 5.0F;
   raw_det_list.detections[9].raw.sensor_id = LEFT;
   raw_det_list.detections[9].raw.range_rate = 0.41F;
   det_props[9].f_ok_to_use = true;
   det_props[9].f_trailer_related_det = false;
   det_props[9].vcs_position.x = -30.0F;
   det_props[9].vcs_position.y = 5.0F;
   host.vcs_speed = 0.0F; // slow host

   for(uint8_t i=0U; i<raw_det_list.number_of_valid_detections;i++){
      raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);
      raw_det_list.detections[i].raw.range = F360_Get_Hypotenuse(raw_det_list.detections[i].processed.vcs_position_x, raw_det_list.detections[i].processed.vcs_position_y);
   }



   // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
   }

   Sort_Detections_Vcs_Long(raw_det_list);

   /** \action
   * Execute the function
   **/
  
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_ok_to_use and f_trailer_related_det mark results are correct
   **/
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "inside trailer det with low speed set as not ok to use, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "inside trailer det with higher speed set as not ok to use, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "near trailer det with low speed set as not ok to use, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "near trailer det with higher speed set as not ok to use, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[4].f_ok_to_use , "outside trailer det with low speed set as not ok to use, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[5].f_ok_to_use , "outside trailer det with higher speed set as not ok to use, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[6].f_ok_to_use , "front det set to not ok to use when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[7].f_ok_to_use , "front det set to not ok to use when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[8].f_ok_to_use , "far rear det set to not ok to use when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[9].f_ok_to_use , "far rear det set to not ok to use when reversing countermeasures active");

   CHECK_TRUE_TEXT(det_props[0].f_trailer_related_det , "inside trailer det with low speed not set to trailer related, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[1].f_trailer_related_det , "inside trailer det with higher speed not set to trailer related, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[2].f_trailer_related_det , "near trailer det with low speed not set to trailer related, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[3].f_trailer_related_det , "near trailer det with higher speed not set to trailer related, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[4].f_trailer_related_det , "outside trailer det with low speed not set to trailer related, when reversing countermeasures active");
   CHECK_TRUE_TEXT(det_props[5].f_trailer_related_det , "outside trailer det with higher speed not set to trailer related, when reversing countermeasures active");
   CHECK_FALSE_TEXT(det_props[6].f_trailer_related_det , "front det set to trailer related when reversing countermeasures active");
   CHECK_FALSE_TEXT(det_props[7].f_trailer_related_det , "front det set to trailer ral when reversing countermeasures active");
   CHECK_FALSE_TEXT(det_props[8].f_trailer_related_det , "far rear det set to trailer ral when reversing countermeasures active");
   CHECK_FALSE_TEXT(det_props[9].f_trailer_related_det , "far rear det set to trailer ral when reversing countermeasures active");
}

/**
*\purpose
* The purpose of the test is to check the inside_trailer detection logic when the host is fast. 
* Trailer is at right, i.e, angle < 0, and 2 detections inside the trailer.
* 2 detection are slightly off the trailer but within the buffer and 2 detections are clearly outside the trailer.
* among the pairs, one of them is with high raw range rate, while the other is with low raw range rate.
* The test checks that only the detections inside area and with low enough raw range rate are marked as not ok to use
*/
TEST(f360_mark_trailer_detections, trailer_dets_are_determined_correctly_based_on_location_and_raw_range_rates_trailer_highspeed)
{
   /** \precond
   * Three pairs of detections with same locations, and only one of them has higher raw range rates. 
   * The trailer partly blocks the right sensor FOV
   **/
   const int8_t trailer_idx = 0;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   trailer_detector_output.trailer_angle[trailer_idx] = F360_DEG2RAD(-30.0F);
   trailer_detector_output.trailer_width[trailer_idx] = 2.0F;
   trailer_detector_output.trailer_length[trailer_idx] = 10.0F;

   trailer_detector_output.joint_position_vcs_long[trailer_idx] = -7.0F;
   trailer_detector_output.joint_position_vcs_lat[trailer_idx] = 0.0F;

   sensors[0].variable.is_valid = true;
   sensors[1].variable.is_valid = true;

   // det1 inside the trailer, low range rate
   raw_det_list.detections[0].processed.vcs_position_x = -8.0F;
   raw_det_list.detections[0].processed.vcs_position_y = 1.0F;
   raw_det_list.detections[0].raw.sensor_id = RIGHT;
   raw_det_list.detections[0].raw.range_rate = 3.9F;
   det_props[0].f_ok_to_use = true;
   det_props[0].f_trailer_related_det = false;
   det_props[0].vcs_position.x = -8.0F;
   det_props[0].vcs_position.y = 1.0F;

   // det2 inside the trailer, higher range rate
   raw_det_list.detections[1].processed.vcs_position_x = -8.0F;
   raw_det_list.detections[1].processed.vcs_position_y = 1.0F;
   raw_det_list.detections[1].raw.sensor_id = RIGHT;
   raw_det_list.detections[1].raw.range_rate = 4.1F;
   det_props[1].f_ok_to_use = true;
   det_props[1].f_trailer_related_det = false;
   det_props[1].vcs_position.x = -8.0F;
   det_props[1].vcs_position.y = 1.0F;

   // det3 outside the trailer but in buffer, lower range rate
   raw_det_list.detections[2].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[2].processed.vcs_position_y = 2.0F;
   raw_det_list.detections[2].raw.sensor_id = RIGHT;
   raw_det_list.detections[2].raw.range_rate = 3.9F;
   det_props[2].f_ok_to_use = true;
   det_props[2].f_trailer_related_det = false;
   det_props[2].vcs_position.x = -7.0F;
   det_props[2].vcs_position.y = 2.0F;

   // det4 outside the trailer but in buffer, higher range rate
   raw_det_list.detections[3].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[3].processed.vcs_position_y = 2.0F;
   raw_det_list.detections[3].raw.sensor_id = RIGHT;
   raw_det_list.detections[3].raw.range_rate = 4.1F;
   det_props[3].f_ok_to_use = true;
   det_props[3].f_trailer_related_det = false;
   det_props[3].vcs_position.x = -7.0F;
   det_props[3].vcs_position.y = 2.0F;

   // det5 outside the trailer, lower range rate
   raw_det_list.detections[4].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[4].processed.vcs_position_y = 5.0F;
   raw_det_list.detections[4].raw.sensor_id = RIGHT;
   raw_det_list.detections[4].raw.range_rate = 3.9F;
   det_props[4].f_ok_to_use = true;
   det_props[4].f_trailer_related_det = false;
   det_props[4].vcs_position.x = -7.0F;
   det_props[4].vcs_position.y = 5.0F;

   // det4 outside the trailer but in buffer, higher range rate
   raw_det_list.detections[5].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[5].processed.vcs_position_y = 5.0F;
   raw_det_list.detections[5].raw.sensor_id = RIGHT;
   raw_det_list.detections[5].raw.range_rate = 4.1F;
   det_props[5].f_ok_to_use = true;
   det_props[5].f_trailer_related_det = false;
   det_props[5].vcs_position.x = -7.0F;
   det_props[5].vcs_position.y = 5.0F;

   host.vcs_speed = 2.1F; // fast host

   for(uint8_t i=0U; i<6;i++){
      raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);
      raw_det_list.detections[i].raw.range = F360_Get_Hypotenuse(raw_det_list.detections[i].processed.vcs_position_x, raw_det_list.detections[i].processed.vcs_position_y);
   }

   // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
   }

   Sort_Detections_Vcs_Long(raw_det_list);

   /** \action
   * Execute the function
   **/
  
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_ok_to_use results are correct
   **/
   CHECK_FALSE_TEXT(det_props[0].f_ok_to_use , "inside trailer det with low speed set to OK to use");
   CHECK_FALSE_TEXT(det_props[1].f_ok_to_use , "inside trailer det with higher speed set to OK to use");
   CHECK_FALSE_TEXT(det_props[2].f_ok_to_use , "near trailer det with low speed set to OK to use");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "near trailer det with higher speed not set to OK to use");
   CHECK_TRUE_TEXT(det_props[4].f_ok_to_use , "outside trailer det with low speed not set to OK to use");
   CHECK_TRUE_TEXT(det_props[5].f_ok_to_use , "outside trailer det with higher speed not set to OK to use");
}

/**
*\purpose
* The purpose of the test is to check the clutter detection logic. 
* Trailer is at left, i.e, angle > 0, and 2 detections inside the clutter detection area.
* 2 detection are off the area.
* among the pairs, one of them is with high raw range rate, while the other is with low raw range rate.
* The test checks that only the detections inside the clutter detection area and with low enough raw range rate are marked as trailer detections
* and all ok to use
*/
TEST(f360_mark_trailer_detections, clutter_dets_are_handled_correctly)
{
   /** \precond
   * Three pairs of detections with same locations, and only one of them has higher raw range rates. 
   * The trailer partly blocks the left sensor FOV
   **/
   const int8_t trailer_idx = 0;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   trailer_detector_output.trailer_angle[trailer_idx] = F360_DEG2RAD(5.0F);
   trailer_detector_output.trailer_width[trailer_idx] = 2.0F;
   trailer_detector_output.trailer_length[trailer_idx] = 10.0F;

   trailer_detector_output.joint_position_vcs_long[trailer_idx] = -7.0F;
   trailer_detector_output.joint_position_vcs_lat[trailer_idx] = 0.0F;

   sensors[0].variable.is_valid = true;
   sensors[1].variable.is_valid = true;

   // det1 inside the detected region, low range rate
   raw_det_list.detections[0].processed.vcs_position_x = -15.0F;
   raw_det_list.detections[0].processed.vcs_position_y = -3.0F;
   raw_det_list.detections[0].raw.sensor_id = LEFT;
   raw_det_list.detections[0].raw.range_rate = 0.39F;
   det_props[0].f_ok_to_use = true;
   det_props[0].f_trailer_related_det = false;
   det_props[0].vcs_position.x = -15.0F;
   det_props[0].vcs_position.y = -3.0F;

   // det2 inside the detected region, higher range rate
   raw_det_list.detections[1].processed.vcs_position_x = -15.0F;
   raw_det_list.detections[1].processed.vcs_position_y = -3.0F;
   raw_det_list.detections[1].raw.sensor_id = LEFT;
   raw_det_list.detections[1].raw.range_rate = 0.41F;
   det_props[1].f_ok_to_use = true;
   det_props[1].f_trailer_related_det = false;
   det_props[1].vcs_position.x = -15.0F;
   det_props[1].vcs_position.y = -3.0F;

   // det3 outside the detected region, lower range rate
   raw_det_list.detections[2].processed.vcs_position_x = -15.0F;
   raw_det_list.detections[2].processed.vcs_position_y = -6.0F;
   raw_det_list.detections[2].raw.sensor_id = LEFT;
   raw_det_list.detections[2].raw.range_rate = 0.39F;
   det_props[2].f_ok_to_use = true;
   det_props[2].f_trailer_related_det = false;
   det_props[2].vcs_position.x = -15.0F;
   det_props[2].vcs_position.y = -6.0F;

   // det4 outside the detected region, higher range rate
   raw_det_list.detections[3].processed.vcs_position_x = -15.0F;
   raw_det_list.detections[3].processed.vcs_position_y = -6.0F;
   raw_det_list.detections[3].raw.sensor_id = LEFT;
   raw_det_list.detections[3].raw.range_rate = 0.41F;
   det_props[3].f_ok_to_use = true;
   det_props[3].f_trailer_related_det = false;
   det_props[3].vcs_position.x = -15.0F;
   det_props[3].vcs_position.y = -6.0F;

   host.vcs_speed = 5.0F; // normal host speed

   for(uint8_t i=0U; i<4;i++){
      raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);
      raw_det_list.detections[i].raw.range = F360_Get_Hypotenuse(raw_det_list.detections[i].processed.vcs_position_x, raw_det_list.detections[i].processed.vcs_position_y);
   }

   // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
   }

   Sort_Detections_Vcs_Long(raw_det_list);

   /** \action
   * Execute the function
   **/
  
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_ok_to_use and f_trailer_related_det mark results are correct
   **/
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "clutter area det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "clutter area det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "off-clutter area det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "off-clutter area det not set to OK to use");

   CHECK_TRUE_TEXT(det_props[0].f_trailer_related_det , "clutter area det with low enough rangerate not set to trailer related");
   CHECK_FALSE_TEXT(det_props[1].f_trailer_related_det , "clutter area det with high rangerate set to trailer related");
   CHECK_FALSE_TEXT(det_props[2].f_trailer_related_det , "off-clutter area det with low enough rangerate set to trailer related");
   CHECK_FALSE_TEXT(det_props[3].f_trailer_related_det , "off-clutter area det with higher rangerate set to trailer related");
}

/**
*\purpose
* The purpose of the test is to check the double bounce detection logic. 
* Trailer is at right, i.e, angle < 0, and 2 detections inside the double bounce detection sector.
* 2 detection are off the sector.
* among the pairs, one of them is with high raw range rate, while the other is with low raw range rate.
* The test checks that only the detections inside the double bounce sectors and with low enough raw range rate are marked as trailer detections
* and all ok to use
*/
TEST(f360_mark_trailer_detections, double_bounce_dets_are_handled_correctly)
{
   /** \precond
   * Three pairs of detections with same locations, and only one of them has higher raw range rates. 
   * The trailer partly blocks the left sensor FOV
   **/
   const int8_t trailer_idx = 0;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   trailer_detector_output.trailer_angle[trailer_idx] = F360_DEG2RAD(-5.0F);
   trailer_detector_output.trailer_width[trailer_idx] = 2.0F;
   trailer_detector_output.trailer_length[trailer_idx] = 10.0F;

   trailer_detector_output.joint_position_vcs_long[trailer_idx] = -7.0F;
   trailer_detector_output.joint_position_vcs_lat[trailer_idx] = 0.0F;

   sensors[0].variable.is_valid = true;
   sensors[1].variable.is_valid = true;

   // det1 inside the doublebounce sector, low range rate
   raw_det_list.detections[0].processed.vcs_position_x = -50.0F;
   raw_det_list.detections[0].processed.vcs_position_y = 6.6F;
   raw_det_list.detections[0].raw.sensor_id = RIGHT;
   raw_det_list.detections[0].raw.range_rate = 0.39F;
   det_props[0].f_ok_to_use = true;
   det_props[0].f_trailer_related_det = false;
   det_props[0].vcs_position.x = -50.0F;
   det_props[0].vcs_position.y = 6.6F;

   // det2 inside the doublebounce sector, higher range rate
   raw_det_list.detections[1].processed.vcs_position_x = -50.0F;
   raw_det_list.detections[1].processed.vcs_position_y = 6.6F;
   raw_det_list.detections[1].raw.sensor_id = RIGHT;
   raw_det_list.detections[1].raw.range_rate = 0.41F;
   det_props[1].f_ok_to_use = true;
   det_props[1].f_trailer_related_det = false;
   det_props[1].vcs_position.x = -50.0F;
   det_props[1].vcs_position.y = 6.6F;

   // det3 outside the double bounce sector, lower range rate
   raw_det_list.detections[2].processed.vcs_position_x = -50.0F;
   raw_det_list.detections[2].processed.vcs_position_y = 11.0F;
   raw_det_list.detections[2].raw.sensor_id = RIGHT;
   raw_det_list.detections[2].raw.range_rate = 0.39F;
   det_props[2].f_ok_to_use = true;
   det_props[2].f_trailer_related_det = false;
   det_props[2].vcs_position.x = -50.0F;
   det_props[2].vcs_position.y = 11.0F;

   // det4 outside the double bounce sector, higher range rate
   raw_det_list.detections[3].processed.vcs_position_x = -50.0F;
   raw_det_list.detections[3].processed.vcs_position_y = 11.0F;
   raw_det_list.detections[3].raw.sensor_id = RIGHT;
   raw_det_list.detections[3].raw.range_rate = 0.41F;
   det_props[3].f_ok_to_use = true;
   det_props[3].f_trailer_related_det = false;
   det_props[3].vcs_position.x = -15.0F;
   det_props[3].vcs_position.y = 11.0F;

   host.vcs_speed = 5.0F; // normal host speed

   for(uint8_t i=0U; i<4;i++){
      raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);
      raw_det_list.detections[i].raw.range = F360_Get_Hypotenuse(raw_det_list.detections[i].processed.vcs_position_x, raw_det_list.detections[i].processed.vcs_position_y);
   }

   // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
   }

   Sort_Detections_Vcs_Long(raw_det_list);

   /** \action
   * Execute the function
   **/
  
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_ok_to_use and f_trailer_related_det mark results are correct
   **/
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "double-bounce sector det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "double-bounce sector det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "off-double-bounce sector det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "off-double-bounce sector det not set to OK to use");

   CHECK_TRUE_TEXT(det_props[0].f_trailer_related_det , "double-bounce sector det with low enough rangerate not set to trailer related");
   CHECK_FALSE_TEXT(det_props[1].f_trailer_related_det , "double-bounce sector det with high rangerate set to trailer related");
   CHECK_FALSE_TEXT(det_props[2].f_trailer_related_det , "off-double-bounce sector det with low enough rangerate set to trailer related");
   CHECK_FALSE_TEXT(det_props[3].f_trailer_related_det , "off-double-bounce sector det with higher rangerate set to trailer related");
}

/**
*\purpose
* The purpose of the test is to check the clutter and doublebounce logics are off when trailers are not visible. 
* Trailer is at right, i.e, angle < 0, and 2 detections inside the double bounce detection sector.
* 2 detection are inside the clutter detection sector.
* However, since the radar is on the left side, non of them should be marked by the logic
* The test checks that despite of qualified parameters, all such detections will not be processed by clutter or doublebounce logic
*/
TEST(f360_mark_trailer_detections, disable_clutter_and_double_bounce_logic_when_trailer_not_visible)
{
   /** \precond
   * Two pairs of detections with same locations, and only one of them has higher raw range rates. 
   **/
   const int8_t trailer_idx = 0;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   trailer_detector_output.trailer_angle[trailer_idx] = F360_DEG2RAD(-5.0F);
   trailer_detector_output.trailer_width[trailer_idx] = 2.0F;
   trailer_detector_output.trailer_length[trailer_idx] = 10.0F;

   trailer_detector_output.joint_position_vcs_long[trailer_idx] = -7.0F;
   trailer_detector_output.joint_position_vcs_lat[trailer_idx] = 0.0F;

   sensors[0].variable.is_valid = true;
   sensors[1].variable.is_valid = true;

   // det1 inside the doublebounce sector, low range rate
   raw_det_list.detections[0].processed.vcs_position_x = -50.0F;
   raw_det_list.detections[0].processed.vcs_position_y = 6.6F;
   raw_det_list.detections[0].raw.sensor_id = LEFT;
   raw_det_list.detections[0].raw.range_rate = 0.39F;
   det_props[0].f_ok_to_use = true;
   det_props[0].f_trailer_related_det = false;
   det_props[0].vcs_position.x = -50.0F;
   det_props[0].vcs_position.y = 6.6F;

   // det2 inside the doublebounce sector, higher range rate
   raw_det_list.detections[1].processed.vcs_position_x = -50.0F;
   raw_det_list.detections[1].processed.vcs_position_y = 6.6F;
   raw_det_list.detections[1].raw.sensor_id = LEFT;
   raw_det_list.detections[1].raw.range_rate = 0.41F;
   det_props[1].f_ok_to_use = true;
   det_props[1].f_trailer_related_det = false;
   det_props[1].vcs_position.x = -50.0F;
   det_props[1].vcs_position.y = 6.6F;

   // det3 inside the detected region, low range rate
   raw_det_list.detections[2].processed.vcs_position_x = -15.0F;
   raw_det_list.detections[2].processed.vcs_position_y = -3.0F;
   raw_det_list.detections[2].raw.sensor_id = LEFT;
   raw_det_list.detections[2].raw.range_rate = 0.39F;
   det_props[2].f_ok_to_use = true;
   det_props[2].f_trailer_related_det = false;
   det_props[2].vcs_position.x = -15.0F;
   det_props[2].vcs_position.y = -3.0F;

   // det4 inside the detected region, higher range rate
   raw_det_list.detections[3].processed.vcs_position_x = -15.0F;
   raw_det_list.detections[3].processed.vcs_position_y = -3.0F;
   raw_det_list.detections[3].raw.sensor_id = LEFT;
   raw_det_list.detections[3].raw.range_rate = 0.41F;
   det_props[3].f_ok_to_use = true;
   det_props[3].f_trailer_related_det = false;
   det_props[3].vcs_position.x = -15.0F;
   det_props[3].vcs_position.y = -3.0F;
   host.vcs_speed = 5.0F; // normal host speed

   for(uint8_t i=0U; i<4;i++){
      raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);
      raw_det_list.detections[i].raw.range = F360_Get_Hypotenuse(raw_det_list.detections[i].processed.vcs_position_x, raw_det_list.detections[i].processed.vcs_position_y);
   }

   // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
   }

   Sort_Detections_Vcs_Long(raw_det_list);

   /** \action
   * Execute the function
   **/
  
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_ok_to_use and f_trailer_related_det mark results are correct
   **/
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "Although the logics should be disabled, double-bounce sector det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "Although the logics should be disabled, double-bounce sector det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "Although the logics should be disabled, clutter sector det not set to OK to use");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "Although the logics should be disabled, clutter sector det not set to OK to use");

   CHECK_FALSE_TEXT(det_props[0].f_trailer_related_det , "Although the logics should be disabled, double-bounce sector det with low enough rangerate set to trailer related");
   CHECK_FALSE_TEXT(det_props[1].f_trailer_related_det , "Although the logics should be disabled, double-bounce sector det with high rangerate set to trailer related");
   CHECK_FALSE_TEXT(det_props[2].f_trailer_related_det , "Although the logics should be disabled, clutter area det with low enough rangerate set to trailer related");
   CHECK_FALSE_TEXT(det_props[3].f_trailer_related_det , "Although the logics should be disabled, clutter area det with higher rangerate set to trailer related");
}

/**
*\purpose
* The purpose of the test is to check the occlusion of detections by trailers. 
* Trailer is at left, i.e, angle > 0, and 1 detection to the left is occluded.
* 1 detection to the left and 2 detections to the right are not occluded.
*/
TEST(f360_mark_trailer_detections, find_occlusion_area_cv_trailer_all_sensors_test_trailer_on_left)
{
   /** \precond
   * Two pairs of symmetric detections, and only one of them is occluded by the trailer. 
   * The trailer partly blocks the left sensor FOV
   **/
   const int8_t trailer_idx = 0;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   trailer_detector_output.trailer_angle[trailer_idx] = F360_DEG2RAD(30.0F);
   trailer_detector_output.trailer_width[trailer_idx] = 2.0F;
   trailer_detector_output.trailer_length[trailer_idx] = 10.0F;

   trailer_detector_output.joint_position_vcs_long[trailer_idx] = -7.0F;
   trailer_detector_output.joint_position_vcs_lat[trailer_idx] = 0.0F;

   sensors[0].variable.is_valid = true;
   sensors[1].variable.is_valid = true;

   // Left detection not occluded
   raw_det_list.detections[0].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[0].processed.vcs_position_y = -3.0F;
   raw_det_list.detections[0].raw.sensor_id = LEFT;
   det_props[0].f_ok_to_use = true;
   det_props[0].f_trailer_related_det = false;
   det_props[0].vcs_position.x = -7.0F;
   det_props[0].vcs_position.y = -3.0F;

   // Left detection occluded
   raw_det_list.detections[1].processed.vcs_position_x = -16.0F;
   raw_det_list.detections[1].processed.vcs_position_y = -3.0F;
   raw_det_list.detections[1].raw.sensor_id = LEFT;
   det_props[1].f_ok_to_use = true;
   det_props[1].f_trailer_related_det = false;
   det_props[1].vcs_position.x = -16.0F;
   det_props[1].vcs_position.y = -3.0F;

   // Right detection not occluded
   raw_det_list.detections[2].processed.vcs_position_x = 7.0F;
   raw_det_list.detections[2].processed.vcs_position_y = 3.0F;
   raw_det_list.detections[2].raw.sensor_id = RIGHT;
   det_props[2].f_ok_to_use = true;
   det_props[2].f_trailer_related_det = false;
   det_props[2].vcs_position.x = 7.0F;
   det_props[2].vcs_position.y = 3.0F;

   // Right detection not occluded
   raw_det_list.detections[3].processed.vcs_position_x = -16.0F;
   raw_det_list.detections[3].processed.vcs_position_y =  3.0F;
   raw_det_list.detections[3].raw.sensor_id = RIGHT;
   det_props[3].f_ok_to_use = true;
   det_props[3].f_trailer_related_det = false;
   det_props[3].vcs_position.x = -16.0F;
   det_props[3].vcs_position.y = 3.0F;

   for(uint8_t i=0U; i<5;i++) raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);


   // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
   }

   Sort_Detections_Vcs_Long(raw_det_list);

   /** \action
   * Execute the function
   **/
  
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that occlusion results are correct
   **/
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "Non-Occluded detection not set to OK to use");
   CHECK_FALSE_TEXT(det_props[1].f_ok_to_use , "Occluded detection set to OK to use");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "Non-Occluded detection not set to OK to use");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use , "Non-Occluded detection not set to OK to use");
}

/**
*\purpose
* The purpose of the test is to check the occlusion of detections by trailers. 
* Trailer is at right, i.e, angle < 0, and 1 detection to the right is occluded.
* 1 detection to the right and 2 detections to the left are not occluded.
*/
TEST(f360_mark_trailer_detections, find_occlusion_area_cv_trailer_all_sensors_test_trailer_on_right)
{
   /** \precond
   * Two pairs of symmetric detections, and only one of them is occluded by the trailer. 
   * The trailer partly blocks the right sensor FOV
   **/
   const int8_t trailer_idx = 0;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   trailer_detector_output.trailer_angle[trailer_idx] = F360_DEG2RAD(-30.0F);
   trailer_detector_output.trailer_width[trailer_idx] = 2.0F;
   trailer_detector_output.trailer_length[trailer_idx] = 10.0F;

   trailer_detector_output.joint_position_vcs_long[trailer_idx] = -7.0F;
   trailer_detector_output.joint_position_vcs_lat[trailer_idx] = 0.0F;

   sensors[0].variable.is_valid = true;
   sensors[1].variable.is_valid = true;

   // Left detection not occluded
   raw_det_list.detections[0].processed.vcs_position_x = -7.0F;
   raw_det_list.detections[0].processed.vcs_position_y = -3.0F;
   raw_det_list.detections[0].raw.sensor_id = LEFT;
   det_props[0].f_ok_to_use = true;
   det_props[0].f_trailer_related_det = false;
   det_props[0].vcs_position.x = -7.0F;
   det_props[0].vcs_position.y = -3.0F;

   // Left detection not occluded
   raw_det_list.detections[1].processed.vcs_position_x = -16.0F;
   raw_det_list.detections[1].processed.vcs_position_y = -3.0F;
   raw_det_list.detections[1].raw.sensor_id = LEFT;
   det_props[1].f_ok_to_use = true;
   det_props[1].f_trailer_related_det = false;
   det_props[1].vcs_position.x = -16.0F;
   det_props[1].vcs_position.y = -3.0F;

   // Right detection not occluded
   raw_det_list.detections[2].processed.vcs_position_x = 7.0F;
   raw_det_list.detections[2].processed.vcs_position_y = 3.0F;
   raw_det_list.detections[2].raw.sensor_id = RIGHT;
   det_props[2].f_ok_to_use = true;
   det_props[2].f_trailer_related_det = false;
   det_props[2].vcs_position.x = 7.0F;
   det_props[2].vcs_position.y = 3.0F;

   // Right detection occluded
   raw_det_list.detections[3].processed.vcs_position_x = -16.0F;
   raw_det_list.detections[3].processed.vcs_position_y =  3.0F;
   raw_det_list.detections[3].raw.sensor_id = RIGHT;
   det_props[3].f_ok_to_use = true;
   det_props[3].f_trailer_related_det = false;
   det_props[3].vcs_position.x = -16.0F;
   det_props[3].vcs_position.y = 3.0F;

   for(uint8_t i=0U; i<5;i++) raw_det_list.detections[i].raw.azimuth = Get_Det_Scs_Az(raw_det_list.detections[i], sensors[raw_det_list.detections[3].raw.sensor_id-1]);


   // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
   for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
   {
      raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
      raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
   }

   Sort_Detections_Vcs_Long(raw_det_list);

   /** \action
   * Execute the function
   **/
  
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that occlusion results are correct
   **/
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "Non-Occluded detection not set to OK to use");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use , "Non-Occluded detection not set to OK to use");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use , "Non-Occluded detection not set to OK to use");
   CHECK_FALSE_TEXT(det_props[3].f_ok_to_use , "Occluded detection set to OK to use");
}

/**
*\purpose
* The purpose of the test is to verify the location of detections w.r.t. a line are computed correctly.
* The lines typically represent the trailer edges
*/
TEST(f360_mark_trailer_detections, check_detection_to_left_of_edge_test)
{
   /** \precond
    * The trailer edge is a line : y = 2*x + 3
    * Two detections are picked, one to the left of the line, and the other the right.
   **/

   const float32_t rear_point[2]{-3.0F, -3.0F};
   const float32_t front_point[2]{1.0F, 5.0F};
   Trailer_line line_param;
   const Trailer_line expected_line_param{8.0F, -4.0F, 12.0F};
   const float32_t detection_on_left[2]{1.0F, 3.0F};
   const float32_t detection_on_right[2]{-4.0F, -3.0F};
   const float32_t detection_on_the_line[2]{-4.0F, -5.0F};

   /** \action
   * Execute the function
   **/
   Compute_Line_Parameters(rear_point, front_point, line_param);
   const bool f_right_det_result = Check_Detection_To_Left_of_Line(detection_on_right, line_param);
   const bool f_left_det_result = Check_Detection_To_Left_of_Line(detection_on_left, line_param);
   const bool f_on_the_line_det_result = Check_Detection_To_Left_of_Line(detection_on_the_line, line_param);

   /** \result
   * Check that the computed line parameters are correct.
   * And the detection locations with respect to the lines are determined correctly.
   **/
   DOUBLES_EQUAL_TEXT(expected_line_param.a, line_param.a, 0.1F, "the first line parameter is wrong");
   DOUBLES_EQUAL_TEXT(expected_line_param.b, line_param.b, 0.1F, "the second line parameter is wrong");
   DOUBLES_EQUAL_TEXT(expected_line_param.c, line_param.c, 0.1F, "the third line parameter, is wrong");
   CHECK_TRUE_TEXT(f_left_det_result , "left detection not determined correctly, if left detected the flag is True");
   CHECK_FALSE_TEXT(f_right_det_result , "right detection not determined correctly, if right detected the flag is False");
   CHECK_FALSE_TEXT(f_on_the_line_det_result , "detection on the line not determined correctly, if on the line the flag is False");
}

/**
*\purpose
* The purpose of the test is to verify the flag f_trailer_related_det and f_ok_to_use will
* be updated to the correct status when there is only one detection.
*/
TEST(f360_mark_trailer_detections, detect_det_on_trailer_single_detection)
{
   /** \precond
   * Use default settings
   **/
   raw_det_list.number_of_valid_detections = 1U;

   /** \action
   * Execute the function
   **/
   Mark_Trailer_Dets(calibs, host, raw_det_list, trailer_detector_output, sensors, det_props);

   /** \result
   * Check that f_trailer_related_det and f_ok_to_use is set to expected value.
   **/
   CHECK_FALSE_TEXT(det_props[0].f_trailer_related_det , "f_trailer_related_det not set right");
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use , "f_ok_to_use not set right");
}
/** @}*/
