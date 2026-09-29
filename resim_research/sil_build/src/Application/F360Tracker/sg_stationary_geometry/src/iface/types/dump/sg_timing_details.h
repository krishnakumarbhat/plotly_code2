/*===================================================================================*\
* FILE: sg_timing_details.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains structure definition for run-time of detailed modules or functionalities.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_TIMING_DETAILS_H
#define SG_TIMING_DETAILS_H

#include <cstdint>

#include "dc_algorithm_step.h"

namespace sg
{
   struct TimingDetails_T
   {
      std::uint64_t dc_steps[static_cast<std::uint8_t>(DC_AlgorithmStep_T::NUM_OF_ALGO_STEPS)]{}; // [us] runtime array for each DC
                                                                                                  // step.
   };
}

#endif
