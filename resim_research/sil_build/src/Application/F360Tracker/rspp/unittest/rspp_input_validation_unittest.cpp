/** \file
 * This file contains unit tests for content of rspp_input_validation.cpp file
 */

#include "rspp_input_validation.h"
#include <CppUTest/TestHarness.h>
#include <cstring>
#include <cmath>
#include <limits>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace rspp_variant_A;

/** \defgroup  test_RSPP_Input_Sensor_Data_Check
 *  @{
 */

/** \brief
 * Tests for RSPP_Input_Sensor_Data_Check function
 */
TEST_GROUP(test_RSPP_Input_Sensor_Data_Check)
{
   // Common test setup
   VariableProps_T sensor_data;

   /** \setup */
   TEST_SETUP()
   {
      memset(&sensor_data, 0, sizeof(VariableProps_T));
      // Initialize with valid default values
      sensor_data.is_valid = true;
      sensor_data.vacs_boresight_az_estimated = 0.0F;
      sensor_data.vacs_boresight_el_estimated = 0.0F;
      sensor_data.vcs_velocity.longitudinal = 0.0F;
      sensor_data.vcs_velocity.lateral = 0.0F;
      sensor_data.number_of_valid_detections = 1U;
   }

   /** \teardown */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test sensor data validation ignores unused input fields
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Unused_Input_Data_Does_Not_Affect_Output)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set valid fields used by validation
    */
   sensor_data.is_valid = true;
   sensor_data.look_id = RSPP_DET_LOOK_ID_1;
   sensor_data.vacs_boresight_az_estimated = 0.1F;
   sensor_data.vacs_boresight_el_estimated = -0.1F;
   sensor_data.vcs_velocity.longitudinal = 5.0F;
   sensor_data.vcs_velocity.lateral = -2.0F;
   sensor_data.number_of_valid_detections = 1U;

   /** \action
    * Validate sensor data and capture baseline result
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Baseline should be valid
    */
   CHECK_EQUAL(true, result);

   /** \precond
    * Modify unused fields only
    */
   sensor_data.timestamp_us = std::numeric_limits<uint64_t>::max();
   sensor_data.overall_rain_level = std::numeric_limits<uint16_t>::max();
   sensor_data.look_index = std::numeric_limits<uint16_t>::max();
   sensor_data.f_sensor_fault_detected = true;

   /** \action
    * Re-validate after modifying unused fields
    */
   result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Output should be unchanged
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with valid default data (all zeros)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Default_Zeros)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All sensor data set to valid default values (zeros)
    */

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for valid default values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation when is_valid flag is false
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Is_Valid_False)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * is_valid flag set to false (validation should be skipped)
    */
   sensor_data.is_valid = false;
   sensor_data.vacs_boresight_az_estimated = 999.0F; // Invalid value, but should be ignored

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true when is_valid is false (no validation performed)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth at minimum boundary (-PI)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_Az_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to minimum valid boundary (-PI)
    */
   sensor_data.vacs_boresight_az_estimated = -3.14159265358979323846F; // -PI

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for boresight azimuth at minimum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth at maximum boundary (PI)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_Az_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to maximum valid boundary (PI)
    */
   sensor_data.vacs_boresight_az_estimated = 3.14159265358979323846F; // PI

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for boresight azimuth at maximum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth just below minimum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_Az_Below_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to just below minimum (-PI - 0.1)
    */
   sensor_data.vacs_boresight_az_estimated = -3.24159265358979323846F; // -PI - 0.1

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight azimuth below minimum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth just above maximum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_Az_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to just above maximum (PI + 0.1)
    */
   sensor_data.vacs_boresight_az_estimated = 3.24159265358979323846F; // PI + 0.1

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight azimuth above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_Az_Positive_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to positive infinity
    */
   sensor_data.vacs_boresight_az_estimated = std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight azimuth at positive infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_Az_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to NaN
    */
   sensor_data.vacs_boresight_az_estimated = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight azimuth at NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth at negative infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_Az_Negative_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to negative infinity
    */
   sensor_data.vacs_boresight_az_estimated = -std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight azimuth at negative infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation at minimum boundary (-PI)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_El_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to minimum valid boundary (-PI)
    */
   sensor_data.vacs_boresight_el_estimated = -3.14159265358979323846F; // -PI

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for boresight elevation at minimum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation at maximum boundary (PI)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_El_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to maximum valid boundary (PI)
    */
   sensor_data.vacs_boresight_el_estimated = 3.14159265358979323846F; // PI

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for boresight elevation at maximum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation just below minimum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_El_Below_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to just below minimum (-PI - 0.1)
    */
   sensor_data.vacs_boresight_el_estimated = -3.24159265358979323846F; // -PI - 0.1

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight elevation below minimum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation just above maximum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_El_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to just above maximum (PI + 0.1)
    */
   sensor_data.vacs_boresight_el_estimated = 3.24159265358979323846F; // PI + 0.1

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight elevation above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_El_Positive_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to positive infinity
    */
   sensor_data.vacs_boresight_el_estimated = std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight elevation at positive infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation at negative infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_El_Negative_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to negative infinity
    */
   sensor_data.vacs_boresight_el_estimated = -std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight elevation at negative infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Boresight_El_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to NaN
    */
   sensor_data.vacs_boresight_el_estimated = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for boresight elevation at NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity at minimum boundary (-138.8)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Vel_Long_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to minimum valid boundary (-138.8)
    */
   sensor_data.vcs_velocity.longitudinal = -138.8F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for longitudinal velocity at minimum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity at maximum boundary (138.8)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Vel_Long_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to maximum valid boundary (138.8)
    */
   sensor_data.vcs_velocity.longitudinal = 138.8F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for longitudinal velocity at maximum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity just below minimum (-138.9)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Long_Below_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to just below minimum (-138.9)
    */
   sensor_data.vcs_velocity.longitudinal = -138.9F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for longitudinal velocity below minimum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity just above maximum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Long_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to just above maximum (138.9)
    */
   sensor_data.vcs_velocity.longitudinal = 138.9F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for longitudinal velocity above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Long_Positive_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to positive infinity
    */
   sensor_data.vcs_velocity.longitudinal = std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for longitudinal velocity at positive infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Long_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to NaN
    */
   sensor_data.vcs_velocity.longitudinal = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for longitudinal velocity at NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity at minimum boundary (-138.8)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Vel_Lat_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to minimum valid boundary (-138.8)
    */
   sensor_data.vcs_velocity.lateral = -138.8F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for lateral velocity at minimum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity at maximum boundary (138.8)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Vel_Lat_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to maximum valid boundary (138.8)
    */
   sensor_data.vcs_velocity.lateral = 138.8F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for lateral velocity at maximum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity just below minimum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Lat_Below_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to just below minimum (-138.9)
    */
   sensor_data.vcs_velocity.lateral = -138.9F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for lateral velocity below minimum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity just above maximum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Lat_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to just above maximum (138.9)
    */
   sensor_data.vcs_velocity.lateral = 138.9F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for lateral velocity above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Lat_Positive_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to positive infinity
    */
   sensor_data.vcs_velocity.lateral = std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for lateral velocity at positive infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity at negative infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Lat_Negative_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to negative infinity
    */
   sensor_data.vcs_velocity.lateral = -std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for lateral velocity at negative infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity at negative infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Long_Negative_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to negative infinity
    */
   sensor_data.vcs_velocity.longitudinal = -std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for longitudinal velocity at negative infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Vel_Lat_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to NaN
    */
   sensor_data.vcs_velocity.lateral = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for lateral velocity at NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with detection count at zero (valid)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Detection_Count_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Detection count set to zero (valid)
    */
   sensor_data.number_of_valid_detections = 0U;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for zero detection count
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with detection count at maximum boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived CPR-7954_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Detection_Count_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Detection count set to maximum valid boundary (MAX_DETS_FOR_SINGLE_SENSOR)
    */
   sensor_data.number_of_valid_detections = MAX_DETS_FOR_SINGLE_SENSOR;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for detection count at maximum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with detection count just above maximum
 * \req
 * CPR-7940_Derived CPR-7951_Derived CPR-7954_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Detection_Count_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Detection count set to just above maximum
    */
   sensor_data.number_of_valid_detections = MAX_DETS_FOR_SINGLE_SENSOR + 1U;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for detection count above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with detection count significantly above maximum
 * \req
 * CPR-7940_Derived CPR-7954_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Detection_Count_Far_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Detection count set far above maximum
    */
   sensor_data.number_of_valid_detections = MAX_DETS_FOR_SINGLE_SENSOR + 100U;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false for detection count far above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with typical valid detection count
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Detection_Count_Typical)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Detection count set to typical value (middle range)
    */
   sensor_data.number_of_valid_detections = MAX_DETS_FOR_SINGLE_SENSOR / 2U;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for typical detection count
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with all parameters at maximum valid boundaries
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_All_Max_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All parameters set to maximum valid boundaries
    */
   sensor_data.vacs_boresight_az_estimated = 3.14159265358979323846F; // PI
   sensor_data.vacs_boresight_el_estimated = 3.14159265358979323846F; // PI
   sensor_data.vcs_velocity.longitudinal = 138.8F;
   sensor_data.vcs_velocity.lateral = 138.8F;
   sensor_data.number_of_valid_detections = MAX_DETS_FOR_SINGLE_SENSOR;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for all parameters at maximum boundaries
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with all parameters at minimum valid boundaries
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_All_Min_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All parameters set to minimum valid boundaries
    */
   sensor_data.vacs_boresight_az_estimated = -3.14159265358979323846F; // -PI
   sensor_data.vacs_boresight_el_estimated = -3.14159265358979323846F; // -PI
   sensor_data.vcs_velocity.longitudinal = -138.8F;
   sensor_data.vcs_velocity.lateral = -138.8F;
   sensor_data.number_of_valid_detections = 0U;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for all parameters at minimum boundaries
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with multiple invalid parameters
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Multiple_Parameters)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Multiple parameters set to invalid values
    */
   sensor_data.vacs_boresight_az_estimated = 4.0F; // Invalid (> PI)
   sensor_data.vcs_velocity.longitudinal = 150.0F; // Invalid (> 138.8)

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false when multiple parameters are invalid
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with positive typical velocities
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Positive_Velocities)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both velocities set to positive typical values
    */
   sensor_data.vcs_velocity.longitudinal = 50.0F;
   sensor_data.vcs_velocity.lateral = 10.0F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for positive typical velocities
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with negative typical velocities
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Negative_Velocities)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both velocities set to negative typical values
    */
   sensor_data.vcs_velocity.longitudinal = -50.0F;
   sensor_data.vcs_velocity.lateral = -10.0F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for negative typical velocities
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with mixed velocity signs
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Mixed_Velocity_Signs)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Velocities set to opposite signs
    */
   sensor_data.vcs_velocity.longitudinal = 50.0F;
   sensor_data.vcs_velocity.lateral = -10.0F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for mixed velocity signs
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with positive boresight angles
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Positive_Boresight_Angles)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both boresight angles set to positive values
    */
   sensor_data.vacs_boresight_az_estimated = 1.0F;
   sensor_data.vacs_boresight_el_estimated = 0.5F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for positive boresight angles
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with negative boresight angles
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Negative_Boresight_Angles)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both boresight angles set to negative values
    */
   sensor_data.vacs_boresight_az_estimated = -1.0F;
   sensor_data.vacs_boresight_el_estimated = -0.5F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for negative boresight angles
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth just inside minimum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_Az_Just_Inside_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to just inside minimum boundary
    */
   sensor_data.vacs_boresight_az_estimated = -3.04159265358979323846F; // -PI + 0.1

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for boresight azimuth just inside minimum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight azimuth just inside maximum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_Az_Just_Inside_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight azimuth set to just inside maximum boundary
    */
   sensor_data.vacs_boresight_az_estimated = 3.04159265358979323846F; // PI - 0.1

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for boresight azimuth just inside maximum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation just inside minimum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_El_Just_Inside_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to just inside minimum boundary
    */
   sensor_data.vacs_boresight_el_estimated = -3.04159265358979323846F; // -PI + 0.1

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for boresight elevation just inside minimum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with boresight elevation just inside maximum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_El_Just_Inside_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Boresight elevation set to just inside maximum boundary
    */
   sensor_data.vacs_boresight_el_estimated = 3.04159265358979323846F; // PI - 0.1

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for boresight elevation just inside maximum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity just inside minimum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Vel_Long_Just_Inside_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to just inside minimum boundary
    */
   sensor_data.vcs_velocity.longitudinal = -119.9F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for longitudinal velocity just inside minimum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with longitudinal velocity just inside maximum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Vel_Long_Just_Inside_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Longitudinal velocity set to just inside maximum boundary
    */
   sensor_data.vcs_velocity.longitudinal = 119.9F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for longitudinal velocity just inside maximum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity just inside minimum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Vel_Lat_Just_Inside_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to just inside minimum boundary
    */
   sensor_data.vcs_velocity.lateral = -119.9F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for lateral velocity just inside minimum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with lateral velocity just inside maximum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Vel_Lat_Just_Inside_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Lateral velocity set to just inside maximum boundary
    */
   sensor_data.vcs_velocity.lateral = 119.9F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for lateral velocity just inside maximum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with small positive velocities
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Small_Positive_Velocities)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both velocities set to small positive values
    */
   sensor_data.vcs_velocity.longitudinal = 0.1F;
   sensor_data.vcs_velocity.lateral = 0.1F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for small positive velocities
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with small negative velocities
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Small_Negative_Velocities)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both velocities set to small negative values
    */
   sensor_data.vcs_velocity.longitudinal = -0.1F;
   sensor_data.vcs_velocity.lateral = -0.1F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for small negative velocities
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with detection count at one (minimum non-zero)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Detection_Count_One)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Detection count set to one
    */
   sensor_data.number_of_valid_detections = 1U;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for detection count of one
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with detection count just below maximum
 * \req
 * CPR-7940_Derived CPR-7954_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Detection_Count_Just_Below_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Detection count set to just below maximum
    */
   sensor_data.number_of_valid_detections = MAX_DETS_FOR_SINGLE_SENSOR - 1U;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for detection count just below maximum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with all velocities at zero
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Velocities_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both velocities set to zero (stationary vehicle)
    */
   sensor_data.vcs_velocity.longitudinal = 0.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for zero velocities
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with both boresight angles at zero
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Boresight_Angles_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both boresight angles set to zero
    */
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for zero boresight angles
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with extreme negative velocities (at boundary)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Extreme_Negative_Velocities)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both velocities set to extreme negative values at boundary
    */
   sensor_data.vcs_velocity.longitudinal = -138.8F;
   sensor_data.vcs_velocity.lateral = -138.8F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for extreme negative velocities at boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with extreme positive velocities (at boundary)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_Extreme_Positive_Velocities)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Both velocities set to extreme positive values at boundary
    */
   sensor_data.vcs_velocity.longitudinal = 138.8F;
   sensor_data.vcs_velocity.lateral = 138.8F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true for extreme positive velocities at boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with is_valid false and all parameters invalid
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Is_Valid_False_All_Other_Invalid)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * is_valid flag set to false with all other parameters invalid
    */
   sensor_data.is_valid = false;
   sensor_data.vacs_boresight_az_estimated = 999.0F;
   sensor_data.vacs_boresight_el_estimated = 999.0F;
   sensor_data.vcs_velocity.longitudinal = 999.0F;
   sensor_data.vcs_velocity.lateral = 999.0F;
   sensor_data.number_of_valid_detections = 999999U;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true when is_valid is false (no validation performed)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with 0 number_of_valid_detections and all other parameters invalid
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Zero_Valid_Dets_All_Other_Invalid)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * number_of_valid_detections set to zero with all other parameters invalid
    */
   sensor_data.number_of_valid_detections = 0U;
   sensor_data.is_valid = true;
   sensor_data.vacs_boresight_az_estimated = 999.0F;
   sensor_data.vacs_boresight_el_estimated = 999.0F;
   sensor_data.vcs_velocity.longitudinal = 999.0F;
   sensor_data.vcs_velocity.lateral = 999.0F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true when number_of_valid_detections is zero (no validation performed)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with 0 number_of_valid_detections and all other parameters invalid
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Zero_Valid_Dets_Valid_Flag_False_All_Other_Invalid)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * 0 number_of_valid_detections and is_valid flag set to false with all other parameters invalid
    */
   sensor_data.number_of_valid_detections = 0U;
   sensor_data.is_valid = false;
   sensor_data.vacs_boresight_az_estimated = 999.0F;
   sensor_data.vacs_boresight_el_estimated = 999.0F;
   sensor_data.vcs_velocity.longitudinal = 999.0F;
   sensor_data.vcs_velocity.lateral = 999.0F;

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return true when number_of_valid_detections is zero (no validation performed)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with all parameters as NaN (except detection count)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_All_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All float parameters set to NaN
    */
   sensor_data.vacs_boresight_az_estimated = std::numeric_limits<float>::quiet_NaN();
   sensor_data.vacs_boresight_el_estimated = std::numeric_limits<float>::quiet_NaN();
   sensor_data.vcs_velocity.longitudinal = std::numeric_limits<float>::quiet_NaN();
   sensor_data.vcs_velocity.lateral = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false when all float parameters are NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with all parameters at infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_All_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All float parameters set to positive infinity
    */
   sensor_data.vacs_boresight_az_estimated = std::numeric_limits<float>::infinity();
   sensor_data.vacs_boresight_el_estimated = std::numeric_limits<float>::infinity();
   sensor_data.vcs_velocity.longitudinal = std::numeric_limits<float>::infinity();
   sensor_data.vcs_velocity.lateral = std::numeric_limits<float>::infinity();

   /** \action
    * Validate sensor data
    */
   bool result = RSPP_Input_Sensor_Data_Check(sensor_data);

   /** \result
    * Should return false when all float parameters are infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor data validation with all valid look_id values
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Valid_All_Look_IDs)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * look_id set to each valid value (RSPP_DET_LOOK_ID_0 to RSPP_DET_LOOK_ID_3)
    */
   bool result = true;
   for (int8_t look_id = static_cast<int8_t>(RSPP_DET_LOOK_ID_0);
        (look_id <= static_cast<int8_t>(RSPP_DET_LOOK_ID_3)) && (true == result);
        ++look_id)
   {
      sensor_data.look_id = static_cast<RSPP_Det_Look_ID_T>(look_id);

      /** \action
       * Validate sensor data
       */
      result = RSPP_Input_Sensor_Data_Check(sensor_data);
   }

   /** \result
    * Should return true for all valid look_id values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor data validation with multiple invalid look_id values including boundary value just above above MAX.
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Input_Sensor_Data_Check, Input_Sensor_Data_Check_TC_Invalid_Look_ID_Multiple)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * look_id set to multiple invalid values
    */
   const int32_t num_invalid = 4;
   const int8_t invalid_look_ids[num_invalid] =
       {
           std::numeric_limits<int8_t>::min(),
           static_cast<int8_t>(RSPP_DET_LOOK_ID_INVALID),
           static_cast<int8_t>(RSPP_DET_LOOK_ID_3 + 1),
           std::numeric_limits<int8_t>::max()};

   bool result = true;
   for (int32_t i = 0; i < num_invalid; ++i)
   {
      sensor_data.look_id = static_cast<RSPP_Det_Look_ID_T>(invalid_look_ids[i]);

      /** \action
       * Validate sensor data
       */
      result = RSPP_Input_Sensor_Data_Check(sensor_data);

      /** \result
       * Should return false for invalid look_id values
       */
      CHECK_EQUAL(false, result);
   }
}

/** @}*/

/** \defgroup  test_RSPP_Check_Input_Detection_Position_Data
 *  @{
 */

/** \brief
 * Tests for RSPP_Check_Input_Detection_Position_Data function
 */
TEST_GROUP(test_RSPP_Check_Input_Detection_Position_Data)
{
   // Common test setup
   Raw_Detection_T raw_detection;
   float32_t fov_max_az_rad;
   float32_t fov_min_az_rad;

   /** \setup */
   TEST_SETUP()
   {
      memset(&raw_detection, 0, sizeof(Raw_Detection_T));
      fov_max_az_rad = 1.0F;
      fov_min_az_rad = -1.0F;
   }

   /** \teardown */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test position data validation ignores unused input fields
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Unused_Input_Data_Does_Not_Affect_Output)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Valid position inputs within FOV
    */
   fov_max_az_rad = 1.0F;
   fov_min_az_rad = -1.0F;
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.5F;
   raw_detection.elevation = 0.1F;

   /** \action
    * Validate detection position data (baseline)
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Baseline should be valid
    */
   CHECK_EQUAL(true, result);

   /** \precond
    * Modify unused fields only
    */
   raw_detection.std_range = std::numeric_limits<float>::quiet_NaN();
   raw_detection.range_rate = std::numeric_limits<float>::infinity();
   raw_detection.std_range_rate = -std::numeric_limits<float>::infinity();
   raw_detection.std_azimuth = 99.0F;
   raw_detection.std_elevation = 88.0F;
   raw_detection.snr = std::numeric_limits<float>::quiet_NaN();
   raw_detection.rcs = std::numeric_limits<float>::quiet_NaN();
   raw_detection.prob_1stazhypo = std::numeric_limits<float>::quiet_NaN();
   raw_detection.sensor_id = -5;
   raw_detection.det_id = -7;
   raw_detection.confid_azimuth = -33;
   raw_detection.confid_elevation = -33;
   raw_detection.f_super_res = true;
   raw_detection.f_host_veh_clutter = true;
   raw_detection.f_nd_target = true;
   raw_detection.f_bistatic = true;
   raw_detection.f_ci_det = true;
   raw_detection.f_idm_det = true;
   raw_detection.f_below_rain_thold = true;

   /** \action
    * Re-validate after modifying unused fields
    */
   result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Output should be unchanged
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with valid default data (all zeros)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Default_Zeros)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All position data set to zero (valid)
    */
   raw_detection.range = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for all zero values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with valid typical values
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Typical_Values)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All position data set to typical valid values
    */
   raw_detection.range = 50.0F;
   raw_detection.azimuth = 0.5F;
   raw_detection.elevation = 0.2F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for typical valid values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with range at minimum boundary (0.0)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Range_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to minimum valid boundary (0.0)
    */
   raw_detection.range = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for range at minimum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with range at maximum boundary (1000.0)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Range_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to maximum valid boundary (500.0)
    */
   raw_detection.range = 500.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for range at maximum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with range just below minimum (negative)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Range_Negative)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to negative value (invalid)
    */
   raw_detection.range = -0.1F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for negative range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with range just above maximum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Range_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to just above maximum (1000.1)
    */
   raw_detection.range = 1000.1F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for range above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with range at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Range_Positive_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to positive infinity
    */
   raw_detection.range = std::numeric_limits<float>::infinity();
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for range at positive infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with range at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Range_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to NaN
    */
   raw_detection.range = std::numeric_limits<float>::quiet_NaN();
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for range at NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with range at negative infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Range_Negative_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to negative infinity
    */
   raw_detection.range = -std::numeric_limits<float>::infinity();
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for range at negative infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with azimuth at minimum FOV boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Azimuth_Min_FOV_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to minimum FOV boundary (fov_min_az_rad = -1.0)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = fov_min_az_rad;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for azimuth at minimum FOV boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with azimuth at maximum FOV boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Azimuth_Max_FOV_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to maximum FOV boundary (fov_max_az_rad = 1.0)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = fov_max_az_rad;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for azimuth at maximum FOV boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with azimuth just below minimum FOV
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Azimuth_Below_Min_FOV)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to just below minimum FOV boundary
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = fov_min_az_rad - 0.1F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for azimuth below minimum FOV
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with azimuth just above maximum FOV
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Azimuth_Above_Max_FOV)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to just above maximum FOV boundary
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = fov_max_az_rad + 0.1F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for azimuth above maximum FOV
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with azimuth at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Azimuth_Positive_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to positive infinity
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = std::numeric_limits<float>::infinity();
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for azimuth at positive infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with azimuth at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Azimuth_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to NaN
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = std::numeric_limits<float>::quiet_NaN();
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for azimuth at NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with azimuth at negative infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Azimuth_Negative_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to negative infinity
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = -std::numeric_limits<float>::infinity();
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for azimuth at negative infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with elevation at minimum boundary (-PI/2)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Elevation_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to minimum valid boundary (-0.53F, -30 deg)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = -0.53F; // -30 deg

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for elevation at minimum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with elevation at maximum boundary (PI/2)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Elevation_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to maximum valid boundary (0.53F, 30 deg)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.53F; // 30 deg

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for elevation at maximum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with elevation just below minimum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Elevation_Below_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to just below minimum (-0.53F - 0.1)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = -0.63F; // -0.53 - 0.1

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for elevation below minimum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with elevation just above maximum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Elevation_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to just above maximum (0.53F + 0.1)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.63F; // 0.53 + 0.1

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for elevation above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with elevation at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Elevation_Positive_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to positive infinity
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = std::numeric_limits<float>::infinity();

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for elevation at positive infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with elevation at negative infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Elevation_Negative_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to negative infinity
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = -std::numeric_limits<float>::infinity();

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for elevation at negative infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with elevation at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Elevation_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to NaN
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for elevation at NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with all parameters at maximum valid boundaries
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_All_Max_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All parameters set to maximum valid boundaries
    */
   raw_detection.range = 500.0F;
   raw_detection.azimuth = fov_max_az_rad;
   raw_detection.elevation = 0.53F; // 30 deg

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for all parameters at maximum boundaries
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with all parameters at minimum valid boundaries
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_All_Min_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All parameters set to minimum valid boundaries
    */
   raw_detection.range = 0.0F;
   raw_detection.azimuth = fov_min_az_rad;
   raw_detection.elevation = -0.53F; // -30 deg

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for all parameters at minimum boundaries
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with multiple invalid parameters
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Multiple_Parameters)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Multiple parameters set to invalid values
    */
   raw_detection.range = -10.0F;                  // Invalid
   raw_detection.azimuth = fov_max_az_rad + 1.0F; // Invalid
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false when multiple parameters are invalid
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with medium range value
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Medium_Range)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to medium value (500.0)
    */
   raw_detection.range = 500.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for medium range value
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with narrow FOV boundaries
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Narrow_FOV)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set narrow FOV boundaries and azimuth within range
    */
   fov_min_az_rad = -0.1F;
   fov_max_az_rad = 0.1F;
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.05F; // Within narrow FOV
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data with narrow FOV
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for azimuth within narrow FOV
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with azimuth outside narrow FOV
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_Outside_Narrow_FOV)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set narrow FOV boundaries and azimuth outside range
    */
   fov_min_az_rad = -0.1F;
   fov_max_az_rad = 0.1F;
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.5F; // Outside narrow FOV
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data with narrow FOV
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false for azimuth outside narrow FOV
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with very small positive range
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Small_Positive_Range)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to very small positive value (0.01)
    */
   raw_detection.range = 0.01F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for very small positive range
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with range just inside maximum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Range_Just_Inside_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range set to just inside maximum (499.9)
    */
   raw_detection.range = 499.9F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for range just inside maximum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with elevation at zero (center)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Elevation_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to zero (center of valid range)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for elevation at zero
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with positive elevation mid-range
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Elevation_Positive_Mid)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to positive mid-range value (0.2 ~ 11.5 deg)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.2F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for positive mid-range elevation
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with negative elevation mid-range
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Elevation_Negative_Mid)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Elevation set to negative mid-range value (-0.2 ~ -11.5 deg)
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = -0.2F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for negative mid-range elevation
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with azimuth just inside minimum FOV boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Azimuth_Just_Inside_Min_FOV)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to just inside minimum FOV boundary
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = fov_min_az_rad + 0.01F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for azimuth just inside minimum FOV
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with azimuth just inside maximum FOV boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Azimuth_Just_Inside_Max_FOV)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Azimuth set to just inside maximum FOV boundary
    */
   raw_detection.range = 100.0F;
   raw_detection.azimuth = fov_max_az_rad - 0.01F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for azimuth just inside maximum FOV
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test position data validation with all parameters at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Invalid_All_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * All parameters set to NaN
    */
   raw_detection.range = std::numeric_limits<float>::quiet_NaN();
   raw_detection.azimuth = std::numeric_limits<float>::quiet_NaN();
   raw_detection.elevation = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate detection position data
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return false when all parameters are NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test position data validation with wide FOV boundaries (full PI range)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Position_Data, Check_Input_Detection_Position_Data_TC_Valid_Wide_FOV)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set wide FOV boundaries (full +-PI range) and azimuth within
    */
   fov_min_az_rad = -3.14159265358979323846F; // -PI
   fov_max_az_rad = 3.14159265358979323846F;  // PI
   raw_detection.range = 100.0F;
   raw_detection.azimuth = 1.0F; // Within wide FOV
   raw_detection.elevation = 0.0F;
   raw_detection.range_rate = 0.0F;

   /** \action
    * Validate detection position data with wide FOV
    */
   bool result = RSPP_Check_Input_Detection_Position_Data(raw_detection, fov_max_az_rad, fov_min_az_rad);

   /** \result
    * Should return true for azimuth within wide FOV
    */
   CHECK_EQUAL(true, result);
}

/** @}*/

/** \defgroup  test_RSPP_Check_Input_Detection_Velocity_Data
 *  @{
 */

/** \brief
 * Tests for RSPP_Check_Input_Detection_Velocity_Data function
 */
TEST_GROUP(test_RSPP_Check_Input_Detection_Velocity_Data)
{
   // Common test setup
   Raw_Detection_T raw_detection;

   /** \setup */
   TEST_SETUP()
   {
      memset(&raw_detection, 0, sizeof(Raw_Detection_T));
   }

   /** \teardown */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test velocity data validation ignores unused input fields
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Unused_Input_Data_Does_Not_Affect_Output)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Valid range rate
    */
   raw_detection.range_rate = 10.0F;

   /** \action
    * Validate detection velocity data (baseline)
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Baseline should be valid
    */
   CHECK_EQUAL(true, result);

   /** \precond
    * Modify unused fields only
    */
   raw_detection.range = std::numeric_limits<float>::quiet_NaN();
   raw_detection.azimuth = std::numeric_limits<float>::infinity();
   raw_detection.elevation = -std::numeric_limits<float>::infinity();
   raw_detection.std_range = std::numeric_limits<float>::quiet_NaN();
   raw_detection.std_azimuth = std::numeric_limits<float>::quiet_NaN();
   raw_detection.std_elevation = std::numeric_limits<float>::quiet_NaN();
   raw_detection.snr = std::numeric_limits<float>::quiet_NaN();
   raw_detection.rcs = std::numeric_limits<float>::quiet_NaN();
   raw_detection.sensor_id = -33;
   raw_detection.det_id = -44;
   raw_detection.f_super_res = true;

   /** \action
    * Re-validate after modifying unused fields
    */
   result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Output should be unchanged
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with valid default data (zero)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Default_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to 0.0 (valid)
    */
   raw_detection.range_rate = 0.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for zero range rate
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with valid positive range rate
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Positive_Range_Rate)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to positive value within valid range
    */
   raw_detection.range_rate = 50.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for valid positive range rate
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with valid negative range rate
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Negative_Range_Rate)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to negative value within valid range
    */
   raw_detection.range_rate = -50.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for valid negative range rate
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with range rate at maximum valid boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to maximum valid value (128.0)
    */
   raw_detection.range_rate = 128.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for range rate at maximum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with range rate at minimum valid boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to minimum valid value (-128.0)
    */
   raw_detection.range_rate = -128.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for range rate at minimum boundary
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with range rate just above maximum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to just above maximum (128.1)
    */
   raw_detection.range_rate = 128.1F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for range rate above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with range rate just below minimum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_Below_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to just below minimum (-128.1)
    */
   raw_detection.range_rate = -128.1F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for range rate below minimum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with range rate significantly above maximum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_Far_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set far above maximum (500.0)
    */
   raw_detection.range_rate = 500.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for range rate far above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with range rate significantly below minimum
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_Far_Below_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set far below minimum (-500.0)
    */
   raw_detection.range_rate = -500.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for range rate far below minimum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with range rate at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_Positive_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to positive infinity
    */
   raw_detection.range_rate = std::numeric_limits<float>::infinity();

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for positive infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with range rate at negative infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_Negative_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to negative infinity
    */
   raw_detection.range_rate = -std::numeric_limits<float>::infinity();

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for negative infinity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with range rate at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to NaN (Not a Number)
    */
   raw_detection.range_rate = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for NaN
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with range rate just inside maximum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Just_Inside_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to just inside maximum (127.9)
    */
   raw_detection.range_rate = 127.9F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for range rate just inside maximum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with range rate just inside minimum boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Just_Inside_Min)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to just inside minimum (-127.9)
    */
   raw_detection.range_rate = -127.9F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for range rate just inside minimum
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with small positive range rate
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Small_Positive)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to small positive value (0.1)
    */
   raw_detection.range_rate = 0.1F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for small positive range rate
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with small negative range rate
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Small_Negative)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to small negative value (-0.1)
    */
   raw_detection.range_rate = -0.1F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for small negative range rate
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with medium positive range rate
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Medium_Positive)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to medium positive value (100.0)
    */
   raw_detection.range_rate = 100.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for medium positive range rate
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with medium negative range rate
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Medium_Negative)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to medium negative value (-100.0)
    */
   raw_detection.range_rate = -100.0F;

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for medium negative range rate
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with range rate at maximum float value
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_Max_Float)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to maximum float value
    */
   raw_detection.range_rate = std::numeric_limits<float>::max();

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for maximum float value
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with range rate at minimum float value
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Invalid_Min_Float)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to minimum float value (most negative)
    */
   raw_detection.range_rate = std::numeric_limits<float>::lowest();

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return false for minimum float value
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test velocity data validation with very small positive epsilon value
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Epsilon_Positive)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to smallest positive float epsilon
    */
   raw_detection.range_rate = std::numeric_limits<float>::epsilon();

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for epsilon value
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test velocity data validation with very small negative epsilon value
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Input_Detection_Velocity_Data, Check_Input_Detection_Velocity_Data_TC_Valid_Epsilon_Negative)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Range rate is set to smallest negative float epsilon
    */
   raw_detection.range_rate = -std::numeric_limits<float>::epsilon();

   /** \action
    * Validate detection velocity data
    */
   bool result = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

   /** \result
    * Should return true for negative epsilon value
    */
   CHECK_EQUAL(true, result);
}

/** @}*/

/** \defgroup  test_RSPP_Check_Detection_Meta_Data
 *  @{
 */

/** \brief
 * Tests for RSPP_Check_Detection_Meta_Data function
 */
TEST_GROUP(test_RSPP_Check_Detection_Meta_Data)
{
   // Common test setup
   Raw_Detection_T raw_detection;

   /** \setup */
   TEST_SETUP()
   {
      memset(&raw_detection, 0, sizeof(Raw_Detection_T));
   }

   /** \teardown */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test metadata validation ignores unused input fields
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Unused_Input_Data_Does_Not_Affect_Output)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Valid sensor and detection IDs
    */
   raw_detection.sensor_id = 1;
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata (baseline)
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Baseline should be valid
    */
   CHECK_EQUAL(true, result);

   /** \precond
    * Modify unused fields only
    */
   raw_detection.range = std::numeric_limits<float>::quiet_NaN();
   raw_detection.range_rate = std::numeric_limits<float>::infinity();
   raw_detection.azimuth = -std::numeric_limits<float>::infinity();
   raw_detection.elevation = std::numeric_limits<float>::quiet_NaN();
   raw_detection.std_range = std::numeric_limits<float>::quiet_NaN();
   raw_detection.std_range_rate = std::numeric_limits<float>::quiet_NaN();
   raw_detection.std_azimuth = std::numeric_limits<float>::quiet_NaN();
   raw_detection.std_elevation = std::numeric_limits<float>::quiet_NaN();
   raw_detection.snr = std::numeric_limits<float>::quiet_NaN();
   raw_detection.rcs = std::numeric_limits<float>::quiet_NaN();
   raw_detection.confid_azimuth = -33;
   raw_detection.confid_elevation = -33;
   raw_detection.f_super_res = true;
   raw_detection.f_host_veh_clutter = true;
   raw_detection.f_nd_target = true;
   raw_detection.f_bistatic = true;
   raw_detection.f_ci_det = true;
   raw_detection.f_idm_det = true;
   raw_detection.f_below_rain_thold = true;

   /** \action
    * Re-validate after modifying unused fields
    */
   result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Output should be unchanged
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test detection metadata validation with valid default data
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Valid_Default_Data)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Valid sensor and detection IDs are set
    */
   raw_detection.sensor_id = 1;
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return true for valid metadata
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test detection metadata validation with invalid sensor ID (zero)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Invalid_Sensor_ID_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to 0 (invalid, must be >= 1)
    */
   raw_detection.sensor_id = 0;
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata with invalid sensor ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for invalid sensor ID
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with negative sensor ID
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Invalid_Sensor_ID_Negative)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to negative value (invalid)
    */
   raw_detection.sensor_id = -1;
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata with negative sensor ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for negative sensor ID
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with sensor ID exceeding maximum
 * \req
 * CPR-7940_Derived CPR-7950_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Invalid_Sensor_ID_Exceeds_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID > MAX_NUMBER_OF_SENSORS (which is 10)
    */
   raw_detection.sensor_id = MAX_NUMBER_OF_SENSORS + 1;
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata with sensor ID exceeding maximum
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for sensor ID exceeding maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with maximum valid sensor ID
 * \req
 * CPR-7940_Derived CPR-7950_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Valid_Max_Sensor_ID)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to maximum valid value (MAX_NUMBER_OF_SENSORS)
    */
   raw_detection.sensor_id = MAX_NUMBER_OF_SENSORS;
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata with maximum valid sensor ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return true for maximum valid sensor ID
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test detection metadata validation with invalid detection ID (zero)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Invalid_Det_ID_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set detection ID to 0 (invalid, must be > 0)
    */
   raw_detection.sensor_id = 1;
   raw_detection.det_id = 0;

   /** \action
    * Validate detection metadata with invalid detection ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for invalid detection ID (zero)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with negative detection ID
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Invalid_Det_ID_Negative)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set detection ID to negative value (invalid)
    */
   raw_detection.sensor_id = 1;
   raw_detection.det_id = -1;

   /** \action
    * Validate detection metadata with negative detection ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for negative detection ID
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with valid large detection ID
 * \req
 * CPR-7940_Derived CPR-7954_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Valid_Large_Det_ID)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set detection ID to a large positive value
    */
   raw_detection.sensor_id = 1;
   raw_detection.det_id = MAX_DETS_FOR_SINGLE_SENSOR; // Large valid detection ID

   /** \action
    * Validate detection metadata with large detection ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return true for large positive detection ID
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test detection metadata validation with both sensor and detection IDs invalid
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Both_IDs_Invalid)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set both sensor ID (to 0) and detection ID (to 0) to invalid values
    */
   raw_detection.sensor_id = 0;
   raw_detection.det_id = 0;

   /** \action
    * Validate detection metadata with both IDs invalid
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false when both IDs are invalid
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with sensor ID just above maximum
 * \req
 * CPR-7940_Derived CPR-7950_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Sensor_ID_Just_Above_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to just above maximum (MAX_NUMBER_OF_SENSORS + 1)
    */
   raw_detection.sensor_id = MAX_NUMBER_OF_SENSORS + 1;
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata with sensor ID just above maximum
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for sensor ID just above maximum
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with sensor ID at boundary (MAX - 1)
 * \req
 * CPR-7940_Derived CPR-7950_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Valid_Sensor_ID_Max_Minus_One)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to MAX_NUMBER_OF_SENSORS - 1
    */
   raw_detection.sensor_id = MAX_NUMBER_OF_SENSORS - 1;
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata with sensor ID at max - 1
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return true for sensor ID at max - 1
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test detection metadata validation with extreme negative sensor ID
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Invalid_Sensor_ID_Extreme_Negative)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to extreme negative value (INT32_MIN)
    */
   raw_detection.sensor_id = -2147483648; // INT32_MIN
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata with extreme negative sensor ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for extreme negative sensor ID
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with extreme positive sensor ID
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Invalid_Sensor_ID_Extreme_Positive)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to extreme positive value (INT32_MAX)
    */
   raw_detection.sensor_id = 2147483647; // INT32_MAX
   raw_detection.det_id = 1;

   /** \action
    * Validate detection metadata with extreme positive sensor ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for extreme positive sensor ID
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test detection metadata validation with various valid middle-range sensor IDs
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Valid_Middle_Range_Sensor_ID)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to a middle-range valid value (5)
    */
   raw_detection.sensor_id = 5;
   raw_detection.det_id = 100;

   /** \action
    * Validate detection metadata with middle-range sensor ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return true for middle-range sensor ID
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test detection metadata validation with both IDs at valid boundaries
 * \req
 * CPR-7940_Derived CPR-7950_Derived CPR-7954_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Valid_Both_IDs_Max_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set both sensor ID and detection ID to maximum/large values
    */
   raw_detection.sensor_id = MAX_NUMBER_OF_SENSORS;
   raw_detection.det_id = MAX_DETS_FOR_SINGLE_SENSOR;

   /** \action
    * Validate detection metadata with both IDs at boundaries
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return true when both IDs are at valid boundaries
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test detection metadata validation with extreme negative detection ID
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Detection_Meta_Data, Check_Detection_Meta_Data_TC_Invalid_Det_ID_Extreme_Negative)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set detection ID to extreme negative value (INT32_MIN)
    */
   raw_detection.sensor_id = 1;
   raw_detection.det_id = -2147483648; // INT32_MIN

   /** \action
    * Validate detection metadata with extreme negative detection ID
    */
   bool result = RSPP_Check_Detection_Meta_Data(raw_detection);

   /** \result
    * Should return false for extreme negative detection ID
    */
   CHECK_EQUAL(false, result);
}

/** @}*/

/** \defgroup  test_RSPP_Check_Sensor_Calibration
 *  @{
 */

/** \brief
 * Tests for RSPP_Check_Sensor_Calibration function
 */
TEST_GROUP(test_RSPP_Check_Sensor_Calibration)
{
   // Common test setup
   ConstantProps_T constant_props;

   void Initialize_Default_Constant_Props(ConstantProps_T & props)
   {
      memset(&props, 0, sizeof(ConstantProps_T));
      // Initialize with valid default values
      props.id = 1U;
      props.polarity = 1;
      props.mounting_position.vcs_position.longitudinal = 0.0F;
      props.mounting_position.vcs_position.lateral = 0.0F;
      props.mounting_position.vcs_position.height = 0.5F; // Valid range: 0.3F to 1.3F
      props.mounting_position.vcs_boresight_azimuth_angle = 0.0F;

      // Initialize all look ID parameters with valid defaults
      for (uint8_t i = 0U; i < RSPP_DET_NUM_LOOK_ID; ++i)
      {
         props.fov_min_az_rad[i] = -1.0F;
         props.fov_max_az_rad[i] = 1.0F;
         props.fov_min_el_rad[i] = -1.0F;
         props.fov_max_el_rad[i] = 1.0F;
         props.range_limits[i] = 300.0F;
         props.v_wrapping[i] = 50.0F;
         props.r_wrapping[i] = 1.0F;
         props.min_aliaised_range_rate[i] = -100.0F;
      }
   }

   /** \setup */
   TEST_SETUP()
   {
      Initialize_Default_Constant_Props(constant_props);
   }

   /** \teardown */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test calibration validation ignores unused input fields
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Unused_Input_Data_Does_Not_Affect_Output)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Valid default calibration from setup
    */

   /** \action
    * Validate sensor calibration (baseline)
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Baseline should be valid
    */
   CHECK_EQUAL(true, result);

   /** \precond
    * Modify unused fields only
    */
   constant_props.internal_reflections.min_host_vel = std::numeric_limits<float>::quiet_NaN();
   constant_props.internal_reflections.occurrence_lowerlimit = std::numeric_limits<float>::infinity();
   constant_props.internal_reflections.occurrence_threshold = -std::numeric_limits<float>::infinity();
   constant_props.internal_reflections.rcs_tolerance = 999.0F;
   constant_props.internal_reflections.azimuth_tolerance = 999.0F;
   constant_props.internal_reflections.range_tolerance = 999.0F;
   constant_props.internal_reflections.max_abs_range_rate = 999.0F;
   constant_props.internal_reflections.rcs_max = 999.0F;
   constant_props.internal_reflections.range_max = 999.0F;
   constant_props.internal_reflections.age_threshold = std::numeric_limits<uint16_t>::max();
   constant_props.internal_reflections.f_enable = true;

   constant_props.mounting_position.vcs_boresight_elevation_angle = std::numeric_limits<float>::quiet_NaN();
   constant_props.mounting_location = static_cast<RSPP_Mounting_Location_T>(0);
   constant_props.sensor_type = RSPP_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;

   /** \action
    * Re-validate after modifying unused fields
    */
   result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Output should be unchanged
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with valid default data
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_Default_Calibration)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Valid calibration data is set up in test setup
    */

   /** \action
    * Validate sensor calibration
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for valid calibration data
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with invalid sensor ID (zero)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Sensor_ID_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID to 0 (invalid, must be >= 1)
    */
   constant_props.id = 0U;

   /** \action
    * Validate calibration with invalid sensor ID
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for sensor ID out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with invalid sensor ID (too high)
 * \req
 * CPR-7940_Derived CPR-7950_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Sensor_ID_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set sensor ID > MAX_NUMBER_OF_SENSORS
    */
   constant_props.id = MAX_NUMBER_OF_SENSORS + 1U;

   /** \action
    * Validate calibration with invalid sensor ID
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for sensor ID out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with invalid polarity (zero)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Polarity_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set polarity to 0 (invalid)
    */
   constant_props.polarity = 0;

   /** \action
    * Validate calibration with invalid polarity
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for invalid polarity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with invalid polarity (2)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Polarity_Two)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set polarity to 2 (invalid, must be 1 or -1)
    */
   constant_props.polarity = 2;

   /** \action
    * Validate calibration with invalid polarity
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for invalid polarity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with valid negative polarity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_Negative_Polarity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set polarity to -1 (valid)
    */
   constant_props.polarity = -1;

   /** \action
    * Validate calibration with negative polarity
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for valid negative polarity
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position longitude too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Long_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position longitude < -10.0F
    */
   constant_props.mounting_position.vcs_position.longitudinal = -11.0F;

   /** \action
    * Validate calibration with invalid mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for mounting position out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position longitude too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Long_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position longitude > 1.0F
    */
   constant_props.mounting_position.vcs_position.longitudinal = 2.0F;

   /** \action
    * Validate calibration with invalid mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for mounting position out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position latitude too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Lat_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position latitude < -1.5F
    */
   constant_props.mounting_position.vcs_position.lateral = -2.0F;

   /** \action
    * Validate calibration with invalid mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for mounting position out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position latitude too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Lat_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position latitude > 1.5F
    */
   constant_props.mounting_position.vcs_position.lateral = 2.0F;

   /** \action
    * Validate calibration with invalid mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for mounting position out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position height too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Height_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position height < 0.3F
    */
   constant_props.mounting_position.vcs_position.height = 0.2F;

   /** \action
    * Validate calibration with invalid mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for mounting position out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position height too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Height_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position height > 1.3F
    */
   constant_props.mounting_position.vcs_position.height = 1.4F;

   /** \action
    * Validate calibration with invalid mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for mounting position out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with boresight azimuth angle too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Boresight_Az_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set boresight azimuth angle < -PI
    */
   constant_props.mounting_position.vcs_boresight_azimuth_angle = -7.0F;

   /** \action
    * Validate calibration with invalid boresight angle
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for boresight angle out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with boresight azimuth angle too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Boresight_Az_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set boresight azimuth angle > PI
    */
   constant_props.mounting_position.vcs_boresight_azimuth_angle = 7.0F;

   /** \action
    * Validate calibration with invalid boresight angle
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for boresight angle out of range
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with boresight azimuth angle at minimum valid boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_Boresight_Az_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set boresight azimuth angle to minimum valid boundary (-PI)
    */
   constant_props.mounting_position.vcs_boresight_azimuth_angle = -3.14159265358979323846F; // -PI

   /** \action
    * Validate calibration with minimum boundary boresight angle
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for minimum boundary boresight angle
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with FOV min azimuth too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Min_Az_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min azimuth < -PI for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_az_rad[look_id] = -3.3F;

      /** \action
       * Validate calibration with invalid FOV min azimuth
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for FOV min azimuth out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV min azimuth too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Min_Az_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min azimuth > PI for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_az_rad[look_id] = 2.3F;

      /** \action
       * Validate calibration with invalid FOV min azimuth
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for FOV min azimuth out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV max azimuth too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Max_Az_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV max azimuth < -PI for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_max_az_rad[look_id] = -3.3F;

      /** \action
       * Validate calibration with invalid FOV max azimuth
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for FOV max azimuth out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV max azimuth too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Max_Az_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV max azimuth > PI for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_max_az_rad[look_id] = 3.3F;

      /** \action
       * Validate calibration with invalid FOV max azimuth
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for FOV max azimuth out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV min azimuth >= FOV max azimuth
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Az_Min_GTE_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min azimuth >= FOV max azimuth for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_az_rad[look_id] = -0.5F;
      constant_props.fov_max_az_rad[look_id] = -0.6F;

      /** \action
       * Validate calibration with invalid FOV azimuth range
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false when FOV min >= FOV max
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV min elevation too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Min_El_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min elevation < -PI for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_el_rad[look_id] = -3.3F;

      /** \action
       * Validate calibration with invalid FOV min elevation
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for FOV min elevation out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV min elevation too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Min_El_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min elevation > PI for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_el_rad[look_id] = 3.3F;

      /** \action
       * Validate calibration with invalid FOV min elevation
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for FOV min elevation out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV max elevation too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Max_El_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV max elevation < -PI for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_max_el_rad[look_id] = -3.3F;

      /** \action
       * Validate calibration with invalid FOV max elevation
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for FOV max elevation out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV max elevation too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Max_El_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV max elevation > PI for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_max_el_rad[look_id] = 3.3F;

      /** \action
       * Validate calibration with invalid FOV max elevation
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for FOV max elevation out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV min elevation > FOV max elevation
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_El_Min_GT_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min elevation > FOV max elevation for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_el_rad[look_id] = -0.3F;
      constant_props.fov_max_el_rad[look_id] = -0.5F;

      /** \action
       * Validate calibration with invalid FOV elevation range
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false when FOV min > FOV max
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with range limit too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Range_Limit_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set range limit < 0.0F for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.range_limits[look_id] = -50.0F;

      /** \action
       * Validate calibration with invalid range limit
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for range limit out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with range limit too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Range_Limit_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set range limit > 500.0F for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.range_limits[look_id] = 600.0F;

      /** \action
       * Validate calibration with invalid range limit
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for range limit out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with velocity wrapping too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_V_Wrapping_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set velocity wrapping < 10.0F for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.v_wrapping[look_id] = 5.0F;

      /** \action
       * Validate calibration with invalid velocity wrapping
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for velocity wrapping out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with velocity wrapping too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_V_Wrapping_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set velocity wrapping > 100.0F for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.v_wrapping[look_id] = 150.0F;

      /** \action
       * Validate calibration with invalid velocity wrapping
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for velocity wrapping out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with minimum aliased range rate too low
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Min_Aliased_RR_Too_Low)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set minimum aliased range rate < -200.0F for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.min_aliaised_range_rate[look_id] = -300.0F;

      /** \action
       * Validate calibration with invalid minimum aliased range rate
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for minimum aliased range rate out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with minimum aliased range rate too high
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Min_Aliased_RR_Too_High)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set minimum aliased range rate > -5.0F for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.min_aliaised_range_rate[look_id] = -4.0F;

      /** \action
       * Validate calibration with invalid minimum aliased range rate
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for minimum aliased range rate out of range
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with all look ID dependent parameters at minimum boundary values
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_All_Look_IDs_Min_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set all parameters to minimum valid boundary values for all look IDs
    */
   constant_props.id = 1U;
   constant_props.mounting_position.vcs_position.longitudinal = -10.0F;
   constant_props.mounting_position.vcs_position.lateral = -1.5F;
   constant_props.mounting_position.vcs_position.height = 0.3F;
   constant_props.mounting_position.vcs_boresight_azimuth_angle = -3.14159265358979323846F; // -PI

   for (uint8_t i = 0U; i < RSPP_DET_NUM_LOOK_ID; ++i)
   {
      constant_props.fov_min_az_rad[i] = -3.14159265358979323846F;         // -PI
      constant_props.fov_max_az_rad[i] = -3.14159265358979323846F + 0.01F; // -PI + 0.01
      constant_props.fov_min_el_rad[i] = -3.14159265358979323846F;         // -PI
      constant_props.fov_max_el_rad[i] = -3.14159265358979323846F + 0.01F; // -PI + 0.01
      constant_props.range_limits[i] = 0.0F;
      constant_props.v_wrapping[i] = 10.0F;
      constant_props.r_wrapping[i] = -2.0F;
      constant_props.min_aliaised_range_rate[i] = -200.0F;
   }

   /** \action
    * Validate calibration with all minimum boundary values
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for valid minimum boundary values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with all look ID dependent parameters at maximum boundary values
 * \req
 * CPR-7940_Derived CPR-7950_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_All_Look_IDs_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set all parameters to maximum valid boundary values for all look IDs
    */
   constant_props.id = MAX_NUMBER_OF_SENSORS;
   constant_props.mounting_position.vcs_position.longitudinal = 1.0F;
   constant_props.mounting_position.vcs_position.lateral = 1.5F;
   constant_props.mounting_position.vcs_position.height = 1.3F;
   constant_props.mounting_position.vcs_boresight_azimuth_angle = 3.14159265358979323846F; // PI

   for (uint8_t i = 0U; i < RSPP_DET_NUM_LOOK_ID; ++i)
   {
      constant_props.fov_min_az_rad[i] = -3.14159265358979323846F; // -PI
      constant_props.fov_max_az_rad[i] = 3.14159265358979323846F;  // PI
      constant_props.fov_min_el_rad[i] = -3.14159265358979323846F; // -PI
      constant_props.fov_max_el_rad[i] = 3.14159265358979323846F;  // PI
      constant_props.range_limits[i] = 500.0F;
      constant_props.v_wrapping[i] = 100.0F;
      constant_props.r_wrapping[i] = 2.0F;
      constant_props.min_aliaised_range_rate[i] = -5.0F;
   }

   /** \action
    * Validate calibration with all maximum boundary values
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for valid maximum boundary values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with invalid value in one look ID while others are valid
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_One_Look_ID_Among_Valid)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set all look IDs valid except one with invalid FOV range
    */
   for (uint8_t i = 0U; i < RSPP_DET_NUM_LOOK_ID; ++i)
   {
      constant_props.fov_min_az_rad[i] = -1.0F;
      constant_props.fov_max_az_rad[i] = 1.0F;
      constant_props.fov_min_el_rad[i] = -1.0F;
      constant_props.fov_max_el_rad[i] = 1.0F;
      constant_props.range_limits[i] = 300.0F;
      constant_props.v_wrapping[i] = 50.0F;
      constant_props.r_wrapping[i] = 1.0F;
      constant_props.min_aliaised_range_rate[i] = -100.0F;
   }

   // Make one look ID invalid
   if (RSPP_DET_NUM_LOOK_ID > 1U)
   {
      constant_props.fov_min_az_rad[1] = 0.5F;
      constant_props.fov_max_az_rad[1] = 0.3F; // min > max
   }

   /** \action
    * Validate calibration with one invalid look ID
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false when any look ID has invalid parameters
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position at minimum valid boundaries
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_Mounting_Pos_Min_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position coordinates to minimum valid boundary values
    */
   constant_props.mounting_position.vcs_position.longitudinal = -10.0F;
   constant_props.mounting_position.vcs_position.lateral = -1.5F;
   constant_props.mounting_position.vcs_position.height = 0.3F;

   /** \action
    * Validate calibration with minimum boundary mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for minimum boundary mounting position values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position at maximum valid boundaries
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_Mounting_Pos_Max_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position coordinates to maximum valid boundary values
    */
   constant_props.mounting_position.vcs_position.longitudinal = 1.0F;
   constant_props.mounting_position.vcs_position.lateral = 1.5F;
   constant_props.mounting_position.vcs_position.height = 1.3F;

   /** \action
    * Validate calibration with maximum boundary mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for maximum boundary mounting position values
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with invalid polarity (-2)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Polarity_Negative_Two)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set polarity to -2 (invalid, must be 1 or -1)
    */
   constant_props.polarity = -2;

   /** \action
    * Validate calibration with invalid polarity
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for invalid polarity
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with boresight azimuth at maximum valid boundary
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_Boresight_Az_Max_Boundary)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set boresight azimuth angle to maximum valid boundary (+PI)
    */
   constant_props.mounting_position.vcs_boresight_azimuth_angle = 3.14159265358979323846F; // PI

   /** \action
    * Validate calibration with maximum boundary boresight angle
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for maximum boundary boresight angle
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with boresight azimuth at zero
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_Boresight_Az_Zero)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set boresight azimuth angle to zero
    */
   constant_props.mounting_position.vcs_boresight_azimuth_angle = 0.0F;

   /** \action
    * Validate calibration with zero boresight angle
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return true for zero boresight angle
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test sensor calibration validation with FOV azimuth min equal to max
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Az_Min_Equal_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min azimuth == FOV max azimuth for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_az_rad[look_id] = -0.5F;
      constant_props.fov_max_az_rad[look_id] = -0.5F; // min == max (invalid)

      /** \action
       * Validate calibration with equal FOV azimuth range
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false when FOV min == FOV max
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV elevation min equal to max
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_FOV_El_Min_Equal_Max)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min elevation == FOV max elevation for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_el_rad[look_id] = -0.3F;
      constant_props.fov_max_el_rad[look_id] = -0.3F; // min == max (valid for elevation)

      /** \action
       * Validate calibration with equal FOV elevation range
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return true when FOV min == FOV max (valid for elevation)
       */
      CHECK_EQUAL(true, result);
   }
}

/** \purpose
 * Test sensor calibration validation with first look ID invalid
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_First_Look_ID)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set first look ID with invalid range limit
    */
   constant_props.range_limits[0] = -10.0F; // Invalid (< 0)

   /** \action
    * Validate calibration with first look ID invalid
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false when first look ID is invalid
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with last look ID invalid
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Last_Look_ID)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set last look ID with invalid velocity wrapping
    */
   if (RSPP_DET_NUM_LOOK_ID > 0U)
   {
      constant_props.v_wrapping[RSPP_DET_NUM_LOOK_ID - 1U] = 150.0F; // Invalid (> 100)
   }

   /** \action
    * Validate calibration with last look ID invalid
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false when last look ID is invalid
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with FOV at exact boundary values
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_FOV_Exact_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV to exact boundary values for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_az_rad[look_id] = -3.14159265358979323846F; // -PI (min boundary)
      constant_props.fov_max_az_rad[look_id] = 3.14159265358979323846F;  // PI (max boundary)
      constant_props.fov_min_el_rad[look_id] = -3.14159265358979323846F; // -PI (min boundary)
      constant_props.fov_max_el_rad[look_id] = 3.14159265358979323846F;  // PI (max boundary)

      /** \action
       * Validate calibration with FOV at exact boundaries
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return true for FOV at exact boundaries
       */
      CHECK_EQUAL(true, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV min at zero boundaries
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Valid_FOV_Zero_Min_Boundaries)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min to zero (valid boundary) for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_az_rad[look_id] = 0.0F;  // Zero is valid for min
      constant_props.fov_max_az_rad[look_id] = 0.01F; // Must be > min
      constant_props.fov_min_el_rad[look_id] = 0.0F;  // Zero is valid for min
      constant_props.fov_max_el_rad[look_id] = 0.01F; // Must be > min

      /** \action
       * Validate calibration with FOV min at zero
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return true for FOV min at zero boundary
       */
      CHECK_EQUAL(true, result);
   }
}

/** \purpose
 * Test sensor calibration validation with mounting position at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Long_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position longitude to NaN
    */
   constant_props.mounting_position.vcs_position.longitudinal = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate calibration with NaN mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for NaN mounting position longitude
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position latitude at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Lat_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position latitude to NaN
    */
   constant_props.mounting_position.vcs_position.lateral = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate calibration with NaN mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for NaN mounting position latitude
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position height at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Height_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position height to NaN
    */
   constant_props.mounting_position.vcs_position.height = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate calibration with NaN mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for NaN mounting position height
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with mounting position at positive infinity
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Mounting_Pos_Infinity)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set mounting position to positive infinity
    */
   constant_props.mounting_position.vcs_position.longitudinal = std::numeric_limits<float>::infinity();

   /** \action
    * Validate calibration with infinity mounting position
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for infinity mounting position
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test sensor calibration validation with FOV min azimuth at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Min_Az_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min azimuth to NaN for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_az_rad[look_id] = std::numeric_limits<float>::quiet_NaN();

      /** \action
       * Validate calibration with NaN FOV min azimuth
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for NaN FOV min azimuth
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV max azimuth at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Max_Az_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV max azimuth to NaN for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_max_az_rad[look_id] = std::numeric_limits<float>::quiet_NaN();

      /** \action
       * Validate calibration with NaN FOV max azimuth
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for NaN FOV max azimuth
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV min elevation at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Min_El_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV min elevation to NaN for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_min_el_rad[look_id] = std::numeric_limits<float>::quiet_NaN();

      /** \action
       * Validate calibration with NaN FOV min elevation
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for NaN FOV min elevation
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with FOV max elevation at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_FOV_Max_El_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set FOV max elevation to NaN for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.fov_max_el_rad[look_id] = std::numeric_limits<float>::quiet_NaN();

      /** \action
       * Validate calibration with NaN FOV max elevation
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for NaN FOV max elevation
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with range limit at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Range_Limit_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set range limit to NaN for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.range_limits[look_id] = std::numeric_limits<float>::quiet_NaN();

      /** \action
       * Validate calibration with NaN range limit
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for NaN range limit
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with velocity wrapping at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_V_Wrapping_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set velocity wrapping to NaN for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.v_wrapping[look_id] = std::numeric_limits<float>::quiet_NaN();

      /** \action
       * Validate calibration with NaN velocity wrapping
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for NaN velocity wrapping
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with minimum aliased range rate at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Min_Aliased_RR_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set minimum aliased range rate to NaN for each look ID
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      Initialize_Default_Constant_Props(constant_props);
      constant_props.min_aliaised_range_rate[look_id] = std::numeric_limits<float>::quiet_NaN();

      /** \action
       * Validate calibration with NaN minimum aliased range rate
       */
      bool result = RSPP_Check_Sensor_Calibration(constant_props);

      /** \result
       * Should return false for NaN minimum aliased range rate
       */
      CHECK_EQUAL(false, result);
   }
}

/** \purpose
 * Test sensor calibration validation with boresight azimuth at NaN
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Check_Sensor_Calibration, Check_Sensor_Calibration_TC_Invalid_Boresight_Az_NaN)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Set boresight azimuth angle to NaN
    */
   constant_props.mounting_position.vcs_boresight_azimuth_angle = std::numeric_limits<float>::quiet_NaN();

   /** \action
    * Validate calibration with NaN boresight azimuth
    */
   bool result = RSPP_Check_Sensor_Calibration(constant_props);

   /** \result
    * Should return false for NaN boresight azimuth
    */
   CHECK_EQUAL(false, result);
}

/** @}*/
