/*===================================================================================*\
* FILE: f360_calculate_merged_object_dimensions.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Calculate_Merged_Object_Dimensions function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_CALCULATE_MERGED_OBJECT_DIMENSIONS_H
#define F360_CALCULATE_MERGED_OBJECT_DIMENSIONS_H

#include "f360_object_track.h"
#include "f360_dimensions.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   F360_Dimensions_T Calculate_Merged_Object_Dimensions(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T & obj_to_keep,
      const F360_Object_Track_T & obj_to_kill);

   void Update_Min_Max_TCS_Pos(
      const F360_Detection_Props_T& det_prop,
      const F360_Object_Track_T& obj_to_keep,
      float32_t& det_x_tcs_max_pos,
      float32_t& det_x_tcs_min_pos
   );

   void Det_Based_Merged_Obj_Length(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T& obj_to_keep,
      const F360_Object_Track_T& obj_to_kill,
      float32_t& measured_len1,
      float32_t& measured_len2
   );

   F360_Dimensions_T Obj_To_Kill_Based_Merged_Obj_Dimensions(
      const F360_Object_Track_T& obj_to_keep,
      const F360_Object_Track_T& obj_to_kill
   );

   float32_t Calc_Time_Ratio(
      const float32_t& obj_to_keep_time_since_init,
      const float32_t& obj_to_kill_time_since_init
   );

   float32_t Calc_Weight(
      const float32_t& obj_to_keep_time_since_init,
      const float32_t& obj_to_kill_time_since_init
   );

   F360_Dimensions_T Final_Merged_Obj_Dimensions(
      const F360_Dimensions_T& dimensions,
      const float32_t& alpha_trust_original_dimension,
      const float32_t& alpha_trust_measured_dimension,
      const float32_t& measured_len1,
      const float32_t& measured_len2
   );

}
#endif
