/*===========================================================================*\
* FILE: sg_contour_initialization_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in contour initialization step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_CONTOUR_INITIALIZATION_CALIBRATIONS_H
#define SG_CONTOUR_INITIALIZATION_CALIBRATIONS_H

#include "sg_reuse.h"

namespace sg
{
struct ContourInitializationCalibs
{
   float cluster_priority_shift_x{};
   float cluster_priority_compaction_factor_x{};
   float cluster_priority_compaction_factor_y{};
   float cluster_importance_decay_coef{};
   // get_unoccluded_dets_mask
   float init_azimuth_margin{};
   float range_margin{};

   // initialize_contours
   float init_look_distance{};
   float init_look_distance_underdrivable{};
   float min_init_segment_length{};
   float sub_cluster_radius{};
   float sensor_look_diversity_threshold{};
   float min_dets_span_for_stationary_hypothesis{};
   float init_vertex_covariance{};
   uint16_t max_num_init_contoured_clusters{};
   uint16_t min_num_dets_for_stationary_hypothesis{};
   uint16_t min_num_vertices_for_stationary_hypothesis{};
   uint8_t sub_min_cluster_points{};
   uint8_t min_dets_for_contour_init{};
   uint8_t min_dets_for_first_contour_init{};
   uint8_t max_det_age_for_contour_init{};
};
} // namespace sg
#endif
