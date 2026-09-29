#ifndef F360_POPULATE_INTERNAL_CLUSTERS_LOG_H
#define F360_POPULATE_INTERNAL_CLUSTERS_LOG_H
/*===================================================================================*\
* FILE:  f360_populate_internal_clusters_log.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/
#include "f360_internal_cluster.h"
#include "f360_cluster.h"
#include "f360_tracker_info.h"
#include "T360_Types.h"

namespace f360_variant_A
{
   void Populate_Internal_Clusters_Data(F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Tracker_Info_T& tracker_info, const F360_Internal_Cluster_T(&cluster_log)[NUMBER_OF_CLUSTERS]);

   void Populate_Internal_Clusters_Log_Data(F360_Internal_Cluster_T(&cluster_log)[NUMBER_OF_CLUSTERS],
      const F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS], const int16_t num_active_clusters,
      const int16_t(&active_cluster_ids)[NUMBER_OF_CLUSTERS]);
}

#endif 
