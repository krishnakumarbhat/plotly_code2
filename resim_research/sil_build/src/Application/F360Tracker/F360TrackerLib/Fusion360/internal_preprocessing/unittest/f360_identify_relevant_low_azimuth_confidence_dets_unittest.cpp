/** \file
 * This file contains unit tests for content of f360_identify_relevant_low_azimuth_confidence_dets.cpp file
 */

#include "f360_identify_relevant_low_azimuth_confidence_dets.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

#include "f360_clear_detections_props.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_update_detection_property.h"
#include "f360_constants.h"
#include "f360_internal_preprocessing.h"
#include "rspp_look_type.h"
#include "rspp_range_type.h"

using namespace f360_variant_A;


/** @}*/
/** \defgroup  f360_handle_low_az_conf_detections
 *  @{
 */

/** \brief
 * The purpose of this test group is to test the Handle_Low_Az_Conf_Detections()
 * Handle_Low_Az_Conf_Detections() is used for flagging low azimith confidence detections in predefined regions  
 */
TEST_GROUP(f360_handle_low_az_conf_detections)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Set up a detection with each relevant sensor type
    * Set VCS position of dections to be in ROI defined in Handle_Low_Az_Conf_Detections()
    * All detections are ok to use
    * All detections initially have f_low_az_conf_det = false
    * All detections are setup to be flagged as f_low_az_conf_det = true
    */
   TEST_SETUP()
   {
      uint32_t i = 0U;
      uint32_t k = 0U;
      det_props[i].f_ok_to_use = true;
      det_props[i].f_low_az_conf_det = false;
      det_props[i].vcs_position.x = 20.0F;
      det_props[i].vcs_position.y = 0.0F;
      det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      raw_detection_list.detections[i].processed.vcs_az = 0.0F;
      raw_detection_list.detections[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
      raw_detection_list.detections[i].raw.sensor_id = k+1;
      sensors[k].constant.sensor_type = F360_SENSOR_TYPE_FLR7_RADAR;
      i = i+1;
      k = k+1;

      det_props[i].f_ok_to_use = true;
      det_props[i].f_low_az_conf_det = false;
      det_props[i].vcs_position.x = 20.0F;
      det_props[i].vcs_position.y = 0.0F;
      det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      raw_detection_list.detections[i].processed.vcs_az = 0.0F;
      raw_detection_list.detections[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
      raw_detection_list.detections[i].raw.sensor_id = k+1;
      sensors[k].constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
      i = i+1;
      k = k+1;

      det_props[i].f_ok_to_use = true;
      det_props[i].f_low_az_conf_det = false;
      det_props[i].vcs_position.x = -20.0F;
      det_props[i].vcs_position.y = 0.0F;
      det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      raw_detection_list.detections[i].processed.vcs_az = F360_DEG2RAD(180.0F);
      raw_detection_list.detections[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
      raw_detection_list.detections[i].raw.sensor_id = k+1;
      sensors[k].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_RADAR;
      i = i+1;
      k = k+1;

      det_props[i].f_ok_to_use = true;
      det_props[i].f_low_az_conf_det = false;
      det_props[i].vcs_position.x = -20.0F;
      det_props[i].vcs_position.y = 0.0F;
      det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      raw_detection_list.detections[i].processed.vcs_az = F360_DEG2RAD(180.0F);
      raw_detection_list.detections[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
      raw_detection_list.detections[i].raw.sensor_id = k+1;
      sensors[k].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;
      i = i+1;
      k = k+1;

      // check detection with low elevation confidence
      det_props[i].f_ok_to_use = true;
      det_props[i].f_low_az_conf_det = false;
      det_props[i].vcs_position.x = -20.0F;
      det_props[i].vcs_position.y = 0.0F;
      det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      raw_detection_list.detections[i].processed.vcs_az = F360_DEG2RAD(180.0F);
      raw_detection_list.detections[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;
      raw_detection_list.detections[i].raw.confid_elevation = 3;
      raw_detection_list.detections[i].raw.sensor_id = k+1;
      sensors[k].constant.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
      i = i+1;
      k = k+1;
   
      raw_detection_list.number_of_valid_detections = i;
   }
};

/** \purpose
 * This test checks that the detections which are setup in ROI near host in Test_Setup(), are flagged as f_low_az_conf_det = true
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_true_for_dets_in_ROI_near_host)
{
   /** \precond
    * The uses the same setup as Test_Setup()
    * All detections are setup to be flagged as f_low_az_conf_det = true
    */

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[4].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}

/** \purpose
 * This test checks that the detections which are setup in ROI near the host in Test_Setup(), are flagged as f_low_az_conf_det = true
 * using the gen7v2 sensors
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_true_for_gen7v2_dets)
{
   /** \precond
    * Preconditions the same as in Test_Setup()
    * Change the sensor 1 and 3 to be gen7v2 sensors
    * All detections are setup to be flagged as f_low_az_conf_det = true
    */
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR;
   sensors[2].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR;

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}
/** @}*/

/** \purpose
 * This test checks that a detection, which is setup in ROI near host in Test_Setup(), is not flagged as f_low_az_conf_det = true
 * if that detection has f_ok_to_use = false
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_is_not_ok_to_use)
{
   /** \precond
    * 1 detection is setup as not ok to use
    * The nok to use flag would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup to be flagged as f_low_az_conf_det = true
    */
   det_props[0].f_ok_to_use = false;
   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_FALSE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}
/** @}*/

/** \purpose
 * This test checks that a detection, which is setup in ROI near host in Test_Setup(), is not flagged as f_low_az_conf_det = true
 * if that detection's azimuth confidence is not RSPP_CONF_AZIMUTH_LOW
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_does_not_have_low_az_conf)
{
   /** \precond
    * 1 detection is setup with azimuth confidence != RSPP_CONF_AZIMUTH_LOW
    * The above would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup to be flagged as f_low_az_conf_det = true
    */
   raw_detection_list.detections[1].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;
   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_FALSE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}
/** @}*/

/** \purpose
 * This test checks that a detection, which is setup in ROI near host in Test_Setup(), is not flagged as f_low_az_conf_det = true
 * if that detection's motion status is not RSPP_DETECTION_MOTION_STATUS_MOVING
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_does_not_have_moving_motion_status)
{
   /** \precond
    * 1 detection does_not_have_moving_motion_status
    * The above would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup to be flagged as f_low_az_conf_det = true
    */
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_FALSE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}
/** @}*/

/** \purpose
 * This test checks that a detection, which is setup to not be in ROI near host, is not flagged as f_low_az_conf_det = true
 * The detection's lateral position is outside ROI
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_is_laterally_outside_ROI)
{
   /** \precond
    * 1 detection is setup to be laterally outside ROI
    * The above would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup to be flagged as f_low_az_conf_det = true
    */
   det_props[3].vcs_position.y = 8.0F;

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_FALSE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly marked as f_low_az_conf_det.");
}
/** @}*/

/** \purpose
 * This test checks that a detection, which is setup to not be in ROI near host, is not flagged as f_low_az_conf_det = true
 * The detection's longitudinal position is outside ROI. Above max longitudinal position threshold
 * This test is only intended for detections from SRR7plus sensor
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_is_longitudinally_above_ROI)
{
   /** \precond
    * 1 detection is setup to be longitudnially above ROI
    * The above would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup to be flagged as f_low_az_conf_det = true
    */
   det_props[0].vcs_position.x = 31.0F;

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}
/** @}*/

/** \purpose
 * This test checks that a detection, which is setup to not be in ROI near host, is not flagged as f_low_az_conf_det = true
 * The detection's longitudinal position is outside ROI. Below min longitudinal position threshold
 * This test is only for detections from SRR7plus sensor
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_is_longitudinally_below_ROI)
{
   /** \precond
    * 1 detection is setup to be longitudnially below ROI
    * The above would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup to be flagged as f_low_az_conf_det = true
    */
   det_props[1].vcs_position.x = -36.0F;

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}
/** @}*/

/** \purpose
 * This test checks that a detection, which is setup in ROI near host in Test_Setup(), is not flagged as f_low_az_conf_det = true
 * if that detection is from a incompatible sensor type
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_if_det_from_incompatible_sensor)
{
   /** \precond
    * 1 detection is setup with incompatible sensor type
    * The above would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup to be flagged as f_low_az_conf_det = true
    */
   sensors[2].constant.sensor_type = F360_SENSOR_TYPE_SRR6_PLUS_RADAR;
   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_FALSE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}

/** \purpose
 * This test checks that a detection, is flagged as f_low_az_conf_det = true
 * if that detection has low azimuth confidence, and is not in ROI near host, and is not azimuth zone in front of host
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_true_if_det_is_low_az_conf_and_not_in_front_of_host)
{
   /** \precond
    * 1 detection is setup with azimuth not in zone in front or behind host (vcs_az = 45 degrees)
    * The above would lead to the detection being flagged as f_low_az_conf_det = true
    * All other detections are setup in ROI near host, to be flagged as f_low_az_conf_det = true
    */
   det_props[3].f_ok_to_use = true;
   det_props[3].f_low_az_conf_det = false;
   det_props[3].vcs_position.x = 70.0F;
   det_props[3].vcs_position.y = 70.0F;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detection_list.detections[3].processed.vcs_az = F360_DEG2RAD(45.0F);
   raw_detection_list.detections[3].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
   raw_detection_list.detections[3].raw.sensor_id = 4;
   sensors[3].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}

/** \purpose
 * This test checks that a detection, is flagged as f_low_az_conf_det = false
 * if that detection has low azimuth confidence, and is not in ROI near host, and is in azimuth zone in front of host
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_is_low_az_conf_and_in_front_of_host)
{
   /** \precond
    * 1 detection is setup with azimuth in zone in front of host (vcs_az = 0 degrees)
    * The detection is setup to be outside ROI
    * The above would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup in ROI near host, to be flagged as f_low_az_conf_det = true
    */
   det_props[3].f_ok_to_use = true;
   det_props[3].f_low_az_conf_det = false;
   det_props[3].vcs_position.x = 70.0F;
   det_props[3].vcs_position.y = 0.0F;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detection_list.detections[3].processed.vcs_az = F360_DEG2RAD(0.0F);
   raw_detection_list.detections[3].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
   raw_detection_list.detections[3].raw.sensor_id = 4;
   sensors[3].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_FALSE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly marked as f_low_az_conf_det.");
}

/** \purpose
 * This test checks that a detection, is flagged as f_low_az_conf_det = true
 * if that detection has low azimuth confidence, and is not in ROI near host, and is not azimuth zone and not behind host
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_true_if_det_is_low_az_conf_and_not_behind_host)
{
   /** \precond
    * 1 detection is setup with azimuth not in zone in front or behind host (vcs_az = 135 degrees)
    * The above would lead to the detection being flagged as f_low_az_conf_det = true
    * All other detections are setup in ROI near host, to be flagged as f_low_az_conf_det = true
    */
   det_props[3].f_ok_to_use = true;
   det_props[3].f_low_az_conf_det = false;
   det_props[3].vcs_position.x = -70.0F;
   det_props[3].vcs_position.y = 70.0F;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detection_list.detections[3].processed.vcs_az = F360_DEG2RAD(135.0F);
   raw_detection_list.detections[3].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
   raw_detection_list.detections[3].raw.sensor_id = 4;
   sensors[3].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}

/** \purpose
 * This test checks that a detection, is flagged as f_low_az_conf_det = false
 * if that detection has low azimuth confidence, and is not in ROI near host, and is in azimuth zone behind host
 */
TEST(f360_handle_low_az_conf_detections, check_if_f_low_az_conf_det_is_set_to_false_if_det_is_low_az_conf_and_behind_host)
{
   /** \precond
    * 1 detection is setup with azimuth in zone behind host (vcs_az = 180 degrees)
    * The detection is setup to be outside ROI
    * The above would lead to the detection being flagged as f_low_az_conf_det = false
    * All other detections are setup in ROI near host, to be flagged as f_low_az_conf_det = true
    */
   det_props[3].f_ok_to_use = true;
   det_props[3].f_low_az_conf_det = false;
   det_props[3].vcs_position.x = -70.0F;
   det_props[3].vcs_position.y = 0.0F;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detection_list.detections[3].processed.vcs_az = F360_DEG2RAD(180.0F);
   raw_detection_list.detections[3].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
   raw_detection_list.detections[3].raw.sensor_id = 4;
   sensors[3].constant.sensor_type = F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;

   /** \action
    * Call Handle_Low_Az_Conf_Detections() for each detection in the test setup
    */
   for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_rspp_detection = raw_detection_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_rspp_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Handle_Low_Az_Conf_Detections(current_sensor, current_rspp_detection, current_detection_prop);
   }

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE_TEXT(det_props[0].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[1].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_TRUE_TEXT(det_props[2].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
   CHECK_FALSE_TEXT(det_props[3].f_low_az_conf_det, "The detection was incorrectly not marked as f_low_az_conf_det.");
}
/** @}*/
