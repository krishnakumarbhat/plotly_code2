/*=============================================================================================*\
* FILE: sg_sort_clusters_by_distance.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for sort_clusters_by_distance and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_SORT_CLUSTERS_BY_DISTANCE_H
#define SG_SORT_CLUSTERS_BY_DISTANCE_H

#include <array>

#include "geometry/geo_point.h"
#include "sg_constants.h"
#include "sg_detection_storage.h"

namespace sg
{
   struct ClusterIdDistance
   {
      Cluster *cluster_ptr = nullptr;
      float distance       = std::numeric_limits<float>::max();
   };

   /**
    * @brief   Calculates the shortest Squared euclidean distances between host and clusters
    *          and returns them in the distances_to_clusters array.
    *          The distance(s) from the distances_to_clusters array are assumed to be initialized with the maximal values of its
    *type. On return, the distances_to_clusters array is updated with minimal distances relating to entry cluster ids.
    *
    * @param[in, out]   distances_to_clusters
    * @param[in]        all_clusters
    * @param[in]        host_position
    *
    * @return  number of clusters
    **/
   uint16_t fill_distances_to_clusters(std::array<ClusterIdDistance, SG_MAX_NUM_INTERNAL_CLUSTERS> &distances_to_clusters,
                                       const Clusters &all_clusters,
                                       const geometry::Point2D_T &host_position);

   /**
    * @brief   Sort cluster ids array by distance between cluster and the host.
    *          On entry the unique_cluster_ptrs array is assumed to be initialized to nullptrs.
    *          On return the unique_cluster_ptrs array is filled out with valid, unique pointers to clusters.
    *          The pointers are sorted ascending with distances between host and cluster.
    *
    * @param   detections
    * @param   host_position
    * @param   unique_cluster_ptrs
    *
    * @return  number of elements returned in the unique_cluster_ids array.
    **/
   uint16_t sort_clusters_by_distance(DetectionStorage &detections,
                                      const geometry::Point2D_T &host_position,
                                      std::array<Cluster *, SG_MAX_NUM_INTERNAL_CLUSTERS> &unique_cluster_ptrs);
}
#endif
