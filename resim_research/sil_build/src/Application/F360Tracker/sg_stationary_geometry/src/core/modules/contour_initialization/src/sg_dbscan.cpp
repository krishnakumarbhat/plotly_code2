/*=============================================================================================*\
* FILE: sg_dbscan.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for dbscan.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "sg_dbscan.h"

#include <algorithm>

#include "sg_expand_cluster.h"
#include "sg_initialize_contours_helpers.h"

namespace sg
{
   void dbscan(const Cluster *const current_cluster_ptr, const float cluster_radius, const uint8_t min_cluster_points)
   {
      // initialize datastructures
      std::size_t new_dets_idx  = 0U;
      const auto detections_end = current_cluster_ptr->end();
      for (auto detection_it = current_cluster_ptr->begin(); detection_it != detections_end; ++detection_it)
      {
         detection_it->temp_cluster_id  = INVALID_CLUSTER_ID;
         detection_it->f_dbscan_visited = false;
         detection_it->f_new            = true; // treat all detections as new detections
         new_dets_idx++;
      }
      (void) new_dets_idx; // MISRA

      uint16_t cluster_id = 1U;
      for (auto detection_it = current_cluster_ptr->begin(); detection_it != detections_end; ++detection_it)
      {
         auto &current_detection           = *detection_it;
         const bool f_valid_for_clustering = (current_detection.f_subset && (!current_detection.f_dbscan_visited));
         if (f_valid_for_clustering)
         {
            current_detection.f_dbscan_visited = true;
            NeighborIterators current_neighbors{};
            bool f_add_to_cluster = false;

            if (current_detection.temp_cluster_id == INVALID_CLUSTER_ID)
            {
               const uint8_t current_num_neighbors =
                  get_dets_in_circular_region(current_neighbors, current_cluster_ptr, current_detection, cluster_radius);

               f_add_to_cluster = (min_cluster_points <= current_num_neighbors);
            }

            if (f_add_to_cluster)
            {
               expand_cluster(current_neighbors, current_detection, current_cluster_ptr, cluster_id, min_cluster_points,
                              cluster_radius);
               ++cluster_id;
            }
         }
      }

      (void) cluster_id; // MISRA
   }
}
