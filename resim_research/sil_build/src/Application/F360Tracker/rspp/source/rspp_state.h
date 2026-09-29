#ifndef RSPP_STATE_H
#define RSPP_STATE_H
/*===========================================================================*/
/**
 * @file rspp_state.h
 *
 * @brief RSPP Internal State Management
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Internal state management for RSPP module. Manages initialization state
 * and sensor calibration storage.
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
 * @defgroup rspp_state State
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
#include "rspp_constants.h"
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
    * Name:  RSPP_State_Reset
    *   This function resets RSPP internal state, clearing sensor calibration
    *   data and setting state to uninitialized.
    *
    * Shared Variables: None
    *
    * Parameters: None
    *
    * Return Value: None
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_State_Reset(void);

   /******************************************************************************
    * Name:  RSPP_State_Initialize
    *   This function initializes RSPP internal state, setting state to
    *   initialized and clearing sensor calibration data.
    *
    * Shared Variables: None
    *
    * Parameters: None
    *
    * Return Value: None
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_State_Initialize(void);

   /******************************************************************************
    * Name:  RSPP_State_Get_Current_State
    *   This function gets the current RSPP state. Used to check if RSPP is
    *   properly initialized.
    *
    * Shared Variables: None
    *
    * Parameters: None
    *
    * Return Value:
    *   RSPP_STATE_UNINITIALIZED - If RSPP is not initialized
    *   RSPP_STATE_INITIALIZED - If RSPP is initialized
    *   RSPP_STATE_INIT_COMPLETE - If RSPP is initialized and sensor calibrations set
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   RSPP_States_Type_T RSPP_State_Get_Current_State(void);

   /******************************************************************************
    * Name:  RSPP_State_Set_Sensor_Calibrations
    *   This function sets the sensor calibrations for all radar sensors used by RSPP.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   sensor_calibration - Reference to array of sensor data structures containing the calibration data
    *
    * Return Value:
    *   RSPP_E_NOT_INITIALIZED - If rspp state is not initialized
    *   RSPP_E_INVALID_SENSOR_ID - If sensor ID is invalid or calibration for sensor is already set
    *   RSPP_E_INVALID_CALIBRATION - If calibration data is invalid
    *   RSPP_E_OK - If calibration set successfully
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
    *
    *  Change References: None
    *
    ******************************************************************************/
   RSPP_Return_Type_T RSPP_State_Set_Sensor_Calibrations(
       const F360_Radar_Sensor_T (&sensor_calibrations)[MAX_NUMBER_OF_SENSORS]);

   /******************************************************************************
    * Name:  RSPP_State_Set_Sensor_Calibration
    *   This function stores reference to sensor calibration data for specified
    *   sensor. Stores pointer reference without copying data. Calibration data
    *   must remain valid throughout RSPP operation.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   sensor_calibration - Reference to static calibration data for the sensor
    *
    * Return Value:
    *   RSPP_E_NOT_INITIALIZED - If rspp state is not initialized
    *   RSPP_E_INVALID_SENSOR_ID - If sensor ID is invalid or calibration for sensor is already set
    *   RSPP_E_INVALID_CALIBRATION - If calibration data is invalid
    *   RSPP_E_OK - If calibration set successfully
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
    *
    * Change References: None
    *
    ******************************************************************************/
   RSPP_Return_Type_T RSPP_State_Set_Sensor_Calibration(
       const ConstantProps_T &sensor_calibration);

   /******************************************************************************
    * Name:  RSPP_State_Get_Sensor_Calibration
    *   This function retrieves sensor calibration data for specified sensor.
    *   Will only be called after validation, so sensor ID is assumed valid.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   sensor_idx - Sensor identifier
    *
    * Return Value:
    *    Reference to calibration data
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   const RSPP_Sensor_Calib_T &RSPP_State_Get_Sensor_Calibration(
       const int32_t sensor_idx);

   /******************************************************************************
    * Name:  RSPP_Update_Sensor_FOV
    *   This function computes field of view (FOV) properties for a sensor
    *   including interior FOV limits and normal vectors for FOV boundaries.
    *   Calculates interior FOV by taking the intersection of sensor FOV with
    *   the interior limit. Computes normal vectors for left and right FOV
    *   boundaries for both LR and MR look IDs.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   interior_fov                  - Interior FOV angles for each look ID (output)
    *   left_fov_normal               - Left FOV boundary normal vectors (output)
    *   right_fov_normal              - Right FOV boundary normal vectors (output)
    *   fov_min_az_rad                - Minimum azimuth FOV angles for each look ID
    *   fov_max_az_rad                - Maximum azimuth FOV angles for each look ID
    *   vcs_boresight_azimuth_angle   - VCS boresight azimuth angle
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Update_Sensor_FOV(
       float32_t (&interior_fov)[RSPP_DET_NUM_LOOK_ID],
       float32_t (&left_fov_normal)[RSPP_DET_NUM_LOOK_ID],
       float32_t (&right_fov_normal)[RSPP_DET_NUM_LOOK_ID],
       const float32_t (&fov_min_az_rad)[RSPP_DET_NUM_LOOK_ID],
       const float32_t (&fov_max_az_rad)[RSPP_DET_NUM_LOOK_ID],
       const float32_t vcs_boresight_azimuth_angle);

} // namespace rspp_variant_A

/** @} doxygen end group */
#endif // RSPP_STATE_H

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
