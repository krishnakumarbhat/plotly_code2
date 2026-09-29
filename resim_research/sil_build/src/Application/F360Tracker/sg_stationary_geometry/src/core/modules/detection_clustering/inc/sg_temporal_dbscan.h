/*=============================================================================================*\
* FILE: sg_temporal_dbscan.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for temporal_dbscan and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_TEMPORAL_DBSCAN_H
#define SG_TEMPORAL_DBSCAN_H

#include <bitset>

#include "sg_constants.h"
#include "sg_detection_storage.h"

namespace sg
{

   /**
    * @brief    assign detections to clusters
    *           this function only updates cluster_ids and num_new_neighbors arrays, but doesn't
    *           update any information inside DetectionStorage itself
    *
    * @param[in, out]    detections - input detection information (changes only temporary variables like f_dbscan_visited and
    *f_dbscan_core)
    * @param[in]         cluster_radius - radius calibration parameter being used for clustering
    * @param[in]         historical_num_neighbors_forgetting_factor - filter weight for the historical number of neighbors of a
    *detection
    * @param[in]         min_cluster_points - minimal number of points that are able to form a cluster
    *
    * @return   N/A
    **/
   void temporal_dbscan(DetectionStorage &detections,
                        const float cluster_radius,
                        const float historical_num_neighbors_forgetting_factor,
                        const uint8_t min_cluster_points);

   /**
    * @brief    Update a detection's number of neighbors
    *
    * @param[in, out]    detection - detection to be updated
    * @param[in]         historical_num_neighbors_forgetting_factor - calibration value for weighting previous number of neighbors
    *
    * @return         N/A
    **/
   void update_neighbors_history(Detection_T &detection, const float historical_num_neighbors_forgetting_factor);

}
#endif
