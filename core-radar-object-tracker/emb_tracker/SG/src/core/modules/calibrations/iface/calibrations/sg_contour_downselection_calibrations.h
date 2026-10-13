/*===========================================================================*\
* FILE: sg_contour_downselection_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in contour downselection step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_CONTOUR_DOWNSELECTION_CALIBRATIONS_H
#define SG_CONTOUR_DOWNSELECTION_CALIBRATIONS_H

#include "sg_reuse.h"

namespace sg
{
struct ContourDownselectionCalibs
{
   float base_priority_ratio{};
   float already_selected_contours_priority_gain{};
   float curvi_roi_curvature_threshold{};
   float roi_front_length_vcs{};
   float roi_half_width_vcs{};
   float roi_rear_length_vcs{};
   float contour_priority_shift_x{};
   float contour_priority_compaction_factor_x{};
   float contour_priority_compaction_factor_y{};
   float min_x_to_downselect_contour{};
   float min_required_underdrivable_share_to_reject_contour{};
   float min_required_overdrivable_share_to_reject_contour{};
   float min_underdrivable_share_along_with_unclassified{};
   float min_unclassified_share_along_with_underdrivable{};
   float min_required_contour_priority_to_downselect{};
   float importance_decay_coef{};
   float position_importance_saturation{};
   float contour_length_sq_threshold{};
   uint16_t min_number_of_valid_vertices{};
};
} // namespace sg
#endif
