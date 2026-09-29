/*===================================================================================*\
* FILE: ocg_assign_underdrivability_status_to_zones.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Assign_Underdrivability_Status_To_Zones() function declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef ASSIGN_UNDERDRIVABILITY_STATUS_TO_ZONES_H
#define ASSIGN_UNDERDRIVABILITY_STATUS_TO_ZONES_H

#include "ocg_underdrivability.h"
namespace ocg
{
    void Assign_Underdrivability_Status_To_Zones(
        OCG_Underdrivability_Internal_T &in_underdrivability,
        const RSPP_Host_T &host,
        const OCG_Calibrations_T &calib);

}
#endif
