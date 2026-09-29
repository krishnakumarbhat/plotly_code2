/*===================================================================================*\
* FILE: f360_sort_priority.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Sort_Priority_With_New_Track, Quick_Sort_Track_Priority and helper functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_sort_priority.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   static void Update_Track_Priority_Pointers(
      const uint32_t(&perm)[NUMBER_OF_OBJECT_TRACKS],
      const int32_t(&obj_idx)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);

   /*===========================================================================*\
   * FUNCTION: Sort_Priority_With_New_Track()
   *===========================================================================
   * ABSTRACT:
   * This function adds new track to the sorted track linked list
   \*===========================================================================*/
   void Sort_Priority_With_New_Track(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T* const p_new_track)
   {
      const uint32_t num_active_objs = static_cast<uint32_t>(tracker_info.num_active_objs);
      F360_Object_Track_T* p_current_track = tracker_info.p_highest_priority_track;

      if (p_current_track == NULL)
      {
         // tracker_info.p_highest_priority_track was not initialized yet
         tracker_info.p_highest_priority_track = p_new_track;
         tracker_info.p_lowest_priority_track = p_new_track;
      }
      else if (tracker_info.p_highest_priority_track->priority < p_new_track->priority)
      {
         // new track priority is greater then highest priority track
         tracker_info.p_highest_priority_track->p_higher_priority_track = p_new_track;
         p_new_track->p_lower_priority_track = tracker_info.p_highest_priority_track;
         tracker_info.p_highest_priority_track = p_new_track;
      }
      else
      {
         bool f_done = false;
         for (uint32_t track_counter = 0U; (track_counter < num_active_objs) && (!f_done); track_counter++)
         {
            F360_Object_Track_T* const p_next_track = p_current_track->p_lower_priority_track;
            if (p_next_track != NULL)
            {
               if (p_next_track->priority < p_new_track->priority)
               {
                  // insert track between p_current_track and p_next_track
                  p_new_track->p_lower_priority_track = p_next_track;
                  p_new_track->p_higher_priority_track = p_current_track;
                  p_current_track->p_lower_priority_track = p_new_track;
                  p_next_track->p_higher_priority_track = p_new_track;
                  f_done = true;
               }
            }
            else
            {
               // there is no next track in prioritized list: add the track at the end
               p_current_track->p_lower_priority_track = p_new_track;
               p_new_track->p_higher_priority_track = p_current_track;
               tracker_info.p_lowest_priority_track = p_new_track;
               f_done = true;
            }

            p_current_track = p_next_track;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Quick_Sort_Track_Priority()
   *===========================================================================
   * ABSTRACT:
   * This function adds new track to the sorted track linked list
   \*===========================================================================*/
   void Quick_Sort_Track_Priority(
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      const uint32_t n_obj = static_cast<uint32_t>(tracker_info.num_active_objs);

      int32_t obj_idx[NUMBER_OF_OBJECT_TRACKS];
      float32_t priority[NUMBER_OF_OBJECT_TRACKS];
      uint32_t perm[NUMBER_OF_OBJECT_TRACKS];

      for (uint32_t i = 0U; i < n_obj; i++)
      {
         const int32_t idx = tracker_info.active_obj_ids[i] - 1;
         obj_idx[i] = idx;
         priority[i] = object_tracks[idx].priority;
      }
      (void)F360_Sort(n_obj, false, priority, perm);

      Update_Track_Priority_Pointers(perm, obj_idx, tracker_info, object_tracks);
   }

   /*===========================================================================*\
   * FUNCTION: Update_Track_Priority_Pointers()
   *===========================================================================
   * ABSTRACT:
   * Update the linked list to reflect any changes in priority
   \*===========================================================================*/
   static void Update_Track_Priority_Pointers(
      const uint32_t(&perm)[NUMBER_OF_OBJECT_TRACKS],
      const int32_t(&obj_idx)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      const uint32_t n_track = static_cast<uint32_t>(tracker_info.num_active_objs);

      if (n_track == 0U)
      {
         tracker_info.p_lowest_priority_track = NULL;
         tracker_info.p_highest_priority_track = NULL;
      }
      else if (n_track == 1U)
      {
         const int32_t idx = tracker_info.active_obj_ids[0] - 1;
         F360_Object_Track_T* const p_current_track = &object_tracks[idx];
         tracker_info.p_lowest_priority_track = p_current_track;
         tracker_info.p_highest_priority_track = p_current_track;

         p_current_track->p_higher_priority_track = NULL;
         p_current_track->p_lower_priority_track = NULL;
      }
      else
      {
         // Highest priority.
         F360_Object_Track_T* p_current_track = &object_tracks[obj_idx[perm[0]]];
         F360_Object_Track_T* p_lower_priority_track = &object_tracks[obj_idx[perm[1]]];
         F360_Object_Track_T* p_higher_priority_track = NULL;

         tracker_info.p_highest_priority_track = p_current_track;

         p_current_track->p_higher_priority_track = p_higher_priority_track;
         p_current_track->p_lower_priority_track = p_lower_priority_track;
         const uint32_t n_max_iter = n_track - 1U;
         for (uint32_t index = 1U; index < n_max_iter; index++)
         {
            p_higher_priority_track = p_current_track;
            p_current_track = p_lower_priority_track;
            p_lower_priority_track = &object_tracks[obj_idx[perm[index + 1U]]];

            p_current_track->p_higher_priority_track = p_higher_priority_track;
            p_current_track->p_lower_priority_track = p_lower_priority_track;
         }

         // Lowest priority.
         p_higher_priority_track = p_current_track;
         p_current_track = p_lower_priority_track;

         tracker_info.p_lowest_priority_track = p_current_track;

         p_current_track->p_higher_priority_track = p_higher_priority_track;
         p_current_track->p_lower_priority_track = NULL;
      }
   }
}
