/*===========================================================================*\
* FILE: f360_vcs_long_sorted_dets_support_functions.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function definition of Sort_Detections_Vcs_Long() and support functions to operate on the sorted list.
*   It contains both the actual sorting function and also support functions for efficient use of the sorted list.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [30-Mar-2018]
*
\*===========================================================================*/

#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_math_func.h"
#include "f360_iterator.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Sort_Detections_Vcs_Long()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
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
   * Sorts detections in longitudinal ascending order. A reference index i stored
   * in raw_detections at certain predefined longitudinal positions as well as index of
   * detection with min/max vcs long position.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Sort_Detections_Vcs_Long(
      rspp_variant_A::RSPP_Detection_List_T& raw_detections)
   {
      const uint32_t ndets = raw_detections.number_of_valid_detections;

      // Reset tracker info structure
      Clear_Dets_Vcs_Long_Sorted_Info(raw_detections);

      // Create temporary array of all detections long position
      float32_t det_long_pos[MAX_NUMBER_OF_DETECTIONS] = {};
      for (uint32_t i = 0U; i < ndets; i++)
      {
         det_long_pos[i] = raw_detections.detections[i].processed.vcs_position_x;
      }

      // Sort temporary array detections
      uint32_t sort_idx[MAX_NUMBER_OF_DETECTIONS] = {};
      (void)F360_Sort(static_cast<uint32_t>(ndets), true, det_long_pos, sort_idx);

      if (ndets > 1U)
      {
         // Special handling of first detection in sorted list
         uint32_t det_idx = sort_idx[0];
         raw_detections.vcslong_det_idx_min = static_cast<int16_t>(det_idx);
         raw_detections.vcslong_sorted_ref_det_idx[0] = raw_detections.vcslong_det_idx_min;
         raw_detections.detections[det_idx].processed.prev_sorted_idx = static_cast<int16_t>(F360_INVALID_ID);
         raw_detections.detections[det_idx].processed.next_sorted_idx = static_cast<int16_t>(sort_idx[1]);

         uint32_t vcs_long_ref_point_idx = 0U;
         Update_Dets_Vcs_Long_Ref_Sorted_Info(
            raw_detections.detections[det_idx].processed.vcs_position_x,
            det_idx,
            vcs_long_ref_point_idx,
            raw_detections);

         // Loop over sorted list but exclude first and last element
         for (uint32_t i = 1U; i < ndets - 1U; i++)
         {
            det_idx = sort_idx[i];

            Update_Dets_Vcs_Long_Ref_Sorted_Info(
               raw_detections.detections[det_idx].processed.vcs_position_x,
               det_idx,
               vcs_long_ref_point_idx,
               raw_detections);

            raw_detections.detections[det_idx].processed.prev_sorted_idx = static_cast<int16_t>(sort_idx[i - 1U]);
            raw_detections.detections[det_idx].processed.next_sorted_idx = static_cast<int16_t>(sort_idx[i + 1U]);
         }

         // Special handling of last detection in sorted list
         det_idx = sort_idx[ndets - 1U];
         raw_detections.detections[det_idx].processed.prev_sorted_idx = static_cast<int16_t>(sort_idx[ndets - 2U]);
         raw_detections.detections[det_idx].processed.next_sorted_idx = static_cast<int16_t>(F360_INVALID_ID);
         raw_detections.vcslong_det_idx_max = static_cast<int16_t>(det_idx);

         Update_Dets_Vcs_Long_Ref_Sorted_Info(
            raw_detections.detections[det_idx].processed.vcs_position_x,
            det_idx,
            vcs_long_ref_point_idx,
            raw_detections);

         // Append the first invalid detection index in the array of reference detections with the
         // index of detection at largest VCS-longitudinal position.
         raw_detections.vcslong_sorted_ref_det_idx[vcs_long_ref_point_idx + 1U] = raw_detections.vcslong_det_idx_max;
      }
      else if (ndets == 1U)
      {
         // Only one detection available
         const uint32_t det_idx = sort_idx[0];
         raw_detections.detections[det_idx].processed.prev_sorted_idx = static_cast<int16_t>(F360_INVALID_ID);
         raw_detections.detections[det_idx].processed.next_sorted_idx = static_cast<int16_t>(F360_INVALID_ID);
         raw_detections.vcslong_det_idx_min = static_cast<int16_t>(det_idx);
         raw_detections.vcslong_det_idx_max = static_cast<int16_t>(det_idx);

         // First and last element of array is filled with idx of detection with smallest/largest vcs longitudinal position
         raw_detections.vcslong_sorted_ref_det_idx[0] = raw_detections.vcslong_det_idx_min;
         raw_detections.vcslong_sorted_ref_det_idx[1] = raw_detections.vcslong_det_idx_min;
      }
      else
      {
         // Do nothing, there are no detections
      }
   }

   /*===========================================================================*\
   * FUNCTION: Update_Dets_Vcs_Long_Ref_Sorted_Info()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const float32_t det_vcs_long - Long position of detection
   *  const uint32_t det_idx - Array index of detection
   *  uint32_t &vcs_long_ref_start_idx - Reference to variable that contains loop start value.
   *  rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
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
   * Updates data relevant to vcs sorted detections in tracker info structure.
   * Also updates which reference point we should start at the next time this
   * function is called.
   *
   * PRECONDITIONS:
   * Requires that the calibration array vcs_long_sorted_ref_points[] is defined
   * in ascending order.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Dets_Vcs_Long_Ref_Sorted_Info(
      const float32_t det_vcs_long,
      const uint32_t det_idx,
      uint32_t& vcs_long_ref_start_idx,
      rspp_variant_A::RSPP_Detection_List_T& raw_detections)
   {
      const uint32_t start_idx = vcs_long_ref_start_idx;

      // Check if detection long pos is the first detection above a reference point
      for (uint32_t vcs_long_ref_point_idx = start_idx; vcs_long_ref_point_idx < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS; vcs_long_ref_point_idx++)
      {
         const int32_t ref_det_idx = raw_detections.vcslong_sorted_ref_det_idx[vcs_long_ref_point_idx + 1U];
         if (F360_INVALID_ID == ref_det_idx)
         {
            const float32_t vcs_long_sorted_ref_points[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS] = {
               -100.0F, -90.0F, -80.0F, -70.0F, -60.0F, -50.0F,
               -40.0F, -30.0F, -20.0F, -10.0F, 0.0F, 10.0F,
               20.0F, 30.0F, 40.0F, 50.0F, 60.0F, 70.0F,
               80.0F, 90.0F, 100.0F, 110.0F, 120.0F, 130.0F,
               140.0F, 150.0F, 160.0F, 170.0F, 180.0F, 190.0F, 200.0F };
            if (det_vcs_long > vcs_long_sorted_ref_points[vcs_long_ref_point_idx])
            {
               // First slot in vcslong_sorted_ref_det_idx is the index of detection with smallest VCS long pos,
               // thus the offset with 1
               raw_detections.vcslong_sorted_ref_det_idx[vcs_long_ref_point_idx + 1U] = static_cast<int16_t>(det_idx);

               // We have found the first detection above current reference point. In other words we now know at which calibration
               // reference point we can start at the next time this function is called.
               vcs_long_ref_start_idx = vcs_long_ref_point_idx + 1U;
            }
            else
            {
               // Current detection is lower than the next non-filled reference detection. This detection can't be larger
               // than any more reference point since they are in increasing order
               break;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Clear_Dets_Vcs_Long_Ref_Sorted_Info()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
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
   * Resets data relevant to vcs sorted detections in tracker info structure.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Clear_Dets_Vcs_Long_Sorted_Info(
      rspp_variant_A::RSPP_Detection_List_T& raw_detections)
   {
      const int16_t invalid_16_bit_signed_id = static_cast<int16_t>(F360_INVALID_ID);
      // Reset max and min points
      raw_detections.vcslong_det_idx_min = invalid_16_bit_signed_id;
      raw_detections.vcslong_det_idx_max = invalid_16_bit_signed_id;

      // Reset reference points including first index that corresponds to the detection index with smallest VCS-long position.
      // And last corresponds to the detection with largest VCS-long position.
      std::fill(cmn::begin(raw_detections.vcslong_sorted_ref_det_idx), cmn::end(raw_detections.vcslong_sorted_ref_det_idx), invalid_16_bit_signed_id);
   }

   /*===========================================================================*\
   * FUNCTION: Get_First_Relevant_Long_Sorted_Det_Idx()
   *===========================================================================
   * RETURN VALUE:
   * int32_t relevant_det_idx
   *
   * PARAMETERS:
   *  const float32_t vcs_long_value - Longitudninal value to find closest index for
   *  rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
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
   * Returns the index of detection that is the closest reference point < vcs_long_value.
   *
   * PRECONDITIONS:
   * Requires that detections have been sorted in VCS-longitudinal order and that
   * raw_detections has been filled with data which is done in function Sort_Detections_Vcs_Long()
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   int32_t Get_First_Relevant_Long_Sorted_Det_Idx(
      const float32_t vcs_long_value,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections)
   {
      // Initialize return variable
      int32_t relevant_det_idx = F360_INVALID_ID;

      // Loop over array with all reference detection indexes until we have found first relevant reference detection.
      // First index is detection index with smallest vcs-long position so start loop at second element
      bool f_continue = true;
      for (uint32_t i = 1U; ((i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS) && f_continue); i++)
      {
         const int32_t det_idx = raw_detections.vcslong_sorted_ref_det_idx[i];

         if ((det_idx > F360_INVALID_ID) &&
            ((raw_detections.detections[det_idx].processed.vcs_position_x - vcs_long_value) > 0.0F))
         {
            // Positive diff indicates we have overshot the desired value. Return previous index.
            relevant_det_idx = raw_detections.vcslong_sorted_ref_det_idx[i - 1U];
            f_continue = false;
         }
         else if (F360_INVALID_ID == det_idx)
         {
            // We have reached an invalid ID in reference points. This means there are no detections above
            // desired long value
            relevant_det_idx = F360_INVALID_ID;
            f_continue = false;
         }
         else
         {
            // We haven't reached a reference position greater than desired value yet, keep looping
            f_continue = true;
         }
      }

      return relevant_det_idx;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Det_Indexes_In_Vcs_Zone()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const float32_t vcs_x_min - Minimum limit of vcs patch in long-direction
   *  const float32_t vcs_x_max - Maximum limit of vcs patch in long-direction
   *  const float32_t vcs_y_min - Minimum limit of vcs patch in lat-direction
   *  const float32_t vcs_y_max - Maximum limit of vcs patch in lat-direction
   *  rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
   *  const F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS] - Reference to detection props structure
   *  int32_t (&relevant_det_idx)[MAX_NUMBER_OF_DETECTIONS] - Array reference to detection index inside zone
   *  uint32_t &num_relevant_dets - Number of detections inside zone
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
   * Returns index of detections inside a squared zone defined in VCS.
   * This function utilizes the vcs-long sorted list of detections to efficiently
   * find relevant detection indexes inside the patch.
   *
   * PRECONDITIONS:
   * Detections have to be sorted in longitudinal direction and information in raw_detections
   * structure is filled by calling Sort_Detections_Vcs_Long().
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Get_Det_Indexes_In_Vcs_Zone(
      const float32_t vcs_x_min,
      const float32_t vcs_x_max,
      const float32_t vcs_y_min,
      const float32_t vcs_y_max,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      int32_t(&relevant_det_idx)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t& num_relevant_dets)
   {
      // Initiazilize data
      num_relevant_dets = 0U;

      // Find the closest reference point < than start of zone in x-min direction
      int32_t det_idx = Get_First_Relevant_Long_Sorted_Det_Idx(
         vcs_x_min,
         raw_detections);

      for (uint32_t i = 0U; i < raw_detections.number_of_valid_detections; i++)
      {
         const bool f_is_dets_remaining = Is_More_Relevant_Dets_In_Vcs_Zone(det_idx, vcs_x_max, det_p);

         if (!f_is_dets_remaining)
         {
            // We have overshot vcs_x_max or no more relevant detections are available
            break;
         }
         else if (det_p[det_idx].vcs_position.x > vcs_x_min)
         {
            // Detection is inside x-direction position gate. Now check lateral gating
            const float32_t det_lat_pos = det_p[det_idx].vcs_position.y;
            if ((det_lat_pos > vcs_y_min) &&
               (det_lat_pos < vcs_y_max))
            {
               // Detection is within lateral gate, add it to return array and increase counter
               relevant_det_idx[num_relevant_dets] = det_idx;
               num_relevant_dets++;
            }
         }
         else
         {
            // Detection x-pos < vcs_x_min
            // Do nothing here, detection not relevant
         }

         // Get next detection index to try
         det_idx = raw_detections.detections[det_idx].processed.next_sorted_idx;
      }
   }

   void Get_Det_Indexes_In_Vcs_Circular_Zone(
      const Point& center,
      const float32_t radius,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t(&relevant_det_idx)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t& num_relevant_dets)
   {
      // Initiazilize data
      num_relevant_dets = 0U;

      const float32_t vcs_x_min = center.x - radius;
      const float32_t vcs_x_max = center.x + radius;
      // Find the closest reference point < than start of zone in x-min direction
      int32_t det_idx = Get_First_Relevant_Long_Sorted_Det_Idx(vcs_x_min, raw_detections);

      const float32_t radius_sq = radius * radius;

      for (uint32_t i = 0U; i < raw_detections.number_of_valid_detections; i++)
      {
         const bool f_is_dets_remaining = Is_More_Relevant_Dets_In_Vcs_Zone(det_idx, vcs_x_max, det_p);

         if (!f_is_dets_remaining)
         {
            // We have overshot vcs_x_max or no more relevant detections are available
            break;
         }
         else if (det_p[det_idx].vcs_position.x > vcs_x_min)
         {
            const float32_t det_long_pos_delta = det_p[det_idx].vcs_position.x - center.x;
            const float32_t det_lat_pos_delta = det_p[det_idx].vcs_position.y - center.y;
            const bool is_inside_circle =
               (F360_Get_Hypotenuse_Squared(det_long_pos_delta, det_lat_pos_delta) <= radius_sq);

            if (is_inside_circle)
            {
               relevant_det_idx[num_relevant_dets] = static_cast<uint32_t>(det_idx);
               det_p[det_idx].f_inside_gate = true;
               num_relevant_dets++;
            }
         }
         else
         {
            // Detection x-pos < vcs_x_min
            // Do nothing here, detection not relevant
         }

         // Get next detection index to try
         det_idx = raw_detections.detections[det_idx].processed.next_sorted_idx;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_More_Relevant_Dets_In_Vcs_Zone()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_dets_remaining
   *
   * PARAMETERS:
   *  const int32_t det_idx
   *  const float32_t vcs_x_max
   *  const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Determines whether or not there are remaining relevant detections below a
   * given vcs-longitudinal position. It also verifies that the detection index
   * provided is a valid one. Returns true if there are relevant detections remaining.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_More_Relevant_Dets_In_Vcs_Zone(
      const int32_t det_idx,
      const float32_t vcs_x_max,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      bool f_is_dets_remaining;

      if (det_idx > F360_INVALID_ID)
      {
         if (det_props[det_idx].vcs_position.x > vcs_x_max)
         {
            f_is_dets_remaining = false;
         }
         else
         {
            f_is_dets_remaining = true;
         }
      }
      else
      {
         f_is_dets_remaining = false;
      }

      return f_is_dets_remaining;
   }
}
