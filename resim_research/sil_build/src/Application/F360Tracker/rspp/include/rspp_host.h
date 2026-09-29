#ifndef RSPP_HOST_H
#define RSPP_HOST_H
/*===========================================================================*/
/**
 * @file rspp_host.h
 *
 * @brief Host Vehicle Information Structure
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Host vehicle state and dynamics information structures.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - VCS: Vehicle Coordinate System
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
 * @defgroup rspp_host Host Vehicle
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_reuse.h"

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/

/*===========================================================================*
 * Exported Enum Class Declarations
 *===========================================================================*/
/******************************************************************************
 * Name:  RSPP_Host_Type
 *   Enumeration defining the type of host vehicle.
 ******************************************************************************/
typedef enum RSPP_Host_Type_Tag : uint8_t
{
   RSPP_HOST_TYPE_PASSENGER_VEHICLE = 0,
   RSPP_HOST_TYPE_COMMERCIAL_VEHICLE = 1
} RSPP_Host_Type;

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/
/******************************************************************************
 * Name:  RSPP_Host_T
 *   Structure containing host vehicle state and dynamics information.
 *
 * Struct Members:
 *   vehicle_index               - Index that increments every time the vehicle state estimator is run.
 *   speed                       - [m/s] Compensated speed measured on the host rear axle
 *   vcs_speed                   - [m/s] Compensated speed in VCS coordinates.
 *   acceleration                - [m/s^2] Acceleration of host vehicle.
 *   vcs_lat_acceleration        - [m/s^2] Lateral acceleration component in VCS.
 *   vcs_long_acceleration       - [m/s^2] Longitudinal acceleration component in VCS.
 *   yaw_rate_rad                - [rad] Yaw rate in radians, compensated for the bias in yaw rate.
 *   vcs_sideslip                - [rad] Side slip in VCS coordinates.
 *   curvature_rear              - [1/m] Curvature on the host rear axle.
 *   dist_rear_axle_to_vcs_m     - [m] Distance from the front bumper to the rear axle.
 *   rear_cornering_compliance   - [rad*m/s^2] Rear cornering compliance of the vehicle.
 *   speed_correction_factor     - [-] Compensation factor for the raw speed, as computed in the vehicle state estimator.
 *   host_type                   - [-] Describes what type the ego vehicle is.
 *   f_trailer_presence_hardware - 1: trailer presence detected by the host vehicle electronically
 *   speed_qf                    - [F360_QUALITY_FACTOR]: Quality factor for speed measurement.
 *   yaw_rate_qf                 - [F360_QUALITY_FACTOR]: Quality factor for yaw rate measurement.
 *   lat_accel_qf                - [F360_QUALITY_FACTOR]: Quality factor for lateral acceleration measurement.
 *   long_accel_qf               - [F360_QUALITY_FACTOR]: Quality factor for longitudinal acceleration measurement.
 *   enum F360_QUALITY_FACTOR: 0 - UNDEF, 1 - TEMP_UNDEF, 2 - INACCURATE, 3 - ACCURATE
 ******************************************************************************/
typedef struct RSPP_Host_Tag
{
   uint32_t vehicle_index;
   float32_t speed;
   float32_t vcs_speed;
   float32_t acceleration;
   float32_t vcs_lat_acceleration;
   float32_t vcs_long_acceleration;
   float32_t yaw_rate_rad;
   float32_t vcs_sideslip;
   float32_t curvature_rear;
   float32_t dist_rear_axle_to_vcs_m;
   float32_t rear_cornering_compliance;
   float32_t speed_correction_factor;
   RSPP_Host_Type host_type;
   bool f_trailer_presence_hardware;
   uint8_t speed_qf;
   uint8_t yaw_rate_qf;
   uint8_t lat_accel_qf;
   uint8_t long_accel_qf;
} RSPP_Host_T;

/*===========================================================================*
 * Exported Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Declarations
 *===========================================================================*/

static_assert(56 == sizeof(RSPP_Host_T),
              "sizeof(RSPP_Host_T) not as expected. Remember to align padding if needed");

/** @} doxygen end group */
#endif

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
