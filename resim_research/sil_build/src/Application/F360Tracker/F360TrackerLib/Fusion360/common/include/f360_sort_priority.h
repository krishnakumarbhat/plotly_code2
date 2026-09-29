#ifndef F360_SORT_PRIORITY_H
#define F360_SORT_PRIORITY_H
/*===================================================================================*\
* FILE: f360_sort_priority.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Sort_Priority_With_New_Track and Quick_Sort_Track_Priority.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_reuse.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   void Sort_Priority_With_New_Track(
      F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T * const p_new_track);

   void Quick_Sort_Track_Priority(
      F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);
}
#endif


