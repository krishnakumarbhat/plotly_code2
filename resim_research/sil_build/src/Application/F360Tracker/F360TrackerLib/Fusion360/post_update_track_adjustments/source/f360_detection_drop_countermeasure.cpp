#include "f360_detection_drop_countermeasure.h"
#include "f360_find_closest_valid_sep_on_given_side.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Flag_Object_Suspectable_For_Detection_Drop()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T & tracker_info - Tracker information containing variant type and active objects
   * const float32_t host_speed - Current host vehicle speed in m/s
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - Array of object tracks to be evaluated
   *
   * EXTERNAL REFERENCES:
   * Is_Object_Within_Detection_Drop_Zone()
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Flags objects that are suspectable for detection drop.
   * 
   * PRECONDITIONS:
   * - tracker_info.num_active_objs must be valid
   * - object_tracks array must contain valid track data
   *
   * POSTCONDITIONS:
   * - Sets f_suspectable_for_det_drop flag to true for objects meeting all criteria
   * - Sets f_suspectable_for_det_drop flag to false for objects not meeting criteria
   *
   \*===========================================================================*/
   void Flag_Object_Suspectable_For_Detection_Drop_Variant_K(
      const F360_Tracker_Info_T & tracker_info,
      const float32_t host_speed,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      if ((tracker_info.variant.type == F360_Tracker_Variant_T::F360_VARIANT_TYPE_K) && (host_speed < 8.33F)) // 30 km/h
      {
         for (int32_t i = 0; i < tracker_info.num_active_objs; ++i)
         {
            const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
            F360_Object_Track_T& obj_trk = object_tracks[obj_idx];

            const bool f_in_zone = Is_Object_Within_Detection_Drop_Zone(obj_trk);
            const bool f_is_moving = obj_trk.f_moving;
            const bool f_low_speed = (std::abs(obj_trk.speed) < 8.33F); // 30 km/h
            const bool f_status_ok = obj_trk.status >= F360_OBJECT_STATUS_UPDATED;
            const float32_t heading_deg_abs = std::abs(obj_trk.vcs_heading.Value_Deg());
            const bool f_heading_ok = (heading_deg_abs < 20.0F) || (heading_deg_abs > 160.0F);
            const bool f_size_ok = (obj_trk.bbox.Get_Length() < 2.5F) && (obj_trk.bbox.Get_Width() < 1.5F);

            if (f_is_moving && f_low_speed && f_in_zone && f_status_ok && f_heading_ok && f_size_ok)
            {
               obj_trk.f_suspectable_for_det_drop = true;
            }
            else
            {
               obj_trk.f_suspectable_for_det_drop = false;
            }
         }
      }
      else
      {
         for (int32_t i = 0; i < tracker_info.num_active_objs; ++i)
         {
            const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
            object_tracks[obj_idx].f_suspectable_for_det_drop = false;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Within_Detection_Drop_Zone()
   *===========================================================================
   * RETURN VALUE:
   * bool - Returns true if object is within the detection drop zone, false otherwise
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj_trk - Object track to evaluate
   *
   * EXTERNAL REFERENCES:
   * Find_Closest_SEP_On_Given_Side()
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determines if an object is within the detection drop zone detection loss may occur. 
   *
   * PRECONDITIONS:
   * - obj_trk must contain valid position data
   * POSTCONDITIONS:
   * - Returns true if object is within zone boundaries
   * - Returns false if object is outside zone boundaries
   *
   \*===========================================================================*/
   bool Is_Object_Within_Detection_Drop_Zone(
      const F360_Object_Track_T& obj_trk)
   {
      bool f_in_zone = false;
      if ((std::abs(obj_trk.vcs_position.x) < 20.0F) && (std::abs(obj_trk.vcs_position.y) < 10.0F) && (obj_trk.behind_sep_id == F360_INVALID_UNSIGNED_ID))
      {
         f_in_zone = true;
      }
      return f_in_zone;
   }
}
