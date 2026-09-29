/** \file
 This test file marks out unassociated detections that are suspicious based on their azimuth confidence value and elevation angle
 */

#include "f360_mark_high_elevation_detection.h"
#include "f360_host.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <CppUTest/UtestMacros.h>

/** \defgroup  f360_mark_high_elevation_detection_qualtest
 *  @{
 */
using namespace f360_variant_A;

/** \brief
 *   This test gruop marks out unassociated detections that are suspicious based on their
 *   azimuth confidence value and elevation angle.
 **/
TEST_GROUP(f360_mark_high_elevation_detection_qualtest)
{
   /** \setup
    * Nothing to setup in this test group
    **/
   F360_Calibrations_T f360_calib{};
   F360_Host_T host{};
   TEST_SETUP()
   {

      Initialize_Tracker_Calibrations(f360_calib);
      host.vcs_speed = 3.0F;
   }

   /** \teardown
    * Nothing to teardown in this test group
    **/
   TEST_TEARDOWN()
   {
   }
};

/**
 *\purpose If the confidence value is too low (3 being the lowest) or the
 * elevation angle is suspiciously high, do not allow detection to start a
 * new track.
 *
 *\req CPR-3860
 */
TEST(f360_mark_high_elevation_detection_qualtest, Check_Det_Az_Conf_and_Elevation)
{
   /** \step{1}
    * Generate Azimuth confidence level and elevation angle for MRR3 seonsor detections (Need to modify the test case once there is requirement update)
    **/

   /** \precond
    * Set up the detection list, sensor calibration,
    * Host list and the expected detection properties.
    **/

   const int32_t number_of_valid_detections = 3;
   rspp_variant_A::RSPP_Detection_T raw_detection_list[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** Initialize local variables**/
   bool expected_f_ok_to_use[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t i = 0U;
   int32_t sid_cur = 0;

   for (i = 0; i < 3; i++)
   {
      raw_detection_list[i].raw.sensor_id = i+1;
      sid_cur = raw_detection_list[i].raw.sensor_id;
      sensors[sid_cur - 1].constant.sensor_type = F360_SENSOR_TYPE_MRR3_RADAR;
      raw_detection_list[i].raw.range = 10;
      raw_detection_list[i].raw.elevation = 0.01;
      raw_detection_list[i].raw.confid_azimuth = 1;
      raw_detection_list[i].raw.f_super_res = false;
      raw_detection_list[i].raw.azimuth = 0.8;
      expected_f_ok_to_use[i] = true;
      /**confidence value is too low or the elevation angle is high,new track is not started.**/
      if (i == 0)
      {
         raw_detection_list[i].raw.confid_azimuth = 2;
         raw_detection_list[i].raw.range = 20.0;
         raw_detection_list[i].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         raw_detection_list[i].raw.elevation = 0.096;
         expected_f_ok_to_use[i] = false;
      }
      /**confidence value is lower boundary value and the elevation angle is low, new track is not started.**/
      else if (i == 1)
      {
         raw_detection_list[i].raw.confid_azimuth = 3;
         raw_detection_list[i].raw.range = 20.0;
         raw_detection_list[i].raw.elevation = 0.09;
         raw_detection_list[i].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         expected_f_ok_to_use[i] = false;
      }
      /**confidence value is lower boundary value or the elevation angle is too high,new track is not started.**/
      else if (i == 2)
      {
         raw_detection_list[i].raw.confid_azimuth = 3;
         raw_detection_list[i].raw.range = 20.0;
         raw_detection_list[i].raw.elevation = 0.097;
         raw_detection_list[i].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         expected_f_ok_to_use[i] = false;
      }
      else
      {
         //Do Nothing
      }
   }

   /** \action
    * Call Check_Detection_Azimuth_Confidence_and_Elevation() to check Azimuth confidence level and elevation angle.
    **/
   for (int det_idx = 0; det_idx < number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_detection_list[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Detection_Props_T &current_detection_prop = detection_props[det_idx];

      Mark_High_Elevation_Detection(current_sensor, f360_calib, host.vcs_speed, current_detection, current_detection_prop);
   }


   /** \result
    * Check if the confidence level and elevation angle are in range and the results are as expected
    **/
   CHECK_EQUAL(expected_f_ok_to_use[0], detection_props[0].f_ok_to_use);
   CHECK_EQUAL(expected_f_ok_to_use[1], detection_props[1].f_ok_to_use);
   CHECK_EQUAL(expected_f_ok_to_use[2], detection_props[2].f_ok_to_use);
}

/** @}*/
