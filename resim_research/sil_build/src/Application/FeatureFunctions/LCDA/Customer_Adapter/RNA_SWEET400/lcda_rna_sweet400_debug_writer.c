/**
 * @file lcda_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_rna_sweet400_debug_writer.h"
#include "lcda_debug_writer.h"
#include "lcda_rna_sweet400_debug_interface.h"
#include "pa_reuse.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Lcda_Rna_Sweet400_Write_Bin_File(void)
{
   /* Get debug data. */
   Lcda_Rna_Sweet400_Debug_Data_T *lcda_debug_data = Lcda_Get_Rna_Sweet400_Debug_Data();

   /* Check input parameters. */
   assert(NULL != lcda_debug_data);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_pos_x_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_curvi_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_pos_y_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_curvi_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_vel_x_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_curvi_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_vel_y_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_curvi_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_pos_x_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_vcs_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_pos_y_0", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_vcs_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_vel_x_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_vcs_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_vel_y_0", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_vcs_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_ttc_0", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_ttc);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_motion_class_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_motion_class);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_change_status_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_change_status);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_object_class_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_object_class);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_object_id_0", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_obj_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_tracker_id_0", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_tracker_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_alert_condition_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[0].lka_alert_condition);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_pos_x_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_curvi_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_pos_y_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_curvi_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_vel_x_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_curvi_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_vel_y_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_curvi_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_pos_x_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_vcs_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_pos_y_1", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_vcs_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_vel_x_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_vcs_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_vel_y_1", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_vcs_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_ttc_1", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_ttc);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_motion_class_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_motion_class);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_change_status_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_change_status);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_object_class_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_object_class);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_object_id_1", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_obj_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_tracker_id_1", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_tracker_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_alert_condition_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[1].lka_alert_condition);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_pos_x_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_curvi_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_pos_y_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_curvi_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_vel_x_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_curvi_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_vel_y_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_curvi_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_pos_x_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_vcs_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_pos_y_2", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_vcs_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_vel_x_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_vcs_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_vel_y_2", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_vcs_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_ttc_2", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_ttc);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_motion_class_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_motion_class);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_change_status_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_change_status);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_object_class_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_object_class);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_object_id_2", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_obj_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_tracker_id_2", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_tracker_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_alert_condition_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[2].lka_alert_condition);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_pos_x_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_curvi_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_pos_y_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_curvi_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_vel_x_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_curvi_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_curvi_vel_y_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_curvi_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_pos_x_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_vcs_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_pos_y_3", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_vcs_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_vel_x_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_vcs_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_vcs_vel_y_3", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_vcs_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_ttc_3", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_ttc);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_motion_class_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_motion_class);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_change_status_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_change_status);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_object_class_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_object_class);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_object_id_3", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_obj_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_tracker_id_3", lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_tracker_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_left_alert_condition_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Left[3].lka_alert_condition);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_pos_x_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_curvi_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_pos_y_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_curvi_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_vel_x_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_curvi_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_vel_y_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_curvi_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_pos_x_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_vcs_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_pos_y_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_vcs_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_vel_x_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_vcs_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_vel_y_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_vcs_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_ttc_0", lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_ttc);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_motion_class_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_motion_class);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_change_status_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_change_status);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_object_class_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_object_class);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_object_id_0", lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_obj_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_tracker_id_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_tracker_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_alert_condition_0",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[0].lka_alert_condition);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_pos_x_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_curvi_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_pos_y_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_curvi_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_vel_x_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_curvi_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_vel_y_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_curvi_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_pos_x_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_vcs_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_pos_y_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_vcs_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_vel_x_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_vcs_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_vel_y_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_vcs_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_ttc_1", lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_ttc);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_motion_class_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_motion_class);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_change_status_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_change_status);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_object_class_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_object_class);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_object_id_1", lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_obj_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_tracker_id_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_tracker_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_alert_condition_1",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[1].lka_alert_condition);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_pos_x_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_curvi_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_pos_y_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_curvi_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_vel_x_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_curvi_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_vel_y_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_curvi_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_pos_x_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_vcs_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_pos_y_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_vcs_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_vel_x_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_vcs_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_vel_y_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_vcs_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_ttc_2", lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_ttc);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_motion_class_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_motion_class);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_change_status_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_change_status);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_object_class_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_object_class);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_object_id_2", lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_obj_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_tracker_id_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_tracker_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_alert_condition_2",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[2].lka_alert_condition);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_pos_x_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_curvi_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_pos_y_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_curvi_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_vel_x_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_curvi_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_curvi_vel_y_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_curvi_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_pos_x_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_vcs_pos_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_pos_y_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_vcs_pos_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_vel_x_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_vcs_vel_long);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_vcs_vel_y_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_vcs_vel_lat);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_ttc_3", lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_ttc);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_motion_class_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_motion_class);

   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_change_status_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_change_status);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_object_class_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_object_class);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_object_id_3", lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_obj_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_tracker_id_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_tracker_id);
   LCDA_STORE_VAL_MGR_WPR("LKA_obj_right_alert_condition_3",
                          lcda_debug_data->lcda_output.customer_output.LKA_Object_Right[3].lka_alert_condition);

   LCDA_STORE_VAL_MGR_WPR("LKA_zone_lat_start", lcda_debug_data->lcda_debug_output.lka_zone.lat_start);
   LCDA_STORE_VAL_MGR_WPR("LKA_zone_lat_end", lcda_debug_data->lcda_debug_output.lka_zone.lat_end);
   LCDA_STORE_VAL_MGR_WPR("LKA_zone_lon_start", lcda_debug_data->lcda_debug_output.lka_zone.lon_start);
   LCDA_STORE_VAL_MGR_WPR("LKA_zone_lon_end", lcda_debug_data->lcda_debug_output.lka_zone.lon_end);

   LCDA_STORE_VAL_MGR_WPR("LKA_zone_hys_lat_start", lcda_debug_data->lcda_debug_output.lka_zone_hys.lat_start);
   LCDA_STORE_VAL_MGR_WPR("LKA_zone_hys_lat_end", lcda_debug_data->lcda_debug_output.lka_zone_hys.lat_end);
   LCDA_STORE_VAL_MGR_WPR("LKA_zone_hys_lon_start", lcda_debug_data->lcda_debug_output.lka_zone_hys.lon_start);
   LCDA_STORE_VAL_MGR_WPR("LKA_zone_hys_lon_end", lcda_debug_data->lcda_debug_output.lka_zone_hys.lon_end);
}

#endif /* BINARY_DEBUG */
