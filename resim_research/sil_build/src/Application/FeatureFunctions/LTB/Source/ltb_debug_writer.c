/**
 * @file ltb_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "ltb_debug_writer.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "ltb_debug_interface.h"
#include "pa_vehicle_in.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Ltb_Write_Bin_File(void)
{
   uint8_t index;

   /* Get debug data. */
   Ltb_Debug_Data_T *ltb_debug_data = Ltb_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != ltb_debug_data);

   /* Log the LTB core input. */
   LTB_STORE_VAL_MGR_WPR("ltb_core_in_f_ltb_enable", ltb_debug_data->ltb_core_input.f_ltb_enable);

   /* Log the LTB core output. */
   LTB_STORE_VAL_MGR_WPR("ltb_core_out_most_critical_side", ltb_debug_data->ltb_core_output.ltb_most_critical_side);

   for (index = FBK_ZERO_UINT; index < FBK_NUMBER_OF_SIDES; index++)
   {
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_waypoint_at_collision_x",
                                   ltb_debug_data->ltb_core_output.ltb_waypoint_at_collision[index].x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_waypoint_at_collision_y",
                                   ltb_debug_data->ltb_core_output.ltb_waypoint_at_collision[index].y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_ttc", ltb_debug_data->ltb_core_output.ltb_ttc[index], index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_ttb", ltb_debug_data->ltb_core_output.ltb_ttb[index], index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_decel_estimate", ltb_debug_data->ltb_core_output.ltb_decel_estimate[index], index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_distance", ltb_debug_data->ltb_core_output.ltb_distance[index], index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_alert_level", ltb_debug_data->ltb_core_output.ltb_alert_level[index], index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_id", ltb_debug_data->ltb_core_output.ltb_id[index], index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_index", ltb_debug_data->ltb_core_output.ltb_index[index], index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_core_out_f_obj_in_zone", ltb_debug_data->ltb_core_output.ltb_f_obj_in_zone[index], index);
   }

   LTB_STORE_VAL_MGR_WPR("ltb_core_out_f_obj_in_zone_left", ltb_debug_data->ltb_core_output.ltb_f_obj_in_zone[FBK_SIDE_LEFT]);
   LTB_STORE_VAL_MGR_WPR("ltb_core_out_f_obj_in_zone_right", ltb_debug_data->ltb_core_output.ltb_f_obj_in_zone[FBK_SIDE_RIGHT]);

   /* Log the persistent data.*/
   LTB_STORE_VAL_MGR_WPR("ltb_alert_prev_cycle_left", ltb_debug_data->ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_LEFT]);
   LTB_STORE_VAL_MGR_WPR("ltb_alert_prev_cycle_right", ltb_debug_data->ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT]);
   LTB_STORE_VAL_MGR_WPR("ltb_side_id_prev_cycle_left", ltb_debug_data->ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_LEFT]);
   LTB_STORE_VAL_MGR_WPR("ltb_side_id_prev_cycle_right", ltb_debug_data->ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT]);
   LTB_STORE_VAL_MGR_WPR("ltb_side_alert_qualifying_counter_left",
                         ltb_debug_data->ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_LEFT]);
   LTB_STORE_VAL_MGR_WPR("ltb_side_alert_qualifying_counter_right",
                         ltb_debug_data->ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT]);
   LTB_STORE_VAL_MGR_WPR("ltb_side_alert_holding_counter_left",
                         ltb_debug_data->ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_LEFT]);
   LTB_STORE_VAL_MGR_WPR("ltb_side_alert_holding_counter_right",
                         ltb_debug_data->ltb_persistent.ltb_side_alert_prev_cycle[FBK_SIDE_RIGHT]);

   /* Log the calibration data.*/
   LTB_STORE_VAL_MGR_WPR("k_ltb_zone_width", ltb_debug_data->ltb_calibration.k_ltb_zone_width);
   LTB_STORE_VAL_MGR_WPR("k_ltb_zone_length", ltb_debug_data->ltb_calibration.k_ltb_zone_length);
   LTB_STORE_VAL_MGR_WPR("k_ltb_prediction_steps_max", ltb_debug_data->ltb_calibration.k_ltb_prediction_steps_max);
   LTB_STORE_VAL_MGR_WPR("k_ltb_critical_approach_min_safe_distance",
                         ltb_debug_data->ltb_calibration.k_ltb_critical_approach_min_safe_distance);
   LTB_STORE_VAL_MGR_WPR("k_ltb_alert_lvl_2_ttc_threshold", ltb_debug_data->ltb_calibration.k_ltb_alert_lvl_2_ttc_threshold);
   LTB_STORE_VAL_MGR_WPR("k_ltb_alert_lvl_3_ttc_threshold", ltb_debug_data->ltb_calibration.k_ltb_alert_lvl_3_ttc_threshold);
   LTB_STORE_VAL_MGR_WPR("k_ltb_alert_lvl_2_ttb_threshold", ltb_debug_data->ltb_calibration.k_ltb_alert_lvl_2_ttb_threshold);
   LTB_STORE_VAL_MGR_WPR("k_ltb_alert_lvl_3_decel_threshold", ltb_debug_data->ltb_calibration.k_ltb_alert_lvl_3_decel_threshold);

   /* Cal & SW version */
   LTB_STORE_VAL_MGR_WPR("ltb_Sw_Major_Version", ltb_debug_data->ltb_version.ltb_sw_major_version);
   LTB_STORE_VAL_MGR_WPR("ltb_Sw_Minor_Version", ltb_debug_data->ltb_version.ltb_sw_minor_version);

   /* Write Ltb ego trajectory data */
   LTB_STORE_VAL_MGR_WPR("ltb_ego_trajectory_valid", ltb_debug_data->ltb_debug_output.ego_trajectory.f_trajectory_valid);
   LTB_STORE_VAL_MGR_WPR("ltb_ego_trajectory_n_prediction_steps", ltb_debug_data->ltb_debug_output.ego_trajectory.n_prediction_steps);

   for (index = FBK_ZERO_UINT; index < FBK_MAX_PREDICTION_STEPS; index++)
   {
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_waypoint_coordinates_x",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].waypoint_coordinates.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_waypoint_coordinates_y",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].waypoint_coordinates.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_waypoint_yaw_angle",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].waypoint_yaw_angle.angle, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_waypoint_speed",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].waypoint_speed, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_circle_center_front_x",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].circle_center_front.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_circle_center_front_y",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].circle_center_front.y, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_circle_center_middle_x",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].circle_center_middle.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_circle_center_middle_y",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].circle_center_middle.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_circle_center_rear_x",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].circle_center_rear.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_circle_center_rear_y",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].circle_center_rear.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_circle_radius",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].circle_radius, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_ego_trajectory_waypoint_valid",
                                   ltb_debug_data->ltb_debug_output.ego_trajectory.waypoint[index].f_waypoint_valid, index);
   }

   /* Write Ltb object data */
   for (index = FBK_ZERO_UINT; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_alert_level", ltb_debug_data->ltb_object_attributes[index].alert_level, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_alert_side", ltb_debug_data->ltb_object_attributes[index].alert_side, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_ttc", ltb_debug_data->ltb_object_attributes[index].ttc, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_ttb", ltb_debug_data->ltb_object_attributes[index].ttb, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_decel_to_avoid_coll", ltb_debug_data->ltb_object_attributes[index].decel_to_avoid_coll,
                                   index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_f_curvi_available", ltb_debug_data->ltb_object_attributes[index].f_curvi_available, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_f_vehicle_state_relevant",
                                   ltb_debug_data->ltb_object_attributes[index].f_vehicle_state_relevant, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_f_obj_ltb_relevant", ltb_debug_data->ltb_object_attributes[index].f_obj_ltb_relevant,
                                   index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_f_obj_in_zone", ltb_debug_data->ltb_object_attributes[index].f_obj_in_zone, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_waypoint_at_collision_x",
                                   ltb_debug_data->ltb_object_attributes[index].waypoint_at_collision.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_waypoint_at_collision_y",
                                   ltb_debug_data->ltb_object_attributes[index].waypoint_at_collision.y, index);
   }

   /* Write Ltb obj trajectory data for most critical object or potentially critical object on left side */
   LTB_STORE_VAL_MGR_WPR("ltb_obj_traj_left_valid",
                         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.f_trajectory_valid);
   LTB_STORE_VAL_MGR_WPR("ltb_obj_traj_left_n_prediction_steps",
                         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.n_prediction_steps);

   for (index = FBK_ZERO_UINT; index < FBK_MAX_PREDICTION_STEPS; index++)
   {
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_waypoint_coordinates_x",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].waypoint_coordinates.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_waypoint_coordinates_y",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].waypoint_coordinates.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_waypoint_yaw_angle",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].waypoint_yaw_angle.angle, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_traj_left_waypoint_speed",
                                   ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].waypoint_speed,
                                   index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_circle_center_front_x",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_front.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_circle_center_front_y",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_front.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_circle_center_middle_x",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_middle.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_circle_center_middle_y",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_middle.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_circle_center_rear_x",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_rear.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_left_circle_center_rear_y",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_rear.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_traj_left_circle_radius",
                                   ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_radius,
                                   index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_traj_left_waypoint_valid",
                                   ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_left.waypoint[index].f_waypoint_valid,
                                   index);
   }

   /* Write Ltb obj trajectory data for most critical object or potentially critical object on right side */
   LTB_STORE_VAL_MGR_WPR("ltb_obj_traj_right_valid",
                         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.f_trajectory_valid);
   LTB_STORE_VAL_MGR_WPR("ltb_obj_traj_right_n_prediction_steps",
                         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.n_prediction_steps);

   for (index = FBK_ZERO_UINT; index < FBK_MAX_PREDICTION_STEPS; index++)
   {
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_waypoint_coordinates_x",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].waypoint_coordinates.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_waypoint_coordinates_y",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].waypoint_coordinates.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_waypoint_yaw_angle",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].waypoint_yaw_angle.angle, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_traj_right_waypoint_speed",
                                   ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].waypoint_speed,
                                   index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_circle_center_front_x",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_front.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_circle_center_front_y",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_front.y, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_circle_center_middle_x",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_middle.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_circle_center_middle_y",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_middle.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_circle_center_rear_x",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_rear.x, index);
      LTB_STORE_ARRAY_ELEM_MGR_WPR(
         "ltb_obj_traj_right_circle_center_rear_y",
         ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_rear.y, index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_traj_right_circle_radius",
                                   ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_radius,
                                   index);

      LTB_STORE_ARRAY_ELEM_MGR_WPR("ltb_obj_traj_right_waypoint_valid",
                                   ltb_debug_data->ltb_debug_output.most_crit_object_trajectory_right.waypoint[index].f_waypoint_valid,
                                   index);
   }
}
#endif /* BINARY_DEBUG */
