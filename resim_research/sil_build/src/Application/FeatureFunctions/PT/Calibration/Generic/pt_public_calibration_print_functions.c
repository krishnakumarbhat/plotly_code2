/**
* @file pt_public_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in pt_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "pt_public_calibration_t.h" // IWYU pragma: keep
#include "pt_public_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Pt_Public_Cal_Print(FILE* c_file_ptr, const Pt_Public_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_zone_max_posn,%f\n", p_cals->k_pt_zone_max_posn);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_min_obj_speed,%f\n", p_cals->k_pt_min_obj_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_overlap_max_match_value,%f\n", p_cals->k_pt_overlap_max_match_value);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_move_max_value,%f\n", p_cals->k_pt_move_max_value);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_group_max_match_value,%f\n", p_cals->k_pt_group_max_match_value);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_group_max_match_value_ad,%f\n", p_cals->k_pt_group_max_match_value_ad);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_group_dir_max_diff_value,%f\n", p_cals->k_pt_group_dir_max_diff_value);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_group_dir_max_avg_diff_value,%f\n", p_cals->k_pt_group_dir_max_avg_diff_value);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_find_max_lat_posn,%f\n", p_cals->k_pt_find_max_lat_posn);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_find_min_speed,%f\n", p_cals->k_pt_find_min_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_find_max_long_posn,%f\n", p_cals->k_pt_find_max_long_posn);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_path_change_match_hyst_default,%f\n", p_cals->k_pt_path_change_match_hyst_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_path_change_differing_states_hyst_default,%f\n", p_cals->k_pt_path_change_differing_states_hyst_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_path_change_match_hyst_more_established,%f\n", p_cals->k_pt_path_change_match_hyst_more_established);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_path_change_one_grouped_one_mature,%f\n", p_cals->k_pt_path_change_one_grouped_one_mature);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_path_change_one_grouped_one_creation,%f\n", p_cals->k_pt_path_change_one_grouped_one_creation);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_kill_path_exceed_dist_thres,%f\n", p_cals->k_pt_kill_path_exceed_dist_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_kill_path_max_diff_posn,%f\n", p_cals->k_pt_kill_path_max_diff_posn);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_lower_lim_obj_orient_lat,%f\n", p_cals->k_pt_lower_lim_obj_orient_lat);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_upper_lim_obj_orient_lat,%f\n", p_cals->k_pt_upper_lim_obj_orient_lat);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_apply_move_point_min_speed,%f\n", p_cals->k_pt_apply_move_point_min_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_apply_move_point_min_yaw_rate,%f\n", p_cals->k_pt_apply_move_point_min_yaw_rate);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_move_point_yaw_rate_thres_calc_ego_shift,%f\n", p_cals->k_pt_move_point_yaw_rate_thres_calc_ego_shift);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_group_paths_min_interval_dist,%f\n", p_cals->k_pt_group_paths_min_interval_dist);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_path_track_long_range_limit,%f\n", p_cals->k_pt_path_track_long_range_limit);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_path_track_lat_range_limit,%f\n", p_cals->k_pt_path_track_lat_range_limit);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_group_overlap_paths_min_diff,%f\n", p_cals->k_pt_group_overlap_paths_min_diff);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_en_algo_min_vel_inactive,%f\n", p_cals->k_pt_en_algo_min_vel_inactive);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_en_algo_max_val_active,%f\n", p_cals->k_pt_en_algo_max_val_active);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_weighting_factor_lut._0_,%f\n", p_cals->k_pt_point_diff_weighting_factor_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_weighting_factor_lut._1_,%f\n", p_cals->k_pt_point_diff_weighting_factor_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_weighting_factor_lut._2_,%f\n", p_cals->k_pt_point_diff_weighting_factor_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_weighting_factor_lut._3_,%f\n", p_cals->k_pt_point_diff_weighting_factor_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_weighting_factor_lut._4_,%f\n", p_cals->k_pt_point_diff_weighting_factor_lut[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_default_range_of_tracking_zone,%f\n", p_cals->k_pt_default_range_of_tracking_zone);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_min_exist_prob_to_be_valid,%f\n", p_cals->k_pt_min_exist_prob_to_be_valid);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_border_conf_lut._0_,%f\n", p_cals->k_pt_dist_obj_to_border_conf_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_border_conf_lut._1_,%f\n", p_cals->k_pt_dist_obj_to_border_conf_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_border_conf_lut._2_,%f\n", p_cals->k_pt_dist_obj_to_border_conf_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_border_conf_lut._3_,%f\n", p_cals->k_pt_dist_obj_to_border_conf_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_border_to_isect_conf_lut._0_,%f\n", p_cals->k_pt_dist_border_to_isect_conf_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_border_to_isect_conf_lut._1_,%f\n", p_cals->k_pt_dist_border_to_isect_conf_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_border_to_isect_conf_lut._2_,%f\n", p_cals->k_pt_dist_border_to_isect_conf_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_border_to_isect_conf_lut._3_,%f\n", p_cals->k_pt_dist_border_to_isect_conf_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_lut._0_,%f\n", p_cals->k_pt_dist_obj_to_path_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_lut._1_,%f\n", p_cals->k_pt_dist_obj_to_path_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_lut._2_,%f\n", p_cals->k_pt_dist_obj_to_path_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_lut._3_,%f\n", p_cals->k_pt_dist_obj_to_path_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_lut._4_,%f\n", p_cals->k_pt_dist_obj_to_path_lut[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_conf_lut._0_,%f\n", p_cals->k_pt_dist_obj_to_path_conf_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_conf_lut._1_,%f\n", p_cals->k_pt_dist_obj_to_path_conf_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_conf_lut._2_,%f\n", p_cals->k_pt_dist_obj_to_path_conf_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_conf_lut._3_,%f\n", p_cals->k_pt_dist_obj_to_path_conf_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_path_conf_lut._4_,%f\n", p_cals->k_pt_dist_obj_to_path_conf_lut[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_lut._0_,%f\n", p_cals->k_pt_similarity_trail_path_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_lut._1_,%f\n", p_cals->k_pt_similarity_trail_path_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_lut._2_,%f\n", p_cals->k_pt_similarity_trail_path_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_lut._3_,%f\n", p_cals->k_pt_similarity_trail_path_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_lut._4_,%f\n", p_cals->k_pt_similarity_trail_path_lut[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_conf_lut._0_,%f\n", p_cals->k_pt_similarity_trail_path_conf_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_conf_lut._1_,%f\n", p_cals->k_pt_similarity_trail_path_conf_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_conf_lut._2_,%f\n", p_cals->k_pt_similarity_trail_path_conf_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_conf_lut._3_,%f\n", p_cals->k_pt_similarity_trail_path_conf_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_similarity_trail_path_conf_lut._4_,%f\n", p_cals->k_pt_similarity_trail_path_conf_lut[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_weight_of_last_trail_point,%f\n", p_cals->k_pt_weight_of_last_trail_point);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_weight_of_sec_last_trail_point,%f\n", p_cals->k_pt_weight_of_sec_last_trail_point);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_heading_diff_lut._0_,%f\n", p_cals->k_pt_heading_diff_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_heading_diff_lut._1_,%f\n", p_cals->k_pt_heading_diff_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_heading_diff_lut._2_,%f\n", p_cals->k_pt_heading_diff_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_heading_diff_lut._3_,%f\n", p_cals->k_pt_heading_diff_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_heading_diff_conf_lut._0_,%f\n", p_cals->k_pt_heading_diff_conf_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_heading_diff_conf_lut._1_,%f\n", p_cals->k_pt_heading_diff_conf_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_heading_diff_conf_lut._2_,%f\n", p_cals->k_pt_heading_diff_conf_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_heading_diff_conf_lut._3_,%f\n", p_cals->k_pt_heading_diff_conf_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_weight_heading_diff_confidence._0_,%f\n", p_cals->k_pt_weight_heading_diff_confidence[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_weight_heading_diff_confidence._1_,%f\n", p_cals->k_pt_weight_heading_diff_confidence[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_weight_heading_diff_confidence._2_,%f\n", p_cals->k_pt_weight_heading_diff_confidence[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_min_confidence_valid_match,%f\n", p_cals->k_pt_min_confidence_valid_match);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_betw_paths_similarity_matching,%f\n", p_cals->k_pt_dist_betw_paths_similarity_matching);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_host_implausibilty_range,%f\n", p_cals->k_pt_host_implausibilty_range);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_trail_max_speed_trail_to_path_conv,%f\n", p_cals->k_pt_trail_max_speed_trail_to_path_conv);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_minimum_host_trail_length,%f\n", p_cals->k_pt_minimum_host_trail_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_max_heading_diff_valid_interval,%f\n", p_cals->k_pt_max_heading_diff_valid_interval);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_f_apply_move_point,%d\n", p_cals->k_pt_f_apply_move_point);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_f_check_object_age_plausibility,%d\n", p_cals->k_pt_f_check_object_age_plausibility);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_enable_host_trail,%d\n", p_cals->k_pt_enable_host_trail);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_max_diff_num_path_point,%d\n", p_cals->k_pt_max_diff_num_path_point);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_group_path_min_overlap_count,%d\n", p_cals->k_pt_group_path_min_overlap_count);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_group_path_min_overlap_count_ad,%d\n", p_cals->k_pt_group_path_min_overlap_count_ad);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_find_min_diff_path_points,%d\n", p_cals->k_pt_find_min_diff_path_points);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_find_max_diff_path_points,%d\n", p_cals->k_pt_find_max_diff_path_points);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_kill_lane_change_min_diff_path_point,%d\n", p_cals->k_pt_kill_lane_change_min_diff_path_point);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_min_diff_num_path_points,%d\n", p_cals->k_pt_min_diff_num_path_points);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_min_path_length_proc_lane_change,%d\n", p_cals->k_pt_min_path_length_proc_lane_change);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_grouping_borders_lut._0_,%d\n", p_cals->k_pt_point_diff_grouping_borders_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_grouping_borders_lut._1_,%d\n", p_cals->k_pt_point_diff_grouping_borders_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_grouping_borders_lut._2_,%d\n", p_cals->k_pt_point_diff_grouping_borders_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_grouping_borders_lut._3_,%d\n", p_cals->k_pt_point_diff_grouping_borders_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_point_diff_grouping_borders_lut._4_,%d\n", p_cals->k_pt_point_diff_grouping_borders_lut[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_cond_kill_implaus_path,%d\n", p_cals->k_pt_cond_kill_implaus_path);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_min_path_length_after_rot,%d\n", p_cals->k_pt_min_path_length_after_rot);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_border_lut._0_,%d\n", p_cals->k_pt_dist_obj_to_border_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_border_lut._1_,%d\n", p_cals->k_pt_dist_obj_to_border_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_border_lut._2_,%d\n", p_cals->k_pt_dist_obj_to_border_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_obj_to_border_lut._3_,%d\n", p_cals->k_pt_dist_obj_to_border_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_border_to_isect_lut._0_,%d\n", p_cals->k_pt_dist_border_to_isect_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_border_to_isect_lut._1_,%d\n", p_cals->k_pt_dist_border_to_isect_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_border_to_isect_lut._2_,%d\n", p_cals->k_pt_dist_border_to_isect_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_dist_border_to_isect_lut._3_,%d\n", p_cals->k_pt_dist_border_to_isect_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_min_path_length_obj_trail,%d\n", p_cals->k_pt_min_path_length_obj_trail);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_range_nearest_border_impl_path,%d\n", p_cals->k_pt_range_nearest_border_impl_path);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_start_of_lane_change_processing,%d\n", p_cals->k_pt_start_of_lane_change_processing);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_end_of_lane_change_processing,%d\n", p_cals->k_pt_end_of_lane_change_processing);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pt_minimum_amount_of_trail_points,%d\n", p_cals->k_pt_minimum_amount_of_trail_points);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
