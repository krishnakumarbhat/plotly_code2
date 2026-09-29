/*===========================================================================*\
* FILE: f360_compute_split_logic_signals.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Compute_Split_Logic_Signals() and 
*   related subfunctions
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_COMPUTE_SPLIT_LOGIC_SIGNALS_H
#define F360_COMPUTE_SPLIT_LOGIC_SIGNALS_H

#include "f360_detection_props.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   enum F360_Object_Orth_Split_Signals_Status_Type_T : uint8_t
   {
      F360_RESET_SPLIT_SIGNALS = 0,
      F360_DONT_INNOVATE_SPLIT_SIGNALS = 1,
      F360_UPDATE_SPLIT_SIGNALS = 2
   };

   void Compute_Split_Logic_Signals(
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      const float32_t dist_to_rear_axle,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]);

   void Compute_And_Filter_Objects_Detections_Max_Delta(
      const float32_t filter_constant,
      const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK],
      F360_Object_Track_T& object);

   F360_Object_Orth_Split_Signals_Status_Type_T Derive_Object_Orth_Split_Signal_Status(
      const F360_Calibrations_T& calibs,
      const F360_Object_Track_T& object,
      const float32_t dist_to_rear_axle);

   void Compute_And_Filter_Objects_Detections_Max_Gap(
      const F360_Calibrations_T& calibs,
      const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK],
      F360_Object_Track_T& object);

   void Compute_Filtered_RR_Err_Max_Gap_And_Threshold(
      const float32_t(&sorted_rr_err)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t& valid_det_cnt,
      F360_Object_Track_T& object,
      float32_t& gap_rr_thresh);

   void Compute_And_Filter_Objects_Detections_Range_Rate_Diff(
       const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
       const F360_Calibrations_T& calibs,
       const float32_t(&orth_sorted_pos)[MAX_DETS_IN_OBJ_TRK],
       const uint32_t(&orth_perm)[MAX_DETS_IN_OBJ_TRK],
       F360_Object_Track_T& object);

   void Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object);

   bool Is_Moving_In_Radial_Direction(
       const Point vcs_position,
       const F360_VCS_Velocity_T vcs_velocity);

   void Apply_Forgetting_Factor_To_RR_Error_Split_Signals(
      const bool f_update_rr_gap,
      F360_Object_Track_T& object);
}

#endif
