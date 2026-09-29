/*===================================================================================*\
* FILE: f360_kill_obj_trk.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Kill_Obj_Trk()
*
* ABBREVIATIONS:
*   NONE
*
* TRACEABILITY INFO:
*   Design Document(s): killObjTrk.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*      ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*      ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/


/******************************
* Includes
*******************************/
#include <cstring>
#include "f360_math.h"
#include "f360_kill_obj_trk.h"
#include "f360_math_func.h"
#include "f360_clear_object_track.h"
#include "f360_push_reduced_id.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_iterator.h"
#include "f360_clear_det_assoc_obj_props.h"

namespace f360_variant_A
{
   /******************************
   *   Function prototypes
   *******************************/
   static void Remove_Sorted_Track(const F360_Object_Track_T & object_track,
      F360_Tracker_Info_T & tracker_info);

   /*===========================================================================*\
   * FUNCTION: Kill_Obj_Trk()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const int32_t obj_trk_id,
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_Tracker_Info_T & tracker_info,
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
   *  Kill a particular object track, indicated by obk_trk_id.
   *  Includes handling of the tracker's lists of active and inactive tracks,
   *  in addition to clearing of contents of the particular object track.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Kill_Obj_Trk(
      const int32_t obj_trk_id,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info)
   {
      const int32_t * const begin = cmn::begin(tracker_info.active_obj_ids);
      const int32_t * const end = cmn::end(tracker_info.active_obj_ids);

      const int32_t * const p_active_obj_id_found = std::find(begin, end, obj_trk_id);

      const bool f_found_obj_among_active_objs = p_active_obj_id_found != end;
      const int32_t num_obj_active = tracker_info.num_active_objs;
      const bool f_valid_obj = (0 < obj_trk_id) && (tracker_info.variant.num_tracks >= static_cast<uint32_t>(obj_trk_id)) && f_found_obj_among_active_objs && (num_obj_active > 0);

      if (f_valid_obj)
      {
         const uint32_t obj_idx_in_active_obj_array = static_cast<uint32_t>(std::distance(begin, p_active_obj_id_found));
         const int32_t obj_trk_idx = obj_trk_id - 1;
         if (object_tracks[obj_trk_idx].reduced_id > F360_INVALID_REDUCED_ID)
         {
            Push_Reduced_Id(object_tracks[obj_trk_idx].reduced_id, tracker_info);
         }

         for (uint32_t idx = obj_idx_in_active_obj_array; idx < (tracker_info.variant.num_tracks - 1U); idx++)
         {
            tracker_info.active_obj_ids[idx] = tracker_info.active_obj_ids[idx + 1U];
         }

         tracker_info.active_obj_ids[tracker_info.variant.num_tracks - 1U] = 0;
         tracker_info.inactive_obj_ids[tracker_info.variant.num_tracks - static_cast<uint32_t>(num_obj_active)] = obj_trk_id;

         tracker_info.num_active_objs = num_obj_active - 1;

         sorted_tracks_remove(tracker_info, object_tracks[obj_trk_idx]);
         Remove_Sorted_Track(object_tracks[obj_trk_idx], tracker_info);
         Clear_Object_Track(object_tracks[obj_trk_idx]);
      }
   }


   /*===========================================================================*\
   * FUNCTION: Remove_Sorted_Track()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * F360_Object_Track_T & object_track,
   * F360_Tracker_Info_T & tracker_info
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
   * This function remove track from sorted linked list
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Remove_Sorted_Track(const F360_Object_Track_T & object_track,
      F360_Tracker_Info_T & tracker_info)
   {
      if (object_track.p_lower_priority_track != NULL)
      {
         object_track.p_lower_priority_track->p_higher_priority_track = object_track.p_higher_priority_track;
      }
      else
      {
         tracker_info.p_lowest_priority_track = object_track.p_higher_priority_track;
      }

      if (object_track.p_higher_priority_track != NULL)
      {
         object_track.p_higher_priority_track->p_lower_priority_track = object_track.p_lower_priority_track;
      }
      else
      {
         tracker_info.p_highest_priority_track = object_track.p_lower_priority_track;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const int32_t obj_trk_id,
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_Tracker_Info_T & tracker_info,
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function clears the object related properties of all detections associated
   * to the specified object and the kills the object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props(
      const int32_t obj_trk_id,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      Clear_Det_Assoc_Obj_Props(object_tracks[obj_trk_id - 1], det_props);
      Kill_Obj_Trk(obj_trk_id, object_tracks, tracker_info);
   }
}



