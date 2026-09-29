#ifndef RSPP_SENSOR_CAPABILITY_H
#define RSPP_SENSOR_CAPABILITY_H
/*===========================================================================*/
/**
 * @file rspp_sensor_capability.h
 *
 * @brief Sensor Capability and Uncertainty Propagation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Sensor capability and uncertainty propagation functions for RSPP module.
 * Provides uncertainty calculations for host velocity, detection parameters,
 * and compensated range rate measurements.
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
 * @defgroup rspp_sensor_capability Sensor Capability
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
#include "rspp_constants.h"
#include "rspp_host.h"
#include "rspp_radar_sensor.h"
#include "rspp_detection_list.h"

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
    * Name:  RSPP_Compute_Raw_Host_Speed_Uncertainty
    *   This function computes uncertainty in raw host speed based on vehicle
    *   state, calculating speed uncertainty considering vehicle dynamics and
    *   maximum allowable speed thresholds.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   host           - Reference to host vehicle state information
    *   max_otg_speed  - Maximum over-the-ground speed threshold
    *
    * Return Value:
    *   Computed host speed uncertainty variance
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   float32_t RSPP_Compute_Raw_Host_Speed_Uncertainty(
       const RSPP_Host_T &host,
       const float32_t max_otg_speed);

   /******************************************************************************
    * Name:  RSPP_Get_Host_Velocity_Uncertainty
    *   This function computes host velocity uncertainty covariance matrix by
    *   propagating host motion uncertainties into velocity covariance considering
    *   translation effects.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   host               - Reference to host vehicle state information
    *   host_speed_var     - Host speed variance
    *   host_yaw_rate_var  - Host yaw rate variance
    *   translation_vec    - Translation vector [x, y]
    *   velocity_cov       - Output velocity covariance matrix [2x2]
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Get_Host_Velocity_Uncertainty(
       const RSPP_Host_T &host,
       const float32_t host_speed_var,
       const float32_t host_yaw_rate_var,
       const float32_t (&translation_vec)[2],
       float32_t (&velocity_cov)[2][2]);

   /******************************************************************************
    * Name:  RSPP_Compute_Raw_Detection_Uncertainty
    *   This function computes azimuth uncertainty for raw detection based on
    *   sensor capabilities. Calculates azimuth uncertainty based on sensor type
    *   and field of view characteristics using piecewise linear interpolation.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   det_az            - Detection azimuth angle
    *   fov_min_az_rad    - Minimum field of view azimuth in radians
    *   fov_max_az_rad    - Maximum field of view azimuth in radians
    *   interior_fov      - Interior field of view array by look ID
    *   look_id           - Detection look ID
    *   sensor_type       - Type of radar sensor
    *   azimuth_var       - Output azimuth variance
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Compute_Raw_Detection_Uncertainty(
       const float32_t det_az,
       const float32_t fov_min_az_rad,
       const float32_t fov_max_az_rad,
       const float32_t (&interior_fov)[RSPP_DET_NUM_LOOK_ID],
       const RSPP_Det_Look_ID_T look_id,
       const RSPP_Sensor_Type_T sensor_type,
       float32_t &azimuth_var);

   /******************************************************************************
    * Name:  RSPP_Get_Limits_For_FOV
    *   This function determines field of view limits based on detection range
    *   type. Extracts FOV limits from interior FOV array based on range type
    *   classification for short, medium, or long range detections.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   range_type        - Detection range type classification
    *   interior_fov      - Interior field of view array by look ID
    *   min_interior_fov  - Minimum interior FOV limit (output)
    *   max_interior_fov  - Maximum interior FOV limit (output)
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Get_Limits_For_FOV(
       const RSPP_Det_Range_Type_T range_type,
       const float32_t (&interior_fov)[RSPP_DET_NUM_LOOK_ID],
       float32_t &min_interior_fov,
       float32_t &max_interior_fov);

   /******************************************************************************
    * Name:  RSPP_Compute_Azimuth_Std_Vec_Based_On_Sensor_Type
    *   This function computes azimuth standard deviation vector based on sensor
    *   type. Populates standard deviation vector with sensor-specific azimuth
    *   uncertainty values for different angular regions.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   sensor_type  - Type of radar sensor
    *   std_vec      - Output standard deviation vector [7 elements]
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Compute_Azimuth_Std_Vec_Based_On_Sensor_Type(
       const RSPP_Sensor_Type_T &sensor_type,
       float32_t (&std_vec)[7]);

   /******************************************************************************
    * Name:  RSPP_Compute_Azimuth_Breakpoints
    *   This function computes azimuth breakpoint vector for piecewise linear
    *   interpolation. Creates breakpoint vector for azimuth uncertainty
    *   interpolation across different angular regions of sensor field of view.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   min_fov            - Minimum field of view angle
    *   az_safety_margin   - Azimuth safety margin
    *   min_interior_fov   - Minimum interior FOV boundary
    *   frac_az           - Fractional azimuth parameter
    *   max_interior_fov   - Maximum interior FOV boundary
    *   max_fov           - Maximum field of view angle
    *   az_breakpoint_vec  - Output azimuth breakpoint vector [7 elements]
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Compute_Azimuth_Breakpoints(
       const float32_t &min_fov,
       const float32_t &az_safety_margin,
       const float32_t &min_interior_fov,
       const float32_t &frac_az,
       const float32_t &max_interior_fov,
       const float32_t &max_fov,
       float32_t (&az_breakpoint_vec)[7]);

   /******************************************************************************
    * Name:  RSPP_Get_Uncertainty_Of_Compensated_Range_Rate
    *   This function propagates uncertainties into compensated range rate
    *   variance. Uses uncertainty propagation to combine sensor velocity,
    *   detection range rate, and azimuth uncertainties into compensated range
    *   rate variance.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   cos_det_az         - Cosine of detection azimuth angle
    *   sin_det_az         - Sine of detection azimuth angle
    *   sens_vel           - Sensor velocity vector [x, y]
    *   var_det_rng_rate   - Detection range rate variance
    *   var_det_az         - Detection azimuth variance
    *   cov_sens_vel       - Sensor velocity covariance matrix [2x2]
    *   var_comp_rng_rate  - Output compensated range rate variance
    *
    * Return Value: None
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   void RSPP_Get_Uncertainty_Of_Compensated_Range_Rate(
       const float32_t cos_det_az,
       const float32_t sin_det_az,
       const float32_t (&sens_vel)[2],
       const float32_t var_det_rng_rate,
       const float32_t var_det_az,
       const float32_t (&cov_sens_vel)[2][2],
       float32_t &var_comp_rng_rate);

}

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
