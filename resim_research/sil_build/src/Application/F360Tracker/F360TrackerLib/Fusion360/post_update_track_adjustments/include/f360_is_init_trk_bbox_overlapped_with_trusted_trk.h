/*===================================================================================*\
* FILE: f360_is_init_trk_bbox_overlapped_with_trusted_trk.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains function signature of Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk ()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): isInitTrkBBoxOverlappedWithTrustedTrk.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef IS_INIT_TRK_BBOX_OVERLAPPED_WITH_TRUSTED_TRK_H
#define IS_INIT_TRK_BBOX_OVERLAPPED_WITH_TRUSTED_TRK_H

#include "f360_reuse.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   bool Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk(
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const int32_t initialized_obj_idx,
      const F360_Tracker_Info_T& tracker_info);

   bool Check_If_Init_Object_Should_Be_Killed(
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& initialized_obj,
      const F360_Object_Track_T& obj_to_check
   );
}
#endif

