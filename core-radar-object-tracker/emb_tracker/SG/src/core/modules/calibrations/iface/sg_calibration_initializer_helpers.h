/*===================================================================================*\
* FILE: sg_calibration_initializer_helpers.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains CalibrationInitializer helpers.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CALIBRATION_INITIALIZER_HELPERS_H
#define SG_CALIBRATION_INITIALIZER_HELPERS_H

#include <cstdint>

namespace sg
{
enum class Scenario : uint8_t
{
   PARKING = 1U,
   CITY    = 2U,
   HIGHWAY = 3U
};

/**
 * @brief         Returns calibration value based on speed_scenario.
 *
 * @param[in]           speed_scenario - enum [PARKING, CITY, HIGHWAY]
 * @param[in]           parking_value - value returned for PARKING
 * @param[in]           city_value - value returned for CITY
 * @param[in]           highway_value - value returned for HIGHWAY
 *
 * @return              calibration value
 **/
template <typename T>
T set_calibration(const Scenario speed_scenario, const T parking_value, const T city_value, const T highway_value)
{
   T result;

   switch (speed_scenario)
   {
      case sg::Scenario::PARKING:
         result = parking_value;
         break;
      case sg::Scenario::CITY:
         result = city_value;
         break;
      case sg::Scenario::HIGHWAY:
         result = highway_value;
         break;
      default:
         result = parking_value;
         break;
   }

   return result;
}
} // namespace sg
#endif
