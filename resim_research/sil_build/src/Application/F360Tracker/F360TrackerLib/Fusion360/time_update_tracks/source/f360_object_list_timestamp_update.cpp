/*===================================================================================*\
* FILE: f360_object_list_timestamp_update.cpp
*====================================================================================
* Copyright 2020 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains funcions to update object list timestamp in tracker info.
*
* ABBREVIATIONS:
*   NONE
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]*
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_object_list_timestamp_update.h"
#include "f360_math_func.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * Internal Function Prototypes
   \*===========================================================================*/

   static uint8_t Fill_Valid_Sensors_Timestamp(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      uint64_t (&valid_sensors_timestamp)[MAX_NUMBER_OF_SENSORS]
   );

   /*===========================================================================*\
   * FUNCTION: Object_List_Timestamp_Update()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * F360_Tracker_Info_T& tracker_info
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function updates object list timestamp in tracker info
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Object_List_Timestamp_Update(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info)
   {
      uint64_t valid_sensors_timestamp[MAX_NUMBER_OF_SENSORS] = {};
      const uint16_t num_of_valid_sensors = Fill_Valid_Sensors_Timestamp(sensors, valid_sensors_timestamp);

      if (num_of_valid_sensors > 0U)
      {
         const uint64_t min_timestamp = *std::min_element(&valid_sensors_timestamp[0], &valid_sensors_timestamp[num_of_valid_sensors]);
         const uint64_t max_timestamp = *std::max_element(&valid_sensors_timestamp[0], &valid_sensors_timestamp[num_of_valid_sensors]);
         tracker_info.object_list_timestamp = min_timestamp + ((max_timestamp - min_timestamp) / 2U);
      }
      else if (tracker_info.object_list_timestamp != 0U)
      {
         const float32_t elapsed_time_micro_s = tracker_info.elapsed_time_s * 1.0e6F;
         tracker_info.object_list_timestamp += static_cast<uint64_t>(elapsed_time_micro_s);
      }
      else
      {
         tracker_info.object_list_timestamp = 0U;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Fill_Valid_Sensors_Timestamp()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
   * uint64_t (&valid_sensors_timestamp)[MAX_NUMBER_OF_SENSORS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function is looking for valid sensors
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static uint8_t Fill_Valid_Sensors_Timestamp(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      uint64_t (&valid_sensors_timestamp)[MAX_NUMBER_OF_SENSORS]
   )
   {
      uint8_t num_of_valid_sensors = 0U;
      for (uint8_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         if ((sensors[sensor_idx].variable.look_id != F360_DET_LOOK_ID_INVALID) && (sensors[sensor_idx].variable.is_valid) && (!sensors[sensor_idx].variable.f_sensor_fault_detected))
         {
            valid_sensors_timestamp[num_of_valid_sensors] = sensors[sensor_idx].variable.timestamp_us;
            num_of_valid_sensors++;
         }
      }
      return num_of_valid_sensors;
   }
}
