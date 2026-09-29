/*=============================================================================================*\
* FILE: sg_measurement_update_contours.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for measurement update contours function.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_MEASUREMENT_UPDATE_CONTOURS_H
#define SG_MEASUREMENT_UPDATE_CONTOURS_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief   Calculates the measurement update for contours.
    *
    * @param   contours
    * @param   detections
    * @param   calibrations
    * @param   min_segment_length
    *
    * @return  N/A
    **/
   void measurement_update_contours(ContourStorage &contours,
                                    DetectionStorage &detections,
                                    const Measurement_Update_Calibrations_T &calibrations,
                                    const float min_segment_length);
}
#endif
