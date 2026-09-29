/*===================================================================================\
 * FILE: sg_reuse.h
 *====================================================================================
 * Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains definition of standard integer types depending on compiler used,
 *   since integer definitions are platform dependent.
 *
 * Applicable Standards (in order of precedence: highest first):
 *   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *   ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*===================================================================================*/
#ifndef SG_REUSE_H
#define SG_REUSE_H

#if defined _MSC_BUILD /* Visual */
#include <cassert>
#include <cstdint>
#define __CPTC__ __cplusplus

#elif defined __TASKING__ /* Tasking Tricore */
#include <cstdint>

#elif defined __GNUC__ /* GNU */
#include <cstdint>
#include <cstdlib>
#define __CPTC__ __cplusplus

#elif defined __DCC__ /* Windriver */
#include <cassert>
#include <cstdint>
#include <cstdlib>

#else
#error Unrecognized platform!
#endif

#endif
