# ifndef LCDA_CORE_CALIBRATION_T_H
# define LCDA_CORE_CALIBRATION_T_H

/**
* @file lcda_core_calibration_t.h
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
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_SMALL_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_SMALL_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_HYS_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_ZONE_Y_HYS_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_ELC_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_ELC_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_ELC_ZONE_Y_HYS_ARRAY_SIZE_DIM0 (6u)
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
#define LCDA_K_BSW_DYNZONE_SPEED_DROPBACK_ARRAY_SIZE_DIM0 (8u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_RANGE_DROPBACK_ARRAY_SIZE_DIM0 (8u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_Y_HYS_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_LATERAL_TTC_LOOKUP_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_LANE_CHANGE_PROB_LOOKUP_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_LOOKUP_EGO_SPEED_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_LOOKUP_EGO_OVERLAP_OFFSET_ARRAY_SIZE_DIM0 (3u)
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
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_SMALL_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_SMALL_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_HYS_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_ZONE_Y_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_ELC_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_ELC_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_ELC_ZONE_Y_HYS_ARRAY_DIM_SIZE (1u)
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
#define LCDA_K_BSW_DYNZONE_SPEED_DROPBACK_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_BSW_DYNZONE_RANGE_DROPBACK_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_ZONE_Y_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_LATERAL_TTC_LOOKUP_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_LANE_CHANGE_PROB_LOOKUP_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_LOOKUP_EGO_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_K_SLC_LOOKUP_EGO_OVERLAP_OFFSET_ARRAY_DIM_SIZE (1u)
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
#define LCDA_CORE_CALIBRATION_SIZE (1500u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_lcda_f_enable_camera_based_guardrail; /**<Flag to enable or disable the usage of camera based guardrail data for LCDA.*/
   uint8_t k_slc_alert_qualifying_counter; /**<When the qualifying counter is equal to or greater than this threshold the SLC alert won't be suppressed.*/
   uint8_t k_slc_min_mature_cycles; /**<When the count in zone exceeds this threshold the object is allowed to create an SLC alert.*/
   uint8_t k_cvw_zone_calculation_mode; /**<Determines how the zone will be calculated. 0=DEFAULT, 1=Zone based on Vehicle Length and Lane Width, 2=Zone fixed to be taken from LCDA core input.*/
   uint8_t k_cvw_min_mature_cycles; /**<Minimum number of mature cylces in zone as a valid cvw target before issuing an alert.*/
   uint8_t k_bsw_stop_alert_reaching_front_custom_limit_mode; /**<Sets the type of front custom boundary, that will stop alert, when reached by overtaking object. 0: BSW zone longitudinal max, 1: Host front bumper, 2: Custom line - k_bsw_line_to_stop_TOS_alert.*/
   uint8_t k_bsw_alert_track_age; /**<Minimum Track age to trigger a warning immediately even though the object does not pass the mature in zone check.*/
   uint8_t k_lcda_min_track_age; /**<Minimum Track age for an object to be selected as a valid candidate for LCDA.*/
   uint8_t k_bsw_min_mature_cycles; /**<Minimum number of mature cycles in zone as a valid bsw target before issuing an alert.*/
   uint8_t k_bsw_fallback_fast_to_slow_qual_thres; /**<Qualification counter threshold for a fallback transition from fast to slow.*/
   uint8_t k_lcda_zone_check_method; /**<Sets the method for checking the object zone occupation. 0: Checks zone overlap, 1: Only check reference point.*/
   uint8_t k_slc_alert_holding_cycles; /**<Number of holding cycles for SLC alert.*/
   uint8_t k_cvw_alert_holding_cycles; /**<Number of holding cycles for CVW alert.*/
   uint8_t k_bsw_alert_holding_cycles; /**<Number of holding cycles for BSW alert.*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   uint8_t k_lm_min_output_hold_cycles; /**<The minimum number of cycles the previous values of the lane model output should be held.*/
   uint8_t k_lm_min_qualification_cycle_count; /**<The minimum number of cycles the lane model data should hold its value before it is considered as valid.*/
   uint8_t k_lcda_default_warntrigger_hmi; /**<Default hmi warntrigger.*/
   uint8_t k_elc_min_mature_cycles; /**< When the mature count in zone exceeds this threshold the object is allowed to create an ELC alert.*/
   uint8_t k_cvw_min_mature_cycles_lc_intention; /**<Minimum number of mature cylces in lane change intention zone as a valid cvw target before issuing an alert*/
   uint8_t k_lcda_lc_intention_cycles_for_zone_change_threshold; /**<Number of cycles before the lane change intention zone is changed depending on changed guardrail information.*/
   uint8_t k_lcda_turn_signal_coast_cycles; /**<Number of scans to hold turn signal after it is cancelled by the driver.*/
   uint8_t k_elc_alert_holding_cycles; /**<Number of holding cycles for ELC alert.*/
   boolean_T k_bsw_f_use_zone_without_hysteresis_lane_change_intention; /**<Enables using of the initial (no hysteresesis) for the lateral overlapping check in lachne change intention logic.*/
   boolean_T k_bsw_f_enable_object_rel_vel_dynzone; /**<Enables dynamic BSW zone adaptation based on object relative velocity.*/
   boolean_T k_bsw_f_use_front_zone_as_n_line; /**<Determines whether front zone line should be used as n line is SOT scenario*/
   boolean_T k_bsw_shrink_zone_method; /**<Determines whether zone will be shrunk using 0 - multiplication or 1 - subtraction ()*/
   boolean_T k_lcda_f_enable_dyn_cvw_ttc_threshold; /**<Enables the CVW dynamic ttc threshold.*/
   boolean_T k_lcda_f_enable_fallback_handler; /**<Enables fallback handler module*/
   boolean_T k_lcda_f_enable_obj_reflection_flag_check; /**<Enables f_reflection flag checking to ignore potential ghost objects, detected by the tracker*/
   boolean_T k_bsw_f_fallback_default_status_slow; /**<Changes default fallback status of the object to the slow*/
   boolean_T k_lcda_f_enable_rel_vel_logic_in_ego_lane_check; /**<Ego lane occupancy will be determined on position as well as relative velocity of the object*/
   boolean_T k_slc_enable_via_cal; /**<Used to decide whether Sim Lane Change should be enabled based on the calibration. TRUE=>k_slc_enable is used to enable SLC, FALSE=>slcEnabled flag from the core input is used to enable SLC.*/
   boolean_T k_slc_enable; /**<Enables the Sim Lane Change feature when the k_slc_enable_via_cal is also set to TRUE.*/
   boolean_T k_cvw_f_most_crit_obj_must_be_closest_relevant_obj; /**<Limits the CVW alert to the closest relevant CVW object. A critical object behind a relevant object will be suppressed.*/
   boolean_T k_enable_cvw_curve_zone_adaptation; /**<allows customers to enable and disable the curveZoneAdaptation function to cut the cvw zone in curve.*/
   boolean_T k_cvw_enable_via_cal; /**<Used to decide whether CVW should be enabled based on the calibration. TRUE=>k_cvw_enable is used to enable CVW, FALSE=>cvwEnabled flag from the core input is used to enable CVW.*/
   boolean_T k_cvw_enable; /**<Enables the CVW feature when the k_cvw_enable_via_cal is also set to TRUE.*/
   boolean_T k_bsw_enable_trailer_zone_extension; /**<Enables an additional adjustment of the BSW zone based on trailer information in core input.*/
   boolean_T k_bsw_enable_dynspeed_zone; /**<Enables dynamic BSW zone adaptation based on ego speed.*/
   boolean_T k_bsw_enable_factor_based_host_speed_adjustment; /**<Enables a host speed dependend bsw zone adjustment with the same factor for normal and hysteresis zone related to host rear. NOTE: Zone definitions must be handled very carefully when enabled!.*/
   boolean_T k_bsw_hold_alert_long_object; /**<Objects that are overtaking and are classified as long trucks would hold alert until whole object leave the zone.*/
   boolean_T k_bsw_enable_specific_front_sot_conditions; /**<Enables alert behaviours different than typical for entering front zone boundary.*/
   boolean_T k_bsw_enable_zone_front_boundary_specific_conditions; /**<Enables alert behaviours different than typical for entering/leaving front zone boundary.*/
   boolean_T k_bsw_enable_via_cal; /**<Used to decide whether BSW should be enabled based on the calibration. TRUE=>k_bsw_enable is used to enable BSW, FALSE=>bswEnabled flag from the core input is used to enable BSW.*/
   boolean_T k_bsw_enable; /**<Enables the BSW feature when the k_bsw_enable_via_cal is also set to TRUE.*/
   boolean_T k_bsw_use_curvi_coordinates; /**<When set to TRUE the BSW module will always use curvi coordinates for every check. Otherwise VCS coordinates will be used.*/
   boolean_T k_bsw_overlap_area_check_enable; /**<Enables verification of target FOI area overlaped by BSW zone.*/
   boolean_T k_bsw_uses_cvw_alert_state_enabled; /**<Enables use of CVW alert state info in BSW 1) BSW hys zone is used directly 2) BSW mature count in zone is bypassed if there is a previous CVW alert on the object.*/
   boolean_T k_lcda_ego_lane_check_center_point_only; /**<Enables the simple center point check to determine ego lane occupation. If disabled a more sophisticated polygon overlap is performed.*/
   boolean_T k_lcda_f_enable_alert_obj_in_ego_lane; /**<Enable alert when object is in ego lane and in BSW zone.*/
   boolean_T k_bsw_f_enable_adv_pos_data_lane_change_intention; /**<Enables additional position verification for alert suppression in lane change intention algorithm.*/
   boolean_T k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge; /**<Enable checking if at least part of the object is outside the zone. When this is set to true, alerts from objects that protrudes beyond the boundaries of the zone will be suppressed.*/
   boolean_T k_lcda_f_enable_suppress_alert_object_no_lane_change_intention; /**<Enable checking if an object will change lane based on relative lateral velocity. When this is set to true, alerts from objects that are in the ego lane and move towards ego center will be suppressed.*/
   boolean_T k_lcda_f_enable_obj_in_ego_lane_check; /**<Enable checking if an object is in the ego lane. When this is set to true, alerts from objects that are considered to be in the ego lane will be suppressed.*/
   boolean_T k_lcda_allow_track_status_new; /**<Allow using the tracks with TRACK_STATUS_NEW in the valid BSW/CVW candidate selection. When it is FALSE, only the mature and coasted tracks are considered for candidate selection.*/
   boolean_T k_lcda_f_disable_due_to_small_curve_radius; /**<Decides whether entire LCDA functionality should be disabled when the host is moving with a very small curve radius such as a round about.*/
   boolean_T k_lcda_enable_via_cal; /**<Used to decide whether LCDA should be enabled based on the calibration. TRUE=>k_lcda_enable is used to enable LCDA, FALSE=>lcdaEnabled flag from the core input is used to enable LCDA.*/
   boolean_T k_lcda_enable; /**<Enables the LCDA feature when the k_lcda_enable_via_cal is also set to TRUE.*/
   boolean_T k_lm_enable_use_vehicle_dyn; /**<Enable lane model estimation based on vehicle dynamics.*/
   boolean_T k_lm_enable_use_camera_data; /**<Enable lane model estimation based on camera data.*/
   boolean_T k_lm_enable_use_navigation_data; /**<Enable lane model estimation based on navigation data.*/
   boolean_T k_lm_use_default_lane_information; /**<Master override flag to use the default lane information.*/
   boolean_T k_lcda_use_default_warntrigger_hmi; /**<Decides whether default hmi warntrigger should be used or not.*/
   boolean_T k_elc_enable_via_cal; /**<Used to decide whether Evasive Lane Change should be enabled based on the calibration. TRUE=>k_elc_enable is used to enable ELC, FALSE=>elcEnabled flag from the core input is used to enable ELC.*/
   boolean_T k_elc_enable; /**<Enables the Evasive Lane Change feature when the k_elc_enable_via_cal is also set to TRUE.*/
   boolean_T k_bsw_f_enable_trailer_zone_adjustment_on_outer_side; /**<Enables modification of the BSW zone by shifting the boundary on the ego side based on trailer information in core input data.*/
   boolean_T k_bsw_f_enable_trailer_zone_adjustment_on_ego_side; /**<Enables modification of the BSW zone by shifting the boundary on the ego side based on trailer information in core input data.*/
   boolean_T k_bsw_f_zone_extension_by_diff_width_host_vs_trailer; /**<Enables adjustment lateral dimension of the BSW zone if the trailer is wider than the host, modify the size with this value.*/
   boolean_T k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present; /**<Flag indicating if the smaller lane change intention cvw zone should be used if no radar guardrail is present.*/
   uint16_t k_lm_min_count_in_state; /**<Minimum number of cycles the criteria for a given state should be satisfied before the output lane model switches to the given state.*/
   float32_T k_bsw_dynzone_object_range[LCDA_K_BSW_DYNZONE_OBJECT_RANGE_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the object relative velocity  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_dynzone_object_rel_vel[LCDA_K_BSW_DYNZONE_OBJECT_REL_VEL_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the object relative velocity  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_lcda_dyn_cvw_ttc_compens_time[LCDA_K_LCDA_DYN_CVW_TTC_COMPENS_TIME_ARRAY_SIZE_DIM0]; /**<Determines time that will be added to ttc threshold to compensate system delays - first value below thresh, second above thresh */
   float32_T k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh; /**<Relative velocity above which higher compensation value will be added to ttc threshold to compensate system delays*/
   float32_T k_bsw_object_position_correction_threshold; /**<Determines relative longitudinal velocity threshold above which higher correction position value will be applied.*/
   float32_T k_lcda_dyn_cvw_ttc_speed_parameter; /**<None*/
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
   float32_T k_slc_lookup_ego_overlap_offset[LCDA_K_SLC_LOOKUP_EGO_OVERLAP_OFFSET_ARRAY_SIZE_DIM0]; /**<Look-up table for the ego speed dependent (see k_slc_lookup_ego_speed) ego overlap offset used to determine if an object is classified as besides the ego.*/
   float32_T k_slc_lookup_ego_speed[LCDA_K_SLC_LOOKUP_EGO_SPEED_ARRAY_SIZE_DIM0]; /**<Look-up table for the ego speed used to determine the ego overlap offset (see k_slc_lookup_ego_overlap_offset).*/
   float32_T k_slc_lane_change_prob_lookup[LCDA_K_SLC_LANE_CHANGE_PROB_LOOKUP_ARRAY_SIZE_DIM0]; /**<Look up table with the lane change probability (to be used along with the lateral ttc lookup).*/
   float32_T k_slc_lateral_ttc_lookup[LCDA_K_SLC_LATERAL_TTC_LOOKUP_ARRAY_SIZE_DIM0]; /**<Look up table with the lateral TTC values to be used to find the corresponding lane change probability> Note that the values should be in strictly monotonic order.*/
   float32_T k_slc_critical_lon_ttc; /**<Longitudinal TTC threshold below which a target is considered as critical for SLC.*/
   float32_T k_slc_critical_lat_ttc; /**<Lateral TTC threshold below which a target is considered as critical for SLC.*/
   float32_T k_slc_min_obj_curvi_long_vel_abs; /**<The minimum longitudinal absolute curvi velocity of the object to be considered as a valid candidate for Sim Lane Change.*/
   float32_T k_slc_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects to be considered as valid Sim Lane Change candidates.*/
   float32_T k_slc_zone_y[LCDA_K_SLC_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining the SLC zone in curvi coordinates. It is specified as a factor of the lane width.*/
   float32_T k_slc_zone_x[LCDA_K_SLC_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining Sim Lane Change zone in curvi coordinates.*/
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
   float32_T k_cvw_lane_change_intention_zone_y[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Lateral coordinates of the corners of a polygon defining cvw zone in curvi coordinates in case of triggering a lane change intention. Depending on k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone and k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present different coordinates defined in k_cvw_lane_change_intention_zone_small_y might be used instead.*/
   float32_T k_cvw_lane_change_intention_zone_x[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_X_ARRAY_SIZE_DIM0]; /**<Longitudinal coordinates of the corners of a polygon defining cvw zone in curvi coordinates in case of triggering a lane change intention. Depending on k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone and k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present different coordinates defined in k_cvw_lane_change_intention_zone_small_x might be used instead.*/
   float32_T k_cvw_min_obj_curvi_long_vel; /**<The minimum longitudinal curvi velocity of an object for the cvw zone.*/
   float32_T k_cvw_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects in the cvw zone.*/
   float32_T k_cvw_ttc_hys; /**<Added to TTC threshold when an alert is active.*/
   float32_T k_cvw_ttc; /**<Maximum TTC to issue an alert.*/
   float32_T k_cvw_candidate_ttc; /**<If TTC of an object is lower than this threshold it is considered a candidate for a CVW alert.*/
   float32_T k_cvw_gap_bridge; /**<Parameter describing the time between two alerts, when the Alert should not go off.*/
   float32_T k_lcda_max_range; /**<Maximum Range of a track that can trigger a warning.*/
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
   float32_T k_bsw_suppress_late_warning_max_rel_vel; /**<Maximum relative velocity at which BSD warning can be suppressed.*/
   float32_T k_bsw_suppress_late_warning_min_pos_behind_host; /**<Minimum object distance behind ego rear at which a BSD warning can be suppressed.*/
   float32_T k_bsw_suppress_late_warning_max_time_till_leave; /**<Maximum time until the object leaves the rear bsd zone border in which a BSD warning can be suppressed.*/
   float32_T k_bsw_min_length_long_object_hys; /**<Minimum length of the object that will be classified as a long object.*/
   float32_T k_bsw_min_length_long_object; /**<Minimum length of the object that will be classified as a long object.*/
   float32_T k_bsw_line_to_stop_TOS_alert; /**<Sets the longitudinal line for front bsw zone boundary to disable alert for TOS case. Requires k_bsw_stop_alert_reaching_front_custom_limit_mode = 2*/
   float32_T k_bsw_n_line_position_for_long_object_sot_scenario; /**<In SOT scenario alert is active when the rear of long object reaches this line. Close to the position of rear host bumper (N line) in VCS coordinates (shall be negative) */
   float32_T k_bsw_dynzone_range_dropback[LCDA_K_BSW_DYNZONE_RANGE_DROPBACK_ARRAY_SIZE_DIM0]; /**<BSW look up table to shrink the dynamic warn zones during drop back = slow overtaking maneuvers based on relative speeds.*/
   float32_T k_bsw_dynzone_speed_dropback_max; /**<Maximum speed until the BSW look up table to shrink the dynamic warn zones is active.*/
   float32_T k_bsw_dynzone_speed_dropback[LCDA_K_BSW_DYNZONE_SPEED_DROPBACK_ARRAY_SIZE_DIM0]; /**<BSW look up table to shrink the dynamic warn zones during drop back = slow overtaking maneuvers based on relative speeds.*/
   float32_T k_min_exist_prob_camera_guardrail; /**<The minimum existence probability of a camera guardrail to determine, if an object is in conflict with its environment.*/
   float32_T k_min_exist_prob_radar_guardrail; /**<The minimum existence probability of a radar guardrail to determine, if an object is in conflict with its environment.*/
   float32_T k_bsw_dynzone_range[LCDA_K_BSW_DYNZONE_RANGE_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the host vehicle speed  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_dynzone_speed[LCDA_K_BSW_DYNZONE_SPEED_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the host vehicle speed  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_fallback_rel_vel_thres_hys; /**<A target can be reclassified as fast back falling object if the relative velocity is below k_bsw_fallback_rel_vel_thres - k_bsw_fallback_rel_vel_thres_hys.*/
   float32_T k_bsw_fallback_rel_vel_thres; /**<A target will be considered as slow back falling object if the relative velocity is above threshold.*/
   float32_T k_bsw_overlap_area_threshold; /**<Mimium area of target FOI, overlapped by BSW zone to raise BSW alert, expressed in percentage of FOI.*/
   float32_T k_bsw_lateral_distance_zone; /**<Distance between car and beginning of the BDS-Zone.*/
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
   float32_T k_lcda_min_exist_prob_lc_intention; /**<Minimum existence probability threshold in case of lane change intention.*/
   float32_T k_lcda_ego_lane_effective_lane_width_factor; /**<Lane width factor to be used when determining if a LCDA object is within the ego lane. Value should be between 0 and 1.*/
   float32_T k_bsw_lane_change_intention_pos_long_thres; /**<Threshold for longitudinal position below which alert won't be suppressed.*/
   float32_T k_bsw_lane_change_intention_pos_lat_thres; /**<Threshold for lateral zone occupation above which alert won't be suppressed.*/
   float32_T k_lcda_lane_change_intention_vel_lat_thresh; /**<Threshold for relative velocity above which alert suppression functionality would be enabled.*/
   float32_T k_lcda_exist_prob_hys_offset; /**<Offset for the existence probability for an object which gave an alert for an arbitrary submodule. This offset will be subtracted from the minimum needed existence probability k_lcda_min_exist_prop.*/
   float32_T k_lcda_exist_prob_lc_intention_hys_offset; /**<Offset for the existence probability of an alerting object when a lane change intention is given. This offset will be subtracted from the minimum needed existence probability k_lcda_min_exist_prop.*/
   float32_T k_lcda_zone_intersect_critical_point_lateral_ratio; /**<Determines which point is checked for objects heading towards the zone if CRITICAL_POINT_IN_ZONE is used as intersect detection. 0.0=lateral center of object, 1.0=lateral inner edge.*/
   float32_T k_lcda_max_ego_vehicle_length; /**<>Maximum ego length - used for plausibility checking during zone creation. If input ego length is larger than this, this max value is used.*/
   float32_T k_lcda_min_ego_vehicle_length; /**<Minimum ego length - used for plausibility checking during zone creation. If input ego length is lower than this, this min value is used.*/
   float32_T k_lcda_max_ego_vehicle_width; /**<Maximum ego width - used for plausibility checking during zone creation. If input ego width is larger than this, this max value is used.*/
   float32_T k_lcda_min_ego_vehicle_width; /**<Minimum ego width - used for plausibility checking during zone creation. If input ego width is lower than this, this min value is used.*/
   float32_T k_lcda_max_lane_width; /**<Maximum lane width - used for plausibility checking during zone creation. If input lane width is larger than this, this max lane width is used.*/
   float32_T k_lcda_min_lane_width; /**<Minimum lane width - used for plausibility checking during zone creation. If input lane width is lower than this, this value width is used.*/
   float32_T k_lcda_curve_radius_threshold_for_zone_adaptation; /**<If the curve radius is below this value, the cvw zone shortening is applied.*/
   float32_T k_lcda_min_curve_radius_hys; /**<Hysteresis value to be applied before LCDA is disabled for small curve radius.*/
   float32_T k_lcda_min_curve_radius; /**<LCDA is disabled when the host is navigating a curve with a radius smaller than this value.*/
   float32_T k_lcda_distance_traveled_scale_factor; /**<Inhibits the amount by which the CVW zone length restores after it was shortened. The restoration (travelled distance of ego) is multiplied by this value.*/
   float32_T k_lcda_host_activation_speed_max_hys; /**<Hysteresis for k_lcda_host_activation_speed_min_max. This value will be added to maximal activation speed threshold if LCDA is currently active.*/
   float32_T k_lcda_host_activation_speed_max; /**<Maximal host speed for which LCDA shall be active.*/
   float32_T k_lcda_host_activation_speed_min_hys; /**<Hysteresis for k_lcda_host_activation_speed_min_min. This value will be subtracted from minimal activation speed threshold if LCDA is currently active.*/
   float32_T k_lcda_host_activation_speed_min; /**<Minimal host speed for which LCDA shall be active.*/
   float32_T k_lcda_min_exist_prop; /**<Min existence probability for tracks*/
   float32_T k_lm_max_plausible_lc_offset_factor; /**<The maximum plausible value for the lane center offset expressed as a factor of the lane width.*/
   float32_T k_lm_max_plausible_lane_width; /**<The maximum value of the lane width for it to be considered plausible.*/
   float32_T k_lm_min_plausible_lane_width; /**<The minimum value of the lane width for it to be considered plausible.*/
   float32_T k_lm_min_lane_exist_prob_percent; /**<Minimum existence probability of the camera lane data for the data to be considered as valid.*/
   float32_T k_lm_hys_delta_yawrate_city_abs; /**<Hysteresis delta yaw rate to calculate the yawrate threshold for swithching to city lane model.*/
   float32_T k_lm_min_yawrate_city_abs; /**<Minimum yaw rate for city lane model.*/
   float32_T k_lm_hys_delta_speed_hway; /**<Hysteresis delta speed used to calculate speed threshold for switching from HIGHWAY to CITY.*/
   float32_T k_lm_min_speed_hway; /**<Minimum ego speed to switch to highway lane model.*/
   float32_T k_lm_lane_center_offset_default; /**<Default lane center offset to be used when the lane center offset cannot be determined.*/
   float32_T k_lcda_lm_lane_width_defaultcountry_default; /**<Default lane width for any nonspecific country to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_germany_default; /**<Default lane width inside Germany to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_korea_default; /**<Default lane width inside Korea to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_china_default; /**<Default lane width inside China to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_japan_default; /**<Default lane width inside Japan to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_us_default; /**<Default lane width inside the United States of America to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lm_lane_width_highway; /**<Lane width to be used for highway driving.*/
   float32_T k_lm_lane_width_city; /**<Lane width to be used for city driving.*/
   float32_T k_elc_obj_safe_deceleration_threshold_hys; /**<Deceleration threshold above which a target needs to stay to remain critical for ELC.*/
   float32_T k_elc_obj_safe_deceleration_threshold; /**<Deceleration threshold above which a target is considered as critical for ELC.*/
   float32_T k_elc_critical_longitudinal_ttc_hys; /**<Longitudinal TTC threshold below which a target needs to stay to remain critical for ELC.*/
   float32_T k_elc_critical_longitudinal_ttc; /**<Longitudinal TTC threshold below which a target is considered as critical for ELC.*/
   float32_T k_elc_min_obj_curvi_long_vel_abs; /**<The minimum longitudinal absolute curvi velocity of the object to be considered as a valid candidate for Evasive Lane Change.*/
   float32_T k_elc_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects to be considered as valid Evasive Lane Change candidates.*/
   float32_T k_elc_zone_y_hys[LCDA_K_ELC_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset as a factor of the lane width that is to be added/subtracted from the initial ELC zone y-coord to get the hysteresis zone y-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_elc_zone_y[LCDA_K_ELC_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining the ELC zone in curvi coordinates. It is specified as a factor of the lane width.*/
   float32_T k_elc_zone_x[LCDA_K_ELC_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining Evasive Lane Change zone in curvi coordinates.*/
   float32_T k_slc_obj_lc_effective_speed_min; /**<Minimum effective lateral speed threshold for flagging object as changing lanes.*/
   float32_T k_slc_warntrigger_TTC_lon_early; /**<TTC threshold adaption for longitudinal TTC warning in case the of HMI setting early warning.*/
   float32_T k_slc_warntrigger_TTC_lon_late; /**<TTC threshold adaption for longitudinal TTC warning in case the of HMI setting late warning.*/
   float32_T k_slc_warntrigger_TTC_lat_early; /**<TTC threshold adaption for lateral TTC warning in case the of HMI setting early warning.*/
   float32_T k_slc_warntrigger_TTC_lat_late; /**<TTC threshold adaption for lateral TTC warning in case the of HMI setting late warning.*/
   float32_T k_slc_critical_lon_ttc_hys; /**<Added to longitudinal TTC threshold when object is considered as critical for SLC.*/
   float32_T k_slc_critical_lat_ttc_hys; /**<Added to lateral TTC threshold when object is considered as critical for SLC.*/
   float32_T k_slc_max_obj_eclipse; /**<The maximum eclipse value of the object to be considered as a valid candidate for Sim Lane Change.*/
   float32_T k_slc_zone_y_hys[LCDA_K_SLC_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset as a factor of the lane width that is to be added/subtracted from the initial SLC zone y-coord to get the hysteresis zone y-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_bsw_x0_hys; /**<Absolute length of upper longitudinal hysteresis of BSW zone from initial zone (offset to p0 of initial zone).*/
   float32_T k_bsw_x1_hys; /**<Absolute length of the bottom longitudinal hysteresis of BSW zone from initial zone (offset to p2 of initial zone).*/
   float32_T k_bsw_y0_hys; /**<The absolute width of the bottom lateral hysteresis of BSW zone on right side (offset to p3 of initial zone).*/
   float32_T k_bsw_y1_hys; /**<The absolute width of the upper lateral hysteresis of BSW zone on right side (offset to p0 of initial zone).*/
   float32_T k_cvw_lane_change_intention_zone_hys_y[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_HYS_Y_ARRAY_SIZE_DIM0]; /**<Offset as a factor of the lane width that is to be added from the initial cvw lane change intention zone.*/
   float32_T k_cvw_lane_change_intention_zone_small_y[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_SMALL_Y_ARRAY_SIZE_DIM0]; /**<Lateral coordinates of the corners of a polygon defining cvw zone in curvi coordinates in case of triggering a lane change intention. Depending on k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone and k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present different coordinates defined in k_cvw_lane_change_intention_zone_small_y might be used instead.*/
   float32_T k_cvw_lane_change_intention_zone_small_x[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_SMALL_X_ARRAY_SIZE_DIM0]; /**<Longitudinal coordinates of the corners of a polygon defining cvw zone in curvi coordinates in case of triggering a lane change intention. Depending on k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone and k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present different coordinates defined in k_cvw_lane_change_intention_zone_x might be used instead.*/
   float32_T k_cvw_critical_obj_decel_after_lane_change; /**<None*/
   float32_T k_cvw_crit_dist_additive_hys; /**<In case that a previous cvw alert has been given, an additive hysteresis can be applied to the critical distance.*/
   float32_T k_cvw_crit_dist_hys_factor; /**<In case that a previous cvw alert has been given, a multiplicative hysteresis factor is applied to the critical distance.*/
   float32_T k_cvw_time_diff_after_obj_decel; /**<Time difference between host and object after deceleration of the object.*/
   float32_T k_cvw_time_obj_start_decel_after_lane_change; /**<Time after lane change where tracked object starts to decelerate.*/
   float32_T k_cvw_curve_zone_factor_inner; /**<While driving, the length of the CVW zone is set to the circumference of the curvature of the ego multiplied by this factor. This leads to a shortening of the CVW zone on the inner side of the curve.*/
   float32_T k_cvw_curve_zone_factor_outer; /**<While driving, the length of the CVW zone is set to the circumference of the curvature of the ego multiplied by this factor. This leads to a shortening of the CVW zone on the outer side of the curve.*/
   float32_T k_zone_hys_obj_width_correction; /**<Object with this width receive a zone hysteresis with factor 1, for smaller objects it is computed as object_width divided by k_zone_hys_obj_width_correction between 0 and 1.*/
   float32_T k_cvw_warntrigger_late; /**<TTC offset to k_cvw_ttc if k_lcda_warntrigger_hmi=0.*/
   float32_T k_cvw_warntrigger_early; /**<TTC offset to k_cvw_ttc if k_lcda_warntrigger_hmi=4.*/
   float32_T k_bsw_guardrail_distance_safety_margin; /**<This value will be added to the guardrail distance for calculations in BSW to prevent falsely filtered due to minor tracker inaccuracies.*/
   float32_T k_bsw_trailer_zone_min_width; /**<Sets minimal width of BSW zone if k_bsw_enable_trailer_zone_extension is enabled.*/
   float32_T k_bsw_warntrigger_late; /**<Length factor of BSW zone in case the of HMI setting late, applies to points 3 and 4, Caution: CVW zone must overlap so that no gap occurs between CVW and BSW zone.*/
   float32_T k_bsw_warntrigger_early; /**<Length factor of BSW zone in case the of HMI setting early warning, applies to points 3 and 4,Caution: CVW zone must overlap so that no gap occurs between CVW and BSW zone.*/
   float32_T k_bsw_lane_change_intention_vel_lat_hys; /**<Hysteresis for relative velocity above which alert suppression functionality would be enabled.*/
   float32_T k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone; /**<Minimal lateral position of a radar guardrail to use the smaller lane change intention cvw zone.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Lcda_Core_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone; /**<Minimal lateral position of a radar guardrail to use the smaller lane change intention cvw zone.*/
   float32_T k_bsw_lane_change_intention_vel_lat_hys; /**<Hysteresis for relative velocity above which alert suppression functionality would be enabled.*/
   float32_T k_bsw_warntrigger_early; /**<Length factor of BSW zone in case the of HMI setting early warning, applies to points 3 and 4,Caution: CVW zone must overlap so that no gap occurs between CVW and BSW zone.*/
   float32_T k_bsw_warntrigger_late; /**<Length factor of BSW zone in case the of HMI setting late, applies to points 3 and 4, Caution: CVW zone must overlap so that no gap occurs between CVW and BSW zone.*/
   float32_T k_bsw_trailer_zone_min_width; /**<Sets minimal width of BSW zone if k_bsw_enable_trailer_zone_extension is enabled.*/
   float32_T k_bsw_guardrail_distance_safety_margin; /**<This value will be added to the guardrail distance for calculations in BSW to prevent falsely filtered due to minor tracker inaccuracies.*/
   float32_T k_cvw_warntrigger_early; /**<TTC offset to k_cvw_ttc if k_lcda_warntrigger_hmi=4.*/
   float32_T k_cvw_warntrigger_late; /**<TTC offset to k_cvw_ttc if k_lcda_warntrigger_hmi=0.*/
   float32_T k_zone_hys_obj_width_correction; /**<Object with this width receive a zone hysteresis with factor 1, for smaller objects it is computed as object_width divided by k_zone_hys_obj_width_correction between 0 and 1.*/
   float32_T k_cvw_curve_zone_factor_outer; /**<While driving, the length of the CVW zone is set to the circumference of the curvature of the ego multiplied by this factor. This leads to a shortening of the CVW zone on the outer side of the curve.*/
   float32_T k_cvw_curve_zone_factor_inner; /**<While driving, the length of the CVW zone is set to the circumference of the curvature of the ego multiplied by this factor. This leads to a shortening of the CVW zone on the inner side of the curve.*/
   float32_T k_cvw_time_obj_start_decel_after_lane_change; /**<Time after lane change where tracked object starts to decelerate.*/
   float32_T k_cvw_time_diff_after_obj_decel; /**<Time difference between host and object after deceleration of the object.*/
   float32_T k_cvw_crit_dist_hys_factor; /**<In case that a previous cvw alert has been given, a multiplicative hysteresis factor is applied to the critical distance.*/
   float32_T k_cvw_crit_dist_additive_hys; /**<In case that a previous cvw alert has been given, an additive hysteresis can be applied to the critical distance.*/
   float32_T k_cvw_critical_obj_decel_after_lane_change; /**<None*/
   float32_T k_cvw_lane_change_intention_zone_small_x[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_SMALL_X_ARRAY_SIZE_DIM0]; /**<Longitudinal coordinates of the corners of a polygon defining cvw zone in curvi coordinates in case of triggering a lane change intention. Depending on k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone and k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present different coordinates defined in k_cvw_lane_change_intention_zone_x might be used instead.*/
   float32_T k_cvw_lane_change_intention_zone_small_y[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_SMALL_Y_ARRAY_SIZE_DIM0]; /**<Lateral coordinates of the corners of a polygon defining cvw zone in curvi coordinates in case of triggering a lane change intention. Depending on k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone and k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present different coordinates defined in k_cvw_lane_change_intention_zone_small_y might be used instead.*/
   float32_T k_cvw_lane_change_intention_zone_hys_y[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_HYS_Y_ARRAY_SIZE_DIM0]; /**<Offset as a factor of the lane width that is to be added from the initial cvw lane change intention zone.*/
   float32_T k_bsw_y1_hys; /**<The absolute width of the upper lateral hysteresis of BSW zone on right side (offset to p0 of initial zone).*/
   float32_T k_bsw_y0_hys; /**<The absolute width of the bottom lateral hysteresis of BSW zone on right side (offset to p3 of initial zone).*/
   float32_T k_bsw_x1_hys; /**<Absolute length of the bottom longitudinal hysteresis of BSW zone from initial zone (offset to p2 of initial zone).*/
   float32_T k_bsw_x0_hys; /**<Absolute length of upper longitudinal hysteresis of BSW zone from initial zone (offset to p0 of initial zone).*/
   float32_T k_slc_zone_y_hys[LCDA_K_SLC_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset as a factor of the lane width that is to be added/subtracted from the initial SLC zone y-coord to get the hysteresis zone y-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_slc_max_obj_eclipse; /**<The maximum eclipse value of the object to be considered as a valid candidate for Sim Lane Change.*/
   float32_T k_slc_critical_lat_ttc_hys; /**<Added to lateral TTC threshold when object is considered as critical for SLC.*/
   float32_T k_slc_critical_lon_ttc_hys; /**<Added to longitudinal TTC threshold when object is considered as critical for SLC.*/
   float32_T k_slc_warntrigger_TTC_lat_late; /**<TTC threshold adaption for lateral TTC warning in case the of HMI setting late warning.*/
   float32_T k_slc_warntrigger_TTC_lat_early; /**<TTC threshold adaption for lateral TTC warning in case the of HMI setting early warning.*/
   float32_T k_slc_warntrigger_TTC_lon_late; /**<TTC threshold adaption for longitudinal TTC warning in case the of HMI setting late warning.*/
   float32_T k_slc_warntrigger_TTC_lon_early; /**<TTC threshold adaption for longitudinal TTC warning in case the of HMI setting early warning.*/
   float32_T k_slc_obj_lc_effective_speed_min; /**<Minimum effective lateral speed threshold for flagging object as changing lanes.*/
   float32_T k_elc_zone_x[LCDA_K_ELC_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining Evasive Lane Change zone in curvi coordinates.*/
   float32_T k_elc_zone_y[LCDA_K_ELC_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining the ELC zone in curvi coordinates. It is specified as a factor of the lane width.*/
   float32_T k_elc_zone_y_hys[LCDA_K_ELC_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset as a factor of the lane width that is to be added/subtracted from the initial ELC zone y-coord to get the hysteresis zone y-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_elc_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects to be considered as valid Evasive Lane Change candidates.*/
   float32_T k_elc_min_obj_curvi_long_vel_abs; /**<The minimum longitudinal absolute curvi velocity of the object to be considered as a valid candidate for Evasive Lane Change.*/
   float32_T k_elc_critical_longitudinal_ttc; /**<Longitudinal TTC threshold below which a target is considered as critical for ELC.*/
   float32_T k_elc_critical_longitudinal_ttc_hys; /**<Longitudinal TTC threshold below which a target needs to stay to remain critical for ELC.*/
   float32_T k_elc_obj_safe_deceleration_threshold; /**<Deceleration threshold above which a target is considered as critical for ELC.*/
   float32_T k_elc_obj_safe_deceleration_threshold_hys; /**<Deceleration threshold above which a target needs to stay to remain critical for ELC.*/
   float32_T k_lm_lane_width_city; /**<Lane width to be used for city driving.*/
   float32_T k_lm_lane_width_highway; /**<Lane width to be used for highway driving.*/
   float32_T k_lcda_lm_lane_width_us_default; /**<Default lane width inside the United States of America to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_japan_default; /**<Default lane width inside Japan to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_china_default; /**<Default lane width inside China to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_korea_default; /**<Default lane width inside Korea to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_germany_default; /**<Default lane width inside Germany to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lcda_lm_lane_width_defaultcountry_default; /**<Default lane width for any nonspecific country to be used when it cannot be determined if the road type is city or highway.*/
   float32_T k_lm_lane_center_offset_default; /**<Default lane center offset to be used when the lane center offset cannot be determined.*/
   float32_T k_lm_min_speed_hway; /**<Minimum ego speed to switch to highway lane model.*/
   float32_T k_lm_hys_delta_speed_hway; /**<Hysteresis delta speed used to calculate speed threshold for switching from HIGHWAY to CITY.*/
   float32_T k_lm_min_yawrate_city_abs; /**<Minimum yaw rate for city lane model.*/
   float32_T k_lm_hys_delta_yawrate_city_abs; /**<Hysteresis delta yaw rate to calculate the yawrate threshold for swithching to city lane model.*/
   float32_T k_lm_min_lane_exist_prob_percent; /**<Minimum existence probability of the camera lane data for the data to be considered as valid.*/
   float32_T k_lm_min_plausible_lane_width; /**<The minimum value of the lane width for it to be considered plausible.*/
   float32_T k_lm_max_plausible_lane_width; /**<The maximum value of the lane width for it to be considered plausible.*/
   float32_T k_lm_max_plausible_lc_offset_factor; /**<The maximum plausible value for the lane center offset expressed as a factor of the lane width.*/
   float32_T k_lcda_min_exist_prop; /**<Min existence probability for tracks*/
   float32_T k_lcda_host_activation_speed_min; /**<Minimal host speed for which LCDA shall be active.*/
   float32_T k_lcda_host_activation_speed_min_hys; /**<Hysteresis for k_lcda_host_activation_speed_min_min. This value will be subtracted from minimal activation speed threshold if LCDA is currently active.*/
   float32_T k_lcda_host_activation_speed_max; /**<Maximal host speed for which LCDA shall be active.*/
   float32_T k_lcda_host_activation_speed_max_hys; /**<Hysteresis for k_lcda_host_activation_speed_min_max. This value will be added to maximal activation speed threshold if LCDA is currently active.*/
   float32_T k_lcda_distance_traveled_scale_factor; /**<Inhibits the amount by which the CVW zone length restores after it was shortened. The restoration (travelled distance of ego) is multiplied by this value.*/
   float32_T k_lcda_min_curve_radius; /**<LCDA is disabled when the host is navigating a curve with a radius smaller than this value.*/
   float32_T k_lcda_min_curve_radius_hys; /**<Hysteresis value to be applied before LCDA is disabled for small curve radius.*/
   float32_T k_lcda_curve_radius_threshold_for_zone_adaptation; /**<If the curve radius is below this value, the cvw zone shortening is applied.*/
   float32_T k_lcda_min_lane_width; /**<Minimum lane width - used for plausibility checking during zone creation. If input lane width is lower than this, this value width is used.*/
   float32_T k_lcda_max_lane_width; /**<Maximum lane width - used for plausibility checking during zone creation. If input lane width is larger than this, this max lane width is used.*/
   float32_T k_lcda_min_ego_vehicle_width; /**<Minimum ego width - used for plausibility checking during zone creation. If input ego width is lower than this, this min value is used.*/
   float32_T k_lcda_max_ego_vehicle_width; /**<Maximum ego width - used for plausibility checking during zone creation. If input ego width is larger than this, this max value is used.*/
   float32_T k_lcda_min_ego_vehicle_length; /**<Minimum ego length - used for plausibility checking during zone creation. If input ego length is lower than this, this min value is used.*/
   float32_T k_lcda_max_ego_vehicle_length; /**<>Maximum ego length - used for plausibility checking during zone creation. If input ego length is larger than this, this max value is used.*/
   float32_T k_lcda_zone_intersect_critical_point_lateral_ratio; /**<Determines which point is checked for objects heading towards the zone if CRITICAL_POINT_IN_ZONE is used as intersect detection. 0.0=lateral center of object, 1.0=lateral inner edge.*/
   float32_T k_lcda_exist_prob_lc_intention_hys_offset; /**<Offset for the existence probability of an alerting object when a lane change intention is given. This offset will be subtracted from the minimum needed existence probability k_lcda_min_exist_prop.*/
   float32_T k_lcda_exist_prob_hys_offset; /**<Offset for the existence probability for an object which gave an alert for an arbitrary submodule. This offset will be subtracted from the minimum needed existence probability k_lcda_min_exist_prop.*/
   float32_T k_lcda_lane_change_intention_vel_lat_thresh; /**<Threshold for relative velocity above which alert suppression functionality would be enabled.*/
   float32_T k_bsw_lane_change_intention_pos_lat_thres; /**<Threshold for lateral zone occupation above which alert won't be suppressed.*/
   float32_T k_bsw_lane_change_intention_pos_long_thres; /**<Threshold for longitudinal position below which alert won't be suppressed.*/
   float32_T k_lcda_ego_lane_effective_lane_width_factor; /**<Lane width factor to be used when determining if a LCDA object is within the ego lane. Value should be between 0 and 1.*/
   float32_T k_lcda_min_exist_prob_lc_intention; /**<Minimum existence probability threshold in case of lane change intention.*/
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
   float32_T k_bsw_lateral_distance_zone; /**<Distance between car and beginning of the BDS-Zone.*/
   float32_T k_bsw_overlap_area_threshold; /**<Mimium area of target FOI, overlapped by BSW zone to raise BSW alert, expressed in percentage of FOI.*/
   float32_T k_bsw_fallback_rel_vel_thres; /**<A target will be considered as slow back falling object if the relative velocity is above threshold.*/
   float32_T k_bsw_fallback_rel_vel_thres_hys; /**<A target can be reclassified as fast back falling object if the relative velocity is below k_bsw_fallback_rel_vel_thres - k_bsw_fallback_rel_vel_thres_hys.*/
   float32_T k_bsw_dynzone_speed[LCDA_K_BSW_DYNZONE_SPEED_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the host vehicle speed  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_dynzone_range[LCDA_K_BSW_DYNZONE_RANGE_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the host vehicle speed  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_min_exist_prob_radar_guardrail; /**<The minimum existence probability of a radar guardrail to determine, if an object is in conflict with its environment.*/
   float32_T k_min_exist_prob_camera_guardrail; /**<The minimum existence probability of a camera guardrail to determine, if an object is in conflict with its environment.*/
   float32_T k_bsw_dynzone_speed_dropback[LCDA_K_BSW_DYNZONE_SPEED_DROPBACK_ARRAY_SIZE_DIM0]; /**<BSW look up table to shrink the dynamic warn zones during drop back = slow overtaking maneuvers based on relative speeds.*/
   float32_T k_bsw_dynzone_speed_dropback_max; /**<Maximum speed until the BSW look up table to shrink the dynamic warn zones is active.*/
   float32_T k_bsw_dynzone_range_dropback[LCDA_K_BSW_DYNZONE_RANGE_DROPBACK_ARRAY_SIZE_DIM0]; /**<BSW look up table to shrink the dynamic warn zones during drop back = slow overtaking maneuvers based on relative speeds.*/
   float32_T k_bsw_n_line_position_for_long_object_sot_scenario; /**<In SOT scenario alert is active when the rear of long object reaches this line. Close to the position of rear host bumper (N line) in VCS coordinates (shall be negative) */
   float32_T k_bsw_line_to_stop_TOS_alert; /**<Sets the longitudinal line for front bsw zone boundary to disable alert for TOS case. Requires k_bsw_stop_alert_reaching_front_custom_limit_mode = 2*/
   float32_T k_bsw_min_length_long_object; /**<Minimum length of the object that will be classified as a long object.*/
   float32_T k_bsw_min_length_long_object_hys; /**<Minimum length of the object that will be classified as a long object.*/
   float32_T k_bsw_suppress_late_warning_max_time_till_leave; /**<Maximum time until the object leaves the rear bsd zone border in which a BSD warning can be suppressed.*/
   float32_T k_bsw_suppress_late_warning_min_pos_behind_host; /**<Minimum object distance behind ego rear at which a BSD warning can be suppressed.*/
   float32_T k_bsw_suppress_late_warning_max_rel_vel; /**<Maximum relative velocity at which BSD warning can be suppressed.*/
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
   float32_T k_lcda_max_range; /**<Maximum Range of a track that can trigger a warning.*/
   float32_T k_cvw_gap_bridge; /**<Parameter describing the time between two alerts, when the Alert should not go off.*/
   float32_T k_cvw_candidate_ttc; /**<If TTC of an object is lower than this threshold it is considered a candidate for a CVW alert.*/
   float32_T k_cvw_ttc; /**<Maximum TTC to issue an alert.*/
   float32_T k_cvw_ttc_hys; /**<Added to TTC threshold when an alert is active.*/
   float32_T k_cvw_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects in the cvw zone.*/
   float32_T k_cvw_min_obj_curvi_long_vel; /**<The minimum longitudinal curvi velocity of an object for the cvw zone.*/
   float32_T k_cvw_lane_change_intention_zone_x[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_X_ARRAY_SIZE_DIM0]; /**<Longitudinal coordinates of the corners of a polygon defining cvw zone in curvi coordinates in case of triggering a lane change intention. Depending on k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone and k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present different coordinates defined in k_cvw_lane_change_intention_zone_small_x might be used instead.*/
   float32_T k_cvw_lane_change_intention_zone_y[LCDA_K_CVW_LANE_CHANGE_INTENTION_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Lateral coordinates of the corners of a polygon defining cvw zone in curvi coordinates in case of triggering a lane change intention. Depending on k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone and k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present different coordinates defined in k_cvw_lane_change_intention_zone_small_y might be used instead.*/
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
   float32_T k_slc_zone_x[LCDA_K_SLC_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining Sim Lane Change zone in curvi coordinates.*/
   float32_T k_slc_zone_y[LCDA_K_SLC_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining the SLC zone in curvi coordinates. It is specified as a factor of the lane width.*/
   float32_T k_slc_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects to be considered as valid Sim Lane Change candidates.*/
   float32_T k_slc_min_obj_curvi_long_vel_abs; /**<The minimum longitudinal absolute curvi velocity of the object to be considered as a valid candidate for Sim Lane Change.*/
   float32_T k_slc_critical_lat_ttc; /**<Lateral TTC threshold below which a target is considered as critical for SLC.*/
   float32_T k_slc_critical_lon_ttc; /**<Longitudinal TTC threshold below which a target is considered as critical for SLC.*/
   float32_T k_slc_lateral_ttc_lookup[LCDA_K_SLC_LATERAL_TTC_LOOKUP_ARRAY_SIZE_DIM0]; /**<Look up table with the lateral TTC values to be used to find the corresponding lane change probability> Note that the values should be in strictly monotonic order.*/
   float32_T k_slc_lane_change_prob_lookup[LCDA_K_SLC_LANE_CHANGE_PROB_LOOKUP_ARRAY_SIZE_DIM0]; /**<Look up table with the lane change probability (to be used along with the lateral ttc lookup).*/
   float32_T k_slc_lookup_ego_speed[LCDA_K_SLC_LOOKUP_EGO_SPEED_ARRAY_SIZE_DIM0]; /**<Look-up table for the ego speed used to determine the ego overlap offset (see k_slc_lookup_ego_overlap_offset).*/
   float32_T k_slc_lookup_ego_overlap_offset[LCDA_K_SLC_LOOKUP_EGO_OVERLAP_OFFSET_ARRAY_SIZE_DIM0]; /**<Look-up table for the ego speed dependent (see k_slc_lookup_ego_speed) ego overlap offset used to determine if an object is classified as besides the ego.*/
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
   float32_T k_lcda_dyn_cvw_ttc_speed_parameter; /**<None*/
   float32_T k_bsw_object_position_correction_threshold; /**<Determines relative longitudinal velocity threshold above which higher correction position value will be applied.*/
   float32_T k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh; /**<Relative velocity above which higher compensation value will be added to ttc threshold to compensate system delays*/
   float32_T k_lcda_dyn_cvw_ttc_compens_time[LCDA_K_LCDA_DYN_CVW_TTC_COMPENS_TIME_ARRAY_SIZE_DIM0]; /**<Determines time that will be added to ttc threshold to compensate system delays - first value below thresh, second above thresh */
   float32_T k_bsw_dynzone_object_rel_vel[LCDA_K_BSW_DYNZONE_OBJECT_REL_VEL_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the object relative velocity  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   float32_T k_bsw_dynzone_object_range[LCDA_K_BSW_DYNZONE_OBJECT_RANGE_ARRAY_SIZE_DIM0]; /**<Dynamic BSW zone lookup table to increase the BSW zone based on the object relative velocity  (kph: [ 0 62.5 125 187.5 250 360]) Note: The point coordinates have to be sequenced in strict monotonic order.*/
   uint16_t k_lm_min_count_in_state; /**<Minimum number of cycles the criteria for a given state should be satisfied before the output lane model switches to the given state.*/
   boolean_T k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present; /**<Flag indicating if the smaller lane change intention cvw zone should be used if no radar guardrail is present.*/
   boolean_T k_bsw_f_zone_extension_by_diff_width_host_vs_trailer; /**<Enables adjustment lateral dimension of the BSW zone if the trailer is wider than the host, modify the size with this value.*/
   boolean_T k_bsw_f_enable_trailer_zone_adjustment_on_ego_side; /**<Enables modification of the BSW zone by shifting the boundary on the ego side based on trailer information in core input data.*/
   boolean_T k_bsw_f_enable_trailer_zone_adjustment_on_outer_side; /**<Enables modification of the BSW zone by shifting the boundary on the ego side based on trailer information in core input data.*/
   boolean_T k_elc_enable; /**<Enables the Evasive Lane Change feature when the k_elc_enable_via_cal is also set to TRUE.*/
   boolean_T k_elc_enable_via_cal; /**<Used to decide whether Evasive Lane Change should be enabled based on the calibration. TRUE=>k_elc_enable is used to enable ELC, FALSE=>elcEnabled flag from the core input is used to enable ELC.*/
   boolean_T k_lcda_use_default_warntrigger_hmi; /**<Decides whether default hmi warntrigger should be used or not.*/
   boolean_T k_lm_use_default_lane_information; /**<Master override flag to use the default lane information.*/
   boolean_T k_lm_enable_use_navigation_data; /**<Enable lane model estimation based on navigation data.*/
   boolean_T k_lm_enable_use_camera_data; /**<Enable lane model estimation based on camera data.*/
   boolean_T k_lm_enable_use_vehicle_dyn; /**<Enable lane model estimation based on vehicle dynamics.*/
   boolean_T k_lcda_enable; /**<Enables the LCDA feature when the k_lcda_enable_via_cal is also set to TRUE.*/
   boolean_T k_lcda_enable_via_cal; /**<Used to decide whether LCDA should be enabled based on the calibration. TRUE=>k_lcda_enable is used to enable LCDA, FALSE=>lcdaEnabled flag from the core input is used to enable LCDA.*/
   boolean_T k_lcda_f_disable_due_to_small_curve_radius; /**<Decides whether entire LCDA functionality should be disabled when the host is moving with a very small curve radius such as a round about.*/
   boolean_T k_lcda_allow_track_status_new; /**<Allow using the tracks with TRACK_STATUS_NEW in the valid BSW/CVW candidate selection. When it is FALSE, only the mature and coasted tracks are considered for candidate selection.*/
   boolean_T k_lcda_f_enable_obj_in_ego_lane_check; /**<Enable checking if an object is in the ego lane. When this is set to true, alerts from objects that are considered to be in the ego lane will be suppressed.*/
   boolean_T k_lcda_f_enable_suppress_alert_object_no_lane_change_intention; /**<Enable checking if an object will change lane based on relative lateral velocity. When this is set to true, alerts from objects that are in the ego lane and move towards ego center will be suppressed.*/
   boolean_T k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge; /**<Enable checking if at least part of the object is outside the zone. When this is set to true, alerts from objects that protrudes beyond the boundaries of the zone will be suppressed.*/
   boolean_T k_bsw_f_enable_adv_pos_data_lane_change_intention; /**<Enables additional position verification for alert suppression in lane change intention algorithm.*/
   boolean_T k_lcda_f_enable_alert_obj_in_ego_lane; /**<Enable alert when object is in ego lane and in BSW zone.*/
   boolean_T k_lcda_ego_lane_check_center_point_only; /**<Enables the simple center point check to determine ego lane occupation. If disabled a more sophisticated polygon overlap is performed.*/
   boolean_T k_bsw_uses_cvw_alert_state_enabled; /**<Enables use of CVW alert state info in BSW 1) BSW hys zone is used directly 2) BSW mature count in zone is bypassed if there is a previous CVW alert on the object.*/
   boolean_T k_bsw_overlap_area_check_enable; /**<Enables verification of target FOI area overlaped by BSW zone.*/
   boolean_T k_bsw_use_curvi_coordinates; /**<When set to TRUE the BSW module will always use curvi coordinates for every check. Otherwise VCS coordinates will be used.*/
   boolean_T k_bsw_enable; /**<Enables the BSW feature when the k_bsw_enable_via_cal is also set to TRUE.*/
   boolean_T k_bsw_enable_via_cal; /**<Used to decide whether BSW should be enabled based on the calibration. TRUE=>k_bsw_enable is used to enable BSW, FALSE=>bswEnabled flag from the core input is used to enable BSW.*/
   boolean_T k_bsw_enable_zone_front_boundary_specific_conditions; /**<Enables alert behaviours different than typical for entering/leaving front zone boundary.*/
   boolean_T k_bsw_enable_specific_front_sot_conditions; /**<Enables alert behaviours different than typical for entering front zone boundary.*/
   boolean_T k_bsw_hold_alert_long_object; /**<Objects that are overtaking and are classified as long trucks would hold alert until whole object leave the zone.*/
   boolean_T k_bsw_enable_factor_based_host_speed_adjustment; /**<Enables a host speed dependend bsw zone adjustment with the same factor for normal and hysteresis zone related to host rear. NOTE: Zone definitions must be handled very carefully when enabled!.*/
   boolean_T k_bsw_enable_dynspeed_zone; /**<Enables dynamic BSW zone adaptation based on ego speed.*/
   boolean_T k_bsw_enable_trailer_zone_extension; /**<Enables an additional adjustment of the BSW zone based on trailer information in core input.*/
   boolean_T k_cvw_enable; /**<Enables the CVW feature when the k_cvw_enable_via_cal is also set to TRUE.*/
   boolean_T k_cvw_enable_via_cal; /**<Used to decide whether CVW should be enabled based on the calibration. TRUE=>k_cvw_enable is used to enable CVW, FALSE=>cvwEnabled flag from the core input is used to enable CVW.*/
   boolean_T k_enable_cvw_curve_zone_adaptation; /**<allows customers to enable and disable the curveZoneAdaptation function to cut the cvw zone in curve.*/
   boolean_T k_cvw_f_most_crit_obj_must_be_closest_relevant_obj; /**<Limits the CVW alert to the closest relevant CVW object. A critical object behind a relevant object will be suppressed.*/
   boolean_T k_slc_enable; /**<Enables the Sim Lane Change feature when the k_slc_enable_via_cal is also set to TRUE.*/
   boolean_T k_slc_enable_via_cal; /**<Used to decide whether Sim Lane Change should be enabled based on the calibration. TRUE=>k_slc_enable is used to enable SLC, FALSE=>slcEnabled flag from the core input is used to enable SLC.*/
   boolean_T k_lcda_f_enable_rel_vel_logic_in_ego_lane_check; /**<Ego lane occupancy will be determined on position as well as relative velocity of the object*/
   boolean_T k_bsw_f_fallback_default_status_slow; /**<Changes default fallback status of the object to the slow*/
   boolean_T k_lcda_f_enable_obj_reflection_flag_check; /**<Enables f_reflection flag checking to ignore potential ghost objects, detected by the tracker*/
   boolean_T k_lcda_f_enable_fallback_handler; /**<Enables fallback handler module*/
   boolean_T k_lcda_f_enable_dyn_cvw_ttc_threshold; /**<Enables the CVW dynamic ttc threshold.*/
   boolean_T k_bsw_shrink_zone_method; /**<Determines whether zone will be shrunk using 0 - multiplication or 1 - subtraction ()*/
   boolean_T k_bsw_f_use_front_zone_as_n_line; /**<Determines whether front zone line should be used as n line is SOT scenario*/
   boolean_T k_bsw_f_enable_object_rel_vel_dynzone; /**<Enables dynamic BSW zone adaptation based on object relative velocity.*/
   boolean_T k_bsw_f_use_zone_without_hysteresis_lane_change_intention; /**<Enables using of the initial (no hysteresesis) for the lateral overlapping check in lachne change intention logic.*/
   uint8_t k_elc_alert_holding_cycles; /**<Number of holding cycles for ELC alert.*/
   uint8_t k_lcda_turn_signal_coast_cycles; /**<Number of scans to hold turn signal after it is cancelled by the driver.*/
   uint8_t k_lcda_lc_intention_cycles_for_zone_change_threshold; /**<Number of cycles before the lane change intention zone is changed depending on changed guardrail information.*/
   uint8_t k_cvw_min_mature_cycles_lc_intention; /**<Minimum number of mature cylces in lane change intention zone as a valid cvw target before issuing an alert*/
   uint8_t k_elc_min_mature_cycles; /**< When the mature count in zone exceeds this threshold the object is allowed to create an ELC alert.*/
   uint8_t k_lcda_default_warntrigger_hmi; /**<Default hmi warntrigger.*/
   uint8_t k_lm_min_qualification_cycle_count; /**<The minimum number of cycles the lane model data should hold its value before it is considered as valid.*/
   uint8_t k_lm_min_output_hold_cycles; /**<The minimum number of cycles the previous values of the lane model output should be held.*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   uint8_t k_bsw_alert_holding_cycles; /**<Number of holding cycles for BSW alert.*/
   uint8_t k_cvw_alert_holding_cycles; /**<Number of holding cycles for CVW alert.*/
   uint8_t k_slc_alert_holding_cycles; /**<Number of holding cycles for SLC alert.*/
   uint8_t k_lcda_zone_check_method; /**<Sets the method for checking the object zone occupation. 0: Checks zone overlap, 1: Only check reference point.*/
   uint8_t k_bsw_fallback_fast_to_slow_qual_thres; /**<Qualification counter threshold for a fallback transition from fast to slow.*/
   uint8_t k_bsw_min_mature_cycles; /**<Minimum number of mature cycles in zone as a valid bsw target before issuing an alert.*/
   uint8_t k_lcda_min_track_age; /**<Minimum Track age for an object to be selected as a valid candidate for LCDA.*/
   uint8_t k_bsw_alert_track_age; /**<Minimum Track age to trigger a warning immediately even though the object does not pass the mature in zone check.*/
   uint8_t k_bsw_stop_alert_reaching_front_custom_limit_mode; /**<Sets the type of front custom boundary, that will stop alert, when reached by overtaking object. 0: BSW zone longitudinal max, 1: Host front bumper, 2: Custom line - k_bsw_line_to_stop_TOS_alert.*/
   uint8_t k_cvw_min_mature_cycles; /**<Minimum number of mature cylces in zone as a valid cvw target before issuing an alert.*/
   uint8_t k_cvw_zone_calculation_mode; /**<Determines how the zone will be calculated. 0=DEFAULT, 1=Zone based on Vehicle Length and Lane Width, 2=Zone fixed to be taken from LCDA core input.*/
   uint8_t k_slc_min_mature_cycles; /**<When the count in zone exceeds this threshold the object is allowed to create an SLC alert.*/
   uint8_t k_slc_alert_qualifying_counter; /**<When the qualifying counter is equal to or greater than this threshold the SLC alert won't be suppressed.*/
   uint8_t k_lcda_f_enable_camera_based_guardrail; /**<Flag to enable or disable the usage of camera based guardrail data for LCDA.*/
} Lcda_Core_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* LCDA_CORE_CALIBRATION_T_H */
