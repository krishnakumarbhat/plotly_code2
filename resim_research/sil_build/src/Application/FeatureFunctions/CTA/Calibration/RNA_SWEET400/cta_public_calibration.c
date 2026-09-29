
/**
* @file cta_public_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the RNA_SWEET400 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "cta_public_calibration_t.h"
#include "cta_public_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Cta_Public_Cal_Update_Defaults(Cta_Public_Calibration_T* cal_dst)
{
    Cta_Public_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)12,
   /**<version*/ (uint16_t)70,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_cta_ego_abs_speed_max*/ (float32_T) 5.0f /**< 5.0 m/s | 18.0 km/h */,
   /**<k_cta_stop_alert_ttc*/ (float32_T) 0.0 /**< 0.0 s */,
   /**<k_cta_stop_alert_ttp*/ (float32_T) -3.0 /**< -3.0 s */,
   /**<k_cta_min_speed*/ (float32_T) 1.1111 /**< 1.1111 m/s | 4.0 km/h */,
   /**<k_cta_butterfly_long*/ {(float32_T)9.0 /**< 9.0 m */,(float32_T)-14.0 /**< -14.0 m */,(float32_T)-39.0 /**< -39.0 m */,(float32_T)34.0 /**< 34.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_butterfly_lat*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)50.0 /**< 50.0 m */,(float32_T)50.0 /**< 50.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_ttc_criticality_level*/ {{(float32_T)3.15 /**< 3.15 s */,(float32_T)3.15 /**< 3.15 s */},{(float32_T)3.15 /**< 3.15 s */,(float32_T)3.15 /**< 3.15 s */}},
   /**<k_cta_speed_criticality_level*/ {{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */},{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */}},
   /**<k_cta_max_long_point_criticality_level*/ {{(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */},{(float32_T)5.0 /**< 5.0 m */,(float32_T)5.0 /**< 5.0 m */}},
   /**<k_cta_min_long_point_criticality_level*/ {{(float32_T)5.0 /**< 5.0 m */,(float32_T)5.0 /**< 5.0 m */},{(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */}},
   /**<k_cta_min_lateral_approach_speed*/ (float32_T) 0.68 /**< 0.68 m/s | 2.45 km/h */,
   /**<k_cta_max_speed*/ (float32_T) 100 /**< 100 m/s | 360.0 km/h */,
   /**<k_cta_max_length_fov*/ (float32_T) 60 /**< 60 m */,
   /**<k_cta_heading_range*/ {(float32_T)0.52f /**< 0.52 rad | 29.79 deg */,(float32_T)2.62f /**< 2.62 rad | 150.11 deg */},
   /**<k_cta_angles_zone_definition*/ {(float32_T)0.78f /**< 0.78 rad | 44.69 deg */,(float32_T)0.78f /**< 0.78 rad | 44.69 deg */},
   /**<k_cta_min_ttc_additional_mature_qualification*/ (float32_T) 3.5 /**< 3.5 s */,
   /**<k_cta_max_object_eclipse_for_level_qualification*/ (float32_T) 0.4,
   /**<k_cta_intersection_line_host_width_percentage*/ (float32_T) 0,
   /**<k_ctb_min_ttp*/ (float32_T) 0.0 /**< 0.0 s */,
   /**<k_cta_sensor_fov_border*/ {(float32_T)0.890118 /**< 0.890118 rad | 51.0 deg */,(float32_T)2.46091 /**< 2.46091 rad | 141.0 deg */},
   /**<k_cta_max_heading_variance*/ (float32_T) 3.0 /**< 3.0 rad^2 */,
   /**<k_cta_range_to_path_segment_ghost_qualif*/ (float32_T) 6.0,
   /**<k_bmw_sp25_ego_abs_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 kph */,
   /**<k_bmw_sp25_banner_criteria_check_time*/ (float32_T) 0.2f /**< 0.2 s */,
   /**<k_bmw_sp25_banner_time*/ (float32_T) 7.0f /**< 7.0 s */,
   /**<k_stla_crit_zone_G_E_line*/ (float32_T) 3.0f /**< 3.0 m */,
   /**<k_stla_crit_zone_D_C_line*/ (float32_T) 2.35f /**< 2.35 m */,
   /**<k_stla_crit_zone_N_Q_line*/ (float32_T) 6.0f /**< 6.0 m */,
   /**<k_stla_crit_zone_Q_QH_line*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_cta_pedestrian_min_size*/ (float32_T) 0.01,
   /**<k_cta_pedestrian_min_speed*/ (float32_T) 0.01,
   /**<k_cta_2wheel_min_size*/ (float32_T) 0.01,
   /**<k_cta_2wheel_min_speed*/ (float32_T) 0.01,
   /**<k_cta_min_deceleration_value*/ (float32_T) 0.0f /**< 0.0 m/s**2 */,
   /**<k_cta_max_deceleration_value*/ (float32_T) 10.0f /**< 10.0 m/s**2 */,
   /**<k_cta_f_adapt_intersect_lines_by_steering_angle*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_obj_heading*/ (boolean_T) 0,
   /**<k_cta_f_apply_heading_compensation_on_intersection_point*/ (boolean_T) 0,
   /**<k_cta_f_calc_ttc_ego_side_enabled*/ (boolean_T) 1,
   /**<k_cta_f_use_heading_for_relative_velocity_calculation*/ (boolean_T) 1,
   /**<k_cta_f_use_ghost_detector*/ (boolean_T) 1,
   /**<k_cta_f_use_object_min_object_age_in_cycles*/ (boolean_T) 0,
   /**<k_cta_f_enable_thres_crit_level_reset*/ (boolean_T) 1,
   /**<k_cta_f_use_front_corners_dist_stop*/ (boolean_T) 0 /**< 0 m */,
   /**<k_bmw_sp25_banner_criteria_check*/ (boolean_T) 0,
   /**<k_cta_f_stop_mode_ttp*/ (boolean_T) 0,
   /**<k_cta_enable_modes*/ {(uint8_t)1,(uint8_t)0},
   /**<k_cta_cycle_count_suppress_true_warning*/ (uint8_t) 3,
   /**<k_cta_cycle_count_hold_true_warning*/ (uint8_t) 3,
   /**<k_cta_min_object_age_check_valid*/ (uint8_t) 4,
   /**<k_cta_min_object_age_thres*/ (uint8_t) 3,
   /**<k_cta_cycles_coasted_to_ignore*/ (uint8_t) 6,
   /**<k_cta_object_supress_counter*/ (uint8_t) 5,
   /**<k_cta_min_mature_cycles_level_qualifiction*/ (uint8_t) 3,
   /**<k_cta_additional_qualification_mature_cycles*/ (uint8_t) 2,
   /**<k_cta_min_age_obj_outside_sensor_fov*/ (uint8_t) 10,
   /**<k_cta_min_qual_age_obj_crossing_paths*/ (uint8_t) 20
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_cta_min_qual_age_obj_crossing_paths*/ (uint8_t) 20,
   /**<k_cta_min_age_obj_outside_sensor_fov*/ (uint8_t) 10,
   /**<k_cta_additional_qualification_mature_cycles*/ (uint8_t) 2,
   /**<k_cta_min_mature_cycles_level_qualifiction*/ (uint8_t) 3,
   /**<k_cta_object_supress_counter*/ (uint8_t) 5,
   /**<k_cta_cycles_coasted_to_ignore*/ (uint8_t) 6,
   /**<k_cta_min_object_age_thres*/ (uint8_t) 3,
   /**<k_cta_min_object_age_check_valid*/ (uint8_t) 4,
   /**<k_cta_cycle_count_hold_true_warning*/ (uint8_t) 3,
   /**<k_cta_cycle_count_suppress_true_warning*/ (uint8_t) 3,
   /**<k_cta_enable_modes*/ {(uint8_t)1,(uint8_t)0},
   /**<k_cta_f_stop_mode_ttp*/ (boolean_T) 0,
   /**<k_bmw_sp25_banner_criteria_check*/ (boolean_T) 0,
   /**<k_cta_f_use_front_corners_dist_stop*/ (boolean_T) 0 /**< 0 m */,
   /**<k_cta_f_enable_thres_crit_level_reset*/ (boolean_T) 1,
   /**<k_cta_f_use_object_min_object_age_in_cycles*/ (boolean_T) 0,
   /**<k_cta_f_use_ghost_detector*/ (boolean_T) 1,
   /**<k_cta_f_use_heading_for_relative_velocity_calculation*/ (boolean_T) 1,
   /**<k_cta_f_calc_ttc_ego_side_enabled*/ (boolean_T) 1,
   /**<k_cta_f_apply_heading_compensation_on_intersection_point*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_obj_heading*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_steering_angle*/ (boolean_T) 0,
   /**<k_cta_max_deceleration_value*/ (float32_T) 10.0f /**< 10.0 m/s**2 */,
   /**<k_cta_min_deceleration_value*/ (float32_T) 0.0f /**< 0.0 m/s**2 */,
   /**<k_cta_2wheel_min_speed*/ (float32_T) 0.01,
   /**<k_cta_2wheel_min_size*/ (float32_T) 0.01,
   /**<k_cta_pedestrian_min_speed*/ (float32_T) 0.01,
   /**<k_cta_pedestrian_min_size*/ (float32_T) 0.01,
   /**<k_stla_crit_zone_Q_QH_line*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_stla_crit_zone_N_Q_line*/ (float32_T) 6.0f /**< 6.0 m */,
   /**<k_stla_crit_zone_D_C_line*/ (float32_T) 2.35f /**< 2.35 m */,
   /**<k_stla_crit_zone_G_E_line*/ (float32_T) 3.0f /**< 3.0 m */,
   /**<k_bmw_sp25_banner_time*/ (float32_T) 7.0f /**< 7.0 s */,
   /**<k_bmw_sp25_banner_criteria_check_time*/ (float32_T) 0.2f /**< 0.2 s */,
   /**<k_bmw_sp25_ego_abs_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 kph */,
   /**<k_cta_range_to_path_segment_ghost_qualif*/ (float32_T) 6.0,
   /**<k_cta_max_heading_variance*/ (float32_T) 3.0 /**< 3.0 rad^2 */,
   /**<k_cta_sensor_fov_border*/ {(float32_T)0.890118 /**< 0.890118 rad | 51.0 deg */,(float32_T)2.46091 /**< 2.46091 rad | 141.0 deg */},
   /**<k_ctb_min_ttp*/ (float32_T) 0.0 /**< 0.0 s */,
   /**<k_cta_intersection_line_host_width_percentage*/ (float32_T) 0,
   /**<k_cta_max_object_eclipse_for_level_qualification*/ (float32_T) 0.4,
   /**<k_cta_min_ttc_additional_mature_qualification*/ (float32_T) 3.5 /**< 3.5 s */,
   /**<k_cta_angles_zone_definition*/ {(float32_T)0.78f /**< 0.78 rad | 44.69 deg */,(float32_T)0.78f /**< 0.78 rad | 44.69 deg */},
   /**<k_cta_heading_range*/ {(float32_T)0.52f /**< 0.52 rad | 29.79 deg */,(float32_T)2.62f /**< 2.62 rad | 150.11 deg */},
   /**<k_cta_max_length_fov*/ (float32_T) 60 /**< 60 m */,
   /**<k_cta_max_speed*/ (float32_T) 100 /**< 100 m/s | 360.0 km/h */,
   /**<k_cta_min_lateral_approach_speed*/ (float32_T) 0.68 /**< 0.68 m/s | 2.45 km/h */,
   /**<k_cta_min_long_point_criticality_level*/ {{(float32_T)5.0 /**< 5.0 m */,(float32_T)5.0 /**< 5.0 m */},{(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */}},
   /**<k_cta_max_long_point_criticality_level*/ {{(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */},{(float32_T)5.0 /**< 5.0 m */,(float32_T)5.0 /**< 5.0 m */}},
   /**<k_cta_speed_criticality_level*/ {{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */},{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */}},
   /**<k_cta_ttc_criticality_level*/ {{(float32_T)3.15 /**< 3.15 s */,(float32_T)3.15 /**< 3.15 s */},{(float32_T)3.15 /**< 3.15 s */,(float32_T)3.15 /**< 3.15 s */}},
   /**<k_cta_butterfly_lat*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)50.0 /**< 50.0 m */,(float32_T)50.0 /**< 50.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_butterfly_long*/ {(float32_T)9.0 /**< 9.0 m */,(float32_T)-14.0 /**< -14.0 m */,(float32_T)-39.0 /**< -39.0 m */,(float32_T)34.0 /**< 34.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_min_speed*/ (float32_T) 1.1111 /**< 1.1111 m/s | 4.0 km/h */,
   /**<k_cta_stop_alert_ttp*/ (float32_T) -3.0 /**< -3.0 s */,
   /**<k_cta_stop_alert_ttc*/ (float32_T) 0.0 /**< 0.0 s */,
   /**<k_cta_ego_abs_speed_max*/ (float32_T) 5.0f /**< 5.0 m/s | 18.0 km/h */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)70,
   /**<Section_Size*/ (uint32_t)12
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Cta_Public_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Cta_Public_Cal_Reverse_Array_Cta_Cal(Cta_Public_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_butterfly_long[0], sizeof(cal_dst->k_cta_butterfly_long), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_butterfly_lat[0], sizeof(cal_dst->k_cta_butterfly_lat), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_ttc_criticality_level[0], sizeof(cal_dst->k_cta_ttc_criticality_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_speed_criticality_level[0], sizeof(cal_dst->k_cta_speed_criticality_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_max_long_point_criticality_level[0], sizeof(cal_dst->k_cta_max_long_point_criticality_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_min_long_point_criticality_level[0], sizeof(cal_dst->k_cta_min_long_point_criticality_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_heading_range[0], sizeof(cal_dst->k_cta_heading_range), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_angles_zone_definition[0], sizeof(cal_dst->k_cta_angles_zone_definition), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_sensor_fov_border[0], sizeof(cal_dst->k_cta_sensor_fov_border), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_enable_modes[0], sizeof(cal_dst->k_cta_enable_modes), CT_ONE_BYTE);
}
#endif /* CT_BIG_ENDIAN */


