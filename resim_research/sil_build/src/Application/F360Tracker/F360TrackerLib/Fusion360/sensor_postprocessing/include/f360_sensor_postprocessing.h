/*===================================================================================*\
* FILE: f360_sensor_postprocessing.h
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
*
*
* ABBREVIATIONS:
*   OTG	Over-The-ground
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*
*
* DEVIATIONS FROM STANDARDS:
*
*
\*==========================================================================================*/

#ifndef SENSOR_POSTPROCESSING_H
#define SENSOR_POSTPROCESSING_H

#include "f360_tracker_info.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_detection_hist.h"
#include "f360_radar_sensor.h"
#include "f360_cluster.h"
#include "f360_timing_info.h"
#include "f360_host.h"


namespace f360_variant_A
{
   void Sensor_Postprocessing(
      const F360_Host_T& host_props,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Hist_T& detection_hist,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T& timing_info
      );
}

#endif
