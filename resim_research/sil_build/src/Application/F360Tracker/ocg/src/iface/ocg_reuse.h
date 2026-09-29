#ifndef OCG_REUSE_H
#define OCG_REUSE_H
/*===================================================================================*\
* FILE: ocg_reuse.h
*====================================================================================
* Copyright (C) 2023 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef LOGSIZE_ASSERT
#define LOGSIZE_ASSERT(expn) typedef char logsize_assertion[(expn)?1:-1]
#endif

#if defined __TASKING__
#ifndef _LIBCPP_CSTDINT
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
#endif

#elif defined __DCC__
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

#elif defined(__GNUC__) || defined(_MSC_BUILD)
#include <cstdint>
#include <cassert>

#else
#error Unrecognized platform!
#endif

#endif
