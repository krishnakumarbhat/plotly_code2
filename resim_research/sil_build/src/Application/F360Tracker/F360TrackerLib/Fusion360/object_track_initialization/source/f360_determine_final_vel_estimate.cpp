/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_initial_detection_checks.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Determine_Final_Vel_Estimate()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_determine_final_vel_estimate.h"
#include "f360_math_func.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Determine_Final_Vel_Estimate()
   *===========================================================================
   * RETURN VALUE:
   * F360_Track_Init_T init_type
   *
   * PARAMETERS:
   * const float32_t posdiff_longvel,
   * const float32_t posdiff_latvel,
   * const float32_t cloud_longvel,
   * const float32_t cloud_latvel,
   * const CONF3_T posdiff_confidence,
   * CONF3_T cloud_confidence,
   * float32_t& longvel_estimate,
   * float32_t& latvel_estimate
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function calculates longvel_estimate and latvel_estimate and returns initialization type
   * based posdiff and cloud confidences and on adequately weighted posdiff and cloud estimates of longvel and latvel.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   F360_Track_Init_T Determine_Final_Vel_Estimate(
      const float32_t posdiff_longvel,
      const float32_t posdiff_latvel,
      const float32_t cloud_longvel,
      const float32_t cloud_latvel,
      const CONF3_T posdiff_confidence,
      CONF3_T cloud_confidence,
      float32_t& longvel_estimate,
      float32_t& latvel_estimate)
   {
      F360_Track_Init_T init_type = F360_TRACK_INIT_INVALID;
      float32_t cloud_weight = 0.0F;
      constexpr float32_t k_max_vel_diff = 2.5F;

      if ((cloud_confidence > CONF3_MED) && (posdiff_confidence >= CONF3_MED))
      {
         const float32_t min_speed = std::fminf(F360_Get_Hypotenuse(cloud_latvel, cloud_longvel), F360_Get_Hypotenuse(posdiff_latvel, posdiff_longvel));
         const float32_t speed_diff = F360_Get_Hypotenuse(cloud_latvel - posdiff_latvel, cloud_longvel - posdiff_longvel);
         const float32_t speed_th = std::fmaxf(k_max_vel_diff, 0.125F * min_speed);

         if (speed_diff > speed_th)
         {
            cloud_confidence = CONF3_MED;
         }
      }

      if (cloud_confidence == CONF3_HIGH)
      {
         if (posdiff_confidence == CONF3_HIGH)
         {
            init_type = F360_TRACK_INIT_WEIGHTED;
            cloud_weight = 0.5F;
         }
         else if (posdiff_confidence == CONF3_MED)
         {
            init_type = F360_TRACK_INIT_WEIGHTED;
            cloud_weight = 0.5F;
         }
         else
         {
            init_type = F360_TRACK_INIT_CLOUD;
            cloud_weight = 1.0F;
         }
      }
      else if (cloud_confidence == CONF3_MED)
      {
         if (posdiff_confidence == CONF3_HIGH)
         {
            init_type = F360_TRACK_INIT_POSDIFF;
            cloud_weight = 0.0F;
         }
         else if ((std::abs(cloud_longvel - posdiff_longvel) < k_max_vel_diff) &&
            (std::abs(cloud_latvel - posdiff_latvel) < k_max_vel_diff))
         {
            init_type = F360_TRACK_INIT_WEIGHTED;
            cloud_weight = 0.5F;
         }
         else
         {
            init_type = F360_TRACK_INIT_INVALID;
         }
      }
      else
      {
         if (posdiff_confidence == CONF3_HIGH)
         {
            init_type = F360_TRACK_INIT_POSDIFF;
            cloud_weight = 0.0F;
         }
         else
         {
            init_type = F360_TRACK_INIT_INVALID;
         }
      }

      longvel_estimate = cloud_weight * cloud_longvel + (1.0F - cloud_weight) * posdiff_longvel;
      latvel_estimate = cloud_weight * cloud_latvel + (1.0F - cloud_weight) * posdiff_latvel;

      return init_type;
   }
}
