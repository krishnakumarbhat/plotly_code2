/*===================================================================================*\
* FILE: f360_norm_heading_angle.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Normalize_Heading_Angle()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_math.h"

#include "f360_reuse.h"
#include "f360_norm_heading_angle.h"
#include "f360_constants.h"
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Normalize_Heading_Angle()
   *===========================================================================
   * RETURN VALUE:
   * float32_t normalized_heading_angle: (radians)
   *
   * PARAMETERS:
   * const float32_t angle_in : (radians)
   * const float32_t interval_center : (radians)
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Adaptive normalization of heading angle.  'Adaptive' means that the
   * center of the normalized interval can be adjusted.The reason for doing
   * this is when calculating(and subsequent averaging of) sigma - points,
   * don't want any of them to go outside of the normalized angle interval.
   *
   * --------------------------------------------------------------------------
   *
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Normalize_Heading_Angle(
      const float32_t angle_in,
      const float32_t interval_center
   )
   {
      /* Adaptive normalization of heading angle.  'Adaptive' means that the
       * center of the normalized interval can be adjusted.The reason for doing
       * this is when calculating(and subsequent averaging of) sigma - points,
       * don't want any of them to go outside of the normalized angle interval.
       */
      const float32_t temp_val = (angle_in + (F360_PI - interval_center)) * (0.5F * F360_1_PI);
      const float32_t mod_val = (temp_val - F360_Floorf(temp_val)) * (F360_2PI);
      const float32_t norm = mod_val - (F360_PI - interval_center);

      return norm;
   }
}
