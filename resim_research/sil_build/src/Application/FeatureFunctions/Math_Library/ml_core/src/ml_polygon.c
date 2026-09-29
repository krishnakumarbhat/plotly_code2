/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include "ml_vector_2d_assertion_helper.h"
#include <assert.h>
#include "ml_bool.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_polygon.h"
#include "ml_line_hesse.h"

/**
 * \param periodicity size of the array
 * \param index into the array
 * A periodic array in this context means that the data represented in the array is circling.
 * An index of array-size flips to index zero. The index given to this macro therefore is
 * allowed to be bigger than periodicity.
 * This macro returns an index in between 0 and periodicity always.
 * \ingroup polygon
 */
#define Get_Index_Of_Periodic_Array(index,periodicity) ((index)%(periodicity)) /* PRQA S 3453 */ /* locally used macro to index an array */

boolean_T Is_Point_In_Polygon(
   const Vector_2d_T *p_polygon,
   const uint8_t      num_polygon_corners,
   const Vector_2d_T *p_point)
{
   boolean_T     is_point_in_polygon_ret;
   Side_Of_Line_Hesse_T side_of_line;
   Line_Hesse_T line;
   uint8_t       index;

   assert(p_polygon != NULL);
   assert(p_point != NULL);
   assert(num_polygon_corners > 0);

   is_point_in_polygon_ret = TRUE;
   side_of_line = LINE_HESSE_SIDE_ON_LINE;

   for (index = 0; index < num_polygon_corners; index++)
   {
      Side_Of_Line_Hesse_T side_of_line_tmp;

      line = Line_Hesse_Create_Using_Two_Points(&(p_polygon[Get_Index_Of_Periodic_Array(index, num_polygon_corners)]),
         &(p_polygon[Get_Index_Of_Periodic_Array(index + 1, num_polygon_corners)]));
      side_of_line_tmp = Line_Hesse_Get_Side_of_Point(&line, p_point, 0.f);
      if (side_of_line == LINE_HESSE_SIDE_ON_LINE)
      {
         /* side_of_line needs to be initialized*/
         side_of_line = side_of_line_tmp;
      }
      else
      {
         if (Is_False(((side_of_line_tmp == LINE_HESSE_SIDE_ON_LINE) || (side_of_line_tmp == side_of_line))))
         {
            is_point_in_polygon_ret = FALSE;
            break;
         }
      }
   }

   return is_point_in_polygon_ret;
}

boolean_T Is_Point_In_Convex_Polygon_Ray_Casting_Method(
   const Vector_2d_T *p_polygon,
   const uint8_t      num_polygon_corners,
   const Vector_2d_T *p_point)
{
   boolean_T result = FALSE;
   uint8_t i;
   uint8_t j;

   assert(p_polygon != NULL);
   assert(p_point != NULL);
   assert(num_polygon_corners > 0);

   j = num_polygon_corners - 1;
   for (i = 0; i < num_polygon_corners; i++)
   {
      if (Abs(p_polygon[j].y - p_polygon[i].y) > THRESHOLD_IS_ZERO)
      {
         if ((p_polygon[i].y >= p_point->y) != (p_polygon[j].y >= p_point->y))
         {
            float32_T test_value = (((p_polygon[j].x - p_polygon[i].x) * (p_point->y - p_polygon[i].y)) / (p_polygon[j].y - p_polygon[i].y)) + p_polygon[i].x;
            if (p_point->x <= test_value)
            {
               result = !Is_True(result);
            }
         }
      }
      j = i;
   }
   return Is_True(result);
}
