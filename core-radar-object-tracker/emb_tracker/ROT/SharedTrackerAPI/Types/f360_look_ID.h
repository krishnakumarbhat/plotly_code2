#ifndef F360_LOOK_ID_H
#define F360_LOOK_ID_H
/*===================================================================================*\
* FILE: f360_look_ID.h
*====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains enum  F360_Look_ID  declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s): LOOK_ID.m
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
 * Name:  F360_Det_Look_ID_T
 *   Enumeration for detection look ID.
 *
 * Enum Values:
 *   F360_DET_LOOK_ID_INVALID - Invalid look ID
 *   F360_DET_LOOK_ID_0       - Look ID 0 (LR, LL)
 *   F360_DET_LOOK_ID_1       - Look ID 1 (LR, ML)
 *   F360_DET_LOOK_ID_2       - Look ID 2 (MR, LL)
 *   F360_DET_LOOK_ID_3       - Look ID 3 (MR, ML)
 ******************************************************************************/
#ifdef __cplusplus
typedef enum F360_Det_Look_ID_Tag : int8_t
#else
typedef enum F360_Det_Look_ID_Tag
#endif
{
   F360_DET_LOOK_ID_INVALID = (-1),
   F360_DET_LOOK_ID_0       = (0),
   F360_DET_LOOK_ID_1       = (1),
   F360_DET_LOOK_ID_2       = (2),
   F360_DET_LOOK_ID_3       = (3),
   F360_DET_NUM_LOOK_ID     = (4)
} F360_Det_Look_ID_T;

#endif
