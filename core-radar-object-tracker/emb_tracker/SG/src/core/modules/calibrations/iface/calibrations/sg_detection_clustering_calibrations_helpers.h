/*===========================================================================*\
* FILE: sg_detection_clustering_calibrations_helpers.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in detection clustering step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_DETECTION_CLUSTERING_CALIBRATIONS_HELPERS_H
#define SG_DETECTION_CLUSTERING_CALIBRATIONS_HELPERS_H

#include <array>

#include "sg_reuse.h"

namespace sg
{
struct Squeeze_Params_T
{
   float factor{};
   float range{};
};

struct LinearPiecewiseTransformCoefficients
{
   struct Coefficients_T
   {
      std::array<Squeeze_Params_T, 3> nondrivable{};
      std::array<Squeeze_Params_T, 3> underdrivable{};
   };

   Coefficients_T lateral{};
   Coefficients_T longitudinal{};
};

struct Existence_Probability_Calibrations_T
{
   float neighbors_factor{};
   float age_factor{};
   float alpha{};
};

struct ClusterDetectionsCalibs
{
   Existence_Probability_Calibrations_T existence_probability{}; // [-] calibrations related to existence probability
   float cluster_radius_nondrivable{};                           // [m] cluster radius for nondrivable
   float cluster_radius_underdrivable{};                         // [m] cluster radius for underdrivable
   float historical_num_neighbors_forgetting_factor{};           // [-] forgetting factor for historical number of neighbors
   uint8_t min_cluster_points_nondrivable{};     // [-] minimum amount of neighbors in region to create expand cluster [for
                                                 // nondrivable]
   uint8_t min_cluster_points_underdrivable{2U}; // [-] minimum amount of neighbors in region to create expand cluster [for
                                                 // underdrivable]
};
} // namespace sg
#endif
