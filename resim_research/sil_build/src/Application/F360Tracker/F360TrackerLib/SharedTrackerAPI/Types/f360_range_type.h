/*===================================================================================*\
* FILE: f360_range_type.h
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
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_RANGE_TYPE_H
#define F360_RANGE_TYPE_H

#include "f360_reuse.h"

typedef enum F360_Det_Range_Type_Tag : int8_t
{
   F360_DET_RANGE_TYPE_INVALID = -1,
   F360_DET_RANGE_TYPE_MEDIUM = 0,
   F360_DET_RANGE_TYPE_LONG = 1
} F360_Det_Range_Type_T;

#endif
