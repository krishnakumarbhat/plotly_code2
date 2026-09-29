/**
* @file ta_core_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in ta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "ta_core_calibration_check.h" // IWYU pragma: keep
#include "ta_core_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Ta_Core_Cal_In_Boundary(const Ta_Core_Calibration_T *p_calibration)
{
   boolean_T f_ta_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   {
    uint8_t x;
    for (x = 0u; x < TA_K_FTA_EGO_OBJ_HEADING_DIFF_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF, p_calibration->k_fta_ego_obj_heading_diff[x], TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF_OFST, p_calibration->k_fta_ego_obj_heading_diff_ofst[x], TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_AREA, p_calibration->k_fta_obj_area[x], TA_MAX_K_FTA_OBJ_AREA);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_AREA_OFST, p_calibration->k_fta_obj_area_ofst[x], TA_MAX_K_FTA_OBJ_AREA_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE, p_calibration->k_fta_obj_eclipse_value[x], TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE_OFST, p_calibration->k_fta_obj_eclipse_value_ofst[x], TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_EXIST_PRBLTY, p_calibration->k_fta_obj_exist_prblty[x], TA_MAX_K_FTA_OBJ_EXIST_PRBLTY);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_EXIST_PRBLTY_OFST, p_calibration->k_fta_obj_exist_prblty_ofst[x], TA_MAX_K_FTA_OBJ_EXIST_PRBLTY_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_HEADING, p_calibration->k_fta_obj_heading[x], TA_MAX_K_FTA_OBJ_HEADING);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_HEADING_OFST, p_calibration->k_fta_obj_heading_ofst[x], TA_MAX_K_FTA_OBJ_HEADING_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_HEADING_RATE, p_calibration->k_fta_obj_heading_rate[x], TA_MAX_K_FTA_OBJ_HEADING_RATE);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_HEADING_RATE_OFST, p_calibration->k_fta_obj_heading_rate_ofst[x], TA_MAX_K_FTA_OBJ_HEADING_RATE_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_HEADING_RATE_STRAIGHT, p_calibration->k_fta_obj_heading_rate_straight[x], TA_MAX_K_FTA_OBJ_HEADING_RATE_STRAIGHT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_HEADING_STRAIGHT, p_calibration->k_fta_obj_heading_straight[x], TA_MAX_K_FTA_OBJ_HEADING_STRAIGHT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_LENGTH, p_calibration->k_fta_obj_length[x], TA_MAX_K_FTA_OBJ_LENGTH);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_LENGTH_OFST, p_calibration->k_fta_obj_length_ofst[x], TA_MAX_K_FTA_OBJ_LENGTH_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_SPEED, p_calibration->k_fta_obj_speed[x], TA_MAX_K_FTA_OBJ_SPEED);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_SPEED_OFST, p_calibration->k_fta_obj_speed_ofst[x], TA_MAX_K_FTA_OBJ_SPEED_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_SPEED_STRAIGHT, p_calibration->k_fta_obj_speed_straight[x], TA_MAX_K_FTA_OBJ_SPEED_STRAIGHT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LAT_VEL, p_calibration->k_fta_obj_vcs_lat_vel[x], TA_MAX_K_FTA_OBJ_VCS_LAT_VEL);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_OFST, p_calibration->k_fta_obj_vcs_lat_vel_ofst[x], TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL, p_calibration->k_fta_obj_vcs_lat_vel_rel[x], TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST, p_calibration->k_fta_obj_vcs_lat_vel_rel_ofst[x], TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LONG_VEL, p_calibration->k_fta_obj_vcs_long_vel[x], TA_MAX_K_FTA_OBJ_VCS_LONG_VEL);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_OFST, p_calibration->k_fta_obj_vcs_long_vel_ofst[x], TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL, p_calibration->k_fta_obj_vcs_long_vel_rel[x], TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST, p_calibration->k_fta_obj_vcs_long_vel_rel_ofst[x], TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB, p_calibration->k_fta_obj_vru_class_prob[x], TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB_OFST, p_calibration->k_fta_obj_vru_class_prob_ofst[x], TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_WIDTH, p_calibration->k_fta_obj_width[x], TA_MAX_K_FTA_OBJ_WIDTH);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_WIDTH_OFST, p_calibration->k_fta_obj_width_ofst[x], TA_MAX_K_FTA_OBJ_WIDTH_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_PFGS_EGO_SPEED, p_calibration->k_pfgs_ego_speed[x], TA_MAX_K_PFGS_EGO_SPEED);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE, p_calibration->k_rta_obj_eclipse_value[x], TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE_OFST, p_calibration->k_rta_obj_eclipse_value_ofst[x], TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_EXIST_PRBLTY, p_calibration->k_rta_obj_exist_prblty[x], TA_MAX_K_RTA_OBJ_EXIST_PRBLTY);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_EXIST_PRBLTY_OFST, p_calibration->k_rta_obj_exist_prblty_ofst[x], TA_MAX_K_RTA_OBJ_EXIST_PRBLTY_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_HEADING, p_calibration->k_rta_obj_heading[x], TA_MAX_K_RTA_OBJ_HEADING);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_HEADING_OFST, p_calibration->k_rta_obj_heading_ofst[x], TA_MAX_K_RTA_OBJ_HEADING_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_LENGTH, p_calibration->k_rta_obj_length[x], TA_MAX_K_RTA_OBJ_LENGTH);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_LENGTH_OFST, p_calibration->k_rta_obj_length_ofst[x], TA_MAX_K_RTA_OBJ_LENGTH_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_SPEED, p_calibration->k_rta_obj_speed[x], TA_MAX_K_RTA_OBJ_SPEED);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_SPEED_OFST, p_calibration->k_rta_obj_speed_ofst[x], TA_MAX_K_RTA_OBJ_SPEED_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VCS_LAT_VEL, p_calibration->k_rta_obj_vcs_lat_vel[x], TA_MAX_K_RTA_OBJ_VCS_LAT_VEL);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_OFST, p_calibration->k_rta_obj_vcs_lat_vel_ofst[x], TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL, p_calibration->k_rta_obj_vcs_lat_vel_rel[x], TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST, p_calibration->k_rta_obj_vcs_lat_vel_rel_ofst[x], TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VCS_LONG_VEL, p_calibration->k_rta_obj_vcs_long_vel[x], TA_MAX_K_RTA_OBJ_VCS_LONG_VEL);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_OFST, p_calibration->k_rta_obj_vcs_long_vel_ofst[x], TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL, p_calibration->k_rta_obj_vcs_long_vel_rel[x], TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST, p_calibration->k_rta_obj_vcs_long_vel_rel_ofst[x], TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB, p_calibration->k_rta_obj_vru_class_prob[x], TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB_OFST, p_calibration->k_rta_obj_vru_class_prob_ofst[x], TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_WIDTH, p_calibration->k_rta_obj_width[x], TA_MAX_K_RTA_OBJ_WIDTH);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_OBJ_WIDTH_OFST, p_calibration->k_rta_obj_width_ofst[x], TA_MAX_K_RTA_OBJ_WIDTH_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_LONG_ACCELERATION, p_calibration->k_ta_ego_long_acceleration[x], TA_MAX_K_TA_EGO_LONG_ACCELERATION);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_LONG_ACCELERATION_OFST, p_calibration->k_ta_ego_long_acceleration_ofst[x], TA_MAX_K_TA_EGO_LONG_ACCELERATION_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_SPEED, p_calibration->k_ta_ego_speed[x], TA_MAX_K_TA_EGO_SPEED);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_SPEED_OFST, p_calibration->k_ta_ego_speed_ofst[x], TA_MAX_K_TA_EGO_SPEED_OFST);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_YAWRATE, p_calibration->k_ta_ego_yawrate[x], TA_MAX_K_TA_EGO_YAWRATE);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_YAWRATE_OFST, p_calibration->k_ta_ego_yawrate_ofst[x], TA_MAX_K_TA_EGO_YAWRATE_OFST);

        }
}
{
    uint8_t x;
    for (x = 0u; x < TA_K_TA_ALERT_LVL_1_TTP_THRESHOLD_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALERT_LVL_1_TTP_THRESHOLD, p_calibration->k_ta_alert_lvl_1_ttp_threshold[x], TA_MAX_K_TA_ALERT_LVL_1_TTP_THRESHOLD);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_ta_critical_approach_check_ego_circles[x], TA_MAX_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES);

        }
}
{
    uint8_t x;
    for (x = 0u; x < TA_K_RTA_INFO_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT, p_calibration->k_rta_info_zone_left_lat[x], TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT_HYS, p_calibration->k_rta_info_zone_left_lat_hys[x], TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT_HYS);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG, p_calibration->k_rta_info_zone_left_long[x], TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG_HYS, p_calibration->k_rta_info_zone_left_long_hys[x], TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG_HYS);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT, p_calibration->k_rta_info_zone_right_lat[x], TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT_HYS, p_calibration->k_rta_info_zone_right_lat_hys[x], TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT_HYS);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG, p_calibration->k_rta_info_zone_right_long[x], TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG_HYS, p_calibration->k_rta_info_zone_right_long_hys[x], TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG_HYS);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_WING_ZONE_LEFT_LAT, p_calibration->k_rta_wing_zone_left_lat[x], TA_MAX_K_RTA_WING_ZONE_LEFT_LAT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_WING_ZONE_LEFT_LAT_HYS, p_calibration->k_rta_wing_zone_left_lat_hys[x], TA_MAX_K_RTA_WING_ZONE_LEFT_LAT_HYS);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_WING_ZONE_LEFT_LONG, p_calibration->k_rta_wing_zone_left_long[x], TA_MAX_K_RTA_WING_ZONE_LEFT_LONG);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_WING_ZONE_LEFT_LONG_HYS, p_calibration->k_rta_wing_zone_left_long_hys[x], TA_MAX_K_RTA_WING_ZONE_LEFT_LONG_HYS);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT, p_calibration->k_rta_wing_zone_right_lat[x], TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT_HYS, p_calibration->k_rta_wing_zone_right_lat_hys[x], TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT_HYS);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG, p_calibration->k_rta_wing_zone_right_long[x], TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG_HYS, p_calibration->k_rta_wing_zone_right_long_hys[x], TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG_HYS);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN, p_calibration->k_ta_lookup_turning_host_curvature_min[x], TA_MAX_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_LOOKUP_TURNING_HOST_SPEED, p_calibration->k_ta_lookup_turning_host_speed[x], TA_MAX_K_TA_LOOKUP_TURNING_HOST_SPEED);

        }
}
{
    uint8_t x;
    for (x = 0u; x < TA_K_FTA_DANGER_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_DANGER_ZONE_LEFT_LAT, p_calibration->k_fta_danger_zone_left_lat[x], TA_MAX_K_FTA_DANGER_ZONE_LEFT_LAT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_DANGER_ZONE_LEFT_LONG, p_calibration->k_fta_danger_zone_left_long[x], TA_MAX_K_FTA_DANGER_ZONE_LEFT_LONG);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LAT, p_calibration->k_fta_danger_zone_right_lat[x], TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LAT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LONG, p_calibration->k_fta_danger_zone_right_long[x], TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LONG);

        }
}

   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_F_FTA_ENABLE, p_calibration->k_f_fta_enable, TA_MAX_K_F_FTA_ENABLE);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC, p_calibration->k_f_fta_enable_brake_gradient_logic, TA_MAX_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_F_FTA_ENABLE_DANGER_ZONES, p_calibration->k_f_fta_enable_danger_zones, TA_MAX_K_F_FTA_ENABLE_DANGER_ZONES);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_F_RTA_ENABLE, p_calibration->k_f_rta_enable, TA_MAX_K_F_RTA_ENABLE);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_F_RTA_ENABLE_INFO_ZONES, p_calibration->k_f_rta_enable_info_zones, TA_MAX_K_F_RTA_ENABLE_INFO_ZONES);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_F_RTA_ENABLE_WING_ZONES, p_calibration->k_f_rta_enable_wing_zones, TA_MAX_K_F_RTA_ENABLE_WING_ZONES);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_F_TA_ENABLE_DEBUG_MODE, p_calibration->k_f_ta_enable_debug_mode, TA_MAX_K_F_TA_ENABLE_DEBUG_MODE);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_BRAKE_DEAD_TIME, p_calibration->k_fta_brake_dead_time, TA_MAX_K_FTA_BRAKE_DEAD_TIME);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_BRAKE_DECELERATION_MAX, p_calibration->k_fta_brake_deceleration_max, TA_MAX_K_FTA_BRAKE_DECELERATION_MAX);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_BRAKE_GRADIENT, p_calibration->k_fta_brake_gradient, TA_MAX_K_FTA_BRAKE_GRADIENT);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_fta_danger_zone_point_size, TA_MAX_K_FTA_DANGER_ZONE_POINT_SIZE);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_fta_obj_age_min, TA_MAX_K_FTA_OBJ_AGE_MIN);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN, p_calibration->k_fta_obj_vcs_long_pos_straight_min, TA_MAX_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX, p_calibration->k_fta_obj_velocity_heading_diff_max, TA_MAX_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY, p_calibration->k_pfgs_qualification_check_f_stationary, TA_MAX_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ, p_calibration->k_pfgs_qualification_counter_fast_obj, TA_MAX_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ, p_calibration->k_pfgs_qualification_counter_slow_obj, TA_MAX_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_PFGS_QUALIFICATION_TTC_MIN, p_calibration->k_pfgs_qualification_ttc_min, TA_MAX_K_PFGS_QUALIFICATION_TTC_MIN);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED, p_calibration->k_pfgs_symbol_request_sides_enabled, TA_MAX_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP, p_calibration->k_rta_f_higher_obj_crit_based_on_lower_ttp, TA_MAX_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_rta_info_zone_point_size, TA_MAX_K_RTA_INFO_ZONE_POINT_SIZE);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_rta_obj_age_min, TA_MAX_K_RTA_OBJ_AGE_MIN);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN, p_calibration->k_rta_ttp_curve_suppression_obj_distance_min, TA_MAX_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX, p_calibration->k_rta_ttp_obj_abs_heading_diff_max, TA_MAX_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX, p_calibration->k_rta_ttp_obj_abs_lat_vel_rel_max, TA_MAX_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_rta_wing_zone_point_size, TA_MAX_K_RTA_WING_ZONE_POINT_SIZE);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ACTIVE_OBJ_TTP_OFFSET, p_calibration->k_ta_active_obj_ttp_offset, TA_MAX_K_TA_ACTIVE_OBJ_TTP_OFFSET);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_ta_alert_holding_cycles, TA_MAX_K_TA_ALERT_HOLDING_CYCLES);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX, p_calibration->k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max, TA_MAX_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX, p_calibration->k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max, TA_MAX_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALERT_LVL_2_TTC_THRESHOLD, p_calibration->k_ta_alert_lvl_2_ttc_threshold, TA_MAX_K_TA_ALERT_LVL_2_TTC_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALERT_LVL_3_TTB_THRESHOLD, p_calibration->k_ta_alert_lvl_3_ttb_threshold, TA_MAX_K_TA_ALERT_LVL_3_TTB_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALERT_LVL_3_TTC_THRESHOLD, p_calibration->k_ta_alert_lvl_3_ttc_threshold, TA_MAX_K_TA_ALERT_LVL_3_TTC_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALERT_LVL_4_DECEL_THRESHOLD, p_calibration->k_ta_alert_lvl_4_decel_threshold, TA_MAX_K_TA_ALERT_LVL_4_DECEL_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALERT_LVL_4_TTC_THRESHOLD, p_calibration->k_ta_alert_lvl_4_ttc_threshold, TA_MAX_K_TA_ALERT_LVL_4_TTC_THRESHOLD);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_ta_alert_qualifying_cycles, TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH, p_calibration->k_ta_always_overwrite_ta_mode_to_both, TA_MAX_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN, p_calibration->k_ta_critical_approach_angle_diff_min, TA_MAX_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE, p_calibration->k_ta_critical_approach_min_safe_distance, TA_MAX_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_ACCELERATION_WEIGHT, p_calibration->k_ta_ego_acceleration_weight, TA_MAX_K_TA_EGO_ACCELERATION_WEIGHT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR, p_calibration->k_ta_ego_circle_host_length_factor, TA_MAX_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_CIRCLE_OFFSET, p_calibration->k_ta_ego_circle_offset, TA_MAX_K_TA_EGO_CIRCLE_OFFSET);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_DECELERATION_WEIGHT, p_calibration->k_ta_ego_deceleration_weight, TA_MAX_K_TA_EGO_DECELERATION_WEIGHT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_MAX_PRED_YAW_ANGLE, p_calibration->k_ta_ego_max_pred_yaw_angle, TA_MAX_K_TA_EGO_MAX_PRED_YAW_ANGLE);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_ta_ego_pred_const_velocity_pred_steps_min, TA_MAX_K_TA_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_SHAPE_GAIN_FIXED, p_calibration->k_ta_ego_shape_gain_fixed, TA_MAX_K_TA_EGO_SHAPE_GAIN_FIXED);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP, p_calibration->k_ta_ego_shape_gain_per_pred_step, TA_MAX_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN, p_calibration->k_ta_ego_yawangle_integration_yawrate_min, TA_MAX_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY, p_calibration->k_ta_f_apply_ttp_hysteresis_globally, TA_MAX_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS, p_calibration->k_ta_f_only_allow_consecutive_ttc_based_alert_levels, TA_MAX_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ, p_calibration->k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj, TA_MAX_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ);
Ct_Is_Bool_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP, p_calibration->k_ta_f_skip_holding_for_single_alert_level_drop, TA_MAX_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_OBJ_ACCELERATION_LAT_WEIGHT, p_calibration->k_ta_obj_acceleration_lat_weight, TA_MAX_K_TA_OBJ_ACCELERATION_LAT_WEIGHT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_OBJ_ACCELERATION_LONG_WEIGHT, p_calibration->k_ta_obj_acceleration_long_weight, TA_MAX_K_TA_OBJ_ACCELERATION_LONG_WEIGHT);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_OBJ_PRED_SPEED_MIN, p_calibration->k_ta_obj_pred_speed_min, TA_MAX_K_TA_OBJ_PRED_SPEED_MIN);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_OBJ_SHAPE_GAIN_FIXED, p_calibration->k_ta_obj_shape_gain_fixed, TA_MAX_K_TA_OBJ_SHAPE_GAIN_FIXED);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP, p_calibration->k_ta_obj_shape_gain_per_pred_step, TA_MAX_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP);
Ct_Is_Uint8_In_Bondaries(&f_ta_calibration_in_boundaries, 0, p_calibration->k_ta_prediction_steps_max, TA_MAX_K_TA_PREDICTION_STEPS_MAX);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TA_STRAIGHT_HOST_CURVATURE_MAX, p_calibration->k_ta_straight_host_curvature_max, TA_MAX_K_TA_STRAIGHT_HOST_CURVATURE_MAX);
Ct_Is_Float_In_Bondaries(&f_ta_calibration_in_boundaries, TA_MIN_K_TAP_LVL_2_HOST_CURVATURE_MIN, p_calibration->k_tap_lvl_2_host_curvature_min, TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN);


   return f_ta_calibration_in_boundaries;
}

