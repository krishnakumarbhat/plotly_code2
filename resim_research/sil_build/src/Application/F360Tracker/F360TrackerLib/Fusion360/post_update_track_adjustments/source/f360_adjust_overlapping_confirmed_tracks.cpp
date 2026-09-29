/*===================================================================================*\
* FILE:  f360_adjust_overlapping_confirmed_tracks.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functions which make adjustments when two confirmed tracks have 
* bounding boxes that are significantly overlapping and have other characteristics 
* that would warrant adjustment.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_adjust_overlapping_confirmed_tracks.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_reuse.h"
#include "f360_calibrations.h"
#include "f360_globals.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_norm_heading_angle.h"
#include "f360_constants.h"
#include "f360_get_wall_time.h"
#include "f360_get_track_bbox_in_vcs.h"
#include "f360_bounding_box.h"
#include "f360_check_if_objects_can_overlap_significantly.h"

namespace f360_variant_A
{
   static bool Is_Object_Considered_As_Confirmed(
      const F360_Object_Status_T obj_status,
      const float32_t obj_confidencelevel,
      const float32_t k_low_confidence_level_thresh,
      const float32_t movable_prob);

   static bool Are_Objects_Close_Vcs_Longitudinally(
      const float32_t lower_longpos_obj_center_x_vcs,
      const float32_t higher_longpos_obj_center_x_vcs,
      const float32_t lower_longpos_obj_length,
      const float32_t distance_buffer);

   /*===========================================================================*\
   * FUNCTION: Adjust_Overlapping_Confirmed_Tracks()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib
   * const F360_Tracker_Info_T & tracker_info
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
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
   * This file contains functions which make adjustments when two confirmed tracks have 
   * bounding boxes that are significantly overlapping and have other characteristics 
   * that would warrant adjustment.
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Adjust_Overlapping_Confirmed_Tracks(
      const F360_Calibrations_T & calib,
      const F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();

      const uint32_t num_active_objs = static_cast<uint32_t>(tracker_info.num_active_objs);

      // Reset flag for all active objects
      for (uint32_t i = 0U; i < num_active_objs; i++)
      {
         const uint32_t trk_idx = static_cast<uint32_t>(tracker_info.active_obj_ids[i]) - 1U;
         object_tracks[trk_idx].f_overlapping_with_object = false;
      }

      if (num_active_objs > 1U)
      {
         const F360_Object_Track_T* p_obj1 = tracker_info.vcslong_sorted_start;
         
         for (uint32_t i = 0U; i < num_active_objs - 1U; i++)
         {
            F360_Object_Track_T obj1;
            uint32_t idx1;
            uint32_t idx2;
            if (NULL != p_obj1)
            {
               obj1 = *p_obj1;
               idx1 = static_cast<uint32_t>(obj1.id) - 1U;
               idx2 = idx1;
            }
            else
            {
               break;
            }
            
            // Check for confirmed tracks with enough confidence to be downselected
            if (Is_Object_Considered_As_Confirmed(obj1.status, obj1.confidenceLevel, calib.low_confidence_level_thresh, obj1.movable_prob))
            {
               for (uint32_t j = i + 1U; j < num_active_objs; j++)
               {
                  const F360_Object_Track_T* const p_obj2 = tracker_info.vcslong_sorted_next_track[idx2];
                  F360_Object_Track_T obj2;
                  if (NULL != p_obj2)
                  {
                     obj2 = *p_obj2;
                     idx2 = static_cast<uint32_t>(obj2.id) - 1U;
                  }
                  else
                  {
                     continue;
                  }

                  // Looping over objects long-sorted. If sufficiently far away, take next object
                  if (!Are_Objects_Close_Vcs_Longitudinally(obj1.bbox.Get_Center().x, obj2.bbox.Get_Center().y, obj1.bbox.Get_Length(), calib.k_puta_overlapping_tracks_long_thr))
                  {
                     break;
                  }

                  if (Is_Object_Considered_As_Confirmed(obj2.status, obj2.confidenceLevel, calib.low_confidence_level_thresh, obj2.movable_prob)
                     && Check_If_Objects_Can_Overlap_Significantly(obj1.bbox.Get_Center(), obj1.bbox.Get_Length(), obj2.bbox.Get_Center(), obj2.bbox.Get_Length()))
                  {
                     // Check for overlapping bounding boxes
                     const float32_t overlap_area = obj1.bbox.Overlap_Area(obj2.bbox);

                     if (overlap_area > 0.0F)
                     {
                        uint32_t higher_conf_idx;
                        uint32_t lower_conf_idx;
                        const float32_t large_overlap_pct = 0.5F;

                        uint32_t idx_large_overlap;
                        if (obj1.confidenceLevel > obj2.confidenceLevel)
                        {
                           higher_conf_idx = idx1;
                           lower_conf_idx = idx2;
                           const float32_t area_box2 = obj2.bbox.Get_Area();
                           idx_large_overlap = (overlap_area > (area_box2 * large_overlap_pct)) ? idx2 : 0U;
                        }
                        else
                        {
                           higher_conf_idx = idx2;
                           lower_conf_idx = idx1;
                           const float32_t area_box1 = obj1.bbox.Get_Area();
                           idx_large_overlap = (overlap_area > (area_box1 * large_overlap_pct)) ? idx1 : 0U;
                        }

                        const bool f_conf_disagree = ((object_tracks[higher_conf_idx].confidenceLevel > calib.k_puta_overlapping_tracks_high_conf_thr) &&
                           (object_tracks[lower_conf_idx].confidenceLevel < calib.k_puta_overlapping_tracks_low_conf_thr));

                        const Angle hdg_diff = (obj1.vcs_heading - obj2.vcs_heading).Normalize();
                        const float32_t heading_diff_abs = std::abs(hdg_diff.Value());
                        const bool f_hdg_disagree = ((heading_diff_abs > calib.k_puta_overlapping_tracks_max_heading_diff) && 
                           (std::abs(heading_diff_abs - F360_PI) > calib.k_puta_overlapping_tracks_max_heading_diff));

                        const bool f_spd_disagree = (std::abs(obj1.speed - obj2.speed) > calib.k_puta_overlapping_tracks_max_speed_diff);

                        const bool f_disagree_combined = (f_conf_disagree && (f_hdg_disagree || f_spd_disagree));

                        if (f_disagree_combined)
                        {
                           object_tracks[lower_conf_idx].f_overlapping_with_object = true;
                           continue;
                        }

                        if ((heading_diff_abs < calib.k_puta_overlapping_tracks_max_heading_diff) &&
                           (!f_spd_disagree) && (idx_large_overlap == lower_conf_idx))
                        {
                           object_tracks[lower_conf_idx].f_overlapping_with_object = true;
                        }
                     }
                  }
               }
            }
            // Update relevant pointer
            p_obj1 = tracker_info.vcslong_sorted_next_track[idx1];
         }
      }
      timing_info.adjust_overlapping_confirmed_trks = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Considered_As_Confirmed()
   *===========================================================================
   * RETURN VALUE:
   * bool f_confirmed_track
   *
   * PARAMETERS:
   * const F360_Object_Status_T obj_status,
   * const float32_t obj_confidencelevel,
   * const float32_t k_low_confidence_level_thresh,
   * const float32_t movable_prob
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
   * This function checks basic requirements for the object to be applicable for the 
   * logic related to adjusting overlapping tracks. The for object to be considered as
   * confirmed, it must either have status Updated or Coasted, have high enough 
   * confidence level and be moveable.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Is_Object_Considered_As_Confirmed(
      const F360_Object_Status_T obj_status,
      const float32_t obj_confidencelevel,
      const float32_t k_low_confidence_level_thresh,
      const float32_t movable_prob)
   {
      const bool f_confirmed_track = (((F360_OBJECT_STATUS_UPDATED == obj_status) || (F360_OBJECT_STATUS_COASTED == obj_status)) 
         && (obj_confidencelevel > k_low_confidence_level_thresh)
         && (movable_prob > 0.5F));
      return f_confirmed_track;
   }

   /*===========================================================================*\
   * FUNCTION: Are_Objects_Close_Vcs_Longitudinally()
   *===========================================================================
   * RETURN VALUE:
   * bool f_possible_to_overlap_longitudinally
   *
   * PARAMETERS:
   *  const float32_t lower_longpos_obj_center_x_vcs,
   *  const float32_t higher_longpos_obj_center_x_vcs,
   *  const float32_t lower_longpos_obj_length,
   *  const float32_t distance_buffer
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
   * This function checks basic requirements for the object longitudinal distance
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Are_Objects_Close_Vcs_Longitudinally(
      const float32_t lower_longpos_obj_center_x_vcs,
      const float32_t higher_longpos_obj_center_x_vcs,
      const float32_t lower_longpos_obj_length,
      const float32_t distance_buffer)
   {
      const float32_t pos_diff_x_vcs = higher_longpos_obj_center_x_vcs - lower_longpos_obj_center_x_vcs;

      const bool f_possible_to_overlap_longitudinally = pos_diff_x_vcs <= (lower_longpos_obj_length + distance_buffer);

      return f_possible_to_overlap_longitudinally;
   }
}
