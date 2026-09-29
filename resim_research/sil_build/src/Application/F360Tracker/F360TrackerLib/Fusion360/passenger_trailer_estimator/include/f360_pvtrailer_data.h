#ifndef F360_PVTRAILER_DATA_H
#define F360_PVTRAILER_DATA_H
/*===================================================================================*\
* FILE: f360_pvtrailer_data.h
*====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===================================================================================*/

#include "f360_reuse.h"

namespace f360_variant_A
{
   const uint16_t DETECTION_ROWS = 80U;
   const uint16_t DETECTION_COLS = 45U;
   const uint32_t X_INTERVALS_NUMBER = 12U; // Number of intervals in x position for when collecting detections

   typedef enum Trailer_Detector_Status_Tag
   {
      TRAILER_DETECTOR_STATUS_NOT_RUNNING = 0U,
      TRAILER_DETECTOR_STATUS_RUNNING = 1U,
      TRAILER_DETECTOR_STATUS_UNKNOWN = 2U, // Default State
   }Trailer_Detector_Status_T;

   typedef struct F360_PVTrailer_Length_Data_Tag
   {
      int32_t detection_row[DETECTION_ROWS];
      uint32_t window_timer;
      float32_t trailer_length_SVM;
      float32_t trailer_length_SVM_conf;
      float32_t trailer_length;
      float32_t trailer_length_peaks;
      float32_t trailer_HV_gap;
      float32_t axle_trailer_length;
      bool f_estimation_done;
   }F360_PVTrailer_Length_Data_T;

   typedef struct F360_PVTrailer_Width_Data_Tag
   {
      uint32_t dets_cnt_per_x_interval[X_INTERVALS_NUMBER][DETECTION_COLS];
      uint32_t n_dets_per_area[X_INTERVALS_NUMBER];
      bool dets_in_left_side[X_INTERVALS_NUMBER];
      bool dets_in_right_side[X_INTERVALS_NUMBER];
      uint32_t total_number_of_dets;
      uint32_t window_timer;
      float32_t trailer_width;
      bool f_estimation_done;
   }F360_PVTrailer_Width_Data_T;

   typedef struct F360_PVTrailer_Angle_Data_Tag
   {
      int16_t HV_cnt;
      bool HV_start;
      float32_t trailer_angle_rad;
      float32_t trailer_angle_rate_rad;
   }F360_PVTrailer_Angle_Data_T;

   typedef struct F360_PVTrailer_Data_Tag
   {
      F360_PVTrailer_Length_Data_T pvtrailer_length;
      F360_PVTrailer_Width_Data_T pvtrailer_width;
      F360_PVTrailer_Angle_Data_T pvtrailer_angle;
      uint32_t radar_detection_timer;
      Trailer_Detector_Status_T trailer_detection_status;
      bool f_trailer_connected;
   }F360_PVTrailer_Data_T;
}
#endif
