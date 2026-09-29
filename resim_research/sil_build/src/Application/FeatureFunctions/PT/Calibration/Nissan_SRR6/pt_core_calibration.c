
/**
* @file pt_core_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the Nissan_SRR6 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in pt_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "pt_core_calibration_t.h"
#include "pt_core_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Pt_Core_Cal_Update_Defaults(Pt_Core_Calibration_T* cal_dst)
{
    Pt_Core_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)376,
   /**<version*/ (uint16_t)20,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)25907,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_pt_zone_max_posn*/ (float32_T) 300.0 /**< 300.0 m */,
   /**<k_pt_min_obj_speed*/ (float32_T) 4.0 /**< 4.0 m/s | 14.4 km/h */,
   /**<k_pt_overlap_max_match_value*/ (float32_T) 30.0 /**< 30.0 m */,
   /**<k_pt_move_max_value*/ (float32_T) 30.0 /**< 30.0 m */,
   /**<k_pt_group_max_match_value*/ (float32_T) 3.0 /**< 3.0 m */,
   /**<k_pt_group_max_match_value_ad*/ (float32_T) 2.0 /**< 2.0 m */,
   /**<k_pt_group_dir_max_diff_value*/ (float32_T) 1.5 /**< 1.5 m */,
   /**<k_pt_group_dir_max_avg_diff_value*/ (float32_T) 2.0,
   /**<k_pt_find_max_lat_posn*/ (float32_T) 60.0 /**< 60.0 m */,
   /**<k_pt_find_min_speed*/ (float32_T) 0.01 /**< 0.01 m/s | 0.04 km/h */,
   /**<k_pt_find_max_long_posn*/ (float32_T) 60.0 /**< 60.0 m */,
   /**<k_pt_path_change_match_hyst_default*/ (float32_T) 0.05 /**< 0.05 m */,
   /**<k_pt_path_change_differing_states_hyst_default*/ (float32_T) 0.15 /**< 0.15 m */,
   /**<k_pt_path_change_match_hyst_more_established*/ (float32_T) 0.20 /**< 0.20 m */,
   /**<k_pt_path_change_one_grouped_one_mature*/ (float32_T) 0.30 /**< 0.30 m */,
   /**<k_pt_path_change_one_grouped_one_creation*/ (float32_T) 0.40 /**< 0.40 m */,
   /**<k_pt_kill_path_exceed_dist_thres*/ (float32_T) 30.0f /**< 30.0 m */,
   /**<k_pt_kill_path_max_diff_posn*/ (float32_T) 5.0 /**< 5.0 m */,
   /**<k_pt_lower_lim_obj_orient_lat*/ (float32_T) 0.7 /**< 0.7 rad | 40.11 deg */,
   /**<k_pt_upper_lim_obj_orient_lat*/ (float32_T) 2.44 /**< 2.44 rad | 139.8 deg */,
   /**<k_pt_apply_move_point_min_speed*/ (float32_T) 0.02 /**< 0.02 m/s | 0.07 km/h */,
   /**<k_pt_apply_move_point_min_yaw_rate*/ (float32_T) 0.016 /**< 0.016 rad/s | 0.92 deg/s */,
   /**<k_pt_move_point_yaw_rate_thres_calc_ego_shift*/ (float32_T) 0.01f /**< 0.01 rad | 0.57 deg */,
   /**<k_pt_group_paths_min_interval_dist*/ (float32_T) 3.0 /**< 3.0 m */,
   /**<k_pt_path_track_long_range_limit*/ (float32_T) 60.0f /**< 60.0 m */,
   /**<k_pt_path_track_lat_range_limit*/ (float32_T) 85.0 /**< 85.0 m */,
   /**<k_pt_group_overlap_paths_min_diff*/ (float32_T) 3.5 /**< 3.5 m */,
   /**<k_pt_en_algo_min_vel_inactive*/ (float32_T) 5.0 /**< 5.0 m/s | 18.0 km/h */,
   /**<k_pt_en_algo_max_val_active*/ (float32_T) 7.5 /**< 7.5 m/s | 27.0 km/h */,
   /**<k_pt_point_diff_weighting_factor_lut*/ {(float32_T)0.5f,(float32_T)0.6f,(float32_T)0.75f,(float32_T)0.9f,(float32_T)1.0f},
   /**<k_pt_default_range_of_tracking_zone*/ (float32_T) 60,
   /**<k_pt_min_exist_prob_to_be_valid*/ (float32_T) 0.5,
   /**<k_pt_dist_obj_to_border_conf_lut*/ {(float32_T)1.0,(float32_T)0.9,(float32_T)0.7,(float32_T)0.25},
   /**<k_pt_dist_border_to_isect_conf_lut*/ {(float32_T)1.0,(float32_T)0.85,(float32_T)0.75,(float32_T)0.4},
   /**<k_pt_dist_obj_to_path_lut*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)1.5 /**< 1.5 m */,(float32_T)3.0 /**< 3.0 m */,(float32_T)5.0 /**< 5.0 m */,(float32_T)6.0 /**< 6.0 m */},
   /**<k_pt_dist_obj_to_path_conf_lut*/ {(float32_T)1.0,(float32_T)0.8,(float32_T)0.7,(float32_T)0.25,(float32_T)0.1},
   /**<k_pt_similarity_trail_path_lut*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)1.2 /**< 1.2 m */,(float32_T)1.8 /**< 1.8 m */,(float32_T)2.3 /**< 2.3 m */},
   /**<k_pt_similarity_trail_path_conf_lut*/ {(float32_T)1.0,(float32_T)0.8,(float32_T)0.56,(float32_T)0.3,(float32_T)0.1},
   /**<k_pt_weight_of_last_trail_point*/ (float32_T) 0.7,
   /**<k_pt_weight_of_sec_last_trail_point*/ (float32_T) 0.3,
   /**<k_pt_heading_diff_lut*/ {(float32_T)0.0873 /**< 0.0873 rad | 5.0 deg */,(float32_T)0.1745 /**< 0.1745 rad | 10.0 deg */,(float32_T)0.3490 /**< 0.3490 rad | 20.0 deg */,(float32_T)0.5235 /**< 0.5235 rad | 29.99 deg */},
   /**<k_pt_heading_diff_conf_lut*/ {(float32_T)1.0,(float32_T)0.8,(float32_T)0.35,(float32_T)0.1},
   /**<k_pt_weight_heading_diff_confidence*/ {(float32_T)0.6,(float32_T)0.3,(float32_T)0.1},
   /**<k_pt_min_confidence_valid_match*/ (float32_T) 0.35,
   /**<k_pt_dist_betw_paths_similarity_matching*/ (float32_T) 0.3,
   /**<k_pt_host_implausibilty_range*/ (float32_T) 0.5f /**< 0.5 m */,
   /**<k_pt_trail_max_speed_trail_to_path_conv*/ (float32_T) 1.5 /**< 1.5 m/s | 5.4 km/h */,
   /**<k_pt_minimum_host_trail_length*/ (float32_T) 20.0f /**< 20.0 m */,
   /**<k_pt_max_heading_diff_valid_interval*/ (float32_T) 0.1f /**< 0.1 rad | 5.73 deg */,
   /**<k_pt_f_apply_move_point*/ (boolean_T) 1,
   /**<k_pt_f_check_object_age_plausibility*/ (boolean_T) 1,
   /**<k_pt_enable_host_trail*/ (boolean_T) 1,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_pt_max_diff_num_path_point*/ (uint8_t) 4,
   /**<k_pt_group_path_min_overlap_count*/ (uint8_t) 2,
   /**<k_pt_group_path_min_overlap_count_ad*/ (uint8_t) 4,
   /**<k_pt_find_min_diff_path_points*/ (uint8_t) 2,
   /**<k_pt_find_max_diff_path_points*/ (uint8_t) 3,
   /**<k_pt_kill_lane_change_min_diff_path_point*/ (uint8_t) 5,
   /**<k_pt_min_diff_num_path_points*/ (uint8_t) 4,
   /**<k_pt_min_path_length_proc_lane_change*/ (uint8_t) 13,
   /**<k_pt_point_diff_grouping_borders_lut*/ {(uint8_t)0,(uint8_t)1,(uint8_t)2,(uint8_t)3,(uint8_t)4},
   /**<k_pt_cond_kill_implaus_path*/ (uint8_t) 2,
   /**<k_pt_min_path_length_after_rot*/ (uint8_t) 2,
   /**<k_pt_dist_obj_to_border_lut*/ {(uint8_t)0,(uint8_t)1,(uint8_t)3,(uint8_t)5},
   /**<k_pt_dist_border_to_isect_lut*/ {(uint8_t)0,(uint8_t)1,(uint8_t)2,(uint8_t)4},
   /**<k_pt_min_path_length_obj_trail*/ (uint8_t) 2,
   /**<k_pt_range_nearest_border_impl_path*/ (uint8_t) 4,
   /**<k_pt_start_of_lane_change_processing*/ (uint8_t) 4u,
   /**<k_pt_end_of_lane_change_processing*/ (uint8_t) 28u,
   /**<k_pt_minimum_amount_of_trail_points*/ (uint8_t) 4
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_pt_minimum_amount_of_trail_points*/ (uint8_t) 4,
   /**<k_pt_end_of_lane_change_processing*/ (uint8_t) 28u,
   /**<k_pt_start_of_lane_change_processing*/ (uint8_t) 4u,
   /**<k_pt_range_nearest_border_impl_path*/ (uint8_t) 4,
   /**<k_pt_min_path_length_obj_trail*/ (uint8_t) 2,
   /**<k_pt_dist_border_to_isect_lut*/ {(uint8_t)0,(uint8_t)1,(uint8_t)2,(uint8_t)4},
   /**<k_pt_dist_obj_to_border_lut*/ {(uint8_t)0,(uint8_t)1,(uint8_t)3,(uint8_t)5},
   /**<k_pt_min_path_length_after_rot*/ (uint8_t) 2,
   /**<k_pt_cond_kill_implaus_path*/ (uint8_t) 2,
   /**<k_pt_point_diff_grouping_borders_lut*/ {(uint8_t)0,(uint8_t)1,(uint8_t)2,(uint8_t)3,(uint8_t)4},
   /**<k_pt_min_path_length_proc_lane_change*/ (uint8_t) 13,
   /**<k_pt_min_diff_num_path_points*/ (uint8_t) 4,
   /**<k_pt_kill_lane_change_min_diff_path_point*/ (uint8_t) 5,
   /**<k_pt_find_max_diff_path_points*/ (uint8_t) 3,
   /**<k_pt_find_min_diff_path_points*/ (uint8_t) 2,
   /**<k_pt_group_path_min_overlap_count_ad*/ (uint8_t) 4,
   /**<k_pt_group_path_min_overlap_count*/ (uint8_t) 2,
   /**<k_pt_max_diff_num_path_point*/ (uint8_t) 4,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_pt_enable_host_trail*/ (boolean_T) 1,
   /**<k_pt_f_check_object_age_plausibility*/ (boolean_T) 1,
   /**<k_pt_f_apply_move_point*/ (boolean_T) 1,
   /**<k_pt_max_heading_diff_valid_interval*/ (float32_T) 0.1f /**< 0.1 rad | 5.73 deg */,
   /**<k_pt_minimum_host_trail_length*/ (float32_T) 20.0f /**< 20.0 m */,
   /**<k_pt_trail_max_speed_trail_to_path_conv*/ (float32_T) 1.5 /**< 1.5 m/s | 5.4 km/h */,
   /**<k_pt_host_implausibilty_range*/ (float32_T) 0.5f /**< 0.5 m */,
   /**<k_pt_dist_betw_paths_similarity_matching*/ (float32_T) 0.3,
   /**<k_pt_min_confidence_valid_match*/ (float32_T) 0.35,
   /**<k_pt_weight_heading_diff_confidence*/ {(float32_T)0.6,(float32_T)0.3,(float32_T)0.1},
   /**<k_pt_heading_diff_conf_lut*/ {(float32_T)1.0,(float32_T)0.8,(float32_T)0.35,(float32_T)0.1},
   /**<k_pt_heading_diff_lut*/ {(float32_T)0.0873 /**< 0.0873 rad | 5.0 deg */,(float32_T)0.1745 /**< 0.1745 rad | 10.0 deg */,(float32_T)0.3490 /**< 0.3490 rad | 20.0 deg */,(float32_T)0.5235 /**< 0.5235 rad | 29.99 deg */},
   /**<k_pt_weight_of_sec_last_trail_point*/ (float32_T) 0.3,
   /**<k_pt_weight_of_last_trail_point*/ (float32_T) 0.7,
   /**<k_pt_similarity_trail_path_conf_lut*/ {(float32_T)1.0,(float32_T)0.8,(float32_T)0.56,(float32_T)0.3,(float32_T)0.1},
   /**<k_pt_similarity_trail_path_lut*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)1.2 /**< 1.2 m */,(float32_T)1.8 /**< 1.8 m */,(float32_T)2.3 /**< 2.3 m */},
   /**<k_pt_dist_obj_to_path_conf_lut*/ {(float32_T)1.0,(float32_T)0.8,(float32_T)0.7,(float32_T)0.25,(float32_T)0.1},
   /**<k_pt_dist_obj_to_path_lut*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)1.5 /**< 1.5 m */,(float32_T)3.0 /**< 3.0 m */,(float32_T)5.0 /**< 5.0 m */,(float32_T)6.0 /**< 6.0 m */},
   /**<k_pt_dist_border_to_isect_conf_lut*/ {(float32_T)1.0,(float32_T)0.85,(float32_T)0.75,(float32_T)0.4},
   /**<k_pt_dist_obj_to_border_conf_lut*/ {(float32_T)1.0,(float32_T)0.9,(float32_T)0.7,(float32_T)0.25},
   /**<k_pt_min_exist_prob_to_be_valid*/ (float32_T) 0.5,
   /**<k_pt_default_range_of_tracking_zone*/ (float32_T) 60,
   /**<k_pt_point_diff_weighting_factor_lut*/ {(float32_T)0.5f,(float32_T)0.6f,(float32_T)0.75f,(float32_T)0.9f,(float32_T)1.0f},
   /**<k_pt_en_algo_max_val_active*/ (float32_T) 7.5 /**< 7.5 m/s | 27.0 km/h */,
   /**<k_pt_en_algo_min_vel_inactive*/ (float32_T) 5.0 /**< 5.0 m/s | 18.0 km/h */,
   /**<k_pt_group_overlap_paths_min_diff*/ (float32_T) 3.5 /**< 3.5 m */,
   /**<k_pt_path_track_lat_range_limit*/ (float32_T) 85.0 /**< 85.0 m */,
   /**<k_pt_path_track_long_range_limit*/ (float32_T) 60.0f /**< 60.0 m */,
   /**<k_pt_group_paths_min_interval_dist*/ (float32_T) 3.0 /**< 3.0 m */,
   /**<k_pt_move_point_yaw_rate_thres_calc_ego_shift*/ (float32_T) 0.01f /**< 0.01 rad | 0.57 deg */,
   /**<k_pt_apply_move_point_min_yaw_rate*/ (float32_T) 0.016 /**< 0.016 rad/s | 0.92 deg/s */,
   /**<k_pt_apply_move_point_min_speed*/ (float32_T) 0.02 /**< 0.02 m/s | 0.07 km/h */,
   /**<k_pt_upper_lim_obj_orient_lat*/ (float32_T) 2.44 /**< 2.44 rad | 139.8 deg */,
   /**<k_pt_lower_lim_obj_orient_lat*/ (float32_T) 0.7 /**< 0.7 rad | 40.11 deg */,
   /**<k_pt_kill_path_max_diff_posn*/ (float32_T) 5.0 /**< 5.0 m */,
   /**<k_pt_kill_path_exceed_dist_thres*/ (float32_T) 30.0f /**< 30.0 m */,
   /**<k_pt_path_change_one_grouped_one_creation*/ (float32_T) 0.40 /**< 0.40 m */,
   /**<k_pt_path_change_one_grouped_one_mature*/ (float32_T) 0.30 /**< 0.30 m */,
   /**<k_pt_path_change_match_hyst_more_established*/ (float32_T) 0.20 /**< 0.20 m */,
   /**<k_pt_path_change_differing_states_hyst_default*/ (float32_T) 0.15 /**< 0.15 m */,
   /**<k_pt_path_change_match_hyst_default*/ (float32_T) 0.05 /**< 0.05 m */,
   /**<k_pt_find_max_long_posn*/ (float32_T) 60.0 /**< 60.0 m */,
   /**<k_pt_find_min_speed*/ (float32_T) 0.01 /**< 0.01 m/s | 0.04 km/h */,
   /**<k_pt_find_max_lat_posn*/ (float32_T) 60.0 /**< 60.0 m */,
   /**<k_pt_group_dir_max_avg_diff_value*/ (float32_T) 2.0,
   /**<k_pt_group_dir_max_diff_value*/ (float32_T) 1.5 /**< 1.5 m */,
   /**<k_pt_group_max_match_value_ad*/ (float32_T) 2.0 /**< 2.0 m */,
   /**<k_pt_group_max_match_value*/ (float32_T) 3.0 /**< 3.0 m */,
   /**<k_pt_move_max_value*/ (float32_T) 30.0 /**< 30.0 m */,
   /**<k_pt_overlap_max_match_value*/ (float32_T) 30.0 /**< 30.0 m */,
   /**<k_pt_min_obj_speed*/ (float32_T) 4.0 /**< 4.0 m/s | 14.4 km/h */,
   /**<k_pt_zone_max_posn*/ (float32_T) 300.0 /**< 300.0 m */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)25907,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)20,
   /**<Section_Size*/ (uint32_t)376
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Pt_Core_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Pt_Core_Cal_Reverse_Array_Pt_Cal(Pt_Core_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_point_diff_weighting_factor_lut[0], sizeof(cal_dst->k_pt_point_diff_weighting_factor_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_dist_obj_to_border_conf_lut[0], sizeof(cal_dst->k_pt_dist_obj_to_border_conf_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_dist_border_to_isect_conf_lut[0], sizeof(cal_dst->k_pt_dist_border_to_isect_conf_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_dist_obj_to_path_lut[0], sizeof(cal_dst->k_pt_dist_obj_to_path_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_dist_obj_to_path_conf_lut[0], sizeof(cal_dst->k_pt_dist_obj_to_path_conf_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_similarity_trail_path_lut[0], sizeof(cal_dst->k_pt_similarity_trail_path_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_similarity_trail_path_conf_lut[0], sizeof(cal_dst->k_pt_similarity_trail_path_conf_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_heading_diff_lut[0], sizeof(cal_dst->k_pt_heading_diff_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_heading_diff_conf_lut[0], sizeof(cal_dst->k_pt_heading_diff_conf_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_weight_heading_diff_confidence[0], sizeof(cal_dst->k_pt_weight_heading_diff_confidence), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_point_diff_grouping_borders_lut[0], sizeof(cal_dst->k_pt_point_diff_grouping_borders_lut), CT_ONE_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_dist_obj_to_border_lut[0], sizeof(cal_dst->k_pt_dist_obj_to_border_lut), CT_ONE_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pt_dist_border_to_isect_lut[0], sizeof(cal_dst->k_pt_dist_border_to_isect_lut), CT_ONE_BYTE);
}
#endif /* CT_BIG_ENDIAN */


