/*===================================================================================*\
* FILE: f360_visibility_info.h
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential – Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of common functions for the determination of visibility
* of points depending on their location wrt FOV, CFOV, occlusion, etc.
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/
#ifndef F360_VISIBILITY_INFO_H
#define F360_VISIBILITY_INFO_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   bool Can_Object_Be_Detected_By_Sensors(
      const F360_Object_Track_T & obj,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]);

   bool Is_VCS_Point_Within_Current_FOV_Of_Sensor(
      const Point& point_pos,
      const F360_Radar_Sensor_T& sensor,
      const F360_Det_Look_ID_T look_id);

   bool Is_Rel_Point_Within_FOV_Azim(
      const float32_t rel_posn_lon,
      const float32_t rel_posn_lat,
      const F360_Radar_Sensor_T& sensor,
      const F360_Det_Range_Type_T range_type);

   bool Is_Rel_Point_Below_Given_Range_Limit(
      const float32_t rel_posn_lon,
      const float32_t rel_posn_lat,
      const float32_t range_limit);

   bool Is_Object_In_Side_Blindzone(
       const F360_Object_Track_T& object_track,
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       const F360_Globals_T& globals);
}
#endif
