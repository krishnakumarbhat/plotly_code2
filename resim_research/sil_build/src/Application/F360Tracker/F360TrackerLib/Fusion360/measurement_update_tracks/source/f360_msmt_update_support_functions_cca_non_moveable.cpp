/*===========================================================================*\
* FILE: f360_msmt_update_support_functions_cca_non_moveable.cpp
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function related to Kalman measurement update step for non-moveable CCA objects
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_msmt_update_support_functions_cca_non_moveable.h" 
#include "f360_msmt_update_support_functions_common.h" 
#include "f360_trk_fltr_cca_states.h"
#include "f360_pseudo_msmt.h"
#include "f360_math_func.h"
#include "f360_LinearSolvers.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   /*===========================================================================*\
    * FUNCTION: Kalman_Gain_Update_CCA_Non_Moveable()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const float32_t (&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION]                             - observation model matrix
    * const float32_t (&p_mat)[STATE_DIMENSION][STATE_DIMENSION]                                                      - state covariance
    * const float32_t (&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]    - pseudo-measurement covariance of measurement uncertainty
    * float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]                                   - kalman gain
    * float32_t (&s_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]          - innovation covaraince
    *
    *
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function updates the Kalman gain for an object.
    *
    * PRECONDITIONS:
    * r_mat (3x3) must have the following structure:
    *    r_mat = [* * 0
    *             * * 0
    *             0 0 *]
    *
    * h_mat (3x6) must have the following structure
    *    h_mat = [1 0 0 0 0 0
    *             0 0 0 1 0 0
    *             0 * 0 0 * 0]
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/

   void Kalman_Gain_Update_CCA_Non_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&s_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]
   )
   {
      // KF equation: H * P
      float32_t temp_hp_mat[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION];
      Hmat_Times_Pmat_CCA_Non_Moveable(h_mat, p_mat, temp_hp_mat);

      // KF equation: H * P * H' + R
      Smat_CCA_Non_Moveable(h_mat, p_mat, r_mat, s_mat);

      // Use LDL solver for solving linear equation
      // KF equation: P*H'*inv(S) = (H*P)'*inv(S)
      Matrix_Division(s_mat, temp_hp_mat, MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE, STATE_DIMENSION, k_mat);
   }


   /*===========================================================================*\
   * FUNCTION: Error_Cov_Update_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]                             - kalman gain
   * const float32_t (&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]    - covariance of measurement uncertainty
   * const float32_t (&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION]                             - observation model matrix
   * float32_t (&p_mat)[STATE_DIMENSION][STATE_DIMENSION]                                                            - state error covariance matrix, updated in this fuction
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function updates an object's error covariance matrix.
   * This Measurement update step is performed through the Joseph form of covariance update to avoid numerical instability
   * P_update = (I - KH) * P * (I - KH)' + KRK'
   *
   * r_mat must have the following structure:
   *    r_mat = [* * 0
   *             * * 0
   *             0 0 *]
   *
   * h_mat must have the following structure
   *    h_mat = [1 0 0 0 0 0
   *             0 0 0 1 0 0
   *             0 * 0 0 * 0]
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Error_Cov_Update_CCA_Non_Moveable(
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      // KF equation: ikh_mat = I - (K * H)
      float32_t ikh_mat[STATE_DIMENSION][STATE_DIMENSION];
      I_Minus_Kmat_Times_Hmat_CCA_Non_Moveable(h_mat, k_mat, ikh_mat);

      // KF equation: ikhp_mat = (I - (K * H)) * P
      float32_t ikhp_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
      IKHmat_Times_Pmat_CCA_Non_Moveable(ikh_mat, p_mat, ikhp_mat);

      // KF equation: ikhp_ikh_mat = ((I - (K * H)) * P) * (I - K * H)'
      float32_t ikhp_ikh_mat[STATE_DIMENSION][STATE_DIMENSION];
      IKHPmat_Times_IKHmat_Transpose_CCA_Non_Moveable(ikhp_mat, ikh_mat, ikhp_ikh_mat);

      // KF equation: krk_mat = K * R * K'
      float32_t krk_mat[STATE_DIMENSION][STATE_DIMENSION];
      Kmat_Times_Rmat_Times_Kmat_CCA_Non_Moveable(k_mat, r_mat, krk_mat);

      // KF equation: p_mat =  ((I - (K * H)) * P) * (I - K * H)'  +  K * R * K'
      F360_Matadd_6x6_6x6(ikhp_ikh_mat, krk_mat, p_mat);
   }


   /*===========================================================================*\
   * FUNCTION: State_Update_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&z_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]                      - pseudo measurement vector
   * const float32_t (&zhat_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]                   - predicted measurement (previous state)
   * const float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]     - kalman gain
   * float32_t (&state)[STATE_DIMENSION]                                                     - object's current state vector
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function updates the current state of an object.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void State_Update_CCA_Non_Moveable(
      const float32_t(&z_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&zhat_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&state)[STATE_DIMENSION])
   {

      // The Kalman filter equation implemented below is x_updated = x_predicted + K*(z - z_pred)
      for (uint32_t row = 0U; row < STATE_DIMENSION; row++)
      {
         float32_t state_increment = 0.0F;
         for (uint32_t col = 0U; col < MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE; col++)
         {
            state_increment += k_mat[row][col] * (z_mat[col] - zhat_mat[col]);
         }
         state[row] += state_increment;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Hmat_Times_Pmat_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&Hmat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CCA Kalman filter update.
   * const float32_t(&Pmat)[STATE_DIMENSION][STATE_DIMENSION] - the P matrix (state error covariance) in the CCA Kalman filter update
   * float32_t(&HPmat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION]) - the resulting matrix multiplcation of Hmat * Pmat
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the matrix multiplication of Hmat * Pmat in the CCA non_moveable KF measurement update step
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Hmat_Times_Pmat_CCA_Non_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&hp_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION])
   {
      hp_mat[0][0] = p_mat[0][0];
      hp_mat[0][1] = p_mat[0][1];
      hp_mat[0][2] = p_mat[0][2];
      hp_mat[0][3] = p_mat[0][3];
      hp_mat[0][4] = p_mat[0][4];
      hp_mat[0][5] = p_mat[0][5];

      hp_mat[1][0] = p_mat[3][0];
      hp_mat[1][1] = p_mat[3][1];
      hp_mat[1][2] = p_mat[3][2];
      hp_mat[1][3] = p_mat[3][3];
      hp_mat[1][4] = p_mat[3][4];
      hp_mat[1][5] = p_mat[3][5];

      hp_mat[2][0] = h_mat[2][1] * p_mat[1][0] + h_mat[2][4] * p_mat[4][0];
      hp_mat[2][1] = h_mat[2][1] * p_mat[1][1] + h_mat[2][4] * p_mat[4][1];
      hp_mat[2][2] = h_mat[2][1] * p_mat[1][2] + h_mat[2][4] * p_mat[4][2];
      hp_mat[2][3] = h_mat[2][1] * p_mat[1][3] + h_mat[2][4] * p_mat[4][3];
      hp_mat[2][4] = h_mat[2][1] * p_mat[1][4] + h_mat[2][4] * p_mat[4][4];
      hp_mat[2][5] = h_mat[2][1] * p_mat[1][5] + h_mat[2][4] * p_mat[4][5];
   }

   /*===========================================================================*\
   * FUNCTION: Smat_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CCA Kalman filter update.
   * const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION] - the P matrix (state error covariance) in the CCA Kalman filter update
   * const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE] - the R matrix, i.e measurement noise matrix
   * float32_t(&s_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]) - the resulting innovation matrix
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the innovation matrix in the CCA non_moveable KF measurement update step
   * The following matrix multiplication is done here:  H * P * H' + R
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Smat_CCA_Non_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&s_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE])
   {
      s_mat[0][0] = p_mat[0][0] + r_mat[0][0];
      s_mat[0][1] = p_mat[0][3] + r_mat[0][1];
      s_mat[0][2] = h_mat[2][1] * p_mat[0][1] + h_mat[2][4] * p_mat[0][4];

      s_mat[1][0] = p_mat[3][0] + r_mat[1][0];
      s_mat[1][1] = p_mat[3][3] + r_mat[1][1];
      s_mat[1][2] = h_mat[2][1] * p_mat[3][1] + h_mat[2][4] * p_mat[3][4];

      s_mat[2][0] = h_mat[2][1] * p_mat[1][0] + h_mat[2][4] * p_mat[4][0];
      s_mat[2][1] = h_mat[2][1] * p_mat[1][3] + h_mat[2][4] * p_mat[4][3];
      s_mat[2][2] = r_mat[2][2] + h_mat[2][1] * (h_mat[2][1] * p_mat[1][1] + h_mat[2][4] * p_mat[4][1]) + h_mat[2][4] * (h_mat[2][1] * p_mat[1][4] + h_mat[2][4] * p_mat[4][4]);
   }


   /*===========================================================================*\
   * FUNCTION: I_Minus_Kmat_Times_Hmat_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CCA Kalman filter update.
   * const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE] - the K matrix, i.e measurement noise matrix
   * float32_t(&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION]) - the resulting matrix
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the matrix multiplication for the CCA non_moveable KF measurement update step
   * The following matrix multiplication is done here:  (I - K*H)
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void I_Minus_Kmat_Times_Hmat_CCA_Non_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      ikh_mat[0][0] = 1.0F - k_mat[0][0];
      ikh_mat[0][1] = -h_mat[2][1] * k_mat[0][2];
      ikh_mat[0][2] = 0.0F;
      ikh_mat[0][3] = -k_mat[0][1];
      ikh_mat[0][4] = -h_mat[2][4] * k_mat[0][2];
      ikh_mat[0][5] = 0.0F;

      ikh_mat[1][0] = -k_mat[1][0];
      ikh_mat[1][1] = 1.0F - h_mat[2][1] * k_mat[1][2];
      ikh_mat[1][2] = 0.0F;
      ikh_mat[1][3] = -k_mat[1][1];
      ikh_mat[1][4] = -h_mat[2][4] * k_mat[1][2];
      ikh_mat[1][5] = 0.0F;

      ikh_mat[2][0] = -k_mat[2][0];
      ikh_mat[2][1] = -h_mat[2][1] * k_mat[2][2];
      ikh_mat[2][2] = 1.0F;
      ikh_mat[2][3] = -k_mat[2][1];
      ikh_mat[2][4] = -h_mat[2][4] * k_mat[2][2];
      ikh_mat[2][5] = 0.0F;

      ikh_mat[3][0] = -k_mat[3][0];
      ikh_mat[3][1] = -h_mat[2][1] * k_mat[3][2];
      ikh_mat[3][2] = 0.0F;
      ikh_mat[3][3] = 1.0F - k_mat[3][1];
      ikh_mat[3][4] = -h_mat[2][4] * k_mat[3][2];
      ikh_mat[3][5] = 0.0F;

      ikh_mat[4][0] = -k_mat[4][0];
      ikh_mat[4][1] = -h_mat[2][1] * k_mat[4][2];
      ikh_mat[4][2] = 0.0F;
      ikh_mat[4][3] = -k_mat[4][1];
      ikh_mat[4][4] = 1.0F - h_mat[2][4] * k_mat[4][2];
      ikh_mat[4][5] = 0.0F;

      ikh_mat[5][0] = -k_mat[5][0];
      ikh_mat[5][1] = -h_mat[2][1] * k_mat[5][2];
      ikh_mat[5][2] = 0.0F;
      ikh_mat[5][3] = -k_mat[5][1];
      ikh_mat[5][4] = -h_mat[2][4] * k_mat[5][2];
      ikh_mat[5][5] = 1.0F;
   }


   /*===========================================================================*\
   * FUNCTION: IKHmat_Times_Pmat_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CCA Kalman filter update.
   * const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE] - the K matrix, i.e measurement noise matrix
   * float32_t(&ikhp_mat)[STATE_DIMENSION][STATE_DIMENSION]) - the resulting matrix
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the matrix multiplication for the CCA non_moveable KF measurement update step
   * The following matrix multiplication is done here:  (I - K*H)
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void IKHmat_Times_Pmat_CCA_Non_Moveable(
      const float32_t(&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&ikhp_mat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      uint32_t row = 0U;
      uint32_t col = 0U;

      for (uint32_t i = 0U; i < STATE_DIMENSION * STATE_DIMENSION; i++)
      {
         ikhp_mat[row][col] =
            ikh_mat[row][0] * p_mat[0][col] +
            ikh_mat[row][1] * p_mat[1][col] +
            ikh_mat[row][3] * p_mat[3][col] +
            ikh_mat[row][4] * p_mat[4][col];

         col++;
         const bool f_row_done = (col == STATE_DIMENSION);
         col = (f_row_done) ? 0U : col;
         row = (f_row_done) ? (row + 1U) : row;
      }

      for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
      {
         ikhp_mat[2][i] += p_mat[2][i];
         ikhp_mat[5][i] += p_mat[5][i];
      }
   }

   /*===========================================================================*\
   * FUNCTION: IKHPmat_Times_IKHmat_Transpose_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&ikhp_mat)[STATE_DIMENSION][STATE_DIMENSION]
   * const float32_t(&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION]
   * float32_t(&ikhp_ikh_mat)[STATE_DIMENSION][STATE_DIMENSION]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the matrix multiplication for the CCA non_moveable KF measurement update step
   * The following matrix multiplication is done here:  ikhp_ikh_mat = ((I - (K * H)) * P) * (I - K * H)'
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void IKHPmat_Times_IKHmat_Transpose_CCA_Non_Moveable(
      const float32_t(&ikhp_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&ikhp_ikh_mat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      uint32_t row = 0U;
      uint32_t col = 0U;

      for (uint32_t i = 0U; i < STATE_DIMENSION * STATE_DIMENSION; i++)
      {
         ikhp_ikh_mat[row][col] =
            ikh_mat[col][0] * ikhp_mat[row][0] +
            ikh_mat[col][1] * ikhp_mat[row][1] +
            ikh_mat[col][3] * ikhp_mat[row][3] +
            ikh_mat[col][4] * ikhp_mat[row][4];

         col++;
         const bool f_row_done = (col == STATE_DIMENSION);
         col = (f_row_done) ? 0U : col;
         row = (f_row_done) ? (row + 1U) : row;
      }

      for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
      {
         ikhp_ikh_mat[i][2] += ikhp_mat[i][2];
         ikhp_ikh_mat[i][5] += ikhp_mat[i][5];
      }
   }


   /*===========================================================================*\
   * FUNCTION: Kmat_Times_Rmat_Times_Kmat_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]
   * const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]
   * float32_t(&krk_mat)[STATE_DIMENSION][STATE_DIMENSION])
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes the matrix multiplication for the CCA non_moveable KF measurement update step
   * The following matrix multiplication is done here:  krk_mat = K * R * K'
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Kmat_Times_Rmat_Times_Kmat_CCA_Non_Moveable(
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&krk_mat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      float32_t rk_t[3][6];
      F360_Matmul_MxN_PxN_Transpose(r_mat, k_mat, rk_t);
      F360_Matmul_MxN_NxP(k_mat, rk_t, krk_mat);
   }

   /*===========================================================================*\
   * FUNCTION: Generate_Single_Pseudo_Range_Rate_Compensated_Measurement_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const uint32_t(&dets_idx)[MAX_DETS_IN_OBJ_TRK]
   * const uint32_t num_dets
   * float32_t& mean_comp_rdot
   * float32_t& mean_cos_vcs_az
   * float32_t& mean_sin_vcs_az
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function takes an array of detection indexes and computes the mean
   * compensated range rate, the mean cos(vcs_azimuth) and the mean sin(vcs_azimuth).
   *
   * PRECONDITIONS:
   * Number of detections must be larger than 0.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Generate_Single_Pseudo_Range_Rate_Compensated_Measurement_CCA(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const uint32_t(&dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t num_dets,
      float32_t& mean_comp_rdot,
      float32_t& mean_cos_vcs_az,
      float32_t& mean_sin_vcs_az)
   {
      // Use mean of compensated range rate detections as measurement
      float32_t sum_comp_rdot = 0.0F;
      float32_t sum_cos_vcs_az = 0.0F;
      float32_t sum_sin_vcs_az = 0.0F;

      for (uint32_t i = 0U; i < num_dets; i++)
      {
         const uint32_t det_idx = dets_idx[i];

         sum_comp_rdot += det_props[det_idx].range_rate_compensated;
         sum_cos_vcs_az += raw_detection_list.detections[det_idx].processed.cos_vcs_az;
         sum_sin_vcs_az += raw_detection_list.detections[det_idx].processed.sin_vcs_az;
      }

      const float32_t inv_selected_dets_num = 1.0F / static_cast<float32_t>(num_dets);
      mean_comp_rdot = sum_comp_rdot * inv_selected_dets_num;
      mean_cos_vcs_az = sum_cos_vcs_az * inv_selected_dets_num;
      mean_sin_vcs_az = sum_sin_vcs_az * inv_selected_dets_num;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Obj_Suspected_Moveable_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK] - reference to an array containing the indexes of the detections that
   *                                                               are used to perform KF measurement update on the object
   * const uint32_t selected_dets_num - number of detections that are used to perform KF measurement update on the object
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks if there are indications that an object can be moveable.
   * The object movable_prob < 0.5F as the precondition.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   bool Is_Obj_Suspected_Moveable_CCA(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t selected_dets_num)
   {
      // Check if any of the detections are classified as moving
      bool f_suspected_moveable = false;
      for (uint32_t i = 0U; i < selected_dets_num; i++)
      {
         const uint32_t det_idx = selected_dets_idx[i];

         if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_props[det_idx].motion_status)
         {
            f_suspected_moveable = true;
            break;
         }
      }

      return f_suspected_moveable;
   }


   /*===========================================================================*\
   * FUNCTION: Limit_Pseudo_Pos_Meas_Impact_On_Obj_Vel_Estimate_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object_track - referense to a single object
   * const float32_t host_yaw_rate - Host yaw rate in radians
   * const F360_Calibrations_T& calib - reference to data structure containing calibration parameters
   * float32_t(&k_mat)[STATE_DIMENSION_CCA][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE] - reference to the KF gain matrix
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function modifies some of the elements in the Kalman Filter gain matrix.
   * The matrix elements that corresponds to how much the pseudo position
   * measurement will impact the object velocity estimate are decreased by a factor.
   * The decrease factor is in the range of [0, 1] and is dependent on the position
   * innovation (i.e. the difference between the time predicted object position and
   * the position pseudo measurement). If the position innovation is small, i.e. the
   * position seems to be changing smoothly, then the decreas factor will be close to
   * 1 meaning that the Kalman gain elements will be modified very little. When the
   * position innovation is larger, i.e. there seems to be a jump in the position, then
   * the decrease factor is closer to 0 meaning that the position measurement will not
   * be allowed to influence the object velocity estimate to as large extent.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Limit_Pseudo_Pos_Meas_Impact_On_Obj_Vel_Estim_CCA(
      const F360_Object_Track_T& object_track,
      const float32_t host_yaw_rate,
      const F360_Calibrations_T& calib,
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE])
   {
      float32_t decrease_factor_x;
      float32_t decrease_factor_y;
      Compute_KF_Gain_Decrease_Factor_CCA(object_track, host_yaw_rate, calib, decrease_factor_x, decrease_factor_y);

      k_mat[F360_TRK_FLTR_CCA_STATE_VX][0] *= decrease_factor_x; // Matrix column 0 corresponds to the longitudinal pseudo position measurement
      k_mat[F360_TRK_FLTR_CCA_STATE_VX][1] *= decrease_factor_x; // Matrix column 1 corresponds to the lateral pseudo position measurement
      k_mat[F360_TRK_FLTR_CCA_STATE_VY][0] *= decrease_factor_y; // Matrix column 0 corresponds to the longitudinal pseudo position measurement
      k_mat[F360_TRK_FLTR_CCA_STATE_VY][1] *= decrease_factor_y; // Matrix column 1 corresponds to the lateral pseudo position measurement
      k_mat[F360_TRK_FLTR_CCA_STATE_AX][0] *= decrease_factor_x; // Matrix column 0 corresponds to the longitudinal pseudo position measurement
      k_mat[F360_TRK_FLTR_CCA_STATE_AX][1] *= decrease_factor_x; // Matrix column 1 corresponds to the lateral pseudo position measurement
      k_mat[F360_TRK_FLTR_CCA_STATE_AY][0] *= decrease_factor_y; // Matrix column 0 corresponds to the longitudinal pseudo position measurement
      k_mat[F360_TRK_FLTR_CCA_STATE_AY][1] *= decrease_factor_y; // Matrix column 1 corresponds to the lateral pseudo position measurement
   }



   /*===========================================================================*\
   * FUNCTION: Compute_KF_Gain_Decrease_Factor_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object_track - reference to a single object
   * const float32_t host_yaw_rate - Host yaw rate in radians
   * const F360_Calibrations_T& calib - reference to data structure containing calibration parameters
   * float32_t& decrease_factor_x -  Decrease factor in x dimension
   * float32_t& decrease_factor_y - Decrease factor in y dimension
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes a decrease factor in the range of [0, 1]. This decrease
   * factor can be used to multiply certain elements of the KF gain matrix with in
   * order to limit how much the position measurement is allowed to impact the object
   * velocity estimate.
   *
   * The decrease factor is dependent on the position innovation (i.e. the difference
   * between the time predicted object position and the position pseudo measurement).
   * If the innovation is smaller than the variable k_max_innov_dist_thres
   * then the decrease factor will be set to 1. If the innovation is between the
   * variables 0.0F and k_max_innov_dist_thres then the decrease factor will decay linearly.
   * If the correction from pseudo position is to reduce the velocity and acceleration,
   * this is indicated by the innovation sign being the same as velocity,
   * then the decrease factor is 1.0. This means that there will be no limit on
   * pseudo position impact on acceleration and velocity estimate.
   * The max innovation thresholds consist of their respective constant calibrations
   * and decays with host yaw rate.
   * When the innovation is larger than the calibration value k_max_innov_dist_thres
   * then the decrease factor will take the smallest value given by the calibration
   * parameter k_min_decreas_factor.
   *
   * Further, for objects with high otg_height, the decrease factor is reduced linearly
   * with the otg_height such that objects with otg_height <= 5.0m will be unaffected and
   * objects with otg_height > 10.0m will have the decrease factor set to 0.001.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Compute_KF_Gain_Decrease_Factor_CCA(
      const F360_Object_Track_T& object_track,
      const float32_t host_yaw_rate,
      const F360_Calibrations_T& calib,
      float32_t& decrease_factor_x,
      float32_t& decrease_factor_y)
   {
      // Limit how much position measurements are allowed to influence velocity estimates
      // for non-moveable objects with only stationary associated detections
      constexpr float32_t min_velocity_threshold = 0.3F;
      const float32_t innov_long_pos = object_track.pseudo_vcs_position.x - object_track.vcs_position.x;
      const float32_t innov_lat_pos = object_track.pseudo_vcs_position.y - object_track.vcs_position.y;

      // Check if the correction from pseudo position will decrease the velocity i.e sign of innovation is opposite to velocity 
      // f_long_pseudo_pos_correction_opposite_xvel is true if innov_long_pos has the opposite sign as compared to xvel and if xvel is greater than 0.3 m/s
      // f_lat_pseudo_pos_correction_opposite_yvel is true if innov_lat_pos has the opposite sign as compared to yvel and if yvel is greater than 0.3 m/s
      const bool f_long_pseudo_pos_correction_opposite_xvel = (((object_track.vcs_velocity.longitudinal * innov_long_pos) < 0.0F) && (std::abs(object_track.vcs_velocity.longitudinal) > min_velocity_threshold));
      const bool f_lat_pseudo_pos_correction_opposite_yvel = (((object_track.vcs_velocity.lateral * innov_lat_pos) < 0.0F) && (std::abs(object_track.vcs_velocity.lateral) > min_velocity_threshold));

      constexpr float32_t max_height_for_height_factor = 10.0F;
      if (object_track.otg_height > max_height_for_height_factor)
      {
         //If object height is over max, set to 0.0 and skip decrease factor calculation
         decrease_factor_x = f_long_pseudo_pos_correction_opposite_xvel ? 1.0F : 0.0F;
         decrease_factor_y = f_lat_pseudo_pos_correction_opposite_yvel ? 1.0F : 0.0F;
      }
      else
      {
         const float32_t k_max_innov_dist_thres = std::max(0.0F, calib.k_max_innov_dist_yaw_rate_gain * std::abs(host_yaw_rate) + calib.k_max_innov_dist_thres);
         const float32_t decrease_factor_temp_x = F360_Linear_Equation_With_Saturation(std::abs(innov_long_pos), 0.0F, k_max_innov_dist_thres, 1.0F, calib.k_min_decrease_factor);
         const float32_t decrease_factor_temp_y = F360_Linear_Equation_With_Saturation(std::abs(innov_lat_pos), 0.0F, k_max_innov_dist_thres, 1.0F, calib.k_min_decrease_factor);

         // Calculate height factor for high objects to further reduce the decrease factor
         constexpr float32_t min_height_for_height_factor = 5.0F;
         float32_t height_factor = 1.0F;
         if (object_track.otg_height > min_height_for_height_factor)
         {
            constexpr float32_t min_height_factor = 0.001F;
            constexpr float32_t max_height_factor = 1.0F;
            // The height factor is restricted between [0.001, 1.0]
            height_factor = F360_Linear_Equation_With_Saturation(object_track.otg_height, min_height_for_height_factor, max_height_for_height_factor, max_height_factor, min_height_factor);
         }
         decrease_factor_x = f_long_pseudo_pos_correction_opposite_xvel ? 1.0F : height_factor * decrease_factor_temp_x;
         decrease_factor_y = f_lat_pseudo_pos_correction_opposite_yvel ? 1.0F : height_factor * decrease_factor_temp_y;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Check_For_Crossing_VRU_Type_Obj()
   *===========================================================================
   * RETURN VALUE:
   * bool -  f_crossing_moveable
   *
   * PARAMETERS:
   * const float32_t host_curvature_rear
   * const F360_Calibrations_T& calib
   * F360_Object_Track_T& object_track
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks if an object is moving laterally, based on change in
   * vcs_position.y over 5 scans
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Check_For_Crossing_VRU_Type_Obj(
      const float32_t host_curvature_rear,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object_track)
   {
      // Threshold Paramerters
      const float32_t longitudinal_pos_threshold = 90.0F;
      const float32_t lateral_pos_threshold = 25.0F;
      const float32_t host_curvature_threshold = 0.007F;
      const float32_t obj_height_threshold = 5.0F;
      const float32_t longitudinal_speed_threshold = 1.0F;

      bool f_crossing_moveable = false;
      if ((std::abs(object_track.vcs_heading.Value()) > calib.k_object_motion_cross_moving_min_abs_vcs_heading_th)
         && (std::abs(object_track.vcs_position.x) < longitudinal_pos_threshold)
         && (std::abs(object_track.vcs_position.y) < lateral_pos_threshold)
         && (std::abs(host_curvature_rear) < host_curvature_threshold)
         && (object_track.otg_height < obj_height_threshold) && (std::abs(object_track.vcs_velocity.longitudinal) < longitudinal_speed_threshold))
      {
         const uint8_t buffer_index = object_track.cca_cross_moving_buffer_index;
         const float32_t lateral_diff = object_track.vcs_position.y - object_track.prev_predicted_vcs_y_pos;
         const int8_t lateral_diff_sign = static_cast<int8_t>(F360_Sign(lateral_diff));
         const float32_t dist_to_host_sq = F360_Get_Hypotenuse_Squared(object_track.vcs_position.x, object_track.vcs_position.y);
         const uint8_t buffer_size = dist_to_host_sq > 2500.0F ? 20U : 5U;

         if (std::abs(lateral_diff) > 0.05F)
         {
            object_track.cca_cross_moving_buffer[buffer_index] = lateral_diff_sign;
         }
         else
         {
            object_track.cca_cross_moving_buffer[buffer_index] = 0;
         }

         // pointer to the next position to fill
         object_track.cca_cross_moving_buffer_index += 1U;
         if (object_track.cca_cross_moving_buffer_index >= buffer_size)
         {
            object_track.cca_cross_moving_buffer_index = 0U;
         }

         int8_t cca_cross_moving_buffer_sum = 0;
         bool f_any_zero = false;

         for (uint8_t idx = 0U; idx < buffer_size; idx++)
         {
            cca_cross_moving_buffer_sum += object_track.cca_cross_moving_buffer[idx];
            if (object_track.cca_cross_moving_buffer[idx] == 0)
            {
               f_any_zero = true;
            }
         }
         const int32_t min_moving_scans_threshold = static_cast <int32_t>(dist_to_host_sq > 2500.0F ? static_cast<int32_t>(13) : static_cast<int32_t>(3));
         const int32_t max_moving_scans_threshold = static_cast <int32_t> (dist_to_host_sq > 2500.0F ? static_cast<int32_t>(17) : static_cast<int32_t>(4));
         const int32_t moving_scans_threshold = (f_any_zero) ? max_moving_scans_threshold : min_moving_scans_threshold;

         if (std::abs(static_cast<int32_t>(cca_cross_moving_buffer_sum)) >= moving_scans_threshold)
         {
            f_crossing_moveable = true;
         }
      }

      return f_crossing_moveable;
   }

   /*===========================================================================*\
   * FUNCTION: Force_Pseudo_Pos_Vel_Lim_Impact_Logic()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const float32_t host_speed,
   * const F360_Object_Track_T& object_track,
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
   * const uint32_t selected_dets_num)
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Purpose of this function is to not allow close by stationary objects to build velocity and become movable obj due to association
   * with noisy ambiguous or stationary ground dets in vicinity when host is slow moving (eg.-tram track dets associating with stat obj)
   * Condition for slow host speed is a safety consideration to allow time for reactive action of feature functions on actual slow moving
   * object such as pedestrian showing up with lower speed than actual if it mistakenly passes through this logic in possible edge cases
   * (when proactive or predictive action might not happen due to low object speed).
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Force_Pseudo_Pos_Vel_Lim_Impact_Logic(
      const float32_t host_speed,
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t selected_dets_num)
   {
      // Parameters for forcing logic for velocity estimate of obj to have limited impact from pseudopos (other conditions can also allow logic to trigger even when these params are not fulfilled)

      const float32_t max_abs_y_pos = 10.0F; // [m] Max distance for forcing velocity estimate of obj to have limited impact from pseudopos
      const float32_t max_abs_x_pos = 10.0F;
      const float32_t max_otg_height = 0.2F; // targeting objects from ground detections
      constexpr float32_t max_host_speed = F360_KPH2MPS(20.0F); // [m/s] Max host speed for forcing velocity estimate of obj to have limited impact from pseudopos

      const bool f_position_and_speed_cond_fulfilled = ((object_track.vcs_position.x < max_abs_x_pos) &&
         (0.0F < object_track.vcs_position.x) &&
         (std::abs(object_track.vcs_position.y) < max_abs_y_pos) &&
         (std::abs(host_speed) < max_host_speed) &&
         (object_track.otg_height < max_otg_height));

      bool f_rr_cond_fulfilled = false;

      if (f_position_and_speed_cond_fulfilled)
      {
         constexpr float32_t max_rr_comp = 0.1F; // [m/s] Max compensated range rate limit for all associated dets in scan for forcing velocity estimate of obj to have limited impact from pseudopos

         // Loop over all selected dets in this scan and check if they are all below rr threshold
         for (uint32_t i = 0U; i < selected_dets_num; i++)
         {
            const uint32_t det_idx = selected_dets_idx[i];

            if (std::abs(det_props[det_idx].range_rate_compensated) < max_rr_comp)
            {
               f_rr_cond_fulfilled = true;
            }
            else
            {
               f_rr_cond_fulfilled = false;
               break;
            }
         }
      }

      // If all conditions met, then return TRUE
      return (f_position_and_speed_cond_fulfilled && f_rr_cond_fulfilled);
   }
}
