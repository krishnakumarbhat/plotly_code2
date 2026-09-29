/*===================================================================================*\
* FILE: f360_e2e_protection.h
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This is the main function for the vehicle processing module.
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*
*
* DEVIATIONS FROM STANDARDS:
*
*
\*==========================================================================================*/

#ifndef E2E_PROTECTION_H
#define E2E_PROTECTION_H

#include "f360_timing_info.h"
namespace f360_variant_A
{
   void E2E_Protection(
      F360_TRKR_TIMING_INFO_T & timing_info
   );
}

#endif
