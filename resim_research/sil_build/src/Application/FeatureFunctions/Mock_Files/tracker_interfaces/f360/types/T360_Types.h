/*===================================================================================*\
* FILE: T360_Types.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file defines important F360 enumerations and macros containing max numbers of sensors
* of different types, max numbers of detections, etc.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef T360_TYPES_H
#define T360_TYPES_H

#include "reuse.h"

// Core logging constants
#define MAX_F360_CLUSTERS 800
#define MAX_F360_OBJECTS 250
#define MAX_REDUCED_OBJECTS 125
#define NUM_SRR_SENSORS 6
#define NUM_ESR_SENSORS 6
#define NUM_MRR_SENSORS 0
#define NUM_FLR_SENSORS 2 // TODO: DFF-1140 change to max supported radars based on possible mounting locations

#define MAX_SRR_RADAR_DETS_PER_SENSOR 256
#define MAX_ESR_DETS_PER_SENSOR 128
#define MAX_MRR_DETS_PER_SENSOR 400
#define MAX_SRR_RADAR_DETS (NUM_SRR_SENSORS*MAX_SRR_RADAR_DETS_PER_SENSOR)
#define MAX_ESR_RADAR_DETS (NUM_ESR_SENSORS*MAX_ESR_DETS_PER_SENSOR)
#define MAX_MRR_RADAR_DETS (NUM_MRR_SENSORS*MAX_MRR_DETS_PER_SENSOR)
#define MAX_FLR_RADAR_DETS (NUM_FLR_SENSORS*MAX_MRR_DETS_PER_SENSOR)
#define MAX_ALL_RADAR_DETS (MAX_SRR_RADAR_DETS + MAX_FLR_RADAR_DETS)
#define NUM_TOTAL_RADAR_SENSORS (NUM_SRR_SENSORS + NUM_ESR_SENSORS + NUM_MRR_SENSORS)

// Tracker internals constants
#define F360_MAX_HIST_DETS_IN_OBJ_TRK 40
#define F360_NUM_POSN_HIST_SLOTS 20
#define F360_MAX_NUMBER_OF_HISTORIC_DETECTIONS 800

// Longi Stat Curves (LSC) Constants
#define NUM_OF_LONGI_STAT_CURVES 4

// Concrete wall detector constant
#define NUM_OF_CWD 2

// Static Environment Polynomials
#define NUM_OF_STATIC_ENV_POLYS (NUM_OF_LONGI_STAT_CURVES + NUM_OF_CWD)

// Underdrivability
#define NUMBER_OF_OVERHEAD_ZONES 50
#define UNDERDRIVABILITY_HEIGHT_STATE_SIZE 3
#define UNDERDRIVABILITY_RCS_STATE_SIZE 6

// VSE Buffering
#define VSE_OUTPUT_BUFFER_LENGTH 5

// Type Definitions

/* F360_QUALITY_FACTOR - Clarifications:
 * The scope/range is defined by the system using the signal. Could be different for different systems using the signal
 */
enum F360_QUALITY_FACTOR
{
   F360_QF_UNDEFINED      = (0), // Indicating that the signal is NOT available from outside system/diagnosis. (Signal is NOT available)
   F360_QF_TEMP_UNDEFINED = (1), // Indicating that the signal is BEYOND the scope/range. (Signal is out of scope)
   F360_QF_INACCURATE     = (2), // Indicating that the signal is WITHIN the scope/range, but the change in signal is too large. (Signal change is NOK)
   F360_QF_ACCURATE       = (3)  // Indicating that the signal is WITHIN the scope/range and the change in signal is OK. (Signal is OK)
};

enum F360_PRNDL_STATE
{
   F360_PRNDL_STATE_PARK = (0),
   F360_PRNDL_STATE_REVERSE = (1),
   F360_PRNDL_STATE_NEUTRAL = (2),
   F360_PRNDL_STATE_DRIVE = (3),
   F360_PRNDL_STATE_FOURTH = (4),
   F360_PRNDL_STATE_THIRD = (5),
   F360_PRNDL_STATE_LOW = (6)
};

enum F360_DET_MOTION
{
   F360_DET_MOTION_INVALID = -1,
   F360_DET_MOTION_STATIONARY,
   F360_DET_MOTION_MOVING,
   F360_DET_MOTION_AMBIGUOUS
};

enum F360_LOOK_ID
{
   F360_LOOK_ID_INVALID = (-1),
   F360_LOOK_ID_0 = (0), // LR, LL
   F360_LOOK_ID_1 = (1), // LR, ML
   F360_LOOK_ID_2 = (2), // MR, LL
   F360_LOOK_ID_3 = (3), // MR, ML
   F360_NUM_LOOK_ID = (4)
};

enum F360_OBJ_STATUS
{
   F360_OBJ_STATUS_INVALID,
   F360_OBJ_STATUS_NEW,
   F360_OBJ_STATUS_NEW_UPDATED,
   F360_OBJ_STATUS_NEW_COASTED,
   F360_OBJ_STATUS_UPDATED,
   F360_OBJ_STATUS_COASTED
};

enum F360_SENSOR
{
   F360_SENSOR_UNKNOWN         = (-1),
   F360_SENSOR_SRR2_RADAR      = (0),
   F360_SENSOR_SRR4_RADAR      = (1),
   F360_SENSOR_SRR4_MM_RADAR   = (2),
   F360_SENSOR_ESR_RADAR       = (3),
   F360_SENSOR_MRR1_RADAR      = (4),
   F360_SENSOR_MRR2_RADAR      = (5),
   F360_SENSOR_MRR3_RADAR      = (6),
   F360_SENSOR_LIDAR           = (7),
   F360_SENSOR_VISION          = (8),
   F360_SENSOR_VEHICLE         = (9),
   F360_SENSOR_SRR5_RADAR      = (10),
   F360_SENSOR_MRR360_RADAR    = (11),
   F360_SENSOR_FLR4_RADAR      = (12),
   F360_SENSOR_FLR4_PLUS_RADAR = (13),
   F360_SENSOR_SRR6_RADAR      = (14),
   F360_SENSOR_SRR6_PLUS_RADAR = (15)
};

// Guardrail Detector (GD) Enum Definitions
enum F360_GUARDRAIL_STATUS
{
   F360_GUARDRAIL_STATUS_INVALID,
   F360_GUARDRAIL_STATUS_NEW,
   F360_GUARDRAIL_STATUS_UPDATED,
   F360_GUARDRAIL_STATUS_COASTED
};

enum F360_TRKFLTR
{
   F360_TRKFLTR_INVALID,
   F360_TRKFLTR_CCV,
   F360_TRKFLTR_CTCA
};

enum F360_CURRENT_MSMT
{
   F360_MSMT_INVALID = 0,
   F360_MSMT_DETS_ONLY = 1
};

enum F360_OBJECT_CLASS
{
   F360_OBJECT_CLASS_UNDETERMINED = (0),
   F360_OBJECT_CLASS_CAR = (1),
   F360_OBJECT_CLASS_MOTORCYCLE = (2),
   F360_OBJECT_CLASS_TRUCK = (3),
   F360_OBJECT_CLASS_PEDESTRIAN = (4),
   F360_OBJECT_CLASS_POLE = (5),
   F360_OBJECT_CLASS_TREE = (6),
   F360_OBJECT_CLASS_ANIMAL = (7),
   F360_OBJECT_CLASS_GOD = (8),
   F360_OBJECT_CLASS_BICYCLE = (9),
   F360_OBJECT_CLASS_UNIDENTIFIED_VEHICLE = (10),
};

enum F360_MOUNT_LOC_LATERAL
{
   F360_MOUNTING_LOC_LAT_UNKNOWN = (-1),
   F360_MOUNTING_LEFT = (0),
   F360_MOUNTING_CENTER = (1),
   F360_MOUNTING_RIGHT = (2),
   F360_MOUNTING_CENTER2 = (3),
   F360_MOUNTING_CENTER3 = (4)
};

enum F360_MOUNT_LOC_LONGITUDINAL
{
   F360_MOUNTING_LOC_LONG_UNKNOWN = (-1),
   F360_MOUNTING_FORWARD = (0),
   F360_MOUNTING_SIDE_1 = (8),
   F360_MOUNTING_SIDE_2 = (16),
   F360_MOUNTING_REAR = (24)
};

enum F360_MOUNT_LOC
{
   F360_MOUNT_LOC_UNKNOWN = F360_MOUNTING_LOC_LONG_UNKNOWN,
   F360_MOUNT_LOC_LEFT_FORWARD = (F360_MOUNTING_LEFT + F360_MOUNTING_FORWARD),
   F360_MOUNT_LOC_LEFT_SIDE1 = (F360_MOUNTING_LEFT + F360_MOUNTING_SIDE_1),
   F360_MOUNT_LOC_LEFT_SIDE2 = (F360_MOUNTING_LEFT + F360_MOUNTING_SIDE_2),
   F360_MOUNT_LOC_LEFT_REAR = (F360_MOUNTING_LEFT + F360_MOUNTING_REAR),
   F360_MOUNT_LOC_CENTER_FORWARD = (F360_MOUNTING_CENTER + F360_MOUNTING_FORWARD),
   F360_MOUNT_LOC_CENTER_REAR = (F360_MOUNTING_CENTER + F360_MOUNTING_REAR),
   F360_MOUNT_LOC_RIGHT_FORWARD = (F360_MOUNTING_RIGHT + F360_MOUNTING_FORWARD),
   F360_MOUNT_LOC_RIGHT_SIDE1 = (F360_MOUNTING_RIGHT + F360_MOUNTING_SIDE_1),
   F360_MOUNT_LOC_RIGHT_SIDE2 = (F360_MOUNTING_RIGHT + F360_MOUNTING_SIDE_2),
   F360_MOUNT_LOC_RIGHT_REAR = (F360_MOUNTING_RIGHT + F360_MOUNTING_REAR),
   F360_MOUNT_LOC_CENTER2_FORWARD = (F360_MOUNTING_CENTER2 + F360_MOUNTING_FORWARD),
   F360_MOUNT_LOC_CENTER2_REAR = (F360_MOUNTING_CENTER2 + F360_MOUNTING_REAR),
   F360_MOUNT_LOC_CENTER3_FORWARD = (F360_MOUNTING_CENTER3 + F360_MOUNTING_FORWARD),
   F360_MOUNT_LOC_CENTER3_REAR = (F360_MOUNTING_CENTER3 + F360_MOUNTING_REAR)
};

#endif
