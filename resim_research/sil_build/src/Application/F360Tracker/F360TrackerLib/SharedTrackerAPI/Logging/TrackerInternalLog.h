#ifndef F360_TRACKER_INTERNAL_LOG_T_H
#define F360_TRACKER_INTERNAL_LOG_T_H
/*===========================================================================*\
 * FILE: TrackerInternalLog.h
 *===========================================================================
 * Copyright 2021 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 *---------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the F360 Tracker Information
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *   Requirements Document(s): PDD-10024333-012_(CADS4_VFP_Ethernet_Communication).doc
 *   (Design & Requirements)
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*===========================================================================*/

#include "../Types/f360_reuse.h"

/*===========================================================================*\
* Other Header Files
\*===========================================================================*/
#include "../Types/T360_Types.h"
#include "f360_internal_detection_history.h"
#include "f360_internal_cluster.h"
#include "f360_internal_object.h"
#include "f360_internal_cwd.h"
#include "f360_internal_underdrivability.h"
#include "f360_internal_reflection_buffer.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
#define TRACKER_INTERNAL_LOG_STREAM_NUM (11)
#define TRACKER_INTERNAL_LOG_STREAM_VERSION (31)
#define TRACKER_INTERNAL_NUM_CHUNKS (125) // Select an even divisor to the full size
#define INTERNAL_REFLECTIONS_BUF_SIZE (64)

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__)
#pragma pack(4,4)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, save_pack, 4)
#else
#endif

typedef struct Internal_VCS_Position_Tag
{
   float longitudinal;
   float lateral;
   float height;
} Internal_VCS_Position_T;

typedef struct Internal_Detection_Posn_Hist_Tag
{
   ::Internal_VCS_Position_T vcs_position[F360_MAX_HIST_DETS_IN_OBJ_TRK];
   float time_since_meas[F360_MAX_HIST_DETS_IN_OBJ_TRK];
   int32_t start_idx;
   int32_t ndets;
   int32_t trk_idx;
   uint8_t f_reserved;
   uint8_t unused[3];
} Internal_Detection_Posn_Hist_T;

/**
This structure is used to hold information about the tracker.
*/
typedef struct Tracker_Internal_Log_Tag
{
   F360_Internal_Detection_Hist_T det_hist[F360_MAX_NUMBER_OF_HISTORIC_DETECTIONS];
   ::Internal_Detection_Posn_Hist_T det_posn_hist[F360_NUM_POSN_HIST_SLOTS];
   F360_Internal_Cluster_T clusters[MAX_F360_CLUSTERS];
   F360_Internal_Object_T obj[MAX_F360_OBJECTS];
   F360_Internal_CWD_T cwd[NUM_TOTAL_RADAR_SENSORS];
   F360_Internal_Reflection_Buffer_T reflection_buffer[NUM_TOTAL_RADAR_SENSORS];
   F360_Internal_Underdrivability_T ud_zone[NUMBER_OF_OVERHEAD_ZONES];
   float up_host_travel_distance;
   uint16_t up_circular_buffer_idx;
   uint8_t unused[30];
} Tracker_Internal_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************
static_assert(sizeof(Tracker_Internal_Log_T) == (413500), "Tracker_Internal_Log_T: Wrong size");
static_assert(sizeof(F360_Internal_Detection_Hist_T) == (60), "F360_Internal_Detection_Hist_T: Wrong size");
static_assert(sizeof(::Internal_Detection_Posn_Hist_T) == (1296), "Internal_Detection_Posn_Hist_T: Wrong size");
static_assert(sizeof(F360_Internal_Cluster_T) == (48), "F360_Internal_Cluster_T: Wrong size");
static_assert(sizeof(F360_Internal_Object_T) == (148), "F360_Internal_Object_T: Wrong size");
static_assert(sizeof(F360_Internal_CWD_T) == (32), "F360_Internal_CWD_T: Wrong size");
static_assert(sizeof(F360_Internal_Underdrivability_T) == (144), "F360_Internal_Underdrivability_T: Wrong size");   // 7208
static_assert(sizeof(F360_Internal_Reflection_Buffer_T) == (1280), "F360_Internal_Reflection_Buffer_T: Wrong size");   // 12288
static_assert((sizeof(Tracker_Internal_Log_T) / TRACKER_INTERNAL_NUM_CHUNKS)* TRACKER_INTERNAL_NUM_CHUNKS == sizeof(Tracker_Internal_Log_T), "Tracker_Internal_Log_T: Num chunks is not an even divisor of the full size");
static_assert((sizeof(Tracker_Internal_Log_T) / TRACKER_INTERNAL_NUM_CHUNKS) < 32000, "Tracker_Internal_Log_T: Max allowed payload size exceeded"); // Max allowed payload size is 32000

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__)
#pragma pack(0)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop, save_pack)
#else
#endif


#endif /* F360_TRACKER_INFO_LOG_T_H */
