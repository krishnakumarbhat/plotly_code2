/*===================================================================================*\
* FILE: f360_sensor_mounting_position.h
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
*   This file contains F360_Sensor_Mounting_Position structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): TypesFusion360.h
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
#ifndef F360_SENSOR_MOUNTING_POSITION_H
#define F360_SENSOR_MOUNTING_POSITION_H

#include "f360_position.h"

typedef struct F360_Sensor_Mounting_Position_Tag
{
   F360_VCS_Position_T vcs_position;        // List including the positioning of the sensor in terms of longitudinal, lateral and height.
   float32_t vcs_boresight_azimuth_angle;   // [rad] Nominal azimuth boresight angle of the sensor in VCS, not compensated for alignment error.
   float32_t vcs_boresight_elevation_angle; // [rad] Nominal elevation boresight angle of the sensor in VCS, not compensated for alignment error.
} F360_Sensor_Mounting_Position_T;

   static_assert((sizeof(((F360_Sensor_Mounting_Position_T*)0)->vcs_position) + 8) == sizeof(F360_Sensor_Mounting_Position_T), "sizeof(F360_Sensor_Mounting_Position_T) not as expected. Remember to align padding if needed");
#endif
