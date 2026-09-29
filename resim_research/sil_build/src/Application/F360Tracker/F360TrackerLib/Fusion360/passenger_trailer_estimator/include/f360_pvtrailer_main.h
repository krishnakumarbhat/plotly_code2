#ifndef F360_PVTRAILER_MAIN_H
#define F360_PVTRAILER_MAIN_H
/*===========================================================================*\
 * FILE: f360_pvtrailer_main.h
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "f360_host.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_pvtrailer_data.h"
#include "f360_timing_info.h"
#include "f360_trailer_detector_flt_fus_output.h"

namespace f360_variant_A
{
   void Run_PV_Trailer_Estimator(
      const F360_Host_T& vehicle_data,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t elapsed_time_s,
      F360_PVTrailer_Data_T& pvtrailer_data,
      F360_TRKR_TIMING_INFO_T& timing_info);

   void Parse_PV_Trailer_Output(
      const F360_PVTrailer_Data_T& pvtrailer_data,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output);

   void PVTrailer_Reset(F360_PVTrailer_Data_T& pvtrailer_data);
}

#endif
