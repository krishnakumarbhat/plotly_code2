/**
* @file esa_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in esa_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "esa_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "esa_core_calibration_check.h"
#include "esa_core_calibration_t.h"
#include "esa_customer_calibration_t.h"
#include "esa_public_calibration_check.h"
#include "esa_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Esa_Update_Core_Cal_By_Public(Esa_Core_Calibration_T* cal_dst, const Esa_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Esa_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_esa_zone_x[0] = cal_src->k_esa_zone_x[0];
        cal_dst->k_esa_zone_x[1] = cal_src->k_esa_zone_x[1];
        cal_dst->k_esa_zone_x[2] = cal_src->k_esa_zone_x[2];
        cal_dst->k_esa_zone_x[3] = cal_src->k_esa_zone_x[3];
        cal_dst->k_esa_zone_x[4] = cal_src->k_esa_zone_x[4];
        cal_dst->k_esa_zone_x[5] = cal_src->k_esa_zone_x[5];
        cal_dst->k_esa_zone_x_hys[0] = cal_src->k_esa_zone_x_hys[0];
        cal_dst->k_esa_zone_x_hys[1] = cal_src->k_esa_zone_x_hys[1];
        cal_dst->k_esa_zone_x_hys[2] = cal_src->k_esa_zone_x_hys[2];
        cal_dst->k_esa_zone_x_hys[3] = cal_src->k_esa_zone_x_hys[3];
        cal_dst->k_esa_zone_x_hys[4] = cal_src->k_esa_zone_x_hys[4];
        cal_dst->k_esa_zone_x_hys[5] = cal_src->k_esa_zone_x_hys[5];
        cal_dst->k_esa_zone_y[0] = cal_src->k_esa_zone_y[0];
        cal_dst->k_esa_zone_y[1] = cal_src->k_esa_zone_y[1];
        cal_dst->k_esa_zone_y[2] = cal_src->k_esa_zone_y[2];
        cal_dst->k_esa_zone_y[3] = cal_src->k_esa_zone_y[3];
        cal_dst->k_esa_zone_y[4] = cal_src->k_esa_zone_y[4];
        cal_dst->k_esa_zone_y[5] = cal_src->k_esa_zone_y[5];
        cal_dst->k_esa_zone_y_hys[0] = cal_src->k_esa_zone_y_hys[0];
        cal_dst->k_esa_zone_y_hys[1] = cal_src->k_esa_zone_y_hys[1];
        cal_dst->k_esa_zone_y_hys[2] = cal_src->k_esa_zone_y_hys[2];
        cal_dst->k_esa_zone_y_hys[3] = cal_src->k_esa_zone_y_hys[3];
        cal_dst->k_esa_zone_y_hys[4] = cal_src->k_esa_zone_y_hys[4];
        cal_dst->k_esa_zone_y_hys[5] = cal_src->k_esa_zone_y_hys[5];
        cal_dst->k_esa_max_range = cal_src->k_esa_max_range;
        cal_dst->k_esa_min_lane_width = cal_src->k_esa_min_lane_width;
        cal_dst->k_esa_max_lane_width = cal_src->k_esa_max_lane_width;
        cal_dst->k_esa_min_exist_prob = cal_src->k_esa_min_exist_prob;
        cal_dst->k_esa_min_curve_radius = cal_src->k_esa_min_curve_radius;
        cal_dst->k_esa_min_curve_radius_hys = cal_src->k_esa_min_curve_radius_hys;
        cal_dst->k_esa_max_curvi_heading_abs = cal_src->k_esa_max_curvi_heading_abs;
        cal_dst->k_esa_min_obj_curvi_long_vel_abs = cal_src->k_esa_min_obj_curvi_long_vel_abs;
        cal_dst->k_esa_critical_longitudinal_ttc = cal_src->k_esa_critical_longitudinal_ttc;
        cal_dst->k_esa_critical_longitudinal_ttc_hys = cal_src->k_esa_critical_longitudinal_ttc_hys;
        cal_dst->k_esa_obj_safe_deceleration_threshold = cal_src->k_esa_obj_safe_deceleration_threshold;
        cal_dst->k_esa_obj_safe_deceleration_threshold_hys = cal_src->k_esa_obj_safe_deceleration_threshold_hys;
        cal_dst->k_esa_host_activation_speed_min = cal_src->k_esa_host_activation_speed_min;
        cal_dst->k_esa_host_activation_speed_min_hys = cal_src->k_esa_host_activation_speed_min_hys;
        cal_dst->k_esa_host_activation_speed_max = cal_src->k_esa_host_activation_speed_max;
        cal_dst->k_esa_host_activation_speed_max_hys = cal_src->k_esa_host_activation_speed_max_hys;
        cal_dst->k_esa_f_enable_via_cal = cal_src->k_esa_f_enable_via_cal;
        cal_dst->k_esa_f_enable = cal_src->k_esa_f_enable;
        cal_dst->k_esa_f_allow_min_curve_radius = cal_src->k_esa_f_allow_min_curve_radius;
        cal_dst->k_esa_f_allow_obj_critical_ttc_and_deceleration = cal_src->k_esa_f_allow_obj_critical_ttc_and_deceleration;
        cal_dst->k_esa_f_allow_obj_selection_ttc = cal_src->k_esa_f_allow_obj_selection_ttc;
        cal_dst->k_esa_f_allow_obj_selection_deceleration = cal_src->k_esa_f_allow_obj_selection_deceleration;
        cal_dst->k_esa_f_allow_obj_selection_long_distance = cal_src->k_esa_f_allow_obj_selection_long_distance;
        cal_dst->k_esa_min_track_age = cal_src->k_esa_min_track_age;
        cal_dst->k_esa_min_mature_cycles = cal_src->k_esa_min_mature_cycles;
        cal_dst->k_esa_alert_holding_cycles = cal_src->k_esa_alert_holding_cycles;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Esa_Update_Core_Cal_By_Core(Esa_Core_Calibration_T* cal_dst, const Esa_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Esa_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_esa_zone_x[0] = cal_src->k_esa_zone_x[0];
        cal_dst->k_esa_zone_x[1] = cal_src->k_esa_zone_x[1];
        cal_dst->k_esa_zone_x[2] = cal_src->k_esa_zone_x[2];
        cal_dst->k_esa_zone_x[3] = cal_src->k_esa_zone_x[3];
        cal_dst->k_esa_zone_x[4] = cal_src->k_esa_zone_x[4];
        cal_dst->k_esa_zone_x[5] = cal_src->k_esa_zone_x[5];
        cal_dst->k_esa_zone_x_hys[0] = cal_src->k_esa_zone_x_hys[0];
        cal_dst->k_esa_zone_x_hys[1] = cal_src->k_esa_zone_x_hys[1];
        cal_dst->k_esa_zone_x_hys[2] = cal_src->k_esa_zone_x_hys[2];
        cal_dst->k_esa_zone_x_hys[3] = cal_src->k_esa_zone_x_hys[3];
        cal_dst->k_esa_zone_x_hys[4] = cal_src->k_esa_zone_x_hys[4];
        cal_dst->k_esa_zone_x_hys[5] = cal_src->k_esa_zone_x_hys[5];
        cal_dst->k_esa_zone_y[0] = cal_src->k_esa_zone_y[0];
        cal_dst->k_esa_zone_y[1] = cal_src->k_esa_zone_y[1];
        cal_dst->k_esa_zone_y[2] = cal_src->k_esa_zone_y[2];
        cal_dst->k_esa_zone_y[3] = cal_src->k_esa_zone_y[3];
        cal_dst->k_esa_zone_y[4] = cal_src->k_esa_zone_y[4];
        cal_dst->k_esa_zone_y[5] = cal_src->k_esa_zone_y[5];
        cal_dst->k_esa_zone_y_hys[0] = cal_src->k_esa_zone_y_hys[0];
        cal_dst->k_esa_zone_y_hys[1] = cal_src->k_esa_zone_y_hys[1];
        cal_dst->k_esa_zone_y_hys[2] = cal_src->k_esa_zone_y_hys[2];
        cal_dst->k_esa_zone_y_hys[3] = cal_src->k_esa_zone_y_hys[3];
        cal_dst->k_esa_zone_y_hys[4] = cal_src->k_esa_zone_y_hys[4];
        cal_dst->k_esa_zone_y_hys[5] = cal_src->k_esa_zone_y_hys[5];
        cal_dst->k_esa_max_range = cal_src->k_esa_max_range;
        cal_dst->k_esa_min_lane_width = cal_src->k_esa_min_lane_width;
        cal_dst->k_esa_max_lane_width = cal_src->k_esa_max_lane_width;
        cal_dst->k_esa_min_exist_prob = cal_src->k_esa_min_exist_prob;
        cal_dst->k_esa_min_curve_radius = cal_src->k_esa_min_curve_radius;
        cal_dst->k_esa_min_curve_radius_hys = cal_src->k_esa_min_curve_radius_hys;
        cal_dst->k_esa_max_curvi_heading_abs = cal_src->k_esa_max_curvi_heading_abs;
        cal_dst->k_esa_min_obj_curvi_long_vel_abs = cal_src->k_esa_min_obj_curvi_long_vel_abs;
        cal_dst->k_esa_critical_longitudinal_ttc = cal_src->k_esa_critical_longitudinal_ttc;
        cal_dst->k_esa_critical_longitudinal_ttc_hys = cal_src->k_esa_critical_longitudinal_ttc_hys;
        cal_dst->k_esa_obj_safe_deceleration_threshold = cal_src->k_esa_obj_safe_deceleration_threshold;
        cal_dst->k_esa_obj_safe_deceleration_threshold_hys = cal_src->k_esa_obj_safe_deceleration_threshold_hys;
        cal_dst->k_esa_host_activation_speed_min = cal_src->k_esa_host_activation_speed_min;
        cal_dst->k_esa_host_activation_speed_min_hys = cal_src->k_esa_host_activation_speed_min_hys;
        cal_dst->k_esa_host_activation_speed_max = cal_src->k_esa_host_activation_speed_max;
        cal_dst->k_esa_host_activation_speed_max_hys = cal_src->k_esa_host_activation_speed_max_hys;
        cal_dst->k_esa_f_enable_via_cal = cal_src->k_esa_f_enable_via_cal;
        cal_dst->k_esa_f_enable = cal_src->k_esa_f_enable;
        cal_dst->k_esa_f_allow_min_curve_radius = cal_src->k_esa_f_allow_min_curve_radius;
        cal_dst->k_esa_f_allow_obj_critical_ttc_and_deceleration = cal_src->k_esa_f_allow_obj_critical_ttc_and_deceleration;
        cal_dst->k_esa_f_allow_obj_selection_ttc = cal_src->k_esa_f_allow_obj_selection_ttc;
        cal_dst->k_esa_f_allow_obj_selection_deceleration = cal_src->k_esa_f_allow_obj_selection_deceleration;
        cal_dst->k_esa_f_allow_obj_selection_long_distance = cal_src->k_esa_f_allow_obj_selection_long_distance;
        cal_dst->k_esa_min_track_age = cal_src->k_esa_min_track_age;
        cal_dst->k_esa_min_mature_cycles = cal_src->k_esa_min_mature_cycles;
        cal_dst->k_esa_alert_holding_cycles = cal_src->k_esa_alert_holding_cycles;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;
        cal_dst->k_unused_padding_byte_1 = cal_src->k_unused_padding_byte_1;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Esa_Update_Customer_Cal_By_Public(Esa_Customer_Calibration_T* cal_dst, const Esa_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Esa_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {

        f_result = (boolean_T) 1;
    }
    return f_result;
}


