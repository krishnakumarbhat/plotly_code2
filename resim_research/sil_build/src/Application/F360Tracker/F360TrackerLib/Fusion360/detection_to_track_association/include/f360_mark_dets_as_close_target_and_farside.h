/*===================================================================================*\
* FILE: f360_mark_dets_as_close_target_and_farside.h
*====================================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations of Mark_Dets_As_Close_Target_And_Farside() and related
* support functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_MARK_DETS_AS_CLOSE_TARGET_AND_FARSIDE_H
#define F360_MARK_DETS_AS_CLOSE_TARGET_AND_FARSIDE_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_calibrations.h"
#include "f360_bounding_box.h"
#include "f360_object_sides.h"

namespace f360_variant_A
{
   void Mark_Dets_As_Close_Target_And_Farside(
      const uint32_t num_dets,
      F360_Object_Track_T & object_track,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T & calibrations);

   void Mark_Detections_As_Close_Target(
      const uint32_t num_dets,
      const BoundingBox & ct_ext_box_on_farside,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]);

   void Farside_Countermeasure(
      F360_Object_Track_T & object_track,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const BoundingBox & fcm_ext_box_on_nearby_side);

   bool Is_Obj_Relevant_For_Close_Target_And_Farside(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calibs,
      const F360_Object_Sides_T closest_orth_edge);

   void Compute_Ext_Bounding_Boxes(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calibs,
      const F360_Object_Sides_T closest_orth_edge,
      BoundingBox & fcm_ext_box_on_nearby_side,
      BoundingBox & ct_ext_box_on_farside);

   bool Is_Det_Relevant_For_Close_Target(
      const F360_Detection_Props_T & det_prop);
}
#endif
