/**
 * @file recw_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "recw_debug_writer.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "recw_debug_interface.h"
#include "recw_types.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Recw_Write_Bin_File(void)
{
   uint8_t index;

   /* Get debug data. */
   Recw_Debug_Data_T *recw_debug_data = Recw_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != recw_debug_data);

   /* Log the RECW core input. */
   RECW_STORE_VAL_MGR_WPR("RECW_core_input_f_enable_recw", recw_debug_data->recw_core_input.f_enable_recw);

   /* Log the RECW core output. */
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_alert_level", recw_debug_data->recw_core_output.recw_alert_level);
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_id", recw_debug_data->recw_core_output.recw_id);
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_index", recw_debug_data->recw_core_output.recw_index);
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_ttc", recw_debug_data->recw_core_output.recw_ttc);
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_crash_prob_combined", recw_debug_data->recw_core_output.recw_crash_prob_combined);
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_crash_prob_braking", recw_debug_data->recw_core_output.recw_crash_prob_braking);
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_crash_prob_steering", recw_debug_data->recw_core_output.recw_crash_prob_steering);
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_ttc_alert_level_1_threshold",
                          recw_debug_data->recw_core_output.ttc_threshold_alert_level_1);
   RECW_STORE_VAL_MGR_WPR("RECW_core_output_ttc_alert_level_2_threshold",
                          recw_debug_data->recw_core_output.ttc_threshold_alert_level_2);

   /* Log the RECW persistent data. */
   RECW_STORE_VAL_MGR_WPR("RECW_persistent_f_host_speed_in_allowed_range",
                          recw_debug_data->recw_persistent.f_host_speed_in_allowed_range);
   RECW_STORE_VAL_MGR_WPR("RECW_persistent_f_is_rear_blocked", recw_debug_data->recw_persistent.f_is_rear_blocked);
   RECW_STORE_VAL_MGR_WPR("RECW_persistent_recw_rear_blockage_object_index",
                          recw_debug_data->recw_persistent.recw_rear_blockage_object_index);
   RECW_STORE_VAL_MGR_WPR("RECW_persistent_rear_blockage_qualifying_counter",
                          recw_debug_data->recw_persistent.recw_rear_blockage_qualifying_counter);
   RECW_STORE_VAL_MGR_WPR("RECW_persistent_recw_alert_qualifying_counter",
                          recw_debug_data->recw_persistent.recw_alert_qualifying_counter);
   RECW_STORE_VAL_MGR_WPR("RECW_persistent_recw_alert_holding_counter", recw_debug_data->recw_persistent.recw_alert_holding_counter);
   RECW_STORE_VAL_MGR_WPR("RECW_persistent_recw_alert_duration_counter", recw_debug_data->recw_persistent.recw_alert_duration_counter);
   RECW_STORE_VAL_MGR_WPR("RECW_persistent_recw_ttc_value_hold", recw_debug_data->recw_persistent.recw_ttc_value_hold);

   for (index = 0; index < RECW_MAX_ID_ARRAY_SIZE; index++)
   {
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_persistent_obj_internal_age", recw_debug_data->recw_persistent.object_data[index].age,
                                    index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_persistent_obj_consecutive_min_crash_prob_counter",
                                    recw_debug_data->recw_persistent.object_data[index].consecutive_min_crash_prob_counter, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_persistent_obj_object_within_lane_counter",
                                    recw_debug_data->recw_persistent.object_data[index].object_within_lane_counter, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_persistent_obj_last_object_pos_x",
                                    recw_debug_data->recw_persistent.object_data[index].last_object_pos.x, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_persistent_obj_last_object_pos_y",
                                    recw_debug_data->recw_persistent.object_data[index].last_object_pos.y, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_persistent_obj_last_object_pos_filtered_x",
                                    recw_debug_data->recw_persistent.object_data[index].last_object_pos_filtered.x, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_persistent_obj_last_object_pos_filtered_y",
                                    recw_debug_data->recw_persistent.object_data[index].last_object_pos_filtered.y, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_persistent_obj_filter_counter",
                                    recw_debug_data->recw_persistent.object_data[index].filter_counter, index);
   }

   /* Log the RECW cals & SW versions */
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_cal_version", recw_debug_data->recw_calibration.Header.version);
   RECW_STORE_VAL_MGR_WPR("Recw_Sw_Major_Version", recw_debug_data->recw_version.recw_sw_major_version);
   RECW_STORE_VAL_MGR_WPR("Recw_Sw_Minor_Version", recw_debug_data->recw_version.recw_sw_minor_version);

   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_lane_filter_width", recw_debug_data->recw_calibration.k_recw_lane_filter_width);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_lane_filter_width_hys", recw_debug_data->recw_calibration.k_recw_lane_filter_width_hys);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_lane_width_slope", recw_debug_data->recw_calibration.k_recw_lane_width_slope);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_lane_filter_max_abs_ego_speed_vcs_coord",
                          recw_debug_data->recw_calibration.k_recw_lane_filter_max_abs_ego_speed_vcs_coord);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_lane_filter_num_consecutive_cycles",
                          recw_debug_data->recw_calibration.k_recw_lane_filter_num_consecutive_cycles);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_f_apply_lane_filter", recw_debug_data->recw_calibration.k_recw_f_apply_lane_filter);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_f_enable_recb", recw_debug_data->recw_calibration.k_recb_f_enable_recb);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_qualifier_nominal_acceleration",
                          recw_debug_data->recw_calibration.k_recb_qualifier_nominal_acceleration);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_integrity", recw_debug_data->recw_calibration.k_recb_integrity);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_ssm_braking", recw_debug_data->recw_calibration.k_recb_ssm_braking);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_ssm_request_cancelled", recw_debug_data->recw_calibration.k_recb_ssm_request_cancelled);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_min_cycles_host_standstill",
                          recw_debug_data->recw_calibration.k_recb_min_cycles_host_standstill);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_max_cycles_braking_request_duration",
                          recw_debug_data->recw_calibration.k_recb_max_cycles_braking_request_duration);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_allowed_consecutive_coasted_cycles",
                          recw_debug_data->recw_calibration.k_recw_max_allowed_consecutive_coasted_cycles);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_f_enable_traffic_light_ghost_detection",
                          recw_debug_data->recw_calibration.k_recw_f_enable_traffic_light_ghost_detection);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_f_enable_heading_filter", recw_debug_data->recw_calibration.k_recw_f_enable_heading_filter);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_en_active_car_wash_logic", recw_debug_data->recw_calibration.k_recw_en_active_car_wash_logic);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_cycles_with_min_crash_prob",
                          recw_debug_data->recw_calibration.k_recw_min_cycles_with_min_crash_prob);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_object_age", recw_debug_data->recw_calibration.k_recw_min_object_age);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_age_for_close_slow_targets",
                          recw_debug_data->recw_calibration.k_recw_min_age_for_close_slow_targets);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_alert_qualifying_cycles", recw_debug_data->recw_calibration.k_recw_alert_qualifying_cycles);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_f_only_allow_consecutive_alert_levels",
                          recw_debug_data->recw_calibration.k_recw_f_only_allow_consecutive_alert_levels);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_f_make_use_of_guardrail", recw_debug_data->recw_calibration.k_recw_f_make_use_of_guardrail);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_nominal_acceleration_applied",
                          recw_debug_data->recw_calibration.k_recb_nominal_acceleration_applied);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_max_ttc", recw_debug_data->recw_calibration.k_recb_max_ttc);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_min_rel_velocity_hys", recw_debug_data->recw_calibration.k_recb_min_rel_velocity_hys);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_min_rel_velocity", recw_debug_data->recw_calibration.k_recb_min_rel_velocity);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_max_host_speed_hys", recw_debug_data->recw_calibration.k_recb_max_host_speed_hys);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_max_host_speed", recw_debug_data->recw_calibration.k_recb_max_host_speed);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_max_velocity_host_standstill",
                          recw_debug_data->recw_calibration.k_recb_max_velocity_host_standstill);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recb_accelerator_pedal_gradient_threshold",
                          recw_debug_data->recw_calibration.k_recb_accelerator_pedal_gradient_threshold);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_speed_not_stationary", recw_debug_data->recw_calibration.k_recw_min_speed_not_stationary);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_heading_accuracy_threshold",
                          recw_debug_data->recw_calibration.k_recw_heading_accuracy_threshold);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_rear_blockage_qualifying_cycles",
                          recw_debug_data->recw_calibration.k_recw_rear_blockage_qualifying_cycles);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_rear_blockage_ego_speed_threshold",
                          recw_debug_data->recw_calibration.k_recw_rear_blockage_ego_speed_threshold);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_rear_blockage_length", recw_debug_data->recw_calibration.k_recw_rear_blockage_length);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_rear_blockage_width", recw_debug_data->recw_calibration.k_recw_rear_blockage_width);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_rear_blockage_speed_threshold",
                          recw_debug_data->recw_calibration.k_recw_rear_blockage_speed_threshold);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_allowed_heading_diff", recw_debug_data->recw_calibration.k_recw_max_allowed_heading_diff);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_allowed_rel_vel_lat_diff",
                          recw_debug_data->recw_calibration.k_recw_max_allowed_rel_vel_lat_diff);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_allowed_rel_vel_long_diff",
                          recw_debug_data->recw_calibration.k_recw_max_allowed_rel_vel_long_diff);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_object_width_warn_on", recw_debug_data->recw_calibration.k_recw_max_object_width_warn_on);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_eclipse_value_for_valid_object",
                          recw_debug_data->recw_calibration.k_recw_max_eclipse_value_for_valid_object);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_speed_ego_car_wash", recw_debug_data->recw_calibration.k_recw_max_speed_ego_car_wash);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_rel_lon_vel_car_wash", recw_debug_data->recw_calibration.k_recw_min_rel_lon_vel_car_wash);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_lat_distance_car_wash", recw_debug_data->recw_calibration.k_recw_max_lat_distance_car_wash);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_lon_distance_car_wash", recw_debug_data->recw_calibration.k_recw_max_lon_distance_car_wash);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_f_suppress_alert_lvl_2_for_pedestrian",
                          recw_debug_data->recw_calibration.k_recw_f_suppress_alert_lvl_2_for_pedestrian);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_rel_lon_vel_release_car_wash",
                          recw_debug_data->recw_calibration.k_recw_max_rel_lon_vel_release_car_wash);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_rel_velocity_hys", recw_debug_data->recw_calibration.k_recw_max_rel_velocity_hys);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_abs_speed_for_young_close_targets",
                          recw_debug_data->recw_calibration.k_recw_min_abs_speed_for_young_close_targets);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_dist_for_young_slow_targets",
                          recw_debug_data->recw_calibration.k_recw_min_dist_for_young_slow_targets);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_host_speed_hys", recw_debug_data->recw_calibration.k_recw_max_host_speed_hys);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_host_speed_hys", recw_debug_data->recw_calibration.k_recw_min_host_speed_hys);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_average_sensor_latency", recw_debug_data->recw_calibration.k_recw_average_sensor_latency);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_factor_ego_width", recw_debug_data->recw_calibration.k_recw_factor_ego_width);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_max_heading_hys", recw_debug_data->recw_calibration.k_recw_max_heading_hys);
   RECW_STORE_VAL_MGR_WPR("RECW_k_recw_min_existence_prob_hys", recw_debug_data->recw_calibration.k_recw_min_existence_prob_hys);

   for (index = FBK_ZERO_UINT; index < (uint8_t) RECW_NUMBER_ALERT_LEVEL; index++)
   {
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_max_ttc_threshold",
                                    recw_debug_data->recw_calibration.k_recw_max_ttc_threshold[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_rel_velocity_for_max_ttc_threshold",
                                    recw_debug_data->recw_calibration.k_recw_min_rel_velocity_for_max_ttc_threshold[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_max_cycles_alert_duration",
                                    recw_debug_data->recw_calibration.k_recw_max_cycles_alert_duration[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_f_use_rear_blockage",
                                    recw_debug_data->recw_calibration.k_recw_f_use_rear_blockage[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_f_allow_alert_on_coasted_objects",
                                    recw_debug_data->recw_calibration.k_recw_f_allow_alert_on_coasted_objects[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_stage_age_for_alert_level",
                                    recw_debug_data->recw_calibration.k_recw_min_stage_age_for_alert_level[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_alert_holding_cycles",
                                    recw_debug_data->recw_calibration.k_recw_alert_holding_cycles[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_ttc_for_alert_level",
                                    recw_debug_data->recw_calibration.k_recw_min_ttc_for_alert_level[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_existence_prob",
                                    recw_debug_data->recw_calibration.k_recw_min_existence_prob[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_crash_prob", recw_debug_data->recw_calibration.k_recw_min_crash_prob[index],
                                    index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_rel_velocity_hys",
                                    recw_debug_data->recw_calibration.k_recw_min_rel_velocity_hys[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_rel_velocity",
                                    recw_debug_data->recw_calibration.k_recw_min_rel_velocity[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_overlap_for_alert_level",
                                    recw_debug_data->recw_calibration.k_recw_min_overlap_for_alert_level[index], index);
   }

   for (index = FBK_ZERO_UINT; index <= (uint8_t) RECW_NUMBER_ALERT_LEVEL; index++)
   {
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_max_heading", recw_debug_data->recw_calibration.k_recw_max_heading[index], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_min_host_speed", recw_debug_data->recw_calibration.k_recw_min_host_speed[index],
                                    index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_max_host_speed", recw_debug_data->recw_calibration.k_recw_max_host_speed[index],
                                    index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_k_recw_max_rel_velocity",
                                    recw_debug_data->recw_calibration.k_recw_max_rel_velocity[index], index);
   }

   /* write object attributes for all objects */
   for (index = 0; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_crash_prob_braking",
                                    recw_debug_data->recw_object_attributes[index].crash_prob_braking, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_crash_prob_steering",
                                    recw_debug_data->recw_object_attributes[index].crash_prob_steering, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_crash_prob_combined",
                                    recw_debug_data->recw_object_attributes[index].crash_prob_combined, index);

      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_needed_brake_acceleration",
                                    recw_debug_data->recw_object_attributes[index].needed_brake_acceleration, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_needed_steering_acceleration",
                                    recw_debug_data->recw_object_attributes[index].needed_steering_acceleration, index);

      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_filtered_diff_pos_x",
                                    recw_debug_data->recw_object_attributes[index].filtered_diff_pos.x, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_filtered_diff_pos_y",
                                    recw_debug_data->recw_object_attributes[index].filtered_diff_pos.y, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_effective_rel_vel_x",
                                    recw_debug_data->recw_object_attributes[index].effective_rel_vel.x, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_effective_rel_vel_y",
                                    recw_debug_data->recw_object_attributes[index].effective_rel_vel.y, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_filtered_heading", recw_debug_data->recw_object_attributes[index].filtered_heading,
                                    index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_overlap", recw_debug_data->recw_object_attributes[index].overlap, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_overlap_line_y_min",
                                    recw_debug_data->recw_object_attributes[index].overlap_line_y_min, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_overlap_line_y_max",
                                    recw_debug_data->recw_object_attributes[index].overlap_line_y_max, index);

      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_ttc", recw_debug_data->recw_object_attributes[index].ttc, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_ttc_alert_level_1_threshold",
                                    recw_debug_data->recw_object_attributes[index].ttc_threshold[RECW_INDEX_ALERT_LEVEL_1], index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_ttc_alert_level_2_threshold",
                                    recw_debug_data->recw_object_attributes[index].ttc_threshold[RECW_INDEX_ALERT_LEVEL_2], index);

      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_alert_level", recw_debug_data->recw_object_attributes[index].alert_level, index);

      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_f_object_is_car_wash_ghost",
                                    recw_debug_data->recw_object_attributes[index].f_object_is_car_wash_ghost, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_obj_f_obj_is_within_lane",
                                    recw_debug_data->recw_object_attributes[index].f_obj_is_within_lane, index);
   }

   /* Write lane filter zones. */
   for (index = 0; index < RECW_DEBUG_NUMBER_OF_LANE_ZONE_POINTS; index++)
   {
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_lane_filter_zone_x", recw_debug_data->recw_debug_output.lane_filter_zone.points[index].x,
                                    index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_lane_filter_zone_y", recw_debug_data->recw_debug_output.lane_filter_zone.points[index].y,
                                    index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_lane_filter_zone_hys_x",
                                    recw_debug_data->recw_debug_output.lane_filter_zone_hys.points[index].x, index);
      RECW_STORE_ARRAY_ELEM_MGR_WPR("RECW_lane_filter_zone_hys_y",
                                    recw_debug_data->recw_debug_output.lane_filter_zone_hys.points[index].y, index);
   }
}

#endif /* BINARY_DEBUG */
