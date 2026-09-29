/*===========================================================================*\
* FILE: f360_try_to_dealiase_range_rate.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Try_To_Dealiase_Range_Rate()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_TRY_TO_DEALIASE_RANGE_RATE_H
#define F360_TRY_TO_DEALIASE_RANGE_RATE_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   bool Try_To_Dealiase_Range_Rate(
      const float32_t rdot,
      const float32_t rdot_pred,
      const float32_t threshold,
      const float32_t rdot_interval_width,
      const float32_t min_aliased_rdot,
      float32_t& rdot_dealiased,
      float32_t& rr_interval);

   bool Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(
      const float32_t rdot,
      const float32_t rdot_pred,
      const float32_t threshold_lower,
      const float32_t threshold_upper,
      const float32_t rdot_interval_width,
      const float32_t min_aliased_rdot,
      float32_t& rdot_dealiased,
      float32_t& rr_interval);
}


#endif
