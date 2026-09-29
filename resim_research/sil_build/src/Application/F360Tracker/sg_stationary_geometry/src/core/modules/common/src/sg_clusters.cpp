/*===================================================================================*\
* FILE: sg_clusters.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definitions of Clusters member functions.
*
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "sg_clusters.h"

namespace sg
{
   Clusters::Clusters()
   {
      // construct new cluster with default id = INVALID_CLUSTER_ID
      (void) m_clusters.push_back(Cluster{}); // Push 1-st cluster for unclustered detections
   }

   ClusterList::iterator Clusters::create_new()
   {
      const auto cluster_iter = m_clusters.push_back(Cluster{});

      if (cluster_iter != m_clusters.end())
      {
         cluster_iter->m_unique_id = m_cluster_id_handler.get_id();
      }

      return cluster_iter;
   }

   void Clusters::clear()
   {
      (void) m_cluster_id_handler.reset();
      m_clusters.clear();
      (void) m_clusters.push_back(Cluster{}); // Push 1-st cluster for unclustered detections
   }

   ClusterList::iterator Clusters::erase(const ClusterList::iterator &cluster_it)
   {
      ClusterList::iterator return_it{std::next(cluster_it)};
      if ((cluster_it != m_clusters.begin()) && cluster_it->size() == 0U)
      {
         const auto f_cluster_id_returned = m_cluster_id_handler.return_id(cluster_it->unique_id());
         if (f_cluster_id_returned)
         {
            return_it = m_clusters.erase(cluster_it);
         }
         else
         {
            // Attempting to return cluster id that doesn't exist in Clusters
            // this indicates inappropriate use of this function or corrupted information inside DetectionStorage
            assert(false);
         }
      }
      else
      {
         // Cluster gathering unclustered detections should never be removed
         // or attempting to remove cluster that still contains detections; detections should
         // be removed from a cluster first before removing cluster itself since this class is
         // not resposible for releasing detection unique_id
         assert(false);
      }

      return return_it;
   }
}
