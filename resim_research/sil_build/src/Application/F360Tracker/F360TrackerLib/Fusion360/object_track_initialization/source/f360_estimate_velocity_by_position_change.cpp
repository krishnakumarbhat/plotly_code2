/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_estimate_velocity_by_position_change.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Estimate_Velocity_By_Position_Change()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include <cstring>
#include "f360_math_func.h"
#include "f360_estimate_velocity_by_position_change.h"
#include "f360_estimate_velocity_by_position_change_helpers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Estimate_Velocity_By_Position_Change()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Detection_Hist_T& det_hist
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detections
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Cluster_T& cluster
   * float32_t& longvel_by_position
   * float32_t& latvel_by_position
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
   * This function estimates velocity of a cluster based on the position change of the cluster detections over time.
   * The algorithm first verifies that the information in the cluster is sufficent by checking that there are enough
   * detections with different tiemstamps. An IRLS (Iterative Reweighted Least Squares) algorithm is then run per sensor
   * (i.e. detections from each sensor is processed separately) to estimate cluster velocity. This is done to minimize
   * effect and estimation biases due to for example incorrect sensor alignment and sensor facia distortion. Results
   * from each sensor is then combined through a weighted mean the weights are based on how many unique detection timestamps
   * there are from each sensor.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   CONF3_T Estimate_Velocity_By_Position_Change(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Cluster_T& cluster,
      float32_t& longvel_by_position,
      float32_t& latvel_by_position)
   {
      /* Find unique timestamps per sensor */
      float32_t unique_ts[MAX_NUMBER_OF_SENSORS][max_ts] = {};
      int32_t num_ts[MAX_NUMBER_OF_SENSORS] = {};

      for (int32_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         const int32_t sens_idx = static_cast<int32_t>(det_hist.det_data[det_idx].sensor_id) - 1;
         const float32_t ts = -det_hist.det_data[det_idx].time_since_meas;
         Update_Unique_Data(sens_idx, ts, unique_ts, num_ts);
      }

      for (int32_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         const int32_t sens_idx = raw_detections.detections[det_idx].raw.sensor_id - 1;
         const float32_t ts = -sensors[sens_idx].refined.time_since_measurement_s;
         Update_Unique_Data(sens_idx, ts, unique_ts, num_ts);
      }

      int32_t total_num_ts = 0;
      int32_t num_single_sensor_ts = 0;
      int32_t min_num_ts = 0;
      int32_t min_num_single_sensor_ts = 0;
      const float32_t cluster_range = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);

      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         if (num_ts[i] > 0)
         {
            total_num_ts += num_ts[i];
            num_single_sensor_ts = (num_ts[i] > num_single_sensor_ts) ? num_ts[i] : num_single_sensor_ts;
            Get_TS_Thresholds(sensors[i].constant, cluster_range, cluster.rep_rdotcomp, min_num_ts, min_num_single_sensor_ts);
         }
      }

      /* Ensure sufficient number of measurements over time are available before proceeding */
      longvel_by_position = 0.0F;
      latvel_by_position = 0.0F;
      CONF3_T estimation_confidence = CONF3_NONE;
      if ((total_num_ts >= min_num_ts) || (num_single_sensor_ts >= min_num_single_sensor_ts))
      {
         float32_t estimated_longvel[MAX_NUMBER_OF_SENSORS];
         float32_t estimated_latvel[MAX_NUMBER_OF_SENSORS];
         float32_t inlier_ratio_long[MAX_NUMBER_OF_SENSORS];
         float32_t inlier_ratio_lat[MAX_NUMBER_OF_SENSORS];
         int32_t num_confirmed_ts[MAX_NUMBER_OF_SENSORS];
         bool estimation_valid[MAX_NUMBER_OF_SENSORS];

         /* Process sensor by sensor */
         for (uint8_t sens_idx = 0U; sens_idx < MAX_NUMBER_OF_SENSORS; sens_idx++)
         {
            float32_t base_weight[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
            float32_t longpos[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
            float32_t latpos[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
            float32_t ts[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
            int32_t ndet = 0;

            /* Prepare data and determine the base weight for each detection from current sensor */
            for (int16_t i = 0; i < cluster.num_old_dets; i++)
            {
               const int16_t det_idx = cluster.old_det_idx[i];
               const F360_Detection_Hist_Data_T& det = det_hist.det_data[det_idx];
               if (sens_idx == (det.sensor_id - 1U))
               {
                  ts[ndet] = -det.time_since_meas;
                  longpos[ndet] = det.vcs_position_x;
                  latpos[ndet] = det.vcs_position_y;
                  base_weight[ndet] = Determine_Base_Weight(det.f_super_res, det.az_conf, det.el_conf, det.elevation);
                  ndet++;
               }
            }

            for (int16_t i = 0; i < cluster.ndets; i++)
            {
               const int16_t det_idx = cluster.detids[i] - 1;
               const rspp_variant_A::Raw_Detection_T& det = raw_detections.detections[det_idx].raw;
               if (sens_idx == (static_cast<uint8_t>(det.sensor_id) - 1U))
               {
                  ts[ndet] = -sensors[sens_idx].refined.time_since_measurement_s;
                  longpos[ndet] = det_props[det_idx].vcs_position.x;
                  latpos[ndet] = det_props[det_idx].vcs_position.y;
                  base_weight[ndet] = Determine_Base_Weight(det.f_super_res, det.confid_azimuth, det.confid_elevation, det.elevation);
                  ndet++;
               }
            }

            /* Determine the thresholds to classify detection as inlier, run IRLS, and check that the result is valid */
            if (ndet > 0)
            {
               float32_t k_delta_long;
               float32_t k_delta_lat;
               Get_Inlier_Thresholds(sensors[sens_idx].constant, cluster_range, cluster.sin_vcs_az, cluster.cos_vcs_az, k_delta_long, k_delta_lat);

               const bool f_valid_result_long = IRLS_2D(ndet, num_ts[sens_idx], k_delta_long, base_weight, unique_ts[sens_idx],
                  ts, longpos, num_confirmed_ts[sens_idx], estimated_longvel[sens_idx], inlier_ratio_long[sens_idx]);
               const bool f_valid_result_lat = IRLS_2D(ndet, num_ts[sens_idx], k_delta_lat, base_weight, unique_ts[sens_idx],
                  ts, latpos, num_confirmed_ts[sens_idx], estimated_latvel[sens_idx], inlier_ratio_lat[sens_idx]);

               const float32_t est_rdot_comp = cluster.cos_vcs_az * estimated_longvel[sens_idx] + cluster.sin_vcs_az * estimated_latvel[sens_idx];
               constexpr float32_t rdot_comp_difference_threshold = 3.0F;
               const bool f_rdot_confirmation = std::abs(est_rdot_comp - cluster.rep_rdotcomp) < rdot_comp_difference_threshold;

               estimation_valid[sens_idx] = f_valid_result_long && f_valid_result_lat && f_rdot_confirmation;
            }
            else
            {
               estimation_valid[sens_idx] = false;
               estimated_longvel[sens_idx] = 0.0F;
               estimated_latvel[sens_idx] = 0.0F;
               inlier_ratio_long[sens_idx] = 0.0F;
               inlier_ratio_lat[sens_idx] = 0.0F;
               num_confirmed_ts[sens_idx] = 0;
            }
         }

         /* Combine valid results from each sensor and determine confidence level */
         float32_t sum_longvel = 0.0F;
         float32_t sum_latvel = 0.0F;
         float32_t sum_weight = 0.0F;
         float32_t sum_inlier_ratio_long = 0.0F;
         float32_t sum_inlier_ratio_lat = 0.0F;
         int32_t sum_confirmed_ts = 0;
         int32_t num_confirmed_single_sensor_ts = 0;
         for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
         {
            if (estimation_valid[i])
            {
               const int32_t num_ts_sq = num_confirmed_ts[i] * num_confirmed_ts[i];
               const float32_t weight = static_cast<float32_t>(num_ts_sq);
               sum_longvel += estimated_longvel[i] * weight;
               sum_latvel += estimated_latvel[i] * weight;
               sum_inlier_ratio_long += inlier_ratio_long[i] * weight;
               sum_inlier_ratio_lat += inlier_ratio_lat[i] * weight;
               sum_weight += weight;
               sum_confirmed_ts += num_confirmed_ts[i];
               num_confirmed_single_sensor_ts = (num_confirmed_ts[i] > num_confirmed_single_sensor_ts) ? num_confirmed_ts[i] : num_confirmed_single_sensor_ts;
            }
         }

         if (sum_weight > 0.0F)
         {
            longvel_by_position = sum_longvel / sum_weight;
            latvel_by_position = sum_latvel / sum_weight;

            const float32_t weighted_inlier_ratio_long = sum_inlier_ratio_long / sum_weight;
            const float32_t weighted_inlier_ratio_lat = sum_inlier_ratio_lat / sum_weight;
            constexpr float32_t minimal_inlier_ratio_threshold_for_high_confidence = 0.75F;
            constexpr float32_t minimal_inlier_ratio_threshold_for_medium_confidence = 0.6F;
            constexpr int8_t increment_for_high_confidence = 2;
            constexpr int8_t increment_for_medium_confidence = 1;

            if ((std::fminf(weighted_inlier_ratio_long, weighted_inlier_ratio_lat) > minimal_inlier_ratio_threshold_for_high_confidence) &&
               ((sum_confirmed_ts >= (min_num_ts + increment_for_high_confidence)) || (num_confirmed_single_sensor_ts >= (min_num_single_sensor_ts + increment_for_high_confidence))))
            {
               estimation_confidence = CONF3_HIGH;
            }
            else if ((std::fminf(weighted_inlier_ratio_long, weighted_inlier_ratio_lat) > minimal_inlier_ratio_threshold_for_medium_confidence) &&
               ((sum_confirmed_ts >= (min_num_ts + increment_for_medium_confidence)) || (num_confirmed_single_sensor_ts >= (min_num_single_sensor_ts + increment_for_medium_confidence))))
            {
               estimation_confidence = CONF3_MED;
            }
            else
            {
               estimation_confidence = CONF3_LOW;
            }
         }
      }

      return estimation_confidence;
   }
}
