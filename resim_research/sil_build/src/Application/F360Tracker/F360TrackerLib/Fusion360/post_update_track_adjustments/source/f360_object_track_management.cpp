/*===================================================================================*\
* FILE:  f360_object_track_management.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* N/A
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_object_track_management.h"
#include "f360_object_track_management_internals.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Obj_Trk_Status_Book_Keeping()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   *   const F360_Tracker_Info_T &tracker_info,
   *   const F360_Calibrations_T &calibs,
   *   F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   *   F360_TRKR_TIMING_INFO_T &timing_info
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
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Obj_Trk_Status_Book_Keeping(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T &calibs,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T &timing_info
   )
   {
      /*
       * NOTE: Switch from stage_age-based coasting, etc. to time-based.  Need something like this
       * to be able to handle allowing the tracker to run each time a sensor reports something (i.e., even a sensor which
       * doesn't have a particular object in its FOV).
       */

      const float32_t start_time = get_wall_time();

      for (int32_t act_obj_idx = 0; act_obj_idx < tracker_info.num_active_objs; act_obj_idx++)
      {
         F360_Object_Track_T &obj_track = object_tracks[tracker_info.active_obj_ids[act_obj_idx] - 1];

         if ((F360_OBJECT_STATUS_INVALID < obj_track.status) && (obj_track.time_since_initialization > F360_EPSILON))
         {
            if (Is_Enough_Valid_Dets_For_Update(det_props, obj_track))
            {
               Change_Object_To_Updated_State(obj_track);
            }
            else
            {
               Change_Object_To_Coasted_State(obj_track, calibs);
            }
         }
      }

      timing_info.obj_trk_status_bookkeeping = get_wall_time() - start_time;
   }
}
