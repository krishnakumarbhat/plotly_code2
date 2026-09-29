/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#include <assert.h>

#include "ml_evaluate_normal_distribution.h"
#include "ml_exp.h"
#include "ml_math.h"


float Evaluate_Normal_Distribution(
   float value,
   float mean,
   float standard_deviation
)
{
   float exponent;
   const  float sqrt2pi = 2.506628274631f;
   float standard_deviation_local = standard_deviation;
   if (standard_deviation_local < THRESHOLD_IS_ZERO)
   {
      assert(standard_deviation_local != 0);
      standard_deviation_local = 1;
   }
   exponent = (value - mean) / standard_deviation_local;
   return ((1.0f / (sqrt2pi*standard_deviation_local))*Fast_Exp(-0.5f*exponent*exponent));
}
