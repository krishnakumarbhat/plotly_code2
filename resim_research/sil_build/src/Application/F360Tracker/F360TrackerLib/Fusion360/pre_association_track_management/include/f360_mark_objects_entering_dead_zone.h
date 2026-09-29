/*===================================================================================*\
* FILE: f360_mark_objects_entering_dead_zone.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Mark_Objects_Enterning_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef MARK_OBJECTS_ENTERING_DEAD_ZONE_H
#define MARK_OBJECTS_ENTERING_DEAD_ZONE_H

#include "f360_host.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Mark_Objects_Enterning_Dead_Zone(
      const F360_Host_T &host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calib,
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]);
}

#endif
