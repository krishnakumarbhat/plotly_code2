/*===================================================================================*\
* FILE: f360_object_status.h
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
*   This file contains enum F360_Object_Status declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): OBJ_STATUS.m
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
#ifndef F360_OBJECT_STATUS_H
#define F360_OBJECT_STATUS_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   typedef enum F360_Object_Status_Tag : uint8_t
   {
      F360_OBJECT_STATUS_INVALID = 0,
      F360_OBJECT_STATUS_NEW = 1,
      F360_OBJECT_STATUS_NEW_COASTED = 2,
      F360_OBJECT_STATUS_NEW_UPDATED = 3,
      F360_OBJECT_STATUS_UPDATED = 4,
      F360_OBJECT_STATUS_COASTED = 5
   }F360_Object_Status_T;
}
#endif
