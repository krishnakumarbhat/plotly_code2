/*===========================================================================*\
* FILE: sg_variant.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains variant configuration parameters
*   used in Stationary Geometries like array sizes, etc.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef SG_VARIANT_H
#define SG_VARIANT_H

#include <cstdint>

#include "sg_input.h"
#include "sg_variant_selector.h"

namespace sg
{
struct Variant final : public VariantSelected
{
   static constexpr std::uint16_t max_num_input_dets{rspp::MAX_NUMBER_OF_DETECTIONS}; // derived from f360 constants
   static constexpr std::uint16_t max_num_internal_dets{max_num_nondrivable_dets + max_num_underdrivable_dets};
   static constexpr std::uint16_t max_num_fused_vertices{max_num_vertices * 2U};
   static constexpr std::uint16_t max_num_fused_contours{max_num_contours};
   static constexpr std::uint16_t max_num_fused_vertices_per_contour{max_num_subvertices_per_contour};
   static constexpr std::uint16_t max_num_output_contours{max_num_fused_contours};
   static constexpr std::uint16_t max_num_output_vertices{max_num_fused_vertices};
   static constexpr std::uint16_t num_bins{static_cast<std::uint16_t>((bin_bounds_max - bin_bounds_min) / bin_width)};
};
} // namespace sg

// Below code maps new multivariant approach to old "separated" constants.
// The constants will be iteratively replaced in whole code and removed from here.
static constexpr std::uint16_t SG_MAX_NUM_CONTOURS = sg::Variant::max_num_contours;

static constexpr std::uint16_t SG_MAX_NUM_VERTICES = sg::Variant::max_num_vertices;

static constexpr std::uint16_t SG_MAX_NUM_VERTICES_PER_CONTOUR = sg::Variant::max_num_vertices_per_contour;
static_assert(SG_MAX_NUM_VERTICES_PER_CONTOUR <= SG_MAX_NUM_VERTICES,
              "Number of vertices per contour cannot be higher than maximum number of vertices");

static constexpr std::uint16_t SG_MAX_NUM_DETS_PER_SUBSEGMENT = sg::Variant::max_num_dets_per_subsegment;

static constexpr std::uint16_t SG_MAX_NUM_INTERNAL_CLUSTERS = sg::Variant::max_num_internal_clusters;

static constexpr std::uint16_t SG_MAX_NUM_INPUT_DETS = sg::Variant::max_num_input_dets;
static_assert(SG_MAX_NUM_INPUT_DETS == sg::rspp::MAX_NUMBER_OF_DETECTIONS,
              "Number of SG input detections should be the same as the number of RSPP detections");

static constexpr std::uint16_t SG_MAX_NUM_REDUCED_OUTPUT_CONTOURS = sg::Variant::max_num_reduced_output_contours;

static constexpr std::uint16_t SG_MAX_NUM_REDUCED_OUTPUT_VERTICES = sg::Variant::max_num_reduced_output_vertices;

static constexpr std::uint16_t SG_MAX_NUM_SUBVERTICES_PER_CONTOUR = sg::Variant::max_num_subvertices_per_contour;

static constexpr std::uint16_t SG_MAX_NUM_NONDRIVABLE_DETS = sg::Variant::max_num_nondrivable_dets;

static constexpr std::uint16_t SG_MAX_NUM_UNDERDRIVABLE_DETS = sg::Variant::max_num_underdrivable_dets;

static constexpr std::uint16_t SG_MAX_NUM_INTERNAL_DETS = sg::Variant::max_num_internal_dets;

static constexpr std::uint8_t SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS = sg::Variant::max_num_det_clustering_neighbours;
static_assert(SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS <= SG_MAX_NUM_INTERNAL_DETS,
              "SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS cannot be greater than SG_MAX_NUM_INTERNAL_DETS");

static constexpr std::uint16_t SG_MAX_NUM_FUSED_VERTICES = sg::Variant::max_num_fused_vertices;

static constexpr std::uint16_t SG_MAX_NUM_FUSED_CONTOURS = sg::Variant::max_num_fused_contours;

static constexpr std::uint16_t SG_MAX_NUM_FUSED_VERTICES_PER_CONTOUR = sg::Variant::max_num_fused_vertices_per_contour;

static constexpr std::uint16_t SG_MAX_NUM_OUTPUT_CONTOURS = sg::Variant::max_num_output_contours;
static_assert(SG_MAX_NUM_OUTPUT_CONTOURS <= SG_MAX_NUM_FUSED_CONTOURS,
              "Number of output contours cannot be highier than maximum number of fused contours");

static constexpr std::uint16_t SG_MAX_NUM_OUTPUT_VERTICES = sg::Variant::max_num_output_vertices;
static_assert(SG_MAX_NUM_OUTPUT_VERTICES <= SG_MAX_NUM_FUSED_VERTICES,
              "Number of output vertices cannot be highier than maximum number of fused vertices");

static constexpr std::uint16_t DC_MAX_NUM_OUTPUT_VERTICES = sg::Variant::dc_max_num_output_vertices;

static constexpr std::uint8_t SG_H_MATRIX_SIZE = sg::Variant::h_matrix_size;

static constexpr std::uint8_t DC_MAX_COORDINATES_REGION_SIZE = sg::Variant::dc_max_coordinates_region_size;
static constexpr std::uint8_t DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION =
   sg::Variant::dc_num_polygon_vertices_before_interpolation;

static constexpr auto SG_BIN_WIDTH      = sg::Variant::bin_width;
static constexpr auto SG_BIN_BOUNDS_MIN = sg::Variant::bin_bounds_min;
static constexpr auto SG_BIN_BOUNDS_MAX = sg::Variant::bin_bounds_max;
static constexpr auto SG_NUM_BINS       = sg::Variant::num_bins;

static constexpr std::uint16_t SG_MAX_NUM_ENDING_SEGMENT_DETS = sg::Variant::max_num_ending_segment_dets;

static constexpr float STARTING_VELOCITY_THRESHOLD = 0.0F;  // TODO (FZD-2392): It should be calibration (used in run-time only)
static constexpr float PARKING_VELOCITY_THRESHOLD  = 3.0F;  // TODO (FZD-2392): It should be calibration (used in run-time only)
static constexpr float CITY_VELOCITY_THRESHOLD     = 14.0F; // TODO (FZD-2392): It should be calibration (used in run-time only)

static constexpr float SG_DEFAULT_DRIVABILITY_CONFIDENCE = 65.0F; // TODO (FZD-2392): It should be calibration (used in run-time
                                                                  // only)

static constexpr std::uint32_t DC_PAST_DATA_MAX_AGE = 10000000U; // TODO (FZD-2391): It should be calibration (used in run-time
                                                                 // only)
static constexpr std::uint32_t DC_PAST_DATA_MAX_DETECTIONS_SUM = 10000000U; // TODO (FZD-2391): It should be calibration (used in
                                                                            // run-time only)

#endif
