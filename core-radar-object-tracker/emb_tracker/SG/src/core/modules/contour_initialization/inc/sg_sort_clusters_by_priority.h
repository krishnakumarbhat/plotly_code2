/*=============================================================================================*\
* FILE: sg_sort_clusters_by_priority.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for sort_clusters_by_priority and helper functions.
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
#include "sg_calibrations.h"
#include "sg_constants.h"
#include "sg_detection_storage.h"

namespace sg
{
struct ClusterIdPriority
{
   Cluster *cluster_ptr = nullptr;
   float priority       = 0.0F;
};

/**
 * @brief Calculate priority of detection
 *
 * @param[in]     detection
 * @param[in]     host_curvature_rear
 * @param[in]     calibrations
 * @return float
 */
float calculate_detection_priority(const Detection_T &detection, const float host_curvature_rear,
                                   const ContourInitializationCalibs &calibrations);

/**
 * @brief   Calculates priorities of clusters and returns them in the priorities_of_clusters array.
 *
 * @param[in, out]   priorities_of_clusters
 * @param[in]        all_clusters
 * @param[in]        host_curvature_rear
 * @param[in]        calibrations
 *
 * @return  number of clusters
 **/
uint16_t fill_clusters_priorities(std::array<ClusterIdPriority, SG_MAX_NUM_INTERNAL_CLUSTERS> &priorities_of_clusters,
                                  const Clusters &all_clusters, const float host_curvature_rear,
                                  const ContourInitializationCalibs &calibrations);

/**
 * @brief   Sort cluster ids array by priority.
 *          On entry the unique_cluster_ptrs array is assumed to be initialized to nullptrs.
 *          On return the unique_cluster_ptrs array is filled out with valid, unique pointers to clusters.
 *          The pointers are sorted ascending with priorities of cluster.
 *
 * @param[in, out]   detections
 * @param[out]       unique_cluster_ptrs
 * @param[in]        host_curvature_rear
 * @param[in]        calibrations
 *
 * @return  number of elements returned in the unique_cluster_ids array.
 **/
uint16_t sort_clusters_by_priority(DetectionStorage &detections,
                                   std::array<Cluster *, SG_MAX_NUM_INTERNAL_CLUSTERS> &unique_cluster_ptrs,
                                   const float host_curvature_rear, const ContourInitializationCalibs &calibrations);
} // namespace sg
#endif
