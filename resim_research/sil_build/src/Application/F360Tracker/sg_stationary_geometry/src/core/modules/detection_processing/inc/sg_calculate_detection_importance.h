/*=============================================================================================*\
* FILE: sg_calculate_detection_importance.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains an algorithm for calculating detection importance.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_CALCULATE_DETECTION_IMPORTANCE_H
#define SG_CALCULATE_DETECTION_IMPORTANCE_H
#include "sg_importance_calibrations.h"
#include "sg_input.h"

namespace sg
{
   float calculate_detection_importance(const float position_x,
                                        const float position_y,
                                        const float position_z,
                                        const uint16_t age,
                                        const uint16_t vertex_age,
                                        const float existence_probability,
                                        const float host_curvature,
                                        const float host_speed,
                                        const Importance_Calibrations_T &importance_calibrations);
}

#endif
