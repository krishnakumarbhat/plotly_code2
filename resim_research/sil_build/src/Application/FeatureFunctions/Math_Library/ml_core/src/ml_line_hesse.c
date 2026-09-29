/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include <assert.h>
#include "ml_bool.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_line_hesse.h"

Line_Hesse_T Line_Hesse_Create(
   const float        distance_to_origin,
   const Vector_2d_T *p_norm_vector
   )
{
   Line_Hesse_T line;

   assert(NULL != p_norm_vector);

   line.distance_to_origin = distance_to_origin;
   line.norm_vector = *p_norm_vector;

   return line;
}

Line_Hesse_T Line_Hesse_Create_Using_Point_And_Unit_Vector(
   const Vector_2d_T *p_point,
   const Vector_2d_T *p_unit_vector
   )
{
   Line_Hesse_T line;

   assert(NULL != p_point);
   assert(NULL != p_unit_vector);

   /* Get the normal vector by rotating the unit vector by 90 degrees.
    * Here the unit vector is rotated 90 degrees clockwise */
   line.norm_vector.x = -p_unit_vector->y;
   line.norm_vector.y = p_unit_vector->x;
   /* Distance will be positive if the norm vector points away from origin */
   line.distance_to_origin = ((line.norm_vector.x * p_point->x) + (line.norm_vector.y * p_point->y));

   return line;
}


Line_Hesse_T Line_Hesse_Create_Using_Point_And_Normal_Vector(
   const Vector_2d_T *p_point,
   const Vector_2d_T *p_normal_vector
   )
{
   Line_Hesse_T line;

   assert(NULL != p_point);
   assert(NULL != p_normal_vector);

   line.norm_vector.x = p_normal_vector->x;
   line.norm_vector.y = p_normal_vector->y;
   /* Distance will be positive if the norm vector points away from origin */
   line.distance_to_origin = ((line.norm_vector.x * p_point->x) + (line.norm_vector.y * p_point->y));

   return line;
}


Line_Hesse_T Line_Hesse_Create_Using_Two_Points(
   const Vector_2d_T *p_point1,
   const Vector_2d_T *p_point2
   )
{
   Line_Hesse_T line;
   Vector_2d_T  vector;
   float        vector_length;
   Vector_2d_T  unit_vector;

   assert(NULL != p_point1);
   assert(NULL != p_point2);

   /* Get the vector from p_point1 to p_point2 */
   vector = Vector_2d_Alg_Diff(p_point2, p_point1);
   /* Calculate the length of the vector */
   vector_length = Vector_2d_Alg_Abs(&vector);

   /* Get the unit vector */
   if (vector_length > THRESHOLD_IS_ZERO)
   {
      unit_vector = Vector_2d_Alg_Normalize_Vector(&vector);
   }
   else
   {
      unit_vector = Create_2d_Vector_X_Normal();
      assert(FALSE);
   }

   line = Line_Hesse_Create_Using_Point_And_Unit_Vector(p_point1, &unit_vector);

   return line;
}


Line_Hesse_T Line_Hesse_Create_Using_Line_Segment(const Line_Segment_T *p_segment)
{
   assert(NULL != p_segment);

   return Line_Hesse_Create_Using_Two_Points(&p_segment->p0, &p_segment->p1);
}


Line_Hesse_T Line_Hesse_Create_Using_Line_Parameter(const Line_Parameter_T *p_line_param)
{
   assert(NULL != p_line_param);

   return Line_Hesse_Create_Using_Point_And_Unit_Vector(&p_line_param->p0, &p_line_param->direction);
}


float Line_Hesse_Get_Distance_Of_Point(
   const Vector_2d_T  *p_point,
   const Line_Hesse_T *p_line
   )
{
   float distance;

   assert(NULL != p_point);
   assert(NULL != p_line);

   distance = ((p_line->norm_vector.x * p_point->x) + (p_line->norm_vector.y * p_point->y)) - p_line->distance_to_origin;

   return distance;
}


Side_Of_Line_Hesse_T Line_Hesse_Get_Side_of_Point(
   const Line_Hesse_T *p_line,
   const Vector_2d_T  *p_point,
   const float margin
   )
{
   float                distance;
   Side_Of_Line_Hesse_T result;

   assert(NULL != p_point);
   assert(NULL != p_line);
   assert(margin >= 0.0f);

   distance = Line_Hesse_Get_Distance_Of_Point(p_point, p_line);
   if (distance < -margin)
   {
      result = LINE_HESSE_SIDE_NEGATIVE;
   }
   else if (distance > margin)
   {
      result = LINE_HESSE_SIDE_POSITIVE;
   }
   else
   {
      result = LINE_HESSE_SIDE_ON_LINE;
   }
   return result;
}


Line_Hesse_T Line_Hesse_Update_Line_Normal_Direction(
   const Vector_2d_T         *p_point,
   const Line_Hesse_T        *p_line,
   const Side_Of_Line_Hesse_T side_of_line
   )
{
   Line_Hesse_T updated_line;
   float        result;

   assert(NULL != p_point);
   assert(NULL != p_line);
   assert(side_of_line != LINE_HESSE_SIDE_ON_LINE);

   /* Find on which side of the three line does the point lie */
   result = Line_Hesse_Get_Distance_Of_Point(p_point, p_line);

   /* If the point doesn't lie on the same side as normal vector then update the signs of normal vector and distance from origin */
   if (((result < 0) && (LINE_HESSE_SIDE_POSITIVE == side_of_line)) || ((result > 0) && (LINE_HESSE_SIDE_NEGATIVE == side_of_line)))
   {
      updated_line.norm_vector.x      = -p_line->norm_vector.x;
      updated_line.norm_vector.y      = -p_line->norm_vector.y;
      updated_line.distance_to_origin = -p_line->distance_to_origin;
   }
   else
   {
      updated_line.norm_vector.x      = p_line->norm_vector.x;
      updated_line.norm_vector.y      = p_line->norm_vector.y;
      updated_line.distance_to_origin = p_line->distance_to_origin;
   }

   return updated_line;
}


Line_Hesse_T Line_Hesse_Create_Using_Azimuth_And_Point(
   const Vector_2d_T *p_point_on_line,
   const float        azimuth_of_line
   )
{
   Line_Hesse_T line;
   Vector_2d_T  unit_vector;

   assert(NULL != p_point_on_line);

   /* Get unit vector */
   unit_vector.x = Fast_Cos(azimuth_of_line);
   unit_vector.y = Fast_Sin(azimuth_of_line);

   /* Create a line using a point on the line and a unit vector parallel/along the line */
   line = Line_Hesse_Create_Using_Point_And_Unit_Vector(p_point_on_line, &unit_vector);

   return line;
}

Vector_2d_T Line_Hesse_Get_Norm_Vector(const Line_Hesse_T *p_line)
{
   assert(NULL != p_line);
   return p_line->norm_vector;
}

Vector_2d_T Line_Hesse_Get_Unit_Vector(const Line_Hesse_T *p_line)
{
   Vector_2d_T unit;
   assert(NULL != p_line);

   unit.x = p_line->norm_vector.y;
   unit.y = -p_line->norm_vector.x;
   return unit;
}

float Line_Hesse_Get_Distance_To_Origin(const Line_Hesse_T *p_line /**< Line to get the norm vector from */
   )
{
   assert(NULL != p_line);
   return p_line->distance_to_origin;
}
