#ifndef SENSOR_INFO_LOG_T_H
#define SENSOR_INFO_LOG_T_H
/*===========================================================================*\
 * FILE: SensorInfoLog.h
 *===========================================================================
 * Copyright 2016 Delphi Technologies, Inc., All Rights Reserved.
 * Delphi Confidential
 *---------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the F360 Tracker Radar Sensor Information
 *
 * ABBREVIATIONS:
 *   TODO: List of abbreviations used, or reference(s) to external document(s)
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *   Requirements Document(s): PDD-10024333-012_(CADS4_VFP_Ethernet_Communication).doc
 *   (Design & Requirements)
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     SW REF 264.15D "Delphi C Coding Standards" [12-Mar-2006]
 *
 * CHANGE SYNERGY INFO:
 * %full_filespec:   SensorInfoLog.h %
 * %version:         1 %
 * %date_modified:   Jan 6th, 2017 %
 *
 * DEVIATIONS FROM STANDARDS:
 *   None
 *
\*===========================================================================*/

#include "f360_reuse.h"
/*===========================================================================*\
* Other Header Files
\*===========================================================================*/
/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/
#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack(4)
#elif defined(__DCC__)
#pragma pack(4,4)
#elif defined(__TMS320C6X__)
#pragma pack(push,4)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, save_pack, 4)
#else
#endif

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/

typedef struct Sensor_Info_Log_Tag
{
   uint64_t timestamp_us;            //!< [us] detection time-stamp 
   uint16_t look_index;              //!< [0-65535] sensor's look index for the current detections;
   uint8_t sensorID;                 //!< ID of sensor.
   uint8_t f_sens_valid;             //!< [bool] indicates if this sensor is mounted and can provide valid detections to the tracker. 
   uint8_t new_measurement_update;   //!< [bool] indicates if this sensor has new measurements for the current frame. 
   int8_t mount_location;            //!< value corresponds to the enum (F360_MOUNT_LOC) which defines mounting locations. 
   int8_t sensor_type;               //!< value corresponds to the enum (enum F360_SENSOR) which defines the type of sensor
   int8_t look_id;                   //!< [-1-3], (enum F360_LOOK_ID) 
   int8_t radar_polarity;            //!<  [(-1)-1] 0 = INVALID, 1 = normal, -1 = flipped

   float vcs_long_posn;              //!< [m] sensor's longitudinal position in VCS coordinate. 
   float vcs_lat_posn;               //!< [m] sensor's lateral position in VCS coordinate. 
   float vcs_long_vel;               //!< [m/s] sensor's longitudinal velocity in VCS coordinate.
   float vcs_lat_vel;                //!< [m/s] sensor's lateral velocity in VCS coordinate.
   float vcs_height_offset_m;        //!< [m] sensor's vertical position in VCS coordinate. 
   float vcs_boresight_az_angle;     //!< [rad] sensor's azimuth boresight angle in VCS coordinate. 
   float vcs_boresight_elev_angle;   //!< [rad] sensor's elevation boresight angle in VCS coordinate. 

   float range_rate_interval_width;  //!< [m/s] width of the range rate interval for current look type. 
   float useful_fov[2];              //!< [rad] minimum and maximum azimuth angle of horizontal FOV for long range. 
   float align_angle_az_rad; //!< [rad] azimuth correction angle used to correct the azimuth angle (az = az_raw - align_angle_az_rad)
   float align_angle_el_rad; //!< [rad] elevation correction angle used to correct the elevation angle (el = el_raw - align_angle_el_rad)

} Sensor_Info_Log_T;

#if defined(__TASKING__)
// TBD
#elif defined(__DCC__) && defined(__DCC_LLVM__)
#pragma pack()
#elif defined(__DCC__)
#pragma pack(0)
#elif defined(__TMS320C6X__)
#pragma pack(pop)
#elif defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop, save_pack)
#else
#endif

#endif /* SENSOR_INFO_LOG_T_H */
