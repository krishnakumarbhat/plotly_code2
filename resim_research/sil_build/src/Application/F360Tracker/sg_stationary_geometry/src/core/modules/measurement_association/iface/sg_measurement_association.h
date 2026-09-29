/*=============================================================================================*\
* FILE: sg_measurement_association.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the declaration of measurement_association function.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_MEASUREMENT_ASSOCIATION_H
#define SG_MEASUREMENT_ASSOCIATION_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief    Measurement association.
    *
    * @param    contours
    * @param    detections
    * @param    measurement_association_calibrations
    * @param    common_calibrations
    *
    **/
   void measurement_association(const ContourStorage &contours,
                                const DetectionStorage &detections,
                                const Measurement_Association_Calibrations_T &measurement_association_calibrations,
                                const Common_Calibrations_T &common_calibrations);
}

#endif