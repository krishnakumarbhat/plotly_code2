#ifndef F360_DETECTION_MOTION_STATUS_VARIANT_A_H
#define F360_DETECTION_MOTION_STATUS_VARIANT_A_H
/*===================================================================================*\
 * FILE: f360_detection_motion_status.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *  Motion status classification enumeration for detections.
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *
 *   Requirements Document(s):
 *
 *   Applicable Standards (in order of precedence: highest first):
 *  - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *  - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#include "f360_reuse.h"

namespace f360_variant_A
{
/******************************************************************************
 * Name:  F360_Detection_Motion_Status_T
 *   Enumeration for detection motion status.
 ******************************************************************************/
typedef enum F360_Detection_Motion_Status_Tag : int8_t
{
   F360_DETECTION_MOTION_STATUS_INVALID    = -1,
   F360_DETECTION_MOTION_STATUS_STATIONARY = 0,
   F360_DETECTION_MOTION_STATUS_MOVING     = 1,
   F360_DETECTION_MOTION_STATUS_AMBIGUOUS  = 2
} F360_Detection_Motion_Status_T;

} // namespace f360_variant_A

#endif
