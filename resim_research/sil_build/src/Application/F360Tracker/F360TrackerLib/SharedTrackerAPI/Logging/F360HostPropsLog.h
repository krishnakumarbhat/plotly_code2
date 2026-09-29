#ifndef F360_HOST_PROPS_LOG_T_H
#define F360_HOST_PROPS_LOG_T_H
/*===================================================================================*\
* FILE: F360HostPropsLog.h
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

#include "../Types/f360_reuse.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/

static const uint8_t F360_HOST_PROPS_LOG_STREAM_NUM = 14U;
static const uint8_t F360_HOST_PROPS_LOG_STREAM_VERSION = 2U;

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

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/

typedef struct F360_Host_Props_Log
{
   // Host props
   float position_x;                   //!< [m] x-position of host vehicle (or VCS origin of host vehicle) in world coordinate
   float position_y;                   //!< [m] y-position of host vehicle (or VCS origin of host vehicle) in world coordinate
   float heading;                   //!< [rad] heading angle at VCS original in world coordinate (speed = raw_speed * speed_correction_factor)
   float delta_pointing;            //!< [rad] Delta for host pointing angle between previous and current tracker iteration
   float delta_position_x;          //!< [m] Delta x-position of host vehicle between previous and current tracker iteration
   float delta_position_y;          //!< [m] Delta y-position of host vehicle between previous and current tracker iteration
} F360_Host_Props_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(F360_Host_Props_Log_T, 24U);

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
