/*===================================================================================*\
* FILE: f360_core_info.h
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
*   This file contains F360_Core_Info_T  structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): defineFusion360Types.m
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
#ifndef F360_CORE_INFO_VARIANT_A_H
#define F360_CORE_INFO_VARIANT_A_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   struct F360_Core_Info_T
   {
      uint64_t time_us;
      uint64_t prev_time_us;
      float32_t elapsed_time_s;
      uint32_t cnt_loops;
   };

   static_assert(24 == sizeof(F360_Core_Info_T), "sizeof(F360_Core_Info_T) not as expected. Remember to align padding if needed");
}

#endif
