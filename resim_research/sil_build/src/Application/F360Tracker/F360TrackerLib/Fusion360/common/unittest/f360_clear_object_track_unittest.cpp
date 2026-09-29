/** \file
   This file contains unit tests for f360_clear_object_track.cpp
*/

#include "f360_clear_object_track.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_clear_object_track
 *  @{
**/


/** \brief
*  Unit test cases for common module.
*  This module includes test cases for f360_clear_object_track.cpp.
**/
TEST_GROUP(f360_clear_object_track)
{
};

/**
*\purpose Checks that all fields of object properties in F360_Object_Track_T
* struct has been cleared after calling Clear_Object_Track(), i.e.
* that floats equal 0.0F, that signed integers equal 0, that unsigned
* integers equal 0U, that booleans equal false and that enums equal
* invalid/unknown/undetermined.
*\req    NA
*/
TEST(f360_clear_object_track, TestAllFieldsBeenCleared)
{
   /** \step{1}
    * Call the Clear_Object_Track() function and verify that all object
    * properties has been cleared.
    **/

   /** \precond
   * Previous to calling the  Clear_Object_Track() function, all object
   * properties should have a value that differs from the default value
   * to be set by the function.
   **/
   F360_Object_Track_T obj_track;
   F360_Object_Track_T obj_track_prior_h;
   F360_Object_Track_T obj_track_prior_l;
   const int32_t orig_obj_id = 2;
   uint32_t expected_size_of_obj_track = 1068U; /* should be equal to sizeof(obj_tracks)*/
   if (sizeof(void *) == 8)
   {
      expected_size_of_obj_track += 28;
   }

   obj_track.speed = 101.1F;
   obj_track.predicted_speed = 101.1F;

   obj_track.hdg_ptng_disagmt = 101.1F;

   obj_track.curvature = 101.1F;

   obj_track.tang_accel = 101.1F;

   obj_track.vcs_position.x = 101.1F;
   obj_track.vcs_position.y = 101.1F;

   obj_track.predicted_vcs_position.x = 101.1F;
   obj_track.predicted_vcs_position.y = 101.1F;

   obj_track.pseudo_vcs_position.x = 101.1F;
   obj_track.pseudo_vcs_position.y = 101.1F;

   obj_track.vcs_velocity.longitudinal = 101.1F;
   obj_track.vcs_velocity.lateral = 101.1F;

   obj_track.predicted_vcs_velocity.longitudinal = 101.1F;
   obj_track.predicted_vcs_velocity.lateral = 101.1F;

   obj_track.vcs_accel.longitudinal = 101.1F;
   obj_track.vcs_accel.lateral = 101.1F;

   obj_track.otg_height = 10.0F;
   obj_track.otg_height_raw = 10.0F;

   obj_track.vcs_heading = Angle{ 101.1F };

   obj_track.Set_Bbox_Orientation(Angle{ 101.1F });

   obj_track.bbox.Set_Length(101.1F);

   obj_track.bbox.Set_Width(101.1F);

   obj_track.orientation_std = 101.1F;

   obj_track.status = F360_OBJECT_STATUS_UPDATED;

   obj_track.occlusion_status = OCCLUSION_STATUS_VISIBLE;

   obj_track.time_since_cluster_created = 101.1F;

   obj_track.time_since_track_updated = 101.1F;

   obj_track.time_since_downselected = 101.1F;

   obj_track.time_since_split = 101.1F;

   for(unsigned int row_ind = 0; row_ind < STATE_DIMENSION; row_ind++)
   {
      for(unsigned int col_ind = 0; col_ind < STATE_DIMENSION; col_ind++)
      {
         obj_track.errcov[row_ind][col_ind] = 101.0F;
      }
   }

   for(unsigned int row_ind = 0; row_ind < 2; row_ind++)
   {
      for(unsigned int col_ind = 0; col_ind < 2; col_ind++)
      {
         obj_track.cca_pnt_filter_cov[row_ind][col_ind] = 101.0F;
      }
   }

   obj_track.init_scheme = F360_TRACK_INIT_POSDIFF;

   obj_track.ndets = 101;

   for(unsigned int idx = 0; idx < MAX_DETS_IN_OBJ_TRK; idx++)
   {
      obj_track.detids[idx] = 101U;
   }

   obj_track.num_rr_inlier_dets = 101;
   obj_track.num_dets_used_in_rr_msmt_update = 101;
   obj_track.num_members_in_slow_moving_obj_cluster = 101;
   obj_track.slow_moving_cluster_id = 101;
   obj_track.length_of_slow_moving_obj_cluster = 101.1F;

   obj_track.length_uncertainty = 101.0F;
   obj_track.width_uncertainty = 101.0F;

   obj_track.f_moving = true;

   obj_track.movable_prob = 1.0F;

   obj_track.f_oncoming = true;

   obj_track.f_vehicular_trk = true;

   obj_track.f_hide_occluded_track_behind_host = true;

   obj_track.mirror_prob = 1.0F;

   obj_track.filtered_combined_nosep_mirror_prob = 1.0F;

   obj_track.historic_num_db_dets_with_forgetting_factor = 101.1F;

   obj_track.num_db_dets = 101;

   obj_track.id = orig_obj_id;

   obj_track.unique_id = 101U;

   obj_track.reduced_id = 101;

   obj_track.reduced_status = F360_OBJECT_STATUS_UPDATED;

   obj_track.cntConsecutiveAmbiguous = 101;

   obj_track.cntConsecutiveMoving = 101;

   obj_track.cntConsecutiveStopped = 101;

   obj_track.cntHostTurnForMirrorProb = 101;

   obj_track.raw_confidence_level = 101.1F;

   obj_track.confidenceLevel = 101.1F;

   obj_track.prev_avrg_conf_level = 101.1F;

   obj_track.time_since_stage_start = 101.1F;

   obj_track.num_types_of_dets[0] = 101;
   obj_track.num_types_of_dets[1] = 101;

   for(unsigned int row_ind = 0; row_ind < F360_PSEUDO_MEAS_DIM; row_ind++)
   {
      for(unsigned int col_ind = 0; col_ind < F360_PSEUDO_MEAS_DIM; col_ind++)
      {
         obj_track.meascov[row_ind][col_ind] = 101.0F;
      }
   }

   obj_track.long_buffer_zone_len1 = 101.1F;

   obj_track.long_buffer_zone_len2 = 101.1F;

   obj_track.lat_buffer_zone_wid1 = 101.1F;

   obj_track.lat_buffer_zone_wid2 = 101.1F;

   obj_track.time_since_initialization = 101.1F;

   obj_track.time_since_last_stop = 101.1F;

   obj_track.time_since_started_move = 101.1F;

   obj_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   obj_track.total_reduced_dets = 101;

   obj_track.filtered_dets = 101.1F;

   obj_track.f_ghost_NU_2_C = true;

   obj_track.f_overlapping_with_object = true;

   obj_track.time_since_measurement = 101.1F;

   obj_track.priority = 101.1F;

   obj_track.p_higher_priority_track = &obj_track_prior_h;
   obj_track.p_lower_priority_track = &obj_track_prior_l;

   obj_track.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   obj_track.min_projection_reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   obj_track.object_class = F360_OBJ_CLASS_BICYCLE;

   obj_track.f_prevent_orientation_std_decrease = true;

   obj_track.dead_zone_status = F360_Dead_Zone_Status_T::INSIDE;

   obj_track.exist_prob = 101.1F;

   obj_track.p_track_state = 101.1F;

   obj_track.probability_pedestrian = 101.1F;
   obj_track.probability_car = 101.1F;
   obj_track.probability_motorcycle = 101.1F;
   obj_track.probability_bicycle = 101.1F;
   obj_track.probability_truck = 101.1F;
   obj_track.probability_undet = 101.1F;
   obj_track.probability_underdrivable_ocg = 101.1F;

   obj_track.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   obj_track.drivable_confidence_sg = 100U;
   obj_track.drivable_status_sg = sg::SG_Drivability_Class_T::UNDERDRIVABLE;
   obj_track.drivable_sg_dist_to_segment_sq = 100.0F;

   obj_track.lsc_next_in_cluster = &obj_track_prior_h;
   obj_track.lsc_prev_in_cluster = &obj_track_prior_l;

   obj_track.behind_sep_id = 2U;
   obj_track.on_sep_id = 2U;
   obj_track.f_behind_sep_ambiguous = true;
   obj_track.sep_intersection_point.x = 101.1F;
   obj_track.sep_intersection_point.y = 101.1F;

   obj_track.conf_longitudinal_position = CONF9_LOW1;
   obj_track.conf_lateral_position = CONF9_LOW1;
   obj_track.conf_speed = CONF9_LOW1;
   obj_track.conf_overall = CONF3_LOW;

   obj_track.orth_delta_filtered = 4.0F;
   obj_track.orth_gap_filtered = 3.5F;
   obj_track.orth_range_rate_diff_filtered = 0.2F;
   obj_track.prev_vcs_center_pos.y = 20.0F;
   obj_track.prev_vcs_center_pos.x = 20.0F;
   obj_track.filtered_pos_diff_heading = F360_DEG2RAD(15.0F);

   obj_track.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 5.0F;
   obj_track.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = 7.0F;
   obj_track.filtered_rr_err_max_gap = 1.0F;
   obj_track.split_type = 2U;

   obj_track.filtered_hist_assoc_det_rr_err_mean = 101.1F;
   obj_track.filtered_hist_assoc_det_rr_err_var = 101.1F;
   obj_track.filtered_hist_assoc_n_dets = 101.1F;

   obj_track.average_rcs = 15.0F;
   obj_track.maximum_rcs = 15.0F;

   obj_track.idm_det_fraction = 0.5F;

   obj_track.low_rcs_dets_cnt = 1U;

   obj_track.num_updates_since_init = 10U;

   obj_track.prev_predicted_vcs_y_pos = 3.0F;

   obj_track.cca_cross_moving_buffer_index = 3U;

   obj_track.f_changed_direction_after_start = true;
   obj_track.direction_before_stopped = 1;

   obj_track.f_shrink_fast = true;
   obj_track.f_suspectable_for_det_drop = true;

   obj_track.f_moveable = true;
   obj_track.cnt_consecutive_visible_from_rear = 1;

   obj_track.number_of_events_of_multipath_with_forgetting_factor = 3.0F;
   obj_track.time_since_initialization_with_forgetting_factor = 2.0F;
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      {
         obj_track.cca_cross_moving_buffer[ind] = 1;
      }
   }

   obj_track.pseudo_pos_cov_outlier_count_orth = 2;

   obj_track.average_grid_search_tcs_position.x = 12.0F;
   obj_track.average_grid_search_tcs_position.y = 1.0F;

   obj_track.length_processed = 10.0F;
   obj_track.width_processed = 2.0F;

   obj_track.pseudo_hdg_state_vec[0] = 10.0F;
   obj_track.pseudo_hdg_state_vec[1] = 11.0F;
   obj_track.pseudo_hdg_state_vec[2] = 12.0F;
   obj_track.pseudo_hdg_state_vec[3] = 13.0F;
   obj_track.pseudo_hdg_state_vec[4] = 14.0F;
   obj_track.pseudo_hdg_state_vec[5] = 15.0F;
   obj_track.pseudo_hdg = 1.5F;

   obj_track.assoc_dets_pct_filtered = 0.8F;
   obj_track.num_dets_in_ext_bbox = 10U;

   obj_track.ud_mov_historic_ndets = 101.1F;
   obj_track.ud_mov_cnt_underdrivable = 101;
   obj_track.ud_overdrivable_det_pct = 0.7F;

   obj_track.bbox_center_otg_altitude = 1.0F;
   obj_track.bbox_height = 1.5F;
   obj_track.obstacle_prob = 0.5F;

   obj_track.aeb_confidence = AEB_CONF_HIGH;

   obj_track.time_since_obj_considered_veh_for_class_freeze = 5.0F;

   /** \action
   * Compute size of obj_track
   **/

   uint32_t size_of_obj_track = sizeof(obj_track);

   /** \result
   * Verify that F360_Object_Track_T interface has not been modified.
   * If this test fails then the code of this UT has to be modified
   * in the following ways:
   *    1) When obj_track is filled with data above, the newly
   *    created field in the interface has to been filled as well.
   *    Example: obj_track.new_field = 101.1F;
   *
   *    2) A new check has to be added below to verify that the newly
   *    created field in the interface has been properly cleared.
   *    Example: DOUBLES_EQUAL(0.0F, obj_track.new_field, 0.0F);
   *
   *    3) The assignment of a value to the expected_size_of_obj_trk
   *    variable in this test has to be modified such that
   *    expected_size_of_obj_trk corresponds to sizeof() the new
   *    F360_Object_Track_T interface.
   **/
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_obj_track, size_of_obj_track,
         "Failed check to verify that F360_Object_Track_T interface has not been modified.");

   /** \action
   * Call the function
   **/

   Clear_Object_Track(obj_track);

   /** \result
   * Verify that all fields of obj_track has been cleared except
   * the id field which should be unchanged.
   **/

   DOUBLES_EQUAL(0.0F, obj_track.pseudo_vcs_position.x, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.pseudo_vcs_position.y, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.speed, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.predicted_speed, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.hdg_ptng_disagmt, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.curvature, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.heading_rate, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.tang_accel, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.vcs_position.x, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.vcs_position.y, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.predicted_vcs_position.x, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.predicted_vcs_position.y, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.vcs_velocity.longitudinal, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.vcs_velocity.lateral, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.predicted_vcs_velocity.longitudinal, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.predicted_vcs_velocity.lateral, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.vcs_accel.longitudinal, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.vcs_accel.lateral, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.otg_height, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.otg_height_raw, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.vcs_heading.Value(), 0.0F);

   DOUBLES_EQUAL(F360_PI, obj_track.orientation_std, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.length_uncertainty, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.width_uncertainty, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.bbox.Get_Orientation().Value(), 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.bbox.Get_Length(), 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.bbox.Get_Width(), 0.0F);

   LONGS_EQUAL(F360_OBJECT_STATUS_INVALID, obj_track.status);
   CHECK_EQUAL(OCCLUSION_STATUS_UNDEFINED, obj_track.occlusion_status);

   DOUBLES_EQUAL(-1.0F, obj_track.time_since_cluster_created, 0.0F);
   DOUBLES_EQUAL(-1.0F, obj_track.time_since_track_updated, 0.0F);

   DOUBLES_EQUAL(-1.0F, obj_track.time_since_downselected, 0.0F);
   DOUBLES_EQUAL(-1.0F, obj_track.time_since_split, 0.0F);

   for(unsigned int row_ind = 0; row_ind < STATE_DIMENSION; row_ind++)
   {
      for(unsigned int col_ind = 0; col_ind < STATE_DIMENSION; col_ind++)
      {
         DOUBLES_EQUAL(0.0F, obj_track.errcov[row_ind][col_ind], 0.0F);
      }
   }

   for(unsigned int row_ind = 0; row_ind < 2; row_ind++)
   {
      for(unsigned int col_ind = 0; col_ind < 2; col_ind++)
      {
         DOUBLES_EQUAL(0.0F, obj_track.cca_pnt_filter_cov[row_ind][col_ind], 0.0F);
      }
   }

   CHECK_EQUAL(F360_TRACK_INIT_INVALID, obj_track.init_scheme);

   LONGS_EQUAL(0, obj_track.ndets);
   for(unsigned int idx = 0; idx < MAX_DETS_IN_OBJ_TRK; idx++)
   {
      UNSIGNED_LONGS_EQUAL(0U, obj_track.detids[idx]);
   }

   LONGS_EQUAL(0, obj_track.num_rr_inlier_dets);
   LONGS_EQUAL(0, obj_track.num_dets_used_in_rr_msmt_update);
   LONGS_EQUAL(0, obj_track.num_members_in_slow_moving_obj_cluster);
   LONGS_EQUAL(0, obj_track.slow_moving_cluster_id);

   DOUBLES_EQUAL(0.0F, obj_track.length_of_slow_moving_obj_cluster, 0.0F);

   CHECK_FALSE(obj_track.f_moving);
   CHECK_TRUE(obj_track.movable_prob < 0.5F);
   CHECK_FALSE(obj_track.f_oncoming);
   CHECK_FALSE(obj_track.f_vehicular_trk);

   CHECK_FALSE(obj_track.f_hide_occluded_track_behind_host);

   DOUBLES_EQUAL(0.0F, obj_track.mirror_prob, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.filtered_combined_nosep_mirror_prob, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.historic_num_db_dets_with_forgetting_factor, 0.0F);
   UNSIGNED_LONGS_EQUAL(0, obj_track.num_db_dets);

   UNSIGNED_LONGS_EQUAL(orig_obj_id, obj_track.id);

   UNSIGNED_LONGS_EQUAL(0U, obj_track.unique_id);

   LONGS_EQUAL(0, obj_track.reduced_id);

   LONGS_EQUAL(F360_OBJECT_STATUS_INVALID, obj_track.reduced_status);

   LONGS_EQUAL(0, obj_track.cntConsecutiveAmbiguous);
   LONGS_EQUAL(0, obj_track.cntConsecutiveMoving);

   LONGS_EQUAL(0, obj_track.cntConsecutiveStopped);
   LONGS_EQUAL(0, obj_track.cntHostTurnForMirrorProb);

   DOUBLES_EQUAL(0.0F, obj_track.raw_confidence_level, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.confidenceLevel, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.prev_avrg_conf_level, 0.0F);

   DOUBLES_EQUAL(-1.0F, obj_track.time_since_stage_start, 0.0F);

   LONGS_EQUAL(0, obj_track.num_types_of_dets[0]);
   LONGS_EQUAL(0, obj_track.num_types_of_dets[1]);

   for(unsigned int row_ind = 0; row_ind < F360_PSEUDO_MEAS_DIM; row_ind++)
   {
      for(unsigned int col_ind = 0; col_ind < F360_PSEUDO_MEAS_DIM; col_ind++)
      {
         DOUBLES_EQUAL(0.0F, obj_track.meascov[row_ind][col_ind], 0.0F);
      }
   }


   DOUBLES_EQUAL(0.0F, obj_track.long_buffer_zone_len1, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.long_buffer_zone_len2, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.lat_buffer_zone_wid1, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.lat_buffer_zone_wid2, 0.0F);


   DOUBLES_EQUAL(-1.0F, obj_track.time_since_initialization, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.time_since_last_stop, 0.0F);
   DOUBLES_EQUAL(-1.0F, obj_track.time_since_started_move, 0.0F);

   LONGS_EQUAL(F360_TRACKER_TRKFLTR_INVALID, obj_track.trk_fltr_type);

   LONGS_EQUAL(0, obj_track.total_reduced_dets);
   DOUBLES_EQUAL(0.0F, obj_track.filtered_dets, 0.0F);

   CHECK_FALSE(obj_track.f_ghost_NU_2_C);
   CHECK_FALSE(obj_track.f_overlapping_with_object);

   DOUBLES_EQUAL(-1.0F, obj_track.time_since_measurement, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.priority, 0.0F);

   CHECK_TRUE(obj_track.p_higher_priority_track == NULL);
   CHECK_TRUE(obj_track.p_lower_priority_track == NULL);

   LONGS_EQUAL(F360_REFERENCE_POINT_CENTER, obj_track.reference_point);
   LONGS_EQUAL(F360_REFERENCE_POINT_CENTER, obj_track.min_projection_reference_point);

   LONGS_EQUAL(F360_OBJ_CLASS_UNDETERMINED, obj_track.object_class);

   CHECK_FALSE(obj_track.f_prevent_orientation_std_decrease);

   CHECK_EQUAL(F360_Dead_Zone_Status_T::UNDEFINED, obj_track.dead_zone_status);

   DOUBLES_EQUAL(0.0F, obj_track.exist_prob, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.p_track_state, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.probability_pedestrian, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.probability_car, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.probability_motorcycle, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.probability_bicycle, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.probability_truck, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.probability_undet, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.probability_underdrivable_ocg, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.ud_mov_historic_ndets, 0.0F);
   UNSIGNED_LONGS_EQUAL(0, obj_track.ud_mov_cnt_underdrivable);

   CHECK_EQUAL(obj_track.underdrivable_status_ocg, ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
   CHECK_TRUE(obj_track.drivable_status_sg == sg::SG_Drivability_Class_T::UNCLASSIFIED);
   CHECK_EQUAL(obj_track.drivable_confidence_sg, 0U);
   DOUBLES_EQUAL(INFTY, obj_track.drivable_sg_dist_to_segment_sq, 0.0F);

   POINTERS_EQUAL(NULL, obj_track.lsc_next_in_cluster);
   POINTERS_EQUAL(NULL, obj_track.lsc_prev_in_cluster);

   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, obj_track.on_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, obj_track.behind_sep_id);
   CHECK_FALSE(obj_track.f_behind_sep_ambiguous);
   DOUBLES_EQUAL(INFTY, obj_track.sep_intersection_point.x, 0.0F);
   DOUBLES_EQUAL(INFTY, obj_track.sep_intersection_point.y, 0.0F);

   CHECK_EQUAL(obj_track.conf_longitudinal_position, CONF9_NONE);
   CHECK_EQUAL(obj_track.conf_lateral_position, CONF9_NONE);
   CHECK_EQUAL(obj_track.conf_speed, CONF9_NONE);
   CHECK_EQUAL(obj_track.conf_overall, CONF3_NONE);

   DOUBLES_EQUAL(0.0F, obj_track.orth_delta_filtered, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.orth_gap_filtered, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.orth_range_rate_diff_filtered, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.prev_vcs_center_pos.y, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.prev_vcs_center_pos.x, 0.0F);
   DOUBLES_EQUAL(INFTY, obj_track.filtered_pos_diff_heading, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.filtered_mean_tcs_y_pos_of_lower_rr_err_bin, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.filtered_mean_tcs_y_pos_of_higher_rr_err_bin, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.filtered_rr_err_max_gap, 0.0F);
   CHECK_EQUAL(0U, obj_track.split_type);

   CHECK_FALSE(obj_track.f_changed_direction_after_start);
   CHECK_EQUAL(obj_track.direction_before_stopped, 0);

   CHECK_FALSE(obj_track.f_shrink_fast);
   CHECK_FALSE(obj_track.f_suspectable_for_det_drop);

   CHECK_FALSE(obj_track.f_moveable);
   CHECK_EQUAL(obj_track.cnt_consecutive_visible_from_rear, 0);

   CHECK_EQUAL(0U, obj_track.num_updates_since_init);

   DOUBLES_EQUAL(0.0F, obj_track.pseudo_hdg_state_vec[0], 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.pseudo_hdg_state_vec[1], 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.pseudo_hdg_state_vec[2], 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.pseudo_hdg_state_vec[3], 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.pseudo_hdg_state_vec[4], 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.pseudo_hdg_state_vec[5], 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.pseudo_hdg, 0.0F);

   DOUBLES_EQUAL(1.0F, obj_track.assoc_dets_pct_filtered, 0.0F);
   CHECK_EQUAL(0U, obj_track.num_dets_in_ext_bbox);

   DOUBLES_EQUAL(0.0F, obj_track.filtered_hist_assoc_det_rr_err_mean , 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.filtered_hist_assoc_det_rr_err_var  , 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.filtered_hist_assoc_n_dets  , 0.0F);

   DOUBLES_EQUAL(-INFTY, obj_track.average_rcs, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.maximum_rcs, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.idm_det_fraction, 0.0F);

   CHECK_EQUAL(0, obj_track.low_rcs_dets_cnt);

   DOUBLES_EQUAL(0.0F, obj_track.prev_predicted_vcs_y_pos, 0.0F);

   CHECK_EQUAL(0, obj_track.cca_cross_moving_buffer_index);

   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      CHECK_EQUAL(0, obj_track.cca_cross_moving_buffer[ind]);
   }

   CHECK_EQUAL(0, obj_track.pseudo_pos_cov_outlier_count_orth);

   DOUBLES_EQUAL(0.0F, obj_track.number_of_events_of_multipath_with_forgetting_factor, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.time_since_initialization_with_forgetting_factor, 0.0F);

   DOUBLES_EQUAL(INFTY, obj_track.average_grid_search_tcs_position.x, 0.0F);
   DOUBLES_EQUAL(INFTY, obj_track.average_grid_search_tcs_position.y, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.length_processed, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.width_processed, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.ud_overdrivable_det_pct, 0.0F);

   DOUBLES_EQUAL(0.0F, obj_track.bbox_center_otg_altitude, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.bbox_height, 0.0F);
   DOUBLES_EQUAL(0.0F, obj_track.obstacle_prob, 0.0F);

   CHECK_EQUAL(obj_track.aeb_confidence, AEB_CONF_INVALID);

   DOUBLES_EQUAL(-1.0F, obj_track.time_since_obj_considered_veh_for_class_freeze, 0.0F);
}

/** @}*/
