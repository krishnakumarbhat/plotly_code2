/*===================================================================================*\
* FILE: dc_calculate_subsegment_features.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains calculate_subsegment_features function implementation.
*
*  Applicable Standards (in order of precedence: highest first):
*    ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*    ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_calculate_subsegment_features.h"

#include <algorithm>
#include <numeric>

#include "dc_features_calculator.h"

namespace sg
{
   namespace dc
   {
      void calculate_subsegment_features(Subsegment_T &subsegment, const SubsegmentDetections_T &current_data)
      {
         if (current_data.num_valid_dets > 0U)
         {
            incorporate_current_data_to_past_data(subsegment, current_data);
         }
         assign_features(subsegment);
      }

      void incorporate_current_data_to_past_data(Subsegment_T &subsegment, const SubsegmentDetections_T &current_data)
      {
         const uint32_t num_current_data    = static_cast<uint32_t>(current_data.num_valid_dets);
         const float num_current_data_float = static_cast<float>(num_current_data);
         subsegment.past_data.add_to_detections_sum(num_current_data);
         const uint32_t num_data    = subsegment.past_data.get_detections_sum();
         const float num_data_float = static_cast<float>(num_data);

         // means
         Signals_T means;
         const Signals_T &old_means = subsegment.past_data.get_means();
         const float current_data_rcs_mean =
            std::accumulate(current_data.rcs.begin(), current_data.rcs.end(), 0.0F) / num_current_data_float;
         means.set_rcs(FeaturesCalculator::calculate_recursive_per_scan_mean(current_data_rcs_mean, num_current_data_float,
                                                                             old_means.get_rcs(), num_data_float));
         const float current_data_z_scs_abs_mean =
            std::accumulate(current_data.z_scs_abs.begin(), current_data.z_scs_abs.end(), 0.0F) / num_current_data_float;
         if (num_current_data == num_data)
         {
            means.set_z_scs_abs_ewma025(current_data_z_scs_abs_mean);
         }
         else
         {
            const float decay_factor = 0.25F;
            means.set_z_scs_abs_ewma025(
               FeaturesCalculator::calculate_EWMA(decay_factor, old_means.get_z_scs_abs_ewma025(), current_data_z_scs_abs_mean));
         }
         means.set_z_scs_abs_recur(FeaturesCalculator::calculate_recursive_per_scan_mean(
            current_data_z_scs_abs_mean, num_current_data_float, old_means.get_z_scs_abs_recur(), num_data_float));

         // maxes
         Signal_Extremes_T maxes;
         const Signal_Extremes_T &old_maxes = subsegment.past_data.get_maxes();
         const auto current_data_z_scs_abs_max_iter = std::max_element(current_data.z_scs_abs.begin(), current_data.z_scs_abs.end());
         maxes.set_z_scs_abs(std::max(old_maxes.get_z_scs_abs(), *current_data_z_scs_abs_max_iter));

         // detections density
         FeaturesCalculator::Under_Nondr_Feature_Config_T underdrivable_cfg{};
         FeaturesCalculator::get_under_nondr_config(underdrivable_cfg);
         FeaturesCalculator::calculate_under_nondr_bins_proportion(subsegment.past_data, current_data, underdrivable_cfg);

         FeaturesCalculator::Over_Nondr_Feature_Config_T overdrivable_cfg{};
         FeaturesCalculator::get_over_nondr_config(overdrivable_cfg);
         FeaturesCalculator::calculate_over_nondr_bins_proportion(subsegment.past_data, current_data, overdrivable_cfg);

         // assign data
         subsegment.past_data.set_means(means);
         subsegment.past_data.set_maxes(maxes);
      }

      void assign_features(Subsegment_T &subsegment)
      {
         subsegment.features.detections_number      = static_cast<float>(subsegment.past_data.get_detections_sum());
         const Signals_T means                      = subsegment.past_data.get_means();
         subsegment.features.rcs_recur_mean         = means.get_rcs();
         subsegment.features.z_scs_abs_ewma025_mean = means.get_z_scs_abs_ewma025();
         subsegment.features.z_scs_abs_recur_mean   = means.get_z_scs_abs_recur();
         subsegment.features.z_scs_abs_max          = subsegment.past_data.get_maxes().get_z_scs_abs();
         subsegment.features.height_under_nondr_bins_proportion =
            subsegment.past_data.get_height_bins_under_nondr().get_dets_freq_bins_mean();
         subsegment.features.height_over_nondr_bins_proportion =
            subsegment.past_data.get_height_bins_over_nondr().get_dets_freq_bins_mean();
         subsegment.features.detection_density =
            static_cast<float>(subsegment.past_data.get_detections_sum()) / static_cast<float>(subsegment.past_data.get_age());
      }
   }
}