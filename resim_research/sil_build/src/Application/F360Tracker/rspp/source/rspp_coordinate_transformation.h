#ifndef RSPP_COORDINATE_TRANSFORMATION_H
#define RSPP_COORDINATE_TRANSFORMATION_H
/*===========================================================================*/
/**
 * @file rspp_coordinate_transformation.h
 *
 * @brief Coordinate Transformation Functions
 *
 *------------------------------------------------------------------------------
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *------------------------------------------------------------------------------
 * @section DESC DESCRIPTION:
 * Coordinate transformation functions for RSPP module.
 * Provides functions to convert detection coordinates from sensor polar
 * coordinates to Vehicle Coordinate System (VCS) Cartesian coordinates,
 * including compensation for sensor misalignment.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - VCS: Vehicle Coordinate System
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *       - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8069
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
 * @defgroup rspp_coordinate_transformation Coordinate Transformation
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
#include "rspp_radar_sensor.h"
#include "rspp_velocity.h"

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
    * Name:  RSPP_Calculate_VCS_Position
    *   This function calculates the VCS coordinates for a detection using the
    *   formulas: x_vcs = x_sensor + r * cos(theta_vcs_az),
    *   y_vcs = y_sensor + r * sin(theta_vcs_az),
    *   z_vcs = -h_sensor + r * sin(theta_vcs_el).
    *
    * Shared Variables: None
    *
    * Parameters:
    *   raw_detection         - Reference to input raw detection in polar sensor coordinates
    *   vcs_mounting_position - Reference to mounting position of the sensor in VCS
    *   processed_detection   - Reference to output processed detection attributes in VCS
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Calculate_VCS_Position(
       const Raw_Detection_T &raw_detection,
       const RSPP_VCS_Position_T &vcs_mounting_position,
       Processed_Detection_T &processed_detection);

   /******************************************************************************
    * Name:  RSPP_Calculate_VCS_Angles
    *   This function calculates the VCS azimuth and elevation angles for a detection.
    *
    *
    * Shared Variables: None
    *
    * Parameters:
    *   polarity             - Polarity of the sensor (1 = normal, -1 = flipped)
    *   sensor_data          - Reference to sensor data containing mounting orientation information
    *   raw_detection        - Reference to raw detection data
    *   processed_detection  - Reference to processed detection to update
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Calculate_VCS_Angles(
       const int8_t polarity,
       const VariableProps_T &sensor_data,
       const Raw_Detection_T &raw_detection,
       Processed_Detection_T &processed_detection);

   /******************************************************************************
    * Name:  RSPP_Calculate_Detection_VCS_Coordinates
    *   This function performs complete coordinate transformation from sensor to
    *   VCS coordinates for a detection, including angle compensation.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   raw_detection             - Reference to input raw detection in polar sensor coordinates
    *   sensor_data               - Reference to dynamic sensor data
    *   sensor_calibration_data   - Reference to sensor calibration data
    *   processed_detection       - Reference to processed detection attributes to update
    *
    * Return Value:
    *   true  - if input detection position data is valid
    *   false - if input detection position data is invalid
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
    *    - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8069
    *
    * Change References: None
    *
    ******************************************************************************/
   bool RSPP_Calculate_Detection_VCS_Coordinates(
       const Raw_Detection_T &raw_detection,
       const VariableProps_T &sensor_data,
       const RSPP_Sensor_Calib_T &sensor_calibration_data,
       Processed_Detection_T &processed_detection);

} // namespace rspp_variant_A

/** @} doxygen end group */
#endif // RSPP_COORDINATE_TRANSFORMATION_H

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
