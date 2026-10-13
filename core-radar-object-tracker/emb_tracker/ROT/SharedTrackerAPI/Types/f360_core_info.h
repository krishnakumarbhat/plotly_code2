#ifndef F360_CORE_INFO_VARIANT_A_H
#define F360_CORE_INFO_VARIANT_A_H
/*===================================================================================*\
 * FILE: f360_core_info.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 *   This file contains F360_Core_Info_T  structure declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s): defineFusion360Types.m
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

#include "f360_reuse.h"

namespace f360_variant_A
{
/******************************************************************************
 * Name:  F360_Core_Info_T
 *   Structure containing core timing and execution information.
 *
 * Struct Members:
 *   time_us            - [us] Current time stamp in microseconds.
 *   prev_time_us       - [us] Previous time stamp in microseconds.
 *   cnt_loops          - Number of times the core processing function has been executed.
 *   elapsed_time_s     - [s] Elapsed time since last core processing function call in seconds.
 ******************************************************************************/
typedef struct F360_Core_Info_Tag
{
   uint64_t time_us;
   uint64_t prev_time_us;
   float32_t elapsed_time_s;
   uint32_t cnt_loops;
} F360_Core_Info_T;

static_assert(24 == sizeof(F360_Core_Info_T), "sizeof(F360_Core_Info_T) not as expected. Remember to align padding if needed");
} // namespace f360_variant_A

#endif
