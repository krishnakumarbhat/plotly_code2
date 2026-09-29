/*===========================================================================*\
* FILE: f360_calibrations.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains F360_Calibrations structure declaration
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_CALIBRATIONS_H
#define F360_CALIBRATIONS_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_sensor_type.h"
#include "f360_conf.h"

namespace f360_variant_A
{
   typedef struct F360_Calibrations_Tag
   {
      float32_t host_vicinity_vcs_x_range;
      float32_t host_vicinity_vcs_y_range;
      float32_t fast_moving_thresh;
      float32_t low_confidence_level_thresh;
      float32_t snr_valid_thresh; // do not use SNR when its value is below this threshold
      // Time Update
      // CTCA
      float32_t k_max_allowed_curv_variance; // [1/m^2] Maximum curvature variance allowed in time update. If resulting curvature variance is larger then it is saturated.
      float32_t k_min_acc_for_increasing_acc_noise; // [m/s^2] Minimum absolute tangential acceleration for when to start ramping up acceleration Q variance in CTCA time update (i.e. for acceleration below this value Q will be at lowest level)
      float32_t k_max_acc_for_increasing_acc_noise; // [m/s^2] Maximum absolute tangential acceleration for when to stop ramping up acceleration Q variance in CTCA time update (i.e. for acceleration above this value Q will be at highest level)
      float32_t k_max_acc_scale_factor; // [-] Maximum scale factor for acceleration Q variance in CTCA time update
      float32_t k_normal_noise_acc; // [m^2/s^4] Acceleration process noise calibration parameter (for Q matrix)
      float32_t k_normal_noise_speed; // [m^2/s^2] Speed process noise calibration parameter (for Q matrix)
      float32_t k_normal_noise_pos_para; // [m^2] Para position process noise calibration parameter (for Q matrix)
      float32_t k_q_tuning_orth_direction_speed_breakpoint; // [m/s] Parameter used for Q matrix tuning of the orth/non-tangential behaviour of the object. Speed threshold that corresponds to when the assumption of "maximum possible curvature is constant and determined my min turning radius" transitions into assumption of "maximum possible curvature is determined by maximum possible lateral acceleration"
      float32_t k_normal_noise_curv; // [1/m^2] Curvature process noise calibration parameter (for Q matrix)
      float32_t k_normal_noise_hdg; // [rad^2] Heading process noise calibration parameter (for Q matrix)
      float32_t k_normal_noise_pos_orth; // [m^2] Orth position process noise calibration parameter (for Q matrix)
      // Other
      float32_t k_conf_overlapping_reduction_factor;
      float32_t k_hyst_time_for_coasted_objects;

      // CTCA measurement update
      float32_t k_ctca_msmnt_update_max_reverse_abs_spd; // [m/s] Maximum absolute value of speed to be considered as a reversing object.

      // CCA measurement update
      float32_t k_cca_msmnt_update_vel_var; // [(m / s)^2] Assumed variance of velocity measuremenmt used in CCA pointing, yaw rate filter measurement update
      float32_t k_speed_th_for_saturating_r; // Speed threshold for saturating R in CCA pointing, yaw rate filter measurement update
      float32_t k_max_innov_dist_thres; // [m] Maximum position innovation allowed for non-moveable CCA objects before the maximum limitation on how much the position measurement is allowed to impact the velocity estimate is reached.
      float32_t k_max_innov_dist_yaw_rate_gain; // [ms/rad] Gain on host yaw rate to compute "total k_max_innov_dist_thres". The gain multiplied with host yaw rate plus k_max_innov_dist_thres yields the "total k_max_innov_dist_thres".
      float32_t k_min_decrease_factor; // [-] Minimum decrease factor of Kalman gain elements to limit on how much the position measurement is allowed to impact the velocity estimate for non-moveable CCA objects. Should be in the range of [0, 1]

      // CTCA and CCA time and measurement update for preventing overshooting of velocity and acceleration estimates at hard breaking
      float32_t k_abs_acc_threshold_for_breaking; // [m/s^2] Threshold on absolute value of acceleration for assuming an object is braking.
      float32_t k_abs_speed_threshold_for_stopping; // [m/s] Threshold on object absolute value of speed for considering it being stopped

       // CTCA and CCA measurement update for preventing overshooting of heading and orietnation when the object is occluded in highway scenario
      float32_t k_speed_threshold_for_kmat_decrease; // [m/s] Threshold of speed for k mat decrease for occluded objects
      float32_t k_x_pos_threshold_for_kmat_decrease; // [m] Threshold of x position for k mat decrease for occluded objects
      float32_t k_heading_threshold_for_kmat_decrease; // [rad] Threshold of heading for k mat decrease for occluded objects
      float32_t k_kmat_decrease_factor_for_occluded; // [-] Decrease factor for the kalman gain factor for occluded objects

      // Track validity
      float32_t k_tv_refl_gr_trk_max_diff_x; // [m] maximum x_vcs distance of an object track from source position to consider it as a source of reflected ghost candidate
      float32_t k_tv_refl_gr_trk_max_diff_y; // [m] maximum y_vcs distance of an object track from source position to consider it as a source of reflected ghost candidate
      float32_t k_tv_refl_gr_trk_max_diff_rel_vel; // [m/s] maximum relative velocity difference between an object track and ghost candidate to consider it as a source of reflection
      float32_t k_tv_refl_gr_trk_max_diff_heading; // [rad] maximum heading difference between an object track and ghost candidate to consider it as a source of reflection
      float32_t k_tv_refl_gr_trk_min_bbox_lat_margin; // [m] minimum tcs lateral extension of source candadiate bbox in which the hypothetic source point has to consider reflection
      float32_t k_tv_refl_gr_trk_bbox_lat_margin; // [m] tcs lateral extension of source candadiate bbox in which the hypothetic source point has to consider reflection
      float32_t k_tv_refl_gr_trk_straight_mov_head_th; // [rad] max allowed abs heading of source candidate consider it as a source of reflected ghost candidate
      float32_t k_tv_refl_gr_trk_min_sep_lon_pos; // [m] lower bound of x_vcs interval on the guardrail in which reflective objects are analyzed
      float32_t k_tv_refl_gr_trk_max_sep_lon_pos; // [m] upper bound of x_vcs interval on the guardrail in which reflective objects are analyzed
      float32_t k_tv_dets_exp_filter_const; // [s] Time constant for first order exponential low pass filter

      float32_t host_vehicle_width;
      float32_t host_vehicle_length;

      float32_t k_vp_vehicle_next_to_ego_max_lat_dist;
      float32_t k_vp_vehicle_next_to_ego_max_long_dist;

      float32_t k_vp_vehicle_next_to_ego_max_abs_heading;

      float32_t k_vp_vehicle_next_to_ego_long_pos_offset;

      float32_t k_min_speed_for_updating_heading; // [m/s] Minimum speed for updating heading (e.g. in measurement update and split logic where heading is computed from position difference)

      // Sensor preprocessing
      float32_t rdot_interval_compatability_dealiasing_gate; // [m/s] Range rate dealiased gate for two range rate intervals to be compatable

      // Clusters Grouping parameters
      float32_t k_stat_clusters_dist_sq_coarse_gate; // [m^2] Squared distance gate for clusters
      float32_t k_moving_clusters_dist_coarse_gate; // [m] Longitudinal distance gate for clusters
      float32_t k_moving_clusters_dist_sq_coarse_gate_1; // [m^2] Smaller squared distance gate for clusters with lower time_since_measurement diff
      float32_t k_moving_clusters_dist_sq_coarse_gate_2; // [m^2] Larger squared distance gate for clusters with higher time_since_measurement diff
      float32_t k_moving_clusters_time_diff_coarse_gate; // [ms] Time difference gate between two clusters
      float32_t k_max_dealiased_range_rate_diff; // [m/s] Maximum difference in dealiased calculated target range rate and detection range rate to dealiase detection.
      float32_t k_rdot_half_gate; // [m/s] Range rate gate for determining if a detection's range rate is close enough to the calculated target range rate to be considered dealiased. If the absolute value of the difference between the detection range rate and calculated target range rate is below this gate, detection range rate will be replaced with calculated target range rate.
      float32_t k_radial_gate; // [m] Size of world-az axis of elliptical fine position gate
      float32_t k_default_max_cross_radial_speed; // [m/s] Default maximum speed along the orthogonal-to-world-az axis of the elliptical fine position gate for clusters, used for determining the size of the cross-radial axis of the elliptical fine position gate for clusters.
      float32_t k_increased_max_cross_radial_speed; // [m/s] Increased maximum speed along the orthogonal-to-world-az axis of the elliptical fine position gate for clusters, used for determining the size of the cross-radial axis of the elliptical fine position gate for clusters. This is designed for close clusters beside host that might be moving very fast in parallel direction.
      float32_t k_range_thr_for_full_az_dependent_adj; // [m] Range threshold for applying the full azimuth dependent adjustment of cross-radial gate size based on detection azimuth. Below this range, full azimuth dependent adjustment of cross-radial gate size will be applied, above this range, adjustment will be applied gradually.
      float32_t k_range_thr_for_az_dependent_adj; // [m] Range threshold for applying azimuth dependent adjustment of cross-radial gate size. Below this range, azimuth dependent adjustment of cross-radial gate size will be applied, above this range it will not be applied.
      float32_t k_slow_moving_speed_thr_for_cross_radial_gate; // [m/s] Speed threshold for when to consider an object as slow moving (as opposed to fast moving) for the purpose of determining the target size component of the cross-radial gate.
      float32_t k_fast_moving_speed_thr_for_cross_radial_gate; // [m/s] Speed threshold for when to consider an object as fast moving for the purpose of determining the target size component of the cross-radial gate. For speed above this threshold, target size will be at maximum.
      float32_t k_slow_moving_size_for_cross_radial_gate; // [m] Target size component of the cross-radial gate for slow moving objects.
      float32_t k_fast_moving_size_for_cross_radial_gate; // [m] Target size component of the cross-radial gate for fast moving objects.
      float32_t k_az_scaling_factor_long_range; // [-] Scaling factor for the azimuth component of the cross-radial gate for long range objects.
      float32_t k_az_scaling_factor_close_range; // [-] Scaling factor for the azimuth component of the cross-radial gate for close range objects.
      float32_t k_cross_radial_movement_default_saturation; // [m] Default maximum allowed movement along the orthogonal-to-world-az axis of the elliptical fine position gate for clusters.
      float32_t k_cross_radial_movement_increased_saturation;// [m] Increased maximum allowed movement along the orthogonal-to-world-az axis of the elliptical fine position gate for clusters, designed for close clusters beside host that might be moving very fast in parallel direction.
      float32_t k_hard_range_thr_for_close_range_logic; // [m] Range threshold for when to switch between close range logic and long range logic for cross-radial gate calculation. This is determined based on the size component of the cross-radial gate and the azimuth scaling factors to ensure a smooth transition between close range and long range logic.
      float32_t k_hard_range_thr_for_long_range_logic; // [m] Range threshold for when to switch between close range logic and long range logic for cross-radial gate calculation. This is determined based on the size component of the cross-radial gate and the azimuth scaling factors to ensure a smooth transition between close range and long range logic.


      // Measurement update parameters
      float32_t k_ref_msmt_cov_cca; // Reference variance for range rate compensated measurements in CCA measurement update.
      float32_t k_ref_msmt_cov_ctca; // Reference variance for range rate compensated measurements in CTCA measurement update.
      float32_t k_acc_vel_state_decay_factor; // Decay factor of velocity and acceleration when all assigned detections are stationary.
      uint32_t k_min_num_selected_dets_per_sensor_for_binning; // If an object is eligible for binning & has more detections than this from a single sensor, bin detections

      // Reference point update parameters
      float32_t k_normal_obj_ref_pnt_hysteresis_factor; // [-] Hysteresis factor for switching from old reference point to new.
      float32_t k_normal_obj_length_thr; // [m] Hysteresis factor for switching from old reference point to new.
      float32_t k_long_obj_length_thr; // [-] Hysteresis factor for switching from old reference point to new.
      float32_t k_long_obj_ref_pnt_hysteresis_factor; // [m] Hysteresis factor for switching from old reference point to new.
      float32_t k_sin_max_pointing_error_sq; // [-] Square of sinus of assumed maximum object pointing error used for increaing position uncertanty after reference point switch
      float32_t k_frac_of_moved_dist; // [-] Scaling parameter that is multplied with the moved distance of the reference point in order to determin how much position uncertainty should be increased after reference point switch
      float32_t k_far_away_object_dist_sq_thr; // [m^2] Distance squared, beyond which only side reference points are chosen

      // Determine reflected object
      float32_t k_mirror_prob_threshold; // Threshold if object should be reduced or not based on its mirror probability, if mirror probablity is higher than this value the object will not be reduced
      float32_t k_reflective_guardrail_track_min_host_speed; // [m/s] Track Validity module: Minimum host speed for which Is_Reflective_Guardrail_Track is run
      float32_t k_reflected_object_max_mirror_probability; // [-] Track Validity module: Maximum mirror probability assigned to an object that is suspected of being a reflection

      // Determine to freeze track
      float32_t min_object_age_for_object_close_to_stat_host; // Minimal age to classify an object as "close to host" when host is stationary

      // Guardrail based angle jump detector
      float32_t k_angle_jump_range_tolerance; // [m] detection range gap condition -> (dist_to_guardrial - tollerance, dist_to_guardrial + tollerance)
      float32_t k_angle_jump_max_abs_range_rate; // [m/s] maximum allowed detection range rate
      float32_t k_angle_jump_min_abs_azimuth_vcs; // [rad] minimum detection azimuth (in vcs) condtion to filter out detection from detector
      float32_t k_angle_jump_max_abs_azimuth_vcs; // [rad] maximum detection azimuth (in vcs) condtion to filter out detection from detector
      float32_t k_angle_jump_max_range; // [m] maximum detection's range for angle jump checking
      float32_t k_angle_jump_long_search_margin; // [m] longitudinal search margin for SEP

      // Mark stationary bounced detections
      float32_t k_stat_bounce_min_host_speed; // [m/s] Minimum host speed for lookng for stationary bounce detections.
      float32_t k_stat_bounce_max_trk_long_posn; // [m] Maximum longitudinal position of true object for which tracker is lookng for stationary bounce detections.
      float32_t k_stat_bounce_max_trk_lat_dist; // [m] Maximum lateral distance from host of true object for which tracker is lookng for stationary bounce detections.
      float32_t k_stat_bounce_max_trk_heading; // [rad] Maximum heading of true object for which tracker is lookng for stationary bounce detections.
      float32_t k_stat_bounce_azimuth_border_ext;// [rad] Extension of left and right azimuth borders of zone where algorithm looks for stationary bounced detections.
      float32_t k_stat_bounce_range_rate_diff_thr; // [m/s] Maximum difference between reported detection range rate and expected detection range rate to flag detection as stationary bounce.
      float32_t k_stat_bounce_min_det_long_posn; // [m] Minumum longitudinal position of detecion, which may be flagged as stationary bounce.

      // Filter out low quality detections on or behind guardrail
      float32_t k_max_rcs_thr_low_quality_detection_filter; // [dB] Detection maximum rcs for filtering away detections behind/on guardrail with low rcs and low azimuth confidence.
      int8_t k_azimuth_conf_low_quality_detection_filter; // [-] Detection azimuth confidence for filtering away detections behind/on guardrail with low rcs and low azimuth confidence.

      // Parameters for Detect_Wheel_Spin_Pairs
      float32_t k_max_wheel_spin_dist_sq;  // [m^2] max squared position difference to consider two detections close
      float32_t k_min_wheel_spin_doppler_spread;  // [m/s] min diff in range rate between close detections to consider them wheel spin
      float32_t k_max_abs_vcs_long_posn_for_wheelspin_pair; // [m] Maximum absolute longitude pos to check for wheelspin pair
      float32_t k_max_abs_vcs_lat_posn_for_wheelspin_pair; // [m] Maximum absolute latitude pos to check for wheelspin pair
      float32_t k_max_azimuth_difference_for_wheelspin_pair; // [rad] max diff in azimuth to consider two detections close
      uint32_t k_max_wheel_spin_dets_to_mark; //max number of detection pairs to be marked as wheel spin
      uint8_t k_wheelspin_pair_max_close_det_iterations; // Maximum number of iterations to search for close detection to a wheelspin pair

      // Parameters for Mark_Detections_Wheel_Spin_From_Objects
      float32_t k_max_abs_vcs_long_posn_for_wheelspin;  // [m]   Maximum distance between object and host to consider wheelspin countermeasure
      float32_t k_min_speed_fast_moving;                // [m/s] Minimum object speed to flag object as fast moving
      float32_t k_min_rr_diff_wheelspin;                // [m/s] Minimum allowed difference between detection RR and object predicted RR, otherwise not marked as WS
      float32_t k_ws_lat_buffer_zone_oncoming;          // [m]   Extension of bounding box in lateral direction for an oncoming object with motion parallel to host
      float32_t k_ws_lat_buffer_zone;                   // [m]   Extension of bounding box in lateral direction for an object with motion parallel to host
      float32_t k_ws_long_buffer_zone;                  // [m]   Extension of bounding box in longitudinal direction

      // Parameters for Detect_Near_By_Wheel_Spins
      float32_t k_nbws_max_lat_pos;      // [m] maxiumum lateral position of detection to be used by algorithm (are of interest)
      float32_t k_nbws_min_lat_pos;      // [m] minimum lateral position of detection to be used by algorithm (are of interest)
      float32_t k_nbws_max_long_pos;     // [m] maxiumum longitudinal position of detection to be used by algorithm (are of interest)
      float32_t k_nbws_min_long_pos;     // [m] minimum longitudinal position of detection to be used by algorithm (are of interest)
      float32_t k_nbws_lat_marking_th;   // [m] lateral distance threshold from cluster to detection for marking
      float32_t k_nbws_long_marking_th;  // [m] longitudinal distance threshold from cluster to detection for marking
      float32_t k_nbws_lat_asc_th;       // [m] lateral distance threshold from cluster to detection for cluster association
      float32_t k_nbws_long_asc_th;      // [m] longitudinal distance threshold from cluster to detection for cluster association

      // Parameters for Mark_Dets_As_Farside_And_Close_Target
      float32_t k_ct_and_fcm_max_abs_heading_diff_to_host; // [rad] Maximum absolute value of heading difference between host and track to be considered as parallel
      float32_t k_ct_and_fcm_max_dist_for_close_target_sq; // [m^2] Maximum distance squared that an object can be at to be considered for function Mark_Dets_As_Farside_And_Close_Target
      float32_t k_ct_orth_buffer_zone_factor;              // Factor multiplied by edge distance to determine how much the ct box should be extended in orth direction
      float32_t k_ct_para_buffer_zone_factor;              // Factor multiplied by edge distance to determine how much the ct box should be extended in para direction

      // Parameters for Cond_Deassoc_Low_RR_Dets
      float32_t k_cond_deassoc_min_obj_spd_for_deassoc; // [m/s] Minimum object speed to consider deassociation of detections
      float32_t k_cond_deassoc_det_comp_rr_max; // [m/s] Maximum detection compensated range rate to consider deassociation of detection
      float32_t k_cond_deassoc_fraction_of_width_to_deassoc; // [] Fraction of width that detection has to be from the visible orth edge to be deassociated

      // Existance Probability
      float32_t k_ep_init_p_det_sensor; // [-] default probability that object is detected by sensor
      float32_t k_ep_p_measurement_with_no_new_meas; // [-] probability of getting good measurement when object is not updated (coasted)
      float32_t k_ep_clutter_prob_with_meas; // [-] probability that new information from sensor is reported as false positive (ghost).
      float32_t k_ep_clutter_prob_with_no_meas;  // [-] probability factor when no new information is associated witch track object
      float32_t k_ep_min_allowed_exist_prob;  // [-] minimal probability value
      float32_t k_ep_prob_track_state_exp_scale;  // [-] scaling factor for quality of state estimate of the track
      float32_t k_ep_prob_track_state_exp_offset;  // [-] offset for approximation of sum(inv(Var_i))
      float32_t k_ep_bottom_saturation_of_normalized_variance; // [-] low state variance saturation level
      float32_t k_ep_variance_th_pos_xy; // [m^2] threshold for position variance
      float32_t k_ep_variance_th_heading; // [rad^2] threshold for heading variance
      float32_t k_ep_variance_th_velocity; // [(m/s)^2] threshold for velocity variance
      float32_t k_ep_variance_th_curvature; // [(1/m)^2] threshold for curvature variance
      float32_t k_ep_variance_th_accel; // [(m/s^2)^2] threshold for acceleration variance
      float32_t k_ep_variance_th_tan_accel; // [(1/s^2)^2] threshold for tangential acceleration variance
      float32_t k_ep_prob_track_state_init_value; // [-] init value for quality of the state estimate of the track.

      // Track Downselection
      float32_t k_track_downselect_dets_threshold;   // Treshold for filtered over the time number of detection.
      float32_t k_track_downselect_dets_threshold_low;  //Low dets treshold for filtered over the time number of detection.
      float32_t k_track_downselect_average_dets_thresh; // Average dets treshold for filtered over the time number of detection.
      float32_t k_track_downselect_min_time_filter_dets_thresh;  // Min time treshold for filtering numer of detections [s].
      float32_t k_track_downselect_confidence_level_lowering_factor;   // Factor for lowering confidence in some specal cases.
      float32_t k_track_downselect_confidence_thresh;   // Confidence threshold used to determine track priority level.

      // Track grouping
      float32_t k_track_grouping_hdg_gate;     // [rad] Heading gate for grouping two objects
      float32_t k_track_grouping_speed_gate;   // [m/s] Speed gate for grouping two objects
      float32_t k_track_grouping_curvature_gate;   // [rad/m] Curvature gate for grouping two objects
      float32_t k_merging_coarse_score_gate; // [-] Coarse score gate for grouping two objects
      float32_t merging_m2m_distance_threshold;                  // [m] maximum metal to metal distance between 2 objects to allow merge
      float32_t merging_lateral_det_spread_threshold;            // [m] maximum lateral spread of detection between 2 objects to allow merge
      float32_t merging_m2m_max_obj_speed;                       // [m/s] maximum objects speed for checking metal to metal and lateral spread conditions
      float32_t k_orth_split_width_threshold;                    // [m] Maximum width for objects after split

      // Calculate Time To Collision (ttc)
      float32_t k_calc_ttc_min_thresh_projected_velocity;  // Minimal value of velocity projected in to host direction [m/s].
      float32_t k_calc_ttc_max_thresh_projected_velocity;  // Maximal value of velocity projected in to host direction [m/s].

      // Post Update Track Adjustments
      float32_t k_puta_max_vcs_xposn_for_ghost_NU_2_C; // [m] maximum allowed longitudinal distance to set f_ghost_NU_2_C flag
      float32_t k_puta_max_vcs_yposn_for_ghost_NU_2_C; // [m] maximum allowed lateral distance to set f_ghost_NU_2_C flag
      float32_t k_puta_overlapping_tracks_max_speed_diff; // [m/s] Maximum speed difference between the two objects to be considered as overlapping.
      float32_t k_puta_overlapping_tracks_max_heading_diff; // [rad] Maximum heading difference between the two objects to be considered as overlapping.
      float32_t k_puta_overlapping_tracks_high_conf_thr; // [-] Minimum required confidence threshold for the object with the higher confidence in overlapping logic
      float32_t k_puta_overlapping_tracks_low_conf_thr; // [-] Maximum required confidence threshold for the object with the lower confidence in overlapping logic
      float32_t k_puta_overlapping_tracks_long_thr; // [m] Maximum absolute difference in longitudinal position between the two relevant objects to continue more detailed checks for overlapping tracks

      float32_t k_puta_obj_size_acc_filt_coef_innov_coasting_obj; // [-] Filter coefficient in object size accuracy for computing innovation for coasting object
      float32_t k_puta_obj_size_acc_filt_coef_innov_updated_obj; // [-] Filter coefficient in object size accuracy for computing innovation for updated object
      float32_t k_puta_obj_size_acc_innov_no_update_length; // [-] Filter coefficient in object size accuracy for computing innovation length
      float32_t k_puta_obj_size_acc_innov_no_update_width; // [-] Filter coefficient in object size accuracy for computing innovation width

      float32_t k_puta_min_object_confidence; // [-] Minimum object confidence level to be treated as trusted track.
      float32_t k_puta_min_object_time; // [s] Minimum object time since cluster was created to treat it as trusted track.
      float32_t k_puta_large_distance; // [m]  Large distance between two object, which is set when object to compare is not trusted.
      float32_t k_puta_orientation_diff_threshold; // Maximum orientation difference to allow kill object.

      // Association: Parameters for detection to track association
      float32_t k_min_assoc_gate_extension_non_moveable; // [m] Minimum possible extension of the circular association gate used for non-moveable objects
      float32_t k_max_assoc_gate_extension_non_moveable; // [m] Maximim possible extension of the circular association gate used for non-moveable objects
      float32_t k_max_assoc_gate_radius_non_moveable; // [m] Maximim possible radius of the circular association gate used for non-moveable objects
      float32_t k_obj_dist_for_min_assoc_gate_extension_non_moveable; // [m] Object distance at which the extension of the circular association gate used for non-moveable objects should be minimum possible
      float32_t k_obj_dist_for_max_assoc_gate_extension_non_moveable; // [m] Object distance at which the extension of the circular association gate used for non-moveable objects should be maximum possible
      float32_t k_spd_dependent_assoc_gate_extension_factor_non_moveable; // [1/s] Multiplication factor for the speed dependent part of the extension of the circular association gate used for non-moveable objects
      float32_t k_min_speed_for_increasing_occluded_long_assoc_buffer; // [m/s] For objects with speeds larger than this the association gates in the less visible para direction will be increased
      float32_t k_max_speed_for_saturating_occluded_long_assoc_buffer_increase; // [m/s] For objects with speeds larger than this the increase of the association gates in the less visible para direction will be saturated at maximum level
      float32_t k_second_min_speed_for_increasing_occluded_long_assoc_buffer; // [m/s] The second minimum breakpoint that we start increasing the longtitudinal association gate buffer
      float32_t k_second_max_speed_for_saturating_occluded_long_assoc_buffer_increase; // [m/s] The second max breakpoint that we saturate the longtitudinal association gate buffer
      float32_t k_min_occluded_long_buffer_increase; // [m] The minimum possible increase of the association gates in the less visible para direction of an object
      float32_t k_max_occluded_long_buffer_increase; // [m] The maximum possible increase of the association gates in the less visible para direction of an object
      float32_t k_min_assoc_gate_long_buffer_moveable_objs; // [m] Minimum size of the longitudinal association buffer for moveable objects
      float32_t k_min_assoc_gate_lat_buffer_moveable_objs; // [m] Minimum size of the laterall association buffer for moveable objects
      float32_t k_range_rate_score_threshold; // Threshold on range rate for association in the general case [m/s]
      float32_t k_range_buffer_max_dist; // [m] Maximum saturation distance for calculating range buffer
      float32_t k_range_buffer_min_val; // [m] Buffer value at minimum saturation distance for calculating range buffer
      float32_t k_range_buffer_max_val; // [m] Buffer value at maximum saturation distance for calculating range buffer
      float32_t k_az_buffer_max_dist; // [m] Maximum saturation distance for calculating azimuth buffer
      float32_t k_az_buffer_min_val; // [m] Buffer value at zero distance for calculating azimuth buffer
      float32_t k_az_buffer_max_val; // [m] Buffer value at maximum saturation distance for calculating azimuth buffer
      float32_t k_min_rr_diff_from_stationary_hypothesis; // [m/s] Minimum difference between moving and stationary range rate hypotheses.

      // Association: Parameters for calculating association detection score
      float32_t k_para_diff_weight_inside_box; // Weight for detection inside a moveable objects bounding box in para direction
      float32_t k_orth_diff_weight_inside_box; // Weight for detection inside a moveable objects bounding box in ortho direction
      float32_t k_rdot_diff_weight_inside_box; // Weight for a detections range rate inside a moveable objects bounding box
      float32_t k_dist_weight_inside_solid_circle; // Weight for detection inside a non-moveable objects outline (circel)
      float32_t k_rdot_diff_weight_inside_solid_circel; // Weight for a detections range rate inside a non-moveable objects outline (circel)
      float32_t k_para_diff_weight_inside_ext_box; // Weight for detection inside a moveable objects extended bounding box in para direction
      float32_t k_orth_diff_weight_inside_ext_box; // Weight for detection inside a moveable objects extended bounding box in ortho direction
      float32_t k_rdot_diff_weight_inside_ext_box; // Weight for a detections range rate inside a moveable objects extended bounding box
      float32_t k_dist_weight_inside_ext_circle; // Weight for detection inside a non-moveable objects association gate but outside of its outline (circel)
      float32_t k_rdot_diff_weight_inside_ext_circel; // Weight for a detections range rate inside a non-moveable objects association gate but outside of its outline (circel)
      float32_t k_score_outside_ext_bbox; // Score to use if detection is outside an objects extended bounding box
      float32_t k_base_score_bbox_center; // Base Score at the bounding box center

      // Association: Parameters for calculating range rate threshold
      float32_t k_vcs_distance_sqr_thr; // Distance squared from VCS for an object to be considered as "far away"
      float32_t k_rr_thr_factor_far_away_coasted; // Scaling factor of range rate threshold for objects coasting and "far away"
      float32_t k_rr_thr_factor_fov_edge; // Scaling factor of range rate threshold if detection is in the edge of FoV
      float32_t k_speed_threshold; // Speed threshold to allow expansion of range rate threshold for far away coasted objects

      // Association: Parameters for detection inlier selection
      float32_t k_rr_error_statistics_forgetting_factor; // [-] Determines the intertia of the mean and variance filter for range rate errors. Low forgetting factor means more history is ignored.
      float32_t k_max_number_of_historic_dets_obj_non_movable; // [-] Maximum number of historic dets after forgetting factor is applied for non-movable objects.
      float32_t k_max_number_of_historic_dets_obj_movable; // [-] Maximum number of historic dets after forgetting factor is applied for movable objects.
      float32_t k_max_historic_rr_error_variance; // [m^2/s^2] An object's maximum variance of historic range rate errors.
      float32_t k_min_range_rate_error_threshold; // [m/s] Minimum threshold for accepting range rate inliers.

      // Object tracks properties
      float32_t k_underdrive_min_trk_long_posn; // [m] maximal longitudinal position of considered tracks

      float32_t k_underdrive_min_zone_long_posn; // [m] minimal longitudinal position of tracks that can be in zone FOV
      float32_t k_underdrive_lat_buffer_factor; // Number that multiplies to the max_ocg_cell_width, and the result is used as a lateral threshold for ocg ud classification

      // Underdrivability for moving objects
      float32_t ud_mov_height_threshold;  // [m] Height threshold above which the object is suspected as underdrivable
      uint32_t ud_mov_cnt_consecutive_scans; // [-] Determines the number of scans for which object has to have historic height mean higher than threshold for underdrivability status to change
      float32_t ud_mov_posx_min_limit; // [m] Determines the minimum position x limit where we start to determine ud status for moving objects
      float32_t ud_mov_posx_max_limit; // [m] Determines the maximum position x limit to which we determine ud status for moving objects
      float32_t ud_mov_prob_can_pass_under; // [-] Defines the probability that the object is underdrivable when it has status "UNDERDRIVABLE_STATUS_CAN_PASS_UNDER"
      float32_t ud_mov_prob_can_not_pass_under; // [-] Defines the probability that the object is underdrivable when it has status "UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER"
      float32_t ud_mov_prob_not_to_consider; // [-] Defines the probability that the object is underdrivable when it has status "UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER"

      // Occupancy Grid related calibrations
      float32_t k_ocg_underdrive_small_curvature_th;  // [m^(-1)] minimal curvature of predicted host path to consider it as straight line
      bool f_ocg_use_curvilinear_simplification;  // Flag that indicates if the small curvature based simplication needs to used for object UD status assignment

      // radar phenomena
      float32_t rp_max_object_lateral_distance; // [m] max object lateral postion to be considered
      float32_t rp_max_abs_pointing_disagreement; // [rad] max absolute object pointing to be considered
      float32_t rp_object_max_longitudinal_margin; // [m] object longitudinal gap between sensor long position and object bumpers position to be considered
      float32_t rp_min_confidence_level; // [-] object minimum confidence level to be considered as reference object

      //object based angle jump detector
      float32_t obj_aj_border_half_width; // [m] half of border width that is use for detection filtration
      float32_t obj_aj_max_double_range_hypothesis; // [m] max detection range for double (doublebounce) hypothesis

      // object based multibounce detector
      float32_t mb_range_rate_diff_th;      //[m/s] range rate diff threshold between estimated range rate and range rate expected from object velocity
      float32_t mb_max_det_range;           //[m] maximum detection range to be checked by the algorithm
      float32_t mb_restricted_area_width;   //[m] width of resticted area
      uint8_t mb_max_num_bounces;        //[-] maximum bounce factor. Above this detetions are not considered as multibounces

      // Pseudo position estimation
      float32_t k_pseudo_pos_max_variance_threshold; // [m^2] Saturation threshold for pseudo position variances
      float32_t k_pseudo_pos_dist_diff_gain; // [-] Gain to be multiplied with distance (between time prediction and pseudo position) function for adding covariance to pseudo position measurement
      float32_t k_pseudo_pos_dist_diff_thr; // [m] Threshold in distance (between time prediction and pseudo position) function for adding covariance to pseudo position measurement. If distance is smaller than this value, no extra covariance is added.
      int32_t k_max_num_consistent_outliers_orth; // [-] Maximum number of consistent pseudo position outlier in orth direction used for pseudo pos meascov outlier countermeasure

      // Uncertainty bias for pseudo position measurement
      float32_t k_pseudo_pos_cov_matrix_bias;              // [m^2] Bias added to pseudo pos variance when estimated edge is visible
      float32_t k_pseudo_pos_cov_matrix_bias_non_movable;  // [m^2] Bias added to pseudo pos variance for non movable objects (with reference point center)

      float32_t k_time_since_init_th_to_enable_outlier_mitigation_cca; // [s] Threshold on minimum time since initialization ob object for enabling the outlier mitigation function for pseudo pos covariance estimation for CCA objects

      // Paramerters for pseudo position in FOVE cases
      float32_t k_pseudo_pos_high_uncertainity;             // [m^2] Pseudo pos meascov when an object is partiallly outside the FOV, such that, a specific position element in TCS (x or y) has close to no impact during msmt update
      float32_t k_fov_normal_rotation_angle;               //  [rad] The angle that describes the rotation of the FOV normals, in order to shrink the FOV

      // Polar uncertainty used in pseudo position measurements
      float32_t k_range_var;                                  // [m^2] Polar pseudo position measurement variance in range direction.
      float32_t k_az_var;                                     // [rad^2] Polar pseudo position measurement variance in azimuth direction.
      float32_t k_raw_pseudo_pos_cov_max_saturation_distance; // [m] Range saturation limit for pseudo measurement variance.

      // Water spray detectors
      float32_t k_ws_min_speed; // [m/s] Min speed of object or host to search for water spray detections
      float32_t k_ws_max_det_rcs; // [dB] Maximum rcs of detection for it to be checked for water spray.

      // Host water spray parameters
      float32_t k_hws_para_box_host_speed_factor; // [1/s] Factor to multiply by speed host to derive box in para direction where to search for water spray detections. The greater the speed the larger the box.
      float32_t k_hws_ortho_box_host_speed_factor; // [1/s] Factor to multiply by speed host to derive box in ortho direction where to search for water spray detections. The greater the speed the larger the box.

      // Object based water spray detector
      float32_t k_ows_min_long_pos; // [m] Min value of VCS zone to search for water spray detections
      float32_t k_ows_max_long_pos; // [m] Max value of VCS zone to search for water spray detections
      float32_t k_ows_max_lat_pos; // [m] Max lateral of VCS zone to search for water spray detections. Symmetric on both sides of host.
      float32_t k_ows_para_box_obj_speed_factor; // [1/s] Factor to multiply by speed host to derive box in para direction where to search for water spray detections. The greater the speed the larger the box.
      float32_t k_ows_ortho_box_obj_speed_factor; // [1/s] Factor to multiply by speed host to derive box in ortho direction where to search for water spray detections. The greater the speed the larger the box.
      float32_t k_ows_range_rate_min_factor; // [-] Factor used for verifying range rate fitness (calculating interval lower limit)
      float32_t k_ows_range_rate_max_factor; // [-] Factor used for verifying range rate fitness (calculating interval upper limit)
      float32_t k_ows_zone_lng_ext_threshold; // [m] Left ceiling for zone area longitudinal extension
      float32_t k_ows_zone_halfwidth_ext_threshold; // [m] Left ceiling for zone area width extension

      // Host vehicle clutter handling
      float32_t max_range_flagging_hvc_dets; // [m] maximum range for flagging host vehicle clutter (HVC) detections as not f_ok_to_use

      // Termination of coasting objects
      float32_t k_max_conf_objtrk_coast_time; // [s] Maximum coasting time for a confirmed track before it is terminated
      float32_t k_max_coast_time_mirror; // [s] Maximum coasting time for a mirror track before it is terminated
      float32_t k_max_coast_time_outside_fov; // [s] Maximum coasting time outside FOV (for front radar only configuration)

      // Object motion thresholds
      float32_t k_object_motion_sigma_ctca_th; // [-] threshold for stationary CTCA object motion clasicication.
      float32_t k_object_motion_min_speed; // [m/s] threshold indicating minimal object speed to treat it as moving.

      int32_t k_object_motion_min_consec_moving_cnt_movable_th; // [-] Threshold indicating minimal count of consecutive scans to treat as moving if it was movable.
      int32_t k_object_motion_min_consec_moving_cnt_th; // [-] Threshold indicating minimal count of consecutive scans to treat as moving
      int32_t k_object_motion_min_consec_moving_cnt_high_yaw_th; // [-] Threshold indicating minimal count of consecutive scans to treat as moving if host yaw rate is high
      float32_t k_object_motion_min_host_yaw_rate_th; // [rad/s] Threshold for treating host as turning

      int32_t k_object_motion_min_consec_stopped; // [-] Threshold indicating minimal count of consecutive scans to treat as stopped
      float32_t k_object_motion_queue_zone_host_stationary_speed_threshold; // [m/s] Speed threshold below which the host is considered to be stationary
      float32_t k_object_motion_queue_zone_long_dist; // [m] Queue zone longitudinal distance - to prevent close-to-host objects from switching to non-moveable
      float32_t k_object_motion_queue_zone_long_dist_host_stationary; // [m] Queue zone longitudinal distance - when host is stationary
      float32_t k_object_motion_queue_zone_lat_dist; // [m] Queue zone lateral distance - to prevent close-to-host objects from switching to non-moveable
      float32_t k_object_motion_max_abs_orient_diff_to_host; // [rad] Maximum allowed absolute orientation diff from the parallel to host direction (0/180deg)
      float32_t k_object_motion_min_consec_stop_time_th; // [s] threshold indicating minimal time interval between two consecutive stops (f_moving from true to false)

      float32_t k_object_motion_min_moving_dets_percentage_th; // [%/100] Minimal percentage of moving detections to treat object as moving
      float32_t k_object_motion_parallel_moving_heading_th; // [rad] Maximal object vcs heading to analyse it in parallel check
      float32_t k_object_motion_parallel_moving_speed_diff_th; // [m/s] Maximal absolute velocity diff between object and host
      float32_t k_object_motion_parallel_moving_lat_posn_th; // [m] Maximum absolute vcs lateral position of track to treat is as parallel moving
      float32_t k_object_motion_parallel_moving_lon_posn_th; // [m] Maximum absolute vcs longitudinal position of track to treat is as parallel moving

      float32_t k_object_motion_nees_min_p_value_th; // [-] Minimal P value used to reject stationary NEES hypothesis
      float32_t k_object_motion_nees_max_test_variable; // [-] Maximum test variable
      float32_t k_object_motion_nees_range_scaling_factor; // [-] Factor used to lower threshold of p value to accept stationary NEES hypothesis

      float32_t k_object_motion_cross_moving_min_abs_vcs_heading_th; // [rad] Minimal absolute object vcs heading to analyse it as cross moving
      float32_t k_object_motion_cross_moving_max_abs_vcs_azimuth_th; // [rad] Maximal absolute object vcs azimuth to analyse it as cross moving

      float32_t k_object_motion_occluded_speed_scale_factor; // [-] Scaling factor used to increase the occluded cross moving weight
      float32_t k_occlusion_zone_long_left_rear_stationary_check;  // [m] Longitudinal position of rear left corner of occlusion zone used for determining moving speed threshold in motion classification
      float32_t k_occlusion_zone_lat_left_rear_stationary_check;  // [m] Lateral position of rear left corner of occlusion zone used for determining moving speed threshold in motion classification
      float32_t k_occlusion_zone_long_right_front_stationary_check; // [m] Longitudinal position of front right corner of occlusion zone used for determining moving speed threshold in motion classification
      float32_t k_occlusion_zone_lat_right_front_stationary_check; // [m] Lateral position of front right corner of occlusion zone used for determining moving speed threshold in motion classification


      // Initialization
      uint16_t k_init_min_num_dets_from_outside_restrictive_zone; // [-] Minimal sufficient number of detections comming from the outside of the restrictive zone, on the sides of the host, required for stationary hypothesis in initialization
      uint16_t k_init_min_num_dets_from_restrictive_zone; // [-] Minimal sufficient number of detections comming from the inside of the restrictive zone, on the sides of the host, required for stationary hypothesis in initialization

      float32_t k_init_fast_moving_upper_threshold; // [m/s] Upper speed threshold under which the object is considered as fast moving during initialization
      float32_t k_init_movable_prob_threshold; // [-] Probability threshold under which the object is considered as nonmovable during initialization
      float32_t k_init_default_confidence; // [-] Initial confidence for a new object.


      // Object size estimation
      float32_t k_movable_max_target_width; // [m] Max width of movable objects classified as vehicular track
      float32_t k_fast_movable_max_target_length; // [m] Max length of movable objects classified as vehicular track
      float32_t k_slow_movable_max_target_length; // [m] Max length of movable objects not classified as vehicular track
      float32_t k_min_CTCA_target_length;      // [m] Min length of movable objects
      float32_t k_nonmoveable_target_diameter;    // [m] diameter of non-moveable objects
      float32_t k_min_aspect_ratio; // [-] Min ratio between length and width
      float32_t k_max_aspect_ratio; // [-] Max ratio between length and width

      // Calibration variables for object size shrink logic. Commonly used in both track grouping (merge) and size update algorithms
      float32_t k_object_shrinking_speed_threshold; // [m/s] Object speed at which to start shrinking object size
      float32_t k_max_length_for_slow_moving_objects; // [m] Maximum length of objects that are driving with speed below calibrations.k_object_shrinking_speed_threshold. If objects are longer they will be shrunken until reaching this length (or smaller)
      float32_t k_min_length_for_slow_moving_objects; // [m] Minimum length of objects that are driving with speed below calibrations.k_object_shrinking_speed_threshold. Minimum length to which to shrink objects.
      float32_t k_speed_for_max_length_of_slow_moving_objects; // [m/s] Object speed threshold for which to start to ramp down the maximum allowed objects size from calibrations.k_max_length_for_slow_moving_objects towards calibrations.k_min_length_for_slow_moving_objects
      float32_t k_speed_for_min_length_of_slow_moving_objects; // [m/s] Object speed threshold for when maximum allowed objects length should be calibrations.k_min_length_for_slow_moving_objects;

      // Object size update
      float32_t k_size_update_min_det_range;                           // [m] Minimum range of closest detection to allow size update of non-visible side
      float32_t k_size_update_base_measurement_uncertainty;            // [m^2] Default measurement variance used for both length and width KF update
      float32_t k_size_update_base_process_noise;                      // [m^2] Default process noise for both length and width used in the KF update
      float32_t k_size_update_speed_threshold_low_speed_process_noise; // [m/s] Upper speed threshold to use low speed process noise in KF update for both length and width
      float32_t k_size_update_low_speed_process_noise;                 // [m^2] Process noise for slow moving objects used in both length and width KF update
      float32_t k_size_update_min_speed_to_update_nonvisible_side;     // [m/s] Minimum speed threshold to allow size update of non-visible side
      float32_t k_size_update_process_noise_pruning;                   // Factor to make filter slower by reducing process noise when estimated side is not visible
      uint32_t k_min_num_dets_to_decrease_meas_uncertainty;         // [-] Minimum number of associated detections to allow decrease in measurement uncertainty

      // Confidence overall
      float32_t k_conf_overall_timeout_to_start_decay; // [s] time a track may coast before the state confidence starts to decay.

      float32_t k_conf_overall_difference_thresh_long_posn_h; // [m] High difference threshold. Used to in confidence state machine to manipulate internal state confidence.
      float32_t k_conf_overall_difference_thresh_long_posn_m; // [m] Medium difference threshold. Used to in confidence state machine to manipulate internal state confidence.
      float32_t k_conf_overall_difference_thresh_long_posn_l; // [m] Low difference threshold. Used to in confidence state machine to manipulate internal state confidence.
      float32_t k_conf_overall_difference_thresh_long_posn_vl; // [m] Very Low difference threshold. Used to in confidence state machine to manipulate internal state confidence.
      float32_t k_conf_overall_difference_thresh_lat_posn_h; // [m] High difference threshold. Used to in confidence state machine to manipulate internal state confidence.
      float32_t k_conf_overall_difference_thresh_lat_posn_m; // [m] Medium difference threshold. Used to in confidence state machine to manipulate internal state confidence.
      float32_t k_conf_overall_difference_thresh_lat_posn_l; // [m] Low difference threshold. Used to in confidence state machine to manipulate internal state confidence.
      float32_t k_conf_overall_difference_thresh_lat_posn_vl; // [m] Very Low difference threshold. Used to in confidence state machine to manipulate internal state confidence.

      float32_t k_conf_cca_overall_difference_thresh_speed_h; // [m/s] High difference threshold. Used to in confidence state machine to manipulate CCA internal state confidence.
      float32_t k_conf_cca_overall_difference_thresh_speed_m; // [m/s] Medium difference threshold. Used to in confidence state machine to manipulate CCA internal state confidence.
      float32_t k_conf_cca_overall_difference_thresh_speed_l; // [m/s] Low difference threshold. Used to in confidence state machine to manipulate CCA internal state confidence.
      float32_t k_conf_cca_overall_difference_thresh_speed_vl; // [m/s] Very Low difference threshold. Used to in confidence state machine to manipulate CCA internal state confidence.

      float32_t k_conf_ctca_overall_difference_thresh_speed_h; // [m/s] High difference threshold. Used to in confidence state machine to manipulate CTCA internal state confidence.
      float32_t k_conf_ctca_overall_difference_thresh_speed_m; // [m/s] Medium difference threshold. Used to in confidence state machine to manipulate CTCA internal state confidence.
      float32_t k_conf_ctca_overall_difference_thresh_speed_l; // [m/s] Low difference threshold. Used to in confidence state machine to manipulate CTCA internal state confidence.
      float32_t k_conf_ctca_overall_difference_thresh_speed_vl; // [m/s] Very Low difference threshold. Used to in confidence state machine to manipulate CTCA internal state confidence.

      // Unreliable low confidence track
      float32_t k_low_conf_unreliability_max_ttc; // [s] Maximum time to cross to consider not downselecting a target based on overall confidence.
      float32_t k_low_conf_unreliability_age_thr; // [s] Age of an object to be considered mature in downselection based on overall confidence logic
      float32_t k_conf_downselection_exclusion_box_lat; // [m] Lateral exclusion zone where tracks will be downselected regardless of overall confidence level.
      float32_t k_conf_downselection_exclusion_box_long; // [m] Longitudinal exclusion zone where tracks will be downselected regardless of overall confidence level.
      float32_t k_low_conf_unreliability_min_heading; // [RAD] Minimum heading for excluding from exclusion zone (so to include object for test for overall confidence).
      float32_t k_low_conf_max_allowed_host_speed_in_cta_scenarios; // [m/s] Track Downselection module: Maximum allowed host speed in CTA scenarios
      float32_t k_low_conf_expected_abs_object_heading_vcs_in_cta_scenarios; // [rad] Track Downselection module: Expected absolute object heading (vcs) in CTA scenarios
      float32_t k_low_conf_max_allowed_abs_heading_difference_in_cta_scenarios; // [rad] Track Downselection module: Maximum allowed absolute object heading difference in CTA scenarios

      // Longi stat curves
      float32_t k_lsc_min_long_pos; // [m] Minimum longitudinal position of objects to be used by longi stat curves.
      float32_t k_lsc_max_long_pos; // [m] Maximum longitudinal position of objects to be used by longi stat curves.
      float32_t k_lsc_long_pos_gate; // [m] Maximum distance between objects in longitudinal direction to cluster objects to be used by longi stat curves
      float32_t k_lsc_lat_pos_gate; // [m] Maximum absolute distance between objects in lateral direction to cluster objects to be used by longi stat curves
      uint32_t k_lsc_min_points_in_cluster; // [-] Minimum objects in a cluster to be eligible to fit polynomial to. NOTE: This must be >= LSC_NR_POLY_COEFF_SLOTS

      float32_t k_lsc_lat_merging_gate; // [m] Crude gate in lateral direction to try merging two clusters
      float32_t k_lsc_long_merging_gate; // [m] Crude gate in longitudinal direction to try merging two clusters
      float32_t k_lsc_cluster_merge_thr; // [m] Gate on how close two clusters' end points are to execute a merge of the two clusters
      float32_t k_lsc_length_score_gain; // [-] Gain factor on the longitudinal spread of a cluster to boost a long curves prio. Calculated as: score =  k_lsc_length_score_gain * (1 / (x_max-x_min))
      float32_t k_lsc_max_a_coeff; // [-] Threshold on "a" coefficient of polynomial to sanity check curves. Too high a value indicates bad longitudinal distribution of cluster and curve is not valid
      float32_t k_distance_to_circle_thr; // [m]  Minimum distance to the host projected path where the guard rail is allowed to be created

      // Static Environment Polynomials
      float32_t k_sep_p2_coeff_poly_linear_thr; // [1/m] Threshold on "p2" coefficient of polynomial to treat the polynomial as a linear line
      float32_t k_sep_max_k_coeff_for_lateral_line; // [-] Maximum slope of line to consider as only extending in lateral direction
      float32_t k_sep_det_on_poly_thr; // [m] Distance threshold for a detection to be flagged as "on" static environment polynomial
      float32_t k_sep_obj_on_poly_thr; // [m] Distance threshold for an object's centroid to be flagged as "on" static environment polynomial

      // Object class determination
      float32_t k_ad_oc_mean_length_pedestrian; // [m] Assumed mean length of pedestrian used to calculate PDF value
      float32_t k_ad_oc_inv_standard_deviation_length_pedestrian; // [1/m] Inverse of assumed mean standard deviation of pedestrian length used to calculate PDF value (inverse is used for code optimization)

      float32_t k_ad_oc_mean_length_2wheel; // [m] Assumed mean length of 2wheel (bicycle, motorcycle) used to calculate PDF value
      float32_t k_ad_oc_inv_standard_deviation_length_2wheel; // [1/m] Inverse of assumed mean standard deviation of 2wheel (bicycle, motorcycle) length used to calculate PDF value (inverse is used for code optimization)

      float32_t k_ad_oc_mean_length_car; // [m] Assumed mean length of car used to calculate PDF value
      float32_t k_ad_oc_inv_standard_deviation_length_car; // [1/m] Inverse of assumed mean standard deviation of car length used to calculate PDF value (inverse is used for code optimization)

      float32_t k_ad_oc_mean_length_truck; // [m] Assumed mean length of truck used to calculate PDF value
      float32_t k_ad_oc_inv_standard_deviation_length_truck; // [m] Inverse of assumed mean standard deviation of truck length used to calculate PDF value (inverse is used for code optimization)

      float32_t k_ad_oc_step_decrease_prob_unknown; // [-] Value used to lower object undetermined probability in several cycles after its creation

      float32_t k_ad_oc_max_stationary_speed; // [m/s] Maximum speed for an object to be considered as stationary
      float32_t k_ad_oc_prob_decrease; // [-] Factor for decreasing probability of object being of a given class

      // Predict existence probability
      float32_t k_p_persist_outside_long_range; // [-] Default persistence probability of a track outside the long look range of the sensor
      float32_t k_p_persist_inside_long_range; // [-] Default persistence probability of a track inside the long look range of the sensor

      // Bistatic
      float32_t k_bistatic_cond_assoc_area_min_lat; // [m] Minimal lateral position of bistatic detection to analyse it whether it is okay to use
      float32_t k_bistatic_cond_assoc_area_max_lat; // [m] Maximal lateral position of bistatic detection to analyse it whether it is okay to use
      float32_t k_bistatic_cond_assoc_area_min_lon; // [m] Minimal longitudinal position of bistatic detection to analyse it whether it is okay to use
      float32_t k_bistatic_cond_assoc_area_max_lon; // [m] Maximal longitudinal position of bistatic detection to analyse it whether it is okay to use
      float32_t k_bistatic_lat_th_extension; // [m] Lateral threshold extension used to accept bistatic detections as okay to use

      // Host mirror track
      float32_t k_host_refl_min_obj_long_pos; // [m] Minimal longitudinal position of object to be analysed whether it is a host mirror track
      float32_t k_host_refl_max_obj_long_pos; // [m] Maximal longitudinal position of object to be analysed whether it is a host mirror track

      float32_t k_host_refl_half_host_length; // [m] Assumed length of host length divided by two

      float32_t k_host_refl_bbox_lat_ext; // [m] Lateral extension of object bbox
      float32_t k_host_refl_bbox_long_ext; // [m] Longitudinal extension of object bbox

      float32_t k_host_refl_filtering_distance; // [m] Longitudinal distance for filtering SEPs crossing host's path

      float32_t k_host_refl_lowspeed_host_speed_th; // [m/s] Maximum host speed to use thresholds defined for slow moving host
      float32_t k_host_refl_lowspeed_speed_diff_th; // [m/s] Maximum diff of host vs object speed when host is slow moving
      float32_t k_host_refl_lowspeed_heading_th; // [rad] Maximum heading of object when host is slowly moving

      float32_t k_host_refl_highspeed_heading_th; // [rad] Maximum heading of object when host is fast moving
      float32_t k_host_refl_highspeed_min_speed_diff_th; // [m/s] Minimal object vs host speed diff when host is fast moving
      float32_t k_host_refl_highspeed_speed_diff_ramp_coef; // [-] Scaling factor used to increase maximum host vs object speed diff
      float32_t k_host_refl_highspeed_max_speed_diff_th; // [m/s] Maximal object vs host speed diff when host is fast moving

      float32_t k_ws_bbox_len_extension_factor; // [-] length extension factor for calculating extended bounding box for water spray detections
      float32_t k_ws_bbox_wid_extension_factor; // [-] width extension factor for calculating extended bounding box for water spray detections

      // Object confidence and raw confidence level
      float32_t k_conf_updated_tracks_filter_const; // [-] Constant used to determine lowpass filter coefficents for objects that were recently updated.
      float32_t k_conf_raw_weight_dets; // [-] Coefficient used to calculate raw confidence level of updated tracks. It determines impact of number of dets on raw confidence level
      float32_t k_conf_raw_max_value_not_reduced_dets; // [-] Max value of raw confidence when object has only associated, but no reduced detections

      float32_t k_conf_coasted_min_time_since_init; // [s] Minimal time since coasted track initialization to use medium confidence coefficients
      float32_t k_conf_coasted_min_time_trusted_track; // [s] Minimum time since coasted track initialization to consider it as tructed track
      float32_t k_conf_coasted_min_long_posn_tructed_track; // [m] Minimum longitudinal distance of object to consider it as trusted track
      float32_t k_conf_coasted_max_long_posn_tructed_track; // [m] Maximum longitudinal distance of object to consider it as trusted track
      float32_t k_conf_coasted_min_average_confidence_level; // [-] Minimum average confidence level of object to use medium confidence coefficients

      // Flagging of azimuth range rate outliers
      float32_t k_az_rdot_max_sq_dist; // [m^2] Maximum squared distance of object from approximated host center to consider flagging detection as range rate outliers
      float32_t k_az_rdot_max_az_diff; // [rad] Maximum spread in azimuth between detections to flag range rate outliers
      float32_t k_az_rdot_min_rdot_diff; // [m/s] Minimum deviation from velocity profile for a detection to be flagged as a range rate outlier

      // Dead zone
      float32_t k_dead_zone_min_host_speed; // [m/s] Minimal host speed to mark objects entering dead zone
      float32_t k_dead_zone_max_obj_vcs_lat_pos; // [m] Maximal object vcs lateral position
      float32_t k_dead_zone_max_obj_vcs_heading; // [m] Maximal object vcs lateral position
      float32_t k_dead_zone_max_rel_vel_diff; // [-] <0, 1> Maximal relative host and object velocity diff to extend association gates
      float32_t k_dead_zone_assoc_gates_additional_enhacementl; // [m] Additional enhancement used to increase longitudinal association gates of object
      float32_t k_dead_zone_long_limit_extension; // [m] Extension for calculating limit of extended dead zone.
      bool k_use_dead_zone_in_stationkeeping_scenarions; // [-] Switch used to turn on marking tracks as in dead zone
            
      // Split logic
      float32_t k_orth_split_min_speed; // [m/s] Minimum speed of an object to perform a split. Should be greater or equal to calibration k_min_speed_for_updating_heading
      float32_t k_orth_split_orth_delta_filter_const; // [-] Low pass filter constant for filtered max delta/spread distance between associated detections of an object.
      float32_t k_orth_split_orth_gap_filter_prop_const; // [-] Proportionality constant that is multiplied with number of associated detections to compute low pass filter constant for filtered max gap distance between associated detections of an object.
      float32_t k_range_rate_diff_saturation_const; // [m/s] Value to which we saturate range-rate difference between left and right detections of an object before it goes to low-pass filter.
      uint32_t k_orth_split_orth_gap_filter_max_dets; // [-] Maximum number of detections that is allowed to determine filter constant for max gap between associated detections in orth direction.
      float32_t k_orth_split_min_orth_gap_for_split_high; // [m] Threshold on objects low pass filtered max detection gap in orthogonal direction value. Signal over this level triggers an orthogonal split regardless of range-rate difference.
      float32_t k_orth_split_min_orth_gap_for_split_short_objects; // [m] Threshold on objects low pass filtered max detection gap in orthogonal direction value. Signal over this level triggers an orthogonal split given sufficient range-rate difference and object length below k_orth_split_max_length_short_object.
      float32_t k_orth_split_min_orth_gap_for_split_medium; // [m] Threshold on objects low pass filtered max detection gap in orthogonal direction value. Signal over this level triggers an orthogonal split given sufficient range-rate difference.
      float32_t k_orth_split_min_orth_gap_for_split_low; // [m] Threshold on objects low pass filtered max detection gap in orthogonal direction value. Signal over this level triggers an orthogonal split given sufficient range-rate difference and certain conditions.
      float32_t k_orth_split_min_rr_diff_for_short_object_orth_gap; // [m/s] Threshold on objects low pass filtered difference between range rates on each side of an object given short object orth_gap_filtered.
      float32_t k_orth_split_min_rr_diff_for_medium_orth_gap; // [m/s] Threshold on objects low pass filtered difference between range rates on each side of an object given medium orth_gap_filtered.
      float32_t k_orth_split_min_rr_diff_for_low_orth_gap; // [m/s] Threshold on objects low pass filtered difference between range rates on each side of an object given low orth_gap_filtered and certain conditions.
      float32_t k_orth_split_min_distance_sq_for_low_orth_gap; // [m] Minimum distance limit where we allow to split an object given lower orth_gap and higher range-rate difference.
      float32_t k_orth_split_max_distance_sq_for_low_orth_gap; // [m] Maximum distance limit where we allow to split an object given lower orth_gap and higher range-rate difference.
      float32_t k_orth_split_max_length_for_low_orth_gap; // [m] Maximum length of an object where we allow to split an object given lower orth_gap and higher range-rate difference.
      float32_t k_orth_split_width_gain; // [-] Gain to determine the updated width after split based on the objects property "orth_delta_filtered"
      float32_t k_orth_split_max_distance_sq; // [m^2] Squared distance limit from host center where we start updating the objects filtered split signals. If object is further away the split signals are reset.
      float32_t k_orth_split_min_distance_sq; // [m^2] Squared distance limit from host center where we start freezing the objects filtered split signals. If objects are closer the the split signals are frozen.
      float32_t k_pos_delta_heading_filter_constant; // [-] Low pass filter constant for filtering an objects heading solely derived from centroid position change between scans
      float32_t k_orth_split_max_length_short_object; // [m] Maximum length for object to be allowed to split using short object gap and range-rate difference.

      // Multi Path detector - low level logic
      float32_t k_mp_object_reflector_size_extension;   // [m] object type reflector extension in size

      // Multi Path detector - high level logic
      float32_t k_mp_max_allowed_host_speed_to_use_MP;      // [m/s] maximum allowed host speed, when checking if object is multipath.
      float32_t k_mp_default_mirror_probability;      // [-] default mirror probability that is assigned to object classified as multipath

      // Multi Path detector - reflector selector
      float32_t k_mp_half_long_zone;     // [m] Half of longitudinal size of outer zone where MP detector works
      float32_t k_mp_half_lat_zone;      // [m] Half of lateral size of outer zone where MP detector works

      // Average object RCS value
      float32_t k_average_rcs_filter_constant; // [-] Low pass filter constant for filtered average object RCS value

      // Trailer
      float32_t k_trailer_distance_rear_axle_to_tow_hitch; // [m] Distance between rear axle to to hitch
      float32_t k_trailer_bbox_extend_length;                   // [m] Extend length for the trailer bounding box

      // Commercial vehicle trailer
      float32_t k_cvt_trailer_width;  // [m] Assumed trailer width
      float32_t k_cvt_reversing_speed_threshold;  // [m/s] Speed below which the vehicle is considered to be reversing
      float32_t k_cvt_slow_speed_threshold;  // [m/s] Speed below which the vehicle is considered to be stationary

      // Overall Confidence Blocker
      uint8_t k_ocb_cnt_delta_midlow_rcs; // [-] Amount to change counter
      uint8_t k_ocb_cnt_delta_low_rcs_or_mult_dets; // [-] Amount to change counter
      uint8_t k_ocb_cnt_max; // [-] Max count that is allowed
      float32_t k_ocb_rcs_thresh_midlow_rcs; // [dBsm] Detection RCS threshold to increase count
      float32_t k_ocb_rcs_thresh_low_rcs; // [dBsm] Secondary detection RCS threshold to increase count
      float32_t k_ocb_rcs_thresh_hi_rcs; // [dBsm] Secondary detection RCS threshold to decrease count
      float32_t k_ocb_max_range; // [m] Max detection range to pass check
      float32_t k_ocb_max_range_rate; // [m] Max detection range to pass check

      // Parameters for Detect_Near_By_Wheel_Spins
      uint16_t k_nbws_max_num_clusters; // [m] max number of clusters to be checked

      bool hide_tracks_outside_guardrail;


      // CCA filter tunings
      float32_t k_cca_low_speed_th_to_ramp_down_proceess_noise; // [m/s] For speeds below this the CCA object obtains the slowest process noise tuning in KF time update.
      float32_t k_cca_high_speed_th_to_ramp_down_proceess_noise; // [m/s] For speeds aboce this the CCA object obtains the fastest process noise tuning in KF time update.
      float32_t q_cca_pos_low_speed; // [m^2] Time continous process noise variance on position state in object para direction. Applied to slow moving CCA objects in KF time update.
      float32_t q_cca_vel_low_speed; // [(m/s)^2] Time continous process noise variance on velocity state in object para direction. Applied to slow moving CCA objects in KF time update.
      float32_t q_cca_acc_low_speed; // [(m/s^2)^2] Time continous process noise variance on acceleration state in object para direction. Applied to slow moving CCA objects in KF time update.
      float32_t init_cca_pnt_filter_cov[2][2]; // Initial state covariance matrix for CCA pointing yaw rate filter
      float32_t k_cca_min_speed_to_update_pnt; // [m/s] Threshold for minumum speed of CCA object to update the pointing of the object

      // Detection double bounce detector
      float32_t k_db_max_range; // [m] Maximum range of detection to check it for double bounce
      uint32_t k_db_max_nr_multi_bounces; // [-] Maximum number of bounces to consider
      float32_t k_db_range_threshold_frac; // [-] Fraction of secondary detection range to consider as threshold for double bounce. Greater range gives a more generous range gate to satisfy double bounce range condition
      float32_t k_db_min_range_threshold; // [m] Minimum range tolerance limit relative predicted range
      float32_t k_db_max_range_threshold; // [m] Maximum range tolerance limit relative predicted range
      float32_t k_db_range_rate_threshold; // [m/s] Range rate threshold for considering if a detection is double bounce
      float32_t k_db_azimuth_thres_k; // [1/m] K value for deriving azimuth threshold as a linear function of range
      float32_t k_db_azimuth_thres_m; // [m] M value for deriving azimuth threshold as a linear function of range
      float32_t k_db_min_azimuth_thres; // [rad] Min azimuth threshold for double bounce
      float32_t k_db_max_azimuth_thres; // [rad] Max azimuth threshold for double bounce

      // Mulitpath detection filter
      float32_t k_min_host_speed_for_bad_az_filter;           // [m/s] Minimum speed of host to filter out detections with bad azimuth if fraction is large enough.
      float32_t k_max_fraction_of_bad_azimuth_dets_default;   // [-] Default max fraction of detections with worst azimuth confidence from a sensor to allow usage of these detections.
      float32_t k_max_fraction_of_bad_azimuth_dets_srr5;      // [-] Max fraction of detections with worst azimuth confidence from a SRR5 sensor to allow usage of these detections.
      uint32_t k_min_num_valid_dets_for_bad_az_filter;        // [-] Minimum number of detections per sensor to filter out detections with bad azimuth if fraction is large enough.

      // Mark MRR3 detections high elevation
      float32_t k_min_host_speed_for_check_det_az_conf_and_elevation;  // [m/s] Sensor preprocessing module: Minimum host speed for checking detection azimuth confidence and elevation
      float32_t k_mrr3_max_range;                                      // [m] Sensor preprocessing module: maximum range of MRR3 sensor
      float32_t k_mrr3_max_abs_elev_angle;                             // Sensor preprocessing module: maximum absolute value of elevation angle for MRR4 detection
      uint8_t k_mrr3_conf_thresh;                                      // [-] Sensor preprocessing module: confidence threshold for MRR3 detection

      // Obstacle probability
      float32_t k_obstacle_prob_min_bbox_height_nonmovable; // [m] Minimum bounding box height for non-movable objects
      float32_t k_obstacle_prob_min_bbox_height_moving; // [m] Minimum bounding box height for moving objects
   } F360_Calibrations_T;

   void Initialize_Tracker_Calibrations(F360_Calibrations_T& calibrations);
}
#endif
