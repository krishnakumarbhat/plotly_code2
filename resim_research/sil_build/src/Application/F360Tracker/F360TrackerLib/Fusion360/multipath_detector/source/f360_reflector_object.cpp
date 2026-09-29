/*===================================================================================*\
* FILE: f360_reflector_object.cpp
*====================================================================================
* Copyright (C) 2021-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Reflector interface class.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#include "f360_reflector_object.h"
#include "f360_intersections.h"
#include "f360_range_rates.h"
#include "f360_points_with_extreme_azimuth.h"
#include "f360_intersections.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Object_Reflector()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * None.
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
   * Default constructor.
   *
   * PRECONDITIONS:
   * Object used for relfector creation should exists as long as reflector.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Object_Reflector::Object_Reflector()
   {
      m_object = nullptr;
   }

   /*===========================================================================*\
   * FUNCTION: Object_Reflector()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Object_Track_T &object - Object, based on, calculations are made.
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
   * Parameterized constructor
   *
   * PRECONDITIONS:
   * Object used for relfector creation should exists as long as reflector.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Object_Reflector::Object_Reflector(const F360_Object_Track_T &object)
      : m_object(&object)
   {
      Calc_Extended_Corners();
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Extended_Corners()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * None.
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
   * Calculate extended bounding box corners based on current state and calibrations
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Object_Reflector::Calc_Extended_Corners()
   {
      const float32_t length = m_object->bbox.Get_Length();
      const float32_t width = m_object->bbox.Get_Width();
      const float32_t extended_len_1 = length;
      const float32_t extended_len_2 = extended_len_1;
      const float32_t extended_wid_1 = width;
      const float32_t extended_wid_2 = extended_wid_1;

      Point extended_corners_tcs[NUM_CORNERS_IN_RECTANGLE] = {};
      extended_corners_tcs[0] = { extended_len_2, extended_wid_2 };   // Front right
      extended_corners_tcs[1] = { extended_len_2, -extended_wid_1 };  // Front left
      extended_corners_tcs[2] = { -extended_len_1, -extended_wid_1 }; // Rear left
      extended_corners_tcs[3] = { -extended_len_1, extended_wid_2 };  // Rear right

      for (uint32_t k = 0U; k < NUM_CORNERS_IN_RECTANGLE; k++)
      {
         Convert_TCS_Posn_To_VCS_Posn(
            extended_corners_tcs[k].x,
            extended_corners_tcs[k].y,
            m_object->bbox.Get_Center().x,
            m_object->bbox.Get_Center().y,
            m_object->bbox.Get_Orientation(),
            m_extended_corners[k].x,
            m_extended_corners[k].y);
      }
   }
     
   /*===========================================================================*\
   * FUNCTION: Find_Intersection()
   * ===========================================================================
   * RETURN VALUE:
   * std::pair<bool, Point> intersection_result - True if reflector is on given 
   *                                                      range and position of found point.
   *
   * PARAMETERS:
   * const Point& end_point_A1
   * const Point& end_point_A2
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
   * Calculate intersection points of reflector with segment described by segment_point_A
   * and segment_point_B. Returned is the closest found intersection point to end_point_A1.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   * 
   * NOTE:
   * It is limited by Determine_Segments_Intersection_Limited() functionality.
   *
   \*===========================================================================*/
   std::pair<bool, Point> Object_Reflector::Find_Intersection(
      const Point& end_point_A1,
      const Point& end_point_A2
   ) const
   {
      float32_t min_dst_squared = INFTY;    
      std::pair<bool, Point> intersection_result = { false, {} };

      for (uint32_t k = 0U; k < NUM_CORNERS_IN_RECTANGLE; k++)
      {    
         const uint32_t next_corner_idx = (k < (NUM_CORNERS_IN_RECTANGLE - 1U)) ? k + 1U : 0U;
         const float32_t A[2] = { end_point_A1.x, end_point_A1.y };
         const float32_t B[2] = { end_point_A2.x, end_point_A2.y };
         const float32_t C[2] = { m_extended_corners[k].x, m_extended_corners[k].y };        
         const float32_t D[2] = { m_extended_corners[next_corner_idx].x, m_extended_corners[next_corner_idx].y };

         Point intersection_point = {};
         const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, intersection_point.x, intersection_point.y);

         if (f_intersect)
         {
            const float32_t lat_diff = intersection_point.y - end_point_A1.y;
            const float32_t long_diff = intersection_point.x - end_point_A1.x;
            const float32_t intersection_to_A_dist_squared = (lat_diff * lat_diff) + (long_diff * long_diff);

            if (intersection_to_A_dist_squared < min_dst_squared)
            {
               min_dst_squared = intersection_to_A_dist_squared;
               intersection_result.first = true;
               intersection_result.second = intersection_point;
            }
         }        
      }

      return intersection_result;
   }

   /*===========================================================================*\
   * FUNCTION: Is_On_Radius()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_is_on_range - True if reflector is on given range.
   *
   * PARAMETERS:
   * const Point& view_point
   * const float32_t radius
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
   * Checks if reflector is on specified range, looking from view_point
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Object_Reflector::Is_On_Radius(
      const Point& view_point, 
      const float32_t radius) const
   {
      Point view_point_tcs(view_point);
      view_point_tcs.Transform_To_Relative_Coordinate_System(m_object->bbox.Get_Center(), m_object->bbox.Get_Orientation());

      const Circle circle_tcs = { view_point_tcs, radius };
      Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};

      // TODO: DFT-1317 Write Any_Intersection_In_TCS() in order to optimize code
      //it can be optimilized by breaking if first intersections point is found and not calculate rest. Here we need to know if we have any
      const bool f_is_on_range = Find_Intersections_in_TCS(circle_tcs, *m_object, intersections_tcs) > 0U ? true : false;

      return f_is_on_range;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Inside()
   * ===========================================================================
   * RETURN VALUE:
   * bool - True if a point is inside box
   *
   * PARAMETERS:
   * const Point& analyzed_point
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
   * A call functions that checks if a point is inside the bounding box of the specified object
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Object_Reflector::Is_Inside(const Point& analyzed_point) const
   {
      return Check_If_Vcs_Point_Is_Inside_Bounding_Box(analyzed_point.x, analyzed_point.y, *m_object);
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Range_Rate_Interval()
   * ===========================================================================
   * RETURN VALUE:
   * Range_Rate_Interval_T result - Range rate scope min/max or INFTY/INFTY
   *
   * PARAMETERS:
   * const Point& view_point
   * const float32_t radius
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
   * Calculate range rate scope. Scope is defined as min and max of projected range 
   * rates from two extreme points defining azimuth spread, looking from view_point
   * 
   * PRECONDITIONS:
   * radius > F360_EPSILON.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Range_Rate_Interval_T Object_Reflector::Compute_Range_Rate_Interval(
      const Point& view_point, 
      const float32_t radius) const
   {
      Range_Rate_Interval_T result = { };
      if (radius > F360_EPSILON)
      {
         Point view_point_tcs(view_point);
         view_point_tcs.Transform_To_Relative_Coordinate_System(m_object->bbox.Get_Center(), m_object->bbox.Get_Orientation());

         Point object_vcs_pos_in_tcs(m_object->vcs_position);
         object_vcs_pos_in_tcs.Transform_To_Relative_Coordinate_System(m_object->bbox.Get_Center(), m_object->bbox.Get_Orientation());


         const Circle searching_circle = { view_point_tcs, radius };
         Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};
         const uint32_t num_intersections = Find_Intersections_in_TCS(searching_circle, *m_object, intersections_tcs);

         if (num_intersections > 0U)
         {
            F360_VCS_Velocity_T velocity_tcs = {};
            F360_Rotate_2D_Vector(
               m_object->vcs_velocity.longitudinal, m_object->vcs_velocity.lateral,
               m_object->bbox.Get_Orientation().Cos(), -m_object->bbox.Get_Orientation().Sin(),
               velocity_tcs.longitudinal, velocity_tcs.lateral);

            const float32_t view_boresight = F360_Atan2f(m_object->bbox.Get_Center().y - view_point.y, m_object->bbox.Get_Center().x - view_point.x);
            const Extreme_Azimuth_Points spread_points_tcs = Determine_Points_With_Extreme_Azimuth(view_point_tcs, view_boresight, intersections_tcs, num_intersections);

            //Calculating velocities at the points of intersection on the object known the velocity at reference point
            F360_VCS_Velocity_T velocity_tcs_min_az_point = {};
            velocity_tcs_min_az_point.longitudinal = velocity_tcs.longitudinal - m_object->heading_rate * (spread_points_tcs.min_az_point.y - object_vcs_pos_in_tcs.y);
            velocity_tcs_min_az_point.lateral = velocity_tcs.lateral + m_object->heading_rate * (spread_points_tcs.min_az_point.x - object_vcs_pos_in_tcs.x);
            F360_VCS_Velocity_T velocity_tcs_max_az_point = {};
            velocity_tcs_max_az_point.longitudinal = velocity_tcs.longitudinal - m_object->heading_rate * (spread_points_tcs.max_az_point.y - object_vcs_pos_in_tcs.y);
            velocity_tcs_max_az_point.lateral = velocity_tcs.lateral + m_object->heading_rate * (spread_points_tcs.max_az_point.x - object_vcs_pos_in_tcs.x);

            const float32_t first_spread_range_rate = Calculate_Projected_Range_Rate(view_point_tcs, spread_points_tcs.min_az_point, velocity_tcs_min_az_point);
            const float32_t second_spread_range_rate = Calculate_Projected_Range_Rate(view_point_tcs, spread_points_tcs.max_az_point, velocity_tcs_max_az_point);
            result.rr_min = std::min(first_spread_range_rate, second_spread_range_rate);
            result.rr_max = std::max(first_spread_range_rate, second_spread_range_rate);
         }
         else
         {
            result = { INFTY, INFTY };
         }
      }
      else
      {
         result = { INFTY, INFTY };
      }
     
      return result;
   }
}
