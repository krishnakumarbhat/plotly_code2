#ifndef F360_POSITION_H
#define F360_POSITION_H
/*===================================================================================*\
* FILE: f360_position.h
*====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains F360_Position  structure declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s): TypesFusion360.h
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
 * Name:  F360_VCS_Position_T
 *  Structure for vehicle coordinate system (VCS) position of radar sensor.
 *
 * Struct Members:
 *   longitudinal - [m] Longitudinal mounting position of the sensor in VCS.
 *   lateral      - [m] Lateral mounting position of the sensor in VCS.
 *   height       - [m] Height above ground mounting position of the sensor.
 ******************************************************************************/
typedef struct F360_VCS_Position_Tag
{
   float32_t longitudinal;
   float32_t lateral;
   float32_t height;
} F360_VCS_Position_T;

#endif
