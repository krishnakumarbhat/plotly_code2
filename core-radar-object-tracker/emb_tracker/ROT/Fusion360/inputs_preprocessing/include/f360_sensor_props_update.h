#ifndef SENSOR_PROPS_UPDATE_H
   #define SENSOR_PROPS_UPDATE_H
   /*===========================================================================*/
   /**
    * @file f360_sensor_props_update.h
    *
    * @brief F360 sensor properties update declarations
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Confidential - Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    * Internal function declaration for F360 sensor properties update function which calculates refined
    * properties including FOV and timing.
    *
    * @section ABBR ABBREVIATIONS:
    *   - FOV: Field of View
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
    * @defgroup f360_sensor_props_update Sensor_Props_Update
    * @{
    */
   /*===========================================================================*/

   /*===========================================================================*
    * Standard Header Files
    *===========================================================================*/

   /*===========================================================================*
    * Other Header Files
    *===========================================================================*/
   #include "f360_constants.h"
   #include "f360_radar_sensor.h"
   #include "f360_radar_sensor_props.h"
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
 * Name:  Update_Sensor_FOV
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
void Update_Sensor_FOV(float32_t (&interior_fov)[F360_DET_NUM_LOOK_ID], float32_t (&left_fov_normal)[F360_DET_NUM_LOOK_ID],
                       float32_t (&right_fov_normal)[F360_DET_NUM_LOOK_ID],
                       const float32_t (&fov_min_az_rad)[F360_DET_NUM_LOOK_ID],
                       const float32_t (&fov_max_az_rad)[F360_DET_NUM_LOOK_ID], const float32_t vcs_boresight_azimuth_angle);

/******************************************************************************
 * Name:  Update_Sensor_Time
 *   This function calculates the time elapsed since the last sensor measurement.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   tracker_time_us                 - Current tracker time in microseconds
 *   sensor_timestamp_us             - Sensor timestamp in microseconds
 *   sensor_time_since_measurement_s - Time since last measurement in seconds (output)
 *
 * Return Value: None
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
void Update_Sensor_Time(const uint64_t tracker_time_us, const uint64_t sensor_timestamp_us,
                        float32_t &sensor_time_since_measurement_s);

/******************************************************************************
 * Name:  Sensor_Props_Update
 *   This function updates the properties of all sensors, including FOV and timing.
 *
 * Shared Variables: None
 *
 * Parameters:
 *   tracker_time_us - Current tracker time in microseconds
 *   sensors         - Array of radar sensors to update (input)
 *   sensors_props   - Array of radar sensor properties to update (output)
 *
 * Return Value: None
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
void Sensor_Props_Update(const uint64_t tracker_time_us, const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
                         F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS]);

} // namespace f360_variant_A

/** @} doxygen end group */
#endif // F360_SENSOR_PROPS_UPDATE_H

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
