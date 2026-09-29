/** \file
   File with set of unit tests for cluster_grouping function
*/

#include "f360_cluster_grouping.h"
#include "f360_cluster_grouping_data_generator.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

#include "f360_kill_cluster.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_clear_cluster.h"
using namespace f360_variant_A;

/** \defgroup  f360_cluster_grouping
 *  @{
 */

/** \brief
 *  Test group for f360_cluster_grouping
 */
TEST_GROUP(f360_cluster_grouping)
{
   /** \setup
   * Setting up arguments for cluster_grouping function and assigning them with reasonable basic values
   **/
   F360_Calibrations_T calib = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Detection_Hist_T detections_hist = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Globals_T globals = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   float32_t rngrate_interval_width_older;
   float32_t rngrate_interval_width_newer;
   float32_t interval_older;
   float32_t interval_newer;

   float32_t tolerance = 1e-3;
   float32_t rdot_interval_width_1 = 69.8359375;
   float32_t rdot_interval_width_2 = 59.6132813;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      globals.max_otg_speed = 70.0F;

      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = rdot_interval_width_1;
      sensors[0].refined.time_since_measurement_s = 0.05F;

      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].constant.v_wrapping[0] = rdot_interval_width_2;
      sensors[1].refined.time_since_measurement_s = 0.04F;

      tracker_info.num_unique_rdot_interval_widths = 2;
      tracker_info.unique_rdot_interval_widths[0] = rdot_interval_width_1;
      tracker_info.unique_rdot_interval_widths[1] = rdot_interval_width_2;

      tracker_info.rdot_interval_compatibility[0][1] = true;
      tracker_info.rdot_interval_compatibility[1][0] = true;

      for (int16_t i = 0; i < NUMBER_OF_CLUSTERS; i++)
      {
         tracker_info.inactive_cluster_ids[i] = i + 1;
         clusters[i].id = i + 1;
      }

      // Filling two clusters with detections, making them applicable for merging
      F360_Cluster_T &cluster_older = clusters[0];
      Add_Det(det_props, raw_detection_list, cluster_older, 1, 1.1F);
      Add_Det(det_props, raw_detection_list, cluster_older, 1, 69.8F);
      Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 1.5F, 0.2F);

      F360_Cluster_T &cluster_newer = clusters[1];
      Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
      Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);
      Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 59.6F, 0.1F);
   }
};

/**
*\purpose  Test if clusters are merged when filled with detections applicable for merging and cluster position are the same
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Two_Active_Clusters_Should_Be_Merged_Same_Cluster_Pos)
{
   /** \precond
   * Add clusters created in test setup as active to tracker
   **/
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to one
   * New cluster should contain current and historical detections from both clusters
   * VCSLong sorted list should be in correct order
   **/
   int32_t expected_number_of_active_clusters = 1;
   int32_t expected_number_of_current_detections = 3;
   int32_t expected_number_of_historical_detections = 3;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[0].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[0].num_old_dets);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_start == NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_start->id == 1);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_next[0] == NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_prev[0] == NULL);
}

/**
*\purpose  Test if clusters are merged when filled with detections applicable for merging and youngest cluster has largest vcs long pos
*          (i.e. swapping of elements in sorted vcs list is not needed)
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Two_Active_Clusters_Should_Be_Merged_Youngest_Cluster_Furthest)
{
   /** \precond
   * Add clusters created in test setup as active to tracker.
   * Change position of one of the current detections in youngest cluster (id == 2) to have a larger vcs long pos (0 was used in default setup)
   **/
   const int16_t det_idx = clusters[1].detids[0] - 1;
   det_props[det_idx].vcs_position.x += 0.1F;
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to one
   * New cluster should contain current and historical detections from both clusters
   * The vcslong sorted list shiuld be in correct order
   **/
   int32_t expected_number_of_active_clusters = 1;
   int32_t expected_number_of_current_detections = 3;
   int32_t expected_number_of_historical_detections = 3;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[0].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[0].num_old_dets);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_start == NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_start->id == 1);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_next[0] == NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_prev[0] == NULL);
}

/**
*\purpose  Test if clusters are merged when filled with detections applicable for merging and oldest cluster has larger vcs long pos
*          (i.e. swapping of elements in sorted vcs list is needed)
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Two_Active_Clusters_Should_Be_Merged_Oldest_Cluster_Furthest)
{
   /** \precond
   * Add clusters created in test setup as active to tracker.
   * Change position of one of the current detections in older cluster (id == 1) to have a larger vcs long pos (0 was used in default setup)
   **/
   const int16_t det_idx = clusters[0].detids[0] - 1;
   det_props[det_idx].vcs_position.x += 0.1F;
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to one
   * New cluster should contain current and historical detections from both clusters
   **/
   int32_t expected_number_of_active_clusters = 1;
   int32_t expected_number_of_current_detections = 3;
   int32_t expected_number_of_historical_detections = 3;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[0].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[0].num_old_dets);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_start == NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_start->id == 1);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_next[0] == NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_prev[0] == NULL);
}

/**
*\purpose  Test if clusters are merged when filled with detections applicable for merging and oldest cluster has larger vcs long pos
*          (i.e. swapping of elements in sorted vcs list is needed) and there are other clustes that form the start and end of the
*          sorted vcslong list (i.e. other clusters with smaller and larger vcs long position)
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Two_Active_Clusters_Should_Be_Merged_Oldest_Cluster_Furthest_More_Clusters_Present)
{
   /** \precond
   * Add clusters created in test setup as active to tracker.
   * Change position of one of the current detections in older cluster (id == 1) to have a larger vcs long pos (0 was used in default setup)
   * Add one cluster with smaller vcs long position
   * Add one cluster with larger vcs long position
   **/
   const int16_t det_idx = clusters[0].detids[0] - 1;
   det_props[det_idx].vcs_position.x += 0.1F;
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   F360_Cluster_T &cluster_start = clusters[2];
   Add_Det(det_props, raw_detection_list, cluster_start, 1, 7.0F, -10.0F); // Detection with long pos -10m and rr that doesn't match the other clusters
   Add_Simple_Cluster_To_Tracker_Status(2, clusters, tracker_info);

   F360_Cluster_T &cluster_end = clusters[3];
   Add_Det(det_props, raw_detection_list, cluster_end, 1, -7.0F, 10.0F); // Detection with long pos 10m and rr that doesn't match the other clusters
   Add_Simple_Cluster_To_Tracker_Status(3, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to one
   * New cluster should contain current and historical detections from both clusters
   * The vcslong sorted list should be in correct order
   **/
   int32_t expected_number_of_active_clusters = 3;
   int32_t expected_number_of_current_detections = 3;
   int32_t expected_number_of_historical_detections = 3;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[0].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[0].num_old_dets);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_start == NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_start->id == 3);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_prev[2] ==  NULL);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_next[2] ==  NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_next[2]->id == 1);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_prev[0] ==  NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_prev[0]->id == 3);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_next[0] ==  NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_next[0]->id == 4);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_prev[3] ==  NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_prev[3]->id == 1);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_next[3] ==  NULL);
}

/**
*\purpose  Test if clusters are merged when filled with detections applicable for merging and oldest cluster has larger vcs long pos
*          (i.e. swapping of elements in sorted vcs list is needed) and there are one more cluster present with long pos in between the two
*          other clusters (such that the clusters to merge are not adjacent to each other in the vcslong sorted list)
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Two_Active_Clusters_Should_Be_Merged_Oldest_Cluster_Furthest_Cluster_Not_Adjacent)
{
   /** \precond
   * Add clusters created in test setup as active to tracker.
   * Change position of one of the current detections in older cluster (id == 1) to have a larger vcs long pos (0 was used in default setup)
   * Add one cluster with vcs long position in between the two other clusters
   * Add one cluster with larger vcs long position
   **/
   const int16_t det_idx = clusters[0].detids[0] - 1;
   det_props[det_idx].vcs_position.x += 0.1F;
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   F360_Cluster_T &cluster_middle = clusters[2];
   Add_Det(det_props, raw_detection_list, cluster_middle, 1, 7.0F, 0.01F); // Detection with long pos 0.01m and rr that doesn't match the other clusters
   Add_Simple_Cluster_To_Tracker_Status(2, clusters, tracker_info);


   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to one
   * New cluster should contain current and historical detections from both clusters
   **/
   int32_t expected_number_of_active_clusters = 2;
   int32_t expected_number_of_current_detections = 3;
   int32_t expected_number_of_historical_detections = 3;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[0].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[0].num_old_dets);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_start == NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_start->id == 1);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_prev[0] ==  NULL);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_next[0] ==  NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_next[0]->id == 3);
   CHECK_FALSE(tracker_info.vcslong_sorted_cluster_prev[2] ==  NULL);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_prev[2]->id == 1);
   CHECK_TRUE(tracker_info.vcslong_sorted_cluster_next[2] ==  NULL);

}

/**
*\purpose  Test for correct operation when none of clusters is active
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_No_Active_Clusters_In_Tracker)
{
   /** \precond
   * Two clusters are filled with detections in test setup, but none of them is added as active to tracker
   **/

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * No active clusters in tracker, no merges in existing clusters
   **/
   int32_t expected_number_of_active_clusters = 0;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose  Test if no cluster merges have been done when only one cluster is active
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Only_One_Active_Cluster_In_Tracker)
{
   /** \precond
   * Adding one cluster as active to tracker
   **/
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * No changes to number of active clusters, no cluster merges done
   **/
   int32_t expected_number_of_active_clusters = 1;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose  Test if clusters are NOT merged when distance between them too large
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Cluster_Detections_Far_Away_From_Each_Other_And_Coarse_Gate_Fails)
{
   /** \precond
   * First cluster is located at (0, 0) as initialized in test setup
   * Modify second cluster by adding detection located at (10, 10) VCS
   * Add both clusters to tracker as active
   **/
   F360_Cluster_T &cluster_newer = clusters[1];
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1, 10.0F);

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * No changes to number of active clusters, no cluster merges done
   **/
   int32_t expected_number_of_active_clusters = 2;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose  Test if clusters are NOT merged when dealiasing fails
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Cluster_Detection_Range_Rates_Do_Not_Match)
{
   /** \precond
   * First cluster is initialized in test setup
   * Modify second cluster by adding additional detection to make dealiasing condition fail
   * Add both clusters to tracker as active
   **/
   F360_Cluster_T &cluster_newer = clusters[1];
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 30.1);
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * No changes to number of active clusters, no cluster merges done
   **/
   int32_t expected_number_of_active_clusters = 2;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose  Test if clusters are NOT merged when distance between them too large
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Cluster_Detections_Far_Away_From_Each_Other_And_Fine_Gate_Fails)
{
   /** \precond
   * First cluster is located at (0, 0) as initialized in test setup
   * Modify second cluster by adding detection located at (1, 1) VCS
   * Add both clusters to tracker as active
   **/
   F360_Cluster_T &cluster_newer = clusters[1];
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1, 1.0F, 1.0F);

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * No changes to number of active clusters, no cluster merges done
   **/
   int32_t expected_number_of_active_clusters = 2;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose  Test if clusters are NOT merged when number of detections in clusters too large to be merged
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Number_Of_Detections_In_Clusters_Too_Large_To_Be_Merged)
{
   /** \precond
   * Fill second cluster to reach maximum number of detections in both clusters combined (3 detections already added in test setup)
   * Add both clusters to tracker as active
   **/
   const uint32_t detections_added_in_test_setup = 3U;
   F360_Cluster_T &cluster_newer = clusters[1];
   for (uint16_t i = 0U; i < MAX_DETS_IN_OBJ_TRK - detections_added_in_test_setup; i++)
   {
      Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1);
   }
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * No changes to number of active clusters, no cluster merges done
   **/
   const int32_t expected_number_of_active_clusters = 1;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose  Test if clusters are NOT merged when number of historical detections in clusters too large to be merged
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Number_Of_Historical_Detections_In_Clusters_Too_Large_To_Be_Merged)
{
   /** \precond
   * Fill second cluster to reach maximum number of historical detections in both clusters combined (3 detections already added in test setup)
   * Add both clusters to tracker as active
   **/
   const uint32_t detections_added_in_test_setup = 3U;
   F360_Cluster_T &cluster_newer = clusters[1];
   for (uint16_t i = 0U; i < MAX_DETS_IN_OBJ_TRK - detections_added_in_test_setup; i++)
   {
      Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);
   }
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * No changes to number of active clusters, no cluster merges done
   **/
   const int32_t expected_number_of_active_clusters = 1;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose Test if 3 clusters are merged when they are applicable for merging
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Three_Active_Clusters_Should_Be_Merged)
{
   /** \precond
   * Create third cluster and add it as active cluster to tracker along with the other two
   **/
   F360_Cluster_T &cluster_newest = clusters[2];
   Add_Det(det_props, raw_detection_list, cluster_newest, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newest, rdot_interval_width_2, 1.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newest, rdot_interval_width_2, 59.7F, 0.1F);

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(2, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to two
   * New cluster should contain current and historical detections from all three clusters
   **/
   int32_t expected_number_of_active_clusters = 1;
   int32_t expected_number_of_current_detections = 4;
   int32_t expected_number_of_historical_detections = 5;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[0].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[0].num_old_dets);
}

/**
*\purpose Test if first two clusters are merged when three active cluster exist
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_First_Two_Out_Of_Three_Clusters_Should_Be_Merged)
{
   /** \precond
   * Create third cluster and add it as active cluser to tracker along with the other two
   **/
   F360_Cluster_T &cluster_newest = clusters[2];
   Add_Det(det_props, raw_detection_list, cluster_newest, 1, 0.1, 2.0F, 5.0F);

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(2, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to two
   * First and second cluster should be merged into one
   **/
   int32_t expected_number_of_active_clusters = 2;
   int32_t expected_number_of_current_detections = 3;
   int32_t expected_number_of_historical_detections = 3;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[0].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[0].num_old_dets);
}

/**
*\purpose Test if first and last clusters are merged when three active cluster exist
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_First_And_Last_Out_Of_Three_Clusters_Should_Be_Merged)
{
   /** \precond
   * Add additional detection to second cluster to make it fail merge checks
   * Create third cluster and add it as active cluser to tracker along with the other two
   **/
   F360_Cluster_T &cluster_new = clusters[1];
   Add_Det(det_props, raw_detection_list, cluster_new, 1, 30.1);

   F360_Cluster_T &cluster_newest = clusters[2];
   Add_Det(det_props, raw_detection_list, cluster_newest, 1, 0.1);

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(2, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to two
   * First and second cluster should be merged into one
   **/
   int32_t expected_number_of_active_clusters = 2;
   int32_t expected_number_of_current_detections = 3;
   int32_t expected_number_of_historical_detections = 1;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[0].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[0].num_old_dets);
}

/**
*\purpose Test if second and third clusters are merged when three active cluster exist
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Second_And_Third_Out_Of_Three_Clusters_Should_Be_Merged)
{
   /** \precond
   * Add additional detection to first cluster (id==1) to make it fail merge checks
   * Create third cluster and add it as active cluster to tracker along with the other two
   **/
   F360_Cluster_T &cluster_old = clusters[0];
   Add_Det(det_props, raw_detection_list, cluster_old, 1, 40.9F);
   Add_Det(det_props, raw_detection_list, cluster_old, 2, 40.7F);
   Add_Det(det_props, raw_detection_list, cluster_old, 2, 41.0F);

   F360_Cluster_T &third_cluster = clusters[2];
   Add_Det(det_props, raw_detection_list, third_cluster, 1, 0.1F);
   Add_Hist_Det(detections_hist, third_cluster, rdot_interval_width_2, 59.1F, 0.2F); // Setting time since measurement for historical detections to 0.2s (i.e. larger than for cluster[1] historical detections) makes this the older cluster meaning that its id will be preserved while cluster[1] will be killed after merge
   Add_Hist_Det(detections_hist, third_cluster, rdot_interval_width_2, 59.7F, 0.2F);

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(2, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to two
   * First and second cluster should be merged into one
   **/
   int32_t expected_number_of_active_clusters = 2;
   int32_t expected_number_of_current_detections = 2;
   int32_t expected_number_of_historical_detections = 4;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[2].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[2].num_old_dets);
}


/**
*\purpose Test if clusters are not merged when all far away from each other
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Three_Clusters_Longitudinaly_Far_Away_From_Each_Other)
{
   /** \precond
   * Add additional detection to second cluster to increase longitudinal distance of whole cluster
   * Create third cluster with large longitudinal distance from the other two
   * and add it as active cluster to tracker along with the other two
   **/
   F360_Cluster_T &cluster_new = clusters[1];
   Add_Det(det_props, raw_detection_list, cluster_new, 1, 59.2F, 10.0F, 0.0F);

   F360_Cluster_T &cluster_newest = clusters[2];
   Add_Det(det_props, raw_detection_list, cluster_newest, 1, 0.1F, 20.0F, 0.0F);

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(2, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to two
   * First and second cluster should be merged into one
   **/
   int32_t expected_number_of_active_clusters = 3;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose Test if the first cluster is killed when marked with f_to_be_killed flag
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_First_Cluster_Is_Marked_To_Be_Killed)
{
   /** \precond
   * Mark first cluster as the one to be killed, add two clusters as active to tracker
   **/
   clusters[0].f_to_be_killed = true;
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Number of active clusters should decrease to one, as the first one was killed,
   * only second cluster still active, no cluster merges expected in the second cluster
   **/
   int32_t expected_number_of_active_clusters = 1;
   int32_t expected_id_of_active_cluster = 2;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_id_of_active_cluster, tracker_info.active_cluster_ids[0]);
}

/**
*\purpose Test if the second cluster is killed when marked with f_to_be_killed flag
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Second_Cluster_Is_Marked_To_Be_Killed)
{
   /** \precond
   * Mark second cluster as the one to be killed, add two clusters as active to tracker
   **/
   clusters[1].f_to_be_killed = true;
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Number of active clusters should decrease to one, as the second one was killed,
   * only first cluster still active, no cluster merges expected in the first cluster
   **/
   int32_t expected_number_of_active_clusters = 1;
   int32_t expected_id_of_active_cluster = 1;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_id_of_active_cluster, tracker_info.active_cluster_ids[0]);
}

/**
*\purpose  Test if both clusters are killed when they are marked with f_to_be_killed flag
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Both_Clusters_Are_Marked_To_Be_Killed)
{
   /** \precond
   * Both clusters are marked to be killed and added to tracker as active
   **/
   clusters[0].f_to_be_killed = true;
   clusters[1].f_to_be_killed = true;

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Number of active clusters should decrease to zero
   **/
   int32_t expected_number_of_active_clusters = 0;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
}

/**
*\purpose Test if two clusters are merged when three active cluster exist, but the first one is marked as to be killed
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_When_Three_Clusters_Applicable_For_Merging_But_First_To_Be_Killed)
{
   /** \precond
   * Mark first cluster as the one to be killed
   * Create third cluster and add it as active cluser to tracker along with the other two
   **/
   clusters[0].f_to_be_killed = true;
   F360_Cluster_T &cluster_newest = clusters[2];
   Add_Det(det_props, raw_detection_list, cluster_newest, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newest, rdot_interval_width_2, 1.1F, 0.2F);

   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   Add_Simple_Cluster_To_Tracker_Status(2, clusters, tracker_info);

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Active clusters number and number of cluster merges should be equal to two
   * Second and third cluster should be merged into one
   **/
   int32_t expected_number_of_active_clusters = 1;
   int32_t expected_number_of_current_detections = 2;
   int32_t expected_number_of_historical_detections = 3;
   CHECK_EQUAL(expected_number_of_active_clusters, tracker_info.num_active_clusters);
   CHECK_EQUAL(expected_number_of_current_detections, clusters[2].ndets);
   CHECK_EQUAL(expected_number_of_historical_detections, clusters[2].num_old_dets);
}

/**
*\purpose Test if confidence blocker counter is correctly managed
*/
TEST(f360_cluster_grouping, Test_Cluster_Grouping_With_Confidence_Blocker)
{
   /** \precond
   * Set up two clusters such that they should merge into one
   * Set confidence blocker counter to 1 for each cluster
   **/

   clusters[10].ndets = 1;
   clusters[10].detids[0] = 10;
   clusters[10].low_rcs_dets_cnt = 1;
   clusters[10].f_dealiased = true;

   det_props[9].vcs_position.x = 0.0F;
   det_props[9].vcs_position.y = 5.0F;
   det_props[9].f_dealiased = true;
   raw_detection_list.detections[9].raw.sensor_id = 1;

   clusters[11].num_old_dets = 1;
   clusters[11].old_det_idx[0] = 0;
   clusters[11].low_rcs_dets_cnt = 1;
   clusters[11].f_dealiased = true;
   clusters[11].vcs_position_x = 0.0F;
   clusters[11].vcs_position_y = 5.0F;

   detections_hist.det_data[0].f_dealiased = true;
   detections_hist.det_data[0].time_since_meas = 0.2F;

   tracker_info.num_active_clusters = 2;
   tracker_info.active_cluster_ids[0] = 11;
   tracker_info.active_cluster_ids[1] = 12;
   tracker_info.vcslong_sorted_cluster_start = &clusters[10];
   tracker_info.vcslong_sorted_cluster_next[10] = &clusters[11];

   /** \action
   * Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   * Check that the confidence blocker counter has increased to two
   * Check that clusters merged succesfully
   **/
   CHECK_EQUAL(2, clusters[11].low_rcs_dets_cnt);
   CHECK_EQUAL(1, tracker_info.num_active_clusters);
}
/** @}*/
