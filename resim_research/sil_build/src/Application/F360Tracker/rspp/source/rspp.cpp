/*===========================================================================*/
/**
 * @file rspp.cpp
 *
 * @brief RSPP main interface implementation
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of the main RSPP API including initialization, calibration
 * management, and detection processing pipeline. Orchestrates coordinate
 * transformation, range rate compensation, motion status classification, and
 * VCS longitudinal sorting of radar detections.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - VCS: Vehicle Coordinate System
 *   - FOV: Field of View
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
#include "rspp.h"
#include "rspp_internal.h"
#include "rspp_state.h"
#include "rspp_input_validation.h"
#include "rspp_coordinate_transformation.h"
#include "rspp_range_rate_compensation.h"
#include "rspp_motion_status_classification.h"
#include "rspp_math.h"
#include "rspp_vcs_long_sorted_dets_support_functions.h"

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
   void RSPP_Initialize(void)
   {
      // Initialize internal state
      RSPP_State_Initialize();
   }

   RSPP_Return_Type_T RSPP_Set_Sensor_Calibrations(
       const F360_Radar_Sensor_T (&sensor_calibrations)[MAX_NUMBER_OF_SENSORS])
   {
      RSPP_Return_Type_T return_status = RSPP_E_OK;

      return_status = RSPP_State_Set_Sensor_Calibrations(sensor_calibrations);

      return return_status;
   }

   RSPP_Return_Type_T RSPP_Process_Detections(
       F360_Radar_Sensor_T (&dynamic_sensor_data)[MAX_NUMBER_OF_SENSORS],
       RSPP_Detection_List_T &detection_list,
       const RSPP_Host_T &vehicle_state_data,
       const RSPP_Core_Info_T &core_info)
   {
      RSPP_Return_Type_T return_status = RSPP_E_OK;

      // Check if RSPP is initialized
      if (RSPP_STATE_INIT_COMPLETE == RSPP_State_Get_Current_State())
      {
         // Input Validation
         bool input_valid = true;

         // Validate sensor data input
         for (int32_t sensor_idx = 0; sensor_idx < static_cast<int32_t>(MAX_NUMBER_OF_SENSORS); sensor_idx++)
         {
            const VariableProps_T &sensor_data = dynamic_sensor_data[sensor_idx].variable;

            // Validate sensor data
            if (!RSPP_Input_Sensor_Data_Check(sensor_data))
            {
               input_valid = false;
            }
            else
            {
               // Calculate and update refined sensor properties
               const RSPP_Sensor_Calib_T &sensor_calibration_data = RSPP_State_Get_Sensor_Calibration(sensor_idx);
               RefinedProps_T &refined_sensor_data = dynamic_sensor_data[sensor_idx].refined;
               RSPP_Calculate_Refined_Sensor_Data(
                   core_info.time_us,
                   sensor_calibration_data,
                   sensor_data,
                   refined_sensor_data);
            }
         }

         // Validate number_of_valid_detections
         if (detection_list.number_of_valid_detections > MAX_NUMBER_OF_DETECTIONS)
         {
            input_valid = false;
         }

         // If input validation fails, discard all detections
         if (!input_valid)
         {
            detection_list.number_of_valid_detections = 0U;
         }
         else
         {
            // Reset uncertainty calculation cache for new processing cycle
            RSPP_Reset_Motion_Status_Cache();

            // Process each valid detection
            bool f_critical_fault_detected = false;
            const uint32_t num_detections = detection_list.number_of_valid_detections;
            for (uint32_t det_idx = 0U; (det_idx < num_detections) && (!f_critical_fault_detected); det_idx++)
            {
               const Raw_Detection_T &raw_detection = detection_list.detections[det_idx].raw;
               Processed_Detection_T &processed_detection = detection_list.detections[det_idx].processed;

               if (!RSPP_Check_Detection_Meta_Data(raw_detection))
               {
                  // Critical detection list fault detected - invalidate detection list
                  detection_list.number_of_valid_detections = 0U;
                  f_critical_fault_detected = true;
               }
               else
               {
                  // Get corresponding sensor data from array
                  const int32_t sensor_idx = raw_detection.sensor_id - 1; // Convert to 0-based index
                  const RSPP_Sensor_Calib_T &sensor_calibration_data = RSPP_State_Get_Sensor_Calibration(sensor_idx);
                  const VariableProps_T &sensor_data = dynamic_sensor_data[sensor_idx].variable;

                  // Coordinate System Transformation
                  const bool f_vcs_coord_ok = RSPP_Calculate_Detection_VCS_Coordinates(
                      raw_detection,
                      sensor_data,
                      sensor_calibration_data,
                      processed_detection);

                  // Range Rate Compensation
                  const bool f_range_rate_comp_ok = RSPP_Calculate_Compensated_RRate(
                      raw_detection,
                      sensor_data,
                      sensor_calibration_data,
                      processed_detection);

                  // Motion Status Classification
                  RSPP_Calculate_Motion_Status(
                      raw_detection,
                      sensor_data,
                      vehicle_state_data,
                      sensor_calibration_data,
                      processed_detection);

                  // Update detection usability flag
                  processed_detection.f_ok_to_use = f_vcs_coord_ok && f_range_rate_comp_ok;
               }
            }

            // Sort detections in VCS longitudinal order and update detection list with sorting info
            RSPP_Sort_Detections_Vcs_Long(detection_list);
         }
      }
      else
      {
         return_status = RSPP_E_NOT_INITIALIZED;
         detection_list.number_of_valid_detections = 0U;
      }

      return return_status;
   }

   void RSPP_Calculate_Refined_Sensor_Data(
       const uint64_t current_time_us,
       const RSPP_Sensor_Calib_T &sensor_calibration_data,
       const VariableProps_T &sensor_data,
       RefinedProps_T &refined_sensor_data)
   {
      // Only update if sensor is valid
      if (sensor_data.is_valid)
      {
         // Update time since measurement
         static const float32_t US_TO_S = 1e-6F;
         const uint64_t time_diff_us = current_time_us - sensor_data.timestamp_us;
         refined_sensor_data.time_since_measurement_s = static_cast<float32_t>(time_diff_us) * US_TO_S;

         // Copy FOV update calibration from RSPP state
         for (uint8_t look_idx = 0U; look_idx < RSPP_DET_NUM_LOOK_ID; look_idx++)
         {
            refined_sensor_data.interior_fov[look_idx] = sensor_calibration_data.interior_fov[look_idx];
            refined_sensor_data.left_fov_normal[look_idx] = sensor_calibration_data.left_fov_normal[look_idx];
            refined_sensor_data.right_fov_normal[look_idx] = sensor_calibration_data.right_fov_normal[look_idx];
         }
      }
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
