/*===========================================================================*/
/**
 * @file rspp_vcs_long_sorted_dets_support_functions.cpp
 *
 * @brief VCS longitudinal sorting support functions for detections
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 * Implementation of support functions for sorting radar detections by their
 * longitudinal position in the Vehicle Coordinate System (VCS). Provides
 * functionality to maintain sorted detection lists with reference points for
 * efficient spatial queries and detection management.
 *
 * @section ABBR ABBREVIATIONS:
 *   - VCS: Vehicle Coordinate System
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
 */
/*===========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "rspp_vcs_long_sorted_dets_support_functions.h"
#include "rspp_math_func.h"

/*===========================================================================*
 * Forward Declarations
 *===========================================================================*/

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
   static constexpr int32_t RSPP_INVALID_ID = -1;

   /*===========================================================================*
    * Function Definitions
    *===========================================================================*/
   void RSPP_Sort_Detections_Vcs_Long(
       RSPP_Detection_List_T &raw_detections)
   {
      const uint32_t ndets = raw_detections.number_of_valid_detections;

      // Reset tracker info structure
      RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detections);

      // Create temporary array of all detections long position
      float32_t det_long_pos[MAX_NUMBER_OF_DETECTIONS] = {};
      for (uint32_t i = 0U; i < ndets; i++)
      {
         det_long_pos[i] = raw_detections.detections[i].processed.vcs_position_x;
      }

      // Sort temporary array detections
      uint32_t sort_idx[MAX_NUMBER_OF_DETECTIONS] = {};
      (void)RSPP_Sort(det_long_pos, static_cast<uint32_t>(ndets), true, sort_idx);

      if (1U < ndets)
      {
         // Special handling of first detection in sorted list
         uint32_t det_idx = sort_idx[0];
         raw_detections.vcslong_det_idx_min = static_cast<int16_t>(det_idx);
         raw_detections.vcslong_sorted_ref_det_idx[0] = raw_detections.vcslong_det_idx_min;
         raw_detections.detections[det_idx].processed.prev_sorted_idx = static_cast<int16_t>(RSPP_INVALID_ID);
         raw_detections.detections[det_idx].processed.next_sorted_idx = static_cast<int16_t>(sort_idx[1]);

         uint32_t vcs_long_ref_point_idx = 0U;
         RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
             raw_detections.detections[det_idx].processed.vcs_position_x,
             det_idx,
             vcs_long_ref_point_idx,
             raw_detections);

         // Loop over sorted list but exclude first and last element
         for (uint32_t i = 1U; i < ndets - 1U; i++)
         {
            det_idx = sort_idx[i];

            RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
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
         raw_detections.detections[det_idx].processed.next_sorted_idx = static_cast<int16_t>(RSPP_INVALID_ID);
         raw_detections.vcslong_det_idx_max = static_cast<int16_t>(det_idx);

         RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
             raw_detections.detections[det_idx].processed.vcs_position_x,
             det_idx,
             vcs_long_ref_point_idx,
             raw_detections);

         // Append the first invalid detection index in the array of reference detections with the
         // index of detection at largest VCS-longitudinal position.
         raw_detections.vcslong_sorted_ref_det_idx[vcs_long_ref_point_idx + 1U] = raw_detections.vcslong_det_idx_max;
      }
      else if (1U == ndets)
      {
         // Only one detection available
         const uint32_t det_idx = sort_idx[0];
         raw_detections.detections[det_idx].processed.prev_sorted_idx = static_cast<int16_t>(RSPP_INVALID_ID);
         raw_detections.detections[det_idx].processed.next_sorted_idx = static_cast<int16_t>(RSPP_INVALID_ID);
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

   void RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       const float32_t det_vcs_long,
       const uint32_t det_idx,
       uint32_t &vcs_long_ref_start_idx,
       RSPP_Detection_List_T &raw_detections)
   {
      const uint32_t start_idx = vcs_long_ref_start_idx;

      // Check if detection long pos is the first detection above a reference point
      for (uint32_t vcs_long_ref_point_idx = start_idx;
           vcs_long_ref_point_idx < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS;
           vcs_long_ref_point_idx++)
      {
         const int32_t ref_det_idx = raw_detections.vcslong_sorted_ref_det_idx[vcs_long_ref_point_idx + 1U];
         if (RSPP_INVALID_ID == ref_det_idx)
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

   void RSPP_Clear_Dets_Vcs_Long_Sorted_Info(
       RSPP_Detection_List_T &raw_detections)
   {
      const int16_t invalid_16_bit_signed_id = static_cast<int16_t>(RSPP_INVALID_ID);
      // Reset max and min points
      raw_detections.vcslong_det_idx_min = invalid_16_bit_signed_id;
      raw_detections.vcslong_det_idx_max = invalid_16_bit_signed_id;

      // Reset reference points including first index that corresponds to the detection index with smallest VCS-long position.
      // And last corresponds to the detection with largest VCS-long position.
      std::fill(cmn::begin(raw_detections.vcslong_sorted_ref_det_idx),
                cmn::end(raw_detections.vcslong_sorted_ref_det_idx), invalid_16_bit_signed_id);
   }

   int32_t RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(
       const float32_t vcs_long_value,
       const RSPP_Detection_List_T &raw_detections)
   {
      // Initialize return variable
      int32_t relevant_det_idx = RSPP_INVALID_ID;

      // Loop over array with all reference detection indexes until we have found first relevant reference detection.
      // First index is detection index with smallest vcs-long position so start loop at second element
      bool f_continue = true;
      for (uint32_t i = 1U; ((i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS) && f_continue); i++)
      {
         const int32_t det_idx = raw_detections.vcslong_sorted_ref_det_idx[i];

         if ((RSPP_INVALID_ID < det_idx) &&
             (0.0F < (raw_detections.detections[det_idx].processed.vcs_position_x - vcs_long_value)))
         {
            // Positive diff indicates we have overshot the desired value. Return previous index.
            relevant_det_idx = raw_detections.vcslong_sorted_ref_det_idx[i - 1U];
            f_continue = false;
         }
         else if (RSPP_INVALID_ID == det_idx)
         {
            // We have reached an invalid ID in reference points. This means there are no detections above
            // desired long value
            relevant_det_idx = RSPP_INVALID_ID;
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
}

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
