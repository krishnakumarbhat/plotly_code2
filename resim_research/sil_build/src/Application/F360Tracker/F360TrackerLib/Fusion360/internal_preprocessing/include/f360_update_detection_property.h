/*===========================================================================*\
* FILE: f360_update_detection_property.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of Update_Detection_Property() and helper functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef UPDATE_DETECTION_PROP_H
#define UPDATE_DETECTION_PROP_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_calibrations.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_radar_sensor_props.h"
#include "f360_host.h"
#include "f360_host_props.h"
#include "f360_tracker_info.h"
#include "f360_static_env_poly_types.h"
#include "f360_timing_info.h"
#include "f360_mark_trailer_detections.h"

namespace f360_variant_A
{
   void Update_Detection_Property(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Host_T &host,
      const F360_Calibrations_T &calib,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Tracker_Info_T &tracker_info,
      F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T &timing_info
   );

   void Complex_Detections_Processing(
      const F360_Calibrations_T &calib,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Host_T &host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T &tracker_info,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T &timing_info
   );

   void Basic_Detections_Processing(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Host_T& host,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calib,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS]
   );
}
#endif


