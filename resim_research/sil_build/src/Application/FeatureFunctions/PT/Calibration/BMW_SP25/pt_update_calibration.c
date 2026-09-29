/**
* @file pt_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in pt_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "pt_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "pt_core_calibration_check.h"
#include "pt_core_calibration_t.h"
#include "pt_customer_calibration_t.h"
#include "pt_public_calibration_check.h"
#include "pt_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Pt_Update_Core_Cal_By_Public(Pt_Core_Calibration_T* cal_dst, const Pt_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Pt_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_pt_zone_max_posn = cal_src->k_pt_zone_max_posn;
        cal_dst->k_pt_min_obj_speed = cal_src->k_pt_min_obj_speed;
        cal_dst->k_pt_overlap_max_match_value = cal_src->k_pt_overlap_max_match_value;
        cal_dst->k_pt_move_max_value = cal_src->k_pt_move_max_value;
        cal_dst->k_pt_group_max_match_value = cal_src->k_pt_group_max_match_value;
        cal_dst->k_pt_group_max_match_value_ad = cal_src->k_pt_group_max_match_value_ad;
        cal_dst->k_pt_group_dir_max_diff_value = cal_src->k_pt_group_dir_max_diff_value;
        cal_dst->k_pt_group_dir_max_avg_diff_value = cal_src->k_pt_group_dir_max_avg_diff_value;
        cal_dst->k_pt_find_max_lat_posn = cal_src->k_pt_find_max_lat_posn;
        cal_dst->k_pt_find_min_speed = cal_src->k_pt_find_min_speed;
        cal_dst->k_pt_find_max_long_posn = cal_src->k_pt_find_max_long_posn;
        cal_dst->k_pt_path_change_match_hyst_default = cal_src->k_pt_path_change_match_hyst_default;
        cal_dst->k_pt_path_change_differing_states_hyst_default = cal_src->k_pt_path_change_differing_states_hyst_default;
        cal_dst->k_pt_path_change_match_hyst_more_established = cal_src->k_pt_path_change_match_hyst_more_established;
        cal_dst->k_pt_path_change_one_grouped_one_mature = cal_src->k_pt_path_change_one_grouped_one_mature;
        cal_dst->k_pt_path_change_one_grouped_one_creation = cal_src->k_pt_path_change_one_grouped_one_creation;
        cal_dst->k_pt_kill_path_exceed_dist_thres = cal_src->k_pt_kill_path_exceed_dist_thres;
        cal_dst->k_pt_kill_path_max_diff_posn = cal_src->k_pt_kill_path_max_diff_posn;
        cal_dst->k_pt_lower_lim_obj_orient_lat = cal_src->k_pt_lower_lim_obj_orient_lat;
        cal_dst->k_pt_upper_lim_obj_orient_lat = cal_src->k_pt_upper_lim_obj_orient_lat;
        cal_dst->k_pt_apply_move_point_min_speed = cal_src->k_pt_apply_move_point_min_speed;
        cal_dst->k_pt_apply_move_point_min_yaw_rate = cal_src->k_pt_apply_move_point_min_yaw_rate;
        cal_dst->k_pt_move_point_yaw_rate_thres_calc_ego_shift = cal_src->k_pt_move_point_yaw_rate_thres_calc_ego_shift;
        cal_dst->k_pt_group_paths_min_interval_dist = cal_src->k_pt_group_paths_min_interval_dist;
        cal_dst->k_pt_path_track_long_range_limit = cal_src->k_pt_path_track_long_range_limit;
        cal_dst->k_pt_path_track_lat_range_limit = cal_src->k_pt_path_track_lat_range_limit;
        cal_dst->k_pt_group_overlap_paths_min_diff = cal_src->k_pt_group_overlap_paths_min_diff;
        cal_dst->k_pt_en_algo_min_vel_inactive = cal_src->k_pt_en_algo_min_vel_inactive;
        cal_dst->k_pt_en_algo_max_val_active = cal_src->k_pt_en_algo_max_val_active;
        cal_dst->k_pt_point_diff_weighting_factor_lut[0] = cal_src->k_pt_point_diff_weighting_factor_lut[0];
        cal_dst->k_pt_point_diff_weighting_factor_lut[1] = cal_src->k_pt_point_diff_weighting_factor_lut[1];
        cal_dst->k_pt_point_diff_weighting_factor_lut[2] = cal_src->k_pt_point_diff_weighting_factor_lut[2];
        cal_dst->k_pt_point_diff_weighting_factor_lut[3] = cal_src->k_pt_point_diff_weighting_factor_lut[3];
        cal_dst->k_pt_point_diff_weighting_factor_lut[4] = cal_src->k_pt_point_diff_weighting_factor_lut[4];
        cal_dst->k_pt_default_range_of_tracking_zone = cal_src->k_pt_default_range_of_tracking_zone;
        cal_dst->k_pt_min_exist_prob_to_be_valid = cal_src->k_pt_min_exist_prob_to_be_valid;
        cal_dst->k_pt_dist_obj_to_border_conf_lut[0] = cal_src->k_pt_dist_obj_to_border_conf_lut[0];
        cal_dst->k_pt_dist_obj_to_border_conf_lut[1] = cal_src->k_pt_dist_obj_to_border_conf_lut[1];
        cal_dst->k_pt_dist_obj_to_border_conf_lut[2] = cal_src->k_pt_dist_obj_to_border_conf_lut[2];
        cal_dst->k_pt_dist_obj_to_border_conf_lut[3] = cal_src->k_pt_dist_obj_to_border_conf_lut[3];
        cal_dst->k_pt_dist_border_to_isect_conf_lut[0] = cal_src->k_pt_dist_border_to_isect_conf_lut[0];
        cal_dst->k_pt_dist_border_to_isect_conf_lut[1] = cal_src->k_pt_dist_border_to_isect_conf_lut[1];
        cal_dst->k_pt_dist_border_to_isect_conf_lut[2] = cal_src->k_pt_dist_border_to_isect_conf_lut[2];
        cal_dst->k_pt_dist_border_to_isect_conf_lut[3] = cal_src->k_pt_dist_border_to_isect_conf_lut[3];
        cal_dst->k_pt_dist_obj_to_path_lut[0] = cal_src->k_pt_dist_obj_to_path_lut[0];
        cal_dst->k_pt_dist_obj_to_path_lut[1] = cal_src->k_pt_dist_obj_to_path_lut[1];
        cal_dst->k_pt_dist_obj_to_path_lut[2] = cal_src->k_pt_dist_obj_to_path_lut[2];
        cal_dst->k_pt_dist_obj_to_path_lut[3] = cal_src->k_pt_dist_obj_to_path_lut[3];
        cal_dst->k_pt_dist_obj_to_path_lut[4] = cal_src->k_pt_dist_obj_to_path_lut[4];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[0] = cal_src->k_pt_dist_obj_to_path_conf_lut[0];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[1] = cal_src->k_pt_dist_obj_to_path_conf_lut[1];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[2] = cal_src->k_pt_dist_obj_to_path_conf_lut[2];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[3] = cal_src->k_pt_dist_obj_to_path_conf_lut[3];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[4] = cal_src->k_pt_dist_obj_to_path_conf_lut[4];
        cal_dst->k_pt_similarity_trail_path_lut[0] = cal_src->k_pt_similarity_trail_path_lut[0];
        cal_dst->k_pt_similarity_trail_path_lut[1] = cal_src->k_pt_similarity_trail_path_lut[1];
        cal_dst->k_pt_similarity_trail_path_lut[2] = cal_src->k_pt_similarity_trail_path_lut[2];
        cal_dst->k_pt_similarity_trail_path_lut[3] = cal_src->k_pt_similarity_trail_path_lut[3];
        cal_dst->k_pt_similarity_trail_path_lut[4] = cal_src->k_pt_similarity_trail_path_lut[4];
        cal_dst->k_pt_similarity_trail_path_conf_lut[0] = cal_src->k_pt_similarity_trail_path_conf_lut[0];
        cal_dst->k_pt_similarity_trail_path_conf_lut[1] = cal_src->k_pt_similarity_trail_path_conf_lut[1];
        cal_dst->k_pt_similarity_trail_path_conf_lut[2] = cal_src->k_pt_similarity_trail_path_conf_lut[2];
        cal_dst->k_pt_similarity_trail_path_conf_lut[3] = cal_src->k_pt_similarity_trail_path_conf_lut[3];
        cal_dst->k_pt_similarity_trail_path_conf_lut[4] = cal_src->k_pt_similarity_trail_path_conf_lut[4];
        cal_dst->k_pt_weight_of_last_trail_point = cal_src->k_pt_weight_of_last_trail_point;
        cal_dst->k_pt_weight_of_sec_last_trail_point = cal_src->k_pt_weight_of_sec_last_trail_point;
        cal_dst->k_pt_heading_diff_lut[0] = cal_src->k_pt_heading_diff_lut[0];
        cal_dst->k_pt_heading_diff_lut[1] = cal_src->k_pt_heading_diff_lut[1];
        cal_dst->k_pt_heading_diff_lut[2] = cal_src->k_pt_heading_diff_lut[2];
        cal_dst->k_pt_heading_diff_lut[3] = cal_src->k_pt_heading_diff_lut[3];
        cal_dst->k_pt_heading_diff_conf_lut[0] = cal_src->k_pt_heading_diff_conf_lut[0];
        cal_dst->k_pt_heading_diff_conf_lut[1] = cal_src->k_pt_heading_diff_conf_lut[1];
        cal_dst->k_pt_heading_diff_conf_lut[2] = cal_src->k_pt_heading_diff_conf_lut[2];
        cal_dst->k_pt_heading_diff_conf_lut[3] = cal_src->k_pt_heading_diff_conf_lut[3];
        cal_dst->k_pt_weight_heading_diff_confidence[0] = cal_src->k_pt_weight_heading_diff_confidence[0];
        cal_dst->k_pt_weight_heading_diff_confidence[1] = cal_src->k_pt_weight_heading_diff_confidence[1];
        cal_dst->k_pt_weight_heading_diff_confidence[2] = cal_src->k_pt_weight_heading_diff_confidence[2];
        cal_dst->k_pt_min_confidence_valid_match = cal_src->k_pt_min_confidence_valid_match;
        cal_dst->k_pt_dist_betw_paths_similarity_matching = cal_src->k_pt_dist_betw_paths_similarity_matching;
        cal_dst->k_pt_host_implausibilty_range = cal_src->k_pt_host_implausibilty_range;
        cal_dst->k_pt_trail_max_speed_trail_to_path_conv = cal_src->k_pt_trail_max_speed_trail_to_path_conv;
        cal_dst->k_pt_minimum_host_trail_length = cal_src->k_pt_minimum_host_trail_length;
        cal_dst->k_pt_max_heading_diff_valid_interval = cal_src->k_pt_max_heading_diff_valid_interval;
        cal_dst->k_pt_f_apply_move_point = cal_src->k_pt_f_apply_move_point;
        cal_dst->k_pt_f_check_object_age_plausibility = cal_src->k_pt_f_check_object_age_plausibility;
        cal_dst->k_pt_enable_host_trail = cal_src->k_pt_enable_host_trail;
        cal_dst->k_pt_max_diff_num_path_point = cal_src->k_pt_max_diff_num_path_point;
        cal_dst->k_pt_group_path_min_overlap_count = cal_src->k_pt_group_path_min_overlap_count;
        cal_dst->k_pt_group_path_min_overlap_count_ad = cal_src->k_pt_group_path_min_overlap_count_ad;
        cal_dst->k_pt_find_min_diff_path_points = cal_src->k_pt_find_min_diff_path_points;
        cal_dst->k_pt_find_max_diff_path_points = cal_src->k_pt_find_max_diff_path_points;
        cal_dst->k_pt_kill_lane_change_min_diff_path_point = cal_src->k_pt_kill_lane_change_min_diff_path_point;
        cal_dst->k_pt_min_diff_num_path_points = cal_src->k_pt_min_diff_num_path_points;
        cal_dst->k_pt_min_path_length_proc_lane_change = cal_src->k_pt_min_path_length_proc_lane_change;
        cal_dst->k_pt_point_diff_grouping_borders_lut[0] = cal_src->k_pt_point_diff_grouping_borders_lut[0];
        cal_dst->k_pt_point_diff_grouping_borders_lut[1] = cal_src->k_pt_point_diff_grouping_borders_lut[1];
        cal_dst->k_pt_point_diff_grouping_borders_lut[2] = cal_src->k_pt_point_diff_grouping_borders_lut[2];
        cal_dst->k_pt_point_diff_grouping_borders_lut[3] = cal_src->k_pt_point_diff_grouping_borders_lut[3];
        cal_dst->k_pt_point_diff_grouping_borders_lut[4] = cal_src->k_pt_point_diff_grouping_borders_lut[4];
        cal_dst->k_pt_cond_kill_implaus_path = cal_src->k_pt_cond_kill_implaus_path;
        cal_dst->k_pt_min_path_length_after_rot = cal_src->k_pt_min_path_length_after_rot;
        cal_dst->k_pt_dist_obj_to_border_lut[0] = cal_src->k_pt_dist_obj_to_border_lut[0];
        cal_dst->k_pt_dist_obj_to_border_lut[1] = cal_src->k_pt_dist_obj_to_border_lut[1];
        cal_dst->k_pt_dist_obj_to_border_lut[2] = cal_src->k_pt_dist_obj_to_border_lut[2];
        cal_dst->k_pt_dist_obj_to_border_lut[3] = cal_src->k_pt_dist_obj_to_border_lut[3];
        cal_dst->k_pt_dist_border_to_isect_lut[0] = cal_src->k_pt_dist_border_to_isect_lut[0];
        cal_dst->k_pt_dist_border_to_isect_lut[1] = cal_src->k_pt_dist_border_to_isect_lut[1];
        cal_dst->k_pt_dist_border_to_isect_lut[2] = cal_src->k_pt_dist_border_to_isect_lut[2];
        cal_dst->k_pt_dist_border_to_isect_lut[3] = cal_src->k_pt_dist_border_to_isect_lut[3];
        cal_dst->k_pt_min_path_length_obj_trail = cal_src->k_pt_min_path_length_obj_trail;
        cal_dst->k_pt_range_nearest_border_impl_path = cal_src->k_pt_range_nearest_border_impl_path;
        cal_dst->k_pt_start_of_lane_change_processing = cal_src->k_pt_start_of_lane_change_processing;
        cal_dst->k_pt_end_of_lane_change_processing = cal_src->k_pt_end_of_lane_change_processing;
        cal_dst->k_pt_minimum_amount_of_trail_points = cal_src->k_pt_minimum_amount_of_trail_points;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Pt_Update_Core_Cal_By_Core(Pt_Core_Calibration_T* cal_dst, const Pt_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Pt_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_pt_zone_max_posn = cal_src->k_pt_zone_max_posn;
        cal_dst->k_pt_min_obj_speed = cal_src->k_pt_min_obj_speed;
        cal_dst->k_pt_overlap_max_match_value = cal_src->k_pt_overlap_max_match_value;
        cal_dst->k_pt_move_max_value = cal_src->k_pt_move_max_value;
        cal_dst->k_pt_group_max_match_value = cal_src->k_pt_group_max_match_value;
        cal_dst->k_pt_group_max_match_value_ad = cal_src->k_pt_group_max_match_value_ad;
        cal_dst->k_pt_group_dir_max_diff_value = cal_src->k_pt_group_dir_max_diff_value;
        cal_dst->k_pt_group_dir_max_avg_diff_value = cal_src->k_pt_group_dir_max_avg_diff_value;
        cal_dst->k_pt_find_max_lat_posn = cal_src->k_pt_find_max_lat_posn;
        cal_dst->k_pt_find_min_speed = cal_src->k_pt_find_min_speed;
        cal_dst->k_pt_find_max_long_posn = cal_src->k_pt_find_max_long_posn;
        cal_dst->k_pt_path_change_match_hyst_default = cal_src->k_pt_path_change_match_hyst_default;
        cal_dst->k_pt_path_change_differing_states_hyst_default = cal_src->k_pt_path_change_differing_states_hyst_default;
        cal_dst->k_pt_path_change_match_hyst_more_established = cal_src->k_pt_path_change_match_hyst_more_established;
        cal_dst->k_pt_path_change_one_grouped_one_mature = cal_src->k_pt_path_change_one_grouped_one_mature;
        cal_dst->k_pt_path_change_one_grouped_one_creation = cal_src->k_pt_path_change_one_grouped_one_creation;
        cal_dst->k_pt_kill_path_exceed_dist_thres = cal_src->k_pt_kill_path_exceed_dist_thres;
        cal_dst->k_pt_kill_path_max_diff_posn = cal_src->k_pt_kill_path_max_diff_posn;
        cal_dst->k_pt_lower_lim_obj_orient_lat = cal_src->k_pt_lower_lim_obj_orient_lat;
        cal_dst->k_pt_upper_lim_obj_orient_lat = cal_src->k_pt_upper_lim_obj_orient_lat;
        cal_dst->k_pt_apply_move_point_min_speed = cal_src->k_pt_apply_move_point_min_speed;
        cal_dst->k_pt_apply_move_point_min_yaw_rate = cal_src->k_pt_apply_move_point_min_yaw_rate;
        cal_dst->k_pt_move_point_yaw_rate_thres_calc_ego_shift = cal_src->k_pt_move_point_yaw_rate_thres_calc_ego_shift;
        cal_dst->k_pt_group_paths_min_interval_dist = cal_src->k_pt_group_paths_min_interval_dist;
        cal_dst->k_pt_path_track_long_range_limit = cal_src->k_pt_path_track_long_range_limit;
        cal_dst->k_pt_path_track_lat_range_limit = cal_src->k_pt_path_track_lat_range_limit;
        cal_dst->k_pt_group_overlap_paths_min_diff = cal_src->k_pt_group_overlap_paths_min_diff;
        cal_dst->k_pt_en_algo_min_vel_inactive = cal_src->k_pt_en_algo_min_vel_inactive;
        cal_dst->k_pt_en_algo_max_val_active = cal_src->k_pt_en_algo_max_val_active;
        cal_dst->k_pt_point_diff_weighting_factor_lut[0] = cal_src->k_pt_point_diff_weighting_factor_lut[0];
        cal_dst->k_pt_point_diff_weighting_factor_lut[1] = cal_src->k_pt_point_diff_weighting_factor_lut[1];
        cal_dst->k_pt_point_diff_weighting_factor_lut[2] = cal_src->k_pt_point_diff_weighting_factor_lut[2];
        cal_dst->k_pt_point_diff_weighting_factor_lut[3] = cal_src->k_pt_point_diff_weighting_factor_lut[3];
        cal_dst->k_pt_point_diff_weighting_factor_lut[4] = cal_src->k_pt_point_diff_weighting_factor_lut[4];
        cal_dst->k_pt_default_range_of_tracking_zone = cal_src->k_pt_default_range_of_tracking_zone;
        cal_dst->k_pt_min_exist_prob_to_be_valid = cal_src->k_pt_min_exist_prob_to_be_valid;
        cal_dst->k_pt_dist_obj_to_border_conf_lut[0] = cal_src->k_pt_dist_obj_to_border_conf_lut[0];
        cal_dst->k_pt_dist_obj_to_border_conf_lut[1] = cal_src->k_pt_dist_obj_to_border_conf_lut[1];
        cal_dst->k_pt_dist_obj_to_border_conf_lut[2] = cal_src->k_pt_dist_obj_to_border_conf_lut[2];
        cal_dst->k_pt_dist_obj_to_border_conf_lut[3] = cal_src->k_pt_dist_obj_to_border_conf_lut[3];
        cal_dst->k_pt_dist_border_to_isect_conf_lut[0] = cal_src->k_pt_dist_border_to_isect_conf_lut[0];
        cal_dst->k_pt_dist_border_to_isect_conf_lut[1] = cal_src->k_pt_dist_border_to_isect_conf_lut[1];
        cal_dst->k_pt_dist_border_to_isect_conf_lut[2] = cal_src->k_pt_dist_border_to_isect_conf_lut[2];
        cal_dst->k_pt_dist_border_to_isect_conf_lut[3] = cal_src->k_pt_dist_border_to_isect_conf_lut[3];
        cal_dst->k_pt_dist_obj_to_path_lut[0] = cal_src->k_pt_dist_obj_to_path_lut[0];
        cal_dst->k_pt_dist_obj_to_path_lut[1] = cal_src->k_pt_dist_obj_to_path_lut[1];
        cal_dst->k_pt_dist_obj_to_path_lut[2] = cal_src->k_pt_dist_obj_to_path_lut[2];
        cal_dst->k_pt_dist_obj_to_path_lut[3] = cal_src->k_pt_dist_obj_to_path_lut[3];
        cal_dst->k_pt_dist_obj_to_path_lut[4] = cal_src->k_pt_dist_obj_to_path_lut[4];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[0] = cal_src->k_pt_dist_obj_to_path_conf_lut[0];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[1] = cal_src->k_pt_dist_obj_to_path_conf_lut[1];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[2] = cal_src->k_pt_dist_obj_to_path_conf_lut[2];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[3] = cal_src->k_pt_dist_obj_to_path_conf_lut[3];
        cal_dst->k_pt_dist_obj_to_path_conf_lut[4] = cal_src->k_pt_dist_obj_to_path_conf_lut[4];
        cal_dst->k_pt_similarity_trail_path_lut[0] = cal_src->k_pt_similarity_trail_path_lut[0];
        cal_dst->k_pt_similarity_trail_path_lut[1] = cal_src->k_pt_similarity_trail_path_lut[1];
        cal_dst->k_pt_similarity_trail_path_lut[2] = cal_src->k_pt_similarity_trail_path_lut[2];
        cal_dst->k_pt_similarity_trail_path_lut[3] = cal_src->k_pt_similarity_trail_path_lut[3];
        cal_dst->k_pt_similarity_trail_path_lut[4] = cal_src->k_pt_similarity_trail_path_lut[4];
        cal_dst->k_pt_similarity_trail_path_conf_lut[0] = cal_src->k_pt_similarity_trail_path_conf_lut[0];
        cal_dst->k_pt_similarity_trail_path_conf_lut[1] = cal_src->k_pt_similarity_trail_path_conf_lut[1];
        cal_dst->k_pt_similarity_trail_path_conf_lut[2] = cal_src->k_pt_similarity_trail_path_conf_lut[2];
        cal_dst->k_pt_similarity_trail_path_conf_lut[3] = cal_src->k_pt_similarity_trail_path_conf_lut[3];
        cal_dst->k_pt_similarity_trail_path_conf_lut[4] = cal_src->k_pt_similarity_trail_path_conf_lut[4];
        cal_dst->k_pt_weight_of_last_trail_point = cal_src->k_pt_weight_of_last_trail_point;
        cal_dst->k_pt_weight_of_sec_last_trail_point = cal_src->k_pt_weight_of_sec_last_trail_point;
        cal_dst->k_pt_heading_diff_lut[0] = cal_src->k_pt_heading_diff_lut[0];
        cal_dst->k_pt_heading_diff_lut[1] = cal_src->k_pt_heading_diff_lut[1];
        cal_dst->k_pt_heading_diff_lut[2] = cal_src->k_pt_heading_diff_lut[2];
        cal_dst->k_pt_heading_diff_lut[3] = cal_src->k_pt_heading_diff_lut[3];
        cal_dst->k_pt_heading_diff_conf_lut[0] = cal_src->k_pt_heading_diff_conf_lut[0];
        cal_dst->k_pt_heading_diff_conf_lut[1] = cal_src->k_pt_heading_diff_conf_lut[1];
        cal_dst->k_pt_heading_diff_conf_lut[2] = cal_src->k_pt_heading_diff_conf_lut[2];
        cal_dst->k_pt_heading_diff_conf_lut[3] = cal_src->k_pt_heading_diff_conf_lut[3];
        cal_dst->k_pt_weight_heading_diff_confidence[0] = cal_src->k_pt_weight_heading_diff_confidence[0];
        cal_dst->k_pt_weight_heading_diff_confidence[1] = cal_src->k_pt_weight_heading_diff_confidence[1];
        cal_dst->k_pt_weight_heading_diff_confidence[2] = cal_src->k_pt_weight_heading_diff_confidence[2];
        cal_dst->k_pt_min_confidence_valid_match = cal_src->k_pt_min_confidence_valid_match;
        cal_dst->k_pt_dist_betw_paths_similarity_matching = cal_src->k_pt_dist_betw_paths_similarity_matching;
        cal_dst->k_pt_host_implausibilty_range = cal_src->k_pt_host_implausibilty_range;
        cal_dst->k_pt_trail_max_speed_trail_to_path_conv = cal_src->k_pt_trail_max_speed_trail_to_path_conv;
        cal_dst->k_pt_minimum_host_trail_length = cal_src->k_pt_minimum_host_trail_length;
        cal_dst->k_pt_max_heading_diff_valid_interval = cal_src->k_pt_max_heading_diff_valid_interval;
        cal_dst->k_pt_f_apply_move_point = cal_src->k_pt_f_apply_move_point;
        cal_dst->k_pt_f_check_object_age_plausibility = cal_src->k_pt_f_check_object_age_plausibility;
        cal_dst->k_pt_enable_host_trail = cal_src->k_pt_enable_host_trail;
        cal_dst->k_pt_max_diff_num_path_point = cal_src->k_pt_max_diff_num_path_point;
        cal_dst->k_pt_group_path_min_overlap_count = cal_src->k_pt_group_path_min_overlap_count;
        cal_dst->k_pt_group_path_min_overlap_count_ad = cal_src->k_pt_group_path_min_overlap_count_ad;
        cal_dst->k_pt_find_min_diff_path_points = cal_src->k_pt_find_min_diff_path_points;
        cal_dst->k_pt_find_max_diff_path_points = cal_src->k_pt_find_max_diff_path_points;
        cal_dst->k_pt_kill_lane_change_min_diff_path_point = cal_src->k_pt_kill_lane_change_min_diff_path_point;
        cal_dst->k_pt_min_diff_num_path_points = cal_src->k_pt_min_diff_num_path_points;
        cal_dst->k_pt_min_path_length_proc_lane_change = cal_src->k_pt_min_path_length_proc_lane_change;
        cal_dst->k_pt_point_diff_grouping_borders_lut[0] = cal_src->k_pt_point_diff_grouping_borders_lut[0];
        cal_dst->k_pt_point_diff_grouping_borders_lut[1] = cal_src->k_pt_point_diff_grouping_borders_lut[1];
        cal_dst->k_pt_point_diff_grouping_borders_lut[2] = cal_src->k_pt_point_diff_grouping_borders_lut[2];
        cal_dst->k_pt_point_diff_grouping_borders_lut[3] = cal_src->k_pt_point_diff_grouping_borders_lut[3];
        cal_dst->k_pt_point_diff_grouping_borders_lut[4] = cal_src->k_pt_point_diff_grouping_borders_lut[4];
        cal_dst->k_pt_cond_kill_implaus_path = cal_src->k_pt_cond_kill_implaus_path;
        cal_dst->k_pt_min_path_length_after_rot = cal_src->k_pt_min_path_length_after_rot;
        cal_dst->k_pt_dist_obj_to_border_lut[0] = cal_src->k_pt_dist_obj_to_border_lut[0];
        cal_dst->k_pt_dist_obj_to_border_lut[1] = cal_src->k_pt_dist_obj_to_border_lut[1];
        cal_dst->k_pt_dist_obj_to_border_lut[2] = cal_src->k_pt_dist_obj_to_border_lut[2];
        cal_dst->k_pt_dist_obj_to_border_lut[3] = cal_src->k_pt_dist_obj_to_border_lut[3];
        cal_dst->k_pt_dist_border_to_isect_lut[0] = cal_src->k_pt_dist_border_to_isect_lut[0];
        cal_dst->k_pt_dist_border_to_isect_lut[1] = cal_src->k_pt_dist_border_to_isect_lut[1];
        cal_dst->k_pt_dist_border_to_isect_lut[2] = cal_src->k_pt_dist_border_to_isect_lut[2];
        cal_dst->k_pt_dist_border_to_isect_lut[3] = cal_src->k_pt_dist_border_to_isect_lut[3];
        cal_dst->k_pt_min_path_length_obj_trail = cal_src->k_pt_min_path_length_obj_trail;
        cal_dst->k_pt_range_nearest_border_impl_path = cal_src->k_pt_range_nearest_border_impl_path;
        cal_dst->k_pt_start_of_lane_change_processing = cal_src->k_pt_start_of_lane_change_processing;
        cal_dst->k_pt_end_of_lane_change_processing = cal_src->k_pt_end_of_lane_change_processing;
        cal_dst->k_pt_minimum_amount_of_trail_points = cal_src->k_pt_minimum_amount_of_trail_points;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Pt_Update_Customer_Cal_By_Public(Pt_Customer_Calibration_T* cal_dst, const Pt_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Pt_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {

        f_result = (boolean_T) 1;
    }
    return f_result;
}


