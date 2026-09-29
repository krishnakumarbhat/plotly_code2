/*===================================================================================*\
* FILE: f360_previous_cycle_info.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*
* DESCRIPTION:
*   This file contains F360_Previous_Cycle_Info_T structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWx_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_PREVIOUS_CYCLE_INFO_VARIANT_A_H
#define F360_PREVIOUS_CYCLE_INFO_VARIANT_A_H

#include "../Types/f360_reuse.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   struct Core_Info_Reduced_T
   {
      uint64_t time_us;
      uint32_t cnt_loops;
      uint8_t padding[4];
   };

   struct Host_Info_Reduced_T
   {
      uint32_t vehicle_index;
   };

   struct Radar_Sensor_Reduced_T
   {
      uint64_t timestamp_us;
      uint16_t look_index;
      uint8_t padding[6];
   };

   struct Previous_Cycle_Info_T
   {
      Radar_Sensor_Reduced_T sensors[MAX_NUMBER_OF_SENSORS];
      Core_Info_Reduced_T core_info;
      Host_Info_Reduced_T host_info;
      bool valid;
      uint8_t padding[3];
   };

   static_assert(16 == sizeof(Core_Info_Reduced_T), "sizeof(Core_Info_Reduced_T) not as expected. Remember to align padding if needed");
   static_assert(4 == sizeof(Host_Info_Reduced_T), "sizeof(Host_Info_Reduced_T) not as expected. Remember to align padding if needed");
   static_assert(16 == sizeof(Radar_Sensor_Reduced_T), "sizeof(Radar_Sensor_Reduced_T) not as expected. Remember to align padding if needed");
   static_assert((sizeof(Previous_Cycle_Info_T::sensors) + 24) == sizeof(Previous_Cycle_Info_T), "sizeof(Previous_Cycle_Info_T) not as expected. Remember to align padding if needed");
}


#endif
