/*===================================================================================*\
* FILE: sg_timer_base.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains abstract class interface for externally provided timer
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef TIMER_BASE_H
#define TIMER_BASE_H

#include "sg_reuse.h"

namespace sg
{
   class TimerBase
   {
     public:
      virtual ~TimerBase()       = default;
      virtual uint64_t elapsed() = 0; // [us] return elapsed time from the timer start.
      virtual void restart()     = 0; // restart the internal timer to initiate the counting of elapsed time from zero.
   };
}

#endif
