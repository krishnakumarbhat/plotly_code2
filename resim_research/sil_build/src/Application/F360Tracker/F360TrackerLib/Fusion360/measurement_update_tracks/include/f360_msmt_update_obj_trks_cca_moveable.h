#ifndef F360_MSMTUPDATE_OBJTRKS_CCA_MOVEABLE_H
#define F360_MSMTUPDATE_OBJTRKS_CCA_MOVEABLE_H
/*===================================================================================*\
* FILE: f360_msmt_update_obj_trks_cca_moveable.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Msmt_Update_ObjTrks_CCA_Moveable() function implementation
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_timing_info.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Msmt_Update_Obj_Trks_CCA_Moveable(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Calibrations_T & calib,
      const uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t selected_dets_num,
      F360_Object_Track_T & object_track,
      F360_TRKR_TIMING_INFO_T & timing_info);
   
   void Set_Acc_Vel_States(
      const float32_t(&state)[STATE_DIMENSION],
      const float32_t& state_decay_factor,
      F360_Object_Track_T& object_track);

   bool Is_Obj_Ok_To_Decay_States(
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]);

   void Update_Measurement_Covariances_For_Long_Range_Crossing_Object(
      const float32_t object_longitudinal_velocity,
      const float32_t object_lateral_velocity,
      const float32_t object_point_x,
      float32_t& R_pseudo_pos_y_increase_factor,
      float32_t& R_rr_addition_long_range_crossing);
}
#endif
