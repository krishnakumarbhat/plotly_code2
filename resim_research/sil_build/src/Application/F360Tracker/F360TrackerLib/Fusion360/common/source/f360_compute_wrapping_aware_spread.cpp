/*===========================================================================*\
* FILE: f360_compute_wrapping_aware_spread.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
*
* DESCRIPTION:
*   Contains function definition of Compute_Wrapping_Aware_Spread()
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_compute_wrapping_aware_spread.h"
#include "f360_math.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Compute_Wrapping_Aware_Spread()
   *===========================================================================
   * RETURN VALUE:
   * float32_t - Doppler spread between two raw range rates [m/s]
   *
   * PARAMETERS:
   * const float32_t rng_rate1 - raw range rate of first detection [m/s]
   * const float32_t rng_rate2 - raw range rate of second detection [m/s]
   * const rspp_variant_A::RSPP_Detection_T &det1 - first raw detection
   * const rspp_variant_A::RSPP_Detection_T &det2 - second raw detection
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - sensor array
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
   * Computes the minimum Doppler spread between two raw range rates, accounting for
   * range rate wrapping when both detections originate from the same sensor.
   * For same-sensor pairs the raw range rates share a common wrapping interval
   * and fold index.
   *
   * PRECONDITIONS:
   * v_wrapping of the sensor is assumed to be greater than
   * 2 * k_min_wheel_spin_doppler_spread.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   float32_t Compute_Wrapping_Aware_Spread(
      const float32_t rng_rate1,
      const float32_t rng_rate2,
      const rspp_variant_A::RSPP_Detection_T &det1,
      const rspp_variant_A::RSPP_Detection_T &det2,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS])
   {
      float32_t spread = std::abs(rng_rate1 - rng_rate2);
      
      const int32_t sensor_idx1 = det1.raw.sensor_id - 1;
      const int32_t sensor_idx2 = det2.raw.sensor_id - 1;

      // Currently we apply wrapping correction only for detections from the same sensor. Extending this two cross-sensor comparison is possible, at cost of increased complexity. 
      if (sensor_idx1 == sensor_idx2)
      {
         const float32_t v_wrap = sensors[sensor_idx1].constant.v_wrapping[sensors[sensor_idx1].variable.look_id];

         // Both detections should be from the same sensor and look ID, so raw_spread < v_wrap. Guard against input error
         if (spread < v_wrap)
         {
            spread = std::min(spread, v_wrap - spread);
         }
      }

      return spread;
   }

   /*===========================================================================*\
   * FUNCTION: F360_Compensate_Det_Range()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const float32_t dealiasing_interval,
   *  const float32_t r_wrapping,
   *  const rspp_variant_A::RSPP_Detection_T &detection,
   *  F360_Detection_Props_T &detection_prop
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
   * This function updates detection's range_dealiased and vcs position after
   * range rate dealiasing (SFW)
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void F360_Compensate_Det_Range(
      const float32_t dealiasing_interval,
      const float32_t r_wrapping,
      const rspp_variant_A::RSPP_Detection_T &detection,
      F360_Detection_Props_T &detection_prop)
   {
      const float32_t delta_range = r_wrapping * dealiasing_interval;
      const Point cur_vcs_position = detection_prop.vcs_position;
      detection_prop.range_dealiased = detection.raw.range + delta_range;
      detection_prop.vcs_position.Set_Position(
         cur_vcs_position.x + delta_range * detection.processed.cos_vcs_az,
         cur_vcs_position.y + delta_range * detection.processed.sin_vcs_az);
   }
}
