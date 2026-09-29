/** \file
 This test file marks out unassociated detections that are suspicious based on their azimuth confidence value and elevation angle
 */

#include "rspp_mark_detection_with_low_detection_conf.h"
#include "rspp_host.h"
#include "rspp_constants.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <CppUTest/UtestMacros.h>

/** \defgroup  rspp_mark_detection_with_low_detection_conf_qualtest
 *  @{
 */
using namespace rspp_variant_A;

/** \brief
 *   This test gruop marks out unassociated detections that are suspicious based on their
 *   azimuth confidence value and elevation angle.
 **/
TEST_GROUP(rspp_mark_detection_with_low_detection_conf_qualtest)
{
   /** \setup
    * Nothing to setup in this test group
    **/
   RSPP_Host_T host{};
   TEST_SETUP()
   {
      host.vcs_speed = 3.0F;
   }

   /** \teardown
    * Nothing to teardown in this test group
    **/
   TEST_TEARDOWN()
   {
   }
};

/** \purpose
 * Generate Azimuth confidence level and elevation angle for SRR4 sensor detections (Need to modify the test case once there is requirement update)
 * \req
 * CPR-3860
 */
TEST(rspp_mark_detection_with_low_detection_conf_qualtest, Check_Det_Az_Conf_and_Elevation1)
{
   /** \step{1}
    * Generate Azimuth confidence level and elevation angle for SRR4 seonsor detections (Need to modify the test case once there is requirement update)
    **/

   /** \precond
    * Set up the detection list, sensor calibration,
    * Host list and the expected detection properties.
    **/
   const int32_t number_of_valid_detections = 11;
   RSPP_Detection_T raw_detection_list[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** Initialize local variables**/
   bool expected_f_azimuth_error_stat_mov[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t i = 0U;
   int32_t sid_cur = 0;

   for (i = 0; i < 8; i++)
   {
      raw_detection_list[i].raw.sensor_id = i + 1;
      sid_cur = raw_detection_list[i].raw.sensor_id;
      sensors[sid_cur - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR4_RADAR;
      raw_detection_list[i].raw.range = 10;
      raw_detection_list[i].raw.elevation = 0.01;
      raw_detection_list[i].raw.confid_azimuth = 1;
      raw_detection_list[i].raw.f_super_res = false;
      raw_detection_list[i].processed.range_rate_compensated = 4.0;
      raw_detection_list[i].raw.azimuth = 0.8;

      /**Check for unassociated SRR4 sensor detections that are suspicious based on elevation angle**/
      if (i == 0)
      {
         raw_detection_list[i].raw.elevation = 0.09;
         expected_f_azimuth_error_stat_mov[i] = true;
      }

      /**Check for unassociated SRR4 sensor detections that are suspicious based on elevation angle**/
      else if (i == 1)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].raw.confid_azimuth = 2;
         raw_detection_list[i].processed.range_rate_compensated = 4.0;
         raw_detection_list[i].raw.range = 16.0;
         expected_f_azimuth_error_stat_mov[i] = true;
      }

      /**Check for unassociated SRR4 sensor detections that are suspicious based on range, azimuth confidence**/
      else if (i == 2)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].processed.range_rate_compensated = 5.0;
         raw_detection_list[i].raw.confid_azimuth = 2;
         raw_detection_list[i].raw.azimuth = 1.8;
         raw_detection_list[i].raw.range = 16.0;
         expected_f_azimuth_error_stat_mov[i] = true;
      }

      /**Check for unassociated SRR4 sensor detections that are suspicious based on range, azimuth, confidence value**/
      else if (i == 3)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].processed.range_rate_compensated = 5.0;
         raw_detection_list[i].raw.range = 15;
         raw_detection_list[i].raw.confid_azimuth = 1;
         raw_detection_list[i].raw.azimuth = 0.4;
         raw_detection_list[i].raw.f_super_res = true;
         expected_f_azimuth_error_stat_mov[i] = true;
      }

      /**Check for associated SRR4 sensor valid detections based on f_super_res and azimuth**/
      else if (i == 4)
      {
         raw_detection_list[i].raw.azimuth = 0.2;
         raw_detection_list[i].raw.f_super_res = true;
         expected_f_azimuth_error_stat_mov[i] = false;
      }
      else
      {
         // Do Nothing
      }

      /** \action
       * Call Check_Detection_Azimuth_Confidence_and_Elevation() to check Azimuth confidence level and elevation angle.
       **/
      bool actual_f_azimuth_error_stat_mov[MAX_NUMBER_OF_DETECTIONS] = {};
      for (int det_idx = 0; det_idx < number_of_valid_detections; det_idx++)
      {
         RSPP_Detection_T &current_detection = raw_detection_list[det_idx];
         const int32_t current_sensor_id = current_detection.raw.sensor_id;
         const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];

         actual_f_azimuth_error_stat_mov[det_idx] = RSPP_Mark_Detection_With_Low_Detection_Confidence(
             current_sensor.constant.sensor_type, host.vcs_speed, current_detection.raw,
             current_detection.processed.range_rate_compensated);
      }

      /** \result
       * Check if the confidence level and elevation angle are in range and the results are as expected
       **/
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[0], actual_f_azimuth_error_stat_mov[0]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[1], actual_f_azimuth_error_stat_mov[1]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[2], actual_f_azimuth_error_stat_mov[2]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[3], actual_f_azimuth_error_stat_mov[3]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[4], actual_f_azimuth_error_stat_mov[4]);
   }
}

/** \purpose
 * Generate Azimuth confidence level and elevation angle for SRR5 sensor detections (Need to modify the test case once there is requirement update)
 * \req
 * CPR-3860
 */
TEST(rspp_mark_detection_with_low_detection_conf_qualtest, Check_Det_Az_Conf_and_Elevation2)
{
   /** \step{1}
    * Generate Azimuth confidence level and elevation angle for SRR5 seonsor detections (Need to modify the test case once there is requirement update)
    **/

   /** \precond
    * Set up the detection list, sensor calibration,
    * Host list and the expected detection properties.
    **/
   const int32_t number_of_valid_detections = 11;
   RSPP_Detection_T raw_detection_list[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** Initialize local variables**/
   bool expected_f_azimuth_error_stat_mov[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t i = 0U;
   int32_t sid_cur = 0;

   for (i = 0; i < 8; i++)
   {
      raw_detection_list[i].raw.sensor_id = i + 1;
      sid_cur = raw_detection_list[i].raw.sensor_id;
      sensors[sid_cur - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR5_RADAR;
      raw_detection_list[i].raw.range = 10;
      raw_detection_list[i].raw.elevation = 0.01;
      raw_detection_list[i].raw.confid_azimuth = 1;
      raw_detection_list[i].raw.f_super_res = false;
      raw_detection_list[i].processed.range_rate_compensated = 4.0;
      raw_detection_list[i].raw.azimuth = 0.8;

      /**Check for unassociated SRR5 sensor detections that are suspicious based on elevation angle**/
      if (i == 0)
      {
         raw_detection_list[i].raw.elevation = 0.09;
         expected_f_azimuth_error_stat_mov[i] = true;
      }
      /**Check for unassociated SRR5 sensor detections that are suspicious based on elevation angle**/
      else if (i == 1)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].raw.confid_azimuth = 2;
         raw_detection_list[i].processed.range_rate_compensated = 4.0;
         raw_detection_list[i].raw.range = 16.0;
         expected_f_azimuth_error_stat_mov[i] = true;
      }

      /**Check for unassociated SRR5 sensor detections that are suspicious based on range, azimuth confidence**/
      else if (i == 2)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].processed.range_rate_compensated = 5.0;
         raw_detection_list[i].raw.confid_azimuth = 2;
         raw_detection_list[i].raw.azimuth = 1.8;
         raw_detection_list[i].raw.range = 16.0;
         expected_f_azimuth_error_stat_mov[i] = true;
      }
      /**Check for unassociated SRR5 sensor detections that are suspicious based on range, azimuth, confidence value**/
      else if (i == 3)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].processed.range_rate_compensated = 5.0;
         raw_detection_list[i].raw.range = 15;
         raw_detection_list[i].raw.confid_azimuth = 1;
         raw_detection_list[i].raw.azimuth = 0.4;
         raw_detection_list[i].raw.f_super_res = true;
         expected_f_azimuth_error_stat_mov[i] = true;
      }
      /**Check for associated SRR5 sensor valid detections based on f_super_res and azimuth**/
      else if (i == 4)
      {
         raw_detection_list[i].raw.azimuth = 0.2;
         raw_detection_list[i].raw.f_super_res = true;
         expected_f_azimuth_error_stat_mov[i] = false;
      }
      else
      {
         // Do Nothing
      }
      /** \action
       * Call Check_Detection_Azimuth_Confidence_and_Elevation() to check Azimuth confidence level and elevation angle.
       **/
      bool actual_f_azimuth_error_stat_mov[MAX_NUMBER_OF_DETECTIONS] = {};
      for (int det_idx = 0; det_idx < number_of_valid_detections; det_idx++)
      {
         RSPP_Detection_T &current_detection = raw_detection_list[det_idx];
         const int32_t current_sensor_id = current_detection.raw.sensor_id;
         const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];

         actual_f_azimuth_error_stat_mov[det_idx] = RSPP_Mark_Detection_With_Low_Detection_Confidence(
             current_sensor.constant.sensor_type, host.vcs_speed, current_detection.raw,
             current_detection.processed.range_rate_compensated);
      }

      /** \result
       * Check if the confidence level and elevation angle are in range and the results are as expected
       **/
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[0], actual_f_azimuth_error_stat_mov[0]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[1], actual_f_azimuth_error_stat_mov[1]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[2], actual_f_azimuth_error_stat_mov[2]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[3], actual_f_azimuth_error_stat_mov[3]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[4], actual_f_azimuth_error_stat_mov[4]);
   }
}

/** \purpose
 * Generate Azimuth confidence level and elevation angle for MRR360 sensor detections (Need to modify the test case once there is requirement update)
 * \req
 * CPR-3860
 */
TEST(rspp_mark_detection_with_low_detection_conf_qualtest, Check_Det_Az_Conf_and_Elevation3)
{
   /** \step{1}
    * Generate Azimuth confidence level and elevation angle for F360 sensor detections (Need to modify the test case once there is requirement update)
    **/

   /** \precond
    * Set up the detection list, sensor calibration,
    * Host list and the expected detection properties.
    **/
   const int32_t number_of_valid_detections = 11;
   RSPP_Detection_T raw_detection_list[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** Initialize local variables**/
   bool expected_f_azimuth_error_stat_mov[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t i = 0U;
   int32_t sid_cur = 0;

   for (i = 0; i < 8; i++)
   {
      raw_detection_list[i].raw.sensor_id = i + 1;
      sid_cur = raw_detection_list[i].raw.sensor_id;
      sensors[sid_cur - 1].constant.sensor_type = RSPP_SENSOR_TYPE_MRR360_RADAR;
      raw_detection_list[i].raw.range = 10;
      raw_detection_list[i].raw.elevation = 0.01;
      raw_detection_list[i].raw.confid_azimuth = 1;
      raw_detection_list[i].raw.f_super_res = false;
      raw_detection_list[i].processed.range_rate_compensated = 4.0;
      raw_detection_list[i].raw.azimuth = 0.8;

      /**Check for unassociated MRR360 sensor detections that are suspicious based on elevation angle**/
      if (i == 0)
      {
         raw_detection_list[i].raw.elevation = 0.09;
         expected_f_azimuth_error_stat_mov[i] = true;
      }

      /**Check for unassociated MRR360 sensor detections that are suspicious based on elevation angle**/
      else if (i == 1)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].raw.confid_azimuth = 2;
         raw_detection_list[i].processed.range_rate_compensated = 4.0;
         raw_detection_list[i].raw.range = 16.0;
         expected_f_azimuth_error_stat_mov[i] = true;
      }
      /**Check for unassociated MRR360 sensor detections that are suspicious based on range, azimuth confidence**/
      else if (i == 2)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].processed.range_rate_compensated = 5.0;
         raw_detection_list[i].raw.confid_azimuth = 2;
         raw_detection_list[i].raw.azimuth = 1.8;
         raw_detection_list[i].raw.range = 16.0;
         expected_f_azimuth_error_stat_mov[i] = true;
      }
      /**Check for unassociated MRR360 sensor detections that are suspicious based on range, azimuth, confidence value**/
      else if (i == 3)
      {
         raw_detection_list[i].raw.elevation = 0.07;
         raw_detection_list[i].processed.range_rate_compensated = 5.0;
         raw_detection_list[i].raw.range = 15;
         raw_detection_list[i].raw.confid_azimuth = 1;
         raw_detection_list[i].raw.azimuth = 0.4;
         raw_detection_list[i].raw.f_super_res = true;
         expected_f_azimuth_error_stat_mov[i] = true;
      }
      /**Check for associated MRR360 sensor valid detections based on f_super_res and azimuth**/
      else if (i == 4)
      {
         raw_detection_list[i].raw.azimuth = 0.2;
         raw_detection_list[i].raw.f_super_res = true;
         expected_f_azimuth_error_stat_mov[i] = false;
      }
      else
      {
         // Do Nothing
      }

      /** \action
       * Call Check_Detection_Azimuth_Confidence_and_Elevation() to check Azimuth confidence level and elevation angle.
       **/
      bool actual_f_azimuth_error_stat_mov[MAX_NUMBER_OF_DETECTIONS] = {};
      for (int det_idx = 0; det_idx < number_of_valid_detections; det_idx++)
      {
         RSPP_Detection_T &current_detection = raw_detection_list[det_idx];
         const int32_t current_sensor_id = current_detection.raw.sensor_id;
         const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];

         actual_f_azimuth_error_stat_mov[det_idx] = RSPP_Mark_Detection_With_Low_Detection_Confidence(
             current_sensor.constant.sensor_type, host.vcs_speed, current_detection.raw,
             current_detection.processed.range_rate_compensated);
      }
      /** \result
       * Check if the confidence level and elevation angle are in range and the results are as expected
       **/
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[0], actual_f_azimuth_error_stat_mov[0]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[1], actual_f_azimuth_error_stat_mov[1]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[2], actual_f_azimuth_error_stat_mov[2]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[3], actual_f_azimuth_error_stat_mov[3]);
      CHECK_EQUAL(expected_f_azimuth_error_stat_mov[4], actual_f_azimuth_error_stat_mov[4]);
   }
}

/** \purpose
 * Test that the elevation check is not performed if the f_mrr360_filter_away_big_elev_angle calib is false
 * \req
 * CPR-3860
 */
TEST(rspp_mark_detection_with_low_detection_conf_qualtest, rspp_mark_high_elevation_detection_off)
{
   /** \step{1}
    * Initialize default parameters
    *
    **/
   RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS]{};
   int32_t num_valid_dets;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};

   /** \precond
    * Set host speed to less than min_host_speed so that the first check is not performed.
    *
    * Create 5 detection with raw_detection_list->processed.f_ok_to_use = true
    * 1. Sensor type = MRR3, -6 < elevation angle < 6 deg (0.104719755119660 rad)
    * 2. Sensor type = MRR3, elevation angle > 6 deg (0.104719755119660 rad)
    * 3. Sensor type = SRR5, elevation angle > 6 deg (0.104719755119660 rad)
    * 4. Sensor type = MRR3, elevation angle < -6 deg (-0.104719755119660 rad)
    * 5. Sensor type = SRR5, elevation angle < -6 deg (-0.104719755119660 rad)
    **/

   detections[0].raw.sensor_id = 1;
   detections[0].raw.elevation = 0.05;
   detections[1].raw.sensor_id = 1;
   detections[1].raw.elevation = 0.25;
   detections[2].raw.sensor_id = 2;
   detections[2].raw.elevation = 0.25;
   detections[3].raw.sensor_id = 1;
   detections[3].raw.elevation = -0.25;
   detections[4].raw.sensor_id = 2;
   detections[4].raw.elevation = -0.25;
   num_valid_dets = 5;

   detections[0].processed.f_ok_to_use = true;
   detections[1].processed.f_ok_to_use = true;
   detections[2].processed.f_ok_to_use = true;
   detections[3].processed.f_ok_to_use = true;
   detections[4].processed.f_ok_to_use = true;

   sensors[0].constant.sensor_type = RSPP_SENSOR_TYPE_MRR360_RADAR; // 4
   sensors[1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR5_RADAR;   // 3

   /** \action
    * Run RSPP_Mark_Detection_With_Low_Detection_Confidence
    **/
   for (int det_idx = 0; det_idx < num_valid_dets; det_idx++)
   {
      RSPP_Detection_T &current_detection = detections[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];

      RSPP_Mark_Detection_With_Low_Detection_Confidence(
          current_sensor.constant.sensor_type, host.vcs_speed, current_detection.raw, current_detection.processed.range_rate_compensated);
   }
   /** \result
    * Check that all the detections are still ok to use (even the MRR360 with high elevation)
    **/
   for (int i = 0; i < num_valid_dets; i++)
   {
      CHECK_TRUE(detections[i].processed.f_ok_to_use);
   }
}

/** @}*/
