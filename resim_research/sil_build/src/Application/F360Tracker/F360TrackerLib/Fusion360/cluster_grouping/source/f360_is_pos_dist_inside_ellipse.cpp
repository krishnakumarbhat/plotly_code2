/*===================================================================================*\
* FILE:  f360_is_pos_dist_inside_ellipse.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of the function Is_Pos_Dist_Inside_Ellipse() which can be used for
* checking if a point is inside an elliptical gate
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#include "f360_is_pos_dist_inside_ellipse.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Pos_Dist_Inside_Ellipse()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t deltx
   * const float32_t delty
   * const float32_t cos_az
   * const float32_t sin_az
   * const float32_t orth_axis
   * const float32_t az_axis
   *
   * EXTERNAL REFERENCES: ellipticalPosnGate.m
   *
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * The function performs elliptical position gating. The ellipsoid gate is tilted
   * in to have one axis at an angle corresponding to cos_az/sin_az. the length of
   * this axis is determined by az_axis. The other ellipsoid axis is orthogonal to
   * the first and its length is given by orth_axis. The values deltx and delty
   * correspond to the position for which to check if it is inside or outside of
   * the ellipsoid.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Pos_Dist_Inside_Ellipse(
      const float32_t deltx,
      const float32_t delty,
      const float32_t cos_az,
      const float32_t sin_az,
      const float32_t orth_axis,
      const float32_t az_axis)
   {
      const float32_t cos_az_sqr = cos_az * cos_az;
      const float32_t sin_az_sqr = sin_az * sin_az;
      const float32_t az_axis_sqr = az_axis * az_axis;
      const float32_t orth_axis_sqr = orth_axis * orth_axis;

      const float32_t temp = ((((orth_axis_sqr * cos_az_sqr) + (az_axis_sqr * sin_az_sqr)) * deltx * deltx) +
         2.0F * cos_az * sin_az * (orth_axis_sqr - az_axis_sqr) * deltx * delty + 
         (orth_axis_sqr * sin_az_sqr + az_axis_sqr * cos_az_sqr) * delty * delty);

      return (temp <= (az_axis_sqr * orth_axis_sqr));
   }
}
