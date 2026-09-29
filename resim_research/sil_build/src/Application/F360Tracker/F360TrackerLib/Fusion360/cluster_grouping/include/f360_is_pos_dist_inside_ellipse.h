/*===================================================================================*\
* FILE:  f360_is_pos_dist_inside_ellipse.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose." 
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of function Is_Pos_Dist_Inside_Ellipse() which can be used for
* checking if a point is inside an elliptical gate
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#ifndef f360_IS_POS_DIST_INSIDE_ELLIPSE_H
#define f360_IS_POS_DIST_INSIDE_ELLIPSE_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   bool Is_Pos_Dist_Inside_Ellipse(
      const float32_t deltx,
      const float32_t delty,
      const float32_t cos_az,
      const float32_t sin_az,
      const float32_t orth_axis,
      const float32_t az_axis);
}
#endif
