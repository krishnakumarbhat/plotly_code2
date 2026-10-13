#ifndef F360_LOOK_TYPE_H
#define F360_LOOK_TYPE_H
/*===================================================================================*\
* FILE: f360_look_type.h
*====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains enum  F360_Look_Type  declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s): LOOK_TYPE.m
 *
 *   Requirements Document(s):
 *
 *  Applicable Standards (in order of precedence: highest first):
 *  - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *  - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#include "f360_reuse.h"

/******************************************************************************
 * Name:  F360_Det_Look_Type_T
 *   Enumeration for detection look type.
 ******************************************************************************/
typedef enum F360_Det_Look_Type_Tag : int8_t
{
   F360_DET_LOOK_TYPE_INVALID = -1,
   F360_DET_LOOK_TYPE_MEDIUM  = 0,
   F360_DET_LOOK_TYPE_LONG    = 1
} F360_Det_Look_Type_T;

#endif
