/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_estimate_velocity_by_cloud.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Estimate_Velocity_By_Cloud()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_estimate_velocity_by_cloud.h"
#include "f360_norm_heading_angle.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Estimate_Velocity_By_Cloud()
   *===========================================================================
   * RETURN VALUE:
   * F360_Track_Init_T init_type
   *
   * PARAMETERS:
   * const F360_Detection_Hist_T& det_hist,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const F360_Cluster_T& cluster,
   * float32_t& longvel_by_cloud,
   * float32_t& latvel_by_cloud
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
   * This function assigns weights for each current and historical detection for cluster based on their azimuth confidence,
   * elevation confidence, super resolution and specific elevation zones. Then it performs IRLS (Iterative Reweighted Least
   * Squares) to find longvel_by_cloud and latvel_by_cloud. Finally it calculates the confidence of calculated velocities
   * based on average weight and azimuth spread of all the detections and number of inliers.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   CONF3_T Estimate_Velocity_By_Cloud(
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Cluster_T& cluster,
      float32_t& longvel_by_cloud,
      float32_t& latvel_by_cloud)
   {
      /* Collect data */
      int32_t num_dets = 0;
      float32_t max_az = -F360_PI;
      float32_t min_az = F360_PI;
      float32_t max_rdot_comp = -1000.0F;
      float32_t min_rdot_comp = 1000.0F;
      float32_t cos_vcs_az[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]{};
      float32_t sin_vcs_az[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]{};
      float32_t rdot_comp[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]{};
      float32_t base_weight[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]{};
      float32_t weights[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]{};
      float32_t azimuths[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]{};
      constexpr float32_t high_az_conf_weight_increment = 0.02F;
      constexpr float32_t low_or_med_az_conf_weight_increment = 0.04F;
      constexpr float32_t low_or_med_confid_elevation_weight_increment = 0.02F;
      constexpr float32_t f_super_res_weight_increment = 0.01F;
      constexpr float32_t abs_elev_above_10_deg_increment = 0.1F;
      constexpr float32_t abs_elev_between_7_and_10_deg_increment = 0.05F;

      for (int32_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         const F360_Detection_Hist_Data_T& det = det_hist.det_data[det_idx];

         max_rdot_comp = std::fmaxf(max_rdot_comp, det.rdot_comp);
         min_rdot_comp = std::fminf(min_rdot_comp, det.rdot_comp);

         float32_t weight = 0.01F;
         weight += det.time_since_meas * 20.0F * 0.002F; // Add 0.002 per 50ms
         weight += (det.az_conf == 1) ? high_az_conf_weight_increment : 0.0F;
         weight += (det.az_conf >= 2) ? low_or_med_az_conf_weight_increment : 0.0F;
         weight += (det.el_conf >= 2) ? low_or_med_confid_elevation_weight_increment : 0.0F;
         weight += (det.f_super_res) ? f_super_res_weight_increment : 0.0F;
         weight += (std::abs(det.elevation) >= F360_DEG2RAD(10.0F)) ? abs_elev_above_10_deg_increment : 0.0F;
         weight += ((std::abs(det.elevation) < F360_DEG2RAD(10.0F)) && (std::abs(det.elevation) > F360_DEG2RAD(7.0F))) ? abs_elev_between_7_and_10_deg_increment : 0.0F;

         const float32_t norm_az = Normalize_Heading_Angle(det.vcs_az, cluster.rep_vcs_az);
         if ((!det.f_FOV_edge) && (det.az_conf < 3))
         {
             max_az = std::fmaxf(max_az, norm_az);
             min_az = std::fminf(min_az, norm_az);
         }

         azimuths[num_dets] = norm_az;
         weights[num_dets] = 1.0F;
         base_weight[num_dets] = 1.0F / weight;
         cos_vcs_az[num_dets] = F360_Cosf(det.vcs_az);
         sin_vcs_az[num_dets] = F360_Sinf(det.vcs_az);
         rdot_comp[num_dets] = det.rdot_comp;
         num_dets++;
      }
      for (int32_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         const rspp_variant_A::Raw_Detection_T& det = raw_detections.detections[det_idx].raw;

         
         max_rdot_comp = std::fmaxf(max_rdot_comp, det_props[det_idx].range_rate_compensated);
         min_rdot_comp = std::fminf(min_rdot_comp, det_props[det_idx].range_rate_compensated);

         float32_t weight = 0.01F;
         weight += (det.confid_azimuth == 1) ? high_az_conf_weight_increment : 0.0F;
         weight += (det.confid_azimuth >= 2) ? low_or_med_az_conf_weight_increment : 0.0F;
         weight += (det.confid_elevation >= 2) ? low_or_med_confid_elevation_weight_increment : 0.0F;
         weight += (det.f_super_res) ? f_super_res_weight_increment : 0.0F;
         weight += (std::abs(det.elevation) >= F360_DEG2RAD(10.0F)) ? abs_elev_above_10_deg_increment : 0.0F;
         weight += ((std::abs(det.elevation) < F360_DEG2RAD(10.0F)) && (std::abs(det.elevation) > F360_DEG2RAD(7.0F))) ? abs_elev_between_7_and_10_deg_increment : 0.0F;

         const float32_t norm_az = Normalize_Heading_Angle(raw_detections.detections[det_idx].processed.vcs_az, cluster.rep_vcs_az);
         if ((!det_props[det_idx].f_FOV_edge) && (det.confid_azimuth < 3))
         {
             max_az = std::fmaxf(max_az, norm_az);
             min_az = std::fminf(min_az, norm_az);
         }

         azimuths[num_dets] = norm_az;
         weights[num_dets] = 1.0F;
         base_weight[num_dets] = 1.0F / weight;
         cos_vcs_az[num_dets] = raw_detections.detections[det_idx].processed.cos_vcs_az;
         sin_vcs_az[num_dets] = raw_detections.detections[det_idx].processed.sin_vcs_az;
         rdot_comp[num_dets] = det_props[det_idx].range_rate_compensated;
         num_dets++;
      }
      const float32_t cluster_range_sq = cluster.vcs_position_x * cluster.vcs_position_x + cluster.vcs_position_y * cluster.vcs_position_y;
      constexpr float32_t range_thold_for_higher_az_spread_req_sq = 150.0F * 150.0F; // 150m

      /* Verify solution */
      constexpr int32_t k_min_num_dets = 5;
      constexpr float32_t default_k_low_min_az_spread_deg = 1.5F;
      float32_t k_min_az_spread_deg = default_k_low_min_az_spread_deg;

      if (cluster.num_types_of_dets[1] > (cluster.num_types_of_dets[0] * 2))
      {
         constexpr float32_t k_high_az_spread_deg = 12.0F;
         k_min_az_spread_deg = k_high_az_spread_deg;
      }
      else if (cluster.num_types_of_dets[1] > cluster.num_types_of_dets[0])
      {
         constexpr float32_t k_mid_high_az_spread_deg = 8.0F;
         k_min_az_spread_deg = k_mid_high_az_spread_deg;
      }
      else if (((cluster.num_types_of_dets[1] * 2) > cluster.num_types_of_dets[0]) || (cluster_range_sq > range_thold_for_higher_az_spread_req_sq))
      {
         constexpr float32_t k_mid_low_az_spread_deg = 4.0F;
         k_min_az_spread_deg = k_mid_low_az_spread_deg;
      }
      else
      {
         //do nothing
      }

      CONF3_T estimation_confidence;
      const float32_t az_spread = max_az - min_az;
      if ((az_spread < F360_DEG2RAD(k_min_az_spread_deg)) || (num_dets < k_min_num_dets))
      {
         estimation_confidence = CONF3_NONE;
      }
      else
      {
         constexpr int32_t k_max_num_iter = 10;
         constexpr float32_t max_allowed_k_delta_multiplier = 3.0F;
         constexpr float32_t min_allowed_k_delta_multiplier = 0.8F;
         constexpr float32_t rep_rdotcomp_slope = 0.0628F;
         constexpr float32_t rep_rdotcomp_constant = 0.36F;

         const float32_t rep_rdotcomp_scaling_factor = std::abs(cluster.rep_rdotcomp) * rep_rdotcomp_slope + rep_rdotcomp_constant;
         const float32_t clamped_scaling_factor = std::fminf(max_allowed_k_delta_multiplier, std::fmaxf(min_allowed_k_delta_multiplier, rep_rdotcomp_scaling_factor));
         const float32_t k_delta = 0.07F * clamped_scaling_factor;

         bool f_valid_result = true;
         int32_t num_strong_inliers = 0;
         int32_t num_weak_inliers = 0;
         float32_t weights_sum = 0.0F;
         float32_t prev_longvel = 1000000.0F;
         float32_t prev_latvel = 1000000.0F;
         float32_t weak_inlier_azimuths[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]{};

         for (int32_t iter = 0; iter < k_max_num_iter; iter++)
         {
            /* Run RLS */
            float32_t Sx = 0.0F;
            float32_t Sy = 0.0F;
            float32_t Sxx = 0.0F;
            float32_t Sxy = 0.0F;
            float32_t Syy = 0.0F;
            for (int32_t i = 0; i < num_dets; i++)
            {
               Sx += cos_vcs_az[i] * rdot_comp[i] * weights[i] * base_weight[i];
               Sy += sin_vcs_az[i] * rdot_comp[i] * weights[i] * base_weight[i];
               Sxx += cos_vcs_az[i] * cos_vcs_az[i] * weights[i] * base_weight[i];
               Sxy += cos_vcs_az[i] * sin_vcs_az[i] * weights[i] * base_weight[i];
               Syy += sin_vcs_az[i] * sin_vcs_az[i] * weights[i] * base_weight[i];
            }
            const float32_t determinant = (Sxx * Syy) - (Sxy * Sxy);
            if (determinant > 0.01F)
            {
               longvel_by_cloud = ((Sx * Syy) - (Sxy * Sy)) / determinant;
               latvel_by_cloud = ((Sxx * Sy) - (Sx * Sxy)) / determinant;

               /* Update weights */
               weights_sum = 0.0F;
               num_strong_inliers = 0;
               num_weak_inliers = 0;
               for (int32_t i = 0; i < num_dets; i++)
               {
                  constexpr float32_t minimal_det_weight_to_be_considered_as_strong_inlier = 0.99F;
                  constexpr float32_t minimal_det_weight_to_be_considered_as_weak_inlier = 0.8F;
                  const float32_t est_rdot_comp = cos_vcs_az[i] * longvel_by_cloud + sin_vcs_az[i] * latvel_by_cloud;
                  weights[i] = k_delta / std::fmaxf(k_delta, std::abs(rdot_comp[i] - est_rdot_comp));
                  weights_sum += weights[i];

                  // Detection is a strong inlier if the fit is almost perfect
                  const bool f_is_strong_inlier = (weights[i] > minimal_det_weight_to_be_considered_as_strong_inlier);
                  // Detection is a weak inlier if the fit (distance from point to the estimated velocity profile curve) is good
                  const bool f_is_weak_inlier = (weights[i] > minimal_det_weight_to_be_considered_as_weak_inlier);
                  if (f_is_strong_inlier)
                  {
                      num_strong_inliers++;
                  }
                  if (f_is_weak_inlier)
                  {
                      weak_inlier_azimuths[num_weak_inliers] = azimuths[i];
                      num_weak_inliers++;
                  }
               }
            }
            else
            {
               f_valid_result = false;
            }

            constexpr float32_t minimal_difference_from_previous_iteration_threshold = 0.01F;
            if ((!f_valid_result) || ((std::abs(longvel_by_cloud - prev_longvel) < minimal_difference_from_previous_iteration_threshold) && (std::abs(latvel_by_cloud - prev_latvel) < minimal_difference_from_previous_iteration_threshold)))
            {
               break;
            }
            else
            {
               prev_longvel = longvel_by_cloud;
               prev_latvel = latvel_by_cloud;
            }
         }

         const float32_t k_min_inliers_az_spread_rad = F360_DEG2RAD(k_min_az_spread_deg);

         /* Flag for high cross radial velocity estimate by cloud logic. Used for not allowing high cloud confidence if flag is TRUE */
         const float32_t cross_radial_to_radial_ratio_upper_lim_fast_obj = 3.0F; // [-] Upper Limit of Ratio of Cloud Cross Radial Velocity Estimate to Radial Velocity Estimate to not flag as high cross radial estimate [Used for faster objects cloud init estimate]
         const float32_t cross_radial_to_radial_ratio_upper_lim_slow_obj = 10.0F; // [-] Upper Limit of Ratio of Cloud Cross Radial Velocity Estimate to Radial Velocity Estimate to not flag as high cross radial estimate [Used for slower objects cloud init estimate]
         const float32_t az_spread_limit_for_cross_radial_check = 4.0F; // [deg] Upper Limit of azimuth spread of dets to check for high cross radial estimate in faster moving objects
         const float32_t cross_radial_max_rdotcomp_check = 1.0F; // [m/s] Maximum average compensated range rate for cross radial check (only check and flag high cross radial vel estimate for slow compensated range rate clusters)

         const float32_t mean_rdotcomp = std::abs(F360_Mean(rdot_comp, static_cast<uint32_t>(num_dets)));
         const float32_t cloud_cross_radial_vel_est = std::abs(-cluster.sin_vcs_az * longvel_by_cloud + cluster.cos_vcs_az * latvel_by_cloud);
         const float32_t cloud_radial_vel_est = std::abs(cluster.cos_vcs_az * longvel_by_cloud + cluster.sin_vcs_az * latvel_by_cloud);

         const bool f_high_cross_radial_cloud_vel_est_fast_obj = ((az_spread < F360_DEG2RAD(az_spread_limit_for_cross_radial_check)) && (cloud_cross_radial_vel_est > cross_radial_to_radial_ratio_upper_lim_fast_obj * cloud_radial_vel_est)); // Flag if cloud init estimate has much higher cross radial velocity than radial velocity without backing from az spread
         const bool f_high_cross_radial_cloud_vel_est_slow_obj = ((mean_rdotcomp < cross_radial_max_rdotcomp_check) && (cloud_cross_radial_vel_est > cross_radial_to_radial_ratio_upper_lim_slow_obj * cloud_radial_vel_est)); // Flag if cloud init estimate has almost purely cross radial velocity from ambiguous dets with low rdotcomp
         const bool f_high_cross_radial_cloud_vel_est_az_increase = (cloud_cross_radial_vel_est > cloud_radial_vel_est); //If cloud cross radial velocity estimate is higher than radial velocity estimate, demand higher azimuth spread for high confidence


         // Detect significant gap in azimuths (compared to the total spread)
         uint32_t inlier_az_perm[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
         (void)F360_Sort(static_cast<uint32_t>(num_weak_inliers), true, weak_inlier_azimuths, inlier_az_perm);
         const float32_t inliers_az_spread = (num_weak_inliers > 1) ? weak_inlier_azimuths[num_weak_inliers - 1] - weak_inlier_azimuths[0] : 0.0F;

         // Gap will be treated as significant, if it is over half of the total spread and the spread adjusted by a gap will be smaller then the threshold
         const float32_t az_thold_scaling_factor = f_high_cross_radial_cloud_vel_est_az_increase ? 2.0F : 1.0F;
         const float32_t az_thold = az_thold_scaling_factor * k_min_inliers_az_spread_rad;
         const float32_t az_gap_threshold = std::max(inliers_az_spread * 0.5F, inliers_az_spread - az_thold);
         bool f_az_gap_found = false;
         for (uint32_t i = 1U; i < static_cast<uint32_t>(num_weak_inliers); i++)
         {
             const float32_t az_diff = weak_inlier_azimuths[i] - weak_inlier_azimuths[i - 1U];
             if (az_diff > az_gap_threshold)
             {
                 // if gap is found then it must be unique, since it's at least half of the total spread
                 f_az_gap_found = true;
                 break;
             }
         }

         // If azimuth gap is found, increase the demanded threshold for confidence of the estimate
         const float32_t weight_th_increase_factor = f_az_gap_found ? 1.25F : 1.0F;

         
         /* Determine Confidence */
         const float32_t average_weight = weights_sum / static_cast<float32_t>(num_dets);
         const float32_t az_spread_factor = inliers_az_spread / k_min_inliers_az_spread_rad;
         const float32_t az_spread_factor_high_conf_threshold = f_high_cross_radial_cloud_vel_est_az_increase ? 2.0F : 1.5F;
         const float32_t az_spread_factor_med_conf_threshold = f_high_cross_radial_cloud_vel_est_az_increase ? 1.5F : 1.0F;
         const float32_t average_weight_minimal_threshold = weight_th_increase_factor * 0.4F;
         const float32_t average_weight_high_conf_threshold = weight_th_increase_factor * 0.8F;
         const float32_t average_weight_med_conf_threshold = weight_th_increase_factor * 0.65F;
         const int32_t inlier_standalone_threshold = 11;
         const int32_t inlier_threshold_bump = std::max(0, num_dets - 15) / 5;
         const bool f_standalone_inlier_only = (num_strong_inliers >= inlier_standalone_threshold + inlier_threshold_bump) && (!f_az_gap_found);


         if ((!f_valid_result) || (average_weight < average_weight_minimal_threshold))
         {
            estimation_confidence = CONF3_NONE;
         }
         else if ((!f_high_cross_radial_cloud_vel_est_fast_obj) && ((!f_high_cross_radial_cloud_vel_est_slow_obj)) && (az_spread_factor > az_spread_factor_high_conf_threshold) &&
             (((average_weight > average_weight_high_conf_threshold) && (num_strong_inliers >= 8)) || f_standalone_inlier_only))
         {
            estimation_confidence = CONF3_HIGH;
         }
         else if ((az_spread_factor > az_spread_factor_med_conf_threshold) && 
            (average_weight > average_weight_med_conf_threshold) && (num_strong_inliers >= k_min_num_dets))
         {
            estimation_confidence = CONF3_MED;
         }
         else
         {
            estimation_confidence = CONF3_LOW;
         }
      }

      return estimation_confidence;
   }
}
