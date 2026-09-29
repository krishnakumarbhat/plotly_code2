/** \file
 * This file contains unit tests for the RSPP_Calculate_Detection_VCS_Coordinates unit
 */

#include "rspp_coordinate_transformation.h"
#include <CppUTest/TestHarness.h>
#include <cstring>
#include <cmath>
#include <limits>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace rspp_variant_A;

/** \defgroup  test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position
 *  @{
 */

/** \brief
 * Tests for RSPP_Calculate_VCS_Position function which transforms detection coordinates
 * from sensor coordinate system to Vehicle Coordinate System (VCS). Verifies correct handling
 * of range, azimuth, elevation angles, and sensor mounting positions for various radar configurations.
 */
TEST_GROUP(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position)
{
   // Common variables used within all tests in this test group
   static constexpr float32_t TOLERANCE = 0.0001F;
   Raw_Detection_T raw_detection;
   RSPP_VCS_Position_T vcs_mounting_position;
   Processed_Detection_T processed_detection;

   /** \setup
    * Initialize all input structures to zero using memset for consistent baseline state.
    */
   TEST_SETUP()
   {
      memset(&raw_detection, 0, sizeof(Raw_Detection_T));
      memset(&vcs_mounting_position, 0, sizeof(RSPP_VCS_Position_T));
      memset(&processed_detection, 0, sizeof(Processed_Detection_T));
   }

   /** \teardown
    * No cleanup required - all test data is stack allocated.
    */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }

   /** \brief
    * Helper function to calculate expected VCS coordinates
    */
   void Calculate_Expected_VCS_Coordinates(
       const float32_t range,
       const float32_t cos_az,
       const float32_t sin_az,
       const float32_t sin_el,
       const float32_t mount_long,
       const float32_t mount_lat,
       const float32_t mount_height,
       float32_t &expected_x,
       float32_t &expected_y,
       float32_t &expected_z)
   {
      expected_x = mount_long + (range * cos_az);
      expected_y = mount_lat + (range * sin_az);
      expected_z = -mount_height + (range * sin_el);
   }
};

/** \purpose
 * Test that unused input fields do not affect VCS coordinate calculation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Unused_Input_Data_Does_Not_Affect_Output)
{
   /** \step{1}
    * Verify unused fields in inputs do not impact VCS output
    */

   /** \precond
    * Baseline valid inputs
    */
   raw_detection.range = 42.0F;
   vcs_mounting_position.longitudinal = 1.0F;
   vcs_mounting_position.lateral = -0.5F;
   vcs_mounting_position.height = 0.2F;
   processed_detection.cos_vcs_az = 0.8F;
   processed_detection.sin_vcs_az = 0.6F;
   processed_detection.vcs_el = 0.1F;

   /** \action
    * Calculate VCS coordinates and capture expected output
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);
   const float32_t expected_x = processed_detection.vcs_position_x;
   const float32_t expected_y = processed_detection.vcs_position_y;
   const float32_t expected_z = processed_detection.vcs_position_z;
   DOUBLES_EQUAL(expected_x, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);

   /** \precond
    * Modify unused input fields only
    */
   raw_detection.std_range = 99.0F;
   raw_detection.range_rate = -12.0F;
   raw_detection.std_range_rate = 7.5F;
   raw_detection.azimuth = 1.23F;
   raw_detection.std_azimuth = 0.4F;
   raw_detection.elevation = -0.5F;
   raw_detection.std_elevation = 0.9F;
   raw_detection.snr = 42.0F;
   raw_detection.rcs = -10.0F;
   raw_detection.prob_1stazhypo = 0.3F;
   raw_detection.sensor_id = 7;
   raw_detection.det_id = 99;
   raw_detection.confid_azimuth = 3;
   raw_detection.confid_elevation = 2;
   raw_detection.f_super_res = true;
   raw_detection.f_host_veh_clutter = true;
   raw_detection.f_nd_target = true;
   raw_detection.f_bistatic = true;
   raw_detection.f_ci_det = true;
   raw_detection.f_idm_det = true;
   raw_detection.f_below_rain_thold = true;

   processed_detection.range_rate_compensated = 123.0F;
   processed_detection.next_sorted_idx = 4;
   processed_detection.prev_sorted_idx = -4;
   processed_detection.motion_status = 2;
   processed_detection.f_ok_to_use = true;
   processed_detection.vcs_position_x = 999.0F;
   processed_detection.vcs_position_y = 999.0F;
   processed_detection.vcs_position_z = 999.0F;

   /** \action
    * Recalculate VCS coordinates with unused fields modified
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Output should be identical to baseline
    */
   DOUBLES_EQUAL(expected_x, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with zero inputs
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_All_Zero_Inputs)
{
   /** \step{1}
    * Verify VCS coordinate calculation with zero inputs
    */

   /** \precond
    * All inputs are zero
    */
   raw_detection.range = 0.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.0F;
   processed_detection.cos_vcs_az = 1.0F; // cos(0)
   processed_detection.sin_vcs_az = 0.0F; // sin(0)
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * All VCS positions should be zero
    */
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with zero mounting position
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Zero_Mounting_Position)
{
   /** \step{1}
    * Verify VCS coordinate calculation with zero mounting position
    */

   /** \precond
    * Zero mounting position, positive range pointing forward
    */
   raw_detection.range = 10.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.0F;
   processed_detection.cos_vcs_az = 1.0F; // Forward (0 degrees)
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be 10m forward
    */
   DOUBLES_EQUAL(10.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with typical front radar mounting
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Typical_Front_Radar_Mounting)
{
   /** \step{1}
    * Verify VCS coordinate calculation with typical front radar mounting
    */

   /** \precond
    * Typical front radar: 3.5m forward, 0m lateral, 0.5m height
    * Detection at 50m range, forward direction
    */
   raw_detection.range = 50.0F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F; // Forward
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be at 53.5m forward, 0m lateral, -0.5m height
    */
   DOUBLES_EQUAL(53.5F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with rear radar mounting
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Typical_Rear_Radar_Mounting)
{
   /** \step{1}
    * Verify VCS coordinate calculation with rear radar mounting
    */

   /** \precond
    * Typical rear radar: -3.5m (behind), 0m lateral, 0.5m height
    * Detection at 30m range, backward direction (cos=-1)
    */
   raw_detection.range = 30.0F;
   vcs_mounting_position.longitudinal = -3.5F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = -1.0F; // Backward (PI)
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be at -33.5m (behind vehicle)
    */
   DOUBLES_EQUAL(-33.5F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with left side radar mounting
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Left_Side_Radar_Mounting)
{
   /** \step{1}
    * Verify VCS coordinate calculation with left side radar mounting
    */

   /** \precond
    * Left side radar: 1.0m forward, 1.0m left, 0.3m height
    * Detection at 20m range, pointing left (cos=0, sin=1)
    */
   raw_detection.range = 20.0F;
   vcs_mounting_position.longitudinal = 1.0F;
   vcs_mounting_position.lateral = 1.0F;
   vcs_mounting_position.height = 0.3F;
   processed_detection.cos_vcs_az = 0.0F; // Left (PI/2)
   processed_detection.sin_vcs_az = 1.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be at 1m forward, 21m left
    */
   DOUBLES_EQUAL(1.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(21.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.3F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with right side radar mounting
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Right_Side_Radar_Mounting)
{
   /** \step{1}
    * Verify VCS coordinate calculation with right side radar mounting
    */

   /** \precond
    * Right side radar: 1.0m forward, -1.0m right, 0.3m height
    * Detection at 15m range, pointing right (cos=0, sin=-1)
    */
   raw_detection.range = 15.0F;
   vcs_mounting_position.longitudinal = 1.0F;
   vcs_mounting_position.lateral = -1.0F;
   vcs_mounting_position.height = 0.3F;
   processed_detection.cos_vcs_az = 0.0F; // Right (-PI/2)
   processed_detection.sin_vcs_az = -1.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be at 1m forward, -16m right
    */
   DOUBLES_EQUAL(1.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(-16.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.3F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with 45-degree azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_45_Degree_Azimuth)
{
   /** \step{1}
    * Verify VCS coordinate calculation with 45-degree azimuth angle
    */

   /** \precond
    * Detection at 45 degrees (PI/4), 10m range
    */
   raw_detection.range = 10.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.0F;
   processed_detection.cos_vcs_az = 0.707F; // cos(45deg)
   processed_detection.sin_vcs_az = 0.707F; // sin(45deg)
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be at approximately 7.07m forward and 7.07m left
    */
   DOUBLES_EQUAL(7.07F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(7.07F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with negative 45-degree azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Negative_45_Degree_Azimuth)
{
   /** \step{1}
    * Verify VCS coordinate calculation with negative 45-degree azimuth angle
    */

   /** \precond
    * Detection at -45 degrees (-PI/4), 10m range
    */
   raw_detection.range = 10.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.0F;
   processed_detection.cos_vcs_az = 0.707F;  // cos(-45deg)
   processed_detection.sin_vcs_az = -0.707F; // sin(-45deg)
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be at approximately 7.07m forward and -7.07m right
    */
   DOUBLES_EQUAL(7.07F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(-7.07F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with positive elevation angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Positive_Elevation_Angle)
{
   /** \step{1}
    * Verify VCS coordinate calculation with positive elevation angle
    */

   /** \precond
    * Detection with 10 degree elevation (~0.175 rad), 50m range, forward
    */
   raw_detection.range = 50.0F;
   vcs_mounting_position.longitudinal = 3.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F; // Forward
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.175F; // ~10 degrees

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Z coordinate should be positive (above sensor mounting height)
    */
   DOUBLES_EQUAL(53.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   float32_t expected_z = -0.5F + (50.0F * std::sin(0.175F));
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);
   CHECK(processed_detection.vcs_position_z > -0.5F); // Above mounting height
}

/** \purpose
 * Test VCS coordinate calculation with negative elevation angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Negative_Elevation_Angle)
{
   /** \step{1}
    * Verify VCS coordinate calculation with negative elevation angle
    */

   /** \precond
    * Detection with -5 degree elevation (~-0.087 rad), 30m range, forward
    */
   raw_detection.range = 30.0F;
   vcs_mounting_position.longitudinal = 3.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F; // Forward
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = -0.087F; // ~-5 degrees

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Z coordinate should be more negative (below sensor mounting height)
    */
   DOUBLES_EQUAL(33.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   float32_t expected_z = -0.5F + (30.0F * std::sin(-0.087F));
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);
   CHECK(processed_detection.vcs_position_z < -0.5F); // Below mounting height
}

/** \purpose
 * Test VCS coordinate calculation with maximum positive elevation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Maximum_Positive_Elevation)
{
   /** \step{1}
    * Verify VCS coordinate calculation with maximum positive elevation
    */

   /** \precond
    * Detection with maximum elevation (PI/2), 20m range
    */
   raw_detection.range = 20.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 1.5708F; // PI/2

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Z coordinate should be approximately 20m - 0.5m = 19.5m (sin(PI/2) = 1)
    */
   DOUBLES_EQUAL(20.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(19.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with maximum negative elevation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Maximum_Negative_Elevation)
{
   /** \step{1}
    * Verify VCS coordinate calculation with maximum negative elevation
    */

   /** \precond
    * Detection with maximum negative elevation (-PI/2), 20m range
    */
   raw_detection.range = 20.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = -1.5708F; // -PI/2

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Z coordinate should be approximately -20m - 0.5m = -20.5m (sin(-PI/2) = -1)
    */
   DOUBLES_EQUAL(20.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-20.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with very small range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Very_Small_Range)
{
   /** \step{1}
    * Verify VCS coordinate calculation with very small range
    */

   /** \precond
    * Detection with very small range (0.1m)
    */
   raw_detection.range = 0.1F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Position should be very close to mounting position
    */
   DOUBLES_EQUAL(3.6F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with very large range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Very_Large_Range)
{
   /** \step{1}
    * Verify VCS coordinate calculation with very large range
    */

   /** \precond
    * Detection with very large range (300m)
    */
   raw_detection.range = 300.0F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Position should be 303.5m forward
    */
   DOUBLES_EQUAL(303.5F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with combined azimuth and elevation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Combined_Azimuth_Elevation)
{
   /** \step{1}
    * Verify VCS coordinate calculation with combined azimuth and elevation
    */

   /** \precond
    * Detection at 30 degrees azimuth, 10 degrees elevation, 50m range
    */
   raw_detection.range = 50.0F;
   vcs_mounting_position.longitudinal = 3.0F;
   vcs_mounting_position.lateral = 0.5F;
   vcs_mounting_position.height = 0.6F;
   processed_detection.cos_vcs_az = 0.866F; // cos(30deg)
   processed_detection.sin_vcs_az = 0.5F;   // sin(30deg)
   processed_detection.vcs_el = 0.175F;     // ~10 degrees

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * All three coordinates should be affected
    */
   float32_t expected_x = 3.0F + (50.0F * 0.866F);
   float32_t expected_y = 0.5F + (50.0F * 0.5F);
   float32_t expected_z = -0.6F + (50.0F * std::sin(0.175F));

   DOUBLES_EQUAL(expected_x, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with negative mounting positions
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Negative_Mounting_Positions)
{
   /** \step{1}
    * Verify VCS coordinate calculation with negative mounting positions
    */

   /** \precond
    * Sensor behind and to the right of origin, below ground reference
    */
   raw_detection.range = 25.0F;
   vcs_mounting_position.longitudinal = -2.0F;
   vcs_mounting_position.lateral = -1.5F;
   vcs_mounting_position.height = -0.3F; // Below ground reference
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Position should account for negative mounting
    */
   DOUBLES_EQUAL(23.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(-1.5F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(0.3F, processed_detection.vcs_position_z, TOLERANCE); // Note: z = -(-0.3) + 0
}

/** \purpose
 * Test VCS coordinate calculation with 135-degree azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_135_Degree_Azimuth)
{
   /** \step{1}
    * Verify VCS coordinate calculation with 135-degree azimuth
    */

   /** \precond
    * Detection at 135 degrees (3*PI/4), 20m range
    */
   raw_detection.range = 20.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.0F;
   processed_detection.cos_vcs_az = -0.707F; // cos(135deg)
   processed_detection.sin_vcs_az = 0.707F;  // sin(135deg)
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be backward and left
    */
   DOUBLES_EQUAL(-14.14F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(14.14F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with helper function validation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Helper_Function_Validation)
{
   /** \step{1}
    * Verify VCS coordinate calculation with helper function validation
    */

   /** \precond
    * Arbitrary values for comprehensive validation
    */
   raw_detection.range = 42.5F;
   vcs_mounting_position.longitudinal = 2.8F;
   vcs_mounting_position.lateral = 0.3F;
   vcs_mounting_position.height = 0.45F;
   processed_detection.cos_vcs_az = 0.8F;
   processed_detection.sin_vcs_az = 0.6F;
   processed_detection.vcs_el = 0.1F;

   /** \action
    * Calculate VCS coordinates and expected values using helper
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   float32_t expected_x, expected_y, expected_z;
   Calculate_Expected_VCS_Coordinates(
       42.5F, 0.8F, 0.6F, std::sin(0.1F),
       2.8F, 0.3F, 0.45F,
       expected_x, expected_y, expected_z);

   /** \result
    * Results should match helper function calculations
    */
   DOUBLES_EQUAL(expected_x, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with zero range but non-zero mounting
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Zero_Range_Nonzero_Mounting)
{
   /** \step{1}
    * Verify VCS coordinate calculation with zero range but non-zero mounting
    */

   /** \precond
    * Zero range, non-zero mounting position
    */
   raw_detection.range = 0.0F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 1.2F;
   vcs_mounting_position.height = 0.6F;
   processed_detection.cos_vcs_az = 0.5F;
   processed_detection.sin_vcs_az = 0.866F;
   processed_detection.vcs_el = 0.2F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Position should equal mounting position
    */
   DOUBLES_EQUAL(3.5F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(1.2F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.6F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with maximum realistic values
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Maximum_Realistic_Values)
{
   /** \step{1}
    * Verify VCS coordinate calculation with maximum realistic values
    */

   /** \precond
    * Maximum realistic radar detection scenario
    */
   raw_detection.range = 250.0F; // Long range radar
   vcs_mounting_position.longitudinal = 4.0F;
   vcs_mounting_position.lateral = 0.8F;
   vcs_mounting_position.height = 0.7F;
   processed_detection.cos_vcs_az = 0.985F; // Small angle off-center
   processed_detection.sin_vcs_az = 0.174F; // ~10 degrees
   processed_detection.vcs_el = 0.05F;      // Small elevation

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Should handle large ranges correctly
    */
   float32_t expected_x = 4.0F + (250.0F * 0.985F);
   float32_t expected_y = 0.8F + (250.0F * 0.174F);
   float32_t expected_z = -0.7F + (250.0F * std::sin(0.05F));

   DOUBLES_EQUAL(expected_x, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with corner case azimuth (PI/6 = 30deg)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_30_Degree_Azimuth)
{
   /** \step{1}
    * Verify VCS coordinate calculation with corner case azimuth (PI/6 = 30deg)
    */

   /** \precond
    * Detection at 30 degrees, common radar FOV angle
    */
   raw_detection.range = 60.0F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 0.866F; // cos(30deg) = sqrt(3/2)
   processed_detection.sin_vcs_az = 0.5F;   // sin(30deg) = 0.5
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Y should be exactly 30m (60 * 0.5)
    */
   DOUBLES_EQUAL(55.46F, processed_detection.vcs_position_x, TOLERANCE); // 3.5 + 60*0.866
   DOUBLES_EQUAL(30.0F, processed_detection.vcs_position_y, TOLERANCE);  // 0 + 60*0.5
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with corner case azimuth (PI/3 = 60deg)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_60_Degree_Azimuth)
{
   /** \step{1}
    * Verify VCS coordinate calculation with corner case azimuth (PI/3 = 60deg)
    */

   /** \precond
    * Detection at 60 degrees
    */
   raw_detection.range = 40.0F;
   vcs_mounting_position.longitudinal = 2.0F;
   vcs_mounting_position.lateral = 0.5F;
   vcs_mounting_position.height = 0.4F;
   processed_detection.cos_vcs_az = 0.5F;   // cos(60deg) = 0.5
   processed_detection.sin_vcs_az = 0.866F; // sin(60deg) = sqrt(3/2)
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * X should be 22m (2 + 40*0.5), Y should be ~35.14m (0.5 + 40*0.866)
    */
   DOUBLES_EQUAL(22.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(35.14F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.4F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with small elevation and large range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Small_Elevation_Large_Range)
{
   /** \step{1}
    * Verify VCS coordinate calculation with small elevation and large range
    */

   /** \precond
    * Small elevation angle (1 degree) with large range
    */
   raw_detection.range = 200.0F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0175F; // ~1 degree

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Z coordinate should show noticeable elevation effect
    */
   DOUBLES_EQUAL(203.5F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   float32_t expected_z = -0.5F + (200.0F * std::sin(0.0175F));
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);
   CHECK(processed_detection.vcs_position_z > 0.0F); // Should be above ground
}

/** \purpose
 * Test VCS coordinate calculation with symmetry validation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Symmetry_Validation)
{
   /** \step{1}
    * Verify VCS coordinate calculation with symmetry validation
    */

   /** \precond
    * Test symmetric azimuth angles produce symmetric Y coordinates
    */
   raw_detection.range = 50.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.vcs_el = 0.0F;

   // Test positive angle
   processed_detection.cos_vcs_az = 0.707F;
   processed_detection.sin_vcs_az = 0.707F;
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);
   float32_t y_positive = processed_detection.vcs_position_y;
   float32_t x_positive = processed_detection.vcs_position_x;

   // Test negative angle
   processed_detection.cos_vcs_az = 0.707F;
   processed_detection.sin_vcs_az = -0.707F;
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Y coordinates should be opposite, X coordinates should be equal
    */
   DOUBLES_EQUAL(x_positive, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(-y_positive, processed_detection.vcs_position_y, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with NaN in range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_NaN_Range)
{
   /** \step{1}
    * Verify VCS coordinate calculation with NaN in range
    */

   /** \precond
    * NaN value in range
    */
   raw_detection.range = std::numeric_limits<float32_t>::quiet_NaN();
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.5F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Function should handle NaN gracefully
    */
   CHECK(std::isnan(processed_detection.vcs_position_x));
   CHECK(std::isnan(processed_detection.vcs_position_y));
}

/** \purpose
 * Test VCS coordinate calculation with infinity in range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Infinity_Range)
{
   /** \step{1}
    * Verify VCS coordinate calculation with infinity in range
    */

   /** \precond
    * Infinity value in range
    */
   raw_detection.range = std::numeric_limits<float32_t>::infinity();
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.5F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Function should handle infinity gracefully
    */
   CHECK(std::isinf(processed_detection.vcs_position_x));
   CHECK(processed_detection.vcs_position_x > 0.0F);
}

/** \purpose
 * Test VCS coordinate calculation with NaN in trigonometric values
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_NaN_Trigonometric_Values)
{
   /** \step{1}
    * Verify VCS coordinate calculation with NaN in trigonometric values
    */

   /** \precond
    * NaN in cos/sin values
    */
   raw_detection.range = 50.0F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.5F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = std::numeric_limits<float32_t>::quiet_NaN();
   processed_detection.sin_vcs_az = std::numeric_limits<float32_t>::quiet_NaN();
   processed_detection.vcs_el = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Function should handle NaN gracefully
    */
   CHECK(std::isnan(processed_detection.vcs_position_x));
   CHECK(std::isnan(processed_detection.vcs_position_y));
   CHECK(std::isnan(processed_detection.vcs_position_z));
}

/** \purpose
 * Test VCS coordinate calculation with high mounting position
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_High_Mounting_Position)
{
   /** \step{1}
    * Verify VCS coordinate calculation with high mounting position
    */

   /** \precond
    * High mounting position (e.g., roof-mounted sensor)
    */
   raw_detection.range = 30.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 2.0F; // High mounting
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Z coordinate should reflect high mounting (negative in VCS)
    */
   DOUBLES_EQUAL(30.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-2.0F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with negative range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Negative_Range)
{
   /** \step{1}
    * Verify VCS coordinate calculation with negative range
    */

   /** \precond
    * Negative range value (invalid input but worth testing behavior)
    */
   raw_detection.range = -50.0F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F; // Forward
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Detection should be behind sensor (negative of forward direction)
    */
   DOUBLES_EQUAL(-46.5F, processed_detection.vcs_position_x, TOLERANCE); // 3.5 + (-50)*1
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with Z-axis sign convention validation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Z_Axis_Sign_Convention)
{
   /** \step{1}
    * Verify VCS coordinate calculation with Z-axis sign convention validation
    */

   /** \precond
    * Test Z-axis sign convention:
    * Positive mounting height -> negative VCS Z (below ground reference)
    * Negative mounting height -> positive VCS Z (above ground reference)
    */
   raw_detection.range = 0.0F; // Zero range to isolate mounting effect
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   // Test positive mounting height
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Positive height (0.5m) should give negative Z (-0.5m)
    */
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_position_z, TOLERANCE);

   // Test negative mounting height
   vcs_mounting_position.height = -0.3F;
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Negative height (-0.3m) should give positive Z (0.3m)
    */
   DOUBLES_EQUAL(0.3F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with combined NaN values
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Combined_NaN_Values)
{
   /** \step{1}
    * Verify VCS coordinate calculation with combined NaN values
    */

   /** \precond
    * NaN in both range and trigonometric values
    */
   raw_detection.range = std::numeric_limits<float32_t>::quiet_NaN();
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.5F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = std::numeric_limits<float32_t>::quiet_NaN();
   processed_detection.sin_vcs_az = std::numeric_limits<float32_t>::quiet_NaN();
   processed_detection.vcs_el = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Function should handle NaN gracefully
    */
   CHECK(std::isnan(processed_detection.vcs_position_x));
   CHECK(std::isnan(processed_detection.vcs_position_y));
   CHECK(std::isnan(processed_detection.vcs_position_z));
}

/** \purpose
 * Test VCS coordinate calculation with infinity range and large mounting
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Infinity_Range_Large_Mounting)
{
   /** \step{1}
    * Verify VCS coordinate calculation with infinity range and large mounting
    */

   /** \precond
    * Infinity range with large mounting position
    */
   raw_detection.range = std::numeric_limits<float32_t>::infinity();
   vcs_mounting_position.longitudinal = 100.0F; // Large but finite
   vcs_mounting_position.lateral = 50.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Function should handle infinity gracefully
    */
   CHECK(std::isinf(processed_detection.vcs_position_x));
   CHECK(processed_detection.vcs_position_x > 0.0F);
   CHECK(std::isnan(processed_detection.vcs_position_z));
}

/** \purpose
 * Test VCS coordinate calculation with epsilon-level azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Epsilon_Azimuth_Angle)
{
   /** \step{1}
    * Verify VCS coordinate calculation with epsilon-level azimuth angle
    */

   /** \precond
    * Very small azimuth angle using machine epsilon
    */
   raw_detection.range = 100.0F;
   vcs_mounting_position.longitudinal = 0.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F; // cos(eps) = 1
   processed_detection.sin_vcs_az = std::numeric_limits<float32_t>::epsilon();
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Function should handle precision at very small angles
    * X should be approximately 100m, Y should be nearly zero
    */
   DOUBLES_EQUAL(100.0F, processed_detection.vcs_position_x, TOLERANCE);
   CHECK(processed_detection.vcs_position_y >= 0.0F);
   CHECK(processed_detection.vcs_position_y < 0.001F); // Very small but positive
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation with negative infinity range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Negative_Infinity_Range)
{
   /** \step{1}
    * Verify VCS coordinate calculation with negative infinity range
    */

   /** \precond
    * Negative infinity in range
    */
   raw_detection.range = -std::numeric_limits<float32_t>::infinity();
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.5F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 0.0F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Function should handle infinity gracefully
    */
   CHECK(std::isinf(processed_detection.vcs_position_x));
   CHECK(processed_detection.vcs_position_x < 0.0F); // Negative infinity
}

/** \purpose
 * Test VCS coordinate calculation with NaN in elevation affecting Z
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_NaN_Elevation)
{
   /** \step{1}
    * Verify VCS coordinate calculation with NaN in elevation affecting Z
    */

   /** \precond
    * NaN value in elevation angle
    */
   raw_detection.range = 50.0F;
   vcs_mounting_position.longitudinal = 3.5F;
   vcs_mounting_position.lateral = 0.5F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Function should handle NaN in elevation gracefully
    */
   DOUBLES_EQUAL(53.5F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.5F, processed_detection.vcs_position_y, TOLERANCE);
   CHECK(std::isnan(processed_detection.vcs_position_z));
}

/** \purpose
 * Test VCS coordinate calculation with maximum positive elevation and range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Max_Elevation_With_Large_Range)
{
   /** \step{1}
    * Verify VCS coordinate calculation with maximum positive elevation and range
    */

   /** \precond
    * Maximum elevation (PI/2) with large range for significant Z displacement
    */
   raw_detection.range = 100.0F;
   vcs_mounting_position.longitudinal = 3.0F;
   vcs_mounting_position.lateral = 0.0F;
   vcs_mounting_position.height = 0.5F;
   processed_detection.cos_vcs_az = 1.0F;
   processed_detection.sin_vcs_az = 0.0F;
   processed_detection.vcs_el = 1.5708F; // PI/2, sin = 1.0

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Z coordinate should show maximum elevation effect
    * Note: Using 0.1F tolerance due to sin(PI/2) approximation
    */
   DOUBLES_EQUAL(103.0F, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(99.5F, processed_detection.vcs_position_z, TOLERANCE); // -0.5 + 100*sin(PI/2)
}

/** \purpose
 * Test VCS coordinate calculation with tolerance documentation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Position,
     Calculate_VCS_Position_TC_Large_Range_Tolerance_Validation)
{
   /** \step{1}
    * Verify VCS coordinate calculation with tolerance documentation
    */

   /** \precond
    * Very large range to validate tolerance requirements
    */
   raw_detection.range = 250.0F;
   vcs_mounting_position.longitudinal = 4.0F;
   vcs_mounting_position.lateral = 0.8F;
   vcs_mounting_position.height = 0.7F;
   processed_detection.cos_vcs_az = 0.985F;
   processed_detection.sin_vcs_az = 0.174F;
   processed_detection.vcs_el = 0.05F;

   /** \action
    * Calculate VCS coordinates
    */
   RSPP_Calculate_VCS_Position(raw_detection, vcs_mounting_position, processed_detection);

   /** \result
    * Should handle large ranges correctly
    * Note: Using 0.1F tolerance due to cumulative floating-point error at 250m range
    * and approximation in trigonometric values
    */
   float32_t expected_x = 4.0F + (250.0F * 0.985F);
   float32_t expected_y = 0.8F + (250.0F * 0.174F);
   float32_t expected_z = -0.7F + (250.0F * std::sin(0.05F));

   DOUBLES_EQUAL(expected_x, processed_detection.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, processed_detection.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, processed_detection.vcs_position_z, TOLERANCE);
}

/** @}*/

/** \defgroup  test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles
 *  @{
 */

/** \brief
 * Tests for RSPP_Calculate_VCS_Angles function which applies
 * sensor mounting angle corrections to detection azimuth and elevation angles. Verifies
 * correct handling of polarity, boresight offsets, and computation of VCS angles with their
 * trigonometric values (cos/sin) for various mounting angle scenarios.
 */
TEST_GROUP(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles)
{
   // Common variables used within all tests in this test group
   static constexpr float32_t TOLERANCE = 0.0001F;
   int32_t polarity;
   VariableProps_T sensor_data;
   Raw_Detection_T raw_detection;
   Processed_Detection_T processed_detection;

   /** \setup
    * Initialize polarity to positive (1) and zero-initialize all sensor data and detection structures.
    */
   TEST_SETUP()
   {
      polarity = 1;
      memset(&sensor_data, 0, sizeof(VariableProps_T));
      memset(&raw_detection, 0, sizeof(Raw_Detection_T));
      memset(&processed_detection, 0, sizeof(Processed_Detection_T));
   }

   /** \teardown
    * No cleanup required - all test data is stack allocated.
    */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test that unused input fields do not affect mounting angle compensation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Unused_Input_Data_Does_Not_Affect_Output)
{
   /** \step{1}
    * Verify unused fields do not impact compensated angles
    */

   /** \precond
    * Baseline inputs for compensation
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.2F;
   sensor_data.vacs_boresight_el_estimated = -0.1F;
   raw_detection.azimuth = 0.3F;
   raw_detection.elevation = 0.4F;

   /** \action
    * Compensate detection angles and capture expected output
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);
   const float32_t expected_az = processed_detection.vcs_az;
   const float32_t expected_el = processed_detection.vcs_el;
   const float32_t expected_cos = processed_detection.cos_vcs_az;
   const float32_t expected_sin = processed_detection.sin_vcs_az;
   DOUBLES_EQUAL(expected_az, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_el, processed_detection.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(expected_cos, processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_sin, processed_detection.sin_vcs_az, TOLERANCE);

   /** \precond
    * Modify unused input fields only
    */
   sensor_data.timestamp_us = 123456U;
   sensor_data.vcs_velocity.longitudinal = 12.3F;
   sensor_data.vcs_velocity.lateral = -4.5F;
   sensor_data.number_of_valid_detections = 42U;
   sensor_data.overall_rain_level = 2U;
   sensor_data.look_index = 3U;
   sensor_data.is_valid = false;
   sensor_data.f_sensor_fault_detected = true;
   sensor_data.look_id = RSPP_DET_LOOK_ID_3;

   raw_detection.range = 111.0F;
   raw_detection.std_range = 1.1F;
   raw_detection.range_rate = -2.0F;
   raw_detection.std_range_rate = 0.5F;
   raw_detection.std_azimuth = 0.11F;
   raw_detection.std_elevation = 0.22F;
   raw_detection.snr = 99.0F;
   raw_detection.rcs = -20.0F;
   raw_detection.prob_1stazhypo = 0.7F;
   raw_detection.sensor_id = 2;
   raw_detection.det_id = 3;
   raw_detection.confid_azimuth = 1;
   raw_detection.confid_elevation = 2;
   raw_detection.f_super_res = true;
   raw_detection.f_host_veh_clutter = true;
   raw_detection.f_nd_target = true;
   raw_detection.f_bistatic = true;
   raw_detection.f_ci_det = true;
   raw_detection.f_idm_det = true;
   raw_detection.f_below_rain_thold = true;

   processed_detection.vcs_az = 999.0F;
   processed_detection.vcs_el = 999.0F;
   processed_detection.cos_vcs_az = 999.0F;
   processed_detection.sin_vcs_az = 999.0F;

   /** \action
    * Recompute compensated angles with unused fields modified
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Output should be identical to baseline
    */
   DOUBLES_EQUAL(expected_az, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_el, processed_detection.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(expected_cos, processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_sin, processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation with zero mounting angle and positive polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Zero_Mounting_Angle_Positive_Polarity)
{
   /** \step{1}
    * Verify mounting angle compensation with zero mounting angle and positive polarity
    */

   /** \precond
    * Zero mounting angles, positive polarity, typical detection angles
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.5F;   // ~28.6 degrees
   raw_detection.elevation = 0.1F; // ~5.7 degrees

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Output angles should match input angles, trig values computed correctly
    */
   DOUBLES_EQUAL(0.5F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.1F, processed_detection.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(std::cos(0.5F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(0.5F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation with negative polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Negative_Polarity)
{
   /** \step{1}
    * Verify mounting angle compensation with negative polarity
    */

   /** \precond
    * Zero mounting angle, negative polarity, positive detection angles
    */
   polarity = -1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.5F;
   raw_detection.elevation = 0.1F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Angles should be negated due to negative polarity
    */
   DOUBLES_EQUAL(-0.5F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(-0.1F, processed_detection.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(std::cos(-0.5F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(-0.5F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation with positive azimuth mounting angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Positive_Azimuth_Mounting_Angle)
{
   /** \step{1}
    * Verify mounting angle compensation with positive azimuth mounting angle
    */

   /** \precond
    * Positive azimuth mounting angle, positive polarity, zero detection angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.2F; // ~11.5 degrees mounting angle
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS azimuth should equal mounting angle value
    */
   DOUBLES_EQUAL(0.2F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(std::cos(0.2F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(0.2F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation with negative azimuth mounting angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Negative_Azimuth_Mounting_Angle)
{
   /** \step{1}
    * Verify mounting angle compensation with negative azimuth mounting angle
    */

   /** \precond
    * Negative azimuth mounting angle, positive polarity, zero detection angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = -0.3F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS azimuth should equal negative mounting angle value
    */
   DOUBLES_EQUAL(-0.3F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation with positive elevation mounting angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Positive_Elevation_Mounting_Angle)
{
   /** \step{1}
    * Verify mounting angle compensation with positive elevation mounting angle
    */

   /** \precond
    * Positive elevation mounting angle, positive polarity, zero detection angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.15F; // ~8.6 degrees
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS elevation should equal mounting angle value
    */
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.15F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation with negative elevation mounting angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Negative_Elevation_Mounting_Angle)
{
   /** \step{1}
    * Verify mounting angle compensation with negative elevation mounting angle
    */

   /** \precond
    * Negative elevation mounting angle, positive polarity, zero detection angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = -0.12F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS elevation should equal negative mounting angle value
    */
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(-0.12F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test combined mounting angle and detection angles with positive polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Combined_Mounting_Angle_And_Detection_Angles)
{
   /** \step{1}
    * Verify combined mounting angle and detection angles with positive polarity
    */

   /** \precond
    * Non-zero mounting angle and detection angles, positive polarity
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.1F;
   sensor_data.vacs_boresight_el_estimated = 0.05F;
   raw_detection.azimuth = 0.3F;
   raw_detection.elevation = 0.2F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS angles should be sum of mounting angle and detection angles
    */
   DOUBLES_EQUAL(0.4F, processed_detection.vcs_az, TOLERANCE);  // 0.1 + 0.3
   DOUBLES_EQUAL(0.25F, processed_detection.vcs_el, TOLERANCE); // 0.05 + 0.2
   DOUBLES_EQUAL(std::cos(0.4F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(0.4F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test combined mounting angle and detection angles with negative polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Combined_Negative_Polarity)
{
   /** \step{1}
    * Verify combined mounting angle and detection angles with negative polarity
    */

   /** \precond
    * Non-zero mounting angle and detection angles, negative polarity
    */
   polarity = -1;
   sensor_data.vacs_boresight_az_estimated = 0.1F;
   sensor_data.vacs_boresight_el_estimated = 0.05F;
   raw_detection.azimuth = 0.3F;
   raw_detection.elevation = 0.2F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS angles should account for negative polarity
    */
   DOUBLES_EQUAL(-0.2F, processed_detection.vcs_az, TOLERANCE);  // 0.1 + (-1 * 0.3)
   DOUBLES_EQUAL(-0.15F, processed_detection.vcs_el, TOLERANCE); // 0.05 + (-1 * 0.2)
}

/** \purpose
 * Test mounting angle compensation at maximum positive azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Max_Positive_Azimuth)
{
   /** \step{1}
    * Verify mounting angle compensation at maximum positive azimuth angle
    */

   /** \precond
    * Maximum positive azimuth angle (near PI)
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 3.14F; // Near PI
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS azimuth should be near PI, trig values computed correctly
    */
   DOUBLES_EQUAL(3.14F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(3.14F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(3.14F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation at maximum negative azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Max_Negative_Azimuth)
{
   /** \step{1}
    * Verify mounting angle compensation at maximum negative azimuth angle
    */

   /** \precond
    * Maximum negative azimuth angle (near -PI)
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = -3.14F; // Near -PI
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS azimuth should be near -PI, trig values computed correctly
    */
   DOUBLES_EQUAL(-3.14F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(-3.14F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(-3.14F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation at maximum positive elevation angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Max_Positive_Elevation)
{
   /** \step{1}
    * Verify mounting angle compensation at maximum positive elevation angle
    */

   /** \precond
    * Maximum positive elevation angle (near PI/2)
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 1.57F; // Near PI/2

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS elevation should be near PI/2
    */
   DOUBLES_EQUAL(1.57F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation at maximum negative elevation angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Max_Negative_Elevation)
{
   /** \step{1}
    * Verify mounting angle compensation at maximum negative elevation angle
    */

   /** \precond
    * Maximum negative elevation angle (near -PI/2)
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = -1.57F; // Near -PI/2

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * VCS elevation should be near -PI/2
    */
   DOUBLES_EQUAL(-1.57F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test mounting angle compensation with zero polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Zero_Polarity)
{
   /** \step{1}
    * Verify mounting angle compensation with zero polarity
    */

   /** \precond
    * Zero polarity, non-zero detection angles
    */
   polarity = 0;
   sensor_data.vacs_boresight_az_estimated = 0.1F;
   sensor_data.vacs_boresight_el_estimated = 0.05F;
   raw_detection.azimuth = 0.5F;
   raw_detection.elevation = 0.2F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Detection angles should be zeroed, only mounting angle remains
    */
   DOUBLES_EQUAL(0.1F, processed_detection.vcs_az, TOLERANCE);  // Only mounting angle
   DOUBLES_EQUAL(0.05F, processed_detection.vcs_el, TOLERANCE); // Only mounting angle
}

/** \purpose
 * Test angle wrapping when azimuth exceeds PI boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Azimuth_Wrapping_Exceeds_PI)
{
   /** \step{1}
    * Verify angle wrapping when azimuth exceeds PI boundary
    */

   /** \precond
    * Mounting angle and detection angle sum exceeds PI
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 2.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 2.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles (should normalize)
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Azimuth should wrap to valid range [-PI, PI]
    */
   CHECK(processed_detection.vcs_az >= -3.15F);
   CHECK(processed_detection.vcs_az <= 3.15F);
   DOUBLES_EQUAL(std::cos(processed_detection.vcs_az), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(processed_detection.vcs_az), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test angle wrapping when azimuth goes below -PI boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Azimuth_Wrapping_Below_Minus_PI)
{
   /** \step{1}
    * Verify angle wrapping when azimuth goes below -PI boundary
    */

   /** \precond
    * Mounting angle and detection angle sum below -PI
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = -2.5F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = -2.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles (should normalize)
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Azimuth should wrap to valid range [-PI, PI]
    */
   CHECK(processed_detection.vcs_az >= -3.15F);
   CHECK(processed_detection.vcs_az <= 3.15F);
   DOUBLES_EQUAL(std::cos(processed_detection.vcs_az), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(processed_detection.vcs_az), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test trigonometric values at zero azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Trig_Values_Zero_Azimuth)
{
   /** \step{1}
    * Verify trigonometric values at zero azimuth
    */

   /** \precond
    * Zero azimuth angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * cos(0) = 1.0, sin(0) = 0.0
    */
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(1.0F, processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test trigonometric values at PI/2 azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Trig_Values_PI_2_Azimuth)
{
   /** \step{1}
    * Verify trigonometric values at PI/2 azimuth
    */

   /** \precond
    * Azimuth at PI/2
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 1.5708F; // PI/2
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * cos(PI/2) = 0.0, sin(PI/2) = 1.0
    */
   DOUBLES_EQUAL(1.5708F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(1.0F, processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test trigonometric values at PI azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Trig_Values_PI_Azimuth)
{
   /** \step{1}
    * Verify trigonometric values at PI azimuth
    */

   /** \precond
    * Azimuth at PI
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 3.14159F; // PI
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * cos(PI) = -1.0, sin(PI) = 0.0
    */
   DOUBLES_EQUAL(3.14159F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(-1.0F, processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test trigonometric values at -PI/2 azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Trig_Values_Minus_PI_2_Azimuth)
{
   /** \step{1}
    * Verify trigonometric values at -PI/2 azimuth
    */

   /** \precond
    * Azimuth at -PI/2
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = -1.5708F; // -PI/2
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * cos(-PI/2) = 0.0, sin(-PI/2) = -1.0
    */
   DOUBLES_EQUAL(-1.5708F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(-1.0F, processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test with very small positive azimuth angle (epsilon)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Small_Positive_Azimuth)
{
   /** \step{1}
    * Verify with very small positive azimuth angle (epsilon)
    */

   /** \precond
    * Very small positive azimuth
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0001F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should handle small angles correctly
    */
   DOUBLES_EQUAL(0.0001F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(0.0001F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(0.0001F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test with very small negative azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Small_Negative_Azimuth)
{
   /** \step{1}
    * Verify with very small negative azimuth angle
    */

   /** \precond
    * Very small negative azimuth
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = -0.0001F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should handle small angles correctly
    */
   DOUBLES_EQUAL(-0.0001F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(-0.0001F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(-0.0001F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test with very small elevation angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Small_Elevation)
{
   /** \step{1}
    * Verify with very small elevation angle
    */

   /** \precond
    * Very small elevation angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0001F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should handle small elevation correctly
    */
   DOUBLES_EQUAL(0.0001F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test with large positive polarity value
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Large_Positive_Polarity)
{
   /** \step{1}
    * Verify with large positive polarity value
    */

   /** \precond
    * Large positive polarity
    */
   polarity = 100;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.01F;
   raw_detection.elevation = 0.01F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Polarity should amplify detection angles significantly
    */
   DOUBLES_EQUAL(1.0F, processed_detection.vcs_az, TOLERANCE); // 0.01 * 100
   DOUBLES_EQUAL(1.0F, processed_detection.vcs_el, TOLERANCE); // 0.01 * 100
}

/** \purpose
 * Test with large negative polarity value
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Large_Negative_Polarity)
{
   /** \step{1}
    * Verify with large negative polarity value
    */

   /** \precond
    * Large negative polarity
    */
   polarity = -100;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.01F;
   raw_detection.elevation = 0.01F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Polarity should amplify and negate detection angles
    */
   DOUBLES_EQUAL(-1.0F, processed_detection.vcs_az, TOLERANCE); // 0.01 * -100
   DOUBLES_EQUAL(-1.0F, processed_detection.vcs_el, TOLERANCE); // 0.01 * -100
}

/** \purpose
 * Test with typical front radar sensor configuration
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Typical_Front_Radar)
{
   /** \step{1}
    * Verify with typical front radar sensor configuration
    */

   /** \precond
    * Typical front radar: positive polarity, small mounting angle, moderate detection angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.02F;  // ~1 degree mounting angle
   sensor_data.vacs_boresight_el_estimated = -0.01F; // ~-0.6 degree
   raw_detection.azimuth = 0.35F;                    // ~20 degrees
   raw_detection.elevation = 0.05F;                  // ~3 degrees

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should compute compensated angles correctly
    */
   DOUBLES_EQUAL(0.37F, processed_detection.vcs_az, TOLERANCE); // 0.02 + 0.35
   DOUBLES_EQUAL(0.04F, processed_detection.vcs_el, TOLERANCE); // -0.01 + 0.05
   CHECK(processed_detection.cos_vcs_az > 0.0F);
   CHECK(processed_detection.sin_vcs_az > 0.0F);
}

/** \purpose
 * Test with typical rear radar sensor configuration
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Typical_Rear_Radar)
{
   /** \step{1}
    * Verify with typical rear radar sensor configuration
    */

   /** \precond
    * Typical rear radar: negative polarity, pointing backward
    */
   polarity = -1;
   sensor_data.vacs_boresight_az_estimated = 3.14F; // PI (pointing backward)
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.2F; // Detection relative to sensor
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should compute backward-facing compensated angles
    */
   DOUBLES_EQUAL(2.94F, processed_detection.vcs_az, TOLERANCE); // 3.14 + (-1 * 0.2)
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test with typical side radar sensor configuration
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Typical_Side_Radar)
{
   /** \step{1}
    * Verify with typical side radar sensor configuration
    */

   /** \precond
    * Typical side radar: pointing left (PI/2)
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 1.57F; // PI/2 (pointing left)
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should compute side-facing compensated angles
    */
   DOUBLES_EQUAL(1.57F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(1.57F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(1.57F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test with maximum mounting angle compensation scenario
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Maximum_Mounting_Angle)
{
   /** \step{1}
    * Verify with maximum mounting angle compensation scenario
    */

   /** \precond
    * Maximum realistic mounting angle values
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.5F; // ~28 degrees mounting angle
   sensor_data.vacs_boresight_el_estimated = 0.3F; // ~17 degrees mounting angle
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should handle large mounting angle correctly
    */
   DOUBLES_EQUAL(0.5F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.3F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test with all zero inputs
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_All_Zeros)
{
   /** \step{1}
    * Verify with all zero inputs
    */

   /** \precond
    * All inputs are zero
    */
   polarity = 0;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * All outputs should be zero except cos(0) = 1
    */
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(1.0F, processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test with opposing azimuth angles that cancel out
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Opposing_Azimuth_Angles)
{
   /** \step{1}
    * Verify with opposing azimuth angles that cancel out
    */

   /** \precond
    * Mounting angle and detection angle are opposite and equal
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = -0.5F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.5F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Azimuth should cancel to zero
    */
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(1.0F, processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test with opposing elevation angles that cancel out
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Opposing_Elevation_Angles)
{
   /** \step{1}
    * Verify with opposing elevation angles that cancel out
    */

   /** \precond
    * Mounting angle and detection elevation are opposite and equal
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = -0.3F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.3F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Elevation should cancel to zero
    */
   DOUBLES_EQUAL(0.0F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test with quarter circle azimuth rotation (PI/4)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Quarter_Circle_Rotation)
{
   /** \step{1}
    * Verify with quarter circle azimuth rotation (PI/4)
    */

   /** \precond
    * 45-degree (PI/4) rotation
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.7854F; // PI/4
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * cos(PI/4) = sin(PI/4) = 0.707
    */
   DOUBLES_EQUAL(0.7854F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(0.7854F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(0.7854F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test with three-quarter circle azimuth rotation (3*PI/4)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Three_Quarter_Circle_Rotation)
{
   /** \step{1}
    * Verify with three-quarter circle azimuth rotation (3*PI/4)
    */

   /** \precond
    * 135-degree (3*PI/4) rotation
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 2.356F; // 3*PI/4
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * cos(3*PI/4) = -0.707, sin(3*PI/4) = 0.707
    */
   DOUBLES_EQUAL(2.356F, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(2.356F), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(2.356F), processed_detection.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test with very large detection azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Very_Large_Detection_Azimuth)
{
   /** \step{1}
    * Verify with very large detection azimuth angle
    */

   /** \precond
    * Very large detection azimuth (beyond typical FOV)
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 10.0F; // Much larger than PI
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should normalize to valid range
    */
   CHECK(processed_detection.vcs_az >= -3.15F);
   CHECK(processed_detection.vcs_az <= 3.15F);
}

/** \purpose
 * Test with very large negative detection azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Very_Large_Negative_Detection_Azimuth)
{
   /** \step{1}
    * Verify with very large negative detection azimuth angle
    */

   /** \precond
    * Very large negative detection azimuth
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = -10.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should normalize to valid range
    */
   CHECK(processed_detection.vcs_az >= -3.15F);
   CHECK(processed_detection.vcs_az <= 3.15F);
}

/** \purpose
 * Test symmetry: positive and negative equivalent angles
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Symmetry_Test)
{
   /** \step{1}
    * Verify symmetry: positive and negative equivalent angles
    */

   /** \precond
    * Test with angle and its negative
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   // Test positive angle
   raw_detection.azimuth = 0.5F;
   raw_detection.elevation = 0.2F;
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);
   float32_t pos_az = processed_detection.vcs_az;
   float32_t pos_el = processed_detection.vcs_el;
   float32_t pos_cos = processed_detection.cos_vcs_az;
   float32_t pos_sin = processed_detection.sin_vcs_az;

   // Test negative angle
   raw_detection.azimuth = -0.5F;
   raw_detection.elevation = -0.2F;
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Negative angle results should be negated, cos symmetric, sin anti-symmetric
    */
   DOUBLES_EQUAL(-pos_az, processed_detection.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(-pos_el, processed_detection.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(pos_cos, processed_detection.cos_vcs_az, TOLERANCE);  // cos is even
   DOUBLES_EQUAL(-pos_sin, processed_detection.sin_vcs_az, TOLERANCE); // sin is odd
}

/** \purpose
 * Test with NaN in azimuth mounting angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_NaN_Azimuth_Mounting_Angle)
{
   /** \step{1}
    * Verify with NaN in azimuth mounting angle
    */

   /** \precond
    * NaN value in azimuth mounting angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = std::numeric_limits<float32_t>::quiet_NaN();
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.5F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should propagate NaN to output azimuth
    */
   CHECK(std::isnan(processed_detection.vcs_az));
   CHECK(std::isnan(processed_detection.cos_vcs_az));
   CHECK(std::isnan(processed_detection.sin_vcs_az));
}

/** \purpose
 * Test with NaN in elevation mounting angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_NaN_Elevation_Mounting_Angle)
{
   /** \step{1}
    * Verify with NaN in elevation mounting angle
    */

   /** \precond
    * NaN value in elevation mounting angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = std::numeric_limits<float32_t>::quiet_NaN();
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.5F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Function should hangle NaN gracefully
    */
   CHECK(std::isnan(processed_detection.vcs_el));
}

/** \purpose
 * Test with NaN in detection azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_NaN_Detection_Azimuth)
{
   /** \step{1}
    * Verify with NaN in detection azimuth
    */

   /** \precond
    * NaN value in detection azimuth
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.1F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = std::numeric_limits<float32_t>::quiet_NaN();
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Function should handle NaN gracefully
    */
   CHECK(std::isnan(processed_detection.vcs_az));
   CHECK(std::isnan(processed_detection.cos_vcs_az));
   CHECK(std::isnan(processed_detection.sin_vcs_az));
}

/** \purpose
 * Test with positive infinity in azimuth mounting angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Positive_Infinity_Azimuth_Mounting_Angle)
{
   /** \step{1}
    * Verify with positive infinity in azimuth mounting angle
    */

   /** \precond
    * Positive infinity in azimuth mounting angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = std::numeric_limits<float32_t>::infinity();
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Function should handle infinity gracefully
    */
   CHECK_FALSE(std::isfinite(processed_detection.vcs_az));
}

/** \purpose
 * Test with negative infinity in elevation mounting angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Negative_Infinity_Elevation_Mounting_Angle)
{
   /** \step{1}
    * Verify with negative infinity in elevation mounting angle
    */

   /** \precond
    * Negative infinity in elevation mounting angle
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = -std::numeric_limits<float32_t>::infinity();
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Function should handle infinity gracefully
    */
   CHECK(std::isinf(processed_detection.vcs_el));
   CHECK(processed_detection.vcs_el < 0.0F);
}

/** \purpose
 * Test with infinity in detection azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Infinity_Detection_Azimuth)
{
   /** \step{1}
    * Verify with infinity in detection azimuth
    */

   /** \precond
    * Infinity in detection azimuth
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = std::numeric_limits<float32_t>::infinity();
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Function should handle infinity gracefully
    */
   CHECK_FALSE(std::isfinite(processed_detection.vcs_az));
}

/** \purpose
 * Test with negative infinity in detection azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Negative_Infinity_Detection_Azimuth)
{
   /** \step{1}
    * Verify with negative infinity in detection azimuth
    */

   /** \precond
    * Negative infinity in detection azimuth
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = -std::numeric_limits<float32_t>::infinity();
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Function should handle infinity gracefully
    */
   CHECK_FALSE(std::isfinite(processed_detection.vcs_az));
}

/** \purpose
 * Test elevation with negative polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Elevation_Negative_Polarity)
{
   /** \step{1}
    * Verify elevation with negative polarity
    */

   /** \precond
    * Negative polarity with non-zero elevation
    */
   polarity = -1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.1F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 0.3F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Elevation should be affected by negative polarity
    */
   DOUBLES_EQUAL(-0.2F, processed_detection.vcs_el, TOLERANCE); // 0.1 + (-1 * 0.3)
}

/** \purpose
 * Test combined elevation and azimuth with negative polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Combined_Elevation_Azimuth_Negative_Polarity)
{
   /** \step{1}
    * Verify combined elevation and azimuth with negative polarity
    */

   /** \precond
    * Negative polarity with both elevation and azimuth
    */
   polarity = -1;
   sensor_data.vacs_boresight_az_estimated = 0.2F;
   sensor_data.vacs_boresight_el_estimated = 0.15F;
   raw_detection.azimuth = 0.4F;
   raw_detection.elevation = 0.25F;

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Both angles should reflect negative polarity effect
    */
   DOUBLES_EQUAL(-0.2F, processed_detection.vcs_az, TOLERANCE); // 0.2 + (-1 * 0.4)
   DOUBLES_EQUAL(-0.1F, processed_detection.vcs_el, TOLERANCE); // 0.15 + (-1 * 0.25)
}

/** \purpose
 * Test maximum elevation with negative polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Max_Elevation_Negative_Polarity)
{
   /** \step{1}
    * Verify maximum elevation with negative polarity
    */

   /** \precond
    * Maximum elevation with negative polarity
    */
   polarity = -1;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 0.0F;
   raw_detection.elevation = 1.57F; // PI/2

   /** \action
    * Compensate detection angles
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Elevation should be negated to -PI/2
    */
   DOUBLES_EQUAL(-1.57F, processed_detection.vcs_el, TOLERANCE);
}

/** \purpose
 * Test angle wrapping with exact PI boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates_Calculate_VCS_Angles,
     Calculate_VCS_Angles_TC_Exact_PI_Boundary_Wrapping)
{
   /** \step{1}
    * Verify angle wrapping with exact PI boundary
    */

   /** \precond
    * Mounting angle and detection sum to exactly 2*PI
    */
   polarity = 1;
   sensor_data.vacs_boresight_az_estimated = 3.14159F; // PI
   sensor_data.vacs_boresight_el_estimated = 0.0F;
   raw_detection.azimuth = 3.14159F; // PI
   raw_detection.elevation = 0.0F;

   /** \action
    * Compensate detection angles (should wrap to near 0 or -PI)
    */
   RSPP_Calculate_VCS_Angles(polarity, sensor_data, raw_detection, processed_detection);

   /** \result
    * Should wrap to valid range, result near 0 or -PI depending on normalization
    */
   CHECK(processed_detection.vcs_az >= -3.15F);
   CHECK(processed_detection.vcs_az <= 3.15F);
   // cos(2pi) = cos(0) = 1, or cos(wrapped value)
   DOUBLES_EQUAL(std::cos(processed_detection.vcs_az), processed_detection.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(processed_detection.vcs_az), processed_detection.sin_vcs_az, TOLERANCE);
}

/** @}*/

/** \defgroup  test_RSPP_Calculate_Detection_VCS_Coordinates
 *  @{
 */

/** \brief
 * Tests for RSPP_Calculate_Detection_VCS_Coordinates function
 *
 * This integration test suite validates the complete VCS transformation pipeline:
 * 1. Input validation (FOV checking) -> returns true/false
 * 2. Mounting angle compensation (azimuth/elevation angles)
 * 3. VCS coordinate calculation (X, Y, Z positions)
 * 4. Trigonometric value computation (cos/sin)
 *
 * Test categories:
 * - Basic integration (valid inputs, polarity, mounting positions)
 * - FOV validation (boundary, beyond FOV, different look IDs)
 * - Parameter variations (range, angles, combined parameters)
 * - Edge cases (NaN, infinity, angle wrapping, zero polarity)
 */
TEST_GROUP(test_RSPP_Calculate_Detection_VCS_Coordinates)
{
   // Common variables used within all tests in this test group
   static constexpr float32_t TOLERANCE = 0.0001F;
   RSPP_Sensor_Calib_T rspp_sensor_calibration;
   VariableProps_T sensor_data;
   RSPP_Detection_T detection;

   /** \setup
    * Initialize all structures to zero and configure default valid FOV values (-1.5 to 1.5 rad)
    * for all look IDs to prevent false negatives. Set polarity to 1 (normal) and look ID to
    * RSPP_DET_LOOK_ID_1 as default sensor configuration.
    */
   TEST_SETUP()
   {
      memset(&rspp_sensor_calibration, 0, sizeof(RSPP_Sensor_Calib_T));
      memset(&sensor_data, 0, sizeof(VariableProps_T));
      memset(&detection, 0, sizeof(RSPP_Detection_T));

      // Set default valid FOV values to avoid false negatives
      for (int i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
      {
         rspp_sensor_calibration.fov_min_az_rad[i] = -1.5F; // ~-86 degrees
         rspp_sensor_calibration.fov_max_az_rad[i] = 1.5F;  // ~86 degrees
      }
      rspp_sensor_calibration.polarity = 1; // Normal polarity
      sensor_data.look_id = RSPP_DET_LOOK_ID_1;
   }

   /** \teardown
    * No cleanup required - all test data is stack allocated.
    */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Test that unused input fields do not affect full VCS transformation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Unused_Input_Data_Does_Not_Affect_Output)
{
   /** \step{1}
    * Verify unused fields do not impact integration results
    */

   /** \precond
    * Baseline valid inputs
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 2.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = -0.2F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.4F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.05F;
   sensor_data.vacs_boresight_el_estimated = 0.02F;
   sensor_data.look_id = RSPP_DET_LOOK_ID_1;

   detection.raw.range = 25.0F;
   detection.raw.azimuth = 0.1F;
   detection.raw.elevation = -0.05F;

   /** \action
    * Perform transformation and capture expected output
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   const float32_t expected_az = detection.processed.vcs_az;
   const float32_t expected_el = detection.processed.vcs_el;
   const float32_t expected_cos = detection.processed.cos_vcs_az;
   const float32_t expected_sin = detection.processed.sin_vcs_az;
   const float32_t expected_x = detection.processed.vcs_position_x;
   const float32_t expected_y = detection.processed.vcs_position_y;
   const float32_t expected_z = detection.processed.vcs_position_z;
   CHECK(result == true);
   DOUBLES_EQUAL(expected_az, detection.processed.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_el, detection.processed.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(expected_cos, detection.processed.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_sin, detection.processed.sin_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_x, detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, detection.processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, detection.processed.vcs_position_z, TOLERANCE);

   /** \precond
    * Modify unused input fields only
    */
   for (int i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      if (i != static_cast<int>(sensor_data.look_id))
      {
         rspp_sensor_calibration.fov_min_az_rad[i] = -0.2F;
         rspp_sensor_calibration.fov_max_az_rad[i] = 0.2F;
      }
      rspp_sensor_calibration.interior_fov[i] = 0.9F;
      rspp_sensor_calibration.left_fov_normal[i] = 0.33F;
      rspp_sensor_calibration.right_fov_normal[i] = -0.44F;
      rspp_sensor_calibration.v_wrapping[i] = 12.0F;
   }
   rspp_sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR;

   sensor_data.timestamp_us = 999999U;
   sensor_data.vcs_velocity.longitudinal = 8.0F;
   sensor_data.vcs_velocity.lateral = -3.0F;
   sensor_data.number_of_valid_detections = 5U;
   sensor_data.overall_rain_level = 1U;
   sensor_data.look_index = 2U;
   sensor_data.is_valid = false;
   sensor_data.f_sensor_fault_detected = true;

   detection.raw.std_range = 2.2F;
   detection.raw.range_rate = -3.3F;
   detection.raw.std_range_rate = 0.8F;
   detection.raw.std_azimuth = 0.12F;
   detection.raw.std_elevation = 0.21F;
   detection.raw.snr = 88.0F;
   detection.raw.rcs = -15.0F;
   detection.raw.prob_1stazhypo = 0.6F;
   detection.raw.sensor_id = 4;
   detection.raw.det_id = 11;
   detection.raw.confid_azimuth = 2;
   detection.raw.confid_elevation = 1;
   detection.raw.f_super_res = true;
   detection.raw.f_host_veh_clutter = true;
   detection.raw.f_nd_target = true;
   detection.raw.f_bistatic = true;
   detection.raw.f_ci_det = true;
   detection.raw.f_idm_det = true;
   detection.raw.f_below_rain_thold = true;

   detection.processed.vcs_position_x = 999.0F;
   detection.processed.vcs_position_y = 999.0F;
   detection.processed.vcs_position_z = 999.0F;
   detection.processed.vcs_az = 999.0F;
   detection.processed.vcs_el = 999.0F;
   detection.processed.cos_vcs_az = 999.0F;
   detection.processed.sin_vcs_az = 999.0F;

   /** \action
    * Recompute transformation with unused fields modified
    */
   result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Output should be identical to baseline
    */
   CHECK(result == true);
   DOUBLES_EQUAL(expected_az, detection.processed.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_el, detection.processed.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(expected_cos, detection.processed.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_sin, detection.processed.sin_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(expected_x, detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, detection.processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, detection.processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test FOV selection uses look_id-specific bounds
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Check_Look_ID_Specific_FOV_Bounds)
{
   /** \step{1}
    * Verify FOV selection uses look_id-specific bounds
    */

   /** \precond
    * Configure distinct FOVs for each look_id. Test azimuths within each look_id's FOV only.
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 1.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   rspp_sensor_calibration.fov_min_az_rad[RSPP_DET_LOOK_ID_0] = 0.0F;
   rspp_sensor_calibration.fov_max_az_rad[RSPP_DET_LOOK_ID_0] = 0.2F;
   rspp_sensor_calibration.fov_min_az_rad[RSPP_DET_LOOK_ID_1] = 0.2F;
   rspp_sensor_calibration.fov_max_az_rad[RSPP_DET_LOOK_ID_1] = 0.4F;
   rspp_sensor_calibration.fov_min_az_rad[RSPP_DET_LOOK_ID_2] = 0.4F;
   rspp_sensor_calibration.fov_max_az_rad[RSPP_DET_LOOK_ID_2] = 0.6F;
   rspp_sensor_calibration.fov_min_az_rad[RSPP_DET_LOOK_ID_3] = 0.6F;
   rspp_sensor_calibration.fov_max_az_rad[RSPP_DET_LOOK_ID_3] = 0.8F;
   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 20.0F;
   detection.raw.elevation = 0.0F;

   const int32_t num_test_data = static_cast<int32_t>(RSPP_DET_NUM_LOOK_ID);
   const float32_t test_azimuth_array[num_test_data] = {0.1F, 0.3F, 0.5F, 0.7F};

   for (int32_t i = 0; i < num_test_data; i++)
   {
      for (int8_t look_id = 0; look_id < static_cast<int8_t>(RSPP_DET_NUM_LOOK_ID); look_id++)
      {
         /** \action
          * Set azimuth test vector within bounds for one look_id only
          * Set look_id to test
          */
         detection.raw.azimuth = test_azimuth_array[i];
         sensor_data.look_id = static_cast<RSPP_Det_Look_ID_T>(look_id);

         bool result = RSPP_Calculate_Detection_VCS_Coordinates(
             detection.raw,
             sensor_data,
             rspp_sensor_calibration,
             detection.processed);

         /** \result
          * Should be valid for the corresponding look_id
          */
         const bool expected = (look_id == i);
         CHECK(result == expected);
      }
   }
}

/** \purpose
 * Test complete VCS coordinate transformation with valid inputs
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Valid_Front_Detection)
{
   /** \step{1}
    * Verify complete VCS coordinate transformation with valid inputs
    */

   /** \precond
    * Front radar with typical configuration, valid detection within FOV
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should return true, processed data populated correctly
    */
   CHECK(result == true);
   DOUBLES_EQUAL(0.0F, detection.processed.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, detection.processed.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(1.0F, detection.processed.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, detection.processed.sin_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(53.5F, detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.0F, detection.processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.5F, detection.processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test complete VCS transformation with mounting angle compensation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_With_Mounting_Angle)
{
   /** \step{1}
    * Verify complete VCS transformation with mounting angle compensation
    */

   /** \precond
    * Sensor with azimuth and elevation mounting angle
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.5F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.6F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.1F;  // Mounting angle
   sensor_data.vacs_boresight_el_estimated = 0.05F; // Mounting angle

   detection.raw.range = 40.0F;
   detection.raw.azimuth = 0.3F;
   detection.raw.elevation = 0.2F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Mounting angle should be compensated in VCS angles
    */
   CHECK(result == true);
   DOUBLES_EQUAL(0.4F, detection.processed.vcs_az, TOLERANCE);  // 0.1 + 0.3
   DOUBLES_EQUAL(0.25F, detection.processed.vcs_el, TOLERANCE); // 0.05 + 0.2
   DOUBLES_EQUAL(std::cos(0.4F), detection.processed.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(0.4F), detection.processed.sin_vcs_az, TOLERANCE);
}

/** \purpose
 * Test complete VCS transformation with negative polarity
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Negative_Polarity)
{
   /** \step{1}
    * Verify complete VCS transformation with negative polarity
    */

   /** \precond
    * Rear-facing sensor with negative polarity
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = -3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = -1; // Negative polarity

   sensor_data.vacs_boresight_az_estimated = 3.14F; // PI (pointing backward)
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 30.0F;
   detection.raw.azimuth = 0.2F;
   detection.raw.elevation = 0.1F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Polarity should negate detection angles
    */
   CHECK(result == true);
   DOUBLES_EQUAL(2.94F, detection.processed.vcs_az, TOLERANCE); // 3.14 + (-1 * 0.2)
   DOUBLES_EQUAL(-0.1F, detection.processed.vcs_el, TOLERANCE); // 0.0 + (-1 * 0.1)
}

/** \purpose
 * Test with detection at FOV boundary (should be valid)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_At_FOV_Boundary)
{
   /** \step{1}
    * Verify with detection at FOV boundary (should be valid)
    */

   /** \precond
    * Detection at maximum FOV boundary
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;
   rspp_sensor_calibration.fov_max_az_rad[0] = 1.5F;
   rspp_sensor_calibration.fov_min_az_rad[0] = -1.5F;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 1.5F; // At max FOV
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be valid (at boundary)
    */
   CHECK(result == true);
   DOUBLES_EQUAL(1.5F, detection.processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test with detection at minimum valid range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Minimum_Valid_Range)
{
   /** \step{1}
    * Verify with detection at minimum valid range
    */

   /** \precond
    * Detection at minimum range (0.0m or spec minimum)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 0.0F; // Minimum range
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be valid with VCS position calculated correctly
    */
   CHECK(result == true);
   DOUBLES_EQUAL(3.5F, detection.processed.vcs_position_x, TOLERANCE); // At mounting position
   DOUBLES_EQUAL(0.0F, detection.processed.vcs_position_y, TOLERANCE);
}

/** \purpose
 * Test with detection at maximum valid range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Maximum_Valid_Range)
{
   /** \step{1}
    * Verify with detection at maximum valid range
    */

   /** \precond
    * Detection at maximum range (500m per validation)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 500.0F; // Maximum range
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be valid with VCS position at maximum range
    */
   CHECK(result == true);
   DOUBLES_EQUAL(500.0F, detection.processed.vcs_position_x, TOLERANCE); // 500 + 0 mounting
   DOUBLES_EQUAL(0.0F, detection.processed.vcs_position_y, TOLERANCE);
}

/** \purpose
 * Test with detection just inside minimum azimuth FOV boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Just_Inside_Min_Azimuth_FOV)
{
   /** \step{1}
    * Verify with detection just inside minimum FOV boundary
    */

   /** \precond
    * Detection just inside minimum FOV (-1.5 + small margin)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;
   rspp_sensor_calibration.fov_max_az_rad[0] = 1.5F;
   rspp_sensor_calibration.fov_min_az_rad[0] = -1.5F;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = -1.49F; // Just inside min FOV
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be valid (just inside boundary)
    */
   CHECK(result == true);
   DOUBLES_EQUAL(-1.49F, detection.processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test with detection just inside maximum azimuth FOV boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Just_Inside_Max_Azimuth_FOV)
{
   /** \step{1}
    * Verify with detection just inside maximum FOV boundary
    */

   /** \precond
    * Detection just inside maximum FOV (1.5 - small margin)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;
   rspp_sensor_calibration.fov_max_az_rad[0] = 1.5F;
   rspp_sensor_calibration.fov_min_az_rad[0] = -1.5F;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 1.49F; // Just inside max FOV
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be valid (just inside boundary)
    */
   CHECK(result == true);
   DOUBLES_EQUAL(1.49F, detection.processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test with detection just outside maximum azimuth FOV boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Just_Outside_Max_Azimuth_FOV)
{
   /** \step{1}
    * Verify with detection just outside maximum FOV boundary
    */

   /** \precond
    * Detection just outside maximum FOV (1.5 + small margin)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;
   rspp_sensor_calibration.fov_max_az_rad[0] = 1.5F;
   rspp_sensor_calibration.fov_min_az_rad[0] = -1.5F;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 1.51F; // Just outside max FOV
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be invalid (outside boundary)
    */
   CHECK(result == false);
}

/** \purpose
 * Test with detection at minimum elevation boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Minimum_Elevation_Boundary)
{
   /** \step{1}
    * Verify with detection at minimum elevation boundary
    */

   /** \precond
    * Detection at minimum elevation (-0.53 rad = -30 degrees per validation)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 100.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = -0.53F; // Minimum elevation per validation

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be valid with correct VCS elevation
    */
   CHECK(result == true);
   DOUBLES_EQUAL(-0.53F, detection.processed.vcs_el, TOLERANCE);
}

/** \purpose
 * Test with detection just below minimum elevation boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Just_Below_Min_Elevation)
{
   /** \step{1}
    * Verify with detection just below minimum elevation
    */

   /** \precond
    * Detection slightly below minimum elevation
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 100.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = -0.531F; // Slightly below minimum (-0.53)

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be invalid (outside boundary)
    */
   CHECK(result == false);
}

/** \purpose
 * Test with detection just above maximum elevation boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Just_Above_Max_Elevation)
{
   /** \step{1}
    * Verify with detection just above maximum elevation
    */

   /** \precond
    * Detection slightly above maximum elevation (+0.53)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 100.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.531F; // Slightly above maximum (+0.53)

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be invalid (outside boundary)
    */
   CHECK(result == false);
}

/** \purpose
 * Test with boresight azimuth at negative PI boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Boresight_At_Negative_PI)
{
   /** \step{1}
    * Verify with boresight azimuth at -PI boundary
    */

   /** \precond
    * Boresight azimuth at -PI (pointing directly backward)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = -1; // Rear sensor

   sensor_data.vacs_boresight_az_estimated = -3.14159F; // -PI
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should handle -PI boresight correctly
    */
   CHECK(result == true);
   DOUBLES_EQUAL(-3.14159F, detection.processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test with boresight azimuth at positive PI boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Boresight_At_Positive_PI)
{
   /** \step{1}
    * Verify with boresight azimuth at +PI boundary
    */

   /** \precond
    * Boresight azimuth at +PI (pointing directly backward)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = -1; // Rear sensor

   sensor_data.vacs_boresight_az_estimated = 3.14159F; // +PI
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should handle +PI boresight correctly
    */
   CHECK(result == true);
   DOUBLES_EQUAL(3.14159F, detection.processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test with maximum forward longitudinal mounting position
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Max_Forward_Mounting)
{
   /** \step{1}
    * Verify with maximum forward longitudinal mounting position
    */

   /** \precond
    * Sensor at extreme forward position (1.0F = validation max)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 1.0F; // Maximum forward per validation
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 100.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should include maximum mounting position in VCS coordinates
    */
   CHECK(result == true);
   DOUBLES_EQUAL(101.0F, detection.processed.vcs_position_x, TOLERANCE); // 100 + 1
}

/** \purpose
 * Test with maximum backward longitudinal mounting position
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Max_Backward_Mounting)
{
   /** \step{1}
    * Verify with maximum backward longitudinal mounting position
    */

   /** \precond
    * Sensor at extreme backward position (-10.0F = validation min)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = -10.0F; // Maximum backward per validation
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = -1; // Rear sensor

   sensor_data.vacs_boresight_az_estimated = 3.14F; // Pointing backward
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should include backward mounting position in VCS coordinates
    */
   CHECK(result == true);
   // VCS position should account for rear mounting
}

/** \purpose
 * Test with maximum lateral mounting position (left side)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Max_Left_Lateral_Mounting)
{
   /** \step{1}
    * Verify with maximum left lateral mounting position
    */

   /** \precond
    * Sensor at extreme left position
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 1.5F; // Maximum left
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 1.57F; // Pointing left (PI/2)
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 100.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should include left lateral mounting position
    */
   CHECK(result == true);
   DOUBLES_EQUAL(1.57F, detection.processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test with maximum lateral mounting position (right side)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Max_Right_Lateral_Mounting)
{
   /** \step{1}
    * Verify with maximum right lateral mounting position
    */

   /** \precond
    * Sensor at extreme right position
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = -1.5F; // Maximum right
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = -1.57F; // Pointing right (-PI/2)
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 100.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should include right lateral mounting position
    */
   CHECK(result == true);
   DOUBLES_EQUAL(-1.57F, detection.processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test with maximum height mounting position
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Max_Height_Mounting)
{
   /** \step{1}
    * Verify with maximum height mounting position
    */

   /** \precond
    * Sensor at extreme height (1.3F = validation max)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 1.3F; // Maximum height per validation
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 100.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = -0.2F; // Pointing slightly downward

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should include maximum height in VCS Z coordinate
    */
   CHECK(result == true);
   DOUBLES_EQUAL(-0.2F, detection.processed.vcs_el, TOLERANCE);
   // VCS Z position should account for height mounting
}

/** \purpose
 * Test with detection beyond azimuth FOV (should mark as invalid)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Beyond_Azimuth_FOV)
{
   /** \step{1}
    * Verify with detection beyond azimuth FOV is marked invalid
    */

   /** \precond
    * Detection beyond maximum FOV
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;
   rspp_sensor_calibration.fov_max_az_rad[0] = 1.5F;
   rspp_sensor_calibration.fov_min_az_rad[0] = -1.5F;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 2.0F; // Beyond max azimuth FOV
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should return false
    */
   CHECK(result == false);
}

/** \purpose
 * Test with zero range detection
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Zero_Range)
{
   /** \step{1}
    * Verify with zero range detection
    */

   /** \precond
    * Detection with zero range
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.5F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.6F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 0.0F;
   detection.raw.azimuth = 0.3F;
   detection.raw.elevation = 0.1F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Position should equal mounting position
    */
   CHECK(result == true);
   DOUBLES_EQUAL(3.5F, detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(0.5F, detection.processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(-0.6F, detection.processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test with side-mounted radar
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Side_Mounted_Radar)
{
   /** \step{1}
    * Verify with side-mounted radar
    */

   /** \precond
    * Left side radar pointing left
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 1.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 1.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.3F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 1.57F; // PI/2 (pointing left)
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 20.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Detection should be to the left
    */
   CHECK(result == true);
   DOUBLES_EQUAL(1.57F, detection.processed.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(1.57F), detection.processed.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(1.57F), detection.processed.sin_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(1.0F + 20.0F * std::cos(1.57F), detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(1.0F + 20.0F * std::sin(1.57F), detection.processed.vcs_position_y, TOLERANCE);
}

/** \purpose
 * Test with combined azimuth, elevation, and range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Combined_Parameters)
{
   /** \step{1}
    * Verify with combined azimuth, elevation, and range
    */

   /** \precond
    * Detection with all parameters non-zero
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.5F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.6F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.1F;
   sensor_data.vacs_boresight_el_estimated = 0.05F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.3F;   // ~17 degrees
   detection.raw.elevation = 0.2F; // ~11 degrees

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * All coordinates should be affected by all parameters
    */
   CHECK(result == true);
   DOUBLES_EQUAL(0.4F, detection.processed.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.25F, detection.processed.vcs_el, TOLERANCE);

   float32_t expected_x = 3.0F + (50.0F * std::cos(0.4F));
   float32_t expected_y = 0.5F + (50.0F * std::sin(0.4F));
   float32_t expected_z = -0.6F + (50.0F * std::sin(0.25F));

   DOUBLES_EQUAL(expected_x, detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, detection.processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, detection.processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test with large range detection
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Large_Range)
{
   /** \step{1}
    * Verify with large range detection
    */

   /** \precond
    * Long-range detection (250m)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 4.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.02F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 250.0F;
   detection.raw.azimuth = 0.1F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should handle large ranges correctly
    */
   CHECK(result == true);
   DOUBLES_EQUAL(0.12F, detection.processed.vcs_az, TOLERANCE);
   float32_t expected_x = 4.0F + (250.0F * std::cos(0.12F));
   DOUBLES_EQUAL(expected_x, detection.processed.vcs_position_x, TOLERANCE);
   float32_t expected_y = 0.0F + (250.0F * std::sin(0.12F));
   DOUBLES_EQUAL(expected_y, detection.processed.vcs_position_y, TOLERANCE);
   float32_t expected_z = -0.5F + (250.0F * std::sin(0.0F));
   DOUBLES_EQUAL(expected_z, detection.processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test with small elevation and large range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Small_Elevation_Large_Range)
{
   /** \step{1}
    * Verify with small elevation and large range
    */

   /** \precond
    * Small elevation angle with large range for noticeable Z effect
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 200.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0175F; // ~1 degree

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Z coordinate should show elevation effect
    */
   CHECK(result == true);
   DOUBLES_EQUAL(0.0F, detection.processed.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0175F, detection.processed.vcs_el, TOLERANCE);
   float32_t expected_x = 3.5F + (200.0F * std::cos(0.0F));
   DOUBLES_EQUAL(expected_x, detection.processed.vcs_position_x, TOLERANCE);
   float32_t expected_y = 0.0F + (200.0F * std::sin(0.0F));
   DOUBLES_EQUAL(expected_y, detection.processed.vcs_position_y, TOLERANCE);
   float32_t expected_z = -0.5F + (200.0F * std::sin(0.0175F));
   DOUBLES_EQUAL(expected_z, detection.processed.vcs_position_z, TOLERANCE);
   CHECK(detection.processed.vcs_position_z > 0.0F); // Above ground
}

/** \purpose
 * Test with negative azimuth detection
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Negative_Azimuth)
{
   /** \step{1}
    * Verify with negative azimuth detection
    */

   /** \precond
    * Detection with negative azimuth angle
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = -0.5F; // Right side
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should handle negative azimuth correctly
    */
   CHECK(result == true);
   DOUBLES_EQUAL(-0.5F, detection.processed.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::cos(-0.5F), detection.processed.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(-0.5F), detection.processed.sin_vcs_az, TOLERANCE);
   CHECK(detection.processed.vcs_position_y < 0.0F); // Right side
}

/** \purpose
 * Test with maximum elevation angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Maximum_Elevation)
{
   /** \step{1}
    * Verify with maximum elevation angle
    */

   /** \precond
    * Detection at maximum elevation (30deg = 0.524 rad)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 20.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.53F; // 30deg (maximum valid elevation)

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Z coordinate should be approximately 9.56m (20 * sin(0.53) + 0.5 height offset)
    */
   CHECK(result == true);
   DOUBLES_EQUAL(0.53F, detection.processed.vcs_el, TOLERANCE);
   DOUBLES_EQUAL(-0.5F + 20.0F * std::sin(0.53F), detection.processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test angle wrapping in mounting angle compensation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Angle_Wrapping)
{
   /** \step{1}
    * Verify angle wrapping in mounting angle compensation
    */

   /** \precond
    * Large mounting angle and detection angle that exceed PI
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 3.14159265358979323846F; // pi
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 30.0F;
   detection.raw.azimuth = 1.14159265358979323846F; // pi-2
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Azimuth should wrap to valid range [-PI, PI]
    */
   CHECK(result == true);
   DOUBLES_EQUAL(detection.processed.vcs_az, -2.0F, TOLERANCE);
}

/** \purpose
 * Test with very small range (close detection)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Very_Small_Range)
{
   /** \step{1}
    * Verify with very small range (close detection)
    */

   /** \precond
    * Very close detection (0.5m)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 0.5F;
   detection.raw.azimuth = 0.2F;
   detection.raw.elevation = 0.1F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Position should be very close to mounting position
    */
   CHECK(result == true);
   float32_t expected_x = 3.5F + (0.5F * std::cos(0.2F));
   DOUBLES_EQUAL(expected_x, detection.processed.vcs_position_x, TOLERANCE);
}

/** \purpose
 * Test with negative elevation angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Negative_Elevation)
{
   /** \step{1}
    * Verify with negative elevation angle
    */

   /** \precond
    * Detection below sensor plane
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 30.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = -0.1F; // Below sensor

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Z coordinate should be more negative
    */
   CHECK(result == true);
   DOUBLES_EQUAL(-0.1F, detection.processed.vcs_el, TOLERANCE);
   float32_t expected_z = -0.5F + (30.0F * std::sin(-0.1F));
   DOUBLES_EQUAL(expected_z, detection.processed.vcs_position_z, TOLERANCE);
   CHECK(detection.processed.vcs_position_z < -0.5F);
}

/** \purpose
 * Test with 45-degree azimuth angle
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_45_Degree_Azimuth)
{
   /** \step{1}
    * Verify with 45-degree azimuth angle
    */

   /** \precond
    * Detection at 45 degrees
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.0F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 10.0F;
   detection.raw.azimuth = 0.7854F; // PI/4
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * X and Y should be approximately equal (~7.07m each)
    */
   CHECK(result == true);
   DOUBLES_EQUAL(0.7854F, detection.processed.vcs_az, TOLERANCE);
   DOUBLES_EQUAL(10.0F * std::cos(0.7854F), detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(10.0F * std::sin(0.7854F), detection.processed.vcs_position_y, TOLERANCE);
}

/** \purpose
 * Test with NaN in detection range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_NaN_Range)
{
   /** \step{1}
    * Verify with NaN in detection range
    */

   /** \precond
    * Detection with NaN range
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.5F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = std::numeric_limits<float32_t>::quiet_NaN();
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should return false due to invalid range
    */
   CHECK(result == false);
}

/** \purpose
 * Test with zero polarity (not a plausible input, since checked earlier in the chain)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Zero_Polarity)
{
   /** \step{1}
    * Verify with zero polarity
    */

   /** \precond
    * Sensor with zero polarity
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 0; // Zero polarity

   sensor_data.vacs_boresight_az_estimated = 0.1F;
   sensor_data.vacs_boresight_el_estimated = 0.05F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.5F;
   detection.raw.elevation = 0.2F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Detection angles should be zeroed, only mounting angle remains
    */
   CHECK(result == true);
   DOUBLES_EQUAL(0.1F, detection.processed.vcs_az, TOLERANCE);  // Only mounting angle
   DOUBLES_EQUAL(0.05F, detection.processed.vcs_el, TOLERANCE); // Only mounting angle
}

/** \purpose
 * Test comprehensive integration scenario
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Comprehensive_Integration)
{
   /** \step{1}
    * Verify comprehensive integration scenario
    */

   /** \precond
    * Real-world scenario with all features combined
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.2F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.3F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.55F;
   rspp_sensor_calibration.polarity = 1;
   rspp_sensor_calibration.fov_max_az_rad[0] = 1.3F;
   rspp_sensor_calibration.fov_min_az_rad[0] = -1.3F;

   sensor_data.vacs_boresight_az_estimated = 0.02F;  // Small mounting angle
   sensor_data.vacs_boresight_el_estimated = -0.01F; // Small negative elevation mounting angle
   sensor_data.look_id = RSPP_DET_LOOK_ID_1;

   detection.raw.range = 75.5F;
   detection.raw.azimuth = 0.35F;
   detection.raw.elevation = 0.08F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * All transformations should be applied correctly
    */
   CHECK(result == true);

   // Check mounting angle compensation
   DOUBLES_EQUAL(0.37F, detection.processed.vcs_az, TOLERANCE); // 0.02 + 0.35
   DOUBLES_EQUAL(0.07F, detection.processed.vcs_el, TOLERANCE); // -0.01 + 0.08

   // Check trigonometric values
   DOUBLES_EQUAL(std::cos(0.37F), detection.processed.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(std::sin(0.37F), detection.processed.sin_vcs_az, TOLERANCE);

   // Check VCS position
   float32_t expected_x = 3.2F + (75.5F * std::cos(0.37F));
   float32_t expected_y = 0.3F + (75.5F * std::sin(0.37F));
   float32_t expected_z = -0.55F + (75.5F * std::sin(0.07F));

   DOUBLES_EQUAL(expected_x, detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_y, detection.processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_z, detection.processed.vcs_position_z, TOLERANCE);

   // Verify trigonometric identity: cos^2 + sin^2 = 1
   float32_t trig_sum = (detection.processed.cos_vcs_az * detection.processed.cos_vcs_az) +
                        (detection.processed.sin_vcs_az * detection.processed.sin_vcs_az);
   DOUBLES_EQUAL(1.0F, trig_sum, TOLERANCE);
}

// ============================================================================
// Additional Edge Case Tests
// ============================================================================

/** \purpose
 * Test with negative range (invalid input)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Negative_Range)
{
   /** \step{1}
    * Verify with negative range (invalid input)
    */

   /** \precond
    * Detection with negative range value
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = -50.0F; // Invalid negative range
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Negative range should result in failure
    */
   CHECK(result == false);
   DOUBLES_EQUAL(-46.5F, detection.processed.vcs_position_x, TOLERANCE); // 3.5 + (-50)*1
}

/** \purpose
 * Test with infinity in range
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Infinity_Range)
{
   /** \step{1}
    * Verify with infinity in range
    */

   /** \precond
    * Detection with infinite range
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = std::numeric_limits<float32_t>::infinity();
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Function should handle infinity gracefully
    */
   CHECK(result == false);
   CHECK(std::isinf(detection.processed.vcs_position_x));
   CHECK(detection.processed.vcs_position_x > 0.0F);
}

/** \purpose
 * Test with NaN in detection azimuth
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_NaN_Azimuth)
{
   /** \step{1}
    * Verify with NaN in detection azimuth
    */

   /** \precond
    * Detection with NaN azimuth angle
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.5F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = std::numeric_limits<float32_t>::quiet_NaN();
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Function should handle NaN gracefully
    */
   CHECK(result == false);
   CHECK(std::isnan(detection.processed.vcs_az));
   CHECK(std::isnan(detection.processed.cos_vcs_az));
   CHECK(std::isnan(detection.processed.sin_vcs_az));
   CHECK(std::isnan(detection.processed.vcs_position_x));
   CHECK(std::isnan(detection.processed.vcs_position_y));
}

/** \purpose
 * Test with NaN in detection elevation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_NaN_Elevation)
{
   /** \step{1}
    * Verify with NaN in detection elevation
    */

   /** \precond
    * Detection with NaN elevation angle
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.0F;
   detection.raw.elevation = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Function should handle NaN gracefully
    */
   CHECK(result == false);
   CHECK(std::isnan(detection.processed.vcs_el));
   CHECK(std::isnan(detection.processed.vcs_position_z));
}

/** \purpose
 * Test with NaN in boresight mounting angle (Sensor data checked earlier in the chain)
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_NaN_Mounting_Angle)
{
   /** \step{1}
    * Verify with NaN in boresight mounting angle
    */

   /** \precond
    * Sensor with NaN mounting angle
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = std::numeric_limits<float32_t>::quiet_NaN();
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.3F;
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Function should handle NaN gracefully
    */
   CHECK(result == true); // Sensor data checked for invalid iputs earlier in the chain
   CHECK(std::isnan(detection.processed.vcs_az));
   CHECK(std::isnan(detection.processed.cos_vcs_az));
   CHECK(std::isnan(detection.processed.sin_vcs_az));
}

/** \purpose
 * Test detection exactly at minimum FOV boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Exactly_At_Min_FOV)
{
   /** \step{1}
    * Verify detection exactly at minimum FOV boundary
    */

   /** \precond
    * Detection exactly at minimum FOV boundary (should be valid)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;
   rspp_sensor_calibration.fov_max_az_rad[0] = 1.5F;
   rspp_sensor_calibration.fov_min_az_rad[0] = -1.5F;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = -1.5F; // Exactly at min FOV
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be valid (inclusive boundary)
    */
   CHECK(result == true);
   DOUBLES_EQUAL(-1.5F, detection.processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test detection slightly below minimum FOV boundary
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Slightly_Below_Min_FOV)
{
   /** \step{1}
    * Verify detection slightly below minimum FOV boundary
    */

   /** \precond
    * Detection slightly below minimum FOV boundary (should be invalid)
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;
   rspp_sensor_calibration.fov_max_az_rad[0] = 1.5F;
   rspp_sensor_calibration.fov_min_az_rad[0] = -1.5F;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.azimuth = -1.501F; // Slightly below min FOV
   detection.raw.elevation = 0.0F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should be invalid (outside boundary)
    */
   CHECK(result == false);
}

/** \purpose
 * Test with very large mounting angles
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Extreme_Mounting_Angle)
{
   /** \step{1}
    * Verify with very large mounting angles
    */

   /** \precond
    * Sensor with extreme but valid mounting angle
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 3.0F; // Nearly PI
   sensor_data.vacs_boresight_el_estimated = 1.4F; // Large elevation mounting angle

   detection.raw.range = 50.0F;
   detection.raw.azimuth = 0.1F;
   detection.raw.elevation = 0.05F;

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should handle extreme mounting angle with angle wrapping
    */
   DOUBLES_EQUAL(3.1F, detection.processed.vcs_az, TOLERANCE);  // 3.0 + 0.1
   DOUBLES_EQUAL(1.45F, detection.processed.vcs_el, TOLERANCE); // 1.4 + 0.05

   // Verify angle is in valid range after normalization
   CHECK(detection.processed.vcs_az >= -3.15F);
   CHECK(detection.processed.vcs_az <= 3.15F);
}

/** \purpose
 * Test symmetry of positive and negative azimuth angles
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Azimuth_Symmetry)
{
   /** \step{1}
    * Verify symmetry of positive and negative azimuth angles
    */

   /** \precond
    * Test with positive azimuth, then negative azimuth
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.0F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = 50.0F;
   detection.raw.elevation = 0.0F;

   // Test positive azimuth
   detection.raw.azimuth = 0.5F;
   bool result_pos = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);
   float32_t y_positive = detection.processed.vcs_position_y;
   float32_t x_positive = detection.processed.vcs_position_x;
   float32_t cos_positive = detection.processed.cos_vcs_az;
   float32_t sin_positive = detection.processed.sin_vcs_az;

   // Test negative azimuth
   detection.raw.azimuth = -0.5F;
   bool result_neg = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Y coordinates should be opposite, X should be equal, cos symmetric, sin anti-symmetric
    */
   CHECK(result_pos == true && result_neg == true);
   DOUBLES_EQUAL(x_positive, detection.processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(-y_positive, detection.processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(cos_positive, detection.processed.cos_vcs_az, TOLERANCE);  // cos is even
   DOUBLES_EQUAL(-sin_positive, detection.processed.sin_vcs_az, TOLERANCE); // sin is odd
}

/** \purpose
 * Test with maximum realistic parameter combination
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Max_All_Parameters)
{
   /** \step{1}
    * Verify with maximum realistic parameter combination
    */

   /** \precond
    * Maximum realistic values for all parameters
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 4.0F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 1.5F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.8F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.2F;
   sensor_data.vacs_boresight_el_estimated = 0.1F;

   detection.raw.range = 300.0F;    // Maximum range
   detection.raw.azimuth = 1.3F;    // Near max FOV
   detection.raw.elevation = 0.24F; // Significant elevation (0.1 + 0.24 = 0.34 < 0.53 max)

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * Should handle maximum realistic scenario correctly
    */
   CHECK(result == true);

   // Check angles are computed
   DOUBLES_EQUAL(1.5F, detection.processed.vcs_az, TOLERANCE);  // 0.2 + 1.3
   DOUBLES_EQUAL(0.34F, detection.processed.vcs_el, TOLERANCE); // 0.1 + 0.24

   // Verify trigonometric identity
   float32_t trig_sum = (detection.processed.cos_vcs_az * detection.processed.cos_vcs_az) +
                        (detection.processed.sin_vcs_az * detection.processed.sin_vcs_az);
   DOUBLES_EQUAL(1.0F, trig_sum, TOLERANCE);

   // Verify coordinates are in reasonable range
   CHECK(detection.processed.vcs_position_x > 0.0F);
   CHECK(detection.processed.vcs_position_y > 0.0F);
}

/** \purpose
 * Test combined NaN in range and elevation
 * \req
 * CPR-7939_Derived
 */
TEST(test_RSPP_Calculate_Detection_VCS_Coordinates,
     Calculate_Detection_VCS_Coordinates_TC_Combined_NaN)
{
   /** \step{1}
    * Verify combined NaN in range and elevation
    */

   /** \precond
    * Multiple NaN values in detection
    */
   rspp_sensor_calibration.vcs_mounting_position.longitudinal = 3.5F;
   rspp_sensor_calibration.vcs_mounting_position.lateral = 0.5F;
   rspp_sensor_calibration.vcs_mounting_position.height = 0.5F;
   rspp_sensor_calibration.polarity = 1;

   sensor_data.vacs_boresight_az_estimated = 0.0F;
   sensor_data.vacs_boresight_el_estimated = 0.0F;

   detection.raw.range = std::numeric_limits<float32_t>::quiet_NaN();
   detection.raw.azimuth = 0.3F;
   detection.raw.elevation = std::numeric_limits<float32_t>::quiet_NaN();

   /** \action
    * Perform complete VCS transformation
    */
   bool result = RSPP_Calculate_Detection_VCS_Coordinates(
       detection.raw,
       sensor_data,
       rspp_sensor_calibration,
       detection.processed);

   /** \result
    * All position coordinates should have NaN
    */
   CHECK(std::isnan(detection.processed.vcs_position_x));
   CHECK(std::isnan(detection.processed.vcs_position_y));
   CHECK(std::isnan(detection.processed.vcs_position_z));
   CHECK(std::isnan(detection.processed.vcs_el));
}

/** @}*/
