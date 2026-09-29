#ifndef SG_TIMING_LOG_H
#define SG_TIMING_LOG_H
/*===========================================================================*\
* FILE: SGTIMINGLOG_H.h
*===========================================================================
* Copyright 2024 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains SG timing
*
* ABBREVIATIONS:
*
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

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
static const int SG_TIMING_LOG_STREAM_NUM  = 181;
static const int SG_TIMING_STREAM_VERSION  = 2;
static const uint8_t NUM_OF_ALGO_STEPS  = 5;
static const uint8_t NUM_OF_ALL_ALGO_STEPS  = 11;

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

   struct TimingDetails_T
   {
      uint64_t dc_steps[NUM_OF_ALGO_STEPS]; // [us] runtime array for each DC step.
   };
   struct SG_Timing_Dump_Log_T
   {
      uint64_t total;   // [us] total runtime of algo
      uint64_t main_steps[NUM_OF_ALL_ALGO_STEPS]; // [us] runtime array for each SG step.
      TimingDetails_T details; // [us] detailed information about sub steps runtime.
   };

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

static_assert(sizeof(SG_Timing_Dump_Log_T) == 136U, "SG_Timing_Dump_Log_T: Wrong size");


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
