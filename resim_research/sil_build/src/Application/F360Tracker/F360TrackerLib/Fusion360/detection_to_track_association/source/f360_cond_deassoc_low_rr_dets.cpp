/*===========================================================================*\
* FILE: f360_cond_deassoc_low_rr_dets.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Cond_Deassoc_Low_RR_Dets()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_cond_deassoc_low_rr_dets.h" 
#include "f360_get_reference_point_orth_side.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_iterator.h"
#include "f360_associate_detection_to_object.h"
#include <algorithm>

namespace f360_variant_A
{

   static void Partially_Deassoc_Det_And_Obj(
      F360_Detection_Props_T& det_prop,
      F360_Object_Track_T& obj);

   static void Keep_Detection(
      const uint32_t det_idx,
      uint32_t& number_of_kept_dets,
      uint32_t(&det_ids_for_kept_dets)[MAX_DETS_IN_OBJ_TRK]);

   /*===========================================================================*\
   * FUNCTION: Cond_Deassoc_Low_RR_Dets()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibs                                 - Calibration structure
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS] - Detection properties structure
   * F360_Object_Track_T & obj                                          - Object to potentially deassociate detections from
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function conditionally deassociates detections from object based on detection 
   * compensated range rate and position of detection compared to visible orth edge 
   * of object. Deassociation is only considered if object has a speed greater than 
   * a calibration value.
   *
   \*===========================================================================*/
   void Cond_Deassoc_Low_RR_Dets(
      const F360_Calibrations_T & calibs,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T & obj)
   {
      // Conditionally deassociate detections in cone of silence for objects with speed 
      const F360_Object_Sides_T visible_orth_side = Get_Reference_Point_Orth_Side(obj.reference_point);
      const bool orth_edge_visible = ((F360_OBJECT_SIDES_LEFT == visible_orth_side) || (F360_OBJECT_SIDES_RIGHT == visible_orth_side));
      if ((std::abs(obj.speed) > calibs.k_cond_deassoc_min_obj_spd_for_deassoc) && orth_edge_visible)
      {

         uint32_t number_of_dets_to_keep = 0U;
         uint32_t det_ids_for_kept_dets[MAX_DETS_IN_OBJ_TRK] = {};
         const uint32_t original_ndets = obj.ndets;
         for (uint32_t det_i = 0U; det_i < original_ndets; det_i++)
         {
            const uint32_t det_idx = obj.detids[det_i] - 1U;

            if (std::abs(detection_props[det_idx].range_rate_compensated) < calibs.k_cond_deassoc_det_comp_rr_max)
            {
               float32_t tcs_det_x = 0.0F;
               float32_t tcs_det_y = 0.0F;
               Convert_VCS_Posn_To_TCS_Posn(detection_props[det_idx].vcs_position.x,
                  detection_props[det_idx].vcs_position.y,
                  obj.bbox.Get_Center().x,
                  obj.bbox.Get_Center().y,
                  obj.bbox.Get_Orientation(),
                  tcs_det_x,
                  tcs_det_y);

               if (F360_OBJECT_SIDES_LEFT == visible_orth_side)
               {
                  // Calculate position threshold in tcs, threshold is a fraction of object width away from the visible orth edge
                  const float32_t tcs_pos_threshold = (-0.5F * obj.bbox.Get_Width() + calibs.k_cond_deassoc_fraction_of_width_to_deassoc * obj.bbox.Get_Width());
                  const bool f_det_too_far_from_left_edge = tcs_det_y > tcs_pos_threshold;
                  if (f_det_too_far_from_left_edge)
                  {
                     Partially_Deassoc_Det_And_Obj(detection_props[det_idx], obj);
                  }
                  else
                  {
                     Keep_Detection(det_idx, number_of_dets_to_keep, det_ids_for_kept_dets);
                  }
               }
               else // Right edge visible
               {
                  // Calculate position threshold in tcs, threshold is a fraction of object width away from the visible orth edge
                  const float32_t tcs_pos_threshold = (0.5F * obj.bbox.Get_Width() - calibs.k_cond_deassoc_fraction_of_width_to_deassoc * obj.bbox.Get_Width());
                  const bool f_det_too_far_from_right_edge = tcs_det_y < tcs_pos_threshold;
                  if (f_det_too_far_from_right_edge)
                  {
                     Partially_Deassoc_Det_And_Obj(detection_props[det_idx], obj);
                  }
                  else
                  {
                     Keep_Detection(det_idx, number_of_dets_to_keep, det_ids_for_kept_dets);
                  }
               }
            }
            else
            {
               Keep_Detection(det_idx, number_of_dets_to_keep, det_ids_for_kept_dets);
            }
         }
         
         if (number_of_dets_to_keep < obj.ndets)
         {
            (void)std::copy(cmn::begin(det_ids_for_kept_dets), cmn::end(det_ids_for_kept_dets), cmn::begin(obj.detids));

            obj.ndets = number_of_dets_to_keep;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Partially_Deassoc_Det_And_Obj()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Detection_Props_T & det_prop - Detection to deassociate
   * F360_Object_Track_T& obj - Object from which the detection should be de-associated
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function sets object track id to 0 for the specified detection and updates the object properties for keeping track of
   * motion status of associated detections.
   *
   * POSTCONDITIONS:
   * The caller must make sure that the following object properties are updated correctly after calling this function:
   * - Number of detections (ndets)
   * - Ids of associated detections (detids)
   *
   \*===========================================================================*/
   static void Partially_Deassoc_Det_And_Obj(
      F360_Detection_Props_T& det_prop,
      F360_Object_Track_T& obj)
   {
      det_prop.object_track_id = 0;
      // Update counter of associated detection types
      if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_prop.motion_status)
      {
         obj.num_types_of_dets[0]--;
      }
      else
      {
         obj.num_types_of_dets[1]--;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Keep_Detection()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const uint32_t det_idx - Index for detection to keep
   * int32_t number_of_kept_dets - Number of detections that are to be kept
   * uint32_t (&det_ids_for_kept_dets)[MAX_DETS_IN_OBJ_TRK] - Array with indices to keep
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function adds a detection to an array of detections to keep and increments the number of detections that are kept.
   *
   \*===========================================================================*/
   static void Keep_Detection(
      const uint32_t det_idx,
      uint32_t& number_of_kept_dets,
      uint32_t(&det_ids_for_kept_dets)[MAX_DETS_IN_OBJ_TRK])
   {
      det_ids_for_kept_dets[number_of_kept_dets] = det_idx + 1U;
      number_of_kept_dets++;
   }
}
