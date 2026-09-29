/*===================================================================================*\
* FILE: f360_update_relative_timestamps.h
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

#ifndef UPDATE_RELATIVE_TIMESTAMPS_H
#define UPDATE_RELATIVE_TIMESTAMPS_H

#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_cluster.h"
#include "f360_detection_hist.h"
namespace f360_variant_A
{
   void Update_Relative_Timestamps(
      const float32_t elapsed_time_s,
      const F360_Tracker_Info_T & p_tracker_info,
      F360_Object_Track_T (&obj_trks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T & p_det_hist);
}

#endif
