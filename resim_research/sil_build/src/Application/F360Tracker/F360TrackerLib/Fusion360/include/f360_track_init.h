/*===================================================================================*\
* FILE: F360_track_init.h
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
*   This file contains F360_Track_Init_Tag enum declaration
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
#ifndef F360_TRACK_INIT_H
#define F360_TRACK_INIT_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   enum F360_Track_Init_T : int8_t
   {
      F360_TRACK_INIT_INVALID = -1,
      F360_TRACK_INIT_STATIONARY = 0,
      F360_TRACK_INIT_CLOUD = 1,
      F360_TRACK_INIT_POSDIFF = 2,
      F360_TRACK_INIT_WEIGHTED = 3
   };
}
#endif
