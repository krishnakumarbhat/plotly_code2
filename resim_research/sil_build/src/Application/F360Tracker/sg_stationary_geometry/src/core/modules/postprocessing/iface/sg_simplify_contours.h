/*=============================================================================================*\
* FILE: sg_simplify_contours.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the functions provided by the algorithm simplify contours.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_SIMPLIFY_CONTOURS_H
#define SG_SIMPLIFY_CONTOURS_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"

namespace sg
{
   /**
    * @brief             Function for simplification of deviations of contours
    *
    * @param[in, out]    contours
    * @param[in]         calibrations
    **/
   void simplify_contours(ContourStorage &contours, const Contour_Postprocessing_Calibrations_T::Simplify_Contours_T &calibrations);
}
#endif
