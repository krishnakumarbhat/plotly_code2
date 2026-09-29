/*=============================================================================================*\
* FILE: sg_expand_cluster_helpers.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for helper functions used in expand_clusters.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_EXPAND_CLUSTER_HELPERS_H
#define SG_EXPAND_CLUSTER_HELPERS_H

#include <bitset>

#include "sg_clustering_aliases.h"
#include "sg_detection_storage.h"

namespace sg
{

   /**
    * @brief    Move new_neighbors to current neigbors if they are not already there
    *
    * @param[in, out]      current_neighbors
    * @param[in]           new_neighbors
    *
    * @return              N/A
    **/
   void add_new_neighbors_to_current_neighbors(NeighborCacheIterators &current_neighbors,
                                               const NewNeighborCacheIterators &new_neighbors);
}
#endif
