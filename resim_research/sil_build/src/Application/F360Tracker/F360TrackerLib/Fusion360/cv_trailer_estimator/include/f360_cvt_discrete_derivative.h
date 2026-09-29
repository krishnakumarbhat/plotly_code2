#ifndef F360_CVT_DISCRETE_DERIVATIVE_H
#define F360_CVT_DISCRETE_DERIVATIVE_H
/******************************************************************************
 * Copyright 2025 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include "f360_reuse.h"

namespace f360_variant_A
{
   // compute discrete time-derivative, given the value in previous scan
   void discrete_time_derivative(
      const float32_t u_now,
      float32_t& u_previous,
      float32_t& time_diff_u);
}
#endif 
