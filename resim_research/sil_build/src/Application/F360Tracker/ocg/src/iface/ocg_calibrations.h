/*===========================================================================*\
* FILE: ocg_calibrations.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains ocg_calibrations structure declaration
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef ocg_calibrations_H
#define ocg_calibrations_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

#include "ocg_reuse.h"
#include "rspp_sensor_type.h"
#include "ocg_underdrivability_type.h"

namespace ocg
{
   struct OCG_Calibrations_T
   {
      // Underdrivability
      float underdrive_max_comp_range_rate;                  // Maximum range-rate error from expected stationary range - rate
      float underdrive_hypothesis_height_can_pass;           // [m] CAN_PASS hypothesis height
      float underdrive_hypothesis_height_is_likely_to_pass;  // [m] IS_LIKELY_TO_PASS hypothesis height
      float underdrive_hypothesis_height_can_not_pass_upper; // [m] CAN_NOT_PASS_UPPER hypothesis height
      float underdrive_hypothesis_height_can_not_pass_lower; // [m] CAN_NOT_PASS_LOWER hypothesis height
      float underdrive_hypothesis_slope_can_pass;            // [dB/m] CAN_PASS hypothesis slope
      float underdrive_hypothesis_slope_is_likely_to_pass;   // [dB/m] IS_LIKELY_TO_PASS hypothesis slope
      float underdrive_hypothesis_slope_can_not_pass_upper;  // [dB/m] CAN_NOT_PASS_UPPER hypothesis slope
      float underdrive_hypothesis_slope_can_not_pass_lower;  // [dB/m] CAN_NOT_PASS_LOWER hypothesis slope
      float underdrive_max_lat_posn;                         // [m] maximal lateral position of detection
      float underdrive_small_curvature_th;                   // [m] minimal curvature of predicted host path to consider it as straight line
      float underdrive_slow_moving_host;                     // [m/s] maximal host speed to consider it as slow moving. If host speed is greater than this threshold zones are not updated.
      int8_t underdrive_az_conf_th;                          // [-] maximal detection azimuth confidence
      int8_t underdrive_el_conf_th;                          // [-] maximal detection elevation confidence
      bool underdrive_use_front_center_sensors;              // [-] used for picking detections. detections from center sensor will be used.
      bool underdrive_use_front_side_sensors;                // [-] used for picking detections. detections from forward right and left sensors will be used.
      
      bool underdrive_filter_duplicate_detections;           // [-] Used to remove duplicated detections with different elevation.
      bool underdrive_filter_negative_elevation;             // [-] Used to remove negative elevation detections
      float underdrive_lateral_weight_factor;                // [-] used for weighting detections infulence based on its lateral position inside the cell.


      // Zones ciricality level - time to arrive threshold
      float underdrive_time_zone_crit_hi;  // [s] High critical zones time to arrive threshold
      float underdrive_time_zone_crit_med; // [s] Medium critical zones time to arrive threshold
      float underdrive_time_zone_crit_low; // [s] Low critical zones time to arrive threshold
      float underdrive_slow_host_speed_th; // [m/s] Corresponds to 45kph. Threshold for slow host speed. For larger host speeds we use a time gap/TTC based approach to conpute where the different criticality zones starts. For slower host speeds we use a range based approach

      // Define sufficient probabilities for different criticality zones to set different overhead suspicious levels
      int32_t underdrive_max_slope_vs_height_index_diff; // [-] Maximum diff between slope and height breakepoint indexes.

      // High critical zone
      float underdrive_breakpoints_height_can_pass_zoneHi_lev2[NUM_BREAKPOINTS_CAN_PASS_ZONE_HIGH_LEV_2]; // [-] Breakepoints used for testing high critical zones strict CAN PASS height probability
      float underdrive_breakpoints_slope_can_pass_zoneHi_lev2[NUM_BREAKPOINTS_CAN_PASS_ZONE_HIGH_LEV_2];  // [-] Breakepoints used for testing high critical zones strict CAN PASS slope probability

      float underdrive_breakpoints_height_is_likely_to_pass_zoneHi_lev2[NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_HIGH_LEV_2]; // [-] Breakepoints used for testing high critical zones IS LIKELY TO PASS height probability
      float underdrive_breakpoints_slope_is_likely_to_pass_zoneHi_lev2[NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_HIGH_LEV_2];  // [-] Breakepoints used for testing high critical zones IS LIKELY TO PASS slope probability

      float underdrive_th_p_can_not_pass_zoneHi_lev2; // [-] Threshold used in testing of strict probabilities. If CAN_NOT_PASS probability is greater than threshold, test does not pass.

      float underdrive_high_crit_zone_is_likely_to_pass_min_number_of_dets; // [-] Minimal number of filtered detections to pass is_likely_to_pass test.

      // Medium critical zone
      float underdrive_breakpoints_height_can_pass_zoneMed_lev2[NUM_BREAKPOINTS_CAN_PASS_ZONE_MED_LEV2]; // [-] Breakepoints used for testing med critical zones strict CAN PASS height probability
      float underdrive_breakpoints_slope_can_pass_zoneMed_lev2[NUM_BREAKPOINTS_CAN_PASS_ZONE_MED_LEV2];  // [-] Breakepoints used for testing med critical zones strict CAN PASS height probability

      float underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev2[NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_MED_LEV2]; // [-] Breakepoints used for testing med critical zones strict IS LIKELY TO PASS height probability
      float underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev2[NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_MED_LEV2];  // [-] Breakepoints used for testing med critical zones strict IS LIKELY TO PASS slope probability

      float underdrive_breakpoints_height_is_likely_to_pass_zoneMed_lev1[NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_MED_LEV1]; // [-] Breakepoints used for testing med critical zones weak IS LIKELY TO PASS height probability
      float underdrive_breakpoints_slope_is_likely_to_pass_zoneMed_lev1[NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_MED_LEV1];  // [-] Breakepoints used for testing med critical zones weak IS LIKELY TO PASS slope probability

      float underdrive_th_p_can_not_pass_zoneMed_lev2; // [-] Threshold used in testing of strict probabilities. If CAN_NOT_PASS probability is greater than threshold, test does not pass.
      float underdrive_th_p_can_not_pass_zoneMed_lev1; // [-] Threshold used in testing of weak probabilities. If CAN_NOT_PASS probability is greater than threshold, test does not pass.

      // Low critical zones
      float underdrive_breakpoints_height_is_likely_to_pass_zoneLow_lev1[NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_LOW_LEV1]; // [-] Breakepoints used for testing med critical zones weak IS LIKELY TO PASS height probability
      float underdrive_breakpoints_slope_is_likely_to_pass_zoneLow_lev1[NUM_BREAKPOINTS_IS_LIKELY_TO_PASS_ZONE_LOW_LEV1];  // [-] Breakepoints used for testing med critical zones weak IS LIKELY TO PASS slope probability

      float underdrive_th_p_can_not_pass_zoneLow_lev1; // [-] Threshold used in testing of weak probabilities. If CAN_NOT_PASS probability is greater than threshold, test does not pass.

      // Tunnel checks
      float underdrive_tunnel_weak_min_is_likely_to_pass_mean_val;
      float underdrive_tunnel_weak_max_can_not_pass_mean_val;

      float underdrive_tunnel_strict_min_is_likely_to_pass_mean_val;
      float underdrive_tunnel_strict_max_can_not_pass_mean_val_med_zone;
      float underdrive_tunnel_strict_max_can_not_pass_mean_val_high_zone;

      // Forgetting factor
      float underdrive_forgeting_factor_furthest_ranges;  // [-] State forgeting factor for furthest zones (longitudinal position greater than underdrive_forgeting_factor_ranges_limits[0])
      float underdrive_forgeting_factor_far_ranges;       // [-] State forgeting factor for far zones (longitudinal position greater than underdrive_forgeting_factor_ranges_limits[1] and smaller than underdrive_forgeting_factor_ranges_limits[0])
      float underdrive_forgeting_factor_medium_ranges;    // [-] State forgeting factor for medium range zones (longitudinal position greater than underdrive_forgeting_factor_ranges_limits[2] and smaller than underdrive_forgeting_factor_ranges_limits[1])
      float underdrive_forgeting_factor_short_ranges;     // [-] State forgeting factor for short range zones (longitudinal position smaller than underdrive_forgeting_factor_ranges_limits[2])
      float underdrive_forgeting_factor_ranges_limits[3]; // [m] Ranges used for determining zone forgeting factor

      // Probability tests
      float underdrive_t_test_var_iter_min_num_dets;      // [-] Minimal number of dets for performing height probability test
      float underdrive_t_test_var_iter_min_sample_var_th; // [-] Minimal sample variance for performing height probability test

      float underdrive_t_test_var_slope_min_input_sample_var; // [-] Minimal filtered sample variance for performing slope probability test
      float underdrive_t_test_var_slope_min_num_dets;         // [-] Minimal number of dets for performing slope probability test
      float underdrive_t_test_var_slope_min_sample_var_th;    // [-] Minimal sample variance for performing slope probability test

      // Student t cdf approx
      float underdrive_min_num_dets_for_approx_method; // minimal number of detections to use Normal distribution rather than Student distribution

      // Student t betanic approx
      float underdrive_sqrt_pi;                                       // [-] precomputed factor, equal to sqrt(pi)
      float underdrive_pre_computed_factors[NUM_PRECOMPUTED_FACTORS]; // [-] precomputed factors used in Student t betanic approximation
      uint32_t underdrive_max_iterations;                             // [-] maximum number of iterations in approximation
   };

   void Initialize_OCG_Calibrations(
       OCG_Calibrations_T &calibrations);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
