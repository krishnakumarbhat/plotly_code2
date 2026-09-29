#ifndef TRAILER_DETECTOR_LOG_H
#define TRAILER_DETECTOR_LOG_H
/*===========================================================================*\
* FILE: TrailerDetectorLog.h
*===========================================================================
* Copyright 2023 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360 Trailer Detector log stream
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
static const int TRAILER_DETECTOR_LOG_STREAM_NUM      = 86;
static const int TRAILER_DETECTOR_LOG_STREAM_VERSION  = 2;

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
This structure is used to hold information about the static environment polynomial logging part
*/
typedef struct Trailer_Detector_Log_Tag
{
   float trailer_length;             // [m]
   float trailer_width;              // [m]
   float trailer_angle;              // [radian]
   float trailer_angle_rate;         // [radian/s]
   float trailer_HV_gap;             // [m]
   uint32_t radar_detection_timer;   // [-]
   uint32_t stationary_timer;        // [-]
   uint8_t trailer_detection_status; //0:NOT RUNNING, 1:RUNNING, 2:UNKNOWN
   uint8_t trailer_presence;         //0:NOT_DETECTED, 1:DETECTED, 2:UNKNOWN
   uint8_t trailer_length_conf;      //0:UNKNOWN, 1:LOW, 2:MEDIAN, 3:HIGH
   uint8_t trailer_presence_conf;    //0:UNKNOWN, 1:LOW, 2:MEDIAN, 3:HIGH
   uint8_t trailer_width_conf;       //0:UNKNOWN, 1:LOW, 2:MEDIAN, 3:HIGH
   uint8_t trailer_angle_conf;       //0:UNKNOWN, 1:LOW, 2:MEDIAN, 3:HIGH
   uint8_t trailer_angle_rate_conf;  //0:UNKNOWN, 1:LOW, 2:MEDIAN, 3:HIGH
   uint8_t padding;

} Trailer_Detector_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

static_assert(sizeof(Trailer_Detector_Log_T) == 36U, "Trailer_Detector_Log_T: Wrong size");

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
