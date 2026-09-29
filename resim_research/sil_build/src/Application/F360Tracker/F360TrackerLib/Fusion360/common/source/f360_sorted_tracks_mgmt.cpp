/*===================================================================================*\
* FILE: f360_sorted_tracks_mgmt.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Sorted_Tracks_Update_List and helper functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_sorted_tracks_mgmt.h"
#include "f360_reuse.h"
#include "f360_iterator.h"
#include <algorithm>

namespace f360_variant_A
{
   static void Insert_New_Track(F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T * const p_prev_track,
      F360_Object_Track_T * const p_next_track,
      F360_Object_Track_T * const p_new_track);

   void Sorted_Tracks_Re_Sort(
      F360_Tracker_Info_T & tracker_info)
   {
      tracker_info.vcslong_sorted_first_infront_of_host = NULL;
      for (int32_t i = 0; i < tracker_info.num_active_objs - 1; i++)
      {
         bool f_done = true;
         F360_Object_Track_T* curr_trk = tracker_info.vcslong_sorted_start;
         for (int32_t j = 0; j < tracker_info.num_active_objs - i - 1; j++)
         {
            F360_Object_Track_T* const next_trk = tracker_info.vcslong_sorted_next_track[curr_trk->id - 1];
            if (NULL == next_trk)
            {
               break;
            }

            if (curr_trk->vcs_position.x > next_trk->vcs_position.x)
            {
               F360_Object_Track_T* const prev_trk = tracker_info.vcslong_sorted_prev_track[curr_trk->id - 1];
               F360_Object_Track_T* const next2_trk = tracker_info.vcslong_sorted_next_track[next_trk->id - 1];
               if (NULL != next2_trk)
               {
                  tracker_info.vcslong_sorted_prev_track[next2_trk->id - 1] = curr_trk;
               }
               tracker_info.vcslong_sorted_next_track[curr_trk->id - 1] = next2_trk;
               tracker_info.vcslong_sorted_prev_track[curr_trk->id - 1] = next_trk;

               tracker_info.vcslong_sorted_next_track[next_trk->id - 1] = curr_trk;
               tracker_info.vcslong_sorted_prev_track[next_trk->id - 1] = prev_trk;

               if (tracker_info.vcslong_sorted_start == curr_trk)
               {
                  tracker_info.vcslong_sorted_start = next_trk;
               }
               else
               {
                  tracker_info.vcslong_sorted_next_track[prev_trk->id - 1] = next_trk;
               }

               f_done = false;
            }
            else
            {
               curr_trk = next_trk;
            }

            F360_Object_Track_T* const prev_trk = tracker_info.vcslong_sorted_prev_track[curr_trk->id - 1];
            if ((prev_trk != NULL)
                  && (prev_trk->vcs_position.x < 0.0F)
                  && (curr_trk->vcs_position.x > 0.0F))
            {
               tracker_info.vcslong_sorted_first_infront_of_host = curr_trk;
            }
         }
         if (f_done)
         {
            break;
         }
      }
      // After sorting, if vcslong_sorted_first_infront_of_host is still NULL
      // it implies that all objects could be in front of host and the sorted start is likely what we are looking for
      if((tracker_info.vcslong_sorted_first_infront_of_host == NULL) 
         && (tracker_info.vcslong_sorted_start != NULL)
         && (tracker_info.vcslong_sorted_start->vcs_position.x > 0.0F))
      {
         tracker_info.vcslong_sorted_first_infront_of_host = tracker_info.vcslong_sorted_start;
      }
   }

   void sorted_tracks_remove(
      F360_Tracker_Info_T & tracker_info,
      const F360_Object_Track_T & dead_track)
   {
      F360_Object_Track_T* const next_trk = tracker_info.vcslong_sorted_next_track[dead_track.id - 1];
      F360_Object_Track_T* const prev_trk = tracker_info.vcslong_sorted_prev_track[dead_track.id - 1];

      if (next_trk != NULL)
      {
         tracker_info.vcslong_sorted_prev_track[next_trk->id - 1] = prev_trk;
      }
      if (prev_trk != NULL)
      {
         tracker_info.vcslong_sorted_next_track[prev_trk->id - 1] = next_trk;
      }
      if ((next_trk == NULL) && (prev_trk == NULL))
      {
         tracker_info.vcslong_sorted_start = NULL;
      }
      if (tracker_info.vcslong_sorted_start == &dead_track)
      {
         tracker_info.vcslong_sorted_start = next_trk;
      }
      tracker_info.vcslong_sorted_next_track[dead_track.id - 1] = NULL;
      tracker_info.vcslong_sorted_prev_track[dead_track.id - 1] = NULL;
   }

   /*===========================================================================*\
   * FUNCTION: Sorted_Tracks_Insert()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Tracker_Info_T & tracker_info
   * F360_Object_Track_T* const track
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
   * This function adds new track to the sorted by vcs long position track 
   * linked list in tracker_info.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Sorted_Tracks_Insert(
      F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T* const p_new_track)
   {
      if (NULL == tracker_info.vcslong_sorted_start)
      {
         // tracker_info.vcslong_sorted_start was not initialized yet.
         tracker_info.vcslong_sorted_start = p_new_track;
      }
      else 
      {
         bool f_success = false;
         F360_Object_Track_T *p_current_track = tracker_info.vcslong_sorted_start;
         const int32_t already_active_obj_count = tracker_info.num_active_objs - 1;  // number of active objects was increased before
         for (int32_t track_counter = 0; (track_counter < already_active_obj_count) && (!f_success); track_counter++)
         { 
            F360_Object_Track_T * const p_next_track = tracker_info.vcslong_sorted_next_track[p_current_track->id - 1];

            if (p_current_track->vcs_position.x > p_new_track->vcs_position.x)
            {
               F360_Object_Track_T * const p_prev_track = tracker_info.vcslong_sorted_prev_track[p_current_track->id - 1];
               Insert_New_Track(tracker_info, p_prev_track, p_current_track, p_new_track);
               f_success = true;
            }
            else if (NULL == p_next_track)
            {
               Insert_New_Track(tracker_info, p_current_track, p_next_track, p_new_track);
               f_success = true;
            }
            else
            {
               // Do nothing
            }

            p_current_track = p_next_track;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Insert_New_Track()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Tracker_Info_T* const tracker_info
   * F360_Object_Track_T* const p_prev_track
   * F360_Object_Track_T* const p_next_track
   * F360_Object_Track_T* const p_new_track
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
   * This function insert new track to linked list.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Insert_New_Track(F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T * const p_prev_track,
      F360_Object_Track_T * const p_next_track,
      F360_Object_Track_T * const p_new_track)
   {
      // Assinge p_current_track and p_next_track as prvious and next track of p_new_track.*
      tracker_info.vcslong_sorted_next_track[p_new_track->id - 1] = p_next_track;
      tracker_info.vcslong_sorted_prev_track[p_new_track->id - 1] = p_prev_track;

      // If p_next track is NULL, element is added at the end of linked list.
      if (NULL != p_next_track)
      {
         // p_next_track exists, assigne p_new_track as previous element of p_next_track
         tracker_info.vcslong_sorted_prev_track[p_next_track->id - 1] = p_new_track;
      }
      
      if (NULL != p_prev_track)
      {
         // p_current_track exists, assigne p_new_track as next element of p_next_track
         tracker_info.vcslong_sorted_next_track[p_prev_track->id - 1] = p_new_track;
      }
      // If p_next track is NULL, element is added at the begining of linked list.
      else
      {
         tracker_info.vcslong_sorted_start = p_new_track;
      }
   }

}
