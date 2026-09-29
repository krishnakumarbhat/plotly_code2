#ifndef RSPP_SENSOR_TYPE_H
#define RSPP_SENSOR_TYPE_H
/*===========================================================================*/
/**
 * @file rspp_sensor_type.h
 *
 * @brief Sensor Type Enumeration
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Sensor type enumeration for radar, lidar, and vision sensors.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - SRR: Short Range Radar
 *   - MRR: Medium Range Radar
 *   - ESR: Extended Short Range Radar
 *   - FLR: Forward Looking Radar
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *
 *   - Requirements Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/51-SoftwareRequirementsSpecifications/CMP_SRS_TrackerCore
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *     - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 *
 * @defgroup rspp_sensor_type Sensor Type
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_reuse.h"

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/

/*===========================================================================*
 * Exported Enum Class Declarations
 *===========================================================================*/
/******************************************************************************
 * Name:  RSPP_Sensor_Type_T
 *   Enum for different sensor types.
 ******************************************************************************/
typedef enum RSPP_Sensor_Type_Tag : int8_t
{
   RSPP_SENSOR_TYPE_UNKNOWN = (-1),
   RSPP_SENSOR_TYPE_SRR2_RADAR = (0),
   RSPP_SENSOR_TYPE_SRR4_RADAR = (1),
   RSPP_SENSOR_TYPE_SRR4_MM_RADAR = (2),
   RSPP_SENSOR_TYPE_SRR5_RADAR = (3),
   RSPP_SENSOR_TYPE_MRR360_RADAR = (4),
   RSPP_SENSOR_TYPE_ESR_RADAR = (5),
   RSPP_SENSOR_TYPE_MRR1_RADAR = (6),
   RSPP_SENSOR_TYPE_MRR2_RADAR = (7),
   RSPP_SENSOR_TYPE_MRR3_RADAR = (8),
   RSPP_SENSOR_TYPE_LIDAR = (9),
   RSPP_SENSOR_TYPE_VISION = (10),
   RSPP_SENSOR_TYPE_VEHICLE = (11),
   RSPP_SENSOR_TYPE_FLR4_RADAR = (12),
   RSPP_SENSOR_TYPE_FLR4_PLUS_RADAR = (13),
   RSPP_SENSOR_TYPE_SRR6_RADAR = (14),
   RSPP_SENSOR_TYPE_SRR6_PLUS_RADAR = (15),
   RSPP_SENSOR_TYPE_SRR7_PLUS_RADAR = (16),
   RSPP_SENSOR_TYPE_FLR7_RADAR = (17),
   RSPP_SENSOR_TYPE_SRR6_PLUS_PLT_RADAR = (18),
   RSPP_SENSOR_TYPE_SRR6_PLUS_PLT_1XCAN_RADAR = (19),
   RSPP_SENSOR_TYPE_SRR6_PLUS_PLT_1XETH_RADAR = (20),
   RSPP_SENSOR_TYPE_SRR6_PLUS_PLT_1GB_EHT_RADAR = (21),
   RSPP_SENSOR_TYPE_SRR7_PLUS_PLT_RADAR = (22),
   RSPP_SENSOR_TYPE_FLR7_PLT_RADAR = (23),
   RSPP_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR = (24),
   RSPP_SENSOR_TYPE_FLR4_PLT_RADAR = (25),
   RSPP_SENSOR_TYPE_FLR4_PLT_STANDALONE_RADAR = (26),
   RSPP_SENSOR_TYPE_SRR7E_PLT_RADAR = (27),
   RSPP_SENSOR_TYPE_SRR7_PLUS_V2_PLT_RADAR = (28),
   RSPP_SENSOR_TYPE_FLR7_V2_PLT_RADAR = (29),
   RSPP_SENSOR_TYPE_SRR8_PLUS_PLT_RADAR = (30),
   RSPP_SENSOR_TYPE_FLR8_PLT_RADAR = (31),
   RSPP_SENSOR_TYPE_FLR8_HD_PLT_RADAR = (32),
   RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR = (33),
   RSPP_SENSOR_TYPE_FLR7_L_PLT_RADAR = (34)
} RSPP_Sensor_Type_T;

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Declarations
 *===========================================================================*/

/** @} doxygen end group */
#endif

/*============================================================================*\
 * AUTHOR(S) IDENTITY (AID)
 *-----------------------------------------------------------------------------
 *
 *  AID         NAME
 *  ---------------------------------------------------------------------------
 *  wzfkqj      Tobias Almroth
\*============================================================================*/

/*============================================================================*\
 * FILE REVISION HISTORY
 *-----------------------------------------------------------------------------
 *
 *  File history can be traced by URL:
 *  "https://gitgerrit.asux.aptiv.com/q/project:CORECOMP%252FALSW%252FOT_ObjectTracking"
\*============================================================================*/

/* END OF FILE -------------------------------------------------------------- */
