#ifndef F360_PVTRAILER_WIDTH_ESTIMATION_H
#define F360_PVTRAILER_WIDTH_ESTIMATION_H
/*===========================================================================*\
 * FILE: f360_pvtrailer_width_estimation.h
 *============================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "f360_host.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_pvtrailer_data.h"

namespace f360_variant_A
{
   void PVTrailer_Estimate_Width(const F360_Host_T& vehicle_data,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_PVTrailer_Width_Data_T& pvtrailer_width);

   void Process_Input(const F360_Host_T& vehicle_data,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_PVTrailer_Width_Data_T& pvtrailer_width);

   void Estimate(F360_PVTrailer_Width_Data_T& pvtrailer_width);
}

#endif
