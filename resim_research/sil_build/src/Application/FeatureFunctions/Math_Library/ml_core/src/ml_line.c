/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <assert.h>
#include "ml_line.h"
#include "ml_line_parameter.h"
#include "ml_macros.h"
#include "ml_math.h"


float32_T Get_Slope_Of_Line(
   const Vector_2d_T * const p_point0,
   const Vector_2d_T * const p_point1)
{
   float32_T delta_x;
   float32_T delta_y;

   assert(p_point0 != NULL);
   assert(p_point1 != NULL);

   delta_x = p_point0->x - p_point1->x;
   delta_y = p_point0->y - p_point1->y;

   assert(delta_x != 0.0);

   return (delta_y / delta_x);
}

float32_T Get_Y_Value_From_Line_Defined_By_2_Points(
   const Vector_2d_T* p_point0,
   const Vector_2d_T* p_point1,
   const float32_T x)
{
   float32_T slope;
   float32_T x2;
   float32_T y2;
   float32_T  y_ret;

   assert(p_point0 != NULL);
   assert(p_point1 != NULL);

   slope = Get_Slope_Of_Line(p_point0, p_point1);
   x2 = p_point1->x;
   y2 = p_point1->y;

   y_ret = y2 - (slope*(x2 - x));

   return y_ret;
}

float32_T Get_Y_Value_From_Line_By_Coordinates(
   const float32_T x1,
   const float32_T y1,
   const float32_T x2,
   const float32_T y2,
   const float32_T x)
{
   float32_T y_ret;

   assert(x2 != x1);

   if (Abs(x2 - x1) <= EPSILON)
   {
      y_ret = y1;
   }
   else
   {
      y_ret = y1 + ((x - x1) * ((y2 - y1) / (x2 - x1)));
   }

   return y_ret;
}
