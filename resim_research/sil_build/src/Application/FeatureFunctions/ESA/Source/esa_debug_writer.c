/**
 * @file esa_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "esa_debug_writer.h"
#include "esa_debug_interface.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Esa_Write_Bin_File(void)
{
   uint8_t i;

   /* Get debug data. */
   Esa_Debug_Data_T *esa_debug_data = Esa_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != esa_debug_data);

   /* Log ESA core input. */
   ESA_STORE_VAL_MGR_WPR("f_esa_enabled", esa_debug_data->esa_core_input.f_esa_enabled);

   ESA_STORE_VAL_MGR_WPR("esa_lane_width", esa_debug_data->esa_core_input.lane_width);
   ESA_STORE_VAL_MGR_WPR("esa_lane_center_offset", esa_debug_data->esa_core_input.lane_center_offset);

   /* Log ESA core output. */
   ESA_STORE_VAL_MGR_WPR("esa_core_status", esa_debug_data->esa_core_output.esa_core_status);

   ESA_STORE_VAL_MGR_WPR("esa_alert_left", esa_debug_data->esa_core_output.esa_alert[FBK_SIDE_LEFT]);
   ESA_STORE_VAL_MGR_WPR("esa_alert_right", esa_debug_data->esa_core_output.esa_alert[FBK_SIDE_RIGHT]);

   ESA_STORE_VAL_MGR_WPR("esa_id_left", esa_debug_data->esa_core_output.esa_id[FBK_SIDE_LEFT]);
   ESA_STORE_VAL_MGR_WPR("esa_id_right", esa_debug_data->esa_core_output.esa_id[FBK_SIDE_RIGHT]);

   ESA_STORE_VAL_MGR_WPR("esa_ttc_left", esa_debug_data->esa_core_output.esa_ttc[FBK_SIDE_LEFT]);
   ESA_STORE_VAL_MGR_WPR("esa_ttc_right", esa_debug_data->esa_core_output.esa_ttc[FBK_SIDE_RIGHT]);

   ESA_STORE_VAL_MGR_WPR("esa_ttp_left", esa_debug_data->esa_core_output.esa_ttp[FBK_SIDE_LEFT]);
   ESA_STORE_VAL_MGR_WPR("esa_ttp_right", esa_debug_data->esa_core_output.esa_ttp[FBK_SIDE_RIGHT]);

   ESA_STORE_VAL_MGR_WPR("esa_decel_to_reach_host_speed_left",
                         esa_debug_data->esa_core_output.esa_decel_to_reach_host_speed[FBK_SIDE_LEFT]);
   ESA_STORE_VAL_MGR_WPR("esa_decel_to_reach_host_speed_right",
                         esa_debug_data->esa_core_output.esa_decel_to_reach_host_speed[FBK_SIDE_RIGHT]);

   ESA_STORE_VAL_MGR_WPR("esa_long_distance_left", esa_debug_data->esa_core_output.esa_long_distance[FBK_SIDE_LEFT]);
   ESA_STORE_VAL_MGR_WPR("esa_long_distance_right", esa_debug_data->esa_core_output.esa_long_distance[FBK_SIDE_RIGHT]);

   for (i = 0; i < ESA_NUMBER_OF_ZONE_POINTS; i++)
   {
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_default_zone_x", esa_debug_data->esa_debug_output.esa_default_zone.points[i].x, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_default_zone_y", esa_debug_data->esa_debug_output.esa_default_zone.points[i].y, i);
   }

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_f_in_zone", esa_debug_data->esa_debug_output.esa_object_data[i].f_obj_in_zone, i);

      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_x_0", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[0].x, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_x_1", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[1].x, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_x_2", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[2].x, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_x_3", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[3].x, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_x_4", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[4].x, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_x_5", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[5].x, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_y_0", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[0].y, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_y_1", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[1].y, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_y_2", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[2].y, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_y_3", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[3].y, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_y_4", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[4].y, i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_obj_zone_y_5", esa_debug_data->esa_debug_output.esa_object_data[i].zone.points[5].y, i);
   }

   /* Write ESA Persistent Data */
   ESA_STORE_VAL_MGR_WPR("f_esa_host_speed_in_activation_range",
                         esa_debug_data->esa_debug_output.esa_persistent.f_host_speed_in_activation_range);
   ESA_STORE_VAL_MGR_WPR("f_esa_disabled_low_curve_radius",
                         esa_debug_data->esa_debug_output.esa_persistent.f_esa_disabled_low_curve_radius);
   for (i = 0; i < FBK_NUMBER_OF_SIDES; i++)
   {
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_prev_alert_obj_index",
                                   esa_debug_data->esa_debug_output.esa_persistent.prev_esa_alert_obj_index[i], i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_prev_alert_obj_id",
                                   esa_debug_data->esa_debug_output.esa_persistent.prev_esa_alert_obj_id[i], i);
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_hold_counter", esa_debug_data->esa_debug_output.esa_persistent.esa_hold_counter[i], i);
   }
   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      ESA_STORE_ARRAY_ELEM_MGR_WPR("esa_mature_count_in_zone",
                                   esa_debug_data->esa_debug_output.esa_persistent.mature_count_in_esa_zone[i], i);
   }

   /* Cal & SW version */
   ESA_STORE_VAL_MGR_WPR("k_esa_cal_version", esa_debug_data->esa_calibration.Header.version);
   ESA_STORE_VAL_MGR_WPR("Esa_Sw_Major_Version", esa_debug_data->esa_version.esa_sw_major_version);
   ESA_STORE_VAL_MGR_WPR("Esa_Sw_Minor_Version", esa_debug_data->esa_version.esa_sw_minor_version);

   ESA_STORE_VAL_MGR_WPR("k_esa_max_range", esa_debug_data->esa_calibration.k_esa_max_range);
}

#endif /* BINARY_DEBUG */
