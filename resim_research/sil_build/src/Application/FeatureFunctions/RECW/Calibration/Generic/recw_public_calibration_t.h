# ifndef RECW_PUBLIC_CALIBRATION_T_H
# define RECW_PUBLIC_CALIBRATION_T_H

/**
* @file recw_public_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in recw_cal.xml.
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
#define RECW_K_RECW_MIN_REL_VELOCITY_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_REL_VELOCITY_HYS_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_HEADING_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_CRASH_PROB_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_EXISTENCE_PROB_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_TTC_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_OVERLAP_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_HOST_SPEED_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_HOST_SPEED_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_REL_VELOCITY_FOR_MAX_TTC_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_TTC_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_REL_VELOCITY_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_LOOKUP_BRAKING_DECELERATION_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_LOOKUP_BRAKING_PROBABILITY_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_LOOKUP_STEERING_ACCELERATION_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_LOOKUP_STEERING_PROBABILITY_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_F_ALLOW_ALERT_ON_COASTED_OBJECTS_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_F_USE_REAR_BLOCKAGE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_ALERT_HOLDING_CYCLES_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_STAGE_AGE_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_CYCLES_ALERT_DURATION_ARRAY_SIZE_DIM0 (2u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_REL_VELOCITY_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_REL_VELOCITY_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_HEADING_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_CRASH_PROB_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_EXISTENCE_PROB_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_TTC_FOR_ALERT_LEVEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_OVERLAP_FOR_ALERT_LEVEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_HOST_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_HOST_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_REL_VELOCITY_FOR_MAX_TTC_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_TTC_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_REL_VELOCITY_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_LOOKUP_BRAKING_DECELERATION_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_LOOKUP_BRAKING_PROBABILITY_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_LOOKUP_STEERING_ACCELERATION_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_LOOKUP_STEERING_PROBABILITY_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_F_ALLOW_ALERT_ON_COASTED_OBJECTS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_F_USE_REAR_BLOCKAGE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_ALERT_HOLDING_CYCLES_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MIN_STAGE_AGE_FOR_ALERT_LEVEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_K_RECW_MAX_CYCLES_ALERT_DURATION_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_PUBLIC_CALIBRATION_SIZE (399u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_recw_f_suppress_alert_lvl_2_for_pedestrian; /**<Suppress alert level 2 for pedestrian object.*/
   uint8_t k_recb_max_cycles_braking_request_duration; /**<Defines the maximum number of cycles a braking request can be active before it will be cancelled. This is independent on the target that caused the braking request.*/
   uint8_t k_recb_qualifier_nominal_acceleration; /**< The mode requested by Central Vehicle Management required for the braking request.*/
   uint8_t k_recb_integrity; /**<The ASIL level required for the braking request.*/
   uint8_t k_recb_ssm_request_cancelled; /**<The standstill management required if a braking request is cancelled.*/
   uint8_t k_recb_ssm_braking; /**<The standstill management required during a braking request.*/
   uint8_t k_recb_min_cycles_host_standstill; /**<Minimal time for which the host has to be stationary before a braking request can be raised.*/
   uint8_t k_recw_rear_blockage_qualifying_cycles; /**<Number of consecutive qualifying cycles with rear blockage before rear blockage is set for alert level calculation.*/
   uint8_t k_recw_max_allowed_consecutive_coasted_cycles; /**<Maximal number of consecutive coasted cycles for which an object is still considered as valid for RECW.*/
   uint8_t k_recw_lane_filter_num_consecutive_cycles; /**<Number of consecutive cycles required by target to be within lane in order to be able to trigger warning*/
   uint8_t k_recw_en_active_car_wash_logic; /**<0 - car wash deactivated, 1 - car wash logic based on target state, 2 - car wash logic based on neutral gear position.*/
   uint8_t k_recw_min_cycles_with_min_crash_prob; /**<Number of consecutive cycles a crash probability of k_recw_min_crash_prob[0] for alert level 1 has to be reached before an object can raise an alert.*/
   uint8_t k_recw_min_object_age; /**<Minimal age of an object in cycles to be considered by RECW.*/
   uint8_t k_recw_min_age_for_close_slow_targets; /**<Minimum age that a slow and close target must have to raise an alert*/
   uint8_t k_recw_max_cycles_alert_duration[RECW_K_RECW_MAX_CYCLES_ALERT_DURATION_ARRAY_SIZE_DIM0]; /**<Defines the maximum number of cycles a warning can be active for alert level 1 and alert level 2 before it will be disabled. This is independent on the target that caused the warning.*/
   uint8_t k_recw_min_stage_age_for_alert_level[RECW_K_RECW_MIN_STAGE_AGE_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0]; /**<Minimal stage age to raise an alert of level 1 and alert of level 2. Coasted objects are also controlled via k_recw_max_allowed_consecutive_coasted_cycles and k_recw_f_allow_alert_on_coasted_objects.*/
   uint8_t k_recw_alert_qualifying_cycles; /**<Number of qualifying cycles for alert level 1. Alert level 2 does not require qualifying.*/
   uint8_t k_recw_alert_holding_cycles[RECW_K_RECW_ALERT_HOLDING_CYCLES_ARRAY_SIZE_DIM0]; /**<Number of holding cycles for alerts of level 1 and alerts of level 2.*/
   boolean_T k_recb_f_enable_recb; /**<Switch to enable the RECB functionality. If turned off RECB will be disabled.*/
   boolean_T k_recw_f_enable_traffic_light_ghost_detection; /**<Switch to enable the traffic light ghost detection.*/
   boolean_T k_recw_f_enable_heading_filter; /**<Switch to enable usage of the heading filter. If turned off the filtered heading will be replaced with the heading provided by the tracker.*/
   boolean_T k_recw_f_apply_lane_filter; /**<If flag is true, targets outside the ego lane will be exluded from warnings.*/
   boolean_T k_recw_f_use_rear_blockage[RECW_K_RECW_F_USE_REAR_BLOCKAGE_ARRAY_SIZE_DIM0]; /**<Flags define whether the rear end blockage algorithm should be used for alert level 1 and alert of level 2.*/
   boolean_T k_recw_f_allow_alert_on_coasted_objects[RECW_K_RECW_F_ALLOW_ALERT_ON_COASTED_OBJECTS_ARRAY_SIZE_DIM0]; /**<Flags define if coasted objects are allowed to raise an alert of level 1 and alert of level 2.*/
   boolean_T k_recw_f_only_allow_consecutive_alert_levels; /**<Flag to only allow consecutive alerts. If activated level 2 alerts can only follow a previous level 1 alert.*/
   boolean_T k_recw_f_make_use_of_guardrail; /**<Switch to activate consideration of guardrail information in estimation of crash probability.*/
   float32_T k_recb_accelerator_pedal_gradient_threshold; /**<Threshold for the accelerator pedal gradient above which a braking request will be cancelled immediately.*/
   float32_T k_recb_nominal_acceleration_applied; /**<The acceleration which is applied to the brakes if a braking request is raised.*/
   float32_T k_recb_max_ttc; /**<The TTC of the relevant target is required to be below this threshold to raise a braking request.*/
   float32_T k_recb_min_rel_velocity_hys; /**<Hysteresis for the minimal relative velocity of an object to be able to raise a braking request. This value will be subtracted from k_recb_min_rel_velocity if a braking request was active before.*/
   float32_T k_recb_min_rel_velocity; /**<Minimal relative velocity of the relevant target which is required to raise a braking request.*/
   float32_T k_recb_max_velocity_host_standstill; /**<Maximal velocity for which the host vehicle will be considered as standstill in the calculation of k_recb_min_cycles_host_standstill.*/
   float32_T k_recb_max_host_speed_hys; /**<Hysteresis for maximal host speed for which RECB can raise a braking request. This value will be added to k_recb_max_host_speed if the host speed was in the allowed range before.*/
   float32_T k_recb_max_host_speed; /**<Maximal host speed for which RECB can raise a braking request.*/
   float32_T k_recw_min_speed_not_stationary; /**<Minimum speed an object needs to provide so that it is classified as non-stationary.*/
   float32_T k_recw_heading_accuracy_threshold; /**<Maximal allowed heading accuracy (greater values are less accurate)*/
   float32_T k_recw_rear_blockage_ego_speed_threshold; /**<Ego speed threshold to release rear blockage. If object speed is above this value rear blockage cannot be active.*/
   float32_T k_recw_rear_blockage_length; /**<Length of blockage zone.*/
   float32_T k_recw_rear_blockage_width; /**<Width of blockage zone.*/
   float32_T k_recw_rear_blockage_speed_threshold; /**<Only objects below this speed threshold are considered valid as blockers for the ego rear.*/
   float32_T k_recw_max_allowed_heading_diff; /**<This defines the maximal allowed difference between the internally filtered heading and the one provided by the tracker for a valid object.*/
   float32_T k_recw_max_allowed_rel_vel_lat_diff; /**<This defines the maximal allowed difference between the internally filtered lateral relative velocity and the one provided by the tracker for a valid object.*/
   float32_T k_recw_max_allowed_rel_vel_long_diff; /**<This defines the maximal allowed difference between the internally filtered longitudinal relative velocity and the one provided by the tracker for a valid object.*/
   float32_T k_recw_max_object_width_warn_on; /**<This is the max object width to be considered by the algo*/
   float32_T k_recw_max_eclipse_value_for_valid_object; /**<Maximal allowed eclipse value for an object to be considered as valid for RECW.*/
   float32_T k_recw_lookup_steering_probability[RECW_K_RECW_LOOKUP_STEERING_PROBABILITY_ARRAY_SIZE_DIM0]; /**<Look-up table of probabilities assigned to steering accelerations specified in k_recw_lookup_steering_acceleration.*/
   float32_T k_recw_lookup_steering_acceleration[RECW_K_RECW_LOOKUP_STEERING_ACCELERATION_ARRAY_SIZE_DIM0]; /**<Look-up table of steering acceleration used for look up of steering probability.*/
   float32_T k_recw_lookup_braking_probability[RECW_K_RECW_LOOKUP_BRAKING_PROBABILITY_ARRAY_SIZE_DIM0]; /**<Look-up table of probabilities assigned to braking decelerations specified in k_recw_lookup_braking_deceleration.*/
   float32_T k_recw_lookup_braking_deceleration[RECW_K_RECW_LOOKUP_BRAKING_DECELERATION_ARRAY_SIZE_DIM0]; /**<Look-up table of brake deceleration used for look up of braking probability.*/
   float32_T k_recw_lane_width_slope; /**<Slope that can be used to increase the lane width used for the lane filter at higher distances.*/
   float32_T k_recw_lane_filter_max_abs_ego_speed_vcs_coord; /**<Maximum absolute ego speed for which the lane filter calculation will be done with vcs coordinates instead of curvi coordinates.*/
   float32_T k_recw_lane_filter_width_hys; /**<Hysteresis for the lane filter width. This value will be added to k_recw_lane_filter_width, if the object was critical before.*/
   float32_T k_recw_lane_filter_width; /**<If k_recw_f_apply_lane_filter is active, only objects inside a zone with the respective lane width will be considered as relevant for RECW.*/
   float32_T k_recw_max_speed_ego_car_wash; /**<Maximum ego speed at which car wash logic will be applied.*/
   float32_T k_recw_min_rel_lon_vel_car_wash; /**<Minimum initial relative longitudinal velocity between ego and target at which the PCR warning can be suppressed due to car wash logic.*/
   float32_T k_recw_max_lat_distance_car_wash; /**<Maximum initial lateral distance between ego and target vehicle in which the PCR warning can be suppressed due to car wash logic.*/
   float32_T k_recw_max_lon_distance_car_wash; /**<Maximum initial longitudinal distance between ego and target vehicle in which the PCR warning can be suppressed due to car wash logic.*/
   float32_T k_recw_max_rel_lon_vel_release_car_wash; /**<Maximum relative longitudinal velocity between ego and target at which the PCR warning suppresion due to car wash logic can be released.*/
   float32_T k_recw_max_rel_velocity_hys; /**<Hysteresis for the maximal relative velocity k_recw_max_rel_velocity. This value is added to k_recw_max_rel_velocity, if the object was critical before.*/
   float32_T k_recw_max_rel_velocity[RECW_K_RECW_MAX_REL_VELOCITY_ARRAY_SIZE_DIM0]; /**<The maximal relative velocity of an object has to be equal or less than this calibration value (alert level dependant) to be considered by RECW.*/
   float32_T k_recw_min_abs_speed_for_young_close_targets; /**<Minimum absolute speed of a young and close target to raise an alert*/
   float32_T k_recw_min_dist_for_young_slow_targets; /**<Minimum distance of a young and slow target to raise an alert*/
   float32_T k_recw_max_ttc_threshold[RECW_K_RECW_MAX_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<Maximal ttc threshold that should be used for alert level 1 and alert level 2. This will be applied for valid relative velocities greater or equal to the corresponding array entry of k_recw_min_rel_velocity_for_max_ttc_threshold. */
   float32_T k_recw_min_rel_velocity_for_max_ttc_threshold[RECW_K_RECW_MIN_REL_VELOCITY_FOR_MAX_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<Minimal relative velocity for alert level 1 and alert level 2 for which the corresponding maximal ttc threshold from k_recw_max_ttc_threshold shall be used. For lower relative velocities the ttc threshold decreases linearly.*/
   float32_T k_recw_max_host_speed_hys; /**<Hysteresis for the maximal host speed for which RECW should be operational. The value will be added to k_recw_max_host_speed if RECW was operational before.*/
   float32_T k_recw_max_host_speed[RECW_K_RECW_MAX_HOST_SPEED_ARRAY_SIZE_DIM0]; /**<Maximal host speed, alert level dependant, for which RECW should be operational.*/
   float32_T k_recw_min_host_speed_hys; /**<Hysteresis for the minimal host speed for which RECW should be operational. The value will be subtracted from k_recw_min_host_speed if RECW was operational before.*/
   float32_T k_recw_min_host_speed[RECW_K_RECW_MIN_HOST_SPEED_ARRAY_SIZE_DIM0]; /**<Minimal host speed, alert level dependant, for which RECW should be operational.*/
   float32_T k_recw_min_overlap_for_alert_level[RECW_K_RECW_MIN_OVERLAP_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0]; /**<Minimum overlap of predicted object position at the host rear bumper to output alert level 1 an 2.*/
   float32_T k_recw_min_ttc_for_alert_level[RECW_K_RECW_MIN_TTC_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0]; /**<Minimal TTC at which a level 1 alert and a level 2 alert can be raised.*/
   float32_T k_recw_min_existence_prob_hys; /**<Hysteresis for minimal existence probability to raise a level 1 alert. This value will be subtracted from k_recw_min_existence_prob[0], if the object was critical before.*/
   float32_T k_recw_min_existence_prob[RECW_K_RECW_MIN_EXISTENCE_PROB_ARRAY_SIZE_DIM0]; /**<Minimal existence probability to raise a level 1 alert and level 2 alert.*/
   float32_T k_recw_min_crash_prob[RECW_K_RECW_MIN_CRASH_PROB_ARRAY_SIZE_DIM0]; /**<Minimal crash probability to raise a level 1 alert and level 2 alert.*/
   float32_T k_recw_average_sensor_latency; /**<Average overall latency between real world and feature output in seconds.*/
   float32_T k_recw_factor_ego_width; /**<Factor to adapt the relevant ego width in order to adjust the feature sensitivity.*/
   float32_T k_recw_max_heading_hys; /**<Hysteresis for the heading angle of an object to be considered for RECW. The value will be added to k_recw_max_approach_angle if the object was critical before.*/
   float32_T k_recw_max_heading[RECW_K_RECW_MAX_HEADING_ARRAY_SIZE_DIM0]; /**<Maximal heading angle of an object to be considered for RECW depending on alert level.*/
   float32_T k_recw_min_rel_velocity_hys[RECW_K_RECW_MIN_REL_VELOCITY_HYS_ARRAY_SIZE_DIM0]; /**<Hysteresis for the minimal relative velocity k_recw_min_rel_velocity for alert level 1 and alert level 2. This value is subtracted from the corresponding array value of k_recw_min_rel_velocity, if the object was critical before.*/
   float32_T k_recw_min_rel_velocity[RECW_K_RECW_MIN_REL_VELOCITY_ARRAY_SIZE_DIM0]; /**<The minimal relative velocity of an object has to be greater or equal than this calibration value to be considered for alert level 1 and alert level 2.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Recw_Public_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_recw_min_rel_velocity[RECW_K_RECW_MIN_REL_VELOCITY_ARRAY_SIZE_DIM0]; /**<The minimal relative velocity of an object has to be greater or equal than this calibration value to be considered for alert level 1 and alert level 2.*/
   float32_T k_recw_min_rel_velocity_hys[RECW_K_RECW_MIN_REL_VELOCITY_HYS_ARRAY_SIZE_DIM0]; /**<Hysteresis for the minimal relative velocity k_recw_min_rel_velocity for alert level 1 and alert level 2. This value is subtracted from the corresponding array value of k_recw_min_rel_velocity, if the object was critical before.*/
   float32_T k_recw_max_heading[RECW_K_RECW_MAX_HEADING_ARRAY_SIZE_DIM0]; /**<Maximal heading angle of an object to be considered for RECW depending on alert level.*/
   float32_T k_recw_max_heading_hys; /**<Hysteresis for the heading angle of an object to be considered for RECW. The value will be added to k_recw_max_approach_angle if the object was critical before.*/
   float32_T k_recw_factor_ego_width; /**<Factor to adapt the relevant ego width in order to adjust the feature sensitivity.*/
   float32_T k_recw_average_sensor_latency; /**<Average overall latency between real world and feature output in seconds.*/
   float32_T k_recw_min_crash_prob[RECW_K_RECW_MIN_CRASH_PROB_ARRAY_SIZE_DIM0]; /**<Minimal crash probability to raise a level 1 alert and level 2 alert.*/
   float32_T k_recw_min_existence_prob[RECW_K_RECW_MIN_EXISTENCE_PROB_ARRAY_SIZE_DIM0]; /**<Minimal existence probability to raise a level 1 alert and level 2 alert.*/
   float32_T k_recw_min_existence_prob_hys; /**<Hysteresis for minimal existence probability to raise a level 1 alert. This value will be subtracted from k_recw_min_existence_prob[0], if the object was critical before.*/
   float32_T k_recw_min_ttc_for_alert_level[RECW_K_RECW_MIN_TTC_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0]; /**<Minimal TTC at which a level 1 alert and a level 2 alert can be raised.*/
   float32_T k_recw_min_overlap_for_alert_level[RECW_K_RECW_MIN_OVERLAP_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0]; /**<Minimum overlap of predicted object position at the host rear bumper to output alert level 1 an 2.*/
   float32_T k_recw_min_host_speed[RECW_K_RECW_MIN_HOST_SPEED_ARRAY_SIZE_DIM0]; /**<Minimal host speed, alert level dependant, for which RECW should be operational.*/
   float32_T k_recw_min_host_speed_hys; /**<Hysteresis for the minimal host speed for which RECW should be operational. The value will be subtracted from k_recw_min_host_speed if RECW was operational before.*/
   float32_T k_recw_max_host_speed[RECW_K_RECW_MAX_HOST_SPEED_ARRAY_SIZE_DIM0]; /**<Maximal host speed, alert level dependant, for which RECW should be operational.*/
   float32_T k_recw_max_host_speed_hys; /**<Hysteresis for the maximal host speed for which RECW should be operational. The value will be added to k_recw_max_host_speed if RECW was operational before.*/
   float32_T k_recw_min_rel_velocity_for_max_ttc_threshold[RECW_K_RECW_MIN_REL_VELOCITY_FOR_MAX_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<Minimal relative velocity for alert level 1 and alert level 2 for which the corresponding maximal ttc threshold from k_recw_max_ttc_threshold shall be used. For lower relative velocities the ttc threshold decreases linearly.*/
   float32_T k_recw_max_ttc_threshold[RECW_K_RECW_MAX_TTC_THRESHOLD_ARRAY_SIZE_DIM0]; /**<Maximal ttc threshold that should be used for alert level 1 and alert level 2. This will be applied for valid relative velocities greater or equal to the corresponding array entry of k_recw_min_rel_velocity_for_max_ttc_threshold. */
   float32_T k_recw_min_dist_for_young_slow_targets; /**<Minimum distance of a young and slow target to raise an alert*/
   float32_T k_recw_min_abs_speed_for_young_close_targets; /**<Minimum absolute speed of a young and close target to raise an alert*/
   float32_T k_recw_max_rel_velocity[RECW_K_RECW_MAX_REL_VELOCITY_ARRAY_SIZE_DIM0]; /**<The maximal relative velocity of an object has to be equal or less than this calibration value (alert level dependant) to be considered by RECW.*/
   float32_T k_recw_max_rel_velocity_hys; /**<Hysteresis for the maximal relative velocity k_recw_max_rel_velocity. This value is added to k_recw_max_rel_velocity, if the object was critical before.*/
   float32_T k_recw_max_rel_lon_vel_release_car_wash; /**<Maximum relative longitudinal velocity between ego and target at which the PCR warning suppresion due to car wash logic can be released.*/
   float32_T k_recw_max_lon_distance_car_wash; /**<Maximum initial longitudinal distance between ego and target vehicle in which the PCR warning can be suppressed due to car wash logic.*/
   float32_T k_recw_max_lat_distance_car_wash; /**<Maximum initial lateral distance between ego and target vehicle in which the PCR warning can be suppressed due to car wash logic.*/
   float32_T k_recw_min_rel_lon_vel_car_wash; /**<Minimum initial relative longitudinal velocity between ego and target at which the PCR warning can be suppressed due to car wash logic.*/
   float32_T k_recw_max_speed_ego_car_wash; /**<Maximum ego speed at which car wash logic will be applied.*/
   float32_T k_recw_lane_filter_width; /**<If k_recw_f_apply_lane_filter is active, only objects inside a zone with the respective lane width will be considered as relevant for RECW.*/
   float32_T k_recw_lane_filter_width_hys; /**<Hysteresis for the lane filter width. This value will be added to k_recw_lane_filter_width, if the object was critical before.*/
   float32_T k_recw_lane_filter_max_abs_ego_speed_vcs_coord; /**<Maximum absolute ego speed for which the lane filter calculation will be done with vcs coordinates instead of curvi coordinates.*/
   float32_T k_recw_lane_width_slope; /**<Slope that can be used to increase the lane width used for the lane filter at higher distances.*/
   float32_T k_recw_lookup_braking_deceleration[RECW_K_RECW_LOOKUP_BRAKING_DECELERATION_ARRAY_SIZE_DIM0]; /**<Look-up table of brake deceleration used for look up of braking probability.*/
   float32_T k_recw_lookup_braking_probability[RECW_K_RECW_LOOKUP_BRAKING_PROBABILITY_ARRAY_SIZE_DIM0]; /**<Look-up table of probabilities assigned to braking decelerations specified in k_recw_lookup_braking_deceleration.*/
   float32_T k_recw_lookup_steering_acceleration[RECW_K_RECW_LOOKUP_STEERING_ACCELERATION_ARRAY_SIZE_DIM0]; /**<Look-up table of steering acceleration used for look up of steering probability.*/
   float32_T k_recw_lookup_steering_probability[RECW_K_RECW_LOOKUP_STEERING_PROBABILITY_ARRAY_SIZE_DIM0]; /**<Look-up table of probabilities assigned to steering accelerations specified in k_recw_lookup_steering_acceleration.*/
   float32_T k_recw_max_eclipse_value_for_valid_object; /**<Maximal allowed eclipse value for an object to be considered as valid for RECW.*/
   float32_T k_recw_max_object_width_warn_on; /**<This is the max object width to be considered by the algo*/
   float32_T k_recw_max_allowed_rel_vel_long_diff; /**<This defines the maximal allowed difference between the internally filtered longitudinal relative velocity and the one provided by the tracker for a valid object.*/
   float32_T k_recw_max_allowed_rel_vel_lat_diff; /**<This defines the maximal allowed difference between the internally filtered lateral relative velocity and the one provided by the tracker for a valid object.*/
   float32_T k_recw_max_allowed_heading_diff; /**<This defines the maximal allowed difference between the internally filtered heading and the one provided by the tracker for a valid object.*/
   float32_T k_recw_rear_blockage_speed_threshold; /**<Only objects below this speed threshold are considered valid as blockers for the ego rear.*/
   float32_T k_recw_rear_blockage_width; /**<Width of blockage zone.*/
   float32_T k_recw_rear_blockage_length; /**<Length of blockage zone.*/
   float32_T k_recw_rear_blockage_ego_speed_threshold; /**<Ego speed threshold to release rear blockage. If object speed is above this value rear blockage cannot be active.*/
   float32_T k_recw_heading_accuracy_threshold; /**<Maximal allowed heading accuracy (greater values are less accurate)*/
   float32_T k_recw_min_speed_not_stationary; /**<Minimum speed an object needs to provide so that it is classified as non-stationary.*/
   float32_T k_recb_max_host_speed; /**<Maximal host speed for which RECB can raise a braking request.*/
   float32_T k_recb_max_host_speed_hys; /**<Hysteresis for maximal host speed for which RECB can raise a braking request. This value will be added to k_recb_max_host_speed if the host speed was in the allowed range before.*/
   float32_T k_recb_max_velocity_host_standstill; /**<Maximal velocity for which the host vehicle will be considered as standstill in the calculation of k_recb_min_cycles_host_standstill.*/
   float32_T k_recb_min_rel_velocity; /**<Minimal relative velocity of the relevant target which is required to raise a braking request.*/
   float32_T k_recb_min_rel_velocity_hys; /**<Hysteresis for the minimal relative velocity of an object to be able to raise a braking request. This value will be subtracted from k_recb_min_rel_velocity if a braking request was active before.*/
   float32_T k_recb_max_ttc; /**<The TTC of the relevant target is required to be below this threshold to raise a braking request.*/
   float32_T k_recb_nominal_acceleration_applied; /**<The acceleration which is applied to the brakes if a braking request is raised.*/
   float32_T k_recb_accelerator_pedal_gradient_threshold; /**<Threshold for the accelerator pedal gradient above which a braking request will be cancelled immediately.*/
   boolean_T k_recw_f_make_use_of_guardrail; /**<Switch to activate consideration of guardrail information in estimation of crash probability.*/
   boolean_T k_recw_f_only_allow_consecutive_alert_levels; /**<Flag to only allow consecutive alerts. If activated level 2 alerts can only follow a previous level 1 alert.*/
   boolean_T k_recw_f_allow_alert_on_coasted_objects[RECW_K_RECW_F_ALLOW_ALERT_ON_COASTED_OBJECTS_ARRAY_SIZE_DIM0]; /**<Flags define if coasted objects are allowed to raise an alert of level 1 and alert of level 2.*/
   boolean_T k_recw_f_use_rear_blockage[RECW_K_RECW_F_USE_REAR_BLOCKAGE_ARRAY_SIZE_DIM0]; /**<Flags define whether the rear end blockage algorithm should be used for alert level 1 and alert of level 2.*/
   boolean_T k_recw_f_apply_lane_filter; /**<If flag is true, targets outside the ego lane will be exluded from warnings.*/
   boolean_T k_recw_f_enable_heading_filter; /**<Switch to enable usage of the heading filter. If turned off the filtered heading will be replaced with the heading provided by the tracker.*/
   boolean_T k_recw_f_enable_traffic_light_ghost_detection; /**<Switch to enable the traffic light ghost detection.*/
   boolean_T k_recb_f_enable_recb; /**<Switch to enable the RECB functionality. If turned off RECB will be disabled.*/
   uint8_t k_recw_alert_holding_cycles[RECW_K_RECW_ALERT_HOLDING_CYCLES_ARRAY_SIZE_DIM0]; /**<Number of holding cycles for alerts of level 1 and alerts of level 2.*/
   uint8_t k_recw_alert_qualifying_cycles; /**<Number of qualifying cycles for alert level 1. Alert level 2 does not require qualifying.*/
   uint8_t k_recw_min_stage_age_for_alert_level[RECW_K_RECW_MIN_STAGE_AGE_FOR_ALERT_LEVEL_ARRAY_SIZE_DIM0]; /**<Minimal stage age to raise an alert of level 1 and alert of level 2. Coasted objects are also controlled via k_recw_max_allowed_consecutive_coasted_cycles and k_recw_f_allow_alert_on_coasted_objects.*/
   uint8_t k_recw_max_cycles_alert_duration[RECW_K_RECW_MAX_CYCLES_ALERT_DURATION_ARRAY_SIZE_DIM0]; /**<Defines the maximum number of cycles a warning can be active for alert level 1 and alert level 2 before it will be disabled. This is independent on the target that caused the warning.*/
   uint8_t k_recw_min_age_for_close_slow_targets; /**<Minimum age that a slow and close target must have to raise an alert*/
   uint8_t k_recw_min_object_age; /**<Minimal age of an object in cycles to be considered by RECW.*/
   uint8_t k_recw_min_cycles_with_min_crash_prob; /**<Number of consecutive cycles a crash probability of k_recw_min_crash_prob[0] for alert level 1 has to be reached before an object can raise an alert.*/
   uint8_t k_recw_en_active_car_wash_logic; /**<0 - car wash deactivated, 1 - car wash logic based on target state, 2 - car wash logic based on neutral gear position.*/
   uint8_t k_recw_lane_filter_num_consecutive_cycles; /**<Number of consecutive cycles required by target to be within lane in order to be able to trigger warning*/
   uint8_t k_recw_max_allowed_consecutive_coasted_cycles; /**<Maximal number of consecutive coasted cycles for which an object is still considered as valid for RECW.*/
   uint8_t k_recw_rear_blockage_qualifying_cycles; /**<Number of consecutive qualifying cycles with rear blockage before rear blockage is set for alert level calculation.*/
   uint8_t k_recb_min_cycles_host_standstill; /**<Minimal time for which the host has to be stationary before a braking request can be raised.*/
   uint8_t k_recb_ssm_braking; /**<The standstill management required during a braking request.*/
   uint8_t k_recb_ssm_request_cancelled; /**<The standstill management required if a braking request is cancelled.*/
   uint8_t k_recb_integrity; /**<The ASIL level required for the braking request.*/
   uint8_t k_recb_qualifier_nominal_acceleration; /**< The mode requested by Central Vehicle Management required for the braking request.*/
   uint8_t k_recb_max_cycles_braking_request_duration; /**<Defines the maximum number of cycles a braking request can be active before it will be cancelled. This is independent on the target that caused the braking request.*/
   uint8_t k_recw_f_suppress_alert_lvl_2_for_pedestrian; /**<Suppress alert level 2 for pedestrian object.*/
} Recw_Public_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* RECW_PUBLIC_CALIBRATION_T_H */
