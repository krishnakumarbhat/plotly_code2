/**
 * @file ta_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_bmw_sp25_debug_writer.h"
#include "pa_reuse.h"
#include "ta_bmw_sp25_debug_interface.h"
#include "ta_debug_writer.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Ta_Bmw_Sp25_Write_Bin_File(void)
{
   /* Get debug data. */
   Ta_Bmw_Sp25_Debug_Data_T *ta_debug_data = Ta_Get_Bmw_Sp25_Debug_Data();

   /* Check input parameters. */
   assert(NULL != ta_debug_data);

   /* TA Debug Output */
   TA_STORE_VAL_MGR_WPR("ta_current_deceleration_estimate", ta_debug_data->ta_output.ta_current_deceleration_estimate);

   /* Persistent BMW specific data. */
   TA_STORE_VAL_MGR_WPR("Ta_Host_Speed_At_Brake_Start", ta_debug_data->ta_debug_output.ta_host_speed_at_brake_start);
   TA_STORE_VAL_MGR_WPR("Ta_Host_Speed_Reduction_Requested", ta_debug_data->ta_debug_output.ta_host_speed_reduction_requested);
   TA_STORE_VAL_MGR_WPR("Ta_Host_Speed_Reduction_Achieved", ta_debug_data->ta_debug_output.ta_host_speed_reduction_achieved);
   TA_STORE_VAL_MGR_WPR("Pfgs_Qualification_Counter_Min", ta_debug_data->ta_debug_output.pfgs_qualification_counter_min);

   /* Set debug information with some logic stuff */
   TA_STORE_VAL_MGR_WPR("f_pfgs_relevant_host_speed", ta_debug_data->ta_debug_output.f_pfgs_relevant_host_speed);
   TA_STORE_VAL_MGR_WPR("ta_core_maneuver", ta_debug_data->ta_debug_output.ta_core_maneuver);
   TA_STORE_VAL_MGR_WPR("ta_lookup_turning_host_curvature_threshold",
                        ta_debug_data->ta_debug_output.ta_lookup_turning_host_curvature_threshold);

   /* FTA Output */
   TA_STORE_VAL_MGR_WPR("f_fta_enable", ta_debug_data->ta_output.f_fta_enable);
   TA_STORE_VAL_MGR_WPR("fta_brake_deceleration_request", ta_debug_data->ta_output.fta_brake_deceleration_request);
   TA_STORE_VAL_MGR_WPR("fta_target_gap", ta_debug_data->ta_output.fta_target_gap);
   TA_STORE_VAL_MGR_WPR("fta_ttc", ta_debug_data->ta_output.fta_ttc);
   TA_STORE_VAL_MGR_WPR("fta_alert_level", ta_debug_data->ta_output.fta_alert_level);
   TA_STORE_VAL_MGR_WPR("fta_symbol_request", ta_debug_data->ta_output.fta_symbol_request);
   TA_STORE_VAL_MGR_WPR("fta_brake_threshold_reduction", ta_debug_data->ta_output.fta_brake_threshold_reduction);
   TA_STORE_VAL_MGR_WPR("fta_brake_conditioning", ta_debug_data->ta_output.fta_brake_conditioning);
   TA_STORE_VAL_MGR_WPR("fta_target_id", ta_debug_data->ta_output.fta_target_id);
   TA_STORE_VAL_MGR_WPR("f_diagnostic_mode", ta_debug_data->ta_output.f_diagnostic_mode);
   TA_STORE_VAL_MGR_WPR("Pfgs_Qualification_Counter", ta_debug_data->ta_debug_output.pfgs_qualification_counter);


   TA_STORE_VAL_MGR_WPR("fta_obj_list_rcs_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_rcs);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_id_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_id);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_age_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_age);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_meas_status_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_meas_status);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_move_status_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_move_status);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_exist_prob_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_exist_prob);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_ref_point_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_ref_point);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_ref_pnt_long_posn_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_ref_pnt_long_posn);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_ref_pnt_long_posn_std_dev_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_ref_pnt_long_posn_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_ref_pnt_lat_posn_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_ref_pnt_lat_posn);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_ref_pnt_lat_posn_std_dev_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_ref_pnt_lat_posn_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_covariance_posn_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_covariance_posn);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_yaw_angle_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_yaw_angle);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_yaw_angle_std_dev_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_yaw_angle_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_long_vel_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_long_vel);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_long_vel_std_dev_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_long_vel_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_lat_vel_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_lat_vel);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_lat_vel_std_dev_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_lat_vel_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_covariance_vel_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_covariance_vel);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_long_accel_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_long_accel);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_long_accel_std_dev_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_long_accel_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_lat_accel_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_lat_accel);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_lat_accel_std_dev_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_lat_accel_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_covariance_accel_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_covariance_accel);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_yawrate_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_yawrate);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_yawrate_std_dev_0",
                        ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_yawrate_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_length_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_length);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_length_std_dev_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_length_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_width_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_width);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_width_std_dev_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_width_std_dev);
   TA_STORE_VAL_MGR_WPR("fta_obj_list_object_class_0", ta_debug_data->ta_output.fta_relevant_object[0].fta_obj_list_object_class);

   /* RTA Output */
   TA_STORE_VAL_MGR_WPR("f_rta_enable", ta_debug_data->ta_output.f_rta_enable);
   TA_STORE_VAL_MGR_WPR("rta_id_left", ta_debug_data->ta_output.rta_id_left);
   TA_STORE_VAL_MGR_WPR("rta_id_right", ta_debug_data->ta_output.rta_id_right);
   TA_STORE_VAL_MGR_WPR("rta_alert_left", ta_debug_data->ta_output.rta_alert_left);
   TA_STORE_VAL_MGR_WPR("rta_alert_right", ta_debug_data->ta_output.rta_alert_right);
   TA_STORE_VAL_MGR_WPR("rta_ttc_left", ta_debug_data->ta_output.rta_ttc_left);
   TA_STORE_VAL_MGR_WPR("rta_ttc_right", ta_debug_data->ta_output.rta_ttc_right);
   TA_STORE_VAL_MGR_WPR("rta_lat_posn_left", ta_debug_data->ta_output.rta_lat_posn_left);
   TA_STORE_VAL_MGR_WPR("rta_long_posn_right", ta_debug_data->ta_output.rta_long_posn_right);
   TA_STORE_VAL_MGR_WPR("rta_lat_posn_right", ta_debug_data->ta_output.rta_lat_posn_right);
   TA_STORE_VAL_MGR_WPR("rta_long_vel_left", ta_debug_data->ta_output.rta_long_vel_left);
   TA_STORE_VAL_MGR_WPR("rta_lat_vel_left", ta_debug_data->ta_output.rta_lat_vel_left);
   TA_STORE_VAL_MGR_WPR("rta_long_vel_right", ta_debug_data->ta_output.rta_long_vel_right);
   TA_STORE_VAL_MGR_WPR("rta_lat_vel_right", ta_debug_data->ta_output.rta_lat_vel_right);
   TA_STORE_VAL_MGR_WPR("rta_existence_probability_left", ta_debug_data->ta_output.rta_existence_probability_left);
   TA_STORE_VAL_MGR_WPR("rta_existence_probability_right", ta_debug_data->ta_output.rta_existence_probability_right);

   TA_STORE_VAL_MGR_WPR("rta_dynamic_area_status", ta_debug_data->ta_output.rta_dynamic_area_status);
   TA_STORE_VAL_MGR_WPR("rta_turning_area_status", ta_debug_data->ta_output.rta_turning_area_status);
}

#endif /* BINARY_DEBUG */
