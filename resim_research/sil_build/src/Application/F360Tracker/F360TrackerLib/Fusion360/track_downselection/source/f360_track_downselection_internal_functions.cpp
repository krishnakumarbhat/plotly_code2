/*===================================================================================*\
* FILE: f360_track_downselection_internal_functions.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv. All Rights Reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definitions of functions used in Track_Downselection()
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#include "f360_track_downselection_internal_functions.h"
#include "f360_math.h"
#include <cstring>
#include "f360_push_reduced_id.h"
#include "f360_math_func.h"
#include "f360_find_closest_valid_sep_on_given_side.h"
#include "f360_reference_point_support_functions.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calc_Track_Priority()
   *===========================================================================
   * RETURN VALUE:
   * float32_t priority
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const F360_Calibrations_T& calib,
   * const F360_Tracker_Info_T& tracker_info,
   * const BoundingBox& overall_confidence_exclusion_box,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const bool f_limited_FOV,
   * F360_Object_Track_T& obj_trk,
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
   * This function updates the downselection priority in range <0, 1>.
   * Higher value of priority means that object is more important.
   * Priority value 0.0 means that object is not a downselection candidate.
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calc_Track_Priority(
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const F360_Tracker_Info_T& tracker_info,
      const BoundingBox& overall_confidence_exclusion_box,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool f_limited_FOV,
      F360_Object_Track_T& obj_trk)
   {
      float32_t priority;

      const bool f_recently_entered_FOV = f_limited_FOV ? Has_Object_Recently_Entered_FOV(host, sensors, obj_trk) : false;

      const bool f_is_low_confidence = (
         ((obj_trk.confidenceLevel < calib.low_confidence_level_thresh) || (CONF3_NONE == obj_trk.conf_overall)) 
         && (!obj_trk.f_suspectable_for_det_drop));

      const bool f_not_downselected_before_coasting = (
         (obj_trk.status == F360_OBJECT_STATUS_COASTED)
         && (obj_trk.reduced_status == F360_OBJECT_STATUS_INVALID));

      const bool f_medium_confidence_not_downselected_before = (
         (obj_trk.confidenceLevel < calib.k_track_downselect_confidence_thresh)
         && (obj_trk.reduced_status == F360_OBJECT_STATUS_INVALID)
         && (obj_trk.status == F360_OBJECT_STATUS_UPDATED));

      if (
         (obj_trk.status < F360_OBJECT_STATUS_NEW_UPDATED)
         || f_is_low_confidence
         || f_not_downselected_before_coasting
         || obj_trk.f_hide_occluded_track_behind_host
         || f_medium_confidence_not_downselected_before
         || Is_Unreliable_Low_Conf_Moveable_Track(obj_trk, host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV))
      {
         priority = 0.0F; // not a candidate for downselection
      }
      else
      {
         priority = obj_trk.priority;
      }

      if (priority > 0.0F)
      {
         Decrease_Priority_and_Confidence_for_Implausible_Tracks(tracker_info, calib, obj_trk, priority);
      }

      if (priority <= 0.0F)
      {
         // Increase priority of minimum priority objects based on variant-specific conditions
         Increase_Priority_and_Adjust_EP_Variant_Based(tracker_info.variant.type, obj_trk, priority);
      }

      // Lower confidence if NU_2_C ghost track
      if (obj_trk.f_ghost_NU_2_C && (obj_trk.time_since_track_updated >= calib.k_hyst_time_for_coasted_objects))
      {
         if (obj_trk.confidenceLevel > (calib.k_track_downselect_confidence_level_lowering_factor * calib.low_confidence_level_thresh))
         {
            obj_trk.confidenceLevel = calib.k_track_downselect_confidence_level_lowering_factor * calib.low_confidence_level_thresh;
         }
      }

      return priority;
   }

   /*===========================================================================*\
   * FUNCTION: Increase_Priority_and_Adjust_EP_Variant_Based()
   *===========================================================================
   * RETURN VALUE:
   *  None
   *
   * PARAMETERS:
   * const F360_Tracker_Variant_T tracker_variant 
   * F360_Object_Track_T& obj_trk
   * float32_t& priority
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
   * This function increases priority of an object based on variant specific conditions.
   * Existence probability is reduced for such objects.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Increase_Priority_and_Adjust_EP_Variant_Based(
       const F360_Tracker_Variant_T tracker_variant,
       F360_Object_Track_T& obj_trk,
       float32_t& priority)
   {

       switch (tracker_variant)
       {
           case F360_VARIANT_TYPE_D:
           {
               // Force downselection of straight-moving objects after 3 tracker cycles
               const float32_t age_threshold = 0.140F; // > 2 tracker cycles
               const float32_t heading_threshold = F360_DEG2RAD(45.0F); // 45 degrees
               const float32_t lateral_threshold = 20.0F;

               const bool f_movable = (obj_trk.movable_prob > 0.5F);
               const bool f_age_above_thr = (obj_trk.time_since_initialization >= age_threshold);
               const bool f_similar_heading_to_host = (std::abs(obj_trk.vcs_heading.Value()) < heading_threshold);
               const bool f_in_lateral_window = (std::abs(obj_trk.vcs_position.y) < lateral_threshold);

               if (f_movable && f_age_above_thr && f_similar_heading_to_host && f_in_lateral_window)
               {
                  const float32_t k_low_ep = 0.61F;
                  // increase priority using object track priority
                  priority = obj_trk.priority;
                  obj_trk.exist_prob = std::min(obj_trk.exist_prob, k_low_ep);
               }
               break;
           }
           default:
           {
               break;
           }
       }
   }

   /*===========================================================================*\
   * FUNCTION: Pop_Reduced_Id()
   *===========================================================================
   * RETURN VALUE:
   *  int32_t reduced_id
   *
   * PARAMETERS:
   * F360_Tracker_Info_T& tracker_info
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
   * This function pops reduced Id from front of array.
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   * There is a reduced IDs to pop
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   int32_t Pop_Reduced_Id(F360_Tracker_Info_T& tracker_info)
   {
      const int32_t num_obj = tracker_info.reduced_num_active_objs;
      const int32_t num_inactive_obj = static_cast<int32_t>(tracker_info.variant.num_reduced_tracks) - num_obj;
      const int32_t reduced_id = tracker_info.reduced_inactive_obj_ids[0];

      // Function get and return first available obj_idx from tracker_info.reduced_inactive_obj_ids[]
      // Then shift the indexes that has been left by one place to "left" and put zero at last element.

      for (int32_t idx = 1; idx < num_inactive_obj; ++idx)
      {
         tracker_info.reduced_inactive_obj_ids[idx - 1] = tracker_info.reduced_inactive_obj_ids[idx];
      }

      tracker_info.reduced_inactive_obj_ids[num_inactive_obj - 1] = 0;
      tracker_info.reduced_active_obj_ids[num_obj] = reduced_id;
      tracker_info.reduced_num_active_objs++;

      return reduced_id;
   }

   /*===========================================================================*\
      * FUNCTION: Select_Obj_Tracks_to_Downselect()
      *===========================================================================
      * RETURN VALUE:
      * none
      *
      * PARAMETERS:
      * const F360_Host_T& host,
      * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      * F360_Tracker_Info_T& tracker_info,
      * const F360_Calibrations_T& calib,
      * F360_Object_Track_T (& object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      * float32_t (& priorities)[NUMBER_OF_OBJECT_TRACKS],
      * int32_t (& candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
      * uint32_t& candidates_cnt
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
      * This function selects, based on calculated by itself priority, tracks to downselection.
      *
      * PRECONDITIONS:
      * All the Pointers should Point to valid structures.
      *
      * POSTCONDITIONS:
      * None
      *
      \*===========================================================================*/
   void Select_Obj_Tracks_to_Downselect(
      const F360_Host_T& host,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      float32_t(&priorities)[NUMBER_OF_OBJECT_TRACKS],
      int32_t(&candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
      uint32_t& candidates_cnt)
   {
      const BoundingBox overall_confidence_exclusion_box = Define_Overall_Confidence_Exclusion_Box_Around_Host(static_env_polys, calib, host.dist_rear_axle_to_vcs_m * 0.5F);

      const bool f_limited_FOV = Determine_FOV_Status(sensors);

      for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[loop_idx] - 1;
         const float32_t track_priority = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[obj_idx]);
         if (track_priority > 0.0F)
         {
            priorities[candidates_cnt] = track_priority;
            candidates_idxs[candidates_cnt] = obj_idx;
            candidates_cnt++;
         }
         else
         {
            if (object_tracks[obj_idx].reduced_id > F360_INVALID_REDUCED_ID)
            {
               Push_Reduced_Id(object_tracks[obj_idx].reduced_id, tracker_info);

               object_tracks[obj_idx].reduced_status = F360_OBJECT_STATUS_INVALID;
               object_tracks[obj_idx].reduced_id = F360_INVALID_REDUCED_ID;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Assign_Reduced_Idxs_To_Prioritized_Tracks()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_Tracker_Info_T& tracker_info,
   * const int32_t (& candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
   * uint32_t (& idxs_of_objs_sorted_by_priority)[NUMBER_OF_OBJECT_TRACKS],
   * const uint32_t candidates_cnt
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
   * This function assigns reduced idxs to prioritized tracks .
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Assign_Reduced_Idxs_To_Prioritized_Tracks(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      const int32_t(&candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
      uint32_t(&ids_of_objs_sorted_by_priority)[NUMBER_OF_OBJECT_TRACKS],
      const uint32_t candidates_cnt)
   {
      for (uint32_t loop_idx = 0U; loop_idx < candidates_cnt; loop_idx++)
      {
         const int32_t obj_idx = candidates_idxs[ids_of_objs_sorted_by_priority[loop_idx]];

         if (object_tracks[obj_idx].reduced_id > F360_INVALID_REDUCED_ID)
         {
            object_tracks[obj_idx].reduced_status = object_tracks[obj_idx].status;
            object_tracks[obj_idx].time_since_downselected += tracker_info.elapsed_time_s;
            tracker_info.reduced_obj_ids[object_tracks[obj_idx].reduced_id - 1] = obj_idx + 1;
         }
         else
         {
            // create new reduced track
            const int32_t reduced_id = Pop_Reduced_Id(tracker_info);
            assert(reduced_id > F360_INVALID_REDUCED_ID);
            object_tracks[obj_idx].reduced_status = F360_OBJECT_STATUS_NEW;
            object_tracks[obj_idx].reduced_id = reduced_id;
            object_tracks[obj_idx].time_since_downselected = 0.0F;
            tracker_info.reduced_obj_ids[reduced_id - 1] = obj_idx + 1;
         }
         object_tracks[obj_idx].f_moveable = object_tracks[obj_idx].f_moveable || object_tracks[obj_idx].f_moving;
      }
   }

   /*===========================================================================*\
  * FUNCTION: Deselect_Existing_Reduced_Tracks()
  *===========================================================================
  * RETURN VALUE:
  * none
  *
  * PARAMETERS:
  * F360_Object_Track_T (& object_tracks)[NUMBER_OF_OBJECT_TRACKS],
  * F360_Tracker_Info_T& tracker_info,
  * int32_t (& candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
  * uint32_t (& idxs_of_objs_sorted_by_priority)[NUMBER_OF_OBJECT_TRACKS],
  * uint32_t& candidates_cnt
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
  * This function deselects any existing reduced tracks now ranked lower than the number of reduced slots.
  *
  * PRECONDITIONS:
  * All the Pointers should Point to valid structures.
  *
  * POSTCONDITIONS:
  * None
  *
  \*===========================================================================*/
   void Deselect_Existing_Reduced_Tracks(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      int32_t(&candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
      uint32_t(&ids_of_objs_sorted_by_priority)[NUMBER_OF_OBJECT_TRACKS],
      uint32_t& candidates_cnt)
   {
      for (uint32_t loop_idx = tracker_info.variant.num_reduced_tracks; loop_idx < candidates_cnt; loop_idx++)
      {
         const int32_t obj_idx = candidates_idxs[ids_of_objs_sorted_by_priority[loop_idx]];
         if (object_tracks[obj_idx].reduced_id > F360_INVALID_REDUCED_ID)
         {
            Push_Reduced_Id(object_tracks[obj_idx].reduced_id, tracker_info);

            object_tracks[obj_idx].reduced_status = F360_OBJECT_STATUS_INVALID;
            object_tracks[obj_idx].reduced_id = F360_INVALID_REDUCED_ID;
         }
      }
      candidates_cnt = tracker_info.variant.num_reduced_tracks;
   }

   /*===========================================================================*\
      * FUNCTION: Decrease_Priority_and_Confidence_for_Implausible_Tracks()
      *===========================================================================
      * RETURN VALUE:
      * None
      *
      * PARAMETERS:
      * F360_Object_Track_T & obj_trk,
      * const F360_Tracker_Info_T &tracker_info,
      * const F360_Calibrations_T &calib,
      * const float32_t priority
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
      * This function decreases object confidence level and priority if object has high mirror probability or is 
      * suspected highway ghost
      *
      * PRECONDITIONS:
      * All the References should point to valid structures.
      *
      * POSTCONDITIONS:
      * None
      *
      \*===========================================================================*/
   void Decrease_Priority_and_Confidence_for_Implausible_Tracks(
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T &calib,
      F360_Object_Track_T& obj_trk,
      float32_t& priority)
   {
      const bool f_possibly_highway_ghost = (tracker_info.f_highway_suspected) && (obj_trk.behind_sep_id != F360_INVALID_UNSIGNED_ID) && (std::abs(obj_trk.vcs_heading.Value()) < 0.75F) && (obj_trk.speed > 10.0F);

      // If object is supposed to be a ghost or mirror decrease its confidence and priority.
      if (f_possibly_highway_ghost || (obj_trk.mirror_prob > calib.k_mirror_prob_threshold))
      {
         if (obj_trk.confidenceLevel > calib.k_track_downselect_confidence_level_lowering_factor * calib.low_confidence_level_thresh)
         {
            obj_trk.confidenceLevel = calib.k_track_downselect_confidence_level_lowering_factor * calib.low_confidence_level_thresh;
         }

         priority = 0.0F; // object is not a candidate for downselection.
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Unreliable_Low_Conf_Moveable_Track()
   *===========================================================================
   * RETURN VALUE:
   * bool f_unreliable
   *
   * PARAMETERS:
   * F360_Object_Track_T& obj_trk,
   * const F360_Host_T& host,
   * const F360_Calibrations_T& calib,
   * const BoundingBox& exclusion_box
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
   * Determines if low confidence track is reliable or not.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Unreliable_Low_Conf_Moveable_Track(
      const F360_Object_Track_T& obj_trk,
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const BoundingBox& exclusion_box,
      const bool f_recently_entered_FOV)
   {

      const float32_t obj_heading_difference = std::abs(obj_trk.vcs_heading.Value()) - calib.k_low_conf_expected_abs_object_heading_vcs_in_cta_scenarios;
      const bool f_obj_moving_perpendicular_to_host = std::abs(obj_heading_difference) <= calib.k_low_conf_max_allowed_abs_heading_difference_in_cta_scenarios;
      const bool host_is_slow = host.vcs_speed < calib.k_low_conf_max_allowed_host_speed_in_cta_scenarios;

      const bool f_is_cta_scenario = host_is_slow && f_obj_moving_perpendicular_to_host;
      const CONF3_T conf_threshold_for_low_conf = (f_is_cta_scenario || f_recently_entered_FOV) ? CONF3_Tag::CONF3_MED : CONF3_Tag::CONF3_HIGH;
      constexpr float32_t movable_thr = 0.5F;

      const bool f_low_confidence = ((obj_trk.conf_overall < conf_threshold_for_low_conf) && (!obj_trk.f_suspectable_for_det_drop));
      const bool f_outside_triangular_zone = Is_Outside_Triangular_Zone_Behind_Host(obj_trk.vcs_position, -host.dist_rear_axle_to_vcs_m);
      const bool f_different_heading = Is_Heading_Different_Than_Host(obj_trk, calib);
      const bool f_outside_exclusion_box = Is_Outside_Exclusion_Box(host, obj_trk, exclusion_box, f_recently_entered_FOV);
      const bool f_low_ttc = Has_Low_TTC(host.vcs_speed, obj_trk.vcs_velocity, obj_trk.vcs_position, calib.k_low_conf_unreliability_max_ttc);
      const bool f_not_mature_or_behind = (obj_trk.time_since_initialization < calib.k_low_conf_unreliability_age_thr) || (obj_trk.trk_fltr_type != F360_TRACKER_TRKFLTR_CTCA) || (obj_trk.vcs_position.x < -(host.vehicle_length * 1.5F)); // 1.5 factor to account for possible small error in host vehicle length and also to ensure that object is unambiguously behind host
      const bool f_movable = (obj_trk.movable_prob > movable_thr);


      const bool f_unreliable = (
         f_low_confidence
         && f_outside_triangular_zone
         && (f_different_heading
            || f_outside_exclusion_box)
         && f_low_ttc
         && f_not_mature_or_behind
         && f_movable);

      return f_unreliable;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Outside_Triangular_Zone_Behind_Host()
   *===========================================================================
   * RETURN VALUE:
   * bool f_outside_triangular_zone_behind_host
   *
   * PARAMETERS:
   * const Point& track_posn,
   * const float32_t triangular_zone_long_shift
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
   * Determine if track is outside triangular zone behind host. Zone is defined
   * by two rays spread apart at 90 degree angle, with y axis beeing the symmetry
   * line, with starting point at vcs_x = triangular_zone_long_shift, vcs_y = 0
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Outside_Triangular_Zone_Behind_Host(
      const Point& track_posn,
      const float32_t triangular_zone_long_shift)
   {
      const float32_t translated_posn_x = track_posn.x - triangular_zone_long_shift;
      const bool f_outside_triangular_zone_behind_host = (track_posn.y < translated_posn_x) || (track_posn.y > -translated_posn_x);

      return f_outside_triangular_zone_behind_host;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Heading_Different_Than_Host()
   *===========================================================================
   * RETURN VALUE:
   * bool f_moving_in_specified_direction
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj_trk,
   * const F360_Calibrations_T &calib
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
   * Determines if a CTCA or fast moving CCA object's heading differs significantly from host's.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Heading_Different_Than_Host(
      const F360_Object_Track_T& obj_trk,
      const F360_Calibrations_T& calib)
   {
      const bool f_moving_in_different_direction_than_host = (
         ((F360_TRACKER_TRKFLTR_CTCA == obj_trk.trk_fltr_type) || (std::abs(obj_trk.speed) > calib.fast_moving_thresh))
         && (std::abs(obj_trk.vcs_heading.Value()) > calib.k_low_conf_unreliability_min_heading));

      return f_moving_in_different_direction_than_host;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Outside_Exclusion_Box()
   *===========================================================================
   * RETURN VALUE:
   * bool f_outside_exclusion_box
   *
   * PARAMETERS:
   * const F360_Host_T host,
   * const F360_Object_Track_T& obj_trk,
   * const BoundingBox& exclusion_box,
   * const bool f_recently_entered_FOV
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
   * Determines if a track is outside of rectangular exclusion box defined around the host.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Outside_Exclusion_Box(
      const F360_Host_T host,
      const F360_Object_Track_T& obj_trk,
      const BoundingBox& exclusion_box,
      const bool f_recently_entered_FOV)
   {
       bool f_outside_exclusion_box;
       if (f_recently_entered_FOV)
       {
           const float32_t current_length = exclusion_box.Get_Length();
           const float32_t speed_diff = obj_trk.speed - host.speed;
           BoundingBox extended_exclusion_box = exclusion_box;
           const float32_t extending_factor = F360_Linear_Equation_With_Saturation(speed_diff, 0.0F, 20.0F, 0.0F, 10.0F);
           extended_exclusion_box.Set_Length(current_length + extending_factor);
           f_outside_exclusion_box = !extended_exclusion_box.Contains(obj_trk.vcs_position);
       }
       else
       {
           f_outside_exclusion_box = !exclusion_box.Contains(obj_trk.vcs_position);
       }
      return f_outside_exclusion_box;
   }

   /*===========================================================================*\
   * FUNCTION: Has_Low_TTC()
   *===========================================================================
   * RETURN VALUE:
   * bool f_low_ttc
   *
   * PARAMETERS:
   * const float32_t host_speed,
   * const F360_VCS_Velocity_T& track_velocity,
   * const Point& track_pos,
   * const float32_t k_low_conf_unreliability_max_ttc
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
   * Determines if a track has low TTC in longitudinal direction.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Has_Low_TTC(
      const float32_t host_speed,
      const F360_VCS_Velocity_T& track_velocity,
      const Point& track_pos,
      const float32_t k_low_conf_unreliability_max_ttc)
   {
      const float32_t vel_diff = (host_speed - track_velocity.longitudinal);
      const bool f_low_ttc = (std::abs(vel_diff) > F360_EPSILON) ? ((track_pos.x / vel_diff) < k_low_conf_unreliability_max_ttc) : false;

      return f_low_ttc;
   }


   /*===========================================================================*\
   * FUNCTION: Define_Overall_Confidence_Exclusion_Box_Around_Host()
   *===========================================================================
   * RETURN VALUE:
   * BoundingBox
   *
   * PARAMETERS:
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS] - array of static environment polynomials
   * const F360_Calibrations_T& calib - tracker calibrations
   * const float32_t box_longpos_shift - shift of rectangular exclusion box in longitudinal direction
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
   * Function defines rectangular axis-aligned exclusion box used to limit the scope of
   * overall confidence filter (to not apply it in exclusion box i.e close to host).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   BoundingBox Define_Overall_Confidence_Exclusion_Box_Around_Host(
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& calib,
      const float32_t box_longpos_shift)
   {
      const float32_t exclusion_box_lat = calib.k_conf_downselection_exclusion_box_lat;

      constexpr float32_t k_long_pos_mid = -2.5F;
      constexpr float32_t k_long_margin = 4.0F;
      constexpr float32_t k_min_abs_lat_pos = 0.0F;

      const Closest_SEP_Info closest_left_sep = Find_Closest_SEP_On_Given_Side(static_env_polys, F360_HOST_LEFT_SIDE, k_long_pos_mid, k_long_margin, k_min_abs_lat_pos);
      const float32_t left_limit = (closest_left_sep.id != F360_INVALID_UNSIGNED_ID) ? std::max(closest_left_sep.lat_pos, -exclusion_box_lat) : -exclusion_box_lat;
      const Point rear_left_corner = Point(-calib.k_conf_downselection_exclusion_box_long - box_longpos_shift, left_limit);

      const Closest_SEP_Info closest_right_sep = Find_Closest_SEP_On_Given_Side(static_env_polys, F360_HOST_RIGHT_SIDE, k_long_pos_mid, k_long_margin, k_min_abs_lat_pos);
      const float32_t right_limit = (closest_right_sep.id != F360_INVALID_UNSIGNED_ID) ? std::min(closest_right_sep.lat_pos, exclusion_box_lat) : exclusion_box_lat;
      const Point front_right_corner = Point(calib.k_conf_downselection_exclusion_box_long - box_longpos_shift, right_limit);

      return BoundingBox(rear_left_corner, front_right_corner);
   }

   /*===========================================================================*\
   * FUNCTION: Cond_LP_Filter_Reduced_Det_Num()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Tracker_Info_T& tracker_info,
   *  const float32_t &filtration_factor,
   *  F360_Object_Track_T(&obj_trks)[NUMBER_OF_OBJECT_TRACKS])
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
   * This function applies a low pass filter to the number of reduced detections
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Cond_LP_Filter_Reduced_Det_Num(
      const F360_Tracker_Info_T& tracker_info,
      const float32_t &filtration_factor,
      F360_Object_Track_T(&obj_trks)[NUMBER_OF_OBJECT_TRACKS])
   {
      for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
      {
         const uint32_t ndets = obj_trks[loop_idx].num_rr_inlier_dets;

         const bool f_make_filtration = !((0U < ndets) && (static_cast<float32_t>(ndets) < obj_trks[loop_idx].filtered_dets));

         if (F360_OBJECT_STATUS_NEW_UPDATED == obj_trks[loop_idx].status)
         {
            obj_trks[loop_idx].filtered_dets = 0.5F; // initial value of low pass filter
         }

         if (f_make_filtration)
         {
            obj_trks[loop_idx].filtered_dets = (filtration_factor * obj_trks[loop_idx].filtered_dets) + ((1.0F - filtration_factor) * static_cast<float32_t>(ndets));
         }

      }
   }

   /*===========================================================================*\
   * FUNCTION: Determine_FOV_Status()
   *===========================================================================
   * RETURN VALUE:
   * bool f_limited_FOV
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
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
   * This function determines whether sensors' FOV is limited or not.
   * FOV is limited if some vcs azimuths are not visible to any sensor.
   * It does so by checking whether whole vcs azimuth interval [-PI,PI] is covered by azimuths interval of each sensor.
   *
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Determine_FOV_Status(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS])
   {
       bool f_limited_FOV = true;


       float32_t min_azimuths[MAX_NUMBER_OF_SENSORS] = {};
       float32_t max_azimuths[MAX_NUMBER_OF_SENSORS] = {};
       uint8_t num_sensors = 0U;
       for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
       {
           if (sensors[i].variable.is_valid)
           {
               const float32_t boresight_az = sensors[i].variable.vacs_boresight_az_estimated;
               const float32_t min_az = boresight_az + F360_Min_Element(sensors[i].constant.fov_min_az_rad, F360_NUM_LOOK_ID);
               const float32_t max_az = boresight_az + F360_Max_Element(sensors[i].constant.fov_max_az_rad, F360_NUM_LOOK_ID);
               min_azimuths[num_sensors] = min_az;
               max_azimuths[num_sensors] = max_az;
               num_sensors++;
           }
       }

       if (num_sensors > 1U)
       {
           // arrays of minimum and maximum azimuths
           uint32_t min_az_sorted_idx[MAX_NUMBER_OF_SENSORS] = {};
           (void)F360_Sort(static_cast<uint32_t>(num_sensors), true, min_azimuths, min_az_sorted_idx);

           float32_t current_max_az = max_azimuths[min_az_sorted_idx[0]];
           num_sensors = std::min(num_sensors, MAX_NUMBER_OF_SENSORS); // need to double-ensure i < MAX_NUMBER_OF_SENSORS, otherwise compiler may reject it if MAX_NUMBER_OF_SENSORS = 1

           for (uint8_t i = 1U; i < num_sensors; i++)
           {
               if (min_azimuths[i] > current_max_az)
               {
                   break;
               }
               current_max_az = std::max(current_max_az, max_azimuths[min_az_sorted_idx[i]]);
           }
           if (current_max_az - 2.0F * F360_PI > min_azimuths[0])
           {
               f_limited_FOV = false;
           }
       }

       return f_limited_FOV;
   }

   /*===========================================================================*\
   * FUNCTION: Has_Object_Recently_Entered_FOV()
   *===========================================================================
   * RETURN VALUE:
   * bool f_recently_entered_FOV
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Object_Track_T& obj_trk
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
   * This function determines whether object has recently entered FOV.
   * It does so by translating track's position by its velocity vector and host motion
   * and checking whether it was in FOV then.
   *
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Has_Object_Recently_Entered_FOV(
       const F360_Host_T& host,
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       const F360_Object_Track_T& obj_trk)
   {
       bool f_recently_entered_FOV = true;

       const bool f_new_object = (obj_trk.time_since_initialization < 0.7F);
       const bool f_driving_straight = (std::abs(obj_trk.vcs_heading.Value()) < 0.3F) || (std::abs(std::abs(obj_trk.vcs_heading.Value()) - F360_PI) < 0.3F);
       const bool f_fast = (obj_trk.speed > 10.0F);

       if ((f_new_object) && (f_driving_straight) && (f_fast))
       {
           Point position = obj_trk.bbox.Get_Center();

           const float32_t time_interval = 1.0F;

           //translate current position by its velocity vector and host motion to determine where it was time_interval ago
           position.x -= obj_trk.vcs_velocity.longitudinal * time_interval;
           position.y -= obj_trk.vcs_velocity.lateral * time_interval;

           // translation of position with respect to host motion
           const float32_t delta_pointing = host.yaw_rate_rad * time_interval;
           const float32_t cos_delta_pointing = F360_Cosf(delta_pointing);
           const float32_t sin_delta_pointing = F360_Sinf(delta_pointing);
           position.x += cos_delta_pointing * host.speed * time_interval;
           position.y += sin_delta_pointing * host.speed * time_interval;

           // determine if object would be in FOV short time ago
           for (uint8_t j = 0U; j < MAX_NUMBER_OF_SENSORS; j++)
           {

               const F360_Radar_Sensor_T sensor = sensors[j];
               if (sensor.variable.is_valid)
               {
                   const float32_t left_fov_normal_vector[2] = { sensor.refined.left_fov_normal[F360_DET_LOOK_ID_0], sensor.refined.left_fov_normal[F360_DET_LOOK_ID_1] };
                   const float32_t right_fov_normal_vector[2] = { sensor.refined.right_fov_normal[F360_DET_LOOK_ID_0], sensor.refined.right_fov_normal[F360_DET_LOOK_ID_1] };

                   if (Is_Point_Inside_FOV(position, sensor, left_fov_normal_vector, right_fov_normal_vector))
                   {
                       f_recently_entered_FOV = false;
                       break;
                   }
               }
           }
       }
       else
       {
           f_recently_entered_FOV = false;
       }

       return f_recently_entered_FOV;
   }
}

