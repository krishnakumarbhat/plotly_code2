/*===========================================================================*\
* FILE: f360_static_env_helpers.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Static_Env_Helpers()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_static_env_helpers.h" 

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Reset_Single_Static_Env_Poly()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *    Static_Env_Poly_T& static_env_poly
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
   * Resets a singe static environment polynomial
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Reset_Single_Static_Env_Poly(Static_Env_Poly_T& static_env_poly)
   {
      static_env_poly.age = 0U;
      static_env_poly.confidence = 0.0F;
      static_env_poly.status = F360_STATIC_ENV_POLY_STATUS_INVALID;
      static_env_poly.poly_type = F360_STATIC_ENV_POLY_TYPE_INVALID;
      static_env_poly.lower_limit = 0.0F;
      static_env_poly.upper_limit = 0.0F;
      static_env_poly.p0 = 0.0F;
      static_env_poly.p1 = 0.0F;
      static_env_poly.p2 = 0.0F;
      static_env_poly.padding = 0U;
   }

   /*===========================================================================*\
   * FUNCTION: Map_Single_LSC_To_Static_Env_Poly()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *    const F360_Longi_Stat_Curve_T& lsc,
   *    Static_Env_Poly_T& static_env_poly
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
   * Maps a polynomial found by LSC module to the common static environment 
   * polynomial type.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Map_Single_LSC_To_Static_Env_Poly(
      const F360_Longi_Stat_Curve_T& lsc,
      Static_Env_Poly_T& static_env_poly)
   {
      if (lsc.f_valid)
      {
         static_env_poly.age = 1U; // LSC is instantaneous
         static_env_poly.confidence = 1.0F; // LSC does not have any confidence signal
         static_env_poly.status = F360_STATIC_ENV_POLY_STATUS_UPDATED; // LSC have no coasting status
         static_env_poly.poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;
         static_env_poly.lower_limit = lsc.x_min;
         static_env_poly.upper_limit = lsc.x_max;
         static_env_poly.p0 = lsc.c;
         static_env_poly.p1 = lsc.b;
         static_env_poly.p2 = lsc.a;
      }
      else
      {
         Reset_Single_Static_Env_Poly(static_env_poly);
      }
   }

}

