/******************************************************************************
 * Copyright 2024 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
#include <cmath>
#include <algorithm>
#include "f360_math.h"
#include "f360_cvt_estimate_trailer_length.h"

namespace f360_variant_A
{
   void Run_Length_Filter(
      const F360_Calibrations_T& calibrations,
      const bool valid_measurement_ekf_1,
      const bool valid_measurement_ekf_2,
      F360_CVT_State_T& cvt_state)
   {
      const float32_t k_max_veh_len = 34.50F; // Maximum vehicle length
      const float32_t k_min_veh_len = 10.0F;  // Maximum vehicle length
      const float32_t length_lp_gains[2] = { 0.1F, 0.005F };
      const float32_t trailer_front_overhang = 1.7F; // [m] How far the trailer extends infront of the kin pin
      const float32_t trailer_rear_overhang = 3.5F; // [m] How far the trailer extends behind the rear axle
      const float32_t length_lp_sig_slope = 0.5F;
      constexpr int32_t min_n_updates_low_pass = 35;

      // 1-joint length estimation
      if (valid_measurement_ekf_1)
      {
         const int32_t n_updates_over_min = cvt_state.one_link.n_updates - min_n_updates_low_pass;
         float32_t weight_low_pass = 1.0F / (F360_Expf(static_cast<float32_t>(n_updates_over_min) * -length_lp_sig_slope) + 1.0F);
         weight_low_pass = ((1.0F - weight_low_pass) * length_lp_gains[0]) + (length_lp_gains[1] * weight_low_pass);

         float32_t full_carriage_length_msmt = -cvt_state.one_link.joint_vcs_longpos + cvt_state.one_link.joint_dist_to_wheels + trailer_rear_overhang;
         full_carriage_length_msmt = std::min(k_max_veh_len, std::max(k_min_veh_len, full_carriage_length_msmt));

         cvt_state.one_link.full_vehicle_length = ((1.0F - weight_low_pass) * cvt_state.one_link.full_vehicle_length) + (weight_low_pass * full_carriage_length_msmt);
      }

      const float32_t full_trailer_length = cvt_state.one_link.full_vehicle_length + cvt_state.one_link.joint_vcs_longpos + trailer_front_overhang;
      cvt_state.one_link.joint_dist_to_center = (full_trailer_length / 2.0F) - trailer_front_overhang;
      cvt_state.one_link.trailer_length = full_trailer_length;
      cvt_state.one_link.trailer_width = calibrations.k_cvt_trailer_width;

      // 2-joint length estimation
      if (valid_measurement_ekf_2)
      {
         const int32_t num_updates_over_min = cvt_state.two_link.n_updates - min_n_updates_low_pass;
         float32_t weight_low_pass = 1.0F / (F360_Expf(static_cast<float32_t>(num_updates_over_min) * -length_lp_sig_slope) + 1.0F);
         weight_low_pass = ((1.0F - weight_low_pass) * length_lp_gains[0]) + (length_lp_gains[1] * weight_low_pass);

         float32_t full_carriage_length_msmt = -cvt_state.two_link.joint1_vcs_longpos + cvt_state.two_link.joint1_dist_to_wheels + cvt_state.two_link.joint2_dist_to_wheels + trailer_rear_overhang;
         full_carriage_length_msmt = std::min(k_max_veh_len, std::max(k_min_veh_len, full_carriage_length_msmt));

         cvt_state.two_link.full_vehicle_length = ((1.0F - weight_low_pass) * cvt_state.two_link.full_vehicle_length) + (weight_low_pass * full_carriage_length_msmt);
      }
      
      const float32_t first_trailer_length = trailer_front_overhang + cvt_state.two_link.joint1_dist_to_wheels;
      // second_trailer_length is total_length - b1 - a1 - b2
      const float32_t second_trailer_length = cvt_state.two_link.joint2_dist_to_wheels + trailer_rear_overhang;
      
      cvt_state.two_link.joint1_dist_to_center = (first_trailer_length / 2.0F) - trailer_front_overhang;
      cvt_state.two_link.trailer1_width = calibrations.k_cvt_trailer_width;
      cvt_state.two_link.trailer1_length = first_trailer_length;
      cvt_state.two_link.joint2_dist_to_center = (second_trailer_length / 2.0F) - trailer_front_overhang;
      cvt_state.two_link.trailer2_width = calibrations.k_cvt_trailer_width;
      cvt_state.two_link.trailer2_length = second_trailer_length;
   }
}
