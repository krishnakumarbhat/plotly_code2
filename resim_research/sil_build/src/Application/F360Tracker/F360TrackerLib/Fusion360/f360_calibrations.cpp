/*===================================================================================\
 * FILE: f360_calibrations.cpp
 *====================================================================================
 * Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains function definition for Initialize_Tracker_Calibrations.
 *
 * Applicable Standards (in order of precedence: highest first):
 *   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *   ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*===================================================================================*/

#include <cstring>
#include "f360_calibrations.h"
#include "f360_math.h"
#include <algorithm>
#include "f360_iterator.h"
#include <cassert>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: F360_Tracker::Initialize_Tracker_Calibrations()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Calibrations_T &calibrations
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
   * This function initializes default values for calibrations
   *
   * PRECONDITIONS:
   * To be called by an object/reference of F360_Tracker.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Initialize_Tracker_Calibrations(F360_Calibrations_T &calibrations)
   {
      calibrations.low_confidence_level_thresh = 0.3F;
      calibrations.host_vicinity_vcs_x_range = 10.0F;
      calibrations.host_vicinity_vcs_y_range = 9.0F;
      calibrations.fast_moving_thresh = 3.0F;

      calibrations.hide_tracks_outside_guardrail = false;

      // CTCA Time update
      calibrations.k_max_allowed_curv_variance = 0.04F;
      calibrations.k_min_acc_for_increasing_acc_noise = 0.5F;
      calibrations.k_max_acc_for_increasing_acc_noise = 2.0F;
      calibrations.k_max_acc_scale_factor = 20.0F;
      calibrations.k_normal_noise_acc = 2.5F;
      calibrations.k_normal_noise_speed = 0.0F;
      calibrations.k_normal_noise_pos_para = 0.2F;
      calibrations.k_q_tuning_orth_direction_speed_breakpoint = 7.0F;
      calibrations.k_normal_noise_curv = 10.0F;
      calibrations.k_normal_noise_hdg = 0.0F;
      calibrations.k_normal_noise_pos_orth = 0.05F;

      // CTCA measurement update
      calibrations.k_ctca_msmnt_update_max_reverse_abs_spd = 3.0F;

      // CCA measurement update
      calibrations.k_cca_msmnt_update_vel_var = 1.0F;
      calibrations.k_speed_th_for_saturating_r = 0.0001F;
      calibrations.k_max_innov_dist_thres = 0.3F;
      calibrations.k_max_innov_dist_yaw_rate_gain = -0.12F / F360_DEG2RAD(15.0F);
      calibrations.k_min_decrease_factor = 0.0F;

      // CTCA time and measurement update for preventing overshooting of speed and tangential acceleration estimates at hard breaking
      calibrations.k_abs_acc_threshold_for_breaking = 0.5F;
      calibrations.k_abs_speed_threshold_for_stopping = 0.5F;

      // CTCA and CCA measurement update for preventing overshooting of heading and orietnation when the object is occluded in highway scenario
      calibrations.k_speed_threshold_for_kmat_decrease = 15.0F;
      calibrations.k_x_pos_threshold_for_kmat_decrease = 50.0F;
      calibrations.k_heading_threshold_for_kmat_decrease = 0.2F;
      calibrations.k_kmat_decrease_factor_for_occluded = 0.1F;

      // Track validity
      calibrations.k_tv_dets_exp_filter_const = 0.95F; //F360_Expf(-T/tau) where T is a fixed time step between samples and tau is desired time constant

      calibrations.k_tv_refl_gr_trk_max_diff_x = 5.5F;
      calibrations.k_tv_refl_gr_trk_max_diff_y = 5.5F;
      calibrations.k_tv_refl_gr_trk_max_diff_rel_vel = 0.2F;
      calibrations.k_tv_refl_gr_trk_max_diff_heading = F360_DEG2RAD(20.0F);
      calibrations.k_tv_refl_gr_trk_min_bbox_lat_margin = 1.0F;
      calibrations.k_tv_refl_gr_trk_bbox_lat_margin = 2.0F;
      calibrations.k_tv_refl_gr_trk_straight_mov_head_th = F360_DEG2RAD(30.0F);
      calibrations.k_tv_refl_gr_trk_min_sep_lon_pos = -80.0F;
      calibrations.k_tv_refl_gr_trk_max_sep_lon_pos = 80.0F;

      calibrations.host_vehicle_width = 2.0F;
      calibrations.host_vehicle_length = 4.0F;
      calibrations.k_vp_vehicle_next_to_ego_max_lat_dist = 5.0F;
      calibrations.k_vp_vehicle_next_to_ego_max_long_dist = 5.0F;
      calibrations.k_vp_vehicle_next_to_ego_max_abs_heading = 0.785398163397448F;
      calibrations.k_vp_vehicle_next_to_ego_long_pos_offset = 0.5F;
      calibrations.k_min_speed_for_updating_heading = 2.0F;
      calibrations.rdot_interval_compatability_dealiasing_gate = 3.0F;

      calibrations.k_conf_overlapping_reduction_factor = 0.9F; // Confidence reduction factor for objects that are overlapping
      calibrations.k_hyst_time_for_coasted_objects = 0.055F; // Hysteresis for when newly COASTED objects are treated similarly to UPDATED objects

      // Cluster grouping parameters
      calibrations.k_moving_clusters_dist_coarse_gate = 8.0F;
      calibrations.k_stat_clusters_dist_sq_coarse_gate = 4.0F;
      calibrations.k_moving_clusters_dist_sq_coarse_gate_1 = 90.0F;
      calibrations.k_moving_clusters_dist_sq_coarse_gate_2 = 156.0F;
      calibrations.k_moving_clusters_time_diff_coarse_gate = 0.1F;
      calibrations.k_max_dealiased_range_rate_diff = 3.0F;
      calibrations.k_rdot_half_gate = 3.0F;
      calibrations.k_radial_gate = 1.5F;
      calibrations.k_default_max_cross_radial_speed = 10.0F;
      calibrations.k_increased_max_cross_radial_speed = 40.0F;
      calibrations.k_range_thr_for_full_az_dependent_adj = 4.0F;
      calibrations.k_range_thr_for_az_dependent_adj = 8.0F;
      calibrations.k_slow_moving_speed_thr_for_cross_radial_gate = 4.0F;
      calibrations.k_fast_moving_speed_thr_for_cross_radial_gate = 8.0F;
      calibrations.k_slow_moving_size_for_cross_radial_gate = 1.5F;
      calibrations.k_fast_moving_size_for_cross_radial_gate = 3.0F;
      calibrations.k_az_scaling_factor_long_range = F360_DEG2RAD(2.0F);
      calibrations.k_az_scaling_factor_close_range = 0.13F;
      calibrations.k_cross_radial_movement_default_saturation = 2.0F;
      calibrations.k_cross_radial_movement_increased_saturation = 4.0F;
      calibrations.k_hard_range_thr_for_close_range_logic = calibrations.k_slow_moving_size_for_cross_radial_gate / (calibrations.k_az_scaling_factor_close_range - calibrations.k_az_scaling_factor_long_range);
      calibrations.k_hard_range_thr_for_long_range_logic = calibrations.k_fast_moving_size_for_cross_radial_gate / (calibrations.k_az_scaling_factor_close_range - calibrations.k_az_scaling_factor_long_range);
      assert(calibrations.k_default_max_cross_radial_speed < calibrations.k_increased_max_cross_radial_speed);
      assert(calibrations.k_range_thr_for_full_az_dependent_adj < calibrations.k_range_thr_for_az_dependent_adj);
      assert(calibrations.k_slow_moving_speed_thr_for_cross_radial_gate < calibrations.k_fast_moving_speed_thr_for_cross_radial_gate);
      assert(calibrations.k_slow_moving_size_for_cross_radial_gate < calibrations.k_fast_moving_size_for_cross_radial_gate);
      assert(calibrations.k_az_scaling_factor_long_range < calibrations.k_az_scaling_factor_close_range);
      assert(calibrations.k_cross_radial_movement_default_saturation < calibrations.k_cross_radial_movement_increased_saturation);


      //Determine reflected object
      calibrations.k_mirror_prob_threshold = 0.30F;
      calibrations.k_reflective_guardrail_track_min_host_speed = 1.0F;
      calibrations.k_reflected_object_max_mirror_probability = 1.0F;

      // Measurement update covariance parameters
      calibrations.k_ref_msmt_cov_cca = 0.9F;
      calibrations.k_ref_msmt_cov_ctca = 0.5F;
      calibrations.k_acc_vel_state_decay_factor = 0.00F;
      calibrations.k_min_num_selected_dets_per_sensor_for_binning = 5U;

      // Reference point paramters - F360Version
      calibrations.k_normal_obj_ref_pnt_hysteresis_factor = 1.01F;
      calibrations.k_normal_obj_length_thr = 6.5F;
      calibrations.k_long_obj_ref_pnt_hysteresis_factor = 1.005F;
      calibrations.k_long_obj_length_thr = 11.5F;
      calibrations.k_sin_max_pointing_error_sq = 0.001217974870088F; // Corresponds to sin(2deg)^2
      calibrations.k_frac_of_moved_dist = 0.1F;
      calibrations.k_far_away_object_dist_sq_thr = 6400.0F;// Corresponds to 80m * 80m

      // Determine to freeze track
      calibrations.min_object_age_for_object_close_to_stat_host = 0.5F;

      // Guardrail based angel jump detector
      calibrations.k_angle_jump_range_tolerance = 1.0F;
      calibrations.k_angle_jump_max_abs_range_rate = 2.0F;
      calibrations.k_angle_jump_min_abs_azimuth_vcs = F360_DEG2RAD(40.0F);
      calibrations.k_angle_jump_max_abs_azimuth_vcs = F360_DEG2RAD(140.0F);
      calibrations.k_angle_jump_max_range = 15.0F;
      calibrations.k_angle_jump_long_search_margin = 5.0F;

      //Mark stationary bounced detections
      calibrations.k_stat_bounce_min_host_speed = 10.0F;
      calibrations.k_stat_bounce_max_trk_long_posn = -15.0F;
      calibrations.k_stat_bounce_max_trk_lat_dist = 6.0F;
      calibrations.k_stat_bounce_max_trk_heading = F360_DEG2RAD(20.0F);
      calibrations.k_stat_bounce_azimuth_border_ext = F360_DEG2RAD(2.0F);
      calibrations.k_stat_bounce_range_rate_diff_thr = 1.0F;
      calibrations.k_stat_bounce_min_det_long_posn = -110.0F;

      // Filter out low quality detections on or behind guardrail
      calibrations.k_azimuth_conf_low_quality_detection_filter = 3;
      calibrations.k_max_rcs_thr_low_quality_detection_filter = -10.0F;

      // Parameters for Detect_Wheel_Spin_Pairs
      calibrations.k_max_wheel_spin_dist_sq = (0.2F);
      calibrations.k_min_wheel_spin_doppler_spread = (3.0F);
      calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair = (80.0F);
      calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair = (20.0F);
      calibrations.k_max_azimuth_difference_for_wheelspin_pair = (0.06F);
      calibrations.k_wheelspin_pair_max_close_det_iterations = 3U;
      calibrations.k_max_wheel_spin_dets_to_mark = 10U;

      // Parameters for Mark_Detections_Wheel_Spin_From_Objects
      calibrations.k_max_abs_vcs_long_posn_for_wheelspin = 80.0F;
      calibrations.k_min_speed_fast_moving = 3.0F;
      calibrations.k_min_rr_diff_wheelspin = 2.0F;
      calibrations.k_ws_lat_buffer_zone_oncoming = 0.40F;
      calibrations.k_ws_lat_buffer_zone = 0.3F;
      calibrations.k_ws_long_buffer_zone = 0.7F;

      // Parameters for Detect_NearBy_Wheel_Spins
      calibrations.k_nbws_max_lat_pos = 5.0F;
      calibrations.k_nbws_min_lat_pos = -5.0F;
      calibrations.k_nbws_max_long_pos = 14.0F;
      calibrations.k_nbws_min_long_pos = -14.0F;
      calibrations.k_nbws_lat_marking_th = 0.2F;
      calibrations.k_nbws_long_marking_th = 0.5F;
      calibrations.k_nbws_lat_asc_th = 0.3F;
      calibrations.k_nbws_long_asc_th = 0.3F;
      calibrations.k_nbws_max_num_clusters = 50U;

      // Parameters for Mark_Dets_As_Close_Target_And_Farside
      calibrations.k_ct_and_fcm_max_abs_heading_diff_to_host = F360_DEG2RAD(30.0F);
      calibrations.k_ct_and_fcm_max_dist_for_close_target_sq = 36.0F;
      calibrations.k_ct_orth_buffer_zone_factor = 2.0F;
      calibrations.k_ct_para_buffer_zone_factor = 0.5F;

      // Parameters for Cond_Deassoc_Low_RR_Dets
      calibrations.k_cond_deassoc_min_obj_spd_for_deassoc = 2.0F;
      calibrations.k_cond_deassoc_det_comp_rr_max = 0.2F;
      calibrations.k_cond_deassoc_fraction_of_width_to_deassoc = 0.8F;

      // Existence Probability
      calibrations.k_ep_init_p_det_sensor = 1.0F;
      calibrations.k_ep_p_measurement_with_no_new_meas = 0.01F;
      calibrations.k_ep_clutter_prob_with_meas = 0.05F;
      calibrations.k_ep_clutter_prob_with_no_meas = 0.05F;
      calibrations.k_ep_min_allowed_exist_prob = 0.01F;
      calibrations.k_ep_prob_track_state_exp_scale = 0.4F;
      calibrations.k_ep_prob_track_state_exp_offset = 0.1F;
      calibrations.k_ep_bottom_saturation_of_normalized_variance = 0.2F * 0.2F;
      calibrations.k_ep_variance_th_pos_xy = 2.0F * 2.0F;
      calibrations.k_ep_variance_th_heading = 0.2F * 0.2F;
      calibrations.k_ep_variance_th_velocity = 1.5F * 1.5F;
      calibrations.k_ep_variance_th_curvature = 0.04F * 0.04F;
      calibrations.k_ep_variance_th_accel = 1.0F * 1.0F;
      calibrations.k_ep_variance_th_tan_accel = 1.5F * 1.5F;
      calibrations.k_ep_prob_track_state_init_value = 0.80F;

      //Post Update track Adjustments
      calibrations.k_puta_max_vcs_xposn_for_ghost_NU_2_C = 50.0F;
      calibrations.k_puta_max_vcs_yposn_for_ghost_NU_2_C = 30.0F;
      calibrations.k_puta_overlapping_tracks_max_speed_diff = 3.0F;
      calibrations.k_puta_overlapping_tracks_max_heading_diff = F360_DEG2RAD(30.0F);
      calibrations.k_puta_overlapping_tracks_high_conf_thr = 0.8F;
      calibrations.k_puta_overlapping_tracks_low_conf_thr = 0.5F;
      calibrations.k_puta_overlapping_tracks_long_thr = 25.0F;

      calibrations.k_puta_obj_size_acc_filt_coef_innov_coasting_obj = 0.9F;
      calibrations.k_puta_obj_size_acc_filt_coef_innov_updated_obj = 0.9F;
      calibrations.k_puta_obj_size_acc_innov_no_update_length = 6.0F;
      calibrations.k_puta_obj_size_acc_innov_no_update_width = 2.0F;

      calibrations.k_puta_min_object_confidence = 0.5F;
      calibrations.k_puta_min_object_time = 0.4F;
      calibrations.k_puta_large_distance = 1e10F;
      calibrations.k_puta_orientation_diff_threshold = F360_DEG2RAD(90.0F);

      // Track Downselection
      calibrations.k_track_downselect_dets_threshold = 1.25F;
      calibrations.k_track_downselect_dets_threshold_low = 0.5F;
      calibrations.k_track_downselect_average_dets_thresh = 1.0F;
      calibrations.k_track_downselect_min_time_filter_dets_thresh = 2.5F;
      calibrations.k_track_downselect_confidence_level_lowering_factor = 0.9F;
      calibrations.k_track_downselect_confidence_thresh = 0.4F;

      // Track grouping
      calibrations.k_track_grouping_hdg_gate = F360_DEG2RAD(30.0F);
      calibrations.k_track_grouping_speed_gate = 1.0F;
      calibrations.k_track_grouping_curvature_gate = 0.02F;
      calibrations.k_merging_coarse_score_gate = 1.5F;
      calibrations.merging_m2m_distance_threshold = 2.2F;
      calibrations.merging_lateral_det_spread_threshold = 4.0F;
      calibrations.merging_m2m_max_obj_speed = F360_KPH2MPS(25.0F);
      calibrations.k_orth_split_width_threshold = 1.8F;

      //Calculate Time To Collision (ttc)
      calibrations.k_calc_ttc_min_thresh_projected_velocity = 1.0F;
      calibrations.k_calc_ttc_max_thresh_projected_velocity = 1e+6F;

      // radar phenomena
      calibrations.rp_max_object_lateral_distance = 13.0F;
      calibrations.rp_max_abs_pointing_disagreement = F360_DEG2RAD(10.0F);
      calibrations.rp_object_max_longitudinal_margin = 2.0F;
      calibrations.rp_min_confidence_level = 0.33F;

      // object based angle jump detector
      calibrations.obj_aj_border_half_width = 0.25F;
      calibrations.obj_aj_max_double_range_hypothesis = 10.0F;

      // object based multibounce detector
      calibrations.mb_max_num_bounces = 4U;
      calibrations.mb_range_rate_diff_th = 1.0F;
      calibrations.mb_max_det_range = 15.0F;
      calibrations.mb_restricted_area_width = 0.5F;

      // Association: Parameters for detection to track association
      calibrations.k_nonmoveable_target_diameter = 0.5F;
      calibrations.k_min_assoc_gate_extension_non_moveable = 0.1F;
      calibrations.k_max_assoc_gate_extension_non_moveable = 1.75F;
      calibrations.k_max_assoc_gate_radius_non_moveable = 2.0F;
      calibrations.k_obj_dist_for_min_assoc_gate_extension_non_moveable = 0.0F;
      calibrations.k_obj_dist_for_max_assoc_gate_extension_non_moveable = 50.0F;
      calibrations.k_spd_dependent_assoc_gate_extension_factor_non_moveable = 0.5F;
      calibrations.k_min_speed_for_increasing_occluded_long_assoc_buffer = 5.55F; // about 20kph
      calibrations.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase = 6.2F; // about 20kph
      calibrations.k_second_min_speed_for_increasing_occluded_long_assoc_buffer = 10.0F; // about 36kph
      calibrations.k_second_max_speed_for_saturating_occluded_long_assoc_buffer_increase = 24.0F; // about 86kph
      calibrations.k_max_occluded_long_buffer_increase = 1.1F;
      calibrations.k_min_occluded_long_buffer_increase = 0.2F;
      calibrations.k_min_assoc_gate_long_buffer_moveable_objs = 0.4F;
      calibrations.k_min_assoc_gate_lat_buffer_moveable_objs = 0.4F;
      calibrations.k_range_rate_score_threshold = 2.0F;
      calibrations.k_range_buffer_max_dist = 100.0F;
      calibrations.k_range_buffer_min_val = 0.0F;
      calibrations.k_range_buffer_max_val = 1.0F;
      calibrations.k_az_buffer_max_dist = 60.0F;
      calibrations.k_az_buffer_min_val = 0.0F;
      calibrations.k_az_buffer_max_val = 3.33F;
      calibrations.k_min_rr_diff_from_stationary_hypothesis = 0.2F;

      // Association: Parameters for calculating association score, detection inside bounding box
      calibrations.k_para_diff_weight_inside_box = 0.15F;
      calibrations.k_orth_diff_weight_inside_box = 0.15F;
      calibrations.k_rdot_diff_weight_inside_box = 0.7F;
      calibrations.k_dist_weight_inside_solid_circle = 0.3F;
      calibrations.k_rdot_diff_weight_inside_solid_circel = 0.7F;
      calibrations.k_base_score_bbox_center = 0.75F;

      // Association: Parameters for calculating association score, detection inside extended bounding box
      calibrations.k_para_diff_weight_inside_ext_box = 0.35F;
      calibrations.k_orth_diff_weight_inside_ext_box = 0.35F;
      calibrations.k_rdot_diff_weight_inside_ext_box = 0.3F;
      calibrations.k_dist_weight_inside_ext_circle = 0.7F;
      calibrations.k_rdot_diff_weight_inside_ext_circel = 0.3F;

      // Association: Score for detections that match in range rate but is outside extended bounding box
      calibrations.k_score_outside_ext_bbox = 5.0F;

      // Association: Parameters for calculating range rate threshold
      calibrations.k_vcs_distance_sqr_thr = 1600.0F; // 40 * 40 meters
      calibrations.k_rr_thr_factor_far_away_coasted = 2.0F;
      calibrations.k_rr_thr_factor_fov_edge = 0.2F;
      calibrations.k_speed_threshold = F360_KPH2MPS(25.0F);

      // Association: detection inlier selection
      calibrations.k_rr_error_statistics_forgetting_factor = 0.83F;
      calibrations.k_max_number_of_historic_dets_obj_non_movable = 8.0F;
      calibrations.k_max_number_of_historic_dets_obj_movable = 4.0F;
      calibrations.k_max_historic_rr_error_variance = 2.0F;
      calibrations.k_min_range_rate_error_threshold = 0.25F;

      // Object tracks properties
      calibrations.k_underdrive_min_trk_long_posn = -5.0F;

      calibrations.k_underdrive_min_zone_long_posn = 0.0F;
      calibrations.k_underdrive_lat_buffer_factor = 1.4F;

      // Underdrivability for moving objects
      calibrations.ud_mov_height_threshold = 6.5F;
      calibrations.ud_mov_cnt_consecutive_scans = 10U;
      calibrations.ud_mov_posx_min_limit = 0.0F;
      calibrations.ud_mov_posx_max_limit = 100.0F;
      calibrations.ud_mov_prob_can_pass_under = 1.0F;
      calibrations.ud_mov_prob_can_not_pass_under = 0.0F;
      calibrations.ud_mov_prob_not_to_consider = 0.0F;

      // Occupancy Grid related caliberations
      calibrations.k_ocg_underdrive_small_curvature_th = 1e-4F;
      calibrations.f_ocg_use_curvilinear_simplification = true;

      // Pseudo position estimation
      calibrations.k_pseudo_pos_dist_diff_gain = 5.0F;
      calibrations.k_pseudo_pos_dist_diff_thr = 0.9F;
      calibrations.k_pseudo_pos_max_variance_threshold = 30.0F;
      calibrations.k_max_num_consistent_outliers_orth = 4;

      // Uncertainty bias for pseudo position measurement
      calibrations.k_pseudo_pos_cov_matrix_bias = 1.0F;
      calibrations.k_pseudo_pos_cov_matrix_bias_non_movable = 1.0F * 1.0F;

      calibrations.k_time_since_init_th_to_enable_outlier_mitigation_cca = 0.75F;

      // Paramerters for pseudo position in FOVE cases
      calibrations.k_pseudo_pos_high_uncertainity = 10000.0F;
      calibrations.k_fov_normal_rotation_angle = F360_DEG2RAD(21.5F);

      // Polar uncertainty
      calibrations.k_range_var = 0.5F * 0.5F;
      calibrations.k_az_var = F360_DEG2RAD(2.0F) * F360_DEG2RAD(2.0F);
      calibrations.k_raw_pseudo_pos_cov_max_saturation_distance = 100.0F * 100.0F;

      // Water spray detectors
      calibrations.k_ws_min_speed = 5.0F;
      calibrations.k_ws_max_det_rcs = -24.5F;

      // Host water spray detector
      calibrations.k_hws_para_box_host_speed_factor = 0.4F;
      calibrations.k_hws_ortho_box_host_speed_factor = 0.15F;

      // Object based water spray detector
      calibrations.k_ows_min_long_pos = -25.0F;
      calibrations.k_ows_max_long_pos = 50.0F;
      calibrations.k_ows_max_lat_pos = 25.0F;
      calibrations.k_ows_para_box_obj_speed_factor = 0.57F;
      calibrations.k_ows_ortho_box_obj_speed_factor = 0.1F;
      calibrations.k_ows_range_rate_min_factor = 0.1F;
      calibrations.k_ows_range_rate_max_factor = 0.7F;
      calibrations.k_ows_zone_lng_ext_threshold = 11.0F;
      calibrations.k_ows_zone_halfwidth_ext_threshold = 1.3F;

      // Host vehicle clutter handling
      calibrations.max_range_flagging_hvc_dets = 0.30F;

      // Longi stat curves
      calibrations.k_lsc_min_long_pos = -250.0F;
      calibrations.k_lsc_max_long_pos = 250.0F;
      calibrations.k_lsc_long_pos_gate = 5.0F;
      calibrations.k_lsc_lat_pos_gate = 1.3F;
      calibrations.k_lsc_min_points_in_cluster = 3U;
      calibrations.k_lsc_long_merging_gate = 3.0F;
      calibrations.k_lsc_lat_merging_gate = 3.0F;
      calibrations.k_lsc_cluster_merge_thr = 1.2F;
      calibrations.k_lsc_length_score_gain = 100.0F;
      calibrations.k_lsc_max_a_coeff = 0.3F;
      calibrations.k_distance_to_circle_thr = 2.0F;

      // Static Environment Polynomials
      calibrations.k_sep_p2_coeff_poly_linear_thr = 0.0001F;
      calibrations.k_sep_max_k_coeff_for_lateral_line = 1000.0F;
      calibrations.k_sep_det_on_poly_thr = 0.75F;
      calibrations.k_sep_obj_on_poly_thr = 0.75F;

      // Termination of coasting objects
      calibrations.k_max_conf_objtrk_coast_time = 0.55F;
      calibrations.k_max_coast_time_mirror = 0.25F;
      calibrations.k_max_coast_time_outside_fov = 0.055F;


      // Initializaiton
      calibrations.k_init_min_num_dets_from_outside_restrictive_zone = 3U;
      calibrations.k_init_min_num_dets_from_restrictive_zone = 6U;

      calibrations.k_init_fast_moving_upper_threshold = 5.0F;
      calibrations.k_init_movable_prob_threshold = 0.5F;
      calibrations.k_init_default_confidence = 0.15F;


      // Object size estimation
      calibrations.k_movable_max_target_width = 2.5F;
      calibrations.k_fast_movable_max_target_length = 25.0F;
      calibrations.k_slow_movable_max_target_length = 4.0F;
      calibrations.k_min_CTCA_target_length = 2.5F;
      calibrations.k_min_aspect_ratio = 0.15F;
      calibrations.k_max_aspect_ratio = 0.667F;

      // Calibration variables for object size shrink logic. Commonly used in both track grouping (merge) and size update algorithms
      calibrations.k_object_shrinking_speed_threshold = 5.0F;
      calibrations.k_max_length_for_slow_moving_objects = 6.5F;
      calibrations.k_min_length_for_slow_moving_objects = 2.5F;
      calibrations.k_speed_for_max_length_of_slow_moving_objects = 3.0F;
      calibrations.k_speed_for_min_length_of_slow_moving_objects = 1.0F;
      assert(calibrations.k_object_shrinking_speed_threshold > calibrations.k_speed_for_max_length_of_slow_moving_objects);
      assert(calibrations.k_speed_for_max_length_of_slow_moving_objects > calibrations.k_speed_for_min_length_of_slow_moving_objects);
      assert(calibrations.k_max_length_for_slow_moving_objects > calibrations.k_min_length_for_slow_moving_objects);


      // Object size update
      calibrations.k_size_update_min_det_range = 5.0F;
      calibrations.k_size_update_base_measurement_uncertainty = 5.0F;
      calibrations.k_size_update_base_process_noise = 1e-3F;
      calibrations.k_size_update_speed_threshold_low_speed_process_noise = 2.0F;
      calibrations.k_size_update_low_speed_process_noise = 1e-6F;
      calibrations.k_size_update_min_speed_to_update_nonvisible_side = 5.0F;
      calibrations.k_size_update_process_noise_pruning = 0.1F;
      calibrations.k_min_num_dets_to_decrease_meas_uncertainty = 5U;

      // Object motion classification
      calibrations.k_object_motion_sigma_ctca_th = 3.0F;
      calibrations.k_object_motion_min_speed = 0.5F;
      calibrations.k_object_motion_min_consec_moving_cnt_movable_th = 2;
      calibrations.k_object_motion_min_consec_moving_cnt_th = 3;
      calibrations.k_object_motion_min_consec_moving_cnt_high_yaw_th = 5;

      calibrations.k_object_motion_min_consec_stopped = 140;
      calibrations.k_object_motion_queue_zone_host_stationary_speed_threshold = 0.5F;
      calibrations.k_object_motion_queue_zone_long_dist = 30.0F;
      calibrations.k_object_motion_queue_zone_long_dist_host_stationary = 15.0F;
      calibrations.k_object_motion_queue_zone_lat_dist = 4.0F;
      calibrations.k_object_motion_max_abs_orient_diff_to_host = F360_DEG2RAD(65.0F);
      calibrations.k_object_motion_min_consec_stop_time_th = 3.0F;

      calibrations.k_object_motion_min_host_yaw_rate_th = F360_DEG2RAD(2.0F);

      calibrations.k_object_motion_min_moving_dets_percentage_th = 0.25F;
      calibrations.k_object_motion_parallel_moving_heading_th = F360_DEG2RAD(10.0F);
      calibrations.k_object_motion_parallel_moving_speed_diff_th = 0.25F;
      calibrations.k_object_motion_parallel_moving_lat_posn_th = 5.0F;
      calibrations.k_object_motion_parallel_moving_lon_posn_th = 5.0F;

      calibrations.k_object_motion_nees_min_p_value_th = 0.001F;
      calibrations.k_object_motion_nees_max_test_variable = 64.0F;
      calibrations.k_object_motion_nees_range_scaling_factor = 0.1F;

      calibrations.k_object_motion_cross_moving_min_abs_vcs_heading_th = F360_DEG2RAD(10.0F);
      calibrations.k_object_motion_cross_moving_max_abs_vcs_azimuth_th = F360_DEG2RAD(45.0F);

      calibrations.k_object_motion_occluded_speed_scale_factor = 2.5F;
      calibrations.k_occlusion_zone_long_left_rear_stationary_check = 0.0F;
      calibrations.k_occlusion_zone_lat_left_rear_stationary_check = -10.0F;
      calibrations.k_occlusion_zone_long_right_front_stationary_check = 10.0F;
      calibrations.k_occlusion_zone_lat_right_front_stationary_check = 10.0F;

      // Confidence overall
      calibrations.k_conf_overall_timeout_to_start_decay = 0.125F;

      calibrations.k_conf_overall_difference_thresh_long_posn_h = 2.0F;
      calibrations.k_conf_overall_difference_thresh_long_posn_m = 1.0F;
      calibrations.k_conf_overall_difference_thresh_long_posn_l = 0.5F;
      calibrations.k_conf_overall_difference_thresh_long_posn_vl = 0.3F;
      calibrations.k_conf_overall_difference_thresh_lat_posn_h = 2.0F;
      calibrations.k_conf_overall_difference_thresh_lat_posn_m = 1.0F;
      calibrations.k_conf_overall_difference_thresh_lat_posn_l = 0.5F;
      calibrations.k_conf_overall_difference_thresh_lat_posn_vl = 0.3F;

      calibrations.k_conf_cca_overall_difference_thresh_speed_h = 1.2F;
      calibrations.k_conf_cca_overall_difference_thresh_speed_m = 0.6F;
      calibrations.k_conf_cca_overall_difference_thresh_speed_l = 0.3F;
      calibrations.k_conf_cca_overall_difference_thresh_speed_vl = 0.15F;

      calibrations.k_conf_ctca_overall_difference_thresh_speed_h = 1.5F;
      calibrations.k_conf_ctca_overall_difference_thresh_speed_m = 0.75F;
      calibrations.k_conf_ctca_overall_difference_thresh_speed_l = 0.375F;
      calibrations.k_conf_ctca_overall_difference_thresh_speed_vl = 0.2F;

      // Unreliable low confidence track
      calibrations.k_low_conf_unreliability_max_ttc = 10.0F;
      calibrations.k_low_conf_unreliability_age_thr = 4.7F;
      calibrations.k_conf_downselection_exclusion_box_lat = 8.0F;
      calibrations.k_conf_downselection_exclusion_box_long = 10.0F;
      calibrations.k_low_conf_unreliability_min_heading = F360_DEG2RAD(30.0F);
      calibrations.k_low_conf_max_allowed_host_speed_in_cta_scenarios = 0.2F;
      calibrations.k_low_conf_expected_abs_object_heading_vcs_in_cta_scenarios = F360_DEG2RAD(90.0F);
      calibrations.k_low_conf_max_allowed_abs_heading_difference_in_cta_scenarios = F360_DEG2RAD(35.0F);

      // Object class determination
      calibrations.k_ad_oc_mean_length_pedestrian = 0.5F;
      calibrations.k_ad_oc_inv_standard_deviation_length_pedestrian = 1.0F / 0.4F;

      calibrations.k_ad_oc_mean_length_2wheel = 1.2F;
      calibrations.k_ad_oc_inv_standard_deviation_length_2wheel = 1.0F / 0.4F;

      calibrations.k_ad_oc_mean_length_car = 5.3F;
      calibrations.k_ad_oc_inv_standard_deviation_length_car = 1.0F / 1.3F;

      calibrations.k_ad_oc_mean_length_truck = 14.0F;
      calibrations.k_ad_oc_inv_standard_deviation_length_truck = 1.0F / 4.0F;

      calibrations.k_ad_oc_step_decrease_prob_unknown = 0.1F;

      calibrations.k_ad_oc_max_stationary_speed = 0.55F;
      calibrations.k_ad_oc_prob_decrease = 0.3F;

      // Predict existence probability
      calibrations.k_p_persist_outside_long_range = 0.66F;
      calibrations.k_p_persist_inside_long_range = 0.99F;

      // Bistatic
      calibrations.k_bistatic_cond_assoc_area_min_lat = -5.0F;
      calibrations.k_bistatic_cond_assoc_area_max_lat = 5.0F;
      calibrations.k_bistatic_cond_assoc_area_min_lon = -12.0F;
      calibrations.k_bistatic_cond_assoc_area_max_lon = 30.0F;
      calibrations.k_bistatic_lat_th_extension = 0.7F;

      // Host mirror track
      calibrations.k_host_refl_min_obj_long_pos = -15.0F;
      calibrations.k_host_refl_max_obj_long_pos = 40.0F;

      calibrations.k_host_refl_half_host_length = 2.5F;

      calibrations.k_host_refl_bbox_lat_ext = 2.75F;
      calibrations.k_host_refl_bbox_long_ext = 2.0F;

      calibrations.k_host_refl_filtering_distance = 20.0F;

      calibrations.k_host_refl_lowspeed_host_speed_th = 5.0F;
      calibrations.k_host_refl_lowspeed_speed_diff_th = 1.0F;
      calibrations.k_host_refl_lowspeed_heading_th = 0.1F;

      calibrations.k_host_refl_highspeed_heading_th = 0.7F;
      calibrations.k_host_refl_highspeed_min_speed_diff_th = 1.0F;
      calibrations.k_host_refl_highspeed_speed_diff_ramp_coef = 0.4F;
      calibrations.k_host_refl_highspeed_max_speed_diff_th = 7.0F;

      calibrations.k_ws_bbox_len_extension_factor = 0.3F;
      calibrations.k_ws_bbox_wid_extension_factor = 0.1F;

      // Object confidence and raw confidence level
      calibrations.k_conf_updated_tracks_filter_const = 0.2F;
      calibrations.k_conf_raw_weight_dets = F360_Logf(0.1F);
      calibrations.k_conf_raw_max_value_not_reduced_dets = 1.0F - F360_Expf(calibrations.k_conf_raw_weight_dets * 1.0F); // max allowed raw confidence level with only associated detections
                                                                                                                         // is equal to confidence with a single reduced detection

      calibrations.k_conf_coasted_min_time_since_init = 1.0F;
      calibrations.k_conf_coasted_min_time_trusted_track = 3.0F;
      calibrations.k_conf_coasted_min_long_posn_tructed_track = 30.0F;
      calibrations.k_conf_coasted_max_long_posn_tructed_track = 100.0F;
      calibrations.k_conf_coasted_min_average_confidence_level = 0.7F;

      // Flagging of azimuth range rate outliers
      calibrations.k_az_rdot_max_sq_dist = 49.0F;
      calibrations.k_az_rdot_max_az_diff = F360_DEG2RAD(3.0F);
      calibrations.k_az_rdot_min_rdot_diff = 0.25F;

      // Dead zone
      calibrations.k_use_dead_zone_in_stationkeeping_scenarions = true;
      calibrations.k_dead_zone_min_host_speed = 4.7F;
      calibrations.k_dead_zone_max_obj_vcs_lat_pos = 5.0F;
      calibrations.k_dead_zone_max_obj_vcs_heading = 0.5F;
      calibrations.k_dead_zone_max_rel_vel_diff = 0.4F;
      calibrations.k_dead_zone_assoc_gates_additional_enhacementl = 1.0F;
      calibrations.k_dead_zone_long_limit_extension = 4.0F;

      // Split logic
      calibrations.k_orth_split_min_speed = 5.0F;
      calibrations.k_orth_split_orth_delta_filter_const = 0.2F;
      calibrations.k_orth_split_orth_gap_filter_prop_const = 0.06F;
      calibrations.k_range_rate_diff_saturation_const = 0.8F;
      calibrations.k_orth_split_orth_gap_filter_max_dets = 5U;
      calibrations.k_orth_split_min_orth_gap_for_split_high = 3.8F;
      calibrations.k_orth_split_min_orth_gap_for_split_short_objects = 2.6F;
      calibrations.k_orth_split_min_orth_gap_for_split_medium = 3.0F;
      calibrations.k_orth_split_min_orth_gap_for_split_low = 1.8F;
      calibrations.k_orth_split_min_rr_diff_for_short_object_orth_gap = 0.25F;
      calibrations.k_orth_split_min_rr_diff_for_medium_orth_gap = 0.3F;
      calibrations.k_orth_split_min_rr_diff_for_low_orth_gap = 0.6F;
      calibrations.k_orth_split_min_distance_sq_for_low_orth_gap = 25.0F * 25.0F;
      calibrations.k_orth_split_max_distance_sq_for_low_orth_gap = 50.0F * 50.0F;
      calibrations.k_orth_split_max_length_for_low_orth_gap = 12.0F;
      calibrations.k_orth_split_width_gain = 1.2F;
      calibrations.k_orth_split_max_distance_sq = 80.0F * 80.0F;
      calibrations.k_orth_split_min_distance_sq = 15.0F * 15.0F;
      calibrations.k_pos_delta_heading_filter_constant = 0.1F;
      calibrations.k_orth_split_max_length_short_object = 6.0F;

      // Multi Path detector - low level logic
      calibrations.k_mp_object_reflector_size_extension = 0.1F;

      // Multi Path detector - high level logic
      calibrations.k_mp_max_allowed_host_speed_to_use_MP = 2.0F;
      calibrations.k_mp_default_mirror_probability = 1.0F;

      // Multi Path detector - reflector selector
      calibrations.k_mp_half_long_zone = 20.0F;
      calibrations.k_mp_half_lat_zone = 30.0F;

      // RCS filter (Radar Cross Section filter)
      calibrations.k_average_rcs_filter_constant = 0.15F;

      // Trailer
      calibrations.k_trailer_distance_rear_axle_to_tow_hitch = 1.2F;
      calibrations.k_trailer_bbox_extend_length = 1.0F;

      // Commercial vehicle trailer
      calibrations.k_cvt_trailer_width = 2.55F;
      calibrations.k_cvt_reversing_speed_threshold = -0.5F;
      calibrations.k_cvt_slow_speed_threshold = 0.1F;

      // Overall Confidence Blocker
      calibrations.k_ocb_cnt_delta_midlow_rcs = 1U;
      calibrations.k_ocb_cnt_delta_low_rcs_or_mult_dets = 2U;
      calibrations.k_ocb_cnt_max = 10U;
      calibrations.k_ocb_rcs_thresh_midlow_rcs = -18.0F;
      calibrations.k_ocb_rcs_thresh_low_rcs = -23.0F;
      calibrations.k_ocb_rcs_thresh_hi_rcs = 5.0F;
      calibrations.k_ocb_max_range = 10.0F;
      calibrations.k_ocb_max_range_rate = 2.0F;

      calibrations.k_cca_low_speed_th_to_ramp_down_proceess_noise = 1.5F;
      calibrations.k_cca_high_speed_th_to_ramp_down_proceess_noise = 5.0F;
      calibrations.q_cca_pos_low_speed = 0.7F;
      calibrations.q_cca_vel_low_speed = 0.0F;
      calibrations.q_cca_acc_low_speed = 0.05F;
      calibrations.init_cca_pnt_filter_cov[0][0] = 0.6F;
      calibrations.init_cca_pnt_filter_cov[0][1] = 0.0F;
      calibrations.init_cca_pnt_filter_cov[1][0] = 0.0F;
      calibrations.init_cca_pnt_filter_cov[1][1] = 0.01F;
      calibrations.k_cca_min_speed_to_update_pnt = 0.001F;


      // Detection double bounce detector
      calibrations.k_db_max_range = 40.0F;
      calibrations.k_db_max_nr_multi_bounces = 3U;
      calibrations.k_db_range_threshold_frac = 0.15F;
      calibrations.k_db_min_range_threshold = 0.2F;
      calibrations.k_db_max_range_threshold = 3.0F;
      calibrations.k_db_range_rate_threshold = 2.0F;
      calibrations.k_db_azimuth_thres_k = -0.0052F;
      calibrations.k_db_azimuth_thres_m = 0.1222F;
      calibrations.k_db_min_azimuth_thres = F360_DEG2RAD(2.0F);
      calibrations.k_db_max_azimuth_thres = F360_DEG2RAD(5.0F);

      // Mulitpath detection filter
      calibrations.k_min_num_valid_dets_for_bad_az_filter = 50U;
      calibrations.k_min_host_speed_for_bad_az_filter = 1.0F;
      calibrations.k_max_fraction_of_bad_azimuth_dets_default = 1.0F;
      calibrations.k_max_fraction_of_bad_azimuth_dets_srr5 = 0.33F;

      // Mark MRR3 detections high elevation
      calibrations.k_min_host_speed_for_check_det_az_conf_and_elevation = 2.0F;
      calibrations.k_mrr3_max_range = 20.0F;
      calibrations.k_mrr3_conf_thresh = 2U;
      calibrations.k_mrr3_max_abs_elev_angle = F360_DEG2RAD(5.5F);

      // Obstacle probability
      calibrations.k_obstacle_prob_min_bbox_height_nonmovable = 0.5F;
      calibrations.k_obstacle_prob_min_bbox_height_moving = 1.0F;

      // Sanity check given calibrations
      assert(calibrations.k_orth_split_min_speed >= calibrations.k_min_speed_for_updating_heading);
      assert(calibrations.k_max_num_consistent_outliers_orth > 1);
   }
}
