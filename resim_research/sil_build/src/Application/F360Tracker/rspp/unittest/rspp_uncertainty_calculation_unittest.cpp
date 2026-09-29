/** \file
 * This file contains unit tests for content of rspp_uncertainty_calculation.cpp file
 */

#include "rspp_uncertainty_calculation.h"
#include <CppUTest/TestHarness.h>

// #include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace rspp_variant_A;

/** \defgroup  test_RSPP_Calculate_Detection_Uncertainty
 *  @{
 */

/** \brief
 * Tests for uncertainty calculation functionality including detection uncertainty calculation,
 * initialization functions, and cache management.
 */
TEST_GROUP(test_RSPP_Calculate_Detection_Uncertainty)
{
   // Common test variables
   RSPP_Detection_T detection;
   RSPP_Host_T host;
   VariableProps_T sensor_data;
   RSPP_Sensor_Calib_T sensor_calibration;
   float32_t range_rate_std;

   /** \setup
    * Set up common test parameters for uncertainty calculation
    */
   TEST_SETUP()
   {
      // Initialize detection with typical values
      detection = {};
      detection.raw.sensor_id = 1U; // Valid sensor ID (1-based indexing)
      detection.raw.range = 50.0F;
      detection.raw.azimuth = 0.1F;                // ~5.7 degrees
      detection.processed.vcs_az = 0.1F;           // VCS azimuth
      detection.processed.cos_vcs_az = 0.995F;     // cos(0.1)
      detection.processed.sin_vcs_az = 0.0998F;    // sin(0.1)
      detection.processed.vcs_position_x = 49.75F; // ~50m * cos(0.1)
      detection.processed.vcs_position_y = 4.99F;  // ~50m * sin(0.1)
      detection.raw.range_rate = -2.0F;
      detection.processed.range_rate_compensated = -1.5F;

      // Set up interior FOV (typical values)
      sensor_calibration = {};
      for (uint8_t i = 0U; i < RSPP_DET_NUM_LOOK_ID; ++i)
      {
         sensor_calibration.interior_fov[i] = 0.95F; // 95% of max FOV
      }
      sensor_calibration.vcs_mounting_position.longitudinal = -0.0599999987F;
      sensor_calibration.vcs_mounting_position.lateral = 0.100000001F;

      // Set up sensor calibration
      sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_calibration.v_wrapping[0] = 100.0F; // High velocity wrapping

      // Standard deviation of raw range rate measurement
      range_rate_std = 0.2F; // Typical value

      // Reset uncertainty calculation module
      RSPP_Reset_Uncertainty_Cache();
   }

   /** \teardown
    * Clean up after tests
    */
   TEST_TEARDOWN()
   {
      // No specific cleanup required
   }
};

/** \purpose
 * Test basic uncertainty calculation for a typical detection
 * \req
 * NA
 */
TEST(test_RSPP_Calculate_Detection_Uncertainty, Calculate_Detection_Uncertainty_Basic_Case)
{
   /** \step{1}
    * Verifies that the uncertainty calculation returns a valid positive value for a typical detection.
    */

   /** \precond
    * Use default test setup values - typical detection at moderate range and azimuth
    */
   float32_t std_range_rate_compensated_scm = 0.0F;

   /** \action
    * Calculate detection uncertainty using new interface
    */
   RSPP_Calculate_Detection_Uncertainty(
       detection.raw, detection.processed, host, sensor_data, sensor_calibration, range_rate_std, std_range_rate_compensated_scm);

   /** \result
    * Uncertainty should be calculated and stored in detection.processed.std_range_rate_compensated_scm
    * Should be a reasonable positive value (greater than 0, less than 10)
    */
   CHECK(std_range_rate_compensated_scm > 0.0F);
   CHECK(std_range_rate_compensated_scm < 10.0F);
}

/** \purpose
 * Test uncertainty calculation with high range detection
 * \req
 * NA
 */
TEST(test_RSPP_Calculate_Detection_Uncertainty, Calculate_Detection_Uncertainty_High_Range)
{
   /** \step{1}
    * Calculates uncertainty for a detection at high range and checks that the result is positive and within expected bounds.
    */

   /** \precond
    * Set up detection at high range
    */
   detection.raw.range = 200.0F;                // High range detection
   detection.processed.vcs_position_x = 199.0F; // Approximate x position for high range
   float32_t std_range_rate_compensated_scm = 0.0F;

   /** \action
    * Calculate detection uncertainty
    */
   RSPP_Calculate_Detection_Uncertainty(
       detection.raw, detection.processed, host, sensor_data, sensor_calibration, range_rate_std, std_range_rate_compensated_scm);

   /** \result
    * High range detections should have higher uncertainty
    */
   CHECK(std_range_rate_compensated_scm > 0.0F);
   CHECK(std_range_rate_compensated_scm < 20.0F);
}

/** \purpose
 * Test uncertainty calculation with high azimuth detection
 * \req
 * NA
 */
TEST(test_RSPP_Calculate_Detection_Uncertainty, Calculate_Detection_Uncertainty_High_Azimuth)
{
   /** \step{1}
    * Calculates uncertainty for a detection at high azimuth angle and verifies the result is positive and reasonable.
    */

   /** \precond
    * Set up detection at high azimuth angle
    */
   detection.processed.vcs_az = 0.5F;       // ~28.6 degrees
   detection.processed.cos_vcs_az = 0.878F; // cos(0.5)
   detection.processed.sin_vcs_az = 0.479F; // sin(0.5)
   float32_t std_range_rate_compensated_scm = 0.0F;

   /** \action
    * Calculate detection uncertainty
    */
   RSPP_Calculate_Detection_Uncertainty(
       detection.raw, detection.processed, host, sensor_data, sensor_calibration, range_rate_std, std_range_rate_compensated_scm);

   /** \result
    * High azimuth detections should have calculated uncertainty
    */
   CHECK(std_range_rate_compensated_scm > 0.0F);
   CHECK(std_range_rate_compensated_scm < 15.0F);
}

/** \purpose
 * Test reset function
 * \req
 * NA
 */
TEST(test_RSPP_Calculate_Detection_Uncertainty, Reset_Uncertainty_Cache)
{
   /** \step{1}
    * Verifies that the uncertainty cache reset function can be called and that subsequent calculations still work.
    */

   /** \precond
    * No specific preconditions - function should be callable any time
    */
   float32_t std_range_rate_compensated_scm = 0.0F;

   /** \action
    * Call reset function
    */
   RSPP_Reset_Uncertainty_Cache();

   /** \result
    * Function should complete without error (no return value to check)
    * Subsequent uncertainty calculations should work correctly
    */
   RSPP_Calculate_Detection_Uncertainty(
       detection.raw, detection.processed, host, sensor_data, sensor_calibration, range_rate_std, std_range_rate_compensated_scm);

   CHECK(std_range_rate_compensated_scm > 0.0F);
}

/** \purpose
 * Test cache reset function
 * \req
 * NA
 */
TEST(test_RSPP_Calculate_Detection_Uncertainty, Reset_Uncertainty_Cache_2)
{
   /** \step{1}
    * Ensures that after resetting the cache, uncertainty can be recalculated for a new detection and both results are valid.
    */

   /** \precond
    * Calculate uncertainty for one detection first to populate cache
    */
   float32_t std_range_rate_compensated_scm = 0.0F;
   RSPP_Calculate_Detection_Uncertainty(
       detection.raw, detection.processed, host, sensor_data, sensor_calibration, range_rate_std, std_range_rate_compensated_scm);

   /** \action
    * Reset cache and calculate uncertainty for another detection
    */
   RSPP_Reset_Uncertainty_Cache();

   RSPP_Detection_T detection2 = detection;
   detection2.raw.range = 100.0F;               // Different range
   detection2.processed.vcs_position_x = 99.5F; // Approximate x position
   float32_t std_range_rate_compensated_scm2 = 0.0F;
   RSPP_Calculate_Detection_Uncertainty(
       detection2.raw, detection2.processed, host, sensor_data, sensor_calibration, range_rate_std,
       std_range_rate_compensated_scm2);

   /** \result
    * Both calculations should produce valid uncertainties
    */
   CHECK(std_range_rate_compensated_scm > 0.0F);
   CHECK(std_range_rate_compensated_scm2 > 0.0F);
}

/**
 *\purpose  Test Sensor_Capability_Detections function
 *\req   NA
 */
TEST(test_RSPP_Calculate_Detection_Uncertainty, Test_Sensor_Capability_Detections)
{
   /** \step{1}
    * Tests uncertainty calculation for multiple detections and sensors, verifying expected output values for each.
    */

   /** \precond
    * Setting up input to function and expected output
    **/
   RSPP_Detection_T detection_array[3] = {detection, detection, detection};
   RSPP_Sensor_Calib_T sensor_calibration_array[2] = {sensor_calibration, sensor_calibration};
   VariableProps_T sensor_data_array[2] = {sensor_data, sensor_data};

   detection_array[0].raw.sensor_id = 1;
   detection_array[1].raw.sensor_id = 1;
   detection_array[2].raw.sensor_id = 2;

   host.speed = 34.3173103F;
   host.yaw_rate_rad = -0.00629700907F;
   host.dist_rear_axle_to_vcs_m = 3.45000005F;
   host.rear_cornering_compliance = 0.00529999984F;
   for (int i = 0; i < 2; i++)
   {
      sensor_data_array[i].vcs_velocity.longitudinal = 22.1280212402F;
      sensor_data_array[i].vcs_velocity.lateral = -13.2757692337F;
      sensor_calibration_array[i].vcs_mounting_position.longitudinal = -0.0599999987F;
      sensor_calibration_array[i].vcs_mounting_position.lateral = 0.100000001F;
      sensor_calibration_array[i].fov_min_az_rad[0] = -0.785398185F;
      sensor_calibration_array[i].fov_max_az_rad[0] = 0.785398185F;
      sensor_calibration_array[i].interior_fov[RSPP_DET_LOOK_ID_0] = -0.785398185F;
      sensor_calibration_array[i].interior_fov[RSPP_DET_LOOK_ID_1] = 0.785398185F;
      sensor_calibration_array[i].interior_fov[RSPP_DET_LOOK_ID_2] = -0.785398185F;
      sensor_calibration_array[i].interior_fov[RSPP_DET_LOOK_ID_3] = 0.785398185F;
   }
   range_rate_std = 0.06F;
   for (int i = 0; i < 3; i++)
   {
      detection_array[i].processed.cos_vcs_az = 0.930880785F;
      detection_array[i].processed.sin_vcs_az = -0.365323097F;
      detection_array[i].raw.range = 125.379997F;
      detection_array[i].raw.azimuth = -0.515454471F;
   }

   /** \action
    * Call reset function
    */
   RSPP_Reset_Uncertainty_Cache();

   for (int i = 0; i < 3; i++)
   {
      /** \\action
       * Call Sensor_Capability_Detections function with correct parameter
       **/
      float32_t std_range_rate_compensated_scm = 0.0F;
      RSPP_Calculate_Detection_Uncertainty(
          detection_array[i].raw, detection_array[i].processed, host, sensor_data_array[detection_array[i].raw.sensor_id - 1],
          sensor_calibration_array[detection_array[i].raw.sensor_id - 1], range_rate_std, std_range_rate_compensated_scm);

      /** \\result
       * Check the expected value for std_vcs_az_scm, std_range_rate_compensated_scm ,vcs_position_cov_scm and vcs_cross_covariances_scm
       **/
      DOUBLES_EQUAL(0.2277275, std_range_rate_compensated_scm, 0.0001F)
   }
}

/** @}*/
