/*===================================================================================*\
* FILE: f360_calculate_jacobian_CTCA.cpp
*====================================================================================
* Copyright 2020 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*------------------------------------------------------------------------------------
* %full_filespec: AIT-63%
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*
*
* ABBREVIATIONS:
*   NONE
*
* TRACEABILITY INFO:
*   Design Document(s): calculateJacobianCTCA.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#include "f360_math.h"
#include "f360_calculate_jacobian_CTCA.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: F360_calculate_jacobian_CTCA()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t cos_psi
   * const float32_t sin_psi
   * const float32_t velocity
   * const float32_t curvature
   * const float32_t acceleration
   * const float32_t dT
   * const float32_t (&tcs_vec_from_center_rear_to_ref_pnt)[2]
   * float32_t jacobian[][STATE_DIMENSION]
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
   *
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void F360_Calculate_Jacobian_CTCA(
      const float32_t cos_psi,
      const float32_t sin_psi,
      const float32_t velocity,
      const float32_t curvature,
      const float32_t acceleration,
      const float32_t dT,
      const float32_t (&tcs_vec_from_center_rear_to_ref_pnt)[2],
      float32_t (&jacobian)[STATE_DIMENSION][STATE_DIMENSION])
   {
      /* CTCA model:
          State vector:
          x   - x position (VCS)
          y   - y position (VCS)
          psi - velocity heading (azimuth angle of velocity vector)
          c   - curvature
          v   - speed (magnitude of velocity vector)
          a   - tangential acceleration

          Model X' = AX + BU (continuous time state equation):
          x'   = v * cos(psi)
          y'   = v * sin(psi)
          psi' = c * v
          c'   = 0
          v'   = a
          a'   = 0

          Solution of discrete-time state equation i.e. x(n+1) = f(x(n)):
          x   = x + cos(psi)*(v*dT + 0.5*a*dT^2) - sin(psi)*(0.5*c*v^2*dT^2 + 0.5*c*a*v*dT^3 + 0.125*c*a^2*dT^4) - c*(dx*(sin(psi)*(v*dT + 0.5*a*dT^2) + c*cos(psi)*(0.5*v^2*dT^2 + 0.5*v*a*dT^3 + 0.125*a^2*dT^4)) + dy*(cos(psi)*(v*dT + 0.5*a*dT^2) - c*sin(psi)*(0.5*v^2*dT^2 + 0.5*v*a*dT^3 + 0.125*a^2*dT^4)))
          y   = y + sin(psi)*(v*dT + 0.5*a*dT^2) + cos(psi)*(0.5*c*v^2*dT^2 + 0.5*c*a*v*dT^3 + 0.125*c*a^2*dT^4) + c*(dx*(cos(psi)*(v*dT + 0.5*a*dT^2) - c*sin(psi)*(0.5*v^2*dT^2 + 0.5*v*a*dT^3 + 0.125*a^2*dT^4)) - dy*(sin(psi)*(v*dT + 0.5*a*dT^2) + c*cos(psi)*(0.5*v^2*dT^2 + 0.5*v*a*dT^3 + 0.125*a^2*dT^4)))
          psi = psi + c*v*dT + 0.5*c*a*dT^2
          c   = c
          v   = v + a*dT
          a   = a
      */

      const float32_t dT2 = dT*dT;
      const float32_t dT3 = dT2 * dT;
      const float32_t dT4 = dT3 * dT;

      const float32_t a2 = acceleration * acceleration;

      const float32_t v_dT = velocity * dT;
      const float32_t c_v_dT2 = curvature * velocity * dT2;
      const float32_t a_c_dT3_05 = acceleration * curvature * dT3 * 0.5F;
      const float32_t a2_c_dT4_0125 = curvature * a2 * dT4 * 0.125F;
      const float32_t a_c_v_dT3_05 = a_c_dT3_05 * velocity;
      const float32_t c_v2_dT2_05 = c_v_dT2 * velocity * 0.5F;
      const float32_t a_dT2_05 = acceleration * dT2 * 0.5F;
      const float32_t a2_dT4_0125 = a2 * dT4 * 0.125F;
      const float32_t a_v_dT3_05 = acceleration * velocity * dT3 * 0.5F;
      const float32_t v2_dT2_05 = velocity * velocity * dT2 * 0.5F;
      const float32_t a_c_dT4_025 = acceleration * curvature * dT4 * 0.25F;
      const float32_t c_v_dT3_05 = curvature * velocity * dT3 * 0.5F;

      const float32_t temp1 = velocity * dT + 0.5F * acceleration * dT2;
      const float32_t temp2 = 0.5F * velocity * velocity * dT2 + 0.5F * velocity * acceleration * dT3 + 0.125F * acceleration * acceleration * dT4;
      const float32_t dx = tcs_vec_from_center_rear_to_ref_pnt[0];
      const float32_t dy = tcs_vec_from_center_rear_to_ref_pnt[1];

      // jacobian[row_idx][col_idx]
      jacobian[0][0] = 1.0F;
      jacobian[0][1] = 0.0F;
      jacobian[0][2] = -sin_psi * (a_dT2_05 + v_dT) - cos_psi * (c_v2_dT2_05 + a_c_v_dT3_05 + a2_c_dT4_0125) + cos_psi * curvature * (- dx * temp1 + curvature * dy * temp2) - sin_psi * curvature * (- curvature * dx * temp2 - dy * temp1);
      jacobian[0][3] = -sin_psi * (a2_dT4_0125 + a_v_dT3_05 + v2_dT2_05) - temp1 * (dx * sin_psi + dy * cos_psi) - 2.0F * curvature * temp2 * (dx * cos_psi - dy * sin_psi);
      jacobian[0][4] = dT * cos_psi - sin_psi*(a_c_dT3_05 + c_v_dT2)            - curvature * dx * sin_psi * dT - 0.5F * curvature * curvature * dx * cos_psi * acceleration * dT3 - curvature * dy * cos_psi * dT + 0.5F * curvature * curvature * dy * sin_psi * acceleration * dT3 + 2.0F * velocity * (- 0.5F * curvature * curvature * dx * cos_psi * dT2 + 0.5F * curvature * curvature * dy * sin_psi * dT2);
      jacobian[0][5] = dT2 * cos_psi * 0.5F - sin_psi * (a_c_dT4_025 + c_v_dT3_05) - 0.5F * curvature * dx * sin_psi * dT2 - 0.5F * curvature * curvature * dx *cos_psi * velocity * dT3 - 0.5F * curvature * dy * cos_psi * dT2 + 0.5F * curvature * curvature * dy * sin_psi * velocity * dT3 + 2.0F * acceleration * (- 0.125F * curvature * curvature * dx * cos_psi * dT4 + 0.125F * curvature * curvature * dy * sin_psi * dT4);

      jacobian[1][0] = 0.0F;
      jacobian[1][1] = 1.0F;
      jacobian[1][2] = cos_psi * (a_dT2_05 + v_dT) - sin_psi * (c_v2_dT2_05 + a_c_v_dT3_05 + a2_c_dT4_0125) - sin_psi * curvature * (dx * temp1 - curvature * dy * temp2) + cos_psi * curvature * (- curvature * dx * temp2 - dy * temp1);
      jacobian[1][3] = cos_psi * (a2_dT4_0125 + a_v_dT3_05 + v2_dT2_05) + temp1 * (dx * cos_psi - dy * sin_psi) + 2.0F * curvature * temp2 * (- dx * sin_psi - dy * cos_psi);
      jacobian[1][4] = dT * sin_psi + cos_psi * (a_c_dT3_05 + c_v_dT2) + curvature * dx * cos_psi * dT - 0.5F * curvature * curvature * dx * sin_psi * acceleration * dT3 - curvature * dy * sin_psi * dT - 0.5F * curvature * curvature * dy * cos_psi * acceleration * dT3 + 2.0F * velocity * (- 0.5F * curvature * curvature * dx * sin_psi * dT2 - 0.5F * curvature * curvature * dy * cos_psi * dT2);
      jacobian[1][5] = dT2 * sin_psi * 0.5F + cos_psi * (a_c_dT4_025 + c_v_dT3_05) + 0.5F * curvature * dx * cos_psi * dT2 - 0.5F * curvature * curvature * dx * sin_psi * velocity * dT3 - 0.5F * curvature * dy * sin_psi * dT2 - 0.5F * curvature * curvature * dy * cos_psi * velocity * dT3 + 2.0F * acceleration * (- 0.125F * curvature * curvature * dx * sin_psi * dT4 - 0.125F * curvature * curvature * dy * cos_psi * dT4);

      jacobian[2][0] = 0.0F;
      jacobian[2][1] = 0.0F;
      jacobian[2][2] = 1.0F;
      jacobian[2][3] = a_dT2_05 + v_dT;
      jacobian[2][4] = dT * curvature;
      jacobian[2][5] = curvature * dT2 * 0.5F;

      jacobian[3][0] = 0.0F;
      jacobian[3][1] = 0.0F;
      jacobian[3][2] = 0.0F;
      jacobian[3][3] = 1.0F;
      jacobian[3][4] = 0.0F;
      jacobian[3][5] = 0.0F;

      jacobian[4][0] = 0.0F;
      jacobian[4][1] = 0.0F;
      jacobian[4][2] = 0.0F;
      jacobian[4][3] = 0.0F;
      jacobian[4][4] = 1.0F;
      jacobian[4][5] = dT;

      jacobian[5][0] = 0.0F;
      jacobian[5][1] = 0.0F;
      jacobian[5][2] = 0.0F;
      jacobian[5][3] = 0.0F;
      jacobian[5][4] = 0.0F;
      jacobian[5][5] = 1.0F;
   }
}
