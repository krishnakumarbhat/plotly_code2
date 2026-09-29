/*===================================================================================*\
* FILE: ocg_test_med_critical_zones.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Test_Med_Critical_Zones() function declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef TEST_MED_CRITICAL_ZONES_H
#define TEST_MED_CRITICAL_ZONES_H

#include "ocg_underdrivability.h"

namespace ocg
{
   void Test_Med_Critical_Zones(
     const OCG_Zones_Probabilities_T& probabilities,
     const float& n_meas_is_likely_to_pass,
     const uint32_t& zone_idx,
     const uint32_t& circ_buff_zone_idx,
     const OCG_Calibrations_T& calib,
     OCG_Cell_Classification& cell_classification
   );

}
#endif
