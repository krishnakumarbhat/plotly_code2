/*===================================================================================*\
* FILE: f360_update_relative_timestamps.cpp
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*
*
* ABBREVIATIONS:
*   OTG	Over-The-ground
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*
*
* DEVIATIONS FROM STANDARDS:
*
*
\*==========================================================================================*/


/******************************
* Includes
*******************************/

#include "f360_update_relative_timestamps.h"
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: F360_Tracker::Update_Relative_Timestamps()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * None
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
   * constructor
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Relative_Timestamps(
      const float32_t elapsed_time_s,
      const F360_Tracker_Info_T & p_tracker_info,
      F360_Object_Track_T (&obj_trks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T & p_det_hist)
   {
      // time_since_measurement updated in Time_Update_Object_Timestamps() in Time_Update_Tracks()
      for (int32_t i = 0; i < p_tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = p_tracker_info.active_obj_ids[i] - 1;
         F360_Object_Track_T * const p_obj_trk = &obj_trks[obj_idx];

         p_obj_trk->time_since_cluster_created += (p_obj_trk->time_since_cluster_created < 0.0F) ? 0.0F : elapsed_time_s;
         p_obj_trk->time_since_track_updated += (p_obj_trk->time_since_track_updated < 0.0F) ? 0.0F : elapsed_time_s;
         p_obj_trk->time_since_stage_start += (p_obj_trk->time_since_stage_start < 0.0F) ? 0.0F : elapsed_time_s;
         p_obj_trk->time_since_initialization += (p_obj_trk->time_since_initialization < 0.0F) ? 0.0F : elapsed_time_s;
         p_obj_trk->time_since_last_stop += (p_obj_trk->time_since_last_stop < 0.0F) ? 0.0F : elapsed_time_s;
         p_obj_trk->time_since_started_move += (p_obj_trk->time_since_started_move < 0.0F) ? 0.0F : elapsed_time_s;
         p_obj_trk->time_since_split += (p_obj_trk->time_since_split < 0.0F) ? 0.0F : elapsed_time_s;
      }

      // time_since_measurement updated in Update_Cluster_Timestamp() in Init_Cluster()
      for (int16_t i = 0; i < p_tracker_info.num_active_clusters; i++)
      {
         const int16_t cluster_idx = p_tracker_info.active_cluster_ids[i] - 1;
         F360_Cluster_T * const p_cluster = &clusters[cluster_idx];

         p_cluster->time_since_cluster_updated += (p_cluster->time_since_cluster_updated < 0.0F) ? 0.0F : elapsed_time_s;
         p_cluster->time_since_measurement += (p_cluster->time_since_measurement < 0.0F) ? 0.0F : elapsed_time_s;
      }

      for (uint16_t i = 0U; i < p_tracker_info.variant.num_hist_dets; i++)
      {
         if (p_det_hist.f_idx_occupied[i])
         {
            p_det_hist.det_data[i].time_since_meas += (p_det_hist.det_data[i].time_since_meas < 0.0F) ? 0.0F : elapsed_time_s;
         }
      }
   }
}
