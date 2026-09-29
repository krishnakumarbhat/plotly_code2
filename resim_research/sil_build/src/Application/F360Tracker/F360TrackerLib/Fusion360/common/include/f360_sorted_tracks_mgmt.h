/*===================================================================================*\
* FILE: f360_sorted_tracks_mgmt.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Sorted_Tracks_Update_List and helper functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef SORTED_TRACKS_MGMT_H
#define SORTED_TRACKS_MGMT_H

#include "f360_tracker_info.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   void Sorted_Tracks_Re_Sort(
      F360_Tracker_Info_T & tracker_info);

   void sorted_tracks_remove(
      F360_Tracker_Info_T & tracker_info,
      const F360_Object_Track_T & dead_track);

   void Sorted_Tracks_Insert(
      F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T* const p_new_track);
}
#endif
