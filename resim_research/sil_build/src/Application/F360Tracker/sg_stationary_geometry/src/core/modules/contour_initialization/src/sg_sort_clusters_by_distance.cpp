/*=============================================================================================*\
* FILE: sg_sort_clusters_by_distance.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for sort_clusters_by_distance and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_sort_clusters_by_distance.h"

#include "geometry/geo_distance.h"

namespace sg
{
   uint16_t fill_distances_to_clusters(std::array<ClusterIdDistance, SG_MAX_NUM_INTERNAL_CLUSTERS> &distances_to_clusters,
                                       const Clusters &all_clusters,
                                       const geometry::Point2D_T &host_position)
   {
      assert((SG_MAX_NUM_INTERNAL_DETS / 3U) <= SG_MAX_NUM_INTERNAL_CLUSTERS);

      uint16_t cnt                = 0U;
      const auto &last_cluster_it = all_clusters.end();
      for (auto cluster_it = std::next(all_clusters.begin()); cluster_it != last_cluster_it; ++cluster_it)
      {
         distances_to_clusters[cnt].cluster_ptr = &(*cluster_it);
         for (const auto &detection : *cluster_it)
         {
            const float distance = geometry::squared_euclidean_distance(host_position, detection.position);
            if (distance < distances_to_clusters[cnt].distance)
            {
               distances_to_clusters[cnt].distance = distance;
            }
         }
         ++cnt;
      }

      assert(cnt <= SG_MAX_NUM_INTERNAL_CLUSTERS);
      assert(static_cast<std::size_t>(cnt + 1) == all_clusters.size());

      return cnt;
   }

   uint16_t sort_clusters_by_distance(DetectionStorage &detections,
                                      const geometry::Point2D_T &host_position,
                                      std::array<Cluster *, SG_MAX_NUM_INTERNAL_CLUSTERS> &unique_cluster_ptrs)
   {
      std::array<ClusterIdDistance, SG_MAX_NUM_INTERNAL_CLUSTERS> distances_to_clusters;

      const uint16_t num_unique_cluster_ids =
         fill_distances_to_clusters(distances_to_clusters, detections.get_clusters(), host_position);

      const auto compare_cluster_distances = [](const ClusterIdDistance &lhs, const ClusterIdDistance &rhs) -> bool
      { return lhs.distance < rhs.distance; };
      std::sort(&distances_to_clusters[0], &distances_to_clusters[num_unique_cluster_ids], compare_cluster_distances);

      for (size_t idx = 0U; idx < num_unique_cluster_ids; idx++)
      {
         unique_cluster_ptrs[idx] = distances_to_clusters[idx].cluster_ptr;
      }

      return num_unique_cluster_ids;
   }
}
