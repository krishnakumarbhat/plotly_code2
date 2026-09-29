/** \file
   This file tests if we mark out super resolution detection pairs
    whose range and range rates are exactly equal and come from the same sensor
*/

#include "f360_mark_out_det_pairs.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>


/** \defgroup  f360_mark_out_det_pairs
 *  @{
 */
using namespace f360_variant_A;
/** \brief
*  Mark high resoltion detection pairs when their range and range rate are equal from a given sensor
**/
TEST_GROUP(f360_mark_out_det_pairs)
{
   /** \setup
   * Nothing to setup in this test group
   **/
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   rspp_variant_A::RSPP_Detection_T dets[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   int32_t num_valid_dets = 2;
   F360_TRKR_TIMING_INFO_T timing_info = {};

   TEST_SETUP()
   {
      // Initialize detections with equal range and range rate from the same sensor and set their status as moving
      dets[0].raw.range = 10.01;
      dets[1].raw.range = 10.01;
      dets[0].raw.range_rate = 2.01;
      dets[1].raw.range_rate = 2.01;
      dets[0].raw.sensor_id = 2;
      dets[1].raw.sensor_id = 2;
      det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      sensors[1].constant.sensor_type = F360_SENSOR_TYPE_FLR4_RADAR;
   }
};

/**
*\purpose
* The purpose of the test is to verify if the detection pairs
* are marked as pairs for equal range and range rates from the same sensor
*/
TEST(f360_mark_out_det_pairs, mark_dets_equal_range_and_range_rate)
{
   /** \precond
    * Same as in setup
   **/

   /** \action
   *Call Mark_Out_Det_Pairs() to mark the detections as detection_pair
   **/
   Mark_Out_Det_Pairs(dets, num_valid_dets, sensors, det_props, timing_info);

   /** \result
   *check f_det_pair is set to true
   **/
   bool expected_flag = true;
   CHECK_EQUAL(det_props[0].f_det_pair, expected_flag);
   CHECK_EQUAL(det_props[1].f_det_pair, expected_flag);
}

/**
*\purpose
* The purpose of the test is to verify if the detection pairs
* are not marked as pairs for equal range and unequal range rates from the same sensor
*/
TEST(f360_mark_out_det_pairs, mark_dets_equal_range_and_unequal_range_rate)
{
   /** \precond
    * Change range rate of one detection to be different than the other detection
   **/
   dets[1].raw.range_rate = 1.89;

   /** \action
   *Call Mark_Out_Det_Pairs() to mark the detections as detection_pair
   **/
   Mark_Out_Det_Pairs(dets, num_valid_dets, sensors, det_props, timing_info);

   /** \result
   *check f_det_pair is set to false
   **/
   bool expected_flag = false;
   CHECK_EQUAL(det_props[0].f_det_pair, expected_flag);
   CHECK_EQUAL(det_props[1].f_det_pair, expected_flag);
}

/**
*\purpose
* The purpose of the test is to verify if the detection pairs
* are marked as pairs for equal range and range rates from the same sensor.
* However only one detection is moving and the other is not
*/
TEST(f360_mark_out_det_pairs, mark_dets_with_only_one_moving_detection)
{
   /** \precond
    * Change motion status of one detection to be ambiguous while the other is moving
   **/
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   /** \action
   *Call Mark_Out_Det_Pairs() to mark the detections as detection_pair
   **/
   Mark_Out_Det_Pairs(dets, num_valid_dets, sensors, det_props, timing_info);

   /** \result
   *check f_det_pair is set to true for moving detection only
   **/
   bool expected_flag_for_moving = true;
   bool expected_flag_for_ambigous = false;
   CHECK_EQUAL(det_props[0].f_det_pair, expected_flag_for_moving);
   CHECK_EQUAL(det_props[1].f_det_pair, expected_flag_for_ambigous);
}

/**
*\purpose
* The purpose of the test is to verify if the detection pairs
* are not marked as pairs for equal range and range rates
* for detections from different sensors
*/
TEST(f360_mark_out_det_pairs, mark_dets_from_different_sensors)
{
   /** \precond
    * Change sensor id of one detection to be different than the other detection
   **/
   dets[1].raw.sensor_id = 1;

   /** \action
   *Call Mark_Out_Det_Pairs() to mark the detections as detection_pair
   **/
   Mark_Out_Det_Pairs(dets, num_valid_dets, sensors, det_props, timing_info);

   /** \result
   *check f_det_pair is set to false
   **/
   bool expected_flag = false;
   CHECK_EQUAL(det_props[0].f_det_pair, expected_flag);
   CHECK_EQUAL(det_props[1].f_det_pair, expected_flag);
}

/**
*\purpose
* The purpose of the test is to verify if the detections coming from gen7 sensors
* are marked as pairs when having the same range and range rates.
*/
TEST(f360_mark_out_det_pairs, mark_dets_from_gen7_sensors)
{
   /** \precond
    * Change sensor type to each of the gen7 sensors
   **/
   constexpr uint8_t sensor_count = 6;
   const F360_Sensor_Type_T sensors_t[sensor_count] = {
   F360_SENSOR_TYPE_SRR7_PLUS_RADAR,
   F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR,
   F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR,
   F360_SENSOR_TYPE_FLR7_PLT_RADAR,
   F360_SENSOR_TYPE_FLR7_RADAR,
   F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR};
      
   for(uint8_t i = 0; i < sensor_count; i++)
   {
      det_props[0].f_det_pair = false;
      det_props[1].f_det_pair = false;
      sensors[1].constant.sensor_type = sensors_t[i];
   
   /** \action
   *Call Mark_Out_Det_Pairs() to mark the detections as detection_pair
   **/
   Mark_Out_Det_Pairs(dets, num_valid_dets, sensors, det_props, timing_info);

   /** \result
   *check f_det_pair is set properly
   **/
   CHECK_EQUAL(det_props[0].f_det_pair, false);
   CHECK_EQUAL(det_props[1].f_det_pair, true);
   }
}

/**
*\purpose
* The purpose of the test is to verify that only detection with higher azimuth confidence is marked as pair
* when the two detections have the same range and range rates and come from gen7 sensor
*/
TEST(f360_mark_out_det_pairs, mark_higher_conf_det_from_gen7_sensor)
{
   /** \precond
    * Change sensor type to gen7 sensor and set different azimuth confidence for the two detections
   **/
   sensors[1].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   dets[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
   
   /** \action
   *Call Mark_Out_Det_Pairs() to mark the detections as detection_pair
   **/
   Mark_Out_Det_Pairs(dets, num_valid_dets, sensors, det_props, timing_info);

   /** \result
   *check f_det_pair is set properly
   **/
   CHECK_EQUAL(det_props[0].f_det_pair, true);
   CHECK_EQUAL(det_props[1].f_det_pair, false);
}

/**
*\purpose
* The purpose of the test is to verify that when two detections coming from gen7 sensor 
* have the same range and range rate, only the one that has the moving status will be marked as a pair. 
*/
TEST(f360_mark_out_det_pairs, mark_det_pair_from_gen7_sensor_when_only_first_det_is_moving)
{
   /** \precond
    * Set the sensor to any gen7 type for both detections
    * Set the motion status of one detection to be ambiguous while the other is moving
    * Set the azimuth confidence of one detection to be low
   **/
   sensors[1].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   
   /** \action
   *Call Mark_Out_Det_Pairs() to mark the detections as detection_pair
   **/
   Mark_Out_Det_Pairs(dets, num_valid_dets, sensors, det_props, timing_info);

   /** \result
   *check f_det_pair is set properly
   **/
   CHECK_EQUAL(det_props[0].f_det_pair, false);
   CHECK_EQUAL(det_props[1].f_det_pair, true);
}

/**
*\purpose
* The purpose of the test is to verify that when two detections coming from gen7 sensor 
* have the same range and range rate, only the one that has the moving status will be marked as a pair. 
*/
TEST(f360_mark_out_det_pairs, mark_det_pair_from_gen7_sensor_when_only_second_det_is_moving)
{
   /** \precond
    * Set the sensor to any gen7 type for both detections
    * Set the motion status of one detection to be ambiguous while the other is moving
    * Set the azimuth confidence of one detection to be low
   **/
   sensors[1].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   
   /** \action
   *Call Mark_Out_Det_Pairs() to mark the detections as detection_pair
   **/
   Mark_Out_Det_Pairs(dets, num_valid_dets, sensors, det_props, timing_info);

   /** \result
   *check f_det_pair is set properly
   **/
   CHECK_EQUAL(det_props[0].f_det_pair, true);
   CHECK_EQUAL(det_props[1].f_det_pair, false);
}

/** @}*/
