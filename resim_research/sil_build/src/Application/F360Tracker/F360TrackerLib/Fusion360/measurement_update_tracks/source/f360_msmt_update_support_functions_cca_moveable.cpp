/*===========================================================================*\
* FILE: f360_msmt_update_support_functions_cca_moveable.cpp
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function related to Kalman measurement update step of CCA objects
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_msmt_update_support_functions_cca_moveable.h" 
#include "f360_msmt_update_support_functions_common.h" 
#include "f360_trk_fltr_cca_states.h"
#include "f360_pseudo_msmt.h"
#include "f360_get_reference_point_para_side.h"
#include "f360_norm_heading_angle.h"
#include "f360_math_func.h"
#include "f360_LinearSolvers.h"
#include "f360_reference_point_support_functions.h"

namespace f360_variant_A
{

   static F360_Reference_Point_T Flip_Reference_Point(
      const F360_Reference_Point_T old_ref_point);

  /*===========================================================================*\
   * FUNCTION: Kalman_Gain_Update_CCA_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION]              - observation model matrix
   * const float32_t (&p_mat)[STATE_DIMENSION][STATE_DIMENSION]                          - state covariance
   * const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]  - pseudo-measurement covariance of measurement uncertainty
   * const uint32_t nr_total_msnmts                                                      - number of valid measurements
   * float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]                    - kalman gain
   * float32_t (&s_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]        - innovation covaraince
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
   *    r_mat must have the following structure:
   *    r_mat = [* * 0 0 0 0 ....
   *             * * 0 0 0 0 ....
   *             0 0 * 0 0 0 ....
   *             . . . . . .
   *             . . . . . .
   *             . . . . . .]
   * h_mat must have the following structure
   *    h_mat = [1 0 0 0 0 0
   *             0 0 0 1 0 0
   *             0 * 0 0 * 0 (this row is repeaded for each present range rate measurement)
   *             . . . . . .
   *             . . . . . .
   *             . . . . . .]
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Kalman_Gain_Update_CCA_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t nr_total_msnmts,
      const bool& obj_ok_to_decay_states,
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&s_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   )
   {
      
      // KF equation: H * P
      float32_t temp_hp_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION];
      Hmat_Times_Pmat_CCA_Moveable(h_mat, p_mat, nr_total_msnmts, temp_hp_mat);

      // KF equation: H * P * H' + R
      // First fill s_mat with H*P*H' only
      HPmat_Times_Hmat_Transpose_CCA_Moveable(temp_hp_mat, h_mat, nr_total_msnmts, s_mat);

      // Now add R into s_mat
      s_mat[0][0] += r_mat[0][0];
      s_mat[0][1] += r_mat[0][1];
      s_mat[1][0] = s_mat[0][1];
      s_mat[1][1] += r_mat[1][1];
      for (uint32_t loop_index_k = 2U; loop_index_k < nr_total_msnmts; loop_index_k++)
      {
         s_mat[loop_index_k][loop_index_k] += r_mat[loop_index_k][loop_index_k];
      }

      // Use LDL solver for solving linear equation
      //KF equation: P*H'*inv(S) = (H*P)'*inv(S)
      Matrix_Division(s_mat, temp_hp_mat, nr_total_msnmts, STATE_DIMENSION, k_mat);

      if (obj_ok_to_decay_states)
      {
         for (uint32_t i = 0U; i < nr_total_msnmts; i++)
         {
            // Don't let acceleration be driven by measurements
            k_mat[F360_TRK_FLTR_CCA_STATE_AX][i] = 0.0F;
            k_mat[F360_TRK_FLTR_CCA_STATE_AY][i] = 0.0F;
         }
      }
   }


   /*===========================================================================*\
   * FUNCTION: Error_Cov_Update_CCA_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]              - kalman gain
   * const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]  - covariance of measurement uncertainty
   * const float32_t (&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION]              - observation model matrix
   * const uint32_t nr_total_msnmts                                                      - number of valid measurements
   * float32_t (&p_mat)[STATE_DIMENSION][STATE_DIMENSION]                                - state error covariance matrix, updated in this fuction
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
   * r_mat must have the following structure:
   *    r_mat = [* * 0 0 0 0 ....
   *             * * 0 0 0 0 ....
   *             0 0 * 0 0 0 ....
   *             . . . . . .
   *             . . . . . .
   *             . . . . . .]
   * h_mat must have the following structure
   *    h_mat = [1 0 0 0 0 0
   *             0 0 0 1 0 0
   *             0 * 0 0 * 0 (this row is repeaded for each present range rate measurement)
   *             . . . . . .
   *             . . . . . .
   *             . . . . . .]
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Error_Cov_Update_CCA_Moveable(
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t nr_total_msnmts,
      float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      /*Measurement update
      Use the Joseph form of covariance update to avoid numerical instability
      P_update = (I - KH) * P * (I - KH)' + KRK'
      */

      // KF equation: I - (K * H)
      float32_t IKH[STATE_DIMENSION][STATE_DIMENSION];

      // First fill with -K*H
      Negative_Kmat_Times_Hmat_CCA_Moveable(k_mat, h_mat, nr_total_msnmts, IKH);

      // Then add identity matrix : I - (K * H)
      for (uint32_t loop_index = 0U; loop_index < STATE_DIMENSION; loop_index ++)
      {
         IKH[loop_index][loop_index] += 1.0F;
      }

      // KF equation: temp3_mat = (I - (K * H)) * P
      float32_t temp3_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
      F360_Matmul_6x6_6x6(IKH, p_mat, temp3_mat);

      // KF equation: temp4_mat = ((I - (K * H)) * P) * (I - K * H)'
      float32_t temp4_mat[STATE_DIMENSION][STATE_DIMENSION];
      F360_Matmul_6x6_6x6_Transpose_Symmetric(temp3_mat, IKH, temp4_mat);

      // KF equation: temp0_mat = K * R
      float32_t temp0_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT];
      Kmat_Times_Rmat_CCA_Moveable(k_mat, r_mat, nr_total_msnmts, temp0_mat);

      // KF equation: temp5_mat = K * R * K'
      float32_t temp5_mat[STATE_DIMENSION][STATE_DIMENSION];
      F360_Matmul_MxN_MxN_Transpose_Symmetric(temp0_mat, k_mat, temp5_mat, nr_total_msnmts);

      // KF equation: p_mat =  ((I - (K * H)) * P) * (I - K * H)'  +  K * R * K'
      F360_Matadd_6x6_6x6(temp4_mat, temp5_mat, p_mat);
   }


   /*===========================================================================*\
   * FUNCTION: State_Update_CCA_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&z_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT]             - pseudo measurement vector
   * const float32_t (&zhat_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT]          - predicted measurement (previous state)
   * const uint32_t nr_total_msnmts                                 - number of valid measurements
   * float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]  - kalman gain
   * float32_t (&state)[STATE_DIMENSION]                               - object's current state vector
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
   void State_Update_CCA_Moveable(
      const float32_t(&z_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&zhat_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t nr_total_msnmts,
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&state)[STATE_DIMENSION])
   {

      // The Kalman filter equation implemented below is x_updated = x_predicted + K*(z - z_pred)
      for (uint32_t row = 0U; row < STATE_DIMENSION; row++)
      {
         float32_t state_increment = 0.0F;
         for (uint32_t col = 0U; col < nr_total_msnmts; col++)
         {
            state_increment += k_mat[row][col] * (z_mat[col] - zhat_mat[col]);
         }
         state[row] += state_increment;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Negative_Kmat_Times_Hmat_CCA_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the K matrix (Kalman gain) used in the CCA Kalman filter update
   * const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CCA Kalman filter update
   * const uint32_t num_measurements - total number of measurements
   * float32_t(&negKHmat)[STATE_DIMENSION][STATE_DIMENSION] - the resulting matrix multiplcation of -Kmat * Hmat
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
   * This function computes the matrix multiplication of -Kmat * Hmat in the CCA KF measurement update step.
   * It utilizes that Hmat has the following structure in order to optimize the computations with respect
   * to run time:
   * Hmat = [1 0 0 0 0 0;
   *         0 0 0 1 0 0;
   *         0 * 0 0 * 0; (this row is repeated for each available range rate measurement)
   *         ....]
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Negative_Kmat_Times_Hmat_CCA_Moveable(
      const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&negKHmat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      constexpr uint32_t first_rr_meas_idx = 2U;

      for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
      {
         
         // Compute first column of result mat as [KH]_i0 = sum_k k_ik*h_k0 where we utilize that h_00 = 1 and h_k0 = 0 for all k != 0
         negKHmat[row_idx][0] = -Kmat[row_idx][0];

         // Compute fourth column of result mat as [KH]_i3 = sum_k k_ik*h_k3 where we utilize that h_13 = 1 and h_k3 = 0 for all k != 1
         negKHmat[row_idx][3] = -Kmat[row_idx][1];

         // Third and sixth columns are all zeros since h_k2/5 = 0 for all k
         negKHmat[row_idx][2] = 0.0F;
         negKHmat[row_idx][5] = 0.0F;
         
         // Compute remaining columns (i.e. second and fifth) of result mat as [KH]_i1/4 = sum_k k_ik*h_k1/4 where we utilize that h_k1/4 = 0 for all k < 2
         for (uint32_t col_idx = 1U; col_idx < STATE_DIMENSION; col_idx += 3U)
         {
            negKHmat[row_idx][col_idx] = -Kmat[row_idx][first_rr_meas_idx] * Hmat[2][col_idx];
            for (uint32_t k = (first_rr_meas_idx + 1U); k < num_measurements; k++)
            {
               negKHmat[row_idx][col_idx] -= Kmat[row_idx][k] * Hmat[k][col_idx];
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Kmat_Times_Rmat_CCA_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the K matrix (Kalman gain) used in the CCA Kalman filter update
   * const float32_t(&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] - The R matrix (measurement error covariance) used in CCA Kalman filter update
   * const uint32_t num_measurements - total number of measurements
   * float32_t(&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the resulting matrix multiplcation of Kmat * Rmat
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
   * This function computes the matrix multiplication of Kmat * Rmat in the CCA KF measurement update step.
   * It utilizes that Rmat has the following structure in order to optimize the computations with respect
   * to run time:
   * Rmat = [* * 0 0 0 0;
   *         * * 0 0 0 0;
   *         0 0 * 0 0 0; (this row, where the * value is located at the matrix diagonal, is repeated for each available range rate measurement)
   *         ....]
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Kmat_Times_Rmat_CCA_Moveable(
      const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t num_measurements,
      float32_t(&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT])
   {
      // Compute first two columns of result mat as [KR]_i0/1 = sum_l k_il*r_l0/1 where we utilize that r_l0/1 = 0 for all l > 1
      for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
      {
         for (uint32_t col_idx = 0U; col_idx < 2U; col_idx++)
         {
            KRmat[row_idx][col_idx] = Kmat[row_idx][0] * Rmat[0][col_idx] + Kmat[row_idx][1] * Rmat[1][col_idx];
         }
      }

      // Compute remaining columns of result mat as [KR]_ij = sum_l k_il*r_lj where we utilize that r_lj = 0 for all l != j 
      for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++) // We can start on first_rr_idx instead of on 0 since HPHT is symmetric
      {
         for (uint32_t col_idx = 2U; col_idx < num_measurements; col_idx++)
         {
            KRmat[row_idx][col_idx] = Kmat[row_idx][col_idx] * Rmat[col_idx][col_idx];
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Hmat_Times_Pmat_CCA_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CCA Kalman filter update.
   * const float32_t(&Pmat)[STATE_DIMENSION][STATE_DIMENSION] - the P matrix (state error covariance) in the CCA Kalman filter update
   * const uint32_t num_measurements - total number of measurements
   * float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - the resulting matrix multiplcation of Hmat * Pmat
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
   * This function computes the matrix multiplication of Hmat * Pmat in the CCA KF measurement update step.
   * It utilizes that Hmat has the following structure in order to optimize the computations with respect
   * to run time:
   * Hmat = [1 0 0 0 0 0;
   *         0 0 0 1 0 0;
   *         0 * 0 0 * 0; (this row is repeated for each available range rate measurement)
   *         ....]
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Hmat_Times_Pmat_CCA_Moveable(
       const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
       const float32_t(&Pmat)[STATE_DIMENSION][STATE_DIMENSION],
       const uint32_t num_measurements,
       float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION])
   {
       // Compute first row of result mat as [HP]_0j = sum_k h_0k*p_kj where we utilize that h_00 = 1 and h_0k = 0 for all k != 0
       for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
       {
           HPmat[0][col_idx] = Pmat[0][col_idx];
       }

       // Compute second row of result mat as [HP]_1j = sum_k h_1k*p_kj where we utilize that h_13 = 1 and h_1k = 0 for all k != 3
       for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
       {
           HPmat[1][col_idx] = Pmat[3][col_idx];
       }

       // Compute remaining rows of result mat as [HP]_ij = sum_k h_ik*p_kj where we utilize that h_i0 = 0 and h_i2 = 0 and h_i3 = 0 and h_i5 = 0 for all i >= 2
       for (uint32_t row_idx = 2U; row_idx < num_measurements; row_idx++)
       {
           for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
           {
               HPmat[row_idx][col_idx] = Hmat[row_idx][1] * Pmat[1][col_idx] + Hmat[row_idx][4] * Pmat[4][col_idx];
           }
       }
   }
   
   /*===========================================================================*\
   * FUNCTION: HPmat_Times_Hmat_Transpose_CCA_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - a matrix corresponding to H * P (linearized measurement model * state error covariance) used in the CCA Kalman filter update
   * const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CCA Kalman filter update
   * const uint32_t num_measurements - total number of measurements
   * float32_t(&HPHTmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the resulting matrix multiplcation of HPmat* Hmat'
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
   * This function computes the matrix multiplication of HPmat * Hmat' in the CCA KF measurement update step.
   * It utilizes that Hmat has the following structure in order to optimize the computations with respect
   * to run time:
   * Hmat = [1 0 0 0 0 0;
   *         0 0 0 1 0 0;
   *         0 * 0 0 * 0; (this row is repeated for each available range rate measurement)
   *         ....]
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void HPmat_Times_Hmat_Transpose_CCA_Moveable(
       const float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
       const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
       const uint32_t num_measurements,
       float32_t(&HPHTmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT])
   {
       // Compute first column of result mat as [HPHT]_i0 = sum_k hp_ik*hT_k0 = sum_k hp_ik*h_0k where we utilize that h_00 = 1 and h_0k = 0 for all k != 0
       for (uint32_t row_idx = 0U; row_idx < num_measurements; row_idx++)
       {
           HPHTmat[row_idx][0] = HPmat[row_idx][0];
           HPHTmat[0][row_idx] = HPHTmat[row_idx][0]; // HPHT is symmetric
       }

       // Compute second column of result mat as [HPHT]_i1 = sum_k hp_ik*hT_k1 = sum_k hp_ik*h_1k where we utilize that h_13 = 1 and h_1k = 0 for all k != 2
       for (uint32_t row_idx = 1U; row_idx < num_measurements; row_idx++) // We can start on 1 instead of on 0 since HPHT is symmetric
       {
           HPHTmat[row_idx][1] = HPmat[row_idx][3];
           HPHTmat[1][row_idx] = HPHTmat[row_idx][1];
       }

       // Compute remaining columns of result mat as [HPHT]_ij = sum_k hp_ik*hT_kj = sum_k hp_ik*h_jk where we utilize that h_i0 = 0 and h_i2 = 0 and h_i3 = 0 and h_i5 = 0 for all i => 2 
       for (uint32_t row_idx = 2U; row_idx < num_measurements; row_idx++) // We can start on first_rr_idx instead of on 0 since HPHT is symmetric
       {
           for (uint32_t col_idx = 2U; col_idx < num_measurements; col_idx++)
           {
               if (row_idx <= col_idx)
               {
                   // Compute value
                   HPHTmat[row_idx][col_idx] = HPmat[row_idx][1] * Hmat[col_idx][1] + HPmat[row_idx][4] * Hmat[col_idx][4];
               }
               else
               {
                   // Copy value to utilize that HPHT is symmetric
                   HPHTmat[row_idx][col_idx] = HPHTmat[col_idx][row_idx];
               }
           }
       }
   }


  /*===========================================================================*\
  * FUNCTION: Measurement_Update_Pointing_Heading_Rate_CCA()
  *===========================================================================
  * RETURN VALUE:
  * None
  *
  * PARAMETERS:
  * const F360_Calibrations_T& calibs,
  * F360_Object_Track_T& obj
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
  * This function computes a KF measurmennt update on the object states pointing
  * and heading rate for a CCA object. The heading is computed as a weighted average 
  * between direction of object velocity vector and object pseudo heading. The velocity
  * vector is trusted if there is no sufficient detection trail. Otherwise, the
  * pseudo heading is being used.
  * 
  * The intention with this KF filter for pointing and heading rate is to prevent 
  * the bounding box of slow moving CCA objects to rotate too fast around its own
  * axis while still enabling to let object velocity vector change unrestrictedly.
  *
  *
  * PRECONDITIONS:
  *
  * POSTCONDITIONS:
  * None
  *
  \*===========================================================================*/
   void Measurement_Update_Pointing_Heading_Rate_CCA(
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj)
   {
      // Extract object information
      const Angle prev_pointing = obj.bbox.Get_Orientation();
      const float32_t prev_heading_rate = obj.heading_rate;
      const float32_t prev_p[2][2] = { {obj.cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][1]}, {obj.cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][1]} };
      const float32_t abs_obj_speed = std::abs(obj.speed);

      /* Compute measurement(object heading) as a weighted average between direction of object velocity vector and
       * object pseudo heading(based on position trail of associated detections). Right when an objects becomes moving
       * we trust the velocity vector (we don't have a sufficient detection trail for pseudo heading estimate) but after
       * some time we transision to use only pseudo heading */
      const float32_t meas_pseudo_hdg_based = obj.pseudo_hdg;
      const Angle measurement_vel_based = obj.vcs_heading.Normalize(meas_pseudo_hdg_based);

      const float32_t init_time_threshold = 1.2F;
      const float32_t high_speed_threshold = obj.time_since_initialization > init_time_threshold ? 7.5F : 15.0F;
      const float32_t low_speed_threshold = obj.time_since_initialization > init_time_threshold ? 5.0F : 12.0F;
      const float32_t max_pseudo_hdg_weight = F360_Linear_Equation_With_Saturation(obj.speed, low_speed_threshold, high_speed_threshold, 1.0F, 0.0F);


      const float32_t pseudo_pos_weight = F360_Linear_Equation_With_Saturation(obj.time_since_started_move, 0.0F, 1.0F, 0.0F, max_pseudo_hdg_weight);
      const Angle measurement((1.0F - pseudo_pos_weight) * measurement_vel_based.Value() + pseudo_pos_weight * meas_pseudo_hdg_based);

      /* Variance of heading measurement.We are more uncertain in slower speeds(i.e.e make filter slower in slower speeds).
       * Also right after initialization the velcoty vector direction can be quite incorrect and the bad measurements are
       * correlated over time which means we decrease the OP matrix too fast. Use the r_init implementatuion here bwlow to
       * prevent this from happening. */
      float32_t r_normal = 0.0F;
      if (abs_obj_speed > calibs.k_speed_th_for_saturating_r)
      {
         r_normal = calibs.k_cca_msmnt_update_vel_var / (obj.speed * obj.speed);
      }
      else
      {
         r_normal = calibs.k_cca_msmnt_update_vel_var / (calibs.k_speed_th_for_saturating_r * calibs.k_speed_th_for_saturating_r);
      }
      const float32_t r_init = F360_Linear_Equation_With_Saturation(static_cast<float32_t>(obj.num_updates_since_init), -1.0F, 14.0F, calibs.init_cca_pnt_filter_cov[0][0], 0.0F);
      const float32_t r = r_normal + r_init;

      // To calculate heading pointing disagreement, use the distance from object center to rear unless reference point is on the rear edge.
      float32_t obj_len_fraction = 0.5F;
      const F360_Object_Sides_T front_or_rear_visible = Get_Reference_Point_Para_Side(obj.reference_point);
      if (front_or_rear_visible == F360_OBJECT_SIDES_REAR)
      {
         obj_len_fraction = 0.0F;
      }

      /* Predicted measurement. The KF measurement update measurement model used are :
       *     meas = vcs_heading = pointing - hdg_pnt_disagreement = pnt + Atan2(curv*dist_to_rear, 1.0F) = pnt + Atan2(yaw_rate/speed*dist_to_rear_axis, 1.0F)
       * (where pnt and yaw_rate are the KF states) */
      const float32_t dist_to_rear = obj.bbox.Get_Length() * obj_len_fraction;
      
      float32_t pred_hdg_pnt_disag = -F360_Atan2f(obj.curvature * dist_to_rear, 1.0F);
      const float32_t diff_pred_and_actual_hdg_pnt_disag = std::abs(Normalize_Heading_Angle(pred_hdg_pnt_disag - obj.hdg_ptng_disagmt, 0.0F));
      // If  the heading pointing disagreement innovation is too large ( > 5 degrees), either the measurement or the model is likely wrong.
      // Thus, dont use pred_hdg_pnt_disag in the calculations
      if (diff_pred_and_actual_hdg_pnt_disag > F360_DEG2RAD(5.0F))
      {
         pred_hdg_pnt_disag = 0.0F;
      }
      
      Angle pred_meas = prev_pointing - pred_hdg_pnt_disag;
      (void) pred_meas.Normalize();

      // Compute KF measurement innovation
      Angle measurement_innovation_angle = (measurement - pred_meas).Normalize();
      if (std::abs(measurement_innovation_angle.Value()) > F360_PI_2) // Object is reversing if heading and pointing are in different directions. Then we need to adjust the measurement
      {
         (void) measurement_innovation_angle.Value(measurement_innovation_angle.Value() + F360_PI).Normalize();
      }
      const float32_t measurement_innovation = measurement_innovation_angle.Value();

      // Msmt update state
      const float32_t temp_var = dist_to_rear * obj.speed / (obj.speed * obj.speed + obj.heading_rate * obj.heading_rate * dist_to_rear * dist_to_rear); // Derivative of h w.r.t. heading_rate
      const float32_t s_inv = 1.0F / (prev_p[0][0] + 2.0F * temp_var * prev_p[0][1] + temp_var * temp_var * prev_p[1][1] + r); // s = H*P*H' + R where H = [1, dist_to_rear*speed/(speed^2 + heading_rate^2 * dist_to_rear^2)] = [1, temp_var]
      const float32_t k[2][1] = { { (prev_p[0][0] + temp_var * prev_p[0][1]) * s_inv }, { (prev_p[0][1] + temp_var * prev_p[1][1]) * s_inv} }; // K = P*H'*inv(S)

      Angle new_pointing = prev_pointing + k[0][0] * measurement_innovation;
      const float32_t new_heading_rate = prev_heading_rate + k[1][0] * measurement_innovation;

      const Angle diff_angle = (obj.vcs_heading - new_pointing).Normalize();
      if ((std::abs(obj.speed) > calibs.k_ctca_msmnt_update_max_reverse_abs_spd) && (std::abs(diff_angle.Value()) > F360_PI_2))
      {
         // Heading and pointing are in different directions and speed is large (i.s. object is reversing fast)
         // Rotate pointing 180 degrees to be more in the direction of the velocity vector
         (void)new_pointing.Value(new_pointing.Value() + F360_PI).Normalize();
         
         obj.reference_point = Flip_Reference_Point(obj.reference_point);
         obj.min_projection_reference_point = Flip_Reference_Point(obj.min_projection_reference_point);
      }

      obj.bbox.Set_Orientation(new_pointing);
      obj.Update_Bbox_Center();
      obj.hdg_ptng_disagmt = (new_pointing - obj.vcs_heading).Normalize().Value();
      obj.heading_rate = Saturate_Heading_Rate(new_heading_rate, abs_obj_speed);

      // Msmt update P:
      //   Use the Joseph form of covariance update to avoid numerical instability (necessary since we are not always using the optimal Kalman gain but are modifying the gain to make filter faster for young objects)
      //   P_new = (I - KH) * P_old * (I - KH)' + KRK' where H = [1, dist_to_rear*speed/(speed^2 + heading_rate^2 * dist_to_rear^2)] = [1, temp_var]

      // KH
      const float32_t h[1][2] = {{ 1.0F, temp_var }};
      float32_t kh[2][2];
      F360_Matmul_MxN_NxP(k, h, kh, 2U, 1U, 2U);

      // I-KH
      const float32_t eye_minus_kh[2][2] = { {1.0F - kh[0][0], -kh[0][1]}, {-kh[1][0], 1.0F - kh[1][1]} };
      
      // (I-KH)*P
      float32_t eye_minus_kh_times_p[2][2];
      F360_Matmul_MxN_NxP(eye_minus_kh, prev_p, eye_minus_kh_times_p, 2U, 2U, 2U);

      // // (I-KH)*P(I-KH)'
      float32_t eye_minus_kh_times_p_times_eye_minus_kh_transpose[2][2];
      F360_Matmul_MxN_PxN_Transpose(eye_minus_kh_times_p, eye_minus_kh, eye_minus_kh_times_p_times_eye_minus_kh_transpose, 2U, 2U, 2U);

      // K*R*K'
      const float32_t krk[2][2] = { {k[0][0]* k[0][0] * r,  k[0][0] * k[1][0] * r}, {k[0][0] * k[1][0] * r, k[1][0] * k[1][0] * r} };

      obj.cca_pnt_filter_cov[0][0] = std::max(eye_minus_kh_times_p_times_eye_minus_kh_transpose[0][0] + krk[0][0], r_init);
      obj.cca_pnt_filter_cov[0][1] = eye_minus_kh_times_p_times_eye_minus_kh_transpose[0][1] + krk[0][1];
      obj.cca_pnt_filter_cov[1][0] = obj.cca_pnt_filter_cov[0][1];
      obj.cca_pnt_filter_cov[1][1] = eye_minus_kh_times_p_times_eye_minus_kh_transpose[1][1] + krk[1][1];
   }


   /*===========================================================================*\
   * FUNCTION: Saturate_Heading_Rate()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t heading_rate,
   * const float32_t abs_obj_speed,
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
   * This function saturates an object heading rate value based on the objects speed.
   * 
   * For large speeds the assumption is that the maximum heading rate are restricted based
   * on maximum lateral acceleration (which is assumed to be constant for all speeds)
   * according to:
   *       heading_rate = lateral acceleration / speed
   * 
   * For small speeds the assumption is that the maximum heading rate is restricted by the
   * minimum turn radius according to:
   *       heading_rate = speed / turn radius
   * 
   * For medium speeds the maxium heading rate is saturated by a constant value.
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Saturate_Heading_Rate(
      const float32_t heading_rate,
      const float32_t abs_obj_speed)
   {
      float32_t saturated_heading_rate = 0.0F;
      
      const float32_t k_cca_heading_rate_high_speed_breakpoint = 7.0F;
      const float32_t k_cca_heading_rate_low_speed_breakpoint = 4.0F;
      const float32_t k_cca_maximum_heading_rate = 1.0F;

      if (abs_obj_speed > k_cca_heading_rate_high_speed_breakpoint)
      {
         // Maximum possible heading rate is limited by maximum lateral acceleration according to acc_lat = speed * heading_rate
         const float32_t max_non_tangential_acc = k_cca_maximum_heading_rate * k_cca_heading_rate_high_speed_breakpoint;
         const float32_t max_heading_rate_for_speed = max_non_tangential_acc / abs_obj_speed;
         saturated_heading_rate = F360_Saturate(heading_rate, -max_heading_rate_for_speed, max_heading_rate_for_speed);
      }
      else if (abs_obj_speed < k_cca_heading_rate_low_speed_breakpoint)
      {
         // Maximum possible heading rate is limited by minimum turn radius according to speed = heading_rate * turn_radius
         const float32_t min_turn_radius = k_cca_heading_rate_low_speed_breakpoint / k_cca_maximum_heading_rate;
         const float32_t max_heading_rate_for_speed = abs_obj_speed / min_turn_radius;
         saturated_heading_rate = F360_Saturate(heading_rate, -max_heading_rate_for_speed, max_heading_rate_for_speed);
      }
      else
      {
         // In this speed interval an object can achieve maximum possible heading rate
         saturated_heading_rate = F360_Saturate(heading_rate, -k_cca_maximum_heading_rate, k_cca_maximum_heading_rate);
      }

      return saturated_heading_rate;
   }


   /*===========================================================================*\
   * FUNCTION: Flip_Reference_Point()
   *===========================================================================
   * RETURN VALUE:
   * F360_Reference_Point_T new_ref_point
   *
   * PARAMETERS:
   * const F360_Reference_Point_T ref_point
   *
   * EXTERNAL REFERENCES:
   * None
   *
   * DEVIATIONS FROM STANDARDS:
   * None
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *--------------------------------------------------------------------------
   * Function returns a new reference point, when object is reversing
   *
   * PRECONDITIONS :
   * None
   *
   * POSTCONDITIONS :
   * None
   *
   \*===========================================================================*/

   static F360_Reference_Point_T Flip_Reference_Point(
      const F360_Reference_Point_T old_ref_point)
   {
      F360_Reference_Point_T new_ref_point;
      switch (old_ref_point)
      {
      case F360_REFERENCE_POINT_FRONT_LEFT:
      {
         new_ref_point = F360_REFERENCE_POINT_REAR_RIGHT;
         break;
      }
      case F360_REFERENCE_POINT_FRONT:
      {
         new_ref_point = F360_REFERENCE_POINT_REAR;
         break;
      }
      case F360_REFERENCE_POINT_FRONT_RIGHT:
      {
         new_ref_point = F360_REFERENCE_POINT_REAR_LEFT;
         break;
      }
      case F360_REFERENCE_POINT_RIGHT:
      {
         new_ref_point = F360_REFERENCE_POINT_LEFT;
         break;
      }
      case F360_REFERENCE_POINT_REAR_RIGHT:
      {
         new_ref_point = F360_REFERENCE_POINT_FRONT_LEFT;
         break;
      }
      case F360_REFERENCE_POINT_REAR:
      {
         new_ref_point = F360_REFERENCE_POINT_FRONT;
         break;
      }
      case F360_REFERENCE_POINT_REAR_LEFT:
      {
         new_ref_point = F360_REFERENCE_POINT_FRONT_RIGHT;
         break;
      }
      case F360_REFERENCE_POINT_LEFT:
      {
         new_ref_point = F360_REFERENCE_POINT_RIGHT;
         break;
      }
      default:
      {
         new_ref_point = F360_REFERENCE_POINT_CENTER;
         break;
      }
      }
      return new_ref_point;
   }

   /*===========================================================================*\
   * FUNCTION: Decrease_Kmat_For_Occluded_CCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib,
   * const F360_Object_Track_T& object_track,
   * float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function decreases k mat corresponding to gain in velocity y and acceleration y
   * from change in pseudo position y if object is:
   *  - occluded
   *  - has high speed
   *  - is oncoming
   *
   \*===========================================================================*/
      void Decrease_Kmat_For_Occluded_CCA(
         const F360_Calibrations_T& calib,
         const F360_Object_Track_T& object_track,
         float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]
      )
      {
         if ((object_track.speed > calib.k_speed_threshold_for_kmat_decrease) &&
            (object_track.vcs_position.x > calib.k_x_pos_threshold_for_kmat_decrease) &&
            (std::abs((std::abs(object_track.vcs_heading.Value()) - F360_PI)) < calib.k_heading_threshold_for_kmat_decrease) &&
            ((object_track.occlusion_status == OCCLUSION_STATUS_OCCLUDED) || (object_track.occlusion_status == OCCLUSION_STATUS_ON_EDGE)) &&
            (object_track.behind_sep_id == F360_INVALID_UNSIGNED_ID)
            )
         {
            k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y] *= calib.k_kmat_decrease_factor_for_occluded;
            k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y] *= calib.k_kmat_decrease_factor_for_occluded;
         }
      }
}
