/*===================================================================================*\
* FILE: f360_mark_objects_entering_dead_zone.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Mark_Objects_Enterning_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_mark_objects_entering_dead_zone.h"
#include "f360_define_dead_zone.h"
#include "f360_mark_objects_entering_dead_zone_helpers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Mark_Objects_Enterning_Dead_Zone()
   *===========================================================================
   * RETURN VALUE:
   * F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS] - updates objects
   *                                                          dead_zone_status field.
   *
   * PARAMETERS:
   * const F360_Host_T& host - estimated host parameters
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - sensor calibrations
   * const F360_Calibrations_T& calib - tracker calibrations
   * const F360_Tracker_Info_T& tracker_info - tracker properties
   * F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS] - tracked objects
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
   * Function verifies whether objects are not entering host 'dead zone' - zone where
   * sensors are unable to detect anything.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Objects_Enterning_Dead_Zone(
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calib,
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      if (calib.k_dead_zone_min_host_speed < host.speed)
      {
         const Dead_Zone_T dead_zone = Define_Dead_Zone(sensors, calib.k_dead_zone_long_limit_extension);

         for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
         {
            const int32_t track_idx = tracker_info.active_obj_ids[i] - 1;
            objects[track_idx].dead_zone_status = Determine_Object_Dead_Zone_Status(dead_zone,
                                                                                    objects[track_idx],
                                                                                    calib.k_dead_zone_max_obj_vcs_lat_pos,
                                                                                    calib.k_dead_zone_max_obj_vcs_heading);
         }
      }
      else
      {
         for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
         {
            const int32_t track_idx = tracker_info.active_obj_ids[i] - 1;
            objects[track_idx].dead_zone_status = F360_Dead_Zone_Status_T::UNDEFINED;
         }
      }

   }
}
