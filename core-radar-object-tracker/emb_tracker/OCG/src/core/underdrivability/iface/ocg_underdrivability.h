/*===================================================================================*\
* FILE: ocg_underdrivability.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Underdrivability() function declaration
*
*   Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef UNDERDRIVABILITY_H
#define UNDERDRIVABILITY_H

#include "ocg_calibrations.h"
#include "ocg_occupancy_grid_types.h"
#include "ocg_underdrivability_type.h"

namespace ocg
{
void Underdrivability(const OCG_Detection_List_T &detection_list, const OCG_Sensor_T &sensor, const OCG_Host_T &host,
                      const OCG_Calibrations_T &calibrations, OCG_Detection_Properties_T (&det_props)[OCG_NUM_DETS],
                      OCG_Underdrivability_Internal_T &in_underdrivability);
}
#endif
