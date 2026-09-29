/*===================================================================================*\
* FILE: f360_tracker_info.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
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
*   This file contains F360_Tracker_Info_T  structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): defineFusion360Types.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_TRACKER_INFO_H
#define F360_TRACKER_INFO_H

#include "f360_constants.h"
#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_cluster.h"
#include "f360_variant.h"

namespace f360_variant_A
{
   typedef struct F360_Tracker_Info_Tag
   {
      uint64_t time_us;
      uint64_t object_list_timestamp; // [us] mean of min max of sensors timstamps
      F360_Object_Track_T* vcslong_sorted_start;
      F360_Object_Track_T* vcslong_sorted_first_infront_of_host;
      F360_Object_Track_T* vcslong_sorted_next_track[NUMBER_OF_OBJECT_TRACKS];
      F360_Object_Track_T* vcslong_sorted_prev_track[NUMBER_OF_OBJECT_TRACKS];
      F360_Object_Track_T* p_highest_priority_track;    // pointer to the track with greatest priority - most important [-]
      F360_Object_Track_T* p_lowest_priority_track;     // pointer to the track with lowest priority - least important [-]
      F360_Cluster_T* vcslong_sorted_cluster_start;
      F360_Cluster_T* vcslong_sorted_cluster_next[NUMBER_OF_CLUSTERS];
      F360_Cluster_T* vcslong_sorted_cluster_prev[NUMBER_OF_CLUSTERS];
      uint32_t cnt_loops;
      float32_t   elapsed_time_s;
      int32_t active_obj_ids[NUMBER_OF_OBJECT_TRACKS];
      int32_t inactive_obj_ids[NUMBER_OF_OBJECT_TRACKS];
      int32_t num_active_objs;
      int32_t reduced_active_obj_ids[NUMBER_OF_REDUCED_OBJECT_TRACKS];
      int32_t reduced_inactive_obj_ids[NUMBER_OF_REDUCED_OBJECT_TRACKS];
      int32_t reduced_obj_ids[NUMBER_OF_REDUCED_OBJECT_TRACKS];
      int32_t reduced_num_active_objs;
      uint32_t num_unique_objs;  // number of all unique objects ever created during this tracker execution
      float32_t   unique_rdot_interval_widths[MAX_NUM_UNIQUE_RDOT_INTERVAL];
      int32_t num_unique_rdot_interval_widths;
      float32_t low_power_clutter_severity;
      float32_t rain_level_filtered;  // average each radar's rain level flag filtered through time
      float32_t nr_suspected_stat_angle_jump_dets_filtered;  // number of suspected stationary angle jump detections filtered through time
      int16_t active_cluster_ids[NUMBER_OF_CLUSTERS];
      int16_t inactive_cluster_ids[NUMBER_OF_CLUSTERS];
      int16_t num_active_clusters;
      int16_t vcslong_sorted_cluster_list[NUMBER_OF_CLUSTERS];
      bool rdot_interval_compatibility[MAX_NUM_UNIQUE_RDOT_INTERVAL][MAX_NUM_UNIQUE_RDOT_INTERVAL];
      bool f_esr_sensor_valid;
      bool f_srr2_sensor_valid;
      bool f_low_power_clutter;
      bool f_severe_angle_jump_detected;  //flag that indicates whether severe angle jump phenomena is present
      bool f_highway_suspected;
      F360_Variant_T variant;
   } F360_Tracker_Info_T;
}
#endif
