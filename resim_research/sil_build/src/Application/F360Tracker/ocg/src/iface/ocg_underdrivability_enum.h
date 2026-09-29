/*===================================================================================*\
* FILE: ocg_underdrivability_enum.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains OCG_Underdrivable_Status_T enum declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef UNDERDRIVABILITY_ENUM_H
#define UNDERDRIVABILITY_ENUM_H

namespace ocg
{
   enum OCG_Underdrivable_Status_T : uint8_t
   {
      UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER = (0), // Host can not pass under an object.
      UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER = (1), // Host is likely to pass under an object.
      UNDERDRIVABLE_STATUS_CAN_PASS_UNDER = (2), // Host can pass under an object.
      UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER = (3), // Track is not in area of interest/ is moving/ is invalid
      UNDERDRIVABLE_STATUS_TOTAL = (4) // Total number of status. This has always to be at the end of enum.
   };

   static_assert(UNDERDRIVABLE_STATUS_TOTAL == 4, "Wrong size of OCG_Underdrivable_Status_T");
}
#endif

