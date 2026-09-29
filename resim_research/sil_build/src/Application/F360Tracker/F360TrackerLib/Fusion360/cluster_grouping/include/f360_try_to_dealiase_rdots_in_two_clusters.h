/*===================================================================================*\
* FILE: f360_try_to_dealiase_rdots_in_two_clusters.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): 
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_TRY_TO_DEALIAS_CLUSTERS_H
#define F360_TRY_TO_DEALIAS_CLUSTERS_H

#include "f360_reuse.h"
#include "f360_cluster.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "f360_globals.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_detection_hist.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   bool Try_To_Dealiase_Rdots_In_Two_Clusters(
      const F360_Cluster_T& cluster_older,
      const F360_Cluster_T& cluster_newer,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T& detections_hist,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Globals_T& globals,
      const F360_Calibrations_T &calib,
      float32_t& rngrate_interval_width_older,
      float32_t& rngrate_interval_width_newer,
      float32_t& interval_older,
      float32_t& interval_newer);
}
#endif
