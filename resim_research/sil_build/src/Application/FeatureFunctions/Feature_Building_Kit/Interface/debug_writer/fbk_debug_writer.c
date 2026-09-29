/**
 * @file fbk_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for Fbk bin writer functions.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_debug_writer.h"
#include "fbk_debug_interface.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Fbk_Write_Bin_File(void)
{
   uint8_t idx;

   /* Get debug data. */
   Fbk_Debug_Data_T *fbk_debug_data         = Fbk_Get_Debug_Data();
   const Fbk_Vehicle_Data_T *p_vehicle_data = &(fbk_debug_data->fbk_vehicle_data);

   /* Check input parameters. */
   assert(NULL != fbk_debug_data);

   FBK_STORE_VAL_MGR_WPR("Fbk_Sw_Major_Version", fbk_debug_data->fbk_version.fbk_sw_major_version);
   FBK_STORE_VAL_MGR_WPR("Fbk_Sw_Minor_Version", fbk_debug_data->fbk_version.fbk_sw_minor_version);
   FBK_STORE_VAL_MGR_WPR("sfl_status", fbk_debug_data->fbk_debug_output.sfl_status);

   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT; idx++)
   {
      FBK_STORE_ARRAY_ELEM_MGR_WPR("fbk_idx_lut", fbk_debug_data->fbk_debug_output.fbk_idx_lut[idx], idx);
   }

   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      FBK_STORE_ARRAY_ELEM_MGR_WPR("fbk_stage_age", fbk_debug_data->fbk_debug_output.fbk_stage_age[idx], idx);
      FBK_STORE_ARRAY_ELEM_MGR_WPR("fbk_stage", fbk_debug_data->fbk_debug_output.fbk_stage[idx], idx);
   }

   FBK_STORE_VAL_MGR_WPR("f_trail_full_buffer", fbk_debug_data->fbk_host_trail.f_trail_full_buffer);
   FBK_STORE_VAL_MGR_WPR("host_trail_diff_dist", fbk_debug_data->fbk_host_trail.trail_diff_dist);
   FBK_STORE_VAL_MGR_WPR("host_trail_diff_heading", fbk_debug_data->fbk_host_trail.trail_diff_heading);
   FBK_STORE_VAL_MGR_WPR("host_trail_host_dist", fbk_debug_data->fbk_host_trail.trail_host_dist);
   FBK_STORE_VAL_MGR_WPR("host_trail_host_heading", fbk_debug_data->fbk_host_trail.trail_host_heading.angle);
   FBK_STORE_VAL_MGR_WPR("host_trail_cos_host_heading", fbk_debug_data->fbk_host_trail.trail_host_heading.cos);
   FBK_STORE_VAL_MGR_WPR("host_trail_sin_host_heading", fbk_debug_data->fbk_host_trail.trail_host_heading.sin);
   FBK_STORE_VAL_MGR_WPR("host_trail_host_x", fbk_debug_data->fbk_host_trail.trail_host_position.x);
   FBK_STORE_VAL_MGR_WPR("host_trail_host_y", fbk_debug_data->fbk_host_trail.trail_host_position.y);
   FBK_STORE_VAL_MGR_WPR("host_trail_index", fbk_debug_data->fbk_host_trail.trail_index);
   FBK_STORE_VAL_MGR_WPR("host_trail_oldest_index", fbk_debug_data->fbk_host_trail.oldest_trail_index);

   for (idx = FBK_ZERO_UINT; idx < FBK_NUM_HOST_TRAIL_POINTS; idx++)
   {
      FBK_STORE_ARRAY_ELEM_MGR_WPR("host_trail_dist", fbk_debug_data->fbk_host_trail.segments[idx].distance_traveled, idx);
      FBK_STORE_ARRAY_ELEM_MGR_WPR("host_trail_heading", fbk_debug_data->fbk_host_trail.segments[idx].heading, idx);
      FBK_STORE_ARRAY_ELEM_MGR_WPR("host_trail_x", fbk_debug_data->fbk_host_trail.segments[idx].point.x, idx);
      FBK_STORE_ARRAY_ELEM_MGR_WPR("host_trail_y", fbk_debug_data->fbk_host_trail.segments[idx].point.y, idx);
   }

   /* Log object related inputs */
   STORE_VAL_MGR_WPR(As_Bww_Tracker_String, "pa_time_diff_to_last_cycle", fbk_debug_data->fbk_debug_output.pa_time_diff_to_last_cycle);

   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      const Fbk_Object_Data_T *p_object_data = &(fbk_debug_data->fbk_object_data[idx]);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_id", p_object_data->id, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_unique_id", p_object_data->unique_id, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_status", p_object_data->status, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_age", p_object_data->age, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_stage_age", p_object_data->stage_age, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_fbk_stage_age", p_object_data->fbk_stage_age, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_existence_probability", p_object_data->existence_probability, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_vcs_long_pos", p_object_data->vcs_pos.x, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_vcs_long_vel", p_object_data->vcs_vel.x, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_vcs_long_vel_rel", p_object_data->vcs_vel_rel.x, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_vcs_long_accel", p_object_data->vcs_accel.x, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_vcs_lat_pos", p_object_data->vcs_pos.y, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_vcs_lat_vel", p_object_data->vcs_vel.y, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_vcs_lat_vel_rel", p_object_data->vcs_vel_rel.y, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_vcs_lat_accel", p_object_data->vcs_accel.y, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_heading", p_object_data->vcs_heading, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_heading_rate", p_object_data->heading_rate, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_heading_variance", p_object_data->heading_variance, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_accuracy_heading", p_object_data->accuracy_heading, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_speed", p_object_data->speed, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_eclipse_value", p_object_data->eclipse_value, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_length", p_object_data->length, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_width", p_object_data->width, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_object_distance", p_object_data->obj_distance, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_obstruction_prob", p_object_data->obstruction_prob, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_reflection_flag", p_object_data->f_reflection, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_class", p_object_data->obj_class, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_class_prob_pedestrian", p_object_data->class_prob_pedestrian, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_class_prob_2wheel", p_object_data->class_prob_2wheel, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_class_prob_car", p_object_data->class_prob_car, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_class_prob_truck", p_object_data->class_prob_truck, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_id_merged_obj", p_object_data->id_merged_obj, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_merge_occured", p_object_data->f_merge_occured, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_curvi_coordinates_calc_method",
                               p_object_data->curvi_coordinates_calc_method, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_curvi_long_posn", p_object_data->curvi_pos.x, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_curvi_long_vel", p_object_data->curvi_vel.x, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_curvi_long_vel_rel", p_object_data->curvi_vel_rel.x, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_curvi_lat_posn", p_object_data->curvi_pos.y, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_curvi_lat_vel", p_object_data->curvi_vel.y, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_curvi_lat_vel_rel", p_object_data->curvi_vel_rel.y, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_curvi_heading", p_object_data->curvi_heading, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_is_fl_origin_sensor", p_object_data->f_is_fl_origin_sensor, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_is_fr_origin_sensor", p_object_data->f_is_fr_origin_sensor, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_is_rl_origin_sensor", p_object_data->f_is_rl_origin_sensor, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_is_rr_origin_sensor", p_object_data->f_is_rr_origin_sensor, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_is_in_fl_sensor_fov", p_object_data->f_is_in_fl_sensor_fov, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_is_in_fr_sensor_fov", p_object_data->f_is_in_fr_sensor_fov, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_is_in_rl_sensor_fov", p_object_data->f_is_in_rl_sensor_fov, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_is_in_rr_sensor_fov", p_object_data->f_is_in_rr_sensor_fov, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_obj_stationary", p_object_data->f_stationary, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_f_obj_moveable", p_object_data->f_moveable, idx);
   }

   /* Log guardrail related inputs */
   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_GUARDRAILS; idx++)
   {
      const Fbk_Guardrail_Data_T *p_guardrail_data = &(fbk_debug_data->fbk_guardrail_data[idx]);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_guardrail_active_flag", p_guardrail_data->f_active, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_guardrail_present_flag", p_guardrail_data->f_present, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_guardrail_status", p_guardrail_data->status, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_guardrail_lateral_position", p_guardrail_data->lat_pos, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_guardrail_existence_probability",
                               p_guardrail_data->existence_probability, idx);
      STORE_ARRAY_ELEM_MGR_WPR(As_Bww_Tracker_String, "pa_guardrail_age", p_guardrail_data->age, idx);
   }

   /* Log vehicle related inputs */
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_host_length", p_vehicle_data->host_length);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_host_width", p_vehicle_data->host_width);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_rear_axle_position", p_vehicle_data->rear_axle_position);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_wheelbase", p_vehicle_data->wheelbase);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_host_speed", p_vehicle_data->host_speed);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_steering_angle", p_vehicle_data->steering_angle);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_yawrate", p_vehicle_data->yawrate);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_long_vel", p_vehicle_data->long_vel);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_long_acc", p_vehicle_data->long_acc);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_lat_acc", p_vehicle_data->lat_acc);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_prndl", p_vehicle_data->prndl);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_lane_width", p_vehicle_data->lane_width);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_lane_center_offset", p_vehicle_data->lane_center_offset);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_turn_signal", p_vehicle_data->turn_signal);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_curvature", p_vehicle_data->curvature);
   STORE_VAL_MGR_WPR(As_Bww_Vehicle_String, "pa_reverse_flag", p_vehicle_data->f_reverse);
}


#endif /* BINARY DEBUG */
