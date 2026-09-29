/*===================================================================================*\
* FILE: f360_define_simplified_dead_zone.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Define_Simplified_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_define_simplified_dead_zone.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Define_Simplified_Dead_Zone()
   *===========================================================================
   * RETURN VALUE:
   * Interval<float32_t> zone - zone representing longitudinal limits of dead zone defined by
   *                      sensors longitudinal positions
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - sensor calibrations
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
   * Function defines simplified dead zone - it is aligned to extreme sensors longitudinal
   * positions.
   *
   * PRECONDITIONS:
   * It is valid when we have all corner sensors.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Interval<float32_t> Define_Simplified_Dead_Zone(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      Interval<float32_t> zone;
      zone.upper = -INFTY;
      zone.lower = INFTY;

      for (uint32_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         if (sensors[i].variable.is_valid)
         {
            zone.upper = std::max(zone.upper, sensors[i].constant.mounting_position.vcs_position.longitudinal);
            zone.lower = std::min(zone.lower, sensors[i].constant.mounting_position.vcs_position.longitudinal);
         }
      }

      return zone;
   }
}
