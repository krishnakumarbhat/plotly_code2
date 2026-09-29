/*===================================================================================*\
* FILE: f360_pre_association_track_management.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains function declaration of Pre_Association_Track_Management()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef PRE_ASSOCIATION_TRACK_MANAGEMENT_H
#define PRE_ASSOCIATION_TRACK_MANAGEMENT_H

#include "f360_timing_info.h"

#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_tracker_info.h"
#include "f360_host.h"
#include "f360_static_env_poly_types.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   void Pre_Association_Track_Management(
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      const F360_Calibrations_T &calibrations,
      const F360_Globals_T& globals,
      const Static_Env_Poly_T (&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T & timing_info
   );
}

#endif
