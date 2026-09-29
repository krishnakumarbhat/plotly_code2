/*=============================================================================================*\
* FILE: sg_extract_contoured_cluster_ids.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for extract_cluster_ids and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_extract_contoured_cluster_ids.h"

#include <algorithm>

namespace sg
{
   uint16_t extract_contoured_cluster_ids(const ContourStorage &contours,
                                          std::array<uint16_t, SG_MAX_NUM_CONTOURS> &contoured_cluster_ids)
   {
      assert(static_cast<uint16_t>(contours.capacity()) == SG_MAX_NUM_CONTOURS); // contoured_cluster_ids must be the same size as
                                                                                 // contours
      uint16_t cnt = 0U;
      for (const auto &contour : contours)
      {
         if (INVALID_CLUSTER_ID != contour.cluster_id)
         {
            if (cnt < SG_MAX_NUM_CONTOURS) // MISRA ("Guarantee that container indices and iterators are within the valid range.")
            {
               contoured_cluster_ids[cnt] = contour.cluster_id;
               cnt++;
            }
         }
      }

      const auto it_cluster_end =
         std::unique(contoured_cluster_ids.begin(), contoured_cluster_ids.begin() + static_cast<std::ptrdiff_t>(cnt));

      const auto num_contoured_clusters = static_cast<uint16_t>(std::distance(contoured_cluster_ids.begin(), it_cluster_end));

      return num_contoured_clusters;
   }
}
