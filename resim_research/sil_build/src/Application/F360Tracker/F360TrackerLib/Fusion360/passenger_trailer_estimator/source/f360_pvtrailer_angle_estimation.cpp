/*===========================================================================*\
 * FILE: f360_pvtrailer_angle_estimation.cpp
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include <algorithm>
#include <cstring>
#include "f360_pvtrailer_angle_estimation.h"
#include "f360_math.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: PVTrailer_Estimate_Angle()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function used to estimate trailer angle
   \*===========================================================================*/
   void PVTrailer_Estimate_Angle(
      const F360_Host_T& vehicle_data,
      const float32_t elapsed_time_s,
      const float32_t trailer_axle_length,
      F360_PVTrailer_Angle_Data_T& pvtrailer_angle)
   {
      float32_t m_trailer_axle_length = trailer_axle_length;
      const float32_t alpha = vehicle_data.vcs_sideslip;
      const float32_t k_host_length_compensator_from_rear_axle = 1.1F;
      const float32_t host_length = k_host_length_compensator_from_rear_axle * vehicle_data.dist_rear_axle_to_vcs_m;

      // set default m_trailer_axle_length
      if (m_trailer_axle_length <= 1.0F)
      {
         m_trailer_axle_length = 4.0F;
      }

      // estimation
      const int16_t HV_cnt_max = 20;
      if ((vehicle_data.speed >= 0.2F) && (fabsf(vehicle_data.yaw_rate_rad) <= 0.02F))
      {
         pvtrailer_angle.HV_cnt++;
         pvtrailer_angle.HV_cnt = std::min(pvtrailer_angle.HV_cnt, HV_cnt_max);
      }
      else
      {
         pvtrailer_angle.HV_cnt = 0;
      }

      if (pvtrailer_angle.HV_cnt == HV_cnt_max)
      {
         pvtrailer_angle.HV_start = true;
      }

      // reset HV_angle prev_trailer_angle
      if (vehicle_data.speed < 0.0F)
      {
         (void)memset(&pvtrailer_angle, 0, sizeof(pvtrailer_angle));
      }

      const float32_t prev_trailer_angle = pvtrailer_angle.trailer_angle_rad;
      if (pvtrailer_angle.HV_start)
      {
         const float32_t dist_rear_axle_to_rear_bumper = host_length - vehicle_data.dist_rear_axle_to_vcs_m;
         const float32_t ratio = dist_rear_axle_to_rear_bumper / vehicle_data.dist_rear_axle_to_vcs_m;
         const float32_t trailer_w = 1.0F / m_trailer_axle_length * std::abs(vehicle_data.speed) * (F360_Cosf(alpha) * F360_Sinf(pvtrailer_angle.trailer_angle_rad) - ratio * F360_Sinf(alpha) * F360_Cosf(pvtrailer_angle.trailer_angle_rad));
         pvtrailer_angle.trailer_angle_rad += vehicle_data.yaw_rate_rad * elapsed_time_s - trailer_w * elapsed_time_s;
      }

      pvtrailer_angle.trailer_angle_rate_rad = (pvtrailer_angle.trailer_angle_rad - prev_trailer_angle) / elapsed_time_s; // [rad/s]
   }
}
