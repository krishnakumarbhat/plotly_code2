#ifndef F360_TERMINATE_CLUSTERS_H
#define F360_TERMINATE_CLUSTERS_H
/*===================================================================================*\
* FILE: f360_terminate_clusters.h
*====================================================================================
* Copyright © 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*----------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains function signature of Terminate_Clusters()
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Requirements Document(s):
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/

#include "f360_tracker_info.h"
#include "f360_detection_hist.h"
namespace f360_variant_A
{
   void Terminate_Clusters(
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T & det_hist,
      F360_Tracker_Info_T & tracker_info
   );
}
#endif
