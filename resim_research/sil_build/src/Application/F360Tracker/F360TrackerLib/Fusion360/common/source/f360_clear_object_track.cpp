/*===================================================================================*\
* FILE: f360_clear_object_track.cpp
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec:
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   The file contains the defination of Clear_Object_Track function
*
* ABBREVIATIONS:
*
* TRACEABILITY INFO:
*   Design Document(s): initObjTrk.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

#include <cstring>
#include "f360_clear_object_track.h"
#include <algorithm>
#include "f360_iterator.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Clear_Object_Track
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T & object_track
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function resets the object properties to default values
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Clear_Object_Track(
      F360_Object_Track_T& object_track)
   {
      object_track.pseudo_hdg_state_vec[0] = 0.0F;
      object_track.pseudo_hdg_state_vec[1] = 0.0F;
      object_track.pseudo_hdg_state_vec[2] = 0.0F;
      object_track.pseudo_hdg_state_vec[3] = 0.0F;
      object_track.pseudo_hdg_state_vec[4] = 0.0F;
      object_track.pseudo_hdg_state_vec[5] = 0.0F;
      object_track.pseudo_hdg = 0.0F;

      object_track.pseudo_vcs_position.x = 0.0F;
      object_track.pseudo_vcs_position.y = 0.0F;
      object_track.speed = 0.0F;
      object_track.predicted_speed = 0.0F;
      object_track.hdg_ptng_disagmt = 0.0F;
      object_track.curvature = 0.0F;
      object_track.tang_accel = 0.0F;

      object_track.vcs_position.x = 0.0F;
      object_track.vcs_position.y = 0.0F;
      object_track.predicted_vcs_position.x = 0.0F;
      object_track.predicted_vcs_position.y = 0.0F;

      object_track.vcs_velocity.longitudinal = 0.0F;
      object_track.vcs_velocity.lateral = 0.0F;
      object_track.predicted_vcs_velocity.longitudinal = 0.0F;
      object_track.predicted_vcs_velocity.lateral = 0.0F;

      object_track.vcs_accel.longitudinal = 0.0F;
      object_track.vcs_accel.lateral = 0.0F;

      (void)object_track.vcs_heading.Value(0.0F);
      object_track.bbox = {};
      object_track.width_processed = 0.0F;
      object_track.length_processed = 0.0F;
      object_track.orientation_std = F360_PI;

      object_track.status = F360_OBJECT_STATUS_INVALID;
      object_track.occlusion_status = OCCLUSION_STATUS_UNDEFINED;

      object_track.time_since_cluster_created = -1.0F;
      object_track.time_since_track_updated = -1.0F;
      object_track.time_since_downselected = -1.0F;
      object_track.time_since_split = -1.0F;

      for (uint8_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
      {
         for (uint8_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
         {
            object_track.errcov[row_idx][col_idx] = 0.0F;
         }
      }

      object_track.init_scheme = F360_TRACK_INIT_INVALID;

      object_track.ndets = 0U;
      std::fill(cmn::begin(object_track.detids), cmn::end(object_track.detids), 0U);

      object_track.num_rr_inlier_dets = 0U;
      object_track.num_dets_used_in_rr_msmt_update = 0U;
      object_track.slow_moving_cluster_id = 0U;
      object_track.num_members_in_slow_moving_obj_cluster = 0U;
      object_track.length_of_slow_moving_obj_cluster = 0.0F;
      object_track.f_moving = false;
      object_track.f_moveable = false;
      object_track.f_oncoming = false;
      object_track.f_hide_occluded_track_behind_host = false;
      object_track.f_vehicular_trk = false;
      object_track.f_changed_direction_after_start = false;
      object_track.direction_before_stopped = 0;
      object_track.cnt_consecutive_visible_from_rear = 0;

      object_track.movable_prob = 0.0F;
      object_track.pseudo_pos_cov_outlier_count_orth = 0;

      object_track.number_of_events_of_multipath_with_forgetting_factor = 0.0F;
      object_track.time_since_initialization_with_forgetting_factor = 0.0F;

      object_track.mirror_prob = 0.0F;
      object_track.filtered_combined_nosep_mirror_prob = 0.0F;
      object_track.num_db_dets = 0U;
      object_track.historic_num_db_dets_with_forgetting_factor = 0.0F;
      object_track.length_uncertainty = 0.0F;
      object_track.width_uncertainty = 0.0F;

      object_track.reduced_id = F360_INVALID_REDUCED_ID;
      object_track.reduced_status = F360_OBJECT_STATUS_INVALID;
      object_track.unique_id = F360_INVALID_UNSIGNED_ID;

      object_track.cntConsecutiveAmbiguous = 0;
      object_track.cntConsecutiveMoving = 0;
      object_track.cntConsecutiveStopped = 0;
      object_track.cntHostTurnForMirrorProb = 0;

      object_track.raw_confidence_level = 0.0F;
      object_track.confidenceLevel = 0.0F;
      object_track.prev_avrg_conf_level = 0.0F;

      object_track.time_since_stage_start = -1.0F;

      object_track.num_types_of_dets[0] = 0;
      object_track.num_types_of_dets[1] = 0;
      for (uint8_t row_idx = 0U; row_idx < F360_PSEUDO_MEAS_DIM; row_idx++)
      {
         for (uint8_t col_idx = 0U; col_idx < F360_PSEUDO_MEAS_DIM; col_idx++)
         {
            object_track.meascov[row_idx][col_idx] = 0.0F;
         }
      }

      object_track.long_buffer_zone_len1 = 0.0F;
      object_track.long_buffer_zone_len2 = 0.0F;
      object_track.lat_buffer_zone_wid1 = 0.0F;
      object_track.lat_buffer_zone_wid2 = 0.0F;

      object_track.time_since_initialization = -1.0F;
      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_INVALID;
      object_track.aeb_confidence = AEB_CONF_INVALID;

      //intention to set 0.0 as it has been observed some ghosts are born with moving flag to true
      //but quickly siwtched to non-moving. Moveable reset logic takes action at this case.
      object_track.time_since_last_stop = 0.0F;
      object_track.time_since_started_move = -1.0F;

      object_track.total_reduced_dets = 0U;
      object_track.filtered_dets = 0.0F;

      object_track.f_ghost_NU_2_C = false;
      object_track.f_overlapping_with_object = false;

      object_track.time_since_measurement = -1.0F;

      object_track.priority = 0.0F;
      object_track.p_higher_priority_track = NULL;
      object_track.p_lower_priority_track = NULL;

      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;

      object_track.object_class = F360_OBJ_CLASS_UNDETERMINED;

      object_track.f_prevent_orientation_std_decrease = false;

      object_track.dead_zone_status = F360_Dead_Zone_Status_T::UNDEFINED;

      object_track.exist_prob = 0.0F;
      object_track.p_track_state = 0.0F;

      object_track.probability_pedestrian = 0.0F;
      object_track.probability_car = 0.0F;
      object_track.probability_bicycle = 0.0F;
      object_track.probability_motorcycle = 0.0F;
      object_track.probability_truck = 0.0F;
      object_track.probability_undet = 0.0F;

      object_track.time_since_obj_considered_veh_for_class_freeze = -1.0F;

      object_track.probability_underdrivable_ocg = 0.0F;

      object_track.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      object_track.drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;
      object_track.drivable_confidence_sg = 0U;
      object_track.drivable_sg_dist_to_segment_sq = INFTY;

      object_track.lsc_next_in_cluster = NULL;
      object_track.lsc_prev_in_cluster = NULL;

      object_track.behind_sep_id = F360_INVALID_UNSIGNED_ID;
      object_track.on_sep_id = F360_INVALID_UNSIGNED_ID;
      object_track.f_behind_sep_ambiguous = false;
      object_track.sep_intersection_point.x = INFTY;
      object_track.sep_intersection_point.y = INFTY;

      object_track.conf_longitudinal_position = CONF9_NONE;
      object_track.conf_lateral_position = CONF9_NONE;
      object_track.conf_speed = CONF9_NONE;
      object_track.conf_overall = CONF3_NONE;
      object_track.low_rcs_dets_cnt = 0U;

      object_track.orth_delta_filtered = 0.0F;
      object_track.orth_gap_filtered = 0.0F;
      object_track.orth_range_rate_diff_filtered = 0.0F;
      object_track.prev_vcs_center_pos.x = 0.0F;
      object_track.prev_vcs_center_pos.y = 0.0F;
      object_track.filtered_pos_diff_heading = INFTY;

      object_track.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 0.0F;
      object_track.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = 0.0F;
      object_track.filtered_rr_err_max_gap = 0.0F;
      object_track.split_type = 0U;

      object_track.average_rcs = -INFTY;
      object_track.maximum_rcs = 0.0F;

      object_track.heading_rate = 0.0F;
      object_track.cca_pnt_filter_cov[0][0] = 0.0F;
      object_track.cca_pnt_filter_cov[0][1] = 0.0F;
      object_track.cca_pnt_filter_cov[1][0] = 0.0F;
      object_track.cca_pnt_filter_cov[1][1] = 0.0F;
      object_track.num_updates_since_init = 0U;

      object_track.filtered_hist_assoc_det_rr_err_mean = 0.0F;
      object_track.filtered_hist_assoc_det_rr_err_var = 0.0F;
      object_track.filtered_hist_assoc_n_dets = 0.0F;

      object_track.otg_height = 0.0F;
      object_track.otg_height_raw = 0.0F;
      object_track.ud_mov_historic_ndets = 0.0F;
      object_track.ud_mov_cnt_underdrivable = 0U;
      object_track.ud_overdrivable_det_pct = 0.0F;

      object_track.prev_predicted_vcs_y_pos = 0.0F;

      object_track.cca_cross_moving_buffer_index = 0U;

      object_track.num_dets_in_ext_bbox = 0U;
      object_track.assoc_dets_pct_filtered = 1.0F;

      object_track.idm_det_fraction = 0.0F;

      for (uint8_t idx = 0U; idx < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; idx++)
      {
         object_track.cca_cross_moving_buffer[idx] = 0;
      }

      object_track.average_grid_search_tcs_position.x = INFTY;
      object_track.average_grid_search_tcs_position.y = INFTY;

      object_track.bbox_center_otg_altitude = 0.0F;
      object_track.bbox_height = 0.0F;
      object_track.obstacle_prob = 0.0F;

      object_track.f_shrink_fast = false;
      object_track.f_suspectable_for_det_drop = false;
   }
}

