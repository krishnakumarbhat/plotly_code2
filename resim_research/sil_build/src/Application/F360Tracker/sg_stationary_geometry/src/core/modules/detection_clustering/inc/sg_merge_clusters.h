/*=============================================================================================*\
* FILE: sg_merge_clusters.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration for merge_clusters function.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_MERGE_CLUSTERS_H
#define SG_MERGE_CLUSTERS_H

#include <array>
#include <bitset>

#include "sg_clustering_aliases.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief        This function determines the cluster_id to be set for the merged cluster. The oldest detection within a
    *               cluster determines the merger_cluster_id. Search for oldest cluster_id in detections_to_cluster and ret
    *               if ages are the same, take the lower cluster id between both
    *
    * @param[in]    neighbors_its - iterator set to neighbors collected during clustering expansion
    *                                               removed
    *
    * @return       cluster id of the oldest detection within neighbors
    **/
   uint16_t determine_merger_cluster_id(const NeighborCacheIterators &neighbors_its);

   /**
    * @brief    This function merges detections from two or more clusters into one and updates
    *           cluster_id array according to a new cluster assignment.
    *
    * @param[in, out]    neighbors - set of neighbors collected during clustering expansion
    *
    * @return   N/A
    **/
   void merge_clusters(const NeighborCacheIterators &neighbors);

   /**
    * @brief              This function gets all unique cluster pointers
    *
    * @param[in, out]     unique_cluster_ptrs - array of unique pointers
    * @param[in]          neighbors_its - iterator set to neighbors collected during clustering expansion
    *
    * @return   N/A
    **/
   void get_unique_cluster_ptrs(std::array<sg::Cluster *, SG_MAX_NUM_INTERNAL_CLUSTERS> &unique_cluster_ptrs,
                                const NeighborCacheIterators &neighbors_its);
}


#endif
