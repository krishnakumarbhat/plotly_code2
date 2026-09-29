#ifndef RSPP_INTERNAL_TYPES_H
#define RSPP_INTERNAL_TYPES_H
/*===========================================================================*/
/**
 * @file rspp_internal_types.h
 *
 * @brief RSPP Internal Type Definitions
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Common type definitions for internal RSPP module including return codes and state enums.
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
 * @defgroup rspp_types Common Types
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
#include "rspp_types.h"
#include "rspp_sensor_mounting_position.h"
#include "rspp_look_ID.h"
#include "rspp_sensor_type.h"

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

   /******************************************************************************
    * Name:  RSPP_States_Type_T
    *   Enum for RSPP module states.
    *
    * Enum Values:
    *   RSPP_STATE_UNINITIALIZED  - RSPP not initialized
    *   RSPP_STATE_INITIALIZED    - RSPP initialized and ready
    *   RSPP_STATE_INIT_COMPLETE  - RSPP initialized and calibrated
    ******************************************************************************/
   typedef enum RSPP_States_Type_Tag : uint8_t
   {
      RSPP_STATE_UNINITIALIZED = 0x00U,
      RSPP_STATE_INITIALIZED = 0x01U,
      RSPP_STATE_INIT_COMPLETE = 0x02U
   } RSPP_States_Type_T;

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   /******************************************************************************
    * Name:  RSPP_Sensor_Calib_T
    *   Structure for sensor-specific calibration data.
    *
    * Struct Members:
    *   fov_min_az_rad        - [rad] Minimum azimuth field of view per look ID
    *   fov_max_az_rad        - [rad] Maximum azimuth field of view per look ID
    *   interior_fov          - [rad] Valid boundary of radar field of view per look ID
    *   left_fov_normal       - Sensor left edge FOV normal vector towards inside per look ID
    *   right_fov_normal      - Sensor right edge FOV normal vector towards inside per look ID
    *   v_wrapping            - [m/s] Range rate dealiasing interval per look ID
    *   vcs_mounting_position - Sensor mounting position in VCS coordinates
    *   sensor_type           - Type of sensor
    *   polarity              - 1 = normal, -1 = flipped
    ******************************************************************************/
   typedef struct RSPP_Sensor_Calib_Tag
   {
      float32_t fov_min_az_rad[RSPP_DET_NUM_LOOK_ID];
      float32_t fov_max_az_rad[RSPP_DET_NUM_LOOK_ID];
      float32_t interior_fov[RSPP_DET_NUM_LOOK_ID];
      float32_t left_fov_normal[RSPP_DET_NUM_LOOK_ID];
      float32_t right_fov_normal[RSPP_DET_NUM_LOOK_ID];
      float32_t v_wrapping[RSPP_DET_NUM_LOOK_ID];
      RSPP_VCS_Position_T vcs_mounting_position;
      RSPP_Sensor_Type_T sensor_type;
      int8_t polarity;
   } RSPP_Sensor_Calib_T;

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/

} // namespace rspp_variant_A

/** @} doxygen end group */
#endif // RSPP_TYPES_H

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
