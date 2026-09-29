#ifndef RSPP_SENSOR_MOUNTING_POSITION_H
#define RSPP_SENSOR_MOUNTING_POSITION_H
/*===========================================================================*/
/**
 * @file rspp_sensor_mounting_position.h
 *
 * @brief Sensor Mounting Position Structure
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Sensor mounting position and orientation structure definition.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - VCS: Vehicle Coordinate System
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
 * @defgroup rspp_sensor_mounting_position Sensor Mounting
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_position.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace rspp_variant_A
{

   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   /******************************************************************************
    * Name:  RSPP_Sensor_Mounting_Position_T
    *   Structure for sensor mounting position and orientation in VCS.
    *
    * Struct Members:
    *   vcs_position                  - List including the positioning of the sensor in terms of longitudinal, lateral and height.
    *   vcs_boresight_azimuth_angle   - [rad] Nominal azimuth boresight angle of the sensor in VCS, not compensated for alignment
    *                                   error.
    *   vcs_boresight_elevation_angle - [rad] Nominal elevation boresight angle of the sensor in VCS,  not compensated for alignment
    *                                   error.
    ******************************************************************************/
   typedef struct RSPP_Sensor_Mounting_Position_Tag
   {
      RSPP_VCS_Position_T vcs_position;
      float32_t vcs_boresight_azimuth_angle;
      float32_t vcs_boresight_elevation_angle;
   } RSPP_Sensor_Mounting_Position_T;

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/

} // namespace rspp_variant_A

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
