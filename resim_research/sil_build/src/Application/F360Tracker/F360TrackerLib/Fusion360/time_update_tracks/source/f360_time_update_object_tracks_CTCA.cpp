/*===================================================================================*\
* FILE:  f360_time_update_object_track_CTCA.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains funcions to implement the prediction step of the Kalman Filter for the
* Coordinated Turn Constant Acceleration (CTCA) motion model
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_time_update_object_tracks_CTCA.h"
#include "f360_math.h"
#include "f360_norm_heading_angle.h"
#include "f360_math_func.h"
#include "f360_get_wall_time.h"
#include "f360_calculate_jacobian_CTCA.h"
#include "f360_handle_spd_and_acc_when_stopping.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_reference_point_support_functions.h"


namespace f360_variant_A
{
   static void Decrease_Hdg_Ptng_Disagreement_Magnitude(
      float32_t& hdg_ptng_disagmt);

   /*===========================================================================*\
   * FUNCTION: Time_Update_Object_Track_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t elapsed_time_s,
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_Tracker_Info_T &tracker_info,
   * const F360_Calibrations_T &calibs,
   * F360_TRKR_TIMING_INFO_T &timing_info
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
   * This function performs a Kalman filter time update for CTCA objects.
   * Between two tracker iterations host has possibly moved causing a translation and rotation of the VCS.
   * Time update therefore consists of two steps:
               1) KF Predict/time update the state vector in the old VCS without accounting for host motion 
               2) Account for host motion by transform the predicted states from the old VCS system to the new.
   * This function only considers the first step. The transformation of the state vector from the old VCS to the
   * new VCS is performed outside of this function.
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/

   void Time_Update_Object_Track_CTCA(
      const float32_t elapsed_time_s,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T &calibs,
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      const float32_t start_time = get_wall_time();

      const uint32_t num_active_objs = static_cast<uint32_t> (tracker_info.num_active_objs);

      for (uint32_t j = 0U; j < num_active_objs; j++)
      {
         const uint32_t obj_idx = static_cast<uint32_t> (tracker_info.active_obj_ids[j]) - 1U;
         F360_Object_Track_T & obj = object_tracks[obj_idx];
         if (obj.trk_fltr_type != F360_TRACKER_TRKFLTR_CTCA)
         {
            continue;
         }

         // Store the position from previous tracker iteration
         obj.prev_vcs_center_pos.x = obj.bbox.Get_Center().x;
         obj.prev_vcs_center_pos.y = obj.bbox.Get_Center().y;
         obj.prev_predicted_vcs_y_pos = obj.predicted_vcs_position.y;

         // Extract the states
         const float32_t prev_heading = obj.vcs_heading.Value();
         const float32_t prev_speed = obj.speed;
         const float32_t prev_curvature = obj.curvature;
         const float32_t prev_tangential_acc = obj.tang_accel;
         const float32_t prev_cos_heading = obj.vcs_heading.Cos();
         const float32_t prev_sin_heading = obj.vcs_heading.Sin();

         // Compute some support variables
         const float32_t prev_yaw_rate = prev_curvature * prev_speed;
         const float32_t T = elapsed_time_s;
         const float32_t T2 = T*T;
         const float32_t T3 = T2*T;
         const float32_t T4 = T3*T;
         const float32_t ce1 = prev_speed * T + 0.5F * T2 * prev_tangential_acc;
         const float32_t ce2 = 0.5F * T2 * prev_speed * prev_yaw_rate + 0.5F * T3 * prev_tangential_acc * prev_yaw_rate + 
            0.125F * prev_tangential_acc * prev_tangential_acc * prev_curvature * T4;
         const float32_t ce3 = 0.5F * prev_speed * prev_speed * T2 + 0.5F * prev_speed * prev_tangential_acc * T3 + 0.125F * prev_tangential_acc * prev_tangential_acc * T4;

         float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
         Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

         // Time update of the object states
         obj.speed = prev_speed + prev_tangential_acc * T;
         // Constant curvature and tangential acceleration are assumed constant so the states curvature and tang_accel are
         // unaffected by the time update

         // To prevent object speed and acceleration estimates to overshoot when object comes to a stop after breaking hard
         Handle_Spd_And_Acc_When_Stopping_CTCA(prev_speed, calibs, obj);

         // These equations correspond to updating position of object reference point based on estimated heading, curvature, speed and acceleration at object center rear.
         // Derivations can be found in documents CTCA_filter.pdf and CTCA_filter_add_on_due_to_pos_in_different_point_than_other_states.pdf and were derived in DEX-3335.
         // Documents can be found here: https://spo.aptiv.com/sites/0304-RSE_2/active/ActiveSafetyAlgorithmGroup/F360Core/SysSWITnV/Forms/AllItems.aspx?viewpath=%2Fsites%2F0304%2DRSE%5F2%2Factive%2FActiveSafetyAlgorithmGroup%2FF360Core%2FSysSWITnV%2FForms%2FAllItems%2Easpx&id=%2Fsites%2F0304%2DRSE%5F2%2Factive%2FActiveSafetyAlgorithmGroup%2FF360Core%2FSysSWITnV%2FENG06%20Software%20Detailed%20Design%20and%20Construct%2FConcepts&viewid=f1c898ef%2D6eeb%2D49cc%2D8954%2D12295ef144b0
         const float32_t dx = prev_cos_heading * ce1 - prev_sin_heading * ce2 - prev_curvature * (tcs_vec_from_center_rear_to_ref_pnt[0] * (prev_sin_heading * ce1 + prev_curvature * prev_cos_heading * ce3) + tcs_vec_from_center_rear_to_ref_pnt[1] * (prev_cos_heading * ce1 - prev_curvature * prev_sin_heading * ce3));
         const float32_t dy = prev_sin_heading * ce1 + prev_cos_heading * ce2 + prev_curvature * (tcs_vec_from_center_rear_to_ref_pnt[0] * (prev_cos_heading * ce1 - prev_curvature * prev_sin_heading * ce3) - tcs_vec_from_center_rear_to_ref_pnt[1] * (prev_sin_heading * ce1 + prev_curvature * prev_cos_heading * ce3));
         obj.vcs_position.Translate(dx, dy);
         obj.bbox.Translate(dx, dy);

         // hdg_ptng_disagmt is allowed to be non-zero for a short time after transitioning from CCA to CTCA but its magnitude needs to be reduced to 0
         Decrease_Hdg_Ptng_Disagreement_Magnitude(obj.hdg_ptng_disagmt);

         if (obj.f_moving) // There is a singularity for heading when the speed is 0 so only update heading when object is moving
         {
            (void)obj.vcs_heading.Value(prev_heading + prev_yaw_rate * T + 0.5F * prev_curvature * prev_tangential_acc * T2).Normalize();
            obj.Set_Bbox_Orientation(obj.vcs_heading + obj.hdg_ptng_disagmt); // Note: hdg_ptng_disagmt can be non-zero only for a short period of time after CCA to CTCA transition, after that it should be 0
         }

         // Time update of the object state covariance matrix
         float32_t q_mat_ctca[6][6] = {};
         Adaptive_Tuning_Of_Proc_Noise_Cov(T, obj, calibs, q_mat_ctca);
         float32_t F[6][6] = {};
         F360_Calculate_Jacobian_CTCA(prev_cos_heading, prev_sin_heading, prev_speed, prev_curvature, prev_tangential_acc, T, tcs_vec_from_center_rear_to_ref_pnt, F);

         float32_t tmp_mat[6][6] = {};
         float32_t tmp_mat1[6][6] = {};
         F360_Matmul_6x6_6x6(F, obj.errcov, tmp_mat);
         F360_Matmul_6x6_6x6_Transpose(tmp_mat, F, tmp_mat1);

         for (uint32_t i = 0U; i < 6U; i++)
         {
            for (uint32_t k = 0U; k < 6U; k++)
            {
               obj.errcov[i][k] = tmp_mat1[i][k] + q_mat_ctca[i][k];
            }
         }

         Saturate_Curvature_Variance(calibs.k_max_allowed_curv_variance, obj.errcov);

         // Transform the polar representation of speed and heading used in the CTCA model to cartesian representation of velocity
         obj.vcs_velocity.longitudinal = obj.speed * obj.vcs_heading.Cos();
         obj.vcs_velocity.lateral = obj.speed * obj.vcs_heading.Sin();

         // Update object's acceleration
         obj.heading_rate = obj.curvature * obj.speed;
         const float32_t non_tangential_accel = obj.heading_rate * obj.speed;
         obj.vcs_accel.longitudinal = obj.tang_accel * obj.vcs_heading.Cos() - non_tangential_accel * obj.vcs_heading.Sin();
         obj.vcs_accel.lateral = obj.tang_accel * obj.vcs_heading.Sin() + non_tangential_accel * obj.vcs_heading.Cos();
      }

      timing_info.time_update_obj_trks_ctca = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Adaptive_Tuning_Of_Proc_Noise_Cov()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t T
   * const F360_Object_Track_T &obj
   * const F360_Calibrations_T & calibs
   * float32_t (&q_mat_ctca)[STATE_DIMENSION][STATE_DIMENSION]
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
   * This function compute the process noise covariance matrix used in CTCA Kalman
   * filter time update
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Adaptive_Tuning_Of_Proc_Noise_Cov(
      const float32_t T,
      const F360_Object_Track_T& obj,
      const F360_Calibrations_T& calibs,
      float32_t(&q_mat_ctca)[STATE_DIMENSION][STATE_DIMENSION])
   {
      const float32_t T2 = T * T;
      const float32_t T3 = T2 * T;
      const float32_t T4 = T3 * T;
      const float32_t T5 = T4 * T;

      float32_t not_rotated_pos_cov[2][2] = {};
      
      /* Direction of travelling:
         Consider independent from cross directiuon of travelling (this is approximation).
         Use a triple integrator model with independent noise in all three states.
         Ignore cross covariances, only consider variance */

      // Independent tangential acceleration noise: Increase uncertainty for acceleration if acceleration is large to better track hard breaking scenarios
      const float32_t tang_acc_scale_factor = F360_Linear_Equation_With_Saturation(std::abs(obj.tang_accel), calibs.k_min_acc_for_increasing_acc_noise, calibs.k_max_acc_for_increasing_acc_noise, 1.0F, calibs.k_max_acc_scale_factor);
      const float32_t adapted_noise_acc = tang_acc_scale_factor * calibs.k_normal_noise_acc;

      // Independent speed noise
      const float32_t adapted_noise_speed = calibs.k_normal_noise_speed;
      
      // Independent position noise in (travelling direction)
      const float32_t adapted_noise_pos_para = calibs.k_normal_noise_pos_para;

      q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A] = adapted_noise_acc * T;
      q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = 0.333F * adapted_noise_acc * T3 + adapted_noise_speed * T;
      not_rotated_pos_cov[0][0] = 0.050F * adapted_noise_acc * T5 + 0.333F * adapted_noise_speed * T3 + adapted_noise_pos_para * T;



      /* Cross direction of travelling:
         Consider independent from direction of travelling (this is an approximation)
         Use a triple integrator model with independent noise in all three states.
         Ignore cross covariances, only consider variance */

      // Independent curvature noise: Assumtion is that below a certain speed the object motion is restricted by the minimum turn radius/maximum
      //                              curvature rather than the maximum non-tangential acceleration. Above this speed threshold motion is restricted by
      //                              maximum possible non-tangential acceleration:
      //                                 lat_acc = speed^2*curvature <=> curvature = lat_acc / speed ^2
      //                              For large speed the noise in curvature should hence be noise in lateral acceleration / speed ^4 (^4 since we need to square to get the variance)
      //                              For small speeds we want to get the same curvature response independent of speed (i.e. it will take same time to reach maximum curvature independent on speed)
      const float32_t abs_speed = std::abs(obj.speed);
      const float32_t saturated_abs_speed = std::max(abs_speed, calibs.k_q_tuning_orth_direction_speed_breakpoint);
      const float32_t speed_sq = abs_speed * abs_speed;
      const float32_t saturated_speed_sq = saturated_abs_speed * saturated_abs_speed;
      const float32_t speed_pow4 = speed_sq * speed_sq;
      const float32_t saturated_speed_pow4 = saturated_speed_sq * saturated_speed_sq;
      const float32_t one_over_saturated_speed_pow_4 = 1.0F / saturated_speed_pow4;
      
      const float32_t curv_scale_factor = F360_Linear_Equation_With_Saturation(abs_speed, calibs.k_q_tuning_orth_direction_speed_breakpoint, 25.0F, 1.0F, 0.15F); // In smaller speeds drivers can normally withstand a higher lateral acceleration compared to in higher speeds (see for example "On the human control of vehicles: an experimental study of acceleration" by Mauro Da Lio and Andrea Saroldi published on the 2014/06/01 in journal "European Transport Research Review")
      const float32_t adapted_noise_curv = curv_scale_factor * calibs.k_normal_noise_curv * one_over_saturated_speed_pow_4;

      // Independent heading noise 
      const float32_t adapted_noise_heading = calibs.k_normal_noise_hdg;
      

      // Position in cross-heading direction
      const float32_t adapted_noise_pos_orth = calibs.k_normal_noise_pos_orth;

      q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = adapted_noise_curv * T;
      // Model used is yaw_rate = speed * curvature.
      // Linearize wrt curvature (since we have a simplified assumption of  independent directions 
      // such that speed in this model is considered a know parameter rather than an uncertain
      // state) and then one integration.
      q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H] = speed_sq * 0.333F * adapted_noise_curv * T3 + adapted_noise_heading * T;
      // Model used is non_tang_acc = speed^2 * curvature.
      // Linearize wrt curvature (we have a simplified assumption of independent directions 
      // such that speed in this model is considered a know parameter rather than an uncertain
      // state) and then double integration. Also ignore the propagation of the independent heading noise
      // (Note: ok for now since set to 0. If we introduce it later then we might need to change this
      not_rotated_pos_cov[1][1] = speed_pow4 * 0.050F * adapted_noise_curv * T5 + adapted_noise_pos_orth * T;

      // Rotate position covariance to VCS
      const float32_t rotation_angle = (obj.speed < 0.0F) ? obj.vcs_heading.Value() + F360_PI : obj.vcs_heading.Value();
      float32_t rotated_pos_cov[2][2];
      Rotate_2D_Covariance_Matrix(F360_Cosf(rotation_angle), F360_Sinf(rotation_angle), not_rotated_pos_cov, rotated_pos_cov);
      q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] = rotated_pos_cov[0][0];
      q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y] = rotated_pos_cov[0][1];
      q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_X] = q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y];
      q_mat_ctca[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] = rotated_pos_cov[1][1];
   }

   /*===========================================================================*\
   * FUNCTION: Saturate_Curvature_Variance()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t k_max_curvature_variance,
   * float32_t (&obj_errcov)[STATE_DIMENSION][STATE_DIMENSION]
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
   * This function checks if the curvature variance is larger than
   * k_max_curvature_variance. If so then the curvature variance is saturated.
   * All cross covariance elements related to curbature state are also impacted
   * by the saturation. They are modified such that the correlation between the
   * curvature state and other states are unchanged by the saturation.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Saturate_Curvature_Variance(
      const float32_t k_max_curvature_variance,
      float32_t (&obj_errcov)[STATE_DIMENSION][STATE_DIMENSION])
   {
      if (obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] > k_max_curvature_variance)
      {
         const float32_t decrease_factor_sq = k_max_curvature_variance / obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C];
         const float32_t decrease_factor = F360_Sqrtf(decrease_factor_sq);

         obj_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C] * decrease_factor;
         obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C];

         obj_errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C] * decrease_factor;
         obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C];

         obj_errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C] * decrease_factor;
         obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C];

         obj_errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_C] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_C] * decrease_factor;
         obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_S] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_C];

         obj_errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_C] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_C] * decrease_factor;
         obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_A] = obj_errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_C];

         obj_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = k_max_curvature_variance;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Decrease_Hdg_Ptng_Disagreement_Magnitude()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * float32_t& hdg_ptng_disagmt
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
   * For CTCA objects the heading pointing disagreement is only allowed to be non-zero for a short
   * period of time right after CCA to CTCA transition. After that it has to be zero. This function
   * decreases the magnitude of the heading pointing disagreement value until it reaches zero.
   * the 
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Decrease_Hdg_Ptng_Disagreement_Magnitude(
      float32_t& hdg_ptng_disagmt)
   {
      const float32_t k_max_hdg_ptng_disagmt_adjustment = F360_DEG2RAD(0.75F);
      if (std::abs(hdg_ptng_disagmt) < k_max_hdg_ptng_disagmt_adjustment)
      {
         // Magnitude of hdg_ptng_disagmt is small. Set to zero
         hdg_ptng_disagmt = 0.0F;
      }
      else if (hdg_ptng_disagmt > 0.0F)
      {
         // Magnitude of hdg_ptng_disagmt is large and signal is positive. Decrease the value towards zero
         hdg_ptng_disagmt -= k_max_hdg_ptng_disagmt_adjustment;
      }
      else // hdg_ptng_disagmt is largely negative
      {
         // Magnitude of hdg_ptng_disagmt is large and signal is negative. Increase the value towards zero.
         hdg_ptng_disagmt += k_max_hdg_ptng_disagmt_adjustment;
      }
   }
}
