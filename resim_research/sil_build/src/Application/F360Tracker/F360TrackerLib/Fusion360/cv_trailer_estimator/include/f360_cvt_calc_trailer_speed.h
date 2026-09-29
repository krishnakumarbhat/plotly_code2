#ifndef F360_CVT_CALC_TRAILER_SPEED_H
#define F360_CVT_CALC_TRAILER_SPEED_H
/******************************************************************************
 * Copyright 2025 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include "f360_reuse.h"

namespace f360_variant_A
{
   void calculate_trailer_longitudinal_velocity(
      const float32_t cos_angle,
      const float32_t sin_angle,
      const float32_t a1,
      const float32_t host_speed,
      const float32_t cos_sideslip,
      const float32_t sin_sideslip,
      const float32_t b1,
      float32_t& joint_lon_vel);
}
#endif
