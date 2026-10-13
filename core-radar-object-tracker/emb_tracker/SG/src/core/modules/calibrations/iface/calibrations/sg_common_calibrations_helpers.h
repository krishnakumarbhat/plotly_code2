/*===========================================================================*\
* FILE: sg_common_calibrations_helpers.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains common calibrations helpers.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_COMMON_CALIBRATIONS_HELPERS_H
#define SG_COMMON_CALIBRATIONS_HELPERS_H

namespace sg
{
struct R_Position_Covariance_T
{
   float x{};
   float y{};
   float xy{};
};

struct Interval_T
{
   float min{};
   float max{};
};

struct Vertical_T
{
   Interval_T overground{};  // [m] overground view range [min, max]
   Interval_T underground{}; //[m] underground view range [min, max]
};

struct View_Range_T
{
   Interval_T lateral{};      // [m] lateral view range [min, max]
   Interval_T longitudinal{}; // [m] longitudinal view range [min, max]
   Vertical_T vertical{};     // [m] vertical view range divided into overground and underground
};

struct View_Ranges_T
{
   View_Range_T nondrivable{};
   View_Range_T overdrivable{};
   View_Range_T underdrivable{};
};
} // namespace sg
#endif
