/*===================================================================================*\
* FILE: ocg_calc_underdrivability_probabilities_helpers.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Calc_Underdrivability_Probabilities() helper functions declarations
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef CALC_UNDERDRIVABILITY_PROBABILITIES_HELPERS_H
#define CALC_UNDERDRIVABILITY_PROBABILITIES_HELPERS_H

#include "ocg_underdrivability.h"
namespace ocg
{
   float Students_T_Betainc_Approx(
     const OCG_Calibrations_T& calib,
     const float sample_value,
     const float dof);

   float Students_T_CDF_Approx(
     const OCG_Calibrations_T& calib,
     const float sample_value,
     const float dof);

   float Compute_Forgetting_Factor(
     const OCG_Calibrations_T& calib,
     const uint32_t zone_idx);

   float Compute_Prob_Of_Height_Hypothesis(
     const OCG_Calibrations_T& calib,
     const float(&innovation)[UD_HEIGHT_STATE_SIZE],
     const float hypothesis_mean,
     const float forgetting_factor,
     float(&states)[UD_HEIGHT_STATE_SIZE]);

   float Compute_Prob_Of_RCS_Slope_Hypothesis(
     const OCG_Calibrations_T& calib,
     const float(&innovation)[UD_RCS_STATE_SIZE],
     const float hypothesis_slope,
     const float forgetting_factor,
     float(&states)[UD_RCS_STATE_SIZE]);
   
   void Calc_Single_Zone_Probabilities(
     OCG_Single_Underdrivability_Zone_T& zone,
     const OCG_Zones_Innovation_T& innovation,
     const OCG_Calibrations_T& calib,
     const uint32_t zone_idx);

   void Assign_Single_Zone_Probabilities(
     OCG_Single_Underdrivability_Zone_T(&zones)[NUM_CELLS_X],
     const uint32_t zone_idx,
     const uint32_t circ_buff_zone_idx);
}
#endif
