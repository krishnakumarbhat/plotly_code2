/** \file
 * This file contains unit tests for content of f360_cvt_two_link_ekf.cpp file
 */

#include "f360_cvt_two_link_ekf.h"
#include "f360_reuse.h"
#include "f360_math.h"
#include "f360_constants.h"
#include <CppUTest/TestHarness.h>
#include <cmath>
#include <vector>
#include <cstring>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup f360_cvt_two_link_ekf
 *  @{
 */

/** \brief
 * Test group for Two_Link_Trailer_EKF function. Tests cover the output initialization and
 * constraints on ekf_state and ekf_state_errcov in all branches (two measurements, one measurement, no measurement,
 * reversing mode, forward mode). The focus is to ensure outputs are always assigned and well-controlled.
 */
TEST_GROUP(f360_cvt_two_link_ekf)
{
   F360_CVT_Input_Data_T cvt_input;
   F360_CVT_State_T cvt_state;
   bool f_updated;
   float32_t initial_ekf_state_errcov[5][5];
   float32_t initial_ekf_state[5];
   F360_Calibrations_T calib{};

   /** \setup
    * Initialize all input and state variables to default valid values.
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);

      // Initialize input data
      memset(&cvt_input, 0, sizeof(cvt_input));
      cvt_input.host_speed = 10.0F;  // [m/s]
      cvt_input.host_yawrate = 0.2F;  // [rad/s]
      cvt_input.host_side_slip_vcs = -0.3F;  // [rad]
      cvt_input.host_rear_axle_vcs_longpos = -5.0F;  // [m]
      cvt_input.n_detections = 10;

      // Initialize state data
      memset(&cvt_state, 0, sizeof(cvt_state));

      // Set up initial ekf_state with reasonable values
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(5.0F);   // x0: trailer angle (rad)
      cvt_state.two_link.ekf_state[1] = 0.5F;   // x1: rear axle to joint (m)
      cvt_state.two_link.ekf_state[2] = 7.0F;   // x2: joint to wheels (m)
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.1F);  // x3: trailer angle (rad)
      cvt_state.two_link.ekf_state[4] = 9.0F;   // x4: joint to wheels (m)

      // Initialize error covariance with diagonal matrix with 0.5F on the diagonal, which is the higher limit of x[0] in Apply_State_Constraints
      for (int32_t i = 0; i < 5; i++)
      {
         for (int32_t j = 0; j < 5; j++)
         {
            cvt_state.two_link.ekf_state_errcov[i][j] = (i == j) ? 0.5F : 0.0F;
         }
      }

      memcpy(&initial_ekf_state_errcov[0][0], &cvt_state.two_link.ekf_state_errcov[0][0], sizeof(initial_ekf_state_errcov));
      memcpy(&initial_ekf_state[0], &cvt_state.two_link.ekf_state[0], sizeof(initial_ekf_state));
      // Set up constraints
      cvt_state.state_constraints.max_x0 = 1.57F;   // pi/2
      cvt_state.state_constraints.min_x0 = -1.57F;
      cvt_state.state_constraints.max_x1 = 15.0F;
      cvt_state.state_constraints.min_x1 = 0.5F;
      cvt_state.state_constraints.max_x2 = 10.0F;
      cvt_state.state_constraints.min_x2 = 3.0F;
      cvt_state.state_constraints.max_x3 = 1.57F;
      cvt_state.state_constraints.min_x3 = -1.57F;
      cvt_state.state_constraints.max_x4 = 10.0F;
      cvt_state.state_constraints.min_x4 = 3.0F;

      // Set up radar position
      cvt_state.radar_vcs_latpos = -0.5F;  // Left side
      cvt_state.radar_vcs_longpos = 0.0F;

      // Initialize measurements as invalid
      cvt_state.primary_measurement.f_msmt_valid = false;
      cvt_state.secondary_measurement.f_msmt_valid = false;

      f_updated = true;  // Initialize to non-zero value to detect if it gets set
   }

   // Helper function to check if array is not zeros
   bool IsArrayInitialized(const float32_t* arr, int32_t size)
   {
      for (int32_t i = 0; i < size; i++)
      {
         if (arr[i] == 0.0F || std::isnan(arr[i]) || std::isinf(arr[i]))
         {
            return false;
         }
      }
      return true;
   }

   // Helper function to check if 2D array is Positive
   bool IsCovarianceMatrixValid(const float32_t* arr, int32_t rows, int32_t cols)
   {
      bool f_element_wrong_found = false;
      for (int32_t i = 0; i < rows; i++)
      {
         for (int32_t j = 0; j < cols; j++)
         {
            if (i == j)
            {
               // diagonal elements to be positive and finite
               f_element_wrong_found = (arr[i * cols + j] <= 0.0F || std::isnan(arr[i * cols + j]) || std::isinf(arr[i * cols + j]));
            }
            else
            {
               // Off-diagonal elements should be non-zero but finite
               f_element_wrong_found = (arr[i * cols + j] == 0.0F) || (std::isnan(arr[i * cols + j]) || std::isinf(arr[i * cols + j]));
            }
            
            if (f_element_wrong_found)
            {
               return false;
            }
         }
      }
      return true;
   }

   // Helper function to check if 2D arrays are equal within tolerance
   bool Are2DArraysEqual(const float32_t* arr1, const float32_t* arr2, int32_t rows, int32_t cols, float32_t tol)
   {
      for (int32_t i = 0; i < rows; i++)
      {
         for (int32_t j = 0; j < cols; j++)
         {
            if (fabsf(arr1[i * cols + j] - arr2[i * cols + j]) > tol)
            {
               return false;
            }
         }
      }
      return true;
   }

   // Helper function to check if 1D arrays are equal within tolerance
   bool AreArraysEqual(const float32_t* arr1, const float32_t* arr2, int32_t size, float32_t tol)
   {
      for (int32_t i = 0; i < size; i++)
      {
         if (fabsf(arr1[i] - arr2[i]) > tol)
         {
            return false;
         }
      }
      return true;
   }

   // Helper function to set up valid two-line measurements
   void SetupValidTwoLinesMeasurements()
   {
      // Setup primary measurement (rearmost link)
      cvt_state.primary_measurement.f_msmt_valid = true;
      cvt_state.primary_measurement.trailer_angle_vcs = cvt_state.two_link.ekf_state[3] + F360_DEG2RAD(0.1F);   // [rad]
      cvt_state.primary_measurement.trailer_intersect_vcs_long = -3.0F;  // [m]

      // Setup secondary measurement (front link)
      cvt_state.secondary_measurement.f_msmt_valid = true;
      cvt_state.secondary_measurement.trailer_angle_vcs = cvt_state.two_link.ekf_state[0] - F360_DEG2RAD(0.1F);  // [rad]
      cvt_state.secondary_measurement.trailer_intersect_vcs_long = -5.0F;  // [m]
   }

   // Helper function to set up valid one-line measurement
   void SetupValidOneLineMeasurement()
   {
      cvt_state.primary_measurement.f_msmt_valid = true;
      cvt_state.primary_measurement.trailer_angle_vcs = 0.1F;
      cvt_state.primary_measurement.trailer_intersect_vcs_long = -3.0F;

      cvt_state.secondary_measurement.f_msmt_valid = false;
   }
};


/** \purpose
 * Test that Two_Link_Trailer_EKF function initializes ekf_state output in reversing mode.
 * Ensures reversing mode preserves state correctly.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_NoMeasurements_ReversingMode_OutputsInitialized)
{
   /** \precond
    * Reversing mode (host_speed < threshold), no valid measurements.
    * Initial ekf_state contains known values.
    */
   cvt_input.host_speed = -5.0F;  // Negative speed indicates reversing

   /** \action
    * Call Two_Link_Trailer_EKF in reversing mode.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.1F, cvt_state, f_updated);

   /** \result
    * f_updated should be false (no measurement).
    * ekf_state and ekf_state_errcov should be updated.
    * In reversing mode, state is partially decayed and errcov is copied.
    * ekf_state_errcov should be the same as before according to our design, so initial values, since only time-update is done.
    */
   CHECK_EQUAL(f_updated, false);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_FALSE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));  // No measurement update, so errcov is initial value
   CHECK_TRUE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov unchanged
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that Two_Link_Trailer_EKF handles valid two-link measurements correctly.
 * Ensures measurement update branch produces assigned outputs.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_TwoValidMeasurements_ForwardMode_MeasurementConfirmedUpdatesOutput)
{
   /** \precond
    * Forward mode with valid measurements from both trailer links.
    * Measurements are within angle gates and intersect constraints.
    * Radar on left side (negative latpos).
    * Both link angles are positive (indicating left-side geometry).
    */
   SetupValidTwoLinesMeasurements();
   cvt_state.radar_vcs_latpos = -0.5F;

   // Ensure state has same sign as measurements for confirmation
   cvt_state.two_link.ekf_state[0] = 0.08F;   // link1 angle, same sign as measurement
   cvt_state.two_link.ekf_state[3] = 0.12F;   // link2 angle, same sign as measurement

   /** \action
    * Call Two_Link_Trailer_EKF with confirmed measurements.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.5F, cvt_state, f_updated);

   /** \result
    * f_updated should be true (measurement confirmed and applied).
    * ekf_state and ekf_state_errcov should be updated with measurement data.
    * All outputs should be assigned and represent valid state after measurement update.
    */
   CHECK_EQUAL(f_updated, true);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_TRUE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));  // Valid after measurement update
   CHECK_FALSE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov changed
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that Two_Link_Trailer_EKF handles valid one-link measurement correctly.
 * Ensures one-link measurement branch produces assigned outputs.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_OneValidMeasurement_ForwardMode_MeasurementConfirmedUpdatesOutput)
{
   /** \precond
    * Forward mode with valid measurement from primary (rearmost) link only.
    * Secondary measurement is invalid.
    * Measurement is within angle gate and intersect constraints.
    * Radar on left side.
    * Link angle is positive for left-side geometry.
    */
   SetupValidOneLineMeasurement();
   cvt_state.radar_vcs_latpos = -0.5F;
   cvt_state.two_link.ekf_state[3] = 0.12F;  // link angle with same sign as measurement

   /** \action
    * Call Two_Link_Trailer_EKF with one valid measurement.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.5F, cvt_state, f_updated);

   /** \result
    * f_updated should be true (measurement confirmed and applied).
    * ekf_state and ekf_state_errcov should be updated.
    * All outputs should be assigned.
    */
   CHECK_EQUAL(f_updated, true);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_TRUE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));  // Valid after measurement update
   CHECK_FALSE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov changed
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that Two_Link_Trailer_EKF handles measurements outside angle gate.
 * Ensures outputs are still assigned when measurement is rejected by gate check.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_TwoMeasurements_AngleOutsideGate_MeasurementRejectedOutputsAssigned)
{
   /** \precond
    * Forward mode with two measurements, but one is outside the angle gate.
    * SetupValidTwoLinesMeasurements() is called with measurements.
    * angle_gate is set to a small value (0.01) to reject the measurements.
    */
   SetupValidTwoLinesMeasurements();
   cvt_state.radar_vcs_latpos = -0.5F;
   cvt_state.two_link.ekf_state[0] = 0.5F;  // Large offset from measurement
   cvt_state.two_link.ekf_state[3] = 0.5F;

   /** \action
    * Call Two_Link_Trailer_EKF with angle_gate set to small value.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.01F, cvt_state, f_updated);

   /** \result
    * f_updated should be false (measurement rejected by gate).
    * ekf_state should still be updated (time update only).
    * and ekf_state_errcov is unchanged due to the design.
    * All outputs should be assigned.
    */
   CHECK_EQUAL(f_updated, false);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_FALSE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));  // No measurement update, so errcov is initial value
   CHECK_TRUE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov unchanged
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that Two_Link_Trailer_EKF handles measurement intersect outside valid range.
 * Ensures outputs are assigned when measurement is rejected by intersect check.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_TwoMeasurements_IntersectOutsideRange_MeasurementRejectedOutputsAssigned)
{
   /** \precond
    * Forward mode with measurements where intersect values are outside valid range.
    * Intersect must be < 0.0 and > (host_rear_axle - x1 - x2 - 5.0).
    */
   SetupValidTwoLinesMeasurements();
   cvt_state.radar_vcs_latpos = -0.5F;
   // Set intersect values outside valid range
   cvt_state.primary_measurement.trailer_intersect_vcs_long = 5.0F;  // Too large
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = 5.0F;

   /** \action
    * Call Two_Link_Trailer_EKF with invalid intersect values.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.5F, cvt_state, f_updated);

   /** \result
    * f_updated should be false (measurement rejected).
    * ekf_state should still be updated (time update only).
    * and ekf_state_errcov is unchanged due to the design.
    * All outputs should be assigned.
    */
   CHECK_EQUAL(f_updated, false);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_FALSE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));  // No measurement update, so errcov is initial value
   CHECK_TRUE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov unchanged
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that Two_Link_Trailer_EKF applies angle state constraints correctly.
 * Ensures outputs are constrained to valid ranges.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_AngleStateOutsideConstraints_ConstraintsApplied)
{
   /** \precond
    * Forward mode, no measurements.
    * Initial state values are set outside their constraint ranges.
    */
   // Set state outside constraints to trigger constraint application
   cvt_state.two_link.ekf_state[0] = 3.0F;  // Beyond max_x0 (1.57)
   cvt_state.two_link.ekf_state[1] = 15.0F; // At max_x1 (15.0)

   /** \action
    * Call Two_Link_Trailer_EKF which will apply constraints.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.1F, cvt_state, f_updated);

   /** \result
    * State values should be clamped to constraint ranges.
    * x0 should be <= 1.57 and >= -1.57.
    * x1 should be <= 15.0 and >= 0.5.
    * All outputs should be assigned.
    * But covariance matrix is unchanged as no measurement update occurred.
    */
   CHECK_FALSE(f_updated);
   CHECK_TRUE(cvt_state.two_link.ekf_state[0] <= cvt_state.state_constraints.max_x0);
   CHECK_TRUE(cvt_state.two_link.ekf_state[0] >= cvt_state.state_constraints.min_x0);
   CHECK_TRUE(cvt_state.two_link.ekf_state[1] <= cvt_state.state_constraints.max_x1);
   CHECK_TRUE(cvt_state.two_link.ekf_state[1] >= cvt_state.state_constraints.min_x1);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_FALSE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));   // No measurement update, so errcov is initial value
   CHECK_TRUE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov unchanged
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that Two_Link_Trailer_EKF outputs are always assigned regardless of branch.
 * This test verifies f_updated is always set (never left uninitialized).
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_VariousInputs_FUpdatedAlwaysSet)
{
   /** \precond
    * Multiple different input scenarios.
    */
   // Initialize f_updated to an out-of-range value to detect if it gets set
   f_updated = true;

   /** \action
    * Call with no measurements.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.1F, cvt_state, f_updated);

   /** \result
    * f_updated should be false (no measurement branch).
    * It should be explicitly set, not left with initial value.
    */
   CHECK_EQUAL(f_updated, false);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_FALSE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));   // No measurement update, so errcov is initial value
   CHECK_TRUE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov unchanged
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed

   // Now test with measurements
   f_updated = false;
   SetupValidOneLineMeasurement();
   cvt_state.radar_vcs_latpos = -0.5F;
   cvt_state.two_link.ekf_state[3] = 0.12F;

   Two_Link_Trailer_EKF(calib, cvt_input, 0.5F, cvt_state, f_updated);

   /** \result
    * f_updated should be true (measurement confirmed branch).
    * It should be explicitly set to the measurement confirmation status.
    */
   CHECK_EQUAL(f_updated, true);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_TRUE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));   // No measurement update, so errcov is initial value
   CHECK_FALSE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov changed
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that function handles radar on right side (positive latpos) correctly.
 * Ensures angle sign checking works for right-side radar geometry.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_RadarRightSide_MeasurementHandledCorrectly)
{
   /** \precond
    * Radar on right side (positive latpos).
    * Measurements set up with negative angles for right-side geometry.
    */
   cvt_state.radar_vcs_latpos = 0.5F;  // Right side

   // For right side, angles should be negative for positive angle gate
   cvt_state.primary_measurement.f_msmt_valid = true;
   cvt_state.primary_measurement.trailer_angle_vcs = -0.1F;
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -3.0F;

   cvt_state.secondary_measurement.f_msmt_valid = true;
   cvt_state.secondary_measurement.trailer_angle_vcs = -0.05F;
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -5.0F;

   // Set state to match right-side geometry (negative angles)
   cvt_state.two_link.ekf_state[0] = -0.08F;
   cvt_state.two_link.ekf_state[3] = -0.12F;

   /** \action
    * Call Two_Link_Trailer_EKF with right-side geometry.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.5F, cvt_state, f_updated);

   /** \result
    * Function should handle right-side geometry correctly.
    * All outputs should be assigned.
    */
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_TRUE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));
   CHECK_FALSE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov changed
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that function handles reversing mode with measurements.
 * Ensures measurement update is not applied in reversing mode even if available.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, SanityCheck_ReversingMode_MeasurementsIgnored)
{
   /** \precond
    * Reversing mode with valid measurements available.
    * Measurements should not be applied in reverse mode.
    */
   cvt_input.host_speed = -5.0F;  // Reversing
   SetupValidTwoLinesMeasurements();
   cvt_state.radar_vcs_latpos = -0.5F;

   /** \action
    * Call Two_Link_Trailer_EKF in reversing mode with measurements.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, 0.5F, cvt_state, f_updated);

   /** \result
    * f_updated should be false (measurements not applied in reverse).
    * Outputs should still be assigned (time update applied).
    * ekf_state should reflect reversing dynamics (decay applied).
    */
   CHECK_EQUAL(f_updated, false);
   CHECK_TRUE(IsArrayInitialized(cvt_state.two_link.ekf_state, 5));
   CHECK_FALSE(IsCovarianceMatrixValid(&cvt_state.two_link.ekf_state_errcov[0][0], 5, 5));
   CHECK_TRUE(Are2DArraysEqual(&cvt_state.two_link.ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], 5, 5, F360_EPSILON));  // Ensure errcov unchanged
   CHECK_FALSE(AreArraysEqual(&cvt_state.two_link.ekf_state[0], &initial_ekf_state[0], 5, F360_EPSILON));  // Ensure state changed
}

/** \purpose
 * Test that KF_Measurement_Update_1msmt function runs as expected.
 * Ensures measurement update with 1 measurement is correct.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, KF_Measurement_Update_1msmt_Works)
{
   /** \precond
    * One line is measured. Initialize with larger errcov to see update effect more clearly.
    * The measured trailer angle is close to the estimated angle state, to allow measurement update.
    */
   SetupValidOneLineMeasurement();
   cvt_state.primary_measurement.trailer_angle_vcs = 0.13F;
   const float32_t epsilon = 1E-3F;

   // Initialize error covariance with identity matrix
   for (int32_t i = 0; i < 5; i++)
   {
      for (int32_t j = 0; j < 5; j++)
      {
         cvt_state.two_link.ekf_state_errcov[i][j] = (i == j) ? 1.0F : 0.0F;
      }
   }
   /** \action
    * Call Two_Link_Trailer_EKF in forward mode.
    */
   KF_Measurement_Update_1msmt(cvt_state.primary_measurement, cvt_input.host_rear_axle_vcs_longpos, cvt_state.two_link.ekf_state, cvt_state.two_link.ekf_state_errcov);

   /** \result
    * ekf_state and ekf_state_errcov should be numerically correct
    */
   DOUBLES_EQUAL_TEXT(0.0385F, cvt_state.two_link.ekf_state[0], epsilon, "x1 should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.5006F, cvt_state.two_link.ekf_state[1], epsilon, "x2 should be updated correctly");
   DOUBLES_EQUAL_TEXT(7.0F, cvt_state.two_link.ekf_state[2], epsilon, "x3 should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.1345F, cvt_state.two_link.ekf_state[3], epsilon, "x4 should be updated correctly");
   DOUBLES_EQUAL_TEXT(9.0F, cvt_state.two_link.ekf_state[4], epsilon, "x5 should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.391F, cvt_state.two_link.ekf_state_errcov[0][0], epsilon, "errcov[0][0] should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.999F, cvt_state.two_link.ekf_state_errcov[1][1], epsilon, "errcov[1][1] should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.999F, cvt_state.two_link.ekf_state_errcov[2][2], epsilon, "errcov[2][2] should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.406F, cvt_state.two_link.ekf_state_errcov[3][3], epsilon, "errcov[3][3] should be updated correctly");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.two_link.ekf_state_errcov[4][4], epsilon, "errcov[4][4] should be almost unchanged");
}

/** \purpose
 * Test that KF_Measurement_Update_2msmt function runs as expected.
 * Ensures measurement update with 2 measurements is correct.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf, KF_Measurement_Update_2msmt_Works)
{
   /** \precond
    * Both of the two lines are valid,set a larger errcov matrix to see the update effect more clearly.
    */
   SetupValidTwoLinesMeasurements();
   cvt_state.primary_measurement.trailer_angle_vcs = 0.08F;
   cvt_state.secondary_measurement.trailer_angle_vcs = 0.15F;
   const float32_t epsilon = 1E-3F;

   // Initialize error covariance with identity matrix
   for (int32_t i = 0; i < 5; i++)
   {
      for (int32_t j = 0; j < 5; j++)
      {
         cvt_state.two_link.ekf_state_errcov[i][j] = (i == j) ? 1.0F : 0.0F;
      }
   }
   /** \action
    * Call Two_Link_Trailer_EKF in forward mode.
    */
   KF_Measurement_Update_2msmt(cvt_state.primary_measurement, cvt_state.secondary_measurement, cvt_input.host_rear_axle_vcs_longpos, cvt_state.two_link.ekf_state, cvt_state.two_link.ekf_state_errcov);

   /** \result
    * ekf_state and ekf_state_errcov should be numerically correct
    */
   DOUBLES_EQUAL_TEXT(0.04F, cvt_state.two_link.ekf_state[0], epsilon, "x1 should be updated correctly");
   DOUBLES_EQUAL_TEXT(1.182F, cvt_state.two_link.ekf_state[1], epsilon, "x2 should be updated correctly");
   DOUBLES_EQUAL_TEXT(7.0F, cvt_state.two_link.ekf_state[2], epsilon, "x3 should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.1532F, cvt_state.two_link.ekf_state[3], epsilon, "x4 should be updated correctly");
   DOUBLES_EQUAL_TEXT(9.0F, cvt_state.two_link.ekf_state[4], epsilon, "x5 should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.3272F, cvt_state.two_link.ekf_state_errcov[0][0], epsilon, "errcov[0][0] should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.9089F, cvt_state.two_link.ekf_state_errcov[1][1], epsilon, "errcov[1][1] should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.999F, cvt_state.two_link.ekf_state_errcov[2][2], epsilon, "errcov[2][2] should be updated correctly");
   DOUBLES_EQUAL_TEXT(0.34F, cvt_state.two_link.ekf_state_errcov[3][3], epsilon, "errcov[3][3] should be updated correctly");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.two_link.ekf_state_errcov[4][4], epsilon, "errcov[4][4] should be almost unchanged");
}

/** \brief
 * Test group for 1-link measurement confirmation tests.
 * Tests cover the output initialization and confirmation logic for 1 valid measurement
 * in various scenarios (different radar positions, states, and measurements).
 * The focus is to ensure measurement confirmation logic works correctly and measurements
 * are only confirmed when all validation criteria are met.
 */
TEST_GROUP(f360_cvt_two_link_ekf_1link_msmt_confirm)
{
   F360_CVT_Input_Data_T cvt_input;
   F360_CVT_State_T cvt_state;
   F360_CVT_State_T initial_cvt_state;
   bool f_updated;
   float32_t initial_ekf_state_errcov[5][5];
   float32_t initial_ekf_state[5];
   F360_Calibrations_T calib{};

   /** \setup
    * Initialize all input and state variables to default valid values
    * and set up 1 valid measured line for all tests in this group.
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);
      // Initialize input data
      memset(&cvt_input, 0, sizeof(cvt_input));
      cvt_input.host_speed = 2.0F;  // [m/s]
      cvt_input.host_yawrate = 0.2F;  // [rad/s]
      cvt_input.host_side_slip_vcs = -0.3F;  // [rad]
      cvt_input.host_rear_axle_vcs_longpos = -5.0F;  // [m]
      cvt_input.n_detections = 10;

      // Initialize state data
      memset(&cvt_state, 0, sizeof(cvt_state));

      // Set up initial ekf_state with reasonable values
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(5.0F);   // x0: trailer angle (rad)
      cvt_state.two_link.ekf_state[1] = 0.5F;   // x1: rear axle to joint (m)
      cvt_state.two_link.ekf_state[2] = 7.0F;   // x2: joint to wheels (m)
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.1F);  // x3: trailer angle (rad)
      cvt_state.two_link.ekf_state[4] = 9.0F;   // x4: joint to wheels (m)

      // Initialize error covariance with diagonal matrix
      for (int32_t i = 0; i < 5; i++)
      {
         for (int32_t j = 0; j < 5; j++)
         {
            cvt_state.two_link.ekf_state_errcov[i][j] = (i == j) ? 1.0F : 0.0F;
         }
      }

      memcpy(&initial_ekf_state_errcov[0][0], &cvt_state.two_link.ekf_state_errcov[0][0], sizeof(initial_ekf_state_errcov));
      memcpy(&initial_ekf_state[0], &cvt_state.two_link.ekf_state[0], sizeof(initial_ekf_state));

      // Set up constraints
      cvt_state.state_constraints.max_x0 = 1.57F;   // pi/2
      cvt_state.state_constraints.min_x0 = -1.57F;
      cvt_state.state_constraints.max_x1 = 15.0F;
      cvt_state.state_constraints.min_x1 = 0.5F;
      cvt_state.state_constraints.max_x2 = 10.0F;
      cvt_state.state_constraints.min_x2 = 3.0F;
      cvt_state.state_constraints.max_x3 = 1.57F;
      cvt_state.state_constraints.min_x3 = -1.57F;
      cvt_state.state_constraints.max_x4 = 10.0F;
      cvt_state.state_constraints.min_x4 = 3.0F;

      // Set up radar position
      cvt_state.radar_vcs_latpos = -2.0F;  // Left side
      cvt_state.radar_vcs_longpos = 0.0F;

      // Set up 1 valid measured line for all tests in this group
      cvt_state.primary_measurement.f_msmt_valid = true;
      cvt_state.primary_measurement.trailer_angle_vcs = 0.1F;
      cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
      cvt_state.secondary_measurement.f_msmt_valid = false;

      f_updated = true;  // Initialize to non-zero value to detect if it gets set

      // Save initial state for comparison
      (void)memcpy(&initial_cvt_state, &cvt_state, sizeof(initial_cvt_state));
   }

};

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * left radar's front angle state[0] is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, LeftRadar_FrontAngleStateNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Left radar, front angle state[0] outside acceptable range (2.0 deg instead of 2.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.0F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.1F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Left radar front angle state[0] not ok should block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * left radar's rear trailer angle state[3] is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, LeftRadar_RearAngleStateNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Left radar, rear angle state[3] outside acceptable range (5.0 deg instead of 5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.0F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.1F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Left radar rear trailer angle state[3] not ok should block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will confirm measurement when
 * left radar's both trailer angle states are within acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, LeftRadar_BothAngleStatesOk_AllowsMsmtUpdate)
{
   /** \precond
    * Left radar, both angle states within acceptable range (2.1 deg and 5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.1F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be allowed
    */
   CHECK_TRUE_TEXT(f_updated, "Left radar both trailer angle states ok will allow msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * left radar's primary measured angle is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, LeftRadar_PrimaryMsmtAngleNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Left radar, primary measured angle outside acceptable range (5.0 deg instead of 5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.0F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Left radar primary measured angle not ok will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's front angle state[0] is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, RightRadar_FrontAngleStateNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, front angle state[0] outside acceptable range (-2.0 deg instead of -2.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.0F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar front angle state[0] not ok should block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's rear trailer angle state[3] is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, RightRadar_RearAngleStateNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, rear angle state[3] outside acceptable range (-5.0 deg instead of -5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.0F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar rear trailer angle state[3] not ok should block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will confirm measurement when
 * right radar's both trailer angle states are within acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, RightRadar_BothAngleStatesOk_AllowsMsmtUpdate)
{
   /** \precond
    * Right radar, both angle states within acceptable range (-2.1 deg and -5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be allowed
    */
   CHECK_TRUE_TEXT(f_updated, "Right radar both trailer angle states ok will allow msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's primary measured angle is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, RightRadar_PrimaryMsmtAngleNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, primary measured angle outside acceptable range (-5.0 deg instead of -5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.0F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar primary measured angle not ok will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's primary measured angle exceeds the gate threshold.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, RightRadar_PrimaryMsmtAngleExceedsGate_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, primary measured angle exceeds the gate (-10.1 deg, gate is 5.0 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-10.1F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar primary measured angle exceeds the gate will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's primary measured intersection is too large (exceeds threshold).
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, RightRadar_PrimaryMsmtIntersectTooLarge_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, primary measured intersection too large (0.0 instead of -17.4)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = 0.0F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar primary measured inersect too large will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's primary measured intersection is too small (below threshold).
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, RightRadar_PrimaryMsmtIntersectTooSmall_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, primary measured intersection too small (-17.5 instead of -17.4)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.5F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar primary measured inersect too small will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * the vehicle is reversing, regardless of other conditions.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_1link_msmt_confirm, WhenReversing_NoMsmtUpdateAllowed)
{
   /** \precond
    * Vehicle is reversing (negative host_speed), otherwise all conditions are valid
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_input.host_speed = -0.6F;  // Reversing
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.1F);

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "When reversing, no measurement update is allowed!");
}

/** \brief
 * Test group for 2-link measurement confirmation tests.
 * Tests cover the output initialization and confirmation logic for 2 valid measurements
 * in various scenarios (different radar positions, states, measurements, and gates).
 * The focus is to ensure measurement confirmation logic works correctly and measurements
 * are only confirmed when all validation criteria are met.
 */
TEST_GROUP(f360_cvt_two_link_ekf_2link_msmt_confirm)
{
   F360_CVT_Input_Data_T cvt_input;
   F360_CVT_State_T cvt_state;
   F360_CVT_State_T initial_cvt_state;
   bool f_updated;
   float32_t initial_ekf_state_errcov[5][5];
   float32_t initial_ekf_state[5];
   F360_Calibrations_T calib{};

   /** \setup
    * Initialize all input and state variables to default valid values
    * and set up 2 valid measured lines for all tests in this group.
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);
      // Initialize input data
      memset(&cvt_input, 0, sizeof(cvt_input));
      cvt_input.host_speed = 10.0F;  // [m/s]
      cvt_input.host_yawrate = 0.2F;  // [rad/s]
      cvt_input.host_side_slip_vcs = -0.3F;  // [rad]
      cvt_input.host_rear_axle_vcs_longpos = -5.0F;  // [m]
      cvt_input.n_detections = 10;

      // Initialize state data
      memset(&cvt_state, 0, sizeof(cvt_state));

      // Set up initial ekf_state with reasonable values
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(5.0F);   // x0: trailer angle (rad)
      cvt_state.two_link.ekf_state[1] = 0.5F;   // x1: rear axle to joint (m)
      cvt_state.two_link.ekf_state[2] = 7.0F;   // x2: joint to wheels (m)
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.1F);  // x3: trailer angle (rad)
      cvt_state.two_link.ekf_state[4] = 9.0F;   // x4: joint to wheels (m)

      // Initialize error covariance with diagonal matrix
      for (int32_t i = 0; i < 5; i++)
      {
         for (int32_t j = 0; j < 5; j++)
         {
            cvt_state.two_link.ekf_state_errcov[i][j] = (i == j) ? 1.0F : 0.0F;
         }
      }

      memcpy(&initial_ekf_state_errcov[0][0], &cvt_state.two_link.ekf_state_errcov[0][0], sizeof(initial_ekf_state_errcov));
      memcpy(&initial_ekf_state[0], &cvt_state.two_link.ekf_state[0], sizeof(initial_ekf_state));
      
      // Set up constraints
      cvt_state.state_constraints.max_x0 = 1.57F;   // pi/2
      cvt_state.state_constraints.min_x0 = -1.57F;
      cvt_state.state_constraints.max_x1 = 15.0F;
      cvt_state.state_constraints.min_x1 = 0.5F;
      cvt_state.state_constraints.max_x2 = 10.0F;
      cvt_state.state_constraints.min_x2 = 3.0F;
      cvt_state.state_constraints.max_x3 = 1.57F;
      cvt_state.state_constraints.min_x3 = -1.57F;
      cvt_state.state_constraints.max_x4 = 10.0F;
      cvt_state.state_constraints.min_x4 = 3.0F;

      // Set up radar position
      cvt_state.radar_vcs_latpos = -0.5F;  // Left side
      cvt_state.radar_vcs_longpos = 0.0F;

      // Set up 2 valid measured lines for all tests in this group
      cvt_state.primary_measurement.f_msmt_valid = true;
      cvt_state.primary_measurement.trailer_angle_vcs = cvt_state.two_link.ekf_state[3] + F360_DEG2RAD(0.1F);
      cvt_state.primary_measurement.trailer_intersect_vcs_long = -3.0F;

      cvt_state.secondary_measurement.f_msmt_valid = true;
      cvt_state.secondary_measurement.trailer_angle_vcs = cvt_state.two_link.ekf_state[0] - F360_DEG2RAD(0.1F);
      cvt_state.secondary_measurement.trailer_intersect_vcs_long = -5.0F;

      f_updated = true;  // Initialize to non-zero value to detect if it gets set

      // Save initial state for comparison
      (void)memcpy(&initial_cvt_state, &cvt_state, sizeof(initial_cvt_state));
   }

};

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * left radar's front angle state[0] is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, LeftRadar_FrontAngleStateNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Left radar, front angle state[0] outside acceptable range (2.0 deg instead of 2.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = -2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.0F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Left radar front angle state[0] not ok should block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * left radar's rear trailer angle state[3] is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, LeftRadar_RearAngleStateNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Left radar, rear angle state[3] outside acceptable range (5.0 deg instead of 5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = -2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.0F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Left radar rear trailer angle state[3] not ok should block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will confirm measurement when
 * left radar's both trailer angle states are within acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, LeftRadar_BothAngleStatesOk_AllowsMsmtUpdate)
{
   /** \precond
    * Left radar, both angle states within acceptable range (2.1 deg and 5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = -2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be allowed
    */
   CHECK_TRUE_TEXT(f_updated, "Left radar both trailer angle states ok will allow msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * left radar's secondary measured angle is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, LeftRadar_SecondaryMsmtAngleNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Left radar, secondary measured angle outside acceptable range (2.0 deg instead of 2.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = -2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(2.0F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Left radar secondary measured angle not ok will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * left radar's primary measured angle is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, LeftRadar_PrimaryMsmtAngleNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Left radar, primary measured angle outside acceptable range (5.0 deg instead of 5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = -2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(5.0F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Left radar primary measured angle not ok will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's front angle state[0] is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_FrontAngleStateNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, front angle state[0] outside acceptable range (-2.0 deg instead of -2.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.0F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar front angle state[0] not ok should block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's rear trailer angle state[3] is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_RearAngleStateNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, rear angle state[3] outside acceptable range (-5.0 deg instead of -5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.0F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar rear trailer angle state[3] not ok should block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will confirm measurement when
 * right radar's both trailer angle states are within acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_BothAngleStatesOk_AllowsMsmtUpdate)
{
   /** \precond
    * Right radar, both angle states within acceptable range (-2.1 deg and -5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be allowed
    */
   CHECK_TRUE_TEXT(f_updated, "Right radar both trailer angle states ok will allow msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's secondary measured angle is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_SecondaryMsmtAngleNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, secondary measured angle outside acceptable range (-2.0 deg instead of -2.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.0F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar secondary measured angle not ok will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's primary measured angle is outside acceptable range.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_PrimaryMsmtAngleNotOk_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, primary measured angle outside acceptable range (-5.0 deg instead of -5.1 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.0F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar primary measured angle not ok will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's primary measured angle exceeds the gate threshold.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_PrimaryMsmtAngleExceedsGate_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, primary measured angle exceeds the gate (-10.1 deg, gate is 5.0 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-10.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar primary measured angle exceeds the gate will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's secondary measured angle exceeds the gate threshold.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_SecondaryMsmtAngleExceedsGate_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, secondary measured angle exceeds the gate (-7.1 deg, gate is 5.0 deg)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-7.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar secondary measured angle exceeds the gate will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's primary measured intersection is too large (exceeds threshold).
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_PrimaryMsmtIntersectTooLarge_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, primary measured intersection too large (0.0 instead of -17.4)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = 0.0F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar primary measured inersect too large will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's primary measured intersection is too small (below threshold).
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_PrimaryMsmtIntersectTooSmall_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, primary measured intersection too small (-17.5 instead of -17.4)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.5F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar primary measured inersect too small will block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's secondary measured intersection is too large (exceeds threshold).
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_SecondaryMsmtIntersectTooLarge_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, secondary measured intersection too large (-17.5 instead of -0.1)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -17.5F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar secondary measured inersect too large block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * right radar's secondary measured intersection is too small (below threshold).
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, RightRadar_SecondaryMsmtIntersectTooSmall_BlocksMsmtUpdate)
{
   /** \precond
    * Right radar, secondary measured intersection too small (0.0 instead of -0.1)
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = 2.0F;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = 0.0F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "Right radar secondary measured inersect too small block msmt update!");
}

/** \purpose
 * Test that Two_Link_Trailer_EKF function will block measurement confirmation when
 * the vehicle is reversing, regardless of other conditions.
 * \req NA
 */
TEST(f360_cvt_two_link_ekf_2link_msmt_confirm, WhenReversing_NoMsmtUpdateAllowed)
{
   /** \precond
    * Vehicle is reversing (negative host_speed), otherwise all conditions are valid
    */
   (void)memcpy(&cvt_state, &initial_cvt_state, sizeof(cvt_state));
   f_updated = false;

   cvt_state.radar_vcs_latpos = 2.0F;
   cvt_input.host_speed = -0.6F;  // Reversing
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(-2.1F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(-5.1F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -17.4F;
   cvt_state.secondary_measurement.trailer_angle_vcs = F360_DEG2RAD(-2.1F);
   cvt_state.secondary_measurement.trailer_intersect_vcs_long = -0.1F;

   /** \action
    * Call Two_Link_Trailer_EKF.
    */
   Two_Link_Trailer_EKF(calib, cvt_input, F360_DEG2RAD(5.0F), cvt_state, f_updated);

   /** \result
    * Measurement update should be blocked
    */
   CHECK_FALSE_TEXT(f_updated, "When reversing, no measurement update will be done!");
}


/** @}*/

/** \defgroup kf_time_update_2link
 *  @{
 */

/** \brief
 * Test group for KF_Time_Update_2link function. Tests cover all branches: reversing mode, forward mode with/without update, and x5 scaling conditions.
 */
TEST_GROUP(kf_time_update_2link)
{
   bool f_reversing;
   bool f_update_valid;
   bool f_radar_left_side;
   float32_t elapsed_time;
   float32_t host_speed;
   float32_t host_yawrate;
   float32_t ekf_state[5];
   float32_t ekf_state_errcov[5][5];
   float32_t initial_ekf_state[5];
   float32_t initial_ekf_state_errcov[5][5];
   float32_t epsilon;
   F360_Calibrations_T calib{};

   /** \setup
    * Initialize all input and state variables to default valid values.
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);

      f_reversing = false;
      f_update_valid = true;
      f_radar_left_side = true;
      elapsed_time = 0.05F;
      host_speed = 2.0F;
      host_yawrate = 0.1F;
      epsilon = 1E-3F;

      // Initialize ekf_state
      ekf_state[0] = 0.1F;  // x1
      ekf_state[1] = 5.0F;  // x2
      ekf_state[2] = 8.0F;  // x3
      ekf_state[3] = -0.2F; // x4
      ekf_state[4] = 10.0F; // x5

      // Initialize ekf_state_errcov as identity matrix
      memset(&ekf_state_errcov[0][0], 0, sizeof(ekf_state_errcov));
      for (int i = 0; i < 5; i++) {
         ekf_state_errcov[i][i] = 1.0F;
      }

      // Save initial values
      (void)memcpy(&initial_ekf_state[0], &ekf_state[0], sizeof(initial_ekf_state));
      (void)memcpy(&initial_ekf_state_errcov[0][0], &ekf_state_errcov[0][0], sizeof(initial_ekf_state_errcov));
   }
};

/** \brief
 * Test KF_Time_Update_2link in reversing mode with x5 scaling applied
 * \req
 * NA
 */
TEST(kf_time_update_2link, the_scaled_x5_doesnt_impact_states_if_reversing)
{
   /** \precond
    * Vehicle is reversing, radar on left side, x4 negative (then x5 scaling applied)
    */
   f_reversing = true;
   f_radar_left_side = true;
   ekf_state[3] = -0.5F;  // x4 < 0
   initial_ekf_state[3] = ekf_state[3];

   /** \action
    * Execute time update
    */
   KF_Time_Update_2link(calib, f_reversing, f_update_valid, f_radar_left_side, elapsed_time, host_speed, host_yawrate, ekf_state, ekf_state_errcov);

   /** \result
    * State updated for reversing, errcov unchanged
    */
   DOUBLES_EQUAL_TEXT(initial_ekf_state[0] * 0.9F, ekf_state[0], epsilon, "x1 scaled by 0.9 in reversing");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[3] * 0.9F, ekf_state[3], epsilon, "x4 scaled by 0.9 in reversing");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[1], ekf_state[1], epsilon, "x2 unchanged");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[2], ekf_state[2], epsilon, "x3 unchanged");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[4], ekf_state[4], epsilon, "x5 unchanged");
   CHECK_TRUE(memcmp(&ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], sizeof(ekf_state_errcov)) == 0);
}

/** \brief
 * Test KF_Time_Update_2link in reversing mode without x5 scaling
 * \req
 * NA
 */
TEST(kf_time_update_2link, the_non_scaled_x5_doesnt_impact_states_anyway_if_reversing)
{
   /** \precond
    * Vehicle is reversing, radar on left side, x4 positive (no x5 scaling)
    */
   f_reversing = true;
   f_radar_left_side = true;
   ekf_state[3] = 0.5F;  // x4 > 0
   initial_ekf_state[3] = ekf_state[3];

   /** \action
    * Execute time update
    */
   KF_Time_Update_2link(calib, f_reversing, f_update_valid, f_radar_left_side, elapsed_time, host_speed, host_yawrate, ekf_state, ekf_state_errcov);

   /** \result
    * State updated for reversing, errcov unchanged
    */
   DOUBLES_EQUAL_TEXT(initial_ekf_state[0] * 0.9F, ekf_state[0], epsilon, "x1 scaled by 0.9 in reversing");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[3] * 0.9F, ekf_state[3], epsilon, "x4 scaled by 0.9 in reversing");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[1], ekf_state[1], epsilon, "x2 unchanged");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[2], ekf_state[2], epsilon, "x3 unchanged");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[4], ekf_state[4], epsilon, "x5 unchanged");
   CHECK_TRUE(memcmp(&ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], sizeof(ekf_state_errcov)) == 0);
}

/** \brief
 * Test KF_Time_Update_2link in stationary mode with x5 scaling
 * \req
 * NA
 */
TEST(kf_time_update_2link, the_scaled_x5_doesnt_impact_states_if_stationary)
{
   /** \precond
    * Vehicle stationary (speed <= threshold), radar on left side, x4 negative
    */
   f_reversing = false;
   host_speed = 0.09F;  // Below slow_speed_threshold (0.1)
   f_radar_left_side = true;
   ekf_state[3] = -0.5F;
   initial_ekf_state[3] = ekf_state[3];

   /** \action
    * Execute time update
    */
   KF_Time_Update_2link(calib, f_reversing, f_update_valid, f_radar_left_side, elapsed_time, host_speed, host_yawrate, ekf_state, ekf_state_errcov);

   /** \result
    * State and errcov unchanged
    */
   CHECK_TRUE(memcmp(&ekf_state[0], &initial_ekf_state[0], sizeof(ekf_state)) == 0);
   CHECK_TRUE(memcmp(&ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], sizeof(ekf_state_errcov)) == 0);
}

/** \brief
 * Test KF_Time_Update_2link in stationary mode without x5 scaling
 * \req
 * NA
 */
TEST(kf_time_update_2link, the_non_scaled_x5_doesnt_impact_states_if_stationary)
{
   /** \precond
    * Vehicle stationary, radar on left side, x4 positive
    */
   f_reversing = false;
   host_speed = 0.09F;
   f_radar_left_side = true;
   ekf_state[3] = 0.5F;
   initial_ekf_state[3] = ekf_state[3];

   /** \action
    * Execute time update
    */
   KF_Time_Update_2link(calib, f_reversing, f_update_valid, f_radar_left_side, elapsed_time, host_speed, host_yawrate, ekf_state, ekf_state_errcov);

   /** \result
    * State and errcov unchanged
    */
   CHECK_TRUE(memcmp(&ekf_state[0], &initial_ekf_state[0], sizeof(ekf_state)) == 0);
   CHECK_TRUE(memcmp(&ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], sizeof(ekf_state_errcov)) == 0);
}

/** \brief
 * Test KF_Time_Update_2link in forward mode with update valid and x5 scaling
 * \req
 * NA
 */
TEST(kf_time_update_2link, time_update_x5_scaled_case_when_forward)
{
   /** \precond
    * Vehicle moving forward, update valid, radar on left side, x4 negative
    */
   f_reversing = false;
   host_speed = 2.0F;  // Above threshold
   f_update_valid = true;
   f_radar_left_side = true;
   ekf_state[3] = -0.5F;
   initial_ekf_state[3] = ekf_state[3];

   /** \action
    * Execute time update
    */
   KF_Time_Update_2link(calib, f_reversing, f_update_valid, f_radar_left_side, elapsed_time, host_speed, host_yawrate, ekf_state, ekf_state_errcov);

   /** \result
    * State updated, errcov updated
    */
   DOUBLES_EQUAL_TEXT(0.09F, ekf_state[0], epsilon, "x1 should be changed");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[1], ekf_state[1], epsilon, "x2 should be unchanged");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[2], ekf_state[2], epsilon, "x3 should be unchanged");
   DOUBLES_EQUAL_TEXT(-0.499F, ekf_state[3], epsilon, "x4 should be changed");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[4], ekf_state[4], epsilon, "x5 should be unchanged");
   DOUBLES_EQUAL_TEXT(0.977F, ekf_state_errcov[0][0], epsilon, "errcov[0][0] should be changed");
   DOUBLES_EQUAL_TEXT(1.0F, ekf_state_errcov[1][1], epsilon, "errcov[1][1] should be almost unchanged");
   DOUBLES_EQUAL_TEXT(1.001F, ekf_state_errcov[2][2], epsilon, "errcov[2][2] should be changed");
   DOUBLES_EQUAL_TEXT(0.985F, ekf_state_errcov[3][3], epsilon, "errcov[3][3] should be changed");
   DOUBLES_EQUAL_TEXT(1.0F, ekf_state_errcov[4][4], epsilon, "errcov[4][4] should be almost unchanged");
   CHECK_FALSE(memcmp(&ekf_state[0], &initial_ekf_state[0], sizeof(ekf_state)) == 0);  // State changed
   CHECK_FALSE(memcmp(&ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], sizeof(ekf_state_errcov)) == 0);  // Errcov changed
}

/** \brief
 * Test KF_Time_Update_2link in forward mode with update valid and no x5 scaling
 * \req
 * NA
 */
TEST(kf_time_update_2link, x5_not_scaled_case_when_forward)
{
   /** \precond
    * Vehicle moving forward, update valid, radar on left side, x4 positive
    */
   f_reversing = false;
   host_speed = 2.0F;
   f_update_valid = true;
   f_radar_left_side = true;
   ekf_state[3] = 0.5F;
   initial_ekf_state[3] = ekf_state[3];

   /** \action
    * Execute time update
    */
   KF_Time_Update_2link(calib, f_reversing, f_update_valid, f_radar_left_side, elapsed_time, host_speed, host_yawrate, ekf_state, ekf_state_errcov);

   /** \result
    * State updated, errcov updated
    */
   DOUBLES_EQUAL_TEXT(0.09F, ekf_state[0], epsilon, "x1 should be changed");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[1], ekf_state[1], epsilon, "x2 should be unchanged");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[2], ekf_state[2], epsilon, "x3 should be unchanged");
   DOUBLES_EQUAL_TEXT(0.491F, ekf_state[3], epsilon, "x4 should be changed");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[4], ekf_state[4], epsilon, "x5 should be unchanged");
   DOUBLES_EQUAL_TEXT(0.977F, ekf_state_errcov[0][0], epsilon, "errcov[0][0] should be changed");
   DOUBLES_EQUAL_TEXT(1.0F, ekf_state_errcov[1][1], epsilon, "errcov[1][1] should be almost unchanged");
   DOUBLES_EQUAL_TEXT(1.001F, ekf_state_errcov[2][2], epsilon, "errcov[2][2] should be changed");
   DOUBLES_EQUAL_TEXT(0.983F, ekf_state_errcov[3][3], epsilon, "errcov[3][3] should be changed");
   DOUBLES_EQUAL_TEXT(1.0F, ekf_state_errcov[4][4], epsilon, "errcov[4][4] should be almost unchanged");
   CHECK_FALSE(memcmp(&ekf_state[0], &initial_ekf_state[0], sizeof(ekf_state)) == 0);
   CHECK_FALSE(memcmp(&ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], sizeof(ekf_state_errcov)) == 0);
}

/** \brief
 * Test KF_Time_Update_2link in forward mode with update invalid and x5 scaling
 * \req
 * NA
 */
TEST(kf_time_update_2link, no_cov_update_if_measurement_invalid_with_x5_scaled)
{
   /** \precond
    * Vehicle moving forward, update invalid, radar on left side, x4 negative
    */
   f_reversing = false;
   host_speed = 2.0F;
   f_update_valid = false;
   f_radar_left_side = true;
   ekf_state[3] = -0.5F;
   initial_ekf_state[3] = ekf_state[3];

   /** \action
    * Execute time update
    */
   KF_Time_Update_2link(calib, f_reversing, f_update_valid, f_radar_left_side, elapsed_time, host_speed, host_yawrate, ekf_state, ekf_state_errcov);

   /** \result
    * State updated, errcov unchanged
    */
   DOUBLES_EQUAL_TEXT(0.09F, ekf_state[0], epsilon, "x1 should be changed");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[1], ekf_state[1], epsilon, "x2 should be unchanged");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[2], ekf_state[2], epsilon, "x3 should be unchanged");
   DOUBLES_EQUAL_TEXT(-0.498F, ekf_state[3], epsilon, "x4 should be changed");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[4], ekf_state[4], epsilon, "x5 should be unchanged");
   CHECK_FALSE(memcmp(&ekf_state[0], &initial_ekf_state[0], sizeof(ekf_state)) == 0);
   CHECK_TRUE(memcmp(&ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], sizeof(ekf_state_errcov)) == 0);
}

/** \brief
 * Test KF_Time_Update_2link in forward mode with update invalid and no x5 scaling
 * \req
 * NA
 */
TEST(kf_time_update_2link, no_cov_update_if_measurement_invalid_with_x5_not_scaled)
{
   /** \precond
    * Vehicle moving forward, update invalid, radar on left side, x4 positive
    */
   f_reversing = false;
   host_speed = 2.0F;
   f_update_valid = false;
   f_radar_left_side = true;
   ekf_state[3] = 0.5F;
   initial_ekf_state[3] = ekf_state[3];

   /** \action
    * Execute time update
    */
   KF_Time_Update_2link(calib, f_reversing, f_update_valid, f_radar_left_side, elapsed_time, host_speed, host_yawrate, ekf_state, ekf_state_errcov);

   /** \result
    * State updated, errcov unchanged
    */
   DOUBLES_EQUAL_TEXT(0.09F, ekf_state[0], epsilon, "x1 should be changed");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[1], ekf_state[1], epsilon, "x2 should be unchanged");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[2], ekf_state[2], epsilon, "x3 should be unchanged");
   DOUBLES_EQUAL_TEXT(0.491F, ekf_state[3], epsilon, "x4 should be changed");
   DOUBLES_EQUAL_TEXT(initial_ekf_state[4], ekf_state[4], epsilon, "x5 should be unchanged");
   CHECK_FALSE(memcmp(&ekf_state[0], &initial_ekf_state[0], sizeof(ekf_state)) == 0);
   CHECK_TRUE(memcmp(&ekf_state_errcov[0][0], &initial_ekf_state_errcov[0][0], sizeof(ekf_state_errcov)) == 0);
}

/** \defgroup apply_state_constraints
 *  @{
 */

/** \brief
 * Test group for Apply_State_Constraints function. Tests cover all branches of the constraint application
 * for each state variable (x0 to x4), ensuring clamping and error covariance adjustment when out of bounds,
 * and no changes when within bounds.
 */
TEST_GROUP(apply_state_constraints_2_link)
{
   F360_CVT_State_Constraints_T state_constraints;
   float32_t KF_state[5];
   float32_t KF_errcov[5][5];
   float32_t original_state[5];
   float32_t original_errcov[5][5];
   float32_t epsilon;

   /** \setup
    * Initialize constraints and state variables to default valid values.
    */
   TEST_SETUP()
   {
      epsilon = 1e-6F;

      // Set up constraints
      state_constraints.max_x0 = F360_DEG2RAD(90.0F);   // Max trailer angle
      state_constraints.min_x0 = F360_DEG2RAD(-90.0F);  // Min trailer angle
      state_constraints.max_x1 = 15.0F;  // Max joint distance
      state_constraints.min_x1 = 1.0F;   // Min joint distance
      state_constraints.max_x2 = 10.0F;  // Max wheelbase
      state_constraints.min_x2 = 0.1F;   // Min wheelbase
      state_constraints.max_x3 = F360_DEG2RAD(90.0F);   // Max trailer angle
      state_constraints.min_x3 = F360_DEG2RAD(-90.0F);  // Min trailer angle
      state_constraints.max_x4 = 10.0F;  // Max wheelbase
      state_constraints.min_x4 = 0.1F;   // Min wheelbase

      // Initialize state within bounds
      KF_state[0] = 0.1F;   // Within min_x0 and max_x0
      KF_state[1] = 5.0F;   // Within min_x1 and max_x1
      KF_state[2] = 8.0F;   // Within min_x2 and max_x2
      KF_state[3] = 0.2F;   // Within min_x3 and max_x3
      KF_state[4] = 9.0F;   // Within min_x4 and max_x4

      // Initialize error covariance with diagonal values
      memset(&KF_errcov[0][0], 0, sizeof(KF_errcov));
      KF_errcov[0][0] = 0.4F;
      KF_errcov[1][1] = 0.9F;
      KF_errcov[2][2] = 0.9F;
      KF_errcov[3][3] = 0.4F;
      KF_errcov[4][4] = 0.9F;

      // Save original state and error covariance for result comparison
      memcpy(&original_state[0], &KF_state[0], sizeof(original_state));
      memcpy(&original_errcov[0][0], &KF_errcov[0][0], sizeof(original_errcov));
   }
};

/** \purpose
 * Test Apply_State_Constraints when all states are within bounds
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, No_State_Change_When_All_States_Within_Bounds)
{
   /** \precond
    * All state values are within their respective bounds, per test group setup
    */

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State and error covariance should remain unchanged
    */
   for (int i = 0; i < 5; i++)
   {
      DOUBLES_EQUAL_TEXT(original_state[i], KF_state[i], epsilon, "State should not change when within bounds");
   }
   for (int i = 0; i < 5; i++)
   {
      DOUBLES_EQUAL_TEXT(original_errcov[i][i], KF_errcov[i][i], epsilon, "Error covariance diagonal should not change");
   }
}

/** \purpose
 * Test Apply_State_Constraints when state[0] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State0_When_It_Exceeds_Max_but_Cov_in_Range_case)
{
   /** \precond
    * State[0] exceeds max_x0
    */
   KF_state[0] = state_constraints.max_x0 + 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[0] should be clamped to max_x0, error covariance[0][0] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x0, KF_state[0], epsilon, "State[0] should be clamped to max");
   DOUBLES_EQUAL_TEXT(original_errcov[0][0] * 1.1F, KF_errcov[0][0], epsilon, "Error covariance[0][0] should increase by 1.1x, capped at 0.5");
}

/** \purpose
 * Test Apply_State_Constraints when state[0] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State0_When_It_Below_Min_but_Cov_in_Range_case)
{
   /** \precond
    * State[0] below min_x0
    */
   KF_state[0] = state_constraints.min_x0 - 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[0] should be clamped to min_x0, error covariance[0][0] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x0, KF_state[0], epsilon, "State[0] should be clamped to min");
   DOUBLES_EQUAL_TEXT(original_errcov[0][0] * 1.1F, KF_errcov[0][0], epsilon, "Error covariance[0][0] should increase by 1.1x, capped at 0.5");
}

/** \purpose
 * Test Apply_State_Constraints when state[1] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State1_When_It_Exceeds_Max_but_Cov_in_Range_case)
{
   /** \precond
    * State[1] exceeds max_x1
    */
   KF_state[1] = state_constraints.max_x1 + 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[1] should be clamped to max_x1, error covariance[1][1] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x1, KF_state[1], epsilon, "State[1] should be clamped to max");
   DOUBLES_EQUAL_TEXT(fminf(original_errcov[1][1] * 1.1F, 1.0F), KF_errcov[1][1], epsilon, "Error covariance[1][1] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[1] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State1_When_It_Below_Min_but_Cov_in_Range_case)
{
   /** \precond
    * State[1] below min_x1
    */
   KF_state[1] = state_constraints.min_x1 - 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[1] should be clamped to min_x1, error covariance[1][1] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x1, KF_state[1], epsilon, "State[1] should be clamped to min");
   DOUBLES_EQUAL_TEXT(fminf(original_errcov[1][1] * 1.1F, 1.0F), KF_errcov[1][1], epsilon, "Error covariance[1][1] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[2] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State2_When_It_Exceeds_Max_but_Cov_in_Range_case)
{
   /** \precond
    * State[2] exceeds max_x2
    */
   KF_state[2] = state_constraints.max_x2 + 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[2] should be clamped to max_x2, error covariance[2][2] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x2, KF_state[2], epsilon, "State[2] should be clamped to max");
   DOUBLES_EQUAL_TEXT(fminf(original_errcov[2][2] * 1.1F, 1.0F), KF_errcov[2][2], epsilon, "Error covariance[2][2] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[2] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State2_When_It_Below_Min_but_Cov_in_Range_case)
{
   /** \precond
    * State[2] below min_x2
    */
   KF_state[2] = state_constraints.min_x2 - 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[2] should be clamped to min_x2, error covariance[2][2] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x2, KF_state[2], epsilon, "State[2] should be clamped to min");
   DOUBLES_EQUAL_TEXT(fminf(original_errcov[2][2] * 1.1F, 1.0F), KF_errcov[2][2], epsilon, "Error covariance[2][2] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[3] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State3_When_It_Exceeds_Max_but_Cov_in_Range_case)
{
   /** \precond
    * State[3] exceeds max_x3
    */
   KF_state[3] = state_constraints.max_x3 + 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[3] should be clamped to max_x3, error covariance[3][3] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x3, KF_state[3], epsilon, "State[3] should be clamped to max");
   DOUBLES_EQUAL_TEXT(fminf(original_errcov[3][3] * 1.1F, 1.0F), KF_errcov[3][3], epsilon, "Error covariance[3][3] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[3] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State3_When_It_Below_Min_but_Cov_in_Range_case)
{
   /** \precond
    * State[3] below min_x3
    */
   KF_state[3] = state_constraints.min_x3 - 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[3] should be clamped to min_x3, error covariance[3][3] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x3, KF_state[3], epsilon, "State[3] should be clamped to min");
   DOUBLES_EQUAL_TEXT(fminf(original_errcov[3][3] * 1.1F, 1.0F), KF_errcov[3][3], epsilon, "Error covariance[3][3] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[4] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State4_When_It_Exceeds_Max_but_Cov_in_Range_case)
{
   /** \precond
    * State[4] exceeds max_x4
    */
   KF_state[4] = state_constraints.max_x4 + 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[4] should be clamped to max_x4, error covariance[4][4] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x4, KF_state[4], epsilon, "State[4] should be clamped to max");
   DOUBLES_EQUAL_TEXT(fminf(original_errcov[4][4] * 1.1F, 1.0F), KF_errcov[4][4], epsilon, "Error covariance[4][4] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[4] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State4_When_It_Below_Min_but_Cov_in_Range_case)
{
   /** \precond
    * State[4] below min_x4
    */
   KF_state[4] = state_constraints.min_x4 - 0.1F;

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[4] should be clamped to min_x4, error covariance[4][4] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x4, KF_state[4], epsilon, "State[4] should be clamped to min");
   DOUBLES_EQUAL_TEXT(fminf(original_errcov[4][4] * 1.1F, 1.0F), KF_errcov[4][4], epsilon, "Error covariance[4][4] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[0] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State0_When_It_Exceeds_Max_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[0] exceeds max_x0
    */
   KF_state[0] = state_constraints.max_x0 + 0.1F;
   KF_errcov[0][0] = 0.47F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[0] should be clamped to max_x0, error covariance[0][0] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x0, KF_state[0], epsilon, "State[0] should be clamped to max");
   DOUBLES_EQUAL_TEXT(0.5F, KF_errcov[0][0], epsilon, "Error covariance[0][0] should increase by 1.1x, capped at 0.5");
}

/** \purpose
 * Test Apply_State_Constraints when state[0] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State0_When_It_Below_Min_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[0] below min_x0
    */
   KF_state[0] = state_constraints.min_x0 - 0.1F;
   KF_errcov[0][0] = 0.47F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[0] should be clamped to min_x0, error covariance[0][0] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x0, KF_state[0], epsilon, "State[0] should be clamped to min");
   DOUBLES_EQUAL_TEXT(0.5F, KF_errcov[0][0], epsilon, "Error covariance[0][0] should increase by 1.1x, capped at 0.5");
}

/** \purpose
 * Test Apply_State_Constraints when state[1] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State1_When_It_Exceeds_Max_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[1] exceeds max_x1
    */
   KF_state[1] = state_constraints.max_x1 + 0.1F;
   KF_errcov[1][1] = 0.92F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[1] should be clamped to max_x1, error covariance[1][1] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x1, KF_state[1], epsilon, "State[1] should be clamped to max");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[1][1], epsilon, "Error covariance[1][1] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[1] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State1_When_It_Below_Min_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[1] below min_x1
    */
   KF_state[1] = state_constraints.min_x1 - 0.1F;
   KF_errcov[1][1] = 0.92F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[1] should be clamped to min_x1, error covariance[1][1] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x1, KF_state[1], epsilon, "State[1] should be clamped to min");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[1][1], epsilon, "Error covariance[1][1] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[2] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State2_When_It_Exceeds_Max_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[2] exceeds max_x2
    */
   KF_state[2] = state_constraints.max_x2 + 0.1F;
   KF_errcov[2][2] = 0.92F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[2] should be clamped to max_x2, error covariance[2][2] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x2, KF_state[2], epsilon, "State[2] should be clamped to max");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[2][2], epsilon, "Error covariance[2][2] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[2] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State2_When_It_Below_Min_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[2] below min_x2
    */
   KF_state[2] = state_constraints.min_x2 - 0.1F;
   KF_errcov[2][2] = 0.92F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[2] should be clamped to min_x2, error covariance[2][2] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x2, KF_state[2], epsilon, "State[2] should be clamped to min");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[2][2], epsilon, "Error covariance[2][2] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[3] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State3_When_It_Exceeds_Max_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[3] exceeds max_x3
    */
   KF_state[3] = state_constraints.max_x3 + 0.1F;
   KF_errcov[3][3] = 0.92F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[3] should be clamped to max_x3, error covariance[3][3] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x3, KF_state[3], epsilon, "State[3] should be clamped to max");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[3][3], epsilon, "Error covariance[3][3] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[3] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State3_When_It_Below_Min_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[3] below min_x3
    */
   KF_state[3] = state_constraints.min_x3 - 0.1F;
   KF_errcov[3][3] = 0.92F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[3] should be clamped to min_x3, error covariance[3][3] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x3, KF_state[3], epsilon, "State[3] should be clamped to min");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[3][3], epsilon, "Error covariance[3][3] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[4] exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State4_When_It_Exceeds_Max_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[4] exceeds max_x4
    */
   KF_state[4] = state_constraints.max_x4 + 0.1F;
   KF_errcov[4][4] = 0.92F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[4] should be clamped to max_x4, error covariance[4][4] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.max_x4, KF_state[4], epsilon, "State[4] should be clamped to max");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[4][4], epsilon, "Error covariance[4][4] should increase by 1.1x, capped at 1.0");
}

/** \purpose
 * Test Apply_State_Constraints when state[4] below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints_2_link, Clamp_State4_When_It_Below_Min_and_Cov_Exceeds_Bound_case)
{
   /** \precond
    * State[4] below min_x4
    */
   KF_state[4] = state_constraints.min_x4 - 0.1F;
   KF_errcov[4][4] = 0.92F;  // Error covariance will exceed bound after adjustment

   /** \action
    * Apply state constraints
    */
   Apply_State_Constraints(state_constraints, KF_state, KF_errcov);

   /** \result
    * State[4] should be clamped to min_x4, error covariance[4][4] adjusted
    */
   DOUBLES_EQUAL_TEXT(state_constraints.min_x4, KF_state[4], epsilon, "State[4] should be clamped to min");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[4][4], epsilon, "Error covariance[4][4] should increase by 1.1x, capped at 1.0");
}
/** @}*/
