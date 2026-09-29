/*===================================================================================*\
* FILE:  f360_kill_coasted_tracks.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contians declaration of function for killing coasting tracks.
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef KILL_COASTED_TRACKS_H
#define KILL_COASTED_TRACKS_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_timing_info.h"
#include "f360_calibrations.h"
#include "f360_globals.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   void  Kill_Coasted_Tracks(
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T &tracker_info,
      F360_TRKR_TIMING_INFO_T & timing_info,
      const F360_Calibrations_T & calibrations,
      const F360_Globals_T& globals,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]);

   bool Is_Non_Movable_Obj_Outside_Front_Only_FOV(
      const F360_Object_Track_T& object_track,
      const F360_Globals_T& globals,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]);
}

#endif
