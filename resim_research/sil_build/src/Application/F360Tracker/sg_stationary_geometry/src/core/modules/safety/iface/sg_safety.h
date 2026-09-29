/*=============================================================================================*\
* FILE: sg_safety.h
* ====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration for safety logic class
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_SAFETY_H
#define SG_SAFETY_H

#include "sg_input.h"
#include "sg_output.h"
#include "sg_reduced_output.h"

namespace sg
{
   class Safety
   {
     public:
      void diagnose(const SG_Input_T &input);
      void diagnose(const SG_Output_T &output);
      void diagnose(const SG_ReducedOutput_T &output);
      void clear_faults();
      bool is_critical_fault_detected() const;
   };
}
#endif