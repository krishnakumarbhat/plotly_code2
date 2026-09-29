/*===================================================================================*\
* FILE:  f360_detect_multipath.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations of functions defined in f360_detect_multipath.cpp
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#ifndef DETECT_MULTIPATH_H
#define DETECT_MULTIPATH_H

#include "f360_constants.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_multipath_detector.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   bool Check_If_Object_Is_MP_For_Any_Sensor(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T& object,
      Multipath_Detector &mp_detector);

   bool Detect_And_Mark_Multipath_Object(
      const F360_Host_T & host,
      const F360_Tracker_Info_T & tracker_info,
      const F360_Calibrations_T & calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Object_Track_T& object,
      Multipath_Detector &mp_detector);
}

#endif
