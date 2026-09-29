/*===================================================================================*\
* FILE: f360_initialize_clusters.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* The file contains the definition of Initialize_Clusters function
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "f360_initialize_clusters.h"

#include "f360_math.h"

#include "f360_clear_cluster.h"
#include "f360_calculate_priority.h"
#include "f360_dbscan.h"
#include "f360_get_wall_time.h"
#include "f360_kill_cluster.h"
#include "f360_math_func.h"
#include "rspp_detection_list.h"
#include "f360_norm_heading_angle.h"
#include "f360_sorted_clusters_mgmt.h"
#include "f360_sort_priority.h"
#include "f360_math_func.h"
#include "f360_calculate_priority.h"
#include <algorithm>

namespace f360_variant_A
{
   /******************************
   * File scope functions declarations
   *******************************/
   static void Update_Cluster_Timestamp(
      F360_Cluster_T& cluster,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list);

   /*===========================================================================*\
   * FUNCTION: Initialize_Clusters()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
   * const F360_Calibrations_T& calibrations,
   * F360_Local_Clusters_T &local_clusters_data,
   * F360_Tracker_Info_T &tracker_info,
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Detection_Hist_T& det_hist
   * F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]
   *
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
   *
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Initialize_Clusters(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Calibrations_T& calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      F360_Local_Clusters_T &local_clusters_data,
      F360_Tracker_Info_T &tracker_info,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T& det_hist,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS])
   {
      /* Figure out which local clusters that can fit into the cluster array by comparing priority of local clusters and clusters to find the most prioritized set. */
      const uint16_t num_unoccupied_clusters = tracker_info.variant.num_clusters - static_cast<uint16_t>(tracker_info.num_active_clusters);
      const uint16_t num_local_clusters_to_add_to_free_slots = std::min(local_clusters_data.num_clusters, num_unoccupied_clusters);
      float32_t local_clusters_prio_sorted_high_to_low[MAX_TRACKER_POSN_CLUSTERS]; // Sorted array of priorities for local custers
      uint32_t local_clusters_high_to_low_prio_perm[MAX_TRACKER_POSN_CLUSTERS]; // Sorted permutation array that will be used to indicate which of the local clusters to add to the clusters array
      float32_t clusters_prio_sorted_low_to_high[NUMBER_OF_CLUSTERS]; // Sorted array of priorities for clusters. Note: Not all elements of this array will be sorted. Only first part of array will be sorted
      int16_t clusters_low_to_high_prio_perm[NUMBER_OF_CLUSTERS]; // Sorted permutation array that will be used to indicate which of the clusters to kill to make room for new more prioritized local clusters. Note: Not all elements of this array will be sorted. Only first part of array will be sorted
      uint16_t num_low_prio_clusters = 0U; // Variable for how many clusters that have lower priority than the highest prioritized local cluster. Note: This variable indicastes how many elements in clusters_low_to_high_prio_perm that are sorted
      if (num_local_clusters_to_add_to_free_slots == local_clusters_data.num_clusters)
      {
         /* All local clusters can fit in the cluster array without killing any of the current clusters, i.e. the cluster
         * array will not be saturated. Fill local_clusters_high_to_low_prio_perm from 0 to "number of local clusters".
         * Order doesn't matter since all local clusters will be added anyways. */
         for (uint16_t local_clust_idx = 0U; local_clust_idx < local_clusters_data.num_clusters; local_clust_idx++)
         {
            local_clusters_high_to_low_prio_perm[local_clust_idx] = local_clust_idx;
         }
      }
      else // The cluster array will be saturated. Make sure to fill the array with the most prioritized of all clusters and local clusters
      {
         // Compute priorities of new local clusters and sort it from lowest to highest priority
         Compute_Local_Cluster_Priority_And_Sort(local_clusters_data, host, detection_props, tracker_info.variant.num_dets_in_track, local_clusters_prio_sorted_high_to_low, local_clusters_high_to_low_prio_perm);

         /* Find the least prioritized clusters. Note: Not all clusters will be sorted. Only the least prioritized that we want to compare with local clusters will be found.
         * The variable num_low_prio_clusters are indicating how many such low prio clusters have been found and sorted. The beginning of the clusters_low_to_high_prio_perm array
          will indicate the sorting order but the end is still unsorted after the function call.*/
         const uint16_t max_num_local_clusters_to_add_to_occupied_slots = local_clusters_data.num_clusters - num_local_clusters_to_add_to_free_slots;
         const float32_t highest_local_cluster_prio_not_fitting_in_array = local_clusters_prio_sorted_high_to_low[num_local_clusters_to_add_to_free_slots];
         Find_Least_Prioritized_Clusters(tracker_info, host, clusters, max_num_local_clusters_to_add_to_occupied_slots, highest_local_cluster_prio_not_fitting_in_array, clusters_prio_sorted_low_to_high, clusters_low_to_high_prio_perm, num_low_prio_clusters);
      }

      // Arrays to store local cluster long pos in such that we can do a batch inssert into the vcs long pos sorted list at the end rather than doing multiple insers each time a locla cluster is moved to the cluster array
      float32_t initialized_clusters_vcs_long[MAX_TRACKER_POSN_CLUSTERS]; // each clustering function can initialize MAX_TRACKER_POSN_CLUSTERS clusters
      int16_t initialized_clusters_id[MAX_TRACKER_POSN_CLUSTERS];

      // Add local clusters to unoccupied cluster slots
      for (uint16_t num_added_local_clusters = 0U; num_added_local_clusters < num_local_clusters_to_add_to_free_slots; num_added_local_clusters++)
      {
         const uint32_t local_cluster_idx = local_clusters_high_to_low_prio_perm[num_added_local_clusters];

         // Get id from inactive cluster list
         const int16_t new_cluster_id = tracker_info.inactive_cluster_ids[num_added_local_clusters];
         const int16_t new_cluster_idx = new_cluster_id - 1;

         // Add id to active cluster list
         tracker_info.active_cluster_ids[tracker_info.num_active_clusters] = new_cluster_id;
         tracker_info.num_active_clusters++;

         /* Initialize the cluster from local cluster data */
         const uint16_t first_det_idx_in_cluster = local_clusters_data.array_of_first_det_idx_in_clusters[local_cluster_idx];  // This is pointing to the index of first detection of given local cluster
         Init_Cluster(first_det_idx_in_cluster, local_clusters_data.array_of_det_idxs_in_clusters, raw_detection_list, calibrations, sensors, tracker_info.variant.num_dets_in_track, local_clusters_data.num_dets_in_clusters[local_cluster_idx], detection_props, clusters[new_cluster_idx]);
         
         // Store data for batch insert of new clusters into the vcs long pos sorted list
         initialized_clusters_vcs_long[num_added_local_clusters] = clusters[new_cluster_idx].vcs_position_x;
         initialized_clusters_id[num_added_local_clusters] = new_cluster_id;
      }

      // Shift out activated cluster id:s from the inactive cluster array
      for (uint16_t idx1 = num_local_clusters_to_add_to_free_slots; idx1 < num_unoccupied_clusters; idx1++)
      {
         const uint16_t idx2 = idx1 - num_local_clusters_to_add_to_free_slots;
         tracker_info.inactive_cluster_ids[idx2] = tracker_info.inactive_cluster_ids[idx1];
      }
      const uint16_t start_idx_to_clear = num_unoccupied_clusters - num_local_clusters_to_add_to_free_slots;
      for (uint16_t idx = start_idx_to_clear; idx < num_unoccupied_clusters; idx++)
      {
         tracker_info.inactive_cluster_ids[idx] = 0;
      }


      // Kill low prio clusters and replace them with higher prioritized local clusters
      uint16_t num_replaced_clusters;
      for (num_replaced_clusters = 0U; num_replaced_clusters < num_low_prio_clusters; num_replaced_clusters++)
      {
         const uint16_t local_cluster_i = num_local_clusters_to_add_to_free_slots + num_replaced_clusters; // Local cluster with highest prio. Note: We have already added the num_added_local_clusters most prioritized clusters to free slots. So in this loop we will have to continue from there
         
         const uint32_t cluster_i = num_replaced_clusters; // Cluster with lowest prio
         
         if (local_clusters_prio_sorted_high_to_low[local_cluster_i] > clusters_prio_sorted_low_to_high[cluster_i]) // Local cluster has higher prio than least prioritized cluster. Kill the cluster and replace with the local cluster
         {
            // Clear/kill the lowest prio cluster
            const int16_t cluster_id = tracker_info.active_cluster_ids[clusters_low_to_high_prio_perm[cluster_i]];
            const int16_t cluster_idx = cluster_id - 1;
            Sorted_Clusters_Remove(tracker_info, clusters[cluster_idx]); // Remove cluster from long pos sorted list
            for (int16_t idx = 0; idx < clusters[cluster_idx].num_old_dets; idx++) // Clear hist det data 
            {
               const int16_t det_idx = clusters[cluster_idx].old_det_idx[idx];
               det_hist.det_data[det_idx] = {};
               det_hist.f_idx_occupied[det_idx] = false;
               det_hist.n_occupied--;
            }

            // Note we don't need to call Clear_Cluster because it is done in Init_Cluster()
            
            /* Initialize the cluster from local cluster data */
            const uint32_t local_cluster_idx = local_clusters_high_to_low_prio_perm[local_cluster_i];
            const uint16_t first_det_idx_in_cluster = local_clusters_data.array_of_first_det_idx_in_clusters[local_cluster_idx];  // This is pointing to the index of first detection of given local cluster
            Init_Cluster(first_det_idx_in_cluster, local_clusters_data.array_of_det_idxs_in_clusters, raw_detection_list, calibrations, sensors, tracker_info.variant.num_dets_in_track, local_clusters_data.num_dets_in_clusters[local_cluster_idx], detection_props, clusters[cluster_idx]);

            // Store data for batch insert of new clusters into the vcs long pos sorted list
            initialized_clusters_vcs_long[local_cluster_i] = clusters[cluster_idx].vcs_position_x;
            initialized_clusters_id[local_cluster_i] = cluster_id;
         }
         else
         {
            // No more local clusters with higher prio than clusters. Break the loop
            break;
         }
      }

      // Do a batch insert of all newly initialized clusters into the vcs long pos sorted cluster list
      const uint16_t num_initialized_local_clusters = num_local_clusters_to_add_to_free_slots + num_replaced_clusters;
      if (num_initialized_local_clusters > 0U)
      {
         Sorted_Clusters_Insert_Batch(tracker_info, clusters, initialized_clusters_vcs_long, initialized_clusters_id, num_initialized_local_clusters);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Init_Cluster()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const uint16_t first_det_idx_in_cluster,
   * const uint16_t (&array_of_det_idxs_in_clusters)[MAX_NUMBER_OF_DETECTIONS],
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
   * const F360_Calibrations_T& calibrations,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const uint16_t max_dets_in_obj_track,
   * const uint16_t number_of_dets_in_cluster,
   * F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Cluster_T &cluster
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
   * This function initialized a new cluster based on the detections contained in a local cluster.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Init_Cluster(
      const uint16_t first_det_idx_in_cluster,
      const int16_t (&array_of_det_idxs_in_clusters)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Calibrations_T& calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const uint8_t max_dets_in_obj_track,
      const uint8_t number_of_dets_in_cluster,
      F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS],
      F360_Cluster_T &cluster)
   {

      Clear_Cluster(cluster);

      // Loop through detections to accumlate and compute cluster properties
      const uint16_t n_dets = std::min(number_of_dets_in_cluster, max_dets_in_obj_track); // consider just up to MAX_DETS_IN_OBJ_TRK dets
      const float32_t reference_vcs_az = raw_detection_list.detections[array_of_det_idxs_in_clusters[first_det_idx_in_cluster]].processed.vcs_az;
      int16_t num_moving_dets = 0;
      bool f_dealiased = false;
      for (uint16_t det_iter = 0U; det_iter < n_dets; det_iter++)
      {
         const int16_t det_index = array_of_det_idxs_in_clusters[first_det_idx_in_cluster + det_iter];

         // Link detection to cluster 
         det_p[det_index].cluster_id = cluster.id;
         cluster.detids[det_iter] = det_index + 1;

         // Accumulate position, range rate and azimuth information
         cluster.rep_rdotcomp += det_p[det_index].range_rate_compensated;
         cluster.vcs_position_x += det_p[det_index].vcs_position.x;
         cluster.vcs_position_y += det_p[det_index].vcs_position.y;
         cluster.rep_vcs_az += Normalize_Heading_Angle(raw_detection_list.detections[det_index].processed.vcs_az, reference_vcs_az);

         // Accumulate dealiasing information
         f_dealiased = f_dealiased || det_p[det_index].f_dealiased;

         if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_p[det_index].motion_status)
         {
            num_moving_dets++;
         }
      }
      cluster.ndets = static_cast<int16_t>(n_dets);
      const float32_t n_dets_inv = (n_dets > 0U) ? (1.0F / static_cast<float32_t>(n_dets)) : 1.0F; // Protection against zero division
      cluster.vcs_position_x *= n_dets_inv;
      cluster.vcs_position_y *= n_dets_inv;
      cluster.rep_rdotcomp *= n_dets_inv;
      cluster.rep_vcs_az = Normalize_Heading_Angle(cluster.rep_vcs_az * n_dets_inv, 0.0F);
      cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
      cluster.num_types_of_dets[0] = num_moving_dets;
      cluster.num_types_of_dets[1] = cluster.ndets - num_moving_dets;
      cluster.f_dealiased = f_dealiased;
      cluster.clutter_counter = 0;

      if (n_dets > 0U)
      {
         // If the cluster only has one detection, and a number of conditions are fulfilled, initialize the counter
         const rspp_variant_A::RSPP_Detection_T &detection = raw_detection_list.detections[array_of_det_idxs_in_clusters[first_det_idx_in_cluster]];
         if ((1 == cluster.ndets) && (detection.raw.range < calibrations.k_ocb_max_range) && (std::abs(detection.raw.range_rate) < calibrations.k_ocb_max_range_rate))
         {
            // Depending on the RCS, two different values are used
            if (detection.raw.rcs < calibrations.k_ocb_rcs_thresh_low_rcs)
            {
               cluster.low_rcs_dets_cnt = calibrations.k_ocb_cnt_delta_low_rcs_or_mult_dets;
            }
            else if (detection.raw.rcs < calibrations.k_ocb_rcs_thresh_midlow_rcs)
            {
               cluster.low_rcs_dets_cnt = calibrations.k_ocb_cnt_delta_midlow_rcs;
            }
            else
            {
               // Do nothing
            }
         }
      }

      cluster.time_since_cluster_updated = 0.0F;
      Update_Cluster_Timestamp(cluster, sensors, raw_detection_list);
   }

   /*===========================================================================*\
   * FUNCTION: Update_Cluster_Timestamp()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Cluster_T & const cluster,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list
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
   * This function updates cluster time since measurement timestamp.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Update_Cluster_Timestamp(
      F360_Cluster_T& cluster,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list)
   {
      cluster.time_since_measurement = 0.0F;
      for (int16_t j = 0; j < cluster.ndets; j++)
      {
         const int16_t det_idx = cluster.detids[j] - 1;
         const int32_t sensor_idx = raw_detect_list.detections[det_idx].raw.sensor_id - 1;
         const float time_since_measurement = sensors[sensor_idx].refined.time_since_measurement_s;

         if (0 == j)
         {
            cluster.time_since_measurement = time_since_measurement;
         }
         else if (time_since_measurement < cluster.time_since_measurement)
         {
            cluster.time_since_measurement = time_since_measurement;
         }
         else
         {
            //Do nothing.
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Find_Least_Prioritized_Clusters()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T& tracker_info,
   * const F360_Host_T& host,
   * const F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
   * const uint16_t max_num_clusters_to_sort,
   * const float32_t highest_local_cluster_prio,
   * float32_t(&priority)[NUMBER_OF_CLUSTERS],
   * uint32_t (&low_to_high_prio_perm)[NUMBER_OF_CLUSTERS],
   * uint16_t num_low_prio_clusters
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
   * This function computes cluster priority and finds the least prioritized clusters.
   * The least prioritized clusters are outputted in sorted order from lowest to highest
   * prioritized. The permutation array low_to_high_prio_perm is indicating the sorted
   * order. (Note that the priority array is not sorted but corresponds to the original
   * order of the clusters in the tracker_info.active_cluster_ids array.). The input
   * variables max_num_clusters_to_sort and highest_local_cluster_prio are determining
   * how many low priority cluster to find and sort. At max highest_local_cluster_prio
   * clusters are found but the sorting stops permaturely if the cluster priority exceeds
   * the value indicated by the input argument highest_local_cluster_prio. The output argument
   * num_low_prio_clusters shows how many cluster with priority lower than
   * that was actually found.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * Only the first num_low_prio_clusters of the output array low_to_high_prio_perm are sorted.
   * Remaning elements are in randomized order.
   *
   \*===========================================================================*/
   void Find_Least_Prioritized_Clusters(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T& host,
      const F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      const uint16_t max_num_clusters_to_sort,
      const float32_t highest_local_cluster_prio,
      float32_t(&priority)[NUMBER_OF_CLUSTERS],
      int16_t (&low_to_high_prio_perm)[NUMBER_OF_CLUSTERS],
      uint16_t & num_low_prio_clusters)
   {
      // Compute cluster priorities and sort it from lowest to highest priority
      for (int16_t cluster_i = 0; cluster_i < tracker_info.num_active_clusters; cluster_i++)
      {
         // Compute cluster priority
         const int16_t cluster_idx = tracker_info.active_cluster_ids[cluster_i] - 1;

         priority[cluster_i] = Calculate_Priority_For_Cluster(host, clusters[cluster_idx]);
         low_to_high_prio_perm[cluster_i] = cluster_i;
      }

      /* Not all clusters needs to be sorted. Only least prioritized clusters to compare with local clusters needs to be found.
      * Therefor do a bubble sort and break once we find the first cluster with higher priority than most prioritized local cluster. */
      num_low_prio_clusters = 0U;
      for (uint16_t num_sorted_clusters = 0U; num_sorted_clusters < max_num_clusters_to_sort; num_sorted_clusters++)
      {
         for (int16_t i = (tracker_info.num_active_clusters - 1); i > static_cast<int16_t>(num_sorted_clusters); i--)
         {
            if (priority[i] < priority[i-1])
            {
               // Swap
               const int16_t tmp_idx = low_to_high_prio_perm[i];
               low_to_high_prio_perm[i] = low_to_high_prio_perm[i - 1];
               low_to_high_prio_perm[i - 1] = tmp_idx;

               const float32_t tmp_prio = priority[i];
               priority[i] = priority[i - 1];
               priority[i - 1] = tmp_prio;
            }
         }
         const float32_t highest_sorted_prio = priority[num_sorted_clusters];
         if (highest_sorted_prio > highest_local_cluster_prio)
         {
            // We have found cluster with higher prio than the best local cluster so we can stop
            break;
         }
         else
         {
            // We have found a low prio cluster. Increase counter
            num_low_prio_clusters++;
         }
      }

      return;
   }


   /*===========================================================================*\
   * FUNCTION: Compute_Local_Cluster_Priority_And_Sort()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Local_Clusters_T& local_clusters,
   * const F360_Host_T& host,
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const uint16_t max_dets_in_cluster,
   * float32_t (&local_clusters_prio)[MAX_TRACKER_POSN_CLUSTERS],
   * uint32_t(&high_to_low_prio_perm)[MAX_TRACKER_POSN_CLUSTERS]
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
   * This function computes and sorts local cluster priority from highest to lowest
   * priority. The output array high_to_low_prio_perm indicates the sorting order
   * while the output array local_clusters_prio ramin in the same order as clusers
   * have in in local_clusters data structure.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Compute_Local_Cluster_Priority_And_Sort(
      const F360_Local_Clusters_T& local_clusters,
      const F360_Host_T& host,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint8_t max_dets_in_cluster,
      float32_t (&local_clusters_prio)[MAX_TRACKER_POSN_CLUSTERS],
      uint32_t(&high_to_low_prio_perm)[MAX_TRACKER_POSN_CLUSTERS])
   {
      for (uint16_t clust_idx = 0U; clust_idx < local_clusters.num_clusters; clust_idx++)
      {
         // Compute priority of all clusters
         const uint16_t ndets = std::min(local_clusters.num_dets_in_clusters[clust_idx], max_dets_in_cluster); // Consider just up to MAX_DETS_IN_OBJ_TRK dets (in line with what is done in Preinitialize_Clusters())
         if (local_clusters.num_dets_in_clusters[clust_idx] > 0U) // Protection against zero division
         {
            // Compute cluster position as mean of all detections. Count number of moving detections in cluster. // NOTE: Cluster is a local new cluster and has no historical detections to loop over
            float32_t mean_long_pos = 0.0F;
            float32_t mean_lat_pos = 0.0F;
            int16_t num_moving_dets = 0;
            const uint16_t first_det_idx_in_cluster = local_clusters.array_of_first_det_idx_in_clusters[clust_idx];  // This is pointer to the index of first detection of given local cluster
            for (uint16_t det_iter = 0U; det_iter < ndets; det_iter++)
            {
               const int16_t det_index = local_clusters.array_of_det_idxs_in_clusters[first_det_idx_in_cluster + det_iter];
               mean_long_pos += det_props[det_index].vcs_position.x;
               mean_lat_pos += det_props[det_index].vcs_position.y;

               if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_props[det_index].motion_status)
               {
                  num_moving_dets++;
               }
            }
            const float32_t inv_ndets = 1.0F / (static_cast<float32_t>(ndets));
            mean_long_pos *= inv_ndets;
            mean_lat_pos *= inv_ndets;

            // Fill temporary cluster with information needed to compute its priority correctly
            F360_Cluster_T tmp_cluster;
            
            tmp_cluster.rep_rdotcomp = 0.0F;
            tmp_cluster.vcs_position_x = mean_long_pos;
            tmp_cluster.vcs_position_y = mean_lat_pos;
            tmp_cluster.f_dealiased = false;
            tmp_cluster.num_types_of_dets[0] = num_moving_dets;
            tmp_cluster.num_types_of_dets[1] = static_cast<int16_t>(ndets) - num_moving_dets;

            // Compute cluster priority
            local_clusters_prio[clust_idx] = Calculate_Priority_For_Cluster(host, tmp_cluster);
         }
         else
         {
            // Empty cluster. Indicate invalid priority
            local_clusters_prio[clust_idx] = -1.0F;
         }
      }

      // Sort clusters from highest to lowest priority
      (void)F360_Sort(static_cast<uint32_t>(local_clusters.num_clusters), false, local_clusters_prio, high_to_low_prio_perm);

      return;
   }
}
