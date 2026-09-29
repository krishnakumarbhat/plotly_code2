/*=============================================================================================*\
* FILE: sg_temporal_expand_cluster.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for temporal_expand_cluster and helper functions
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_TEMPORAL_EXPAND_CLUSTER_H
#define SG_TEMPORAL_EXPAND_CLUSTER_H

#include <bitset>

#include "sg_clustering_aliases.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief    expands a cluster from a single point, adding new detections to that cluster and updating temporal data.
    *           This function only updated cluster_ids array with new cluster assignments, but
    *           doesn't do any changes to DetectionStorage itself.
    *
    * @param[in, out]      current_neighbors
    * @param[in, out]      detections
    * @param[in, out]      current_detection
    * @param[in]           cluster_id
    * @param[in]           min_cluster_points
    * @param[in]           cluster_radius
    * @param[in]           historical_num_neighbors_forgetting_factor,
    *
    * @return              N/A
    **/
   void temporal_expand_cluster(NeighborCacheIterators &current_neighbors,
                                DetectionStorage &detections,
                                Detection_T &current_detection,
                                const uint16_t cluster_id,
                                const uint8_t min_cluster_points,
                                const float cluster_radius,
                                const float historical_num_neighbors_forgetting_factor);
}
#endif
