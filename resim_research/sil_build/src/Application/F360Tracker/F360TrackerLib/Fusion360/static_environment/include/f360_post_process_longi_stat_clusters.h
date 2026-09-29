/*===========================================================================*\
* FILE: f360_post_process_longi_stat_clusters.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Post_Process_Longi_Stat_Clusters() and related subfunctions
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_POST_PROCESS_LONGI_STAT_CLUSTERS_H
#define F360_POST_PROCESS_LONGI_STAT_CLUSTERS_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_calibrations.h"
#include "f360_longi_stat_cluster.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Post_Process_Longi_Stat_Clusters(
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      const float32_t host_turn_radius,
      uint16_t&nr_valid_clusters,
      F360_Longi_Stat_Cluster_T (&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
   );

   void Merge_Longi_Stat_Clusters(
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      uint16_t&nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
   );

   bool Try_To_Merge_Longi_Stat_Clusters(
      const uint16_t nr_clusters,
      const F360_Longi_Stat_Cluster_T(&clusters)[NR_LONGI_STAT_CLUSTERS],
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      uint16_t& next_idx,
      uint16_t& nr_next_clusters,
      F360_Longi_Stat_Cluster_T(&next_clusters)[NR_LONGI_STAT_CLUSTERS]
   );

   void Merge_Longi_Stat_Cluster_Pair(
      const uint16_t nr_clusters,
      const F360_Longi_Stat_Cluster_T(&clusters)[NR_LONGI_STAT_CLUSTERS],
      const uint16_t primary_cluster_idx,
      const uint16_t secondary_cluster_idx,
      uint16_t& nr_next_clusters,
      F360_Longi_Stat_Cluster_T(&next_clusters)[NR_LONGI_STAT_CLUSTERS]
   );

   void Remove_Overhead_Longi_Stat_Clusters(
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
   );

   bool Check_If_Object_In_Path_Of_Host(
      const F360_Calibrations_T& calibs,
      const float32_t host_turn_radius,
      const F360_Object_Track_T& obj
   );

   void Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(
      const F360_Calibrations_T& calibs,
      const float32_t host_turn_radius,
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
   );
}


#endif
