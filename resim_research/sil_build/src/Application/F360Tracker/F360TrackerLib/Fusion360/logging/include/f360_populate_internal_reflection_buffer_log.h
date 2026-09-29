#ifndef F360_POPULATE_INTERNAL_REFLECTION_BUFFER_LOG_H
#define F360_POPULATE_INTERNAL_REFLECTION_BUFFER_LOG_H
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
#include "f360_internal_reflection_buffer.h"
#include "f360_radar_sensor_props.h"
#include "T360_Types.h"

namespace f360_variant_A
{
   void Populate_Internal_Reflection_Buffer_Data(F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      const F360_Internal_Reflection_Buffer_T(&reflection_buffer_log)[MAX_NUMBER_OF_SENSORS]);

   void Populate_Internal_Reflection_Buffer_Log_Data(F360_Internal_Reflection_Buffer_T(&reflection_buffer_log)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS]);
}

#endif 
