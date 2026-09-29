/*===================================================================================*\
* FILE: f360_move_dets_from_killed_to_kept_object.h
*====================================================================================
* Copyright 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential  Restricted Aptiv information. Do not disclose.
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
*   This file contains declaration of Move_Dets_From_Killed_To_Kept_Object(). 
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
*      ESGW_4-2_PE-SWx_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

#ifndef F360_MOVE_DETS_FROM_KILLED_TO_KEPT_OBJECT_H
#define F360_MOVE_DETS_FROM_KILLED_TO_KEPT_OBJECT_H

#include "f360_host.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_constants.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Move_Dets_From_Killed_To_Kept_Object(
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calib,    
      const F360_Object_Track_T & object_track_to_kill,
      F360_Object_Track_T & object_track_to_keep,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]);
}
#endif
