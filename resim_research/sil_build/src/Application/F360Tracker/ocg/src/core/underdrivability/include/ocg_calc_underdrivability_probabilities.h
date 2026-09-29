/*===================================================================================*\
* FILE: ocg_calc_underdrivability_probabilities.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Calc_Underdrivability_Probabilities() function declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef CALC_UNDERDRIVABILITY_PROBABILITIES_H
#define CALC_UNDERDRIVABILITY_PROBABILITIES_H

#include "ocg_underdrivability.h"
namespace ocg
{
   void Calc_Underdrivability_Probabilities(
      OCG_Underdrivability_Internal_T& in_underdrivability,
      OCG_Zones_Innovation_T(&zones_innovation)[NUM_CELLS_X],
      const OCG_Calibrations_T& calib);

}
#endif
