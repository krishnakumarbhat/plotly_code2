/*===================================================================================*\
* FILE: f360_update_extended_bbox_offsets_for_object_in_dead_zone.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_update_extended_bbox_offsets_for_object_in_dead_zone.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib     - Tracker calibrations
   * const float32_t host_speed          - Host speed
   * const Dead_Zone_T& dead_zone         - Dead zone longitudinal limits
   * const F360_Object_Track_T& object    - Analysed object
   * float32_t& rear_offset              - Offset from rear bumper defining rear association gate
   * float32_t& front_offset             - Offset from front bumper defining front association gate
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
   * Function verifies whether objects offsets defining extended bounding box (association gates)
   * should be increased, and if yes - it does it.
   *
   * PRECONDITIONS:
   * Dead zone front and rear limit contain signed values.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(
      const F360_Calibrations_T& calib,
      const float32_t host_speed,
      const Dead_Zone_T& dead_zone,
      const F360_Object_Track_T& object,
      float32_t& rear_offset,
      float32_t& front_offset)
   {
      const float32_t host_speed_div = std::max(host_speed, F360_EPSILON);
      const float32_t rel_vel_diff = std::abs((host_speed - object.speed) / host_speed_div);

      if ((F360_Dead_Zone_Status_T::OUTSIDE != object.dead_zone_status) &&
         (F360_Dead_Zone_Status_T::UNDEFINED != object.dead_zone_status) &&
         ((F360_TRACKER_TRKFLTR_CTCA == object.trk_fltr_type) || (std::abs(object.speed) > calib.fast_moving_thresh)) &&
         (rel_vel_diff < calib.k_dead_zone_max_rel_vel_diff) &&
         (F360_INVALID_UNSIGNED_ID == object.on_sep_id) &&
         (F360_INVALID_UNSIGNED_ID == object.behind_sep_id))
      {
         float32_t desired_front_offset = front_offset;
         float32_t desired_rear_offset = rear_offset;

         const float32_t rear_bumper_position = object.bbox.Get_Center().x - 0.5F * object.bbox.Get_Length();
         const float32_t front_bumper_position = object.bbox.Get_Center().x + 0.5F * object.bbox.Get_Length();
         bool f_saturate_when_entering_front = false;

         if (F360_Dead_Zone_Status_T::INSIDE == object.dead_zone_status)
         {
            desired_front_offset = dead_zone.basic.upper - front_bumper_position + calib.k_dead_zone_assoc_gates_additional_enhacementl; // extend to front sensor postion + additional calib value
            desired_rear_offset = rear_bumper_position - dead_zone.basic.lower + calib.k_dead_zone_assoc_gates_additional_enhacementl; // extend to rear sensor position + additional calib value
         }
         else if (F360_Dead_Zone_Status_T::IN_REAR == object.dead_zone_status)
         {
            desired_front_offset = dead_zone.basic.upper - front_bumper_position + calib.k_dead_zone_assoc_gates_additional_enhacementl; // extend to front sensor position + additional calib value
            desired_rear_offset = rear_bumper_position - dead_zone.extended.lower; // extend to extended dead zone rear position
         }
         else if (F360_Dead_Zone_Status_T::ENTERING_REAR == object.dead_zone_status)
         {
            desired_front_offset = dead_zone.basic.lower - front_bumper_position + calib.k_dead_zone_assoc_gates_additional_enhacementl; // extend to rear sensor position + calib value
            desired_rear_offset = rear_bumper_position - dead_zone.extended.lower; // extend to extended dead zone rear position
         }
         else if (F360_Dead_Zone_Status_T::IN_FRONT == object.dead_zone_status)
         {
            desired_front_offset = dead_zone.extended.upper - front_bumper_position; // extend to extended dead zone front position
            desired_rear_offset = rear_bumper_position - dead_zone.basic.lower + calib.k_dead_zone_assoc_gates_additional_enhacementl; // extend to rear sensor position + calib value
         }
         else if (F360_Dead_Zone_Status_T::ENTERING_FRONT == object.dead_zone_status)
         {
            desired_front_offset = dead_zone.extended.upper - front_bumper_position; // extend to extended dead zone front position
            desired_rear_offset = rear_bumper_position - dead_zone.basic.upper + calib.k_dead_zone_assoc_gates_additional_enhacementl; // extend to front sensor posiiton + additional calib value
            // To make offset limited when considering AEB logic:
            f_saturate_when_entering_front = true;
         }
         else
         {
            // Do nothing
         }

         constexpr float32_t min_host_speed_weight_threshold = 0.0F;
         constexpr float32_t max_host_speed_weight_threshold = 15.0F;
         constexpr float32_t max_weight_value = 1.0F;
         constexpr float32_t min_weight_value = 0.0F;

         rear_offset = std::max(rear_offset, F360_Linear_Equation_With_Saturation(host_speed, min_host_speed_weight_threshold, max_host_speed_weight_threshold, min_weight_value, max_weight_value) * desired_rear_offset);
         front_offset = std::max(front_offset, F360_Linear_Equation_With_Saturation(host_speed, min_host_speed_weight_threshold, max_host_speed_weight_threshold, min_weight_value, max_weight_value) * desired_front_offset);
         
         if (f_saturate_when_entering_front)
         {
            constexpr float32_t dead_zone_offset_saturation = 4.0F;
            rear_offset = std::min(rear_offset, dead_zone_offset_saturation);
            front_offset = std::min(front_offset, dead_zone_offset_saturation);

         }
      }
   }
}
