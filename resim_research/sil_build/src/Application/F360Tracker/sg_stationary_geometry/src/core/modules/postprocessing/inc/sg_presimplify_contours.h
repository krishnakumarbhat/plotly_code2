/*=============================================================================================*\
* FILE: sg_presimplify_contours.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definictions of presimplify_contour functions and helpers
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standsards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_PRESIMPLIFY_CONTOURS_H
#define SG_PRESIMPLIFY_CONTOURS_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"

namespace sg
{
   /**
    * @brief             Function for pre_simplification of deviations of contours
    *
    * @param[in, out]    contours
    * @param[in]         calibrations
    **/
   void presimplify_contours(ContourStorage &contours, const Contour_Postprocessing_Calibrations_T::Simplify_Contours_T &calibrations);

   /**
    * @brief             Descriptive statistics approach - mean of deviations,
    *                    simplify if a contour is deviating out more than a mean deviation
    *
    * @param[in, out]    contour
    * @param[in]         host_length
    **/
   void mean_of_deviations_presimplify(Contour_T &contour, const float host_length);
}
#endif
