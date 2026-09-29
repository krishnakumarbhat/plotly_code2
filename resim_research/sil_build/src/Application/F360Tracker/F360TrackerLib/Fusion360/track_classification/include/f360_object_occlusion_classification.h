#ifndef OBJECT_OCCLUSION_CLASSIFICATION_H
#define OBJECT_OCCLUSION_CLASSIFICATION_H
/*===================================================================================*\
* FILE:  f360_object_occlusion_classification.h
*====================================================================================
*Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Object_Occlusion_Classification() function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/

#include "f360_occlusion_types.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Object_Occlusion_Classification(
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);
}

#endif
