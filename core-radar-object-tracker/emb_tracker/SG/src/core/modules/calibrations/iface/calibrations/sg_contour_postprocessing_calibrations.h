/*===========================================================================*\
* FILE: sg_contour_postprocessing_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in contour postprocessing step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_CONTOUR_POSTPROCESSING_CALIBRATIONS_H
#define SG_CONTOUR_POSTPROCESSING_CALIBRATIONS_H

#include "sg_contour_postprocessing_calibrations_helpers.h"

namespace sg
{
struct ContourPostprocessingCalibs
{
   DeclutterContoursCalibs declutter_contours{};
   MergeContoursCalibs merge_contours{};
   SimplifyContoursCalibs simplify_contours{};
   RemoveVerticesCalibs remove_vertices{};
   SelectAndRemoveExcessContoursCalibs select_and_remove_excess_contours{};
};
} // namespace sg
#endif
