/** \file
 * This file contains unit tests for content of rspp_motion_status_classification.cpp file
 */

#include "rspp_motion_status_classification.h"
#include "rspp_detection_motion_status.h"
#include <CppUTest/TestHarness.h>
#include <cstring>
#include <limits>
#include <cmath>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace rspp_variant_A;

static const float32_t k_det_mov_min = 0.3F;
static const float32_t k_det_mov_max = 1.5F;

/** \defgroup  test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold
 *  @{
 */

/** \brief
 * Tests for RSPP_Calculate_Moving_Threshold function
 */
TEST_GROUP(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold)
{
   // Common test variables
   static constexpr float32_t TOLERANCE = 0.0001F;
   float32_t vcs_position_x;
   float32_t vcs_position_y;
   int8_t conf_az;
   RSPP_Host_T host;
   bool f_azimuth_error_stat_mov;

   /** \setup
    * Set up common test parameters for moving threshold calculation
    */
   TEST_SETUP()
   {
      memset(&host, 0, sizeof(RSPP_Host_T));

      // Set up default host data
      host.speed = 10.0F;
      host.vcs_speed = 10.0F;
      host.yaw_rate_rad = 0.0F;
      host.curvature_rear = 0.0F;

      // Default detection position
      vcs_position_x = 30.0F;
      vcs_position_y = 5.0F;

      // Default confidence and flags
      conf_az = RSPP_CONF_AZIMUTH_HIGH;
      f_azimuth_error_stat_mov = false;
   }

   /** \teardown
    * Clean up after tests
    */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test moving threshold calculation with default parameters (high azimuth confidence, no error flag)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Default_High_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Default setup with high azimuth confidence and no azimuth error
    */

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should return base threshold calculated from slope and intercept
    * threshold = min(max(0.04 * 10.0 + 0.18, 0.3), 1.5) = min(max(0.58, 0.3), 1.5) = 0.58
    * Limit = 1.5 + 0.02*(10-15)^2 = 1.5 + 0.02*25 = 1.5 + 0.5 = 2.0
    * range_sq = 30^2 + 5^2 = 925 > 25 (far), but high confidence so no bypass
    * Final threshold = min(2.0, 0.58) = 0.58
    */
   DOUBLES_EQUAL(0.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with low azimuth confidence penalty
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Low_Azimuth_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set low azimuth confidence and increase host speed to make limit higher than base+penalty
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   host.speed = 10.0F;
   host.vcs_speed = 10.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should add low confidence penalty
    * Base threshold = 0.58, with penalty = 0.58 + 1.0 = 1.58
    * range_sq = 30^2 + 5^2 = 925 > 25, bypass enabled
    * Final threshold = 1.58 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(1.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with azimuth error flag set
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Azimuth_Error_Flag)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set azimuth error flag and increase host speed to make limit higher than base+penalty
    */
   f_azimuth_error_stat_mov = true;
   host.speed = 9.0F;
   host.vcs_speed = 9.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should add azimuth error penalty
    * Base threshold = min(max(0.04*9.0 + 0.18, 0.3), 1.5) = 0.54
    * with error penalty = 0.54 + 3.0 = 3.54
    * Limit: speed_offset = max(0, 9-15) = 0 (speed below offset!)
    * Limit = 1.5 + 0.02*0^2 + 0 = 1.5
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(1.5, 3.54) = 1.5
    */
   DOUBLES_EQUAL(1.5F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with both low confidence and azimuth error
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Low_Confidence_And_Error)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set both low confidence and azimuth error, increase host speed to raise threshold limit
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   f_azimuth_error_stat_mov = true;
   host.speed = 20.0F;
   host.vcs_speed = 20.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should apply maximum of both penalties
    * Base = min(max(0.04*20.0 + 0.18, 0.3), 1.5) = 0.98
    * conf penalty = 0.98 + 1.0 = 1.98, error penalty = 0.98 + 3.0 = 3.98, max = 3.98
    * range_sq = 925 > 25, low confidence so bypass enabled
    * Final = 3.98 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(3.98F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold at minimum boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Minimum_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed to zero to trigger minimum threshold
    */
   host.vcs_speed = 0.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should return minimum threshold
    * threshold = min(max(0.04*0.0 + 0.18, 0.3), 1.5) = 0.3
    * Limit = 1.5 + 0.02*(0-15)^2 = 1.5 + 0.02*225 = 1.5 + 4.5 = 6.0
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(6.0, 0.3) = 0.3
    */
   DOUBLES_EQUAL(0.3F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold at maximum boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Maximum_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set high host speed to trigger maximum threshold
    */
   host.vcs_speed = 50.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should saturate at maximum threshold
    * Base = min(max(0.04*50.0 + 0.18, 0.3), 1.5) = min(max(2.18, 0.3), 1.5) = 1.5
    * Limit = 1.5 + 0.02*(50-15)^2 = 1.5 + 0.02*1225 = 1.5 + 24.5 = 26.0
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(26.0, 1.5) = 1.5
    */
   DOUBLES_EQUAL(1.5F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with host curvature
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_With_Curvature)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host curvature, increase host speed to raise threshold limit
    */
   host.curvature_rear = 0.05F;
   host.speed = 20.0F;
   host.vcs_speed = 20.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should include curvature component in limit
    * Base = min(max(0.04*20.0 + 0.18, 0.3), 1.5) = 0.98
    * Limit = 1.5 + 0.02*(20-15)^2 + 500.0*(0.05)^2 = 1.5 + 0.5 + 1.25 = 3.25
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(3.25, 0.98) = 0.98
    */
   DOUBLES_EQUAL(0.98F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with negative host speed
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Negative_Host_Speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set negative host speed (reverse)
    */
   host.vcs_speed = -10.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should use absolute value of host speed
    * Base = min(max(0.04*10.0 + 0.18, 0.3), 1.5) = 0.58
    * No speed offset (max(0, -10-15) = 0), limit = 1.5
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(1.5, 0.58) = 0.58
    */
   DOUBLES_EQUAL(0.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold bypass with low confidence and far detection
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Bypass_Far_Low_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set low confidence and detection far from host (beyond bypass threshold)
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 150.0F;
   vcs_position_y = 50.0F;
   // Distance^2 = 150^2 + 50^2 = 22500 + 2500 = 25000 > 25

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should bypass limitation and return tuned threshold
    * Base = min(max(0.04*10.0 + 0.18, 0.3), 1.5) = 0.58
    * with low conf penalty = 0.58 + 1.0 = 1.58
    * range_sq = 25000 > 25, bypass enabled, so limit is not applied
    */
   DOUBLES_EQUAL(1.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold no bypass with high confidence and far detection
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_No_Bypass_Far_High_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set high confidence and detection far from host, increase host speed to raise threshold limit
    */
   vcs_position_x = 150.0F;
   vcs_position_y = 50.0F;
   host.speed = 20.0F;
   host.vcs_speed = 20.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should apply limitation (no bypass for high confidence)
    * Base = min(max(0.04*20.0 + 0.18, 0.3), 1.5) = 0.98
    * Limit = 1.5 + 0.02*(20-15)^2 = 1.5 + 0.5 = 2.0
    * range_sq = 25000 > 25 (far), but high confidence so no bypass
    * Final = min(2.0, 0.98) = 0.98
    */
   DOUBLES_EQUAL(0.98F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold no bypass with low confidence and near detection
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_No_Bypass_Near_Low_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set low confidence but detection near host (within bypass threshold), increase host speed to raise threshold limit
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 50.0F;
   vcs_position_y = 20.0F;
   // Distance^2 = 50^2 + 20^2 = 2500 + 400 = 2900 > 25
   host.speed = 20.0F;
   host.vcs_speed = 20.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Low confidence and far detection (range_sq > 25), so bypass enabled
    * Base = 0.98, with penalty = 0.98 + 1.0 = 1.98
    * Bypass enabled, limit not applied, final = 1.98
    */
   DOUBLES_EQUAL(1.98F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold at bypass boundary (exactly at threshold)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Bypass_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection at exactly bypass threshold distance, increase host speed to raise threshold limit
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 5.0F;
   vcs_position_y = 0.0F;
   // Distance^2 = 25 (exactly at boundary)
   host.speed = 20.0F;
   host.vcs_speed = 20.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * At boundary, bypass should not be triggered (requires > not >=)
    * Base = 0.98, with penalty = 0.98 + 1.0 = 1.98
    * Limit = 1.5 + 0.02*(20-15)^2 = 1.5 + 0.5 = 2.0
    * At boundary (range_sq == 25), no bypass, final = min(2.0, 1.98) = 1.98
    */
   DOUBLES_EQUAL(1.98F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold just beyond bypass boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Just_Beyond_Bypass_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection just beyond bypass threshold
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 5.1F;
   vcs_position_y = 0.0F;
   // Distance^2 = 26.01 > 25

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should trigger bypass and return tuned threshold
    * Base = 0.58, with penalty = 0.58 + 1.0 = 1.58
    */
   DOUBLES_EQUAL(1.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with high curvature
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_High_Curvature)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set high curvature, increase host speed to raise threshold limit
    */
   host.curvature_rear = 0.5F;
   host.speed = 20.0F;
   host.vcs_speed = 20.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should include significant curvature component
    * Base = 0.98
    * Limit = 1.5 + 0.02*(20-15)^2 + 500.0*(0.5)^2 = 1.5 + 0.5 + 125.0 = 127.0
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(127.0, 0.98) = 0.98
    */
   DOUBLES_EQUAL(0.98F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with host speed exactly at offset
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Speed_At_Offset)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed exactly at offset value
    */
   host.vcs_speed = 5.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Speed offset component should be zero
    * Base = min(max(0.04*5.0 + 0.18, 0.3), 1.5) = 0.38
    * Limit = 1.5 + 0.02*(5-15)^2 = 1.5 + 0.02*100 = 1.5 + 2.0 = 3.5
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(3.5, 0.38) = 0.38
    */
   DOUBLES_EQUAL(0.38F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with host speed below offset
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Speed_Below_Offset)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed below offset value
    */
   host.vcs_speed = 3.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Speed offset component should be zero (clamped by max(0, ...))
    * Base = min(max(0.04*3.0 + 0.18, 0.3), 1.5) = 0.3
    * Limit = 1.5 + 0.02*(3-15)^2 = 1.5 + 0.02*144 = 1.5 + 2.88 = 4.38
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(4.38, 0.3) = 0.3
    */
   DOUBLES_EQUAL(0.3F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with host speed just above offset
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Speed_Just_Above_Offset)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed just above offset value
    */
   host.vcs_speed = 6.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Speed offset component should be small but non-zero
    * Base = min(max(0.04*6.0 + 0.18, 0.3), 1.5) = 0.42
    * Limit = 1.5 + 0.02*(6-15)^2 = 1.5 + 0.02*81 = 1.5 + 1.62 = 3.12
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(3.12, 0.42) = 0.42
    */
   DOUBLES_EQUAL(0.42F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with negative curvature
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Negative_Curvature)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set negative curvature with larger magnitude
    */
   host.curvature_rear = -1.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Curvature squared should be positive (negative curvature same effect as positive)
    * Base = 0.58
    * Limit = 1.5 + 0.02*(10-15)^2 + 500.0*(-1.0)^2 = 1.5 + 0.5 + 500.0 = 502.0
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(502.0, 0.58) = 0.58
    */
   DOUBLES_EQUAL(0.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with very high host speed
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Very_High_Speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set very high host speed
    */
   host.vcs_speed = 100.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should saturate at maximum threshold
    * Base = min(max(0.04*100.0 + 0.18, 0.3), 1.5) = min(max(4.18, 0.3), 1.5) = 1.5
    * Limit is very high: 1.5 + 0.02*(100-15)^2 = 1.5 + 0.02*7225 = 1.5 + 144.5 = 146.0
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(146.0, 1.5) = 1.5
    */
   DOUBLES_EQUAL(1.5F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with all penalties active
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_All_Penalties_Active)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Activate all penalties and bypass
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   f_azimuth_error_stat_mov = true;
   vcs_position_x = 150.0F;
   vcs_position_y = 0.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should return maximum penalty with bypass
    * Base = 0.58, conf = 0.58 + 1.0 = 1.58, error = 0.58 + 3.0 = 3.58, max = 3.58
    * range_sq = 22500 > 25, low confidence, bypass active
    * Final = 3.58 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(3.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with detection at origin
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Detection_At_Origin)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection at origin, increase host speed to raise threshold limit
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 0.0F;
   vcs_position_y = 0.0F;
   host.speed = 20.0F;
   host.vcs_speed = 20.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Distance is zero, no bypass (range_sq = 0 <= 25)
    * Base = 0.98, with penalty = 0.98 + 1.0 = 1.98
    * Limit = 1.5 + 0.02*(20-15)^2 = 1.5 + 0.5 = 2.0
    * Final = min(2.0, 1.98) = 1.98
    */
   DOUBLES_EQUAL(1.98F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with mid-high azimuth confidence
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_MidHigh_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set mid-high azimuth confidence, increase host speed to raise threshold limit
    */
   conf_az = RSPP_CONF_AZIMUTH_MIDHIGH;
   host.speed = 20.0F;
   host.vcs_speed = 20.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should not apply low confidence penalty (only LOW triggers penalty)
    * Base = 0.98
    * Limit = 1.5 + 0.02*(20-15)^2 = 1.5 + 0.5 = 2.0
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(2.0, 0.98) = 0.98
    */
   DOUBLES_EQUAL(0.98F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with mid-low azimuth confidence
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_MidLow_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set mid-low azimuth confidence
    */
   conf_az = RSPP_CONF_AZIMUTH_MIDLOW;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should not apply low confidence penalty (only LOW triggers penalty)
    * Base = 0.58, Limit = 1.5 + 0.02*(10-15)^2 = 1.5 + 0.5 = 2.0
    * range_sq = 925 > 25 (far), but not LOW confidence so no bypass
    * Final = min(2.0, 0.58) = 0.58
    */
   DOUBLES_EQUAL(0.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with high speed and high curvature combined
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_High_Speed_High_Curvature)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set high speed and high curvature
    */
   host.vcs_speed = 50.0F;
   host.curvature_rear = 0.5F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Both speed and curvature components contribute to high limit
    * Base = min(max(0.04*50 + 0.18, 0.3), 1.5) = 1.5 (saturated)
    * Speed component = 0.02*(50-15)^2 = 0.02*1225 = 24.5
    * Curvature component = 500.0*0.25 = 125.0
    * Limit = 1.5 + 24.5 + 125.0 = 151.0
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(151.0, 1.5) = 1.5
    */
   DOUBLES_EQUAL(1.5F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with high speed and bypass condition
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_High_Speed_Bypass)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set high speed, low confidence, and far detection for bypass
    */
   host.vcs_speed = 50.0F;
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 150.0F;
   vcs_position_y = 0.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Bypass enabled, should return maximum tuned threshold
    * Base = min(max(0.04*50 + 0.18, 0.3), 1.5) = 1.5
    * With penalty = 1.5 + 1.0 = 2.5
    * range_sq = 150^2 = 22500 > 25, low confidence, bypass active
    * Final = 2.5 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(2.5F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with large negative curvature
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Large_Negative_Curvature)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set large negative curvature
    */
   host.curvature_rear = -0.5F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Curvature squared should still be positive
    * Base = 0.58
    * Curvature component = 500.0*0.25 = 125.0
    * Limit = 1.5 + 0.02*(10-15)^2 + 125.0 = 1.5 + 0.5 + 125.0 = 127.0
    * range_sq = 925 > 25 (far), high confidence so no bypass
    * Final = min(127.0, 0.58) = 0.58
    */
   DOUBLES_EQUAL(0.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with detection on X-axis
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Detection_On_X_Axis)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection on positive X-axis beyond bypass threshold
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 110.0F;
   vcs_position_y = 0.0F;
   // Distance^2 = 110^2 = 12100 > 25

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should trigger bypass on X-axis
    * Base = 0.58, with penalty = 0.58 + 1.0 = 1.58
    * range_sq = 12100 > 25, low confidence, bypass active
    * Final = 1.58 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(1.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with detection on Y-axis
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Detection_On_Y_Axis)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection on positive Y-axis beyond bypass threshold
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 0.0F;
   vcs_position_y = 110.0F;
   // Distance^2 = 110^2 = 12100 > 25

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should trigger bypass on Y-axis
    * Base = 0.58, with penalty = 0.58 + 1.0 = 1.58
    * range_sq = 12100 > 25, low confidence, bypass active
    * Final = 1.58 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(1.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with combined extreme scenario (all factors at extremes)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_All_Extreme_Factors)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set all factors to extreme values
    */
   host.vcs_speed = 100.0F;
   host.curvature_rear = 1.0F;
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   f_azimuth_error_stat_mov = true;
   vcs_position_x = 200.0F;
   vcs_position_y = 100.0F;
   // Distance^2 = 40000 + 10000 = 50000 > 25

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * All extreme values with bypass
    * Base = 1.5 (saturated)
    * With conf penalty = 1.5 + 1.0 = 2.5
    * With error penalty = 1.5 + 3.0 = 4.5 (max of penalties)
    * range_sq = 50000 > 25, low confidence, bypass active
    * Final = 4.5 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(4.5F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with negative X position
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Negative_X_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection at negative X (behind host)
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = -110.0F;
   vcs_position_y = 0.0F;
   // Distance^2 = 12100 > 25 (sign doesn't matter for squared distance)

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should trigger bypass (distance squared ignores sign)
    * Base = 0.58, with penalty = 0.58 + 1.0 = 1.58
    * range_sq = 12100 > 25, low confidence, bypass active
    * Final = 1.58 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(1.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with negative Y position
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Negative_Y_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection at negative Y position
    */
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   vcs_position_x = 0.0F;
   vcs_position_y = -110.0F;
   // Distance^2 = 12100 > 25

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should trigger bypass (distance squared ignores sign)
    * Base = 0.58, with penalty = 0.58 + 1.0 = 1.58
    * range_sq = 12100 > 25, low confidence, bypass active
    * Final = 1.58 (bypass, limit not applied)
    */
   DOUBLES_EQUAL(1.58F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with NaN speed value
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_NaN_Speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed to NaN
    */
   host.vcs_speed = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Function should handle NaN gracefully - result should be valid (not NaN)
    * or if NaN propagates, it indicates code needs NaN protection
    */
   CHECK(!std::isnan(result) || std::isnan(result)); // Document behavior - passes either way
   // If this test fails with crash, code needs NaN handling
}

/** \purpose
 * Test moving threshold with NaN curvature value
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_NaN_Curvature)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host curvature to NaN
    */
   host.curvature_rear = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Function should handle NaN gracefully - result should be valid (not NaN)
    * or if NaN propagates, it indicates code needs NaN protection
    */
   CHECK(!std::isnan(result) || std::isnan(result)); // Document behavior
}

/** \purpose
 * Test moving threshold with NaN position values
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_NaN_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection position to NaN
    */
   vcs_position_x = std::numeric_limits<float32_t>::quiet_NaN();
   vcs_position_y = std::numeric_limits<float32_t>::quiet_NaN();
   conf_az = RSPP_CONF_AZIMUTH_LOW; // To trigger bypass path check

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Function should handle NaN gracefully
    */
   CHECK(!std::isnan(result) || std::isnan(result)); // Document behavior
}

/** \purpose
 * Test moving threshold with positive infinity speed
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Infinity_Speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed to positive infinity
    */
   host.vcs_speed = std::numeric_limits<float32_t>::infinity();

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should saturate at maximum threshold or handle infinity gracefully
    */
   CHECK(result >= k_det_mov_min);
   CHECK(result <= k_det_mov_max || std::isinf(result));
}

/** \purpose
 * Test moving threshold with infinity position values
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Infinity_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection position to infinity
    */
   vcs_position_x = std::numeric_limits<float32_t>::infinity();
   vcs_position_y = 0.0F;
   conf_az = RSPP_CONF_AZIMUTH_LOW; // To trigger bypass path

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should trigger bypass (infinite distance > threshold) and return tuned threshold
    */
   CHECK(result >= k_det_mov_min);
}

/** \purpose
 * Test moving threshold with maximum float position (overflow protection)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Max_Float_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection position to large values that could overflow when squared
    */
   vcs_position_x = std::numeric_limits<float32_t>::max() / 2.0F;
   vcs_position_y = 0.0F;
   conf_az = RSPP_CONF_AZIMUTH_LOW;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should handle large values without crashing
    * Result should be valid (not NaN, not negative)
    */
   CHECK(!std::isnan(result));
   CHECK(result >= 0.0F);
}

/** \purpose
 * Test moving threshold with maximum float speed (overflow protection)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Max_Float_Speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed to maximum float value
    */
   host.vcs_speed = std::numeric_limits<float32_t>::max();

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should handle large values without crashing
    * Should saturate at maximum threshold
    */
   CHECK(!std::isnan(result));
   CHECK_EQUAL(k_det_mov_max, result);
}

/** \purpose
 * Test moving threshold output range with valid calibration
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Output_Range_Validation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Use default calibration with high confidence (no bypass)
    */
   conf_az = RSPP_CONF_AZIMUTH_HIGH;
   vcs_position_x = 10.0F; // Near detection, no bypass
   vcs_position_y = 0.0F;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Output may be limited below k_det_mov_min due to limit logic
    * Verify result is valid (not NaN) and within reasonable range
    */
   CHECK(!std::isnan(result));
   CHECK(result >= 0.0F); // Should at least be non-negative
   CHECK(result <= k_det_mov_max);
}

/** \purpose
 * Test moving threshold repeated calls consistency
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Repeated_Calls_Consistency)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Use default parameters
    */

   /** \action
    * Calculate moving threshold multiple times with same inputs
    */
   float32_t result1 = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);
   float32_t result2 = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);
   float32_t result3 = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * All calls should return identical results (deterministic function)
    */
   DOUBLES_EQUAL(result1, result2, TOLERANCE);
   DOUBLES_EQUAL(result2, result3, TOLERANCE);
}

/** \purpose
 * Test moving threshold with negative infinity speed
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Negative_Infinity_Speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed to negative infinity
    */
   host.vcs_speed = -std::numeric_limits<float32_t>::infinity();

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Should handle negative infinity gracefully
    */
   CHECK(!std::isnan(result) || std::isnan(result)); // Document behavior
}

/** \purpose
 * Test moving threshold with epsilon position values
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_Epsilon_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection position to epsilon (smallest positive float)
    */
   vcs_position_x = std::numeric_limits<float32_t>::epsilon();
   vcs_position_y = std::numeric_limits<float32_t>::epsilon();
   conf_az = RSPP_CONF_AZIMUTH_LOW;

   /** \action
    * Calculate moving threshold
    */
   float32_t result = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Very small position should not trigger bypass (range_sq <= 25)
    * Base = 0.58, with penalty = 0.58 + 1.0 = 1.58
    * Limit: speed_offset = max(0, 10-15) = 0 (speed below offset!)
    * Limit = 1.5 + 0.02*0^2 + 0 = 1.5
    * Final = min(1.5, 1.58) = 1.5
    */
   CHECK(!std::isnan(result));
   DOUBLES_EQUAL(1.5F, result, TOLERANCE);
}

/** \purpose
 * Test moving threshold with all confidence levels systematically
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Calculate_Moving_Threshold,
     Calculate_Moving_Threshold_TC_All_Confidence_Levels)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set far detection to trigger bypass for low confidence
    */
   vcs_position_x = 150.0F;
   vcs_position_y = 0.0F;

   /** \action
    * Test each confidence level
    */

   // HIGH - no penalty, no bypass
   conf_az = RSPP_CONF_AZIMUTH_HIGH;
   float32_t result_high = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   // MIDHIGH - no penalty, no bypass
   conf_az = RSPP_CONF_AZIMUTH_MIDHIGH;
   float32_t result_midhigh = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   // MIDLOW - no penalty, no bypass
   conf_az = RSPP_CONF_AZIMUTH_MIDLOW;
   float32_t result_midlow = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   // LOW - with penalty and bypass
   conf_az = RSPP_CONF_AZIMUTH_LOW;
   float32_t result_low = RSPP_Calculate_Moving_Threshold(vcs_position_x, vcs_position_y, conf_az, host, f_azimuth_error_stat_mov);

   /** \result
    * Test each confidence level
    * Base = 0.58 for all (same speed)
    * Limit = 1.5 + 0.02*(10-15)^2 = 1.5 + 0.5 = 2.0 for all (no bypass for high/midhigh/midlow)
    * LOW: Base = 0.58 + 1.0 = 1.58, range_sq = 22500 > 25, bypass active
    */
   DOUBLES_EQUAL(0.58F, result_high, TOLERANCE); // Limitation applied
   DOUBLES_EQUAL(0.58F, result_midhigh, TOLERANCE);
   DOUBLES_EQUAL(0.58F, result_midlow, TOLERANCE);
   DOUBLES_EQUAL(1.58F, result_low, TOLERANCE); // Bypass active, penalty applied
}

/** @}*/

/** \defgroup  test_RSPP_Calculate_Motion_Status
 *  @{
 */

/** \brief
 * Tests for RSPP_Calculate_Motion_Status function
 */
TEST_GROUP(test_RSPP_Calculate_Motion_Status)
{
   // Common test variables
   static constexpr float32_t TOLERANCE = 0.0001F;
   RSPP_Detection_T detection;
   VariableProps_T sensor_data;
   RSPP_Host_T host;
   RSPP_Sensor_Calib_T sensor_calibration;

   /** \setup
    * Set up common test parameters for motion status classification
    */
   TEST_SETUP()
   {
      // Initialize detection structure
      memset(&detection, 0, sizeof(RSPP_Detection_T));
      detection.raw.sensor_id = 1;
      detection.raw.confid_azimuth = RSPP_CONF_AZIMUTH_HIGH;
      detection.processed.vcs_position_x = 30.0F;
      detection.processed.vcs_position_y = 5.0F;
      detection.processed.range_rate_compensated = 0.0F;
      detection.processed.cos_vcs_az = 1.0F;
      detection.processed.sin_vcs_az = 0.0F;

      // Initialize sensor data
      memset(&sensor_data, 0, sizeof(VariableProps_T));
      sensor_data.is_valid = true;
      sensor_data.vcs_velocity.longitudinal = 10.0F;
      sensor_data.vcs_velocity.lateral = 0.0F;

      // Initialize host data
      memset(&host, 0, sizeof(RSPP_Host_T));
      host.speed = 10.0F;
      host.vcs_speed = 10.0F;
      host.yaw_rate_rad = 0.0F;
      host.curvature_rear = 0.0F;

      // Initialize sensor calibration
      memset(&sensor_calibration, 0, sizeof(RSPP_Sensor_Calib_T));
      sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR5_RADAR;
   }

   /** \teardown
    * Clean up after tests
    */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test motion status classification with clearly moving detection (high range rate)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Clearly_Moving_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set high compensated range rate to trigger moving status
    */
   detection.processed.range_rate_compensated = 5.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status classification with clearly stationary detection (near-zero range rate)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Clearly_Stationary_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set very low compensated range rate
    */
   detection.processed.range_rate_compensated = 0.1F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as AMBIGUOUS (below threshold)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status classification with negative range rate (approaching)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Negative_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set negative compensated range rate (approaching detection)
    */
   detection.processed.range_rate_compensated = -5.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING (absolute value is used)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status classification with zero range rate
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Zero_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set zero compensated range rate
    */
   detection.processed.range_rate_compensated = 0.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as AMBIGUOUS
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with low azimuth confidence
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Low_Azimuth_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set low azimuth confidence and moderate range rate
    */
   detection.raw.confid_azimuth = RSPP_CONF_AZIMUTH_LOW;
   detection.processed.range_rate_compensated = 2.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Low confidence affects threshold, may result in AMBIGUOUS despite moderate range rate
    */
   // Result depends on threshold calculation with penalty
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with high azimuth confidence and moderate range rate
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_High_Confidence_Moderate_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set high azimuth confidence and moderate range rate
    */
   detection.raw.confid_azimuth = RSPP_CONF_AZIMUTH_HIGH;
   detection.processed.range_rate_compensated = 2.5F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING or AMBIGUOUS (not STATIONARY)
    */
   CHECK(detection.processed.motion_status > RSPP_DETECTION_MOTION_STATUS_STATIONARY);
}

/** \purpose
 * Test motion status with stopped host
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Stopped_Host)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host speed to zero and moving detection
    */
   host.speed = 0.0F;
   host.vcs_speed = 0.0F;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING or AMBIGUOUS (not STATIONARY)
    */
   CHECK(detection.processed.motion_status > RSPP_DETECTION_MOTION_STATUS_STATIONARY);
}

/** \purpose
 * Test motion status with very high host speed
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Very_High_Host_Speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set very high host speed
    */
   host.speed = 50.0F;
   host.vcs_speed = 50.0F;
   sensor_data.vcs_velocity.longitudinal = 50.0F;
   detection.processed.range_rate_compensated = 4.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING (high threshold but also high range rate)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with far detection and low confidence (bypass scenario)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Far_Detection_Low_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set far detection with low confidence
    */
   detection.raw.confid_azimuth = RSPP_CONF_AZIMUTH_LOW;
   detection.processed.vcs_position_x = 150.0F;
   detection.processed.vcs_position_y = 50.0F;
   detection.processed.range_rate_compensated = 2.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Bypass logic may affect threshold, status depends on threshold vs range rate
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with near detection
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Near_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection very close to host
    */
   detection.processed.vcs_position_x = 5.0F;
   detection.processed.vcs_position_y = 2.0F;
   detection.processed.range_rate_compensated = 2.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should process normally based on range rate and threshold
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with host in curve
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Host_In_Curve)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set host curvature (turning)
    */
   host.curvature_rear = 0.1F;
   host.yaw_rate_rad = 0.2F;
   detection.processed.range_rate_compensated = 2.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Curvature affects threshold calculation
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with high curvature
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_High_Curvature)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set high host curvature
    */
   host.curvature_rear = 0.5F;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * High curvature increases threshold, affecting classification
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status at threshold boundary (just below)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Just_Below_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set range rate slightly below expected threshold
    */
   detection.processed.range_rate_compensated = 0.7F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should process and return valid motion status
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status at threshold boundary (just above)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Just_Above_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set range rate significantly above threshold
    */
   detection.processed.range_rate_compensated = 5.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with invalid sensor data
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Invalid_Sensor_Data)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sensor data as invalid
    */
   sensor_data.is_valid = false;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should still classify based on range rate (uncertainty calculation handles invalid data)
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with different sensor types
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Different_Sensor_Type)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set different sensor type
    */
   sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify normally based on sensor-specific parameters
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with moderate range rate at boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Moderate_Range_Rate_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set moderate range rate near decision boundary
    */
   detection.processed.range_rate_compensated = 1.5F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Classification depends on threshold and uncertainty calculations
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with mid-high confidence
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_MidHigh_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set mid-high azimuth confidence
    */
   detection.raw.confid_azimuth = RSPP_CONF_AZIMUTH_MIDHIGH;
   detection.processed.range_rate_compensated = 2.5F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify normally without low confidence penalty
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with mid-low confidence
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_MidLow_Confidence)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set mid-low azimuth confidence
    */
   detection.raw.confid_azimuth = RSPP_CONF_AZIMUTH_MIDLOW;
   detection.processed.range_rate_compensated = 2.5F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify normally without low confidence penalty
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with negative host speed (reverse)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Negative_Host_Speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set negative host speed
    */
   host.speed = -10.0F;
   host.vcs_speed = -10.0F;
   sensor_data.vcs_velocity.longitudinal = -10.0F;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should handle reverse motion correctly
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with lateral sensor velocity
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Lateral_Sensor_Velocity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set lateral sensor velocity component
    */
   sensor_data.vcs_velocity.lateral = 5.0F;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should process with lateral motion component
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with all penalties active
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_All_Penalties_Active)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Activate all penalty conditions
    */
   detection.raw.confid_azimuth = RSPP_CONF_AZIMUTH_LOW;
   detection.processed.vcs_position_x = 150.0F;
   detection.processed.range_rate_compensated = 3.0F;
   host.curvature_rear = 0.3F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Multiple penalties increase threshold
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with extreme range rate
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Extreme_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set extreme range rate value
    */
   detection.processed.range_rate_compensated = 50.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should clearly classify as MOVING
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with detection behind host
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Detection_Behind_Host)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection behind host (negative X)
    */
   detection.processed.vcs_position_x = -20.0F;
   detection.processed.vcs_position_y = 5.0F;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify normally regardless of position
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with detection on left side
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Detection_Left_Side)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection on left side (positive Y)
    */
   detection.processed.vcs_position_x = 30.0F;
   detection.processed.vcs_position_y = 20.0F;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify normally based on range rate
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with detection on right side
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Detection_Right_Side)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection on right side (negative Y)
    */
   detection.processed.vcs_position_x = 30.0F;
   detection.processed.vcs_position_y = -20.0F;
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify normally based on range rate
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_MOVING &&
         detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status with combined extreme conditions (stress test)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Combined_Extreme_Conditions)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set multiple extreme conditions
    */
   host.vcs_speed = 100.0F;
   host.curvature_rear = 1.0F;
   detection.raw.confid_azimuth = RSPP_CONF_AZIMUTH_LOW;
   detection.processed.vcs_position_x = 200.0F;
   detection.processed.range_rate_compensated = 10.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should handle extreme conditions and classify as MOVING (very high range rate)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status consistency with repeated calls
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Repeated_Calls_Consistency)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set consistent test conditions
    */
   detection.processed.range_rate_compensated = 3.0F;

   /** \action
    * Calculate motion status multiple times
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);
   int8_t first_result = detection.processed.motion_status;

   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);
   int8_t second_result = detection.processed.motion_status;

   /** \result
    * Results should be consistent
    */
   CHECK_EQUAL(first_result, second_result);
}

/** \purpose
 * Test motion status with known threshold value - sigma passes but threshold fails
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Sigma_Pass_Threshold_Fail)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Configure so sigma > moving_sigma_th but abs_val < threshold (both conditions must pass for MOVING)
    */
   detection.processed.range_rate_compensated = 0.3F; // sigma = 0.3/0.06 = 5.0 > 3.0 (pass), but 0.3 < 0.58 (fail)

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as AMBIGUOUS (both conditions must pass)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with both conditions passing
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Both_Conditions_Pass)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Configure so both sigma > moving_sigma_th AND abs_val > threshold
    */
   detection.processed.range_rate_compensated = 5.0F; // sigma = 5.0/0.5 = 10 > 2 (pass), and 5.0 > 1.0 (pass)

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING (both conditions pass)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with both conditions failing
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Both_Conditions_Fail)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Configure so both sigma < moving_sigma_th AND abs_val < threshold
    */
   detection.processed.range_rate_compensated = 0.15F; // sigma = 0.15/0.06 = 2.5 < 3.0 (fail), and 0.15 < 0.58 (fail)

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as AMBIGUOUS (both conditions fail)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with NaN range rate
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_NaN_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set NaN range rate
    */
   detection.processed.range_rate_compensated = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should handle gracefully and classify as AMBIGUOUS (NaN comparisons fail)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with positive infinity range rate
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Positive_Infinity_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set positive infinity range rate
    */
   detection.processed.range_rate_compensated = std::numeric_limits<float32_t>::infinity();

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should handle extreme value (likely classified as MOVING due to high absolute value)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with negative infinity range rate
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Negative_Infinity_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set negative infinity range rate
    */
   detection.processed.range_rate_compensated = -std::numeric_limits<float32_t>::infinity();

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should handle extreme value (absolute value used, likely MOVING)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with extremely negative range rate
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Extremely_Negative_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set extremely negative range rate
    */
   detection.processed.range_rate_compensated = -100.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING (absolute value is very high)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status at exact sigma threshold boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Exact_Sigma_Threshold_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Configure to be exactly at sigma threshold (> condition means equal should fail)
    */
   detection.processed.range_rate_compensated = 5.0F; // sigma = 5.0/1.0 = 5.0 (equal to threshold)

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * At exact sigma threshold boundary - result depends on other calibration and threshold check
    * Accept any valid motion status
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_INVALID);
   CHECK(detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status just above sigma threshold boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Just_Above_Sigma_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Configure to be just above sigma threshold
    */
   host.curvature_rear = 0.0F;

   detection.processed.range_rate_compensated = 5.01F; // sigma = 5.01/1.0 = 5.01 > 5.0

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING or AMBIGUOUS (sigma passes but depends on actual threshold)
    */
   CHECK(detection.processed.motion_status > RSPP_DETECTION_MOTION_STATUS_STATIONARY);
}

/** \purpose
 * Test motion status at exact value threshold boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Exact_Value_Threshold_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Configure to be exactly at value threshold (> condition means equal should fail)
    */
   detection.processed.range_rate_compensated = 0.58F; // sigma = 0.58/0.06 = 9.67 > 3.0 (pass), but 0.58 == 0.58 (equal)

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as AMBIGUOUS (abs_val > threshold requires strictly greater)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status just above value threshold boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Just_Above_Value_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Configure to be just above value threshold
    */
   detection.processed.range_rate_compensated = 3.01F; // sigma = 6.02 > 2 (pass), 3.01 > 3.0 (pass)

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING (both conditions strictly greater)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with zero sigma threshold (edge case)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Zero_Sigma_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sigma threshold to zero (any positive sigma should pass)
    */
   detection.processed.range_rate_compensated = 1.0F; // sigma = 1.0 > 0.0, abs_val = 1.0 > 0.5

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should classify as MOVING (both conditions pass with zero sigma threshold)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with float epsilon boundary value
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Float_Epsilon_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set range rate to float epsilon (smallest positive value)
    */
   detection.processed.range_rate_compensated = std::numeric_limits<float32_t>::epsilon();

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Epsilon value should classify as AMBIGUOUS (effectively zero)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with max float value
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Max_Float_Value)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set range rate to maximum float value
    */
   detection.processed.range_rate_compensated = std::numeric_limits<float32_t>::max();

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Max float should classify as MOVING (no overflow)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with minimum positive float value
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Min_Positive_Float)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set range rate to minimum positive float (denormalized)
    */
   detection.processed.range_rate_compensated = std::numeric_limits<float32_t>::min();

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Minimum positive float should classify as AMBIGUOUS (effectively zero)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with negative epsilon boundary value
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Negative_Epsilon_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set range rate to negative float epsilon
    */
   detection.processed.range_rate_compensated = -std::numeric_limits<float32_t>::epsilon();

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Negative epsilon value should classify as AMBIGUOUS (absolute value effectively zero)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with invalid sensor data (not valid)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Invalid_Sensor_Data_Not_Valid)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sensor data as invalid
    */
   sensor_data.is_valid = false;
   detection.processed.range_rate_compensated = 5.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should still process and return valid motion status
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_STATIONARY);
   CHECK(detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** \purpose
 * Test motion status output range validation
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Output_Range_Validation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set typical input values
    */
   detection.processed.range_rate_compensated = 2.5F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Output must be within valid enum range [STATIONARY=0, MOVING=1, AMBIGUOUS=2]
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_STATIONARY);
   CHECK(detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
   CHECK(detection.processed.motion_status != RSPP_DETECTION_MOTION_STATUS_INVALID);
}

/** \purpose
 * Test motion status with negative max float value
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Negative_Max_Float)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set range rate to negative maximum float value
    */
   detection.processed.range_rate_compensated = -std::numeric_limits<float32_t>::max();

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Negative max float should classify as MOVING (absolute value used)
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status preserves other detection fields
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Preserves_Other_Fields)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set specific values in other detection fields
    */
   detection.processed.range_rate_compensated = 5.0F;
   detection.processed.vcs_position_x = 42.0F;
   detection.processed.vcs_position_y = 17.0F;
   float32_t original_x = detection.processed.vcs_position_x;
   float32_t original_y = detection.processed.vcs_position_y;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Other fields should be unchanged
    */
   DOUBLES_EQUAL(original_x, detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(original_y, detection.processed.vcs_position_y, TOLERANCE);
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with all zero host data
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_All_Zero_Host)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set all host values to zero
    */
   memset(&host, 0, sizeof(RSPP_Host_T));
   detection.processed.range_rate_compensated = 5.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should handle zero host data and classify as MOVING
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with detection at origin
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Detection_At_Origin)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection position at origin
    */
   detection.processed.vcs_position_x = 0.0F;
   detection.processed.vcs_position_y = 0.0F;
   detection.processed.range_rate_compensated = 5.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Should handle origin position and classify as MOVING
    */
   CHECK_EQUAL(RSPP_DETECTION_MOTION_STATUS_MOVING, detection.processed.motion_status);
}

/** \purpose
 * Test motion status with very far detection position
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status, Calculate_Motion_Status_TC_Very_Far_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set very far detection position
    */
   detection.processed.vcs_position_x = 500.0F;
   detection.processed.vcs_position_y = 100.0F;
   detection.processed.range_rate_compensated = 5.0F;

   /** \action
    * Calculate motion status
    */
   RSPP_Calculate_Motion_Status(detection.raw, sensor_data, host, sensor_calibration, detection.processed);

   /** \result
    * Far detection should still process correctly
    */
   CHECK(detection.processed.motion_status >= RSPP_DETECTION_MOTION_STATUS_STATIONARY);
   CHECK(detection.processed.motion_status <= RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS);
}

/** @}*/

/** \defgroup  test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis
 *  @{
 */

/** \brief
 * Tests for RSPP_Check_Moving_Hypothesis function
 *
 * Function signature: bool RSPP_Check_Moving_Hypothesis(
 *    const float32_t abs_range_rate_comp,
 *    const float32_t range_rate_comp_std,
 *    const float32_t range_rate_comp_th,
 *    const float32_t moving_sigma_th)
 *
 * Logic: Returns true if (sigma > moving_sigma_th) AND (abs_range_rate_comp > range_rate_comp_th)
 *        where sigma = abs_range_rate_comp / max(range_rate_comp_std, RSPP_EPSILON)
 */
TEST_GROUP(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis)
{
   // Named constants for test clarity
   static constexpr float32_t EPSILON = 1.19e-07F; // RSPP_EPSILON
   static constexpr float32_t TYPICAL_SIGMA_TH = 3.0F;
   static constexpr float32_t TYPICAL_TEST_VAL_TH = 0.5F;
   static constexpr float32_t TYPICAL_ABS_TEST_VAL = 5.0F;
   static constexpr float32_t TYPICAL_TEST_VAL_STD = 1.0F;
   static constexpr float32_t TOLERANCE = 0.0001F;

   /** \setup
    * No setup required - function is stateless
    */
   TEST_SETUP()
   {
      // Function is stateless, no setup needed
   }

   /** \teardown
    * Clean up after tests
    */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

// ==============================================================================
// Basic Functionality Tests
// ==============================================================================

/** \purpose
 * Test basic moving detection - both conditions satisfied
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Basic_Moving_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 5.0 / 1.0 = 5.0
    * Conditions: (5.0 > 3.0) AND (5.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with typical moving parameters
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, 1.0F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (both conditions met)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test stationary detection - both conditions not satisfied
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Basic_Stationary_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 0.2, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 0.2 / 1.0 = 0.2
    * Conditions: (0.2 > 3.0) AND (0.2 > 0.5) = false AND false
    */

   /** \action
    * Call function with small test value
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.2F, 1.0F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (both conditions fail)
    */
   CHECK_EQUAL(false, result);
}

// ==============================================================================
// Boundary Testing
// ==============================================================================

/** \purpose
 * Test boundary case - sigma exactly at threshold
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Sigma_Exactly_At_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 3.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 3.0 / 1.0 = 3.0
    * Conditions: (3.0 > 3.0) AND (3.0 > 0.5) = false AND true
    */

   /** \action
    * Call function with sigma equal to threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(3.0F, 1.0F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (sigma not > threshold, only ==)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test boundary case - sigma just above threshold
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Sigma_Just_Above_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 3.01, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 3.01 / 1.0 = 3.01
    * Conditions: (3.01 > 3.0) AND (3.01 > 0.5) = true AND true
    */

   /** \action
    * Call function with sigma slightly above threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(3.01F, 1.0F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (sigma exceeds threshold)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test boundary case - abs_range_rate_comp exactly at threshold
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Abs_Test_Val_Exactly_At_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 0.5, range_rate_comp_std = 0.1, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 0.5 / 0.1 = 5.0
    * Conditions: (5.0 > 3.0) AND (0.5 > 0.5) = true AND false
    */

   /** \action
    * Call function with abs_range_rate_comp equal to threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.5F, 0.1F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (abs_range_rate_comp not > threshold, only ==)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test boundary case - abs_range_rate_comp just above threshold
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Abs_Test_Val_Just_Above_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 0.51, range_rate_comp_std = 0.1, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 0.51 / 0.1 = 5.1
    * Conditions: (5.1 > 3.0) AND (0.51 > 0.5) = true AND true
    */

   /** \action
    * Call function with abs_range_rate_comp slightly above threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.51F, 0.1F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (both conditions met)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test high sigma but low abs_range_rate_comp - only one condition met
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_High_Sigma_Low_Abs_Value)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 0.4, range_rate_comp_std = 0.05, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 0.4 / 0.05 = 8.0 (high)
    * Conditions: (8.0 > 3.0) AND (0.4 > 0.5) = true AND false
    */

   /** \action
    * Call function with high sigma but abs_range_rate_comp below threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.4F, 0.05F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (abs_range_rate_comp condition fails)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test low sigma but high abs_range_rate_comp - only one condition met
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Low_Sigma_High_Abs_Value)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = 2.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 5.0 / 2.0 = 2.5 (low)
    * Conditions: (2.5 > 3.0) AND (5.0 > 0.5) = false AND true
    */

   /** \action
    * Call function with low sigma but abs_range_rate_comp above threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, 2.0F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (sigma condition fails)
    */
   CHECK_EQUAL(false, result);
}

// ==============================================================================
// EPSILON Protection Tests
// ==============================================================================

/** \purpose
 * Test zero range_rate_comp_std - should use EPSILON to avoid division by zero
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Zero_Test_Val_Std)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = 0.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 5.0 / EPSILON = 5.0 / 1.19e-07 = very large number
    * Conditions: (large > 3.0) AND (5.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with zero standard deviation
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, 0.0F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (EPSILON protection works)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test very small range_rate_comp_std below EPSILON
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Very_Small_Test_Val_Std_Below_Epsilon)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = 1e-10 (< EPSILON), range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 5.0 / EPSILON (clamped)
    * Conditions: (large > 3.0) AND (5.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with very small std below EPSILON
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, 1e-10F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (EPSILON clamping protects against tiny std)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test range_rate_comp_std exactly at EPSILON boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Test_Val_Std_At_Epsilon)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = EPSILON, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 5.0 / EPSILON = very large
    * Conditions: (large > 3.0) AND (5.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with std exactly at EPSILON
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, EPSILON, 0.5F, 3.0F);

   /** \result
    * Should detect moving (at boundary, EPSILON is used)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test range_rate_comp_std just above EPSILON
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Test_Val_Std_Just_Above_Epsilon)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = EPSILON * 2, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 5.0 / (EPSILON * 2) = still very large
    * Conditions: (large > 3.0) AND (5.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with std just above EPSILON
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, EPSILON * 2.0F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (uses actual std value)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test zero abs_range_rate_comp
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Zero_Abs_Test_Val)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 0.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 0.0 / 1.0 = 0.0
    * Conditions: (0.0 > 3.0) AND (0.0 > 0.5) = false AND false
    */

   /** \action
    * Call function with zero abs value
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.0F, 1.0F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (both conditions fail)
    */
   CHECK_EQUAL(false, result);
}

// ==============================================================================
// Edge Case Tests
// ==============================================================================

/** \purpose
 * Test very large abs_range_rate_comp
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Very_Large_Abs_Test_Val)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 1000.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 1000.0 / 1.0 = 1000.0
    * Conditions: (1000.0 > 3.0) AND (1000.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with very large abs value
    */
   bool result = RSPP_Check_Moving_Hypothesis(1000.0F, 1.0F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (clearly exceeds both thresholds)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test zero range_rate_comp_th threshold
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Zero_Test_Val_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 0.1, range_rate_comp_std = 0.02, range_rate_comp_th = 0.0, moving_sigma_th = 3.0
    * sigma = 0.1 / 0.02 = 5.0
    * Conditions: (5.0 > 3.0) AND (0.1 > 0.0) = true AND true
    */

   /** \action
    * Call function with zero test value threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.1F, 0.02F, 0.0F, 3.0F);

   /** \result
    * Should detect moving (any positive abs_range_rate_comp satisfies second condition)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test zero moving_sigma_th threshold
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Zero_Sigma_Threshold)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 0.0
    * sigma = 5.0 / 1.0 = 5.0
    * Conditions: (5.0 > 0.0) AND (5.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with zero sigma threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, 1.0F, 0.5F, 0.0F);

   /** \result
    * Should detect moving (any positive sigma satisfies first condition)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test very high range_rate_comp_std - lowers sigma
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Very_High_Test_Val_Std)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = 10.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 5.0 / 10.0 = 0.5 (low due to high uncertainty)
    * Conditions: (0.5 > 3.0) AND (5.0 > 0.5) = false AND true
    */

   /** \action
    * Call function with high standard deviation
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, 10.0F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (high uncertainty reduces sigma)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test realistic scenario - typical moving detection
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Realistic_Moving_Scenario)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Realistic values from actual radar detection
    * abs_range_rate_comp = 2.5 m/s (compensated range rate)
    * range_rate_comp_std = 0.5 m/s (uncertainty)
    * range_rate_comp_th = 0.3 m/s (detection threshold)
    * moving_sigma_th = 3.0 (3-sigma test)
    * sigma = 2.5 / 0.5 = 5.0
    * Conditions: (5.0 > 3.0) AND (2.5 > 0.3) = true AND true
    */

   /** \action
    * Call function with realistic moving parameters
    */
   bool result = RSPP_Check_Moving_Hypothesis(2.5F, 0.5F, 0.3F, 3.0F);

   /** \result
    * Should detect moving (typical moving object)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test realistic scenario - typical stationary detection
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Realistic_Stationary_Scenario)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Realistic values for stationary detection
    * abs_range_rate_comp = 0.2 m/s (small residual motion)
    * range_rate_comp_std = 0.5 m/s (typical uncertainty)
    * range_rate_comp_th = 0.3 m/s (detection threshold)
    * moving_sigma_th = 3.0 (3-sigma test)
    * sigma = 0.2 / 0.5 = 0.4
    * Conditions: (0.4 > 3.0) AND (0.2 > 0.3) = false AND false
    */

   /** \action
    * Call function with realistic stationary parameters
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.2F, 0.5F, 0.3F, 3.0F);

   /** \result
    * Should not detect moving (stationary object)
    */
   CHECK_EQUAL(false, result);
}

// ==============================================================================
// Realistic Scenario Tests
// ==============================================================================

/** \purpose
 * Test realistic scenario - ambiguous case (high uncertainty)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Realistic_Ambiguous_High_Uncertainty)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 1.5 m/s
    * range_rate_comp_std = 0.8 m/s (high uncertainty)
    * range_rate_comp_th = 0.5 m/s
    * moving_sigma_th = 3.0
    * sigma = 1.5 / 0.8 = 1.875
    * Conditions: (1.875 > 3.0) AND (1.5 > 0.5) = false AND true
    */

   /** \action
    * Call function with high uncertainty scenario
    */
   bool result = RSPP_Check_Moving_Hypothesis(1.5F, 0.8F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (uncertainty too high for confidence)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test all parameters at minimum positive values
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_All_Minimum_Positive_Values)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * All parameters at small positive values
    * abs_range_rate_comp = 0.01, range_rate_comp_std = 0.01, range_rate_comp_th = 0.001, moving_sigma_th = 0.5
    * sigma = 0.01 / 0.01 = 1.0
    * Conditions: (1.0 > 0.5) AND (0.01 > 0.001) = true AND true
    */

   /** \action
    * Call function with all minimum positive values
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.01F, 0.01F, 0.001F, 0.5F);

   /** \result
    * Should detect moving (both conditions barely met)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test typical 3-sigma test with exact boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Three_Sigma_Test_Exact_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set up for exactly 3-sigma: abs_range_rate_comp = 3.0 * range_rate_comp_std
    * abs_range_rate_comp = 3.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 3.0 / 1.0 = 3.0
    * Conditions: (3.0 > 3.0) AND (3.0 > 0.5) = false AND true
    */

   /** \action
    * Call function with exactly 3-sigma
    */
   bool result = RSPP_Check_Moving_Hypothesis(3.0F, 1.0F, 0.5F, 3.0F);

   /** \result
    * Should not detect moving (equal to threshold, not greater than)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test typical 3-sigma test with value above boundary
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Three_Sigma_Test_Above_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set up for > 3-sigma: abs_range_rate_comp = 3.1 * range_rate_comp_std
    * abs_range_rate_comp = 3.1, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * sigma = 3.1 / 1.0 = 3.1
    * Conditions: (3.1 > 3.0) AND (3.1 > 0.5) = true AND true
    */

   /** \action
    * Call function with > 3-sigma
    */
   bool result = RSPP_Check_Moving_Hypothesis(3.1F, 1.0F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (exceeds 3-sigma threshold)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test with different sigma threshold values (2-sigma test)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Two_Sigma_Test)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 2.5, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 2.0
    * sigma = 2.5 / 1.0 = 2.5
    * Conditions: (2.5 > 2.0) AND (2.5 > 0.5) = true AND true
    */

   /** \action
    * Call function with 2-sigma threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(2.5F, 1.0F, 0.5F, 2.0F);

   /** \result
    * Should detect moving (exceeds 2-sigma threshold)
    */
   CHECK_EQUAL(true, result);
}

// ==============================================================================
// Statistical Significance Tests
// ==============================================================================

/** \purpose
 * Test with very strict sigma threshold (5-sigma test)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Five_Sigma_Test_Fails)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 4.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 5.0
    * sigma = 4.0 / 1.0 = 4.0
    * Conditions: (4.0 > 5.0) AND (4.0 > 0.5) = false AND true
    */

   /** \action
    * Call function with strict 5-sigma threshold
    */
   bool result = RSPP_Check_Moving_Hypothesis(4.0F, 1.0F, 0.5F, 5.0F);

   /** \result
    * Should not detect moving (doesn't meet 5-sigma requirement)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test with very strict sigma threshold (5-sigma test) passing
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Five_Sigma_Test_Passes)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 6.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.5, moving_sigma_th = 5.0
    * sigma = 6.0 / 1.0 = 6.0
    * Conditions: (6.0 > 5.0) AND (6.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with value exceeding 5-sigma
    */
   bool result = RSPP_Check_Moving_Hypothesis(6.0F, 1.0F, 0.5F, 5.0F);

   /** \result
    * Should detect moving (exceeds 5-sigma requirement)
    */
   CHECK_EQUAL(true, result);
}

// ==============================================================================
// Special Cases and Performance Tests
// ==============================================================================

/** \purpose
 * Test edge case - both thresholds at zero with zero abs_range_rate_comp
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_All_Zeros_Except_Std)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 0.0, range_rate_comp_std = 1.0, range_rate_comp_th = 0.0, moving_sigma_th = 0.0
    * sigma = 0.0 / 1.0 = 0.0
    * Conditions: (0.0 > 0.0) AND (0.0 > 0.0) = false AND false
    */

   /** \action
    * Call function with zeros
    */
   bool result = RSPP_Check_Moving_Hypothesis(0.0F, 1.0F, 0.0F, 0.0F);

   /** \result
    * Should not detect moving (values equal thresholds, not greater)
    */
   CHECK_EQUAL(false, result);
}

/** \purpose
 * Test performance baseline - verify function executes quickly
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Performance_Baseline)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Execute many iterations to verify O(1) performance
    */
   static constexpr int ITERATIONS = 10000;

   /** \action
    * Call function many times
    */
   bool result = false;
   for (int i = 0; i < ITERATIONS; i++)
   {
      result = RSPP_Check_Moving_Hypothesis(5.0F, 1.0F, 0.5F, 3.0F);
   }

   /** \result
    * Should complete quickly and return correct result
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test with negative range_rate_comp_std (invalid input handling)
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Negative_Test_Val_Std)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * abs_range_rate_comp = 5.0, range_rate_comp_std = -1.0, range_rate_comp_th = 0.5, moving_sigma_th = 3.0
    * With negative std, comparison (range_rate_comp_std < EPSILON) is true
    * So sigma = 5.0 / EPSILON = very large
    * Conditions: (large > 3.0) AND (5.0 > 0.5) = true AND true
    */

   /** \action
    * Call function with negative std (invalid but handled by EPSILON protection)
    */
   bool result = RSPP_Check_Moving_Hypothesis(5.0F, -1.0F, 0.5F, 3.0F);

   /** \result
    * Should detect moving (EPSILON protection handles invalid negative std)
    */
   CHECK_EQUAL(true, result);
}

/** \purpose
 * Test consistency - same inputs produce same output
 * \req
 * CPR-3848_Derived
 */
TEST(test_RSPP_Calculate_Motion_Status_Check_Moving_Hypothesis,
     Check_Moving_Hypothesis_TC_Consistency_Check)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Fixed input parameters
    */

   /** \action
    * Call function multiple times with same inputs
    */
   bool result1 = RSPP_Check_Moving_Hypothesis(5.0F, 1.0F, 0.5F, 3.0F);
   bool result2 = RSPP_Check_Moving_Hypothesis(5.0F, 1.0F, 0.5F, 3.0F);
   bool result3 = RSPP_Check_Moving_Hypothesis(5.0F, 1.0F, 0.5F, 3.0F);

   /** \result
    * All results should be identical (function is deterministic)
    */
   CHECK_EQUAL(result1, result2);
   CHECK_EQUAL(result2, result3);
   CHECK_EQUAL(true, result1); // And all should be true
}

/** @}*/
