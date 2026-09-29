# ifndef PT_PUBLIC_CALIBRATION_T_H
# define PT_PUBLIC_CALIBRATION_T_H

/**
* @file pt_public_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in pt_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ct_calibration_header_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for all calibrations */
/* Macros for array sizes for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_POINT_DIFF_WEIGHTING_FACTOR_LUT_ARRAY_SIZE_DIM0 (5u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_OBJ_TO_BORDER_CONF_LUT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_OBJ_TO_PATH_LUT_ARRAY_SIZE_DIM0 (5u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_OBJ_TO_PATH_CONF_LUT_ARRAY_SIZE_DIM0 (5u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_SIMILARITY_TRAIL_PATH_LUT_ARRAY_SIZE_DIM0 (5u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_SIMILARITY_TRAIL_PATH_CONF_LUT_ARRAY_SIZE_DIM0 (5u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_HEADING_DIFF_LUT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_HEADING_DIFF_CONF_LUT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT_ARRAY_SIZE_DIM0 (5u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_OBJ_TO_BORDER_LUT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_BORDER_TO_ISECT_LUT_ARRAY_SIZE_DIM0 (4u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_POINT_DIFF_WEIGHTING_FACTOR_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_OBJ_TO_BORDER_CONF_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_OBJ_TO_PATH_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_OBJ_TO_PATH_CONF_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_SIMILARITY_TRAIL_PATH_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_SIMILARITY_TRAIL_PATH_CONF_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_HEADING_DIFF_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_HEADING_DIFF_CONF_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_OBJ_TO_BORDER_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_K_PT_DIST_BORDER_TO_ISECT_LUT_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_PUBLIC_CALIBRATION_SIZE (375u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_pt_minimum_amount_of_trail_points; /**<Amount of minimum needed points for path creation based on trail information. */
   uint8_t k_pt_end_of_lane_change_processing; /**<End until which grid points will be used for lane change processing.*/
   uint8_t k_pt_start_of_lane_change_processing; /**<Start from which grid points will be used for lane change processing.*/
   uint8_t k_pt_range_nearest_border_impl_path; /**<Index range between host and nearest path border to the host.*/
   uint8_t k_pt_min_path_length_obj_trail; /**<Minimum path length of the created object trail for further comparison in matching*/
   uint8_t k_pt_dist_border_to_isect_lut[PT_K_PT_DIST_BORDER_TO_ISECT_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between path border and intersection with coordinate axis metric. This lists the values of the metric*/
   uint8_t k_pt_dist_obj_to_border_lut[PT_K_PT_DIST_OBJ_TO_BORDER_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between object and path border metric. This lists the values of the metric*/
   uint8_t k_pt_min_path_length_after_rot; /**<Describes the minimum path length which is needed for paths after rotation to be valid enough for objects being matched to them.*/
   uint8_t k_pt_cond_kill_implaus_path; /**<indicates the difference of number of path points for validity analysis of path*/
   uint8_t k_pt_point_diff_grouping_borders_lut[PT_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT_ARRAY_SIZE_DIM0]; /**<Describes the borders of grouping so that an weighting factor is chosen accordingly. This applies only to points which are contained in one path only*/
   uint8_t k_pt_min_path_length_proc_lane_change; /**<indicates the minimum number of path points to to apply processing of path points for lane change paths*/
   uint8_t k_pt_min_diff_num_path_points; /**<indicates the min differential path points between two paths for killing the path with lower number of points*/
   uint8_t k_pt_kill_lane_change_min_diff_path_point; /**<indicates the min difference of points of the path that can be considered as potential lane change path*/
   uint8_t k_pt_find_max_diff_path_points; /**<indicates the maximum difference of points between path and left point of object that can be used for matching*/
   uint8_t k_pt_find_min_diff_path_points; /**<indicates the minimum difference of points of the path tha can be used for matching*/
   uint8_t k_pt_group_path_min_overlap_count_ad; /**<indicates the minimum overlap count value to determine if two path perfectly matches */
   uint8_t k_pt_group_path_min_overlap_count; /**<indicates the minimum overlap count value to determine if two path matches*/
   uint8_t k_pt_max_diff_num_path_point; /**<indicates the maximum difference of path points*/
   boolean_T k_pt_enable_host_trail; /**<Flag to enable or disable creation of host trails used for host path generation. */
   boolean_T k_pt_f_check_object_age_plausibility; /**<Flag to enable or disable the object age plausibility check.*/
   boolean_T k_pt_f_apply_move_point; /**<indicates the flag to switch on/off function move point based on yawrate and vehicle speed. Default switch off it will have impact on adapt intersection length based on steering angle*/
   float32_T k_pt_max_heading_diff_valid_interval; /**<Threshold indicating the maximum allowed heading difference for an interval to be used for path creation. */
   float32_T k_pt_minimum_host_trail_length; /**<criteria for length of the host trail. Only if the length of the trail is exceeding the threshold, a path shall be created based on the trail. */
   float32_T k_pt_trail_max_speed_trail_to_path_conv; /**<Speed based criteria for conversion of trail to path information. If the host speed is lower than the threshold, the trail information can be converted to trail information. */
   float32_T k_pt_host_implausibilty_range; /**<Offset which is applied to the host dimensions such that when a path point of a extrapolated path is contained in the extrapolation process, the path is invalidated.*/
   float32_T k_pt_dist_betw_paths_similarity_matching; /**<Threshold of difference of distances between object and path and also for host an path. When two paths are similar, the nearer path to the host shall be used as match.*/
   float32_T k_pt_min_confidence_valid_match; /**<Threshold for the confidence in order for the path to be a valid match to an object*/
   float32_T k_pt_weight_heading_diff_confidence[PT_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE_ARRAY_SIZE_DIM0]; /**<Since three heading differences are calculated in total, the actual heading difference where the object is located shall be weighted stronger than the next headings. */
   float32_T k_pt_heading_diff_conf_lut[PT_K_PT_HEADING_DIFF_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Describes the confidence of heading difference between path heading and tracker heading.*/
   float32_T k_pt_heading_diff_lut[PT_K_PT_HEADING_DIFF_LUT_ARRAY_SIZE_DIM0]; /**<Describes the heading difference between path heading and tracker heading in rad*/
   float32_T k_pt_weight_of_sec_last_trail_point; /**<Weight for the second last created point of the path to which the object is associated to. Used for similarity calculation of a path*/
   float32_T k_pt_weight_of_last_trail_point; /**<Weight for the last created point of the path to which the object is associated to. Used for similarity calculation of a path*/
   float32_T k_pt_similarity_trail_path_conf_lut[PT_K_PT_SIMILARITY_TRAIL_PATH_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for similarity of left path points by object and path pair candidate metric. This lists the values of the metric*/
   float32_T k_pt_similarity_trail_path_lut[PT_K_PT_SIMILARITY_TRAIL_PATH_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for similarity of left path points by object and path pair candidate metric. This lists the values of the metric*/
   float32_T k_pt_dist_obj_to_path_conf_lut[PT_K_PT_DIST_OBJ_TO_PATH_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between object to next path point metric. This lists the confidence values*/
   float32_T k_pt_dist_obj_to_path_lut[PT_K_PT_DIST_OBJ_TO_PATH_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between object to next path point metric. This lists the values of the metric */
   float32_T k_pt_dist_border_to_isect_conf_lut[PT_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between path border and intersection with coordinate axis metric. This lists the confidence values*/
   float32_T k_pt_dist_obj_to_border_conf_lut[PT_K_PT_DIST_OBJ_TO_BORDER_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between object and path border metric. This lists the confidence values*/
   float32_T k_pt_min_exist_prob_to_be_valid; /**<minimum existence probability an object needs to have in order for it to be considered for path tracking.*/
   float32_T k_pt_default_range_of_tracking_zone; /**<indicates the default absolute maximum lateral distance which can be displayed by the grid point array*/
   float32_T k_pt_point_diff_weighting_factor_lut[PT_K_PT_POINT_DIFF_WEIGHTING_FACTOR_LUT_ARRAY_SIZE_DIM0]; /**<Describes the borders of grouping so that an weighting factor is chosen accordingly. This applies only to points which are contained in one path only*/
   float32_T k_pt_en_algo_max_val_active; /**<indicates the maximum ego velocity to keep path algo active*/
   float32_T k_pt_en_algo_min_vel_inactive; /**<indicates the minimum ego velocity to enable path algo*/
   float32_T k_pt_group_overlap_paths_min_diff; /**<indicates the minimum distance between two paths at the the same grid point for path overlapping*/
   float32_T k_pt_path_track_lat_range_limit; /**<Indicates the maximum lateral distance for objects to be used for tracking paths.*/
   float32_T k_pt_path_track_long_range_limit; /**<Indicates the maximum longitudinal distance for objects to be used for tracking paths.*/
   float32_T k_pt_group_paths_min_interval_dist; /**<indicates the minimum difference distance between two path points to be considered as crossing paths*/
   float32_T k_pt_move_point_yaw_rate_thres_calc_ego_shift; /**<indicates the lower angle threshold for calculation of ego shifting vector by curvature*/
   float32_T k_pt_apply_move_point_min_yaw_rate; /**<indicates the lower yaw rate threshold for applying moving of path points*/
   float32_T k_pt_apply_move_point_min_speed; /**<indicates the lower speed threshold for applying moving of path points*/
   float32_T k_pt_upper_lim_obj_orient_lat; /**<indicates the upper heading limit for an object to be classified as laterally moving*/
   float32_T k_pt_lower_lim_obj_orient_lat; /**<indicates the lower heading limit for an object to be classified as laterally moving*/
   float32_T k_pt_kill_path_max_diff_posn; /**<indicates the max differential position between [ZERO-1, ZERO+1] to kill 3rd 4th lane path.*/
   float32_T k_pt_kill_path_exceed_dist_thres; /**<indicates the threshold of the position of the zero reference point to kill 3rd 4th lane path in vcs.*/
   float32_T k_pt_path_change_one_grouped_one_creation; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This hysteresis is used when one path is in grouped state while the other is in creation state.*/
   float32_T k_pt_path_change_one_grouped_one_mature; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This hysteresis is used when one path is in grouped state while the other is in mature state.*/
   float32_T k_pt_path_change_match_hyst_more_established; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This hysteresis will be used when two paths are compared which are unequally established.*/
   float32_T k_pt_path_change_differing_states_hyst_default; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This here depicts the default value when two compared paths are equally established, but in different states.*/
   float32_T k_pt_path_change_match_hyst_default; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This here depicts the default value when two compared paths are equally established.*/
   float32_T k_pt_find_max_long_posn; /**<indicates the maximum longitudinal position of the object within conflict zone*/
   float32_T k_pt_find_min_speed; /**<indicates the minimum speed of the object when object is valid for find match algorithm*/
   float32_T k_pt_find_max_lat_posn; /**<indicates the maximum lateral position of the object to be considered as valid*/
   float32_T k_pt_group_dir_max_avg_diff_value; /**<indicates the maximum average difference value to distinguish two path*/
   float32_T k_pt_group_dir_max_diff_value; /**<indicates the maximum difference value to distinguish two path*/
   float32_T k_pt_group_max_match_value_ad; /**<indicates the maximum match value of the difference of path points to group related paths for better match*/
   float32_T k_pt_group_max_match_value; /**<indicates the maximum match value of the difference of path points to group related paths.indicates the minimum required match value of the difference of path points to group related paths*/
   float32_T k_pt_move_max_value; /**<indicates the maximum value to check if path edges exceed limits after rotation*/
   float32_T k_pt_overlap_max_match_value; /**<indicates the maximum match value of the deviation between average value and first value with residuals to group related paths*/
   float32_T k_pt_min_obj_speed; /**<indicates the minimum speed of the object to be used to create path*/
   float32_T k_pt_zone_max_posn; /**<indicates the maximum value of the estimated path point. For position outside this range, defined infinitive value is used.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Pt_Public_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_pt_zone_max_posn; /**<indicates the maximum value of the estimated path point. For position outside this range, defined infinitive value is used.*/
   float32_T k_pt_min_obj_speed; /**<indicates the minimum speed of the object to be used to create path*/
   float32_T k_pt_overlap_max_match_value; /**<indicates the maximum match value of the deviation between average value and first value with residuals to group related paths*/
   float32_T k_pt_move_max_value; /**<indicates the maximum value to check if path edges exceed limits after rotation*/
   float32_T k_pt_group_max_match_value; /**<indicates the maximum match value of the difference of path points to group related paths.indicates the minimum required match value of the difference of path points to group related paths*/
   float32_T k_pt_group_max_match_value_ad; /**<indicates the maximum match value of the difference of path points to group related paths for better match*/
   float32_T k_pt_group_dir_max_diff_value; /**<indicates the maximum difference value to distinguish two path*/
   float32_T k_pt_group_dir_max_avg_diff_value; /**<indicates the maximum average difference value to distinguish two path*/
   float32_T k_pt_find_max_lat_posn; /**<indicates the maximum lateral position of the object to be considered as valid*/
   float32_T k_pt_find_min_speed; /**<indicates the minimum speed of the object when object is valid for find match algorithm*/
   float32_T k_pt_find_max_long_posn; /**<indicates the maximum longitudinal position of the object within conflict zone*/
   float32_T k_pt_path_change_match_hyst_default; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This here depicts the default value when two compared paths are equally established.*/
   float32_T k_pt_path_change_differing_states_hyst_default; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This here depicts the default value when two compared paths are equally established, but in different states.*/
   float32_T k_pt_path_change_match_hyst_more_established; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This hysteresis will be used when two paths are compared which are unequally established.*/
   float32_T k_pt_path_change_one_grouped_one_mature; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This hysteresis is used when one path is in grouped state while the other is in mature state.*/
   float32_T k_pt_path_change_one_grouped_one_creation; /**<Hysteresis for the confidence value which is applied additive when a path change shall be applied to a given object. This hysteresis is used when one path is in grouped state while the other is in creation state.*/
   float32_T k_pt_kill_path_exceed_dist_thres; /**<indicates the threshold of the position of the zero reference point to kill 3rd 4th lane path in vcs.*/
   float32_T k_pt_kill_path_max_diff_posn; /**<indicates the max differential position between [ZERO-1, ZERO+1] to kill 3rd 4th lane path.*/
   float32_T k_pt_lower_lim_obj_orient_lat; /**<indicates the lower heading limit for an object to be classified as laterally moving*/
   float32_T k_pt_upper_lim_obj_orient_lat; /**<indicates the upper heading limit for an object to be classified as laterally moving*/
   float32_T k_pt_apply_move_point_min_speed; /**<indicates the lower speed threshold for applying moving of path points*/
   float32_T k_pt_apply_move_point_min_yaw_rate; /**<indicates the lower yaw rate threshold for applying moving of path points*/
   float32_T k_pt_move_point_yaw_rate_thres_calc_ego_shift; /**<indicates the lower angle threshold for calculation of ego shifting vector by curvature*/
   float32_T k_pt_group_paths_min_interval_dist; /**<indicates the minimum difference distance between two path points to be considered as crossing paths*/
   float32_T k_pt_path_track_long_range_limit; /**<Indicates the maximum longitudinal distance for objects to be used for tracking paths.*/
   float32_T k_pt_path_track_lat_range_limit; /**<Indicates the maximum lateral distance for objects to be used for tracking paths.*/
   float32_T k_pt_group_overlap_paths_min_diff; /**<indicates the minimum distance between two paths at the the same grid point for path overlapping*/
   float32_T k_pt_en_algo_min_vel_inactive; /**<indicates the minimum ego velocity to enable path algo*/
   float32_T k_pt_en_algo_max_val_active; /**<indicates the maximum ego velocity to keep path algo active*/
   float32_T k_pt_point_diff_weighting_factor_lut[PT_K_PT_POINT_DIFF_WEIGHTING_FACTOR_LUT_ARRAY_SIZE_DIM0]; /**<Describes the borders of grouping so that an weighting factor is chosen accordingly. This applies only to points which are contained in one path only*/
   float32_T k_pt_default_range_of_tracking_zone; /**<indicates the default absolute maximum lateral distance which can be displayed by the grid point array*/
   float32_T k_pt_min_exist_prob_to_be_valid; /**<minimum existence probability an object needs to have in order for it to be considered for path tracking.*/
   float32_T k_pt_dist_obj_to_border_conf_lut[PT_K_PT_DIST_OBJ_TO_BORDER_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between object and path border metric. This lists the confidence values*/
   float32_T k_pt_dist_border_to_isect_conf_lut[PT_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between path border and intersection with coordinate axis metric. This lists the confidence values*/
   float32_T k_pt_dist_obj_to_path_lut[PT_K_PT_DIST_OBJ_TO_PATH_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between object to next path point metric. This lists the values of the metric */
   float32_T k_pt_dist_obj_to_path_conf_lut[PT_K_PT_DIST_OBJ_TO_PATH_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between object to next path point metric. This lists the confidence values*/
   float32_T k_pt_similarity_trail_path_lut[PT_K_PT_SIMILARITY_TRAIL_PATH_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for similarity of left path points by object and path pair candidate metric. This lists the values of the metric*/
   float32_T k_pt_similarity_trail_path_conf_lut[PT_K_PT_SIMILARITY_TRAIL_PATH_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for similarity of left path points by object and path pair candidate metric. This lists the values of the metric*/
   float32_T k_pt_weight_of_last_trail_point; /**<Weight for the last created point of the path to which the object is associated to. Used for similarity calculation of a path*/
   float32_T k_pt_weight_of_sec_last_trail_point; /**<Weight for the second last created point of the path to which the object is associated to. Used for similarity calculation of a path*/
   float32_T k_pt_heading_diff_lut[PT_K_PT_HEADING_DIFF_LUT_ARRAY_SIZE_DIM0]; /**<Describes the heading difference between path heading and tracker heading in rad*/
   float32_T k_pt_heading_diff_conf_lut[PT_K_PT_HEADING_DIFF_CONF_LUT_ARRAY_SIZE_DIM0]; /**<Describes the confidence of heading difference between path heading and tracker heading.*/
   float32_T k_pt_weight_heading_diff_confidence[PT_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE_ARRAY_SIZE_DIM0]; /**<Since three heading differences are calculated in total, the actual heading difference where the object is located shall be weighted stronger than the next headings. */
   float32_T k_pt_min_confidence_valid_match; /**<Threshold for the confidence in order for the path to be a valid match to an object*/
   float32_T k_pt_dist_betw_paths_similarity_matching; /**<Threshold of difference of distances between object and path and also for host an path. When two paths are similar, the nearer path to the host shall be used as match.*/
   float32_T k_pt_host_implausibilty_range; /**<Offset which is applied to the host dimensions such that when a path point of a extrapolated path is contained in the extrapolation process, the path is invalidated.*/
   float32_T k_pt_trail_max_speed_trail_to_path_conv; /**<Speed based criteria for conversion of trail to path information. If the host speed is lower than the threshold, the trail information can be converted to trail information. */
   float32_T k_pt_minimum_host_trail_length; /**<criteria for length of the host trail. Only if the length of the trail is exceeding the threshold, a path shall be created based on the trail. */
   float32_T k_pt_max_heading_diff_valid_interval; /**<Threshold indicating the maximum allowed heading difference for an interval to be used for path creation. */
   boolean_T k_pt_f_apply_move_point; /**<indicates the flag to switch on/off function move point based on yawrate and vehicle speed. Default switch off it will have impact on adapt intersection length based on steering angle*/
   boolean_T k_pt_f_check_object_age_plausibility; /**<Flag to enable or disable the object age plausibility check.*/
   boolean_T k_pt_enable_host_trail; /**<Flag to enable or disable creation of host trails used for host path generation. */
   uint8_t k_pt_max_diff_num_path_point; /**<indicates the maximum difference of path points*/
   uint8_t k_pt_group_path_min_overlap_count; /**<indicates the minimum overlap count value to determine if two path matches*/
   uint8_t k_pt_group_path_min_overlap_count_ad; /**<indicates the minimum overlap count value to determine if two path perfectly matches */
   uint8_t k_pt_find_min_diff_path_points; /**<indicates the minimum difference of points of the path tha can be used for matching*/
   uint8_t k_pt_find_max_diff_path_points; /**<indicates the maximum difference of points between path and left point of object that can be used for matching*/
   uint8_t k_pt_kill_lane_change_min_diff_path_point; /**<indicates the min difference of points of the path that can be considered as potential lane change path*/
   uint8_t k_pt_min_diff_num_path_points; /**<indicates the min differential path points between two paths for killing the path with lower number of points*/
   uint8_t k_pt_min_path_length_proc_lane_change; /**<indicates the minimum number of path points to to apply processing of path points for lane change paths*/
   uint8_t k_pt_point_diff_grouping_borders_lut[PT_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT_ARRAY_SIZE_DIM0]; /**<Describes the borders of grouping so that an weighting factor is chosen accordingly. This applies only to points which are contained in one path only*/
   uint8_t k_pt_cond_kill_implaus_path; /**<indicates the difference of number of path points for validity analysis of path*/
   uint8_t k_pt_min_path_length_after_rot; /**<Describes the minimum path length which is needed for paths after rotation to be valid enough for objects being matched to them.*/
   uint8_t k_pt_dist_obj_to_border_lut[PT_K_PT_DIST_OBJ_TO_BORDER_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between object and path border metric. This lists the values of the metric*/
   uint8_t k_pt_dist_border_to_isect_lut[PT_K_PT_DIST_BORDER_TO_ISECT_LUT_ARRAY_SIZE_DIM0]; /**<Used for calculation of confidence of a path object pair for distance between path border and intersection with coordinate axis metric. This lists the values of the metric*/
   uint8_t k_pt_min_path_length_obj_trail; /**<Minimum path length of the created object trail for further comparison in matching*/
   uint8_t k_pt_range_nearest_border_impl_path; /**<Index range between host and nearest path border to the host.*/
   uint8_t k_pt_start_of_lane_change_processing; /**<Start from which grid points will be used for lane change processing.*/
   uint8_t k_pt_end_of_lane_change_processing; /**<End until which grid points will be used for lane change processing.*/
   uint8_t k_pt_minimum_amount_of_trail_points; /**<Amount of minimum needed points for path creation based on trail information. */
} Pt_Public_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* PT_PUBLIC_CALIBRATION_T_H */
