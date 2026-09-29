/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_estimate_velocity_by_position_change_helpers.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definitions of support functions to Estimate_Velocity_By_Position_Change()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include <cmath>
#include "f360_estimate_velocity_by_position_change_helpers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Unique_Data()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const int32_t sens_idx - senor index for new time stamp ts
   * const float32_t ts new timestamp to be added to the unique_ts
   * float32_t(&unique_ts)[MAX_NUMBER_OF_SENSORS][max_ts] - array of unique timestamps per senor
   * int32_t(&num_ts)[MAX_NUMBER_OF_SENSORS] - array with number of unique timestamps per sensor
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
   * This function takes a new timestamp and the corresonding sensor index from with the timestamp originates.
   * If there isn't already any corresponding timestamp in the unique_ts array (for the sensor) then this new
   * timestamp is added to the array and the corresponding num_ts for the sensor is increased by 1.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Unique_Data(
      const int32_t sens_idx,
      const float32_t ts,
      float32_t(&unique_ts)[MAX_NUMBER_OF_SENSORS][max_ts],
      int32_t(&num_ts)[MAX_NUMBER_OF_SENSORS])
   {
      bool f_found = false;
      for (int32_t i = 0; i < num_ts[sens_idx]; i++)
      {
         if (std::abs(ts - unique_ts[sens_idx][i]) < F360_EPSILON)
         {
            f_found = true;
            break;
         }
      }
      if ((!f_found) && (num_ts[sens_idx] < max_ts))
      {
         const int32_t n = num_ts[sens_idx];
         unique_ts[sens_idx][n] = ts;
         num_ts[sens_idx]++;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Get_TS_Thresholds()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const ConstantProps_T& constant_sensor_props - sensor properties
   * const float32_t cluster_range - range to cluster
   * const float32_t cluster_rdotcomp - compensated range rate of cluster
   * int32_t& min_num_ts - threshold for minumum number of unique detection timestamps in a cluster in order to allow initialization based on position change over time
   * int32_t& min_num_single_sensor_ts - threshold for minumum number of unique detection timestamps from one individual sensor in a cluster in order to allow initialization based on position change over time
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
   * This function computes thresholds for the number of the number of unique detection
   * timestamps a cluster should contain in order to allow initialization based on
   * position change over time. Thresholds are set based on sensor type as well as
   * range and compensated range rate to the cluster. Also, the function takes prior
   * threshold values into account and if these are larger than the current
   * suggestion then the prior thresholds are used.
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Get_TS_Thresholds(
      const ConstantProps_T& constant_sensor_props,
      const float32_t cluster_range,
      const float32_t cluster_rdotcomp,
      int32_t& min_num_ts,
      int32_t& min_num_single_sensor_ts)
   {
      const int32_t prev_min_num_ts = min_num_ts;
      const int32_t prev_min_num_single_sensor_ts = min_num_single_sensor_ts;

      switch (constant_sensor_props.sensor_type)
      {
         case F360_SENSOR_TYPE_SRR5_RADAR:
         case F360_SENSOR_TYPE_MRR3_RADAR:
         {
            constexpr float32_t k_long_range_threshold = 100.0F;
            constexpr float32_t k_mid_range_threshold = 70.0F;

            if (cluster_range > k_long_range_threshold)
            {
               constexpr int32_t k_min_num_ts_in_long_range = 11;
               constexpr int32_t k_min_num_single_sensor_ts_in_long_range = 8;
               min_num_ts = k_min_num_ts_in_long_range;
               min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_long_range;
            }
            else if (cluster_range > k_mid_range_threshold)
            {
               constexpr int32_t k_min_num_ts_in_mid_range = 8;
               constexpr int32_t k_min_num_single_sensor_ts_in_mid_range = 5;
               min_num_ts = k_min_num_ts_in_mid_range;
               min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_mid_range;
            }
            else
            {
               constexpr int32_t k_min_num_ts_in_close_range = 6;
               constexpr int32_t k_min_num_single_sensor_ts_in_close_range = 4;
               min_num_ts = k_min_num_ts_in_close_range;
               min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_close_range;
            }
            break;
         }
         case F360_SENSOR_TYPE_MRR360_RADAR:
         {
            if (std::abs(constant_sensor_props.range_limits[1] - constant_sensor_props.range_limits[2]) > 40.0F)
            {
               // MRR360 in FMCW multi-mode
               constexpr float32_t k_long_range_threshold = 130.0F;
               constexpr float32_t k_mid_range_threshold = 90.0F;

               if (cluster_range > k_long_range_threshold)
               {
                  constexpr int32_t k_min_num_ts_in_long_range = 11;
                  constexpr int32_t k_min_num_single_sensor_ts_in_long_range = 8;
                  min_num_ts = k_min_num_ts_in_long_range;
                  min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_long_range;
               }
               else if (cluster_range > k_mid_range_threshold)
               {
                  constexpr int32_t k_min_num_ts_in_mid_range = 8;
                  constexpr int32_t k_min_num_single_sensor_ts_in_mid_range = 5;
                  min_num_ts = k_min_num_ts_in_mid_range;
                  min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_mid_range;
               }
               else
               {
                  constexpr int32_t k_min_num_ts_in_close_range = 6;
                  constexpr int32_t k_min_num_single_sensor_ts_in_close_range = 4;
                  min_num_ts = k_min_num_ts_in_close_range;
                  min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_close_range;
               }
            }
            else
            {
               // MRR360 in SFW mode
               constexpr float32_t k_long_range_threshold = 150.0F;
               constexpr float32_t k_mid_range_threshold = 120.0F;

               if (cluster_range > k_long_range_threshold)
               {
                  constexpr int32_t k_min_num_ts_in_long_range = 12;
                  constexpr int32_t k_min_num_single_sensor_ts_in_long_range = 9;
                  min_num_ts = k_min_num_ts_in_long_range;
                  min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_long_range;
               }
               else if (cluster_range > k_mid_range_threshold)
               {
                  constexpr int32_t k_min_num_ts_in_mid_range = 11;
                  constexpr int32_t k_min_num_single_sensor_ts_in_mid_range = 8;
                  min_num_ts = k_min_num_ts_in_mid_range;
                  min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_mid_range;
               }
               else
               {
                  constexpr int32_t k_min_num_ts_in_close_range = 10;
                  constexpr int32_t k_min_num_single_sensor_ts_in_close_range = 6;
                  min_num_ts = k_min_num_ts_in_close_range;
                  min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_close_range;
               }
            }
            break;
         }
         case F360_SENSOR_TYPE_FLR4_RADAR:
         case F360_SENSOR_TYPE_FLR4_PLT_RADAR:
         case F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR:
         case F360_SENSOR_TYPE_FLR4_PLUS_RADAR:
         case F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR:
         case F360_SENSOR_TYPE_FLR7_RADAR:
         case F360_SENSOR_TYPE_FLR7_PLT_RADAR:
         case F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR:
         case F360_SENSOR_TYPE_SRR6_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR:
         case F360_SENSOR_TYPE_SRR7_PLUS_RADAR:
         case F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR:
         case F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR:
         default:
         {
            constexpr float32_t k_long_range_threshold = 200.0F;
            constexpr float32_t k_mid_range_threshold = 150.0F;

            if (cluster_range > k_long_range_threshold)
            {
               constexpr int32_t k_min_num_ts_in_long_range = 12;
               constexpr int32_t k_min_num_single_sensor_ts_in_long_range = 9;
               min_num_ts = k_min_num_ts_in_long_range;
               min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_long_range;
            }
            else if (cluster_range > k_mid_range_threshold)
            {
               constexpr int32_t k_min_num_ts_in_mid_range = 11;
               constexpr int32_t k_min_num_single_sensor_ts_in_mid_range = 8;
               min_num_ts = k_min_num_ts_in_mid_range;
               min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_mid_range;
            }
            else
            {
               constexpr int32_t k_min_num_ts_in_close_range = 10;
               constexpr int32_t k_min_num_single_sensor_ts_in_close_range = 6;
               min_num_ts = k_min_num_ts_in_close_range;
               min_num_single_sensor_ts = k_min_num_single_sensor_ts_in_close_range;
            }
            break;
         }
      }

      /* Require more data for clusters that appear near stationary */
      constexpr float32_t near_stationary_rdot_threshold = 2.0F;
      if (std::abs(cluster_rdotcomp) < near_stationary_rdot_threshold)
      {
         constexpr int8_t min_num_single_sensor_ts_increment_for_near_stationary_cluster = 2;
         constexpr int8_t min_num_ts_increment_for_near_stationary_cluster = 3;
         min_num_single_sensor_ts += min_num_single_sensor_ts_increment_for_near_stationary_cluster;
         min_num_ts += min_num_ts_increment_for_near_stationary_cluster;
      }

      /* If the previous thresholds were higher, reset to those values */
      if (prev_min_num_single_sensor_ts > min_num_single_sensor_ts)
      {
         min_num_single_sensor_ts = prev_min_num_single_sensor_ts;
      }

      if (prev_min_num_ts > min_num_ts)
      {
         min_num_ts = prev_min_num_ts;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Base_Weight()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const bool f_super_res
   * const int8_t az_conf
   * const int8_t el_conf
   * const float32_t elevation
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
   * This function determines the IRLS (Iterative Reweighted Least Squares) base weight
   * for a detection. The base weight is unchanged between iterations (i.e. not impacted
   * by the outlier rejection part of the algorithm) and are only dependent
   * on preassumptions of the detection quality based on its super resolution flag,
   * azimuth and elevation confidence and elevation angle.
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Determine_Base_Weight(
      const bool f_super_res,
      const int8_t az_conf,
      const int8_t el_conf,
      const float32_t elevation)
   {
      float32_t base_weight = 1.0F;
      if (az_conf == 3)
      {
         constexpr float32_t base_weight_multiplier_for_low_az_conf_det = 0.8F;
         base_weight *= base_weight_multiplier_for_low_az_conf_det;
      }
      else if (az_conf == 2)
      {
         constexpr float32_t base_weight_multiplier_for_med_az_conf_det = 0.9F;
         base_weight *= base_weight_multiplier_for_med_az_conf_det;
      }
      else
      {
         // do nothing
      }
      if (el_conf > 0) // not ideal
      {
         constexpr float32_t base_weight_multiplier_for_not_high_el_conf_det = 0.95F;
         base_weight *= base_weight_multiplier_for_not_high_el_conf_det;
      }

      if (f_super_res)
      {
         constexpr float32_t base_weight_multiplier_for_super_res_det = 0.8F;
         base_weight *= base_weight_multiplier_for_super_res_det;
      }

      if (std::abs(elevation) > F360_DEG2RAD(10.0F))
      {
         constexpr float32_t base_weight_multiplier_for_outside_plus_minus_10_deg_elev_det = 0.5F;
         base_weight *= base_weight_multiplier_for_outside_plus_minus_10_deg_elev_det;
      }
      else if (std::abs(elevation) > F360_DEG2RAD(6.0F))
      {
         constexpr float32_t base_weight_multiplier_from_between_6_to_10_deg_det = 0.85F;
         base_weight *= base_weight_multiplier_from_between_6_to_10_deg_det;
      }
      else
      {
         // do nothing
      }
      return base_weight;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Inlier_Thresholds()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const ConstantProps_T& constant_sensor_props
   * const float32_t cluster_range
   * const float32_t cluster_sin_az
   * const float32_t cluster_cos_az
   * float32_t& k_delta_long
   * float32_t& k_delta_lat
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
   * This function determines the position thresholds to use for classifying a detection as
   * an inlier in the IRLS (Iterative Reweighted Least Squares) algorithm that estimates
   * cluster velocity based on position change over time. Thresholds are computed based on
   * sensor type as well as range and angle to the cluster.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Get_Inlier_Thresholds(
      const ConstantProps_T& constant_sensor_props,
      const float32_t cluster_range,
      const float32_t cluster_sin_az,
      const float32_t cluster_cos_az,
      float32_t& k_delta_long,
      float32_t& k_delta_lat)
   {
      switch (constant_sensor_props.sensor_type)
      {
         case F360_SENSOR_TYPE_FLR7_RADAR:
         case F360_SENSOR_TYPE_FLR7_PLT_RADAR:
         case F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR:
         {
            constexpr float32_t flr7_plt_max_inlier_thresh_radial = 2.0F;
            constexpr float32_t flr7_plt_max_inlier_thresh_cross_radial = 4.0F;
            constexpr float32_t flr7_plt_max_valid_cluster_tracking_range_for_radial_threshold = 150.0F;
            constexpr float32_t flr7_plt_max_valid_cluster_tracking_range_for_cross_radial_threshold = 250.0F;

            const float32_t inlier_thresh_radial = std::fminf(flr7_plt_max_inlier_thresh_radial, flr7_plt_max_inlier_thresh_radial / flr7_plt_max_valid_cluster_tracking_range_for_radial_threshold * cluster_range);
            const float32_t inlier_thresh_cross_radial = std::fminf(flr7_plt_max_inlier_thresh_cross_radial, flr7_plt_max_inlier_thresh_cross_radial / flr7_plt_max_valid_cluster_tracking_range_for_cross_radial_threshold * cluster_range);
            k_delta_long = std::fmaxf(0.25F, std::abs(cluster_cos_az * inlier_thresh_radial + cluster_sin_az * inlier_thresh_cross_radial));
            k_delta_lat = std::fmaxf(0.25F, std::abs(cluster_sin_az * inlier_thresh_radial + cluster_cos_az * inlier_thresh_cross_radial));
            break;
         }
         case F360_SENSOR_TYPE_FLR4_RADAR:
         case F360_SENSOR_TYPE_FLR4_PLT_RADAR:
         case F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR:
         case F360_SENSOR_TYPE_FLR4_PLUS_RADAR:
         case F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR:
         {
            constexpr float32_t flr4_plus_plt_max_inlier_thresh_radial = 2.0F;
            constexpr float32_t flr4_plus_plt_max_inlier_thresh_cross_radial = 4.0F;
            constexpr float32_t flr4_plus_plt_max_valid_cluster_tracking_range_for_radial_threshold = 150.0F;
            constexpr float32_t flr4_plus_plt_max_valid_cluster_tracking_range_for_cross_radial_threshold = 200.0F;

            const float32_t inlier_thresh_radial = std::fminf(flr4_plus_plt_max_inlier_thresh_radial, flr4_plus_plt_max_inlier_thresh_radial / flr4_plus_plt_max_valid_cluster_tracking_range_for_radial_threshold * cluster_range);
            const float32_t inlier_thresh_cross_radial = std::fminf(flr4_plus_plt_max_inlier_thresh_cross_radial, flr4_plus_plt_max_inlier_thresh_cross_radial / flr4_plus_plt_max_valid_cluster_tracking_range_for_cross_radial_threshold * cluster_range);
            k_delta_long = std::fmaxf(0.25F, std::abs(cluster_cos_az * inlier_thresh_radial + cluster_sin_az * inlier_thresh_cross_radial));
            k_delta_lat = std::fmaxf(0.25F, std::abs(cluster_sin_az * inlier_thresh_radial + cluster_cos_az * inlier_thresh_cross_radial));
            break;
         }
         case F360_SENSOR_TYPE_SRR5_RADAR:
         {
            constexpr float32_t srr5_max_inlier_thresh_radial = 1.5F;
            constexpr float32_t srr5_max_inlier_thresh_cross_radial = 2.0F;
            constexpr float32_t srr5_max_valid_cluster_tracking_range = 75.0F;

            const float32_t inlier_thresh_radial = std::fminf(srr5_max_inlier_thresh_radial, srr5_max_inlier_thresh_radial / srr5_max_valid_cluster_tracking_range * cluster_range);
            const float32_t inlier_thresh_cross_radial = std::fminf(srr5_max_inlier_thresh_cross_radial, srr5_max_inlier_thresh_cross_radial / srr5_max_valid_cluster_tracking_range * cluster_range);
            k_delta_long = std::fmaxf(0.25F, std::abs(cluster_cos_az * inlier_thresh_radial + cluster_sin_az * inlier_thresh_cross_radial));
            k_delta_lat = std::fmaxf(0.25F, std::abs(cluster_sin_az * inlier_thresh_radial + cluster_cos_az * inlier_thresh_cross_radial));
            break;
         }
         case F360_SENSOR_TYPE_MRR360_RADAR:
         {
            //medium range, long range
            constexpr float32_t medium_to_long_range_interval_thresh = 40.0F;
            if (std::abs(constant_sensor_props.range_limits[1] - constant_sensor_props.range_limits[2]) > medium_to_long_range_interval_thresh)
            {
               // MRR360 in FMCW multi-mode
               constexpr float32_t mrr360_FMCW_mode_max_inlier_thresh_radial = 1.5F;
               constexpr float32_t mrr360_FMCW_mode_max_inlier_thresh_cross_radial = 2.0F;
               constexpr float32_t mrr360_FMCW_mode_valid_cluster_tracking_range = 75.0F;

               const float32_t inlier_thresh_radial = std::fminf(mrr360_FMCW_mode_max_inlier_thresh_radial, mrr360_FMCW_mode_max_inlier_thresh_radial / mrr360_FMCW_mode_valid_cluster_tracking_range * cluster_range);
               const float32_t inlier_thresh_cross_radial = std::fminf(mrr360_FMCW_mode_max_inlier_thresh_cross_radial, mrr360_FMCW_mode_max_inlier_thresh_cross_radial / mrr360_FMCW_mode_valid_cluster_tracking_range * cluster_range);
               k_delta_long = std::fmaxf(0.25F, std::abs(cluster_cos_az * inlier_thresh_radial + cluster_sin_az * inlier_thresh_cross_radial));
               k_delta_lat = std::fmaxf(0.25F, std::abs(cluster_sin_az * inlier_thresh_radial + cluster_cos_az * inlier_thresh_cross_radial));
            }
            else
            {
               // MRR360 in SFW mode
               constexpr float32_t mrr360_SFW_max_inlier_thresh_radial = 2.0F;
               constexpr float32_t mrr360_SFW_max_inlier_thresh_cross_radial = 3.0F;
               constexpr float32_t mrr360_SFW_valid_cluster_tracking_range = 150.0F;

               const float32_t inlier_thresh_radial = std::fminf(mrr360_SFW_max_inlier_thresh_radial, mrr360_SFW_max_inlier_thresh_radial / mrr360_SFW_valid_cluster_tracking_range * cluster_range);
               const float32_t inlier_thresh_cross_radial = std::fminf(mrr360_SFW_max_inlier_thresh_cross_radial, mrr360_SFW_max_inlier_thresh_cross_radial / mrr360_SFW_valid_cluster_tracking_range * cluster_range);
               k_delta_long = std::fmaxf(0.25F, std::abs(cluster_cos_az * inlier_thresh_radial + cluster_sin_az * inlier_thresh_cross_radial));
               k_delta_lat = std::fmaxf(0.25F, std::abs(cluster_sin_az * inlier_thresh_radial + cluster_cos_az * inlier_thresh_cross_radial));
            }
            break;
         }
         case F360_SENSOR_TYPE_MRR3_RADAR:
         case F360_SENSOR_TYPE_SRR6_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR:
         case F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR:
         default:
         {
            constexpr float32_t default_max_inlier_thresh_radial = 2.0F;
            constexpr float32_t default_max_inlier_thresh_cross_radial = 3.0F;
            constexpr float32_t default_valid_cluster_tracking_range = 150.0F;

            const float32_t inlier_thresh_radial = std::fminf(default_max_inlier_thresh_radial, default_max_inlier_thresh_radial / default_valid_cluster_tracking_range * cluster_range);
            const float32_t inlier_thresh_cross_radial = std::fminf(default_max_inlier_thresh_cross_radial, default_max_inlier_thresh_cross_radial / default_valid_cluster_tracking_range * cluster_range);
            k_delta_long = std::fmaxf(0.25F, std::abs(cluster_cos_az * inlier_thresh_radial + cluster_sin_az * inlier_thresh_cross_radial));
            k_delta_lat = std::fmaxf(0.25F, std::abs(cluster_sin_az * inlier_thresh_radial + cluster_cos_az * inlier_thresh_cross_radial));
            break;
         }
      }
   }


   /*===========================================================================*\
   * FUNCTION: IRLS_2D()
   *===========================================================================
   * RETURN VALUE:
   *
   * PARAMETERS:
   * const int32_t num_msmt
   * const int32_t num_ts
   * const float32_t k_delta
   * const float32_t(&base_weight)[MAX_DETS_IN_OBJ_TRK * 2U]
   * const float32_t(&unique_ts)[max_ts]
   * const float32_t(&ts)[MAX_DETS_IN_OBJ_TRK * 2U]
   * const float32_t(&pos_reference)[MAX_DETS_IN_OBJ_TRK * 2U]
   * float32_t& velocity
   * float32_t& inlier_ratio
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
   * This function consists of the IRLS (Iterative Reweighted Least Squares) algorithm.
   * 2D velocity estimates are obtained as well as a 2D initial position.
   * Algorithm robustly solves the optimiazation following problem for p0 (initial position)
   * and v (velocity):
   *
   * argmin Sum [w_base_i * w_i * (x_i - (p0 + v*t_i))^2]
   *
   * In above equations the variables corresponds to
   * p0: Initial 2D position (to be estimated)
   * v: 2D velocity (to be estimated)
   * x_i: Measurement number i. 2D position measurement
   * t_i: Time stamp of corresponding x_i measurement
   * w_base_i: Base weight i. This weight is not changed in the IRLS function It is a prior weight to represent the expected detection quality
   * w_i: IRLS weight. Outlier robustness is achieved by iteratively updating this weight inside the IRLS loop to punish measurements that are far away from current estimates.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool IRLS_2D(
      const int32_t num_msmt,
      const int32_t num_ts,
      const float32_t k_delta,
      const float32_t(&base_weight)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER],
      const float32_t(&unique_ts)[max_ts],
      const float32_t(&ts)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER],
      const float32_t(&pos_reference)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER],
      int32_t& num_confirmed_ts,
      float32_t& velocity,
      float32_t& inlier_ratio)
   {
      float32_t weights[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]{};
      float32_t position = 0.0F;
      bool f_valid_result = true;

      float32_t prev_vel = 0.0F;
      float32_t prev_pos = 0.0F;

      int32_t outlier_cutoff_idx = -1;
      bool outlier_rejection_valid = false;
      bool f_outlier_candidate[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];

      // Prepare weight array, and check if outlier rejection is applicable
      for (int32_t i = 0; i < num_msmt; i++)
      {
         weights[i] = 1.0F;

         if (i < (num_msmt - 1))
         {
            if (outlier_cutoff_idx == -1)
            {
               // Look for a jump-ahead in timestamps
               if (std::abs(ts[i + 1] - ts[i]) > 0.125F)
               {
                  outlier_cutoff_idx = i;
                  outlier_rejection_valid = true;
               }
            }
            else
            {
               // Make sure that all newer data have no jump-aheads
               if (std::abs(ts[i + 1] - ts[i]) > 0.075F)
               {
                  outlier_rejection_valid = false;
               }
            }
         }
      }

      // Only allow outlier rejection if there are at most 2 outliers
      outlier_rejection_valid = outlier_rejection_valid && (outlier_cutoff_idx < 2);

      // Mark detections eligible for outlier rejection
      for (int32_t i = 0; i < num_msmt; i++)
      {
         f_outlier_candidate[i] = outlier_rejection_valid && (i <= outlier_cutoff_idx);
      }

      constexpr int32_t k_max_num_iter = 10;
      for (int32_t iter = 0; iter < k_max_num_iter; iter++)
      {
         // Run RLS
         float32_t Sx = 0.0F;
         float32_t Sy = 0.0F;
         float32_t Sxx = 0.0F;
         float32_t Sxy = 0.0F;
         float32_t Syy = 0.0F;
         for (int32_t i = 0; i < num_msmt; i++)
         {
            Sx += pos_reference[i] * weights[i] * base_weight[i];
            Sy += ts[i] * pos_reference[i] * weights[i] * base_weight[i];
            Sxx += weights[i] * base_weight[i];
            Sxy += ts[i] * weights[i] * base_weight[i];
            Syy += ts[i] * ts[i] * weights[i] * base_weight[i];
         }
         const float32_t determinant = (Sxx * Syy) - (Sxy * Sxy);
         constexpr float32_t min_determinant_threshold = 0.01F;

         if (fabsf(determinant) > min_determinant_threshold)
         {
            position = ((Sx * Syy) - (Sxy * Sy)) / determinant;
            velocity = ((Sxx * Sy) - (Sx * Sxy)) / determinant;

            // Update weights
            for (int32_t i = 0; i < num_msmt; i++)
            {
               const float32_t est_pos = position + ts[i] * velocity;
               weights[i] = k_delta / std::fmaxf(k_delta, std::abs(pos_reference[i] - est_pos));
               constexpr float32_t low_weight_threshold = 0.6F;
               if ((weights[i] < low_weight_threshold) && f_outlier_candidate[i])
               {
                  weights[i] = 0.0F;
               }
            }
         }
         else
         {
            f_valid_result = false;
         }
         constexpr float32_t minimal_difference_from_previous_iteration_threshold = 0.01F;

         if (((std::abs(position - prev_pos) < minimal_difference_from_previous_iteration_threshold) &&
            (std::abs(velocity - prev_vel) < minimal_difference_from_previous_iteration_threshold)) ||
            (!f_valid_result))
         {
            break;
         }
         else
         {
            prev_pos = position;
            prev_vel = velocity;
         }
      }

      if (f_valid_result)
      {
         // Count confirmed time slots and inliers
         int32_t num_inliers = 0;
         num_confirmed_ts = 0;
         for (int32_t i = 0; i < num_ts; i++)
         {
            bool f_inlier = false;
            bool f_confirmed_ts = false;
            for (int32_t j = 0; j < num_msmt; j++)
            {
               if (std::abs(ts[j] - unique_ts[i]) < F360_EPSILON)
               {
                  const float32_t est_pos = position + ts[j] * velocity;
                  if (std::abs(est_pos - pos_reference[j]) < k_delta)
                  {
                     f_inlier = true;
                  }

                  if (weights[j] > 0.01F)
                  {
                     f_confirmed_ts = true;
                  }
               }
            }

            if (f_inlier)
            {
               num_inliers++;
            }
            if (f_confirmed_ts)
            {
               num_confirmed_ts++;
            }
         }

         if (num_confirmed_ts > 0)
         {
            inlier_ratio = static_cast<float32_t>(num_inliers) / static_cast<float32_t>(num_confirmed_ts);
         }
         else
         {
            f_valid_result = false;
         }
      }

      return f_valid_result;
   }
}
