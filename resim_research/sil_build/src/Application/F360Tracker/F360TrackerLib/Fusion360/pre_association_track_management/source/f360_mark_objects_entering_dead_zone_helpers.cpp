/*===================================================================================*\
* FILE: f360_mark_objects_entering_dead_zone_helpers.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of supporting functions used by Mark_Objects_Enterning_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_mark_objects_entering_dead_zone_helpers.h"
#include "f360_math_func.h"
#include "f360_bounding_box.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_In_Dead_Zone()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_inside_dead_zone - Flag indicating whether object center position is in dead zone
   *
   * PARAMETERS:
   * const float32_t center_long_pos - Object center longitudinal position
   * const Interval<float32_t>& zone - Dead zone longitudinal limits
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
   * Function verifies whether object center longitudinal position is in dead zone
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Center_In_Dead_Zone(
      const float32_t center_long_pos,
      const Interval<float32_t>& zone)
   {
      const bool f_is_inside_dead_zone = zone.Contains(center_long_pos);

      return f_is_inside_dead_zone;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Rear_In_Zone()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_rear_inside_dead_zone - Flag indicating whether any of objects
   *                                           rear corners is within dead zone
   *
   * PARAMETERS:
   * const BboxCorners& bbox           - Object bounding box corners
   * const Interval<float32_t>& zone  - Dead zone longitudinal limits
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
   * Function verifies whether any of objects rear corners is inside dead zone.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Rear_In_Zone(
      const BboxCorners& bbox,
      const Interval<float32_t>& zone)
   {
      const bool f_is_rear_inside_dead_zone = zone.Contains(bbox.Rear_Left().x) || zone.Contains(bbox.Rear_Right().x);

      return f_is_rear_inside_dead_zone;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Front_In_Zone()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_front_inside_dead_zone - Flag indicating whether any of objects
   *                                            front corners is within dead zone
   *
   * PARAMETERS:
   * const BboxCorners& bbox           - Object bounding box corners
   * const Interval<float32_t>& zone  - Dead zone longitudinal limits
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
   * Function verifies whether any of objects front corners is inside dead zone.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Front_In_Zone(
      const BboxCorners& bbox,
      const Interval<float32_t>& zone)
   {
      const bool f_is_front_inside_dead_zone = zone.Contains(bbox.Front_Left().x) || zone.Contains(bbox.Front_Right().x);

      return f_is_front_inside_dead_zone;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Object_Dead_Zone_Status()
   *===========================================================================
   * RETURN VALUE:
   * F360_Dead_Zone_Status_T dead_zone_status - Object dead zone status
   *
   * PARAMETERS:
   * const Dead_Zone_T& dead_zone                     - Dead zone longitudinal limits
   * const F360_Object_Track_T& object                - Analysed object
   * const float32_t k_dead_zone_max_obj_vcs_lat_pos - Max lateral position of the object
   *                                                    to be considered in dead zone
   * const float32_t k_dead_zone_max_obj_vcs_heading - Max vcs_heading of the object to
   *                                                    be considered in dead zone
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
   * Function determines object dead zone status
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   F360_Dead_Zone_Status_T Determine_Object_Dead_Zone_Status(
      const Dead_Zone_T& dead_zone,
      const F360_Object_Track_T& object,
      const float32_t k_dead_zone_max_obj_vcs_lat_pos, 
      const float32_t k_dead_zone_max_obj_vcs_heading)
   {
      F360_Dead_Zone_Status_T dead_zone_status;

      if ((std::abs(object.vcs_position.y) < k_dead_zone_max_obj_vcs_lat_pos) &&
         (std::abs(object.vcs_heading.Value()) < k_dead_zone_max_obj_vcs_heading))
      {
         const BboxCorners bbox = object.bbox.Get_Corners();

         if (Is_Center_In_Dead_Zone(object.bbox.Get_Center().x, dead_zone.basic))
         {
            dead_zone_status = F360_Dead_Zone_Status_T::INSIDE;
         }
         else if (Is_Front_In_Zone(bbox, dead_zone.basic))
         {
            dead_zone_status = F360_Dead_Zone_Status_T::IN_REAR;
         }
         else if (Is_Rear_In_Zone(bbox, dead_zone.basic))
         {
            dead_zone_status = F360_Dead_Zone_Status_T::IN_FRONT;
         }
         else if (Is_Front_In_Zone(bbox, Interval<float32_t>{dead_zone.extended.lower, dead_zone.basic.lower}))
         {
            dead_zone_status = F360_Dead_Zone_Status_T::ENTERING_REAR;
         }
         else if (Is_Rear_In_Zone(bbox, Interval<float32_t>{ dead_zone.basic.upper, dead_zone.extended.upper }))
         {
            dead_zone_status = F360_Dead_Zone_Status_T::ENTERING_FRONT;
         }
         else
         {
            dead_zone_status = F360_Dead_Zone_Status_T::OUTSIDE;
         }
      }
      else
      {
         dead_zone_status = F360_Dead_Zone_Status_T::OUTSIDE;
      }

      return dead_zone_status;
   }
}
