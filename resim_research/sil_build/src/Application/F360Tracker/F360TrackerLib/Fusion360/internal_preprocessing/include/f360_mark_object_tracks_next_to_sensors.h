/*===========================================================================*\
* FILE: f360_mark_object_tracks_next_to_sensors.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*    Contains function declaration of Mark_Object_Tracks_Next_To_Sensors() and Calc_Object_Track_Side_Pos().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_MARK_OBJECT_TRACKS_NEXT_TO_SENSORS_H
#define F360_MARK_OBJECT_TRACKS_NEXT_TO_SENSORS_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_radar_sensor.h"
#include "f360_radar_sensor_props.h"
#include "f360_timing_info.h"
namespace f360_variant_A
{
   void Mark_Object_Tracks_Next_To_Sensors(
      const F360_Calibrations_T &calib,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Radar_Sensor_Props_T (&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_TRKR_TIMING_INFO_T &tracker_time_info
   );

   float32_t Calc_Object_Track_Side_Pos(
      const float32_t vcs_sensor_lat_pos,
      const float32_t object_track_center_lat_pos,
      const float32_t object_track_width
   );
}
#endif
