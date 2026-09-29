# ifndef CED_PUBLIC_CALIBRATION_T_H
# define CED_PUBLIC_CALIBRATION_T_H

/**
* @file ced_public_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in ced_cal.xml.
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
#define CED_K_CED_FIRST_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_SECOND_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_THIRD_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_ALERT_TTP_MIN_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_CRASH_LINE_HOST_LENGTH_PERCENTAGE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_HONDA_ELATCH_ZONES_WIDTH_TABLE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_SLIGHT_TURN_LAT_VEL_TABLE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_SLIGHT_TURN_POSITION_LIMITS_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_LAT_POS_SHIFT_LONG_DIST_THRESHOLDS_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_LAT_POS_SHIFT_LAT_DIST_THRESHOLDS_ARRAY_SIZE_DIM0 (2u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_FIRST_WARNING_TTC_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_SECOND_WARNING_TTC_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_THIRD_WARNING_TTC_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_ALERT_TTP_MIN_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_CRASH_LINE_HOST_LENGTH_PERCENTAGE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_HONDA_ELATCH_ZONES_WIDTH_TABLE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_SLIGHT_TURN_LAT_VEL_TABLE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_SLIGHT_TURN_POSITION_LIMITS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_LAT_POS_SHIFT_LONG_DIST_THRESHOLDS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_LAT_POS_SHIFT_LAT_DIST_THRESHOLDS_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_PUBLIC_CALIBRATION_SIZE (319u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_ced_f_choose_ref_point_funnel_check; /**<Set refference point in funnel occupance check. (0: deafult, 1: front bumper, 2: nearest corner)*/
   uint8_t k_ced_object_ftm_age_min; /**<Minimal tracker age of object coming from front direction needed to be relevant.*/
   uint8_t k_ced_object_age_min; /**<Minimal tracker age of object needed to be relevant.*/
   uint8_t k_ced_min_cycles_for_path_match_for_no_suppress; /**<Minimum amount of consecutive cycles a new object needs to be matched to a path so that suppression is not applied to it.*/
   uint8_t k_ced_suppress_alert_object_age_max; /**<Based on the nearest path a young object causing an alert can be suppressed. In case that nearest path suppression shall not be used, this calibration can be set to zero.*/
   uint8_t k_ced_allow_opposite_side_alerts; /**<Objects on one ego side are allowed to give an alert for the opposite side (0: not allowed, 1: only allowed if object is matched to a path, 2: allowed for all objects).*/
   uint8_t k_ced_alert_holding_cycles; /**<Number of holding cycles for alert.*/
   uint8_t k_ced_alert_qualifying_cycles_slow_objects; /**<Number of qualifying cycles for objects that are classified as slow objects. See also k_ced_alert_qualifying_cycles and k_ced_slow_objects_long_vel_max.*/
   uint8_t k_ced_alert_qualifying_cycles; /**<Number of qualifying cycles for objects that are not classified as slow objects. See also k_ced_alert_qualifying_cycles_slow_objects and k_ced_slow_objects_long_vel_max.*/
   boolean_T k_ced_lat_pos_shift_enable; /**<Flag enabling logic for shifting lateral position*/
   boolean_T k_ced_f_object_lat_on_one_side_of_border; /**<Flag to check if the alert should be suppressed due to the location of the object. Only if the reference point (depend on FOV in which it is located) is on the correct side of the specified line(k_ced_lat_pos_of_border), alert shall be triggered*/
   boolean_T k_ced_f_adapt_heading_ego_lane; /**<Flag whether heading angle should be zeroed */
   boolean_T k_ced_f_honda_use_alert_ttc_threshold; /**<Flag whether there should be ttc threshold check performed in post run*/
   boolean_T k_ced_f_allow_coasted_object_alerts; /**<Objects that are coasting are allowed to give alerts.*/
   boolean_T k_ced_f_use_only_mature_paths; /**<Pathtracking data can be used only if matched path is mature.*/
   boolean_T k_ced_f_allow_ego_lane_alerts; /**<Objects that are laterally in the ego lane (defined by k_ced_ego_lane_width) are allowed to give alerts.*/
   boolean_T k_ced_f_handle_both_side_alerts_as_object_side; /**<Objects that alert both sides are restricted to only alert their current side.*/
   boolean_T k_ced_f_path_tracking_enable; /**<Enable use of path tracking information.*/
   boolean_T k_ced_f_suppress_alert_holding_for_obj_below_min_ttp; /**<If enabled alert holding on objects with a TTP below k_ced_alert_ttp_min is suppressed.*/
   boolean_T k_ced_f_suppress_alert_holding_for_uncritical_objects; /**<If enabled alert holding on objects with heading or long velocity out of allowed range is suppressed.*/
   boolean_T k_ced_f_third_warning_level_enable; /**<Enable the third warning level.*/
   boolean_T k_ced_f_second_warning_level_enable; /**<Enable the second warning level.*/
   boolean_T k_ced_f_enable_heading_exp_moving_average; /**<Indicates if the exponential moving average function for the object heading is enabled.*/
   float32_T k_ced_warning_pred_lat_dist_max_histeresis; /**<This parameter defines the hystereis of maximum lateral object distance threshold for the ALL warning levels. */
   float32_T k_ced_lat_pos_shift_width_thresh; /**<If object width is equal or greater than this threshold, the lateral position shift will be applied.*/
   float32_T k_ced_object_predicted_max_width_slope_offset; /**<Offset added to object predicted width. It compensates error related to the slope reduce factor.*/
   float32_T k_ced_object_predicted_max_width_slope_reduce_factor; /**<Factor reducing the slope of predicted width.*/
   float32_T k_bmw_ced_speed_max_hysteresis; /**<CED will only work for absolute ego speed values equal or below this value of hysteresis for activation speed.*/
   float32_T k_ced_lat_pos_shift_lat_dist_thresholds[CED_K_CED_LAT_POS_SHIFT_LAT_DIST_THRESHOLDS_ARRAY_SIZE_DIM0]; /**<Lateral distance thresholds for lateral shift calculations. Full shift is applied for objects located farer than 2nd threshold, no shift below 1st threshold, linear slope between them*/
   float32_T k_ced_lat_pos_shift_long_dist_thresholds[CED_K_CED_LAT_POS_SHIFT_LONG_DIST_THRESHOLDS_ARRAY_SIZE_DIM0]; /**<Longitudinal distance thresholds for lateral shift calculations. Full shift is applied for objects located farer than 2nd threshold, no shift below 1st threshold, linear slope between them*/
   float32_T k_ced_lat_pos_max_shift; /**<Maximal value of the lateral position shift, in direction away from ego*/
   float32_T k_ced_lat_pos_of_border; /**<Lateral position of line indicating the longitudinal boundary along which we verify the alert. For RL FOV: Calculated from the left side of the host, For RR FOV: Calculated from the right side of the host*/
   float32_T k_ced_slight_turn_position_limits[CED_K_CED_SLIGHT_TURN_POSITION_LIMITS_ARRAY_SIZE_DIM0]; /**<Longitudinal (first) and lateral (second) thresholds for object distance in slight turn logic. By default set to invalid ced distance*/
   float32_T k_ced_slight_turn_lat_vel_table[CED_K_CED_SLIGHT_TURN_LAT_VEL_TABLE_ARRAY_SIZE_DIM0]; /**<Table with the lateral velocity thresholds for the slight turn logic. If y-speed is below 1st value, sets heading to 0; if over 2nd value, keep initial heading; if between thresholds, heading grows linearly from zero to initial value*/
   float32_T k_honda_min_eratch_alert_duration; /**<The minimum duration of the eratch alert in Honda*/
   float32_T k_honda_elatch_zones_width_table[CED_K_HONDA_ELATCH_ZONES_WIDTH_TABLE_ARRAY_SIZE_DIM0]; /**<Honda table with door elatch zones width for EW_Zone_Information_T ew_elatch_sense_stt   */
   float32_T k_honda_min_alert_duration; /**<The minimum duration of the alert.*/
   float32_T k_ced_object_vel_max; /**<The maximum speed of the target object.*/
   float32_T k_ced_object_ftm_long_vel_rel_max; /**<The maximum relative longitudinal velocity of the front oncoming target object.*/
   float32_T k_ced_object_long_vel_rel_max; /**<Maximum approaching longitudinal relative velocity of the target object.*/
   float32_T k_ced_suppress_range_to_nearest_path_max; /**<Maximum allowed range between the target object and its nearest path segment.*/
   float32_T k_ced_suppress_pt_heading_diff_ced_alert_max; /**<Maximum allowed heading difference of the target object and its nearest path for alerting the object.*/
   float32_T k_ced_alert_holding_obj_long_vel_min; /**<Minimum approaching longitudinal relative velocity of the target object for alert holding logic.*/
   float32_T k_ced_alert_holding_obj_abs_heading_max; /**<Maximum allowed absolute heading angle of the target object for alert holding logic.*/
   float32_T k_ced_ego_lane_parking_maneuver_speed; /**<Sets the speed for a parking scenario of an approaching object. Is used for suppression of objects moving on the ego lane when those are parking.*/
   float32_T k_ced_ego_lane_parking_range; /**<Sets the range for a parking scenario of an approaching object. Is used for suppression of objects moving on the ego lane when those are parking.*/
   float32_T k_ced_ego_lane_width; /**<Sets the virtual width of the ego lane. Is used for classifying objects ego side as EGO_LANE and alert side as INTERSEC_BOTH_SIDES.*/
   float32_T k_ced_slow_objects_long_vel_max; /**<Maximal longitudinal object velocity of an object to be classified as slow object regarding alert qualifying. Use 0.0f to disable different handling of slow objects.*/
   float32_T k_ced_funnel_zone_width; /**<Sets the width of the funnel zones on the side furthest away from the ego.*/
   float32_T k_ced_funnel_zone_length; /**<Sets the length of the funnel zones.*/
   float32_T k_ced_collision_zone_width; /**<Sets the width of the collision zone next to the vehicle. Should be set to door length plus safety margin.*/
   float32_T k_ced_offset_to_path_weight; /**<Weight to change the influence of the path offset for the predicted object at the crash line.*/
   float32_T k_ced_object_min_dist_to_crash_line_for_path_match; /**<Specifies the minimum absolute distance of the object to the crash line to activate path matching functionality. Objects that are closer will be predicted using their heading.*/
   float32_T k_ced_object_width_safety_margin_for_critical_path_match; /**<This value is added to the width of the predicted object, for any object that is matched to the same path as any object with an active alert.*/
   float32_T k_ced_object_width_safety_margin_for_active_alert; /**<This value is added to the width of the predicted object, for any object that was alerted in the previous cycle.*/
   float32_T k_ced_crash_line_host_length_percentage[CED_K_CED_CRASH_LINE_HOST_LENGTH_PERCENTAGE_ARRAY_SIZE_DIM0]; /**<The crash line is set as a percentage of the vehicle length. (0.0 = front bumper, 1.0 = rear bumper). Element [0] refers to objects coming from front, [1] from rear */
   float32_T k_ced_ego_abs_speed_max; /**<CED will only work for absolute ego speed values equal or below this value.*/
   float32_T k_ced_alert_ttp_min[CED_K_CED_ALERT_TTP_MIN_ARRAY_SIZE_DIM0]; /**<Only objects with a time-to-pass equal or above this value can trigger a warning. Also alert holding can be deactived for objects with a TTP below this value using k_ced_f_suppress_alert_holding_for_obj_below_min_ttp.*/
   float32_T k_ced_third_warning_pred_lat_dist_max; /**<This parameter defines the maximum lateral object distance threshold for the third warning level. It shall not be less than k_ced_collision_zone_width.*/
   float32_T k_ced_second_warning_pred_lat_dist_max; /**<This parameter defines the maximum lateral object distance threshold for the second warning level. It shall not be less than k_ced_collision_zone_width.*/
   float32_T k_ced_honda_srr6_long_dist_threshold[CED_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the longitudinal distance from host rear bumper to object reference point threshold for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_honda_srr6_custom_ttc_alert_hysteresis[CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_SIZE_DIM0]; /**<This parameter defines the custom TTC hysteresis for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_honda_srr6_custom_ttc_alert_threshold[CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the custom TTC threshold for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_third_warning_ttc_threshold[CED_K_CED_THIRD_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the TTC threshold for the third warning level if it is enabled. [FRONT REAR]*/
   float32_T k_ced_second_warning_ttc_threshold[CED_K_CED_SECOND_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the TTC threshold for the second warning level if it is enabled. [FRONT REAR]*/
   float32_T k_ced_first_warning_ttc_threshold[CED_K_CED_FIRST_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the TTC threshold for the first warning level. [FRONT REAR]*/
   float32_T k_ced_object_ftm_lat_vel_max; /**<The maximum lateral front oncoming target object velocity.*/
   float32_T k_ced_object_ftm_long_vel_min; /**<The minimum longitudinal front approaching target object velocity.*/
   float32_T k_ced_object_ftm_long_vel_rel_min; /**<The minimum relative longitudinal velocity of the front oncoming target object.*/
   float32_T k_ced_object_ftm_heading_abs_angle_min; /**<The minimum allowed absolute heading angle of the front coming target.*/
   float32_T k_ced_object_ftm_existence_probability_min; /**<The minimum existence probability of the target object coming from front direction.*/
   float32_T k_ced_object_heading_exp_moving_average_alpha; /**<Set alpha value for exponential moving average filter for object heading. Higher alpha means more weight for current values.*/
   float32_T k_ced_object_max_width_increase_factor_without_path_match; /**<Maximal width increase factor of the predicted object for objects without path match. The applied width increase factor will be at the maximal value at the end of the funnel zone and will decrease linearly based on the distance to the crash line. Thus setting the value to 0.0f deactivates the width increase completely.*/
   float32_T k_ced_object_max_width_increase_factor_with_path_match; /**<Maximal width increase factor of the predicted object for objects with path match. The applied width increase factor will be at the maximal value at the end of the funnel zone and will decrease linearly based on the distance to the crash line. Thus setting the value to 0.0f deactivates the width increase completely.*/
   float32_T k_ced_object_lat_vel_max; /**<Maximum lateral target velocity.*/
   float32_T k_ced_object_long_vel_min; /**<Minimum longitudinal target object velocity.*/
   float32_T k_ced_object_long_vel_rel_min; /**<Minimum approaching longitudinal relative velocity of the target object.*/
   float32_T k_ced_object_heading_abs_angle_max; /**<Maximum allowed absolute heading angle of the target object.*/
   float32_T k_ced_object_existence_probability_min; /**<Minimum existence probability of the target object.*/
   float32_T k_ced_object_heading_predicted_weight; /**<Weights the predicted heading that will be used for objects without path match. This parameter can control the influence of the object heading on the predicted object and can be used as a temporary fix to ignore an inaccurate tracker heading for the predicted object.*/
   float32_T k_ced_object_acceleration_weight; /**<Weights the influence of the objects acceleration on the prediction and TTC calculation.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Ced_Public_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_ced_object_acceleration_weight; /**<Weights the influence of the objects acceleration on the prediction and TTC calculation.*/
   float32_T k_ced_object_heading_predicted_weight; /**<Weights the predicted heading that will be used for objects without path match. This parameter can control the influence of the object heading on the predicted object and can be used as a temporary fix to ignore an inaccurate tracker heading for the predicted object.*/
   float32_T k_ced_object_existence_probability_min; /**<Minimum existence probability of the target object.*/
   float32_T k_ced_object_heading_abs_angle_max; /**<Maximum allowed absolute heading angle of the target object.*/
   float32_T k_ced_object_long_vel_rel_min; /**<Minimum approaching longitudinal relative velocity of the target object.*/
   float32_T k_ced_object_long_vel_min; /**<Minimum longitudinal target object velocity.*/
   float32_T k_ced_object_lat_vel_max; /**<Maximum lateral target velocity.*/
   float32_T k_ced_object_max_width_increase_factor_with_path_match; /**<Maximal width increase factor of the predicted object for objects with path match. The applied width increase factor will be at the maximal value at the end of the funnel zone and will decrease linearly based on the distance to the crash line. Thus setting the value to 0.0f deactivates the width increase completely.*/
   float32_T k_ced_object_max_width_increase_factor_without_path_match; /**<Maximal width increase factor of the predicted object for objects without path match. The applied width increase factor will be at the maximal value at the end of the funnel zone and will decrease linearly based on the distance to the crash line. Thus setting the value to 0.0f deactivates the width increase completely.*/
   float32_T k_ced_object_heading_exp_moving_average_alpha; /**<Set alpha value for exponential moving average filter for object heading. Higher alpha means more weight for current values.*/
   float32_T k_ced_object_ftm_existence_probability_min; /**<The minimum existence probability of the target object coming from front direction.*/
   float32_T k_ced_object_ftm_heading_abs_angle_min; /**<The minimum allowed absolute heading angle of the front coming target.*/
   float32_T k_ced_object_ftm_long_vel_rel_min; /**<The minimum relative longitudinal velocity of the front oncoming target object.*/
   float32_T k_ced_object_ftm_long_vel_min; /**<The minimum longitudinal front approaching target object velocity.*/
   float32_T k_ced_object_ftm_lat_vel_max; /**<The maximum lateral front oncoming target object velocity.*/
   float32_T k_ced_first_warning_ttc_threshold[CED_K_CED_FIRST_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the TTC threshold for the first warning level. [FRONT REAR]*/
   float32_T k_ced_second_warning_ttc_threshold[CED_K_CED_SECOND_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the TTC threshold for the second warning level if it is enabled. [FRONT REAR]*/
   float32_T k_ced_third_warning_ttc_threshold[CED_K_CED_THIRD_WARNING_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the TTC threshold for the third warning level if it is enabled. [FRONT REAR]*/
   float32_T k_ced_honda_srr6_custom_ttc_alert_threshold[CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the custom TTC threshold for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_honda_srr6_custom_ttc_alert_hysteresis[CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_SIZE_DIM0]; /**<This parameter defines the custom TTC hysteresis for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_honda_srr6_long_dist_threshold[CED_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the longitudinal distance from host rear bumper to object reference point threshold for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_second_warning_pred_lat_dist_max; /**<This parameter defines the maximum lateral object distance threshold for the second warning level. It shall not be less than k_ced_collision_zone_width.*/
   float32_T k_ced_third_warning_pred_lat_dist_max; /**<This parameter defines the maximum lateral object distance threshold for the third warning level. It shall not be less than k_ced_collision_zone_width.*/
   float32_T k_ced_alert_ttp_min[CED_K_CED_ALERT_TTP_MIN_ARRAY_SIZE_DIM0]; /**<Only objects with a time-to-pass equal or above this value can trigger a warning. Also alert holding can be deactived for objects with a TTP below this value using k_ced_f_suppress_alert_holding_for_obj_below_min_ttp.*/
   float32_T k_ced_ego_abs_speed_max; /**<CED will only work for absolute ego speed values equal or below this value.*/
   float32_T k_ced_crash_line_host_length_percentage[CED_K_CED_CRASH_LINE_HOST_LENGTH_PERCENTAGE_ARRAY_SIZE_DIM0]; /**<The crash line is set as a percentage of the vehicle length. (0.0 = front bumper, 1.0 = rear bumper). Element [0] refers to objects coming from front, [1] from rear */
   float32_T k_ced_object_width_safety_margin_for_active_alert; /**<This value is added to the width of the predicted object, for any object that was alerted in the previous cycle.*/
   float32_T k_ced_object_width_safety_margin_for_critical_path_match; /**<This value is added to the width of the predicted object, for any object that is matched to the same path as any object with an active alert.*/
   float32_T k_ced_object_min_dist_to_crash_line_for_path_match; /**<Specifies the minimum absolute distance of the object to the crash line to activate path matching functionality. Objects that are closer will be predicted using their heading.*/
   float32_T k_ced_offset_to_path_weight; /**<Weight to change the influence of the path offset for the predicted object at the crash line.*/
   float32_T k_ced_collision_zone_width; /**<Sets the width of the collision zone next to the vehicle. Should be set to door length plus safety margin.*/
   float32_T k_ced_funnel_zone_length; /**<Sets the length of the funnel zones.*/
   float32_T k_ced_funnel_zone_width; /**<Sets the width of the funnel zones on the side furthest away from the ego.*/
   float32_T k_ced_slow_objects_long_vel_max; /**<Maximal longitudinal object velocity of an object to be classified as slow object regarding alert qualifying. Use 0.0f to disable different handling of slow objects.*/
   float32_T k_ced_ego_lane_width; /**<Sets the virtual width of the ego lane. Is used for classifying objects ego side as EGO_LANE and alert side as INTERSEC_BOTH_SIDES.*/
   float32_T k_ced_ego_lane_parking_range; /**<Sets the range for a parking scenario of an approaching object. Is used for suppression of objects moving on the ego lane when those are parking.*/
   float32_T k_ced_ego_lane_parking_maneuver_speed; /**<Sets the speed for a parking scenario of an approaching object. Is used for suppression of objects moving on the ego lane when those are parking.*/
   float32_T k_ced_alert_holding_obj_abs_heading_max; /**<Maximum allowed absolute heading angle of the target object for alert holding logic.*/
   float32_T k_ced_alert_holding_obj_long_vel_min; /**<Minimum approaching longitudinal relative velocity of the target object for alert holding logic.*/
   float32_T k_ced_suppress_pt_heading_diff_ced_alert_max; /**<Maximum allowed heading difference of the target object and its nearest path for alerting the object.*/
   float32_T k_ced_suppress_range_to_nearest_path_max; /**<Maximum allowed range between the target object and its nearest path segment.*/
   float32_T k_ced_object_long_vel_rel_max; /**<Maximum approaching longitudinal relative velocity of the target object.*/
   float32_T k_ced_object_ftm_long_vel_rel_max; /**<The maximum relative longitudinal velocity of the front oncoming target object.*/
   float32_T k_ced_object_vel_max; /**<The maximum speed of the target object.*/
   float32_T k_honda_min_alert_duration; /**<The minimum duration of the alert.*/
   float32_T k_honda_elatch_zones_width_table[CED_K_HONDA_ELATCH_ZONES_WIDTH_TABLE_ARRAY_SIZE_DIM0]; /**<Honda table with door elatch zones width for EW_Zone_Information_T ew_elatch_sense_stt   */
   float32_T k_honda_min_eratch_alert_duration; /**<The minimum duration of the eratch alert in Honda*/
   float32_T k_ced_slight_turn_lat_vel_table[CED_K_CED_SLIGHT_TURN_LAT_VEL_TABLE_ARRAY_SIZE_DIM0]; /**<Table with the lateral velocity thresholds for the slight turn logic. If y-speed is below 1st value, sets heading to 0; if over 2nd value, keep initial heading; if between thresholds, heading grows linearly from zero to initial value*/
   float32_T k_ced_slight_turn_position_limits[CED_K_CED_SLIGHT_TURN_POSITION_LIMITS_ARRAY_SIZE_DIM0]; /**<Longitudinal (first) and lateral (second) thresholds for object distance in slight turn logic. By default set to invalid ced distance*/
   float32_T k_ced_lat_pos_of_border; /**<Lateral position of line indicating the longitudinal boundary along which we verify the alert. For RL FOV: Calculated from the left side of the host, For RR FOV: Calculated from the right side of the host*/
   float32_T k_ced_lat_pos_max_shift; /**<Maximal value of the lateral position shift, in direction away from ego*/
   float32_T k_ced_lat_pos_shift_long_dist_thresholds[CED_K_CED_LAT_POS_SHIFT_LONG_DIST_THRESHOLDS_ARRAY_SIZE_DIM0]; /**<Longitudinal distance thresholds for lateral shift calculations. Full shift is applied for objects located farer than 2nd threshold, no shift below 1st threshold, linear slope between them*/
   float32_T k_ced_lat_pos_shift_lat_dist_thresholds[CED_K_CED_LAT_POS_SHIFT_LAT_DIST_THRESHOLDS_ARRAY_SIZE_DIM0]; /**<Lateral distance thresholds for lateral shift calculations. Full shift is applied for objects located farer than 2nd threshold, no shift below 1st threshold, linear slope between them*/
   float32_T k_bmw_ced_speed_max_hysteresis; /**<CED will only work for absolute ego speed values equal or below this value of hysteresis for activation speed.*/
   float32_T k_ced_object_predicted_max_width_slope_reduce_factor; /**<Factor reducing the slope of predicted width.*/
   float32_T k_ced_object_predicted_max_width_slope_offset; /**<Offset added to object predicted width. It compensates error related to the slope reduce factor.*/
   float32_T k_ced_lat_pos_shift_width_thresh; /**<If object width is equal or greater than this threshold, the lateral position shift will be applied.*/
   float32_T k_ced_warning_pred_lat_dist_max_histeresis; /**<This parameter defines the hystereis of maximum lateral object distance threshold for the ALL warning levels. */
   boolean_T k_ced_f_enable_heading_exp_moving_average; /**<Indicates if the exponential moving average function for the object heading is enabled.*/
   boolean_T k_ced_f_second_warning_level_enable; /**<Enable the second warning level.*/
   boolean_T k_ced_f_third_warning_level_enable; /**<Enable the third warning level.*/
   boolean_T k_ced_f_suppress_alert_holding_for_uncritical_objects; /**<If enabled alert holding on objects with heading or long velocity out of allowed range is suppressed.*/
   boolean_T k_ced_f_suppress_alert_holding_for_obj_below_min_ttp; /**<If enabled alert holding on objects with a TTP below k_ced_alert_ttp_min is suppressed.*/
   boolean_T k_ced_f_path_tracking_enable; /**<Enable use of path tracking information.*/
   boolean_T k_ced_f_handle_both_side_alerts_as_object_side; /**<Objects that alert both sides are restricted to only alert their current side.*/
   boolean_T k_ced_f_allow_ego_lane_alerts; /**<Objects that are laterally in the ego lane (defined by k_ced_ego_lane_width) are allowed to give alerts.*/
   boolean_T k_ced_f_use_only_mature_paths; /**<Pathtracking data can be used only if matched path is mature.*/
   boolean_T k_ced_f_allow_coasted_object_alerts; /**<Objects that are coasting are allowed to give alerts.*/
   boolean_T k_ced_f_honda_use_alert_ttc_threshold; /**<Flag whether there should be ttc threshold check performed in post run*/
   boolean_T k_ced_f_adapt_heading_ego_lane; /**<Flag whether heading angle should be zeroed */
   boolean_T k_ced_f_object_lat_on_one_side_of_border; /**<Flag to check if the alert should be suppressed due to the location of the object. Only if the reference point (depend on FOV in which it is located) is on the correct side of the specified line(k_ced_lat_pos_of_border), alert shall be triggered*/
   boolean_T k_ced_lat_pos_shift_enable; /**<Flag enabling logic for shifting lateral position*/
   uint8_t k_ced_alert_qualifying_cycles; /**<Number of qualifying cycles for objects that are not classified as slow objects. See also k_ced_alert_qualifying_cycles_slow_objects and k_ced_slow_objects_long_vel_max.*/
   uint8_t k_ced_alert_qualifying_cycles_slow_objects; /**<Number of qualifying cycles for objects that are classified as slow objects. See also k_ced_alert_qualifying_cycles and k_ced_slow_objects_long_vel_max.*/
   uint8_t k_ced_alert_holding_cycles; /**<Number of holding cycles for alert.*/
   uint8_t k_ced_allow_opposite_side_alerts; /**<Objects on one ego side are allowed to give an alert for the opposite side (0: not allowed, 1: only allowed if object is matched to a path, 2: allowed for all objects).*/
   uint8_t k_ced_suppress_alert_object_age_max; /**<Based on the nearest path a young object causing an alert can be suppressed. In case that nearest path suppression shall not be used, this calibration can be set to zero.*/
   uint8_t k_ced_min_cycles_for_path_match_for_no_suppress; /**<Minimum amount of consecutive cycles a new object needs to be matched to a path so that suppression is not applied to it.*/
   uint8_t k_ced_object_age_min; /**<Minimal tracker age of object needed to be relevant.*/
   uint8_t k_ced_object_ftm_age_min; /**<Minimal tracker age of object coming from front direction needed to be relevant.*/
   uint8_t k_ced_f_choose_ref_point_funnel_check; /**<Set refference point in funnel occupance check. (0: deafult, 1: front bumper, 2: nearest corner)*/
} Ced_Public_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* CED_PUBLIC_CALIBRATION_T_H */
