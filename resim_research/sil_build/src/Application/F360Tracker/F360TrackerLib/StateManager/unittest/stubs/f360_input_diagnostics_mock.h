/*=========================================================================
*  FILE: f360_input_diagnostics_mock.h
*=========================================================================
* Copyright (C) 2020 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*========================================================================*/
#ifndef INPUT_DIAGNOSTICS_MOCK_H
#define INPUT_DIAGNOSTICS_MOCK_H

#include "f360_input_diagnostics.h"

namespace f360_variant_A
{
   class Input_Diagnostics_Mock : public Input_Diagnostics
   {
   public:
       virtual Input_Faults_T Execute(
           const F360_Core_Info_T& core_info,
           const F360_Host_T& host,
           const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
           const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]);

      virtual void Initialize();
   };
}




#endif
