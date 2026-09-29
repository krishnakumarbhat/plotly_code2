#ifndef F360_IDENTIFY_OBJECTS_THAT_ARE_IMMEDIATLY_BEHIND_ANOTHER_OBJECT
#define F360_IDENTIFY_OBJECTS_THAT_ARE_IMMEDIATLY_BEHIND_ANOTHER_OBJECT
/*===================================================================================*\
* FILE: f360_identify_objects_that_are_immediatly_behind_another_object.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declarations of the following functions.
*   Identify_Objects_That_Are_Immediatly_Behind_Another_Object()
*   Check_If_Object_is_Behind_Another_Object()
* 
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_reuse.h"
#include "f360_radar_sensor.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   void Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      const float32_t k_far_away_object_dist_sq_thr,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      bool (&f_object_is_behind_another_object_array)[NUMBER_OF_OBJECT_TRACKS]);

   void Check_If_Object_is_Behind_Another_Object(
      const float32_t k_far_away_object_dist_sq_thr,
      const F360_Tracker_Info_T& tracker_info,
      const int32_t idx1,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      bool (&f_object_is_behind_another_object_array)[NUMBER_OF_OBJECT_TRACKS]);
}
#endif
