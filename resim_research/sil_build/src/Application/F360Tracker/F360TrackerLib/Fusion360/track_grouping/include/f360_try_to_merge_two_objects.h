/*===================================================================================*\
* FILE: f360_try_to_merge_two_objects.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declarations of  Try_To_Merge_Two_Objects function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_TRY_TO_MERGE_TWO_OBJECTS_H
#define F360_TRY_TO_MERGE_TWO_OBJECTS_H

#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_tracker_info.h"
#include "f360_globals.h"
#include "f360_calibrations.h"
#include "f360_constants.h"
#include "f360_static_env_poly_types.h"
#include "f360_host.h"
#include "f360_globals.h"
#include "f360_dimensions.h"

namespace f360_variant_A
{
   void Try_To_Merge_Two_Objects(
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const int32_t idx1,
      const int32_t idx2,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T & tracker_info,
      int32_t & kill_idx);

   void Choose_Obj_Idx_To_Be_Kept(
      const F360_Globals_T& globals,
      const int32_t idx1,
      const int32_t idx2,
      const F360_Object_Track_T& obj1,
      const F360_Object_Track_T& obj2,
      int32_t& keep_idx,
      int32_t& kill_idx);

   float32_t Get_Closest_Corner_VCS_Y(
      const BoundingBox& bbox);

   Point Calculate_Merged_Obj_Center_And_Update_Width(
       const F360_Object_Track_T& obj,
       F360_Dimensions_T& dimensions);

   bool Check_If_Detection_Gaps_Support_Merge(
      const F360_Object_Track_T& obj1,
      const F360_Object_Track_T& obj2,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]);

   bool Any_Detection_Within_Merged_Extended_BBox(
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T& kill_obj,
      const F360_Object_Track_T& keep_obj,
      const BoundingBox& merged_bbox);
}
#endif
