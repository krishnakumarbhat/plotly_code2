/*===================================================================================*\
* FILE: f360_are_two_objects_preconditions_ok_for_merge.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Are_Two_Objects_Preconditions_OK_For_Merge function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/


#include "f360_are_two_objects_preconditions_ok_for_merge.h"

#include "f360_math_func.h"
#include "f360_norm_heading_angle.h"
#include "f360_merge_bbox_overlap_test.h"
#include "f360_point.h"
#include "f360_bounding_box.h"
#include <algorithm>

namespace f360_variant_A
{
   static BoundingBox Create_Bounding_Box_On_Object_Detections(const F360_Object_Track_T & object, const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]);

   static void Create_Points_On_Obj_Detections(Point(&det_points)[MAX_DETS_IN_OBJ_TRK], const F360_Object_Track_T & object, const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]);

   /*===========================================================================*\
   * FUNCTION: Are_Two_Objects_Preconditions_OK_For_Merge()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib,
   * const F360_Object_Track_T & first_object,
   * const F360_Object_Track_T & second_object,
   * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Functions checks whether two objects meet preliminary conditions for merge.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Are_Two_Objects_Preconditions_OK_For_Merge(
      const F360_Calibrations_T & calib,
      const F360_Object_Track_T & first_object,
      const F360_Object_Track_T & second_object,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      float32_t & coarse_gate_score)
   {
      const bool f_coarse_gate_ok = Are_Merging_Coarse_Gate_Conditions_Met(
         first_object, second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);

      const bool f_bbox_overlap_test_ok = Merge_Bbox_Overlap_Test(first_object, second_object);

      const bool f_metal_to_metal_test_ok = Merge_Metal_To_Metal_Test(first_object, second_object, detection_props, calib);

      const bool f_preconditions_meet = (f_coarse_gate_ok && f_bbox_overlap_test_ok && f_metal_to_metal_test_ok);

      return f_preconditions_meet;
   }

   /*===========================================================================*\
   * FUNCTION: Merge_Metal_To_Metal_Test()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Object_Track_T & first_object,
   * const F360_Object_Track_T & second_object,
   * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * const F360_Calibrations_T & calib
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
   * Test performed only for slow moving objects. For objects above threshold function returns true.
   * Function creates bounding box spreaded over both objects' detections and calculate
   * two metrics of distance between bboxes:
   * - closest possible - distance AKA metal to metal distance
   * - lateral spread   - largest possible lateral distance between bboxes corners - this is only
   *                      calculated from first_object perspective.
   * If these values are below calibration values, then function returns true.
   * If object does not have any assigned detection, then created bbox is infinitly small and 
   * is located on object's center. 
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Merge_Metal_To_Metal_Test(
      const F360_Object_Track_T & first_object,
      const F360_Object_Track_T & second_object,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T & calib)
   {
      bool conditions_valid;
      if ((first_object.speed < calib.merging_m2m_max_obj_speed) || (second_object.speed < calib.merging_m2m_max_obj_speed))
      {
         const BoundingBox bbox_A = Create_Bounding_Box_On_Object_Detections(first_object, detection_props);
         const BoundingBox bbox_B = Create_Bounding_Box_On_Object_Detections(second_object, detection_props);
         const Distance_Between_Bboxes dist_between_bboxes = bbox_A.Combined_Distance_To(bbox_B);

         const bool metal_to_metal_condition_valid = (dist_between_bboxes.closest_distance < calib.merging_m2m_distance_threshold);
         const bool lateral_spread_condtition_valid = (dist_between_bboxes.lateral_spread < calib.merging_lateral_det_spread_threshold);
         conditions_valid = (metal_to_metal_condition_valid && lateral_spread_condtition_valid);
      }
      else
      {
         conditions_valid = true;
      }
      return conditions_valid;
   }

   /*===========================================================================*\
   * FUNCTION: Create_Bounding_Box_On_Object_Detections()
   *===========================================================================
   * RETURN VALUE:
   * BoundingBox
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object,
   * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Function creates bounding box spreaded over object detections. If there is none
   * detection assigned then created bbox is infinitly small and 
   * is located on object's center. 
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static BoundingBox Create_Bounding_Box_On_Object_Detections(
      const F360_Object_Track_T & object, 
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      BoundingBox bbox;
      if (object.ndets > 0U)
      {
         Point det_points_A[MAX_DETS_IN_OBJ_TRK] = {};
         Create_Points_On_Obj_Detections(det_points_A, object, detection_props);
         bbox = BoundingBox(det_points_A, object.ndets, object.bbox.Get_Orientation());
      }
      else // If there is no any detections associated, create infinite small bbox around object center
      {
         bbox = BoundingBox(object.bbox.Get_Center(), 0.0F, 0.0F, object.bbox.Get_Orientation());
      }
      return bbox;
   }

   /*===========================================================================*\
   * FUNCTION: Create_Points_On_Obj_Detections()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * Point(&det_points)[MAX_DETS_IN_OBJ_TRK],
   * const F360_Object_Track_T & object,
   * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Edit N Points which are stored in det_points array to be located in same place
   * as N detections whicha are associated to object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   static void Create_Points_On_Obj_Detections(
      Point(&det_points)[MAX_DETS_IN_OBJ_TRK], 
      const F360_Object_Track_T & object, 
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (uint32_t det_index = 0U; det_index < object.ndets; det_index++)
      {
         const F360_Detection_Props_T& detection = detection_props[object.detids[det_index] - 1U];
         det_points[det_index].Set_Position(detection.vcs_position.x, detection.vcs_position.y);
      }
   }
   /*===========================================================================*\
   * FUNCTION: Are_Merging_Coarse_Gate_Conditions_Met()
   *===========================================================================
   * RETURN VALUE:
   * bool gating_conditions_met
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object,
   * const F360_Object_Track_T & second_object,
   * const float32_t heading_gate,
   * const float32_t speed_gate
   * const float32_t curvature_gate
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
   * Checks whether coarse conditions for merging two objects are met:
   * - both objects are moving or both objects are not moveable
   * - distance between objects is low enough
   * - difference between objects' heading is below heading_gate value
   * - difference between objects' speed is below speed_gate value
   * - difference between objects' curvature is below curvature gate value
   * If the objects' speed is low (< 5m/s) also check score that is calcuated
   * as the sum of percentage differences from all the conditions above.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Are_Merging_Coarse_Gate_Conditions_Met(
      const F360_Object_Track_T & first_object,
      const F360_Object_Track_T & second_object,
      const float32_t heading_gate,
      const float32_t speed_gate,
      const float32_t curvature_gate,
      const float32_t coarse_score_gate,
      float32_t & coarse_gate_score)
   {
      float32_t distance_threshold = first_object.bbox.Get_Length() + second_object.bbox.Get_Length();
      float32_t speed_threshold = speed_gate;
      const float32_t low_speed_threshold = 4.0F;
      const float32_t abs_mean_speed = std::abs(0.5F * (first_object.speed + second_object.speed));
      if (abs_mean_speed < low_speed_threshold)
      {
         constexpr float32_t distance_threshold_mul = 0.9F;
         constexpr float32_t speed_threshold_mul = 0.75F;

         distance_threshold *= distance_threshold_mul;
         speed_threshold *= speed_threshold_mul;
      }

      const bool both_objects_moving = (first_object.f_moving) && (second_object.f_moving);
      const bool both_objects_not_moveable = (first_object.movable_prob < 0.5F) && (second_object.movable_prob < 0.5F);
      const bool both_moving_or_both_not_moveable = both_objects_moving || both_objects_not_moveable;

      const float32_t distance_diff = std::max(std::abs(first_object.bbox.Get_Center().x - second_object.bbox.Get_Center().x), std::abs(first_object.bbox.Get_Center().y - second_object.bbox.Get_Center().y));
      const bool f_distance_ok = distance_diff < distance_threshold;

      const float32_t normalized_heading = Normalize_Heading_Angle(second_object.vcs_heading.Value(), first_object.vcs_heading.Value()); // Makes second object vcs heading "near" first object heading. 
      const float32_t heading_diff = std::abs(first_object.vcs_heading.Value() - normalized_heading);
      const bool f_heading_ok = heading_diff < heading_gate;

      const float32_t speed_diff = std::abs(first_object.speed - second_object.speed);
      const bool f_speed_ok = speed_diff < speed_threshold;

      const float32_t curvature_diff = std::abs(first_object.curvature - second_object.curvature);
      const bool f_curvature_ok = curvature_diff < curvature_gate;

      const float32_t score = distance_diff / distance_threshold + heading_diff / heading_gate + speed_diff / speed_threshold + curvature_diff / curvature_gate;
      coarse_gate_score = score;

      return (both_moving_or_both_not_moveable &&
         f_distance_ok &&
         f_heading_ok &&
         f_speed_ok &&
         f_curvature_ok &&
         ((abs_mean_speed > low_speed_threshold) || (score < coarse_score_gate)));
   }
}
