/*===================================================================================*\
* FILE: f360_update_object_confidence_levels_helpers.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of supporting functions used in Update_Object_Confidence_Levels()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_UPDATE_OBJECT_CONFIDENCE_LEVELS_HELPERS_H
#define F360_UPDATE_OBJECT_CONFIDENCE_LEVELS_HELPERS_H

#include "f360_reuse.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
namespace f360_variant_A
{
   void Calc_Average_Confidence_Level(
      const float32_t& elapsed_time,
      F360_Object_Track_T& object);

   float32_t Calculate_Conf_Tau_For_Coasted_Objects(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib);

   float32_t Determine_Raw_Confidence_Level_Of_Coasted_Object(
      const F360_Object_Track_T& object,
      const float32_t time_since_coasted_track_updated_th);

   float32_t Determine_Raw_Confidence_Level_Of_Updated_Object(
      const uint32_t object_num_rr_inlier_dets,
      const uint32_t object_n_dets,
      const float32_t k_conf_raw_weight_dets,
      const float32_t max_confidence_not_reduced_dets);

   void Reduce_Confidence_Based_On_Object_Flags(
      const float32_t k_conf_overlapping_reduction_factor,
      F360_Object_Track_T& object);

   float32_t Determine_Filter_Coef(
      const F360_Object_Track_T& object,
      const float32_t elapsed_time,
      const F360_Calibrations_T& calib);

   float32_t Determine_Raw_Confidence_Level(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib);
}
#endif
