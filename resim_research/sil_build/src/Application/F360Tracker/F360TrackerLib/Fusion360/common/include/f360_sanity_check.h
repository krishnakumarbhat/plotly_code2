/*===================================================================================*\
* FILE: f360_sanity_check.h
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
*   This is the main function for the vehicle processing module.
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

#ifndef SANITY_CHECK_H
#define SANITY_CHECK_H

#include "f360_reuse.h"
#include "f360_globals.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_detection_hist.h"
#include "f360_cluster.h"
#include "f360_object_track.h"
#include "f360_timing_info.h"
namespace f360_variant_A
{
   bool Sanity_Check(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T& detection_hist,
      const F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T& timing_info
   );
}

#endif
