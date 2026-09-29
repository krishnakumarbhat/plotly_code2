/*===========================================================================*\
* FILE: f360_longi_stat_curve_init.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of Longi_Stat_Curve_Init
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_longi_stat_curve_init.h" 

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: F360_Longi_Stat_Curve_Init()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_All_Longi_Stat_Curve_Tag &curves
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
   * This function initialize the array of longi stat curves to default values
   *
   * PRECONDITIONS:
   * All the pointers should point to valid structure
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void F360_Longi_Stat_Curve_Init(F360_Longi_Stat_Curve_T (&longi_stat_curves)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES])
   {
      for (uint32_t i = 0U; i < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; i++)
      {
         longi_stat_curves[i].f_valid = false;
         longi_stat_curves[i].x_min = 0.0F;
         longi_stat_curves[i].x_max = 0.0F;
         longi_stat_curves[i].a = 0.0F;
         longi_stat_curves[i].b = 0.0F;
         longi_stat_curves[i].c = 0.0F;
         longi_stat_curves[i].mean_lat_pos = INFTY;
      }
   }
}

