/**
* @file scw_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in scw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "scw_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "scw_core_calibration_check.h"
#include "scw_core_calibration_t.h"
#include "scw_customer_calibration_t.h"
#include "scw_public_calibration_check.h"
#include "scw_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Scw_Update_Core_Cal_By_Public(Scw_Core_Calibration_T* cal_dst, const Scw_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Scw_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_scw_min_host_speed = cal_src->k_scw_min_host_speed;
        cal_dst->k_scw_min_host_speed_hys = cal_src->k_scw_min_host_speed_hys;
        cal_dst->k_scw_max_lat_pos_ratio = cal_src->k_scw_max_lat_pos_ratio;
        cal_dst->k_scw_candidate_heading[0] = cal_src->k_scw_candidate_heading[0];
        cal_dst->k_scw_candidate_heading[1] = cal_src->k_scw_candidate_heading[1];
        cal_dst->k_scw_candidate_heading_hys = cal_src->k_scw_candidate_heading_hys;
        cal_dst->k_scw_candidate_yawrate = cal_src->k_scw_candidate_yawrate;
        cal_dst->k_scw_candidate_yawrate_hys = cal_src->k_scw_candidate_yawrate_hys;
        cal_dst->k_scw_candidate_velocity[0] = cal_src->k_scw_candidate_velocity[0];
        cal_dst->k_scw_candidate_velocity[1] = cal_src->k_scw_candidate_velocity[1];
        cal_dst->k_scw_candidate_velocity_hys = cal_src->k_scw_candidate_velocity_hys;
        cal_dst->k_scw_candidate_relative_velocity[0] = cal_src->k_scw_candidate_relative_velocity[0];
        cal_dst->k_scw_candidate_relative_velocity[1] = cal_src->k_scw_candidate_relative_velocity[1];
        cal_dst->k_scw_candidate_relative_vel_hys = cal_src->k_scw_candidate_relative_vel_hys;
        cal_dst->k_scw_min_candidate_existence_probability = cal_src->k_scw_min_candidate_existence_probability;
        cal_dst->k_scw_min_exist_prob_radar_guardrail = cal_src->k_scw_min_exist_prob_radar_guardrail;
        cal_dst->k_scw_min_dynamic_lat_ttc = cal_src->k_scw_min_dynamic_lat_ttc;
        cal_dst->k_scw_max_dynamic_lat_ttc = cal_src->k_scw_max_dynamic_lat_ttc;
        cal_dst->k_scw_min_guardrail_lat_ttc = cal_src->k_scw_min_guardrail_lat_ttc;
        cal_dst->k_scw_max_guardrail_lat_ttc = cal_src->k_scw_max_guardrail_lat_ttc;
        cal_dst->k_scw_trailer_lat_ttc_extension = cal_src->k_scw_trailer_lat_ttc_extension;
        cal_dst->k_scw_min_dynamic_lat_distance = cal_src->k_scw_min_dynamic_lat_distance;
        cal_dst->k_scw_max_dynamic_lat_distance = cal_src->k_scw_max_dynamic_lat_distance;
        cal_dst->k_scw_min_guardrail_lat_distance = cal_src->k_scw_min_guardrail_lat_distance;
        cal_dst->k_scw_max_guardrail_lat_distance = cal_src->k_scw_max_guardrail_lat_distance;
        cal_dst->k_scw_critical_lat_ttc_hys = cal_src->k_scw_critical_lat_ttc_hys;
        cal_dst->k_scw_critical_lat_distance_hys = cal_src->k_scw_critical_lat_distance_hys;
        cal_dst->k_scw_initial_zone_x[0] = cal_src->k_scw_initial_zone_x[0];
        cal_dst->k_scw_initial_zone_x[1] = cal_src->k_scw_initial_zone_x[1];
        cal_dst->k_scw_initial_zone_x[2] = cal_src->k_scw_initial_zone_x[2];
        cal_dst->k_scw_initial_zone_x[3] = cal_src->k_scw_initial_zone_x[3];
        cal_dst->k_scw_initial_zone_x[4] = cal_src->k_scw_initial_zone_x[4];
        cal_dst->k_scw_initial_zone_x[5] = cal_src->k_scw_initial_zone_x[5];
        cal_dst->k_scw_initial_zone_y[0] = cal_src->k_scw_initial_zone_y[0];
        cal_dst->k_scw_initial_zone_y[1] = cal_src->k_scw_initial_zone_y[1];
        cal_dst->k_scw_initial_zone_y[2] = cal_src->k_scw_initial_zone_y[2];
        cal_dst->k_scw_initial_zone_y[3] = cal_src->k_scw_initial_zone_y[3];
        cal_dst->k_scw_initial_zone_y[4] = cal_src->k_scw_initial_zone_y[4];
        cal_dst->k_scw_initial_zone_y[5] = cal_src->k_scw_initial_zone_y[5];
        cal_dst->k_scw_hys_zone_x_offset[0] = cal_src->k_scw_hys_zone_x_offset[0];
        cal_dst->k_scw_hys_zone_x_offset[1] = cal_src->k_scw_hys_zone_x_offset[1];
        cal_dst->k_scw_hys_zone_x_offset[2] = cal_src->k_scw_hys_zone_x_offset[2];
        cal_dst->k_scw_hys_zone_x_offset[3] = cal_src->k_scw_hys_zone_x_offset[3];
        cal_dst->k_scw_hys_zone_x_offset[4] = cal_src->k_scw_hys_zone_x_offset[4];
        cal_dst->k_scw_hys_zone_x_offset[5] = cal_src->k_scw_hys_zone_x_offset[5];
        cal_dst->k_scw_hys_zone_y_offset[0] = cal_src->k_scw_hys_zone_y_offset[0];
        cal_dst->k_scw_hys_zone_y_offset[1] = cal_src->k_scw_hys_zone_y_offset[1];
        cal_dst->k_scw_hys_zone_y_offset[2] = cal_src->k_scw_hys_zone_y_offset[2];
        cal_dst->k_scw_hys_zone_y_offset[3] = cal_src->k_scw_hys_zone_y_offset[3];
        cal_dst->k_scw_hys_zone_y_offset[4] = cal_src->k_scw_hys_zone_y_offset[4];
        cal_dst->k_scw_hys_zone_y_offset[5] = cal_src->k_scw_hys_zone_y_offset[5];
        cal_dst->k_scw_lateral_distance_default = cal_src->k_scw_lateral_distance_default;
        cal_dst->k_scw_lateral_ttc_max = cal_src->k_scw_lateral_ttc_max;
        cal_dst->k_scw_lateral_ttc_default = cal_src->k_scw_lateral_ttc_default;
        cal_dst->k_scw_ttle_max = cal_src->k_scw_ttle_max;
        cal_dst->k_scw_ttle_default = cal_src->k_scw_ttle_default;
        cal_dst->k_scw_ttp_max = cal_src->k_scw_ttp_max;
        cal_dst->k_scw_ttp_default = cal_src->k_scw_ttp_default;
        cal_dst->k_scw_trailer_zone_ext_safety_margin = cal_src->k_scw_trailer_zone_ext_safety_margin;
        cal_dst->k_scw_trailer_zone_ext_safety_margin_lat = cal_src->k_scw_trailer_zone_ext_safety_margin_lat;
        cal_dst->k_scw_max_zone_length = cal_src->k_scw_max_zone_length;
        cal_dst->k_scw_max_zone_width = cal_src->k_scw_max_zone_width;
        cal_dst->k_scw_f_adjust_zones_to_ego_size = cal_src->k_scw_f_adjust_zones_to_ego_size;
        cal_dst->k_scw_f_enable_via_cal = cal_src->k_scw_f_enable_via_cal;
        cal_dst->k_scw_f_dynamic_enable_via_cal = cal_src->k_scw_f_dynamic_enable_via_cal;
        cal_dst->k_scw_f_guardrail_enable_via_cal = cal_src->k_scw_f_guardrail_enable_via_cal;
        cal_dst->k_scw_f_enable = cal_src->k_scw_f_enable;
        cal_dst->k_scw_f_enable_dynamic = cal_src->k_scw_f_enable_dynamic;
        cal_dst->k_scw_f_enable_guardrail = cal_src->k_scw_f_enable_guardrail;
        cal_dst->k_scw_f_enable_trailer_zone_extension = cal_src->k_scw_f_enable_trailer_zone_extension;
        cal_dst->k_scw_f_enable_trailer_ttc_extension = cal_src->k_scw_f_enable_trailer_ttc_extension;
        cal_dst->k_scw_min_guardrail_age = cal_src->k_scw_min_guardrail_age;
        cal_dst->k_scw_guardrail_freeze_period = cal_src->k_scw_guardrail_freeze_period;
        cal_dst->k_scw_min_candidate_age = cal_src->k_scw_min_candidate_age;
        cal_dst->k_scw_candidate_mature_cycles_in_zone_threshold = cal_src->k_scw_candidate_mature_cycles_in_zone_threshold;
        cal_dst->k_scw_guardrail_cycles_in_zone_threshold = cal_src->k_scw_guardrail_cycles_in_zone_threshold;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Scw_Update_Core_Cal_By_Core(Scw_Core_Calibration_T* cal_dst, const Scw_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Scw_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_scw_min_host_speed = cal_src->k_scw_min_host_speed;
        cal_dst->k_scw_min_host_speed_hys = cal_src->k_scw_min_host_speed_hys;
        cal_dst->k_scw_max_lat_pos_ratio = cal_src->k_scw_max_lat_pos_ratio;
        cal_dst->k_scw_candidate_heading[0] = cal_src->k_scw_candidate_heading[0];
        cal_dst->k_scw_candidate_heading[1] = cal_src->k_scw_candidate_heading[1];
        cal_dst->k_scw_candidate_heading_hys = cal_src->k_scw_candidate_heading_hys;
        cal_dst->k_scw_candidate_yawrate = cal_src->k_scw_candidate_yawrate;
        cal_dst->k_scw_candidate_yawrate_hys = cal_src->k_scw_candidate_yawrate_hys;
        cal_dst->k_scw_candidate_velocity[0] = cal_src->k_scw_candidate_velocity[0];
        cal_dst->k_scw_candidate_velocity[1] = cal_src->k_scw_candidate_velocity[1];
        cal_dst->k_scw_candidate_velocity_hys = cal_src->k_scw_candidate_velocity_hys;
        cal_dst->k_scw_candidate_relative_velocity[0] = cal_src->k_scw_candidate_relative_velocity[0];
        cal_dst->k_scw_candidate_relative_velocity[1] = cal_src->k_scw_candidate_relative_velocity[1];
        cal_dst->k_scw_candidate_relative_vel_hys = cal_src->k_scw_candidate_relative_vel_hys;
        cal_dst->k_scw_min_candidate_existence_probability = cal_src->k_scw_min_candidate_existence_probability;
        cal_dst->k_scw_min_exist_prob_radar_guardrail = cal_src->k_scw_min_exist_prob_radar_guardrail;
        cal_dst->k_scw_min_dynamic_lat_ttc = cal_src->k_scw_min_dynamic_lat_ttc;
        cal_dst->k_scw_max_dynamic_lat_ttc = cal_src->k_scw_max_dynamic_lat_ttc;
        cal_dst->k_scw_min_guardrail_lat_ttc = cal_src->k_scw_min_guardrail_lat_ttc;
        cal_dst->k_scw_max_guardrail_lat_ttc = cal_src->k_scw_max_guardrail_lat_ttc;
        cal_dst->k_scw_trailer_lat_ttc_extension = cal_src->k_scw_trailer_lat_ttc_extension;
        cal_dst->k_scw_min_dynamic_lat_distance = cal_src->k_scw_min_dynamic_lat_distance;
        cal_dst->k_scw_max_dynamic_lat_distance = cal_src->k_scw_max_dynamic_lat_distance;
        cal_dst->k_scw_min_guardrail_lat_distance = cal_src->k_scw_min_guardrail_lat_distance;
        cal_dst->k_scw_max_guardrail_lat_distance = cal_src->k_scw_max_guardrail_lat_distance;
        cal_dst->k_scw_critical_lat_ttc_hys = cal_src->k_scw_critical_lat_ttc_hys;
        cal_dst->k_scw_critical_lat_distance_hys = cal_src->k_scw_critical_lat_distance_hys;
        cal_dst->k_scw_initial_zone_x[0] = cal_src->k_scw_initial_zone_x[0];
        cal_dst->k_scw_initial_zone_x[1] = cal_src->k_scw_initial_zone_x[1];
        cal_dst->k_scw_initial_zone_x[2] = cal_src->k_scw_initial_zone_x[2];
        cal_dst->k_scw_initial_zone_x[3] = cal_src->k_scw_initial_zone_x[3];
        cal_dst->k_scw_initial_zone_x[4] = cal_src->k_scw_initial_zone_x[4];
        cal_dst->k_scw_initial_zone_x[5] = cal_src->k_scw_initial_zone_x[5];
        cal_dst->k_scw_initial_zone_y[0] = cal_src->k_scw_initial_zone_y[0];
        cal_dst->k_scw_initial_zone_y[1] = cal_src->k_scw_initial_zone_y[1];
        cal_dst->k_scw_initial_zone_y[2] = cal_src->k_scw_initial_zone_y[2];
        cal_dst->k_scw_initial_zone_y[3] = cal_src->k_scw_initial_zone_y[3];
        cal_dst->k_scw_initial_zone_y[4] = cal_src->k_scw_initial_zone_y[4];
        cal_dst->k_scw_initial_zone_y[5] = cal_src->k_scw_initial_zone_y[5];
        cal_dst->k_scw_hys_zone_x_offset[0] = cal_src->k_scw_hys_zone_x_offset[0];
        cal_dst->k_scw_hys_zone_x_offset[1] = cal_src->k_scw_hys_zone_x_offset[1];
        cal_dst->k_scw_hys_zone_x_offset[2] = cal_src->k_scw_hys_zone_x_offset[2];
        cal_dst->k_scw_hys_zone_x_offset[3] = cal_src->k_scw_hys_zone_x_offset[3];
        cal_dst->k_scw_hys_zone_x_offset[4] = cal_src->k_scw_hys_zone_x_offset[4];
        cal_dst->k_scw_hys_zone_x_offset[5] = cal_src->k_scw_hys_zone_x_offset[5];
        cal_dst->k_scw_hys_zone_y_offset[0] = cal_src->k_scw_hys_zone_y_offset[0];
        cal_dst->k_scw_hys_zone_y_offset[1] = cal_src->k_scw_hys_zone_y_offset[1];
        cal_dst->k_scw_hys_zone_y_offset[2] = cal_src->k_scw_hys_zone_y_offset[2];
        cal_dst->k_scw_hys_zone_y_offset[3] = cal_src->k_scw_hys_zone_y_offset[3];
        cal_dst->k_scw_hys_zone_y_offset[4] = cal_src->k_scw_hys_zone_y_offset[4];
        cal_dst->k_scw_hys_zone_y_offset[5] = cal_src->k_scw_hys_zone_y_offset[5];
        cal_dst->k_scw_lateral_distance_default = cal_src->k_scw_lateral_distance_default;
        cal_dst->k_scw_lateral_ttc_max = cal_src->k_scw_lateral_ttc_max;
        cal_dst->k_scw_lateral_ttc_default = cal_src->k_scw_lateral_ttc_default;
        cal_dst->k_scw_ttle_max = cal_src->k_scw_ttle_max;
        cal_dst->k_scw_ttle_default = cal_src->k_scw_ttle_default;
        cal_dst->k_scw_ttp_max = cal_src->k_scw_ttp_max;
        cal_dst->k_scw_ttp_default = cal_src->k_scw_ttp_default;
        cal_dst->k_scw_trailer_zone_ext_safety_margin = cal_src->k_scw_trailer_zone_ext_safety_margin;
        cal_dst->k_scw_trailer_zone_ext_safety_margin_lat = cal_src->k_scw_trailer_zone_ext_safety_margin_lat;
        cal_dst->k_scw_max_zone_length = cal_src->k_scw_max_zone_length;
        cal_dst->k_scw_max_zone_width = cal_src->k_scw_max_zone_width;
        cal_dst->k_scw_f_adjust_zones_to_ego_size = cal_src->k_scw_f_adjust_zones_to_ego_size;
        cal_dst->k_scw_f_enable_via_cal = cal_src->k_scw_f_enable_via_cal;
        cal_dst->k_scw_f_dynamic_enable_via_cal = cal_src->k_scw_f_dynamic_enable_via_cal;
        cal_dst->k_scw_f_guardrail_enable_via_cal = cal_src->k_scw_f_guardrail_enable_via_cal;
        cal_dst->k_scw_f_enable = cal_src->k_scw_f_enable;
        cal_dst->k_scw_f_enable_dynamic = cal_src->k_scw_f_enable_dynamic;
        cal_dst->k_scw_f_enable_guardrail = cal_src->k_scw_f_enable_guardrail;
        cal_dst->k_scw_f_enable_trailer_zone_extension = cal_src->k_scw_f_enable_trailer_zone_extension;
        cal_dst->k_scw_f_enable_trailer_ttc_extension = cal_src->k_scw_f_enable_trailer_ttc_extension;
        cal_dst->k_scw_min_guardrail_age = cal_src->k_scw_min_guardrail_age;
        cal_dst->k_scw_guardrail_freeze_period = cal_src->k_scw_guardrail_freeze_period;
        cal_dst->k_scw_min_candidate_age = cal_src->k_scw_min_candidate_age;
        cal_dst->k_scw_candidate_mature_cycles_in_zone_threshold = cal_src->k_scw_candidate_mature_cycles_in_zone_threshold;
        cal_dst->k_scw_guardrail_cycles_in_zone_threshold = cal_src->k_scw_guardrail_cycles_in_zone_threshold;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;
        cal_dst->k_unused_padding_byte_1 = cal_src->k_unused_padding_byte_1;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Scw_Update_Customer_Cal_By_Public(Scw_Customer_Calibration_T* cal_dst, const Scw_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Scw_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {

        f_result = (boolean_T) 1;
    }
    return f_result;
}


