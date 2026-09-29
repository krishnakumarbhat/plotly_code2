/*=============================================================================================*\
* FILE: sg_squeeze_detections.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for squeeze_detections and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_squeeze_detections.h"

#include <cmath>
#include <limits>

namespace sg
{
   void squeeze_detections(DetectionStorage &detections, const Linear_Piecewise_Transform_Coefficients_T &coefficients)
   {
      const auto detections_dummy_tail = detections.end();
      for (auto detection_it = detections.begin(); detection_it != detections_dummy_tail; ++detection_it)
      {
         auto &detection = *detection_it;
         if (detection.drivability == SG_Drivability_Class_T::UNDERDRIVABLE)
         {
            detection.position_squeezed.x = transform_x(detection, coefficients.longitudinal.underdrivable);
            detection.position_squeezed.y = transform_y(detection, coefficients.lateral.underdrivable);
         }
         else
         {
            detection.position_squeezed.x = transform_x(detection, coefficients.longitudinal.nondrivable);
            detection.position_squeezed.y = transform_y(detection, coefficients.lateral.nondrivable);
         }
      }
   }

   float transform_x(const Detection_T &detection, const Intervals_T &intervals)
   {
      return piecewise_linear_scaling(detection.position.x, intervals);
   }

   float transform_y(const Detection_T &detection, const Intervals_T &intervals)
   {
      return piecewise_linear_scaling(detection.position.y, intervals);
   }

   // Returns interval number the input value is within. Input value has to be provided as abs(value).
   std::size_t get_pos_corresp_interval_idx(const float value, const Intervals_T &intervals)
   {
      std::size_t idx = 0U;

      // Input value equal or lower than intervals highest limit
      if (value <= intervals.back().range)
      {
         float low  = 0.0F;
         float high = 0.0F;

         for (; idx < intervals.size(); ++idx)
         {
            high                  = intervals[idx].range;
            const bool f_in_range = in_range(value, low, high);

            if (f_in_range)
            {
               break;
            }

            low = high;
         }
         (void) low;  // MISRA
         (void) high; // MISRA
      }
      else // Input value higher than highest intervals limit
      {
         idx = intervals.size() - 1U;
      }

      return idx;
   }


   float piecewise_linear_scaling(const float position, const Intervals_T &intervals)
   {
      float scaled_position = 0.0F;
      if (std::fabs(position) >= std::numeric_limits<float>::epsilon())
      {
         const float position_abs                   = std::abs(position);
         const std::size_t pos_corresp_interval_idx = get_pos_corresp_interval_idx(position_abs, intervals);

         float max_prev_interval_value     = 0.0F;
         float min_max_interval_value_diff = 0.0F;
         float prev_interval_limit         = 0.0F;

         for (std::size_t idx = 0U; idx < pos_corresp_interval_idx; ++idx)
         {
            min_max_interval_value_diff = (intervals[idx].range - prev_interval_limit) * intervals[idx].factor;
            max_prev_interval_value += min_max_interval_value_diff;
            prev_interval_limit = intervals[idx].range;
         }
         (void) min_max_interval_value_diff; // MISRA

         scaled_position = max_prev_interval_value + (position_abs - prev_interval_limit) * intervals[pos_corresp_interval_idx].factor;

         if (position < 0.0F)
         {
            scaled_position *= -1.0F;
         }
      }

      return scaled_position;
   }
}
