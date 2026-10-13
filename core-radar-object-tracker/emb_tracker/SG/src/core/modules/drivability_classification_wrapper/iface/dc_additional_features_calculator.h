/*=============================================================================================*\
* FILE: dc_additional_features_calculator.h
* ====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of AdditionalFeaturesCalculator class that calculates new features for DC
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef DC_ADDITIONAL_FEATURES_CALCULATOR_WRAPPER_H
#define DC_ADDITIONAL_FEATURES_CALCULATOR_WRAPPER_H

#include <array>

#include "dc_vertices_dump.h"
#include "sg_variant.h"

static constexpr uint16_t DC_TOOLING_WRAPPER_NUMBER_OF_BINS_FOR_Z = 17U;
static_assert(DC_TOOLING_WRAPPER_NUMBER_OF_BINS_FOR_Z % 2U == 1U, "DC_TOOLING_WRAPPER_NUMBER_OF_BINS_FOR_Z must be odd");

static constexpr float BINS_INIT_VALUE = -8.5F;

// do not edit the value below
// value conatins information about number of all bins
// (+2 to the number of bins given by user, these two bins represent outliers
static constexpr uint16_t DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z = DC_TOOLING_WRAPPER_NUMBER_OF_BINS_FOR_Z + 2U;

static constexpr uint16_t DC_TOOLING_WRAPPER_NUMBER_OF_ABS_BINS_FOR_Z = (DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z + 1U) / 2U;
static constexpr std::array<float, DC_TOOLING_WRAPPER_NUMBER_OF_ABS_BINS_FOR_Z> LOW_Z_ABS_WEIGHTS  = {1.0F, 1.0F, 1.0F, 0.5F, 0.0F,
                                                                                                     0.0F, 0.0F, 0.0F, 0.0F, 0.0F};
static constexpr std::array<float, DC_TOOLING_WRAPPER_NUMBER_OF_ABS_BINS_FOR_Z> HIGH_Z_ABS_WEIGHTS = {
   0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.5F, 1.0F, 1.0F};
static constexpr std::array<float, DC_TOOLING_WRAPPER_NUMBER_OF_ABS_BINS_FOR_Z> MIDDLE_Z_ABS_WEIGHTS = {
   0.0F, 0.0F, 0.0F, 0.5F, 2.0F, 2.0F, 2.0F, 0.5F, 0.0F, 0.0F};

static constexpr float DC_DEFAULT_VALLEY_SYMMETRY_FACTOR = 0.1F;

static constexpr float DC_MIN_VALLEY_EDGE = 0.1F;

struct DC_Additional_Features_Dump_T
{
   std::array<std::array<uint16_t, DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z>, DC_MAX_NUM_OUTPUT_VERTICES> z_scs_bins{};

   std::array<std::array<uint16_t, DC_TOOLING_WRAPPER_NUMBER_OF_ABS_BINS_FOR_Z>, DC_MAX_NUM_OUTPUT_VERTICES> z_scs_abs_bins{};

   std::array<float, DC_MAX_NUM_OUTPUT_VERTICES> valley_detection_level;

   uint32_t current_vertices_ids[DC_MAX_NUM_OUTPUT_VERTICES] = {};

   float z_scs_abs_avg[DC_MAX_NUM_OUTPUT_VERTICES];
};

class AdditionalFeaturesCalculator
{
 public:
   AdditionalFeaturesCalculator() = default;

   DC_Additional_Features_Dump_T dc_additional_features_dump;
   void calculate_z_scs_abs_avg(const sg::DC_Assigned_Detections_Dump_T (&assigned_detections)[DC_MAX_NUM_OUTPUT_VERTICES]);
   void count_bin_detections(const sg::DC_Vertices_Dump_T &dc_vertices);
   void detect_valley();

 private:
   void find_vertex_from_past_step_and_move_bins(
      const uint32_t vertex_id_to_find, const uint16_t past_vertex_idx,
      const std::array<std::array<uint16_t, DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z>, DC_MAX_NUM_OUTPUT_VERTICES> &past_bins);

   template <class T, size_t N> T sum(const std::array<T, N> &arr);

   template <size_t N>
   std::array<float, N> multiply_element_wise(const std::array<float, N> &arr1, const std::array<uint16_t, N> &arr2);

   void update_abs_bins();
};

#endif
