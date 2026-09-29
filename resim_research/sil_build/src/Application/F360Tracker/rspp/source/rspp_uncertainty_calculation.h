#ifndef RSPP_UNCERTAINTY_CALCULATION_H
#define RSPP_UNCERTAINTY_CALCULATION_H
/*===========================================================================*/
/**
 * @file rspp_uncertainty_calculation.h
 *
 * @brief Detection Uncertainty Calculation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Uncertainty calculation module for RSPP. Calculates detection uncertainties
 * for range rate compensation.
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
 * @defgroup rspp_uncertainty Uncertainty
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_detection.h"
#include "rspp_host.h"
#include "rspp_radar_sensor.h"
#include "rspp_internal_types.h"

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
    * Name:  RSPP_Calculate_Detection_Uncertainty
    *   This function calculates uncertainty for a single detection. Calculates
    *   and stores the standard deviation of compensated range rate in
    *   std_range_rate_compensated_scm. Based on the uncertainty propagation approach
    *   from rspp_sensor_capability.
    *
    * Shared Variables: None
    *
    * Parameters:
    *    raw_detection                  - Reference to raw detection data (input)
    *    processed_detection            - Reference to processed detection data (input)
    *    host                           - Reference to host vehicle data (input)
    *    sensor_data                    - Reference to sensor variable properties (input)
    *    sensor_calibration_data        - Reference to sensor calibration data (input)
    *    range_rate_std                 - Range rate standard deviation parameter from RSPP calibration (input)
    *    std_range_rate_compensated_scm - Reference to output standard deviation of compensated range rate (output)
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Calculate_Detection_Uncertainty(
       const Raw_Detection_T &raw_detection,
       const Processed_Detection_T &processed_detection,
       const RSPP_Host_T &host,
       const VariableProps_T &sensor_data,
       const RSPP_Sensor_Calib_T &sensor_calibration_data,
       const float32_t range_rate_std,
       float32_t &std_range_rate_compensated_scm);

   /******************************************************************************
    * Name:  RSPP_Reset_Uncertainty_Cache
    *   This function resets the sensor uncertainty cache for new processing
    *   cycle. Called at start of each detection processing cycle.
    *
    * Shared Variables: None
    *
    * Parameters: None
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Reset_Uncertainty_Cache(void);

} // namespace rspp_variant_A

/** @} doxygen end group */
#endif // RSPP_UNCERTAINTY_CALCULATION_H

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
