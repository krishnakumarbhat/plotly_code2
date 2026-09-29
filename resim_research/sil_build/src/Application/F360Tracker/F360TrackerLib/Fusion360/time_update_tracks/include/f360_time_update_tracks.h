/*===================================================================================*\
* FILE:  f360_time_update_tracks.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file constains declaration of main function for performing time update of objects
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef TIME_UPDATING_TRACKS_H
#define TIME_UPDATING_TRACKS_H

#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_host_props.h"
#include "f360_radar_sensor.h"
#include "f360_object_track.h"
#include "f360_detection_hist.h"
#include "f360_tracker_info.h"
#include "f360_timing_info.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   void Time_Update_Tracks(
      const F360_Calibrations_T & calibrations,
      const float32_t elapsed_time_s,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_Props_T & host_props,
      const F360_Globals_T& globals,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info,
      F360_TRKR_TIMING_INFO_T & timing_info);
}

#endif
