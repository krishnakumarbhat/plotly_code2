/*===================================================================================*\
* FILE: f360_update_merged_objects_properties.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declarations of functions related to Updating Merged Object Properties.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_UPDATE_MERGED_OBJECTS_PROPERTIES_H
#define F360_UPDATE_MERGED_OBJECTS_PROPERTIES_H

#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_dimensions.h"
#include "f360_static_env_poly_types.h"
#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   void Update_Merged_Objects_Properties(
      const F360_Object_Track_T & object_track_to_kill,
      const F360_Calibrations_T & calibrations,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const Point& merged_object_center_vcs,
      const F360_Dimensions_T& dimensions,
      F360_Object_Track_T& object_track_to_keep);

   void Adjust_Obj_States_After_Merge(
      const F360_Dimensions_T & merged_object_initial_size,
      const F360_Calibrations_T& calibrations,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const Point& merged_object_center_vcs,
      F360_Object_Track_T& object_track_to_keep);
}
#endif
