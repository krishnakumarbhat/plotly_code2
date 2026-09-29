/*===========================================================================*\
* FILE: f360_msmt_update_support_functions_ctca.cpp
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function related to Kalman measurement update step of CTCA objects
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_msmt_update_support_functions_ctca.h"
#include "f360_msmt_update_support_functions_common.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_pseudo_msmt.h"
#include "f360_math_func.h"
#include "f360_LinearSolvers.h"
#include "f360_norm_heading_angle.h"
#include "f360_reference_point_support_functions.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Fill_H_Z_Mat_Single_Row_With_RR_Info_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track
   * const F360_Detection_Props_T & det_prop
   * const rspp_variant_A::RSPP_Detection_T & det
   * const uint32_t row_idx
   * float32_t (&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION]
   * float32_t (&z_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   * float32_t (&zhat_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function fills a row of the H matrix, the Z and Z_hat vectors with range rate information.
   * The H matrix the measurement model linearized at the current state.
   * The Z vector is the measurement vector and the Z_hat vector is the expected measurement vector.
   * Z_hat is derived using the non-linearized measurement model h(x).
   * (Z - Z_hat) is the innovation, later used in the kalman filter.
   *
   \*===========================================================================*/
   void Fill_H_Z_Mat_Single_Row_With_RR_Info_CTCA(
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T& det_prop,
      const rspp_variant_A::RSPP_Detection_T& det,
      const uint32_t row_idx,
      float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      float32_t(&z_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&zhat_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   )
   {
      const float32_t det_cos_vcs_az = det.processed.cos_vcs_az;
      const float32_t det_sin_vcs_az = det.processed.sin_vcs_az;

      const float32_t trk_cos_vcs_head = object_track.vcs_heading.Cos();
      const float32_t trk_sin_vcs_head = object_track.vcs_heading.Sin();
      const float32_t trk_spd = object_track.speed;
      const float32_t trk_curv = object_track.curvature;

      float32_t tcs_vec_from_ref_pnt_to_center_rear[2];
      Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(object_track, tcs_vec_from_ref_pnt_to_center_rear);
      tcs_vec_from_ref_pnt_to_center_rear[0] = -tcs_vec_from_ref_pnt_to_center_rear[0];
      tcs_vec_from_ref_pnt_to_center_rear[1] = -tcs_vec_from_ref_pnt_to_center_rear[1];
      float32_t vcs_x_center_rear = 0.0F;
      float32_t vcs_y_center_rear = 0.0F;
      Convert_TCS_Posn_To_VCS_Posn(tcs_vec_from_ref_pnt_to_center_rear[0], tcs_vec_from_ref_pnt_to_center_rear[1], object_track.vcs_position.x, object_track.vcs_position.y, object_track.bbox.Get_Orientation(), vcs_x_center_rear, vcs_y_center_rear); // Note: It is intended to use object vcs posiiton rather than object bbox center here since the vector tcs_vec_from_ref_pnt_to_center_rear is defined from object reference point
      const float32_t dx = det_prop.vcs_position.x - vcs_x_center_rear;
      const float32_t dy = det_prop.vcs_position.y - vcs_y_center_rear;

      h_mat[row_idx][F360_TRK_FLTR_CTCA_STATE_X] = -det_sin_vcs_az * trk_curv * trk_spd;
      h_mat[row_idx][F360_TRK_FLTR_CTCA_STATE_Y] = det_cos_vcs_az * trk_curv * trk_spd;
      h_mat[row_idx][F360_TRK_FLTR_CTCA_STATE_H] = trk_spd * (trk_cos_vcs_head * (det_sin_vcs_az + trk_curv * (tcs_vec_from_ref_pnt_to_center_rear[0] * det_cos_vcs_az + tcs_vec_from_ref_pnt_to_center_rear[1] * det_sin_vcs_az)) - trk_sin_vcs_head * (det_cos_vcs_az + trk_curv * (tcs_vec_from_ref_pnt_to_center_rear[1] * det_cos_vcs_az - tcs_vec_from_ref_pnt_to_center_rear[0] * det_sin_vcs_az)));
      h_mat[row_idx][F360_TRK_FLTR_CTCA_STATE_C] = trk_spd * (dx * det_sin_vcs_az - dy * det_cos_vcs_az);
      h_mat[row_idx][F360_TRK_FLTR_CTCA_STATE_S] = (trk_cos_vcs_head - trk_curv * dy) * det_cos_vcs_az + (trk_sin_vcs_head + trk_curv * dx) * det_sin_vcs_az;
      h_mat[row_idx][F360_TRK_FLTR_CTCA_STATE_A] = 0.0F;

      z_mat[row_idx] = det_prop.range_rate_compensated;

      // Project object velocity vectors in point of detection (compensated for object yaw rate) onto detection radial direction to get expected measurement.
      zhat_mat[row_idx] = trk_spd * h_mat[row_idx][F360_TRK_FLTR_CTCA_STATE_S];
   }

   /*===========================================================================*\
   * FUNCTION: Filling_Msmt_Cov_By_H_RR_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib - calibration structure
   * const uint32_t selected_dets_num - number of detections that are chosen to be used for the measurement update
   * uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT] - number of detections in each bin, used as a weigth
   * float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] - measurement covariance matrix
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function updates comp range rate measurements covariance matrix for CTCA objects. The value is weigth based on the amount of detections
   * that were in each bin during binning step. It is calculated as:
   * R[i][i] =  calib.k_ref_msmt_cov_ctca * (N_max / N_i), where:
   *  - N_max is maximum number of detections in one bin,
   *  - N_i is number of detections in current bin.
   *
   * PRECONDITIONS:
   * r_mat must be initialized to zero before call to function
   * selected_dets_num must be larger than 0, i.e. there must be available range rate measuremnts to be used in the Kalman filter update
   *
   \*===========================================================================*/
   void Filling_Msmt_Cov_By_H_RR_CTCA(
      const F360_Calibrations_T& calib,
      const uint32_t selected_dets_num,
      const uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT])
   {
      constexpr uint32_t first_rr_comp_idx_in_meas_cov = static_cast<uint32_t>(MSMT_UPDATE_MAX_NUM_OF_NON_RR_MSMT);
      const float32_t max_dets = static_cast<float32_t>(F360_Max_Element(selected_dets_score, selected_dets_num));

      for (uint32_t i = 0U; i < selected_dets_num; i++)
      {
         const uint32_t index = first_rr_comp_idx_in_meas_cov + i;
         // Check against division by 0
         float32_t weigth;
         if (selected_dets_score[i] == 0U)
         {
            weigth = INFTY;
         }
         else
         {
            weigth = max_dets / static_cast<float32_t>(selected_dets_score[i]);
         }
         r_mat[index][index] = calib.k_ref_msmt_cov_ctca * weigth;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Filling_Msmt_Cov_By_Pos_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track - single object track
   * float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] - measurement covariance matrix
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function updates position measurement covariance matrix for CTCA objects.
   *
   \*===========================================================================*/
   void Filling_Msmt_Cov_By_Pos_CTCA(
      const F360_Object_Track_T& object_track,
      float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   )
   {
      r_mat[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X];
      r_mat[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y] = object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y];
      r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X] = object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X];
      r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y];
   }

   /*===========================================================================*\
   * FUNCTION: Kalman_Gain_Update_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION]              - observation model matrix
   * const float32_t (&hT_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]             - transposed observation model matrix
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
   * r_mat must have the following structure:
   *    r_mat = [* * 0 0 0 0 ....
   *             * * 0 0 0 0 ....
   *             0 0 0 * 0 0 ....
   *             . . . . . .
   *             . . . . . .
   *             . . . . . .]
   * h_mat must have the following structure
   *    h_mat = [1 0 0 0 0 0
   *             0 1 0 0 0 0
   *             * * * * * 0 (this row is repeaded for each present range rate measurement)
   *             . . . . . .
   *             . . . . . .
   *             . . . . . .]
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Kalman_Gain_Update_CTCA(
      const float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t nr_total_msnmts,
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&s_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   )
   {

      // KF equation: H * P
      float32_t temp_hp_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION];
      Hmat_Times_Pmat_CTCA(h_mat, p_mat, nr_total_msnmts, temp_hp_mat);

      // KF equation: H * P * H' + R
      // First fill s_mat with H*P*H' only
      HPmat_Times_Hmat_Transpose_CTCA(temp_hp_mat, h_mat, nr_total_msnmts, s_mat);
      // Now add R into s_mat
      s_mat[0][0] += r_mat[0][0];
      s_mat[0][1] += r_mat[0][1];
      s_mat[1][0] = s_mat[0][1];
      s_mat[1][1] += r_mat[1][1];
      for (uint32_t loop_index_k = 2U; loop_index_k < nr_total_msnmts; loop_index_k++)
      {
         s_mat[loop_index_k][loop_index_k] += r_mat[loop_index_k][loop_index_k];
      }

      //KF equation: P*H'*inv(S) = (H*P)'*inv(S)
      Matrix_Division(s_mat, temp_hp_mat, nr_total_msnmts, STATE_DIMENSION, k_mat);
   }

   /*===========================================================================*\
   * FUNCTION: Error_Cov_Update_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]              - kalman gain
   * const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]  - covariance of measurement uncertainty
   * const float32_t (&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION]              - observation model matrix
   * const uint32_t nr_total_msnmts                                                   - number of valid measurements                                           - a boolean indicating if there is a heading measurement present
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
   *
   * PRECONDITIONS:
   * r_mat must have the following structure:
   *    r_mat = [* * 0 0 0 0 ....
   *             * * 0 0 0 0 ....
   *             0 0 * 0 0 0 ....
   *             . . . . . .
   *             . . . . . .
   *             . . . . . .]
   * h_mat must have the following structure
   *    h_mat = [1 0 0 0 0 0
   *             0 1 0 0 0 0
   *             * * * * * 0 (this row is repeaded for each present range rate measurement)
   *             . . . . . .
   *             . . . . . .
   *             . . . . . .]
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Error_Cov_Update_CTCA(
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
      float32_t temp2_mat[STATE_DIMENSION][STATE_DIMENSION];
      // First fill with -K*H
      Negative_Kmat_Times_Hmat_CTCA(k_mat, h_mat, nr_total_msnmts, temp2_mat);
      // Then add identity matrix
      for (uint32_t loop_index = 0U; loop_index < STATE_DIMENSION; loop_index ++)
      {
         temp2_mat[loop_index][loop_index] += 1.0F;
      }

      // KF equation: (I - (K * H)) * P
      float32_t temp3_mat[STATE_DIMENSION][STATE_DIMENSION];
      F360_Matmul_6x6_6x6(temp2_mat, p_mat, temp3_mat);

      // KF equation: ((I - (K * H)) * P) * (I - K * H)'
      float32_t temp4_mat[STATE_DIMENSION][STATE_DIMENSION];
      F360_Matmul_6x6_6x6_Transpose_Symmetric(temp3_mat, temp2_mat, temp4_mat);

      // KF equation: K * R
      float32_t temp0_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT];
      Kmat_Times_Rmat_CTCA(k_mat, r_mat, nr_total_msnmts, temp0_mat);

      // KF equation: K * R * K'
      float32_t temp5_mat[STATE_DIMENSION][STATE_DIMENSION];
      F360_Matmul_MxN_MxN_Transpose_Symmetric(temp0_mat, k_mat, temp5_mat, nr_total_msnmts);

      //  KF equation: ((I - (K * H)) * P) * (I - K * H)'  +  K * R * K'
      F360_Matadd_6x6_6x6(temp4_mat, temp5_mat, p_mat);
   }

   /*===========================================================================*\
   * FUNCTION: State_Update_CTCA()
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
   void State_Update_CTCA(
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
   * FUNCTION: Hmat_Times_Pmat_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CTCA Kalman filter update.
   * const float32_t(&Pmat)[STATE_DIMENSION][STATE_DIMENSION] - the P matrix (state error covariance) in the CTCA Kalman filter update
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
   * This function computes the matrix multiplication of Hmat * Pmat in the CTCA KF measurement update step.
   * It utilizes that Hmat has the following structure in order to optimize the computations with respect
   * to run time:
   * Hmat = [1 0 0 0 0 0;
   *         0 1 0 0 0 0;
   *         * * * * * 0; (this row is repeated for each available range rate measurement)
   *         ....]
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Hmat_Times_Pmat_CTCA(
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&Pmat)[STATE_DIMENSION][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION])
   {
      // Compute first two rows of result mat as [HP]_0j = sum_k h_0k*p_kj where we utilize that h_00 = 1 and h_11 = 1 and h_0k = 0 for all k != 0 and h_1k = 0 for all k != 1
      for (uint32_t row_idx = 0U; row_idx < 2U; row_idx++)
      {
         for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
         {
            HPmat[row_idx][col_idx] = Pmat[row_idx][col_idx];
         }
      }

      constexpr uint32_t first_rr_idx = static_cast<uint32_t>(MSMT_UPDATE_MAX_NUM_OF_NON_RR_MSMT);

      // Compute remaining rows of result mat as [HP]_ij = sum_k h_ik*p_kj where we utilize that h_i5 = 0 for all i => first_rr_idx
      for (uint32_t row_idx = first_rr_idx; row_idx < num_measurements; row_idx++)
      {
         for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
         {
            HPmat[row_idx][col_idx] = Hmat[row_idx][0] * Pmat[0][col_idx] + Hmat[row_idx][1] * Pmat[1][col_idx] + Hmat[row_idx][2] * Pmat[2][col_idx] + Hmat[row_idx][3] * Pmat[3][col_idx] + Hmat[row_idx][4] * Pmat[4][col_idx];
         }
      }
   }


   /*===========================================================================*\
   * FUNCTION: HPmat_Times_Hmat_Transpose_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - a matrix corresponding to H * P (linearized measurement model * state error covariance) used in the CTCA Kalman filter update
   * const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CTCA Kalman filter update
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
   * This function computes the matrix multiplication of HPmat * Hmat' in the CTCA KF measurement update step.
   * It utilizes that Hmat has the following structure in order to optimize the computations with respect
   * to run time:
   * Hmat = [1 0 0 0 0 0;
   *         0 1 0 0 0 0;
   *         * * * * * 0; (this row is repeated for each available range rate measurement)
   *         ....]
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void HPmat_Times_Hmat_Transpose_CTCA(
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

      // Compute second column of result mat as [res_mat]_i1 = sum_k hp_ik*hT_k1 = sum_k hp_ik*h_1k where we utilize that h_11 = 1 and h_1k = 0 for all k != 1

      for (uint32_t row_idx = 1U; row_idx < num_measurements; row_idx++) // We can start on 1 instead of on 0 since HPHT is symmetric
      {
         HPHTmat[row_idx][1] = HPmat[row_idx][1];
         HPHTmat[1][row_idx] = HPHTmat[row_idx][1];
      }

      constexpr uint32_t first_rr_idx = static_cast<uint32_t>(MSMT_UPDATE_MAX_NUM_OF_NON_RR_MSMT);

      // Compute remaining columns of result mat as [HPNT]_ij = sum_k hp_ik*hT_kj = sum_k hp_ik*h_jk where we utilize that h_i5 = 0 for all i => first_rr_idx
      for (uint32_t row_idx = first_rr_idx; row_idx < num_measurements; row_idx++) // We can start on first_rr_idx instead of on 0 since HPHT is symmetric
      {
         for (uint32_t col_idx = first_rr_idx; col_idx < num_measurements; col_idx++)
         {
            if (row_idx <= col_idx)
            {
               // Compute value
               HPHTmat[row_idx][col_idx] = HPmat[row_idx][0] * Hmat[col_idx][0] + HPmat[row_idx][1] * Hmat[col_idx][1] + HPmat[row_idx][2] * Hmat[col_idx][2] + HPmat[row_idx][3] * Hmat[col_idx][3] + HPmat[row_idx][4] * Hmat[col_idx][4];
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
   * FUNCTION: Negative_Kmat_Times_Hmat_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the K matrix (Kalman gain) used in the CTCA Kalman filter update
   * const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] - the H matrix (linearized measurement model) used in the CTCA Kalman filter update
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
   * This function computes the matrix multiplication of -Kmat * Hmat in the CTCA KF measurement update step.
   * It utilizes that Hmat has the following structure in order to optimize the computations with respect
   * to run time:
   * Hmat = [1 0 0 0 0 0;
   *         0 1 0 0 0 0;
   *         * * * * * 0; (this row is repeated for each available range rate measurement)
   *         ....]
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Negative_Kmat_Times_Hmat_CTCA(
      const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&negKHmat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      constexpr uint32_t first_rr_meas_idx = static_cast<uint32_t>(MSMT_UPDATE_MAX_NUM_OF_NON_RR_MSMT);

      // Compute first and second column of result mat as [KH]_i0 = sum_k k_ik*h_k0 where we utilize that h_00 = 1 and h_k0 = 0 for all k != 0 or k <= first_rr_meas_idx and h_11 = 1 and h_k1 = 0 for all k != 1 or k <= first_rr_meas_idx
      for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
      {
         for (uint32_t col_idx = 0U; col_idx < 2U; col_idx++)
         {
            negKHmat[row_idx][col_idx] = -Kmat[row_idx][col_idx];
            for (uint32_t k = first_rr_meas_idx; k < num_measurements; k++)
            {
               negKHmat[row_idx][col_idx] -= Kmat[row_idx][k] * Hmat[k][col_idx];
            }
         }
      }

      // Compute remaining columns of result mat as [KH]_ij = sum_k K_ik*h_kj where we utilize that h_kj = 0 for all k > first_rr_idx and that h_k5 = 0 for all j
      for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
      {
         for (uint32_t col_idx = first_rr_meas_idx; col_idx < (STATE_DIMENSION - 1U); col_idx++)
         {
            negKHmat[row_idx][col_idx] = 0.0F;
            for (uint32_t k = first_rr_meas_idx; k < num_measurements; k++)
            {
               negKHmat[row_idx][col_idx] -= Kmat[row_idx][k] * Hmat[k][col_idx];
            }
         }
         negKHmat[row_idx][5] = 0.0F;
      }
   }


   /*===========================================================================*\
   * FUNCTION: Kmat_Times_Rmat_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the K matrix (Kalman gain) used in the CTCA Kalman filter update
   * const float32_t(&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] - The R matrix (measurement error covariance) used in CTCA Kalman filter update
   * const uint32_t num_measurements - total number of measurements
   * float32_t(&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] - - the resulting matrix multiplcation of Kmat * Rmat
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
   * This function computes the matrix multiplication of Kmat * Rmat in the CTCA KF measurement update step.
   * It utilizes that Rmat has the following structure in order to optimize the computations with respect
   * to run time:
   * Rmat = [* * 0 0 0 0;
   *         * * 0 0 0 0;
   *         0 0 0 * 0 0; (this row, where the * value is located at the matrix diagonal, is repeated for each available range rate measurement)
   *         ....]
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Kmat_Times_Rmat_CTCA(
      const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t num_measurements,
      float32_t(&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT])
   {
      // Compute first two columns of result mat as [KR]_i0/1 = sum_l k_il*hT_l0/1 where we utilize that h_l0/1 = 0 for all l > 1
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
   * FUNCTION: Reset_Pmat_state()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const uint8_t state - state to be reset
   * const float32_t uncertainty_variance - variance of the state
   * float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION] - P matrix
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function resets given state in P matrix. It sets covariance-related elements of Pmatrix to 0 and variance to specified value.
   *
   \*===========================================================================*/
   void Reset_Pmat_state(
       const float32_t uncertainty_variance,
       const uint8_t state,
       float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION]
   )
   {
       for (uint8_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
       {
           if (row_idx == state)
           {
               for (uint8_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
               {
                   if (col_idx == state)
                   {
                       p_mat[row_idx][col_idx] = uncertainty_variance;
                   }
                   else
                   {
                       p_mat[row_idx][col_idx] = 0.0F;
                   }
               }
           }
           else
           {
               p_mat[row_idx][state] = 0.0F;
           }
       }
   }

   /*===========================================================================*\
   * FUNCTION: Decrease_Kmat_For_Occluded_CTCA()
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
   * This function decreases k mat corresponding to gain in heading and curvature
   * from change in pseudo position y if object is:
   *  - occluded
   *  - has high speed
   *  - is oncoming
   *
   \*===========================================================================*/
   void Decrease_Kmat_For_Occluded_CTCA(
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
         k_mat[F360_TRK_FLTR_CTCA_STATE_H][1] *= calib.k_kmat_decrease_factor_for_occluded;
         k_mat[F360_TRK_FLTR_CTCA_STATE_C][1] *= calib.k_kmat_decrease_factor_for_occluded;
      }
   }
}
