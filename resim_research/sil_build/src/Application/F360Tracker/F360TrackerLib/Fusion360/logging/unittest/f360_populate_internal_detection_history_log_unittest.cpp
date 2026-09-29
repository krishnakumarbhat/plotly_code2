/** \file
 * This file contains unit tests for content of f360_populate_internal_detection_history_log.cpp file
 */

#include "f360_populate_internal_detection_history_log.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_populate_internal_detection_history_log
 *  @{
 */

/** \brief
 * Test suit for functions in f360_populate_internal_detection_history_log.cpp
 * Verify those functions return the value as expected.
 */
TEST_GROUP(f360_populate_internal_detection_history_log)
{
   // Declare common variables used within all tests in this test group.
   F360_Internal_Detection_Hist_T det_hist_data_log[MAX_NUMBER_OF_HISTORIC_DETECTIONS];
   F360_Detection_Hist_T det_hist;

   /** \setup
    * Set up the default det_hist data for all the cases in this test group
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      det_hist.f_idx_occupied[1] = true;
      det_hist.det_data[1].vcs_position_y = 2.0F;
      det_hist.det_data[1].vcs_position_x = 5.0F;
      det_hist.det_data[1].rdot = 6.5F;
      det_hist.det_data[1].rdot_comp = 5.0F;
      det_hist.det_data[1].vcs_az = 0.2F;
      det_hist.det_data[1].time_since_meas = 2.0F;
      det_hist.det_data[1].v_wrapping = 26.0F;
      det_hist.det_data[1].r_wrapping = 10.0F;
      det_hist.det_data[1].cluster_idx = 2U;
      det_hist.det_data[1].motion_status = 0;
      det_hist.det_data[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
      det_hist.det_data[1].f_dealiased = true;
      det_hist.det_data[1].f_FOV_edge = false;
      det_hist.det_data[1].f_selected = true;
      det_hist.det_data[1].f_is_range_in_all_looks = true;
      det_hist.det_data[1].f_potential_angle_jump = false;
   }

   /** \teardown
    * Describe what is done in test teardown. Remove test teardown function and this tag if it is not used.
    */
   TEST_TEARDOWN()
   {
      // Perform any necessary clean up. E.g. mock().clear().
   }

   // Define helper functions used in this test group here. E.g. Add_Detection_To_Detection_List().
};

/** \purpose
 * Verify Populate_Internal_Detection_History_Log_Data() returns the data as expected
 * \req NA
 */
TEST(f360_populate_internal_detection_history_log, Test_Populate_Internal_Detection_History_Log_Data)
{
   /** \precond
    * use test group default values
    */

   /** \action
    * call Populate_Internal_Detection_History_Log_Data().
    */
   Populate_Internal_Detection_History_Log_Data(det_hist_data_log, det_hist);
   /** \result
    * Describe expected output. E.g. check that the output match expected data.
    */
   DOUBLES_EQUAL_TEXT(2.0F, det_hist_data_log[0].vcs_lat_posn, F360_EPSILON, "Expected historic detection lateral position not 2.0F")
   DOUBLES_EQUAL_TEXT(5.0F, det_hist_data_log[0].vcs_long_posn, F360_EPSILON, "Expected historic detection longitudinal position is not 0.1F")
   DOUBLES_EQUAL_TEXT(6.5F, det_hist_data_log[0].rdot, F360_EPSILON, "Expected historic detection range rate is not 6.5F")
   DOUBLES_EQUAL_TEXT(5.0F, det_hist_data_log[0].rdot_comp, F360_EPSILON, "Expected historic detection  range rate compensated is not 5.0F")
   DOUBLES_EQUAL_TEXT(0.2F, det_hist_data_log[0].vcs_az, F360_EPSILON, "Expected historic detection vcs_az is not 0.2F")
   DOUBLES_EQUAL_TEXT(2.0F, det_hist_data_log[0].time_since_meas, F360_EPSILON, "Expected historic detectiontime_since_meas is not 2.0F")
   DOUBLES_EQUAL_TEXT(26.0F, det_hist_data_log[0].v_wrapping, F360_EPSILON, "Expected historic detection v_wrapping is not 26.0F")
   DOUBLES_EQUAL_TEXT(10.0F, det_hist_data_log[0].r_wrapping, F360_EPSILON, "Expected historic detection r_wrapping is not 10.0F")
   CHECK_EQUAL_TEXT(2U, det_hist_data_log[0].cluster_idx, "Expected historic detection cluster index is not 2U")
   CHECK_EQUAL_TEXT(1U, det_hist_data_log[0].occupied_idx, "Expected historic detection occupied index is not 0U")
   CHECK_EQUAL_TEXT(0U, det_hist_data_log[0].motion_status, "Expected historic detection motion status is not 0U")
   CHECK_EQUAL_TEXT(1U, det_hist_data_log[0].wheel_spin_type, "Expected historic detection wheel_spin_type is not 1U")
   CHECK_EQUAL_TEXT(1U, det_hist_data_log[0].f_dealiased, "Expected historic detection f_dealiased is not 1U")
   CHECK_EQUAL_TEXT(0U, det_hist_data_log[0].f_FOV_edge, "Expected historic detection f_FOV_edge is not 0U")
   CHECK_EQUAL_TEXT(1U, det_hist_data_log[0].f_selected, "Expected historic detection f_selected is not 1U")
   CHECK_EQUAL_TEXT(1U, det_hist_data_log[0].f_is_range_in_all_looks, "Expected historic detection f_is_range_in_all_looks is not 1U")
   CHECK_EQUAL_TEXT(0U, det_hist_data_log[0].f_potential_angle_jump, "Expected historic detection f_potential_angle_jump is not 0U")

   det_hist.det_data[1].f_dealiased = false;
   det_hist.det_data[1].f_FOV_edge = true;
   det_hist.det_data[1].f_selected = false;
   det_hist.det_data[1].f_is_range_in_all_looks = false;
   det_hist.det_data[1].f_potential_angle_jump = true;
   Populate_Internal_Detection_History_Log_Data(det_hist_data_log, det_hist);
   CHECK_EQUAL_TEXT(0U, det_hist_data_log[0].f_dealiased, "Expected historic detection f_dealiased is not 1U")
   CHECK_EQUAL_TEXT(1U, det_hist_data_log[0].f_FOV_edge, "Expected historic detection f_FOV_edge is not 0U")
   CHECK_EQUAL_TEXT(0U, det_hist_data_log[0].f_selected, "Expected historic detection f_selected is not 1U")
   CHECK_EQUAL_TEXT(0U, det_hist_data_log[0].f_is_range_in_all_looks, "Expected historic detection f_is_range_in_all_looks is not 1U")
   CHECK_EQUAL_TEXT(1U, det_hist_data_log[0].f_potential_angle_jump, "Expected historic detection f_potential_angle_jump is not 0U")

}

/** \purpose
 * Verify Link_Historical_Detections_To_Clusters does link the det hist to cluster information correctly
 * \req NA
 */
TEST(f360_populate_internal_detection_history_log, Test_Link_Historical_Detections_To_Clusters)
{
   /** \precond
    * Declare the variables that needed by Link_Historical_Detections_To_Clusters().
    */
   F360_Tracker_Info_T tracker_info{};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]{};

   /** \action
    * set up the input values then call Link_Historical_Detections_To_Clusters().
    */
   tracker_info.num_active_clusters = 1U;
   tracker_info.active_cluster_ids[0] = 3U;
   tracker_info.variant.num_hist_dets = 3U;
   det_hist.n_occupied = 3;
   clusters[2].id = 3;
   clusters[2].num_old_dets = 0;
   clusters[2].old_det_idx[0] = 0;
   Link_Historical_Detections_To_Clusters(tracker_info, det_hist, clusters);
   int16_t num_old_det_iter1 = clusters[2].num_old_dets;
   int16_t old_det_idx_iter1 = clusters[2].old_det_idx[0];
   int32_t num_of_occupied_iter1 = det_hist.n_occupied;
   bool f_idx_occupied_iter1 = det_hist.f_idx_occupied[1];


   tracker_info.variant.num_hist_dets = 1U;
   clusters[2].num_old_dets = 0;
   Link_Historical_Detections_To_Clusters(tracker_info, det_hist, clusters);
   int16_t num_old_det_iter2 = clusters[2].num_old_dets;
   int16_t old_det_idx_iter2 = clusters[2].old_det_idx[0];
   int32_t num_of_occupied_iter2 = det_hist.n_occupied;
   bool f_idx_occupied_iter2 = det_hist.f_idx_occupied[1];
   /** \result
    *  check that the output match expected data.
    */

   CHECK_EQUAL_TEXT(1, num_old_det_iter1, "Expected cluster linked number of old detection is not 1");
   CHECK_EQUAL_TEXT(1, old_det_idx_iter1, "Expected cluster linked det index is not 1");
   CHECK_EQUAL_TEXT(3, num_of_occupied_iter1, "Expected cluster linked old detection is not 3");
   CHECK_EQUAL_TEXT(true, f_idx_occupied_iter1, "Expected f_idx_occupied_iter1 is not true");
   CHECK_EQUAL_TEXT(0, num_old_det_iter2, "Expected cluster linked number of old detection is not 1");
   CHECK_EQUAL_TEXT(1, old_det_idx_iter2, "Expected cluster linked det index is not 1");
   CHECK_EQUAL_TEXT(2, num_of_occupied_iter2, "Expected cluster linked old detection is not 2");
   CHECK_EQUAL_TEXT(false, f_idx_occupied_iter2, "Expected f_idx_occupied_iter1 is not true");
}

/** \purpose
 * Verify Populate_Internal_Detection_History_Data returns the data as expected
 * \req NA
 */
TEST(f360_populate_internal_detection_history_log, Test_Populate_Internal_Detection_History_Data)
{
   /** \precond
    * Initialize the input variables
    */
   F360_Tracker_Info_T tracker_info{};
   tracker_info.variant.num_hist_dets = 1U;
   det_hist_data_log[0].vcs_lat_posn = 2.0F;
   det_hist_data_log[0].vcs_long_posn = 5.0F;
   det_hist_data_log[0].vcs_lat_posn = 2.0F;
   det_hist_data_log[0].vcs_long_posn = 5.0F;
   det_hist_data_log[0].rdot = 6.5F;
   det_hist_data_log[0].rdot_comp = 5.0F;
   det_hist_data_log[0].vcs_az = 0.2F;
   det_hist_data_log[0].time_since_meas = 2.0F;
   det_hist_data_log[0].v_wrapping = 26.0F;
   det_hist_data_log[0].r_wrapping = 10.0F;
   det_hist_data_log[0].cluster_idx = 2U;
   det_hist_data_log[0].occupied_idx = 1U;
   det_hist_data_log[0].motion_status = 0U;
   det_hist_data_log[0].wheel_spin_type = 1U;
   det_hist_data_log[0].f_dealiased = 1U;
   det_hist_data_log[0].f_FOV_edge = 0U;
   det_hist_data_log[0].f_selected = 1U;
   det_hist_data_log[0].f_is_range_in_all_looks = 1U;
   det_hist_data_log[0].f_potential_angle_jump = 0U;

   /** \action
    * call Populate_Internal_Detection_History_Data().
    */
   Populate_Internal_Detection_History_Data(det_hist, tracker_info, det_hist_data_log);

   /** \result
    * check that the output match expected data.
    */
   DOUBLES_EQUAL_TEXT(2.0F, det_hist.det_data[1].vcs_position_y, F360_EPSILON, "Expected historic detection lateral position not 2.0F")
   DOUBLES_EQUAL_TEXT(5.0F, det_hist.det_data[1].vcs_position_x, F360_EPSILON, "Expected historic detection longitudinal position is not 0.1F")
   DOUBLES_EQUAL_TEXT(6.5F, det_hist.det_data[1].rdot, F360_EPSILON, "Expected historic detection range rate is not 6.5F")
   DOUBLES_EQUAL_TEXT(5.0F, det_hist.det_data[1].rdot_comp, F360_EPSILON, "Expected historic detection  range rate compensated is not 5.0F")
   DOUBLES_EQUAL_TEXT(0.2F, det_hist.det_data[1].vcs_az, F360_EPSILON, "Expected historic detection vcs_az is not 0.2F")
   DOUBLES_EQUAL_TEXT(2.0F, det_hist.det_data[1].time_since_meas, F360_EPSILON, "Expected historic detectiontime_since_meas is not 2.0F")
   DOUBLES_EQUAL_TEXT(26.0F, det_hist.det_data[1].v_wrapping, F360_EPSILON, "Expected historic detection v_wrapping is not 26.0F")
   DOUBLES_EQUAL_TEXT(10.0F, det_hist.det_data[1].r_wrapping, F360_EPSILON, "Expected historic detection r_wrapping is not 10.0F")
   CHECK_EQUAL_TEXT(2U, det_hist.det_data[1].cluster_idx, "Expected historic detection cluster index is not 2U")
   CHECK_EQUAL_TEXT(0U, det_hist.det_data[1].motion_status, "Expected historic detection motion status is not 0U")
   CHECK_EQUAL_TEXT(1U, det_hist.det_data[1].wheel_spin_type, "Expected historic detection wheel_spin_type is not 1U")
   CHECK_EQUAL_TEXT(1U, det_hist.det_data[1].f_dealiased, "Expected historic detection f_dealiased is not 1U")
   CHECK_EQUAL_TEXT(0U, det_hist.det_data[1].f_FOV_edge, "Expected historic detection f_FOV_edge is not 0U")
   CHECK_EQUAL_TEXT(1U, det_hist.det_data[1].f_selected, "Expected historic detection f_selected is not 1U")
   CHECK_EQUAL_TEXT(1U, det_hist.det_data[1].f_is_range_in_all_looks, "Expected historic detection f_is_range_in_all_looks is not 1U")
   CHECK_EQUAL_TEXT(0U, det_hist.det_data[1].f_potential_angle_jump, "Expected historic detection f_potential_angle_jump is not 0U")

   /** \action
    * change the det_hist_data_log and call Populate_Internal_Detection_History_Data() again to verify if it will return the values as expected
    */
   det_hist_data_log[0].occupied_idx = 0;
   Populate_Internal_Detection_History_Data(det_hist, tracker_info, det_hist_data_log);

   /** \result
    * check that the output match expected data.
    */
   DOUBLES_EQUAL_TEXT(2.0F, det_hist.det_data[0].vcs_position_y, F360_EPSILON, "Expected historic detection lateral position not 2.0F")
   DOUBLES_EQUAL_TEXT(5.0F, det_hist.det_data[0].vcs_position_x, F360_EPSILON, "Expected historic detection longitudinal position is not 0.1F")
   DOUBLES_EQUAL_TEXT(6.5F, det_hist.det_data[0].rdot, F360_EPSILON, "Expected historic detection range rate is not 6.5F")
   DOUBLES_EQUAL_TEXT(5.0F, det_hist.det_data[0].rdot_comp, F360_EPSILON, "Expected historic detection  range rate compensated is not 5.0F")
   DOUBLES_EQUAL_TEXT(0.2F, det_hist.det_data[0].vcs_az, F360_EPSILON, "Expected historic detection vcs_az is not 0.2F")
   DOUBLES_EQUAL_TEXT(2.0F, det_hist.det_data[0].time_since_meas, F360_EPSILON, "Expected historic detectiontime_since_meas is not 2.0F")
   DOUBLES_EQUAL_TEXT(26.0F, det_hist.det_data[0].v_wrapping, F360_EPSILON, "Expected historic detection v_wrapping is not 26.0F")
   DOUBLES_EQUAL_TEXT(10.0F, det_hist.det_data[0].r_wrapping, F360_EPSILON, "Expected historic detection r_wrapping is not 10.0F")
   CHECK_EQUAL_TEXT(2U, det_hist.det_data[0].cluster_idx, "Expected historic detection cluster index is not 2U")
   CHECK_EQUAL_TEXT(0U, det_hist.det_data[0].motion_status, "Expected historic detection motion status is not 0U")
   CHECK_EQUAL_TEXT(1U, det_hist.det_data[0].wheel_spin_type, "Expected historic detection wheel_spin_type is not 1U")
   CHECK_EQUAL_TEXT(1U, det_hist.det_data[0].f_dealiased, "Expected historic detection f_dealiased is not 1U")
   CHECK_EQUAL_TEXT(0U, det_hist.det_data[0].f_FOV_edge, "Expected historic detection f_FOV_edge is not 0U")
   CHECK_EQUAL_TEXT(1U, det_hist.det_data[0].f_selected, "Expected historic detection f_selected is not 1U")
   CHECK_EQUAL_TEXT(1U, det_hist.det_data[0].f_is_range_in_all_looks, "Expected historic detection f_is_range_in_all_looks is not 1U")
   CHECK_EQUAL_TEXT(0U, det_hist.det_data[0].f_potential_angle_jump, "Expected historic detection f_potential_angle_jump is not 0U")

   /** \action
    * add another det hist log that makes the the logged data over tracker_info.variant.num_hist_dets
    * call Populate_Internal_Detection_History_Data() again to verify if it will return the values as expected
    */
   det_hist_data_log[1].vcs_long_posn = 5.0F;
   det_hist_data_log[1].vcs_lat_posn = 2.0F;
   det_hist_data_log[1].vcs_long_posn = 5.0F;
   det_hist_data_log[1].rdot = 6.5F;
   det_hist_data_log[1].rdot_comp = 5.0F;
   det_hist_data_log[1].vcs_az = 0.2F;
   det_hist_data_log[1].time_since_meas = 2.0F;
   det_hist_data_log[1].v_wrapping = 26.0F;
   det_hist_data_log[1].r_wrapping = 10.0F;
   det_hist_data_log[1].cluster_idx = 2U;
   det_hist_data_log[1].occupied_idx = 2;
   det_hist_data_log[1].motion_status = 0U;
   det_hist_data_log[1].wheel_spin_type = 1U;
   det_hist_data_log[1].f_dealiased = 1U;
   det_hist_data_log[1].f_FOV_edge = 0U;
   det_hist_data_log[1].f_selected = 1U;
   det_hist_data_log[1].f_is_range_in_all_looks = 1U;
   det_hist_data_log[1].f_potential_angle_jump = 0U;
   Populate_Internal_Detection_History_Data(det_hist, tracker_info, det_hist_data_log);


   det_hist_data_log[1].occupied_idx = 3;
   tracker_info.variant.num_hist_dets = 2U;
   Populate_Internal_Detection_History_Data(det_hist, tracker_info, det_hist_data_log);

   det_hist_data_log[1].occupied_idx = 2020;
   Populate_Internal_Detection_History_Data(det_hist, tracker_info, det_hist_data_log);
   CHECK_EQUAL_TEXT(1, det_hist.n_occupied, "Expected historic det_hist.n_occupied is not 0");

}

/** @}*/
