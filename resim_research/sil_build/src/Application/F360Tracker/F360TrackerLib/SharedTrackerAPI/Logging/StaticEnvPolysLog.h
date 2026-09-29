#ifndef STATIC_ENV_POLYS_LOG_H
#define STATIC_ENV_POLYS_LOG_H
/*===========================================================================*\
* FILE: StaticEnvPolysLog.h
*===========================================================================
* Copyright 2020 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains the F360 Tracker Static Environment Polynomials 
*   logging structure
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
static const int STATIC_ENV_POLYS_LOG_STREAM_NUM      = 19;
static const int STATIC_ENV_POLYS_LOG_STREAM_VERSION = 3;

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
typedef struct F360_Static_Env_Poly_Data_Log_Tag
{
   float confidence; // [-]
   float lower_limit; // [m] Lower validity limit
   float upper_limit; // [m] Upper validity limit
   float p0; // Polynomial zero order coefficient
   float p1; // Polynomial first order coefficient
   float p2; // Polynomial second order coefficient
   uint8_t age; // [scans] Saturates at 255
   uint8_t poly_type; // [0-3] (enum F360_Static_Env_Poly_Type_) F360_STATIC_ENV_POLY_TYPE_INVALID = 0, F360_STATIC_ENV_POLY_TYPE_CURVG = 1, F360_STATIC_ENV_POLY_TYPE_LSC = 2, F360_STATIC_ENV_POLY_TYPE_CWD = 3
   uint8_t status; // [0-2] (enum F360_Static_Env_Poly_Status_T) F360_STATIC_ENV_POLY_STATUS_INVALID = 0, F360_STATIC_ENV_POLY_STATUS_UPDATED = 1, F360_STATIC_ENV_POLY_STATUS_COASTED = 2,
   uint8_t unused;
} F360_Static_Env_Poly_Data_Log_T;

typedef struct F360_Static_Env_Poly_Log_Tag
{
   uint64_t timestamp_us;  //!< [us] time-stamp of current tracker iteration
   uint32_t tracker_index; //!< index of the current tracker iteration
   F360_Static_Env_Poly_Data_Log_T static_env_polys[NUM_OF_STATIC_ENV_POLYS];
} F360_Static_Env_Poly_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(F360_Static_Env_Poly_Log_T, 236U);

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
