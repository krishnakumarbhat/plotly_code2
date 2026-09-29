/*===========================================================================*/
/**
 * @file rspp_input_validation.cpp
 *
 * @brief RSPP input validation and range checking functions
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of validation functions for RSPP inputs including sensor data,
 * ego vehicle motion, and radar detection lists. Performs range checking, NaN/Inf
 * detection, and data integrity validation to ensure safe processing.
 *
 * @section ABBR ABBREVIATIONS:
 *   - RSPP: Radar Signal Pre-Processing
 *   - NaN: Not a Number
 *   - Inf: Infinity
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
#include "rspp_input_validation.h"
#include "rspp_constants.h"
#include "rspp_math.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/
namespace rspp_variant_A
{
   /******************************************************************************
    * Name:  RSPP_Is_Invalid_Float
    *   This function checks if a floating point value is NaN (Not a Number) or
    *   Inf (Infinity), returning true if the value is invalid.
    *
    * Shared Variables: None
    *
    * Parameters:
    *   value - Floating point value to check for validity
    *
    * Return Value:
    *   true - If value is NaN or Inf
    *   false - Otherwise
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   static bool RSPP_Is_Invalid_Float(const float32_t value);

   /******************************************************************************
    * Name:  RSPP_Is_Float_Out_Of_Range
    *   This function checks if a floating point value is outside the specified
    *   range [min_val, max_val] or is invalid (NaN/Inf).
    *
    * Shared Variables: None
    *
    * Parameters:
    *   value   - Floating point value to check
    *   min_val - Minimum valid value (inclusive)
    *   max_val - Maximum valid value (inclusive)
    *
    * Return Value:
    *   true - If value is out of range or invalid
    *   false - Otherwise
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   static bool RSPP_Is_Float_Out_Of_Range(
       const float32_t value,
       const float32_t min_val,
       const float32_t max_val);
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
   static bool RSPP_Is_Invalid_Float(const float32_t value)
   {
      return (std::isnan(value) || std::isinf(value));
   }

   static bool RSPP_Is_Float_Out_Of_Range(
       const float32_t value,
       const float32_t min_val,
       const float32_t max_val)
   {
      return (RSPP_Is_Invalid_Float(value) || (value < min_val) || (value > max_val));
   }

   bool RSPP_Input_Sensor_Data_Check(
       const VariableProps_T &sensor_data)
   {
      bool is_valid = true;

      // Validate each sensor's dynamic data only if there are valid detections
      if ((true == sensor_data.is_valid) && (0U < sensor_data.number_of_valid_detections))
      {
         // Validate look id
         const int8_t look_id = static_cast<int8_t>(sensor_data.look_id);
         const int8_t look_id_min = static_cast<int8_t>(RSPP_DET_LOOK_ID_0);
         const int8_t look_id_max = static_cast<int8_t>(RSPP_DET_LOOK_ID_3);
         if ((look_id < look_id_min) || (look_id > look_id_max))
         {
            is_valid = false;
         }

         // Validate boresight azimuth
         const float32_t boresight_az_min = -RSPP_PI;
         const float32_t boresight_az_max = RSPP_PI;
         if (RSPP_Is_Float_Out_Of_Range(sensor_data.vacs_boresight_az_estimated, boresight_az_min, boresight_az_max))
         {
            is_valid = false;
         }

         // Validate boresight elevation
         const float32_t boresight_el_min = -RSPP_PI;
         const float32_t boresight_el_max = RSPP_PI;
         if (RSPP_Is_Float_Out_Of_Range(sensor_data.vacs_boresight_el_estimated, boresight_el_min, boresight_el_max))
         {
            is_valid = false;
         }

         // Validate vehicle velocities (from sensor data)
         const float32_t vcs_long_vel_min = -138.8F;
         const float32_t vcs_long_vel_max = 138.8F;
         if (RSPP_Is_Float_Out_Of_Range(sensor_data.vcs_velocity.longitudinal, vcs_long_vel_min, vcs_long_vel_max))
         {
            is_valid = false;
         }

         const float32_t vcs_lat_vel_min = -138.8F;
         const float32_t vcs_lat_vel_max = 138.8F;
         if (RSPP_Is_Float_Out_Of_Range(sensor_data.vcs_velocity.lateral, vcs_lat_vel_min, vcs_lat_vel_max))
         {
            is_valid = false;
         }

         // Validate detection count
         if (MAX_DETS_FOR_SINGLE_SENSOR < sensor_data.number_of_valid_detections)
         {
            is_valid = false;
         }
      }

      return is_valid;
   }

   bool RSPP_Check_Input_Detection_Position_Data(
       const Raw_Detection_T &raw_detection,
       const float32_t fov_max_az_rad,
       const float32_t fov_min_az_rad)
   {
      bool is_valid = true;

      // Validate range
      const float32_t range_min = 0.0F;
      const float32_t range_max = 500.0F;
      if (RSPP_Is_Float_Out_Of_Range(raw_detection.range, range_min, range_max))
      {
         is_valid = false;
      }

      // Validate azimuth
      const float32_t azimuth_min = fov_min_az_rad;
      const float32_t azimuth_max = fov_max_az_rad;
      if (RSPP_Is_Float_Out_Of_Range(raw_detection.azimuth, azimuth_min, azimuth_max))
      {
         is_valid = false;
      }

      // Validate elevation
      const float32_t elevation_min = -0.53F; // -30 deg
      const float32_t elevation_max = 0.53F;  // 30 deg
      if (RSPP_Is_Float_Out_Of_Range(raw_detection.elevation, elevation_min, elevation_max))
      {
         is_valid = false;
      }

      return is_valid;
   }

   bool RSPP_Check_Input_Detection_Velocity_Data(
       const Raw_Detection_T &raw_detection)
   {
      bool is_valid = true;

      // Validate range rate
      const float32_t range_rate_min = -128.0F;
      const float32_t range_rate_max = 128.0F;
      if (RSPP_Is_Float_Out_Of_Range(raw_detection.range_rate, range_rate_min, range_rate_max))
      {
         is_valid = false;
      }

      return is_valid;
   }

   bool RSPP_Check_Detection_Meta_Data(
       const Raw_Detection_T &raw_detection)
   {
      bool is_valid = true;

      // Validate sensor ID
      if ((0 >= raw_detection.sensor_id) || (static_cast<int32_t>(MAX_NUMBER_OF_SENSORS) < raw_detection.sensor_id))
      {
         is_valid = false;
      }

      // Validate detection ID
      if ((0 >= raw_detection.det_id) || (static_cast<int32_t>(MAX_DETS_FOR_SINGLE_SENSOR) < raw_detection.det_id))
      {
         is_valid = false;
      }

      return is_valid;
   }

   bool RSPP_Check_Sensor_Calibration(
       const ConstantProps_T &constant_props)
   {
      bool is_valid = true;

      // Validate sensor ID
      if ((0U == constant_props.id) || (static_cast<uint32_t>(MAX_NUMBER_OF_SENSORS) < constant_props.id))
      {
         is_valid = false;
      }

      // Validate polarity (must be 1 or -1)
      if ((1 != constant_props.polarity) && (-1 != constant_props.polarity))
      {
         is_valid = false;
      }

      // Validate mounting position coordinates
      const float32_t mounting_pos_long_min = -10.0F;
      const float32_t mounting_pos_long_max = 1.0F;
      if (RSPP_Is_Float_Out_Of_Range(constant_props.mounting_position.vcs_position.longitudinal,
                                     mounting_pos_long_min, mounting_pos_long_max))
      {
         is_valid = false;
      }
      const float32_t mounting_pos_lat_min = -1.5F;
      const float32_t mounting_pos_lat_max = 1.5F;
      if (RSPP_Is_Float_Out_Of_Range(constant_props.mounting_position.vcs_position.lateral,
                                     mounting_pos_lat_min, mounting_pos_lat_max))
      {
         is_valid = false;
      }
      const float32_t mounting_pos_height_min = 0.3F;
      const float32_t mounting_pos_height_max = 1.3F;
      if (RSPP_Is_Float_Out_Of_Range(constant_props.mounting_position.vcs_position.height,
                                     mounting_pos_height_min, mounting_pos_height_max))
      {
         is_valid = false;
      }

      // Validate boresight azimuth angle
      const float32_t boresight_az_angle_min = -RSPP_PI;
      const float32_t boresight_az_angle_max = RSPP_PI;
      if (RSPP_Is_Float_Out_Of_Range(constant_props.mounting_position.vcs_boresight_azimuth_angle,
                                     boresight_az_angle_min, boresight_az_angle_max))
      {
         is_valid = false;
      }

      // Validate FOV and range parameters for each look ID
      for (uint8_t look_id = 0U; (look_id < RSPP_DET_NUM_LOOK_ID) && (is_valid == true); look_id++)
      {
         // Validate azimuth FOV ranges
         const float32_t fov_min_az_min = -RSPP_PI;
         const float32_t fov_min_az_max = RSPP_PI;
         if (RSPP_Is_Float_Out_Of_Range(constant_props.fov_min_az_rad[look_id], fov_min_az_min, fov_min_az_max))
         {
            is_valid = false;
         }
         const float32_t fov_max_az_min = -RSPP_PI;
         const float32_t fov_max_az_max = RSPP_PI;
         if (RSPP_Is_Float_Out_Of_Range(constant_props.fov_max_az_rad[look_id], fov_max_az_min, fov_max_az_max))
         {
            is_valid = false;
         }

         // Validate elevation FOV ranges
         const float32_t fov_min_el_min = -RSPP_PI;
         const float32_t fov_min_el_max = RSPP_PI;
         if (RSPP_Is_Float_Out_Of_Range(constant_props.fov_min_el_rad[look_id], fov_min_el_min, fov_min_el_max))
         {
            is_valid = false;
         }
         const float32_t fov_max_el_min = -RSPP_PI;
         const float32_t fov_max_el_max = RSPP_PI;
         if (RSPP_Is_Float_Out_Of_Range(constant_props.fov_max_el_rad[look_id], fov_max_el_min, fov_max_el_max))
         {
            is_valid = false;
         }

         // Validate FOV min < max for azimuth and elevation
         if (constant_props.fov_min_az_rad[look_id] >= constant_props.fov_max_az_rad[look_id])
         {
            is_valid = false;
         }
         if (constant_props.fov_min_el_rad[look_id] > constant_props.fov_max_el_rad[look_id])
         {
            is_valid = false;
         }

         // Validate range limits
         const float32_t range_limit_min = 0.0F;
         const float32_t range_limit_max = 500.0F;
         if (RSPP_Is_Float_Out_Of_Range(constant_props.range_limits[look_id], range_limit_min, range_limit_max))
         {
            is_valid = false;
         }

         // Validate range rate wrapping intervals (must be positive)
         const float32_t v_wrapping_min = 10.0F;
         const float32_t v_wrapping_max = 100.0F;
         if (RSPP_Is_Float_Out_Of_Range(constant_props.v_wrapping[look_id], v_wrapping_min, v_wrapping_max))
         {
            is_valid = false;
         }

         // Validate minimum aliased range rate
         const float32_t min_aliased_range_rate_min = -200.0F;
         const float32_t min_aliased_range_rate_max = -5.0F;
         if (RSPP_Is_Float_Out_Of_Range(constant_props.min_aliaised_range_rate[look_id],
                                        min_aliased_range_rate_min, min_aliased_range_rate_max))
         {
            is_valid = false;
         }
      }

      return is_valid;
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
