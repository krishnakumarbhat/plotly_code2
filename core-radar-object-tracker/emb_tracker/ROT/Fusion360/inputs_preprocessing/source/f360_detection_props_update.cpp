/*===========================================================================*/
/**
 * @file f360_detection_props_update.cpp
 *
 * @brief F360 detection properties update implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of F360 detection properties update function which calculates refined
 * properties including VCS coordinates and range rate compensation.
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
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <algorithm>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "f360_coordinate_transformation.h"
#include "f360_detection_props_update.h"
#include "f360_input_validation.h"
#include "f360_math.h"
#include "f360_motion_status_classification.h"
#include "f360_range_rate_compensation.h"

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
void Detection_Props_Update(const F360_Host_T &host, const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
                            const F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS],
                            const F360_Detection_List_T &detect_list, F360_Detection_Props_List_T &detect_props_list)
{
   // Reset uncertainty calculation cache for new processing cycle
   Reset_Motion_Status_Cache();

   // Process each valid detection
   for (uint32_t det_idx = 0U; det_idx < detect_list.number_of_valid_detections; det_idx++)
   {
      const F360_Detection_T &raw_detection = detect_list.detections[det_idx];
      F360_Detection_Props_T &detect_props  = detect_props_list.detection_props[det_idx];

      // Get corresponding sensor data from array
      const int32_t sensor_idx                      = raw_detection.sensor_id - 1; // Convert to 0-based index
      const F360_Radar_Sensor_T &sensor_data        = sensors[sensor_idx];
      const F360_Radar_Sensor_Props_T &sensor_props = sensors_props[sensor_idx];

      // Coordinate System Transformation
      const bool f_vcs_coord_ok = Calculate_Detection_VCS_Coordinates(raw_detection, sensor_data, detect_props);

      // Range Rate Compensation
      const bool f_range_rate_comp_ok = Calculate_Compensated_RRate(raw_detection, sensor_data, detect_props);

      // Motion Status Classification
      Calculate_Motion_Status(raw_detection, sensor_data, sensor_props, host, detect_props);

      // Update detection usability flag
      detect_props.f_ok_to_use = f_vcs_coord_ok && f_range_rate_comp_ok;
   }
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
