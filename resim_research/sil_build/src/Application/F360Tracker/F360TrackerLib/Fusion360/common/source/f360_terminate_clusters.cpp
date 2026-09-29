/*===================================================================================*\
* FILE: f360_terminate_clusters.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*  This file contains function definition of Terminate_Clusters()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "f360_clear_cluster.h"
#include "f360_kill_cluster.h"
#include "f360_terminate_clusters.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Terminate_Clusters()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS] - Clusters
   * F360_Detection_Hist_T & det_hist - Historical detection structure
   * F360_Tracker_Info_T & tracker_info - Tracker info
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function kills all clusters marked with f_to_be_killed
   *
   \*===========================================================================*/
   void Terminate_Clusters(
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T & det_hist,
      F360_Tracker_Info_T & tracker_info)
   {
      for (int16_t i = tracker_info.num_active_clusters - 1; i >= 0; i--)
      {
         const int16_t cluster_idx = tracker_info.active_cluster_ids[i] - 1;

         if (clusters[cluster_idx].f_to_be_killed)
         {
            kill_cluster(clusters[cluster_idx], det_hist, tracker_info);
         }
      }
   }
}
