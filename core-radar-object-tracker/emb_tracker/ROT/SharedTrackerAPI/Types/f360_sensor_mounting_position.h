#ifndef F360_SENSOR_MOUNTING_POSITION_H
#define F360_SENSOR_MOUNTING_POSITION_H
/*===================================================================================*\
 * FILE: f360_sensor_mounting_position.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
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
 *  Applicable Standards (in order of precedence: highest first):
 *  - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *  - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#include "f360_position.h"

/******************************************************************************
 * Name:  F360_Sensor_Mounting_Position_T
 *   Structure for sensor mounting position and orientation in VCS.
 *
 * Struct Members:
 *   vcs_position                  - List including the positioning of the sensor in terms of longitudinal, lateral and height.
 *   vcs_boresight_azimuth_angle   - [rad] Nominal azimuth boresight angle of the sensor in VCS, not compensated for alignment
 *                                   error.
 *   vcs_boresight_elevation_angle - [rad] Nominal elevation boresight angle of the sensor in VCS, not compensated for alignment
 *                                   error.
 ******************************************************************************/
typedef struct F360_Sensor_Mounting_Position_Tag
{
   F360_VCS_Position_T vcs_position;
   float32_t vcs_boresight_azimuth_angle;
   float32_t vcs_boresight_elevation_angle;
} F360_Sensor_Mounting_Position_T;

static_assert((sizeof(((F360_Sensor_Mounting_Position_T *)0)->vcs_position) + 8) == sizeof(F360_Sensor_Mounting_Position_T),
              "sizeof(F360_Sensor_Mounting_Position_T) not as expected. Remember to align padding if needed");

#endif
