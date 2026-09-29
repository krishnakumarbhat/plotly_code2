/*===================================================================================*\
* FILE: f360_track_downselection.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv. All Rights Reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains funcions which does down-selection of object tracks.
*
* ABBREVIATIONS:
*   NONE
*
* TRACEABILITY INFO:
*   Design Document(s): downSelectObkTrks.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

/******************************
* Includes
*******************************/
#include "f360_math.h"
#include <cstring>
#include "f360_track_downselection.h"
#include "f360_push_reduced_id.h"
#include "f360_math_func.h"
#include "f360_get_wall_time.h"
#include "f360_priority_update_tracks.h"
#include "f360_track_downselection_internal_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Track_Downselection()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const F360_Calibrations_T& calib,
   * const Static_Env_Poly_T (&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * F360_Tracker_Info_T& tracker_info,
   * F360_TRKR_TIMING_INFO_T& timing_info
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
   * This function does down-selection of object tracks.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Track_Downselection(
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const Static_Env_Poly_T (&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      // Recalculate priority with up-to-date movable_prob and confidenceLevel
      Update_Track_Priority(host, object_tracks, tracker_info);

      for (uint16_t idx = 0U; idx < tracker_info.variant.num_reduced_tracks; ++idx)
      {
         tracker_info.reduced_obj_ids[idx] = 0;
      }

      float32_t priorities[NUMBER_OF_OBJECT_TRACKS];
      int32_t candidates_idxs[NUMBER_OF_OBJECT_TRACKS];
      uint32_t candidates_cnt = 0U;

      Cond_LP_Filter_Reduced_Det_Num(tracker_info, calib.k_tv_dets_exp_filter_const, object_tracks);
      Select_Obj_Tracks_to_Downselect(host, static_env_polys, sensors, tracker_info, calib, object_tracks, priorities, candidates_idxs, candidates_cnt);

      /*
         Create reduced tracks by priority, as long as slots exist
         New reduced tracks always initially have a reducedStatus of NEW
         Existing reduced tracks can be pre-empted when there are not enough slots
         Note: reducedID is invalidated when an fused track is killed
      */

      uint32_t ids_of_objs_sorted_by_priority[NUMBER_OF_OBJECT_TRACKS];
      (void)F360_Sort(candidates_cnt, false, priorities, ids_of_objs_sorted_by_priority);

      if (candidates_cnt > tracker_info.variant.num_reduced_tracks)
      {
         Deselect_Existing_Reduced_Tracks(object_tracks, tracker_info, candidates_idxs, ids_of_objs_sorted_by_priority, candidates_cnt);
      }

      if (0U < candidates_cnt)
      {
         Assign_Reduced_Idxs_To_Prioritized_Tracks(object_tracks, tracker_info, candidates_idxs, ids_of_objs_sorted_by_priority, candidates_cnt);
      }

      timing_info.track_downselection = get_wall_time() - start_time;
   }
}
