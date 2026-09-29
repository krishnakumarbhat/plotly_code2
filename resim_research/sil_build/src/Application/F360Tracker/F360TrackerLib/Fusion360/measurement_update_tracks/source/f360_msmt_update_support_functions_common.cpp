/*===================================================================================*\
* FILE: f360_msmt_update_support_functions_common.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains common functions for CCA and CTCA methods
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include <algorithm>
#include "f360_msmt_update_support_functions_common.h"
#include "f360_math.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Select_Dets_For_RR_Update()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS]
   * const uint32_t& k_min_num_selected_dets_per_sensor_for_binning
   * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Object_Track_T& object_track
   * uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK]
   * uint32_t& selected_dets_num
   * uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function updates list of detections that are valid to be used in the
   * range-rate measurement update. It also calculates how many detections there
   * are in each bin.
   \*===========================================================================*/
   void Select_Dets_For_RR_Update(
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t& k_min_num_selected_dets_per_sensor_for_binning,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object_track,
      uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& selected_dets_num,
      uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT])
   {
      selected_dets_num = 0U;

      // Loop over associated detections and select valid ones for measurement update.
      for (uint32_t i_det = 0U; i_det < object_track.ndets; i_det++)
      {
         const uint32_t det_idx = object_track.detids[i_det] - 1U;

         if (det_props[det_idx].f_rr_inlier)
         {
            selected_dets_idx[selected_dets_num] = det_idx;
            selected_dets_num++;
         }
      }

      // Rough initial check if object is eligible for detection binning to reduce selected_dets_num
      if ((object_track.movable_prob > 0.5F) && (selected_dets_num > k_min_num_selected_dets_per_sensor_for_binning))
      {
         Try_To_Decrease_Num_Selected_Dets(detections, det_props, k_min_num_selected_dets_per_sensor_for_binning, selected_dets_idx, selected_dets_num, selected_dets_score);
      }

      // Logkeeping of which dets were selected for the measurement update
      object_track.num_dets_used_in_rr_msmt_update = selected_dets_num;
      for (uint32_t i_det = 0U; i_det < selected_dets_num; i_det++)
      {
         const uint32_t det_idx = selected_dets_idx[i_det];
         det_props[det_idx].f_used_in_rr_msmt_update = true;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Try_To_Decrease_Num_Selected_Dets()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const int32_t& k_min_num_selected_dets_per_sensor_for_binning
   * uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK]
   * uint32_t& selected_dets_num
   * uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Binning is done over detection azimuth only (not range or position). The reason for
   * this is that the measured range rate from a point on an object is correlated only with
   * detection azimuth and not range (even for a yawing object for which different points
   * of the object has different velocities). This is because radar can only measure the radial
   * component of the velocity so the cross radial velocity differenece due to yawing can not be measured.
   \*===========================================================================*/
   void Try_To_Decrease_Num_Selected_Dets(
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t& k_min_num_selected_dets_per_sensor_for_binning,
      uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& selected_dets_num,
      uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT])
   {
      uint32_t selected_dets_idx_final[MAX_DETS_IN_OBJ_TRK];
      uint32_t selected_dets_num_final = 0U;

      float32_t max_det_az_per_sensor[MAX_NUMBER_OF_SENSORS];
      float32_t min_det_az_per_sensor[MAX_NUMBER_OF_SENSORS];
      uint32_t det_idx_per_sensor[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK];
      uint32_t num_det_per_sensor[MAX_NUMBER_OF_SENSORS];
      Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);

      for (uint8_t i_sensor = 0U; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
      {
         if (num_det_per_sensor[i_sensor] > k_min_num_selected_dets_per_sensor_for_binning)
         {
            // Enough detections from this sensor -> can bin detections
            uint32_t bin_array_det_idx[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK];
            uint32_t bin_array_num_det_in_bin[MSMT_UPDATE_NUM_BINS_PER_SENSOR];
            Bin_Detections_By_Azimuth(detections, det_idx_per_sensor[i_sensor], num_det_per_sensor[i_sensor], min_det_az_per_sensor[i_sensor], max_det_az_per_sensor[i_sensor], bin_array_det_idx, bin_array_num_det_in_bin);

            // Compute representative detection for each bin and add to the selected dets
            Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin, selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
         }
         else
         {
            // Not enough detections from this sensor -> keep these detections in selected dets
            for (uint32_t i_det = 0U; i_det < num_det_per_sensor[i_sensor]; i_det++)
            {
               selected_dets_idx_final[selected_dets_num_final] = det_idx_per_sensor[i_sensor][i_det];
               selected_dets_num_final++;
            }
         }
      }
      // Save final detections to output structure
      selected_dets_num = selected_dets_num_final;
      (void)std::copy(cmn::begin(selected_dets_idx_final), cmn::end(selected_dets_idx_final), cmn::begin(selected_dets_idx));
   }

   /*===========================================================================*\
    * FUNCTION: Extract_Detection_Properties_Per_Sensor
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
    * const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
    * const uint32_t& selected_dets_num,
    * float32_t(&min_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
    * float32_t(&max_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
    * uint32_t(&det_idx_per_sensor)[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK],
    * uint8_t(&num_det_per_sensor)[MAX_NUMBER_OF_SENSORS])
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * For the selected detections, separates these based on their sensor id and
    * extracts the min and max azimuth per sensor.
   \*===========================================================================*/
   void Extract_Detection_Properties_Per_Sensor(
      const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t& selected_dets_num,
      float32_t(&min_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
      float32_t(&max_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
      uint32_t(&det_idx_per_sensor)[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK],
      uint32_t(&num_det_per_sensor)[MAX_NUMBER_OF_SENSORS])
   {
      std::fill(cmn::begin(num_det_per_sensor), cmn::end(num_det_per_sensor), 0U);
      std::fill(cmn::begin(max_det_az_per_sensor), cmn::end(max_det_az_per_sensor), -F360_2PI);
      std::fill(cmn::begin(min_det_az_per_sensor), cmn::end(min_det_az_per_sensor), F360_2PI);

      for (uint32_t i = 0U; i < selected_dets_num; i++)
      {
         const uint32_t det_idx = selected_dets_idx[i];
         const int32_t sensor_idx = detections[det_idx].raw.sensor_id - 1;

         if (detections[det_idx].raw.azimuth > max_det_az_per_sensor[sensor_idx])
         {
            max_det_az_per_sensor[sensor_idx] = detections[det_idx].raw.azimuth;
         }
         if (detections[det_idx].raw.azimuth < min_det_az_per_sensor[sensor_idx])
         {
            min_det_az_per_sensor[sensor_idx] = detections[det_idx].raw.azimuth;
         }

         det_idx_per_sensor[sensor_idx][num_det_per_sensor[sensor_idx]] = det_idx;
         num_det_per_sensor[sensor_idx]++;
      }
   }

   /*===========================================================================*\
    * FUNCTION: Bin_Detections_By_Azimuth
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
    * const uint32_t(&det_idx_for_sensor)[MAX_DETS_IN_OBJ_TRK],
    * const uint8_t num_det_for_sensor,
    * const float32_t min_az_for_sensor,
    * const float32_t max_az_for_sensor,
    * uint32_t(&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
    * uint32_t(&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR])
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * Binns detections by azimuth using a set number of bins with a varying bin
    * width that is determined by the delta between the min and max azimuth.
    *
    * PRECONDITIONS:
    * min_az_for_sensor must be < max_az_for_sensor
    *
   \*===========================================================================*/
   void Bin_Detections_By_Azimuth(
      const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&det_idx_for_sensor)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t num_det_for_sensor,
      const float32_t min_az_for_sensor,
      const float32_t max_az_for_sensor,
      uint32_t(&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
      uint32_t(&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR])
   {
      std::fill(cmn::begin(bin_array_num_det_in_bin), cmn::end(bin_array_num_det_in_bin), 0U);

      const float32_t azimuth_spread = max_az_for_sensor - min_az_for_sensor;
      constexpr float32_t INV_MSMT_UPDATE_NUM_BINS_PER_SENSOR = 1.0F / static_cast<float32_t>(MSMT_UPDATE_NUM_BINS_PER_SENSOR);
      const float32_t bin_azimuth_width = std::max(azimuth_spread * INV_MSMT_UPDATE_NUM_BINS_PER_SENSOR, azimuth_spread * INV_MSMT_UPDATE_NUM_BINS_PER_SENSOR + F360_EPSILON); // 0-division protection

      for (uint32_t i_det = 0U; i_det < num_det_for_sensor; i_det++)
      {
         const uint32_t det_idx = det_idx_for_sensor[i_det];
         const float32_t det_azimuth = detections[det_idx].raw.azimuth;

         // Compute in which bin the detection fit
         const float32_t bin_idx_float = (det_azimuth - min_az_for_sensor) / (bin_azimuth_width);

         // To account for precision errors in a safe way when det_azimuth = min_az or max_az.
         // Note that the static_cast is intentionally intended to also have a truncating effect similar to a floorf() function.
         // Also note that bin_idx_clamped is intentionally signed such that if bin_idx_float is -0.00.., it won't overflow
         const int32_t bin_idx_clamped = Clamp(static_cast<int32_t>(bin_idx_float), 0, MSMT_UPDATE_NUM_BINS_PER_SENSOR-1);

         // Assign detection to bin
         bin_array_det_idx[bin_idx_clamped][bin_array_num_det_in_bin[bin_idx_clamped]] = det_idx_for_sensor[i_det];
         bin_array_num_det_in_bin[bin_idx_clamped]++;
      }
   }

   /*===========================================================================*\
    * FUNCTION: Select_Median_Range_Rate_Detection_For_Each_Bin
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
    * const uint32_t(&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
    * const uint32_t(&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR],
    * uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
    * uint32_t& selected_dets_num)
    * uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function picks the detection with median range rate for each bin and gives the score
    * that is based on the amount of detections in each bin. If a bin slot is empty,
    * nothing happens. The selected detection has the median value of range rate to minimize
    * the impact of the noise.
   \*===========================================================================*/
   void Select_Median_Range_Rate_Detection_For_Each_Bin(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
      const uint32_t(&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR],
      uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& selected_dets_num,
      uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT])
   {
      for (int8_t i_bin = 0; i_bin < MSMT_UPDATE_NUM_BINS_PER_SENSOR; i_bin++)
      {
         const uint32_t num_dets_in_bin = bin_array_num_det_in_bin[i_bin];

         if (num_dets_in_bin > 0U)
         {
            // Pick detection with median range rate
            uint32_t median_det_idx = 0U;
            if (num_dets_in_bin == 1U)
            {
               // Choose this single detection
               median_det_idx = bin_array_det_idx[i_bin][0];
            }
            else
            {
               // Extract range rate for detections
               float32_t det_rr_array[MAX_DETS_IN_OBJ_TRK];
               for (uint32_t i_det = 0U; i_det < num_dets_in_bin; i_det++)
               {
                  det_rr_array[i_det] = det_props[bin_array_det_idx[i_bin][i_det]].range_rate_compensated;
               }

               uint32_t perm[MAX_DETS_IN_OBJ_TRK];
               (void)F360_Sort(num_dets_in_bin, true, det_rr_array, perm); // det_rr_array is now sorted in ascending order

               const uint32_t median_idx = (num_dets_in_bin - 1U) / 2U;
               median_det_idx = bin_array_det_idx[i_bin][perm[median_idx]];
            }
            selected_dets_idx[selected_dets_num] = median_det_idx;
            selected_dets_score[selected_dets_num] = bin_array_num_det_in_bin[i_bin];
            selected_dets_num++;
         }
      }
   }

}
