/*=============================================================================================*\
* FILE: sg_expand_cluster_helpers.cpp
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

#include "sg_expand_cluster_helpers.h"

namespace sg
{

   void add_new_neighbors_to_current_neighbors(NeighborCacheIterators &current_neighbors, const NewNeighborCacheIterators &new_neighbors)
   {
      for (auto &new_neighbor_it : new_neighbors)
      {
         // add only new_neighbors which are not already present in curr_neighbors
         const bool f_not_yet_in_curr_neighbors =
            !std::any_of(current_neighbors.begin(), current_neighbors.end(),
                         [&new_neighbor_it](const auto &curr_neighbor_intern_it)
                         { return (*new_neighbor_it)->unique_id == (*curr_neighbor_intern_it)->unique_id; });

         if (f_not_yet_in_curr_neighbors && (!current_neighbors.full()))
         {
            (void) current_neighbors.push_back(new_neighbor_it);
            // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
         }
         (void) new_neighbor_it; // MISRA
      }
   }
}
