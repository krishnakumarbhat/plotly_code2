/*===================================================================================*\
* FILE: ocg_shift_circular_zones.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Shift_Circular_Zones() function declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef HANDLE_CIRCULAR_BUFFER_IDX_H
#define HANDLE_CIRCULAR_BUFFER_IDX_H

#include "ocg_underdrivability.h"

namespace ocg
{
    void Shift_Circular_Zones(
        OCG_Underdrivability_Internal_T &in_underdrivability,
        const RSPP_Host_T host);
}
#endif
