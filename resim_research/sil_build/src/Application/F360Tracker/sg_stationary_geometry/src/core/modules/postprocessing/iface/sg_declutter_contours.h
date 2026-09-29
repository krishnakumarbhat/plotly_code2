/*=============================================================================================*\
* FILE: sg_declutter_contours.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the declaration of declutter_contours function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_DECLUTTER_CONTOURS_H
#define SG_DECLUTTER_CONTOURS_H

#include "sg_calibrations.h"
#include "sg_contour_storage.h"

namespace sg
{
   /**
    * @brief            Function finds and remove occluded contours in the same cluster.
    *
    * @param[in, out]   contours
    * @param[in]        calibrations
    * @param[in]        azimuth_epsilon
    *
    **/
   void declutter_contours(ContourStorage &contours,
                           const Contour_Postprocessing_Calibrations_T::Declutter_Contours_T &calibrations,
                           const float azimuth_epsilon);
}
#endif
