#ifndef F360_TRACKER_VERSION_VARIANT_A_H
#define F360_TRACKER_VERSION_VARIANT_A_H
/*===================================================================================*\
 * FILE: f360_tracker_version.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *
 *   Requirements Document(s):
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#include "f360_reuse.h"

namespace f360_variant_A
{
static const int8_t Tracker_Version_Major = 11;
static const int8_t Tracker_Version_Minor = 1;
static const int8_t Tracker_Version_Patch = 0;
} // namespace f360_variant_A

#ifndef F360_TRACKER_VERSION_BUILD_ID
   #define F360_TRACKER_VERSION_BUILD_ID 0x0000000
#endif

#endif
