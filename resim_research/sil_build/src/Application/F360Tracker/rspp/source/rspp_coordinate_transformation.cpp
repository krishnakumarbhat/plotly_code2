/*===========================================================================*/
/**
 * @file rspp_coordinate_transformation.cpp
 *
 * @brief RSPP coordinate transformation and misalignment compensation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of coordinate transformations from sensor coordinates to
 * vehicle coordinate system (VCS), including azimuth and elevation misalignment
 * compensation, polarity correction, and mounting position offset application.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - VCS: Vehicle Coordinate System
 *   - VACS: Vehicle Alignment Coordinate System
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
#include "rspp_coordinate_transformation.h"
#include "rspp_input_validation.h"
#include "rspp_math.h"
#include "rspp_norm_heading_angle.h"

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
   void RSPP_Calculate_VCS_Position(
       const Raw_Detection_T &raw_detection,
       const RSPP_VCS_Position_T &vcs_mounting_position,
       Processed_Detection_T &processed_detection)
   {
      // Calculate VCS position coordinates
      processed_detection.vcs_position_x =
          vcs_mounting_position.longitudinal +
          (raw_detection.range * processed_detection.cos_vcs_az);

      processed_detection.vcs_position_y =
          vcs_mounting_position.lateral +
          (raw_detection.range * processed_detection.sin_vcs_az);

      processed_detection.vcs_position_z =
          -vcs_mounting_position.height +
          (raw_detection.range * RSPP_Sinf(processed_detection.vcs_el));
   }

   void RSPP_Calculate_VCS_Angles(
       const int8_t polarity,
       const VariableProps_T &sensor_data,
       const Raw_Detection_T &raw_detection,
       Processed_Detection_T &processed_detection)
   {
      // Compensate detection VCS azimuth and elevation misalignment angle
      // Apply polarity correction and misalignment compensation
      const float32_t polarity_factor = static_cast<float32_t>(polarity);

      processed_detection.vcs_az = RSPP_Normalize_Heading_Angle(
          sensor_data.vacs_boresight_az_estimated +
              (raw_detection.azimuth * polarity_factor),
          0.0F);

      processed_detection.vcs_el =
          sensor_data.vacs_boresight_el_estimated +
          (raw_detection.elevation * polarity_factor);

      // Compute trigonometric values for efficiency
      processed_detection.cos_vcs_az = RSPP_Cosf(processed_detection.vcs_az);
      processed_detection.sin_vcs_az = RSPP_Sinf(processed_detection.vcs_az);
   }

   bool RSPP_Calculate_Detection_VCS_Coordinates(
       const Raw_Detection_T &raw_detection,
       const VariableProps_T &sensor_data,
       const RSPP_Sensor_Calib_T &sensor_calibration_data,
       Processed_Detection_T &processed_detection)
   {
      // Check input detection position data
      const bool f_input_valid = RSPP_Check_Input_Detection_Position_Data(
          raw_detection, sensor_calibration_data.fov_max_az_rad[sensor_data.look_id],
          sensor_calibration_data.fov_min_az_rad[sensor_data.look_id]);

      // Compensate for azimuth and elevation misalignment
      RSPP_Calculate_VCS_Angles(
          sensor_calibration_data.polarity,
          sensor_data,
          raw_detection,
          processed_detection);

      // Calculate VCS coordinates using mounting position from calibration
      RSPP_Calculate_VCS_Position(
          raw_detection,
          sensor_calibration_data.vcs_mounting_position,
          processed_detection);

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
