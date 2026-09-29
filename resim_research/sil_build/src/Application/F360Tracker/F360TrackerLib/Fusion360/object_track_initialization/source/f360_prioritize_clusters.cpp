/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_prioritize_clusters.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Prioritize_Clusters()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_prioritize_clusters.h"
#include "f360_initial_detection_checks.h"
#include "f360_calculate_priority.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Prioritize_Clusters()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibrations,
   * const F360_Host_T& host,
   * const F360_Tracker_Info_T& tracker_info,
   * const F360_Detection_Hist_T& det_hist,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
   * int32_t(&prioritized_cluster_ids)[NUMBER_OF_CLUSTERS],
   * uint32_t& num_clusters)
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
   * This function performs checks on all active clusters to check whether they are viable for further
   * initialization and prioritizes them based on the priority calculated in Calculate_Priority().
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Prioritize_Clusters(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      int32_t(&prioritized_cluster_ids)[NUMBER_OF_CLUSTERS],
      uint32_t& num_clusters)
   {
      uint32_t num_clusters_prel = 0U;
      float32_t priority[NUMBER_OF_CLUSTERS]{};
      int16_t cluster_id[NUMBER_OF_CLUSTERS]{};

      for (int32_t i = 0; i < tracker_info.num_active_clusters; i++)
      {
         F360_Cluster_T& cluster = clusters[tracker_info.active_cluster_ids[i] - 1];

         if (cluster.f_dealiased &&
            (cluster.ndets >= 1) &&
            ((cluster.num_old_dets + cluster.ndets) >= static_cast<int16_t>(calibrations.k_init_min_num_dets_from_outside_restrictive_zone)))
         {
            const bool f_detection_checks_passed = Initial_Detection_Checks(det_hist, raw_detections, det_props, cluster);

            if (f_detection_checks_passed)
            {
               cluster_id[num_clusters_prel] = cluster.id;

               constexpr float32_t k_moving_prob = 1.0F;
               constexpr float32_t k_confidence = 0.5F;

               priority[num_clusters_prel] = Calculate_Priority(host, k_moving_prob, k_confidence, cluster.vcs_position_x, cluster.vcs_position_y); // Note: It is intentional to not use the function Calculate_Priority_For_Cluster here since we want the moving probability to be 1 for all clusters
               num_clusters_prel++;
            }
         }
      }

      num_clusters = 0U;
      const uint32_t num_of_tracks_available_to_initialize = tracker_info.variant.num_tracks - static_cast<uint32_t>(tracker_info.num_active_objs);
      //if there are more than one cluster valid for initialization than empty track slots
      if ((num_clusters_prel > 1U) && (num_clusters_prel > num_of_tracks_available_to_initialize))
      {
         uint32_t permutation[NUMBER_OF_CLUSTERS];
         //sort clusters valid to be initialized by the priority
         (void)F360_Sort(num_clusters_prel, false, priority, permutation);

         for (uint32_t i = 0U; i < num_clusters_prel; i++)
         {
            // use only clusters which priority is higher than the lowest priority track currently existing in the tracker or fill to the max available amount
            if ((priority[i] > tracker_info.p_lowest_priority_track->priority) || (num_clusters < num_of_tracks_available_to_initialize))
            {
               prioritized_cluster_ids[i] = cluster_id[permutation[i]];
               num_clusters++;
            }
         }
      }
      else
      {
         // if there is enough room to fit all the clusters valid to be initialized then put all into the list
         num_clusters = num_clusters_prel;
         for (uint32_t i = 0U; i < num_clusters; i++)
         {
            prioritized_cluster_ids[i] = cluster_id[i];
         }
      }
   }
}
