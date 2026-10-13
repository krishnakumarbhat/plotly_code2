/*=============================================================================================*\
* FILE: sg_remove_vertices.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains remove_vertices function declaration.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_REMOVE_VERTICES_H
#define SG_REMOVE_VERTICES_H

#include "sg_contour_postprocessing_calibrations_helpers.h"
#include "sg_contour_storage.h"
#include "sg_split_or_shorten_contour.h"

namespace sg
{
/**
 * @brief             Removes vertices from contours based on conditions defined in subfunctions:
 *                       - out of range, uncertain or unreliable
 * @param[in, out]    contours
 * @param[in]         calibrations
 **/
void remove_vertices(ContourStorage &contours, const RemoveVerticesCalibs &calibrations);
} // namespace sg
#endif
