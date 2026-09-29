/**
* @file cta_public_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "cta_public_calibration_check.h" // IWYU pragma: keep
#include "cta_public_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Cta_Public_Cal_In_Boundary(const Cta_Public_Calibration_T *p_calibration)
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
    for (x = 0u; x < CTA_K_CTA_BUTTERFLY_LAT_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_BUTTERFLY_LAT, p_calibration->k_cta_butterfly_lat[x], CTA_MAX_K_CTA_BUTTERFLY_LAT);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_BUTTERFLY_LONG, p_calibration->k_cta_butterfly_long[x], CTA_MAX_K_CTA_BUTTERFLY_LONG);

        }
}

   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_BMW_SP25_BANNER_CRITERIA_CHECK, p_calibration->k_bmw_sp25_banner_criteria_check, CTA_MAX_K_BMW_SP25_BANNER_CRITERIA_CHECK);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_BMW_SP25_BANNER_CRITERIA_CHECK_TIME, p_calibration->k_bmw_sp25_banner_criteria_check_time, CTA_MAX_K_BMW_SP25_BANNER_CRITERIA_CHECK_TIME);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_BMW_SP25_BANNER_TIME, p_calibration->k_bmw_sp25_banner_time, CTA_MAX_K_BMW_SP25_BANNER_TIME);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_BMW_SP25_EGO_ABS_SPEED_MAX_HYS, p_calibration->k_bmw_sp25_ego_abs_speed_max_hys, CTA_MAX_K_BMW_SP25_EGO_ABS_SPEED_MAX_HYS);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_2WHEEL_MIN_SIZE, p_calibration->k_cta_2wheel_min_size, CTA_MAX_K_CTA_2WHEEL_MIN_SIZE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_2WHEEL_MIN_SPEED, p_calibration->k_cta_2wheel_min_speed, CTA_MAX_K_CTA_2WHEEL_MIN_SPEED);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_cycle_count_hold_true_warning, CTA_MAX_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_cycle_count_suppress_true_warning, CTA_MAX_K_CTA_CYCLE_COUNT_SUPPRESS_TRUE_WARNING);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_EGO_ABS_SPEED_MAX, p_calibration->k_cta_ego_abs_speed_max, CTA_MAX_K_CTA_EGO_ABS_SPEED_MAX);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_ADAPT_INTERSECT_LINES_BY_OBJ_HEADING, p_calibration->k_cta_f_adapt_intersect_lines_by_obj_heading, CTA_MAX_K_CTA_F_ADAPT_INTERSECT_LINES_BY_OBJ_HEADING);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_ADAPT_INTERSECT_LINES_BY_STEERING_ANGLE, p_calibration->k_cta_f_adapt_intersect_lines_by_steering_angle, CTA_MAX_K_CTA_F_ADAPT_INTERSECT_LINES_BY_STEERING_ANGLE);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_APPLY_HEADING_COMPENSATION_ON_INTERSECTION_POINT, p_calibration->k_cta_f_apply_heading_compensation_on_intersection_point, CTA_MAX_K_CTA_F_APPLY_HEADING_COMPENSATION_ON_INTERSECTION_POINT);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_CALC_TTC_EGO_SIDE_ENABLED, p_calibration->k_cta_f_calc_ttc_ego_side_enabled, CTA_MAX_K_CTA_F_CALC_TTC_EGO_SIDE_ENABLED);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_ENABLE_THRES_CRIT_LEVEL_RESET, p_calibration->k_cta_f_enable_thres_crit_level_reset, CTA_MAX_K_CTA_F_ENABLE_THRES_CRIT_LEVEL_RESET);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_STOP_MODE_TTP, p_calibration->k_cta_f_stop_mode_ttp, CTA_MAX_K_CTA_F_STOP_MODE_TTP);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_FRONT_CORNERS_DIST_STOP, p_calibration->k_cta_f_use_front_corners_dist_stop, CTA_MAX_K_CTA_F_USE_FRONT_CORNERS_DIST_STOP);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_GHOST_DETECTOR, p_calibration->k_cta_f_use_ghost_detector, CTA_MAX_K_CTA_F_USE_GHOST_DETECTOR);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_HEADING_FOR_RELATIVE_VELOCITY_CALCULATION, p_calibration->k_cta_f_use_heading_for_relative_velocity_calculation, CTA_MAX_K_CTA_F_USE_HEADING_FOR_RELATIVE_VELOCITY_CALCULATION);
Ct_Is_Bool_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_F_USE_OBJECT_MIN_OBJECT_AGE_IN_CYCLES, p_calibration->k_cta_f_use_object_min_object_age_in_cycles, CTA_MAX_K_CTA_F_USE_OBJECT_MIN_OBJECT_AGE_IN_CYCLES);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_INTERSECTION_LINE_HOST_WIDTH_PERCENTAGE, p_calibration->k_cta_intersection_line_host_width_percentage, CTA_MAX_K_CTA_INTERSECTION_LINE_HOST_WIDTH_PERCENTAGE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_DECELERATION_VALUE, p_calibration->k_cta_max_deceleration_value, CTA_MAX_K_CTA_MAX_DECELERATION_VALUE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_HEADING_VARIANCE, p_calibration->k_cta_max_heading_variance, CTA_MAX_K_CTA_MAX_HEADING_VARIANCE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_LENGTH_FOV, p_calibration->k_cta_max_length_fov, CTA_MAX_K_CTA_MAX_LENGTH_FOV);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_OBJECT_ECLIPSE_FOR_LEVEL_QUALIFICATION, p_calibration->k_cta_max_object_eclipse_for_level_qualification, CTA_MAX_K_CTA_MAX_OBJECT_ECLIPSE_FOR_LEVEL_QUALIFICATION);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MAX_SPEED, p_calibration->k_cta_max_speed, CTA_MAX_K_CTA_MAX_SPEED);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_min_age_obj_outside_sensor_fov, CTA_MAX_K_CTA_MIN_AGE_OBJ_OUTSIDE_SENSOR_FOV);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_DECELERATION_VALUE, p_calibration->k_cta_min_deceleration_value, CTA_MAX_K_CTA_MIN_DECELERATION_VALUE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_LATERAL_APPROACH_SPEED, p_calibration->k_cta_min_lateral_approach_speed, CTA_MAX_K_CTA_MIN_LATERAL_APPROACH_SPEED);
Ct_Is_Uint8_In_Bondaries(&f_cta_calibration_in_boundaries, 0, p_calibration->k_cta_min_qual_age_obj_crossing_paths, CTA_MAX_K_CTA_MIN_QUAL_AGE_OBJ_CROSSING_PATHS);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_SPEED, p_calibration->k_cta_min_speed, CTA_MAX_K_CTA_MIN_SPEED);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION, p_calibration->k_cta_min_ttc_additional_mature_qualification, CTA_MAX_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_PEDESTRIAN_MIN_SIZE, p_calibration->k_cta_pedestrian_min_size, CTA_MAX_K_CTA_PEDESTRIAN_MIN_SIZE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_PEDESTRIAN_MIN_SPEED, p_calibration->k_cta_pedestrian_min_speed, CTA_MAX_K_CTA_PEDESTRIAN_MIN_SPEED);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF, p_calibration->k_cta_range_to_path_segment_ghost_qualif, CTA_MAX_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_STOP_ALERT_TTC, p_calibration->k_cta_stop_alert_ttc, CTA_MAX_K_CTA_STOP_ALERT_TTC);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTA_STOP_ALERT_TTP, p_calibration->k_cta_stop_alert_ttp, CTA_MAX_K_CTA_STOP_ALERT_TTP);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_CTB_MIN_TTP, p_calibration->k_ctb_min_ttp, CTA_MAX_K_CTB_MIN_TTP);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_STLA_CRIT_ZONE_D_C_LINE, p_calibration->k_stla_crit_zone_D_C_line, CTA_MAX_K_STLA_CRIT_ZONE_D_C_LINE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_STLA_CRIT_ZONE_G_E_LINE, p_calibration->k_stla_crit_zone_G_E_line, CTA_MAX_K_STLA_CRIT_ZONE_G_E_LINE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_STLA_CRIT_ZONE_N_Q_LINE, p_calibration->k_stla_crit_zone_N_Q_line, CTA_MAX_K_STLA_CRIT_ZONE_N_Q_LINE);
Ct_Is_Float_In_Bondaries(&f_cta_calibration_in_boundaries, CTA_MIN_K_STLA_CRIT_ZONE_Q_QH_LINE, p_calibration->k_stla_crit_zone_Q_QH_line, CTA_MAX_K_STLA_CRIT_ZONE_Q_QH_LINE);


   return f_cta_calibration_in_boundaries;
}

