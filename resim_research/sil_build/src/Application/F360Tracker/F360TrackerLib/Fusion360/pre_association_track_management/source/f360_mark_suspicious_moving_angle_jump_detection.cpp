/*===========================================================================*\
* FILE: f360_mark_suspicious_moving_angle_jump_detection.cpp
*============================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains implementation of Mark_Suspicious_Moving_Angle_Jump_Detection()
*   for detecting angle jumps from moving objects.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#include "f360_mark_suspicious_moving_angle_jump_detection.h"
#include "f360_mark_suspicious_angle_jump_detections.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_try_to_dealiase_range_rate.h"
#include "f360_calc_predicted_range_rate.h"
#include "f360_detection_association_support_functions.h"
#include "f360_check_if_point_is_inside_box.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Check_Angle_Jump_Candidate()
   *===========================================================================
   * RETURN VALUE:
   * bool - true if the detection is an angle jump candidate, false otherwise
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T& sensor,
   * const rspp_variant_A::RSPP_Detection_T& rspp_det,
   * const F360_Detection_Props_T& det_prop,
   * const BoundingBox& extended_box,
   * const F360_Object_Track_T& obj,
   * const float32_t new_vcs_x, const float32_t new_vcs_y,
   * const float32_t cos_new_az, const float32_t sin_new_az,
   * const float32_t rr_th_lower, const float32_t rr_th_upper
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
   * Validates whether a detection with a shifted azimuth angle is a candidate
   * for moving object angle jump detection. Checks if the shifted position falls
   * within an object's extended bounding box and verifies that the range rate
   * can be dealiased to the predicted range rate within the specified threshold
   * interval.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Check_Angle_Jump_Candidate(
      const F360_Radar_Sensor_T&               sensor,
      const rspp_variant_A::RSPP_Detection_T&  rspp_det,
      const F360_Detection_Props_T&            det_prop,
      const BoundingBox&                       extended_box,
      const F360_Object_Track_T&               obj,
      const float32_t new_vcs_x, const float32_t new_vcs_y,
      const float32_t cos_new_az, const float32_t sin_new_az,
      const float32_t rr_th_lower, const float32_t rr_th_upper)
   {
      bool f_angle_jump_candidate = false;
      if (extended_box.Contains(Point(new_vcs_x, new_vcs_y)))
      {
         F360_Detection_Props_T temp_det_prop = det_prop;
         temp_det_prop.vcs_position.x = new_vcs_x;
         temp_det_prop.vcs_position.y = new_vcs_y;
         rspp_variant_A::RSPP_Detection_T temp_det_raw = rspp_det;
         temp_det_raw.processed.cos_vcs_az = cos_new_az;
         temp_det_raw.processed.sin_vcs_az = sin_new_az;

         const float32_t predicted_range_rate = Calc_Predicted_Range_Rate(
               temp_det_prop, temp_det_raw, obj, sensor);

         float32_t dealiased_rr = 0.0F;
         float32_t rr_interval  = 0.0F;
         f_angle_jump_candidate =  Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(
               rspp_det.raw.range_rate,
               predicted_range_rate,
               rr_th_lower,
               rr_th_upper,
               sensor.constant.v_wrapping[sensor.variable.look_id],
               sensor.constant.min_aliaised_range_rate[sensor.variable.look_id],
               dealiased_rr,
               rr_interval);
      }
      return f_angle_jump_candidate;
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Suspicious_Moving_Angle_Jump_Detection()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibrations,
   * const F360_Radar_Sensor_T& sensor,
   * const F360_Host_T& host,
   * const rspp_variant_A::RSPP_Detection_T& rspp_det,
   * F360_Detection_Props_T& det_prop
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
   * This function marks out suspicious moving angle jump detections by checking
   * if the detection could be an angle jump from a moving object.
   * For forward sensors the relevant target is an oncoming object.
   * For rear corner sensors the relevant target is an ongoing object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Suspicious_Moving_Angle_Jump_Detection(
      const F360_Radar_Sensor_T& sensor,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T& det_prop)
   {
      const float32_t min_host_speed = F360_KPH2MPS(20.0F);
      const float32_t abs_range_rate_comp_cond = 0.7F;
      const float32_t det_min_vcs_az_angle = F360_DEG2RAD(15.0F);
      const float32_t det_snr_cond = 25.0F;
      const float32_t det_min_range_fwd = 10.0F;
      const float32_t det_min_range_rear = 4.0F;
      const float32_t det_max_range_rear = 15.0F;
      const float32_t det_max_range_fwd = INFTY;

      const bool f_rear_corner_sensor = Is_Rear_Corner_Sensor(sensor);
      const float32_t det_min_range = f_rear_corner_sensor ? det_min_range_rear : det_min_range_fwd;
      const float32_t det_max_range = f_rear_corner_sensor ? det_max_range_rear : det_max_range_fwd;

      // Evaluate all detection-based conditions before calling Determine_Precond_For_Angle_Jumps
      // to avoid the LUT lookup cost when the detection cannot be an angle jump candidate.
      if ((!det_prop.f_angle_amb) &&
          (det_prop.f_ok_to_use) &&
          (host.speed > min_host_speed) &&
          (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_prop.motion_status) &&
          (std::abs(det_prop.range_rate_compensated) > abs_range_rate_comp_cond) &&
          (std::abs(rspp_det.processed.vcs_az) > det_min_vcs_az_angle) &&
          (rspp_det.raw.range > det_min_range) &&
          (rspp_det.raw.range < det_max_range) &&
          (f_rear_corner_sensor || (rspp_det.raw.snr < det_snr_cond)))
      {
         Sensor_Stationary_Angle_Ambiguity_T sensor_angle_amb = {};
         const bool f_is_moving_angle_jump_context = true;  // Moving angle jump detection
         Determine_Precond_For_Angle_Jumps(sensor, rspp_det, det_prop, f_is_moving_angle_jump_context, sensor_angle_amb);

         if (sensor_angle_amb.f_sensor_relevant)
         {
            // Per-sensor loop parameters
            const float32_t rr_th_lower = f_rear_corner_sensor ? -0.4F : -1.0F;
            const float32_t rr_th_upper = f_rear_corner_sensor ? 0.4F  :  1.0F;
            const F360_Object_Track_T* const obj_list_start = f_rear_corner_sensor
               ? tracker_info.vcslong_sorted_start
               : tracker_info.vcslong_sorted_first_infront_of_host;
            // f_apply_original_det_pos_in_obj_check check is applied when the detection (non shifted positions) 
            // is close enough that it could legitimately sit inside a moving objects bbox
            const bool f_apply_original_det_pos_in_obj_check = rspp_det.raw.range < 15.0F;

            bool f_angle_jump_detected = false;
            bool f_original_det_pos_in_obj = false;
            
            // Check each angle ambiguity candidate for a potential moving object angle jump
            for (uint8_t az_cand_idx = 0U;
                 (az_cand_idx < sensor_angle_amb.nr_of_candidates) && (!f_angle_jump_detected) && (!f_original_det_pos_in_obj);
                 az_cand_idx++)
            {
               const float32_t angle_shift = sensor_angle_amb.angle_ambiguity_candidates_rad[az_cand_idx];
               if (angle_shift < INFTY)
               {
                  const float32_t new_vcs_az_candidate = sensor.variable.vacs_boresight_az_estimated + (rspp_det.raw.azimuth + angle_shift) * static_cast<float32_t>(sensor.constant.polarity);

                  // Calculate new VCS position with the angle candidate
                  const float32_t cos_new_az = F360_Cosf(new_vcs_az_candidate);
                  const float32_t sin_new_az = F360_Sinf(new_vcs_az_candidate);
                  const float32_t new_vcs_x = sensor.constant.mounting_position.vcs_position.longitudinal + rspp_det.raw.range * cos_new_az;
                  const float32_t new_vcs_y = sensor.constant.mounting_position.vcs_position.lateral      + rspp_det.raw.range * sin_new_az;

                  // Search for nearby moving objects that could be the source of an angle jump.
                  // Forward: start at first object in front and search oncoming objects (longitudinal velocity < -3).
                  // Rear: start at list head (object with most negative x) and search ongoing objects (longitudinal velocity > +3).
                  const F360_Object_Track_T* obj = obj_list_start;
                  for (uint32_t i = 0U; 
                     (i < NUMBER_OF_OBJECT_TRACKS) && (!f_angle_jump_detected) && (!f_original_det_pos_in_obj); i++)
                  {
                     const float32_t rough_obj_search_distance_x = 8.0F;

                     // Break if the list end is reached or the object is too far ahead of the detection candidate
                     if ((NULL == obj) || ((obj->vcs_position.x - new_vcs_x) > rough_obj_search_distance_x))
                     {
                        break;
                     }
                     const int32_t obj_idx = obj->id - 1;

                     const float32_t rough_obj_search_distance_y = 5.0F;
                     const float32_t dx = new_vcs_x - obj->vcs_position.x;
                     const float32_t dy = new_vcs_y - obj->vcs_position.y;
                     const bool f_within_rough_zone_vcs = (std::abs(dx) <= rough_obj_search_distance_x) && (std::abs(dy) <= rough_obj_search_distance_y);

                     const bool f_obj_vel_ok = f_rear_corner_sensor
                        ? (obj->vcs_velocity.longitudinal > 3.0F)
                        : (obj->vcs_velocity.longitudinal < -3.0F);

                     if ((obj->movable_prob > 0.5F) && f_obj_vel_ok && f_within_rough_zone_vcs)
                     {
                        BoundingBox box{ obj->bbox };
                        Determine_Extended_Bounding_Box(*obj, box);

                        // Prevent flagging detections whose original (non-shifted) position is already inside
                        // the object association gate. At close range, a small angle shift would keep the
                        // detection within the same object gate, this detection is unlikely to be an angle jump.
                        // This detection should not be evaluated since this angle jump flagging only aims to prevent angle jump
                        // detections to be used in initialization.
                        if (f_apply_original_det_pos_in_obj_check 
                           && box.Contains(Point(det_prop.vcs_position.x, det_prop.vcs_position.y)))
                        {
                           f_original_det_pos_in_obj = true;
                        }
                        else if (Check_Angle_Jump_Candidate(sensor, rspp_det, det_prop, box, *obj,
                                    new_vcs_x, new_vcs_y, cos_new_az, sin_new_az, rr_th_lower, rr_th_upper))
                        {
                           f_angle_jump_detected = true;
                           det_prop.f_angle_amb = true;
                        }
                        else
                        {
                           // Continue searching
                        }
                     }
                     // Advance to the next longitudinally sorted object
                     obj = tracker_info.vcslong_sorted_next_track[obj_idx];
                  }
               }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Process_Moving_Angle_Jump_Detections()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibrations,
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Host_T& host,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
   * const F360_Tracker_Info_T& tracker_info,
   * F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Main function that loops over all detections and calls Mark_Suspicious_Moving_Angle_Jump_Detection()
   * for each detection to identify potential angle jumps from moving objects.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Process_Moving_Angle_Jump_Detections(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (uint32_t det_idx = 0U; det_idx < raw_detect_list.number_of_valid_detections; det_idx++)
      {
         const rspp_variant_A::RSPP_Detection_T& detection = raw_detect_list.detections[det_idx];
         const int32_t sensor_idx = detection.raw.sensor_id - 1;
         const F360_Radar_Sensor_T& sensor = sensors[sensor_idx];
            
         Mark_Suspicious_Moving_Angle_Jump_Detection(
            sensor,
            host,
            detection,
            tracker_info,
            detection_props[det_idx]);
      }
   }
}
