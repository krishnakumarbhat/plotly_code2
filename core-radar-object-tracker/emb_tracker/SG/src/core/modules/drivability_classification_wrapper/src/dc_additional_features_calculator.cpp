/*===================================================================================*\
* FILE: dc_additional_features_calculator.cpp
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definitions of functions used to calculate additional features
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_additional_features_calculator.h"

#include <algorithm>
#include <cmath>
#include <numeric>

void AdditionalFeaturesCalculator::calculate_z_scs_abs_avg(
   const sg::DC_Assigned_Detections_Dump_T (&assigned_detections)[DC_MAX_NUM_OUTPUT_VERTICES])
{
   std::fill_n(dc_additional_features_dump.z_scs_abs_avg, DC_MAX_NUM_OUTPUT_VERTICES, 0.0F);
   for (auto i{0U}; i < DC_MAX_NUM_OUTPUT_VERTICES; i++)
   {
      const auto num_valid_dets = assigned_detections[i].num_valid_dets;
      if (num_valid_dets > 0U)
      {
         float z_scs_abs_sum{0.0F};
         for (auto j{0U}; j < num_valid_dets; j++)
         {
            z_scs_abs_sum += assigned_detections[i].z_scs_abs[j];
         }
         dc_additional_features_dump.z_scs_abs_avg[i] = z_scs_abs_sum / static_cast<float>(num_valid_dets);
      }
   }
}

void AdditionalFeaturesCalculator::find_vertex_from_past_step_and_move_bins(
   const uint32_t vertex_id_to_find, const uint16_t past_step_vertex_idx,
   const std::array<std::array<uint16_t, DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z>, DC_MAX_NUM_OUTPUT_VERTICES> &past_bins)
{
   for (uint16_t current_vertex_idx = 0U; current_vertex_idx < DC_MAX_NUM_OUTPUT_VERTICES; current_vertex_idx++)
   {
      if (vertex_id_to_find == dc_additional_features_dump.current_vertices_ids[current_vertex_idx])
      {
         for (uint16_t bin_idx = 0U; bin_idx < DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z; bin_idx++)
         {
            dc_additional_features_dump.z_scs_bins[current_vertex_idx][bin_idx] = past_bins[past_step_vertex_idx][bin_idx];
         }
         break;
      }
   }
}

void AdditionalFeaturesCalculator::update_abs_bins()
{
   for (uint16_t current_vertex_idx = 0U; current_vertex_idx < SG_MAX_NUM_OUTPUT_VERTICES; current_vertex_idx++)
   {
      auto j = DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z - 1U;
      auto k = DC_TOOLING_WRAPPER_NUMBER_OF_ABS_BINS_FOR_Z - 1U;
      for (uint16_t i = 0U; i < DC_TOOLING_WRAPPER_NUMBER_OF_ABS_BINS_FOR_Z; i++)
      {
         if (j == i)
         {
            dc_additional_features_dump.z_scs_abs_bins[current_vertex_idx][k] =
               dc_additional_features_dump.z_scs_bins[current_vertex_idx][i];
         }
         else
         {
            dc_additional_features_dump.z_scs_abs_bins[current_vertex_idx][k] =
               dc_additional_features_dump.z_scs_bins[current_vertex_idx][i] +
               dc_additional_features_dump.z_scs_bins[current_vertex_idx][j];
         }
         j--;
         k--;
      }
   }
}

template <class T, size_t N> T AdditionalFeaturesCalculator::sum(const std::array<T, N> &arr)
{
   return std::accumulate(arr.begin(), arr.end(), 0.0F);
}

template <size_t N>
std::array<float, N> AdditionalFeaturesCalculator::multiply_element_wise(const std::array<float, N> &arr1,
                                                                         const std::array<uint16_t, N> &arr2)
{
   std::array<float, N> result = {};
   for (size_t i = 0U; i < N; i++)
   {
      result[i] = arr1[i] * static_cast<float>(arr2[i]);
   }
   return result;
}

void AdditionalFeaturesCalculator::count_bin_detections(const sg::DC_Vertices_Dump_T &dc_vertices)
{
   // prepare current data
   const std::array<std::array<uint16_t, DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z>, DC_MAX_NUM_OUTPUT_VERTICES> past_bins =
      dc_additional_features_dump.z_scs_bins;

   dc_additional_features_dump.z_scs_bins                 = {};
   uint32_t past_vertices_ids[DC_MAX_NUM_OUTPUT_VERTICES] = {};

   for (uint16_t idx = 0U; idx < DC_MAX_NUM_OUTPUT_VERTICES; idx++)
   {
      past_vertices_ids[idx]                                = dc_additional_features_dump.current_vertices_ids[idx];
      dc_additional_features_dump.current_vertices_ids[idx] = dc_vertices.unique_id[idx];
   }

   // move bins from past steps
   for (uint16_t past_vertex_idx = 0U; past_vertex_idx < DC_MAX_NUM_OUTPUT_VERTICES; past_vertex_idx++)
   {
      const uint32_t vertex_id_to_find = past_vertices_ids[past_vertex_idx];

      if (vertex_id_to_find != 0U)
      {
         find_vertex_from_past_step_and_move_bins(vertex_id_to_find, past_vertex_idx, past_bins);
      }
   }

   // count current assigned detections
   for (uint16_t idx = 0U; idx < DC_MAX_NUM_OUTPUT_VERTICES; idx++)
   {
      const uint16_t num_of_associated_dets = dc_vertices.assigned_detections[idx].num_associated_dets;

      for (uint16_t det_idx = 0U; det_idx < num_of_associated_dets; det_idx++)
      {
         const float z_scs = dc_vertices.assigned_detections[idx].z_scs[det_idx];

         // We assume that each bin has 1 meter
         if ((z_scs > BINS_INIT_VALUE) &&
             (z_scs <= (BINS_INIT_VALUE + static_cast<float>(DC_TOOLING_WRAPPER_NUMBER_OF_BINS_FOR_Z))))
         {
            // We assume that each bin has 1 meter
            float current_bin_end = BINS_INIT_VALUE + 1.0F;
            for (uint16_t bin_number = 1U; bin_number <= DC_TOOLING_WRAPPER_NUMBER_OF_BINS_FOR_Z; bin_number++)
            {
               if (z_scs <= current_bin_end)
               {
                  dc_additional_features_dump.z_scs_bins[idx][bin_number]++;
                  break;
               }
               current_bin_end += 1.0F;
            }
         }
         else if (z_scs <= BINS_INIT_VALUE)
         {
            dc_additional_features_dump.z_scs_bins[idx][0U]++;
         }
         else
         {
            dc_additional_features_dump.z_scs_bins[idx][DC_TOOLING_WRAPPER_NUMBER_OF_ALL_BINS_FOR_Z - 1U]++;
         }
      }
   }
}

void AdditionalFeaturesCalculator::detect_valley()
{
   update_abs_bins();

   for (uint16_t current_vertex_idx = 0U; current_vertex_idx < SG_MAX_NUM_OUTPUT_VERTICES; current_vertex_idx++)
   {
      const auto &height_hist_abs = dc_additional_features_dump.z_scs_abs_bins[current_vertex_idx];

      const float low_z_val    = sum(multiply_element_wise(LOW_Z_ABS_WEIGHTS, height_hist_abs));
      const float high_z_val   = sum(multiply_element_wise(HIGH_Z_ABS_WEIGHTS, height_hist_abs));
      const float middle_z_val = sum(multiply_element_wise(MIDDLE_Z_ABS_WEIGHTS, height_hist_abs));

      float valley_symmetry_factor = DC_DEFAULT_VALLEY_SYMMETRY_FACTOR;
      if (std::max(low_z_val, high_z_val) > DC_MIN_VALLEY_EDGE)
      {
         valley_symmetry_factor = 1.0F - std::pow(std::abs(low_z_val - high_z_val) / std::max(low_z_val, high_z_val), 0.5F);
      }
      const float dets_num                = static_cast<float>(sum(height_hist_abs));
      const float detection_number_factor = 1.0F - std::exp(-0.2F * (dets_num - 0.4F));

      float valley_feature =
         detection_number_factor * valley_symmetry_factor * (low_z_val + high_z_val) / (std::max(middle_z_val, 1.0F));

      valley_feature = 1.0F - 1.0F / (valley_feature + 1.0F);

      dc_additional_features_dump.valley_detection_level[current_vertex_idx] = valley_feature;
   }
}
