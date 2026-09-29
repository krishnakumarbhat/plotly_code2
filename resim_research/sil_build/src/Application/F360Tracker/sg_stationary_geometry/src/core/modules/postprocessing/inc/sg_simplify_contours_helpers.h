/*=============================================================================================*\
* FILE: sg_simplify_contours_helpers.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains simplify_contours function helpers
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_SIMPLIFY_CONTOURS_HELPERS_H
#define SG_SIMPLIFY_CONTOURS_HELPERS_H

#include "sg_contour_storage.h"

namespace sg
{
   /**
    * @brief             Auxiliary function to remove endpoints that create
    *                    acute angles with the rest of the contour
    *
    * @param[in, out]    contour
    * @param[in]         simplify_min_turning_angle
    **/
   void remove_acute_endpoints(Contour_T &contour, const float simplify_min_turning_angle);
}

#endif