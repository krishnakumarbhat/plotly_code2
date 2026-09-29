/*===========================================================================*\
* FILE: f360_mark_trailer_detections.h
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* 
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_MARK_TRAILER_DETECTIONS_H
#define F360_MARK_TRAILER_DETECTIONS_H

#include "f360_reuse.h"
#include "f360_iterator.h"
#include "f360_point.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_bounding_box.h"
#include "f360_trailer_detector_flt_fus_output.h"

namespace f360_variant_A
{
   typedef struct Stationary_Dets_Zones_Count_Tag
   {
      float32_t stationary_lat_dist_between_3_10;
      float32_t stationary_lat_dist_between_10_15;
      float32_t stationary_lat_dist_between_15_20;
      float32_t stationary_lat_dist_between_20_25;
      float32_t stationary_lat_dist_between_25_30;
      float32_t stationary_lat_dist_between_30_50;
      float32_t stationary_lat_dist_between_50_70;
      uint8_t stationary_det_num;
      uint8_t stationary_det_num_3_10;
      uint8_t stationary_det_num_10_15;
      uint8_t stationary_det_num_15_20;
      uint8_t stationary_det_num_20_25;
      uint8_t stationary_det_num_25_30;
      uint8_t stationary_det_num_30_50;
      uint8_t stationary_det_num_50_70;

   } Stationary_Dets_Zones_Count_T;

   typedef struct Trailer_Line_Tag{
      float32_t a;
      float32_t b;
      float32_t c;
   } Trailer_line;

   struct Trailer_BBox_Fov_Limits {
      float32_t leftmost_angle;
      float32_t rightmost_angle;
   };

   void Find_Occlusion_Area_CV_Trailer_All_Sensors(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Trailer_Estimator_Output_T& trailer,
      const uint32_t trailer_idx,
      Trailer_line(&left_line_of_sight),
      Trailer_line(&right_line_of_sight),
      Trailer_line(&trailer_edge));

   bool Check_Detection_To_Left_of_Line(
      const float32_t(&detection)[2],
      const Trailer_line (&line_param));

   void Compute_Line_Parameters(
      const float32_t(&rear_point)[2],
      const float32_t(&front_point)[2],
      Trailer_line (&line_param));

   void Mark_Trailer_Dets(
      const F360_Calibrations_T& calibs,
      const F360_Host_T& f360_host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Trailer_Estimator_Output_T& trailer,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&det_Props)[MAX_NUMBER_OF_DETECTIONS]);
   
   void Get_Trailer_Corner_Fov_Limits_For_Sensor(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const int32_t snsr_idx,
      const BoundingBox& trailer_bbox,
      Trailer_BBox_Fov_Limits& trailer_corner_fov_limits);

   BoundingBox Get_Trailer_Bounding_Box(
      const F360_Calibrations_T& calibs,
      const F360_Host_T& f360_host,
      const F360_Trailer_Estimator_Output_T& trailer,
      const float32_t hitch_offset);

   void Calc_Stationary_Dets_In_Zones(
      const rspp_variant_A::RSPP_Detection_T& raw_det,
      const F360_Detection_Props_T& det_prop,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      Stationary_Dets_Zones_Count_T& stat_dets,
      uint8_t& d1_det_num,
      float32_t& rho_i,
      float32_t& alpha_i);
       
   void Flag_Reflection_Dets(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& f360_host,
      const uint8_t& d1_det_num,
      const float32_t& rho_i,
      const float32_t& alpha_i,
      const Stationary_Dets_Zones_Count_T& stat_dets,
      F360_Detection_Props_T(&det_Props)[MAX_NUMBER_OF_DETECTIONS]);

   float32_t Trailer_Reflection(
      const float32_t d1_range,
      const float32_t d2_range,
      const float32_t alpha,
      const float32_t stationary_lat_dist);
       
}

#endif
