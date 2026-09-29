/*===================================================================================*\
* FILE: ocg_calc_underdrivability_probabilities.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Calc_Underdrivability_Probabilities() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5045)
#endif

#include "ocg_calc_underdrivability_probabilities.h"
#include "ocg_calc_underdrivability_probabilities_helpers.h"
#include "cmn_math_func.h"
#include "cmn_calc_circular_zone_idx.h"

namespace ocg
{
   /*===========================================================================*\
   * FUNCTION: Calc_Underdrivability_Probabilities()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * OCG_Underdrivability_Internal_T& underdrivability
   * OCG_Zones_Innovation_T(&zones_innovation)[NUM_CELLS_X]
   * const OCG_Calibrations_T& calib
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
   * Function calculates underdrivability probabilities of zones.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_Underdrivability_Probabilities(
      OCG_Underdrivability_Internal_T& in_underdrivability,
      OCG_Zones_Innovation_T(&zones_innovation)[NUM_CELLS_X],
      const OCG_Calibrations_T& calib)
   {
      for (uint32_t zone_idx = 0U; zone_idx < NUM_CELLS_X; zone_idx++)
      {
         const uint32_t circ_buff_zone_idx = cmn::Calc_Circular_Zone_Idx(in_underdrivability.props.circular_buffer_idx, zone_idx, NUM_CELLS_X);
       
         Calc_Single_Zone_Probabilities(in_underdrivability.zones[circ_buff_zone_idx], zones_innovation[circ_buff_zone_idx], calib, zone_idx);

         Assign_Single_Zone_Probabilities(in_underdrivability.zones, zone_idx, circ_buff_zone_idx);
      }
   }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
