#ifndef RSPP_H
#define RSPP_H
/*===========================================================================*/
/**
 * @file rspp.h
 *
 * @brief RSPP Main Interface
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *   Main API interface for RSPP initialization, calibration, and processing.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
 *       wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
 *       - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8059
 *       - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8062
 *       - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8065
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
 *   - [CPR-8065]; The RSPP_Process_Detections interface temporarily includes RSPP_Core_Info_T to support
 *     time_since_measurement signal updates; This parameter is scheduled for removal in a future release.
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 *
 * @defgroup rspp Main Interface
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_types.h"
#include "rspp_detection_list.h"
#include "rspp_radar_sensor.h"
#include "rspp_host.h"
#include "rspp_core_info.h"

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
    * Name: RSPP_Initialize
    *   This function initializes internal data of RSPP and updates internal
    *   state. Must be called before any other RSPP functions.
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
    *    - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8059
    *
    *  Change References: None
    *
    ******************************************************************************/
   void RSPP_Initialize(void);

   /******************************************************************************
    * Name: RSPP_Set_Sensor_Calibrations
    *   This function sets the sensor calibrations for all radar sensors used by RSPP.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   sensor_calibrations - Reference to array of sensor data structures containing the calibration data
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
    *    - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8062
    *
    *  Change References: None
    *
    ******************************************************************************/
   RSPP_Return_Type_T RSPP_Set_Sensor_Calibrations(
       const F360_Radar_Sensor_T (&sensor_calibrations)[MAX_NUMBER_OF_SENSORS]);

   /******************************************************************************
    * Name: RSPP_Process_Detections
    *   This function processes radar detections for object tracking. Main
    *   processing function that applies coordinate transformation, range rate
    *   compensation, and motion status classification. Each detection contains
    *   sensor_id field used to index into sensor data array (sensor_id - 1 =
    *   array index).
    *
    * Shared Variables: None
    *
    * Parameters:
    *   dynamic_sensor_data - Reference to array of sensor data structures
    *   detection_list      - Reference to detection list (input/output)
    *   vehicle_state_data  - Reference to host vehicle data for uncertainty calculation
    *   core_info           - Reference to core timing and execution information
    *
    * Return Value:
    *   RSPP_E_NOT_INITIALIZED - If component was not properly initialized
    *   RSPP_E_OK - Otherwise
    *
    * Design Information:
    *  - https://polarion.asux.aptiv.com/polarion/#/project/CORE_PERCEPTION_RadarAlgoSW/
    *    wiki/53-SoftwareDetailedDesigns/AAU_SDD_RSPP
    *    - @wi.implements CORE_PERCEPTION_RadarAlgoSW/CPR-8065
    *
    *  Change References: None
    *
    ******************************************************************************/
   RSPP_Return_Type_T RSPP_Process_Detections(
       F360_Radar_Sensor_T (&dynamic_sensor_data)[MAX_NUMBER_OF_SENSORS],
       RSPP_Detection_List_T &detection_list,
       const RSPP_Host_T &vehicle_state_data,
       const RSPP_Core_Info_T &core_info);

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
