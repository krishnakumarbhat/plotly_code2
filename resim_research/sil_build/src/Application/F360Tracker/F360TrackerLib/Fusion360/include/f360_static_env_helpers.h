/*===========================================================================*\
* FILE: f360_static_env_helpers.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains helper functions for the static environment class
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_STATIC_ENV_HELPERS_H
#define F360_STATIC_ENV_HELPERS_H

#include "f360_static_env_poly_types.h"
#include "f360_longi_stat_curve.h"

namespace f360_variant_A
{
   void Reset_Single_Static_Env_Poly(Static_Env_Poly_T& static_env_poly);

   void Map_Single_LSC_To_Static_Env_Poly(
      const F360_Longi_Stat_Curve_T& lsc,
      Static_Env_Poly_T& static_env_poly);
}


#endif
