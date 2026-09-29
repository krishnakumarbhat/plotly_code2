/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <assert.h>
#include "reuse.h"

#include "ml_vector_2d_assertion_helper.h"
#include "ml_bool.h"
#include "ml_interval.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"

#ifdef ST_ENABLE_SPE2_VEC
#include "st_vector_2d_spe.h" // Header File for Vector2D SPE2 Code.
#endif

#define VECTOR_2D_ALGEBRA_HALF    (0.5f)


Vector_2d_T Create_2d_Vector_Coordinates(
   const float32_T x,
   const float32_T y)
{
   Vector_2d_T ret_value;

   assert(Is_Not_Nan(x));
   assert(Is_Not_Nan(y));

   ret_value.x = x;
   ret_value.y = y;

   return ret_value;
}

Vector_2d_T Create_2d_Vector_Origin(void)
{
   Vector_2d_T ret_value;

   ret_value.x = 0;
   ret_value.y = 0;

   return ret_value;
}

Vector_2d_T Create_2d_Vector_X_Normal(void)
{
   Vector_2d_T ret_value;

   ret_value.x = 1;
   ret_value.y = 0;

   return ret_value;
}

Vector_2d_T Create_2d_Vector_Y_Normal(void)
{
   Vector_2d_T ret_value;

   ret_value.x = 0;
   ret_value.y = 1;

   return ret_value;
}

Vector_2d_T Vector_2d_Alg_Multiply_Scalar(
   const Vector_2d_T *const p_vector_a,
   const float32_T          factor)
{
   Vector_2d_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(Is_Not_Nan(factor));

#ifdef ST_ENABLE_SPE2_VEC
   Alg_Multiply_Scalar_Asm((float *)p_vector_a, (float *)&factor, (float *)&ret_value);
#else
   ret_value.x = p_vector_a->x * factor;
   ret_value.y = p_vector_a->y * factor;
#endif

   return ret_value;
}

Vector_2d_T Vector_2d_Alg_Add(
   const Vector_2d_T *const p_vector_a,
   const Vector_2d_T *const p_vector_b)
{
   Vector_2d_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

#ifdef ST_ENABLE_SPE2_VEC
   Alg_Add_Asm((float *)p_vector_a, (float *)p_vector_b, (float *)&ret_value);
#else
   ret_value.x = p_vector_a->x + p_vector_b->x;
   ret_value.y = p_vector_a->y + p_vector_b->y;
#endif

   return ret_value;
}

Vector_2d_T Vector_2d_Alg_Middle(
   const Vector_2d_T *const p_vector_a,
   const Vector_2d_T *const p_vector_b)
{
   Vector_2d_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

   ret_value = Vector_2d_Alg_Add(p_vector_a, p_vector_b);
   ret_value = Vector_2d_Alg_Multiply_Scalar(&ret_value, VECTOR_2D_ALGEBRA_HALF);

   return ret_value;
}

float32_T Vector_2d_Alg_Scalar_Product(
   const Vector_2d_T *const p_vector_a,
   const Vector_2d_T *const p_vector_b)
{
   float32_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

#ifdef ST_ENABLE_SPE2_VEC
   ret_value = Alg_Scalar_Product_Asm((float *)p_vector_a, (float *)p_vector_b);
#else
   ret_value = (p_vector_a->x * p_vector_b->x) + (p_vector_a->y * p_vector_b->y);
#endif

   return ret_value;
}

float32_T Vector_2d_Alg_Distance(
   const Vector_2d_T *const p_vector_a,
   const Vector_2d_T *const p_vector_b)
{
   float32_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

   ret_value = Fast_Hypot(p_vector_a->x - p_vector_b->x, p_vector_a->y - p_vector_b->y);

   return ret_value;
}

Vector_2d_T Vector_2d_Alg_Rotate_Half_Pi(const Vector_2d_T *const p_vector)
{
   Vector_2d_T ret_value;

   assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

   ret_value = Create_2d_Vector_Coordinates(p_vector->y, -p_vector->x);

   return ret_value;
}

Vector_2d_T Vector_2d_Alg_Normalize_Vector(const Vector_2d_T *const p_vector)
{
   Vector_2d_T ret_value;

   assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

   assert(p_vector->x != 0 || p_vector->y != 0);      /* This prevents division by zero */

   ret_value = Vector_2d_Alg_Multiply_Scalar(p_vector, 1 / (Vector_2d_Alg_Abs(p_vector)));

   return ret_value;
}

float32_T Vector_2d_Alg_Abs(const Vector_2d_T *const p_vector)
{
   assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

#ifdef ST_ENABLE_SPE2_VEC
   return Alg_Abs_Asm((float *)p_vector);
#else
   return Fast_Sqrt((p_vector->x * p_vector->x) + (p_vector->y * p_vector->y));
#endif
}

float32_T Vector_2d_Alg_Abs_Squared(const Vector_2d_T *const p_vector)
{
   float32_T ret_val;
   assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

#ifdef ST_ENABLE_SPE2_VEC
   ret_val = Alg_Abs_Sq_Asm((float *)p_vector);
#else
   ret_val = (p_vector->x * p_vector->x) + (p_vector->y * p_vector->y);
#endif
   return ret_val;
}

Vector_2d_T Vector_2d_Alg_Diff(
   const Vector_2d_T *const p_vector_a,
   const Vector_2d_T *const p_vector_b)
{
   Vector_2d_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

#ifdef ST_ENABLE_SPE2_VEC
   Alg_Diff_Asm((float *)p_vector_a, (float *)p_vector_b, (float *)&ret_value);
#else
   ret_value.x = p_vector_a->x - p_vector_b->x;
   ret_value.y = p_vector_a->y - p_vector_b->y;
#endif
   return ret_value;
}

Vector_2d_T Vector_2d_Alg_Abs_Component_Wise(const Vector_2d_T *const p_vector_a)
{
   Vector_2d_T ret_vec;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

#ifdef ST_ENABLE_SPE2_VEC
   Alg_Abs_CompWise_Asm((float *)p_vector_a, (float *)&ret_vec);
#else
   ret_vec.x = Abs(p_vector_a->x);
   ret_vec.y = Abs(p_vector_a->y);
#endif

   return ret_vec;
}

Vector_2d_T Vector_2d_Alg_Sqrt_Component_Wise(const Vector_2d_T *const p_vector_a)
{
   Vector_2d_T ret_vec;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(0 <= p_vector_a->x);
   assert(0 <= p_vector_a->y);

#ifdef ST_ENABLE_SPE2_VEC
   Alg_Sqrt_CompWise_Asm((float *)p_vector_a, (float *)&ret_vec);
#else
   ret_vec.x = (float32_T)Fast_Sqrt(p_vector_a->x);
   ret_vec.y = (float32_T)Fast_Sqrt(p_vector_a->y);
#endif

   return ret_vec;
}

float32_T Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(
   const Vector_2d_T *const p_vector_a,
   const Vector_2d_T *const p_vector_b)
{
   float32_T dot_product;
   float32_T length_vector_a;
   float32_T length_vector_b;
   float32_T cos_theta = VECTOR_2D_ALGEBRA_NOT_A_COSINE;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));
   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

   dot_product = Vector_2d_Alg_Scalar_Product(p_vector_a, p_vector_b);
   length_vector_a = Vector_2d_Alg_Abs(p_vector_a);
   length_vector_b = Vector_2d_Alg_Abs(p_vector_b);
   if ((length_vector_a > THRESHOLD_IS_ZERO) && (length_vector_b > THRESHOLD_IS_ZERO))
   {
      /* cos(angle_between(a,b)) =a*b / (|a|*|b|)*/
      cos_theta = dot_product / (length_vector_a * length_vector_b);
   }
   else
   {
      /*To do the calculation a division by both vector length is needed.Therefore the vector lengths need to be nonzero
      * This is in this else cause not the case. Throw an assertion*/
      assert(FALSE);
   }

   return cos_theta;
}

Vector_2d_T Vector_2d_Alg_Limit_Vector(
   const Vector_2d_T *const p_vect_in,
   const Vector_2d_T *const p_limit_max_in,
   const Vector_2d_T *const p_limit_min_in)
{
   Vector_2d_T vect_out;

   assert(p_vect_in != NULL);
   assert(Vector_Is_Not_Nan(p_vect_in));
   assert(p_limit_max_in != NULL);
   assert(Vector_Is_Not_Nan(p_limit_max_in));
   assert(p_limit_min_in != NULL);
   assert(Vector_Is_Not_Nan(p_limit_min_in));

#ifdef ST_ENABLE_SPE2_VEC
   Alg_LimVector_Asm((float *)p_vect_in, (float *)p_limit_max_in, (float *)p_limit_min_in, (float *)&vect_out);
#else
   vect_out.x = Enforce_Range(p_vect_in->x, p_limit_min_in->x, p_limit_max_in->x);
   vect_out.y = Enforce_Range(p_vect_in->y, p_limit_min_in->y, p_limit_max_in->y);
#endif

   return vect_out;
}

