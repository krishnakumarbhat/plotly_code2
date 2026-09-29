/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "Geometric_2d_Structs.h"
#include "Geometric_2d_Factory.h"
#include "Geometric_2d_Functions.h"
#include "line_hesse.h"

#include "st_math.h"
#include "st_line_hesse.h"

#include <assert.h>


Line_Normal_T Create_Line_Normal(
   const Vector_2d_T *p_p0,
   const Vector_2d_T *p_p1)
{
   Line_Normal_T ret_value;

   assert(p_p0 != NULL);
   assert(p_p1 != NULL);

   ret_value.p0 = *p_p0;

   ret_value.n.x = -p_p0->y + p_p1->y;
   ret_value.n.y = p_p0->x - p_p1->x;

   return ret_value;
}

Line_Side_T Get_Side_of_Point(
   const Line_Normal_T *p_line,
   const Vector_2d_T   *p_point)
{
   Line_Side_T ret_value;

   assert(p_line != NULL);
   assert(p_point != NULL);

   ret_value = (Line_Side_T)Sign(((p_line->p0.x - p_point->x) * p_line->n.x) + ((p_line->p0.y - p_point->y) * p_line->n.y));

   return ret_value;
}

Line_Hesse_T Line_Hesse_Create_Using_Line_Normal(const Line_Normal_T *p_line_normal)
{
   assert(NULL != p_line_normal);

   return Line_Hesse_Create_Using_Point_And_Normal_Vector(&p_line_normal->p0, &p_line_normal->n);
}
