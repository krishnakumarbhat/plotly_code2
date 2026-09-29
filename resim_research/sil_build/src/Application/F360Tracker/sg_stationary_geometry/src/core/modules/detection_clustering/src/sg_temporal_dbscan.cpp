/*=============================================================================================*\
* FILE: sg_temporal_dbscan.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for temporal_dbscan and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "sg_temporal_dbscan.h"

#include <algorithm>

#include "sg_cluster_detections_helpers.h"
#include "sg_temporal_expand_cluster.h"

namespace sg
{
   void temporal_dbscan(DetectionStorage &detections,
                        const float cluster_radius,
                        const float historical_num_neighbors_forgetting_factor,
                        const uint8_t min_cluster_points)
   {
      const auto detections_end_it = detections.end();
      for (auto detection_it = detections.begin(); detection_it != detections_end_it; ++detection_it)
      {
         detection_it->temp_cluster_id  = detection_it->cluster_id;
         detection_it->f_dbscan_visited = false;
         detection_it->f_new            = (0U < detection_it->age) ? false : true;
      }

      const DetectionCache &detection_cache = detections.get_cache();
      const auto sorted_dets_end_it         = detection_cache.cend();
      for (auto detection_it = detection_cache.cbegin(); detection_it != sorted_dets_end_it; ++detection_it)
      {
         auto &current_detection           = **detection_it;
         const bool f_valid_for_clustering = (current_detection.f_subset && (!current_detection.f_dbscan_visited));
         if (f_valid_for_clustering)
         {
            current_detection.f_dbscan_visited = true;
            NeighborCacheIterators current_neighbors{};

            const uint8_t current_num_neighbors =
               get_dets_in_circular_region(current_neighbors, detections, current_detection, cluster_radius);

            current_detection.current_num_neighbors = static_cast<float>(current_num_neighbors);
            const float weighted_num_neighbors = current_detection.current_num_neighbors + current_detection.cumulated_num_neighbors;
            update_neighbors_history(current_detection, historical_num_neighbors_forgetting_factor);
            current_detection.f_dbscan_core = (static_cast<float>(min_cluster_points) <= weighted_num_neighbors);
            const bool f_add_to_cluster     = current_detection.f_dbscan_core && (0U < current_num_neighbors);

            if (f_add_to_cluster)
            {
               uint16_t cluster_id{};
               if (current_detection.cluster_id == INVALID_CLUSTER_ID)
               {
                  const auto cluster_it = detections.get_clusters().create_new();
                  cluster_id            = cluster_it->unique_id();
               }
               else
               {
                  cluster_id = current_detection.cluster_id;
               }

               temporal_expand_cluster(current_neighbors, detections, current_detection, cluster_id, min_cluster_points,
                                       cluster_radius, historical_num_neighbors_forgetting_factor);
            }
         }
      }
   }

   void update_neighbors_history(Detection_T &detection, const float historical_num_neighbors_forgetting_factor)
   {
      detection.cumulated_num_neighbors =
         historical_num_neighbors_forgetting_factor * detection.cumulated_num_neighbors + detection.current_num_neighbors;
   }
}
