/*===========================================================================*\
* FILE: f360_find_closest_valid_sep_on_given_side.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Find_Closest_SEP_On_Given_Side()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_FIND_CLOSEST_VALID_SEP_ON_SIDE_H
#define F360_FIND_CLOSEST_VALID_SEP_ON_SIDE_H

#include "f360_static_env_poly_types.h"
#include "f360_side_of_host.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   Closest_SEP_Info Find_Closest_SEP_On_Given_Side(
      const Static_Env_Poly_T(&stat_env_poly)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Side_Of_Host_T host_side,
      const float32_t long_pos,
      const float32_t long_margin = 0.0F,
      const float32_t min_abs_lat_pos = 0.0F
   );
}


#endif
