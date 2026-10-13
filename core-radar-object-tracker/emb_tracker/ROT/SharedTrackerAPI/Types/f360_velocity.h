#ifndef F360_VELOCITY_H
#define F360_VELOCITY_H
/*===================================================================================*\
 * FILE: f360_velocity.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains F360_Velocity_T structure declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s): TypesFusion360.h
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
 * Name:  F360_VCS_Velocity_T
 *   Structure for Vehicle Coordinate System velocity.
 *
 * Struct Members:
 *   longitudinal - [m/s] Longitudinal velocity component in VCS.
 *   lateral      - [m/s] Lateral velocity component in VCS.
 ******************************************************************************/
typedef struct F360_VCS_Velocity_Tag
{
   float32_t longitudinal;
   float32_t lateral;
} F360_VCS_Velocity_T;

#endif
