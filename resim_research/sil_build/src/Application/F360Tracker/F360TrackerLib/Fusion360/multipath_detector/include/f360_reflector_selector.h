/*===================================================================================*\
* FILE:  f360_reflector_selector.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of Object_Track_Reflector_Selector class.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef F360_OBJECT_TRACK_REFLECTOR_SELECTOR_H
#define F360_OBJECT_TRACK_REFLECTOR_SELECTOR_H

#include "f360_tracker_info.h"
#include "f360_reflector_object.h"
#include "f360_check_if_point_is_inside_box.h"

namespace f360_variant_A
{
   class Object_Track_Reflector_Selector
   {
   public:
      Object_Track_Reflector_Selector(
         const float32_t half_zone_length,
         const float32_t half_zone_width,
         const float32_t size_extension,
         const F360_Tracker_Info_T &tracker_info);

      std::pair<bool, Point> Get_Reflection_Point(
         const Point &sensor_posn,
         const Point &item_posn,
         const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]);

   private:
      std::pair<bool, Point> m_reflection_point = { false, {} };
      float32_t m_outer_zone[2][2] = {};         // Zone borders: [0][0] - rear, [0][1] - front, [1][0] - left, [1][1] - right
      float32_t m_inner_zone[2][2] = {};         // Subset of outer zone.
      int32_t m_obj_ids_in_outer_zone[NUMBER_OF_OBJECT_TRACKS] = {};
      int32_t m_obj_ids_inside_inner_zone[NUMBER_OF_OBJECT_TRACKS] = {};
      int32_t m_num_objs_in_outer_zone;      // Objects number inside outer zone
      int32_t m_objects_nb_in_inner_zone;    // Objects number inside inner zone
      float32_t m_size_extension;              // Object size extension

      void Set_Outer_Zone(const float32_t half_zone_legth, const float32_t half_zone_width);
      void Select_Stationary_Objects_Inside_Outer_Zone(const F360_Tracker_Info_T &tracker_info);
      void Set_Inner_Zone(const Point &sensor_posn, const Point &item_posn);
      void Select_Objects_Inside_Inner_Zone(const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);
      void Find_Closest_Reflection_Point(const Point &sensor_posn, const Point &item_posn, const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]);
      static float32_t Calc_Two_2D_Point_Sqrd_Distance(const Point &point_1, const Point &point_2);
   };
}

#endif
