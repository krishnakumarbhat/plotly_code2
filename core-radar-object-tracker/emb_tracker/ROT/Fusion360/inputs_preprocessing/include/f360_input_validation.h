#ifndef F360_INPUT_VALIDATION_H
   #define F360_INPUT_VALIDATION_H
   /*===========================================================================*/
   /**
    * @file f360_input_validation.h
    *
    * @brief Input Data Validation
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Confidential - Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    * Input validation module for RSPP. Validates sensor data and detection data
    * according to interface constraints.
    *
    * @section ABBR ABBREVIATIONS:
    *   - RSPP: Radar Signal Pre-Processing
    *
    * @section TRACE TRACEABILITY INFO:
    *   - Design Document(s):
    *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
    *       - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8150
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
    * @defgroup f360_input_validation Input Validation
    * @{
    */
   /*===========================================================================*/

   /*===========================================================================*
    * Standard Header Files
    *===========================================================================*/

   /*===========================================================================*
    * Other Header Files
    *===========================================================================*/
   #include "f360_detection_list.h"
   #include "f360_radar_sensor.h"
   #include "f360_reuse.h"

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
 * Name:  Input_Sensor_Data_Check
 *   This function validates input sensor data array with respect to signal
 *   constraints according to interface definition.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   sensor_data - Reference to input sensor data structure
 *
 * Return Value:
 *   true  - if all data is valid
 *   false - if invalid data detected
 *
 * Design Information:
 *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *    - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8150
 *
 * Change References: None
 *
 ******************************************************************************/
bool Input_Sensor_Data_Check(const F360_Radar_Sensor_T &sensor_data);

/******************************************************************************
 * Name:  Check_Input_Detection_Position_Data
 *   This function validates the input detection position related data,
 *   verifying raw detection data with respect to signal constraints
 *   according to interface definition.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   raw_detection - Reference to input detection data to check
 *   fov_max_az_rad - Maximum azimuth field of view in radians
 *   fov_min_az_rad - Minimum azimuth field of view in radians
 *
 * Return Value:
 *   true - If all position data is valid
 *   false - if any position data is invalid
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
bool Check_Input_Detection_Position_Data(const F360_Detection_T &raw_detection, const float32_t fov_max_az_rad,
                                         const float32_t fov_min_az_rad);

/******************************************************************************
 * Name:  Check_Input_Detection_Velocity_Data
 *   This function validates the input detection velocity related data,
 *   verifying raw detection velocity data with respect to signal constraints
 *   according to interface definition.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   raw_detection - Reference to input detection data to check
 *
 * Return Value:
 *   true - If all velocity data is valid
 *   false - if any velocity data is invalid
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
bool Check_Input_Detection_Velocity_Data(const F360_Detection_T &raw_detection);

/******************************************************************************
 * Name:  Check_Detection_Meta_Data
 *   This function checks that the detection meta data is consistent,
 *   validating fields that are used for accessing arrays and memory locations
 *   to prevent out-of-bounds access and potential crashes.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   raw_detection - Reference to input detection data to check
 *
 * Return Value:
 *   true  - if all meta data is valid
 *   false - if any meta data is invalid
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
bool Check_Detection_Meta_Data(const F360_Detection_T &raw_detection);

/******************************************************************************
 * Name:  Check_Sensor_Calibration
 *   This function validates sensor calibration data with respect to physical
 *   constraints and interface definition requirements.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   constant_props - Reference to input sensor constant properties to check
 *
 * Return Value:
 *   true  - if all calibration data is valid
 *   false - if any calibration data is invalid
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
bool Check_Sensor_Calibration(const F360_Radar_Sensor_T &constant_props);

} // namespace f360_variant_A

/** @} doxygen end group */
#endif // F360_INPUT_VALIDATION_H

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
