/** \file
 * This file contains unit tests for content of f360_kill_cluster.cpp file
 */

#include "f360_kill_cluster.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>
#include <algorithm>
#include <cstring>

using namespace f360_variant_A;

/** \defgroup  f360_kill_cluster
 *  @{
 **/

/** \brief
*  This group includes testing of the functionality of kill_cluster.
**/
TEST_GROUP(f360_kill_cluster)
{
   /** \setup
   * A cluster is set up with two old detections, detection history is initialized,
   * and tracker info is configured with two active clusters.
   **/
   F360_Cluster_T cluster;
   F360_Detection_Hist_T det_hist;
   F360_Tracker_Info_T tracker_info;

   TEST_SETUP()
   {
      // Setup cluster with two old detections
      cluster.id = 3;
      cluster.num_old_dets = 2;
      cluster.old_det_idx[0] = 0;
      cluster.old_det_idx[1] = 1;
      cluster.ndets = 2;

      // Setup detection history with two occupied detections
      det_hist.n_occupied = 2;
      det_hist.f_idx_occupied[0] = true;
      det_hist.f_idx_occupied[1] = true;
      det_hist.det_data[0] = {};
      det_hist.det_data[1] = {};
      det_hist.det_data[0].cluster_idx = 3; // Setting this to match cluster id and to check if det hist is cleared properly
      det_hist.det_data[1].cluster_idx = 3; // Setting this to match cluster id and to check if det hist is cleared properly

      // Setup tracker info with two active clusters and two inactive clusters
      tracker_info.num_active_clusters = 2;
      tracker_info.variant.num_clusters = 4;
      tracker_info.active_cluster_ids[0] = 1;
      tracker_info.active_cluster_ids[1] = 3;
      tracker_info.inactive_cluster_ids[0] = 2;
      tracker_info.inactive_cluster_ids[1] = 4;
   }
};

/**
*\purpose  This test checks that when a cluster is killed, it is removed from active clusters,
*\         added to inactive clusters, its associated detections are cleared, and the cluster is reset.
*\req      NA
*/
TEST(f360_kill_cluster, Kill_Active_Cluster_With_Old_Detections)
{
   /** \precond
   A cluster with two old detections is set up and is active.
   **/

   /** \action
   * Call kill_cluster() on the active cluster.
   **/
   kill_cluster(cluster, det_hist, tracker_info);

   /** \result
   * Check that the cluster is removed from active clusters, added to inactive clusters,
   * detection history is cleared, and the cluster is reset.
   **/
   CHECK_EQUAL_TEXT(1, tracker_info.num_active_clusters, "Number of active clusters should decrease by 1")
   CHECK_EQUAL_TEXT(3, tracker_info.inactive_cluster_ids[2], "Cluster id should be added to inactive clusters")
   CHECK_EQUAL_TEXT(0, det_hist.n_occupied, "Detection history occupation should be decreased")
   CHECK_FALSE_TEXT(det_hist.f_idx_occupied[0], "Detection history occupied flag should be false for killed cluster")
   CHECK_FALSE_TEXT(det_hist.f_idx_occupied[1], "Detection history occupied flag should be false for killed cluster")
   CHECK_EQUAL_TEXT(0, cluster.ndets, "Cluster should be reset (ndets = 0)")
   CHECK_EQUAL_TEXT(0, cluster.num_old_dets, "Cluster should be reset (num_old_dets = 0)")
   CHECK_EQUAL_TEXT(0, det_hist.det_data[0].cluster_idx, "det_data[0] should be cleared to default (cluster_idx = 0)")
   CHECK_EQUAL_TEXT(0, det_hist.det_data[1].cluster_idx, "det_data[1] should be cleared to default (cluster_idx = 0)")
}

/**
*\purpose  This test checks that kill_cluster does not kill a cluster if num_active_clusters == 0.
*\req      NA
*/
TEST(f360_kill_cluster, Cluster_Not_Killed_When_No_Active_Clusters)
{
   /** \precond
   Tracker info is set up with zero active clusters.
   **/
   cluster.id = 3;
   tracker_info.num_active_clusters = 0; // num_active_clusters == 0

   // Save original state for later comparison
   int16_t orig_ndets = cluster.ndets;
   int16_t orig_num_old_dets = cluster.num_old_dets;

   /** \action
   * Call kill_cluster() when there are no active clusters.
   **/
   kill_cluster(cluster, det_hist, tracker_info);

   /** \result
   * Check that the number of active clusters and cluster properties remain unchanged.
   **/
   CHECK_EQUAL_TEXT(0, tracker_info.num_active_clusters, "Number of active clusters should remain unchanged")
   CHECK_EQUAL_TEXT(orig_ndets, cluster.ndets, "Cluster ndets should remain unchanged")
   CHECK_EQUAL_TEXT(orig_num_old_dets, cluster.num_old_dets, "Cluster num_old_dets should remain unchanged")
}

/**
*\purpose  This test checks that kill_cluster does not kill a cluster if cluster.id == 0.
*\req      NA
*/
TEST(f360_kill_cluster, Cluster_Not_Killed_When_Id_Zero)
{
   /** \precond
   Cluster id is set to zero.
   **/
   cluster.id = 0;

   // Save original state for later comparison
   int16_t orig_ndets = cluster.ndets;
   int16_t orig_num_old_dets = cluster.num_old_dets;

   /** \action
   * Call kill_cluster() with cluster id zero.
   **/
   kill_cluster(cluster, det_hist, tracker_info);

   /** \result
   * Check that the number of active clusters and cluster properties remain unchanged.
   **/
   CHECK_EQUAL_TEXT(2, tracker_info.num_active_clusters, "Number of active clusters should remain unchanged")
   CHECK_EQUAL_TEXT(orig_ndets, cluster.ndets, "Cluster ndets should remain unchanged")
   CHECK_EQUAL_TEXT(orig_num_old_dets, cluster.num_old_dets, "Cluster num_old_dets should remain unchanged")
}

/**
*\purpose  This test checks that kill_cluster does not kill a cluster if cluster.id > num_clusters.
*\req      NA
*/
TEST(f360_kill_cluster, Cluster_Not_Killed_When_Id_Exceeds_NumClusters)
{
   /** \precond
   Cluster id is set greater than the number of clusters in tracker info.
   **/
   cluster.id = 10; // cluster.id > tracker_info.variant.num_clusters
   tracker_info.variant.num_clusters = 4;

   // Save original state for later comparison
   int16_t orig_ndets = cluster.ndets;
   int16_t orig_num_old_dets = cluster.num_old_dets;

   /** \action
   * Call kill_cluster() with cluster id exceeding num_clusters.
   **/
   kill_cluster(cluster, det_hist, tracker_info);

   /** \result
   * Check that the number of active clusters and cluster properties remain unchanged.
   **/
   CHECK_EQUAL_TEXT(2, tracker_info.num_active_clusters, "Number of active clusters should remain unchanged")
   CHECK_EQUAL_TEXT(orig_ndets, cluster.ndets, "Cluster ndets should remain unchanged")
   CHECK_EQUAL_TEXT(orig_num_old_dets, cluster.num_old_dets, "Cluster num_old_dets should remain unchanged")
}

/**
*\purpose  This test checks that kill_cluster does not kill a cluster if cluster.id is not found among active clusters.
*\req      NA
*/
TEST(f360_kill_cluster, Cluster_Not_Killed_When_Id_Not_Active)
{
   /** \precond
   Cluster id is not present in the list of active clusters.
   **/
   cluster.id = 8; // Not present in active_cluster_ids
   tracker_info.active_cluster_ids[0] = 1;
   tracker_info.active_cluster_ids[1] = 3;
   tracker_info.num_active_clusters = 2;
   tracker_info.variant.num_clusters = 10;

   // Save original state for later comparison
   int16_t orig_ndets = cluster.ndets;
   int16_t orig_num_old_dets = cluster.num_old_dets;

   /** \action
   * Call kill_cluster() with a cluster id not in active clusters.
   **/
   kill_cluster(cluster, det_hist, tracker_info);

   /** \result
   * Check that the number of active clusters and cluster properties remain unchanged.
   **/
   CHECK_EQUAL_TEXT(2, tracker_info.num_active_clusters, "Number of active clusters should remain unchanged")
   CHECK_EQUAL_TEXT(orig_ndets, cluster.ndets, "Cluster ndets should remain unchanged")
   CHECK_EQUAL_TEXT(orig_num_old_dets, cluster.num_old_dets, "Cluster num_old_dets should remain unchanged")
}

/** @} */
