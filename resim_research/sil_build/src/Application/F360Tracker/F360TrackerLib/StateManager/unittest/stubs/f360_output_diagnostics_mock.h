/*=========================================================================
*  FILE: f360_output_diagnostics_mock.h
*=========================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*========================================================================*/
#ifndef OUTPUT_DIAGNOSTICS_MOCK_H
#define OUTPUT_DIAGNOSTICS_MOCK_H

#include "f360_output_diagnostics.h"

namespace f360_variant_A
{
   class Output_Diagnostics_Mock : public Output_Diagnostics
   {
   public:
      virtual Output_Faults_T Execute(
         const F360_Object_Log_Output_T& obj_log,
         const Tracker_Info_Log_T& r_tracker_info) const;
      
   };
}

#endif
