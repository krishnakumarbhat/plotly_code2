/*===================================================================================*\
* FILE:  f360_object_motion_classification.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation of Object_Motion_Classification() function.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#ifndef OBJECT_MOTION_CLASSIFICATION_H
#define OBJECT_MOTION_CLASSIFICATION_H

#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_tracker_info.h"
#include "f360_globals.h"
#include "f360_host.h"
#include "f360_detection_props.h"
#include "f360_occlusion_types.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   void Object_Motion_Classification(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T& host,
      const F360_Globals_T& globals,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T& calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS]);
}
#endif
