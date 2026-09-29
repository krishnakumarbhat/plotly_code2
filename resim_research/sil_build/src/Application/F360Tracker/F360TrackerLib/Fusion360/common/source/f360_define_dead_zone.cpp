/*===================================================================================*\
* FILE: f360_define_dead_zone.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Define_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_define_dead_zone.h"
#include "f360_define_simplified_dead_zone.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Define_Dead_zone()
   *===========================================================================
   * RETURN VALUE:
   * Dead_Zone_T zone - zone representing longitudinal limits of dead zone defined by
   *                    sensors longitudinal positions
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - sensor calibrations,
   * const float32_t dead_zone_long_limit_extension - longitudinal length of extended dead zone
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
   * Function defines dead zone aligned to extreme sensors longitudinal positions
   *
   * PRECONDITIONS:
   * It is valid when we have all corner sensors.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Dead_Zone_T Define_Dead_Zone(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t dead_zone_long_limit_extension)
   {
      Dead_Zone_T dead_zone;
      dead_zone.basic = Define_Simplified_Dead_Zone(sensors);

      dead_zone.extended.upper = dead_zone.basic.upper + dead_zone_long_limit_extension;
      dead_zone.extended.lower = dead_zone.basic.lower - dead_zone_long_limit_extension;

      return dead_zone;
   }
}
