/*===========================================================================*\
* FILE: f360_downselect_longi_stat_clusters.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Downselect_Longi_Stat_Clusters() and related subfunctions
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_DOWNSELECT_LONGI_STAT_CURVES_H
#define F360_DOWNSELECT_LONGI_STAT_CURVES_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_calibrations.h"
#include "f360_longi_stat_cluster.h"

namespace f360_variant_A
{
   void Downselect_Longi_Stat_Clusters(
      const uint16_t nr_valid_clusters,
      const F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS],
      const F360_Calibrations_T& calibs,
      uint16_t& nr_downselected_clusters,
      F360_Longi_Stat_Cluster_T(&downselected_clusters)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
   );

   float32_t Calc_Longi_Stat_Cluster_Score(
      const F360_Longi_Stat_Cluster_T& lsc_cluster,
      const F360_Calibrations_T& calibs
   );

   void Find_Special_Longi_Clusters(
      const uint16_t nr_valid_clusters,
      const F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS],
      float32_t(&cluster_score_array)[NR_LONGI_STAT_CLUSTERS]);
}
#endif
