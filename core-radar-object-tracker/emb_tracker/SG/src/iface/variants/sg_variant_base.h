/*===========================================================================*\
* FILE: sg_variant_base.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Stationary Geometries base variant.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef SG_VARIANT_BASE_H
#define SG_VARIANT_BASE_H

#include <cstdint>

namespace sg
{
struct VariantBase
{
   static constexpr float bin_width{1.0F};
   static constexpr float bin_bounds_min{0.0F};
   static constexpr float bin_bounds_max{250.0F};
   static constexpr std::uint16_t max_num_contours{100U};
   static constexpr std::uint16_t max_num_det_clustering_neighbours{250U};
   static constexpr std::uint16_t max_num_dets_per_subsegment{10U};
   static constexpr std::uint16_t max_num_ending_segment_dets{20U};
   static constexpr std::uint16_t max_num_internal_clusters{250U};
   static constexpr std::uint16_t max_num_nondrivable_dets{235U};
   static constexpr std::uint16_t max_num_reduced_output_contours{10U};
   static constexpr std::uint16_t max_num_reduced_output_vertices{100U};
   static constexpr std::uint16_t max_num_subvertices_per_contour{100U};
   static constexpr std::uint16_t max_num_underdrivable_dets{65U};
   static constexpr std::uint16_t max_num_vertices_per_contour{50U};
   static constexpr std::uint16_t max_num_vertices{200U};
   static constexpr std::uint8_t h_matrix_size{4U};
   static constexpr std::uint8_t dc_max_coordinates_region_size{8U};
   static constexpr std::uint8_t dc_num_polygon_vertices_before_interpolation{4U};
   static constexpr std::uint16_t dc_max_num_output_vertices{500U};
};
} // namespace sg

#endif
