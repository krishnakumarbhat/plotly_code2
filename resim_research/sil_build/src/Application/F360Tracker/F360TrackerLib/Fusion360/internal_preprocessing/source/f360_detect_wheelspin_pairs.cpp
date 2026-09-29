/*===========================================================================*\
* FILE: f360_detect_wheelspin_pairs.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains functionality for detection wheelspin based on pairing detections.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_math.h"
#include <algorithm>
#include "f360_iterator.h"
#include "f360_detect_wheelspin_pairs.h"
#include "f360_compute_wrapping_aware_spread.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   enum Distance_Check_T : uint8_t
   {
      DISTANCE_OK = (0),   // Indicates that the euclidian distance is ok according to threshold
      DISTANCE_NOK = (1),  // Indicates that the euclidian distance is not ok according to threshold
      LONG_POS_NOK = (2)   // Indicates that the euclidian distance was broken by the long position alone
   };

   using Sorted_Detection_Iterator_T = int16_t(*)(const rspp_variant_A::RSPP_Detection_T &); // Pointer to sorted detection iterator function

   /*===========================================================================*\
   * FUNCTION: Check_Distance()
   *===========================================================================
   * RETURN VALUE:
   * Distance_Check
   *
   * PARAMETERS:
   * const F360_Detection_Props_T &first_det_props
   * const F360_Detection_Props_T &second_det_props
   * const float32_t max_wheel_spin_dist_sq
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
   * Check if the distance between the two detections is within max_wheel_spin_dist_sq.
   * If the long position difference break the threshold alone it is indicated in the
   * return enum and no further calculations are performed.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static Distance_Check_T Check_Distance(
         const F360_Detection_Props_T &first_det_props,
         const F360_Detection_Props_T &second_det_props,
         const float32_t max_wheel_spin_dist_sq)
   {
      Distance_Check_T return_value;

      const float32_t long_pos_diff = first_det_props.vcs_position.x - second_det_props.vcs_position.x;
      const float32_t long_pos_diff_sq = long_pos_diff * long_pos_diff;

      if (long_pos_diff_sq > max_wheel_spin_dist_sq)
      {
         return_value = LONG_POS_NOK;
      }
      else
      {
         const float32_t lat_pos_diff = first_det_props.vcs_position.y - second_det_props.vcs_position.y;
         const float32_t dist_sq = long_pos_diff_sq + lat_pos_diff * lat_pos_diff;

         return_value = dist_sq < max_wheel_spin_dist_sq ? DISTANCE_OK : DISTANCE_NOK;
      }

      return return_value;
   }

   /*===========================================================================*\
   * FUNCTION: Sorted_Detection_Forward_Iterator()
   *===========================================================================
   * RETURN VALUE:
   * int16_t
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T &current_detection
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
   * Returns next_sorted_idx.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static inline int16_t Sorted_Detection_Forward_Iterator(
         const rspp_variant_A::RSPP_Detection_T &current_detection)
   {
      return current_detection.processed.next_sorted_idx;
   }

   /*===========================================================================*\
   * FUNCTION: Sorted_Detection_Backward_Iterator()
   *===========================================================================
   * RETURN VALUE:
   * int16_t
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T &current_detection
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
   * Returns eitherprev_sorted_idx.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static inline int16_t Sorted_Detection_Backward_Iterator(
      const rspp_variant_A::RSPP_Detection_T &current_detection)
   {
      return current_detection.processed.prev_sorted_idx;
   }


   /*===========================================================================*\
   * FUNCTION: Search_And_Mark_Close_Dets()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const uint32_t start_idx
   * const uint8_t search_iterations
   * const Sorted_Detection_Iterator_T Sorted_Detection_Iterator - pointer to iterator function,
   * const float32_t max_wheel_spin_dist_sq
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detections
   * F360_Detection_Props_T (&detProps)[MAX_NUMBER_OF_DETECTIONS]
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
   * Function that iterate a specified number of iterations along the sorted detection
   * props either backwards or forward. If a detection is within a defined distance
   * it is marked as wheelspin.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Search_And_Mark_Close_Dets(
         const uint32_t start_idx,
         const uint8_t search_iterations,
         const Sorted_Detection_Iterator_T Sorted_Detection_Iterator,
         const float32_t max_wheel_spin_dist_sq,
         const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
         F360_Detection_Props_T (&detProps)[MAX_NUMBER_OF_DETECTIONS])
   {
      // Step forward search_iterations detections and see if they are close to the start detection
      bool f_continue_search = true;

      int32_t det_iter_idx = static_cast<int32_t>(start_idx);
      for(uint8_t search_steps = 0U; (search_steps < search_iterations) && f_continue_search; search_steps++)
      {
         det_iter_idx = Sorted_Detection_Iterator(raw_detections.detections[det_iter_idx]);

         if(det_iter_idx >= 0)
         {
            const Distance_Check_T gate_check = Check_Distance(detProps[start_idx], detProps[det_iter_idx], max_wheel_spin_dist_sq);

            if(DISTANCE_OK == gate_check)
            {
               //Mark this detection as wheel spin as well since it is really close to other wheel spin detection
               detProps[det_iter_idx].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
            }
            else if (LONG_POS_NOK == gate_check)
            {
               f_continue_search = false;
            }
            else
            {
               // Just continue looking
            }
         }
         else
         {
            f_continue_search = false;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Find_Wheel_Spin_Candidates()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T &calibrations
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
   * uint32_t (&save_i_array)[MAX_SAVED_WHEEL_SPIN_PAIRS]
   * uint32_t (&save_j_array)[MAX_SAVED_WHEEL_SPIN_PAIRS]
   * float32_t (&save_doppler_spread_array)[MAX_SAVED_WHEEL_SPIN_PAIRS]
   * F360_Detection_Props_T (&detProps)[MAX_NUMBER_OF_DETECTIONS]
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
   *
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   template <uint32_t MAX_SAVED_WHEEL_SPIN_PAIRS>
   static void Find_Wheel_Spin_Candidates(
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      uint32_t (&save_i_array)[MAX_SAVED_WHEEL_SPIN_PAIRS],
      uint32_t (&save_j_array)[MAX_SAVED_WHEEL_SPIN_PAIRS],
      float32_t (&save_doppler_spread_array)[MAX_SAVED_WHEEL_SPIN_PAIRS],
      F360_Detection_Props_T (&detProps)[MAX_NUMBER_OF_DETECTIONS])
   {
      const uint32_t num_dets = raw_detections.number_of_valid_detections;

      if (num_dets > 0U)
      {
         // Loop through all detection combinations and see if they are close enough spatially but has a difference in range rate. Save the MAX_SAVED_WHEEL_SPIN_PAIRS number of detection pairs with the highest doppler spread. These are wheel spin detection candidates.
         uint32_t detIndi = static_cast<uint32_t>(raw_detections.vcslong_det_idx_min);
         for (uint32_t i = 0U; i < (num_dets - 1U); i++)
         {
            if (i > 0U)
            {
               detIndi = static_cast<uint32_t>(raw_detections.detections[detIndi].processed.next_sorted_idx);
            }

            const bool f_det_ok_to_flag = (std::abs(detProps[detIndi].vcs_position.x) <= calibrations.k_max_abs_vcs_long_posn_for_wheelspin_pair) &&
               (std::abs(detProps[detIndi].vcs_position.y) <= calibrations.k_max_abs_vcs_lat_posn_for_wheelspin_pair);

            if (f_det_ok_to_flag)
            {
               bool f_long_pos_ok = true;
               uint32_t detIndj = detIndi;
               for (uint32_t j = (i + 1U); (j < num_dets) && f_long_pos_ok; j++)
               {

                  detIndj = static_cast<uint32_t>(raw_detections.detections[detIndj].processed.next_sorted_idx);

                  // Check distance with coarse gate on long pos
                  const Distance_Check_T distance_gate_check = Check_Distance(detProps[detIndi],detProps[detIndj], calibrations.k_max_wheel_spin_dist_sq);
                  f_long_pos_ok = distance_gate_check != LONG_POS_NOK;

                  if (f_long_pos_ok)
                  {
                     const bool f_close_in_az = (std::abs(raw_detections.detections[detIndi].processed.vcs_az - raw_detections.detections[detIndj].processed.vcs_az) < calibrations.k_max_azimuth_difference_for_wheelspin_pair);
                     const bool f_compute_spread = (DISTANCE_OK == distance_gate_check) && f_close_in_az;
                     if (f_compute_spread)
                     {  // Compute doppler spread accounting for Doppler wrapping
                        const float32_t rng_rate1 = raw_detections.detections[detIndi].raw.range_rate;
                        const float32_t rng_rate2 = raw_detections.detections[detIndj].raw.range_rate;

                        const float32_t spread = Compute_Wrapping_Aware_Spread(
                           rng_rate1, rng_rate2,
                           raw_detections.detections[detIndi], raw_detections.detections[detIndj],
                           sensors);
                        // Keep track of the detection pairs with the largest Doppler spreads.
                        if ((spread >=calibrations.k_min_wheel_spin_doppler_spread) && (spread > save_doppler_spread_array[MAX_SAVED_WHEEL_SPIN_PAIRS - 1U])) // Doppler spread is large enough for pair to be stored as wheel spin candidates
                        {  // Store this detection pair
                           // Find at which index to insert new spread value to keep save_doppler_spread_array sorted with highest spread first
                           uint32_t last;
                           if (spread > save_doppler_spread_array[0])
                           {
                              last = 0U; // Insert on first place
                           }
                           else // Use interval halving to find where to insert new value
                           {
                              uint32_t first = 0U;
                              last = MAX_SAVED_WHEEL_SPIN_PAIRS - 1U;
                              while (last > first + 1U)
                              {
                                 const uint32_t diff = last - first;
                                 const uint32_t test = first + static_cast<uint32_t>(F360_Ceilf(static_cast<float32_t>(diff) / 2.0F));
                                 if (save_doppler_spread_array[test] < spread)
                                 {
                                    last = test;
                                 }
                                 else
                                 {
                                    first = test;
                                 }
                              }
                           } // Last is the index where to insert new value
                             // Shift elements to the right to make space for new value. Detection pair with the smallest doppler spread is thrown away.
                           for (uint32_t k = MAX_SAVED_WHEEL_SPIN_PAIRS - 1U; k > last; k--) {
                              save_doppler_spread_array[k] = save_doppler_spread_array[k - 1U];
                              save_i_array[k] = save_i_array[k - 1U];
                              save_j_array[k] = save_j_array[k - 1U];
                           }
                           // Insert new values
                           save_doppler_spread_array[last] = spread;
                           save_i_array[last] = detIndi;
                           save_j_array[last] = detIndj;

                        } // gate to check for significant doppler spread
                        else if (spread < 0.5F)
                        {
                            detProps[detIndi].f_has_close_det_with_similar_rr = true;
                            detProps[detIndj].f_has_close_det_with_similar_rr = true;
                        }
                        else
                        {
                            // Do nothing - spread is too small to mark as wheel spin and too big to save as similar rr.
                        }
                     } // gate to check for spatial closeness
                  }
               } // for inner loop: determining largest Doppler spread
            }//If det_ok_to_flag
         } // for outer loop: determining largest Doppler spread
      } // If any detections

   }

   /*===========================================================================*\
   * FUNCTION: Detect_Wheel_Spin_Pairs()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const F360_Calibrations_T &calibrations
   *   const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
   *   const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
   *   F360_Detection_Props_T (&detProps)[MAX_NUMBER_OF_DETECTIONS]
   *   F360_TRKR_TIMING_INFO_T &timing_info
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
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/

   void Detect_Wheel_Spin_Pairs(
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T (&detProps)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      // This function takes a list of detections and attempts to mark any
      // detections which look like they come from a spinning wheel.

      // 
      // 18 Needed to get exact same output as with old implementation for log CSLPFT_DDR9308_20180305_LowThreshFollowTargetHighway_113427_017.dvl.
      // TODO: Can this be reduced if including a check when saving pairs with highest doppler spread to see if there is any other pair with same i or j with higher/lower doppler spread?
      const uint8_t MAX_SAVED_WHEEL_SPIN_PAIRS = 20U; // Max number of stored possible wheel spin pairs. Should be greater than k_max_wheel_spin_dets_to_mark.
      uint32_t save_i_array[MAX_SAVED_WHEEL_SPIN_PAIRS];
      uint32_t save_j_array[MAX_SAVED_WHEEL_SPIN_PAIRS];
      float32_t save_doppler_spread_array[MAX_SAVED_WHEEL_SPIN_PAIRS];

      const float32_t start_time = get_wall_time();
      
      // Initialize arrays
      for (uint32_t l = 0U; l < MAX_SAVED_WHEEL_SPIN_PAIRS; l++)
      {
         save_doppler_spread_array[l] = -1.0F; // Initialize to something negative because only positive values will be stored here later. Important that initial values are negative because of sorting done later.
      }

      std::fill(cmn::begin(save_i_array), cmn::end(save_i_array), 0U);
      std::fill(cmn::begin(save_j_array), cmn::end(save_j_array), 0U);

      Find_Wheel_Spin_Candidates(calibrations, raw_detections, sensors, save_i_array, save_j_array, save_doppler_spread_array, detProps);

        // Loop through the wheel spin candiates from highest doppler spread to lowest and mark detections as wheel spin.
      uint32_t nr_found_wheel_spin_pairs = 0U;
      bool f_no_more_dets = false;
      for (uint32_t k = 0U; (k < MAX_SAVED_WHEEL_SPIN_PAIRS) && (!f_no_more_dets); k++)
      {
         const uint32_t detIndi = save_i_array[k];
         const uint32_t detIndj = save_j_array[k];
         

         // No more pairs with enough doppler spread has been found or maximum number of wheel spin pairs has been marked (last condition is to match while loop condition of previous implementation. TODO: Could possibly be removed?)
         f_no_more_dets = (calibrations.k_max_wheel_spin_dets_to_mark <= nr_found_wheel_spin_pairs) || (calibrations.k_min_wheel_spin_doppler_spread > save_doppler_spread_array[k]);

         const bool f_can_mark_dets = 
            (!f_no_more_dets) &&
            (detProps[detIndi].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID) &&
            (detProps[detIndj].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_INVALID);

         if (f_can_mark_dets)
         {  // There was a large enough Doppler spread to mark out at least 2
            // wheel spin detections
            nr_found_wheel_spin_pairs += 1U;
            detProps[detIndi].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
            detProps[detIndj].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
            
            Search_And_Mark_Close_Dets(detIndi, calibrations.k_wheelspin_pair_max_close_det_iterations, &Sorted_Detection_Forward_Iterator, calibrations.k_max_wheel_spin_dist_sq, raw_detections, detProps);
            Search_And_Mark_Close_Dets(detIndi, calibrations.k_wheelspin_pair_max_close_det_iterations, &Sorted_Detection_Backward_Iterator, calibrations.k_max_wheel_spin_dist_sq, raw_detections, detProps);
            Search_And_Mark_Close_Dets(detIndj, calibrations.k_wheelspin_pair_max_close_det_iterations, &Sorted_Detection_Forward_Iterator, calibrations.k_max_wheel_spin_dist_sq, raw_detections, detProps);
            Search_And_Mark_Close_Dets(detIndj, calibrations.k_wheelspin_pair_max_close_det_iterations, &Sorted_Detection_Backward_Iterator, calibrations.k_max_wheel_spin_dist_sq, raw_detections, detProps);
         } // Detection has already been marked as wheel spin earlier, do nothing and continue to check next possible wheel spin pair.
        
      } // end for loop: saved possible wheel spin pair 
      
       // demark detections that were marked as wheel_spin if they have detection with similar range rate in vicinity
      for (uint32_t i = 0U; i < raw_detections.number_of_valid_detections; i++)
      {
          if ((detProps[i].f_has_close_det_with_similar_rr) && (detProps[i].wheel_spin_type == F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS))
          {
              detProps[i].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
          }
      }
      timing_info.detect_wheelspin = get_wall_time() - start_time;

   } //function
     // NOTES:
     //  (1) For now, greedily grab first detections in expanded bounding box, neglecting possibility that expanded
     //      bounding boxes can overlap.
}
