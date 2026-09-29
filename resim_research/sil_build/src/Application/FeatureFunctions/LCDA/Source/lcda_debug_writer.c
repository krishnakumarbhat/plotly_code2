/**
 * @file lcda_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_debug_writer.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "lcda_debug_interface.h"
#include "lcda_types.h"
#include "pa_vehicle_in.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Lcda_Write_Bin_File(void)
{
   uint8_t side;
   uint8_t i;

   /* Get debug data. */
   Lcda_Debug_Data_T *lcda_debug_data = Lcda_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != lcda_debug_data);

   /* Log LCDA core input. */
   LCDA_STORE_VAL_MGR_WPR("lane_width", lcda_debug_data->lcda_core_input.lane_width);
   LCDA_STORE_VAL_MGR_WPR("lane_center_offset", lcda_debug_data->lcda_core_input.lane_center_offset);
   LCDA_STORE_VAL_MGR_WPR("lane_lateral_speed_left", lcda_debug_data->lcda_core_input.lane_lateral_speed[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("lane_lateral_speed_right", lcda_debug_data->lcda_core_input.lane_lateral_speed[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("f_use_cvw_lane_change_intention_zone",
                          lcda_debug_data->lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* Radar guardrail */
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("radar_guardrail_lateral_position",
                                    lcda_debug_data->lcda_core_input.guardrail_data[side].radar.lateral_position, side);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("radar_guardrail_confidence",
                                    lcda_debug_data->lcda_core_input.guardrail_data[side].radar.confidence, side);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("radar_guardrail_status", lcda_debug_data->lcda_core_input.guardrail_data[side].radar.status,
                                    side);

      /* Camera guardrail */
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("camera_guardrail_lateral_position",
                                    lcda_debug_data->lcda_core_input.guardrail_data[side].camera.lateral_position, side);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("camera_guardrail_confidence",
                                    lcda_debug_data->lcda_core_input.guardrail_data[side].camera.confidence, side);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("camera_guardrail_status", lcda_debug_data->lcda_core_input.guardrail_data[side].camera.status,
                                    side);

      /* Lane change flag */
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("f_lane_change", lcda_debug_data->lcda_core_input.f_lane_change[side], side);

      /* CVW criticallity mode */
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_crit_mode", lcda_debug_data->lcda_core_input.cvw_crit_mode[side], side);
   }

   /* Log LCDA Status */
   LCDA_STORE_VAL_MGR_WPR("lcda_status", lcda_debug_data->lcda_core_output.lcda_status);

   /* Log BSW data */
   LCDA_STORE_VAL_MGR_WPR("f_bsw_is_enabled", lcda_debug_data->lcda_core_output.bsw_core_output.f_bsw_is_enabled);

   LCDA_STORE_VAL_MGR_WPR("bsw_alert_left", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bsw_alert_right", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bsw_id_left", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bsw_id_right", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bsw_ttp_left", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_ttp[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bsw_ttp_right", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_ttp[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bsw_ttle_left", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_ttle[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bsw_ttle_right", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_ttle[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bsw_distance_left", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_distance[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bsw_distance_right", lcda_debug_data->lcda_core_output.bsw_core_output.bsw_distance[FBK_SIDE_RIGHT]);

   for (i = 0; i < LCDA_NUMBER_OF_ZONE_POINTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_default_zone_x", lcda_debug_data->lcda_debug_output.bsw_default_zone.points[i].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_default_zone_y", lcda_debug_data->lcda_debug_output.bsw_default_zone.points[i].y, i);
   }

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_f_in_zone", lcda_debug_data->lcda_debug_output.bsw_object_data[i].f_obj_in_zone, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_f_is_long", lcda_debug_data->lcda_debug_output.bsw_object_data[i].f_obj_long, i);

      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_x_0", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[0].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_x_1", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[1].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_x_2", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[2].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_x_3", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[3].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_x_4", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[4].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_x_5", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[5].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_y_0", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[0].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_y_1", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[1].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_y_2", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[2].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_y_3", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[3].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_y_4", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[4].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_obj_zone_y_5", lcda_debug_data->lcda_debug_output.bsw_object_data[i].zone.points[5].y, i);
   }

   /* Log CVW data */
   LCDA_STORE_VAL_MGR_WPR("f_cvw_is_enabled", lcda_debug_data->lcda_core_output.cvw_core_output.f_cvw_is_enabled);

   LCDA_STORE_VAL_MGR_WPR("cvw_alert_left", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("cvw_alert_right", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("cvw_id_left", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("cvw_id_right", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("cvw_ttc_left", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("cvw_ttc_right", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("cvw_ttp_left", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_ttp[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("cvw_ttp_right", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_ttp[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("cvw_ttle_left", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_ttle[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("cvw_ttle_right", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_ttle[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("cvw_distance_left", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_distance[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("cvw_distance_right", lcda_debug_data->lcda_core_output.cvw_core_output.cvw_distance[FBK_SIDE_RIGHT]);

   for (i = 0; i < LCDA_NUMBER_OF_ZONE_POINTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_default_zone_x", lcda_debug_data->lcda_debug_output.cvw_default_zone.points[i].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_default_zone_y", lcda_debug_data->lcda_debug_output.cvw_default_zone.points[i].y, i);
   }

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_f_in_zone", lcda_debug_data->lcda_debug_output.cvw_object_data[i].f_obj_in_zone, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_f_mature_in_zone",
                                    lcda_debug_data->lcda_debug_output.cvw_object_data[i].f_obj_mature_in_zone, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_behind_guardrail",
                                    lcda_debug_data->lcda_debug_output.cvw_object_data[i].f_obj_behind_guardrail, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_in_ego_lane", lcda_debug_data->lcda_debug_output.cvw_object_data[i].f_obj_in_ego_lane, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_ttc", lcda_debug_data->lcda_debug_output.cvw_object_data[i].ttc, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_critical_dist", lcda_debug_data->lcda_debug_output.cvw_object_data[i].critical_distance, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_front_position",
                                    lcda_debug_data->lcda_debug_output.cvw_object_data[i].obj_front_position, i);

      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_x_0", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[0].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_x_1", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[1].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_x_2", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[2].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_x_3", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[3].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_x_4", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[4].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_x_5", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[5].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_y_0", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[0].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_y_1", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[1].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_y_2", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[2].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_y_3", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[3].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_y_4", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[4].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_obj_zone_y_5", lcda_debug_data->lcda_debug_output.cvw_object_data[i].zone.points[5].y, i);
   }

   /* Log ELC data */
   LCDA_STORE_VAL_MGR_WPR("f_elc_is_enabled", lcda_debug_data->lcda_core_output.elc_core_output.f_elc_is_enabled);

   LCDA_STORE_VAL_MGR_WPR("elc_alert_left", lcda_debug_data->lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("elc_alert_right", lcda_debug_data->lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("elc_id_left", lcda_debug_data->lcda_core_output.elc_core_output.elc_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("elc_id_right", lcda_debug_data->lcda_core_output.elc_core_output.elc_id[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("elc_ttc_left", lcda_debug_data->lcda_core_output.elc_core_output.elc_ttc[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("elc_ttc_right", lcda_debug_data->lcda_core_output.elc_core_output.elc_ttc[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("elc_decel_to_reach_host_speed_left",
                          lcda_debug_data->lcda_core_output.elc_core_output.elc_decel_to_reach_host_speed[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("elc_decel_to_reach_host_speed_right",
                          lcda_debug_data->lcda_core_output.elc_core_output.elc_decel_to_reach_host_speed[FBK_SIDE_RIGHT]);

   for (i = 0; i < LCDA_NUMBER_OF_ZONE_POINTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_default_zone_x", lcda_debug_data->lcda_debug_output.elc_default_zone.points[i].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_default_zone_y", lcda_debug_data->lcda_debug_output.elc_default_zone.points[i].y, i);
   }

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_f_in_zone", lcda_debug_data->lcda_debug_output.elc_object_data[i].f_obj_in_zone, i);

      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_x_0", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[0].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_x_1", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[1].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_x_2", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[2].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_x_3", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[3].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_x_4", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[4].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_x_5", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[5].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_y_0", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[0].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_y_1", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[1].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_y_2", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[2].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_y_3", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[3].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_y_4", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[4].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_obj_zone_y_5", lcda_debug_data->lcda_debug_output.elc_object_data[i].zone.points[5].y, i);
   }

   /* Log SLC data */
   LCDA_STORE_VAL_MGR_WPR("f_slc_is_enabled", lcda_debug_data->lcda_core_output.slc_core_output.f_slc_is_enabled);

   LCDA_STORE_VAL_MGR_WPR("slc_alert_left", lcda_debug_data->lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("slc_alert_right", lcda_debug_data->lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("slc_id_left", lcda_debug_data->lcda_core_output.slc_core_output.slc_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("slc_id_right", lcda_debug_data->lcda_core_output.slc_core_output.slc_id[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("slc_lon_ttc_left", lcda_debug_data->lcda_core_output.slc_core_output.slc_lon_ttc[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("slc_lon_ttc_right", lcda_debug_data->lcda_core_output.slc_core_output.slc_lon_ttc[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("slc_lat_ttc_left", lcda_debug_data->lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("slc_lat_ttc_right", lcda_debug_data->lcda_core_output.slc_core_output.slc_lat_ttc[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("slc_ttp_left", lcda_debug_data->lcda_core_output.slc_core_output.slc_ttp[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("slc_ttp_right", lcda_debug_data->lcda_core_output.slc_core_output.slc_ttp[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("slc_lane_change_prob_left",
                          lcda_debug_data->lcda_core_output.slc_core_output.slc_lane_change_prob[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("slc_lane_change_prob_right",
                          lcda_debug_data->lcda_core_output.slc_core_output.slc_lane_change_prob[FBK_SIDE_RIGHT]);

   for (i = 0; i < LCDA_NUMBER_OF_ZONE_POINTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_default_zone_x", lcda_debug_data->lcda_debug_output.slc_default_zone.points[i].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_default_zone_y", lcda_debug_data->lcda_debug_output.slc_default_zone.points[i].y, i);
   }

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_f_in_zone", lcda_debug_data->lcda_debug_output.slc_object_data[i].f_obj_in_zone, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_f_behind_guardrail",
                                    lcda_debug_data->lcda_debug_output.slc_object_data[i].f_obj_behind_guardrail, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_f_ttc_below_threshold",
                                    lcda_debug_data->lcda_debug_output.slc_object_data[i].f_obj_ttc_below_threshold, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_f_besides_ego",
                                    lcda_debug_data->lcda_debug_output.slc_object_data[i].f_obj_besides_ego, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_f_overlap", lcda_debug_data->lcda_debug_output.slc_object_data[i].f_obj_overlap, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_f_lane_change",
                                    lcda_debug_data->lcda_debug_output.slc_object_data[i].f_obj_lane_change, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_f_misses_ego", lcda_debug_data->lcda_debug_output.slc_object_data[i].f_obj_misses_ego, i);

      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_ttc_lat", lcda_debug_data->lcda_debug_output.slc_object_data[i].lat_ttc, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_ttc_lon", lcda_debug_data->lcda_debug_output.slc_object_data[i].lon_ttc, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_lc_prob", lcda_debug_data->lcda_debug_output.slc_object_data[i].lane_change_prob, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_effective_lateral_speed",
                                    lcda_debug_data->lcda_debug_output.slc_object_data[i].effective_lateral_speed, i);

      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_x_0", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[0].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_x_1", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[1].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_x_2", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[2].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_x_3", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[3].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_x_4", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[4].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_x_5", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[5].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_y_0", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[0].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_y_1", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[1].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_y_2", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[2].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_y_3", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[3].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_y_4", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[4].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_obj_zone_y_5", lcda_debug_data->lcda_debug_output.slc_object_data[i].zone.points[5].y, i);
   }

   /* Write LCDA Persistent Data */
   LCDA_STORE_VAL_MGR_WPR("turn_signal_held", lcda_debug_data->lcda_persistent.turn_signal_held);
   LCDA_STORE_VAL_MGR_WPR("turn_signal_held_counter", lcda_debug_data->lcda_persistent.turn_signal_held_counter);
   LCDA_STORE_VAL_MGR_WPR("f_host_speed_in_activation_range", lcda_debug_data->lcda_persistent.f_host_speed_in_activation_range);
   LCDA_STORE_VAL_MGR_WPR("f_curve_radius_valid", lcda_debug_data->lcda_persistent.f_curve_radius_valid);
   LCDA_STORE_VAL_MGR_WPR("f_bsw_prev_reset", lcda_debug_data->lcda_persistent.f_bsw_prev_reset);
   LCDA_STORE_VAL_MGR_WPR("f_cvw_prev_reset", lcda_debug_data->lcda_persistent.f_cvw_prev_reset);

   /* Write BSW Persistent Data */
   for (i = 0; i < FBK_NUMBER_OF_SIDES; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_prev_alert_obj_id",
                                    lcda_debug_data->lcda_debug_output.bsw_persistent.prev_bsw_alert_obj_id[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_hold_counter", lcda_debug_data->lcda_debug_output.bsw_persistent.bsw_hold_counter[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("f_bsw_prev_active", lcda_debug_data->lcda_debug_output.bsw_persistent.f_prev_bsw_active[i], i);
   }
   for (i = 0; i < LCDA_OBJ_MAX_ARRAY_SIZE; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_mature_count_in_zone",
                                    lcda_debug_data->lcda_debug_output.bsw_persistent.mature_count_in_bsw_zone[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_fallback_state", lcda_debug_data->lcda_debug_output.bsw_persistent.fallback_state[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("bsw_fallback_fast_to_slow_qual_ctr",
                                    lcda_debug_data->lcda_debug_output.bsw_persistent.fallback_fast_to_slow_qual_ctr[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("f_prev_long_truck_status",
                                    lcda_debug_data->lcda_debug_output.bsw_persistent.f_prev_long_truck_status[i], i);
   }

   /* Write CVW Persistent Data */
   for (i = 0; i < FBK_NUMBER_OF_SIDES; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_prev_curve_zone_factor",
                                    lcda_debug_data->lcda_debug_output.cvw_persistent.prev_curve_zone_factor[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_prev_alert_obj_index",
                                    lcda_debug_data->lcda_debug_output.cvw_persistent.prev_cvw_alert_obj_index[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_prev_alert_obj_id",
                                    lcda_debug_data->lcda_debug_output.cvw_persistent.prev_cvw_alert_obj_id[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_hold_counter", lcda_debug_data->lcda_debug_output.cvw_persistent.cvw_hold_counter[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("f_cvw_prev_active", lcda_debug_data->lcda_debug_output.cvw_persistent.f_prev_cvw_active[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("f_prev_used_small_lc_intention_zone",
                                    lcda_debug_data->lcda_debug_output.cvw_persistent.f_prev_used_small_lc_intention_zone[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("lc_intention_zone_change_counter",
                                    lcda_debug_data->lcda_debug_output.cvw_persistent.lc_intention_zone_change_counter[i], i);
   }
   for (i = 0; i < LCDA_OBJ_MAX_ARRAY_SIZE; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("cvw_mature_count_in_zone",
                                    lcda_debug_data->lcda_debug_output.cvw_persistent.mature_count_in_cvw_zone[i], i);
   }

   /* Write ELC Persistent Data */
   for (i = 0; i < FBK_NUMBER_OF_SIDES; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_prev_alert_obj_index",
                                    lcda_debug_data->lcda_debug_output.elc_persistent.prev_elc_alert_obj_index[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_prev_alert_obj_id",
                                    lcda_debug_data->lcda_debug_output.elc_persistent.prev_elc_alert_obj_id[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_hold_counter", lcda_debug_data->lcda_debug_output.elc_persistent.elc_hold_counter[i], i);
   }
   for (i = 0; i < LCDA_OBJ_MAX_ARRAY_SIZE; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("elc_mature_count_in_zone",
                                    lcda_debug_data->lcda_debug_output.elc_persistent.mature_count_in_elc_zone[i], i);
   }

   /* Write SLC Persistent Data */
   for (i = 0; i < FBK_NUMBER_OF_SIDES; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_qualifying_counter",
                                    lcda_debug_data->lcda_debug_output.slc_persistent.slc_qualifying_counter[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_prev_alert_obj_index",
                                    lcda_debug_data->lcda_debug_output.slc_persistent.prev_slc_alert_obj_index[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_prev_alert_obj_id",
                                    lcda_debug_data->lcda_debug_output.slc_persistent.prev_slc_alert_obj_id[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_hold_counter", lcda_debug_data->lcda_debug_output.slc_persistent.slc_hold_counter[i], i);
   }
   for (i = 0; i < LCDA_OBJ_MAX_ARRAY_SIZE; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("slc_mature_count_in_zone",
                                    lcda_debug_data->lcda_debug_output.slc_persistent.mature_count_in_slc_zone[i], i);
   }
   /*Write LCDA common object data*/
   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("lcda_obj_ref_point_bsw_x", lcda_debug_data->lcda_debug_output.obj_ref_point_bsw[i].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("lcda_obj_ref_point_bsw_y", lcda_debug_data->lcda_debug_output.obj_ref_point_bsw[i].y, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("lcda_obj_ref_point_cvw_x", lcda_debug_data->lcda_debug_output.obj_ref_point_cvw[i].x, i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("lcda_obj_ref_point_cvw_y", lcda_debug_data->lcda_debug_output.obj_ref_point_cvw[i].y, i);
   }
   /* Cal & SW version */
   LCDA_STORE_VAL_MGR_WPR("k_lcda_cal_version", lcda_debug_data->lcda_calibration.Header.version);
   LCDA_STORE_VAL_MGR_WPR("Lcda_Sw_Major_Version", lcda_debug_data->lcda_version.lcda_sw_major_version);
   LCDA_STORE_VAL_MGR_WPR("Lcda_Sw_Minor_Version", lcda_debug_data->lcda_version.lcda_sw_minor_version);

   /* General LCDA calibrations */
   LCDA_STORE_VAL_MGR_WPR("k_lcda_zone_check_method", lcda_debug_data->lcda_calibration.k_lcda_zone_check_method);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_f_enable_obj_in_ego_lane_check",
                          lcda_debug_data->lcda_calibration.k_lcda_f_enable_obj_in_ego_lane_check);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_ego_lane_check_center_point_only",
                          lcda_debug_data->lcda_calibration.k_lcda_ego_lane_check_center_point_only);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_ego_lane_effective_lane_width_factor",
                          lcda_debug_data->lcda_calibration.k_lcda_ego_lane_effective_lane_width_factor);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_max_range", lcda_debug_data->lcda_calibration.k_lcda_max_range);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_pedestrian_min_size", lcda_debug_data->lcda_calibration.k_lcda_pedestrian_min_size);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_pedestrian_min_speed", lcda_debug_data->lcda_calibration.k_lcda_pedestrian_min_speed);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_2wheel_min_size", lcda_debug_data->lcda_calibration.k_lcda_2wheel_min_size);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_2wheel_min_speed", lcda_debug_data->lcda_calibration.k_lcda_2wheel_min_speed);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_f_disable_due_to_small_curve_radius",
                          lcda_debug_data->lcda_calibration.k_lcda_f_disable_due_to_small_curve_radius);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_min_curve_radius", lcda_debug_data->lcda_calibration.k_lcda_min_curve_radius);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_min_curve_radius_hys", lcda_debug_data->lcda_calibration.k_lcda_min_curve_radius_hys);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_host_activation_speed_min", lcda_debug_data->lcda_calibration.k_lcda_host_activation_speed_min);

   /* BSW-specific */
   LCDA_STORE_VAL_MGR_WPR("k_bsw_enable", lcda_debug_data->lcda_calibration.k_bsw_enable);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_y_width", lcda_debug_data->lcda_calibration.k_bsw_y_width);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_y0", lcda_debug_data->lcda_calibration.k_bsw_y0);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_x_length", lcda_debug_data->lcda_calibration.k_bsw_x_length);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_x0", lcda_debug_data->lcda_calibration.k_bsw_x0);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_x0_hys", lcda_debug_data->lcda_calibration.k_bsw_x0_hys);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_x1_hys", lcda_debug_data->lcda_calibration.k_bsw_x1_hys);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_y0_hys", lcda_debug_data->lcda_calibration.k_bsw_y0_hys);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_y1_hys", lcda_debug_data->lcda_calibration.k_bsw_y1_hys);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_min_mature_cycles", lcda_debug_data->lcda_calibration.k_bsw_min_mature_cycles);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_max_heading_abs", lcda_debug_data->lcda_calibration.k_bsw_max_heading_abs);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_min_obj_long_vel", lcda_debug_data->lcda_calibration.k_bsw_min_obj_long_vel);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_fallback_rel_vel_thres", lcda_debug_data->lcda_calibration.k_bsw_fallback_rel_vel_thres);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_enable_dynspeed_zone", lcda_debug_data->lcda_calibration.k_bsw_enable_dynspeed_zone);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_f_enable_fallback_handler", lcda_debug_data->lcda_calibration.k_lcda_f_enable_fallback_handler);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_enable_trailer_zone_extension",
                          lcda_debug_data->lcda_calibration.k_bsw_enable_trailer_zone_extension);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_f_zone_extension_by_diff_width_host_vs_trailer",
                          lcda_debug_data->lcda_calibration.k_bsw_f_zone_extension_by_diff_width_host_vs_trailer);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_f_enable_trailer_zone_adjustment_on_ego_side",
                          lcda_debug_data->lcda_calibration.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_f_enable_trailer_zone_adjustment_on_outer_side",
                          lcda_debug_data->lcda_calibration.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_trailer_zone_ext_safety_margin",
                          lcda_debug_data->lcda_calibration.k_bsw_trailer_zone_ext_safety_margin);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_trailer_zone_ext_safety_margin_hys",
                          lcda_debug_data->lcda_calibration.k_bsw_trailer_zone_ext_safety_margin_hys);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_enable_zone_front_boundary_specific_conditions",
                          lcda_debug_data->lcda_calibration.k_bsw_enable_zone_front_boundary_specific_conditions);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_enable_specific_front_sot_conditions",
                          lcda_debug_data->lcda_calibration.k_bsw_enable_specific_front_sot_conditions);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_use_curvi_coordinates", lcda_debug_data->lcda_calibration.k_bsw_use_curvi_coordinates);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_hold_alert_long_object", lcda_debug_data->lcda_calibration.k_bsw_hold_alert_long_object);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_min_length_long_object", lcda_debug_data->lcda_calibration.k_bsw_min_length_long_object);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_min_length_long_object_hys", lcda_debug_data->lcda_calibration.k_bsw_min_length_long_object_hys);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_stop_alert_reaching_front_custom_limit_mode",
                          lcda_debug_data->lcda_calibration.k_bsw_stop_alert_reaching_front_custom_limit_mode);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_f_enable_suppress_alert_object_no_lane_change_intention",
                          lcda_debug_data->lcda_calibration.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge",
                          lcda_debug_data->lcda_calibration.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_f_enable_alert_obj_in_ego_lane",
                          lcda_debug_data->lcda_calibration.k_lcda_f_enable_alert_obj_in_ego_lane);
   LCDA_STORE_VAL_MGR_WPR("k_lcda_f_enable_obj_reflection_flag_check",
                          lcda_debug_data->lcda_calibration.k_lcda_f_enable_obj_reflection_flag_check);
   LCDA_STORE_VAL_MGR_WPR("k_bsw_fallback_rel_vel_thres_hys", lcda_debug_data->lcda_calibration.k_bsw_fallback_rel_vel_thres_hys);

   LCDA_STORE_VAL_MGR_WPR("k_initial_bsw_zone_front_ego_side_x", lcda_debug_data->lcda_calibration.k_bsw_zone_front_ego_side_x);
   LCDA_STORE_VAL_MGR_WPR("k_initial_bsw_zone_front_ego_side_y", lcda_debug_data->lcda_calibration.k_bsw_zone_front_ego_side_y);
   LCDA_STORE_VAL_MGR_WPR("k_initial_bsw_zone_rear_outer_side_x", lcda_debug_data->lcda_calibration.k_bsw_zone_rear_outer_side_x);
   LCDA_STORE_VAL_MGR_WPR("k_initial_bsw_zone_rear_outer_side_y", lcda_debug_data->lcda_calibration.k_bsw_zone_rear_outer_side_y);
   LCDA_STORE_VAL_MGR_WPR("k_initial_bsw_zone_front_ego_side_x_hys",
                          lcda_debug_data->lcda_calibration.k_bsw_zone_front_ego_side_x_hys);
   LCDA_STORE_VAL_MGR_WPR("k_initial_bsw_zone_front_ego_side_y_hys",
                          lcda_debug_data->lcda_calibration.k_bsw_zone_front_ego_side_y_hys);
   LCDA_STORE_VAL_MGR_WPR("k_initial_bsw_zone_rear_outer_side_x_hys",
                          lcda_debug_data->lcda_calibration.k_bsw_zone_rear_outer_side_x_hys);
   LCDA_STORE_VAL_MGR_WPR("k_initial_bsw_zone_rear_outer_side_y_hys",
                          lcda_debug_data->lcda_calibration.k_bsw_zone_rear_outer_side_y_hys);

   /* CVW-specific */
   LCDA_STORE_VAL_MGR_WPR("k_cvw_enable", lcda_debug_data->lcda_calibration.k_cvw_enable);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_x0", lcda_debug_data->lcda_calibration.k_cvw_x0);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_y0", lcda_debug_data->lcda_calibration.k_cvw_y0);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_y_width0", lcda_debug_data->lcda_calibration.k_cvw_y_width0);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_x_length0", lcda_debug_data->lcda_calibration.k_cvw_x_length0);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_y1", lcda_debug_data->lcda_calibration.k_cvw_y1);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_x_length1", lcda_debug_data->lcda_calibration.k_cvw_x_length1);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_y_width1", lcda_debug_data->lcda_calibration.k_cvw_y_width1);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_min_mature_cycles", lcda_debug_data->lcda_calibration.k_cvw_min_mature_cycles);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_max_curvi_heading_abs", lcda_debug_data->lcda_calibration.k_cvw_max_curvi_heading_abs);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_min_obj_curvi_long_vel", lcda_debug_data->lcda_calibration.k_cvw_min_obj_curvi_long_vel);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_ttc", lcda_debug_data->lcda_calibration.k_cvw_ttc);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_ttc_hys", lcda_debug_data->lcda_calibration.k_cvw_ttc_hys);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_max_object_curvi_relative_speed",
                          lcda_debug_data->lcda_calibration.k_cvw_max_object_curvi_relative_speed);
   LCDA_STORE_VAL_MGR_WPR("k_cvw_object_curvi_relative_speed_hys",
                          lcda_debug_data->lcda_calibration.k_cvw_object_curvi_relative_speed_hys);
   LCDA_STORE_VAL_MGR_WPR("k_lka_ov_zone_width", lcda_debug_data->lcda_calibration.k_lka_ov_zone_width);

   for (i = 0; i < LCDA_K_CVW_MIN_OBJECT_CURVI_RELATIVE_SPEED_ARRAY_SIZE_DIM0; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("k_cvw_min_object_curvi_relative_speed",
                                    lcda_debug_data->lcda_calibration.k_cvw_min_object_curvi_relative_speed[i], i);
   }

   for (i = 0; i < LCDA_K_CVW_ZONE_Y_HYS_ARRAY_SIZE_DIM0; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("k_cvw_zone_y_hys", lcda_debug_data->lcda_calibration.k_cvw_zone_y_hys[i], i);
   }
   for (i = 0; i < LCDA_K_BSW_DYNZONE_RANGE_ARRAY_SIZE_DIM0; i++)
   {
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("k_bsw_dynzone_range", lcda_debug_data->lcda_calibration.k_bsw_dynzone_range[i], i);
      LCDA_STORE_ARRAY_ELEM_MGR_WPR("k_bsw_dynzone_speed", lcda_debug_data->lcda_calibration.k_bsw_dynzone_speed[i], i);
   }
}

#endif /* BINARY_DEBUG */
