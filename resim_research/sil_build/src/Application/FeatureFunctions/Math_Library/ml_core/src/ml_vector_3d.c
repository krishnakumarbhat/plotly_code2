/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_vector_3d.h"
#include "ml_vector_3d_tag.h"
#include "ml_math.h"
#include <assert.h>

#define Vector_Is_Not_Nan(vector) (Is_Not_Nan((vector)->x) && Is_Not_Nan((vector)->y) && Is_Not_Nan((vector)->z)) /* PRQA S 3453 */ /* Macro is used for assertions */

#define VECTOR_3D_ALGEBRA_HALF    (0.5f)

Vector_3d_T Create_3d_Vector_Coordinates(
   const float32_T x,
   const float32_T y,
   const float32_T z
   )
   {
       Vector_3d_T ret;
       ret.x = x;
       ret.y = y;
       ret.z = z;
       return ret;
   }

Vector_3d_T Create_3d_Vector_Origin(void)
{
    Vector_3d_T ret;
    ret.x = 0.0f;
    ret.y = 0.0f;
    ret.z = 0.0f;
    return ret;
}

Vector_3d_T Create_3d_Vector_X_Normal(void)
{
    Vector_3d_T ret;
    ret.x = 1.0f;
    ret.y = 0.0f;
    ret.z = 0.0f;
    return ret;
}

Vector_3d_T Create_3d_Vector_Y_Normal(void)
{
    Vector_3d_T ret;
    ret.x = 0.0f;
    ret.y = 1.0f;
    ret.z = 0.0f;
    return ret;
}

Vector_3d_T Create_3d_Vector_Z_Normal(void)
{
    Vector_3d_T ret;
    ret.x = 0.0f;
    ret.y = 0.0f;
    ret.z = 1.0f;
    return ret;
}

Vector_3d_T Vector_3d_Alg_Multiply_Scalar(
   const Vector_3d_T * const p_vector_a,
   const float32_T factor
   )
{
    Vector_3d_T ret;
    ret.x = p_vector_a->x * factor;
    ret.y = p_vector_a->y * factor;
    ret.z = p_vector_a->z * factor;
    return ret;
}

Vector_3d_T Vector_3d_Alg_Add(
   const Vector_3d_T * const p_vector_a,
   const Vector_3d_T * const p_vector_b
   )
{
    Vector_3d_T ret;
    ret.x = p_vector_a->x + p_vector_b->x;
    ret.y = p_vector_a->y + p_vector_b->y;
    ret.z = p_vector_a->z + p_vector_b->z;
    return ret;
}

Vector_3d_T Vector_3d_Alg_Middle(
   const Vector_3d_T * const p_vector_a,
   const Vector_3d_T * const p_vector_b
   )
{
    Vector_3d_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

   ret_value = Vector_3d_Alg_Add(p_vector_a, p_vector_b);
   ret_value = Vector_3d_Alg_Multiply_Scalar(&ret_value, VECTOR_3D_ALGEBRA_HALF);

   return ret_value;
}

float32_T Vector_3d_Alg_Scalar_Product(
   const Vector_3d_T * const p_vector_a,
   const Vector_3d_T * const p_vector_b
   )
{
    float32_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

   ret_value = (p_vector_a->x * p_vector_b->x) + (p_vector_a->y * p_vector_b->y);

   return ret_value;
}


Vector_3d_T Vector_3d_Alg_Normalize_Vector(const Vector_3d_T * const p_vector)
{
    Vector_3d_T ret_value;

   assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

   assert(p_vector->x != 0 || p_vector->y != 0);      /* This prevents division by zero */

   ret_value = Vector_3d_Alg_Multiply_Scalar(p_vector, 1 / (Vector_3d_Alg_Abs(p_vector)));

   return ret_value;
}

float32_T Vector_3d_Alg_Abs(const Vector_3d_T * const p_vector)
{
    assert(p_vector != NULL);
    assert(Vector_Is_Not_Nan(p_vector));

    return Fast_Sqrt((p_vector->x * p_vector->x) + (p_vector->y * p_vector->y) + (p_vector->z * p_vector->z));
}

float32_T Vector_3d_Alg_Abs_Squared(const Vector_3d_T * const p_vector)
{
    assert(p_vector != NULL);
   assert(Vector_Is_Not_Nan(p_vector));

   return (p_vector->x * p_vector->x) + (p_vector->y * p_vector->y) + (p_vector->z * p_vector->z);
}

Vector_3d_T Vector_3d_Alg_Diff(
   const Vector_3d_T * p_vector_a,
   const Vector_3d_T * p_vector_b)
{
    Vector_3d_T ret_value;

   assert(p_vector_a != NULL);
   assert(Vector_Is_Not_Nan(p_vector_a));

   assert(p_vector_b != NULL);
   assert(Vector_Is_Not_Nan(p_vector_b));

   ret_value.x = p_vector_a->x - p_vector_b->x;
   ret_value.y = p_vector_a->y - p_vector_b->y;
   ret_value.z = p_vector_a->z - p_vector_b->z;

   return ret_value;
}
