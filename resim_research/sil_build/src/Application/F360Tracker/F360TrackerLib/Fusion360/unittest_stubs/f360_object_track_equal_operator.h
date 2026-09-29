/*===================================================================================*\
* FILE:  f360_object_track_equal_operator.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Equality operator of two object tracks is defined here for use in UTs.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef F360_OBJECT_TRACK_EQUAL_OPERATOR_H
#define F360_OBJECT_TRACK_EQUAL_OPERATOR_H

#include "f360_object_track.h"

namespace f360_variant_A
{
   inline bool operator==(const F360_Object_Track_T& track_1, const F360_Object_Track_T& track_2)
   {
      bool are_equal = track_1.pseudo_vcs_position == track_2.pseudo_vcs_position;
      are_equal = are_equal && (std::abs(track_1.speed - track_2.speed) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.predicted_speed - track_2.predicted_speed) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.hdg_ptng_disagmt - track_2.hdg_ptng_disagmt) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.curvature - track_2.curvature) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.tang_accel - track_2.tang_accel) < F360_EPSILON);
      are_equal = are_equal && (track_1.vcs_position == track_2.vcs_position);
      are_equal = are_equal && (track_1.predicted_vcs_position == track_2.predicted_vcs_position);
      are_equal = are_equal && (std::abs(track_1.vcs_velocity.lateral - track_2.vcs_velocity.lateral) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.vcs_velocity.longitudinal - track_2.vcs_velocity.longitudinal) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.predicted_vcs_velocity.lateral - track_2.predicted_vcs_velocity.lateral) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.predicted_vcs_velocity.longitudinal - track_2.predicted_vcs_velocity.longitudinal) < F360_EPSILON);
      are_equal = are_equal && (track_1.vcs_accel == track_2.vcs_accel);
      are_equal = are_equal && (std::abs(track_1.vcs_heading.Value() - track_2.vcs_heading.Value()) < F360_EPSILON);
      are_equal = are_equal && (track_1.bbox.Get_Orientation() == track_2.bbox.Get_Orientation());
      are_equal = are_equal && (std::abs(track_1.bbox.Get_Length() - track_2.bbox.Get_Length()) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.bbox.Get_Width() - track_2.bbox.Get_Width()) < F360_EPSILON);
      are_equal = are_equal && (track_1.status == track_2.status);
      are_equal = are_equal && (std::abs(track_1.time_since_cluster_created - track_2.time_since_cluster_created) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.time_since_track_updated - track_2.time_since_track_updated) < F360_EPSILON);

      for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
      {
         for (uint32_t j = 0U; j < STATE_DIMENSION; j++)
         {
            are_equal = are_equal && (std::abs(track_1.errcov[i][j] - track_2.errcov[i][j]) < F360_EPSILON);
         }
      }

      are_equal = are_equal && (track_1.init_scheme == track_2.init_scheme);
      are_equal = are_equal && (track_1.ndets == track_2.ndets);

      for (uint32_t i = 0U; i < MAX_DETS_IN_OBJ_TRK; i++)
      {
         are_equal = are_equal && (track_1.detids[i] == track_2.detids[i]);
      }

      are_equal = are_equal && (track_1.num_rr_inlier_dets == track_2.num_rr_inlier_dets);
      are_equal = are_equal && (track_1.f_moving == track_2.f_moving);
      are_equal = are_equal && (track_1.movable_prob == track_2.movable_prob);
      are_equal = are_equal && (track_1.f_oncoming == track_2.f_oncoming);
      are_equal = are_equal && (track_1.f_vehicular_trk == track_2.f_vehicular_trk);
      are_equal = are_equal && (std::abs(track_1.mirror_prob - track_2.mirror_prob) < F360_EPSILON);
      are_equal = are_equal && (track_1.id == track_2.id);
      are_equal = are_equal && (track_1.reduced_id == track_2.reduced_id);
      are_equal = are_equal && (track_1.reduced_status == track_2.reduced_status);
      are_equal = are_equal && (track_1.cntConsecutiveAmbiguous == track_2.cntConsecutiveAmbiguous);
      are_equal = are_equal && (track_1.cntConsecutiveMoving == track_2.cntConsecutiveMoving);
      are_equal = are_equal && (std::abs(track_1.raw_confidence_level - track_2.raw_confidence_level) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.confidenceLevel - track_2.confidenceLevel) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.prev_avrg_conf_level - track_2.prev_avrg_conf_level) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.time_since_stage_start - track_2.time_since_stage_start) < F360_EPSILON);
      are_equal = are_equal && (track_1.num_types_of_dets[0] == track_2.num_types_of_dets[0]);
      are_equal = are_equal && (track_1.num_types_of_dets[1] == track_2.num_types_of_dets[1]);

      for (uint32_t i = 0U; i < F360_PSEUDO_MEAS_DIM; i++)
      {
         for (uint32_t j = 0U; j < F360_PSEUDO_MEAS_DIM; j++)
         {
            are_equal = are_equal && (std::abs(track_1.meascov[i][j] - track_2.meascov[i][j]) < F360_EPSILON);
         }
      }

      for (uint32_t i = 0U; i < 2; i++)
      {
         for (uint32_t j = 0U; j < 2; j++)
         {
            are_equal = are_equal && (std::abs(track_1.cca_pnt_filter_cov[i][j] - track_2.cca_pnt_filter_cov[i][j]) < F360_EPSILON);
         }
      }

      are_equal = are_equal && (std::abs(track_1.long_buffer_zone_len2 - track_2.long_buffer_zone_len2) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.long_buffer_zone_len1 - track_2.long_buffer_zone_len1) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.lat_buffer_zone_wid2 - track_2.lat_buffer_zone_wid2) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.lat_buffer_zone_wid1 - track_2.lat_buffer_zone_wid1) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.time_since_initialization - track_2.time_since_initialization) < F360_EPSILON);
      are_equal = are_equal && (track_1.trk_fltr_type == track_2.trk_fltr_type);
      are_equal = are_equal && (track_1.total_reduced_dets == track_2.total_reduced_dets);
      are_equal = are_equal && (std::abs(track_1.filtered_dets - track_2.filtered_dets) < F360_EPSILON);
      are_equal = are_equal && (track_1.f_ghost_NU_2_C == track_2.f_ghost_NU_2_C);
      are_equal = are_equal && (track_1.f_overlapping_with_object == track_2.f_overlapping_with_object);
      are_equal = are_equal && (std::abs(track_1.time_since_measurement - track_2.time_since_measurement) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.priority - track_2.priority) < F360_EPSILON);
      are_equal = are_equal && (track_1.p_higher_priority_track == track_2.p_higher_priority_track);
      are_equal = are_equal && (track_1.p_lower_priority_track == track_2.p_lower_priority_track);
      are_equal = are_equal && (track_1.reference_point == track_2.reference_point);
      are_equal = are_equal && (track_1.object_class == track_2.object_class);
      are_equal = are_equal && (track_1.vcs_heading == track_2.vcs_heading);
      are_equal = are_equal && (track_1.dead_zone_status == track_2.dead_zone_status);
      are_equal = are_equal && (std::abs(track_1.exist_prob - track_2.exist_prob) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.p_track_state - track_2.p_track_state) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.probability_pedestrian - track_2.probability_pedestrian) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.probability_car - track_2.probability_car) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.probability_bicycle - track_2.probability_bicycle) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.probability_motorcycle - track_2.probability_motorcycle) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.probability_truck - track_2.probability_truck) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.probability_undet - track_2.probability_undet) < F360_EPSILON);
      are_equal = are_equal && (track_1.underdrivable_status_ocg == track_2.underdrivable_status_ocg);
      are_equal = are_equal && (track_1.lsc_next_in_cluster == track_2.lsc_next_in_cluster);
      are_equal = are_equal && (track_1.lsc_prev_in_cluster == track_2.lsc_prev_in_cluster);
      are_equal = are_equal && (track_1.behind_sep_id == track_2.behind_sep_id);
      are_equal = are_equal && (track_1.on_sep_id == track_2.on_sep_id);
      are_equal = are_equal && (track_1.f_behind_sep_ambiguous == track_2.f_behind_sep_ambiguous);
      are_equal = are_equal && (std::abs(track_1.sep_intersection_point.x - track_2.sep_intersection_point.x) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.sep_intersection_point.y - track_2.sep_intersection_point.y) < F360_EPSILON);
      are_equal = are_equal && (track_1.conf_longitudinal_position == track_2.conf_longitudinal_position);
      are_equal = are_equal && (track_1.conf_lateral_position == track_2.conf_lateral_position);
      are_equal = are_equal && (track_1.conf_speed == track_2.conf_speed);
      are_equal = are_equal && (track_1.conf_overall == track_2.conf_overall);
      are_equal = are_equal && (std::abs(track_1.orth_delta_filtered - track_2.orth_delta_filtered) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.orth_gap_filtered - track_2.orth_gap_filtered) < F360_EPSILON);
      are_equal = are_equal && (track_1.prev_vcs_center_pos == track_2.prev_vcs_center_pos);
      are_equal = are_equal && (std::abs(track_1.filtered_pos_diff_heading - track_2.filtered_pos_diff_heading) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.heading_rate - track_2.heading_rate) < F360_EPSILON);
      are_equal = are_equal && (std::abs(track_1.num_updates_since_init - track_2.num_updates_since_init) < F360_EPSILON);
      return are_equal;
   }
}
#endif


