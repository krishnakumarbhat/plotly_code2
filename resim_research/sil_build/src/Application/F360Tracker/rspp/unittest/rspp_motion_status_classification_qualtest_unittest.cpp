/** \file
 * This file contains qualitative unit tests for content of rspp_motion_status_classification.cpp file
 */

#include "rspp_motion_status_classification.h"
#include "rspp_host.h"
#include "rspp_math_func.h"
#include "rspp_detection_motion_status.h"
#include <CppUTest/TestHarness.h>
#include <cfloat>

using namespace rspp_variant_A;

/** \defgroup  rspp_motion_status_classification__stopped_host
 *  @{
 */

/** \brief
 * Check correctness of detection motion status determination when host does not move
 */
TEST_GROUP(rspp_motion_status_classification__stopped_host)
{
   RSPP_Detection_T detection;
   VariableProps_T sensor_data;
   RSPP_Host_T host;
   RSPP_Sensor_Calib_T sensor_calibration;

   /** \setup
    * Initialize calibrations and set up detection with stopped host scenario.
    */
   TEST_SETUP()
   {
      // Initialize detection structure
      detection.raw.sensor_id = 1;

      // Initialize sensor data
      sensor_data.is_valid = true;
      sensor_data.vcs_velocity.longitudinal = 0.0F;
      sensor_data.vcs_velocity.lateral = 0.0F;

      // stopped host
      host.vcs_speed = 0.0F;
      host.curvature_rear = 0.0F;

      // Initialize sensor calibration
      sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR5_RADAR;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/** \purpose
 * Check correctness of detection motion determination when det's range rate is close to 0.0F
 * \req  CPR-3848
 */
TEST(rspp_motion_status_classification__stopped_host, Detection_Motion_Classification__mark_det_as_ambiguous_dealiased_case_1)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = 1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 0;
   detection.raw.range_rate = 0.1F;
   detection.processed.range_rate_compensated = 0.1F; // For stopped host, compensated = raw

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as ambiguous
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Check correctness of detection motion determination when det's range rate is close to 0.0F
 * \req  CPR-3848
 */
TEST(rspp_motion_status_classification__stopped_host, Detection_Motion_Classification__mark_det_as_ambiguous_dealiased_case_2)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = 1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 0;
   detection.raw.range_rate = 0.2F;
   detection.processed.range_rate_compensated = 0.2F; // For stopped host, compensated = raw

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as ambiguous
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Check correctness of detection motion determination when det's range rate is high enough
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__stopped_host, Detection_Motion_Classification__mark_det_as_moving_dealiased_case_1)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = 1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 0;
   detection.raw.range_rate = 2.0F;
   detection.processed.range_rate_compensated = 2.0F; // For stopped host, compensated = raw

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as moving
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Check correctness of detection motion determination when det's conf az is low
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__stopped_host, Detection_Motion_Classification__mark_det_as_amb_due_to_low_conf_az)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = 1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 3; // Low confidence
   detection.raw.range_rate = 1.0F;
   detection.processed.range_rate_compensated = 1.0F; // For stopped host, compensated = raw

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as ambiguous
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** @}*/

/** \defgroup  rspp_motion_status_classification__moving_forward_host
 *  @{
 */

/** \brief
 * Check correctness of detection motion status determination when host moves forward (speed = 10 m/s)
 */
TEST_GROUP(rspp_motion_status_classification__moving_forward_host)
{
   RSPP_Detection_T detection;
   VariableProps_T sensor_data;
   RSPP_Host_T host;
   RSPP_Sensor_Calib_T sensor_calibration;

   /** \setup
    * Initialize calibrations and set up detection with forward-moving host (10 m/s).
    */
   TEST_SETUP()
   {
      // Initialize detection structure
      detection.raw.sensor_id = 1;

      // Initialize sensor data
      sensor_data.is_valid = true;
      sensor_data.vcs_velocity.longitudinal = 10.0F;
      sensor_data.vcs_velocity.lateral = 0.0F;

      host.vcs_speed = 10.0F;
      host.curvature_rear = 0.0F;

      // Initialize sensor calibration
      sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR5_RADAR;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/** \purpose
 * Check correctness of detection motion determination when detection is marked as f_azimuth_error_stat_mov
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__moving_forward_host, Detection_Motion_Classification__mark_det_as_amb_due_to_being_az_error_stat)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = 1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.range_rate = -10.0F;
   detection.raw.elevation = 0.1F;
   detection.processed.range_rate_compensated = 0.0F; // Compensated value after ego motion compensation
   host.vcs_speed = 10.0F;

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as ambiguous
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Check correctness of detection motion determination when det's range rate is close to -10.0F and relative high motion threshold
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__moving_forward_host, Detection_Motion_Classification__mark_det_as_ambiguous_dealiased_case_1)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = 1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 0;
   detection.raw.range_rate = -10.1F;
   detection.processed.range_rate_compensated = -0.1F; // Small residual after ego motion compensation

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as ambiguous
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Check correctness of detection motion determination when det's range rate is close to -10.0F and relative low motion threshold
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__moving_forward_host, Detection_Motion_Classification__mark_det_as_ambiguous_dealiased_case_2)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = 1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 0;
   detection.raw.range_rate = -10.2F;
   detection.processed.range_rate_compensated = -0.2F; // Small residual after ego motion compensation

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as ambiguous
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Check correctness of detection motion determination when det's range rate is high enough
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__moving_forward_host, Detection_Motion_Classification__mark_det_as_moving_dealiased_case_1)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = 1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 0;
   detection.raw.range_rate = 2.0F;
   detection.processed.range_rate_compensated = 12.0F; // High compensated range rate indicating moving object

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as moving
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}
/** @}*/

/** \defgroup  rspp_motion_status_classification__fast_moving_forward_host
 *  @{
 */

/** \brief
 * Check correctness of detection motion status determination when host moves forward with high speed (speed = 30 m/s).
 * Predicted range rate is then high enough to try to dealias detection.
 */
TEST_GROUP(rspp_motion_status_classification__fast_moving_forward_host)
{
   RSPP_Detection_T detection;
   VariableProps_T sensor_data;
   RSPP_Host_T host;
   RSPP_Sensor_Calib_T sensor_calibration;

   /** \setup
    * Initialize calibrations and set up detection with fast forward-moving host (30 m/s).
    */
   TEST_SETUP()
   {
      // Initialize detection structure
      detection.raw.sensor_id = 1;

      // Initialize sensor data
      sensor_data.is_valid = true;
      sensor_data.vcs_velocity.longitudinal = 30.0F;
      sensor_data.vcs_velocity.lateral = 0.0F;

      host.vcs_speed = 30.0F; // Fast moving host
      host.curvature_rear = 0.0F;

      // Initialize sensor calibration
      sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR5_RADAR;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/** \purpose
 * Check correctness of detection motion determination when det's range rate is high enough for moving classification
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__fast_moving_forward_host, Detection_Motion_Classification__aliased_moving_det)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = -1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 0;
   detection.raw.range_rate = -20.0F;                  // Aliased range rate
   detection.processed.range_rate_compensated = 10.0F; // High compensated range rate after dealiasing

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as moving
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Check correctness of detection motion determination when det's compensated range rate is close to zero
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__fast_moving_forward_host, Detection_Motion_Classification__aliased_amb_det)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * NA
    */
   detection.processed.cos_vcs_az = -1.0F;
   detection.processed.sin_vcs_az = 0.0F;
   detection.raw.confid_azimuth = 0;
   detection.raw.range_rate = -30.0F;                 // Aliased range rate
   detection.processed.range_rate_compensated = 0.0F; // Zero compensated range rate after dealiasing

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Detection marked as ambiguous
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Check correctness of detection motion classification with pre-calculated compensated range rate.
 * This test verifies the motion classification logic when compensated range rate is already available.
 * \req CPR-3848
 */
TEST(rspp_motion_status_classification__fast_moving_forward_host, Detection_Motion_Classification_With_Compensated_Range_Rate)
{
   /** \step{1}
    * Execute test and verify motion status classification.
    */

   /** \precond
    * Set up a detection with specific compensated range rate
    */
   const float32_t det_az = 0.3F;
   detection.processed.cos_vcs_az = RSPP_Cosf(det_az);
   detection.processed.sin_vcs_az = RSPP_Sinf(det_az);
   detection.raw.range_rate = -10.0F;
   detection.processed.range_rate_compensated = 18.6601F; // High compensated range rate

   /** \action
    * Call RSPP_Calculate_Motion_Status().
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Check that detection is classified as moving due to high compensated range rate.
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}
/** @}*/
