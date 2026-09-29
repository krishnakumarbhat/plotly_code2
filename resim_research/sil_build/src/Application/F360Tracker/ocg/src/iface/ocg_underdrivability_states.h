/*===================================================================================*\
* FILE: ocg_underdrivability_states.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Underdrivability_Height_State_T and Underdrivability_RCS_State_T enum declarations
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef UNDERDRIVABILITY_STATES_H
#define UNDERDRIVABILITY_STATES_H

#include "ocg_reuse.h"

namespace ocg
{
   enum Underdrivability_Height_State_T : uint8_t
   {
      UD_HEIGHT_STATE_DET_COUNT = 0U,
      UD_HEIGHT_STATE_ELEVATION_ANGLE = 1U,
      UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE = 2U,
      UD_HEIGHT_STATE_SIZE = 3U
   };

   enum Underdrivability_RCS_State_T : uint8_t
   {
      UD_RCS_STATE_DET_COUNT = 0U,
      UD_RCS_STATE_RANGE = 1U,
      UD_RCS_STATE_SQUARED_RANGE = 2U,
      UD_RCS_STATE_RCS = 3U,
      UD_RCS_STATE_SQUARED_RCS = 4U,
      UD_RCS_STATE_RANGE_RCS_PROD = 5U,
      UD_RCS_STATE_SIZE = 6U
   };
}

#endif
