#ifndef F360_MOTION_STATUS_CLASSIFICATION_H
   #define F360_MOTION_STATUS_CLASSIFICATION_H
   /*===========================================================================*/
   /**
    * @file f360_motion_status_classification.h
    *
    * @brief Detection Motion Status Classification
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Confidential - Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    * Motion status classification module for RSPP. Classifies detection motion
    * status based on compensated range rate.
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
    * @section DFS DEVIATIONS FROM DESIGN:
    *   - [CPR-8083]; Uncertainty calculations computed inside that requires additional inputs; This will be refactored in future.
    *
    * @ updates to areas outside the scope of procedures:
    *   - Refer to module footer comment block.
    *
    * @defgroup f360_motion_status Motion Status
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
 * Name:  Reset_Motion_Status_Cache
 *   This function resets the uncertainty calculation cache used in motion
 *   status classification. Should be called at the beginning of each
 *   processing cycle to ensure fresh calculations.
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
void Reset_Motion_Status_Cache(void);

/******************************************************************************
 * Name:  Calculate_Moving_Threshold
 *   This function calculates the motion threshold for detection motion status
 *   classification incorporating host dynamics and detection characteristics.
 *   Combines base moving threshold with host speed and curvature components,
 *   and applies range-based bypass logic for low azimuth confidence detections.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   vcs_pos_x                - Detection VCS longitudinal position
 *   vcs_pos_y                - Detection VCS lateral position
 *   conf_az                  - Detection azimuth confidence level
 *   host                     - Host vehicle data containing speed and curvature
 *   f_azimuth_error_stat_mov - Azimuth error flag affecting motion classification
 *
 * Return Value:
 *   Calculated motion threshold
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
float32_t Calculate_Moving_Threshold(const Point vcs_pos, const int8_t conf_az, const F360_Host_T &host,
                                     const bool f_azimuth_error_stat_mov);

/******************************************************************************
 * Name:  Check_Moving_Hypothesis
 *   This function tests the moving hypothesis using statistical criteria.
 *   Moving hypothesis passes when BOTH conditions are met: (1) Absolute test
 *   value exceeds threshold (abs_range_rate_comp > moving_th), and (2) Sigma ratio
 *   exceeds threshold (abs_range_rate_comp/range_rate_comp_std > moving_sigma_th).
 *
 * Shared Variables: None
 *
 * Parameters:
 *   abs_range_rate_comp  - Absolute value of compensated range rate
 *   range_rate_comp_std  - Compensated range rate standard deviation
 *   range_rate_comp_th   - Minimum threshold for compensated range rate
 *   moving_sigma_th      - Sigma threshold for compensated range rate / standard deviation ratio
 *
 * Return Value:
 *   true - If moving hypothesis passes both criteria
 *   false - Otherwise
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
bool Check_Moving_Hypothesis(const float32_t abs_range_rate_comp, const float32_t range_rate_comp_std,
                             const float32_t range_rate_comp_th, const float32_t moving_sigma_th);

/******************************************************************************
 * Name:  Calculate_Motion_Status
 *   This function classifies the motion status of a single detection using
 *   uncertainty. Determines motion status based on compensated range rate and
 *   uncertainty using statistical test (sigma threshold). Classification:
 *   STATIONARY - statistical test indicates stationary hypothesis,
 *   MOVING - |compensated_range_rate| >= moving_threshold,
 *   AMBIGUOUS - failed stationary test but below moving threshold.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   raw_detection           - Reference to raw detection data (input)
 *   sensor_data             - Reference to sensor data (input)
 *   sensor_props_data       - Reference to sensor refined properties data (input)
 *   host_veh                - Reference to host vehicle data (input)
 *   detection_props         - Reference to detection properties data (output)
 *
 * Return Value: None
 *
 * Design Information:
 *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *    - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8132
 *
 * Change References: None
 *
 ******************************************************************************/
void Calculate_Motion_Status(const F360_Detection_T &raw_detection, const F360_Radar_Sensor_T &sensor_data,
                             const F360_Radar_Sensor_Props_T &sensor_props_data, const F360_Host_T &host_veh,
                             F360_Detection_Props_T &detection_props);

} // namespace f360_variant_A

/** @} doxygen end group */
#endif // F360_MOTION_STATUS_CLASSIFICATION_H

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
