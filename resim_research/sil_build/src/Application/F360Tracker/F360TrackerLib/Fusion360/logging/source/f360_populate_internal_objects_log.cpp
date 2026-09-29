/*===================================================================================*\
* FILE:  f360_populate_internal_objects_log.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/
#include "f360_populate_internal_objects_log.h"

namespace f360_variant_A
{
   void Populate_Internal_Objects_Data(F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calibrations,
      const F360_Internal_Object_T(&internal_objects_log)[NUMBER_OF_OBJECT_TRACKS])
   {
      for (uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         if ((0.0F <= internal_objects_log[i].time_since_initialization) && (0U < internal_objects_log[i].id))
         {
            const uint16_t obj_idx = internal_objects_log[i].id - 1U;

            if (F360_TRACKER_TRKFLTR_CTCA == objects[obj_idx].trk_fltr_type)
            {
               objects[obj_idx].errcov[0][2] = internal_objects_log[i].other_state_covariance[0];
               objects[obj_idx].errcov[2][0] = internal_objects_log[i].other_state_covariance[0];
               objects[obj_idx].errcov[0][3] = internal_objects_log[i].other_state_covariance[1];
               objects[obj_idx].errcov[3][0] = internal_objects_log[i].other_state_covariance[1];
               objects[obj_idx].errcov[0][4] = internal_objects_log[i].other_state_covariance[2];
               objects[obj_idx].errcov[4][0] = internal_objects_log[i].other_state_covariance[2];
               objects[obj_idx].errcov[0][5] = internal_objects_log[i].other_state_covariance[3];
               objects[obj_idx].errcov[5][0] = internal_objects_log[i].other_state_covariance[3];
               objects[obj_idx].errcov[1][2] = internal_objects_log[i].other_state_covariance[4];
               objects[obj_idx].errcov[2][1] = internal_objects_log[i].other_state_covariance[4];
               objects[obj_idx].errcov[1][3] = internal_objects_log[i].other_state_covariance[5];
               objects[obj_idx].errcov[3][1] = internal_objects_log[i].other_state_covariance[5];
               objects[obj_idx].errcov[1][4] = internal_objects_log[i].other_state_covariance[6];
               objects[obj_idx].errcov[4][1] = internal_objects_log[i].other_state_covariance[6];
               objects[obj_idx].errcov[1][5] = internal_objects_log[i].other_state_covariance[7];
               objects[obj_idx].errcov[5][1] = internal_objects_log[i].other_state_covariance[7];
               objects[obj_idx].errcov[2][3] = internal_objects_log[i].other_state_covariance[8];
               objects[obj_idx].errcov[3][2] = internal_objects_log[i].other_state_covariance[8];
               objects[obj_idx].errcov[2][5] = internal_objects_log[i].other_state_covariance[9];
               objects[obj_idx].errcov[5][2] = internal_objects_log[i].other_state_covariance[9];
               objects[obj_idx].errcov[3][4] = internal_objects_log[i].other_state_covariance[10];
               objects[obj_idx].errcov[4][3] = internal_objects_log[i].other_state_covariance[10];
               objects[obj_idx].errcov[4][5] = internal_objects_log[i].other_state_covariance[11];
               objects[obj_idx].errcov[5][4] = internal_objects_log[i].other_state_covariance[11];

            }
            else if (F360_TRACKER_TRKFLTR_CCA == objects[obj_idx].trk_fltr_type)
            {
               objects[obj_idx].errcov[0][1] = internal_objects_log[i].other_state_covariance[0]; // px w vx
               objects[obj_idx].errcov[1][0] = internal_objects_log[i].other_state_covariance[0];
               objects[obj_idx].errcov[0][2] = internal_objects_log[i].other_state_covariance[1]; // px w ax
               objects[obj_idx].errcov[2][0] = internal_objects_log[i].other_state_covariance[1];
               objects[obj_idx].errcov[0][4] = internal_objects_log[i].other_state_covariance[2]; // px w vy
               objects[obj_idx].errcov[4][0] = internal_objects_log[i].other_state_covariance[2];
               objects[obj_idx].errcov[0][5] = internal_objects_log[i].other_state_covariance[3]; // px w ay
               objects[obj_idx].errcov[5][0] = internal_objects_log[i].other_state_covariance[3];
               objects[obj_idx].errcov[1][2] = internal_objects_log[i].other_state_covariance[4]; // vx w ax
               objects[obj_idx].errcov[2][1] = internal_objects_log[i].other_state_covariance[4];
               objects[obj_idx].errcov[1][3] = internal_objects_log[i].other_state_covariance[5]; // vx w py
               objects[obj_idx].errcov[3][1] = internal_objects_log[i].other_state_covariance[5];
               objects[obj_idx].errcov[1][5] = internal_objects_log[i].other_state_covariance[6]; // vx w ay
               objects[obj_idx].errcov[5][1] = internal_objects_log[i].other_state_covariance[6];
               objects[obj_idx].errcov[2][3] = internal_objects_log[i].other_state_covariance[7]; // ax w py
               objects[obj_idx].errcov[3][2] = internal_objects_log[i].other_state_covariance[7];
               objects[obj_idx].errcov[2][4] = internal_objects_log[i].other_state_covariance[8]; // ax w vy
               objects[obj_idx].errcov[4][2] = internal_objects_log[i].other_state_covariance[8];
               objects[obj_idx].errcov[3][4] = internal_objects_log[i].other_state_covariance[9]; // py w vy
               objects[obj_idx].errcov[4][3] = internal_objects_log[i].other_state_covariance[9];
               objects[obj_idx].errcov[3][5] = internal_objects_log[i].other_state_covariance[10]; // py w ay
               objects[obj_idx].errcov[5][3] = internal_objects_log[i].other_state_covariance[10];
               objects[obj_idx].errcov[4][5] = internal_objects_log[i].other_state_covariance[11]; // vy w ay
               objects[obj_idx].errcov[5][4] = internal_objects_log[i].other_state_covariance[11];
            }
            else // For CCV, mapping the errcov to CCA, filter type will be updated to CCA later 
            {
               objects[obj_idx].errcov[0][1] = internal_objects_log[i].other_state_covariance[0]; // px w vx
               objects[obj_idx].errcov[1][0] = internal_objects_log[i].other_state_covariance[0];
               objects[obj_idx].errcov[0][2] = 0.0F; // px w ax
               objects[obj_idx].errcov[2][0] = 0.0F;
               objects[obj_idx].errcov[0][4] = internal_objects_log[i].other_state_covariance[1]; // px w vy
               objects[obj_idx].errcov[4][0] = internal_objects_log[i].other_state_covariance[1];
               objects[obj_idx].errcov[0][5] = 0.0F; // px w ay
               objects[obj_idx].errcov[5][0] = 0.0F;
               objects[obj_idx].errcov[1][2] = 0.0F; // vx w ax
               objects[obj_idx].errcov[2][1] = 0.0F;
               objects[obj_idx].errcov[1][3] = internal_objects_log[i].other_state_covariance[2]; // vx w py
               objects[obj_idx].errcov[3][1] = internal_objects_log[i].other_state_covariance[2];
               objects[obj_idx].errcov[1][5] = 0.0F; // vx w ay
               objects[obj_idx].errcov[5][1] = 0.0F;
               objects[obj_idx].errcov[2][3] = 0.0F; // ax w py
               objects[obj_idx].errcov[3][2] = 0.0F;
               objects[obj_idx].errcov[2][4] = 0.0F; // ax w vy
               objects[obj_idx].errcov[4][2] = 0.0F;
               objects[obj_idx].errcov[3][4] = internal_objects_log[i].other_state_covariance[3]; // py w vy
               objects[obj_idx].errcov[4][3] = internal_objects_log[i].other_state_covariance[3];
               objects[obj_idx].errcov[3][5] = 0.0F; // py w ay
               objects[obj_idx].errcov[5][3] = 0.0F;
               objects[obj_idx].errcov[4][5] = 0.0F; // vy w ay
               objects[obj_idx].errcov[5][4] = 0.0F;
            }

            objects[obj_idx].orth_delta_filtered = internal_objects_log[i].orth_delta_filtered;
            objects[obj_idx].orth_gap_filtered = internal_objects_log[i].orth_gap_filtered;
            objects[obj_idx].orth_range_rate_diff_filtered = internal_objects_log[i].orth_range_rate_diff_filtered;
            objects[obj_idx].filtered_pos_diff_heading = internal_objects_log[i].filtered_pos_diff_heading;
            objects[obj_idx].time_since_initialization = internal_objects_log[i].time_since_initialization;
            objects[obj_idx].filtered_dets = internal_objects_log[i].filtered_dets;
            objects[obj_idx].prev_avrg_conf_level = internal_objects_log[i].prev_avrg_conf_level;
            objects[obj_idx].length_uncertainty = internal_objects_log[i].length_uncertainty;
            objects[obj_idx].width_uncertainty = internal_objects_log[i].width_uncertainty;
            objects[obj_idx].mirror_prob = internal_objects_log[i].mirror_prob;
            objects[obj_idx].filtered_combined_nosep_mirror_prob = internal_objects_log[i].filtered_combined_nosep_mirror_prob;
            objects[obj_idx].historic_num_db_dets_with_forgetting_factor = internal_objects_log[i].historic_num_db_dets_with_forgetting_factor;
            objects[obj_idx].average_rcs = internal_objects_log[i].average_rcs;
            objects[obj_idx].hdg_ptng_disagmt = internal_objects_log[i].hdg_ptng_disagmt;
            if(F360_TRACKER_TRKFLTR_CCV == objects[obj_idx].trk_fltr_type)
            {                
               //initial value applied: cov of pointing filter
               (void)std::copy(cmn::begin(calibrations.init_cca_pnt_filter_cov), cmn::end(calibrations.init_cca_pnt_filter_cov), cmn::begin(objects[obj_idx].cca_pnt_filter_cov));
               objects[obj_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
            }
            else
            {
               objects[obj_idx].cca_pnt_filter_cov[0][0] = internal_objects_log[i].cca_pnt_filter_cov[0];
               objects[obj_idx].cca_pnt_filter_cov[1][1] = internal_objects_log[i].cca_pnt_filter_cov[1];
               objects[obj_idx].cca_pnt_filter_cov[0][1] = internal_objects_log[i].cca_pnt_filter_cov[2];
               objects[obj_idx].cca_pnt_filter_cov[1][0] = internal_objects_log[i].cca_pnt_filter_cov[2];
            }
            objects[obj_idx].filtered_hist_assoc_det_rr_err_mean = internal_objects_log[i].filtered_hist_assoc_det_rr_err_mean;
            objects[obj_idx].filtered_hist_assoc_det_rr_err_var = internal_objects_log[i].filtered_hist_assoc_det_rr_err_var;
            objects[obj_idx].filtered_hist_assoc_n_dets = internal_objects_log[i].filtered_hist_assoc_n_dets;
            objects[obj_idx].cntConsecutiveAmbiguous = internal_objects_log[i].cntConsecutiveAmbiguous;
            objects[obj_idx].cntConsecutiveMoving = internal_objects_log[i].cntConsecutiveMoving;
            objects[obj_idx].cntHostTurnForMirrorProb = internal_objects_log[i].cntHostTurnForMirrorProb;
            objects[obj_idx].direction_before_stopped = internal_objects_log[i].direction_before_stopped;
            objects[obj_idx].f_hide_occluded_track_behind_host = (1U == internal_objects_log[i].f_hide_occluded_track_behind_host);
            objects[obj_idx].cnt_consecutive_visible_from_rear = internal_objects_log[i].cnt_consecutive_visible_from_rear;
            objects[obj_idx].f_changed_direction_after_start = (1U == internal_objects_log[i].f_changed_direction_after_start);
            objects[obj_idx].total_reduced_dets = static_cast<uint32_t>(internal_objects_log[i].total_reduced_dets);
            objects[obj_idx].pseudo_pos_cov_outlier_count_orth = internal_objects_log[i].pseudo_pos_cov_outlier_count_orth;
            objects[obj_idx].id = static_cast<int32_t>(internal_objects_log[i].id);
            objects[obj_idx].num_updates_since_init = internal_objects_log[i].num_updates_since_init;
            objects[obj_idx].min_projection_reference_point = static_cast<F360_Reference_Point_T>(internal_objects_log[i].min_projection_reference_point);
            objects[obj_idx].behind_sep_id = internal_objects_log[i].behind_sep_id;
            objects[obj_idx].on_sep_id = internal_objects_log[i].on_sep_id;
            objects[obj_idx].conf_longitudinal_position = static_cast<CONF9_T>(internal_objects_log[i].conf_longitudinal_position);
            objects[obj_idx].conf_lateral_position = static_cast<CONF9_T>(internal_objects_log[i].conf_lateral_position);
            objects[obj_idx].conf_speed = static_cast<CONF9_T>(internal_objects_log[i].conf_speed);
            objects[obj_idx].conf_overall = static_cast<CONF3_T>(internal_objects_log[i].conf_overall);
            objects[obj_idx].low_rcs_dets_cnt = internal_objects_log[i].low_rcs_dets_cnt;
            objects[obj_idx].f_ghost_NU_2_C = (1U == internal_objects_log[i].f_ghost_NU_2_C);
            objects[obj_idx].f_overlapping_with_object = (1U == internal_objects_log[i].f_overlapping_with_object);
            objects[obj_idx].otg_height = internal_objects_log[i].ud_mov_historic_height_mean;
            objects[obj_idx].ud_mov_historic_ndets = internal_objects_log[i].ud_mov_historic_ndets;
            objects[obj_idx].ud_mov_cnt_underdrivable = internal_objects_log[i].ud_mov_cnt_underdrivable;
            objects[obj_idx].ud_overdrivable_det_pct = internal_objects_log[i].ud_overdrivable_det_pct;
            objects[obj_idx].time_since_last_stop = internal_objects_log[i].time_since_last_stop;
            objects[obj_idx].cntConsecutiveStopped = internal_objects_log[i].cntConsecutiveStopped;
            objects[obj_idx].f_prevent_orientation_std_decrease  = (1U == internal_objects_log[i].f_prevent_orientation_std_decrease);
            objects[obj_idx].orientation_std = internal_objects_log[i].orientation_std;
            objects[obj_idx].time_since_split = internal_objects_log[i].time_since_split;
            objects[obj_idx].prev_predicted_vcs_y_pos = internal_objects_log[i].prev_predicted_vcs_y_pos;
            objects[obj_idx].cca_cross_moving_buffer_index = internal_objects_log[i].cca_cross_moving_buffer_index;
            objects[obj_idx].bbox.Set_Length(internal_objects_log[i].bbox_length);
            objects[obj_idx].bbox.Set_Width(internal_objects_log[i].bbox_width);
            objects[obj_idx].time_since_started_move = internal_objects_log[i].time_since_started_move;
            objects[obj_idx].pseudo_hdg = internal_objects_log[i].pseudo_hdg;
            (void)std::copy(cmn::begin(internal_objects_log[i].pseudo_hdg_state_vec), cmn::end(internal_objects_log[i].pseudo_hdg_state_vec), cmn::begin(objects[obj_idx].pseudo_hdg_state_vec));
            (void)std::copy(cmn::begin(internal_objects_log[i].cca_cross_moving_buffer), cmn::end(internal_objects_log[i].cca_cross_moving_buffer), cmn::begin(objects[obj_idx].cca_cross_moving_buffer));
            objects[obj_idx].maximum_rcs = internal_objects_log[i].maximum_rcs;
            objects[obj_idx].otg_height_raw = internal_objects_log[i].otg_height_raw;
            objects[obj_idx].assoc_dets_pct_filtered = internal_objects_log[i].assoc_dets_pct_filtered;
            objects[obj_idx].bbox_center_otg_altitude = internal_objects_log[i].bbox_center_otg_altitude;
            objects[obj_idx].bbox_height = internal_objects_log[i].bbox_height;
            objects[obj_idx].filtered_rr_err_max_gap = internal_objects_log[i].filtered_rr_err_max_gap;
            objects[obj_idx].filtered_mean_tcs_y_pos_of_lower_rr_err_bin = internal_objects_log[i].filtered_mean_tcs_y_pos_of_lower_rr_err_bin;
            objects[obj_idx].filtered_mean_tcs_y_pos_of_higher_rr_err_bin = internal_objects_log[i].filtered_mean_tcs_y_pos_of_higher_rr_err_bin;
            objects[obj_idx].f_shrink_fast = (1U == internal_objects_log[i].f_shrink_fast);
            objects[obj_idx].f_suspectable_for_det_drop = (1U == internal_objects_log[i].f_suspectable_for_det_drop);
            objects[obj_idx].time_since_obj_considered_veh_for_class_freeze = internal_objects_log[i].time_since_obj_considered_veh_for_class_freeze;
            objects[obj_idx].predicted_vcs_position.y = internal_objects_log[i].predicted_vcs_position_y;
         }
      }
   }

   void Populate_Internal_Objects_Log_Data(F360_Internal_Object_T(&internal_objects_log)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS], const int32_t num_active_objects,
      const int32_t(&active_obj_ids)[NUMBER_OF_OBJECT_TRACKS])
   {
      for (int32_t i = 0; i < num_active_objects; i++)
      {
         const int32_t obj_idx = active_obj_ids[i] - 1;

         if (F360_TRACKER_TRKFLTR_CTCA == objects[obj_idx].trk_fltr_type)
         {
            internal_objects_log[i].other_state_covariance[0] = objects[obj_idx].errcov[0][2];
            internal_objects_log[i].other_state_covariance[1] = objects[obj_idx].errcov[0][3];
            internal_objects_log[i].other_state_covariance[2] = objects[obj_idx].errcov[0][4];
            internal_objects_log[i].other_state_covariance[3] = objects[obj_idx].errcov[0][5];
            internal_objects_log[i].other_state_covariance[4] = objects[obj_idx].errcov[1][2];
            internal_objects_log[i].other_state_covariance[5] = objects[obj_idx].errcov[1][3];
            internal_objects_log[i].other_state_covariance[6] = objects[obj_idx].errcov[1][4];
            internal_objects_log[i].other_state_covariance[7] = objects[obj_idx].errcov[1][5];
            internal_objects_log[i].other_state_covariance[8] = objects[obj_idx].errcov[2][3];
            internal_objects_log[i].other_state_covariance[9] = objects[obj_idx].errcov[2][5];
            internal_objects_log[i].other_state_covariance[10] = objects[obj_idx].errcov[3][4];
            internal_objects_log[i].other_state_covariance[11] = objects[obj_idx].errcov[4][5];
         }
         else
         {
            internal_objects_log[i].other_state_covariance[0] = objects[obj_idx].errcov[0][1]; // px w vx
            internal_objects_log[i].other_state_covariance[1] = objects[obj_idx].errcov[0][2]; // px w ax
            internal_objects_log[i].other_state_covariance[2] = objects[obj_idx].errcov[0][4]; // px w vy
            internal_objects_log[i].other_state_covariance[3] = objects[obj_idx].errcov[0][5]; // px w ay
            internal_objects_log[i].other_state_covariance[4] = objects[obj_idx].errcov[1][2]; // vx w ax
            internal_objects_log[i].other_state_covariance[5] = objects[obj_idx].errcov[1][3]; // vx w py
            internal_objects_log[i].other_state_covariance[6] = objects[obj_idx].errcov[1][5]; // vx w ay
            internal_objects_log[i].other_state_covariance[7] = objects[obj_idx].errcov[2][3]; // ax w py
            internal_objects_log[i].other_state_covariance[8] = objects[obj_idx].errcov[2][4]; // ax w vy
            internal_objects_log[i].other_state_covariance[9] = objects[obj_idx].errcov[3][4]; // py w vy
            internal_objects_log[i].other_state_covariance[10] = objects[obj_idx].errcov[3][5]; // py w ay
            internal_objects_log[i].other_state_covariance[11] = objects[obj_idx].errcov[4][5]; // vy w ay
         }
         internal_objects_log[i].orth_delta_filtered = objects[obj_idx].orth_delta_filtered;
         internal_objects_log[i].orth_gap_filtered = objects[obj_idx].orth_gap_filtered;
         internal_objects_log[i].orth_range_rate_diff_filtered = objects[obj_idx].orth_range_rate_diff_filtered;
         internal_objects_log[i].filtered_pos_diff_heading = objects[obj_idx].filtered_pos_diff_heading;
         internal_objects_log[i].time_since_initialization = objects[obj_idx].time_since_initialization;
         internal_objects_log[i].filtered_dets = objects[obj_idx].filtered_dets;
         internal_objects_log[i].prev_avrg_conf_level = objects[obj_idx].prev_avrg_conf_level;
         internal_objects_log[i].length_uncertainty = objects[obj_idx].length_uncertainty;
         internal_objects_log[i].width_uncertainty = objects[obj_idx].width_uncertainty;
         internal_objects_log[i].mirror_prob = objects[obj_idx].mirror_prob;
         internal_objects_log[i].filtered_combined_nosep_mirror_prob = objects[obj_idx].filtered_combined_nosep_mirror_prob;
         internal_objects_log[i].historic_num_db_dets_with_forgetting_factor = objects[obj_idx].historic_num_db_dets_with_forgetting_factor;
         internal_objects_log[i].average_rcs = objects[obj_idx].average_rcs;
         internal_objects_log[i].hdg_ptng_disagmt = objects[obj_idx].hdg_ptng_disagmt;
         internal_objects_log[i].cca_pnt_filter_cov[0] = objects[obj_idx].cca_pnt_filter_cov[0][0];
         internal_objects_log[i].cca_pnt_filter_cov[1] = objects[obj_idx].cca_pnt_filter_cov[1][1];
         internal_objects_log[i].cca_pnt_filter_cov[2] = objects[obj_idx].cca_pnt_filter_cov[0][1];
         internal_objects_log[i].filtered_hist_assoc_det_rr_err_mean = objects[obj_idx].filtered_hist_assoc_det_rr_err_mean;
         internal_objects_log[i].filtered_hist_assoc_det_rr_err_var = objects[obj_idx].filtered_hist_assoc_det_rr_err_var;
         internal_objects_log[i].filtered_hist_assoc_n_dets = objects[obj_idx].filtered_hist_assoc_n_dets;
         internal_objects_log[i].cntConsecutiveAmbiguous = objects[obj_idx].cntConsecutiveAmbiguous;
         internal_objects_log[i].cntConsecutiveMoving = objects[obj_idx].cntConsecutiveMoving;
         internal_objects_log[i].cntHostTurnForMirrorProb = objects[obj_idx].cntHostTurnForMirrorProb;
         internal_objects_log[i].direction_before_stopped = objects[obj_idx].direction_before_stopped;
         internal_objects_log[i].f_hide_occluded_track_behind_host = objects[obj_idx].f_hide_occluded_track_behind_host? 1U : 0U;
         internal_objects_log[i].cnt_consecutive_visible_from_rear = objects[obj_idx].cnt_consecutive_visible_from_rear;
         internal_objects_log[i].f_changed_direction_after_start = objects[obj_idx].f_changed_direction_after_start ? 1U : 0U;
         internal_objects_log[i].total_reduced_dets = static_cast<int32_t>(objects[obj_idx].total_reduced_dets);
         internal_objects_log[i].pseudo_pos_cov_outlier_count_orth = objects[obj_idx].pseudo_pos_cov_outlier_count_orth;
         internal_objects_log[i].id = static_cast<uint16_t>(objects[obj_idx].id);
         internal_objects_log[i].num_updates_since_init = objects[obj_idx].num_updates_since_init;
         internal_objects_log[i].min_projection_reference_point = static_cast<uint8_t>(objects[obj_idx].min_projection_reference_point);
         internal_objects_log[i].behind_sep_id = objects[obj_idx].behind_sep_id;
         internal_objects_log[i].on_sep_id = objects[obj_idx].on_sep_id;
         internal_objects_log[i].conf_longitudinal_position = static_cast<uint8_t>(objects[obj_idx].conf_longitudinal_position);
         internal_objects_log[i].conf_lateral_position = static_cast<uint8_t>(objects[obj_idx].conf_lateral_position);
         internal_objects_log[i].conf_speed = static_cast<uint8_t>(objects[obj_idx].conf_speed);
         internal_objects_log[i].conf_overall = static_cast<uint8_t>(objects[obj_idx].conf_overall);
         internal_objects_log[i].low_rcs_dets_cnt = objects[obj_idx].low_rcs_dets_cnt;
         internal_objects_log[i].f_ghost_NU_2_C = objects[obj_idx].f_ghost_NU_2_C ? 1U : 0U;
         internal_objects_log[i].f_overlapping_with_object = objects[obj_idx].f_overlapping_with_object ? 1U : 0U;
         internal_objects_log[i].ud_mov_historic_height_mean = objects[obj_idx].otg_height;
         internal_objects_log[i].ud_mov_historic_ndets = objects[obj_idx].ud_mov_historic_ndets;
         internal_objects_log[i].ud_overdrivable_det_pct = objects[obj_idx].ud_overdrivable_det_pct;
         internal_objects_log[i].ud_mov_cnt_underdrivable = objects[obj_idx].ud_mov_cnt_underdrivable;
         internal_objects_log[i].time_since_last_stop = objects[obj_idx].time_since_last_stop;
         internal_objects_log[i].cntConsecutiveStopped = objects[obj_idx].cntConsecutiveStopped;
         internal_objects_log[i].f_prevent_orientation_std_decrease = objects[obj_idx].f_prevent_orientation_std_decrease ? 1U : 0U;
         internal_objects_log[i].orientation_std = objects[obj_idx].orientation_std;
         internal_objects_log[i].time_since_split = objects[obj_idx].time_since_split;
         internal_objects_log[i].prev_predicted_vcs_y_pos = objects[obj_idx].prev_predicted_vcs_y_pos;
         internal_objects_log[i].cca_cross_moving_buffer_index = objects[obj_idx].cca_cross_moving_buffer_index;
         internal_objects_log[i].bbox_width = objects[obj_idx].bbox.Get_Width();
         internal_objects_log[i].bbox_length = objects[obj_idx].bbox.Get_Length();
         internal_objects_log[i].time_since_started_move = objects[obj_idx].time_since_started_move;
         internal_objects_log[i].pseudo_hdg = objects[obj_idx].pseudo_hdg;
         (void)std::copy(cmn::begin(objects[obj_idx].pseudo_hdg_state_vec), cmn::end(objects[obj_idx].pseudo_hdg_state_vec), cmn::begin(internal_objects_log[i].pseudo_hdg_state_vec));
         (void)std::copy(cmn::begin(objects[obj_idx].cca_cross_moving_buffer), cmn::end(objects[obj_idx].cca_cross_moving_buffer), cmn::begin(internal_objects_log[i].cca_cross_moving_buffer));
         internal_objects_log[i].maximum_rcs = objects[obj_idx].maximum_rcs;
         internal_objects_log[i].otg_height_raw = objects[obj_idx].otg_height_raw;
         internal_objects_log[i].assoc_dets_pct_filtered = objects[obj_idx].assoc_dets_pct_filtered;
         internal_objects_log[i].bbox_center_otg_altitude = objects[obj_idx].bbox_center_otg_altitude;
         internal_objects_log[i].bbox_height = objects[obj_idx].bbox_height;
         internal_objects_log[i].filtered_rr_err_max_gap = objects[obj_idx].filtered_rr_err_max_gap;
         internal_objects_log[i].filtered_mean_tcs_y_pos_of_lower_rr_err_bin = objects[obj_idx].filtered_mean_tcs_y_pos_of_lower_rr_err_bin;
         internal_objects_log[i].filtered_mean_tcs_y_pos_of_higher_rr_err_bin = objects[obj_idx].filtered_mean_tcs_y_pos_of_higher_rr_err_bin;
         internal_objects_log[i].f_shrink_fast = objects[obj_idx].f_shrink_fast ? 1U : 0U;
         internal_objects_log[i].f_suspectable_for_det_drop = objects[obj_idx].f_suspectable_for_det_drop ? 1U : 0U;
         internal_objects_log[i].time_since_obj_considered_veh_for_class_freeze = objects[obj_idx].time_since_obj_considered_veh_for_class_freeze;
         internal_objects_log[i].predicted_vcs_position_y = objects[obj_idx].predicted_vcs_position.y;
      }
   }
}
