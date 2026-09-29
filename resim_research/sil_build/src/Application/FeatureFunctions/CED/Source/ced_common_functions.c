/**
 * @file ced_common_functions.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Core CED algorithm.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "ced_common_functions.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "ml_math.h"

/*===========================================================================*\
 * Global Functions	Definition
 \*===========================================================================*/

float32_T Ced_Get_Time_To_Travel_Given_Distance(const float32_T distance /**< Distance [m] */,
                                                const float32_T velocity /**< Velocity [m/s] */,
                                                const float32_T acceleration /**< Acceleration [m/s^2] */)
{
   float32_T time_to_travel_distance = CED_INVALID_TIME;

   if (velocity >= EPSILON)
   {
      if (Fbk_Abs_F(acceleration) >= EPSILON)
      {
         /* Calculate time to travel distance based on velocity and acceleration
          *
          * a = acceleration
          * v = velocity
          * d = distance
          *
          * 1/2*at^2 + vt - d = 0  converted to pq-formula:  t^2 + (2v/a)t - (2d)/a */

         float32_T p_half, q, radicand;

         p_half = velocity / acceleration;
         q      = (-2.0f * distance) / acceleration;

         radicand = (p_half * p_half) - q;

         /* Check if real roots are found */
         if (radicand >= FBK_ZERO_F)
         {
            if (acceleration > FBK_ZERO_F)
            {
               /* Object is accelerating. First root is taken as relevant time. */
               time_to_travel_distance = -p_half + Fast_Sqrt(radicand);
            }
            else
            {
               /* Object is decelerating. Second root is taken as relevant time. */
               time_to_travel_distance = -p_half - Fast_Sqrt(radicand);
            }
         }
      }
      else
      {
         /* Calculate time based on distance and velocity alone */
         time_to_travel_distance = distance / velocity;
      }
   }

   return time_to_travel_distance;
}
