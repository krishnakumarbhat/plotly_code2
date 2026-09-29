/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_cluster_detection_downselection.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definitions of:
* Downselect_Detections()
* Determine_Positions_In_Set_Of_Dets_To_Clear()
* Count_Unique_Tsm_And_Update_Num_Of_Dets_For_Each() 
* 
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_cluster_detection_downselection.h"
#include "f360_math_func.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Downselect_Detections()
   *===========================================================================
   *
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const uint16_t& max_dets_to_downselect,
   * Detections_Set& set_of_detections,
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   *
   * ABSTRACT:
   * This function performs downselection of detections, so that number of detections would not exceed max limit.
   * Dowselection is based on time since measurements and is designed to approximate uniform distribution as much
   * as possible, while utilizing all available space for detections.
   * 
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Downselect_Detections(
      const uint16_t& max_dets_to_downselect,
      Detections_Set& set_of_detections)
   {
      if (set_of_detections.num_dets <= max_dets_to_downselect)
      {
         // Fewer than max detections -> downselect all of them
         for (uint16_t j = 0U; j < set_of_detections.num_dets; j++)
         {
            set_of_detections.f_downselected[j] = true;
         }
      }
      else
      {
         // More than max detections -> choose a subset

         /* Compute how many unique "time since measurements" there are in the dataset and how many detections
         * there are for each unique time since measurement. This is done by sorting the detections according
         * to increasing "time since measurement" and then looping over them in sorted order. */
         float32_t sorted_time_since_meas[WORST_CASE_NUM_DETS_IN_CLUSTER];
         (void)std::copy(cmn::begin(set_of_detections.time_since_meas), cmn::end(set_of_detections.time_since_meas), cmn::begin(sorted_time_since_meas));
         uint32_t perm[WORST_CASE_NUM_DETS_IN_CLUSTER];
         (void)F360_Sort(static_cast<uint32_t>(set_of_detections.num_dets), true, sorted_time_since_meas, perm);

         Unique_Time_Detection_Data detections_unique_time_data{};
         Count_Unique_Tsm_And_Update_Num_Of_Dets_For_Each(sorted_time_since_meas, set_of_detections.num_dets, detections_unique_time_data);

         /* Loop over all unique timestamps over and over again and pick one new detection from each
         * of them until we have picked max_dets_to_downselect. (In case all detections from a timestamp
         * are already picked we skip this timestamp).
         * In the worst case we only pick one detection from one unique timestamp in each loop
         * and then we have to iterate max_dets_to_downselect times. Although most often we pick more
         * detections per loop and then the loop will be ended prematurely as soon as we have picked
         * max_dets_to_downselect detections. */
         bool picked_max_available_dets = false;
         uint16_t num_downselected_dets = 0U;
         for (uint16_t i = 0U; (i < max_dets_to_downselect) && (!picked_max_available_dets); i++)
         {
            uint16_t curr_tsm_first_det_i = 0U; // Reset
            // Loop over all unique timestamps starting from youngest to oldest
            for (uint16_t uniqe_ts_i = 0U; (uniqe_ts_i < detections_unique_time_data.num_unique_tsm) && (!picked_max_available_dets); uniqe_ts_i++)
            {
               uint16_t& cnt_curr_tsm_ds_dets = detections_unique_time_data.num_downselected_dets_per_unique_tsm[uniqe_ts_i];
               if (cnt_curr_tsm_ds_dets < detections_unique_time_data.num_dets_per_unique_tsm[uniqe_ts_i])
               {
                  // There are detections left from this timestamp that has not been downselected. Pick one of them to downselect
                  const uint16_t det_i = curr_tsm_first_det_i + cnt_curr_tsm_ds_dets;
                  const uint32_t detection_set_idx = perm[det_i];
                  set_of_detections.f_downselected[detection_set_idx] = true;
                  cnt_curr_tsm_ds_dets++;

                  num_downselected_dets++;
                  if (num_downselected_dets >= max_dets_to_downselect)
                  {
                     picked_max_available_dets = true;
                  }
               }
               curr_tsm_first_det_i += detections_unique_time_data.num_dets_per_unique_tsm[uniqe_ts_i];
            }
         }
      }
   }
   
   /*===========================================================================*\
   * FUNCTION: Count_Unique_Tsm_And_Update_Num_Of_Dets_For_Each()
   *===========================================================================
   *
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t (&timestamp_array)[WORST_CASE_NUM_DETS_IN_CLUSTER]
   * const uint16_t num_dets
   * Unique_Time_Detection_Data& unique_time_det_data
   *
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   *
   * ABSTRACT:
   * This function counts unique timestamps and updates number of detections for each.
   *
   *
   * PRECONDITIONS:
   * The timestamp array is in sorted order
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Count_Unique_Tsm_And_Update_Num_Of_Dets_For_Each(
      const float32_t (&timestamp_array)[WORST_CASE_NUM_DETS_IN_CLUSTER],
      const uint16_t num_dets,
      Unique_Time_Detection_Data& unique_time_det_data)
   {
      // Initialize the counters with help of first detection and then loop over all other detections.
      unique_time_det_data.num_dets_per_unique_tsm[0] = 1U;   // Initialize with first detection
      unique_time_det_data.num_unique_tsm = 1U;       // Initialize with first detection 

      for (uint16_t det_i = 1U; det_i < num_dets; det_i++) // Loop over remaining detections (i.e. start loop from 1)
      {
         if ((timestamp_array[det_i] - timestamp_array[det_i - 1U]) < F360_EPSILON)
         {
            // Same time as previously has been found
            const uint16_t idx = unique_time_det_data.num_unique_tsm - 1U;
            unique_time_det_data.num_dets_per_unique_tsm[idx] += 1U;
         }
         else
         {
            // New unique time has been found
            unique_time_det_data.num_dets_per_unique_tsm[unique_time_det_data.num_unique_tsm] = 1U;
            unique_time_det_data.num_unique_tsm += 1U;
         }
      }
   }
}
