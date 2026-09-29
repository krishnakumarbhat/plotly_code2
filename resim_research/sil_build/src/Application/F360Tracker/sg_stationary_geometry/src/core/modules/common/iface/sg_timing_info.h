/*===================================================================================*\
* FILE: sg_timing_info.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains class for runtime measurement purposes.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef TIMING_INFO_H
#define TIMING_INFO_H

#include "sg_algorithm_step.h"
#include "sg_timer_base.h"
#include "sg_timing_details.h"

namespace sg
{
   class TimingInfo
   {
     public:
      uint64_t total;                                                                   // [us] total runtime of algo
      uint64_t main_steps[static_cast<uint8_t>(SG_AlgorithmStep_T::NUM_OF_ALGO_STEPS)]; // [us] runtime array for each SG main step.
      TimingDetails_T details; // [us] detailed information about sub steps runtime.

      TimingInfo(TimerBase *const _timer = nullptr) : total{}, main_steps{}, details(), timer{_timer}
      {
      }

      inline uint64_t elapsed() // [us] return elapsed time if valid external timer is provided
      {
         return (timer != nullptr) ? timer->elapsed() : 0ULL;
      }

      inline void restart() // restart timer if valid external timer is provided
      {
         if (timer != nullptr)
         {
            timer->restart();
         }
      }

      /**
       * @brief      Reset all fields
       *
       * return      N/A
       **/
      void reset()
      {
         total = 0U;
         std::fill(std::begin(main_steps), std::end(main_steps), 0U);
         std::fill(std::begin(details.dc_steps), std::end(details.dc_steps), 0U);
      }

     private:
      TimerBase *const timer;
   };
}

#endif
