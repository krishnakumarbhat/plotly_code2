/*=============================================================================================*\
* FILE: sg_expand_cluster.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for expand_cluster and helper functions
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_EXPAND_CLUSTER_H
#define SG_EXPAND_CLUSTER_H

#include "sg_clustering_aliases.h"
#include "sg_detection_storage.h"
#include "sg_expand_cluster_helpers.h"
#include "sg_init_aliases.h"

namespace sg
{
   /**
    * @brief    expands a cluster from a single point, adding new detections to that cluster.
    *           This function only updates cluster information in cluster_ids array with new cluster assignments, but
    *           doesn't do any changes to DetectionStorage itself
    *
    * @param[in, out]      current_neighbors
    * @param[in, out]      current_detection
    * @param[in]           current_cluster_ptr
    * @param[in]           cluster_id
    * @param[in]           min_cluster_points
    * @param[in]           cluster_radius
    *
    * @return              N/A
    **/
   void expand_cluster(NeighborIterators &current_neighbors,
                       Detection_T &current_detection,
                       const Cluster *const current_cluster_ptr,
                       const uint16_t cluster_id,
                       const uint8_t min_cluster_points,
                       const float cluster_radius);
}
#endif
