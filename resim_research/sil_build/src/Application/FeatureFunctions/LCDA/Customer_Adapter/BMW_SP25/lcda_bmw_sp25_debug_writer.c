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

#include "lcda_bmw_sp25_debug_interface.h"
#include "lcda_debug_writer.h"
#include "pa_reuse.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Lcda_Bmw_Sp25_Write_Bin_File(void)
{
   /* Get debug data. */
   Lcda_Bmw_Sp25_Debug_Data_T *lcda_debug_data = Lcda_Get_Bmw_Sp25_Debug_Data();

   /* Check input parameters. */
   assert(NULL != lcda_debug_data);

   /* Persistent data */
   LCDA_STORE_VAL_MGR_WPR("lm_persist_vdyn_count_city2hway", lcda_debug_data->lcda_debug_output.lane_model.vdyn_count_city2hway);
   LCDA_STORE_VAL_MGR_WPR("lm_persist_vdyn_count_hway2city", lcda_debug_data->lcda_debug_output.lane_model.vdyn_count_hway2city);
   LCDA_STORE_VAL_MGR_WPR("lm_persist_navi_count_city", lcda_debug_data->lcda_debug_output.lane_model.navi_count_city);
   LCDA_STORE_VAL_MGR_WPR("lm_persist_navi_count_hway", lcda_debug_data->lcda_debug_output.lane_model.navi_count_hway);
   LCDA_STORE_VAL_MGR_WPR("lm_persist_vdyn_road_type", lcda_debug_data->lcda_debug_output.lane_model.vdyn_road_type);
   LCDA_STORE_VAL_MGR_WPR("lm_persist_navi_road_type", lcda_debug_data->lcda_debug_output.lane_model.navi_road_type);

   /* Lane model camera data */
   LCDA_STORE_VAL_MGR_WPR("lm_camera_data_lane_width",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output_camera.lane_width);
   LCDA_STORE_VAL_MGR_WPR("lm_camera_data_lane_center_offset",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output_camera.lane_center_offset);
   LCDA_STORE_VAL_MGR_WPR("lm_camera_data_lane_lateral_speed_left",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output_camera.lane_lateral_speed[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("lm_camera_data_lane_lateral_speed_right",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output_camera.lane_lateral_speed[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("lm_camera_data_status", lcda_debug_data->lcda_debug_output.lane_model.lane_model_output_camera.status);
   LCDA_STORE_VAL_MGR_WPR("lm_camera_data_calculation_method",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output_camera.calculation_method);

   /* Lane model output */
   LCDA_STORE_VAL_MGR_WPR("lm_out_lane_width", lcda_debug_data->lcda_debug_output.lane_model.lane_model_output.lane_width);
   LCDA_STORE_VAL_MGR_WPR("lm_out_lane_center_offset",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output.lane_center_offset);
   LCDA_STORE_VAL_MGR_WPR("lm_out_lane_lateral_speed_left",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output.lane_lateral_speed[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("lm_out_lane_lateral_speed_right",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output.lane_lateral_speed[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("lm_out_calculation_method",
                          lcda_debug_data->lcda_debug_output.lane_model.lane_model_output.calculation_method);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_f_lcda_enabled", lcda_debug_data->lcda_output.f_lcda_enabled);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_f_bsw_enabled", lcda_debug_data->lcda_output.f_bsw_enabled);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_f_cvw_enabled", lcda_debug_data->lcda_output.f_cvw_enabled);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_f_slc_enabled", lcda_debug_data->lcda_output.f_slc_enabled);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_f_awa_enabled", lcda_debug_data->lcda_output.f_awa_enabled);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_bsw_alert_left", lcda_debug_data->lcda_output.bsw_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_bsw_alert_right", lcda_debug_data->lcda_output.bsw_alert[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_bsw_id_left", lcda_debug_data->lcda_output.bsw_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_bsw_id_right", lcda_debug_data->lcda_output.bsw_id[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_bsw_ttp_left", lcda_debug_data->lcda_output.bsw_ttp[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_bsw_ttp_right", lcda_debug_data->lcda_output.bsw_ttp[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_bsw_ttle_left", lcda_debug_data->lcda_output.bsw_ttle[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_bsw_ttle_right", lcda_debug_data->lcda_output.bsw_ttle[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_alert_left", lcda_debug_data->lcda_output.cvw_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_alert_right", lcda_debug_data->lcda_output.cvw_alert[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_id_left", lcda_debug_data->lcda_output.cvw_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_id_right", lcda_debug_data->lcda_output.cvw_id[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_ttc_left", lcda_debug_data->lcda_output.cvw_ttc[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_ttc_right", lcda_debug_data->lcda_output.cvw_ttc[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_ttp_left", lcda_debug_data->lcda_output.cvw_ttp[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_ttp_right", lcda_debug_data->lcda_output.cvw_ttp[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_ttle_left", lcda_debug_data->lcda_output.cvw_ttle[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_cvw_ttle_right", lcda_debug_data->lcda_output.cvw_ttle[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_alert_left", lcda_debug_data->lcda_output.slc_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_alert_right", lcda_debug_data->lcda_output.slc_alert[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_id_left", lcda_debug_data->lcda_output.slc_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_id_right", lcda_debug_data->lcda_output.slc_id[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_ttc_left", lcda_debug_data->lcda_output.slc_ttc[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_ttc_right", lcda_debug_data->lcda_output.slc_ttc[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_ttp_left", lcda_debug_data->lcda_output.slc_ttp[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_ttp_right", lcda_debug_data->lcda_output.slc_ttp[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_lane_change_probability_left",
                          lcda_debug_data->lcda_output.slc_lane_change_probability[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_slc_lane_change_probability_right",
                          lcda_debug_data->lcda_output.slc_lane_change_probability[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_awa_alert_left", lcda_debug_data->lcda_output.awa_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_awa_alert_right", lcda_debug_data->lcda_output.awa_alert[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_awa_id_left", lcda_debug_data->lcda_output.awa_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_awa_id_right", lcda_debug_data->lcda_output.awa_id[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_awa_ttc_left", lcda_debug_data->lcda_output.awa_ttc[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_awa_ttc_right", lcda_debug_data->lcda_output.awa_ttc[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_awa_dec_left", lcda_debug_data->lcda_output.awa_dec[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_awa_dec_right", lcda_debug_data->lcda_output.awa_dec[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lane_width", lcda_debug_data->lcda_output.lane_width);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lane_center_offset", lcda_debug_data->lcda_output.lane_center_offset);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lane_lateral_speed_left", lcda_debug_data->lcda_output.lane_lateral_speed[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lane_lateral_speed_right", lcda_debug_data->lcda_output.lane_lateral_speed[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_type_left", lcda_debug_data->lcda_output.lcda_object_type_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_type_right", lcda_debug_data->lcda_output.lcda_object_type_right);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_id_left", lcda_debug_data->lcda_output.lcda_object_id_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_px_left", lcda_debug_data->lcda_output.lcda_object_px_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_py_left", lcda_debug_data->lcda_output.lcda_object_py_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_ttc_left", lcda_debug_data->lcda_output.lcda_object_ttc_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_vx_left", lcda_debug_data->lcda_output.lcda_object_vx_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_vy_left", lcda_debug_data->lcda_output.lcda_object_vy_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_existance_probability_left",
                          lcda_debug_data->lcda_output.lcda_object_existance_probability_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_lane_change_probability_left",
                          lcda_debug_data->lcda_output.lcda_object_lane_change_probability_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_id_right", lcda_debug_data->lcda_output.lcda_object_id_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_px_right", lcda_debug_data->lcda_output.lcda_object_px_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_py_right", lcda_debug_data->lcda_output.lcda_object_py_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_ttc_right", lcda_debug_data->lcda_output.lcda_object_ttc_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_vx_right", lcda_debug_data->lcda_output.lcda_object_vx_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_vy_right", lcda_debug_data->lcda_output.lcda_object_vy_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_existance_probability_right",
                          lcda_debug_data->lcda_output.lcda_object_existance_probability_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_lane_change_probability_right",
                          lcda_debug_data->lcda_output.lcda_object_lane_change_probability_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_length_left", lcda_debug_data->lcda_output.lcda_object_length_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_length_right", lcda_debug_data->lcda_output.lcda_object_length_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_width_left", lcda_debug_data->lcda_output.lcda_object_width_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_object_width_right", lcda_debug_data->lcda_output.lcda_object_width_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_lane_change_probability_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_event_data_qualifier",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_event_data_qualifier);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_existance_probability_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_existance_probability_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_existance_probability_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_extended_qualifier",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_extended_qualifier);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_id_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_lane_change_probability_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_lane_change_probability_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_lcda_function_state",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_qualifier_lcda_function_state);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_length_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_id_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_id_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_length_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_length_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_position_x_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_position_y_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_status_bsw",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_bsw);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_status_cvw",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_cvw);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_status_slc",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_status_slc);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_timestamp_left_hour",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.hour);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_timestamp_left_minute",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.minute);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_timestamp_left_second",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_left.second);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_ttc_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_ttle_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_ttp_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_velocity_x_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_velocity_y_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_width_left",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_left);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_position_x_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_x_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_position_y_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_position_y_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_timestamp_right_hour",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.hour);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_timestamp_right_minute",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.minute);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_timestamp_right_second",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_timestamp_right.second);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_ttc_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttc_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_ttle_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttle_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_ttp_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_ttp_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_velocity_x_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_x_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_velocity_y_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_velocity_y_right);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_lcda_output_bus_signals_width_right",
                          lcda_debug_data->lcda_output.bmw_lcda_output_bus_signals.bmw_lcda_object_width_right);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enabled", lcda_debug_data->lcda_input.f_lcda_enable);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_bsw", lcda_debug_data->lcda_input.f_lcda_enable_bsw);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_cvw", lcda_debug_data->lcda_input.f_lcda_enable_cvw);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_slc", lcda_debug_data->lcda_input.f_lcda_enable_slc);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_awa", lcda_debug_data->lcda_input.f_lcda_enable_awa);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_dropback", lcda_debug_data->lcda_input.f_lcda_enable_dropback);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_fallback", lcda_debug_data->lcda_input.f_lcda_enable_fallback);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_environment_plausibilization",
                          lcda_debug_data->lcda_input.f_lcda_enable_environment_plausibilization);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_trailer_mode", lcda_debug_data->lcda_input.f_lcda_trailer_mode);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_trailer_connected", lcda_debug_data->lcda_input.f_lcda_trailer_connected);

   LCDA_STORE_VAL_MGR_WPR("f_lcda_enable_cvw_limit_zone", lcda_debug_data->lcda_input.f_lcda_enable_cvw_limit_zone);
   LCDA_STORE_VAL_MGR_WPR("lcda_cvw_limit_zone_range", lcda_debug_data->lcda_input.lcda_cvw_limit_zone_range);
   LCDA_STORE_VAL_MGR_WPR("f_lcda_enable_bsw_GBT", lcda_debug_data->lcda_input.f_lcda_enable_bsw_GBT);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_lcda_warntrigger_hmi", lcda_debug_data->lcda_input.lcda_warntrigger_hmi);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_basic_lane_model", lcda_debug_data->lcda_input.f_lcda_enable_basic_lane_model);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_f_lcda_enable_extended_lane_model",
                          lcda_debug_data->lcda_input.f_lcda_enable_extended_lane_model);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_driver_side", lcda_debug_data->lcda_input.driver_side);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_country_type", lcda_debug_data->lcda_input.country_type);

   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_navigation_data_road_type", lcda_debug_data->lcda_input.navigation_data_road_type);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_navigation_data_number_of_lanes",
                          lcda_debug_data->lcda_input.navigation_data_number_of_lanes);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_c_f_lcda_enable_bsw",
                          lcda_debug_data->lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_c_f_lcda_enable_cvw",
                          lcda_debug_data->lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_c_f_lcda_enable_slc",
                          lcda_debug_data->lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_c_f_lcda_enable_awa",
                          lcda_debug_data->lcda_input.lcda_coding_parameters.c_f_lcda_enable_awa);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_c_f_lcda_enabled", lcda_debug_data->lcda_input.lcda_coding_parameters.c_f_lcda_enabled);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_c_lcda_max_vel_upper_limit",
                          lcda_debug_data->lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_c_lcda_min_vel_lower_limit",
                          lcda_debug_data->lcda_input.lcda_coding_parameters.c_lcda_min_vel_lower_limit);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_c_min_curve_radii", lcda_debug_data->lcda_input.lcda_coding_parameters.c_min_curve_radii);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_curve_radii", lcda_debug_data->lcda_input.lcda_input_signals.curve_radii);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_lcda_function_error", lcda_debug_data->lcda_input.lcda_input_signals.lcda_function_error);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_lcda_vehicle_condition", lcda_debug_data->lcda_input.lcda_input_signals.vehicle_condition);
   LCDA_STORE_VAL_MGR_WPR("bmw_sp25_input_lcda_vehicle_driving_direction",
                          lcda_debug_data->lcda_input.lcda_input_signals.vehicle_driving_direction);

   /* Camera data */
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_width_ego", lcda_debug_data->lcda_input.camera_data->lane_width_ego);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_width_left", lcda_debug_data->lcda_input.camera_data->lane_width_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_width_right", lcda_debug_data->lcda_input.camera_data->lane_width_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_center_offset", lcda_debug_data->lcda_input.camera_data->lane_center_offset);
   LCDA_STORE_VAL_MGR_WPR("camera_data_quality_lane_width_ego", lcda_debug_data->lcda_input.camera_data->quality_lane_width_ego);
   LCDA_STORE_VAL_MGR_WPR("camera_data_quality_lane_width_left", lcda_debug_data->lcda_input.camera_data->quality_lane_width_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_quality_lane_width_right", lcda_debug_data->lcda_input.camera_data->quality_lane_width_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_quality_lane_center_offset",
                          lcda_debug_data->lcda_input.camera_data->quality_lane_center_offset);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_distance_first_left", lcda_debug_data->lcda_input.camera_data->lane_distance_first_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_distance_first_right", lcda_debug_data->lcda_input.camera_data->lane_distance_first_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_distance_second_left", lcda_debug_data->lcda_input.camera_data->lane_distance_second_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_distance_second_right",
                          lcda_debug_data->lcda_input.camera_data->lane_distance_second_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_angle_first_left", lcda_debug_data->lcda_input.camera_data->lane_angle_first_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_angle_first_right", lcda_debug_data->lcda_input.camera_data->lane_angle_first_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_angle_second_left", lcda_debug_data->lcda_input.camera_data->lane_angle_second_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_angle_second_right", lcda_debug_data->lcda_input.camera_data->lane_angle_second_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_existance_probability_first_left",
                          lcda_debug_data->lcda_input.camera_data->lane_existance_probability_first_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_existance_probability_first_right",
                          lcda_debug_data->lcda_input.camera_data->lane_existance_probability_first_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_existance_probability_second_left",
                          lcda_debug_data->lcda_input.camera_data->lane_existance_probability_second_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_existance_probability_second_right",
                          lcda_debug_data->lcda_input.camera_data->lane_existance_probability_second_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_type_first_left", lcda_debug_data->lcda_input.camera_data->lane_type_first_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_type_first_right", lcda_debug_data->lcda_input.camera_data->lane_type_first_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_type_second_left", lcda_debug_data->lcda_input.camera_data->lane_type_second_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_type_second_right", lcda_debug_data->lcda_input.camera_data->lane_type_second_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_border_status_first_left",
                          lcda_debug_data->lcda_input.camera_data->lane_border_status_first_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_border_status_first_right",
                          lcda_debug_data->lcda_input.camera_data->lane_border_status_first_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_border_status_second_left",
                          lcda_debug_data->lcda_input.camera_data->lane_border_status_second_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_border_status_second_right",
                          lcda_debug_data->lcda_input.camera_data->lane_border_status_second_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_color_first_left", lcda_debug_data->lcda_input.camera_data->lane_color_first_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_color_first_right", lcda_debug_data->lcda_input.camera_data->lane_color_first_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_curvature_first_left", lcda_debug_data->lcda_input.camera_data->lane_curvature_first_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_curvature_first_right",
                          lcda_debug_data->lcda_input.camera_data->lane_curvature_first_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_curvature_second_left",
                          lcda_debug_data->lcda_input.camera_data->lane_curvature_second_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_curvature_second_right",
                          lcda_debug_data->lcda_input.camera_data->lane_curvature_second_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_curvature_change_first_left",
                          lcda_debug_data->lcda_input.camera_data->lane_curvature_change_first_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_curvature_change_first_right",
                          lcda_debug_data->lcda_input.camera_data->lane_curvature_change_first_right);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_curvature_change_second_left",
                          lcda_debug_data->lcda_input.camera_data->lane_curvature_change_second_left);
   LCDA_STORE_VAL_MGR_WPR("camera_data_lane_curvature_change_second_right",
                          lcda_debug_data->lcda_input.camera_data->lane_curvature_change_second_right);

   /* Lane change counter */
   LCDA_STORE_VAL_MGR_WPR("Lane_Change_Counter_left", lcda_debug_data->lcda_debug_output.pre_run.lane_change_counter[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lane_Change_Counter_right", lcda_debug_data->lcda_debug_output.pre_run.lane_change_counter[FBK_SIDE_RIGHT]);

   /* Camera lane plausibilisation counter */
   LCDA_STORE_VAL_MGR_WPR("Camera_Lane_Plausibilisation_Counter_left",
                          lcda_debug_data->lcda_debug_output.pre_run.camera_lane_plausibilisation_counter[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Camera_Lane_Plausibilisation_Counter_right",
                          lcda_debug_data->lcda_debug_output.pre_run.camera_lane_plausibilisation_counter[FBK_SIDE_RIGHT]);
}

#endif /* BINARY_DEBUG */
