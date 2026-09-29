/*===================================================================================*\
* FILE:  f360_time_update_tracks.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

/******************************
* Includes
*******************************/

#include "f360_time_update_tracks.h"
#include "f360_kill_coasted_tracks.h"
#include "f360_object_list_timestamp_update.h"
#include "f360_time_update_object_tracks_CCA.h"
#include "f360_time_update_object_tracks_CTCA.h"
#include "f360_predict_exist_prob.h"
#include "f360_get_wall_time.h"
#include "f360_priority_update_tracks.h"
#include "f360_convert_object_prev_vcs_to_current_vcs.h"
#include "f360_math.h"
#include "f360_sorted_tracks_mgmt.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Time_Update_Tracks()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations,
   * const float32_t elapsed_time_s,
   * const F360_Host_T & host,
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Host_Props_T & host_props,
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_Tracker_Info_T & tracker_info,
   * F360_TRKR_TIMING_INFO_T &timing_info)
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
   * Main function for performing Kalman filter time update of objects.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Time_Update_Tracks(
      const F360_Calibrations_T & calibrations,
      const float32_t elapsed_time_s,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_Props_T & host_props,
      const F360_Globals_T& globals,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T & tracker_info,
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      const float32_t start_time = get_wall_time();

      Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);         
      Time_Update_Object_Tracks_CCA(elapsed_time_s, tracker_info, calibrations, object_tracks, timing_info);
      Time_Update_Object_Track_CTCA(elapsed_time_s, object_tracks, tracker_info, calibrations, timing_info);
      Object_List_Timestamp_Update(sensors, tracker_info);

      // Between two tracker iterations host might have moved, compensate for the translation and rotation of the VCS.
      Convert_Object_Prev_Vcs_To_Current_Vcs(tracker_info, host_props, object_tracks);

      for (int32_t index = 0; index < tracker_info.num_active_objs; index++)
      {
         F360_Object_Track_T& obj = object_tracks[tracker_info.active_obj_ids[index] - 1];

         // Store predicted states for confidence calculation
         obj.predicted_vcs_position = obj.vcs_position;
         obj.predicted_vcs_velocity.longitudinal = obj.vcs_velocity.longitudinal;
         obj.predicted_vcs_velocity.lateral = obj.vcs_velocity.lateral;
         obj.predicted_speed = obj.speed;
      }

      Sorted_Tracks_Re_Sort(tracker_info);

      Predict_Existence_Probability(tracker_info, sensors, calibrations, object_tracks, timing_info);

      Update_Track_Priority(host, object_tracks, tracker_info);

      timing_info.time_update_tracks = get_wall_time() - start_time;
   }
}
