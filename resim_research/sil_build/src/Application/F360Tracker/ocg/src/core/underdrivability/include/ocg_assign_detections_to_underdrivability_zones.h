/*===================================================================================*\
* FILE: ocg_assign_detections_to_underdrivability_zones.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Assign_Detections_To_Underdrivability_Zones() function declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef ASSIGN_DETECTIONS_TO_UNDERDRIVABILITY_ZONES_H
#define ASSIGN_DETECTIONS_TO_UNDERDRIVABILITY_ZONES_H

#include "ocg_underdrivability.h"
#include "ocg_constants.h"

namespace ocg
{
    void Assign_Detections_To_Underdrivability_Zones(
        const OCG_Underdrivability_Internal_T &in_underdrivability,
        OCG_Zones_Innovation_T (&zones_innovation)[NUM_CELLS_X],
        const rspp_variant_A::RSPP_Detection_List_T &detection_list,
        const OCG_Calibrations_T &calib,
        const rspp_variant_A::F360_Radar_Sensor_T (&sensors)[rspp_variant_A::MAX_NUMBER_OF_SENSORS]);
}
#endif
