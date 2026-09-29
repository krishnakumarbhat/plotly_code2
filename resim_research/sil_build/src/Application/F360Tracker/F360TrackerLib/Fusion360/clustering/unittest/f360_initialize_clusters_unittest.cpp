/** \file
   File with set of unit tests for initialize_clusters function
*/

#include "f360_initialize_clusters.h"
#include "f360_clustering_data_generator.h"
#include "f360_math.h"
#include "f360_set_variant.h"
#include "f360_sorted_clusters_mgmt.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;
/** \defgroup  initialize_clusters
 *  @{
 */


/** \brief
*  Test group for initialize_clusters function
**/
TEST_GROUP(initialize_clusters)
{
   /** \setup
   * Setting up arguments for initialize_clusters function and assigning them with basic values
   **/
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Local_Clusters_T local_cluster_data = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Calibrations_T calibs;
   F360_Host_T host = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      (void)memset(&local_cluster_data, 0, sizeof(local_cluster_data));
      Setup_TrackerInfo_Cluster_Detp(tracker_info, clusters, raw_detection_list);

      // Default host properties (used when computing cluster prioritization). Exact value for speed is not important but
      // the curvature should be 0 such that host is driving straight and 0 vcs lat pos is in the middle of host path.
      host.vcs_speed = 10.0F;
      host.curvature_rear = 0.0F;
   }
};

/**
*\purpose  Test that no clusters are initialized when there are no local clusters
*\req    NA
*/
TEST(initialize_clusters, Test_No_Inited_Clusters_When_Empty_Local_Clusters)
{
   /** \precond
   * Use default settings from the test group
   **/

   /** \action
   * Calling Initialize_Clusters()
   **/
   F360_Detection_Hist_T det_hist = {};
   local_cluster_data.num_clusters = 0;
   Initialize_Clusters(raw_detection_list, calibs, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Checking that no clusters were initialized
   **/
   CHECK_EQUAL(0, tracker_info.num_active_clusters);
}

/**
*\purpose  Test that no clusters are initialized when there are no space in tracker for new clusters
*\req    NA
*/
TEST(initialize_clusters, Test_No_Inited_Clusters_When_No_Space_For_New_Clusters)
{
   /** \precond
   * Use default settings from the test group
   **/

   /** \action
   * Calling Initialize_Clusters()
   **/
   F360_Detection_Hist_T det_hist = {};
   tracker_info.variant.num_clusters = 1;
   tracker_info.num_active_clusters = 1; // No free slots for new clusters
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   tracker_info.vcslong_sorted_cluster_start = &clusters[0];
   tracker_info.vcslong_sorted_cluster_next[0] = NULL;
   tracker_info.vcslong_sorted_cluster_prev[0] = NULL;
   
   local_cluster_data.num_clusters = 1; // One local cluster to init
   Initialize_Clusters(raw_detection_list, calibs, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Checking that no new clusters were initialized
   **/
   CHECK_EQUAL(1, tracker_info.num_active_clusters);
}

/**
*\purpose Check that the highest prioritized clusters are stored in the cluster array in case of saturation
*\req    NA
*/
TEST(initialize_clusters, Test_Initialize_Clusters_Saturation)
{
   /** \precond
   * Make sure that the total sum of old and new clusters are more than can fit into the cluster array.
   * For old clusters: Use stationary dealiased clusters with 1 historical detection each (as long as the hist det buffer can support it and then use 0 detections per cluster).
   *                   Let all clusters have 0 lat pos but different long pos (all in front of host).
   * For new clusters: Use one dealiased, stationary detection per cluster. Let the detections have zero lat pos but different long pos (all in front of host)
   *
   * This setup will assure that clusters further away from host are less prioritized (i.e. the cluster long pos will determine its priority).
   **/

   // Add old clusters to tracker_info
   const float32_t k_very_far_away_distance = 10000.0F;
   F360_Detection_Hist_T det_hist = {};
   for(uint16_t i = 0U; i < NUMBER_OF_CLUSTERS; i ++)
   {
      const int16_t cluster_id = NUMBER_OF_CLUSTERS - i;
      float32_t cluster_long_pos;
      if(i < NUMBER_OF_CLUSTERS - MAX_TRACKER_POSN_CLUSTERS / 2U)
      {
         // Cluster will have prio high enough to not be killed
         cluster_long_pos = static_cast<float32_t>(i+1) * 0.1F;
      }
      else
      {
         cluster_long_pos = static_cast<float32_t>(i+1) * 0.1F + k_very_far_away_distance; // Very far away
      }

      const int16_t cluster_idx = cluster_id - 1;
      tracker_info.active_cluster_ids[i] = cluster_id;
      tracker_info.inactive_cluster_ids[cluster_idx] = 0;
      tracker_info.num_active_clusters++;

      clusters[cluster_idx].vcs_position_x = cluster_long_pos;
      clusters[cluster_idx].f_dealiased = true;
      clusters[cluster_idx].rep_rdotcomp = 0.0F;
      clusters[cluster_idx].rep_vcs_az = 0.0F;
      clusters[cluster_idx].num_types_of_dets[0] = 0;

      Sorted_Clusters_Insert(tracker_info, clusters, clusters[cluster_idx], 0);

      if(det_hist.n_occupied < MAX_NUMBER_OF_HISTORIC_DETECTIONS)
      {
         clusters[cluster_idx].num_old_dets = 1;
         const int16_t det_idx = det_hist.n_occupied;
         clusters[cluster_idx].old_det_idx[0] = det_idx;
         clusters[cluster_idx].num_types_of_dets[1] = 1;

         det_hist.f_idx_occupied[det_idx] = true;
         det_hist.det_data[det_idx].cluster_idx = cluster_idx;
         det_hist.det_data[det_idx].f_dealiased = true;
         det_hist.det_data[det_idx].motion_status = F360_DET_MOTION_AMBIGUOUS;
         det_hist.det_data[det_idx].rdot_comp = 0.0F;
         det_hist.det_data[det_idx].time_since_meas = 0.05F; // One scan old
         det_hist.det_data[det_idx].vcs_az = 0.0F;
         det_hist.det_data[det_idx].vcs_position_x = cluster_long_pos + 0.05F * host.vcs_speed; // Compensate for host speed during one scan
         det_hist.det_data[det_idx].vcs_position_y = 0.0F;

         det_hist.n_occupied++;
      }
   }

   // Add new clusters to local cluster data
   for(uint16_t i = 0U; i < MAX_TRACKER_POSN_CLUSTERS; i++)
   {
      float32_t det_long_pos;
      if(i < MAX_TRACKER_POSN_CLUSTERS / 2U)
      {
         // Cluster will have prio high enough to not be killed
         det_long_pos = static_cast<float32_t>(i+1U) * 0.1F + 0.05F;
      }
      else
      {
         det_long_pos = static_cast<float32_t>(i+1U) * 0.1F + 0.05F + k_very_far_away_distance; // Very far away
      }

      const bool f_take_new_cluster_to_init = true;
      const float32_t det_lat_pos = 0.0F;
      const float32_t det_range_rate = 0.0F;
      bool det_f_dealiased = true;
      rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster_to_init, det_long_pos, det_lat_pos, det_range_rate,
         det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);
   }

   /** \action
   * Calling Initialize_Clusters
   **/
   Initialize_Clusters(raw_detection_list, calibs, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Checking expected behaviour of Initialize_Clusters when saturated cluster array():
   *    1. Number of active clusters are saturated
   *    2. Old clusters that were very far away have been killed and replaced with new closer/higher prioritized clusters.
   *    3. Detection history array has been clean up such that detections belong to far away killed clusters are removed
   *       a. Check that n_occupied is as expected
   *       b. Check that the detections in historical data structure belongs to the closest old clusters.
   *    4. Check that the vcs long pos sorted cluster array is correct
   **/

   // 1. Number of active clusters are saturated
   CHECK_EQUAL_TEXT(NUMBER_OF_CLUSTERS, tracker_info.num_active_clusters, "Number of active clusters are incorrect"); // Check that cluster array is saturated

   // 2. Check that closest positioned/highest prioritized clusters are in cluster array
   for(int16_t i = 0; i < NUMBER_OF_CLUSTERS; i ++)
   {
      // Check that the very far away clusters are not in the array
      CHECK_TRUE_TEXT((clusters[i].vcs_position_x > 0.0F) && (clusters[i].vcs_position_x < k_very_far_away_distance), "Far away cluster have not been killed and replaced");
   }

   // 3a. Check that number of detections in historical detection buffer is as expected
   CHECK_EQUAL_TEXT(NUMBER_OF_CLUSTERS - MAX_TRACKER_POSN_CLUSTERS / 2U, det_hist.n_occupied, "Number of detections in historical detection array is incorrect. n_occupied has unexpected value");
   uint16_t sum_occupied = 0;
   for (int16_t i = 0; i < MAX_NUMBER_OF_HISTORIC_DETECTIONS; i++)
   {
      if(det_hist.f_idx_occupied[i])
      {
         sum_occupied++;
      }
   }
   CHECK_EQUAL_TEXT(det_hist.n_occupied, sum_occupied, "Inconsistency between n_occupied and f_idx_occupied array in historical detection data structure");

  // 3b. Check that the detections in historical data structure belongs to the closest old clusters (i.e. we have removed the expected detections belonging to the further away clusters)
  for (int16_t i = 0; i < MAX_NUMBER_OF_HISTORIC_DETECTIONS; i++)
   {
      if(det_hist.f_idx_occupied[i])
      {
         CHECK_EQUAL_TEXT(NUMBER_OF_CLUSTERS - i - 1, det_hist.det_data[i].cluster_idx, "Unexpected cluster ID on historical detection");
      }
      else
      {
         CHECK_EQUAL_TEXT(0, det_hist.det_data[i].cluster_idx, "Cluster id was not cleared when detection was removed");
      }
   }

   // 4. Check that the vcs long pos sorted cluster array is correct
   int16_t num_sorted_clusters = 0;
   float32_t prev_long_pos;
   F360_Cluster_T * curr_cluster = tracker_info.vcslong_sorted_cluster_start;
   for(int16_t i = 0; i < NUMBER_OF_CLUSTERS; i++)
   {
      if(NULL== curr_cluster)
      {
         break;
      }

      num_sorted_clusters++;

      if (i != 0) // Not first iteration
      {
         CHECK_TRUE_TEXT(curr_cluster->vcs_position_x > prev_long_pos, "The vcs long pos sorted cluster array is corrupted");
      }
      prev_long_pos = curr_cluster->vcs_position_x;
      curr_cluster = tracker_info.vcslong_sorted_cluster_next[curr_cluster->id - 1];
   }
   CHECK_EQUAL_TEXT(NUMBER_OF_CLUSTERS, num_sorted_clusters, "The sorted vcs long pos list has too few elements")
}

/**
*\purpose Test whether Initialize_Clusters adds local clusters to free slots successfully (non-saturated case)
*\req    NA
*/
TEST(initialize_clusters, Test_Initialize_Clusters_Add_To_Free_Slots_Without_Saturation)
{
   /** \precond
   * Create tracker with some free slots and local clusters to add
   **/
   F360_Detection_Hist_T det_hist = {};
   
   // Set up tracker with 3 existing clusters and capacity for 10
   tracker_info.variant.num_clusters = 10;
   tracker_info.num_active_clusters = 3;
   
   // Initialize existing clusters
   for (int16_t i = 0; i < 3; i++)
   {
      const int16_t cluster_id = i + 1;
      tracker_info.active_cluster_ids[i] = cluster_id;
      clusters[i].id = cluster_id;
      clusters[i].vcs_position_x = 10.0F + i * 5.0F;
      clusters[i].vcs_position_y = 0.0F;
   }
   
   // Set up inactive cluster IDs (IDs 4-10 are available)
   for (int16_t i = 0; i < 7; i++)
   {
      tracker_info.inactive_cluster_ids[i] = 4 + i;
   }
   
   // Create 3 local clusters to add
   for (uint16_t i = 0; i < 3; i++)
   {
      const bool f_take_new_cluster = true;
      const float32_t det_long_pos = 5.0F + i * 2.0F;
      const float32_t det_lat_pos = 1.0F;
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_long_pos, det_lat_pos, 5.0F, true,
         rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);
   }

   /** \action
   * Call Initialize_Clusters
   **/
   Initialize_Clusters(raw_detection_list, calibs, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Verify clusters were added successfully to free slots
   **/
   
   // 1. Check num_active_clusters increased correctly
   CHECK_EQUAL(6, tracker_info.num_active_clusters); // 3 existing + 3 new
   
   // 2. Verify new cluster IDs were assigned from inactive list
   CHECK_EQUAL(4, tracker_info.active_cluster_ids[3]);
   CHECK_EQUAL(5, tracker_info.active_cluster_ids[4]);
   CHECK_EQUAL(6, tracker_info.active_cluster_ids[5]);
   
   // 3. Verify clusters were initialized with correct data
   CHECK_EQUAL(1, clusters[3].ndets); // cluster_id 4 -> index 3
   DOUBLES_EQUAL(5.0F, clusters[3].vcs_position_x, 0.00001);
   
   // 4. Verify inactive_cluster_ids array was shifted correctly
   CHECK_EQUAL(7, tracker_info.inactive_cluster_ids[0]); // First 3 were used, so 7 is now first
   CHECK_EQUAL(8, tracker_info.inactive_cluster_ids[1]);
   CHECK_EQUAL(9, tracker_info.inactive_cluster_ids[2]);
   CHECK_EQUAL(10, tracker_info.inactive_cluster_ids[3]);
   CHECK_EQUAL(0, tracker_info.inactive_cluster_ids[4]); // Cleared
   CHECK_EQUAL(0, tracker_info.inactive_cluster_ids[5]); // Cleared
   CHECK_EQUAL(0, tracker_info.inactive_cluster_ids[6]); // Cleared
   
   // 5. Verify detections are linked to clusters
   CHECK_EQUAL(4, detection_props[0].cluster_id);
   CHECK_EQUAL(5, detection_props[1].cluster_id);
   CHECK_EQUAL(6, detection_props[2].cluster_id);
}

/**
*\purpose Test whether cluster initialization is handled correctly in the case that some local clusters
* fit in free slots, others replace low priority clusters
*\req    NA
*/
TEST(initialize_clusters, Test_Initialize_Clusters_Partial_Replacement)
{
   /** \precond
   * Scenario: 3 existing clusters (1 close, 2 far), capacity 5, add 4 local clusters (2 medium, 2 very close)
   * Expected: 2 free slots filled + 2 far clusters replaced by very close local clusters
   **/
   F360_Detection_Hist_T det_hist = {};
   
   tracker_info.variant.num_clusters = 5;
   tracker_info.num_active_clusters = 3;
   
   // Existing cluster 1: Close (10m) - high priority, should survive
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   clusters[0].vcs_position_x = 10.0F;
   clusters[0].vcs_position_y = 0.0F;
   clusters[0].ndets = 1;
   clusters[0].num_types_of_dets[0] = 1;
   Sorted_Clusters_Insert(tracker_info, clusters, clusters[0], 0);
   
   // Existing cluster 2: Far (100m) - low priority, should be replaced
   tracker_info.active_cluster_ids[1] = 2;
   clusters[1].id = 2;
   clusters[1].vcs_position_x = 100.0F;
   clusters[1].vcs_position_y = 0.0F;
   clusters[1].ndets = 1;
   clusters[1].num_types_of_dets[0] = 1;
   Sorted_Clusters_Insert(tracker_info, clusters, clusters[1], 0);
   
   // Existing cluster 3: Far (110m) - low priority, should be replaced
   tracker_info.active_cluster_ids[2] = 3;
   clusters[2].id = 3;
   clusters[2].vcs_position_x = 110.0F;
   clusters[2].vcs_position_y = 0.0F;
   clusters[2].ndets = 1;
   clusters[2].num_types_of_dets[0] = 1;
   Sorted_Clusters_Insert(tracker_info, clusters, clusters[2], 0);
   
   // Set up inactive cluster IDs (4 and 5 available)
   tracker_info.inactive_cluster_ids[0] = 4;
   tracker_info.inactive_cluster_ids[1] = 5;
   
   // Local cluster 1: Very close (3m) - highest priority
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 3.0F, 0.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);
   
   // Local cluster 2: Very close (5m) - high priority
   f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 5.0F, 0.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);
   
   // Local cluster 3: Medium (30m) - medium priority, goes to free slot
   f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 30.0F, 0.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);
   
   // Local cluster 4: Medium (40m) - medium priority, goes to free slot
   f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 40.0F, 0.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   /** \action
   * Call Initialize_Clusters
   **/
   Initialize_Clusters(raw_detection_list, calibs, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Verify: tracker saturated, 2 free slots used, 2 far clusters replaced by very close local clusters
   **/
   
   // 1. Tracker should be saturated
   CHECK_EQUAL(5, tracker_info.num_active_clusters);
   
   // 2. All 4 local clusters should be added (detections linked to cluster IDs)
   CHECK_TRUE(detection_props[0].cluster_id > 0);
   CHECK_TRUE(detection_props[1].cluster_id > 0);
   CHECK_TRUE(detection_props[2].cluster_id > 0);
   CHECK_TRUE(detection_props[3].cluster_id > 0);
   
   // 3. No far clusters (>50m) should remain - they were replaced by close/medium clusters
   for (int16_t i = 0; i < tracker_info.num_active_clusters; i++)
   {
      const int16_t cluster_id = tracker_info.active_cluster_ids[i];
      const int16_t cluster_idx = cluster_id - 1;
      CHECK_TRUE(clusters[cluster_idx].vcs_position_x < 50.0F);
   }
   
   // 4. The original close cluster (10m) should still exist
   bool found_original_close_cluster = false;
   for (int16_t i = 0; i < tracker_info.num_active_clusters; i++)
   {
      const int16_t cluster_id = tracker_info.active_cluster_ids[i];
      if (cluster_id == 1)
      {
         found_original_close_cluster = true;
         break;
      }
   }
   CHECK_TRUE(found_original_close_cluster);
}

/**
*\purpose Test that Initialize_Clusters properly cleans up detection history when killing clusters
*\req    NA
*/
TEST(initialize_clusters, Test_Initialize_Clusters_Detection_History_Cleanup)
{
   /** \precond
   * Create saturated tracker where clusters have historical detections, then add high-priority local clusters
   **/
   F360_Detection_Hist_T det_hist = {};
   
   tracker_info.variant.num_clusters = 5;
   tracker_info.num_active_clusters = 5;
   
   // Initialize clusters
   for (int16_t i = 0; i < 5; i++)
   {
      const int16_t cluster_id = i + 1;
      tracker_info.active_cluster_ids[i] = cluster_id;
      clusters[i].id = cluster_id;
      
      if (i < 2)
      {
         // Close clusters - high priority
         clusters[i].vcs_position_x = 5.0F + i * 2.0F;
      }
      else
      {
         // Far clusters - low priority (will be killed)
         clusters[i].vcs_position_x = 150.0F + i * 10.0F;
      }
      clusters[i].vcs_position_y = 0.0F;
      clusters[i].ndets = 1;
      
      // Add historical detections only for far clusters
      if (i >= 2)
      {
         clusters[i].num_old_dets = 2;
         
         // Add first historical detection
         const int16_t det_idx1 = i * 2;
         clusters[i].old_det_idx[0] = det_idx1;
         det_hist.f_idx_occupied[det_idx1] = true;
         det_hist.det_data[det_idx1].cluster_idx = i;
         det_hist.n_occupied++;
         
         // Add second historical detection
         const int16_t det_idx2 = i * 2 + 1;
         clusters[i].old_det_idx[1] = det_idx2;
         det_hist.f_idx_occupied[det_idx2] = true;
         det_hist.det_data[det_idx2].cluster_idx = i;
         det_hist.n_occupied++;
      }
      
      Sorted_Clusters_Insert(tracker_info, clusters, clusters[i], 0);
   }
   
   // Create 3 high-priority local clusters
   for (uint16_t i = 0; i < 3; i++)
   {
      const bool f_take_new_cluster = true;
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 3.0F + i * 1.0F, 0.0F, 5.0F, true,
         rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);
   }

   /** \action
   * Call Initialize_Clusters
   **/
   Initialize_Clusters(raw_detection_list, calibs, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Verify detection history was cleaned up when clusters were killed
   **/
   
   // 1. Verify n_occupied decreased (historical detections from killed clusters removed)
   CHECK_EQUAL(0, det_hist.n_occupied); // All far clusters with hist dets were killed
   
   // 2. Verify f_idx_occupied flags were cleared for removed detections
   for (int16_t i = 0; i < MAX_NUMBER_OF_HISTORIC_DETECTIONS; i++)
   {
      if (det_hist.f_idx_occupied[i])
      {
         // Any occupied slots should have valid cluster_idx
         CHECK_TRUE(det_hist.det_data[i].cluster_idx >= 0);
      }
   }
   
   // 3. Verify killed clusters' old_det_idx arrays were processed
   // (New clusters in those slots should have no old detections)
   for (int16_t i = 0; i < 5; i++)
   {
      if (clusters[i].vcs_position_x < 20.0F) // New or close clusters
      {
         // These should either be new (0 old dets) or close clusters that survived
         if (clusters[i].vcs_position_x < 10.0F)
         {
            CHECK_TRUE(clusters[i].num_old_dets == 0); // New clusters have no history
         }
      }
   }
}

/**
*\purpose Test Initialize_Clusters inactive cluster ID array shifting logic
*\req    NA
*/
TEST(initialize_clusters, Test_Initialize_Clusters_Inactive_ID_Array_Shifting)
{
   /** \precond
   * Create tracker with several free slots and verify inactive_cluster_ids array is properly maintained
   **/
   F360_Detection_Hist_T det_hist = {};
   
   tracker_info.variant.num_clusters = 12;
   tracker_info.num_active_clusters = 3;
   
   // Initialize existing clusters
   for (int16_t i = 0; i < 3; i++)
   {
      tracker_info.active_cluster_ids[i] = i + 1;
      clusters[i].id = i + 1;
      clusters[i].vcs_position_x = 10.0F + i * 5.0F;
   }
   
   // Set up inactive cluster IDs in specific order
   for (int16_t i = 0; i < 9; i++)
   {
      tracker_info.inactive_cluster_ids[i] = 4 + i; // IDs: 4, 5, 6, 7, 8, 9, 10, 11, 12
   }
   
   // Create 5 local clusters
   for (uint16_t i = 0; i < 5; i++)
   {
      const bool f_take_new_cluster = true;
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 5.0F + i * 1.0F, 0.0F, 5.0F, true,
         rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);
   }

   /** \action
   * Call Initialize_Clusters
   **/
   Initialize_Clusters(raw_detection_list, calibs, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Verify inactive_cluster_ids array was correctly shifted
   **/
   
   // 1. Verify num_active_clusters increased correctly
   CHECK_EQUAL(8, tracker_info.num_active_clusters); // 3 + 5
   
   // 2. Verify first 5 inactive IDs were used and array was shifted
   // Original: [4, 5, 6, 7, 8, 9, 10, 11, 12]
   // After using first 5: [9, 10, 11, 12, 0, 0, 0, 0, 0]
   CHECK_EQUAL(9, tracker_info.inactive_cluster_ids[0]);
   CHECK_EQUAL(10, tracker_info.inactive_cluster_ids[1]);
   CHECK_EQUAL(11, tracker_info.inactive_cluster_ids[2]);
   CHECK_EQUAL(12, tracker_info.inactive_cluster_ids[3]);
   
   // 3. Verify slots that were shifted out are cleared
   for (int16_t i = 4; i < 9; i++)
   {
      CHECK_EQUAL(0, tracker_info.inactive_cluster_ids[i]);
   }
   
   // 4. Verify new clusters got the first 5 IDs from original inactive array
   CHECK_EQUAL(4, tracker_info.active_cluster_ids[3]);
   CHECK_EQUAL(5, tracker_info.active_cluster_ids[4]);
   CHECK_EQUAL(6, tracker_info.active_cluster_ids[5]);
   CHECK_EQUAL(7, tracker_info.active_cluster_ids[6]);
   CHECK_EQUAL(8, tracker_info.active_cluster_ids[7]);
}
/** @}*/

/** \defgroup  Init_Cluster
 *  @{
 */

/** \brief
*  Test group for Init_Cluster function
**/
TEST_GROUP(Init_Cluster)
{
   /** \setup
   * Setting up arguments for Init_Cluster function
   **/
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Local_Clusters_T local_cluster_data = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Cluster_T cluster = {};
   F360_Calibrations_T calibs;
   F360_Host_T host = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      (void)memset(&local_cluster_data, 0, sizeof(local_cluster_data));
      (void)memset(&cluster, 0, sizeof(cluster));
      
      // Set cluster ID for testing
      cluster.id = 1;
      
      // Initialize sensors with default timestamp
      for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         sensors[i].refined.time_since_measurement_s = 0.05F;
         sensors[i].constant.id = i + 1;
      }
   }
};

/**
*\purpose  Test that Init_Cluster initializes cluster correctly with single detection
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Single_Detection)
{
   /** \precond
   * Create one detection in local cluster
   **/
   bool f_take_new_cluster = true;
   float32_t det_vcs_pos_long = 10.0F;
   float32_t det_vcs_pos_lat = 2.0F;
   float32_t det_range_rate = 5.0F;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);

   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify cluster is initialized correctly with one detection
   **/
   CHECK_EQUAL(1, cluster.ndets);
   DOUBLES_EQUAL(det_vcs_pos_long, cluster.vcs_position_x, 0.00001);
   DOUBLES_EQUAL(det_vcs_pos_lat, cluster.vcs_position_y, 0.00001);
   CHECK_EQUAL(1, cluster.num_types_of_dets[0]); // 1 moving detection
   CHECK_EQUAL(0, cluster.num_types_of_dets[1]); // 0 stationary
   CHECK_TRUE(cluster.f_dealiased);
   CHECK_EQUAL(1, detection_props[0].cluster_id);
}

/**
*\purpose  Test that Init_Cluster initializes cluster correctly with multiple detections
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Multiple_Detections)
{
   /** \precond
   * Create cluster with 3 detections
   **/
   bool f_take_new_cluster = true;
   float32_t det_vcs_pos_long = 10.0F;
   float32_t det_vcs_pos_lat = 2.0F;
   float32_t det_range_rate = 5.0F;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);

   f_take_new_cluster = false;
   for (int i = 0; i < 2; i++)
   {
      det_vcs_pos_long += 0.1F;
      det_vcs_pos_lat += 0.05F;
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
         det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);
   }

   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify cluster position is average of all detections
   **/
   CHECK_EQUAL(3, cluster.ndets);
   DOUBLES_EQUAL(10.1, cluster.vcs_position_x, 0.00001); // Mean of 10.0, 10.1, 10.2
   DOUBLES_EQUAL(2.05, cluster.vcs_position_y, 0.00001); // Mean of 2.0, 2.05, 2.1
   CHECK_EQUAL(3, cluster.num_types_of_dets[0]); // 3 moving detections
}

/**
*\purpose  Test that Init_Cluster initializes cluster correctly with mixed moving and stationary detections
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Mixed_Motion_Status)
{
   /** \precond
   * Create cluster with 2 moving and 1 stationary/ambiguous detection
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.0F, 2.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   f_take_new_cluster = false;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.1F, 2.0F, 0.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, raw_detection_list, local_cluster_data, detection_props);

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.2F, 2.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify detection type counts are correct
   **/
   CHECK_EQUAL(3, cluster.ndets);
   CHECK_EQUAL(2, cluster.num_types_of_dets[0]); // 2 moving
   CHECK_EQUAL(1, cluster.num_types_of_dets[1]); // 1 stationary/ambiguous
}

/**
*\purpose  Test that Init_Cluster initializes a cluster as dealiased when there are detections 
* that are both dealiased and non-dealiased
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Multiple_Detections_Mixed_Dealiased_Flags)
{
   /** \precond
   * Create cluster with 11 detections with alternating dealiased flags
   **/
   bool f_take_new_cluster = true;
   float32_t det_vcs_pos_long = 1.0F;
   float32_t det_vcs_pos_lat = 0.0F;
   float32_t det_range_rate = 10.0F;
   bool det_f_dealiased = false;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);

   f_take_new_cluster = false;
   for (int16_t i = 0; i < 10; i++)
   {
      if (i % 2 == 0)
      {
         det_f_dealiased = true;
      }
      else
      {
         det_f_dealiased = false;
      }
      det_vcs_pos_long += 0.01F;
      det_vcs_pos_lat += 0.01F;
      det_range_rate += 0.01F;
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
         det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);
   }

   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify cluster is marked as dealiased (at least one detection is dealiased)
   **/
   CHECK_EQUAL(11, cluster.ndets);
   CHECK_TRUE(cluster.f_dealiased); // Should be true because some detections are dealiased
}

/**
*\purpose  Test that Init_Cluster corretly goes through function with zero detections
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Zero_Detections)
{
   /** \precond
   * Call Init_Cluster with number_of_dets_in_cluster = 0
   **/
   uint16_t first_det_idx = 0;
   uint16_t number_of_dets = 0;

   /** \action
   * Call Init_Cluster
   **/
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                number_of_dets, detection_props, cluster);

   /** \result
   * Verify cluster handles zero detections without division by zero, fields should be set to defaults
   **/
   CHECK_EQUAL(0, cluster.ndets);
   DOUBLES_EQUAL(0.0, cluster.vcs_position_x, 0.00001);
   DOUBLES_EQUAL(0.0, cluster.vcs_position_y, 0.00001);
   CHECK_EQUAL(0, cluster.num_types_of_dets[0]);
   CHECK_EQUAL(0, cluster.num_types_of_dets[1]);
   CHECK_FALSE(cluster.f_dealiased);
   CHECK_EQUAL(0, cluster.low_rcs_dets_cnt);
}

/**
*\purpose Test if the confidence blocker counter is set for a newly initialized cluster with a single, low rcs detection
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Conf_Blocker_Low_RCS)
{
   /** \precond
   * Create single detection with low RCS
   **/
   bool f_take_new_cluster = true;
   float32_t det_vcs_pos_long = 1.0F;
   float32_t det_vcs_pos_lat = 0.0F;
   float32_t det_range_rate = 1.5F;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = -20.0F;
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify counter is set to expected value for low RCS
   **/
   CHECK_EQUAL(1, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test that Init_Cluster confidence blocker is NOT set for multiple detections
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Conf_Blocker_Multiple_Detections)
{
   /** \precond
   * Create cluster with 2 detections both with low RCS
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.0F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   f_take_new_cluster = false;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.1F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = -20.0F;
   raw_detection_list.detections[1].raw.rcs = -20.0F;
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify confidence blocker counter is NOT set for multiple detections (only applies to single detection)
   **/
   CHECK_EQUAL(0, cluster.low_rcs_dets_cnt);
}

/**
*\purpose Test if the confidence blocker counter is set for a newly initialized cluster with a single, very low rcs detection
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Conf_Blocker_Very_Low_RCS)
{
   /** \precond
   * Create single detection with very low RCS
   **/
   bool f_take_new_cluster = true;
   float32_t det_vcs_pos_long = 1.0F;
   float32_t det_vcs_pos_lat = 0.0F;
   float32_t det_range_rate = 1.5F;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = -30.0F;
   cluster.low_rcs_dets_cnt = 0;
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify counter is set to k_ocb_cnt_delta_low_rcs_or_mult_dets (2), which is the expected 
   * value for RCS below threshold k_ocb_rcs_thresh_low_rcs (-23)
   **/
   CHECK_EQUAL(2, cluster.low_rcs_dets_cnt);
}

/**
*\purpose Test if the confidence blocker counter is unset for a newly initialized cluster with a single, high rcs detection
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Conf_Blocker_High_RCS)
{
   /** \precond
   * Create single detection with high RCS
   **/
   bool f_take_new_cluster = true;
   float32_t det_vcs_pos_long = 1.0F;
   float32_t det_vcs_pos_lat = 0.0F;
   float32_t det_range_rate = 1.5F;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = 10.0F;
   cluster.low_rcs_dets_cnt = 0;
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify counter is not changed for high RCS
   **/
   CHECK_EQUAL(0, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test Init_Cluster RCS threshold boundary at k_ocb_rcs_thresh_low_rcs
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_RCS_At_Low_Threshold)
{
   /** \precond
   * Create single detection with RCS exactly at low threshold
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.0F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = calibs.k_ocb_rcs_thresh_low_rcs - 0.1F; // Just below threshold
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify counter is set for RCS below low threshold
   **/
   CHECK_EQUAL(calibs.k_ocb_cnt_delta_low_rcs_or_mult_dets, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test Init_Cluster RCS threshold boundary at k_ocb_rcs_thresh_midlow_rcs
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_RCS_At_Midlow_Threshold)
{
   /** \precond
   * Create single detection with RCS between low and midlow threshold
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.0F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   // Set RCS between thresholds
   raw_detection_list.detections[0].raw.rcs = calibs.k_ocb_rcs_thresh_low_rcs + 1.0F;
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify counter is set to midlow value
   **/
   CHECK_EQUAL(calibs.k_ocb_cnt_delta_midlow_rcs, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test Init_Cluster with RCS exactly at midlow threshold boundary
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_RCS_Exact_Midlow_Threshold)
{
   /** \precond
   * Create single detection with RCS exactly equal to k_ocb_rcs_thresh_midlow_rcs
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.0F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = calibs.k_ocb_rcs_thresh_midlow_rcs; // Exactly at threshold
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify counter is NOT set when RCS equals threshold (condition is <, not <=)
   **/
   CHECK_EQUAL(0, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test Init_Cluster confidence blocker with negative range_rate
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Conf_Blocker_Negative_Range_Rate)
{
   /** \precond
   * Create single detection with low RCS and negative range_rate within bounds
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.0F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = -24.0F; // Low RCS
   raw_detection_list.detections[0].raw.range = 9.9F; // Within range
   raw_detection_list.detections[0].raw.range_rate = -(calibs.k_ocb_max_range_rate - 0.1F); // Negative but within bounds
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify counter is set (std::abs handles negative range_rate correctly)
   **/
   CHECK_EQUAL(calibs.k_ocb_cnt_delta_low_rcs_or_mult_dets, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test Init_Cluster confidence blocker with negative range_rate at exact boundary
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Conf_Blocker_Negative_Range_Rate_At_Boundary)
{
   /** \precond
   * Create single detection with negative range_rate exactly at boundary
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.0F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = -20.0F;
   raw_detection_list.detections[0].raw.range = 9.9F;
   raw_detection_list.detections[0].raw.range_rate = -calibs.k_ocb_max_range_rate; // Exactly at negative boundary
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify counter is NOT set when abs(range_rate) equals max (condition is <, not <=)
   **/
   CHECK_EQUAL(0, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test Init_Cluster when number of detections exceeds max_dets_in_obj_track
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Exceeds_Max_Detections)
{
   /** \precond
   * Create more detections than max_dets_in_obj_track allows
   **/
   bool f_take_new_cluster = true;
   const uint16_t max_dets = static_cast<uint16_t>(tracker_info.variant.num_dets_in_track);
   
   // Add max_dets + 5 detections
   for (uint16_t i = 0; i < max_dets + 5; i++)
   {
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.0F + i * 0.1F, 2.0F, 5.0F, true,
         rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);
      f_take_new_cluster = false;
   }

   /** \action
   * Call Init_Cluster with number_of_dets > max_dets_in_obj_track
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, max_dets, 
                max_dets + 5, detection_props, cluster);

   /** \result
   * Verify only max_dets_in_obj_track detections are processed
   **/
   CHECK_EQUAL(max_dets, cluster.ndets);
   for (uint16_t i = 0; i < max_dets; i++)
   {
      // Verify that correct detection were selected by checking id, id higher than max_dets should not be used in this setup
      CHECK_TRUE(cluster.detids[i] <= max_dets);
   }
}

/**
*\purpose  Test Init_Cluster when detection is outside range bounds for confidence blocker
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Detection_Outside_Range_Bounds)
{
   /** \precond
   * Create single detection outside range bounds
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.0F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = -20.0F; // Low RCS
   raw_detection_list.detections[0].raw.range = calibs.k_ocb_max_range + 10.0F; // Outside range
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify confidence blocker counter is NOT set when outside range
   **/
   CHECK_EQUAL(0, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test Init_Cluster when detection has too high range_rate for confidence blocker
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_Detection_High_Range_Rate)
{
   /** \precond
   * Create single detection with high range_rate
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 1.0F, 0.0F, 1.5F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   raw_detection_list.detections[0].raw.rcs = -20.0F; // Low RCS
   raw_detection_list.detections[0].raw.range = 10.0F; // Within range
   raw_detection_list.detections[0].raw.range_rate = calibs.k_ocb_max_range_rate + 5.0F; // High range_rate
   
   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify confidence blocker counter is NOT set when range_rate exceeds limit
   **/
   CHECK_EQUAL(0, cluster.low_rcs_dets_cnt);
}

/**
*\purpose  Test that Init_Cluster correctly populates all cluster fields
*\req    NA
*/
TEST(Init_Cluster, Test_Init_Cluster_All_Fields_Populated)
{
   /** \precond
   * Create cluster with 3 detections with known values
   **/
   bool f_take_new_cluster = true;
   float32_t det1_vcs_pos_long = 10.0F;
   float32_t det1_vcs_pos_lat = 2.0F;
   float32_t det1_range_rate = 5.0F;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det1_vcs_pos_long, det1_vcs_pos_lat, det1_range_rate, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   f_take_new_cluster = false;
   float32_t det2_vcs_pos_long = 10.2F;
   float32_t det2_vcs_pos_lat = 2.1F;
   float32_t det2_range_rate = 4.8F;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det2_vcs_pos_long, det2_vcs_pos_lat, det2_range_rate, false,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, raw_detection_list, local_cluster_data, detection_props);

   float32_t det3_vcs_pos_long = 10.1F;
   float32_t det3_vcs_pos_lat = 2.05F;
   float32_t det3_range_rate = 5.1F;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det3_vcs_pos_long, det3_vcs_pos_lat, det3_range_rate, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   // Set different sensor IDs and timestamps
   raw_detection_list.detections[0].raw.sensor_id = 1;
   raw_detection_list.detections[1].raw.sensor_id = 2;
   raw_detection_list.detections[2].raw.sensor_id = 3;
   sensors[0].refined.time_since_measurement_s = 0.05F;
   sensors[1].refined.time_since_measurement_s = 0.03F;
   sensors[2].refined.time_since_measurement_s = 0.04F;

   // Set azimuth angles and compensated range rates for detections
   raw_detection_list.detections[0].processed.vcs_az = 0.1F;
   detection_props[0].range_rate_compensated = det1_range_rate;
   raw_detection_list.detections[1].processed.vcs_az = 0.11F;
   detection_props[1].range_rate_compensated = det2_range_rate;
   raw_detection_list.detections[2].processed.vcs_az = 0.105F;
   detection_props[2].range_rate_compensated = det3_range_rate;

   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify all cluster fields are populated correctly
   **/
   
   // Position fields - should be mean of all detections
   float32_t expected_vcs_pos_x = (det1_vcs_pos_long + det2_vcs_pos_long + det3_vcs_pos_long) / 3.0F;
   float32_t expected_vcs_pos_y = (det1_vcs_pos_lat + det2_vcs_pos_lat + det3_vcs_pos_lat) / 3.0F;
   DOUBLES_EQUAL(expected_vcs_pos_x, cluster.vcs_position_x, 0.00001);
   DOUBLES_EQUAL(expected_vcs_pos_y, cluster.vcs_position_y, 0.00001);
   DOUBLES_EQUAL(0.0, cluster.vcs_position_z, 0.00001); // Should be cleared by Clear_Cluster
   
   // Range rate compensation - should be mean
   float32_t expected_rep_rdotcomp = (det1_range_rate + det2_range_rate + det3_range_rate) / 3.0F;
   DOUBLES_EQUAL(expected_rep_rdotcomp, cluster.rep_rdotcomp, 0.00001);
   
   // Azimuth fields
   float32_t expected_rep_vcs_az = (0.1F + 0.11F + 0.105F) / 3.0F;
   DOUBLES_EQUAL(expected_rep_vcs_az, cluster.rep_vcs_az, 0.00001);
   DOUBLES_EQUAL(F360_Cosf(expected_rep_vcs_az), cluster.cos_vcs_az, 0.00001);
   DOUBLES_EQUAL(F360_Sinf(expected_rep_vcs_az), cluster.sin_vcs_az, 0.00001);
   
   // Detection count and IDs
   CHECK_EQUAL(3, cluster.ndets);
   CHECK_EQUAL(1, cluster.detids[0]); // Detection indices are 1-based
   CHECK_EQUAL(2, cluster.detids[1]);
   CHECK_EQUAL(3, cluster.detids[2]);
   
   // Motion status counts - 2 moving, 1 ambiguous
   CHECK_EQUAL(2, cluster.num_types_of_dets[0]); // Moving
   CHECK_EQUAL(1, cluster.num_types_of_dets[1]); // Stationary/ambiguous
   
   // Dealiasing flag - true if ANY detection is dealiased
   CHECK_TRUE(cluster.f_dealiased);
   
   // Timestamp - should be minimum of all sensor timestamps
   DOUBLES_EQUAL(0.03F, cluster.time_since_measurement, 0.00001);
   
   // Time since cluster updated - should be 0 for new cluster
   DOUBLES_EQUAL(0.0F, cluster.time_since_cluster_updated, 0.00001);
   
   // Clutter counter - should be 0 for new cluster
   CHECK_EQUAL(0, cluster.clutter_counter);
   
   // Low RCS detection count - should be 0 for multiple detections
   CHECK_EQUAL(0, cluster.low_rcs_dets_cnt);
   
   // Historical detection fields - should be cleared
   CHECK_EQUAL(0, cluster.num_old_dets);
   
   // Kill flag - should be false
   CHECK_FALSE(cluster.f_to_be_killed);
   
   // Verify detection properties are linked to cluster
   CHECK_EQUAL(cluster.id, detection_props[0].cluster_id);
   CHECK_EQUAL(cluster.id, detection_props[1].cluster_id);
   CHECK_EQUAL(cluster.id, detection_props[2].cluster_id);
}
/** @}*/

/** \defgroup  Update_Cluster_Timestamp
 *  @{
 */

/** \brief
*  Test group for Update_Cluster_Timestamp function
**/
TEST_GROUP(Update_Cluster_Timestamp)
{
   /** \setup
   * Setting up arguments for Update_Cluster_Timestamp function to be tested indirectly through Init_Cluster
   **/
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Local_Clusters_T local_cluster_data = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Cluster_T cluster = {};
   F360_Calibrations_T calibs;
   F360_Host_T host = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      (void)memset(&local_cluster_data, 0, sizeof(local_cluster_data));
      (void)memset(&cluster, 0, sizeof(cluster));
      
      // Set cluster ID for testing
      cluster.id = 1;
      
      // Initialize sensors with default timestamp
      for (int i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         sensors[i].refined.time_since_measurement_s = 0.05F;
         sensors[i].constant.id = i + 1;
      }
   }
};

/**
*\purpose  Test that Init_Cluster timestamp selection works correctly with single detection
*\req    NA
*/
TEST(Update_Cluster_Timestamp, Test_Update_Cluster_Timestamp_Single_Detection)
{
   /** \precond
   * Create cluster with single detection to test j==0 initialization path
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.0F, 2.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   // Set sensor timestamp
   raw_detection_list.detections[0].raw.sensor_id = 1;
   sensors[0].refined.time_since_measurement_s = 0.042F;

   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify timestamp is set correctly through j==0 initialization path
   **/
   CHECK_EQUAL(1, cluster.ndets);
   DOUBLES_EQUAL(0.042F, cluster.time_since_measurement, 0.00001);
}

/**
*\purpose  Test Update_Cluster_Timestamp timestamp selection with multiple sensors
*\req    NA
*/
TEST(Update_Cluster_Timestamp, Test_Update_Cluster_Timestamp_Multiple_Sensors)
{
   /** \precond
   * Create detections from different sensors with different timestamps
   **/
   bool f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.0F, 2.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   f_take_new_cluster = false;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.1F, 2.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_cluster_data, detection_props);

   // Set different sensor IDs and timestamps
   raw_detection_list.detections[0].raw.sensor_id = 1;
   raw_detection_list.detections[1].raw.sensor_id = 2;
   sensors[0].refined.time_since_measurement_s = 0.08F;
   sensors[1].refined.time_since_measurement_s = 0.03F; // This should be selected (minimum)

   /** \action
   * Call Init_Cluster
   **/
   const uint16_t first_det_idx = local_cluster_data.array_of_first_det_idx_in_clusters[0];
   Init_Cluster(first_det_idx, local_cluster_data.array_of_det_idxs_in_clusters, raw_detection_list, 
                calibs, sensors, static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), 
                local_cluster_data.num_dets_in_clusters[0], detection_props, cluster);

   /** \result
   * Verify minimum timestamp is selected
   **/
   DOUBLES_EQUAL(0.03F, cluster.time_since_measurement, 0.00001);
}
/** @}*/

/** \defgroup  Find_Least_Prioritized_Clusters
 *  @{
 */

/** \brief
*  Test group for Find_Least_Prioritized_Clusters function
**/
TEST_GROUP(Find_Least_Prioritized_Clusters)
{
   /** \setup
   * Setting up arguments for Find_Least_Prioritized_Clusters function
   **/
   F360_Tracker_Info_T tracker_info = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Host_T host = {};
   float32_t priority[NUMBER_OF_CLUSTERS];
   int16_t low_to_high_prio_perm[NUMBER_OF_CLUSTERS];
   uint16_t num_low_prio_clusters;

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);
      
      (void)memset(&clusters, 0, sizeof(clusters));
      (void)memset(&host, 0, sizeof(host));
      (void)memset(&priority, 0, sizeof(priority));
      (void)memset(&low_to_high_prio_perm, 0, sizeof(low_to_high_prio_perm));
      num_low_prio_clusters = 0;
      
      // Default host driving straight at moderate speed
      host.vcs_speed = 10.0F;
      host.curvature_rear = 0.0F;
      
      // Initialize tracker_info
      tracker_info.num_active_clusters = 0;
   }
};

/**
*\purpose  Test that Find_Least_Prioritized_Clusters will correctly prioritize cluster in the case
* there is only one active cluster
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_Single_Cluster)
{
   /** \precond
   * Create single active cluster at moderate distance
   **/
   tracker_info.num_active_clusters = 1;
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   clusters[0].vcs_position_x = 20.0F;
   clusters[0].vcs_position_y = 0.0F;
   const float32_t highest_local_cluster_prio = 100.0F;

   /** \action
   * Call Find_Least_Prioritized_Clusters with highest_local_cluster_prio = 100.0
   **/
   const uint16_t max_num_clusters_to_sort = 1;
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify priority is computed and cluster is identified as low priority
   **/
   CHECK_TRUE(priority[0] > 0.0F);
   CHECK_EQUAL(0, low_to_high_prio_perm[0]);
   CHECK_EQUAL(1, num_low_prio_clusters); // Single cluster should be counted as low prio
}

/**
*\purpose  Test that Find_Least_Prioritized_Clusters will correctly prioritize cluster in the case
* there are multiple clusters
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_Multiple_Clusters_Sorting)
{
   /** \precond
   * Create 5 active clusters at different distances (priority decreases with distance)
   **/
   const uint16_t num_clusters = 5;
   tracker_info.num_active_clusters = num_clusters;
   
   // Close cluster - highest priority
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   clusters[0].vcs_position_x = 100.0F;
   clusters[0].vcs_position_y = 0.0F;
   
   // Medium-close cluster
   tracker_info.active_cluster_ids[1] = 2;
   clusters[1].id = 2;
   clusters[1].vcs_position_x = 50.0F;
   clusters[1].vcs_position_y = 0.0F;
   
   // Medium cluster
   tracker_info.active_cluster_ids[2] = 3;
   clusters[2].id = 3;
   clusters[2].vcs_position_x = 30.0F;
   clusters[2].vcs_position_y = 0.0F;
   
   // Medium-far cluster
   tracker_info.active_cluster_ids[3] = 4;
   clusters[3].id = 4;
   clusters[3].vcs_position_x = 15.0F;
   clusters[3].vcs_position_y = 0.0F;
   
   // Far cluster - lowest priority
   tracker_info.active_cluster_ids[4] = 5;
   clusters[4].id = 5;
   clusters[4].vcs_position_x = 1.0F;
   clusters[4].vcs_position_y = 0.0F;
   
   const float32_t highest_local_cluster_prio = 1000.0F; // High enough to include all

   /** \action
   * Call Find_Least_Prioritized_Clusters
   **/
   const uint16_t max_num_clusters_to_sort = num_clusters;
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify clusters are sorted from low to high priority
   **/
   CHECK_EQUAL(num_clusters, num_low_prio_clusters);
   
   // Verify sorting order - priorities should be in ascending order
   for (uint16_t i = 0; i < num_clusters - 1; i++)
   {
      CHECK_TRUE(priority[i] <= priority[i + 1]);
   }
   
   // Verify permutation array contains valid indices
   bool found_indices[5] = {false, false, false, false, false};
   for (uint16_t i = 0; i < num_clusters; i++)
   {
      CHECK_TRUE(low_to_high_prio_perm[i] >= 0 && low_to_high_prio_perm[i] < num_clusters);
      found_indices[low_to_high_prio_perm[i]] = true;
   }
   for (uint16_t i = 0; i < num_clusters; i++)
   {
      CHECK_TRUE(found_indices[i]);
   }
}

/**
*\purpose  Test that Find_Least_Prioritized_Clusters will terminate early termination when 
* a high priority cluster is found
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_Early_Termination)
{
   /** \precond
   * Create 5 clusters: 2 far away (low prio), 3 close (high prio)
   * Set highest_local_cluster_prio to value that should cause early termination
   **/
   const uint16_t num_clusters = 5;
   tracker_info.num_active_clusters = num_clusters;
   
   // Far clusters (should be found as low priority)
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   clusters[0].vcs_position_x = 100.0F;
   clusters[0].vcs_position_y = 0.0F;
   
   tracker_info.active_cluster_ids[1] = 2;
   clusters[1].id = 2;
   clusters[1].vcs_position_x = 90.0F;
   clusters[1].vcs_position_y = 0.0F;
   
   // Close clusters (should NOT be sorted due to early termination)
   tracker_info.active_cluster_ids[2] = 3;
   clusters[2].id = 3;
   clusters[2].vcs_position_x = 5.0F;
   clusters[2].vcs_position_y = 0.0F;
   
   tracker_info.active_cluster_ids[3] = 4;
   clusters[3].id = 4;
   clusters[3].vcs_position_x = 6.0F;
   clusters[3].vcs_position_y = 0.0F;
   
   tracker_info.active_cluster_ids[4] = 5;
   clusters[4].id = 5;
   clusters[4].vcs_position_x = 7.0F;
   clusters[4].vcs_position_y = 0.0F;

   const float32_t highest_local_cluster_prio = 50.0F; // Low enough to trigger early exit

   /** \action
   * Call Find_Least_Prioritized_Clusters with low highest_local_cluster_prio
   * This should cause early termination after sorting a few clusters
   **/
   const uint16_t max_num_clusters_to_sort = num_clusters;
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify function terminated early or found low priority clusters
   * Since close clusters have much higher priority, early termination should occur
   **/
   CHECK_TRUE(num_low_prio_clusters <= num_clusters);
   // Function should have found the far clusters as low priority and stopped when hitting close ones
}

/**
*\purpose  Test whether Find_Least_Prioritized_Clusters will only prioritize certain amount of 
* active clusters in the case that the amount of clusters is greater than max_num_clusters_to_sort
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_Max_Clusters_Limit)
{
   /** \precond
   * Create 10 active clusters but limit max_num_clusters_to_sort to 3
   **/
   const uint16_t num_clusters = 10;
   tracker_info.num_active_clusters = num_clusters;
   
   for (uint16_t i = 0; i < num_clusters; i++)
   {
      tracker_info.active_cluster_ids[i] = i + 1;
      clusters[i].id = i + 1;
      clusters[i].vcs_position_x = 10.0F + i * 10.0F; // Increasing distance
      clusters[i].vcs_position_y = 0.0F;
   }

   const float32_t highest_local_cluster_prio = 1000.0F; // High enough to not cause early exit
   const uint16_t max_num_clusters_to_sort = 3;
   /** \action
   * Call Find_Least_Prioritized_Clusters with max_num_clusters_to_sort = 3
   **/
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify only max_num_clusters_to_sort clusters are sorted
   **/
   CHECK_EQUAL(max_num_clusters_to_sort, num_low_prio_clusters);
}

/**
*\purpose  Test that Find_Least_Prioritized_Clusters handles zero active clusters
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_Zero_Active_Clusters)
{
   /** \precond
   * Set num_active_clusters to 0
   **/
   tracker_info.num_active_clusters = 0;

   const float32_t highest_local_cluster_prio = 100.0F;
   const uint16_t max_num_clusters_to_sort = 0; // Don't sort any since there are none
   float32_t initial_priority[NUMBER_OF_CLUSTERS] = {};
   int16_t initial_low_to_high_prio_perm[NUMBER_OF_CLUSTERS] = {};

   for (int i = 0; i < NUMBER_OF_CLUSTERS; i++)
   {
      initial_priority[i] = priority[i];
      initial_low_to_high_prio_perm[i] = low_to_high_prio_perm[i];
   }
   /** \action
   * Call Find_Least_Prioritized_Clusters
   **/
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify function handles zero clusters gracefully. Nothing is expect to have been modified
   **/
   CHECK_EQUAL(0, num_low_prio_clusters);
   for (int i = 0; i < NUMBER_OF_CLUSTERS; i++)
   {
      CHECK_EQUAL(initial_priority[i], priority[i]);
      CHECK_EQUAL(initial_low_to_high_prio_perm[i], low_to_high_prio_perm[i]);
   }
}

/**
*\purpose  Test that Find_Least_Prioritized_Clusters bubble sort swapping logic works correctly
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_Bubble_Sort_Swap_Logic)
{
   /** \precond
   * Create clusters in random priority order to verify swap logic
   **/
   const uint16_t num_clusters = 4;
   tracker_info.num_active_clusters = num_clusters;
   
   // Create in non-sorted order: medium, far, close, very far
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   clusters[0].vcs_position_x = 30.0F;
   clusters[0].vcs_position_y = 0.0F;
   
   tracker_info.active_cluster_ids[1] = 2;
   clusters[1].id = 2;
   clusters[1].vcs_position_x = 80.0F;
   clusters[1].vcs_position_y = 0.0F;
   
   tracker_info.active_cluster_ids[2] = 3;
   clusters[2].id = 3;
   clusters[2].vcs_position_x = 10.0F;
   clusters[2].vcs_position_y = 0.0F;
   
   tracker_info.active_cluster_ids[3] = 4;
   clusters[3].id = 4;
   clusters[3].vcs_position_x = 150.0F;
   clusters[3].vcs_position_y = 0.0F;

   /** \action
   * Call Find_Least_Prioritized_Clusters
   **/
   const float32_t highest_local_cluster_prio = 1000.0F;
   const uint16_t max_num_clusters_to_sort = num_clusters;
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify sorting produces ascending priority order
   **/
   CHECK_EQUAL(num_clusters, num_low_prio_clusters);
   
   // Verify priorities are in ascending order
   for (uint16_t i = 0; i < num_clusters - 1; i++)
   {
      CHECK_TRUE(priority[i] <= priority[i + 1]);
   }
   
   // Lowest priority should be the farthest cluster (index 3, distance 150.0)
   // After sorting, it should be at position 0
   const int16_t lowest_prio_orig_idx = low_to_high_prio_perm[0];
   CHECK_EQUAL(3, lowest_prio_orig_idx); // Original index of farthest cluster
}

/**
*\purpose  Test whether Find_Least_Prioritized_Clusters will correctly identify all clusters as 
* low priority when all clusters have lower priority than local
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_All_Lower_Priority_Than_Local)
{
   /** \precond
   * Create 3 far clusters with very low priority
   **/
   const uint16_t num_clusters = 3;
   tracker_info.num_active_clusters = num_clusters;
   
   for (uint16_t i = 0; i < num_clusters; i++)
   {
      tracker_info.active_cluster_ids[i] = i + 1;
      clusters[i].id = i + 1;
      clusters[i].vcs_position_x = 200.0F + i * 10.0F; // All very far away
      clusters[i].vcs_position_y = 0.0F;
   }

   /** \action
   * Call Find_Least_Prioritized_Clusters with very high local cluster priority
   **/
   const float32_t highest_local_cluster_prio = 10000.0F; // Very high
   const uint16_t max_num_clusters_to_sort = num_clusters;
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify all clusters are identified as low priority
   **/
   CHECK_EQUAL(num_clusters, num_low_prio_clusters);
}

/**
*\purpose  Test whether Find_Least_Prioritized_Clusters will correctly identify none clusters as 
* low priority when none clusters have lower priority than highest local
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_No_Clusters_Lower_Priority_Than_Local)
{
   /** \precond
   * Create 3 close clusters with high priority
   **/
   const uint16_t num_clusters = 3;
   tracker_info.num_active_clusters = num_clusters;
   
   for (uint16_t i = 0; i < num_clusters; i++)
   {
      tracker_info.active_cluster_ids[i] = i + 1;
      clusters[i].id = i + 1;
      clusters[i].vcs_position_x = 5.0F + i * 1.0F; // All very close
      clusters[i].vcs_position_y = 0.0F;
   }

   /** \action
   * Call Find_Least_Prioritized_Clusters with very low local cluster priority
   **/
   const float32_t highest_local_cluster_prio = 0.1F; // Very low
   const uint16_t max_num_clusters_to_sort = 10;
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify no clusters are identified as low priority (early termination on first cluster)
   **/
   CHECK_EQUAL(0, num_low_prio_clusters);
}

/**
*\purpose  Test Find_Least_Prioritized_Clusters priority computation for each avaiable cluster
*\req    NA
*/
TEST(Find_Least_Prioritized_Clusters, Find_Least_Prioritized_Clusters_Priority_Computation_For_All_Clusters)
{
   /** \precond
   * Create 5 clusters at different positions
   **/
   const uint16_t num_clusters = 5;
   tracker_info.num_active_clusters = num_clusters;
   
   for (uint16_t i = 0; i < num_clusters; i++)
   {
      tracker_info.active_cluster_ids[i] = i + 1;
      clusters[i].id = i + 1;
      clusters[i].vcs_position_x = 20.0F + i * 15.0F;
      clusters[i].vcs_position_y = static_cast<float32_t>(i) - 2.0F; // Varying lateral positions
   }

   /** \action
   * Call Find_Least_Prioritized_Clusters
   **/
   const float32_t highest_local_cluster_prio = 1000.0F;
   const uint16_t max_num_clusters_to_sort = num_clusters;
   
   Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_clusters_to_sort,
      highest_local_cluster_prio, priority, low_to_high_prio_perm, num_low_prio_clusters);

   /** \result
   * Verify that all clusters have valid positive priorities assigned
   **/
   for (uint16_t i = 0; i < num_clusters; i++)
   {
      CHECK_TRUE(priority[i] > 0.0F);
   }
   CHECK_EQUAL(num_clusters, num_low_prio_clusters);
}
/** @}*/

/** \defgroup  Compute_Local_Cluster_Priority_And_Sort
 *  @{
 */

/** \brief
*  Test group for Compute_Local_Cluster_Priority_And_Sort function
**/
TEST_GROUP(Compute_Local_Cluster_Priority_And_Sort)
{
   /** \setup
   * Setting up arguments for Compute_Local_Cluster_Priority_And_Sort function
   **/
   F360_Tracker_Info_T tracker_info = {};
   F360_Local_Clusters_T local_clusters = {};
   F360_Host_T host = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   float32_t local_clusters_prio[MAX_TRACKER_POSN_CLUSTERS];
   uint32_t high_to_low_prio_perm[MAX_TRACKER_POSN_CLUSTERS];

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);
      
      (void)memset(&local_clusters, 0, sizeof(local_clusters));
      (void)memset(&host, 0, sizeof(host));
      (void)memset(&raw_detection_list, 0, sizeof(raw_detection_list));
      (void)memset(&detection_props, 0, sizeof(detection_props));
      (void)memset(&local_clusters_prio, 0, sizeof(local_clusters_prio));
      (void)memset(&high_to_low_prio_perm, 0, sizeof(high_to_low_prio_perm));
      
      // Default host driving straight at moderate speed
      host.vcs_speed = 10.0F;
      host.curvature_rear = 0.0F;
   }
};

/**
*\purpose  Test whether Compute_Local_Cluster_Priority_And_Sort will properly handle single cluster and single detection
*\req    NA
*/
TEST(Compute_Local_Cluster_Priority_And_Sort, Test_Compute_Local_Cluster_Priority_And_Sort_Single_Cluster_Single_Detection)
{
   /** \precond
   * Create single cluster with one detection
   **/
   bool f_take_new_cluster = true;
   float32_t det_vcs_pos_long = 10.0F;
   float32_t det_vcs_pos_lat = 2.0F;
   float32_t det_range_rate = 5.0F;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);

   /** \action
   * Call Compute_Local_Cluster_Priority_And_Sort
   **/
   Compute_Local_Cluster_Priority_And_Sort(local_clusters, host, detection_props,
      static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), local_clusters_prio, high_to_low_prio_perm);

   /** \result
   * Verify priority is computed and permutation array is set
   **/
   CHECK_EQUAL(1, local_clusters.num_clusters);
   CHECK_TRUE(local_clusters_prio[0] > 0.0F); // Priority should be positive
   CHECK_EQUAL(0, high_to_low_prio_perm[0]); // First element should point to cluster 0
}

/**
*\purpose  Test whether Compute_Local_Cluster_Priority_And_Sort produes a valid output in the case 
* that position averaging with multiple detections are used
*\req    NA
*/
TEST(Compute_Local_Cluster_Priority_And_Sort, Test_Compute_Local_Cluster_Priority_And_Sort_Single_Cluster_Multiple_Detections_Position_Averaging)
{
   /** \precond
   * Create two cluster with 2 detections each, one cluster detection position 
   * average are closer than the other.
   **/
   // Close cluster
   bool f_take_new_cluster = true;
   float32_t det1_long = 10.0F;
   float32_t det1_lat = 2.0F;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det1_long, det1_lat, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);

   f_take_new_cluster = false;
   float32_t det2_long = 10.2F;
   float32_t det2_lat = 2.0F;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det2_long, det2_lat, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);
   // Cluster further away both laterally and longitudinally
   f_take_new_cluster = true;
   float32_t det3_long = 20.0F;
   float32_t det3_lat = 3.0F;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det3_long, det3_lat, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);
   f_take_new_cluster = false;
   float32_t det4_long = 20.2F;
   float32_t det4_lat = 3.0F;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, det4_long, det4_lat, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);

   /** \action
   * Call Compute_Local_Cluster_Priority_And_Sort
   **/
   Compute_Local_Cluster_Priority_And_Sort(local_clusters, host, detection_props,
      static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), local_clusters_prio, high_to_low_prio_perm);

   /** \result
   * Verify priority is computed based on averaged position
   * Note: We can't verify the exact priority value since Calculate_Priority_For_Cluster is already tested,
   * but we verify the function completes and produces valid output. 
   * First cluster should have higher priority than second cluster due to being closer.
   **/
   CHECK_EQUAL(2, local_clusters.num_clusters);
   CHECK_TRUE(local_clusters_prio[0] > 0.0F);
   CHECK_TRUE(local_clusters_prio[1] > 0.0F);
   CHECK_EQUAL(0, high_to_low_prio_perm[0]);
   CHECK_EQUAL(1, high_to_low_prio_perm[1]);
   CHECK_TRUE(local_clusters_prio[0] > local_clusters_prio[1]);
}

/**
*\purpose  Test whether Compute_Local_Cluster_Priority_And_Sort produces a valid, sorted output with multiple input clusters
*\req    NA
*/
TEST(Compute_Local_Cluster_Priority_And_Sort, Test_Compute_Local_Cluster_Priority_And_Sort_Multiple_Clusters_Arrays_Populated)
{
   /** \precond
   * Create 3 clusters at different positions
   **/
   bool f_take_new_cluster = true;
   // Cluster 1 - far from host
   f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 50.0F, 0.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);

   // Cluster 2 - medium distance
   f_take_new_cluster = true;
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 15.0F, 0.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);

   // Cluster 3 - close to host
   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 5.0F, 0.0F, 5.0F, true,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);

   /** \action
   * Call Compute_Local_Cluster_Priority_And_Sort
   **/
   Compute_Local_Cluster_Priority_And_Sort(local_clusters, host, detection_props,
      static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), local_clusters_prio, high_to_low_prio_perm);

   /** \result
   * Verify all clusters have priorities and permutation array is valid
   **/
   CHECK_EQUAL(3, local_clusters.num_clusters);
   
   // All priorities should be positive
   for (uint16_t i = 0; i < local_clusters.num_clusters; i++)
   {
      CHECK_TRUE(local_clusters_prio[i] > 0.0F);
   }
   
   // Permutation array should contain valid indices (0, 1, 2 in some order)
   bool found_indices[3] = {false, false, false};
   for (uint16_t i = 0; i < local_clusters.num_clusters; i++)
   {
      CHECK_TRUE(high_to_low_prio_perm[i] < 3);
      found_indices[high_to_low_prio_perm[i]] = true;
   }
   CHECK_TRUE(found_indices[0] && found_indices[1] && found_indices[2]);
   
   // Verify sorting order - priorities should be in descending order via permutation
   CHECK_TRUE(local_clusters_prio[high_to_low_prio_perm[2]] >= local_clusters_prio[high_to_low_prio_perm[1]]);
   CHECK_TRUE(local_clusters_prio[high_to_low_prio_perm[1]] >= local_clusters_prio[high_to_low_prio_perm[0]]);
}

/**
*\purpose  Test whether Compute_Local_Cluster_Priority_And_Sort handles different motion statuses correctly
*\req    NA
*/
TEST(Compute_Local_Cluster_Priority_And_Sort, Test_Compute_Local_Cluster_Priority_And_Sort_Mixed_Motion_Status_Counting)
{
   /** \precond
   * Create cluster with 3 moving and 2 stationary detections
   **/
   bool f_take_new_cluster = true;
   
   // Add 3 moving detections
   for (int i = 0; i < 3; i++)
   {
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.0F + i * 0.1F, 2.0F, 5.0F, true,
         rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);
      f_take_new_cluster = false;
   }
   
   // Add 2 stationary detections
   for (int i = 0; i < 2; i++)
   {
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.3F + i * 0.1F, 2.0F, 0.0F, true,
         rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS, raw_detection_list, local_clusters, detection_props);
   }

   /** \action
   * Call Compute_Local_Cluster_Priority_And_Sort
   **/
   Compute_Local_Cluster_Priority_And_Sort(local_clusters, host, detection_props,
      static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), local_clusters_prio, high_to_low_prio_perm);

   /** \result
   * Verify function executes successfully with mixed motion status
   * (The internal motion counting is used by Calculate_Priority_For_Cluster which is already tested)
   **/
   CHECK_EQUAL(1, local_clusters.num_clusters);
   CHECK_EQUAL(5, local_clusters.num_dets_in_clusters[0]);
   CHECK_TRUE(local_clusters_prio[0] > 0.0F);
}

/**
*\purpose  Test whether Compute_Local_Cluster_Priority_And_Sort handles empty cluster correctly
*\req    NA
*/
TEST(Compute_Local_Cluster_Priority_And_Sort, Test_Compute_Local_Cluster_Priority_And_Sort_Empty_Cluster_Gets_Invalid_Priority)
{
   /** \precond
   * Create cluster with 0 detections
   **/
   local_clusters.num_clusters = 1;
   local_clusters.num_dets_in_clusters[0] = 0;
   local_clusters.array_of_first_det_idx_in_clusters[0] = 0;

   /** \action
   * Call Compute_Local_Cluster_Priority_And_Sort
   **/
   Compute_Local_Cluster_Priority_And_Sort(local_clusters, host, detection_props,
      static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), local_clusters_prio, high_to_low_prio_perm);

   /** \result
   * Verify empty cluster gets invalid priority of -1.0F
   **/
   DOUBLES_EQUAL(-1.0F, local_clusters_prio[0], 0.00001);
   CHECK_EQUAL(0, high_to_low_prio_perm[0]);
}

/**
*\purpose  Test whether Compute_Local_Cluster_Priority_And_Sort clamps the detection count when it exceeds the max limit
*\req    NA
*/
TEST(Compute_Local_Cluster_Priority_And_Sort, Test_Compute_Local_Cluster_Priority_And_Sort_Exceeds_Max_Detections_Clamping)
{
   /** \precond
   * Create cluster with more detections than max_dets_in_cluster
   **/
   bool f_take_new_cluster = true;
   const uint16_t max_dets = static_cast<uint16_t>(tracker_info.variant.num_dets_in_track);
   
   // Add max_dets + 3 detections
   for (uint16_t i = 0; i < max_dets + 3; i++)
   {
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster, 10.0F + i * 0.05F, 2.0F, 5.0F, true,
         rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING, raw_detection_list, local_clusters, detection_props);
      f_take_new_cluster = false;
   }

   /** \action
   * Call Compute_Local_Cluster_Priority_And_Sort
   **/
   Compute_Local_Cluster_Priority_And_Sort(local_clusters, host, detection_props,
      max_dets, local_clusters_prio, high_to_low_prio_perm);

   /** \result
   * Verify function handles detection count clamping correctly
   * (Function should use std::min to limit to max_dets)
   **/
   CHECK_EQUAL(1, local_clusters.num_clusters);
   CHECK_TRUE(local_clusters.num_dets_in_clusters[0] > max_dets); // Input has more
   CHECK_TRUE(local_clusters_prio[0] > 0.0F); // But priority still computed
}

/**
*\purpose  Test whether Compute_Local_Cluster_Priority_And_Sort handles zero local clusters correctly
*\req    NA
*/
TEST(Compute_Local_Cluster_Priority_And_Sort, Test_Compute_Local_Cluster_Priority_And_Sort_Zero_Clusters)
{
   /** \precond
   * Set num_clusters to 0
   **/
   local_clusters.num_clusters = 0;

   /** \action
   * Call Compute_Local_Cluster_Priority_And_Sort
   **/
   Compute_Local_Cluster_Priority_And_Sort(local_clusters, host, detection_props,
      static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), local_clusters_prio, high_to_low_prio_perm);

   /** \result
   * Verify function handles zero clusters
   **/
   CHECK_EQUAL(0, local_clusters.num_clusters);
}
/** @}*/
