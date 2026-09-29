/** \file
 * This file contains unit tests for content of f360_cvt_one_link_ekf.cpp file
 */

#include "f360_cvt_one_link_ekf.h"
#include "f360_reuse.h"
#include "f360_math_func.h"
#include <CppUTest/TestHarness.h>
#include <cmath>
#include <vector>
#include <cstring>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

//=============================================================================
// Tests for Calc_Jacobian
//=============================================================================


/** \defgroup compute_jacobian
 *  @{
 */

/** \brief
 * Test group for Calc_Jacobian functions. It contains the minimal set of variables needed for running that function.
 */
TEST_GROUP(compute_jacobian)
{
   float32_t t;  // time elapsed
   float32_t host_speed;  // m/s
   float32_t host_yawrate;  // rad/s
   float32_t epsilon;
   float32_t x[3] = {};  // trailer angle, joint dist, wheelbase

   /** \setup
    * Initialize all input and state variables to default valid values.
    */
   TEST_SETUP()
   {
      t = 0.05F;  // 50ms
      host_speed = 2.0F;  // m/s
      host_yawrate = 0.1F;  // rad/s
      epsilon = 1E-3F;
      x[0] = 0.1F;
      x[1] = 5.0F;
      x[2] = 8.0F;
   }
};

/** \purpose
 * Test Calc_Jacobian calculation with typical values
 * \req
 * NA
 */
TEST(compute_jacobian, Jacobian_Computed_Correctly_for_Typical_Forward_Case)
{
   /** \precond
    * Forward motion with small angles and typical parameter values
    */
   float32_t F[3][3] = {};
   epsilon = 1E-5F;

   /** \action
    * Calculate Jacobian matrix
    */
   Calc_Jacobian(t, host_speed, host_yawrate, x, F);

   /** \result
    * Check that Jacobian is calculated and has expected structure
    * F[0][0] should depend on cosine/sine terms
    * F[1][1] should be 1.0
    * F[2][2] should be 1.0
    * F[1][0], F[1][2], F[2][0], F[2][1] should be 0.0
    */
   DOUBLES_EQUAL_TEXT(9.8787E-1F, F[0][0], epsilon, "F[0][0] should be 9.8787E-1");
   DOUBLES_EQUAL_TEXT(-6.2E-4F, F[0][1], epsilon, "F[0][1] should be -6.2E-4");
   DOUBLES_EQUAL_TEXT(5.4E-4F, F[0][2], epsilon, "F[0][2] should be 5.4E-4");
   DOUBLES_EQUAL_TEXT(1.0F, F[1][1], epsilon, "F[1][1] should be 1.0");
   DOUBLES_EQUAL_TEXT(1.0F, F[2][2], epsilon, "F[2][2] should be 1.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][0], epsilon, "F[1][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][2], epsilon, "F[1][2] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][0], epsilon, "F[2][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][1], epsilon, "F[2][1] should be 0.0");
}

/** \purpose
 * Test Calc_Jacobian with zero time step
 * \req
 * NA
 */
TEST(compute_jacobian, Jacobian_Correct_when_zero_time_elapsed)
{
   /** \precond
    * Time step is zero
    */
   t = 0.0F;
   float32_t F[3][3] = {};

   /** \action
    * Calculate Jacobian with zero time
    */
   Calc_Jacobian(t, host_speed, host_yawrate, x, F);

   /** \result
    * F[0][0] should be 1.0 (no time variation)
    * Diagonal elements [1][1] and [2][2] should be 1.0
    */
   DOUBLES_EQUAL_TEXT(1.0F, F[0][0], epsilon, "F[0][0] should be 1.0 when t=0");
   DOUBLES_EQUAL_TEXT(0.0F, F[0][1], epsilon, "F[0][1] should be 0.0 when t=0");
   DOUBLES_EQUAL_TEXT(0.0F, F[0][2], epsilon, "F[0][2] should be 0.0 when t=0");
   DOUBLES_EQUAL_TEXT(1.0F, F[1][1], epsilon, "F[1][1] should be 1.0");
   DOUBLES_EQUAL_TEXT(1.0F, F[2][2], epsilon, "F[2][2] should be 1.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][0], epsilon, "F[1][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][2], epsilon, "F[1][2] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][0], epsilon, "F[2][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][1], epsilon, "F[2][1] should be 0.0");
}

/** \purpose
 * Test Calc_Jacobian with large angles
 * \req
 * NA
 */
TEST(compute_jacobian, Jacobian_Correct_With_Large_Angle)
{
   /** \precond
    * Large trailer angle (60 degrees)
    */
   x[0] = F360_DEG2RAD(60.0F);  // Large angle
   float32_t F[3][3] = {};
   epsilon = 1E-5F;

   /** \action
    * Calculate Jacobian with large angles
    */
   Calc_Jacobian(t, host_speed, host_yawrate, x, F);

   /** \result
    * Check basic structure remains valid
    */
   DOUBLES_EQUAL_TEXT(9.9645E-1F, F[0][0], epsilon, "F[0][0] should be 9.964E-1F");
   DOUBLES_EQUAL_TEXT(-3.12E-4F, F[0][1], epsilon, "F[0][1] should be -3.12E-4F");
   DOUBLES_EQUAL_TEXT(1.548E-3F, F[0][2], epsilon, "F[0][2] should be 1.548E-3F");
   DOUBLES_EQUAL_TEXT(1.0F, F[1][1], epsilon, "F[1][1] should be 1.0");
   DOUBLES_EQUAL_TEXT(1.0F, F[2][2], epsilon, "F[2][2] should be 1.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][0], epsilon, "F[1][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][2], epsilon, "F[1][2] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][0], epsilon, "F[2][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][1], epsilon, "F[2][1] should be 0.0");
}

/** \purpose
 * Test Calc_Jacobian calculation with negative angle
 * \req
 * NA
 */
TEST(compute_jacobian, Jacobian_Computed_Correctly_with_Negative_Angle)
{
   /** \precond
    * Forward motion with negative angles and typical parameter values
    */
   float32_t F[3][3] = {};
   epsilon = 1E-6F;
   x[0] = -0.2F;

   /** \action
    * Calculate Jacobian matrix
    */
   Calc_Jacobian(t, host_speed, host_yawrate, x, F);

   /** \result
    * Check that Jacobian is calculated and has expected structure
    * F[0][0] should depend on cosine/sine terms
    * F[1][1] should be 1.0
    * F[2][2] should be 1.0
    * F[1][0], F[1][2], F[2][0], F[2][1] should be 0.0
    */
   DOUBLES_EQUAL_TEXT(9.87128E-1F, F[0][0], epsilon, "F[0][0] should be 9.87128E-1");
   DOUBLES_EQUAL_TEXT(-6.1254E-4F, F[0][1], epsilon, "F[0][1] should be -6.1254E-4");
   DOUBLES_EQUAL_TEXT(7.241E-5F, F[0][2], epsilon, "F[0][2] should be 7.241E-4");
   DOUBLES_EQUAL_TEXT(1.0F, F[1][1], epsilon, "F[1][1] should be 1.0");
   DOUBLES_EQUAL_TEXT(1.0F, F[2][2], epsilon, "F[2][2] should be 1.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][0], epsilon, "F[1][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][2], epsilon, "F[1][2] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][0], epsilon, "F[2][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][1], epsilon, "F[2][1] should be 0.0");
}

/** \purpose
 * Test Calc_Jacobian calculation with negative yaw rate
 * \req
 * NA
 */
TEST(compute_jacobian, Jacobian_Computed_Correctly_with_Negative_Yawrate)
{
   /** \precond
    * Forward motion with negative yawrate and typical state values
    */
   host_yawrate = -0.3F;
   float32_t F[3][3] = {};
   epsilon = 1E-5F;

   /** \action
    * Calculate Jacobian matrix
    */
   Calc_Jacobian(t, host_speed, host_yawrate, x, F);

   /** \result
    * Check that Jacobian is calculated and has expected structure
    * F[0][0] should depend on cosine/sine terms
    * F[1][1] should be 1.0
    * F[2][2] should be 1.0
    * F[1][0], F[1][2], F[2][0], F[2][1] should be 0.0
    */
   DOUBLES_EQUAL_TEXT(9.86626E-1F, F[0][0], epsilon, "F[0][0] should be 9.86626E-1");
   DOUBLES_EQUAL_TEXT(1.8656E-3F, F[0][1], epsilon, "F[0][1] should be 1.8656E-3");
   DOUBLES_EQUAL_TEXT(-1.01E-3F, F[0][2], epsilon, "F[0][2] should be -1.01E-3");
   DOUBLES_EQUAL_TEXT(1.0F, F[1][1], epsilon, "F[1][1] should be 1.0");
   DOUBLES_EQUAL_TEXT(1.0F, F[2][2], epsilon, "F[2][2] should be 1.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][0], epsilon, "F[1][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][2], epsilon, "F[1][2] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][0], epsilon, "F[2][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][1], epsilon, "F[2][1] should be 0.0");
}

/** \purpose
 * Test Calc_Jacobian calculation with negative and low host speed
 * \req
 * NA
 */
TEST(compute_jacobian, Jacobian_Computed_Correctly_with_Negative_Low_host_speed)
{
   /** \precond
    * Forward motion with negative and low speed and typical state values
    */
   host_speed = -0.2F;
   float32_t F[3][3] = {};
   epsilon = 1E-6F;

   /** \action
    * Calculate Jacobian matrix
    */
   Calc_Jacobian(t, host_speed, host_yawrate, x, F);

   /** \result
    * Check that Jacobian is calculated and has expected structure
    * F[0][0] should depend on cosine/sine terms
    * F[1][1] should be 1.0
    * F[2][2] should be 1.0
    * F[1][0], F[1][2], F[2][0], F[2][1] should be 0.0
    */
   DOUBLES_EQUAL_TEXT(1.001555F, F[0][0], epsilon, "F[0][0] should be 1.00155");
   DOUBLES_EQUAL_TEXT(-6.218E-4F, F[0][1], epsilon, "F[0][1] should be -6.218E-4");
   DOUBLES_EQUAL_TEXT(3.73E-4F, F[0][2], epsilon, "F[0][2] should be 3.73E-4");
   DOUBLES_EQUAL_TEXT(1.0F, F[1][1], epsilon, "F[1][1] should be 1.0");
   DOUBLES_EQUAL_TEXT(1.0F, F[2][2], epsilon, "F[2][2] should be 1.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][0], epsilon, "F[1][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[1][2], epsilon, "F[1][2] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][0], epsilon, "F[2][0] should be 0.0");
   DOUBLES_EQUAL_TEXT(0.0F, F[2][1], epsilon, "F[2][1] should be 0.0");
}

/** @}*/

//=============================================================================
// Tests for KF_Time_Update
//=============================================================================

/** \defgroup kf_time_update
 * @{
 */

/** \brief
 * Test group for KF_Time_Update functions. It contains the minimal set of variables needed for running that function.
 */

TEST_GROUP(kf_time_update)
{
   bool f_valid_msmt;
   float32_t elapsed_time;
   float32_t host_speed;
   float32_t host_yawrate;
   float32_t epsilon;
   F360_CVT_State_T cvt_state;
   float32_t x[3] = {};  // trailer angle, joint dist, wheelbase
   F360_Calibrations_T calib{};

   /** \setup
    * Initialize all input and state variables to default valid values.
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);

      f_valid_msmt = true;
      elapsed_time = 0.05F;
      host_speed = 2.0F;
      host_yawrate = 0.1F;
      epsilon = 1E-3F;
      x[0] = 0.1F;
      x[1] = 5.0F;
      x[2] = 8.0F;

      // Initialize state data
      memset(&cvt_state, 0, sizeof(cvt_state));

      // Initialize error covariance with diagonal matrix
      for (int32_t i = 0; i < 3; i++)
      {
         for (int32_t j = 0; j < 3; j++)
         {
            cvt_state.one_link.ekf_state_errcov[i][j] = (i == j) ? 0.5F : 0.01F;
         }
      }
   }
};

/** \purpose
 * Test time update during forward motion with valid measurement
 * \req
 * NA
 */
TEST(kf_time_update, KF_Time_Updated_Correctly_Forward_With_Measurement)
{
   /** \precond
    * Vehicle moving forward at typical speed
    * Valid measurement available
    */
   float32_t P[3][3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));

   /** \action
    * Execute time update
    */
   KF_Time_Update(calib, f_valid_msmt, elapsed_time, host_speed, host_yawrate, x, P);

   /** \result
    * Check that state was updated
    * Error covariance should be increased
    */
   DOUBLES_EQUAL_TEXT(9.06E-2F, x[0], epsilon, "x[0] should be changed");
   DOUBLES_EQUAL_TEXT(5.0F, x[1], epsilon, "x[1] should be unchanged");
   DOUBLES_EQUAL_TEXT(8.0F, x[2], epsilon, "x[2] should be unchanged");
   DOUBLES_EQUAL_TEXT(2.487F, P[0][0], epsilon, "P[0][0] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(9.57E-3F, P[0][1], epsilon, "P[0][1] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(1.01E-2F, P[0][2], epsilon, "P[0][2] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(9.57E-3F, P[1][0], epsilon, "P[1][0] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(5.01E-1F, P[1][1], epsilon, "P[1][1] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(1E-2F, P[1][2], epsilon, "P[1][2] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(1.01E-2F, P[2][0], epsilon, "P[2][0] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(1E-2F, P[2][1], epsilon, "P[2][1] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(5.1E-1F, P[2][2], epsilon, "P[2][2] should have been updated correctly");
}

/** \purpose
 * Test time update during forward motion without measurement
 * \req
 * NA
 */
TEST(kf_time_update, KF_Time_Updated_Correctly_Forward_Without_Measurement)
{
   /** \precond
    * Vehicle moving forward, but no valid measurement
    */
   bool f_valid_msmt = false;
   float32_t P[3][3];
   float32_t P_prev[3][3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));
   (void)memcpy(&P_prev[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P_prev));

   /** \action
    * Execute time update without measurement
    */
   KF_Time_Update(calib, f_valid_msmt, elapsed_time, host_speed, host_yawrate, x, P);

   /** \result
    * State should be updated
    * Error covariance should remain unchanged (no measurement, no P update)
    */
   DOUBLES_EQUAL_TEXT(9.06E-2F, x[0], epsilon, "x[0] should be changed");
   DOUBLES_EQUAL_TEXT(5.0F, x[1], epsilon, "x[1] should be unchanged");
   DOUBLES_EQUAL_TEXT(8.0F, x[2], epsilon, "x[2] should be unchanged");

   for (int32_t row = 0; row < 3; row++)
   {
      for (int32_t col = 0; col < 3; col++)
      {
         std::string message = "The value of P[" + std::to_string(row) + "][" + std::to_string(col) + "] should be unchanged.";
         DOUBLES_EQUAL_TEXT(P_prev[row][col], P[row][col], epsilon, message.c_str());
      }
   }
}

/** \purpose
 * Test time update during reversing motion
 * \req
 * NA
 */
TEST(kf_time_update, KF_Time_Updated_Correctly_Reversing)
{
   /** \precond
    * Vehicle reversing (speed < -0.5)
    * Valid measurement available
    */
   host_speed = -1.0F;  // Reversing
   x[0] = 0.2F;
   float32_t P[3][3];
   float32_t P_prev[3][3];
   float32_t prev_x[3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));
   (void)memcpy(&P_prev[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P_prev));
   (void)memcpy(&prev_x[0], &x[0], sizeof(prev_x));

   /** \action
    * Execute time update during reversing
    */
   KF_Time_Update(calib, f_valid_msmt, elapsed_time, host_speed, host_yawrate, x, P);

   /** \result
    * State should be multiplied by 0.9 (reversing gain)
    * Expected x[0] = 0.2 * 0.9 = 0.18
    */
   DOUBLES_EQUAL_TEXT(prev_x[0] * 0.9F, x[0], epsilon, "State 0 should be reduced by factor 0.9 during reversing");
   DOUBLES_EQUAL_TEXT(prev_x[1], x[1], epsilon, "State 1 should stay unchanged during reversing");
   DOUBLES_EQUAL_TEXT(prev_x[2], x[2], epsilon, "State 2 should stay unchanged during reversing");

   for (int32_t row = 0; row < 3; row++)
   {
      for (int32_t col = 0; col < 3; col++)
      {
         std::string message = "The value of P[" + std::to_string(row) + "][" + std::to_string(col) + "] should be unchanged.";
         DOUBLES_EQUAL_TEXT(P_prev[row][col], P[row][col], epsilon, message.c_str());
      }
   }
}

/** \purpose
 * Test time update in stationary/near-zero speed condition
 * \req
 * NA
 */
TEST(kf_time_update, KF_Time_Updated_Correctly_Stationary)
{
   /** \precond
    * Vehicle stationary (speed < 0.1)
    */
   host_speed = 0.09F;  // Stationary (below threshold 0.1)
   float32_t P[3][3];
   float32_t P_prev[3][3];
   float32_t prev_x[3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));
   (void)memcpy(&P_prev[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P_prev));
   (void)memcpy(&prev_x[0], &x[0], sizeof(prev_x));

   /** \action
    * Execute time update when stationary
    */
   KF_Time_Update(calib, f_valid_msmt, elapsed_time, host_speed, host_yawrate, x, P);

   /** \result
    * Neither state nor covariance should change
    */
   DOUBLES_EQUAL_TEXT(prev_x[0], x[0], epsilon, "State 0 should stay unchanged if stationary");
   DOUBLES_EQUAL_TEXT(prev_x[1], x[1], epsilon, "State 1 should stay unchanged if stationary");
   DOUBLES_EQUAL_TEXT(prev_x[2], x[2], epsilon, "State 2 should stay unchanged if stationary");

   for (int32_t row = 0; row < 3; row++)
   {
      for (int32_t col = 0; col < 3; col++)
      {
         std::string message = "The value of P[" + std::to_string(row) + "][" + std::to_string(col) + "] should be unchanged.";
         DOUBLES_EQUAL_TEXT(P_prev[row][col], P[row][col], epsilon, message.c_str());
      }
   }
}

/** \purpose
 * Test time update with very slow forward speed (near threshold)
 * \req
 * NA
 */
TEST(kf_time_update, KF_Time_Updated_Correctly_Slow_Forward_Motion)
{
   /** \precond
    * Vehicle moving forward but very slowly (0.11 m/s, just above threshold of 0.1)
    */
   host_speed = 0.11F;  // Just above threshold
   float32_t P[3][3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));

   /** \action
    * Execute time update at slow forward speed
    */
   KF_Time_Update(calib, f_valid_msmt, elapsed_time, host_speed, host_yawrate, x, P);

   /** \result
    * State should be updated (forward case)
    */
   DOUBLES_EQUAL_TEXT(9.18E-2F, x[0], epsilon, "x[0] should be changed");
   DOUBLES_EQUAL_TEXT(5.0F, x[1], epsilon, "x[1] should be unchanged");
   DOUBLES_EQUAL_TEXT(8.0F, x[2], epsilon, "x[2] should be unchanged");
   DOUBLES_EQUAL_TEXT(2.499F, P[0][0], epsilon, "P[0][0] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(9.68E-3F, P[0][1], epsilon, "P[0][1] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(1.01E-2F, P[0][2], epsilon, "P[0][2] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(9.68E-3F, P[1][0], epsilon, "P[1][0] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(5.01E-1F, P[1][1], epsilon, "P[1][1] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(1E-2F, P[1][2], epsilon, "P[1][2] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(1.01E-2F, P[2][0], epsilon, "P[2][0] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(1E-2F, P[2][1], epsilon, "P[2][1] should have been updated correctly");
   DOUBLES_EQUAL_TEXT(5.1E-1F, P[2][2], epsilon, "P[2][2] should have been updated correctly");
}

/** \purpose
 * Test time update between forward and reversing thresholds
 * \req
 * NA
 */
TEST(kf_time_update, KF_Time_Updated_Correctly_Reversing_Slowly)
{
   /** \precond
    * Vehicle speed between forward threshold (0.1) and reversing threshold (-0.5)
    * This is the "stationary" case
    */
   host_speed = -0.4F;  // Between thresholds (not reversing yet)
   float32_t P[3][3];
   float32_t P_prev[3][3];
   float32_t prev_x[3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));
   (void)memcpy(&P_prev[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P_prev));
   (void)memcpy(&prev_x[0], &x[0], sizeof(prev_x));

   /** \action
    * Execute time update with speed between thresholds
    */
   KF_Time_Update(calib, f_valid_msmt, elapsed_time, host_speed, host_yawrate, x, P);

   /** \result
    * Neither state nor covariance should change (stationary branch)
    */
   DOUBLES_EQUAL_TEXT(prev_x[0], x[0], epsilon, "State 0 should stay unchanged if stationary");
   DOUBLES_EQUAL_TEXT(prev_x[1], x[1], epsilon, "State 1 should stay unchanged if stationary");
   DOUBLES_EQUAL_TEXT(prev_x[2], x[2], epsilon, "State 2 should stay unchanged if stationary");

   for (int32_t row = 0; row < 3; row++)
   {
      for (int32_t col = 0; col < 3; col++)
      {
         std::string message = "The value of P[" + std::to_string(row) + "][" + std::to_string(col) + "] should be unchanged.";
         DOUBLES_EQUAL_TEXT(P_prev[row][col], P[row][col], epsilon, message.c_str());
      }
   }
}
/** @}*/

//=============================================================================
// Tests for KF_Measurement_Update
//=============================================================================

/** \defgroup kf_msmt_update
 *  @{
 */

/** \brief
 * Test group for KF_Measurement_Update functions. It contains the minimal set of variables needed for running that function.
 */
TEST_GROUP(kf_msmt_update)
{
   float32_t angle_msmt;
   float32_t intersect_msmt;
   float32_t x[3];
   float32_t epsilon;
   F360_CVT_State_T cvt_state;

   /** \setup
    * Initialize all input and state variables to default valid values.
    */
   TEST_SETUP()
   {
      angle_msmt = 0.15F;
      intersect_msmt = 6.5F;
      x[0] = 0.1F;
      x[1] = 5.0F;
      x[2] = 8.0F;
      epsilon = 1E-3F;

      // Initialize state data
      memset(&cvt_state, 0, sizeof(cvt_state));

      // Initialize error covariance with diagonal matrix
      for (int32_t i = 0; i < 3; i++)
      {
         for (int32_t j = 0; j < 3; j++)
         {
            cvt_state.one_link.ekf_state_errcov[i][j] = (i == j) ? 0.5F : 0.01F;
         }
      }
   }
};

/** \purpose
 * Test measurement update with typical measurement values
 * \req
 * NA
 */
TEST(kf_msmt_update, Measurement_Update_Correctly_with_NonZero_innovation)
{
   /** \precond
    * Typical angle and intersect measurements
    */
   angle_msmt = 0.15F;
   intersect_msmt = 6.5F;
   float32_t P[3][3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));
   epsilon = 1E-4F;

   /** \action
    * Execute measurement update
    */
   KF_Measurement_Update(angle_msmt, intersect_msmt, x, P);

   /** \result
    * Check that state was updated
    * All three state components should potentially change based on Kalman gain
    */
   DOUBLES_EQUAL_TEXT(1.0086E-1F, x[0], epsilon, "x[0] should be changed when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(5.01852F, x[1], epsilon, "x[1] should be changed when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(8.0003F, x[2], epsilon, "x[2] should be changed when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(4.9504E-1F, P[0][0], epsilon, "P[0][0] should have been updated correctly when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(9.778E-3F, P[0][1], epsilon, "P[0][1] should have been updated correctly when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(9.898E-3F, P[0][2], epsilon, "P[0][2] should have been updated correctly when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(9.778E-3F, P[1][0], epsilon, "P[1][0] should have been updated correctly when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(4.938E-1F, P[1][1], epsilon, "P[1][1] should have been updated correctly when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(9.87E-3F, P[1][2], epsilon, "P[1][2] should have been updated correctly when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(9.89E-3F, P[2][0], epsilon, "P[2][0] should have been updated correctly when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(9.87E-3F, P[2][1], epsilon, "P[2][1] should have been updated correctly when measurement is not trivial");
   DOUBLES_EQUAL_TEXT(4.999E-1F, P[2][2], epsilon, "P[2][2] should have been updated correctly when measurement is not trivial");
}

/** \purpose
 * Test measurement update with exact state equal to measurement
 * \req
 * NA
 */
TEST(kf_msmt_update, Measurement_Update_Correctly_with_Zero_innovation)
{
   /** \precond
    * Measurement exactly matches current state estimate (no innovation)
    */
   angle_msmt = 0.1F;   // Same as initial x[0]
   intersect_msmt = 5.0F;  // Same as initial x[1]
   float32_t P[3][3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));
   epsilon = 1E-4F;
   /** \action
    * Execute measurement update with zero innovation
    */
   KF_Measurement_Update(angle_msmt, intersect_msmt, x, P);

   /** \result
    * State should remain close to original
    * Error covariance should be reduced and independent of innovation
    */
   DOUBLES_EQUAL_TEXT(1E-1F, x[0], epsilon, "x[0] should be unchanged when innovation is zero");
   DOUBLES_EQUAL_TEXT(5.0F, x[1], epsilon, "x[1] should be unchanged when innovation is zero");
   DOUBLES_EQUAL_TEXT(8.0F, x[2], epsilon, "x[2] should be unchanged when innovation is zero");
   DOUBLES_EQUAL_TEXT(4.9504E-1F, P[0][0], epsilon, "P[0][0] should have been updated correctly, despite of zero innovation");
   DOUBLES_EQUAL_TEXT(9.778E-3F, P[0][1], epsilon, "P[0][1] should have been updated correctly, despite of zero innovation");
   DOUBLES_EQUAL_TEXT(9.898E-3F, P[0][2], epsilon, "P[0][2] should have been updated correctly, despite of zero innovation");
   DOUBLES_EQUAL_TEXT(9.778E-3F, P[1][0], epsilon, "P[1][0] should have been updated correctly, despite of zero innovation");
   DOUBLES_EQUAL_TEXT(4.938E-1F, P[1][1], epsilon, "P[1][1] should have been updated correctly, despite of zero innovation");
   DOUBLES_EQUAL_TEXT(9.87E-3F, P[1][2], epsilon, "P[1][2] should have been updated correctly, despite of zero innovation");
   DOUBLES_EQUAL_TEXT(9.89E-3F, P[2][0], epsilon, "P[2][0] should have been updated correctly, despite of zero innovation");
   DOUBLES_EQUAL_TEXT(9.87E-3F, P[2][1], epsilon, "P[2][1] should have been updated correctly, despite of zero innovation");
   DOUBLES_EQUAL_TEXT(4.999E-1F, P[2][2], epsilon, "P[2][2] should have been updated correctly, despite of zero innovation");
}

/** \purpose
 * Test measurement update with large measurement deviation
 * \req
 * NA
 */
TEST(kf_msmt_update, Measurement_Update_Correctly_with_Large_innovation)
{
   /** \precond
    * Measurement significantly different from current state
    */
   angle_msmt = 0.5F;   // Larger deviation
   intersect_msmt = 8.0F;  // Larger deviation
   float32_t P[3][3];
   (void)memcpy(&P[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(P));

   /** \action
    * Execute measurement update with large deviation
    */
   KF_Measurement_Update(angle_msmt, intersect_msmt, x, P);

   /** \result
    * State should be updated significantly, i.e., no saturation
    * Check x and P 
    */
   DOUBLES_EQUAL_TEXT(1.04E-1F, x[0], epsilon, "x[0] should be changed accordingly when innovation is large");
   DOUBLES_EQUAL_TEXT(5.037F, x[1], epsilon, "x[1] should be changed accordingly when innovation is large");
   DOUBLES_EQUAL_TEXT(8.0F, x[2], epsilon, "x[2] should be changed accordingly when innovation is large");
   DOUBLES_EQUAL_TEXT(4.950E-1F, P[0][0], epsilon, "P[0][0] should have been updated correctly, despite of large innovation");
   DOUBLES_EQUAL_TEXT(9.778E-3F, P[0][1], epsilon, "P[0][1] should have been updated correctly, despite of large innovation");
   DOUBLES_EQUAL_TEXT(9.898E-3F, P[0][2], epsilon, "P[0][2] should have been updated correctly, despite of large innovation");
   DOUBLES_EQUAL_TEXT(9.778E-3F, P[1][0], epsilon, "P[1][0] should have been updated correctly, despite of large innovation");
   DOUBLES_EQUAL_TEXT(4.938E-1F, P[1][1], epsilon, "P[1][1] should have been updated correctly, despite of large innovation");
   DOUBLES_EQUAL_TEXT(9.874E-3F, P[1][2], epsilon, "P[1][2] should have been updated correctly, despite of large innovation");
   DOUBLES_EQUAL_TEXT(9.898E-3F, P[2][0], epsilon, "P[2][0] should have been updated correctly, despite of large innovation");
   DOUBLES_EQUAL_TEXT(9.874E-3F, P[2][1], epsilon, "P[2][1] should have been updated correctly, despite of large innovation");
   DOUBLES_EQUAL_TEXT(4.999E-1F, P[2][2], epsilon, "P[2][2] should have been updated correctly, despite of large innovation");
}
/** @}*/

//=============================================================================
// Tests for Apply_State_Constraints
//=============================================================================

/** \defgroup apply_state_constraints
 *  @{
 */

/** \brief
 * Test group for Apply_State_Constraints functions. It contains the minimal set of variables needed for running that function.
 */
TEST_GROUP(apply_state_constraints)
{
   float32_t epsilon;
   F360_CVT_State_T cvt_state;

   /** \setup
    * Initialize all input and state variables to default valid values.
    */
   TEST_SETUP()
   {
      epsilon = 1E-3F;

      // Initialize state data
      memset(&cvt_state, 0, sizeof(cvt_state));

      // Set up initial ekf_state with reasonable values
      cvt_state.one_link.ekf_state[0] = 0.1F;   // x0: trailer angle (rad)
      cvt_state.one_link.ekf_state[1] = 5.0F;   // x1: rear axle to joint (m)
      cvt_state.one_link.ekf_state[2] = 8.0F;   // x2: 1/(joint2wheels) [1/m]

      // Initialize error covariance with diagonal matrix
      for (int32_t i = 0; i < 3; i++)
      {
         for (int32_t j = 0; j < 3; j++)
         {
            cvt_state.one_link.ekf_state_errcov[i][j] = (i == j) ? 0.5F : 0.01F;
         }
      }

      // Set up constraints
      cvt_state.state_constraints.max_x0 = F360_DEG2RAD(90.0F);   // Max trailer angle
      cvt_state.state_constraints.min_x0 = F360_DEG2RAD(-90.0F);  // Min trailer angle
      cvt_state.state_constraints.max_x1 = 15.0F;  // Max joint distance
      cvt_state.state_constraints.min_x1 = 1.0F;   // Min joint distance
      cvt_state.state_constraints.max_x2 = 10.0F;  // Max wheelbase
      cvt_state.state_constraints.min_x2 = 0.1F;   // Min wheelbase
   }
};

/** \purpose
 * Test constraint application when state is within bounds
 * \req
 * NA
 */
TEST(apply_state_constraints, No_State_Saturation_If_Nothing_Exceeds_Limits)
{
   /** \precond
    * All state values within constraints
    */
   float32_t KF_state[3] = { cvt_state.state_constraints.max_x0 - 0.001F, cvt_state.state_constraints.max_x1 - 0.1F, cvt_state.state_constraints.max_x2 - 0.1F };  // all states are inrange
   float32_t KF_errcov[3][3];
   cvt_state.one_link.ekf_state_errcov[0][0] = 0.4F;
   cvt_state.one_link.ekf_state_errcov[1][1] = 0.9F;
   cvt_state.one_link.ekf_state_errcov[2][2] = 0.9F;
   (void)memcpy(&KF_errcov[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(KF_errcov));
   float32_t initial_state0 = KF_state[0];
   float32_t initial_state1 = KF_state[1];
   float32_t initial_state2 = KF_state[2];
   float32_t initial_errcov00 = KF_errcov[0][0];
   float32_t initial_errcov11 = KF_errcov[1][1];
   float32_t initial_errcov22 = KF_errcov[2][2];

   /** \action
    * Apply constraints
    */
   Apply_State_Constraints(cvt_state.state_constraints, KF_state, KF_errcov);

   /** \result
    * State should not change
    * Error covariance should not change
    */
   DOUBLES_EQUAL_TEXT(initial_state0, KF_state[0], epsilon, "State[0] should not change");
   DOUBLES_EQUAL_TEXT(initial_state1, KF_state[1], epsilon, "State[1] should not change");
   DOUBLES_EQUAL_TEXT(initial_state2, KF_state[2], epsilon, "State[2] should not change");
   DOUBLES_EQUAL_TEXT(initial_errcov00, KF_errcov[0][0], epsilon, "Errcov[0][0] should not change");
   DOUBLES_EQUAL_TEXT(initial_errcov11, KF_errcov[1][1], epsilon, "Errcov[1][1] should not change");
   DOUBLES_EQUAL_TEXT(initial_errcov22, KF_errcov[2][2], epsilon, "Errcov[2][2] should not change");
}

/** \purpose
 * Test constraint application when states are within bounds but covariance exceeds limits
 * \req
 * NA
 */
TEST(apply_state_constraints, No_State_Saturation_if_only_Covariance_Exceeds_Limits)
{
   /** \precond
    * All state values within constraints
    */
   float32_t KF_state[3] = { cvt_state.state_constraints.max_x0 - 0.001F, cvt_state.state_constraints.max_x1 - 0.1F, cvt_state.state_constraints.max_x2 - 0.1F };  // all states are inrange
   cvt_state.one_link.ekf_state_errcov[0][0] = 0.6F;
   cvt_state.one_link.ekf_state_errcov[1][1] = 1.1F;
   cvt_state.one_link.ekf_state_errcov[2][2] = 1.1F;
   float32_t KF_errcov[3][3];
   (void)memcpy(&KF_errcov[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(KF_errcov));
   float32_t initial_state0 = KF_state[0];
   float32_t initial_state1 = KF_state[1];
   float32_t initial_state2 = KF_state[2];
   float32_t initial_errcov00 = KF_errcov[0][0];
   float32_t initial_errcov11 = KF_errcov[1][1];
   float32_t initial_errcov22 = KF_errcov[2][2];

   /** \action
    * Apply constraints
    */
   Apply_State_Constraints(cvt_state.state_constraints, KF_state, KF_errcov);

   /** \result
    * State should not change
    * Error covariance should not change
    */
   DOUBLES_EQUAL_TEXT(initial_state0, KF_state[0], epsilon, "State[0] should not change");
   DOUBLES_EQUAL_TEXT(initial_state1, KF_state[1], epsilon, "State[1] should not change");
   DOUBLES_EQUAL_TEXT(initial_state2, KF_state[2], epsilon, "State[2] should not change");
   DOUBLES_EQUAL_TEXT(initial_errcov00, KF_errcov[0][0], epsilon, "Errcov[0][0] should not change");
   DOUBLES_EQUAL_TEXT(initial_errcov11, KF_errcov[1][1], epsilon, "Errcov[1][1] should not change");
   DOUBLES_EQUAL_TEXT(initial_errcov22, KF_errcov[2][2], epsilon, "Errcov[2][2] should not change");
}

/** \purpose
 * Test constraint application when states exceed bounds and covariance also exceeds limits
 * \req
 * NA
 */
TEST(apply_state_constraints, Saturate_Covariance_if_it_Also_Exceeds_Limits_When_States_Exceed)
{
   /** \precond
    * All states exceed their respective maximum constraints and covariance exceeds limits
    */
   float32_t KF_state[3] = { cvt_state.state_constraints.max_x0 + 0.001F, cvt_state.state_constraints.max_x1 + 0.1F, cvt_state.state_constraints.max_x2 + 0.1F };  // all states are inrange
   cvt_state.one_link.ekf_state_errcov[0][0] = 0.6F;
   cvt_state.one_link.ekf_state_errcov[1][1] = 1.1F;
   cvt_state.one_link.ekf_state_errcov[2][2] = 1.1F;
   float32_t KF_errcov[3][3];
   (void)memcpy(&KF_errcov[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(KF_errcov));

   /** \action
    * Apply constraints
    */
   Apply_State_Constraints(cvt_state.state_constraints, KF_state, KF_errcov);

   /** \result
    * State should be clamped
    * Error covariance should be clamped
    */
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.max_x0, KF_state[0], epsilon,
                     "State[0] should be clamped to max");
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.max_x1, KF_state[1], epsilon,
                     "State[1] should be clamped to max");
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.max_x2, KF_state[2], epsilon,
                     "State[2] should be clamped to max");
   DOUBLES_EQUAL_TEXT(0.5F, KF_errcov[0][0], epsilon, "Errcov[0][0] should be clamped");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[1][1], epsilon, "Errcov[1][1] should be clamped");
   DOUBLES_EQUAL_TEXT(1.0F, KF_errcov[2][2], epsilon, "Errcov[2][2] should be clamped");
}

/** \purpose
 * Test state constraint violation - state exceeds maximum
 * \req
 * NA
 */
TEST(apply_state_constraints, Too_Large_States_Clamped_to_Max)
{
   /** \precond
    * All states exceed their respective maximum constraints
    */
   float32_t KF_state[3] = { cvt_state.state_constraints.max_x0 + 0.001F, cvt_state.state_constraints.max_x1 + 0.1F, cvt_state.state_constraints.max_x2 + 0.1F };  // all states exceeded
   cvt_state.one_link.ekf_state_errcov[0][0] = 0.4F;
   cvt_state.one_link.ekf_state_errcov[1][1] = 0.9F;
   cvt_state.one_link.ekf_state_errcov[2][2] = 0.9F;
   float32_t KF_errcov[3][3];
   float32_t KF_errcov_prev[3][3];
   (void)memcpy(&KF_errcov[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(KF_errcov));
   (void)memcpy(&KF_errcov_prev[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(KF_errcov_prev));

   /** \action
    * Apply constraints
    */
   Apply_State_Constraints(cvt_state.state_constraints, KF_state, KF_errcov);

   /** \result
    * All states should be clamped to their respective maximum values
    * Error covariance should be increased (1.1x up to 0.5)
    */
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.max_x0, KF_state[0], epsilon,
                     "State[0] should be clamped to max");
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.max_x1, KF_state[1], epsilon,
                     "State[1] should be clamped to max");
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.max_x2, KF_state[2], epsilon,
                     "State[2] should be clamped to max");
   DOUBLES_EQUAL_TEXT(KF_errcov[0][0], KF_errcov_prev[0][0] * 1.1F, epsilon, "Errcov[0][0] should increase to 1.1 times of the original value");
   DOUBLES_EQUAL_TEXT(KF_errcov[1][1], KF_errcov_prev[1][1] * 1.1F, epsilon, "Errcov[1][1] should increase to 1.1 times of the original value");
   DOUBLES_EQUAL_TEXT(KF_errcov[2][2], KF_errcov_prev[2][2] * 1.1F, epsilon, "Errcov[2][2] should increase to 1.1 times of the original value");
}

/** \purpose
 * Test state constraint violation - state below minimum
 * \req
 * NA
 */
TEST(apply_state_constraints, Too_Small_States_Clamped_to_Min)
{
   /** \precond
    * All states below their respective minimum constraints
    */
   float32_t KF_state[3] = { cvt_state.state_constraints.min_x0 - 0.001F, cvt_state.state_constraints.min_x1 - 0.1F, cvt_state.state_constraints.min_x2 - 0.1F };  // all states exceeded the limits
   cvt_state.one_link.ekf_state_errcov[0][0] = 0.4F;
   cvt_state.one_link.ekf_state_errcov[1][1] = 0.9F;
   cvt_state.one_link.ekf_state_errcov[2][2] = 0.9F;
   float32_t KF_errcov[3][3];
   float32_t KF_errcov_prev[3][3];
   (void)memcpy(&KF_errcov[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(KF_errcov));
   (void)memcpy(&KF_errcov_prev[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(KF_errcov_prev));

   /** \action
    * Apply constraints
    */
   Apply_State_Constraints(cvt_state.state_constraints, KF_state, KF_errcov);

   /** \result
    * All states should be clamped to their respective minimum values
    * Error covariance should be increased (1.1x up to 0.5, 1.0, 1.0 for diagonal terms respectively)
    */
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.min_x0, KF_state[0], epsilon, 
                     "State[0] should be clamped to min");
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.min_x1, KF_state[1], epsilon, 
                     "State[1] should be clamped to min");
   DOUBLES_EQUAL_TEXT(cvt_state.state_constraints.min_x2, KF_state[2], epsilon, 
                     "State[2] should be clamped to min");
   DOUBLES_EQUAL_TEXT(KF_errcov[0][0], KF_errcov_prev[0][0] * 1.1F, epsilon, "Errcov[0][0] should increase to 1.1 times of the original value");
   DOUBLES_EQUAL_TEXT(KF_errcov[1][1], KF_errcov_prev[1][1] * 1.1F, epsilon, "Errcov[1][1] should increase to 1.1 times of the original value");
   DOUBLES_EQUAL_TEXT(KF_errcov[2][2], KF_errcov_prev[2][2] * 1.1F, epsilon, "Errcov[2][2] should increase to 1.1 times of the original value");
}

/** @}*/

//=============================================================================
// Tests for One_Trailer_EKF
//=============================================================================

/** \defgroup f360_cvt_one_link_ekf
 *  @{
 */

/** \brief
 * Test group for One Link EKF functions. Tests cover Calc_Jacobian, KF_Time_Update,
 * KF_Measurement_Update, Apply_State_Constraints, and One_Trailer_EKF with all branches
 * including forward motion, reversing, stationary, measurement valid/invalid, and constraint violations.
 */
TEST_GROUP(f360_cvt_one_link_ekf)
{
   F360_CVT_Input_Data_T cvt_input;
   F360_CVT_State_T cvt_state;
   bool f_updated;
   float32_t initial_ekf_state[3];
   float32_t initial_ekf_state_errcov[3][3];
   float32_t epsilon;
   F360_Calibrations_T calib{};

   /** \setup
    * Initialize all input and state variables to default valid values.
    */
   TEST_SETUP()
   {
      epsilon = 1E-3F;

      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);

      // Initialize input data
      memset(&cvt_input, 0, sizeof(cvt_input));
      cvt_input.host_speed = 2.0F;  // [m/s] Forward motion
      cvt_input.host_yawrate = 0.1F;  // [rad/s]
      cvt_input.host_side_slip_vcs = 0.05F;  // [rad]
      cvt_input.host_rear_axle_vcs_longpos = -3.0F;  // [m]
      cvt_input.n_detections = 5;

      // Initialize state data
      memset(&cvt_state, 0, sizeof(cvt_state));

      // Set up initial ekf_state with reasonable values
      cvt_state.one_link.ekf_state[0] = 0.1F;   // x0: trailer angle (rad)
      cvt_state.one_link.ekf_state[1] = 5.0F;   // x1: rear axle to joint (m)
      cvt_state.one_link.ekf_state[2] = 8.0F;   // x2: 1/(joint2wheels) [1/m]

      // Initialize error covariance with diagonal matrix
      for (int32_t i = 0; i < 3; i++)
      {
         for (int32_t j = 0; j < 3; j++)
         {
            cvt_state.one_link.ekf_state_errcov[i][j] = (i == j) ? 0.5F : 0.01F;
         }
      }

      (void)memcpy(&initial_ekf_state[0], &cvt_state.one_link.ekf_state[0], sizeof(initial_ekf_state));
      (void)memcpy(&initial_ekf_state_errcov[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(initial_ekf_state_errcov));

      // Set up constraints
      cvt_state.state_constraints.max_x0 = F360_DEG2RAD(90.0F);   // Max trailer angle
      cvt_state.state_constraints.min_x0 = F360_DEG2RAD(-90.0F);  // Min trailer angle
      cvt_state.state_constraints.max_x1 = 15.0F;  // Max joint distance
      cvt_state.state_constraints.min_x1 = 1.0F;   // Min joint distance
      cvt_state.state_constraints.max_x2 = 10.0F;  // Max wheelbase
      cvt_state.state_constraints.min_x2 = 0.1F;   // Min wheelbase

      // Set up measurement
      cvt_state.primary_measurement.trailer_angle_vcs = 0.15F;  // [rad]
      cvt_state.primary_measurement.trailer_intersect_vcs_long = 2.0F;  // [m]
      cvt_state.primary_measurement.f_msmt_valid = true;

      f_updated = false;  // Initialize to non-zero value to detect if it gets set
   }
};

/** \purpose
 * Higher level test of One_Trailer_EKF()
 * When the measurement is invalid, the time-update and constriants should still work
 * Once the measurement is valid, all processses will run based on the time-updated states
 * \req
 * NA
 */
TEST(f360_cvt_one_link_ekf, OneTrailerEKF_Overall_Works_as_Expected)
{
   /** \precond
    * Vehicle moving forward with valid measurement
    * Measurement within gate limits
    */
   cvt_state.primary_measurement.f_msmt_valid = false;
   cvt_state.primary_measurement.trailer_angle_vcs = F360_DEG2RAD(4.9F);
   cvt_state.primary_measurement.trailer_intersect_vcs_long = -2.0F + cvt_input.host_rear_axle_vcs_longpos;
   cvt_state.one_link.ekf_state[0] = F360_DEG2RAD(5.0F);
   const float32_t angle_gate = F360_DEG2RAD(6.0F);
   cvt_state.state_constraints.max_x1 = 4.0F;  // such that x1 exceeds the bound
   cvt_state.state_constraints.max_x2 = 7.0F;  // such that x2 exceeds the bound
   cvt_state.one_link.ekf_state_errcov[0][0] = 0.6F;  // violating the constraints
   cvt_state.one_link.ekf_state_errcov[1][1] = 1.6F;  // violating the constraints
   cvt_state.one_link.ekf_state_errcov[2][2] = 1.6F;  // violating the constraints
   epsilon = 1E-4F;

   /** \action
    * Execute One_Trailer_EKF
    */
   One_Trailer_EKF(calib, cvt_input, angle_gate, cvt_state, f_updated);

   /** \result
   * f_updated should be false (measurement was invalid)
   * State should always be within constraints after update
   */
   CHECK_FALSE_TEXT(f_updated, "No measurement update should be done when the measurement is invalid! ");
   CHECK_TRUE_TEXT(cvt_state.one_link.ekf_state[0] >= cvt_state.state_constraints.min_x0, 
            "State[0] should be >= min");
   CHECK_TRUE_TEXT(cvt_state.one_link.ekf_state[0] <= cvt_state.state_constraints.max_x0, 
            "State[0] should be <= max");
   DOUBLES_EQUAL_TEXT(cvt_state.one_link.ekf_state[1], cvt_state.state_constraints.max_x1, epsilon, "x1 should be saturated at higher bound");
   DOUBLES_EQUAL_TEXT(cvt_state.one_link.ekf_state[2], cvt_state.state_constraints.max_x2, epsilon, "x2 should be saturated at higher bound");
   DOUBLES_EQUAL_TEXT(0.6F, cvt_state.one_link.ekf_state_errcov[0][0], epsilon, "When x0 state complies with its constriants, ekf_state_errcov[0][0] will not be constrained, even if it is extremely high.");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.one_link.ekf_state_errcov[1][1], epsilon, "When x1 violates its constriants, ekf_state_errcov[1][1] will also should be constrained, in this case, at higher bound");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.one_link.ekf_state_errcov[2][2], epsilon, "When x2 violates its constriants, ekf_state_errcov[2][2] will also should be constrained, in this case, at higher bound");

   /** \action
    * Make the measurement valid and Execute One_Trailer_EKF again
    */
   cvt_state.primary_measurement.f_msmt_valid = true;
   One_Trailer_EKF(calib, cvt_input, angle_gate, cvt_state, f_updated);

   /** \result
   * f_updated should be true now (measurement was invalid)
   * State should still be within constraints after update
   */
   CHECK_TRUE_TEXT(f_updated, "Measurement update should be done when the measurement is valid! ");
   DOUBLES_EQUAL_TEXT(cvt_state.one_link.ekf_state[0], F360_DEG2RAD(3.9811F), epsilon, "x0 should be computed as expected.");
   DOUBLES_EQUAL_TEXT(cvt_state.one_link.ekf_state[1], 3.9511F, epsilon, "x1 should be computed as expected.");
   DOUBLES_EQUAL_TEXT(cvt_state.one_link.ekf_state[2], 6.9995F, epsilon, "x2 should be computed as expected.");
}


/** \purpose
 * Test the measurement update conditions for One_Trailer_EKF()
 * \req
 * NA
 */
TEST(f360_cvt_one_link_ekf, OneTrailerEKF_Update_Measurement_When_Measurement_is_Confirmed)
{
   /** \precond
    * Vehicle moving forward with valid measurement
    * Measurement within gate limits
    */

    // Define the struct to save all parameters for one test
   struct one_trailer_ekf_udpate_test_T
   {
      bool msmt_valid;
      float32_t msmt_theta;
      float32_t msmt_a1;
      float32_t trailer_angle;
      float32_t host_speed;
      bool update_expected;
      std::string test_description;  // Descriptor of the test
   };

   cvt_state.state_constraints.max_x1 = 3.0F;
   cvt_state.state_constraints.min_x1 = -2.0F;
   const float32_t angle_gate = F360_DEG2RAD(6.0F);

   // Define different test cases
   std::vector<one_trailer_ekf_udpate_test_T> all_test_cases = {
      //msmt_valid,  msmt_theta,           msmt_a1,      trailer_angle,        host_speed,  update_expected,  test_description
       {false,       F360_DEG2RAD(4.6F),   2.0F,         F360_DEG2RAD(4.6F),  -0.5F,        false,            "Test1: False measurement valid flag should fail all measurement confirmation"},
       {true,        F360_DEG2RAD(4.5F),   2.0F,         F360_DEG2RAD(4.6F),  -0.5F,        true,             "Test2: Estimated angle larger than 4.5 deg may confirm the measurement"},
       {true,        F360_DEG2RAD(4.6F),   2.0F,         F360_DEG2RAD(4.5F),  -0.5F,        true,             "Test3: Measured angle larger than 4.5 deg may confirm the measurement"},
       {true,        F360_DEG2RAD(4.5F),   2.0F,         F360_DEG2RAD(4.5F),  -0.5F,        false,            "Test4: When none of the measured and estimated angle is larger than 4.5 deg, measurement will not be confirmed"},
       {true,        F360_DEG2RAD(10.6F),  2.0F,         F360_DEG2RAD(4.6F),  -0.5F,        false,            "Test5: Measured angle being too much larger than the estimated will not be confirmed"},
       {true,        F360_DEG2RAD(-4.6F),  2.0F,         F360_DEG2RAD(4.6F),  -0.5F,        false,            "Test6: Measured angle being too much smaller than the estimated will not be confirmed"},
       {true,        F360_DEG2RAD(4.6F),   5.0F,         F360_DEG2RAD(4.6F),  -0.5F,        false,            "Test7: If measured joint vcs position is much larger(forward) compared to the estimated, the measurement will not be confirmed."},
       {true,        F360_DEG2RAD(4.6F),  -4.1F,         F360_DEG2RAD(4.6F),  -0.5F,        false,            "Test8: If measured joint vcs position is much smaller(backward) compared to the estimated, the measurement will not be confirmed."},
       {true,        F360_DEG2RAD(4.6F),   2.0F,         F360_DEG2RAD(4.6F),  -0.51F,       false,            "Test9: If the host is reversing with absolute speed larger than 0.5 m/s, the measurement will not be confirmed."},
       {true,        F360_DEG2RAD(4.6F),   2.0F,         F360_DEG2RAD(4.6F),   0.0F,        true,             "Test10: Even if the host is stationary, if other conditions are met, the measurement should still be confirmed"},
       {true,        F360_DEG2RAD(4.6F),   2.0F,         F360_DEG2RAD(4.6F),   2.0F,        true,             "Test11: When host is driving forward, the measurement can be confirmed"},
       {true,        F360_DEG2RAD(4.6F),   2.0F,         3.01F,                2.0F,        false,            "Test12: When the angle state exceed the bounds, although the measurement will not be confirmed, and state will be reset"},
       {true,        F360_DEG2RAD(4.6F),   2.0F,         -2.01F,               2.0F,        false,            "Test13: When the angle state exceed the bounds, although the measurement will not be confirmed, and state will be reset"},
      };


   /** \action
    * Execute One_Trailer_EKF
    */
   for (const one_trailer_ekf_udpate_test_T &test_case_i : all_test_cases)
   {
      cvt_state.primary_measurement.f_msmt_valid = test_case_i.msmt_valid;
      cvt_state.primary_measurement.trailer_angle_vcs = test_case_i.msmt_theta;
      cvt_state.primary_measurement.trailer_intersect_vcs_long = -test_case_i.msmt_a1 + cvt_input.host_rear_axle_vcs_longpos;
      cvt_state.one_link.ekf_state[0] = test_case_i.trailer_angle;
      cvt_input.host_speed = test_case_i.host_speed;
      const bool f_update_expected = test_case_i.update_expected;
      f_updated = !f_update_expected;

      One_Trailer_EKF(calib, cvt_input, angle_gate, cvt_state, f_updated);

      /** \result
      * f_updated should be as expected
      * State should always be within constraints after update
      */
      CHECK_TRUE_TEXT(f_updated == f_update_expected, test_case_i.test_description.c_str());
      CHECK_TRUE_TEXT(cvt_state.one_link.ekf_state[0] >= cvt_state.state_constraints.min_x0, 
               "State[0] should be >= min");
      CHECK_TRUE_TEXT(cvt_state.one_link.ekf_state[0] <= cvt_state.state_constraints.max_x0, 
               "State[0] should be <= max");
      CHECK_TRUE_TEXT(cvt_state.one_link.ekf_state[1] >= cvt_state.state_constraints.min_x1, 
               "State[1] should be >= min");
      CHECK_TRUE_TEXT(cvt_state.one_link.ekf_state[1] <= cvt_state.state_constraints.max_x1, 
               "State[1] should be <= max");
      CHECK_TRUE_TEXT(cvt_state.one_link.ekf_state[2] >= cvt_state.state_constraints.min_x2, 
               "State[2] should be >= min");
      CHECK_TRUE_TEXT(cvt_state.one_link.ekf_state[2] <= cvt_state.state_constraints.max_x2, 
               "State[2] should be <= max");
   }
}

/** @}*/
