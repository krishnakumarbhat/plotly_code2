/*===========================================================================*\
* FILE: sg_detection_processing_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in detection_processing step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_DETECTION_PROCESSING_CALIBRATIONS_H
#define SG_DETECTION_PROCESSING_CALIBRATIONS_H

#include "sg_detection_processing_calibrations_helpers.h"
#include "sg_importance_calibrations.h"

namespace sg
{
struct DetectionProcessingCalibs
{
   DownselectInputDetectionsCalibs downselect_input_detections{};
   ImportanceCalibs importance_calibrations{};
};
} // namespace sg
#endif
