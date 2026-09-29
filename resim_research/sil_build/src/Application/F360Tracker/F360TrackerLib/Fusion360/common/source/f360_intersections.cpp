/*===================================================================================*\
* FILE: f360_intersections.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Interscetions functionalty.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_intersections.h"
#include "f360_circle.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Intersections_in_TCS()
   *===========================================================================
   * RETURN VALUE:
   * uint32_t valid_points_cnt - Number of found intersections.
   *
   * PARAMETERS:
   *  const Circle &circle_tcs                                          - Circle in TCS
   *  const F360_Object_Track_T &object                                 - Object structure
   *  Point(&intersections_tcs)[Max_Num_Intersections_Circle_Rectangle] - Indirect output
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
   * Looks for intersection point between object and circle.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   uint32_t Find_Intersections_in_TCS(
      const Circle &circle_tcs,  
      const F360_Object_Track_T &object,
      Point(&intersections_tcs)[Max_Num_Intersections_Circle_Rectangle]
   )
   {
      const uint8_t Max_Num_Intersections_Circle_Line = 2U;

      /*
      Solves equations:
       Case 1: (x-a)^2 + (y-b)^2 = r^2 and y = c 
       Case 2: (x-a)^2 + (y-b)^2 = r^2 and x = d
       where a,b,c,d,r are known
      */
      const float32_t half_length = 0.5F * object.bbox.Get_Length();
      const float32_t half_width = 0.5F * object.bbox.Get_Width();

      uint32_t valid_points_cnt = 0U;

      // Front and rear segments (case 1)
      const float32_t x_array[Max_Num_Intersections_Circle_Line] = { half_length, -half_length };
      for (uint32_t i = 0U; i < Max_Num_Intersections_Circle_Line; i++)
      {
         const float32_t &x = x_array[i];

         float32_t found_values[Max_Num_Intersections_Circle_Line];
         const uint32_t found_values_cnt = circle_tcs.Y_Intersects_At_X(x, found_values);

         for (uint32_t k = 0U; k < found_values_cnt; k++)
         {
            const float32_t &y = found_values[k];
            if ((-half_width <= y) && (y <= half_width))
            {
               intersections_tcs[valid_points_cnt] = { x, y };
               valid_points_cnt++;
            }
         }
      }

      // Left and right segments (case 2)
      const float32_t y_array[Max_Num_Intersections_Circle_Line] = { half_width, -half_width };
      for (uint32_t i = 0U; i < Max_Num_Intersections_Circle_Line; i++)
      {
         const float32_t &y = y_array[i];
         float32_t found_values[Max_Num_Intersections_Circle_Line];
         const uint32_t found_values_cnt = circle_tcs.X_Intersects_At_Y(y, found_values);

         for (uint32_t k = 0U; k < found_values_cnt; k++)
         {
            const float32_t &x = found_values[k];
            if ((-half_length <= x) && (x <= half_length))
            {
               intersections_tcs[valid_points_cnt] = { x, y };
               valid_points_cnt++;
            }
         }
      }
      return valid_points_cnt;
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Segments_Intersection_Limited()
   *===========================================================================
   * RETURN VALUE:
   * bool f_intersect - True if intersections is found
   *
   * PARAMETERS:
   * const float32_t (&end_point_A1)[2]
   * const float32_t (&end_point_A2)[2]
   * const float32_t (&end_point_B1)[2]
   * const float32_t (&end_point_B2)[2]
   * float32_t & result_x,
   * float32_t & result_y
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
   * Finds intersection point between two segments A (A1A2) and B (B1B2).
   * In case of 
   * - Full or partial overlapping 
   * - At least one of the segment is reduced to a point
   * - Point C is on AB segment (C, AB are examples, other configurations are also handled this way)
   * 
   * ==> False is returned.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Determine_Segments_Intersection_Limited(
      const float32_t(&end_point_A1)[2],
      const float32_t(&end_point_A2)[2],
      const float32_t(&end_point_B1)[2],
      const float32_t(&end_point_B2)[2],
      float32_t & result_x,
      float32_t & result_y)
   {
      //TODO: DFT-1315: Add UT or refactor Determine_Segments_Intersection_Limited()

      /* Given 4 (2D) points, determine if the two line segments formed by them
         intersect and if so, at what point. 
      */
      bool f_intersect = false;

      /*
       To simply rule out lines that don't intersect, check the sign of the
       cross product between segments A and A2B1 and then A and A2B2
       If B does indeed cross A, the aforementioned cross products
       (corresponding to clockwise and counterclockwise directions) will have
       different signs.  If the signs are the same or zero, then you can return a
       false intersection value.
       Here we assume the line segments are formed by A and B
      */
      const float32_t diff_A2_A1[2] = { end_point_A2[0] - end_point_A1[0] , end_point_A2[1] - end_point_A1[1] };
      const float32_t diff_B1_A2[2] = { end_point_B1[0] - end_point_A2[0] , end_point_B1[1] - end_point_A2[1] };
      const float32_t diff_B2_A2[2] = { end_point_B2[0] - end_point_A2[0], end_point_B2[1] - end_point_A2[1] };

      const float32_t cross_prod1 = diff_A2_A1[0] * diff_B1_A2[1] - diff_A2_A1[1] * diff_B1_A2[0];
      const float32_t cross_prod2 = diff_A2_A1[0] * diff_B2_A2[1] - diff_A2_A1[1] * diff_B2_A2[0];

      if ((cross_prod1 * cross_prod2) < 0.0F)
      {
         const float32_t diff_B2_B1[2] = { end_point_B2[0] - end_point_B1[0], end_point_B2[1] - end_point_B1[1] };
         const float32_t cross_prod3 = diff_A2_A1[0] * diff_B2_B1[1] - diff_A2_A1[1] * diff_B2_B1[0];

         const float32_t cross_prod_th = 1.0e-4F;
         if (std::abs(cross_prod3) > cross_prod_th)
         {
            const float32_t diff_B1_A1[2] = { end_point_B1[0] - end_point_A1[0], end_point_B1[1] - end_point_A1[1] };
            const float32_t cross_prod4 = diff_B1_A1[0] * diff_B2_B1[1] - diff_B1_A1[1] * diff_B2_B1[0];
            const float32_t t = cross_prod4 / cross_prod3;

            const float32_t cross_prod5 = diff_B1_A1[0] * diff_A2_A1[1] - diff_B1_A1[1] * diff_A2_A1[0];
            const float32_t u = cross_prod5 / cross_prod3;

            const bool t_and_u_in_valid_interval = ((0.0F <= t) && (t <= 1.0F) && (0.0F <= u) && (u <= 1.0F));
            if (t_and_u_in_valid_interval)
            {
               f_intersect = true;
               result_x = end_point_A1[0] + t * diff_A2_A1[0];
               result_y = end_point_A1[1] + t * diff_A2_A1[1];
            }
         }
      }

      return f_intersect;
   }
}
