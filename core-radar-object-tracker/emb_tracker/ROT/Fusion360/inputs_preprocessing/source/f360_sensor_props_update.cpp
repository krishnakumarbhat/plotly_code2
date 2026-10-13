/*===========================================================================*/
/**
 * @file f360_sensor_props_update.cpp
 *
 * @brief F360 sensor properties update implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of F360 sensor properties update function which calculates refined
 * properties including FOV and timing,
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
#include "f360_input_validation.h"
#include "f360_math.h"
#include "f360_sensor_props_update.h"

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
void Update_Sensor_FOV(float32_t (&interior_fov)[F360_DET_NUM_LOOK_ID], float32_t (&left_fov_normal)[F360_DET_NUM_LOOK_ID],
                       float32_t (&right_fov_normal)[F360_DET_NUM_LOOK_ID],
                       const float32_t (&fov_min_az_rad)[F360_DET_NUM_LOOK_ID],
                       const float32_t (&fov_max_az_rad)[F360_DET_NUM_LOOK_ID], const float32_t vcs_boresight_azimuth_angle)
{
   // normal vectors for edges of interior FOV.
   const float32_t fov_interior_limit           = 1.1345F; // 65 degrees
   const float32_t min_fov_az_angle_lr          = std::min(fov_min_az_rad[F360_DET_LOOK_ID_0], fov_min_az_rad[F360_DET_LOOK_ID_1]);
   const float32_t min_fov_az_interior_angle_lr = std::max(min_fov_az_angle_lr, -fov_interior_limit);
   const float32_t max_fov_az_angle_lr          = std::max(fov_max_az_rad[F360_DET_LOOK_ID_0], fov_max_az_rad[F360_DET_LOOK_ID_1]);
   const float32_t max_fov_az_interior_angle_lr = std::min(max_fov_az_angle_lr, fov_interior_limit);
   const float32_t min_fov_az_angle_mr          = std::min(fov_min_az_rad[F360_DET_LOOK_ID_2], fov_min_az_rad[F360_DET_LOOK_ID_3]);
   const float32_t min_fov_az_interior_angle_mr = std::max(min_fov_az_angle_mr, -fov_interior_limit);
   const float32_t max_fov_az_angle_mr          = std::max(fov_max_az_rad[F360_DET_LOOK_ID_2], fov_max_az_rad[F360_DET_LOOK_ID_3]);
   const float32_t max_fov_az_interior_angle_mr = std::min(max_fov_az_angle_mr, fov_interior_limit);

   interior_fov[F360_DET_LOOK_ID_0] = min_fov_az_interior_angle_lr;
   interior_fov[F360_DET_LOOK_ID_1] = max_fov_az_interior_angle_lr;
   interior_fov[F360_DET_LOOK_ID_2] = min_fov_az_interior_angle_mr;
   interior_fov[F360_DET_LOOK_ID_3] = max_fov_az_interior_angle_mr;

   const float32_t min_fov_vcs_az_angle_lr = vcs_boresight_azimuth_angle + min_fov_az_interior_angle_lr;
   const float32_t max_fov_vcs_az_angle_lr = vcs_boresight_azimuth_angle + max_fov_az_interior_angle_lr;
   const float32_t min_fov_vcs_az_angle_mr = vcs_boresight_azimuth_angle + min_fov_az_interior_angle_mr;
   const float32_t max_fov_vcs_az_angle_mr = vcs_boresight_azimuth_angle + max_fov_az_interior_angle_mr;

   left_fov_normal[F360_DET_LOOK_ID_0]  = -F360_Sinf(min_fov_vcs_az_angle_lr);
   left_fov_normal[F360_DET_LOOK_ID_1]  = F360_Cosf(min_fov_vcs_az_angle_lr);
   right_fov_normal[F360_DET_LOOK_ID_0] = F360_Sinf(max_fov_vcs_az_angle_lr);
   right_fov_normal[F360_DET_LOOK_ID_1] = -F360_Cosf(max_fov_vcs_az_angle_lr);
   left_fov_normal[F360_DET_LOOK_ID_2]  = -F360_Sinf(min_fov_vcs_az_angle_mr);
   left_fov_normal[F360_DET_LOOK_ID_3]  = F360_Cosf(min_fov_vcs_az_angle_mr);
   right_fov_normal[F360_DET_LOOK_ID_2] = F360_Sinf(max_fov_vcs_az_angle_mr);
   right_fov_normal[F360_DET_LOOK_ID_3] = -F360_Cosf(max_fov_vcs_az_angle_mr);
}

void Update_Sensor_Time(const uint64_t tracker_time_us, const uint64_t sensor_timestamp_us,
                        float32_t &sensor_time_since_measurement_s)
{
   const uint64_t time_diff_us     = tracker_time_us - sensor_timestamp_us;
   sensor_time_since_measurement_s = static_cast<float32_t>(time_diff_us) * 1e-6F;
}

void Sensor_Props_Update(const uint64_t tracker_time_us, const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
                         F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS])
{
   for (uint8_t sens_idx = 0U; sens_idx < MAX_NUMBER_OF_SENSORS; ++sens_idx)
   {
      const F360_Radar_Sensor_T &sensor       = sensors[sens_idx];
      F360_Radar_Sensor_Props_T &sensor_props = sensors_props[sens_idx];

      if (sensor.is_valid)
      {
         // Update Sensor FOV properties for valid sensors
         Update_Sensor_FOV(sensor_props.interior_fov, sensor_props.left_fov_normal, sensor_props.right_fov_normal,
                           sensor.fov_min_az_rad, sensor.fov_max_az_rad, sensor.mounting_position.vcs_boresight_azimuth_angle);

         // Update Sensor Time Properties for updated sensors
         Update_Sensor_Time(tracker_time_us, sensor.timestamp_us, sensor_props.time_since_measurement_s);
      }
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
