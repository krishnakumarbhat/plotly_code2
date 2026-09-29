/**
* @file cta_core_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "cta_core_calibration_check.h" // IWYU pragma: keep
#include "cta_core_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Cta_Core_Cal_In_Boundary(const Cta_Core_Calibration_T *p_calibration)
{
   boolean_T f_cta_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   {
    uint8_t x;
    for (x = 0u; x < CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM0; x++)
    {
        uint8_t y;
        for (y = 0u; y < CTA_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL_ARRAY_SIZE_DIM1; y++)
        {
            Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL, p_calibration->k_cta_max_long_point_criticality_level[x][y], CTA_MAX_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL, p_calibration->k_cta_min_long_point_criticality_level[x][y], CTA_MAX_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_SPEED_CRITICALITY_LEVEL, p_calibration->k_cta_speed_criticality_level[x][y], CTA_MAX_K_CTA_SPEED_CRITICALITY_LEVEL);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_TTC_CRITICALITY_LEVEL, p_calibration->k_cta_ttc_criticality_level[x][y], CTA_MAX_K_CTA_TTC_CRITICALITY_LEVEL);

        }
    }
}

   {
    uint8_t x;
    for (x = 0u; x < CTA_K_CTA_ANGLES_ZONE_DEFINITION_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_ANGLES_ZONE_DEFINITION, p_calibration->k_cta_angles_zone_definition[x], CTA_MAX_K_CTA_ANGLES_ZONE_DEFINITION);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_enable_modes[x], CTA_MAX_K_CTA_ENABLE_MODES);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_HEADING_RANGE, p_calibration->k_cta_heading_range[x], CTA_MAX_K_CTA_HEADING_RANGE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_SENSOR_FOV_BORDER, p_calibration->k_cta_sensor_fov_border[x], CTA_MAX_K_CTA_SENSOR_FOV_BORDER);

        }
}
{
    uint8_t x;
    for (x = 0u; x < CTA_K_CTB_SAFETY_DIST_HOST_VEL_LUT_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_SAFETY_DIST_HOST_VEL_LUT, p_calibration->k_ctb_safety_dist_host_vel_lut[x], CTA_MAX_K_CTB_SAFETY_DIST_HOST_VEL_LUT);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_UPPER_SAFETY_DISTANCE_THRES_LUT, p_calibration->k_ctb_upper_safety_distance_thres_lut[x], CTA_MAX_K_CTB_UPPER_SAFETY_DISTANCE_THRES_LUT);

        }
}
{
    uint8_t x;
    for (x = 0u; x < CTA_K_CTA_FCTA_STEER_ANGLE_TABLE_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_FCTA_STEER_ANGLE_TABLE, p_calibration->k_cta_fcta_steer_angle_table[x], CTA_MAX_K_CTA_FCTA_STEER_ANGLE_TABLE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_FCTA_STEER_FACTOR_TABLE, p_calibration->k_cta_fcta_steer_factor_table[x], CTA_MAX_K_CTA_FCTA_STEER_FACTOR_TABLE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_RCTA_STEER_ANGLE_TABLE, p_calibration->k_cta_rcta_steer_angle_table[x], CTA_MAX_K_CTA_RCTA_STEER_ANGLE_TABLE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_RCTA_STEER_FACTOR_TABLE, p_calibration->k_cta_rcta_steer_factor_table[x], CTA_MAX_K_CTA_RCTA_STEER_FACTOR_TABLE);

        }
}
{
    uint8_t x;
    for (x = 0u; x < CTA_K_CTA_BUTTERFLY_LAT_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_BUTTERFLY_LAT, p_calibration->k_cta_butterfly_lat[x], CTA_MAX_K_CTA_BUTTERFLY_LAT);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_BUTTERFLY_LONG, p_calibration->k_cta_butterfly_long[x], CTA_MAX_K_CTA_BUTTERFLY_LONG);

        }
}

   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_2WHEEL_MIN_SIZE, p_calibration->k_cta_2wheel_min_size, CTA_MAX_K_CTA_2WHEEL_MIN_SIZE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_2WHEEL_MIN_SPEED, p_calibration->k_cta_2wheel_min_speed, CTA_MAX_K_CTA_2WHEEL_MIN_SPEED);
Ct_Is_Uint16_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_DEBUG_MODE, CTA_MAX_K_CTA_DEBUG_MODE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_ACCELERATIONPEDAL_GRADIENT_THRESHOLD_CTB, p_calibration->k_cta_accelerationpedal_gradient_threshold_ctb, CTA_MAX_K_CTA_ACCELERATIONPEDAL_GRADIENT_THRESHOLD_CTB);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_addit_mature_cycles_outside_sensor_fov, CTA_MAX_K_CTA_ADDIT_MATURE_CYCLES_OUTSIDE_SENSOR_FOV);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_age_for_new_creation_below_long_intersection, CTA_MAX_K_CTA_AGE_FOR_NEW_CREATION_BELOW_LONG_INTERSECTION);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_AMOUNT_BUTTERFLY_POINTS_IN_USE, p_calibration->k_cta_amount_butterfly_points_in_use, CTA_MAX_K_CTA_AMOUNT_BUTTERFLY_POINTS_IN_USE);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_cycle_count_hold_true_warning, CTA_MAX_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_cycle_count_suppress_true_warning, CTA_MAX_K_CTA_CYCLE_COUNT_SUPPRESS_TRUE_WARNING);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_cycles_valid_match_of_pot_ghost, CTA_MAX_K_CTA_CYCLES_VALID_MATCH_OF_POT_GHOST);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_DIST_THRES_CRIT_LEVEL_RESET, p_calibration->k_cta_dist_thres_crit_level_reset, CTA_MAX_K_CTA_DIST_THRES_CRIT_LEVEL_RESET);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_EGO_ABS_SPEED_MAX, p_calibration->k_cta_ego_abs_speed_max, CTA_MAX_K_CTA_EGO_ABS_SPEED_MAX);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_ENABLE_CTB, p_calibration->k_cta_enable_ctb, CTA_MAX_K_CTA_ENABLE_CTB);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_ADAPT_INTERSECT_LINES_BY_HOST_SPEED, p_calibration->k_cta_f_adapt_intersect_lines_by_host_speed, CTA_MAX_K_CTA_F_ADAPT_INTERSECT_LINES_BY_HOST_SPEED);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_ADAPT_INTERSECT_LINES_BY_OBJ_HEADING, p_calibration->k_cta_f_adapt_intersect_lines_by_obj_heading, CTA_MAX_K_CTA_F_ADAPT_INTERSECT_LINES_BY_OBJ_HEADING);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_ADAPT_INTERSECT_LINES_BY_STEERING_ANGLE, p_calibration->k_cta_f_adapt_intersect_lines_by_steering_angle, CTA_MAX_K_CTA_F_ADAPT_INTERSECT_LINES_BY_STEERING_ANGLE);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_APPLY_HEADING_COMPENSATION_ON_INTERSECTION_POINT, p_calibration->k_cta_f_apply_heading_compensation_on_intersection_point, CTA_MAX_K_CTA_F_APPLY_HEADING_COMPENSATION_ON_INTERSECTION_POINT);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_APPLY_PATH_TRACKING, p_calibration->k_cta_f_apply_path_tracking, CTA_MAX_K_CTA_F_APPLY_PATH_TRACKING);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_BRAKE_OVERRIDING_CTB, p_calibration->k_cta_f_brake_overriding_ctb, CTA_MAX_K_CTA_F_BRAKE_OVERRIDING_CTB);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_CALC_TTC_EGO_SIDE_ENABLED, p_calibration->k_cta_f_calc_ttc_ego_side_enabled, CTA_MAX_K_CTA_F_CALC_TTC_EGO_SIDE_ENABLED);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_CALC_TTP_EGO_SIDE_ENABLED, p_calibration->k_cta_f_calc_ttp_ego_side_enabled, CTA_MAX_K_CTA_F_CALC_TTP_EGO_SIDE_ENABLED);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_CHECK_OBSTRUCTION_PROBABILITY_SIGNAL, p_calibration->k_cta_f_check_obstruction_probability_signal, CTA_MAX_K_CTA_F_CHECK_OBSTRUCTION_PROBABILITY_SIGNAL);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_CHECK_REFLECTION_SIGNAL, p_calibration->k_cta_f_check_reflection_signal, CTA_MAX_K_CTA_F_CHECK_REFLECTION_SIGNAL);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_DISCARD_PT_HEADING_WHEN_MOVING, p_calibration->k_cta_f_discard_pt_heading_when_moving, CTA_MAX_K_CTA_F_DISCARD_PT_HEADING_WHEN_MOVING);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_ENABLE_HEADING_EXP_MOVING_AVERAGE, p_calibration->k_cta_f_enable_heading_exp_moving_average, CTA_MAX_K_CTA_F_ENABLE_HEADING_EXP_MOVING_AVERAGE);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_ENABLE_THRES_CRIT_LEVEL_RESET, p_calibration->k_cta_f_enable_thres_crit_level_reset, CTA_MAX_K_CTA_F_ENABLE_THRES_CRIT_LEVEL_RESET);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_PREVENT_FALL_BACK_TO_CRITLEVEL_1, p_calibration->k_cta_f_prevent_fall_back_to_critlevel_1, CTA_MAX_K_CTA_F_PREVENT_FALL_BACK_TO_CRITLEVEL_1);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_STOP_MODE_TTP, p_calibration->k_cta_f_stop_mode_ttp, CTA_MAX_K_CTA_F_STOP_MODE_TTP);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_BRAKE_GRADIENT, p_calibration->k_cta_f_use_brake_gradient, CTA_MAX_K_CTA_F_USE_BRAKE_GRADIENT);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_FRONT_CORNERS_DIST_STOP, p_calibration->k_cta_f_use_front_corners_dist_stop, CTA_MAX_K_CTA_F_USE_FRONT_CORNERS_DIST_STOP);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_GHOST_DETECTOR, p_calibration->k_cta_f_use_ghost_detector, CTA_MAX_K_CTA_F_USE_GHOST_DETECTOR);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_HEADING_FOR_RELATIVE_VELOCITY_CALCULATION, p_calibration->k_cta_f_use_heading_for_relative_velocity_calculation, CTA_MAX_K_CTA_F_USE_HEADING_FOR_RELATIVE_VELOCITY_CALCULATION);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_OBJECT_MIN_OBJECT_AGE_IN_CYCLES, p_calibration->k_cta_f_use_object_min_object_age_in_cycles, CTA_MAX_K_CTA_F_USE_OBJECT_MIN_OBJECT_AGE_IN_CYCLES);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_OBJECT_SUPRESS_COUNTER, p_calibration->k_cta_f_use_object_supress_counter, CTA_MAX_K_CTA_F_USE_OBJECT_SUPRESS_COUNTER);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_REL_VEL_ISECT_POINT_CALC, p_calibration->k_cta_f_use_rel_vel_isect_point_calc, CTA_MAX_K_CTA_F_USE_REL_VEL_ISECT_POINT_CALC);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_GHOST_CONDITION_MAX_HEADING_DIFF_PATH_TRACKER, p_calibration->k_cta_ghost_condition_max_heading_diff_path_tracker, CTA_MAX_K_CTA_GHOST_CONDITION_MAX_HEADING_DIFF_PATH_TRACKER);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_HOST_WIDTH_SENSOR_FOV_SUPPR_FACTOR, p_calibration->k_cta_host_width_sensor_fov_suppr_factor, CTA_MAX_K_CTA_HOST_WIDTH_SENSOR_FOV_SUPPR_FACTOR);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_INTERSECTION_LINE_HOST_WIDTH_PERCENTAGE, p_calibration->k_cta_intersection_line_host_width_percentage, CTA_MAX_K_CTA_INTERSECTION_LINE_HOST_WIDTH_PERCENTAGE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_DECELERATION_VALUE, p_calibration->k_cta_max_deceleration_value, CTA_MAX_K_CTA_MAX_DECELERATION_VALUE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_HEADING_VARIANCE, p_calibration->k_cta_max_heading_variance, CTA_MAX_K_CTA_MAX_HEADING_VARIANCE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_LENGTH_FOV, p_calibration->k_cta_max_length_fov, CTA_MAX_K_CTA_MAX_LENGTH_FOV);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_OBJECT_ECLIPSE_FOR_LEVEL_QUALIFICATION, p_calibration->k_cta_max_object_eclipse_for_level_qualification, CTA_MAX_K_CTA_MAX_OBJECT_ECLIPSE_FOR_LEVEL_QUALIFICATION);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_OBSTRUCTION_PROBABILITY, p_calibration->k_cta_max_obstruction_probability, CTA_MAX_K_CTA_MAX_OBSTRUCTION_PROBABILITY);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_SEG_HEADING_DIFF_NO_GHOST, p_calibration->k_cta_max_seg_heading_diff_no_ghost, CTA_MAX_K_CTA_MAX_SEG_HEADING_DIFF_NO_GHOST);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_SPEED, p_calibration->k_cta_max_speed, CTA_MAX_K_CTA_MAX_SPEED);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_min_age_obj_outside_sensor_fov, CTA_MAX_K_CTA_MIN_AGE_OBJ_OUTSIDE_SENSOR_FOV);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_DECELERATION_VALUE, p_calibration->k_cta_min_deceleration_value, CTA_MAX_K_CTA_MIN_DECELERATION_VALUE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_HOST_SPEED_TO_DISCARD_PT_INFO, p_calibration->k_cta_min_host_speed_to_discard_pt_info, CTA_MAX_K_CTA_MIN_HOST_SPEED_TO_DISCARD_PT_INFO);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_LATERAL_APPROACH_SPEED, p_calibration->k_cta_min_lateral_approach_speed, CTA_MAX_K_CTA_MIN_LATERAL_APPROACH_SPEED);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_PARK_ANGLE, p_calibration->k_cta_min_park_angle, CTA_MAX_K_CTA_MIN_PARK_ANGLE);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_min_qual_age_obj_crossing_paths, CTA_MAX_K_CTA_MIN_QUAL_AGE_OBJ_CROSSING_PATHS);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_REL_EXISTENCE_PROBABILITY, p_calibration->k_cta_min_rel_existence_probability, CTA_MAX_K_CTA_MIN_REL_EXISTENCE_PROBABILITY);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_SPEED, p_calibration->k_cta_min_speed, CTA_MAX_K_CTA_MIN_SPEED);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION, p_calibration->k_cta_min_ttc_additional_mature_qualification, CTA_MAX_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_OBJ_DIST_TO_DISCARD_PT_INFO, p_calibration->k_cta_obj_dist_to_discard_pt_info, CTA_MAX_K_CTA_OBJ_DIST_TO_DISCARD_PT_INFO);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_OBJECT_HEADING_EXP_MOVING_AVERAGE_ALPHA, p_calibration->k_cta_object_heading_exp_moving_average_alpha, CTA_MAX_K_CTA_OBJECT_HEADING_EXP_MOVING_AVERAGE_ALPHA);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_PEDESTRIAN_MIN_SIZE, p_calibration->k_cta_pedestrian_min_size, CTA_MAX_K_CTA_PEDESTRIAN_MIN_SIZE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_PEDESTRIAN_MIN_SPEED, p_calibration->k_cta_pedestrian_min_speed, CTA_MAX_K_CTA_PEDESTRIAN_MIN_SPEED);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF, p_calibration->k_cta_range_to_path_segment_ghost_qualif, CTA_MAX_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_RCTA_HOST_SPEED_FACTOR, p_calibration->k_cta_rcta_host_speed_factor, CTA_MAX_K_CTA_RCTA_HOST_SPEED_FACTOR);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_REL_WARNING_HYSTERESIS, p_calibration->k_cta_rel_warning_hysteresis, CTA_MAX_K_CTA_REL_WARNING_HYSTERESIS);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_SPEED_THRESH_FOR_REL_VEL_CALC, p_calibration->k_cta_speed_thresh_for_rel_vel_calc, CTA_MAX_K_CTA_SPEED_THRESH_FOR_REL_VEL_CALC);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_STOP_ALERT_TTC, p_calibration->k_cta_stop_alert_ttc, CTA_MAX_K_CTA_STOP_ALERT_TTC);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_STOP_ALERT_TTP, p_calibration->k_cta_stop_alert_ttp, CTA_MAX_K_CTA_STOP_ALERT_TTP);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_SWITCH, p_calibration->k_cta_switch, CTA_MAX_K_CTA_SWITCH);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_TTC_CALC_POSITIVE_REF_POINT, p_calibration->k_cta_ttc_calc_positive_ref_point, CTA_MAX_K_CTA_TTC_CALC_POSITIVE_REF_POINT);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_TTC_WARNTRIGGER_EARLY, p_calibration->k_cta_ttc_warntrigger_early, CTA_MAX_K_CTA_TTC_WARNTRIGGER_EARLY);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_TTC_WARNTRIGGER_LATE, p_calibration->k_cta_ttc_warntrigger_late, CTA_MAX_K_CTA_TTC_WARNTRIGGER_LATE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_BRAKING_JERK, p_calibration->k_ctb_braking_jerk, CTA_MAX_K_CTB_BRAKING_JERK);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_CONST_DECEL_AFTER_RAMP_IN, p_calibration->k_ctb_const_decel_after_ramp_in, CTA_MAX_K_CTB_CONST_DECEL_AFTER_RAMP_IN);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_EVENT_TIME_BUFFER, p_calibration->k_ctb_event_time_buffer, CTA_MAX_K_CTB_EVENT_TIME_BUFFER);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_HOST_ACC_WEIGHT, p_calibration->k_ctb_host_acc_weight, CTA_MAX_K_CTB_HOST_ACC_WEIGHT);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_LOWER_SAFETY_DISTANCE_THRES, p_calibration->k_ctb_lower_safety_distance_thres, CTA_MAX_K_CTB_LOWER_SAFETY_DISTANCE_THRES);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_MAX_BRAKING_TIME, p_calibration->k_ctb_max_braking_time, CTA_MAX_K_CTB_MAX_BRAKING_TIME);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_ctb_min_brake_hold_ctr_thres, CTA_MAX_K_CTB_MIN_BRAKE_HOLD_CTR_THRES);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_ctb_min_brake_qual_ctr_thres, CTA_MAX_K_CTB_MIN_BRAKE_QUAL_CTR_THRES);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_MIN_BRAKING_TIME, p_calibration->k_ctb_min_braking_time, CTA_MAX_K_CTB_MIN_BRAKING_TIME);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_RAMP_IN_TIME, p_calibration->k_ctb_ramp_in_time, CTA_MAX_K_CTB_RAMP_IN_TIME);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_RESPONSETIME_BRAKE_ACTUATION, p_calibration->k_ctb_responsetime_brake_actuation, CTA_MAX_K_CTB_RESPONSETIME_BRAKE_ACTUATION);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_TIME_TO_ASK_FOR_FINAL_BRAKE_DECEL, p_calibration->k_ctb_time_to_ask_for_final_brake_decel, CTA_MAX_K_CTB_TIME_TO_ASK_FOR_FINAL_BRAKE_DECEL);


   return f_cta_calibration_in_boundaries;
}

