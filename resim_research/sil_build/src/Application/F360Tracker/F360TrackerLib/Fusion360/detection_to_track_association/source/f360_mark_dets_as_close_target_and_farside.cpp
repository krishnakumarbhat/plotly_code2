/*===================================================================================*\
* FILE:  f360_mark_dets_as_close_target_and_farside.cpp
*====================================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functions to mark detections as close target, if they are too close
* to an unassociated target, or deassociate from an object if the detection is too far 
* from the associated object on the farside.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_mark_dets_as_close_target_and_farside.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_calc_dist_to_edge.h"
#include "f360_get_reference_point_orth_side.h"
#include "f360_get_edge_tcs_start_and_end_point.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Mark_Dets_As_Close_Target_And_Farside()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const uint32_t num_dets
   * F360_Object_Track_T & object_track
   * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Calibrations_T & calibrations
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
   * This function does two things :
   * 1. Loops over all detections and marks unassociated detections as "close target" 
   *    if they are inside a predefined zone (ct_ext_box_on_farside), i.e. relatively 
   *    close to an unassociated target. 
   * 2. Loop over all associated detections for a target and deassociates them if they 
   *    are considered to be on the farside, meaning that they are outside a predefined 
   *    zone (fcm_ext_box_on_nearby_side).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Mark_Dets_As_Close_Target_And_Farside(
      const uint32_t num_dets,
      F360_Object_Track_T & object_track,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T & calibrations)
   {
      // Compute closest edge in orthogonal direction (left or right)
      const F360_Object_Sides_T closest_orth_edge = Get_Reference_Point_Orth_Side(object_track.reference_point);

      /* This functionality only applies for objects that:
       * - Are moveable
       * - Are in close distance (within 6 m)
       * - Are heading in the same (opposite) direction of host (within 30 degrees)
       * - Has a clear visible orth edge (left or right)
       */
      if (Is_Obj_Relevant_For_Close_Target_And_Farside(object_track, calibrations, closest_orth_edge))
      {
         // Compute extended bounding boxes
         BoundingBox ct_ext_box_on_farside { object_track.bbox };
         BoundingBox fcm_ext_box_on_nearby_side{ object_track.bbox };
         Compute_Ext_Bounding_Boxes(object_track, calibrations, closest_orth_edge, fcm_ext_box_on_nearby_side, ct_ext_box_on_farside);

         Mark_Detections_As_Close_Target(num_dets, ct_ext_box_on_farside, det_props);

         Farside_Countermeasure(object_track, det_props, fcm_ext_box_on_nearby_side);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Detections_As_Close_Target()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const uint32_t num_dets
   * const BoundingBox & ct_ext_box_on_farside
   * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS])
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
   * This function marks detections as being f_close_target if they fulfil some
   * preconditions (e.g. being unassociated, not marked as close target before, 
   * not wheel spin and not double bounce) and are inside the predefined zone
   * ct_ext_box_on_farside.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Detections_As_Close_Target(
      const uint32_t num_dets,
      const BoundingBox & ct_ext_box_on_farside,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      // Loop over all detections
      for (uint32_t det_idx = 0U; det_idx < num_dets; det_idx++)
      {
         // Check if the detection is relevant: must be unassociated, not marked as close target before, not wheel spin and not double bounce
         if (Is_Det_Relevant_For_Close_Target(det_props[det_idx]))
         {
            // Check if detection is inside ct_ext_box_on_farside and mark correspondingly
            det_props[det_idx].f_close_target = ct_ext_box_on_farside.Contains(det_props[det_idx].vcs_position);
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Farside_Countermeasure()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T & object_track
   * F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const BoundingBox & fcm_ext_box_on_nearby_side 
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
   * This function loops over all associated detections for a target and 
   * deassociates them if they are considered to be on the farside, meaning that
   * they are outside a predefined zone (fcm_ext_box_on_nearby_side). The
   * detection is also marked as f_close_target = true and f_ok_to_use = false.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Farside_Countermeasure(
      F360_Object_Track_T& object_track,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const BoundingBox& fcm_ext_box_on_nearby_side)
   {
      // Initialize variables for old (prev) and new (plausible) number of associated detections
      const uint32_t prev_ndets = object_track.ndets;
      uint32_t plausible_ndets = 0U;

      // Loop over all associated detections for farside countermeasure
      for (uint32_t i = 0U; i < prev_ndets; i++)
      {
         const uint32_t det_idx = object_track.detids[i] - 1U;
         if (!fcm_ext_box_on_nearby_side.Contains(det_props[det_idx].vcs_position))
         {
            // Detection is considered as bad and being too far away from target. Mark as close target, not ok to use and deassociate it from target
            det_props[det_idx].f_close_target = true;
            det_props[det_idx].f_ok_to_use = false;
            det_props[det_idx].object_track_id = 0;

            // Update counter of associated detection types
            if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_props[det_idx].motion_status)
            {
               object_track.num_types_of_dets[0]--;
            }
            else
            {
               object_track.num_types_of_dets[1]--;
            }
         }
         else
         {
            // Detection is inside acceptable farside zone. Increment detection list accordingly
            object_track.detids[plausible_ndets] = object_track.detids[i];
            plausible_ndets++;
         }
      }

      // If farside countermeasure has been activated (detections were removed), update number of detections as object properties.
      const bool f_countermeasure_activated = (prev_ndets > plausible_ndets);
      if (f_countermeasure_activated)
      {
         // Loop over elements in detids that should be reset to 0
         for (uint32_t j = plausible_ndets; j < prev_ndets; j++)
         {
            object_track.detids[j] = 0U;
         }
         object_track.ndets = plausible_ndets;
      }
      else
      {
         // No detections were marked as not ok. Do not update object properties. 
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Obj_Relevant_For_Close_Target_And_Farside()
   *===========================================================================
   * RETURN VALUE:
   * bool f_relevant
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track
   * const F360_Calibrations_T & calibs
   * const F360_Visible_Edges_Type_T closest_orth_edge
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
   * This function determines if an object is relevant for the two parts: close
   * target functionality (ct) and farside countermeasure (fcm). The target has to
   * be: CTCA or fast moving, close enough (euclidian distance), heading in parallel/opposite
   * direction of host and have a well-defined orthogonal edge.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Obj_Relevant_For_Close_Target_And_Farside(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calibs,
      const F360_Object_Sides_T closest_orth_edge)
   {
      const bool f_close_enough_x_direction = (object_track.vcs_position.x < 5.0F);
      const bool f_is_ctca_or_fast_moving_obj = ((F360_TRACKER_TRKFLTR_CTCA == object_track.trk_fltr_type) || (std::abs(object_track.speed) > calibs.fast_moving_thresh));
      const bool f_dist_to_obj_is_small_enough = ((object_track.vcs_position.x * object_track.vcs_position.x + object_track.vcs_position.y * object_track.vcs_position.y) < calibs.k_ct_and_fcm_max_dist_for_close_target_sq);
      const bool f_obj_hdg_is_parallell_to_host = ((std::abs(object_track.vcs_heading.Value()) < calibs.k_ct_and_fcm_max_abs_heading_diff_to_host) || (std::abs(object_track.vcs_heading.Value()) > (F360_PI - calibs.k_ct_and_fcm_max_abs_heading_diff_to_host)));
      const bool f_is_any_long_side_visible = ((F360_OBJECT_SIDES_LEFT == closest_orth_edge) || (F360_OBJECT_SIDES_RIGHT == closest_orth_edge));

      const bool f_relevant = (f_is_ctca_or_fast_moving_obj &&
         f_dist_to_obj_is_small_enough &&
         f_obj_hdg_is_parallell_to_host &&
         f_is_any_long_side_visible && 
          f_close_enough_x_direction);
      return f_relevant;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Ext_Bounding_Boxes()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track
   * const F360_Calibrations_T & calibs
   * const F360_Visible_Edges_Type_T closest_orth_edge
   * BoundingBox & fcm_ext_box_on_nearby_side
   * BoundingBox & ct_ext_box_on_farside
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
   * This function calculates the two bounding boxes used in close target
   * functionality (ct) and the farside countermeasure (fcm). The "ct" bounding
   * box is extended on the farside of the target considered. Detections inside
   * this box may be considered as close target. The "fcm" is extended on the nearby 
   * side of the target. Detections outside this box may be considered as not ok
   * to use and deassociated.
   *
   * PRECONDITIONS:
   * closest_orth_edge is a valid orth edge (left or right).
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Compute_Ext_Bounding_Boxes(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calibs,
      const F360_Object_Sides_T closest_orth_edge,
      BoundingBox & fcm_ext_box_on_nearby_side,
      BoundingBox & ct_ext_box_on_farside)
   {
      // Compute VCS origin in TCS
      Point tcs_vcs_origin;
      Convert_VCS_Posn_To_TCS_Posn(0.0F, 0.0F, object_track.bbox.Get_Center().x, object_track.bbox.Get_Center().y,
         object_track.bbox.Get_Orientation(), tcs_vcs_origin.x, tcs_vcs_origin.y);

      // Compute start and end point of orth edge
      Point tcs_start;
      Point tcs_end;
      const bool f_valid_edge = Get_Edge_TCS_Start_And_End_Point(object_track, closest_orth_edge, tcs_start, tcs_end);
      
      if (f_valid_edge)
      {
         // Compute distance to orth edge from VCS origin
         const float32_t closest_orth_edge_dist = Calculate_Distance_To_Edge(tcs_start, tcs_end, tcs_vcs_origin);

         if (F360_OBJECT_SIDES_LEFT == closest_orth_edge)
         {
            // The left side and edge corresponding to width1 is closest
            fcm_ext_box_on_nearby_side.Extend_Boundaries(
               object_track.lat_buffer_zone_wid1,
               0.0F,
               object_track.long_buffer_zone_len1,
               object_track.long_buffer_zone_len2);
            ct_ext_box_on_farside.Extend_Boundaries(
               0.0F, 
               calibs.k_ct_orth_buffer_zone_factor * closest_orth_edge_dist, 
               calibs.k_ct_para_buffer_zone_factor * closest_orth_edge_dist, 
               calibs.k_ct_para_buffer_zone_factor * closest_orth_edge_dist);
         }
         else
         {
            // The right side and edge corresponding to width2 is closest
            fcm_ext_box_on_nearby_side.Extend_Boundaries(
               0.0F,
               object_track.lat_buffer_zone_wid2, 
               object_track.long_buffer_zone_len1, 
               object_track.long_buffer_zone_len2);
            ct_ext_box_on_farside.Extend_Boundaries(
               calibs.k_ct_orth_buffer_zone_factor * closest_orth_edge_dist, 
               0.0F,
               calibs.k_ct_para_buffer_zone_factor * closest_orth_edge_dist, 
               calibs.k_ct_para_buffer_zone_factor * closest_orth_edge_dist);
         }
      }
      else
      {
         /* This should never happen, but if it does: 
         * - Make fcm box as accepting as possible on both sides. Detections should not be marked as not ok to use wrongly.
         * - Make ct as small as possible. Detections should not be marked as close target wrongly. Minimum size is considered to be the solid box.
         */
         fcm_ext_box_on_nearby_side.Extend_Boundaries(
            object_track.lat_buffer_zone_wid1,
            object_track.lat_buffer_zone_wid2,
            object_track.long_buffer_zone_len1,
            object_track.long_buffer_zone_len2);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_Relevant_For_Close_Target()
   *===========================================================================
   * RETURN VALUE:
   * bool f_relevant
   *
   * PARAMETERS:
   * const F360_Detection_Props_T & det_prop
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
   * This function determines if detection is relevant for the close target 
   * functionality (ct), i.e. unassociated to any track, not previously marked
   * as close target, not marked as wheel spin and not marked as double bounce.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Det_Relevant_For_Close_Target(
      const F360_Detection_Props_T & det_prop)
   {
      const bool f_relevant = ((det_prop.object_track_id == 0) &&
         (!det_prop.f_close_target) &&
         (F360_DETECTION_WHEELSPIN_TYPE_INVALID == det_prop.wheel_spin_type) &&
         (!det_prop.f_double_bounce));
      return f_relevant;
   }
}
