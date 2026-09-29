/*=============================================================================================*\
* FILE: sg_dissociate_detections.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the functions provided by the algorithm dissociate detections.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_DISSOCIATE_DETECTIONS_H
#define SG_DISSOCIATE_DETECTIONS_H

#include <bitset>

#include "sg_contour_storage.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief It resets the contour association for detection if the contour ID is not valid; otherwise, it resets the segment
    *association for detections.
    *
    * @param [in, out] detections: current detection storage
    * @param [in]      contours: current contour storage
    **/
   void dissociate_detections(sg::DetectionStorage &detections, const sg::ContourStorage &contours);
}

#endif