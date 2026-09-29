/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_vector_2d_assertion_helper.h"
#include <assert.h>
#include "ml_macros.h"
#include "ml_line_parameter.h"


Line_Parameter_T Create_Line_Parameter_Form(
   const Vector_2d_T * const p_point_a,
   const Vector_2d_T * const p_point_b)
{
   Line_Parameter_T ret_value;

   assert(NULL != p_point_a);
   assert(NULL != p_point_b);

   assert(Is_Not_Equal(p_point_a->x, p_point_b->x) || Is_Not_Equal(p_point_a->y, p_point_b->y));

   ret_value.p0 = *(p_point_a);

   ret_value.direction.x = p_point_a->x - p_point_b->x;
   ret_value.direction.y = p_point_a->y - p_point_b->y;

   return ret_value;
}

float32_T Get_Y_Value_From_Line(
   const Line_Parameter_T *const p_line,
   const float32_T         x)
{
   float32_T ret_value;

   assert(NULL != p_line);
   assert(p_line->direction.x != 0);

   ret_value = p_line->p0.y + ((x - p_line->p0.x) * (p_line->direction.y / p_line->direction.x));

   return ret_value;
}
