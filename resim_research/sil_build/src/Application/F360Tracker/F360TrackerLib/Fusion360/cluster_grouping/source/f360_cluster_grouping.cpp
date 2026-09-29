/*===================================================================================*\
* FILE: f360_cluster_grouping.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This is the main function for grouping of clusters. During grouping, dealiasing
*   possibilities are evaluated and after confirmation, executed.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


#include "f360_cluster_grouping.h"
#include "f360_calculate_priority.h"
#include "f360_update_cluster_state.h"
#include "f360_try_to_dealiase_rdots_in_two_clusters.h"
#include "f360_merge_two_clusters.h"
#include "f360_kill_cluster.h"
#include "f360_sorted_clusters_mgmt.h"
#include "f360_coarse_cluster_gate.h"
#include "f360_fine_cluster_gate.h"
#include "f360_get_wall_time.h"
#include "f360_sort_priority.h"
#include "f360_terminate_clusters.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
   static bool Check_Cluster_Grouping_Conditions(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calib,
      const F360_Cluster_T& cluster_1,
      const F360_Cluster_T& cluster_2,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T & det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      float32_t& rdot_interval_1,
      float32_t& rdot_interval_2,
      float32_t& alias_interval_1,
      float32_t& alias_interval_2);

   static void Swap_Elements_In_VscLong_Sorted_List(
      F360_Cluster_T*& cluster_1,
      F360_Cluster_T*& cluster_2,
      F360_Tracker_Info_T& tracker_info);

   /*===========================================================================*\
   * FUNCTION: Cluster_Grouping()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Globals_T & globals,
   * const F360_Calibrations_T &calib,
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Tracker_Info_T & tracker_info,
   * F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Detection_Hist_T & det_hist,
   * F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
   * F360_TRKR_TIMING_INFO_T & timing_info
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
   * This function merges clusters when their parameters indicate they contain 
   * current and historical detections from the same object.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Cluster_Grouping(
      const F360_Globals_T & globals,
      const F360_Calibrations_T &calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();

      // Update cluster state
      for (int16_t i = 0; i < tracker_info.num_active_clusters; i++)
      {
         const int16_t cluster_idx = tracker_info.active_cluster_ids[i] - 1;
         Update_Cluster_State(detection_props, raw_detection_list, det_hist, clusters[cluster_idx]);
      }
      Sorted_Clusters_Re_Sort(tracker_info);

      // Associate clusters and de-alias detection range-rates
      F360_Cluster_T* cluster_1 = tracker_info.vcslong_sorted_cluster_start;
      F360_Cluster_T* cluster_2;
      if (NULL != cluster_1)
      {
         cluster_2 = tracker_info.vcslong_sorted_cluster_next[cluster_1->id - 1];
      }
      else
      {
         cluster_2 = NULL;
      }

      while ((NULL != cluster_1) && (NULL != cluster_2))
      {
         if ((!cluster_1->f_to_be_killed) && (!cluster_2->f_to_be_killed))
         {
            float32_t rdot_interval_1 = 0.0F;
            float32_t rdot_interval_2 = 0.0F;
            float32_t alias_interval_1 = 0.0F;
            float32_t alias_interval_2 = 0.0F;

            const bool f_can_be_merged = Check_Cluster_Grouping_Conditions(globals, calib, *cluster_1, *cluster_2,
               tracker_info, detection_props, det_hist, raw_detection_list, sensors, rdot_interval_1,
               rdot_interval_2, alias_interval_1, alias_interval_2);

            if (f_can_be_merged)
            {
               Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, detection_props, det_hist, cluster_1->id, cluster_2->id,
                  rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2, calib.k_max_dealiased_range_rate_diff);

               if (cluster_1->vcs_position_x > cluster_2->vcs_position_x)
               {
                  /* The merge process resulted in that the vcs_positions has been switched such that cluster 2 is now closer then cluster 1.
                   * Note: This code loopes over clusters in incresing vcs order so it is assumed cluster 1 should be closer than cluster 2.
                   * As a consequence the vcs long pos sorted cluster list should be updated and cluster 1 and cluster 2 must switch. */
                  // Switch pointers to next and previous clusters in vcs long sorted list
                  Swap_Elements_In_VscLong_Sorted_List(cluster_1, cluster_2, tracker_info);
               }
            }
         }

         // Always iterate second cluster.
         cluster_2 = tracker_info.vcslong_sorted_cluster_next[cluster_2->id - 1];

         // Check if first cluster need to be iterated -> then also second cluster need to be reset.
         if ((NULL == cluster_2) ||
            (cluster_1->f_to_be_killed) ||
            ((cluster_2->vcs_position_x - cluster_1->vcs_position_x) > calib.k_moving_clusters_dist_coarse_gate))
         {
            cluster_1 = tracker_info.vcslong_sorted_cluster_next[cluster_1->id - 1];
            cluster_2 = tracker_info.vcslong_sorted_cluster_next[cluster_1->id - 1];
         }
      }

      Terminate_Clusters(clusters, det_hist, tracker_info);

      timing_info.cluster_grouping = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Check_Cluster_Grouping_Conditions()
   *===========================================================================
   * RETURN VALUE:
   * bool grouping_conditions_valid
   *
   * PARAMETERS:
   * const F360_Globals_T& globals,
   * const F360_Calibrations_T& calib,
   * const F360_Cluster_T& cluster_1,
   * const F360_Cluster_T& cluster_2,
   * const F360_Tracker_Info_T& tracker_info,
   * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * const F360_Detection_Hist_T & det_hist,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * float32_t& rdot_interval_1,
   * float32_t& rdot_interval_2,
   * float32_t& alias_interval_1,
   * float32_t& alias_interval_2
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
   * This function verifies if two clusters should be merged together.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Check_Cluster_Grouping_Conditions(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calib,
      const F360_Cluster_T& cluster_1,
      const F360_Cluster_T& cluster_2,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T & det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      float32_t& rdot_interval_1,
      float32_t& rdot_interval_2,
      float32_t& alias_interval_1,
      float32_t& alias_interval_2
      )
   {
      bool f_success = Coarse_Cluster_Gate(calib, cluster_1, cluster_2);

      if (f_success)
      {
         f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_1, cluster_2, sensors, raw_detection_list, detection_props, det_hist, tracker_info, globals, calib,
            rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);
      }

      if (f_success)
      {
         f_success = Fine_Cluster_Gate(calib, cluster_1, cluster_2, rdot_interval_1, rdot_interval_2, alias_interval_1, alias_interval_2);
      }

      return f_success;
   }


   /*===========================================================================*\
   * FUNCTION: Swap_Elements_In_VscLong_Sorted_List()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Cluster_T* const cluster_1,
   * F360_Cluster_T* const cluster_2,
   * F360_Tracker_Info_T& tracker_info
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
   * This function swaps the location of two clusters in the sorted vcslong cluster list.
   * The function assumes that before the swaping cluster 1 is before cluster 2 in the list.
   *
   * PRECONDITIONS:
   * Cluster 1 is before cluster 2 in the list. I.e. Cluster 2 cant be the start of the list
   * Cluster 2 is after cluster 1 in the list. I.e. Cluster 1 cant be the end of the list
   *
   * POSTCONDITIONS:
   *
   \*===========================================================================*/
   static void Swap_Elements_In_VscLong_Sorted_List(
      F360_Cluster_T*& cluster_1,
      F360_Cluster_T*& cluster_2,
      F360_Tracker_Info_T& tracker_info)
   {
      if ((cluster_1 == NULL) || (cluster_2 == NULL))
      {
         /* This should never happen. If this happens the preconditions for the function are not fullfilled or the vcslong sorted list is corrupted.
         * For code safety and MISRA compliance such that we are sure we never try to access values of cluster_1 and cluster_2 pointer in case it is NULL.
         * Do nothing */
      }
      else
      {
         const int16_t cluster_1_idx = cluster_1->id - 1;
         const int16_t cluster_2_idx = cluster_2->id - 1;

         F360_Cluster_T* const cluster_1_next = tracker_info.vcslong_sorted_cluster_next[cluster_1_idx];
         F360_Cluster_T* const cluster_1_prev = tracker_info.vcslong_sorted_cluster_prev[cluster_1_idx];
         F360_Cluster_T* const cluster_2_prev = tracker_info.vcslong_sorted_cluster_prev[cluster_2_idx];
         F360_Cluster_T* const cluster_2_next = tracker_info.vcslong_sorted_cluster_next[cluster_2_idx];

         if ((cluster_2_prev == NULL) || (cluster_1_next == NULL))
         {
            /* Do nothing */
         }
         else
         {
            // Swap the "inner" links
            const bool f_adjacent_clusters = (cluster_1_next == cluster_2);
            if (f_adjacent_clusters)
            {
               tracker_info.vcslong_sorted_cluster_next[cluster_2_idx] = cluster_1;
               tracker_info.vcslong_sorted_cluster_prev[cluster_1_idx] = cluster_2;
            }
            else
            {
               tracker_info.vcslong_sorted_cluster_next[cluster_2_idx] = cluster_1_next;
               tracker_info.vcslong_sorted_cluster_prev[cluster_1_next->id - 1] = cluster_2;

               tracker_info.vcslong_sorted_cluster_prev[cluster_1_idx] = cluster_2_prev;
               tracker_info.vcslong_sorted_cluster_next[cluster_2_prev->id - 1] = cluster_1;
            }
            
            // Swap the "outer" links
            tracker_info.vcslong_sorted_cluster_prev[cluster_2_idx] = cluster_1_prev;
            tracker_info.vcslong_sorted_cluster_next[cluster_1_idx] = cluster_2_next;
            
            if (cluster_1_prev == NULL)
            {
               // Cluster 1 used to be the start of the sorted list which means now cluster 2 should be the start instead
               tracker_info.vcslong_sorted_cluster_start = cluster_2;
            }
            else
            {
               tracker_info.vcslong_sorted_cluster_next[cluster_1_prev->id - 1] = cluster_2;
            }

            if (cluster_2_next != NULL)
            {
               // Cluster 2 was not at the end of the sorted list
               tracker_info.vcslong_sorted_cluster_prev[cluster_2_next->id - 1] = cluster_1;
            }

            F360_Cluster_T* const tmp = cluster_1;
            cluster_1 = cluster_2;
            cluster_2 = tmp;
         }
      }
   }

}
