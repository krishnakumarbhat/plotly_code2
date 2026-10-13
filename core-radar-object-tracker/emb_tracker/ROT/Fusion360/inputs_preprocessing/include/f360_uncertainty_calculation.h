#ifndef F360_UNCERTAINTY_CALCULATION_H
   #define F360_UNCERTAINTY_CALCULATION_H
   /*===========================================================================*/
   /**
    * @file f360_uncertainty_calculation.h
    *
    * @brief Detection Uncertainty Calculation
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Confidential - Restricted Aptiv information. Do not disclose
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
    *       wiki/52-SoftwareArchitecture/CMP_SAD_Interface_Library_TrackerCore
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
    * @defgroup f360_uncertainty Uncertainty
    * @{
    */
   /*===========================================================================*/

   /*===========================================================================*
    * Standard Header Files
    *===========================================================================*/

   /*===========================================================================*
    * Other Header Files
    *===========================================================================*/
   #include "f360_detection.h"
   #include "f360_detection_props.h"
   #include "f360_host.h"
   #include "f360_radar_sensor.h"
   #include "f360_radar_sensor_props.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
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
 * Name:  Calculate_Detection_Uncertainty
 *   This function calculates uncertainty for a single detection. Calculates
 *   and stores the standard deviation of compensated range rate in
 *   std_range_rate_compensated_scm. Based on the uncertainty propagation approach
 *   from f360_sensor_capability.
 *
 * Shared Variables: None
 *
 * Parameters:
 *    raw_detection                  - Reference to raw detection data (input)
 *    detection_props                - Reference to processed detection properties (input)
 *    host                           - Reference to host vehicle data (input)
 *    sensor_data                    - Reference to sensor data (input)
 *    sensor_props_data              - Reference to sensor refined properties data (input)
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
void Calculate_Detection_Uncertainty(const F360_Detection_T &raw_detection, const F360_Detection_Props_T &detection_props,
                                     const F360_Host_T &host, const F360_Radar_Sensor_T &sensor_data,
                                     const F360_Radar_Sensor_Props_T &sensor_props_data, const float32_t range_rate_std,
                                     float32_t &std_range_rate_compensated_scm);

/******************************************************************************
 * Name:  Reset_Uncertainty_Cache
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
void Reset_Uncertainty_Cache(void);

} // namespace f360_variant_A

/** @} doxygen end group */
#endif // F360_UNCERTAINTY_CALCULATION_H

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
