#ifndef RSPP_RADAR_SENSOR_VARIANT_A_H
#define RSPP_RADAR_SENSOR_VARIANT_A_H
/*===========================================================================*/
/**
 * @file rspp_radar_sensor.h
 *
 * @brief Radar Sensor Structure
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Radar sensor configuration and calibration structure definitions.
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
 *
 * @defgroup rspp_radar_sensor Radar Sensor
 * @{
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include "rspp_reuse.h"

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_velocity.h"
#include "rspp_range_type.h"
#include "rspp_look_type.h"
#include "rspp_sensor_mounting_position.h"
#include "rspp_mounting_location.h"
#include "rspp_sensor_type.h"
#include "rspp_look_ID.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace rspp_variant_A
{
   /*===========================================================================*
    * Exported Enum Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/
   /******************************************************************************
    * Name:  F360_Internal_Reflections_Calib_T
    *  Structure for internal reflections identification calibrations.
    *
    * Struct Members:
    *   min_host_vel          - [m/s] identification of internal reflections only updates its buffer slots if host is moving at
    *                           least this fast
    *   occurrence_lowerlimit - [fraction of tracker iterations] identification of internal reflections resets buffer slots below
    *                           this occurrence rate
    *   occurrence_threshold  - [fraction of tracker iterations] identification of internal reflections considers a buffer slot a
    *                           reflection if it occurred at least this often (as a fraction of occurrence_count)
    *   rcs_tolerance         - [dB/m^2] identification of internal reflections counts recurrence of detections based on this rcs
    *                           tolerance
    *   azimuth_tolerance     - [rad] identification of internal reflections counts recurrence of detections based on this azimuth
    *                           tolerance
    *   range_tolerance       - [m/s] identification of internal reflections counts recurrence of detections based on this range
    *                           tolerance
    *   max_abs_range_rate    - [m/s] identification of internal reflections only treats detections below abs(range_rate)
    *   rcs_max               - [dB/m^2] identification of internal reflections only treats detections below this rcs
    *   range_max             - [m] identification of internal reflections only treats detections within this range
    *   age_threshold         - [tracker iterations] Mininmum number of tracker iterations before a buffer slot is evaluated if it
    *                           is an internal reflection
    *   f_enable              - [-] When set to true, the internal reflections functionality will be active for this sensor
    ******************************************************************************/
   typedef struct F360_Internal_Reflections_Calib_Tag
   {
      float32_t min_host_vel;
      float32_t occurrence_lowerlimit;
      float32_t occurrence_threshold;

      float32_t rcs_tolerance;
      float32_t azimuth_tolerance;
      float32_t range_tolerance;

      float32_t max_abs_range_rate;
      float32_t rcs_max;
      float32_t range_max;

      uint16_t age_threshold;

      bool f_enable;

      uint8_t padding[1];
   } F360_Internal_Reflections_Calib_T;

   /******************************************************************************
    * Name:  ConstantProps_T
    *  Structure for constant properties of the radar sensor.
    *
    * Struct Members:
    *   internal_reflections    - Contains enable/disable & tuning parameters for the internal reflections functionality
    *   mounting_position       - Mounting position data for the sensor.
    *   range_limits            - [m] List of limits on the range for which the sensors can measure for each variant of look index.
    *   fov_min_az_rad          - [rad] List of minimum field of view of the azimuth angle for each variant of look index.
    *   fov_max_az_rad          - [rad] List of maximum field of view of the azimuth angle for each variant of look index.
    *   fov_min_el_rad          - [rad] List of minimum field of view of the elevation angle for each variant of look index.
    *   fov_max_el_rad          - [rad] List of maximum field of view of the elevation angle for each variant of look index.
    *   min_aliaised_range_rate - [m/s] List of minimum values of the aliased range rate that can be received for each variant of
    *                             look index.
    *   v_wrapping              - [m/s] Range rate dealiasing interval
    *   r_wrapping              - [m] Range offset after doppler unfolding; 0 for FMCW and nonzero for SFW
    *   id                      - ID of the sensor.
    *   polarity                - 1 = normal, -1 = flipped
    *   mounting_location       - Tag describing the location of the sensor on the host vehicle.
    *   sensor_type             - Tag describing the type of sensor.
    ******************************************************************************/
   typedef struct ConstantProps_Tag
   {
      // Internal reflection calibrations
      F360_Internal_Reflections_Calib_T internal_reflections;

      // Mounting properties
      RSPP_Sensor_Mounting_Position_T mounting_position;

      // Detection properties
      float32_t range_limits[RSPP_DET_NUM_LOOK_ID];
      float32_t fov_min_az_rad[RSPP_DET_NUM_LOOK_ID];
      float32_t fov_max_az_rad[RSPP_DET_NUM_LOOK_ID];
      float32_t fov_min_el_rad[RSPP_DET_NUM_LOOK_ID];
      float32_t fov_max_el_rad[RSPP_DET_NUM_LOOK_ID];
      float32_t min_aliaised_range_rate[RSPP_DET_NUM_LOOK_ID];
      float32_t v_wrapping[RSPP_DET_NUM_LOOK_ID];
      float32_t r_wrapping[RSPP_DET_NUM_LOOK_ID];

      uint32_t id;
      int32_t polarity;

      // Mounting properties
      RSPP_Mounting_Location_T mounting_location;

      // Sensor properties
      RSPP_Sensor_Type_T sensor_type;

      uint8_t padding[2];
   } ConstantProps_T;

   /******************************************************************************
    * Name:  VariableProps_T
    *  Structure for variable properties of the radar sensor.
    *
    * Struct Members:
    *   timestamp_us                - [us] Time stamp of the current radar scan in microseconds.
    *   vcs_velocity                - Velocity of the sensor in VCS
    *   vacs_boresight_az_estimated - [rad] Estimated boresight azimuth angle of the sensor in VCS, compensated for sensor
    *                                 alignment error.
    *   vacs_boresight_el_estimated - [rad] Estimated boresight elevation angle of the sensor in VCS, compensated for sensor
    *                                 alignment error.
    *   number_of_valid_detections  - Number of detections provided from this sensor in the current scan.
    *   overall_rain_level          - Rain level; 0 - no rain; 1 - low rain; 2 - high rain
    *   look_index                  - Look index for the current detection
    *   is_valid                    - A flag that indicates if sensor is used/enabled.
    *   f_sensor_fault_detected     - Indicates if a sensor fault is detected (only used in a specific project)
    *   look_id                     - Look ID for the current detection
    ******************************************************************************/
   typedef struct VariableProps_Tag
   {
      uint64_t timestamp_us;

      // Motion info
      RSPP_VCS_Velocity_T vcs_velocity;
      float32_t vacs_boresight_az_estimated;
      float32_t vacs_boresight_el_estimated;

      uint32_t number_of_valid_detections;

      uint16_t overall_rain_level;

      uint16_t look_index;

      bool is_valid;

      bool f_sensor_fault_detected;

      RSPP_Det_Look_ID_T look_id;

      uint8_t padding[5];
   } VariableProps_T;

   /******************************************************************************
    * Name:  RefinedProps_T
    *  Structure for refined properties of the radar sensor.
    *
    * Struct Members:
    *   interior_fov      - [rad] The valid boundary of the radar field of view - per look id.
    *   left_fov_normal   - Sensor left edge field of view normal vector towards inside [-sinf(theta), cosf(theta)] - per look id.
    *   right_fov_normal  - Sensor right edge field of view normal vector towards inside [sinf(theta), -cosf(theta)] - per look id.
    *   time_since_measurement_s - [s] Delta time between RSPP excution and sensor measurement timestamp.
    ******************************************************************************/
   typedef struct RefinedProps_Tag
   {
      float32_t interior_fov[RSPP_DET_NUM_LOOK_ID];
      float32_t left_fov_normal[RSPP_DET_NUM_LOOK_ID];
      float32_t right_fov_normal[RSPP_DET_NUM_LOOK_ID];
      float32_t time_since_measurement_s;
   } RefinedProps_T;

   /******************************************************************************
    * Name:  F360_Radar_Sensor_T
    *  Structure for radar sensor properties.
    *
    * Struct Members:
    *   constant - Sensor calibration parameters which is set during initialization.
    *   variable - Sensor data that should be updated before tracker iteration.
    *   refined  - Refined sensor properties that are helpful for several intermediate functions.
    ******************************************************************************/
   typedef struct F360_Radar_Sensor_Tag
   {
      ConstantProps_T constant;
      VariableProps_T variable;
      RefinedProps_T refined;

      uint8_t padding[1];
   } F360_Radar_Sensor_T;

   /*===========================================================================*
    * Exported Class Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Declarations
    *===========================================================================*/
   /******************************************************************************
    * Name:  RSPP_Get_Range_Type
    *   Function to map Look ID to Range Type
    *
    * Shared Variables: None
    *
    * Parameters:
    *   look_ID - Input Look ID
    *
    * Return Value:
    *   Corresponding Range Type
    *
    * Design Information: None
    *
    * Change References: None
    *
    ******************************************************************************/
   inline RSPP_Det_Range_Type_T RSPP_Get_Range_Type(const RSPP_Det_Look_ID_T look_ID)
   {
      RSPP_Det_Range_Type_T range_type;
      switch (look_ID)
      {
      case RSPP_DET_LOOK_ID_0:
      {
         range_type = RSPP_DET_RANGE_TYPE_LONG;
         break;
      }
      case RSPP_DET_LOOK_ID_1:
      {
         range_type = RSPP_DET_RANGE_TYPE_LONG;
         break;
      }
      case RSPP_DET_LOOK_ID_2:
      {
         range_type = RSPP_DET_RANGE_TYPE_MEDIUM;
         break;
      }
      case RSPP_DET_LOOK_ID_3:
      {
         range_type = RSPP_DET_RANGE_TYPE_MEDIUM;
         break;
      }
      default:
      {
         range_type = RSPP_DET_RANGE_TYPE_INVALID;
         break;
      }
      }

      return range_type;
   }

   static_assert(1 == sizeof(RSPP_Mounting_Location_T),
                 "sizeof(RSPP_Mounting_Location_T) != 1. Remember to align padding if needed");
   static_assert(1 == sizeof(RSPP_Sensor_Type_T),
                 "sizeof(RSPP_Sensor_Type_T) != 1. Remember to align padding if needed");
   static_assert(1 == sizeof(RSPP_Det_Look_ID_T),
                 "sizeof(RSPP_Det_Look_ID_T) != 1. Remember to align padding if needed");
   static_assert(4 == RSPP_DET_NUM_LOOK_ID,
                 "RSPP_DET_NUM_LOOK_ID != 4. Remember to align padding if needed");

   static_assert(40 == sizeof(F360_Internal_Reflections_Calib_T),
                 "sizeof(F360_Internal_Reflections_Calib_T) not as expected. Remember to align padding if needed");
   static_assert(12 == sizeof(RSPP_VCS_Position_T),
                 "sizeof(RSPP_VCS_Position_T) not as expected. Remember to align padding if needed");
   static_assert(20 == sizeof(RSPP_Sensor_Mounting_Position_T),
                 "sizeof(RSPP_Sensor_Mounting_Position_T) not as expected. Remember to align padding if needed");
   static_assert(8 == sizeof(RSPP_VCS_Velocity_T),
                 "sizeof(RSPP_VCS_Velocity_T) not as expected. Remember to align padding if needed");

   static_assert(200 == sizeof(ConstantProps_T),
                 "sizeof(ConstantProps_T) not as expected. Remember to align padding if needed");
   static_assert(40 == sizeof(VariableProps_T),
                 "sizeof(VariableProps_T) not as expected. Remember to align padding if needed");
   static_assert(52 == sizeof(RefinedProps_T),
                 "sizeof(RefinedProps_T) not as expected. Remember to align padding if needed");
   static_assert(296 == sizeof(F360_Radar_Sensor_T),
                 "sizeof(F360_Radar_Sensor_T) not as expected. Remember to align padding if needed");

} // namespace rspp_variant_A

/** @} doxygen end group */
#endif

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
