#ifndef F360_REUSE_H
#define F360_REUSE_H
/*===================================================================================*\
 * FILE: f360_reuse.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION
 *
 * ABBREVIATIONS
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *
 *   Requirements Document(s):
 *
 *   Applicable Standards (in order of precedence: highest first):
 *  - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *  - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#ifndef LOGSIZE_ASSERT
   #define LOGSIZE_ASSERT(type_name, size) typedef char type_name##_size_check_struct[1 - 2 * !!(sizeof(type_name) != (size))]
#endif

#if defined(__TASKING__)
   #ifndef _LIBCPP_CSTDINT
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
   #endif

#elif defined(__DCC__) || defined(__TMS320C6X__)
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#elif defined(__GNUC__) || defined(_MSC_BUILD)
   #ifdef __cplusplus
      #include <cassert>
      #include <cstdint>
   #else
      #include <assert.h>
      #include <stdint.h>
   #endif

#else
   #error Unrecognized platform!
#endif

typedef float float32_t; // IEE754 single-precision
/* bit fields */
typedef unsigned char bitfield8_t;   /* 8 bit bitfields */
typedef unsigned short bitfield16_t; /* 16 bit bitfields */
typedef unsigned int bitfield32_t;   /* 32 bit bitfields */

#endif
