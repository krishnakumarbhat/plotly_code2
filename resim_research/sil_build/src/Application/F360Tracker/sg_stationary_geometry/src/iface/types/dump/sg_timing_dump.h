/*===================================================================================*\
* FILE: sg_timing_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes i.e. timing dump type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef SG_TIMING_DUMP_H
#define SG_TIMING_DUMP_H

#include "sg_algorithm_step.h"
#include "sg_timing_details.h"

namespace sg
{
   static const SG_LogStreamInfo_T timing_dump_stream{181U, 2U};

   struct SG_Timing_Dump_T
   {
      uint64_t total{};                                                                   // [us] total runtime of algo
      uint64_t main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::NUM_OF_ALGO_STEPS)]{}; // [us] runtime array for each SG step.
      TimingDetails_T details{}; // [us] detailed information about sub steps runtime.
   };
}

#endif
