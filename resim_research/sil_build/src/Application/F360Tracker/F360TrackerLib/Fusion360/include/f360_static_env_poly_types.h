/*===========================================================================*\
* FILE: f360_static_env_poly_types.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Static_Env_Poly()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_STATIC_ENV_POLY_H
#define F360_STATIC_ENV_POLY_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   enum Static_Env_Poly_Status_T : uint8_t
   {
      F360_STATIC_ENV_POLY_STATUS_INVALID = 0,
      F360_STATIC_ENV_POLY_STATUS_UPDATED = 1,
      F360_STATIC_ENV_POLY_STATUS_COASTED = 2,
   };

   enum Static_Env_Poly_Type_T : uint8_t
   {
      F360_STATIC_ENV_POLY_TYPE_INVALID = 0,
      F360_STATIC_ENV_POLY_TYPE_CURVG = 1, // Host curvature based guardrail estimate
      F360_STATIC_ENV_POLY_TYPE_LSC = 2, // Longitudinal static curves
      F360_STATIC_ENV_POLY_TYPE_CWD = 3, // Concrete wall detector
   };

   struct Static_Env_Poly_T
   {
      // Describes a polynomial on the form y(t) = p2*t^2 + p1*t + p0
      // Where t is either longitudinal or lateral VCS position depending on type
      float32_t confidence; // [-] Ranges from 0-1
      float32_t lower_limit; // [m] Lower validity limit
      float32_t upper_limit; // [m] Upper validity limit
      float32_t p0; // Polynomial zero order coefficient
      float32_t p1; // Polynomial first order coefficient
      float32_t p2; // Polynomial second order coefficient
      Static_Env_Poly_Status_T status;
      Static_Env_Poly_Type_T poly_type;
      uint8_t age; // [scans] Saturates at 255
      uint8_t padding;

      inline float32_t SEP_Lateral_Pos_At(const float32_t vcs_long_position) const
      {
         return (p2 * vcs_long_position * vcs_long_position + p1 * vcs_long_position + p0);
      }
   };

   struct Closest_SEP_Info
   {
      float32_t lat_pos; // [m] Lateral position of closest sep 
      uint8_t id; // [-] Id of closest sep
      uint8_t padding[3];
   };

   static_assert(28 == sizeof(Static_Env_Poly_T), "sizeof(Static_Env_Poly_T) not as expected. Remember to align padding if needed");
   static_assert(8 == sizeof(Closest_SEP_Info), "sizeof(Closest_SEP_Info) not as expected. Remember to align padding if needed");
}


#endif
