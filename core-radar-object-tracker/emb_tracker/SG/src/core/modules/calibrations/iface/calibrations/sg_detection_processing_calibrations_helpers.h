/*===========================================================================*\
* FILE: sg_detection_processing_calibrations_helpers.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains detection_processing calibrations helpers.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_DETECTION_PROCESSING_CALIBRATIONS_HELPERS_H
#define SG_DETECTION_PROCESSING_CALIBRATIONS_HELPERS_H

#include "sg_reuse.h"

namespace sg
{
struct DownselectInputDetectionsCalibs
{
   float max_rr_compens_for_stat_det{};             // [m/s] max range rate compensated for a stationary detection
   float accepted_probability_level{};              // [-] accepted probability level for a detection to be downselected [0-1]
   float max_host_speed_for_poor_azim_confid_det{}; // [m/s] maximum host speed to get detection with high azimuth
   float max_distance_for_poor_azim_confid_det{};   // [m] maximum detection distance with high azimuth
   int8_t maximum_valid_azimuth_confidence_value{}; // [-] maximum valid azimuth confidence
};
} // namespace sg
#endif
