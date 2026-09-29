/*===========================================================================*/
/**
 * @file rspp_range_rate_compensation.cpp
 *
 * @brief Range rate ego-motion compensation implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of range rate compensation for radar detections including
 * ego-motion compensation and velocity wrapping for disambiguated measurements.
 * Compensates detection range rates by removing the contribution of host
 * vehicle velocity.
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

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_range_rate_compensation.h"
#include "rspp_input_validation.h"
#include "rspp_math.h"

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

   /*===========================================================================*
    * Local Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Static Variable Definitions
    *===========================================================================*/

   /*===========================================================================*
    * Function Definitions
    *===========================================================================*/
   float32_t RSPP_Calculate_Ego_Motion_Range_Rate (
       const RSPP_VCS_Velocity_T &sensor_vcs_velocity,
       const float32_t vcs_azimuth)
   {
      // Calculate predicted range rate based on ego vehicle motion
      const float32_t ego_motion_range_rate =
          (sensor_vcs_velocity.longitudinal * RSPP_Cosf(vcs_azimuth)) +
          (sensor_vcs_velocity.lateral * RSPP_Sinf(vcs_azimuth));

      return ego_motion_range_rate;
   }

   void RSPP_Apply_Range_Rate_Wrapping(
       const float32_t range_rate_interval_width,
       float32_t &compensated_range_rate)
   {
      if (0.0F < range_rate_interval_width)
      {
         const float32_t half_interval = range_rate_interval_width * 0.5F;
         const float32_t lower_bound = -half_interval;
         const float32_t upper_bound = half_interval;

         // Apply wrapping if outside bounds
         if (lower_bound > compensated_range_rate)
         {
            // Calculate number of intervals to add
            const float32_t intervals_to_add = std::ceil((lower_bound - compensated_range_rate) / range_rate_interval_width);
            compensated_range_rate += (intervals_to_add * range_rate_interval_width);
         }
         else if (upper_bound < compensated_range_rate)
         {
            // Calculate number of intervals to subtract
            const float32_t intervals_to_subtract = std::ceil((compensated_range_rate - upper_bound) / range_rate_interval_width);
            compensated_range_rate -= (intervals_to_subtract * range_rate_interval_width);
         }
         else
         {
            // No wrapping needed - value is within bounds
         }
      }
   }

   bool RSPP_Calculate_Compensated_RRate(
       const Raw_Detection_T &raw_detection,
       const VariableProps_T &sensor_data,
       const RSPP_Sensor_Calib_T &sensor_calibration_data,
       Processed_Detection_T &processed_detection)
   {
      // Check detect
      const bool f_input_valid = RSPP_Check_Input_Detection_Velocity_Data(raw_detection);

      // Initial Compensation
      const float32_t ego_motion_range_rate = RSPP_Calculate_Ego_Motion_Range_Rate (
          sensor_data.vcs_velocity, processed_detection.vcs_az);

      processed_detection.range_rate_compensated = raw_detection.range_rate + ego_motion_range_rate;

      // Interval Wrapping
      RSPP_Apply_Range_Rate_Wrapping(
          sensor_calibration_data.v_wrapping[sensor_data.look_id], processed_detection.range_rate_compensated);

      return f_input_valid;
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
