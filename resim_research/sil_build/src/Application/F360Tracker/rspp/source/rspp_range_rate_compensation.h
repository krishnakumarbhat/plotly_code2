#ifndef RSPP_RANGE_RATE_COMPENSATION_H
#define RSPP_RANGE_RATE_COMPENSATION_H
/*===========================================================================*/
/**
 * @file rspp_range_rate_compensation.h
 *
 * @brief Range Rate Compensation for Ego Motion
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Range rate compensation module for RSPP. Compensates range rate measurements
 * for ego vehicle motion.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *       - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8130
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
 * @defgroup rspp_range_rate Range Rate
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_internal_types.h"
#include "rspp_detection_list.h"
#include "rspp_host.h"
#include "rspp_velocity.h"
#include "rspp_radar_sensor.h"
#include "rspp_look_ID.h"

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
    * Name:  RSPP_Calculate_Compensated_RRate
    *   This function calculates the range rate of the detection compensated for
    *   ego vehicle motion using a two-step process: (1) Initial Compensation -
    *   r_comp = r_meas + r_pred where r_pred = v_long * cos(theta_az) + v_lat *
    *   sin(theta_az), and (2) Interval Wrapping - applies wrapping based on
    *   sensor-specific interval width.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   raw_detection            - Reference to raw detection data (input)
    *   sensor_data              - Reference to sensor variable properties (input)
    *   sensor_calibration_data  - Reference to sensor calibration data (input)
    *   processed_detection      - Reference to processed detection data (output)
    *
    * Return Value:
    *   true - If input detection velocity data is valid
    *   false - if input detection velocity data is invalid
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
    *    - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8130
    *
    * Change References: None
    *
    ******************************************************************************/
   bool RSPP_Calculate_Compensated_RRate(
       const Raw_Detection_T &raw_detection,
       const VariableProps_T &sensor_data,
       const RSPP_Sensor_Calib_T &sensor_calibration_data,
       Processed_Detection_T &processed_detection);

   /******************************************************************************
    * Name:  RSPP_Apply_Range_Rate_Wrapping
    *   This function applies interval wrapping to compensated range rate to keep
    *   range rate within the specified interval: lowerBound = -0.5 * range_rate_interval_width,
    *   upperBound = +0.5 * range_rate_interval_width.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   range_rate_interval_width  - Range rate interval width for wrapping
    *   compensated_range_rate     - Reference to range rate value to wrap
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Apply_Range_Rate_Wrapping(
       const float32_t range_rate_interval_width,
       float32_t &compensated_range_rate);

   /******************************************************************************
    * Name:  RSPP_Calculate_Ego_Motion_Range_Rate
    *   This function calculates the predicted range rate based on ego vehicle motion
    *   using the formula: r_pred = v_long * cos(theta_az) + v_lat * sin(theta_az).
    *
    * Shared Variables: None
    *
    * Parameters:
    *   sensor_vcs_velocity  - Reference to sensor velocity in VCS
    *   vcs_azimuth          - Detection azimuth angle in VCS [rad]
    *
    * Return Value:
    *   float32_t - Predicted range rate [m/s]
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   float32_t RSPP_Calculate_Ego_Motion_Range_Rate(
       const RSPP_VCS_Velocity_T &sensor_vcs_velocity,
       const float32_t vcs_azimuth);

} // namespace rspp_variant_A

/** @} doxygen end group */
#endif // RSPP_RANGE_RATE_COMPENSATION_H

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
