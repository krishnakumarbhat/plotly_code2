/*===================================================================================*\
* FILE: f360_determine_association_hypothesis.cpp
*====================================================================================
* Copyright (C) 2020-2022 Aptiv Technologies, Inc., All Rights Reserved.
* Aptiv Confidential
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functionality for determining the detection to track association
*   hypothesis.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*====================================================================================*/
#include "f360_determine_association_hypothesis.h"
#include "f360_calc_predicted_range_rate.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_detection_association_support_functions.h"
#include "f360_try_to_dealiase_range_rate.h"

namespace f360_variant_A
{
/*===========================================================================*\
* FUNCTION: Determine_Association_Hypothesis
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
*   const F360_Calibrations_T & calibrations
*   const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
*   const uint32_t (&det_idx_array)[MAX_DETS_IN_OBJ_TRK]
*   const uint32_t num_dets_in_array
*   const int32_t obj_idx
*   const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS]
*   const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
*   F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
*   F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
*   float32_t (&det_score_array)[MAX_NUMBER_OF_DETECTIONS]
*   float32_t (&det_rdot_comp_array)[MAX_NUMBER_OF_DETECTIONS]
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
* This function tries to associate detections to the object with index obj_idx.
* The full object list is passed since we need to handle cases where a detection
* fits association gates for several objects.
*
* PRECONDITIONS:
* Before first call to function the parameter det_best_score array must
* have been initialized by calling function Initialize_Detection_Score_Array().
*
* POSTCONDITIONS:
* None
\*===========================================================================*/

void Determine_Association_Hypothesis(const F360_Calibrations_T &calibrations,
                                      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS], const float32_t host_vcs_speed,
                                      const int16_t (&det_idx_array)[MAX_NUMBER_OF_DETECTIONS], const uint32_t num_dets_in_array,
                                      const int32_t obj_idx, const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                                      const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
                                      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
                                      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
                                      float32_t (&det_score_array)[MAX_NUMBER_OF_DETECTIONS],
                                      float32_t (&det_rdot_comp_array)[MAX_NUMBER_OF_DETECTIONS])
{
   for (uint32_t i = 0U; i < num_dets_in_array; i++)
   {
      // Support variable to track current detection index
      const int16_t det_idx            = det_idx_array[i];
      const F360_Detection_T &det_raw  = detections[det_idx];
      F360_Detection_Props_T &det_prop = detection_props[det_idx];
      F360_Object_Track_T &obj         = object_tracks[obj_idx];

      if (Is_Det_Allowed_To_Associate(det_prop, det_raw, obj, calibrations, sep))
      {
         // Detection satisfies conditions to be tried for association
         const uint32_t sensor_idx              = static_cast<uint32_t>(det_raw.sensor_id) - 1U;
         const F360_Radar_Sensor_T &sensor      = sensors[sensor_idx];
         const float32_t curr_rr_interval_width = sensor.v_wrapping[sensor.look_id];

         // Calculate predicited range rate
         const float32_t predicted_range_rate = Calc_Predicted_Range_Rate(det_prop, obj, sensor);

         // Calculate dynamic threshold value for association on range rate
         float32_t range_rate_th_lower = 0.0F;
         float32_t range_rate_th_upper = 0.0F;
         Calc_Range_Rate_Threshold(obj, det_raw, det_prop, sensor, calibrations, host_vcs_speed, range_rate_th_lower,
                                   range_rate_th_upper);

         // Dealias detection range rate towards predicted range rate
         float32_t dealiased_rngrate;
         float32_t rr_interval;
         bool f_inside_gate = Try_To_Dealiase_Range_Rate_Nonsymetrical_Threshold_Interval(
            det_raw.range_rate, predicted_range_rate, range_rate_th_lower, range_rate_th_upper, curr_rr_interval_width,
            sensor.min_aliaised_range_rate[sensor.look_id], dealiased_rngrate, rr_interval);

         const float32_t predicted_compensated_range_rate = predicted_range_rate +
                                                            sensor.vcs_velocity.longitudinal * det_prop.cos_vcs_az +
                                                            sensor.vcs_velocity.lateral * det_prop.sin_vcs_az;
         const bool f_detection_motion_ambiguous = std::abs(predicted_compensated_range_rate) > 0.9F * curr_rr_interval_width;
         if (f_inside_gate && f_detection_motion_ambiguous)
         {
            const bool f_moving_hypothesis_significantly_better_than_stat =
               Compare_Against_Stationary_Hypothesis(calibrations, sensor, det_raw, range_rate_th_lower, range_rate_th_upper,
                                                     predicted_range_rate, dealiased_rngrate, det_prop);
            f_inside_gate = f_moving_hypothesis_significantly_better_than_stat;
         }

         // Check if range rate matches predicted range rate
         if (f_inside_gate)
         {
            const float32_t delta_range = sensor.r_wrapping[sensor.look_id] * rr_interval;

            Point uncompensated_vcs_position = Point(det_prop.vcs_position.x, det_prop.vcs_position.y);
            if (fabsf(det_prop.range_dealiased - det_raw.range) > F360_EPSILON)
            {
               // Recompute the uncompensated VCS position for dealiased detections.
               uncompensated_vcs_position.Set_Position(
                  sensor.mounting_position.vcs_position.longitudinal + (det_raw.range * det_prop.cos_vcs_az),
                  sensor.mounting_position.vcs_position.lateral + (det_raw.range * det_prop.sin_vcs_az));
            }

            const Point temp_vcs_position = Point(uncompensated_vcs_position.x + delta_range * det_prop.cos_vcs_az,
                                                  uncompensated_vcs_position.y + delta_range * det_prop.sin_vcs_az);

            const float32_t temp_score = Calc_Det_Score(obj, calibrations, dealiased_rngrate - predicted_range_rate,
                                                        temp_vcs_position, det_prop.f_water_spray);

            const float32_t prev_best_score = det_score_array[det_idx];

            // Only associate if detection score to current track is better (lower) than any eventual previous association to
            // another track
            if (temp_score < prev_best_score)
            {
               const Point prev_vcs_position    = det_prop.vcs_position;
               const int32_t prev_assoc_obj_idx = det_prop.object_track_id - 1;
               det_prop.object_track_id         = obj_idx + 1;
               det_prop.range_rate_dealiased    = dealiased_rngrate;
               det_prop.range_rate_predicted    = predicted_range_rate;

               det_prop.range_dealiased = detections[det_idx].range + delta_range;
               det_prop.vcs_position.Set_Position(temp_vcs_position.x, temp_vcs_position.y);

               // Calculate compensated range rate for use by measurement updating function
               const float32_t aliased_range_rate_compensated =
                  det_raw.range_rate -
                  (-sensor.vcs_velocity.longitudinal * det_prop.cos_vcs_az - sensor.vcs_velocity.lateral * det_prop.sin_vcs_az);

               det_rdot_comp_array[det_idx] = aliased_range_rate_compensated + (rr_interval * sensor.v_wrapping[sensor.look_id]);
               det_score_array[det_idx]     = temp_score;

               // Clear countermeasure flags for associated detections
               det_prop.wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

               if (prev_assoc_obj_idx > -1)
               {
                  bool is_contained = false;
                  // Detection have already associated to a previous object but now has a better association score to the current
                  // object.
                  if (object_tracks[prev_assoc_obj_idx].movable_prob > MOVABLE_PROB_THRESHOLD)
                  {
                     is_contained = object_tracks[prev_assoc_obj_idx].bbox.Contains(prev_vcs_position);
                  }
                  else
                  {
                     is_contained = object_tracks[prev_assoc_obj_idx].bbox.Circle_Contains(prev_vcs_position);
                  }
                  if (is_contained)
                  {
                     // If detection is inside the bounding box of the previous associated object we disregard the detection
                     // completely
                     det_prop.object_track_id = 0;
                     det_score_array[det_idx] = INFTY;
                     det_prop.f_ok_to_use     = false;
                  }
               }
            }
            else
            {
               bool is_contained = false;
               // Detection have already associated to a previous object but now has a better association score to the current
               // object.
               if (obj.movable_prob > MOVABLE_PROB_THRESHOLD)
               {
                  is_contained = obj.bbox.Contains(det_prop.vcs_position);
               }
               else
               {
                  is_contained = obj.bbox.Circle_Contains(det_prop.vcs_position);
               }
               if (is_contained)
               {
                  // If detection is inside the bounding box of the current object that wants to associate we disregard the
                  // detection completely
                  det_prop.object_track_id = 0;
                  det_score_array[det_idx] = INFTY;
                  det_prop.f_ok_to_use     = false;
               }
            }
         }
      }
   }
}

} // namespace f360_variant_A
