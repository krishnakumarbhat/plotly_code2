#ifndef F360_RANGE_TYPE_H
#define F360_RANGE_TYPE_H
/*===================================================================================*\
 * FILE: f360_range_type.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains enum F360_Range_Type declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s): RANGE_TYPE.m
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

/******************************************************************************
 * Name:  F360_Det_Range_Type_T
 *   Enum for classification of detection range type.
 *
 * Enum Values:
 *   F360_DET_RANGE_TYPE_INVALID   - Invalid range type.
 *   F360_DET_RANGE_TYPE_MEDIUM    - Medium range detection.
 *   F360_DET_RANGE_TYPE_LONG      - Long range detection.
 ******************************************************************************/
typedef enum F360_Det_Range_Type_Tag : int8_t
{
   F360_DET_RANGE_TYPE_INVALID = -1,
   F360_DET_RANGE_TYPE_MEDIUM  = 0,
   F360_DET_RANGE_TYPE_LONG    = 1
} F360_Det_Range_Type_T;

#endif
