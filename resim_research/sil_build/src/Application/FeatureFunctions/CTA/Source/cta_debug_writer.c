/**
 * @file cta_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_debug_writer.h"
#include "cta_debug_interface.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pt_output_t.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Cta_Write_Bin_File(void)
{
   uint8_t i;

   /* Get debug data. */
   Cta_Debug_Data_T *cta_debug_data = Cta_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != cta_debug_data);

   /* Log CTA core ouput. */
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_crit_level_left",
                         cta_debug_data->cta_core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_crit_level_right",
                         cta_debug_data->cta_core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_crit_level_left",
                         cta_debug_data->cta_core_output.cta_alert_level[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_crit_level_right",
                         cta_debug_data->cta_core_output.cta_alert_level[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_brake_qualifier_left",
                         cta_debug_data->cta_core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_brake_qualifier_right",
                         cta_debug_data->cta_core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_brake_qualifier_left",
                         cta_debug_data->cta_core_output.f_brake_qualifier[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_brake_qualifier_right",
                         cta_debug_data->cta_core_output.f_brake_qualifier[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_standstill_qualifier_left",
                         cta_debug_data->cta_core_output.f_standstill_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_standstill_qualifier_right",
                         cta_debug_data->cta_core_output.f_standstill_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_standstill_qualifier_left",
                         cta_debug_data->cta_core_output.f_standstill_qualifier[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_standstill_qualifier_right",
                         cta_debug_data->cta_core_output.f_standstill_qualifier[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_brake_deceleration_left",
                         cta_debug_data->cta_core_output.brake_deceleration[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_brake_deceleration_right",
                         cta_debug_data->cta_core_output.brake_deceleration[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_brake_deceleration_left",
                         cta_debug_data->cta_core_output.brake_deceleration[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_brake_deceleration_right",
                         cta_debug_data->cta_core_output.brake_deceleration[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_id_left", cta_debug_data->cta_core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_id_right", cta_debug_data->cta_core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_id_left", cta_debug_data->cta_core_output.cta_id[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_id_right", cta_debug_data->cta_core_output.cta_id[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_index_left", cta_debug_data->cta_core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_index_right", cta_debug_data->cta_core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_index_left", cta_debug_data->cta_core_output.cta_index[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_index_right", cta_debug_data->cta_core_output.cta_index[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_long_isect_left",
                         cta_debug_data->cta_core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_long_isect_right",
                         cta_debug_data->cta_core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_long_isect_left",
                         cta_debug_data->cta_core_output.cta_long_intersection[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_long_isect_right",
                         cta_debug_data->cta_core_output.cta_long_intersection[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_ttc_left", cta_debug_data->cta_core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_ttc_right", cta_debug_data->cta_core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_ttc_left", cta_debug_data->cta_core_output.cta_obj_ttc[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_ttc_right", cta_debug_data->cta_core_output.cta_obj_ttc[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_ttp_left", cta_debug_data->cta_core_output.cta_obj_ttp[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_ttp_right", cta_debug_data->cta_core_output.cta_obj_ttp[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_ttp_left", cta_debug_data->cta_core_output.cta_obj_ttp[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_ttp_right", cta_debug_data->cta_core_output.cta_obj_ttp[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_cta_heading_left",
                         cta_debug_data->cta_core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_rcta_cta_heading_right",
                         cta_debug_data->cta_core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_cta_heading_left",
                         cta_debug_data->cta_core_output.cta_heading[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("cta_core_out_fcta_cta_heading_right",
                         cta_debug_data->cta_core_output.cta_heading[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("f_cta_enabled", cta_debug_data->cta_core_output.f_cta_enabled);
   CTA_STORE_VAL_MGR_WPR("cta_status", cta_debug_data->cta_core_output.cta_status);

   CTA_STORE_VAL_MGR_WPR("cta_most_critical_rcta_obj_index_left",
                         cta_debug_data->cta_debug_output.cta_internals.cta_most_critical_rcta_obj_index_left);
   CTA_STORE_VAL_MGR_WPR("cta_most_critical_rcta_obj_index_right",
                         cta_debug_data->cta_debug_output.cta_internals.cta_most_critical_rcta_obj_index_right);
   CTA_STORE_VAL_MGR_WPR("cta_most_critical_fcta_obj_index_left",
                         cta_debug_data->cta_debug_output.cta_internals.cta_most_critical_fcta_obj_index_left);
   CTA_STORE_VAL_MGR_WPR("cta_most_critical_fcta_obj_index_right",
                         cta_debug_data->cta_debug_output.cta_internals.cta_most_critical_fcta_obj_index_right);

   /* Cta internals. */
   for (i = FBK_ZERO_UINT; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_ref_point_lat", cta_debug_data->cta_object_attributes[i].ref_point.point.y, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_ref_point_lon", cta_debug_data->cta_object_attributes[i].ref_point.point.x, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_ref_point_position", cta_debug_data->cta_object_attributes[i].ref_point.ref_point_index, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_ref_point_ttp_lat", cta_debug_data->cta_object_attributes[i].ref_point_ttp.point.y, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_ref_point_ttp_lon", cta_debug_data->cta_object_attributes[i].ref_point_ttp.point.x, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_ref_point_ttp_position",
                                   cta_debug_data->cta_object_attributes[i].ref_point_ttp.ref_point_index, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rel_vel_lat", cta_debug_data->cta_object_attributes[i].relative_velocity.y, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rel_vel_lon", cta_debug_data->cta_object_attributes[i].relative_velocity.x, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_ttc", cta_debug_data->cta_object_attributes[i].ttc, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_ttp", cta_debug_data->cta_object_attributes[i].ttp, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_approach_side", cta_debug_data->cta_object_attributes[i].approach_side, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_heading", cta_debug_data->cta_object_attributes[i].CTA_heading, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_intersection_point_lon",
                                   cta_debug_data->cta_object_attributes[i].long_isect_point[CTA_MODE_REAR], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_intersection_point_lon",
                                   cta_debug_data->cta_object_attributes[i].long_isect_point[CTA_MODE_FRONT], i);

      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_obj_crit_level", cta_debug_data->cta_debug_output.cta_internals.crit_level_rcta[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_obj_crit_level", cta_debug_data->cta_debug_output.cta_internals.crit_level_fcta[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_path_index_matched_to_obj",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_index_of_path_match_to_obj[i], i);

      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_min_isect_level_1",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_isect_rcta_level_one[i].min, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_max_isect_level_1",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_isect_rcta_level_one[i].max, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_min_isect_level_2",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_isect_rcta_level_two[i].min, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_max_isect_level_2",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_isect_rcta_level_two[i].max, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_min_isect_level_1",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_isect_fcta_level_one[i].min, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_max_isect_level_1",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_isect_fcta_level_one[i].max, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_min_isect_level_2",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_isect_fcta_level_two[i].min, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_max_isect_level_2",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_isect_fcta_level_two[i].max, i);

      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_ttc_level_1",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_ttc_rcta_level_one[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_ttc_level_2",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_ttc_rcta_level_two[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_ttc_level_1",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_ttc_fcta_level_one[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_ttc_level_2",
                                   cta_debug_data->cta_debug_output.cta_internals.cta_ttc_fcta_level_two[i], i);

      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_prev_approach_side", cta_debug_data->cta_object_persistent[i].prev_approach_side, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_obj_validity_supression_counter",
                                   cta_debug_data->cta_object_persistent[i].obj_validity_suppression_counter, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR(
         "cta_rcta_level_suppression_counter_one",
         cta_debug_data->cta_object_persistent[i].crit_level_suppression_counter[CTA_MODE_REAR][FBK_SIDE_LEFT], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR(
         "cta_rcta_level_suppression_counter_two",
         cta_debug_data->cta_object_persistent[i].crit_level_suppression_counter[CTA_MODE_REAR][FBK_SIDE_RIGHT], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR(
         "cta_fcta_level_suppression_counter_one",
         cta_debug_data->cta_object_persistent[i].crit_level_suppression_counter[CTA_MODE_FRONT][FBK_SIDE_LEFT], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR(
         "cta_fcta_level_suppression_counter_two",
         cta_debug_data->cta_object_persistent[i].crit_level_suppression_counter[CTA_MODE_FRONT][FBK_SIDE_RIGHT], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_f_prev_cta_alert_suppress",
                                   cta_debug_data->cta_object_persistent[i].f_prev_cta_alert_suppress, i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_rcta_obj_n_alert_cycles",
                                   cta_debug_data->cta_object_persistent[i].n_alert_cycles[CTA_MODE_REAR], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_fcta_obj_n_alert_cycles",
                                   cta_debug_data->cta_object_persistent[i].n_alert_cycles[CTA_MODE_FRONT], i);

      CTA_STORE_ARRAY_ELEM_MGR_WPR("cta_f_obj_is_valid", cta_debug_data->cta_debug_output.cta_internals.f_object_is_valid[i], i);
   }

   /* CTB internals. */
   CTA_STORE_VAL_MGR_WPR("f_ctb_rcta_ttc_left_qualifier",
                         cta_debug_data->cta_debug_output.ctb_internals.f_ctb_ttc_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("f_ctb_rcta_ttc_right_qualifier",
                         cta_debug_data->cta_debug_output.ctb_internals.f_ctb_ttc_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("f_ctb_fcta_ttc_left_qualifier",
                         cta_debug_data->cta_debug_output.ctb_internals.f_ctb_ttc_qualifier[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("f_ctb_fcta_ttc_right_qualifier",
                         cta_debug_data->cta_debug_output.ctb_internals.f_ctb_ttc_qualifier[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("f_ctb_rcta_safety_dist_left_qualifier",
                         cta_debug_data->cta_debug_output.ctb_internals.f_ctb_safety_dist_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("f_ctb_rcta_safety_dist_right_qualifier",
                         cta_debug_data->cta_debug_output.ctb_internals.f_ctb_safety_dist_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("f_ctb_fcta_safety_dist_left_qualifier",
                         cta_debug_data->cta_debug_output.ctb_internals.f_ctb_safety_dist_qualifier[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("f_ctb_fcta_safety_dist_right_qualifier",
                         cta_debug_data->cta_debug_output.ctb_internals.f_ctb_safety_dist_qualifier[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("ctb_rcta_brake_left_qualification_counter",
                         cta_debug_data->cta_debug_output.ctb_internals.ctb_brake_qualification_counter[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("ctb_rcta_brake_right_qualification_counter",
                         cta_debug_data->cta_debug_output.ctb_internals.ctb_brake_qualification_counter[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_brake_left_qualification_counter",
                         cta_debug_data->cta_debug_output.ctb_internals.ctb_brake_qualification_counter[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR(
      "ctb_fcta_brake_right_qualification_counter",
      cta_debug_data->cta_debug_output.ctb_internals.ctb_brake_qualification_counter[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("ctb_rcta_brake_left_holding_counter",
                         cta_debug_data->cta_debug_output.ctb_internals.ctb_brake_holding_counter[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("ctb_rcta_brake_right_holding_counter",
                         cta_debug_data->cta_debug_output.ctb_internals.ctb_brake_holding_counter[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_brake_left_holding_counter",
                         cta_debug_data->cta_debug_output.ctb_internals.ctb_brake_holding_counter[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_brake_right_holding_counter",
                         cta_debug_data->cta_debug_output.ctb_internals.ctb_brake_holding_counter[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("ctb_rcta_dist_driving_tube_left",
                         cta_debug_data->cta_debug_output.ctb_internals.distance_to_driving_tube[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("ctb_rcta_dist_driving_tube_right",
                         cta_debug_data->cta_debug_output.ctb_internals.distance_to_driving_tube[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_dist_driving_tube_left",
                         cta_debug_data->cta_debug_output.ctb_internals.distance_to_driving_tube[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_dist_driving_tube_right",
                         cta_debug_data->cta_debug_output.ctb_internals.distance_to_driving_tube[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);


   CTA_STORE_VAL_MGR_WPR("ctb_rcta_event_time_left",
                         cta_debug_data->cta_debug_output.ctb_internals.event_time[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("ctb_rcta_event_time_right",
                         cta_debug_data->cta_debug_output.ctb_internals.event_time[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_event_time_left",
                         cta_debug_data->cta_debug_output.ctb_internals.event_time[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_event_time_right",
                         cta_debug_data->cta_debug_output.ctb_internals.event_time[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);

   CTA_STORE_VAL_MGR_WPR("ctb_rcta_min_safety_dist", cta_debug_data->cta_debug_output.ctb_internals.min_safety_dist[CTA_MODE_REAR]);
   CTA_STORE_VAL_MGR_WPR("ctb_rcta_max_safety_dist", cta_debug_data->cta_debug_output.ctb_internals.max_safety_dist[CTA_MODE_REAR]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_min_safety_dist", cta_debug_data->cta_debug_output.ctb_internals.min_safety_dist[CTA_MODE_FRONT]);
   CTA_STORE_VAL_MGR_WPR("ctb_fcta_max_safety_dist", cta_debug_data->cta_debug_output.ctb_internals.max_safety_dist[CTA_MODE_FRONT]);

   /* Cal & SW version */
   CTA_STORE_VAL_MGR_WPR("k_cta_cal_version", cta_debug_data->cta_calibration.Header.version);
   CTA_STORE_VAL_MGR_WPR("Cta_Sw_Major_Version", cta_debug_data->cta_version.cta_sw_major_version);
   CTA_STORE_VAL_MGR_WPR("Cta_Sw_Minor_Version", cta_debug_data->cta_version.cta_sw_minor_version);

   /* Set initial zones */
   /* Debug calibrations in order to avoid toggling from approach sides or Rcta */
   /* Input zone definition expected to be defined for Rear right CTA */
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_long_p_1", cta_debug_data->cta_core_input.cta_zone.points[0].x);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_long_p_2", cta_debug_data->cta_core_input.cta_zone.points[1].x);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_long_p_3", cta_debug_data->cta_core_input.cta_zone.points[2].x);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_long_p_4", cta_debug_data->cta_core_input.cta_zone.points[3].x);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_long_p_5", cta_debug_data->cta_core_input.cta_zone.points[4].x);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_long_p_6", cta_debug_data->cta_core_input.cta_zone.points[5].x);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_long_p_7", cta_debug_data->cta_core_input.cta_zone.points[6].x);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_long_p_8", cta_debug_data->cta_core_input.cta_zone.points[7].x);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_lat_p_1", cta_debug_data->cta_core_input.cta_zone.points[0].y);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_lat_p_2", cta_debug_data->cta_core_input.cta_zone.points[1].y);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_lat_p_3", cta_debug_data->cta_core_input.cta_zone.points[2].y);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_lat_p_4", cta_debug_data->cta_core_input.cta_zone.points[3].y);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_lat_p_5", cta_debug_data->cta_core_input.cta_zone.points[4].y);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_lat_p_6", cta_debug_data->cta_core_input.cta_zone.points[5].y);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_lat_p_7", cta_debug_data->cta_core_input.cta_zone.points[6].y);
   CTA_STORE_VAL_MGR_WPR("cta_initial_butterfly_lat_p_8", cta_debug_data->cta_core_input.cta_zone.points[7].y);
   CTA_STORE_VAL_MGR_WPR("cta_stop_mode", cta_debug_data->cta_core_input.cta_stop_mode);

   /*Calibrations*/
   CTA_STORE_VAL_MGR_WPR("k_cta_host_width_sensor_fov_suppr_factor",
                         cta_debug_data->cta_calibration.k_cta_host_width_sensor_fov_suppr_factor);
   CTA_STORE_VAL_MGR_WPR("k_cta_rel_warning_hysteresis", cta_debug_data->cta_calibration.k_cta_rel_warning_hysteresis);
   CTA_STORE_VAL_MGR_WPR("k_cta_stop_alert_ttc", cta_debug_data->cta_calibration.k_cta_stop_alert_ttc);
   CTA_STORE_VAL_MGR_WPR("k_cta_amount_butterfly_points_in_use", cta_debug_data->cta_calibration.k_cta_amount_butterfly_points_in_use);
   CTA_STORE_VAL_MGR_WPR("k_cta_pedestrian_min_size", cta_debug_data->cta_calibration.k_cta_pedestrian_min_size);
   CTA_STORE_VAL_MGR_WPR("k_cta_pedestrian_min_speed", cta_debug_data->cta_calibration.k_cta_pedestrian_min_speed);
   CTA_STORE_VAL_MGR_WPR("k_cta_2wheel_min_size", cta_debug_data->cta_calibration.k_cta_2wheel_min_size);
   CTA_STORE_VAL_MGR_WPR("k_cta_2wheel_min_speed", cta_debug_data->cta_calibration.k_cta_2wheel_min_speed);
   CTA_STORE_VAL_MGR_WPR("k_ctb_min_brake_hold_ctr_thres", cta_debug_data->cta_calibration.k_ctb_min_brake_hold_ctr_thres);

   /*Calibrations which are enabling specific code parts*/
   CTA_STORE_VAL_MGR_WPR("k_cta_f_apply_heading_compensation_on_intersection_point",
                         cta_debug_data->cta_calibration.k_cta_f_apply_heading_compensation_on_intersection_point);
   CTA_STORE_VAL_MGR_WPR("k_cta_f_adapt_intersect_lines_by_obj_heading",
                         cta_debug_data->cta_calibration.k_cta_f_adapt_intersect_lines_by_obj_heading);
   CTA_STORE_VAL_MGR_WPR("k_cta_f_adapt_intersect_lines_by_steering_angle",
                         cta_debug_data->cta_calibration.k_cta_f_adapt_intersect_lines_by_steering_angle);
   CTA_STORE_VAL_MGR_WPR("k_cta_f_enable_thres_crit_level_reset",
                         cta_debug_data->cta_calibration.k_cta_f_enable_thres_crit_level_reset);
   CTA_STORE_VAL_MGR_WPR("k_cta_f_calc_ttc_ego_side_enabled", cta_debug_data->cta_calibration.k_cta_f_calc_ttc_ego_side_enabled);
   CTA_STORE_VAL_MGR_WPR("k_cta_f_apply_path_tracking", cta_debug_data->cta_calibration.k_cta_f_apply_path_tracking);
   CTA_STORE_VAL_MGR_WPR("k_cta_ego_abs_speed_max", cta_debug_data->cta_calibration.k_cta_ego_abs_speed_max);

   CTA_STORE_VAL_MGR_WPR("k_cta_max_length_fov", cta_debug_data->cta_calibration.k_cta_max_length_fov);
   CTA_STORE_VAL_MGR_WPR("k_CTA_MODE_REAR_angle_deg", cta_debug_data->cta_calibration.k_cta_angles_zone_definition[1]);
   CTA_STORE_VAL_MGR_WPR("k_cta_side_angle_deg", cta_debug_data->cta_calibration.k_cta_angles_zone_definition[0]);

   for (i = FBK_ZERO_UINT; i < CTA_NUM_CRIT_LEVEL; i++)
   {
      CTA_STORE_ARRAY_ELEM_MGR_WPR("k_cta_rcta_ttc_criticality_level",
                                   cta_debug_data->cta_calibration.k_cta_ttc_criticality_level[CTA_MODE_REAR][i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("k_cta_rcta_speed_criticality_level",
                                   cta_debug_data->cta_calibration.k_cta_speed_criticality_level[CTA_MODE_REAR][i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("k_cta_rcta_min_long_point_criticality_level",
                                   cta_debug_data->cta_calibration.k_cta_min_long_point_criticality_level[CTA_MODE_REAR][i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("k_cta_rcta_max_long_point_criticality_level",
                                   cta_debug_data->cta_calibration.k_cta_max_long_point_criticality_level[CTA_MODE_REAR][i], i);

      CTA_STORE_ARRAY_ELEM_MGR_WPR("k_cta_fcta_ttc_criticality_level",
                                   cta_debug_data->cta_calibration.k_cta_ttc_criticality_level[CTA_MODE_FRONT][i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("k_cta_fcta_speed_criticality_level",
                                   cta_debug_data->cta_calibration.k_cta_speed_criticality_level[CTA_MODE_FRONT][i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("k_cta_fcta_min_long_point_criticality_level",
                                   cta_debug_data->cta_calibration.k_cta_min_long_point_criticality_level[CTA_MODE_FRONT][i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("k_cta_fcta_max_long_point_criticality_level",
                                   cta_debug_data->cta_calibration.k_cta_max_long_point_criticality_level[CTA_MODE_FRONT][i], i);
   }

   CTA_STORE_VAL_MGR_WPR("k_cta_rcta_sensor_fov_border", cta_debug_data->cta_calibration.k_cta_sensor_fov_border[CTA_MODE_REAR]);
   CTA_STORE_VAL_MGR_WPR("k_cta_fcta_sensor_fov_border", cta_debug_data->cta_calibration.k_cta_sensor_fov_border[CTA_MODE_FRONT]);

   CTA_STORE_VAL_MGR_WPR("k_cta_min_speed", cta_debug_data->cta_calibration.k_cta_min_speed);
   CTA_STORE_VAL_MGR_WPR("k_cta_max_speed", cta_debug_data->cta_calibration.k_cta_max_speed);
   CTA_STORE_VAL_MGR_WPR("k_cta_min_rel_existence_probability", cta_debug_data->cta_calibration.k_cta_min_rel_existence_probability);
   CTA_STORE_VAL_MGR_WPR("k_cta_heading_range_min", cta_debug_data->cta_calibration.k_cta_heading_range[0]);
   CTA_STORE_VAL_MGR_WPR("k_cta_heading_range_max", cta_debug_data->cta_calibration.k_cta_heading_range[1]);
   CTA_STORE_VAL_MGR_WPR("k_cta_min_park_angle", cta_debug_data->cta_calibration.k_cta_min_park_angle);
   CTA_STORE_VAL_MGR_WPR("k_cta_intersection_line_host_width_percentage",
                         cta_debug_data->cta_calibration.k_cta_intersection_line_host_width_percentage);
   CTA_STORE_VAL_MGR_WPR("k_cta_f_stop_mode_ttp", cta_debug_data->cta_calibration.k_cta_f_stop_mode_ttp);
   CTA_STORE_VAL_MGR_WPR("k_cta_stop_alert_ttp", cta_debug_data->cta_calibration.k_cta_stop_alert_ttp);
   CTA_STORE_VAL_MGR_WPR("k_cta_min_mature_cycles_level_qualifiction",
                         cta_debug_data->cta_calibration.k_cta_min_mature_cycles_level_qualifiction);
   CTA_STORE_VAL_MGR_WPR("k_cta_max_obstruction_probability", cta_debug_data->cta_calibration.k_cta_max_obstruction_probability);
   CTA_STORE_VAL_MGR_WPR("k_cta_ghost_condition_max_heading_diff_path_tracker",
                         cta_debug_data->cta_calibration.k_cta_ghost_condition_max_heading_diff_path_tracker);
   CTA_STORE_VAL_MGR_WPR("k_cta_min_lateral_approach_speed", cta_debug_data->cta_calibration.k_cta_min_lateral_approach_speed);
   CTA_STORE_VAL_MGR_WPR("k_cta_f_use_object_min_object_age_in_cycles",
                         cta_debug_data->cta_calibration.k_cta_f_use_object_min_object_age_in_cycles);
   CTA_STORE_VAL_MGR_WPR("k_cta_cycles_coasted_to_ignore", cta_debug_data->cta_calibration.k_cta_cycles_coasted_to_ignore);
   CTA_STORE_VAL_MGR_WPR("k_cta_f_use_heading_for_relative_velocity_calculation",
                         cta_debug_data->cta_calibration.k_cta_f_use_heading_for_relative_velocity_calculation);
   CTA_STORE_VAL_MGR_WPR("k_cta_cycle_count_hold_true_warning", cta_debug_data->cta_calibration.k_cta_cycle_count_hold_true_warning);
   CTA_STORE_VAL_MGR_WPR("k_ctb_min_braking_time", cta_debug_data->cta_calibration.k_ctb_min_braking_time);
   CTA_STORE_VAL_MGR_WPR("k_ctb_max_braking_time", cta_debug_data->cta_calibration.k_ctb_max_braking_time);
   CTA_STORE_VAL_MGR_WPR("k_cta_cycle_count_suppress_true_warning",
                         cta_debug_data->cta_calibration.k_cta_cycle_count_suppress_true_warning);
}

#endif /* BINARY_DEBUG */
