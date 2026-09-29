/**
 * @file scw_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

#include "scw_debug_writer.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include "scw_debug_interface.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Scw_Write_Bin_File(void)
{
   uint8_t i;

   /* Get debug data. */
   Scw_Debug_Data_T *scw_debug_data = Scw_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != scw_debug_data);

   /* Log the SCW core input. */
   SCW_STORE_VAL_MGR_WPR("scw_f_enable", scw_debug_data->scw_core_input.f_scw_enable);
   SCW_STORE_VAL_MGR_WPR("scw_f_enable_dynamic", scw_debug_data->scw_core_input.f_scw_enable_dynamic);
   SCW_STORE_VAL_MGR_WPR("scw_f_enable_guardrail", scw_debug_data->scw_core_input.f_scw_enable_guardrail);
   for (i = FBK_ZERO_UINT; i < PA_OBJ_NUMBER_OF_GUARDRAILS; i++)
   {
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_core_input_radar_guardrail_type",
                                   scw_debug_data->scw_core_input.guardrail_data[i].radar.type, i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_core_input_radar_guardrail_lat_position",
                                   scw_debug_data->scw_core_input.guardrail_data[i].radar.lateral_position, i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_core_input_radar_guardrail_confidence",
                                   scw_debug_data->scw_core_input.guardrail_data[i].radar.confidence, i);
   }

   /* Log the SCW core output. */
   SCW_STORE_VAL_MGR_WPR("scw_core_output_alert_level_left", scw_debug_data->scw_core_output.alert_level[FBK_SIDE_LEFT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_type_left", scw_debug_data->scw_core_output.obj_type[FBK_SIDE_LEFT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_id_left", scw_debug_data->scw_core_output.obj_id[FBK_SIDE_LEFT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_index_left", scw_debug_data->scw_core_output.obj_index[FBK_SIDE_LEFT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_lat_distance_left", scw_debug_data->scw_core_output.obj_lateral_distance[FBK_SIDE_LEFT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_lat_ttc_left", scw_debug_data->scw_core_output.obj_lateral_ttc[FBK_SIDE_LEFT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_ttle_left", scw_debug_data->scw_core_output.obj_ttle[FBK_SIDE_LEFT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_ttp_left", scw_debug_data->scw_core_output.obj_ttp[FBK_SIDE_LEFT]);

   SCW_STORE_VAL_MGR_WPR("scw_core_output_alert_level_right", scw_debug_data->scw_core_output.alert_level[FBK_SIDE_RIGHT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_type_right", scw_debug_data->scw_core_output.obj_type[FBK_SIDE_RIGHT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_id_right", scw_debug_data->scw_core_output.obj_id[FBK_SIDE_RIGHT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_index_right", scw_debug_data->scw_core_output.obj_index[FBK_SIDE_RIGHT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_lat_distance_right",
                         scw_debug_data->scw_core_output.obj_lateral_distance[FBK_SIDE_RIGHT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_lat_ttc_right", scw_debug_data->scw_core_output.obj_lateral_ttc[FBK_SIDE_RIGHT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_ttle_right", scw_debug_data->scw_core_output.obj_ttle[FBK_SIDE_RIGHT]);
   SCW_STORE_VAL_MGR_WPR("scw_core_output_obj_ttp_right", scw_debug_data->scw_core_output.obj_ttp[FBK_SIDE_RIGHT]);

   for (i = FBK_ZERO_UINT; i < (PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT); i++)
   {
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_mature_in_zone_count", scw_debug_data->scw_persistent.dyn_obj_data[i].mature_in_zone_count, i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_object_importance_counter",
                                   scw_debug_data->scw_debug_output.scw_object_importance_counter[i], i);
   }

   for (i = FBK_ZERO_UINT; i < SCW_NUMBER_OF_ZONE_POINTS; i++)
   {
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_initial_zone_x", scw_debug_data->scw_debug_output.scw_zone.points[i].x, i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_initial_zone_y", scw_debug_data->scw_debug_output.scw_zone.points[i].y, i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_hysteresis_zone_x", scw_debug_data->scw_debug_output.scw_hysteresis_zone.points[i].x, i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_hysteresis_zone_y", scw_debug_data->scw_debug_output.scw_hysteresis_zone.points[i].y, i);
   }

   /* Cal & SW version */
   SCW_STORE_VAL_MGR_WPR("k_scw_cal_version", scw_debug_data->scw_calibration.Header.version);
   SCW_STORE_VAL_MGR_WPR("Scw_Sw_Major_Version", scw_debug_data->scw_version.scw_sw_major_version);
   SCW_STORE_VAL_MGR_WPR("Scw_Sw_Minor_Version", scw_debug_data->scw_version.scw_sw_minor_version);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_enable_trailer_zone_extension",
                         scw_debug_data->scw_calibration.k_scw_f_enable_trailer_zone_extension);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_enable_trailer_ttc_extension", scw_debug_data->scw_calibration.k_scw_f_enable_trailer_ttc_extension);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_enable_guardrail", scw_debug_data->scw_calibration.k_scw_f_enable_guardrail);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_enable_dynamic", scw_debug_data->scw_calibration.k_scw_f_enable_dynamic);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_enable", scw_debug_data->scw_calibration.k_scw_f_enable);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_guardrail_enable_via_cal", scw_debug_data->scw_calibration.k_scw_f_guardrail_enable_via_cal);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_dynamic_enable_via_cal", scw_debug_data->scw_calibration.k_scw_f_dynamic_enable_via_cal);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_enable_via_cal", scw_debug_data->scw_calibration.k_scw_f_enable_via_cal);
   SCW_STORE_VAL_MGR_WPR("k_scw_guardrail_cycles_in_zone_threshold",
                         scw_debug_data->scw_calibration.k_scw_guardrail_cycles_in_zone_threshold);
   SCW_STORE_VAL_MGR_WPR("k_scw_candidate_mature_cycles_in_zone_threshold",
                         scw_debug_data->scw_calibration.k_scw_candidate_mature_cycles_in_zone_threshold);
   SCW_STORE_VAL_MGR_WPR("k_scw_f_adjust_zones_to_ego_size", scw_debug_data->scw_calibration.k_scw_f_adjust_zones_to_ego_size);
   SCW_STORE_VAL_MGR_WPR("k_scw_min_candidate_age", scw_debug_data->scw_calibration.k_scw_min_candidate_age);
   SCW_STORE_VAL_MGR_WPR("k_scw_min_exist_prob_radar_guardrail", scw_debug_data->scw_calibration.k_scw_min_exist_prob_radar_guardrail);
   SCW_STORE_VAL_MGR_WPR("k_scw_min_host_speed_hys", scw_debug_data->scw_calibration.k_scw_min_host_speed_hys);
   SCW_STORE_VAL_MGR_WPR("k_scw_min_host_speed", scw_debug_data->scw_calibration.k_scw_min_host_speed);

   for (i = SCW_MIN; i <= SCW_MAX; i++)
   {
      SCW_STORE_ARRAY_ELEM_MGR_WPR("k_scw_candidate_velocity", scw_debug_data->scw_calibration.k_scw_candidate_velocity[i], i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("k_scw_candidate_relative_velocity",
                                   scw_debug_data->scw_calibration.k_scw_candidate_relative_velocity[i], i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("k_scw_candidate_heading", scw_debug_data->scw_calibration.k_scw_candidate_heading[i], i);
   }

   SCW_STORE_VAL_MGR_WPR("k_scw_candidate_yawrate", scw_debug_data->scw_calibration.k_scw_candidate_yawrate);
   SCW_STORE_VAL_MGR_WPR("k_scw_candidate_velocity_hys", scw_debug_data->scw_calibration.k_scw_candidate_velocity_hys);
   SCW_STORE_VAL_MGR_WPR("k_scw_candidate_relative_vel_hys", scw_debug_data->scw_calibration.k_scw_candidate_relative_vel_hys);
   SCW_STORE_VAL_MGR_WPR("k_scw_candidate_heading_hys", scw_debug_data->scw_calibration.k_scw_candidate_heading_hys);
   SCW_STORE_VAL_MGR_WPR("k_scw_candidate_yawrate_hys", scw_debug_data->scw_calibration.k_scw_candidate_yawrate_hys);

   for (i = FBK_ZERO_UINT; i < SCW_NUMBER_OF_ZONE_POINTS; i++)
   {
      SCW_STORE_ARRAY_ELEM_MGR_WPR("k_scw_hys_zone_y_offset", scw_debug_data->scw_calibration.k_scw_hys_zone_y_offset[i], i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("k_scw_hys_zone_x_offset", scw_debug_data->scw_calibration.k_scw_hys_zone_x_offset[i], i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("k_scw_initial_zone_y", scw_debug_data->scw_calibration.k_scw_initial_zone_y[i], i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("k_scw_initial_zone_x", scw_debug_data->scw_calibration.k_scw_initial_zone_x[i], i);
   }

   SCW_STORE_VAL_MGR_WPR("k_scw_trailer_zone_ext_safety_margin", scw_debug_data->scw_calibration.k_scw_trailer_zone_ext_safety_margin);
   SCW_STORE_VAL_MGR_WPR("k_scw_trailer_zone_ext_safety_margin_lat",
                         scw_debug_data->scw_calibration.k_scw_trailer_zone_ext_safety_margin_lat);
   SCW_STORE_VAL_MGR_WPR("k_scw_max_zone_length", scw_debug_data->scw_calibration.k_scw_max_zone_length);
   SCW_STORE_VAL_MGR_WPR("k_scw_max_zone_width", scw_debug_data->scw_calibration.k_scw_max_zone_width);
   SCW_STORE_VAL_MGR_WPR("k_scw_lateral_distance_default", scw_debug_data->scw_calibration.k_scw_lateral_distance_default);
   SCW_STORE_VAL_MGR_WPR("k_scw_lateral_ttc_max", scw_debug_data->scw_calibration.k_scw_lateral_ttc_max);
   SCW_STORE_VAL_MGR_WPR("k_scw_lateral_ttc_default", scw_debug_data->scw_calibration.k_scw_lateral_ttc_default);
   SCW_STORE_VAL_MGR_WPR("k_scw_ttle_max", scw_debug_data->scw_calibration.k_scw_ttle_max);
   SCW_STORE_VAL_MGR_WPR("k_scw_ttle_default", scw_debug_data->scw_calibration.k_scw_ttle_default);
   SCW_STORE_VAL_MGR_WPR("k_scw_ttp_max", scw_debug_data->scw_calibration.k_scw_ttp_max);
   SCW_STORE_VAL_MGR_WPR("k_scw_ttp_default", scw_debug_data->scw_calibration.k_scw_ttp_default);

   /* Log the SCW persistent data. */
   for (i = FBK_ZERO_UINT; i < PA_OBJ_NUMBER_OF_GUARDRAILS; i++)
   {
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_guardrail_count_in_zone", scw_debug_data->scw_persistent.count_in_zone_grail[i], i);
      SCW_STORE_ARRAY_ELEM_MGR_WPR("scw_guardrail_lat_position", scw_debug_data->scw_persistent.grail_lat_position[i], i);
   }
}

#endif /* BINARY_DEBUG */
