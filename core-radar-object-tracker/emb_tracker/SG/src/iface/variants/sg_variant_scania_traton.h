/*===========================================================================*\
* FILE: sg_variant_scania.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Stationary Geometries SCANIA variant.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef SG_VARIANT_SCANIA_H
#define SG_VARIANT_SCANIA_H

#include <cstdint>

#include "sg_variant_base.h"

namespace sg
{
struct VariantScaniaTraton : public VariantBase
{
   static constexpr float bin_bounds_min{-30.0F};
   static constexpr float bin_bounds_max{60.0F};
   static constexpr std::uint16_t max_num_contours{8U};
   static constexpr std::uint16_t max_num_det_clustering_neighbours{20U};
   static constexpr std::uint16_t max_num_internal_clusters{41U};
   static constexpr std::uint16_t max_num_nondrivable_dets{40U};
   static constexpr std::uint16_t max_num_reduced_output_contours{4U};
   static constexpr std::uint16_t max_num_reduced_output_vertices{20U};
   static constexpr std::uint16_t max_num_underdrivable_dets{0U};
   static constexpr std::uint16_t max_num_vertices_per_contour{10U};
   static constexpr std::uint16_t max_num_vertices{20U};
   static constexpr std::uint16_t max_num_subvertices_per_contour{4U};
};
} // namespace sg

#endif
