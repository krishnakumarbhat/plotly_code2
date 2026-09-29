/*===================================================================================*\
* FILE: f360_detection_to_track_association.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the declaration of Detection_To_Track_Association().
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DET_TO_TRACK_ASSOCIATION_H
#define DET_TO_TRACK_ASSOCIATION_H

#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"
#include "f360_detection_props.h"
#include "f360_object_track.h"
#include "f360_timing_info.h"
#include "f360_static_env_poly_types.h"

namespace f360_variant_A
{
   void Detection_To_Track_Association(
      const F360_Host_T & host,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T & tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calibrations,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info);
}
#endif
