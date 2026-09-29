/*===========================================================================*\
* FILE: f360_split_objects_in_orth_direction.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Split_Objects_In_Orth_Direction()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_SPLIT_OBJECTS_IN_ORTH_DIRECTION_H
#define F360_SPLIT_OBJECTS_IN_ORTH_DIRECTION_H

#include "f360_calibrations.h"
#include "f360_static_env_poly_types.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_detection_props.h"
#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"
#include "f360_split_type.h"

namespace f360_variant_A
{
   void Split_Objects_In_Orth_Direction(
      const F360_Host_T & host,
      const F360_Calibrations_T& calibs,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS]);

   void Find_Objects_To_Split(
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      uint32_t& nr_objects_to_split,
      uint32_t(&obj_idx_to_split)[NUMBER_OF_OBJECT_TRACKS]);

   bool Is_Object_Valid_For_Split(
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& object);

   int32_t Split_Single_Object_In_Ortho_Direction(
      const F360_Host_T & host,
      const F360_Calibrations_T& calibs,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& object_to_split,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS]);

   void Fill_New_Object_Properties(
      const F360_Object_Track_T& object_to_split,
      const int32_t new_obj_id,
      const uint32_t new_unique_id,
      F360_Object_Track_T& new_obj);

   void Re_Associate_Detections(
      const F360_Host_T& host,
      const F360_Calibrations_T& calibs,
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object_to_split,
      F360_Object_Track_T& new_object);

   void Find_Re_Associated_Detections_Ids(
      const F360_Object_Track_T& object_to_split,
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t& nr_dets_org_obj,
      uint32_t(&org_obj_det_ids)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& nr_dets_new_obj,
      uint32_t(&new_obj_det_ids)[MAX_DETS_IN_OBJ_TRK]);

   void Re_Associate_Detections_Single_Object(
      const F360_Host_T& host,
      const F360_Calibrations_T& calibs,
      const uint32_t nr_dets,
      const uint32_t(&obj_det_ids)[MAX_DETS_IN_OBJ_TRK],
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object);

   void Adapt_Objects_Properties_After_Orth_Split(
      const F360_Host_T & host,
      const F360_Calibrations_T& calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& org_object,
      F360_Object_Track_T& new_object);
}
#endif
