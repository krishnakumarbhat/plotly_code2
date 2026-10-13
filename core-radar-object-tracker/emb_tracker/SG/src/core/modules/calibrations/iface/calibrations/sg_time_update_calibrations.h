/*===========================================================================*\
* FILE: sg_time_update_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in time update step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_TIME_UPDATE_CALIBRATIONS_H
#define SG_TIME_UPDATE_CALIBRATIONS_H

#include "sg_time_update_calibrations_helpers.h"

namespace sg
{
struct TimeUpdateCalibs
{
   Process_Noise_T noise{};
};
} // namespace sg
#endif
