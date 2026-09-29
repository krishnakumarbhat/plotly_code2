/*===========================================================================*\
* FILE: f360_allocate_id_for_initialized_object.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of Allocate_Id_For_Initialized_Object();
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_allocate_id_for_initialized_object.h"
#include "f360_kill_obj_trk.h"
#include "f360_clear_det_assoc_obj_props.h"

/*===========================================================================*\
* FUNCTION: Allocate_Id_For_Initialized_Object
* ===========================================================================
* RETURN VALUE:
* int32_t new_id
*
* PARAMETERS:
* F360_Tracker_Info_T & tracker_info
* F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
* F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Return next free ID which can be used by new initialized object track. If number of 
* active tracks is saturated - kill track with lowest priority and return its ID.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/

namespace f360_variant_A
{
   int32_t Allocate_Id_For_Initialized_Object(
      F360_Tracker_Info_T & tracker_info, 
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      // Objects saturation case - kill lowest priority track.
      if ((tracker_info.num_active_objs >= static_cast<int32_t>(tracker_info.variant.num_tracks)) && (tracker_info.p_lowest_priority_track != NULL))
      {
         // Clear associated detection's properties. Otherwise, detections will incorrectly have the object ID of the newly created object.
         Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props(tracker_info.p_lowest_priority_track->id, object_tracks, tracker_info, det_props);
      }

      //  Get new object track ID from start of inactive IDs list (to re-use oldest IDs first). 
      const int32_t new_id = tracker_info.inactive_obj_ids[0];

      //  Shift inactive list one position towards start of array
      const int32_t num_inactive = static_cast<int32_t>(tracker_info.variant.num_tracks) - tracker_info.num_active_objs;
      for (int32_t index = 0; index < num_inactive - 1; index++)
      {
         tracker_info.inactive_obj_ids[index] = tracker_info.inactive_obj_ids[index + 1];
      }
      tracker_info.inactive_obj_ids[num_inactive - 1] = 0;

      // Add new ID to list of active tracks. Increment and save number of active object tracks. Increment unique objects counter.
      tracker_info.active_obj_ids[tracker_info.num_active_objs] = new_id;
      tracker_info.num_active_objs++;
      tracker_info.num_unique_objs++;

      return new_id;
   }
}
