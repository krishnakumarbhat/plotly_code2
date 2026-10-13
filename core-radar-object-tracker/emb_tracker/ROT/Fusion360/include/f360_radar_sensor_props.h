/*===================================================================================*\
* FILE: f360_radar_sensor_props.h
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
*   This file contains F360_Sensor_Props_T structure declaration
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
#ifndef F360_RADAR_SENSOR_PROPS_H
#define F360_RADAR_SENSOR_PROPS_H

#include "f360_constants.h"
#include "f360_internal_reflection_buffer_slot.h"
#include "f360_look_ID.h"
#include "f360_reuse.h"
#include "f360_velocity.h"

namespace f360_variant_A
{
/******************************************************************************
 * Name:  F360_Radar_Sensor_Props_T
 *  Structure for radar sensor properties.
 *
 * Struct Members:
 *   internal_reflections_buffer              - Buffer containing internal reflection patterns
 *   interior_fov                             - [rad] Interior FOV angle for each look ID, i.e. the angle between the left and
 *                                              right FOV normals
 *   left_fov_normal                          - [rad] Normalized normal vector of the left FOV boundary for each look ID
 *                                              (point towards inside of FOV)
 *   right_fov_normal                         - [rad] Normalized normal vector of the right FOV boundary for each look ID
 *                                              (point towards inside of FOV)
 *   time_since_measurement_s                 - [s] Delta time between RSPP excution and sensor measurement timestamp
 *   low_power_clutter_severity               - Severity of low power clutter in the current radar frame, calculated as the
 *                                              ratio between the number of low power detections and the total number of
 *                                              detections
 *   rain_level_filtered                      - Filtered rain level based on the current and past low power clutter severity
 *                                              values
 *   next_to_sensor_object_track_id           - Object Track id of the vehicle next to the sensor (-1 for none)
 *   next_to_sensor_object_track_min_long_pos - [m] Minimum longitudinal position of the vehicle next to the sensor according to
 *                                              its object track
 *   next_to_sensor_object_track_max_long_pos - [m] Maximum longitudinal position of the vehicle next to the sensor according to
 *                                              its object track
 *   next_to_sensor_object_track_min_lat_pos  - [m] Minimum lateral position of the vehicle next to the sensor according to its
 *                                              object track
 *   next_to_sensor_object_track_max_lat_pos  - [m] Maximum lateral position of the vehicle next to the sensor according to its
 *                                              object track
 *   f_object_track_next_to_sensor            - Flag indicating there is a vehicle next to the sensor according to its
 *                                              object track
 ******************************************************************************/
typedef struct F360_Radar_Sensor_Props_Tag
{
   Internal_Reflection_Buffer_Slot internal_reflections_buffer[INTERNAL_REFLECTIONS_BUFFER_SIZE];

   // Refined properties
   float32_t interior_fov[F360_DET_NUM_LOOK_ID];
   float32_t left_fov_normal[F360_DET_NUM_LOOK_ID];
   float32_t right_fov_normal[F360_DET_NUM_LOOK_ID];
   float32_t time_since_measurement_s;

   // Rain countermeasure related signals
   float32_t low_power_clutter_severity;
   float32_t rain_level_filtered;

   // GD info
   int32_t next_to_sensor_object_track_id;
   float32_t next_to_sensor_object_track_min_long_pos;
   float32_t next_to_sensor_object_track_max_long_pos;
   float32_t next_to_sensor_object_track_min_lat_pos;
   float32_t next_to_sensor_object_track_max_lat_pos;
   bool f_object_track_next_to_sensor;

   uint8_t padding[3];
} F360_Radar_Sensor_Props_T;

static_assert((sizeof(F360_Radar_Sensor_Props_T::internal_reflections_buffer) + 84) == sizeof(F360_Radar_Sensor_Props_T),
              "sizeof(F360_Radar_Sensor_Props_T) not as expected. Remember to align padding if needed");
} // namespace f360_variant_A
#endif
