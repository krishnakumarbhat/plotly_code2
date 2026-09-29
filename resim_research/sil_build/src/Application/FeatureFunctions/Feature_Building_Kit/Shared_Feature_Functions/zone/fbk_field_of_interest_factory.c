/**
 * @file fbk_field_of_interest_factory.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file with functions to work with field of interest.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "fbk_ref_point.h"
#include "ml_float_range_t.h"
#include "ml_interval.h"
#include "ml_math.h"
#include "ml_math_infinity.h" // IWYU pragma: keep
#include "ml_polygon.h"
#include "ml_saturated_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "pa_reuse.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
static int32_t Fbk_Compare_Points_Angle_From_Origin(const void *p_point_a, const void *p_point_b)
{
   int32_t res_val;
   const float32_T *angle_a;
   const float32_T *angle_b;

   assert(NULL != p_point_a);
   assert(NULL != p_point_b);

   /* coverity[misra_c_2012_rule_11_5_violation][usage of void pointer is required for qsort comparison function]*/
   angle_a = (const float32_T *) p_point_a;
   /* coverity[misra_c_2012_rule_11_5_violation][usage of void pointer is required for qsort comparison function]*/
   angle_b = (const float32_T *) p_point_b;

   if (angle_a[1] > angle_b[1])
   {
      res_val = -FBK_ONE_INT;
   }
   else if (angle_a[1] < angle_b[1])
   {
      res_val = FBK_ONE_INT;
   }
   else
   {
      res_val = FBK_ZERO_INT;
   }

   return res_val;
}

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Create_Field_Of_Interest(Fbk_Field_Of_Interest_T *p_res_foi,
                                  const float32_T *p_x,
                                  const float32_T *p_y,
                                  const uint8_t field_of_interest_size)
{
   uint8_t idx;

   /* Check for valid input */
   assert(NULL != p_res_foi);
   assert(NULL != p_x);
   assert(NULL != p_y);
   assert(FBK_MAX_SIZE_OF_FOI >= field_of_interest_size);

   /* Set points according to input */
   for (idx = FBK_ZERO_UINT; idx < field_of_interest_size; idx++)
   {
      p_res_foi->points[idx] = Create_2d_Vector_Coordinates(p_x[idx], p_y[idx]);
   }

   /* Set size according to input */
   p_res_foi->size = field_of_interest_size;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Reset_Field_Of_Interest(Fbk_Field_Of_Interest_T *p_res_foi)
{
   uint8_t idx;

   /* Check for valid input */
   assert(NULL != p_res_foi);

   /* Set points according to input */
   for (idx = FBK_ZERO_UINT; idx < (uint8_t) FBK_MAX_SIZE_OF_FOI; idx++)
   {
      p_res_foi->points[idx].x = FBK_ZERO_F;
      p_res_foi->points[idx].y = FBK_ZERO_F;
   }

   p_res_foi->size = FBK_ZERO_UINT;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
float32_T Fbk_Get_Area_Field_Of_Interest(const Fbk_Field_Of_Interest_T *p_field_of_interest)
{
   /* Return value */
   float32_T foi_area = FBK_ZERO_F;

   /* Initialize variables */
   uint8_t idx;
   uint8_t prev_idx;

   /* Assert */
   assert(NULL != p_field_of_interest);

   /* Initialize prev_idx */
   prev_idx = (uint8_t) (p_field_of_interest->size - FBK_ONE_UINT);

   /* Shoelace formula */
   for (idx = FBK_ZERO_UINT; idx < p_field_of_interest->size; idx++)
   {
      /* Accumulate area of triangles using cross product */
      foi_area += (p_field_of_interest->points[prev_idx].x + p_field_of_interest->points[idx].x)
                  * (p_field_of_interest->points[prev_idx].y - p_field_of_interest->points[idx].y);

      /* Set previous idx */
      prev_idx = idx;
   }

   /* Calculate area */
   foi_area = Fbk_Half(Fbk_Abs_F(foi_area));

   return foi_area;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
float32_T Fbk_Get_Area_Bounding_Box(const Fbk_Bounding_Box_T *bbox)
{
   float32_T bbox_length;
   float32_T bbox_width;
   float32_T bbox_area;
   bbox_length = (bbox->x.max) - (bbox->x.min);
   bbox_width  = (bbox->y.max - bbox->y.min);

   if (Fbk_Is_True(bbox_length >= FBK_ZERO_F) && Fbk_Is_True(bbox_width >= FBK_ZERO_F))
   {
      bbox_area = bbox_length * bbox_width;
   }
   else
   {
      bbox_area = FBK_ZERO_F;
   }
   return bbox_area;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Create_Field_Of_Interest_From_Object_Data(Fbk_Field_Of_Interest_T *p_object_foi,
                                                   const Vector_2d_T obj_center_pos,
                                                   const float32_T obj_length,
                                                   const float32_T obj_width,
                                                   const float32_T obj_heading)
{
   Vector_2d_T point_offset[4u];
   uint8_t i_point;

   /* Calculates the half length and width of an object only once */
   float32_T half_length = Fbk_Half(obj_length);
   float32_T half_width  = Fbk_Half(obj_width);

   /* Calculate sine and cosine of heading_predicted only once */
   float32_T cos_heading_predicted = Fast_Cos(obj_heading);
   float32_T sin_heading_predicted = Fast_Sin(obj_heading);

   /* Set size for rectangle */
   p_object_foi->size = FBK_FOI_SIZE_TETRAGON;

   /* set up object zone */
   /*  x = long, y = lat */
   /*    0 ----- 1       */
   /*    |       |       */
   /*    |  pos  |       */
   /*    |       |       */
   /*    3 ----- 2       */

   point_offset[0] = Create_2d_Vector_Coordinates(half_length, -half_width);
   point_offset[1] = Create_2d_Vector_Coordinates(half_length, half_width);
   point_offset[2] = Create_2d_Vector_Coordinates((-half_length), half_width);
   point_offset[3] = Create_2d_Vector_Coordinates((-half_length), -half_width);

   /* Set corner points of object FoI */
   for (i_point = FBK_ZERO_UINT; i_point < p_object_foi->size; i_point++)
   {
      p_object_foi->points[i_point].x =
         obj_center_pos.x + (point_offset[i_point].x * cos_heading_predicted) - (point_offset[i_point].y * sin_heading_predicted);
      p_object_foi->points[i_point].y =
         obj_center_pos.y + (point_offset[i_point].y * cos_heading_predicted) + (point_offset[i_point].x * sin_heading_predicted);
   }
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
Fbk_Bounding_Box_T Fbk_Get_Field_Of_Interest_Bounding_Box(const Fbk_Field_Of_Interest_T *p_foi)
{
   /* Return value */
   Fbk_Bounding_Box_T bbox_foi;

   /* Iterator */
   uint8_t i;

   /* Asserts */
   assert(NULL != p_foi);

   /* Initialize values to first FoI point */
   bbox_foi.x.min = p_foi->points[0].x;
   bbox_foi.x.max = p_foi->points[0].x;
   bbox_foi.y.min = p_foi->points[0].y;
   bbox_foi.y.max = p_foi->points[0].y;

   for (i = FBK_ONE_UINT; i < p_foi->size; i++)
   {
      /* Get min and max value for x coordinates */
      bbox_foi.x.min = Fbk_Min(bbox_foi.x.min, p_foi->points[i].x);
      bbox_foi.x.max = Fbk_Max(bbox_foi.x.max, p_foi->points[i].x);

      /* Get min and max value for y coordinates */
      bbox_foi.y.min = Fbk_Min(bbox_foi.y.min, p_foi->points[i].y);
      bbox_foi.y.max = Fbk_Max(bbox_foi.y.max, p_foi->points[i].y);
   }

   return bbox_foi;
}


/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Are_Bounding_Boxes_Overlapping(const Fbk_Bounding_Box_T *p_bbox_a, const Fbk_Bounding_Box_T *p_bbox_b)
{
   /* Return value */
   boolean_T f_bbox_overlap = FBK_FALSE;
   boolean_T f_overlapp_x;
   boolean_T f_overlapp_y;

   /* Asserts */
   assert(NULL != p_bbox_a);
   assert(NULL != p_bbox_b);

   /* Check if bounding boxes are overlapping */
   f_overlapp_x = Does_Float_Range_Overlap_Float_Range(&p_bbox_a->x, &p_bbox_b->x);
   f_overlapp_y = Does_Float_Range_Overlap_Float_Range(&p_bbox_a->y, &p_bbox_b->y);
   if (f_overlapp_y && f_overlapp_x)
   {
      f_bbox_overlap = FBK_TRUE;
   }

   return f_bbox_overlap;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Are_Fields_Of_Interest_Overlapping(const Fbk_Field_Of_Interest_T *p_foi_a /**< First field of interest*/,
                                                 const Fbk_Field_Of_Interest_T *p_foi_b /**< Second field of interest*/)
{
   /* Return value */
   boolean_T f_overlap = FBK_FALSE;

   /* Iterator */
   uint8_t i;

   Fbk_Bounding_Box_T bbox_foi_a;
   Fbk_Bounding_Box_T bbox_foi_b;

   /* Asserts */
   assert(NULL != p_foi_a);
   assert(NULL != p_foi_b);

   /* Get bounding boxes for both fields of interest. */
   bbox_foi_a = Fbk_Get_Field_Of_Interest_Bounding_Box(p_foi_a);
   bbox_foi_b = Fbk_Get_Field_Of_Interest_Bounding_Box(p_foi_b);

   /* Check that bounding boxes are overlapping as a preliminary condition. */
   if (Fbk_Is_True(Fbk_Are_Bounding_Boxes_Overlapping(&bbox_foi_a, &bbox_foi_b)))
   {
      Vector_2d_T bbox_center_point;

      /* Get center point in case of rectangular FoI and check against other FoI. */
      if (FBK_FOI_SIZE_TETRAGON == p_foi_a->size)
      {
         /* Check if center point of A is located in FoI B. */

         bbox_center_point.x = bbox_foi_a.x.min + Fbk_Half(bbox_foi_a.x.max - bbox_foi_a.x.min);
         bbox_center_point.y = bbox_foi_a.y.min + Fbk_Half(bbox_foi_a.y.max - bbox_foi_a.y.min);

         f_overlap = Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_foi_b->points, p_foi_b->size, &bbox_center_point);
      }

      if (Fbk_Is_False(f_overlap) && (FBK_FOI_SIZE_TETRAGON == p_foi_b->size))
      {
         /* Check if center point of B is located in FoI A. */
         bbox_center_point.x = bbox_foi_b.x.min + Fbk_Half(bbox_foi_b.x.max - bbox_foi_b.x.min);
         bbox_center_point.y = bbox_foi_b.y.min + Fbk_Half(bbox_foi_b.y.max - bbox_foi_b.y.min);

         f_overlap = Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_foi_a->points, p_foi_a->size, &bbox_center_point);
      }


      /* Next check corner points. */
      if (Fbk_Is_False(f_overlap))
      {
         /* Check corner points of A against FoI B. */
         for (i = FBK_ZERO_UINT; i < p_foi_a->size; i++)
         {
            if (Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_foi_b->points, p_foi_b->size, &p_foi_a->points[i]))
            {
               f_overlap = FBK_TRUE;
               break;
            }
         }
         /* Check corner points of B against FoI A. */
         for (i = FBK_ZERO_UINT; i < p_foi_b->size; i++)
         {
            if (Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_foi_a->points, p_foi_a->size, &p_foi_b->points[i]))
            {
               f_overlap = FBK_TRUE;
               break;
            }
         }
      }

      if (Fbk_Is_False(f_overlap))
      {
         /* Check if foi A or B is a subset of another one in longitudinal direction */
         boolean_T foia_in_foib_long = Is_Float_Interval_Subset_Of_Float_Interval(&bbox_foi_a.x, &bbox_foi_b.x);
         boolean_T foib_in_foia_long = Is_Float_Interval_Subset_Of_Float_Interval(&bbox_foi_b.x, &bbox_foi_a.x);

         /* Check if foi A and B overlap in lateral direction */
         boolean_T fois_ab_overlap_lat = Does_Float_Range_Overlap_Float_Range(&bbox_foi_a.y, &bbox_foi_b.y);

         float32_T foia_ref_lat = bbox_foi_a.y.min + Fbk_Half(bbox_foi_a.y.max - bbox_foi_a.y.min);
         float32_T foib_ref_lat = bbox_foi_b.y.min + Fbk_Half(bbox_foi_b.y.max - bbox_foi_b.y.min);

         /* Check if center point of A or B is in range of another one in lateral direction*/
         boolean_T ref_lat_foia_in_foib = Is_Float_Contained_In_Float_Range(foia_ref_lat, &bbox_foi_b.y);
         boolean_T ref_lat_foib_in_foia = Is_Float_Contained_In_Float_Range(foib_ref_lat, &bbox_foi_a.y);

         /* Combine checks for long objects that are not covered by above methods*/
         f_overlap = (boolean_T) (((foia_in_foib_long && ref_lat_foia_in_foib) || (foib_in_foia_long && ref_lat_foib_in_foia))
                                  && fois_ab_overlap_lat);
      }
   }

   return f_overlap;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
float32_T Fbk_Get_Bounding_Boxes_Overlapped_Area(const Fbk_Bounding_Box_T *bbox_a, const Fbk_Bounding_Box_T *bbox_b)
{

   Fbk_Bounding_Box_T overlapped_bbox;
   float32_T overlapped_width;
   float32_T overlapped_length;
   float32_T overlapped_area;
   overlapped_area = FBK_ZERO_F;

   if (Fbk_Are_Bounding_Boxes_Overlapping(bbox_a, bbox_b))
   {
      overlapped_bbox.x.min = Fbk_Max(bbox_a->x.min, bbox_b->x.min);
      overlapped_bbox.x.max = Fbk_Min(bbox_a->x.max, bbox_b->x.max);
      overlapped_bbox.y.min = Fbk_Max(bbox_a->y.min, bbox_b->y.min);
      overlapped_bbox.y.max = Fbk_Min(bbox_a->y.max, bbox_b->y.max);

      if (Fbk_Is_True(overlapped_bbox.x.max > overlapped_bbox.x.min) && Fbk_Is_True(overlapped_bbox.y.max > overlapped_bbox.y.min))
      {
         overlapped_length = overlapped_bbox.x.max - overlapped_bbox.x.min;
         overlapped_width  = overlapped_bbox.y.max - overlapped_bbox.y.min;
         overlapped_area   = overlapped_length * overlapped_width;
      }
   }

   return overlapped_area;
}


/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(const Vector_2d_T *p_foi_point,
                                                                const Vector_2d_T *p_vector_origin,
                                                                const Vector_2d_T *p_vector_dir)
{
   /* Return value */
   boolean_T f_point_in_vector_dir = FBK_FALSE;

   /* Move FoI point to vector origin. */
   const Vector_2d_T foi_point_orig = Vector_2d_Alg_Diff(p_foi_point, p_vector_origin);

   /* Check whether the given point lies in the general direction of the given vector. */

   if (Vector_2d_Alg_Scalar_Product(&foi_point_orig, p_vector_dir) >= FBK_ZERO_F)
   {
      f_point_in_vector_dir = FBK_TRUE;
   }

   return f_point_in_vector_dir;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Are_Two_Lines_Intersecting(Vector_2d_T *p_intersection_point,
                                         const Vector_2d_T *p_line1_start,
                                         const Vector_2d_T *p_line1_end,
                                         const Vector_2d_T *p_line2_start,
                                         const Vector_2d_T *p_line2_end)
{
   /* Return value */
   boolean_T f_lines_intersecting = FBK_FALSE;

   if (NULL != p_intersection_point)
   {
      float32_T m1 = INFINITY, m2 = INFINITY;
      float32_T c1 = FBK_ZERO_F, c2 = FBK_ZERO_F;
      float32_T dx, dy;

      dx = p_line1_end->x - p_line1_start->x;
      dy = p_line1_end->y - p_line1_start->y;
      if (Fbk_Abs_F(dx) > THRESHOLD_IS_ZERO)
      {
         /* Get slope of line 1. */
         m1 = dy / dx;

         /* Get intercept of line 1. */
         c1 = p_line1_start->y - (m1 * p_line1_start->x);
      }

      dx = p_line2_end->x - p_line2_start->x;
      dy = p_line2_end->y - p_line2_start->y;
      if (Fbk_Abs_F(dx) > THRESHOLD_IS_ZERO)
      {
         /* Get slope of line 2. */
         m2 = dy / dx;

         /* Get intercept of line 2. */
         c2 = p_line2_start->y - (m2 * p_line2_start->x);
      }

      if (Fbk_Abs_F(m1 - m2) < THRESHOLD_IS_ZERO)
      {
         /* Lines are parallel, no intersection. */
         p_intersection_point->x = INFINITY;
         p_intersection_point->y = INFINITY;
      }
      else if ((INFINITY == m1) && (INFINITY == m2))
      {
         /* Both lines have infinite slopes. */
         p_intersection_point->x = INFINITY;
         p_intersection_point->y = INFINITY;
      }
      else if ((INFINITY == m1) && (INFINITY != m2))
      {
         /* Line 1 has an infinite slope and is parallel to y-axis. */
         p_intersection_point->x = p_line1_start->x;
         p_intersection_point->y = (m2 * p_intersection_point->x) + c2;

         /* Lines are intersecting (not necessarily in range of given line segments). */
         f_lines_intersecting = FBK_TRUE;
      }
      else if ((INFINITY != m1) && (INFINITY == m2))
      {
         /* Line 2 has an infinite slope and is parallel to y-axis */
         p_intersection_point->x = p_line2_start->x;
         p_intersection_point->y = (m1 * p_intersection_point->x) + c1;

         /* Lines are intersecting (not necessarily in range of given line segments). */
         f_lines_intersecting = FBK_TRUE;
      }
      else
      {
         /* Calculate intersection point. */
         p_intersection_point->x = (c2 - c1) / (m1 - m2);
         p_intersection_point->y = (m1 * p_intersection_point->x) + c1;

         /* Lines are intersecting (not necessarily in range of given line segments). */
         f_lines_intersecting = FBK_TRUE;
      }
   }

   return f_lines_intersecting;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Is_Intersection_Point_In_Range(Vector_2d_T *p_intersection_point,
                                             const Vector_2d_T *p_line1_start,
                                             const Vector_2d_T *p_line1_end,
                                             const Vector_2d_T *p_line2_start,
                                             const Vector_2d_T *p_line2_end)
{
   boolean_T f_lines_intersect, f_point_in_range;
   Float_Range_T range1_x, range1_y, range2_x, range2_y;

   f_lines_intersect = Fbk_Are_Two_Lines_Intersecting(p_intersection_point, p_line1_start, p_line1_end, p_line2_start, p_line2_end);

   range1_x = Create_Float_Range(p_line1_start->x, p_line1_end->x);
   range1_y = Create_Float_Range(p_line1_start->y, p_line1_end->y);
   range2_x = Create_Float_Range(p_line2_start->x, p_line2_end->x);
   range2_y = Create_Float_Range(p_line2_start->y, p_line2_end->y);

   f_point_in_range = FBK_FALSE;

   if (Fbk_Is_True(f_lines_intersect) && Is_Float_Contained_In_Float_Range(p_intersection_point->x, &range1_x)
       && Is_Float_Contained_In_Float_Range(p_intersection_point->x, &range2_x)
       && Is_Float_Contained_In_Float_Range(p_intersection_point->y, &range1_y)
       && Is_Float_Contained_In_Float_Range(p_intersection_point->y, &range2_y))
   {
      f_point_in_range = FBK_TRUE;
   }

   return f_point_in_range;
}


/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
float32_T Fbk_Get_Time_To_Leave_Field_Of_Interest_Given_Point_And_Velocity_Vector(const Fbk_Field_Of_Interest_T *p_foi,
                                                                                  const Vector_2d_T *p_vel_vector_origin,
                                                                                  const Vector_2d_T *p_vel_vector_dir)
{
   /* Return value */
   float32_T time_to_leave = FBK_INVALID_TIME;
   boolean_T f_intersecting;

   /* Iterator */
   uint8_t point_index;

   float32_T min_distance_to_foi_edge = FBK_INVALID_DISTANCE;

   /* Loop over FoI line segments. */
   for (point_index = FBK_ZERO_UINT; point_index < p_foi->size; point_index++)
   {
      /* Set next point index and loop around to zero for last entry. */
      const uint8_t next_point_index = (uint8_t) (point_index + FBK_ONE_UINT) % p_foi->size;

      /* First check if the general direction of the line segment matches the velocity vector direction. */
      if (Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(&(p_foi->points[point_index]), p_vel_vector_origin, p_vel_vector_dir)
          || Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(&(p_foi->points[next_point_index]), p_vel_vector_origin,
                                                                   p_vel_vector_dir))
      {
         /* Get intersection point of two lines. */
         Vector_2d_T intersection_point;
         const Vector_2d_T vel_vector_end = Vector_2d_Alg_Add(p_vel_vector_origin, p_vel_vector_dir);
         f_intersecting = (boolean_T) Fbk_Are_Two_Lines_Intersecting(&intersection_point, &(p_foi->points[point_index]),
                                                                     &(p_foi->points[next_point_index]), p_vel_vector_origin,
                                                                     &vel_vector_end);
         if (f_intersecting)
         {
            /* Calculate distance between intersection point and velocity vector origin. */
            const float32_T distance = Vector_2d_Alg_Distance(&intersection_point, p_vel_vector_origin);

            /* Set distance to min distance if smaller than previous values. */
            min_distance_to_foi_edge = Fbk_Min(distance, min_distance_to_foi_edge);
         }
      }
   }

   /* Calculate time to leave FoI border. */
   if (min_distance_to_foi_edge < FBK_INVALID_DISTANCE)
   {
      const float32_T velocity = Vector_2d_Alg_Abs(p_vel_vector_dir);
      if (velocity > THRESHOLD_IS_ZERO)
      {
         time_to_leave = min_distance_to_foi_edge / velocity;
      }
   }

   return time_to_leave;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
float32_T Fbk_Get_Time_For_Object_To_Leave_Field_Of_Interest(const Fbk_Field_Of_Interest_T *p_zone_foi,
                                                             const Fbk_Field_Of_Interest_T *p_object_foi,
                                                             const Vector_2d_T *p_object_position,
                                                             const Vector_2d_T *p_object_velocity,
                                                             const boolean_T f_point_shift)
{
   /* Return value*/
   float32_T time_to_leave = FBK_INVALID_TIME;

   /* Iterator */
   uint8_t point_index;

   const uint8_t shift_point[] = {FBK_FOI_FRONT_RIGHT_CORNER, FBK_FOI_FRONT_LEFT_CORNER, FBK_FOI_REAR_LEFT_CORNER,
                                  FBK_FOI_REAR_RIGHT_CORNER};

   boolean_T f_ttp_calculated = FBK_FALSE;

   /* Set the reverse velocity vector. */
   const Vector_2d_T reverse_velocity = Vector_2d_Alg_Multiply_Scalar(p_object_velocity, -FBK_ONE_F);

   /* Iterate over object zone points to find the point opposite of the velocity vector. */
   for (point_index = FBK_ZERO_UINT; point_index < p_object_foi->size; point_index++)
   {
      if (Fbk_Is_Field_Of_Interest_Point_In_Direction_Of_Vector(&(p_object_foi->points[point_index]), p_object_position,
                                                                &reverse_velocity))
      {
         /* Also check that this point is located inside the zone FoI to avoid intersection edge cases. */
         if (Is_Point_In_Convex_Polygon_Ray_Casting_Method(p_zone_foi->points, p_zone_foi->size, &(p_object_foi->points[point_index])))
         {
            /* Calculate time to leave given the zone FoI
               and using the object point opposite to the object travel direction as the reference. */
            time_to_leave = Fbk_Get_Time_To_Leave_Field_Of_Interest_Given_Point_And_Velocity_Vector(
               p_zone_foi, &(p_object_foi->points[point_index]), p_object_velocity);

            f_ttp_calculated = FBK_TRUE;
         }

         /* If ref point outside the zone (and TTP not calculated yet) allow to switch ref point to opposite corner of the target
          * in lateral direction*/
         if (Fbk_Is_True(f_point_shift) && Fbk_Is_False(f_ttp_calculated))
         {
            time_to_leave = Fbk_Get_Time_To_Leave_Field_Of_Interest_Given_Point_And_Velocity_Vector(
               p_zone_foi, &(p_object_foi->points[shift_point[point_index]]), p_object_velocity);

            f_ttp_calculated = FBK_TRUE;
         }

         /* If TTP calculated break the loop. */
         if (Fbk_Is_True(f_ttp_calculated))
         {
            break;
         }
      }
   }

   return time_to_leave;
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Get_Intersection_Polygon(Fbk_Field_Of_Interest_T *result_polygon,
                                  const Fbk_Field_Of_Interest_T *first_polygon,
                                  const Fbk_Field_Of_Interest_T *second_polygon)
{
   boolean_T f_point_in_polygon2, f_point_in_polygon1;
   result_polygon->size = FBK_ZERO_UINT;


   /* Check if any corner of polygon 1 is inside polygon 2 (and vice versa) and on its base find intersection points. All points
    * found create intersection polygon */
   f_point_in_polygon2 = Fbk_Get_Intersection_Points_For_Point_In_Polygon(result_polygon, first_polygon, second_polygon);
   f_point_in_polygon1 = Fbk_Get_Intersection_Points_For_Point_In_Polygon(result_polygon, second_polygon, first_polygon);

   /* TBD: intersection points for case when 0 corners inside another polygon -> verify if it applies to real scenarios
    */

   /* Sort polygon points in counterclockwise order */
   if (Fbk_Is_False(f_point_in_polygon1) && Fbk_Is_False(f_point_in_polygon2))
   {
      Fbk_Sort_Polygon_Points(result_polygon);
   }
}


/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Get_Intersection_Points_For_Point_In_Polygon(Fbk_Field_Of_Interest_T *result_polygon,
                                                           const Fbk_Field_Of_Interest_T *first_polygon,
                                                           const Fbk_Field_Of_Interest_T *second_polygon)
{
   Vector_2d_T intersection_point;

   boolean_T f_intersection;
   boolean_T f_point_exists;
   boolean_T f_point_in_polygon;
   boolean_T f_any_point_in_polygon = FBK_FALSE;

   uint8_t i, j;

   /* Check if any corner of polygon1 is inside polygon2. If true, add this point to result polygon and verify if two lines
    * creating this point intersect with second polygon */
   for (i = FBK_ZERO_UINT; i < first_polygon->size; i++)
   {
      f_point_in_polygon =
         Is_Point_In_Convex_Polygon_Ray_Casting_Method(second_polygon->points, second_polygon->size, &(first_polygon->points[i]));
      if (Fbk_Is_True(f_point_in_polygon))
      {
         result_polygon->points[result_polygon->size] = first_polygon->points[i];
         Sat_Inc_Uint8(&result_polygon->size);
         f_any_point_in_polygon = FBK_TRUE;

         /* Intersection of the line with all edges of the polygon 2 */
         for (j = FBK_ZERO_UINT; j < second_polygon->size; j++)
         {

            /* First line (points n and n-1) of polygon 1 starting in considered corner point */
            if (i == FBK_ZERO_UINT)
            {
               f_intersection = Fbk_Is_Intersection_Point_In_Range(
                  &intersection_point, &(first_polygon->points[i]), &(first_polygon->points[first_polygon->size - FBK_ONE_UINT]),
                  &(second_polygon->points[j]), &(second_polygon->points[(j + FBK_ONE_UINT) % second_polygon->size]));
            }
            else
            {
               f_intersection = Fbk_Is_Intersection_Point_In_Range(
                  &intersection_point, &(first_polygon->points[i]), &(first_polygon->points[i - FBK_ONE_UINT]),
                  &(second_polygon->points[j]), &(second_polygon->points[(j + FBK_ONE_UINT) % second_polygon->size]));
            }
            f_point_exists = Fbk_Is_Point_In_Array(&intersection_point, result_polygon->points, result_polygon->size);

            /* Add intersection point to result polygon */
            if (Fbk_Is_True(f_intersection) && Fbk_Is_False(f_point_exists))
            {
               result_polygon->points[result_polygon->size] = intersection_point;
               Sat_Inc_Uint8(&result_polygon->size);
            }

            /* Second line (points n and n+1) of polygon 1 starting in considered corner point */
            f_intersection = Fbk_Is_Intersection_Point_In_Range(
               &intersection_point, &(first_polygon->points[i]), &(first_polygon->points[(i + FBK_ONE_UINT) % first_polygon->size]),
               &(second_polygon->points[j]), &(second_polygon->points[(j + FBK_ONE_UINT) % second_polygon->size]));
            f_point_exists = Fbk_Is_Point_In_Array(&intersection_point, result_polygon->points, result_polygon->size);

            /* Add intersection point to result polygon */
            if (Fbk_Is_True(f_intersection) && Fbk_Is_False(f_point_exists))
            {
               result_polygon->points[result_polygon->size] = intersection_point;
               Sat_Inc_Uint8(&result_polygon->size);
            }
         }
      }
   }

   return f_any_point_in_polygon;
}


/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Sort_Polygon_Points(Fbk_Field_Of_Interest_T *p_polygon)
{
   float32_T mean_x = FBK_ZERO_F;
   float32_T mean_y = FBK_ZERO_F;
   Vector_2d_T buf[FBK_MAX_SIZE_OF_FOI];
   float32_T angles[FBK_MAX_SIZE_OF_FOI][2u];
   uint8_t i, idx;

   assert(NULL != p_polygon);

   /* Calculate center point of polygon (average value) */
   for (i = FBK_ZERO_UINT; i < p_polygon->size; i++)
   {
      mean_x += p_polygon->points[i].x;
      mean_y += p_polygon->points[i].y;
   }

   if (p_polygon->size > FBK_ZERO_UINT)
   {
      mean_x /= (float32_T) p_polygon->size;
      mean_y /= (float32_T) p_polygon->size;
   }

   /* Calculate angle of the line created by corner of the polygon and center point */
   for (i = FBK_ZERO_UINT; i < p_polygon->size; i++)
   {
      angles[i][FBK_ZERO_UINT] = (float32_T) i;
      angles[i][FBK_ONE_UINT]  = Fast_Atan2(p_polygon->points[i].y - mean_y, p_polygon->points[i].x - mean_x);

      /* Polygon points saved in buffer*/
      buf[i].x = p_polygon->points[i].x;
      buf[i].y = p_polygon->points[i].y;
   }

   /* coverity[misra_c_2012_rule_21_9_violation][qsort is only used on small array with well-defined comparison function]*/
   qsort(angles, p_polygon->size, sizeof(angles[FBK_ZERO_UINT]), Fbk_Compare_Points_Angle_From_Origin);

   /* Update polygon with sorted points */
   for (i = FBK_ZERO_UINT; i < p_polygon->size; i++)
   {
      idx                    = (uint8_t) angles[i][FBK_ZERO_UINT];
      p_polygon->points[i].x = buf[idx].x;
      p_polygon->points[i].y = buf[idx].y;
   }
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
boolean_T Fbk_Is_Point_In_Array(const Vector_2d_T *p_point, const Vector_2d_T *p_array, const uint8_t size)
{
   boolean_T result = FBK_FALSE;

   uint8_t i;

   assert(NULL != p_point);
   assert(NULL != p_array);
   assert(FBK_MAX_SIZE_OF_FOI >= size);

   for (i = FBK_ZERO_UINT; i < size; i++)
   {
      if (Fbk_Equal_F(p_point->x, p_array[i].x) && Fbk_Equal_F(p_point->y, p_array[i].y))
      {
         result = FBK_TRUE;
         break;
      }
   }

   return result;
}
