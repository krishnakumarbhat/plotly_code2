/** \file
 * This file contains unit tests for content of f360_update_global_parameters.cpp file
 */

#include "f360_update_global_parameters.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  Update_Global_Parameters
 *  @{
 */

/** \brief
 * Test group dedicated to verifying main function Update_Global_Parameters calls the correct sub functions
 */
TEST_GROUP(Update_Global_Parameters)
{
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibrations = {};
   F360_Globals_T globals = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   /** \setup
     * Set host.vcs_speed = 9.0F
     */
   TEST_SETUP()
   {
      host.vcs_speed = 9.0F;
   }
};

/** \purpose
 * Verify the function Calc_Obj_Mov_Stat_Thresh is called by verifying that globals.obj_mov_stat_spd_thresh
 * is updated.
 * \req
 * NA
 */
TEST(Update_Global_Parameters, Check_Calc_Obj_Mov_Stat_Thresh_Is_Called)
{
   /** \precond
     * None.
     */

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Verify globals.obj_mov_stat_spd_thresh = 1.4
    */
   DOUBLES_EQUAL_TEXT(1.4F, globals.obj_mov_stat_spd_thresh, F360_EPSILON, "obj_mov_stat_spd_thresh was not updated as expected")
}
/** @}*/

/** \defgroup  Update_Global_Parameters__Calculate_Shrinked_FOV_Normals
 *  @{
 */

/** \brief
 * This test group checks the functionality of Calculate_Shrinked_FOV_Normals to ensure that it correctly
 * rotates the FoV normals (right/left_fov_normal_lr) found in sensor properties according to the calibration value
 * k_fov_normal_rotation_angle
 */
TEST_GROUP(Update_Global_Parameters__Calculate_Shrinked_FOV_Normals)
{
   F360_Host_T host = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibrations = {};
   F360_Globals_T globals = {};
   const uint32_t valid_sensor_idx1 = 0;
   const uint32_t valid_sensor_idx2 = MAX_NUMBER_OF_SENSORS - 1;

   /** \setup
    * Set calibrations.k_fov_normal_rotation_angle to 0
    * For all sensors, set
    * - is_valid is valid to false
    * - left_fov_normal[F360_LOOK_ID_0] = 1.11F
    * - left_fov_normal[F360_LOOK_ID_1] = 1.12F
    * - right_fov_normal[F360_LOOK_ID_0] = 1.13F
    * - right_fov_normal[F360_LOOK_ID_1] = 1.14F
    * - left_fov_normal[F360_LOOK_ID_2] = 1.21F
    * - left_fov_normal[F360_LOOK_ID_3] = 1.22F
    * - right_fov_normal[F360_LOOK_ID_2] = 1.23F
    * - right_fov_normal[F360_LOOK_ID_3] = 1.24F
    * - rotated_left_fov_normal_lr[0] and rotated_left_fov_normal_lr[1] to 0.9
    * - rotated_right_fov_normal_lr[0] and rotated_right_fov_normal_lr[1] to 0.9
    */
   TEST_SETUP()
   {
      calibrations.k_fov_normal_rotation_angle = 0.0F;

      for (uint8_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         sensors[sensor_idx].variable.is_valid = false;
         sensors[sensor_idx].variable.look_id = F360_DET_LOOK_ID_INVALID;

         // Long range look
         sensors[sensor_idx].refined.left_fov_normal[F360_DET_LOOK_ID_0] = 1.11F;
         sensors[sensor_idx].refined.left_fov_normal[F360_DET_LOOK_ID_1] = 1.12F;
         sensors[sensor_idx].refined.right_fov_normal[F360_DET_LOOK_ID_0] = 1.13F;
         sensors[sensor_idx].refined.right_fov_normal[F360_DET_LOOK_ID_1] = 1.14F;

         // Medium range look
         sensors[sensor_idx].refined.left_fov_normal[F360_DET_LOOK_ID_2] = 1.21F;
         sensors[sensor_idx].refined.left_fov_normal[F360_DET_LOOK_ID_3] = 1.22F;
         sensors[sensor_idx].refined.right_fov_normal[F360_DET_LOOK_ID_2] = 1.23F;
         sensors[sensor_idx].refined.right_fov_normal[F360_DET_LOOK_ID_3] = 1.24F;

         globals.rotated_left_fov_normal[sensor_idx][0] = 0.9F;
         globals.rotated_left_fov_normal[sensor_idx][1] = 0.9F;

         globals.rotated_right_fov_normal[sensor_idx][0] = 0.9F;
         globals.rotated_right_fov_normal[sensor_idx][1] = 0.9F;
      }
   }
};

/** \purpose
 * Verify that if no sensor is valid, no entry in globals is changed compared to the test setup
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Calculate_Shrinked_FOV_Normals, No_Valid_Sensors)
{
   /** \precond
    * None.
    */

   /** \action
    * Call Update_Global_Parameters
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Verify nothing has changed since the test setup
    */
      for (uint8_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_left_fov_normal[sensor_idx][0], F360_EPSILON, "This value was changed when it was expected no to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_left_fov_normal[sensor_idx][1], F360_EPSILON, "This value was changed when it was expected no to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_right_fov_normal[sensor_idx][0], F360_EPSILON, "This value was changed when it was expected no to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_right_fov_normal[sensor_idx][1], F360_EPSILON, "This value was changed when it was expected no to")
      }
}

/** \purpose
 * Verify that only if a sensor is valid, the rotated FOV normals in globals are set to the
 * corresponding FOV normal in sensors during a long range look
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Calculate_Shrinked_FOV_Normals, Valid_Sensors_No_Rotation_Long_Range_Look)
{
   /** \precond
    * Set is_valid for sensor_idx 1 and 4 to true
    */
   const uint8_t valid_sensor_idx_1 = 1U;
   const uint8_t valid_sensor_idx_2 = 4U;
   sensors[valid_sensor_idx_1].variable.is_valid = true;
   sensors[valid_sensor_idx_2].variable.is_valid = true;
   sensors[valid_sensor_idx_1].variable.look_id = F360_DET_LOOK_ID_0; // Long range look
   sensors[valid_sensor_idx_2].variable.look_id = F360_DET_LOOK_ID_1;

   /** \action
    * Call Update_Global_Parameters
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Verify that sensor_idx 1 and 4,
    * - globals.rotated_left_fov_normal[sensor_idx][0]
    * - globals.rotated_left_fov_normal[sensor_idx][1]
    * - globals.rotated_right_fov_normal[sensor_idx][0]
    * - globals.rotated_right_fov_normal[sensor_idx][1]
    * are set to the same value as the corresponding normal in sensors.
    * For all other sensor_idx, the value should be unchanged from the setup.
    */
   for (uint8_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
   {
      if ((valid_sensor_idx_1 == sensor_idx) || ( valid_sensor_idx_2 == sensor_idx))
      {
         DOUBLES_EQUAL_TEXT(1.11F, globals.rotated_left_fov_normal[sensor_idx][0], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(1.12F, globals.rotated_left_fov_normal[sensor_idx][1], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(1.13F, globals.rotated_right_fov_normal[sensor_idx][0], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(1.14F, globals.rotated_right_fov_normal[sensor_idx][1], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
      }
      else
      {
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_left_fov_normal[sensor_idx][0], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_left_fov_normal[sensor_idx][1], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_right_fov_normal[sensor_idx][0], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_right_fov_normal[sensor_idx][1], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
      }
   }
}

/** \purpose
 * Verify that only if a sensor is valid, the rotated FOV normals in globals are set to the
 * corresponding FOV normal in sensors during a medium range look
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Calculate_Shrinked_FOV_Normals, Valid_Sensors_No_Rotation_Medium_Range_Look)
{
   /** \precond
    * Set is_valid for sensor_idx 1 and 4 to true
    */
   const uint8_t valid_sensor_idx_1 = 1U;
   const uint8_t valid_sensor_idx_2 = 4U;
   sensors[valid_sensor_idx_1].variable.is_valid = true;
   sensors[valid_sensor_idx_2].variable.is_valid = true;
   sensors[valid_sensor_idx_1].variable.look_id = F360_DET_LOOK_ID_2; // Medium range look
   sensors[valid_sensor_idx_2].variable.look_id = F360_DET_LOOK_ID_3;

   /** \action
    * Call Update_Global_Parameters
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Verify that sensor_idx 1 and 4,
    * - globals.rotated_left_fov_normal[sensor_idx][0]
    * - globals.rotated_left_fov_normal[sensor_idx][1]
    * - globals.rotated_right_fov_normal[sensor_idx][0]
    * - globals.rotated_right_fov_normal[sensor_idx][1]
    * are set to the same value as the corresponding normal in sensors.
    * For all other sensor_idx, the value should be unchanged from the setup.
    */
   for (uint8_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
   {
      if ((valid_sensor_idx_1 == sensor_idx) || ( valid_sensor_idx_2 == sensor_idx))
      {
         DOUBLES_EQUAL_TEXT(1.21F, globals.rotated_left_fov_normal[sensor_idx][0], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(1.22F, globals.rotated_left_fov_normal[sensor_idx][1], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(1.23F, globals.rotated_right_fov_normal[sensor_idx][0], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(1.24F, globals.rotated_right_fov_normal[sensor_idx][1], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
      }
      else
      {
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_left_fov_normal[sensor_idx][0], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_left_fov_normal[sensor_idx][1], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_right_fov_normal[sensor_idx][0], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
         DOUBLES_EQUAL_TEXT(0.9F, globals.rotated_right_fov_normal[sensor_idx][1], F360_EPSILON, "This value was not set to the the same value as the corresponding value in sensor_props when it was expected to")
      }
   }
}

/** \purpose
 * Verify that if a sensor is valid and rotation is made, the rotated FOV normals in globals are set to the
 * correct values
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Calculate_Shrinked_FOV_Normals, Valid_Sensor_Rotation)
{
   /** \precond
    * calibrations.k_fov_normal_rotation_angle to rotate 30 degrees
    */
   calibrations.k_fov_normal_rotation_angle = F360_DEG2RAD(30.0F);

   uint8_t valid_sensor = 1U;
   sensors[valid_sensor].variable.is_valid = true;
   sensors[valid_sensor].refined.left_fov_normal[0] = 1.0F; // unit vector pointing vcs 0 deg
   sensors[valid_sensor].refined.left_fov_normal[1] = 0.0F;
   sensors[valid_sensor].refined.right_fov_normal[0] = 1.0F;
   sensors[valid_sensor].refined.right_fov_normal[1] = 0.0F;
   sensors[valid_sensor].variable.look_id = F360_DET_LOOK_ID_0; // Long range look

   const float32_t exp_left_fov_normal_lr_x = 0.866025403784439F; // Unit vector pointing vcs 30 deg
   const float32_t exp_left_fov_normal_lr_y = 0.5F;

   const float32_t exp_right_fov_normal_lr_x = 0.866025403784439F; // unit vector pointing vcs -30 deg
   const float32_t exp_right_fov_normal_lr_y = -0.5F;

   /** \action
    * Call Update_Global_Parameters
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Verify that globals.rotated_left_fov_normal[valid_sensor][0] is set to exp_left_fov_normal_lr_x
    * Verify that globals.rotated_left_fov_normal[valid_sensor][1] is set to exp_left_fov_normal_lr_y
    * Verify that globals.rotated_right_fov_normal[valid_sensor][0] is set to exp_right_fov_normal_lr_x
    * Verify that globals.rotated_right_fov_normal[valid_sensor][1] is set to exp_right_fov_normal_lr_y
    */
   DOUBLES_EQUAL_TEXT(exp_left_fov_normal_lr_x, globals.rotated_left_fov_normal[valid_sensor][0], F360_EPSILON, "The vector was not rotated as expected")
   DOUBLES_EQUAL_TEXT(exp_left_fov_normal_lr_y, globals.rotated_left_fov_normal[valid_sensor][1], F360_EPSILON, "The vector was not rotated as expected")
   DOUBLES_EQUAL_TEXT(exp_right_fov_normal_lr_x, globals.rotated_right_fov_normal[valid_sensor][0], F360_EPSILON, "The vector was not rotated as expected")
   DOUBLES_EQUAL_TEXT(exp_right_fov_normal_lr_y, globals.rotated_right_fov_normal[valid_sensor][1], F360_EPSILON, "The vector was not rotated as expected")

}
/** @}*/


/** \defgroup Update_Global_Parameters__Check_Sensor_Configuration
 *  @{
 */

/** \brief
 * This test group verifies the functionality of the function Check_Sensor_Configuration() to ensure that this function
 * only sets f_single_front_center_radar_only to true when sensors are set up such that it matches a
 * single sensor that is mounted at F360_MOUNTING_LOCATION_CENTER_FORWARD.
 */
TEST_GROUP(Update_Global_Parameters__Check_Sensor_Configuration)
{
   F360_Host_T host = {};
   F360_Calibrations_T calibrations = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};

   /** \setup
    * Set is_valid flag for all sensors to false.
    * Set mounting location for all sensors to F360_MOUNTING_LOCATION_UNKNOWN
    * Set f_single_front_center_radar_only to false.
    */
   TEST_SETUP()
   {
      for (F360_Radar_Sensor_T& sensor : sensors)
      {
         sensor.variable.is_valid = false;
         sensor.constant.mounting_location = F360_MOUNTING_LOCATION_UNKNOWN;
      }
      globals.f_single_front_center_radar_only = false;
   }
};

/** \purpose
 * Verify Check_Sensor_Configuration doesn't set f_single_front_center_radar_only to true when there are no valid sensors
 * and none of the sensors are mounted in F360_MOUNTING_LOCATION_CENTER_FORWARD.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, No_Valid_Sensors_And_Not_Mounted_In_Center)
{
   /** \precond
    * None.
    */

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that globals.f_single_front_center_radar_only is set to false
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only,"f_single_front_center_radar_only was set to true when it was expected to be false.")
}

/** \purpose
 * Verify Check_Sensor_Configuration doesn't set f_single_front_center_radar_only to true when there are no valid sensors
 * but all of the sensors are mounted in F360_MOUNTING_LOCATION_CENTER_FORWARD.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, No_Valid_Sensors_And_Mounted_In_Center)
{
   /** \precond
    * Set mounting_location to F360_MOUNTING_LOCATION_CENTER_FORWARD for all sensors in sensors.
    */
   for (F360_Radar_Sensor_T& sensor : sensors)
   {
      sensor.constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
   }

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that globals.f_single_front_center_radar_only is set to false
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only,"f_single_front_center_radar_only was set to true when it was expected to be false.")
}

/** \purpose
 * Verify Check_Sensor_Configuration doesn't set f_single_front_center_radar_only to true when all sensors are valid and
 * all of the sensors are mounted in F360_MOUNTING_LOCATION_CENTER_FORWARD.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, All_Sensors_Valid_And_Mounted_In_Center)
{
   /** \precond
    * Set mounting_location to F360_MOUNTING_LOCATION_CENTER_FORWARD for all sensors in sensors.
    * Set is_valid to true for all sensors in sensors.
    */
   for (F360_Radar_Sensor_T& sensor : sensors)
   {
      sensor.constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
      sensor.variable.is_valid = true;
   }

    /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that globals.f_single_front_center_radar_only is set to false
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only,"f_single_front_center_radar_only was set to true when it was expected to be false.")
}

/** \purpose
 * Verify Check_Sensor_Configuration doesn't set f_single_front_center_radar_only to true when all sensors are valid but
 * none of the sensors are mounted in F360_MOUNTING_LOCATION_CENTER_FORWARD.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, All_Sensors_Valid_But_Not_Mounted_In_Center)
{
   /** \precond
    * Set is_valid to true for all sensors in sensors.
    */
   for (F360_Radar_Sensor_T& sensor : sensors)
   {
      sensor.variable.is_valid = true;
   }

    /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that globals.f_single_front_center_radar_only is set to false
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only,"f_single_front_center_radar_only was set to true when it was expected to be false.")
}

/** \purpose
 * Verify Check_Sensor_Configuration do set f_single_front_center_radar_only to true when a single sensor is valid at index 0 and
 * it is mounted in F360_MOUNTING_LOCATION_CENTER_FORWARD.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Sensor_Valid_At_Idx0_And_Mounted_In_Center)
{
   /** \precond
    * Set is_valid to true and mounting location to F360_MOUNTING_LOCATION_CENTER_FORWARD for a single sensor in sensors.
    */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
   sensors[0].variable.is_valid = true;

    /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that globals.f_single_front_center_radar_only is set to false
    */
   CHECK_TRUE_TEXT(globals.f_single_front_center_radar_only,"f_single_front_center_radar_only was set to false when it was expected to be true.")
}

/** \purpose
 * Verify Check_Sensor_Configuration do set f_single_front_center_radar_only to true when a single sensor is valid at index 3 and
 * it is mounted in F360_MOUNTING_LOCATION_CENTER_FORWARD.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Sensor_Valid_At_Idx3_And_Mounted_In_Center)
{
   /** \precond
    * Set is_valid to true and mounting location to F360_MOUNTING_LOCATION_CENTER_FORWARD for a single sensor in sensors.
    */
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
   sensors[3].variable.is_valid = true;

    /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that globals.f_single_front_center_radar_only is set to false
    */
   CHECK_TRUE_TEXT(globals.f_single_front_center_radar_only,"f_single_front_center_radar_only was set to false when it was expected to be true.")
}

/** \purpose
 * Verify Check_Sensor_Configuration correctly identifies four corner sensors configuration (one in each corner)
 * and sets f_four_corner_sensors_available to true. Also verify that Calculate_Four_Corner_Sensor_Config_Position_Bounds
 * correctly calculates min/max longitudinal positions for the four corner sensors.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Four_Corner_Sensors_Available)
{
   /** \precond
    * Set four sensors as valid with corner mounting locations.
    * Configure longitudinal mounting positions to enable bounds calculation verification.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[1].variable.is_valid = true;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[2].variable.is_valid = true;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensors[2].constant.mounting_position.vcs_position.longitudinal = -1.0F;
   
   sensors[3].variable.is_valid = true;
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
   sensors[3].constant.mounting_position.vcs_position.longitudinal = -1.0F;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that both flags are set correctly and bounds are calculated.
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "f_single_front_center_radar_only should be false with four corner sensors.")
   CHECK_TRUE_TEXT(globals.f_four_corner_sensors_available, "f_four_corner_sensors_available should be true with all four corners.")
   DOUBLES_EQUAL_TEXT(-1.0F, globals.four_corner_sensor_config_min_longitudinal_position, F360_EPSILON, "Min longitudinal position not calculated correctly.")
   DOUBLES_EQUAL_TEXT(2.0F, globals.four_corner_sensor_config_max_longitudinal_position, F360_EPSILON, "Max longitudinal position not calculated correctly.")
}

/** \purpose
 * Verify Check_Sensor_Configuration correctly identifies when only three corner sensors are available
 * and sets f_four_corner_sensors_available to false. Also verify that Calculate_Four_Corner_Sensor_Config_Position_Bounds
 * resets min/max longitudinal position bounds to 0 when four corner sensors are not available.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Three_Corner_Sensors_Only)
{
   /** \precond
    * Set three sensors as valid with corner mounting locations (missing right rear).
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[1].variable.is_valid = true;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[2].variable.is_valid = true;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensors[2].constant.mounting_position.vcs_position.longitudinal = -1.0F;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that f_four_corner_sensors_available is false and bounds are reset to 0.
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "f_single_front_center_radar_only should be false with three sensors.")
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "f_four_corner_sensors_available should be false with only three corners.")
   DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_min_longitudinal_position, F360_EPSILON, "Min longitudinal position should be reset to 0.")
   DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_max_longitudinal_position, F360_EPSILON, "Max longitudinal position should be reset to 0.")
}

/** \purpose
 * Verify Check_Sensor_Configuration correctly identifies when four sensors are available
 * but not one in each corner (e.g., two left forward sensors). Also verify that
 * Calculate_Four_Corner_Sensor_Config_Position_Bounds resets min/max longitudinal position bounds to 0.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Four_Sensors_But_Not_One_In_Each_Corner)
{
   /** \precond
    * Set four sensors as valid but with duplicate corner positions.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[1].variable.is_valid = true;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;  // Duplicate
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[2].variable.is_valid = true;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensors[2].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[3].variable.is_valid = true;
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensors[3].constant.mounting_position.vcs_position.longitudinal = -1.0F;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that f_four_corner_sensors_available is false since not all four unique corners are covered.
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "f_single_front_center_radar_only should be false with four sensors.")
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "f_four_corner_sensors_available should be false without unique corners.")
   DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_min_longitudinal_position, F360_EPSILON, "Min longitudinal position should be reset to 0.")
   DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_max_longitudinal_position, F360_EPSILON, "Max longitudinal position should be reset to 0.")
}

/** \purpose
 * Verify Check_Sensor_Configuration correctly identifies four corner sensors plus one additional front center sensor
 * and still calculates bounds only for the corner sensors. Verify that Calculate_Four_Corner_Sensor_Config_Position_Bounds
 * excludes the center sensor from the min/max longitudinal position calculation.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Four_Corner_Sensors_Plus_Front_Center)
{
   /** \precond
    * Set four corner sensors plus one front center sensor as valid.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[1].variable.is_valid = true;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   
   sensors[2].variable.is_valid = true;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensors[2].constant.mounting_position.vcs_position.longitudinal = -1.0F;
   
   sensors[3].variable.is_valid = true;
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
   sensors[3].constant.mounting_position.vcs_position.longitudinal = -1.0F;
   
   sensors[4].variable.is_valid = true;
   sensors[4].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
   sensors[4].constant.mounting_position.vcs_position.longitudinal = 3.0F; // Further forward than corners

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that f_four_corner_sensors_available is true and bounds exclude the center sensor.
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "f_single_front_center_radar_only should be false with five sensors.")
   CHECK_TRUE_TEXT(globals.f_four_corner_sensors_available, "f_four_corner_sensors_available should be true with all four corners.")
   DOUBLES_EQUAL_TEXT(-1.0F, globals.four_corner_sensor_config_min_longitudinal_position, F360_EPSILON, "Min longitudinal position should be from corners only.")
   DOUBLES_EQUAL_TEXT(2.0F, globals.four_corner_sensor_config_max_longitudinal_position, F360_EPSILON, "Max longitudinal position should be from corners only, excluding center sensor.")
}

/** \purpose
 * Verify Check_Sensor_Configuration sets front sensor flags for LEFT_FORWARD mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Left_Forward)
{
   /** \precond
    * Set sensor[0] as valid and mounted LEFT_FORWARD.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that front sensor flags are set, corner and single center flags are not.
    */
   CHECK_TRUE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_rear_sensor_available, "Rear sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets front sensor flags for RIGHT_FORWARD mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Right_Forward)
{
   /** \precond
    * Set sensor[0] as valid and mounted RIGHT_FORWARD.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that front sensor flags are set, corner and single center flags are not.
    */
   CHECK_TRUE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_rear_sensor_available, "Rear sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets rear sensor flags for LEFT_REAR mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Left_Rear)
{
   /** \precond
    * Set sensor[0] as valid and mounted LEFT_REAR.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that rear sensor flags are set, corner and single center flags are not.
    */
   CHECK_TRUE_TEXT(globals.f_rear_sensor_available, "Rear sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets rear sensor flags for RIGHT_REAR mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Right_Rear)
{
   /** \precond
    * Set sensor[0] as valid and mounted RIGHT_REAR.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that rear sensor flags are set, corner and single center flags are not.
    */
   CHECK_TRUE_TEXT(globals.f_rear_sensor_available, "Rear sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets front sensor flags for CENTER_FORWARD mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Center_Forward)
{
   /** \precond
    * Set sensor[0] as valid and mounted CENTER_FORWARD.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that single front center radar only flag is set.
    */
   CHECK_TRUE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should be true.");
   CHECK_TRUE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_rear_sensor_available, "Rear sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets rear sensor flags for CENTER_REAR mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Center_Rear)
{
   /** \precond
    * Set sensor[0] as valid and mounted CENTER_REAR.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that rear sensor flag is set, single front center radar only is not.
    */
   CHECK_TRUE_TEXT(globals.f_rear_sensor_available, "Rear sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets front sensor flags for CENTER2_FORWARD mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Center2_Forward)
{
   /** \precond
    * Set sensor[0] as valid and mounted CENTER2_FORWARD.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER2_FORWARD;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that front sensor flag is set, single front center radar only is not.
    */
   CHECK_TRUE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_rear_sensor_available, "Rear sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets rear sensor flags for CENTER2_REAR mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Center2_Rear)
{
   /** \precond
    * Set sensor[0] as valid and mounted CENTER2_REAR.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER2_REAR;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that rear sensor flag is set, single front center radar only is not.
    */
   CHECK_TRUE_TEXT(globals.f_rear_sensor_available, "Rear sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets front sensor flags for CENTER3_FORWARD mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Center3_Forward)
{
   /** \precond
    * Set sensor[0] as valid and mounted CENTER3_FORWARD.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER3_FORWARD;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that front sensor flag is set, single front center radar only is not.
    */
   CHECK_TRUE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_rear_sensor_available, "Rear sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets rear sensor flags for CENTER3_REAR mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Center3_Rear)
{
   /** \precond
    * Set sensor[0] as valid and mounted CENTER3_REAR.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER3_REAR;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that rear sensor flag is set, single front center radar only is not.
    */
   CHECK_TRUE_TEXT(globals.f_rear_sensor_available, "Rear sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration does not set any flags for UNKNOWN mounting location.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Single_Valid_Unknown_Mounting)
{
   /** \precond
    * Set sensor[0] as valid and mounted UNKNOWN.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_UNKNOWN;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that no configuration flags are set.
    */
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should not be available.");
   CHECK_FALSE_TEXT(globals.f_rear_sensor_available, "Rear sensor should not be available.");
}

/** \purpose
 * Verify Check_Sensor_Configuration sets both front and rear sensor flags when both are present.
 * \req
 * NA
 */
TEST(Update_Global_Parameters__Check_Sensor_Configuration, Valid_Front_And_Rear_Sensors)
{
   /** \precond
    * Set sensor[0] as valid and mounted LEFT_FORWARD.
    * Set sensor[1] as valid and mounted RIGHT_REAR.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[1].variable.is_valid = true;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;

   /** \action
    * Call Update_Global_Parameters().
    */
   Update_Global_Parameters(host, sensors, calibrations, globals, timing_info);

   /** \result
    * Check that both front and rear sensor flags are set.
    */
   CHECK_TRUE_TEXT(globals.f_front_or_front_corner_sensor_available, "Front or front corner sensor should be available.");
   CHECK_TRUE_TEXT(globals.f_rear_sensor_available, "Rear sensor should be available.");
   CHECK_FALSE_TEXT(globals.f_four_corner_sensors_available, "Four corner sensors should not be available.");
   CHECK_FALSE_TEXT(globals.f_single_front_center_radar_only, "Single front center radar only should not be available.");
}
/** @}*/


/** \defgroup Calculate_Four_Corner_Sensor_Config_Position_Bounds 
 *  @{
 */

/** \brief
 * This test group verifies the functionality of the function Calculate_Four_Corner_Sensor_Config_Position_Bounds () to ensure that this function
 * calculates the minimum and maximum longitudinal positions of the four corner sensors only and stores them in the globals structure.
 * Only processes sensors if all four corner sensors are available.
 */
TEST_GROUP(Calculate_Four_Corner_Sensor_Config_Position_Bounds)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};

   /** \setup
    * Set is_valid flag for all sensors to false.
    * Set mounting location for all sensors to F360_MOUNTING_LOCATION_UNKNOWN
    * Set f_single_front_center_radar_only to false.
    */
   TEST_SETUP()
   {
      for (F360_Radar_Sensor_T& sensor : sensors)
      {
         sensor.variable.is_valid = false;
         sensor.constant.mounting_location = F360_MOUNTING_LOCATION_UNKNOWN;
      }
      globals.f_single_front_center_radar_only = false;
   }
};

/** \purpose
 * Verify Calculate_Four_Corner_Sensor_Config_Position_Bounds correctly calculates min/max longitudinal 
 * position bounds when f_four_corner_sensors_available is true and four corner sensors are configured.
 * \req
 * NA
 */
TEST(Calculate_Four_Corner_Sensor_Config_Position_Bounds, Calculate_Position_Bounds_With_Four_Corner_Config)
{
   /** \precond
    * Set four corner sensors with different longitudinal positions and set f_four_corner_sensors_available to true.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 1.5F;
   
   sensors[1].variable.is_valid = true;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.5F; // Max value
   
   sensors[2].variable.is_valid = true;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensors[2].constant.mounting_position.vcs_position.longitudinal = -2.0F; // Min value
   
   sensors[3].variable.is_valid = true;
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
   sensors[3].constant.mounting_position.vcs_position.longitudinal = -1.5F;
   
   globals.f_four_corner_sensors_available = true;

   /** \action
    * Call Calculate_Four_Corner_Sensor_Config_Position_Bounds().
    */
   Calculate_Four_Corner_Sensor_Config_Position_Bounds(sensors, globals);

   /** \result
    * Verify min/max bounds are calculated from corner sensor positions only.
    */
   DOUBLES_EQUAL_TEXT(-2.0F, globals.four_corner_sensor_config_min_longitudinal_position, F360_EPSILON, "Min longitudinal position should be -2.0F from left rear sensor.")
   DOUBLES_EQUAL_TEXT(2.5F, globals.four_corner_sensor_config_max_longitudinal_position, F360_EPSILON, "Max longitudinal position should be 2.5F from right forward sensor.")
}

/** \purpose
 * Verify Calculate_Four_Corner_Sensor_Config_Position_Bounds resets min/max longitudinal position bounds to 0 
 * when f_four_corner_sensors_available is false, regardless of sensor configuration.
 * \req
 * NA
 */
TEST(Calculate_Four_Corner_Sensor_Config_Position_Bounds, Calculate_Position_Bounds_Without_Four_Corner_Config)
{
   /** \precond
    * Set four corner sensors with longitudinal positions but set f_four_corner_sensors_available to false.
    * Initialize bounds to non-zero values to verify they get reset.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 1.5F;
   
   sensors[1].variable.is_valid = true;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.5F;
   
   sensors[2].variable.is_valid = true;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensors[2].constant.mounting_position.vcs_position.longitudinal = -2.0F;
   
   sensors[3].variable.is_valid = true;
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
   sensors[3].constant.mounting_position.vcs_position.longitudinal = -1.5F;
   
   globals.f_four_corner_sensors_available = false;
   globals.four_corner_sensor_config_min_longitudinal_position = -999.0F; // Non-zero initial value
   globals.four_corner_sensor_config_max_longitudinal_position = 999.0F;  // Non-zero initial value

   /** \action
    * Call Calculate_Four_Corner_Sensor_Config_Position_Bounds().
    */
   Calculate_Four_Corner_Sensor_Config_Position_Bounds(sensors, globals);

   /** \result
    * Verify bounds are reset to 0 regardless of sensor positions.
    */
   DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_min_longitudinal_position, F360_EPSILON, "Min longitudinal position should be reset to 0 when four corner config unavailable.")
   DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_max_longitudinal_position, F360_EPSILON, "Max longitudinal position should be reset to 0 when four corner config unavailable.")
}

/** \purpose
 * Verify Calculate_Four_Corner_Sensor_Config_Position_Bounds resets min/max longitudinal position bounds to 0 
 * when f_four_corner_sensors_available is false and all sensors are invalid.
 * \req
 * NA
 */
TEST(Calculate_Four_Corner_Sensor_Config_Position_Bounds, Calculate_Position_Bounds_All_Sensors_Invalid)
{
   /** \precond
    * Set all sensors as invalid but with corner mounting locations and longitudinal positions.
    * initialize f_four_corner_sensors_available to false and bounds to non-zero values.
    */
   sensors[0].variable.is_valid = false; // Invalid
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 1.5F;
   
   sensors[1].variable.is_valid = false; // Invalid
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.5F;
   
   sensors[2].variable.is_valid = false; // Invalid
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
   sensors[2].constant.mounting_position.vcs_position.longitudinal = -2.0F;
   
   sensors[3].variable.is_valid = false; // Invalid
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
   sensors[3].constant.mounting_position.vcs_position.longitudinal = -1.5F;
   
   globals.f_four_corner_sensors_available = true; // Pre-set to true
   globals.four_corner_sensor_config_min_longitudinal_position = -100.0F; // Non-zero initial value
   globals.four_corner_sensor_config_max_longitudinal_position = 100.0F;  // Non-zero initial value
   globals.f_four_corner_sensors_available = false; // Should be false since all sensors are invalid

   /** \action
    * Call Calculate_Four_Corner_Sensor_Config_Position_Bounds().
    */
   Calculate_Four_Corner_Sensor_Config_Position_Bounds(sensors, globals);

   /** \result
    * Verify that both configuration flag and bounds are reset when no sensors are valid.
    */
   DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_min_longitudinal_position, F360_EPSILON, "Min longitudinal position should be reset to 0 when all sensors invalid.")
   DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_max_longitudinal_position, F360_EPSILON, "Max longitudinal position should be reset to 0 when all sensors invalid.")
}

/** \purpose
 * Verify Calculate_Four_Corner_Sensor_Config_Position_Bounds resets min/max longitudinal position bounds to 0
 * when the sensors' positions are unexpected.
 * \req
 * NA
 */
TEST(Calculate_Four_Corner_Sensor_Config_Position_Bounds, Calculate_Position_Bounds_All_Sensors_Positions_Are_Unexpected)
{
    /** \precond
     * Set all sensors as invalid but with corner mounting locations and longitudinal positions.
     * initialize f_four_corner_sensors_available to true and bounds to non-zero values.
     */
    sensors[0].variable.is_valid = true; // valid
    sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
    sensors[0].constant.mounting_position.vcs_position.longitudinal = INFTY + 1.0F; // Unexpected

    sensors[1].variable.is_valid = true; // valid
    sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
    sensors[1].constant.mounting_position.vcs_position.longitudinal = INFTY + 1.0F; // Unexpected

    sensors[2].variable.is_valid = true; // valid
    sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
    sensors[2].constant.mounting_position.vcs_position.longitudinal = -INFTY - 1.0F; // Unexpected

    sensors[3].variable.is_valid = true; // valid
    sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
    sensors[3].constant.mounting_position.vcs_position.longitudinal = -INFTY - 1.0F; // Unexpected

    globals.f_four_corner_sensors_available = true; // Pre-set to true
    globals.four_corner_sensor_config_min_longitudinal_position = -100.0F; // Non-zero initial value to ensure it gets reset
    globals.four_corner_sensor_config_max_longitudinal_position = 100.0F;  // Non-zero initial value to ensure it gets reset
    globals.f_four_corner_sensors_available = true;

    /** \action
     * Call Calculate_Four_Corner_Sensor_Config_Position_Bounds().
     */
    Calculate_Four_Corner_Sensor_Config_Position_Bounds(sensors, globals);

    /** \result
     * Verify that both configuration flag and bounds are reset when no sensors are valid.
     */
    DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_min_longitudinal_position, F360_EPSILON, "Min longitudinal position should be reset to 0 when all sensors invalid.")
    DOUBLES_EQUAL_TEXT(0.0F, globals.four_corner_sensor_config_max_longitudinal_position, F360_EPSILON, "Max longitudinal position should be reset to 0 when all sensors invalid.")
}
/** @}*/

/** \defgroup Calculate_Average_Sensor_Position
 *  @{
 */

/** \brief
 * This test group verifies that Calculate_Average_Sensor_Position()
 * computes average longitudinal and lateral sensor mounting position
 * using only valid sensors.
 */
TEST_GROUP(Calculate_Average_Sensor_Position)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};

   /** \setup
    * Set all sensors as invalid and set globals average position fields to non-zero values.
    */
   TEST_SETUP()
   {
      globals.average_sensor_position_x = 99.0F;
      globals.average_sensor_position_y = -99.0F;
   }
};

/** \purpose
 * Verify average sensor position is set to (0, 0) when no sensors are valid.
 * \req
 * NA
 */
TEST(Calculate_Average_Sensor_Position, No_Valid_Sensors)
{
   /** \precond
    * All sensors are invalid from setup.
    */

   /** \action
    * Call Calculate_Average_Sensor_Position().
    */
   Calculate_Average_Sensor_Position(sensors, globals);

   /** \result
    * Verify both average position components are set to 0.
    */
   DOUBLES_EQUAL_TEXT(0.0F, globals.average_sensor_position_x, F360_EPSILON, "Average longitudinal position should be 0 when no sensors are valid.")
   DOUBLES_EQUAL_TEXT(0.0F, globals.average_sensor_position_y, F360_EPSILON, "Average lateral position should be 0 when no sensors are valid.")
}

/** \purpose
 * Verify average sensor position equals the mounting position of the only valid sensor.
 * \req
 * NA
 */
TEST(Calculate_Average_Sensor_Position, Single_Valid_Sensor)
{
   /** \precond
    * Set one valid sensor with known mounting position.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 1.25F;
   sensors[0].constant.mounting_position.vcs_position.lateral = -0.75F;

   /** \action
    * Call Calculate_Average_Sensor_Position().
    */
   Calculate_Average_Sensor_Position(sensors, globals);

   /** \result
    * Verify average equals the selected valid sensor position.
    */
   DOUBLES_EQUAL_TEXT(1.25F, globals.average_sensor_position_x, F360_EPSILON, "Average longitudinal position should match the single valid sensor.")
   DOUBLES_EQUAL_TEXT(-0.75F, globals.average_sensor_position_y, F360_EPSILON, "Average lateral position should match the single valid sensor.")
}

/** \purpose
 * Verify average sensor position is computed from valid sensors only.
 * \req
 * NA
 */
TEST(Calculate_Average_Sensor_Position, Multiple_Valid_Sensors)
{
   /** \precond
    * Set three valid sensors and one invalid sensor with non-zero values.
    */
   sensors[0].variable.is_valid = true;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -1.0F;
   sensors[0].constant.mounting_position.vcs_position.lateral = 2.0F;

   sensors[1].variable.is_valid = true;
   sensors[1].constant.mounting_position.vcs_position.longitudinal = 2.0F;
   sensors[1].constant.mounting_position.vcs_position.lateral = -1.0F;

   sensors[2].variable.is_valid = true;
   sensors[2].constant.mounting_position.vcs_position.longitudinal = 5.0F;
   sensors[2].constant.mounting_position.vcs_position.lateral = 1.0F;

   sensors[3].variable.is_valid = false;
   sensors[3].constant.mounting_position.vcs_position.longitudinal = 100.0F;
   sensors[3].constant.mounting_position.vcs_position.lateral = 100.0F;

   /** \action
    * Call Calculate_Average_Sensor_Position().
    */
   Calculate_Average_Sensor_Position(sensors, globals);

   /** \result
    * Verify average is computed from valid sensors only:
    * x = (-1 + 2 + 5) / 3 = 2
    * y = (2 - 1 + 1) / 3 = 0.6666667
    */
   DOUBLES_EQUAL_TEXT(2.0F, globals.average_sensor_position_x, F360_EPSILON, "Average longitudinal position should be calculated from valid sensors only.")
   DOUBLES_EQUAL_TEXT(0.6666667F, globals.average_sensor_position_y, 1e-6F, "Average lateral position should be calculated from valid sensors only.")
}
/** @}*/
