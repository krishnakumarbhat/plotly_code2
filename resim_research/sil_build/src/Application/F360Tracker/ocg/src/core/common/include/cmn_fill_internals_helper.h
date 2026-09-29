/*===================================================================================*\
* FILE: cmn_fill_internals_helper.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential  Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains internals stream definitions
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef FILL_INTERNALS_HELPER_h
#define FILL_INTERNALS_HELPER_h

#include "ocg_internals_type.h"
#include "ocg_underdrivability_states.h"

namespace ocg
{
    UD_Height_States_T Fill_Height_States(const float in_state[UD_HEIGHT_STATE_SIZE]);

    UD_RCS_States_T Fill_RCS_States(const float in_state[UD_RCS_STATE_SIZE]);

    void Fill_Height_Array(const UD_Height_States_T& in_state, float out_state[UD_HEIGHT_STATE_SIZE]);

    void Fill_RCS_Array(const UD_RCS_States_T& in_state, float out_state[UD_RCS_STATE_SIZE]);
}

#endif
