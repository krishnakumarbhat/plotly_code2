/*===================================================================================*\
* FILE: f360_kill_obj_trk.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains function signature of Kill_Obj_Trk()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): killObjTrk.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*      ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*      ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef KILL_OBJ_TRK_H
#define KILL_OBJ_TRK_H

#include "f360_reuse.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Kill_Obj_Trk(
      const int32_t obj_trk_id,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info);

   void Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props(
      const int32_t obj_trk_id,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);
}
#endif
