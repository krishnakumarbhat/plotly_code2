/*===================================================================================*\
 * FILE:  f360_common_object_output.cpp
 *====================================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *------------------------------------------------------------------------------------
 * Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 */
#include "f360_rot_object_output.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_mounting_location.h"
#include "f360_reference_point_support_functions.h"
#include "f360_velocity.h"
#include "f360_accel.h"
#include "f360_norm_heading_angle.h"

namespace f360_variant_A
{
   struct States_In_Ref_Pnt_T {
      F360_VCS_Velocity_T vcs_velocity_in_ref_pnt;
      F360_VCS_Accel_T vcs_acceleration_in_ref_pnt;
      float32_t speed_in_ref_pnt;
      float32_t hdg_in_ref_pnt;
      float32_t tang_accel_in_ref_pnt;
      float32_t curv_in_ref_pnt;
   };
   static float calc_target_relative_velocity_longitudinal(const float target_absolute_velocity_x_iso, const float host_velocity_x_iso, const float target_position_y_iso, const float host_yaw_rate_iso)
   {
      const float host_yaw_velocity_component = -1.0F * (target_position_y_iso)*host_yaw_rate_iso;

      return target_absolute_velocity_x_iso - host_velocity_x_iso - host_yaw_velocity_component;
   }
   static float calc_target_relative_velocity_lateral(const float target_absolute_velocity_y_iso, const float host_velocity_y_iso, const float target_position_x_iso, const float host_yaw_rate_iso)
   {
      const float host_yaw_velocity_component = target_position_x_iso * host_yaw_rate_iso;

      return target_absolute_velocity_y_iso - host_velocity_y_iso - host_yaw_velocity_component;
   }
   static float calc_target_relative_acceleration_longitudinal(const float target_absolute_acc_x_iso, const float host_acc_x_iso, const float target_position_x_iso, const float host_yaw_rate_iso)
   {
      const float host_yaw_acc_component = target_position_x_iso * host_yaw_rate_iso * host_yaw_rate_iso;

      return target_absolute_acc_x_iso - host_acc_x_iso - host_yaw_acc_component;
   }
   static float calc_target_relative_acceleration_lateral(const float target_absolute_acc_y_iso, const float host_acc_y_iso, const float target_position_y_iso, const float host_yaw_rate)
   {
      const float host_yaw_acc_component = target_position_y_iso * host_yaw_rate * host_yaw_rate;

      return target_absolute_acc_y_iso - host_acc_y_iso - host_yaw_acc_component;
   }
   static void Convert_Var_To_XY_For_CTCA_Objects(const F360_Object_Track_T &object_tracks, ROT_Object_Output_T &rot_obj_output)
   {
        const float32_t speed = object_tracks.speed;
        const float32_t sin_heading = object_tracks.vcs_heading.Sin();
        const float32_t cos_heading = object_tracks.vcs_heading.Cos();
        const float32_t obj_acc = object_tracks.tang_accel;
        const float32_t curvature = object_tracks.curvature;

        // Rotate speed and acceleration variance to x and y direction
        float32_t jacobian[4][4] = {};
        float32_t jacobian_transpose[4][4] = {};
        float32_t state_covariance[4][4] = {};
        float32_t temp_mat[4][4];
        float32_t output_cov[4][4];
        jacobian[0][0] = -speed * sin_heading;                                             // d_vx /d_heading
        jacobian[0][1] = 0.0F;                                                             // d_vx /d_curvature
        jacobian[0][2] = cos_heading;                                                      // d_vx/d_speed
        jacobian[0][3] = 0.0F;                                                             // d_vx/d_acc
        jacobian[1][0] = speed * cos_heading;                                              // d_vy /d_heading
        jacobian[1][1] = 0.0F;                                                             // d_vy /d_curvature
        jacobian[1][2] = sin_heading;                                                      // d_vy/d_speed
        jacobian[1][3] = 0.0F;                                                             // d_vy/d_acc
        jacobian[2][0] = -obj_acc * sin_heading - curvature * speed * speed * cos_heading; // d_ax /d_heading
        jacobian[2][1] = -speed * speed * sin_heading;                                     // d_ax /d_curvature
        jacobian[2][2] = -2.0F * speed * curvature * sin_heading;                          // d_ax/d_speed
        jacobian[2][3] = cos_heading;                                                      // d_ax/d_acc
        jacobian[3][0] = obj_acc * cos_heading - curvature * speed * speed * sin_heading;  // d_ay /d_heading
        jacobian[3][1] = speed * speed * cos_heading;                                      // d_ay /d_curvature
        jacobian[3][2] = 2.0F * curvature * speed * cos_heading;                           // d_ay/d_speed
        jacobian[3][3] = sin_heading;                                                      // d_ay/d_acc
        state_covariance[0][0] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H];
        state_covariance[0][1] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C];
        state_covariance[0][2] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S];
        state_covariance[0][3] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A];
        state_covariance[1][0] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H];
        state_covariance[1][1] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C];
        state_covariance[1][2] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_S];
        state_covariance[1][3] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_A];
        state_covariance[2][0] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H];
        state_covariance[2][1] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_C];
        state_covariance[2][2] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S];
        state_covariance[2][3] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_A];
        state_covariance[3][0] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H];
        state_covariance[3][1] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_C];
        state_covariance[3][2] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_S];
        state_covariance[3][3] = object_tracks.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A];
        F360_Transpose_2D(jacobian, jacobian_transpose);
        F360_Matmul_MxN_NxP(jacobian, state_covariance, temp_mat, 4U, 4U, 4U);
        F360_Matmul_MxN_NxP(temp_mat, jacobian_transpose, output_cov, 4U, 4U, 4U);
        rot_obj_output.iso_x_vel_var = output_cov[0][0];
        rot_obj_output.iso_y_vel_var = output_cov[1][1];
        rot_obj_output.iso_xy_vel_cov = -output_cov[0][1];
        rot_obj_output.iso_x_acc_var = output_cov[2][2];
        rot_obj_output.iso_y_acc_var = output_cov[3][3];
        rot_obj_output.iso_xy_acc_cov = -output_cov[2][3];
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Heading_Rate_Variance()
   *===========================================================================
   * RETURN VALUE:
   * float32_t
   *
   * PARAMETERS:
   *  const float32_t heading_rate
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function computing the heading rate variance from a given heading rate
   *
   \*===========================================================================*/
    static float32_t Compute_Heading_Rate_Variance(const float32_t heading_rate)
    {
        const float32_t smallest_heading_rate_calib = 0.000F; // 0 deg/s
        const float32_t largest_heading_rate_calib = 0.698F; // 40 deg/s
        const float32_t smallest_heading_rate_std_calib = 0.052F; // 3 deg/s
        const float32_t largest_heading_rate_std_calib = 0.209F; // 12 deg/s

        const float32_t std = F360_Linear_Equation_With_Saturation(
            std::abs(heading_rate),
            smallest_heading_rate_calib,
            largest_heading_rate_calib,
            smallest_heading_rate_std_calib,
            largest_heading_rate_std_calib);

        const float32_t var = std * std;

        return var;
    }

    /*===========================================================================*\
     * FUNCTION: Compute_Object_States_In_Reference_Point(()
     *===========================================================================
     * RETURN VALUE:
     *
     * PARAMETERS:
     * const F360_Object_Track_T &object_tracks_vcs
     * States_In_Ref_Pnt_T&  states_in_ref_pnt
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Function takes an object and computes the vcs velocity and acceleration in
     * the object reference point. For CCA objects these can be taken directly from
     * the tracked object but for CTCA objects the velcity and acceleration is tracked
     * in the rear center of the object rather than in the reference point and a
     * transformation is needed. The transformation done is based on the following two
     * kinematic equations that holds for rigid bodies:
     * v2 = v1 + yaw_rate x vec_from_point1_to_point2
     * a2 = a1 + yaw_ratex yaw_rate x vec_from_point1_to_point2 + yaw_acc x vec_from_point1_to_point2
     *
    \*===========================================================================*/
    static void Compute_Object_States_In_Reference_Point(
        const F360_Object_Track_T &object_track_vcs,
        States_In_Ref_Pnt_T &states_in_ref_pnt)
    {
        if (F360_TRACKER_TRKFLTR_CTCA == object_track_vcs.trk_fltr_type)
        {
            // Compute vector from rear center to reference point
            const Point vec_from_center_to_ref_pnt_tcs = Get_Reference_Point_Pos_In_TCS(object_track_vcs.reference_point, object_track_vcs.bbox.Get_Length(), object_track_vcs.bbox.Get_Width());
            const float32_t vec_from_rear_center_to_ref_pnt_tcs[2] = { vec_from_center_to_ref_pnt_tcs.x + 0.5F * object_track_vcs.bbox.Get_Length(),vec_from_center_to_ref_pnt_tcs.y };
            float32_t vec_from_rear_center_to_ref_pnt_vcs[2];
            F360_Rotate_2D_Vector(vec_from_rear_center_to_ref_pnt_tcs[0], vec_from_rear_center_to_ref_pnt_tcs[1], object_track_vcs.bbox.Get_Orientation().Cos(), object_track_vcs.bbox.Get_Orientation().Sin(), vec_from_rear_center_to_ref_pnt_vcs[0], vec_from_rear_center_to_ref_pnt_vcs[1]);

            // Transform velocity from rear center to reference point according to:
            //    vel_ref_pnt = vel_rear_center + yaw_rate x vec_from_rear_center_to_ref_pnt
            const float32_t yaw_rate_cross_vec_from_rear_center_to_front_vcs[2] = { -object_track_vcs.heading_rate * vec_from_rear_center_to_ref_pnt_vcs[1],
                object_track_vcs.heading_rate * vec_from_rear_center_to_ref_pnt_vcs[0] };

            states_in_ref_pnt.vcs_velocity_in_ref_pnt.longitudinal = object_track_vcs.vcs_velocity.longitudinal + yaw_rate_cross_vec_from_rear_center_to_front_vcs[0];
            states_in_ref_pnt.vcs_velocity_in_ref_pnt.lateral = object_track_vcs.vcs_velocity.lateral + yaw_rate_cross_vec_from_rear_center_to_front_vcs[1];

            // Transform acceleration from rear center to reference point according to: 
            //   acc_ref_pnt = acc_rear_center + yaw_rate x yaw_rate x vec_from_rear_center_to_ref_pnt + yaw_acc x vec_from_rear_center_to_ref_pnt
            // where yaw acceleration is estimated according to:
            //   yaw_rate = speed * curvature
            //   yaw_acc = d/dt(yaw_rate) = d/dt(speed * curvature) = tang_acc * curvature  + speed * curvature_rate = / CTCA assumption of constant curvature => curvature rate = 0/ = tang_acc * curvature
            const float32_t yaw_rate_cross_yaw_rate_cross_vec_from_rear_center_to_front_vcs[2] = { -object_track_vcs.heading_rate * yaw_rate_cross_vec_from_rear_center_to_front_vcs[1],
                object_track_vcs.heading_rate * yaw_rate_cross_vec_from_rear_center_to_front_vcs[0] };
            const float32_t yaw_acc = object_track_vcs.tang_accel * object_track_vcs.curvature;
            const float32_t yaw_acc_cross_vec_from_rear_center_to_front_vcs[2] = { -yaw_acc * vec_from_rear_center_to_ref_pnt_vcs[1], yaw_acc * vec_from_rear_center_to_ref_pnt_vcs[0] };

            states_in_ref_pnt.vcs_acceleration_in_ref_pnt.longitudinal = object_track_vcs.vcs_accel.longitudinal + yaw_rate_cross_yaw_rate_cross_vec_from_rear_center_to_front_vcs[0] + yaw_acc_cross_vec_from_rear_center_to_front_vcs[0];
            states_in_ref_pnt.vcs_acceleration_in_ref_pnt.lateral = object_track_vcs.vcs_accel.lateral + yaw_rate_cross_yaw_rate_cross_vec_from_rear_center_to_front_vcs[1] + yaw_acc_cross_vec_from_rear_center_to_front_vcs[1];

            // Compute object polar states to also be in reference point
            states_in_ref_pnt.speed_in_ref_pnt = F360_Sqrtf(states_in_ref_pnt.vcs_velocity_in_ref_pnt.longitudinal * states_in_ref_pnt.vcs_velocity_in_ref_pnt.longitudinal + states_in_ref_pnt.vcs_velocity_in_ref_pnt.lateral * states_in_ref_pnt.vcs_velocity_in_ref_pnt.lateral);
            states_in_ref_pnt.hdg_in_ref_pnt = F360_Atan2f(states_in_ref_pnt.vcs_velocity_in_ref_pnt.lateral, states_in_ref_pnt.vcs_velocity_in_ref_pnt.longitudinal);
            if (object_track_vcs.speed < 0.0F)
            {
               states_in_ref_pnt.speed_in_ref_pnt = -states_in_ref_pnt.speed_in_ref_pnt;
               states_in_ref_pnt.hdg_in_ref_pnt = Normalize_Heading_Angle(states_in_ref_pnt.hdg_in_ref_pnt + F360_PI, 0.0F);
            }
            states_in_ref_pnt.tang_accel_in_ref_pnt = states_in_ref_pnt.vcs_acceleration_in_ref_pnt.longitudinal * F360_Cosf(states_in_ref_pnt.hdg_in_ref_pnt) + states_in_ref_pnt.vcs_acceleration_in_ref_pnt.lateral * F360_Sinf(states_in_ref_pnt.hdg_in_ref_pnt);
            states_in_ref_pnt.curv_in_ref_pnt = (std::abs(states_in_ref_pnt.speed_in_ref_pnt) > 1e-4F) ? (object_track_vcs.heading_rate / states_in_ref_pnt.speed_in_ref_pnt) : object_track_vcs.curvature; // Division by zero protection.
        }
        else
        {
            states_in_ref_pnt.vcs_velocity_in_ref_pnt.longitudinal = object_track_vcs.vcs_velocity.longitudinal;
            states_in_ref_pnt.vcs_velocity_in_ref_pnt.lateral = object_track_vcs.vcs_velocity.lateral;
            states_in_ref_pnt.vcs_acceleration_in_ref_pnt.longitudinal = object_track_vcs.vcs_accel.longitudinal;
            states_in_ref_pnt.vcs_acceleration_in_ref_pnt.lateral = object_track_vcs.vcs_accel.lateral;
            states_in_ref_pnt.speed_in_ref_pnt = object_track_vcs.speed;
            states_in_ref_pnt.hdg_in_ref_pnt = object_track_vcs.vcs_heading.Value();
            states_in_ref_pnt.tang_accel_in_ref_pnt = object_track_vcs.tang_accel;
            states_in_ref_pnt.curv_in_ref_pnt = object_track_vcs.curvature;
        }
     }

    static void Fill_ROT_Object_Info_From_Object_Track(
        const F360_Object_Track_T &object_tracks_vcs,
        const States_In_Ref_Pnt_T &obj_states_in_ref_pnt,
        ROT_Object_Output_T &rot_obj_output)
    {
        rot_obj_output.vcs_x_posn = object_tracks_vcs.vcs_position.x;
        rot_obj_output.vcs_y_posn = object_tracks_vcs.vcs_position.y;
        rot_obj_output.vcs_x_vel = obj_states_in_ref_pnt.vcs_velocity_in_ref_pnt.longitudinal;
        rot_obj_output.vcs_y_vel = obj_states_in_ref_pnt.vcs_velocity_in_ref_pnt.lateral;
        rot_obj_output.vcs_x_acc = obj_states_in_ref_pnt.vcs_acceleration_in_ref_pnt.longitudinal;
        rot_obj_output.vcs_y_acc = obj_states_in_ref_pnt.vcs_acceleration_in_ref_pnt.lateral;
        rot_obj_output.vcs_heading = obj_states_in_ref_pnt.hdg_in_ref_pnt;
        rot_obj_output.vcs_pointing = object_tracks_vcs.bbox.Get_Orientation().Value();
        rot_obj_output.vcs_curvature = obj_states_in_ref_pnt.curv_in_ref_pnt;
        rot_obj_output.speed = obj_states_in_ref_pnt.speed_in_ref_pnt;
        rot_obj_output.tang_accel = obj_states_in_ref_pnt.tang_accel_in_ref_pnt;

        for (int i = 0; i < 6; ++i) {
           rot_obj_output.vcs_state_variance[i] = object_tracks_vcs.errcov[i][i];
        }

        if (F360_TRACKER_TRKFLTR_CTCA == object_tracks_vcs.trk_fltr_type)
         {
            rot_obj_output.vcs_supplemental_state_covariance[0] = object_tracks_vcs.errcov[0][1];
            rot_obj_output.vcs_supplemental_state_covariance[1] = object_tracks_vcs.errcov[2][4];
            rot_obj_output.vcs_supplemental_state_covariance[2] = object_tracks_vcs.errcov[3][5];
         }
         else if (F360_TRACKER_TRKFLTR_CCA == object_tracks_vcs.trk_fltr_type)
         {
            rot_obj_output.vcs_supplemental_state_covariance[0] = object_tracks_vcs.errcov[0][3];
            rot_obj_output.vcs_supplemental_state_covariance[1] = object_tracks_vcs.errcov[1][4];
            rot_obj_output.vcs_supplemental_state_covariance[2] = object_tracks_vcs.errcov[2][5];
         }
         else
         {
            rot_obj_output.vcs_supplemental_state_covariance[0] = object_tracks_vcs.errcov[0][2];
            rot_obj_output.vcs_supplemental_state_covariance[1] = object_tracks_vcs.errcov[1][3];
            rot_obj_output.vcs_supplemental_state_covariance[2] = object_tracks_vcs.errcov[4][5];
         }

        rot_obj_output.confidence_level = object_tracks_vcs.confidenceLevel;
        rot_obj_output.time_since_stage_start = object_tracks_vcs.time_since_stage_start;
        rot_obj_output.probability_pedestrian = object_tracks_vcs.probability_pedestrian;
        rot_obj_output.probability_car = object_tracks_vcs.probability_car;
        rot_obj_output.probability_motorcycle = object_tracks_vcs.probability_motorcycle;
        rot_obj_output.probability_bicycle = object_tracks_vcs.probability_bicycle;
        rot_obj_output.probability_truck = object_tracks_vcs.probability_truck;
        rot_obj_output.probability_undet = object_tracks_vcs.probability_undet;

        rot_obj_output.movable_prob = object_tracks_vcs.movable_prob;

        rot_obj_output.ndets = object_tracks_vcs.ndets;
        rot_obj_output.num_dets_used_in_rr_msmt_update = object_tracks_vcs.num_dets_used_in_rr_msmt_update;
         
        rot_obj_output.f_onguardrail = (F360_INVALID_UNSIGNED_ID == object_tracks_vcs.on_sep_id) ? 0U : 1U; 
        rot_obj_output.trk_fltr_type = object_tracks_vcs.trk_fltr_type;
        rot_obj_output.obstacle_prob = static_cast<uint8_t>(std::round(100.0F * object_tracks_vcs.obstacle_prob));
        rot_obj_output.aeb_confidence = static_cast<uint8_t>(object_tracks_vcs.aeb_confidence);
    }

    static void Convert_From_VCS_To_ISO(
        const F360_Host_T& host_info,
        const F360_Object_Track_T &object_tracks_vcs,
        const States_In_Ref_Pnt_T &obj_states_in_ref_pnt,
        ROT_Object_Output_T &rot_obj_output)
    {
        rot_obj_output.iso_x_posn = object_tracks_vcs.vcs_position.x + host_info.dist_rear_axle_to_vcs_m;
        rot_obj_output.iso_y_posn = -object_tracks_vcs.vcs_position.y;
        rot_obj_output.iso_x_vel = obj_states_in_ref_pnt.vcs_velocity_in_ref_pnt.longitudinal;
        rot_obj_output.iso_y_vel = -obj_states_in_ref_pnt.vcs_velocity_in_ref_pnt.lateral;
        rot_obj_output.iso_x_acc = obj_states_in_ref_pnt.vcs_acceleration_in_ref_pnt.longitudinal;
        rot_obj_output.iso_y_acc = -obj_states_in_ref_pnt.vcs_acceleration_in_ref_pnt.lateral;
        rot_obj_output.iso_orientation = -object_tracks_vcs.bbox.Get_Orientation().Value();
        rot_obj_output.iso_relative_x_vel = calc_target_relative_velocity_longitudinal(rot_obj_output.iso_x_vel,
           host_info.vcs_speed * F360_Cosf(host_info.vcs_sideslip), rot_obj_output.iso_y_posn, -host_info.yaw_rate_rad);
        rot_obj_output.iso_relative_y_vel = calc_target_relative_velocity_lateral(rot_obj_output.iso_y_vel,
           -host_info.vcs_speed * F360_Sinf(host_info.vcs_sideslip), rot_obj_output.iso_x_posn, -host_info.yaw_rate_rad);
        rot_obj_output.iso_relative_x_acc = calc_target_relative_acceleration_longitudinal(rot_obj_output.iso_x_acc, host_info.vcs_long_acceleration, rot_obj_output.iso_x_posn, -host_info.yaw_rate_rad);
        rot_obj_output.iso_relative_y_acc = calc_target_relative_acceleration_lateral(rot_obj_output.iso_y_acc, -host_info.vcs_lat_acceleration, rot_obj_output.iso_y_posn, -host_info.yaw_rate_rad);
        rot_obj_output.iso_orientation_var = object_tracks_vcs.orientation_std * object_tracks_vcs.orientation_std;
        if (object_tracks_vcs.trk_fltr_type == F360_TRACKER_TRKFLTR_CTCA)
        {
            rot_obj_output.iso_x_posn_var = object_tracks_vcs.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X];
            rot_obj_output.iso_y_posn_var = object_tracks_vcs.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y];
            rot_obj_output.iso_xy_posn_cov = -object_tracks_vcs.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y];
            rot_obj_output.iso_orientation_rate = -object_tracks_vcs.heading_rate;
            rot_obj_output.iso_orientation_rate_var = Compute_Heading_Rate_Variance(object_tracks_vcs.heading_rate);
            Convert_Var_To_XY_For_CTCA_Objects(object_tracks_vcs, rot_obj_output);
        }
        else // CCA
        {
           rot_obj_output.iso_x_posn_var = object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X];
           rot_obj_output.iso_y_posn_var = object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y];
           rot_obj_output.iso_xy_posn_cov = -object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y];
           rot_obj_output.iso_x_vel_var = object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX];
           rot_obj_output.iso_y_vel_var = object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY];
           rot_obj_output.iso_xy_vel_cov = -object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY];
           rot_obj_output.iso_orientation_rate = -object_tracks_vcs.heading_rate;
           rot_obj_output.iso_orientation_rate_var = Compute_Heading_Rate_Variance(object_tracks_vcs.heading_rate);
           rot_obj_output.iso_x_acc_var = object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX];
           rot_obj_output.iso_y_acc_var = object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY];
           rot_obj_output.iso_xy_acc_cov = -object_tracks_vcs.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY];
        }

    }

    static void Compute_Object_Sensor_Source(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS], 
       const F360_Object_Track_T &object_track, 
       const rspp_variant_A::RSPP_Detection_List_T& det_list, 
       ROT_Object_Output_T &rot_obj_output)
    {
        uint16_t status_sensor_temp = 0x0000U;
        const uint16_t front_mid_sensor = 0x0001U;
        const uint16_t front_left_sensor = 0x0002U;
        const uint16_t left_side_sensor = 0x0004U;
        const uint16_t rear_left_sensor = 0x0008U;
        const uint16_t rear_mid_sensor = 0x0010U;
        const uint16_t rear_right_sensor = 0x0020U;
        const uint16_t right_side_sensor = 0x0040U;
        const uint16_t front_right_sensor = 0x0080U;
        for (uint32_t i = 0U; i < object_track.ndets; ++i)
        {
            const uint32_t det_index = object_track.detids[i] - 1U;
            const uint8_t sID = static_cast<uint8_t>(det_list.detections[det_index].raw.sensor_id);
            if ((sID > 0U) && ((object_track.status == F360_OBJECT_STATUS_NEW_UPDATED) || (object_track.status == F360_OBJECT_STATUS_UPDATED)))
            {
                switch (sensors[sID - 1U].constant.mounting_location)
                {
                case F360_MOUNTING_LOCATION_CENTER_FORWARD:
                case F360_MOUNTING_LOCATION_CENTER2_FORWARD:
                case F360_MOUNTING_LOCATION_CENTER3_FORWARD:
                    status_sensor_temp = status_sensor_temp | front_mid_sensor;
                    break;
                case F360_MOUNTING_LOCATION_LEFT_FORWARD:
                    status_sensor_temp = status_sensor_temp | front_left_sensor;
                    break;
                case F360_MOUNTING_LOCATION_LEFT_SIDE1:
                case F360_MOUNTING_LOCATION_LEFT_SIDE2:
                    status_sensor_temp = status_sensor_temp | left_side_sensor;
                    break;
                case F360_MOUNTING_LOCATION_LEFT_REAR:
                    status_sensor_temp = status_sensor_temp | rear_left_sensor;
                    break;
                case F360_MOUNTING_LOCATION_CENTER_REAR:
                case F360_MOUNTING_LOCATION_CENTER2_REAR:
                case F360_MOUNTING_LOCATION_CENTER3_REAR:
                    status_sensor_temp = status_sensor_temp | rear_mid_sensor;
                    break;
                case F360_MOUNTING_LOCATION_RIGHT_REAR:
                    status_sensor_temp = status_sensor_temp | rear_right_sensor;
                    break;
                case F360_MOUNTING_LOCATION_RIGHT_SIDE1:
                case F360_MOUNTING_LOCATION_RIGHT_SIDE2:
                    status_sensor_temp = status_sensor_temp | right_side_sensor;
                    break;
                case F360_MOUNTING_LOCATION_RIGHT_FORWARD:
                    status_sensor_temp = status_sensor_temp | front_right_sensor;
                    break;
                case F360_MOUNTING_LOCATION_UNKNOWN:
                default:
                    break;
                    // Do nothing    
                }
            }
        }
        rot_obj_output.sensor_src = status_sensor_temp;
    }

    static void Map_ROT_Object_Status(const F360_Object_Track_T &object_track, ROT_Object_Output_T &rot_obj_output)
    {
        uint8_t obj_status = 255U;
        if ((object_track.reduced_status == F360_OBJECT_STATUS_UPDATED) ||
            ((object_track.reduced_status == F360_OBJECT_STATUS_COASTED) && (object_track.num_rr_inlier_dets > 0U)))
        {
            obj_status = 0U; // MeasurementStatus_Measured
        }
        else if ((object_track.reduced_status == F360_OBJECT_STATUS_NEW) || (object_track.reduced_status == F360_OBJECT_STATUS_NEW_UPDATED))
        {
            obj_status = 1U; // MeasurementStatus_New
        }
        else if (object_track.reduced_status == F360_OBJECT_STATUS_COASTED)
        {
            obj_status = 2U; // MeasurementStatus_Predicted
        }
        else
        {
            obj_status = 255U;
        }
        rot_obj_output.object_status = obj_status;
    }

    static void Map_ROT_Object_Classification(const F360_Object_Track_T &object_track, ROT_Object_Output_T &rot_obj_output)
    {
        uint8_t tmp_obj_class = 0U;
        switch (object_track.object_class)
        {
        case F360_OBJ_CLASS_CAR:
            tmp_obj_class = 1U;
            break;
        case F360_OBJ_CLASS_MOTORCYCLE:
            tmp_obj_class = 2U;
            break;
        case F360_OBJ_CLASS_TRUCK:
            tmp_obj_class = 3U;
            break;
        case F360_OBJ_CLASS_PEDESTRIAN:
            tmp_obj_class = 4U;
            break;
        case F360_OBJ_CLASS_BICYCLE:
            tmp_obj_class = 5U;
            break;
        default:
            tmp_obj_class = 0U;
            break;
        }
        rot_obj_output.object_class = tmp_obj_class;
    }

    static void Map_Obj_Reference_Point(const F360_Object_Track_T &object_track, ROT_Object_Output_T &rot_obj_output)
    {
        uint8_t tmp_obj_ref_pt = 255U;
        switch (object_track.reference_point)
        {
        case F360_REFERENCE_POINT_CENTER:
            tmp_obj_ref_pt = 0U;
            break;
        case F360_REFERENCE_POINT_FRONT_LEFT:
            tmp_obj_ref_pt = 1U;
            break;
        case F360_REFERENCE_POINT_FRONT:
            tmp_obj_ref_pt = 2U;
            break;
        case F360_REFERENCE_POINT_FRONT_RIGHT:
            tmp_obj_ref_pt = 3U;
            break;
        case F360_REFERENCE_POINT_RIGHT:
            tmp_obj_ref_pt = 4U;
            break;
        case F360_REFERENCE_POINT_REAR_RIGHT:
            tmp_obj_ref_pt = 5U;
            break;
        case F360_REFERENCE_POINT_REAR:
            tmp_obj_ref_pt = 6U;
            break;
        case F360_REFERENCE_POINT_REAR_LEFT:
            tmp_obj_ref_pt = 7U;
            break;
        case F360_REFERENCE_POINT_LEFT:
            tmp_obj_ref_pt = 8U;
            break;
        default:
            tmp_obj_ref_pt = 0U;
            break;
        }
        rot_obj_output.reference_point = tmp_obj_ref_pt;
    }

    static void Map_ROT_Object_Motion_Status(const F360_Object_Track_T& object_track, ROT_Object_Output_T& rot_obj_output)
    {
       if (object_track.f_moving)
       {
          rot_obj_output.movement_status = 3U;
       }
       else if (object_track.f_moveable)
       {
          rot_obj_output.movement_status = 2U;
       }
       else
       {
          rot_obj_output.movement_status = 1U;
       }
    }

    static void Map_ROT_Object_Occlusion_Status(const F360_Object_Track_T& object_track, ROT_Object_Output_T& rot_obj_output)
    {
       switch (object_track.occlusion_status)
       {
       case OCCLUSION_STATUS_OCCLUDED:
          rot_obj_output.occlusion_status = 1U;
          break;
       case OCCLUSION_STATUS_ON_EDGE:
          rot_obj_output.occlusion_status = 2U;
          break;
       case OCCLUSION_STATUS_VISIBLE:
          rot_obj_output.occlusion_status = 3U;
          break;
       case OCCLUSION_STATUS_UNDEFINED:
       default:
          rot_obj_output.occlusion_status = 0U;
          break;
       }
    }

    static void Map_ROT_Underdrivable_OCG_Status(const F360_Object_Track_T& object_track, ROT_Object_Output_T& rot_obj_output)
    {
        switch (object_track.underdrivable_status_ocg)
        {
        case ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER:
            rot_obj_output.underdrivable_status_ocg = 0U;
            break;
        case ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER:
            rot_obj_output.underdrivable_status_ocg = 1U;
            break;
        case ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER:
            rot_obj_output.underdrivable_status_ocg = 2U;
            break;
        default:
            rot_obj_output.underdrivable_status_ocg = 3U; // ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER = 3
            break;
        }
    }

    static void Map_ROT_Drivable_SG_Status(const F360_Object_Track_T& object_track, ROT_Object_Output_T& rot_obj_output)
    {
       switch (object_track.drivable_status_sg)
       {
       case sg::SG_Drivability_Class_T::UNCLASSIFIED:
          rot_obj_output.drivable_status_sg = 0U;
          break;
       case sg::SG_Drivability_Class_T::OVERDRIVABLE:
          rot_obj_output.drivable_status_sg = 1U;
          break;
       case sg::SG_Drivability_Class_T::NONDRIVABLE:
          rot_obj_output.drivable_status_sg = 2U;
          break;
       case sg::SG_Drivability_Class_T::UNDERDRIVABLE:
          rot_obj_output.drivable_status_sg = 3U;
          break;
       default:
          rot_obj_output.drivable_status_sg = 0U; // sg::SG_Drivability_Class_T::UNCLASSIFIED = 0
          break;
       }
    }

    void Set_ROT_Object_Output(
       const F360_Host_T &host_info, 
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       const rspp_variant_A::RSPP_Detection_List_T& det_list,
       const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS], 
       ROT_Object_Output_T (&rot_obj_output)[NUMBER_OF_REDUCED_OBJECT_TRACKS])
    {
        for (uint32_t obj_idx = 0U; obj_idx < NUMBER_OF_OBJECT_TRACKS; ++obj_idx)
        {
            const uint32_t obj_reduced_id = static_cast<uint32_t>(object_tracks[obj_idx].reduced_id);
            if ((obj_reduced_id > 0U) && (obj_reduced_id <= NUMBER_OF_REDUCED_OBJECT_TRACKS))
            {
                const uint32_t rot_obj_idx = obj_reduced_id - 1U;
                States_In_Ref_Pnt_T states_in_ref_pnt = {};
                Compute_Object_States_In_Reference_Point(object_tracks[obj_idx], states_in_ref_pnt); // For CTCA object velocity and acceleration are tracked in object REAR CENTER rather than in the REFERENCE POINT so we need to do a transformation
                Fill_ROT_Object_Info_From_Object_Track(object_tracks[obj_idx], states_in_ref_pnt, rot_obj_output[rot_obj_idx]);
                Convert_From_VCS_To_ISO(host_info, object_tracks[obj_idx], states_in_ref_pnt, rot_obj_output[rot_obj_idx]); // convert radar tracker VCS(SAE is used) to ISO coordinate
                Compute_Object_Sensor_Source(sensors, object_tracks[obj_idx], det_list, rot_obj_output[rot_obj_idx]);
                Map_Obj_Reference_Point(object_tracks[obj_idx], rot_obj_output[rot_obj_idx]);
                Map_ROT_Object_Status(object_tracks[obj_idx], rot_obj_output[rot_obj_idx]);
                Map_ROT_Object_Classification(object_tracks[obj_idx], rot_obj_output[rot_obj_idx]);
                Map_ROT_Object_Motion_Status(object_tracks[obj_idx], rot_obj_output[rot_obj_idx]);
                Map_ROT_Underdrivable_OCG_Status(object_tracks[obj_idx], rot_obj_output[rot_obj_idx]);
                rot_obj_output[rot_obj_idx].probability_underdrivable_ocg = object_tracks[obj_idx].probability_underdrivable_ocg;
                Map_ROT_Drivable_SG_Status(object_tracks[obj_idx], rot_obj_output[rot_obj_idx]);
                rot_obj_output[rot_obj_idx].drivable_confidence_sg = object_tracks[obj_idx].drivable_confidence_sg;
                rot_obj_output[rot_obj_idx].time_since_published = object_tracks[obj_idx].time_since_downselected;
                rot_obj_output[rot_obj_idx].time_since_created = object_tracks[obj_idx].time_since_initialization;
                rot_obj_output[rot_obj_idx].length = object_tracks[obj_idx].length_processed;
                rot_obj_output[rot_obj_idx].length_var = object_tracks[obj_idx].length_uncertainty;
                rot_obj_output[rot_obj_idx].width = object_tracks[obj_idx].width_processed;
                rot_obj_output[rot_obj_idx].width_var = object_tracks[obj_idx].width_uncertainty;
                rot_obj_output[rot_obj_idx].id = object_tracks[obj_idx].id;
                rot_obj_output[rot_obj_idx].unique_id = object_tracks[obj_idx].unique_id;
                rot_obj_output[rot_obj_idx].existence_probability = object_tracks[obj_idx].exist_prob;
                rot_obj_output[rot_obj_idx].mirror_prob = object_tracks[obj_idx].mirror_prob;
                rot_obj_output[rot_obj_idx].radar_cross_section = object_tracks[obj_idx].average_rcs;
                rot_obj_output[rot_obj_idx].otg_height = object_tracks[obj_idx].f_moving ? object_tracks[obj_idx].otg_height : 0.0F;
                Map_ROT_Object_Occlusion_Status(object_tracks[obj_idx], rot_obj_output[rot_obj_idx]);
            }
        }
    }


    void Set_ROT_Scl_Faults(const Functional_Safety_Faults_Log_T& functional_safety_faults_log,
       All_SCL_Faults_T& fault_status)
    {
       // Functional_Safety_Faults_Log_T is currently populated in Log_Functional_Safety_Faults() where most signals are booleans but converted to uint8 for logging
       fault_status.core_info_faults.cnt_loops_no_increase = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.core_info.cnt_loops_no_increase);
       fault_status.core_info_faults.elapsed_time_above_upper_limit = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.core_info.elapsed_time_above_upper_limit);
       fault_status.core_info_faults.elapsed_time_below_lower_limit = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.core_info.elapsed_time_below_lower_limit);
       fault_status.core_info_faults.time_us_no_increase = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.core_info.time_us_no_increase);

       fault_status.host_info_faults.host_lateral_acceleration_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.host_info.host_lateral_acceleration_invalid);
       fault_status.host_info_faults.host_longitudinal_acceleration_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.host_info.host_longitudinal_acceleration_invalid);
       fault_status.host_info_faults.host_speed_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.host_info.host_speed_invalid);
       fault_status.host_info_faults.host_yawrate_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.host_info.host_yawrate_invalid);
       fault_status.host_info_faults.vehicle_index_no_increase = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.host_info.vehicle_index_no_increase);

       fault_status.raw_detection_faults.range_is_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.raw_detection.range_is_invalid);
       fault_status.raw_detection_faults.range_rate_is_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.raw_detection.range_rate_is_invalid);
       fault_status.raw_detection_faults.azimuth_is_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.raw_detection.azimuth_is_invalid);
       fault_status.raw_detection_faults.elevation_is_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.raw_detection.elevation_is_invalid);

       for (uint8_t i = 0U; i < static_cast<uint8_t>(f360_variant_A::MAX_NUMBER_OF_SENSORS); i++)
       {
          fault_status.radar_sensor_calib_faults[i].boresight_angle_is_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.sensors_calibs[i].boresight_angle_is_invalid);
          fault_status.radar_sensor_calib_faults[i].mounting_pos_is_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.sensors_calibs[i].mounting_pos_is_invalid);
          fault_status.radar_sensor_calib_faults[i].polarity_is_invalid = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.sensors_calibs[i].polarity_is_invalid);
          
          fault_status.radar_sensor_faults[i].look_index_no_increase = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.sensors[i].look_index_no_increase);
          fault_status.radar_sensor_faults[i].sensor_vs_tracker_timestamp_divergence = static_cast<bitfield8_t>(functional_safety_faults_log.input_faults.sensors[i].sensor_vs_tracker_timestamp_divergence);
       }

       fault_status.object_faults.f_track_accelerations_faulty = static_cast<bitfield8_t>(functional_safety_faults_log.output_faults.f_track_accelerations_faulty);
       fault_status.object_faults.f_track_positions_faulty = static_cast<bitfield8_t>(functional_safety_faults_log.output_faults.f_track_positions_faulty);
       fault_status.object_faults.f_track_velocities_faulty = static_cast<bitfield8_t>(functional_safety_faults_log.output_faults.f_track_velocities_faulty);
       fault_status.object_faults.f_severe_angle_jump_presence_fault = static_cast<bitfield8_t>(functional_safety_faults_log.output_faults.f_severe_angle_jump_presence_fault);

       // functional_safety_faults_log.scl_output_faults.overall_fault_status is 'natively' of type CYCLE_FAULT_STATUS - uint8_t
       fault_status.overall_fault_status = functional_safety_faults_log.scl_output_faults.overall_fault_status;

    }
}
