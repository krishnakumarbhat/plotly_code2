/*=============================================================================================*\
* FILE: sg_importance_calibration.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains a class with importance calibration.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "sg_importance_calibrations.h"

#include <cassert>

namespace sg
{
   void Importance_Calibrations_T::adjust_distance_factors(const float host_speed)
   {
      const float scale = 1.0F + host_speed_impact_factor * std::fabs(host_speed);
      assert(scale >= 1.0F);
      distance_factors[0U] /= scale;
      distance_factors[1U] *= scale;
   }
}