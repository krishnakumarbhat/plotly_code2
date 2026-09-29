/*===========================================================================*/
/**
 * @file rspp_uncertainty_calculation.cpp
 *
 * @brief Detection uncertainty calculation implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of uncertainty calculations for radar detections including
 * position, velocity, and acceleration uncertainty propagation. Manages
 * cached sensor-specific uncertainty computations for efficient processing
 * of multiple detections from the same sensor.
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
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_reuse.h" // For MAX_NUMBER_OF_SENSORS

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_uncertainty_calculation.h"
#include "rspp_sensor_capability.h"
#include "rspp_math.h"
#include "rspp_constants.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Using Namespaces
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace rspp_variant_A
{
   /*===========================================================================*
    * Local Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Local Type Declarations
    *===========================================================================*/
   struct SensorUncertaintyCache
   {
      float32_t sensor_vel_var[2][2];
      float32_t det_vcs_azimuth_var;
      float32_t sens_vcs_vel_vec[2];
      bool is_computed;
   };
   struct UncertaintyCache
   {
      float32_t host_speed_var;
      float32_t det_range_rate_var;
      bool is_computed;
   };

   /*===========================================================================*
    * Local Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Static Variable Definitions
    *===========================================================================*/
   static SensorUncertaintyCache s_sensor_cache[MAX_NUMBER_OF_SENSORS]{};
   static UncertaintyCache s_cache{};

   /*===========================================================================*
    * Function Definitions
    *===========================================================================*/
   void RSPP_Reset_Uncertainty_Cache(void)
   {
      // Reset cache for new processing cycle
      for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         s_sensor_cache[i].is_computed = false;
      }
      s_cache.is_computed = false;
   }

   void RSPP_Calculate_Detection_Uncertainty(
       const Raw_Detection_T &raw_detection,
       const Processed_Detection_T &processed_detection,
       const RSPP_Host_T &host,
       const VariableProps_T &sensor_data,
       const RSPP_Sensor_Calib_T &sensor_calibration_data,
       const float32_t range_rate_std,
       float32_t &std_range_rate_compensated_scm)
   {
      const int32_t sensor_idx = static_cast<int32_t>(raw_detection.sensor_id) - 1; // Convert to 0-based index

      // Compute host speed uncertainty (cached globally)
      if (!s_cache.is_computed)
      {
         static const float32_t MAX_OTG_SPEED = 70.0F; /**< Maximum OTG speed for host uncertainty */
         s_cache.host_speed_var = RSPP_Compute_Raw_Host_Speed_Uncertainty(host, MAX_OTG_SPEED);
         s_cache.det_range_rate_var = range_rate_std * range_rate_std;
         s_cache.is_computed = true;
      }

      // Compute sensor-specific uncertainties (cached per sensor)
      if (!s_sensor_cache[sensor_idx].is_computed)
      {
         // Compute sensor velocity uncertainty at sensor location
         const float32_t sens_vcs_mounting_pos[2] = {
             sensor_calibration_data.vcs_mounting_position.longitudinal,
             sensor_calibration_data.vcs_mounting_position.lateral};

         static const float32_t HOST_YAW_RATE_VAR = 1e-6F; /**< Host yaw rate variance */
         RSPP_Get_Host_Velocity_Uncertainty(
             host,
             s_cache.host_speed_var,
             HOST_YAW_RATE_VAR,
             sens_vcs_mounting_pos,
             s_sensor_cache[sensor_idx].sensor_vel_var);

         // Cache sensor velocity vector
         s_sensor_cache[sensor_idx].sens_vcs_vel_vec[0] = sensor_data.vcs_velocity.longitudinal;
         s_sensor_cache[sensor_idx].sens_vcs_vel_vec[1] = sensor_data.vcs_velocity.lateral;

         s_sensor_cache[sensor_idx].is_computed = true;
      }

      // Compute raw detection uncertainties
      float32_t det_azimuth_var;
      RSPP_Compute_Raw_Detection_Uncertainty(
          raw_detection.azimuth,
          sensor_calibration_data.fov_min_az_rad[sensor_data.look_id],
          sensor_calibration_data.fov_max_az_rad[sensor_data.look_id],
          sensor_calibration_data.interior_fov,
          sensor_data.look_id,
          sensor_calibration_data.sensor_type,
          det_azimuth_var);

      // Add mounting uncertainty to azimuth
      static const float32_t AZIMUTH_MOUNTING_VAR_RAD = RSPP_deg2rad(0.15F) * RSPP_deg2rad(0.15F); // Azimuth mounting uncertainty
      s_sensor_cache[sensor_idx].det_vcs_azimuth_var = det_azimuth_var + AZIMUTH_MOUNTING_VAR_RAD;

      // Propagate uncertainties to compensated range rate (detection-specific)
      float32_t det_comp_rng_rate_var;
      RSPP_Get_Uncertainty_Of_Compensated_Range_Rate(
          processed_detection.cos_vcs_az,
          processed_detection.sin_vcs_az,
          s_sensor_cache[sensor_idx].sens_vcs_vel_vec,
          s_cache.det_range_rate_var,
          s_sensor_cache[sensor_idx].det_vcs_azimuth_var,
          s_sensor_cache[sensor_idx].sensor_vel_var,
          det_comp_rng_rate_var);

      // Store the standard deviation
      std_range_rate_compensated_scm = RSPP_Sqrtf(det_comp_rng_rate_var);
   }

} // namespace rspp_variant_A

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
