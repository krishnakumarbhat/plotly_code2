/*===================================================================================*\
* FILE: f360_sorted_clusters_mgmt.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declarations for functions to sort clusters
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_SORTED_CLUSTERS_MGMT_H
#define F360_SORTED_CLUSTERS_MGMT_H

#include "f360_reuse.h"
#include "f360_tracker_info.h"
#include "f360_cluster.h"

namespace f360_variant_A
{
   void Sorted_Clusters_Re_Sort(
      F360_Tracker_Info_T & tracker_info);

   void Sorted_Clusters_Update_List(
      F360_Tracker_Info_T & tracker_info);

   void Sorted_Clusters_Insert(
      F360_Tracker_Info_T & tracker_info,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Cluster_T & new_cluster,
      const int16_t initial_guess_id);

   void Sorted_Clusters_Insert_Batch(
      F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      float32_t(&vcs_long)[MAX_TRACKER_POSN_CLUSTERS],
      int16_t(&new_cluster_ids)[MAX_TRACKER_POSN_CLUSTERS],
      const uint16_t num_clusters);

   void Sorted_Clusters_Remove(
      F360_Tracker_Info_T & tracker_info,
      const F360_Cluster_T & dead_cluster);
}
#endif
