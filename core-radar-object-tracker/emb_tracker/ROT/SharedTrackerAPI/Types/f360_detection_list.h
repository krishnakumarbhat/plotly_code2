#ifndef F360_DETECTION_LIST_VARIANT_A_H
#define F360_DETECTION_LIST_VARIANT_A_H
/*===================================================================================*\
 * FILE: f360_detection_list.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *  Detection list container structure
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

#include "f360_constants.h"
#include "f360_detection.h"

namespace f360_variant_A
{
/******************************************************************************
 * Name:  F360_Detection_List_T
 *   Structure containing a list of detections
 *
 * Struct Members:
 *   detections                      - An array of detections.
 *   number_of_valid_detections      - Number of valid detections at in the detection array
 ******************************************************************************/
typedef struct F360_Detection_List_Tag
{
   F360_Detection_T detections[MAX_NUMBER_OF_DETECTIONS];
   uint32_t number_of_valid_detections;
} F360_Detection_List_T;

static_assert((MAX_NUMBER_OF_DETECTIONS * sizeof(F360_Detection_T) + 4) == sizeof(F360_Detection_List_T),
              "sizeof(F360_Detection_T) not as expected. Remember to align padding if needed");

} // namespace f360_variant_A

#endif
