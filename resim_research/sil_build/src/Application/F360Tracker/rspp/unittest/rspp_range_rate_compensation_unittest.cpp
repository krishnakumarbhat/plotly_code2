/** \file
 * This file contains unit tests for content of rspp_range_rate_compensation.cpp file
 */

#include "rspp_range_rate_compensation.h"
#include <CppUTest/TestHarness.h>
#include <cstring>
#include <cmath>
#include <limits>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace rspp_variant_A;

/** \defgroup  test_RSPP_Calculate_Compensated_RRate
 *  @{
 */

/** \brief
 * Tests for RSPP_Calculate_Compensated_RRate function
 *
 * Integration function that:
 * 1. Validates input detection velocity data (returns bool)
 * 2. Calculates predicted range rate from ego vehicle motion
 * 3. Compensates raw range rate: compensated = raw + predicted
 * 4. Applies interval wrapping to keep within bounds
 * 5. Stores result in detection.processed.range_rate_compensated
 */
TEST_GROUP(test_RSPP_Calculate_Compensated_RRate)
{
   static constexpr float32_t TOLERANCE = 0.0001F;
   RSPP_Detection_T detection;
   VariableProps_T sensor_data;
   RSPP_Sensor_Calib_T rspp_sensor_calibration;
   bool result;

   /** \setup
    * Initialize vehicle velocity, detection, and interval width to zero/default values.
    */
   TEST_SETUP()
   {
      memset(&detection, 0, sizeof(RSPP_Detection_T));
      memset(&sensor_data, 0, sizeof(VariableProps_T));
      memset(&rspp_sensor_calibration, 0, sizeof(RSPP_Sensor_Calib_T));
      result = false;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/**************************************************************************
 * Basic Functionality Tests
 **************************************************************************/

/** \purpose
 * Test basic compensation with zero ego velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Zero_Ego_Velocity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero ego velocity, valid raw range rate
    */
   sensor_data.vcs_velocity.longitudinal = 0.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 10.0F; // Valid range rate
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated range rate equals raw (no ego motion)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(10.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test basic compensation with pure longitudinal ego velocity at 0deg azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Pure_Longitudinal_Zero_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Longitudinal ego velocity, detection straight ahead, valid raw range rate
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = -15.0F; // Object approaching in sensor frame
   detection.processed.vcs_az = 0.0F; // Straight ahead
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = raw + predicted = -15 + 20*cos(0) = -15 + 20 = 5.0 m/s
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(5.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test compensation with lateral ego velocity at 90deg azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Pure_Lateral_90_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Lateral ego velocity, detection at 90deg (left side), valid raw range rate
    */
   sensor_data.vcs_velocity.longitudinal = 0.0F;
   sensor_data.vcs_velocity.lateral = 5.0F; // Moving left
   detection.raw.range_rate = 8.0F;
   detection.processed.vcs_az = 1.5708F; // 90deg in radians
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = raw + predicted = 8 + 5*sin(90deg) = 8 + 5 = 13.0 m/s
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(13.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test compensation with combined longitudinal and lateral velocities at 45deg
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Combined_Velocities_45_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Both velocity components, 45deg azimuth, valid raw range rate
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 10.0F;
   detection.raw.range_rate = -5.0F;
   detection.processed.vcs_az = 0.7854F; // 45deg in radians
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = 20*cos(45deg) + 10*sin(45deg)
    * Compensated = -5 + predicted m/s
    * Returns true for valid input
    */
   float32_t predicted = 20.0F * std::cos(0.7854F) + 10.0F * std::sin(0.7854F);
   float32_t expected = -5.0F + predicted;
   CHECK_TRUE(result);
   DOUBLES_EQUAL(expected, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test zero raw range rate with ego motion
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Zero_Raw_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero raw range rate (stationary in sensor frame), ego moving forward
    */
   sensor_data.vcs_velocity.longitudinal = 15.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 0.0F;
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = 0 + 15 = 15.0 m/s (ego motion only)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(15.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/**************************************************************************
 * Range Rate Wrapping Tests
 **************************************************************************/

/** \purpose
 * Test that wrapping is applied when compensated range rate exceeds upper bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Wrapping_Above_Upper_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Compensation results in value above upper bound
    */
   sensor_data.vcs_velocity.longitudinal = 50.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 35.0F; // Raw + predicted = 35 + 50 = 85
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F; // Bounds: [-50, 50]

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Before wrapping: 85.0
    * After wrapping: 85 - 100 = -15.0 m/s
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-15.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test that wrapping is applied when compensated range rate is below lower bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Wrapping_Below_Lower_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Compensation results in value below lower bound
    */
   sensor_data.vcs_velocity.longitudinal = -40.0F; // Reversing
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = -45.0F; // Raw + predicted = -45 + (-40) = -85
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F; // Bounds: [-50, 50]

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Before wrapping: -85.0
    * After wrapping: -85 + 100 = 15.0 m/s
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(15.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test no wrapping when result is within bounds
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_No_Wrapping_Within_Bounds)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Compensation results in value within bounds
    */
   sensor_data.vcs_velocity.longitudinal = 10.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 5.0F; // Raw + predicted = 5 + 10 = 15
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F; // Bounds: [-50, 50]

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated: 15.0 (within bounds, no wrapping)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(15.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test wrapping with multiple intervals required
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Multiple_Interval_Wrapping)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very large compensation requiring multiple intervals
    */
   sensor_data.vcs_velocity.longitudinal = 100.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 85.0F; // Raw + predicted = 85 + 100 = 185
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F; // Bounds: [-50, 50]

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Before wrapping: 185.0
    * After wrapping: 185 - 200 = -15.0 m/s (wrapped by 2 intervals)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-15.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test wrapping with zero interval width (no wrapping applied)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Zero_Interval_Width)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero interval width (wrapping disabled)
    */
   sensor_data.vcs_velocity.longitudinal = 50.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 35.0F; // Raw + predicted = 85
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 0.0F; // No wrapping

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated: 85.0 (no wrapping applied)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(85.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/**************************************************************************
 * Input Validation Tests
 **************************************************************************/

/** \purpose
 * Test with valid range rate within [-200, 200] m/s
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Valid_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Valid range rate within interface limits
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 100.0F; // Valid: within [-128, 128]
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 500.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (valid input)
    * Compensated = 100 + 20 = 120.0 m/s
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(120.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test with invalid range rate above maximum (beyond 128.0 m/s limit)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Invalid_Range_Rate_Above_Max)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Invalid range rate above interface maximum (128.0 m/s)
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 201.0F; // Invalid: > 128.0
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 500.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns false (invalid input)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Test with invalid range rate below minimum (beyond -128.0 m/s limit)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Invalid_Range_Rate_Below_Min)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Invalid range rate below interface minimum (-128.0 m/s)
    */
   sensor_data.vcs_velocity.longitudinal = -20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = -201.0F; // Invalid: < -128.0
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 500.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns false (invalid input)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Test with NaN range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_NaN_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN range rate
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = std::numeric_limits<float32_t>::quiet_NaN();
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns false (invalid input)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Test with positive infinity range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Positive_Infinity_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Positive infinity range rate
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = std::numeric_limits<float32_t>::infinity();
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns false (invalid input)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Test with negative infinity range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Negative_Infinity_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Negative infinity range rate
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = -std::numeric_limits<float32_t>::infinity();
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns false (invalid input)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Test at exact upper boundary of valid range (128.0 m/s)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Exact_Upper_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Range rate exactly at upper boundary
    */
   sensor_data.vcs_velocity.longitudinal = 5.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 128.0F; // Exactly at boundary
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 500.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (valid at boundary)
    * Compensated = 128 + 5 = 133.0 m/s
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(133.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test at exact lower boundary of valid range (-128.0 m/s)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Exact_Lower_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Range rate exactly at lower boundary
    */
   sensor_data.vcs_velocity.longitudinal = -5.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = -128.0F; // Exactly at boundary
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 500.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (valid at boundary)
    * Compensated = -128 + (-5) = -133.0 m/s
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-133.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/**************************************************************************
 * Real-World Scenario Tests
 **************************************************************************/

/** \purpose
 * Test highway driving scenario with approaching vehicle
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Highway_Approaching_Vehicle)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Highway speed (33.3 m/s = 120 km/h), vehicle approaching ahead
    */
   sensor_data.vcs_velocity.longitudinal = 33.3F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = -20.0F; // Approaching in sensor frame
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = -20 + 33.3 = 13.3 m/s (relative closing speed)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(13.3F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test lane change scenario with lateral motion
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Lane_Change_Lateral_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Forward motion with lane change (lateral velocity), detection at 60deg
    */
   sensor_data.vcs_velocity.longitudinal = 25.0F;
   sensor_data.vcs_velocity.lateral = 3.0F;
   detection.raw.range_rate = -8.0F;
   detection.processed.vcs_az = 1.0472F; // 60deg in radians
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = 25*cos(60deg) + 3*sin(60deg)
    * Compensated = -8 + predicted m/s
    * Returns true for valid input
    */
   float32_t predicted = 25.0F * std::cos(1.0472F) + 3.0F * std::sin(1.0472F);
   float32_t expected = -8.0F + predicted;
   CHECK_TRUE(result);
   DOUBLES_EQUAL(expected, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test reversing scenario
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Reversing_Vehicle)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle reversing, detection behind
    */
   sensor_data.vcs_velocity.longitudinal = -5.0F; // Reversing
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 3.0F;      // Object receding in sensor frame
   detection.processed.vcs_az = 3.1416F; // 180deg (behind)
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = -5*cos(180deg) = -5*(-1) = 5.0
    * Compensated = 3 + 5 = 8.0 m/s
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(8.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test stationary vehicle with moving detection (overtaking)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Stationary_Ego_Moving_Object)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Ego stationary, fast-moving detection
    */
   sensor_data.vcs_velocity.longitudinal = 0.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 40.0F; // Fast approaching object
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = 40 + 0 = 40.0 m/s (object's true velocity)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(40.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test curved road scenario with combined velocities
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Curved_Road_Combined_Motion)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Both longitudinal and lateral velocity (curved road), detection at side
    */
   sensor_data.vcs_velocity.longitudinal = 22.0F; // ~80 km/h
   sensor_data.vcs_velocity.lateral = 4.0F;       // Turning
   detection.raw.range_rate = -12.0F;
   detection.processed.vcs_az = -1.5708F; // -90deg (right side)
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = 22*cos(-90deg) + 4*sin(-90deg) = 0 + 4*(-1) = -4.0
    * Compensated = -12 + (-4) = -16.0 m/s
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-16.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/**************************************************************************
 * Edge Cases and Special Conditions
 **************************************************************************/

/** \purpose
 * Test with very small interval width and high compensation
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Small_Interval_High_Compensation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Small interval width, high compensation value
    */
   sensor_data.vcs_velocity.longitudinal = 80.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 25.0F; // Raw + predicted = 105
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 10.0F; // Very small interval, bounds: [-5, 5]

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Before wrapping: 105.0
    * After wrapping: wrapped to within [-5, 5]
    * 105 mod 10 = 5, result at upper bound
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(5.0F, detection.processed.range_rate_compensated, TOLERANCE);
   CHECK(detection.processed.range_rate_compensated >= -5.0F);
   CHECK(detection.processed.range_rate_compensated <= 5.0F);
}

/** \purpose
 * Test with negative azimuth angle
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Negative_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Negative azimuth angle (right side of vehicle)
    */
   sensor_data.vcs_velocity.longitudinal = 30.0F;
   sensor_data.vcs_velocity.lateral = 5.0F;
   detection.raw.range_rate = -10.0F;
   detection.processed.vcs_az = -0.5236F; // -30deg in radians
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = 30*cos(-30deg) + 5*sin(-30deg)
    * Compensated = -10 + predicted m/s
    * Returns true for valid input
    */
   float32_t predicted = 30.0F * std::cos(-0.5236F) + 5.0F * std::sin(-0.5236F);
   float32_t expected = -10.0F + predicted;
   CHECK_TRUE(result);
   DOUBLES_EQUAL(expected, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test with all zero inputs
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_All_Zero_Inputs)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * All inputs zero
    */
   sensor_data.vcs_velocity.longitudinal = 0.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 0.0F;
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = 0 + 0 = 0.0 m/s
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(0.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test compensation exactly at wrapping boundary
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Exact_Wrapping_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Compensation results in exactly upper bound value
    */
   sensor_data.vcs_velocity.longitudinal = 30.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 20.0F; // Raw + predicted = 50 (at upper bound)
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F; // Bounds: [-50, 50]

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = 50.0 (at boundary, no wrapping)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(50.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test with large azimuth angle (near 180deg)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Large_Azimuth_180)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection behind vehicle (180deg azimuth)
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 15.0F;
   detection.processed.vcs_az = 3.1416F; // 180deg in radians
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = 20*cos(180deg) = 20*(-1) = -20.0
    * Compensated = 15 + (-20) = -5.0 m/s
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-5.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test negative ego velocities (both components)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Negative_Ego_Velocities)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Both velocity components negative
    */
   sensor_data.vcs_velocity.longitudinal = -15.0F;
   sensor_data.vcs_velocity.lateral = -5.0F;
   detection.raw.range_rate = 10.0F;
   detection.processed.vcs_az = 0.7854F; // 45deg in radians
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = -15*cos(45deg) + (-5)*sin(45deg)
    * Compensated = 10 + predicted m/s
    * Returns true for valid input
    */
   float32_t predicted = -15.0F * std::cos(0.7854F) + (-5.0F) * std::sin(0.7854F);
   float32_t expected = 10.0F + predicted;
   CHECK_TRUE(result);
   DOUBLES_EQUAL(expected, detection.processed.range_rate_compensated, TOLERANCE);
}

/**************************************************************************
 * Combined Edge Cases
 **************************************************************************/

/** \purpose
 * Test invalid input with wrapping required
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Invalid_Input_With_Wrapping)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Invalid range rate but wrapping still applied
    */
   sensor_data.vcs_velocity.longitudinal = 50.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 250.0F; // Invalid: > 200
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns false (invalid input)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Test maximum valid values for all inputs
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Maximum_Valid_Values)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Maximum realistic values for all inputs
    */
   sensor_data.vcs_velocity.longitudinal = 55.0F; // ~200 km/h
   sensor_data.vcs_velocity.lateral = 15.0F;
   detection.raw.range_rate = 127.0F;                                // Just below limit
   detection.processed.vcs_az = 0.5236F;                             // 30deg
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 500.0F; // Large interval

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = 55*cos(30deg) + 15*sin(30deg)
    * Compensated (before wrapping) = 127 + predicted m/s
    * Compensated (after wrapping within [-250, 250])
    * Returns true for valid input
    */
   float32_t predicted = 55.0F * std::cos(0.5236F) + 15.0F * std::sin(0.5236F);
   float32_t expected = 127.0F + predicted;
   CHECK_TRUE(result);
   DOUBLES_EQUAL(expected, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test minimum valid values (most negative)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Minimum_Valid_Values)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Most negative valid values
    */
   sensor_data.vcs_velocity.longitudinal = -55.0F;
   sensor_data.vcs_velocity.lateral = -15.0F;
   detection.raw.range_rate = -127.0F;
   detection.processed.vcs_az = 2.0944F; // 120deg
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 500.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = -55*cos(120deg) + (-15)*sin(120deg)
    * Compensated = -127 + predicted m/s
    * Returns true for valid input
    */
   float32_t predicted = -55.0F * std::cos(2.0944F) + (-15.0F) * std::sin(2.0944F);
   float32_t expected = -127.0F + predicted;
   CHECK_TRUE(result);
   DOUBLES_EQUAL(expected, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test fractional interval width with compensation
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Fractional_Interval_Width)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Fractional interval width
    */
   sensor_data.vcs_velocity.longitudinal = 25.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 40.0F; // Raw + predicted = 65
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 50.5F; // Fractional interval, bounds: [-25.25, 25.25]

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Before wrapping: 65.0
    * After wrapping: 65 - 50.5 = 14.5 m/s (within bounds)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(14.5F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test with very small ego velocities and detection near boundary
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Small_Velocities_Near_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very small ego velocities, detection near wrapping boundary
    */
   sensor_data.vcs_velocity.longitudinal = 0.5F;
   sensor_data.vcs_velocity.lateral = 0.2F;
   detection.raw.range_rate = 49.5F;     // Near boundary
   detection.processed.vcs_az = 0.7854F; // 45deg
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * predicted = 0.5*cos(45deg) + 0.2*sin(45deg)
    * Compensated = 49.5 + predicted (just under upper bound)
    * Returns true for valid input
    */
   float32_t predicted = 0.5F * std::cos(0.7854F) + 0.2F * std::sin(0.7854F);
   float32_t expected = 49.5F + predicted;
   CHECK_TRUE(result);
   DOUBLES_EQUAL(expected, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test that v_wrapping array is indexed with correct look_id
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Correct_Look_ID_Selection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Different v_wrapping values for each look_id
    * Set up scenario where wrapping will produce different results
    */
   // Configure different v_wrapping for each look_id
   rspp_sensor_calibration.v_wrapping[RSPP_DET_LOOK_ID_0] = 100.0F; // Bounds: [-50, 50]
   rspp_sensor_calibration.v_wrapping[RSPP_DET_LOOK_ID_1] = 50.0F;  // Bounds: [-25, 25]
   rspp_sensor_calibration.v_wrapping[RSPP_DET_LOOK_ID_2] = 40.0F;  // Bounds: [-20, 20]
   rspp_sensor_calibration.v_wrapping[RSPP_DET_LOOK_ID_3] = 30.0F;  // Bounds: [-15, 15]

   // Common test parameters that will exceed bounds for smaller intervals
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 15.0F; // Raw + predicted = 35.0
   detection.processed.vcs_az = 0.0F;

   /** \action
    * Test with look_id = RSPP_DET_LOOK_ID_0
    */
   sensor_data.look_id = RSPP_DET_LOOK_ID_0;
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * v_wrapping[0] = 100.0F (bounds: [-50, 50])
    * Compensated = 35.0 (no wrapping needed)
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(35.0F, detection.processed.range_rate_compensated, TOLERANCE);

   /** \action
    * Test with look_id = RSPP_DET_LOOK_ID_1
    */
   sensor_data.look_id = RSPP_DET_LOOK_ID_1;
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * v_wrapping[1] = 50.0F (bounds: [-25, 25])
    * Before wrapping: 35.0 > 25.0 (exceeds upper bound)
    * After wrapping: 35.0 - 50.0 = -15.0
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-15.0F, detection.processed.range_rate_compensated, TOLERANCE);

   /** \action
    * Test with look_id = RSPP_DET_LOOK_ID_2
    */
   sensor_data.look_id = RSPP_DET_LOOK_ID_2;
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * v_wrapping[2] = 40.0F (bounds: [-20, 20])
    * Before wrapping: 35.0 > 20.0 (exceeds upper bound)
    * After wrapping: 35.0 - 40.0 = -5.0
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-5.0F, detection.processed.range_rate_compensated, TOLERANCE);

   /** \action
    * Test with look_id = RSPP_DET_LOOK_ID_3
    */
   sensor_data.look_id = RSPP_DET_LOOK_ID_3;
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * v_wrapping[3] = 30.0F (bounds: [-15, 15])
    * Before wrapping: 35.0 > 15.0 (exceeds upper bound)
    * After wrapping: 35.0 - 30.0 = 5.0
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(5.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test wrapping with exact interval multiple below lower bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Exact_Multiple_Below_Lower_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set up compensated value to be exactly one interval below lower bound
    * v_wrapping = 100.0F, bounds: [-50, 50]
    * Target: lowerBound - 1*w = -50 - 100 = -150
    * Note: Raw range_rate must be in [-128, 128]
    */
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   // Configure to produce exactly -150.0 before wrapping
   // predicted = longitudinal * cos(0) = -25.0
   // raw = -125.0 (within valid range)
   // compensated = -125.0 + (-25.0) = -150.0
   sensor_data.vcs_velocity.longitudinal = -25.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = -125.0F;
   detection.processed.vcs_az = 0.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Before wrapping: -150.0 = lowerBound - 1*w
    * intervals_to_add = ceil((-50 - (-150)) / 100) = ceil(100/100) = 1
    * After wrapping: -150.0 + 100.0 = -50.0 (exactly at lower bound)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-50.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test wrapping with exact interval multiple above upper bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Exact_Multiple_Above_Upper_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set up compensated value to be exactly one interval above upper bound
    * v_wrapping = 100.0F, bounds: [-50, 50]
    * Target: upperBound + 1*w = 50 + 100 = 150
    * Note: Raw range_rate must be in [-128, 128]
    */
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   // Configure to produce exactly 150.0 before wrapping
   // predicted = longitudinal * cos(0) = 25.0
   // raw = 125.0 (within valid range)
   // compensated = 125.0 + 25.0 = 150.0
   sensor_data.vcs_velocity.longitudinal = 25.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 125.0F;
   detection.processed.vcs_az = 0.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Before wrapping: 150.0 = upperBound + 1*w
    * intervals_to_subtract = ceil((150 - 50) / 100) = ceil(100/100) = 1
    * After wrapping: 150.0 - 100.0 = 50.0 (exactly at upper bound)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(50.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/**************************************************************************
 * Additional Enhancement Tests - Addressing Review Recommendations
 **************************************************************************/

/** \purpose
 * Test NaN in longitudinal ego velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_NaN_Longitudinal_Velocity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN in longitudinal velocity, valid raw range rate
    */
   sensor_data.vcs_velocity.longitudinal = std::numeric_limits<float32_t>::quiet_NaN();
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 10.0F; // Valid range rate
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (raw range rate is valid)
    * But compensated result is NaN due to NaN * cos(0)
    */
   CHECK_TRUE(result);
   CHECK(std::isnan(detection.processed.range_rate_compensated));
}

/** \purpose
 * Test NaN in lateral ego velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_NaN_Lateral_Velocity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN in lateral velocity, valid raw range rate
    */
   sensor_data.vcs_velocity.longitudinal = 0.0F;
   sensor_data.vcs_velocity.lateral = std::numeric_limits<float32_t>::quiet_NaN();
   detection.raw.range_rate = 10.0F;     // Valid range rate
   detection.processed.vcs_az = 1.5708F; // 90deg
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (raw range rate is valid)
    * But compensated result is NaN due to NaN * sin(90deg)
    */
   CHECK_TRUE(result);
   CHECK(std::isnan(detection.processed.range_rate_compensated));
}

/** \purpose
 * Test infinity in longitudinal ego velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Infinity_Longitudinal_Velocity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Positive infinity in longitudinal velocity
    */
   sensor_data.vcs_velocity.longitudinal = std::numeric_limits<float32_t>::infinity();
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 10.0F; // Valid range rate
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (raw range rate is valid)
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Test NaN in azimuth angle
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_NaN_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN in azimuth angle, valid velocities and range rate
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 5.0F;
   detection.raw.range_rate = 10.0F; // Valid range rate
   detection.processed.vcs_az = std::numeric_limits<float32_t>::quiet_NaN();
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (raw range rate is valid)
    * Compensated result is NaN due to cos(NaN) and sin(NaN)
    */
   CHECK_TRUE(result);
   CHECK(std::isnan(detection.processed.range_rate_compensated));
}

/** \purpose
 * Test negative interval width (should behave like zero - no wrapping)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Negative_Interval_Width)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Negative interval width (invalid, wrapping should not apply)
    */
   sensor_data.vcs_velocity.longitudinal = 50.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 35.0F; // Raw + predicted = 85
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = -100.0F; // Negative interval

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (raw range rate is valid)
    * Compensated = 85.0 (no wrapping applied with negative interval)
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(85.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test combined NaN in both velocity and range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Combined_NaN_Velocity_And_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN in both ego velocity and raw range rate
    */
   sensor_data.vcs_velocity.longitudinal = std::numeric_limits<float32_t>::quiet_NaN();
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = std::numeric_limits<float32_t>::quiet_NaN();
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns false (raw range rate is invalid)
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Test very large azimuth angle (beyond +-pi)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Very_Large_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very large azimuth angle (10 radians, ~3.18pi)
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 5.0F;
   detection.raw.range_rate = 10.0F;
   detection.processed.vcs_az = 10.0F; // ~573deg (multiple revolutions)
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (raw range rate is valid)
    * Trigonometric functions handle large angles
    * predicted = 20*cos(10) + 5*sin(10)
    * Compensated = 10 + predicted m/s
    */
   float32_t predicted = 20.0F * std::cos(10.0F) + 5.0F * std::sin(10.0F);
   float32_t expected = 10.0F + predicted;
   CHECK_TRUE(result);
   DOUBLES_EQUAL(expected, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test exact zero result after compensation
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Exact_Zero_After_Compensation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Values chosen to result in exactly zero after compensation
    */
   sensor_data.vcs_velocity.longitudinal = -10.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 10.0F; // Raw + predicted = 10 + (-10) = 0
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns true (valid input)
    * Compensated = 0.0 m/s exactly
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(0.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test sequential compensation calls to verify proper overwriting
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Sequential_Calls)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * First call with one set of values
    */
   sensor_data.vcs_velocity.longitudinal = 20.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 10.0F;
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * First calculation
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * First result: 10 + 20 = 30.0 m/s
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(30.0F, detection.processed.range_rate_compensated, TOLERANCE);

   /** \precond
    * Second call with different values on same detection struct
    */
   sensor_data.vcs_velocity.longitudinal = 15.0F;
   detection.raw.range_rate = -5.0F;

   /** \action
    * Second calculation (should overwrite previous result)
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Second result: -5 + 15 = 10.0 m/s (previous value completely overwritten)
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(10.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test wrapping at exact half-interval boundary
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Wrapping_At_Exact_Half_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Compensation results in exactly rspp_sensor_calibration.v_wrapping[sensor_data.look_id] / 2
    */
   sensor_data.vcs_velocity.longitudinal = 25.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = -25.0F; // Raw + predicted = 0
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F; // Half interval = 50.0

   /** \action
    * Adjust to hit exact half-interval boundary
    */
   detection.raw.range_rate = 25.0F; // Raw + predicted = 50.0 (exactly at upper bound)
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = 50.0 (at upper bound, no wrapping needed)
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(50.0F, detection.processed.range_rate_compensated, TOLERANCE);

   /** \action
    * Test at negative half-interval boundary
    */
   detection.raw.range_rate = -75.0F; // Raw + predicted = -50.0 (exactly at lower bound)
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Compensated = -50.0 (at lower bound, no wrapping needed)
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(-50.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test combined infinity in velocity and valid range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Infinity_Lateral_Velocity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Negative infinity in lateral velocity
    */
   sensor_data.vcs_velocity.longitudinal = 0.0F;
   sensor_data.vcs_velocity.lateral = -std::numeric_limits<float32_t>::infinity();
   detection.raw.range_rate = 10.0F;     // Valid
   detection.processed.vcs_az = 1.5708F; // 90deg
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 100.0F;

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Returns false (raw range rate is valid)
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Test very small interval with exact compensation value
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate, Calculate_Compensated_RRate_TC_Very_Small_Interval_Exact_Value)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very small interval width with compensation exactly at multiple of interval
    */
   sensor_data.vcs_velocity.longitudinal = 1.0F;
   sensor_data.vcs_velocity.lateral = 0.0F;
   detection.raw.range_rate = 9.0F; // Raw + predicted = 10.0
   detection.processed.vcs_az = 0.0F;
   rspp_sensor_calibration.v_wrapping[sensor_data.look_id] = 0.5F; // Bounds: [-0.25, 0.25]

   /** \action
    * Calculate compensated range rate
    */
   result = RSPP_Calculate_Compensated_RRate(
       detection.raw, sensor_data, rspp_sensor_calibration, detection.processed);

   /** \result
    * Before wrapping: 10.0
    * After wrapping: 10.0 mod 0.5 = 0.0 (exactly at zero)
    * Returns true for valid input
    */
   CHECK_TRUE(result);
   DOUBLES_EQUAL(0.0F, detection.processed.range_rate_compensated, TOLERANCE);
}

/** @}*/

/** \defgroup  test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping
 *  @{
 */

/** \brief
 * Tests for RSPP_Apply_Range_Rate_Wrapping function
 *
 * Validates interval wrapping to keep range rate within specified bounds.
 * Bounds: [-interval_width/2, +interval_width/2]
 * Wrapping adds/subtracts multiples of interval_width to bring value into range.
 */
TEST_GROUP(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping)
{
   static constexpr float32_t TOLERANCE = 0.0001F;
   float32_t interval_width;
   float32_t compensated_range_rate;

   /** \setup
    * Initialize interval width and compensated range rate to zero.
    */
   TEST_SETUP()
   {
      interval_width = 0.0F;
      compensated_range_rate = 0.0F;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

// ============================================================================
// Basic Functionality Tests
// ============================================================================

/** \purpose
 * Test with value within bounds (no wrapping needed)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Within_Bounds)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value within interval bounds
    */
   interval_width = 100.0F; // Bounds: [-50, +50]
   compensated_range_rate = 25.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain unchanged
    */
   DOUBLES_EQUAL(25.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with value at zero
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Zero_Value)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero range rate
    */
   interval_width = 100.0F;
   compensated_range_rate = 0.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain zero
    */
   DOUBLES_EQUAL(0.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with positive overflow requiring wrapping
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Positive_Overflow)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value exceeding upper bound
    */
   interval_width = 100.0F; // Bounds: [-50, +50]
   compensated_range_rate = 75.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped to negative side (75 - 100 = -25)
    */
   DOUBLES_EQUAL(-25.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with negative overflow requiring wrapping
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Negative_Overflow)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value below lower bound
    */
   interval_width = 100.0F; // Bounds: [-50, +50]
   compensated_range_rate = -75.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped to positive side (-75 + 100 = 25)
    */
   DOUBLES_EQUAL(25.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with value slightly above upper bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Slightly_Above_Upper_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value just above upper bound
    */
   interval_width = 100.0F; // Bounds: [-50, +50]
   compensated_range_rate = 50.5F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (50.5 - 100 = -49.5)
    */
   DOUBLES_EQUAL(-49.5F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with value slightly below lower bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Slightly_Below_Lower_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value just below lower bound
    */
   interval_width = 100.0F; // Bounds: [-50, +50]
   compensated_range_rate = -50.5F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (-50.5 + 100 = 49.5)
    */
   DOUBLES_EQUAL(49.5F, compensated_range_rate, TOLERANCE);
}

// ============================================================================
// Boundary Tests
// ============================================================================

/** \purpose
 * Test with value exactly at upper bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Exactly_At_Upper_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value exactly at upper bound (inclusive)
    */
   interval_width = 100.0F;
   compensated_range_rate = 50.0F; // Exactly at +interval_width/2

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain at boundary (within bounds)
    */
   DOUBLES_EQUAL(50.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with value exactly at lower bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Exactly_At_Lower_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value exactly at lower bound (inclusive)
    */
   interval_width = 100.0F;
   compensated_range_rate = -50.0F; // Exactly at -interval_width/2

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain at boundary (within bounds)
    */
   DOUBLES_EQUAL(-50.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with value very close to but within upper bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Just_Below_Upper_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value just below upper bound
    */
   interval_width = 100.0F;
   compensated_range_rate = 49.99F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain unchanged (within bounds)
    */
   DOUBLES_EQUAL(49.99F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with value very close to but within lower bound
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Just_Above_Lower_Bound)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value just above lower bound
    */
   interval_width = 100.0F;
   compensated_range_rate = -49.99F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain unchanged (within bounds)
    */
   DOUBLES_EQUAL(-49.99F, compensated_range_rate, TOLERANCE);
}

// ============================================================================
// Multiple Interval Wrapping Tests
// ============================================================================

/** \purpose
 * Test with value exceeding by two intervals
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Two_Intervals_Positive)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value exceeding by more than one interval
    */
   interval_width = 100.0F;
   compensated_range_rate = 175.0F; // Exceeds by 1.25 intervals

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped by 2 intervals (175 - 200 = -25)
    */
   DOUBLES_EQUAL(-25.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with value below by two intervals
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Two_Intervals_Negative)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value below by more than one interval
    */
   interval_width = 100.0F;
   compensated_range_rate = -175.0F; // Below by 1.25 intervals

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped by 2 intervals (-175 + 200 = 25)
    */
   DOUBLES_EQUAL(25.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with very large positive value
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Very_Large_Positive)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very large positive value (multiple intervals)
    */
   interval_width = 100.0F;
   compensated_range_rate = 525.0F; // 5+ intervals beyond upper bound

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped to within bounds (525 - 600 = -75, then -75 + 100 = 25)
    */
   CHECK(compensated_range_rate >= -50.0F && compensated_range_rate <= 50.0F);
   DOUBLES_EQUAL(25.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with very large negative value
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Very_Large_Negative)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very large negative value (multiple intervals)
    */
   interval_width = 100.0F;
   compensated_range_rate = -525.0F; // 5+ intervals below lower bound

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped to within bounds (-525 + 600 = 75, then 75 - 100 = -25)
    */
   CHECK(compensated_range_rate >= -50.0F && compensated_range_rate <= 50.0F);
   DOUBLES_EQUAL(-25.0F, compensated_range_rate, TOLERANCE);
}

// ============================================================================
// Different Interval Width Tests
// ============================================================================

/** \purpose
 * Test with small interval width
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Small_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Small interval width (10 m/s)
    */
   interval_width = 10.0F; // Bounds: [-5, +5]
   compensated_range_rate = 7.5F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (7.5 - 10 = -2.5)
    */
   DOUBLES_EQUAL(-2.5F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with large interval width
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Large_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Large interval width (200 m/s)
    */
   interval_width = 200.0F; // Bounds: [-100, +100]
   compensated_range_rate = 75.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain unchanged (within bounds)
    */
   DOUBLES_EQUAL(75.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with interval width of 1
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Unit_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Unit interval width
    */
   interval_width = 1.0F; // Bounds: [-0.5, +0.5]
   compensated_range_rate = 0.75F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (0.75 - 1.0 = -0.25)
    */
   DOUBLES_EQUAL(-0.25F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with very small interval width
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Very_Small_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very small interval
    */
   interval_width = 0.1F; // Bounds: [-0.05, +0.05]
   compensated_range_rate = 0.08F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (0.08 - 0.1 = -0.02)
    */
   DOUBLES_EQUAL(-0.02F, compensated_range_rate, TOLERANCE);
}

// ============================================================================
// Edge Case Tests
// ============================================================================

/** \purpose
 * Test with zero interval width (no wrapping should occur)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Zero_Interval_Width)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero interval width
    */
   interval_width = 0.0F;
   compensated_range_rate = 75.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain unchanged (no wrapping when interval_width <= 0)
    */
   DOUBLES_EQUAL(75.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with negative interval width (no wrapping should occur)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Negative_Interval_Width)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Negative interval width (invalid but handled)
    */
   interval_width = -100.0F;
   compensated_range_rate = 75.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain unchanged (no wrapping when interval_width <= 0)
    */
   DOUBLES_EQUAL(75.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with NaN in range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_NaN_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN range rate
    */
   interval_width = 100.0F;
   compensated_range_rate = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * NaN should remain NaN (comparisons with NaN are false)
    */
   CHECK(std::isnan(compensated_range_rate));
}

/** \purpose
 * Test with positive infinity in range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Positive_Infinity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Positive infinity range rate
    */
   interval_width = 100.0F;
   compensated_range_rate = std::numeric_limits<float32_t>::infinity();

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Infinity handling depends on implementation (likely remains or becomes NaN)
    */
   // After wrapping attempt with infinity, result behavior is implementation-defined
   CHECK(std::isinf(compensated_range_rate) || std::isnan(compensated_range_rate));
}

/** \purpose
 * Test with negative infinity in range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Negative_Infinity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Negative infinity range rate
    */
   interval_width = 100.0F;
   compensated_range_rate = -std::numeric_limits<float32_t>::infinity();

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Infinity handling depends on implementation
    */
   CHECK(std::isinf(compensated_range_rate) || std::isnan(compensated_range_rate));
}

/** \purpose
 * Test with NaN in interval width
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_NaN_Interval_Width)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN interval width
    */
   interval_width = std::numeric_limits<float32_t>::quiet_NaN();
   compensated_range_rate = 75.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain unchanged (NaN > 0.0F is false)
    */
   DOUBLES_EQUAL(75.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with infinity in interval width
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Infinity_Interval_Width)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Infinity interval width (effectively no bounds)
    */
   interval_width = std::numeric_limits<float32_t>::infinity();
   compensated_range_rate = 1000.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value should remain unchanged (always within infinite bounds)
    */
   DOUBLES_EQUAL(1000.0F, compensated_range_rate, TOLERANCE);
}

// ============================================================================
// Precision and Fractional Tests
// ============================================================================

/** \purpose
 * Test with fractional interval width
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Fractional_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Fractional interval width
    */
   interval_width = 50.5F; // Bounds: [-25.25, +25.25]
   compensated_range_rate = 30.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (30 - 50.5 = -20.5)
    */
   DOUBLES_EQUAL(-20.5F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with fractional range rate value
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Fractional_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Fractional range rate
    */
   interval_width = 100.0F;
   compensated_range_rate = 75.75F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (75.75 - 100 = -24.25)
    */
   DOUBLES_EQUAL(-24.25F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test wrapping precision with values requiring exact calculation
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Precision_Test)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Values requiring precise calculation
    */
   interval_width = 99.99F;
   compensated_range_rate = 149.985F; // Exactly 1.5 intervals

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped precisely
    */
   float32_t expected = 149.985F - (2.0F * 99.99F); // Wraps by 2 intervals
   DOUBLES_EQUAL(expected, compensated_range_rate, TOLERANCE);
}

// ============================================================================
// Symmetry Tests
// ============================================================================

/** \purpose
 * Test symmetry of positive and negative wrapping
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Symmetry)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Equal magnitude positive and negative values
    */
   interval_width = 100.0F;

   float32_t positive_value = 75.0F;
   float32_t negative_value = -75.0F;

   /** \action
    * Apply wrapping to both
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, positive_value);
   RSPP_Apply_Range_Rate_Wrapping(interval_width, negative_value);

   /** \result
    * Results should be symmetric (75 -> -25, -75 -> 25)
    */
   DOUBLES_EQUAL(-25.0F, positive_value, TOLERANCE);
   DOUBLES_EQUAL(25.0F, negative_value, TOLERANCE);
   DOUBLES_EQUAL(-positive_value, negative_value, TOLERANCE);
}

// ============================================================================
// Real-World Scenario Tests
// ============================================================================

/** \purpose
 * Test typical radar range rate wrapping scenario
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Typical_Radar_Scenario)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Typical automotive radar parameters (+-50 m/s range)
    */
   interval_width = 100.0F;
   compensated_range_rate = 62.5F; // Slightly above ambiguity

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped to resolve ambiguity
    */
   DOUBLES_EQUAL(-37.5F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test high-speed scenario with large compensation
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_High_Speed_Scenario)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * High-speed scenario (highway speeds, large ego compensation)
    */
   interval_width = 100.0F;
   compensated_range_rate = 85.0F; // After ego vehicle compensation

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (85 - 100 = -15)
    */
   DOUBLES_EQUAL(-15.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test with value exactly at one interval beyond
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Exactly_One_Interval_Beyond)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value exactly one full interval beyond bound
    */
   interval_width = 100.0F;
   compensated_range_rate = 150.0F; // Exactly at upper_bound + interval

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped by exactly one interval (150 - 100 = 50, at upper bound)
    */
   DOUBLES_EQUAL(50.0F, compensated_range_rate, TOLERANCE);
}

/**************************************************************************
 * Additional Enhancement Tests - Addressing Review Recommendations
 **************************************************************************/

/** \purpose
 * Test sequential wrapping (idempotence) - applying wrapping twice should not change result
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Sequential_Wrapping_Idempotence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value that requires wrapping
    */
   interval_width = 100.0F;
   compensated_range_rate = 175.0F; // Requires wrapping

   /** \action
    * Apply wrapping first time
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);
   float32_t first_result = compensated_range_rate;

   /** \result
    * First wrapping produces -25.0F
    */
   DOUBLES_EQUAL(-25.0F, first_result, TOLERANCE);

   /** \action
    * Apply wrapping second time on already wrapped value
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Second wrapping produces same result (idempotence property)
    */
   DOUBLES_EQUAL(first_result, compensated_range_rate, TOLERANCE);
   DOUBLES_EQUAL(-25.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test value exactly at half interval width (positive)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Exactly_At_Positive_Half_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value exactly at upper bound (interval_width / 2)
    */
   interval_width = 100.0F;
   compensated_range_rate = 50.0F; // Exactly interval_width / 2

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value at upper bound should remain unchanged (within valid range)
    */
   DOUBLES_EQUAL(50.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test value exactly at negative half interval width
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Exactly_At_Negative_Half_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Value exactly at lower bound (-interval_width / 2)
    */
   interval_width = 100.0F;
   compensated_range_rate = -50.0F; // Exactly -interval_width / 2

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value at lower bound should remain unchanged (within valid range)
    */
   DOUBLES_EQUAL(-50.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test combined edge case: very small interval with large range rate
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Small_Interval_Large_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very small interval with large range rate (many intervals to wrap)
    */
   interval_width = 0.5F;
   compensated_range_rate = 100.0F; // Requires 200 intervals of wrapping

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped to within [-0.25, 0.25] bounds
    * 100.0 mod 0.5 = 0, so result should be 0.0
    */
   DOUBLES_EQUAL(0.0F, compensated_range_rate, TOLERANCE);
   CHECK(compensated_range_rate >= -0.25F);
   CHECK(compensated_range_rate <= 0.25F);
}

/** \purpose
 * Test combined edge case: large interval with small range rate near boundary
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Large_Interval_Small_Range_Rate_Near_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Large interval with small range rate that's just outside bounds
    */
   interval_width = 1000.0F;
   compensated_range_rate = 500.5F; // Just beyond upper bound of 500.0

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped (500.5 - 1000 = -499.5)
    */
   DOUBLES_EQUAL(-499.5F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test multiple sequential wrapping operations with different intervals
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Multiple_Different_Intervals)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Starting value and multiple interval widths to test
    */
   compensated_range_rate = 175.0F;

   /** \action
    * Apply wrapping with first interval (100.0)
    */
   interval_width = 100.0F;
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * First wrapping: 175 - 200 = -25
    */
   DOUBLES_EQUAL(-25.0F, compensated_range_rate, TOLERANCE);

   /** \action
    * Apply wrapping with second interval (50.0) on the wrapped value
    */
   interval_width = 50.0F;
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Second wrapping: -25 is already within [-25, 25], stays at -25
    */
   DOUBLES_EQUAL(-25.0F, compensated_range_rate, TOLERANCE);

   /** \action
    * Apply wrapping with third interval (20.0) on the wrapped value
    */
   interval_width = 20.0F;
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Third wrapping: -25 is outside [-10, 10], wraps to -5
    * -25 + 20 = -5
    */
   DOUBLES_EQUAL(-5.0F, compensated_range_rate, TOLERANCE);
}

/** \purpose
 * Test wrapping with prime number intervals for edge case coverage
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Prime_Number_Interval)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Prime number interval width
    */
   interval_width = 17.0F; // Prime number
   compensated_range_rate = 25.0F;

   /** \action
    * Apply wrapping
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Value wrapped to within [-8.5, 8.5]
    * 25 - 17 = 8, which is within bounds
    */
   DOUBLES_EQUAL(8.0F, compensated_range_rate, TOLERANCE);
   CHECK(compensated_range_rate >= -8.5F);
   CHECK(compensated_range_rate <= 8.5F);
}

/** \purpose
 * Test wrapping consistency with positive and negative equivalent values
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Apply_RRate_Wrapping,
     Apply_RRate_Wrapping_TC_Positive_Negative_Equivalence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Two values that should wrap to opposite but equal magnitude results
    */
   interval_width = 100.0F;
   compensated_range_rate = 175.0F;

   /** \action
    * Apply wrapping to positive value
    */
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);
   float32_t positive_result = compensated_range_rate;

   /** \result
    * Positive value wraps to -25.0F
    */
   DOUBLES_EQUAL(-25.0F, positive_result, TOLERANCE);

   /** \action
    * Apply wrapping to equivalent negative value
    */
   compensated_range_rate = -175.0F;
   RSPP_Apply_Range_Rate_Wrapping(interval_width, compensated_range_rate);

   /** \result
    * Negative value wraps to +25.0F (equal magnitude, opposite sign)
    */
   DOUBLES_EQUAL(25.0F, compensated_range_rate, TOLERANCE);
   DOUBLES_EQUAL(positive_result, -compensated_range_rate, TOLERANCE);
}

/** @}*/

/** \defgroup  test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate
 *  @{
 */

/** \brief
 * Tests for RSPP_Calculate_Ego_Motion_Range_Rate  function
 *
 * Validates calculation of predicted range rate based on ego vehicle motion.
 * Formula: r_pred = v_long * cos(theta_az) + v_lat * sin(theta_az)
 */
TEST_GROUP(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate)
{
   static constexpr float32_t TOLERANCE = 0.0001F;
   RSPP_VCS_Velocity_T sensor_vcs_velocity;
   float32_t detection_azimuth;

   /** \setup
    * Initialize vehicle velocity and detection azimuth to zero.
    */
   TEST_SETUP()
   {
      memset(&sensor_vcs_velocity, 0, sizeof(RSPP_VCS_Velocity_T));
      detection_azimuth = 0.0F;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

// ============================================================================
// Basic Functionality Tests
// ============================================================================

/** \purpose
 * Test with zero velocity and zero azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Zero_Velocity_Zero_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero vehicle velocity, detection at zero azimuth
    */
   sensor_vcs_velocity.longitudinal = 0.0F;
   sensor_vcs_velocity.lateral = 0.0F;
   detection_azimuth = 0.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should be zero
    */
   DOUBLES_EQUAL(0.0F, result, TOLERANCE);
}

/** \purpose
 * Test with pure longitudinal velocity and zero azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Pure_Longitudinal_Zero_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle moving forward at 25 m/s, detection directly ahead
    */
   sensor_vcs_velocity.longitudinal = 25.0F;
   sensor_vcs_velocity.lateral = 0.0F;
   detection_azimuth = 0.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should equal longitudinal velocity (cos(0) = 1)
    */
   DOUBLES_EQUAL(25.0F, result, TOLERANCE);
}

/** \purpose
 * Test with pure lateral velocity and 90 degree azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Pure_Lateral_90_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle with lateral velocity, detection at 90 degrees
    */
   sensor_vcs_velocity.longitudinal = 0.0F;
   sensor_vcs_velocity.lateral = 10.0F;
   detection_azimuth = M_PI / 2.0F; // 90 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should equal lateral velocity (sin(pi/2) = 1)
    */
   DOUBLES_EQUAL(10.0F, result, TOLERANCE);
}

/** \purpose
 * Test with combined longitudinal and lateral velocity at 45 degrees
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Combined_Velocity_45_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle with both velocity components, detection at 45 degrees
    */
   sensor_vcs_velocity.longitudinal = 20.0F;
   sensor_vcs_velocity.lateral = 10.0F;
   detection_azimuth = M_PI / 4.0F; // 45 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result = 20*cos(45deg) + 10*sin(45deg)
    */
   float32_t expected = 20.0F * std::cos(M_PI / 4.0F) + 10.0F * std::sin(M_PI / 4.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

// ============================================================================
// Azimuth Angle Variation Tests
// ============================================================================

/** \purpose
 * Test with 180 degree azimuth (behind vehicle)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_180_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Forward velocity, detection behind vehicle
    */
   sensor_vcs_velocity.longitudinal = 25.0F;
   sensor_vcs_velocity.lateral = 0.0F;
   detection_azimuth = M_PI; // 180 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should be negative (cos(pi) = -1)
    */
   DOUBLES_EQUAL(-25.0F, result, TOLERANCE);
}

/** \purpose
 * Test with -90 degree azimuth (left side)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Negative_90_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Lateral velocity, detection at -90 degrees
    */
   sensor_vcs_velocity.longitudinal = 0.0F;
   sensor_vcs_velocity.lateral = 8.0F;
   detection_azimuth = -M_PI / 2.0F; // -90 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should be negative (sin(-pi/2) = -1)
    */
   DOUBLES_EQUAL(-8.0F, result, TOLERANCE);
}

/** \purpose
 * Test with 30 degree azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_30_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Combined velocity, detection at 30 degrees
    */
   sensor_vcs_velocity.longitudinal = 20.0F;
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = M_PI / 6.0F; // 30 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result = 20*cos(30deg) + 5*sin(30deg) = 20*0.866 + 5*0.5 = 19.82
    */
   float32_t expected = 20.0F * std::cos(M_PI / 6.0F) + 5.0F * std::sin(M_PI / 6.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

/** \purpose
 * Test with 60 degree azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_60_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Combined velocity, detection at 60 degrees
    */
   sensor_vcs_velocity.longitudinal = 20.0F;
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = M_PI / 3.0F; // 60 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result = 20*cos(60deg) + 5*sin(60deg) = 20*0.5 + 5*0.866 = 14.33
    */
   float32_t expected = 20.0F * std::cos(M_PI / 3.0F) + 5.0F * std::sin(M_PI / 3.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

/** \purpose
 * Test with -45 degree azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Negative_45_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Combined velocity, detection at -45 degrees
    */
   sensor_vcs_velocity.longitudinal = 15.0F;
   sensor_vcs_velocity.lateral = 10.0F;
   detection_azimuth = -M_PI / 4.0F; // -45 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result = 15*cos(-45deg) + 10*sin(-45deg) = 15*0.707 - 10*0.707 = 3.54
    */
   float32_t expected = 15.0F * std::cos(-M_PI / 4.0F) + 10.0F * std::sin(-M_PI / 4.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

/** \purpose
 * Test with small azimuth angle
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Small_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Combined velocity, detection at small angle (5 degrees)
    */
   sensor_vcs_velocity.longitudinal = 30.0F;
   sensor_vcs_velocity.lateral = 2.0F;
   detection_azimuth = 0.0873F; // ~5 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be close to longitudinal velocity for small angles
    */
   float32_t expected = 30.0F * std::cos(0.0873F) + 2.0F * std::sin(0.0873F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
   CHECK(result > 29.0F && result < 31.0F);
}

// ============================================================================
// Negative Velocity Tests
// ============================================================================

/** \purpose
 * Test with negative longitudinal velocity (reverse)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Negative_Longitudinal)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle reversing at -5 m/s, detection ahead
    */
   sensor_vcs_velocity.longitudinal = -5.0F;
   sensor_vcs_velocity.lateral = 0.0F;
   detection_azimuth = 0.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should be negative
    */
   DOUBLES_EQUAL(-5.0F, result, TOLERANCE);
}

/** \purpose
 * Test with negative lateral velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Negative_Lateral)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle with negative lateral velocity, detection at 90 degrees
    */
   sensor_vcs_velocity.longitudinal = 0.0F;
   sensor_vcs_velocity.lateral = -6.0F;
   detection_azimuth = M_PI / 2.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should be negative
    */
   DOUBLES_EQUAL(-6.0F, result, TOLERANCE);
}

/** \purpose
 * Test with both negative velocities
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Both_Negative_Velocities)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle with both negative velocity components
    */
   sensor_vcs_velocity.longitudinal = -10.0F;
   sensor_vcs_velocity.lateral = -5.0F;
   detection_azimuth = M_PI / 4.0F; // 45 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result = -10*cos(45deg) + (-5)*sin(45deg) = -10*0.707 - 5*0.707 = -10.61
    */
   float32_t expected = -10.0F * std::cos(M_PI / 4.0F) + (-5.0F) * std::sin(M_PI / 4.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
   CHECK(result < 0.0F);
}

// ============================================================================
// Large Value Tests
// ============================================================================

/** \purpose
 * Test with high longitudinal velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_High_Longitudinal_Velocity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle at high speed (50 m/s = 180 km/h)
    */
   sensor_vcs_velocity.longitudinal = 50.0F;
   sensor_vcs_velocity.lateral = 0.0F;
   detection_azimuth = 0.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should equal high velocity
    */
   DOUBLES_EQUAL(50.0F, result, TOLERANCE);
}

/** \purpose
 * Test with high lateral velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_High_Lateral_Velocity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle with high lateral velocity (unusual but valid)
    */
   sensor_vcs_velocity.longitudinal = 0.0F;
   sensor_vcs_velocity.lateral = 20.0F;
   detection_azimuth = M_PI / 2.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Predicted range rate should equal lateral velocity
    */
   DOUBLES_EQUAL(20.0F, result, TOLERANCE);
}

/** \purpose
 * Test with maximum realistic velocities
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Maximum_Velocities)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle with maximum realistic velocities
    */
   sensor_vcs_velocity.longitudinal = 55.0F; // ~200 km/h
   sensor_vcs_velocity.lateral = 15.0F;
   detection_azimuth = M_PI / 6.0F; // 30 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be calculated correctly with large values
    */
   float32_t expected = 55.0F * std::cos(M_PI / 6.0F) + 15.0F * std::sin(M_PI / 6.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

// ============================================================================
// Edge Case Tests
// ============================================================================

/** \purpose
 * Test with very small velocities
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Very_Small_Velocities)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle with very small velocities
    */
   sensor_vcs_velocity.longitudinal = 0.01F;
   sensor_vcs_velocity.lateral = 0.005F;
   detection_azimuth = M_PI / 4.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be very small but calculated correctly
    */
   float32_t expected = 0.01F * std::cos(M_PI / 4.0F) + 0.005F * std::sin(M_PI / 4.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

/** \purpose
 * Test with NaN in longitudinal velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_NaN_Longitudinal)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN in longitudinal velocity
    */
   sensor_vcs_velocity.longitudinal = std::numeric_limits<float32_t>::quiet_NaN();
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = 0.5F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be NaN
    */
   CHECK(std::isnan(result));
}

/** \purpose
 * Test with NaN in lateral velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_NaN_Lateral)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN in lateral velocity
    */
   sensor_vcs_velocity.longitudinal = 10.0F;
   sensor_vcs_velocity.lateral = std::numeric_limits<float32_t>::quiet_NaN();
   detection_azimuth = 0.5F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be NaN
    */
   CHECK(std::isnan(result));
}

/** \purpose
 * Test with NaN in azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_NaN_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN in detection azimuth
    */
   sensor_vcs_velocity.longitudinal = 10.0F;
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be NaN
    */
   CHECK(std::isnan(result));
}

/** \purpose
 * Test with infinity in longitudinal velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Infinity_Longitudinal)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Infinity in longitudinal velocity
    */
   sensor_vcs_velocity.longitudinal = std::numeric_limits<float32_t>::infinity();
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = 0.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be infinity
    */
   CHECK(std::isinf(result));
   CHECK(result > 0.0F);
}

/** \purpose
 * Test with negative infinity in lateral velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Negative_Infinity_Lateral)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Negative infinity in lateral velocity
    */
   sensor_vcs_velocity.longitudinal = 10.0F;
   sensor_vcs_velocity.lateral = -std::numeric_limits<float32_t>::infinity();
   detection_azimuth = M_PI / 2.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be negative infinity
    */
   CHECK(std::isinf(result));
   CHECK(result < 0.0F);
}

// ============================================================================
// Symmetry and Mathematical Property Tests
// ============================================================================

/** \purpose
 * Test symmetry of positive and negative azimuth with lateral velocity
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Azimuth_Symmetry)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Pure lateral velocity with +-azimuth angles
    */
   sensor_vcs_velocity.longitudinal = 0.0F;
   sensor_vcs_velocity.lateral = 10.0F;

   float32_t positive_azimuth = 0.5F;
   float32_t negative_azimuth = -0.5F;

   /** \action
    * Calculate for both positive and negative azimuth
    */
   float32_t result_positive = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, positive_azimuth);
   float32_t result_negative = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, negative_azimuth);

   /** \result
    * Results should be opposite due to sin symmetry
    */
   DOUBLES_EQUAL(-result_positive, result_negative, TOLERANCE);
}

/** \purpose
 * Test with azimuth at 270 degrees (-90 degrees equivalent)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_270_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Lateral velocity, detection at 270 degrees (3pi/2)
    */
   sensor_vcs_velocity.longitudinal = 0.0F;
   sensor_vcs_velocity.lateral = 8.0F;
   detection_azimuth = 3.0F * M_PI / 2.0F; // 270 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be negative (sin(270deg) = -1)
    */
   DOUBLES_EQUAL(-8.0F, result, TOLERANCE);
}

/** \purpose
 * Test that perpendicular components don't interfere
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Perpendicular_Components)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Longitudinal velocity only, detection at 90 degrees (perpendicular)
    */
   sensor_vcs_velocity.longitudinal = 30.0F;
   sensor_vcs_velocity.lateral = 0.0F;
   detection_azimuth = M_PI / 2.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be near zero (cos(90deg) = 0)
    */
   DOUBLES_EQUAL(0.0F, result, TOLERANCE);
}

// ============================================================================
// Real-World Scenario Tests
// ============================================================================

/** \purpose
 * Test typical highway driving scenario
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Highway_Driving)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Highway speed (120 km/h = 33.3 m/s), slight lateral motion, detection ahead
    */
   sensor_vcs_velocity.longitudinal = 33.3F;
   sensor_vcs_velocity.lateral = 0.5F;
   detection_azimuth = 0.1F; // ~5.7 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be close to longitudinal velocity
    */
   float32_t expected = 33.3F * std::cos(0.1F) + 0.5F * std::sin(0.1F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
   CHECK(result > 32.0F && result < 34.0F);
}

/** \purpose
 * Test lane change scenario
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Lane_Change)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle changing lanes: forward motion with significant lateral velocity
    */
   sensor_vcs_velocity.longitudinal = 25.0F;
   sensor_vcs_velocity.lateral = 3.0F;
   detection_azimuth = M_PI / 3.0F; // 60 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Both velocity components should contribute
    */
   float32_t expected = 25.0F * std::cos(M_PI / 3.0F) + 3.0F * std::sin(M_PI / 3.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

/** \purpose
 * Test parking scenario with low speed
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Parking)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Low speed parking maneuver
    */
   sensor_vcs_velocity.longitudinal = 2.0F;
   sensor_vcs_velocity.lateral = 1.5F;
   detection_azimuth = M_PI / 4.0F; // 45 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Low speed result
    */
   float32_t expected = 2.0F * std::cos(M_PI / 4.0F) + 1.5F * std::sin(M_PI / 4.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

/** \purpose
 * Test emergency braking scenario
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Emergency_Braking)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Vehicle decelerating rapidly, detection ahead
    */
   sensor_vcs_velocity.longitudinal = 15.0F; // Reduced from higher speed
   sensor_vcs_velocity.lateral = 0.2F;
   detection_azimuth = 0.05F; // Nearly straight ahead

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be close to current longitudinal velocity
    */
   float32_t expected = 15.0F * std::cos(0.05F) + 0.2F * std::sin(0.05F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

// ============================================================================
// Additional Edge Case Tests (Priority 1 Enhancements)
// ============================================================================

/** \purpose
 * Test with combined NaN in both velocity components
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Combined_NaN_Values)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * NaN in both longitudinal and lateral velocities
    */
   sensor_vcs_velocity.longitudinal = std::numeric_limits<float32_t>::quiet_NaN();
   sensor_vcs_velocity.lateral = std::numeric_limits<float32_t>::quiet_NaN();
   detection_azimuth = 0.5F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be NaN (NaN propagates through any operation)
    */
   CHECK(std::isnan(result));
}

/** \purpose
 * Test with mixed infinity and NaN values
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Infinity_And_NaN)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Infinity in longitudinal, NaN in lateral velocity
    */
   sensor_vcs_velocity.longitudinal = std::numeric_limits<float32_t>::infinity();
   sensor_vcs_velocity.lateral = std::numeric_limits<float32_t>::quiet_NaN();
   detection_azimuth = M_PI / 4.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be NaN (NaN dominates in arithmetic operations)
    */
   CHECK(std::isnan(result));
}

/** \purpose
 * Test with zero longitudinal and non-zero lateral at 0deg azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Zero_Longitudinal_Lateral_At_Zero_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero longitudinal velocity, non-zero lateral, detection ahead
    */
   sensor_vcs_velocity.longitudinal = 0.0F;
   sensor_vcs_velocity.lateral = 10.0F;
   detection_azimuth = 0.0F;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be zero (lateral velocity doesn't contribute at 0deg azimuth, sin(0) = 0)
    */
   DOUBLES_EQUAL(0.0F, result, TOLERANCE); // Tight tolerance for exact calculation
}

/** \purpose
 * Test with non-zero longitudinal and zero lateral at 90deg azimuth
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Longitudinal_At_90_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Non-zero longitudinal, zero lateral, detection perpendicular
    */
   sensor_vcs_velocity.longitudinal = 25.0F;
   sensor_vcs_velocity.lateral = 0.0F;
   detection_azimuth = M_PI / 2.0F; // 90 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be near zero (longitudinal doesn't contribute at 90deg, cos(pi/2) = 0)
    */
   DOUBLES_EQUAL(0.0F, result, TOLERANCE); // Looser tolerance for floating point precision
}

/** \purpose
 * Test with large azimuth angle beyond pi (angle wrapping behavior)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Large_Azimuth_Beyond_Pi)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Normal velocities, azimuth > pi (tests angle handling)
    */
   sensor_vcs_velocity.longitudinal = 20.0F;
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = 5.0F; // > pi (~286 degrees)

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be calculated correctly (trigonometric functions handle any angle)
    */
   float32_t expected = 20.0F * std::cos(5.0F) + 5.0F * std::sin(5.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

/** \purpose
 * Test with large negative azimuth angle beyond -pi
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Large_Negative_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Normal velocities, azimuth < -pi
    */
   sensor_vcs_velocity.longitudinal = 20.0F;
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = -5.0F; // < -pi (~-286 degrees)

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be calculated correctly
    */
   float32_t expected = 20.0F * std::cos(-5.0F) + 5.0F * std::sin(-5.0F);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
}

/** \purpose
 * Test with azimuth at exactly pi boundary
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Exactly_Pi)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Combined velocities, azimuth exactly at pi
    */
   sensor_vcs_velocity.longitudinal = 20.0F;
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = M_PI;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result = -20 (cos(pi) = -1, sin(pi) = 0)
    */
   DOUBLES_EQUAL(-20.0F, result, TOLERANCE);
}

/** \purpose
 * Test with azimuth at exactly -pi boundary
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Exactly_Negative_Pi)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Combined velocities, azimuth exactly at -pi
    */
   sensor_vcs_velocity.longitudinal = 20.0F;
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = -M_PI;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result = -20 (cos(-pi) = -1, sin(-pi) = 0)
    */
   DOUBLES_EQUAL(-20.0F, result, TOLERANCE);
}

/** \purpose
 * Test with azimuth at 2pi (full rotation)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Two_Pi)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Combined velocities, azimuth at 2pi (360 degrees, equivalent to 0)
    */
   sensor_vcs_velocity.longitudinal = 25.0F;
   sensor_vcs_velocity.lateral = 3.0F;
   detection_azimuth = 2.0F * M_PI;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be close to longitudinal velocity (cos(2pi) = 1, sin(2pi) = 0)
    */
   float32_t expected = 25.0F * std::cos(2.0F * M_PI) + 3.0F * std::sin(2.0F * M_PI);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
   DOUBLES_EQUAL(25.0F, result, TOLERANCE); // Should be close to 25
}

/** \purpose
 * Test precision with result that should be exactly zero
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Exact_Zero_Result)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Equal magnitude opposite contribution velocities
    */
   sensor_vcs_velocity.longitudinal = 10.0F;
   sensor_vcs_velocity.lateral = -10.0F;
   detection_azimuth = M_PI / 4.0F; // 45 degrees where cos = sin

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should be near zero (10*cos(45deg) + (-10)*sin(45deg) = 10*0.707 - 10*0.707 = 0)
    */
   DOUBLES_EQUAL(0.0F, result, TOLERANCE);
}

/** \purpose
 * Test with very large azimuth (stress test)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Very_Large_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Normal velocities, very large azimuth (10pi)
    */
   sensor_vcs_velocity.longitudinal = 20.0F;
   sensor_vcs_velocity.lateral = 5.0F;
   detection_azimuth = 10.0F * M_PI;

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should still be calculated (trigonometric functions periodic)
    */
   float32_t expected = 20.0F * std::cos(10.0F * M_PI) + 5.0F * std::sin(10.0F * M_PI);
   DOUBLES_EQUAL(expected, result, TOLERANCE);
   // cos(10pi) = 1, sin(10pi) = 0, so result = 20
   DOUBLES_EQUAL(20.0F, result, TOLERANCE);
}

/** \purpose
 * Test cross-validation with alternative calculation
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Cross_Validation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Typical driving scenario with known geometry
    */
   sensor_vcs_velocity.longitudinal = 30.0F;
   sensor_vcs_velocity.lateral = 2.0F;
   detection_azimuth = M_PI / 6.0F; // 30 degrees

   /** \action
    * Calculate using function and validate with manual calculation
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   // Alternative calculation using explicit formula breakdown
   float32_t cos_term = sensor_vcs_velocity.longitudinal * std::cos(detection_azimuth);
   float32_t sin_term = sensor_vcs_velocity.lateral * std::sin(detection_azimuth);
   float32_t manual_result = cos_term + sin_term;

   /** \result
    * Both calculation methods should produce identical results
    */
   DOUBLES_EQUAL(manual_result, result, TOLERANCE); // Very tight tolerance
   DOUBLES_EQUAL(result, cos_term + sin_term, TOLERANCE);
}

/** \purpose
 * Test sensor velocity with positive infinity in both components
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Sensor_Velocities_Infinity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Infinity in both velocity components
    */
   sensor_vcs_velocity.longitudinal = std::numeric_limits<float32_t>::infinity();
   sensor_vcs_velocity.lateral = std::numeric_limits<float32_t>::infinity();
   detection_azimuth = M_PI / 4.0F; // 45 degrees

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should handle infinities withouth crashing.
    */
   CHECK(std::isinf(result));
   CHECK(result > 0.0F);
}

/** \purpose
 * Test sensor velocitywith opposite infinities (indeterminate form)
 * \req
 * CPR-7942_Derived
 */
TEST(test_RSPP_Calculate_Compensated_RRate_Calculate_Ego_Motion_RRate,
     Calculate_Ego_Motion_RRate_TC_Sensor_Velocity_Opposite_Infinities)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Positive infinity in longitudinal, negative infinity in lateral at angle where both contribute equally
    */
   sensor_vcs_velocity.longitudinal = std::numeric_limits<float32_t>::infinity();
   sensor_vcs_velocity.lateral = -std::numeric_limits<float32_t>::infinity();
   detection_azimuth = M_PI / 4.0F; // 45 degrees, cos = sin

   /** \action
    * Calculate predicted range rate
    */
   float32_t result = RSPP_Calculate_Ego_Motion_Range_Rate(sensor_vcs_velocity, detection_azimuth);

   /** \result
    * Result should handle infinities withouth crashing.
    */
   CHECK(std::isnan(result) || std::isinf(result));
}

/** @}*/
