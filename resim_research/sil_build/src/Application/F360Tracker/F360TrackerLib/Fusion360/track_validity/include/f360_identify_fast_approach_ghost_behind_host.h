/*===================================================================================*\
* FILE:  f360_identify_fast_approach_ghost_behind_host.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations of functions defined in f360_identify_fast_approach_ghost_behind_host.cpp
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#ifndef F360_IDENTIFY_FAST_APPROACH_GHOST_BEHIND_HOST_H
#define F360_IDENTIFY_FAST_APPROACH_GHOST_BEHIND_HOST_H

#include "f360_host.h"
#include "f360_object_track.h"
#include "f360_occlusion_types.h"
#include "f360_radar_sensor.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Identify_Fast_Approach_Ghost_Behind_Host(
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object);
}
#endif
