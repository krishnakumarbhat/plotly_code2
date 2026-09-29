/*===================================================================================*\
* FILE:  f360_fine_cluster_gate.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the definition of the function Fine_Cluster_Gate() which can be used
* to check if two clusters are close to each other in position. The two clusters may be of
* different age and the possible position difference of the clusters caused by this is 
* accounted for by utilizing the range rate of the older cluster together with the age 
* difference
* 
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_math.h"
#include "f360_fine_cluster_gate.h"
#include "f360_is_pos_dist_inside_ellipse.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   static bool Calc_Dist_Gate_Of_Two_Clusters(
      const F360_Calibrations_T& calib,
      const F360_Cluster_T& cluster_older,
      const F360_Cluster_T& cluster_newer,
      const float32_t rdot_interval_older,
      const float32_t n_aliased_older,
      const float32_t rdot_interval_newer,
      const float32_t n_aliased_newer
   );

   bool Fine_Cluster_Gate(
      const F360_Calibrations_T& calib,
      const F360_Cluster_T& cluster1,
      const F360_Cluster_T& cluster2,
      const float32_t rdot_interval_1,
      const float32_t rdot_interval_2,
      const float32_t n_aliased_1,
      const float32_t n_aliased_2
   )
   {
      bool f_success;

      if (cluster1.time_since_cluster_updated > cluster2.time_since_cluster_updated)
      {
         f_success = Calc_Dist_Gate_Of_Two_Clusters(calib, cluster1, cluster2, rdot_interval_1, n_aliased_1, rdot_interval_2, n_aliased_2);
      }
      else
      {
         f_success = Calc_Dist_Gate_Of_Two_Clusters(calib, cluster2, cluster1, rdot_interval_2, n_aliased_2, rdot_interval_1, n_aliased_1);
      }

      return f_success;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Dist_Gate_Of_Two_Clusters()
   *===========================================================================
   * RETURN VALUE:
   * float32_t radial_gate - the calculated radial gate size
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib
   * const F360_Cluster_T& cluster_older
   * const F360_Cluster_T& cluster_newer
   * const float32_t rdot_interval_older
   * const float32_t n_aliased_older
   * const float32_t rdot_interval_newer
   * const float32_t n_aliased_newer
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
   * Calculates the radial gate size. The base radial gate is taken from calibration.
   * For fast moving clusters (abs_mean_rdot_comp > fast_moving_thresh), the gate is
   * increased with a range-dependent component.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Calc_Dist_Gate_Of_Two_Clusters(
      const F360_Calibrations_T& calib,
      const F360_Cluster_T& cluster_older,
      const F360_Cluster_T& cluster_newer,
      const float32_t rdot_interval_older,
      const float32_t n_aliased_older,
      const float32_t rdot_interval_newer,
      const float32_t n_aliased_newer
   )
   {
      const float32_t range = F360_Get_Hypotenuse((cluster_older.vcs_position_x + cluster_newer.vcs_position_x) * 0.5F, 
         (cluster_older.vcs_position_y + cluster_newer.vcs_position_y) * 0.5F);

      const float32_t delta_time = cluster_older.time_since_cluster_updated - cluster_newer.time_since_cluster_updated;
      const float32_t temp_rdot_comp_older = cluster_older.rep_rdotcomp + (n_aliased_older * rdot_interval_older);
      const float32_t trk_az_rep_cos = cluster_older.cos_vcs_az;
      const float32_t trk_az_rep_sin = cluster_older.sin_vcs_az;

      const float32_t rdot_dist = delta_time * temp_rdot_comp_older;
      const float32_t delta_x = cluster_older.vcs_position_x + rdot_dist * trk_az_rep_cos - cluster_newer.vcs_position_x;
      const float32_t delta_y = cluster_older.vcs_position_y + rdot_dist * trk_az_rep_sin - cluster_newer.vcs_position_y;

      const float32_t temp_rdot_comp_newer = cluster_newer.rep_rdotcomp + (n_aliased_newer * rdot_interval_newer);
      const float32_t abs_mean_rdot_comp = std::abs(temp_rdot_comp_newer + temp_rdot_comp_older) * 0.5F;

      /* Position gating.
      * Tilted ellipse is used in world coordinates for gating. Elipse is rotated by old cluster azimuth with
      * the(minor) axis, in the world - az direction and the(major) axis, orthogonal to world - az, has a size which increases with
      * time difference between the two clusters. Center of elipse is considered as old cluster's position.
      * Checking distance is between old cluster to new cluster with time difference consideration.
      * Radial gate size is set and increases for clusters with high range rate.
      * Cross-radial gate size increases with range due to azimuth uncertainty (faster in closer ranges) and accounts for movement during
      * update-time difference between clusters.
      * Gates are adjusted for close range region beside host - cross radial gate is increased to capture objects moving fast parallel to host
      * and radial gate is decreased to compensate total ellipse size.
      */

      // Calculate Radial Gate
      float32_t radial_gate = Calc_Radial_Gate(calib, abs_mean_rdot_comp, range);

      // Adjust Radial gate and maximum cross-radial speed for close range region beside host
      float32_t max_cross_radial_speed = calib.k_default_max_cross_radial_speed;
      if (range < calib.k_range_thr_for_az_dependent_adj) // k_range_thr_for_az_dependent_adj = 8.0
      {
         Close_Range_Beside_Host_Adjustment(calib, range, trk_az_rep_sin, radial_gate, max_cross_radial_speed);
      }

      // Calculate Cross-Radial Gate
      const float32_t cross_radial_gate = Calc_Cross_Radial_Gate(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);

      const bool f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
         cross_radial_gate, radial_gate);

      return f_success;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Radial_Gate()
   *===========================================================================
   * RETURN VALUE:
   * float32_t radial_gate - the calculated radial gate size
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib
   * const float32_t abs_mean_rdot_comp - absolute mean compensated range rate
   * const float32_t range - mean range of the two clusters
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
   * Calculates the radial gate size. The base radial gate is taken from calibration.
   * For fast moving clusters (abs_mean_rdot_comp > fast_moving_thresh), the gate is
   * increased with a range-dependent component.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calc_Radial_Gate(
      const F360_Calibrations_T& calib,
      const float32_t abs_mean_rdot_comp,
      const float32_t range
   )
   {
       const bool f_fast_moving_radial = abs_mean_rdot_comp > calib.fast_moving_thresh;
       float32_t radial_gate = f_fast_moving_radial ? 2.0F * calib.k_radial_gate : calib.k_radial_gate;


       const float32_t range_threshold_for_radial_gate_extension = 70.0F;
       if ((range > range_threshold_for_radial_gate_extension) && (f_fast_moving_radial))
       {
           const float32_t max_radial_gate_increase = 0.75F;
           const float32_t range_for_max_radial_gate_extension = 100.0F;
           radial_gate += F360_Linear_Equation_With_Saturation(range, range_threshold_for_radial_gate_extension, range_for_max_radial_gate_extension, 0.0F, max_radial_gate_increase);
       }

      return radial_gate;
   }

   /*===========================================================================*\
   * FUNCTION: Close_Range_Beside_Host_Adjustment()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib
   * const float32_t range - mean range of the two clusters
   * const float32_t trk_az_rep_sin - sine of the older cluster's azimuth
   * float32_t& radial_gate -  radial gate to be adjusted
   * float32_t& max_cross_radial_speed - (in/out) max cross-radial speed to be adjusted
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
   * Adjusts the radial gate and maximum cross-radial speed for clusters located
   * at close range and beside the host vehicle. This increases
   * the cross-radial gate (to allow for fast-moving objects parallel to the host)
   * and decreases the radial gate (to compensate for the total ellipse size).
   * The adjustment is based on both range and azimuth, with a smooth transition.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Close_Range_Beside_Host_Adjustment(
       const F360_Calibrations_T& calib,
       const float32_t range,
       const float32_t trk_az_rep_sin,
       float32_t& radial_gate,
       float32_t& max_cross_radial_speed)
   {
       // Minimum absolute sine of azimuth for when to start applying the adjustment (i.e. for abs(sin(az)) below this value, adjustment will be 0).
       const float32_t min_abs_sin_az_for_adj = 0.4F; // This corresponds to around 23.6 degrees of azimuth

       // Absolute sine of azimuth for when to apply the full adjustment (i.e. for abs(sin(az)) above this value, adjustment will be at maximum).
       const float32_t abs_sin_az_for_full_adj = 0.8F; //  This corresponds to around 53.1 degrees of azimuth.

       // The adjustment is based on both azimuth and range, with a smooth transition.
       // It is at maximum for objects close to the host and beside the host (i.e. high absolute sine of azimuth) and smoothly reduces to 0 when going away from this region in terms of both azimuth and range.
       const float32_t abs_sin_vcs_az = std::abs(trk_az_rep_sin);
       const float32_t azimuth_factor = F360_Linear_Equation_With_Saturation(abs_sin_vcs_az, min_abs_sin_az_for_adj, abs_sin_az_for_full_adj, 0.0F, 1.0F);
       const float32_t range_factor = F360_Linear_Equation_With_Saturation(range,
           calib.k_range_thr_for_az_dependent_adj, // k_range_thr_for_az_dependent_adj = 8.0
           calib.k_range_thr_for_full_az_dependent_adj, // k_range_thr_for_full_az_dependent_adj = 4.0
           0.0F,
           1.0F); 
       const float32_t combined_factor = azimuth_factor * range_factor; // 0 when outside of adjustment region, 1 when in the region close to and beside the host, smoothly increasing from 0 to 1 when approaching this region
       // see https://jiraprod.aptiv.com/browse/DFD-3378 for visualization of the region where adjustment is applied and the effect of the adjustment.

       // Increase the maximum cross-radial speed in the close range region beside the host to increase the cross-radial gate therein
       // This allows merging of clusters coming from the high-speed parallel moving objects in this region.
       max_cross_radial_speed += combined_factor * (calib.k_increased_max_cross_radial_speed - calib.k_default_max_cross_radial_speed); // k_increased_max_cross_radial_speed = 40.0, k_default_max_cross_radial_speed = 10.0

       // Decrease the radial gate to compensate for the total ellipse size increase caused by the cross-radial gate increase.
       const float32_t decreased_radial_gate = 2.0F;
       if (radial_gate > decreased_radial_gate)
       {
           radial_gate += combined_factor * (decreased_radial_gate - radial_gate);
       }
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Cross_Radial_Gate_Short_Range()
   *===========================================================================
   * RETURN VALUE:
   * float32_t cross_radial_gate - the calculated cross-radial gate size for short range
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib - calibration parameters
   * const float32_t range - mean range of the two clusters
   * const float32_t max_cross_radial_speed - maximum cross-radial speed
   * const float32_t delta_time - time difference between clusters
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
   * Calculates the cross-radial gate size for clusters at short range.
   * The gate size is based on azimuth scaling and movement uncertainty,
   * with saturation for high crossing speeds. All parameters are sourced
   * from calibration.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calc_Cross_Radial_Gate_Short_Range(
       const F360_Calibrations_T& calib,
       const float32_t range,
       const float32_t max_cross_radial_speed,
       const float32_t delta_time)
   {
       // Calculate cross-radial movement uncertainty based on maximum cross-radial speed and time difference with saturation. If no close range aimuth dependent adjustment is applied, maximum cross-radial speed will be at default level (k_default_max_cross_radial_speed)
       // k_default_max_cross_radial_speed = 10.0, k_increased_max_cross_radial_speed = 40.0, k_cross_radial_movement_default_saturation = 2.0, k_cross_radial_movement_increased_saturation = 4.0
       const float32_t cross_radial_movement_saturation = F360_Linear_Equation_With_Saturation(max_cross_radial_speed,
           calib.k_default_max_cross_radial_speed, // 10.0
           calib.k_increased_max_cross_radial_speed, // 40.0
           calib.k_cross_radial_movement_default_saturation, // 2.0
           calib.k_cross_radial_movement_increased_saturation); // 4.0
       const float32_t cross_radial_movement_uncertainty = std::min(max_cross_radial_speed * delta_time, cross_radial_movement_saturation);

       const float32_t azimuth_uncertainty = calib.k_az_scaling_factor_close_range * range; // k_az_scaling_factor_close_range = 0.13

       const float32_t cross_radial_gate = azimuth_uncertainty + cross_radial_movement_uncertainty;

       return cross_radial_gate;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Cross_Radial_Gate_Long_Range()
   *===========================================================================
   * RETURN VALUE:
   * float32_t cross_radial_gate - the calculated cross-radial gate size for long range
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib - calibration parameters
   * const float32_t abs_mean_rdot_comp - absolute mean compensated range rate
   * const float32_t range - mean range of the two clusters
   * const float32_t max_cross_radial_speed - maximum cross-radial speed
   * const float32_t delta_time - time difference between clusters
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
   * Calculates the cross-radial gate size for cluster association at long range.
   * The gate size is based on azimuth noise, target size (interpolated from speed),
   * and movement uncertainty. All parameters are sourced from calibration.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calc_Cross_Radial_Gate_Long_Range(
       const F360_Calibrations_T& calib,
       const float32_t abs_mean_rdot_comp,
       const float32_t range,
       const float32_t max_cross_radial_speed,
       const float32_t delta_time)
   {
       // Calculate cross-radial target size, azimuth noise and movement uncertainty
       const float32_t cross_radial_target_size = F360_Linear_Equation_With_Saturation(abs_mean_rdot_comp,
           calib.k_slow_moving_speed_thr_for_cross_radial_gate, // 4.0
           calib.k_fast_moving_speed_thr_for_cross_radial_gate, // 8.0
           calib.k_slow_moving_size_for_cross_radial_gate, // 1.5
           calib.k_fast_moving_size_for_cross_radial_gate); // 3.0
       const float32_t azimuth_uncertainty = calib.k_az_scaling_factor_long_range * range; // k_az_scaling_factor_long_range = RAD2DEG(2.0) = 0.034906585
       const float32_t cross_radial_movement_uncertainty = std::min(max_cross_radial_speed * delta_time, calib.k_cross_radial_movement_default_saturation); // k_cross_radial_movement_default_saturation = 2.0

       const float32_t cross_radial_gate = azimuth_uncertainty + cross_radial_target_size + cross_radial_movement_uncertainty;

       return cross_radial_gate;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Cross_Radial_Gate()
   *===========================================================================
   * RETURN VALUE:
   * float32_t cross_radial_gate - the calculated cross-radial gate size
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib - calibration parameters
   * const float32_t abs_mean_rdot_comp - absolute mean compensated range rate
   * const float32_t range - mean range of the two clusters
   * const float32_t max_cross_radial_speed - maximum cross-radial speed
   * const float32_t delta_time - time difference between clusters
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
   * Calculates the cross-radial gate size for cluster association. The gate size
   * depends on range, range rate, maximum cross-radial speed, and time difference.
   * For close ranges, the gate is based on azimuth scaling and movement uncertainty.
   * For longer ranges, it is based on azimuth noise, target size, and movement uncertainty.
   * The function ensures smooth transition between close and long range logic.
   * Below 15 meters close range logic always applies.
   * Above 32 meters long range logic always applies.
   * Between 15 and 32 meters, there is smooth transition between the two logics.
   * Exact transitioning threshold depending on the speed of the object (i.e. for faster objects, close range logic will be applied up to longer range compared to slower objects).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calc_Cross_Radial_Gate(
       const F360_Calibrations_T& calib,
       const float32_t abs_mean_rdot_comp,
       const float32_t range,
       const float32_t max_cross_radial_speed,
       const float32_t delta_time)
   {
       float32_t cross_radial_gate;

       if (range < calib.k_hard_range_thr_for_close_range_logic) // k_hard_range_thr_for_close_range_logic = 15.7739630
       {
           // Short range logic 
           cross_radial_gate = Calc_Cross_Radial_Gate_Short_Range(calib, range, max_cross_radial_speed, delta_time);
       }
       else if (range > calib.k_hard_range_thr_for_long_range_logic) // k_hard_range_thr_for_long_range_logic = 31.5479259
       {
           // Long range logic
           cross_radial_gate = Calc_Cross_Radial_Gate_Long_Range(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);
       }
       else
       {
           // Speed-dependent smooth transition, 
           // k_slow_moving_speed_thr_for_cross_radial_gate = 4.0, k_fast_moving_speed_thr_for_cross_radial_gate = 8.0
           const float32_t range_threshold = F360_Linear_Equation_With_Saturation(abs_mean_rdot_comp,
              calib.k_slow_moving_speed_thr_for_cross_radial_gate, // 4.0
              calib.k_fast_moving_speed_thr_for_cross_radial_gate, // 8.0
              calib.k_hard_range_thr_for_close_range_logic, // 15.7739630
              calib.k_hard_range_thr_for_long_range_logic); // 31.5479259

           if (range < range_threshold)
           {
               cross_radial_gate = Calc_Cross_Radial_Gate_Short_Range(calib, range, max_cross_radial_speed, delta_time);
           }
           else
           {
               cross_radial_gate = Calc_Cross_Radial_Gate_Long_Range(calib, abs_mean_rdot_comp, range, max_cross_radial_speed, delta_time);
           }
       }

       return cross_radial_gate;
   }
}

