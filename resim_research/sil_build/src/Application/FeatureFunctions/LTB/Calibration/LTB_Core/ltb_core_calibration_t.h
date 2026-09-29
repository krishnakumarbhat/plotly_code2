# ifndef LTB_CORE_CALIBRATION_T_H
# define LTB_CORE_CALIBRATION_T_H

/**
* @file ltb_core_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in ltb_cal.xml.
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

/* Macros for dimension size for all array variables */


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_CORE_CALIBRATION_SIZE (124u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_ltb_alert_holding_cycles; /**<Number of cycle for holding an alert level.*/
   uint8_t k_ltb_alert_qualifying_cycles; /**<Number of cycle to qualify an alert level.*/
   uint8_t k_ltb_prediction_steps_max; /**<Maximum number of steps to predict trajectories of the ego and the relevant objects.*/
   uint8_t k_ltb_ego_pred_const_velocity_pred_steps_min; /**<This value indicates at which prediction step to switch to a consltbnt velocity prediction model when LTB alert level 4 was active in previous cycle.*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   boolean_T k_f_ltb_enable_brake_gradient_logic; /**<LTB considers the brake gradient or jerk if this flag is enabled. It uses the value from k_ltb_brake_gradient.*/
   boolean_T k_ltb_f_skip_holding_for_single_alert_level_drop; /**<Flag to skip alert level holding if the drop is only to one level lower. Only affects drop from level 3.*/
   boolean_T k_ltb_f_only_allow_consecutive_ttc_based_alert_levels; /**<Flag to only allow consecutive alerts. Applies to alert level 3.*/
   float32_T k_ltb_bmw_sp25_v_ego_max_hys; /**<The speed hystereses for ltb activation or deactivation.*/
   float32_T k_ltb_bmw_sp25_v_ego_max; /**<The max host speed below which ltb would be activated.*/
   float32_T k_ltb_object_long_vel_min; /**<The minimum longitudinal target object velocity.*/
   float32_T k_ltb_brake_gradient; /**<LTB assumes that the vehicle deceleration will ramp up with the specified gradient or jerk. Only active if k_f_ltb_enable_brake_gradient_logic is enabled.*/
   float32_T k_ltb_brake_dead_time; /**<LTB assumes that the vehicle will start deceleration after the specified dead time of the braking system.*/
   float32_T k_ltb_brake_deceleration_max; /**<LTB caps the maximum braking deceleration to this value.*/
   float32_T k_ltb_alert_lvl_3_decel_threshold; /**<LTB raises an alert level 3 for an object with an estimated host deceleration to avoid a collision greater than or equal to the specified value.*/
   float32_T k_ltb_alert_lvl_3_ttc_threshold; /**<LTB raises an alert level 3 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ltb_alert_lvl_2_ttb_threshold; /**<LTB raises an alert level 3 only for an object with a calculated TTB (time-to-brake) smaller than or equal to the specified value.*/
   float32_T k_ltb_alert_lvl_2_ttc_threshold; /**<LTB raises an alert level 2 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ltb_alert_lvl_1_ttc_threshold; /**<LTB raises an alert level 1 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ltb_critical_approach_angle_diff_min; /**<LTB considers an approach of an object to the ego only as critical above this angle difference.*/
   float32_T k_ltb_critical_approach_min_safe_distance; /**<LTB considers an approach of an object to the ego with a distance smaller than the specified value as a collision.*/
   float32_T k_ltb_obj_shape_gain_per_pred_step; /**<LTB grows or shrinks the ego shape by multiplying the specified value with the ego dimensions at every prediction step. This means, the more in the future a prediction step is, the more the shape will be grown or shrunk. This is in addition to the fixed gain specified by k_obj_shape_gain_fixed. Value greater than 1: object shape will grow, value lower than 1: object shape will shrink*/
   float32_T k_ltb_obj_shape_gain_fixed; /**<LTB grows or shrinks an object's shape by multiplying the specified value with the object's dimensions once at the beginning of the prediction phase. Value greater than 1: object shape will be increased, value lower than 1: object shape will be decreased*/
   float32_T k_ltb_obj_pred_speed_min; /**<LTB only continues object trajectory calculation if the predicted waypoint speed is equal or above this threshold.*/
   float32_T k_ltb_ego_shape_gain_per_pred_step; /**<LTB grows or shrinks an object's shape by multiplying the specified value with the object's dimensions at every prediction step. This means, the more in the future a prediction step is, the more the shape will be grown or shrunk. This is in addition to the fixed gain specified by k_ego_shape_gain_fixed. Value greater than 1: ego shape will grow, value lower than 1: ego shape will shrink*/
   float32_T k_ltb_ego_yawangle_integration_yawrate_min; /**<This value indicates the minimum (absolute) yawrate value to start integrating the ego yawangle. Below this threshold the yawangle resets.*/
   float32_T k_ltb_ego_max_pred_yaw_angle; /**<LTB only continues the ego trajectory calculation if the (absolute) predicted yaw angle to the last straight section is below this threshold.*/
   float32_T k_ltb_ego_deceleration_weight; /**<This weight factor controls the influence of the negative acceleration / deceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ltb_ego_circle_host_length_factor; /**<This weight factor controls the influence of the host vehicle length when calculating the host vehicle circle offsets.*/
   float32_T k_ltb_ego_circle_offset; /**<This weight factor adds an additional offset to the host circles.*/
   float32_T k_ltb_ego_shape_gain_fixed; /**<LTB grows or shrinks the ego shape by multiplying the specified value with the ego dimensions once at the beginning of the prediction phase. Value greater than 1: ego shape will be increased, value lower than 1: ego shape will be decreased*/
   float32_T k_ltb_ego_acceleration_weight; /**<This weight factor controls the influence of the positive acceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ltb_zone_width; /**<Sets the width of the zone.*/
   float32_T k_ltb_zone_length; /**<Sets the length of the zone.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Ltb_Core_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_ltb_zone_length; /**<Sets the length of the zone.*/
   float32_T k_ltb_zone_width; /**<Sets the width of the zone.*/
   float32_T k_ltb_ego_acceleration_weight; /**<This weight factor controls the influence of the positive acceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ltb_ego_shape_gain_fixed; /**<LTB grows or shrinks the ego shape by multiplying the specified value with the ego dimensions once at the beginning of the prediction phase. Value greater than 1: ego shape will be increased, value lower than 1: ego shape will be decreased*/
   float32_T k_ltb_ego_circle_offset; /**<This weight factor adds an additional offset to the host circles.*/
   float32_T k_ltb_ego_circle_host_length_factor; /**<This weight factor controls the influence of the host vehicle length when calculating the host vehicle circle offsets.*/
   float32_T k_ltb_ego_deceleration_weight; /**<This weight factor controls the influence of the negative acceleration / deceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ltb_ego_max_pred_yaw_angle; /**<LTB only continues the ego trajectory calculation if the (absolute) predicted yaw angle to the last straight section is below this threshold.*/
   float32_T k_ltb_ego_yawangle_integration_yawrate_min; /**<This value indicates the minimum (absolute) yawrate value to start integrating the ego yawangle. Below this threshold the yawangle resets.*/
   float32_T k_ltb_ego_shape_gain_per_pred_step; /**<LTB grows or shrinks an object's shape by multiplying the specified value with the object's dimensions at every prediction step. This means, the more in the future a prediction step is, the more the shape will be grown or shrunk. This is in addition to the fixed gain specified by k_ego_shape_gain_fixed. Value greater than 1: ego shape will grow, value lower than 1: ego shape will shrink*/
   float32_T k_ltb_obj_pred_speed_min; /**<LTB only continues object trajectory calculation if the predicted waypoint speed is equal or above this threshold.*/
   float32_T k_ltb_obj_shape_gain_fixed; /**<LTB grows or shrinks an object's shape by multiplying the specified value with the object's dimensions once at the beginning of the prediction phase. Value greater than 1: object shape will be increased, value lower than 1: object shape will be decreased*/
   float32_T k_ltb_obj_shape_gain_per_pred_step; /**<LTB grows or shrinks the ego shape by multiplying the specified value with the ego dimensions at every prediction step. This means, the more in the future a prediction step is, the more the shape will be grown or shrunk. This is in addition to the fixed gain specified by k_obj_shape_gain_fixed. Value greater than 1: object shape will grow, value lower than 1: object shape will shrink*/
   float32_T k_ltb_critical_approach_min_safe_distance; /**<LTB considers an approach of an object to the ego with a distance smaller than the specified value as a collision.*/
   float32_T k_ltb_critical_approach_angle_diff_min; /**<LTB considers an approach of an object to the ego only as critical above this angle difference.*/
   float32_T k_ltb_alert_lvl_1_ttc_threshold; /**<LTB raises an alert level 1 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ltb_alert_lvl_2_ttc_threshold; /**<LTB raises an alert level 2 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ltb_alert_lvl_2_ttb_threshold; /**<LTB raises an alert level 3 only for an object with a calculated TTB (time-to-brake) smaller than or equal to the specified value.*/
   float32_T k_ltb_alert_lvl_3_ttc_threshold; /**<LTB raises an alert level 3 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ltb_alert_lvl_3_decel_threshold; /**<LTB raises an alert level 3 for an object with an estimated host deceleration to avoid a collision greater than or equal to the specified value.*/
   float32_T k_ltb_brake_deceleration_max; /**<LTB caps the maximum braking deceleration to this value.*/
   float32_T k_ltb_brake_dead_time; /**<LTB assumes that the vehicle will start deceleration after the specified dead time of the braking system.*/
   float32_T k_ltb_brake_gradient; /**<LTB assumes that the vehicle deceleration will ramp up with the specified gradient or jerk. Only active if k_f_ltb_enable_brake_gradient_logic is enabled.*/
   float32_T k_ltb_object_long_vel_min; /**<The minimum longitudinal target object velocity.*/
   float32_T k_ltb_bmw_sp25_v_ego_max; /**<The max host speed below which ltb would be activated.*/
   float32_T k_ltb_bmw_sp25_v_ego_max_hys; /**<The speed hystereses for ltb activation or deactivation.*/
   boolean_T k_ltb_f_only_allow_consecutive_ttc_based_alert_levels; /**<Flag to only allow consecutive alerts. Applies to alert level 3.*/
   boolean_T k_ltb_f_skip_holding_for_single_alert_level_drop; /**<Flag to skip alert level holding if the drop is only to one level lower. Only affects drop from level 3.*/
   boolean_T k_f_ltb_enable_brake_gradient_logic; /**<LTB considers the brake gradient or jerk if this flag is enabled. It uses the value from k_ltb_brake_gradient.*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   uint8_t k_ltb_ego_pred_const_velocity_pred_steps_min; /**<This value indicates at which prediction step to switch to a consltbnt velocity prediction model when LTB alert level 4 was active in previous cycle.*/
   uint8_t k_ltb_prediction_steps_max; /**<Maximum number of steps to predict trajectories of the ego and the relevant objects.*/
   uint8_t k_ltb_alert_qualifying_cycles; /**<Number of cycle to qualify an alert level.*/
   uint8_t k_ltb_alert_holding_cycles; /**<Number of cycle for holding an alert level.*/
} Ltb_Core_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* LTB_CORE_CALIBRATION_T_H */
