#ifndef F360_DETECTION_PRIORITY_H
#define F360_DETECTION_PRIORITY_H
/*===================================================================================\
 * FILE: f360_detection_priority.h
 *====================================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains function definition for detection_priority.
 *
 * Applicable Standards (in order of precedence: highest first):
 *   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *   ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*===================================================================================*/

#include "f360_reuse.h"

namespace f360_variant_A
{
   float32_t Detection_Priority(const float32_t range, const float32_t vcs_azimuth, const float32_t range_rate,
      const float32_t host_speed, const float32_t min_range_rate, const float32_t vun, const float32_t curvi_lat_pos);
}
#endif
