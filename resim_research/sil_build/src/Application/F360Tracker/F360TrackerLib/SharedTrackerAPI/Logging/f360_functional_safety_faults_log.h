#ifndef F360_FUNCTIONAL_SAFETY_FAULTS_LOG_H
#define F360_FUNCTIONAL_SAFETY_FAULTS_LOG_H
/*===================================================================================*\
* FILE:  f360_functional_safety_faults_log.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
\*===================================================================================*/

#include "f360_reuse.h"
#include "T360_Types.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
static const int F360_FUNCTIONAL_SAFETY_FAULTS_LOG_STREAM_NUM = 22;
static const int F360_FUNCTIONAL_SAFETY_FAULTS_LOG_STREAM_VERSION = 12;

#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

typedef struct Core_Info_Faults_Log_Tag
{
   uint8_t time_us_no_increase;
   uint8_t cnt_loops_no_increase;
   uint8_t elapsed_time_below_lower_limit;
   uint8_t elapsed_time_above_upper_limit;
} Core_Info_Faults_Log_T;

typedef struct Host_Info_Faults_Log_Tag
{
   uint8_t vehicle_index_no_increase;
   uint8_t host_speed_invalid;
   uint8_t host_yawrate_invalid;
   uint8_t host_longitudinal_acceleration_invalid;
   uint8_t host_lateral_acceleration_invalid;
   uint8_t UNUSED[3];
} Host_Info_Faults_Log_T;

typedef struct Raw_Detection_Faults_T_Log_Tag
{
   uint8_t range_is_invalid;
   uint8_t range_rate_is_invalid;
   uint8_t azimuth_is_invalid;
   uint8_t elevation_is_invalid;
   uint8_t unused[4];
} Raw_Detection_Faults_T_Log_T;

typedef struct Radar_Sensor_Faults_Log_Tag
{
   uint8_t look_index_no_increase;
   uint8_t sensor_vs_tracker_timestamp_divergence;
   uint8_t UNUSED[2];
} Radar_Sensor_Faults_Log_T;

typedef struct Radar_Sensor_Calib_Faults_Log_Tag
{
   uint8_t mounting_pos_is_invalid;
   uint8_t polarity_is_invalid;
   uint8_t boresight_angle_is_invalid;
   uint8_t UNUSED;
} Radar_Sensor_Calib_Faults_Log_T;

typedef struct Output_Faults_Log_Tag
{
    uint8_t f_track_positions_faulty;     // flag indicating that at least one track VCS position is incorrect
    uint8_t f_track_velocities_faulty;    // flag indicating that at least one track OTG-velocity is incorrect
    uint8_t f_track_accelerations_faulty; // flag indicating that at least one track tangential acceleration is incorrect
    uint8_t f_severe_angle_jump_presence_fault; // flag indicating that there are severe angle jump phenomena in output
} Output_Faults_Log_T;

typedef struct Input_Faults_Log_Tag
{
   Radar_Sensor_Calib_Faults_Log_T sensors_calibs[NUM_TOTAL_RADAR_SENSORS];
   Radar_Sensor_Faults_Log_T sensors[NUM_TOTAL_RADAR_SENSORS];
   Core_Info_Faults_Log_T core_info;
   Host_Info_Faults_Log_T host_info;
   Raw_Detection_Faults_T_Log_T raw_detection;
} Input_Faults_Log_T;

typedef struct SCL_Output_Log_Tag
{
   // Hamming coded signals - 60: FAULT_PRESENT_STATUS, 105: FAULT_PARTIAL_PRESENT_STATUS, 195: FAULT_NOT_PRESENT_STATUS
   uint8_t sensors_calibs_fault_status[NUM_TOTAL_RADAR_SENSORS];
   uint8_t sensors_fault_status[NUM_TOTAL_RADAR_SENSORS];
   uint8_t core_info_fault_status;
   uint8_t host_info_fault_status;
   uint8_t raw_detection_fault_status;
   uint8_t object_track_fault_status;
   uint8_t angle_jump_fault_status;
   uint8_t overall_fault_status;
   uint8_t should_reset;
   uint8_t UNUSED;
} SCL_Output_Log_T;

typedef struct Functional_Safety_Faults_Log_Tag
{
   Input_Faults_Log_T input_faults;
   Output_Faults_Log_T output_faults;
   SCL_Output_Log_T scl_output_faults;
} Functional_Safety_Faults_Log_T;

LOGSIZE_ASSERT(Functional_Safety_Faults_Log_T, 152U);

#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif

#endif
