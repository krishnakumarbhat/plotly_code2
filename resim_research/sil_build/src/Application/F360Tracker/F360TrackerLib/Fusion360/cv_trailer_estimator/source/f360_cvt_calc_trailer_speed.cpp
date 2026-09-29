/******************************************************************************
 * Copyright 2025 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include "f360_cvt_calc_trailer_speed.h"

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
      float32_t& joint_lon_vel)
   {
      // the ratio between the distances of front wheel to rear wheel axle and the rear wheel axle to the joint
      // To be used to compute the joint lateral velocity, assuming front driven and same angular speed
      const float32_t joint_fwhl_r_ratio = a1 / b1;

      // rotational motion kinematics to map the host front wheel speed onto the  lateral speed of joint, w.r.t. the host body
      const float32_t lat_vel_proj = sin_angle * joint_fwhl_r_ratio * sin_sideslip;

      // longitudinal speed of the joint, along the host body
      const float32_t lon_vel_proj = cos_angle * cos_sideslip;

      // Sum of the projection from lateral velocity and longitudinal velocity of the joint
      // "-" subtraction because the side slip angle also follows VCS frame convention, i.e., positive steering right and negative left.
      joint_lon_vel = (lon_vel_proj - lat_vel_proj) * host_speed;
   }
}
