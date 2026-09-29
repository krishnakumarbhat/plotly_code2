#ifndef F360_SENSOR_CALIB_LOG_H
#define F360_SENSOR_CALIB_LOG_H
/*===================================================================================*\
* FILE:  f360_sensor_calib_log.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
*/

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

#include <f360_reuse.h>

static const int32_t F360_Sensor_Calib_Log_Stream_Num = 72;
static const int32_t F360_Sensor_Calib_Log_Stream_Ver = 4;
static const int32_t F360_Sensor_Calib_Log_Max_Payload_Size = 32000;

typedef struct F360_Sensor_Calib_Log_Tag
{
   // Sensor properties
   uint8_t id; // Sensor index (given by the tracker wrapper)
   int8_t sensor_type; // Type of sensor (F360_SENSOR)

   // Mounting properties
   int8_t mounting_location; // F360_MOUNT_LOC
   int8_t polarity; // 1 = normal, -1 = flipped
   float vcs_position_longitudinal;
   float vcs_position_lateral;
   float vcs_position_height;
   float vcs_boresight_azimuth_angle;
   float vcs_boresight_elevation_angle;

   float low_power_clutter_severity;
   float rain_level_filtered;

   // Detection properties
   float range_limits[4];
   float fov_min_az_rad[4];
   float fov_max_az_rad[4];
   float fov_min_el_rad[4];
   float fov_max_el_rad[4];
   float min_aliaised_range_rate[4];
   float v_wrapping[4];
   float r_wrapping[4];
} F360_Sensor_Calib_Log_T;

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif

#endif 
