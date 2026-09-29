/**
* @file pt_public_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in pt_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "pt_public_calibration_check.h" // IWYU pragma: keep
#include "pt_public_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Pt_Public_Cal_In_Boundary(const Pt_Public_Calibration_T *p_calibration)
{
   boolean_T f_pt_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   {
    uint8_t x;
    for (x = 0u; x < PT_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE, p_calibration->k_pt_weight_heading_diff_confidence[x], PT_MAX_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE);

        }
}
{
    uint8_t x;
    for (x = 0u; x < PT_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT, p_calibration->k_pt_dist_border_to_isect_conf_lut[x], PT_MAX_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, 0, p_calibration->k_pt_dist_border_to_isect_lut[x], PT_MAX_K_PT_DIST_BORDER_TO_ISECT_LUT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_DIST_OBJ_TO_BORDER_CONF_LUT, p_calibration->k_pt_dist_obj_to_border_conf_lut[x], PT_MAX_K_PT_DIST_OBJ_TO_BORDER_CONF_LUT);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, 0, p_calibration->k_pt_dist_obj_to_border_lut[x], PT_MAX_K_PT_DIST_OBJ_TO_BORDER_LUT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_HEADING_DIFF_CONF_LUT, p_calibration->k_pt_heading_diff_conf_lut[x], PT_MAX_K_PT_HEADING_DIFF_CONF_LUT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_HEADING_DIFF_LUT, p_calibration->k_pt_heading_diff_lut[x], PT_MAX_K_PT_HEADING_DIFF_LUT);

        }
}
{
    uint8_t x;
    for (x = 0u; x < PT_K_PT_DIST_OBJ_TO_PATH_CONF_LUT_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_DIST_OBJ_TO_PATH_CONF_LUT, p_calibration->k_pt_dist_obj_to_path_conf_lut[x], PT_MAX_K_PT_DIST_OBJ_TO_PATH_CONF_LUT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_DIST_OBJ_TO_PATH_LUT, p_calibration->k_pt_dist_obj_to_path_lut[x], PT_MAX_K_PT_DIST_OBJ_TO_PATH_LUT);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, 0, p_calibration->k_pt_point_diff_grouping_borders_lut[x], PT_MAX_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_POINT_DIFF_WEIGHTING_FACTOR_LUT, p_calibration->k_pt_point_diff_weighting_factor_lut[x], PT_MAX_K_PT_POINT_DIFF_WEIGHTING_FACTOR_LUT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_SIMILARITY_TRAIL_PATH_CONF_LUT, p_calibration->k_pt_similarity_trail_path_conf_lut[x], PT_MAX_K_PT_SIMILARITY_TRAIL_PATH_CONF_LUT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_SIMILARITY_TRAIL_PATH_LUT, p_calibration->k_pt_similarity_trail_path_lut[x], PT_MAX_K_PT_SIMILARITY_TRAIL_PATH_LUT);

        }
}

   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_APPLY_MOVE_POINT_MIN_SPEED, p_calibration->k_pt_apply_move_point_min_speed, PT_MAX_K_PT_APPLY_MOVE_POINT_MIN_SPEED);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_APPLY_MOVE_POINT_MIN_YAW_RATE, p_calibration->k_pt_apply_move_point_min_yaw_rate, PT_MAX_K_PT_APPLY_MOVE_POINT_MIN_YAW_RATE);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_COND_KILL_IMPLAUS_PATH, p_calibration->k_pt_cond_kill_implaus_path, PT_MAX_K_PT_COND_KILL_IMPLAUS_PATH);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_DEFAULT_RANGE_OF_TRACKING_ZONE, p_calibration->k_pt_default_range_of_tracking_zone, PT_MAX_K_PT_DEFAULT_RANGE_OF_TRACKING_ZONE);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_DIST_BETW_PATHS_SIMILARITY_MATCHING, p_calibration->k_pt_dist_betw_paths_similarity_matching, PT_MAX_K_PT_DIST_BETW_PATHS_SIMILARITY_MATCHING);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_EN_ALGO_MAX_VAL_ACTIVE, p_calibration->k_pt_en_algo_max_val_active, PT_MAX_K_PT_EN_ALGO_MAX_VAL_ACTIVE);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_EN_ALGO_MIN_VEL_INACTIVE, p_calibration->k_pt_en_algo_min_vel_inactive, PT_MAX_K_PT_EN_ALGO_MIN_VEL_INACTIVE);
Ct_Is_Bool_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_ENABLE_HOST_TRAIL, p_calibration->k_pt_enable_host_trail, PT_MAX_K_PT_ENABLE_HOST_TRAIL);
Ct_Is_Bool_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_F_APPLY_MOVE_POINT, p_calibration->k_pt_f_apply_move_point, PT_MAX_K_PT_F_APPLY_MOVE_POINT);
Ct_Is_Bool_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_F_CHECK_OBJECT_AGE_PLAUSIBILITY, p_calibration->k_pt_f_check_object_age_plausibility, PT_MAX_K_PT_F_CHECK_OBJECT_AGE_PLAUSIBILITY);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_FIND_MAX_DIFF_PATH_POINTS, p_calibration->k_pt_find_max_diff_path_points, PT_MAX_K_PT_FIND_MAX_DIFF_PATH_POINTS);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_FIND_MAX_LAT_POSN, p_calibration->k_pt_find_max_lat_posn, PT_MAX_K_PT_FIND_MAX_LAT_POSN);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_FIND_MAX_LONG_POSN, p_calibration->k_pt_find_max_long_posn, PT_MAX_K_PT_FIND_MAX_LONG_POSN);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_FIND_MIN_DIFF_PATH_POINTS, p_calibration->k_pt_find_min_diff_path_points, PT_MAX_K_PT_FIND_MIN_DIFF_PATH_POINTS);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_FIND_MIN_SPEED, p_calibration->k_pt_find_min_speed, PT_MAX_K_PT_FIND_MIN_SPEED);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_GROUP_DIR_MAX_AVG_DIFF_VALUE, p_calibration->k_pt_group_dir_max_avg_diff_value, PT_MAX_K_PT_GROUP_DIR_MAX_AVG_DIFF_VALUE);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_GROUP_DIR_MAX_DIFF_VALUE, p_calibration->k_pt_group_dir_max_diff_value, PT_MAX_K_PT_GROUP_DIR_MAX_DIFF_VALUE);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_GROUP_MAX_MATCH_VALUE, p_calibration->k_pt_group_max_match_value, PT_MAX_K_PT_GROUP_MAX_MATCH_VALUE);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_GROUP_MAX_MATCH_VALUE_AD, p_calibration->k_pt_group_max_match_value_ad, PT_MAX_K_PT_GROUP_MAX_MATCH_VALUE_AD);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_GROUP_OVERLAP_PATHS_MIN_DIFF, p_calibration->k_pt_group_overlap_paths_min_diff, PT_MAX_K_PT_GROUP_OVERLAP_PATHS_MIN_DIFF);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT, p_calibration->k_pt_group_path_min_overlap_count, PT_MAX_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT_AD, p_calibration->k_pt_group_path_min_overlap_count_ad, PT_MAX_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT_AD);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_GROUP_PATHS_MIN_INTERVAL_DIST, p_calibration->k_pt_group_paths_min_interval_dist, PT_MAX_K_PT_GROUP_PATHS_MIN_INTERVAL_DIST);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_HOST_IMPLAUSIBILTY_RANGE, p_calibration->k_pt_host_implausibilty_range, PT_MAX_K_PT_HOST_IMPLAUSIBILTY_RANGE);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_KILL_LANE_CHANGE_MIN_DIFF_PATH_POINT, p_calibration->k_pt_kill_lane_change_min_diff_path_point, PT_MAX_K_PT_KILL_LANE_CHANGE_MIN_DIFF_PATH_POINT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_KILL_PATH_EXCEED_DIST_THRES, p_calibration->k_pt_kill_path_exceed_dist_thres, PT_MAX_K_PT_KILL_PATH_EXCEED_DIST_THRES);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_KILL_PATH_MAX_DIFF_POSN, p_calibration->k_pt_kill_path_max_diff_posn, PT_MAX_K_PT_KILL_PATH_MAX_DIFF_POSN);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_LOWER_LIM_OBJ_ORIENT_LAT, p_calibration->k_pt_lower_lim_obj_orient_lat, PT_MAX_K_PT_LOWER_LIM_OBJ_ORIENT_LAT);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MAX_DIFF_NUM_PATH_POINT, p_calibration->k_pt_max_diff_num_path_point, PT_MAX_K_PT_MAX_DIFF_NUM_PATH_POINT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MAX_HEADING_DIFF_VALID_INTERVAL, p_calibration->k_pt_max_heading_diff_valid_interval, PT_MAX_K_PT_MAX_HEADING_DIFF_VALID_INTERVAL);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MIN_CONFIDENCE_VALID_MATCH, p_calibration->k_pt_min_confidence_valid_match, PT_MAX_K_PT_MIN_CONFIDENCE_VALID_MATCH);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MIN_DIFF_NUM_PATH_POINTS, p_calibration->k_pt_min_diff_num_path_points, PT_MAX_K_PT_MIN_DIFF_NUM_PATH_POINTS);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MIN_EXIST_PROB_TO_BE_VALID, p_calibration->k_pt_min_exist_prob_to_be_valid, PT_MAX_K_PT_MIN_EXIST_PROB_TO_BE_VALID);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MIN_OBJ_SPEED, p_calibration->k_pt_min_obj_speed, PT_MAX_K_PT_MIN_OBJ_SPEED);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, 0, p_calibration->k_pt_min_path_length_after_rot, PT_MAX_K_PT_MIN_PATH_LENGTH_AFTER_ROT);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MIN_PATH_LENGTH_OBJ_TRAIL, p_calibration->k_pt_min_path_length_obj_trail, PT_MAX_K_PT_MIN_PATH_LENGTH_OBJ_TRAIL);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MINIMUM_AMOUNT_OF_TRAIL_POINTS, p_calibration->k_pt_minimum_amount_of_trail_points, PT_MAX_K_PT_MINIMUM_AMOUNT_OF_TRAIL_POINTS);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MINIMUM_HOST_TRAIL_LENGTH, p_calibration->k_pt_minimum_host_trail_length, PT_MAX_K_PT_MINIMUM_HOST_TRAIL_LENGTH);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MOVE_MAX_VALUE, p_calibration->k_pt_move_max_value, PT_MAX_K_PT_MOVE_MAX_VALUE);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_MOVE_POINT_YAW_RATE_THRES_CALC_EGO_SHIFT, p_calibration->k_pt_move_point_yaw_rate_thres_calc_ego_shift, PT_MAX_K_PT_MOVE_POINT_YAW_RATE_THRES_CALC_EGO_SHIFT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_OVERLAP_MAX_MATCH_VALUE, p_calibration->k_pt_overlap_max_match_value, PT_MAX_K_PT_OVERLAP_MAX_MATCH_VALUE);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_PATH_CHANGE_DIFFERING_STATES_HYST_DEFAULT, p_calibration->k_pt_path_change_differing_states_hyst_default, PT_MAX_K_PT_PATH_CHANGE_DIFFERING_STATES_HYST_DEFAULT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_PATH_CHANGE_MATCH_HYST_DEFAULT, p_calibration->k_pt_path_change_match_hyst_default, PT_MAX_K_PT_PATH_CHANGE_MATCH_HYST_DEFAULT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_PATH_CHANGE_MATCH_HYST_MORE_ESTABLISHED, p_calibration->k_pt_path_change_match_hyst_more_established, PT_MAX_K_PT_PATH_CHANGE_MATCH_HYST_MORE_ESTABLISHED);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_CREATION, p_calibration->k_pt_path_change_one_grouped_one_creation, PT_MAX_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_CREATION);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_MATURE, p_calibration->k_pt_path_change_one_grouped_one_mature, PT_MAX_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_MATURE);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_PATH_TRACK_LAT_RANGE_LIMIT, p_calibration->k_pt_path_track_lat_range_limit, PT_MAX_K_PT_PATH_TRACK_LAT_RANGE_LIMIT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_PATH_TRACK_LONG_RANGE_LIMIT, p_calibration->k_pt_path_track_long_range_limit, PT_MAX_K_PT_PATH_TRACK_LONG_RANGE_LIMIT);
Ct_Is_Uint8_In_Bondaries(&f_pt_calibration_in_boundaries, 0, p_calibration->k_pt_range_nearest_border_impl_path, PT_MAX_K_PT_RANGE_NEAREST_BORDER_IMPL_PATH);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_TRAIL_MAX_SPEED_TRAIL_TO_PATH_CONV, p_calibration->k_pt_trail_max_speed_trail_to_path_conv, PT_MAX_K_PT_TRAIL_MAX_SPEED_TRAIL_TO_PATH_CONV);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_UPPER_LIM_OBJ_ORIENT_LAT, p_calibration->k_pt_upper_lim_obj_orient_lat, PT_MAX_K_PT_UPPER_LIM_OBJ_ORIENT_LAT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_WEIGHT_OF_LAST_TRAIL_POINT, p_calibration->k_pt_weight_of_last_trail_point, PT_MAX_K_PT_WEIGHT_OF_LAST_TRAIL_POINT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_WEIGHT_OF_SEC_LAST_TRAIL_POINT, p_calibration->k_pt_weight_of_sec_last_trail_point, PT_MAX_K_PT_WEIGHT_OF_SEC_LAST_TRAIL_POINT);
Ct_Is_Float_In_Bondaries(&f_pt_calibration_in_boundaries, PT_MIN_K_PT_ZONE_MAX_POSN, p_calibration->k_pt_zone_max_posn, PT_MAX_K_PT_ZONE_MAX_POSN);


   return f_pt_calibration_in_boundaries;
}

