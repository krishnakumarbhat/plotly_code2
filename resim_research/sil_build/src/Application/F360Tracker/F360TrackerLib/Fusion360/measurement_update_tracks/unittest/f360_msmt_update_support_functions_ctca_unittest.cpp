/** \file
 * This file contains unit tests for content of f360_msmt_update_support_functions_ctca.cpp file
 */

#include "f360_msmt_update_support_functions_ctca.h"
#include <CppUTest/TestHarness.h>

#include "f360_math_func.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_pseudo_msmt.h"
#include <algorithm>

using namespace f360_variant_A;


/** \defgroup  f360_msmt_update_support_functions_ctca_KF_functions_simple
*  @{
*/

/** \brief
* This test group sets up test data and expected output for CTCA update in simple case.
* One sensor is setup in VCS origin and oriented with bore sight aligned with VCS-long.
* Covariance matrix P and measurement noise variance matrix R are both diagonal matrices
* Matlab script that was used to generate expected data can be found in DFD-307
*/
TEST_GROUP(f360_msmt_update_support_functions_ctca_KF_functions_simple)
{
   uint32_t nr_pseudo_msnmts;
   uint32_t nr_dets;
   uint32_t nr_total_msnmts;
   float32_t h_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
   float32_t p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t z_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t zhat_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t state[STATE_DIMENSION];

   float32_t s_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};

   float32_t exp_s_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t exp_k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t exp_p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t exp_state[STATE_DIMENSION] = {};

   float32_t rdot_comp;
   float32_t azimuth;
   float32_t det_vcs_x;
   float32_t det_vcs_y;

   float32_t threshold = 0.0001F; // Threshold for comparing calculated and expected data

   /** \setup
   * Create data for simple CTCA update
   */
   TEST_SETUP()
   {
      // Setup arbitrary reasonable state vector
      state[F360_TRK_FLTR_CTCA_STATE_X] = 10.0F;
      state[F360_TRK_FLTR_CTCA_STATE_Y] = 0.0F;
      state[F360_TRK_FLTR_CTCA_STATE_H] = 0.0F;
      state[F360_TRK_FLTR_CTCA_STATE_C] = 0.0F;
      state[F360_TRK_FLTR_CTCA_STATE_S] = 5.0F;
      state[F360_TRK_FLTR_CTCA_STATE_A] = 0.0F;

      // One detection
      nr_dets = 1U;
      azimuth = F360_DEG2RAD(0.0F);
      rdot_comp = state[F360_TRK_FLTR_CTCA_STATE_S] + 0.1F; // Add some value to force state update
      det_vcs_x = state[F360_TRK_FLTR_CTCA_STATE_X]; // Place the detection exactly on the object position for simplicity
      det_vcs_y = state[F360_TRK_FLTR_CTCA_STATE_Y]; // Place the detection exactly on the object position for simplicity

      // Set up H, Z and Zhat matrix
      //Two pseudo position measurements
      nr_pseudo_msnmts = 2U;
      nr_total_msnmts = nr_dets + nr_pseudo_msnmts;
      z_mat[0] = state[F360_TRK_FLTR_CTCA_STATE_X] + 0.1F; // Pseudo position, add some value to force state update
      z_mat[1] = state[F360_TRK_FLTR_CTCA_STATE_Y]; // Pseudo position

      h_mat[0][F360_TRK_FLTR_CTCA_STATE_X] = 1.0F;
      h_mat[1][F360_TRK_FLTR_CTCA_STATE_Y] = 1.0F;

      zhat_mat[0] = state[F360_TRK_FLTR_CTCA_STATE_X];
      zhat_mat[1] = state[F360_TRK_FLTR_CTCA_STATE_Y];
      for (uint32_t i = 0U; i < nr_dets; i++)
      {
         float32_t cos_az = F360_Cosf(azimuth);
         float32_t sin_az = F360_Sinf(azimuth);
         float32_t speed = state[F360_TRK_FLTR_CTCA_STATE_S];
         float32_t curv = state[F360_TRK_FLTR_CTCA_STATE_C];
         float32_t cos_head = F360_Cosf(state[F360_TRK_FLTR_CTCA_STATE_H]);
         float32_t sin_head = F360_Sinf(state[F360_TRK_FLTR_CTCA_STATE_H]);
         float32_t delta_x = det_vcs_x - state[F360_TRK_FLTR_CTCA_STATE_X];
         float32_t delta_y = det_vcs_y - state[F360_TRK_FLTR_CTCA_STATE_Y];

         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_X] = -sin_az * curv * speed;
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_Y] = cos_az * curv * speed;
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_H] = speed * (cos_head * sin_az - sin_head * cos_az);
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_C] = speed * (delta_x * sin_az - delta_y * cos_az);
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_S] = (cos_head - curv * delta_y) * cos_az + (sin_head + curv * delta_x) * sin_az;
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_A] = 0.0F;

         z_mat[i + nr_pseudo_msnmts] = rdot_comp;

         zhat_mat[i + nr_pseudo_msnmts] = speed * h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_S];
      }

      // Set up arbitrary symmetric P matrix as identity matrix
      for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
      {
         p_mat[i][i] = 1.0F;
      }

      // Set up arbitrary symmetric R matrix
      for (uint32_t i = 0U; i < nr_total_msnmts; i++)
      {
         r_mat[i][i] = 3.0F;
      }

      // Setup expected data
      // Kalman gain
      exp_k_mat[0][0] = 0.25F;
      exp_k_mat[0][1] = 0.0F;
      exp_k_mat[0][2] = 0.0F;
      exp_k_mat[1][0] = 0.0F;
      exp_k_mat[1][1] = 0.25F;
      exp_k_mat[1][2] = 0.0F;
      exp_k_mat[2][0] = 0.0F;
      exp_k_mat[2][1] = 0.0F;
      exp_k_mat[2][2] = 0.0F;
      exp_k_mat[3][0] = 0.0F;
      exp_k_mat[3][1] = 0.0F;
      exp_k_mat[3][2] = 0.0F;
      exp_k_mat[4][0] = 0.0F;
      exp_k_mat[4][1] = 0.0F;
      exp_k_mat[4][2] = 0.25F;
      exp_k_mat[5][0] = 0.0F;
      exp_k_mat[5][1] = 0.0F;
      exp_k_mat[5][2] = 0.0F;

      // Innovation covariance S
      exp_s_mat[0][0] = 4.0F;
      exp_s_mat[0][1] = 0.0F;
      exp_s_mat[0][2] = 0.0F;
      exp_s_mat[1][0] = 0.0F;
      exp_s_mat[1][1] = 4.0F;
      exp_s_mat[1][2] = 0.0F;
      exp_s_mat[2][0] = 0.0F;
      exp_s_mat[2][1] = 0.0F;
      exp_s_mat[2][2] = 4.0F;

      // Error covariance matrix
      exp_p_mat[0][0] = 0.75F;
      exp_p_mat[0][1] = 0.0F;
      exp_p_mat[0][2] = 0.0F;
      exp_p_mat[0][3] = 0.0F;
      exp_p_mat[0][4] = 0.0F;
      exp_p_mat[0][5] = 0.0F;
      exp_p_mat[1][0] = 0.0F;
      exp_p_mat[1][1] = 0.75F;
      exp_p_mat[1][2] = 0.0F;
      exp_p_mat[1][3] = 0.0F;
      exp_p_mat[1][4] = 0.0F;
      exp_p_mat[1][5] = 0.0F;
      exp_p_mat[2][0] = 0.0F;
      exp_p_mat[2][1] = 0.0F;
      exp_p_mat[2][2] = 1.0F;
      exp_p_mat[2][3] = 0.0F;
      exp_p_mat[2][4] = 0.0F;
      exp_p_mat[2][5] = 0.0F;
      exp_p_mat[3][0] = 0.0F;
      exp_p_mat[3][1] = 0.0F;
      exp_p_mat[3][2] = 0.0F;
      exp_p_mat[3][3] = 1.0F;
      exp_p_mat[3][4] = 0.0F;
      exp_p_mat[3][5] = 0.0F;
      exp_p_mat[4][0] = 0.0F;
      exp_p_mat[4][1] = 0.0F;
      exp_p_mat[4][2] = 0.0F;
      exp_p_mat[4][3] = 0.0F;
      exp_p_mat[4][4] = 0.75F;
      exp_p_mat[4][5] = 0.0F;
      exp_p_mat[5][0] = 0.0F;
      exp_p_mat[5][1] = 0.0F;
      exp_p_mat[5][2] = 0.0F;
      exp_p_mat[5][3] = 0.0F;
      exp_p_mat[5][4] = 0.0F;
      exp_p_mat[5][5] = 1.0F;

      // State update
      exp_state[0] = 10.025F;
      exp_state[1] = 0.0F;
      exp_state[2] = 0.0F;
      exp_state[3] = 0.0F;
      exp_state[4] = 5.025F;
      exp_state[5] = 0.0F;
   }

};

/** \purpose
* Verify that innovation covariance matrix S and Kalman gain matrix is calculated as expected in simple CTCA case.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_ctca_KF_functions_simple, Test_Kalman_Gain_Update)
{

   /** \action
   * Call function
   */
   Kalman_Gain_Update_CTCA(
      h_mat,
      p_mat,
      r_mat,
      nr_total_msnmts,
      k_mat,
      s_mat);

   /** \result
   * Compare computed against expected data
   */

   // S-matrix
   for (uint32_t i = 0U; i < nr_total_msnmts; i++)
   {
      for (uint32_t j = 0U; j < nr_total_msnmts; j++)
      {
         DOUBLES_EQUAL(exp_s_mat[i][j], s_mat[i][j], threshold);
      }
   }

   // K matrix
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      for (uint32_t j = 0U; j < nr_total_msnmts; j++)
      {
         DOUBLES_EQUAL(exp_k_mat[i][j], k_mat[i][j], threshold);
      }
   }
}

/** \purpose
* Verify that error covariance matrix is calculated as expected in simple CTCA case.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_ctca_KF_functions_simple, Test_Error_Cov_Update)
{

   /** \action
   * Call function
   */
   Error_Cov_Update_CTCA(
      exp_k_mat,
      r_mat,
      h_mat,
      nr_total_msnmts,
      p_mat);

   /** \result
   * Compare computed P matrix against expected data
   */
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      for (uint32_t j = 0U; j < STATE_DIMENSION; j++)
      {
         DOUBLES_EQUAL(exp_p_mat[i][j], p_mat[i][j], threshold);
      }
   }
}

/** \purpose
* Verify that state vector is calculated as expected in simple CTCA case.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_ctca_KF_functions_simple, Test_State_Update)
{

   /** \action
   * Call function
   */
   State_Update_CTCA(
      z_mat,
      zhat_mat,
      nr_total_msnmts,
      exp_k_mat,
      state);

   /** \result
   * Compare computed State vector against expected data
   */
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      DOUBLES_EQUAL(exp_state[i], state[i], threshold);
   }
}

/** @}*/

/** \defgroup  f360_msmt_update_support_functions_ctca_KF_functions_general
*  @{
*/

/** \brief
* This test group sets up test data and expected output for CTCA update in a general case.
* One sensor is setup in VCS origin and oriented with bore sight aligned with VCS-long.
* Covariance matrix P and measurement noise variance matrix R are both arbitrariliy generated
* as symmetric and reasonable.
* Matlab script that was used to generate expected data can be found in DFD-307
*/
TEST_GROUP(f360_msmt_update_support_functions_ctca_KF_functions_general)
{
   uint32_t nr_pseudo_msnmts;
   uint32_t nr_dets;
   uint32_t nr_total_msnmts;
   float32_t h_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
   float32_t p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t z_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t zhat_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t state[STATE_DIMENSION];

   float32_t s_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};

   float32_t exp_s_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t exp_k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t exp_p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t exp_state[STATE_DIMENSION] = {};

   float32_t rdot_comp[3]; // Must be the size of number of detections
   float32_t azimuth[3]; // Must be the size of number of detections
   float32_t det_vcs_x[3]; // Must be the size of number of detections
   float32_t det_vcs_y[3]; // Must be the size of number of detections

   const float32_t threshold = 0.0001F; // Threshold for comparing calculated and expected data

   /** \setup
   * Create data for general CTCA update
   */
   TEST_SETUP()
   {
      // Setup arbitrary reasonable state vector
      state[F360_TRK_FLTR_CTCA_STATE_X] = 10.0F;
      state[F360_TRK_FLTR_CTCA_STATE_Y] = 0.0F;
      state[F360_TRK_FLTR_CTCA_STATE_H] = F360_DEG2RAD(5.0F);
      state[F360_TRK_FLTR_CTCA_STATE_C] = 0.002F;
      state[F360_TRK_FLTR_CTCA_STATE_S] = 5.0F;
      state[F360_TRK_FLTR_CTCA_STATE_A] = 0.1F;

      // Three detections
      nr_dets = 3U;
      azimuth[0] = F360_DEG2RAD(0.0F);
      azimuth[1] = F360_DEG2RAD(5.0F);
      azimuth[2] = F360_DEG2RAD(10.0F);
      rdot_comp[0] = state[F360_TRK_FLTR_CTCA_STATE_S] + 0.1F; // Add some value to force state update
      rdot_comp[1] = state[F360_TRK_FLTR_CTCA_STATE_S] - 0.1F; // Add some value to force state update
      rdot_comp[2] = state[F360_TRK_FLTR_CTCA_STATE_S] + 0.2F; // Add some value to force state update
      det_vcs_x[0] = state[F360_TRK_FLTR_CTCA_STATE_X] + 0.2F; // Place the detections close to the object
      det_vcs_x[1] = state[F360_TRK_FLTR_CTCA_STATE_X] - 0.1F; // Place the detections close to the object
      det_vcs_x[2] = state[F360_TRK_FLTR_CTCA_STATE_X] + 0.3F; // Place the detections close to the object
      det_vcs_y[0] = state[F360_TRK_FLTR_CTCA_STATE_Y] - 0.2F; // Place the detections close to the object
      det_vcs_y[1] = state[F360_TRK_FLTR_CTCA_STATE_Y] + 0.3F; // Place the detections close to the object
      det_vcs_y[2] = state[F360_TRK_FLTR_CTCA_STATE_Y] + 0.4F; // Place the detections close to the object

      // Set up H, Z and Zhat matrix
      // Two pseudo position measurements
      nr_pseudo_msnmts = 2U;
      nr_total_msnmts = nr_dets + nr_pseudo_msnmts;
      z_mat[0] = state[F360_TRK_FLTR_CTCA_STATE_X] + 0.1F; // Pseudo position x, add some value to force state update
      z_mat[1] = state[F360_TRK_FLTR_CTCA_STATE_Y]; // Pseudo position y

      h_mat[0][F360_TRK_FLTR_CTCA_STATE_X] = 1.0F;
      h_mat[1][F360_TRK_FLTR_CTCA_STATE_Y] = 1.0F;

      zhat_mat[0] = state[F360_TRK_FLTR_CTCA_STATE_X];
      zhat_mat[1] = state[F360_TRK_FLTR_CTCA_STATE_Y];

      for (uint32_t i = 0U; i < nr_dets; i++)
      {
         float32_t cos_az = F360_Cosf(azimuth[i]);
         float32_t sin_az = F360_Sinf(azimuth[i]);
         float32_t speed = state[F360_TRK_FLTR_CTCA_STATE_S];
         float32_t curv = state[F360_TRK_FLTR_CTCA_STATE_C];
         float32_t cos_head = F360_Cosf(state[F360_TRK_FLTR_CTCA_STATE_H]);
         float32_t sin_head = F360_Sinf(state[F360_TRK_FLTR_CTCA_STATE_H]);
         float32_t delta_x = det_vcs_x[i] - state[F360_TRK_FLTR_CTCA_STATE_X];
         float32_t delta_y = det_vcs_y[i] - state[F360_TRK_FLTR_CTCA_STATE_Y];

         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_X] = -sin_az * curv * speed;
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_Y] = cos_az * curv * speed;
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_H] = speed * (cos_head * sin_az - sin_head * cos_az);
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_C] = speed * (delta_x * sin_az - delta_y * cos_az);
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_S] = (cos_head - curv * delta_y) * cos_az + (sin_head + curv * delta_x) * sin_az;
         h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_A] = 0.0F;

         z_mat[i + nr_pseudo_msnmts] = rdot_comp[i];

         zhat_mat[i + nr_pseudo_msnmts] = speed * h_mat[i + nr_pseudo_msnmts][F360_TRK_FLTR_CTCA_STATE_S];
      }

      // Set up arbitrary symmetric P matrix
      // Fill upper triangle
      p_mat[0][0] = 3.94F;
      p_mat[0][1] = 0.15F;
      p_mat[0][2] = 1.11F;
      p_mat[0][3] = 0.36F;
      p_mat[0][4] = -0.28F;
      p_mat[0][5] = 1.22F;
      p_mat[1][1] = 6.13F;
      p_mat[1][2] = 1.56F;
      p_mat[1][3] = 1.43F;
      p_mat[1][4] = -0.86F;
      p_mat[1][5] = 0.97F;
      p_mat[2][2] = 8.18F;
      p_mat[2][3] = -0.63F;
      p_mat[2][4] = 2.4F;
      p_mat[2][5] = 0.81F;
      p_mat[3][3] = 8.859999999999999F;
      p_mat[3][4] = 2.12F;
      p_mat[3][5] = -1.61F;
      p_mat[4][4] = 9.31F;
      p_mat[4][5] = -0.47F;
      p_mat[5][5] = 1.9F;
      // Fill lower triangle
      for (int32_t col = 0U; col < STATE_DIMENSION; col++)
      {
         for (int32_t row = 0U; row < col; row++)
         {
            p_mat[col][row] = p_mat[row][col];
         }
      }

      // Set up R matrix
      // Fill upper triangle
      r_mat[0][0] = 1.3F;
      r_mat[0][1] = -0.23F;
      r_mat[1][0] = r_mat[0][1];
      r_mat[1][1] = 1.8F;
      r_mat[2][2] = 0.11F;
      r_mat[3][3] = 0.9F;
      r_mat[4][4] = 1.4F;

      // Fill the K matrix with values for tests that doesn't fill it
      k_mat[0][0] = 0.74825F;
      k_mat[0][1] = 0.021276F;
      k_mat[0][2] = -0.0043383F;
      k_mat[0][3] = -0.035875F;
      k_mat[0][4] = 0.026013F;
      k_mat[1][0] = 0.031239F;
      k_mat[1][1] = 0.7583F;
      k_mat[1][2] = -0.0057788F;
      k_mat[1][3] = -0.072695F;
      k_mat[1][4] = 0.03867F;
      k_mat[2][0] = 0.1697F;
      k_mat[2][1] = 0.19141F;
      k_mat[2][2] = 0.05374F;
      k_mat[2][3] = -0.71174F;
      k_mat[2][4] = 0.81061F;
      k_mat[3][0] = 0.04135F;
      k_mat[3][1] = 0.048649F;
      k_mat[3][2] = 0.39719F;
      k_mat[3][3] = -0.38309F;
      k_mat[3][4] = 0.032319F;
      k_mat[4][0] = 0.031961F;
      k_mat[4][1] = 0.026861F;
      k_mat[4][2] = 0.62273F;
      k_mat[4][3] = 0.075311F;
      k_mat[4][4] = 0.31883F;
      k_mat[5][0] = 0.24275F;
      k_mat[5][1] = 0.15688F;
      k_mat[5][2] = -0.075102F;
      k_mat[5][3] = 0.079445F;
      k_mat[5][4] = 0.0077887F;

      // Setup expected data
      // Kalman gain      
      exp_k_mat[0][0] = 0.74825F;
      exp_k_mat[0][1] = 0.021276F;
      exp_k_mat[0][2] = -0.0043383F;
      exp_k_mat[0][3] = -0.035875F;
      exp_k_mat[0][4] = 0.026013F;
      exp_k_mat[1][0] = 0.031239F;
      exp_k_mat[1][1] = 0.7583F;
      exp_k_mat[1][2] = -0.0057788F;
      exp_k_mat[1][3] = -0.072695F;
      exp_k_mat[1][4] = 0.03867F;
      exp_k_mat[2][0] = 0.1697F;
      exp_k_mat[2][1] = 0.19141F;
      exp_k_mat[2][2] = 0.05374F;
      exp_k_mat[2][3] = -0.71174F;
      exp_k_mat[2][4] = 0.81061F;
      exp_k_mat[3][0] = 0.04135F;
      exp_k_mat[3][1] = 0.048649F;
      exp_k_mat[3][2] = 0.39719F;
      exp_k_mat[3][3] = -0.38309F;
      exp_k_mat[3][4] = 0.032319F;
      exp_k_mat[4][0] = 0.031961F;
      exp_k_mat[4][1] = 0.026861F;
      exp_k_mat[4][2] = 0.62273F;
      exp_k_mat[4][3] = 0.075311F;
      exp_k_mat[4][4] = 0.31883F;
      exp_k_mat[5][0] = 0.24275F;
      exp_k_mat[5][1] = 0.15688F;
      exp_k_mat[5][2] = -0.075102F;
      exp_k_mat[5][3] = 0.079445F;
      exp_k_mat[5][4] = 0.0077887F;

      // Innovation covariance S
      exp_s_mat[0][0] = 5.24F;
      exp_s_mat[0][1] = -0.08F;
      exp_s_mat[0][2] = -0.40126F;
      exp_s_mat[0][3] = -0.8354F;
      exp_s_mat[0][4] = -0.41568F;
      exp_s_mat[1][0] = -0.08F;
      exp_s_mat[1][1] = 7.93F;
      exp_s_mat[1][2] = -0.045586F;
      exp_s_mat[1][3] = -2.9977F;
      exp_s_mat[1][4] = -2.5603F;
      exp_s_mat[2][0] = -0.40126F;
      exp_s_mat[2][1] = -0.045586F;
      exp_s_mat[2][2] = 22.4586F;
      exp_s_mat[2][3] = -6.9815F;
      exp_s_mat[2][4] = -9.7289F;
      exp_s_mat[3][0] = -0.8354F;
      exp_s_mat[3][1] = -2.9977F;
      exp_s_mat[3][2] = -6.9815F;
      exp_s_mat[3][3] = 24.5774F;
      exp_s_mat[3][4] = 27.0973F;
      exp_s_mat[4][0] = -0.41568F;
      exp_s_mat[4][1] = -2.5603F;
      exp_s_mat[4][2] = -9.7289F;
      exp_s_mat[4][3] = 27.0973F;
      exp_s_mat[4][4] = 33.8186F;

      // Error covariance matrix   
      exp_p_mat[0][0] = 0.96783F;
      exp_p_mat[0][1] = -0.1338F;
      exp_p_mat[0][2] = 0.17659F;
      exp_p_mat[0][3] = 0.042565F;
      exp_p_mat[0][4] = 0.035371F;
      exp_p_mat[0][5] = 0.27949F;
      exp_p_mat[1][0] = -0.1338F;
      exp_p_mat[1][1] = 1.3578F;
      exp_p_mat[1][2] = 0.3055F;
      exp_p_mat[1][3] = 0.078057F;
      exp_p_mat[1][4] = 0.041F;
      exp_p_mat[1][5] = 0.22656F;
      exp_p_mat[2][0] = 0.17659F;
      exp_p_mat[2][1] = 0.3055F;
      exp_p_mat[2][2] = 4.4861F;
      exp_p_mat[2][3] = 1.026F;
      exp_p_mat[2][4] = 0.93499F;
      exp_p_mat[2][5] = -0.16197F;
      exp_p_mat[3][0] = 0.042565F;
      exp_p_mat[3][1] = 0.078057F;
      exp_p_mat[3][2] = 1.026F;
      exp_p_mat[3][3] = 0.32941F;
      exp_p_mat[3][4] = 0.16117F;
      exp_p_mat[3][5] = -0.059363F;
      exp_p_mat[4][0] = 0.035371F;
      exp_p_mat[4][1] = 0.041F;
      exp_p_mat[4][2] = 0.93499F;
      exp_p_mat[4][3] = 0.16117F;
      exp_p_mat[4][4] = 0.31545F;
      exp_p_mat[4][5] = -0.021819F;
      exp_p_mat[5][0] = 0.27949F;
      exp_p_mat[5][1] = 0.22656F;
      exp_p_mat[5][2] = -0.16197F;
      exp_p_mat[5][3] = -0.059363F;
      exp_p_mat[5][4] = -0.021819F;
      exp_p_mat[5][5] = 1.0891F;

      // State update
      exp_state[0] = 10.0836F;
      exp_state[1] = 0.018095F;
      exp_state[2] = 0.35983F;
      exp_state[3] = 0.096936F;
      exp_state[4] = 5.1397F;
      exp_state[5] = 0.10952F;  
   }

};

/** \purpose
* Verify that innovation covariance matrix S and Kalman gain matrix is calculated as expected in a general CTCA case.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_ctca_KF_functions_general, Test_Kalman_Gain_Update)
{

   /** \action
   * Call function
   */
   Kalman_Gain_Update_CTCA(
      h_mat,
      p_mat,
      r_mat,
      nr_total_msnmts,
      k_mat,
      s_mat);

   /** \result
   * Compare computed against expected data
   */

   // S-matrix
   for (uint32_t i = 0U; i < nr_total_msnmts; i++)
   {
      for (uint32_t j = 0U; j < nr_total_msnmts; j++)
      {
         DOUBLES_EQUAL(exp_s_mat[i][j], s_mat[i][j], threshold);
      }
   }

   // K matrix
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      for (uint32_t j = 0U; j < nr_total_msnmts; j++)
      {
         DOUBLES_EQUAL(exp_k_mat[i][j], k_mat[i][j], threshold);
      }
   }
}

/** \purpose
* Verify that error covariance matrix is calculated as expected in a general CTCA case.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_ctca_KF_functions_general, Test_Error_Cov_Update)
{
   /** \action
   * Call function
   */
   Error_Cov_Update_CTCA(
      k_mat,
      r_mat,
      h_mat,
      nr_total_msnmts,
      p_mat);

   /** \result
   * Compare computed P matrix against expected data
   */
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      for (uint32_t j = 0U; j < STATE_DIMENSION; j++)
      {
         DOUBLES_EQUAL(exp_p_mat[i][j], p_mat[i][j], threshold);
      }
   }
}

/** \purpose
* Verify that state vector is calculated as expected in a general CTCA case.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_ctca_KF_functions_general, Test_State_Update)
{

   /** \action
   * Call function
   */
   State_Update_CTCA(
      z_mat,
      zhat_mat,
      nr_total_msnmts,
      k_mat,
      state);

   /** \result
   * Compare computed State vector against expected data
   */
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      DOUBLES_EQUAL(exp_state[i], state[i], threshold);
   }
}
/** @}*/


/** \defgroup  f360_msmt_update_support_functions_ctca_fill_R
*  @{
*/

/** \brief
*  These tests check that functions that fill the process noise covariance matrix (R) are working as intended.
*  Initialize object track and det_props with zeros.
**/
TEST_GROUP(f360_msmt_update_support_functions_ctca_fill_R)
{
   /** \setup
   * Declare variable for tracker calibrations and initialize it to default tracker calibration value.
   * Declare a variable for tracked objects.
   **/
   F360_Calibrations_T calib;
   F360_Object_Track_T object_track;

   uint32_t selected_dets_score[MSMT_UPDATE_MAX_NUM_OF_MSMT]{};


   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      std::fill(cmn::begin(selected_dets_score), cmn::end(selected_dets_score), 1U);
   }
};

/*\purpose
 * Check Filling_Msmt_Cov_By_Pos_CTCA correctly filling in r_mat
*/
TEST(f360_msmt_update_support_functions_ctca_fill_R, Test_Filling_Msmt_Cov_By_Pos_CTCA)
{
   /** \step{1}
   * Testing that Filling_Msmt_Cov_By_Pos_CTCA returns the correct value
   **/

   /** \precond
   * Set the test case initial condition, i.e. meascov values for the object.
   **/

   float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t expected_r[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};

   object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = 1.0F;
   object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y] = 2.5F;
   object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X] = 2.5F;
   object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = 2.0F;

   expected_r[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = 1.0F;
   expected_r[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y] = 2.5F;
   expected_r[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X] = 2.5F;
   expected_r[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = 2.0F;

   /** \action
   * Call the function Filling_Msmt_Cov_By_Pos_CTCA
   **/

   Filling_Msmt_Cov_By_Pos_CTCA(object_track, r_mat);

   /** \result
   * Check that function returns the expected r_mat
   **/

   bool f_r_mat_fill_succeeded = false;
   f_r_mat_fill_succeeded = (fabsf(r_mat[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] - expected_r[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X]) < 1e-6f)
      && (fabsf(r_mat[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y] - expected_r[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y]) < 1e-6f)
      && (fabsf(r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X] - expected_r[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X]) < 1e-6f)
      && (fabsf(r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] - expected_r[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y]) < 1e-6f);

   CHECK_TEXT(f_r_mat_fill_succeeded, "The elements in R matrix was not filled correctly in terms of position.");
}


/*\purpose
 * Check Filling_Msmt_Cov_By_H_RR_CTCA correctly filling in r_mat
*/
TEST(f360_msmt_update_support_functions_ctca_fill_R, Test_Filling_Msmt_Cov_By_H_RR_CTCA)
{
   /** \precond
   * Set the test number of selected rr measurements to 3 (any random non-zero number would do and here we choose 3).
   * Initialize the R matrix variable to contain all zeros.
   **/
   const uint32_t selected_dets_num = 3U;
   float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = { };

   /** \action
   * Call the function Filling_Msmt_Cov_By_H_RR_CTCA
   **/
   Filling_Msmt_Cov_By_H_RR_CTCA(calib, selected_dets_num, selected_dets_score, r_mat);

   /** \result
   * Check that function returns the expected r_mat. Expected is that the diagonal elements [2,2], [3,3] and [4,4] are set to calib.k_ref_msmt_cov_ctca.
   * All other elements should be unchanged by the function call (i.e. they should be zero).
   **/
   float32_t expected_r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = { };
   expected_r_mat[2][2] = calib.k_ref_msmt_cov_ctca;
   expected_r_mat[3][3] = calib.k_ref_msmt_cov_ctca;
   expected_r_mat[4][4] = calib.k_ref_msmt_cov_ctca;

   char textFailure[100];
   for (uint8_t i = 0; i < MSMT_UPDATE_MAX_NUM_OF_MSMT; i++)
   {
      for (uint8_t j = 0; j < MSMT_UPDATE_MAX_NUM_OF_MSMT; j++)
      {
         (void)sprintf(textFailure, "Returned r_mat value failed on element row_idx=%d and column_idx=%d: \n", i, j);
         DOUBLES_EQUAL_TEXT(expected_r_mat[i][j], r_mat[i][j], 1e-2F, textFailure);
      }
   }
}

/*\purpose
 * Check Filling_Msmt_Cov_By_H_RR_CTCA correctly filling in r_mat while having different amount of detections in each bin
 * and also check when one of the bins has score set to 0.
*/
TEST(f360_msmt_update_support_functions_ctca_fill_R, Test_Filling_Msmt_Cov_By_H_RR_CTCA_Different_Bins)
{
   /** \precond
   * Set the test number of selected rr measurements to 4.
   * Set different score for each detection which corresponds to how many detections were previously in each bin
   * with the last bin having score set to 0.
   * Initialize the R matrix variable to contain all zeros.
   **/
   const uint32_t selected_dets_num = 4U;
   selected_dets_score[0] = 4U;
   selected_dets_score[1] = 1U;
   selected_dets_score[2] = 10U;
   selected_dets_score[3] = 0U;
   float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = { };

   /** \action
   * Call the function Filling_Msmt_Cov_By_H_RR_CTCA
   **/
   Filling_Msmt_Cov_By_H_RR_CTCA(calib, selected_dets_num, selected_dets_score, r_mat);

   /** \result
   * Check that function returns the expected r_mat. The score is calculated as r = calib.k_ref_msmt_cov_ctca * (N_max / N_i),
   * where N_max is maximum number of detections in all bins, and N_i is number of detections in current bin.
   * Expected score for elements in diagonal are set to:
   * - [2,2] calib.k_ref_msmt_cov_ctca * 10/4
   * - [3,3] calib.k_ref_msmt_cov_ctca * 10/1
   * - [4,4] calib.k_ref_msmt_cov_ctca * 10/10
   * - [5,5] calib.k_ref_msmt_cov_ctca * INFTY
   * All other elements should be unchanged by the function call (i.e. they should be zero).
   **/
   float32_t expected_r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = { };
   expected_r_mat[2][2] = calib.k_ref_msmt_cov_ctca * (10.0F / 4.0F);
   expected_r_mat[3][3] = calib.k_ref_msmt_cov_ctca * (10.0F / 1.0F);
   expected_r_mat[4][4] = calib.k_ref_msmt_cov_ctca * (10.0F / 10.0F);
   expected_r_mat[5][5] = calib.k_ref_msmt_cov_ctca * INFTY;

   char textFailure[100];
   for (uint8_t i = 0; i < MSMT_UPDATE_MAX_NUM_OF_MSMT; i++)
   {
      for (uint8_t j = 0; j < MSMT_UPDATE_MAX_NUM_OF_MSMT; j++)
      {
         (void)sprintf(textFailure, "Returned r_mat value failed on element row_idx=%d and column_idx=%d: \n", i, j);
         DOUBLES_EQUAL_TEXT(expected_r_mat[i][j], r_mat[i][j], 1e-2F, textFailure);
      }
   }
}
/** @}*/


/** \defgroup  f360_msmt_update_support_functions_common_fill_h_z_mat_single_row_with_rr_info
*  @{
*/

/** \brief
*  
**/
TEST_GROUP(f360_msmt_update_support_functions_ctca_fill_h_z_mat_single_row_with_rr_info)
{
   /** \setup
   * Create an object, detections and return parameters along with expected data.
   **/
   F360_Detection_Props_T det_prop = {};
   rspp_variant_A::RSPP_Detection_T det{};
   F360_Object_Track_T object_track;
   
   uint32_t row_idx = 1U;
   float32_t h_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
   float32_t z_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t zhat_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};

   float32_t test_pass_threshold = 1e-6F;

   float32_t h_row_expected_data[STATE_DIMENSION] = {};
   float32_t z_expected_data;
   float32_t z_hat_expected_data;

   /** \setup
   * Create a track with vcs heading and pointing 90 degrees and place its center rear at position (10, 10) in VCS. Object bounding box dimentsions are set to 7 x 2.5m (length x width) and reference point to FRONT_RIGHT
   * Set track speed to 10 and curvature to 0.01
   * Create a detection on (9, 9) in VCS with vcs azimuth of 45 degrees.
   * Set range rate compensated to 2.0
   */
   TEST_SETUP()
   {
      object_track.vcs_heading = Angle{ F360_DEG2RAD(90.0F) };
      object_track.bbox.Set_Orientation(object_track.vcs_heading);
      object_track.reference_point = F360_REFERENCE_POINT_RIGHT;
      object_track.bbox.Set_Length(7.0F);
      object_track.bbox.Set_Width(2.5F);
      const float32_t obj_rear_center_pos[2] = {10.0F, 10.0F};
      object_track.vcs_position.x = obj_rear_center_pos[0] +  object_track.bbox.Get_Orientation().Cos() * 0.5F * object_track.bbox.Get_Length() - object_track.bbox.Get_Orientation().Sin() * 0.5F * object_track.bbox.Get_Width(); // This makes sure center rear ends up in wanted position
      object_track.vcs_position.y = obj_rear_center_pos[1] +  object_track.bbox.Get_Orientation().Sin() * 0.5F * object_track.bbox.Get_Length() + object_track.bbox.Get_Orientation().Cos() * 0.5F * object_track.bbox.Get_Width(); // This makes sure center rear ends up in wanted position
      object_track.Update_Bbox_Center();
      object_track.speed = 10.0F;
      object_track.curvature = 0.01F;

      det_prop.vcs_position.x = 9.0F;
      det_prop.vcs_position.y = 9.0F;
      det.processed.vcs_az = F360_DEG2RAD(45.0F);
      det.processed.cos_vcs_az = F360_Cosf(det.processed.vcs_az);
      det.processed.sin_vcs_az = F360_Sinf(det.processed.vcs_az);
      det_prop.range_rate_compensated = 2.0F;

      // Expected data calculated by hand
      h_row_expected_data[F360_TRK_FLTR_CTCA_STATE_X] = -0.07071067811F; 
      h_row_expected_data[F360_TRK_FLTR_CTCA_STATE_Y] = 0.07071067811F; 
      h_row_expected_data[F360_TRK_FLTR_CTCA_STATE_H] = -7.23016683764F;
      h_row_expected_data[F360_TRK_FLTR_CTCA_STATE_C] = 0.0F;
      h_row_expected_data[F360_TRK_FLTR_CTCA_STATE_S] = 0.70710678118F; 
      h_row_expected_data[F360_TRK_FLTR_CTCA_STATE_A] = 0.0F;

      z_expected_data = 2.0F;
      z_hat_expected_data = 7.07106781187F;  
   }
};

/*\purpose
* Verify that function Fill_H_Z_Mat_Single_Row_With_RR_Info() works as intended.
*/
TEST(f360_msmt_update_support_functions_ctca_fill_h_z_mat_single_row_with_rr_info, Fill_H_Z_Mat_Single_Row_With_RR_Info)
{
   /** \precond
   Use default test setup from test group
   **/

   /** \action
   * Call the function Fill_H_Z_Mat_Single_Row_With_RR_Info
   **/
   Fill_H_Z_Mat_Single_Row_With_RR_Info_CTCA(object_track, det_prop, det, row_idx, h_mat, z_mat, zhat_mat);

   /** \result
   * Check that correct row in H matrix and correct element in z and z_hat vectors match the expected data.
   **/
   for (uint32_t state_idx = 0U; state_idx < STATE_DIMENSION; state_idx++)
   {
      DOUBLES_EQUAL_TEXT(h_row_expected_data[state_idx], h_mat[row_idx][state_idx], test_pass_threshold, "An element in the H matrix did not match expected data.");
   }

   DOUBLES_EQUAL_TEXT(z_expected_data, z_mat[row_idx], test_pass_threshold, "The element in the measurement vector Z did not match expected data.");
   DOUBLES_EQUAL_TEXT(z_hat_expected_data, zhat_mat[row_idx], test_pass_threshold, "The element in the expected measurement vector z_hat did not match expected data.");
}
/** @}*/

/** \defgroup  f360_msmt_update_support_functions_ctca_optimize_matrix_mulitplications
*  @{
*/

/** \brief
*  This is a test group containing testst of functions that are used to compute run time optimized matrix multiplications
**/
TEST_GROUP(f360_msmt_update_support_functions_ctca_optimize_matrix_mulitplications)
{
   /** \setup
   * Declare matrices to operate on
   **/
   float32_t Hmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION];
   float32_t Pmat[STATE_DIMENSION][STATE_DIMENSION];
   float32_t Rmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT];
   float32_t Kmat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT];

   float32_t HPmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION];

   float32_t HPHTmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT];

   float32_t negKHmat[STATE_DIMENSION][STATE_DIMENSION];

   float32_t KRmat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT];

   uint32_t num_measurements;
   
   float32_t test_pass_threshold = 1e-4F;

   /** \setup
   * Initialize matrices to operate on
   */
   TEST_SETUP()
   {
      // Hmat
      memset(Hmat, 0, sizeof(Hmat));
      Hmat[0][0] = 1.0F;
      Hmat[1][1] = 1.0F;

      Hmat[2][0] = -0.98F;
      Hmat[2][1] = 4.82F;
      Hmat[2][2] = -0.98F;
      Hmat[2][3] = 1.2F;
      Hmat[2][4] = -3.46F;
      Hmat[2][5] = 0.0F;

      Hmat[3][0] = -3.39F;
      Hmat[3][1] = 2.58F;
      Hmat[3][2] = 3.71F;
      Hmat[3][3] = -1.5F;
      Hmat[3][4] = 1.85F;
      Hmat[3][5] = 0.0F;

      Hmat[4][0] = 0.30F;
      Hmat[4][1] = 3.32F;
      Hmat[4][2] = 0.97F;
      Hmat[4][3] = -1.65F;
      Hmat[4][4] = -2.01F;
      Hmat[4][5] = 0.0F;

      num_measurements = 5U;

      // Pmat
      memset(Pmat, 0, sizeof(Pmat));
      Pmat[0][0] = 3.4600F;
      Pmat[0][1] = 2.3400F;
      Pmat[0][2] = -1.10F;
      Pmat[0][3] = -1.2300F;
      Pmat[0][4] =-1.1100F;
      Pmat[0][5] = 0.1100F;

      Pmat[1][0] = Pmat[0][1];
      Pmat[1][1] = 8.859999999999999F;
      Pmat[1][2] = -1.4700F;
      Pmat[1][3] = 1.9900F;
      Pmat[1][4] = -0.0500F;
      Pmat[1][5] = 0.9000F;

      Pmat[2][0] = Pmat[0][2];
      Pmat[2][1] = Pmat[1][2];
      Pmat[2][2] = 4.5400F;
      Pmat[2][3] = 1.1500F;
      Pmat[2][4] = -0.9700F;
      Pmat[2][5] = -0.4500F;

      Pmat[3][0] = Pmat[0][3];
      Pmat[3][1] = Pmat[1][3];
      Pmat[3][2] = Pmat[2][3];
      Pmat[3][3] = 4.1300F;
      Pmat[3][4] = 1.1800F;
      Pmat[3][5] = -0.7400F;

      Pmat[4][0] = Pmat[0][4];
      Pmat[4][1] = Pmat[1][4];
      Pmat[4][2] = Pmat[2][4];
      Pmat[4][3] = Pmat[3][4];
      Pmat[4][4] = 2.1700F;
      Pmat[4][5] = -0.2700F;

      Pmat[5][0] = Pmat[0][5];
      Pmat[5][1] = Pmat[1][5];
      Pmat[5][2] = Pmat[2][5];
      Pmat[5][3] = Pmat[3][5];
      Pmat[5][4] = Pmat[4][5];
      Pmat[5][5] = 1.2500F;

      // Rmat
      memset(Rmat, 0, sizeof(Rmat));
      Rmat[0][0] = 7.39F;
      Rmat[0][1] = 2.01F;
      Rmat[1][0] = Rmat[0][1] ;
      Rmat[1][1] = 9.539999999999999F;
      Rmat[2][2] = 3.56F;
      Rmat[3][3] = 6.62F;
      Rmat[4][4] = 2.81F; 

      // Kmat
      memset(Kmat, 0, sizeof(Kmat));

      Kmat[0][0] = 0.094300000000000F;
      Kmat[0][1] = 0.006100000000000F;
      Kmat[0][2] = -0.023700000000000F;
      Kmat[0][3] = -0.102400000000000F;
      Kmat[0][4] = 0.137900000000000F;

      Kmat[1][0] = 0.018000000000000F;
      Kmat[1][1] = 0.057200000000000F;
      Kmat[1][2] = 0.169800000000000F;
      Kmat[1][3] = 0.008900000000000F;
      Kmat[1][4] = 0.004900000000000F;

      Kmat[2][0] = 0.063900000000000F;
      Kmat[2][1] = -0.066900000000000F;
      Kmat[2][2] = -0.097800000000000F;
      Kmat[2][3] = 0.168800000000000F;
      Kmat[2][4] = 0.107400000000000F;

      Kmat[3][0] = 0.049800000000000F;
      Kmat[3][1] = -0.007300000000000F;
      Kmat[3][2] = 0.171400000000000F;
      Kmat[3][3] = 0.076600000000000F;
      Kmat[3][4] = -0.243100000000000F;

      Kmat[4][0] = 0.006300000000000F;
      Kmat[4][1] = 0.074100000000000F;
      Kmat[4][2] = 0.061100000000000F;
      Kmat[4][3] = 0.018300000000000F;
      Kmat[4][4] = -0.167600000000000F;

      Kmat[5][0] = -0.044100000000000F;
      Kmat[5][1] = 0.010900000000000F;
      Kmat[5][2] = -0.015600000000000F;
      Kmat[5][3] = 0.002100000000000F;
      Kmat[5][4] = 0.061500000000000F;

      // H * P
      memset(HPmat, 0, sizeof(HPmat));
      HPmat[0][0] = 3.460000000000000F;
      HPmat[0][1] = 2.340000000000000F;
      HPmat[0][2] = -1.100000000000000F;
      HPmat[0][3] = -1.230000000000000F;
      HPmat[0][4] = -1.110000000000000F;
      HPmat[0][5] = 0.110000000000000F;
                  
      HPmat[1][0] = 2.340000000000000F;
      HPmat[1][1] = 8.859999999999999F;
      HPmat[1][2] = -1.470000000000000F;
      HPmat[1][3] = 1.990000000000000F;
      HPmat[1][4] = -0.050000000000000F;
      HPmat[1][5] = 0.900000000000000F;
                
      HPmat[2][0] = 11.330600000000000F;
      HPmat[2][1] = 44.413600000000002F;
      HPmat[2][2] = -5.720400000000001F;
      HPmat[2][3] = 10.543400000000000F;
      HPmat[2][4] = -4.294800000000000F;
      HPmat[2][5] = 4.717400000000000F;

      HPmat[3][0] = -9.981700000000002F;
      HPmat[3][1] = 6.395000000000000F;
      HPmat[3][2] = 13.260300000000003F;
      HPmat[3][3] = 9.558399999999999F;
      HPmat[3][4] = 2.279700000000001F;
      HPmat[3][5] = 0.890100000000000F;

      HPmat[4][0] = 12.000399999999999F;
      HPmat[4][1] = 25.508299999999998F;
      HPmat[4][2] = -0.754400000000000F;
      HPmat[4][3] = -1.832999999999999F;
      HPmat[4][4] = -7.748599999999999F;
      HPmat[4][5] = 4.348200000000000F;

      // H * P * H'
      memset(HPHTmat, 0, sizeof(HPHTmat));
      HPHTmat[0][0] = 3.4600000000000F;
      HPHTmat[0][1] = 2.3400000000000F;
      HPHTmat[0][2] = 11.3306000000000F;
      HPHTmat[0][3] = -9.9817000000000F;
      HPHTmat[0][4] = 12.0004000000000F;

      HPHTmat[1][0] = 2.3400000000000F;
      HPHTmat[1][1] = 8.8600000000000F;
      HPHTmat[1][2] = 44.4136000000000F;
      HPHTmat[1][3] = 6.3950000000000F;
      HPHTmat[1][4] = 25.5083000000000F;

      HPHTmat[2][0] = 11.3306000000000F;
      HPHTmat[2][1] = 44.4136000000000F;
      HPHTmat[2][2] = 236.0876440000000F;
      HPHTmat[2][3] = 31.1931900000000F;
      HPHTmat[2][4] = 136.5394820000000F;

      HPHTmat[3][0] = -9.9817000000000F;
      HPHTmat[3][1] = 6.3950000000000F;
      HPHTmat[3][2] = 31.1931900000000F;
      HPHTmat[3][3] = 89.4126210000000F;
      HPHTmat[3][4] = 10.7458240000000F;

      HPHTmat[4][0] = 12.0004000000000F;
      HPHTmat[4][1] = 25.5083000000000F;
      HPHTmat[4][2] = 136.5394820000000F;
      HPHTmat[4][3] = 10.7458240000000F;
      HPHTmat[4][4] = 106.1550440000000F;

      // -K * H'
      memset(negKHmat, 0, sizeof(negKHmat));
      negKHmat[0][0] = -0.506032000000000F;
      negKHmat[0][1] = -0.085502000000000F;
      negKHmat[0][2] = 0.222915000000000F;
      negKHmat[0][3] = 0.102375000000000F;
      negKHmat[0][4] = 0.384617000000000F;
      negKHmat[0][5] = 0.0F;

      negKHmat[1][0] = 0.177105000000000F;
      negKHmat[1][1] = -0.914866000000000F;
      negKHmat[1][2] = 0.128632000000000F;
      negKHmat[1][3] = -0.182325000000000F;
      negKHmat[1][4] = 0.580892000000000F;
      negKHmat[1][5] = 0.0F;

      negKHmat[2][0] = 0.380268000000000F;
      negKHmat[2][1] = -0.253776000000000F;
      negKHmat[2][2] = -0.826270000000000F;
      negKHmat[2][3] = 0.547770000000000F;
      negKHmat[2][4] = -0.434794000000000F;
      negKHmat[2][5] = 0.0F;

      negKHmat[3][0] = 0.450776000000000F;
      negKHmat[3][1] = -0.209384000000000F;
      negKHmat[3][2] = 0.119593000000000F;
      negKHmat[3][3] = -0.491895000000000F;
      negKHmat[3][4] = -0.037297000000000F;
      negKHmat[3][5] = 0.0F;

      negKHmat[4][0] = 0.165895000000000F;
      negKHmat[4][1] = 0.140616000000000F;
      negKHmat[4][2] = 0.154557000000000F;
      negKHmat[4][3] = -0.322410000000000F;
      negKHmat[4][4] = -0.159325000000000F;
      negKHmat[4][5] = 0.0F;

      negKHmat[5][0] = 0.017481000000000F;
      negKHmat[5][1] = -0.145306000000000F;
      negKHmat[5][2] = -0.082734000000000F;
      negKHmat[5][3] = 0.123345000000000F;
      negKHmat[5][4] = 0.065754000000000F;
      negKHmat[5][5] = 0.0F;

      // K * R
      memset(KRmat, 0, sizeof(KRmat));
      KRmat[0][0] = 0.709138000000000F;
      KRmat[0][1] = 0.247737000000000F;
      KRmat[0][2] = -0.084372000000000F;
      KRmat[0][3] = -0.677888000000000F;
      KRmat[0][4] = 0.387499000000000F;

      KRmat[1][0] = 0.247992000000000F;
      KRmat[1][1] = 0.581868000000000F;
      KRmat[1][2] = 0.604488000000000F;
      KRmat[1][3] = 0.058918000000000F;
      KRmat[1][4] = 0.013769000000000F;

      KRmat[2][0] = 0.337752000000000F;
      KRmat[2][1] = -0.509787000000000F;
      KRmat[2][2] = -0.348168000000000F;
      KRmat[2][3] = 1.117456000000000F;
      KRmat[2][4] = 0.301794000000000F;

      KRmat[3][0] = 0.353349000000000F;
      KRmat[3][1] = 0.030456000000000F;
      KRmat[3][2] = 0.610184000000000F;
      KRmat[3][3] = 0.507092000000000F;
      KRmat[3][4] = -0.683111000000000F;

      KRmat[4][0] = 0.195498000000000F;
      KRmat[4][1] = 0.719577000000000F;
      KRmat[4][2] = 0.217516000000000F;
      KRmat[4][3] = 0.121146000000000F;
      KRmat[4][4] = -0.470956000000000F;

      KRmat[5][0] = -0.303990000000000F;
      KRmat[5][1] = 0.015345000000000F;
      KRmat[5][2] = -0.055536000000000F;
      KRmat[5][3] = 0.013902000000000F;
      KRmat[5][4] = 0.172815000000000F;
   }
};

/*\purpose
* Verify that function Hmat_Times_Pmat_CTCA() works as intended
*/
TEST(f360_msmt_update_support_functions_ctca_optimize_matrix_mulitplications, Hmat_Times_Pmat_CTCA)
{
   /** \precond
   Use default setup from test group
   **/

   /** \action
   * Call the function Hmat_Times_Pmat_CTCA()
   **/
   float32_t output_HPmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
   Hmat_Times_Pmat_CTCA(Hmat, Pmat, num_measurements, output_HPmat);

   /** \result
   * Check that output is as expected
   **/

   for (uint32_t meas_idx = 0U; meas_idx < num_measurements; meas_idx++)
   {
      for (uint32_t state_idx = 0U; state_idx < STATE_DIMENSION; state_idx++)
      {
         DOUBLES_EQUAL_TEXT(HPmat[meas_idx][state_idx], output_HPmat[meas_idx][state_idx], test_pass_threshold, "Unexpected output after matrix multipliction");
      }
   }
}

/*\purpose
* Verify that function HPmat_Times_Hmat_Transpose_CTCA() works as intended
*/
TEST(f360_msmt_update_support_functions_ctca_optimize_matrix_mulitplications, HPmat_Times_Hmat_Transpose_CTCA)
{
   /** \precond
   Use default setup from test group
   **/

   /** \action
   * Call the function HPmat_Times_Hmat_Transpose_CTCA()
   **/
   float32_t output_HPHTmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   HPmat_Times_Hmat_Transpose_CTCA(HPmat, Hmat, num_measurements, output_HPHTmat);

   /** \result
   * Check that output is as expected
   **/

   for (uint32_t meas_idx1 = 0U; meas_idx1 < num_measurements; meas_idx1++)
   {
      for (uint32_t meas_idx2 = 0U; meas_idx2 < num_measurements; meas_idx2++)
      {
         DOUBLES_EQUAL_TEXT(HPHTmat[meas_idx1][meas_idx2], output_HPHTmat[meas_idx1][meas_idx2], test_pass_threshold, "Unexpected output after matrix multipliction");
      }
   }
}

/*\purpose
* Verify that function Negative_Kmat_Times_Hmat_CTCA() works as intended
*/
TEST(f360_msmt_update_support_functions_ctca_optimize_matrix_mulitplications, Negative_Kmat_Times_Hmat_CTCA)
{
   /** \precond
   Use default setup from test group
   **/

   /** \action
   * Call the function Negative_Kmat_Times_Hmat_CTCA()
   **/
   float32_t output_negKHmat[STATE_DIMENSION][STATE_DIMENSION] = {};
   Negative_Kmat_Times_Hmat_CTCA(Kmat, Hmat, num_measurements, output_negKHmat);

   /** \result
   * Check that output is as expected
   **/

   for (uint32_t state_idx1 = 0U; state_idx1 < STATE_DIMENSION; state_idx1++)
   {
      for (uint32_t state_idx2 = 0U; state_idx2 < STATE_DIMENSION; state_idx2++)
      {
         DOUBLES_EQUAL_TEXT(negKHmat[state_idx1][state_idx2], output_negKHmat[state_idx1][state_idx2], test_pass_threshold, "Unexpected output after matrix multipliction");
      }
   }
}

/*\purpose
* Verify that function Kmat_Times_Rmat_CTCA() works as intended
*/
TEST(f360_msmt_update_support_functions_ctca_optimize_matrix_mulitplications,Kmat_Times_Rmat_CTCA)
{
   /** \precond
   Use default setup from test group
   **/

   /** \action
   * Call the function Kmat_Times_Rmat_CTCA()
   **/
   float32_t output_KRmat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   Kmat_Times_Rmat_CTCA(Kmat, Rmat, num_measurements, output_KRmat);

   /** \result
   * Check that output is as expected
   **/

   for (uint32_t state_idx = 0U; state_idx < STATE_DIMENSION; state_idx++)
   {
      for (uint32_t meas_idx = 0U; meas_idx < num_measurements; meas_idx++)
      {
         DOUBLES_EQUAL_TEXT(KRmat[state_idx][meas_idx], output_KRmat[state_idx][meas_idx], test_pass_threshold, "Unexpected output after matrix multipliction");
      }
   }
}
/** @}*/
