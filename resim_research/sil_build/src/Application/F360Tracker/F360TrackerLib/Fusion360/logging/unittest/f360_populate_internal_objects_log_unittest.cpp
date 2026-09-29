/** \file
 * This file contains unit tests for content of f360_populate_internal_objects_log.cpp file
 */

#include "f360_populate_internal_objects_log.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_populate_internal_objects_log
 *  @{
 */

/** \brief
 * Test suit for functions in f360_populate_internal_objects_log.cpp
 * Verify those functions return the value as expected.
 */
TEST_GROUP(f360_populate_internal_objects_log)
{
   // Declare common variables used within all tests in this test group.
   F360_Internal_Object_T internal_objects_log[NUMBER_OF_OBJECT_TRACKS]{};
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS]{};
   int32_t num_active_objects = 0;
   int32_t active_obj_ids[NUMBER_OF_OBJECT_TRACKS]{};
};

/** \purpose  
 * Verify Populate_Internal_Objects_Log_Data returns the data as expected
 * \req NA
 */
TEST(f360_populate_internal_objects_log, Test_Populate_Internal_Objects_Log_Data)
{
   /** \precond
    * Initialized the input variables for Populate_Internal_Objects_Log_Data()
    */
   num_active_objects = 3;
   active_obj_ids[0] = 1;
   active_obj_ids[1] = 2;
   active_obj_ids[2] = 3;
   objects[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   objects[0].errcov[0][2] = 0.2F; 
   objects[0].errcov[0][3] = 0.2F;
   objects[0].errcov[0][4] = 0.2F;
   objects[0].errcov[0][5] = 0.2F;
   objects[0].errcov[1][2] = 0.2F;
   objects[0].errcov[1][3] = 0.2F;
   objects[0].errcov[1][4] = 0.2F;
   objects[0].errcov[1][5] = 0.2F;
   objects[0].errcov[2][3] = 0.2F;
   objects[0].errcov[2][5] = 0.2F;
   objects[0].errcov[3][4] = 0.2F;
   objects[0].errcov[4][5] = 0.2F;
   objects[0].orth_delta_filtered = 0.1F;
   objects[0].orth_gap_filtered = 0.05F;
   objects[0].filtered_pos_diff_heading = 0.02F;
   objects[0].time_since_initialization = 2.5F;
   objects[0].filtered_dets = 5.2F;
   objects[0].prev_avrg_conf_level = 0.7F;
   objects[0].length_uncertainty = 0.4F;
   objects[0].width_uncertainty = 0.4F;
   objects[0].mirror_prob = 0.1F;
   objects[0].average_rcs = 2.1F;
   objects[0].hdg_ptng_disagmt = 0.02F;
   objects[0].cca_pnt_filter_cov[0][0] = 0.1F;
   objects[0].cca_pnt_filter_cov[1][1] = 0.1F;
   objects[0].cca_pnt_filter_cov[0][1] = 0.1F;
   objects[0].filtered_hist_assoc_det_rr_err_mean = 0.1F;
   objects[0].filtered_hist_assoc_det_rr_err_var = 0.1F;
   objects[0].filtered_hist_assoc_n_dets = 4.1F;
   objects[0].cntConsecutiveAmbiguous = 1;
   objects[0].cntConsecutiveMoving = 50;
   objects[0].cntHostTurnForMirrorProb = 2;
   objects[0].total_reduced_dets = 4U;
   objects[0].id = 1;
   objects[0].num_updates_since_init = 40;
   objects[0].min_projection_reference_point = F360_REFERENCE_POINT_REAR;
   objects[0].behind_sep_id = 2;
   objects[0].on_sep_id = 0;
   objects[0].conf_longitudinal_position = CONF9_MED3;
   objects[0].conf_lateral_position = CONF9_MED2;
   objects[0].conf_speed = CONF9_MED2;
   objects[0].conf_overall = CONF3_HIGH;
   objects[0].low_rcs_dets_cnt = 3U;
   objects[0].f_ghost_NU_2_C = false;
   objects[0].f_overlapping_with_object = false;
   objects[0].otg_height = 1.5F;
   objects[0].ud_mov_historic_ndets = 4.0F;
   objects[0].ud_mov_cnt_underdrivable = 2U;
   objects[0].time_since_last_stop = 0.0F;
   objects[0].cntConsecutiveStopped = 0;
   objects[0].time_since_split = 0.0F;
   objects[0].bbox.Set_Length(10.0F);
   objects[0].bbox.Set_Width(2.0F);

   objects[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   objects[1].errcov[0][1] = 0.3F; 
   objects[1].errcov[0][2] = 0.3F;
   objects[1].errcov[0][4] = 0.3F;
   objects[1].errcov[0][5] = 0.3F;
   objects[1].errcov[1][2] = 0.3F;
   objects[1].errcov[1][3] = 0.3F;
   objects[1].errcov[1][5] = 0.3F;
   objects[1].errcov[2][3] = 0.3F;
   objects[1].errcov[2][4] = 0.3F;
   objects[1].errcov[3][4] = 0.3F;
   objects[1].errcov[3][5] = 0.3F;
   objects[1].errcov[4][5] = 0.3F;
   objects[1].orth_delta_filtered = 0.0F;
   objects[1].orth_gap_filtered = 0.0F;
   objects[1].filtered_pos_diff_heading = 0.0F;
   objects[1].time_since_initialization = 1.5F;
   objects[1].filtered_dets = 1.2F;
   objects[1].prev_avrg_conf_level = 0.6F;
   objects[1].length_uncertainty = 0.9F;
   objects[1].width_uncertainty = 0.9F;
   objects[1].mirror_prob = 0.0F;
   objects[1].average_rcs = 0.1F;
   objects[1].hdg_ptng_disagmt = 0.0F;
   objects[1].cca_pnt_filter_cov[0][0] = 0.2F;
   objects[1].cca_pnt_filter_cov[1][1] = 0.2F;
   objects[1].cca_pnt_filter_cov[0][1] = 0.2F;
   objects[1].filtered_hist_assoc_det_rr_err_mean = 0.1F;
   objects[1].filtered_hist_assoc_det_rr_err_var = 0.1F;
   objects[1].filtered_hist_assoc_n_dets = 1.1F;
   objects[1].cntConsecutiveAmbiguous = 1;
   objects[1].cntConsecutiveMoving = 0;
   objects[1].cntHostTurnForMirrorProb = 0;
   objects[1].total_reduced_dets = 1U;
   objects[1].id = 2;
   objects[1].num_updates_since_init = 20;
   objects[1].min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
   objects[1].behind_sep_id = 2;
   objects[1].on_sep_id = 1;
   objects[1].conf_longitudinal_position = CONF9_MED3;
   objects[1].conf_lateral_position = CONF9_MED2;
   objects[1].conf_speed = CONF9_MED2;
   objects[1].conf_overall = CONF3_HIGH;
   objects[1].low_rcs_dets_cnt = 3U;
   objects[1].f_ghost_NU_2_C = false;
   objects[1].f_overlapping_with_object = false;
   objects[1].otg_height = 0.5F;
   objects[1].ud_mov_historic_ndets = 0.0F;
   objects[1].ud_mov_cnt_underdrivable = 0U;
   objects[1].time_since_last_stop = 0.0F;
   objects[1].cntConsecutiveStopped = 0;
   objects[1].time_since_split = 0.0F;
   objects[1].bbox.Set_Length(8.0F);
   objects[1].bbox.Set_Width(1.5F);

   objects[2].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   objects[2].errcov[0][1] = 0.3F; 
   objects[2].errcov[0][2] = 0.3F;
   objects[2].errcov[0][4] = 0.3F;
   objects[2].errcov[0][5] = 0.3F;
   objects[2].errcov[1][2] = 0.3F;
   objects[2].errcov[1][3] = 0.3F;
   objects[2].errcov[1][5] = 0.3F;
   objects[2].errcov[2][3] = 0.3F;
   objects[2].errcov[2][4] = 0.3F;
   objects[2].errcov[3][4] = 0.3F;
   objects[2].errcov[3][5] = 0.3F;
   objects[2].errcov[4][5] = 0.3F;
   objects[2].orth_delta_filtered = 0.0F;
   objects[2].orth_gap_filtered = 0.0F;
   objects[2].filtered_pos_diff_heading = 0.0F;
   objects[2].time_since_initialization = 1.5F;
   objects[2].filtered_dets = 1.2F;
   objects[2].prev_avrg_conf_level = 0.6F;
   objects[2].length_uncertainty = 0.9F;
   objects[2].width_uncertainty = 0.9F;
   objects[2].mirror_prob = 0.0F;
   objects[2].average_rcs = 0.1F;
   objects[2].hdg_ptng_disagmt = 0.0F;
   objects[2].cca_pnt_filter_cov[0][0] = 0.2F;
   objects[2].cca_pnt_filter_cov[1][1] = 0.2F;
   objects[2].cca_pnt_filter_cov[0][1] = 0.2F;
   objects[2].filtered_hist_assoc_det_rr_err_mean = 0.1F;
   objects[2].filtered_hist_assoc_det_rr_err_var = 0.1F;
   objects[2].filtered_hist_assoc_n_dets = 1.1F;
   objects[2].cntConsecutiveAmbiguous = 1;
   objects[2].cntConsecutiveMoving = 0;
   objects[2].cntHostTurnForMirrorProb = 0;
   objects[2].total_reduced_dets = 1U;
   objects[2].id = 3;
   objects[2].num_updates_since_init = 20;
   objects[2].min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
   objects[2].behind_sep_id = 2;
   objects[2].on_sep_id = 1;
   objects[2].conf_longitudinal_position = CONF9_MED3;
   objects[2].conf_lateral_position = CONF9_MED2;
   objects[2].conf_speed = CONF9_MED2;
   objects[2].conf_overall = CONF3_HIGH;
   objects[2].low_rcs_dets_cnt = 3U;
   objects[2].f_ghost_NU_2_C = true;
   objects[2].f_overlapping_with_object = true;
   objects[2].otg_height = 0.5F;
   objects[2].ud_mov_historic_ndets = 0.0F;
   objects[2].ud_mov_cnt_underdrivable = 0U;
   objects[2].time_since_last_stop = 0.0F;
   objects[2].cntConsecutiveStopped = 0;
   objects[2].time_since_split = 0.0F;
   objects[2].bbox.Set_Length(6.0F);
   objects[2].bbox.Set_Width(1.0F);
   /** \action
    * call Populate_Internal_Objects_Log_Data().
    */
   Populate_Internal_Objects_Log_Data(internal_objects_log, objects, num_active_objects, active_obj_ids);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[0], F360_EPSILON,  "Expected internal object log other state covariance[0][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[1], F360_EPSILON,  "Expected internal object log other state covariance[0][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[2], F360_EPSILON,  "Expected internal object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[3], F360_EPSILON,  "Expected internal object log other state covariance[0][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[4], F360_EPSILON,  "Expected internal object log other state covariance[1][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[5], F360_EPSILON,  "Expected internal object log other state covariance[1][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[6], F360_EPSILON,  "Expected internal object log other state covariance[1][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[7], F360_EPSILON,  "Expected internal object log other state covariance[1][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[8], F360_EPSILON,  "Expected internal object log other state covariance[2][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[9], F360_EPSILON,  "Expected internal object log other state covariance[2][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[10], F360_EPSILON,  "Expected internal object log other state covariance[3][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[0].other_state_covariance[11], F360_EPSILON,  "Expected internal object log other state covariance[4][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[0].orth_delta_filtered, F360_EPSILON,  "Expected internal object log orth_delta_filtered is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.05F, internal_objects_log[0].orth_gap_filtered, F360_EPSILON,  "Expected internal object log orth_gap_filtered is not 0.05F");
   DOUBLES_EQUAL_TEXT(0.02F, internal_objects_log[0].filtered_pos_diff_heading, F360_EPSILON,  "Expected internal object log filtered_pos_diff_heading is not 0.02F");
   DOUBLES_EQUAL_TEXT(2.5F, internal_objects_log[0].time_since_initialization, F360_EPSILON,  "Expected internal object log time_since_initialization is not 2.5F");
   DOUBLES_EQUAL_TEXT(5.2F, internal_objects_log[0].filtered_dets, F360_EPSILON,  "Expected internal object log filtered_dets is not 5.2F");
   DOUBLES_EQUAL_TEXT(0.7F, internal_objects_log[0].prev_avrg_conf_level, F360_EPSILON,  "Expected internal object log prev_avrg_conf_level is not 0.7F");
   DOUBLES_EQUAL_TEXT(0.4F, internal_objects_log[0].length_uncertainty, F360_EPSILON,  "Expected internal object log length_uncertainty is not 0.4F");
   DOUBLES_EQUAL_TEXT(0.4F, internal_objects_log[0].width_uncertainty, F360_EPSILON,  "Expected internal object log width_uncertainty is not 0.4F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[0].mirror_prob, F360_EPSILON,  "Expected internal object log mirror_prob is not 0.1F");
   DOUBLES_EQUAL_TEXT(2.1F, internal_objects_log[0].average_rcs, F360_EPSILON,  "Expected internal object log average_rcs is not 2.1F");
   DOUBLES_EQUAL_TEXT(0.02F, internal_objects_log[0].hdg_ptng_disagmt, F360_EPSILON,  "Expected internal object log hdg_ptng_disagmt is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[0].cca_pnt_filter_cov[0], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[0].cca_pnt_filter_cov[1], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[0].cca_pnt_filter_cov[2], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[0].filtered_hist_assoc_det_rr_err_mean, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_det_rr_err_mean is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[0].filtered_hist_assoc_det_rr_err_var, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_det_rr_err_var is not 0.1F");
   DOUBLES_EQUAL_TEXT(4.1F, internal_objects_log[0].filtered_hist_assoc_n_dets, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_n_dets is not 4.1F");
   DOUBLES_EQUAL_TEXT(1.5F, internal_objects_log[0].ud_mov_historic_height_mean, F360_EPSILON, "Expected internal object log ud_mov_historic_height_mean is not 1.5F");
   DOUBLES_EQUAL_TEXT(4.0F, internal_objects_log[0].ud_mov_historic_ndets, F360_EPSILON, "Expected internal object log ud_mov_historic_ndets is not 4.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[0].time_since_last_stop, F360_EPSILON, "Expected internal object log time_since_last_stop is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[0].time_since_split, F360_EPSILON, "Expected internal object log time_since_split is not 0.0");
   DOUBLES_EQUAL_TEXT(10.0F, internal_objects_log[0].bbox_length, F360_EPSILON, "Expected internal object log bbox_length is not 10.0F");
   DOUBLES_EQUAL_TEXT(2.0F, internal_objects_log[0].bbox_width, F360_EPSILON, "Expected internal object log bbox_width is no 2.0F");
   CHECK_EQUAL_TEXT(1, internal_objects_log[0].cntConsecutiveAmbiguous,  "Expected internal object log cntConsecutiveAmbiguous is not 1");
   CHECK_EQUAL_TEXT(50, internal_objects_log[0].cntConsecutiveMoving,  "Expected internal object log cntConsecutiveMoving is not 50");
   CHECK_EQUAL_TEXT(2, internal_objects_log[0].cntHostTurnForMirrorProb,  "Expected internal object log cntHostTurnForMirrorProb is not 2");
   CHECK_EQUAL_TEXT(4, internal_objects_log[0].total_reduced_dets,  "Expected internal object log total_reduced_dets is not 4");
   CHECK_EQUAL_TEXT(1, internal_objects_log[0].id,  "Expected internal object log id is not 1");
   CHECK_EQUAL_TEXT(40, internal_objects_log[0].num_updates_since_init,  "Expected internal object log num_updates_since_init is not 40F");
   CHECK_EQUAL_TEXT(6, internal_objects_log[0].min_projection_reference_point,  "Expected internal object log min_projection_reference_point is not 6");
   CHECK_EQUAL_TEXT(2, internal_objects_log[0].behind_sep_id,  "Expected internal object log behind_sep_id is not 2");
   CHECK_EQUAL_TEXT(0, internal_objects_log[0].on_sep_id,  "Expected internal object log on_sep_id is not 0");
   CHECK_EQUAL_TEXT(7, internal_objects_log[0].conf_longitudinal_position,  "Expected internal object log conf_longitudinal_position is not 7");
   CHECK_EQUAL_TEXT(6, internal_objects_log[0].conf_lateral_position,  "Expected internal object log conf_lateral_position is not 6");
   CHECK_EQUAL_TEXT(6, internal_objects_log[0].conf_speed,  "Expected internal object log conf_speed is not 6");
   CHECK_EQUAL_TEXT(3, internal_objects_log[0].conf_overall,  "Expected internal object log conf_overall is not 3");
   CHECK_EQUAL_TEXT(3, internal_objects_log[0].low_rcs_dets_cnt,  "Expected internal object log low_rcs_dets_cnt is not 3");
   CHECK_EQUAL_TEXT(0, internal_objects_log[0].f_ghost_NU_2_C,  "Expected internal object log f_ghost_NU_2_C is not 0");
   CHECK_EQUAL_TEXT(0, internal_objects_log[0].f_overlapping_with_object,  "Expected internal object log f_overlapping_with_object is not 0");
   CHECK_EQUAL_TEXT(2, internal_objects_log[0].ud_mov_cnt_underdrivable,  "Expected internal object log ud_mov_cnt_underdrivable is not 2");
   CHECK_EQUAL_TEXT(0, internal_objects_log[0].cntConsecutiveStopped,  "Expected internal object log cntConsecutiveStopped is not 0");

  
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[0], F360_EPSILON,  "Expected internal object log other state covariance[0][2] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[1], F360_EPSILON,  "Expected internal object log other state covariance[0][3] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[2], F360_EPSILON,  "Expected internal object log other state covariance[0][4] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[3], F360_EPSILON,  "Expected internal object log other state covariance[0][5] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[4], F360_EPSILON,  "Expected internal object log other state covariance[1][2] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[5], F360_EPSILON,  "Expected internal object log other state covariance[1][3] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[6], F360_EPSILON,  "Expected internal object log other state covariance[1][4] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[7], F360_EPSILON,  "Expected internal object log other state covariance[1][5] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[8], F360_EPSILON,  "Expected internal object log other state covariance[2][3] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[9], F360_EPSILON,  "Expected internal object log other state covariance[2][5] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[10], F360_EPSILON,  "Expected internal object log other state covariance[3][4] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[1].other_state_covariance[11], F360_EPSILON,  "Expected internal object log other state covariance[4][5] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[1].orth_delta_filtered, F360_EPSILON,  "Expected internal object log orth_delta_filtered is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[1].orth_gap_filtered, F360_EPSILON,  "Expected internal object log orth_gap_filtered is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[1].filtered_pos_diff_heading, F360_EPSILON,  "Expected internal object log filtered_pos_diff_heading is not 0.0F");
   DOUBLES_EQUAL_TEXT(1.5F, internal_objects_log[1].time_since_initialization, F360_EPSILON,  "Expected internal object log time_since_initialization is not 1.5F");
   DOUBLES_EQUAL_TEXT(1.2F, internal_objects_log[1].filtered_dets, F360_EPSILON,  "Expected internal object log filtered_dets is not 1.2F");
   DOUBLES_EQUAL_TEXT(0.6F, internal_objects_log[1].prev_avrg_conf_level, F360_EPSILON,  "Expected internal object log prev_avrg_conf_level is not 0.6F");
   DOUBLES_EQUAL_TEXT(0.9F, internal_objects_log[1].length_uncertainty, F360_EPSILON,  "Expected internal object log length_uncertainty is not 0.9F");
   DOUBLES_EQUAL_TEXT(0.9F, internal_objects_log[1].width_uncertainty, F360_EPSILON,  "Expected internal object log width_uncertainty is not 0.9F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[1].mirror_prob, F360_EPSILON,  "Expected internal object log mirror_prob is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[1].average_rcs, F360_EPSILON,  "Expected internal object log average_rcs is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[1].hdg_ptng_disagmt, F360_EPSILON,  "Expected internal object log hdg_ptng_disagmt is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[1].cca_pnt_filter_cov[0], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[1].cca_pnt_filter_cov[1], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[1].cca_pnt_filter_cov[2], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[1].filtered_hist_assoc_det_rr_err_mean, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_det_rr_err_mean is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[1].filtered_hist_assoc_det_rr_err_var, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_det_rr_err_var is not 0.1F");
   DOUBLES_EQUAL_TEXT(1.1F, internal_objects_log[1].filtered_hist_assoc_n_dets, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_n_dets is not 1.1F");
   DOUBLES_EQUAL_TEXT(0.5F, internal_objects_log[1].ud_mov_historic_height_mean, F360_EPSILON, "Expected internal object log ud_mov_historic_height_mean is not 0.5F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[1].ud_mov_historic_ndets, F360_EPSILON, "Expected internal object log ud_mov_historic_ndets is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[1].time_since_last_stop, F360_EPSILON, "Expected internal object log time_since_last_stop is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[1].time_since_split, F360_EPSILON, "Expected internal object log time_since_split is not 0.0F");
   DOUBLES_EQUAL_TEXT(8.0F, internal_objects_log[1].bbox_length, F360_EPSILON, "Expected internal object log bbox_length is not 8.0F");
   DOUBLES_EQUAL_TEXT(1.5F, internal_objects_log[1].bbox_width, F360_EPSILON, "Expected internal object log bbox_width is no 1.5F");
   CHECK_EQUAL_TEXT(1, internal_objects_log[1].cntConsecutiveAmbiguous,  "Expected internal object log cntConsecutiveAmbiguous is not 1");
   CHECK_EQUAL_TEXT(0, internal_objects_log[1].cntConsecutiveMoving,  "Expected internal object log cntConsecutiveMoving is not 0");
   CHECK_EQUAL_TEXT(0, internal_objects_log[1].cntHostTurnForMirrorProb,  "Expected internal object log cntHostTurnForMirrorProb is not 0");
   CHECK_EQUAL_TEXT(1, internal_objects_log[1].total_reduced_dets,  "Expected internal object log total_reduced_dets is not 1");
   CHECK_EQUAL_TEXT(2, internal_objects_log[1].id,  "Expected internal object log id is not 2");
   CHECK_EQUAL_TEXT(20, internal_objects_log[1].num_updates_since_init,  "Expected internal object log num_updates_since_init is not 20F");
   CHECK_EQUAL_TEXT(0, internal_objects_log[1].min_projection_reference_point,  "Expected internal object log min_projection_reference_point is not 0");
   CHECK_EQUAL_TEXT(2, internal_objects_log[1].behind_sep_id,  "Expected internal object log behind_sep_id is not 2");
   CHECK_EQUAL_TEXT(1, internal_objects_log[1].on_sep_id,  "Expected internal object log on_sep_id is not 1");
   CHECK_EQUAL_TEXT(7, internal_objects_log[1].conf_longitudinal_position,  "Expected internal object log conf_longitudinal_position is not 7");
   CHECK_EQUAL_TEXT(6, internal_objects_log[1].conf_lateral_position,  "Expected internal object log conf_lateral_position is not 6");
   CHECK_EQUAL_TEXT(6, internal_objects_log[1].conf_speed,  "Expected internal object log conf_speed is not 6");
   CHECK_EQUAL_TEXT(3, internal_objects_log[1].conf_overall,  "Expected internal object log conf_overall is not 3");
   CHECK_EQUAL_TEXT(3, internal_objects_log[1].low_rcs_dets_cnt,  "Expected internal object log low_rcs_dets_cnt is not 3");
   CHECK_EQUAL_TEXT(0, internal_objects_log[1].f_ghost_NU_2_C,  "Expected internal object log f_ghost_NU_2_C is not 0");
   CHECK_EQUAL_TEXT(0, internal_objects_log[1].f_overlapping_with_object,  "Expected internal object log f_overlapping_with_object is not 0");
   CHECK_EQUAL_TEXT(0, internal_objects_log[1].ud_mov_cnt_underdrivable,  "Expected internal object log ud_mov_cnt_underdrivable is not 0");
   CHECK_EQUAL_TEXT(0, internal_objects_log[1].cntConsecutiveStopped,  "Expected internal object log cntConsecutiveStopped is not 0");
   
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[0], F360_EPSILON,  "Expected internal object log other state covariance[0][2] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[1], F360_EPSILON,  "Expected internal object log other state covariance[0][3] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[2], F360_EPSILON,  "Expected internal object log other state covariance[0][4] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[3], F360_EPSILON,  "Expected internal object log other state covariance[0][5] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[4], F360_EPSILON,  "Expected internal object log other state covariance[1][2] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[5], F360_EPSILON,  "Expected internal object log other state covariance[1][3] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[6], F360_EPSILON,  "Expected internal object log other state covariance[1][4] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[7], F360_EPSILON,  "Expected internal object log other state covariance[1][5] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[8], F360_EPSILON,  "Expected internal object log other state covariance[2][3] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[9], F360_EPSILON,  "Expected internal object log other state covariance[2][5] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[10], F360_EPSILON,  "Expected internal object log other state covariance[3][4] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.3F, internal_objects_log[2].other_state_covariance[11], F360_EPSILON,  "Expected internal object log other state covariance[4][5] is not 0.3F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[2].orth_delta_filtered, F360_EPSILON,  "Expected internal object log orth_delta_filtered is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[2].orth_gap_filtered, F360_EPSILON,  "Expected internal object log orth_gap_filtered is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[2].filtered_pos_diff_heading, F360_EPSILON,  "Expected internal object log filtered_pos_diff_heading is not 0.0F");
   DOUBLES_EQUAL_TEXT(1.5F, internal_objects_log[2].time_since_initialization, F360_EPSILON,  "Expected internal object log time_since_initialization is not 1.5F");
   DOUBLES_EQUAL_TEXT(1.2F, internal_objects_log[2].filtered_dets, F360_EPSILON,  "Expected internal object log filtered_dets is not 1.2F");
   DOUBLES_EQUAL_TEXT(0.6F, internal_objects_log[2].prev_avrg_conf_level, F360_EPSILON,  "Expected internal object log prev_avrg_conf_level is not 0.6F");
   DOUBLES_EQUAL_TEXT(0.9F, internal_objects_log[2].length_uncertainty, F360_EPSILON,  "Expected internal object log length_uncertainty is not 0.9F");
   DOUBLES_EQUAL_TEXT(0.9F, internal_objects_log[2].width_uncertainty, F360_EPSILON,  "Expected internal object log width_uncertainty is not 0.9F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[2].mirror_prob, F360_EPSILON,  "Expected internal object log mirror_prob is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[2].average_rcs, F360_EPSILON,  "Expected internal object log average_rcs is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[2].hdg_ptng_disagmt, F360_EPSILON,  "Expected internal object log hdg_ptng_disagmt is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[2].cca_pnt_filter_cov[0], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[2].cca_pnt_filter_cov[1], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, internal_objects_log[2].cca_pnt_filter_cov[2], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[2].filtered_hist_assoc_det_rr_err_mean, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_det_rr_err_mean is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, internal_objects_log[2].filtered_hist_assoc_det_rr_err_var, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_det_rr_err_var is not 0.1F");
   DOUBLES_EQUAL_TEXT(1.1F, internal_objects_log[2].filtered_hist_assoc_n_dets, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_n_dets is not 1.1F");
   DOUBLES_EQUAL_TEXT(0.5F, internal_objects_log[2].ud_mov_historic_height_mean, F360_EPSILON, "Expected internal object log ud_mov_historic_height_mean is not 0.5F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[2].ud_mov_historic_ndets, F360_EPSILON, "Expected internal object log ud_mov_historic_ndets is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[2].time_since_last_stop, F360_EPSILON, "Expected internal object log time_since_last_stop is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, internal_objects_log[2].time_since_split, F360_EPSILON, "Expected internal object log time_since_split is not 0.0F");
   DOUBLES_EQUAL_TEXT(6.0F, internal_objects_log[2].bbox_length, F360_EPSILON, "Expected internal object log bbox_length is not 6.0F");
   DOUBLES_EQUAL_TEXT(1.0F, internal_objects_log[2].bbox_width, F360_EPSILON, "Expected internal object log bbox_width is no 1.0F");
   CHECK_EQUAL_TEXT(1, internal_objects_log[2].cntConsecutiveAmbiguous,  "Expected internal object log cntConsecutiveAmbiguous is not 1");
   CHECK_EQUAL_TEXT(0, internal_objects_log[2].cntConsecutiveMoving,  "Expected internal object log cntConsecutiveMoving is not 0");
   CHECK_EQUAL_TEXT(0, internal_objects_log[2].cntHostTurnForMirrorProb,  "Expected internal object log cntHostTurnForMirrorProb is not 0");
   CHECK_EQUAL_TEXT(1, internal_objects_log[2].total_reduced_dets,  "Expected internal object log total_reduced_dets is not 1");
   CHECK_EQUAL_TEXT(3, internal_objects_log[2].id,  "Expected internal object log id is not 2");
   CHECK_EQUAL_TEXT(20, internal_objects_log[2].num_updates_since_init,  "Expected internal object log num_updates_since_init is not 20F");
   CHECK_EQUAL_TEXT(0, internal_objects_log[2].min_projection_reference_point,  "Expected internal object log min_projection_reference_point is not 0");
   CHECK_EQUAL_TEXT(2, internal_objects_log[2].behind_sep_id,  "Expected internal object log behind_sep_id is not 2");
   CHECK_EQUAL_TEXT(1, internal_objects_log[2].on_sep_id,  "Expected internal object log on_sep_id is not 1");
   CHECK_EQUAL_TEXT(7, internal_objects_log[2].conf_longitudinal_position,  "Expected internal object log conf_longitudinal_position is not 7");
   CHECK_EQUAL_TEXT(6, internal_objects_log[2].conf_lateral_position,  "Expected internal object log conf_lateral_position is not 6");
   CHECK_EQUAL_TEXT(6, internal_objects_log[2].conf_speed,  "Expected internal object log conf_speed is not 6");
   CHECK_EQUAL_TEXT(3, internal_objects_log[2].conf_overall,  "Expected internal object log conf_overall is not 3");
   CHECK_EQUAL_TEXT(3, internal_objects_log[2].low_rcs_dets_cnt,  "Expected internal object log low_rcs_dets_cnt is not 3");
   CHECK_EQUAL_TEXT(1, internal_objects_log[2].f_ghost_NU_2_C,  "Expected internal object log f_ghost_NU_2_C is not 1");
   CHECK_EQUAL_TEXT(1, internal_objects_log[2].f_overlapping_with_object,  "Expected internal object log f_overlapping_with_object is not 1");
   CHECK_EQUAL_TEXT(0, internal_objects_log[2].ud_mov_cnt_underdrivable,  "Expected internal object log ud_mov_cnt_underdrivable is not 0");
   CHECK_EQUAL_TEXT(0, internal_objects_log[2].cntConsecutiveStopped,  "Expected internal object log cntConsecutiveStopped is not 0");
}

/** \purpose  
 * Verify Populate_Internal_Objects_Data returns the data as expected
 * \req NA
 */
TEST(f360_populate_internal_objects_log, Test_Populate_Internal_Objects_Data)
{
   /** \precond
    * Initialize the varibles values for Populate_Internal_Objects_Data()
    */
   F360_Calibrations_T calibrations;
   calibrations.init_cca_pnt_filter_cov[0][0] = 0.6F;
   calibrations.init_cca_pnt_filter_cov[0][1] = 0.0F;
   calibrations.init_cca_pnt_filter_cov[1][0] = 0.0F;
   calibrations.init_cca_pnt_filter_cov[1][1] = 0.01F;
   objects[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   internal_objects_log[0].other_state_covariance[0] = 0.2F;
   internal_objects_log[0].other_state_covariance[1] = 0.2F;
   internal_objects_log[0].other_state_covariance[2] = 0.2F;
   internal_objects_log[0].other_state_covariance[3] = 0.2F;
   internal_objects_log[0].other_state_covariance[4] = 0.2F;
   internal_objects_log[0].other_state_covariance[5] = 0.2F;
   internal_objects_log[0].other_state_covariance[6] = 0.2F;
   internal_objects_log[0].other_state_covariance[7] = 0.2F;
   internal_objects_log[0].other_state_covariance[8] = 0.2F;
   internal_objects_log[0].other_state_covariance[9] = 0.2F;
   internal_objects_log[0].other_state_covariance[10] = 0.2F;
   internal_objects_log[0].other_state_covariance[11] = 0.2F;
   internal_objects_log[0].orth_delta_filtered = 0.1F;
   internal_objects_log[0].orth_gap_filtered = 0.05F;
   internal_objects_log[0].filtered_pos_diff_heading = 0.02F;
   internal_objects_log[0].time_since_initialization = 2.5F;
   internal_objects_log[0].filtered_dets = 5.2F;
   internal_objects_log[0].prev_avrg_conf_level = 0.7F;
   internal_objects_log[0].length_uncertainty = 0.4F;
   internal_objects_log[0].width_uncertainty = 0.4F;
   internal_objects_log[0].mirror_prob = 0.1F;
   internal_objects_log[0].average_rcs = 2.1F;
   internal_objects_log[0].hdg_ptng_disagmt = 0.02F;
   internal_objects_log[0].cca_pnt_filter_cov[0] = 0.1F;
   internal_objects_log[0].cca_pnt_filter_cov[1] = 0.1F;
   internal_objects_log[0].cca_pnt_filter_cov[2] = 0.1F;
   internal_objects_log[0].filtered_hist_assoc_det_rr_err_mean = 0.1F;
   internal_objects_log[0].filtered_hist_assoc_det_rr_err_var = 0.1F;
   internal_objects_log[0].filtered_hist_assoc_n_dets = 4.1F;
   internal_objects_log[0].ud_mov_historic_height_mean = 1.5F;
   internal_objects_log[0].ud_mov_historic_ndets = 4.0F;
   internal_objects_log[0].time_since_last_stop = 0.0F;
   internal_objects_log[0].time_since_split = 0.0F;
   internal_objects_log[0].cntConsecutiveAmbiguous = 1;
   internal_objects_log[0].cntConsecutiveMoving = 50;
   internal_objects_log[0].cntHostTurnForMirrorProb = 2;
   internal_objects_log[0].total_reduced_dets = 4;
   internal_objects_log[0].id = 1;
   internal_objects_log[0].num_updates_since_init = 40;
   internal_objects_log[0].min_projection_reference_point = 6;
   internal_objects_log[0].behind_sep_id = 2;
   internal_objects_log[0].on_sep_id = 0;
   internal_objects_log[0].conf_longitudinal_position = 7;
   internal_objects_log[0].conf_lateral_position = 6;
   internal_objects_log[0].conf_speed = 6;
   internal_objects_log[0].conf_overall = 3;
   internal_objects_log[0].low_rcs_dets_cnt = 3;
   internal_objects_log[0].f_ghost_NU_2_C = 0;
   internal_objects_log[0].f_overlapping_with_object = 0;
   internal_objects_log[0].ud_mov_cnt_underdrivable = 2;
   internal_objects_log[0].cntConsecutiveStopped = 0;
   internal_objects_log[0].bbox_length = 9.0F;
   internal_objects_log[0].bbox_width = 2.0F;

   internal_objects_log[1].time_since_initialization = -1.0F;
   internal_objects_log[1].id = 2;
   internal_objects_log[1].orth_delta_filtered = 0.1F;
   internal_objects_log[2].time_since_initialization = 1.0F;
   internal_objects_log[2].id = 0;
   internal_objects_log[2].orth_delta_filtered = 0.1F;
   /** \action
    * call Populate_Internal_Objects_Data().
    */
   Populate_Internal_Objects_Data(objects, calibrations, internal_objects_log);

   /** \result
    * check that the output match expected data.
    */
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][2], F360_EPSILON,  "Expected internal object log other state covariance[0][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[2][0], F360_EPSILON,  "Expected internal object log other state covariance[0][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][3], F360_EPSILON,  "Expected internal object log other state covariance[0][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][0], F360_EPSILON,  "Expected internal object log other state covariance[0][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][4], F360_EPSILON,  "Expected internal object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][0], F360_EPSILON,  "Expected internal object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][5], F360_EPSILON,  "Expected internal object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[5][0], F360_EPSILON,  "Expected internal object log other state covariance[0][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][2], F360_EPSILON,  "Expected internal object log other state covariance[1][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[2][1], F360_EPSILON,  "Expected internal object log other state covariance[1][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][3], F360_EPSILON,  "Expected internal object log other state covariance[1][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][1], F360_EPSILON,  "Expected internal object log other state covariance[1][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][4], F360_EPSILON,  "Expected internal object log other state covariance[2][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][1], F360_EPSILON,  "Expected internal object log other state covariance[2][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][5], F360_EPSILON,  "Expected internal object log other state covariance[3][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[5][1], F360_EPSILON,  "Expected internal object log other state covariance[4][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[2][3], F360_EPSILON,  "Expected internal object log other state covariance[1][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][2], F360_EPSILON,  "Expected internal object log other state covariance[1][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[2][5], F360_EPSILON,  "Expected internal object log other state covariance[1][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[5][2], F360_EPSILON,  "Expected internal object log other state covariance[2][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][4], F360_EPSILON,  "Expected internal object log other state covariance[2][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][3], F360_EPSILON,  "Expected internal object log other state covariance[3][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][5], F360_EPSILON,  "Expected internal object log other state covariance[4][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[5][4], F360_EPSILON,  "Expected internal object log other state covariance[4][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[0].orth_delta_filtered, F360_EPSILON,  "Expected internal object log orth_delta_filtered is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.05F, objects[0].orth_gap_filtered, F360_EPSILON,  "Expected internal object log orth_gap_filtered is not 0.05F");
   DOUBLES_EQUAL_TEXT(0.02F, objects[0].filtered_pos_diff_heading, F360_EPSILON,  "Expected internal object log filtered_pos_diff_heading is not 0.02F");
   DOUBLES_EQUAL_TEXT(2.5F, objects[0].time_since_initialization, F360_EPSILON,  "Expected internal object log time_since_initialization is not 2.5F");
   DOUBLES_EQUAL_TEXT(5.2F, objects[0].filtered_dets, F360_EPSILON,  "Expected internal object log filtered_dets is not 5.2F");
   DOUBLES_EQUAL_TEXT(0.7F, objects[0].prev_avrg_conf_level, F360_EPSILON,  "Expected internal object log prev_avrg_conf_level is not 0.7F");
   DOUBLES_EQUAL_TEXT(0.4F, objects[0].length_uncertainty, F360_EPSILON,  "Expected internal object log length_uncertainty is not 0.4F");
   DOUBLES_EQUAL_TEXT(0.4F, objects[0].width_uncertainty, F360_EPSILON,  "Expected internal object log width_uncertainty is not 0.4F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[0].mirror_prob, F360_EPSILON,  "Expected internal object log mirror_prob is not 0.1F");
   DOUBLES_EQUAL_TEXT(2.1F, objects[0].average_rcs, F360_EPSILON,  "Expected internal object log average_rcs is not 2.1F");
   DOUBLES_EQUAL_TEXT(0.02F, objects[0].hdg_ptng_disagmt, F360_EPSILON,  "Expected internal object log hdg_ptng_disagmt is not 0.02F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[0].cca_pnt_filter_cov[0][0], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[0].cca_pnt_filter_cov[1][1], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[0].cca_pnt_filter_cov[0][1], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[0].cca_pnt_filter_cov[1][0], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[0].filtered_hist_assoc_det_rr_err_mean, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_det_rr_err_mean is not 0.1F");
   DOUBLES_EQUAL_TEXT(0.1F, objects[0].filtered_hist_assoc_det_rr_err_var, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_det_rr_err_var is not 0.1F");
   DOUBLES_EQUAL_TEXT(4.1F, objects[0].filtered_hist_assoc_n_dets, F360_EPSILON,  "Expected internal object log filtered_hist_assoc_n_dets is not 4.1F");
   DOUBLES_EQUAL_TEXT(1.5F, objects[0].otg_height, F360_EPSILON, "Expected internal object log ud_mov_historic_height_mean is not 1.5F");
   DOUBLES_EQUAL_TEXT(4.0F, objects[0].ud_mov_historic_ndets, F360_EPSILON, "Expected internal object log ud_mov_historic_ndets is not 4.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].time_since_last_stop, F360_EPSILON, "Expected internal object log time_since_last_stop is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].time_since_split, F360_EPSILON, "Expected internal object log time_since_split is not 0.0");
   DOUBLES_EQUAL_TEXT(9.0F, objects[0].bbox.Get_Length(), F360_EPSILON, "Expected object bbox length is not 9.0");
   DOUBLES_EQUAL_TEXT(2.0F, objects[0].bbox.Get_Width(), F360_EPSILON, "Expected object bbox width is not 2.0");
   CHECK_EQUAL_TEXT(1, objects[0].cntConsecutiveAmbiguous,  "Expected internal object log cntConsecutiveAmbiguous is not 1");
   CHECK_EQUAL_TEXT(50, objects[0].cntConsecutiveMoving,  "Expected internal object log cntConsecutiveMoving is not 50");
   CHECK_EQUAL_TEXT(2, objects[0].cntHostTurnForMirrorProb,  "Expected internal object log cntHostTurnForMirrorProb is not 2");
   CHECK_EQUAL_TEXT(4, objects[0].total_reduced_dets,  "Expected internal object log total_reduced_dets is not 4");
   CHECK_EQUAL_TEXT(1, objects[0].id,  "Expected internal object log id is not 1");
   CHECK_EQUAL_TEXT(40, objects[0].num_updates_since_init,  "Expected internal object log num_updates_since_init is not 40F");
   CHECK_EQUAL_TEXT(6, objects[0].min_projection_reference_point,  "Expected internal object log min_projection_reference_point is not 6");
   CHECK_EQUAL_TEXT(2, objects[0].behind_sep_id,  "Expected internal object log behind_sep_id is not 2");
   CHECK_EQUAL_TEXT(0, objects[0].on_sep_id,  "Expected internal object log on_sep_id is not 0");
   CHECK_EQUAL_TEXT(7, objects[0].conf_longitudinal_position,  "Expected internal object log conf_longitudinal_position is not 7");
   CHECK_EQUAL_TEXT(6, objects[0].conf_lateral_position,  "Expected internal object log conf_lateral_position is not 6");
   CHECK_EQUAL_TEXT(6, objects[0].conf_speed,  "Expected internal object log conf_speed is not 6");
   CHECK_EQUAL_TEXT(3, objects[0].conf_overall,  "Expected internal object log conf_overall is not 3");
   CHECK_EQUAL_TEXT(3, objects[0].low_rcs_dets_cnt,  "Expected internal object log low_rcs_dets_cnt is not 3");
   CHECK_EQUAL_TEXT(0, objects[0].f_ghost_NU_2_C,  "Expected internal object log f_ghost_NU_2_C is not 0");
   CHECK_EQUAL_TEXT(0, objects[0].f_overlapping_with_object,  "Expected internal object log f_overlapping_with_object is not 0");
   CHECK_EQUAL_TEXT(2, objects[0].ud_mov_cnt_underdrivable,  "Expected internal object log ud_mov_cnt_underdrivable is not 2");
   CHECK_EQUAL_TEXT(0, objects[0].cntConsecutiveStopped,  "Expected internal object log cntConsecutiveStopped is not 0");
   DOUBLES_EQUAL_TEXT(0.0F, objects[1].orth_delta_filtered, F360_EPSILON,  "Expected internal object log orth_delta_filtered is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[2].orth_delta_filtered, F360_EPSILON,  "Expected internal object log orth_delta_filtered is not 0.0F");
   
   /** \action
    * change the object input data call Populate_Internal_Objects_Data() again to test the branch.
    */
   objects[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   Populate_Internal_Objects_Data(objects, calibrations, internal_objects_log);
  
   /** \result
    * check that the output match expected data.
    */
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][1], F360_EPSILON,  "Expected object log other state covariance[0][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][0], F360_EPSILON,  "Expected object log other state covariance[0][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][2], F360_EPSILON,  "Expected object log other state covariance[0][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[2][0], F360_EPSILON,  "Expected object log other state covariance[0][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][4], F360_EPSILON,  "Expected object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][0], F360_EPSILON,  "Expected object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][5], F360_EPSILON,  "Expected object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[5][0], F360_EPSILON,  "Expected object log other state covariance[0][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][2], F360_EPSILON,  "Expected object log other state covariance[1][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[2][1], F360_EPSILON,  "Expected object log other state covariance[1][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][3], F360_EPSILON,  "Expected object log other state covariance[1][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][1], F360_EPSILON,  "Expected object log other state covariance[1][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][5], F360_EPSILON,  "Expected object log other state covariance[2][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[5][1], F360_EPSILON,  "Expected object log other state covariance[2][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[2][3], F360_EPSILON,  "Expected object log other state covariance[3][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][2], F360_EPSILON,  "Expected object log other state covariance[4][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[2][4], F360_EPSILON,  "Expected object log other state covariance[1][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][2], F360_EPSILON,  "Expected object log other state covariance[1][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][4], F360_EPSILON,  "Expected object log other state covariance[1][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][3], F360_EPSILON,  "Expected object log other state covariance[2][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][5], F360_EPSILON,  "Expected object log other state covariance[2][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[5][3], F360_EPSILON,  "Expected object log other state covariance[3][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][5], F360_EPSILON,  "Expected object log other state covariance[4][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[5][4], F360_EPSILON,  "Expected object log other state covariance[4][5] is not 0.2F");
   
   /** \action
    * change the object input data call Populate_Internal_Objects_Data() again to test the branch.
    */
   objects[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   Populate_Internal_Objects_Data(objects, calibrations, internal_objects_log);
   
   /** \result
    * check that the output match expected data.
    */
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][1], F360_EPSILON,  "Expected object log other state covariance[0][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][0], F360_EPSILON,  "Expected object log other state covariance[0][2] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[0][2], F360_EPSILON,  "Expected object log other state covariance[0][3] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[2][0], F360_EPSILON,  "Expected object log other state covariance[0][3] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[0][4], F360_EPSILON,  "Expected object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][0], F360_EPSILON,  "Expected object log other state covariance[0][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[0][5], F360_EPSILON,  "Expected object log other state covariance[0][4] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[5][0], F360_EPSILON,  "Expected object log other state covariance[0][5] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[1][2], F360_EPSILON,  "Expected object log other state covariance[1][2] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[2][1], F360_EPSILON,  "Expected object log other state covariance[1][3] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[1][3], F360_EPSILON,  "Expected object log other state covariance[1][4] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][1], F360_EPSILON,  "Expected object log other state covariance[1][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[1][5], F360_EPSILON,  "Expected object log other state covariance[2][3] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[5][1], F360_EPSILON,  "Expected object log other state covariance[2][5] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[2][3], F360_EPSILON,  "Expected object log other state covariance[3][4] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[3][2], F360_EPSILON,  "Expected object log other state covariance[4][5] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[2][4], F360_EPSILON,  "Expected object log other state covariance[1][3] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[4][2], F360_EPSILON,  "Expected object log other state covariance[1][4] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[3][4], F360_EPSILON,  "Expected object log other state covariance[1][5] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.2F, objects[0].errcov[4][3], F360_EPSILON,  "Expected object log other state covariance[2][3] is not 0.2F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[3][5], F360_EPSILON,  "Expected object log other state covariance[2][5] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[5][3], F360_EPSILON,  "Expected object log other state covariance[3][4] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[4][5], F360_EPSILON,  "Expected object log other state covariance[4][5] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].errcov[5][4], F360_EPSILON,  "Expected object log other state covariance[4][5] is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.6F, objects[0].cca_pnt_filter_cov[0][0], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.6F");
   DOUBLES_EQUAL_TEXT(0.01F, objects[0].cca_pnt_filter_cov[1][1], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].cca_pnt_filter_cov[0][1], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.0F");
   DOUBLES_EQUAL_TEXT(0.0F, objects[0].cca_pnt_filter_cov[1][0], F360_EPSILON,  "Expected internal object log cca_pnt_filter_cov is not 0.01F");
   CHECK_EQUAL_TEXT(3,objects[0].trk_fltr_type, "Expected object log track motion model is not 3");
}
/** @}*/
