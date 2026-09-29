/*===================================================================================*\
* FILE: f360_track_grouping.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Track_Grouping function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#include "f360_track_grouping.h"

#include "f360_try_to_merge_two_objects.h"
#include "f360_get_wall_time.h"
#include "f360_compute_split_logic_signals.h"
#include "f360_split_objects_in_orth_direction.h"

namespace f360_variant_A
{
   static bool Is_Obj_Valid_For_Merge(const F360_Object_Track_T& obj);

   /*===========================================================================*\
   * FUNCTION: Track_Grouping()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS]
   * const F360_Host_T& host
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list
   * const F360_Globals_T& globals
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Tracker_Info_T & tracker_info
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
   * Functions iterate after longitudinally sorted objects and checks whether 
   * any two of them could be merged and merge them if it is needed.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Track_Grouping(
      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();
      const F360_Object_Track_T* obj1 = tracker_info.vcslong_sorted_start;
      for (int32_t i = 0; i < (tracker_info.num_active_objs - 1); i++) // Outer loop over objects
      {  
         if (NULL == obj1)
         {
            break;
         }

         const int32_t idx1 = obj1->id - 1;
         const F360_Object_Track_T* const temp_next_object = tracker_info.vcslong_sorted_next_track[idx1];

         if (Is_Obj_Valid_For_Merge(*obj1))
         {
            Try_To_Merge_With_Other_Object(sensors, raw_detection_list, calib, static_env_polys, idx1, host, globals, object_tracks, detection_props, tracker_info);
         }

         obj1 = tracker_info.vcslong_sorted_next_track[idx1];
         // If obj1 was killed during the merge, make sure that the next object in the sorted list is processed unless it's the last one in the list
         if ((i < tracker_info.num_active_objs - 2) && (NULL == obj1))
         {
             obj1 = temp_next_object;
         }
      }

      Compute_Split_Logic_Signals(detection_props, sensors, raw_detection_list.detections, calib, tracker_info, host.dist_rear_axle_to_vcs_m, object_tracks);

      Split_Objects_In_Orth_Direction(
         host,
         calib,
         static_env_polys,
         sensors,
         globals,
         object_tracks,
         tracker_info,
         detection_props);

      timing_info.track_grouping = get_wall_time() - start_time;

   }

   /*===========================================================================*\
   * FUNCTION: Try_To_Merge_With_Other_Object()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list
   * const F360_Calibrations_T & calib
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
   * const int32_t idx1
   * const F360_Host_T & host
   * const F360_Globals_T& globals
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Function takes first object and compare it with next objects on 
   * longitudinally sorted targets list and verify if any object could be merged 
   * with the first object. 
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Try_To_Merge_With_Other_Object(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const int32_t idx1,
      const F360_Host_T & host,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T& tracker_info)
   {
      int32_t idx2 = idx1;
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         bool f_take_next_object;
         const F360_Object_Track_T* const obj2 = tracker_info.vcslong_sorted_next_track[idx2];
         if (NULL != obj2)
         {
            idx2 = obj2->id - 1;
  
            const float32_t max_long_dist_to_keep_iterating_over_objects = 2.0F * object_tracks[idx1].bbox.Get_Length();
            const bool is_long_dist_between_objs_close = ((object_tracks[idx2].bbox.Get_Center().x - object_tracks[idx1].bbox.Get_Center().x) <= max_long_dist_to_keep_iterating_over_objects);
            if(is_long_dist_between_objs_close)
            {
               int32_t kill_idx = F360_INVALID_ID;
               if (Is_Obj_Valid_For_Merge(*obj2))
               {
                   Try_To_Merge_Two_Objects(host, sensors, raw_detection_list, calib, static_env_polys, idx1, idx2, globals, object_tracks, detection_props, tracker_info, kill_idx);
               }
               f_take_next_object = (idx1 != kill_idx);
            }
            else
            {
               f_take_next_object = false;
            }
         }
         else
         {
            f_take_next_object = false;
         }
         if (!f_take_next_object)
         {
            break;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Obj_Is_Valid_For_Merge()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Object_Track_T &obj
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
   * Functions check some basic object properties to make sure it is valid to be merged.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static bool Is_Obj_Valid_For_Merge(const F360_Object_Track_T &obj)
   {
      const float32_t min_speed_for_merging = 1.0F; // [m/s]
      const float32_t max_hdg_pnt_diagreement_for_merging = F360_DEG2RAD(10.0F); // [rad]

      const bool f_obj_is_ok_to_merge = (obj.f_moving) && (std::abs(obj.speed) > min_speed_for_merging) &&
         (std::abs(obj.hdg_ptng_disagmt) < max_hdg_pnt_diagreement_for_merging) &&
         (obj.ndets > 0U) && (!obj.f_hide_occluded_track_behind_host);

      return f_obj_is_ok_to_merge;
   }
}
