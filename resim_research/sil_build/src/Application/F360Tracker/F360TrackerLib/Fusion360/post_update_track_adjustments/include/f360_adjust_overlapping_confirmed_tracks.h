/*===================================================================================*\
* FILE: f360_adjust_overlapping_confirmed_tracks.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations of Adjust_Overlapping_Confirmed_Tracks().
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_ADJUST_OVERLAPPING_CONFIRMED_TRACKS_H
#define F360_ADJUST_OVERLAPPING_CONFIRMED_TRACKS_H

#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_timing_info.h"
namespace f360_variant_A
{
   void Adjust_Overlapping_Confirmed_Tracks(
      const F360_Calibrations_T & calib,
      const F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info);
}
#endif
