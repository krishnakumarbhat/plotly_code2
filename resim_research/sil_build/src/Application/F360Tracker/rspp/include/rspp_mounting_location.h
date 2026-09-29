#ifndef RSPP_MOUNTING_LOCATION_H
#define RSPP_MOUNTING_LOCATION_H
/*===========================================================================*/
/**
 * @file rspp_mounting_location.h
 *
 * @brief Sensor Mounting Location Enumeration
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Mounting location enumeration for radar sensor placement.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
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
 * @defgroup rspp_mounting_location Mounting Location
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
 * Name:  RSPP_Mounting_Location_T
 *   Enumeration for detection mounting location.
 ******************************************************************************/
typedef enum RSPP_Mounting_Location_Tag : int8_t
{
   RSPP_MOUNTING_LOCATION_UNKNOWN = (-1),
   RSPP_MOUNTING_LOCATION_LEFT_FORWARD = (0),
   RSPP_MOUNTING_LOCATION_LEFT_SIDE1 = (8),
   RSPP_MOUNTING_LOCATION_LEFT_SIDE2 = (16),
   RSPP_MOUNTING_LOCATION_LEFT_REAR = (24),
   RSPP_MOUNTING_LOCATION_CENTER_FORWARD = (1),
   RSPP_MOUNTING_LOCATION_CENTER_REAR = (25),
   RSPP_MOUNTING_LOCATION_RIGHT_FORWARD = (2),
   RSPP_MOUNTING_LOCATION_RIGHT_SIDE1 = (10),
   RSPP_MOUNTING_LOCATION_RIGHT_SIDE2 = (18),
   RSPP_MOUNTING_LOCATION_RIGHT_REAR = (26),
   RSPP_MOUNTING_LOCATION_CENTER2_FORWARD = (3),
   RSPP_MOUNTING_LOCATION_CENTER2_REAR = (27),
   RSPP_MOUNTING_LOCATION_CENTER3_FORWARD = (4),
   RSPP_MOUNTING_LOCATION_CENTER3_REAR = (28)
} RSPP_Mounting_Location_T;

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
