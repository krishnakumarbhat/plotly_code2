/*===========================================================================*\
* FILE: sg_time_update_calibrations_helpers.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains time update calibrations helpers.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_TIME_UPDATE_CALIBRATIONS_HELPERS_H
#define SG_TIME_UPDATE_CALIBRATIONS_HELPERS_H

namespace sg
{
struct Process_Noise_T
{
   float host_speed_weight_lat{};
   float host_speed_weight_lon{};
   float process_noise_base_std{};
   float host_speed_weight{};
};
} // namespace sg
#endif
