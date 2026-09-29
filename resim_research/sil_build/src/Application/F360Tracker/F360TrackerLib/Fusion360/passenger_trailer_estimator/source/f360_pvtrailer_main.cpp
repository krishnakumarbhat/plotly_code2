/*===========================================================================*\
 * FILE: f360_pvtrailer_main.cpp
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include <cstring>
#include "f360_pvtrailer_main.h"
#include "f360_get_wall_time.h"
#include "f360_pvtrailer_angle_estimation.h"
#include "f360_pvtrailer_length_estimation.h"
#include "f360_pvtrailer_width_estimation.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Run_PV_Trailer_Estimator()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Main function for PVTrailer estimator
   \*===========================================================================*/
   void Run_PV_Trailer_Estimator(
      const F360_Host_T& vehicle_data,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t elapsed_time_s,
      F360_PVTrailer_Data_T& pvtrailer_data,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      // Estimate states and parameters
      PVTrailer_Estimate_Length(
         vehicle_data,
         raw_detect_list,
         all_detections,
         sensors,
         pvtrailer_data.pvtrailer_length);

      PVTrailer_Estimate_Width(
         vehicle_data,
         raw_detect_list,
         all_detections,
         sensors,
         pvtrailer_data.pvtrailer_width);

      PVTrailer_Estimate_Angle(
         vehicle_data,
         elapsed_time_s,
         pvtrailer_data.pvtrailer_length.axle_trailer_length,
         pvtrailer_data.pvtrailer_angle);

      // Update status
      if (pvtrailer_data.pvtrailer_length.f_estimation_done && pvtrailer_data.pvtrailer_width.f_estimation_done)
      {
         pvtrailer_data.radar_detection_timer = 0U;
         pvtrailer_data.trailer_detection_status = TRAILER_DETECTOR_STATUS_NOT_RUNNING;
      }
      else
      {
         pvtrailer_data.radar_detection_timer++;
         pvtrailer_data.trailer_detection_status = TRAILER_DETECTOR_STATUS_RUNNING;
      }
      pvtrailer_data.f_trailer_connected = true;

      timing_info.trailer_detector = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Parse_PV_Trailer_Output()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Map data from data struct to common output struct
   \*===========================================================================*/
   void Parse_PV_Trailer_Output(
      const F360_PVTrailer_Data_T& pvtrailer_data,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output)
   {
      trailer_estimator_output.trailer_presence[0] = pvtrailer_data.f_trailer_connected ? TRAILER_PRESENCE_STATE_DETECTED : TRAILER_PRESENCE_STATE_NOT_DETECTED;
      if (pvtrailer_data.f_trailer_connected)
      {
         trailer_estimator_output.trailer_angle[0] = pvtrailer_data.pvtrailer_angle.trailer_angle_rad;
         trailer_estimator_output.trailer_length[0] = pvtrailer_data.pvtrailer_length.trailer_length;
         trailer_estimator_output.trailer_width[0] = pvtrailer_data.pvtrailer_width.trailer_width;

      }
      else
      {
         trailer_estimator_output.trailer_angle[0] = 0.0F;
         trailer_estimator_output.trailer_length[0] = 0.0F;
         trailer_estimator_output.trailer_width[0] = 0.0F;
      }

      trailer_estimator_output.trailer_presence[1] = TRAILER_PRESENCE_STATE_NOT_DETECTED;
   }

   /*===========================================================================*\
   * FUNCTION: PVTrailer_Reset()
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Clear and reset PVTrailer data
   \*===========================================================================*/
   void PVTrailer_Reset(F360_PVTrailer_Data_T& pvtrailer_data)
   {
      (void)memset(&pvtrailer_data, 0, sizeof(pvtrailer_data));
   }
}
