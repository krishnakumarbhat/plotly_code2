/*===================================================================================*\
* FILE: f360_velocity.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
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
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_VELOCITY_H
#define F360_VELOCITY_H

#include "f360_reuse.h"

typedef struct F360_VCS_Velocity_Tag
{
   float32_t longitudinal; // [m/s] Longitudinal velocity component in VCS.
   float32_t lateral;      // [m/s] Lateral velocity component in VCS.
} F360_VCS_Velocity_T;

#endif
