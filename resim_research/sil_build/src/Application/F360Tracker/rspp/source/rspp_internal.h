#ifndef RSPP_INTERNAL_H
#define RSPP_INTERNAL_H
/*===========================================================================*/
/**
 * @file rspp_internal.h
 *
 * @brief RSPP Internal Interface
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   Internal API interface for RSPP state management and helper functions.
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
 * @defgroup rspp_internal Internal Interface
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_state.h"
#include "rspp_radar_sensor.h"

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

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/

   /******************************************************************************
    * Name:  RSPP_Calculate_Refined_Sensor_Data
    *   This function calculates refined properties including FOV and timing,
    *   updating the sensor refined properties based on calibration data. This
    *   provides the missing functionality from the old RSPP Update_Sensors()
    *   function.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   current_time_us         - Current tracker time in microseconds
    *   sensor_calibration_data - Reference to sensor calibration data (input)
    *   sensor_data             - Reference to sensor variable properties (input)
    *   refined_sensor_data     - Reference to sensor refined properties (output)
    *
    * Return Value: None
    *
    * Design Information: None
    *
    *  Change References: None
    *
    ******************************************************************************/
   void RSPP_Calculate_Refined_Sensor_Data(
       const uint64_t current_time_us,
       const RSPP_Sensor_Calib_T &sensor_calibration_data,
       const VariableProps_T &sensor_data,
       RefinedProps_T &refined_sensor_data);

} // namespace rspp_variant_A

/** @} doxygen end group */
#endif // RSPP_H

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
