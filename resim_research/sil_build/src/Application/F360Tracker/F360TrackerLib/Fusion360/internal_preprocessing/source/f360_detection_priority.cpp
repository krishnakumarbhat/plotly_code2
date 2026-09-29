/*===================================================================================\
 * FILE: f360_detection_priority.cpp
 *====================================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains function definition for detection_priority.
 *
 * Applicable Standards (in order of precedence: highest first):
 *   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *   ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*===================================================================================*/

#include "f360_math.h"
#include "f360_constants.h"
#include "f360_detection_priority.h"
#include "rspp_detection_motion_status.h"

namespace f360_variant_A
{
   static float32_t Polar_Position_Priority(const float32_t range, const float32_t vcs_azimuth, const float32_t curvi_lat_pos);
   static float32_t Motion_Priority(const float32_t range_rate, const float32_t vcs_azimuth, 
      const float32_t host_speed, const float32_t min_range_rate, const float32_t vun);

   float32_t Detection_Priority(const float32_t range, const float32_t vcs_azimuth, const float32_t range_rate,
      const float32_t host_speed, const float32_t min_range_rate, const float32_t vun, const float32_t curvi_lat_pos)
   {
      // Overall most important detections: 2
      // Overall Least important detections: -2

      // abs azimuth min, azimuth max
      constexpr float32_t exclude_zone_az[2] = {F360_DEG2RAD(57.0F), F360_DEG2RAD(138.0F)};
      constexpr float32_t exclude_zone_rng = 100.0F;
      float32_t priority_sum = 0.0F;

      const float32_t position_prio = Polar_Position_Priority(range, vcs_azimuth, curvi_lat_pos);
      const float32_t motion_prio = Motion_Priority(range_rate, vcs_azimuth, host_speed, min_range_rate, vun);
      // by this line, priority_sum is in [0, 2]
      priority_sum = position_prio + motion_prio;
      
      // if a det falls into the less interesting zone, then its priority sum is moved into [-2, 0] but the relative priority
      // between less interesting detections are kept.
      if ((exclude_zone_rng < range)
         && ((exclude_zone_az[0] < fabsf(vcs_azimuth)) && (fabsf(vcs_azimuth) < exclude_zone_az[1])))
      {
         priority_sum -= 2.0F;
      }

      return priority_sum;
   }

   static float32_t Polar_Position_Priority(const float32_t range, const float32_t vcs_azimuth, const float32_t curvi_lat_pos)
   {
      constexpr float32_t k_az_scale = 1.2F;
      constexpr float32_t k_range_scale = 0.0001F;

      float32_t curvi_lat_component = 0.0F;
      // 5 meter curvi lateral when range is 200 meters for rear objects only
      const float32_t k_curvi_lat_thres = range * 0.025F;
      constexpr float32_t k_vcs_az_thres = F360_DEG2RAD(90.0F);
      if ((fabsf(curvi_lat_pos) < k_curvi_lat_thres) && (k_vcs_az_thres < fabsf(vcs_azimuth)))
      {
         curvi_lat_component = 0.8F;
      }
      const float32_t az_component = 1.0F - (k_az_scale * vcs_azimuth * vcs_azimuth); // may be negative
      const float32_t range_component = fmaxf(0.0F, 1.0F - (k_range_scale * range * range)); // must be positive

      float32_t priority = fmaxf(az_component, range_component); // limited to range [0, 1]
      priority = fmaxf(priority, curvi_lat_component);
      return priority;
   }

   static float32_t Motion_Priority(const float32_t range_rate, const  float32_t vcs_azimuth, 
      const float32_t host_speed, const float32_t min_range_rate, const float32_t vun)
   {
      constexpr float32_t k_motion_threshold_inv = 0.2F;

      const float32_t predicted_stationary_range_rate = -host_speed * F360_Cosf(vcs_azimuth);
      const float32_t temp = fmodf(predicted_stationary_range_rate - min_range_rate, vun);
      float32_t predicted_stationary_range_rate_aliased;
      if (temp < 0.0F)
      {
         // fmodf() does not behave like MATLAB mod, so need to add vun if result is negative
         predicted_stationary_range_rate_aliased = temp + vun + min_range_rate;
      }
      else
      {
         predicted_stationary_range_rate_aliased = temp + min_range_rate;
      }
      
      const float32_t priority = fminf(1.0F, fabsf(predicted_stationary_range_rate_aliased - range_rate) * k_motion_threshold_inv); // limited to range [0, 1]
      return priority;
   }
}
