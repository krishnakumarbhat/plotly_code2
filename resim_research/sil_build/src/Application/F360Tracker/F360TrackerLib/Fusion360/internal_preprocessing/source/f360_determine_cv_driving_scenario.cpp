/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_determine_driving_scenario.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Determine_CV_Driving_Scenario()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_determine_cv_driving_scenario.h"
#include "f360_math.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Determine_CV_Driving_Scenario
   *===========================================================================
   * RETURN VALUE:
   * bool highway_suspected
   *
   * PARAMETERS:
   * const F360_Host_T& host
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
   * Function determines, for commercial vehicles only, if the host is driving
   * with significant speed without significant speed/yaw_rate change between
   * 5 seconds time frames, which might indicate highway drive scenario.
   *
   * PRECONDITIONS:
   * host.host_type == F360_HOST_TYPE_COMMERCIAL_VEHICLE
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool highway_suspected = false;
   static float32_t speed_buffer = 0.0F;
   static uint8_t index = 0U;

   bool Determine_CV_Driving_Scenario(const F360_Host_T& host)
   {
      if (host.host_type == F360_HOST_TYPE_COMMERCIAL_VEHICLE)
      {
         speed_buffer += host.vcs_speed;
         if (index >= 99U)
         {
            //buffer filled calc the mean
            const float32_t speed_buffer_mean = 0.01F * speed_buffer;
            //current host speed 10% above the mean from last 100 scans?
            const float32_t speed_deviation = std::abs(speed_buffer_mean - host.vcs_speed);
            if ((host.vcs_speed >= 9.5F) && (speed_deviation <= 0.1F * speed_buffer_mean))
            {
               highway_suspected = true;
            }
            else 
            {
               highway_suspected = false;
            }
            index = 0U;
            speed_buffer = 0.0F;
         }
         else
         {
            ++index;
         }
      }
      else
      {
         highway_suspected = false;
      }
      return highway_suspected;
   }

   /*===========================================================================*\
   * FUNCTION: Reset_Determine_CV_Driving_Scenario_Variables
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
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
   * Helper function intended for resetting the state of initialized static variables
   * in Determine_CV_Driving_Scenario().
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Reset_Determine_CV_Driving_Scenario_Variables()
   {
      highway_suspected = false;
      speed_buffer = 0.0F;
      index = 0U;
   }
}
