/** \file
 * This file contains sw qualifications tests for rspp_input_validation
 */

#include "rspp_input_validation.h"
#include "rspp_host.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <cmath>
#include <limits>
#include <cstring>

using namespace rspp_variant_A;

/** \brief
 * Software qualification tests for RSPP input validation functions
 */
TEST_GROUP(rspp_input_validation_qualtest)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   RSPP_Detection_List_T detection_list{};
   Raw_Detection_T raw_detection{};
   ConstantProps_T constant_props{};

   /** \setup
    * Initialize test data with valid baseline values
    */
   TEST_SETUP()
   {
      // Initialize sensor data with valid values
      memset(sensors, 0, sizeof(sensors));
      sensors[0].variable.is_valid = true;
      sensors[0].variable.timestamp_us = 1000000;
      sensors[0].variable.number_of_valid_detections = 1;
      sensors[0].variable.look_id = RSPP_DET_LOOK_ID_0;
      sensors[0].variable.vcs_velocity.longitudinal = 10.0F;
      sensors[0].variable.vcs_velocity.lateral = 0.0F;
      sensors[0].variable.vacs_boresight_az_estimated = 0.0F;
      sensors[0].variable.vacs_boresight_el_estimated = 0.0F;

      // Initialize detection list with valid values
      memset(&detection_list, 0, sizeof(detection_list));
      detection_list.number_of_valid_detections = 1;

      // Initialize raw detection with valid values
      memset(&raw_detection, 0, sizeof(raw_detection));
      raw_detection.range = 50.0F;
      raw_detection.azimuth = 0.1F;
      raw_detection.elevation = 0.05F;
      raw_detection.range_rate = -2.0F;
      raw_detection.sensor_id = 1;
      raw_detection.det_id = 1;

      detection_list.detections[0].raw = raw_detection;

      // Initialize constant properties with valid values
      memset(&constant_props, 0, sizeof(constant_props));
      constant_props.id = 1;
      constant_props.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      constant_props.mounting_position.vcs_position.longitudinal = -1.0F;
      constant_props.mounting_position.vcs_position.lateral = 0.5F;
      constant_props.mounting_position.vcs_position.height = 0.6F;
      constant_props.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
      constant_props.polarity = 1;

      for (int32_t i = 0; i < RSPP_DET_NUM_LOOK_ID; ++i)
      {
         constant_props.fov_min_az_rad[i] = -1.0F;
         constant_props.fov_max_az_rad[i] = 1.0F;
         constant_props.fov_min_el_rad[i] = -0.5F;
         constant_props.fov_max_el_rad[i] = 0.5F;
         constant_props.range_limits[i] = 200.0F;
         constant_props.r_wrapping[i] = 1.0F;
         constant_props.v_wrapping[i] = 30.0F;
         constant_props.min_aliaised_range_rate[i] = -25.0F;
      }
   }

   /** \teardown
    * No specific cleanup needed
    */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test that RSPP_Input_Sensor_Data_Check function accepts correctly typed sensor data
 * structures as specified in interface requirements.
 * \req
 * CPR-4690, CPR-2792
 */
TEST(rspp_input_validation_qualtest, Sensor_Data_Validation_Interface_Requirements)
{
   /** \step{1}
    * Verify sensor data validation interface accepts valid input structures.
    */

   /** \precond
    * Sensor data structures initialized with valid values from test setup.
    */

   /** \action
    * Call RSPP_Input_Sensor_Data_Check function to test that inputs, as specified in requirements, are accepted.
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensors[0].variable);

   /** \result
    * Test should compile, run, and return true for valid sensor data. Interface requirements are satisfied.
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test that RSPP_Check_Input_Detection_Position_Data function accepts correctly typed
 * detection data structures as specified in interface requirements.
 * \req
 * CPR-4690, CPR-2792
 */
TEST(rspp_input_validation_qualtest, Detection_Data_Validation_Interface_Requirements)
{
   /** \step{1}
    * Verify detection data validation interface accepts valid input structures.
    */

   /** \precond
    * Detection data structures initialized with valid values from test setup.
    */

   /** \action
    * Call RSPP_Check_Input_Detection_Position_Data function to test that inputs, as specified in requirements, are accepted.
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(
       raw_detection, constant_props.fov_max_az_rad[0], constant_props.fov_min_az_rad[0]);

   /** \result
    * Test should compile, run, and return true for valid detection data. Interface requirements are satisfied.
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test that RSPP_Check_Sensor_Calibration function accepts correctly typed sensor
 * constant properties structures as specified in interface requirements.
 * \req
 * CPR-4690, CPR-2792
 */
TEST(rspp_input_validation_qualtest, Sensor_Constant_Props_Validation_Interface_Requirements)
{
   /** \step{1}
    * Verify sensor calibration validation interface accepts valid input structures.
    */

   /** \precond
    * Sensor constant properties initialized with valid values from test setup.
    */

   /** \action
    * Call RSPP_Check_Sensor_Calibration function to test that inputs, as specified in requirements, are accepted.
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Test should compile, run, and return true for valid constant properties. Interface requirements are satisfied.
    */
   CHECK_EQUAL(true, result);
}

/** \defgroup  rspp_input_validation_sensor_boundary_qualtest
 *  @{
 */

/** \brief
 * Check correctness of sensor data validation at boundary conditions
 */
TEST_GROUP(rspp_input_validation_sensor_boundary_qualtest)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};

   /** \setup
    * Initialize sensor with boundary valid values
    */
   TEST_SETUP()
   {
      memset(sensors, 0, sizeof(sensors));
      sensors[0].variable.is_valid = true;
      sensors[0].variable.timestamp_us = 1000000;
      sensors[0].variable.number_of_valid_detections = 1; // Minimum valid count to trigger validation
      sensors[0].variable.look_id = RSPP_DET_LOOK_ID_0;
      sensors[0].variable.vcs_velocity.longitudinal = 120.0F;  // Maximum valid velocity
      sensors[0].variable.vcs_velocity.lateral = -120.0F;      // Minimum valid velocity
      sensors[0].variable.vacs_boresight_az_estimated = 3.14F; // Close to PI boundary
      sensors[0].variable.vacs_boresight_el_estimated = 3.14F; // Close to PI boundary
   }
};

/** \purpose
 * Test that sensor data validation correctly accepts maximum valid boundary values
 * for velocity and angle parameters.
 * \req
 * CPR-3845
 */
TEST(rspp_input_validation_sensor_boundary_qualtest, Sensor_Data_Validation_Max_Boundary_Values)
{
   /** \step{1}
    * Verify sensor data validation accepts maximum boundary values.
    */

   /** \precond
    * Sensor data initialized with maximum valid boundary values from test setup
   * (velocity=120.0F, azimuth~PI, elevation~PI).
    */

   /** \action
    * Call RSPP_Input_Sensor_Data_Check with boundary values
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensors[0].variable);

   /** \result
    * Validation should return true for maximum valid boundary values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test that sensor data validation correctly rejects velocity values that exceed
 * the maximum valid boundary (120.0 m/s).
 * \req
 * CPR-3845
 */
TEST(rspp_input_validation_sensor_boundary_qualtest, Sensor_Data_Validation_Exceed_Max_Boundary)
{
   /** \step{1}
    * Verify sensor data validation rejects values exceeding maximum boundary.
    */

   /** \precond
    * Sensor velocity set to 150.0F, exceeding maximum valid value of 120.0F.
    */
   sensors[0].variable.vcs_velocity.longitudinal = 150.0F; // Exceeds 120.0F

   /** \action
    * Call RSPP_Input_Sensor_Data_Check with exceeding values
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensors[0].variable);

   /** \result
    * Validation should return false for values exceeding maximum boundary
    */
   CHECK_EQUAL(false, result);
}

/** @}*/

/** \defgroup  rspp_input_validation_detection_boundary_qualtest
 *  @{
 */

/** \brief
 * Check correctness of detection data validation at boundary conditions
 */
TEST_GROUP(rspp_input_validation_detection_boundary_qualtest)
{
   Raw_Detection_T raw_detection{};
   const float32_t fov_max_az_rad = 3.14159265358979323846F;  // PI
   const float32_t fov_min_az_rad = -3.14159265358979323846F; // -PI

   /** \setup
    * Initialize detection with boundary valid values
    */
   TEST_SETUP()
   {
      memset(&raw_detection, 0, sizeof(raw_detection));
      raw_detection.range = 500.0F;                    // Maximum valid range
      raw_detection.azimuth = fov_max_az_rad;          // At PI boundary
      raw_detection.elevation = 0.5236F;               // Close to 30deg boundary
      raw_detection.range_rate = 128.0F;               // Maximum valid range rate
      raw_detection.sensor_id = MAX_NUMBER_OF_SENSORS; // Maximum valid sensor ID
      raw_detection.det_id = 1;
   }
};

/** \purpose
 * Test that detection data validation correctly accepts maximum valid boundary values
 * for range, azimuth, elevation, and range rate parameters.
 * \req
 * CPR-3845
 */
TEST(rspp_input_validation_detection_boundary_qualtest, Detection_Data_Validation_Max_Boundary_Values)
{
   /** \step{1}
    * Verify detection data validation accepts maximum boundary values.
    */

   /** \precond
    * Detection data initialized with maximum valid boundary values from test setup
    * (range=500.0F, azimuth=FOV max, elevation~30deg, range_rate=128.0F).
    */

   /** \action
    * Call RSPP_Check_Input_Detection_Position_Data with boundary values
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(
       raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Validation should return true for maximum valid boundary values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test that detection data validation correctly accepts minimum valid boundary values
 * for range, azimuth, elevation, and range rate parameters.
 * \req
 * CPR-3845
 */
TEST(rspp_input_validation_detection_boundary_qualtest, Detection_Data_Validation_Min_Boundary_Values)
{
   /** \step{1}
    * Verify detection data validation accepts minimum boundary values.
    */

   /** \precond
    * Detection data set to minimum valid boundary values.
    */
   raw_detection.range = 0.0F;             // Minimum valid range
   raw_detection.azimuth = fov_min_az_rad; // Close to fov_min_az_rad boundary
   raw_detection.elevation = -0.5236F;     // Close to -30deg boundary
   raw_detection.range_rate = -200.0F;     // Minimum valid range rate
   raw_detection.sensor_id = 1;            // Minimum valid sensor ID

   /** \action
    * Call RSPP_Check_Input_Detection_Position_Data with minimum boundary values
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(
       raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Validation should return true for minimum valid boundary values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test that detection metadata validation correctly rejects sensor ID values
 * that exceed the maximum valid sensor count.
 * \req
 * CPR-3845
 */
TEST(rspp_input_validation_detection_boundary_qualtest, Detection_Data_Validation_Invalid_Sensor_ID)
{
   /** \step{1}
    * Verify detection validation rejects invalid sensor ID.
    */

   /** \precond
    * Sensor ID set to MAX_NUMBER_OF_SENSORS + 1, exceeding valid range.
    */
   raw_detection.sensor_id = MAX_NUMBER_OF_SENSORS + 1; // Exceeds maximum

   /** \action
    * Call RSPP_Check_Detection_Meta_Data with invalid sensor ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Validation should return false for invalid sensor ID
    */
   CHECK_EQUAL(false, result);
}

/** @}*/

/** \defgroup  rspp_input_validation_error_handling_qualtest
 *  @{
 */

/** \brief
 * Check correctness of input validation error handling with invalid data
 */
TEST_GROUP(rspp_input_validation_error_handling_qualtest)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   RSPP_Detection_List_T detection_list{};
   ConstantProps_T constant_props{};

   /** \setup
    * Initialize structures with baseline valid values
    */
   TEST_SETUP()
   {
      // Initialize with valid baseline data
      memset(sensors, 0, sizeof(sensors));
      sensors[0].variable.is_valid = true;
      sensors[0].variable.vcs_velocity.longitudinal = 10.0F;
      sensors[0].variable.vcs_velocity.lateral = 0.0F;
      sensors[0].variable.vacs_boresight_az_estimated = 0.0F;
      sensors[0].variable.vacs_boresight_el_estimated = 0.0F;

      memset(&detection_list, 0, sizeof(detection_list));
      detection_list.number_of_valid_detections = 1;
      detection_list.detections[0].raw.sensor_id = 1;
      detection_list.detections[0].raw.det_id = 1;
      detection_list.detections[0].raw.range = 50.0F;
      detection_list.detections[0].raw.azimuth = 0.0F;
      detection_list.detections[0].raw.elevation = 0.0F;
      detection_list.detections[0].raw.range_rate = 0.0F;

      memset(&constant_props, 0, sizeof(constant_props));
      constant_props.id = 0;
      constant_props.polarity = 1;
      constant_props.mounting_position.vcs_position.longitudinal = -1.0F;
      constant_props.mounting_position.vcs_position.lateral = 0.5F;
      constant_props.mounting_position.vcs_position.height = 0.6F;
      constant_props.mounting_position.vcs_boresight_azimuth_angle = 0.0F;

      for (int32_t i = 0; i < RSPP_DET_NUM_LOOK_ID; ++i)
      {
         constant_props.fov_min_az_rad[i] = -1.0F;
         constant_props.fov_max_az_rad[i] = 1.0F;
         constant_props.fov_min_el_rad[i] = -0.5F;
         constant_props.fov_max_el_rad[i] = 0.5F;
         constant_props.range_limits[i] = 200.0F;
         constant_props.r_wrapping[i] = 1.0F;
         constant_props.v_wrapping[i] = 30.0F;
         constant_props.min_aliaised_range_rate[i] = -25.0F;
      }
   }
};

/** \purpose
 * Test that sensor calibration validation correctly rejects FOV configuration
 * where minimum azimuth is greater than or equal to maximum azimuth.
 * \req
 * CPR-3845
 */
TEST(rspp_input_validation_error_handling_qualtest, Constant_Props_Validation_Invalid_FOV_Range)
{
   /** \step{1}
    * Verify sensor calibration validation rejects invalid FOV range configuration.
    */

   /** \precond
    * FOV azimuth configured with min (1.0F) >= max (0.5F), which is invalid.
    */
   constant_props.fov_min_az_rad[0] = 1.0F;
   constant_props.fov_max_az_rad[0] = 0.5F; // max < min

   /** \action
    * Call RSPP_Check_Sensor_Calibration with invalid FOV range
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Validation should return false for invalid FOV range
    */
   CHECK_EQUAL(false, result);
}

/** @}*/
