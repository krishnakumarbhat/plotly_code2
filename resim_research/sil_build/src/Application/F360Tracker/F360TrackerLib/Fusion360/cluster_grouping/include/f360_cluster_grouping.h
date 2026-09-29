/*===================================================================================*\
* FILE: f360_cluster_grouping.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This is the main function for grouping of clusters. During grouping, dealiasing
*   possibilities are evaluated and after confirmation, executed.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_CLUSTER_GROUPING_H
#define F360_CLUSTER_GROUPING_H

#include "f360_globals.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "f360_detection_props.h"
#include "f360_detection_hist.h"
#include "f360_cluster.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Cluster_Grouping(
      const F360_Globals_T & globals,
      const F360_Calibrations_T &calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T & timing_info
   );
}

#endif
