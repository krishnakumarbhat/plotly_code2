/*===================================================================================*\
* FILE:  f360_priority_update_tracks.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function to time update the object priority 
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#include "f360_priority_update_tracks.h"

#include "f360_calculate_priority.h"
#include "f360_get_wall_time.h"
#include "f360_sort_priority.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Track_Priority()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const F360_Host_T &host_props,
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_Tracker_Info_T &tracker_info)
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
   * This function calculates basic priority value for the cluster or object with
   * given number of detection, position and velocity
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Track_Priority(
      const F360_Host_T &host_props,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T &tracker_info)
   {

      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;

         if (object_tracks[obj_idx].status != F360_OBJECT_STATUS_INVALID)
         {
            // ndets is from previous instance. Currently it is reset before detections to object association.
            object_tracks[obj_idx].priority = Calculate_Priority(host_props, object_tracks[obj_idx].movable_prob, object_tracks[obj_idx].confidenceLevel, object_tracks[obj_idx].vcs_position.x, object_tracks[obj_idx].vcs_position.y);
         }
      }

      Quick_Sort_Track_Priority(tracker_info, object_tracks);
   }
}
