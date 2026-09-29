/*===========================================================================*\
* FILE: f360_calc_obj_size.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Calc_Obj_Size()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_CALC_OBJ_SIZE_H
#define F360_CALC_OBJ_SIZE_H

#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_calibrations.h"
#include "f360_constants.h"
#include "rspp_detection_list.h"
#include "f360_reference_point_support_functions.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(
      const F360_Calibrations_T& calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const F360_Object_Track_T& object_track,
      float32_t& process_noise);

   void Update_Measurement_Noise_If_Many_Detections(
       const F360_Calibrations_T& calib,
       const float32_t object_length,
       const float32_t measured_length,
       const bool f_left_or_right_visible,
       F360_Object_Track_T& object_track,
       float32_t& measurement_uncertainty);

   void Calc_Obj_Size(
       const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
       const rspp_variant_A::RSPP_Detection_List_T& detection_list,
       const F360_Calibrations_T& calib,
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       const F360_Globals_T& globals,
       const F360_Tracker_Info_T& tracker_info,
       const float32_t& CIPV_long_pos,
       F360_Object_Track_T& object_track);

   struct Target_Dimension_T
   {
      float32_t maximum;
      float32_t minimum;
   };

   struct Dimension_Limits_T
   {
      Target_Dimension_T length;
      Target_Dimension_T width;
   };

   struct Kalman_Noise_T
   {
      float32_t process;
      float32_t measurement;
   };

}
#endif
