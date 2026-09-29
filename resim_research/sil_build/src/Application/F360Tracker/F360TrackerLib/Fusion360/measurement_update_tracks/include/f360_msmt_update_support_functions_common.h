/*===================================================================================*\
* FILE: f360_msmt_update_support_functions_common.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains supporting functions declarations for f360_measurmant_update_tracks
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_MSMT_UPDATE_SUPPORT_FUNCTIONS_COMMON_H
#define F360_MSMT_UPDATE_SUPPORT_FUNCTIONS_COMMON_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "rspp_detection.h"
#include "f360_detection_props.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Select_Dets_For_RR_Update(
      const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t& k_min_num_selected_dets_per_sensor_for_binning,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T & object_track,
      uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      uint32_t & selected_dets_num,
      uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );

   void Extract_Detection_Properties_Per_Sensor(
      const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t& selected_dets_num,
      float32_t(&min_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
      float32_t(&max_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
      uint32_t(&det_idx_per_sensor)[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK],
      uint32_t(&num_det_per_sensor)[MAX_NUMBER_OF_SENSORS]);

   void Bin_Detections_By_Azimuth(
      const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&det_idx_for_sensor)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t num_det_for_sensor,
      const float32_t min_az_for_sensor,
      const float32_t max_az_for_sensor,
      uint32_t(&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
      uint32_t(&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR]);

   void Select_Median_Range_Rate_Detection_For_Each_Bin(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
      const uint32_t(&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR],
      uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& selected_dets_num,
      uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]);

   void Try_To_Decrease_Num_Selected_Dets(
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t& k_min_num_selected_dets_per_sensor_for_binning,
      uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& selected_dets_num,
      uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );
}
#endif
