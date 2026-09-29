/*=============================================================================================*\
* FILE: sg_calculate_detection_importance.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains an algorithm for calculating detection importance.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "sg_calculate_detection_importance.h"

namespace sg
{
   float calculate_detection_importance(const float position_x,
                                        const float position_y,
                                        const float position_z,
                                        const uint16_t age,
                                        const uint16_t vertex_age,
                                        const float existence_probability,
                                        const float host_curvature,
                                        const float host_speed,
                                        const Importance_Calibrations_T &importance_calibrations)
   {
      float adjusted_position_y = position_y;
      float importance          = 1.0F;

      if (vertex_age > importance_calibrations.max_associated_det_vertex_age)
      {
         importance = 0.0F;
      }
      else
      {
         // taking into account host curvature - using approximate formula as described in FZD-657
         if (std::fabs(host_curvature) > 1e-5F)
         {
            adjusted_position_y = adjusted_position_y - 0.5F * position_x * position_x * host_curvature;
         }

         // adjust detection position
         float adjusted_position_x        = position_x - host_speed;
         const float amplification_factor = adjusted_position_x * host_speed;
         if (amplification_factor <= 0.0F)
         {
            adjusted_position_x *= (1.0F - importance_calibrations.shape_correction_factors[0U] * amplification_factor);
         }
         else
         {
            adjusted_position_y /= (1.0F + importance_calibrations.shape_correction_factors[1U] * amplification_factor);
         }

         // calculate Manhattan distance using adjusted positions
         const float adjusted_distance_to_host = importance_calibrations.distance_factors[0U] * std::fabs(adjusted_position_x)
                                                 + importance_calibrations.distance_factors[1U] * std::fabs(adjusted_position_y)
                                                 + importance_calibrations.distance_factors[2U] * std::fabs(position_z);
         if (importance_calibrations.use_existence_probability)
         {
            importance = existence_probability;
         }

         importance /= (1.0F + importance_calibrations.age_impact_factor * static_cast<float>(age))
                       * (1.0F + importance_calibrations.distance_impact_factor * adjusted_distance_to_host);
      }
      return importance;
   }
}
