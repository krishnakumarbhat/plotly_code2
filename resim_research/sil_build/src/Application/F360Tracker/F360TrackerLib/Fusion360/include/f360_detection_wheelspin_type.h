/*===================================================================================*\
* FILE: f360_detection_wheelspin_type.h
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
*   This file contains enum F360_Detection_Wheelspin_Type_T declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): DET_WHEELSPIN_TYPE.m
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
#ifndef F360_DETECTION_WHEELSPIN_TYPE_H
#define F360_DETECTION_WHEELSPIN_TYPE_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   typedef enum F360_Detection_Wheelspin_Type_Tag : uint8_t
   {
      F360_DETECTION_WHEELSPIN_TYPE_INVALID = 0,
      F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS = 1,
      F360_DETECTION_WHEELSPIN_TYPE_OBJECT = 2,
      F360_DETECTION_WHEELSPIN_TYPE_NEARBY = 3
   }F360_Detection_Wheelspin_Type_T;
}
#endif
