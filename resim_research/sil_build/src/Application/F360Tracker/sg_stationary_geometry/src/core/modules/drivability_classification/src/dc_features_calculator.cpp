/*===================================================================================*\
* FILE: dc_features_calculator.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definitions of functions used to calculate features
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_features_calculator.h"

#include <algorithm>
#include <functional>

namespace sg
{
   namespace dc
   {
      float FeaturesCalculator::calculate_slope_with_saturation(const float input_arg,
                                                                const float low_arg_thr,
                                                                const float high_arg_thr,
                                                                const float low_arg_saturation_value,
                                                                const float high_arg_saturation_value)
      {
         float output_value{0.0F};
         if (input_arg > high_arg_thr)
         {
            output_value = high_arg_saturation_value;
         }
         else if (input_arg < low_arg_thr)
         {
            output_value = low_arg_saturation_value;
         }
         else
         {
            output_value =
               ((high_arg_saturation_value - low_arg_saturation_value) / (high_arg_thr - low_arg_thr)) * (input_arg - low_arg_thr)
               + low_arg_saturation_value;
         }
         return output_value;
      }

      float FeaturesCalculator::calculate_recursive_mean(const float old_mean, const float num_of_values, const float current_value)
      {
         float new_mean = current_value;
         if (num_of_values > 1.0F)
         {
            new_mean = ((num_of_values - 1.0F) * old_mean + current_value) / num_of_values;
         }
         return new_mean;
      }

      float FeaturesCalculator::calculate_recursive_per_scan_mean(const float current_mean,
                                                                  const float current_num_of_values,
                                                                  const float old_mean,
                                                                  const float total_num_of_values)
      {
         assert(total_num_of_values > 0.0F);
         assert((current_num_of_values >= 0.0F) && (current_num_of_values <= total_num_of_values));
         const float new_weight = current_num_of_values / total_num_of_values;
         return new_weight * current_mean + (1.0F - new_weight) * old_mean;
      }

      float FeaturesCalculator::calculate_EWMA(const float decay_factor, const float old_mean, const float current_mean)
      {
         assert((decay_factor >= 0.0F) && (decay_factor <= 1.0F));
         return decay_factor * current_mean + (1.0F - decay_factor) * old_mean;
      }

      float FeaturesCalculator::calculate_weighted_mean(const float weighted_sum, const float sum_of_weights)
      {
         assert(sum_of_weights > 0.0F);
         return weighted_sum / sum_of_weights;
      }

      float FeaturesCalculator::calculate_variance(const float sum_of_values,
                                                   const float sum_of_squared_values,
                                                   const float mean_value,
                                                   const float num_of_values)
      {
         assert(num_of_values > 0.0F);
         assert(sum_of_squared_values >= 0.0F);
         return std::max(0.0F, sum_of_squared_values + mean_value * (num_of_values * mean_value - 2.0F * sum_of_values))
                / num_of_values;
      }

      float FeaturesCalculator::calculate_weighted_variance(const float weighted_sum_of_squared_values,
                                                            const float weighted_sum,
                                                            const float weighted_mean,
                                                            const float sum_of_weights)
      {
         assert(weighted_sum_of_squared_values >= 0.0F);
         assert(sum_of_weights > 0.0F);
         float weighted_variance{0.0F};
         if (sum_of_weights > 1.0F)
         {
            weighted_variance = (weighted_sum_of_squared_values - 2.0F * weighted_sum * weighted_mean
                                 + sum_of_weights * weighted_mean * weighted_mean)
                                / (sum_of_weights - 1.0F);
            if (weighted_variance < 0.0F)
            {
               weighted_variance = 0.0F;
            }
         }
         return weighted_variance;
      }

      float FeaturesCalculator::calculate_range(const float value1, const float value2)
      {
         return std::fabs(value1 - value2);
      }

      void FeaturesCalculator::get_under_nondr_config(FeaturesCalculator::Under_Nondr_Feature_Config_T &cfg)
      {
         cfg.z_thr_low                             = 3.0F;
         cfg.z_thr_high                            = 5.0F;
         cfg.low_age_thr                           = 20U;
         cfg.low_age_dets_decrease                 = 0.3F;
         cfg.close_range_thr                       = 40.0F;
         cfg.close_range_min_limit                 = 0.2F;
         cfg.eliminate_low_z_low_rcs               = false;
         cfg.low_z_low_rcs_thr                     = -10.0F;
         cfg.no_activity                           = 100U;
         cfg.skip_reflections_low_bin_low_range    = true;
         cfg.skip_reflections_thr                  = 30.0F;
         cfg.increase_weight_high_dets             = true;
         cfg.increase_weight_high_dets_low_z       = 15.0F;
         cfg.increase_weight_high_dets_high_z      = 40.0F;
         cfg.increase_weight_high_dets_low_weight  = 1.0F;
         cfg.increase_weight_high_dets_high_weight = 2.0F;
      }

      void FeaturesCalculator::calculate_under_nondr_bins_proportion(Past_Data_T &past_data,
                                                                     const SubsegmentDetections_T &current_data,
                                                                     const Under_Nondr_Feature_Config_T &cfg)
      {
         Signals_Height_Bins_T &height_bins = past_data.get_height_bins_under_nondr();

         // Simulate that denominator increase in time with no dets just up to cfg.free_demon_time. Shall have meaning in long range
         if ((past_data.get_age() - height_bins.get_age_last_update()) < cfg.no_activity)
         {
            height_bins.set_denominator(height_bins.get_denominator() + (past_data.get_age() - height_bins.get_age_last_update()));
         }
         else
         {
            height_bins.set_denominator(height_bins.get_denominator() + cfg.no_activity);
         }
         height_bins.set_age_last_update(past_data.get_age());

         // CalculateBinsDetsNo - we start function called so in MATLAB

         for (uint8_t index = 0U; index < current_data.num_associated_dets; index++)
         {
            // Increase low bin dets no, with weights
            if (current_data.z_scs_abs[index] < cfg.z_thr_low)
            {
               if ((!cfg.eliminate_low_z_low_rcs) || (current_data.rcs[index] > cfg.low_z_low_rcs_thr))
               {
                  const float range_min = *std::min_element(
                     std::begin(current_data.range), std::next(std::begin(current_data.range), current_data.num_associated_dets));
                  if ((!cfg.skip_reflections_low_bin_low_range) || (current_data.z_scs[index] > 0.0F)
                      || (range_min > cfg.skip_reflections_thr))
                  {
                     float factor_switch_value{0.0F};
                     if (past_data.get_age() > cfg.low_age_thr)
                     {
                        factor_switch_value = 1.0F;
                     }
                     else
                     {
                        factor_switch_value = cfg.low_age_dets_decrease;
                     }
                     const float ramp_sat_value =
                        calculate_slope_with_saturation(range_min, 0.0F, cfg.close_range_thr, cfg.close_range_min_limit, 1.0F);

                     height_bins.set_low_bin_detections_sum(height_bins.get_low_bin_detections_sum()
                                                            + (factor_switch_value * ramp_sat_value));
                  }
               }
            }
            // Increase high bin dets no
            if (current_data.z_scs_abs[index] > cfg.z_thr_high)
            {
               float weighted_det{1.0F};
               if (cfg.increase_weight_high_dets)
               {
                  weighted_det = calculate_slope_with_saturation(
                     current_data.z_scs_abs[index], cfg.increase_weight_high_dets_low_z, cfg.increase_weight_high_dets_high_z,
                     cfg.increase_weight_high_dets_low_weight, cfg.increase_weight_high_dets_high_weight);
               }
               height_bins.set_high_bin_detections_sum(height_bins.get_high_bin_detections_sum() + weighted_det);
            }
         }

         // Calculate features
         float low_bin{0.0F};
         float high_bin{0.0F};
         if (height_bins.get_denominator() > 0U)
         {
            low_bin  = height_bins.get_low_bin_detections_sum() / static_cast<float>(height_bins.get_denominator());
            high_bin = height_bins.get_high_bin_detections_sum() / static_cast<float>(height_bins.get_denominator());
         }

         if (low_bin + high_bin > 0.0F)
         {
            height_bins.set_dets_freq_bins_mean(low_bin / (low_bin + high_bin));
         }
         else
         {
            height_bins.set_dets_freq_bins_mean(0.5F);
         }
      }

      void FeaturesCalculator::get_over_nondr_config(FeaturesCalculator::Over_Nondr_Feature_Config_T &cfg)
      {
         cfg.reflected_z_thr = -0.2F;
         cfg.over_gnd_z_thr  = 0.4F;

         cfg.close_range_thr  = 10.0F;
         cfg.far_range_thr    = 20.0F;
         cfg.wide_factor      = 1.3F;
         cfg.max_overdr_range = 25.0F;

         cfg.increase_weight_high_dets             = true;
         cfg.increase_weight_high_dets_low_z       = 1.0F;
         cfg.increase_weight_high_dets_high_z      = 20.0F;
         cfg.increase_weight_high_dets_low_weight  = 1.2F;
         cfg.increase_weight_high_dets_high_weight = 5.0F;
      }

      void FeaturesCalculator::calculate_over_nondr_bins_proportion(Past_Data_T &past_data,
                                                                    const SubsegmentDetections_T &current_data,
                                                                    const Over_Nondr_Feature_Config_T &cfg)
      {
         const float tolerance              = 1e-7F;
         Signals_Height_Bins_T &height_bins = past_data.get_height_bins_over_nondr();

         height_bins.set_denominator(past_data.get_age());

         const float range_min = *std::min_element(std::begin(current_data.range),
                                                   std::next(std::begin(current_data.range), current_data.num_associated_dets));

         for (uint8_t index = 0U; index < current_data.num_associated_dets; index++)
         {
            float z_thr_over_gnd;
            float z_thr_reflected;
            if (current_data.z_scs[index] > 0.0F)
            {
               z_thr_reflected = 0.0F;
               z_thr_over_gnd  = calculate_slope_with_saturation(range_min, cfg.close_range_thr, cfg.far_range_thr,
                                                                 cfg.over_gnd_z_thr, cfg.wide_factor * cfg.over_gnd_z_thr);
            }
            else
            {
               z_thr_reflected = calculate_slope_with_saturation(range_min, cfg.close_range_thr, cfg.far_range_thr,
                                                                 cfg.reflected_z_thr, cfg.wide_factor * cfg.reflected_z_thr);
               z_thr_over_gnd  = 0.0F;
            }

            // Increase low bin dets number
            if ((range_min <= cfg.max_overdr_range) && (current_data.z_scs[index] < z_thr_over_gnd)
                && (current_data.z_scs[index] > z_thr_reflected))
            {
               height_bins.set_low_bin_detections_sum(height_bins.get_low_bin_detections_sum() + 1.0F);
            }
            else // Increase high bin dets number
            {
               float z_weight{1.0F};
               if (cfg.increase_weight_high_dets)
               {
                  z_weight = calculate_slope_with_saturation(current_data.z_scs_abs[index], cfg.increase_weight_high_dets_low_z,
                                                             cfg.increase_weight_high_dets_high_z,
                                                             cfg.increase_weight_high_dets_low_weight,
                                                             cfg.increase_weight_high_dets_high_weight);
               }

               const float high_arg_threshold        = 100.0F;
               const float low_arg_saturation_value  = 1.0F;
               const float high_arg_saturation_value = 4.0F;
               const float x_weight = calculate_slope_with_saturation(range_min, cfg.max_overdr_range, high_arg_threshold,
                                                                      low_arg_saturation_value, high_arg_saturation_value);

               const float conf_elev_weight_high                = 2.0F;
               const float conf_elev_weight_low                 = 1.0F;
               const int8_t confid_elevation_with_higher_weight = 1;
               const float conf_elev_weight = (current_data.confid_elevation[index] == confid_elevation_with_higher_weight
                                                  ? conf_elev_weight_high
                                                  : conf_elev_weight_low);

               height_bins.set_high_bin_detections_sum(height_bins.get_high_bin_detections_sum()
                                                       + z_weight * x_weight * conf_elev_weight);
            }

            (void) z_thr_over_gnd;  // MISRA
            (void) z_thr_reflected; // MISRA
         }

         (void) range_min; // MISRA

         // Calculate features
         float low_bin{0.0F};
         float high_bin{0.0F};
         if (height_bins.get_denominator() > 0U)
         {
            low_bin  = height_bins.get_low_bin_detections_sum() / static_cast<float>(height_bins.get_denominator());
            high_bin = height_bins.get_high_bin_detections_sum() / static_cast<float>(height_bins.get_denominator());
         }

         if ((low_bin + high_bin) > tolerance)
         {
            height_bins.set_dets_freq_bins_mean(low_bin / (low_bin + high_bin));
         }
         else
         {
            height_bins.set_dets_freq_bins_mean(0.5F);
         }
      }
   }
}