#ifndef F360_TRACKER_INFO_LOG_H
#define F360_TRACKER_INFO_LOG_H
/*===================================================================================*\
 * FILE:  TrackerInfoLog.h
 *====================================================================================
 * Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "f360_reuse.h"

#define TRACKER_INFO_LOG_STREAM_NUM     7
#define TRACKER_INFO_LOG_STREAM_VERSION 10

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
   #pragma warning(push)
   #pragma warning(error:4820)
#elif 0
   #pragma GCC diagnostic push
   #pragma GCC diagnostic error "-Wpadded"
#endif

typedef struct Tracker_Info_Log_Tag
{
   uint64_t sw_version_buildID; //!< tracker software build identification

   uint64_t timestamp_us;                            //!< [us] time-stamp of current tracker iteration.
   uint32_t tracker_index;                           //!< index of the current tracker iteration.
   float elapsed_time_s;                             //!< [s] time since previous tracker iteration.
   float nr_suspected_stat_angle_jump_dets_filtered; //!< filtered number of suspicious angle jump detections

   uint32_t num_unique_objs;         //!< [0 - 2^32-1] number of all unique objects ever created during this tracker execution.
   uint16_t num_active_objs;         //!< number of active tracks in current frame.
   uint16_t reduced_num_active_objs; //!< number of active downselected(reduced) tracks in current frame.
   uint16_t num_active_clusters;     //!< number of active clusters in current frame.
   uint16_t number_of_historic_detections; //!< number of historic detections in current frame.

   uint8_t f_severe_angle_jump_detected; //!< signal indicating that severe angle jump phenomena is present

   uint8_t sw_version_major; //!< tracker software major version
   uint8_t sw_version_minor; //!< tracker software minor version
   uint8_t sw_version_patch; //!< tracker software patch version

   uint8_t tracker_variant_type;
   uint8_t unused[3];

} Tracker_Info_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(Tracker_Info_Log_T, 48U);

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
   #pragma warning(pop)
#elif 0
   #pragma GCC diagnostic pop
#endif

#endif
