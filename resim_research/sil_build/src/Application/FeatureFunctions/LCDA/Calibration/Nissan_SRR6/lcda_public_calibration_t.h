# ifndef LCDA_PUBLIC_CALIBRATION_T_H
# define LCDA_PUBLIC_CALIBRATION_T_H

/**
* @file lcda_public_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in lcda_cal.xml.
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
#define LCDA_K_BSW_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_ZONE_X_HYS_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_ZONE_Y_HYS_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_FIXED_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_FIXED_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_FIXED_ZONE_X_HYS_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_FIXED_ZONE_Y_HYS_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_SPEED_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_RANGE_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_Y_HYS_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_OBJECT_POSITION_CORRECTION_DELAY_TIME_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_MIN_OBJECT_CURVI_RELATIVE_SPEED_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_LCDA_CVW_TTC_CONST_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_LCDA_CVW_TTC_ACCEL_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_LCDA_DYN_CVW_TTC_COMPENS_TIME_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_OBJECT_REL_VEL_ARRAY_SIZE_DIM0 (10u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_OBJECT_RANGE_ARRAY_SIZE_DIM0 (10u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_ZONE_X_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_ZONE_Y_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_FIXED_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_FIXED_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_FIXED_ZONE_X_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_FIXED_ZONE_Y_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_RANGE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_Y_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_OBJECT_POSITION_CORRECTION_DELAY_TIME_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_MIN_OBJECT_CURVI_RELATIVE_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_LCDA_CVW_TTC_CONST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_LCDA_CVW_TTC_ACCEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_LCDA_DYN_CVW_TTC_COMPENS_TIME_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_OBJECT_REL_VEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_OBJECT_RANGE_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_PUBLIC_CALIBRATION_SIZE (778u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_cvw_min_mature_cycles; /**<Minimum number of mature cylces in zone as a valid cvw target before issuing an alert.*/
   uint8_t k_bsw_alert_track_age; /**<Minimum Track age to trigger a warning immediately even though the object does not pass the mature in zone check.*/
   uint8_t k_lcda_min_track_age; /**<Minimum Track age for an object to be selected as a valid candidate for LCDA.*/
   uint8_t k_bsw_min_mature_cycles; /**<Minimum number of mature cycles in zone as a valid bsw target before issuing an alert.*/
   uint8_t k_lcda_zone_check_method; /**<Sets the method for checking the object zone occupation. 0: Checks zone overlap, 1: Only check reference point.*/
   uint8_t k_cvw_alert_holding_cycles; /**<Number of holding cycles for CVW alert.*/
   uint8_t k_bsw_alert_holding_cycles; /**<Number of holding cycles for BSW alert.*/
   boolean_T k_bsw_f_enable_object_rel_vel_dynzone; /**<Enables dynamic BSW zone adaptation based on object relative velocity.*/
   boolean_T k_bsw_f_use_front_zone_as_n_line; /**<Determines whether front zone line should be used as n line is SOT scenario*/
   boolean_T k_bsw_shrink_zone_method; /**<Determines whether zone will be shrunk using 0 - multiplication or 1 - subtraction ()*/
   boolean_T k_lcda_f_enable_dyn_cvw_ttc_threshold; /**<Enables the CVW dynamic ttc threshold.*/
   boolean_T k_lcda_f_enable_fallback_handler; /**<Enables fallback handler module*/
   boolean_T k_lcda_f_enable_obj_reflection_flag_check; /**<Enables f_reflection flag checking to ignore potential ghost objects, detected by the tracker*/
   boolean_T k_honda_srr6_enable_alert_hold_due_out_of_fov; /**<Defines if the holding of Honda alert due to object lost in radar fov is turned on*/
   boolean_T k_honda_srr6_enable_alert_hold_due_slow_down; /**<Defines if the holding of Honda alert due to ego slowing down is turned on.*/
   boolean_T k_cvw_enable_via_cal; /**<Used to decide whether CVW should be enabled based on the calibration. TRUE=>k_cvw_enable is used to enable CVW, FALSE=>cvwEnabled flag from the core input is used to enable CVW.*/
   boolean_T k_cvw_enable; /**<Enables the CVW feature when the k_cvw_enable_via_cal is also set to TRUE.*/
   boolean_T k_bsw_enable_trailer_zone_extension; /**<Enables an additional adjustment of the BSW zone based on trailer information in core input.*/
   boolean_T k_bsw_enable_dynspeed_zone; /**<Enables dynamic BSW zone adaptation based on ego speed.*/
   boolean_T k_bsw_use_curvi_coordinates; /**<When set to TRUE the BSW module will always use curvi coordinates for every check. Otherwise VCS coordinates will be used.*/
   boolean_T k_bsw_overlap_area_check_enable; /**<Enables verification of target FOI area overlaped by BSW zone.*/
   boolean_T k_bsw_uses_cvw_alert_state_enabled; /**<Enables use of CVW alert state info in BSW 1) BSW hys zone is used directly 2) BSW mature count in zone is bypassed if there is a previous CVW alert on the object.*/
   boolean_T k_lcda_ego_lane_check_center_point_only; /**<Enables the simple center point check to determine ego lane occupation. If disabled a more sophisticated polygon overlap is performed.*/
   boolean_T k_lcda_f_enable_alert_obj_in_ego_lane; /**<Enable alert when object is in ego lane and in BSW zone.*/
   boolean_T k_lcda_f_enable_obj_in_ego_lane_check; /**<Enable checking if an object is in the ego lane. When this is set to true, alerts from objects that are considered to be in the ego lane will be suppressed.*/
   boolean_T k_lcda_f_disable_due_to_small_curve_radius; /**<Decides whether entire LCDA functionality should be disabled when the host is moving with a very small curve radius such as a round about.*/
   float32_T k_bsw_dynzone_object_range[LCDA_K_BSW_DYNZONE_OBJECT_RANGE_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the object relative velocity  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_dynzone_object_rel_vel[LCDA_K_BSW_DYNZONE_OBJECT_REL_VEL_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the object relative velocity  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_lcda_dyn_cvw_ttc_compens_time[LCDA_K_LCDA_DYN_CVW_TTC_COMPENS_TIME_ARRAY_SIZE_DIM0]; /**<Determines time that will be added to ttc threshold to compensate system delays - first value below thresh, second above thresh */
   float32_T k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh; /**<Relative velocity above which higher compensation value will be added to ttc threshold to compensate system delays*/
   float32_T k_bsw_object_position_correction_threshold; /**<Determines relative longitudinal velocity threshold above which higher correction position value will be applied.*/
   float32_T k_lcda_dyn_cvw_ttc_speed_parameter; /**<None*/
   float32_T k_lcda_honda_narrow_beeper_max_speed_l; /**<Maximum host speed below which narrow beeper zone will be used immediately*/
   float32_T k_lcda_honda_narrow_beeper_max_speed_h; /**<Maximum host speed above which narrow beeper zone will not be used*/
   float32_T k_lcda_cvw_ttc_accel[LCDA_K_LCDA_CVW_TTC_ACCEL_ARRAY_SIZE_DIM0]; /**<Acceleration value used to calculate the dynamic TTC of the CVW module*/
   float32_T k_lcda_cvw_ttc_const[LCDA_K_LCDA_CVW_TTC_CONST_ARRAY_SIZE_DIM0]; /**<Constant value used to calculate the dynamic TTC of the CVW module*/
   float32_T k_cvw_object_curvi_relative_speed_hys; /**<Hysteresis value of relative object speed to be candidate*/
   float32_T k_cvw_max_object_curvi_relative_speed; /**<Max relative object speed to be candidate*/
   float32_T k_cvw_min_object_curvi_relative_speed[LCDA_K_CVW_MIN_OBJECT_CURVI_RELATIVE_SPEED_ARRAY_SIZE_DIM0]; /**<Min relative object speed to be candidate*/
   float32_T k_bsw_min_speed_for_tos_scenario; /**<Min relative target speed required to trigger the TOS scenario*/
   float32_T k_bsw_object_position_correction_delay_time[LCDA_K_BSW_OBJECT_POSITION_CORRECTION_DELAY_TIME_ARRAY_SIZE_DIM0]; /**<Corrects delay time from tracker, and based on that value we shift longitudinal object position during holding.*/
   float32_T k_lcda_2wheel_min_speed; /**<Minimum speed for an object classified as 2wheel to be selected as a valid candidate for LCDA*/
   float32_T k_lcda_2wheel_min_size; /**<Minimum size for an object classified as 2wheel to be selected as a valid candidate for LCDA*/
   float32_T k_lcda_pedestrian_min_speed; /**<Minimum speed for an object classified as pedestrian to be selected as a valid candidate for LCDA*/
   float32_T k_lcda_pedestrian_min_size; /**<Minimum size for an object classified as pedestrian to be selected as a valid candidate for LCDA*/
   float32_T k_bsw_zone_rear_outer_side_y_hys; /**<Hysteresis of the lateral position of the inner(close to ego) rear vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_rear_outer_side_x_hys; /**<Hysteresis of the longintudinal position of the inner(close to ego) rear vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_front_ego_side_y_hys; /**<Hysteresis of the lateral position of the inner(close to ego) front vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_front_ego_side_x_hys; /**<Hysteresis of the longintudinal position of the inner(close to ego) front vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_rear_outer_side_y; /**<Lateral position of the inner(close to ego) rear vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_rear_outer_side_x; /**<Longintudinal position of the inner(close to ego) rear vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_front_ego_side_y; /**<Lateral position of the inner(close to ego) front vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_front_ego_side_x; /**<Longintudinal position of the inner(close to ego) front vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_obj_max_rel_vel_hys; /**<Hysteresis applied to maximum realitive velocity check if object triggered alert in previous cycle*/
   float32_T k_bsw_obj_max_rel_vel_thresh; /**<Maximum realitive velocity for which alert should be triggered*/
   float32_T k_honda_alert_level_two_holding_time; /**<Defines the duration of the Honda level 2 alert*/
   float32_T k_honda_beeper_zone_width; /**<Width of the beeper zone*/
   float32_T k_honda_beeper_zone_length; /**<Length of the beeper zone*/
   float32_T k_honda_ego_speed_stop_holding; /**<Defines ego speed below which the alert holding is terminated.*/
   float32_T k_lka_ov_zone_width; /**<LKA zone width.*/
   float32_T k_bsw_x0; /**<The longitudinal distance of start point p0 of the BSW zone (ref point is center of the ego rear bumper).*/
   float32_T k_bsw_x_length; /**<The longitudinal length of the BSW zone (offset of p2 from p0 with ref point at center of the ego rear bumper).*/
   float32_T k_bsw_y0; /**<Lateral distance of start point p0 of BSW Zone for right side (ref point at center of ego rear bumper).*/
   float32_T k_bsw_y_width; /**<The lateral width of the BSW zone (offset of p0 from p5 for right side zone).*/
   float32_T k_cvw_x0; /**<The longitudinal distance of start point p0 of the CVW zone for right side (ref point is center of ego rear bumper).*/
   float32_T k_cvw_y0; /**<Lateral distance of start point p5 of right side CVW Zone (ref point is center of ego rear bumper).*/
   float32_T k_cvw_y_width0; /**<Lateral width of CVW zone (lat offset of p0 from p5 for right side zone).*/
   float32_T k_cvw_x_length0; /**<Longitudinal length of CVW zone (long offset of p1 from p0 with ref point at center of ego rear bumper).*/
   float32_T k_cvw_y1; /**<Lateral start point p3 of right side CVW zone (ref point is center of the ego rear bumper).*/
   float32_T k_cvw_x_length1; /**<Longitudinal length of CVW zone (long offset of p2 from p1 with ref point at center of the ego rear bumper).*/
   float32_T k_cvw_y_width1; /**<Lateral width of CVW zone (lat offset of p2 from p3 for right side zone).*/
   float32_T k_cvw_min_obj_curvi_long_vel; /**<The minimum longitudinal curvi velocity of an object for the cvw zone.*/
   float32_T k_cvw_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects in the cvw zone.*/
   float32_T k_cvw_ttc_hys; /**<Added to TTC threshold when an alert is active.*/
   float32_T k_cvw_ttc; /**<Maximum TTC to issue an alert.*/
   float32_T k_cvw_candidate_ttc; /**<If TTC of an object is lower than this threshold it is considered a candidate for a CVW alert.*/
   float32_T k_cvw_gap_bridge; /**<Parameter describing the time between two alerts, when the Alert should not go off.*/
   float32_T k_cvw_zone_y_hys_min; /**<Min value of the hysterese [m] of the cvw. Offset added to initial zone.*/
   float32_T k_cvw_zone_y_hys_max; /**<Max value of the hysterese [m] of the cvw. Offset added to initial zone.*/
   float32_T k_cvw_zone_y_hys[LCDA_K_CVW_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset as a factor of the lane width that is to be added/subtracted from the initial CVW zone y-coord to get the hysteresis zone y-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_cvw_zone_y[LCDA_K_CVW_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining cvw zone in curvi coordinates. It is specified as a factor of the lane width.*/
   float32_T k_cvw_zone_x[LCDA_K_CVW_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining cvw zone in curvi coordinates.*/
   float32_T k_bsw_trailer_zone_ext_safety_margin_hys; /**<Hysteresis for longitudinal extension of the rear edge of the bsw zone behind the trailer edge.*/
   float32_T k_bsw_trailer_zone_ext_safety_margin; /**<Longitudinal extension of the rear edge of the bsw zone behind the trailer edge.*/
   float32_T k_bsw_max_obj_long_vel_hysteresis; /**<Hysteresis of the maximum longitudinal velocity in vcs of an object for the bsw zone, that is added to k_bsw_max_obj_long_vel.*/
   float32_T k_bsw_min_obj_long_vel_hysteresis; /**<Hysteresis of the minimum longitudinal velocity in vcs of an object for the bsw zone, that is added to k_bsw_min_obj_long_vel.*/
   float32_T k_bsw_max_obj_long_vel; /**<The maximum longitudinal velocity in vcs of an object for the bsw zone.*/
   float32_T k_bsw_min_obj_long_vel; /**<The minimum longitudinal velocity in vcs of an object for the bsw zone.*/
   float32_T k_bsw_max_heading_abs_hysteresis; /**<Hysteresis of the maximum allowed absolute heading for objects in the bsw zone.*/
   float32_T k_bsw_max_heading_abs; /**<The maximum allowed absolute heading for objects in the bsw zone.*/
   float32_T k_bsw_suppress_late_warning_max_time_till_leave; /**<Maximum time until the object leaves the rear bsd zone border in which a BSD warning can be suppressed.*/
   float32_T k_bsw_line_to_stop_TOS_alert; /**<Sets the longitudinal line for front bsw zone boundary to disable alert for TOS case. Requires k_bsw_stop_alert_reaching_front_custom_limit_mode = 2*/
   float32_T k_bsw_n_line_position_for_long_object_sot_scenario; /**<In SOT scenario alert is active when the rear of long object reaches this line. Close to the position of rear host bumper (N line) in VCS coordinates (shall be negative) */
   float32_T k_bsw_dynzone_range[LCDA_K_BSW_DYNZONE_RANGE_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the host vehicle speed  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_dynzone_speed[LCDA_K_BSW_DYNZONE_SPEED_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the host vehicle speed  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_fallback_rel_vel_thres_hys; /**<A target can be reclassified as fast back falling object if the relative velocity is below k_bsw_fallback_rel_vel_thres - k_bsw_fallback_rel_vel_thres_hys.*/
   float32_T k_bsw_fallback_rel_vel_thres; /**<A target will be considered as slow back falling object if the relative velocity is above threshold.*/
   float32_T k_bsw_overlap_area_threshold; /**<Mimium area of target FOI, overlapped by BSW zone to raise BSW alert, expressed in percentage of FOI.*/
   float32_T k_bsw_fixed_zone_y_hys[LCDA_K_BSW_FIXED_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset value that will be added to the initial fixed BSW zone points y-coord to calculate the hysteresis zone y-coord values. Note hys zone is always larger than the initial zone.*/
   float32_T k_bsw_fixed_zone_x_hys[LCDA_K_BSW_FIXED_ZONE_X_HYS_ARRAY_SIZE_DIM0]; /**<Offset value that will be added to the initial fixed BSW zone points x-coord to calculate the hysteresis zone x-coord values. Note hys zone is always larger than the initial zone.*/
   float32_T k_bsw_fixed_zone_y[LCDA_K_BSW_FIXED_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining bsw zone in VCS coordinates. Used for mode BSW_ZONE_CALC_FIXED_ZONE_VCS.*/
   float32_T k_bsw_fixed_zone_x[LCDA_K_BSW_FIXED_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining bsw zone in VCS coordinates. Used for mode BSW_ZONE_CALC_FIXED_ZONE_VCS.*/
   float32_T k_bsw_zone_y_hys_max; /**<Max value of the hysteresis [m] of the bsw, which is calculated via the object width.*/
   float32_T k_bsw_zone_y_hys_min; /**<Min value of the hysteresis [m] of the bsw, which is calculated via the object width.*/
   float32_T k_bsw_zone_y_hys[LCDA_K_BSW_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset value that will be added (or subtracted) to the initial BSW zone points y-coord to calculate the hysteresis zone y-coord values. Note hys zone is always larger than the initial zone.*/
   float32_T k_bsw_zone_x_hys[LCDA_K_BSW_ZONE_X_HYS_ARRAY_SIZE_DIM0]; /**<Offset value that will be added to the initial BSW zone points x-coord to calculate the hysteresis zone x-coord values. Note hys zone is always larger than the initial zone.*/
   float32_T k_bsw_zone_y[LCDA_K_BSW_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining bsw zone in VCS coordinates.*/
   float32_T k_bsw_zone_x[LCDA_K_BSW_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining bsw zone in VCS coordinates.*/
   float32_T k_lcda_ego_lane_effective_lane_width_factor; /**<Lane width factor to be used when determining if a LCDA object is within the ego lane. Value should be between 0 and 1.*/
   float32_T k_bsw_lane_change_intention_pos_long_thres; /**<Threshold for longitudinal position below which alert won't be suppressed.*/
   float32_T k_bsw_lane_change_intention_pos_lat_thres; /**<Threshold for lateral zone occupation above which alert won't be suppressed.*/
   float32_T k_lcda_lane_change_intention_vel_lat_thresh; /**<Threshold for relative velocity above which alert suppression functionality would be enabled.*/
   float32_T k_lcda_zone_intersect_critical_point_lateral_ratio; /**<Determines which point is checked for objects heading towards the zone if CRITICAL_POINT_IN_ZONE is used as intersect detection. 0.0=lateral center of object, 1.0=lateral inner edge.*/
   float32_T k_lcda_min_curve_radius_hys; /**<Hysteresis value to be applied before LCDA is disabled for small curve radius.*/
   float32_T k_lcda_min_curve_radius; /**<LCDA is disabled when the host is navigating a curve with a radius smaller than this value.*/
   float32_T k_lcda_distance_traveled_scale_factor; /**<Inhibits the amount by which the CVW zone length restores after it was shortened. The restoration (travelled distance of ego) is multiplied by this value.*/
   float32_T k_lcda_host_activation_speed_max_hys; /**<Hysteresis for k_lcda_host_activation_speed_min_max. This value will be added to maximal activation speed threshold if LCDA is currently active.*/
   float32_T k_lcda_host_activation_speed_max; /**<Maximal host speed for which LCDA shall be active.*/
   float32_T k_lcda_host_activation_speed_min_hys; /**<Hysteresis for k_lcda_host_activation_speed_min_min. This value will be subtracted from minimal activation speed threshold if LCDA is currently active.*/
   float32_T k_lcda_host_activation_speed_min; /**<Minimal host speed for which LCDA shall be active.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Lcda_Public_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_lcda_host_activation_speed_min; /**<Minimal host speed for which LCDA shall be active.*/
   float32_T k_lcda_host_activation_speed_min_hys; /**<Hysteresis for k_lcda_host_activation_speed_min_min. This value will be subtracted from minimal activation speed threshold if LCDA is currently active.*/
   float32_T k_lcda_host_activation_speed_max; /**<Maximal host speed for which LCDA shall be active.*/
   float32_T k_lcda_host_activation_speed_max_hys; /**<Hysteresis for k_lcda_host_activation_speed_min_max. This value will be added to maximal activation speed threshold if LCDA is currently active.*/
   float32_T k_lcda_distance_traveled_scale_factor; /**<Inhibits the amount by which the CVW zone length restores after it was shortened. The restoration (travelled distance of ego) is multiplied by this value.*/
   float32_T k_lcda_min_curve_radius; /**<LCDA is disabled when the host is navigating a curve with a radius smaller than this value.*/
   float32_T k_lcda_min_curve_radius_hys; /**<Hysteresis value to be applied before LCDA is disabled for small curve radius.*/
   float32_T k_lcda_zone_intersect_critical_point_lateral_ratio; /**<Determines which point is checked for objects heading towards the zone if CRITICAL_POINT_IN_ZONE is used as intersect detection. 0.0=lateral center of object, 1.0=lateral inner edge.*/
   float32_T k_lcda_lane_change_intention_vel_lat_thresh; /**<Threshold for relative velocity above which alert suppression functionality would be enabled.*/
   float32_T k_bsw_lane_change_intention_pos_lat_thres; /**<Threshold for lateral zone occupation above which alert won't be suppressed.*/
   float32_T k_bsw_lane_change_intention_pos_long_thres; /**<Threshold for longitudinal position below which alert won't be suppressed.*/
   float32_T k_lcda_ego_lane_effective_lane_width_factor; /**<Lane width factor to be used when determining if a LCDA object is within the ego lane. Value should be between 0 and 1.*/
   float32_T k_bsw_zone_x[LCDA_K_BSW_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining bsw zone in VCS coordinates.*/
   float32_T k_bsw_zone_y[LCDA_K_BSW_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining bsw zone in VCS coordinates.*/
   float32_T k_bsw_zone_x_hys[LCDA_K_BSW_ZONE_X_HYS_ARRAY_SIZE_DIM0]; /**<Offset value that will be added to the initial BSW zone points x-coord to calculate the hysteresis zone x-coord values. Note hys zone is always larger than the initial zone.*/
   float32_T k_bsw_zone_y_hys[LCDA_K_BSW_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset value that will be added (or subtracted) to the initial BSW zone points y-coord to calculate the hysteresis zone y-coord values. Note hys zone is always larger than the initial zone.*/
   float32_T k_bsw_zone_y_hys_min; /**<Min value of the hysteresis [m] of the bsw, which is calculated via the object width.*/
   float32_T k_bsw_zone_y_hys_max; /**<Max value of the hysteresis [m] of the bsw, which is calculated via the object width.*/
   float32_T k_bsw_fixed_zone_x[LCDA_K_BSW_FIXED_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining bsw zone in VCS coordinates. Used for mode BSW_ZONE_CALC_FIXED_ZONE_VCS.*/
   float32_T k_bsw_fixed_zone_y[LCDA_K_BSW_FIXED_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining bsw zone in VCS coordinates. Used for mode BSW_ZONE_CALC_FIXED_ZONE_VCS.*/
   float32_T k_bsw_fixed_zone_x_hys[LCDA_K_BSW_FIXED_ZONE_X_HYS_ARRAY_SIZE_DIM0]; /**<Offset value that will be added to the initial fixed BSW zone points x-coord to calculate the hysteresis zone x-coord values. Note hys zone is always larger than the initial zone.*/
   float32_T k_bsw_fixed_zone_y_hys[LCDA_K_BSW_FIXED_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset value that will be added to the initial fixed BSW zone points y-coord to calculate the hysteresis zone y-coord values. Note hys zone is always larger than the initial zone.*/
   float32_T k_bsw_overlap_area_threshold; /**<Mimium area of target FOI, overlapped by BSW zone to raise BSW alert, expressed in percentage of FOI.*/
   float32_T k_bsw_fallback_rel_vel_thres; /**<A target will be considered as slow back falling object if the relative velocity is above threshold.*/
   float32_T k_bsw_fallback_rel_vel_thres_hys; /**<A target can be reclassified as fast back falling object if the relative velocity is below k_bsw_fallback_rel_vel_thres - k_bsw_fallback_rel_vel_thres_hys.*/
   float32_T k_bsw_dynzone_speed[LCDA_K_BSW_DYNZONE_SPEED_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the host vehicle speed  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_dynzone_range[LCDA_K_BSW_DYNZONE_RANGE_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the host vehicle speed  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_n_line_position_for_long_object_sot_scenario; /**<In SOT scenario alert is active when the rear of long object reaches this line. Close to the position of rear host bumper (N line) in VCS coordinates (shall be negative) */
   float32_T k_bsw_line_to_stop_TOS_alert; /**<Sets the longitudinal line for front bsw zone boundary to disable alert for TOS case. Requires k_bsw_stop_alert_reaching_front_custom_limit_mode = 2*/
   float32_T k_bsw_suppress_late_warning_max_time_till_leave; /**<Maximum time until the object leaves the rear bsd zone border in which a BSD warning can be suppressed.*/
   float32_T k_bsw_max_heading_abs; /**<The maximum allowed absolute heading for objects in the bsw zone.*/
   float32_T k_bsw_max_heading_abs_hysteresis; /**<Hysteresis of the maximum allowed absolute heading for objects in the bsw zone.*/
   float32_T k_bsw_min_obj_long_vel; /**<The minimum longitudinal velocity in vcs of an object for the bsw zone.*/
   float32_T k_bsw_max_obj_long_vel; /**<The maximum longitudinal velocity in vcs of an object for the bsw zone.*/
   float32_T k_bsw_min_obj_long_vel_hysteresis; /**<Hysteresis of the minimum longitudinal velocity in vcs of an object for the bsw zone, that is added to k_bsw_min_obj_long_vel.*/
   float32_T k_bsw_max_obj_long_vel_hysteresis; /**<Hysteresis of the maximum longitudinal velocity in vcs of an object for the bsw zone, that is added to k_bsw_max_obj_long_vel.*/
   float32_T k_bsw_trailer_zone_ext_safety_margin; /**<Longitudinal extension of the rear edge of the bsw zone behind the trailer edge.*/
   float32_T k_bsw_trailer_zone_ext_safety_margin_hys; /**<Hysteresis for longitudinal extension of the rear edge of the bsw zone behind the trailer edge.*/
   float32_T k_cvw_zone_x[LCDA_K_CVW_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining cvw zone in curvi coordinates.*/
   float32_T k_cvw_zone_y[LCDA_K_CVW_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining cvw zone in curvi coordinates. It is specified as a factor of the lane width.*/
   float32_T k_cvw_zone_y_hys[LCDA_K_CVW_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset as a factor of the lane width that is to be added/subtracted from the initial CVW zone y-coord to get the hysteresis zone y-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_cvw_zone_y_hys_max; /**<Max value of the hysterese [m] of the cvw. Offset added to initial zone.*/
   float32_T k_cvw_zone_y_hys_min; /**<Min value of the hysterese [m] of the cvw. Offset added to initial zone.*/
   float32_T k_cvw_gap_bridge; /**<Parameter describing the time between two alerts, when the Alert should not go off.*/
   float32_T k_cvw_candidate_ttc; /**<If TTC of an object is lower than this threshold it is considered a candidate for a CVW alert.*/
   float32_T k_cvw_ttc; /**<Maximum TTC to issue an alert.*/
   float32_T k_cvw_ttc_hys; /**<Added to TTC threshold when an alert is active.*/
   float32_T k_cvw_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects in the cvw zone.*/
   float32_T k_cvw_min_obj_curvi_long_vel; /**<The minimum longitudinal curvi velocity of an object for the cvw zone.*/
   float32_T k_cvw_y_width1; /**<Lateral width of CVW zone (lat offset of p2 from p3 for right side zone).*/
   float32_T k_cvw_x_length1; /**<Longitudinal length of CVW zone (long offset of p2 from p1 with ref point at center of the ego rear bumper).*/
   float32_T k_cvw_y1; /**<Lateral start point p3 of right side CVW zone (ref point is center of the ego rear bumper).*/
   float32_T k_cvw_x_length0; /**<Longitudinal length of CVW zone (long offset of p1 from p0 with ref point at center of ego rear bumper).*/
   float32_T k_cvw_y_width0; /**<Lateral width of CVW zone (lat offset of p0 from p5 for right side zone).*/
   float32_T k_cvw_y0; /**<Lateral distance of start point p5 of right side CVW Zone (ref point is center of ego rear bumper).*/
   float32_T k_cvw_x0; /**<The longitudinal distance of start point p0 of the CVW zone for right side (ref point is center of ego rear bumper).*/
   float32_T k_bsw_y_width; /**<The lateral width of the BSW zone (offset of p0 from p5 for right side zone).*/
   float32_T k_bsw_y0; /**<Lateral distance of start point p0 of BSW Zone for right side (ref point at center of ego rear bumper).*/
   float32_T k_bsw_x_length; /**<The longitudinal length of the BSW zone (offset of p2 from p0 with ref point at center of the ego rear bumper).*/
   float32_T k_bsw_x0; /**<The longitudinal distance of start point p0 of the BSW zone (ref point is center of the ego rear bumper).*/
   float32_T k_lka_ov_zone_width; /**<LKA zone width.*/
   float32_T k_honda_ego_speed_stop_holding; /**<Defines ego speed below which the alert holding is terminated.*/
   float32_T k_honda_beeper_zone_length; /**<Length of the beeper zone*/
   float32_T k_honda_beeper_zone_width; /**<Width of the beeper zone*/
   float32_T k_honda_alert_level_two_holding_time; /**<Defines the duration of the Honda level 2 alert*/
   float32_T k_bsw_obj_max_rel_vel_thresh; /**<Maximum realitive velocity for which alert should be triggered*/
   float32_T k_bsw_obj_max_rel_vel_hys; /**<Hysteresis applied to maximum realitive velocity check if object triggered alert in previous cycle*/
   float32_T k_bsw_zone_front_ego_side_x; /**<Longintudinal position of the inner(close to ego) front vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_front_ego_side_y; /**<Lateral position of the inner(close to ego) front vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_rear_outer_side_x; /**<Longintudinal position of the inner(close to ego) rear vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_rear_outer_side_y; /**<Lateral position of the inner(close to ego) rear vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_front_ego_side_x_hys; /**<Hysteresis of the longintudinal position of the inner(close to ego) front vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_front_ego_side_y_hys; /**<Hysteresis of the lateral position of the inner(close to ego) front vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_rear_outer_side_x_hys; /**<Hysteresis of the longintudinal position of the inner(close to ego) rear vertex of the BSW zone in VCS coordinates.*/
   float32_T k_bsw_zone_rear_outer_side_y_hys; /**<Hysteresis of the lateral position of the inner(close to ego) rear vertex of the BSW zone in VCS coordinates.*/
   float32_T k_lcda_pedestrian_min_size; /**<Minimum size for an object classified as pedestrian to be selected as a valid candidate for LCDA*/
   float32_T k_lcda_pedestrian_min_speed; /**<Minimum speed for an object classified as pedestrian to be selected as a valid candidate for LCDA*/
   float32_T k_lcda_2wheel_min_size; /**<Minimum size for an object classified as 2wheel to be selected as a valid candidate for LCDA*/
   float32_T k_lcda_2wheel_min_speed; /**<Minimum speed for an object classified as 2wheel to be selected as a valid candidate for LCDA*/
   float32_T k_bsw_object_position_correction_delay_time[LCDA_K_BSW_OBJECT_POSITION_CORRECTION_DELAY_TIME_ARRAY_SIZE_DIM0]; /**<Corrects delay time from tracker, and based on that value we shift longitudinal object position during holding.*/
   float32_T k_bsw_min_speed_for_tos_scenario; /**<Min relative target speed required to trigger the TOS scenario*/
   float32_T k_cvw_min_object_curvi_relative_speed[LCDA_K_CVW_MIN_OBJECT_CURVI_RELATIVE_SPEED_ARRAY_SIZE_DIM0]; /**<Min relative object speed to be candidate*/
   float32_T k_cvw_max_object_curvi_relative_speed; /**<Max relative object speed to be candidate*/
   float32_T k_cvw_object_curvi_relative_speed_hys; /**<Hysteresis value of relative object speed to be candidate*/
   float32_T k_lcda_cvw_ttc_const[LCDA_K_LCDA_CVW_TTC_CONST_ARRAY_SIZE_DIM0]; /**<Constant value used to calculate the dynamic TTC of the CVW module*/
   float32_T k_lcda_cvw_ttc_accel[LCDA_K_LCDA_CVW_TTC_ACCEL_ARRAY_SIZE_DIM0]; /**<Acceleration value used to calculate the dynamic TTC of the CVW module*/
   float32_T k_lcda_honda_narrow_beeper_max_speed_h; /**<Maximum host speed above which narrow beeper zone will not be used*/
   float32_T k_lcda_honda_narrow_beeper_max_speed_l; /**<Maximum host speed below which narrow beeper zone will be used immediately*/
   float32_T k_lcda_dyn_cvw_ttc_speed_parameter; /**<None*/
   float32_T k_bsw_object_position_correction_threshold; /**<Determines relative longitudinal velocity threshold above which higher correction position value will be applied.*/
   float32_T k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh; /**<Relative velocity above which higher compensation value will be added to ttc threshold to compensate system delays*/
   float32_T k_lcda_dyn_cvw_ttc_compens_time[LCDA_K_LCDA_DYN_CVW_TTC_COMPENS_TIME_ARRAY_SIZE_DIM0]; /**<Determines time that will be added to ttc threshold to compensate system delays - first value below thresh, second above thresh */
   float32_T k_bsw_dynzone_object_rel_vel[LCDA_K_BSW_DYNZONE_OBJECT_REL_VEL_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the object relative velocity  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_dynzone_object_range[LCDA_K_BSW_DYNZONE_OBJECT_RANGE_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the object relative velocity  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   boolean_T k_lcda_f_disable_due_to_small_curve_radius; /**<Decides whether entire LCDA functionality should be disabled when the host is moving with a very small curve radius such as a round about.*/
   boolean_T k_lcda_f_enable_obj_in_ego_lane_check; /**<Enable checking if an object is in the ego lane. When this is set to true, alerts from objects that are considered to be in the ego lane will be suppressed.*/
   boolean_T k_lcda_f_enable_alert_obj_in_ego_lane; /**<Enable alert when object is in ego lane and in BSW zone.*/
   boolean_T k_lcda_ego_lane_check_center_point_only; /**<Enables the simple center point check to determine ego lane occupation. If disabled a more sophisticated polygon overlap is performed.*/
   boolean_T k_bsw_uses_cvw_alert_state_enabled; /**<Enables use of CVW alert state info in BSW 1) BSW hys zone is used directly 2) BSW mature count in zone is bypassed if there is a previous CVW alert on the object.*/
   boolean_T k_bsw_overlap_area_check_enable; /**<Enables verification of target FOI area overlaped by BSW zone.*/
   boolean_T k_bsw_use_curvi_coordinates; /**<When set to TRUE the BSW module will always use curvi coordinates for every check. Otherwise VCS coordinates will be used.*/
   boolean_T k_bsw_enable_dynspeed_zone; /**<Enables dynamic BSW zone adaptation based on ego speed.*/
   boolean_T k_bsw_enable_trailer_zone_extension; /**<Enables an additional adjustment of the BSW zone based on trailer information in core input.*/
   boolean_T k_cvw_enable; /**<Enables the CVW feature when the k_cvw_enable_via_cal is also set to TRUE.*/
   boolean_T k_cvw_enable_via_cal; /**<Used to decide whether CVW should be enabled based on the calibration. TRUE=>k_cvw_enable is used to enable CVW, FALSE=>cvwEnabled flag from the core input is used to enable CVW.*/
   boolean_T k_honda_srr6_enable_alert_hold_due_slow_down; /**<Defines if the holding of Honda alert due to ego slowing down is turned on.*/
   boolean_T k_honda_srr6_enable_alert_hold_due_out_of_fov; /**<Defines if the holding of Honda alert due to object lost in radar fov is turned on*/
   boolean_T k_lcda_f_enable_obj_reflection_flag_check; /**<Enables f_reflection flag checking to ignore potential ghost objects, detected by the tracker*/
   boolean_T k_lcda_f_enable_fallback_handler; /**<Enables fallback handler module*/
   boolean_T k_lcda_f_enable_dyn_cvw_ttc_threshold; /**<Enables the CVW dynamic ttc threshold.*/
   boolean_T k_bsw_shrink_zone_method; /**<Determines whether zone will be shrunk using 0 - multiplication or 1 - subtraction ()*/
   boolean_T k_bsw_f_use_front_zone_as_n_line; /**<Determines whether front zone line should be used as n line is SOT scenario*/
   boolean_T k_bsw_f_enable_object_rel_vel_dynzone; /**<Enables dynamic BSW zone adaptation based on object relative velocity.*/
   uint8_t k_bsw_alert_holding_cycles; /**<Number of holding cycles for BSW alert.*/
   uint8_t k_cvw_alert_holding_cycles; /**<Number of holding cycles for CVW alert.*/
   uint8_t k_lcda_zone_check_method; /**<Sets the method for checking the object zone occupation. 0: Checks zone overlap, 1: Only check reference point.*/
   uint8_t k_bsw_min_mature_cycles; /**<Minimum number of mature cycles in zone as a valid bsw target before issuing an alert.*/
   uint8_t k_lcda_min_track_age; /**<Minimum Track age for an object to be selected as a valid candidate for LCDA.*/
   uint8_t k_bsw_alert_track_age; /**<Minimum Track age to trigger a warning immediately even though the object does not pass the mature in zone check.*/
   uint8_t k_cvw_min_mature_cycles; /**<Minimum number of mature cylces in zone as a valid cvw target before issuing an alert.*/
} Lcda_Public_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* LCDA_PUBLIC_CALIBRATION_T_H */
