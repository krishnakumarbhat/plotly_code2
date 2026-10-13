/*===========================================================================*\
* FILE: sg_drivability_classification_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in drivability classification step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_DRIVABILITY_CLASSIFICATION_CALIBRATIONS_H
#define SG_DRIVABILITY_CLASSIFICATION_CALIBRATIONS_H

#include "geometry/geo_point.h"
#include "sg_reuse.h"
#include "sg_variant.h"

namespace sg
{
struct DrivabilityClassificationCalibs
{
   // critical region for drivability classification
   geometry::Point2D_T critical_region_polygon[DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION]{};
   float critical_region_min_abs_curvature_threshold{};
   float critical_region_max_abs_curvature_threshold{};
   float subsegment_length{};
   uint8_t critical_region_subdivisions{};

   // update subsegments
   float projection_distance_threshold{};    // min distance to the nearest vertex when projection can take place
   float angle_between_segments_threshold{}; // min angle in radians between lines to consider them not parallel

   // assign_detections_to_subsegments_and_update_features
   float det_seg_assignment_threshold{};

   // subsegments classification
   float min_num_dets_for_classification{};
   float overdrivable_max_z{};
   float underdrivable_min_z{};
   float underdrivable_min_z_std{};
   float decision_tree_obstacle_prob_thr{};
   float decision_tree_overdrivability_obstacle_prob_thr{};

   // features calculation
   float features_inliers_threshold{};
   float features_radar_height{};
   float features_z_max_threshold{};
   float features_z_min_threshold{};
   float features_min_rr_comp_of_stat_dealias_det{};

   int8_t features_max_azimuth_confidence{};
   int8_t features_max_elevation_confidence{};

   bool classify_subsegments{};
   bool create_subsegments{};
};
} // namespace sg
#endif
