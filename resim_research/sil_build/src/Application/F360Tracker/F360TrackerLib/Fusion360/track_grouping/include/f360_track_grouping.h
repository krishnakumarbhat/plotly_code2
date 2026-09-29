/*===================================================================================*\
* FILE: f360_track_grouping.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declarations of functions related to track grouping.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_TRACK_GROUPING_H
#define F360_TRACK_GROUPING_H

#include "f360_reuse.h"
#include "f360_detection_hist.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_globals.h"
#include "f360_calibrations.h"
#include "f360_constants.h"
#include "f360_timing_info.h"
#include "f360_detection_props.h"
#include "f360_static_env_poly_types.h"
#include "f360_host.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   void Track_Grouping(

      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T & timing_info);

   void Try_To_Merge_With_Other_Object(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const int32_t idx1,
      const F360_Host_T & host,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T& tracker_info);

}
#endif
