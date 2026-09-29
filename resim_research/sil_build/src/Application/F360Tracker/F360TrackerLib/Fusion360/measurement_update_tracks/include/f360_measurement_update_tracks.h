/*===================================================================================*\
* FILE: f360_measurement_update_tracks.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains function declaration of Measurement_Update_Tracks()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef MEASUREMENT_UPDATE_TRACKS_H
#define MEASUREMENT_UPDATE_TRACKS_H

#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_host_props.h"
#include "f360_tracker_info.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Measurement_Update_Tracks(
      const F360_Calibrations_T & calibrations,
      const F360_Host_T & host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info);
}
#endif
