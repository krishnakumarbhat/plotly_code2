/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <assert.h>
#include "ml_angle.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_trigonometry.h"

Angle_T Create_Angle(const float32_T angle_rad)
{
   Angle_T angle_return;

   assert(Is_Not_Nan(angle_rad));/* cppcheck-suppress duplicateExpression */

   angle_return.angle = angle_rad;
   angle_return.sin = Fast_Sin(angle_rad);
   angle_return.cos = Fast_Cos(angle_rad);

   return angle_return;
}

void Normalize_Angle_Struct(
   Angle_T  *p_theta_in,
   const Angle_T  *p_theta_ref)
{
   float32_T normalized_angle;

   assert(NULL != p_theta_in);
   assert(NULL != p_theta_ref);

   normalized_angle = Normalize_Angle(p_theta_in->angle, p_theta_ref->angle);

   *p_theta_in = Create_Angle(normalized_angle);
}

float32_T Normalize_Angle(
   const float32_T theta_in,
   const float32_T theta_ref)
{
   float32_T mod_out;
   float32_T theta_out;

   assert(Is_Not_Nan(theta_in));/* cppcheck-suppress duplicateExpression */
   assert(Is_Not_Nan(theta_ref));/* cppcheck-suppress duplicateExpression */

   mod_out = theta_in + ((float32_T)PI - theta_ref);

   assert(Is_Not_Nan(mod_out));/* cppcheck-suppress duplicateExpression */
   if (Is_Nan(mod_out)) /*PRQA S 3341*/ /* The comparison of floating point expressions for equality is intended. This is a check for NaN */
   {
      theta_out = mod_out;
   }
   else
   {
      float32_T pi_theta_ref;
      /* not nan, do floating point mod */
      /* Since for most calls to Normalize_Angle both angles will be
      * close to each other try to solve the ambiguity with a subtraction.
      * Only do more computational intensive calculations if this fails.*/
      if (mod_out >= (2.0f * PI))
      {
         mod_out -= 2.0f * ((float32_T)PI);
      }
      else if (mod_out < 0.0f)
      {
         mod_out += 2.0f * ((float32_T)PI);
      }
      else
      {
         /* nothing to do */
      }
      pi_theta_ref = (((float32_T)PI) - theta_ref);
      if ((mod_out >= 0.0f) && (mod_out <= (2.0f*PI)))
      {
         theta_out = mod_out - pi_theta_ref;
      }
      else
      {
         theta_out = theta_in - ((2.0f*PI) * floorf((theta_in + pi_theta_ref) / (2.0f*PI)));
      }
   }
   return theta_out;
}

Angle_T Angle_Diff(
   const Angle_T * const p_angle_a,
   const Angle_T * const p_angle_b)
{
   Angle_T   angle_return;
   float32_T normalized_a;
   float32_T diff_of_angles;
   assert(NULL != p_angle_a);
   assert(NULL != p_angle_b);


   normalized_a = Normalize_Angle(p_angle_a->angle, p_angle_b->angle);
   diff_of_angles = normalized_a - p_angle_b->angle;

   angle_return = Create_Angle(diff_of_angles);
   return angle_return;
}

Angle_T Angle_Mean(
   const Angle_T * const p_angle_a,
   const Angle_T * const p_angle_b)
{
   Angle_T   angle_return;
   float32_T normalized_a;
   float32_T mean_of_angles;
   assert(NULL != p_angle_a);
   assert(NULL != p_angle_b);


   normalized_a = Normalize_Angle(p_angle_a->angle, p_angle_b->angle);
   mean_of_angles = 0.5f*(normalized_a + p_angle_b->angle);
   mean_of_angles = Normalize_Angle(mean_of_angles, 0.0f);

   angle_return = Create_Angle(mean_of_angles);
   return angle_return;
}
