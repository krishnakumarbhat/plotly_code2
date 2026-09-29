/*===================================================================================*\
* FILE: f360_push_reduced_id.h
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
*   This file contains Push_Reduced_Id() declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): initStatObjDetProps.m
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
#ifndef F360_PUSH_REDUCED_ID_H
#define F360_PUSH_REDUCED_ID_H

#include "f360_reuse.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Push_Reduced_Id(
      const int32_t reduced_id,
      F360_Tracker_Info_T &tracker_info);
}
#endif
