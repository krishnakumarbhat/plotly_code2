/*===================================================================================*\
* FILE: f360_trk_fltr_ctca_states.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Trk_Fltr_CTCA_States_T enum declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_TRK_FLTR_CTCA_STATES_VARIANT_A_H
#define F360_TRK_FLTR_CTCA_STATES_VARIANT_A_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   typedef enum F360_Trk_Fltr_CTCA_States_Tag : uint8_t
   {
      F360_TRK_FLTR_CTCA_STATE_X = 0, // VCS position x
      F360_TRK_FLTR_CTCA_STATE_Y = 1, // VCS position y
      F360_TRK_FLTR_CTCA_STATE_H = 2, // VCS heading
      F360_TRK_FLTR_CTCA_STATE_C = 3, // curvature
      F360_TRK_FLTR_CTCA_STATE_S = 4, // Over the ground speed
      F360_TRK_FLTR_CTCA_STATE_A = 5 // Over the ground tangential acceleration (heading aligned)
   }F360_Trk_Fltr_CTCA_States_T;
}
#endif
