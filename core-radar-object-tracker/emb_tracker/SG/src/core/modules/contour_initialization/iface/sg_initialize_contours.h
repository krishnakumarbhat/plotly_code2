/*=============================================================================================*\
* FILE: sg_initialize_contours.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for initialize_contours.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_INITIALIZE_CONTOURS_H
#define SG_INITIALIZE_CONTOURS_H

#include "sg_common_calibrations.h"
#include "sg_contour_initialization_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"

namespace sg
{
/**
 * @brief   Initializes contours.
 *
 * @param[in]         contour_initialization_calibrations
 * @param[in]         common_calibrations
 * @param[in]         host_curvature_rear
 * @param[in, out]    detections
 * @param[in, out]    contours
 *
 * @return  N/A
 *=============================================================================================*/
void initialize_contours(const ContourInitializationCalibs &contour_initialization_calibrations,
                         const CommonCalibs &common_calibrations, const float host_curvature_rear, DetectionStorage &detections,
                         ContourStorage &contours);
} // namespace sg
#endif
