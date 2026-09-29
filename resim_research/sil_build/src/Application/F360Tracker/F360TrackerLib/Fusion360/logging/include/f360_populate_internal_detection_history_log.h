#ifndef F360_POPULATE_INTERNAL_DETECTION_HISTORY_LOG_H
#define F360_POPULATE_INTERNAL_DETECTION_HISTORY_LOG_H
/*===================================================================================*\
* FILE:  f360_populate_internal_detection_history_log.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/

#include "f360_internal_detection_history.h"
#include "f360_detection_hist.h"
#include "f360_tracker_info.h"
#include "T360_Types.h"

namespace f360_variant_A
{
   void Populate_Internal_Detection_History_Data(F360_Detection_Hist_T& det_hist,
      const F360_Tracker_Info_T&tracker_info,
      const F360_Internal_Detection_Hist_T(&det_hist_data_log)[MAX_NUMBER_OF_HISTORIC_DETECTIONS]);

   void Populate_Internal_Detection_History_Log_Data(
      F360_Internal_Detection_Hist_T(&det_hist_data_log)[MAX_NUMBER_OF_HISTORIC_DETECTIONS],
      const F360_Detection_Hist_T& det_hist);

   void Link_Historical_Detections_To_Clusters(
      const F360_Tracker_Info_T&tracker_info,
      F360_Detection_Hist_T& det_hist,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]);
}

#endif 
