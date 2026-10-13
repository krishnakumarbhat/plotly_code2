/*===========================================================================*\
* FILE: sg_common_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains common calibrations used in various steps.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_COMMON_CALIBRATIONS_H
#define SG_COMMON_CALIBRATIONS_H

#include "geometry/geo_point.h"
#include "sg_common_calibrations_helpers.h"
#include "sg_reuse.h"

namespace sg
{
struct CommonCalibs
{
   View_Ranges_T view_ranges{};
   R_Position_Covariance_T r_position_covariance{};
   geometry::Point2D_T host_position{}; // [m] host position
   uint64_t elapsed_time_th{};          // [us] Expected time elapsed between two consecutive scans
   float min_segment_length{};
   float initial_reliability{};
   float azimuth_epsilon{};
};
} // namespace sg
#endif
