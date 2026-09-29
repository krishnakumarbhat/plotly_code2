#ifndef F360_TRACKER_INFO_LOG_T_H
#define F360_TRACKER_INFO_LOG_T_H
/*===========================================================================*\
 * FILE: AllObjectsLog.h
 *===========================================================================
 * Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 *---------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the F360 Tracker All Objects Information
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
 *     SW REF 264.15D "Delphi C Coding Standards" [12-Mar-2006]
 *
 * CHANGE SYNERGY INFO:
 * %full_filespec:   AllObjectsLog.h %
 * %version:         1 %
 * %date_modified:   Jan 5, 2017 %
 *
 * DEVIATIONS FROM STANDARDS:
 *   None
 *
\*===========================================================================*/


#include "../Types/f360_reuse.h"
/*===========================================================================*\
 * Other Header Files
\*===========================================================================*/
#include "../Types/T360_Types.h"

/*===========================================================================*\
 * Exported local (file scope) Constants
\*===========================================================================*/
#define TRACKER_INFO_LOG_STREAM_NUM 7
#define TRACKER_INFO_LOG_STREAM_VERSION 9

/**
This structure is used to hold information about the tracker.
*/
typedef struct Tracker_Info_Log_Tag
{
   uint64_t sw_version_buildID; //!< tracker software build identification

   uint64_t timestamp_us;                        //!< [us] time-stamp of current tracker iteration.
   uint64_t object_list_timestamp;               //!< [us] object list timestamp related to measurement timestamp
   float    elapsed_time_s;                      //!< [s] time since previous tracker iteration.
   float nr_suspected_stat_angle_jump_dets_filtered; //!< filtered number of suspicious angle jump detections
   uint32_t tracker_index;                       //!< index of the current tracker iteration.

   uint32_t num_unique_objs;                               //!< [0 - 2^32-1] number of all unique objects ever created during this tracker execution.
   uint16_t active_obj_ids[MAX_F360_OBJECTS];              //!< [0-250] nonzero numbers indicate IDs of active tracks.
   uint16_t inactive_obj_ids[MAX_F360_OBJECTS];            //!< [0-250] nonzero numbers indicate IDs of inactive tracks.
   uint16_t num_active_objs;                               //!< [0-250] number of active tracks in current frame.
   uint16_t reduced_active_obj_ids[MAX_REDUCED_OBJECTS];   //!< [0-125] nonzero numbers indicate IDs of active downselected(reduced) tracks.
   uint16_t reduced_inactive_obj_ids[MAX_REDUCED_OBJECTS]; //!< [0-125] nonzero numbers indicate IDs of inactive downselected(reduced) tracks.
   uint16_t reduced_obj_ids[MAX_REDUCED_OBJECTS];          //!< [0-250] mapping from downselected track ID to original track ID.
                                                           //!< Value of i-th element is the track ID of the object whose downselected(reduced) track ID is i+1.
   uint16_t reduced_num_active_objs;                       //!< [0-125] number of active downselected(reduced) tracks in current frame.
   uint16_t num_active_clusters;                           //!< number of active clusters in current frame.
   uint16_t number_of_historic_detections;                 //!< number of historic detections in current frame.

   uint8_t f_severe_angle_jump_detected;                      //!< signal indicating that severe angle jump phenomena is present 

   uint8_t sw_version_major;   //!< tracker software major version
   uint8_t sw_version_minor;   //!< tracker software minor version
   uint8_t sw_version_patch;   //!< tracker software patch version

   uint8_t tracker_variant_type;

   uint8_t padding_1;
   uint8_t padding_2;
   uint8_t padding_3;

} Tracker_Info_Log_T;


//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(Tracker_Info_Log_T, 7056U);

#endif /* F360_TRACKER_INFO_LOG_T_H */
