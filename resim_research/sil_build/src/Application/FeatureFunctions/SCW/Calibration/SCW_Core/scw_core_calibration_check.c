/**
* @file scw_core_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in scw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "scw_core_calibration_check.h" // IWYU pragma: keep
#include "scw_core_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Scw_Core_Cal_In_Boundary(const Scw_Core_Calibration_T *p_calibration)
{
   boolean_T f_scw_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   {
    uint8_t x;
    for (x = 0u; x < SCW_K_SCW_CANDIDATE_HEADING_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CANDIDATE_HEADING, p_calibration->k_scw_candidate_heading[x], SCW_MAX_K_SCW_CANDIDATE_HEADING);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CANDIDATE_RELATIVE_VELOCITY, p_calibration->k_scw_candidate_relative_velocity[x], SCW_MAX_K_SCW_CANDIDATE_RELATIVE_VELOCITY);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CANDIDATE_VELOCITY, p_calibration->k_scw_candidate_velocity[x], SCW_MAX_K_SCW_CANDIDATE_VELOCITY);

        }
}
{
    uint8_t x;
    for (x = 0u; x < SCW_K_SCW_HYS_ZONE_X_OFFSET_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_HYS_ZONE_X_OFFSET, p_calibration->k_scw_hys_zone_x_offset[x], SCW_MAX_K_SCW_HYS_ZONE_X_OFFSET);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_HYS_ZONE_Y_OFFSET, p_calibration->k_scw_hys_zone_y_offset[x], SCW_MAX_K_SCW_HYS_ZONE_Y_OFFSET);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_INITIAL_ZONE_X, p_calibration->k_scw_initial_zone_x[x], SCW_MAX_K_SCW_INITIAL_ZONE_X);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_INITIAL_ZONE_Y, p_calibration->k_scw_initial_zone_y[x], SCW_MAX_K_SCW_INITIAL_ZONE_Y);

        }
}

   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CANDIDATE_HEADING_HYS, p_calibration->k_scw_candidate_heading_hys, SCW_MAX_K_SCW_CANDIDATE_HEADING_HYS);
Ct_Is_Uint8_In_Bondaries(&f_scw_calibration_in_boundaries, 0, p_calibration->k_scw_candidate_mature_cycles_in_zone_threshold, SCW_MAX_K_SCW_CANDIDATE_MATURE_CYCLES_IN_ZONE_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CANDIDATE_RELATIVE_VEL_HYS, p_calibration->k_scw_candidate_relative_vel_hys, SCW_MAX_K_SCW_CANDIDATE_RELATIVE_VEL_HYS);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CANDIDATE_VELOCITY_HYS, p_calibration->k_scw_candidate_velocity_hys, SCW_MAX_K_SCW_CANDIDATE_VELOCITY_HYS);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CANDIDATE_YAWRATE, p_calibration->k_scw_candidate_yawrate, SCW_MAX_K_SCW_CANDIDATE_YAWRATE);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CANDIDATE_YAWRATE_HYS, p_calibration->k_scw_candidate_yawrate_hys, SCW_MAX_K_SCW_CANDIDATE_YAWRATE_HYS);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CRITICAL_LAT_DISTANCE_HYS, p_calibration->k_scw_critical_lat_distance_hys, SCW_MAX_K_SCW_CRITICAL_LAT_DISTANCE_HYS);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_CRITICAL_LAT_TTC_HYS, p_calibration->k_scw_critical_lat_ttc_hys, SCW_MAX_K_SCW_CRITICAL_LAT_TTC_HYS);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_ADJUST_ZONES_TO_EGO_SIZE, p_calibration->k_scw_f_adjust_zones_to_ego_size, SCW_MAX_K_SCW_F_ADJUST_ZONES_TO_EGO_SIZE);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_DYNAMIC_ENABLE_VIA_CAL, p_calibration->k_scw_f_dynamic_enable_via_cal, SCW_MAX_K_SCW_F_DYNAMIC_ENABLE_VIA_CAL);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_ENABLE, p_calibration->k_scw_f_enable, SCW_MAX_K_SCW_F_ENABLE);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_ENABLE_DYNAMIC, p_calibration->k_scw_f_enable_dynamic, SCW_MAX_K_SCW_F_ENABLE_DYNAMIC);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_ENABLE_GUARDRAIL, p_calibration->k_scw_f_enable_guardrail, SCW_MAX_K_SCW_F_ENABLE_GUARDRAIL);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_ENABLE_TRAILER_TTC_EXTENSION, p_calibration->k_scw_f_enable_trailer_ttc_extension, SCW_MAX_K_SCW_F_ENABLE_TRAILER_TTC_EXTENSION);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_ENABLE_TRAILER_ZONE_EXTENSION, p_calibration->k_scw_f_enable_trailer_zone_extension, SCW_MAX_K_SCW_F_ENABLE_TRAILER_ZONE_EXTENSION);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_ENABLE_VIA_CAL, p_calibration->k_scw_f_enable_via_cal, SCW_MAX_K_SCW_F_ENABLE_VIA_CAL);
Ct_Is_Bool_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_F_GUARDRAIL_ENABLE_VIA_CAL, p_calibration->k_scw_f_guardrail_enable_via_cal, SCW_MAX_K_SCW_F_GUARDRAIL_ENABLE_VIA_CAL);
Ct_Is_Uint8_In_Bondaries(&f_scw_calibration_in_boundaries, 0, p_calibration->k_scw_guardrail_cycles_in_zone_threshold, SCW_MAX_K_SCW_GUARDRAIL_CYCLES_IN_ZONE_THRESHOLD);
Ct_Is_Uint8_In_Bondaries(&f_scw_calibration_in_boundaries, 0, p_calibration->k_scw_guardrail_freeze_period, SCW_MAX_K_SCW_GUARDRAIL_FREEZE_PERIOD);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_LATERAL_DISTANCE_DEFAULT, p_calibration->k_scw_lateral_distance_default, SCW_MAX_K_SCW_LATERAL_DISTANCE_DEFAULT);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_LATERAL_TTC_DEFAULT, p_calibration->k_scw_lateral_ttc_default, SCW_MAX_K_SCW_LATERAL_TTC_DEFAULT);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_LATERAL_TTC_MAX, p_calibration->k_scw_lateral_ttc_max, SCW_MAX_K_SCW_LATERAL_TTC_MAX);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MAX_DYNAMIC_LAT_DISTANCE, p_calibration->k_scw_max_dynamic_lat_distance, SCW_MAX_K_SCW_MAX_DYNAMIC_LAT_DISTANCE);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MAX_DYNAMIC_LAT_TTC, p_calibration->k_scw_max_dynamic_lat_ttc, SCW_MAX_K_SCW_MAX_DYNAMIC_LAT_TTC);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MAX_GUARDRAIL_LAT_DISTANCE, p_calibration->k_scw_max_guardrail_lat_distance, SCW_MAX_K_SCW_MAX_GUARDRAIL_LAT_DISTANCE);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MAX_GUARDRAIL_LAT_TTC, p_calibration->k_scw_max_guardrail_lat_ttc, SCW_MAX_K_SCW_MAX_GUARDRAIL_LAT_TTC);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MAX_LAT_POS_RATIO, p_calibration->k_scw_max_lat_pos_ratio, SCW_MAX_K_SCW_MAX_LAT_POS_RATIO);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MAX_ZONE_LENGTH, p_calibration->k_scw_max_zone_length, SCW_MAX_K_SCW_MAX_ZONE_LENGTH);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MAX_ZONE_WIDTH, p_calibration->k_scw_max_zone_width, SCW_MAX_K_SCW_MAX_ZONE_WIDTH);
Ct_Is_Uint8_In_Bondaries(&f_scw_calibration_in_boundaries, 0, p_calibration->k_scw_min_candidate_age, SCW_MAX_K_SCW_MIN_CANDIDATE_AGE);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MIN_CANDIDATE_EXISTENCE_PROBABILITY, p_calibration->k_scw_min_candidate_existence_probability, SCW_MAX_K_SCW_MIN_CANDIDATE_EXISTENCE_PROBABILITY);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MIN_DYNAMIC_LAT_DISTANCE, p_calibration->k_scw_min_dynamic_lat_distance, SCW_MAX_K_SCW_MIN_DYNAMIC_LAT_DISTANCE);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MIN_DYNAMIC_LAT_TTC, p_calibration->k_scw_min_dynamic_lat_ttc, SCW_MAX_K_SCW_MIN_DYNAMIC_LAT_TTC);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MIN_EXIST_PROB_RADAR_GUARDRAIL, p_calibration->k_scw_min_exist_prob_radar_guardrail, SCW_MAX_K_SCW_MIN_EXIST_PROB_RADAR_GUARDRAIL);
Ct_Is_Uint8_In_Bondaries(&f_scw_calibration_in_boundaries, 0, p_calibration->k_scw_min_guardrail_age, SCW_MAX_K_SCW_MIN_GUARDRAIL_AGE);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MIN_GUARDRAIL_LAT_DISTANCE, p_calibration->k_scw_min_guardrail_lat_distance, SCW_MAX_K_SCW_MIN_GUARDRAIL_LAT_DISTANCE);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MIN_GUARDRAIL_LAT_TTC, p_calibration->k_scw_min_guardrail_lat_ttc, SCW_MAX_K_SCW_MIN_GUARDRAIL_LAT_TTC);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MIN_HOST_SPEED, p_calibration->k_scw_min_host_speed, SCW_MAX_K_SCW_MIN_HOST_SPEED);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_MIN_HOST_SPEED_HYS, p_calibration->k_scw_min_host_speed_hys, SCW_MAX_K_SCW_MIN_HOST_SPEED_HYS);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_TRAILER_LAT_TTC_EXTENSION, p_calibration->k_scw_trailer_lat_ttc_extension, SCW_MAX_K_SCW_TRAILER_LAT_TTC_EXTENSION);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_TRAILER_ZONE_EXT_SAFETY_MARGIN, p_calibration->k_scw_trailer_zone_ext_safety_margin, SCW_MAX_K_SCW_TRAILER_ZONE_EXT_SAFETY_MARGIN);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_TRAILER_ZONE_EXT_SAFETY_MARGIN_LAT, p_calibration->k_scw_trailer_zone_ext_safety_margin_lat, SCW_MAX_K_SCW_TRAILER_ZONE_EXT_SAFETY_MARGIN_LAT);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_TTLE_DEFAULT, p_calibration->k_scw_ttle_default, SCW_MAX_K_SCW_TTLE_DEFAULT);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_TTLE_MAX, p_calibration->k_scw_ttle_max, SCW_MAX_K_SCW_TTLE_MAX);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_TTP_DEFAULT, p_calibration->k_scw_ttp_default, SCW_MAX_K_SCW_TTP_DEFAULT);
Ct_Is_Float_In_Bondaries(&f_scw_calibration_in_boundaries, SCW_MIN_K_SCW_TTP_MAX, p_calibration->k_scw_ttp_max, SCW_MAX_K_SCW_TTP_MAX);


   return f_scw_calibration_in_boundaries;
}

