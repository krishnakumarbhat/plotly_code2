/*===================================================================================*\
 * FILE:  f360_populate_internal_trailer_detector_log.cpp
 *====================================================================================
 * Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *------------------------------------------------------------------------------------
 * Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 *
\*==========================================================================================*/
#include <cstring>
#include "f360_populate_internal_trailer_detector_log.h"

namespace f360_variant_A
{
   void Populate_Internal_Trailer_Detector_Log_Data(F360_Internal_Trailer_Detector_T& trailer_internal_log,
      const F360_PVTrailer_Data_T& pvtrailer_data)
   {
      trailer_internal_log.tp_window_timer = 0U;
      trailer_internal_log.tp_f_estimation_done = false;
      trailer_internal_log.TP_Relative_Ratio_dets_n_02 = 0;
      trailer_internal_log.TP_Relative_Ratio_dets_n_03 = 0;
      trailer_internal_log.TP_Relative_Ratio_dets_n_04 = 0;
      trailer_internal_log.TP_Relative_Ratio_dets_n_05 = 0;
      (void)memset(trailer_internal_log.TP_Mean_RR_value_array, 0, sizeof(trailer_internal_log.TP_Mean_RR_value_array));
      (void)memset(trailer_internal_log.TP_Mean_RR_num_array, 0, sizeof(trailer_internal_log.TP_Mean_RR_num_array));

      trailer_internal_log.tl_f_estimation_done = pvtrailer_data.pvtrailer_length.f_estimation_done;
      trailer_internal_log.tl_reset_timer = 0U;
      trailer_internal_log.tl_window_timer = pvtrailer_data.pvtrailer_length.window_timer;
      trailer_internal_log.axel_trailer_length = pvtrailer_data.pvtrailer_length.axle_trailer_length;
      (void)memcpy(trailer_internal_log.detection_row, pvtrailer_data.pvtrailer_length.detection_row, sizeof(trailer_internal_log.detection_row));
      
      trailer_internal_log.tw_f_estimation_done = pvtrailer_data.pvtrailer_width.f_estimation_done;
      trailer_internal_log.tw_reset_timer = 0U;
      trailer_internal_log.tw_window_timer = pvtrailer_data.pvtrailer_width.window_timer;
      (void)memcpy(trailer_internal_log.detection_col, pvtrailer_data.pvtrailer_width.dets_cnt_per_x_interval, sizeof(trailer_internal_log.detection_col));

      trailer_internal_log.HV_angle = pvtrailer_data.pvtrailer_angle.trailer_angle_rad;
      trailer_internal_log.HV_cnt = pvtrailer_data.pvtrailer_angle.HV_cnt;
      trailer_internal_log.prev_trailer_angle = 0.0F;
      trailer_internal_log.trailer_axel_length = 0.0F;
   }

   void Populate_Internal_Trailer_Detector_Data(F360_PVTrailer_Data_T& pvtrailer_data,
      const F360_Internal_Trailer_Detector_T& trailer_internal_log, 
      const Trailer_Detector_Log_T& trailer_detector_log)
   {
      pvtrailer_data.pvtrailer_length.f_estimation_done = trailer_internal_log.tl_f_estimation_done;
      pvtrailer_data.pvtrailer_length.window_timer = trailer_internal_log.tl_window_timer;
      pvtrailer_data.pvtrailer_length.axle_trailer_length = trailer_internal_log.axel_trailer_length;
      (void)memcpy(pvtrailer_data.pvtrailer_length.detection_row, trailer_internal_log.detection_row, sizeof(pvtrailer_data.pvtrailer_length.detection_row));

      pvtrailer_data.pvtrailer_width.f_estimation_done = trailer_internal_log.tw_f_estimation_done;
      pvtrailer_data.pvtrailer_width.window_timer = trailer_internal_log.tw_window_timer;
      (void)memcpy(pvtrailer_data.pvtrailer_width.dets_cnt_per_x_interval, trailer_internal_log.detection_col, sizeof(pvtrailer_data.pvtrailer_width.dets_cnt_per_x_interval));

      pvtrailer_data.pvtrailer_angle.trailer_angle_rad = trailer_internal_log.HV_angle;
      pvtrailer_data.pvtrailer_angle.HV_cnt = trailer_internal_log.HV_cnt;

      pvtrailer_data.radar_detection_timer = trailer_detector_log.radar_detection_timer;
      pvtrailer_data.pvtrailer_width.trailer_width = trailer_detector_log.trailer_width;
      pvtrailer_data.pvtrailer_length.trailer_length = trailer_detector_log.trailer_length;
   }
}
