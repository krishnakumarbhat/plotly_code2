/*===================================================================================*\
* FILE: f360_cancel_new_updated_trk_overlapping_confirmed_trks.h
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
*   This file contains Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks() declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): cancelNewUpdatedTrkOverlappingConfirmedTrks.m
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
#ifndef F360_CANCEL_NEW_UPDATED_TRK_OVERLAPPING_CONFIRMED_TRKS_H
#define F360_CANCEL_NEW_UPDATED_TRK_OVERLAPPING_CONFIRMED_TRKS_H

#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_detection_props.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks(
      const F360_Calibrations_T & calib,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T & timing_info);
}
#endif


