/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_test_stationary_hypothesis.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Test_Stationary_Hypothesis()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_test_stationary_hypothesis.h"
#include "f360_math_func.h"
#include "f360_host.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Test_Stationary_Hypothesis()
   *===========================================================================
   * RETURN VALUE:
   * NONE
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibrations
   * const F360_Host_T& host
   * const F360_Detection_Hist_T& det_hist
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detections
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Cluster_T& cluster
   * float32_t& longvel_estimate
   * float32_t& latvel_estimate
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
   * This function checks weather the detection position and range rates of a cluster
   * corresponds to that of a stationary target. Function checks that compensated range
   * rate of detections in cluster is small enough and that the detection position spread
   * is small enough.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   F360_Track_Init_T Test_Stationary_Hypothesis(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Cluster_T& cluster,
      float32_t& longvel_estimate,
      float32_t& latvel_estimate)
   {
      /* Collect data */
      int16_t count_rdot_inlier = 0;
      int16_t count_pos_inlier = 0;

      const float32_t cos_az = cluster.cos_vcs_az;
      const float32_t sin_az = cluster.sin_vcs_az;


      constexpr float32_t med_az_conf_gate_modifier = 0.05F;
      constexpr float32_t low_az_conf_gate_modifier = 0.2F;
      constexpr float32_t el_conf_gate_modifier = 0.2F;
      constexpr float32_t max_orth_gate_increment_modifier = 2.0F;
      constexpr float32_t orth_gate_increment_modifier_coefficient = 0.04F;

      const float32_t cluster_range = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);
      const float32_t para_gate = 1.5F;
      const float32_t orth_gate = 0.35F + std::fminf(std::fmaxf(0.0F, cluster_range - 5.0F) * orth_gate_increment_modifier_coefficient, max_orth_gate_increment_modifier);
      const float32_t cluster_para = cos_az * cluster.vcs_position_x + sin_az * cluster.vcs_position_y;
      const float32_t cluster_orth = -sin_az * cluster.vcs_position_x + cos_az * cluster.vcs_position_y;

      // Check if object is close: within +10m in front and +/-20m left/right
      // and host speed is low (< 0.2 m/s)
      const bool is_close_object = (((cluster.vcs_position_x >= 0.0F) && (cluster.vcs_position_x <= 10.0F)) &&
                                   (std::abs(cluster.vcs_position_y) <= 20.0F));
      const bool is_host_speed_low = (std::abs(host.speed) < 0.2F);
      const float32_t initial_rdot_gate = (is_close_object && is_host_speed_low) ? 0.4F : 0.8F;

      for (int16_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         const F360_Detection_Hist_Data_T& det = det_hist.det_data[det_idx];
         float32_t rdot_gate = initial_rdot_gate;
         rdot_gate += (det.az_conf == 1) ? med_az_conf_gate_modifier : 0.0F;
         rdot_gate += (det.az_conf >= 2) ? low_az_conf_gate_modifier : 0.0F;
         rdot_gate += (det.el_conf > 0) ? el_conf_gate_modifier : 0.0F;

         if (std::abs(det.rdot_comp) < rdot_gate)
         {
            count_rdot_inlier++;
         }

         const float32_t det_para = cos_az * det.vcs_position_x + sin_az * det.vcs_position_y;
         const float32_t det_orth = -sin_az * det.vcs_position_x + cos_az * det.vcs_position_y;
         if ((std::abs(det_para - cluster_para) < para_gate) && (std::abs(det_orth - cluster_orth) < orth_gate))
         {
            count_pos_inlier++;
         }
      }
      for (int32_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         const F360_Detection_Props_T det_prop = det_props[det_idx];
         const rspp_variant_A::Raw_Detection_T& det = raw_detections.detections[det_idx].raw;

         float32_t rdot_gate = initial_rdot_gate;
         rdot_gate += (det.confid_azimuth == 1) ? med_az_conf_gate_modifier : 0.0F;
         rdot_gate += (det.confid_azimuth >= 2) ? low_az_conf_gate_modifier : 0.0F;
         rdot_gate += (det.confid_elevation > 0) ? el_conf_gate_modifier : 0.0F;

         if (std::abs(det_prop.range_rate_compensated) < rdot_gate)
         {
            count_rdot_inlier++;
         }

         const float32_t det_para = cos_az * det_prop.vcs_position.x + sin_az * det_prop.vcs_position.y;
         const float32_t det_orth = -sin_az * det_prop.vcs_position.x + cos_az * det_prop.vcs_position.y;
         if ((std::abs(det_para - cluster_para) < para_gate) && (std::abs(det_orth - cluster_orth) < orth_gate))
         {
            count_pos_inlier++;
         }
      }

      uint16_t min_num_dets{};
      const float32_t az_from_90deg = std::abs(cluster.rep_vcs_az) - F360_PI_2;
      constexpr float32_t restrictive_zone_range_threshold = 15.0F;
      constexpr float32_t restrictive_zone_angle_threshold = 15.0F;

      // be more restrictive in the 30 deg cones on the sides of the host and closer than 15m
      if ((cluster_range < restrictive_zone_range_threshold) && (std::abs(az_from_90deg) < F360_DEG2RAD(restrictive_zone_angle_threshold)))
      {
         min_num_dets = calibrations.k_init_min_num_dets_from_restrictive_zone;
      }
      else
      {
         min_num_dets = calibrations.k_init_min_num_dets_from_outside_restrictive_zone;
      }

      const int16_t total_num_dets = cluster.ndets + cluster.num_old_dets;
      const bool f_ndets_ok = (total_num_dets >= static_cast<int16_t>(min_num_dets));
      const float32_t pos_inlier_ratio_threshold_for_being_stationary = 3.0F / 5.0F;
      const float32_t rdot_inlier_ratio_threshold_for_being_stationary = 3.0F / 4.0F;
      const bool f_stationary_pos = static_cast<float32_t>(count_pos_inlier) > (pos_inlier_ratio_threshold_for_being_stationary * static_cast<float32_t>(total_num_dets));
      const bool f_stationary_rdot = static_cast<float32_t>(count_rdot_inlier) > (rdot_inlier_ratio_threshold_for_being_stationary * static_cast<float32_t>(total_num_dets));

      F360_Track_Init_T init_type = F360_TRACK_INIT_INVALID;
      if (f_stationary_rdot && f_stationary_pos && f_ndets_ok)
      {
         init_type = F360_TRACK_INIT_STATIONARY;
         longvel_estimate = 0.0F;
         latvel_estimate = 0.0F;
      }
      return init_type;
   }
}
