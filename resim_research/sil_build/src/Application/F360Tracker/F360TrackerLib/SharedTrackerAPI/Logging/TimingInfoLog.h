#ifndef TIMING_INFO_LOG_H
#define TIMING_INFO_LOG_H
/*===========================================================================*\
* FILE: TimingInfoLog.h
*===========================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains the F360 Tracker Timing Information
*
* ABBREVIATIONS:
*   TODO: List of abbreviations used, or reference(s) to external document(s)
*
* TRACEABILITY INFO:
*   Design Document(s):
*   Requirements Document(s): PDD-10024333-012_(CADS4_VFP_Ethernet_Communication).doc
*   (Design & Requirements)
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* CHANGE SYNERGY INFO:
* %full_filespec:   TimingInfoLog.h %
* %version:         1 %
* %date_modified:   June 27, 2017 %
*
* DEVIATIONS FROM STANDARDS:
*   None
*
\*===========================================================================*/


#include "f360_reuse.h"
/*===========================================================================*\
* Other Header Files
\*===========================================================================*/


/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
static const int TIMING_INFO_LOG_STREAM_NUM = 9;
static const int TIMING_INFO_LOG_STREAM_VERSION = 22;

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack(4)
#elif defined(__DCC__)
#pragma pack(4,4)
#elif defined(__TMS320C6X__)
#pragma pack(push,4)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, save_pack, 4)
#else
#endif

/**
This structure is used to hold information about execution time
*/
typedef struct ItemizedExecutionTiming_Log_Tag {
   float host_state_update;
   float calc_obj_mov_stat_thresh;
   float update_sensor_valid_info;
   float configure_rdot_interval_compability;
   float sensor_motion;
   float update_det_hist;
   float ego_motion_compensate_dets;
   float detect_wheelspin;
   float check_az_el_conf;
   float mark_out_det_pairs;
   float sensor_capability_module;
   float double_bounce_detection_countermeasure;

   float time_update_obj_trks_cca;
   float time_update_obj_trks_ctca;

   float split_tracks;
   float kill_coasted_tracks;

   float assoc_dets_with_obj_trks;
   float assoc_countermeasure;
   float store_det_data;
   float det_downselect;
   float cluster_moving_detections;
   float cluster_leftover_detections;
   float initialize_clusters; 
   float assoc_unconf_obj_trks;
   float merge_tracks;

   float pseudo_estimations;
   float initialize_tracked_objects;

   float obj_trk_status_bookkeeping;
   float adjust_fltr_type_dependent_params;
   float obj_trk_properties;
   float cancel_new_updated_trk_overlapping_confirmed_trks;
   float adjust_overlapping_confirmed_trks;
   float det_mean_var;
   float object_underdrivability_classification;
   float assign_underdrivability_status_to_tracks_ocg;
   float determine_underdrivable_for_movable;

   float mark_object_tracks_next_to_sensors;

   float sanity_check;

   float reserve1;
   float reserve2;
   float reserve3;
   float reserve4;
   float reserve5;

   float msmt_update_obj_trks_cca_moveable;
   float msmt_update_obj_trks_cca_non_moveable;
   float msmt_update_obj_trks_cca;
   float msmt_update_obj_trks_ctca;

   float assoc_unconf_obj_trks_sub_time_1;
   float assoc_unconf_obj_trks_sub_time_2;
   float assoc_unconf_obj_trks_sub_time_3;
   float assoc_unconf_obj_trks_sub_time_4;
   float assoc_unconf_obj_trks_sub_time_5;
   float assoc_unconf_obj_trks_sub_time_6;

} ItemizedExecutionTiming_Log_T;

typedef struct OverallTiming_Log_Tag
{
   float time_taken_core_tracker;  //!< (Seconds) Time taken by Tracker PC to run tracker core functions in the current iteration
   float time_taken_core_and_udp_packing;  //!< (Seconds) Time taken by Tracker PC to run tracker core functions and packing UDP streams, in the current iteration
   float time_taken_full_tracker_DLL_func; //!< (Seconds) Time taken by Tracker PC to latch sensor data, run tracker core functions and packing UDP streams, in the current iteration 
   float inputs_preprocessing;
   float internal_preprocessing;
   float time_update_tracks;
   float trailer_detector;
   float pre_association_track_management;
   float detection_to_track_association;
   float clustering;
   float cluster_grouping;
   float track_grouping;
   float measurement_update_tracks;
   float initialize_tracks;
   float post_update_track_adjustments;
   float track_classification;
   float track_validity;
   float track_downselection;
   float e2e_protection;
   float occlusion;
   float lsc_module;
   float concrete_wall_detector;
   float sensor_postprocessing;
}OverallTiming_Log_T;

typedef struct Timing_Info_Log_Tag {
   uint32_t tracker_index; //!< index of the current tracker iteration
   float timestamp;           //!< [seconds] time-stamp of current tracker iteration
   float prev_timestamp;      //!< [seconds] time-stamp of previous tracker iteration
   ItemizedExecutionTiming_Log_T itemized_execution_times;
   OverallTiming_Log_T overall_execution_times;
} Timing_Info_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(Timing_Info_Log_T, 316U);

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack()
#elif defined(__DCC__)
#pragma pack(0)
#elif defined(__TMS320C6X__)
#pragma pack(pop)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop, save_pack)
#else
#endif

#endif /* TIMING_INFO_LOG_H */
