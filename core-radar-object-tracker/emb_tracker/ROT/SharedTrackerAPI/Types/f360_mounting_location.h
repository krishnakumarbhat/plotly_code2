#ifndef F360_MOUNTING_LOCATION_H
#define F360_MOUNTING_LOCATION_H
/*===================================================================================*\
* FILE: F360_Mounting_Location.h
*====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains enum F360_Mounting_Location declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s): MOUNT_LOC.m
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

#include "f360_reuse.h"

/******************************************************************************
 * Name:  F360_Mounting_Location_T
 *   Enumeration for detection mounting location.
 ******************************************************************************/
#ifdef __cplusplus
typedef enum F360_Mounting_Location_Tag : int8_t
#else
typedef enum F360_Mounting_Location_Tag
#endif
{
   F360_MOUNTING_LOCATION_UNKNOWN         = (-1),
   F360_MOUNTING_LOCATION_LEFT_FORWARD    = (0),
   F360_MOUNTING_LOCATION_LEFT_SIDE1      = (8),
   F360_MOUNTING_LOCATION_LEFT_SIDE2      = (16),
   F360_MOUNTING_LOCATION_LEFT_REAR       = (24),
   F360_MOUNTING_LOCATION_CENTER_FORWARD  = (1),
   F360_MOUNTING_LOCATION_CENTER_REAR     = (25),
   F360_MOUNTING_LOCATION_RIGHT_FORWARD   = (2),
   F360_MOUNTING_LOCATION_RIGHT_SIDE1     = (10),
   F360_MOUNTING_LOCATION_RIGHT_SIDE2     = (18),
   F360_MOUNTING_LOCATION_RIGHT_REAR      = (26),
   F360_MOUNTING_LOCATION_CENTER2_FORWARD = (3),
   F360_MOUNTING_LOCATION_CENTER2_REAR    = (27),
   F360_MOUNTING_LOCATION_CENTER3_FORWARD = (4),
   F360_MOUNTING_LOCATION_CENTER3_REAR    = (28)
} F360_Mounting_Location_T;

#endif
