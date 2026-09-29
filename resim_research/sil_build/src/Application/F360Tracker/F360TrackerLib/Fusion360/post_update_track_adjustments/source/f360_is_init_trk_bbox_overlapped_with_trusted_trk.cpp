/*===================================================================================*\
* FILE:  f360_is_init_trk_bbox_overlapped_with_trusted_trk.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functions to check if the bounding box of an existing track is
* overlapped with the bounding box of an initialized track.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_math.h"
#include "f360_is_init_trk_bbox_overlapped_with_trusted_trk.h"
#include "f360_math_func.h"
#include "f360_get_track_bbox_in_vcs.h"
#include "f360_norm_heading_angle.h"
#include "f360_check_if_objects_can_overlap_significantly.h"
#include <algorithm>

namespace f360_variant_A
{
   static bool Is_Object_Valid(
      const int32_t initialized_obj_id,
      const F360_Calibrations_T & calib,
      const F360_Object_Track_T & obj);

   /*===========================================================================*\
   * FUNCTION: Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk()
   *===========================================================================
   * RETURN VALUE:
   * bool f_init_obj_should_be_killed
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib
   * const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * const int32_t initialized_obj_idx
   * const F360_Tracker_Info_T & tracker_info
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
   * Function to check if the bounding box of an existing trusted track is
   * overlapped with the bounding box of initialized track. We are not looping
   * through all existing tracks. Instead we first find a few tracks that are
   * nearest to the initialized track and then check the overlap.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Init_Trk_BBox_Overlapped_With_Trusted_Trk(
      const F360_Calibrations_T & calib,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const int32_t initialized_obj_idx,
      const F360_Tracker_Info_T& tracker_info)
   {
      bool f_init_obj_should_be_killed = false;

      if (tracker_info.num_active_objs > 0)
      {
         // Calculating the distances between existing trusted tracks and the initialized track.
         float32_t dist_to_other_confirmed_trks_sq[NUMBER_OF_OBJECT_TRACKS];
         int32_t n_trusted_trks = 0;
         const int32_t NUM_SELECTED_TRKS_TO_COMPARE = 4;
         const F360_Object_Track_T & initialized_obj = object_tracks[initialized_obj_idx];
         const Point init_obj_center_pos = initialized_obj.bbox.Get_Center();

         for (int32_t idx = 0; idx < tracker_info.num_active_objs; idx++)
         {

            const F360_Object_Track_T & obj_to_compare = (object_tracks[tracker_info.active_obj_ids[idx] - 1]);
            if (Is_Object_Valid(initialized_obj.id, calib, obj_to_compare)
               && Check_If_Objects_Can_Overlap_Significantly(init_obj_center_pos, initialized_obj.bbox.Get_Length(), obj_to_compare.bbox.Get_Center(), obj_to_compare.bbox.Get_Length()))
            {
               const float32_t dx = (obj_to_compare.bbox.Get_Center().x - init_obj_center_pos.x);
               const float32_t dy = (obj_to_compare.bbox.Get_Center().y - init_obj_center_pos.y);

               dist_to_other_confirmed_trks_sq[idx] = dx*dx + dy*dy;
               n_trusted_trks++;
            }
            else
            {
               dist_to_other_confirmed_trks_sq[idx] = calib.k_puta_large_distance;
            }
         }

         const int32_t n_trks_to_compare = std::min(NUM_SELECTED_TRKS_TO_COMPARE, n_trusted_trks);

         if (n_trks_to_compare > 0)
         {
            // Selected a few tracks that are nearest to the initialized track.
            int32_t selected_trks_ids[NUM_SELECTED_TRKS_TO_COMPARE] = {};

            for (int32_t idx = 0; idx < n_trks_to_compare; idx++)
            {
               const uint32_t min_idx = F360_Min_Index(dist_to_other_confirmed_trks_sq, static_cast<uint32_t>(tracker_info.num_active_objs));

               selected_trks_ids[idx] = tracker_info.active_obj_ids[min_idx];
               dist_to_other_confirmed_trks_sq[min_idx] = calib.k_puta_large_distance;
            }

            // Check overlap
            for (int32_t idx = 0; idx < n_trks_to_compare; idx++)
            {
               const int32_t obj_id = selected_trks_ids[idx];
               const F360_Object_Track_T& obj_to_check = object_tracks[obj_id - 1];

               f_init_obj_should_be_killed = Check_If_Init_Object_Should_Be_Killed(calib, initialized_obj, obj_to_check);
               if (f_init_obj_should_be_killed)
               {
                  break;
               }
            }
         }
         else
         {
            f_init_obj_should_be_killed = false;
         }
      }
      else
      {
         f_init_obj_should_be_killed = false;
      }
      return f_init_obj_should_be_killed;
   }

   /*===========================================================================*\
   * FUNCTION: Check_If_Init_Object_Should_Be_Killed()
   *===========================================================================
   * RETURN VALUE:
   * bool f_init_obj_should_be_killed
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib,
   * const F360_Object_Track_T & initialized_obj,
   * const F360_Object_Track_T & obj_to_check
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
   * This function determine if new object should be erased/killed. Function
     calculate overlapping area between two objects (initialized object and
     currently checked object) bounding boxes and than divide it by area of
     initialized bounding boxes area. If this value is above some given
     threshold and also orientation difference between these object is below
     some given threshold than we assume that initialized object should
     be killed.
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Check_If_Init_Object_Should_Be_Killed(
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& initialized_obj,
      const F360_Object_Track_T& obj_to_check)
   {
      const float32_t orientation_diff_norm = (initialized_obj.bbox.Get_Orientation() - obj_to_check.bbox.Get_Orientation()).Normalize().Value();
      const float32_t orientation_diff_abs_norm = std::abs(orientation_diff_norm);
      bool f_init_obj_should_be_killed = orientation_diff_abs_norm < calib.k_puta_orientation_diff_threshold;

      if (f_init_obj_should_be_killed)
      {
         float32_t overlapping_ratio;
         const float32_t initialized_obj_bbox_area = initialized_obj.bbox.Get_Area();

         if (initialized_obj_bbox_area > F360_EPSILON)
         {
            const float32_t overlapping_area = initialized_obj.bbox.Overlap_Area(obj_to_check.bbox);
            overlapping_ratio = overlapping_area / initialized_obj_bbox_area;
         }
         else
         {
            overlapping_ratio = 0.0F;
         }

         const float32_t overlapping_ratio_threshold = 0.75F;
         f_init_obj_should_be_killed = (overlapping_ratio_threshold < overlapping_ratio);
      }

      return f_init_obj_should_be_killed;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Valid()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const int32_t & initialized_obj_id
   * const F360_Calibrations_T & calib
   * const F360_Object_Track_T & obj
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
   * Functions returns true if object meet all conditions to be treated as trusted
   * object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Is_Object_Valid(
      const int32_t initialized_obj_id,
      const F360_Calibrations_T & calib,
      const F360_Object_Track_T & obj)
   {
      return ((obj.id != initialized_obj_id)
           && (F360_OBJECT_STATUS_NEW_UPDATED < obj.status)
           && (obj.f_moving)
           && (obj.confidenceLevel > calib.k_puta_min_object_confidence)
           && (obj.time_since_cluster_created >= calib.k_puta_min_object_time));
   }

}
