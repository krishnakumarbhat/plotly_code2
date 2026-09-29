/*=========================================================================
*  FILE: f360_input_diagnostics_mock.cpp
*=========================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*========================================================================*/
#include <CppUTestExt/MockSupport.h>
#include "f360_input_diagnostics_mock.h"

namespace f360_variant_A
{
   Input_Faults_T Input_Diagnostics_Mock::Execute(
      const F360_Core_Info_T& core_info,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      Input_Faults_T default_return_value{};
      mock().actualCall("Input_Diagnostics::Execute");
      return *(static_cast<Input_Faults_T*>(mock().returnPointerValueOrDefault(&default_return_value)));
   }

   void Input_Diagnostics_Mock::Initialize()
   {
      mock().actualCall("Input_Diagnostics::Initialize");
   }
}
