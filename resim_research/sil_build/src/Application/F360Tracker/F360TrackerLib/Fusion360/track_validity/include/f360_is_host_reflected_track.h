/*===================================================================================*\
* FILE: f360_is_host_reflected_track.h
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains Is_Mirror_Track() function declaration
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_IS_HOST_MIRROR_TRACK_H
#define F360_IS_HOST_MIRROR_TRACK_H

#include "f360_host.h"
#include "f360_object_track.h"
#include "f360_static_env_poly_types.h"
#include"f360_calibrations.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "f360_globals.h"
#include "f360_trailer_manager.h"

namespace f360_variant_A
{
   bool Is_Host_Reflected_Track(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T& host,
      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const int32_t obj_idx,
      const F360_Trailer_Estimator_Output_T& trailer_data,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);

}
#endif
