/*===================================================================================*\
* FILE: f360_pre_association_track_management.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains implementations of functions related to measurement update of tracks.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_pre_association_track_management.h"
#include "f360_get_wall_time.h"
#include "f360_math_func.h"
#include "f360_object_based_radar_phenomena.h"
#include "f360_object_based_water_spray_detector.h"
#include "f360_mark_objects_entering_dead_zone.h"
#include "f360_detection_angle_jumps.h"
#include "f360_mark_suspicious_moving_angle_jump_detection.h"
#include "f360_static_env_polys_support_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Pre_Association_Track_Management()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T & tracker_info,
   * const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Host_T& host,
   * const F360_Calibrations_T &calibrations,
   * const F360_Globals_T& globals,
   * const Static_Env_Poly_T (&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_TRKR_TIMING_INFO_T & timing_info
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * NA
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Pre_Association_Track_Management(
      const F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      const F360_Calibrations_T &calibrations,
      const F360_Globals_T& globals,
      const Static_Env_Poly_T (&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T & timing_info
   )
   {
      const float32_t start_time = get_wall_time();

      Detect_Water_Spray_From_Objects(tracker_info, calibrations, object_tracks, raw_detect_list, sensors, detection_props);

      Detect_Angle_Jumps(sensors, static_env_polys, raw_detect_list, calibrations, detection_props);

      Process_Moving_Angle_Jump_Detections(sensors, host, raw_detect_list, tracker_info, detection_props);

      constexpr float32_t min_host_speed = F360_KPH2MPS(5.0F);
      if (std::abs(host.speed) > min_host_speed)
      {
         Check_Dets_Against_Radar_Phenomena(tracker_info, object_tracks, sensors, calibrations, raw_detect_list, detection_props);
      }

      Flag_Objects_On_And_Behind_SEP(tracker_info, static_env_polys, calibrations, globals, object_tracks);

      if (calibrations.k_use_dead_zone_in_stationkeeping_scenarions)
      {
         Mark_Objects_Enterning_Dead_Zone(host, sensors, calibrations, tracker_info, object_tracks);
      }

      timing_info.pre_association_track_management = get_wall_time() - start_time;
   }
}
