/*===================================================================================*\
* FILE: f360_sensor_type.h
*====================================================================================
* Copyright 2017 Delphi Technologies), Inc.), All Rights Reserved.
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
*   This file contains enum F360_Sensor_Type declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): SENSOR.m
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
#ifndef F360_SENSOR_TYPE_H
#define F360_SENSOR_TYPE_H

#include "f360_reuse.h"

typedef enum F360_Sensor_Type_Tag : int8_t
{
   F360_SENSOR_TYPE_UNKNOWN         = (-1),
   F360_SENSOR_TYPE_SRR2_RADAR      = (0),
   F360_SENSOR_TYPE_SRR4_RADAR      = (1),
   F360_SENSOR_TYPE_SRR4_MM_RADAR   = (2),
   F360_SENSOR_TYPE_SRR5_RADAR      = (3),
   F360_SENSOR_TYPE_MRR360_RADAR    = (4),
   F360_SENSOR_TYPE_ESR_RADAR       = (5),
   F360_SENSOR_TYPE_MRR1_RADAR      = (6),
   F360_SENSOR_TYPE_MRR2_RADAR      = (7),
   F360_SENSOR_TYPE_MRR3_RADAR      = (8),
   F360_SENSOR_TYPE_LIDAR           = (9),
   F360_SENSOR_TYPE_VISION          = (10),
   F360_SENSOR_TYPE_VEHICLE         = (11),
   F360_SENSOR_TYPE_FLR4_RADAR      = (12),
   F360_SENSOR_TYPE_FLR4_PLUS_RADAR = (13),
   F360_SENSOR_TYPE_SRR6_RADAR      = (14),
   F360_SENSOR_TYPE_SRR6_PLUS_RADAR = (15),
   F360_SENSOR_TYPE_SRR7_PLUS_RADAR = (16),
   F360_SENSOR_TYPE_FLR7_RADAR      = (17),
   F360_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR = (18),
   F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR = (19),
   F360_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR = (20),
   F360_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR = (21),
   F360_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR = (22),
   F360_SENSOR_TYPE_FLR7_PLT_RADAR = (23),
   F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR = (24),
   F360_SENSOR_TYPE_FLR4_PLT_RADAR = (25),
   F360_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR = (26),
   F360_SENSOR_TYPE_SRR7E_PLT_RADAR = (27),
   F360_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR = (28),
   F360_SENSOR_TYPE_FLR7_V2_PLT_RADAR = (29),
   F360_SENSOR_TYPE_SRR8_PLUS_PLT_RADAR = (30),
   F360_SENSOR_TYPE_FLR8_PLT_RADAR = (31),
   F360_SENSOR_TYPE_FLR8_HD_PLT_RADAR = (32),
   F360_SENSOR_TYPE_SRR7_L_PLT_RADAR = (33),
   F360_SENSOR_TYPE_FLR7_L_PLT_RADAR = (34)
} F360_Sensor_Type_T;

#endif
