/*===================================================================================*\
* FILE:  f360_reflector_selector.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Object_Track_Reflector_Selector class methods.
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_reflector_selector.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Object_Track_Reflector_Selector::Set_Outer_Zone()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t half_zone_legth
   * const float32_t half_zone_width
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
   * This function defines outer zone for Multipath detector to limit number of objects.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Object_Track_Reflector_Selector::Set_Outer_Zone(
      const float32_t half_zone_legth, 
      const float32_t half_zone_width)
   {
      m_outer_zone[0][0] = -half_zone_legth;
      m_outer_zone[0][1] = half_zone_legth;
      m_outer_zone[1][0] = -half_zone_width;
      m_outer_zone[1][1] = half_zone_width;
   }

   /*===========================================================================*\
   * FUNCTION: Object_Track_Reflector_Selector::Select_Stationary_Objects_Inside_Outer_Zone()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T & tracker_info
   * const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function limits number of object for further processing.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Object_Track_Reflector_Selector::Select_Stationary_Objects_Inside_Outer_Zone(
      const F360_Tracker_Info_T & tracker_info)
   {
      m_num_objs_in_outer_zone = 0;

      const F360_Object_Track_T* curr_trk = tracker_info.vcslong_sorted_start; // Start with first object in list
      for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
      {

         if ((!curr_trk->f_moving) && Check_If_Point_Is_Inside_Box_In_Same_CS(curr_trk->bbox.Get_Center().x, curr_trk->bbox.Get_Center().y, m_outer_zone))
         {
            m_obj_ids_in_outer_zone[m_num_objs_in_outer_zone] = curr_trk->id;
            m_num_objs_in_outer_zone++;
         }

         // take next object from list
         curr_trk = tracker_info.vcslong_sorted_next_track[curr_trk->id - 1];
      }
   }

   /*===========================================================================*\
   * FUNCTION: Object_Track_Reflector_Selector::Object_Track_Reflector_Selector()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t half_zone_length
   * const float32_t half_zone_width
   * const float32_t size_extension,
   * const F360_Tracker_Info_T & tracker_info
   * const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
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
   * This is parametized constructor for Object_Track_Reflector_Selector class.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Object_Track_Reflector_Selector::Object_Track_Reflector_Selector(
      const float32_t half_zone_length,
      const float32_t half_zone_width,
      const float32_t size_extension,
      const F360_Tracker_Info_T & tracker_info) :
      m_num_objs_in_outer_zone(0), m_objects_nb_in_inner_zone(0), m_size_extension(size_extension)
   {
      Set_Outer_Zone(half_zone_length, half_zone_width);
      Select_Stationary_Objects_Inside_Outer_Zone(tracker_info);
   }

   /*===========================================================================*\
   * FUNCTION: Object_Track_Reflector_Selector::Set_Inner_Zone()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const Point &sensor_posn
   * const Point &item_posn
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
   * This function defines inner zone that is subset outer zone.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Object_Track_Reflector_Selector::Set_Inner_Zone(
      const Point &sensor_posn,
      const Point &item_posn)
   {
      const float inner_zone_offset = 0.1F;

      if (item_posn.x > sensor_posn.x)
      {
         m_inner_zone[0][1] = item_posn.x;
         m_inner_zone[0][0] = sensor_posn.x;
      }
      else if (item_posn.x < sensor_posn.x)
      {
         m_inner_zone[0][1] = sensor_posn.x;
         m_inner_zone[0][0] = item_posn.x;
      }
      else
      {
         m_inner_zone[0][1] = item_posn.x + inner_zone_offset;
         m_inner_zone[0][0] = item_posn.x - inner_zone_offset;
      }

      if (item_posn.y > sensor_posn.y)
      {
         m_inner_zone[1][1] = item_posn.y;
         m_inner_zone[1][0] = sensor_posn.y;
      }
      else if (item_posn.y < sensor_posn.y)
      {
         m_inner_zone[1][1] = sensor_posn.y;
         m_inner_zone[1][0] = item_posn.y;
      }
      else
      {
         m_inner_zone[1][1] = item_posn.y + inner_zone_offset;
         m_inner_zone[1][0] = item_posn.y - inner_zone_offset;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Object_Track_Reflector_Selector::Select_Objects_Inside_Inner_Zone()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function selects objects inside inner zone.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Object_Track_Reflector_Selector::Select_Objects_Inside_Inner_Zone(
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   )
   {
      m_objects_nb_in_inner_zone = 0;
      for (int32_t idx = 0; idx < m_num_objs_in_outer_zone; idx++)
      {
         const int32_t obj_id = m_obj_ids_in_outer_zone[idx];
         const float32_t x_pos = object_tracks[obj_id - 1].bbox.Get_Center().x;
         const float32_t y_pos = object_tracks[obj_id - 1].bbox.Get_Center().y;
         
         // Stationary objects created out of low rcs detections shouldn't be source of reflections
         const float32_t object_dist_squared = x_pos * x_pos + y_pos * y_pos;
         const float32_t range_dependent_rcs_threshold = object_dist_squared > 100.0F ? 0.0F : -12.0F;  // Objects near the host in general have lower rcs, so for objects within 10m (squared distances < 100 m^2) we set rcs threshold to -12dBsm, for objects farther than 10m we set rcs threshold to 0dBsm
         if (Check_If_Point_Is_Inside_Box_In_Same_CS(x_pos, y_pos, m_inner_zone) && (object_tracks[obj_id - 1].average_rcs > range_dependent_rcs_threshold))  // Selector candidates should have higher rcs than threshold to be selected as reflectors.
         {
            m_obj_ids_inside_inner_zone[m_objects_nb_in_inner_zone] = obj_id;
            m_objects_nb_in_inner_zone++;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Object_Track_Reflector_Selector::Find_Closest_Reflector()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const Point &sensor_posn
   * const Point &item_posn
   * const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function finds object closest to sensor which has intersection
   * with line conecting this sensor with potential ghost(item_posn).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Object_Track_Reflector_Selector::Find_Closest_Reflection_Point(
      const Point &sensor_posn,
      const Point &item_posn,
      const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      float32_t shortest_sqrd_dist = INFTY;
      std::pair<bool, Point> intersection = { false, {} };

      for (int32_t idx = 0; idx < m_objects_nb_in_inner_zone; ++idx)
      {
         const int32_t obj_id = m_obj_ids_inside_inner_zone[idx];

         const Object_Reflector reflector_object = Object_Reflector(objects[obj_id - 1]);

         intersection = reflector_object.Find_Intersection(sensor_posn, item_posn);

         if (intersection.first)
         {
            const float32_t sensor_to_obj_sqrd_dist = Calc_Two_2D_Point_Sqrd_Distance(sensor_posn, intersection.second);

            if (sensor_to_obj_sqrd_dist < shortest_sqrd_dist)
            {
               shortest_sqrd_dist = sensor_to_obj_sqrd_dist;
               m_reflection_point = intersection;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Object_Track_Reflector_Selector::Find_Closest_Reflection_Point()
   * ===========================================================================
   * RETURN VALUE:
   * std::pair<bool, Point>
   *
   * PARAMETERS:
   * const Point &sensor_posn
   * const Point &item_posn
   * const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
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
   * This function finds object closest to sensor and returns intersection point its bbox
   * with the line conecting sensor position and item position.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   std::pair<bool, Point> Object_Track_Reflector_Selector::Get_Reflection_Point(
      const Point &sensor_posn,
      const Point &item_posn,
      const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      m_reflection_point = { false, {} };

      Set_Inner_Zone(sensor_posn, item_posn);
      Select_Objects_Inside_Inner_Zone(objects);
      Find_Closest_Reflection_Point(sensor_posn, item_posn, objects);

      return m_reflection_point;
   }

   /*===========================================================================*\
   * FUNCTION: Object_Track_Reflector_Selector::Calc_Two_2D_Point_Sqrd_Distance()
   * ===========================================================================
   * RETURN VALUE:
   * float32_t
   *
   * PARAMETERS:
   * const Point &point_1
   * const Point &point_2
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
   * This function calculates cartesian distance between two points on 2D plane.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Object_Track_Reflector_Selector::Calc_Two_2D_Point_Sqrd_Distance(
      const Point &point_1,
      const Point &point_2)
   {
      const float32_t x_dist = point_1.x - point_2.x;
      const float32_t y_dist = point_1.y - point_2.y;
      return (x_dist * x_dist + y_dist * y_dist);
   }
}
