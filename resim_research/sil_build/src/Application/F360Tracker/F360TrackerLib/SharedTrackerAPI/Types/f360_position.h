/*===================================================================================*\
* FILE: f360_position.h
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
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_POSITION_H
#define F360_POSITION_H

#include "f360_reuse.h"

typedef struct F360_VCS_Position_Tag
{
   float32_t longitudinal; // [m] Longitudinal mounting position of the sensor in VCS.
   float32_t lateral;      // [m] Lateral mounting position of the sensor in VCS.
   float32_t height;       // [m] Height above ground mounting position of the sensor.
} F360_VCS_Position_T;


#endif

