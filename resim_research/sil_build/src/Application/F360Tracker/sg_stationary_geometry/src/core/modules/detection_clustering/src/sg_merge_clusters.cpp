/*=============================================================================================*\
* FILE: sg_merge_clusters.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for merge_clusters function.
* This function merges detections from two or more clusters into one,
* and assings new cluster_id to merged detections. It also releases cluster_ids of merged clusters.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_merge_clusters.h"

#include <algorithm>

namespace sg
{
   uint16_t determine_merger_cluster_id(const NeighborCacheIterators &neighbors_its)
   {
      uint16_t oldest_cluster_id{};
      uint16_t oldest_age{};

      bool first_loop_iteration = true;
      for (const auto &current_neighbor_it : neighbors_its)
      {
         const auto current_neighbor   = *current_neighbor_it;
         const auto current_cluster_id = current_neighbor->temp_cluster_id;
         const auto current_age        = current_neighbor->age;
         if (current_neighbor->f_dbscan_core)
         {
            if (first_loop_iteration)
            {
               oldest_cluster_id    = current_cluster_id;
               oldest_age           = current_age;
               first_loop_iteration = false;
            }
            if (current_age == oldest_age)
            {
               if ((current_cluster_id < oldest_cluster_id) && (current_cluster_id > 0U))
               {
                  oldest_cluster_id = current_cluster_id;
               }
            }
            else if ((current_age > oldest_age) && (current_cluster_id != INVALID_CLUSTER_ID))
            {
               oldest_cluster_id = current_cluster_id;
               oldest_age        = current_age;
            }
            else
            {
               // MISRA
            }
         }
         (void) current_age;        // MISRA
         (void) current_cluster_id; // MISRA
      }
      (void) oldest_age; // MISRA

      return oldest_cluster_id;
   }


   void get_unique_cluster_ptrs(std::array<sg::Cluster *, SG_MAX_NUM_INTERNAL_CLUSTERS> &unique_cluster_ptrs,
                                const NeighborCacheIterators &neighbors_its)
   {
      std::array<bool, 2 * SG_MAX_NUM_INTERNAL_CLUSTERS + 1> cluster_ptrs_helper{false};
      std::size_t cluster_idx = 0U;

      for (const auto &neighbor : neighbors_its)
      {
         if ((*neighbor)->f_dbscan_core && (!cluster_ptrs_helper[(*neighbor)->cluster->unique_id()]))
         {
            cluster_ptrs_helper[(*neighbor)->cluster->unique_id()] = true;
            unique_cluster_ptrs[cluster_idx]                       = (*neighbor)->cluster;
            ++cluster_idx;
         }
      }
      (void) cluster_ptrs_helper; // MISRA
      (void) cluster_idx;         // MISRA
   }

   void merge_clusters(const NeighborCacheIterators &neighbors)
   {
      std::array<sg::Cluster *, SG_MAX_NUM_INTERNAL_CLUSTERS> unique_cluster_ptrs{};
      get_unique_cluster_ptrs(unique_cluster_ptrs, neighbors);

      const uint16_t merger_cluster_id = determine_merger_cluster_id(neighbors);

      // set neighbors to the merger cluster id
      for (const auto &current_neighbor_it : neighbors)
      {
         (*current_neighbor_it)->temp_cluster_id = merger_cluster_id;
      }

      // set rest of a neighbors cluster dets to the merger cluster id
      // iterate over unique cluster pointers, then set detection temp_cluster_id for all cluster dets
      for (auto &unique_cluster_ptr : unique_cluster_ptrs)
      {
         if (unique_cluster_ptr == nullptr)
         {
            break;
         }
         else if (unique_cluster_ptr->unique_id() != INVALID_CLUSTER_ID)
         {
            for (auto current_cluster_det_it = unique_cluster_ptr->begin(); current_cluster_det_it != unique_cluster_ptr->end();
                 current_cluster_det_it++)
            {
               current_cluster_det_it->temp_cluster_id = merger_cluster_id;
            }
         }
         else
         {
            // MISRA
         }
      }
      (void) merger_cluster_id; // MISRA
   }
}
