/*===================================================================================*\
* FILE: f360_msmt_update_obj_trks_cca_non_moveable.cpp
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Msmt_Update_ObjTrks_CCA_Non_Moveable() function implementation
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_msmt_update_obj_trks_cca_non_moveable.h"
#include "f360_msmt_update_support_functions_cca_non_moveable.h"
#include "f360_norm_heading_angle.h"
#include "f360_get_wall_time.h"
#include "f360_pseudo_msmt.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_math.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
  /*===========================================================================*\
   * FUNCTION: Msmt_Update_Obj_Trks_CCA_Non_Moveable()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T & host - host property structure
   * const F360_Globals_T & global - global values structure
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS] - detection properties array
   * const F360_Calibrations_T & calib - calibration structure
   * F360_Object_Track_T & object_track - single object track
   * F360_TRKR_TIMING_INFO_T & timing_info - timing structure
   * uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK] - array of selected detections indexes
   * uint32_t selected_dets_num - number of selected detections
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
   * This function does KF measurement update of a CCA object.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Msmt_Update_Obj_Trks_CCA_Non_Moveable(
      const F360_Host_T& host,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Calibrations_T & calib,
      const uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t selected_dets_num,
      F360_Object_Track_T& object_track,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      if (selected_dets_num > 0U)
      {
         const float32_t x_posn = object_track.vcs_position.x;
         const float32_t y_posn = object_track.vcs_position.y;
         const float32_t x_vel = object_track.vcs_velocity.longitudinal;
         const float32_t y_vel = object_track.vcs_velocity.lateral;
         const float32_t x_accel = object_track.vcs_accel.longitudinal;
         const float32_t y_accel = object_track.vcs_accel.lateral;

         // Initialize state vector
         float32_t state[STATE_DIMENSION];
         state[F360_TRK_FLTR_CCA_STATE_X] = x_posn;
         state[F360_TRK_FLTR_CCA_STATE_VX] = x_vel;
         state[F360_TRK_FLTR_CCA_STATE_AX] = x_accel;
         state[F360_TRK_FLTR_CCA_STATE_Y] = y_posn;
         state[F360_TRK_FLTR_CCA_STATE_VY] = y_vel;
         state[F360_TRK_FLTR_CCA_STATE_AY] = y_accel;

         float32_t z_mat[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE];
         float32_t zhat_mat[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE];
         float32_t h_mat[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION] = {};
         float32_t r_mat[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE] = {};

         // Position measurement
         z_mat[0] = object_track.pseudo_vcs_position.x;
         z_mat[1] = object_track.pseudo_vcs_position.y;

         zhat_mat[0] = x_posn;
         zhat_mat[1] = y_posn;

         h_mat[0][F360_TRK_FLTR_CCA_STATE_X] = 1.0F;
         h_mat[1][F360_TRK_FLTR_CCA_STATE_Y] = 1.0F;

         r_mat[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X];
         r_mat[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y] = object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y];
         r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X] = object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X];
         r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y];

         // Fill h (linearized measurement model), z (measurements), z_hat (prediced measurements) and r (measurement covariance) matrices with velocity information
         float32_t mean_rr_comp = 0.0F;
         float32_t mean_cos_az = 0.0F;
         float32_t mean_sin_az = 0.0F;
         Generate_Single_Pseudo_Range_Rate_Compensated_Measurement_CCA(det_props, raw_detection_list, selected_dets_idx, selected_dets_num, mean_rr_comp, mean_cos_az, mean_sin_az);

         h_mat[2][F360_TRK_FLTR_CCA_STATE_VX] = mean_cos_az;
         h_mat[2][F360_TRK_FLTR_CCA_STATE_VY] = mean_sin_az;

         z_mat[2] = mean_rr_comp;

         zhat_mat[2] = (x_vel * mean_cos_az) + (y_vel * mean_sin_az);

         r_mat[2][2] = calib.k_ref_msmt_cov_cca;

         // Kalman gain matrix K, and innovation covariance matrix S
         float32_t k_mat[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE];
         float32_t s_mat[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE];
         Kalman_Gain_Update_CCA_Non_Moveable(h_mat, object_track.errcov, r_mat, k_mat, s_mat);
         const bool is_crossing_vru = Check_For_Crossing_VRU_Type_Obj(host.curvature_rear, calib, object_track);
         const bool is_suspected_moveable_CCA = Is_Obj_Suspected_Moveable_CCA(det_props, selected_dets_idx, selected_dets_num);
         const bool force_pseudo_pos_vel_limit = Force_Pseudo_Pos_Vel_Lim_Impact_Logic(host.speed, object_track, det_props, selected_dets_idx, selected_dets_num);
         if ((!is_suspected_moveable_CCA) &&
            ((!is_crossing_vru) || (force_pseudo_pos_vel_limit)))
         {
            Limit_Pseudo_Pos_Meas_Impact_On_Obj_Vel_Estim_CCA(object_track, host.yaw_rate_rad, calib, k_mat);
         }

         Error_Cov_Update_CCA_Non_Moveable(k_mat, r_mat, h_mat, object_track.errcov);

         State_Update_CCA_Non_Moveable(z_mat, zhat_mat, k_mat, state);

         const float32_t delta_pox_x = state[F360_TRK_FLTR_CCA_STATE_X] - object_track.vcs_position.x;
         const float32_t delta_pos_y = state[F360_TRK_FLTR_CCA_STATE_Y] - object_track.vcs_position.y;
         object_track.vcs_position.x = state[F360_TRK_FLTR_CCA_STATE_X];
         object_track.vcs_position.y = state[F360_TRK_FLTR_CCA_STATE_Y];
         object_track.bbox.Translate(delta_pox_x, delta_pos_y);

         object_track.vcs_velocity.longitudinal = state[F360_TRK_FLTR_CCA_STATE_VX];
         object_track.vcs_accel.longitudinal = state[F360_TRK_FLTR_CCA_STATE_AX];
         object_track.vcs_velocity.lateral = state[F360_TRK_FLTR_CCA_STATE_VY];
         object_track.vcs_accel.lateral = state[F360_TRK_FLTR_CCA_STATE_AY];

         // Compute object speed
         object_track.speed = F360_Get_Hypotenuse(object_track.vcs_velocity.lateral, object_track.vcs_velocity.longitudinal);

         // Compute tangential acceleration for object
         object_track.tang_accel = object_track.vcs_accel.longitudinal * object_track.vcs_heading.Cos() + object_track.vcs_accel.lateral * object_track.vcs_heading.Sin();

         // Update object heading, bbox pointing
         object_track.vcs_heading = Angle{F360_Atan2f(object_track.vcs_velocity.lateral, object_track.vcs_velocity.longitudinal)}.Normalize(); // Heading corresponds to the direction of the velocity vector (which is allowed to rotate even when object stands still)
         object_track.Set_Bbox_Orientation(object_track.vcs_heading);
      }

      const float32_t run_time = get_wall_time() - start_time;
      timing_info.msmt_update_obj_trks_cca += run_time;
      timing_info.msmt_update_obj_trks_cca_non_moveable += run_time;
   }
}
