#ifndef F360_RADAR_SENSOR_VARIANT_A_H
   #define F360_RADAR_SENSOR_VARIANT_A_H
   /*===========================================================================*/
   /**
    * @file f360_radar_sensor.h
    *
    * @brief Radar Sensor Structure
    *
    *------------------------------------------------------------------------------
    *
    * Copyright (C) 2026 Aptiv. All rights reserved.
    * Confidential - Restricted Aptiv information. Do not disclose
    *
    *------------------------------------------------------------------------------
    *
    * @section DESC DESCRIPTION:
    * Radar sensor configuration and calibration structure definitions.
    *
    * @section ABBR ABBREVIATIONS:
    *   - F360: F360 Tracker
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
    *
    * @defgroup f360_radar_sensor Radar Sensor
    * @{
    */
   /*===========================================================================*/

   /*===========================================================================*
    * Standard Header Files
    *===========================================================================*/
   #include "f360_reuse.h"

   /*===========================================================================*
    * Other Header Files
    *===========================================================================*/
   #include "f360_look_ID.h"
   #include "f360_look_type.h"
   #include "f360_mounting_location.h"
   #include "f360_range_type.h"
   #include "f360_sensor_mounting_position.h"
   #include "f360_sensor_type.h"
   #include "f360_velocity.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

/*===========================================================================*
 * Namespace Definition
 *===========================================================================*/
namespace f360_variant_A
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
 * Name:  F360_Radar_Sensor_T
 *  Structure for radar sensor properties.
 *  Members are ordered by descending alignment size (8, 4, 2, 1 byte) so that
 *  the compiler inserts no implicit internal padding. Only 7 bytes of explicit
 *  trailing padding are required to satisfy the struct's 8-byte alignment.
 *
 * Struct Members:
 *   timestamp_us                - [us]   Time stamp of the current radar scan in microseconds.
 *   vcs_velocity                -        Velocity of the sensor in VCS.
 *   internal_reflections        -        Enable/disable & tuning parameters for internal reflections identification.
 *   mounting_position           -        Mounting position data for the sensor.
 *   polarity                    -        1 = normal, -1 = flipped.
 *   vacs_boresight_az_estimated - [rad]  Estimated boresight azimuth angle in VCS, compensated for alignment error.
 *   vacs_boresight_el_estimated - [rad]  Estimated boresight elevation angle in VCS, compensated for alignment error.
 *   range_limits                - [m]    Range limits for each look index variant.
 *   fov_min_az_rad              - [rad]  Minimum azimuth field of view for each look index variant.
 *   fov_max_az_rad              - [rad]  Maximum azimuth field of view for each look index variant.
 *   fov_min_el_rad              - [rad]  Minimum elevation field of view for each look index variant.
 *   fov_max_el_rad              - [rad]  Maximum elevation field of view for each look index variant.
 *   min_aliaised_range_rate     - [m/s]  Minimum aliased range rate for each look index variant.
 *   v_wrapping                  - [m/s]  Range rate dealiasing interval.
 *   r_wrapping                  - [m]    Range offset after doppler unfolding; 0 for FMCW, nonzero for SFW.
 *   id                          -        ID of the sensor.
 *   number_of_valid_detections  -        Number of valid detections from this sensor in the current scan.
 *   overall_rain_level          -        Rain level; 0 - no rain; 1 - low rain; 2 - high rain.
 *   look_index                  -        Look index for the current detection.
 *   look_id                     -        Look ID for the current detection.
 *   mounting_location           -        Tag describing the location of the sensor on the host vehicle.
 *   sensor_type                 -        Tag describing the type of sensor.
 *   is_valid                    -        Indicates if the sensor is used/enabled.
 *   f_sensor_fault_detected     -        Indicates if a sensor fault is detected (only used in a specific project).
 *   padding                     -        Explicit trailing padding; no implicit padding exists in this struct.
 ******************************************************************************/
typedef struct F360_Radar_Sensor_Tag
{
   uint64_t timestamp_us;
   F360_VCS_Velocity_T vcs_velocity;
   F360_Internal_Reflections_Calib_T internal_reflections;
   F360_Sensor_Mounting_Position_T mounting_position;
   int32_t polarity;
   float32_t vacs_boresight_az_estimated;
   float32_t vacs_boresight_el_estimated;
   float32_t range_limits[F360_DET_NUM_LOOK_ID];
   float32_t fov_min_az_rad[F360_DET_NUM_LOOK_ID];
   float32_t fov_max_az_rad[F360_DET_NUM_LOOK_ID];
   float32_t fov_min_el_rad[F360_DET_NUM_LOOK_ID];
   float32_t fov_max_el_rad[F360_DET_NUM_LOOK_ID];
   float32_t min_aliaised_range_rate[F360_DET_NUM_LOOK_ID];
   float32_t v_wrapping[F360_DET_NUM_LOOK_ID];
   float32_t r_wrapping[F360_DET_NUM_LOOK_ID];
   uint32_t id;
   uint32_t number_of_valid_detections;
   uint16_t overall_rain_level;
   uint16_t look_index;
   F360_Det_Look_ID_T look_id;
   F360_Mounting_Location_T mounting_location;
   F360_Sensor_Type_T sensor_type;
   bool is_valid;
   bool f_sensor_fault_detected;
   uint8_t padding[7];
} F360_Radar_Sensor_T;

/*===========================================================================*
 * Exported Class Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Declarations
 *===========================================================================*/

/******************************************************************************
 * Name:  Get_Look_Type
 *   Function to map Look ID to Look Type
 *
 * Shared Variables: None
 *
 * Parameters:
 *   look_ID - Input Look ID
 *
 * Return Value:
 *   Corresponding Look Type
 *
 * Design Information: None
 *
 * Change References: None
 *
 ******************************************************************************/
inline F360_Det_Look_Type_T Get_Look_Type(const F360_Det_Look_ID_T look_ID)
{
   F360_Det_Look_Type_T look_type;
   switch (look_ID)
   {
      case F360_DET_LOOK_ID_0:
      {
         look_type = F360_DET_LOOK_TYPE_LONG;
         break;
      }
      case F360_DET_LOOK_ID_1:
      {
         look_type = F360_DET_LOOK_TYPE_MEDIUM;
         break;
      }
      case F360_DET_LOOK_ID_2:
      {
         look_type = F360_DET_LOOK_TYPE_LONG;
         break;
      }
      case F360_DET_LOOK_ID_3:
      {
         look_type = F360_DET_LOOK_TYPE_MEDIUM;
         break;
      }
      default:
      {
         look_type = F360_DET_LOOK_TYPE_INVALID;
         break;
      }
   }

   return look_type;
}

/******************************************************************************
 * Name:  Get_Range_Type
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
inline F360_Det_Range_Type_T Get_Range_Type(const F360_Det_Look_ID_T look_ID)
{
   F360_Det_Range_Type_T range_type;
   switch (look_ID)
   {
      case F360_DET_LOOK_ID_0:
      {
         range_type = F360_DET_RANGE_TYPE_LONG;
         break;
      }
      case F360_DET_LOOK_ID_1:
      {
         range_type = F360_DET_RANGE_TYPE_LONG;
         break;
      }
      case F360_DET_LOOK_ID_2:
      {
         range_type = F360_DET_RANGE_TYPE_MEDIUM;
         break;
      }
      case F360_DET_LOOK_ID_3:
      {
         range_type = F360_DET_RANGE_TYPE_MEDIUM;
         break;
      }
      default:
      {
         range_type = F360_DET_RANGE_TYPE_INVALID;
         break;
      }
   }

   return range_type;
}

static_assert(1 == sizeof(F360_Mounting_Location_T), "sizeof(F360_Mounting_Location_T) != 1. Remember to align padding if needed");
static_assert(1 == sizeof(F360_Sensor_Type_T), "sizeof(F360_Sensor_Type_T) != 1. Remember to align padding if needed");
static_assert(1 == sizeof(F360_Det_Look_ID_T), "sizeof(F360_Det_Look_ID_T) != 1. Remember to align padding if needed");
static_assert(4 == F360_DET_NUM_LOOK_ID, "F360_DET_NUM_LOOK_ID != 4. Remember to align padding if needed");

static_assert(40 == sizeof(F360_Internal_Reflections_Calib_T),
              "sizeof(F360_Internal_Reflections_Calib_T) not as expected. Remember to align padding if needed");
static_assert(12 == sizeof(F360_VCS_Position_T),
              "sizeof(F360_VCS_Position_T) not as expected. Remember to align padding if needed");
static_assert(20 == sizeof(F360_Sensor_Mounting_Position_T),
              "sizeof(F360_Sensor_Mounting_Position_T) not as expected. Remember to align padding if needed");
static_assert(8 == sizeof(F360_VCS_Velocity_T),
              "sizeof(F360_VCS_Velocity_T) not as expected. Remember to align padding if needed");

static_assert(
   240 == sizeof(F360_Radar_Sensor_T),
   "sizeof(F360_Radar_Sensor_T) not as expected. Members must stay ordered by descending alignment to avoid implicit padding");

} // namespace f360_variant_A

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
