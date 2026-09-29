/*===================================================================================*\
* FILE: f360_find_detection_inliers.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Find_Detection_Inliers().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_FIND_DETECTION_INLIERS_H
#define F360_FIND_DETECTION_INLIERS_H

#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Find_Detection_Inliers(
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calibrations,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);

   void Find_Detection_Inliers_For_Single_Object(
      const F360_Host_T& host,
      const F360_Calibrations_T& calibrations,
      F360_Object_Track_T& object,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);
    
   void Find_Valid_Detections(
      const F360_Object_Track_T & object_track,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t (&valid_det_indices)[MAX_DETS_IN_OBJ_TRK],
      uint32_t & n_valid_dets);

   void Select_Detection_Inliers(
      const F360_Host_T& host,
      const F360_Calibrations_T& calibrations,
      const uint32_t(&valid_det_indices)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t n_valid_dets,
      float32_t(&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK],
      F360_Object_Track_T& object_track,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);

   void Calculate_Abs_Range_Rate_Diffs(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t (&valid_det_indices)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t n_valid_dets,
      float32_t (&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK]);

   void Update_Historic_Range_Rate_Error_Statistics(
      const F360_Calibrations_T& calibrations,
      const float32_t (&abs_rdot_diffs)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t n_valid_dets,
      F360_Object_Track_T& object_track);

   void Clear_Detection_Inliers_To_Object(
      F360_Object_Track_T & object_track,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);

   bool Is_Obj_In_Curvi_Roi(
       const F360_Host_T& host,
       const F360_Object_Track_T& object_track);

   bool Is_Obj_In_Rect_Roi(
       const F360_Object_Track_T& object_track);
}
#endif
