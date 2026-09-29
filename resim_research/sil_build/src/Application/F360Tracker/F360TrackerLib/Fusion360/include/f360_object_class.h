/*===================================================================================*\
* FILE: f360_object_class.h
*====================================================================================
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: Susan Chen
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains enum F360_Object_Class declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): OBJ_CLASS.m
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
#ifndef F360_OBJ_CLASS_H
#define F360_OBJ_CLASS_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   typedef enum F360_Object_Class_Tag : uint8_t
   {
      F360_OBJ_CLASS_UNDETERMINED = 0,
      F360_OBJ_CLASS_CAR = 1,
      F360_OBJ_CLASS_MOTORCYCLE = 2,
      F360_OBJ_CLASS_TRUCK = 3,
      F360_OBJ_CLASS_PEDESTRIAN = 4,
      F360_OBJ_CLASS_POLE = 5,
      F360_OBJ_CLASS_TREE = 6,
      F360_OBJ_CLASS_ANIMAL = 7,
      F360_OBJ_CLASS_GOD = 8,
      F360_OBJ_CLASS_BICYCLE = 9,
      F360_OBJ_CLASS_UNIDENTIFIED_VEHICLE = 10
   }F360_Object_Class_T;
}
#endif
