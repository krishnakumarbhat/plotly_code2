/*===================================================================================*\
* FILE: f360_update_object_reference_point_support_functions.cpp
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function support functions related to updating an objects 
*   reference point.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_update_object_reference_point_support_functions.h"
#include "f360_reference_point_support_functions.h"
#include "f360_bounding_box.h"
#include "f360_math_func.h"
#include "f360_norm_heading_angle.h"
#include "f360_get_reference_point_para_side.h"
#include "f360_get_reference_point_orth_side.h"
#include "f360_trk_fltr_ctca_states.h"

namespace f360_variant_A
{
   static inline float32_t Compute_Dot_Product(
      const float32_t(&vec_A)[2],
      const float32_t(&vec_B)[2]);

   static inline float32_t Tune_Hysteresis_Treshold(
      const F360_Calibrations_T& calib,
      const float32_t& object_length);

   /*===========================================================================*\
   * FUNCTION: Update_Object_KF_States_After_Reference_Point_Change()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Object_Track_T& obj  - Object structure
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function updates the object's KF states to reflect a change in reference point.
   * The vcs position of the object is changed such that it corresponds to the position
   * of the new reference point without shifting the position of the object bbox (i.e. 
   * bbox center is unchanged). In addition, for CTCA objects the position uncertainty of
   * the object is increased when the reference point is changed.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Update_Object_KF_States_After_Reference_Point_Change(
      F360_Object_Track_T& obj)
   {
      // Store previous position temporarily
      Point old_ref_point_pos;
      old_ref_point_pos.x = obj.vcs_position.x;
      old_ref_point_pos.y = obj.vcs_position.y;

      // Get the position of the reference point in TCS
      Point ref_point_pos_tcs;
      ref_point_pos_tcs = Get_Reference_Point_Pos_In_TCS(obj.reference_point, obj.bbox.Get_Length(), obj.bbox.Get_Width());

      // Transform the reference point's position to VCS
      Point new_ref_point_pos_vcs;
      const float32_t cos_pointing = obj.bbox.Get_Orientation().Cos();
      const float32_t sin_pointing = obj.bbox.Get_Orientation().Sin();

      new_ref_point_pos_vcs.x = obj.bbox.Get_Center().x + ref_point_pos_tcs.x * cos_pointing - ref_point_pos_tcs.y * sin_pointing;
      new_ref_point_pos_vcs.y = obj.bbox.Get_Center().y + ref_point_pos_tcs.x * sin_pointing + ref_point_pos_tcs.y * cos_pointing;

      // Assign new reference point's position to object position and predicted position
      obj.vcs_position.x = new_ref_point_pos_vcs.x;
      obj.vcs_position.y = new_ref_point_pos_vcs.y;
      obj.predicted_vcs_position.x += new_ref_point_pos_vcs.x - old_ref_point_pos.x;
      obj.predicted_vcs_position.y += new_ref_point_pos_vcs.y - old_ref_point_pos.y;
   }

   /*===========================================================================*\
    * FUNCTION: Increase_Object_Pos_Uncertainty_After_Reference_Point_Change()
    *===========================================================================
    * RETURN VALUE:
    * None.
    *
    * PARAMETERS:
    * const F360_Reference_Point_T old_ref_pnt
    * F360_Object_Track_T& obj
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function increases the uncertainty of object position (P-matrix) for CTCA objects
    * when a reference point switch has occured.
    *
    * PRECONDITIONS:
    * None.
    *
    * POSTCONDITIONS:
    * None.
    *
   \*===========================================================================*/
   void Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(
      const F360_Reference_Point_T old_ref_pnt,
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj)
   {
      if ((old_ref_pnt != F360_REFERENCE_POINT_CENTER) && (obj.trk_fltr_type == F360_TRACKER_TRKFLTR_CTCA))
      {
         // Extract which sides that were previously and are currently visible
         const F360_Object_Sides_T prev_rear_front_side = Get_Reference_Point_Para_Side(old_ref_pnt);
         const F360_Object_Sides_T prev_right_left_side = Get_Reference_Point_Orth_Side(old_ref_pnt);
         const F360_Object_Sides_T curr_rear_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
         const F360_Object_Sides_T curr_right_left_side = Get_Reference_Point_Orth_Side(obj.reference_point);

         // Compute the position uncertainty increase in TCS coordinate system
         float32_t pos_cov_increase_mat_tcs[2][2] = {};
         if (prev_rear_front_side != curr_rear_front_side) // Reference point has shifted in para (length) direction
         {

            const bool f_move_whole_length = ((F360_OBJECT_SIDES_FRONT == prev_rear_front_side) && (F360_OBJECT_SIDES_REAR == curr_rear_front_side)) ||
               ((F360_OBJECT_SIDES_REAR == prev_rear_front_side) && (F360_OBJECT_SIDES_FRONT == curr_rear_front_side));
            const float32_t len_moved = (f_move_whole_length ? obj.bbox.Get_Length() : 0.5F * obj.bbox.Get_Length());
            const float32_t k_max_length_error = calibs.k_frac_of_moved_dist * len_moved;

            pos_cov_increase_mat_tcs[0][0] = k_max_length_error * k_max_length_error;
            pos_cov_increase_mat_tcs[1][1] = len_moved * len_moved * calibs.k_sin_max_pointing_error_sq;

         }
         if (prev_right_left_side != curr_right_left_side) // Reference point has shifted in orth (width) direction
         {
            const bool f_move_whole_width = ((F360_OBJECT_SIDES_LEFT == prev_right_left_side) && (F360_OBJECT_SIDES_RIGHT == curr_right_left_side)) ||
               ((F360_OBJECT_SIDES_RIGHT == prev_right_left_side) && (F360_OBJECT_SIDES_LEFT == curr_right_left_side));
            const float32_t wid_moved = (f_move_whole_width ? obj.bbox.Get_Width() : 0.5F * obj.bbox.Get_Width());
            const float32_t k_max_width_error = calibs.k_frac_of_moved_dist * wid_moved;

            pos_cov_increase_mat_tcs[1][1] = k_max_width_error * k_max_width_error;
            pos_cov_increase_mat_tcs[0][0] = wid_moved * wid_moved * calibs.k_sin_max_pointing_error_sq;
         }

         // Rotate position uncertainty increment to VCS coordinate system
         float32_t pos_cov_increase_mat_vcs[2][2] = {};
         Rotate_2D_Covariance_Matrix(obj.bbox.Get_Orientation().Cos(),
            obj.bbox.Get_Orientation().Sin(),
            pos_cov_increase_mat_tcs,
            pos_cov_increase_mat_vcs);

         // Add the increment to object position state uncertainty
         obj.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] += pos_cov_increase_mat_vcs[0][0];
         obj.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] += pos_cov_increase_mat_vcs[1][1];
         obj.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y] += pos_cov_increase_mat_vcs[0][1];
         obj.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_X] = obj.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y];
      }
   }

   /*===========================================================================*\
   * FUNCTION: Apply_Reference_Point_Hysteresis()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const float32_t(&normalized_ref_point_vectors)[8][2]
   *  const float32_t(&vec_from_host_to_obj)[2]
   *  const float32_t& k_reference_point_hysteresis_factor
   *  const F360_Reference_Point_T& curr_ref_point_with_min_proj_val
   *  const float32_t& proj_val_of_curr_reference_point
   *  const F360_Reference_Point_T& prev_reference_point
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function applies hysteresis to the refrence point by ensuring that
   * the projected value is better than the previous reference point by some
   * amount according to a hysteresis factor.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Apply_Reference_Point_Hysteresis(
      const F360_Calibrations_T& calibrations,
      const F360_Object_Track_T& obj,
      Reference_Point_Data& ref_pnt_ignoring_visibility,
      Reference_Point_Data_Ext& ref_pnt_considering_visibility)
   {
      const float32_t hysteresis_factor = Tune_Hysteresis_Treshold(calibrations, obj.bbox.Get_Length());

      Apply_Reference_Point_Hysteresis_Considering_Visibility(hysteresis_factor, ref_pnt_considering_visibility);

      Apply_Reference_Point_Hysteresis_Ignoring_Visibility(hysteresis_factor, ref_pnt_considering_visibility, ref_pnt_ignoring_visibility);
   }

   /*===========================================================================*\
   * FUNCTION: Apply_Reference_Point_Hysteresis_Considering_Visibility()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t hysteresis_factor,
   * Reference_Point_Data_Ext& ref_pnt_considering_visibility
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks if the hysteresis check for the reference point that
   * considers visibility is applicable, and if so, applies it.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Apply_Reference_Point_Hysteresis_Considering_Visibility(
      const float32_t hysteresis_factor,
      Reference_Point_Data_Ext& ref_pnt_considering_visibility)
   {
      const bool f_bypass_hysteresis_considering_visibility = ((F360_REFERENCE_POINT_CENTER == ref_pnt_considering_visibility.previous_point.reference_point) || (!ref_pnt_considering_visibility.previous_point.f_visible));

      if (f_bypass_hysteresis_considering_visibility)
      {
         // Do nothing, hysteresis check not applicable
      }
      else
      {
         Choose_New_Reference_Point_With_Hysteresis(hysteresis_factor, ref_pnt_considering_visibility.previous_point, ref_pnt_considering_visibility.new_point);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Apply_Reference_Point_Hysteresis_Ignoring_Visibility()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t hysteresis_factor,
   * const Reference_Point_Data_Ext& ref_pnt_considering_visibility,
   * Reference_Point_Data& ref_pnt_ignoring_visibility
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks if the hysteresis check for the reference point that
   * ignores visibility is applicable, and if so, applies it.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Apply_Reference_Point_Hysteresis_Ignoring_Visibility(
      const float32_t hysteresis_factor,
      const Reference_Point_Data_Ext& ref_pnt_considering_visibility,
      Reference_Point_Data& ref_pnt_ignoring_visibility)
   {
      const bool f_obj_not_on_fov_edge = ref_pnt_considering_visibility.f_all_new_ref_pnt_candidates_visible;
      const bool f_bypass_hysteresis_ignoring_visibility = (F360_REFERENCE_POINT_CENTER == ref_pnt_ignoring_visibility.previous_point.reference_point);

      if (f_obj_not_on_fov_edge)
      {
         ref_pnt_ignoring_visibility.new_point.reference_point = ref_pnt_considering_visibility.new_point.reference_point;
      }
      else if (f_bypass_hysteresis_ignoring_visibility)
      {
         // Do nothing, hysteresis check not applicable
      }
      else
      {
         Choose_New_Reference_Point_With_Hysteresis(hysteresis_factor, ref_pnt_ignoring_visibility.previous_point, ref_pnt_ignoring_visibility.new_point);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Tune_Hysteresis_Treshold()
   *===========================================================================
   * RETURN VALUE:
   * float32_t hysteresis_threshold
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib - Calibration structure
   * const F360_Object_Track_T& obj    - Object structure
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function tunes the hysteresis factor based on object length.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static inline float32_t Tune_Hysteresis_Treshold(
      const F360_Calibrations_T& calib,
      const float32_t& object_length)
   {
      const float32_t hysteresis_threshold = F360_Linear_Equation_With_Saturation(object_length,
         calib.k_normal_obj_length_thr,
         calib.k_long_obj_length_thr,
         calib.k_normal_obj_ref_pnt_hysteresis_factor,
         calib.k_long_obj_ref_pnt_hysteresis_factor);

      return hysteresis_threshold;
   }

   /*===========================================================================*\
   * FUNCTION: Choose_New_Reference_Point_With_Hysteresis()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t& k_reference_point_hysteresis_factor,
   * const Reference_Point_Candidate& prev_ref_point,
   * Reference_Point_Candidate& new_ref_point
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Given a new, a previous reference point, and their respective projection 
   * value, this function will apply hysteresis on the new reference point to
   * to ensure the new reference point is better with some margin compared to 
   * the old. If it is not better, the new reference point will be substituted
   * with the previous.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Choose_New_Reference_Point_With_Hysteresis(
      const float32_t& k_reference_point_hysteresis_factor,
      const Reference_Point_Candidate& prev_ref_point,
      Reference_Point_Candidate& new_ref_point)
   {
      const bool f_pass_hysteresis = new_ref_point.projection_value < prev_ref_point.projection_value * k_reference_point_hysteresis_factor;
      
      if (f_pass_hysteresis)
      {
         // Do nothing
      }
      else
      {
         // Did not pass hysteresis, pick previous reference point
         new_ref_point.reference_point = prev_ref_point.reference_point;
      }
   }


   /*===========================================================================*\
   * FUNCTION: Derive_Object_Reference_Point()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const bool f_current_object_behind_another_object          - Only allow corner reference points
   * const float32_t k_far_away_object_dist_sq_thr              - Distance squared, beyond which only side reference points are chosen
   * const float32_t host_dist_rear_axle_to_vcs_m               - Distance from rear axle to VCS origin [m]
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - Sensors
   * const F360_Globals_T& globals                              - Globals structure
   * const F360_Object_Track_T& object_track                    - Object structure
   * F360_Reference_Point_T & new_reference_point               - Default setting in center
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
   * The function calculates the reference point in VCS with the approach :
   * 1. Calculate unit vectors from object center to all possible reference
        points and normalize them.
   * 2. Calculate vector from host center to obj center
   * 3. Take scalar product of 1) and 2)
   * 4. If specific conditions regarding the position and the heading of the object are fullfilled
   * then multiply by 0.6 the scalar products for left and right
   * 5. Smallest (i.e. most negative) value decides reference point
   * 6. If the ref pnt with smallest value is not visible then the next best visible point is returned
   * 7. The ref pnt with the minimum projection (regardless of visibility) is stored in min_projection_refernce_point
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Derive_Object_Reference_Point(
      const bool f_current_object_behind_another_object,
      const float32_t k_far_away_object_dist_sq_thr,
      const float32_t host_dist_rear_axle_to_vcs_m,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const F360_Object_Track_T& object_track,
      Reference_Point_Data& ref_pnt_ignoring_visibility,
      Reference_Point_Data_Ext& ref_pnt_considering_visibility)
   {      
      const uint8_t num_relevant_ref_points = Get_Number_Of_Relevant_Reference_Points(k_far_away_object_dist_sq_thr, object_track.vcs_position);

      // f_current_object_behind_another_object indicates that the object is closely behind another object from host's point of view.
      // Force it to only choose from the corner reference points if that is the case.
      const uint8_t starting_ref_point_idx = (f_current_object_behind_another_object) ? 4U : 0U;
      float32_t normalized_ref_point_vectors[8][2];
      Compute_Side_And_Corner_Normalized_Reference_Point_Vectors(object_track, normalized_ref_point_vectors);

      // Compute a vector from host center to an object's center point
      float32_t vec_from_host_to_obj[2];
      const Point obj_center_vcs_pos = object_track.bbox.Get_Center();
      vec_from_host_to_obj[0] = obj_center_vcs_pos.x + 0.6F * host_dist_rear_axle_to_vcs_m;
      vec_from_host_to_obj[1] = obj_center_vcs_pos.y;

      float32_t ref_pnt_vec_proj_val[8];
      std::fill(cmn::begin(ref_pnt_vec_proj_val), cmn::end(ref_pnt_vec_proj_val), INFTY);

      bool ref_pnt_f_visible[8];

      // Setting up the conditions for decreasing the dot product for the sides
      const float32_t abs_vcs_heading = std::abs(object_track.vcs_heading.Value());
      const bool f_heading_is_ok = (abs_vcs_heading < F360_DEG2RAD(95.0F)) && (abs_vcs_heading > F360_DEG2RAD(45.0F));

      const float32_t obj_abs_pos_vcs_x = std::abs(object_track.vcs_position.x);
      const float32_t obj_abs_pos_vcs_y = std::abs(object_track.vcs_position.y);
      const bool f_position_is_ok = (obj_abs_pos_vcs_x > 5.0F) && (obj_abs_pos_vcs_y > 4.0F);
      
      const bool f_ok_to_disincentivise_side_refpnts = f_heading_is_ok && f_position_is_ok;

      std::fill(cmn::begin(ref_pnt_f_visible), cmn::end(ref_pnt_f_visible), false);
      for (uint8_t ref_pnt_idx = starting_ref_point_idx; ref_pnt_idx < num_relevant_ref_points; ref_pnt_idx++)
      {
          ref_pnt_vec_proj_val[ref_pnt_idx] = Compute_Dot_Product(normalized_ref_point_vectors[ref_pnt_idx], vec_from_host_to_obj);
          if (f_ok_to_disincentivise_side_refpnts)
          {
              if ((1U == ref_pnt_idx) || (3U == ref_pnt_idx)) // right or left side
              {   
                  // The best reference point is expected to be one with most negative value. So, by multiplying with the shrinking factor, 
                  // the left and right side projection values are made less negative i.e disincentivized
                  const float32_t shrinking_factor = 0.44F;
                  ref_pnt_vec_proj_val[ref_pnt_idx] = ref_pnt_vec_proj_val[ref_pnt_idx] * shrinking_factor;
              }
          }

         const F360_Reference_Point_T curr_ref_point = Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(ref_pnt_idx);
         ref_pnt_f_visible[ref_pnt_idx] = Is_Ref_Point_In_Sensors_FOV(curr_ref_point, object_track, sensors, globals);
      }

      Extract_Previous_Reference_Point_Data(object_track.reference_point, ref_pnt_vec_proj_val, ref_pnt_f_visible, ref_pnt_considering_visibility.previous_point);
      Extract_Previous_Reference_Point_Data(object_track.min_projection_reference_point, ref_pnt_vec_proj_val, ref_pnt_f_visible, ref_pnt_ignoring_visibility.previous_point);

      Find_Best_Reference_Point_Ignoring_Visibility(ref_pnt_vec_proj_val, num_relevant_ref_points, ref_pnt_ignoring_visibility.new_point);

      Find_Best_Reference_Point_Considering_Visibility(ref_pnt_vec_proj_val, ref_pnt_f_visible, num_relevant_ref_points, ref_pnt_considering_visibility.new_point);

      ref_pnt_considering_visibility.f_all_new_ref_pnt_candidates_visible = Are_All_Reference_Points_Visible(ref_pnt_f_visible, num_relevant_ref_points);
   }

   /*===========================================================================*\
   * FUNCTION: Extract_Previous_Reference_Point_Data()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Reference_Point_T& prev_reference_point,
   * const float32_t(&ref_pnt_vec_proj_val)[8],
   * const bool(&ref_pnt_f_visible)[8],
   * Reference_Point_Candidate& prev_reference_point_candidate
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function will extract reference point data for the previous reference
   * point.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Extract_Previous_Reference_Point_Data(
      const F360_Reference_Point_T& prev_reference_point,
      const float32_t(&ref_pnt_vec_proj_val)[8],
      const bool(&ref_pnt_f_visible)[8],
      Reference_Point_Candidate& prev_reference_point_candidate)
   {
      prev_reference_point_candidate.reference_point = prev_reference_point;

      if (F360_REFERENCE_POINT_CENTER != prev_reference_point)
      {
         const uint8_t prev_ref_pnt_idx = Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(prev_reference_point_candidate.reference_point);
         prev_reference_point_candidate.projection_value = ref_pnt_vec_proj_val[prev_ref_pnt_idx];
         prev_reference_point_candidate.f_visible = ref_pnt_f_visible[prev_ref_pnt_idx];
      }
      else
      {
         // No projection/visibility data to transfer, reference point hysteresis will not be applicable
      }
   }

   /*===========================================================================*\
   * FUNCTION: Are_All_Reference_Points_Visible()
   *===========================================================================
   * RETURN VALUE:
   * bool f_all_reference_points_visible
   *
   * PARAMETERS:
   * const bool(&ref_pnt_f_visible)[8],
   * const uint8_t num_relevant_ref_points
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function will return true if all reference points are visible, false 
   * if not.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Are_All_Reference_Points_Visible(
      const bool(&ref_pnt_f_visible)[8],
      const uint8_t num_relevant_ref_points)
   {
      bool f_all_reference_points_visible = true;
      for (uint8_t i = 0U; i < num_relevant_ref_points; i++)
      {
         f_all_reference_points_visible = f_all_reference_points_visible && ref_pnt_f_visible[i];
      }

      return f_all_reference_points_visible;
   }

   /*===========================================================================*\
   * FUNCTION: Find_Best_Reference_Point_Ignoring_Visibility()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t(&ref_pnt_vec_proj_val)[8]
   * const uint8_t num_relevant_ref_points
   * Reference_Point_Candidate& ref_point_with_best_projection_value
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Finds the reference point with the lowest projection value
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Find_Best_Reference_Point_Ignoring_Visibility(
      const float32_t(&ref_pnt_vec_proj_val)[8],
      const uint8_t num_relevant_ref_points,
      Reference_Point_Candidate& ref_point_with_best_projection_value)
   {
      const uint8_t ref_pnt_idx_with_min_projection_value = static_cast<uint8_t>(F360_Min_Index(ref_pnt_vec_proj_val, static_cast<uint32_t>(num_relevant_ref_points)));

      ref_point_with_best_projection_value.projection_value = ref_pnt_vec_proj_val[ref_pnt_idx_with_min_projection_value];
      ref_point_with_best_projection_value.reference_point = Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(ref_pnt_idx_with_min_projection_value);
   }

   /*===========================================================================*\
   * FUNCTION: Find_Best_Reference_Point_Considering_Visibility()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t(&ref_pnt_vec_proj_val)[8]
   * const uint8_t num_relevant_ref_points
   * Reference_Point_Candidate& ref_point_with_best_projection_value
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Finds the reference point with the lowest projection value that is also
   * visible
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Find_Best_Reference_Point_Considering_Visibility(
      const float32_t(&ref_pnt_vec_proj_val)[8],
      const bool(&ref_pnt_f_visible)[8],
      const uint8_t num_relevant_ref_points,
      Reference_Point_Candidate& ref_point_with_best_projection_value_and_visible)
   {
      // Must be unreasonable high value to not pass hysteresis check if no reference point is visible. Best projection value is negative
      ref_point_with_best_projection_value_and_visible.projection_value = 1000000.0F;

      for (uint8_t ref_pnt_idx = 0U; ref_pnt_idx < num_relevant_ref_points; ref_pnt_idx++)
      {
         const float32_t projection_value = ref_pnt_vec_proj_val[ref_pnt_idx];
         const bool f_reference_point_visible = ref_pnt_f_visible[ref_pnt_idx];

         if ((projection_value < ref_point_with_best_projection_value_and_visible.projection_value) && f_reference_point_visible)
         {
            ref_point_with_best_projection_value_and_visible.projection_value = projection_value;
            ref_point_with_best_projection_value_and_visible.reference_point = Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(ref_pnt_idx);
         }
      }
   }
   
   /*===========================================================================*\
   * FUNCTION: Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum()
   *===========================================================================
   * RETURN VALUE:
   * F360_Reference_Point_T ref_point_enum
   *
   * PARAMETERS:
   * const uint8_t ref_point_vector_idx
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
   * Function converts the reference point, type uint8_t, to reference point 
   * enumeration.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   F360_Reference_Point_T Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(const uint8_t ref_point_vector_idx)
   {
      // Note: these corresponds to the order the array is filled in the function Compute_Side_And_Corner_Normalized_Reference_Point_Vectors() 
      F360_Reference_Point_T ref_point_enum;
      switch (ref_point_vector_idx)
      {
         case 0U:
         {
            ref_point_enum = F360_REFERENCE_POINT_FRONT;
            break;
         }
         case 1U:
         {
            ref_point_enum = F360_REFERENCE_POINT_RIGHT;
            break;
         }
         case 2U:
         {
            ref_point_enum = F360_REFERENCE_POINT_REAR;
            break;
         }
         case 3U:
         {
            ref_point_enum = F360_REFERENCE_POINT_LEFT;
            break;
         }
         case 4U:
         {
            ref_point_enum = F360_REFERENCE_POINT_FRONT_LEFT;
            break;
         }
         case 5U:
         {
            ref_point_enum = F360_REFERENCE_POINT_FRONT_RIGHT;
            break;
         }
         case 6U:
         {
            ref_point_enum = F360_REFERENCE_POINT_REAR_RIGHT;
            break;
         }
         case 7U:
         {
            ref_point_enum = F360_REFERENCE_POINT_REAR_LEFT;
            break;
         }
         default:
         {
            ref_point_enum = F360_REFERENCE_POINT_CENTER;
            break;
         }
      }
      return ref_point_enum;
   }

   /*===========================================================================*\
   * FUNCTION: Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index()
   *===========================================================================
   * RETURN VALUE:
   * uint8_t ref_point_vector_idx
   *
   * PARAMETERS:
   * const F360_Reference_Point_T ref_point_enum
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
   * The function converts reference point enumeration to an unsigned char.
   *
   * enumeration.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   uint8_t Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(const F360_Reference_Point_T ref_point_enum)
   {
      // Note: this is the inverse of Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum()
      uint8_t ref_point_vector_idx; 
      switch (ref_point_enum)
      {
         case F360_REFERENCE_POINT_FRONT:
         {
            ref_point_vector_idx = 0U;
            break;
         }
         case F360_REFERENCE_POINT_RIGHT:
         {
            ref_point_vector_idx = 1U;
            break;
         }
         case F360_REFERENCE_POINT_REAR:
         {
            ref_point_vector_idx = 2U;
            break;
         }
         case F360_REFERENCE_POINT_LEFT:
         {
            ref_point_vector_idx = 3U;
            break;
         }
         case F360_REFERENCE_POINT_FRONT_LEFT:
         {
            ref_point_vector_idx = 4U;
            break;
         }
         case F360_REFERENCE_POINT_FRONT_RIGHT:
         {
            ref_point_vector_idx = 5U;
            break;
         }
         case F360_REFERENCE_POINT_REAR_RIGHT:
         {
            ref_point_vector_idx = 6U;
            break;
         }
         case F360_REFERENCE_POINT_REAR_LEFT:
         {
            ref_point_vector_idx = 7U;
            break;
         }
         default:
         {
            // Default rear 
            ref_point_vector_idx = 2U;
            break;
         }
      }
      return ref_point_vector_idx;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Side_And_Corner_Normalized_Reference_Point_Vectors()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object_track
   * float32_t (&normalized_ref_point_vectors)[8][2]
   *
   * EXTERNAL REFERENCES:
   * None
   *
   * DEVIATIONS FROM STANDARDS:
   * None
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *--------------------------------------------------------------------------
   * Computes normalized vectors from object center to object side and corner
   * reference points in VCS
   *
   * PRECONDITIONS :
   * None
   *
   * POSTCONDITIONS :
   * None
   *
   \*===========================================================================*/
   void Compute_Side_And_Corner_Normalized_Reference_Point_Vectors(
      const F360_Object_Track_T& object_track,
      float32_t (&normalized_ref_point_vectors)[8][2])
   {
      const float32_t cos_pointing = object_track.bbox.Get_Orientation().Cos();
      const float32_t sin_pointing = object_track.bbox.Get_Orientation().Sin();

      const float32_t half_width = 0.5F * object_track.bbox.Get_Width();
      const float32_t half_length = 0.5F * object_track.bbox.Get_Length();
      const float32_t corner_vector_length_inverse = 1.0F / F360_Sqrtf(half_length * half_length + half_width * half_width);

      // The order of these vectors is important
      // If changed, one need to update corresponding mapping functions
      normalized_ref_point_vectors[0][0] = cos_pointing; // Front
      normalized_ref_point_vectors[0][1] = sin_pointing;

      normalized_ref_point_vectors[1][0] = -sin_pointing; // Right
      normalized_ref_point_vectors[1][1] = cos_pointing;

      normalized_ref_point_vectors[2][0] = -cos_pointing; // Rear
      normalized_ref_point_vectors[2][1] = -sin_pointing;

      normalized_ref_point_vectors[3][0] = sin_pointing; // Left
      normalized_ref_point_vectors[3][1] = -cos_pointing;

      normalized_ref_point_vectors[4][0] = half_length * cos_pointing * corner_vector_length_inverse + half_width * sin_pointing * corner_vector_length_inverse; // Front left
      normalized_ref_point_vectors[4][1] = half_length * sin_pointing * corner_vector_length_inverse - half_width * cos_pointing * corner_vector_length_inverse;

      normalized_ref_point_vectors[5][0] = half_length * cos_pointing * corner_vector_length_inverse - half_width * sin_pointing * corner_vector_length_inverse; // Front right
      normalized_ref_point_vectors[5][1] = half_length * sin_pointing * corner_vector_length_inverse + half_width * cos_pointing * corner_vector_length_inverse;

      normalized_ref_point_vectors[6][0] = -half_length * cos_pointing * corner_vector_length_inverse - half_width * sin_pointing * corner_vector_length_inverse; // Rear right
      normalized_ref_point_vectors[6][1] = -half_length * sin_pointing * corner_vector_length_inverse + half_width * cos_pointing * corner_vector_length_inverse;

      normalized_ref_point_vectors[7][0] = -half_length * cos_pointing * corner_vector_length_inverse + half_width * sin_pointing * corner_vector_length_inverse; // Rear left
      normalized_ref_point_vectors[7][1] = -half_length * sin_pointing * corner_vector_length_inverse - half_width * cos_pointing * corner_vector_length_inverse;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Dot_Product()
   *===========================================================================
   * RETURN VALUE:
   * float32_t dot_product
   *
   * PARAMETERS:
   * const float32_t(&vec_A)[2]
   * const float32_t(&vec_B)[2]
   *
   * EXTERNAL REFERENCES:
   * None
   *
   * DEVIATIONS FROM STANDARDS:
   * None
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *--------------------------------------------------------------------------
   * Returns the dot product of two vectors.
   *
   * PRECONDITIONS :
   * None
   *
   * POSTCONDITIONS :
   * None
   *
   \*===========================================================================*/
   static inline float32_t Compute_Dot_Product(
      const float32_t(&vec_A)[2],
      const float32_t(&vec_B)[2])
   {
      const float32_t dot_product = vec_A[0] * vec_B[0] + vec_A[1] * vec_B[1];
      return dot_product;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Number_Of_Relevant_Reference_Points()
   *===========================================================================
   * RETURN VALUE:
   * float32_t num_relevant_ref_points
   *
   * PARAMETERS:
   * const float32_t k_far_away_object_dist_sq_thr
   * const Point& obj_vcs_position
   *
   * EXTERNAL REFERENCES:
   * None
   *
   * DEVIATIONS FROM STANDARDS:
   * None
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   *--------------------------------------------------------------------------
   * Based on the distance to object, checks if only side or side & corner
   * reference points shall be used.
   *
   * PRECONDITIONS :
   * None
   *
   * POSTCONDITIONS :
   * None
   *
   \*===========================================================================*/
   uint8_t Get_Number_Of_Relevant_Reference_Points(
      const float32_t k_far_away_object_dist_sq_thr,
      const Point& obj_vcs_position)
   {
      uint8_t num_relevant_ref_points;
      if (F360_Get_Hypotenuse_Squared(obj_vcs_position.x, obj_vcs_position.y) > k_far_away_object_dist_sq_thr)
      {
         // Object far away, only consider side reference points
         num_relevant_ref_points = 4U;
      }
      else
      {
         // Object close, consider side and corner reference points
         num_relevant_ref_points = 8U;
      }
      return num_relevant_ref_points;
   }

   /*===========================================================================*\
   * FUNCTION: Reference_Point_Blind_Spot_Countermeasure()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj                       
   * Reference_Point_Data_Ext& ref_pnt_considering_visibility
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function implements a blind spot countermeasure by forcing the reference
   * point to be on the visible orthogonal side when an object is detected to be in
   * sensor blind spot area. The blind spot area is for simplicity defined based
   * on the dead zone status of the object combined with a lateral threshold.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Reference_Point_Blind_Spot_Countermeasure(
      const F360_Object_Track_T& obj,
      Reference_Point_Data_Ext& ref_pnt_considering_visibility)
   {
      constexpr float32_t blind_spot_lateral_thr = 3.0F; // [m] Beyond this lateral threshold object is not considered to be in blind spot 
      // Only apply countermeasure for objects entering blind spot from the rear or when center position is inside.
      const bool f_valid_dead_zone_status = (obj.dead_zone_status == F360_Dead_Zone_Status_T::IN_REAR) || 
                                            (obj.dead_zone_status == F360_Dead_Zone_Status_T::INSIDE);

      if (f_valid_dead_zone_status && (std::abs(obj.bbox.Get_Center().y) < blind_spot_lateral_thr))
      {
         const F360_Object_Sides_T visible_orth_side = Get_Reference_Point_Orth_Side(obj.reference_point);
         if (visible_orth_side == F360_OBJECT_SIDES_LEFT)
         {
            ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_LEFT;
         }
         else if (visible_orth_side == F360_OBJECT_SIDES_RIGHT)
         {
            ref_pnt_considering_visibility.new_point.reference_point = F360_REFERENCE_POINT_RIGHT;
         }
         else
         {
            // Do nothing, MISRA
         }
      }
   }
}
