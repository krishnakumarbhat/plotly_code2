/*===================================================================================*\
* FILE: ocg_test_sufficient_probabilities.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Test_Sufficient_Probabilities() and accompanying functions declarations
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef TEST_SUFFICIENT_PROBABILITIES_H
#define TEST_SUFFICIENT_PROBABILITIES_H

#include "ocg_reuse.h"

namespace ocg
{
/*===========================================================================*\
* FUNCTION: Find_First_Lower_Height_Breakpoint_Index()
*===========================================================================
* RETURN VALUE:
* int32_t height_bp_idx                        - index of first height breakpoint smaller or equal to prob_height
*
* PARAMETERS:
* const float (&breakpoints_height)[num_breakpoints]   - array of height breakpoints
* const float prob_height                       - height probability
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function finds first breakpoint that is smaller or equal to height probability.
*
* PRECONDITIONS:
* breakpoints_height array has to be sorted descending.
*
* POSTCONDITIONS:
* None.
*
\*===========================================================================*/
template <int32_t num_breakpoints>
int32_t Find_First_Lower_Height_Breakpoint_Index(const float (&breakpoints_height)[num_breakpoints], const float prob_height)
{
   int32_t height_bp_idx = -1;

   for (int32_t breakpoint_idx = 0; breakpoint_idx < num_breakpoints; breakpoint_idx++)
   {
      if (breakpoints_height[breakpoint_idx] <= prob_height)
      {
         height_bp_idx = breakpoint_idx;
         break;
      }
   }
   return height_bp_idx;
}

/*===========================================================================*\
* FUNCTION: Find_Last_Higher_Slope_Breakpoint_Index()
*===========================================================================
* RETURN VALUE:
* int32_t slope_bp_idx                         - index of greatest slope breakpoint greater or equal to slope probability
*
* PARAMETERS:
* const float (&breakpoints_slope)[num_breakpoints]   - array of slope breakpoints
* const float prob_slope                        - slope probability
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function finds greatest slope breakpoint greater than slope probability.
*
* PRECONDITIONS:
* breakpoints_slope array has to be sorted ascending.
*
* POSTCONDITIONS:
* None.
*
\*===========================================================================*/
template <int32_t num_breakpoints>
int32_t Find_Last_Higher_Slope_Breakpoint_Index(const float (&breakpoints_slope)[num_breakpoints], const float prob_slope)
{
   int32_t slope_bp_idx = -1;
   for (int32_t breakpoint_idx = (num_breakpoints - 1); 0 <= breakpoint_idx; breakpoint_idx--)
   {
      if (breakpoints_slope[breakpoint_idx] <= prob_slope)
      {
         slope_bp_idx = breakpoint_idx;
         break;
      }
   }
   return slope_bp_idx;
}

/*===========================================================================*\
* FUNCTION: Test_Sufficient_Probabilities()
*===========================================================================
* RETURN VALUE:
* bool                                   - flag indicating whether test passed
*
* PARAMETERS:
* const float prob_height                       - height probability
* const float prob_slope                        - slope probability
* const float prob_false                        - false test probability
* const float (&breakpoints_height)[num_breakpoints]   - array of height breakpoints
* const float (&breakpoints_slope)[num_breakpoints]   - array of slope breakpoints
* const float threshold_prob_false                - false probability threshold
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function tests sufficient probabilities.
*
* PRECONDITIONS:
* breakpoints_height and breakpoints_slope arrays need to have same size and
* contain unique values.
*
* POSTCONDITIONS:
* None.
*
\*===========================================================================*/
template <int32_t num_breakpoints>
bool Test_Sufficient_Probabilities(const float prob_height, const float prob_slope, const float prob_false,
                                   const float (&breakpoints_height)[num_breakpoints],
                                   const float (&breakpoints_slope)[num_breakpoints], const float threshold_prob_false,
                                   const int32_t k_max_slope_vs_height_index_diff)
{
   bool sufficient_probabilities = false;

   (void)prob_slope;
   (void)breakpoints_slope;
   (void)k_max_slope_vs_height_index_diff;

   if (prob_false < threshold_prob_false)
   {
      if (prob_height >= breakpoints_height[0]) // assuming all breakpoints have the same value
      {
         sufficient_probabilities = true;
      }
   }
   return sufficient_probabilities;
}
} // namespace ocg
#endif
