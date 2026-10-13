/*===========================================================================*/
/**
 * @file f360_coordinate_transformation.cpp
 *
 * @brief RSPP coordinate transformation and misalignment compensation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
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
#include "f360_coordinate_transformation.h"
#include "f360_input_validation.h"
#include "f360_math.h"
#include "f360_norm_heading_angle.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Using Namespaces
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
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
void Calculate_VCS_Position(const F360_Detection_T &raw_detection, const F360_VCS_Position_T &vcs_mounting_position,
                            F360_Detection_Props_T &detection_props)
{
   // Calculate VCS position coordinates
   detection_props.vcs_position.Set_Position(vcs_mounting_position.longitudinal +
                                                (raw_detection.range * detection_props.cos_vcs_az),
                                             vcs_mounting_position.lateral + (raw_detection.range * detection_props.sin_vcs_az));

   detection_props.vcs_position_z = -vcs_mounting_position.height + (raw_detection.range * F360_Sinf(detection_props.vcs_el));
}

void Calculate_VCS_Angles(const F360_Radar_Sensor_T &sensor_data, const F360_Detection_T &raw_detection,
                          F360_Detection_Props_T &detection_props)
{
   // Compensate detection VCS azimuth and elevation misalignment angle
   // Apply polarity correction and misalignment compensation
   const float32_t polarity_factor = static_cast<float32_t>(sensor_data.polarity);

   detection_props.vcs_az =
      Normalize_Heading_Angle(sensor_data.vacs_boresight_az_estimated + (raw_detection.azimuth * polarity_factor), 0.0F);

   detection_props.vcs_el = sensor_data.vacs_boresight_el_estimated + (raw_detection.elevation * polarity_factor);

   // Compute trigonometric values for efficiency
   detection_props.cos_vcs_az = F360_Cosf(detection_props.vcs_az);
   detection_props.sin_vcs_az = F360_Sinf(detection_props.vcs_az);
}

bool Calculate_Detection_VCS_Coordinates(const F360_Detection_T &raw_detection, const F360_Radar_Sensor_T &sensor_data,
                                         F360_Detection_Props_T &detection_props)
{
   // Check input detection position data
   const bool f_input_valid = Check_Input_Detection_Position_Data(raw_detection, sensor_data.fov_max_az_rad[sensor_data.look_id],
                                                                  sensor_data.fov_min_az_rad[sensor_data.look_id]);

   // Compensate for azimuth and elevation misalignment
   Calculate_VCS_Angles(sensor_data, raw_detection, detection_props);

   // Calculate VCS coordinates using mounting position from calibration
   Calculate_VCS_Position(raw_detection, sensor_data.mounting_position.vcs_position, detection_props);

   return f_input_valid;
}

} // namespace f360_variant_A

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
