#ifndef F360_PRIORITIZE_CLUSTERS_H
#define F360_PRIORITIZE_CLUSTERS_H

/*===========================================================================*\
* FILE: f360_prioritize_clusters.h
*============================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Prioritize_Clusters()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [September 06, 2020]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_detection_hist.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_cluster.h"

namespace f360_variant_A
{
   void Prioritize_Clusters(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      int32_t(&prioritized_cluster_ids)[NUMBER_OF_CLUSTERS],
      uint32_t& num_clusters);
}

#endif
