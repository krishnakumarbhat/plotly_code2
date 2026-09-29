/*===========================================================================*\
* FILE: f360_det_angle_jump_internals.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Det_Angle_Jump_Supports()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_det_angle_jump_internals.h"
#include "f360_math_func.h"
#include "f360_find_closest_valid_sep_on_given_side.h"

namespace f360_variant_A
{
   inline static SEP_Info Derive_Sep_Info(
      const float32_t closest_sep_lat_pos,
      const float32_t sensor_lateral_pos,
      const float32_t range_tolerance,
      const float32_t sensor_lateral_velocity);

   inline static Close_SEPs Find_Laterally_Closest_Seps(
      const F360_Radar_Sensor_T &sensor,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& calib);

   inline static bool Does_Det_Fit_To_Sep(
      const SEP_Info& sep,
      const float32_t max_abs_range_rate_diff,
      const float32_t det_range,
      const float32_t det_range_rate);

   /*===========================================================================*\
   * FUNCTION: Find_Closest_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * Closest_SEP_Info closest_sep - Structure with ID and lateral position of closest SEP. 
   *                                ID is F360_INVALID_UNSIGNED_ID if no SEP is found.
   *
   * PARAMETERS:
   *  const F360_Radar_Sensor_T & p_sensor                                     - Sensor structure
   *  const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS] - Array containing all SEP's
   *  const float32_t long_search_margin                                      - longitudinal search margin
   *  const float32_t min_abs_lat_pos,                                        - minimum absolute lateral position of SEP
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determine SEP for each sensor. Determination is done by taking the SEP which is
   * closest to host on the same side as the sensor.
   * Center sensors are not considered.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Closest_SEP_Info Find_Closest_SEP(
      const F360_Radar_Sensor_T& sensor,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const float32_t long_search_margin,
      const float32_t min_abs_lat_pos)
   {
      F360_Side_Of_Host_T host_side;

      if ((F360_MOUNTING_LOCATION_LEFT_FORWARD == sensor.constant.mounting_location) ||
         (F360_MOUNTING_LOCATION_LEFT_SIDE1 == sensor.constant.mounting_location) ||
         (F360_MOUNTING_LOCATION_LEFT_SIDE2 == sensor.constant.mounting_location) ||
         (F360_MOUNTING_LOCATION_LEFT_REAR == sensor.constant.mounting_location))
      {
         host_side = F360_HOST_LEFT_SIDE;
      }
      else if ((F360_MOUNTING_LOCATION_RIGHT_FORWARD == sensor.constant.mounting_location) ||
         (F360_MOUNTING_LOCATION_RIGHT_SIDE1 == sensor.constant.mounting_location) ||
         (F360_MOUNTING_LOCATION_RIGHT_SIDE2 == sensor.constant.mounting_location) ||
         (F360_MOUNTING_LOCATION_RIGHT_REAR == sensor.constant.mounting_location))
      {
         host_side = F360_HOST_RIGHT_SIDE;
      }
      else
      {
         // Don't do angle jump check for detections coming from this sensor
         host_side = F360_HOST_INVALID_SIDE;
      }

      const Closest_SEP_Info closest_sep = Find_Closest_SEP_On_Given_Side(static_env_polys, host_side, sensor.constant.mounting_position.vcs_position.longitudinal, long_search_margin, min_abs_lat_pos);

      return closest_sep;
   }

   /*===========================================================================*\
   * FUNCTION: Get_SEP_Info_Per_Sensor()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   *  const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS]
   *  const F360_Calibrations_T& calib
   *  Close_SEPs(&seps_info_per_sensor)[MAX_NUMBER_OF_SENSORS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Gather information about chosen SEP per sensor.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Get_SEP_Info_Per_Sensor(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& calib,
      Close_SEPs(&seps_info_per_sensor)[MAX_NUMBER_OF_SENSORS])
   {
      for (uint32_t sen_idx = 0U; sen_idx < MAX_NUMBER_OF_SENSORS; sen_idx++)
      {
         if (sensors[sen_idx].variable.is_valid)
         {
            seps_info_per_sensor[sen_idx] = Find_Laterally_Closest_Seps(sensors[sen_idx], static_env_polys, calib);
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Find_Laterally_Closest_Seps()
   * ===========================================================================
   * RETURN VALUE:
   * Close_SEPs closest_seps
   *
   * PARAMETERS:
   *  const F360_Radar_Sensor_T &sensor             - sensor
   *  const float32_t sensor_lateral_velocity      - sensor lateral velocity
   *  const Static_Env_Poly_T(&static_env_polys)    - polylines
   *  const float32_t range_tolerance              - range tolerance used for sep min/max range evaluation
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Find the closest seps (i.e. two closest ones which are laterally far enough)
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   inline static Close_SEPs Find_Laterally_Closest_Seps(
      const F360_Radar_Sensor_T &sensor,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& calib)
   {
      Close_SEPs closest_seps = {};
      const Closest_SEP_Info sep = Find_Closest_SEP(sensor, static_env_polys, calib.k_angle_jump_long_search_margin);

      if (F360_INVALID_UNSIGNED_ID != sep.id)
      {
         closest_seps.first_closest = Derive_Sep_Info(sep.lat_pos,
            sensor.constant.mounting_position.vcs_position.lateral,
            calib.k_angle_jump_range_tolerance,
            sensor.variable.vcs_velocity.lateral);

         const float32_t min_lateral_dist_between_two_seps = 0.8F;
         const float32_t min_SEP_lat_pos_abs = std::abs(sep.lat_pos) + min_lateral_dist_between_two_seps;
         const Closest_SEP_Info second_closest_sep_inf = Find_Closest_SEP(sensor, static_env_polys, calib.k_angle_jump_long_search_margin, min_SEP_lat_pos_abs);
         if (F360_INVALID_UNSIGNED_ID != second_closest_sep_inf.id)
         {
            closest_seps.second_closest = Derive_Sep_Info(second_closest_sep_inf.lat_pos,
               sensor.constant.mounting_position.vcs_position.lateral,
               calib.k_angle_jump_range_tolerance,
               sensor.variable.vcs_velocity.lateral);
         }
      }

      return closest_seps;
   }

   /*===========================================================================*\
   * FUNCTION: Derive_Sep_Info()
   * ===========================================================================
   * RETURN VALUE:
   * SEP_Info sep
   *
   * PARAMETERS:
   * const float32_t closest_sep_lat_pos
   * const float32_t sensor_lateral_pos
   * const float32_t range_tolerance
   * const float32_t sensor_lateral_velocity
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Derives sep information based on sensor params and calibs
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   inline static SEP_Info Derive_Sep_Info(
      const float32_t closest_sep_lat_pos,
      const float32_t sensor_lateral_pos,
      const float32_t range_tolerance,
      const float32_t sensor_lateral_velocity)
   {
      SEP_Info sep{};
      const float32_t distance_to_guardrail = std::abs(closest_sep_lat_pos - sensor_lateral_pos);
      const float32_t sign = (closest_sep_lat_pos > 0.0F) ? -1.0F : 1.0F;

      sep.f_valid = true;
      sep.max_range = distance_to_guardrail + range_tolerance;
      sep.min_range = distance_to_guardrail - range_tolerance;
      sep.expected_range_rate = sign * sensor_lateral_velocity;

      return sep;
   }

   /*===========================================================================*\
   * FUNCTION: Detect_Angle_Jump_From_SEP()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const Close_SEPs (&sep_info_per_sensor)[MAX_NUMBER_OF_SENSORS]
   *  const rspp_variant_A::RSPP_Detection_List_T& det_raw
   *  const F360_Calibrations_T& calib
   *  F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Detect angle jumps that can come from stationary barriers. Those detections have similar range
   * as the barrier and range rate close to zero. The root cause of the problem is on a radar side.
   * This function is only a countermeasure that supports tracker. More details can be found in DEX-691
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Detect_Angle_Jump_From_SEP(
      const Close_SEPs(&close_seps_per_sensor)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& det_raw,
      const F360_Calibrations_T& calib,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (uint32_t det_idx = 0U; det_idx < det_raw.number_of_valid_detections; det_idx++)
      {
         const int32_t sensor_idx = det_raw.detections[det_idx].raw.sensor_id - 1;
         const rspp_variant_A::RSPP_Detection_T& single_det_raw = det_raw.detections[det_idx];
         F360_Detection_Props_T& single_det_props = det_props[det_idx];

         single_det_props.f_potential_angle_jump = Is_Det_Angle_Jump(det_raw.detections[det_idx], single_det_props, std::abs(single_det_raw.processed.vcs_az), close_seps_per_sensor[sensor_idx], calib);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_Angle_Jump()
   * ===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   *  const rspp_variant_A::RSPP_Detection_T& det_raw,
   *  const F360_Detection_Props_T& det_prop,
   *  const float32_t abs_det_vcs_az,
   *  const Close_SEPs& sep_sensor_info,
   *  const F360_Calibrations_T& calib
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Evaluate conditions whether a detection is an angle jump from stationary
   * barriers or not.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Det_Angle_Jump(
      const rspp_variant_A::RSPP_Detection_T& det_raw,
      const F360_Detection_Props_T& det_prop,
      const float32_t abs_det_vcs_az,
      const Close_SEPs& closest_seps,
      const F360_Calibrations_T& calib)
   {
      const bool f_common = 
         (det_raw.raw.range < calib.k_angle_jump_max_range) &&
         (det_raw.raw.confid_azimuth != rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH) &&
         (F360_INVALID_UNSIGNED_ID == det_prop.on_sep_id) &&
         (F360_INVALID_UNSIGNED_ID == det_prop.behind_sep_id) &&
         ((abs_det_vcs_az < calib.k_angle_jump_min_abs_azimuth_vcs) || (calib.k_angle_jump_max_abs_azimuth_vcs < abs_det_vcs_az));

      const bool f_angle_jump_from_first_sep = Does_Det_Fit_To_Sep(closest_seps.first_closest, calib.k_angle_jump_max_abs_range_rate, det_raw.raw.range, det_raw.raw.range_rate);
      const bool f_angle_jump_from_second_sep = Does_Det_Fit_To_Sep(closest_seps.second_closest, calib.k_angle_jump_max_abs_range_rate, det_raw.raw.range, det_raw.raw.range_rate);

      return f_common && (f_angle_jump_from_first_sep || f_angle_jump_from_second_sep);
   }

   /*===========================================================================*\
    * FUNCTION: Does_Det_Fit_To_Sep()
    * ===========================================================================
    * RETURN VALUE:
    * bool 
    *
    * PARAMETERS:
    * const SEP_Info& sep_info
    * const float32_t max_abs_range_rate
    * const float32_t det_range
    * const float32_t det_range_rate
    *
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * Checks if detection fits to sep
    *
    * PRECONDITIONS:
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   inline static bool Does_Det_Fit_To_Sep(
      const SEP_Info& sep,
      const float32_t max_abs_range_rate_diff,
      const float32_t det_range,
      const float32_t det_range_rate)
   {
      return sep.f_valid &&
         (det_range < sep.max_range) &&
         (det_range > sep.min_range) &&
         (std::abs(det_range_rate - sep.expected_range_rate) < max_abs_range_rate_diff);
   }
}

