#ifndef F360_OCCLUSION_H
#define F360_OCCLUSION_H
/*===================================================================================*\
* FILE:  f360_occlusion.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*/

#include "f360_reuse.h"
#include "f360_tracker_info.h"
#include "f360_radar_sensor.h"
#include "f360_object_track.h"
#include "f360_timing_info.h"
#include "f360_occlusion_types.h"

namespace f360_variant_A
{
   void Update_Occlusion_Data(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      F360_TRKR_TIMING_INFO_T& timing_info);

   F360_Occlusion_Status_T Get_Object_Occlusion_Status(
      const F360_Object_Track_T& object,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS]);

   F360_Occlusion_Status_T Get_Point_Occlusion_Status(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      const float32_t vcs_longpos,
      const float32_t vcs_latpos,
      const int32_t obj_id = -1,
      const int32_t sensor_id = -1);
}

#endif
