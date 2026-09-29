/*===================================================================================*\
* FILE f360_object_based_water_spray_detector.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION
* This file contains declarations of Object_Water_Spray_Detector() and related
* support functions.
*
* Applicable Standards (in order of precedence highest first)
*     ESGW_4-2_PE-SWX_00-01-A01_EN, APTIV C++ Coding Standards [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN APTIV C Coding Standards [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_OBJECT_BASED_WATER_SPRAY_DETECTOR_H
#define F360_OBJECT_BASED_WATER_SPRAY_DETECTOR_H

#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"


namespace f360_variant_A
{
   void Detect_Water_Spray_From_Objects(
      const F360_Tracker_Info_T & tracker_info,
      const F360_Calibrations_T & calibs,
      const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_det_list,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T (&dets_props)[MAX_NUMBER_OF_DETECTIONS]
   );
}
#endif
