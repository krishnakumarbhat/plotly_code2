/*===================================================================================*\
* FILE: f360_are_two_objects_preconditions_ok_for_merge.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Are_Two_Objects_Preconditions_OK_For_Merge function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None. 
*
\*===================================================================================*/
#ifndef F360_ARE_TWO_OBJECTS_PRECONDITIONS_OK_FOR_MERGE_H
#define F360_ARE_TWO_OBJECTS_PRECONDITIONS_OK_FOR_MERGE_H

#include "f360_globals.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   bool Are_Two_Objects_Preconditions_OK_For_Merge(
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& first_object,
      const F360_Object_Track_T& second_object,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      float32_t & coarse_gate_score);

   bool Are_Merging_Coarse_Gate_Conditions_Met(
      const F360_Object_Track_T & first_object,
      const F360_Object_Track_T & second_object,
      const float32_t heading_gate,
      const float32_t speed_gate,
      const float32_t curvature_gate,
      const float32_t coarse_score_gate,
      float32_t & coarse_gate_score);

   bool Merge_Metal_To_Metal_Test(
      const F360_Object_Track_T & first_object,
      const F360_Object_Track_T & second_object,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T & calib);
}
#endif
