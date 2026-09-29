/*===================================================================================*\
* FILE: f360_globals.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Globals_T  structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): defineFusion360Types.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_GLOBALS_H
#define F360_GLOBALS_H

#include "f360_reuse.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   typedef struct F360_Globals_Tag
   {
      float32_t obj_mov_stat_spd_thresh;
      float32_t rear_cornering_compliance;
      float32_t oncoming_speed_thresh;
      float32_t obj_vehicular_spd_thresh;
      float32_t default_half_length;
      float32_t default_half_width;
      float32_t default_length;
      float32_t default_width;
      float32_t default_height;
      float32_t max_otg_speed;
      float32_t rotated_left_fov_normal[MAX_NUMBER_OF_SENSORS][2]; // indexing: [sensor_idx][0 -> x-component of the vector for the current look id, 1 -> y-component]
      float32_t rotated_right_fov_normal[MAX_NUMBER_OF_SENSORS][2]; // indexing: [sensor_idx][0 -> x-component of the vector for the current look id, 1 -> y-component]
      float32_t four_corner_sensor_config_min_longitudinal_position;
      float32_t four_corner_sensor_config_max_longitudinal_position;
      float32_t average_sensor_position_x;
      float32_t average_sensor_position_y;
      bool f_single_front_center_radar_only;
      bool f_four_corner_sensors_available;
      bool f_front_or_front_corner_sensor_available;
      bool f_rear_sensor_available;
   }F360_Globals_T;

   static_assert((
         sizeof(((F360_Globals_T*)0)->rotated_left_fov_normal)
       + sizeof(((F360_Globals_T*)0)->rotated_right_fov_normal)
      + 60
       ) == sizeof(F360_Globals_T), "sizeof(F360_Globals_T) not as expected. Remember to align padding if needed");
}
#endif
