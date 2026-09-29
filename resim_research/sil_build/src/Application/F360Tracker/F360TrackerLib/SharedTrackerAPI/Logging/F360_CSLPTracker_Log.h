#ifndef F360_CSLP_TRACKER_LOG_H
#define F360_CSLP_TRACKER_LOG_H
/*****************************************************************************
* Copyright 2010-2017 Delphi Technologies, Inc., All Rights Reserved
*
* %full_name: F360_CSLPTracker_Log.h
* %created_by: krn
* %date_modified: 07 Mar 2017
*
* File Description:
*   This file contains type defintions for the F360 Tracker Log interface
*
*****************************************************************************/

/*************************
 *   Include Files       *
 *************************/
#include "f360_reuse.h"
#include "F360_TrackerOptions.h"
#include "Sensor_Calibrations.h"
#include "F360_CSLPTrackerChannelOptions.h"

#define F360_CSLP_TRACKER_INFO_LOG_STREAM_NUM ((uint8_t)30)
#define F360_CSLP_TRACKER_INFO_LOG_STREAM_VERSION ((uint8_t)28)

// How often should this struct be logged (once every F360_CSLP_TRACKER_INFO_LOG_CADENCE)
#define F360_CSLP_TRACKER_INFO_LOG_CADENCE ((uint8_t)100)

/* ---------------------- */
/* Force 4-Byte alignment */
/* ---------------------- */

#if !defined (__MINGW32__) && (defined (WIN32) || defined (_WIN32) ||  defined (_WIN64) ||  defined (__CYGWIN__) ) 
#  pragma pack(push,save_pack)
#  pragma pack(4)
#endif

typedef struct F360_CSLP_Tracker_Input_Log_Tag
{
   Sensor_Calibrations_T           sensor_calib;
   F360_CSLPTracker_CANChannelInfo can_info;
   LoggingOptions_T log_opt;
   ProcessOptions_T proc_opt;
} F360_CSLP_Tracker_Info_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(F360_CSLP_Tracker_Info_Log_T, 3240U);
LOGSIZE_ASSERT(Sensor_Calibrations_T, 3060U);
LOGSIZE_ASSERT(F360_CSLPTracker_CANChannelInfo, 132U);
LOGSIZE_ASSERT(ProcessOptions_T, 20U);
LOGSIZE_ASSERT(LoggingOptions_T, 25U);

/* -------------------------- */
/* End Force 4-byte alignment */
/* -------------------------- */
#if !defined (__MINGW32__) && (defined (WIN32) || defined (_WIN32) ||  defined (_WIN64) ||  defined (__CYGWIN__) ) 
#  pragma pack(pop,save_pack)
#endif
#endif
