/*===================================================================================*\
* FILE: f360_cancel_new_updated_trk_overlapping_confirmed_trks.cpp
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec:
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
*   The file contains the defination of Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks function
*
* ABBREVIATIONS:
*
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

#include "f360_cancel_new_updated_trk_overlapping_confirmed_trks.h"
#include <algorithm>
#include "f360_iterator.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_is_init_trk_bbox_overlapped_with_trusted_trk.h"
#include "f360_kill_obj_trk.h"
#include "f360_get_wall_time.h"
#include "f360_clear_det_assoc_obj_props.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
   /******************************
   *   Function prototypes
   *******************************/

   /*===========================================================================*\
   * FUNCTION: Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * F360_Tracker_Info_T & tracker_info
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_TRKR_TIMING_INFO_T & timing_info
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
   * This function Cancel new updated tracks
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks(
      const F360_Calibrations_T & calib,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();
      int32_t active_obj_ids[NUMBER_OF_OBJECT_TRACKS];

      (void)std::copy(cmn::begin(tracker_info.active_obj_ids), cmn::end(tracker_info.active_obj_ids), cmn::begin(active_obj_ids));
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_trk_idx = active_obj_ids[i] - 1;
         if ((F360_OBJECT_STATUS_NEW_UPDATED == object_tracks[obj_trk_idx].status) && object_tracks[obj_trk_idx].f_moving)
         {
            const bool f_success = Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk(calib, object_tracks, obj_trk_idx, tracker_info);

            // % undo the track initialization.Track filter fields will be of default value.
            if (f_success)
            {
               // It's necessary to clear detections associated to killed object to prevent them from being used in CWD.
               Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props(obj_trk_idx + 1, object_tracks, tracker_info, det_props);
            }
         }
      }

      timing_info.cancel_new_updated_trk_overlapping_confirmed_trks = get_wall_time() - start_time;
   }
}


