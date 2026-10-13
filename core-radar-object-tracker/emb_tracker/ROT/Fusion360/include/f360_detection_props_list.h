/*===================================================================================*\
 * FILE: f360_detection_props_list.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitive Business - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *  Detection properties container structure with sorted reference indices.
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
#ifndef F360_DETECTION_PROPS_LIST_H
#define F360_DETECTION_PROPS_LIST_H

#include "f360_constants.h"
#include "f360_detection_props.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
/******************************************************************************
 * Name:  F360_Detection_Props_List_T
 *   Structure containing a list of detections properties along with sorted reference detection indices.
 *
 * Struct Members:
 *   detection_props                - An array of detection properties.
 *   number_of_valid_detections     - Number of valid detections at in the detection array
 *   vcslong_det_idx_min            - Sorted vcs-long index of detection with most negative vcs-long position
 *   vcslong_det_idx_max            - Sorted vcs-long index of detection with most positive vcs-long position
 *   vcslong_sorted_ref_det_idx     - An array containing reference detection indicies sorted in vcs longitudinal order.
 ******************************************************************************/
typedef struct F360_Detection_Props_List_Tag
{
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS];
   int16_t vcslong_det_idx_min;
   int16_t vcslong_det_idx_max;
   int16_t vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS];
} F360_Detection_Props_List_T;

static_assert(((MAX_NUMBER_OF_DETECTIONS * sizeof(F360_Detection_Props_T) + 72) == sizeof(F360_Detection_Props_List_T)),
              "sizeof(F360_Detection_Props_List_T) not as expected. Remember to align padding if needed");

} // namespace f360_variant_A

#endif
