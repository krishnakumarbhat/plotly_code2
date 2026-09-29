/**
 * @file pt_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "pt_debug_writer.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include "pt_debug_interface.h"
#include "pt_output_t.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Pt_Write_Bin_File(void)
{
   uint8_t i;

   /* Get debug data. */
   Pt_Debug_Data_T *pt_debug_data = Pt_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != pt_debug_data);

   /* Log the PT core input. */

   /* Log the PT core output. */
   PT_STORE_VAL_MGR_WPR("path_out_enable_flag", pt_debug_data->pt_core_output.f_pt_operational);
   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_segment_heading_diff_nearest_path",
                                  pt_debug_data->pt_core_output.nearest_path_output[i].segment_heading_diff, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_path_range_vcs_proj_to_path_segment_nearest_path",
                                  pt_debug_data->pt_core_output.nearest_path_output[i].range_vcs_proj_to_path_segment, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_track_idx_nearest_path",
                                  pt_debug_data->pt_core_output.nearest_path_output[i].track_idx_nearest_path, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_path_heading", pt_debug_data->pt_core_output.path_obj_pair_output[i].path_heading, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_track_match", pt_debug_data->pt_core_output.path_obj_pair_output[i].track_match, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_track_match_last_cycle",
                                  pt_debug_data->pt_core_output.path_obj_pair_output[i].track_match_last_cycle, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_track_match_age",
                                  pt_debug_data->pt_core_output.path_obj_pair_output[i].track_match_age, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_range_at_zero", pt_debug_data->pt_core_output.path_obj_pair_output[i].range_at_zero, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_range_at_host_edge",
                                  pt_debug_data->pt_core_output.path_obj_pair_output[i].range_at_host_edge, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_range_to_current_path_part",
                                  pt_debug_data->pt_core_output.path_obj_pair_output[i].range_to_current_path_part, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_length_of_trajectory",
                                  pt_debug_data->pt_core_output.path_obj_pair_output[i].length_of_trajectory, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_path_direction", pt_debug_data->pt_core_output.path_obj_pair_output[i].path_direction, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_out_path_state", pt_debug_data->pt_core_output.path_obj_pair_output[i].path_state, i);
   }


   /* Log the persistent data.*/


   /* Cal & SW version */
   PT_STORE_VAL_MGR_WPR("k_pt_cal_version", pt_debug_data->pt_calibration.Header.version);
   PT_STORE_VAL_MGR_WPR("Pt_Sw_Major_Version", pt_debug_data->pt_version.pt_sw_major_version);
   PT_STORE_VAL_MGR_WPR("Pt_Sw_Minor_Version", pt_debug_data->pt_version.pt_sw_minor_version);

   /* Write path information */
   PT_STORE_VAL_MGR_WPR("path_number", pt_debug_data->pt_debug_output.pt_paths.number_of_paths);
   PT_STORE_VAL_MGR_WPR("path_amount_finished_paths", pt_debug_data->pt_debug_output.pt_paths.number_of_finished_paths);

   for (i = 0; i < PT_NUMBER_OF_PATHS; i++)
   {
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_point_status", pt_debug_data->pt_debug_output.pt_paths.paths[i].new_path_point_status, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_border_status", pt_debug_data->pt_debug_output.pt_paths.paths[i].path_border_status, i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("path_direction", pt_debug_data->pt_debug_output.pt_paths.paths[i].direction, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_used_object_id",
                                  pt_debug_data->pt_debug_output.pt_paths.paths[i].obj_curr_used_for_path_build.id, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_used_object_age",
                                  pt_debug_data->pt_debug_output.pt_paths.paths[i].obj_curr_used_for_path_build.age, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_first_p", pt_debug_data->pt_debug_output.pt_paths.paths[i].first_p, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_last_p", pt_debug_data->pt_debug_output.pt_paths.paths[i].last_p, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_state", pt_debug_data->pt_debug_output.pt_paths.paths[i].path_state, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_age", pt_debug_data->pt_debug_output.pt_paths.paths[i].path_age, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_num_groupings", pt_debug_data->pt_debug_output.pt_paths.paths[i].num_groupings, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_first_point_x", pt_debug_data->pt_debug_output.pt_paths.paths[i].first.x, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_first_point_y", pt_debug_data->pt_debug_output.pt_paths.paths[i].first.y, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_last_point_x", pt_debug_data->pt_debug_output.pt_paths.paths[i].last_mat.x, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_last_point_y", pt_debug_data->pt_debug_output.pt_paths.paths[i].last_mat.y, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_index", pt_debug_data->pt_debug_output.pt_paths.paths[i].path_index, i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_max_speed_on_path", pt_debug_data->pt_debug_output.pt_paths.paths[i].max_speed, i);

      /* Write debug info for path reset reason */
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_reset_reason", pt_debug_data->pt_debug_output.Pt_Debug_Reset_Reason[i], i);
   }

   for (i = FBK_ZERO_UINT; i < PT_NUM_GRID_POINTS; i++)
   {
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_0", pt_debug_data->pt_debug_output.pt_paths.paths[0].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_1", pt_debug_data->pt_debug_output.pt_paths.paths[1].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_2", pt_debug_data->pt_debug_output.pt_paths.paths[2].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_3", pt_debug_data->pt_debug_output.pt_paths.paths[3].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_4", pt_debug_data->pt_debug_output.pt_paths.paths[4].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_5", pt_debug_data->pt_debug_output.pt_paths.paths[5].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_6", pt_debug_data->pt_debug_output.pt_paths.paths[6].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_7", pt_debug_data->pt_debug_output.pt_paths.paths[7].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_8", pt_debug_data->pt_debug_output.pt_paths.paths[8].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_9", pt_debug_data->pt_debug_output.pt_paths.paths[9].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_10", pt_debug_data->pt_debug_output.pt_paths.paths[10].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_11", pt_debug_data->pt_debug_output.pt_paths.paths[11].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_12", pt_debug_data->pt_debug_output.pt_paths.paths[12].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_13", pt_debug_data->pt_debug_output.pt_paths.paths[13].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_14", pt_debug_data->pt_debug_output.pt_paths.paths[14].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_15", pt_debug_data->pt_debug_output.pt_paths.paths[15].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_16", pt_debug_data->pt_debug_output.pt_paths.paths[16].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_17", pt_debug_data->pt_debug_output.pt_paths.paths[17].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_18", pt_debug_data->pt_debug_output.pt_paths.paths[18].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_19", pt_debug_data->pt_debug_output.pt_paths.paths[19].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_20", pt_debug_data->pt_debug_output.pt_paths.paths[20].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_21", pt_debug_data->pt_debug_output.pt_paths.paths[21].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_22", pt_debug_data->pt_debug_output.pt_paths.paths[22].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_23", pt_debug_data->pt_debug_output.pt_paths.paths[23].path_points[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_lat_points_24", pt_debug_data->pt_debug_output.pt_paths.paths[24].path_points[i], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("grid_pt_array", pt_debug_data->pt_core_input.grid_pt_array[i], i);
   }

   /* Write_Pure_Rotation_Path */
   for (i = FBK_ZERO_UINT; i < PT_NUMBER_OF_PATHS; i++)
   {
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_f_ext_lower_border",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Rotation_Data.f_path_extended_lower_border[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_f_ext_upper_border",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Rotation_Data.f_path_extended_upper_border[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_f_shrinked_lower_border",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Rotation_Data.f_path_shrinked_lower_border[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_f_shrinked_upper_border",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Rotation_Data.f_path_shrinked_upper_border[i], i);
   }

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_confidence_factor_total",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_confidence_factor_total[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_dist_border_to_isect_confidence",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_dist_border_to_isect_confidence[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_dist_obj_to_border_confidence",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_dist_obj_to_border_confidence[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_dist_obj_to_path_confidence",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_dist_obj_to_path_confidence[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_similarity_trail_path_confidence",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_similarity_trail_path_confidence[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_heading_diff_confidence",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_heading_diff_confidence[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_heading_segment_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_heading_segment_confidence[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_heading_segment_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_heading_segment_confidence[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_heading_segment_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_heading_segment_confidence[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("best_path_index", pt_debug_data->pt_debug_output.Pt_Debug_Internal.best_path_index[i], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("path_match_reason", pt_debug_data->pt_debug_output.Pt_Debug_Match_Reason[i], i);

#ifdef SRF_FULL_DEBUG_MODE

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_obj_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_border_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_border_to_isect_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_dist_obj_to_path_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_similarity_trail_path_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[i][24], i);


      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_heading_diff_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_confidence_factor_total_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.confidence_factor_total[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_closest_to_successive_pt_heading_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[i][24], i);


      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_rad_object_to_path_dist_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_relevant_dist_comp_matching_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][3], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][4], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][5], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][6], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][7], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][8], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][9], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][10], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][11], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][12], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][13], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][14], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][15], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][16], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][17], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][18], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][19], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][20], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][21], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][22], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][23], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_weighted_mean_diff_last_points_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[i][24], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][0][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][1][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][2][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][3][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][4][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][5][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][6][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][7][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][8][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][9][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][10][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][11][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][12][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][13][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][14][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][15][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][16][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][17][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][18][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][19][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][20][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][21][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][22][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][23][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_diff_pt_segment_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][24][0], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][0][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][1][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][2][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][3][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][4][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][5][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][6][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][7][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][8][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][9][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][10][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][11][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][12][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][13][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][14][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][15][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][16][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][17][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][18][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][19][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][20][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][21][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][22][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][23][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_diff_pt_segment_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][24][1], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][0][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][1][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][2][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][3][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][4][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][5][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][6][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][7][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][8][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][9][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][10][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][11][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][12][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][13][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][14][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][15][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][16][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][17][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][18][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][19][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][20][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][21][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][22][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][23][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_diff_pt_segment_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[i][24][2], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][0][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][1][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][2][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][3][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][4][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][5][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][6][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][7][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][8][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][9][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][10][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][11][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][12][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][13][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][14][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][15][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][16][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][17][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][18][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][19][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][20][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][21][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][22][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][23][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_heading_segment_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][24][0], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][0][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][1][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][2][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][3][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][4][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][5][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][6][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][7][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][8][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][9][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][10][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][11][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][12][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][13][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][14][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][15][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][16][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][17][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][18][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][19][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][20][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][21][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][22][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][23][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_heading_segment_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][24][1], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][0][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][1][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][2][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][3][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][4][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][5][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][6][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][7][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][8][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][9][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][10][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][11][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][12][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][13][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][14][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][15][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][16][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][17][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][18][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][19][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][20][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][21][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][22][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][23][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_heading_segment_confidence_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[i][24][2], i);


      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][0][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][1][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][2][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][3][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][4][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][5][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][6][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][7][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][8][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][9][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][10][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][11][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][12][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][13][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][14][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][15][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][16][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][17][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][18][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][19][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][20][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][21][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][22][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][23][0], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_first_weights_to_heading_segments_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][24][0], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][0][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][1][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][2][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][3][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][4][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][5][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][6][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][7][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][8][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][9][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][10][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][11][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][12][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][13][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][14][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][15][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][16][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][17][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][18][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][19][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][20][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][21][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][22][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][23][1], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_second_weights_to_heading_segments_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][24][1], i);

      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_0",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][0][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_1",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][1][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_2",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][2][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_3",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][3][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_4",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][4][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_5",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][5][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_6",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][6][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_7",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][7][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_8",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][8][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_9",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][9][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_10",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][10][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_11",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][11][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_12",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][12][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_13",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][13][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_14",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][14][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_15",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][15][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_16",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][16][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_17",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][17][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_18",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][18][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_19",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][19][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_20",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][20][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_21",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][21][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_22",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][22][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_23",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][23][2], i);
      PT_STORE_ARRAY_ELEM_MGR_WPR("all_third_weights_to_heading_segments_24",
                                  pt_debug_data->pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[i][24][2], i);

#endif // PT_FULL_DEBUG_MODE
   }
}

#endif /* BINARY_DEBUG */
