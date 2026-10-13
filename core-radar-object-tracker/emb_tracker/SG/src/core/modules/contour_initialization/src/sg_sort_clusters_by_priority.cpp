/*=============================================================================================*\
* FILE: sg_sort_clusters_by_priority.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for sort_clusters_by_priority and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_sort_clusters_by_priority.h"

#include <math.h>

#include "geometry/geo_distance.h"
#include "sg_math.h"

namespace sg
{
float calculate_detection_priority(const Detection_T &detection, const float host_curvature_rear,
                                   const ContourInitializationCalibs &calibrations)
{
   const auto x_transformed =
      (detection.position.x - calibrations.cluster_priority_shift_x) / calibrations.cluster_priority_compaction_factor_x;
   auto y_transformed =
      calculate_curvi_lat_pos(-host_curvature_rear, sg::geometry::Point2D_T(detection.position.x, detection.position.y));
   y_transformed /= calibrations.cluster_priority_compaction_factor_y;
   const float square_distance = x_transformed * x_transformed + y_transformed * y_transformed;
   const float det_priority    = 1.0F / (1.0F + calibrations.cluster_importance_decay_coef * square_distance);
   return det_priority;
}

uint16_t fill_clusters_priorities(std::array<ClusterIdPriority, SG_MAX_NUM_INTERNAL_CLUSTERS> &priorities_of_clusters,
                                  const Clusters &all_clusters, const float host_curvature_rear,
                                  const ContourInitializationCalibs &calibrations)
{
   assert((SG_MAX_NUM_INTERNAL_DETS / 3U) <= SG_MAX_NUM_INTERNAL_CLUSTERS);

   uint16_t cluster_cnt        = 0U;
   const auto &last_cluster_it = all_clusters.end();
   for (auto cluster_it = std::next(all_clusters.begin()); cluster_it != last_cluster_it; ++cluster_it)
   {
      auto det_cnt                                    = 0U;
      priorities_of_clusters[cluster_cnt].cluster_ptr = &(*cluster_it);
      for (const auto &detection : *cluster_it)
      {
         priorities_of_clusters[cluster_cnt].priority +=
            calculate_detection_priority(detection, host_curvature_rear, calibrations);
         ++det_cnt;
      }
      priorities_of_clusters[cluster_cnt].priority /= static_cast<float>(det_cnt);
      ++cluster_cnt;
   }

   assert(cluster_cnt <= SG_MAX_NUM_INTERNAL_CLUSTERS);
   assert(static_cast<std::size_t>(cluster_cnt + 1U) == all_clusters.size());

   return cluster_cnt;
}

uint16_t sort_clusters_by_priority(DetectionStorage &detections,
                                   std::array<Cluster *, SG_MAX_NUM_INTERNAL_CLUSTERS> &unique_cluster_ptrs,
                                   const float host_curvature_rear, const ContourInitializationCalibs &calibrations)
{
   std::array<ClusterIdPriority, SG_MAX_NUM_INTERNAL_CLUSTERS> clusters_priorites;

   const uint16_t num_unique_cluster_ids =
      fill_clusters_priorities(clusters_priorites, detections.get_clusters(), host_curvature_rear, calibrations);

   const auto compare_cluster_priorities = [](const ClusterIdPriority &lhs, const ClusterIdPriority &rhs) -> bool
   { return lhs.priority > rhs.priority; };
   std::sort(std::begin(clusters_priorites), std::end(clusters_priorites), compare_cluster_priorities);

   for (size_t idx = 0U; idx < num_unique_cluster_ids; idx++)
   {
      unique_cluster_ptrs[idx] = clusters_priorites[idx].cluster_ptr;
   }

   return num_unique_cluster_ids;
}
} // namespace sg
