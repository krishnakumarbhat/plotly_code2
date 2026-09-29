/*=========================================================================
 *  FILE: f360_output_diagnostics_mock.cpp
 *=========================================================================
 * Copyright (C) 2020 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *========================================================================*/
#include <CppUTestExt/MockSupport.h>
#include "f360_output_diagnostics_mock.h"

namespace f360_variant_A
{
   Output_Faults_T Output_Diagnostics_Mock::Execute(
      const F360_Object_Log_Output_T &obj_log,
      const Tracker_Info_Log_T &r_tracker_info) const
   {
      Output_Faults_T default_return_value1{};
      mock().actualCall("Output_Diagnostics::Execute");
      return *(static_cast<Output_Faults_T *>(mock().returnPointerValueOrDefault(&default_return_value1)));
   }
}
