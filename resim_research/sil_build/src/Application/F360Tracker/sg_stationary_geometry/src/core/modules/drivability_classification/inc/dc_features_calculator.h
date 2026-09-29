/*===================================================================================*\
* FILE: dc_features_calculator.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declarations of functions used to calculate features
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_FEATURES_CALCULATOR_H
#define DC_FEATURES_CALCULATOR_H

#include <cmath>

#include "dc_past_data.h"
#include "dc_subsegment_detections.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class FeaturesCalculator
      {
        private:
         /**
          * @brief      Calculates value for the input_arg using linear function saturated on both sides
          *
          * @param[in]  input_arg - argument for the function
          * @param[in]  low_arg_thr - threshold below which, values will be saturated
          * @param[in]  high_arg_thr - threshold above which, values will be saturated
          * @param[in]  low_arg_saturation_value - value for the arguments in range (-Inf, low_arg_thr]
          * @param[in]  high_arg_saturation_value - value for the arguments in range [high_arg_thr, Inf)
          *
          * @return     float - the value of the saturated linear function
          **/
         static float calculate_slope_with_saturation(const float input_arg,
                                                      const float low_arg_thr,
                                                      const float high_arg_thr,
                                                      const float low_arg_saturation_value,
                                                      const float high_arg_saturation_value);

        public:
         struct Under_Nondr_Feature_Config_T
         {
            float z_thr_low;
            float z_thr_high;

            uint32_t low_age_thr; // [scans] - decrease dets importance below this age
            float low_age_dets_decrease;
            float close_range_thr;       // [m]
            float close_range_min_limit; // What is the dets importance at range = 0m
            bool eliminate_low_z_low_rcs;
            float low_z_low_rcs_thr;
            uint32_t no_activity; // [scans]
            bool skip_reflections_low_bin_low_range;
            float skip_reflections_thr;

            bool increase_weight_high_dets;
            float increase_weight_high_dets_low_z;
            float increase_weight_high_dets_high_z;
            float increase_weight_high_dets_low_weight;
            float increase_weight_high_dets_high_weight;
         };

         struct Over_Nondr_Feature_Config_T
         {
            float reflected_z_thr; // [m], reflacted detections are with negative z
            float over_gnd_z_thr;  // [m]

            float close_range_thr;  // [m]
            float far_range_thr;    // [m]
            float wide_factor;      // [-]
            float max_overdr_range; // [m], reflacted are with negative z

            bool increase_weight_high_dets;
            float increase_weight_high_dets_low_z;
            float increase_weight_high_dets_high_z;
            float increase_weight_high_dets_low_weight;
            float increase_weight_high_dets_high_weight;
         };
         /**
          * @brief      Fill Under_Nondr_Feature_Config_T structure
          *
          * @param[in]  cfg
          **/
         static void get_under_nondr_config(Under_Nondr_Feature_Config_T &cfg);

         /**
          * @brief      Fill Over_Nondr_Feature_Config_T structure
          *
          * @param[in]  cfg
          **/
         static void get_over_nondr_config(Over_Nondr_Feature_Config_T &cfg);

         /**
          * @brief      Calculates recursive mean given the previous value of the mean and a new value
          *
          * @param[in]  old_mean - previous value of the mean
          * @param[in]  num_of_values - number of values used to calculate new mean (i.e. including current_value)
          * @param[in]  current_value - a value to be included in the mean
          *
          * @return     new_mean - the value of the mean including the new value
          **/
         static float calculate_recursive_mean(const float old_mean, const float num_of_values, const float current_value);

         /**
          * @brief      Calculates recursive mean given the previous value of the mean and a value of the mean for current scan
          *
          * @param[in]  current_mean - value of the mean for current scan
          * @param[in]  current_num_of_values - number of values used to calculate current mean
          * @param[in]  old_mean - previous value of the mean
          * @param[in]  total_num_of_values - number of values used to calculate the new mean
          *
          * @return     new_mean - the value of the mean including the value of the mean from current scan
          **/
         static float calculate_recursive_per_scan_mean(const float current_mean,
                                                        const float current_num_of_values,
                                                        const float old_mean,
                                                        const float total_num_of_values);

         /**
          * @brief      Calculates Exponentially Weighted Moving Average
          *
          * @param[in]  decay_factor - weight of the current_mean
          * @param[in]  old_mean - previous value of the mean
          * @param[in]  current_mean - current value of the mean
          *
          * @return     float - the value of the mean including the value of the mean from current scan
          **/
         static float calculate_EWMA(const float decay_factor, const float old_mean, const float current_mean);

         /**
          * @brief      Calculates weighted mean
          *
          * @param[in]  weighted_sum - the weighted sum of elements
          * @param[in]  sum_of_weights
          *
          * @return     float - the weighted mean value
          **/
         static float calculate_weighted_mean(const float weighted_sum, const float sum_of_weights);

         /**
          * @brief      Calculates variance
          *
          * @param[in]  sum_of_values
          * @param[in]  sum_of_squared_values
          * @param[in]  mean_value
          * @param[in]  num_of_values
          *
          * @return     float - the value of the variance
          **/
         static float calculate_variance(const float sum_of_values,
                                         const float sum_of_squared_values,
                                         const float mean_value,
                                         const float num_of_values);

         /**
          * @brief      Calculates weighted variance
          *
          * @param[in]  weighted_sum_of_squared_values - the weighted sum of squares of elements
          * @param[in]  weighted_sum - the weighted sum of elements
          * @param[in]  weighted_mean - weighted mean of elements
          * @param[in]  sum_of_weights - sum of weights
          *
          * @return     weighted_variance - the weighted variance value
          **/
         static float calculate_weighted_variance(const float weighted_sum_of_squared_values,
                                                  const float weighted_sum,
                                                  const float weighted_mean,
                                                  const float sum_of_weights);

         /**
          * @brief      Calculates range
          *
          * @param[in]  value1 - the first value
          * @param[in]  value2 - the second value
          *
          * @return     float - the range
          **/
         static float calculate_range(const float value1, const float value2);

         /**
          * @brief      Calculates bins for the discrimination between underdrivable and nondrivable obstacles
          *
          * @param[out] past_data - past data of subsegment
          * @param[in]  current_data - current detctions used for features calculations
          * @param[in]  cfg - under/nondrivable feature config
          *
          **/
         static void calculate_under_nondr_bins_proportion(Past_Data_T &past_data,
                                                           const SubsegmentDetections_T &current_data,
                                                           const Under_Nondr_Feature_Config_T &cfg);

         /**
          * @brief      Calculates bins for the discrimination between overdrivable and nondrivable obstacles
          *
          * @param[out] past_data - past data of subsegment
          * @param[in]  current_data - current detctions used for features calculations
          * @param[in]  cfg - over/nondrivable feature config
          *
          **/
         static void calculate_over_nondr_bins_proportion(Past_Data_T &past_data,
                                                          const SubsegmentDetections_T &current_data,
                                                          const Over_Nondr_Feature_Config_T &cfg);
      };
   }
}
#endif