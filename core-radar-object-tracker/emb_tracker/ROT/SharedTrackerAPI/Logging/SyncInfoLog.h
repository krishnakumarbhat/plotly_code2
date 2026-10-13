#ifndef SYNC_INFO_LOG_T_H
#define SYNC_INFO_LOG_T_H
/*===================================================================================*\
* FILE: SyncInfoLog.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "T360_Types.h"
#include "f360_reuse.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/

static const uint8_t SYNC_INFO_LOG_STREAM_NUM     = 1U;
static const uint8_t SYNC_INFO_LOG_STREAM_VERSION = 3U;

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
   #pragma pack(4)
#elif defined(__DCC__)
   #pragma pack(4, 4)
#elif defined(__TMS320C6X__)
   #pragma pack(push, 4)
#elif defined(_MSC_VER) || defined(__GNUC__)
   #pragma pack(push, save_pack, 4)
#else
#endif

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/

//! Flat timing-and-index record logged every tracker iteration (stream #1, v3).
//! Contains the iteration timestamp, per-sensor timestamps and look indices,
//! and the VSE iteration index used by SCL. The DRA alignment estimates and
//! speed-correction factor are recorded in Vehicle_Info_Log_T (stream #4).
typedef struct Sync_Info_Log_Tag
{
   uint64_t sensor_timestamp_us[NUM_TOTAL_RADAR_SENSORS]; //!< [us] timestamp for every sensor
   uint64_t tracker_timestamp_us;                         //!< [us] timestamp of tracker iteration
   float elapsed_time_s;                                  //!< [s]  time since previous tracker iteration
   uint32_t tracker_index;                                //!< index of tracker iteration
   uint32_t vse_index;                                    //!< VSE iteration index used by SCL
   uint16_t sensor_look_index[NUM_TOTAL_RADAR_SENSORS];   //!< look index for every sensor
} Sync_Info_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(Sync_Info_Log_T, 140U);

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

#endif
