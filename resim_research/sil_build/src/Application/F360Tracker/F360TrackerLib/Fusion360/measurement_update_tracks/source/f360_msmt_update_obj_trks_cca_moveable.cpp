/*===================================================================================*\
* FILE: f360_msmt_update_obj_trks_cca_moveable.cpp
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Msmt_Update_ObjTrks_CCA_Moveable() function implementation
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_msmt_update_obj_trks_cca_moveable.h"
#include "f360_msmt_update_support_functions_cca_moveable.h"
#include "f360_norm_heading_angle.h"
#include "f360_get_wall_time.h"
#include "f360_pseudo_msmt.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_handle_spd_and_acc_when_stopping.h"

namespace f360_variant_A
{
   static bool Associated_Detections_Stationary(
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   );

   static bool High_Heading_Pointing_Disagreement_Objs(
      const F360_Object_Track_T& object_track
   );

   static bool Acc_Perpendicular_To_Heading_Objs(
      const F360_Object_Track_T& object_track
   );

   /*===========================================================================*\
    * FUNCTION: Msmt_Update_Obj_Trks_CCA_Moveable()
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

   void Msmt_Update_Obj_Trks_CCA_Moveable(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Calibrations_T& calib,
      const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t selected_dets_num,
      F360_Object_Track_T& object_track,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      if (selected_dets_num > 0U)
      {
         const uint32_t non_rr_pseudo_msmts_num = 2U;
         const uint32_t total_pseudo_msmts_num = selected_dets_num + non_rr_pseudo_msmts_num;

         const float32_t x_posn = object_track.vcs_position.x;
         const float32_t y_posn = object_track.vcs_position.y;
         const float32_t x_vel = object_track.vcs_velocity.longitudinal;
         const float32_t y_vel = object_track.vcs_velocity.lateral;
         const float32_t x_accel = object_track.vcs_accel.longitudinal;
         const float32_t y_accel = object_track.vcs_accel.lateral;

         const bool obj_ok_to_decay_states = Is_Obj_Ok_To_Decay_States(object_track, det_props);
         const float32_t state_decay_factor = obj_ok_to_decay_states ? calib.k_acc_vel_state_decay_factor : 1.0F;

         // Initialize state vector
         float32_t state[STATE_DIMENSION];
         state[F360_TRK_FLTR_CCA_STATE_X] = x_posn;
         state[F360_TRK_FLTR_CCA_STATE_VX] = x_vel;
         state[F360_TRK_FLTR_CCA_STATE_AX] = x_accel;
         state[F360_TRK_FLTR_CCA_STATE_Y] = y_posn;
         state[F360_TRK_FLTR_CCA_STATE_VY] = y_vel;
         state[F360_TRK_FLTR_CCA_STATE_AY] = y_accel;

         float32_t z_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT];
         float32_t zhat_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT];
         float32_t h_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
         float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};

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

         float32_t R_pseudo_pos_y_increase_factor;
         float32_t R_rr_addition_long_range_crossing;
         Update_Measurement_Covariances_For_Long_Range_Crossing_Object( 
            object_track.vcs_velocity.longitudinal,
            object_track.vcs_velocity.lateral,
            object_track.vcs_position.x,
            R_pseudo_pos_y_increase_factor,
            R_rr_addition_long_range_crossing);
         
         // Adjust the pseudopos y meascov according to R_pseudo_pos_y_increase_factor
         r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] *= R_pseudo_pos_y_increase_factor;

         // Range rate measurements
         const float32_t normalized_rr_variance = static_cast<float32_t>(selected_dets_num) * (calib.k_ref_msmt_cov_cca + R_rr_addition_long_range_crossing);
         for (uint32_t loop_index_j = 0U; loop_index_j < selected_dets_num; loop_index_j++)
         {
            const uint32_t index = non_rr_pseudo_msmts_num + loop_index_j;

            const float32_t cos_vcs_az = raw_detection_list.detections[selected_dets_idx[loop_index_j]].processed.cos_vcs_az;
            const float32_t sin_vcs_az = raw_detection_list.detections[selected_dets_idx[loop_index_j]].processed.sin_vcs_az;

            z_mat[index] = det_props[selected_dets_idx[loop_index_j]].range_rate_compensated;

            zhat_mat[index] = (x_vel * cos_vcs_az) + (y_vel * sin_vcs_az);

            h_mat[index][F360_TRK_FLTR_CCA_STATE_VX] = cos_vcs_az;
            h_mat[index][F360_TRK_FLTR_CCA_STATE_VY] = sin_vcs_az;

            r_mat[index][index] = normalized_rr_variance;
         }

         // Kalman gain matrix K, and innovation covariance matrix S
         float32_t k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT];
         float32_t s_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT];
         Kalman_Gain_Update_CCA_Moveable(h_mat, object_track.errcov, r_mat, total_pseudo_msmts_num, obj_ok_to_decay_states, k_mat, s_mat);

         Decrease_Kmat_For_Occluded_CCA(calib, object_track, k_mat);

         Error_Cov_Update_CCA_Moveable(k_mat, r_mat, h_mat, total_pseudo_msmts_num, object_track.errcov);

         State_Update_CCA_Moveable(z_mat, zhat_mat, total_pseudo_msmts_num, k_mat, state);

         const float32_t delta_pox_x = state[F360_TRK_FLTR_CCA_STATE_X] - object_track.vcs_position.x;
         const float32_t delta_pos_y = state[F360_TRK_FLTR_CCA_STATE_Y] - object_track.vcs_position.y;

         object_track.vcs_position.x = obj_ok_to_decay_states ? object_track.pseudo_vcs_position.x : state[F360_TRK_FLTR_CCA_STATE_X];
         object_track.vcs_position.y = obj_ok_to_decay_states ? object_track.pseudo_vcs_position.y : state[F360_TRK_FLTR_CCA_STATE_Y];
         object_track.bbox.Translate(delta_pox_x, delta_pos_y);

         const F360_VCS_Velocity_T prev_vel = object_track.vcs_velocity;

         Set_Acc_Vel_States(state, state_decay_factor, object_track);

         // Compute object speed
         object_track.speed = F360_Get_Hypotenuse(object_track.vcs_velocity.lateral, object_track.vcs_velocity.longitudinal);

         // Compute tangential acceleration for object
         object_track.tang_accel = object_track.vcs_accel.longitudinal * object_track.vcs_heading.Cos() + object_track.vcs_accel.lateral * object_track.vcs_heading.Sin();

         // Handle when object stops from hard break
         Handle_Spd_And_Acc_When_Stopping_CCA(prev_vel, calib, object_track);

         // Update object heading, bbox pointing, heading rate and curvature
         (void)object_track.vcs_heading.Value(F360_Atan2f(object_track.vcs_velocity.lateral, object_track.vcs_velocity.longitudinal)).Normalize(); // Heading corresponds to the direction of the velocity vector (which is allowed to rotate even when object stands still)

         if (object_track.f_moving && (object_track.speed > calib.k_cca_min_speed_to_update_pnt)) // Note: For CCA speed is always positive so we don't need to take abs
         {
            Measurement_Update_Pointing_Heading_Rate_CCA(calib, object_track);

            // Compute object bbox curvature
            object_track.curvature = object_track.heading_rate / object_track.speed;
         }
         else
         {
            // BBox of stationary objects should not rotate Therefore: 
            //    - Freeze object pointing (i.e. don't update)
            //    - Set heading rate and curvature to 0
            object_track.heading_rate = 0.0F;
            object_track.curvature = 0.0F;
         }
      }

      const float32_t run_time = get_wall_time() - start_time;
      timing_info.msmt_update_obj_trks_cca_moveable += run_time;
      timing_info.msmt_update_obj_trks_cca += run_time;
   }

   /*===========================================================================*\
    * FUNCTION: Set_Acc_Vel_States()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const float32_t(&state)[STATE_DIMENSION]
    * const float32_t& state_decay_factor
    * F360_Object_Track_T& object_track
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
    * This function sets velocity and acceleration states to CCA objects.
    * If the object is set as OK to decay states - the lateral acceleration and
    * velocity will be decayed.
    *
    * PRECONDITIONS:
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   void Set_Acc_Vel_States(
      const float32_t(&state)[STATE_DIMENSION],
      const float32_t& state_decay_factor,
      F360_Object_Track_T& object_track
   )
   {
      object_track.vcs_velocity.longitudinal = state[F360_TRK_FLTR_CCA_STATE_VX];
      object_track.vcs_accel.longitudinal = state[F360_TRK_FLTR_CCA_STATE_AX];
      object_track.vcs_velocity.lateral = state[F360_TRK_FLTR_CCA_STATE_VY] * state_decay_factor;
      object_track.vcs_accel.lateral = state[F360_TRK_FLTR_CCA_STATE_AY] * state_decay_factor;
   }

   /*===========================================================================*\
    * FUNCTION: Is_Obj_Ok_To_Decay_States()
    *===========================================================================
    * RETURN VALUE:
    * bool
    *
    * PARAMETERS:
    * F360_Object_Track_T& object_track
    * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function checks if the object is OK to decay states. If CCA moving obj is
    * far away from the host with siginifcant heading-pointing disagreement,
    * it is a young object with significant length, and all associated detections
    * are not moving - it will be flagged.
    *
    * If the acceleration vector is almost perpendicular to the heading direction, 
    * the heading pointing diff is not significant and the object has positive 
    * longitudinal velocity it will be flagged as well.
    * 
    * PRECONDITIONS:
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   bool Is_Obj_Ok_To_Decay_States(
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
      const bool all_dets_stationary = object_track.ndets > 0U ? Associated_Detections_Stationary(object_track, det_props) : false;
      const bool obj_ok_to_decay_states = High_Heading_Pointing_Disagreement_Objs(object_track) ? true : Acc_Perpendicular_To_Heading_Objs(object_track);

      return obj_ok_to_decay_states && all_dets_stationary;
   }

   /*===========================================================================*\
    * FUNCTION: Update_Measurement_Covariances_For_Long_Range_Crossing_Object()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const float32_t& object_longitudinal_velocity
    * const float32_t& object_lateral_velocity
    * const float32_t& object_point_x
    * float32_t&  R_pseudo_pos_y_increase_factor
    * float32_t& R_rr_addition_long_range_crossing
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
    * This functions increases the measurement covariances for the lateral position
    * and range-rate for crossing objects at long range.
    *
    * r_mat_multiplyer is used for multiplying r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y]
    * after the function is called in order to either modify or keep the same the measurement covariances
    *
    * R_rr_addition_long_range_crossing follows the same logic with r_mat_multiplyer and
    * is used for modifying normalized_rr_variance after this function is called
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   void Update_Measurement_Covariances_For_Long_Range_Crossing_Object(
      const float32_t object_longitudinal_velocity,
      const float32_t object_lateral_velocity,
      const float32_t object_point_x,
      float32_t& R_pseudo_pos_y_increase_factor,
      float32_t& R_rr_addition_long_range_crossing)
   {
      constexpr float32_t longitudinal_position_threshold = 60.0F;
      constexpr float32_t lateral_velocity_threshold = 12.0F;
      constexpr float32_t longitudinal_velocity_threshold = 3.0F;

      // If the object is a long range crossing object, so increase the meascov
      if ((object_point_x > longitudinal_position_threshold) &&
         (std::abs(object_lateral_velocity) > lateral_velocity_threshold) &&
         (std::abs(object_longitudinal_velocity) < longitudinal_velocity_threshold))
      {
         R_pseudo_pos_y_increase_factor = 20.0F;
         R_rr_addition_long_range_crossing = 4.0F;
      }
      // If it is not a long range crossing object, the measurement covariances remain unchanged  
      else
      {
         R_pseudo_pos_y_increase_factor = 1.0F;
         R_rr_addition_long_range_crossing = 0.0F;
      }
   }

   static bool Associated_Detections_Stationary(
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
      bool all_dets_stationary = true;
      for (uint32_t i = 0U; i < object_track.ndets; i++)
      {
         const uint32_t det_idx = object_track.detids[i] - 1U;
         if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_props[det_idx].motion_status)
         {
            all_dets_stationary = false;
            break;
         }
      }
      return all_dets_stationary;
   }

   static bool High_Heading_Pointing_Disagreement_Objs(
      const F360_Object_Track_T& object_track
   )
   {
      constexpr float32_t time_init_threshold = 2.0F;
      constexpr float32_t hdg_pntg_diff_threshold = 0.5F;
      constexpr float32_t vcs_pos_x_threshold = 90.0F;

      const bool obj_age_ok_to_decay_states = object_track.time_since_initialization < time_init_threshold;
      const bool obj_hdg_pntg_diff_ok_to_decay_states = std::fabs(object_track.hdg_ptng_disagmt) > hdg_pntg_diff_threshold;
      const bool obj_pos_ok_to_decay_states = object_track.vcs_position.x > vcs_pos_x_threshold;
      bool obj_ok_to_decay_states = false;


      if (obj_age_ok_to_decay_states && 
         obj_hdg_pntg_diff_ok_to_decay_states &&
         obj_pos_ok_to_decay_states &&
         object_track.f_moving && 
         (object_track.bbox.Get_Length() > 3.0F))
      {
         obj_ok_to_decay_states = true;
      }
      else
      {
         obj_ok_to_decay_states = false;
      }

      return obj_ok_to_decay_states;
   }

   static bool Acc_Perpendicular_To_Heading_Objs(
      const F360_Object_Track_T& object_track
   )
   {
      constexpr float32_t acc_angle_heading_diff_threshold_min = F360_DEG2RAD(65.0F); //changed min threshold so its still symmetrical
      constexpr float32_t acc_angle_heading_diff_threshold_max = F360_DEG2RAD(115.0F);
      constexpr float32_t hdg_pntg_diff_threshold = 0.18F;
      constexpr float32_t acc_longi_threshold = 1.0F;
      constexpr float32_t speed_threshold = 4.0F;
      const float32_t acc_angle = F360_Atan2f(object_track.vcs_accel.lateral, object_track.vcs_accel.longitudinal);
      const float32_t acc_angle_heading_diff = std::abs(acc_angle - object_track.vcs_heading.Value());
      bool obj_ok_to_decay_states = false; 
      if (((acc_angle_heading_diff_threshold_min < acc_angle_heading_diff) 
         && (acc_angle_heading_diff < acc_angle_heading_diff_threshold_max))
         && (std::abs(object_track.hdg_ptng_disagmt) < hdg_pntg_diff_threshold)
         && (std::abs(object_track.vcs_accel.longitudinal) > acc_longi_threshold)
         && (std::abs(object_track.speed) < speed_threshold))
      {
         obj_ok_to_decay_states = true;
      }
      else
      {
         //do nothing
      }
      return obj_ok_to_decay_states;
   }
}
