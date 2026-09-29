/*===========================================================================*\
* FILE: f360_find_closest_valid_sep_on_given_side.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Find_Closest_SEP_On_Given_Side()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_find_closest_valid_sep_on_given_side.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Find_Closest_SEP_On_Given_Side()
   * ===========================================================================
   * RETURN VALUE:
   * Closest_SEP_Info closest_sep_info, structure containing the id and lateral position of the closest valid SEP on the given side.
   *
   * PARAMETERS:
   *  const Static_Env_Poly_T(&stat_env_poly)[F360_NUM_OF_STATIC_ENV_POLYS] - polylines
   *  const F360_Side_Of_Host_T host_side
   *  const float32_t long_pos              - longitudinal position at sep is being looked for
   *  const float32_t long_margin           - longitudinal margin of sep checks w.r.t. long_pos param
   *  const float32_t min_abs_lat_pos       - minimum lateral absolut postion of sep to be taken into consideration
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
   * Find which Static Environment Polynomial is the closest one on a given side
   * of host for a longitudinal position. Looking algorithm can be tuned by 
   * min_abs_lat_pos and long_margin
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * Caller to function must check the id of found SEP is valid before usage of sep lat pos in return structure.
   * If no SEP have been found, lat_pos may be uninitialized.
   *
   \*===========================================================================*/
   Closest_SEP_Info Find_Closest_SEP_On_Given_Side(
      const Static_Env_Poly_T(&stat_env_poly)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Side_Of_Host_T host_side,
      const float32_t long_pos,
      const float32_t long_margin,
      const float32_t min_abs_lat_pos
   )
   {
      Closest_SEP_Info closest_sep = {};
      if (F360_HOST_INVALID_SIDE != host_side)
      {
         closest_sep.id = F360_INVALID_UNSIGNED_ID;
         float32_t closest_lat_pos_abs = INFTY;
         for (uint8_t i = 0U; i < F360_NUM_OF_STATIC_ENV_POLYS; i++)
         {
            const Static_Env_Poly_T& sep = stat_env_poly[i];
            if ((F360_STATIC_ENV_POLY_STATUS_INVALID != sep.status) &&
               ((sep.lower_limit - long_margin) < long_pos) &&
               (long_pos < (sep.upper_limit + long_margin)))
            {
               // Longitudinal position is within valid SEP interval, now calculate lateral position
               const float32_t current_sep_lat_pos = sep.SEP_Lateral_Pos_At(long_pos);
               const float32_t current_sep_lat_pos_abs = std::abs(current_sep_lat_pos);
               if ((F360_Sign(host_side) == F360_Sign(current_sep_lat_pos)) && (min_abs_lat_pos < current_sep_lat_pos_abs))
               {
                  // SEP is on desired side, now make sure that we pick the closest one
                  if (current_sep_lat_pos_abs < closest_lat_pos_abs)
                  {
                     closest_sep.id = i + 1U;
                     closest_lat_pos_abs = current_sep_lat_pos_abs;
                     closest_sep.lat_pos = current_sep_lat_pos;
                  }
               }
            }
         }
      }
      else
      {
         closest_sep.id = F360_INVALID_UNSIGNED_ID;
      }

      return closest_sep;
   }
}
