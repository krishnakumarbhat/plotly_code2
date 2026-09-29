/*===========================================================================*/
/**
 * @file rspp_mark_detection_with_low_detection_conf.cpp
 *
 * @brief Low detection confidence marking implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of detection quality assessment and marking for detections
 * with low confidence. Evaluates sensor-specific characteristics including
 * azimuth confidence, elevation angles, and range rate to identify detections
 * that may have elevated uncertainty affecting motion classification.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - SRR: Short Range Radar
 *   - MRR: Medium Range Radar
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
#include "rspp_constants.h"
#include "rspp_mark_detection_with_low_detection_conf.h"
#include "rspp_math.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/
namespace rspp_variant_A
{
   /******************************************************************************
    * Name:  RSPP_Should_Detection_Be_Processed
    *   This function determines if a detection should be processed for confidence
    *   evaluation based on host speed and sensor type. Only processes detections
    *   for specific sensor types (SRR4, SRR5, MRR360) when host speed exceeds
    *   the minimum threshold.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   sensor_type  - Type of radar sensor
    *   host_vcs_speed - Current host vehicle speed
    *
    * Return Value:
    *   true - If detection should be processed for confidence checks
    *   false - Otherwise
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   static inline bool RSPP_Should_Detection_Be_Processed(
       const RSPP_Sensor_Type_T sensor_type,
       const float32_t host_vcs_speed);
}

/*===========================================================================*
 * Using Namespaces
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace rspp_variant_A
{
   /*===========================================================================*
    * Local Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Local Enum Class Declarations
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
   static inline bool RSPP_Should_Detection_Be_Processed(
       const RSPP_Sensor_Type_T sensor_type,
       const float32_t host_vcs_speed)
   {
      bool f_process_det = false;
      const float32_t k_min_host_speed_for_check_det_az_conf_and_elevation = 2.0F;
      if (k_min_host_speed_for_check_det_az_conf_and_elevation < host_vcs_speed)
      {
         switch (sensor_type)
         {
         case RSPP_SENSOR_TYPE_SRR4_RADAR:
         case RSPP_SENSOR_TYPE_SRR5_RADAR:
         case RSPP_SENSOR_TYPE_MRR360_RADAR:
         {
            f_process_det = true;
            break;
         }
         default:
         {
            break;
         }
         }
      }
      return f_process_det;
   }

   bool RSPP_Mark_Detection_With_Low_Detection_Confidence(
       const RSPP_Sensor_Type_T sensor_type,
       const float32_t host_vcs_speed,
       const Raw_Detection_T &raw_detection,
       const float32_t range_rate_compensated)
   {
      bool f_azimuth_error_stat_mov = false;

      const bool f_process_det = RSPP_Should_Detection_Be_Processed(sensor_type, host_vcs_speed);
      if (f_process_det)
      {
         const float32_t k_srr4_min_range_az_conf = 15.0F;
         const float32_t k_srr4_max_range_rate_comp_az_conf = 5.0F;
         const float32_t k_srr4_min_azimuth_az_conf = 0.785398F;   // 45 deg
         const float32_t k_srr4_max_azimuth_super_res = 0.349066F; // 20 deg
         const float32_t k_srr4_max_elevation = 0.0872665F;        // 5 deg

         const bool f_low_az_conf_slow_det =
             (1 < raw_detection.confid_azimuth) &&
             (k_srr4_max_range_rate_comp_az_conf > std::abs(range_rate_compensated)) &&
             (k_srr4_min_range_az_conf < raw_detection.range);

         const bool f_low_az_conf_high_az =
             (1 < raw_detection.confid_azimuth) &&
             (k_srr4_min_azimuth_az_conf < std::abs(raw_detection.azimuth)) &&
             (k_srr4_min_range_az_conf < raw_detection.range);

         const bool f_super_res_az_too_big =
             raw_detection.f_super_res && (k_srr4_max_azimuth_super_res < std::abs(raw_detection.azimuth));

         const bool f_elevation_too_big = k_srr4_max_elevation < std::abs(raw_detection.elevation);

         const bool f_low_detection_conf =
             f_low_az_conf_slow_det || f_low_az_conf_high_az || f_super_res_az_too_big || f_elevation_too_big;

         if (f_low_detection_conf)
         {
            f_azimuth_error_stat_mov = true;
         }
      }

      return f_azimuth_error_stat_mov;
   }
}

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
