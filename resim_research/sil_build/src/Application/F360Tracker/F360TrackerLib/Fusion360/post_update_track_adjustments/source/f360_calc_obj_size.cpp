/*===========================================================================*\
* FILE: f360_calc_obj_size.cpp
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Calc_Obj_Size()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/


#include <algorithm>

#include "f360_calc_obj_size.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_get_reference_point_para_side.h"
#include "f360_get_reference_point_orth_side.h"
#include "f360_iterator.h"
#include "f360_shrinking_supporting_functions.h"

namespace f360_variant_A
{
    static void Set_Default_Kalman_Noise(
        const F360_Calibrations_T& calib,
        const float32_t object_speed,
        const bool f_edge_visible,
        Kalman_Noise_T& noise);
   static void Limit_Size_And_Enforce_CTCA_Obj_Aspect_Ratio(
      const F360_Calibrations_T& calib,
      const Dimension_Limits_T& dim_limits,
      F360_Object_Track_T& obj);
   static void Update_Object_Length(
       const float32_t measured_length,
       const float32_t gain,
       const bool f_update_uncertainty,
       F360_Object_Track_T& obj);
   static void Update_Object_Width(
       const float32_t measured_width,
       const float32_t gain,
       const bool f_update_uncertainty,
       F360_Object_Track_T& obj);
   static void Apply_Detection_Based_Uncertainty_Scaling(
       const uint32_t object_ndets,
       const uint32_t min_dets_threshold,
       const uint32_t max_scale_limit,
       float32_t& measurement_uncertainty);

   /*===========================================================================*\
   * FUNCTION: Update_Noise_If_Obj_Outside_Front_Only_Fov()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const F360_Calibrations_T& calib,
   *  const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   *  const F360_Globals_T& globals,
   *  const F360_Object_Track_T& object_track,
   *  float32_t& process_noise
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
   * Update the process noise if an object next to the host is partially outside
   * front radar FOV which in turn affects the object length
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(
      const F360_Calibrations_T& calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const F360_Object_Track_T& object_track,
      float32_t& process_noise)
   {
      if (globals.f_single_front_center_radar_only)
      {
         switch (object_track.reference_point)
         {
         case (F360_REFERENCE_POINT_LEFT):
         {
            const bool both_points_on_side_visible = (Is_Ref_Point_In_Sensors_FOV(F360_REFERENCE_POINT_FRONT_LEFT, object_track, sensors, globals)
                                                   && Is_Ref_Point_In_Sensors_FOV(F360_REFERENCE_POINT_REAR_LEFT, object_track, sensors, globals));
            if (!both_points_on_side_visible)
            {
               process_noise = process_noise * calib.k_size_update_process_noise_pruning;
            }
            break;
         }
         case (F360_REFERENCE_POINT_RIGHT):
         {
            const bool both_points_on_side_visible = (Is_Ref_Point_In_Sensors_FOV(F360_REFERENCE_POINT_FRONT_RIGHT, object_track, sensors, globals) &&
               Is_Ref_Point_In_Sensors_FOV(F360_REFERENCE_POINT_REAR_RIGHT, object_track, sensors, globals));
            if (!both_points_on_side_visible)
            {
               process_noise = process_noise * calib.k_size_update_process_noise_pruning;
            }
            break;
         }
         default:
            break;
         }
      }
   }
   /*===========================================================================*\
   * FUNCTION: Update_Measurement_Noise_If_Many_Detections()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib
   * const float32_t object_length
   * const float32_t measured_length
   * const bool f_left_or_right_visible,
   * F360_Object_Track_T& object_track,
   * float32_t& measurement_uncertainty
   *
   * POSTCONDITIONS:
   * None
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function increases the confidence in the measurement (i.e. decreases the measurement noise) if
   * object has many associated detections and the measured length is larger than object's length or
   * lower based on some conditions regarding the innovation.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Measurement_Noise_If_Many_Detections(
       const F360_Calibrations_T& calib,
       const float32_t object_length,
       const float32_t measured_length,
       const bool f_left_or_right_visible,
       F360_Object_Track_T& object_track,
       float32_t& measurement_uncertainty)
   {
       // For increasing the length faster
       const bool f_fast_increase_length = (object_track.ndets > calib.k_min_num_dets_to_decrease_meas_uncertainty) && (object_length < measured_length);

       // For reducing the length faster
       const float32_t length_innovation = measured_length - object_length;
       const bool f_long_obj_big_negative_innovation = ((length_innovation < -0.6F) && (object_length > 6.0F));
       const bool f_very_big_negative_innov = (length_innovation < -1.5F);
       const bool f_measured_len_valid = ((measured_length / object_length) > 0.45F); // To not be sensitive to outliers
       const uint32_t k_min_num_dets_to_decrease_meas_uncertainty_for_len_reduction = 3U;
       const bool f_fast_reduction_length = ((object_track.ndets > k_min_num_dets_to_decrease_meas_uncertainty_for_len_reduction)
           && f_left_or_right_visible
           && (f_long_obj_big_negative_innovation || f_very_big_negative_innov)
           && f_measured_len_valid);

       object_track.f_shrink_fast = false;
       if (f_fast_increase_length)
       {
           Apply_Detection_Based_Uncertainty_Scaling(
               object_track.ndets,
               calib.k_min_num_dets_to_decrease_meas_uncertainty,
               calib.k_min_num_dets_to_decrease_meas_uncertainty,
               measurement_uncertainty);
       }
       else if (f_fast_reduction_length)
       {
           // Make measurement more certain if there are more than 3 dets, eg. trucks and object needs to shrink
           Apply_Detection_Based_Uncertainty_Scaling(
               object_track.ndets,
               k_min_num_dets_to_decrease_meas_uncertainty_for_len_reduction,
               5U,
               measurement_uncertainty);
           object_track.f_shrink_fast = true;
       }
       else
       {
           // Do nothing. For MISRA warning
       }
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Obj_Size()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const rspp_variant_A::RSPP_Detection_List_T& detection_list,
   * const F360_Calibrations_T& calib,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Globals_T& globals,
   * const F360_Tracker_Info_T& tracker_info,
   * const float32_t& CIPV_long_pos,
   * F360_Object_Track_T& object_track
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
   * Calculates object size - length and width.
   *
   * 1) Non-movable objects (movable_prob < 0.5) are forced to a default size 0.5m x 0.5m.
   * 2) For movable objects a two-stage shrink logic is applied:
   *    - Fast shrink: below a low speed threshold the box is exponentially
   *      driven toward the non-movable diameter unless inhibited (CIPV or
   *      variant-K zone).
   *    - Slow shrink: within an intermediate speed band the maximum allowed
   *      length is a speed-dependent linear interpolation; length is relaxed
   *      toward that bound.
   * Variant / scenario protections:
   *    - CIPV objects and objects in designated zones for variant K are
   *      exempted from shrink logic.
   * 3) If shrinking does not apply and there are sufficient, newly associated
   *    detections, a per-dimension 1D Kalman update is executed:
   *    - Projects each detection into the object's para / orth axes based on
   *      current orientation to derive measured extents (max - min).
   *    - Updates only those dimensions whose corresponding edges are visible
   *      (front/rear or left/right) OR when speed is high enough to justify
   *      updating a non-visible side.
   *    - Adapts process noise for: low vs higher speed, invisible edges
   *      (pruning factor), and partial FOV coverage in front-only radar mode.
   *    - Reduces measurement variance when many detections support a longer
   *      measured length than the current estimate.
   * 4) After each update (shrink or Kalman) applies calibrated min/max limits
   *    and (for CTCA / fast CCA) enforces aspect ratio constraints.
   * 5) Leaves size unchanged if prerequisites (e.g. number of detections,
   *    status) are not met.
   *
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_Obj_Size(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& detection_list,
      const F360_Calibrations_T& calib,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const F360_Tracker_Info_T& tracker_info,
      const float32_t& CIPV_long_pos,
      F360_Object_Track_T& object_track)
   {
      // if non-movable, set to default
      if (object_track.movable_prob < 0.5F)
      {
         object_track.bbox.Set_Length(calib.k_nonmoveable_target_diameter);
         object_track.bbox.Set_Width(calib.k_nonmoveable_target_diameter);
      }
      else
      {
          Dimension_Limits_T obj_dim_limits{};
          obj_dim_limits.length.minimum = calib.k_nonmoveable_target_diameter;
          obj_dim_limits.length.maximum = calib.k_fast_movable_max_target_length;
          obj_dim_limits.width.minimum = calib.k_nonmoveable_target_diameter;
          obj_dim_limits.width.maximum = calib.k_movable_max_target_width;

          const float32_t speed_limit_for_fast_shrinkage = 1.0F; // [m/s] speed limit below which fast shrinkage is applied

          const float32_t object_length = object_track.bbox.Get_Length();
          const float32_t object_width = object_track.bbox.Get_Width();
          const float32_t abs_obj_speed = std::abs(object_track.speed);

          const bool f_prevent_shrink_variant_zone = In_Special_Zone_For_No_Shrinking(object_track.vcs_position, tracker_info.variant.type);

          const bool f_prevent_shrink_CIPV = Determine_CIPV_Status(object_track, CIPV_long_pos, calib);

          const bool f_limit_size_slow_speed =
              (abs_obj_speed < calib.k_object_shrinking_speed_threshold)
              && (abs_obj_speed >= speed_limit_for_fast_shrinkage)
              && (!f_prevent_shrink_CIPV)
              && (!f_prevent_shrink_variant_zone);

          const float32_t upper_length_for_slow_speed =
              F360_Linear_Equation_With_Saturation(abs_obj_speed,
                  calib.k_speed_for_min_length_of_slow_moving_objects,
                  calib.k_speed_for_max_length_of_slow_moving_objects,
                  calib.k_min_length_for_slow_moving_objects,
                  calib.k_max_length_for_slow_moving_objects);

          const bool f_slow_shrink = (f_limit_size_slow_speed) && (object_length > upper_length_for_slow_speed);
          const bool f_fast_shrink = (abs_obj_speed < speed_limit_for_fast_shrinkage) && (!f_prevent_shrink_CIPV) && (!f_prevent_shrink_variant_zone);

          // fast shrink to 0.5mx0.5m below 1.0m/s
          if (f_fast_shrink)
          {
              const float32_t rounding_buffer = 0.01F; // [m] to avoid oscillation around calib.k_nonmoveable_target_diameter
              const bool f_shrinked = (object_length < calib.k_nonmoveable_target_diameter + rounding_buffer) && (object_width < calib.k_nonmoveable_target_diameter + rounding_buffer);
              if (f_shrinked)
              {
                  object_track.bbox.Set_Length(calib.k_nonmoveable_target_diameter);
                  object_track.bbox.Set_Width(calib.k_nonmoveable_target_diameter);

                  // Update affected object signals after resizing length and width
                  object_track.Update_Bbox_Center();
              }
              else
              {
                  const float32_t gain = 0.4F;
                  const float32_t measured_length = calib.k_nonmoveable_target_diameter;
                  const float32_t measured_width = calib.k_nonmoveable_target_diameter;

                  Update_Object_Length(measured_length, gain, false, object_track);
                  Update_Object_Width(measured_width, gain, false, object_track);

                  // Update affected object signals after resizing length and width
                  object_track.Update_Bbox_Center();
              }
          }
          // slow shrink to small if 1.0 < speed < 5.0
          else if (f_slow_shrink)
          {
              const float32_t gain = 0.3F;
              const float32_t measured_length = upper_length_for_slow_speed - 0.3F; // -0.3 to avoid hovering above upper_length_for_slow_shrink

              Update_Object_Length(measured_length, gain, false, object_track);

              Limit_Size_And_Enforce_CTCA_Obj_Aspect_Ratio(calib, obj_dim_limits, object_track); // Keep aspect ratio
          }
          // if object is not eligible for shrinking and there are detections, update size with Kalman filter
          else if ((object_track.ndets >= 2U) && (object_track.status == F360_OBJECT_STATUS_UPDATED))
          {
              obj_dim_limits.length.maximum = f_limit_size_slow_speed ? upper_length_for_slow_speed : calib.k_fast_movable_max_target_length;

              // Find extents of associated detections to get size measurements.
              const float32_t cos_pointing = object_track.bbox.Get_Orientation().Cos();
              const float32_t sin_pointing = object_track.bbox.Get_Orientation().Sin();

              float32_t min_para = INFTY;
              float32_t max_para = -INFTY;
              float32_t min_orth = INFTY;
              float32_t max_orth = -INFTY;
              bool f_range_criterea_met = true;

              for (uint32_t i = 0U; i < object_track.ndets; i++)
              {
                  const uint32_t det_idx = object_track.detids[i] - 1U;

                  // Project the detections on para/orth axes of the object and find minimum and maximum
                  const float32_t det_para = cos_pointing * det_props[det_idx].vcs_position.x + sin_pointing * det_props[det_idx].vcs_position.y;
                  const float32_t det_orth = -sin_pointing * det_props[det_idx].vcs_position.x + cos_pointing * det_props[det_idx].vcs_position.y;

                  min_para = (det_para < min_para) ? det_para : min_para;
                  max_para = (det_para > max_para) ? det_para : max_para;
                  min_orth = (det_orth < min_orth) ? det_orth : min_orth;
                  max_orth = (det_orth > max_orth) ? det_orth : max_orth;

                  const float32_t curr_det_range = detection_list.detections[det_idx].raw.range;
                  f_range_criterea_met = ((curr_det_range > calib.k_size_update_min_det_range) && f_range_criterea_met);
              }

              // Check if left/right or front/rear are visible
              const F360_Object_Sides_T visible_side = Get_Reference_Point_Orth_Side(object_track.reference_point);
              const bool f_left_or_right_visible = (visible_side != F360_OBJECT_SIDES_INVALID);
              const F360_Object_Sides_T visible_front_rear = Get_Reference_Point_Para_Side(object_track.reference_point);
              const bool f_front_or_rear_visible = (visible_front_rear != F360_OBJECT_SIDES_INVALID);
              const bool f_ok_to_update_non_visible_side = ((object_track.speed > calib.k_size_update_min_speed_to_update_nonvisible_side) && f_range_criterea_met);

              bool f_update_length = f_left_or_right_visible || f_ok_to_update_non_visible_side;
              const bool f_update_width = f_front_or_rear_visible || f_ok_to_update_non_visible_side;


              const float32_t measured_length = F360_Saturate(max_para - min_para, obj_dim_limits.length.minimum, obj_dim_limits.length.maximum);
              const float32_t length_threshold = std::min(1.0F, object_length);
              // Don't update length if measurement not valid and we can't see sides
              if ((measured_length < length_threshold) &&
                 (object_track.speed > 8.0F) &&
                 (!f_left_or_right_visible))
              {
                  f_update_length = false;
              }

              // Update length
              if (f_update_length)
              {

                  Kalman_Noise_T noise{};
                  Set_Default_Kalman_Noise(calib, object_track.speed, f_left_or_right_visible, noise);

                  Update_Process_Noise_If_Obj_Outside_Front_Only_Fov(calib, sensors, globals, object_track, noise.process);

                  Update_Measurement_Noise_If_Many_Detections(calib, object_length, measured_length, f_left_or_right_visible, object_track, noise.measurement);

                  // Kalman filter for length estimation
                  object_track.length_uncertainty += noise.process;
                  const float32_t denominator = object_track.length_uncertainty + noise.measurement;
                  const float32_t gain = (denominator > F360_EPSILON) ? object_track.length_uncertainty / denominator : 0.0F;
                  Update_Object_Length(measured_length, gain, true, object_track);
              }

              if (f_update_width)
              {
                  // Update width
                  const float32_t measured_width = F360_Saturate(max_orth - min_orth, obj_dim_limits.width.minimum, obj_dim_limits.width.maximum);

                  Kalman_Noise_T noise{};
                  Set_Default_Kalman_Noise(calib, object_track.speed, f_front_or_rear_visible, noise);

                  // Kalman filter for width estimation
                  object_track.width_uncertainty += noise.process;
                  const float32_t denominator = object_track.width_uncertainty + noise.measurement;
                  const float32_t gain = (denominator > F360_EPSILON) ? object_track.width_uncertainty / denominator : 0.0F;
                  Update_Object_Width(measured_width, gain, true, object_track);
              }

              Limit_Size_And_Enforce_CTCA_Obj_Aspect_Ratio(calib, obj_dim_limits, object_track);
          }
          else
          {
              // No size update, keep previous size
          }
      }
   }

   /*===========================================================================*\
  * FUNCTION: Set_Default_Kalman_Noise()
  *===========================================================================
  * RETURN VALUE:
  * None
  *
  * PARAMETERS:
  * const F360_Calibrations_T & calib
  * const float32_t object_speed
  * const bool f_edge_visible
  * Kalman_Noise_T & noise
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
  * Functions sets default Kalman noise values for size update for both dimensions.
  * Process noise is also adjusted for object speed and edge visibility.
  *
  * PRECONDITIONS:
  * None
  *
  * POSTCONDITIONS:
  * None
  *
  \*===========================================================================*/
   static void Set_Default_Kalman_Noise(
	   const F360_Calibrations_T& calib,
	   const float32_t object_speed,
       const bool f_edge_visible,
       Kalman_Noise_T& noise)
   {
      noise.process = std::abs(object_speed) < calib.k_size_update_speed_threshold_low_speed_process_noise ? calib.k_size_update_low_speed_process_noise : calib.k_size_update_base_process_noise;
      noise.measurement = calib.k_size_update_base_measurement_uncertainty;
      // If relevant edge is not visible, make filter slower by reducing process noise
      if (!f_edge_visible)
      {
          noise.process = noise.process * calib.k_size_update_process_noise_pruning;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Limit_Size_And_Enforce_CTCA_Obj_Aspect_Ratio()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calib
   * const Dimension_Limits_T & dim_limits
   * F360_Object_Track_T & obj
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
   * Functions checks wheter the object bounding box dimensions are within the allowed minimum and
   * maximum limits. The function also checks so that the length and width ratio of the object are
   * within allowed limits. If the current object dimensions are deemed to be improper then the
   * function computes and returns new proposed object width and length that are within allowed
   * limits. If the current object dimensions are deemed to be okay then the function returns the
   * current dimensions.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Limit_Size_And_Enforce_CTCA_Obj_Aspect_Ratio(
      const F360_Calibrations_T & calib,
      const Dimension_Limits_T & dim_limits,
      F360_Object_Track_T & obj)
   {
      float32_t resized_length = obj.bbox.Get_Length();
      float32_t resized_width = obj.bbox.Get_Width();

      // Limit object size
      resized_length = F360_Saturate(resized_length, dim_limits.length.minimum, dim_limits.length.maximum);
      resized_width = F360_Saturate(resized_width, dim_limits.width.minimum, dim_limits.width.maximum);

      // Enforce object aspect ratio while making sure object dimensions are still limited
      if ((F360_TRACKER_TRKFLTR_CTCA == obj.trk_fltr_type) || ((F360_TRACKER_TRKFLTR_CCA == obj.trk_fltr_type) && (std::abs(obj.speed) > calib.fast_moving_thresh)))
      {
         const float32_t aspect_width = F360_Saturate(calib.k_min_aspect_ratio * resized_length, dim_limits.width.minimum, dim_limits.width.maximum);
         resized_width = std::max(resized_width, aspect_width);
         const float32_t aspect_length = F360_Saturate(resized_width / calib.k_max_aspect_ratio, dim_limits.length.minimum, dim_limits.length.maximum);
         resized_length = std::max(resized_length, aspect_length);
      }
      obj.bbox.Set_Length(resized_length);
      obj.bbox.Set_Width(resized_width);

      // Update affected object signals after resizing length and width
      obj.Update_Bbox_Center();
   }

   /*===========================================================================*\
   * FUNCTION: Update_Object_Length()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t measured_length
   * const float32_t gain
   * const bool f_update_uncertainty
   * F360_Object_Track_T& obj
   *
   * EXTERNAL REFERENCES:
   * Updates obj.bbox length via Set_Length().
   * Optionally modifies obj.length_uncertainty.
   *
   * DEVIATIONS FROM STANDARDS:
   *    None.
   *
   * ---------------------------------------------------------------------------
   * ABSTRACT:
   * ---------------------------------------------------------------------------
   * Performs a first-order (alpha / Kalman style) update of the tracked object
   * length using: new = old + gain * (measurement - old). Optionally applies the
   * standard posterior variance reduction (1 - gain) * prior_variance.
   *
   * PRECONDITIONS:
   * obj.bbox length and obj.length_uncertainty contain prior state estimates.
   * Gain is assumed to be properly computed by caller (no range enforcement here).
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static void Update_Object_Length(
       const float32_t measured_length,
       const float32_t gain,
       const bool f_update_uncertainty,
       F360_Object_Track_T& obj
   )
   {
       const float32_t object_length = obj.bbox.Get_Length();
       const float32_t updated_length = object_length + gain * (measured_length - object_length);
       obj.bbox.Set_Length(updated_length);
       if (f_update_uncertainty)
       {
           // Posterior variance update: P = (1 - K) * P
           obj.length_uncertainty = (1.0F - gain) * obj.length_uncertainty;
       }
   }

   /*===========================================================================*\
   * FUNCTION: Update_Object_Width()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t measured_width
   * const float32_t gain
   * const bool f_update_uncertainty
   * F360_Object_Track_T& obj
   *
   * EXTERNAL REFERENCES:
   *    Updates obj.bbox width via Set_Width().
   *    Optionally modifies obj.width_uncertainty.
   *
   * DEVIATIONS FROM STANDARDS:
   *    None.
   *
   * ---------------------------------------------------------------------------
   * ABSTRACT:
   * ---------------------------------------------------------------------------
   * Performs a first-order (alpha / Kalman style) update of the tracked object
   * width using: new = old + gain * (measurement - old). Optionally applies the
   * standard posterior variance reduction (1 - gain) * prior_variance.
   *
   * PRECONDITIONS:
   * obj.bbox width and obj.width_uncertainty contain prior state estimates.
   * Gain is assumed to be properly computed by caller.
   *
   * POSTCONDITIONS:
   *  None.
   *
   \*===========================================================================*/
   static void Update_Object_Width(
       const float32_t measured_width,
       const float32_t gain,
       const bool f_update_uncertainty,
       F360_Object_Track_T& obj
   )
   {
       const float32_t object_width = obj.bbox.Get_Width();
       const float32_t updated_width = object_width + gain * (measured_width - object_width);
       obj.bbox.Set_Width(updated_width);

       if (f_update_uncertainty)
       {
           // Posterior variance update: P = (1 - K) * P
           obj.width_uncertainty = (1.0F - gain) * obj.width_uncertainty;
       }
   }

   /*===========================================================================*\
   * FUNCTION: Apply_Detection_Based_Uncertainty_Scaling()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const uint32_t object_ndets - Number of detections associated with the object
   * const uint32_t min_dets_threshold - Minimum number of detections threshold
   * const uint32_t max_scale_limit - Maximum limit for the scale calculation
   * float32_t& measurement_uncertainty - Measurement uncertainty to be scaled
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
   * Calculates a scale factor based on the number of detections above a minimum
   * threshold and applies it to the measurement uncertainty. The scale factor is
   * computed as 1.0 / min(n_dets_above_threshold, max_limit) and applied as the
   * square to reduce measurement uncertainty when there are many detections.
   *
   * PRECONDITIONS:
   * object_ndets >= min_dets_threshold for meaningful results
   *
   * POSTCONDITIONS:
   * measurement_uncertainty is modified by the squared scale factor
   *
   \*===========================================================================*/
   static void Apply_Detection_Based_Uncertainty_Scaling(
       const uint32_t object_ndets,
       const uint32_t min_dets_threshold,
       const uint32_t max_scale_limit,
       float32_t& measurement_uncertainty)
   {
       const uint32_t n_dets_above_min_nbr = object_ndets - min_dets_threshold;
       // To ensure that even in case, changes happen outisde the function, we never devide by 0
       const float32_t scaler  = static_cast<float32_t>(std::min(n_dets_above_min_nbr, max_scale_limit));
       if (scaler > 0.0F)
       {
           const float32_t ndet_scale = 1.0F / scaler;
           measurement_uncertainty *= ndet_scale * ndet_scale;
       }
       else
       {
           // Do nothing
       }
   }
}

