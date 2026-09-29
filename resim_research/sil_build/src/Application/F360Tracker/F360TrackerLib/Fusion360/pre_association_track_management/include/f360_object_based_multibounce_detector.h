/*===================================================================================*\
* FILE: f360_object_based_multibounce_detector.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains declarations of functions related to object based 
*    multibounce detector.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_OBJECT_BASED_MULTIBOUNCE_DETECTOR_H
#define F360_OBJECT_BASED_MULTIBOUNCE_DETECTOR_H

#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"


namespace f360_variant_A
{
   void Check_Dets_Against_Multibounces(
      const F360_Object_Track_T &obj_track,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const bool(&relevant_sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calibs,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   );
}
#endif
