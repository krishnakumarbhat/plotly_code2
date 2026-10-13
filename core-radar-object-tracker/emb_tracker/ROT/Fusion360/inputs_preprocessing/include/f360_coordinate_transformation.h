#ifndef F360_COORDINATE_TRANSFORMATION_H
   #define F360_COORDINATE_TRANSFORMATION_H
   /*===========================================================================*/
   /**
    * @file f360_coordinate_transformation.h
    *
    * @brief Coordinate Transformation Functions
    *
    *------------------------------------------------------------------------------
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Confidential - Restricted Aptiv information. Do not disclose
    *------------------------------------------------------------------------------
    * @section DESC DESCRIPTION:
    * Coordinate transformation functions for F360 module.
    * Provides functions to convert detection coordinates from sensor polar
    * coordinates to Vehicle Coordinate System (VCS) Cartesian coordinates,
    * including compensation for sensor misalignment.
    *
    * @section ABBR ABBREVIATIONS:
    *   - F360: Fusion 360
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
    * @defgroup f360_coordinate_transformation Coordinate Transformation
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
   #include "f360_radar_sensor.h"
   #include "f360_velocity.h"

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
 * Name:  Calculate_VCS_Position
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
 *   detection_props        - Reference to output detection properties in VCS
 *
 * Return Value: None
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
void Calculate_VCS_Position(const F360_Detection_T &raw_detection, const F360_VCS_Position_T &vcs_mounting_position,
                            F360_Detection_Props_T &detection_props);

/******************************************************************************
 * Name:  Calculate_VCS_Angles
 *   This function calculates the VCS azimuth and elevation angles for a detection.
 *
 *
 * Shared Variables: None
 *
 * Parameters:
 *   sensor_data          - Reference to sensor data containing mounting orientation information
 *   raw_detection        - Reference to raw detection data
 *   detection_props      - Reference to detection properties to update
 *
 * Return Value: None
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
void Calculate_VCS_Angles(const F360_Radar_Sensor_T &sensor_data, const F360_Detection_T &raw_detection,
                          F360_Detection_Props_T &detection_props);

/******************************************************************************
 * Name:  Calculate_Detection_VCS_Coordinates
 *   This function performs complete coordinate transformation from sensor to
 *   VCS coordinates for a detection, including angle compensation.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   raw_detection             - Reference to input raw detection in polar sensor coordinates
 *   sensor_data               - Reference to dynamic sensor data
 *   detection_props           - Reference to detection properties to update
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
bool Calculate_Detection_VCS_Coordinates(const F360_Detection_T &raw_detection, const F360_Radar_Sensor_T &sensor_data,
                                         F360_Detection_Props_T &detection_props);

} // namespace f360_variant_A

/** @} doxygen end group */
#endif // F360_COORDINATE_TRANSFORMATION_H

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
