/*===================================================================================*\
* FILE: f360_merge_two_clusters.h
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
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
*   This file contains function signature of merge two clusters function
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
#ifndef F360_MERGE_TWO_CLUSTERS_H
#define F360_MERGE_TWO_CLUSTERS_H

#include "f360_reuse.h"
#include "f360_cluster.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_detection_hist.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Merge_Two_Clusters(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      const int16_t cluster_id_1,
      const int16_t cluster_id_2,
      const float32_t rngrate_interval_width_1,
      const float32_t rngrate_interval_width_2,
      const float32_t interval_1,
      const float32_t interval_2,
      const float32_t k_max_dealiased_range_rate_diff);
}
#endif
