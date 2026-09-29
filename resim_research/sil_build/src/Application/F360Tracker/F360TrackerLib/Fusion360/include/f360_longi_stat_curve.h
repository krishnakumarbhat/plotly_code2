/*===========================================================================*\
* FILE: f360_longi_stat_curve.h
*============================================================================
* Copyright (C) 2021 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Longi_Stat_Curve_T structure declaration
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_LONGI_STAT_CURVE_H
#define F360_LONGI_STAT_CURVE_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   typedef struct F360_Longi_Stat_Curve_Tag
   {
      // Structure describes a second degree polynomial on the form ax^2+bx+c
      float32_t x_min; // [m] Minimum valid longitudinal position
      float32_t x_max; // [m] Maximum valid longitudinal position
      float32_t a; // [-] Second order coefficient
      float32_t b; // [-] First order coefficient
      float32_t c; // [-] Zero order coefficient
      float32_t mean_lat_pos; // [m] Mean lateral position of all objects used to fit the polynomial
      bool f_valid;
      uint8_t padding[3];

      inline float32_t LSC_Lateral_Pos_At(const float32_t vcs_long_position) const
      {
         return (a * vcs_long_position * vcs_long_position + b * vcs_long_position + c);
      }
   } F360_Longi_Stat_Curve_T;
}

#endif
