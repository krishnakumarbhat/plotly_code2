#ifndef F360_ESTIMATE_VELOCITY_BY_POSITION_CHANGE_HELPERS_H
#define F360_ESTIMATE_VELOCITY_BY_POSITION_CHANGE_HELPERS_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_estimate_velocity_by_position_change_helpers.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declarations of support functions to Estimate_Velocity_By_Position_Change()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   const int32_t max_ts = 14;

   void Update_Unique_Data(
      const int32_t sens_idx,
      const float32_t ts,
      float32_t(&unique_ts)[MAX_NUMBER_OF_SENSORS][max_ts],
      int32_t(&num_ts)[MAX_NUMBER_OF_SENSORS]);

   void Get_TS_Thresholds(
      const ConstantProps_T& constant_sensor_props,
      const float32_t cluster_range,
      const float32_t cluster_rdotcomp,
      int32_t& min_num_ts,
      int32_t& min_num_single_sensor_ts);

   float32_t Determine_Base_Weight(
      const bool f_super_res,
      const int8_t az_conf,
      const int8_t el_conf,
      const float32_t elevation);

   void Get_Inlier_Thresholds(
      const ConstantProps_T& constant_sensor_props,
      const float32_t cluster_range,
      const float32_t cluster_sin_az,
      const float32_t cluster_cos_az,
      float32_t& k_delta_long,
      float32_t& k_delta_lat);

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
      float32_t& inlier_ratio);
}

#endif
