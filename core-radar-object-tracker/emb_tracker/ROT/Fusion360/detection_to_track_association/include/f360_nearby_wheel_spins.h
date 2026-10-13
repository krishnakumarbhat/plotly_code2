/*===========================================================================*\
* FILE: f360_nearby_wheel_spins.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Detect_Near_By_Wheel_Spins() declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_NEAR_BY_WHEEL_SPINS_H
#define F360_NEAR_BY_WHEEL_SPINS_H

#include "f360_calibrations.h"
#include "f360_detection_props_list.h"

namespace f360_variant_A
{
void Detect_Nearby_Wheel_Spins(const F360_Calibrations_T &calibrations, const uint32_t number_of_valid_detections,
                               F360_Detection_Props_List_T &detection_props_list);
}
#endif
