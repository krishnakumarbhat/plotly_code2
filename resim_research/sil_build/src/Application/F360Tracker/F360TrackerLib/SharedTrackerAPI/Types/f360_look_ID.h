/*===================================================================================*\
* FILE: f360_look_ID.h
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
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_LOOK_ID_H
#define F360_LOOK_ID_H

#include "f360_reuse.h"

typedef enum F360_Det_Look_ID_Tag : int8_t
{
   F360_DET_LOOK_ID_INVALID = (-1),
   F360_DET_LOOK_ID_0 = (0), // LR, LL
   F360_DET_LOOK_ID_1 = (1), // LR, ML
   F360_DET_LOOK_ID_2 = (2), // MR, LL
   F360_DET_LOOK_ID_3 = (3), // MR, ML
   F360_DET_NUM_LOOK_ID = (4)
} F360_Det_Look_ID_T;

#endif
