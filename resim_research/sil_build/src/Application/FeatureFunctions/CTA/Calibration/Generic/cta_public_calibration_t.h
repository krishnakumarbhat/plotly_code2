# ifndef CTA_PUBLIC_CALIBRATION_T_H
# define CTA_PUBLIC_CALIBRATION_T_H

/**
* @file cta_public_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in cta_cal.xml.
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
#define CTA_K_CTA_BUTTERFLY_LONG_ARRAY_SIZE_DIM0 (8u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_BUTTERFLY_LAT_ARRAY_SIZE_DIM0 (8u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_TTC_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_TTC_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_SPEED_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_SPEED_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_HEADING_RANGE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_ANGLES_ZONE_DEFINITION_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTB_UPPER_SAFETY_DISTANCE_THRES_LUT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_ENABLE_MODES_ARRAY_SIZE_DIM0 (2u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_BUTTERFLY_LONG_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_BUTTERFLY_LAT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_TTC_CRITICALITY_LEVEL_ARRAY_DIM_SIZE (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_SPEED_CRITICALITY_LEVEL_ARRAY_DIM_SIZE (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_DIM_SIZE (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL_ARRAY_DIM_SIZE (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_HEADING_RANGE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_ANGLES_ZONE_DEFINITION_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTB_UPPER_SAFETY_DISTANCE_THRES_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_K_CTA_ENABLE_MODES_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_PUBLIC_CALIBRATION_SIZE (379u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_cta_age_for_new_creation_below_long_intersection; /**<When an object is newly created directly in the longitudinal area between host width and longitudinal intersection, it shall be suppressed.*/
   uint8_t k_cta_cycles_valid_match_of_pot_ghost; /**<Cycles for an object to be matched to a path with a minimum confidence so that it is not classified as potential ghost anymore*/
   uint8_t k_cta_min_qual_age_obj_crossing_paths; /**<age of the object which is necessary to qualify in case of a lane change, when it is crossing its nearest path*/
   uint8_t k_ctb_min_brake_hold_ctr_thres; /**<Holds the brake qualifier on each side for at least this specified amount of cycles.*/
   uint8_t k_ctb_min_brake_qual_ctr_thres; /**<Qualification counter threshold for braking logic.*/
   uint8_t k_cta_additional_qualification_mature_cycles; /**<Additional maturity cycles for an object to qualify for a warning level.*/
   uint8_t k_cta_min_mature_cycles_level_qualifiction; /**<Minimum required number of consecutive cycles for an object status to be mature for activation of a criticality level.*/
   uint8_t k_cta_ghost_validation_min_mature; /**<Min mature age of a potential ghost to be considerd by the object validation.*/
   uint8_t k_cta_ghost_validation_min_age; /**<Min age of a potential ghost to be considerd by the object validation.*/
   uint8_t k_cta_object_supress_counter; /**<This counter supresses objects that are untrusted for a number of cycles.*/
   uint8_t k_cta_cycles_coasted_to_ignore; /**<Defines the cycles that an object can maximal be coasted to be still considered by the algo.*/
   uint8_t k_cta_min_object_age_thres; /**<Minimal age of an object in cycles used for criticality level calculation.*/
   uint8_t k_cta_min_object_age_check_valid; /**<Minimal age of an object in cycles to be considered by the algorithm.*/
   uint8_t k_cta_cycle_count_hold_true_warning; /**<Minimum number of cycles requiered to hold each criticality level.*/
   uint8_t k_cta_cycle_count_suppress_true_warning; /**<Qualifier for the number of cycles required for each critical level.*/
   uint8_t k_cta_enable_modes[CTA_K_CTA_ENABLE_MODES_ARRAY_SIZE_DIM0]; /**<This calibration enables either rear or front CTA. First dimension = RCTA, Second dimension = FCTA.*/
   uint8_t k_cta_amount_butterfly_points_in_use; /**<Actual amount of points which are in use for the definition of butterfly zone. Must be less than maximum allowed butterfly points*/
   boolean_T k_cta_f_stop_mode_ttp; /**<Summarizes the alert stop modes for CTA. If flag is enabled we use TTP threshold for alert suppresion, otherwise we use TTC*/
   boolean_T k_cta_f_enable_heading_exp_moving_average; /**<Indicates if the exponential moving average function for the object heading is enabled.*/
   boolean_T k_cta_f_use_brake_gradient; /**<Flag used to enable the usage of the k_cta_braking_gradient instead of calculating it from k_cta_target_acceleration_target_braking and k_cta_target_acceleration_target_braking_time.*/
   boolean_T k_cta_f_brake_overriding_ctb; /**<Flag used to enable CTB overriding.*/
   boolean_T k_cta_enable_ctb; /**<This flag enables CTB if set to TRUE and disables it if set to FALSE.*/
   boolean_T k_cta_f_use_front_corners_dist_stop; /**<Flag indicating to use front corners of an object to check, whether it has already passed a user defined longitudinal axis so that alert can be turned off. Default are rear corners.*/
   boolean_T k_cta_f_enable_thres_crit_level_reset; /**<Flag indicating whether to reset the criticality level when an object passes a user defined threshold.*/
   boolean_T k_cta_f_use_rel_vel_isect_point_calc; /**<Flag indicating whether to use relative velocity or heading of tracker to calculate intersection point.*/
   boolean_T k_cta_f_use_object_min_object_age_in_cycles; /**<Flag indicating whether object age threshold is used to check cta relevance for this object.*/
   boolean_T k_cta_f_use_ghost_detector; /**<This flags activates the check whether the object is a ghost within the object validation.*/
   boolean_T k_cta_f_use_object_supress_counter; /**<This flag indicates whether the object_supress_counter shall be used.*/
   boolean_T k_cta_f_discard_pt_heading_when_moving; /**<Switch to turn on or off path tracking when moving backwards when additional conditions are fulfilled (min_host_speed_to_discard_pt_info, obj_dist_to_discard_pt_info).*/
   boolean_T k_cta_f_apply_path_tracking; /**<Switch to turn on or off path tracking algorithm.*/
   boolean_T k_cta_f_use_heading_for_relative_velocity_calculation; /**<If flag is set to true, the relative velocity vector of an object will be calculated by making use of heading and absolute velocity of an object. If it is set to false, the relative velocity provided by the tracker will be used.*/
   boolean_T k_cta_f_calc_ttp_ego_side_enabled; /**<Switch to turn on or off calculate TTP with reference to the ego side instead of x-axis defined in VCS.*/
   boolean_T k_cta_f_calc_ttc_ego_side_enabled; /**<Switch to turn on or off calculate TTC with reference to the ego side instead of x-axis defined in VCS.*/
   boolean_T k_cta_f_apply_heading_compensation_on_intersection_point; /**<Switch to turn on or off the adaption of calculated intersection point by a value depending on target heading and ego width.*/
   boolean_T k_cta_f_prevent_fall_back_to_critlevel_1; /**<Set to 0: Criticality level may drop to level 1 once it has been higher than 1; set to 1: criticality level will remain higher than 1 once it has been higher than 1 even though the conditions for the higher level are not met any longer.*/
   boolean_T k_cta_f_adapt_intersect_lines_by_host_speed; /**<Switch to turn on or off host-speed-based adaptation of the intersect lines associated with the criticality levels.*/
   boolean_T k_cta_f_adapt_intersect_lines_by_obj_heading; /**<Switch to turn on or off object-heading-based adaptation of the intersect lines associated with the criticality levels.*/
   boolean_T k_cta_f_adapt_intersect_lines_by_steering_angle; /**<Switch to turn on or off steering-angle-based adaptation of the intersect lines associated with the criticality levels.*/
   float32_T k_cta_speed_thresh_for_rel_vel_calc; /**<Below this threshold object relative velocity will be calculated by using its heading */
   float32_T k_cta_ttc_calc_positive_ref_point; /**<Maximum deceleration value for braking*/
   float32_T k_cta_max_deceleration_value; /**<Maximum deceleration value for braking*/
   float32_T k_cta_min_deceleration_value; /**<Minimum deceleration value for braking*/
   float32_T k_cta_2wheel_min_speed; /**<Minimum speed for an object classified as 2wheel to be selected as a valid candidate for CTA when other criteria are met*/
   float32_T k_cta_2wheel_min_size; /**<Minimum size for an object classified as 2wheel to be selected as a valid candidate for CTA when other criteria are met*/
   float32_T k_cta_pedestrian_min_speed; /**<Minimum speed for an object classified as pedestrian to be selected as a valid candidate for CTA when other criteria are met*/
   float32_T k_cta_pedestrian_min_size; /**<Minimum size for an object classified as pedestrian to be selected as a valid candidate for CTA when other criteria are met*/
   float32_T k_cta_object_heading_exp_moving_average_alpha; /**<Set alpha value for exponential moving average filter for object heading. Higher alpha means more weight for current values.*/
   float32_T k_cta_range_to_path_segment_ghost_qualif; /**<range to the path segment where object is located so that it can be qualified as ghost*/
   float32_T k_cta_max_seg_heading_diff_no_ghost; /**<heading diff threshold for the path segment where object is currently located in of the nearest located path so that object is not qualified as ghost*/
   float32_T k_cta_max_heading_variance; /**<Maximum heading variance for valid objects.*/
   float32_T k_cta_dist_thres_crit_level_reset; /**<Indicates at which lateral distance the criticality level is reset. Distance is calculated as the % of vehicle width. Requires k_cta_f_enable_thres_crit_level_reset to be true.*/
   float32_T k_ctb_time_to_ask_for_final_brake_decel; /**<Time when the final brake deceleration shall be requested so that deceleration is increased incrementally.*/
   float32_T k_ctb_host_acc_weight; /**<This weight factor controls the influence of the acceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ctb_braking_jerk; /**<This describes the jerk of the ramp used for emergency braking. Describes an absolute value.*/
   float32_T k_ctb_const_decel_after_ramp_in; /**<Describes the final value of deceleration reached after the braking process. Describes an absolute value.*/
   float32_T k_ctb_ramp_in_time; /**<This describes the time before the maximum deceleration of the ramp for actuation of the brake in the event of emergency braking is reached.*/
   float32_T k_ctb_responsetime_brake_actuation; /**<Describes the time between output of brake request and activation of brake.*/
   float32_T k_ctb_max_braking_time; /**<Describes the maximal threshold value of TTC Interval for brake request.*/
   float32_T k_ctb_min_braking_time; /**<Describes the lower threshold value of TTC Interval for brake request.*/
   float32_T k_ctb_event_time_buffer; /**<Describes the buffer used additionally to the overall event time, so that no exact brake is needed.*/
   float32_T k_ctb_safety_dist_host_vel_lut[CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0]; /**<Depicts the host velocity dimension for an adaption of the upper safety distance threshold within CTB.*/
   float32_T k_ctb_upper_safety_distance_thres_lut[CTA_K_CTB_UPPER_SAFETY_DISTANCE_THRES_LUT_ARRAY_SIZE_DIM0]; /**<Describes the maximum distance at which the ego must remain in relation to the target's driving tube. This upper threshold is used for blocked scenarios. It is adapted dependent on the host vehicle velocity.*/
   float32_T k_ctb_lower_safety_distance_thres; /**<Describes the minimum distance at which the ego must remain in relation to the target's driving tube.*/
   float32_T k_cta_intersection_line_host_width_percentage; /**<The lateral position of the longitudinal intersection point is set as a percentage of the vehicle half vehicle width. (0.0 = host mid, 1.0 = host side).*/
   float32_T k_cta_min_ttc_additional_mature_qualification; /**<Above this parameter additional maturity cycles are required to trigger warning level.*/
   float32_T k_cta_ghost_condition_max_heading_diff_path_tracker; /**<This defines the maximum difference between the heading provided by the path algo and the tracker that is accaptable.*/
   float32_T k_cta_obj_dist_to_discard_pt_info; /**<Threshold for the distance between host and target so that path tracking information usage is disabled.*/
   float32_T k_cta_min_host_speed_to_discard_pt_info; /**<Threshold for the host speed so that path tracking information usage is disabled.*/
   float32_T k_cta_angles_zone_definition[CTA_K_CTA_ANGLES_ZONE_DEFINITION_ARRAY_SIZE_DIM0]; /**<Angles which can be used for definition of field of interest. Parameter 0 defines top border of that area measured from the horizontal line covering radar position up toward the x axis. 1 defines bottom border of that area measured from the horizontal line covering radar position down toward the x axis*/
   float32_T k_cta_heading_range[CTA_K_CTA_HEADING_RANGE_ARRAY_SIZE_DIM0]; /**<Parameter describing the permissible heading range of objects to be valid.*/
   float32_T k_cta_max_length_fov; /**<One of the parameter defining field of interest. This parameter defines maximum length of that area in polar coordinate system attached to the radar position.*/
   float32_T k_cta_max_speed; /**<Maximum speed of object to be a valid candidate for CTA.*/
   float32_T k_cta_rcta_host_speed_factor; /**<Factor for host speed-based adaptation of adaptation of the intersect lines associated with the criticality levels.*/
   float32_T k_cta_rel_warning_hysteresis; /**<Relative offset for the hysteresis of all relevant parameters for each criticality level (in percent).*/
   float32_T k_cta_min_rel_existence_probability; /**<Minimum existence probability of object.*/
   float32_T k_cta_min_lateral_approach_speed; /**<Minimum lateral speed of targets to be considered.*/
   float32_T k_cta_min_long_point_criticality_level[CTA_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0][CTA_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1]; /**<Lower border of field of interest located on the longitudinal axis in vcs. For RCTA the reference is on the rear bumper and for FCTA on the front bumper. First dimension = RCTA, Second dimension = FCTA.*/
   float32_T k_cta_max_long_point_criticality_level[CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0][CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1]; /**<Upper border of field of interest located on the longitudinal axis in vcs. For RCTA the reference is on the rear bumper and for FCTA on the front bumper. First dimension = RCTA, Second dimension = FCTA.*/
   float32_T k_cta_speed_criticality_level[CTA_K_CTA_SPEED_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0][CTA_K_CTA_SPEED_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1]; /**<Minimum target speed for the first and second warning level. First dimension = RCTA, Second dimension = FCTA*/
   float32_T k_cta_ttc_criticality_level[CTA_K_CTA_TTC_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0][CTA_K_CTA_TTC_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1]; /**<Maximum Offset of the target's TTC for the first warning level. First dimension = RCTA, Second dimension = FCTA*/
   float32_T k_cta_butterfly_lat[CTA_K_CTA_BUTTERFLY_LAT_ARRAY_SIZE_DIM0]; /**<The lateral coordinates of butterfly zone points. Total amount is restricted by k_cta_amount_butterfly_points_in_use*/
   float32_T k_cta_butterfly_long[CTA_K_CTA_BUTTERFLY_LONG_ARRAY_SIZE_DIM0]; /**<The longitudinal coordinates of butterfly zone points. Total amount is restricted by k_cta_amount_butterfly_points_in_use*/
   float32_T k_cta_min_speed; /**<Minimum speed for a target to be considered.*/
   float32_T k_cta_stop_alert_ttp; /**<Minimum TTP to stop a triggered alert early.*/
   float32_T k_cta_stop_alert_ttc; /**<Minimum TTC to stop a triggered alert early.*/
   float32_T k_cta_ego_abs_speed_max; /**<CTA will only work for absolute ego speed values equal or below this value.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Cta_Public_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_cta_ego_abs_speed_max; /**<CTA will only work for absolute ego speed values equal or below this value.*/
   float32_T k_cta_stop_alert_ttc; /**<Minimum TTC to stop a triggered alert early.*/
   float32_T k_cta_stop_alert_ttp; /**<Minimum TTP to stop a triggered alert early.*/
   float32_T k_cta_min_speed; /**<Minimum speed for a target to be considered.*/
   float32_T k_cta_butterfly_long[CTA_K_CTA_BUTTERFLY_LONG_ARRAY_SIZE_DIM0]; /**<The longitudinal coordinates of butterfly zone points. Total amount is restricted by k_cta_amount_butterfly_points_in_use*/
   float32_T k_cta_butterfly_lat[CTA_K_CTA_BUTTERFLY_LAT_ARRAY_SIZE_DIM0]; /**<The lateral coordinates of butterfly zone points. Total amount is restricted by k_cta_amount_butterfly_points_in_use*/
   float32_T k_cta_ttc_criticality_level[CTA_K_CTA_TTC_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0][CTA_K_CTA_TTC_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1]; /**<Maximum Offset of the target's TTC for the first warning level. First dimension = RCTA, Second dimension = FCTA*/
   float32_T k_cta_speed_criticality_level[CTA_K_CTA_SPEED_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0][CTA_K_CTA_SPEED_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1]; /**<Minimum target speed for the first and second warning level. First dimension = RCTA, Second dimension = FCTA*/
   float32_T k_cta_max_long_point_criticality_level[CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0][CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1]; /**<Upper border of field of interest located on the longitudinal axis in vcs. For RCTA the reference is on the rear bumper and for FCTA on the front bumper. First dimension = RCTA, Second dimension = FCTA.*/
   float32_T k_cta_min_long_point_criticality_level[CTA_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0][CTA_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1]; /**<Lower border of field of interest located on the longitudinal axis in vcs. For RCTA the reference is on the rear bumper and for FCTA on the front bumper. First dimension = RCTA, Second dimension = FCTA.*/
   float32_T k_cta_min_lateral_approach_speed; /**<Minimum lateral speed of targets to be considered.*/
   float32_T k_cta_min_rel_existence_probability; /**<Minimum existence probability of object.*/
   float32_T k_cta_rel_warning_hysteresis; /**<Relative offset for the hysteresis of all relevant parameters for each criticality level (in percent).*/
   float32_T k_cta_rcta_host_speed_factor; /**<Factor for host speed-based adaptation of adaptation of the intersect lines associated with the criticality levels.*/
   float32_T k_cta_max_speed; /**<Maximum speed of object to be a valid candidate for CTA.*/
   float32_T k_cta_max_length_fov; /**<One of the parameter defining field of interest. This parameter defines maximum length of that area in polar coordinate system attached to the radar position.*/
   float32_T k_cta_heading_range[CTA_K_CTA_HEADING_RANGE_ARRAY_SIZE_DIM0]; /**<Parameter describing the permissible heading range of objects to be valid.*/
   float32_T k_cta_angles_zone_definition[CTA_K_CTA_ANGLES_ZONE_DEFINITION_ARRAY_SIZE_DIM0]; /**<Angles which can be used for definition of field of interest. Parameter 0 defines top border of that area measured from the horizontal line covering radar position up toward the x axis. 1 defines bottom border of that area measured from the horizontal line covering radar position down toward the x axis*/
   float32_T k_cta_min_host_speed_to_discard_pt_info; /**<Threshold for the host speed so that path tracking information usage is disabled.*/
   float32_T k_cta_obj_dist_to_discard_pt_info; /**<Threshold for the distance between host and target so that path tracking information usage is disabled.*/
   float32_T k_cta_ghost_condition_max_heading_diff_path_tracker; /**<This defines the maximum difference between the heading provided by the path algo and the tracker that is accaptable.*/
   float32_T k_cta_min_ttc_additional_mature_qualification; /**<Above this parameter additional maturity cycles are required to trigger warning level.*/
   float32_T k_cta_intersection_line_host_width_percentage; /**<The lateral position of the longitudinal intersection point is set as a percentage of the vehicle half vehicle width. (0.0 = host mid, 1.0 = host side).*/
   float32_T k_ctb_lower_safety_distance_thres; /**<Describes the minimum distance at which the ego must remain in relation to the target's driving tube.*/
   float32_T k_ctb_upper_safety_distance_thres_lut[CTA_K_CTB_UPPER_SAFETY_DISTANCE_THRES_LUT_ARRAY_SIZE_DIM0]; /**<Describes the maximum distance at which the ego must remain in relation to the target's driving tube. This upper threshold is used for blocked scenarios. It is adapted dependent on the host vehicle velocity.*/
   float32_T k_ctb_safety_dist_host_vel_lut[CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0]; /**<Depicts the host velocity dimension for an adaption of the upper safety distance threshold within CTB.*/
   float32_T k_ctb_event_time_buffer; /**<Describes the buffer used additionally to the overall event time, so that no exact brake is needed.*/
   float32_T k_ctb_min_braking_time; /**<Describes the lower threshold value of TTC Interval for brake request.*/
   float32_T k_ctb_max_braking_time; /**<Describes the maximal threshold value of TTC Interval for brake request.*/
   float32_T k_ctb_responsetime_brake_actuation; /**<Describes the time between output of brake request and activation of brake.*/
   float32_T k_ctb_ramp_in_time; /**<This describes the time before the maximum deceleration of the ramp for actuation of the brake in the event of emergency braking is reached.*/
   float32_T k_ctb_const_decel_after_ramp_in; /**<Describes the final value of deceleration reached after the braking process. Describes an absolute value.*/
   float32_T k_ctb_braking_jerk; /**<This describes the jerk of the ramp used for emergency braking. Describes an absolute value.*/
   float32_T k_ctb_host_acc_weight; /**<This weight factor controls the influence of the acceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ctb_time_to_ask_for_final_brake_decel; /**<Time when the final brake deceleration shall be requested so that deceleration is increased incrementally.*/
   float32_T k_cta_dist_thres_crit_level_reset; /**<Indicates at which lateral distance the criticality level is reset. Distance is calculated as the % of vehicle width. Requires k_cta_f_enable_thres_crit_level_reset to be true.*/
   float32_T k_cta_max_heading_variance; /**<Maximum heading variance for valid objects.*/
   float32_T k_cta_max_seg_heading_diff_no_ghost; /**<heading diff threshold for the path segment where object is currently located in of the nearest located path so that object is not qualified as ghost*/
   float32_T k_cta_range_to_path_segment_ghost_qualif; /**<range to the path segment where object is located so that it can be qualified as ghost*/
   float32_T k_cta_object_heading_exp_moving_average_alpha; /**<Set alpha value for exponential moving average filter for object heading. Higher alpha means more weight for current values.*/
   float32_T k_cta_pedestrian_min_size; /**<Minimum size for an object classified as pedestrian to be selected as a valid candidate for CTA when other criteria are met*/
   float32_T k_cta_pedestrian_min_speed; /**<Minimum speed for an object classified as pedestrian to be selected as a valid candidate for CTA when other criteria are met*/
   float32_T k_cta_2wheel_min_size; /**<Minimum size for an object classified as 2wheel to be selected as a valid candidate for CTA when other criteria are met*/
   float32_T k_cta_2wheel_min_speed; /**<Minimum speed for an object classified as 2wheel to be selected as a valid candidate for CTA when other criteria are met*/
   float32_T k_cta_min_deceleration_value; /**<Minimum deceleration value for braking*/
   float32_T k_cta_max_deceleration_value; /**<Maximum deceleration value for braking*/
   float32_T k_cta_ttc_calc_positive_ref_point; /**<Maximum deceleration value for braking*/
   float32_T k_cta_speed_thresh_for_rel_vel_calc; /**<Below this threshold object relative velocity will be calculated by using its heading */
   boolean_T k_cta_f_adapt_intersect_lines_by_steering_angle; /**<Switch to turn on or off steering-angle-based adaptation of the intersect lines associated with the criticality levels.*/
   boolean_T k_cta_f_adapt_intersect_lines_by_obj_heading; /**<Switch to turn on or off object-heading-based adaptation of the intersect lines associated with the criticality levels.*/
   boolean_T k_cta_f_adapt_intersect_lines_by_host_speed; /**<Switch to turn on or off host-speed-based adaptation of the intersect lines associated with the criticality levels.*/
   boolean_T k_cta_f_prevent_fall_back_to_critlevel_1; /**<Set to 0: Criticality level may drop to level 1 once it has been higher than 1; set to 1: criticality level will remain higher than 1 once it has been higher than 1 even though the conditions for the higher level are not met any longer.*/
   boolean_T k_cta_f_apply_heading_compensation_on_intersection_point; /**<Switch to turn on or off the adaption of calculated intersection point by a value depending on target heading and ego width.*/
   boolean_T k_cta_f_calc_ttc_ego_side_enabled; /**<Switch to turn on or off calculate TTC with reference to the ego side instead of x-axis defined in VCS.*/
   boolean_T k_cta_f_calc_ttp_ego_side_enabled; /**<Switch to turn on or off calculate TTP with reference to the ego side instead of x-axis defined in VCS.*/
   boolean_T k_cta_f_use_heading_for_relative_velocity_calculation; /**<If flag is set to true, the relative velocity vector of an object will be calculated by making use of heading and absolute velocity of an object. If it is set to false, the relative velocity provided by the tracker will be used.*/
   boolean_T k_cta_f_apply_path_tracking; /**<Switch to turn on or off path tracking algorithm.*/
   boolean_T k_cta_f_discard_pt_heading_when_moving; /**<Switch to turn on or off path tracking when moving backwards when additional conditions are fulfilled (min_host_speed_to_discard_pt_info, obj_dist_to_discard_pt_info).*/
   boolean_T k_cta_f_use_object_supress_counter; /**<This flag indicates whether the object_supress_counter shall be used.*/
   boolean_T k_cta_f_use_ghost_detector; /**<This flags activates the check whether the object is a ghost within the object validation.*/
   boolean_T k_cta_f_use_object_min_object_age_in_cycles; /**<Flag indicating whether object age threshold is used to check cta relevance for this object.*/
   boolean_T k_cta_f_use_rel_vel_isect_point_calc; /**<Flag indicating whether to use relative velocity or heading of tracker to calculate intersection point.*/
   boolean_T k_cta_f_enable_thres_crit_level_reset; /**<Flag indicating whether to reset the criticality level when an object passes a user defined threshold.*/
   boolean_T k_cta_f_use_front_corners_dist_stop; /**<Flag indicating to use front corners of an object to check, whether it has already passed a user defined longitudinal axis so that alert can be turned off. Default are rear corners.*/
   boolean_T k_cta_enable_ctb; /**<This flag enables CTB if set to TRUE and disables it if set to FALSE.*/
   boolean_T k_cta_f_brake_overriding_ctb; /**<Flag used to enable CTB overriding.*/
   boolean_T k_cta_f_use_brake_gradient; /**<Flag used to enable the usage of the k_cta_braking_gradient instead of calculating it from k_cta_target_acceleration_target_braking and k_cta_target_acceleration_target_braking_time.*/
   boolean_T k_cta_f_enable_heading_exp_moving_average; /**<Indicates if the exponential moving average function for the object heading is enabled.*/
   boolean_T k_cta_f_stop_mode_ttp; /**<Summarizes the alert stop modes for CTA. If flag is enabled we use TTP threshold for alert suppresion, otherwise we use TTC*/
   uint8_t k_cta_amount_butterfly_points_in_use; /**<Actual amount of points which are in use for the definition of butterfly zone. Must be less than maximum allowed butterfly points*/
   uint8_t k_cta_enable_modes[CTA_K_CTA_ENABLE_MODES_ARRAY_SIZE_DIM0]; /**<This calibration enables either rear or front CTA. First dimension = RCTA, Second dimension = FCTA.*/
   uint8_t k_cta_cycle_count_suppress_true_warning; /**<Qualifier for the number of cycles required for each critical level.*/
   uint8_t k_cta_cycle_count_hold_true_warning; /**<Minimum number of cycles requiered to hold each criticality level.*/
   uint8_t k_cta_min_object_age_check_valid; /**<Minimal age of an object in cycles to be considered by the algorithm.*/
   uint8_t k_cta_min_object_age_thres; /**<Minimal age of an object in cycles used for criticality level calculation.*/
   uint8_t k_cta_cycles_coasted_to_ignore; /**<Defines the cycles that an object can maximal be coasted to be still considered by the algo.*/
   uint8_t k_cta_object_supress_counter; /**<This counter supresses objects that are untrusted for a number of cycles.*/
   uint8_t k_cta_ghost_validation_min_age; /**<Min age of a potential ghost to be considerd by the object validation.*/
   uint8_t k_cta_ghost_validation_min_mature; /**<Min mature age of a potential ghost to be considerd by the object validation.*/
   uint8_t k_cta_min_mature_cycles_level_qualifiction; /**<Minimum required number of consecutive cycles for an object status to be mature for activation of a criticality level.*/
   uint8_t k_cta_additional_qualification_mature_cycles; /**<Additional maturity cycles for an object to qualify for a warning level.*/
   uint8_t k_ctb_min_brake_qual_ctr_thres; /**<Qualification counter threshold for braking logic.*/
   uint8_t k_ctb_min_brake_hold_ctr_thres; /**<Holds the brake qualifier on each side for at least this specified amount of cycles.*/
   uint8_t k_cta_min_qual_age_obj_crossing_paths; /**<age of the object which is necessary to qualify in case of a lane change, when it is crossing its nearest path*/
   uint8_t k_cta_cycles_valid_match_of_pot_ghost; /**<Cycles for an object to be matched to a path with a minimum confidence so that it is not classified as potential ghost anymore*/
   uint8_t k_cta_age_for_new_creation_below_long_intersection; /**<When an object is newly created directly in the longitudinal area between host width and longitudinal intersection, it shall be suppressed.*/
} Cta_Public_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* CTA_PUBLIC_CALIBRATION_T_H */
