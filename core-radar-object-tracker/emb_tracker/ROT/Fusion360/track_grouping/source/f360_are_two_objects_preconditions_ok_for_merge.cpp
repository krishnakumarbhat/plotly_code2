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
#include "f360_bounding_box.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_math_func.h"
#include "f360_merge_bbox_overlap_test.h"
#include "f360_norm_heading_angle.h"
#include "f360_point.h"
#include <algorithm>

namespace f360_variant_A
{
static BoundingBox Create_Bounding_Box_On_Object_Detections(
   const F360_Object_Track_T &object, const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]);

static void Create_Points_On_Obj_Detections(Point (&det_points)[MAX_DETS_IN_OBJ_TRK], const F360_Object_Track_T &object,
                                            const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]);

static float32_t Calculate_Predicted_Compensated_Range_Rate(const F360_Detection_Props_T &det, const float32_t vx,
                                                            const float32_t vy, const float32_t omega, const float32_t rot_x,
                                                            const float32_t rot_y);

static float32_t Determine_Det_Gap_Ratio(const float32_t &k_min_allowed_detection_spread, const float32_t &min1,
                                         const float32_t &max1, const float32_t &min2, const float32_t &max2);

/*===========================================================================*\
* FUNCTION: Calculate_Predicted_Compensated_Range_Rate()
*===========================================================================
* RETURN VALUE:
* float32_t rr_pred - predicted compensated range rate
*
* PARAMETERS:
* const F360_Detection_Props_T& det - detection properties
* float32_t vx - longitudinal velocity
* float32_t vy - lateral velocity
* float32_t omega - yaw rate
* float32_t rot_x - rotation center x-coordinate
* float32_t rot_y - rotation center y-coordinate
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
* Computes the predicted compensated range rate for a detection given the
* object's velocity and yaw rate. Formula: vx*cos_az + vy*sin_az + omega*(dx*sin_az - dy*cos_az)
\*===========================================================================*/
static float32_t Calculate_Predicted_Compensated_Range_Rate(const F360_Detection_Props_T &det, const float32_t vx,
                                                            const float32_t vy, const float32_t omega, const float32_t rot_x,
                                                            const float32_t rot_y)
{
   const float32_t dx = det.vcs_position.x - rot_x;
   const float32_t dy = det.vcs_position.y - rot_y;
   return vx * det.cos_vcs_az + vy * det.sin_vcs_az + omega * (dx * det.sin_vcs_az - dy * det.cos_vcs_az);
}

/*===========================================================================*\
* FUNCTION: Are_RRs_Consistent_With_Low_Curvature()
*===========================================================================
* RETURN VALUE:
* bool - true if range rates at longitudinal extremes are consistent with
*        low-curvature (straight-motion) hypothesis, false otherwise
*
* PARAMETERS:
* const F360_Object_Track_T& obj1, obj2 - objects being merged
* const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
* uint32_t min1_para_det_idx, max1_para_det_idx - extreme detection indices for obj1
* uint32_t min2_para_det_idx, max2_para_det_idx - extreme detection indices for obj2
* float32_t min1_para, max1_para, min2_para, max2_para - longitudinal extents
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
* Validates that the range rates at the longitudinally extreme detections
* are consistent with the low-curvature (straight-motion) hypothesis. Uses
* the object with smaller absolute curvature as the reference motion model.
\*===========================================================================*/
bool Are_RRs_Consistent_With_Low_Curvature(const F360_Object_Track_T &obj1, const F360_Object_Track_T &obj2,
                                           const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                           const uint32_t min1_para_det_idx, const uint32_t max1_para_det_idx,
                                           const uint32_t min2_para_det_idx, const uint32_t max2_para_det_idx,
                                           const float32_t min1_para, const float32_t max1_para, const float32_t min2_para,
                                           const float32_t max2_para)
{
   bool f_rrs_consistent = false;

   // Reference motion state: use the object with the smaller absolute curvature,
   // as its states better represent the straight-motion hypothesis.
   const F360_Object_Track_T &obj_ref = (std::abs(obj1.curvature) <= std::abs(obj2.curvature)) ? obj1 : obj2;

   const float32_t vx    = obj_ref.vcs_velocity.longitudinal;
   const float32_t vy    = obj_ref.vcs_velocity.lateral;
   const float32_t omega = obj_ref.curvature * obj_ref.speed; // assumed yaw rate

   // Get center of rotation: for CTCA objects, it's the center of the rear edge. For CCA it's the center of the object.
   float32_t rot_x;
   float32_t rot_y;
   if (F360_TRACKER_TRKFLTR_CTCA == obj_ref.trk_fltr_type)
   {
      const Point rear_center_tcs_pos(-0.5F * obj_ref.bbox.Get_Length(), 0.0F);
      Convert_TCS_Posn_To_VCS_Posn(rear_center_tcs_pos.x, rear_center_tcs_pos.y, obj_ref.bbox.Get_Center().x,
                                   obj_ref.bbox.Get_Center().y, obj_ref.bbox.Get_Orientation(), rot_x, rot_y);
   }
   else
   {
      rot_x = obj_ref.bbox.Get_Center().x;
      rot_y = obj_ref.bbox.Get_Center().y;
   }

   // Detections at the global longitudinal extremes (0-based indices into detection_props)
   const uint32_t global_min_det_idx = (min1_para <= min2_para) ? min1_para_det_idx : min2_para_det_idx;
   const uint32_t global_max_det_idx = (max1_para >= max2_para) ? max1_para_det_idx : max2_para_det_idx;

   // Check range rate errors at both longitudinal extremes
   const F360_Detection_Props_T &det_min = detection_props[global_min_det_idx];
   const float32_t rr_pred_min           = Calculate_Predicted_Compensated_Range_Rate(det_min, vx, vy, omega, rot_x, rot_y);
   const float32_t rr_error_min          = std::abs(det_min.range_rate_compensated - rr_pred_min);

   const F360_Detection_Props_T &det_max = detection_props[global_max_det_idx];
   const float32_t rr_pred_max           = Calculate_Predicted_Compensated_Range_Rate(det_max, vx, vy, omega, rot_x, rot_y);
   const float32_t rr_error_max          = std::abs(det_max.range_rate_compensated - rr_pred_max);

   constexpr float32_t k_max_rr_error = 0.5F;
   f_rrs_consistent                   = (rr_error_min < k_max_rr_error) && (rr_error_max < k_max_rr_error);

   return f_rrs_consistent;
}

/*===========================================================================*\
 * FUNCTION: Determine_Det_Gap_Ratio()
 *===========================================================================
 * RETURN VALUE:
 * float32_t gap_ratio
 *
 * PARAMETERS:
 * const float32_t & k_min_allowed_detection_spread
 * const float32_t & min1
 * const float32_t & max1
 * const float32_t & min2
 * const float32_t & max2
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
 * Calculates the gap ratio between two detections of obj1 and obj2.
 * The gap is calculated along a single axis (either longitudinal or orthogonal).
 * The gap ratio is defined as the gap between
 * the detection clusters divided by the total spread of the clusters.
 *
 * The function computes:
 * - Total spread: distance from the global minimum to global maximum position
 * - Gap: distance between the two clusters if they don't overlap (zero otherwise)
 * - Gap ratio: gap / spread (if spread exceeds min_spread_for_ratio threshold)
 *
 * A gap ratio of zero indicates overlapping clusters (favorable for merge), while
 * a high ratio indicates separated clusters (unfavorable for merge). The
 * min_spread_for_ratio threshold prevents unstable ratios for tightly clustered detections.
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * None
 \*===========================================================================*/
static float32_t Determine_Det_Gap_Ratio(const float32_t &k_min_allowed_detection_spread, const float32_t &min1,
                                         const float32_t &max1, const float32_t &min2, const float32_t &max2)
{
   // For para calculation: /Calculate longitudinal (parallel) spread and gap
   // For orth calculation: Calculate orthogonal (lateral) spread and gap
   const float32_t global_min = std::min(min1, min2);
   const float32_t global_max = std::max(max1, max2);
   const float32_t det_spread = global_max - global_min;

   // Calculate gap between longitudinal/orthogonal detection clusters
   // Gap exists only if clusters don't overlap
   float32_t det_gap;
   if (max1 < min2)
   {
      // For para calculation: obj1 is rear, obj2 is front
      // For orth calculation: obj1 detections on one side, obj2 on the other
      det_gap = min2 - max1;
   }
   else if (max2 < min1)
   {
      // For para calculation: obj2 is rear, obj1 is front
      // For orth calculation: obj2 detections on one side, obj1 on the other
      det_gap = min1 - max2;
   }
   else
   {
      // For para calculation: clusters overlap longitudinally - no gap (good for merge)
      // For orth calculation: clusters overlap laterally - no gap (good for merge)
      det_gap = 0.0F;
   }

   // For para calculation:  Calculate longitudinal gap ratio (gap as fraction of total spread)
   // For orth calculation:  Calculate lateral gap ratio (gap as fraction of total spread)
   float32_t gap_ratio;
   if (det_spread > k_min_allowed_detection_spread)
   {
      gap_ratio = det_gap / det_spread;
   }
   else
   {
      gap_ratio = 0.0F;
   }

   return gap_ratio;
}

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
bool Are_Two_Objects_Preconditions_OK_For_Merge(const F360_Calibrations_T &calib, const F360_Object_Track_T &first_object,
                                                const F360_Object_Track_T &second_object,
                                                const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                                float32_t &coarse_gate_score)
{
   const bool f_coarse_gate_ok = Are_Merging_Coarse_Gate_Conditions_Met(
      first_object, second_object, detection_props, calib.k_track_grouping_hdg_gate, calib.k_track_grouping_speed_gate,
      calib.k_track_grouping_curvature_gate, calib.k_merging_coarse_score_gate, coarse_gate_score);

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
bool Merge_Metal_To_Metal_Test(const F360_Object_Track_T &first_object, const F360_Object_Track_T &second_object,
                               const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                               const F360_Calibrations_T &calib)
{
   bool conditions_valid;
   if ((first_object.speed < calib.merging_m2m_max_obj_speed) || (second_object.speed < calib.merging_m2m_max_obj_speed))
   {
      const BoundingBox bbox_A                          = Create_Bounding_Box_On_Object_Detections(first_object, detection_props);
      const BoundingBox bbox_B                          = Create_Bounding_Box_On_Object_Detections(second_object, detection_props);
      const Distance_Between_Bboxes dist_between_bboxes = bbox_A.Combined_Distance_To(bbox_B);

      const bool metal_to_metal_condition_valid = (dist_between_bboxes.closest_distance < calib.merging_m2m_distance_threshold);
      const bool lateral_spread_condtition_valid =
         (dist_between_bboxes.lateral_spread < calib.merging_lateral_det_spread_threshold);
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
   const F360_Object_Track_T &object, const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS])
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
static void Create_Points_On_Obj_Detections(Point (&det_points)[MAX_DETS_IN_OBJ_TRK], const F360_Object_Track_T &object,
                                            const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS])
{
   for (uint32_t det_index = 0U; det_index < object.ndets; det_index++)
   {
      const F360_Detection_Props_T &detection = detection_props[object.detids[det_index] - 1U];
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
* const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
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
bool Are_Merging_Coarse_Gate_Conditions_Met(const F360_Object_Track_T &first_object, const F360_Object_Track_T &second_object,
                                            const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                            const float32_t heading_gate, const float32_t speed_gate,
                                            const float32_t curvature_gate, const float32_t coarse_score_gate,
                                            float32_t &coarse_gate_score)
{
   float32_t distance_threshold        = first_object.bbox.Get_Length() + second_object.bbox.Get_Length();
   float32_t speed_threshold           = speed_gate;
   const float32_t low_speed_threshold = 4.0F;
   const float32_t abs_mean_speed      = std::abs(0.5F * (first_object.speed + second_object.speed));
   if (abs_mean_speed < low_speed_threshold)
   {
      constexpr float32_t distance_threshold_mul = 0.9F;
      constexpr float32_t speed_threshold_mul    = 0.75F;

      distance_threshold *= distance_threshold_mul;
      speed_threshold *= speed_threshold_mul;
   }

   const bool both_objects_moving = (first_object.f_moving) && (second_object.f_moving);
   const bool both_objects_not_moveable =
      (first_object.movable_prob < MOVABLE_PROB_THRESHOLD) && (second_object.movable_prob < MOVABLE_PROB_THRESHOLD);
   const bool both_moving_or_both_not_moveable = both_objects_moving || both_objects_not_moveable;

   const float32_t distance_diff = std::max(std::abs(first_object.bbox.Get_Center().x - second_object.bbox.Get_Center().x),
                                            std::abs(first_object.bbox.Get_Center().y - second_object.bbox.Get_Center().y));
   const bool f_distance_ok      = distance_diff < distance_threshold;

   const float32_t normalized_heading =
      Normalize_Heading_Angle(second_object.vcs_heading.Value(),
                              first_object.vcs_heading.Value()); // Makes second object vcs heading "near" first object heading.
   const float32_t heading_diff = std::abs(first_object.vcs_heading.Value() - normalized_heading);
   const bool f_heading_ok      = heading_diff < heading_gate;

   const float32_t speed_diff = std::abs(first_object.speed - second_object.speed);
   const bool f_speed_ok      = speed_diff < speed_threshold;

   const float32_t curvature_diff = std::abs(first_object.curvature - second_object.curvature);
   bool f_curvature_ok            = curvature_diff < curvature_gate;

   /* Relaxed curvature path: if all other checks pass but curvature fails, allow a larger curvature difference when
      detections support the merge, i.e. detections gaps are small and range rates are consistent with motion of the smallest
      curvature object. */
   constexpr float32_t min_speed_for_relaxed_curv_gate  = 5.0F;
   const bool f_speed_high_enough_for_relaxed_curv_gate = (abs_mean_speed > min_speed_for_relaxed_curv_gate);
   const bool f_any_object_below_curv_thresh =
      ((std::abs(first_object.curvature) < curvature_gate) || (std::abs(second_object.curvature) < curvature_gate));
   if ((!f_curvature_ok) && f_distance_ok && f_heading_ok && f_speed_ok && f_speed_high_enough_for_relaxed_curv_gate &&
       f_any_object_below_curv_thresh)
   {
      constexpr float32_t k_relaxed_curvature_gate_para_gap_ratio = 0.15F;
      constexpr float32_t k_relaxed_curvature_gate_orth_gap_ratio = 0.1F;
      const bool f_perform_range_rate_consistency_check           = true;

      const bool f_dets_support_merge =
         Check_If_Detections_Support_Merge(first_object, second_object, detection_props, k_relaxed_curvature_gate_para_gap_ratio,
                                           k_relaxed_curvature_gate_orth_gap_ratio, f_perform_range_rate_consistency_check);

      if (f_dets_support_merge)
      {
         f_curvature_ok = curvature_diff < (curvature_gate * 2.0F);
      }
   }

   const float32_t score = distance_diff / distance_threshold + heading_diff / heading_gate + speed_diff / speed_threshold +
                           curvature_diff / curvature_gate;
   coarse_gate_score = score;

   return (both_moving_or_both_not_moveable && f_distance_ok && f_heading_ok && f_speed_ok && f_curvature_ok &&
           ((abs_mean_speed > low_speed_threshold) || (score < coarse_score_gate)));
}

/*===========================================================================*\
* FUNCTION: Check_If_Detections_Support_Merge()
*===========================================================================
* RETURN VALUE:
* bool f_dets_support_merge
*
* PARAMETERS:
* const F360_Object_Track_T & obj1
* const F360_Object_Track_T & obj2
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
* Verifies detection continuity between two objects being considered for merge.
* Projects all detections from both objects onto a longitudinal axis and
* calculates the total spread and gap between detection clusters. This helps
* identify suspicious merges where two separate objects (e.g., two bicyclists)
* would be incorrectly merged.
*
* PRECONDITIONS:
* Both objects must have valid associated detections
*
* POSTCONDITIONS:
* Returns structure with continuity metrics
\*===========================================================================*/
bool Check_If_Detections_Support_Merge(const F360_Object_Track_T &obj1, const F360_Object_Track_T &obj2,
                                       const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                       const float32_t k_max_allowed_para_gap_to_len_ratio,
                                       const float32_t k_max_allowed_orth_gap_to_width_ratio,
                                       const bool f_check_rr_straight_motion)
{
   const float32_t k_min_allowed_detection_spread =
      1.0F; /* minimum allowed detection spread threshold in either para or orth direction to avoid unstable ratios while computing
               gap to spread ratio */

   // Transform detections from obj1 to TCS and get longitudinal (x) and orthogonal (y) extents
   float32_t min1_para;
   float32_t max1_para;
   float32_t min1_orth;
   float32_t max1_orth;
   uint32_t min1_para_det_idx = obj1.detids[0];
   uint32_t max1_para_det_idx = obj1.detids[0];
   Determine_Min_Max_Det_POS_in_Object_TCS(obj1.bbox.Get_Center(), obj1.bbox.Get_Orientation(), obj1.ndets, obj1.detids,
                                           detection_props, min1_para, max1_para, min1_orth, max1_orth, min1_para_det_idx,
                                           max1_para_det_idx);

   // Transform detections from obj2 to TCS and get longitudinal (x) and orthogonal (y) extents
   float32_t min2_para;
   float32_t max2_para;
   float32_t min2_orth;
   float32_t max2_orth;
   uint32_t min2_para_det_idx = obj2.detids[0];
   uint32_t max2_para_det_idx = obj2.detids[0];
   Determine_Min_Max_Det_POS_in_Object_TCS(obj1.bbox.Get_Center(), obj1.bbox.Get_Orientation(), obj2.ndets, obj2.detids,
                                           detection_props, min2_para, max2_para, min2_orth, max2_orth, min2_para_det_idx,
                                           max2_para_det_idx);

   // Calculate longitudinal gap ratio(gap as fraction of total spread)
   const float32_t para_gap_ratio =
      Determine_Det_Gap_Ratio(k_min_allowed_detection_spread, min1_para, max1_para, min2_para, max2_para);

   // Calculate lateral gap ratio (gap as fraction of total spread)
   const float32_t orth_gap_ratio =
      Determine_Det_Gap_Ratio(k_min_allowed_detection_spread, min1_orth, max1_orth, min2_orth, max2_orth);

   // Check both longitudinal and orthogonal gaps
   bool f_dets_support_merge =
      (para_gap_ratio < k_max_allowed_para_gap_to_len_ratio) && (orth_gap_ratio < k_max_allowed_orth_gap_to_width_ratio);

   if (f_check_rr_straight_motion && f_dets_support_merge)
   {
      f_dets_support_merge =
         Are_RRs_Consistent_With_Low_Curvature(obj1, obj2, detection_props, min1_para_det_idx, max1_para_det_idx,
                                               min2_para_det_idx, max2_para_det_idx, min1_para, max1_para, min2_para, max2_para);
   }

   return f_dets_support_merge;
}

/*===========================================================================*\
 * FUNCTION: Determine_Min_Max_Det_POS_in_Object_TCS()
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const Point& object1_center_vcs_position
 * const Angle& object1_vcs_pointing
 * const uint32_t obj_ndets
 * const uint32_t(&obj_detids)[MAX_DETS_IN_OBJ_TRK]
 * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
 * float32_t& min_para
 * float32_t& max_para
 * float32_t& min_orth
 * float32_t& max_orth
 * uint32_t& min_para_det_idx
 * uint32_t& max_para_det_idx
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
 * Transforms all detections passed to the function from VCS to the object's TCS positions
 * From the transformed positions it determines the minimum and maximum longitudinal (para) and
 * orthogonal (orth) positions of the detections in the object's TCS.
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * None
 \*===========================================================================*/
void Determine_Min_Max_Det_POS_in_Object_TCS(const Point &object1_center_vcs_position, const Angle &object1_vcs_pointing,
                                             const uint32_t obj_ndets, const uint32_t (&obj_detids)[MAX_DETS_IN_OBJ_TRK],
                                             const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                             float32_t &min_para, float32_t &max_para, float32_t &min_orth, float32_t &max_orth,
                                             uint32_t &min_para_det_idx, uint32_t &max_para_det_idx)
{
   // Transform detections from obj2 to TCS and get longitudinal (x) and orthogonal (y) extents
   // The TCS position of detections should always be within 1000m, with respect to this usecase
   min_para = 1000.0F;
   max_para = -1000.0F;
   min_orth = 1000.0F;
   max_orth = -1000.0F;

   for (uint32_t i = 0U; i < obj_ndets; ++i)
   {
      const uint32_t raw_id                   = obj_detids[i];
      const uint32_t det_idx                  = raw_id - 1U;
      const F360_Detection_Props_T &det_props = detection_props[det_idx];

      float32_t det_tcs_x; // Longitudinal
      float32_t det_tcs_y; // Orthogonal

      // Transform to obj1's TCS
      Convert_VCS_Posn_To_TCS_Posn(det_props.vcs_position.x, det_props.vcs_position.y, object1_center_vcs_position.x,
                                   object1_center_vcs_position.y, object1_vcs_pointing, det_tcs_x, det_tcs_y);

      if (det_tcs_x < min_para)
      {
         min_para         = det_tcs_x;
         min_para_det_idx = det_idx;
      }
      if (det_tcs_x > max_para)
      {
         max_para         = det_tcs_x;
         max_para_det_idx = det_idx;
      }
      min_orth = std::min(min_orth, det_tcs_y);
      max_orth = std::max(max_orth, det_tcs_y);
   }
}
} // namespace f360_variant_A
