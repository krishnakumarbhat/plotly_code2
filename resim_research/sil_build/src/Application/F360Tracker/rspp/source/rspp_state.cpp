/*===========================================================================*/
/**
 * @file rspp_state.cpp
 *
 * @brief RSPP internal state management implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of RSPP module state management including initialization,
 * calibration storage, and state tracking. Manages sensor-specific calibration
 * data and module lifecycle state transitions.
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
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <cstring>
#include <algorithm>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_state.h"
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
   static RSPP_Sensor_Calib_T RSPP_Sensor_Calibrations[MAX_NUMBER_OF_SENSORS] = {};
   static bool RSPP_Sensor_Calibrations_Initialized[MAX_NUMBER_OF_SENSORS] = {};
   static RSPP_States_Type_T RSPP_State = RSPP_STATE_UNINITIALIZED;

   /*===========================================================================*
    * Function Definitions
    *===========================================================================*/
   void RSPP_State_Reset(void)
   {
      for (uint8_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         // Reset Sensor calibration data
         for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
         {
            RSPP_Sensor_Calibrations[sensor_idx].fov_min_az_rad[look_id] = 0.0F;
            RSPP_Sensor_Calibrations[sensor_idx].fov_max_az_rad[look_id] = 0.0F;
            RSPP_Sensor_Calibrations[sensor_idx].interior_fov[look_id] = 0.0F;
            RSPP_Sensor_Calibrations[sensor_idx].left_fov_normal[look_id] = 0.0F;
            RSPP_Sensor_Calibrations[sensor_idx].right_fov_normal[look_id] = 0.0F;
            RSPP_Sensor_Calibrations[sensor_idx].v_wrapping[look_id] = 0.0F;
         }
         RSPP_Sensor_Calibrations[sensor_idx].vcs_mounting_position.height = 0.0F;
         RSPP_Sensor_Calibrations[sensor_idx].vcs_mounting_position.lateral = 0.0F;
         RSPP_Sensor_Calibrations[sensor_idx].vcs_mounting_position.longitudinal = 0.0F;
         RSPP_Sensor_Calibrations[sensor_idx].sensor_type = RSPP_SENSOR_TYPE_UNKNOWN;
         RSPP_Sensor_Calibrations[sensor_idx].polarity = 0;

         // Set Sensor calibration initialized flags to false
         RSPP_Sensor_Calibrations_Initialized[sensor_idx] = false;
      }

      // Set state to initialized
      RSPP_State = RSPP_STATE_UNINITIALIZED;
   }

   void RSPP_State_Initialize(void)
   {
      // Reset State
      RSPP_State_Reset();

      // Set state to initialized
      RSPP_State = RSPP_STATE_INITIALIZED;
   }

   RSPP_States_Type_T RSPP_State_Get_Current_State(void)
   {
      return RSPP_State;
   }

   RSPP_Return_Type_T RSPP_State_Set_Sensor_Calibrations(
       const F360_Radar_Sensor_T (&sensor_calibrations)[MAX_NUMBER_OF_SENSORS])
   {
      RSPP_Return_Type_T return_status = RSPP_E_OK;

      // Check if RSPP is initialized
      if (RSPP_STATE_UNINITIALIZED == RSPP_State)
      {
         return_status = RSPP_E_NOT_INITIALIZED;
      }
      else
      {
         // Set calibration for each sensor
         for (uint32_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
         {
            // Set sensor calibration in RSPP state
            return_status = RSPP_State_Set_Sensor_Calibration(sensor_calibrations[sensor_idx].constant);

            // If error occurs, break the loop and return the error
            if (return_status != RSPP_E_OK)
            {
               break;
            }
         }

         // If all sensors are initialized, update state to INIT_COMPLETE
         if (RSPP_E_OK == return_status)
         {
            RSPP_State = RSPP_STATE_INIT_COMPLETE;
         }
      }

      return return_status;
   }

   RSPP_Return_Type_T RSPP_State_Set_Sensor_Calibration(
       const ConstantProps_T &sensor_calibration)
   {
      RSPP_Return_Type_T return_status = RSPP_E_OK;
      const uint32_t sensor_idx = sensor_calibration.id - 1U;

      // Validate sensor ID
      if ((MAX_NUMBER_OF_SENSORS <= sensor_idx) || (RSPP_Sensor_Calibrations_Initialized[sensor_idx]))
      {
         return_status = RSPP_E_INVALID_SENSOR_ID;
      }
      else
      {
         // Validate sensor calibration data using dedicated validation function
         const bool f_sensor_valid = RSPP_Check_Sensor_Calibration(sensor_calibration);

         if (!f_sensor_valid)
         {
            return_status = RSPP_E_INVALID_CALIBRATION;
         }
         else
         {
            RSPP_Sensor_Calib_T &rspp_sensor_calibration = RSPP_Sensor_Calibrations[sensor_idx];

            // Store sensor calibration data
            RSPP_Update_Sensor_FOV(
                rspp_sensor_calibration.interior_fov,
                rspp_sensor_calibration.left_fov_normal,
                rspp_sensor_calibration.right_fov_normal,
                sensor_calibration.fov_min_az_rad,
                sensor_calibration.fov_max_az_rad,
                sensor_calibration.mounting_position.vcs_boresight_azimuth_angle);

            // Copy other calibration data
            for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
            {
               rspp_sensor_calibration.v_wrapping[look_id] = sensor_calibration.v_wrapping[look_id];
               rspp_sensor_calibration.fov_min_az_rad[look_id] = sensor_calibration.fov_min_az_rad[look_id];
               rspp_sensor_calibration.fov_max_az_rad[look_id] = sensor_calibration.fov_max_az_rad[look_id];
            }
            const RSPP_Sensor_Mounting_Position_T &mounting_position = sensor_calibration.mounting_position;
            rspp_sensor_calibration.vcs_mounting_position.height = mounting_position.vcs_position.height;
            rspp_sensor_calibration.vcs_mounting_position.lateral = mounting_position.vcs_position.lateral;
            rspp_sensor_calibration.vcs_mounting_position.longitudinal = mounting_position.vcs_position.longitudinal;
            rspp_sensor_calibration.sensor_type = sensor_calibration.sensor_type;
            rspp_sensor_calibration.polarity = static_cast<int8_t>(sensor_calibration.polarity);

            // Set sensor calibration initialized flag to true
            RSPP_Sensor_Calibrations_Initialized[sensor_idx] = true;
         }
      }

      return return_status;
   }

   const RSPP_Sensor_Calib_T &RSPP_State_Get_Sensor_Calibration(
       const int32_t sensor_idx)
   {
      // This is called only after validation, so sensor_idx is assumed valid
      return RSPP_Sensor_Calibrations[sensor_idx];
   }

   void RSPP_Update_Sensor_FOV(
       float32_t (&interior_fov)[RSPP_DET_NUM_LOOK_ID],
       float32_t (&left_fov_normal)[RSPP_DET_NUM_LOOK_ID],
       float32_t (&right_fov_normal)[RSPP_DET_NUM_LOOK_ID],
       const float32_t (&fov_min_az_rad)[RSPP_DET_NUM_LOOK_ID],
       const float32_t (&fov_max_az_rad)[RSPP_DET_NUM_LOOK_ID],
       const float32_t vcs_boresight_azimuth_angle)
   {
      // normal vectors for edges of interior FOV.
      const float32_t fov_interior_limit = 1.1345F; // 65 degrees
      const float32_t min_fov_az_angle_lr = std::min(fov_min_az_rad[RSPP_DET_LOOK_ID_0], fov_min_az_rad[RSPP_DET_LOOK_ID_1]);
      const float32_t min_fov_az_interior_angle_lr = std::max(min_fov_az_angle_lr, -fov_interior_limit);
      const float32_t max_fov_az_angle_lr = std::max(fov_max_az_rad[RSPP_DET_LOOK_ID_0], fov_max_az_rad[RSPP_DET_LOOK_ID_1]);
      const float32_t max_fov_az_interior_angle_lr = std::min(max_fov_az_angle_lr, fov_interior_limit);
      const float32_t min_fov_az_angle_mr = std::min(fov_min_az_rad[RSPP_DET_LOOK_ID_2], fov_min_az_rad[RSPP_DET_LOOK_ID_3]);
      const float32_t min_fov_az_interior_angle_mr = std::max(min_fov_az_angle_mr, -fov_interior_limit);
      const float32_t max_fov_az_angle_mr = std::max(fov_max_az_rad[RSPP_DET_LOOK_ID_2], fov_max_az_rad[RSPP_DET_LOOK_ID_3]);
      const float32_t max_fov_az_interior_angle_mr = std::min(max_fov_az_angle_mr, fov_interior_limit);

      interior_fov[RSPP_DET_LOOK_ID_0] = min_fov_az_interior_angle_lr;
      interior_fov[RSPP_DET_LOOK_ID_1] = max_fov_az_interior_angle_lr;
      interior_fov[RSPP_DET_LOOK_ID_2] = min_fov_az_interior_angle_mr;
      interior_fov[RSPP_DET_LOOK_ID_3] = max_fov_az_interior_angle_mr;

      const float32_t min_fov_vcs_az_angle_lr = vcs_boresight_azimuth_angle + min_fov_az_interior_angle_lr;
      const float32_t max_fov_vcs_az_angle_lr = vcs_boresight_azimuth_angle + max_fov_az_interior_angle_lr;
      const float32_t min_fov_vcs_az_angle_mr = vcs_boresight_azimuth_angle + min_fov_az_interior_angle_mr;
      const float32_t max_fov_vcs_az_angle_mr = vcs_boresight_azimuth_angle + max_fov_az_interior_angle_mr;

      left_fov_normal[RSPP_DET_LOOK_ID_0] = -RSPP_Sinf(min_fov_vcs_az_angle_lr);
      left_fov_normal[RSPP_DET_LOOK_ID_1] = RSPP_Cosf(min_fov_vcs_az_angle_lr);
      right_fov_normal[RSPP_DET_LOOK_ID_0] = RSPP_Sinf(max_fov_vcs_az_angle_lr);
      right_fov_normal[RSPP_DET_LOOK_ID_1] = -RSPP_Cosf(max_fov_vcs_az_angle_lr);
      left_fov_normal[RSPP_DET_LOOK_ID_2] = -RSPP_Sinf(min_fov_vcs_az_angle_mr);
      left_fov_normal[RSPP_DET_LOOK_ID_3] = RSPP_Cosf(min_fov_vcs_az_angle_mr);
      right_fov_normal[RSPP_DET_LOOK_ID_2] = RSPP_Sinf(max_fov_vcs_az_angle_mr);
      right_fov_normal[RSPP_DET_LOOK_ID_3] = -RSPP_Cosf(max_fov_vcs_az_angle_mr);
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
