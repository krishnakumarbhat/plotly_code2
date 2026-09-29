/*===================================================================================\
 * FILE: ocg_calibrations.cpp
 *====================================================================================
 * Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains function definition for Initialize_OCG_Calibrations.
 *
 * Applicable Standards (in order of precedence: highest first):
 *   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *   ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*===================================================================================*/

#include "ocg_calibrations.h"
#include "rspp_sensor_type.h"

namespace ocg
{
   /*===========================================================================*\
   * FUNCTION: Initialize_OCG_Calibrations()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * OCG_Calibrations_T &calibrations
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function initializes default values for calibrations
   *
   * PRECONDITIONS:
   * To be called during OCG initialization
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Initialize_OCG_Calibrations(
       OCG_Calibrations_T &calibrations)
   {
      calibrations.underdrive_max_comp_range_rate = 1.0F;
      calibrations.underdrive_hypothesis_height_can_pass = 4.0F;
      calibrations.underdrive_hypothesis_height_is_likely_to_pass = 2.5F;
      calibrations.underdrive_hypothesis_height_can_not_pass_upper = 1.5F;
      calibrations.underdrive_hypothesis_height_can_not_pass_lower = -3.0F;
      calibrations.underdrive_hypothesis_slope_can_pass = 0.17F;
      calibrations.underdrive_hypothesis_slope_is_likely_to_pass = 0.15F;
      calibrations.underdrive_hypothesis_slope_can_not_pass_upper = 0.13F;
      calibrations.underdrive_hypothesis_slope_can_not_pass_lower = -0.02F;
      calibrations.underdrive_max_lat_posn = 15.0F;
      calibrations.underdrive_small_curvature_th = 1e-4F;
      calibrations.underdrive_slow_moving_host = 1e-2F;
      calibrations.underdrive_use_front_center_sensors = true;
      calibrations.underdrive_use_front_side_sensors = false;
      calibrations.underdrive_filter_duplicate_detections = false;
      calibrations.underdrive_filter_negative_elevation = false;
      calibrations.underdrive_az_conf_th = 2;
      calibrations.underdrive_el_conf_th = 3;
      calibrations.underdrive_time_zone_crit_hi = 2.4F;
      calibrations.underdrive_time_zone_crit_med = 4.8F;
      calibrations.underdrive_time_zone_crit_low = 6.0F;
      calibrations.underdrive_slow_host_speed_th = 12.5F;
      calibrations.underdrive_lateral_weight_factor = 0.5f;

      // Define sufficient probabilities for different criticality zones to set different overhead suspicious levels
      calibrations.underdrive_max_slope_vs_height_index_diff = 2;

      // Highly critical zones
      calibrations.underdrive_breakpoints_height_can_pass_zoneHi_lev2[0] = 0.95F;
      calibrations.underdrive_breakpoints_height_can_pass_zoneHi_lev2[1] = 0.9F;
      calibrations.underdrive_breakpoints_height_can_pass_zoneHi_lev2[2] = 0.7F;
      calibrations.underdrive_breakpoints_height_can_pass_zoneHi_lev2[3] = 0.1F;

      calibrations.underdrive_breakpoints_slope_can_pass_zoneHi_lev2[0] = 0.0F;
      calibrations.underdrive_breakpoints_slope_can_pass_zoneHi_lev2[1] = 0.05F;
      calibrations.underdrive_breakpoints_slope_can_pass_zoneHi_lev2[2] = 0.7F;
      calibrations.underdrive_breakpoints_slope_can_pass_zoneHi_lev2[3] = 0.85F;

      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneHi_lev2[0] = 0.95F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneHi_lev2[1] = 0.85F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneHi_lev2[2] = 0.3F;

      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneHi_lev2[0] = 0.1F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneHi_lev2[1] = 0.85F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneHi_lev2[2] = 0.9F;

      calibrations.underdrive_th_p_can_not_pass_zoneHi_lev2 = 0.1F;

      calibrations.underdrive_high_crit_zone_is_likely_to_pass_min_number_of_dets = 10.0F;

      // Medium critical zone
      calibrations.underdrive_breakpoints_height_can_pass_zoneMed_lev2[0] = 0.8F;
      calibrations.underdrive_breakpoints_height_can_pass_zoneMed_lev2[1] = 0.6F;
      calibrations.underdrive_breakpoints_height_can_pass_zoneMed_lev2[2] = 0.15F;

      calibrations.underdrive_breakpoints_slope_can_pass_zoneMed_lev2[0] = 0.15F;
      calibrations.underdrive_breakpoints_slope_can_pass_zoneMed_lev2[1] = 0.6F;
      calibrations.underdrive_breakpoints_slope_can_pass_zoneMed_lev2[2] = 0.75F;

      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev2[0] = 0.95F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev2[1] = 0.85F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev2[2] = 0.3F;

      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev2[0] = 0.2F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev2[1] = 0.85F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev2[2] = 0.9F;

      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev1[0] = 0.95F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev1[1] = 0.8F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev1[2] = 0.7F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev1[3] = 0.15F;

      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev1[0] = 0.0F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev1[1] = 0.2F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev1[2] = 0.6F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev1[3] = 0.8F;

      calibrations.underdrive_th_p_can_not_pass_zoneMed_lev2 = 0.2F;
      calibrations.underdrive_th_p_can_not_pass_zoneMed_lev1 = 0.3F;

      // Low critical zones
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneLow_lev1[0] = 0.9F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneLow_lev1[1] = 0.6F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneLow_lev1[2] = 0.2F;
      calibrations.underdrive_breakpoints_height_is_likely_to_pass_zoneLow_lev1[3] = 0.15F;

      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneLow_lev1[0] = 0.0F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneLow_lev1[1] = 0.1F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneLow_lev1[2] = 0.6F;
      calibrations.underdrive_breakpoints_slope_is_likely_to_pass_zoneLow_lev1[3] = 0.8F;

      calibrations.underdrive_th_p_can_not_pass_zoneLow_lev1 = 0.3F;

      // Tunnel checks
      calibrations.underdrive_tunnel_weak_min_is_likely_to_pass_mean_val = 0.4F;
      calibrations.underdrive_tunnel_weak_max_can_not_pass_mean_val = 0.1F;

      calibrations.underdrive_tunnel_strict_min_is_likely_to_pass_mean_val = 0.5F;
      calibrations.underdrive_tunnel_strict_max_can_not_pass_mean_val_med_zone = 0.05F;

      calibrations.underdrive_tunnel_strict_max_can_not_pass_mean_val_high_zone = 0.02F;

      // Forgeting factor
      calibrations.underdrive_forgeting_factor_furthest_ranges = 0.93F;
      calibrations.underdrive_forgeting_factor_far_ranges = 0.95F;
      calibrations.underdrive_forgeting_factor_medium_ranges = 0.99F;
      calibrations.underdrive_forgeting_factor_short_ranges = 0.995F;

      calibrations.underdrive_forgeting_factor_ranges_limits[0] = 120.0F;
      calibrations.underdrive_forgeting_factor_ranges_limits[1] = 80.0F;
      calibrations.underdrive_forgeting_factor_ranges_limits[2] = 55.0F;

      // Probability tests
      calibrations.underdrive_t_test_var_iter_min_num_dets = 2.0F;
      calibrations.underdrive_t_test_var_iter_min_sample_var_th = 1e-5F;

      calibrations.underdrive_t_test_var_slope_min_input_sample_var = 10.0F;
      calibrations.underdrive_t_test_var_slope_min_num_dets = 4.0F;
      calibrations.underdrive_t_test_var_slope_min_sample_var_th = 1e-5F;

      // Student t cdf aprox
      calibrations.underdrive_min_num_dets_for_approx_method = 30.0F;

      // Student t betanic approx
      calibrations.underdrive_sqrt_pi = 1.7724539F;
      calibrations.underdrive_pre_computed_factors[0] = 0.3333333F;
      calibrations.underdrive_pre_computed_factors[1] = 0.3000000F;
      calibrations.underdrive_pre_computed_factors[2] = 0.2380952F;
      calibrations.underdrive_pre_computed_factors[3] = 0.1944444F;
      calibrations.underdrive_pre_computed_factors[4] = 0.1636364F;
      calibrations.underdrive_pre_computed_factors[5] = 0.1410256F;
      calibrations.underdrive_pre_computed_factors[6] = 0.1238095F;
      calibrations.underdrive_pre_computed_factors[7] = 0.1102941F;
      calibrations.underdrive_pre_computed_factors[8] = 0.09941521F;
      calibrations.underdrive_pre_computed_factors[9] = 0.09047619F;
      calibrations.underdrive_pre_computed_factors[10] = 0.08300395F;
      calibrations.underdrive_pre_computed_factors[11] = 0.07666667F;
      calibrations.underdrive_pre_computed_factors[12] = 0.07122507F;
      calibrations.underdrive_pre_computed_factors[13] = 0.06650247F;
      calibrations.underdrive_pre_computed_factors[14] = 0.06236559F;
      calibrations.underdrive_max_iterations = 15U;
   }
}
