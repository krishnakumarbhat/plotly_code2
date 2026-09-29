/*=============================================================================================*\
* FILE: sg_dbscan.h
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

#ifndef SG_DBSCAN_H
#define SG_DBSCAN_H

#include <bitset>

#include "sg_constants.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief    clusters detections writing the results into cluster_ids array, but doesn't
    *           update any information inside DetectionStorage itself
    *
    * @param[in]         current_cluster_ptr - pointer to current cluster
    * @param[in]         cluster_radius - radius calibration parameter being used for clustering
    * @param[in]         min_cluster_points - minimal number of points that are able to form a cluster
    *
    * @return   N/A
    **/
   void dbscan(const Cluster *const current_cluster_ptr, const float cluster_radius, const uint8_t min_cluster_points);
}
#endif
