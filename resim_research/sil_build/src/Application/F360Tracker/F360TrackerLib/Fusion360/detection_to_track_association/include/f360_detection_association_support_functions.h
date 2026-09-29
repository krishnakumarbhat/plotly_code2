/*===========================================================================*\
* FILE: f360_detection_association_support_functions.h
*============================================================================
* Copyright (C) 2020-2022 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of supporting functions for detection association.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_DETECTION_ASSOCIATION_SUPPORT_FUNCTIONS_H
#define F360_DETECTION_ASSOCIATION_SUPPORT_FUNCTIONS_H

#include "f360_detection_props.h"
#include "rspp_detection.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_tracker_info.h"
#include "f360_static_env_poly_types.h"
#include "f360_bounding_box.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   float32_t Calc_Det_Score(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calib,
      const float32_t range_rate_diff,
      const Point & vcs_pos,
      const bool f_water_spray);

   void Calc_Range_Rate_Threshold(
      const F360_Object_Track_T & object_track,
      const rspp_variant_A::RSPP_Detection_T& det_raw,
      const F360_Detection_Props_T & det_prop,
      const F360_Radar_Sensor_T& sens,
      const F360_Calibrations_T & calibrations,
      const float32_t host_vcs_speed,
      float32_t & range_rate_th_lower,
      float32_t & range_rate_th_upper
   );

   bool Is_Det_Allowed_To_Associate(
      const F360_Detection_Props_T & det_p,
      const rspp_variant_A::RSPP_Detection_T &det_raw,
      const F360_Object_Track_T & obj,
      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
   );

   bool Compare_Against_Stationary_Hypothesis(
      const F360_Calibrations_T& calibrations,
      const F360_Radar_Sensor_T& sensor,
      const rspp_variant_A::RSPP_Detection_T& detection,
      const float32_t range_rate_threshold_lower,
      const float32_t range_rate_threshold_upper,
      const float32_t predicted_range_rate,
      const float32_t dealiased_rngrate,
      F360_Detection_Props_T& det_p
   );

   bool  Is_Low_Az_Conf_At_Boundaries_Association_Allowed(
      const F360_Detection_Props_T& det_p,
      const F360_Object_Track_T& obj,
      const float32_t max_heading_threshold
   );

   bool Is_Association_Wrt_SEP_Allowed(
      const F360_Detection_Props_T & det_p,
      const F360_Object_Track_T & obj,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
   );

   void Assign_Association_Hypothesis(
      const uint32_t number_of_valid_detections,
      const float32_t (&det_rdot_comp_array)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Tracker_Info_T & tracker_info,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   );

   float32_t Get_Score_Based_On_Detection_Position_Inside_Solid_Bbox(
      const Point& det_position_vcs,
      const F360_Object_Track_T& object_track,
      const F360_Calibrations_T& calib);
   
   float32_t Calculate_Final_Det_Score_Inside_Solid_Bbox(
      const Point & det_position_vcs,
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calib,
      const float32_t range_rate_diff);

   float32_t Get_Score_Based_On_Detection_Position_Between_Solid_Bbox_And_Ext_Bbox(
      const Point& det_position_vcs,
      const F360_Object_Track_T& object_track);

   float32_t Calculate_Final_Det_Score_Inside_Extended_Bbox(
      const Point & det_position_vcs,
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calib,
      const float32_t range_rate_diff);

   float32_t Calculate_Detection_Association_Cost(
      const F360_Detection_Props_T & det_prop,
      const F360_Object_Track_T & obj_trk);

   float32_t Calculate_Final_Det_Score_Inside_Solid_Circle(
      const Point& det_position_vcs,
      const F360_Object_Track_T& object_track,
      const F360_Calibrations_T& calib,
      const float32_t range_rate_diff);

   float32_t Calculate_Final_Det_Score_Inside_Extended_Circle(
      const Point& det_position_vcs,
      const F360_Object_Track_T& object_track,
      const F360_Calibrations_T& calib,
      const float32_t range_rate_diff);
   
   void Deassociate_Detection(
      const rspp_variant_A::RSPP_Detection_T& detection,
      const F360_Radar_Sensor_T& sensor,
      F360_Detection_Props_T& det_prop);

   bool Is_Object_Valid_For_Tightened_Gates(
      const F360_Object_Track_T& object_track
   );

   float32_t Calculate_Tightened_Range_Rate_Threshold(
      const F360_Object_Track_T& object_track,
      const float32_t max_range_rate_threshold
   );
} 

#endif
