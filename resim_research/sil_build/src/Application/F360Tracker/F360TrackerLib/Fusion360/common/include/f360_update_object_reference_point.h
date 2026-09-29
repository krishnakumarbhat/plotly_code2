#ifndef F360_UPDATE_OBJECT_REFERENCE_POINT_H
#define F360_UPDATE_OBJECT_REFERENCE_POINT_H
/*===================================================================================*\
* FILE: f360_update_object_reference_point.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declarations of the following functions.
*   Update_Object_Reference_Point()
* 
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   void Update_Object_Reference_Point(
      const float32_t host_dist_rear_axle_to_vcs_m,
      const bool f_update_obj_states,
      const bool f_update_obj_pos_only,
      const bool f_current_object_behind_another_object,
      const F360_Calibrations_T& calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& obj);
}
#endif
