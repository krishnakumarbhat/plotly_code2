/*===================================================================================*\
* FILE: f360_msmt_update_obj_trks_ctca.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Msmt_Update_ObjTrks_CTCA() function implementation
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_msmt_update_obj_trks_ctca.h"
#include "f360_msmt_update_support_functions_ctca.h"
#include "f360_norm_heading_angle.h"
#include "f360_get_wall_time.h"
#include "f360_regularize_trk_hdg_spd.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_handle_spd_and_acc_when_stopping.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Msmt_Update_ObjTrks_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS] - detection properties array
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Calibrations_T & calib - calibration structure
   * const float32_t elapsed_time_s - tracker elapsed time in seconds
   * const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK]  - array of selected detections indexes
   * const uint32_t selected_dets_num - number of selected detections
   * uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   * F360_Object_Track_T & object_track - single object track
   * F360_TRKR_TIMING_INFO_T & timing_info - timing structure
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
   * This function does measurement of object tracks for CTCA.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Msmt_Update_ObjTrks_CTCA(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Calibrations_T& calib,
      const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t selected_dets_num,
      const uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      F360_Object_Track_T& object_track,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      // This track needs a measurement update
      if (object_track.ndets > 0U)
      {
         const float32_t x_posn = object_track.vcs_position.x;
         const float32_t y_posn = object_track.vcs_position.y;

         const float32_t hdg = object_track.vcs_heading.Value();
         const float32_t spd = object_track.speed;

         const float32_t curv = object_track.curvature;
         const float32_t accel = object_track.tang_accel;

         float32_t state[STATE_DIMENSION];
         state[F360_TRK_FLTR_CTCA_STATE_X] = x_posn;
         state[F360_TRK_FLTR_CTCA_STATE_Y] = y_posn;
         state[F360_TRK_FLTR_CTCA_STATE_H] = hdg;
         state[F360_TRK_FLTR_CTCA_STATE_C] = curv;
         state[F360_TRK_FLTR_CTCA_STATE_S] = spd;
         state[F360_TRK_FLTR_CTCA_STATE_A] = accel;

         constexpr uint32_t non_rr_pseudo_msmts_num = static_cast<uint32_t>(MSMT_UPDATE_MAX_NUM_OF_NON_RR_MSMT);
         const uint32_t total_pseudo_msmts_num = selected_dets_num + non_rr_pseudo_msmts_num;

         // Linearized measurement matrix
         float32_t h_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};

         h_mat[0][F360_TRK_FLTR_CTCA_STATE_X] = 1.0F;
         h_mat[1][F360_TRK_FLTR_CTCA_STATE_Y] = 1.0F;

         // Measurements consisting of pseudo measurements (position, heading) and compensated range rates of all down-selected detections)
         float32_t z_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT];

         z_mat[0] = object_track.pseudo_vcs_position.x;
         z_mat[1] = object_track.pseudo_vcs_position.y;

         //Predicted measurement
         float32_t zhat_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT];

         zhat_mat[0] = x_posn;
         zhat_mat[1] = y_posn;

         // Fill rows of h_mat, z_mat, zhat_mat related to range rates of selected detections
         for (uint32_t loop_idx = 0U; loop_idx < selected_dets_num; loop_idx++)
         {
            Fill_H_Z_Mat_Single_Row_With_RR_Info_CTCA(object_track, det_props[selected_dets_idx[loop_idx]],
               raw_detection_list.detections[selected_dets_idx[loop_idx]], non_rr_pseudo_msmts_num + loop_idx, h_mat, z_mat, zhat_mat);
         }

         // Measurement covariance matrix R, has to be intitialized with zeroes
         float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
         Filling_Msmt_Cov_By_H_RR_CTCA(calib, selected_dets_num, selected_dets_score, r_mat);

         Filling_Msmt_Cov_By_Pos_CTCA(object_track, r_mat);

         float32_t k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT];
         float32_t s_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT];
         Kalman_Gain_Update_CTCA(h_mat, object_track.errcov, r_mat, total_pseudo_msmts_num, k_mat, s_mat);

         Decrease_Kmat_For_Occluded_CTCA(calib, object_track, k_mat);

         Error_Cov_Update_CTCA(k_mat, r_mat, h_mat, total_pseudo_msmts_num, object_track.errcov);

         State_Update_CTCA(z_mat, zhat_mat, total_pseudo_msmts_num, k_mat, state);

         const float32_t new_pos_x = state[F360_TRK_FLTR_CTCA_STATE_X];
         const float32_t new_pos_y = state[F360_TRK_FLTR_CTCA_STATE_Y];
         const float32_t delta_pox_x = new_pos_x - object_track.vcs_position.x;
         const float32_t delta_pos_y = new_pos_y - object_track.vcs_position.y;

         object_track.vcs_position.x = new_pos_x;
         object_track.vcs_position.y = new_pos_y;
         object_track.bbox.Translate(delta_pox_x, delta_pos_y);

         if ((object_track.f_moving) ||
            ((object_track.movable_prob > 0.5F) && (std::abs(object_track.speed) > calib.k_min_speed_for_updating_heading)))
         {
            (void)object_track.vcs_heading.Value(state[F360_TRK_FLTR_CTCA_STATE_H]).Normalize();
            object_track.curvature = state[F360_TRK_FLTR_CTCA_STATE_C];
         }
         else
         {
            object_track.curvature = 0.0F;
            Reset_Pmat_state(calib.k_max_allowed_curv_variance, F360_TRK_FLTR_CTCA_STATE_C, object_track.errcov);
         }

         if (object_track.f_changed_direction_after_start)
         {
            constexpr float32_t heading_uncertainty_variance = F360_DEG2RAD(27.0F) * F360_DEG2RAD(27.0F);
            Reset_Pmat_state(heading_uncertainty_variance, F360_TRK_FLTR_CTCA_STATE_H, object_track.errcov);
         }

         const float32_t prev_speed = object_track.speed;
         object_track.speed = state[F360_TRK_FLTR_CTCA_STATE_S];
         object_track.tang_accel = state[F360_TRK_FLTR_CTCA_STATE_A];

         // To prevent object speed and acceleration estimates to overshoot when object comes to a stop after breaking hard
         Handle_Spd_And_Acc_When_Stopping_CTCA(prev_speed, calib, object_track);

         Regularize_Trk_Hdg_Spd(calib, object_track);

         object_track.Set_Bbox_Orientation(object_track.vcs_heading + object_track.hdg_ptng_disagmt); // hdg_ptng_disagmt can be non-zero only for a short period of time after CCA to CTCA transition, after that it should be 0

         // Update states that are indirectly affected by updates done in kalman filter
         object_track.vcs_velocity.longitudinal = object_track.speed * object_track.vcs_heading.Cos();
         object_track.vcs_velocity.lateral = object_track.speed * object_track.vcs_heading.Sin();

         object_track.heading_rate = object_track.curvature * object_track.speed;
         const float32_t non_tangential_accel = object_track.heading_rate * object_track.speed;
         object_track.vcs_accel.longitudinal = (object_track.tang_accel * object_track.vcs_heading.Cos()) - (non_tangential_accel * object_track.vcs_heading.Sin());
         object_track.vcs_accel.lateral = (object_track.tang_accel * object_track.vcs_heading.Sin()) + (non_tangential_accel * object_track.vcs_heading.Cos());
      }

      timing_info.msmt_update_obj_trks_ctca += get_wall_time() - start_time;
   }
}
