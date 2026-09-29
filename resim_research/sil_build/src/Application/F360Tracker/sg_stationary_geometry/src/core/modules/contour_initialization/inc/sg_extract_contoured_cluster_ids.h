/*=============================================================================================*\
* FILE: sg_extract_contoured_cluster_ids.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for extract_cluster_ids and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_EXTRACT_CLUSTERS_IDS_H
#define SG_EXTRACT_CLUSTERS_IDS_H

#include <array>

#include "sg_contour_storage.h"

namespace sg
{
   /**
    * @brief   Extracts contoured cluster IDs from contours. In other words, each cluster that
    *          contains at least on contour will be put (its ID) to the contoured_cluster_ids
    *          array
    *
    * @param   contours
    * @param   contoured_cluster_ids
    *
    * @return  number of contoured clusters (defines how many valid elements
    * @return  contoured_cluster_ids array contains.
    **/
   uint16_t extract_contoured_cluster_ids(const ContourStorage &contours,
                                          std::array<uint16_t, SG_MAX_NUM_CONTOURS> &contoured_cluster_ids);
}
#endif
