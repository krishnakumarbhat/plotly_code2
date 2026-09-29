#ifndef F360_OBJECT_SIDES_TYPE_H
#define F360_OBJECT_SIDES_TYPE_H
/*===================================================================================*\
* FILE: f360_object_sides.h
*====================================================================================
* Copyright 2019 Aptiv Technologies, Inc., All Rights Reserved.
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
*   This file contains enum F360_Object_Sides_Type_T declaration and enumerates the four different sides (front/right/rear/left) of an object.
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
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
#include "f360_reuse.h"

namespace f360_variant_A
{
   enum F360_Object_Sides_T : uint8_t
   {
      F360_OBJECT_SIDES_INVALID = 0U,
      F360_OBJECT_SIDES_FRONT = 1U,
      F360_OBJECT_SIDES_RIGHT = 2U,
      F360_OBJECT_SIDES_REAR = 3U,
      F360_OBJECT_SIDES_LEFT = 4U,
   };
}
#endif

