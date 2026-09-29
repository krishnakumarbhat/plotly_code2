/*===================================================================================*\
* FILE:  f360_populate_internal_reflection_buffer_log.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/
#include "f360_populate_internal_reflection_buffer_log.h"

namespace f360_variant_A
{
   void Populate_Internal_Reflection_Buffer_Data(F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      const F360_Internal_Reflection_Buffer_T(&reflection_buffer_log)[MAX_NUMBER_OF_SENSORS])
   {
      for (uint8_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         for (uint8_t buffer_idx = 0U; buffer_idx < INTERNAL_REFLECTIONS_BUFFER_SIZE; buffer_idx++)
         {
            sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].azimuth = reflection_buffer_log[sensor_idx].azimuth[buffer_idx];
            sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].rcs = reflection_buffer_log[sensor_idx].amplitude[buffer_idx];
            sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].range = reflection_buffer_log[sensor_idx].range[buffer_idx];
            sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].occurrence_count = reflection_buffer_log[sensor_idx].occurrence_count[buffer_idx];
            sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].age = reflection_buffer_log[sensor_idx].age[buffer_idx];
         }
      }
   }

   void Populate_Internal_Reflection_Buffer_Log_Data(F360_Internal_Reflection_Buffer_T(&reflection_buffer_log)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS])
   {
      for (uint8_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         for (uint8_t buffer_idx = 0U; buffer_idx < INTERNAL_REFLECTIONS_BUFFER_SIZE; buffer_idx++)
         {
            reflection_buffer_log[sensor_idx].azimuth[buffer_idx] = sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].azimuth;
            reflection_buffer_log[sensor_idx].amplitude[buffer_idx] = sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].rcs;
            reflection_buffer_log[sensor_idx].range[buffer_idx] = sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].range;
            reflection_buffer_log[sensor_idx].occurrence_count[buffer_idx] = sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].occurrence_count;
            reflection_buffer_log[sensor_idx].age[buffer_idx] = sensor_props[sensor_idx].internal_reflections_buffer[buffer_idx].age;
         }
      }
   }
}
