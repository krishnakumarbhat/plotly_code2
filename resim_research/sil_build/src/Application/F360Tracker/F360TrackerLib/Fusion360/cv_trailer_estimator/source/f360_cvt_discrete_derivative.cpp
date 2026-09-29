/******************************************************************************
 * Copyright 2025 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include "f360_cvt_discrete_derivative.h"

namespace f360_variant_A
{
   // compute discrete time-derivative, given the value in previous scan
   void discrete_time_derivative(const float32_t u_now,
                                 float32_t &u_previous,
                                 float32_t &time_diff_u)
   {
      const float32_t discrete_difference = u_now - u_previous;

      // Compute discrete time-derivative w.r.t. previous scan
      time_diff_u = discrete_difference / 0.05F;

      // Update the previous u for the next scan
      u_previous = u_now;
   }

}
