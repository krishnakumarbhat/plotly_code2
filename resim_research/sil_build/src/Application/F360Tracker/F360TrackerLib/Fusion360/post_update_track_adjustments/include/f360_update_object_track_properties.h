/*===================================================================================*\
* FILE: f360_update_object_track_properties.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains Update_Object_Track_Properties() declaration.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_UPDATE_OBJECT_TRACK_PROPERTIES_H
#define F360_UPDATE_OBJECT_TRACK_PROPERTIES_H

#include "f360_calibrations.h"
#include "f360_globals.h"
#include "f360_tracker_info.h"
#include "rspp_detection_list.h"
#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "f360_detection_props.h"
#include "f360_object_track.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Update_Object_Track_Properties(
      const F360_Calibrations_T & calib,
      const F360_Globals_T & globals,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detect_list,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info
      );
}
#endif
