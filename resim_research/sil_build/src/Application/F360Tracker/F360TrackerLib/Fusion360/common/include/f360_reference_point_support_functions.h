#ifndef F360_REFERENCE_POINT_SUPPORT_FUNCTIONS_H
#define F360_REFERENCE_POINT_SUPPORT_FUNCTIONS_H
/*===================================================================================*\
* FILE: f360_reference_point_support_functions.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declarations of support functions related to handling of 
*   object reference point.
* 
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_reference_point.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"
#include "f360_point.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
   bool Is_Ref_Point_In_Sensors_FOV(
      const F360_Reference_Point_T& ref_point,
      const F360_Object_Track_T& object_track,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals);

   bool Is_Point_Inside_FOV(
      const Point& point_vcs,
      const F360_Radar_Sensor_T& sensor,
      const float32_t(&left_fov_normal)[2],
      const float32_t(&right_fov_normal)[2]);

   Point Get_Reference_Point_Pos_In_TCS(
      const F360_Reference_Point_T ref_point,
      const float32_t bbox_length,
      const float32_t bbox_width);

   void Get_Distances_From_Ref_Point_To_Obj_Sides(
      const F360_Object_Track_T& obj,
      float32_t& len1,
      float32_t& len2,
      float32_t& wid1,
      float32_t& wid2);

   void Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(
      const F360_Object_Track_T& obj,
      float32_t(&tcs_vec_from_center_rear_to_ref_pnt)[2]);
}
#endif
