/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_populate_track_properties.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Populate_Track_Properties() and some of its helper functions.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_populate_track_properties.h"
#include "f360_math_func.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_static_env_polys_support_functions.h"
#include "f360_update_object_reference_point.h"
#include "f360_calculate_obstacle_prob.h"

namespace f360_variant_A
{
   static void Init_Obj_Track_Covariances(
      const F360_Host_T& host,
      F360_Object_Track_T& obj);

   static bool Determine_Refpoint_And_Size(const F360_Host_T& host,
      const F360_Calibrations_T& calibrations,
      const F360_Globals_T& globals,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj);

   static void Determine_Track_Position(
      const F360_Cluster_T& cluster,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj);

   /*===========================================================================*\
   * FUNCTION: Populate_Track_Properties()
   *===========================================================================
   * RETURN VALUE:
   * NONE
   *
   * PARAMETERS:
   * const F360_Globals_T& globals
   * const F360_Calibrations_T& calibrations
   * const F360_Host_T& host
   * const F360_Cluster_T& cluster
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
   * const F360_Detection_Hist_T& det_hist,
   * const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const uint32_t new_unique_id
   * const float32_t init_longvel
   * const float32_t init_latvel
   * F360_Object_Track_T& obj
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
   * This function Initializes the states of a new object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Populate_Track_Properties(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const F360_Cluster_T& cluster,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Hist_T& det_hist,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const uint32_t new_unique_id,
      const float32_t init_longvel,
      const float32_t init_latvel,
      F360_Object_Track_T& obj)
   {
      obj.unique_id = new_unique_id;
      obj.status = F360_OBJECT_STATUS_NEW_UPDATED;
      obj.raw_confidence_level = calibrations.k_init_default_confidence;
      obj.confidenceLevel = calibrations.k_init_default_confidence;
      obj.conf_longitudinal_position = CONF9_NONE;
      obj.conf_lateral_position = CONF9_NONE;
      obj.conf_speed = CONF9_NONE;
      obj.filtered_hist_assoc_det_rr_err_var = 2.0F;

      obj.hdg_ptng_disagmt = 0.0F;
      obj.time_since_stage_start = 0.0F;
      obj.time_since_initialization = 0.0F;
      obj.time_since_track_updated = 0.0F;
      obj.time_since_split = -1.0F;
      obj.vcs_accel.longitudinal = 0.0F;
      obj.vcs_accel.lateral = 0.0F;
      obj.tang_accel = 0.0F;
      obj.curvature = 0.0F;
      obj.heading_rate = 0.0F;
      obj.num_updates_since_init = 0U;
      obj.exist_prob = 0.97F;
      obj.priority = 1.0F;


      // Properties related to init velocity.
      obj.vcs_velocity.longitudinal = init_longvel;
      obj.vcs_velocity.lateral = init_latvel;
      obj.speed = F360_Get_Hypotenuse(obj.vcs_velocity.longitudinal, obj.vcs_velocity.lateral);
      obj.f_moving = (obj.speed > globals.obj_mov_stat_spd_thresh);
      obj.movable_prob = obj.f_moving ? 1.0F : 0.0F;
      obj.time_since_started_move = obj.f_moving ? 0.0F : -1.0F;
      obj.f_vehicular_trk = (obj.speed > globals.obj_vehicular_spd_thresh);
      obj.low_rcs_dets_cnt = cluster.low_rcs_dets_cnt;
      obj.num_rr_inlier_dets = obj.ndets;
      obj.time_since_measurement = cluster.time_since_measurement;

      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      (void)obj.vcs_heading.Value(F360_Atan2f(obj.vcs_velocity.lateral, obj.vcs_velocity.longitudinal));

      // Properties related to init position, reference point and object dimensions
      obj.vcs_position = Point(cluster.vcs_position_x, cluster.vcs_position_y);
      obj.bbox_center_otg_altitude = -cluster.vcs_position_z;
      const bool f_update_postition = Determine_Refpoint_And_Size(host, calibrations, globals, sensors, det_props, obj);
      if (f_update_postition)
      {
         Determine_Track_Position(cluster, det_props, obj);
      }

      // Other properties
      Init_Obj_Track_Covariances(host, obj);

      Flag_Single_Object_On_And_Behind_SEP(sep, calibrations, globals, obj);


      // Initialize pseudo heading filter
      float32_t dets_x_pos[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      float32_t dets_y_pos[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      float32_t dets_time_since_measurement[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      uint32_t num_dets = 0U;
      float32_t max_rcs = -1000.0F;
      for (int16_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         dets_x_pos[num_dets] = det_props[det_idx].vcs_position.x;
         dets_y_pos[num_dets] = det_props[det_idx].vcs_position.y;
         const int32_t sensor_idx = raw_detections.detections[det_idx].raw.sensor_id - 1;
         dets_time_since_measurement[num_dets] = sensors[sensor_idx].refined.time_since_measurement_s;
         max_rcs = fmaxf(max_rcs, raw_detections.detections[det_idx].raw.rcs);
         num_dets++;
      }
      for (int16_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         dets_x_pos[num_dets] = det_hist.det_data[det_idx].vcs_position_x;
         dets_y_pos[num_dets] = det_hist.det_data[det_idx].vcs_position_y;
         dets_time_since_measurement[num_dets] = det_hist.det_data[det_idx].time_since_meas;
         num_dets++;
      }
      uint32_t sort_idx[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER] = {};
      (void)F360_Sort(static_cast<uint32_t>(num_dets), true, dets_time_since_measurement, sort_idx);

      const float32_t forgetting_factor = F360_Powf(0.5F, 0.05F * obj.speed / 3.0F);
      for (uint32_t i = 0U; i < num_dets; i++)
      {
         const uint32_t det_idx = sort_idx[i];
         const float32_t time_diff = dets_time_since_measurement[det_idx] - dets_time_since_measurement[sort_idx[1]];
         const float32_t scans_since_measurement = std::floor(time_diff / 0.05F);
         const float32_t weight = F360_Powf(forgetting_factor, scans_since_measurement);
         obj.pseudo_hdg_state_vec[0] += weight;
         obj.pseudo_hdg_state_vec[1] += weight * dets_x_pos[det_idx];
         obj.pseudo_hdg_state_vec[2] += weight * dets_y_pos[det_idx];
         obj.pseudo_hdg_state_vec[3] += weight * dets_x_pos[det_idx] * dets_x_pos[det_idx];
         obj.pseudo_hdg_state_vec[4] += weight * dets_y_pos[det_idx] * dets_y_pos[det_idx];
         obj.pseudo_hdg_state_vec[5] += weight * dets_x_pos[det_idx] * dets_y_pos[det_idx];
      }

      obj.time_since_cluster_created = dets_time_since_measurement[num_dets - 1U];
      obj.maximum_rcs = max_rcs;

      const float32_t range = F360_Get_Hypotenuse(obj.bbox.Get_Center().x, obj.bbox.Get_Center().y); // need sqrt for linear interpolation of range
      obj.obstacle_prob = Calculate_Instantaneous_Obstacle_Prob(obj, range);
      obj.aeb_confidence = AEB_CONF_LOW;

   }

   /*===========================================================================*\
   * FUNCTION: Init_Obj_Track_Covariances()
   *===========================================================================
   * RETURN VALUE:
   * NONE
   *
   * PARAMETERS:
   * const F360_Host_T& host
   * F360_Object_Track_T& obj
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
   * This function initializes the object variances/covariances of a new object. The P matrix
   * for the CCA Kalman filter and the CCA pointing/yawrate filter is being initialized
   * as well as the object length and width uncertainties.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Init_Obj_Track_Covariances(
      const F360_Host_T& host,
      F360_Object_Track_T& obj)
   {
      (void)memset(obj.errcov, 0, sizeof(obj.errcov));

      // Compute position covariance
      float32_t cov_mat[2][2];
      const float32_t vec_host_center_to_obj[2] = { obj.vcs_position.x + 0.6F * host.dist_rear_axle_to_vcs_m, obj.vcs_position.y };
      const float32_t angle_host_center_to_obj = F360_Atan2f(vec_host_center_to_obj[1], vec_host_center_to_obj[0]);
      const float32_t cos_angle_host_center_to_obj = F360_Cosf(angle_host_center_to_obj);
      const float32_t sin_angle_host_center_to_obj = F360_Sinf(angle_host_center_to_obj);
      const float32_t range_host_center_to_obj_sq = vec_host_center_to_obj[0] * vec_host_center_to_obj[0] + vec_host_center_to_obj[1] * vec_host_center_to_obj[1];
      const float32_t range_sq_for_cross_range_variance_coefficient = 0.0012F;
      const float32_t cross_range_variance = std::fmaxf(range_host_center_to_obj_sq * range_sq_for_cross_range_variance_coefficient, 1.0F);
      const float32_t range_variance = 1.0F;
      const float32_t range_cross_range_pos_var_mat[2][2] = { {range_variance, 0.0F}, {0.0F, cross_range_variance} };
      Rotate_2D_Covariance_Matrix(cos_angle_host_center_to_obj, sin_angle_host_center_to_obj, range_cross_range_pos_var_mat, cov_mat);

      obj.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X] = cov_mat[F360_2D_IDX_X][F360_2D_IDX_X];
      obj.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y] = cov_mat[F360_2D_IDX_X][F360_2D_IDX_Y];
      obj.errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_X] = cov_mat[F360_2D_IDX_Y][F360_2D_IDX_X];
      obj.errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y] = cov_mat[F360_2D_IDX_Y][F360_2D_IDX_Y];

      if (obj.movable_prob > 0.5F)
      {
         // Compute velocity covariance
         const float32_t radial_vel_errcov = 2.0F;
         const float32_t cross_radial_vel_errcov_at_low_speed = 2.0F;
         const float32_t max_cross_radial_vel_errcov = 36.0F;
         const float32_t speed_mult_factor = 0.09F;
         const float32_t cross_radial_vel_errcov = cross_radial_vel_errcov_at_low_speed + fminf(max_cross_radial_vel_errcov, obj.speed * obj.speed * speed_mult_factor);

         const float32_t vel_errcov_mat[2][2] = { {radial_vel_errcov, 0.0F}, {0.0F, cross_radial_vel_errcov} };
         Rotate_2D_Covariance_Matrix(cos_angle_host_center_to_obj, sin_angle_host_center_to_obj, vel_errcov_mat, cov_mat);

         obj.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = cov_mat[F360_2D_IDX_X][F360_2D_IDX_X];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = cov_mat[F360_2D_IDX_X][F360_2D_IDX_Y];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = cov_mat[F360_2D_IDX_Y][F360_2D_IDX_X];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = cov_mat[F360_2D_IDX_Y][F360_2D_IDX_Y];

         // Compute acceleration covariance
         const float32_t radial_acc_errcov = 1.0F;
         const float32_t cross_radial_acc_errcov = 1.0F;

         const float32_t accel_errcov_mat[2][2] = { {radial_acc_errcov, 0.0F}, {0.0F, cross_radial_acc_errcov} };
         Rotate_2D_Covariance_Matrix(cos_angle_host_center_to_obj, sin_angle_host_center_to_obj, accel_errcov_mat, cov_mat);

         obj.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] = cov_mat[F360_2D_IDX_X][F360_2D_IDX_X];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] = cov_mat[F360_2D_IDX_X][F360_2D_IDX_Y];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AX] = cov_mat[F360_2D_IDX_Y][F360_2D_IDX_X];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] = cov_mat[F360_2D_IDX_Y][F360_2D_IDX_Y];
      }
      else
      {
         // Set velocity and acceleration covariance for non moveable objects
         constexpr float32_t vel_diagonal_errcov_mat_element = 0.27F;
         constexpr float32_t stat_obj_vel_errcov_mat[2][2] = { {vel_diagonal_errcov_mat_element, 0.0F}, {0.0F, vel_diagonal_errcov_mat_element} };

         obj.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = stat_obj_vel_errcov_mat[F360_2D_IDX_X][F360_2D_IDX_X];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = stat_obj_vel_errcov_mat[F360_2D_IDX_X][F360_2D_IDX_Y];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = stat_obj_vel_errcov_mat[F360_2D_IDX_Y][F360_2D_IDX_X];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = stat_obj_vel_errcov_mat[F360_2D_IDX_Y][F360_2D_IDX_Y];


         // Compute acceleration covariance
         constexpr float32_t accel_diagonal_errcov_mat_element = 0.1F;
         constexpr float32_t stat_obj_accel_errcov_mat[2][2] = { {accel_diagonal_errcov_mat_element, 0.0F}, {0.0F, accel_diagonal_errcov_mat_element} };

         obj.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] = stat_obj_accel_errcov_mat[F360_2D_IDX_X][F360_2D_IDX_X];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] = stat_obj_accel_errcov_mat[F360_2D_IDX_X][F360_2D_IDX_Y];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AX] = stat_obj_accel_errcov_mat[F360_2D_IDX_Y][F360_2D_IDX_X];
         obj.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] = stat_obj_accel_errcov_mat[F360_2D_IDX_Y][F360_2D_IDX_Y];
      }

      // Initialize covariance matrix for pointing filter
      obj.cca_pnt_filter_cov[0][0] = 1.0F;
      obj.cca_pnt_filter_cov[0][1] = 0.0F;
      obj.cca_pnt_filter_cov[1][0] = 0.0F;
      obj.cca_pnt_filter_cov[1][1] = 0.01F;

      // Initialize uncertainties for size filter
      obj.length_uncertainty = 0.2F;
      obj.width_uncertainty = 0.2F;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Refpoint_And_Size()
   *===========================================================================
   * RETURN VALUE:
   * NONE
   *
   * PARAMETERS:
   * const F360_Host_T& host
   * const F360_Calibrations_T& calibrations
   * const F360_Globals_T& globals
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Cluster_T& cluster
   * F360_Object_Track_T& obj
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
   * This function determines the initial object size and reference point of a new object given a
   * rough initial object position estimate and the object speed.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Determine_Refpoint_And_Size(const F360_Host_T& host,
      const F360_Calibrations_T& calibrations,
      const F360_Globals_T& globals,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj)
   {
      bool f_need_to_update_position;
      if (obj.movable_prob > calibrations.k_init_movable_prob_threshold)
      {
         float32_t length;
         float32_t width;
         const float32_t x_pos_threshold = 20.0F;
         const float32_t y_pos_threshold = 25.0F;
         const uint32_t ndets_threshold = 7U;

         if (obj.speed < calibrations.fast_moving_thresh)
         {
            constexpr float32_t slow_moving_obj_bbox_length = 1.0F;
            constexpr float32_t slow_moving_obj_bbox_width = 1.0F;
            length = slow_moving_obj_bbox_length;
            width = slow_moving_obj_bbox_width;
            float32_t calculated_length = 0.0F;

            if ((std::abs(obj.vcs_position.x) < x_pos_threshold) && (std::abs(obj.vcs_position.y) < y_pos_threshold) && (obj.ndets > ndets_threshold) &&
               (Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj)))
            {
                calculated_length = Compute_Length_Line(det_props, obj);
            }
            length = F360_Saturate(calculated_length, length, 2.5F);
         }
         else if (obj.speed < calibrations.k_init_fast_moving_upper_threshold)
         {
            constexpr float32_t fast_moving_obj_bbox_length = 1.75F;
            constexpr float32_t fast_moving_obj_bbox_width = 1.0F;
            length = fast_moving_obj_bbox_length;
            width = fast_moving_obj_bbox_width;
            float32_t calculated_length = 0.0F;

            if ((std::abs(obj.vcs_position.x) < x_pos_threshold) && (std::abs(obj.vcs_position.y) < y_pos_threshold) && (obj.ndets > ndets_threshold) &&
               (Is_Pca_Principal_Dir_Close_To_Heading(det_props, obj)))
            {
                calculated_length = Compute_Length_Line(det_props, obj);
            }
            length = F360_Saturate(calculated_length, length, 3.0F);
         }
         else
         {
            constexpr float32_t very_fast_moving_obj_bbox_length = 4.0F;
            constexpr float32_t very_fast_moving_obj_bbox_width = 1.5F;
            length = very_fast_moving_obj_bbox_length;
            width = very_fast_moving_obj_bbox_width;
         }

         obj.reference_point = F360_REFERENCE_POINT_CENTER;
         obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
         obj.bbox.Set_Center(obj.vcs_position);


         obj.bbox.Set_Length(length);
         obj.bbox.Set_Width(width);
         obj.Set_Bbox_Orientation(obj.vcs_heading);
         Update_Object_Reference_Point(host.dist_rear_axle_to_vcs_m, false, false, false, calibrations, sensors, globals, obj);
         f_need_to_update_position = true;
         obj.bbox_height = calibrations.k_obstacle_prob_min_bbox_height_moving;
      }
      else
      {
         // For nonmovable objects we use center as reference point and default dimension.
         obj.reference_point = F360_REFERENCE_POINT_CENTER;
         obj.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
         obj.bbox.Set_Center(obj.vcs_position);
         obj.bbox.Set_Length(calibrations.k_nonmoveable_target_diameter);
         obj.bbox.Set_Width(calibrations.k_nonmoveable_target_diameter);
         obj.Set_Bbox_Orientation(obj.vcs_heading);
         obj.bbox_height = calibrations.k_obstacle_prob_min_bbox_height_nonmovable;

         f_need_to_update_position = false;
      }
      return f_need_to_update_position;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Length_Line()
   *===========================================================================
   * RETURN VALUE:
   * float32_t - the computed length of the line
   *
   * PARAMETERS:
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Object_Track_T& obj
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
   * This function computes the length of the line formed by the detections by
   * projecting them onto the heading direction of the object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Compute_Length_Line(
       const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
       const F360_Object_Track_T& obj)
   {

      // Compute span along slope direction
      const float32_t dir_cos = obj.vcs_heading.Cos(); 
      const float32_t dir_sin = obj.vcs_heading.Sin();
      float32_t min_proj = 0.0F;
      float32_t max_proj = 0.0F;
      bool initialized = false;
      for (uint16_t i = 0U; i < obj.ndets; i++)
      {
          const uint32_t det_idx = obj.detids[i] - 1U;
          const float32_t proj = det_props[det_idx].vcs_position.x * dir_cos + det_props[det_idx].vcs_position.y * dir_sin;
          if (!initialized)
          {
              min_proj = proj;
              max_proj = proj;
              initialized = true;
          }
          else
          {
              min_proj = std::fminf(min_proj, proj);
              max_proj = std::fmaxf(max_proj, proj);
          }
      }
      const float32_t span_out = initialized ? max_proj - min_proj : 0.0F;
      return span_out;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Pca_Principal_Dir_Close_To_Heading()
   *===========================================================================
   * RETURN VALUE:
   * bool - true if evident direction is found and it is close to heading
   *
   * PARAMETERS:
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const F360_Object_Track_T& obj
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
   * This function checks if the principal direction from PCA of the detections is close to the object's heading.
   * PCA (Principal Component Analysis) finds the dominant variance direction once we have enough detections (> 3).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
      bool Is_Pca_Principal_Dir_Close_To_Heading(
         const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
         const F360_Object_Track_T& obj)
      {
         float32_t mean_x = 0.0F;
         float32_t mean_y = 0.0F;

         for (uint16_t i = 0U; i < obj.ndets; i++)
         {
            const uint32_t det_idx = obj.detids[i] - 1U;

            mean_x += det_props[det_idx].vcs_position.x;
            mean_y += det_props[det_idx].vcs_position.y;
         }
         

         bool reliable_direction = false;
         float32_t wrapped_angle_diff = 0.0F; // just a value
         constexpr float32_t k_max_angle_diff_rad = F360_DEG2RAD(25.0F);
         
         // We first check the number of dets before we call the function
         // so we set count to 1  if its zero just for the MISRA warning
         if (obj.ndets >= 3U)
         {
            const float32_t inv_count = 1.0F / static_cast<float32_t>(obj.ndets);
            mean_x *= inv_count;
            mean_y *= inv_count;

            float32_t cov_xx = 0.0F;
            float32_t cov_xy = 0.0F;
            float32_t cov_yy = 0.0F;

            for (uint16_t i = 0U; i < obj.ndets; i++)
            {
                const uint32_t det_idx = obj.detids[i] - 1U;

                const float32_t dx = det_props[det_idx].vcs_position.x - mean_x;
                const float32_t dy = det_props[det_idx].vcs_position.y - mean_y;
                cov_xx += dx * dx;
                cov_xy += dx * dy;
                cov_yy += dy * dy;

            }

            cov_xx *= inv_count;
            cov_xy *= inv_count;
            cov_yy *= inv_count;

            // Degenerate covariance -> no reliable direction
            const float32_t trace = cov_xx + cov_yy;
            reliable_direction = (trace > 1e-4F);

            // Principal eigenvector angle for 2x2 symmetric matrix [[a, b], [b, c]]
            const float32_t angle_based_on_dets_spread = 0.5F * F360_Atan2f(2.0F * cov_xy, (cov_xx - cov_yy));
            const Angle angle_diff = (Angle{ angle_based_on_dets_spread } - obj.vcs_heading).Normalize();
            const float32_t abs_angle_diff = std::fabs(angle_diff.Value());
            wrapped_angle_diff = (abs_angle_diff > F360_PI * 0.5F) ? (F360_PI - abs_angle_diff) : abs_angle_diff;
         }
         
         return (reliable_direction && (std::fabs(wrapped_angle_diff) <= k_max_angle_diff_rad) );
      }

   /*===========================================================================*\
   * FUNCTION: Determine_Track_Position()
   *===========================================================================
   * RETURN VALUE:
   * NONE
   *
   * PARAMETERS:
   * const F360_Cluster_T& cluster,
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Object_Track_T& obj
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
   * This function computes an improved object initial position estimate given the
   * position of the latest detections in the corresponding cluster and an initial
   * estimate of the object reference point.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Determine_Track_Position(
      const F360_Cluster_T& cluster,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj)
   {
      const float32_t cos_hdg = obj.vcs_heading.Cos();
      const float32_t sin_hdg = obj.vcs_heading.Sin();
      float32_t para_max = -1000.0F;
      float32_t para_min = 1000.0F;
      float32_t orth_max = -1000.0F;
      float32_t orth_min = 1000.0F;

      for (int32_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         const F360_Detection_Props_T& det = det_props[det_idx];

         const float32_t det_para = cos_hdg * det.vcs_position.x + sin_hdg * det.vcs_position.y;
         const float32_t det_orth = -sin_hdg * det.vcs_position.x + cos_hdg * det.vcs_position.y;

         para_max = std::fmaxf(para_max, det_para);
         para_min = std::fminf(para_min, det_para);
         orth_max = std::fmaxf(orth_max, det_orth);
         orth_min = std::fminf(orth_min, det_orth);
      }

      float32_t refpos_para;
      float32_t refpos_orth;
      switch (obj.reference_point)
      {
         case F360_REFERENCE_POINT_FRONT_LEFT:
            refpos_para = para_max;
            refpos_orth = orth_min;
            break;
         case F360_REFERENCE_POINT_FRONT:
            refpos_para = para_max;
            refpos_orth = (orth_min + orth_max) * 0.5F;
            break;
         case F360_REFERENCE_POINT_FRONT_RIGHT:
            refpos_para = para_max;
            refpos_orth = orth_max;
            break;
         case F360_REFERENCE_POINT_RIGHT:
            refpos_para = (para_min + para_max) * 0.5F;
            refpos_orth = orth_max;
            break;
         case F360_REFERENCE_POINT_REAR_RIGHT:
            refpos_para = para_min;
            refpos_orth = orth_max;
            break;
         case F360_REFERENCE_POINT_REAR:
            refpos_para = para_min;
            refpos_orth = (orth_min + orth_max) * 0.5F;
            break;
         case F360_REFERENCE_POINT_REAR_LEFT:
            refpos_para = para_min;
            refpos_orth = orth_min;
            break;
         case F360_REFERENCE_POINT_LEFT:
            refpos_para = (para_min + para_max) * 0.5F;
            refpos_orth = orth_min;
            break;
         default:
            refpos_para = (para_min + para_max) * 0.5F;
            refpos_orth = (orth_min + orth_max) * 0.5F;
            break;
      }

      const float32_t refpos_long = cos_hdg * refpos_para - sin_hdg * refpos_orth;
      const float32_t refpos_lat = sin_hdg * refpos_para + cos_hdg * refpos_orth;

      obj.vcs_position.x = refpos_long;
      obj.vcs_position.y = refpos_lat;
      obj.Update_Bbox_Center();
   }
}
