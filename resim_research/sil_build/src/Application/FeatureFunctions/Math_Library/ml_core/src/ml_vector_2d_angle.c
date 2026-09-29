/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <assert.h>
#include "ml_vector_2d_assertion_helper.h"
#include "ml_angle_assertion_helper.h"
#include "ml_angle.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"

#ifdef ST_ENABLE_SPE2_VEC
#include "ml_vector_2d_spe.h" // Header File for Vector2D SPE2 Code.
#endif


Vector_2d_T Vector_2d_Alg_Rotate(
   const Angle_T *const     p_angle,
   const Vector_2d_T *const p_vector)
{
   Vector_2d_T ret_value;

   assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

   assert(p_angle != NULL);
   assert(Angle_Is_Not_Nan(p_angle));

#ifdef ST_ENABLE_SPE2_VEC
   {
      float32_T in_a[2];
      float32_T in_b[2];
      float32_T output_value[2];

      in_a[0] = p_vector->x;
      in_a[1] = p_vector->y;

      in_b[0] = p_angle->sin;
      in_b[1] = p_angle->cos;

      Alg_Rotate_Asm((float *)in_a, (float *)in_b, (float *)output_value);

      ret_value.x = output_value[0];
      ret_value.y = output_value[1];
   }
#else
   ret_value.x = (p_vector->x * p_angle->cos) - (p_vector->y * p_angle->sin);
   ret_value.y = (p_vector->x * p_angle->sin) + (p_vector->y * p_angle->cos);
#endif

   return ret_value;
}

Angle_T Vector_2d_Alg_Angle_From_Vector(const Vector_2d_T * const p_vector)
{
   Angle_T angle_ret;

   assert(NULL != p_vector);

   angle_ret.angle = Fast_Atan2(p_vector->y, p_vector->x);
   angle_ret = Create_Angle(angle_ret.angle);

   return angle_ret;
}

float32_T Vector_2d_Alg_Project_On_Angle(
   const Vector_2d_T *const p_vector_to_project,
   const Angle_T *const     p_angle)
{
   float32_T ret_value;

   assert(p_vector_to_project != NULL);
   assert(Vector_Is_Not_Nan(p_vector_to_project));

   assert(p_angle != NULL);
   assert(Angle_Is_Not_Nan(p_angle));

   ret_value = Vector_2d_Alg_Abs(p_vector_to_project) * p_angle->cos;

   return ret_value;
}

float32_T Vector_2d_Alg_Project_On_Rotated_X_Axis(
   const Vector_2d_T *const p_vector_to_project,
   const Angle_T *const     p_angle)
{
   float32_T ret_value;

   assert(p_vector_to_project != NULL);
   assert(Vector_Is_Not_Nan(p_vector_to_project));

   assert(p_angle != NULL);
   assert(Angle_Is_Not_Nan(p_angle));

   ret_value = (p_vector_to_project->x * p_angle->cos) + (p_vector_to_project->y * p_angle->sin);

   return ret_value;
}

Vector_2d_T Vector_2d_Alg_Rotate_Negative(
   const Angle_T *const     p_angle,
   const Vector_2d_T *const p_vector)
{
   /*Rotation matrix
   * R(-a) = [ cos(-a)   -sin(-a);
   * sin(-a)    cos(-a)];
   * = [ cos(a)     sin(a)
   * -sin(a)     cos(a)]*/

   Vector_2d_T point_out;

   assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

   assert(p_angle != NULL);
   assert(Angle_Is_Not_Nan(p_angle));

#ifdef ST_ENABLE_SPE2_VEC
   {
      float32_T in_a[2];
      float32_T in_b[2];
      float32_T output_value[2];

      in_a[0] = p_vector->x;
      in_a[1] = p_vector->y;

      in_b[0] = p_angle->sin;
      in_b[1] = p_angle->cos;

      Alg_RotateNegative_Asm((float *)in_a, (float *)in_b, (float *)output_value);

      point_out.x = output_value[0];
      point_out.y = output_value[1];
   }
#else
   point_out.x = (p_angle->cos * p_vector->x) + (p_angle->sin * p_vector->y);
   point_out.y = (-p_angle->sin * p_vector->x) + (p_angle->cos * p_vector->y);
#endif

   return point_out;
}

float32_T Vector_2d_Alg_Scalar_Product_With_Angle(
   const Vector_2d_T *const p_vector,
   const Angle_T *const     p_angle /**< relative to x axis*/)
{
   float32_T ret_value;

   assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

   assert(p_angle != NULL);
   assert(Angle_Is_Not_Nan(p_angle));

#ifdef ST_ENABLE_SPE2_VEC
   {
      float32_T in_a[2];
      float32_T in_b[2];

      in_a[0] = p_vector->x;
      in_a[1] = p_vector->y;

      in_b[0] = p_angle->sin;
      in_b[1] = p_angle->cos;

      ret_value = Alg_Scalar_Product_with_angle_Asm((float *)in_a, (float *)in_b);
   }
#else
   ret_value = (p_vector->x * p_angle->cos) + (p_vector->y * p_angle->sin);
#endif

   return ret_value;
}

Vector_2d_T Vector_2d_Alg_Angle_To_Vector(const Angle_T *const p_angle /**< Angle towards x-axis */)
{
   Vector_2d_T vector;

   assert(NULL != p_angle);

   vector.x = p_angle->cos;
   vector.y = p_angle->sin;

   return vector;
}


Vector_2d_T Vector_2d_Alg_Angle_To_Perpendicular_Vector(const Angle_T *const p_angle /**< Angle towards x-axis */)
{
   Vector_2d_T vector;

   assert(NULL != p_angle);

   vector.x = -p_angle->sin;
   vector.y = p_angle->cos;

   return vector;
}
