/*===========================================================================*\
* FILE: f360_detection_double_bounce_detector.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains definitions of functions used to determine if detection is double bounced.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_detection_double_bounce_detector.h"
#include "f360_math_func.h"
#include "f360_try_to_dealiase_range_rate.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Double_Bounce_Detection_In_Limits()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T &det_primary
   * const rspp_variant_A::RSPP_Detection_T &det_secondary
   * const F360_Radar_Sensor_T &sensor
   * const F360_Calibrations_T_T &calibs
   * const uint32_t nr_multi_bounce
   * const float32_t k_max_az_diff
   * F360_Detection_Props_T &det_secondary
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *
   * --------------------------------------------------------------------------
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static bool Double_Bounce_Detection_In_Limits(
      const rspp_variant_A::RSPP_Detection_T& det_primary,
      const rspp_variant_A::RSPP_Detection_T& det_secondary,
      const F360_Radar_Sensor_T& sensor,
      const F360_Calibrations_T& calibs,
      const uint32_t nr_multi_bounce,
      const float32_t k_max_az_diff)
   {
      bool f_double_bounce = false;
      if ((nr_multi_bounce <= calibs.k_db_max_nr_multi_bounces) && (nr_multi_bounce > 1U))
      {
         const float32_t predicted_range = det_primary.raw.range * static_cast<float32_t>(nr_multi_bounce);
         float32_t rng_tolerance = calibs.k_db_range_threshold_frac * det_secondary.raw.range;
         rng_tolerance = F360_Saturate(rng_tolerance, calibs.k_db_min_range_threshold, calibs.k_db_max_range_threshold);

         if (std::abs(predicted_range - det_secondary.raw.range) < rng_tolerance)
         {
            // Detection looks like double bounce based on azimuth and range, now check range rate that may be aliased
            const float32_t predicted_rangerate = det_primary.raw.range_rate * static_cast<float32_t>(nr_multi_bounce);

            float32_t dealiased_rngrate = 0.0F;
            float32_t zero_interval = 0.0F;
            const bool f_inside_gate = Try_To_Dealiase_Range_Rate(
               det_secondary.raw.range_rate,
               predicted_rangerate,
               calibs.k_db_range_rate_threshold,
               sensor.constant.v_wrapping[sensor.variable.look_id],
               sensor.constant.min_aliaised_range_rate[sensor.variable.look_id],
               dealiased_rngrate,
               zero_interval);

            if (f_inside_gate)
            {
               // We have passed the range rate gate for double bounce and also
               // dealiased the range rate so mark the detection as double bounce
               f_double_bounce = true;
            }
            else
            {
               // A multi bounce between the ego vehicle and a surface parallel to the ego side is resulting in a zero range rate
               // (under the precondition that the distance between the two surfaces stays the same, no lateral movement)
               const float32_t vcs_azimuth_primary = std::abs(det_primary.processed.vcs_az);

               if ((vcs_azimuth_primary > (F360_PI_2 - k_max_az_diff)) &&
                  (vcs_azimuth_primary < (F360_PI_2 + k_max_az_diff)) &&
                  (std::abs(det_primary.raw.range_rate) < calibs.k_db_range_rate_threshold))
               {
                  // Primary detection is in cone of silence, now check characteristics of secondary detection
                  if ((std::abs(det_secondary.raw.range_rate) < calibs.k_db_range_rate_threshold) &&
                     (std::abs(vcs_azimuth_primary - std::abs(det_secondary.processed.vcs_az)) < k_max_az_diff))
                  {
                     // Secondary detection fit characteristic of double bounce in the cone of silence
                     f_double_bounce = true;
                  }
               }
            }
         }
      }

      return f_double_bounce;
   }

   /*===========================================================================*\
    * FUNCTION: Double_Bounce_Detection_Countermeasure()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const rspp_variant_A::RSPP_Detection_T (&dets)[MAX_NUMBER_OF_DETECTIONS],
    * F360_Detection_Props_T (&dets)[MAX_NUMBER_OF_DETECTIONS],
    * const uint32_t num_dets,
    * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
    * const F360_Globals_T &globals,
    * const F360_Calibrations_T &calibs,
    * F360_TRKR_TIMING_INFO_T &timing_info
    *
    * EXTERNAL REFERENCES:
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * Search for characteristics that indicates one or several bounces between host and target.
    * Flag detections that appear to be false due to multiple reflections.
    * --------------------------------------------------------------------------
    *
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    \*===========================================================================*/
   void Double_Bounce_Detection_Countermeasure(
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_T(&dets)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t num_dets,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibs)
   {
      // Divide relevant detections to their parent sensor
      uint32_t ndets[MAX_NUMBER_OF_SENSORS] = {};
      float32_t det_az_array[MAX_NUMBER_OF_SENSORS][MAX_DETS_FOR_SINGLE_SENSOR];
      int32_t det_idx_array[MAX_NUMBER_OF_SENSORS][MAX_DETS_FOR_SINGLE_SENSOR];
      for (int32_t det_idx = 0; det_idx < static_cast<int32_t>(num_dets); det_idx++)
      {
         if ((detection_props[det_idx].f_ok_to_use) &&
            (dets[det_idx].raw.range < calibs.k_db_max_range) &&
            (!dets[det_idx].raw.f_bistatic))
         {
            const int32_t sens_idx = dets[det_idx].raw.sensor_id - 1;
            uint32_t& number_of_dets_curr_sensor = ndets[sens_idx];
            det_az_array[sens_idx][number_of_dets_curr_sensor] = dets[det_idx].raw.azimuth * static_cast<float32_t>(sensors[sens_idx].constant.polarity);
            det_idx_array[sens_idx][number_of_dets_curr_sensor] = det_idx;
            number_of_dets_curr_sensor++;
         }
      }

      // Detection based multi bounce algo
      for (uint32_t sens_idx = 0U; sens_idx < MAX_NUMBER_OF_SENSORS; sens_idx++)
      {
         if (ndets[sens_idx] > 1U)
         {
            // Sort detections from this sensor on azimuth
            uint32_t az_sorted_idx[MAX_DETS_FOR_SINGLE_SENSOR]{};
            (void)F360_Sort(ndets[sens_idx], true, det_az_array[sens_idx], az_sorted_idx);

            for (uint32_t i = 0U; i < ndets[sens_idx] - 1U; i++)
            {
               for (uint32_t j = i + 1U; j < ndets[sens_idx]; j++)
               {
                  const int32_t det_idx1 = det_idx_array[sens_idx][az_sorted_idx[i]];
                  const int32_t det_idx2 = det_idx_array[sens_idx][az_sorted_idx[j]];

                  // Detection with shortest range is our reference detection (primary)
                  struct det_t { int32_t primary; int32_t secondary; } det;

                  if (dets[det_idx1].raw.range < dets[det_idx2].raw.range)
                  {
                     det.primary = det_idx1;
                     det.secondary = det_idx2;
                  }
                  else
                  {
                     det.primary = det_idx2;
                     det.secondary = det_idx1;
                  }

                  // Determine azimuth threshold based on range of primary detection
                  float32_t k_max_az_diff = calibs.k_db_azimuth_thres_k * dets[det.primary].raw.range + calibs.k_db_azimuth_thres_m;
                  k_max_az_diff = F360_Saturate(k_max_az_diff, calibs.k_db_min_azimuth_thres, calibs.k_db_max_azimuth_thres);

                  if ((dets[det_idx2].raw.azimuth - dets[det_idx1].raw.azimuth) * static_cast<float32_t>(sensors[sens_idx].constant.polarity) > k_max_az_diff)
                  {
                     // Azimuth diff is too large, detection pair can't be multi bounce
                     break;
                  }
                  // Variable to decide to run double bounce logic or not
                  uint32_t nr_multi_bounce;
                  // Preventing possible division with 0
                  if ((dets[det.primary].raw.range > 0.0F) && (dets[det.primary].raw.range < (0.5F * sensors[sens_idx].constant.range_limits[sensors[sens_idx].variable.look_id])))
                  {
                     // A rounded ratio of test and reference detection gives the estimated number of bounces
                     float32_t const dets_ratio = dets[det.secondary].raw.range / dets[det.primary].raw.range + 0.5F;
                     nr_multi_bounce = static_cast<uint32_t>(dets_ratio);
                  }
                  else
                  {
                     nr_multi_bounce = 0U;
                  }

                  // Continue running algo for this iteration only if double bounce is possible and number of bounces is within limits
                  const bool f_double_bounce = Double_Bounce_Detection_In_Limits(dets[det.primary], dets[det.secondary], sensors[sens_idx], calibs, nr_multi_bounce, k_max_az_diff);

                  if (f_double_bounce)
                  {
                     detection_props[det.secondary].f_ok_to_use = false;
                     detection_props[det.secondary].f_double_bounce = true;
                  }
               }
            }
         }
      }
   }
}
